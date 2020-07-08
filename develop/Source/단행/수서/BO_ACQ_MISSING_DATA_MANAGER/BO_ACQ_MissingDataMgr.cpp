// BO_ACQ_MissingDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_MissingDataMgr.h"
#include "../BO_ACQ_STATISTICS/BO_Purchase_Statistics.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"
#include "../../../����/BO_ACQ_COMMON/PriceSumDlg.h"
#include "../../../����/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_MissingDataMgr dialog


CBO_ACQ_MissingDataMgr::CBO_ACQ_MissingDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_MissingDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_MissingDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nRow		= -1;
	m_nEditCol	= -1;
	m_bCellEditing	= TRUE;	
	m_strDivNo				= _T("");	
	///������ ��ǰå�� ���
	m_sDeliveredCountBackup = _T("");
	m_strOrderPrice_Unit	= _T("0");
	m_strPosNumber			= _T("");
	m_strIs5163				= _T("");
	m_strIs64				= _T("");

	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDM				 = NULL;
	m_pPrintDM = NULL;
	m_pPrintObjDM		 = NULL;
	m_pSearchDlg = NULL;
	m_pResultSearchDlg = NULL;
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
}
CBO_ACQ_MissingDataMgr::~CBO_ACQ_MissingDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(m_pResultSearchDlg)
	{
		delete m_pResultSearchDlg;
		m_pResultSearchDlg = NULL;
	}	
	if(m_pPrintDlg)
	{
		delete m_pPrintDlg;
		m_pPrintDlg = NULL;
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

}

VOID CBO_ACQ_MissingDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_MissingDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_MissingDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_MissingDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnRESULT_SEARCH, OnbtnRESULTSEARCH)
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_MissingDataMgr message handlers

VOID CBO_ACQ_MissingDataMgr::OnClose() 
{
	DestroyWindow();
}

// �ʱ�ȭ
BOOL CBO_ACQ_MissingDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strVal;

	//=======================================================================
	// 1. SM����
	//=======================================================================
	if( InitESL_Mgr(_T("K2UPGRADE_�̳��ڷ����")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	//=======================================================================
	// 2. DM����
	//=======================================================================
	m_pDM			= FindDM(_T("DM_�̳��ڷ����")			);
	m_pPrintDM		= FindDM(_T("DM_�̳��ڷ����_��")		);
	m_pPrintObjDM = FindDM(_T("DM_�̳��ڷ����_��_��¿�"));
	if( m_pDM == NULL || m_pPrintDM == NULL || m_pPrintObjDM == NULL )
	{
		ESLAfxMessageBox(_T("[DM_�̳��ڷ����]\n[DM_�̳��ڷ����_��]\n[DM_�̳��ڷ����_��_��¿�]\n�������� �ʾҽ��ϴ�."));
		return FALSE;
	}

	//=======================================================================
	// 3. CM����
	//=======================================================================
	// 2004.11.16 �켱�����ڷ� ��뿩�ο� ���� CM�� �ٸ� �ɷ� �����Ͽ� ����Ѵ�.
	// BEGIN:
	CString strPriorityCatalogYN;
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), strPriorityCatalogYN);

	m_pCM = FindCM(_T("CM_�̳��ڷ����"));
	if( m_pCM == NULL ) 
	{
		ESLAfxMessageBox( _T("[CM_�̳��ڷ����]�� �������� �ʾҽ��ϴ�.") );
		return FALSE;
	}
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	// END:

	//=======================================================================
	// 4. ���������������, 5163/64 ���θ� ESL_Manage���̺��� ���� �����´�.
	//=======================================================================
	// 2005-01-13
	// ���� �ڸ��� �������� �����´�.
	GetManageValue(_T("����"), _T("����"), _T("���������������"), _T(""), m_strPosNumber);
	if( GetManageValue(_T("����"), _T("����"), _T("���԰��ݴ�������"), _T(""), m_strOrderPrice_Unit) < 0 )
	{
		m_strOrderPrice_Unit = _T("0");
	}
	GetManageValue(_T("��Ÿ"), _T("����"), _T("5163"), _T(""), m_strIs5163);
	GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T(""), m_strIs64);

		
	//=======================================================================
	// 5. ��縮�ҽ��� �����.
	//=======================================================================
	GetDlgItem(IDC_stcMISSING_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtMISSING_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcMISSING_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtMISSING_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);	
	GetDlgItem(IDC_btnRESULT_SEARCH)->ShowWindow(SW_HIDE);	
	
	//=======================================================================
	// 6. �˻�âŬ������ ��������� ����Ѵ�.
	//=======================================================================
	if( m_pSearchDlg != NULL )
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_MISSING, PURCHASE, BO_MISSING, m_pDM->CONNECTION_INFO);


	//=======================================================================
	// 7. ��¹� Ŭ������ ��������� ����Ѵ�.
	//=======================================================================
	if( m_pPrintDlg != NULL )
	{
		delete m_pPrintDlg;
		m_pPrintDlg = NULL;
	}
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_MISSING, PURCHASE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	

	//=======================================================================
	// 8. �����׸��� �����Ѵ�.
	//=======================================================================
	InitSortItem();

	//=======================================================================
	// 9. ��ũ������ �����Ѵ�.
	//=======================================================================
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);

	//=======================================================================
	// 10. �̳��ڷ��� ��,å���� �˷��ش�.
	//=======================================================================
	SetMissingDataInfo();

	//=======================================================================
	// 13. �˾��޴��� �����Ѵ�.
	//=======================================================================
	InitPopupMenuItem();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  
}

