// SePurchaseForm.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseForm.h"
#include "SeSubHistoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseForm dialog


CSePurchaseForm::CSePurchaseForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchaseForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseForm)
	m_bCLAIM_YN = FALSE;
	m_bREAD_FIRST_YN = FALSE;
	//}}AFX_DATA_INIT
	
	m_pParentMgr = pParent;
	m_pDM = NULL;
	m_pDM_SUB_HISTORY = NULL;
	m_pCM = NULL;
	m_nOpenMode = 1; //default
	m_bIsLastCtrlFocused = FALSE;

	m_bIsLightVersion = FALSE;
	m_bIsDepartMngUse = FALSE;

	m_pDepartmentOrder = NULL;
	m_bIsDepartMngFirstShow = TRUE;

}

CSePurchaseForm::~CSePurchaseForm()
{
	// 17/09/06 ������ : ����>����>üũ�ΰ������� �޸𸮴��� ���� �߻�
//*/ ADD ---------------------------------------------------------------------------
	if(m_pDepartmentOrder) 
	{
		delete m_pDepartmentOrder;
		m_pDepartmentOrder = NULL;
	}
//*/ END ---------------------------------------------------------------------------
}

VOID CSePurchaseForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseForm)
	DDX_Check(pDX, IDC_chkPUR_CLAIM_YN, m_bCLAIM_YN);
	DDX_Check(pDX, IDC_chkPUR_READ_FIRST_YN, m_bREAD_FIRST_YN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseForm, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseForm)
	ON_BN_CLICKED(IDC_btnPUR_SUB_HISTORY, OnbtnPURSUBHISTORY)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_edtPUR_CLAIM_INTERVAL, OnKillfocusedtPURCLAIMINTERVAL)
	ON_EN_SETFOCUS(IDC_edtPUR_CLAIM_INTERVAL, OnSetfocusedtPURCLAIMINTERVAL)
	ON_BN_CLICKED(IDC_btnPUR_DEPARTMNG, OnbtnPURDEPARTMNG)
	ON_MESSAGE(BOOKCNTCHANGE, OnDepartmentBookCntChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseForm message handlers
BOOL CSePurchaseForm::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

BOOL CSePurchaseForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		
	if( InitESL_Mgr( _T("����_����_��������") ) < 0 ) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_��������") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_��������"));
	m_pDM = FindDM(_T("DM_����_��������"));
	m_pDM_SUB_HISTORY =  FindDM(_T("DM_����_�����̷�����"));


	if(!m_pDM || !m_pCM || !m_pDM_SUB_HISTORY)  {
		AfxMessageBox( _T("DM,CM Error : ����_����_��������") );
		return FALSE;
	}
	
	if(m_nOpenMode>0)
		GetDlgItem(IDC_btnPUR_SUB_HISTORY)->ShowWindow(SW_HIDE);

	
	//control color
	AddColorControl(IDC_edtPUR_ACQ_BOOK_CNT, _T(""), _T(""), 'E');

	//����Ʈ ���� settting
	if(m_bIsLightVersion)
	{
		SetLightVersion();
	}

	//�μ� ���� ��� ���� 
	if(!m_bIsDepartMngUse)
	{
		SetDepartMngNoUse();
	}
	else
	{
		CreateDepartMngDlg();
	}


	//�����ϰ�쿡�� display
	CString str64;
	GetManageValue( _T("��Ÿ"), _T("����"), _T("����"), _T("����"), str64 );
	if ( str64 == _T("Y") ) 
	{
		GetDlgItem(IDC_sYookSa1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbDept)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_sYookSa2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbCource)->ShowWindow(SW_SHOW);		
	}
	else
	{
		GetDlgItem(IDC_sYookSa1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbDept)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_sYookSa2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbCource)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_UNIV)->ShowWindow(SW_HIDE);		
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSePurchaseForm::SetLightVersion()
{

	//����Ʈ ������ ��� �����켱����, ���å�� ���� ��Ʈ�Ѿ��ְ� �Լ�å�� ���� ��Ʈ���� ��ġ�� ���ġ �Ѵ� 
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_chkPUR_READ_FIRST_YN);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_STATIC_PUR_REG_BOOK_CNT);
	if(pWnd) pWnd->ShowWindow(FALSE);

	pWnd = GetDlgItem(IDC_edtPUR_REG_BOOK_CNT);
	if(pWnd) pWnd->ShowWindow(FALSE);

	CRect rect;
	
	pWnd = GetDlgItem(IDC_STATIC_PUR_ACQ_BOOK_CNT);
	if(pWnd)
	{
		pWnd->GetClientRect(rect);
	
		rect.left = 12;
		rect.right = 61;
		rect.top = 39;
		rect.bottom = 53;

		pWnd->MoveWindow(rect);
	}
	
	pWnd = GetDlgItem(IDC_edtPUR_ACQ_BOOK_CNT);
	if(pWnd)
	{
		pWnd->GetClientRect(rect);
		
		rect.left = 63;
		rect.right = 180;
		rect.top = 37;
		rect.bottom = 57;		
		
		pWnd->MoveWindow(rect);
	}

	//����Ʈ ������ ���� ������ �����켱�̴�
	m_bREAD_FIRST_YN = TRUE;

	UpdateData(FALSE);
}

