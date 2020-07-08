// BO_LOC_34001.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_34001.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3400 dialog

CBO_LOC_3400::CBO_LOC_3400(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3400)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBO_LOC_3400::~CBO_LOC_3400()
{
	m_pLoanShareManager->DestroyWindow( );
	if ( NULL!=m_pLoanShareManager )
	{
		delete m_pLoanShareManager;
		m_pLoanShareManager = NULL;
	}

	if ( NULL!=m_pLoanMangeValue )
	{
		delete m_pLoanMangeValue;
		m_pLoanMangeValue = NULL;
	}

	if ( NULL!=m_pSpfWork )
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}

	FreeMailMgr() ;
}

VOID CBO_LOC_3400::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3400)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3400, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3400)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3400 message handlers

BOOL CBO_LOC_3400::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here

	if(InitESL_Mgr(_T("BO_LOC_3400")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pLoanShareManager = new CLoanShareManager(this);
	m_pLoanShareManager->Create(this);

	m_pLoanMangeValue = new CLonaManageValue(this);
	m_pLoanMangeValue->pMain = pMain;
	m_pLoanMangeValue->pParentMgr = (CESL_Mgr*)this;
	m_pLoanMangeValue->SM_STYLE = 'T';
	m_pLoanMangeValue->m_pInfo = this->m_pInfo;

	m_pLoanMangeValue->DoModal();

	m_pSpfWork = new CBL_BO_LOC_34(this,m_pLoanMangeValue);

	const INT gridCnt = 1;
	CString gridAlias[ gridCnt ] = { _T("grid1") };
	CArray< CString, CString >removeListArray;
	CESL_Grid *pGrid = NULL;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_BO_LOC_3400"), gridAlias[ i ] );
		if( !pGrid )
			return -1;
		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );

		if(!m_pInfo->m_bUserInfoExportYN)
		{
			pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		}
		else
		{
			pGrid->m_bPrivacyPrintLogRecord = TRUE;
			pGrid->m_strPrintInfo = _T("����ڷ� ����");
		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_3400::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid * pGrid1 = (CMSHFlexGrid*)GetDlgItem(IDC_gridDMAIN1);
	
	if(pGrid1->GetSafeHwnd() == NULL) {
		return;
	}

	pGrid1->MoveWindow(0, 0, cx, cy);
}

BOOL CBO_LOC_3400::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}


INT CBO_LOC_3400::PrintList()
{
	INT ids;
	
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BO_LOC_3400") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("PrintList") );
	
	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BO_LOC_3400_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("PrintList") );
	
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}

	if(PrivacyPrintLogRecord(pSrcDM, _T("�̿��� ���� ���"), _T("E") ,0 ) < 0 ) return 1;
	
	pPrintDM->FreeData();

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3400") , _T("grid1") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( 0 < nSelectCnt )
	{		
		INT nIndex = -1;

		for( INT j=0; j<nSelectCnt; j++ )
		{
			nIndex = pGrid->SelectGetIdx();

			ids = CLocCommonAPI::AddDM( pSrcDM , pPrintDM , nIndex, -1, this);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("PrintList") ); 

			pGrid->SelectGetNext();
		}
	}
	else
	{
		ids = CLocCommonAPI::AddDM( pSrcDM , pPrintDM , this);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("PrintList") ); 
	}

	CString sIndex , strValue , strUserNo;
	ids = GetManageValue( _T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), strValue );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("PrintList") );

	strValue.TrimLeft();	strValue.TrimRight();
	strValue.MakeUpper();

	for( INT i = 0; i < pPrintDM->GetRowCount() ; i++ )
	{
		sIndex.Format( _T("%d") , i+1 );
		pPrintDM->SetCellData( _T("��ȣ") , sIndex , i );
		
		if( _T("Y") != strValue )
		{
			ids = pPrintDM->GetCellData( _T("�����ڹ�ȣ") , i , strUserNo );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("PrintList") );

			CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_pLoanMangeValue->m_sIsUnityLoanService );
			ids = pPrintDM->SetCellData( _T("�����ڹ�ȣ"), strUserNo , i );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("PrintList") );
			
		}
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����ڷ���"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;
}



INT CBO_LOC_3400::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;
	
	EFS_END
	return -1;
	
}

INT CBO_LOC_3400::OpenMail()
{
EFS_BEGIN

EFS_END
	return -1;
}

CString CBO_LOC_3400::MakeBodyContents(CString strLibCode, CString strUserName, CString strBookTitle, CString strShelfCode, CString date)
{

	CString strBody = _T("") ;
	CString strTemp = _T("") ;

	
	strTemp = _T("�ȳ��ϼ���.") +strLibCode +_T("�Դϴ�. <br><br>") ;
	strBody += strTemp ;

	strTemp = strUserName + _T(" ���� ��û�Ͻ� ������ ���� �ڷᰡ ��� �Ǿ����ϴ�. <br><br>") ;
	strBody += strTemp ;

	strTemp = _T(" ���� : ") + strBookTitle + _T("<br> û����ȣ : ") +strShelfCode+ _T("<br>") ;
	strBody += strTemp ;

	strTemp = _T("������� : ")+ date + _T("<br><br>") ;
	strBody += strTemp ;

	
    strBody += _T("�ȳ��� ��ʽÿ�") ;



	return strBody ;
}

VOID CBO_LOC_3400::FreeMailMgr()
{

	if(m_pMailRecord)
    {
		delete m_pMailRecord;
		m_pMailRecord = NULL ;
	}


	if(m_pSendMailMgr)
	{
		delete m_pSendMailMgr ;
		m_pSendMailMgr = NULL ;
	}



}

HBRUSH CBO_LOC_3400::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

LRESULT CBO_LOC_3400::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CBO_LOC_3400::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN
	if (pMsg->message == WM_USER+1907)
	{

		CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3400"), _T("grid1"));
		ASSERT(pGrid);
		
		if (pGrid->GetSafeHwnd() == pMsg->hwnd && m_pInfo->m_bUserInfoExportYN == FALSE)
		{
			AfxMessageBox(_T("�׸��� �������� ����� ����� ������ �����ϴ�.\r\n�ش� ������ �����ڸ� ���ؼ��� �ο����� �� �ֽ��ϴ�."), MB_ICONWARNING);
			return TRUE;
		}
	}
	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END

	return FALSE;
}