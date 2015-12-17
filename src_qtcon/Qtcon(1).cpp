#include "Qtcon.h"
#pragma comment(lib, "Shell32.lib")

DWORD WINAPI receiveMessage(LPVOID lpParameter)
{
	lpThread_Data DATA = (lpThread_Data) lpParameter;
	char clothMessage[100];
	std::string cmd, data;
	int pos;
	while (1)
	{
		recv(*DATA->socketConn, clothMessage, sizeof(clothMessage), 0);
		cmd = data = clothMessage;

		pos = cmd.find(":");
		if (-1 != pos)
		{
			cmd = cmd.substr(0, pos);
			data = data.substr(pos + 1);
			if ("HAT" == cmd)
			{
				*DATA->cType = costumeType::HAT;
				strcpy(DATA->mNCloth, data.c_str());
			}
			else if ("CLOTH" == cmd)
			{
				*DATA->cType = costumeType::CLOTH;
				strcpy(DATA->mNCloth, data.c_str());
			}
			else if ("TROUSER" == cmd)
			{
				*DATA->cType = costumeType::TROUSER;
				strcpy(DATA->mNCloth, data.c_str());
			}
			/*
			else if ("SHOES" == cmd)
			{
				*DATA->cType = costumeType::SHOES;
				strcpy(DATA->mNCloth, data.c_str());
			}*/
			else if ("SHOW_OPENGL" == cmd)
			{
				glutShowWindow();
				LPRECT windowRect = new tagRECT;
				HWND desktop = GetDesktopWindow();
				GetClientRect(desktop, windowRect);
				glutReshapeWindow(windowRect->right - windowRect->left + 5, windowRect->bottom - windowRect->top + 5);
				printf("width:%d height:%d", windowRect->right - windowRect->left, windowRect->bottom - windowRect->top);

				//HWND opengl_handle = FindWindow(NULL, "3D试衣系统");
				//ShowWindow(opengl_handle, SW_MAXIMIZE);
				glutPositionWindow(-5, -5);
				//SetWindowPos(opengl_handle, NULL, -5, -5, -5, -5, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_DRAWFRAME);
			}
			else if ("HIDE_OPENGL" == cmd)
			{
				glutHideWindow();
			}
			else if ("SCREENSHOT_END" == cmd)
			{
				//printf("message:%s type:%s pos:%d\n", cmd, data.c_str(), pos);
				*DATA->cType = costumeType::SCREENSHOT_END;
				strcpy(DATA->syashinPath, data.c_str());
			}
		}
		else                            
		{
			//strcpy(Qtcon::mNCloth, "P21");
			*DATA->cType = costumeType::DEFAULT;
		}
		printf("message:%s type:%d\n", clothMessage);
	}
	return 1;
}
Qtcon::Qtcon()
{
	strcpy(syashinPath, ""); //初始化
	strcpy(mNCloth, ""); //初始化
	strcpy(mPCloth, ""); //初始化

	DATA.cType = &cType;
	DATA.mNCloth = mNCloth;
	DATA.syashinPath = syashinPath;

	//初始化socket编程环境
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	while (WSAStartup(wVersionRequested, &wsaData))
		Sleep(1000); //阻塞

	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PORT);

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	listen(sockSrv, 1);
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR_IN);

	//int a = 0;
	char fName[FILENAME_MAX];
	GetModuleFileName(NULL, fName, FILENAME_MAX);
	*strrchr(fName, '\\') = '\0';
	strcat_s(fName, FILENAME_MAX, "\\QT\\3d_fitting.exe");
	printf(fName);
	while ((int)ShellExecute(0, "open",  fName, "", "", SW_SHOWNORMAL) <= 31)
	{ 
		..............................................printf("test\n");
		Sleep(1000); //阻塞
	}
	//system("G:/t/Skeletal_Animation_Tutorial/bin32/QT/3d_fitting.exe");
	//printf("accept!\n");
	socketConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len); //阻塞,直到程序开始

	DATA.socketConn = &socketConn;

	recThread = CreateThread(NULL, 0, receiveMessage, &DATA, 0, NULL);
}

void Qtcon::sendMessage(int con)
{
	char message[3];
	sprintf(message, "%d", con);

	send(socketConn, message, strlen(message) + 1, 0); //并没有检查是否错误
}

void Qtcon::control(QT_CONTROL con)
{
	sendMessage(con);
}

void Qtcon::close()
{
	CloseHandle(recThread);
	closesocket(socketConn);
}