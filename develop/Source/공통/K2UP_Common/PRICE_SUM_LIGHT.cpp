// PRICE_SUM_LIGHT.cpp : implementation file
//

#include "stdafx.h"
#include "PRICE_SUM_LIGHT.h"
#include "PriceSum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPRICE_SUM_LIGHT dialog


CPRICE_SUM_LIGHT::CPRICE_SUM_LIGHT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CPRICE_SUM_LIGHT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPRICE_SUM_LIGHT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_OpenMode = -1;
	m_PriceMgr = NULL;
	m_OpType = 0;
	m_IsMissingData = FALSE;
	m_IsSpecies = FALSE;
}

CPRICE_SUM_LIGHT::~CPRICE_SUM_LIGHT()
{
	if(m_PriceMgr)
	{
		delete m_PriceMgr;
		m_PriceMgr = NULL;
	}
}

VOID CPRICE_SUM_LIGHT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPRICE_SUM_LIGHT)
	DDX_Control(pDX, IDC_tabPRICE_SUM, m_tabPriceSum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPRICE_SUM_LIGHT, CDialog)
	//{{AFX_MSG_MAP(CPRICE_SUM_LIGHT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabPRICE_SUM, OnSelchangetabPRICESUM)
	ON_BN_CLICKED(IDC_bPRINT, OnbPRINT)
	ON_CBN_SELCHANGE(IDC_cmbPSUM_CLASS, OnSelchangecmbPSUMCLASS)
	ON_BN_CLICKED(IDC_btnPSUM, OnbtnPSUM)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPRICE_SUM_LIGHT message handlers
