// BO_LOC_14001.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_14001.h"
#include "..\\..\\..\\..\\����\\�˻�\\BOBookDetailViewer\\BOBookDetailViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1400 dialog


CBO_LOC_1400::CBO_LOC_1400(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1400)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_LOC_1400::~CBO_LOC_1400()
{

}

VOID CBO_LOC_1400::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1400)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1400, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1400)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1400 message handlers

BOOL CBO_LOC_1400::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_1400")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new CBL_BO_LOC_1400( this );

	CESL_Grid *pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_BO_LOC_1400") , _T("GRID") );
	if( pGrid_MainGrid_ALL == NULL ) return -1;
	
	pGrid_MainGrid_ALL->InitPopupMenuItem();
	pGrid_MainGrid_ALL->AddPopupMenuItem( (CView*)pMain , this->VIEW_STYLE , NULL );

	ChangeToolBar();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_1400::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	pGrid->MoveWindow(0, 0, cx, cy);
}

BOOL CBO_LOC_1400::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

INT CBO_LOC_1400::PrintList()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BO_LOC_1400") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BO_LOC_1400_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}

	const INT nCopyCnt=4;
	pPrintDM->FreeData();

	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("û����ȣ") , _T("û����ȣ") } , 
		{ _T("����") , _T("����") } , 
		{ _T("��Ϲ�ȣ") , _T("��Ϲ�ȣ") } , 
		{ _T("������ȣ") , _T("������ȣ") }
	};


	CESL_Grid *pGrid = (CESL_Grid *)FindControl( _T("CM_BO_LOC_1400") , _T("GRID") );
	if( pGrid == NULL ) return -1;
	pGrid->SelectMakeList();

	INT nSelectCnt;
	nSelectCnt = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();

	INT nMode = 0;
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();


	CString sData;
	INT nIndex;

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		ids = pSrcDM->GetCellData( _T("��������") , nIndex , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

		if( sData.Compare( _T("�Ƿ�") ) != 0 )
		{
			if( nMode == 1 )
				pGrid->SelectGetNext();
			
			continue;
		}

		pPrintDM->InsertRow(-1);

		INT nInsertIndex = pPrintDM->GetRowCount()-1;

		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex  , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

			ids = pPrintDM->SetCellData( sField[j][1] , sData , nInsertIndex );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , j , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}

		if( nMode == 1 )
			pGrid->SelectGetNext();
	}

	if( pPrintDM->GetRowCount() < 1 ) 
	{
		AfxMessageBox( _T("�˻��� �ڷ��� ���� �Ƿ� �ڷᰡ �������� �ʽ��ϴ�.") );
		return 1;
	}

	pPrintDM->SORT( _T("������ȣ") );

	CString sNumber;
	for( i = 0 ; i < pPrintDM->GetRowCount() ; i++ )
	{
		ids = pPrintDM->GetCellData( _T("������ȣ") , i , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

		sNumber.Format( _T("%s - %d") , sData , i+1 );

		ids = pPrintDM->SetCellData( _T("��ȣ") , sNumber , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		
	}
	
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("������������Ƿڵ������"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 1;
	}

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);

	CString sShelfLoc;
	ids = GetControlData( _T("CM_BO_LOC_1400") , _T("�ڷ��") , sShelfLoc , -1 , 1);
	pSIReportManager->SetTempVariable( _T("�Ҽ�") , sShelfLoc );

	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_1400::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}

BEGIN_EVENTSINK_MAP(CBO_LOC_1400, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_1400)
	ON_EVENT(CBO_LOC_1400, IDC_gridMAIN, -601 /* DblClick */, OnDblClickgridMAIN, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_1400::OnDblClickgridMAIN() 
{

	EFS_BEGIN
	
	CString sCMAlias = _T("CM_BO_LOC_1400");
	CString sGridAlias = _T("GRID");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnClickgridMAIN") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnClickgridMAIN") );
	
	if( pGrid->GetMouseRow() < 1 ) return;
	
	pGrid->SelectMakeList();
	if( pGrid->SelectGetCount() > 0 )
		pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );
	DetailView();
	
	EFS_END
	
}

INT CBO_LOC_1400::DetailView()
{
	EFS_BEGIN
		
	CString sParentCMAlias = _T("CM_BO_LOC_1400");
	CString sParentDMAlias = _T("DM_BO_LOC_1400");
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("DetailView") );
	
	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("DetailView") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("GRID") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("DetailView") );
	
	if( pParentDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("�ڷḦ �˻����ֽʽÿ�") );
		return 0;
	}

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;
	
	INT nMode = 0;
	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	INT nStartPos = 0;
	INT nTmpStartPos = pGrid->GetIdx();

	if( nSelectCnt > 1 )
	{
		nMode = 1;
		pGrid->SelectGetHeadPosition();
	}
	else
	{
		nSelectCnt = pParentDM->GetRowCount();
		nStartPos = pGrid->GetIdx();
	}
	
	INT nIndex;
	
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 1 )
			nIndex = pGrid->SelectGetIdx();
		else
			nIndex = i;
		
		pParentDM->GetCellData( _T("������KEY") , nIndex, sSpeciesKey );
		sSpeciesKeyList.AddTail( sSpeciesKey );
		asGridRow.Add( nIndex );
		
		if( nIndex == nTmpStartPos )
		{
			nStartPos = i;
		}

		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}
	
	CBOBookDetailViewerDlg Dlg(this);
	
	Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , nStartPos );
	Dlg.DoModal();
	
	return 0;
	
	EFS_END
	return -1;
	
}

INT CBO_LOC_1400::ChangeToolBar()
{
	EFS_BEGIN
	
	return 0;
	
	EFS_END
		
	return -1;
	
}
HBRUSH CBO_LOC_1400::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
