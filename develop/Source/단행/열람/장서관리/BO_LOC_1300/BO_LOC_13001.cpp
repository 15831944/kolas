// BO_LOC_13001.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_13001.h"
#include "..\\..\\..\\����\\BO_SPECIES_API\\BO_SPECIES.h"

#include "KolisNetService.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1300 dialog

CBO_LOC_1300::CBO_LOC_1300(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1300)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	parent = NULL;
}

CBO_LOC_1300::~CBO_LOC_1300()
{
}

VOID CBO_LOC_1300::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1300)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_1300, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1300)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1300 message handlers

BOOL CBO_LOC_1300::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_1300")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	CESL_Grid *pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_BO_LOC_1300") , _T("MainGrid") );
	if( pGrid_MainGrid_ALL == NULL ) return -1;

	pGrid_MainGrid_ALL->InitPopupMenuItem();
	pGrid_MainGrid_ALL->AddPopupMenuItem( (CView*)pMain , this->VIEW_STYLE , NULL );

	parent = (CESL_Mgr*)this;

	ChangeToolBar();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_1300::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	if(pGrid) pGrid->MoveWindow(0, 0, cx, cy);
	

	EFS_END
}

BOOL CBO_LOC_1300::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

INT CBO_LOC_1300::GetParentDMCount()
{
	EFS_BEGIN

	CString sParentDMAlias = _T("DM_BO_LOC_1300");
	CESL_DataMgr *pDM = FindDM( sParentDMAlias );
	if( pDM == NULL ) return -1;

	return pDM->GetRowCount();

	EFS_END
	return -1;

}

INT CBO_LOC_1300::DetailView()
{
	EFS_BEGIN

	
	CBL_BOSHELF SpfWork(this);
	SpfWork.SPFExcutePath( _T("�谡�ڷ����_�󼼺���") );

	return 0;

	EFS_END
	return -1;

}

BEGIN_EVENTSINK_MAP(CBO_LOC_1300, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_1300)
	ON_EVENT(CBO_LOC_1300, IDC_gridMAIN, -601 /* DblClick */, OnDblClickgridMAIN, VTS_NONE)
	ON_EVENT(CBO_LOC_1300, IDC_gridMAIN, -600 /* Click */, OnClickgridMAIN, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_1300::OnDblClickgridMAIN() 
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_1300");
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

VOID CBO_LOC_1300::OnClickgridMAIN() 
{
	EFS_BEGIN

	return ;

	EFS_END
}

VOID CBO_LOC_1300::OnTimer(UINT nIDEvent) 
{
	EFS_BEGIN

	CDialog::OnTimer(nIDEvent);

	EFS_END
}


INT CBO_LOC_1300::PrintList()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BO_LOC_1300") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BO_LOC_1300_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}

	const INT nCopyCnt=12;
	pPrintDM->FreeData();

	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("û����ȣ") , _T("û����ȣ") } , 
		{ _T("��ǥ��") , _T("����") } , 
		{ _T("������") , _T("������") } , 
		{ _T("������") , _T("������") } , 
		{ _T("BB_�����") , _T("�����") } , 
		{ _T("���ı���") , _T("���ı���") } ,
		{ _T("BB_��Ϲ�ȣ") , _T("��Ϲ�ȣ") } ,
		{ _T("�����ȣ") , _T("�����ȣ") } ,
		{ _T("BB_�谡����") , _T("�谡����") },
		{ _T("����") , _T("����") } ,
		{ _T("���»���") , _T("���»���") } ,
		{ _T("BB_�ڷ�Ǹ�") , _T("�ڷ��") }
	};

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_1300") , _T("MainGrid") );
	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();
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
				sEMsg.Format( _T("pSrcDM sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("pPrintDM sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}

		if( nMode == 1)
			pGrid->SelectGetNext();
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("�谡�ڷ���"));
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

INT CBO_LOC_1300::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}

