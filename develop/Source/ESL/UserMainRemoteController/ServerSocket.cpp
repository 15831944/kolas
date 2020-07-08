// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ServerMgr.h"
#include "ServerSocket.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{
	CAsyncSocket::CAsyncSocket();
	// KOL.UDF.022 ��ť���ڵ� ����
	m_pHwnd = NULL;
}

CServerSocket::~CServerSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions



/////////////////////////////////////////////////////////////////////////////
//*************************�޽����� ���� �� ������*************************//
void CServerSocket::SetWnd(HWND hwnd)
{
	m_pHwnd=hwnd;
}

////////////////////////////////////////////////////////////////////////////
//******************Ŭ���̾�Ʈ�� ���ӿ�û�� �޴� �Լ�*********************//
void CServerSocket::OnAccept(int nErrorCode) 
{
	//Ŭ���̾�Ʈ�� ���� ���� ��� ó�� �Լ� ȣ��
	::SendMessage(m_pHwnd,WM_ACCEPT_CLIENT,0,0);
}