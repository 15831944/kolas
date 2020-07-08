// DonatorMgrMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DonatorMgrMainDlg.h"
#include "DonateDataView.h"
//=========================================
//20080617 REM BY PJW : �������� �������ؼ� ����� ����Ѵ�.
//#include "../BO_ACQ_PRINT_MGR/BasicPrintMgr.h"
//=========================================

//==============================================
//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
#include "SBL.H"
//==============================================

#include "../../../����/COMMON_DONATOR_MANAGER/DonateDataView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonatorMgrMainDlg dialog


CDonatorMgrMainDlg::CDonatorMgrMainDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonatorMgrMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorMgrMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
	
	//=========================================
	//20080617 REM BY PJW : �������� �������ؼ� ����� ����Ѵ�.
	m_pAddressDM = NULL;
	m_pPrintMgr = NULL;
	//=========================================
}

CDonatorMgrMainDlg::~CDonatorMgrMainDlg()
{

	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
}

VOID CDonatorMgrMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorMgrMainDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorMgrMainDlg, CDialog)
	//{{AFX_MSG_MAP(CDonatorMgrMainDlg)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(DONATOR_QUERY_CREATE, OnQueryCreate)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorMgrMainDlg message handlers

VOID CDonatorMgrMainDlg::OnClose() 
{
	DestroyWindow();
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �����ڰ��� �ʱ�ȭ
//
// [ �̴Ͻ��� ]
// 1. SM/CM/DM ����
// 2. �˻�â�� �����ϰ� OpenMode = 1000���� �����Ѵ�.
//		- CDonatorGetter_Search
// 3. �����׸��� �����Ѵ�.
// 4. �˾��޴��� �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CDonatorMgrMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//=======================================================================
	// 1. SM/CM/DM ����
	//=======================================================================
	if( InitESL_Mgr( _T("K2UPGRADE_�����ڰ���") ) < 0 )
	{
		ESLAfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_MANAGER_02") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_�����ڰ���"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if( m_pCM == NULL || m_pGrid == NULL )
	{
		ESLAfxMessageBox( _T("[CM_�����ڰ���]�� ������ �� �����ϴ�.") );
		return FALSE;
	}


	//JOB.2019.0017 ADD BY KYJ
	//�������� ���� ����,�α� ������ ����
	//-------------------------------------------------------------------------
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("������ ����");
	}
	//-------------------------------------------------------------------------
	
	m_pDM = FindDM(_T("DM_�����ڰ���"));
	if( m_pDM == NULL )
	{
		ESLAfxMessageBox( _T("[DM_�����ڰ���]�� ������ �� �����ϴ�.") );
		return FALSE;
	}
	//==================================================
	//20080617 ADD BY PJW : �ּҷ��̺� DM�߰�
	m_pAddressDM = FindDM(_T("DM_�ּҷ��̺�"));
	if( m_pAddressDM == NULL )
	{
		ESLAfxMessageBox( _T("[DM_�ּҷ��̺�]�� ������ �� �����ϴ�.") );
		return FALSE;
	}
	//==================================================
	
	
	//=======================================================================
	// 2. �˻�â�� �����ϰ� OpenMode = 1000���� �����Ѵ�.
	//		- CDonatorGetter_Search
	//=======================================================================
	m_pSearchDlg = new CDonatorNameGetter_Search(this);
	m_pSearchDlg->SetOpenMode(1000);


	//=======================================================================
	// 3. �����׸��� �����Ѵ�.
	//=======================================================================
	InitSortItem();
	
	//=========================================
	//20080617 REM BY PJW : �������� �������ؼ� ����� ����Ѵ�.
	m_pPrintMgr = new CBasicPrintMgr(this);
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	//���������� ����
	CLibInfoMgr mgr(this);
	mgr.GetLibInfo(GetUserID(), &m_cLibInfo);
	//=========================================

	//=======================================================================
	// 4. �˾��޴��� �����Ѵ�.
	//=======================================================================
	InitPopupMenuItem();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  
}

VOID CDonatorMgrMainDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridDONATOR_LIST);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(-2, 0, cx+2, cy);			
}

