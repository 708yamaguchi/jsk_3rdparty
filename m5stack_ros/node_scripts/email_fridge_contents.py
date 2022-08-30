#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cv2
from cv_bridge import CvBridge
from jsk_robot_startup.msg import EmailBody
from m5stack_ros import EmailRosserial, GroveMultiChannelGas, ToF
import os
import rospy
from sensor_msgs.msg import Image


class EmailFridgeContents(EmailRosserial):
    """
    This class receives /timer_cam_image topic and send email with the image
    The purpose is to keep the M5 TimerCam in the refrigerator
    and monitor the contents regularly.
    """

    def __init__(self):
        super(EmailFridgeContents, self).__init__()
        self.device_name = 'M5TimerCam'
        self.subject = '冷蔵庫の中の状態'
        # Subscribers
        rospy.Subscriber('timer_cam_image', Image, self.image_cb)
        self.img_file_path = '/tmp/email_fridge_contents.png'

        self.gas = GroveMultiChannelGas()
        self.tof = ToF()
        self.tof_threshold = 30
        # Do not send old image
        if os.path.exists(self.img_file_path):
            os.remove(self.img_file_path)

    def image_cb(self, msg):
        self.image = msg
        bridge = CvBridge()
        img = bridge.imgmsg_to_cv2(msg)
        cv2.imwrite(self.img_file_path, img)
        self.update_last_communication()

    # When door is left open, low battery or next day, send email
    def check_status(self, event):
        if self.tof is not None and self.tof > self.tof_threshold:
            self.send_email()
            rospy.loginfo('Send email because the fridge door is left open')
        super(EmailFridgeContents, self).check_status(event)

    # Check the contents in the fridge by camera
    def email_image_body(self):
        email_body = EmailBody()
        if os.path.exists(self.img_file_path):
            email_body.type = 'img'
            email_body.message = '冷蔵庫の中身の写真です\n'
            email_body.file_path = self.img_file_path
            email_body.img_size = 100
        else:
            email_body.type = 'text'
            email_body.message = '冷蔵庫の中身の写真は届いていません\n'
        email_body.message += '\n'  # end of this section
        return email_body

    # Check the gas level in the fridge
    def email_gas_body(self, index):
        email_body = EmailBody()
        email_body.type = 'text'
        email_body.message = self.gas.message(index)
        return email_body

    # Check the fridge door state by tof
    def email_tof_body(self):
        email_body = EmailBody()
        email_body.type = 'text'
        email_body.message = self.tof.message()
        if self.tof.tof is None:
            email_body.message = '冷蔵庫のToFのデータは届いていません\n'
        else:
            if self.tof.tof > self.tof_threshold:
                email_body.message += '冷蔵庫の扉が開いたままです。(ToF: {})\n'.format(
                    self.tof.tof)
            else:
                email_body.message += '冷蔵庫の扉は閉じられています。(ToF: {})\n'.format(
                    self.tof.tof)
        email_body.message += '\n'  # end of this section
        return email_body

    def create_email_body(self):
        body = super(EmailFridgeContents, self).create_email_body()
        for i in range(len(self.gas_info)):
            body.append(self.email_gas_body(i))
        body.append(self.email_tof_body())
        body.append(self.email_image_body())
        return body

    def send_email(self):
        super(EmailFridgeContents, self).send_email()
        # Do not send the same image twice
        if os.path.exists(self.img_file_path):
            os.remove(self.img_file_path)


if __name__ == '__main__':
    rospy.init_node('email_spot_cooler')
    efc = EmailFridgeContents()
    rospy.spin()
