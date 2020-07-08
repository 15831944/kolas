// TransferSearch.cpp : implementation file
//

#include "stdafx.h"
#include "TransferSearch.h"
#include "..\..\..\����\COMMON_DONATOR_MANAGER\DonatorNameGetterDlg.h"
#include "..\..\..\����\COMMON_DONATOR_MANAGER\DonatorNameGetter_Search.h"
#include "..\SE_API\SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransferSearch dialog


CTransferSearch::CTransferSearch(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferSearch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMgr = pParent;
	m_pCM = NULL;
	m_nACQ_TYPE = 1; //default
	m_strQuery = _T("");
	m_nSearchType = 0; //default

}

CTransferSearch::~CTransferSearch()
{
}

VOID CTransferSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferSearch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransferSearch, CDialog)
	//{{AFX_MSG_MAP(CTransferSearch)
	ON_BN_CLICKED(IDC_btnTRA_SEARCH_DONATOR, OnbtnTRASEARCHDONATOR)
	ON_BN_CLICKED(IDC_btnTRA_SEARCH_CLEAR, OnbtnTRASEARCHCLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferSearch message handlers
BOOL CTransferSearch::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CTransferSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("����_����_�ڷ��ΰ�_�˻�") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_�ڷ��ΰ�_�˻�") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_����_����_�ڷ��ΰ�_�˻�"));
	if(!m_pCM) 	{
		AfxMessageBox(_T("CM Error : CM_����_����_�ڷ��ΰ�_�˻�"));
		return FALSE;
	}		

	//���Ա��п� ���� ��Ʈ�� ����
	if(1 == m_nACQ_TYPE)
	{
		GetDlgItem(IDC_btnTRA_SEARCH_DONATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtTRA_SEARCH_DONATOR)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_cmbTRA_SEARCH_PROVIDER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcTRA_SEARCH_PROVIDER)->SetWindowText(_T("������"));
	}
	
	//�˻� ������ ���� ���ش�.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbTRA_SEARCH_TYPE);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("�ΰ����ڷ�") );
		pCombo->InsertString( 1, _T("�ΰ���ڷ�") );
		pCombo->SetCurSel(0);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CTransferSearch::MakeQuery()
{

	//å���� ����
	CString ACQ_YEAR = _T("");					m_pCM->GetControlMgrData(_T("���Գ⵵"),ACQ_YEAR);
	CString ACQ_DATE1 = _T("");					m_pCM->GetControlMgrData(_T("�Լ�����1"),ACQ_DATE1);
	CString ACQ_DATE2 = _T("");					m_pCM->GetControlMgrData(_T("�Լ�����2"),ACQ_DATE2);
	CString RECEIPT_NO1 = _T("");				m_pCM->GetControlMgrData(_T("������ȣ1"),RECEIPT_NO1);
	CString RECEIPT_NO2 = _T("");				m_pCM->GetControlMgrData(_T("������ȣ2"),RECEIPT_NO2);
	CString SHELF_LOC_CODE = _T("");			m_pCM->GetControlMgrData(_T("�ڷ��"),SHELF_LOC_CODE);
	

	//���� ���� ���� 
	//���� �� ����
	CString PROVIDER_CODE = _T("");				m_pCM->GetControlMgrData(_T("����ó"),PROVIDER_CODE);
	CString DONATOR_NAME = _T("");				m_pCM->GetControlMgrData(_T("������"),DONATOR_NAME);
	
	//����
	CString ISSN = _T("");
	CString ISSN1 = _T("");						m_pCM->GetControlMgrData(_T("ISSN1"),ISSN1);
	CString ISSN2 = _T("");						m_pCM->GetControlMgrData(_T("ISSN2"),ISSN2);		
	CString PUB_FREQ = _T("");					m_pCM->GetControlMgrData(_T("�����"),PUB_FREQ);
	
	CString TITLE1= _T("");						m_pCM->GetControlMgrData(_T("����1"),TITLE1);
	CString TITLE2= _T("");						m_pCM->GetControlMgrData(_T("����2"),TITLE2);
	CString PUBLISHER1 = _T("");				m_pCM->GetControlMgrData(_T("������1"),PUBLISHER1);
	CString PUBLISHER2 = _T("");				m_pCM->GetControlMgrData(_T("������2"),PUBLISHER2);
	
		
	PROVIDER_CODE = TrimAll(PROVIDER_CODE);
	DONATOR_NAME = TrimAll(DONATOR_NAME);
	ISSN1 = TrimAll(ISSN1);
	ISSN2 = TrimAll(ISSN2);
	PUB_FREQ = TrimAll(PUB_FREQ);
	TITLE1 = TrimAll(TITLE1);
	TITLE2 = TrimAll(TITLE2);
	PUBLISHER1 = TrimAll(PUBLISHER1);
	PUBLISHER2 = TrimAll(PUBLISHER2);


	//���� ���� �˻��� ��� 
	//���ʸ� �Է�  - [ IDX_ ] ���Ŀ� catsearch 
	//�����ʸ� �Է� - error!
	//���ʴ� �Է� - [ IDX_I ] ���Ŀ� ���� �˻� 
	
	CString strQuery = _T("");
	CString tmpquery = _T("");
	
	//���� ���� ���� 
	if(1 == m_nACQ_TYPE) {
		strQuery += _T("B.ACQ_CODE = '1'");
	} else if(2 == m_nACQ_TYPE) {
		strQuery += _T("B.ACQ_CODE = '2'");
	}

	//�߰�(5163)
	strQuery += _T(" AND I.REC_KEY = S.REC_KEY");
	
	//���� K2UP
	strQuery += _T(" AND I.REC_KEY = B.SPECIES_KEY");
	strQuery += _T(" AND V.REC_KEY = B.VOL_KEY");
	strQuery += _T(" AND A.REC_KEY = B.ACQ_TYPE_VOL_KEY");
	strQuery += _T(" AND B.BOOK_TYPE = 'C'");

	
	//�˻���������
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbTRA_SEARCH_TYPE);
	m_nSearchType = 0;
	if(pCombo) m_nSearchType = pCombo->GetCurSel();
	
	if(0 == m_nSearchType) //�ΰ� ��� �ڷ�
	{
		strQuery += _T(" AND B.WORKING_STATUS = 'SEL111O'");
		strQuery += _T(" AND B.TRANSFER_YN = 'N'");
	}
	else if(1 == m_nSearchType) //�ΰ�� �ڷ�(�ǽð� �ΰ�,�ǽð� �谡);
	{
		strQuery += _T(" AND (B.WORKING_STATUS = 'SEL111O' OR B.WORKING_STATUS = 'SEL112N')");
		strQuery += _T(" AND B.TRANSFER_YN = 'Y'");
	}

	if(RECEIPT_NO1.IsEmpty() && !RECEIPT_NO2.IsEmpty())
	{
		AfxMessageBox(_T("������ȣ ������ �޺κи� �Է� �Ǿ����ϴ�!"));
		return -1;
	}


	//������ȣ
	tmpquery.Empty();
	tmpquery = MakeIntRangeQuery(_T("B.RECEIPT_NO"),RECEIPT_NO1,RECEIPT_NO2);
	if(!tmpquery.IsEmpty())
	{
		strQuery += _T(" AND ");
		strQuery += tmpquery;
	}
	
	
	//�Լ�����
	if(ACQ_DATE1.IsEmpty() && !ACQ_DATE2.IsEmpty())
	{
		AfxMessageBox(_T("�Լ����� ������ �޺κи� �Է� �Ǿ����ϴ�!"));
		return -2;
	}
	

	tmpquery.Empty();
	tmpquery = MakeFitAndRangeQuery(_T("B.ACQ_DATE"),ACQ_DATE1,ACQ_DATE2);
	if(!tmpquery.IsEmpty())
	{
		strQuery += _T(" AND ");
		strQuery += tmpquery;
	}
	


	if(!ACQ_YEAR.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T("B.ACQ_YEAR = '%s'"), ACQ_YEAR);
		strQuery += tmpquery;
	}
	
	
	if(!SHELF_LOC_CODE.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T("B.SHELF_LOC_CODE = '%s'"), SHELF_LOC_CODE);
		strQuery += tmpquery;
	}
	

	if(TITLE1.IsEmpty() && !TITLE2.IsEmpty()) {
		AfxMessageBox(_T("ERROR!: ���� �˻��� ������ �׸����� �� �� �����ϴ�"));
		return -1;
	}

	if(PUBLISHER1.IsEmpty() && !PUBLISHER2.IsEmpty()) {
		AfxMessageBox(_T("ERROR!: ������ �˻��� ������ �׸����� �� �� �����ϴ�"));
		return -2;
	}


	//MakeIDXRangeQuery
	if(!TITLE1.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery = MakeIDXRangeQuery(_T("TITLE"), TITLE1, TITLE2);
		strQuery += tmpquery;
	}

	if(!PUBLISHER1.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery = MakeIDXRangeQuery(_T("PUBLISHER"), PUBLISHER1, PUBLISHER2);
		strQuery += tmpquery;
	}


	//��ġ �˻� 
	if(!PUB_FREQ.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T(" I.PUB_FREQ = '%s'"), PUB_FREQ);
		strQuery += tmpquery;
	}
	
	//ISSN
	if(!ISSN1.IsEmpty())
	{
		if(!ISSN2.IsEmpty())
		{
			ISSN.Format(_T("%s-%s"),ISSN1,ISSN2);
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" I.ST_ISSN = '%s'"), ISSN);
			strQuery += tmpquery;
		}

	}

	m_strQuery = strQuery;

	return 0;

}

