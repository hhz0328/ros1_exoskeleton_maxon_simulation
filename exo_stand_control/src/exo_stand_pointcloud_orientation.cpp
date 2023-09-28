#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/robot_trajectory/robot_trajectory.h>

//外骨骼左腿作为演示，环境的搭建见图片 exo_stand_pointcloud_orientation.cpp环境搭建.png
// 使用前先启动点云  roslaunch exo_stand_moveit_config exo_stand_intelD435i_obstacle_pointcloud.launch

void scale_trajectory_speed(moveit::planning_interface::MoveGroupInterface::Plan &plan, double scale)//轨迹重定义
{
    int n_joints = plan.trajectory_.joint_trajectory.joint_names.size();

    for(int i=0; i<plan.trajectory_.joint_trajectory.points.size(); i++)
    {
        plan.trajectory_.joint_trajectory.points[i].time_from_start *= 1/scale;
        
        for(int j=0; j<n_joints; j++)
        {
            plan.trajectory_.joint_trajectory.points[i].velocities[j] *= scale;
            plan.trajectory_.joint_trajectory.points[i].accelerations[j] *= scale*scale;
        }
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "moveit_revise_trajectory_demo");
    ros::NodeHandle node_handle; 
    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface left("left_group");
    moveit::planning_interface::MoveGroupInterface right("right_group");

    left.setGoalJointTolerance(0.001);
    right.setGoalJointTolerance(0.001);

    left.setMaxAccelerationScalingFactor(0.5);
    left.setMaxVelocityScalingFactor(0.5);
    right.setMaxAccelerationScalingFactor(0.5);
    right.setMaxVelocityScalingFactor(0.5);

    // 控制机械臂先回到初始化位置
    left.setNamedTarget("left_home");
    right.setNamedTarget("right_home");

    left.move();
    right.move();

    sleep(1);


    double targetPose[4] = {-0.9526, 1.1956, -1.2026, 1.1956};
    std::vector<double> left_joint_positions1(2);
    std::vector<double> left_joint_positions2(2);
    left_joint_positions1[0] = targetPose[0];
    left_joint_positions1[1] = targetPose[1];
    left_joint_positions2[0] = targetPose[2];
    left_joint_positions2[1] = targetPose[3];

    left.setJointValueTarget(left_joint_positions1);
    left.move();
    sleep(1);

    left.setJointValueTarget(left_joint_positions2);
    left.move();
    sleep(0.2);

    // 控制机械臂先回到初始化位置
    left.setNamedTarget("left_home");
    left.move();
    sleep(0.2);


    left.setJointValueTarget(left_joint_positions1);
    moveit::planning_interface::MoveGroupInterface::Plan plan_left1;
    moveit::planning_interface::MoveItErrorCode success1 = left.plan(plan_left1);
    ROS_INFO("Plan (pose goal) %s",success1?"":"FAILED");      
    scale_trajectory_speed(plan_left1, 0.3);//调用最上方的轨迹重定义函数
    //让机械臂按照规划的轨迹开始运动。
    if(success1)
      left.execute(plan_left1);
    sleep(0.2);

    left.setJointValueTarget(left_joint_positions2);
    moveit::planning_interface::MoveGroupInterface::Plan plan_left2;
    moveit::planning_interface::MoveItErrorCode success2 = left.plan(plan_left2);
    ROS_INFO("Plan (pose goal) %s",success2?"":"FAILED");      
    scale_trajectory_speed(plan_left2, 0.3);//调用最上方的轨迹重定义函数
    //让机械臂按照规划的轨迹开始运动。
    if(success2)
      left.execute(plan_left2);
    sleep(0.2);

    // 控制机械臂先回到初始化位置
    left.setNamedTarget("left_home");
    right.setNamedTarget("right_home");

    left.move();
    right.move();
    sleep(1);

    ros::shutdown(); 

    return 0;
}
