#include <ros/ros.h>
#include <std_msgs/Header.h>
#include <sensor_msgs/Image.h>
#include <UnitreeCameraSDK.hpp>
#include <camera_info_manager/camera_info_manager.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

int main(int argc, char **argv){

    ros::init(argc, argv, "go1_republisher");
	ros::NodeHandle nh;

    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub_img_left = it.advertise("camera_face/left/image_raw", 1);;
    image_transport::Publisher pub_img_right = it.advertise("camera_face/right/image_raw", 1);
    
    int deviceNode = 1; // default 1 -> /dev/video1
    cv::Size frameSize(1856, 800); // defalut image size: 1856 X 800
    int fps = 30;
    
    UnitreeCamera cam(deviceNode);  ///< init camera by device node number
    if(!cam.isOpened())
        exit(EXIT_FAILURE);
    
    cam.setRawFrameSize(frameSize); ///< set camera frame size
    cam.setRawFrameRate(fps);       ///< set camera frame rate
    
    std::cout << "Device Position Number:" << cam.getPosNumber() << std::endl;
    
    cam.startCapture();            ///< start camera capturing

    ros::Rate loop_rate(30);
    long count = 0;

    while(cam.isOpened() && ros::ok()){
        
        cv::Mat frame;
        std::chrono::microseconds t;
        if(!cam.getRawFrame(frame, t)){ ///< get camera raw image
            usleep(1000);
            continue;
        }
 
        cv::Mat left,right;
        frame(cv::Rect(0, 0, frame.size().width/2, frame.size().height)).copyTo(right);
        frame(cv::Rect(frame.size().width/2,0, frame.size().width/2, frame.size().height)).copyTo(left);

        ros::Time current_time;
        current_time = ros::Time::now();

        std_msgs::Header header;

        header.seq = count++;
		header.stamp = current_time;
		header.frame_id = "camera_face";

        sensor_msgs::ImagePtr msg_left = cv_bridge::CvImage(header, "bgr8", left).toImageMsg();
        sensor_msgs::ImagePtr msg_right = cv_bridge::CvImage(header, "bgr8", right).toImageMsg();

        pub_img_left.publish(msg_left);
        pub_img_right.publish(msg_right);

        ros::spinOnce();
		loop_rate.sleep();
    }
    
    cam.stopCapture();  ///< stop camera capturing
    
    return 0;
}