CString CTransferSearch::MakeIDXRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	
	CString tmpquery;
	CSeApi api(this);

	if(strData2.IsEmpty()) // catsearch
	{ 
		api.MakeIndexWord(strData1);

if(ORA_CATSEARCH == m_pInfo->GetSearchIndexType())
		tmpquery.Format(_T(" CATSEARCH(I.IDX_%s, '%s*', NULL)>0"),strFieldName, strData1);
else		
		tmpquery.Format(_T("&ECOSEARCH(I.REC_KEY,SE_IDX_%s, '%s*')"),strFieldName, strData1);

		
	}
	else // between
	{

		api.MakeIndexWord(strData1);
		api.MakeIndexWord(strData2);
		tmpquery.Format(_T("I.IDX_I%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}
	
	return tmpquery;
}

CString CTransferSearch::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData2.IsEmpty()) nOption = 2;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s >= '%s'"),strFieldName,strData1);
		if(nOption == 2) tmpquery.Format(_T("%s <= '%s'"),strFieldName,strData2);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

CString CTransferSearch::MakeIntRangeQuery(CString strFieldName, CString strData1, CString strData2)
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

CString CTransferSearch::MakeFitAndRangeQuery(CString strFieldName, CString strData1, CString strData2)
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

CString CTransferSearch::TrimAll(CString strData)
{
	strData.TrimLeft();
	strData.TrimRight();

	return strData;
}

VOID CTransferSearch::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

VOID CTransferSearch::OnbtnTRASEARCHDONATOR() 
{
	// TODO: Add your control notification handler code here
	CDonatorNameGetter_Search searchDlg(this);
	searchDlg.SetOpenMode(-1000);
	if(IDOK == searchDlg.DoModal())
	{
		CDonatorNameGetterDlg searchResult(this);
		if(IDOK == searchResult.DoModal())
		{
			m_pCM->SetControlMgrData(_T("������"),searchResult.GetDonatorName());
		}
	}
}

VOID CTransferSearch::OnOK() 
{
	// TODO: Add extra validation here
	
	if(MakeQuery()<0) return;

	SendQueryToParent();

	return;

	CDialog::OnOK();
}

VOID CTransferSearch::OnbtnTRASEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();
}

BOOL CTransferSearch::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			OnOK(); 
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


VOID CTransferSearch::SetFocusOnTitle()
{
	//���� setfocus
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_edtTRA_SEARCH_TITLE1);
	if(pWnd) pWnd->SetFocus();
}
HBRUSH CTransferSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
