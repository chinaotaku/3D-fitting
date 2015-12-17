#pragma once
/************************************
**可调用的函数

**speed_up():bool
**speed_down():bool
**time_up():bool
**time_down:bool

**track_it_per_time(const NUI_SKELETON_DATA* skeletonData):guesture_state

**使用时候直接调用track_it_per_time(const NUI_SKELETON_DATA* skeletonData)函数，该函数返回uesture_state结构体
**guesture_state结构体中的数据：
**struct guesture_state{
**bool ON_OFF = false;
**bool LEFT = false;
**bool RIGHT = false;
**bool UP = false;
**bool DOWN = false;
**};
**********************************/

//设定6个状态
#define SLIDE_RIGHT 7
#define SLIDE_LEFT 8
#define SLIDE_UP 9
#define SLIDE_DOWN 10
#define NONE_SLIDE 11

#include <Kinect.h>
#include <time.h>
#include <Windows.h>




struct _2dPosition{
	float x;
	float y;
	float z;
	clock_t time;
};
struct guesture_state{
	bool ON_OFF = false;
	bool LEFT = false;
	bool RIGHT = false;
	bool UP = false;
	bool DOWN = false;
	bool SCREENSHOT = false;
};

class GestureTracker
{
private:

	//最小手势速度
	float min_gesture_speed;
	int xtime_gap;
	int ytime_gap;

	//计时器 promises that just per second a gesture is avialable
	clock_t latest_complished_gesture_time;


	//x,y的初始和末状态的记录点
	float *first_Lx_tracked_coord;
	float *last_Lx_tracked_coord;
	float *first_Ly_tracked_coord;
	float *last_Ly_tracked_coord;

	float *first_Rx_tracked_coord;
	float *last_Rx_tracked_coord;
	float *first_Ry_tracked_coord;
	float *last_Ry_tracked_coord;

	//记录手在头部以上的状态
	bool is_left_hand_uper_than_head = false;
	bool is_right_hand_uper_than_head = false;

	//实时手部位的位置
	_2dPosition current_left_hand_position;
	_2dPosition current_right_hand_position;

	//上一次的手部位置
	_2dPosition old_left_hand_position;
	_2dPosition old_right_hand_position;


	//当前的速度
	float current_Lx_speed;
	float current_Ly_speed;

	float current_Rx_speed;
	float current_Ry_speed;


	//设定初始可监测标记
	bool is_tracking_Lx_coord;
	bool is_tracking_Ly_coord;

	bool is_tracking_Rx_coord;
	bool is_tracking_Ry_coord;

	//记录两个时间点
	clock_t m_Lxpfirst_time;
	clock_t m_Lxplast_time;

	clock_t m_Rxpfirst_time;
	clock_t m_Rxplast_time;

	clock_t m_Lypfirst_time;
	clock_t m_Lyplast_time;

	clock_t m_Rypfirst_time;
	clock_t m_Ryplast_time;

	//滑动标志
	int lxslide_orientation;
	int rxslide_orientation;
	int lyslide_orientation;
	int ryslide_orientation;

	//实时手势
	guesture_state current_guesture_state;

	//更新实时手的位置
	void refresh_hand_position(_2dPosition Lposition, _2dPosition Rposition);

	//手势监测函数设定
	void tracking_gesture(const NUI_SKELETON_DATA* skeletonData);

	//每时每刻计算出当前的速度
	void refresh_current_speed();

	//重置当前手势状态
	void reset_gesture_state();



	
	

public:
	GestureTracker(){
		min_gesture_speed = 0.0017;
		xtime_gap = 500;
		ytime_gap = 1000;

		current_Lx_speed = 0.0;
		current_Ly_speed = 0.0;

		current_Rx_speed = 0.0;
		current_Ry_speed = 0.0;

		//mark current tracking right/left hand's x/y coord
		is_tracking_Lx_coord = false;
		is_tracking_Ly_coord = false;

		is_tracking_Rx_coord = false;
		is_tracking_Ry_coord = false;

		//mark current tracking directon
		lxslide_orientation = NONE_SLIDE;
		rxslide_orientation = NONE_SLIDE;
		lyslide_orientation = NONE_SLIDE;
		ryslide_orientation = NONE_SLIDE;

		//initialize timer
		latest_complished_gesture_time = clock();

	}
	~GestureTracker();

	//总加载函数
	guesture_state track_it_per_time(const NUI_SKELETON_DATA* skeletonData);

	//plus the speed
	bool speed_up(){
		if (min_gesture_speed > 0.0017&&min_gesture_speed < 2.0024){
			min_gesture_speed = min_gesture_speed + 0.0001;
			return true;
		}
		return false;
	}

	bool speed_down(){
		if (min_gesture_speed > 0.0017&&min_gesture_speed < 2.0024){
			min_gesture_speed = min_gesture_speed - 0.0001;
			return true;
		}
		return false;
	}


	//plus the time gap
	bool time_up(){
		if (xtime_gap<3000 && xtime_gap>500){
			xtime_gap = xtime_gap + 500;
			ytime_gap = ytime_gap + 500;
			return true;
		}
		return false;
	}
	bool time_down(){
		if (xtime_gap<3000 && xtime_gap>500){
			xtime_gap = xtime_gap - 500;
			ytime_gap = ytime_gap - 500;
			return true;
		}
		return false;
	}



};

