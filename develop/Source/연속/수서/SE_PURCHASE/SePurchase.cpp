// SePurchase.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchase.h"
#include "..\SE_API\SeApi.h"
#include "..\SE_API\SePurchaseNoDlg.h"
#include "SeMarcImport.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchase dialog


CSePurchase::CSePurchase(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pDM = NULL;
	m_pParentDM = NULL;	
	m_pDM_SEQ_NO = NULL;	
	m_pDM_WORK_NO = NULL;	
	m_pParentGrid = NULL;
	m_pCM = NULL;
	m_nOpenMode = 1; //default
	m_Marc.Init();
	m_nCurrentIdx = -1;
	m_bIsImported = FALSE;
	m_bIsWorkNoChanged = FALSE;
	m_bIsNewWorkNo = FALSE;
	m_nMoveOption = 1; //default

	m_bIsLightVersion = FALSE;
	m_bIsDepartMngUse = FALSE;

	m_pDepartmentOrder = NULL;

}


CSePurchase::~CSePurchase()
{
	if(m_pDepartmentOrder)
	{
		delete m_pDepartmentOrder;
		m_pDepartmentOrder = NULL;
	}
}

VOID CSePurchase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchase, CDialog)
	//{{AFX_MSG_MAP(CSePurchase)
	ON_CBN_SELCHANGE(IDC_cmbSPE_KDCDDDC, OnSelchangecmbSPEKDCDDDC)
	ON_BN_CLICKED(IDC_btnMARC_IMPORT, OnbtnMARCIMPORT)
	ON_BN_CLICKED(IDC_btnALL_CLEAR, OnbtnALLCLEAR)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnSPE_CREATE, OnbtnSPECREATE)
	ON_EN_KILLFOCUS(IDC_edtSPE_ORDER_NO, OnKillfocusedtSPEORDERNO)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnDEPARTMNG, OnbtnDEPARTMNG)
	ON_MESSAGE(BOOKCNTCHANGE, OnDepartmentBookCntChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchase message handlers

BOOL CSePurchase::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr( _T("����_����_������") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_������") );
		return false;
	}
	
	m_pDM = FindDM(_T("DM_����_������"));
	m_pDM_SEQ_NO = FindDM(_T("DM_����_������_������ȣ")); 
	m_pDM_WORK_NO = FindDM(_T("DM_����_������_�۾���ȣ"));
	m_pCM = FindCM(_T("CM_����_������"));
	

	if(!m_pDM || !m_pCM || !m_pDM_SEQ_NO || !m_pDM_WORK_NO) {
		AfxMessageBox( _T("DM,CM Error : ����_����_������") );
		return false;
	}
	
	InitControls();
	
	if(InitOpenMode()<0) {
		EndDialog(0);
		return FALSE;
	}

	if(m_bIsLightVersion)
	{
		SetLightVersion();
	}

	if(!m_bIsDepartMngUse)
	{
		SetDepartMngNoUse();
	}
	else
	{
		CreateDepartMngDlg();
	
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSePurchase::SetLightVersion()
{
	//����Ʈ ������ ��� ��� å�� ��Ʈ���� ���ش� 
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_stcPUR_REG_BOOK_CNT);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_edtPUR_REG_BOOK_CNT);
	if(pWnd) pWnd->ShowWindow(FALSE);
}

VOID CSePurchase::SetDepartMngNoUse()
{
	//�μ� ��������� ������� �ʴ� ��� ����μ��� �μ����� ��ư�� ����� 
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_stcPUR_ISSUE);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_edtPUR_ISSUE);
	if(pWnd) pWnd->ShowWindow(FALSE);	

	pWnd = GetDlgItem(IDC_btnDEPARTMNG);
	if(pWnd) pWnd->ShowWindow(FALSE);

}

VOID CSePurchase::CreateDepartMngDlg()
{
	m_pDepartmentOrder = new CSeDepartmentOrder(this);
	if(!m_pDepartmentOrder) return;

	m_pDepartmentOrder->m_nOpenMode = m_nOpenMode;
	m_pDepartmentOrder->m_nOpenLockMode = 1;

	if(m_nOpenMode<0)
	{
		//�μ��� �ڷ����� 
		CString strPURCHASE_SPECIES_KEY;
		CString stPURCHASE_MANAGE_KEY;
		strPURCHASE_SPECIES_KEY = m_pDM->GetCellData(_T("REC_KEY"),0);

		
		CString strSPECIES_KEY;	
		CString strQuery;
		strSPECIES_KEY = m_pDM->GetCellData(_T("SPECIES_KEY"),0);


		if(!strSPECIES_KEY.IsEmpty())
		{
			strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);

			m_pDM->GetOneValueQuery(strQuery,stPURCHASE_MANAGE_KEY);
		}


		m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
		m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;

	}

	m_pDepartmentOrder->Create((CESL_Mgr*)this);
	m_pDepartmentOrder->CenterWindow();
	m_pDepartmentOrder->ShowWindow(FALSE);

	//�������� ���� �μ��� �Լ� å���� ������ ��� 
	//�Լ� å���� ���� ���� ���ϵ��� �Ѵ�
	INT nAcqExpBookCnt = 0;
	if(m_nOpenMode<0)
	{
		m_pDepartmentOrder->RefreshAll();
		nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt(FALSE);
		if(nAcqExpBookCnt>0)
		{
			CString strCnt;
			CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("�Լ�å��"));
			pEdit->EnableWindow(FALSE);
			

		}
	}
}


