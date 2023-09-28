#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# 将txt文件中的四个joint值，一次存入array1、array2、array3、array4
import rospy

def read_and_store_data(file_path):
    array1 = []
    array2 = []
    array3 = []
    array4 = []

    with open(file_path, 'r') as file:
        content = file.read()

    data_list = [data.strip() for data in content.split('---') if data.strip()]

    for data in data_list:
        data_values = [float(val.strip()) for val in data.strip('[\n]').split(',')]
        if len(data_values) == 4:
            array1.append(data_values[0])
            array2.append(data_values[1])
            array3.append(data_values[2])
            array4.append(data_values[3])

    return array1, array2, array3, array4

if __name__ == '__main__':
    rospy.init_node('data_processing_node', anonymous=True)
    
    file_path = "/home/hhz/joint_new.txt"  # file path
    
    array1, array2, array3, array4 = read_and_store_data(file_path)

    print("joint1: ", array1[225])
    print("joint2: ", array2)
    print("joint3: ", array3[225])
    print("joint4: ", array4)
    
    rospy.spin()
