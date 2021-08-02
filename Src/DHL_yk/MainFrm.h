// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__3F531B4D_21DF_49EA_B4AF_63AA5555104F__INCLUDED_)
#define AFX_MAINFRM_H__3F531B4D_21DF_49EA_B4AF_63AA5555104F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMainFrame : public CFrameWnd	//CXTPFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:


	BOOL m_bChecked;
	BOOL m_bOptions[6];

	CString m_strIniFileName;
	UINT m_nRibbonStyle;

	int m_nShape;
	int m_nStyle;
	int m_nColumns;

	int m_nStyleSet;
	int m_nThemeColors;
	int m_nThemeFonts;

	BOOL m_bAnimation;
	BOOL m_bLayoutRTL;
	BOOL m_bShowWorkspace;
	BOOL  IOCTrat;

	//CString m_csStylesPath;

	
	//int    m_nVisualTheme;
	//CString m_strStylesPath;

	CString ToolTipsTextsp;
// Operations
public:
	CMap<UINT,UINT, CWnd* , CWnd*> m_mapPanes;
	int m_nCount;
	virtual void OnTimerspop(BOOL Tstx,CString ToolTipsText);
	virtual void OnTimerschushi();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
//	BOOL CreateRibbonBar();
//	void LoadIcons();

	//int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	void ShowToolTips(LPCTSTR lpszText);
	void ShowConnectionsNumber();
	static void ProcessReceiveComplete(ClientContext *pContext);
	static void ProcessReceive(ClientContext *pContext);
	void Activate(UINT nPort, UINT nMaxConnections);
	static void CALLBACK NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif //_DEBUG

protected:  // control bar embedded members
	void SetTheme(int iTheme);
	int            m_iTheme;
	/*CXTPTabClientWnd m_wndClient;
	CXTTrayIcon m_TrayIcon;
	CXTPStatusBar  m_wndStatusBar;
	CXTPDockingPane*        m_pActivePane;
	CXTPDockingPaneManager  m_paneManager;
	CXTPDockingPane* CreatePane(int x, int y, CRuntimeClass* pNewViewClass, CString strFormat, XTPDockingPaneDirection direction, CXTPDockingPane* pNeighbour = NULL);
*/
	//CTabClientWnd m_wndClient;
	//CXTTrayIcon m_TrayIcon;
	CStatusBar  m_wndStatusBar;
	//CDockingPane*        m_pActivePane;
	//CDockingPaneManager  m_paneManager;
	//CDockingPane* CreatePane(int x, int y, CRuntimeClass* pNewViewClass, CString strFormat, XTPDockingPaneDirection direction, CXTPDockingPane* pNeighbour = NULL);



// Generated message map functions
protected:

    HICON m_hDrawIcon;
	HICON m_hEmptyIcon;
    int m_nCurrent;
    NOTIFYICONDATA m_NotifyIcon;

    BOOL  MinTray;  //������ʾ״̬����
	UINT  Zjshulian;

	//{{AFX_MSG(CMainFrame)
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnUpdateStatusBar(CCmdUI *pCmdUI);
	afx_msg void OnShow();
	afx_msg void OnExit();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSettings();
	afx_msg void OnBuild();
	afx_msg void OnAppExit();
	afx_msg void OnTools();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnSystray(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNewstips();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnViewStatusBar();
	afx_msg void OnAddSkin();
	afx_msg CString Onlinetips();
	afx_msg CString OnVoicepromptsm();
	afx_msg void OnVoiceprompt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCustomize();
//	afx_msg void OnOptionsStyle(UINT nStyle);
	afx_msg void OnUpdateOptionsStyle(CCmdUI* pCmdUI);
	void OnFrameTheme();
	void OnUpdateFrameTheme(CCmdUI* pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__3F531B4D_21DF_49EA_B4AF_63AA5555104F__INCLUDED_)
