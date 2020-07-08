// BO_ACQ_PurchaseDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_PurchaseDataMgr.h"
#include "BO_ACQ_OrderDlg.h"
#include "BO_ACQ_ReviewDlg.h"
#include "PurchaseDataPrintDlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/DivisionNo_Select_Dlg.h"
#include "../../../����/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
#include "../../../����/BO_ACQ_COMMON/BO_ACQ_ImportMgr.h"
#include "../../../����/BO_ACQ_COMMON/OrderMailMgr.h"
#include "../../../����/Duplicate_Data_Search/DUP_KEY_SETTING.h"
#include <direct.h>
// 2005-01-31
#include "DataExportDlg.h"
#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileImportMgr.h"
#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileExportMgr.h"
#include "../../../����/CMarcChanger/MARC_CHANGER_01.h"
//==============================================
//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
#include "SBL.H"
//==============================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PurchaseDataMgr dialog


CBO_ACQ_PurchaseDataMgr::CBO_ACQ_PurchaseDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_PurchaseDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_PurchaseDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pPrintDM = NULL;
	m_pPrintObjDM = NULL;
	m_pSubTotalPrintDM = NULL;
	m_pSearchDlg = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_strDivNo = _T("");
	m_strAcqYear = _T("");
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_bPurchaseCheckYN = FALSE;
	m_pSortItemSet = NULL;
	m_nRow = -1;
	m_strPosNumber.Empty(); //2005-01-13
	m_strIs5163 = _T("");
	m_strIs64	= _T("");
	m_strIs90	= _T("");
	m_strOrderPrice_Unit = _T("");
}

CBO_ACQ_PurchaseDataMgr::~CBO_ACQ_PurchaseDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}	
	if(m_pDuplicateSearchMgr)
	{
		delete m_pDuplicateSearchMgr;
		m_pDuplicateSearchMgr = NULL;
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

VOID CBO_ACQ_PurchaseDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_PurchaseDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_PurchaseDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_PurchaseDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(DUPLICATE_DATA_DELETE, OnDuplicateDataDelete)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PurchaseDataMgr message handlers
VOID CBO_ACQ_PurchaseDataMgr::OnClose() 
{
	
	CDialog::OnClose();
}

BOOL CBO_ACQ_PurchaseDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_���Դ���ڷ����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pDM = FindDM(_T("DM_�����ڷ����"));
	m_pPrintDM = FindDM(_T("DM_�����ڷ����_��"));
	m_pPrintObjDM = FindDM(_T("DM_�����ڷ����_��¿�"));
	m_pSubTotalPrintDM = FindDM(_T("DM_�ֹ��ڷ����_���"));
	// 2004.11.16 �켱�����ڷ� ��뿩�ο� ���� CM�� �ٸ� �ɷ� �����Ͽ� ����Ѵ�.
	CString strPriorityCatalogYN;
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), strPriorityCatalogYN);
	// 2005-01-13
	// ���� �ڸ��� �������� �����´�.
	GetManageValue(_T("����"), _T("����"), _T("���������������"), _T(""), m_strPosNumber);
	if( GetManageValue(_T("����"), _T("����"), _T("���԰��ݴ�������"), _T(""), m_strOrderPrice_Unit) < 0 )
	{
		m_strOrderPrice_Unit = _T("0");
	}
	
	// 2005-01-28
	GetManageValue(_T("��Ÿ"), _T("����"), _T("5163"), _T(""), m_strIs5163);
	GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T(""), m_strIs64);
	GetManageValue(_T("��Ÿ"), _T("����"), _T("�����"), _T(""), m_strIs90);

	
	m_pCM = FindCM(_T("CM_�����ڷ����"));
	if ( m_pCM == NULL ) {
		AfxMessageBox( _T("CM_�����ڷ���� error") );
		return -2;
	}
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	GetDlgItem(IDC_stcPURCHASE_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcPURCHASE_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPURCHASE_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);	
	
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_PURCHASE, PURCHASE, BO_PURCHASE, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_PURCHASE, PURCHASE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	//m_pPrintDlg->SetIs90Flag(m_strIs90);
	InitSortItem();
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	SetPurchaseDataInfo();
	InitPopupMenuItem();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_PurchaseDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//CButton *pBtn = (CButton *)GetDlgItem(IDC_stcPURCHASE_DATA);
	//if(pBtn->GetSafeHwnd() == NULL) return;
	//pBtn->MoveWindow(5, 5, cx-10, 30);

	CButton *pBtn;

	CRect rect;
	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_GROUP_CREATE_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcPURCHASE_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(165, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtPURCHASE_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(225, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridPURCHASE_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//==========================================
	//20080604 UPDATE BY PJW : ��ũ�ѹ� ������ �ϱ����켭 ���α��� ����
	//pGrid->MoveWindow(-2, 35, cx+2, cy-30);
	pGrid->MoveWindow(-2, 35, cx+2, cy-33);		
	//==========================================
}

VOID CBO_ACQ_PurchaseDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) 
	{
		m_pPrintDlg->Create(this);
	}
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowPurchaseDataDlg(OPERATION_TYPE nOpType)
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	BOOL strNewSaveFlag;
	CSpecies_Form dlg(this);
	dlg.SetAcqType(PURCHASE);
	dlg.SetSpeciesType(BO_PURCHASE);
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
	dlg.SetParentType(SPECIES);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	//========================================================
	//20080613 ADD BY PJW : ���������ϰ�� ������ Flag�� �޾ƿ´�
	strNewSaveFlag = FALSE;
	strNewSaveFlag = dlg.m_bNewSaveFlag;
	//========================================================
	

	SetPurchaseDataInfo();
	INT nCurCnt = m_pPrintDM->GetRowCount();

	//JOB.2020.0026 ADD BY KYJ : �Է� > �ݱ� > �Է� > ���� �� ��� �׸��� �׸��� ��ĭ �����Ǵ� ���� ����
	if( nCurCnt > 0 && SINSERT == nOpType )
	{
	//=====================================================
	//2008.12.15 ADD BY PJW : UDF_����ϱ����� �Է��ϰ�쵵 �����Ѵ�.
	CString strSpeciesRegCode;
	m_pPrintDM->GetCellData(_T("IBS_���ε�ϱ���"),nCurCnt-1,strSpeciesRegCode);
	strSpeciesRegCode.TrimLeft();	strSpeciesRegCode.TrimRight();
	strSpeciesRegCode.Replace(_T(" "),_T("/"));
	m_pPrintDM->SetCellData(_T("UDF_����ϱ���"),strSpeciesRegCode,nCurCnt-1);
	//=====================================================
	}
	//========================================================
	//20080613 UPDATE BY PJW : ���������ϰ�� �ֹ���ȣ�� ������ �ֹ���ȣ�� �������� 
	//if(nOpType == SINSERT)
	strNewSaveFlag = dlg.m_bNewSaveFlag;
	if(SINSERT == nOpType || strNewSaveFlag == strNewSaveFlag)
	//========================================================
	{
		if(nCurCnt > nRowCnt)
		{
			// ����-�Էµ� å ����ó�� _T("Y") ����
			// 2005-09-28 ������
			//
				for(INT i = nRowCnt ; i<nCurCnt ; i++)
				{
					m_pPrintDM->SetCellData(_T("BP_��������"), _T("Y"), i );
				}
				AllControlDisplay(_T("CM_�����ڷ����"));
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}
}

VOID CBO_ACQ_PurchaseDataMgr::DeletePurchaseData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_PURCHASE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("����Ÿ��"), (INT)SPECIES_DELETE);
	api.SetTempData(_T("å���˸��ƽ�"), _T("BP_����å��"));
	api.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_��ȯ����������"));
	api.SetTempData(_T("��������"), _T(""));
	api.SetTempPtr(_T("��Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("��������Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("���Ϸù�ȣ����Ʈ"), NULL);
	ids = api.SPFExecutePath(_T("����"));
	if(ids == 0)
	{
		SetPurchaseDataInfo();
		DisplayGrid();	
	}
}

VOID CBO_ACQ_PurchaseDataMgr::ReviewPurchaseData()
{
	CString strGroupNo, strSortDesc;
	
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� ���������� �����ϴ�."));
		return;
	}
	
	//===================================
	//20080603 ADD BY PJW : ����ó�����ڷ��� åŰ�� �Ѱ��ش�.
	m_pSearchDlg->GetGroupCollection()->m_strSelectBookKey = GetSelectBookKey();
	//===================================

	//�����ݷ����� ��ü�ڷ��� ��� Ư�������� ������ ���� ����
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("��ü�ڷ�"))
	{
		ESLAfxMessageBox(_T("���� ������ ������ �����ϴ�. �˻��� ���Ͽ� Ư�� ������ ������ �ֽʽÿ�."));
		return;
	}
	
	CBO_ACQ_ReviewDlg dlg(this);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	if(IDCANCEL == dlg.DoModal()) return ;

	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupNo);
	m_pCM->GetControlMgrData(_T("�׷��ȣ"), strGroupNo);
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempData(_T("�׷��ȣ"), strGroupNo.GetBuffer(0));
	api.SetTempData(_T("���Գ⵵"), m_pSearchDlg->GetGroupCollection()->GetAcqYear().GetBuffer(0));
	api.SetTempData(_T("��������"), m_pSearchDlg->GetGroupCollection()->GetWorkCode().GetBuffer(0));
	INT ids = api.SPFExecutePath(_T("����ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �� �� �����ϴ�."));
		return;
	}
	ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �Ͽ����ϴ�."));
	if(dlg.m_bReviewResultPrint)
	{	
		ShowDataProcessingDlg();
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CMakePrintDMMgr PrintMgr(this);
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("����ƮŸ��"), BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO);
		PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
		PrintMgr.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
		PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_����"));
		PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_���ΰ�"));
		PrintMgr.SetTempData(_T("åDM�˸��ƽ�"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
		PrintMgr.SetTempData(_T("KOLAS2����"), FALSE);
		PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), FALSE);
		PrintMgr.SetTempData(_T("��µ�����Ÿ��"), 0);
		PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("�׷��÷���"), (DWORD*)m_pSearchDlg->GetGroupCollection());
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pSubTotalPrintDM);
		PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
		PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T(""));
		
		ids = PrintMgr.SPFExecutePath(_T("�Ұ��ڷ����"));
		DeleteDataProcessingDlg();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
			return ;
		}
		if(ids == 1000) return ;
		
		m_pPrintDlg->CreatePrintMgr();
		m_pPrintDlg->SetPrintDM(m_pSubTotalPrintDM);
		m_pPrintDlg->SetSIReportFileName(_T("K2UP_���ǰ���ڷ���"));
		m_pPrintDlg->PrintPaper();
	}
}

