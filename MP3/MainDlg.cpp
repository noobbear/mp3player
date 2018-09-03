#include "stdafx.h"
#include "MainDlg.h"

void OnIntialComBox(HWND hwnd)
{
	HWND hwndcomb = GetDlgItem(hwnd, IDC_MODEL);
	static int i = 0;
	if (i == 0)
	{
		//ComboBox_InsertString��ComboBox�ؼ������ĳ��ѡ��
		ComboBox_InsertString(hwndcomb, -1, TEXT("˳�򲥷�"));
		ComboBox_InsertString(hwndcomb, -1, TEXT("����ѭ��"));	
		ComboBox_InsertString(hwndcomb, -1, TEXT("�б�ѭ��"));
		ComboBox_InsertString(hwndcomb, -1, TEXT("�������"));
		ComboBox_SetCurSel(hwndcomb, iLoopType); // ����Ĭ����ʾ����
		i++;
	}
}

extern int iVolume;
TCHAR DEFAULTLISTFILE[MAX_PATH];//����Ĭ��·��
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	HICON hIcon =LoadIcon((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON1));
	 SendMessage(hwnd, WM_SETICON, TRUE,
		(LPARAM)hIcon);
	 SetVolumeTrack(hwnd);//����������
	 OnIntialComBox(hwnd);//ComBox�ؼ���ʼ��	
	 ReadList(hwnd); // ��ȡ�б��ļ�
	return TRUE;
}


void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	DWORD MESSAGE_INPUT = codeNotify;
	switch (id)
	{
	case IDC_ADD://���
	{
		addSong(hwnd);
		SaveList(hwnd);//�����б���Ϣ
		break;
	}
	case ID_ADD://���
	{
		addSong(hwnd);
		SaveList(hwnd);//�����б���Ϣ
		break;
	}
	case IDC_START://����
	{	
		PlayMusic(hwnd);
		break;
	}
	case IDC_PAUSE://��ͣ
	{
		PauseMusic(hwnd);
		break;
	}
	case IDC_STOP://ֹͣ
	{
		StopMusic(hwnd);
		break;
	}
	case IDC_LAST://��һ��
	{
		Jumpmusic(hwnd,0);
		break;
	}
	case IDC_NEXT://��һ��
	{
		Jumpmusic(hwnd,1);
		break;
	}
	case IDC_DELETE://ɾ��
	{
		DELETESONG(hwnd);
		SaveList(hwnd);
		break;
	}
	
	case IDC_QINGKONG://���
	{	int ret = MessageBox(hwnd, TEXT("�ף��Ƿ�����б���Ϣ"), TEXT("��ܰ��ʾ"), MB_YESNO);
		if (IDNO == ret)
		{
		}
		else if (IDYES == ret)
		{
			ClearListAll(hwnd);
			SaveList(hwnd);
		}	 	
		 break;
	}
	case IDC_SONG://�����б�
	{
		if (LBN_DBLCLK == codeNotify)//˫�������б����һ�׸���ʱ
		{
//			StopMusic(hwnd);
			getCurrentPoint(hwnd);//���ѡ�еĸ���
			PlayMusic(hwnd);
		}
		break;
	}
	case ID_SAVE:
	{
		SaveList(hwnd);//�����б���Ϣ
		MessageBox(hwnd, TEXT("�����б��ѱ��棡"), TEXT("��ܰ��ʾ"), MB_OK );	
		break;
	}
	case ID_LOAD:
	{
		ReadList(hwnd);//��ȡ�б���Ϣ
		MessageBox(hwnd, TEXT("�����벥���б�"), TEXT("��ܰ��ʾ"), MB_OK);
		break;
	}
	case ID_SET://����
	{
		MessageBox(hwnd, TEXT("  �ù���������\n  ��������ѯ����QQ1399284825"), TEXT("Are you kidding me��"), MB_OK|MB_ICONWARNING);	break;
	}
	case ID_SHUOMING: //˵��
	{
		MessageBox(hwnd, TEXT("��ӭ�Ķ���Ҫ˵����\n\n�ļ��������Ա�������б���Ϣ�����أ����ߴӱ��ض�ȡ���б��У�\n\n����ֶ���Ӹ���������ӱ��ظ������б��У����Զ����浽���أ�\n\n����ģʽ�е�����ģʽ���Ӽ�࣬Ĭ�ϱ�׼ģʽ��\n\n�������ӱ��ڴ���ڻ�������������;\n\n���ɾ������հ�ť�����򶼻��Զ������б��и�����ɾ���������ֶ����棻\n\n�����������ر�ʱ���Զ����������Ϣ�����ڴ�ʱ�Զ���ȡ����\n\t��������ɾ������ļ�"), TEXT("˵��"), MB_OK);	break;
	}
	case ID_ABOUT: //����
	{
		MessageBox(hwnd, TEXT("\t������� *�����* ��������Ȩ���У�\n\t ��л���ʹ�ã������汾����Ӹ�ʹ��ܡ�\n\t ����һ���൱�����Ĳ�������\n\t ��ӭ���������������\n\t лл\n\t\t2016/07"), TEXT("�������"), MB_OK);	break;
	}
	case ID_BAIDU: //ȥ�ٶ�
	{
		ShellExecute(hwnd, "open", "http://baidu.com/", NULL, NULL, SW_NORMAL);
		break;
	}
	case ID_WANGYI: //ȥ����������
	{
		ShellExecute(hwnd, "open", "http://music.163.com/", NULL, NULL, SW_NORMAL);
		break;
	}
	case ID_KUGOU: //ȥ�ṷ
	{
		ShellExecute(hwnd, "open", "http://www.kugou.com/", NULL, NULL, SW_NORMAL);
		break;
	}
	case ID_MINI://����
	{
		SetMiniMode(hwnd);
		break;
	}
	case ID_BIAOZHUN://��׼
	{
		SetBIAOZHUNMode(hwnd);
		break;
	}
	default:break;
	}
}

void Main_OnClose(HWND hwnd)
{	
	int ret = MessageBox(hwnd, TEXT("�ף����ȷ����С��������\n�����˳���Ӧ�ó���"), TEXT("��ܰ��ʾ"), MB_YESNOCANCEL | MB_ICONQUESTION);
	if(IDYES==ret)
	{ 
		MinimizeToTray(hwnd);
		ShowWindow(hwnd, SW_HIDE);
	}
	else if (IDNO == ret)
	{
		SaveList(hwnd);//���沥���б���Ϣ;
		EndDialog(hwnd, 0);
	}
}