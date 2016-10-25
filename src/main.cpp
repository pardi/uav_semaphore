#include <uavsemaphore.h>

int main(int argc, char** argv){

	ros::init(argc, argv, "uav_semaphore_node");

   	ros::NodeHandle n;

   	ROS_INFO("Start UAV Semaphore node");

	uavsem::uavsemaphore us(&n);

	ros::spin();
}