VOID CBO_ACQ_PurchaseDataMgr::OrderPurcahseData()
{
	CBO_ACQ_Business_Api api(this);
	//BOOL bGridSelectCol;
	//==========================
	//2008.07.01 ADD BY PJW : �ֹ���ȣ�� �ο��Ѵ�.
	//==========================
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� ���������� �����ϴ�."));
		return;
	}
	//�����ݷ����� ��ü�ڷ��� ��� Ư�������� ������ ���� ����
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("��ü�ڷ�"))
	{
		ESLAfxMessageBox(_T("���� ������ ������ �����ϴ�. �˻��� ���Ͽ� Ư�� ������ ������ �ֽʽÿ�."));
		return;
	}

	CString strSpeciesKey;
	CString strBookSpeciesKey;
	INT nIdx, y;
	for ( y=0 ; y<m_pDM->GetRowCount() ; y++ )
	{	
		m_pDM->SetCellData(_T("����"), _T(""), y );	
	}			

	for (nIdx = 0 ; nIdx < m_pPrintDM->GetRowCount() ; nIdx++ )
	{
		if ( m_pGrid->GetTextMatrix(nIdx+1,1) == _T("V") )
		{
			if ( m_pPrintDM->GetCellData ( _T("BP_��������"), nIdx ) != _T("Y") )
			{
				/*
				if ( IDYES == ESLAfxMessageBox(_T("�� ���� ���� �ֹ���ȣ�� �ο����� ���� �ڷᰡ �ֽ��ϴ�. \r\n�ֹ���ȣ�� �ο��Ͻðų� �����и��� ���� �� �������� �ֹ���ȣ�� �ο��� �ڷḸ �ֵ��� �ؾ� �մϴ�. \r\n���� ���������� ���� ���� �и��� �Ͻðڽ��ϱ�? "), MB_YESNO) )
				{
					ShowDivNoMgrDlg();
					return;	
				}
				*/
				ESLAfxMessageBox(_T("������ �ڷ��� �ֹ���ȣ�� �ο����� ���� �ڷᰡ �ֽ��ϴ�. \r\n�ֹ���ȣ�� �ο��Ͽ� �ֽʽÿ�."));				
				return;				
			}
			m_pPrintDM->SetCellData( _T("����"), _T("V"), nIdx  );
			// 2008.03.12
			// �������� ���� ���¸� å������ �ݿ��Ѵ�.
			// DM_������������_�� -> DM_������������
			strSpeciesKey = m_pPrintDM->GetCellData(_T("IBS_��KEY"), nIdx );
			for ( y=0 ; y<m_pDM->GetRowCount() ; y++ )
			{
				strBookSpeciesKey = m_pDM->GetCellData(_T("IBS_��KEY"), y );
				if ( strSpeciesKey == strBookSpeciesKey )
				{
					m_pDM->SetCellData(_T("����"), _T("V"), y );
				}
			}			
		}
		else
		{
			m_pPrintDM->SetCellData( _T("����"), _T(""), nIdx  );
		}
	}	

	//�׷��÷��� ������ �����Ѵ�.
	CGroupCollection GroupCollection(this);
	m_pSearchDlg->GetGroupCollection()->CopyData(&GroupCollection);
	
	CheckGiveOrderSerialNo(&GroupCollection);
	api.CheckValidationOrderProcess(m_pDM, &GroupCollection);
	if(GroupCollection.GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("�ֹ��ϱ� ���� ���������� �����ϴ�."));
		return;
	}	
	
	//===================================
	//20080603 ADD BY PJW : ����ó�����ڷ��� åŰ�� �Ѱ��ش�.
	CString strSelectBookKey;
	strSelectBookKey = GetSelectBookKey();
	if( strSelectBookKey.IsEmpty() )
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	m_pSearchDlg->GetGroupCollection()->m_strSelectBookKey = strSelectBookKey;
	//===================================
	CBO_ACQ_OrderDlg dlg(this);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetGroupCollection(&GroupCollection);
	if(IDCANCEL == dlg.DoModal()) return ;
	
	api.InitTempData();	
	api.SetTempPtr(_T("�׷�����"), (DWORD*)&GroupCollection);
	CString strData = GroupCollection.GetAcqYear();
	api.SetTempData(_T("���Գ⵵"), strData.GetBuffer(0));
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = api.SPFExecutePath(_T("�ֹ�ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ �ֹ�ó�� �� �� �����ϴ�."));
		return;
	}
	//JOB.2019.0073 : �浹 License ����
	// 20070814 ADD PJW : �ֹ����Ϲ߼� ��� ���� ���� ����ǵ��� �Ѵ�.
// 	CString strMailSendYN;
// 	m_pDM ->GetOneValueQuery(_T(" SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='�ֹ������ڵ��߼�' AND GROUP_3='����' AND MANAGE_CODE = UDF_MANAGE_CODE "), strMailSendYN);
// 	
// 	if ( strMailSendYN == _T("Y") )	ShowOrderCancelMailMgr(ORDER, AUTO, &GroupCollection);
	//ShowOrderCancelMailMgr(ORDER, AUTO, &GroupCollection);

	ESLAfxMessageBox(_T("������ �ڷḦ �ֹ�ó�� �Ͽ����ϴ�."));
	//================================================
	//20080617 REM BY PJW : �׸��忡 ���� ����ó�� �ֹ�ó���Ҽ� �ֵ��� �ϱ����ؼ� �׷������ ���� ����
	//m_pSearchDlg->GetGroupCollection()->DeleteThisGroupInfo(&GroupCollection);
	//================================================
	api.InitTempData();
	api.SetTempPtr(_T("DM_BOOK"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("DM_SPECIES"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("DM_VOLUME"), (DWORD*)NULL);
	api.SetTempPtr(_T("GROUPINFO"), (DWORD*)&GroupCollection);
	//==================================================
	//20080604 UPDATE BY PJW : �׸����� ������ ��� ������� ���� �������� ���� �׷������ ���� �ʴ´�.
	//ids = api.SPFExecutePath(_T("�׷��ڷ����"));
	//if(ids < 0)
	//{
	//	ESLAfxMessageBox(_T("�ֹ��ڷḦ ������ �� �����ϴ�."));
	//	return ;
	//}
	CString strSelect;
	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();
	for ( INT i=0 ; i < nRowCnt ; i++ )
	{	
		m_pDM->GetCellData( _T("����"), i, strSelect );
		if( _T("V") == strSelect )
		{
			m_pDM->DeleteRow(i);
			--nRowCnt;
			i--;
		}
	}
	nRowCnt = m_pPrintDM->GetRowCount();
	for ( i=0 ; i < nRowCnt ; i++ )
	{	
		m_pPrintDM->GetCellData( _T("����"), i, strSelect );
		if( _T("V") == strSelect )
		{
			m_pPrintDM->DeleteRow(i);
			--nRowCnt;
			i--;
		}
	}
	//==================================================

	SetPurchaseDataInfo();
	
	//bGridSelectCol = m_pGrid->m_bProtectSelectColumn ; 
	//m_pGrid->m_bProtectSelectColumn = TRUE ;
	DisplayGrid();
	//m_pGrid->m_bProtectSelectColumn = bGridSelectCol ;
}

VOID CBO_ACQ_PurchaseDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CCodeChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.SetAcqType(1);
	if( IDOK == dlg.DoModal() )
	{
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_PurchaseDataMgr::ShowChangePurchaseInfoDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CPurchaseInfoChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.SetSpeciesType(1);

	if(IDOK == dlg.DoModal())
	{
//		m_pGrid->SetProtectSelectColumn(TRUE);
		//===================================================
		//2009.03.27 UPDATE BY LKS : ��¹��� ���Ͽ� ��˻� ���ش�.
		OnQueryCreate(0,0);
		//===================================================
		DisplayGrid();
//		m_pGrid->SetProtectSelectColumn(FALSE);
		AllControlDisplay(_T("CM_�����ڷ����"));
	}
	SetPurchaseDataInfo();
	/******************************************
	 ******************************************/
	//20080319 ADD BY PJW : �׸����� ������ �����ϱ� ���ؼ� �缱�� ���ش�.
	INT nSelect;
	CString strTemp;
	INT nCnt;
	nCnt = m_pGrid->SelectGetCount();
	nSelect = m_pGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		m_pGrid->SetTextMatrix( nSelect+1, 1,_T("V"));
		nSelect = m_pGrid->SelectGetNext();
	}

	/******************************************
	 ******************************************/
//DEL	CString strWhere;
//DEL	strWhere.Format(_T("sdfds"));
//DEL	m_pDM_BOOK->RefreshDataManager(strWhere);


}

VOID CBO_ACQ_PurchaseDataMgr::ShowDivNoMgrDlg()
{
	CDivisionNo_Mgr mgr(this);
	mgr.SetWorkStatus(_T("���Դ��"));
	mgr.DoModal();
	
	//===================================================
	//2009.06.04 ADD BY LKS : �˻��� ���� ���� �˻�
	
	CString strAcqNo;
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_edtPURCHASE_DIVNO);
		pEdit->GetWindowText(strAcqNo);
	if(!strAcqNo.IsEmpty())
		OnQueryCreate(0,0);
	//===================================================
}

VOID CBO_ACQ_PurchaseDataMgr::ShowPriceSumDlg()
{
	CPriceSumDlg pDlg(this);
	pDlg.SetOpenMode(K2UPGRADE);
	pDlg.SetSpeciesType(BO_PURCHASE);
	pDlg.SetParentDM(m_pPrintDM);
	pDlg.SetParentGrid(m_pGrid);
	pDlg.SetSpeciesYN(TRUE);
	pDlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
	pDlg.DoModal();
}

