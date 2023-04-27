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

    pub = rospy.Publisher('camera/image_raw', Image, queue_size=1)

    rate = rospy.Rate(25) # 25hz
    while not rospy.is_shutdown():
        global vid
        ret, frame = vid.read()
        print("capturing frame..")
        imgMsg = bridge.cv2_to_imgmsg(frame, "bgr8")
        pub.publish(imgMsg)

        rate.sleep()

if __name__ == '__main__':
    try:
        start_node()
    except rospy.ROSInterruptException:
        pass

vid.release()
cv2.destroyAllWindows()
