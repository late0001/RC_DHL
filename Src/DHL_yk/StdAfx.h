// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#undef  WINVER  
#define  WINVER  0X0500 

/*
Windows 95��98 and Windows NT   4.0     WINVER=0x0400         
Windows 98 and Windows 2000				WINVER=0x0500     
Windows 2000                            WINVER=0x0500 
Windows xp								WINVER=0x0501  
Windows 7								WINVER=0x0600  
  */

#if !defined(AFX_STDAFX_H__4B858A7B_A54D_460A_A51B_CD810BB0B803__INCLUDED_)
#define AFX_STDAFX_H__4B858A7B_A54D_460A_A51B_CD810BB0B803__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <afxtempl.h>
#include <winsock2.h>
#include <winioctl.h>
#define MFCXLIB_STATIC 
#include "XTToolkitPro.h"

#include "include\IOCPServer.h"
#include "macros.h"
#include "vfw.h" // DrawDibOpen
#pragma comment(lib, "vfw32.lib")
#include "SkinH.h"
//#pragma comment (lib,"SkinHu.lib")
#pragma comment ( lib, "SkinH.lib" )

enum 
{
	WM_CLIENT_CONNECT = WM_APP + 0x1001,
	WM_CLIENT_CLOSE,
	WM_CLIENT_NOTIFY,
	WM_DATA_IN_MSG,
	WM_DATA_OUT_MSG,

	
	WM_ADDTOLIST = WM_USER + 102,	// ��ӵ��б���ͼ��
	WM_ADDFINDGROUP,				// ����ʱ���ҷ���
	WM_REMOVEFROMLIST,				// ���б���ͼ��ɾ��
	WM_OPENMANAGERDIALOG,			// ��һ���ļ�������
	WM_OPENSCREENSPYDIALOG,			// ��һ����Ļ���Ӵ���
	WM_OPENWEBCAMDIALOG,			// ������ͷ���Ӵ���
	WM_OPENAUDIODIALOG,				// ��һ��������������
	WM_OPENKEYBOARDDIALOG,			// �򿪼��̼�¼����
	WM_OPENPSLISTDIALOG,			// �򿪽��̹�����
	WM_OPENSHELLDIALOG,				// ��shell����
	WM_RESETPORT,					// �ı�˿�
    WM_OPENREGEDITDIALOG,           // ��ע��������
	WM_OPENPSERLISTDIALOG,			// �򿪷��������
//	WM_FINDALL,						// ɸѡȫ������
//	WM_FIND,						// ɸѡ
	WM_OPENRUSERGDIALOG,            // �򿪷�����������
	//DOSS
	////////////////��������������//////////////////////////////////////
	WM_FREE_YES,					// ���˽��
	WM_FREE_NO,					// ��������
	//////////////////////////////////////////////////////////////
//DOSS
//////////////////////////////////////////////////////////////////////////
	FILEMANAGER_DLG = 1,
	SCREENSPY_DLG,
	WEBCAM_DLG,
	AUDIO_DLG,
	KEYBOARD_DLG,
	SYSTEM_DLG,
	SHELL_DLG,
	SERMANAGER_DLG,
    REGEDIT_DLG,
	USER_DLG
};

typedef struct
{	
	BYTE			bToken;			// = 1
	OSVERSIONINFOEX	OsVerInfoEx;	// �汾��Ϣ
	int				CPUClockMhz;	// CPU��Ƶ
	char			CPUNumber[20];	// CPU����
	IN_ADDR			IPAddress;		// �洢32λ��IPv4�ĵ�ַ���ݽṹ
	char			HostName[50];	// ������
	DWORD			MemSize;		// �ڴ��С
	DWORD			DriverSize;		// Ӳ������
	bool			bIsWebCam;		// �Ƿ�������ͷ
	DWORD			dwSpeed;		// ����
	char			Version[50];	// ����˰汾��Ϣ
	char			MarkTime[50];	// ����˰汾��Ϣ
	char			UpGroup[50];	// ���߷���
	char			RunTime[50];	// ����ʱ��
}LOGININFO;

typedef struct 
{
	DWORD	dwSizeHigh;
	DWORD	dwSizeLow;
}FILESIZE;

//�Զ�����Ϣ///////////////////////////////////////
#define WM_SYSTRAY						WM_USER+100
#define WM_SOCKET						WM_USER+101
#define WM_WORKTHREAD					WM_USER+102
///////////////////////////////////////////////////

#define MAKEINT64(low, high) ((unsigned __int64)(((DWORD)(low)) | ((unsigned __int64)((DWORD)(high))) << 32))

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4B858A7B_A54D_460A_A51B_CD810BB0B803__INCLUDED_)