BOOL CBO_LOC_1300::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_USER+1980) {

		INT i = 0;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

INT CBO_LOC_1300::ChangeToolBar()
{
	EFS_BEGIN
		
	return 0;
	
	EFS_END
		
	return -1;
	
}

INT CBO_LOC_1300::PrintList2()
{
		EFS_BEGIN

	INT ids;

	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BO_LOC_1300") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BO_LOC_1300_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}

	const INT nCopyCnt=14;
	pPrintDM->FreeData();

	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("û����ȣ") , _T("û����ȣ") } , 
		{ _T("��ǥ��") , _T("����") } , 
		{ _T("������") , _T("������") } , 
		{ _T("������") , _T("������") } , 
		{ _T("BB_�����") , _T("�����") } , 
		{ _T("���ı���") , _T("���ı���") } ,
		{ _T("BB_��Ϲ�ȣ") , _T("��Ϲ�ȣ") } ,
		{ _T("�����ȣ") , _T("�����ȣ") } ,
		{ _T("BB_�谡����") , _T("����") },
		{ _T("����") , _T("����") } ,
		{ _T("���»���") , _T("���»���") } ,
		{ _T("BB_�ڷ�Ǹ�") , _T("�ڷ��") },
		{ _T("BB_�谡��������") , _T("�谡������") },
		{ _T("BB_�谡����������") , _T("�谡������") }
	};

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_1300") , _T("MainGrid") );
	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();
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
				sEMsg.Format( _T("pSrcDM sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("pPrintDM sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}

		if( nMode == 1)
			pGrid->SelectGetNext();
	}

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("�谡�����ڷ���"));
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

VOID CBO_LOC_1300::Delete()
{
	CString sParentDMAlias = _T("DM_BO_LOC_1300");
	CESL_DataMgr *pDM = FindDM( sParentDMAlias );
	if( pDM == NULL ) return;

	CESL_Grid *pGrid = (CESL_Grid *)FindControl( _T("CM_BO_LOC_1300") , _T("MainGrid") );
	if( pGrid == NULL ) return;

	pGrid->SelectMakeList();
	
	INT nSelectedRowCount;
	nSelectedRowCount = pGrid->SelectGetCount();	
	if( nSelectedRowCount <= 0 )
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	if ( AfxMessageBox(_T("�������� ������ å �ϳ��� �����ϴ� ��� ���� �����˴ϴ�.\n���� ���õ� ��� ������ �����ǿ��� �����Ͻñ� �ٶ��ϴ�.\n\n������ å�� �����Ͻðڽ��ϱ�?"), MB_YESNO) != IDYES )
	{
		return;
	}	

	CString strSpeciesKey;
	CString strBookKey;
	CString strStatus;
	CString strRegNo;
	CString strQuery;
	CString strTmp;
	CString strMessage;
	INT nIndex;
	INT i;
	INT nLoanBookCnt, nDeleteCnt, nReservationCnt, nDupAddCnt;
	INT	ids;

	nLoanBookCnt = nDeleteCnt = nReservationCnt = nDupAddCnt = 0;
	pGrid->SelectGetTailPosition();
	
	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return;
	}

	CBlLocProgressBarDlg *ProgressBar;
	ProgressBar = new CBlLocProgressBarDlg( this );
	ProgressBar->Create( this );
	ProgressBar->CenterWindow( this );
	ProgressBar->ShowWindow( SW_SHOW );
	INT nUpper = nSelectedRowCount;
	if( nSelectedRowCount == 1 ) nUpper = 1;
	ProgressBar->InitRange(nUpper);

	for ( i=0 ; i<nSelectedRowCount ; i++ )
	{
		ProgressBar->ChangeProgressBar(i);
		nIndex = pGrid->SelectGetIdx();

		pDM->GetCellData(_T("BS_��KEY"), nIndex, strSpeciesKey );
		pDM->GetCellData(_T("BB_åKEY"), nIndex, strBookKey );
		pDM->GetCellData(_T("BB_��Ϲ�ȣ"), nIndex, strRegNo );

		strQuery.Format(_T("SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE REC_KEY = %s"), strBookKey);
		pDM->GetOneValueQuery(strQuery, strStatus);
		CLocCommonAPI::GetWorkingStatusDescToCode( strStatus, 1 );
		pDM->SetCellData(_T("BB_�谡����"), strStatus, nIndex );
		
		if ( strStatus.Find(_T("����")) >= 0 || 
			strStatus == _T("����å�η����������ڷ�") || 
			strStatus == _T("����ڷ�") ) 
		{
			nLoanBookCnt ++;
			pGrid->SelectGetPrev();
			continue;
		}
		else if ( strStatus == _T("��ġ�ڷ�") )
		{
			strQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3'"), strSpeciesKey);
			pDM->GetOneValueQuery(strQuery, strTmp);
			if ( _ttoi(strTmp.GetBuffer(0)) > 1 )
			{
				nReservationCnt++;
				pGrid->SelectGetPrev();
				continue;
			}
		}	

		CString strControlNo = _T(""); 
		pDM->GetCellData(_T("�����ȣ"), nIndex, strControlNo );
		CKolisNetService::setLocalBookRegNoInfo(parent,m_pInfo->LIB_CODE,strControlNo,strRegNo,_T(""),_T(""),_T(""),_T("D"),_T(""),_T(""));

		strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s"), strSpeciesKey);
		pDM->GetOneValueQuery(strQuery, strTmp);
		if ( _ttoi(strTmp.GetBuffer(0)) > 1 )
		{
			DeleteBook(pDM, strBookKey, strRegNo, nIndex);
									
			nDeleteCnt++;
		}
		else
		{
			strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE DUP_SPECIES_KEY = %s"), strSpeciesKey);
			pDM->GetOneValueQuery(strQuery, strTmp);
			if ( _ttoi(strTmp.GetBuffer(0)) > 0 )
			{
				nDupAddCnt++;
				pGrid->SelectGetPrev();
				continue;
			}

			DeleteAll(pDM, strSpeciesKey);
			nDeleteCnt++;
		}		
		pDM->DeleteRow(nIndex);
		pGrid->SelectGetPrev();
	}

	ProgressBar->DestroyWindow();
	if( ProgressBar != NULL ) delete ProgressBar;

	ids = pDM->SendFrame(TRUE);
	if (ids < 0) 
	{
		AfxMessageBox(_T("���� ����"));
		return;
	}
	
	ids = pDM->EndFrame();

	AllControlDisplay( _T("CM_BO_LOC_1300") , -1 );


	strMessage = _T("");

	if ( nDeleteCnt > 0 ) 
	{
		strTmp.Format(_T("�� %d ���� �����Ǿ����ϴ�.\n"), nDeleteCnt);
		strMessage += strTmp;
	}

	if ( nLoanBookCnt+nReservationCnt+nDupAddCnt > 0 ) 
	{
		strTmp.Format(_T("�� %d ���� ������ �� �����ϴ�.\n"), nLoanBookCnt+nReservationCnt+nDupAddCnt);
		strMessage += strTmp;
	}
	
	if ( nLoanBookCnt > 0 ) 
	{
		strTmp.Format(_T("   ������ %d ��\n"), nLoanBookCnt);
		strMessage += strTmp;
	}

	if ( nReservationCnt > 0 ) 
	{
		strTmp.Format(_T("   ������ %d ��\n"), nReservationCnt);
		strMessage += strTmp;
	}

	if ( nDupAddCnt > 0 ) 
	{
		strTmp.Format(_T("   �����߱� %d ��\n"), nDupAddCnt);
		strMessage += strTmp;	
	}
	
	AfxMessageBox(strMessage);
	
	pDM->EndFrame();
}

