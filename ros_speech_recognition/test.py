import rospy
from speech_recognition_msgs.msg import SpeechRecognitionCandidates

a = SpeechRecognitionCandidates()
a.transcript = ["今から動作を教えます"]

rospy.init_node('hoge')
b = rospy.Publisher('/speech_to_text', SpeechRecognitionCandidates, queue_size=1)
while not rospy.is_shutdown():
    rospy.sleep(5)
    b.publish(a)