VOID CSePurchaseForm::SaveAllDataToDM()
{
	
	UpdateData(TRUE);
	//���� �������� DM�� �����Ѵ�
	//DM���� ����������� data��
	const INT cnt = 10;
	CString alias[ cnt ] = {
		_T("SP_��������"),			//	0
		_T("SP_����������"),		//	1
		_T("SP_����������"),		//	2
		_T("SP_����ó_�ڵ�"),		//	3
		_T("SP_���˰���"),			//	4
		_T("SP_���å��"),			//	5
		_T("SP_�Լ�å��"),			//	6
		_T("SP_����μ�"),			//  7
		_T("SP_��û�к�"),			//  8
		_T("SP_��û�а�")};			//	9
		
	CString tmp;

	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");	
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM->SetCellData(alias[i],tmp,0);
	}

	//���˿���
	if(m_bCLAIM_YN)
		m_pDM->SetCellData(_T("SP_���˿���"),_T("Y"),0);
	else 
	    m_pDM->SetCellData(_T("SP_���˿���"),_T("N"),0);
	
		//�����켱����
	if(m_bREAD_FIRST_YN)
		m_pDM->SetCellData(_T("SP_�����켱����"),_T("Y"),0);
	else 
	    m_pDM->SetCellData(_T("SP_�����켱����"),_T("N"),0);

}


INT CSePurchaseForm::RefreshAll(CString strSpeciesKey)
{
	CString strWhere;
	strWhere.Format(_T("SPECIES_KEY = %s"),strSpeciesKey);
	m_pDM->RefreshDataManager(strWhere);
	m_pCM->AllControlDisplay(0);
	
	m_pDM_SUB_HISTORY->FreeData();

	//���԰��� �����Ͱ� ���� ���� ���� �����͸� �������� �Է��ϴ� ����̴�
	INT nRowCnt = m_pDM->GetRowCount();

	if(nRowCnt>0)
	{
		CString strREC_KEY;
		strREC_KEY = m_pDM->GetCellData(_T("SP_�����̷�����KEY"),0);
		strWhere.Format(_T("REC_KEY = %s"),strREC_KEY);
		if(!strREC_KEY.IsEmpty()) {
			m_pDM_SUB_HISTORY->RefreshDataManager(strWhere);
		}
	}

	
	INT nRow = m_pDM_SUB_HISTORY->GetRowCount();

	CString tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SP_���˿���"),0);
	if(_T("Y") == tmp)
		m_bCLAIM_YN = TRUE;
	else 
		m_bCLAIM_YN = FALSE;

	tmp = _T("");
	

	tmp = m_pDM->GetCellData(_T("SP_�����켱����"),0);
	if(_T("Y") == tmp)
		m_bREAD_FIRST_YN = TRUE;
	else 
		m_bREAD_FIRST_YN = FALSE;
	
	//2005.02.16 ���� +++
	if ( m_pDepartmentOrder == NULL ) 
		CreateDepartMngDlg();
	//2005.02.16 ���� ---

	if(0 == nRowCnt)
	{
		m_pDepartmentOrder->m_nOpenMode = 1;	
	}
	else
	{
		m_pDepartmentOrder->m_nOpenMode = -1;

		//�μ������ڷ�
		if(m_pDepartmentOrder)
		{
			
			if(m_nOpenMode<0)
			{
				//�μ��� �ڷ����� 
				CString strPURCHASE_SPECIES_KEY;
				CString stPURCHASE_MANAGE_KEY;
				stPURCHASE_MANAGE_KEY = m_pDM->GetCellData(_T("SP_���԰���KEY"),0);

				//m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
				m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;

			}

			//�������� ���� �μ��� �Լ� å���� ������ ��� 
			//�Լ� å���� ���� ���� ���ϵ��� �Ѵ�
			CString strCnt;
			CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("SP_�Լ�å��"));
			pEdit->EnableWindow(TRUE);
			INT nAcqExpBookCnt = 0;
			if(m_nOpenMode<0)
			{
				m_pDepartmentOrder->RefreshAll();
				nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt();
				if(nAcqExpBookCnt>0)
				{
					pEdit->EnableWindow(FALSE);
					
				}
			}
		}

	
	}
	
	UpdateData(FALSE);
	
	return 0;
}

