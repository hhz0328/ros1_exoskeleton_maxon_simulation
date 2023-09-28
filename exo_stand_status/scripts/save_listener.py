#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# 接收/joint_states/position的话题消息。10s后，自动停止接收消息，并保存到同一工作空间目录下的joint_states.txt，在读取txt文件内容，保存到四个数组中

import rospy
from sensor_msgs.msg import JointState
import time

array1 = []
array2 = []
array3 = []
array4 = []

def joint_states_callback(msg):
    global array1, array2, array3, array4
    with open("joint_states.txt", "a") as file:
        for i in range(len(msg.position)):
            file.write(str(msg.position[i]))
            if i < len(msg.position) - 1:
                file.write(", ")
        file.write("\n")

def main():
    rospy.init_node("joint_states_subscriber")
    sub = rospy.Subscriber("/joint_states", JointState, joint_states_callback)
    
    rate = rospy.Rate(10)  # 10 Hz
    
    start_time = rospy.Time.now()
    duration = rospy.Duration(10.0)  # 10 seconds
    
    while rospy.Time.now() - start_time < duration:
        rate.sleep()
    
    # Read data from file
    with open("joint_states.txt", "r") as file:
        for line in file:
            values = line.strip().split(", ")
            if len(values) == 4:
                array1.append(float(values[0]))
                array2.append(float(values[1]))
                array3.append(float(values[2]))
                array4.append(float(values[3]))
    
    # Print the arrays
    print("joint1:", " ".join(map(str, array1)))
    print("joint2:", " ".join(map(str, array2)))
    print("joint3:", " ".join(map(str, array3)))
    print("joint4:", " ".join(map(str, array4)))

if __name__ == "__main__":
    main()