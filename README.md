# ros_exoskeleton_maxon_simulation
## 1）功能介绍
在ros1-noetic中实现了外骨骼机器人仿真，实现了外骨骼机器人的关节角度读取并发布、视觉测距、仿真机器人运动轨迹的自定义规划。
## 2）环境配置
（1）**用到的模块**：gazebo，rviz，moveit

（2）**编译过程**：将以上的功能包放入到 **/catkin_ws/src** ，再在 **/catkin_ws** 中打开终端用 **catkin_make** 命令进行编译
## 3）使用
- （1）打开联合仿真（可以通过moveit提供的交互界面，对机器人进行控制）：
```
  $ roscore
  $ roslaunch exo_stand_moveit_config full_leg_sim.launch
```
- （2）使用python的API接口，进行控制（在功能包exo_stand_control/scripts目录下）：
```
  $ rosrun exo_stand_control exo_stand_fk.py
```
- （3）使用cpp的API接口，进行控制（在功能包exo_stand_control/src目录下）：
```
  $ rosrun exo_stand_control exo_stand_fk
```
