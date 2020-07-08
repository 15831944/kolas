// ServerMgr.cpp : implementation file
//

#include "stdafx.h"
#include "ServerMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerMgr

CServerMgr::CServerMgr()
{
	m_pServerSocket = NULL;
	// KOL.UDF.022 ��ť���ڵ� ����
	m_pClientSocket = NULL;
	m_ptrList.RemoveAll();
	m_pHwnd = NULL;
}

CServerMgr::~CServerMgr()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerMgr, CAsyncSocket)
	//{{AFX_MSG_MAP(CServerMgr)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerMgr member functions



/////////////////////////////////////////////////////////////////////////////
//�ڵ����� �Լ�
void CServerMgr::SetWnd(HWND hwnd)
{
	m_pHwnd	=	hwnd;
}


/////////////////////////////////////////////////////////////////////////////
//���� ������ ���� �Լ�
INT CServerMgr::CreateServer()
{
	m_pServerSocket = new CServerSocket;
	if( NULL == m_pServerSocket )
	{
		return	-100;
	}

	m_pServerSocket->SetWnd(m_pHwnd);
 	if( !m_pServerSocket->Create(5000,SOCK_STREAM) )
	{
		return	GetLastError();
	}

	if( !m_pServerSocket->Listen() )
	{
		return	GetLastError();
	}
	return	GetLastError();
}

////////////////////////////////////////////////////////////////////////////
//Ŭ���̾�Ʈ�� ���ӿ�û�� �޴� �Լ�
//Ŭ���̾�Ʈ�� ���� ��û�� �ް�, ��� �� �� �ִ� ���ο� ������ ����
//Ŭ���̾�Ʈ�� ������ ��������ش�
LONG CServerMgr::OnAcceptClient(UINT wParam, LONG lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	INT	nResult	= 1;

	m_pClientSocket	=	new CClientSocket;
	if( NULL	==	m_pServerSocket )
	{
		return	-200;
	}

	m_pClientSocket->SetWnd(m_pHwnd);

	if( !m_pServerSocket->Accept(*m_pClientSocket) )
	{
		return	GetLastError();
	}

	//������ Client�� Pointer ������, IP, Port�� ���� �ϴ� ���� ����
	CClientData*	clientData	=	new	CClientData;
	CString			strIP		=	"";
	UINT			nPort		=	0;

	if( NULL	==	clientData )
	{
		return	-201;
	}

	if( !m_pClientSocket->GetPeerName(strIP,nPort) )
	{
		return	GetLastError();
	}
	
	
	/////////////////////////////////////////////
	//���� IP ���� Check �Լ�

	//CheckDuplicateIP(strIP);

	
	//������ Client�� IP�� Port�� ����
	clientData->m_strIP			= strIP;
	clientData->m_pClientSocket	= m_pClientSocket;


	//���ӿ� �����ߴٴ� Message�� Client�� ������
	nResult	=	ProcessSend(m_pClientSocket,TYPE_CONNECT_SUCCESS);
	if( 1	!=	nResult )
	{
		return	nResult;
	}
	//Object�� ����
	m_ptrList.AddTail(clientData);
	
	return	1;
}


/////////////////////////////
//���� ����� ó�� �ϴ� �Լ�
LONG CServerMgr::OnCloseServer() 
{
	INT			nResult		=	CloseAllClient();
	if( 1	!=	nResult )
	{
		return nResult;
	}

	nResult		=	ProcessCloseServer();
	if( 1	!=	nResult )
	{
		return nResult;
	}
	/*
	for( int i = nClientCnt-1 ; i >= 0 ; i-- )
	{
		POSITION		pos = m_ptrList.FindIndex(i);
		if( NULL == pos )
		{
			nResult = -301;
			return	nResult;
		}

		CClientData*	pClientData;
		pClientData		= (CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pClientSocket = pClientData->m_pClientSocket;		

		nResult			= ProcessSend(pClientSocket,TYPE_CLOSE_SERVER);
		if( 1 != nResult )
		{
			return nResult;
		}

		ProcessCloseClient(pClientData,pos);
		if( GetClientCnt() != nClientCnt  )
		{
			nClientCnt--;
			i--;
		}
	}
	*/
	return 1;
}