VOID CDonatorMgrMainDlg::ShowSearchDlg()
{
	if( m_pSearchDlg->GetCreate() == FALSE )
	{
		m_pSearchDlg->Create(this);
	}
	
	m_pSearchDlg->ShowWindow(SW_SHOW);
	m_pSearchDlg->UpdateWindow();
	m_pSearchDlg->CenterWindow();
}

VOID CDonatorMgrMainDlg::ShowPrintDlg()
{
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("����� ������ ����� �����ϴ�."));
		return;
	}
	
	//JOB.2019.0017 ADD BY KYJ
	//�������� ���� ����,�α� ����
	//--------------------------------------------------------------------------------------------------------------------------
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	//if( PrivacyPrintLogRecord(m_pDM, _T("�����ڸ�� ���"), _T("E") ,0 ) < 0 ) return;
	m_pPrintMgr->SetPrivacyPrintLogParm( TRUE, m_pInfo->USER_PK, m_pInfo->USER_ID, _T("�����ڸ�� ���"), m_pInfo->MANAGE_CODE, _T("�����ڸ�� ���"),	0 );
	//--------------------------------------------------------------------------------------------------------------------------
	

	//CBasicPrintMgr mgr(this);
	//mgr.SetPrintDM(m_pDM);
	//mgr.SetSIReportFilename(_T("K2UP_�����ڸ��"));
	//mgr.Print();
	m_pPrintMgr->SetPrintDM(m_pDM);
	m_pPrintMgr->SetSIReportFilename(_T("K2UP_�����ڸ��"));
	m_pPrintMgr->Print();
}

VOID CDonatorMgrMainDlg::ShowDonatorMgrDlg(OPERATION_TYPE nOpType)
{
	// 2005-02-17
	if(nOpType == SUPDATE)
	{
		INT ids = CheckSelectDataValid();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return ;
		}
	}

	//===================================================
	//2009.04.08 ADD BY PJW : �׸��� ������ �Ұ�� �������ڷᰡ ������� ����Ŀ���� ������ �߰��Ѵ�.
	m_pGrid->SelectMakeList();
//DEL	if(nOpType == GUPDATE)
//DEL	{
		if( m_pGrid->SelectGetCount() > 0 )
		{
			m_pGrid->SetAt( m_pGrid->GetIdx() , 0 , _T("V") );
			m_pGrid->SelectMakeList();
		}
//DEL	}	
	//===================================================
	
	INT nRowCnt = m_pDM->GetRowCount();
	CDonatorMgr dlg(this);
	dlg.SetOpenMode(nOpType);
	dlg.DoModal();
	if(nOpType == SINSERT)
	{
		INT nCurCnt = m_pDM->GetRowCount();
		if(nCurCnt > nRowCnt)
		{
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}
}

VOID CDonatorMgrMainDlg::DeleteDonatorMgrDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	if(IDNO == ESLAfxMessageBox(_T("������ ������ ������ ���� �Ͻðڽ��ϱ�?"), MB_YESNO)) return ;

	ids = m_pGrid->SelectMakeList();
	INT i = m_pGrid->SelectGetHeadPosition();
	m_pDM->StartFrame();
	CString strTmpData;
	while(i >= 0)
	{	
		strTmpData = m_pDM->GetCellData(_T("����������KEY"), i);
		m_pDM->MakeDeleteFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
		i = m_pGrid->SelectGetNext();
	}
	ids = m_pDM->SendFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ ������ ������ ������ �� �����ϴ�."));
		return ;
	}
	ids = m_pDM->EndFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ ������ ������ ������ �� �����ϴ�."));
		return ;
	}
	ESLAfxMessageBox(_T("������ ������ ������ �����Ͽ����ϴ�."));
	
	i = m_pGrid->SelectGetTailPosition();
	while(i >= 0)
	{
		m_pDM->DeleteRow(i);
		i = m_pGrid->SelectGetPrev();
	}
	DisplayGrid();
}

