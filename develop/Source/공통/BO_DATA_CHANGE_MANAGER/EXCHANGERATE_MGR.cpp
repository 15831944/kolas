// EXCHANGERATE_MGR.cpp : implementation file
//

#include "stdafx.h"
#include "EXCHANGERATE_MGR.h"
#include "EXCHANGERATE_INSERT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEXCHANGERATE_MGR dialog


CEXCHANGERATE_MGR::CEXCHANGERATE_MGR(CESL_Mgr* pParent, CString currency_code)
	: CESL_Mgr(CEXCHANGERATE_MGR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEXCHANGERATE_MGR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_currencycode = currency_code;
	m_pDM = NULL;
	m_pCM = NULL;
}

CEXCHANGERATE_MGR::~CEXCHANGERATE_MGR()
{
}

VOID CEXCHANGERATE_MGR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEXCHANGERATE_MGR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEXCHANGERATE_MGR, CDialog)
	//{{AFX_MSG_MAP(CEXCHANGERATE_MGR)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnEX_INSERT, OnbtnINSERT)
	ON_BN_CLICKED(IDC_btnEX_UPDATE, OnbtnUPDATE)
	ON_BN_CLICKED(IDC_btnEX_DELETE, OnbtnDELETE)
	ON_BN_CLICKED(IDC_btnEX_SEARCH, OnbtnSEARCH)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEXCHANGERATE_MGR message handlers

VOID CEXCHANGERATE_MGR::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
		
	CDialog::OnClose();
}

BOOL CEXCHANGERATE_MGR::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_ȯ������")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_ȯ������"));
	m_pDM = FindDM(_T("DM_ȯ������"));

	RefreshDM();

	m_pCM->SetControlMgrData(_T("ȭ�󱸺�"), m_currencycode);
	GetDlgItem(IDC_cmbEX_CURRENCY_CODE)->EnableWindow(FALSE);

	m_pCM->ControlDisplay(_T("MainGrid"));

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CEXCHANGERATE_MGR::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CEXCHANGERATE_MGR::OnbtnINSERT() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pCM->GetControlMgrData(_T("ȭ�󱸺�"), m_currencycode);
	CEXCHANGERATE_INSERT dlg(this);
	dlg.SetOpenMode(-1);
	dlg.SetMainGrid(pGrid);
	dlg.SetDM(m_pDM);
	dlg.SetCurrencyCode(m_currencycode);

	if(IDOK == dlg.DoModal())
		m_pCM->ControlDisplay(_T("MainGrid"));
}

VOID CEXCHANGERATE_MGR::OnbtnUPDATE() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pCM->GetControlMgrData(_T("ȭ�󱸺�"), m_currencycode);
	CEXCHANGERATE_INSERT dlg(this);
	dlg.SetOpenMode(1);
	dlg.SetMainGrid(pGrid);
	dlg.SetDM(m_pDM);
	dlg.SetCurrencyCode(m_currencycode);

	if(IDOK == dlg.DoModal())
		m_pCM->ControlDisplay(_T("MainGrid"));
}

VOID CEXCHANGERATE_MGR::OnbtnDELETE() 
{
	// TODO: Add your control notification handler code here
	INT ids = this->SelectMakeList(m_pCM->CM_ALIAS, _T("MainGrid"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("���� �׸��� ã�� �� �����ϴ�."));
		return;
	}
	INT i = this->SelectGetHeadPosition(m_pCM->CM_ALIAS, _T("MainGrid"));
	if(i < 0)
	{
		ESLAfxMessageBox(_T("���� �׸��� ã�� �� �����ϴ�."));
		return;
	}
	
	CString key;

	m_pDM->StartFrame();

	while(i >= 0)
	{
		m_pDM->GetCellData(_T("ȯ������KEY"), i, key);
		m_pDM->MakeDeleteFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), key);
		
		i = this->SelectGetNext(m_pCM->CM_ALIAS, _T("MainGrid"));
	}
	ids = m_pDM->SendFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("ȯ������ ���� �۾��� �������� ���Ͽ����ϴ�."));
		return;
	}
	ids = m_pDM->EndFrame();
	if(ids < 0)  // 2004.09.08 if�� �����־ �߰��Ͽ���.
	{
		ESLAfxMessageBox(_T("ȯ������ ���� �۾��� �������� ���Ͽ����ϴ�."));
		return;
	}
	ESLAfxMessageBox(_T("�����Ͻ� ȯ�������� �����Ͽ����ϴ�."));
	
	i = this->SelectGetTailPosition(m_pCM->CM_ALIAS, _T("MainGrid"));
	while(i >= 0)
	{
		m_pDM->DeleteRow(i);
		i = this->SelectGetPrev(m_pCM->CM_ALIAS, _T("MainGrid"));
	}
	this->ControlDisplay(m_pCM->CM_ALIAS, _T("MainGrid"));
}