VOID CBO_ACQ_PurchaseDataMgr::CopyPurchaseDataSearch()	
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	if(!m_pDuplicateSearchMgr)
	{
		m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
		m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	}

	if(m_pDuplicateSearchMgr->GetRunning())		
	{
		ESLAfxMessageBox(_T("�̹� �ߺ����簡 �������Դϴ�."));
		return ;
	}		
	m_pDuplicateSearchMgr->ShowWorkDuplicateSearch(_T("�ߺ�����"), m_pPrintDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"), m_pSearchDlg->GetGroupCollection());
}
/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ] 
// �������� ���ٹ�ư ����
//
// [ �̴Ͻ��� ]
// 1. ��¿� DM �� �׸����� ������ üũ�Ͽ� ���� ���� �ִ� ������ �����Ͽ�
//	  �������縦 �غ��Ѵ�.
// 2. �������� ������ ���� �������� Ŭ������ �����Ѵ�.
// 3. �������簡 �������ε� �� ������ �Ϸ� �Ѵٸ� �޽����� �˷��ش�.
// 4. �������縦 �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_PurchaseDataMgr::DupPurchaseDataSearch()
{
	INT ids = 0;


	//=======================================================================
	// 1. ��¿� DM �� �׸����� ������ üũ�Ͽ� ���� ���� �ִ� ������ �����Ͽ�
	//	  �������縦 �غ��Ѵ�.
	//=======================================================================
	ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}


	//=======================================================================
	// 2. �������� ������ ���� �������� Ŭ������ �����Ѵ�.
	//=======================================================================
	if(!m_pDuplicateSearchMgr)
	{
		m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
		m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	}

	//=======================================================================
	// 3. �������簡 �������ε� �� ������ �Ϸ� �Ѵٸ� �޽����� �˷��ش�.
	//=======================================================================
	if(m_pDuplicateSearchMgr->GetRunning())		
	{
		ESLAfxMessageBox(_T("�̹� �������簡 �������Դϴ�."));
		return ;
	}		
	
	//=======================================================================
	// 4. �������縦 �����Ѵ�.
	//	- m_pPrintDM : DM_�����ڷ����_��
	//=======================================================================
	m_pDuplicateSearchMgr->ShowHoldDuplicateSearch(_T("��������"), m_pPrintDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"));
}

BOOL CBO_ACQ_PurchaseDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �˻����������� DM�� �����Ѵ�.
// [ �̴Ͻ��� ]
// 1. ���α׷���â�� �����Ѵ�.
// 2. �˻��� �������� �̿��Ͽ� åDM�� �����Ѵ�.
// 3. ����� åDM�� �̿��Ͽ� ��DM�� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
UINT CBO_ACQ_PurchaseDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
	BEGIN_SBL();
	//==============================================
	
	//=======================================================================
	// ��������
	//=======================================================================	
	CString strQuery = _T("");
	
	//=======================================================================
	// 1. ���α׷���â�� �����Ѵ�.
	//=======================================================================
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("���Դ���ڷḦ �������Դϴ�."));

	//=======================================================================
	// 2. �˻��� �������� �̿��Ͽ� åDM�� �����Ѵ�.
	//=======================================================================
	m_pSearchDlg->GetWhereStmt(strQuery);
	TRACE(_T("strQuery"));
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());

	//=======================================================================
	// 3. ����� åDM�� �̿��Ͽ� ��DM�� �����Ѵ�.
	//=======================================================================
	m_pPrintDM->FreeData();
	if( 0 != m_pDM->GetRowCount() )
	{
		CBO_ACQ_Business_Api mgr(this);
		mgr.InitTempData();
		mgr.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
		mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
		mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	
		INT ids = mgr.SPFExecutePath(_T("åTo��������"));
		DeleteDataProcessingDlg();
			if( ids < 0 )	return ids;
	
		m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
		SetPurchaseDataInfo();
		//SortByGroupInfo();

		DisplayGrid();
	}
	else
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		SetPurchaseDataInfo();
		DisplayGrid();
	}
	
	// 2006.04.18 KBS - �����䱸���� : �˻��� �˻�â �����
	if( m_strIs90 == _T("Y") )	m_pSearchDlg->HideSearchDlg();
	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
	INT nRowCount = m_pPrintDM->GetRowCount();
	END_SBL(nRowCount);
	//==============================================
	return 0;
}


