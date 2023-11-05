# ros_exoskeleton_maxon_simulation——2023.7.28
## 1）功能介绍
在ros1-noetic中实现了外骨骼机器人仿真，实现了外骨骼机器人的关节角度读取并发布、视觉测距、基于3d点云的障碍物识别避障、仿真机器人运动轨迹的自定义规划。

视频展示（来源于本人B站）：https://www.bilibili.com/video/BV1f94y157Qa/?spm_id_from=333.1007.0.0

![](https://github.com/hhz0328/ros_exoskeleton_maxon_simulation/blob/main/%E4%BB%BF%E7%9C%9F%E6%8E%A7%E5%88%B6%E6%A1%86%E6%9E%B6.jpg)
![](https://github.com/hhz0328/ros_exoskeleton_maxon_simulation/blob/main/exo_stand_control/exo_stand_pointcloud_orientation.cpp%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA.png)
## 2）环境配置
（1）**用到的模块**：gazebo，rviz，moveit

（2）**编译过程**：将以上的功能包放入到 **/catkin_ws/src** ，再在 **/catkin_ws** 中打开终端用 **catkin_make** 命令进行编译
## 3）使用
- **（1）打开联合仿真（可以通过movei提供的交互界面，对机器人进行控制）：**
```
  $ roscore
  $ roslaunch exo_stand_moveit_config full_leg_sim.launch
```
- **（2）使用python的API接口，进行控制（在功能包exo_stand_control/scripts目录下）：**
```
  $ rosrun exo_stand_control exo_stand_fk.py
```
- **（3）使用cpp的API接口，进行控制（在功能包exo_stand_control/src目录下）：**
```
  $ rosrun exo_stand_control exo_stand_fk
```
- **（4）配合intel D435i深度相机，将障碍物3d点云导入到仿真环境中（注意同一时间只能打开一个rviz，否则会报错，所以在使用时要使用 **ctrl+c** 关闭先前打开的联合仿真终端命令）：**
```
  $ roslaunch exo_stand_moveit_config exo_stand_intelD435i_obstacle_pointcloud.launch
```
- **（5）python实现消息记录的订阅和保存（在功能包exo_stand_status/scripts目录下）：**
```
  $ rosrun exo_stand_status save_listener.py
```
- **（6）cpp实现消息记录的订阅和保存（在功能包exo_stand_status/src目录下）：**
```
  $ rosrun exo_stand_status listener_continue_trajectory
```
- **（7）将录制的.bag文件转换成.csv格式并导出到工作空间下（在功能包rosbag_to_csv中，需要特别注意rosbag_to_csv这功能包在catkin_make之前，需要更新一下rosdep，否则会显示编译出错）：**
```
  $ rosrun rosbag_to_csv rosbag_to_csv.py
```