VOID CBO_ACQ_MissingDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//====================================================
	//2008.07.07 UPDATE BY PJW : SIZE ������
//	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcMISSING_DATA);
//	if(pBtn->GetSafeHwnd() == NULL) return;
//	pBtn->MoveWindow(5, 5, cx-10, 30);
	//====================================================

	CRect rect;
	//====================================================
	//2008.07.07 UPDATE BY PJW : SIZE ������
	CButton *pBtn = (CButton*)GetDlgItem(IDC_stcMISSING_GROUP_CREATE_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	//====================================================
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtMISSING_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcMISSING_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(165, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtMISSING_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(225, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_btnRESULT_SEARCH);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(400, 10, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMISSING_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 40, cx-10, cy-45);	
	pGrid->MoveWindow(-2, 35, cx+2, cy);
}

VOID CBO_ACQ_MissingDataMgr::ShowDivNoMgrDlg()
{
	CDivisionNo_Mgr mgr(this);
	mgr.SetWorkStatus(_T("�˼��Ϸ�"));
	mgr.DoModal();
}

VOID CBO_ACQ_MissingDataMgr::OrderCancel()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
}

VOID CBO_ACQ_MissingDataMgr::CheckInMissingData(CString strWorkingStatus)
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("�׸���"), (DWORD*)m_pGrid);
	api.SetTempData(_T("�׸����÷���ȣ"), m_nEditCol);
	api.SetTempData(_T("���º��濩��"), TRUE);
	api.SetTempData(_T("�ڷ����"), strWorkingStatus.GetBuffer(0));
	ids = api.SPFExecutePath(_T("�˼�ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�۾��Ͻ� �˼��۾� ������ ������ �� �����ϴ�."));
		return;
	}
	else if(ids > 0) return ;
	
	ESLAfxMessageBox(_T("�۾��Ͻ� �˼��۾� ������ �����Ͽ����ϴ�."));

	m_pPrintDM->FreeData();
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	ids = mgr.SPFExecutePath(_T("åTo��"));
	SetMissingDataInfo();
	DisplayGrid();
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ڷ����
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_MissingDataMgr::DeleteMissingData()
{
	//=======================================================================
	// 1. �ڷ� ���� ���θ� üũ�Ѵ�.
	//=======================================================================
	INT ids = CheckSelectDataValid();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	//=======================================================================
	// 2. API�� ���� �����ϰ� [����]�۾��� �����Ѵ�.
	//=======================================================================
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_MISSING);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("����Ÿ��"), (INT)VOLUME_DELETE);
	api.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
	api.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_��ȯ���ΰ���"));
	api.SetTempData(_T("��������"), _T(""));
	api.SetTempPtr(_T("��Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("��������Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("���Ϸù�ȣ����Ʈ"), NULL);
	ids = api.SPFExecutePath(_T("����"));

	//=======================================================================
	// 3. [����]�۾��� ���� ���� �Ǿ��ٸ� �ϴܳ���� �׸��带 �ٽ� �׸���.
	//=======================================================================
	if( ids == 0 )
	{
		SetMissingDataInfo();
		DisplayGrid();	
	}
}

