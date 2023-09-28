#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# 未成功

import rospy, sys
import moveit_commander
from moveit_msgs.msg import PlanningScene

class MoveItFkDemo:
    def __init__(self):
        moveit_commander.roscpp_initialize(sys.argv)
        rospy.init_node('exo_stand_fk', anonymous=True)
        
        self.scene = PlanningScene()
        self.scene.is_diff = True
        
        self.left = moveit_commander.MoveGroupCommander('left_group')
        self.right = moveit_commander.MoveGroupCommander('right_group')
        
        self.left.set_goal_joint_tolerance(0.001)
        self.right.set_goal_joint_tolerance(0.001)

        self.left.set_max_acceleration_scaling_factor(0.5)
        self.left.set_max_velocity_scaling_factor(0.5)
        self.right.set_max_acceleration_scaling_factor(0.5)
        self.right.set_max_velocity_scaling_factor(0.5)
        
        self.left.set_named_target('left_home')
        self.right.set_named_target('right_home')

        self.left.go()
        self.right.go()
        rospy.sleep(1)
         
        left_joint_positions = [-0.772, 0.7416]
        right_joint_positions = [-0.373, 0]

        self.left.set_joint_value_target(left_joint_positions)
        self.right.set_joint_value_target(right_joint_positions)

        # 将两个机械臂的规划结果合并
        combined_trajectory = self.left.plan() + self.right.plan()

        # 使用规划结果执行运动
        self.left.execute(combined_trajectory)

        self.left.set_named_target('left_home')
        self.right.set_named_target('right_home')

        self.left.go()
        self.right.go()
        rospy.sleep(1)
        
        moveit_commander.roscpp_shutdown()
        moveit_commander.os._exit(0)

if __name__ == "__main__":
    try:
        MoveItFkDemo()
    except rospy.ROSInterruptException:
        pass
