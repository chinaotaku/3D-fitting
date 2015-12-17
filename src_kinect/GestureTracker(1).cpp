#include "GestureTracker.h"
#include <stdio.h>


//GestureTracker::GestureTracker()
//{
//}


GestureTracker::~GestureTracker()
{
}




//更新实时手的位置
void GestureTracker::refresh_hand_position(_2dPosition Lposition, _2dPosition Rposition){


	    
	//left hand

	old_left_hand_position.time = current_left_hand_position.time;
	old_left_hand_position.x = current_left_hand_position.x;
	old_left_hand_position.y = current_left_hand_position.y;
	old_left_hand_position.z = current_left_hand_position.z;

	current_left_hand_position.time = Lposition.time;
	current_left_hand_position.x = Lposition.x;
	current_left_hand_position.y = Lposition.y;
	current_left_hand_position.z = Lposition.z;

	//right hand


	old_right_hand_position.time = current_right_hand_position.time;
	old_right_hand_position.x = current_right_hand_position.x;
	old_right_hand_position.y = current_right_hand_position.y;
	old_right_hand_position.z = current_right_hand_position.z;

	current_right_hand_position.time = Rposition.time;
	current_right_hand_position.x = Rposition.x;
	current_right_hand_position.y = Rposition.y;
	current_right_hand_position.z = Rposition.z;




}