UINT CBO_ACQ_PurchaseDataMgr::OnDuplicateDataDelete(LPARAM lParam, WPARAM wParam)
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("��DM"), NULL);
	api.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("����������"), (DWORD*)m_pDuplicateSearchMgr->GetAcqKeyArray());
	api.SetTempData(_T("��Alias"), _T("IBS_��KEY"));
	INT ids = api.SPFExecutePath(_T("DM�ڷ����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ DM���� ������ �� �����ϴ�."));
		return 0;
	}
	DisplayGrid();
	SetPurchaseDataInfo();
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//
// [ �̴Ͻ��� ]
//
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PurchaseDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT ids, nGroupNo, nWorkNo, nSerialNo, nBookCnt;
	DOUBLE dPrice, dDisc_Rate, dExchangeRate, dDisc_Price;
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode;
	CString strData1, strData2, strPrice, strDiscRate, strExchangeRate;
	CMarc marc;
	
	TCHAR* pEndPtr = NULL;

	if( m_nRow != nRow )
	{
		strTmpData = m_pPrintDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if( ids < 0 )	return -1;

		strTmpData.Empty();	
		ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if( ids >= 0 )	m_pPrintDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, nRow);

		strTmpData.Empty();	
		ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if( ids >= 0 )	m_pPrintDM->SetCellData(_T("IBS_�ǻ���"), strTmpData, nRow);

		strTmpData.Empty();	
		// IBS ��ǥ���� �ְ������°�?
		strTmpData = m_pPrintDM->GetCellData(_T("IBS_��ǥ��"), nRow);

		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_ǥ������"), strTmpData, nRow);

		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_������"), strTmpData, nRow);
		
		// 2005-01-27 5163�߰�����(���翡�� �ݿ�)
		if( _T("Y") == m_strIs5163 || _T("Y") == m_strIs64 )
		{
			strTmpData =m_pPrintDM->GetCellData(_T("BS_����������ڵ�1"), nRow);
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�����з��ڵ�1"), strTmpData, strTmpData);
			m_pPrintDM->SetCellData(_T("UDF_����������ڵ�1"), strTmpData, nRow);

			// 2005-01-28
			// UDF_���������ڻ���
			strData1 = m_pPrintDM->GetCellData(_T("UDF_ǥ������"), nRow);
			strData2 = m_pPrintDM->GetCellData(_T("IBS_������"), nRow);
			strTmpData.Empty();
			if( strData2.IsEmpty() == TRUE )	strTmpData = strData1;
			else  strTmpData = strData1 + _T(" / ") + strData2;
			m_pPrintDM->SetCellData(_T("UDF_���������ڻ���"), strTmpData, nRow);

			// UDF_�����ڻ���
			strData1.Empty();		strData2.Empty();
			strData1 = m_pPrintDM->GetCellData(_T("IBS_������"), nRow);
			strData2 = m_pPrintDM->GetCellData(_T("IBS_�����"), nRow);
			if( strData2.IsEmpty() == TRUE )	strTmpData = strData1;
			else  strTmpData = strData1 + _T(", ") + strData2;
			m_pPrintDM->SetCellData(_T("UDF_�����ڻ���"), strTmpData, nRow);

			// UDF_���δܰ�
			strPrice = m_pPrintDM->GetCellData(_T("BP_����"), nRow);
			//dPrice = (DOUBLE)atof(strPrice.GetBuffer(0));
			dPrice = _tcstod(strPrice.GetBuffer(0), &pEndPtr);

			strDiscRate = m_pPrintDM->GetCellData(_T("BP_������"), nRow);
			//dDisc_Rate = (DOUBLE)atof(strDiscRate.GetBuffer(0));
			dDisc_Rate = _tcstod(strDiscRate.GetBuffer(0), &pEndPtr);

			strExchangeRate = m_pPrintDM->GetCellData(_T("BP_ȯ��"), nRow);
			//dExchangeRate = (DOUBLE)atof(strExchangeRate.GetBuffer(0));
			dExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);

			// ���δܰ� ���
			dDisc_Price = (dPrice - ((dPrice*dDisc_Rate)/100)) * dExchangeRate;
			
			// ����Ѱ���� �Ҽ��� ������.
			//20070512 pjw���� ��ȭ�ϰ�� �Ҽ�������
			if( _T("\\") ==  m_pPrintDM->GetCellData(_T("BP_ȭ�󱸺�"), nRow) )
			{
				strTmpData.Format(_T("%d"), (INT)dDisc_Price);
			}
			else
			{
				strTmpData.Format(_T("%.0f"), dDisc_Price);
			}

			//strTmpData.Format(_T("%.0f"), dDisc_Price);

			// ������� 1000�������� �޸��� �����Ͽ� UDF_���δܰ��� �����Ѵ�.
			m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, strTmpData);
			m_pPrintDM->SetCellData(_T("UDF_���δܰ�"), strTmpData, nRow);
		}

		m_nRow = nRow;

		if( m_nRow == m_pPrintDM->GetRowCount()-1 )		m_nRow = -1;
	}



	//=======================================================================
	// 1. UDF_�������� �� UDF_������������� �����Ѵ�.
	//=======================================================================
	if( strDMFieldAlias == _T("UDF_��������") )
	{	
		strAcqYear = m_pPrintDM->GetCellData(_T("BP_���Գ⵵"), nRow);
		strWorkCode = m_pPrintDM->GetCellData(_T("IBS_��������"), nRow);
		strGroupNo = m_pPrintDM->GetCellData(_T("BP_�׷��ȣ"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		strWorkNo = m_pPrintDM->GetCellData(_T("BP_�۾���ȣ"), nRow);
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		strSerialNo = m_pPrintDM->GetCellData(_T("BP_�Ϸù�ȣ"), nRow);
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));

		// ���������� �����Ѵ�.
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);

		// ������������� �����Ѵ�.
		strGroupNo.Format(_T("%04d-%03d-%05d"), nGroupNo, nWorkNo, nSerialNo);
		m_pPrintDM->SetCellData(_T("UDF_�����������"), strGroupNo, nRow);
	}
	//=======================================================================
	// 2. UDF_�Է±��м����� �����Ѵ�.
	//=======================================================================
	else if( strDMFieldAlias == _T("UDF_�Է±��м���") )
	{
		strTmpData.Empty();
		strTmpData = m_pPrintDM->GetCellData(_T("BP_�Է±���"), nRow);

		if		( strTmpData == _T("0") )	str = _T("ȭ���Է�");
		else if	( strTmpData == _T("1") )	str = _T("��Ȱ�����");
		else if ( strTmpData == _T("2") )	str = _T("����ڷ����");
		else if	( strTmpData == _T("3") )	str = _T("���Ϲ���");
		else								str = _T("�������");

		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	//=======================================================================
	// 3. UDF_�������м����� �����Ѵ�.
	//=======================================================================
	else if(strDMFieldAlias == _T("BP_��������"))
	{ 
		strTmpData.Empty();
		strTmpData = m_pPrintDM->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		
		if		( strTmpData == _T("1") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC��������ȣ����"), str, str);
		}
		else if	( strTmpData == _T("2") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC��������ȣ����"), str, str);
		}
		else if ( strTmpData == _T("3") || strTmpData == _T("7") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("LC������ȣ"), str, str);
		}
		else /*if( strTmpData.IsEmpty() == TRUE )*/ 
		{
			str = _T("");
		}

		m_pPrintDM->SetCellData(_T("UDF_�������м���"), str, nRow);
	}
	//=======================================================================
	// 4. UDF_����ó��ȣ������ �����Ѵ�.
	//=======================================================================
	else if( strDMFieldAlias == _T("UDF_����ó��ȣ����") )
	{
		strTmpData.Empty();
		strTmpData = m_pPrintDM->GetCellData(_T("BP_����ó��ȣ"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����ó����"), strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	//=======================================================================
	// UDF_�ܰ��� �����Ѵ�.
	//=======================================================================
	else if(strDMFieldAlias == _T("UDF_�ܰ�"))
	{
		str.Replace(_T(","), _T(""));
		str.TrimLeft();		str.TrimRight();
		//dPrice = (DOUBLE)atof(str);
		dPrice = _tcstod(str.GetBuffer(0), &pEndPtr);

		if( _T("\\") ==  m_pPrintDM->GetCellData(_T("BP_ȭ�󱸺�"), nRow) )
		{
			str.Format(_T("%d"), (INT)dPrice);
		}
		else	str = TruncPrice(m_pPrintDM, dPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	//=======================================================================
	// UDF_��ȯ�������� �����Ѵ�.
	//=======================================================================
	else if( strDMFieldAlias == _T("UDF_������") )
	{
		//20070829 ADD PJW : �������� ���濡 ���� �������� �������� ���̺��� �ٷ� �޾ƿ´�.
		//BP_�ѱǼ� 
		CString strSpeciesPrice;
		CString strBookCnt;
		CString strVolCnt;
		DOUBLE dSpeciesPrice;
		INT iBookCnt = 1;
		INT iVolCnt;

		strSpeciesPrice = m_pPrintDM->GetCellData(_T("BP_����"), nRow);
		//dSpeciesPrice = atof(strSpeciesPrice);
		dSpeciesPrice = _tcstod(strSpeciesPrice.GetBuffer(0), &pEndPtr);

		strVolCnt = m_pPrintDM->GetCellData(_T("BP_�ѱǼ�"), nRow);
		iVolCnt = _ttoi( strVolCnt );
		if( iVolCnt == 1 )
		{
			strBookCnt = m_pPrintDM->GetCellData(_T("UDF_å��"), nRow);
			iBookCnt = _ttoi(strBookCnt);
			dSpeciesPrice*=iBookCnt;
			strSpeciesPrice.Format(_T("%.2f"), dSpeciesPrice);
			str = strSpeciesPrice;
			//=====================================================
			//2008.12.12 REM BY PJW : å�� �ѱ��϶� �̿ܿ��� UDF_������ ���� �Ѵ�.
// 			m_pPrintDM->SetCellData(_T("UDF_������"), strSpeciesPrice, nRow);
			//=====================================================
		}
	
		//

		str.TrimLeft();		str.TrimRight();
		// 2005-01-26
		//dPrice = (DOUBLE)atof(str.GetBuffer(0));
		dPrice = _tcstod(str.GetBuffer(0), &pEndPtr);

		//20070512 pjw���� ��ȭ�ϰ�� �Ҽ�������
		
		if( _T("\\") ==  m_pPrintDM->GetCellData(_T("BP_ȭ�󱸺�"), nRow) )
		{
			str.Format(_T("%d"), (INT)dPrice);
		}
		else
		{
			str = TruncPrice(m_pPrintDM, dPrice);
		}
		
		//str = TruncPrice(m_pPrintDM, dPrice);
		
		//=====================================================
		//2008.12.12 ADD BY PJW : �������� �����ݿ� ���� �߸��� �־ ������ 
// 		m_pPrintDM->SetCellData(_T("UDF_������"), strSpeciesPrice, nRow);
		m_pPrintDM->SetCellData(_T("UDF_������"), str, nRow);
		//=====================================================

		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(_T("UDF_��ȯ������"), str, nRow);
	}
	//=======================================================================
	// 5. UDF_���������ݰ� UDF_��ȯ������������ �����Ѵ�.
	//=======================================================================
	else if(strDMFieldAlias == _T("UDF_����������"))
	{
		//20070829 ADD PJW : �������� ���濡 ���� ���ְ����� �������� ���̺��� �ٷ� �޾ƿ´�.
		/*
		CString strSpeciesPrice;
		CString strPriceDiscountRate;
		CString strExchangeRate;
		CString strBookCnt;
		DOUBLE dSpeciesPrice;
		DOUBLE dExchangeRate;
		DOUBLE dPriceDiscountRate;
		DOUBLE dOrderPRICE;
		INT iBookCnt;
		
		strSpeciesPrice = m_pPrintDM->GetCellData(_T("BP_����"), nRow);
		strPriceDiscountRate = m_pPrintDM->GetCellData(_T("BP_������"), nRow);
		strExchangeRate = m_pPrintDM->GetCellData(_T("BP_ȯ��"), nRow);
		strBookCnt = m_pPrintDM->GetCellData(_T("UDF_å��"), nRow);
		dSpeciesPrice = atof(strSpeciesPrice);
		dExchangeRate = atof(strExchangeRate);
		dPriceDiscountRate = atof(strPriceDiscountRate);
		iBookCnt = _ttoi(strBookCnt);
		dOrderPRICE = dSpeciesPrice * dExchangeRate * ( ( 100.0-dPriceDiscountRate )/100.0 ) * iBookCnt;
		strTmpData.Format(_T("%.2f"), dOrderPRICE);
		m_pPrintDM->SetCellData(_T("BP_���ְ���"), strTmpData, nRow);
		*/

		
		//20070831 ADD PJW : �������� ���濡 ���� ���԰����� �����Ѵ�..
		CString strSpeciesPrice;
		CString strPriceDiscountRate;
		CString strExchangeRate;
		CString strBookCnt;
		DOUBLE dSpeciesPrice;
		DOUBLE dExchangeRate;
		DOUBLE dPriceDiscountRate;
		DOUBLE dOrderPRICE;
		INT iBookCnt;
		strSpeciesPrice = m_pPrintDM->GetCellData(_T("UDF_������"), nRow);
		strPriceDiscountRate = m_pPrintDM->GetCellData(_T("BP_������"), nRow);
		strExchangeRate = m_pPrintDM->GetCellData(_T("BP_ȯ��"), nRow);
		//dSpeciesPrice = atof(strSpeciesPrice);
		dSpeciesPrice = _tcstod(strSpeciesPrice.GetBuffer(0), &pEndPtr);

		//dExchangeRate = atof(strExchangeRate);
		dExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);

		//dPriceDiscountRate = atof(strPriceDiscountRate);
		dPriceDiscountRate = _tcstod(strPriceDiscountRate.GetBuffer(0), &pEndPtr);

		dOrderPRICE = dSpeciesPrice * dExchangeRate * ( ( 100.0-dPriceDiscountRate )/100.0 );
		strTmpData.Format(_T("%.2f"), dOrderPRICE);
		m_pPrintDM->SetCellData(_T("BP_���ְ���"), strTmpData, nRow);

		//20070829 DELETE PJW : ���ο��ڵ忡 ���缭 ���� �ڵ� ����
		//strTmpData = m_pPrintDM->GetCellData(_T("BP_���ְ���"), nRow);
		
		//
		//dPrice = (DOUBLE)atof(strTmpData);
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);


		//20070512 pjw���� ��ȭ�ϰ�� �Ҽ�������
		if( _T("\\") ==  m_pPrintDM->GetCellData(_T("BP_ȭ�󱸺�"), nRow) )
		{
			strTmpData.Format(_T("%d"), (INT)dPrice);
		}
		else
		{
			if( dPrice == 0 )	strTmpData = _T("0");
			else
			{
				if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
				else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
				else										strTmpData.Format(_T("%.f"), dPrice);
			}
		}
		
		/*
		if( dPrice == 0 )	strTmpData = _T("0");
		else
		{
			if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
			else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
			else										strTmpData.Format(_T("%.f"), dPrice);
		}
		*/
		
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pPrintDM->SetCellData(_T("UDF_����������"), strTmpData, nRow);
		m_pPrintDM->SetCellData(_T("UDF_��ȯ����������"), str, nRow);
	}
	//=======================================================================
	// ������ϱ��� ����, ��ó��, �μ����� �����Ѵ�.
	// 2005.11.02 ������ �߰�
	//=======================================================================
	//  �����+++
	else if( strDMFieldAlias == _T("UDF_������ϱ��м���") )
	{
		strTmpData.Empty();		str.Empty();
		strTmpData = m_pPrintDM->GetCellData(_T("BS_������ϱ���"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("������ϱ���"), strTmpData, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if( strDMFieldAlias == _T("UDF_��ó��") )
	{
		if( m_pPrintDM->GetCellData(_T("BP_��û����"), nRow) == _T("DEP") )
		{
			strTmpData.Empty();		str.Empty();
			strTmpData = m_pPrintDM->GetCellData(_T("BP_��ó��"), nRow);
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�к�"), strTmpData, str);
			m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
		}
		else	m_pPrintDM->SetCellData(strDMFieldAlias, _T(""), nRow);
	}
	// �����---
	// ������+++
	else if( strDMFieldAlias == _T("UDF_�μ���") )
	{
		if( m_pPrintDM->GetCellData(_T("BP_��û����"), nRow) == _T("CLA") )
		{
			strTmpData.Empty();		str.Empty();
			strTmpData = m_pPrintDM->GetCellData(_T("BP_�μ��ڵ�"), nRow);
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�����μ�"), strTmpData, str);
			m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
		}
		else	m_pPrintDM->SetCellData(strDMFieldAlias, _T(""), nRow);
	}
	// ������---
	//=======================================================================
	//2008.12.04 ADD BY LKS : ��ϱ��� ������ " " ���� "/"�� ��ü (ex. EM JU  -> EM/JU)
	else if( strDMFieldAlias == _T("UDF_����ϱ���") )
	{
		m_pPrintDM->GetCellData(_T("IBS_���ε�ϱ���"),nRow,str);
		str.TrimLeft();str.TrimRight();
		str.Replace(_T(" "),_T("/"));
		m_pPrintDM->SetCellData(_T("UDF_����ϱ���"),str,nRow);
	}
	//=====================================================
	
			
	return 0;
}

VOID CBO_ACQ_PurchaseDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if( nCurSel < 0 )							bSetTopRow = FALSE;
	if( m_pPrintDM->GetRowCount() == 0 )		bSetTopRow = FALSE;
	if( nCurSel > m_pPrintDM->GetRowCount() )	bSetTopRow = FALSE;

	m_pGrid->Display();
	if( bSetTopRow == TRUE )		m_pGrid->SetTopRow(nCurSel);
}

VOID CBO_ACQ_PurchaseDataMgr::SetPurchaseDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d�� %då�� ���� ��� �ڷᰡ �˻��Ǿ����ϴ�."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), strMsg);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//
// [ �̴Ͻ��� ]
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PurchaseDataMgr::ShowDataPurchaseImport()
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT nRowCnt, nCurCnt, ids;
	// �ڷ���� Ŭ����
	CBO_ACQ_ImportMgr importDlg(this);



	nRowCnt = m_pPrintDM->GetRowCount();

	importDlg.SetAcqType(PURCHASE);

	//=======================================================================
	// 1. ������ȣ�� ��������� �����Ѵ�.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("�׷��ȣ"), m_strDivNo);

	//=======================================================================
	// 2. �׷������� �˻� Ŭ�������� ������ ������ ���������� Ȯ���Ѵ�.
	// 3. ���������� ���ٸ� ������ȣ����Ʈ â�� ����.
	// 4. ����Ʈ �����̶�� �������� ������� ����������� �׳� �н��Ѵ�.
	//=======================================================================
	CGroupCollection* pGroups = m_pSearchDlg->GetGroupCollection();		
	if( pGroups->GetGroupCount() == 0 )
	{
		// 3.
		if( IDYES == ESLAfxMessageBox(_T("������ ���������� �����ϴ�. ������ �����Ͻðڽ��ϱ�?"), MB_YESNO) )
		{
			CDivisionNo_List_Dlg dlg(this);
			dlg.SetWorkStatus(_T("���Դ��"));
			dlg.SetAcquisitYear(pGroups->GetAcqYear());
			dlg.SetGroupCollection(pGroups);
			dlg.DoModal();
		}
		// 4.
		else if (m_pInfo->GetKIIUPClientVersion() == 'L' )
		{
			pGroups->AddGroupInfo(pGroups->GetAcqYear(), _T("�������"), pGroups->GetWorkCode());
		}
		else return 0;
	}
	// 4.
	else if( m_pInfo->GetKIIUPClientVersion() == 'L' )
	{
		pGroups->AddGroupInfo(pGroups->GetAcqYear(), _T("�������"), pGroups->GetWorkCode());
	}
	

	//=======================================================================
	// 5. �׷��� ���ٸ� ���� �Ѵ�.
	//=======================================================================
	if( pGroups->IsEmpty() == TRUE )	return 0;


	//=======================================================================
	// 6. ����Ŭ������ �׷�����/DM/Grid�� ������ �Ŀ� ȭ���� ����.
	//=======================================================================
	importDlg.SetGroups(m_pSearchDlg->GetGroupCollection());
	importDlg.SetParentDM(m_pPrintDM);
	importDlg.SetParentBookDM(m_pDM);
	importDlg.SetParentGrid(m_pGrid);
	importDlg.DoModal();


	//=======================================================================
	// 7. ����Ʈ �����̶�� ���� ����� �׷��� �����.
	//=======================================================================
	if( m_pInfo->GetKIIUPClientVersion() == 'L' )
	{
		CGroupInfo Group;
		Group.SetAcqYear(pGroups->GetAcqYear());
		Group.SetGroupInfo(_T("�������"));
		Group.SetWorkCode(pGroups->GetWorkCode());
		ids = pGroups->DeleteThisGroupInfo(&Group);
		if( ids < 0 )	return -1;
	}
	
	//=======================================================================
	// 8. ���Ե� �ڷῡ ���� ���� �� å���� ���� ȭ�鿡 ���� �Ѵ�.
	//=======================================================================
	SetPurchaseDataInfo();

	//=======================================================================
	// 9. ���Ե� �ڷᰡ �ִٸ� ���������� _T("Y")�� �����ϰ� �׸��带 �ٽ� �׸���.
	//=======================================================================
	nCurCnt = m_pPrintDM->GetRowCount();	
	if( nCurCnt > nRowCnt )
	{
		// ���Ե� å ����ó�� _T("Y") ����
		// 2005-11-14 ������ �߰�
		//			
		for(INT i=nRowCnt ; i<nCurCnt ; i++)
		{
				m_pPrintDM->SetCellData(_T("BP_��������"), _T("Y"), i );
		}
		AllControlDisplay(_T("CM_�����ڷ����"));			
		m_pGrid->DisplayNumber();
		m_pGrid->SetTopRow(nRowCnt+1);
	}
	return 0;
}

VOID CBO_ACQ_PurchaseDataMgr::SetCheckOrderInvalidData(CArray <INT, INT> * pArrayInvalidData)
{
	INT cnt = pArrayInvalidData->GetSize();
	if(cnt == 0) return;
	for(INT i=0 ; i<cnt ; i++)
	{
		SetGridRowBackColor(m_pGrid, pArrayInvalidData->GetAt(i));
	}
}

INT CBO_ACQ_PurchaseDataMgr::SetGridRowBackColor(CESL_Grid *pGrid, INT nRow)
{
	if (pGrid == NULL) return -1;

	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(red);
			pGrid->SetCellForeColor(black);
		}
		pGrid->SetTextMatrix(nRow+1, 1, _T("V"));
	}
	
	pGrid->SetCol(nCol);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - DM�� ���� �ִ��� üũ�ϰ�, �׸��忡 ������ �ڷḦ üũ�Ͽ�
//	���� �����ִ� Row�� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ��¿� DM�� ���� �ִ��� Ȯ���Ѵ�.
// 2. �׸��忡 ���� üũ�� �����Ѵ�.
// 3. �׸��忡 ���� üũ�� Row�� ���� ���� �ִ� ���� �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PurchaseDataMgr::CheckSelectDataValid()
{
	INT ids = 0;

	//=======================================================================
	// 1. ��¿� DM�� ���� �ִ��� Ȯ���Ѵ�.
	//=======================================================================
	if(m_pPrintDM->GetRowCount() == 0) return -1;


	//=======================================================================
	// 2. �׸��忡 ���� üũ�� �����Ѵ�.
	//=======================================================================
	ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;


	//=======================================================================
	// 3. �׸��忡 ���� üũ�� Row�� ���� ���� �ִ� ���� �����Ѵ�.
	//=======================================================================
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

UINT CBO_ACQ_PurchaseDataMgr::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��¹��� ���� DM�� �����ϵ��� �ش��Լ��� ȣ���Ѵ�.
// - PrintPurchaseData() : �Ϲݸ���Ʈ ���
// - PrintPurchaseSubTotalBookData() : å�Ұ����
// - PrintPurchaseSubTotalData() : �Ұ����
//
// [ �̴Ͻ��� ] - ����
/////////////////////////////////////////////////////////////////////////////
UINT CBO_ACQ_PurchaseDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	// 2005-01-29 5163�� �߰�.
	switch( lParam )
	{
	// ���Դ���ڷ���, ���Դ���ڷ���(��û�μ�����)
	case  BO_ACQ_PURCHASE_DATA:
	case BO_ACQ_PURCHASE_DATA_5163:		return PrintPurchaseData(lParam);
		break;
	// ���Դ���ڷ���(��ϱ��к�)
	case BO_ACQ_PURCHASE_DATA_REGCODE:	return PrintPurchaseSubTotalBookData(lParam);
		break;	
	default:							return PrintPurchaseSubTotalData(lParam, wParam);
		break;
	}
	
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//
// [ �̴Ͻ��� ]
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PurchaseDataMgr::PrintPurchaseSubTotalData(INT nPrintType, INT nExtraInfo)
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT ids = -1;
	BOOL bExtraInfo = FALSE;
	CString strDivAlias = _T("BP_��������KEY");
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	CMakePrintDMMgr PrintMgr(this);
	
	
	//=======================================================================
	// 1. ���ڸ� ���Ͽ� KOALS2������ ��¹����� �Ǵ��Ѵ�.
	//=======================================================================
	if(nExtraInfo == 1000) bExtraInfo = TRUE;
	
	//=======================================================================
	// 2. _T("�׸��弱���ڷ�") ����̶�� �׸��忡 ����üũ�� �ڷᰡ �ִ��� ���� Ȯ���Ѵ�.
	//=======================================================================
	if( m_pPrintDlg->GetPrintDataType() == 2 )
	{
		ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return -1;
		}
	}

	//=======================================================================
	// 3. Progress Ctrl â�� �����Ѵ�.
	//=======================================================================
	ShowDataProcessingDlg();
	
	//=======================================================================
	// 4. CMakePrintDMMgr Ŭ������ ������ �����Ѵ�.
	//=======================================================================
	PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
	PrintMgr.SetTempData(_T("����ƮŸ��"), nPrintType);
	PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
	PrintMgr.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
	PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_����"));
/*	]
	if(nPrintType == BO_ACQ_ORDER_DATA_MEDIACODE)   //2005-01-31
		PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("BP_���ְ���"));
	else
		PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_���ΰ�"));
*/
	PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_���ΰ�"));
	PrintMgr.SetTempData(_T("åDM�˸��ƽ�"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
	PrintMgr.SetTempData(_T("KOLAS2����"), bExtraInfo);
	PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
	PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
	PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
	PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
	PrintMgr.SetTempPtr(_T("�׷��÷���"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pSubTotalPrintDM);
	PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
	
	//=======================================================================
	// 5. ����Ʈ Ÿ�Կ� ���� �����ʵ带 �߰� �����Ѵ�.
	//=======================================================================
	switch( nPrintType )
	{
	case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
	case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
	case BO_ACQ_ORDER_DATA_DIVNO:
		{
			PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BP_�׷��ȣ"));
		}
		break;

	case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER :
	case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER :
	case BO_ACQ_ORDER_DATA_PROVIDER :
		{
		PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BP_����ó��ȣ"));
		}
		break;

	case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE :
	case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE :
	case BO_ACQ_ORDER_DATA_SUBJECTCODE :
		{
		PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BP_��������"));
		}
		break;

	// 2005-01-31 5163�� ��ü���к� �ֹ���� �߰�.
	case BO_ACQ_ORDER_DATA_MEDIACODE :
		{
		PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BB_��ü����"));
		}
		break;

	// 2005-10-19 ����� ��ó�� �߰�
	case BO_ACQ_PURCHASE_DATA_DEPT:
		{
			PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BP_��ó��"));
		}
		break;

	// 2005-10-19 ����� �����з��ڵ�1�� �߰�
	case BO_ACQ_PURCHASE_DATA_USERDEFINECODE1:	
		{
			PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BS_����������ڵ�1"));
		}
		break;

	default:
	{
		ESLAfxMessageBox(_T("��¹� ������ ������ �� �����ϴ�."));
		DeleteDataProcessingDlg();
		return -1;
	}
		break;
	}
	

	//=======================================================================
	// 6. CMakePrintDMMgr�� _T("�Ұ��ڷ����")�� �����Ѵ�.
	//=======================================================================
	ids = PrintMgr.SPFExecutePath(_T("�Ұ��ڷ����"));


	//=======================================================================
	// 7. ������ Progress Ctrl�� �����.
	//=======================================================================
	DeleteDataProcessingDlg();

	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	else if( ids == 1000 )		return 0;
	
	
	//=======================================================================
	// 8. DM_�ֹ��ڷ����_����� ��¿� DM���� �����ϰ� SIReport�� ����Ѵ�.
	//=======================================================================
	m_pPrintDlg->SetPrintDM(m_pSubTotalPrintDM);
	m_pPrintDlg->Print();

	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::PrintPurchaseSubTotalBookData(INT nPrintType)
{
	INT ids = -1;
	CESL_DataMgr PrintBookDM;
	PrintBookDM.InitDMPKFromFile(m_pDM->DB_MGR_ALIAS);
	PrintBookDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	PrintBookDM.SetMarcMgr(m_pInfo->pMarcMgr);

	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	CString strDivAlias = _T("BP_��������KEY");
	CMakePrintDMMgr PrintMgr(this);
	if(m_pPrintDlg->GetPrintDataType() == 2)
	{
		ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return -1;
		}
	}
	ShowDataProcessingDlg();

	PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
	PrintMgr.SetTempData(_T("����ƮŸ��"), nPrintType);
	PrintMgr.SetTempData(_T("���о˸��ƽ�"), strDivAlias.GetBuffer(0));
	PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("BB_����"));
	PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_���ΰ�"));
	
	// nPrintType�� ���� �ڷᱸ�к� ��� �߰� (64)
	// 2005-09-05 ������
	//
	switch(nPrintType)
	{
	case BO_ACQ_PURCHASE_DATA_REGCODE:	PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BB_��ϱ���"));
		break;

	default:
		{
			ESLAfxMessageBox(_T(",���� ������ �� �� �����ϴ�.\n��ϱ������� ���� �մϴ�."));
			PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BB_��ϱ���"));
		}
		break;
	}
	//
	// �̻� ��.

	PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
	PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
	PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
	PrintMgr.SetTempPtr(_T("�׷��÷���"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
	PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pDM);
	PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)&PrintBookDM);
	PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
	ids = PrintMgr.SPFExecutePath(_T("å�Ұ��ڷ����"));
	DeleteDataProcessingDlg();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	if(ids == 1000)
		return 0;
	
	m_pPrintDlg->SetPrintDM(&PrintBookDM);
	m_pPrintDlg->Print();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���Դ���ڷ���, ���Դ���ڷ���(��û�μ�����)
// [ �̴Ͻ��� ]
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PurchaseDataMgr::PrintPurchaseData(INT nPrintType)
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT ids = -1;
	CString strDivAlias;
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	CMakePrintDMMgr PrintMgr(this);

	//=======================================================================
	// 1. ���о˸��ƽ��� ��������Ű(ACQ_KEY)�� �����Ѵ�.
	//=======================================================================
	strDivAlias = _T("BP_��������KEY");

	//=======================================================================
	// 2. �׸��� �����ڷ� ����̶�� ������ �ڷḸ DM�� �����Ѵ�.
	//=======================================================================
	if( m_pPrintDlg->GetPrintDataType() == 2 )
	{
		ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return -1;
		}
	}

	//=======================================================================
	// 3. Progress Ctrl ����
	//=======================================================================
	ShowDataProcessingDlg();
	
	//=======================================================================
	// 4. ����ƮDM���� Ŭ������ �ʿ��Ѱ����� �����Ѵ�. - �Ϲݸ���Ʈ���
	//=======================================================================
	PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
	PrintMgr.SetTempData(_T("����ƮŸ��"), nPrintType);
	PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
	PrintMgr.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
	PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_������"));
	PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_����������"));
	PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
	PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
	PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
	PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
	PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pDM);
	PrintMgr.SetTempPtr(_T("�������DM"), (DWORD*)m_pPrintDM);
	PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pPrintObjDM);
	PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
	ids = PrintMgr.SPFExecutePath(_T("�Ϲݸ���Ʈ���"));

	//=======================================================================
	// 5. Progress Ctrl �Ҹ�
	//=======================================================================
	DeleteDataProcessingDlg();

	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	else if(ids == 1000)		return 0;
	
	//=======================================================================
	// 6. ������ DM�� SIReport�� ����Ѵ�.
	//=======================================================================
	m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
	m_pPrintDlg->Print();

	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::CheckGiveOrderSerialNo(CGroupCollection *pGroupCollection)
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("�׷�����"), (DWORD*)pGroupCollection);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = api.SPFExecutePath(_T("�ֹ��Ϸù�ȣȮ��"));
	return ids;
}