VOID CSePurchase::InitControls()
{
	
	//control color
	AddColorControl(IDC_edtSPE_ORDER_NO, _T(""), _T(""), 'E');		
	AddColorControl(IDC_edtSPE_TITLE1, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtSPE_PUBLISHER, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtPUR_ACQ_BOOK_CNT, _T(""), _T(""), 'E');
	
	/*
	AddColorControl(IDC_edtPUR_START_DATE, _T("CM_����_������"), _T("����������"), 'D');
	AddColorControl(IDC_edtPUR_END_DATE, _T("CM_����_������"), _T("����������"), 'D');
	*/

	AddColorControl(IDC_cmbSPE_CONTROL_TYPE, _T("CM_����_������"), _T("�����ڷᱸ��_�ڵ�"), 'M');
	AddColorControl(IDC_cmbSPE_PUB_FREQ, _T("CM_����_������"), _T("�����"), 'M');

	//-------------------------------------------------------------------
	// 2005-10-11 Ȳ����: ��ó �а��� ���簡 �ƴϸ� �����.
	//-------------------------------------------------------------------
	CString str64;
	GetManageValue( _T("��Ÿ"), _T("����"), _T("����"), _T("����"), str64 );
	if ( str64 == _T("Y") ) 
	{
		GetDlgItem(IDC_STATIC_DEPT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbDept)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_COURSE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbCource)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_DEPT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbDept)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_COURSE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbCource)->ShowWindow(SW_HIDE);
	}


	if(m_nOpenMode>0) //�Է� 
	{
		//���� ��ư�� ����� 
		//GetDlgItem(IDC_btnPREV)->ShowWindow(SW_HIDE);
		//2005-10-13 Ȳ���� �߰�
		GetDlgItem(IDC_btnPREV)->SetWindowText(_T("���� ����"));
		GetDlgItem(IDC_btnNEXT)->ShowWindow(SW_HIDE);


		//�Է� �з���ȣ ó��
		OnSelchangecmbSPEKDCDDDC();

		//2005-09-27 Ȳ����: �Է½� ���� ���� ������ȣ�� �ʱ�ȭ�Ѵ�.
		CString strSeq_No, strSeq_YEAR, strQuery;
		strSeq_No.Empty();
		strSeq_YEAR.Empty();

		GetDlgItem(IDC_edtSPE_ORDER_YEAR)->GetWindowText(strSeq_YEAR);
		strQuery.Format(_T("SELECT MAX(SEQ_NO) from SE_PURCHASE_SEQ_NO_TBL WHERE working_status = '0' and acq_year = '%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), strSeq_YEAR);
		m_pDM_SEQ_NO->GetOneValueQuery(strQuery,strSeq_No);
		GetDlgItem(IDC_edtSPE_ORDER_NO)->SetWindowText(strSeq_No);

	}
	else
	{
		//���� �Է��� ���� ���� ��ư���� ����� 
		GetDlgItem(IDC_btnSPE_CREATE)->SetWindowText(_T("���� ����"));
		
		//���� �׸��� read onlyȭ �Ѵ�  
		CEdit* pEdit;
		pEdit = (CEdit*)GetDlgItem(IDC_edtSPE_ORDER_NO);
		pEdit->SetReadOnly();

		
		//�ڷ����,ȭ�������  ��ư�� �����
		GetDlgItem(IDC_btnMARC_IMPORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnALL_CLEAR)->ShowWindow(SW_HIDE);

	}

	AddColorControl(IDC_edtSPE_ORDER_NO, _T(""), _T(""), 'E');		
	AddColorControl(IDC_edtSPE_TITLE1, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtSPE_PUBLISHER, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtPUR_ACQ_BOOK_CNT, _T(""), _T(""), 'E');
	

}

INT CSePurchase::InitOpenMode()
{

	
	if(m_nOpenMode>0) //�Է� 
	{
		
			
	
	}
	else //���� 
	{
	
		if(!m_pParentDM) {
			AfxMessageBox(_T("�θ� DM�� ã�� �� �����ϴ�!"));
			return -1;
		}
		
		if(m_nCurrentIdx<0) {
			AfxMessageBox(_T("������ ������ �����ϴ�!"));
			return -2;
		}

		/*
		//�θ� �׸��忡�� ���������� ���´�
		if(!m_pParentGrid) {
			AfxMessageBox(_T("�θ� �׸��带 ã�� �� �����ϴ�!"));
			return -1;
		}

		
		m_pParentGrid->SelectMakeList();
		m_nCurrentIdx = m_pParentGrid->SelectGetHeadPosition();
		if(m_nCurrentIdx<0) {
			AfxMessageBox(_T("������ ������ �����ϴ�!"));
			return -3;
		}
		*/

		CString strREC_KEY;
		strREC_KEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
		
		if(!strREC_KEY.IsEmpty())
			RefreshAll(strREC_KEY);

	}



	return 0;
}
//======================================================================
// [�Լ�����]
//  �Էµ� ���������� �����Ѵ�.
// 
// [MINI SPEC] 
// 1. ����/������/������ȣ/�����/�Լ�å���� ��Ʈ�ѿ��� �����´�.
// 2. �����͸� DM�� �����Ѵ�. 
//======================================================================

VOID CSePurchase::OnOK() 
{
	//==================================================================
	// 1. ����/������/������ȣ/�����/�Լ�å���� ��Ʈ�ѿ��� �����´�.
	//==================================================================
	CString strTitle;
	m_pCM->GetControlMgrData(_T("����"),strTitle);

	if(strTitle.IsEmpty()) 
	{
		AfxMessageBox(_T("������ �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}

	//������(260)
	CString strPublisher;
	m_pCM->GetControlMgrData(_T("������"),strPublisher);

	if(strPublisher.IsEmpty()) 
	{
		AfxMessageBox(_T("�����ڰ� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}
	
	CString strSEQ_NO;
	m_pCM->GetControlMgrData(_T("������ȣ"),strSEQ_NO);

	if(	strSEQ_NO.IsEmpty())
	{	
		AfxMessageBox(_T("������ȣ�� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}
	
	CString strPUB_FREQ;
	m_pCM->GetControlMgrData(_T("�����"),strPUB_FREQ);
	strPUB_FREQ.TrimLeft();
	strPUB_FREQ.TrimRight();

	if(strPUB_FREQ.IsEmpty())
	{
		AfxMessageBox(_T("����󵵰� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}
	
	CString strACQ_BOOK_CNT;
	INT nACQ_BOOK_CNT = 0;
	m_pCM->GetControlMgrData(_T("�Լ�å��"),strACQ_BOOK_CNT);
	if(strACQ_BOOK_CNT.IsEmpty())
	{
		AfxMessageBox(_T("�Լ�å���� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}

	nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	if(nACQ_BOOK_CNT<1)
	{
		AfxMessageBox(_T("�Լ�å���� 1 �̻��̾�� �մϴ�!"));
		return;
	}
	
	//==================================================================
	// 2. �����͸� DM�� �����Ѵ�.
	//==================================================================
	SaveAllDataToDM();

	INT ids = 0;
	CSeApi api(this);

	if(m_nOpenMode>0)
	{	
		ids = api.SPECIES_PUR_OBJ_INSERT(m_pDM,
										 &m_Marc,
										 m_pDM_SEQ_NO,
										 m_pDM_WORK_NO,
										 m_bIsNewWorkNo,
										 0,
										 m_bIsDepartMngUse);
	}
	else 
	{
		
		ids = api.SPECIES_PUR_OBJ_UPDATE(m_pDM,
										 &m_Marc,
										 m_pDM_SEQ_NO,
										 m_pDM_WORK_NO,
										 m_bIsNewWorkNo,
										 m_bIsWorkNoChanged,
										 0,
										 m_bIsDepartMngUse);

	}
	

	if(m_bIsDepartMngUse)
	{
		SaveDepartmentOrderInfo();
	}

	RefreshParent();


	if(ids<0)
	{
		AfxMessageBox(_T("���忡 �����Ͽ����ϴ�!"));
	}
	else 
	{
		AfxMessageBox(_T("�����Ͽ����ϴ�!"));
		
		//member variables initialize
		m_bIsWorkNoChanged = FALSE;
		m_bIsNewWorkNo = FALSE;
	
	}
	
	if(m_nOpenMode>0) //�Է�  

	{
		m_pDM->FreeData(); 
		m_pCM->AllControlClear();
		
		//2005-09-27
		//�Է�������ȣ�� �״�� �����Ѵ�.
		GetDlgItem(IDC_edtSPE_ORDER_NO)->SetWindowText(strSEQ_NO);

		m_Marc.Init();
		m_bIsImported = FALSE;
		m_bIsWorkNoChanged = FALSE;
		m_bIsNewWorkNo = FALSE;
		
		CString str;
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSPE_WORK_NO);
		if(!pCombo) return;
		
		pCombo->GetWindowText(str);
		if( str == _T("����ȣ") )
		{
			INT i = pCombo->GetCurSel();
			if( i > 0)
				pCombo->SetCurSel(i-1);
		}
	//	pCombo->ResetContent();

		return;
	}
	else //����
	{
		
		return;
	}

	CDialog::OnOK();
}

VOID CSePurchase::RefreshParent()
{
	//�θ� DM�� ������ DM�� ��ũ�� copy���ְ� grid�� display�Ѵ�
	INT idx = -1;
	CString strPROVIDER_CODE;
	CString strMANAGE_CODE;
	CString strMAT_CDOE;
	CString strDesc;
	CString strStreamMarc;

	if(m_nOpenMode>0) //�Է� 
	{
		m_pParentDM->InsertRow(-1);
		idx = m_pParentDM->GetRowCount()-1;

		//test
		m_pParentDM->SetCellData(_T("UDF_�����۾�����"),_T("���Դ��"),idx);
	}	
	else
	{
		idx = m_nCurrentIdx;
	}
	
	
	CopyDMToDMByAlias(m_pDM,0,m_pParentDM,idx);
	m_pInfo->pMarcMgr->Encoding(&m_Marc, strStreamMarc);
	m_pParentDM->SetCellData(_T("MARC"),strStreamMarc,idx);

	
	//����ó
	strPROVIDER_CODE = m_pDM->GetCellData(_T("����ó_�ڵ�"),0);
	m_pParentDM->SetCellData(_T("����ó_�ڵ�"),strPROVIDER_CODE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����ó����"),strPROVIDER_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_����ó"),strDesc,idx);


	//��������
	strMANAGE_CODE = m_pDM->GetCellData(_T("��������_�ڵ�"),0);
	m_pParentDM->SetCellData(_T("��������_�ڵ�"),strMANAGE_CODE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������"),strMANAGE_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_��������"),strDesc,idx);


	//�ڷᱸ��
	strMAT_CDOE = m_pDM->GetCellData(_T("�ڷᱸ��_�ڵ�"),0);
	m_pParentDM->SetCellData(_T("�ڷᱸ��_�ڵ�"),strMAT_CDOE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�ڷᱸ��"),strMAT_CDOE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_�ڷᱸ��"),strDesc,idx);

	m_pParentGrid->SetProtectSelectColumn(TRUE);
	//m_pParentGrid->DisplayLine(idx);
	m_pParentGrid->Display();
	m_pParentGrid->SetReverse(idx);
}

INT CSePurchase::RefreshAll(CString strREC_KEY)
{
	
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strREC_KEY);

	m_pDM->RefreshDataManager(strWhere);
	m_pCM->AllControlDisplay(0);
	

	//��ũ����ü�� decoding �Ѵ�.
	m_Marc.Init();
	CString strStreamMarc;
	strStreamMarc = m_pDM->GetCellData(_T("MARC"),0);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_Marc);	


	//ISSN
	CString tmp = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("022$a"),tmp);
	INT npos = tmp.Find(_T("-"));
	if(npos>0) {
		CString strISSN1,strISSN2;
		strISSN1 = tmp.Left(npos);
		strISSN2 = tmp.Mid(npos+1);
		
		strISSN1.TrimLeft(); 	strISSN1.TrimRight();
		strISSN2.TrimLeft(); 	strISSN2.TrimRight();
		
		m_pCM->SetControlMgrData(_T("ISSN1"),strISSN1);
		m_pCM->SetControlMgrData(_T("ISSN2"),strISSN2);
	}
	
	//����󵵰� ��ĭ�� ���� �����Ⱓ(_)�� ǥ���Ѵ�.
	CString strPUB_FREQ = _T("");
	

	//�����(�����Ⱓó��)
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@18"),strPUB_FREQ);
	
	strPUB_FREQ.TrimLeft();
	strPUB_FREQ.TrimRight();

	if(strPUB_FREQ.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("�����"),_T("_"));
	}

	//�з� ��ȣ ó�� 
	m_pCM->SetControlMgrData(_T("�з���ȣ����_�ڵ�"),m_pDM->GetCellData(_T("�з���ȣ����_�ڵ�"), 0));
	m_pCM->SetControlMgrData(_T("������ȣ_�ڵ�"),m_pDM->GetCellData(_T("������ȣ_�ڵ�"), 0));
	OnSelchangecmbSPEKDCDDDC();

	
	//���� �� �۾���ȣ ó��
	m_bIsWorkNoChanged = FALSE;
	m_bIsNewWorkNo = FALSE;


	CString strSEQ_NO = _T("");
	strSEQ_NO = m_pDM->GetCellData(_T("������ȣ"),0);
	if(strSEQ_NO.IsEmpty()) return 0;

	CString strYear = _T("");
	strYear = m_pDM->GetCellData(_T("�����⵵"),0);
	if(strYear.IsEmpty()) return 0;


	CString strWorkNo = _T("");
	strWorkNo = m_pDM->GetCellData(_T("�������۾���ȣ"),0);
	
	CString strQuery;
	strQuery.Format(_T("ACQ_YEAR = '%s' AND SEQ_NO = %s"),strYear,strSEQ_NO);
	m_pDM_SEQ_NO->RefreshDataManager(strQuery);
	if(0 == m_pDM_SEQ_NO->GetRowCount()) return 0;

	CString strSEQ_NO_KEY = _T("");
	strSEQ_NO_KEY = m_pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);

	if(_T("0") == strWorkNo || strWorkNo.IsEmpty() ) //�۾���ȣ�� ������� �ʴ� ��� 
	{
		SetWorkNo(_T(""),_T(""),FALSE);
	}
	else //����ϴ� ���
	{
		SetWorkNo(strSEQ_NO_KEY,strWorkNo);
	}

	return 0;
}

//======================================================================
// [�Լ�����]
//  �����͸� DM�� �����Ѵ�.
// 
// [MINI SPEC] 
// 1. �Է±����� üũ�Ͽ� DM�� FREE�ϰ� �������� �߰��Ѵ�.
// 2. DM �����Լ��� ȣ���Ѵ�. 
// 3. MARC �����Ը� ȣ���Ѵ�.
//======================================================================
INT CSePurchase::SaveAllDataToDM()
{
	//==================================================================
	// 1. �Է±����� üũ�Ͽ� DM�� FREE�ϰ� �������� �߰��Ѵ�.
	//==================================================================
	if(m_nOpenMode>0 && !m_bIsImported)
	{
		m_pDM->FreeData();
		m_pDM->InsertRow(-1);
	}
	
	//==================================================================
	// 2. DM �����Լ��� ȣ���Ѵ�. 
	//==================================================================
	SaveSpecieaData();

	//==================================================================
	// 3. MARC �����Ը� ȣ���Ѵ�.
	//==================================================================
	SetMarcByControlData();


	return 0;
}

//======================================================================
// [�Լ�����]
//  �������� DM�� �����Ѵ�.
// 
// [MINI SPEC] 
// 1. DM���� ���� ������ �����͸� �ʱ�ȭ
// 2. ��Ʈ�ѷκ��� DM���� �����͸� �ű��.
//======================================================================
INT CSePurchase::SaveSpecieaData()
{
	
	//==================================================================
	// 1. DM���� ���� ������ �����͸� �ʱ�ȭ
	//==================================================================
	const INT cnt = 20;
	CString alias[ cnt ] = {
		_T("��������"),					//	0
		_T("����ó_�ڵ�"),			    //	1
		_T("����������"),				//	2
		_T("����������"),				//	3
		_T("�Լ�å��"),					//	4
		_T("���å��"),					//	5
		_T("��������_�ڵ�"),			//	6
		_T("��ϱ���_�ڵ�"),			//	7
		_T("��ü����_�ڵ�"),			//	8
		_T("������ϱ���_�ڵ�"),		//	9
		_T("�̿��󱸺�_�ڵ�"),		// 10
		_T("�̿����ѱ���_�ڵ�"),		// 11
		_T("�ڷᱸ��_�ڵ�"),			// 12
		_T("�����ڷᱸ��_�ڵ�"),		// 13
		_T("���ı���_�ڵ�"),            // 14
		_T("������ȣ_�ڵ�"),            // 15
		_T("�з���ȣ����_�ڵ�"),		// 16	
		_T("����μ�"),					// 17	
		_T("��û�а�"),					// 18	
		_T("��û�к�"),					// 19	
	};				

	//==================================================================
	// 2. ��Ʈ�ѷκ��� DM���� �����͸� �ű��.
	//==================================================================
	CString tmp;
	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");	
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM->SetCellData(alias[i],tmp,0);
	}
	
	//==================================================================
	// 3. �Է� ������ ó���Ѵ�.
	//==================================================================
	if(m_nOpenMode>0)
	{
		if(m_bIsImported) m_pDM->SetCellData(_T("�Է±���"),_T("1"),0); //�ֹ��� 
		else m_pDM->SetCellData(_T("�Է±���"),_T("0"),0); //ȭ�� �Է� 
	}
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSPE_WORK_NO);
	if(!pCombo) return 0;
	tmp	= _T("");
	pCombo->GetWindowText(tmp);	
	CString strYear = _T("");
	CString strNo = _T("");

	//�����⵵, ������ȣ, �۾���ȣ check
	if(m_nOpenMode>0)
	{
		
		//�����⵵ 
		m_pCM->GetControlMgrData(_T("�����⵵"),strYear);
		m_pDM->SetCellData(_T("�����⵵"),strYear,0);
		
		//������ȣ 
		m_pCM->GetControlMgrData(_T("������ȣ"),strNo);
		m_pDM->SetCellData(_T("������ȣ"),strNo,0);


		//�Է½ÿ��� �� �۾� ��ȣ�� �����ߴ°� ���θ� �Ǵ��Ѵ�.
		if(_T("����ȣ") == tmp) 
		{
			m_bIsNewWorkNo = TRUE;
		}
		else if(_T("������") == tmp)
		{
			m_pDM->SetCellData(_T("�������۾���ȣ"),_T("0"),0);
		}
		else
		{
			m_pDM->SetCellData(_T("�������۾���ȣ"),tmp,0);

		}



	}
	else
	{
		//�����ÿ��� �����⵵ Ȥ�� ������ȣ�� ��������� �ִ����� check�� �۾���ȣ�� �ݿ��Ѵ� 
		CString strExistingYear;
		CString strExistingSeqNo;
		CString strNewYear;
		CString strNewSeqNo;
		strExistingYear = m_pDM->GetCellData(_T("�����⵵"),0);
		strExistingSeqNo = m_pDM->GetCellData(_T("������ȣ"),0);
		m_pCM->GetControlMgrData(_T("�����⵵"),strNewYear);
		m_pCM->GetControlMgrData(_T("������ȣ"),strNewSeqNo);
		
		
		//�����⵵ 
		m_pDM->SetCellData(_T("�����⵵"),strNewYear,0);
		
		//������ȣ 
		m_pDM->SetCellData(_T("������ȣ"),strNewSeqNo,0);
		

		if(strExistingYear != strNewYear || strExistingSeqNo != strNewSeqNo)
		{
			if(_T("����ȣ") == tmp) //�� ������ȣ �� ����ȣ
			{
				m_bIsNewWorkNo = TRUE;
				return 0;
			}
			else if(_T("������") == tmp) //�� ������ȣ �� �۾���ȣ 0 
			{
				m_pDM->SetCellData(_T("�������۾���ȣ"),_T("0"),0);
				m_bIsWorkNoChanged = TRUE;
				return 0;
			}
			else //�� ������ȣ �� ���� ��ȣ
			{
				m_pDM->SetCellData(_T("�������۾���ȣ"),tmp,0);
				m_bIsWorkNoChanged = TRUE;
				return 0;
			}

		}
		
		
		
		//���� �ÿ��� ������ �۾� ��ȣ�� ��������� check�ؼ� ������ �־�� �Ѵ�.
		if(0 == m_pDM_WORK_NO->GetRowCount()) return 0;  //�۾���ȣ ������
		
	
		if(_T("����ȣ") == tmp) 
		{
			m_bIsNewWorkNo = TRUE;
			return 0;
		}
		else
		{
			CString strExistingWorkNo = _T("");
			strExistingWorkNo = m_pDM->GetCellData(_T("�������۾���ȣ"),0); //���� �۾���ȣ
			
			if(tmp == strExistingWorkNo) //��ȭ ���� 
			{
				return 0; 
			}
			else //��ȭ ���� 
			{
				m_bIsWorkNoChanged = TRUE;
				m_pDM->SetCellData(_T("�������۾���ȣ"),tmp,0);
				return 0; 
			}
			
			
		}
	}

	return 0;
}

INT CSePurchase::SetMarcByControlData()
{
	//�Է°� ������忡 ���� ������ �ڼ��� �Ұ�!
	//����� ���� �����͸� ������ �ٽ� �Է��Ѵ�
	
	//���� ��ũ �׷� ����
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	CString strtmp;
	
	if(m_nOpenMode>0) 
	{
		// ���� ��ũ leader ����
		CSeApi api(this);
		api.SetSerialMarcReader(&m_Marc);
	}

	//���������͸� ����� �Է�
	const INT cnt = 8;
	CString strtag[cnt] = {
		_T("245$a"),	//0
		_T("245$x"),	//1
		_T("440$a"),	//2
		_T("245$d"),	//3
		_T("260$a"),	//4	
		_T("260$b"),	//5	
		_T("022$a"), 	//6
		_T("008@18")};  //7
	

	for(INT i=0;i<cnt;i++)
	{
		m_pInfo->pMarcMgr->DeleteItem(&m_Marc,strtag[i]);
	}
	
	/*
	m_pCM->GetControlMgrData(_T("����"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a"),strtmp);
	strtmp = _T("");
	*/

	m_pCM->GetControlMgrData(_T("����"),strtmp);
		
	strtmp.TrimLeft();
	strtmp.TrimRight();
		
	if(!strtmp.IsEmpty())
	{
		if(strtmp.GetAt(0) == '(') //��Ī�� ���� ��쿡�� ���ñ�ȣ 20
		{
			m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a!20"),strtmp);
		}
		else
		{
			m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a"),strtmp);
		}
	}

	strtmp = _T("");


	m_pCM->GetControlMgrData(_T("����"),strtmp);

	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$x"),strtmp);
	strtmp = _T("");
	
	m_pCM->GetControlMgrData(_T("�Ѽ���"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("440$a"),strtmp);
	strtmp = _T("");
	
	m_pCM->GetControlMgrData(_T("����"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$d"),strtmp);
	strtmp = _T("");

	m_pCM->GetControlMgrData(_T("������"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("260$a"),strtmp);
	strtmp = _T("");

	m_pCM->GetControlMgrData(_T("������"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("260$b"),strtmp);
	strtmp = _T("");


	//ISSNó��
	CString strISSN,strISSN1,strISSN2;
	m_pCM->GetControlMgrData(_T("ISSN1"),strISSN1);
	m_pCM->GetControlMgrData(_T("ISSN2"),strISSN2);

	if(!strISSN1.IsEmpty() && !strISSN2.IsEmpty())
	{
		strISSN.Format(_T("%s-%s"),strISSN1,strISSN2);
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("022$a"),strISSN);
		strtmp = _T("");
	}
	
	//008 TAG ó��
	////////////////////////////////////////////////////////////////////////////
	
	//�Է�����, ���̼�����  �Է¸��(������ �ƴ� ���)������ ���� 
	if(m_nOpenMode>0 && !m_bIsImported) 
	{
		//���� ���� (40�ڸ�)
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@39"),_T(" "));
		
		CTime t = CTime::GetCurrentTime();
		CString tmp,inputdate;
		tmp.Format(_T("%04d%02d%02d"),t.GetYear(),t.GetMonth(),t.GetDay());
		inputdate = tmp.Mid(2);
		m_pInfo->pMarcMgr->DeleteItem(&m_Marc,_T("008@0-5"));
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@0-5"),inputdate);
	
		//�������(������ c)�� ǥ�� 
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@6"),_T("c"));

		//���Լ�(���� r)�� ǥ��
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@19"),_T("r"));

	}
	
	
	//�����
	m_pCM->GetControlMgrData(_T("�����"),strtmp);

	
	if(_T("_") == strtmp)
	{
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@18"),_T(" "));
	}
	else
	{
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@18"),strtmp);	
	}
	
	//�����(310$a)ó��: �������� �Է� (008 tag�� ���󰣴�)
	CString strDesc;
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("������ڵ�"),strtmp,strDesc);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("310$a"),strDesc);

	strtmp = _T("");


	m_pInfo->pMarcMgr->DeleteEmptySubfield(&m_Marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(&m_Marc);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&m_Marc);

	return 0;
}

INT CSePurchase::SetControlDataByMarc()
{
	//��ũ���԰� ��ũ ������ɿ��� ���� �Լ��� 
	//��ũ���� ��Ʈ�� �����͸� �����Ѵ� 
	

	//���� ��ũ �׷� ����
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	CString strtmp;
	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("245$a"),strtmp);
	m_pCM->SetControlMgrData(_T("����"),strtmp);
	strtmp = _T("");
	
	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("245$x"),strtmp);
	m_pCM->SetControlMgrData(_T("����"),strtmp);
	strtmp = _T("");

	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("440$a"),strtmp);
	m_pCM->SetControlMgrData(_T("�Ѽ���"),strtmp);
	strtmp = _T("");
	
	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("245$d"),strtmp);
	m_pCM->SetControlMgrData(_T("����"),strtmp);
	strtmp = _T("");

	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("260$a"),strtmp);
	m_pCM->SetControlMgrData(_T("������"),strtmp);
	strtmp = _T("");

	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("260$b"),strtmp);
	m_pCM->SetControlMgrData(_T("������"),strtmp);
	strtmp = _T("");

	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("300$c"),strtmp);
	m_pCM->SetControlMgrData(_T("ũ��"),strtmp);
	strtmp = _T("");


	//�����
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@18"),strtmp);
	
	strtmp.TrimLeft();
	strtmp.TrimRight();

	if(strtmp.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("�����"),_T("_"));
	}
	else
	{
		m_pCM->SetControlMgrData(_T("�����"),strtmp);
	}

	strtmp = _T("");


	//ISSNó��
	CString strISSN,strISSN1,strISSN2;
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("022$a"),strISSN);
	INT npos = strISSN.Find(_T("-"));
	if(npos>0)
	{
		strISSN1 = strISSN.Left(npos);
		strISSN2 = strISSN.Mid(npos+1);

		strISSN1.TrimLeft();	strISSN1.TrimRight();
		strISSN2.TrimLeft();	strISSN2.TrimRight();
		
		m_pCM->SetControlMgrData(_T("ISSN1"),strISSN1);
		m_pCM->SetControlMgrData(_T("ISSN2"),strISSN2);

	}


	return 0;
}