//手势监测函数设定
void GestureTracker::tracking_gesture(const NUI_SKELETON_DATA* skeletonData){
	//anylise the speed and compish gesture
	//left hand
	if (current_Lx_speed >= min_gesture_speed){
		if (!is_tracking_Lx_coord&&!is_tracking_Ly_coord&&!is_tracking_Rx_coord&&!is_tracking_Ry_coord){
			
			clock_t currentTime=clock();
			if (currentTime-latest_complished_gesture_time>=xtime_gap){
			is_tracking_Lx_coord = true;
			lxslide_orientation = SLIDE_RIGHT;
			}


		}
	}
	else if (current_Lx_speed < - min_gesture_speed){
		if (!is_tracking_Lx_coord&&!is_tracking_Ly_coord&&!is_tracking_Rx_coord&&!is_tracking_Ry_coord){

			clock_t currentTime = clock();
			if (currentTime - latest_complished_gesture_time >= xtime_gap){
			is_tracking_Lx_coord = true;
			lxslide_orientation = SLIDE_LEFT;
			}
		}
	}
	else if (current_Lx_speed < min_gesture_speed&&current_Lx_speed >= -min_gesture_speed){
		if (is_tracking_Lx_coord&&!is_tracking_Ly_coord&&!is_tracking_Rx_coord&&!is_tracking_Ry_coord){

			if(lxslide_orientation == SLIDE_RIGHT){

				clock_t currentTime = clock();
				latest_complished_gesture_time = currentTime;

				current_guesture_state.RIGHT = true;
			}
			else{

				clock_t currentTime = clock();
				latest_complished_gesture_time = currentTime;

				current_guesture_state.LEFT = true;
			}
			is_tracking_Lx_coord = false;
			lxslide_orientation = NONE_SLIDE;

		}

	}

	if (current_Ly_speed >= min_gesture_speed){
		if (!is_tracking_Lx_coord&&!is_tracking_Ly_coord&&!is_tracking_Rx_coord&&!is_tracking_Ry_coord){

			if (skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y > skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y){
				latest_complished_gesture_time = clock();
			}

			clock_t currentTime = clock();
			if (currentTime - latest_complished_gesture_time >= ytime_gap){
			is_tracking_Ly_coord = true;
			lyslide_orientation = SLIDE_UP;
			}

		}
	}
	else if (current_Ly_speed < - min_gesture_speed){
		if (!is_tracking_Lx_coord&&!is_tracking_Ly_coord&&!is_tracking_Rx_coord&&!is_tracking_Ry_coord){

			if (skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y > skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y){
				latest_complished_gesture_time = clock();
			}

			clock_t currentTime = clock();
			if (currentTime - latest_complished_gesture_time >= ytime_gap){
			is_tracking_Ly_coord = true;
			lyslide_orientation = SLIDE_DOWN;
			}
		}
	}
	else if (current_Ly_speed < min_gesture_speed&&current_Ly_speed >= -min_gesture_speed){

		if (!is_tracking_Lx_coord&&is_tracking_Ly_coord&&!is_tracking_Rx_coord&&!is_tracking_Ry_coord){
			if (skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y > skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y){
				latest_complished_gesture_time = clock();
			}
			else{
				if (lyslide_orientation == SLIDE_UP){

					clock_t currentTime = clock();
					latest_complished_gesture_time = currentTime;

					current_guesture_state.UP = true;
				}
				else{
					clock_t currentTime = clock();
					latest_complished_gesture_time = currentTime;

					current_guesture_state.DOWN = true;
				}
			}

			is_tracking_Ly_coord = false;
			lyslide_orientation = NONE_SLIDE;
		}

	}

	//right hand 
	if (current_Rx_speed >= min_gesture_speed){
		if (!is_tracking_Lx_coord&&!is_tracking_Ly_coord&&!is_tracking_Rx_coord&&!is_tracking_Ry_coord){
			clock_t currentTime = clock();
			if (currentTime - latest_complished_gesture_time >= xtime_gap){
			is_tracking_Rx_coord = true;
			rxslide_orientation = SLIDE_RIGHT;
			}

		}
	}
	else if (current_Rx_speed< - min_gesture_speed){
		if (!is_tracking_Lx_coord&&!is_tracking_Ly_coord&&!is_tracking_Rx_coord&&!is_tracking_Ry_coord){
			clock_t currentTime = clock();
			if (currentTime - latest_complished_gesture_time >= xtime_gap){
			is_tracking_Rx_coord = true;
			rxslide_orientation = SLIDE_LEFT;
			}

		}
	}
	else if (current_Rx_speed < min_gesture_speed&&current_Rx_speed >= -min_gesture_speed){
		if (!is_tracking_Lx_coord&&!is_tracking_Ly_coord&&is_tracking_Rx_coord&&!is_tracking_Ry_coord){
			if (rxslide_orientation == SLIDE_RIGHT){

				clock_t currentTime = clock();
				latest_complished_gesture_time = currentTime;

				current_guesture_state.RIGHT = true;
			}
			else{

				clock_t currentTime = clock();
				latest_complished_gesture_time = currentTime;

				current_guesture_state.LEFT = true;
			}
			is_tracking_Rx_coord = false;
			rxslide_orientation = NONE_SLIDE;
		}

	}

	if (current_Ry_speed >= min_gesture_speed){
		if (!is_tracking_Lx_coord&&!is_tracking_Ly_coord&&!is_tracking_Rx_coord&&!is_tracking_Ry_coord){
			if (skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y > skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y){
				latest_complished_gesture_time = clock();
			}
			clock_t currentTime = clock();
			if (currentTime - latest_complished_gesture_time >= ytime_gap){
			is_tracking_Ry_coord = true;
			ryslide_orientation = SLIDE_UP;
			}

		}
	}
	else if (current_Ry_speed < - min_gesture_speed){
		if (!is_tracking_Lx_coord&&!is_tracking_Ly_coord&&!is_tracking_Rx_coord&&!is_tracking_Ry_coord){
			if (skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y > skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y){
				latest_complished_gesture_time = clock();
			}
			clock_t currentTime = clock();
			if (currentTime - latest_complished_gesture_time >= ytime_gap){
			is_tracking_Ry_coord = true;
			ryslide_orientation = SLIDE_DOWN;
			}

		}
	}
	else if (current_Ry_speed < min_gesture_speed&&current_Ry_speed >= -min_gesture_speed){

		if (!is_tracking_Lx_coord&&!is_tracking_Ly_coord&&!is_tracking_Rx_coord&&is_tracking_Ry_coord){

			if (skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y > skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y){
				latest_complished_gesture_time = clock();
			}
			else{
				if (ryslide_orientation == SLIDE_UP){
					clock_t currentTime = clock();
					current_guesture_state.UP = true;
				}
				else {
					clock_t currentTime = clock();
					latest_complished_gesture_time = currentTime;
					current_guesture_state.DOWN = true;
				}
			}


			is_tracking_Ry_coord = false;
			ryslide_orientation = NONE_SLIDE;
		}


		//get ON_OFF flags for turning on/off the subwindows
		//right hand
		if (skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y > skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y){
			if (!is_right_hand_uper_than_head){
//				GetLocalTime(&latest_complished_gesture_time);
				is_right_hand_uper_than_head = true;
				current_guesture_state.ON_OFF = true;
			}
			else
			{
				current_guesture_state.ON_OFF = false;
//				GetLocalTime(&latest_complished_gesture_time);
			}
		}
		else if (skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y <= skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y)
		{
			is_right_hand_uper_than_head = false;
			//			GetLocalTime(&latest_complished_gesture_time);
		}


		//get SCREEN_SHOT flags for taking a screen shot for user
		//left hand
		if (skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y > skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y){
			if (!is_left_hand_uper_than_head){
//				GetLocalTime(&latest_complished_gesture_time);
				is_left_hand_uper_than_head = true;
				current_guesture_state.SCREENSHOT = true;
			}
			else
			{
				current_guesture_state.SCREENSHOT = false;
//				GetLocalTime(&latest_complished_gesture_time);
			}
		}
		else if (skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y <= skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y)
		{
			is_left_hand_uper_than_head = false;
			//			GetLocalTime(&latest_complished_gesture_time);
		}

	}


	/****************
	//零状态
	//left hand
	if (!is_tracking_Lx_coord){
		if (current_Lx_speed >= min_gesture_speed){
			is_tracking_Lx_coord = true;
			lxslide_orientation = SLIDE_RIGHT;
			
		}
		else if (current_Lx_speed <= -min_gesture_speed){
			is_tracking_Lx_coord = true;
			lxslide_orientation = SLIDE_LEFT;
		
		}
	}


	if (!is_tracking_Ly_coord){
		if (current_Ly_speed >= min_gesture_speed){
			is_tracking_Ly_coord = true;
			lyslide_orientation = SLIDE_UP;
			

		}
		else if(current_Ly_speed <= -min_gesture_speed){
			is_tracking_Ly_coord = true;
			lyslide_orientation = SLIDE_DOWN;
			
		}
	}



		//right hand
	if (!is_tracking_Rx_coord){
		if (current_Rx_speed >= min_gesture_speed){
			is_tracking_Rx_coord = true;
			rxslide_orientation = SLIDE_RIGHT;
			
		}
		else if (current_Rx_speed <= -min_gesture_speed){
			is_tracking_Rx_coord = true;
			rxslide_orientation = SLIDE_LEFT;
			
		}
	}


	if (is_tracking_Ry_coord){
		if (current_Ry_speed >= min_gesture_speed){
			is_tracking_Ry_coord = true;
			ryslide_orientation = SLIDE_UP;
			
		}
		else if (current_Ry_speed <= -min_gesture_speed){
			is_tracking_Ry_coord = true;
			ryslide_orientation = SLIDE_DOWN;
			
		}
	}

	//末状态

		//left hand
	if (is_tracking_Lx_coord){
		if (lxslide_orientation == SLIDE_RIGHT){
			if (current_Lx_speed < min_gesture_speed){
				is_tracking_Lx_coord = false;

				//complete gesture anylise
				current_guesture_state.RIGHT = true;
				lxslide_orientation == NONE_SLIDE;

			}
		}
		else if (lxslide_orientation == SLIDE_LEFT){
			if (current_Lx_speed > -min_gesture_speed){
				is_tracking_Lx_coord = false;
				current_guesture_state.LEFT = true;
				lxslide_orientation == NONE_SLIDE;

			}
		}
	}


	if (is_tracking_Ly_coord){
		if (lyslide_orientation == SLIDE_UP){
			if (current_Ly_speed < min_gesture_speed){
				is_tracking_Ly_coord = false;
				lyslide_orientation == NONE_SLIDE;
				current_guesture_state.UP = true;


			}
		}
		else if (lyslide_orientation == SLIDE_DOWN){
			if (current_Ly_speed > -min_gesture_speed){
				is_tracking_Ly_coord = false;
				lyslide_orientation == NONE_SLIDE;
				current_guesture_state.DOWN = true;

			}
		}
	}

	//right hand
	if (is_tracking_Rx_coord){
		if (rxslide_orientation == SLIDE_RIGHT){
			if (current_Rx_speed < min_gesture_speed){
				is_tracking_Rx_coord = false;
				rxslide_orientation == NONE_SLIDE;
				current_guesture_state.RIGHT = true;
			}
		}
		else if (rxslide_orientation == SLIDE_LEFT){
			if (current_Rx_speed > -min_gesture_speed){
				is_tracking_Rx_coord = false;
				//初始化最后一个监测点x
				ryslide_orientation == NONE_SLIDE;
				current_guesture_state.LEFT = true;
			}
		}

	}


	if (is_tracking_Ry_coord){
		if (ryslide_orientation == SLIDE_UP){
			if (current_Ry_speed < min_gesture_speed){
				is_tracking_Ry_coord = false;
				//初始化最后一个监测点y
				ryslide_orientation = NONE_SLIDE;
				current_guesture_state.UP = true;

			}
		}
		else if (ryslide_orientation == SLIDE_DOWN){
			if (current_Ry_speed > -min_gesture_speed){
				is_tracking_Ry_coord = false;
				//初始化最后一个监测点y
				current_guesture_state.DOWN = true;
				ryslide_orientation = NONE_SLIDE;

			}
		}

	}
	********************/

}

