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
const char* SOFTNAME = "3D����ϵͳ";
/*
** KUMAHIME��д ��������������
*/
#include "Qtcon.h"
#include "Kinect.h"
/////////////////////////////////////////////
/*
** TOMMEGO��д ��������һ����
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
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") //ȥ������̨
///////////////////////////////////////////
/*
** Mesh����Ҫ���õ���һЩ��
*/
#include "core.h"
#include "Bmp.h"
#include "ogl.h"
#include "glsl.h"
///////////////////////////////////////////
/*
** Mesh 3Dģ�Ͷ�ȡ����Ҫ��(����Mesh.h�����Ƚ϶࣬�������а���������һ��KUMAHIME���̳е���Costume)
*/
vec4f lightvec(1, 1, -1, 0); //����������Mesh.h֮ǰ��������շ����ǿ��
#include "Mesh.h"
///////////////////////////////////////////
/*
 ** KUMAHIME��д ��Ҫ�õ��ĺ��������ݵĴ��
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
	gluPerspective(90.0,float(vp[2])/float(vp[3]) , 0.01, 100); //�ӽ����� 
	m_kinect->BackgroundPoints(90.0, float(vp[2]) / float(vp[3]), -50);

	// modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(180, 0, 1, 0);
	//glRotatef(viewangle_y,1,0,0);		// set rotation
	//glRotatef(viewangle_x,0,1,0);		// set rotation

	if (m_qtCon->cType == costumeType::SCREENSHOT_END && !Wait_Opengl(5))
	{
		DrawString("������Ƭ", wait_color);
		Show_SYASHIN(m_qtCon->syashinPath, vp[2], vp[3]); //������Ƭ��������	
		glutSwapBuffers();
		return;
	}
	else if (m_qtCon->cType == costumeType::SCREENSHOT_END)
	{
		m_qtCon->cType = costumeType::DEFAULT;
	}

	m_kinect->Draw(); //����kinect��ȡ��ʵʱͼ��

	if (DrawLoadingString("��������"))//[������MethodOfMain.h]
	{
		GET_GESTURE = false;
	}
	else
	{
		GET_GESTURE = true;
	}
	ScreenShot(); //���� �ܵ����ƿ���

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
			HandleGesture();// ��������
		DrawAll(boneData); // ���Ƹ��ݹ����������з�װ��hat cloth trouser shoes��[������MethodOfMain.h]
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

static void idle() //��ѭ��
{
	changeCostume(); //�ı��װ [������MethodOfMain.h]
	m_kinect->Update(); //m_kinectˢ��
	glutPostRedisplay(); //����
}

static void reshape(int width, int height)
{

}

int main(int argc, char **argv) 
{ 
  glutInit(&argc, argv);  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutCreateWindow("3D����ϵͳ");
  HWND opengl_handle = FindWindow(NULL, "3D����ϵͳ");
  SetWindowLong(opengl_handle, GWL_STYLE, GetWindowLong(opengl_handle, GWL_STYLE) & ~WS_CAPTION);
 // glutFullScreen();
  glutHideWindow();

  init(); //����ʾ֮ǰ��ʼ��
  glutDisplayFunc(DrawScene);
  glutIdleFunc(&idle);
  glutKeyboardFunc(keyboard);
//  glutReshapeFunc(reshape);
  //glutTimerFunc(20, timer, -1);
  glewInit();
  wglSwapIntervalEXT(0); //��ֱͬ��
  //LoopBGM(); // ����BGM
  glutMainLoop();
  return 0;
}
///////////////////////////////////////////