VOID CBO_ACQ_PurchaseDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	if(m_pInfo->GetKIIUPClientVersion() != 'L')
	{
		RemoveListArray.Add(_T("��������"));
	}
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	for(INT i=0 ; i<RemoveListArray.GetSize() ; i++)
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&RemoveListArray[i], TRUE);
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_PurchaseDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_PurchaseDataMgr)
	ON_EVENT(CBO_ACQ_PurchaseDataMgr, IDC_gridPURCHASE_DATA, -601 /* DblClick */, OnDblClickgridPURCHASEDATA, VTS_NONE)
	ON_EVENT(CBO_ACQ_PurchaseDataMgr, IDC_gridPURCHASE_DATA, -607 /* MouseUp */, OnMouseUpgridPURCHASEDATA, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_PurchaseDataMgr::OnDblClickgridPURCHASEDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return ;
	ShowPurchaseDataDlg(GUPDATE);
}

VOID CBO_ACQ_PurchaseDataMgr::DupKeySetting()
{
	CDUP_KEY_SETTING Dlg(this, _T(".\\duplicate_search_key.txt"));
	Dlg.DoModal();
}

VOID CBO_ACQ_PurchaseDataMgr::ShowOrderCancelMailMgr(INT nOpType, INT nOpenType, CGroupCollection * pGroupCollection)
{
//JOB.2019.0073 : �浹 License ����
// 	COrderMailMgr mailMgr(this);
// 	if(pGroupCollection == NULL) pGroupCollection = m_pSearchDlg->GetGroupCollection();
// 	
// 	mailMgr.SetSelectSpeciesKey(m_strSelectSpeciesKey);
// 	mailMgr.SetParentGroupCollection(pGroupCollection);
// 	mailMgr.SetOperationType(nOpType);
// 	mailMgr.SetOpenType(nOpenType);
// 	mailMgr.SetAcqYear(pGroupCollection->GetAcqYear());
// 	mailMgr.DoModal();
}

