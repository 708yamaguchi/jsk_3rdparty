# Mainly copied from
# https://github.com/Abandon-ht/ModuleLLM_Development_Guide/blob/fb7f6871dcb2f23f6d74e88c65a29dee43f67988/PC/python/llm-qwen2.5-1B.py

import argparse
import json
import socket

import rospy
from speech_recognition_msgs.msg import SpeechRecognitionCandidates
from std_msgs.msg import String


class TCPClient:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sock = None

    def connect(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))

    def send_json(self, data):
        json_data = json.dumps(data, ensure_ascii=False) + '\n'
        self.sock.sendall(json_data.encode('utf-8'))

    def receive_response(self):
        response = ''
        while True:
            part = self.sock.recv(4096).decode('utf-8')
            response += part
            if '\n' in response:
                break
        return response.strip()

    def close(self):
        if self.sock:
            self.sock.close()


class LLMClient:
    def __init__(self, tcp_client):
        self.tcp_client = tcp_client
        self.work_id = None

    @staticmethod
    def create_init_data():
        return {
            "request_id": "llm_001",
            "work_id": "llm",
            "action": "setup",
            "object": "llm.setup",
            "data": {
                "model": "qwen2.5-1.5B-ax630c",
                "response_format": "llm.utf-8.stream",
                "input": "llm.utf-8.stream",
                "enoutput": True,
                "max_token_len": 1023,
                "prompt": "あなたはユーザーアシスタントです。ユーザの入力のうち、以下の言葉に最も近いものを、単語だけで返してください。近いものがなければNoneを返してください。「サーボオン」「サーボオフ」「教示開始」「教示終了」「再生開始」「再生終了」"
            }
        }

    def setup(self):
        init_data = self.create_init_data()
        self.tcp_client.send_json(init_data)
        response = self.tcp_client.receive_response()
        response_data = json.loads(response)
        self.work_id = self._parse_setup_response(response_data, init_data['request_id'])

    def _parse_setup_response(self, response_data, sent_request_id):
        error = response_data.get('error')
        request_id = response_data.get('request_id')

        if request_id != sent_request_id:
            print(f"Request ID mismatch: sent {sent_request_id}, received {request_id}")
            return None

        if error and error.get('code') != 0:
            print(f"Error Code: {error['code']}, Message: {error['message']}")
            return None

        return response_data.get('work_id')

    def send_inference_request(self, user_input):
        self.tcp_client.send_json({
            "request_id": "llm_001",
            "work_id": self.work_id,
            "action": "inference",
            "object": "llm.utf-8.stream",
            "data": {
                "delta": user_input,
                "index": 0,
                "finish": True
            }
        })

    def handle_inference_response(self):
        full_response = ""
        while True:
            response = self.tcp_client.receive_response()
            response_data = json.loads(response)
            data = self._parse_inference_response(response_data)
            if data is None:
                break

            full_response += data.get('delta')
            if data.get('finish'):
                break

        return full_response

    def _parse_inference_response(self, response_data):
        error = response_data.get('error')
        if error and error.get('code') != 0:
            print(f"Error Code: {error['code']}, Message: {error['message']}")
            return None
        return response_data.get('data')

    def exit_session(self):
        deinit_data = {
            "request_id": "llm_exit",
            "work_id": self.work_id,
            "action": "exit"
        }
        self.tcp_client.send_json(deinit_data)
        response = self.tcp_client.receive_response()
        response_data = json.loads(response)
        print("Exit Response:", response_data)


class ROSLLMBridge:
    def __init__(self, llm_client):
        self.llm_client = llm_client
        rospy.Subscriber('/speech_to_text', SpeechRecognitionCandidates, self.speech_to_text_callback)
        self.keyword_pub = rospy.Publisher('/text_to_keyword', String, queue_size=10)

    def speech_to_text_callback(self, msg):
        if not msg.transcript:
            rospy.logwarn("Received empty transcript.")
            return

        # Use the first transcript with the highest confidence
        user_input = msg.transcript[0]
        rospy.loginfo(f"Received input from /speech_to_text: {user_input}")

        # Send input to LLM and publish the response
        self.llm_client.send_inference_request(user_input)
        inference_response = self.llm_client.handle_inference_response()
        if inference_response:
            self.keyword_pub.publish(String(data=inference_response))
            rospy.loginfo(f"Published to /text_to_keyword: {inference_response}")


def main(host, port):
    tcp_client = TCPClient(host, port)
    llm_client = LLMClient(tcp_client)

    try:
        tcp_client.connect()
        print("Setup LLM...")
        llm_client.setup()
        print("Setup LLM finished.")

        # while True:
        #     user_input = input("Enter your message (or 'exit' to quit): ")
        #     if user_input.lower() == 'exit':
        #         break

        #     llm_client.send_inference_request(user_input)
        #     llm_client.handle_inference_response()

        ROSLLMBridge(llm_client)
        rospy.spin()

    finally:
        llm_client.exit_session()
        tcp_client.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='TCP Client to send JSON data.')
    parser.add_argument('--host', type=str, default='localhost', help='Server hostname (default: localhost)')
    parser.add_argument('--port', type=int, default=10001, help='Server port (default: 10001)')

    args = parser.parse_args()
    main(args.host, args.port)
