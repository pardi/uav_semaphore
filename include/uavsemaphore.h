#ifndef __UAVSEMAPHORE_H__
#define __UAVSEMAPHORE_H__

// Standard Lib
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mutex>  
// ROS Lib
#include <ros/ros.h>
#include <uav_semaphore/holdDropZone.h>
#include <uav_semaphore/flyZone.h>

#define LAYER_N 3
 
using namespace std;

namespace uavsem {

	class uavsemaphore{

	public:
		// ROS constructor
		uavsemaphore(ros::NodeHandle *n);

		~uavsemaphore();
		
	private:


		bool holdDropZoneService(uav_semaphore::holdDropZone::Request&, uav_semaphore::holdDropZone::Response&);
		bool holdFlyZoneService(uav_semaphore::flyZone::Request&, uav_semaphore::flyZone::Response&);

		int found_ID(const int);

		// ROS

		ros::NodeHandle* n_;

		// Mutex
		std::mutex dropzone_mtx_;
		std::mutex flyzone_mtx_;

		int uavID_dz_;
		int uavID_fz_[3];

		// Service
		ros::ServiceServer hold_dz_ser_;
		ros::ServiceServer hold_fz_ser_;

	};
} 
#endif 