INT CBO_ACQ_PurchaseDataMgr::MakePrintDM()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0) return -1;
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("��ü�ڷ�"))
	{
		ESLAfxMessageBox(_T("���� ������ ������ �����ϴ�. �˻��� ���Ͽ� Ư�� ������ ������ �ֽʽÿ�."));
		return -1;
	}
	CESL_DataMgr * pDM = FindDM(_T("DM_�����ڷ����_���"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("��¿�DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	INT ids = api.SPFExecutePath(_T("��¿�DM����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	return 0;
}

INT CBO_ACQ_PurchaseDataMgr::MakeOrderPrintDM(CString strSortField)
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0) return 0;
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("��ü�ڷ�"))
	{
		ESLAfxMessageBox(_T("���� ������ ������ �����ϴ�. �˻��� ���Ͽ� Ư�� ������ ������ �ֽʽÿ�."));
		return -1;
	}
	CESL_DataMgr * pDM = FindDM(_T("DM_�ֹ��ڷ����_���"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("��¿�DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	api.SetTempData(_T("SORT_FIELD"), strSortField.GetBuffer(0));
	INT ids = api.SPFExecutePath(_T("�ֹ������¿�DM����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	return 0;
}

VOID CBO_ACQ_PurchaseDataMgr::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) 
	{
		m_pDataProcessingDlg = new CDataProcessingDlg(this);
	}

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

VOID CBO_ACQ_PurchaseDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

VOID CBO_ACQ_PurchaseDataMgr::GiveOrderSerialNo()
{
	//BOOL bGridSelectCol ;

	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return;
	}

	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� ���������� �����ϴ�."));
		return;
	}
	
	//����/���� üũ
	INT nIdx;
	INT ids = -1;
	INT nSelCount = 0;
	ids = m_pGrid->SelectMakeList();
	if ( ids < 0 ) return;

	nIdx = m_pGrid->SelectGetHeadPosition();
	if ( nIdx < 0 ) 
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	while ( nIdx > -1 )
	{
		if ( m_pPrintDM->GetCellData( _T("BP_��������"), nIdx ) != _T("Y") ) 
		{
			nSelCount++;
		}
		//===========================================================
		//20080625 ADD BY PJW : �׸��弱���� DM�� �ݿ��Ѵ�

		CString strChk;
		CString strSpeciesKey;
		strChk = _T("V");
		m_pPrintDM->SetCellData( _T("����"), strChk, nIdx );
		m_pPrintDM->GetCellData( _T("IBS_��KEY"), nIdx, strSpeciesKey);
		SelectGridToDmChk(m_pDM,strSpeciesKey,strChk);

		//===========================================================

		nIdx = m_pGrid->SelectGetNext();
	}

	if ( nSelCount > 0 )
	{
		CString strMessage;
		strMessage.Format( _T("���õ� �ڷ��� ����ó������ ���� �ڷᰡ %d�� �ֽ��ϴ�. \r\n�ֹ�ó���� �ϱ����� ����ó���� �����Ͽ� �ֽʽÿ�"), nSelCount );
		AfxMessageBox ( strMessage );
		return;
	}

	//�����Ϸù�ȣ �ο� ��ƾ
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempPtr(_T("SpeciesDM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	ids = api.SPFExecutePath(_T("�����Ϸù�ȣ�ο�"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�ֹ��Ϸù�ȣ�� �ο��� �� �����ϴ�."));
		return;
	}
	else if(ids > 0)
	{
		CString strMsg;
		strMsg.Format(_T("���������� �ο����� ���� [%d]���� �ڷῡ ���Ͽ��� �ֹ��Ϸù�ȣ�� �ο����� �ʾҽ��ϴ�.\n")
			_T("�ش� �ڷ�� ���������ο��� �ֹ��Ϸù�ȣ�� �ο��� �� �ֽ��ϴ�."), ids);
		ESLAfxMessageBox(strMsg);
	}
	//=============================
	//20080.07.01 REM BY PJW : �ֹ��Ϸù�ȣ �ο� �޼����� �������� �ʴ´�.
//	else
//		ESLAfxMessageBox(_T("�ֹ��Ϸù�ȣ�� �ο� �Ͽ����ϴ�."));
	//=============================

	//bGridSelectCol = m_pGrid->m_bProtectSelectColumn ; 
	m_pGrid->m_bProtectSelectColumn = TRUE;
	DisplayGrid();
	m_pGrid->m_bProtectSelectColumn = FALSE;
}

