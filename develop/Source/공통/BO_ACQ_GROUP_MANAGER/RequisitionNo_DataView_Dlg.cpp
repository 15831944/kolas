// RequisitionNo_DataView_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "RequisitionNo_DataView_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequisitionNo_DataView_Dlg dialog


CRequisitionNo_DataView_Dlg::CRequisitionNo_DataView_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequisitionNo_DataView_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequisitionNo_DataView_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	m_pCM = NULL;
	m_pMainCM = NULL;
	m_pDM = NULL;
	m_pMainDM = NULL;
}
CRequisitionNo_DataView_Dlg::~CRequisitionNo_DataView_Dlg()
{
}

VOID CRequisitionNo_DataView_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequisitionNo_DataView_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequisitionNo_DataView_Dlg, CDialog)
	//{{AFX_MSG_MAP(CRequisitionNo_DataView_Dlg)
	ON_BN_CLICKED(IDC_btnDIVNO_INFO_PREV, OnbtnDIVNOINFOPREV)
	ON_BN_CLICKED(IDC_btnDIVNO_INFO_NEXT, OnbtnDIVNOINFONEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequisitionNo_DataView_Dlg message handlers

BOOL CRequisitionNo_DataView_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_������ȣ�ڷ�Ȯ��")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_������ȣ�ڷ�Ȯ��"));
	m_pDM = FindDM(_T("DM_������ȣ�ڷ�Ȯ��"));
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	GetDlgItem(IDC_btnDIVNO_INFO_PRINT)->ShowWindow(SW_HIDE);
	Display(0);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequisitionNo_DataView_Dlg::InitFirstLastIndex()
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	INT ids = pGrid->SelectMakeList();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	m_nFirstIdx = pGrid->SelectGetHeadPosition();
	m_nLastIdx = pGrid->SelectGetTailPosition();
}

VOID CRequisitionNo_DataView_Dlg::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnDIVNO_INFO_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDIVNO_INFO_NEXT)->EnableWindow(TRUE);

	if(m_nFirstIdx == idx || idx < 0)
		GetDlgItem(IDC_btnDIVNO_INFO_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx == idx || idx < 0)
		GetDlgItem(IDC_btnDIVNO_INFO_NEXT)->EnableWindow(FALSE);
}

VOID CRequisitionNo_DataView_Dlg::Display(INT idx)
{
	INT ids;
	CString errstr;
	CESL_Grid * pGrid = (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	switch(idx)
	{
	case 0:
		InitFirstLastIndex();
		pGrid->SelectMakeList();
		ids = pGrid->SelectGetHeadPosition();
		ShowNextPrevButton(ids);
		if (ids < 0)
		{
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		ids = pGrid->SelectGetPrev();
		ShowNextPrevButton(ids);
		if (ids == -1) return;
		break;
	case 1:
		ids = pGrid->SelectGetNext();
		ShowNextPrevButton(ids);
		if (ids == -1) return;
		break;
	}
	
	m_nIdx = ids;
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pMainDM->GetCellData(_T("���Գ⵵"), m_nIdx));
	m_pCM->SetControlMgrData(_T("������ȣ"), m_pMainDM->GetCellData(_T("������ȣ"), m_nIdx));
	m_pCM->SetControlMgrData(_T("������ȣ������"), m_pMainDM->GetCellData(_T("������ȣ������"), m_nIdx));
	m_pCM->SetControlMgrData(_T("������ȣ������"), m_pMainDM->GetCellData(_T("������ȣ������"), m_nIdx));
	
	ids = GetTotalPriceAndBookCount();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("���� ������ȣ�� �Ѱ������� ������ �� �����ϴ�."));
		return;
	}

	ids = MakeDM();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("���� ������ȣ�� ���� ������ ������ �� �����ϴ�."));
		return;
	}
	DisplayGrid(_T("MainGrid"));
	pGrid->SetReverse(m_nIdx);
}

INT CRequisitionNo_DataView_Dlg::GetTotalPriceAndBookCount()
{
	CString str64;
	GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T("����"),str64);
	if(str64 == _T("Y"))
		str64 = _T("-1");
	else
		str64 = _T("0");
	CString strAcqYear, strReqNo, strTotalPrice, strBookCnt;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("������ȣ"), strReqNo);
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("AcqCode"), 2);
	mgr.SetTempData(_T("AcqYear"), strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("DivNo"), strReqNo.GetBuffer(0));	
	mgr.SetTempData(_T("str64"), str64.GetBuffer(0));	
	INT ids = mgr.SPFExecutePath(_T("�Ѱ���������"));
	if(ids < 0) return ids;

	mgr.GetTempData(_T("TotalPrice"), strTotalPrice);
	mgr.GetTempData(_T("BookCount"), strBookCnt);

	m_pCM->SetControlMgrData(_T("��å��"), strBookCnt);
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strTotalPrice, strTotalPrice);
	m_pCM->SetControlMgrData(_T("�Ѱ���"), strTotalPrice);
	return 0;
}