VOID CBO_ACQ_MissingDataMgr::ShowMissingDataDlg(OPERATION_TYPE nOpType)
{
	// 2005-01-06 ������ �׸����� ��� �ٱǺ� ������ ȭ�� �������� ���� �ϱ� ���� �ϰ� display�ϹǷ� 
	// ���� �÷� ��ȣ ����� ���� �д� 
	if(m_pGrid) m_pGrid->m_bProtectSelectColumn = TRUE;

	INT nRowCnt = m_pPrintDM->GetRowCount();
	CSpecies_Form dlg(this);
	dlg.SetAcqType(PURCHASE);
	dlg.SetSpeciesType(BO_MISSING);
	dlg.SetOpType(nOpType);
	if(nOpType != SINSERT)
	{
		if(0 == m_pPrintDM->GetRowCount()) 
		{
			ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
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

	dlg.SetSpeciesKeyAlias(_T("IBS_��KEY"));
	dlg.SetAccuKeyAlias(_T("BP_��������KEY"));
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentType(VOLUME);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
//	DisplayGrid();
	SetMissingDataInfo();
	if(nOpType == SINSERT)
	{
		INT nCurSel = m_pPrintDM->GetRowCount();
		if(nCurSel > nRowCnt)
		{
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}

	// 2005-01-06 
	// ���� �÷� ��ȣ ����� ����.
	if(m_pGrid) m_pGrid->m_bProtectSelectColumn = FALSE;

}

VOID CBO_ACQ_MissingDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();	
}

VOID CBO_ACQ_MissingDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

BOOL CBO_ACQ_MissingDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_MissingDataMgr::OnbtnRESULTSEARCH() 
{
	// TODO: Add your control notification handler code here
	if(m_pResultSearchDlg != NULL)
	{
		delete m_pResultSearchDlg;
		m_pResultSearchDlg = NULL;
	}
	m_pResultSearchDlg = new CSearchMgr(this, BO_ACQ_RESEARCH, m_pPrintDM, m_pGrid);
	m_pResultSearchDlg->ShowSearchDlg();
}

UINT CBO_ACQ_MissingDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("�̳��ڷḦ �������Դϴ�."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		SetMissingDataInfo();
		DisplayGrid();
		return 0;
	}
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	INT ids = mgr.SPFExecutePath(_T("åTo��"));
	DeleteDataProcessingDlg();
	if(ids < 0) return ids;
	m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
	SetMissingDataInfo();
	//SortByGroupInfo();
	DisplayGrid();
	
	return 0;
}

UINT CBO_ACQ_MissingDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	INT ids;
	if(lParam == BO_ACQ_MISSING_DATA)
	{
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CString strDivAlias = _T("BP_��������KEY");
		CString strExtraAlias = _T("BB_���Ϸù�ȣ");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue, strExtraAlias);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
				return -1;
			}
		}
		ShowDataProcessingDlg();
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("����ƮŸ��"), lParam);
		PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
		PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pPrintDM);
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pPrintObjDM);
		PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
		ids = PrintMgr.SPFExecutePath(_T("�Ǹ���Ʈ���"));
		DeleteDataProcessingDlg();
		if(ids < 0) 
		{
			ESLAfxMessageBox(_T("��µ����Ϳ� ������ �ֽ��ϴ�."));
			return -1;
		}
		m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
		/********************************************
		 ********************************************/
		//20080425 ADD BY PJW : ����ȭ�� ��½� �׸��峻���� ���� ������ ����
		if( 1 == m_pPrintDlg->GetPrintDataType() && 0 == m_pPrintDlg->GetDataSortYN())
		{
			INT nCntPrintDM;
			INT nCntPrintObjDM;
			CString strPrintDMRecKey;
			CString strPrintObjDMRecKey;
			nCntPrintDM = m_pPrintDM->GetRowCount();
			nCntPrintObjDM = m_pPrintObjDM->GetRowCount();
			for(INT i = 0 ; i < nCntPrintDM ; i++)
			{
				m_pPrintDM->GetCellData(_T("BB_åKEY"),i,strPrintDMRecKey);
				for(INT j = 0 ; j < nCntPrintObjDM ; j++)
				{
					m_pPrintObjDM->GetCellData(_T("BB_åKEY"),j,strPrintObjDMRecKey);
					if( strPrintDMRecKey == strPrintObjDMRecKey )
					{
						m_pPrintObjDM->SORT_SwapRow(i,j);
					}

				}

			}
		}
		/********************************************
		 ********************************************/
		m_pPrintDlg->Print();
	}
	if(lParam == BO_ACQ_NONCHECKIN_STATISTICS)
	{
		CBO_Purchase_Statistics dlg(this);
		dlg.SetOpenType(-1000);
		dlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
		dlg.SetDivNoWorkStatus(_T("�˼��Ϸ�"));
		dlg.SetSpeciesType(BO_MISSING);
		dlg.DoModal();		
	}
	return 0;
}

