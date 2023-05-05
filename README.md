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

- in `/scripts` do `sudo chmod +x mono.py` and `sudo chmod +x stereo.py`
- source your workspace 

# To use:

## Mono & Stereo camera publishing

To publish mono (left channel):
- `roslaunch go1_republisher camera_mono.launch`

To publish stereo channels (left/right):
- `roslaunch go1_republisher camera_stereo.launch`

To enable image post processing for rectification (dont forget to calibrate your camera and replace yaml calibration data):
- `roslaunch go1_republisher proc_mono.launch`
- `roslaunch go1_republisher proc_stereo.launch`

## Imu and Odometry

To publish the imu and odom topics/tf:
- `roslaunch go1_republisher imu_odom.launch`

# TODO

- better namespacing to launch multiple instances
- rewrite Python camera node as C++
