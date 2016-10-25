#include <uavsemaphore.h>

using namespace uavsem;

uavsemaphore::uavsemaphore(ros::NodeHandle *n){
	
	// Store node handle
	n_ = n;

	// Define service
	hold_dz_ser_ = n_->advertiseService("uav_semaphore/hold_drop_zone", &uavsemaphore::holdDropZoneService, this);
	hold_fz_ser_ = n_->advertiseService("uav_semaphore/hold_fly_zone", &uavsemaphore::holdFlyZoneService, this);

	// Variable 
	uavID_dz_ = -1;

	// Reset ID layer

	uavID_fz_[0] = -1;
	uavID_fz_[1] = -1;
	uavID_fz_[2] = -1;
}

uavsemaphore::~uavsemaphore(){

}

bool uavsemaphore::holdDropZoneService(uav_semaphore::holdDropZone::Request  &req, uav_semaphore::holdDropZone::Response &res){

	dropzone_mtx_.lock();

	if ((uavID_dz_ == -1) || (uavID_dz_ == req.uav_id)){
		if (req.hold_on == true)			
			uavID_dz_ = req.uav_id;
		else
			uavID_dz_ = -1;
		res.status = true; // able to perform request operation
	}
	else
		res.status = false; // unable to perform request operation
	
	dropzone_mtx_.unlock();

	return true;
}

bool uavsemaphore::holdFlyZoneService(uav_semaphore::flyZone::Request  &req, uav_semaphore::flyZone::Response &res){

	flyzone_mtx_.lock();

	int free_layer = found_ID(req.uav_id);

	if (free_layer != -1){
		if (req.hold_on == true){		
			uavID_fz_[free_layer] = req.uav_id;
			res.meters = (free_layer == 0)?5:(free_layer == 1)?15:25;
		}
		else{
			uavID_fz_[free_layer] = -1;
			res.meters = -1;
		}
		
		res.status = true;		
	}
	else{
		res.meters = -1;
		res.status = false;
	}

	flyzone_mtx_.unlock();

	return true;
}

int uavsemaphore::found_ID(const int id){

	// Check if uavid already store

	for(int i = LAYER_N; i--;){
		if (uavID_fz_[i] == id)
			return i;
	}

	// Check if a layer is free

	for(int i = LAYER_N; i--;){
		if (uavID_fz_[i] == -1)
			return i;
	}

	return -1;
}