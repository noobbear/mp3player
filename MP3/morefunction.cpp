#include "stdafx.h"
#include "MainDlg.h"

TCHAR szFile[MAX_PATH]; //�����õ���ԭʼ�Ĵ��
TCHAR szPath[MAX_PATH]; //·��
TCHAR szFileName[50 * MAX_PATH]; //���������ÿ���ļ�·��������
//�ļ�·����ַ
TCHAR chMusic[MAX_PATH];
//��ǰ�����ļ���·����
TCHAR chShortName[MAX_PATH];
//�������ȣ�״̬����ֵ
TCHAR songLength[20];
static TCHAR musicTotalTime[256]; // ��¼һ�׸����ʱ��
static int totalSecond; // ��¼һ�׸������
HWND hwndMusicSileder;//���Ÿ����Ľ��������
HWND hwndVolumeSilder;//�������ƵĽ��������
int iVolume; //������С
//int count;//ͳ��ֵ
static int totalMusicCount = 0; // ��¼�б�ĸ���������
static int numplayingsong = 0;//������ǰ���ŵĸ�������� ��ʼ��Ϊ0
static int curTime; // ��ǰ�������ŵ�λ��
//int pMethod; //�Զ����ŷ�ʽ


void SetVolumeTrack(HWND hwnd)//��ʼ��������
{
	SendDlgItemMessage(hwnd, IDC_VOICE, TBM_SETRANGEMIN, false, 0);
	SendDlgItemMessage(hwnd, IDC_VOICE, TBM_SETRANGEMAX, false, 1000);
	SendDlgItemMessage(hwnd, IDC_VOICE, TBM_SETPOS, true, 200);
	iVolume = 1000 - 200;
}
void SetVolume(HWND hwnd, int volume)
 {
	TCHAR cmd[MAX_PATH];
    //int v = volume;
	wsprintf(cmd, "setaudio mysound volume to %d", volume);
	mciSendString(cmd, (LPTSTR)NULL, 0, 0);


//	wsprintf(szVolume, "setaudio mysound volume to %d", iVolume);
//	mciSendString(szVolume, (LPTSTR)NULL, 0, 0);
	//SendDlgItemMessage(hwnd, IDC_VOICE, TBM_SETPOS, true, volume);
	//iVolume = volume;
}
int getVolume(HWND hwnd)////��ȡ����ֵ
{
	TCHAR cmd[MAX_PATH];
	TCHAR buff[MAX_PATH];
//	wsprintf(cmd, "status %s volume", chShortName);
//	mciSendString(cmd, buff, sizeof(buff), 0);
	return 1000 - (SendMessage(GetDlgItem(hwnd, IDC_VOICE), TBM_GETPOS, 0, 0));

}

///////////////////////////////////////////////////////////////////
BOOL Main_VSCROll(HWND hwnd, HWND hwndctl, UINT code, int pos)//��ֱ����������������
{

		hwndVolumeSilder = GetDlgItem(hwnd, IDC_VOICE);
		if (hwndVolumeSilder == hwndctl)
		{
			if (TB_ENDTRACK == code) // ����ͷ�
			{
				iVolume = 1000 - (SendMessage(GetDlgItem(hwnd, IDC_VOICE), TBM_GETPOS, 0, 0));
				TCHAR szVolume[MAX_PATH];
				wsprintf(szVolume, "setaudio mysound volume to %d", iVolume);
				mciSendString(szVolume, (LPTSTR)NULL, 0, 0);
			}
		}
	return FALSE;
}



void JumpProcess(HWND hwnd, int pos)//�϶��������ӵ�ǰλ�ÿ�ʼ����
{
	TCHAR cmd[MAX_PATH + 20];
	wsprintf(cmd, "play mysound from %ld", pos * 1000);
	mciSendString(cmd, "", 0, NULL);
}
BOOL Main_HSCROLL(HWND hwnd, HWND hwndctl, UINT code, int pos)//ˮƽ��������������������
{
	TCHAR cmd[MAX_PATH];
	if (TB_ENDTRACK == code)
	{
	 HWND hwndTrack = GetDlgItem(hwnd, IDC_JINGDU);
	 int p = SendMessage(hwndTrack, TBM_GETPOS, 0, 0);
	 wsprintf(cmd, "play %s from %ld", chShortName, p *1000);
	 mciSendString(cmd, "", 0, NULL);
	 }
	hwndMusicSileder = GetDlgItem(hwnd, IDC_JINGDU);
	if (hwndMusicSileder == hwndctl) //��������Ŀؼ����ǻ���
	{
		if (TB_ENDTRACK == code) // ����ͷ�
		{
			int p = SendMessage(hwndctl, TBM_GETPOS, 0, 0);
			JumpProcess(hwnd, p);
		}
	}
	return 0;//��ʱ
}


