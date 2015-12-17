#pragma once

static const int BGM_NUM = 10;
char* BGMList[BGM_NUM] = { "../BGM/��ʯ�� - ����ɢ�i - �����ְ�.mp3", 
						   "../BGM/��Զ������ - �ձ�acg.mp3", 
						   "../BGM/��ʯ�� - ���Ĥ�ζȤǤ� - ���ٰ洿����.mp3", 
						   "../BGM/������ - С������ - �ͺ�.mp3",
						   "../BGM/Ī���� �����������.mp3",
						   "../BGM/������� - �������� - ���ٰ洿����.mp3",
						   "../BGM/����� - С������ - ������ ��������������.mp3",
						   "../BGM/ҹ�� - Ф��.mp3",
						   "../BGM/Chopin - ������Բ���� - Ф�� �� E ���.mp3",
						   "../BGM/���� - һ�˾� - ����.mp3"};

DWORD WINAPI LoopBGM(LPVOID lpParameter) //ѭ������BGM
{
	
	MCI_OPEN_PARMS OpenParms;//���豸
	OpenParms.lpstrDeviceType = (char*)MCI_ALL_DEVICE_ID;// "mpegvideo";

	OpenParms.wDeviceID = NULL;

	MCI_PLAY_PARMS PlayParms;//�����ļ�
	PlayParms.dwFrom = 0;

	while (1)
	{
		for (int i = 0; i < BGM_NUM; i++)
		{
			printf("%s:\n", BGMList[i]);
			OpenParms.lpstrElementName = BGMList[i];
			printf("%d\n", mciSendCommand(0, MCI_OPEN,
				MCI_OPEN_ELEMENT | MCI_WAIT, (DWORD)&OpenParms));
			mciSendCommand(OpenParms.wDeviceID, MCI_PLAY, MCI_WAIT, (DWORD)&PlayParms);
		}
	}
}

void LoopBGM()
{
	CreateThread(NULL, 0, LoopBGM, NULL, 0, NULL);
}