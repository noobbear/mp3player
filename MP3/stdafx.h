// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once


#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include <commdlg.h>//�ؼ�ͷ�ļ�
#include <Shlobj.h> // ���� ����ļ���//FileInit��������
#include <mmsystem.h> // ���� mciSendString ����
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h> 

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include "shellapi.h" //���� NOTIFYICONDATA
#include "resource.h"
#include "COMMCTRL.h"
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"winmm.lib") //���ֿ��ļ�

//��ǰ״̬
//TCHAR status[MAX_PATH];
//int pMethod; //�Զ����ŷ�ʽ

#define WM_MYMESSAGE WM_USER+100 // �Զ�����Ϣ,��Ӧ��С�����̵�
// ѭ��ģʽ�ĺ궨��
#define ORDER_LOOP 0 // ˳�򲥷�
#define SINGLE_LOOP 1 // ����ѭ��
#define CIRCULATION_LOOP 2 // �б�ѭ��
#define RANDOM_LOOP 3 // �������
static int iLoopType=0 ;// ѭ��ģʽ����ʼΪ˳�򲥷�



