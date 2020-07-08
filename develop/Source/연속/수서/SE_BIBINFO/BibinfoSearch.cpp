// BibinfoSearch.cpp : implementation file
//

#include "stdafx.h"
#include "BibinfoSearch.h"
#include "..\SE_API\SeApi.h"
#include "..\..\..\����\COMMON_DONATOR_MANAGER\DonatorNameGetterDlg.h"
#include "..\..\..\����\COMMON_DONATOR_MANAGER\DonatorNameGetter_Search.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBibinfoSearch dialog


CBibinfoSearch::CBibinfoSearch(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBibinfoSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBibinfoSearch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pMgr = pParentMgr;
	m_strQuery = _T("");
	m_nACQ_TYPE = 1; //default ����
	m_nSearchAcqType = -1; //default
	
}

CBibinfoSearch::~CBibinfoSearch()
{
}


VOID CBibinfoSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBibinfoSearch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBibinfoSearch, CDialog)
	//{{AFX_MSG_MAP(CBibinfoSearch)
	ON_BN_CLICKED(IDC_btnBIB_MORE, OnbtnBIBMORE)
	ON_CBN_SELCHANGE(IDC_cmbBIB_ACQ_TYPE, OnSelchangecmbBIBACQTYPE)
	ON_BN_CLICKED(IDC_btnBIB_DONATOR_SEARCH, OnbtnBIBDONATORSEARCH)
	ON_BN_CLICKED(IDC_btnBIB_SEARCH_CLEAR, OnbtnBIBSEARCHCLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBibinfoSearch message handlers

BOOL CBibinfoSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();


	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("����_����_������������_�˻�") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_������������_�˻�") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_����_������������_�˻�"));
	if(!m_pCM) 	{
		AfxMessageBox(_T("CM Error : CM_����_������������_�˻�"));
		return FALSE;
	}		
	

	GetWindowRect(m_rcWindow);
	m_bShowMode = false;
	ShowControlByShowMode();
	

	if(1 == m_nACQ_TYPE)
	{
		m_pCM->SetControlMgrData(_T("���Ա���"),_T("1"));
	}
	else if(2 == m_nACQ_TYPE) 
	{
		m_pCM->SetControlMgrData(_T("���Ա���"),_T("2"));
	}

	OnSelchangecmbBIBACQTYPE(); 

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBibinfoSearch::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CBibinfoSearch::ShowControlByShowMode()
{
	INT i = 0;
	INT nCodeResourcesHeight = 180;
	CWnd *pWnd = NULL;

	if (m_bShowMode)
	{
		GetDlgItem(IDC_btnBIB_MORE)->SetWindowText(_T("�����˻�"));
		this->SetWindowText(_T("�󼼰˻�"));
	}
	else
	{
		GetDlgItem(IDC_btnBIB_MORE)->SetWindowText(_T("�󼼰˻�"));
		this->SetWindowText(_T("�����˻�"));
	}

	CRect rcWindow;
	GetWindowRect(rcWindow);
	if (m_bShowMode && rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	
	//control
	nArrCodeResourceID[nCount++] = IDC_cmbBIB_MAT_CODE;
	nArrCodeResourceID[nCount++] = IDC_cmbBIB_FORM_CODE;
	nArrCodeResourceID[nCount++] = IDC_cmbBIB_MEDIA_CODE;
	nArrCodeResourceID[nCount++] = IDC_cmbBIB_CLAIM_YN;
	nArrCodeResourceID[nCount++] = IDC_cmbBIB_BINDING_YN;
	nArrCodeResourceID[nCount++] = IDC_edtBIB_CONTROL_NO1;
	nArrCodeResourceID[nCount++] = IDC_edtBIB_CONTROL_NO2;
	nArrCodeResourceID[nCount++] = IDC_edtBIB_START_DATE;
	nArrCodeResourceID[nCount++] = IDC_edtBIB_END_DATE;
	
	//static
	nArrCodeResourceID[nCount++] = IDC_stcBIB_HIDE1;
	nArrCodeResourceID[nCount++] = IDC_stcBIB_HIDE2;
	nArrCodeResourceID[nCount++] = IDC_stcBIB_HIDE3;
	

	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (m_bShowMode)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	// 2005-04-04 SM5ong
	OnSelchangecmbBIBACQTYPE();

	// Resize Window Size
	if (m_bShowMode)
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	
	//Invalidate();
	UpdateWindow();
	
	
}

VOID CBibinfoSearch::OnbtnBIBMORE() 
{
	// TODO: Add your control notification handler code here
	if(m_bShowMode) 
		m_bShowMode = false;
	else
		m_bShowMode = true;
	
	ShowControlByShowMode();
}

INT CBibinfoSearch::MakeQuery()
{

	//���� �� ����
	CString PROVIDER_CODE = _T("");				m_pCM->GetControlMgrData(_T("����ó"),PROVIDER_CODE);
	CString DONATOR_NAME = _T("");				m_pCM->GetControlMgrData(_T("������"),DONATOR_NAME);
	
	//����
	CString ISSN = _T("");
	CString ISSN1 = _T("");						m_pCM->GetControlMgrData(_T("ISSN1"),ISSN1);
	CString ISSN2 = _T("");						m_pCM->GetControlMgrData(_T("ISSN2"),ISSN2);		
	CString PUB_FREQ = _T("");					m_pCM->GetControlMgrData(_T("�����"),PUB_FREQ);
	CString PUB_STATUS = _T("");				m_pCM->GetControlMgrData(_T("�������"),PUB_STATUS);

	CString TITLE1= _T("");						m_pCM->GetControlMgrData(_T("����1"),TITLE1);
	CString TITLE2= _T("");						m_pCM->GetControlMgrData(_T("����2"),TITLE2);
	CString PUBLISHER1 = _T("");				m_pCM->GetControlMgrData(_T("������1"),PUBLISHER1);
	CString PUBLISHER2 = _T("");				m_pCM->GetControlMgrData(_T("������2"),PUBLISHER2);
	
	CString CONTROL_NO1 = _T("");				m_pCM->GetControlMgrData(_T("�����ȣ1"),CONTROL_NO1);
	CString CONTROL_NO2 = _T("");				m_pCM->GetControlMgrData(_T("�����ȣ2"),CONTROL_NO2);

	CString MEDIA_CODE = _T("");				m_pCM->GetControlMgrData(_T("��ü����"),MEDIA_CODE);
	CString FORM_CODE = _T("");					m_pCM->GetControlMgrData(_T("���ı���"),FORM_CODE);
	CString MAT_CODE = _T("");					m_pCM->GetControlMgrData(_T("�ڷᱸ��"),MAT_CODE);
	
	CString START_DATE = _T("");				m_pCM->GetControlMgrData(_T("������"),START_DATE);
	CString END_DATE = _T("");					m_pCM->GetControlMgrData(_T("������"),END_DATE);

	//2005-12-28 Ȳ���� : �Է����� �����߰�
	CString INPUT_DATE1 = _T("");				m_pCM->GetControlMgrData(_T("�Է���1"),INPUT_DATE1);
	CString INPUT_DATE2 = _T("");				m_pCM->GetControlMgrData(_T("�Է���2"),INPUT_DATE2);
	
	
	PROVIDER_CODE = TrimAll(PROVIDER_CODE);
	DONATOR_NAME = TrimAll(DONATOR_NAME);
	ISSN1 = TrimAll(ISSN1);
	ISSN2 = TrimAll(ISSN2);
	PUB_FREQ = TrimAll(PUB_FREQ);
	TITLE1 = TrimAll(TITLE1);
	TITLE2 = TrimAll(TITLE2);
	PUBLISHER1 = TrimAll(PUBLISHER1);
	PUBLISHER2 = TrimAll(PUBLISHER2);
	CONTROL_NO1 = TrimAll(CONTROL_NO1);
	CONTROL_NO2 = TrimAll(CONTROL_NO2);
	MEDIA_CODE = TrimAll(MEDIA_CODE);
	FORM_CODE = TrimAll(FORM_CODE);
	MAT_CODE = TrimAll(MAT_CODE);
	


	//���� ���� �˻��� ��� 
	//���ʸ� �Է�  - [ IDX_ ] ���Ŀ� catsearch 
	//�����ʸ� �Է� - error!
	//���ʴ� �Է� - [ IDX_I ] ���Ŀ� ���� �˻� 

	
	//���� ���� ���� 
	CString strQuery = _T("");
	CString strTmp,tmpquery;
	m_pCM->GetControlMgrData(_T("���Ա���"),strTmp);
	strTmp.TrimLeft();	
	strTmp.TrimRight(); 
	
	strQuery += _T("I.REC_KEY = S.REC_KEY");

	if(_T("1") == strTmp) //���� 
	{ 
		m_nSearchAcqType = 1;
		
		strQuery += _T(" AND ");
		strQuery += _T("I.REC_KEY = P.SPECIES_KEY");
		if(!PROVIDER_CODE.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" P.PROVIDER_CODE = '%s'"), PROVIDER_CODE);
			strQuery += tmpquery;
		}

	}
	else if(_T("2") == strTmp) //���� 
	{ 
		m_nSearchAcqType = 2;
		
		strQuery += _T(" AND ");
		strQuery += _T("I.REC_KEY = D.SPECIES_KEY");
		
		if(!DONATOR_NAME.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" D.DONATOR_NAME = '%s'"), DONATOR_NAME);
			strQuery += tmpquery;
		}

	}
	else if(strTmp.IsEmpty()) //�������
	{
		m_nSearchAcqType = 3;
	}
	else
	{
		return -1;

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
	
	if(!PUB_STATUS.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T(" I.PUB_STATUS = '%s'"), PUB_STATUS);
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


	//////�󼼰˻� OPTION
	if(m_bShowMode) 
	{
		tmpquery.Empty();
		tmpquery = MakeRangeQuery(_T("I.CONTROL_NO"),CONTROL_NO1,CONTROL_NO2);
		if(!tmpquery.IsEmpty()) {
			strQuery += _T(" AND ");
			strQuery += tmpquery;
		}
		

		if(!MEDIA_CODE.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" I.MEDIA_CODE = '%s'"), MEDIA_CODE);
			strQuery += tmpquery;
		}

		if(!FORM_CODE.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" I.FORM_CODE = '%s'"), FORM_CODE);
			strQuery += tmpquery;
		}
		
		if(!MAT_CODE.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" I.MAT_CODE = '%s'"), MAT_CODE);
			strQuery += tmpquery;
		}

		//�����Ⱓ �˻�
		if(_T("1") == strTmp) {
		
			if(!START_DATE.IsEmpty()) //���������� 
			{
				strQuery += _T(" AND ");
				tmpquery.Format(_T(" P.END_DATE >= '%s'"), START_DATE);
				strQuery += tmpquery;
			}
			
			if(!END_DATE.IsEmpty()) //���� ������ 
			{
				strQuery += _T(" AND ");
				tmpquery.Format(_T(" P.START_DATE <= '%s'"), END_DATE);
				strQuery += tmpquery;
			}


		} else if(_T("2") == strTmp) { 

	
			if(!START_DATE.IsEmpty()) //���������� 
			{
				strQuery += _T(" AND ");
				tmpquery.Format(_T(" D.START_DATE >= '%s'"), START_DATE);
				strQuery += tmpquery;
			}
		
		}

		//�Է��� �˻� �߰�
		if(!INPUT_DATE1.IsEmpty()) //���������� 
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" S.INPUT_DATE >= '%s'"), INPUT_DATE1);
			strQuery += tmpquery;
		}
		
		if(!INPUT_DATE2.IsEmpty()) //���� ������ 
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" S.INPUT_DATE <= '%s'"), INPUT_DATE2);
			strQuery += tmpquery;
		}
	
	}
	
	
	m_strQuery = strQuery;



	return 0;

}