INT CSePurchase::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

VOID CSePurchase::OnSelchangecmbSPEKDCDDDC() 
{
	// TODO: Add your control notification handler code here
	
	CESL_ControlMultiComboBox *pMultiCombo_Code = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("�з���ȣ����_�ڵ�"));
	if (!pMultiCombo_Code) return;
    CESL_ControlMultiComboBox *pMultiCombo_Subject = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("������ȣ_�ڵ�"));
	if (!pMultiCombo_Subject) return;
  
	CString class_name = _T("");
	CString strWhichCode;
	pMultiCombo_Code->GetData(strWhichCode,pMultiCombo_Code->GetCurSel(),1);
	strWhichCode.TrimLeft();		strWhichCode.TrimRight();
	strWhichCode.MakeUpper();
	CString str90;
	GetManageValue(_T("��Ÿ"), _T("����"), _T("�����"), _T(""), str90);

	if (strWhichCode == _T("KDC")) class_name = _T("KDC��������ȣ����");
	if (strWhichCode == _T("DDC")) class_name = _T("DDC��������ȣ����");
	//2005-11-23 Ȳ���� LC
	if (strWhichCode == _T("LC") && str90 == _T("Y")) class_name = _T("LC������ȣ");	

	pMultiCombo_Subject->FreeComboData();
	pMultiCombo_Subject->ResetContent();
	pMultiCombo_Subject->InitMultiComboBox(2);
	if (class_name.GetLength() == 0) return ;
	
	
	INT nCode = 256;
	INT nDefaultValue = 0;
	CString strCode[256], strDesc[256];
	nDefaultValue = 0;
	m_pInfo->pCodeMgr->GetCode(class_name, strCode, strDesc, nCode, nDefaultValue);	
	if (nCode == 0) return;
	

	CString strDefaultValue;
	strDefaultValue	= m_pDM->GetCellData(_T("������ȣ_�ڵ�"), 0);
	
	for (INT i = 0; i < nCode; i++) {
		if (strCode[i].GetLength() == 0) strCode[i] = _T("  ");
		
		if (strDefaultValue == strCode[i]) nDefaultValue = i;

		
		pMultiCombo_Subject->AddItem(strCode[i], 0, i);
		pMultiCombo_Subject->AddItem(strDesc[i], 1, i);
	}
	
	pMultiCombo_Subject->SetCurSel(nDefaultValue);	

}

