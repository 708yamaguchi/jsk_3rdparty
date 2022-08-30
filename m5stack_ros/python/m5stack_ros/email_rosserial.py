# -*- coding: utf-8 -*-

from jsk_robot_startup.msg import Email, EmailBody
from m5stack_ros import Battery
import rospy


class EmailRosserial(object):
    """
    This is base class to daily sends email according to the M5 device status.

    To build a new mail notification system by inheriting this class, you need
      - Create an instance of a module that communicates via rosserial
        by inheriting from the RosserialModule class (e.g. self.battery)
      - Override create_mail_body() based on the above created instance
      - Override check_status() based on the above created instance
      - Set self.modules variables as the list of the above create instance.
      - Update self.subject, self.sender_address and self.receiver_address
    """
    def __init__(self):
        # Battery module class
        self.battery = Battery()
        self.modules = [self.battery]
        # Publish email
        self.email_duration = rospy.get_param('~email_duration', 1 * 60 * 60)
        self.pub = rospy.Publisher('email', Email, queue_size=1)
        self.last_send_email = None
        # Check status of M5 device and sensor and send email if needed
        self.check_duration = rospy.get_param('~check_duration', 30)
        rospy.Timer(rospy.Duration(self.check_duration), self.check_status)
        # Override these variables in child class
        self.subject = 'Subject'
        self.sender_address = ''
        self.receiver_address = ''

    def send_email(self):
        email_msg = Email()
        now = rospy.Time.now()
        email_msg.header.stamp = now
        email_msg.subject = self.subject
        email_msg.sender_address = self.sender_address
        email_msg.receiver_address = self.receiver_address
        email_body = self.create_email_body()
        email_msg.body = email_body
        # Publish Email
        self.pub.publish(email_msg)
        self.last_send_email = rospy.Time.now()
        rospy.loginfo('Send email')
        # Reset rosserial regularly to avoid failing communication eternally
        for module in self.modules:
            module.reset_rosserial()

    def create_email_body(self):
        """
        return list of EmailBody, each of which you want to write in the email.
        """
        email_body = EmailBody()
        email_body.type = 'text'
        message = ''
        message += self.battery.comm_message()
        message += self.battery.message()
        email_body.message = message
        return [email_body]

    # Send email if M5 battery is low
    # or email has not been sent for self.email_duration[s]
    def check_status(self, event):
        """
        Check M5 device status and send email if needed.
        """
        rospy.loginfo('Check status of M5 device')
        if self.battery.low_bat:
            self.send_email()
            rospy.loginfo('Send email because battery is low')
            return
        if self.last_send_email is None:
            self.send_email()
            rospy.loginfo('Send email at first time')
            return
        secs_from_last_email = (rospy.Time.now() - self.last_send_email).secs
        if secs_from_last_email > self.email_duration:
            self.send_email()
            rospy.loginfo(
                'Send email because it has not been sent for {} [s]'.format(
                    self.email_duration))
        else:
            rospy.loginfo('Timer is called, but do not send email')