CString CBibinfoSearch::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
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

CString CBibinfoSearch::MakeIDXRangeQuery(CString strFieldName, CString strData1, CString strData2)
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



CString CBibinfoSearch::TrimAll(CString strData)
{
	strData.TrimLeft();
	strData.TrimRight();

	return strData;
}

VOID CBibinfoSearch::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

VOID CBibinfoSearch::OnOK() 
{
	// TODO: Add extra validation here

	if(MakeQuery()<0) return;

	SendQueryToParent();

	return;
	CDialog::OnOK();
}

VOID CBibinfoSearch::OnSelchangecmbBIBACQTYPE() 
{
	// TODO: Add your control notification handler code here
	
	CString strTmp;
	m_pCM->GetControlMgrData(_T("���Ա���"),strTmp);
	strTmp.TrimLeft();	
	strTmp.TrimRight(); 

	if( _T("1") == strTmp) { //���� 

		
		GetDlgItem(IDC_cmbBIB_PROVDER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtBIB_END_DATE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcACQ_TYPE)->SetWindowText(_T("����ó"));
		GetDlgItem(IDC_stcDATE)->SetWindowText(_T("�����Ⱓ"));
		

		GetDlgItem(IDC_edtBIB_DONATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnBIB_DONATOR_SEARCH)->ShowWindow(SW_HIDE);
			
		GetDlgItem(IDC_edtBIB_START_DATE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtBIB_END_DATE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcBIB_HIDE2)->ShowWindow(SW_SHOW);


	} else if( _T("2") == strTmp) { //����


		GetDlgItem(IDC_cmbBIB_PROVDER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBIB_END_DATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcACQ_TYPE)->SetWindowText(_T("������"));
		GetDlgItem(IDC_stcDATE)->SetWindowText(_T("������"));
		GetDlgItem(IDC_stcBIB_HIDE2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_edtBIB_DONATOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnBIB_DONATOR_SEARCH)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_edtBIB_START_DATE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtBIB_END_DATE)->ShowWindow(SW_SHOW);

		
	} 
	else  //������� 
	{


		GetDlgItem(IDC_cmbBIB_PROVDER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBIB_END_DATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcACQ_TYPE)->SetWindowText(_T(""));
		GetDlgItem(IDC_stcDATE)->SetWindowText(_T(""));
		GetDlgItem(IDC_edtBIB_DONATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnBIB_DONATOR_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBIB_START_DATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBIB_END_DATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcBIB_HIDE2)->ShowWindow(SW_HIDE);


	}

}

VOID CBibinfoSearch::OnbtnBIBDONATORSEARCH() 
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

VOID CBibinfoSearch::OnbtnBIBSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();

	if(1 == m_nACQ_TYPE)
	{
		m_pCM->SetControlMgrData(_T("���Ա���"),_T("1"));
	}
	else if(2 == m_nACQ_TYPE) 
	{
		m_pCM->SetControlMgrData(_T("���Ա���"),_T("2"));
	}

}

BOOL CBibinfoSearch::PreTranslateMessage(MSG* pMsg) 
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

VOID CBibinfoSearch::SetFocusOnTitle()
{
	//���� setfocus
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_edtBIB_TITLE1);
	if(pWnd) pWnd->SetFocus();
}
HBRUSH CBibinfoSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