INT CBO_LOC_1300::DeleteBook(CESL_DataMgr *pDM, CString strBookKey, CString strRegNo, INT nIndex)
{
	CString strQuery;
	CString strStreamMarc;
	CString strSpeciesKey;
	CString strOption;
	INT ids;

	strQuery.Format(_T("DELETE FROM BO_BOOK_TBL WHERE REC_KEY = %s;"), strBookKey );	
	ids = pDM->AddFrame(strQuery);
	if ( ids < 0 ) return ids;

	ids = pDM->GetCellData(_T("BS_MARC"), nIndex, strStreamMarc );
	if ( ids < 0 ) return ids;

	ids = pDM->GetCellData(_T("BS_��KEY"), nIndex, strSpeciesKey );
	if ( ids < 0 ) return ids;	

	InsertToDropAccessionNoTbl(strRegNo, pDM, FALSE);

	CMarc marc;	
	m_pMarcMgr = m_pInfo->pMarcMgr;

	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return ids;	

	CESL_DataMgr *DM_Book = FindDM( _T("DM_����_����DM_å����") );
	strOption.Format(_T("b.SPECIES_KEY = %s"), strSpeciesKey);
	ids = DM_Book->RefreshDataManager(strOption);
	if (ids < 0) return ids;
	
	INT i, nRowCount;
	CString strTempBookKey;
	nRowCount = DM_Book->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		DM_Book->GetCellData(_T("BB_åKEY"), i, strTempBookKey);
		if ( strBookKey == strTempBookKey )
		{
			DM_Book->DeleteRow(i);
			break;
		}
	}

	CCatApi_BO *Convert = new CCatApi_BO(this);
	if( Convert == NULL )
	{
		AfxMessageBox(_T("CCatApi_BO is NULL!!"));
		return -100;
	}	
	
	ids = Convert->ConvertBookTo049(&marc, DM_Book);
	if ( ids < 0 ) return ids;
	strStreamMarc = _T("");
	ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	if ( ids < 0 ) return ids;	

	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("MARC") , _T("STRING") , strStreamMarc );	
	pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL") , _T("REC_KEY") , _T("NUMERIC") , strSpeciesKey );	

	if ( ids < 0 ) return ids;	

	return 0;
}

