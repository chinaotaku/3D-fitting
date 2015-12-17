#pragma once

#include <Windows.h>
#include <NuiApi.h>
#include <string>
#include <glew.h>
#include <GL/glut.h>


#define KINECT_FORMAT_640X480 1
#define KINECT_FORMAT_1280X960 2

/*
类 kinect ：提供打开Kinect机器的函数，获取图像信息，关闭kinect机器
结构体： init_format；提供两个初始化kinect的像素格式，format_640x480和format_1280x960
每次调用ProcessColor函数都要更新状态，即调用Update()函数

使用流程：
//初始化
Kinect kinect;
kinect.CreateFirstConnected(kinect.m_init_format.format_640x480);
//获取图像信息
kinect.Update();
BYTE *bitMap;
bitMap=kinect.ProcessColor();
*/
class Kinect
{
public:
	Kinect();
	~Kinect();
//	int                     Run();
	/// <summary>
	/// Main processing function
	/// </summary>
	void                    Update();

	/// <summary>
	/// Create the first connected Kinect found 
	/// </summary>
	/// <returns>S_OK on success, otherwise failure code</returns>
	bool                 CreateFirstConnected(int format);

	/// <summary>
	/// 功能：  绘制背景kinect获取的背景图像
	///		    需要先调用BackgroundPoints(GLdouble fovy, GLdouble screen_aspect, GLfloat pos_z)函数设置位置
	/// </summary>
	void				Draw();

	/// <summary>
	/// 功能：  获取骨骼数据
	/// 参数：  1、骨骼数据，接收当前获取的骨骼数据
	/// 返回值：骨骼旋转
	/// </summary>
	bool			BoneData(NUI_SKELETON_DATA* skeletionData, NUI_SKELETON_BONE_ORIENTATION* boneData);
	
	/// <summary>
	/// 功能：  设置背景绘制位置
	/// 参数：  1、
	/// </summary>
	void BackgroundPoints(GLdouble fovy, GLdouble screen_aspect, GLfloat pos_z);

	/// <summary>
	/// 功能：  获取适配以后的屏幕宽、高与骨骼数据宽、高比值
	/// 参数：  1、屏幕宽与骨骼数据宽的比值 2、屏幕高与骨骼数据高的比值
	/// </summary>
	void getVideoSize(GLfloat* v_x, GLfloat* v_y);

private:
//	HWND                    m_hWnd;
	// Current Kinect
	INuiSensor*             m_pNuiSensor;

	HANDLE                  m_pColorStreamHandle;
	HANDLE                  m_hNextColorFrameEvent;
	HANDLE					m_hNextSkeletonFrameEvent;

	GLuint					m_texureId;
	GLfloat					(*m_bPoints)[3];

	int						m_format;
	NUI_SKELETON_DATA*		m_pSkeletonData;
//	NUI_SKELETON_BONE_ORIENTATION*		m_pBoneData;

	/// <summary>
	/// Handle new color data
	/// </summary>
	NUI_LOCKED_RECT*                    ProcessColor();

	NUI_SKELETON_FRAME skeletonFrame;

	GLfloat video_x; //适配以后的屏幕宽
	GLfloat video_y; //适配以后的屏幕高
	GLfloat video_z; //背景与屏幕的距离

	const GLfloat BONE_WIDTH_Z = 2.2 / 4.; //骨骼数据宽与z距离的比值(固定值)
	const GLfloat BONE_HEIGHT_Z = 1.6 / 4.; //骨骼数据与z距离的比值(固定值)
};

