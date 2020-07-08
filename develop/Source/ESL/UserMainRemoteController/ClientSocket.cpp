// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ServerMgr.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
	CAsyncSocket::CAsyncSocket();
}

CClientSocket::~CClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions


/////////////////////////////////////////////////////////////////////////////
//*************************�޽����� ���� �� ������*************************//
void CClientSocket::SetWnd(HWND hwnd)
{
	m_pHwnd=hwnd;
}

/////////////////////////////////////////////////////////////////////////////
//******************�޽����� �޾����� �˷��ִ� �Լ�************************//
void CClientSocket::OnReceive(int nErrorCode) 
{
	::SendMessage(m_pHwnd,WM_RECEIVE_MSG,(WPARAM)this,0);
}