VOID CSePurchase::OnbtnMARCIMPORT() 
{
	// TODO: Add your control notification handler code here
	
	//�ڷ� ���� 
	CSeMarcImport dlg(this);
	if(dlg.DoModal() != IDOK) return;
	
	m_pDM->FreeData();
	m_pDM->InsertRow(-1);
	
	CopyDMToDMByAlias(dlg.m_pDM,dlg.m_nCurrentIdx,m_pDM,0);
	m_Marc = dlg.m_Marc;
	
	SetControlDataByMarc();
	
	//�����⵵,������ȣ 
	CString stYear;
	CString strPurchaseNo;

	m_pCM->GetControlMgrData(_T("�����⵵"),stYear);
	m_pCM->GetControlMgrData(_T("������ȣ"),strPurchaseNo);

	m_pCM->AllControlDisplay(0);

	m_pCM->SetControlMgrData(_T("�����⵵"),stYear);
	m_pCM->SetControlMgrData(_T("������ȣ"),strPurchaseNo);

	m_bIsImported = TRUE;
	
}
//=====================================================================
// [�Լ�����]
//  ȭ������� ����� �����Ѵ�.
// [MINI SPEC]
//  1. ������ȣ�� �ӽ� �����Ѵ�. [2005-10-04] Ȳ���� ����
//  2. ��Ʈ���� clear�Ѵ�.
//  3. ������ȣ�� �����ϰ� �۾���ȣ�� �����Ѵ�. [2005-10-04] Ȳ���� ����
//  
//=====================================================================
VOID CSePurchase::OnbtnALLCLEAR() 
{
	//2005-09-30 ������ȣ�� ���ܳ��´�.
	CString strPurchaseNo;
	m_pCM->GetControlMgrData(_T("������ȣ"),strPurchaseNo);

	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();

	m_pCM->SetControlMgrData(_T("������ȣ"),strPurchaseNo);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSPE_WORK_NO);
	if(!pCombo) return;
	// pCombo->ResetContent();
}


