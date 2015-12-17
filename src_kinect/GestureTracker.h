#pragma once
/************************************
**�ɵ��õĺ���

**speed_up():bool
**speed_down():bool
**time_up():bool
**time_down:bool

**track_it_per_time(const NUI_SKELETON_DATA* skeletonData):guesture_state

**ʹ��ʱ��ֱ�ӵ���track_it_per_time(const NUI_SKELETON_DATA* skeletonData)�������ú�������uesture_state�ṹ��
**guesture_state�ṹ���е����ݣ�
**struct guesture_state{
**bool ON_OFF = false;
**bool LEFT = false;
**bool RIGHT = false;
**bool UP = false;
**bool DOWN = false;
**};
**********************************/

//�趨6��״̬
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

	//��С�����ٶ�
	float min_gesture_speed;
	int xtime_gap;
	int ytime_gap;

	//��ʱ�� promises that just per second a gesture is avialable
	clock_t latest_complished_gesture_time;


	//x,y�ĳ�ʼ��ĩ״̬�ļ�¼��
	float *first_Lx_tracked_coord;
	float *last_Lx_tracked_coord;
	float *first_Ly_tracked_coord;
	float *last_Ly_tracked_coord;

	float *first_Rx_tracked_coord;
	float *last_Rx_tracked_coord;
	float *first_Ry_tracked_coord;
	float *last_Ry_tracked_coord;

	//��¼����ͷ�����ϵ�״̬
	bool is_left_hand_uper_than_head = false;
	bool is_right_hand_uper_than_head = false;

	//ʵʱ�ֲ�λ��λ��
	_2dPosition current_left_hand_position;
	_2dPosition current_right_hand_position;

	//��һ�ε��ֲ�λ��
	_2dPosition old_left_hand_position;
	_2dPosition old_right_hand_position;


	//��ǰ���ٶ�
	float current_Lx_speed;
	float current_Ly_speed;

	float current_Rx_speed;
	float current_Ry_speed;


	//�趨��ʼ�ɼ����
	bool is_tracking_Lx_coord;
	bool is_tracking_Ly_coord;

	bool is_tracking_Rx_coord;
	bool is_tracking_Ry_coord;

	//��¼����ʱ���
	clock_t m_Lxpfirst_time;
	clock_t m_Lxplast_time;

	clock_t m_Rxpfirst_time;
	clock_t m_Rxplast_time;

	clock_t m_Lypfirst_time;
	clock_t m_Lyplast_time;

	clock_t m_Rypfirst_time;
	clock_t m_Ryplast_time;

	//������־
	int lxslide_orientation;
	int rxslide_orientation;
	int lyslide_orientation;
	int ryslide_orientation;

	//ʵʱ����
	guesture_state current_guesture_state;

	//����ʵʱ�ֵ�λ��
	void refresh_hand_position(_2dPosition Lposition, _2dPosition Rposition);

	//���Ƽ�⺯���趨
	void tracking_gesture(const NUI_SKELETON_DATA* skeletonData);

	//ÿʱÿ�̼������ǰ���ٶ�
	void refresh_current_speed();

	//���õ�ǰ����״̬
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

	//�ܼ��غ���
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

