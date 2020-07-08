// SePurchaseSearch.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseSearch.h"
#include "..\SE_API\SePurchaseNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseSearch dialog


CSePurchaseSearch::CSePurchaseSearch(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchaseSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseSearch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pMgr = pParent;
	m_strQuery = _T("");
	m_strSEQ_NO = _T("");
	m_strSEQ_YEAR = _T("");
}

CSePurchaseSearch::~CSePurchaseSearch()
{
}

VOID CSePurchaseSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseSearch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseSearch, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseSearch)
	ON_BN_CLICKED(IDC_btnPUR_SEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_btnPUR_SEQ_NO, OnbtnPURSEQNO)
	ON_BN_CLICKED(IDC_btnPUR_SEARCH_CLEAR, OnbtnPURSEARCHCLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseSearch message handlers
BOOL CSePurchaseSearch::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSePurchaseSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		
	if(InitESL_Mgr( _T("����_����_�����ڷ����_�˻�") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_�����ڷ����_�˻�") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_����_�����ڷ����_�˻�"));
	
	if(!m_pCM ) {
		AfxMessageBox( _T("CM Error : ����_����_�����ڷ����_�˻�") );
		return false;
	}

	//�Է� ������ ���� ���ش�.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_INPUT_TYPE);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("ȭ��") );
		pCombo->InsertString( 1, _T("�ڷ����") );
		pCombo->InsertString( 2, _T("���Ϲ���") );
		pCombo->InsertString( 3, _T("��ü") );
		pCombo->SetCurSel( 0 );
	}

	//�ڷ���¸� �������ش�
	pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_WORKING_STATUS);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("���Դ��") );
		pCombo->InsertString( 1, _T("�ֹ���") );
		pCombo->InsertString( 2, _T("��ü") );
		pCombo->SetCurSel( 0 );
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSePurchaseSearch::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

CString CSePurchaseSearch::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = '%s'"),strFieldName,strData1);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

CString CSePurchaseSearch::MakeIntRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = %s"),strFieldName,strData1);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN %s AND %s"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}


VOID CSePurchaseSearch::OnbtnSEARCH()
{
	if(MakeQuery()<0) return;

	SendQueryToParent();

}

