# -*- coding: utf-8 -*-
from m5stack_ros import RosserialModule
from std_msgs.msg import UInt16
import rospy


class ToF(RosserialModule):
    def __init__(self):
        rosserial_name = rospy.get_param(
            '~tof_rosserial_name', None)
        super(ToF, self).__init__(rosserial_name)
        # ToF info
        rospy.Subscriber('tof', UInt16, self.tof_cb)
        self.tof = None
        self.module_name = 'GroveMultiChannelGas'

    def tof_cb(self, msg):
        self.tof = msg.data
        self.update_last_communication()

    # Check M5 device battery
    def message(self, index):
        message = ''
        if self.tof is not None:
            message = 'ToFデータは{}です。'.format(self.tof)
        return message
