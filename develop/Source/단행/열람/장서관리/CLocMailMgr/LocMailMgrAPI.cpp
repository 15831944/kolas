// LocMailMgrAPI.cpp : implementation file
//

#include "stdafx.h"
#include "LocMailMgrAPI.h"
#include "SelectMailModeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocMailMgrAPI dialog


CLocMailMgrAPI::CLocMailMgrAPI(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocMailMgrAPI)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLocMailInfo = NULL;
}

CLocMailMgrAPI::~CLocMailMgrAPI()
{
	if( m_pLocMailInfo != NULL ) delete m_pLocMailInfo;
	m_pLocMailInfo = NULL;	
}

VOID CLocMailMgrAPI::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocMailMgrAPI)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CLocMailMgrAPI, CDialog)
	//{{AFX_MSG_MAP(CLocMailMgrAPI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocMailMgrAPI message handlers

// ���� ���� �߰�
// ���� ���� 
// ������ �����ϴ� SQL��
// ������â �����ٰ��ΰ� �����ΰ�
INT CLocMailMgrAPI::MakeRecord( INT nMailClass , CString sWhere , bool bShowWindow )
{
	EFS_BEGIN

	INT ids;

	if( m_pLocMailInfo == NULL )
	{
		CreateMailInfo();
	}


	// ���Ͽ� ���õ� ������ ����
	ids = m_pLocMailInfo->MakeMailInfo( nMailClass , sWhere );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddRecord( INT nMailMode , CString sLoanKey )") );
	}

	if( 0 < ids ) 
	{
		return ids;
	}

	// ������ ���� �ڷᰡ ����.
	INT		nMailCnt	=	m_pLocMailInfo->GetMailCnt();
	if( 1 > nMailCnt ) 
	{
		// 2009.03.23 UPDATE BY KSJ : ���ܻ�Ȳ ó���� ���Ͽ� Return�� ����
		//DEL return 1;
		return 1010;
	}

	m_pLocMailInfo->SendMail(nMailClass,bShowWindow);

	return 0;

	EFS_END
	return -1;

}

/*
// ���� ���� �߰�
// ���� ���� 
// ������ �����ϴ� SQL��
// ������â �����ٰ��ΰ� �����ΰ�
INT CLocMailMgrAPI::MakeRecord( INT nMailClass , CESL_DataMgr *pDM , bool bShowWindow )
{
EFS_BEGIN

	INT ids;

	// ���Ͽ� ���õ� ������ ����
	ids = m_pLocMailInfo->MakeMailInfo( nMailClass , pDM , m_pSendMailMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddRecord( INT nMailMode , CString sLoanKey )") );

	if( m_pSendMailMgr->m_nRecordCount < 1 )
		return 1;

	SendMail(bShowWindow);

	return 0;

  EFS_END
return -1;

}
*/

INT CLocMailMgrAPI::CreateMailInfo()
{
	EFS_BEGIN

	if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
		return 0;

	m_pLocMailInfo = new CLocMailInfo(pParentMgr);
	m_pLocMailInfo->SetInitValue();
	return 0;

	EFS_END
	return -1;

}

INT CLocMailMgrAPI::IsUseBatchSendMail()
{
	EFS_BEGIN

	return m_pLocMailInfo->IsUseBatchSendMail();


	EFS_END
	return -1;

}

