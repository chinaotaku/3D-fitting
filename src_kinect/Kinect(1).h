#pragma once

#include <Windows.h>
#include <NuiApi.h>
#include <string>
#include <glew.h>
#include <GL/glut.h>


#define KINECT_FORMAT_640X480 1
#define KINECT_FORMAT_1280X960 2

/*
�� kinect ���ṩ��Kinect�����ĺ�������ȡͼ����Ϣ���ر�kinect����
�ṹ�壺 init_format���ṩ������ʼ��kinect�����ظ�ʽ��format_640x480��format_1280x960
ÿ�ε���ProcessColor������Ҫ����״̬��������Update()����

ʹ�����̣�
//��ʼ��
Kinect kinect;
kinect.CreateFirstConnected(kinect.m_init_format.format_640x480);
//��ȡͼ����Ϣ
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
	/// ���ܣ�  ���Ʊ���kinect��ȡ�ı���ͼ��
	///		    ��Ҫ�ȵ���BackgroundPoints(GLdouble fovy, GLdouble screen_aspect, GLfloat pos_z)��������λ��
	/// </summary>
	void				Draw();

	/// <summary>
	/// ���ܣ�  ��ȡ��������
	/// ������  1���������ݣ����յ�ǰ��ȡ�Ĺ�������
	/// ����ֵ��������ת
	/// </summary>
	bool			BoneData(NUI_SKELETON_DATA* skeletionData, NUI_SKELETON_BONE_ORIENTATION* boneData);
	
	/// <summary>
	/// ���ܣ�  ���ñ�������λ��
	/// ������  1��
	/// </summary>
	void BackgroundPoints(GLdouble fovy, GLdouble screen_aspect, GLfloat pos_z);

	/// <summary>
	/// ���ܣ�  ��ȡ�����Ժ����Ļ������������ݿ��߱�ֵ
	/// ������  1����Ļ����������ݿ�ı�ֵ 2����Ļ����������ݸߵı�ֵ
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

	GLfloat video_x; //�����Ժ����Ļ��
	GLfloat video_y; //�����Ժ����Ļ��
	GLfloat video_z; //��������Ļ�ľ���

	const GLfloat BONE_WIDTH_Z = 2.2 / 4.; //�������ݿ���z����ı�ֵ(�̶�ֵ)
	const GLfloat BONE_HEIGHT_Z = 1.6 / 4.; //����������z����ı�ֵ(�̶�ֵ)
};