void addSong(HWND hwnd)
{
	//TCHAR szFile[MAX_PATH]; //�����õ���ԭʼ�Ĵ��
	//TCHAR szPath[MAX_PATH]; //·��
	//TCHAR szFileName[50 * MAX_PATH]; //���������ÿ���ļ�·��������
	TCHAR *p;
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = TEXT('\0');
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("ALL\0*.*\0mp3\0*.mp3\0");
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.hwndOwner = hwnd;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
	if (GetOpenFileName(&ofn))
	{
		lstrcpyn(szPath, szFile, ofn.nFileOffset);
		szPath[ofn.nFileOffset] = '\0';
		if (szPath[ofn.nFileOffset - 1] != '\\')
		{
			szPath[ofn.nFileOffset - 1] = '\\';
		}
		ZeroMemory(szFileName, sizeof(szFileName));
		p = szFile + ofn.nFileOffset;
		int i = 0;
		while (*p)
		{
			lstrcat(szFileName, szPath);
			lstrcat(szFileName, p);
			SendDlgItemMessage(hwnd, IDC_SONG, LB_INSERTSTRING, totalMusicCount, (LPARAM)szFileName);
			p += strlen(p) + 1;
			i++;
			ZeroMemory(szFileName, sizeof(szFileName));
		}
		totalMusicCount = SendDlgItemMessage(hwnd, IDC_SONG, LB_GETCOUNT, 0, 0);
	}
}



///////////////////////////////////////
//����һ�׸�����ʱ��
void MusicTotalTime(HWND hwnd)
{
	TCHAR musicTotalLength[256];
	int intSecond, intMinute;
	mciSendString("status mysound length", musicTotalLength, sizeof(musicTotalLength) / sizeof(TCHAR), NULL);//��ø����ĳ���
	totalSecond = atoi(musicTotalLength) / 1000; //��ʱ�䣨���룩���� 1000 �õ���������������//ת����ʽ
	intMinute = totalSecond / 60; //���������� 6 �õ���������������
	intSecond = (totalSecond - intMinute * 60); //��������ȥ������������*60 �õ�ʣ������
	wsprintf(musicTotalTime, "%02d:%02d", intMinute, intSecond); //02s ��ʾʱ����ӻ���������λ�����㲹 0��
	SetDlgItemText(hwnd, IDC_TOTALTIME, musicTotalTime);
}



void CALLBACK MusicNowTime(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime)
{
	//��ȡ�������ŵĵ�ǰλ��
	TCHAR curPosition[256];
	mciSendString("status mysound position", curPosition, sizeof(curPosition) / sizeof(TCHAR), NULL); //ָ������λ��
	curTime = atol(curPosition);
	int iMinute = curTime / 60000;
	int iSecond = (curTime % 60000) / 1000;
	// ��ʾʱ��
	TCHAR sCurTime[256];
	wsprintf(sCurTime, "%02d:%02d", iMinute, iSecond);
	SetDlgItemText(hwnd, IDC_NOWTIME, sCurTime);
	if (0 == strcmp(sCurTime, musicTotalTime)) // �ж�һ�׸��Ƿ���
	{
		Sleep(200);
		Jumpmusic(hwnd,1);//ֱ��������һ��
	}
}



/*****
�������ƣ� SetProgressBar���ص�������
�������ܣ� ���Ÿ���ʱ�������Զ�����
*****/
void CALLBACK SetProgressBar(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime)
{
	TCHAR nowMusicTime[256];
	TCHAR totalLength[256];
	mciSendString("status mysound length", totalLength, sizeof(totalLength) / sizeof(TCHAR), NULL);//��ø����ĳ���
	SendDlgItemMessage(hwnd, IDC_JINGDU, TBM_SETRANGEMAX, false, (LPARAM)totalSecond);
	SendDlgItemMessage(hwnd, IDC_JINGDU, TBM_SETRANGEMIN, false, (LPARAM)0);
	mciSendString("status mysound position", nowMusicTime, sizeof(nowMusicTime) / sizeof(TCHAR), NULL);
	int musicSliderTime = atoi(nowMusicTime);
	musicSliderTime = musicSliderTime / 1000;
	SendDlgItemMessage(hwnd, IDC_JINGDU, TBM_SETPOS, true, (LPARAM)musicSliderTime);
}