VOID CSePurchase::BringPrevData()
{
	INT nRow = m_pParentDM->GetRowCount();
	if(nRow<1) return; 
	
	INT nIdx = nRow - 1;

	CString strREC_KEY;
	strREC_KEY = m_pParentDM->GetCellData(_T("REC_KEY"),nIdx);
	
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strREC_KEY);

	m_pDM->RefreshDataManager(strWhere);
	m_pCM->AllControlDisplay(0);
	

	//��ũ����ü�� decoding �Ѵ�.
	m_Marc.Init();
	CString strStreamMarc;
	strStreamMarc = m_pDM->GetCellData(_T("MARC"),0);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_Marc);	


	//ISSN
	CString tmp = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("022$a"),tmp);
	INT npos = tmp.Find(_T("-"));
	if(npos>0) {
		CString strISSN1,strISSN2;
		strISSN1 = tmp.Left(npos);
		strISSN2 = tmp.Mid(npos+1);
		
		strISSN1.TrimLeft(); 	strISSN1.TrimRight();
		strISSN2.TrimLeft(); 	strISSN2.TrimRight();
		
		m_pCM->SetControlMgrData(_T("ISSN1"),strISSN1);
		m_pCM->SetControlMgrData(_T("ISSN2"),strISSN2);
	}


	//�з� ��ȣ ó�� 
	m_pCM->SetControlMgrData(_T("�з���ȣ����_�ڵ�"),m_pDM->GetCellData(_T("�з���ȣ����_�ڵ�"), 0));
	m_pCM->SetControlMgrData(_T("������ȣ_�ڵ�"),m_pDM->GetCellData(_T("������ȣ_�ڵ�"), 0));
	OnSelchangecmbSPEKDCDDDC();

	//�����͸� �����
	m_pDM->FreeData(); 
	m_Marc.Init();
	m_bIsImported = FALSE;
	m_bIsWorkNoChanged = FALSE;
	m_bIsNewWorkNo = FALSE;

	//������ȣ�� �۾���ȣ�� �����
	m_pCM->SetControlMgrData(_T("������ȣ"),_T(""));

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSPE_WORK_NO);
	if(!pCombo) return;
	//2005-10-13 Ȳ���� �߰�
