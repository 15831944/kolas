// SearchSerial.cpp : implementation file
//

#include "stdafx.h"
//#include "IndexGenerator.h"
#include "SearchSerial.h"
#include "..\..\..\����\����\SE_API\SeApi.h"
#include "..\..\..\����\COMMON_DONATOR_MANAGER\DonatorNameGetterDlg.h"
#include "..\..\..\����\COMMON_DONATOR_MANAGER\DonatorNameGetter_Search.h"
#include "..\..\..\����\����\SE_API\SePurchaseNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchSerial dialog


CSearchSerial::CSearchSerial(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchSerial)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParentMgr = pParent;
	m_pCM = NULL;
	m_Create = FALSE;
	m_strQuery = _T("");
	m_nACQ_TYPE = 1; //default ����
	m_nSearchAcqType = -1; //default
//	m_pcmbIndexProvder = NULL;
//	m_pcmbIndexAcqType = NULL;
//	m_pcmbIndexFormCode = NULL;
//	m_pcmbIndexMediaCode = NULL;
//	m_pcmbIndexMatCode = NULL;
}

CSearchSerial::~CSearchSerial()
{
//	if( NULL != m_pcmbIndexProvder )
//	{
//		delete m_pcmbIndexProvder;
//		m_pcmbIndexProvder = NULL;
//	}
//
//	if( NULL != m_pcmbIndexAcqType )
//	{
//		delete m_pcmbIndexAcqType;
//		m_pcmbIndexAcqType = NULL;
//	}
//
//	if( NULL != m_pcmbIndexFormCode )
//	{
//		delete m_pcmbIndexFormCode;
//		m_pcmbIndexFormCode = NULL;
//	}
//
//	if( NULL != m_pcmbIndexMediaCode )
//	{
//		delete m_pcmbIndexMediaCode;
//		m_pcmbIndexMediaCode = NULL;
//	}
//
//	if( NULL != m_pcmbIndexMatCode )
//	{
//		delete m_pcmbIndexMatCode;
//		m_pcmbIndexMatCode = NULL;
//	}

}