////////////////////////////////////////
//��������
static int flagPauseOrPlay = 1;//����ȫ�ֱ����ж�����ͣ���Ǽ���
void StopMusic(HWND hwnd)//ֹͣ����
{
	HWND hwndPause = GetDlgItem(hwnd, IDC_PAUSE);
	if (1 == flagPauseOrPlay)
	{
		SetWindowText(hwndPause, TEXT("��ͣII")); //�޸İ�ť������
		flagPauseOrPlay = 0;
	}
	mciSendString("close mysound", NULL, 0, NULL);
}
void PlayMusic(HWND hwnd)
{
	HWND hwndPause = GetDlgItem(hwnd, IDC_PAUSE);
	if (1 == flagPauseOrPlay)
	{
		SetWindowText(hwndPause, TEXT("��ͣII")); //�޸İ�ť������
		flagPauseOrPlay = 0;
	}
	//����ֹͣ��ǰ���ŵĸ���
	StopMusic(hwnd);
	//�õ�Ҫ���ŵĸ���·��
	HWND listHwnd = GetDlgItem(hwnd, IDC_SONG);
	int curSelIndex = ListBox_GetCurSel(listHwnd);
	numplayingsong = curSelIndex;//����ǰ���ŵĸ�����¼��ȫ�ֱ�����
	TCHAR temp[MAX_PATH + 12] = "NULL";
	ListBox_GetText(listHwnd, curSelIndex, temp);
	if(strcmp(temp,"NULL")==0)
			MessageBox(hwnd, TEXT("��ǰû��ѡ�и��������б�Ϊ�գ�"), TEXT("��ܰ��ʾ"), MB_OK);
	else
	{
	GetShortPathName(temp, chMusic, sizeof(chMusic));

	TCHAR musicName[MAX_PATH];  
	SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETTEXT, numplayingsong, (LPARAM)musicName);
	SetDlgItemText(hwnd, IDC_NAME, musicName); // ��ʾ���ڲ��Ÿ���������

	TCHAR playCmd[MAX_PATH + 12];
	wsprintf(playCmd, "open %s alias mysound", chMusic);
	mciSendString(playCmd, NULL, 0, NULL);
	mciSendString("play mysound", NULL, 0, NULL);

	SetVolume(hwnd, iVolume);

	MusicTotalTime(hwnd); // ��ʾÿ�׸�������ʱ��
	SetTimer(hwnd, 1, 1000, MusicNowTime);
	SetTimer(hwnd, 2, 1000, SetProgressBar);
	
	}

}
///////////��ȡȥ��·���ĸ������Ա���ʾ


//////////////////////////////////////////////////////////////
void PauseMusic(HWND hwnd)
{
		HWND hwndPause = GetDlgItem(hwnd, IDC_PAUSE);
		if (1 == flagPauseOrPlay)
		{
			SetWindowText(hwndPause, TEXT("��ͣII")); //�޸İ�ť������
	        //����PlayMusic�����Ǳ����������ʱ���Ź�����ڵĸ���
			//TCHAR playCmd[MAX_PATH + 12];
			//wsprintf(playCmd, "open %s alias mysound", chMusic);
			//mciSendString(playCmd, NULL, 0, NULL);//�������ʹ����б��"����"��ť��Ч,���ɵ��������������б�ǰ���ڲ��ŵĸ�����bug
			mciSendString("play mysound", NULL, 0, NULL);
			flagPauseOrPlay = 0;
		}
		else if (0 == flagPauseOrPlay)
		{
			TCHAR playCmd[MAX_PATH + 12];
			wsprintf(playCmd, "open %s alias mysound", chMusic);
			mciSendString("pause mysound", NULL, 0, NULL);
			flagPauseOrPlay = 1;
			SetWindowText(hwndPause, TEXT("������")); //�޸İ�ť������
		}
}
int getCurrentPoint(HWND hwnd)//��ȡ��ǰ��ѡ�еģ�����
{
	int i = SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETCURSEL, i, (LPARAM)chMusic);
	//curTime = i;
	return i;
}
//////////////////////////
///�����׸�

