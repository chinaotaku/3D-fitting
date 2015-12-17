#pragma once

static const int BGM_NUM = 10;
char* BGMList[BGM_NUM] = { "../BGM/久石让 - 空中散歩 - 纯音乐版.mp3", 
						   "../BGM/久远寺有珠 - 日本acg.mp3", 
						   "../BGM/久石让 - いつも何度でも - 钢琴版纯音乐.mp3", 
						   "../BGM/钢琴曲 - 小步舞曲 - 巴赫.mp3",
						   "../BGM/莫扎特 土耳其进行曲.mp3",
						   "../BGM/网络歌手 - 亡灵序曲 - 钢琴版纯音乐.mp3",
						   "../BGM/贝多芬 - 小步舞曲 - 钢琴曲 世界著名钢琴曲.mp3",
						   "../BGM/夜曲 - 肖邦.mp3",
						   "../BGM/Chopin - 华丽大圆舞曲 - 肖邦 降 E 大调.mp3",
						   "../BGM/姫神 - 一人静 - 姬神.mp3"};

DWORD WINAPI LoopBGM(LPVOID lpParameter) //循环播放BGM
{
	
	MCI_OPEN_PARMS OpenParms;//打开设备
	OpenParms.lpstrDeviceType = (char*)MCI_ALL_DEVICE_ID;// "mpegvideo";

	OpenParms.wDeviceID = NULL;

	MCI_PLAY_PARMS PlayParms;//播放文件
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