BOOL CDonatorMgrMainDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - CDonatorNameGetter_SearchŬ�������� ������� ������ �̿��Ͽ� DM�� �����ϰ� �׸��带 �׷��ش�.
//
// [ �̴Ͻ��� ]
// 1. Progress Ctrl�� �����Ѵ�.
// 2. �˻� Ŭ������ ����Ǿ� �ִ� �������� �����´�.
// 3. ������ �������� �̿��Ͽ� DM�� �����Ѵ�.
// 4. �����׸��� Ŭ������ �̿��Ͽ� DM�� �����Ѵ�.
// 5. ������ Progress Ctrl�� �Ҹ��Ѵ�.
// 6. �׸��带 �׸���.
//
/////////////////////////////////////////////////////////////////////////////
UINT CDonatorMgrMainDlg::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
	BEGIN_SBL();
	//==============================================

	//=======================================================================
	// ��������
	//=======================================================================
	INT ids;
	CString strQuery;

	//=======================================================================
	// 1. Progress Ctrl�� �����Ѵ�.
	//=======================================================================
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("�������ڷḦ �������Դϴ�."));


	//=======================================================================
	// 2. �˻� Ŭ������ ����Ǿ� �ִ� �������� �����´�.
	//=======================================================================
	strQuery = m_pSearchDlg->GetWhereStmt();

	//=======================================================================
	// 3. ������ �������� �̿��Ͽ� DM�� �����Ѵ�.
	//=======================================================================
	ids = m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("������������ �˻��� �� �����ϴ�."));
		return -1;
	}
	if( m_pDM->GetRowCount() == 0 )
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		DisplayGrid();
		return 0;
	}

	//=======================================================================
	// 4. �����׸��� Ŭ������ �̿��Ͽ� DM�� �����Ѵ�.
	//=======================================================================
	m_pSortItemSet->DataSort(m_pDM, !m_pSearchDlg->GetDataSortYN());

	//=======================================================================
	// 5. ������ Progress Ctrl�� �Ҹ��Ѵ�.
	//=======================================================================
	DeleteDataProcessingDlg();

	//=======================================================================
	// 6. �׸��带 �׸���.
	//=======================================================================
	
	//===================================================
	//2009.03.09 ADD BY PJW : ����å���� ����å���� ���� UDF_å���� �ִ´�.
	INT nDmRowCnt;
	INT nSumBookCnt;
	CString strSumBookCnt;
	nDmRowCnt = m_pDM->GetRowCount();
	for ( INT i = 0 ; i < nDmRowCnt ; i++ )
	{
		nSumBookCnt = _ttoi(m_pDM->GetCellData(_T("����å��"), i ));
		nSumBookCnt += _ttoi(m_pDM->GetCellData(_T("����å��"), i ));
		strSumBookCnt.Format( _T("%d"), nSumBookCnt );
		m_pDM->SetCellData(_T("UDF_å��"),strSumBookCnt, i );
	}
	//===================================================

	DisplayGrid();

	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
	INT nRowCount = m_pDM->GetRowCount();
	END_SBL(nRowCount);
	//==============================================
	
	return 0;
}

INT CDonatorMgrMainDlg::CheckSelectDataValid()
{
	if(m_pDM->GetRowCount() == 0) return -1;

	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if(!pGrid) return -1;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CDonatorMgrMainDlg::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

BEGIN_EVENTSINK_MAP(CDonatorMgrMainDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDonatorMgrMainDlg)
	ON_EVENT(CDonatorMgrMainDlg, IDC_gridDONATOR_LIST, -601 /* DblClick */, OnDblClickgridDONATORLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDonatorMgrMainDlg::OnDblClickgridDONATORLIST() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return ;

	if(m_pDM->GetRowCount() == 0) return ;	
	ShowDonatorMgrDlg(GUPDATE);
}

VOID CDonatorMgrMainDlg::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(this);

	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(this))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//���α׷��� ��Ʈ�� �ʱ�ȭ
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
//	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CDonatorMgrMainDlg::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

