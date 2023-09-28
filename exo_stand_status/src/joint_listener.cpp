#include <ros/ros.h>
#include <iostream>
#include <fstream>
#include <vector>

// 将txt文件中的四个joint值，一次存入array1、array2、array3、array4
// 同时可以滤除四个joint值都为0的情况

int main(int argc, char** argv)
{
    ros::init(argc, argv, "data_processing_node");
    ros::NodeHandle nh;

    std::string file_path = "/home/hhz/joint_new.txt";  // file path

    std::ifstream file(file_path);
    if (!file.is_open())
    {
        ROS_ERROR_STREAM("Could not open the file: " << file_path);
        return -1;
    }

    std::vector<float> array1, array2, array3, array4;
    std::string line;
    while (std::getline(file, line))
    {
        if (line == "---")
            continue;

        float val1, val2, val3, val4;
        int num_matched = sscanf(line.c_str(), "[%f, %f, %f, %f]", &val1, &val2, &val3, &val4);
        if (num_matched == 4 && (val1+val2+val3+val4) != 0)//当4个数都是0时，表示还没有进行规划，所以要滤除
        {
            array1.push_back(val1);
            array2.push_back(val2);
            array3.push_back(val3);
            array4.push_back(val4);
        }
    }

    // Print the arrays
    std::cout << "joint1: ";
    for (float val : array1)
        std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "joint2: ";
    for (float val : array2)
        std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "joint3: ";
    for (float val : array3)
        std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "joint4: ";
    for (float val : array4)
        std::cout << val << " ";
    std::cout << std::endl;

    return 0;
}