VOID CSearchSerial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchSerial)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchSerial, CDialog)
	//{{AFX_MSG_MAP(CSearchSerial)
	ON_CBN_SELCHANGE(IDC_cmbINDEX_ACQ_TYPE, OnSelchangecmbINDEXACQTYPE)
	ON_BN_CLICKED(IDC_btnINDEX_SEARCH_CLEAR, OnbtnINDEXSEARCHCLEAR)
	ON_BN_CLICKED(IDC_btnINDEX_DONATOR_SEARCH, OnbtnINDEXDONATORSEARCH)
	ON_BN_CLICKED(IDC_btnPURSEQNO, OnbtnPURSEQNO)
	ON_WM_CTLCOLOR() 
	ON_BN_CLICKED(IDC_btnSerialClose, OnbtnSerialClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchSerial message handlers
BOOL CSearchSerial::Create(CWnd *pParentWnd)
{
	m_Create = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSearchSerial::Init()
{
	//===================================================
	//2010.06.01 ADD BY PJW : �޺��ڽ� �ʱ�ȭ

//	CString strClassName;
//
//	strClassName.Format(_T("���Ա���"));
//	SetCodeControl( m_pcmbIndexAcqType, IDC_cmbINDEX_ACQ_TYPE, strClassName );
//
//	strClassName.Format(_T("����ó����"));
//	SetCodeControl(m_pcmbIndexProvder, IDC_cmbINDEX_PROVDER, strClassName);
//
//	strClassName.Format(_T("���ı���"));
//	SetCodeControl(m_pcmbIndexFormCode, IDC_cmbINDEX_FORM_CODE, strClassName);
//
//	strClassName.Format(_T("��ü����"));
//	SetCodeControl(m_pcmbIndexMediaCode, IDC_cmbINDEX_MEDIA_CODE, strClassName);
//
//	strClassName.Format(_T("�ڷᱸ��"));
//	SetCodeControl(m_pcmbIndexMatCode, IDC_cmbINDEX_MAT_CODE, strClassName);
	//===================================================

//	CESL_ControlMultiComboBox* pcmbIndexAcqType = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbINDEX_ACQ_TYPE);
	if(1 == m_nACQ_TYPE)
	{
		
//		pcmbIndexAcqType->SelectItem(_T("1"));
//		INT nIdx = m_pcmbIndexAcqType->GetCurSel();
//		INT nIdx = pMultiComboBox->SetCurSel(0);
//		pcmbIndexAcqType->Select(_T("1"));
		m_pCM->SetControlMgrData(_T("���Ա���"),_T("1"));
	}
	else if(2 == m_nACQ_TYPE) 
	{
//		pcmbIndexAcqType->SelectItem(_T("1"));
//		m_pcmbIndexAcqType->SelectItem(_T("2"));
		m_pCM->SetControlMgrData(_T("���Ա���"),_T("2"));
	}

	OnSelchangecmbINDEXACQTYPE();	
	return TRUE;
}

BOOL CSearchSerial::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// TODO: Add extra initialization here
	m_pControlDm.SetCONNECTION_INFO(m_strConnectInfo);
	if (InitESL_Mgr(_T("SM_IndexGeneratorSearch_Serial")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	m_pCM = FindCM(_T("CM_IndexGeneratorSearch_Serial"));
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("����_��������� �˻� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	GetWindowRect(m_rcWindow);

	m_pCM->AllControlDisplay();
//	m_bShowMode = false;
//	m_bShowMode = true;
	ShowControlByShowMode();

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSearchSerial::ShowControlByShowMode()
{
	INT i = 0;
	INT nCodeResourcesHeight = 150;
	CWnd *pWnd = NULL;

//	if (m_bShowMode) {
//		GetDlgItem(IDC_btnINDEX_MORE)->SetWindowText(_T("�����˻�"));
//		this->SetWindowText(_T("�󼼰˻�"));
//	} else {
//		GetDlgItem(IDC_btnINDEX_MORE)->SetWindowText(_T("�󼼰˻�"));
//		this->SetWindowText(_T("�����˻�"));
//	}

//	CRect rcWindow;
//	GetWindowRect(rcWindow);
//	if (m_bShowMode && rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	
	//control
	nArrCodeResourceID[nCount++] = IDC_cmbINDEX_MAT_CODE;
	nArrCodeResourceID[nCount++] = IDC_cmbINDEX_FORM_CODE;
	nArrCodeResourceID[nCount++] = IDC_cmbINDEX_MEDIA_CODE;
//	nArrCodeResourceID[nCount++] = IDC_cmbINDEX_CLAIM_YN;
//	nArrCodeResourceID[nCount++] = IDC_cmbINDEX_BINDING_YN;
//	nArrCodeResourceID[nCount++] = IDC_edtINDEX_CONTROL_NO1;   //�����ȣ ���� �ű�.
//	nArrCodeResourceID[nCount++] = IDC_edtINDEX_CONTROL_NO2;
	nArrCodeResourceID[nCount++] = IDC_edtINDEX_START_DATE;
	nArrCodeResourceID[nCount++] = IDC_edtINDEX_END_DATE;
	
	//static
//	nArrCodeResourceID[nCount++] = IDC_stcINDEX_HIDE1;
	nArrCodeResourceID[nCount++] = IDC_stcINDEX_HIDE2;
//	nArrCodeResourceID[nCount++] = IDC_stcINDEX_HIDE3;
	

	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
//		if (pWnd->GetSafeHwnd() != NULL)
//		{
//			if (m_bShowMode)
//				pWnd->ShowWindow(SW_SHOW);
//			else
//				pWnd->ShowWindow(SW_HIDE);
//		}
		pWnd->ShowWindow(SW_SHOW);
	}

	// Resize Window Size
//	if (m_bShowMode)
//		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
//	else
//		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
//	
	//Invalidate();
	UpdateWindow();
	ShowCtrlByAcqType();

}

//VOID CSearchSerial::OnbtnINDEXMORE() 
//{
//	// TODO: Add your control notification handler code here
////	if(m_bShowMode) ~
////		m_bShowMode = false;
////	else
////		m_bShowMode = true;
//	m_bShowMode = true;
//	
//	ShowControlByShowMode();	
//}

VOID CSearchSerial::OnSelchangecmbINDEXACQTYPE() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ShowCtrlByAcqType();
	
}
VOID CSearchSerial::ShowCtrlByAcqType()
{
	CString strTmp;
//	m_pCM->GetControlMgrData(_T("���Ա���"),strTmp);
//	strTmp.TrimLeft();	
//	strTmp.TrimRight(); 
	CESL_ControlMultiComboBox* pcmbIndexAcqType = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbINDEX_ACQ_TYPE);
	INT nIdx = pcmbIndexAcqType->GetCurSel();
	pcmbIndexAcqType->GetLBText(nIdx, strTmp);

	if( _T("1") == strTmp) { //���� 

	
		GetDlgItem(IDC_cmbINDEX_PROVDER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtINDEX_END_DATE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcACQ_TYPE)->SetWindowText(_T("����ó"));
		GetDlgItem(IDC_stcACQ_YEAR)->SetWindowText(_T("�����⵵"));
		GetDlgItem(IDC_edtACQYEAR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcACQ_NO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtACQ_NO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcINDEX_DATE)->SetWindowText(_T("�����Ⱓ"));
		

		GetDlgItem(IDC_edtINDEX_DONATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnINDEX_DONATOR_SEARCH)->ShowWindow(SW_HIDE);
			
		GetDlgItem(IDC_edtINDEX_START_DATE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtINDEX_END_DATE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcINDEX_HIDE2)->ShowWindow(SW_SHOW);


	} else if( _T("2") == strTmp) { //����


		GetDlgItem(IDC_cmbINDEX_PROVDER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtINDEX_END_DATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcACQ_TYPE)->SetWindowText(_T(""));
		GetDlgItem(IDC_stcACQ_YEAR)->SetWindowText(_T("������"));
		GetDlgItem(IDC_edtACQYEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcACQ_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtACQ_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcINDEX_DATE)->SetWindowText(_T("������"));
		GetDlgItem(IDC_stcINDEX_HIDE2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_edtINDEX_DONATOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnINDEX_DONATOR_SEARCH)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_edtINDEX_START_DATE)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_edtINDEX_END_DATE)->ShowWindow(SW_SHOW);

		
	} 
	else  //�������, ����, ��Ȱ�� �� 
	{
		GetDlgItem(IDC_cmbINDEX_PROVDER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtINDEX_END_DATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcACQ_TYPE)->SetWindowText(_T(""));
		GetDlgItem(IDC_stcACQ_YEAR)->SetWindowText(_T(""));
		GetDlgItem(IDC_edtACQYEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcACQ_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtACQ_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcINDEX_DATE)->SetWindowText(_T(""));
		GetDlgItem(IDC_edtINDEX_DONATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnINDEX_DONATOR_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtINDEX_START_DATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtINDEX_END_DATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcINDEX_HIDE2)->ShowWindow(SW_HIDE);
	}
}

