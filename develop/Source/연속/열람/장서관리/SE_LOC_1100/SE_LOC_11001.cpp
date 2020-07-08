// SE_LOC_11001.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_11001.h"
#include "SE_LOC_1150.h"
#include "..\\..\\..\\..\\����\\�˻�\\SEBookDetailViewer\\SEBookDetailViewerDlg.h"
#include "..\\..\\..\\..\\����\\�˻�\\SEBookDetailViewer\\SEBookAppViewer.h"

#include "SE_LOC_1160.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1100 dialog


CSE_LOC_1100::CSE_LOC_1100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1100::~CSE_LOC_1100()
{
}

VOID CSE_LOC_1100::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1100, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1100)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1100 message handlers

BOOL CSE_LOC_1100::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSE_LOC_1100::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MAINGRID);
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	pGrid->MoveWindow(0, 0, cx, cy);
	
}

BOOL CSE_LOC_1100::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1100")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	// �˾� �޴� �ֱ�
	CESL_Grid *pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_SE_LOC_1100") , _T("MainGrid") );
	if( pGrid_MainGrid_ALL == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 ,_T("OnInitDialog") )
		
	CArray <CString,CString> RemoveListArray;
	pGrid_MainGrid_ALL->InitPopupMenuItem();
	pGrid_MainGrid_ALL->AddPopupMenuItem( (CView*)pMain , this->VIEW_STYLE , NULL );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_SE_LOC_1100") );
	if (pDM) pDM->SetMarcMgr(m_pInfo->pMarcMgr);
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	EFS_END
	
	return FALSE;
}

BEGIN_EVENTSINK_MAP(CSE_LOC_1100, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_LOC_1100)
	ON_EVENT(CSE_LOC_1100, IDC_MAINGRID, -601 /* DblClick */, OnDblClickMaingrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_LOC_1100::OnDblClickMaingrid() 
{
	// �� ���� �����ֱ�	
	CString sCMAlias = _T("CM_SE_LOC_1100");
	CString sGridAlias = _T("MainGrid");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnClickgridMAIN") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnClickgridMAIN") );
	
	if( pGrid->GetMouseRow() < 1 ) return;
	
	pGrid->SelectMakeList();
	if( pGrid->SelectGetCount() > 0 )
		pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );

	DetailView();
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-23 ~ 2005-12-23
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
   @Modify History : 2005-12-23
                     ���� ���� �߰� 
 	 
   [   �Լ�����   ] 
      [����] �谡���� - �ΰ� ��� ����

   [  PARAMETERS  ]
      NULL 

   [ RETURN VALUE ]
      0    : ���� 
      ���� : ����

   [   �̴Ͻ���   ]
      1. �ʱ�ȭ �۾�
	  2. ����Ʈ�� DM �ʱ�ȭ
	  3. �ΰ� ��� ��� ���̾�α�
	  4. SIReport ����
	  5. ��¿� DM�� �����
	  6. ���
	  7. ���� ���� ����� ���� �۾�
