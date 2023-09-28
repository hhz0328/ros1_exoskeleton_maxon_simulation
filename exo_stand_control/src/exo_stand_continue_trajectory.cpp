#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/robot_trajectory/robot_trajectory.h>
#include <moveit/trajectory_processing/iterative_time_parameterization.h>
#include <moveit_msgs/OrientationConstraint.h>

// 实现多轨迹连续规划，多轨迹的拼接过程中速度不为零

int main(int argc, char **argv)
{
    ros::init(argc, argv, "exo_stand_continue_trajectory");
    ros::NodeHandle node_handle; 
    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface left("left_group");
    moveit::planning_interface::MoveGroupInterface right("right_group");

    left.setGoalJointTolerance(0.001);
    right.setGoalJointTolerance(0.001);

    double accScale = 0.5;
    double velScale = 0.5;
    left.setMaxAccelerationScalingFactor(accScale);
    left.setMaxVelocityScalingFactor(velScale);
    right.setMaxAccelerationScalingFactor(accScale);
    right.setMaxVelocityScalingFactor(velScale);

    // 控制机械臂先回到初始化位置
    left.setNamedTarget("left_home");
    right.setNamedTarget("right_home");
    left.move();
    right.move();
    sleep(1);

    // 获取机器人的起始位置
    moveit::core::RobotStatePtr start_state(left.getCurrentState());
    const robot_state::JointModelGroup *joint_model_group = start_state->getJointModelGroup(left.getName());

    // std::vector<double> joint_group_positions;
    // start_state->copyJointGroupPositions(joint_model_group, joint_group_positions);


    double targetPose[4] = {-0.9526, 1.1956, -1.2026, 1.1956};
    std::vector<double> left_joint_positions1(2);
    std::vector<double> left_joint_positions2(2);

    left_joint_positions1[0] = targetPose[0];
    left_joint_positions1[1] = targetPose[1];

    left_joint_positions2[0] = targetPose[2];
    left_joint_positions2[1] = targetPose[3];


    //设置第一个目标点
    left.setJointValueTarget(left_joint_positions1);

    // 计算第一条轨迹
    moveit::planning_interface::MoveGroupInterface::Plan plan1;
    moveit::planning_interface::MoveItErrorCode success = left.plan(plan1);

    joint_model_group = start_state->getJointModelGroup(left.getName());    
    start_state->setJointGroupPositions(joint_model_group, left_joint_positions1);
    left.setStartState(*start_state);

    //设置第二个目标点
    left.setJointValueTarget(left_joint_positions2);

    // 计算第二条轨迹
    moveit::planning_interface::MoveGroupInterface::Plan plan2;
    success = left.plan(plan2);

    joint_model_group = start_state->getJointModelGroup(left.getName());    
    start_state->setJointGroupPositions(joint_model_group, left_joint_positions2);
    left.setStartState(*start_state);

    //连接两条轨迹
    moveit_msgs::RobotTrajectory trajectory;

    trajectory.joint_trajectory.joint_names = plan1.trajectory_.joint_trajectory.joint_names;
    trajectory.joint_trajectory.points = plan1.trajectory_.joint_trajectory.points;
    for (size_t j = 1; j < plan2.trajectory_.joint_trajectory.points.size(); j++)
    {
        trajectory.joint_trajectory.points.push_back(plan2.trajectory_.joint_trajectory.points[j]);
    }

    moveit::planning_interface::MoveGroupInterface::Plan joinedPlan;
    robot_trajectory::RobotTrajectory rt(left.getCurrentState()->getRobotModel(), "left_group");
    rt.setRobotTrajectoryMsg(*left.getCurrentState(), trajectory);
    trajectory_processing::IterativeParabolicTimeParameterization iptp;
    iptp.computeTimeStamps(rt, velScale, accScale);

    rt.getRobotTrajectoryMsg(trajectory);
    joinedPlan.trajectory_ = trajectory;

    if (!left.execute(joinedPlan))
    {
        ROS_ERROR("Failed to execute plan");
        return false;
    }

    sleep(1);

    ROS_INFO("Finished");

    // 控制机械臂先回到初始化位置
    left.setNamedTarget("left_home");
    left.move();
    sleep(1);

    ros::shutdown(); 

    return 0;
}
