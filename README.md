# go1_republisher
Publish camera/imu/odometry as ROS topics on the Unitree Go1 dogs.

- ssh into one of the jetson nanos
- `ps aux | grep point` and `sudo kill pid` to free video devices
- create a new catkin ws on the desktop like `mkdir custom_ws`
- `cd custom_ws`
- `mkdir src`
- git clone `unitree_legged_sdk` there
- git clone this repo there
- `catkin_make`
- source your workspace 

to use:

- in `/scripts` do `sudo chmod +x camery.py`
`roslaunch go1_republisher camera.launch` will publish a camera as Image topic

`roslaunch go1_republisher imu_odom.launch` will publish `/imu` and `/odom` topics
