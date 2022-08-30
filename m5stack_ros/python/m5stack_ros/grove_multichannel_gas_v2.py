# -*- coding: utf-8 -*-
from m5stack_ros import RosserialModule
from std_msgs.msg import UInt16
import rospy


class GroveMultiChannelGas(RosserialModule):
    def __init__(self):
        rosserial_name = rospy.get_param(
            '~grove_multi_channel_gas_v2_rosserial_name', None)
        super(GroveMultiChannelGas, self).__init__(rosserial_name)
        # Gas info
        self.gas_info = [
            {'Name': 'NO2', 'Topic': 'gas_v2_102b', 'Concentration': None},
            {'Name': 'C2H5CH', 'Topic': 'gas_v2_302b', 'Concentration': None},
            {'Name': 'VOC', 'Topic': 'gas_v2_502b', 'Concentration': None},
            {'Name': 'CO', 'Topic': 'gas_v2_702b', 'Concentration': None}]
        gas_topic_names = [gas['Topic'] for gas in self.gas_info]
        for gas_topic_name in gas_topic_names:
            rospy.Subscriber(
                gas_topic_name, UInt16, self.gas_cb, gas_topic_name)
        self.module_name = 'GroveMultiChannelGas'

    def gas_cb(self, msg, gas_topic_name):
        for gas_info in self.gas_info:
            if gas_info['Topic'] == gas_topic_name:
                gas_info['Concentration'] = msg.data
        self.update_last_communication()

    # Check M5 device battery
    def message(self, index):
        message = ''
        gas_info = self.gas_info[index]
        if gas_info['Concentration'] is None:
            message = '{}のデータは届いていません\n'.format(
                gas_info['Name'])
        else:
            message = '{}の濃度は{}です\n'.format(
                gas_info['Name'], gas_info['Concentration'])

        return message
