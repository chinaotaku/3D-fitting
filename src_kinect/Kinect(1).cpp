#include "Kinect.h"
#include "Bmp.h"
#include <iostream>
#include <strsafe.h>
using namespace std;

void Kinect::BackgroundPoints(GLdouble fovy, GLdouble screen_aspect, GLfloat pos_z)
{
	if (m_bPoints == NULL) 
		m_bPoints = new GLfloat[4][3];

	GLdouble video_aspect;
	if (m_format == KINECT_FORMAT_1280X960)
		video_aspect = (GLfloat)1280 / (GLfloat)960;
	else
		video_aspect = (GLfloat)640 / (GLfloat)480;
	
	//顶点初始化 
	GLfloat back_p[4][3] = { { -1., -1., 1. }, { 1., -1., 1. },    //0 左下方 1 右下方 
	{ -1., 1., 1. }, { 1., 1., 1. } }; //2 左上方 3 右上方


	GLdouble screen_y = fabs(pos_z) * tan(fovy / 2.f * 3.14f / 180.f);//与视点距离pos_z的屏幕高的1/2
	GLdouble screen_x = screen_y * screen_aspect; //与视点距离pos_z的屏幕宽的1/2
	video_z = pos_z;

	if ((video_aspect / screen_aspect) > 1) //视频宽的相对比例大
	{
		video_x = screen_x;
		video_y = video_x / video_aspect;
	}
	else
	{
		video_y = screen_y;
		video_x = video_y * video_aspect;
	}

	for (int i = 0; i < 4; i++)
	{
		back_p[i][0] *= video_x;
		m_bPoints[i][0] = back_p[i][0];
	}

	for (int i = 0; i < 4; i++)
	{
		back_p[i][1] *= video_y;
		m_bPoints[i][1] = back_p[i][1];
	}

	for (int i = 0; i < 4; i++)
	{
		back_p[i][2] *= pos_z;
		m_bPoints[i][2] = back_p[i][2];
	}

}

void Kinect::getVideoSize(GLfloat* v_x, GLfloat* v_y)
{
	*v_x = abs(video_x / video_z / BONE_WIDTH_Z);
	*v_y = abs(video_y / video_z / BONE_HEIGHT_Z);
}

Kinect::Kinect() :m_hNextColorFrameEvent(INVALID_HANDLE_VALUE),
m_pColorStreamHandle(INVALID_HANDLE_VALUE),
m_pNuiSensor(NULL),
m_bPoints(NULL),
m_pSkeletonData(&skeletonFrame.SkeletonData[0])
//m_pBoneData(new NUI_SKELETON_BONE_ORIENTATION[NUI_SKELETON_POSITION_COUNT])
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &m_texureId);
	glBindTexture(GL_TEXTURE_2D, m_texureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//glBindTexture(GL_TEXTURE_2D, 0);
	//m_bPoints = NULL;
	//m_pSkeletonData = NULL;
	//m_pBoneData = new NUI_SKELETON_BONE_ORIENTATION;
}


Kinect::~Kinect()
{
	if (m_pNuiSensor)
	{
		m_pNuiSensor->NuiShutdown();
	}

	if (m_hNextColorFrameEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hNextColorFrameEvent);
	}
}

//更新kinect状态
void Kinect::Update()
{
//	cout << m_pNuiSensor;
	if (NULL == m_pNuiSensor)
	{
		return;
	}
	if (WAIT_OBJECT_0 == WaitForSingleObject(m_hNextColorFrameEvent, 0))
	{
		NUI_LOCKED_RECT* kRect = ProcessColor();

		if (kRect != NULL) 
		{
			byte* KPixls = kRect->pBits;

			byte k = 0;

			/*
			 * 数据转化为rgba，否则图像颜色会出错
			 */
			for (int i = 0; i < kRect->size; i += 4)
			{
				k = KPixls[i];
				KPixls[i] = KPixls[i + 2];
				KPixls[i + 2] = k;
				KPixls[i + 3] = 255;
			}

			glBindTexture(GL_TEXTURE_2D, m_texureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 960, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, KPixls);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

//产生第一个连接
bool Kinect::CreateFirstConnected(int format)
{
	INuiSensor * pNuiSensor;
	HRESULT hr;

	int iSensorCount = 0;
	hr = NuiGetSensorCount(&iSensorCount);
	if (FAILED(hr))
	{
		return false;
	}

	// Look at each Kinect sensor
	for (int i = 0; i < iSensorCount; ++i)
	{
		// Create the sensor so we can check status, if we can't create it, move on to the next
		hr = NuiCreateSensorByIndex(i, &pNuiSensor);
		if (FAILED(hr))
		{
			continue;
		}

		// Get the status of the sensor, and if connected, then we can initialize it
		hr = pNuiSensor->NuiStatus();
		if (S_OK == hr)
		{
			m_pNuiSensor = pNuiSensor;
			break;
		}

		// This sensor wasn't OK, so release it since we're not using it
		pNuiSensor->Release();
	}

	if (NULL != m_pNuiSensor)
	{
		// Initialize the Kinect and specify that we'll be using color
		hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON);
		if (SUCCEEDED(hr))
		{
			// Create an event that will be signaled when color data is available
			m_hNextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			// Create an event that will be signaled when skeleton data is available
			m_hNextSkeletonFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			// Open a color image stream to receive color frames
			if (KINECT_FORMAT_640X480 == format)
			{
				hr = m_pNuiSensor->NuiImageStreamOpen(
					NUI_IMAGE_TYPE_COLOR,
					NUI_IMAGE_RESOLUTION_640x480,
					0,
					2,
					m_hNextColorFrameEvent,
					&m_pColorStreamHandle);
				m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonFrameEvent, 0);
			}
			else if (KINECT_FORMAT_1280X960 == format)
			{
				hr = m_pNuiSensor->NuiImageStreamOpen(
					NUI_IMAGE_TYPE_COLOR,
					NUI_IMAGE_RESOLUTION_1280x960,
					0,
					2,
					m_hNextColorFrameEvent,
					&m_pColorStreamHandle);
				m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonFrameEvent, 0);
			}
			else
			{
				return false;
			}
			m_format = format;
		}
	}

	if (NULL == m_pNuiSensor || FAILED(hr))
	{
		//        SetStatusMessage(L"No ready Kinect found!");
		return E_FAIL;
	}

	return true;
}


