#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include "unitree_legged_sdk/unitree_legged_sdk.h"
#include <chrono>
#include <pthread.h>

using namespace UNITREE_LEGGED_SDK;
class Custom
{
	public:
		UDP high_udp;

		HighCmd high_cmd = {0};
		HighState high_state = {0};

	public:
		Custom():
		high_udp(8090, "192.168.123.161", 8082, sizeof(HighCmd), sizeof(HighState))
		{
			high_udp.InitCmdData(high_cmd);
		}

	void highUdpSend()
	{
		// printf("high udp send is running\n");

		high_udp.SetSend(high_cmd);
		high_udp.Send();
	}

	void highUdpRecv()
	{
		high_udp.Recv();
		high_udp.GetRecv(high_state);
		// printf("%i", high_state.gaitType);
	}
};

Custom custom;
ros::Publisher pub_imu;
ros::Publisher pub_odom;

long high_count = 0;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "go1_imu");
	ros::NodeHandle nh;
	pub_imu = nh.advertise<sensor_msgs::Imu>("imu", 1);
	pub_odom = nh.advertise<nav_msgs::Odometry>("odom", 1);

	LoopFunc loop_udpSend("high_udp_send", 0.002, 3, boost::bind(&Custom::highUdpSend, &custom));
	LoopFunc loop_udpRecv("high_udp_recv", 0.002, 3, boost::bind(&Custom::highUdpRecv, &custom));

	loop_udpSend.start();
	loop_udpRecv.start();

	ros::Rate loop_rate(100);

	long count = 0;

	while (ros::ok())
	{
		ros::Time current_time;

		current_time = ros::Time::now();
			
		sensor_msgs::Imu msg_imu;

		msg_imu.header.seq = count;
		msg_imu.header.stamp = current_time;
		msg_imu.header.frame_id = "imu_link";

		msg_imu.orientation.w = custom.high_state.imu.quaternion[0];
		msg_imu.orientation.x = custom.high_state.imu.quaternion[1];
		msg_imu.orientation.y = custom.high_state.imu.quaternion[2];
		msg_imu.orientation.z = custom.high_state.imu.quaternion[3];

		msg_imu.angular_velocity.x = custom.high_state.imu.gyroscope[0];
		msg_imu.angular_velocity.y = custom.high_state.imu.gyroscope[1];
		msg_imu.angular_velocity.z = custom.high_state.imu.gyroscope[2];

		msg_imu.linear_acceleration.x = custom.high_state.imu.accelerometer[0];
		msg_imu.linear_acceleration.y = custom.high_state.imu.accelerometer[1];
		msg_imu.linear_acceleration.z = custom.high_state.imu.accelerometer[2];

		pub_imu.publish(msg_imu);

		nav_msgs::Odometry msg_odom;

		msg_odom.header.seq = count;
		msg_odom.header.stamp = current_time;
		msg_odom.header.frame_id = "odom";
		msg_odom.child_frame_id = "base_link";
	
		msg_odom.pose.pose.position.x = custom.high_state.position[0];
		msg_odom.pose.pose.position.y = custom.high_state.position[1];
		msg_odom.pose.pose.position.z = custom.high_state.position[2];
		
		msg_odom.pose.pose.orientation.w = custom.high_state.imu.quaternion[0];
		msg_odom.pose.pose.orientation.x = custom.high_state.imu.quaternion[1];
		msg_odom.pose.pose.orientation.y = custom.high_state.imu.quaternion[2];
		msg_odom.pose.pose.orientation.z = custom.high_state.imu.quaternion[3];

		msg_odom.twist.twist.linear.x = custom.high_state.velocity[0];
		msg_odom.twist.twist.linear.y = custom.high_state.velocity[1];
		msg_odom.twist.twist.angular.z = custom.high_state.velocity[2];

		pub_odom.publish(msg_odom);

		ros::spinOnce();

		loop_rate.sleep();
  	}

    return 0;
}