VOID CBO_ACQ_PurchaseDataMgr::ShowSortDlg(INT nType/*=1*/)
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

UINT CBO_ACQ_PurchaseDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

BOOL CBO_ACQ_PurchaseDataMgr::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_���Գ⵵"));
	pTmpData->Add(_T("IBS_��������"));
	pTmpData->Add(_T("BP_�׷��ȣ"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("BP_�۾���ȣ"));
	pTmpData->Add(_T("BP_�Ϸù�ȣ"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_�������м���"), _T("BP_��������"));
	pMap->SetAt(_T("UDF_�Է±��м���"), _T("BP_�Է±���"));
	pMap->SetAt(_T("UDF_����ó��ȣ����"), _T("BP_����ó��ȣ"));
	pMap->SetAt(_T("UDF_������"), _T("UDF_��ȯ������"));
	
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

VOID CBO_ACQ_PurchaseDataMgr::ChangeGroupInfo()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CDivisionNo_Select_Dlg dlg(this);
	dlg.SetWorkStatus(_T("���Դ��"));
	if(m_pInfo->GetKIIUPClientVersion() == 'L') dlg.SetAllowNULLGroup(TRUE);
	if(IDCANCEL == dlg.DoModal()) return ;
	
	CGroupInfo * pTargetGroup = dlg.GetSelGroup();

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GROUP"), (DWORD*)pTargetGroup);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_PURCHASE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	ids = api.SPFExecutePath(_T("�׷���������"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("���������� ������ �� �����ϴ�."));
		return ;
	}
	ESLAfxMessageBox(_T("�����Ͻ� �ڷῡ ���Ͽ� ���������� �����Ͽ����ϴ�."));
	
	if(pTargetGroup && !m_pSearchDlg->GetGroupCollection()->IsHaveGroupInfo(pTargetGroup))
		m_pSearchDlg->GetGroupCollection()->AddGroupInfo(pTargetGroup);
	SetPurchaseDataInfo();
	m_pGrid->SetProtectSelectColumn(TRUE);
	DisplayGrid();
	m_pGrid->SetProtectSelectColumn(FALSE);
}

CString CBO_ACQ_PurchaseDataMgr::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	// 2004.12.01 ������å�� ���� ��������������� �о�ͼ� ������ �ڸ�����ŭ ǥ���ϵ��� �Ѵ�.
	INT nPos = -1;
	CString strTmpData, strTmp, strValue;

	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	// 2005-01-13
	if		( m_strPosNumber == _T("1") )  	strTmpData = strTmp.Left(nPos+2);
	else if	( m_strPosNumber == _T("2") )	strTmpData = strTmp.Left(nPos+3);
	else									strTmpData = strTmp.Left(nPos);		// m_strPosNumber == _T("0") || m_strPosNumber.Empty()

	return strTmpData;
}

