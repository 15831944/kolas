// BO_ACQ_DonateDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_NonDonateDataMgr.h"
#include "../../../����/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
#include "../../../����/BO_DATA_CHANGE_MANAGER/DonateInfoChange.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/RequisitionNo_Mgr.h"
#include "..\..\..\����\Duplicate_Data_Search\DUP_KEY_SETTING.h"

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
// CBO_ACQ_DonateDataMgr dialog


CBO_ACQ_NonDonateDataMgr::CBO_ACQ_NonDonateDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_NonDonateDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_DonateDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pPrintDM = NULL;
	m_pPrintObjDM = NULL;
	m_pSearchDlg = NULL;
	m_strDivNo = _T("");
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_pSortItemSet = NULL;
	m_nRow = -1;
}

CBO_ACQ_NonDonateDataMgr::~CBO_ACQ_NonDonateDataMgr()
{
	if(m_pDuplicateSearchMgr)
	{
		delete m_pDuplicateSearchMgr;
		m_pDuplicateSearchMgr = NULL;
	}
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
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


VOID CBO_ACQ_NonDonateDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_DonateDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_NonDonateDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_DonateDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	ON_MESSAGE(DUPLICATE_DATA_DELETE, OnDuplicateDataDelete)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_DonateDataMgr message handlers

VOID CBO_ACQ_NonDonateDataMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

BOOL CBO_ACQ_NonDonateDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_�����δ��ڷ����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_�����δ��ڷ����"));
	m_pDM = FindDM(_T("DM_�����δ��ڷ����"));
	m_pPrintDM = FindDM(_T("DM_�����δ��ڷ����_��"));
	m_pPrintObjDM = FindDM(_T("DM_�����δ��ڷ����_��_��¿�"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("�̵���ڷ�");
	}
	
	GetDlgItem(IDC_stcNONDONATE_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtNONDONATE_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcNONDONATE_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtNONDONATE_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_NONDONATE, DONATE, BO_NONDONATE, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_NONDONATE, DONATE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	InitSortItem();
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	CString strMsg = _T("���� �̵���ڷḦ �����մϴ�.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	SetNonDonateDataInfo();
	InitPopupMenuItem();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_NonDonateDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//====================================================
	//2008.07.07 UPDATE BY PJW : SIZE ������
//	CButton *pBtn = (CButton *)GetDlgItem(IDC_stcNONDONATE_DATA);
//	if(pBtn->GetSafeHwnd() == NULL) return;
//	pBtn->MoveWindow(5, 5, cx-10, 30);
	//====================================================
	CRect rect;
	//====================================================
	//2008.07.07 UPDATE BY PJW : SIZE ������
// 	pBtn = (CButton*)GetDlgItem(IDC_stcNONDONATE_GROUP_CREATE_YEAR);
	CButton *pBtn = (CButton*)GetDlgItem(IDC_stcNONDONATE_GROUP_CREATE_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	//====================================================
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtNONDONATE_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcNONDONATE_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(165, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtNONDONATE_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(225, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridNONDONATE_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//==========================================
	//20080604 UPDATE BY PJW : ��ũ�ѹ� ������ �ϱ����켭 ���α��� ����
	//pGrid->MoveWindow(-2, 35, cx+2, cy);
	pGrid->MoveWindow(-2, 35, cx+2, cy-33);		
	//==========================================		
	
}

BOOL CBO_ACQ_NonDonateDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}	

VOID CBO_ACQ_NonDonateDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_NonDonateDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	m_pPrintDlg->PrivacyPrintLogYN = TRUE;
}

VOID CBO_ACQ_NonDonateDataMgr::ShowNonDonateDataDlg(OPERATION_TYPE nOpType)
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	CArray<INT, INT> arrDelIdx;	arrDelIdx.RemoveAll();
	CSpecies_Form dlg(this);
	dlg.SetAcqType(DONATE);
	dlg.SetSpeciesType(BO_NONDONATE);
	dlg.SetOpType(nOpType);
	if(nOpType != SINSERT)
	{
		if(0 == m_pPrintDM->GetRowCount()) 
		{
			ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
			return ;
		}
		dlg.SetDonateRegYNArray(&arrDelIdx);
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
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	
	if(nOpType != SINSERT)
	{
		INT cnt = arrDelIdx.GetSize();
		if(cnt > 0)
		{
			CBO_ACQ_Business_Api api(this);
			api.DeleteSelectRow(m_pPrintDM, &arrDelIdx, _T(""));
			SetNonDonateDataInfo();
			DisplayGrid();
			return ;
		}
	}
	SetNonDonateDataInfo();
	if(nOpType == SINSERT)
	{
		INT nCurCnt = m_pPrintDM->GetRowCount();
		if(nCurCnt > nRowCnt)
		{
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}
}

VOID CBO_ACQ_NonDonateDataMgr::DeleteNonDonateData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_NONDONATE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)DONATE);
	api.SetTempData(_T("����Ÿ��"), (INT)SPECIES_DELETE);
	api.SetTempData(_T("å���˸��ƽ�"), _T("BP_��ǰå��"));
	api.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_��ȯ����"));
	api.SetTempData(_T("��������"), _T(""));
	api.SetTempPtr(_T("��Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("��������Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("���Ϸù�ȣ����Ʈ"), NULL);
	ids = api.SPFExecutePath(_T("����"));
	if(ids == 0)
	{
		SetNonDonateDataInfo();
		DisplayGrid();	
	}
/*
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_NONDONATE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)DONATE);
	api.SetTempData(_T("��Ű�˸��ƽ�"), _T("IBS_��KEY"));
	api.SetTempData(_T("��������Ű�˸��ƽ�"), _T("BP_��������KEY"));
	api.SetTempData(_T("��������"), _T(""));
	api.SetTempPtr(_T("��Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("��������Ű����Ʈ"), NULL);
	ids = api.SPFExecutePath(_T("������"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ������ �� �����ϴ�."));
		return;
	}
	if(ids == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ �����Ͽ����ϴ�."));
		SetNonDonateDataInfo();
		DisplayGrid();
	}
*/
}

VOID CBO_ACQ_NonDonateDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CCodeChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.SetAcqType(2);
	if(IDOK == dlg.DoModal())
	{
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_NonDonateDataMgr::ShowChangeDonateInfoDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CDonateInfoChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	if(IDOK == dlg.DoModal())
	{
		//===================================================
		//2009.03.23 UPDATE BY LKS : ���/�̵�� ���� ����ÿ��� �ش� �׸��尡 �����ǹǷ� 'V'�� �����ؼ� �ȵȴ�.
		/*�����ҽ�
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);*/
		if(dlg.m_bRegYn)
		{
			DisplayGrid();
		}
		else
		{
			m_pGrid->SetProtectSelectColumn(TRUE);
			DisplayGrid();
			m_pGrid->SetProtectSelectColumn(FALSE);
		}
		//===================================================
	}
}

VOID CBO_ACQ_NonDonateDataMgr::ShowDivNoMgrDlg()
{
	CRequisitionNo_Mgr dlg(this);
	dlg.SetOpenMode(0);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.DoModal();
}

UINT CBO_ACQ_NonDonateDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
	BEGIN_SBL();
	//==============================================
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("�����̵���ڷḦ �������Դϴ�."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery,  m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		SetNonDonateDataInfo();
		DisplayGrid();
		return 0;
	}

	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	INT ids = mgr.SPFExecutePath(_T("åTo��������"));
	if(ids < 0) return ids;
	DeleteDataProcessingDlg();
	m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
	SetNonDonateDataInfo();
	//SortByGroupInfo();
	DisplayGrid();
	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
	INT nRowCount = m_pPrintDM->GetRowCount();
	END_SBL(nRowCount);
	//==============================================
	
	return 0;
}

UINT CBO_ACQ_NonDonateDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	if(lParam == BO_ACQ_NONDONATE_DATA)
	{
		CString strDivAlias = _T("BP_��������KEY");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			INT ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
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
		PrintMgr.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
		//20070817 UPDATE PJW : �Ѱ踦 ���ؼ� UDF_������ BP_�������� ��ȯ
		PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("BP_����"));
		PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T(""));
		PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pDM);
		PrintMgr.SetTempPtr(_T("�������DM"), (DWORD*)m_pPrintDM);
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pPrintObjDM);
		PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
		INT ids = PrintMgr.SPFExecutePath(_T("�Ϲݸ���Ʈ���"));
		DeleteDataProcessingDlg();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
			return -1;
		}
		m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
		m_pPrintDlg->Print();		
	}
	return 0;
}