// �ϰ� ������ ���
// return 1 : �ϰ� ������ ����� ������� �ʴ´�.
//        2 : ������ ���� �ڷᰡ �������� �ʴ´�.
INT CLocMailMgrAPI::SendBatchMail( INT nMailClass )
{
	EFS_BEGIN

	INT ids;

	// ���� ���� ����
	if( m_pLocMailInfo == NULL )
	{
		CreateMailInfo();
	}

	// �� ������ ���Ǵ��� �˻�
	ids = m_pLocMailInfo->IsUseBatchSendMail( nMailClass );
	if( ids > 0 ) return 1;

	// ���Ͽ� ���õ� ������ ����
	ids = m_pLocMailInfo->BatchMakeMailInfo( nMailClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddRecord( INT nMailMode , CString sLoanKey )") );

	// ������ ���� �ڷᰡ ����.
	if( m_pLocMailInfo->GetMailCnt() < 1 ) return 2;

	m_pLocMailInfo->SendMail( nMailClass, FALSE );
	
	return 0;

	EFS_END
	return -1;

}

// ���� �̸� �������� �Լ�
CString CLocMailMgrAPI::GetMailName( INT nMailClass )
{
	EFS_BEGIN

	return m_pLocMailInfo->GetMailName( nMailClass );

	EFS_END
	return _T("");

}

// DM ���ڵ��� ������ ���� ������ �������θ� �˷��ִ� �Լ�
INT CLocMailMgrAPI::GetLogDMCnt( INT nMailClass , INT &nDMRCnt , INT &nSuccessDMRCnt , INT &nFailDMRCnt)
{
	EFS_BEGIN

	INT ids;

	ids = m_pLocMailInfo->GetLogDMCnt( nMailClass , nDMRCnt , nSuccessDMRCnt , nFailDMRCnt);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetLogDMCnt") );

	return 0;

	EFS_END
	return -1;

}

// ���� �α� ���
INT CLocMailMgrAPI::AddFailLog( INT nMailClass , CESL_DataMgr *pDstDM )
{
	EFS_BEGIN

	INT ids;

	ids = m_pLocMailInfo->AddFailLog( nMailClass , pDstDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddFailLog") );

	return 0;

	EFS_END
	return -1;

}

// ���� ������ ���� ���� ����� �����Ͽ� ������ ������.
INT CLocMailMgrAPI::MakeRecord( INT nMailClass , CESL_DataMgr *pDM , CESL_Grid *pGrid , bool bShowWindow )
{
	EFS_BEGIN

	INT ids;

	if( m_pLocMailInfo == NULL )
		CreateMailInfo();

	// 1. ���� ������ �����Ѵ�.
	if( nMailClass == -1 )
	{
		nMailClass = SelectMailClass();
		if( nMailClass == -1 ) return 1;	// ���� ������ �������� �ʰ� ����Ͽ���.
	}

	// 2. ���� ����� �����Ͽ� ������ ������.
	// ���Ͽ� ���õ� ������ ����
	ids = m_pLocMailInfo->MakeMailInfo( nMailClass , pDM , pGrid );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeRecord( INT nMailMode , CString sLoanKey )") );
	if( ids > 0 ) return 2;	// ����ڰ� ����Ͽ���.


	// ������ ���� �ڷᰡ ����.
	if( m_pLocMailInfo->GetMailCnt() < 1 ) return 3;

	// 4. ���� ������
	ids = m_pLocMailInfo->SendMail(nMailClass,bShowWindow);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeRecord") );
	if( ids > 0 ) return ids;

	
	// 5. DM�� �׸��� ����
	ids = m_pLocMailInfo->DMAndGridSetting( nMailClass , pDM , pGrid );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeRecord") );
	
	return 0;

	EFS_END
	return -1;

}


// ���� ������ ������ �Լ�
INT CLocMailMgrAPI::SelectMailClass()
{
	EFS_BEGIN

	CSelectMailModeDlg SelectDlg(this,m_pLocMailInfo);
	SelectDlg.DoModal();

	if( SelectDlg.m_nCloseMode == 0 ) return -1;

	return SelectDlg.GetMailClass();

	EFS_END
	return -1;

}


/// Where���� �������� �ڷ� ���� ������
INT CLocMailMgrAPI::MakeRecord( INT nMailClass , CStringArray &saWhere , bool bShowWindow )
{
	EFS_BEGIN
		
	INT ids;
	
	if( m_pLocMailInfo == NULL )
	{
		CreateMailInfo();
	}
	
	
	// ���Ͽ� ���õ� ������ ����
	ids = m_pLocMailInfo->MakeMailInfo( nMailClass , saWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddRecord( INT nMailMode , CString sLoanKey )") );
	
	// ������ ���� �ڷᰡ ����.
	if( m_pLocMailInfo->GetMailCnt() < 1 ) return 1;
	
	m_pLocMailInfo->SendMail(nMailClass,bShowWindow);
	
	return 0;
	
	EFS_END
	return -1;
	
}