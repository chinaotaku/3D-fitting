/////////////////////////////////////////////
//
// Skeletal Animation Tutorial
//
// (C) by Sven Forstmann in 2014
//
// License : MIT
// http://opensource.org/licenses/MIT
/////////////////////////////////////////////
// Mathlib included from 
// http://sourceforge.net/projects/nebuladevice/
///////////////////////////////////////////////
const char* SOFTNAME = "3D试衣系统";
/*
** KUMAHIME编写 面向对象的两个类
*/
#include "Qtcon.h"
#include "Kinect.h"
/////////////////////////////////////////////
/*
** TOMMEGO编写 面向对象的一个类
*/
#include "GestureTracker.h"
////////////////////////////////////////////
//#include <iostream> 
#include <vector> 
#include <string> 
#include <stdio.h>
#include <glew.h>
#include <wglew.h>
#include <windows.h>
#include <mmsystem.h>
#include <GL/glut.h>
#include <strsafe.h>

#pragma comment(lib,"winmm.lib")
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") //去掉控制台
///////////////////////////////////////////
/*
** Mesh中需要调用到的一些库
*/
#include "core.h"
#include "Bmp.h"
#include "ogl.h"
#include "glsl.h"
///////////////////////////////////////////
/*
** Mesh 3D模型读取的主要类(由于Mesh.h依赖比较多，所以其中包含了另外一个KUMAHIME所继承的类Costume)
*/
vec4f lightvec(1, 1, -1, 0); //必须设置在Mesh.h之前，定义光照方向和强度
#include "Mesh.h"
///////////////////////////////////////////
/*
 ** KUMAHIME编写 需要用到的函数和数据的存放
 */
#include "DATA.h"
#include "BGM.h"
#include "MethodOfMain.h"
//////////////////////////////////////////

void DrawScene()
{
	// select level of detail for rendering
	// (lods are generated automatically by the ogre xml converter )

	if (GetAsyncKeyState(VK_F1))  screenshot_time = 0; 
	if (GetAsyncKeyState(VK_ESCAPE)) { m_qtCon->control(QT_CONTROL::EXIT); Sleep(500); _exit(0); }

	//glutHideWindow();
	//glutShowWindow();
	//HWND window = FindWindow(NULL, "Skinned Skeletal Animation Sample (c) Sven Forstmann in 2014");
	//SendMessage(window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);

	// mouse pointer position
	//POINT cursor;
	//GetCursorPos(&cursor); 

	// camera orientation
	//float	viewangle_x = float(cursor.x-1280/2)/4.0;
	//float	viewangle_y = float(cursor.y-768/2)/4.0;  

	//time
	static uint t0=timeGetTime(); 
	double time_elapsed=double(timeGetTime()-t0)/1000;

	// clear and basic
	glClearDepth(1);
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// projection
	int vp[4];
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glGetIntegerv(GL_VIEWPORT, vp);
	gluPerspective(90.0,float(vp[2])/float(vp[3]) , 0.01, 100); //视角设置 
	m_kinect->BackgroundPoints(90.0, float(vp[2]) / float(vp[3]), -50);

	// modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(180, 0, 1, 0);
	//glRotatef(viewangle_y,1,0,0);		// set rotation
	//glRotatef(viewangle_x,0,1,0);		// set rotation

	if (m_qtCon->cType == costumeType::SCREENSHOT_END && !Wait_Opengl(5))
	{
		DrawString("拍摄照片", wait_color);
		Show_SYASHIN(m_qtCon->syashinPath, vp[2], vp[3]); //绘制照片，有缩放	
		glutSwapBuffers();
		return;
	}
	else if (m_qtCon->cType == costumeType::SCREENSHOT_END)
	{
		m_qtCon->cType = costumeType::DEFAULT;
	}

	m_kinect->Draw(); //绘制kinect获取的实时图像

	if (DrawLoadingString("正在载入"))//[定义于MethodOfMain.h]
	{
		GET_GESTURE = false;
	}
	else
	{
		GET_GESTURE = true;
	}
	ScreenShot(); //拍照 受到手势控制

	NUI_SKELETON_BONE_ORIENTATION boneData[NUI_SKELETON_POSITION_COUNT];

	if (m_kinect->BoneData(&m_skeletonData, boneData))
	{
		glDisable(GL_CULL_FACE);
		if (OverThanAngle(50, boneData[NUI_SKELETON_POSITION_HIP_CENTER].absoluteRotation.rotationQuaternion))
		{
			glutSwapBuffers();
			return;
		}
		//glCullFace(GL_BACK);
		if (GET_GESTURE)
			HandleGesture();// 处理手势
		DrawAll(boneData); // 绘制根据骨骼数据所有服装（hat cloth trouser shoes）[定义于MethodOfMain.h]
	}
	glutSwapBuffers();
}
///////////////////////////////////////////

void keyboard(unsigned char key, int x1, int y1)
{
	HandleKeyboard(key);
}

static void timer(int p)
{
	glutPostRedisplay();
	m_kinect->Update();
	glutTimerFunc(16, timer, 25);
}

static void idle() //主循环
{
	changeCostume(); //改变服装 [定义于MethodOfMain.h]
	m_kinect->Update(); //m_kinect刷新
	glutPostRedisplay(); //绘制
}

static void reshape(int width, int height)
{

}

int main(int argc, char **argv) 
{ 
  glutInit(&argc, argv);  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutCreateWindow("3D试衣系统");
  HWND opengl_handle = FindWindow(NULL, "3D试衣系统");
  SetWindowLong(opengl_handle, GWL_STYLE, GetWindowLong(opengl_handle, GWL_STYLE) & ~WS_CAPTION);
 // glutFullScreen();
  glutHideWindow();

  init(); //在显示之前初始化
  glutDisplayFunc(DrawScene);
  glutIdleFunc(&idle);
  glutKeyboardFunc(keyboard);
//  glutReshapeFunc(reshape);
  //glutTimerFunc(20, timer, -1);
  glewInit();
  wglSwapIntervalEXT(0); //垂直同步
  //LoopBGM(); // 播放BGM
  glutMainLoop();
  return 0;
}
///////////////////////////////////////////