/*VOID CPRICE_SUM_LIGHT::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}
*/BOOL CPRICE_SUM_LIGHT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tabPRICE_SUM);
	pTab->InsertItem(0, _T("KDC������"));
	pTab->InsertItem(1, _T("DDC������"));
	pTab->InsertItem(2, _T("��ϱ��к�"));
	pTab->InsertItem(3, _T("��ġ���к�"));
	pTab->InsertItem(4, _T("���ı��к�"));

	if (InitESL_Mgr(_T("K2UP_�����ջ�")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	m_PriceMgr = new CPriceSum(this);
	INT ids = m_PriceMgr->InitPriceSum(_T("CM_LIGHT_����_����"), m_tabCount, m_IsSpecies, m_IsMissingData);
	CString errstr;
	if(ids == 0)
	{
		CString totalprice, totalbookcnt;
		totalprice.Format(_T("%.2f"), m_PriceMgr->m_TotalPrice);
		totalbookcnt.Format(_T("%d"), m_PriceMgr->m_TotalBookCnt);
		this->SetControlData(_T("CM_�����ջ�"), _T("��������"), totalprice);
		this->SetControlData(_T("CM_�����ջ�"), _T("��å��"), totalbookcnt);

		this->AllControlDisplay(_T("CM_�����ջ�"));
		m_OpType = 1;
	}
	else m_OpType = -1;

	ShowCtrlByOpenMode();

	CString dateType[3][2] =
	{
		_T("0"), _T("�Է���"),
		_T("1"), _T("�ֹ���"),
		_T("2"), _T("�˼���"),
	};

	CString priceSumType[2][2] =
	{
		_T("0"), _T("���������ջ�"),
		_T("1"), _T("�����������ջ�")
	};
	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(_T("CM_�����ջ�"), _T("�Է���"), dateType, _T("�����ջ곯¥����"), 3);
	mgr.SetMulTiComboBoxStrData(_T("CM_�����ջ�"), _T("���������ջ�"), priceSumType, _T("�����ջ걸��"), 2);

	// �׸��� ���ı�� ����
	CESL_Grid * pGrid = (CESL_Grid *)FindControl(_T("CM_�����ջ�"), _T("MainGrid"));
	if(pGrid) pGrid->SetSort(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPRICE_SUM_LIGHT::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);	
}

VOID CPRICE_SUM_LIGHT::OnSelchangetabPRICESUM(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	INT count = m_tabPriceSum.GetCurSel();
	switch(count)
	{
	case 0:
		m_PriceMgr->MakePriceDM(0, m_IsSpecies, _T("KDC��������"), m_OpType);
		break;
	case 1:
		m_PriceMgr->MakePriceDM(1, m_IsSpecies, _T("LDC��������"), m_OpType);
		break;
	case 2:
		m_PriceMgr->MakePriceDM(2, m_IsSpecies, _T("��ϱ���"), m_OpType);
		break;
	case 3:
		m_PriceMgr->MakePriceDM(3, m_IsSpecies, _T("��ġ����"), m_OpType);
		break;
	case 4:
		m_PriceMgr->MakePriceDM(4, m_IsSpecies, _T("���ı���"), m_OpType);
		break;
	}

	AllControlDisplay(_T("CM_�����ջ�"));

	*pResult = 0;
}

VOID CPRICE_SUM_LIGHT::SetIsSpecies(BOOL isSpecies)
{
	m_IsSpecies = isSpecies;
}

VOID CPRICE_SUM_LIGHT::SetTabCount(INT tabCount)
{
	m_tabCount = tabCount;
}

VOID CPRICE_SUM_LIGHT::OnbPRINT() 
{
	// TODO: Add your control notification handler code here
	CBasicPrintMgr mgr(this);
	mgr.SetPrintDMAlias(_T("DM_�����ջ�"));
	mgr.SetMainCMAlias(_T("CM_�����ջ�"));
	mgr.SetPrintType(BO_ACQ_LIGHT_GRID_PRICESUM);
	INT tabCount = m_tabPriceSum.GetCurSel(); 
	if(tabCount == 0)
		mgr.SetSIReportFilename(_T("KDC�����������ջ�"));
	else if(tabCount == 1)
		mgr.SetSIReportFilename(_T("DDC�����������ջ�"));
	else if(tabCount == 2)
		mgr.SetSIReportFilename(_T("��ϱ��к������ջ�"));
	else if(tabCount == 3)
		mgr.SetSIReportFilename(_T("��ġ���к������ջ�"));
	else if(tabCount == 4)
		mgr.SetSIReportFilename(_T("���ı��к������ջ�"));
	mgr.Print();
}

VOID CPRICE_SUM_LIGHT::SetOpenMode(INT openMode)
{
	m_OpenMode = openMode;
}

VOID CPRICE_SUM_LIGHT::ShowCtrlByOpenMode()
{
	if(m_OpenMode < 0)
	{
		ESLAfxMessageBox(_T("���� �����ջ� ���¸�带 ������ �ֽʽÿ�."));
		EndDialog(IDCANCEL);
		return;
	}
	GetDlgItem(IDC_cmbPSUM_DATE_CLASS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DATE1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DATE2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_btnVIEW_DIVISION_NO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPSUM_DIVISION_NO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtPSUM_DIVISION_NO2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_cmbPSUM_CLASS)->EnableWindow(TRUE);

	if(m_OpenMode == PRICE_SUM_LIGHT)
		GetDlgItem(IDC_cmbPSUM_CLASS)->EnableWindow(FALSE);
}

VOID CPRICE_SUM_LIGHT::OnSelchangecmbPSUMCLASS() 
{
	// TODO: Add your control notification handler code here
	CString priceSumClass = _T("");
	GetControlData(_T("CM_�����ջ�"), _T("�����ջ걸��"), priceSumClass);
	ShowCtrlByPriceSumClass(priceSumClass);
}

VOID CPRICE_SUM_LIGHT::ShowCtrlByPriceSumClass(CString priceSumClass)
{
	GetDlgItem(IDC_cmbPSUM_DATE_CLASS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DATE1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DATE2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_btnVIEW_DIVISION_NO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DIVISION_NO1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_edtPSUM_DIVISION_NO2)->ShowWindow(SW_SHOW);
	if(priceSumClass == _T("0"))
	{
		GetDlgItem(IDC_btnVIEW_DIVISION_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_DIVISION_NO1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_DIVISION_NO2)->ShowWindow(SW_HIDE);	
	}
	else if(priceSumClass == _T("1"))
	{
		GetDlgItem(IDC_cmbPSUM_DATE_CLASS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_DATE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtPSUM_DATE2)->ShowWindow(SW_HIDE);
	}
	else
		return;
}

VOID CPRICE_SUM_LIGHT::OnbtnPSUM() 
{
	// TODO: Add your control notification handler code here
	m_IsSpecies = FALSE;
	INT count = m_tabPriceSum.GetCurSel();
	INT ids = 0;
	switch(count)
	{
	case 0:
		ids = m_PriceMgr->SetReckeyList(_T("CM_�����ջ�"), _T("KDC��������"));
		break;
	case 1:
		ids = m_PriceMgr->SetReckeyList(_T("CM_�����ջ�"), _T("LDC��������"));
		break;
	case 2:
		ids = m_PriceMgr->SetReckeyList(_T("CM_�����ջ�"), _T("��ϱ���"));
		break;
	case 3:
		ids = m_PriceMgr->SetReckeyList(_T("CM_�����ջ�"), _T("��ġ����"));
		break;
	case 4:
		ids = m_PriceMgr->SetReckeyList(_T("CM_�����ջ�"), _T("���ı���"));
		break;
	}
	if(ids < 0)
	{
		//����ó��
		ESLAfxMessageBox(_T("�����ջ��� ���� DB�۾��� ������ �ֽ��ϴ�."));
		return;
	}
	else if( ids > 0) return ;
	CString totalprice, totalbookcnt;
	totalprice.Format(_T("%.2f"), m_PriceMgr->m_TotalPrice);
	totalbookcnt.Format(_T("%d"), m_PriceMgr->m_TotalBookCnt);
	this->SetControlData(_T("CM_�����ջ�"), _T("��������"), totalprice);
	this->SetControlData(_T("CM_�����ջ�"), _T("��å��"), totalbookcnt);

	this->AllControlDisplay(_T("CM_�����ջ�"));
	m_OpType = -1;
}

VOID CPRICE_SUM_LIGHT::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	DestroyWindow();
}

VOID CPRICE_SUM_LIGHT::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}
