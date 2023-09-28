#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <fstream>

// 订阅/joint_states/position消息，并保存为txt文件

void jointStatesCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
    std::ofstream file("/home/hhz/msgs/joint_states2.txt", std::ios_base::app);
    if (!file.is_open())
    {
        ROS_ERROR_STREAM("Could not open the file.");
        return;
    }

    for (size_t i = 0; i < msg->position.size(); ++i)
    {
        file << msg->position[i];
        if (i < msg->position.size() - 1)
            file << ", ";
    }
    file << std::endl;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "joint_states_subscriber");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/joint_states", 10, jointStatesCallback);

    ros::spin();
    
    return 0;
}