--------------------------------------------------------------------------------------------*/
INT CSE_LOC_1100::PrintList()
{
EFS_BEGIN

	INT ids;

	// ----------------------------------------------------------------------------
	// 1. �ʱ�ȭ �۾�
	// ----------------------------------------------------------------------------
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_SE_LOC_1100") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1100::PrintList") );

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1100") , _T("MainGrid") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSE_LOC_1100::PrintList") );
	pGrid->SelectMakeList();

	// 0 : ��ü , 1 : �׸��忡 ������ �ڷḸ
	INT nMode;
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		nSelectCnt = pSrcDM->GetRowCount();
		nMode = 0;
	}
	else
	{
		nMode =1;
	}

	// ----------------------------------------------------------------------------
	// 2. ����Ʈ�� DM �ʱ�ȭ
	// ----------------------------------------------------------------------------
	CESL_DataMgr PrintDM;
	CESL_DataMgr PrintDM2;
	CLocCommonAPI::MakeCopyDM( this , pSrcDM , &PrintDM );
	CLocCommonAPI::MakeCopyDM( this , pSrcDM , &PrintDM2 );
	
	if( nMode == 0 )
	{
		PrintDM.FreeData();
		CLocCommonAPI::AddDM( pSrcDM , &PrintDM , this );
	}
	else
	{
		INT nIndex;
		for( INT i = 0;i < nSelectCnt;i++ )
		{
			nIndex = pGrid->SelectGetIdx();
			CLocCommonAPI::AddDM( pSrcDM , &PrintDM , nIndex , -1 , this );
			pGrid->SelectGetNext();
		}
	}	

	if( PrintDM.GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}
	
	// ----------------------------------------------------------------------------
	// 3. �ΰ� ��� ��� ���̾�α�
	// ----------------------------------------------------------------------------
	CSE_LOC_1150 Dlg(this);
	Dlg.DoModal();

	CString sPassDate = Dlg.m_sPassDate;
	sPassDate.TrimLeft(); sPassDate.TrimRight();

	if( sPassDate.IsEmpty() ) 
	{
		AfxMessageBox( _T("�ΰ� ���ڰ� �����ϴ�.") );
		return 0;
	}

	// ----------------------------------------------------------------------------
	// 4. SIReport ����
	// ----------------------------------------------------------------------------
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return NULL;
	}
	pSIReportManager->SetCONNECTION_INFO(pSrcDM->CONNECTION_INFO);	
	pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;
	
	CSE_LOC_1160* pS = new CSE_LOC_1160( this );

	// 2005-12-23 ADD BY PDW(jr) =================================================	
	// ���� ��� ���ý� 
	CString sDefenceFlag = _T("");
	GetManageValue( _T("��Ÿ") , _T("����") , _T("�����") , _T("") , sDefenceFlag );
	if( Dlg.m_bGabPrintOpt )
	{
		CString sPrintAlias = _T("����_�ڷ�Ǻ�_�ΰ��ڷ�_����_���");
		if ( sDefenceFlag == _T("Y") ) sPrintAlias = _T("����_�ڷ�Ǻ�_�ΰ��ڷ�_����_���_�����");
		
		ids = pSIReportManager->GetSIReportFileFromDB( sPrintAlias );
		pS->Create( IDD_SE_LOC_1160, this );
		pS->ShowWindow( FALSE );
		pS->MakeStatistics();
	}
	// ===========================================================================
	else
	{
		if ( sDefenceFlag == _T("Y") )
			ids = pSIReportManager->GetSIReportFileFromDB(_T("����_�ڷ�Ǻ�_�ΰ��ڷ���_�����"));
		else
			ids = pSIReportManager->GetSIReportFileFromDB(_T("����_�ڷ�Ǻ�_�ΰ��ڷ���"));
	}

	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return NULL;
	}
	
	ids = pSIReportManager->DeleteTempPageFiles();	
	pSIReportManager->SetDataMgr(0, &PrintDM2, 0);

	// ----------------------------------------------------------------------------
	// 5. ��¿� DM�� �����.
	// ----------------------------------------------------------------------------
	BOOL bIsPrint;
	INT nPageCnt = 0;
	CString sData;
	CString sShelfLoc;
	CString sTmpShelfLoc;

	PrintDM.SORT( _T("�ڷ��") );

	for( INT i = 0;i < PrintDM.GetRowCount();i++ )
	{
		bIsPrint = FALSE;
		
		// ��¥ ��¹� DM�� INSERT�Ѵ�.
		CLocCommonAPI::AddDM( &PrintDM , &PrintDM2 , i , -1 );
		
		// DM�� ���������� �Դٸ� ���
		if( (i == PrintDM.GetRowCount()-1) )
			bIsPrint = TRUE;
		// ���� �ڷ�ǰ� ���� �ο��� �ڷ���� Ʋ���ٸ� ���
		else
		{
			ids = PrintDM.GetCellData( _T("�ڷ��") , i , sShelfLoc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSE_LOC_1100::PrintList") );
			
			ids = PrintDM.GetCellData( _T("�ڷ��") , i+1 , sTmpShelfLoc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CSE_LOC_1100::PrintList") );
			
			if( sShelfLoc.Compare(sTmpShelfLoc) != 0 )
				bIsPrint = TRUE;
		}
		
		// ����� �ƴ϶�� DM�� �ٽ� INSERT �۾�
		if( !bIsPrint ) continue;
		
		// ��¥ ��¹� DM�� ��Ϲ�ȣ�� �����Ѵ�.
		PrintDM2.SORT(_T("��Ϲ�ȣ"));

		// 2005-12-26 ADD BY PDW(Jr) ==============================================
		INT nSpeciesCnt = 0;			// �� ī��Ʈ ���� 
		CString sPreSKey;               // ���� ��Ű
		CString sCurSKey;               // ���� ��Ű 
		// ========================================================================
		CString sOrder;					// ��ȣ
		
		for( INT k = 0;k < PrintDM2.GetRowCount();k++ )
		{
			sOrder.Format( _T("%d") , k+1 );
			ids = PrintDM2.SetCellData( _T("��ȣ") , sOrder , k );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CSE_LOC_1100::PrintList") );	
			
			// 2005-12-26 ADD BY PDW(Jr) ==============================================
			// �� ī��Ʈ ������ ���Ѵ�.
			ids = PrintDM2.GetCellData( _T("������KEY") , k , sCurSKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_1100::PrintList()") );

			if ( sPreSKey != sCurSKey )
			{
				sPreSKey = sCurSKey;
				nSpeciesCnt++;
			}
			// ========================================================================
		}
		
		// ------------------------------------------------------------------------
		// 6. ���
		// ------------------------------------------------------------------------
		// ��� ��Ÿ ���� ����
		CString sDocCnt;
		sDocCnt.Format(_T(" %d"), k );
		
		// 2005-12-26 ADD BY PDW(Jr) ==============================================
		// ����� ��ī��Ʈ ����
		// �Ҿ����� SIReport�� �̷������� �����Ų��. �Ѥ� 
		if ( sDefenceFlag == _T("Y") )
			sDocCnt.Format(_T(" %d/%d"), nSpeciesCnt, k );
		// ========================================================================
		ids = pSIReportManager->SetTempVariable( _T("�ڷ��")   , sShelfLoc );
		ids = pSIReportManager->SetTempVariable( _T("�ΰ�����") , sPassDate );
		ids = pSIReportManager->SetTempVariable( _T("��å��")   , sDocCnt );

		if( Dlg.m_bGabPrintOpt )
		{
			pS->MakePrintPage( pSIReportManager, nPageCnt++ );
		}

		// ��ũ�� Manual �ݺ� ���
		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);	
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CSE_LOC_1100::PrintList") );				
		
		PrintDM2.FreeData();
		
	}
	
	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
		
	if ( pS )
	{
		delete pS;	
		pS = NULL;
	}
	
	return 0;

EFS_END
	return -1;
}

INT CSE_LOC_1100::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}


