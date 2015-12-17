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
	char* mNCloth = nullptr; //当前的服装路径
	costumeType* cType = nullptr; //当前的服装的种类
	char* syashinPath = nullptr;//当前照片的路径
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
	**全局变量不科学，赶时间，以后修改
	*/
	///////////////////////////////////////////////
	char mNCloth[50]; //当前的服装路径
	costumeType cType = costumeType::DEFAULT; //当前的服装的种类

	char syashinPath[100];
	///////////////////////////////////////////////
private:
	void sendMessage(int con);
	void close(); //结束

	SOCKET socketConn = NULL;
	HANDLE recThread;
	char mPCloth[50]; // 过去服装路径

	Thred_Data DATA;
	const int PORT = 25252; //端口号：にっこにっこにー
	const char* QT_PATH = "../bin32/QT/3d_fitting.exe"; //Qt程序的路径 G:/t/tcp_client/Debug/tcp_client.exe
	};