INT CBO_LOC_1300::DeleteAll(CESL_DataMgr *pDM, CString strSpeciesKey)
{
	if ( strSpeciesKey == _T("") ) return -1;
	
	CESL_DataMgr DM_Species;
	CESL_DataMgr DM_Book;
	CESL_DataMgr DM_Volume;
	CESL_DataMgr * DM_Toc = new CESL_DataMgr(); 

	DM_Toc->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	
	DM_Species.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	DM_Species.InitDMPKFromFile(_T("DM_����_����DM_������"));
	DM_Species.SetMarcMgr(m_pInfo->pMarcMgr);
	
	DM_Book.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	DM_Book.InitDMPKFromFile(_T("DM_����_����DM_å����"));
	DM_Book.SetMarcMgr(m_pInfo->pMarcMgr);

	DM_Volume.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	DM_Volume.InitDMPKFromFile(_T("DM_����_����DM_������"));
	DM_Volume.SetMarcMgr(m_pInfo->pMarcMgr);

	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = DM_Volume.RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)DM_Volume.RefList.GetNext(pos);
		if (pRef && pRef->FIELD_ALIAS == ( _T("BB_��������")))
		{
			pRef->DATA_TYPE = _T("");
			break;
		}
	}

	CString strCurDate = CTime::GetCurrentTime().Format(_T("%Y/%m/%d"));
	
	LPCTSTR lpszDeleteSpeciesTblName = _T("BO_DELETE_SPECIES_TBL");
	LPCTSTR lpszDeleteBookTblName = _T("BO_DELETE_BOOK_TBL");

	CArray <CString, CString> RemoveAliasList;

	
	INT nBookCount = -1;
	CString strTemp;
	CString strRegNo;
	CString strOption;
	CString strQuery;
	CString strRecKey;	
	CString strTocKey;
	INT		ids;

	CESL_DataMgr * pDM_EBook ; 
	pDM_EBook = new CESL_DataMgr;
	pDM_EBook->SetCONNECTION_INFO(CONNECTION_INFO);
	pDM_EBook->InitDMPKFromFile(_T("DMUP_����_��_EBOOK"));
	pDM_EBook->SetMarcMgr(m_pInfo->pMarcMgr);

	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = DM_Species.RefreshDataManager(strOption);
	if (ids < 0) return -100;
	
	if (DM_Species.GetRowCount() != 1) return -100;
	
	RemoveAliasList.RemoveAll();
	ids = DM_Species.SetCellData(_T("LAST_WORK"), GetWorkLogMsg(_T("�谡�ڷ��������"),WORK_LOG), 0);
	
	DM_Species.MakeQueryByDM(RemoveAliasList, &DM_Species, lpszDeleteSpeciesTblName, 0, &DM_Species);
	
	strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"), DM_Species.TBL_NAME, strSpeciesKey);
	ids = pDM->AddFrame(strQuery);
	if (ids < 0) return ids;
	
	strQuery.Format(_T("UPDATE IDX_BO_TBL SET SPECIES_CLASS = '3', DELETE_DATE = '%s' WHERE REC_KEY = %s;"), strCurDate, strSpeciesKey);
	ids = pDM->AddFrame(strQuery);
	if (ids < 0) return ids;
	
	ids = DM_Species.MakeRecKey(strRecKey);
	if (ids < 0) return ids;

	strQuery.Format(_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s"),strSpeciesKey);
	ids = DM_Toc->RestructDataManager(strQuery);
	if( ids < 0 ) return ids;
	
	INT nCnt = DM_Toc->GetRowCount();
	if( nCnt > 0 )
	{
		for( INT i = 0; i<nCnt ; i++ )
		{
			strTocKey = DM_Toc->GetCellData(i,0);
			strQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE REC_KEY = '%s';"),strTocKey);
			ids = pDM->AddFrame(strQuery);
			if (ids < 0) return ids;
			
			strQuery.Format(_T("DELETE FROM IDX_BO_TOC_TBL WHERE REC_KEY = '%s';"),strTocKey);
			ids = pDM->AddFrame(strQuery);
			if (ids < 0) return ids;
			
			strQuery.Format(_T("DELETE FROM BO_CONTENTS_TBL WHERE TOC_COMMON_KEY = '%s';"),strTocKey);
			ids = pDM->AddFrame(strQuery);
			if (ids < 0) return ids;
			
			strQuery.Format(_T("DELETE FROM BO_ABSTRACTS_TBL WHERE TOC_COMMON_KEY = '%s';"),strTocKey);
			ids = pDM->AddFrame(strQuery);
			if (ids < 0) return ids;
		}
	}
	
	strOption.Format(_T("b.SPECIES_KEY = %s"), strSpeciesKey);
	ids = DM_Book.RefreshDataManager(strOption);
	if (ids < 0) return ids;
	
	CString sBookKeyList;
	nBookCount = DM_Book.GetRowCount();
	for (INT idxBook = 0; idxBook < nBookCount; idxBook++)
	{
		strRegNo = DM_Book.GetCellData( _T("BB_��Ϲ�ȣ"), idxBook);
		if(!strRegNo.IsEmpty()) 
			InsertToDropAccessionNoTbl(strRegNo, &DM_Species, FALSE);
		
		strRecKey = DM_Book.GetCellData( _T("BB_åKEY"), idxBook);
		if (strRecKey.IsEmpty()) continue;
		
		sBookKeyList = sBookKeyList + _T(", ") + strRecKey;
	}

	strQuery.Format(_T("DELETE FROM %s WHERE b.SPECIES_KEY = %s;"), DM_Book.TBL_NAME, strSpeciesKey);
	ids = pDM->AddFrame(strQuery);
	if (ids < 0) return ids;
	
	CString strEbookQeury ;
	INT nRowCnt ;
	strEbookQeury.Format (_T("BOOK_KEY IN ( SELECT REC_KEY FROM BO_BOOK_TBL b WHERE %s)"),strOption ) ;
	ids = pDM_EBook->RefreshDataManager (strEbookQeury);
	nRowCnt = pDM_EBook->GetRowCount ();
	for(INT i = 0; i < nRowCnt ;i++)
	{
		pDM_EBook->SetCellData (_T("BB_DB_FLAG"),_T("D"),i) ;
	}
	CBO_SPECIES  pBO_SPECIES(this) ;
	CString strTmpData ;
	
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add( _T("BB_��Ϲ�ȣ"));
	RemoveAliasList.Add( _T("BB_������ȣ"));
	
	ids = ConvertBookToVolume(&DM_Book, &DM_Volume);
	if (ids < 0) return ids;
	
	nBookCount = DM_Volume.GetRowCount();
	for (idxBook = 0; idxBook < nBookCount; idxBook++)
	{
		ids = DM_Species.MakeRecKey(strRecKey);
		if (ids < 0) return ids;
		DM_Volume.SetCellData( _T("BB_åKEY"), strRecKey, idxBook);
		DM_Volume.SetCellData( _T("BB_��������"), strCurDate, idxBook);
		DM_Volume.SetCellData(_T("FIRST_WORK"), GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG) , idxBook);
		DM_Volume.SetCellData(_T("LAST_WORK"), GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG) , idxBook);
		
		DM_Species.MakeQueryByDM(RemoveAliasList, &DM_Volume, lpszDeleteBookTblName, idxBook, &DM_Species);
		 
		strTmpData = DM_Book.GetCellData (_T("BB_åKEY"),idxBook);
		pBO_SPECIES.SetEBookDMDBFlag (pDM_EBook,strTmpData,_T(""));
	}
	
	pBO_SPECIES.MakeEBookQueryFrame (pDM_EBook,pDM ) ;    

	CString sEcoSearch;
	if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
		sEcoSearch.Format( _T("ECO$P_DELETE_BO$I(%s);"), strSpeciesKey );
		ids = pDM->AddFrame(sEcoSearch);
		if ( 0 > ids ) return -1;
	}
	else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
		AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
		return -1;
	}		
	
	return 0;
}