//////////////////////////////////////////////
//Ŭ���̾�Ʈ���� �޽��� �޾��� ��� ó�� �Լ�
LONG CServerMgr::OnReceiveMsg(UINT wParam, LONG lParam) 
{
	INT nResult		= 1;
	INT nClientCnt	= GetClientCnt();
	if( nClientCnt < 1 )
	{
		return	-400;
	}

	CSocketData	Soketdata;

	for( int i = nClientCnt-1 ; i >= 0 ; i-- )
	{
		POSITION	pos	= m_ptrList.FindIndex(i);
		if( NULL	==	pos )
		{
			return	-401;
		}

		CClientData*	pClientData;
		pClientData		= (CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pSocket = pClientData->m_pClientSocket;
		if( pSocket	==	(CClientSocket*)wParam )
		{			
			do
			{
				if( !pSocket->Receive( &Soketdata, sizeof(Soketdata) ) )
				{
					return	GetLastError();
				}

				nResult		= ProcessData(&Soketdata, pClientData);
				if( 1	!=	nResult )
				{
					return	nResult;
				}
			}			
			while( pSocket->IsSerializable() );
		}		
		if(nClientCnt	!=	GetClientCnt())
		{
				nClientCnt--;
				i--;
		}
	}
	return	1;
}


////////////////////////////////////////
//Ŭ���̾�Ʈ ���� ���� �޽��� ó�� �Լ�
//-------------------------------------------------------------------------------//
INT CServerMgr::ProcessData(CSocketData *pSocketData, CClientData *pClientData)
{
	::SendMessage(m_pHwnd,WM_TYPE_INFO,(WPARAM)this,pSocketData->m_nType);

	INT nResult		= -501;
	INT nClientCnt	= GetClientCnt();
	if( nClientCnt < 1 )
	{
		nResult = -500;
	}

	CClientData		*pData;
	POSITION		pos;

	for( int i = nClientCnt-1 ; i >= 0 ; i-- )
	{
		pos		=	m_ptrList.FindIndex(i);	
		if( NULL	==	pos )
		{
			return	-502;
		}

		pData	=	(CClientData*)m_ptrList.GetAt(pos);
		if( pData	==	pClientData )
		{					
			break;				
		}
	}
	///////////////////////////////////////////
	//���� �޽����� Ÿ�Կ� ���� ó�� �ϴ� ����
	if(		 TYPE_CONNECT		==	pSocketData->m_nType )
	{	
		ProcessConnectClient(pSocketData,pClientData,pos);
		return	1;
	}

//********************************************************************************************************
	else if( TYPE_CONNECT_CHECK	==	pSocketData->m_nType )
	{
		::SendMessage(m_pHwnd,WM_RECEIVE_MSG
					,(WPARAM)this,TYPE_CONNECT_CHECK);
		return	1;
	}
	else if( TYPE_CLOSE_CLIENT	==	pSocketData->m_nType )
	{
		/*
		if( 1 == ProcessSend(pClientData->m_pClientSocket,TYPE_CLOSE_CLIENT) )
		{
			ProcessCloseClient(pClientData,pos);

			::SendMessage(m_pHwnd,WM_DELETE_CLOSE_CLIENT_LIST
						,(WPARAM)this,(LPARAM)(LPCTSTR)pClientData->m_strIP);
			return	1;
		}
		nResult = -503;
		return	nResult;
		*/
		nResult = ProcessSend(pClientData->m_pClientSocket,TYPE_CLOSE_CLIENT);
		if( 1 != nResult )
		{
			return	nResult;
		}
		ProcessCloseClient(pClientData,pos);

		::SendMessage(m_pHwnd,WM_DELETE_CLOSE_CLIENT_LIST
					,(WPARAM)this,(LPARAM)(LPCTSTR)pClientData->m_strIP);
	}
	else if( TYPE_CLOSE_SERVER	==	pSocketData->m_nType )
	{
		::SendMessage(m_pHwnd,WM_CLOSE_SERVER
					,(WPARAM)this,(LPARAM)(LPCTSTR)pClientData->m_strIP);
		return 1;
	}
	return	nResult;
}

//////////////////////////////
//Ŭ���̾�Ʈ�� ���� ���� ���
void CServerMgr::ProcessConnectClient(CSocketData *Socketdata, CClientData *pClient, POSITION pos)
{
	CString		strMsg;
	strMsg.Format("%s:%s",Socketdata->m_cName,pClient->m_strIP);

	::SendMessage(m_pHwnd,WM_PRINT_CLIENT_LIST
					,0,(LPARAM)(LPCTSTR)strMsg);
}

////////////////////////////
//Ŭ���̾�Ʈ ���� ó�� �Լ�
void CServerMgr::ProcessCloseClient(CClientData *pClientData,POSITION pos)
{
	if( NULL != pClientData )
	{			
		m_ptrList.RemoveAt( pos );
	}
}

/////////////////////
//���� ���� �Լ�
INT CServerMgr::ProcessCloseServer()
{
	if( NULL != m_pServerSocket )
	{
		m_pServerSocket->ShutDown();

		delete			m_pServerSocket;
		m_pServerSocket	= NULL;
	}
	else
	{
		return	-1001;
	}
	return	1;
}

///////////////////////////////////////
//Ŭ���̾�Ʈ ��ü�� �޽��� ������ �Լ�
//-------------------------------------------------------------------------------//
INT CServerMgr::SendMsg(INT nType)
{
	INT nResult		=	1;

	INT	nClientCnt = GetClientCnt();
	//�� Ŭ���̾�Ʈ�� �޽��� ����
	for( int i = nClientCnt-1 ; i >= 0 ; i-- )
	{
		POSITION	pos	=	m_ptrList.FindIndex(i);
		if( NULL	==	pos )
		{
			return		-701;
		}
		CClientData*	pClientData;
		pClientData		=	(CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pClientSocket	=	pClientData->m_pClientSocket;		

		nResult			=	ProcessSend(pClientSocket,nType);
		if( 1 != nResult )
		{
			return	nResult;
		}
	}
	return	1;
}

//////////////////////////////////
//Ŭ���̾�Ʈ�� �޽��� ������ �Լ�
INT CServerMgr::ProcessSend(CClientSocket *pSocket, INT nType)
{
	INT				nReturn		= -600;
	CSocketData*	pSocketData	= new CSocketData;
	if( NULL	==	pSocketData )
	{
		return		-601;
	}

	if( NULL	!=	pSocketData )
	{
		pSocketData->m_nType	=	nType;
	
		if( !pSocket->Send( pSocketData , sizeof(pSocketData) ) )
		{
			return	GetLastError();
		}
		delete	pSocketData;
		return	1;
	}
	return	nReturn;
}

/////////////////////////////////////////
//������ Ŭ���̾�Ʈ�� �޽��� ������ �Լ�
INT CServerMgr::SendToSelectClient(INT nType, CString strIp)
{
	// Ŭ���̾�Ʈ�� �޽��� ����
	INT nResult		= -801;

	INT nClientCnt	= GetClientCnt();
	for( int i = nClientCnt-1 ; i >= 0 ; i-- )
	{
		POSITION		pos = m_ptrList.FindIndex(i);
		if( NULL	==	pos )
		{
			return		-802;
		}
		CClientData*	pClientData;
		pClientData	=	(CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pSocket = pClientData->m_pClientSocket;


		//IP�� Client Check
		if( strIp	== pClientData->m_strIP )
		{
			nResult	=	ProcessSend(pSocket,nType);
			if( 1	!= nResult )
			{
				return	nResult;
			}
			break;
		}
		
		/*
		//Socket���� Client Check
		if( strIp	== pClientData->m_pClientSocket )
		{
			nResult	=	ProcessSend(pSocket,nType);
			if( 1	!= nResult )
			{
				return	nResult;
			}
			break;
		}
		*/
		if(nClientCnt != GetClientCnt())
		{
			nClientCnt--;
			i--;
		}
	}
	return	nResult;
}



//��� Ŭ���̾�Ʈ ����
INT CServerMgr::CloseAllClient()
{	
	INT nResult		= 1;

	INT nClientCnt	= GetClientCnt();
	//Ŭ���̾�Ʈ�� ���� ��ŭ �ݺ��� �Ͽ�
	//�� Ŭ���̾�Ʈ ����
	for( INT nIndex = nClientCnt-1 ; nIndex >= 0 ; nIndex-- )
	{
		POSITION pos	= m_ptrList.FindIndex(nIndex);
		if( NULL == nResult )
		{
			return		-902;
		}

		CClientData*	pClientData;
		pClientData		= (CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pClientSocket = pClientData->m_pClientSocket;		

		ProcessCloseClient(pClientData,pos);
		/*
		if( GetClientCnt() != nClientCnt  )
		{
			nClientCnt--;
			nIndex--;
		}
		*/
	}
	return	1;
}



BOOL CServerMgr::CloseSelectClient(INT nIndex)
{
	//Ŭ���̾�Ʈ ����
	POSITION pos	= m_ptrList.FindIndex(nIndex);
	if( NULL == pos )
	{
		return	FALSE;
	}

	CClientData*	pClientData;
	pClientData		= (CClientData*)m_ptrList.GetAt(pos);
	CClientSocket*	pClientSocket = pClientData->m_pClientSocket;		
	ProcessCloseClient(pClientData,pos);

	return	TRUE;
}


////////////////////////////////////////////////
//���� �Ǿ� �ִ� Ŭ���̾�Ʈ�� ������ ���ϴ� �Լ�
INT CServerMgr::GetClientCnt()
{
	return m_ptrList.GetCount();
}

void CClientData::Close()
{
	m_strIP		= "";
	m_strName	= "";
	if( m_pClientSocket )
	{
		m_pClientSocket->ShutDown(2);
		m_pClientSocket->Close();
		if(NULL != m_pClientSocket)
		{
			delete		m_pClientSocket;
		}
		m_pClientSocket	= NULL;
	}
}

INT CServerMgr::CheckDuplicateIP(CString strIP)
{
	INT nResult;

	INT nClientCnt	= GetClientCnt();	
	for( int i = 0 ; i < nClientCnt ; i++ )
	{
		POSITION	pos	=	m_ptrList.FindIndex(i);
		if( NULL	==	pos )
		{
			return	-1101;
		}

		CClientData*	pClientData;
		pClientData		=	(CClientData*)m_ptrList.GetAt(pos);

		CClientSocket*	pSocket	=	pClientData->m_pClientSocket;
		if( strIP	==	pClientData->m_strIP )
		{

			nResult		=	ProcessSend(m_pClientSocket,TYPE_CONNECT_FAILED);
			if( 1	!=	nResult )
			{
				return	nResult;
			}
			return	-1102;
		}		
	}
	return 1;
}
