#if !defined(AFX_UPDATEIP_H__89D145DD_E4BB_4515_89F4_5508299872A1__INCLUDED_)
#define AFX_UPDATEIP_H__89D145DD_E4BB_4515_89F4_5508299872A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UPDATEIP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUPDATEIP form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CUPDATEIP : public CFormView
{
//protected:
//	CUPDATEIP();           // protected constructor used by dynamic creation
//	DECLARE_DYNCREATE(CUPDATEIP)

// Form Data
public:
	CUPDATEIP();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CUPDATEIP)

	//{{AFX_DATA(CUPDATEIP)
	enum { IDD = IDD_UPDATEIP };
	CComboBox	m_Combo_DnsIP;
	CComboBox	m_Combo_FtpIP;
//	CStaticLink	m_3322;
//	CString	m_FtpUrl;
//	CString	m_FtpUser;
//	UINT	m_FtpPort;
//	CString	m_FtpPass;
//	CString	m_FtpIP;
	CString	m_DnsUser;
	CString	m_DnsPass;
	CString	m_DnsDomain;
	CString	m_DnsUser1;
	CString	m_DnsPass1;
	CString	m_DnsDomain1;
//	CString	m_zifushuan;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	virtual CString Base64Encode(LPCTSTR lpszSrc);   //���ܺ���
	virtual CString Base64Decode(LPCTSTR lpszSrc);   //���ܺ���
	virtual CString GetIPAddress();    //����IP��ȡ
	virtual int ReplaceStr(char* sSrc, char* sMatchStr, char* sReplaceStr);
	virtual int memfind(const char *mem, const char *str, int sizem, int sizes);
	virtual char* substr(char   *str,int   istar,int   iend);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUPDATEIP)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:
//	virtual ~CUPDATEIP();

	CIniFile m_Ini;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

#endif

	// Generated message map functions
	//{{AFX_MSG(CUPDATEIP)
	afx_msg void OnBtnDnsupdate();
	afx_msg void OnEditchangeComboFtpip();
	afx_msg void OnBtnFtpupdate();
	afx_msg void OnB_IP();
	afx_msg void OnFTPServer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATEIP_H__89D145DD_E4BB_4515_89F4_5508299872A1__INCLUDED_)