CString CBO_LOC_1300::GetToday()
{
	EFS_BEGIN

	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;

	return _T("");

	EFS_END
	return _T("");
}


INT CBO_LOC_1300::InsertToDropAccessionNoTbl(CString strRegNo, 
										CESL_DataMgr * pDM_Out, 
										BOOL IsCheckInsert)
{
	EFS_BEGIN

	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	
	CString strCode = strRegNo.Left(nRegCodeLength);
	INT nNumber = _ttoi((strRegNo.Right(12-nRegCodeLength)).GetBuffer(0));
	
	CString query, strTmpData;
	if(IsCheckInsert)	
	{
		query.Format(_T("SELECT REC_KEY FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE = '%s' AND REG_NO = %d AND MANAGE_CODE = UDF_MANAGE_CODE"), strCode, nNumber);
		pDM_Out->GetOneValueQuery(query, strTmpData);
		if(strTmpData.GetLength() != 0) return 0;
	}
	CString strRecKey;
	INT ids = pDM_Out->MakeRecKey(strRecKey);
	if (ids < 0) return ids;
	query.Format(
		_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, REG_CODE, REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE) VALUES(%s, 'MO', '%s', %d, '%s', '%s', UDF_MANAGE_CODE);"),
			strRecKey,
			strCode,
			nNumber,
			GetWorkLogMsg(_T("�谡�ڷ����_����"),WORK_LOG),
			GetWorkLogMsg(_T("�谡�ڷ����_����"),WORK_LOG)
			);
	ids = pDM_Out->AddFrame(query);
	if (ids < 0) return ids;
	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_1300::ConvertBookToVolume(CESL_DataMgr *pDM_Book, 
									CESL_DataMgr *pDM_Volume)
{
	EFS_BEGIN

	if (!pDM_Book || !pDM_Volume) return -35001;

	pDM_Volume->FreeData();

	CArray <INT, INT> CopyRowIdxList;
	CopyRowIdxList.RemoveAll();

	INT nRowCount = pDM_Book->GetRowCount();
	INT i, j, ids;
	CString curVOL_SORT, tempVOL_SORT, strDBFLAG;
	BOOL IsNewVolume;
	for (i = 0; i < nRowCount; i++) {
		strDBFLAG = _T("");
		ids = pDM_Book->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("D")) continue;
		if (pDM_Book->GetCellData(_T("BB_å�η��÷���"), i) == _T("A")) continue;
		IsNewVolume = TRUE;
		ids = pDM_Book->GetCellData(_T("BB_�����"), i, curVOL_SORT);
		for (j = 0; j < i; j++) {
			if (pDM_Book->GetCellData(_T("BB_å�η��÷���"), j) == _T("A")) continue;
			ids = pDM_Book->GetCellData(_T("BB_�����"), j, tempVOL_SORT);
			if (curVOL_SORT == tempVOL_SORT) {
				IsNewVolume = FALSE;
				break;
			}
		}
		if (IsNewVolume == TRUE) CopyRowIdxList.Add(i);
	}

	INT nCopyRowIdxList = CopyRowIdxList.GetSize();
	INT RowIdx = 0;
	CString strItemData;
	for (i = 0; i < nCopyRowIdxList; i++) {
		pDM_Volume->InsertRow(-1);
		CopyDM(pDM_Book, CopyRowIdxList.GetAt(i), pDM_Volume, RowIdx);
		ids = pDM_Volume->GetCellData(_T("BB_�����"), RowIdx, curVOL_SORT);
		strItemData.Format(_T("%d"), GetBookCount(pDM_Book, _T("BB"), _T(""), curVOL_SORT));
		ids = pDM_Volume->SetCellData(_T("BB_å��"), strItemData, RowIdx);

		RowIdx++;
	}

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_1300::GetBookCount(CESL_DataMgr *pDM_Book, 
						  CString strToAliasHeader, 
						  CString strRegCode, 
						  LPCTSTR lpszVol)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	INT nReturnCnt = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, strToAliasHeader, lpszVol)) continue;

		if (!strRegCode.IsEmpty())
			if (pDM_Book->GetCellData( strToAliasHeader + _T("_��ϱ���_�ڵ�"), idx) != strRegCode) continue;
		
		nReturnCnt++;
	}

	return nReturnCnt;

	EFS_END
	return -1;
}