INT CBO_ACQ_NonDonateDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData;
	INT nGroupNo;
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

	if(strDMFieldAlias == _T("UDF_������ȣ����"))
	{	
		strAcqYear = m_pPrintDM->GetCellData(_T("BP_���Գ⵵"), nRow);
		strGroupNo = m_pPrintDM->GetCellData(_T("BP_�׷��ȣ"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		str.Format(_T("%s-%04d"), strAcqYear, nGroupNo);// = strAcqYear+_T("-")+strGroupNo;
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
		strGroupNo.Format(_T("%04d"), nGroupNo);
		m_pPrintDM->SetCellData(_T("UDF_���������ȣ����"), strGroupNo, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_�Է±��м���"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("BP_�Է±���"), nRow);
		if(strTmpData == _T("0")) str = _T("ȭ���Է�");
		else if(strTmpData == _T("1")) str = _T("��Ȱ�����");
		else if(strTmpData == _T("2")) str = _T("����ڷ����");
		else if(strTmpData == _T("3")) str = _T("���Ϲ���");
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
	else if(strDMFieldAlias == _T("BP_����"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(_T("UDF_��ȯ����"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_å��"))
		m_pPrintDM->SetCellData(_T("BP_��ǰå��"), str, nRow);
	else if(strDMFieldAlias == _T("UDF_�ܰ�"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CBO_ACQ_NonDonateDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pPrintDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pPrintDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

VOID CBO_ACQ_NonDonateDataMgr::SetNonDonateDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d�� %då�� ���� �̵���ڷᰡ �˻��Ǿ����ϴ�."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), strMsg);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

INT CBO_ACQ_NonDonateDataMgr::CheckSelectDataValid()
{
	if(m_pPrintDM->GetRowCount() == 0) return -1;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_ACQ_NonDonateDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_NonDonateDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_DonateDataMgr)
	ON_EVENT(CBO_ACQ_NonDonateDataMgr, IDC_gridNONDONATE_DATA, -601 /* DblClick */, OnDblClickgridNONDONATEDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_NonDonateDataMgr::OnDblClickgridNONDONATEDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0 ) return ;
	ShowNonDonateDataDlg(GUPDATE)	;
}

VOID CBO_ACQ_NonDonateDataMgr::ShowDataProcessingDlg()
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

VOID CBO_ACQ_NonDonateDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

VOID CBO_ACQ_NonDonateDataMgr::DupPurchaseDataSearch()
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
		ESLAfxMessageBox(_T("�̹� �������簡 �������Դϴ�."));
		return ;
	}		
	m_pDuplicateSearchMgr->ShowHoldDuplicateSearch(_T("��������"), m_pPrintDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"));
}

VOID CBO_ACQ_NonDonateDataMgr::DupKeySetting()
{
	CDUP_KEY_SETTING Dlg(this, _T(".\\duplicate_search_key.txt"));
	Dlg.DoModal();
}

UINT CBO_ACQ_NonDonateDataMgr::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}

/*
INT CBO_ACQ_NonDonateDataMgr::SortByGroupInfo()
{
	if(m_pPrintDM == NULL) return 0;
	if(m_pPrintDM->GetRowCount() == 0) return 0;

	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BP_���Գ⵵");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_�Ϸù�ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
/ *
	strAlias[nCondColumns] = _T("IBS_������");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("IBS_��ǥ��");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("IBS_������");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
* /
	m_pPrintDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return 0;
}
*/

UINT CBO_ACQ_NonDonateDataMgr::OnDuplicateDataDelete(LPARAM lParam, WPARAM wParam)
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
	SetNonDonateDataInfo();
	return 0;
}

VOID CBO_ACQ_NonDonateDataMgr::ShowSortDlg(INT nType/*=1*/)
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

UINT CBO_ACQ_NonDonateDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

BOOL CBO_ACQ_NonDonateDataMgr::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_���Գ⵵"));
	pTmpData->Add(_T("BP_�׷��ȣ"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_�������м���"), _T("BP_��������"));
	pMap->SetAt(_T("UDF_�Է±��м���"), _T("BP_�Է±���"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("UDF_�Ϸù�ȣ"));
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

HBRUSH CBO_ACQ_NonDonateDataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}