//	pCombo->ResetContent();
}


VOID CSePurchase::OnbtnPREV() 
{
	// TODO: Add your control notification handler code here
	
	//�Է¸���� ���� �������� �ҷ�����
	if(m_nOpenMode>0)
	{
		BringPrevData();
		
		//2005-09-27 Ȳ����: �Է½� ���� ���� ������ȣ�� �ʱ�ȭ�Ѵ�.
		CString strSeq_No, strSeq_YEAR, strQuery;
		strSeq_No.Empty();
		strSeq_YEAR.Empty();
		strQuery.Empty();

		GetDlgItem(IDC_edtSPE_ORDER_YEAR)->GetWindowText(strSeq_YEAR);
		strQuery.Format(_T("SELECT MAX(SEQ_NO) from SE_PURCHASE_SEQ_NO_TBL WHERE working_status = '0' and acq_year = '%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), strSeq_YEAR);
		m_pDM_SEQ_NO->GetOneValueQuery(strQuery,strSeq_No);

		GetDlgItem(IDC_edtSPE_ORDER_NO)->SetWindowText(strSeq_No);

		return;
	}

	
	
	if(1 == m_nMoveOption)
	{
		m_nCurrentIdx--;

		if(m_nCurrentIdx < 0)
		{
			AfxMessageBox(_T("ó�� �׸��Դϴ�!"));
			m_nCurrentIdx++;
			return;
		}
		
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetPrev();

		if(nIdx<0) 
		{
			INT nHeadIdx = m_pParentGrid->SelectGetHeadPosition();
			
			if(m_nCurrentIdx == nHeadIdx)
			{
				AfxMessageBox(_T("ó�� �׸��Դϴ�!"));
				m_pParentGrid->SelectGetHeadPosition();
				return;
			}
			else //���� ���� ������ ���� ��� �� ���� ���������� ���Ѵ� 
			{
				m_nCurrentIdx = nHeadIdx;
			}
		
		}
		else //���� ������ �������� �����δ�
		{
			m_nCurrentIdx = nIdx;
		}

	}
	
	CString strREC_KEY;
	strREC_KEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
	RefreshAll(strREC_KEY);
	m_pParentGrid->SetReverse(m_nCurrentIdx);
	
}

VOID CSePurchase::OnbtnNEXT() 
{
	// TODO: Add your control notification handler code here
	
	if(1 == m_nMoveOption)
	{
		m_nCurrentIdx++;
		INT nMaxIdx = m_pParentDM->GetRowCount()-1;
		if(m_nCurrentIdx > nMaxIdx)
		{
			AfxMessageBox(_T("������ �׸��Դϴ�!"));
			m_nCurrentIdx--;
			return;
		}
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetNext();

		if(nIdx<0) 
		{
			INT nTailIdx = m_pParentGrid->SelectGetTailPosition();
			
			if(m_nCurrentIdx == nTailIdx)
			{
				AfxMessageBox(_T("������ �׸��Դϴ�!"));
				m_pParentGrid->SelectGetTailPosition();
				return;
			}
			else //���� ���� ������ ���� ��� �ǵ��� ���������� ���Ѵ� 
			{
				m_nCurrentIdx = nTailIdx;
			}
		}
		else //���� ������ �������� �����δ�
		{
			m_nCurrentIdx = nIdx;
		}
	}

	CString strREC_KEY;
	strREC_KEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
	RefreshAll(strREC_KEY);
	m_pParentGrid->SetReverse(m_nCurrentIdx);

}

VOID CSePurchase::OnbtnSPECREATE() 
{
	// TODO: Add your control notification handler code here
	// ���� �Է�
	CSePurchaseNoDlg dlg(this);
	if(dlg.DoModal() != IDOK) return;
	
	
	//���� ��忡���� ���� ������ �ֹ�ó�����ΰ��� �ľ��� �ֹ�ó���� ������ ���� �Ұ� 
	if(m_nOpenMode<0)
	{
		if(m_pDM_SEQ_NO->GetRowCount()>0) 
		{
			CString strWORKING_STATUS = _T("");
			strWORKING_STATUS = m_pDM_SEQ_NO->GetCellData(_T("�۾�����"),0);
			if(_T("1") == strWORKING_STATUS)
			{
				AfxMessageBox(_T("���� �ڷ�� �ֹ� ���̹Ƿ� ������ ������ �� �����ϴ�!"));
				return;
			}
		}
	}

	

	m_pCM->SetControlMgrData(_T("�����⵵"),dlg.m_strYear);
	m_pCM->SetControlMgrData(_T("������ȣ"),dlg.m_strPurchaseNo);


	SetPurchaseNo();
}

VOID CSePurchase::OnKillfocusedtSPEORDERNO() 
{
	// TODO: Add your control notification handler code here
	// �Է½� ������ȣ kill focus: �ش� ���Գ⵵�� �ش� ������ ������ �ش� �۾���ȣ�� �ѷ��ش� 
	if(m_nOpenMode<0) return;
	
	SetPurchaseNo();
}

VOID CSePurchase::SetPurchaseNo()
{

	CString strSEQ_NO = _T("");
	m_pCM->GetControlMgrData(_T("������ȣ"),strSEQ_NO);
	if(strSEQ_NO.IsEmpty()) return;

	CString strYear;
	m_pCM->GetControlMgrData(_T("�����⵵"),strYear);
	if(strYear.IsEmpty()) return;

	CString strQuery;
	strQuery.Format(_T("ACQ_YEAR = '%s' AND SEQ_NO = %s"),strYear,strSEQ_NO);
	m_pDM_SEQ_NO->RefreshDataManager(strQuery);
	
	INT nRowCnt = m_pDM_SEQ_NO->GetRowCount();
	if(0 == nRowCnt) {
		AfxMessageBox(_T("�ش� �����⵵�� �������� �ʴ� ������ȣ�Դϴ�!"));
		//==========================================
		//20080606 UPDATE BY PJW : �������ùٸ��� ������� NULL�� �����Ѵ�.
		//GetDlgItem(IDC_edtSPE_ORDER_NO)->SetFocus();
		CEdit * pOrderNo = (CEdit *)GetDlgItem(IDC_edtSPE_ORDER_NO); 
		pOrderNo->SetWindowText(_T(""));
		pOrderNo->SetFocus();
		//=========================================
		return;
	}


	if(m_nOpenMode) //�Է½ÿ��� ���Դ�� �ش��ϴ� ������ �Է� �� �� �ְ� �Ѵ�
	{
		CString strWorking_status;
		strWorking_status = m_pDM_SEQ_NO->GetCellData(_T("�۾�����"),0);
		if(_T("0") != strWorking_status) //���Դ���� �ƴϸ� 
		{
			AfxMessageBox(_T("�ش� ������ �ֹ����Դϴ�. ���Դ�� ������ ������ �ּ���!"));
			GetDlgItem(IDC_edtSPE_ORDER_NO)->SetFocus();
			return;
		}
	}
	
	
	CString strWorkNo = _T("");
	strWorkNo = m_pDM_SEQ_NO->GetCellData(_T("�������۾���ȣ"),0);
	
	CString strREC_KEY = _T("");
	strREC_KEY = m_pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);


	if(_T("0") == strWorkNo) //�۾���ȣ�� ������� �ʴ� ��� 
	{
		SetWorkNo(_T(""),_T(""),FALSE);
	}
	else //����ϴ� ���
	{
		SetWorkNo(strREC_KEY,strWorkNo); //������ ��ȣ�� setting 
	}

}

VOID CSePurchase::SetWorkNo(CString strSEQ_NO_KEY, CString strCurrentWorkNo , BOOL bUseWorkNo)
{
	//�۾���ȣ ���� �Լ� 
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSPE_WORK_NO);
	if(!pCombo) return;
	
	if(!bUseWorkNo) //������
	{
		pCombo->ResetContent();
		pCombo->InsertString( 0, _T("������") );
		pCombo->SetCurSel( 0 );
		return;
	}

	if(strSEQ_NO_KEY.IsEmpty()) return;
	
	CString strQuery;
	strQuery.Format(_T("PURCHASE_SEQ_NO_KEY = %s"),strSEQ_NO_KEY);
	m_pDM_WORK_NO->RefreshDataManager(strQuery);
	
	pCombo->ResetContent();
	INT nRowCnt = m_pDM_WORK_NO->GetRowCount();
	CString strWorkNo;

	for(INT i=0;i<nRowCnt;i++)
	{
		strWorkNo = _T("");
		strWorkNo = m_pDM_WORK_NO->GetCellData(_T("�۾���ȣ"),i);
		pCombo->InsertString( i, strWorkNo);
	}
	
	pCombo->InsertString( nRowCnt , _T("����ȣ") );
		
	
	if(strCurrentWorkNo.IsEmpty())
	{
		pCombo->SetCurSel( 0 );
	}
	else
	{
		INT nIndex = pCombo->FindStringExact(0,strCurrentWorkNo);
	
		if(nIndex>=0) pCombo->SetCurSel( nIndex );
		else pCombo->SetCurSel( 0 );

	}

}
HBRUSH CSePurchase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	INT nID = pWnd->GetDlgCtrlID();
	if (IsColorControl(nID)) 
	{
		return SetColor(pDC);
	}

	return hbr;
}