NUI_LOCKED_RECT*  Kinect::ProcessColor()
{
	
	HRESULT hr;
	NUI_IMAGE_FRAME imageFrame;
	BYTE *picture_address;

	//Attempt to get the skeleton frame
	hr = m_pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);
	//if (FAILED(hr))
	{
	//	printf("no\n");
	}

	// smooth out the skeleton data
	m_pNuiSensor->NuiTransformSmooth(&skeletonFrame, NULL);

	// Attempt to get the color frame
	hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pColorStreamHandle, 0, &imageFrame);
	if (FAILED(hr))
	{
		return NULL;
	}
//	printf("seikou!\n");
	INuiFrameTexture * pTexture = imageFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;

	// Lock the frame data so the Kinect knows not to modify it while we're reading it
	pTexture->LockRect(0, &LockedRect, NULL, 0);

	// Make sure we've received valid data
	if (LockedRect.Pitch != 0)
	{
		//set a byte array to a object
		//		m_pDrawColor->Draw(static_cast<BYTE *>(LockedRect.pBits), LockedRect.size);
		picture_address = static_cast<BYTE *>(LockedRect.pBits);

	}

	// We're done with the texture so unlock it
	pTexture->UnlockRect(0);

	// Release the frame
	m_pNuiSensor->NuiImageStreamReleaseFrame(m_pColorStreamHandle, &imageFrame); 
	return &LockedRect;
}
bool Kinect::BoneData(NUI_SKELETON_DATA* skeletonData, NUI_SKELETON_BONE_ORIENTATION* boneData)
{
	
	HRESULT hr = -1;
	loopi(0, NUI_SKELETON_COUNT)
	{
		if (NUI_SKELETON_TRACKED == m_pSkeletonData[i].eTrackingState)
		{
			//printf("cout:%d\n", i);
			hr = NuiSkeletonCalculateBoneOrientations(&m_pSkeletonData[i], boneData);
			if (SUCCEEDED(hr))
			{
				//printf("suceed!\n");
				//getchar();
				*skeletonData = m_pSkeletonData[i];
				//printf("ske:%d %d bone:%d %d\n", skeletonData, m_pSkeletonData, boneData, m_pBoneData);
				//getchar();
				break;
			}
		}
	}
	if (FAILED(hr))
		return false;
	return true;
}

void Kinect::Draw()
{
	if (NULL == m_bPoints)
	{
		printf("please set m_bPoints!\n");
		return;
	}
	glPushMatrix();
	glRotatef(180, 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, m_texureId);
	glBegin(GL_QUAD_STRIP);

	//glTexCoord2f(1., 1.);
	glTexCoord2f(0., 1.);
	glVertex3fv(m_bPoints[0]);

	//glTexCoord2f(0., 1.);
	glTexCoord2f(1., 1.);
	glVertex3fv(m_bPoints[1]);

	//glTexCoord2f(1., 0.);
	glTexCoord2f(0., 0.);
	glVertex3fv(m_bPoints[2]);

	//glTexCoord2f(0., 0.);
	glTexCoord2f(1., 0.);
	glVertex3fv(m_bPoints[3]);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
///////////////////////////////////////////////////////////////////////////////
	/*
	Vector4 data;
	LONG x, y;
	USHORT depth;

	loopi(0, NUI_SKELETON_COUNT)
	{
		
		if (NUI_SKELETON_TRACKED == m_pSkeletonData[i].eTrackingState)
		{
			//printf("cout:%d\n", i);
			loopj(0, NUI_SKELETON_POSITION_COUNT)
			{			
				data = m_pSkeletonData[i].SkeletonPositions[j];
				NuiTransformSkeletonToDepthImage(data, &x, &y, &depth);
				glPushMatrix();
				glColor3f(1, 1, 1);
				//glRotatef(180., 0, 1, 0);
				glTranslatef(m_bPoints[2][0] / m_bPoints[2][2], m_bPoints[2][1] / m_bPoints[2][2], 0);  
				glTranslatef(data.x, data.y, data.z * -1);		
				//printf("x:%f y:%f z:%f\n", data.x, data.y,data.z);
				glutSolidSphere(0.1, 10, 10);
				glPopMatrix();
			}
		}
	}*/
///////////////////////////////////////////////////////////////////////////////

	glPopMatrix();
}
