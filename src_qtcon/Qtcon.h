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
	char* mNCloth = nullptr; //輝念議捲廾揃抄
	costumeType* cType = nullptr; //輝念議捲廾議嶽窃
	char* syashinPath = nullptr;//輝念孚頭議揃抄
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
	**畠蕉延楚音親僥��枯扮寂��參朔俐個
	*/
	///////////////////////////////////////////////
	char mNCloth[50]; //輝念議捲廾揃抄
	costumeType cType = costumeType::DEFAULT; //輝念議捲廾議嶽窃

	char syashinPath[100];
	///////////////////////////////////////////////
private:
	void sendMessage(int con);
	void close(); //潤崩

	SOCKET socketConn = NULL;
	HANDLE recThread;
	char mPCloth[50]; // 狛肇捲廾揃抄

	Thred_Data DATA;
	const int PORT = 25252; //極笥催�困砲辰海砲辰海蓬`
	const char* QT_PATH = "../bin32/QT/3d_fitting.exe"; //Qt殻會議揃抄 G:/t/tcp_client/Debug/tcp_client.exe
	};