//每时每刻计算出当前的速度
void GestureTracker::refresh_current_speed(){
	//left hand

	float dlx;
	float dly;
	float drx;
	float dry;

	WORD dlt = current_left_hand_position.time - old_left_hand_position.time;
	WORD drt = current_right_hand_position.time - old_right_hand_position.time;

	dlx = current_left_hand_position.x - old_left_hand_position.x;
	dly = current_left_hand_position.y - old_left_hand_position.y;
	drx = current_right_hand_position.x - old_right_hand_position.x;
	dry = current_right_hand_position.y - old_right_hand_position.y;

//	printf_s("dlx:%f  dly:%f  drx:%f  dry:%f  \n",dlx,dly,drx,dry);
	
	float dlx_speed;
	float dly_speed;
	float drx_speed;
	float dry_speed;
	current_Lx_speed = dlx / dlt;
	current_Ly_speed = dly / dlt;
	current_Rx_speed = drx / drt;
	current_Ry_speed = dry / drt;

	//pritntf result
 //   printf_s("lxspeed:%f,lyspeed:%f,rxspeed:%f,ryspeed:%f  \n", current_Lx_speed, current_Ly_speed, current_Rx_speed, current_Ry_speed);
	
}

void GestureTracker::reset_gesture_state(){
	this->current_guesture_state.ON_OFF = false;
	this->current_guesture_state.LEFT = false;
	this->current_guesture_state.RIGHT = false;
	this->current_guesture_state.UP = false;
	this->current_guesture_state.DOWN = false;
	this->current_guesture_state.SCREENSHOT = false;
}


guesture_state GestureTracker::track_it_per_time(const NUI_SKELETON_DATA* skeletonData){

	//tracks it when left/right hand's position has changed
	if (old_left_hand_position.x == skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x&&
		old_left_hand_position.y == skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y&&
		old_left_hand_position.z == skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z&&
		old_right_hand_position.x == skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x &&
		old_right_hand_position.y == skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y &&
		old_right_hand_position.z == skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z
		){
		return current_guesture_state;

	}


	_2dPosition left_hand_datas;
	_2dPosition right_hand_datas;
	left_hand_datas.time = clock();
	right_hand_datas.time = left_hand_datas.time;

	left_hand_datas.x = skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x;
	left_hand_datas.y = skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y;
	left_hand_datas.z = skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z;

	right_hand_datas.x = skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x;
	right_hand_datas.y = skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y;
	right_hand_datas.z = skeletonData->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z;

	//更新手的坐标
	refresh_hand_position(left_hand_datas, right_hand_datas);
	//refresh current speed
	refresh_current_speed();
	//监测手势
	tracking_gesture(skeletonData);

	//返回手势状态
	guesture_state state = current_guesture_state;
	reset_gesture_state();

	return state;
}