#pragma once

#include "Winsock2.h"
#include <iostream>
#include "stdlib.h"
#include "../src_animation/Enum.h"
#include <process.h>
#include <glew.h>
#include <GL/glut.h>
#pragma comment(lib, "Ws2_32.lib")

typedef struct Thred_Data
{
	char* mNCloth = nullptr; //��ǰ�ķ�װ·��
	costumeType* cType = nullptr; //��ǰ�ķ�װ������
	char* syashinPath = nullptr;//��ǰ��Ƭ��·��
	SOCKET* socketConn = nullptr;
} *lpThread_Data;

class Qtcon
{
public:
	Qtcon();
	void control(QT_CONTROL con);
	bool isChange() { return (strcmp(mNCloth, mPCloth)); }
	void change()  { strcpy(mPCloth, mNCloth); }

	/*
	**ȫ�ֱ�������ѧ����ʱ�䣬�Ժ��޸�
	*/
	///////////////////////////////////////////////
	char mNCloth[50]; //��ǰ�ķ�װ·��
	costumeType cType = costumeType::DEFAULT; //��ǰ�ķ�װ������

	char syashinPath[100];
	///////////////////////////////////////////////
private:
	void sendMessage(int con);
	void close(); //����

	SOCKET socketConn = NULL;
	HANDLE recThread;
	char mPCloth[50]; // ��ȥ��װ·��

	Thred_Data DATA;
	const int PORT = 25252; //�˿ںţ��ˤä��ˤä��˩`
	const char* QT_PATH = "../bin32/QT/3d_fitting.exe"; //Qt�����·�� G:/t/tcp_client/Debug/tcp_client.exe
	};