// BO_LOC_38601.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_38601.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3860 dialog


CBO_LOC_3860::CBO_LOC_3860(CESL_Mgr* pParent)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3860)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3860::~CBO_LOC_3860()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CBO_LOC_3860::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3860)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3860, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3860)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3860 message handlers

BOOL CBO_LOC_3860::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
		
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	INT ids = InitESL_Mgr( _T("BO_LOC_3860") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	ids = MakeGroupInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	// ȭ����� �׸��� ����
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3860") , _T("���α׸���") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	// 1. �׸����� m_fpEventHandler ����
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	// 2010.09.07 ADD BY KSJ : ���յ������� ��뿩��
	GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_LOC_3860::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return;

	CWnd* pWnd = GetDlgItem(IDC_grid3860_Main);

	if( pWnd == NULL ) return ;

	if( pWnd->GetSafeHwnd() == NULL ) return;

	pWnd->MoveWindow( 0 , 0 , cx , cy );
		

EFS_END
}


BOOL CBO_LOC_3860::Create(CWnd* pParentWnd) 
{
EFS_BEGIN


	return CDialog::Create(IDD, pParentWnd);


EFS_END
return FALSE;

}



INT CBO_LOC_3860::MakeGroupInfo()
{
EFS_BEGIN

	
	INT ids;

	CESL_DataMgr* pGroupDM = FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if( pGroupDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeGroupInfo") );

	ids = pGroupDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeGroupInfo") ); 

	return 0;

EFS_END
return -1;

}



INT CBO_LOC_3860::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{	
EFS_BEGIN

	if( strDMFieldAlias.Compare( _T("�Ҽ�") ) == 0 )
	{
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("����Ҽ�����") , str , str );
	}
	else if( strDMFieldAlias.Compare( _T("����") ) == 0 )
	{
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("������������") , str , str );
	}
	else if( strDMFieldAlias.Compare( _T("�Է��ڷ��") ) == 0 )
	{
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�ڷ�Ǳ���") , str , str );
	}

	return 0;


EFS_END
return -1;

}
HBRUSH CBO_LOC_3860::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
