#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <fstream>
#include <chrono>
#include <thread>

// 接收/joint_states/position的话题消息。10s后，自动停止接收消息，并保存到同一工作空间目录下的joint_states.txt，在读取txt文件内容，保存到四个数组中

std::vector<double> array1, array2, array3, array4;

void jointStatesCallback(const sensor_msgs::JointState::ConstPtr& msg) {
    std::ofstream file("joint_states.txt", std::ios_base::app);
    if (!file.is_open()) {
        ROS_ERROR_STREAM("Could not open the file.");
        return;
    }

    for (size_t i = 0; i < msg->position.size(); ++i) {
        file << msg->position[i];
        if (i < msg->position.size() - 1)
            file << ", ";
    }
    file << std::endl;

    file.close();
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "joint_states_subscriber");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/joint_states", 10, jointStatesCallback);

    ros::Rate rate(10); // 10 Hz

    // Wait for 10 seconds
    for (int i = 0; i < 100; ++i) {
        ros::spinOnce();
        rate.sleep();
    }

    // Read data from file
    std::ifstream file("joint_states.txt");
    if (!file.is_open()) {
        ROS_ERROR_STREAM("Could not open the file.");
        return -1;
    }

    std::string line;
    while (std::getline(file, line)) {
        double val1, val2, val3, val4;
        if (sscanf(line.c_str(), "%lf, %lf, %lf, %lf", &val1, &val2, &val3, &val4) == 4) {
            array1.push_back(val1);
            array2.push_back(val2);
            array3.push_back(val3);
            array4.push_back(val4);
        }
    }

    // Print the arrays
    std::cout << "joint1: ";
    for (double val : array1)
        std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "joint2: ";
    for (double val : array2)
        std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "joint3: ";
    for (double val : array3)
        std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "joint4: ";
    for (double val : array4)
        std::cout << val << " ";
    std::cout << std::endl;

    return 0;
}
