#!/usr/bin/env python
# -*- coding: utf-8 -*-

from jsk_robot_startup.msg import Email
import rosgraph
import rosnode
import rospy
from std_msgs.msg import Int16
import subprocess
from socket import error as socket_error

try:
    from xmlrpc.client import ServerProxy
except ImportError:
    from xmlrpclib import ServerProxy


class EmailSpotCooler(object):
    """
    This class receives /moisture topic
    and calculates the amount of the water in the spot cooler tank.

    This class daily sends email according to the task status.

    Regularly, it resets serial_node.py
    This is because the connection between serial_node.py
    and M5StickC is terminated by deepsleep in M5StickC.
    """

    def __init__(self):
        self.rosserial_name = rospy.get_param('~rosserial_name')
        self.last_communication = rospy.Time.now()
        # Subscribe moisture
        rospy.Subscriber('moisture', Int16, self.moisture_cb)
        self.moisture = 4096  # 0: most moist, 4095: least moist
        # Publish email
        self.pub = rospy.Publisher('email', Email, queue_size=1)
        # Timer callback to send email every 24 hours
        rospy.Timer(rospy.Duration(24 * 60 * 60), self.send_email)

    def moisture_cb(self, msg):
        self.moisture = msg.data
        rospy.loginfo('I got moisture data: {}'.format(self.moisture))
        rospy.last_communication = rospy.Time.now()
        # Reset rosserial because the connection is terminated by M5StickC
        # after M5StickC sends topic
        self.reset_rosserial()

    def send_email(self, event):
        email_msg = Email()
        now = rospy.Time.now()
        email_msg.header.stamp = now
        email_msg.subject = 'スポットクーラーのタンクの水量'
        body = ''
        # Check amount of the water
        if self.moisture < 3000:
            body += 'タンクに水が溜まっています。交換してください。\n'
            body += 'moisture: {}'.format(self.moisture)

        else:
            body += 'タンクに水は溜まっていません。\b'
            body += 'moisture: {}'.format(self.moisture)
        # Check communication status
        elapsed_time = rospy.Time.now() - self.last_communication
        if (elapsed_time.secs > 24 * 60 * 60):
            body += '1日以上、M5StickCと通信が出来ていません。情報が古い可能性があります。'
        # TODO: Check M5StickC battery
        email_msg.body = body
        self.pub.publish(email_msg)
        rospy.loginfo('Send email')
        # Reset rosserial regularly to avoid failing communication eternally
        self.reset_rosserial()

    # Reset rosserial by sending SIGTERM to rosserial
    # See https://answers.ros.org/question/271776/how-can-i-retrieve-a-list-of-process-ids-of-ros-nodes/  # NOQA
    # rosnode.kill_nodes() cannot kill serial_node.py absolutely
    def reset_rosserial(self):
        ID = '/rosnode'
        master = rosgraph.Master(ID)
        node_api = rosnode.get_api_uri(
            master, self.rosserial_name, skip_cache=True)
        node = ServerProxy(node_api)
        try:
            pid = rosnode._succeed(node.getPid(ID))
        except socket_error as serr:
            rospy.logerr(serr)
            rospy.logerr('Skip killing rosserial')
        else:
            # To kill rosserial completely, call kill twice
            subprocess.call(['kill', str(pid)])
            rospy.sleep(5)
            subprocess.call(['kill', str(pid)])
            rospy.loginfo('Reset rosserial by sending SIGTERM to rosserial')


if __name__ == '__main__':
    rospy.init_node('email_spot_cooler')
    esc = EmailSpotCooler()
    rospy.spin()
