#!/usr/bin/env python
# -*- coding: utf-8 -*-

from jsk_robot_startup.msg import EmailBody
from m5stack_ros import EmailRosserial, EARTH
import rospy


class EmailSpotCooler(EmailRosserial):
    """
    This class receives /moisture topic
    and calculates the amount of the water in the spot cooler tank.
    The result is sent via email.
    """

    def __init__(self):
        super(EmailSpotCooler, self).__init__()
        self.earth = EARTH()
        self.modules.append(self.earth)
        self.subject = 'スポットクーラーのタンクの水量'

    # When full water, low battery or next day, send email
    def check_status(self, event):
        # Send email as soon as possible when the water is full
        if self.earth.moisture is not None and \
           self.earth.moisture < self.earth.moisture_thre:
            self.send_email()
            rospy.loginfo('Send email because tank water is full')
        # send email when low battery or next day
        super(EmailSpotCooler, self).check_status(event)

    # Check amount of the water in the tank
    def water_email_body(self):
        message = self.earth.message()
        email_body = EmailBody()
        email_body.type = 'text'
        email_body.message = message
        return email_body

    def create_email_body(self):
        body = super(EmailSpotCooler, self).create_email_body()
        body.append(self.water_email_body())
        return body


if __name__ == '__main__':
    rospy.init_node('email_spot_cooler')
    esc = EmailSpotCooler()
    rospy.spin()