void Jumpmusic(HWND hwnd,int num)
{
	int playorstop = 0;
	if(flagPauseOrPlay==1)
	{
		flagPauseOrPlay = 0;
		HWND hwndPause = GetDlgItem(hwnd, IDC_PAUSE);
		SetWindowText(hwndPause, TEXT("��ͣII")); //�޸İ�ť������
	}
	playControl(hwnd);
	srand((unsigned)time(NULL));//���������
	SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_SETCURSEL, numplayingsong, 0);
	numplayingsong = SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETCURSEL, 0, 0);
	totalMusicCount = SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETCOUNT, 0, 0);
	if (num == 0)/////////////////////////////��һ��//
	{
		if (RANDOM_LOOP == iLoopType)// �������
		{
			int n= numplayingsong;///////�����ظ�����
			while (n == numplayingsong)
			{
				numplayingsong = rand() % (totalMusicCount);
			}
		}
		else if (SINGLE_LOOP == iLoopType)//����ѭ��
		{
			numplayingsong = numplayingsong;
		}
		else if (CIRCULATION_LOOP == iLoopType)//�б�ѭ��
		{
			if (numplayingsong == 0)
			{
				numplayingsong = totalMusicCount - 1;
			}
			else if (numplayingsong < totalMusicCount - 1)
			{
				numplayingsong = numplayingsong - 1;
			}
		}
		else if (ORDER_LOOP == iLoopType)//˳�򲥷�
		{
			if (numplayingsong > 0)
			{
				numplayingsong = numplayingsong - 1;
			}
			else if (numplayingsong == 0)
			{
				playorstop = 1;
			}
		}
	}
	else if (num == 1)///////////////////////////////////////////////////////////��һ��
	{
		if (RANDOM_LOOP == iLoopType)// �������
		{
			int n = numplayingsong;///////�����ظ�����
			while (n == numplayingsong)
			{
				numplayingsong = rand() % (totalMusicCount);
			}
		}
		else if (SINGLE_LOOP == iLoopType)//����ѭ��
		{
			numplayingsong = numplayingsong;
		}
		else if (CIRCULATION_LOOP == iLoopType)//�б�ѭ��
		{
			if (numplayingsong < totalMusicCount - 1)
			{
				numplayingsong = numplayingsong + 1;
			}
			else if (numplayingsong == totalMusicCount - 1)
			{
				numplayingsong = 0;
			}
		}
		else if (ORDER_LOOP == iLoopType)//˳�򲥷�
		{
			if (numplayingsong < totalMusicCount - 1)
			{
				numplayingsong = numplayingsong + 1;
			}
			else if (numplayingsong == totalMusicCount - 1)
			{
				playorstop = 1;
			}
		}
	}
	SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_SETCURSEL, numplayingsong, 0);
	numplayingsong = SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETCURSEL, 0, 0);
	if(playorstop==0)
		PlayMusic(hwnd);
	else
	{
		StopMusic(hwnd);
	}
}


