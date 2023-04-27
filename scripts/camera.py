#!/usr/bin/env python
import cv2
import rospy
from cv_bridge import CvBridge
from sensor_msgs.msg import Image

vid = cv2.VideoCapture(1)

print(cv2.__version__)

# vid.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))

vid.set(cv2.CAP_PROP_FRAME_WIDTH, 1856) # stereo feed, divide this by 2 if you want left image, offset crop half-width to get right image
vid.set(cv2.CAP_PROP_FRAME_HEIGHT, 800)

bridge = CvBridge()

def start_node():
    rospy.init_node('camera_republisher')
    rospy.loginfo('cam_pub node started')

    pub_left = rospy.Publisher('camera_front/left/image_raw', Image, queue_size=1)
    pub_right = rospy.Publisher('camera_front/right/image_raw', Image, queue_size=1)

    rate = rospy.Rate(25) # 25hz
    while not rospy.is_shutdown():
        global vid
        ret, frame = vid.read()
        print("capturing frame..")
    
        frame_left = frame[0:800, 0:928]
        frame_right = frame[0:800, 928:1856]

        img_left = bridge.cv2_to_imgmsg(frame_left, "bgr8")
        img_right = bridge.cv2_to_imgmsg(frame_right, "bgr8")

        pub_left.publish(img_left)
        pub_right.publish(img_right)

        rate.sleep()

if __name__ == '__main__':
    try:
        start_node()
    except rospy.ROSInterruptException:
        pass

vid.release()
cv2.destroyAllWindows()
