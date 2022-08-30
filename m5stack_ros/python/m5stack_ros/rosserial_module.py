# -*- coding: utf-8 -*-
import rospy
from datetime import datetime
import rosgraph
import rosnode
import subprocess
from socket import error as socket_error

try:
    from xmlrpc.client import ServerProxy
except ImportError:
    from xmlrpclib import ServerProxy


class RosserialModule(object):
    # If rosserial_name is not given, reset_rosserial() does not work
    def __init__(self, rosserial_name=None):
        self.last_communication = None
        self.module_name = 'rosserial module'
        self.rosserial_name = rosserial_name

    def update_last_communication(self):
        self.last_communication = rospy.Time.now()

    # Check communication status
    def comm_message(self):
        message = ''
        if self.last_communication is None:
            message += 'まだ{}と通信が出来ていません。\n'.format(self.module_name)
        elif (rospy.Time.now() - self.last_communication).secs > 24 * 60 * 60:
            message += '1日以上、{}と通信が出来ていません。情報が古い可能性があります。\n'.format(
                self.module_name)
        else:
            unix_time_for_jst = self.last_communication.secs + (9 * 60 * 60)
            dt = datetime.utcfromtimestamp(unix_time_for_jst)
            message += '最後に通信した時刻 {} (JST)\n'.format(dt)
        return message

    # Reset rosserial by sending SIGTERM to rosserial
    # See https://answers.ros.org/question/271776/how-can-i-retrieve-a-list-of-process-ids-of-ros-nodes/  # NOQA
    # rosnode.kill_nodes() cannot kill serial_node.py absolutely
    def reset_rosserial(self):
        if self.rosserial_name is None:
            return
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