VOID CSearchSerial::OnbtnINDEXSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();

	CESL_ControlMultiComboBox* pcmbIndexAcqType = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbINDEX_ACQ_TYPE);

	if(1 == m_nACQ_TYPE){
		pcmbIndexAcqType->SelectItem(_T("1"));
	}
	else if(2 == m_nACQ_TYPE){
		pcmbIndexAcqType->SelectItem(_T("2"));
	}
	ShowCtrlByAcqType();
	
}

VOID CSearchSerial::OnOK() 
{
	// TODO: Add extra validation here
	if(MakeQuery()<0) return;

	SendQueryToParent();

	return;
	CDialog::OnOK();
}
VOID CSearchSerial::SendQueryToParent()
{
//	:: SendMessage(	m_pParentMgr->GetSafeHwnd(), QUERYCREATE , 0, 0);
}
INT CSearchSerial::MakeQuery()
{

	//���� �� ����
	CString PROVIDER_CODE = _T("");				m_pCM->GetControlMgrData(_T("����ó"),PROVIDER_CODE);
	CString DONATOR_NAME = _T("");				m_pCM->GetControlMgrData(_T("������"),DONATOR_NAME);
	
	CString ACQ_YEAR = _T("");					m_pCM->GetControlMgrData(_T("�����⵵"),ACQ_YEAR);
	CString ACQ_NO= _T("");						m_pCM->GetControlMgrData(_T("������ȣ"),ACQ_NO);

	//����
	CString CONTROL_NO1 = _T("");				m_pCM->GetControlMgrData(_T("�����ȣ1"),CONTROL_NO1);
	CString CONTROL_NO2 = _T("");				m_pCM->GetControlMgrData(_T("�����ȣ2"),CONTROL_NO2);
	
	CString MEDIA_CODE = _T("");				m_pCM->GetControlMgrData(_T("��ü����"),MEDIA_CODE);
	CString FORM_CODE = _T("");					m_pCM->GetControlMgrData(_T("���ı���"),FORM_CODE);
	CString MAT_CODE = _T("");					m_pCM->GetControlMgrData(_T("�ڷᱸ��"),MAT_CODE);
	
	CString START_DATE = _T("");				m_pCM->GetControlMgrData(_T("������"),START_DATE);
	CString END_DATE = _T("");					m_pCM->GetControlMgrData(_T("������"),END_DATE);
	
	
	PROVIDER_CODE = TrimAll(PROVIDER_CODE);
	DONATOR_NAME = TrimAll(DONATOR_NAME);
	ACQ_YEAR = TrimAll(ACQ_YEAR);
	ACQ_NO = TrimAll(ACQ_NO);
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
		strQuery += _T("I.REC_KEY = P.SPECIES_KEY AND I.REC_KEY = SP.SPECIES_KEY");

		// ����ó
		if(!PROVIDER_CODE.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" P.PROVIDER_CODE = '%s'"), PROVIDER_CODE);
			strQuery += tmpquery;
		}

		// �����⵵
		if(!ACQ_YEAR.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" SP.ORDER_YEAR = '%s'"), ACQ_YEAR);
			strQuery += tmpquery;
		}
		
		// ������ȣ
		if(!ACQ_NO.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" SP.ORDER_SEQ_NO = %s"), ACQ_NO);
			strQuery += tmpquery;
		}
	}
	else if(_T("2") == strTmp) //���� 
	{ 
		m_nSearchAcqType = 2;
		
		strQuery += _T(" AND ");
		strQuery += _T("I.REC_KEY = D.SPECIES_KEY");
		
		// ������
		if(!DONATOR_NAME.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" D.DONATOR_NAME = '%s'"), DONATOR_NAME);
			strQuery += tmpquery;
		}

	}
