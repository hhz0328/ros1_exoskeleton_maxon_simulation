#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# 订阅/joint_states/position消息，并保存为txt文件

import rospy
from sensor_msgs.msg import JointState

def joint_states_callback(msg):
    with open('/home/hhz/msgs/joint_states.txt', 'a') as file:
        position_str = ', '.join(str(val) for val in msg.position)
        file.write(position_str + '\n')

if __name__ == '__main__':
    rospy.init_node('joint_states_subscriber', anonymous=True)
    rospy.Subscriber('/joint_states', JointState, joint_states_callback)
    rospy.spin()