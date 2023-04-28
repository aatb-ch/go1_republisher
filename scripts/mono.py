#!/usr/bin/env python
import cv2
import rospy
from cv_bridge import CvBridge
from sensor_msgs.msg import Image
from sensor_msgs.msg import CameraInfo
from camera_info_manager import CameraInfoManager

bridge = CvBridge()

left_ci = None
vid = None

def start_node():

    rospy.init_node('camera_republisher', anonymous=True)
    rospy.loginfo('cam_pub node started')

    deviceId = rospy.get_param('~device_id')
    camera_name = rospy.get_param('~camera_name')
    calibration_left = rospy.get_param('~calibration_left')

    left_ci = CameraInfoManager(cname=camera_name+'/left', namespace='left')

    if calibration_left:
        left_ci.setURL(calibration_left)
        left_ci.loadCameraInfo()

    global vid
    vid = cv2.VideoCapture(deviceId)

    vid.set(cv2.CAP_PROP_FRAME_WIDTH, 1856) # stereo feed, divide this by 2 if you want left image, offset crop half-width to get right image
    vid.set(cv2.CAP_PROP_FRAME_HEIGHT, 800)

    pub_left = rospy.Publisher(rospy.get_namespace()+'left/image_raw', Image, queue_size=1)
    
    pub_left_ci = rospy.Publisher(rospy.get_namespace()+'left/camera_info', CameraInfo, queue_size=1)

    rate = rospy.Rate(25) # 25hz
    count = 0

    while not rospy.is_shutdown():
        
        ret, frame = vid.read()
        print("capturing frame..")
    
        frame_left = frame[0:800, 928:1856]
        img_left = bridge.cv2_to_imgmsg(frame_left, "bgr8")

        now = rospy.get_rostime()
        
        img_left.header.stamp = now
        img_left.header.frame_id = camera_name
        img_left.header.seq = count
        pub_left.publish(img_left)

        l_ci = left_ci.getCameraInfo()
        l_ci.header = img_left.header
        pub_left_ci.publish(l_ci)
    
        rate.sleep()

if __name__ == '__main__':
    try:
        start_node()
    except rospy.ROSInterruptException:
        pass

vid.release()
cv2.destroyAllWindows()
