#pragma once

void HandleKeyboard(unsigned char key)
{
	switch (key)
	{
	case '7':
		m_qtCon->control((QT_CONTROL)70);
		break;
	case '8':
		printf("m_gestureTracker.speed_up() %d", m_gestureTracker.speed_up());
		break;
	case '2':
		printf("m_gestureTracker.speed_down() %d", m_gestureTracker.speed_down());
		break;
	case '4':
		printf("m_gestureTracker.time_down() %d", m_gestureTracker.time_down());
		break;
	case '6':
		printf("m_gestureTracker.time_up() %d", m_gestureTracker.time_up());
		break;
	default:
		break;
	}
}

void selectFont(int size, int charset, const char* face)
{
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_BOLD, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void drawCNString(const char* str)
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);

	// �����ַ��ĸ���
	// �����˫�ֽ��ַ��ģ����������ַ����������ֽڲ���һ���ַ�
	// ����һ���ֽ���һ���ַ�
	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}

	// ������ַ�ת��Ϊ���ַ�
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';

	// �������ַ�
	for (i = 0; i<len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	// ����������ʱ��Դ
	free(wstring);
	glDeleteLists(list, 1);
}

void changeCostume() // �ı��װ����ѡ���װ��
{

	if (m_qtCon->isChange())
	{
		material = dataPath + m_qtCon->mNCloth + Separate + m_qtCon->mNCloth + materialSuffix;
		mesh = dataPath + m_qtCon->mNCloth + Separate + m_qtCon->mNCloth + meshSuffix;
		skeleton = dataPath + m_qtCon->mNCloth + Separate + m_qtCon->mNCloth + skeletonSuffix;
		switch (m_qtCon->cType)
		{
		case costumeType::HAT:
			if (hat.isLoading())
				break;
			hat.setCostume(material.c_str(), mesh.c_str(), skeleton.c_str());
			m_qtCon->change();
			break;
		case costumeType::CLOTH:
			if (cloth.isLoading())
				break;
			cloth.setCostume(material.c_str(), mesh.c_str(), skeleton.c_str());
			m_qtCon->change();
			break;
		case costumeType::TROUSER:
			if (trouser.isLoading())
				break;
			trouser.setCostume(material.c_str(), mesh.c_str(), skeleton.c_str());
			m_qtCon->change();
			break;
		case costumeType::SHOES:
			if (shoes.isLoading())
				break;
			shoes.setCostume(material.c_str(), mesh.c_str(), skeleton.c_str());
			m_qtCon->change();
			break;
		default:
			break;
		}
	}
}

void DrawAll(const NUI_SKELETON_BONE_ORIENTATION* boneData) //��������ͼ��
{
	GLfloat x = 0, y = 0;
	m_kinect->getVideoSize(&x, &y);
	vector2 vec(x, y);
	hat.drawCostume(boneData, &m_skeletonData, vec);
	cloth.drawCostume(boneData, &m_skeletonData, vec);
	trouser.drawCostume(boneData, &m_skeletonData, vec);
	shoes.drawCostume(boneData, &m_skeletonData, vec);
}

void init()
{
	m_skeletonData.Position.x = m_skeletonData.Position.y = m_skeletonData.Position.z = -0.09;
	m_kinect = new Kinect();

	if (m_kinect->CreateFirstConnected(KINECT_FORMAT_1280X960))
		printf("suss\n");
	else
		printf("not\n");
	//Sleep(1000);
	m_qtCon = new Qtcon();
	//Sleep(3000);

	//trouser.setCostume(material.c_str(), mesh.c_str(), skeleton.c_str());
}

void DrawString(const char* str, const GLfloat* rgb)
{
		selectFont(100, GB2312_CHARSET, "Ĭİ�ż���д��");  //���������ϵͳ�ļ���Ѱ�� 
		glColor3fv(rgb);     //������ɫ  
		glRasterPos3f(0.28, 0.3, 0.5);  //��ʼλ�� 
		drawCNString(str);   //������ַ���  
}