VOID CSePurchase::OnbtnDEPARTMNG() 
{
	// TODO: Add your control notification handler code here
	if(!m_pDepartmentOrder) return;
	

	//�μ��� �ڷ����� 
	CString strPURCHASE_SPECIES_KEY;
	CString stPURCHASE_MANAGE_KEY;
	strPURCHASE_SPECIES_KEY = m_pDM->GetCellData(_T("REC_KEY"),0);

	
	if(m_nOpenMode<0)
	{
		CString strSPECIES_KEY;	
		CString strQuery;
		strSPECIES_KEY = m_pDM->GetCellData(_T("SPECIES_KEY"),0);


		if(!strSPECIES_KEY.IsEmpty())
		{
			strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);

			m_pDM->GetOneValueQuery(strQuery,stPURCHASE_MANAGE_KEY);
		}
	}

	m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
	m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;
	
	//test
	//m_pDepartmentOrder->RefreshAll();
	INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt(FALSE);
	m_pDepartmentOrder->ShowWindow(TRUE);

}

VOID CSePurchase::OnDepartmentBookCntChange(WPARAM w,LPARAM l)
{
	if(!m_pDepartmentOrder) return;

	INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt(FALSE);
	CString strCnt;
	CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("�Լ�å��"));
	

	if(0 == nAcqExpBookCnt)
	{
		pEdit->EnableWindow(TRUE);
	}
	else
	{
		strCnt.Format(_T("%d"),nAcqExpBookCnt);
		m_pCM->SetControlMgrData(_T("�Լ�å��"),strCnt);
		pEdit->EnableWindow(FALSE);
	}

}