INT CSE_LOC_1100::DetailView()
{
	EFS_BEGIN
		
	CString sParentCMAlias = _T("CM_SE_LOC_1100");
	CString sParentDMAlias = _T("DM_SE_LOC_1100");
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("DetailView") );
	
	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("DetailView") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
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
			nStartPos = i;
		
		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}
	
	CSEBookDetailViewerDlg Dlg(this);
	
	Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , nStartPos );
	Dlg.DoModal();
	
	return 0;
	
	EFS_END
	return -1;
	
}

INT CSE_LOC_1100::AppView()
{
	EFS_BEGIN

	CString sParentCMAlias = _T("CM_SE_LOC_1100");
	CString sParentDMAlias = _T("DM_SE_LOC_1100");
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("AppView") );
	
	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("AppView") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("AppView") );
	
	CString sSpeciesKey;

	if( pParentDM->GetRowCount() < 1 )
		return 0;

	
	INT nIndex = pGrid->GetIdx();

	pParentDM->GetCellData( _T("������KEY") , nIndex , sSpeciesKey );

	CSEBookAppViewer dlg( this );
	dlg.m_bookKey = sSpeciesKey;
	dlg.DoModal();
	

	return 0;

	EFS_END

	return -1;
}


INT CSE_LOC_1100::SimplePrintList()
{
	EFS_BEGIN

	INT ids;
		
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_SE_LOC_1100") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1100") , _T("MainGrid") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	pGrid->SelectMakeList();
	
	// 0 : ��ü , 1 : �׸��忡 ������ �ڷḸ
	INT nMode;
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		nSelectCnt = pSrcDM->GetRowCount();
		nMode = 0;
	}
	else
	{
		nMode =1;
	}
	
	// �׸��忡 ���õ� �ڷ�� PrintDM�� �����Ѵ�.
	CESL_DataMgr PrintDM;
	CLocCommonAPI::MakeCopyDM( this , pSrcDM , &PrintDM );
	
	if( nMode == 0 )
	{
		PrintDM.FreeData();
		CLocCommonAPI::AddDM( pSrcDM , &PrintDM , this );
	}
	else
	{
		INT nIndex;
		for( INT i = 0 ; i < nSelectCnt ; i++ )
		{
			nIndex = pGrid->SelectGetIdx();
			CLocCommonAPI::AddDM( pSrcDM , &PrintDM , nIndex , -1 , this );
			pGrid->SelectGetNext();
		}
	}
	
	
	if( PrintDM.GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}

	CString sOrder;
	for( INT i = 0 ; i < PrintDM.GetRowCount() ; i++ )
	{
		sOrder.Format( _T("%d") , i+1 );
		PrintDM.SetCellData( _T("��ȣ") , sOrder , i  );
	}
	
	// �Ϲ����� �ݺ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(PrintDM.CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����_�谡����ڷ�"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 1;
	}
	
	pSIReportManager->SetDataMgr(0, &PrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;

	EFS_END
	
	return -1;

}
HBRUSH CSE_LOC_1100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