VOID CEXCHANGERATE_MGR::OnOK() 
{
	// TODO: Add extra validation here
	if(!m_pDM)
	{
		ESLAfxMessageBox(_T("ȯ������ DM�� ã�� ���Ͽ����ϴ�."));
		return;
	}

	INT ids = this->SelectMakeList(m_pCM->CM_ALIAS, _T("MainGrid"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	INT i = this->SelectGetHeadPosition(m_pCM->CM_ALIAS, _T("MainGrid"));
	if(i < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	CString exchangerate;
	m_pDM->GetCellData(_T("ȯ��"), i, exchangerate);
	m_exchangerate = exchangerate;
	
	// ȯ�� _T("���뿩��") �⺻�� �����ϱ�
	// ������ ȯ���� ���Ͽ� _T("���뿩��") ���� �Ѵ�.
	// 2005-09-28 ������
	//
	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();

	for(INT j=0; j<m_pDM->GetRowCount(); j++)
	{
		m_pDM->SetCellData(_T("���뿩��"), _T("N"), j);
		CString strKey = m_pDM->GetCellData(_T("ȯ������KEY"), j);
		m_pDM->AddDBFieldData(_T("APPLY_YN"), _T("STRING"), _T("N"));
		m_pDM->MakeUpdateFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strKey);
	}

	m_pDM->AddDBFieldData(_T("APPLY_YN"), _T("STRING"), _T("Y"));
	m_pDM->MakeUpdateFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), m_pDM->GetCellData(_T("ȯ������KEY"), i) );

	ids = m_pDM->SendFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�⺻������ ������ �� �����ϴ�."));
		return;
	}
	ids = m_pDM->EndFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�⺻������ ������ �� �����ϴ�."));
		return;
	}

	m_pDM->SetCellData(_T("���뿩��"), _T("Y"), i);
	//--�̻�. ��.--


	CDialog::OnOK();
}

VOID CEXCHANGERATE_MGR::OnChangeedtYEAR() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	RefreshDM();	
}

CString CEXCHANGERATE_MGR::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

INT CEXCHANGERATE_MGR::RefreshDM()
{
	CString year;
	m_pCM->GetControlMgrData(_T("�⵵"), year);
	if(year.IsEmpty()) year = GetTodayYear();
	if(m_currencycode.IsEmpty())
		m_pInfo->pCodeMgr->GetDefaultCode(_T("ȭ���ȣ����"), m_currencycode);
	CString query, startdate, enddate, key;
	query.Format(_T("select rec_key from cd_code_tbl where class = 23 and code like '%s%%'"), m_currencycode); 
	m_pDM->GetOneValueQuery(query, key);
	startdate.Format(_T("%s/%s/%s"), year, _T("01"), _T("01"));
	enddate.Format(_T("%s/%s/%s"), year, _T("12"), _T("31"));
	query.Format(_T("USER_CODE_MNG_KEY = %s AND INPUT_DATE >= '%s' AND INPUT_DATE <= '%s'"), key, startdate, enddate);
	INT ids = m_pDM->RefreshDataManager(query);
	
	return ids;
}

VOID CEXCHANGERATE_MGR::OnbtnSEARCH() 
{
	// TODO: Add your control notification handler code here
	CString strYear;
	INT ids = m_pCM->GetControlMgrData(_T("ȭ�󱸺�"), m_currencycode);
	ids = m_pCM->GetControlMgrData(_T("�⵵"), strYear);
	if(ids < 0) return;
	
	ids = this->RefreshDM();
	if(ids < 0) return;
	m_pCM->ControlDisplay(_T("MainGrid"));
}


BEGIN_EVENTSINK_MAP(CEXCHANGERATE_MGR, CDialog)
    //{{AFX_EVENTSINK_MAP(CEXCHANGERATE_MGR)
	ON_EVENT(CEXCHANGERATE_MGR, IDC_gridEXCHANGERATE, -601 /* DblClick */, OnDblClickgridEXCHANGERATE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CEXCHANGERATE_MGR::OnDblClickgridEXCHANGERATE() 
{
	// TODO: Add your control notification handler code here
	OnbtnUPDATE();
}

HBRUSH CEXCHANGERATE_MGR::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