VOID CSePurchase::SaveDepartmentOrderInfo()
{
	if(!m_pDepartmentOrder) return;
	

	//�μ��� �ڷ����� 
	CString strPURCHASE_SPECIES_KEY;
	CString stPURCHASE_MANAGE_KEY;
	strPURCHASE_SPECIES_KEY = m_pDM->GetCellData(_T("REC_KEY"),0);

	
	if(m_nOpenMode<0)
	{
		CString strSPECIES_KEY;	
		CString strQuery;
		strSPECIES_KEY = m_pDM->GetCellData(_T("SPECIES_KEY"),0);


		if(!strSPECIES_KEY.IsEmpty())
		{
			strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);

			m_pDM->GetOneValueQuery(strQuery,stPURCHASE_MANAGE_KEY);
		}
	}

	m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
	m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;

	//test
	if(m_nOpenMode>0)
	{
		INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt(FALSE);
		if(0 == nAcqExpBookCnt)
		{
			INT nACQ_BOOK_CNT = 1;	
			CString strACQ_BOOK_CNT;
			m_pCM->GetControlMgrData(_T("�Լ�å��"),strACQ_BOOK_CNT);
			nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));

			m_pDepartmentOrder->InsertDefaultAcqBookCnt(nACQ_BOOK_CNT);
		}
	}
	
	INT ids = m_pDepartmentOrder->SaveAllData();

	if(ids<0) AfxMessageBox(_T("�μ������ֹ����� ���忡 ���� �߽��ϴ�!"));


}