INT CSePurchaseSearch::MakeQuery()
{

	CString INPUT_DATE1 = _T("");				m_pCM->GetControlMgrData(_T("�Է�����1"),INPUT_DATE1);
	CString INPUT_DATE2 = _T("");				m_pCM->GetControlMgrData(_T("�Է�����2"),INPUT_DATE2);		
	CString ORDER_YEAR = _T("");				m_pCM->GetControlMgrData(_T("�����⵵"),ORDER_YEAR);
	CString END_DATE1 = _T("");					m_pCM->GetControlMgrData(_T("����������1"),END_DATE1);
	CString END_DATE2 = _T("");					m_pCM->GetControlMgrData(_T("����������2"),END_DATE2);
	CString SEQ_NO1 = _T("");					m_pCM->GetControlMgrData(_T("������ȣ1"),SEQ_NO1);

	
	INPUT_DATE1 = TrimAll(INPUT_DATE1);
	INPUT_DATE2 = TrimAll(INPUT_DATE2);
	ORDER_YEAR = TrimAll(ORDER_YEAR);
	END_DATE1 = TrimAll(END_DATE1);
	END_DATE2 = TrimAll(END_DATE2);
	SEQ_NO1 = TrimAll(SEQ_NO1);

	
	CString tmpquery = _T("");
	CString strQuery = _T("");


	//�������̺���� ���� ���� 
	strQuery = _T("S.ORDER_SEQ_NO = N.SEQ_NO AND S.ORDER_YEAR = N.ACQ_YEAR");


	//�Է±���
	CString INPUT_TYPE;
	INT nType = 0;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_INPUT_TYPE);
	if(pCombo) nType = pCombo->GetCurSel();
	
	
	if(nType != 3) //��ü�� �ƴϸ�  
	{
		strQuery += _T(" AND ");
		INPUT_TYPE.Format(_T("%d"),nType);
		tmpquery.Format(_T("S.INPUT_TYPE = '%s'"), INPUT_TYPE);
		strQuery += tmpquery;
	}

	

	//�۾�����
	pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_WORKING_STATUS);
	if(pCombo) nType = pCombo->GetCurSel();

	if(nType != 2) //��ü �� �ƴϸ� 
	{
		strQuery += _T(" AND ");
		INPUT_TYPE.Format(_T("%d"),nType);
		tmpquery.Format(_T("N.WORKING_STATUS = '%s'"), INPUT_TYPE);
		strQuery += tmpquery;

	}


	//�����⵵
	m_strSEQ_YEAR = _T("");


	if(!ORDER_YEAR.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T("S.ORDER_YEAR = '%s'"), ORDER_YEAR);
		strQuery += tmpquery;

		//�����⵵ ����
		m_strSEQ_YEAR = ORDER_YEAR;
	}
	
	
	//������ȣ(�ϴ��� ��ġ�˻�)
	m_strSEQ_NO = _T("");

	if(!SEQ_NO1.IsEmpty())
	{

		strQuery += _T(" AND ");
		tmpquery.Format(_T("S.ORDER_SEQ_NO = '%s'"), SEQ_NO1);
		strQuery += tmpquery;
	
		//���� ��ȣ ����
		m_strSEQ_NO = SEQ_NO1;
	}




	if(INPUT_DATE1.IsEmpty() && !INPUT_DATE2.IsEmpty())
	{
		AfxMessageBox(_T("�Է����� ������ �޺κи� �Է� �Ǿ����ϴ�!"));
		return -1;
	}



	//�Է���
	tmpquery.Empty();
	tmpquery = MakeRangeQuery(_T("S.INPUT_DATE"),INPUT_DATE1,INPUT_DATE2);
	if(!tmpquery.IsEmpty())
	{
		strQuery += _T(" AND ");
		strQuery += tmpquery;
	}

	
	if(END_DATE1.IsEmpty() && !END_DATE2.IsEmpty())
	{
		AfxMessageBox(_T("���������� ������ �޺κи� �Է� �Ǿ����ϴ�!"));
		return -1;
	}
	

	//����������
	tmpquery.Empty();
	tmpquery = MakeRangeQuery(_T("S.END_DATE"),END_DATE1,END_DATE2);
	if(!tmpquery.IsEmpty())
	{

		strQuery += _T(" AND ");
		strQuery += tmpquery;
	}


	m_strQuery = strQuery;
	
	return 0;

}

CString CSePurchaseSearch::TrimAll(CString strData)
{
	strData.TrimLeft();
	strData.TrimRight();

	return strData;
}

VOID CSePurchaseSearch::OnbtnPURSEQNO() 
{
	// TODO: Add your control notification handler code here
	// ���� ��ȣ ���� 
	CSePurchaseNoDlg dlg(this);
	dlg.m_nOpenMode = -1;

	if(dlg.DoModal() != IDOK) return;
	
	m_pCM->SetControlMgrData(_T("�����⵵"),dlg.m_strYear);
	m_pCM->SetControlMgrData(_T("������ȣ1"),dlg.m_strPurchaseNo);
	

	//���� �۾� ���� ���� 
	CString strWorking_status;
	strWorking_status = dlg.m_strWorking_status;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_WORKING_STATUS);
	if(pCombo)
	{
		if(_T("0") == strWorking_status) pCombo->SetCurSel(0);
		else if(_T("1") == strWorking_status) pCombo->SetCurSel(1);
	}

	//�Է� ������ ������ _T("��ü")�� ����
	pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_INPUT_TYPE);
	if(pCombo) pCombo->SetCurSel(3);
	

	//�ڵ� �˻�
	OnbtnSEARCH();

}

BOOL CSePurchaseSearch::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			OnbtnSEARCH();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSePurchaseSearch::OnbtnPURSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_INPUT_TYPE);
	if(pCombo)
	{
		pCombo->SetCurSel( 0 );
	}

	m_pCM->AllControlClear();
}

HBRUSH CSePurchaseSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
