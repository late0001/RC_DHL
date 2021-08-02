/*------------------------------------------------------------------------------
 [ģ������]
	CAudioCode

 [�ļ�����]
	AudioCode.h

 [����ļ�]
	AudioCode.cpp

 [Ŀ��]
	������Ƶ��ѹ�� 
     	  
 [����]		
	��װ g.729 api
	
 [������]	
	G729a.lib
                                               
------------------------------------------------------------------------------*/
#include <afxwin.h>
#ifndef _AUDIOCODE_H_
#define _AUDIOCODE_H_

// ������Ƶ����֡�Ĵ�С
#define SIZE_AUDIO_FRAME 960
// ������Ƶ���ݰ���С
#define SIZE_AUDIO_PACKED 60

class CAudioCode
{
public:
	virtual  ~CAudioCode();
	CAudioCode();
	// ������Ƶ����
	BOOL EncodeAudioData(char *pin,int len,char* pout,int* lenr);
	// ������Ƶ����
	BOOL DecodeAudioData(char *pin,int len,char* pout,int* lenr);
};


#endif