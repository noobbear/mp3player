#include "stdafx.h"
#include "MainDlg.h"


BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
		HANDLE_MSG(hWnd, WM_SYSCOMMAND, Main_OnSysCommand);//�����С��
		HANDLE_MSG(hWnd, WM_HSCROLL, Main_HSCROLL); // ��Ӧ���ֻ��������϶�
		HANDLE_MSG(hWnd, WM_VSCROLL, Main_VSCROll); // ��Ӧ�������������϶�
		HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);//�ؼ��¼�
		HANDLE_MSG(hWnd, WM_CLOSE, Main_OnClose);//�ر�
		case WM_MYMESSAGE:
		{
			if (lParam == WM_LBUTTONDOWN|| lParam == WM_RBUTTONDOWN) //������ͼ������/�ҵ���
			{
				ShowWindow(hWnd, SW_SHOW);
			}
		}
	} 
	return FALSE;
}




int APIENTRY wWinMain(HINSTANCE hInstance,
	 HINSTANCE hPrevInstance,
	 LPWSTR    lpCmdLine,
	 int       nCmdShow)
{
	InitCommonControls();
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MP3), NULL, Main_Proc);
	return 0;
}