VOID CDonatorMgrMainDlg::DisplayGrid()
{
	INT nCurSel = m_pGrid->GetTopRow();
	BOOL bSetTopRow = TRUE;
	
	if( nCurSel < 0 )						
	{
		bSetTopRow = FALSE;
	}

	if( m_pDM->GetRowCount() == 0 )
	{
		bSetTopRow = FALSE;
	}
	
	if( nCurSel > m_pDM->GetRowCount() )
	{
		bSetTopRow = FALSE;
	}
	
	m_pGrid->Display();

	if( bSetTopRow == TRUE )	m_pGrid->SetTopRow(nCurSel);
}

VOID CDonatorMgrMainDlg::ShowDonateDataDlg()
{
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return ;
	}
	if(CheckSelectDataValid() < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CDonateDataView dlg(this);
	dlg.SetMainDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.DoModal();
}

VOID CDonatorMgrMainDlg::ShowSortDlg()
{
	if(m_pSortItemSet == NULL || m_pDM == NULL) 
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return;
	}
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(1);
	if(IDOK == dlg.DoModal())
		DisplayGrid();
}

BOOL CDonatorMgrMainDlg::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetSortItem(m_pSortItemSet);
	return TRUE;
}
HBRUSH CDonatorMgrMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


//=======================================================
//20080617 ADD BY PJW : ������ �μ�����¿� �ִ� ���� ����� �����ڰ��� �̵�
//1. �μ������
VOID CDonatorMgrMainDlg::DonateConfirmDataPrint()
{
	if(CheckSelectDataValid() < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	BOOL bMakePrint = FALSE;
	CDonateDataView view(this);
	view.ShowDonateDataSearchDlgByModal();
	
	CString strSpeciesCnt, strBookCnt, strMsg, strDonator, strDonateData;
	CESL_DataMgr * pBO_BookDM = FindDM(_T("DM_���������ڷ����"));
	CESL_DataMgr * pBO_PrintDM = FindDM(_T("DM_���������ڷ����_��"));
	CESL_DataMgr * pSE_DM = FindDM(_T("DM_����_�����ڷ�"));
	CESL_DataMgr * pPrintDM = FindDM(_T("DM_����_�����ڷ�"));
	
	if(pBO_BookDM == NULL || pBO_PrintDM == NULL || pSE_DM == NULL || pPrintDM == NULL) return ;

	m_pPrintMgr->InitRptFile();

	INT i = m_pGrid->SelectGetHeadPosition();
	INT ids;
	
	while(i >= 0)
	{
		view.MakeBO_DonateData(m_pDM, pBO_BookDM, pBO_PrintDM, i);
		ids = SetMarcInfo(pBO_PrintDM);
		if(ids < 0) return ;
		view.MakeSE_DonateData(m_pDM, pSE_DM, i);
		view.MakeMergeDonateData(pBO_PrintDM, pSE_DM, pPrintDM);

		strDonator = m_pDM->GetCellData(_T("������"), i);

		if(pPrintDM->GetRowCount() == 0) 
		{
			strMsg.Format(_T("%s���� �����ڷᰡ �����ϴ�."), strDonator);
			ESLAfxMessageBox(strMsg);
			i = m_pGrid->SelectGetNext();
			continue;
		}
		
		ids = view.SetPrintDonateConfirmData(m_pDM, pBO_BookDM, pBO_PrintDM, pSE_DM, pPrintDM, m_pPrintMgr, &m_cLibInfo, i);
		if(ids < 0) 
		{
			i = m_pGrid->SelectGetNext();
			continue;
		}
		m_pPrintMgr->MakeXMLPageFile();
		bMakePrint = TRUE;
		i = m_pGrid->SelectGetNext();
	}
	if (bMakePrint == TRUE)
		m_pPrintMgr->Print2();
	else 
		return;
}

//2. ���������
VOID CDonatorMgrMainDlg::DonatorDataPrint()
{
	if(CheckSelectDataValid() < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	m_pPrintMgr->SetPrintDM(m_pDM);
	m_pPrintMgr->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	m_pPrintMgr->SetSIReportFilename(_T("K2UP_�����ڰ�����"));
	m_pPrintMgr->InitRptFile();

	INT i = m_pGrid->SelectGetHeadPosition();
	while(i >= 0)
	{
		m_pPrintMgr->InitPrintData();
		m_pPrintMgr->AddPrintData(_T("������"), m_pDM->GetCellData(_T("������"), i));
		m_pPrintMgr->AddPrintData(_T("��������"), m_cLibInfo.GetLibName());
		m_pPrintMgr->MakeXMLPageFile();
		i = m_pGrid->SelectGetNext();
	}
	m_pPrintMgr->Print2();
}

//3. �ּҷ��̺����
VOID CDonatorMgrMainDlg::AddressLabelPrint()
{
	if(CheckSelectDataValid() < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	CString sLabelType;
	INT		ids;
	/// �ּ� ���̺� ���� ���� GET
	ids = GetManageValue( _T("����")  , _T("����") , _T("�ּҷ��̺�����") , _T("") , sLabelType );
	if( ids < 0 ) return;

	ids = MakeAddressDM();
	if(ids < 0) return;
	
	m_pPrintMgr->InitPrintData();
	m_pPrintMgr->SetPrintDM(m_pAddressDM);
	m_pPrintMgr->SetPrintType(BO_ACQ_DEFAULT);
	if ( sLabelType == '0' )
	{
		m_pPrintMgr->SetSIReportFilename(_T("K2UP_�������ּҷ��̺�"));
	}
	else
	{
		m_pPrintMgr->SetSIReportFilename(_T("K2UP_�������ּҷ��̺�_2X8"));
	}
	
	m_pPrintMgr->Print();
}

INT CDonatorMgrMainDlg::MakeAddressDM()
{
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = m_pGrid->SelectGetHeadPosition();
	BOOL bInsert = TRUE;
	CString ArrAddressAlias1[4] = {_T("�ּ�1"), _T("������1"), _T("�����ȣ1"), _T("�ּ�����1")};
	CString ArrAddressAlias2[4] = {_T("�ּ�2"), _T("������2"), _T("�����ȣ2"), _T("�ּ�����2")};
	m_pAddressDM->FreeData();

	while(i >= 0)
	{
		AddAddressDM(i, bInsert, ArrAddressAlias1, ArrAddressAlias2);
		i = m_pGrid->SelectGetNext();
		bInsert = !bInsert;
	}
	return 0;
}

INT CDonatorMgrMainDlg::AddAddressDM(INT nIdx, BOOL bInsert, CString * ArrAlias1, CString * ArrAlias2)
{
	INT nCurRow;
	CString ArrAddressInfo[4];

	if(bInsert) 
		m_pAddressDM->InsertRow(-1);

	nCurRow = m_pAddressDM->GetRowCount() - 1;

	ArrAddressInfo[0] = m_pDM->GetCellData(_T("�ּ�"), nIdx);
	ArrAddressInfo[1] = m_pDM->GetCellData(_T("������"), nIdx);
	ArrAddressInfo[2] = m_pDM->GetCellData(_T("�����ȣ"), nIdx);
	ArrAddressInfo[3] = m_pDM->GetCellData(_T("�ּ�"), nIdx) + _T("\r\n\r\n") + m_pDM->GetCellData(_T("������"), nIdx) + _T(" ����") + _T("\r\n\r\n") + m_pDM->GetCellData(_T("�����ȣ"), nIdx);
	if(bInsert) m_pAddressDM->SetCellData(ArrAlias1, 4, ArrAddressInfo, nCurRow);
	else m_pAddressDM->SetCellData(ArrAlias2, 4, ArrAddressInfo, nCurRow);
	return 0;
}

INT CDonatorMgrMainDlg::SetMarcInfo(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	if(pDM->GetRowCount() == 0) return 0;
	INT nRowCount = pDM->GetRowCount();

	CString strTmpData;
	CMarc marc;
	INT ids;
	for(INT i=0; i<nRowCount ; i++)
	{
		strTmpData = pDM->GetCellData(_T("BS_MARC"), i);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) pDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, i);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) pDM->SetCellData(_T("IBS_�ǻ���"), strTmpData, i);
		strTmpData.Empty();	 strTmpData = pDM->GetCellData(_T("IBS_��ǥ��"), i);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) pDM->SetCellData(_T("UDF_ǥ������"), strTmpData, i);
	}

	return 0;
}
//=======================================================
