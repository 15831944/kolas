// SE_LOC_13001.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_13001.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1300 dialog


CSE_LOC_1300::CSE_LOC_1300(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1300)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1300::~CSE_LOC_1300()
{
}

VOID CSE_LOC_1300::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1300)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSE_LOC_1300, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1300)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1300 message handlers

BOOL CSE_LOC_1300::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1300")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	// �˾� �޴� �ֱ�
	CESL_Grid *pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_SE_LOC_1300") , _T("MainGrid") );
	if( pGrid_MainGrid_ALL == NULL ) return -1;

	CArray <CString,CString> RemoveListArray;
	pGrid_MainGrid_ALL->InitPopupMenuItem();
	pGrid_MainGrid_ALL->AddPopupMenuItem( (CView*)pMain , this->VIEW_STYLE , &RemoveListArray );

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CSE_LOC_1300::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	if(pGrid) 
		pGrid->MoveWindow(0, 0, cx, cy);
	

	EFS_END
}

BOOL CSE_LOC_1300::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

INT CSE_LOC_1300::GetParentDMCount()
{
	EFS_BEGIN

	CString sParentDMAlias = _T("DM_SE_LOC_1300");
	CESL_DataMgr *pDM = FindDM( sParentDMAlias );
	if( pDM == NULL ) return -1;

	return pDM->GetRowCount();

	EFS_END
	return -1;

}

INT CSE_LOC_1300::DetailView()
{
	EFS_BEGIN

		
	CBL_SE_LOC_13 SpfWork(this);
	SpfWork.SPFExcutePath( _T("�谡�ڷ����_�󼼺���") );

	return 0;

	EFS_END
	return -1;

}

BEGIN_EVENTSINK_MAP(CSE_LOC_1300, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_LOC_1300)
	ON_EVENT(CSE_LOC_1300, IDC_gridMAIN, -601 /* DblClick */, OnDblClickgridMAIN, VTS_NONE)
	ON_EVENT(CSE_LOC_1300, IDC_gridMAIN, -600 /* Click */, OnClickgridMAIN, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_LOC_1300::OnDblClickgridMAIN() 
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_SE_LOC_1300");
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


	EFS_END
}

VOID CSE_LOC_1300::OnClickgridMAIN() 
{
	EFS_BEGIN

	return ;

	EFS_END
}

VOID CSE_LOC_1300::OnTimer(UINT nIDEvent) 
{
	EFS_BEGIN

	CDialog::OnTimer(nIDEvent);

	EFS_END
}


INT CSE_LOC_1300::PrintList()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_SE_LOC_1300") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_SE_LOC_1300_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}

	// pSrcDM -> pPrintDM���� ����
	const INT nCopyCnt=10;
	pPrintDM->FreeData();

	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("û����ȣ") , _T("û����ȣ") } , 
		{ _T("��ǥ��") , _T("����") } , 
		{ _T("������") , _T("������") } , 
		{ _T("������") , _T("������") } , 
		{ _T("BB_�����") , _T("�����") } , 
		{ _T("���»���") , _T("���»���") } ,
		{ _T("BB_��Ϲ�ȣ") , _T("��Ϲ�ȣ") } ,
		{ _T("�����ȣ") , _T("�����ȣ") } ,
		{ _T("BB_�谡����") , _T("�谡����") } ,
		{ _T("����") , _T("����") }
	};


	CString sData;
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1300") , _T("MainGrid") );
	pGrid->SelectMakeList();
	
	INT nSelectCnt = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();
	
	// 0 : ��� ��� , 1 : ������ �׸��常 ���
	INT nMode = 0;
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
	INT nIndex;
	
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();
		
		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("��ȣ") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}
		
		if( nMode == 1)
			pGrid->SelectGetNext();
	}
	
	// �Ϲ����� �ݺ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����_�谡�ڷ���"));
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

	EFS_END
	return -1;

}

INT CSE_LOC_1300::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1300::PrintList2()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_SE_LOC_1300") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_SE_LOC_1300_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}

	// pSrcDM -> pPrintDM���� ����
	const INT nCopyCnt=14;
	pPrintDM->FreeData();

	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("û����ȣ") , _T("û����ȣ") } , 
		{ _T("��ǥ��") , _T("����") } , 
		{ _T("������") , _T("������") } , 
		{ _T("������") , _T("������") } , 
		{ _T("BB_�����") , _T("�����") } , 
		{ _T("���»���") , _T("���»���") } ,
		{ _T("BB_��Ϲ�ȣ") , _T("��Ϲ�ȣ") } ,
		{ _T("�����ȣ") , _T("�����ȣ") } ,
		{ _T("BB_�谡����") , _T("�谡����") } ,
		{ _T("����") , _T("����") } ,
		{ _T("���»���") , _T("���»���") } ,
		{ _T("BB_�ڷ�Ǹ�") , _T("�ڷ��") } ,
		{ _T("BB_�谡��������") , _T("�谡������") } ,
		{ _T("BB_�谡����������") , _T("�谡������") }
	};


	CString sData;
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1300") , _T("MainGrid") );
	pGrid->SelectMakeList();
	
	INT nSelectCnt = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();
	
	// 0 : ��� ��� , 1 : ������ �׸��常 ���
	INT nMode = 0;
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
	INT nIndex;
	
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();
		
		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("��ȣ") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}
		
		if( nMode == 1)
			pGrid->SelectGetNext();
	}
	
	// �Ϲ����� �ݺ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����_�谡�����ڷ���"));
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

	EFS_END
	return -1;

}

HBRUSH CSE_LOC_1300::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
