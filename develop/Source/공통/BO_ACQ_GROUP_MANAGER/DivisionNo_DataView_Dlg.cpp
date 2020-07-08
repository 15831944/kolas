// DivisionNo_DataView_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DivisionNo_DataView_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_DataView_Dlg dialog


CDivisionNo_DataView_Dlg::CDivisionNo_DataView_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDivisionNo_DataView_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDivisionNo_DataView_Dlg)
	m_bDupYN = FALSE;
	m_bReviewYN = FALSE;
	m_bCopyYN = FALSE;
	//}}AFX_DATA_INIT
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	m_pCM = NULL;
	m_pMainCM = NULL;
	m_pDM = NULL;
	m_pMainDM = NULL;
}

CDivisionNo_DataView_Dlg::~CDivisionNo_DataView_Dlg()
{
}

VOID CDivisionNo_DataView_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNo_DataView_Dlg)
	DDX_Check(pDX, IDC_chkDIVNO_HOLD_DUP_SEARCH, m_bDupYN);
	DDX_Check(pDX, IDC_chkDIVNO_PURCHASE_CHECK, m_bReviewYN);
	DDX_Check(pDX, IDC_chkDIVNO_WORK_DUP_SEARCH, m_bCopyYN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNo_DataView_Dlg, CDialog)
	//{{AFX_MSG_MAP(CDivisionNo_DataView_Dlg)
	ON_BN_CLICKED(IDC_btnDIVNO_INFO_NEXT, OnbtnDIVNOINFONEXT)
	ON_BN_CLICKED(IDC_btnDIVNO_INFO_PREV, OnbtnDIVNOINFOPREV)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_DataView_Dlg message handlers

BOOL CDivisionNo_DataView_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_���������ڷ�Ȯ��")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	m_pCM = FindCM(_T("CM_���������ڷ�Ȯ��"));
	m_pDM = FindDM(_T("DM_���������ڷ�Ȯ��"));
	
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	GetDlgItem(IDC_btnDIVNO_INFO_PRINT)->ShowWindow(SW_HIDE);
	Display(0);

	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_edtDIVNO_STATUS);
	pEdit->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDivisionNo_DataView_Dlg::InitFirstLastIndex()
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

VOID CDivisionNo_DataView_Dlg::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnDIVNO_INFO_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDIVNO_INFO_NEXT)->EnableWindow(TRUE);

	if(m_nFirstIdx == idx || idx < 0)
		GetDlgItem(IDC_btnDIVNO_INFO_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx == idx || idx < 0)
		GetDlgItem(IDC_btnDIVNO_INFO_NEXT)->EnableWindow(FALSE);
}

VOID CDivisionNo_DataView_Dlg::Display(INT idx)
{
	INT ids;
	INT nSelectRow;
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
	nSelectRow = ids;

	CString strTmpData;
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pMainDM->GetCellData(_T("���Գ⵵"), ids));
	strTmpData = m_pMainDM->GetCellData(_T("�۾�����"), ids);
	if(strTmpData == _T("0")) strTmpData = _T("���Դ��");
	else if(strTmpData == _T("1")) strTmpData = _T("�ֹ�");
	else if(strTmpData == _T("2")) strTmpData = _T("�˼��Ϸ�");
	m_pCM->SetControlMgrData(_T("�۾�����"), strTmpData);
	m_pCM->SetControlMgrData(_T("������ȣ"), m_pMainDM->GetCellData(_T("������ȣ"), ids));
	m_pCM->SetControlMgrData(_T("����������"), m_pMainDM->GetCellData(_T("����������"), ids));
	m_pCM->SetControlMgrData(_T("����������"), m_pMainDM->GetCellData(_T("����������"), ids));
	m_pCM->SetControlMgrData(_T("��������"), m_pMainDM->GetCellData(_T("��������"), ids));
	strTmpData = m_pMainDM->GetCellData(_T("�ߺ����翩��"), ids);
	if(strTmpData == _T("Y")) m_bCopyYN = TRUE;		else m_bCopyYN = FALSE;
	strTmpData = m_pMainDM->GetCellData(_T("�������翩��"), ids);
	if(strTmpData == _T("Y")) m_bDupYN = TRUE;		else m_bDupYN = FALSE;
	strTmpData = m_pMainDM->GetCellData(_T("���ǿϷῩ��"), ids);
	if(strTmpData == _T("Y")) m_bReviewYN = TRUE;	else m_bReviewYN = FALSE;
	UpdateData(FALSE);

	ids = GetTotalPriceAndBookCount();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("���������� �Ѱ������� ������ �� �����ϴ�."));
		return;
	}
	ids = MakeDM();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("���������� ���� ������ ������ �� �����ϴ�."));
		return;
	}
	DisplayGrid(_T("MainGrid"));

	pGrid->SetReverse(nSelectRow);
	m_nIdx = nSelectRow;
}

