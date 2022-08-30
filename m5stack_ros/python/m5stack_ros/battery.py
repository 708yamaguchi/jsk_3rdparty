# -*- coding: utf-8 -*-
from m5stack_ros import RosserialModule
from std_msgs.msg import Float32
import rospy


class Battery(RosserialModule):
    def __init__(self):
        rosserial_name = rospy.get_param('~battery_rosserial_name', None)
        super(Battery, self).__init__(rosserial_name)
        # Battery info
        rospy.Subscriber('battery_level', Float32, self.battery_level_cb)
        self.low_bat = False
        self.bat_level = None
        self.low_bat_thre = 3.6
        self.module_name = 'バッテリ管理モジュール'

    def battery_level_cb(self, msg):
        self.bat_level = msg.data
        rospy.loginfo('I got battery_level data: {}'.format(self.bat_level))
        if self.bat_level < self.low_bat_thre:
            self.low_bat = True
        else:
            self.low_bat = False
        self.update_last_communication()
        # Reset rosserial because the connection is terminated by M5StickC
        # after M5StickC sends topic
        rospy.sleep(3)  # Wait for other callback functions to exit
        self.reset_rosserial()

    # Check M5 device battery
    def message(self):
        message = ''
        if self.bat_level is None:
            message += ''
        elif self.low_bat:
            message += 'バッテリ残量はわずかです({}[V])。充電してください。\n'.format(
                self.bat_level)
        else:
            message += 'バッテリ残量は十分です({}[V])。\n'.format(
                self.bat_level)
        return message