INT CRequisitionNo_DataView_Dlg::MakeDM()
{
	CString str64;
	GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T("����"),str64);
	if(str64 == _T("Y"))
		str64 = _T("-1");
	else
		str64 = _T("0");

	CString strAcqYear, strReqNo, strTotalPrice, strBookCnt;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("������ȣ"), strReqNo);
	m_pCM->GetControlMgrData(_T("��å��"), strBookCnt);
	m_pCM->GetControlMgrData(_T("�Ѱ���"), strTotalPrice);
	strTotalPrice.Replace(_T(","), _T(""));
	INT totalBookCnt = _ttoi(strBookCnt.GetBuffer(0));
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("AcqCode"), 2);
	mgr.SetTempData(_T("AcqYear"), strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("DivNo"), strReqNo.GetBuffer(0));
	mgr.SetTempData(_T("BookCount"), totalBookCnt);
	mgr.SetTempData(_T("TotalPrice"), strTotalPrice.GetBuffer(0));
	mgr.SetTempData(_T("str64"), str64.GetBuffer(0));
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = mgr.SPFExecutePath(_T("�ڷ�Ȯ��DM����"));
	if(ids < 0) return ids;
	
	return 0;
}

INT CRequisitionNo_DataView_Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("�ڷ����"))
	{
		CString strWorkingStatus = m_pDM->GetCellData(_T("�ڷ����"), nRow);
		if(strWorkingStatus == _T("BOT111O")) str = _T("�����ڷ�");
		else if(strWorkingStatus == _T("BOT112O")) str = _T("�����ڷ�");
		else if(strWorkingStatus == _T("BOA111N")) str = _T("���Դ���ڷ�");
		else if(strWorkingStatus == _T("BOA112N")) str = _T("�ֹ��ڷ�");
		else if(strWorkingStatus == _T("BOA113O")) str = _T("�̳�ǰ�ڷ�");
		else if(strWorkingStatus == _T("BOA114O")) str = _T("�����ΰ����ڷ�");
		else if(strWorkingStatus == _T("BOA211O")) str = _T("���������ڷ�");
		else if(strWorkingStatus == _T("BOA212O")) str = _T("�����̵���ڷ�");
		else if(strWorkingStatus == _T("BOR111N")) str = _T("����ΰ��ڷ�");
		else if(strWorkingStatus == _T("BOR112N")) str = _T("����ڷ�");
		else if(strWorkingStatus == _T("BOR113O")) str = _T("�����ι�ȣ�ο��ڷ�");
		else if(strWorkingStatus == _T("BOC111O")) str = _T("�����������ڷ�");
		else if(strWorkingStatus == _T("BOC112O")) str = _T("�з�����ڷ�");
		else if(strWorkingStatus == _T("BOC113N")) str = _T("��������ڷ�");
		else if(strWorkingStatus == _T("BOC114O")) str = _T("�����ΰ����ڷ�");
		else if(strWorkingStatus == _T("BOL111O")) str = _T("�����ΰ��ڷ�");
		else if(strWorkingStatus == _T("BOL112N")) str = _T("�谡�ڷ�");
		else if(strWorkingStatus == _T("BOL113O")) str = _T("���������ڷ�");
		else if(strWorkingStatus == _T("BOL114O")) str = _T("�ļ��ڷ�");
		else if(strWorkingStatus == _T("BOL115O")) str = _T("�����ڷ�");
		else if(strWorkingStatus == _T("BOL211O")) str = _T("���ܴ����ڷ�");
		else if(strWorkingStatus == _T("BOL212O")) str = _T("���������ڷ�");		
		else if(strWorkingStatus == _T("BOL411O")) str = _T("����å�η����������ڷ�");
		else if(strWorkingStatus == _T("BOL511O")) str = _T("���յ���Ÿ���ݳ�");
		else if(strWorkingStatus == _T("BOL611O")) str = _T("���յ���Ÿ������");
		else if(strWorkingStatus == _T("BOL213O")) str = _T("Ư�������ڷ�");
		else if(strWorkingStatus == _T("BOL214O")) str = _T("��޴����ڷ�");
		else if(strWorkingStatus == _T("BOL215O")) str = _T("����ڷ�");
		else if(strWorkingStatus == _T("BOL116O")) str = _T("�������ڷ�");
		else if(strWorkingStatus == _T("BOL218O")) str = _T("��ȣ���������ڷ�");		
		else if(strWorkingStatus == _T("BOL219O")) str = _T("å���������ڷ�");
	}
	else if(strDMFieldAlias == _T("�ݾ�"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
	}
	return 0;
}

VOID CRequisitionNo_DataView_Dlg::DisplayGrid(CString gridAlias)
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(gridAlias);
	if(!pGrid) return;
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	pGrid->Display();
}

VOID CRequisitionNo_DataView_Dlg::OnbtnDIVNOINFOPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID CRequisitionNo_DataView_Dlg::OnbtnDIVNOINFONEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

HBRUSH CRequisitionNo_DataView_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