INT CDivisionNo_DataView_Dlg::GetTotalPriceAndBookCount()
{
	CString str64;
	GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T("����"),str64);
	if(str64 == _T("Y"))
		str64 = _T("-1");
	else
		str64 = _T("0");
	
	CString strAcqYear, strDivNo, strTotalPrice, strBookCnt, strWorkCode;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("������ȣ"), strDivNo);
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("AcqCode"), 1);
	mgr.SetTempData(_T("AcqYear"), strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("DivNo"), strDivNo.GetBuffer(0));
	mgr.SetTempData(_T("WorkCode"), strWorkCode.GetBuffer(0));
	mgr.SetTempData(_T("str64"), str64.GetBuffer(0));
	INT ids = mgr.SPFExecutePath(_T("�Ѱ���������"));
	if(ids < 0) return ids;

	mgr.GetTempData(_T("TotalPrice"), strTotalPrice);
	mgr.GetTempData(_T("BookCount"), strBookCnt);
	
	if(strTotalPrice.IsEmpty()) strTotalPrice = _T("0");
	if(strBookCnt.IsEmpty()) strBookCnt = _T("0");
	m_pCM->SetControlMgrData(_T("��å��"), strBookCnt);
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strTotalPrice, strTotalPrice);
	m_pCM->SetControlMgrData(_T("�Ѱ���"), strTotalPrice);
	return 0;
}

INT CDivisionNo_DataView_Dlg::MakeDM()
{
	CString str64;
	GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T("����"),str64);
	if(str64 == _T("Y"))
		str64 = _T("-1");
	else
		str64 = _T("0");

	CString strAcqYear, strDivNo, strTotalPrice, strBookCnt, strWorkCode;
	m_pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear);
	m_pCM->GetControlMgrData(_T("������ȣ"), strDivNo);
	m_pCM->GetControlMgrData(_T("��å��"), strBookCnt);
	m_pCM->GetControlMgrData(_T("�Ѱ���"), strTotalPrice);
	strTotalPrice.Replace(_T(","), _T(""));
	m_pCM->GetControlMgrData(_T("��������"), strWorkCode);
	INT totalBookCnt = _ttoi(strBookCnt.GetBuffer(0));
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("AcqCode"), 1);
	mgr.SetTempData(_T("AcqYear"), strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("DivNo"), strDivNo.GetBuffer(0));
	mgr.SetTempData(_T("BookCount"), totalBookCnt);
	mgr.SetTempData(_T("TotalPrice"), strTotalPrice.GetBuffer(0));
	mgr.SetTempData(_T("WorkCode"), strWorkCode.GetBuffer(0));
	mgr.SetTempData(_T("str64"), str64.GetBuffer(0));
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = mgr.SPFExecutePath(_T("�ڷ�Ȯ��DM����"));
	if(ids < 0) return ids;
	
	return 0;
}

INT CDivisionNo_DataView_Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
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
		else if(strWorkingStatus == _T("BOL252N")) str = _T("�н��ڷ�");
		else if(strWorkingStatus == _T("BOL116O")) str = _T("�������ڷ�");
		else if(strWorkingStatus == _T("BOL218O")) str = _T("��ȣ�����ڷ�");							
		else if(strWorkingStatus == _T("BOL219O")) str = _T("å���������ڷ�");
		else if(strWorkingStatus == _T("BOL221O")) str = _T("���ܴ����ڷ�");
		else if(strWorkingStatus == _T("BOL222O")) str = _T("���������ڷ�");
		else if(strWorkingStatus == _T("BOL223O")) str = _T("Ư�������ڷ�");
		else if(strWorkingStatus == _T("BOL224O")) str = _T("��޴����ڷ�");
		else if(strWorkingStatus == _T("BOL225O")) str = _T("����ڷ�");
		else if(strWorkingStatus == _T("BOL251N")) str = _T("���������ڷ�");
		else if(strWorkingStatus == _T("BOL253N")) str = _T("�����ڷ�");
		else if(strWorkingStatus == _T("BOL217O")) str = _T("�������ڷ�");		
		else if(strWorkingStatus == _T("BOT211O")) str = _T("���������߱��ڷ�");
		else if(strWorkingStatus == _T("BOT212O")) str = _T("���������߱��ڷ�");
	}
	else if(strDMFieldAlias == _T("�ݾ�"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
	}
	return 0;
}

VOID CDivisionNo_DataView_Dlg::DisplayGrid(CString gridAlias)
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(gridAlias);
	if(!pGrid) return;
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	pGrid->Display();
}

VOID CDivisionNo_DataView_Dlg::OnbtnDIVNOINFONEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CDivisionNo_DataView_Dlg::OnbtnDIVNOINFOPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

HBRUSH CDivisionNo_DataView_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
