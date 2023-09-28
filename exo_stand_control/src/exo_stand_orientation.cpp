#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/robot_trajectory/robot_trajectory.h>

// 轨迹重定义，更改规划过程中的 “速度和加速度”

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


    right.setJointValueTarget(right_joint_positions);
    left.setJointValueTarget(left_joint_positions);
    moveit::planning_interface::MoveGroupInterface::Plan plan_left;
    moveit::planning_interface::MoveGroupInterface::Plan plan_right;

    moveit::planning_interface::MoveItErrorCode success1 = left.plan(plan_left);
    moveit::planning_interface::MoveItErrorCode success2 = right.plan(plan_right);

    ROS_INFO("Plan (pose goal) %s",success1?"":"FAILED");   
    ROS_INFO("Plan (pose goal) %s",success2?"":"FAILED");   
    
    scale_trajectory_speed(plan_left, 0.3);//调用最上方的轨迹重定义函数
    scale_trajectory_speed(plan_right, 0.3);

    //让机械臂按照规划的轨迹开始运动。
    if(success1)
      left.execute(plan_left);
    sleep(1);

    if(success2)
      right.execute(plan_right);
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