///////////////////////////////////////////////////
///����ģʽ
void playControl(HWND hwnd)
{
	HWND hwndcomb = GetDlgItem(hwnd, IDC_MODEL);
	iLoopType =  ComboBox_GetCurSel(hwndcomb);

}
///////////////////////////////////////
////�����б���Ϣ
void SaveList(HWND hwnd)
{
	int i;
	FILE *fp;// �򿪱�������ļ�
	if ((fp = fopen("D:/666list.list", "w")) == NULL)
	{
		MessageBox(hwnd, "���������б�ʧ�ܣ�", "��Ǹ", MB_OK | MB_ICONWARNING);
	}
	else
	{
		iVolume = getVolume(hwnd);
		playControl(hwnd);
		fprintf(fp, "%d,%d,%d\n", iLoopType, totalMusicCount, iVolume); // ����ģʽ����������������
		HWND hwndlist = GetDlgItem(hwnd, IDC_SONG);
		for (i = 0; i<totalMusicCount; i++)
		{
			TCHAR szFile[MAX_PATH];
			SendMessage(hwndlist, LB_GETTEXT, i, (LPARAM)szFile);//���ÿ�׸裬��������ļ�
			fwrite(szFile, sizeof(char), MAX_PATH, fp);////fprintf(fp, "%s\n", szFile); 
		}
	} 
	fclose(fp);
}
//////////////////////��ȡ������Ϣ
void ReadList(HWND hwnd)
{
	FILE *fp;
	if (NULL == (fp = fopen("D:/666list.list", "r")))
	{
		MessageBox(hwnd, "�����б��ļ�������", "����", MB_OK |MB_ICONWARNING);
	}
	else
	{
			fscanf(fp, "%d,%d,%d\n", &iLoopType, &totalMusicCount, &iVolume);// ��ȡģʽ����������������
			SendDlgItemMessage(hwnd, IDC_VOICE, TBM_SETPOS, true,  1000-iVolume);//����������	
			ComboBox_SetCurSel(GetDlgItem(hwnd,IDC_MODEL), iLoopType);
			for (int i = 0; i<totalMusicCount; i++)
			{
				fread(szFile, sizeof(char), MAX_PATH, fp);
				SendDlgItemMessage(hwnd, IDC_SONG, LB_INSERTSTRING, i, (LPARAM)szFile);
			}
			//PauseMusic(hwnd);
	}
}
//////////////////ɾ������
void DELETESONG(HWND hwnd)
{
	HWND hwndList = GetDlgItem(hwnd, IDC_SONG);
	int k = ListBox_GetCurSel(hwndList);
	ListBox_DeleteString(hwndList, k);
	if (k == numplayingsong)
	{
		StopMusic(hwnd);
		SetDlgItemText(hwnd, IDC_NAME, NULL); // ��ǰû�и�����ʾ
		KillTimer(hwnd, 1); // �ر���ʾ��ǰ����ʱ��Ķ�ʱ��
		KillTimer(hwnd, 2); // �رջ��������ƵĶ�ʱ��
		SetDlgItemText(hwnd, IDC_NOWTIME, NULL); // ��տ�ʼʱ�����ʾ
		SetDlgItemText(hwnd, IDC_TOTALTIME, NULL); // �����ʱ�����ʾ
		SendDlgItemMessage(hwnd, IDC_JINGDU, TBM_SETPOS, true, (LPARAM)NULL); // ���������ϻؿ�ʼλ��
		totalMusicCount --; // ��ԭ����¼���ļ�ʱ������������һ
	}
}
/////ɾ���б���Ϣ
void ClearListAll(HWND hwnd)
{
	HWND hwndList = GetDlgItem(hwnd, IDC_SONG);
	int k = ListBox_GetCount(hwndList);
	for (int i = k - 1; i >= 0; i--)
	{
		ListBox_DeleteString(hwndList, i);
	}
	StopMusic(hwnd);
	SetDlgItemText(hwnd, IDC_NAME, NULL); // ��ǰû�и�����ʾ
	KillTimer(hwnd, 1); // �ر���ʾ��ǰ����ʱ��Ķ�ʱ��
	KillTimer(hwnd, 2); // �رջ��������ƵĶ�ʱ��
	SetDlgItemText(hwnd, IDC_NOWTIME, NULL); // ��տ�ʼʱ�����ʾ
	SetDlgItemText(hwnd, IDC_TOTALTIME, NULL); // �����ʱ�����ʾ
	SendDlgItemMessage(hwnd, IDC_JINGDU, TBM_SETPOS, true, (LPARAM)NULL); // ���������ϻؿ�ʼλ��
	totalMusicCount = 0; // ��ԭ����¼���ļ�ʱ������������
	numplayingsong = 0; // ��ԭ���б��������
}


///////////////////////////////////
//����ģʽ
static int iMiniModeFlag = 1; // ����ģʽ�ı��//1->��׼//0->����
void SetMiniMode(HWND hwnd)
{
	HMENU hMenu = GetMenu(hwnd);
	//HWND hwndmodel = GetDlgItem(hwnd, ID_MINI);
	if (0 == iMiniModeFlag)
	{
		MessageBox(hwnd, "��ǰ��������ģʽ��", "��ʾ", MB_OK);
	}else
	{

		SetWindowPos(hwnd, 0, 122, 75, 473, 159, SWP_NOMOVE | SWP_NOZORDER);
		iMiniModeFlag = 0;
	}
}
void SetBIAOZHUNMode(HWND hwnd)
	{
		HMENU hMenu = GetMenu(hwnd);
		if (1 == iMiniModeFlag)
		{
			MessageBox(hwnd, "��ǰ���Ǳ�׼ģʽ��", "��ʾ", MB_OK );
		}
		else
		{
			SetWindowPos(hwnd, 0, 122, 75, 701, 423, SWP_NOMOVE | SWP_NOZORDER);
			iMiniModeFlag = 1;
		}
}


/////////////////////////////////////
//�������ܣ� ��С��������
void MinimizeToTray(HWND hwnd)
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = IDI_ICON1;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_MYMESSAGE; //�Զ�����Ϣ
	HINSTANCE hin = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE); //��ó�����
	nid.hIcon = LoadIcon(hin, MAKEINTRESOURCE(IDI_ICON1));
	lstrcpy(nid.szTip, TEXT("������"));
	Shell_NotifyIcon(NIM_ADD, &nid);
	ShowWindow(hwnd, SW_HIDE); //���ش���
}

/////////////////////////////
//�������ܣ� ���ϵͳ��С����Ϣ

void Main_OnSysCommand(HWND hwnd, UINT cmd, int x, int y)
{
	if (SC_MINIMIZE == cmd)
	{
		ShowWindow(hwnd, SW_HIDE);
		return;
	}
}