VOID CBO_ACQ_MissingDataMgr::SetMissingDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d�� %då�� �̳� �ڷᰡ �˻��Ǿ����ϴ�."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), strMsg);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

INT CBO_ACQ_MissingDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode, strOrderSerialNo;
	INT nGroupNo, nSerialNo, nWorkNo, nOrderSerialNo;
	TCHAR* pEndPtr = NULL;

	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pPrintDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("IBS_�ǻ���"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pPrintDM->GetCellData(_T("IBS_��ǥ��"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_ǥ������"), strTmpData, nRow);
		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_������"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pPrintDM->GetRowCount()-1) m_nRow = -1;
	}
	
	if(strDMFieldAlias == _T("UDF_��������"))
	{	
		strAcqYear = m_pPrintDM->GetCellData(_T("BP_���Գ⵵"), nRow);
		strWorkCode = m_pPrintDM->GetCellData(_T("IBS_��������"), nRow);
		strGroupNo = m_pPrintDM->GetCellData(_T("BP_�׷��ȣ"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		strWorkNo = m_pPrintDM->GetCellData(_T("BP_�۾���ȣ"), nRow);
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		strSerialNo = m_pPrintDM->GetCellData(_T("BP_�Ϸù�ȣ"), nRow);
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
		strOrderSerialNo = m_pPrintDM->GetCellData(_T("BP_�����Ϸù�ȣ"), nRow);
		nOrderSerialNo = _ttoi(strOrderSerialNo.GetBuffer(0));
		strTmpData.Format(_T("%04d-%05d"), nGroupNo, nOrderSerialNo);
		m_pPrintDM->SetCellData(_T("UDF_�����������"), strTmpData, nRow);
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);		
	}
	else if(strDMFieldAlias == _T("BP_��������"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		
		if		( strTmpData == _T("1") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC��������ȣ����"), str, str);
		}
		else if	( strTmpData == _T("2") ) 
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC��������ȣ����"), str, str);
		}
		else if	( strTmpData == _T("3") || strTmpData == _T("7") ) 
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("LC������ȣ"), str, str);
		}
		else //if(strTmpData.IsEmpty()) 
		{
			str = _T("");
		}

		m_pPrintDM->SetCellData(_T("UDF_�������м���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_����ó��ȣ����"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("BP_����ó��ȣ"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����ó����"), strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	/*
	else if(strDMFieldAlias == _T("BP_������"))
	{
		strTmpData = m_pPrintDM->GetCellData(strDMFieldAlias, nRow);
		if(strTmpData.IsEmpty()) 
		{
			str = _T("0");
			m_pPrintDM->SetCellData(strDMFieldAlias, _T("0"), nRow);
		}
	}
	*/
	else if(strDMFieldAlias == _T("UDF_���ΰ�"))
	{
		DOUBLE dPrice;
		strTmpData = m_pPrintDM->GetCellData(_T("UDF_���ְ���_��"), nRow);
		strTmpData.Replace(_T(","), _T(""));
		//dPrice = (DOUBLE)atof(strTmpData);
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		if( dPrice == 0 )	strTmpData = _T("0");
		else
		{
			if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
			else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
			else										strTmpData.Format(_T("%.0f"), dPrice);
		}		

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, strTmpData, nRow);
		m_pPrintDM->SetCellData(_T("UDF_��ȯ���ΰ���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_����"))
	{
		CString strTmpData;
		INT nBookCnt = 0;
		DOUBLE dPrice;
		strTmpData = m_pPrintDM->GetCellData(_T("BB_����"), nRow);
		//dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		strTmpData = m_pPrintDM->GetCellData(_T("UDF_å��"), nRow);
		nBookCnt = _ttoi(strTmpData.GetBuffer(0));
		// 2004.09.14
		strTmpData = TruncPrice(m_pPrintDM, dPrice*nBookCnt);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(_T("UDF_��ȯ����"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_�ܰ�"))
	{
		// 2004.12.02
		CString strTmpData;
		DOUBLE dPrice;
		str.Replace(_T(","), _T(""));
		//dPrice = (DOUBLE)atof(str.GetBuffer(0));
		dPrice = _tcstod(str.GetBuffer(0), &pEndPtr);

		strTmpData = TruncPrice(m_pPrintDM, dPrice);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CBO_ACQ_MissingDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pPrintDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pPrintDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_MissingDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_MissingDataMgr)
	ON_EVENT(CBO_ACQ_MissingDataMgr, IDC_gridMISSING_DATA, -600 /* Click */, OnClickgridMISSINGDATA, VTS_NONE)
	ON_EVENT(CBO_ACQ_MissingDataMgr, IDC_gridMISSING_DATA, -601 /* DblClick */, OnDblClickgridMISSINGDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_MissingDataMgr::OnClickgridMISSINGDATA() 
{
	// TODO: Add your control notification handler code here
/*	INT nRow = -1;
	INT nCol = -1;
	nRow = m_pGrid->GetRow();
	nCol = m_pGrid->GetCol();
	
	if (10 == nCol) 
	{
		this->m_sDeliveredCountBackup = m_pGrid->GetText();
		m_pGrid->GridEdit(m_pGrid->GetRow()-1, m_pGrid->GetCol()-1);
	}
	else
	{
		this->m_bCellEditing = FALSE;
	}	
*/
}


BOOL CBO_ACQ_MissingDataMgr::PreTranslateMessage(MSG* pMsg) 
{
	if (VK_RETURN == pMsg->wParam ) 
	{
		if (GetDlgItem(IDC_gridMISSING_DATA)->m_hWnd == pMsg->hwnd)
		{
			this->m_bCellEditing = !this->m_bCellEditing;
			if (!this->m_bCellEditing) 
			{
				OnClickgridMISSINGDATA();
			}
		}
	}
	
	if (ESL_GRID_MODIFY == pMsg->message)
	{
		// ��Ǳå�� �÷��� �����Ǿ��ٸ�.
		if (pMsg->lParam == m_nEditCol) 
		{	
			if (0>ReCalcBookCount(pMsg->wParam + 1))	///���� ���ȣ
			{
				ESLAfxMessageBox(_T("�̳�å���� �����ϴµ� �����Ͽ����ϴ�."));
			}	
/*			if(0>ReCalcDeliveryPrice(pMsg->wParam + 1))	///���� ���ȣ
			{
				ESLAfxMessageBox(_T("��ǰ������ �����ϴµ� �����Ͽ����ϴ�."));
			}
*/		}
	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

BOOL CBO_ACQ_MissingDataMgr::CheckIsDigit(CString str)
{
	if(str.IsEmpty()) return FALSE;

	TCHAR * tmp = str.GetBuffer(0);
	INT size = str.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;
}


INT CBO_ACQ_MissingDataMgr::ReCalcBookCount(INT nRow/*def:-1*/)
{
	INT nCurRow = -1;
	if (-1==nRow)	nCurRow = m_pGrid->GetRow();
	else			nCurRow = nRow;
	///�Էµ� å �� ����
	CString sTemp, strDeliveredCount;
	BOOL bInvalidCount = FALSE;
	INT nOrderCount;
	
	nOrderCount	= _ttoi((m_pGrid->GetTextMatrix(nCurRow, m_nEditCol)).GetBuffer(0));
	strDeliveredCount = m_pGrid->GetTextMatrix(nCurRow, m_nEditCol+1);
	
	if(!CheckIsDigit(strDeliveredCount))
	{
		sTemp.Format(_T("��ǰå���� ���� �ƴѰ�( %s )�� �ԷµǾ����ϴ�."), strDeliveredCount);
		bInvalidCount = TRUE;
	}
	INT nDeliveredCount = _ttoi((strDeliveredCount).GetBuffer(0));
	INT nNondeliveredCount = nOrderCount-nDeliveredCount;

	///��ǰå���� �ֹ�å������ ũ�ٸ�
	if (nOrderCount<nDeliveredCount) 
	{
		sTemp.Format(_T("��ǰå��(%d)�� �ֹ�å��(%d)���� Ů�ϴ�."), nDeliveredCount, nOrderCount);
		bInvalidCount = TRUE;
	}
	///��ǰå���� 0���� �۴ٸ�
	else if (0>nDeliveredCount)
	{
		sTemp.Format(_T("��ǰå��(%d)�� 0 ���� �۽��ϴ�."), nDeliveredCount, nOrderCount);
		bInvalidCount = TRUE;
	}
	
	///�߸��� å�� �Էµ�
	if (bInvalidCount) 
	{
		ESLAfxMessageBox(sTemp);
		///���� ��ǰå��/�ֹ�å�� ������ ����
		m_pGrid->SetTextMatrix(nCurRow, m_nEditCol+1, m_pPrintDM->GetCellData(_T("UDF_��ǰå��"), nCurRow-1));
		nDeliveredCount = _ttoi(m_pPrintDM->GetCellData(_T("UDF_��ǰå��"), nCurRow-1).GetBuffer(0));
		sTemp.Format(_T("%d"), nOrderCount-nDeliveredCount);
		m_pGrid->SetTextMatrix(nCurRow, m_nEditCol+2, sTemp);
		return 1;
	}
	
	sTemp.Format(_T("%d"), nOrderCount-nDeliveredCount);
	m_pGrid->SetTextMatrix(nCurRow, m_nEditCol+2, sTemp);

	return 0;
}

VOID CBO_ACQ_MissingDataMgr::OnDblClickgridMISSINGDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0 ) return ;
	if(m_pGrid->GetCol() == m_nEditCol+1) return ;
	
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return;
	}
	ShowMissingDataDlg(GUPDATE);
}

INT CBO_ACQ_MissingDataMgr::CheckSelectDataValid()
{
	if(m_pPrintDM->GetRowCount() == 0) return -1;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_ACQ_MissingDataMgr::SelDataDeliveryProc()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("GET_COLUMN"), _T("å��"));
	api.SetTempData(_T("SET_COLUMN"), _T("��ǰå��"));
	api.SetTempData(_T("CLEAR_COLUMN"), _T("�̳�å��"));
	ids = api.SPFExecutePath(_T("��ǰó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ��ǰó���� �� �����ϴ�."));
		return ;
	}
}

VOID CBO_ACQ_MissingDataMgr::SelDataDeliveryCancelProc()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("GET_COLUMN"), _T("å��"));
	api.SetTempData(_T("SET_COLUMN"), _T("�̳�å��"));
	api.SetTempData(_T("CLEAR_COLUMN"), _T("��ǰå��"));
	ids = api.SPFExecutePath(_T("��ǰ���"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ��ǰ����� �� �����ϴ�."));
		return ;
	}
}

VOID CBO_ACQ_MissingDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();

	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	m_pGrid->m_bGridEdit = TRUE;
	m_pGrid->SELEDITOR_RemoveAllColumns();
	CBO_ACQ_Business_Api api(this);
	m_nEditCol = api.GetGridColumnIdx(m_pGrid, _T("��ǰå��"));
	m_pGrid->SELEDITOR_AddColumn(m_nEditCol);
}

VOID CBO_ACQ_MissingDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CCodeChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.SetAcqType(1);
	dlg.SetSpeciesYN(FALSE);
	if(IDOK == dlg.DoModal())
	{
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_MissingDataMgr::ShowDataProcessingDlg()
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

VOID CBO_ACQ_MissingDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

/*
INT CBO_ACQ_MissingDataMgr::SortByGroupInfo()
{
	if(m_pPrintDM == NULL) return 0;
	if(m_pPrintDM->GetRowCount() == 0) return 0;

	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_���Գ⵵");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�����Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return 0;
}
*/

CString CBO_ACQ_MissingDataMgr::GetCommonManageValue(CString strGroup2, CString strGroup3, CString strClassAlias)
{
	CString strValue, strQuery;
	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2='%s' AND GROUP_3='%s' AND CLASS_ALIAS='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), 
		strGroup2, strGroup3, strClassAlias);
	m_pDM->GetOneValueQuery(strQuery, strValue);
	return strValue;
}

VOID CBO_ACQ_MissingDataMgr::ShowPriceSumDlg()
{
	CPriceSumDlg pDlg(this);
	pDlg.SetOpenMode(K2UPGRADE);
	pDlg.SetSpeciesType(BO_MISSING);
	pDlg.SetParentDM(m_pPrintDM);
	pDlg.SetParentBookDM(m_pDM);
	pDlg.SetParentGrid(m_pGrid);
	pDlg.SetSpeciesYN(FALSE);
	pDlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
	pDlg.DoModal();
}

VOID CBO_ACQ_MissingDataMgr::ShowSortDlg(INT nType/*=1*/)
{
	if(m_pSortItemSet == NULL || m_pPrintDM == NULL) 
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return;
	}
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pPrintDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(nType);
	if(IDOK == dlg.DoModal() && nType > 0)
		DisplayGrid();
}

UINT CBO_ACQ_MissingDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

BOOL CBO_ACQ_MissingDataMgr::InitSortItem()
{
	CStringArray* pTmpData	 = NULL;	
	CMapStringToString* pMap = NULL;
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_���Գ⵵"));
	pTmpData->Add(_T("IBS_��������"));
	pTmpData->Add(_T("BP_�׷��ȣ"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("BP_�����Ϸù�ȣ"));
	pTmpData->Add(_T("BB_���Ϸù�ȣ"));

	pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_�������м���"), _T("BP_��������"));
	pMap->SetAt(_T("UDF_�Է±��м���"), _T("BP_�Է±���"));
	pMap->SetAt(_T("UDF_����ó��ȣ����"), _T("BP_����ó��ȣ"));
	
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

CString CBO_ACQ_MissingDataMgr::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	// 2004.12.01 ������å�� ���� ��������������� �о�ͼ� ������ �ڸ�����ŭ ǥ���ϵ��� �Ѵ�.
	CString strTmpData, strTmp, strValue;
	INT nPos = -1;

	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	// 2005-01-13
	if(m_strPosNumber == _T("1"))  
		strTmpData = strTmp.Left(nPos+2);
	else if(m_strPosNumber == _T("2"))  
		strTmpData = strTmp.Left(nPos+3);
	else  // m_strPosNumber == _T("0") || m_strPosNumber.Empty()
		strTmpData = strTmp.Left(nPos);

	return strTmpData;
}

HBRUSH CBO_ACQ_MissingDataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