/*	else if(strTmp.IsEmpty()){ //�������
		m_nSearchAcqType = 3;
	}
	else{
		return -1;
	}
*/
	// 2004.10.13 ������� ���� �׿ܴ� m_nSearchAcqType = 3
	else m_nSearchAcqType = 3;

	// �����ȣ
	tmpquery = MakeRangeQuery(_T("I.CONTROL_NO"),CONTROL_NO1,CONTROL_NO2);
	if(!tmpquery.IsEmpty()) {
		if(tmpquery == _T("ERROR")) return -3;	
		strQuery += _T(" AND ");
		strQuery += tmpquery;
	}

/*	//��ġ �˻� 
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

*/
	//////�󼼰˻� OPTION
//	if(m_bShowMode) 
//	{
		tmpquery.Empty();
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
//		
//	
//	}
	
	
	m_strQuery = strQuery;



	return 0;

}

CString CSearchSerial::TrimAll(CString strData)
{
	strData.TrimLeft();
	strData.TrimRight();

	return strData;
}
CString CSearchSerial::MakeIDXRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery;
	if(strData1.IsEmpty() && !strData2.IsEmpty())
	{
		AfxMessageBox(_T("ERROR!: �����˻����� �����ʹ� ���ڸ����� �Է��� �� �����ϴ�."));
		return _T("ERROR");
	} 

	if(strData2.IsEmpty()) // catsearch
	{ 
		
		CSeApi api(this);
		api.MakeIndexWord(strData1);


		if(ORA_CATSEARCH == m_pInfo->GetSearchIndexType())
				tmpquery.Format(_T(" CATSEARCH(I.IDX_%s, '%s*', NULL)>0"),strFieldName, strData1);
		else		
				tmpquery.Format(_T("&ECOSEARCH(I.REC_KEY,SE_IDX_%s, '%s*')"),strFieldName, strData1);


	}
	else // between
	{
		tmpquery.Format(_T("I.IDX_%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}


	return tmpquery;
}
CString CSearchSerial::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	

	if(strData1.IsEmpty() && !strData2.IsEmpty()) //���ڸ��� �Է��� ��� ���� ó��
	{
		AfxMessageBox(_T("ERROR!: �����ȣ ������ ���ڸ��� �Է��� �� �����ϴ�."));
		return _T("ERROR");
	}
	if(!strData1.IsEmpty() && strData2.IsEmpty()) //��ġ�˻�
	{
		if (InputValueCheck(strData1, _T("�����ڷᱸ��")) < 0 )
		{
			AfxMessageBox(_T("ERROR!: �����ȣ�� �ùٸ� �������� �Էµ��� �ʾҽ��ϴ�."));
			return _T("ERROR");
		} 
		tmpquery.Format(_T("%s = '%s'"),strFieldName,strData1);
	}
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) //�����˻�
	{
		if (InputValueCheck(strData1, _T("�����ڷᱸ��")) < 0 ){
			AfxMessageBox(_T("ERROR!: �����ȣ�� �ùٸ� �������� �Էµ��� �ʾҽ��ϴ�."));
			return _T("ERROR");
		} 
		if (InputValueCheck(strData2, _T("�����ڷᱸ��")) < 0 ){
			AfxMessageBox(_T("ERROR!: �����ȣ�� �ùٸ��������� �Էµ��� �ʾҽ��ϴ�"));
			return _T("ERROR");
		}
		tmpquery.Format(_T("%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

VOID CSearchSerial::OnbtnINDEXDONATORSEARCH() 
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

BOOL CSearchSerial::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if ( GetDlgItem(IDC_cmbINDEX_ACQ_TYPE)->m_hWnd == pMsg->hwnd ||
//			 GetDlgItem(IDC_edtINDEX_TITLE1)->m_hWnd == pMsg->hwnd   ||
//			 GetDlgItem(IDC_edtINDEX_TITLE2)->m_hWnd == pMsg->hwnd    ||
//			 GetDlgItem(IDC_edtINDEX_PUBLISHER1)->m_hWnd == pMsg->hwnd    ||
//			 GetDlgItem(IDC_edtINDEX_PUBLISHER2)->m_hWnd == pMsg->hwnd    ||
//			 GetDlgItem(IDC_cmbINDEX_PUB_FREQ)->m_hWnd == pMsg->hwnd   ||
			 GetDlgItem(IDC_cmbINDEX_PROVDER)->m_hWnd == pMsg->hwnd   ||
//			 GetDlgItem(IDC_edtINDEX_ISSN1)->m_hWnd == pMsg->hwnd  ||
//			 GetDlgItem(IDC_edtINDEX_ISSN2)->m_hWnd == pMsg->hwnd  ||
			 GetDlgItem(IDC_edtINDEX_CONTROL_NO1)->m_hWnd == pMsg->hwnd ||
			 GetDlgItem(IDC_edtINDEX_CONTROL_NO2)->m_hWnd == pMsg->hwnd	||
			 GetDlgItem(IDC_edtINDEX_START_DATE)->m_hWnd == pMsg->hwnd   ||
			 GetDlgItem(IDC_edtINDEX_END_DATE)->m_hWnd == pMsg->hwnd   ||
			 GetDlgItem(IDC_cmbINDEX_FORM_CODE)->m_hWnd == pMsg->hwnd   ||
			 GetDlgItem(IDC_cmbINDEX_MEDIA_CODE)->m_hWnd == pMsg->hwnd   ||
			 GetDlgItem(IDC_cmbINDEX_MAT_CODE)->m_hWnd == pMsg->hwnd   ||
			 GetDlgItem(IDC_edtINDEX_DONATOR)->m_hWnd == pMsg->hwnd   )
		{
			GetDlgItem(IDOK)->SetFocus();
			PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
			return TRUE;
			
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

INT CSearchSerial::InputValueCheck(CString strData, CString strCodeClassName)
{
	CString codes[6];
	CString code, strChar, strTemp;
	INT nstrLength, nCodeType;

	nstrLength = strData.GetLength();

	//�����ȣ�� ���.
	if(strCodeClassName == _T("�����ڷᱸ��"))   
	{
		if(nstrLength != 12) return -1;
		nCodeType =3;	 	
		strChar = strData.Left(nCodeType);

		//���� �ڵ� �̿��� ���� ���������� Ȯ��.
		for(INT i=nCodeType; i<nstrLength; i++)
		{
			strTemp = strData.Mid(i,1);
			if(strTemp < 0x0030 || strTemp > 0x0039) return -1;
		}
	}
	//��Ϲ�ȣ�� ���.
	else // if(strCodeClassName == _T("��ϱ���"))  
	{
		if(nstrLength < 3 ) return -1;
		nCodeType = 2;		
		strChar = strData.Left(nCodeType);
		
		//���� �ڵ� �̿��� ���� ���������� Ȯ��.
		for(INT i=nCodeType; i<nstrLength; i++)
		{
			strTemp = strData.Mid(i,1);
			if(strTemp < 0x0030 || strTemp > 0x0039) return -1;
		}
		//12�ڸ��� ä���� �ʰ� �Է��� ��� �� �ڸ�����ŭ _T("0")�� �߰�.
		for(i=0; i<(12 - nstrLength) ; i++)
		{
			strData.Insert(nCodeType, _T("0"));
		}
	}


	//�Է��� �ڵ�κ��� ������ �ڵ尪�� �� �ϳ��� ��ġ�ϴ� �� Ȯ��.
	INT startrow = 0;
	INT codeCnt = 	m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(strCodeClassName);
	for(INT i=0 ; i < codeCnt ; i++)
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(strCodeClassName, codes, 6, startrow);
		code = codes[2];
		
		code.TrimLeft(); code.TrimRight();
		
		if(code == strChar) return 0;
		
		startrow++;
	}

	return -1;
}

VOID CSearchSerial::OnbtnPURSEQNO() 
{
	// TODO: Add your control notification handler code here
	// ���� ��ȣ ���� 
	CSePurchaseNoDlg dlg(this);
	// ���� �����ư �� ����
	dlg.m_nOpenMode = -1;

	if(dlg.DoModal() != IDOK) return;
	
	m_pCM->SetControlMgrData(_T("�����⵵"),dlg.m_strYear);
	m_pCM->SetControlMgrData(_T("������ȣ"),dlg.m_strPurchaseNo);
	
	//�ڵ� �˻�
//	OnOK();
	
}

HBRUSH CSearchSerial::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


VOID CSearchSerial::EnableThemeDialogTexture(HWND hwndDlg)
{
	HINSTANCE hDll=LoadLibrary(_T("UxTheme.dll"));							// 'UxTheme.dll'
	if(hDll==NULL)
		return;	// the DLL won't be available on anything except Windows XP
	ULONG (PASCAL *lpfnEnableTheme)(HWND, DWORD);
	(FARPROC&)lpfnEnableTheme=GetProcAddress(hDll,"EnableThemeDialogTexture");	// 'EnableThemeDialogTexture'
	if(lpfnEnableTheme)
		lpfnEnableTheme(hwndDlg, 6);		// ETDT_ENABLETAB = (ETDT_ENABLE | ETDT_USETABTEXTURE) = (2|4) = 6
	else ASSERT(FALSE);
	FreeLibrary(hDll);
}

void CSearchSerial::OnbtnSerialClose() 
{
	// TODO: Add your control notification handler code here
	CSearchSerial::OnCancel();
	
}



//INT CSearchSerial::SetCodeControl(CESL_ControlMultiComboBox *pMultiComboBox, UINT nID, CString strClassName)
//{
//	// �ڵ�� ���� ���������� �о�´�.
//	if (pMultiComboBox == NULL) 
//	{
//		pMultiComboBox = new CESL_ControlMultiComboBox(NULL);
//	}
//	CString strQuery;
//	CString strClassCode;
//	strQuery.Format(_T("SELECT CLASS_CODE FROM CD_CLASS_CODE_TBL WHERE CLASS_NAME='%s'"), strClassName);
//	m_pControlDm.GetOneValueQuery( strQuery, strClassCode );
//	strClassCode.TrimLeft();			strClassCode.TrimRight();
//	strQuery.Format(_T("SELECT CODE, DESCRIPTION FROM CD_CODE_TBL WHERE CLASS=%s ORDER BY CODE"), strClassCode);
//	m_pControlDm.RestructDataManager(strQuery);
//
//	pMultiComboBox->SubclassDlgItem(nID, this);
//	pMultiComboBox->ResetContent();
//	pMultiComboBox->InitMultiComboBox(2);
//	CString strData[2];
//	strData[0] = _T("");
//	strData[1] = _T("");
//	INT nRows = m_pControlDm.GetRowCount();
//	for (INT i = 0; i < nRows; i++) {
//		strData[0] = m_pControlDm.GetCellData(i, 0);
//		strData[1] = m_pControlDm.GetCellData(i, 1);
//		strData[0].TrimLeft();			strData[0].TrimRight();
//		if (strData[0] == _T("")) strData[0] = _T("   ");
//		pMultiComboBox->SetData(strData, 2);
//	}
//	pMultiComboBox->SetCurSel(0);
//
//	return 0;
//}