VOID CSePurchaseForm::ClearAll()
{

	m_pCM->AllControlClear();
	m_bCLAIM_YN = FALSE;
	m_bREAD_FIRST_YN = FALSE;
	UpdateData(FALSE);

}

VOID CSePurchaseForm::OnbtnPURSUBHISTORY() 
{
	// TODO: Add your control notification handler code here
	INT nRowCnt = m_pDM->GetRowCount();
	if(0 == nRowCnt) 
	{
		AfxMessageBox(_T("���� �����̷��� �������� �ʽ��ϴ�!"));
		return;
	}

	CString strREC_KEY;
	strREC_KEY = m_pDM->GetCellData(_T("SP_���԰���KEY"),0);

	CString strCurrentSubHistoryKey;
	strCurrentSubHistoryKey = m_pDM_SUB_HISTORY->GetCellData(_T("�����̷�KEY"),0);

	CSeSubHistoryDlg dlg(this);
	dlg.m_strCurrentKey = strCurrentSubHistoryKey;
	dlg.m_strManageKey = strREC_KEY;
	if(dlg.DoModal() != IDOK) return;

	CString strNewKey;
	strNewKey = dlg.m_strNewKey;
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strNewKey);
	m_pDM_SUB_HISTORY->RefreshDataManager(strWhere);

	if(0 == m_pDM_SUB_HISTORY->GetRowCount()) return;

	
	//�� �����̷� ������ �������ش�
	
	//�����̷�����KEY
	CString strtmp = _T("");
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("�����̷�KEY"),0);
	m_pDM->SetCellData(_T("SP_�����̷�����KEY"),strtmp,0);
	strtmp = _T("");


	//����ó_�ڵ� 
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("����ó_�ڵ�"),0);
	m_pDM->SetCellData(_T("SP_����ó_�ڵ�"),strtmp,0);
	strtmp = _T("");


	//�������� 
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("��������"),0);
	m_pDM->SetCellData(_T("SP_��������"),strtmp,0);
	strtmp = _T("");


	//����������
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("����������"),0);
	m_pDM->SetCellData(_T("SP_����������"),strtmp,0);
	strtmp = _T("");


	//���������� 
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("����������"),0);
	m_pDM->SetCellData(_T("SP_����������"),strtmp,0);
	strtmp = _T("");

	//����μ� 
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("����μ�"),0);
	m_pDM->SetCellData(_T("SP_����μ�"),strtmp,0);
	strtmp = _T("");

	m_pCM->AllControlDisplay(0);

}

HBRUSH CSePurchaseForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (IsColorControl(pWnd->GetDlgCtrlID())) {
		return SetColor(pDC);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

VOID CSePurchaseForm::SetFocusOnFirstCtrl()
{
	GetDlgItem(IDC_cmbPUR_PROVIDER_CODE)->SetFocus();
}

VOID CSePurchaseForm::OnKillfocusedtPURCLAIMINTERVAL() 
{
	// TODO: Add your control notification handler code here
	
	m_bIsLastCtrlFocused = FALSE;
}

BOOL CSePurchaseForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_TAB)
		{
			if(m_bIsLastCtrlFocused)
			{
				// ������ ��Ʈ���� killfocus�ϸ� parent���� �޽����� ������
				:: SendMessage(	m_pParentMgr->GetSafeHwnd(), KILLFOCUS_LAST_CTRL , 0, 0);

				return TRUE;
			}
		}
		else if ( pMsg->wParam == VK_ESCAPE )
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSePurchaseForm::OnSetfocusedtPURCLAIMINTERVAL() 
{
	// TODO: Add your control notification handler code here
	m_bIsLastCtrlFocused = TRUE;
}

VOID CSePurchaseForm::SetDepartMngNoUse()
{
	//�μ� ��������� ������� �ʴ� ��� ����μ��� �μ����� ��ư�� ����� 
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_stcPUR_ISSUE);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_edtPUR_ISSUE);
	if(pWnd) pWnd->ShowWindow(FALSE);	

	pWnd = GetDlgItem(IDC_btnPUR_DEPARTMNG);
	if(pWnd) pWnd->ShowWindow(FALSE);

}