INT CBO_LOC_1300::CopyDM(CESL_DataMgr *pDM_From, 
					INT nFromIdx, 
					CESL_DataMgr *pDM_To, 
					INT nToIdx, 
					CString strToAliasHeader, 
					CStringArray *pArrIgnoreAlias)
{
	EFS_BEGIN

	if (pDM_From == NULL || pDM_To == NULL) return -1;

	INT ids = -1;
	INT nFind = -1;

	INT nRowCount = pDM_From->GetRowCount();
	if (nRowCount - 1 < nFromIdx || nFromIdx < 0) return -1;

	nRowCount = pDM_To->GetRowCount();
	if (nRowCount - 1 < nToIdx ) return -1;

	if (nToIdx < 0)
	{
		ids = pDM_To->InsertRow(-1);
		if (ids < 0) return -1;
		nToIdx = pDM_To->GetRowCount() - 1;
	}	

	CString strAlias;
	CString strItemData;

	CESL_DataMgr::reference *pRef = NULL;
	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if (pRef == NULL) continue;

		strAlias = pRef->FIELD_ALIAS;

		ids = pDM_From->GetCellData(strAlias, nFromIdx, strItemData);
		if (ids < 0) continue;

		if (!strToAliasHeader.IsEmpty())
		{
			nFind = strAlias.Find(_T("_"));
			if (nFind >= 0)
				strAlias = strAlias.Mid(nFind + 1);
			strAlias = strToAliasHeader + _T("_") + strAlias;
		}

		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		
		if ( strAlias == _T("IBS_�谡��ġ��ȣ") ) continue;
		if ( strAlias == _T("BS_�谡��ġ��ȣ") ) continue;
		if ( strAlias == _T("BB_��ϱ���_�ڵ�") ) continue;

		ids = pDM_To->SetCellData(strAlias, strItemData, nToIdx);
		if (ids < 0) continue;
	}

	return nToIdx;

	EFS_END
	return -1;
}