bool DrawLoadingString(const char* str) //������ʾ
{
	if (hat.isLoading() || cloth.isLoading() || trouser.isLoading() || shoes.isLoading())
	{
		DrawString(str, wait_color);
		return true;
	}
	else
	{
		return false;
	}
}

void ScreenShot() //����
{
	if (screenshot_time >= 0 && screenshot_time <= 3)
	{
		if (0 == screenshot_time)
		{
			clock_past = clock();
			screenshot_time++;
		}
		if ((clock() - clock_past) / CLOCKS_PER_SEC >= 1)
		{
			clock_past = clock();
			screenshot_time++;
		}
		if (screenshot_time > 3)
		{
			screenshot_time = -2;
			printf("screenshot_time:%d\n", screenshot_time);
			return;
		}
		char str[20];
		sprintf(str, "���յ���ʱ��%d", 4 - screenshot_time);
		DrawString(str, wait_color);
	}
	else if (-2 == screenshot_time)
	{
		m_qtCon->control(QT_CONTROL::SCREENSHOT);
		screenshot_time = -1;
		printf("screenshot_time:%d\n", screenshot_time);
		return;
	}
}

void HandleGesture()
{

	m_gestrueState = m_gestureTracker.track_it_per_time(&m_skeletonData);
	
	if (m_gestrueState.ON_OFF) { printf("ON\n"); m_qtCon->control(QT_CONTROL::ON_OFF); return; }
	if (m_gestrueState.LEFT) { printf("LEFT\n"); m_qtCon->control(QT_CONTROL::RIGHT); }
	if (m_gestrueState.RIGHT) { printf("RIGHT\n"); m_qtCon->control(QT_CONTROL::LEFT); }
	if (m_gestrueState.UP) { printf("UP\n"); m_qtCon->control(QT_CONTROL::UP); }
	if (m_gestrueState.DOWN) { printf("DOWN\n"); m_qtCon->control(QT_CONTROL::DOWN); }
	if (m_gestrueState.SCREENSHOT) { printf("SCREENSHOT\n"); screenshot_time = 0; }
}

/*
 *���ݸ�����·����ȫ����ʾͼ�񣨻������ţ�
 */
void Show_SYASHIN(const char* bmpPath,float width,float height)
{
	if (strcmp(m_bmpPath, bmpPath))
	{
		if (-1 != m_bmpTextureId)
			glDeleteTextures(1, (GLuint *)&m_bmpTextureId);
		m_bmpTextureId = ogl_tex_bmp(bmpPath, GL_LINEAR_MIPMAP_LINEAR);
		strcpy(m_bmpPath, bmpPath);
	}
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, m_bmpTextureId);
	glScalef(-1, 1, 1);
	glBegin(GL_QUAD_STRIP);

	glTexCoord2f(0., 1.);
	glVertex3f(float(width) / float(height)  * -1., -1., 1);

	glTexCoord2f(1., 1.);
	glVertex3f(float(width) / float(height) * 1., -1., 1);


	glTexCoord2f(0., 0.);
	glVertex3f(float(width) / float(height)  * -1., 1., 1);

	glTexCoord2f(1., 0.);
	glVertex3f(float(width) / float(height) * 1., 1., 1);

	glEnd();
}

bool Wait_Opengl(int second) //�����ȴ�
{
	if (wait_past == -1) //��һ��
		wait_past = clock();

	bool result = (((clock() - wait_past) / CLOCKS_PER_SEC) >= second);
	if (result)
		wait_past = -1;
	return result;
}

bool OverThanAngle(int angle, Vector4 qua)
{
	matrix33 hipCenterMatrix(quaternion(qua.x, qua.y, qua.z, qua.w));
	float hipCentAngle = atan2(-hipCenterMatrix.m[2][0], sqrt((pow(hipCenterMatrix.m[2][1], 2) + pow(hipCenterMatrix.m[2][2], 2))));
	char hint[50];
	sprintf(hint, "����ת������%d��", angle);
	if (abs(hipCentAngle) >= 3.1415926 / 180. * (float)angle)
	{
		DrawString(hint, warning_color);
		return true;
	}
	else
	{
		return false;
	}
}