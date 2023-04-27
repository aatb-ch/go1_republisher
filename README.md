# go1_republisher
Publish camera/imu/odometry as ROS topics on the Unitree Go1 dogs.

- ssh into one of the jetson nanos
- `ps aux | grep point` and `sudo kill pid` to free video devices
- follow these instructions, customize if needed:

```
mkdir custom_ws
cd custom_ws
mkdir src
cd src
git clone https://github.com/unitreerobotics/unitree_legged_sdk.git
git clone https://github.com/ros-perception/camera_info_manager_py.git
git clone https://github.com/aatb-ch/go1_republisher.git
cd ~/custom_ws
catkin_make
```

- in `/scripts` do `sudo chmod +x camery.py`
- source your workspace 

# To use:

- `roslaunch go1_republisher camera.launch` will publish a camera as Image topic
- `roslaunch go1_republisher imu_odom.launch` will publish `/imu` and `/odom` topics