BOOL CBO_LOC_1300::IsBook(CESL_DataMgr *pDM_Book, 
					 INT idx, 
					 CString strToAliasHeader, 
					 LPCTSTR lpszVol)
{
	if (pDM_Book == NULL) return FALSE;

	if (pDM_Book->GetCellData( strToAliasHeader + _T("_å�η��÷���"), idx) == _T("A")) return FALSE;

	if (pDM_Book->GetCellData( strToAliasHeader + _T("_DB_FLAG"), idx) == _T("D")) return FALSE;

	if (lpszVol != NULL)
		if (pDM_Book->GetCellData( strToAliasHeader + _T("_�����"), idx) != lpszVol) return FALSE;

	return TRUE;
}

BOOL CBO_LOC_1300::IsStringInArray(CStringArray *pArr, 
							  CString str)
{
	EFS_BEGIN

	if (pArr == NULL || str.IsEmpty()) return FALSE;

	INT nRowCount = pArr->GetSize();
	if (nRowCount == 0) return FALSE;

	for (INT i = 0; i < nRowCount; i++)
	{
		if (pArr->GetAt(i) == str) return TRUE;
	}

	return FALSE;

	EFS_END
	return FALSE;
}

BOOL CBO_LOC_1300::IsStringInArray(CArray<CString, CString&> *pArr, 
							  CString str)
{
	EFS_BEGIN

	if (pArr == NULL || str.IsEmpty()) return FALSE;

	INT nRowCount = pArr->GetSize();
	if (nRowCount == 0) return FALSE;

	for (INT i = 0; i < nRowCount; i++)
	{
		if (pArr->GetAt(i) == str) return TRUE;
	}

	return FALSE;

	EFS_END
	return FALSE;
}

HBRUSH CBO_LOC_1300::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

INT CBO_LOC_1300::SetSelectGridFocus()
{
	INT nIdx;

	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_LOC_1300"));
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(_T("MainGrid")));

	pGrid->SelectMakeList();
	nIdx = pGrid->SelectGetHeadPosition();
	if( 0 < nIdx )
	{
		pGrid->SetReverse(nIdx);
	}

	return 0 ;

}