VOID CSePurchaseForm::CreateDepartMngDlg()
{
	m_pDepartmentOrder = new CSeDepartmentOrder(this);
	if(!m_pDepartmentOrder) return;

	m_pDepartmentOrder->m_nOpenMode = m_nOpenMode;
	m_pDepartmentOrder->m_nOpenLockMode = -1;
	m_pDepartmentOrder->Create((CESL_Mgr*)this);
	m_pDepartmentOrder->CenterWindow();
	m_pDepartmentOrder->ShowWindow(FALSE);



	/*
	if(m_nOpenMode<0)
	{
		//�μ��� �ڷ����� 
		CString strPURCHASE_SPECIES_KEY;
		CString stPURCHASE_MANAGE_KEY;
		stPURCHASE_MANAGE_KEY = m_pDM->GetCellData(_T("SP_���԰���KEY"),0);

		//m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
		m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;

	}

	m_pDepartmentOrder->Create((CESL_Mgr*)this);
	m_pDepartmentOrder->CenterWindow();
	m_pDepartmentOrder->ShowWindow(FALSE);

	//�������� ���� �μ��� �Լ� å���� ������ ��� 
	//�Լ� å���� ���� ���� ���ϵ��� �Ѵ�
	if(m_nOpenMode<0)
	{
		m_pDepartmentOrder->GetTotalAcqExpBookCnt();
		if(m_pDepartmentOrder->m_nTotalAcqExpBookCnt>0)
		{
			CString strCnt;
			CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("SP_�Լ�å��"));
			pEdit->EnableWindow(FALSE);
			
		}
	}
	*/


}

VOID CSePurchaseForm::OnbtnPURDEPARTMNG() 
{
	// TODO: Add your control notification handler code here
	if(!m_pDepartmentOrder) return;
	

	//�μ��� �ڷ����� 
	CString strPURCHASE_SPECIES_KEY;
	CString stPURCHASE_MANAGE_KEY;
	stPURCHASE_MANAGE_KEY = m_pDM->GetCellData(_T("SP_���԰���KEY"),0);

	m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
	m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;
	

	if(m_bIsDepartMngFirstShow) 
	{
		m_pDepartmentOrder->RefreshAll();
		m_bIsDepartMngFirstShow = FALSE;

	}


	INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt();
	m_pDepartmentOrder->ShowWindow(TRUE);
}

VOID CSePurchaseForm::SaveDepartmentOrderInfo()
{
	if(!m_pDepartmentOrder) return;
	
	//�μ��� �ڷ����� 
	CString strPURCHASE_SPECIES_KEY;
	CString stPURCHASE_MANAGE_KEY;
	stPURCHASE_MANAGE_KEY = m_pDM->GetCellData(_T("SP_���԰���KEY"),0);

	m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
	m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;


	if(m_pDepartmentOrder->m_nOpenMode>0)
	{
		INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt(FALSE);
		if(0 == nAcqExpBookCnt)
		{
			INT nACQ_BOOK_CNT = 1;	
			CString strACQ_BOOK_CNT;
			m_pCM->GetControlMgrData(_T("SP_�Լ�å��"),strACQ_BOOK_CNT);
			nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));

			m_pDepartmentOrder->InsertDefaultAcqBookCnt(nACQ_BOOK_CNT);
		}
	}


	INT ids = m_pDepartmentOrder->SaveAllData();

	//�������� �������� �Է��ϴ� ���
	if(m_nOpenMode != m_pDepartmentOrder->m_nOpenMode)
	{
		m_pDepartmentOrder->m_nOpenMode = m_nOpenMode;
	}

	
	m_pDepartmentOrder->RefreshAll();


	if(ids<0) AfxMessageBox(_T("�μ������ֹ����� ���忡 ���� �߽��ϴ�!"));
}

VOID CSePurchaseForm::OnDepartmentBookCntChange(WPARAM w,LPARAM l)
{
	if(!m_pDepartmentOrder) return;

	INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt();
	CString strCnt;
	CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("SP_�Լ�å��"));
	

	if(0 == nAcqExpBookCnt)
	{
		pEdit->EnableWindow(TRUE);
	}
	else
	{
		strCnt.Format(_T("%d"),nAcqExpBookCnt);
		m_pCM->SetControlMgrData(_T("SP_�Լ�å��"),strCnt);
		pEdit->EnableWindow(FALSE);
	}

}