VOID CBO_ACQ_PurchaseDataMgr::ShowFileExport()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	CFileDialog dlg(FALSE, _T("*.*"), _T("����.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("��������(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return ;

	_tchdir(szCurrentDir);

	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return ;
    sFileName = dlg.GetPathName();	

	FILE *fp;
	fp = _tfopen( sFileName , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox(_T("������ �����ϼ���."));
		return ;
	}

		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
		
	CString sMarc;
	CMarc marc;
	INT idx;
	idx = m_pGrid->SelectGetHeadPosition();
	while ( idx >= 0 ) {
		sMarc = m_pPrintDM->GetCellData( _T("BS_MARC"), idx );

		m_pInfo->pMarcMgr->Decoding( sMarc, &marc );
		m_pInfo->pMarcMgr->Encoding( &marc, sMarc, TRUE );

		sMarc += _T("\r\n");
		_ftprintf( fp, sMarc );

		idx = m_pGrid->SelectGetNext();
	}

	fclose( fp );

	INT nTotCount = m_pGrid->SelectGetCount();
	CString sMessage;
	sMessage.Format( _T("������ %d�ǿ� ���� �����ۼ��� �Ϸ�Ǿ����ϴ�."), nTotCount );
	AfxMessageBox( sMessage );

}

VOID CBO_ACQ_PurchaseDataMgr::ShowFileImport()
{
	INT nNextRowCnt;

	INT nRowCnt = m_pPrintDM->GetRowCount();
	CFileImportDlg dlg(this);
	dlg.SetBeginImportStatus(_T("��������"), _T("BOA111N"));
	dlg.SetOpenMode(4000);
	dlg.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	//========================================================
	//2008.07.15 ADD BY PJW : ���Խ� �߰��� ������  GroupInfo�� �߰��Ѵ�
	//2008.07.25 UPDATE BY PJW : ������ ������ �߰� ���Եɰ��� ACQNO�� �߰����� �ʴ´�.
	nNextRowCnt = m_pPrintDM->GetRowCount();
	if( nRowCnt < nNextRowCnt )
	{
		INT nIdx;
		
		CString strImportAcqNo;
		CString strImportAcqYear;	

		CString strWhere;
		CString strSpeciesKey;
		CString strSpeciesKeys;
		CString strAcqNo;
		INT nCount;
		INT nPos;
		
		strImportAcqNo = dlg.m_strExtGruopInfo;
		strImportAcqYear = dlg.m_strExtAcqYear;

		CEdit* pEdit = (CEdit*) GetDlgItem(IDC_edtPURCHASE_DIVNO);
		pEdit->GetWindowText(strAcqNo);
		nPos = strAcqNo.Find(strImportAcqNo);
		if( nPos < 0 )
		{
			m_pSearchDlg->GetGroupCollection()->AddGroupInfo(strImportAcqYear, strImportAcqNo, _T("1"));
		}
	}
	//========================================================

	SetPurchaseDataInfo();
	INT nCurCnt = m_pPrintDM->GetRowCount();
	if(nRowCnt < nCurCnt)
	{
		//========================================================
		//2008.07.15 ADD BY PJW : ���Թ����� �ֹ���ȣ�ο� �����ǵ��� ����
		for(INT i = nRowCnt ; i<nCurCnt ; i++)
		{
		m_pPrintDM->SetCellData(_T("BP_��������"), _T("Y"), i );
		}
		AllControlDisplay(_T("CM_�����ڷ����"));
		//========================================================
		m_pGrid->DisplayNumber();
		m_pGrid->SetTopRow(nRowCnt + 1);
	}
}

VOID CBO_ACQ_PurchaseDataMgr::ShowACQFileExport()
{
	INT ids;
	CString strAcqYear, strGroupInfo, strWorkCode, strQuery, strResult;
	CDataExportDlg Dlg(this);
	ids = Dlg.DoModal();
	if( ids == IDCANCEL ) return ;

	strAcqYear = Dlg.GetAcqYear();
	strGroupInfo = Dlg.GetSeqNo();
	strWorkCode = Dlg.GetWorkCode();

	if(strAcqYear.IsEmpty() || strGroupInfo.IsEmpty() || strWorkCode.IsEmpty())
	{
		AfxMessageBox(_T("���� ������ ��� �Է��ϼž� �մϴ�."));
		return ;
	}
	
	strQuery.Format(_T("SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL ")
								_T("WHERE ACQ_YEAR = %s AND SEQ_NO = %s AND WORK_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),
								strAcqYear, strGroupInfo, strWorkCode);
	strResult.Empty();
	ids = m_pDM->GetOneValueQuery(strQuery, strResult);

	if( ids <0 || strResult.IsEmpty()) 
	{
		AfxMessageBox(_T("�Է����ǿ� �´� ������ �������� �ʽ��ϴ�."));
		return ;
	}

	CBO_ACQ_FileExportMgr ExportMgr;
	
	ExportMgr.SetAcqYear(strAcqYear);
	ExportMgr.SetGroupInfo(strGroupInfo);
	ExportMgr.SetWorkCode(strWorkCode);
	ExportMgr.SetSpeciesType(BO_ORDER);		//��Ÿ��
	
	// ������ �����Ѵ�.
	ids = ExportMgr.FileExportProc();

	if(ids == 0)
		AfxMessageBox(_T("���� ������ �Ϸ��Ͽ����ϴ�."));

}

VOID CBO_ACQ_PurchaseDataMgr::ShowACQFileImport()
{
	CBO_ACQ_FileImportMgr  ImportMgr(this);

	ImportMgr.SetSpeciesType(BO_ORDER);		//��Ÿ��
	// ������ �����Ѵ�.
	INT ids = ImportMgr.FileImportProc();
	if(ids < 0) return ;

	if(ids ==0)
		AfxMessageBox(_T("���� ������ �Ϸ��Ͽ����ϴ�."));	
	else return ;
}


VOID CBO_ACQ_PurchaseDataMgr::ProcSelect()
{
	//����ó��
	INT nIdx;
	INT ids = -1;
	BOOL bGridSelectCol ;
	if ( AfxMessageBox (_T("���õ� �ڷῡ ���� ����ó���� �Ͻðڽ��ϱ�?") , MB_YESNO ) ==  IDYES )
	{
		ids = m_pGrid->SelectMakeList();
		if ( ids < 0 ) return;

		nIdx = m_pGrid->SelectGetHeadPosition();
		while ( nIdx > -1 )
		{
			CString strPReckey;
			CString strQuery;
			strPReckey = m_pPrintDM->GetCellData ( _T("BP_��������KEY") , nIdx );
			if ( ids < 0 ) return;

			ids = m_pPrintDM->SetCellData ( _T("BP_��������"), _T("Y"), nIdx ) ;
			if ( ids < 0 ) return;

			strQuery.Format ( _T("UPDATE BO_PURCHASEINFO_TBL SET SEL_YN = 'Y' WHERE REC_KEY = '%s'; ") , strPReckey );
			ids = m_pPrintDM->ExecuteQuery( strQuery, 1 );
			if ( ids < 0 ) return;
		
			nIdx = m_pGrid->SelectGetNext();
		}
	}
	else
		AfxMessageBox ( _T("��ҵǾ����ϴ�."));

	bGridSelectCol = m_pGrid->m_bProtectSelectColumn ; 
	m_pGrid->m_bProtectSelectColumn = TRUE;
	AllControlDisplay(_T("CM_�����ڷ����"));
	m_pGrid->m_bProtectSelectColumn = bGridSelectCol;
}

VOID CBO_ACQ_PurchaseDataMgr::ProcUnSelect()
{
	//����ó��
	INT nIdx;
	INT ids = -1;
	BOOL bGridSelectCol;
	if ( AfxMessageBox (_T("���õ� �ڷῡ ���� ����ó���� �Ͻðڽ��ϱ�?") , MB_YESNO ) ==  IDYES )
	{
		ids = m_pGrid->SelectMakeList();
		if ( ids < 0 ) return;

		nIdx = m_pGrid->SelectGetHeadPosition();
		while ( nIdx > -1 )
		{
			CString strPReckey;
			CString strQuery;
			strPReckey = m_pPrintDM->GetCellData ( _T("BP_��������KEY") , nIdx );
			if ( ids < 0 ) return;

			ids = m_pPrintDM->SetCellData ( _T("BP_��������"), _T("N"), nIdx ) ;
			if ( ids < 0 ) return;

			strQuery.Format ( _T("UPDATE BO_PURCHASEINFO_TBL SET SEL_YN = 'N' WHERE REC_KEY = '%s'; ") , strPReckey );
			ids = m_pPrintDM->ExecuteQuery( strQuery, 1 );
			if ( ids < 0 ) return;
		
			nIdx = m_pGrid->SelectGetNext();
		}
	}
	else
		AfxMessageBox ( _T("��ҵǾ����ϴ�."));
	
	bGridSelectCol = m_pGrid->m_bProtectSelectColumn ; 
	m_pGrid->m_bProtectSelectColumn = TRUE;
	AllControlDisplay(_T("CM_�����ڷ����"));
	m_pGrid->m_bProtectSelectColumn = bGridSelectCol;
}


VOID CBO_ACQ_PurchaseDataMgr::ChangeMarc()
{
	//===========================================
	//2008.07.16 ADD BY PJW : �������ڷᰡ ������� ��ũ��ü ���̾�α׸� ����� �ʴ´�.
	INT nSelectCnt;
	m_pGrid->SelectMakeList();
	nSelectCnt = m_pGrid->SelectGetCount();
	if( 1 > nSelectCnt ) 
	{
		AfxMessageBox(_T("��ũ��ü �� �ڷḦ ������ �ּ���."));
		return;
	}
	
	//===========================================
	CMARC_CHANGER_01 dlg(this);
	//===============================================
	//2008.07.02 UPDATE BY PJW : ��ǥå�� �����͵� �����Ѵ�.
	//dlg.SetParentMgr(_T("IBS_��KEY"), _T("CM_�����ڷ����"), _T("MainGrid"), _T("DM_�����ڷ����_��"), _T("BS_MARC"));
	dlg.SetParentMgr(_T("IBS_��KEY"), _T("CM_�����ڷ����"), _T("MainGrid"), _T("DM_�����ڷ����_��"), _T("BS_MARC"), _T("DM_�����ڷ����"));
	//===============================================
	
	//===============================================
	//2008.08.22 UPDATE BY PJW : �������������ϰ� �׸��带 ���÷����Ѵ�.
// 	if ( dlg.DoModal() == IDOK )
//	{
//		AllControlDisplay(_T("CM_�����ڷ����"));
//		m_pDM->RefreshPositionInfo();		
//	}
	m_pGrid->SetProtectSelectColumn(TRUE);
	if ( dlg.DoModal() == IDOK )
	{
		AllControlDisplay(_T("CM_�����ڷ����"));
		m_pDM->RefreshPositionInfo();		
	}
	m_pGrid->Display();
	m_pGrid->SetProtectSelectColumn(FALSE);
	//===============================================
}

HBRUSH CBO_ACQ_PurchaseDataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
//===================================
//2008.07.02 ADD BY PJW : ���õ��ڷ��� ��Ű�� ��´�.
//CString CBO_ACQ_PurchaseDataMgr::GetSelectSpeciesKey()
//{
//	
//	INT nSelect;
//	CString strSpeciesDmKey;
//	CString strTemp;
//	CString strSelectSpeciesKey;
//	INT nSelectCnt;
//	INT nDmCnt;
//	m_pGrid->SelectMakeList();
//	nSelectCnt = m_pGrid->SelectGetCount();
//	nSelect = m_pGrid->SelectGetHeadPosition();
//	strSelectSpeciesKey = _T("");
//	for( INT i = 0 ; i < nSelectCnt ; i++ )
//	{
//		m_pPrintDM->GetCellData(_T("IBS_��KEY"),nSelect,strSpeciesDmKey);
//		
//		if( strSelectSpeciesKey.IsEmpty() )
//		{
//			strSelectSpeciesKey.Format(_T("%s"),strSpeciesDmKey);
//		}
//		else
//		{
//			strTemp = strSelectSpeciesKey;
//			strSelectSpeciesKey.Format(_T("%s,%s"), strTemp, strSpeciesDmKey);
//		}
//		nSelect = m_pGrid->SelectGetNext();
//	}
//	return strSelectBookKey; 
//}
//===================================
//===================================
//20080603 ADD BY PJW : ���õ��ڷ��� åŰ,��Ű�� ��´�.
CString CBO_ACQ_PurchaseDataMgr::GetSelectBookKey()
{
	
	INT nSelect;
	CString strSpeciesDmKey;
	CString strBookDmKey;
	CString strTemp;
	CString strSelectBookKey;
	CString strSelectSpeciesKey;
	INT nSelectCnt;
	INT nDmCnt;
	m_pGrid->SelectMakeList();
	nSelectCnt = m_pGrid->SelectGetCount();
	nSelect = m_pGrid->SelectGetHeadPosition();
	nDmCnt = m_pDM->GetRowCount();
	strSelectBookKey = _T("");
	strSelectSpeciesKey = _T("");
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		

		m_pPrintDM->GetCellData(_T("IBS_��KEY"),nSelect,strSpeciesDmKey);
		if( strSelectSpeciesKey.IsEmpty() )
		{
			strSelectSpeciesKey.Format(_T("%s"),strSpeciesDmKey);
		}
		else
		{
			strTemp = strSelectSpeciesKey;
			strSelectSpeciesKey.Format(_T("%s,%s"), strTemp, strSpeciesDmKey);
		}

		for( INT j = 0 ; j < nDmCnt ; j++ )
		{
			m_pDM->GetCellData(_T("IBS_��KEY"),j,strBookDmKey);
			if( strSpeciesDmKey == strBookDmKey )
			{
				m_pDM->GetCellData(_T("BB_åKEY"),j,strBookDmKey);
				if( strSelectBookKey.IsEmpty() )
				{
					strSelectBookKey.Format(_T("%s"),strBookDmKey);
				}
				else
				{
					strTemp = strSelectBookKey;
					strSelectBookKey.Format(_T("%s,%s"), strTemp, strBookDmKey);
				}
			}
		}
		
		nSelect = m_pGrid->SelectGetNext();
	}
	m_strSelectSpeciesKey = strSelectSpeciesKey;
	return strSelectBookKey; 
}

//========================================================================
//20080605 ADD BY PJW : ���콺 UP�� ���������� SelectGridToDmChk()ȣ��
void CBO_ACQ_PurchaseDataMgr::OnMouseUpgridPURCHASEDATA(short Button, short Shift, long x, long y) 
{
	INT nSelectRow, nSelectCol;
	CString strChk;
	CString strSpeciesKey;
//	CString strBookKey;
//	CMSHFlexGrid *pGrid;
	

//  pGrid		= ( CMSHFlexGrid * )GetDlgItem( IDC_gridPURCHASE_DATA );
//	nSelectRow	= pGrid->GetRow();
//	nSelectCol	= pGrid->GetCol();
//	strChk		= pGrid->GetText();
	//m_pGrid
	//pGrid		= ( CMSHFlexGrid * )GetDlgItem( IDC_gridPURCHASE_DATA );
	nSelectRow	= m_pGrid->GetRow();
	nSelectCol	= m_pGrid->GetCol();
	strChk		= m_pGrid->GetText();
	
	if(  0 >= nSelectRow )
	{
		return;
	}

	nSelectRow -= 1;
	if ( 1 == Button && 1 == nSelectCol ) 
	{
		m_pPrintDM->SetCellData( _T("����"), strChk, nSelectRow );
		m_pPrintDM->GetCellData( _T("IBS_��KEY"), nSelectRow, strSpeciesKey);
		SelectGridToDmChk(m_pDM,strSpeciesKey,strChk);
	}
}
//========================================================================

//========================================================================
//20080605 ADD BY PJW : �׸��忡 ���� DM�� ���� ���� �����Ѵ�.
INT CBO_ACQ_PurchaseDataMgr::SelectGridToDmChk(	CESL_DataMgr* pDM		, 
												 CString strSpeciesKey	, 
												 CString strChk	
												 )
{
	CString strTempKey;
	INT i;

	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("IBS_��KEY"), i, strTempKey);
		if( strSpeciesKey == strTempKey )
		{
			pDM->SetCellData( _T("����"), strChk, i );
		}
	}
	return 0;
}
//========================================================================


VOID CBO_ACQ_PurchaseDataMgr::KSearch(CString WorkType, CString strAcqCode, CString strAcqYear, CString strWorkCode, CString strSeqNo)
{	
	CString strQuery;
	
	
	BEGIN_SBL();
	strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND I.SPECIES_CLASS IN('0', '1') AND B.WORKING_STATUS = 'BOA111N' AND B.ACQ_CODE = '%s' AND P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO IN (%s)")
		, strAcqCode, strAcqYear, strWorkCode, strSeqNo );
	m_pDM->RefreshDataManager(strQuery);
	END_SBL(m_pDM->GetRowCount());

	m_pPrintDM->FreeData();
	ShowDataProcessingDlg();
	if( 0 != m_pDM->GetRowCount() )
	{
		CBO_ACQ_Business_Api mgr(this);
		mgr.InitTempData();
		mgr.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
		mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
		mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	
		INT ids = mgr.SPFExecutePath(_T("åTo��������"));
		DeleteDataProcessingDlg();
		if( ids < 0 )	return;
	
		m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
		SetPurchaseDataInfo();
		//SortByGroupInfo();	
		DisplayGrid();
	}
	DeleteDataProcessingDlg();

	m_pCM->AllControlDisplay(-1);
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), strSeqNo);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), strAcqYear);

	m_pSearchDlg->GetGroupCollection()->RemoveAll();
	m_pSearchDlg->GetGroupCollection()->SetAcqCode(strAcqCode);	
	m_pSearchDlg->GetGroupCollection()->AddGroupInfo(strAcqYear, strSeqNo, strWorkCode);	
	m_pSearchDlg->GetGroupCollection()->SetHaveGroupInfo(TRUE);
	
	//m_pSearchDlg->GetGroupCollection()->m_bAllGroupInfo = FALSE;

	//m_pPrintDM
	
}