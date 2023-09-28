#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>

// C++正运动学fk规划，提供关节角度，实现外骨骼机器人的运动

int main(int argc, char **argv)
{
    /* code */
    ros::init(argc, argv, "exo_stand_fk");
    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface left("left_group");
    moveit::planning_interface::MoveGroupInterface right("right_group");

    left.setGoalJointTolerance(0.001);
    right.setGoalJointTolerance(0.001);

    left.setMaxAccelerationScalingFactor(0.2);
    left.setMaxVelocityScalingFactor(0.2);
    right.setMaxAccelerationScalingFactor(0.2);
    right.setMaxVelocityScalingFactor(0.2);

    // 控制机械臂先回到初始化位置
    left.setNamedTarget("left_home");
    right.setNamedTarget("right_home");

    left.move();
    right.move();
    sleep(1);

    double targetPose[4] = {-0.373, 0, -0.772, 0.7416};
    std::vector<double> right_joint_positions(2);
    std::vector<double> left_joint_positions(2);
    right_joint_positions[0] = targetPose[0];
    right_joint_positions[1] = targetPose[1];
    left_joint_positions[0] = targetPose[2];
    left_joint_positions[1] = targetPose[3];

    right.setJointValueTarget(right_joint_positions);
    left.setJointValueTarget(left_joint_positions);
    
    left.move();
    right.move();
    sleep(1);

    // 控制机械臂先回到初始化位置
    left.setNamedTarget("left_home");
    right.setNamedTarget("right_home");

    left.move();
    right.move();
    sleep(1);

    ros::shutdown();

    return 0;
}
