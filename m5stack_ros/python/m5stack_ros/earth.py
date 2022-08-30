# -*- coding: utf-8 -*-
from m5stack_ros import RosserialModule
from std_msgs.msg import Int16
import rospy


class EARTH(RosserialModule):
    def __init__(self):
        rosserial_name = rospy.get_param('~earth_rosserial_name', None)
        super(EARTH, self).__init__(rosserial_name)
        # Subscribe moisture
        rospy.Subscriber('moisture', Int16, self.moisture_cb)
        self.moisture = None  # 0: most moist, 4095: least moist
        self.moisture_thre = 3000

    def moisture_cb(self, msg):
        self.moisture = msg.data
        rospy.loginfo('I got moisture data: {}'.format(self.moisture))
        self.last_communication = rospy.Time.now()

    def message(self):
        message = ''
        if self.moisture is None or self.moisture > self.moisture_thre:
            message += 'タンクに水は溜まっていません。\n'
        else:
            message += 'タンクに水が溜まっています。交換してください。\n'
        message += '水分量 {} （基準値3000）\n'.format(self.moisture)
        return message
