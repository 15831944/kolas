// SeSpeciesForm.cpp : implementation file
//

#include "stdafx.h"
#include "SeSpeciesForm.h"
#include "SeMarcEdit.h"
#include "SeMemoEdit.h"
#include "SeTitleChangeDlg.h"
#include "SePublisherChangeDlg.h"
#include "SeMediaChangeDlg.h"
#include "SePubfreqChangeDlg.h"

#include "..\SE_HISTORY\SE_HISTORY_MAIN.h"
#include "..\SE_API\SeApi.h"
#include "..\SE_API\SeMarcImport.h"
#include "..\SE_API\SeDuplicatedTitleDlg.h"
#include "..\..\..\�������\Light_GatherDataSearch\UC_MARC_IMPORT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeSpeciesForm dialog


CSeSpeciesForm::CSeSpeciesForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeSpeciesForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeSpeciesForm)
	m_bBINDING_YN = FALSE;
	//}}AFX_DATA_INIT
	m_nACQ_TYPE = 1; //default
	m_strSpeciesKey = _T("");
	m_pPurchaseDlg = NULL;
	m_pDonateDlg = NULL;
	m_pExpectDlg = NULL;
	m_nOpenMode = 1; //default �Է�
	m_pCM = NULL;
	m_pDM = NULL;
	m_pDM_IDX = NULL;
	m_pDM_SUB_HISTORY = NULL;
	m_pDM_SPE_HISTORY = NULL;
	m_bIsExpCreated = FALSE;
	m_bIsExpVerified = FALSE;

	m_nPubExpType = -1;  
	m_nAcqExpType = -1;  

	m_pParentGrid = NULL;
	m_pParentDM = NULL;
	
	m_Marc.Init();
	m_strMEMO = _T("");
	m_nCurrentIdx = -1;


	m_strPURCHASE_YN = _T("");
	m_strDONATE_YN = _T("");
	
	m_pDiscontinueDlg = NULL;
	m_bIsDiscontinued = FALSE;
	m_bIsTitleChanged = FALSE;
	m_bIsPublisherChanged = FALSE;

	m_nMoveOption = 1; //default
	m_bIsLastCtrlFocused = FALSE;

	m_bIsLightVersion = FALSE;
	m_bIsDepartMngUse = FALSE;
	AfxInitRichEdit();

}

CSeSpeciesForm::~CSeSpeciesForm()
{
	FreeChildDialog();
}

VOID CSeSpeciesForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeSpeciesForm)
	DDX_Check(pDX, IDC_chkSPE_BINDING_YN, m_bBINDING_YN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeSpeciesForm, CDialog)
	//{{AFX_MSG_MAP(CSeSpeciesForm)
	ON_BN_CLICKED(IDC_btnMARC_EDIT, OnbtnMARCEDIT)
	ON_BN_CLICKED(IDC_btnMEMO, OnbtnMEMO)
	ON_BN_CLICKED(IDC_btnEXPECT, OnbtnEXPECT)
	ON_BN_CLICKED(IDC_btnHISTORY, OnbtnHISTORY)
	ON_BN_CLICKED(IDC_btnMARC_IMPORT, OnbtnMARCIMPORT)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnALL_CLEAR, OnbtnALLCLEAR)
	ON_BN_CLICKED(IDC_btnSPE_TITLE_CHANGE, OnbtnSPETITLECHANGE)
	ON_BN_CLICKED(IDC_btnSPE_MEDIA_CODE_CHANGE, OnbtnSPEMEDIACODECHANGE)
	ON_BN_CLICKED(IDC_btnSPE_PUBLISHER_CHANGE, OnbtnSPEPUBLISHERCHANGE)
	ON_BN_CLICKED(IDC_btnSPE_PUB_FREQ_CHANGE, OnbtnSPEPUBFREQCHANGE)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnCOMMON_LIST, OnbtnCOMMONLIST)
	ON_CBN_KILLFOCUS(IDC_cmbSPE_SHELP_LOC_CODE, OnKillfocuscmbSPESHELPLOCCODE)
	ON_BN_CLICKED(IDC_btnSPE_TITLE_DUP_INSPECT, OnbtnSPETITLEDUPINSPECT)
	ON_MESSAGE(KILLFOCUS_LAST_CTRL, OnChildLastKillFocus)
	ON_CBN_SETFOCUS(IDC_cmbSPE_SHELP_LOC_CODE, OnSetfocuscmbSPESHELPLOCCODE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeSpeciesForm message handlers

BOOL CSeSpeciesForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if( InitESL_Mgr( _T("����_����_��") ) < 0 ) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_��") );
		return FALSE;
	}


	m_pCM = FindCM(_T("CM_����_��"));
	m_pDM = FindDM(_T("DM_����_��"));
	m_pDM_IDX =  FindDM(_T("DM_����_����"));
	m_pDM_SUB_HISTORY =  FindDM(_T("DM_����_�����̷�����"));
	m_pDM_SPE_HISTORY =  FindDM(_T("DM_����_��_�̷�����"));


	if(!m_pCM || !m_pDM || !m_pDM_IDX || !m_pDM_SUB_HISTORY || !m_pDM_SPE_HISTORY) {
		AfxMessageBox( _T("CM,DM Error : ����_��,����_����") );
		return FALSE;
	}
	

	//����Ʈ ���� setting 
	if(m_bIsLightVersion)
	{	
		SetLightVersion();
	}


	//���� �Ǵ� ���� ���̾�α� ����
	CreateChildDialog();

	//��Ʈ�� ����
	InitControls();

	//�Է� �Ǵ� ���� ��� ���� 
	if(InitOpenMode()<0) {
		//����
		EndDialog(0);
		return FALSE;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSeSpeciesForm::SetLightVersion()
{
	//����Ʈ ������ ��� ��������� ��Ʈ�Ѿ��ְ� ���� ���� ��Ʈ���� ��ġ�� ���ġ �Ѵ� 	
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_chkSPE_BINDING_YN);
	if(pWnd) pWnd->ShowWindow(FALSE);
		
	CRect rect;
	
	pWnd = GetDlgItem(IDC_STATIC_SPE_PRICE);
	if(pWnd)
	{
		pWnd->GetWindowRect(rect);
		ScreenToClient(&rect);
		rect.left = rect.left-43;
		rect.right = rect.right-43;
		
		/*
		rect.left = 12;
		rect.right = 45;
		rect.top = 265;
		rect.bottom = 280;
		*/

		pWnd->MoveWindow(rect);
	}
		
	pWnd = GetDlgItem(IDC_edtSPE_PRICE);
	if(pWnd)
	{
		pWnd->GetWindowRect(rect);
		ScreenToClient(&rect);

		rect.left = rect.left-43;
		rect.right = rect.right-33;
		
		/*
		rect.left = 63;
		rect.right = 144;
		rect.top = 263;
		rect.bottom = 283;		
		*/
			
		pWnd->MoveWindow(rect);
	}

}


VOID CSeSpeciesForm::InitControls()
{

	ShowUserDefineCode();

	//���ݼ��� ���� 
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbSPE_PRICE_PERSONALITY );
	pCombo->InsertString( 0, _T("����") );
	pCombo->InsertString( 1, _T("���ǰ") );
	pCombo->InsertString( 2, _T("���ݺҸ�") );
	pCombo->SetCurSel( 0 );
	

	//color
	AddColorControl(IDC_edtSPE_PUBLISHER, _T(""), _T(""), 'E');
	AddColorControl(IDC_cmbSPE_CONTROL_TYPE, _T("CM_����_��"), _T("SS_�����ڷᱸ��_�ڵ�"), 'M');
	AddColorControl(IDC_cmbSPE_PUB_FREQ, _T("CM_����_��"), _T("�����"), 'M');
	AddColorControl(IDC_cmbSPE_PUB_STATUS, _T("CM_����_��"), _T("�������"), 'M');
	AddColorControl(IDC_cmbSPE_SHELP_LOC_CODE, _T("CM_����_��"), _T("SS_�谡��ġ_�ڵ�"), 'M');

	
	//�Է� ��� 
	if(m_nOpenMode>0) 
	{
		
		//���� color
		AddColorControl(IDC_edtSPE_TITLE1, _T(""), _T(""), 'E');
		
		

		//�����ȣ 
		m_pCM->SetControlMgrData(_T("SS_�����ȣ"),_T("<<�ڵ� �ο�>>")); 
		
		//���� ��ư�� �����.
		GetDlgItem(IDC_btnSPE_TITLE_CHANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSPE_PUBLISHER_CHANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSPE_PUB_FREQ_CHANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSPE_MEDIA_CODE_CHANGE)->ShowWindow(SW_HIDE);
		

		//���� ������ �����
		//GetDlgItem(IDC_btnPREV)->ShowWindow(SW_HIDE); 
		
		//���� ��ư�� �Է½� �������� �ҷ����� �������
		GetDlgItem(IDC_btnPREV)->SetWindowText(_T("��������\n�ҷ�����"));
		GetDlgItem(IDC_btnNEXT)->ShowWindow(SW_HIDE);
		
		//�� �̷� ��ư�� �����
		GetDlgItem(IDC_btnHISTORY)->ShowWindow(SW_HIDE);

	}
	else
	{
		
		CEdit* pEdit;
		pEdit = (CEdit*)GetDlgItem(IDC_edtSPE_TITLE1);
		if(pEdit) pEdit->SetReadOnly();
		
		GetDlgItem(IDC_btnSPE_TITLE_DUP_INSPECT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnMARC_IMPORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnCOMMON_LIST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnALL_CLEAR)->ShowWindow(SW_HIDE);

	}

	ShowUserDefineCode();
}

INT CSeSpeciesForm::InitOpenMode()
{
	
	if(m_nOpenMode>0) //�Է� 
	{
	

	
	}
	else //����
	{
		//�θ� �׸��忡�� ���������� ���´�
		if(!m_pParentGrid) {
			AfxMessageBox(_T("�θ� �׸��带 ã�� �� �����ϴ�!"));
			return -1;
		}

		if(!m_pParentDM) {
			AfxMessageBox(_T("�θ� DM�� ã�� �� �����ϴ�!"));
			return -2;
		}
		
	
		if(m_nCurrentIdx<0) {
			AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
			return -3;
		}
	
		m_strSpeciesKey = m_pParentDM->GetCellData(_T("ISS_��KEY"),m_nCurrentIdx);
		
		if(!m_strSpeciesKey.IsEmpty())
			RefreshAll(m_strSpeciesKey);
		
	}

	return 0;

}

INT CSeSpeciesForm::RefreshAll(CString strSpeciesKey)
{
	//��Ű�� ������ ��,���� DM�� refresh �Ѵ�
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strSpeciesKey);
	m_pDM->RefreshDataManager(strWhere);
	m_pDM_IDX->RefreshDataManager(strWhere);
	

	//��ũ����ü�� decoding �Ѵ�.
	m_Marc.Init();
	CString strStreamMarc;
	strStreamMarc = m_pDM->GetCellData(_T("MARC"),0);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_Marc);	


	//�⺻ control������ �����ش�
	m_pCM->AllControlDisplay(0);
	
	
	//â����, �󰣳� -- ��ĭ�� ���� trim�ؼ� ��Ʈ�ѿ� ����
	CString strFoundationYear,strDiscontinueYear;
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@7-10"),strFoundationYear);
	strFoundationYear.TrimRight();
	strFoundationYear.TrimLeft();
	m_pCM->SetControlMgrData(_T("â����"),strFoundationYear);
	
	
	//�󰣳� 
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@11-14"),strDiscontinueYear);
	strDiscontinueYear.TrimRight();
	strDiscontinueYear.TrimLeft();
	m_pCM->SetControlMgrData(_T("�󰣳�"),strDiscontinueYear);		



	//������2(550 tag �� �Ǹ����� ������)
	CArray<CString,CString&> arrayPublisher;
	CString strPublisher550 = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("550$a"),strPublisher550,&arrayPublisher);
	
	INT nSize = arrayPublisher.GetSize();
	if(nSize>0)
	{
		strPublisher550 = arrayPublisher.GetAt(nSize-1);
	}

	if(!strPublisher550.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("������2"),strPublisher550);
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
	

	//���ݼ���
	CString tmp;
	tmp = m_pDM->GetCellData(_T("SS_���ݼ���"),0);
	
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbSPE_PRICE_PERSONALITY );
	CString strCombo;

	if ( tmp == _T("") ) tmp = _T("����");

	for(INT i=0;i<pCombo->GetCount();i++)
	{
		strCombo = _T("");
		pCombo->SetCurSel(i);
		pCombo->GetWindowText(strCombo);
		if(tmp == strCombo) break;		
	}


	//��������
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SS_��������"),0);
	if(_T("Y") == tmp)
		m_bBINDING_YN = TRUE;
	else 
		m_bBINDING_YN = FALSE;

	tmp = _T("");
	

	//ISSN
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
	// 2008.05.15 ADD BY PDJ
	// ������� ó��
	else
	{
		m_pCM->SetControlMgrData(_T("ISSN1"),_T(""));
		m_pCM->SetControlMgrData(_T("ISSN2"),_T(""));
	}

	//�޸�
	m_strMEMO = m_pDM->GetCellData(_T("SS_�޸�"),0);


	//���� ���� ����
	m_strPURCHASE_YN = m_pDM->GetCellData(_T("SS_���Կ���"),0);
	m_strDONATE_YN = m_pDM->GetCellData(_T("SS_��������"),0);
	
	//��û�а�
	//m_pCM->SetControlMgrData(_T(""),strISSN1);

	//��û�к�
	//m_pCM->SetControlMgrData(_T("ISSN1"),strISSN1);



	if(1 == m_nACQ_TYPE)  //���� 
	{

		if(_T("Y") == m_strPURCHASE_YN)
		{

			//���� ���������� ����� 
			if(m_pExpectDlg) delete m_pExpectDlg;
			m_pExpectDlg = NULL;
			m_bIsExpCreated = FALSE;
			m_bIsExpVerified = FALSE;
			m_nPubExpType = -1;  
			m_nAcqExpType = -1;  

			
		}
		else if(_T("N") == m_strPURCHASE_YN)
		{
			//���������� Ȯ���غ��Ƽ� ���������� ������ ������ �´�
			BringExpData(strSpeciesKey,2);

		}
		
	}
	else if(2 == m_nACQ_TYPE) //���� 
	{

		if(_T("Y") == m_strDONATE_YN)
		{
			
			//���� ���������� ����� 
			if(m_pExpectDlg) delete m_pExpectDlg;
			m_pExpectDlg = NULL;
			m_bIsExpCreated = FALSE;
			m_bIsExpVerified = FALSE;
			m_nPubExpType = -1;  
			m_nAcqExpType = -1;  

		}
		else if(_T("N") == m_strDONATE_YN)
		{
			//���������� Ȯ���غ��Ƽ� ���������� ������ ������ �´�
			BringExpData(strSpeciesKey,1);

		}
	}

	
	//���̷�DM free 
	m_pDM_SPE_HISTORY->FreeData();


	//�󰣿���
	if(m_pDiscontinueDlg)
	{
		delete m_pDiscontinueDlg;
		m_pDiscontinueDlg = NULL;
	}
	m_bIsDiscontinued = FALSE;

	
	//������ ���� 
	m_bIsTitleChanged = FALSE;

	
	//������ ���� ����
	m_bIsPublisherChanged = FALSE;


	//child 
	if(1 == m_nACQ_TYPE)
	{
		m_pPurchaseDlg->RefreshAll(strSpeciesKey);
	}
	else if(2 == m_nACQ_TYPE)
	{
		m_pDonateDlg->RefreshAll(strSpeciesKey);
	}

	UpdateData(FALSE);
	

	return 0;
}


VOID CSeSpeciesForm::BringPrevData()
{
	//�Է¸�忡���� �������� �ҷ�����
	INT nParentRowCnt = m_pParentDM->GetRowCount();
	if(0 == nParentRowCnt) return;
	
	//������ �� ������ �ҷ��´�
	INT nIdx = nParentRowCnt -1;
	CString strSpeciesKey = m_pParentDM->GetCellData(_T("ISS_��KEY"),nIdx);

	//��Ű�� ������ ��,���� DM�� refresh �Ѵ�
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strSpeciesKey);
	m_pDM->RefreshDataManager(strWhere);
	m_pDM_IDX->RefreshDataManager(strWhere);
	
	
	//�⺻ control������ �����ش�
	m_pCM->AllControlDisplay(0);
	

	//�����ȣ�� ����
	m_pCM->SetControlMgrData(_T("SS_�����ȣ"),_T(""));

	//2005-10-04 : ��ũ������ ���´�
	//��ũ����ü�� decoding �Ѵ�.
	CMarc tempMarc;
	tempMarc.Init();
	CString strStreamMarc;
	strStreamMarc = m_pDM->GetCellData(_T("MARC"),0);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &tempMarc);
	
	
	//����󵵰� ��ĭ�� ���� �����Ⱓ(_)�� ǥ���Ѵ�.
	CString strPUB_FREQ = _T("");
	

	//�����(�����Ⱓó��)
	m_pInfo->pMarcMgr->GetItem(&tempMarc,_T("008@18"),strPUB_FREQ);
	
	strPUB_FREQ.TrimLeft();
	strPUB_FREQ.TrimRight();

	if(strPUB_FREQ.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("�����"),_T("_"));
	}
	

	//���ݼ���
	CString tmp;
	tmp = m_pDM->GetCellData(_T("SS_���ݼ���"),0);
	
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbSPE_PRICE_PERSONALITY );
	CString strCombo;

	for(INT i=0;i<pCombo->GetCount();i++)
	{
		strCombo = _T("");
		pCombo->SetCurSel(i);
		pCombo->GetWindowText(strCombo);
		if(tmp == strCombo) break;
	}


	//��������
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SS_��������"),0);
	if(_T("Y") == tmp)
		m_bBINDING_YN = TRUE;
	else 
		m_bBINDING_YN = FALSE;

	tmp = _T("");
	
	//ISSN
	m_pInfo->pMarcMgr->GetItem(&tempMarc,_T("022$a"),tmp);
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
	
		
	/*
	//���� ���� ����
	m_strPURCHASE_YN = m_pDM->GetCellData(_T("SS_���Կ���"),0);
	m_strDONATE_YN = m_pDM->GetCellData(_T("SS_��������"),0);
	
	
	if(1 == m_nACQ_TYPE)  //���� 
	{

		if(_T("Y") == m_strPURCHASE_YN)
		{

			//���� ���������� ����� 
			if(m_pExpectDlg) delete m_pExpectDlg;
			m_pExpectDlg = NULL;
			m_bIsExpCreated = FALSE;
			m_bIsExpVerified = FALSE;
			m_nPubExpType = -1;  
			m_nAcqExpType = -1;  

			
		}
		else if(_T("N") == m_strPURCHASE_YN)
		{
			//���������� Ȯ���غ��Ƽ� ���������� ������ ������ �´�
			BringExpData(strSpeciesKey,2);

		}
		
	}
	else if(2 == m_nACQ_TYPE) //���� 
	{

		if(_T("Y") == m_strDONATE_YN)
		{
			
			//���� ���������� ����� 
			if(m_pExpectDlg) delete m_pExpectDlg;
			m_pExpectDlg = NULL;
			m_bIsExpCreated = FALSE;
			m_bIsExpVerified = FALSE;
			m_nPubExpType = -1;  
			m_nAcqExpType = -1;  

		}
		else if(_T("N") == m_strDONATE_YN)
		{
			//���������� Ȯ���غ��Ƽ� ���������� ������ ������ �´�
			BringExpData(strSpeciesKey,1);

		}
	}
	*/
	

	//�����͸� �����
	m_pDM->FreeData();
	m_pDM_IDX->FreeData();

	//child �� ��Ʈ�ѿ� �����͸� �Ѹ���, �����͸� �����
	if(1 == m_nACQ_TYPE)
	{
		m_pPurchaseDlg->RefreshAll(strSpeciesKey);
		m_pPurchaseDlg->m_pDM->FreeData();
		m_pPurchaseDlg->m_pDM_SUB_HISTORY->FreeData();
	}
	else if(2 == m_nACQ_TYPE)
	{
		m_pDonateDlg->RefreshAll(strSpeciesKey);
		m_pDonateDlg->m_pDM->FreeData();
	}

	UpdateData(FALSE);
	
	
	return;
}


VOID CSeSpeciesForm::BringExpData(CString strSPECIES_KEY,INT nACQ_TYPE)
{
	//���� ������ Ȯ���ؼ� �ش� ���Ա����� ���� �����͸� ������ �´�.
	CESL_DataMgr* pDM_MONTH = FindDM(_T("DM_��_��������"));
	CESL_DataMgr* pDM_WEEK = FindDM(_T("DM_��_�ְ�����"));
	CESL_DataMgr* pDM_DAY = FindDM(_T("DM_��_�ϰ�����"));
	CESL_DataMgr* pDM_ETC = FindDM(_T("DM_��_��Ÿ����"));
	CESL_DataMgr* pDM_VOL = FindDM(_T("DM_����_��_����_��ȣ"));
	CESL_DataMgr* pDM_OTHER_VOL = FindDM(_T("DM_����_��_����_��������ȣ"));
	CESL_DataMgr* pDM_APPENDIX = FindDM(_T("DM_����_��_����_�η�"));


	if(!pDM_MONTH || !pDM_WEEK || !pDM_DAY || !pDM_ETC || !pDM_VOL || !pDM_OTHER_VOL || !pDM_APPENDIX)
	{
		AfxMessageBox(_T("���������� ������ DM�� ã�� ���߽��ϴ�!")); 	
		return;
	}

	pDM_MONTH->FreeData();
	pDM_WEEK->FreeData();
	pDM_DAY->FreeData();	
	pDM_ETC->FreeData();	
	pDM_VOL->FreeData();
	pDM_OTHER_VOL->FreeData();	
	pDM_APPENDIX->FreeData();		

	
	CString strQuery;
	const INT cnt = 5;
	CString strResult[cnt];
	
	if(2 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_KEY,PUB_EXP_KEY,ACQ_EXP_TYPE,PUB_EXP_TYPE FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	}
	else if(1 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_KEY,PUB_EXP_KEY,ACQ_EXP_TYPE,PUB_EXP_TYPE FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	}

	m_pDM->GetValuesQuery(strQuery,strResult,5);

	if(strResult[0].IsEmpty()) return;
	
	CString strACQ_KEY;
	CString strACQ_EXP_KEY;
	CString strPUB_EXP_KEY;
	CString strACQ_EXP_TYPE;
	CString strPUB_EXP_TYPE;
	
	strACQ_KEY = strResult[0];
	strACQ_EXP_KEY = strResult[1];
	strPUB_EXP_KEY =  strResult[2];
	strACQ_EXP_TYPE =  strResult[3];
	strPUB_EXP_TYPE =  strResult[4];


	if(strACQ_EXP_TYPE.IsEmpty() || strPUB_EXP_TYPE.IsEmpty()) {

		//���� ���������� ����� 
		if(m_pExpectDlg) delete m_pExpectDlg;
		m_pExpectDlg = NULL;
		m_bIsExpCreated = FALSE;
		m_bIsExpVerified = FALSE;
		m_nPubExpType = -1;  
		m_nAcqExpType = -1;  
	
		return;
	}

	//��ȣ 
	strQuery.Format(_T("ACQ_KEY = %s AND FORMAT_TYPE = '1'"),strACQ_KEY);
	pDM_VOL->RefreshDataManager(strQuery);


	//��������ȣ 
	strQuery.Format(_T("ACQ_KEY = %s AND FORMAT_TYPE = '2'"),strACQ_KEY);
	pDM_OTHER_VOL->RefreshDataManager(strQuery);


	//�η�
	strQuery.Format(_T("ACQ_KEY = %s"),strACQ_KEY);
	pDM_APPENDIX->RefreshDataManager(strQuery);

	
	if(strACQ_EXP_TYPE == strPUB_EXP_TYPE) //���� ������ �Լ� ������ ���� �� ���
	{
		
		CESL_DataMgr* pDM_MONTH_SUB = FindDM(_T("DM_��_��������_����"));
		CESL_DataMgr* pDM_WEEK_SUB = FindDM(_T("DM_��_�ְ�����_����"));
		CESL_DataMgr* pDM_DAY_SUB = FindDM(_T("DM_��_�ϰ�����_����"));
		CESL_DataMgr* pDM_ETC_SUB = FindDM(_T("DM_��_��Ÿ����_����"));
		
		if(!pDM_MONTH_SUB || !pDM_WEEK_SUB || !pDM_DAY_SUB || !pDM_ETC_SUB)
		{
			AfxMessageBox(_T("���������� ������ ���� DM�� ã�� ���߽��ϴ�!")); 	
			return;
		}

		strQuery.Format(_T("REC_KEY = %s"),strPUB_EXP_KEY); //������
	

		switch(strPUB_EXP_TYPE.GetAt(0))
		{
			case 'M':
				pDM_MONTH->RefreshDataManager(strQuery);
				break;
			case 'W':
				pDM_WEEK->RefreshDataManager(strQuery);
				break;
			case 'D':
				pDM_DAY->RefreshDataManager(strQuery);
				break;	
			case 'E':
				pDM_ETC->RefreshDataManager(strQuery);
				break;
			default:
				break;
		}

		strQuery.Format(_T("REC_KEY = %s"),strACQ_EXP_KEY); //�Լ���
		
		switch(strACQ_EXP_TYPE.GetAt(0))
		{
			case 'M':
				pDM_MONTH_SUB->RefreshDataManager(strQuery);
				break;
			case 'W':
				pDM_WEEK_SUB->RefreshDataManager(strQuery);
				break;
			case 'D':
				pDM_DAY_SUB->RefreshDataManager(strQuery);
				break;	
			case 'E':
				pDM_ETC_SUB->RefreshDataManager(strQuery);
				break;
			default:
				break;
			
		}

		//DATA copy
		switch(strPUB_EXP_TYPE.GetAt(0))
		{
			case 'M':
				pDM_MONTH->InsertRow(-1);
				CopyDMToDMByAlias(pDM_MONTH_SUB,0,pDM_MONTH,1);
				break;
			case 'W':
				pDM_WEEK->InsertRow(-1);
				CopyDMToDMByAlias(pDM_WEEK_SUB,0,pDM_WEEK,1);
				break;
			case 'D':
				pDM_DAY->InsertRow(-1);
				CopyDMToDMByAlias(pDM_DAY_SUB,0,pDM_DAY,1);
				break;	
			case 'E':
				pDM_ETC->InsertRow(-1);
				CopyDMToDMByAlias(pDM_ETC_SUB,0,pDM_ETC,1);
			default:
				break;

		}


	}
	else //���� ������ �Լ� ������ �ٸ� ���
	{

		strQuery.Format(_T("REC_KEY = %s"),strPUB_EXP_KEY); //������
		
		switch(strPUB_EXP_TYPE.GetAt(0))
		{
			case 'M':
				pDM_MONTH->RefreshDataManager(strQuery);
				break;
			case 'W':
				pDM_WEEK->RefreshDataManager(strQuery);
				break;
			case 'D':
				pDM_DAY->RefreshDataManager(strQuery);
				break;	
			case 'E':
				pDM_ETC->RefreshDataManager(strQuery);
				break;
			default:
				break;

		}

		strQuery.Format(_T("REC_KEY = %s"),strACQ_EXP_KEY); //�Լ���
		
		switch(strACQ_EXP_TYPE.GetAt(0))
		{
			case 'M':
				pDM_MONTH->RefreshDataManager(strQuery);
				break;
			case 'W':
				pDM_WEEK->RefreshDataManager(strQuery);
				break;
			case 'D':
				pDM_DAY->RefreshDataManager(strQuery);
				break;	
			case 'E':
				pDM_ETC->RefreshDataManager(strQuery);
				break;
			default:
				break;

		}
	}

	if(m_pExpectDlg) delete m_pExpectDlg;
	m_pExpectDlg = NULL;

	m_bIsExpVerified = TRUE;
	m_bIsExpCreated = FALSE;
	m_nPubExpType = GetIntExpType(strPUB_EXP_TYPE);  
	m_nAcqExpType = GetIntExpType(strACQ_EXP_TYPE);   


}

INT CSeSpeciesForm::DeleteExpData(CString strSPECIES_KEY,INT nACQ_TYPE)
{
	//���� ������ Ȯ���ؼ� �ش� ���Ա����� ���� �����͸� �����.
	CString strQuery;
	const INT cnt = 5;
	CString strResult[cnt];
	
	if(2 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_KEY,PUB_EXP_KEY,ACQ_EXP_TYPE,PUB_EXP_TYPE FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	}
	else if(1 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_KEY,PUB_EXP_KEY,ACQ_EXP_TYPE,PUB_EXP_TYPE FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	}

	m_pDM->GetValuesQuery(strQuery,strResult,5);

	if(strResult[0].IsEmpty()) return -1;
	
	CString strACQ_KEY;
	CString strACQ_EXP_KEY;
	CString strPUB_EXP_KEY;
	CString strACQ_EXP_TYPE;
	CString strPUB_EXP_TYPE;
	
	strACQ_KEY = strResult[0];
	strACQ_EXP_KEY = strResult[1];
	strPUB_EXP_KEY =  strResult[2];
	strACQ_EXP_TYPE =  strResult[3];
	strPUB_EXP_TYPE =  strResult[4];
	

	m_pDM->StartFrame();

	CString strTblName;
	
	//�Լ��� 
	if(!strACQ_EXP_TYPE.IsEmpty() && !strACQ_EXP_KEY.IsEmpty())
	{
		
		strTblName = GetExpTable(strACQ_EXP_TYPE);
		strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"),strTblName,strACQ_EXP_KEY);
		m_pDM->InitDBFieldData();
		m_pDM->AddFrame(strQuery);
	
	}
	
	//������ 
	if(!strPUB_EXP_TYPE.IsEmpty() && !strPUB_EXP_KEY.IsEmpty())
	{

		strTblName = GetExpTable(strPUB_EXP_TYPE);
		strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"),strTblName,strPUB_EXP_KEY);
		m_pDM->InitDBFieldData();
		m_pDM->AddFrame(strQuery);

	}
		
	//��ȣ , ��������ȣ
	strQuery.Format(_T("DELETE FROM SE_VOL_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
	m_pDM->InitDBFieldData();
	m_pDM->AddFrame(strQuery);
	
	
	//�η�
	strQuery.Format(_T("DELETE FROM SE_APPENDIX_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
	m_pDM->InitDBFieldData();
	m_pDM->AddFrame(strQuery);
	
	
	//����, ���� ���� update
	/*
	if(2 == nACQ_TYPE)
	{
		strQuery.Format(_T("UPDATE SE_DONATE_MANAGE_TBL SET ACQ_EXP_KEY = NULL,PUB_EXP_KEY = NULL")
						_T(",ACQ_EXP_TYPE = NULL,PUB_EXP_TYPE = NULL WHERE REC_KEY = %s;"),strACQ_KEY);
	}
	else if(1 == nACQ_TYPE)
	{
		strQuery.Format(_T("UPDATE SE_PURCHASE_MANAGE_TBL SET ACQ_EXP_KEY = NULL,PUB_EXP_KEY = NULL")
						_T(",ACQ_EXP_TYPE = NULL,PUB_EXP_TYPE = NULL WHERE REC_KEY = %s;"),strACQ_KEY);
	}
	*/

	//���� 2004.2.20 -- ���� Ÿ�Ը� ���쵵��...
	if(2 == nACQ_TYPE)
	{
		strQuery.Format(_T("UPDATE SE_DONATE_MANAGE_TBL SET ACQ_EXP_TYPE = NULL,PUB_EXP_TYPE = NULL")
						_T(" WHERE REC_KEY = %s;"),strACQ_KEY);
	}
	else if(1 == nACQ_TYPE)
	{
		strQuery.Format(_T("UPDATE SE_PURCHASE_MANAGE_TBL SET ACQ_EXP_TYPE = NULL,PUB_EXP_TYPE = NULL")
						_T(" WHERE REC_KEY = %s;"),strACQ_KEY);
	}
	
	m_pDM->InitDBFieldData();
	m_pDM->AddFrame(strQuery);


	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();

	return ids;
}

CString CSeSpeciesForm::GetExpTable(CString strType)
{

	CString strTBL_NAME = _T("");

	switch(strType.GetAt(0))
	{
	case 'M':
		strTBL_NAME = _T("SE_MONTH_EXP_TBL");
		break;
	case 'W':
		strTBL_NAME = _T("SE_WEEK_EXP_TBL");
		break;
	case 'D':
		strTBL_NAME = _T("SE_DAY_EXP_TBL");
		break;
	case 'E':
		strTBL_NAME = _T("SE_ETC_EXP_TBL");
		break;
	default:
		break;
	}

	return strTBL_NAME;
}

VOID CSeSpeciesForm::RefreshParent()
{
	//�θ� DM�� ������ DM�� copy���ְ� grid�� display�Ѵ�
	INT idx;
	
	if(m_nOpenMode>0) //�Է� 
	{
		m_pParentDM->InsertRow(-1);
		idx = m_pParentDM->GetRowCount()-1;
	}	
	else
	{
		
		idx = m_nCurrentIdx;
		CopyDMToDMByAlias(m_pDM_IDX,0,m_pParentDM,idx);

	}
	

	CString strDesc;
	CopyDMToDMByAlias(m_pDM_IDX,0,m_pParentDM,idx);
	

	//�����
	CString strPUB_FREQ;
	strPUB_FREQ = m_pDM_IDX->GetCellData(_T("ISS_�����"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("������ڵ�"),strPUB_FREQ,strDesc);
	m_pParentDM->SetCellData(_T("UDF_�����"),strDesc,idx);


	//�������
	CString strPUB_STATUS;
	strPUB_STATUS = m_pDM_IDX->GetCellData(_T("ISS_�������"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������ڵ�"),strPUB_STATUS,strDesc);
	m_pParentDM->SetCellData(_T("UDF_�������"),strDesc,idx);


	//��������
	CString strBINDING_YN = _T("");
	strBINDING_YN = m_pDM->GetCellData(_T("SS_��������"),0);
	m_pParentDM->SetCellData(_T("SS_��������"),strBINDING_YN,idx);

	
	//��ϱ���
	CString strREG_CODE = _T("");
	strREG_CODE = m_pDM->GetCellData(_T("SS_��ϱ���_�ڵ�"),0);
	m_pParentDM->SetCellData(_T("SS_��ϱ���_�ڵ�"),strREG_CODE,idx);


	//�̿����ѱ���
	CString strUSE_LIMIT_CODE = _T("");
	strUSE_LIMIT_CODE = m_pDM->GetCellData(_T("SS_�̿����ѱ���_�ڵ�"),0);
	m_pParentDM->SetCellData(_T("SS_�̿����ѱ���_�ڵ�"),strUSE_LIMIT_CODE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�̿����ѱ���"),strUSE_LIMIT_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_�̿����ѱ���"),strDesc,idx);
	

	
	//��������
	CString strMANAGE_CODE = _T("");
	strMANAGE_CODE = m_pDM->GetCellData(_T("SS_��������_�ڵ�"),0);
	m_pParentDM->SetCellData(_T("SS_��������_�ڵ�"),strMANAGE_CODE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������"),strMANAGE_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_��������"),strDesc,idx);
	
	

	//�ڷᱸ��
	CString strMAT_CODE = _T("");
	strMAT_CODE = m_pDM->GetCellData(_T("SS_�ڷᱸ��_�ڵ�"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�ڷᱸ��"),strMAT_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_�ڷᱸ��"),strDesc,idx);



	//�谡��ġ�ڵ�
	CString strSHELF_LOC_CODE = _T("");
	strSHELF_LOC_CODE = m_pDM->GetCellData(_T("SS_�谡��ġ_�ڵ�"),0);
	m_pParentDM->SetCellData(_T("SS_�谡��ġ_�ڵ�"),strSHELF_LOC_CODE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�ڷ�Ǳ���"),strSHELF_LOC_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_�ڷ��"),strDesc,idx);



	//�з���ȣ����
	CString strCLASS_NO_TYPE = _T("");
	strCLASS_NO_TYPE = m_pDM->GetCellData(_T("SS_�з���ȣ����_�ڵ�"),0);
	m_pParentDM->SetCellData(_T("SS_�з���ȣ����_�ڵ�"),strCLASS_NO_TYPE,idx);
	

	//ȭ�󱸺�
	CString strCURRENCY_CODE = _T("");
	strCURRENCY_CODE = m_pDM->GetCellData(_T("SS_ȭ�󱸺�_�ڵ�"),0);
	m_pParentDM->SetCellData(_T("SS_ȭ�󱸺�_�ڵ�"),strCURRENCY_CODE,idx);

	
	//����
	CString strPRICE = _T("");
	strPRICE = m_pDM->GetCellData(_T("SS_����"),0);
	m_pParentDM->SetCellData(_T("SS_����"),strPRICE,idx);
	

	CString strPURCHASE_YN;
	CString strDONATE_YN;
	strPURCHASE_YN = m_pDM->GetCellData(_T("SS_���Կ���"),0);
	strDONATE_YN = m_pDM->GetCellData(_T("SS_��������"),0);
		
	if(_T("Y") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
	{
		m_pParentDM->SetCellData(_T("UDF_���Ա���"),_T("����/����"),idx);
	}
	else if(_T("N") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
	{
		m_pParentDM->SetCellData(_T("UDF_���Ա���"),_T("����"),idx);
	}
	else if(_T("Y") == strPURCHASE_YN &&  _T("N") == strDONATE_YN)
	{
		m_pParentDM->SetCellData(_T("UDF_���Ա���"),_T("����"),idx);
	}		


	// �к�, ��ó�� �߰�
	// 2005.09.15 ������
	//
	CString strDept = _T("");
	CString strCource = _T("");
	if(m_nACQ_TYPE == 1)	
	{
		strDept = m_pPurchaseDlg->m_pDM->GetCellData(_T("SP_��û�к�"),0);
		strCource = m_pPurchaseDlg->m_pDM->GetCellData(_T("SP_��û�а�"),0);
	}
	else if (m_nACQ_TYPE == 2)
	{
		strDept = m_pDonateDlg->m_pDM->GetCellData(_T("SD_��û�к�"),0);
		strCource = m_pDonateDlg->m_pDM->GetCellData(_T("SD_��û�а�"),0);
	}

	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�к�"),strDept,strDesc);
	m_pParentDM->SetCellData(_T("UDF_��ó��"),strDesc,idx);	
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�а�"),strCource,strDesc);
	m_pParentDM->SetCellData(_T("UDF_�а���"),strDesc,idx);
	
	
	m_pParentGrid->DisplayLine(idx);

}

INT CSeSpeciesForm::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
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

VOID CSeSpeciesForm::CreateChildDialog()
{
	CRect rect;
	this->GetClientRect(rect);
	
	rect.top = 320;
	rect.bottom = 465;

	if(1 == m_nACQ_TYPE)
	{
		m_pPurchaseDlg = new CSePurchaseForm(this);
		m_pPurchaseDlg->m_nOpenMode = m_nOpenMode;
		m_pPurchaseDlg->m_bIsLightVersion = m_bIsLightVersion;
		m_pPurchaseDlg->m_bIsDepartMngUse = m_bIsDepartMngUse;
		m_pPurchaseDlg->Create((CESL_Mgr*)this);
		m_pPurchaseDlg->ShowWindow(SW_SHOW);
		m_pPurchaseDlg->MoveWindow( rect );
	}
	else if(2 == m_nACQ_TYPE)
	{
		m_pDonateDlg = new CSeDonateForm(this);
		m_pDonateDlg->m_bIsLightVersion = m_bIsLightVersion;
		m_pDonateDlg->Create((CESL_Mgr*)this);
		m_pDonateDlg->ShowWindow(SW_SHOW);
		m_pDonateDlg->MoveWindow( rect );

	}

}

VOID CSeSpeciesForm::FreeChildDialog()
{
	if(1 == m_nACQ_TYPE)
	{
		if(m_pPurchaseDlg) delete m_pPurchaseDlg; 
		m_pPurchaseDlg = NULL;
	}
	else if(2 == m_nACQ_TYPE)
	{	
		if(m_pDonateDlg) delete m_pDonateDlg; 
		m_pDonateDlg = NULL;
	}

	if(m_pExpectDlg) delete m_pExpectDlg;
	m_pExpectDlg = NULL;

}	

VOID CSeSpeciesForm::SaveAllDataToDM()
{
	if(m_nOpenMode>0) //�Է¸��
	{
		//���� Ȥ�� ���� ������ �����Ѵ� 		
		if(1 == m_nACQ_TYPE) {
		
			m_pPurchaseDlg->m_pDM->FreeData();
			m_pPurchaseDlg->m_pDM->InsertRow(-1);
			m_pPurchaseDlg->SaveAllDataToDM();
		
		} else {

			m_pDonateDlg->m_pDM->FreeData();
			m_pDonateDlg->m_pDM->InsertRow(-1);
			m_pDonateDlg->SaveAllDataToDM();
		}

		//��ũ����ü�� ������ ��� �´�
		SetMarcByControlData();


		//�׿��� data�� ��DM�� �ִ´�.
		m_pDM->FreeData();
		m_pDM->InsertRow(-1);
		SaveSpeciesDataToDM();

	}
	else   //�������
	{
		
		//���Ա����� ��ȭ(����->����, ����->����)�ǰų� ��Ÿ�� ���Ա����� ��� child�� INSERT�̴� 
		if(m_strPURCHASE_YN.IsEmpty() || m_strDONATE_YN.IsEmpty()) {
			AfxMessageBox(_T("���� ���� ������ ��Ȯ���� �ʽ��ϴ�!"));
			return;
		}
		

		if(1 == m_nACQ_TYPE)  //���� 
		{

			if(_T("Y") == m_strPURCHASE_YN)
			{
				m_pPurchaseDlg->SaveAllDataToDM();
			
			}
			else if(_T("N") == m_strPURCHASE_YN)
			{
				m_pPurchaseDlg->m_pDM->FreeData();
				m_pPurchaseDlg->m_pDM->InsertRow(-1);
				m_pPurchaseDlg->SaveAllDataToDM();

			}
		
		}
		else if(2 == m_nACQ_TYPE) //���� 
		{

			if(_T("Y") == m_strDONATE_YN)
			{

				m_pDonateDlg->SaveAllDataToDM();

			}
			else if(_T("N") == m_strDONATE_YN)
			{
				m_pDonateDlg->m_pDM->FreeData();
				m_pDonateDlg->m_pDM->InsertRow(-1);
				m_pDonateDlg->SaveAllDataToDM();

			}
		}
		

		//��ũ����ü�� ������ ��� �´�
		SetMarcByControlData();

		//�׿��� data�� ��DM�� �ִ´�.
		SaveSpeciesDataToDM();
	}


}

INT CSeSpeciesForm::SetMarcByControlData()
{
	//�Է°� ������忡 ���� ������ �ڼ��� �Ұ�!
	//����� ���� �����͸� ������ �ٽ� �Է��Ѵ�
	
	//���� ��ũ �׷� ����
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	CString strtmp;
	
	// ���� ��ũ leader ����
	CSeApi api(this);
	api.SetSerialMarcReader(&m_Marc);

	//���������͸� ����� �Է�
	const INT cnt = 14;
	CString strtag[cnt] = {
		_T("245$x"),	//1
		_T("440$a"),	//2
		_T("245$d"),	//3
		_T("260$a"),	//4	
		_T("260$b"),	//5	
		_T("030$a"),	//6
		_T("300$c"),	//7	
		_T("300$a"),	//8	
		_T("022$a"),	//9	
		_T("008@6"),	//10
		_T("008@7-10"),	//11
		_T("008@11-14"),//12
		_T("008@18"),	//13
		_T("008@19")};	//14
	

	for(INT i=0;i<cnt;i++)
	{
		m_pInfo->pMarcMgr->DeleteItem(&m_Marc,strtag[i]);
	}
	

	if(m_nOpenMode>0 || m_bIsDiscontinued || m_bIsTitleChanged) //������ ���� ��忡���� Ư�� ����
	{
		
		m_pInfo->pMarcMgr->DeleteItem(&m_Marc,_T("245$a"));
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
	}
	
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
	

	//�����ڰ� ���� �Ǿ��� ��� ����ÿ� 550tag�� �߰��Ѵ�.
	if(m_bIsPublisherChanged)  
	{
		m_pCM->GetControlMgrData(_T("������2"),strtmp);
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("550$a"),strtmp);
		m_bIsPublisherChanged = FALSE;
		strtmp = _T("");
	}


	m_pCM->GetControlMgrData(_T("CODEN"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("030$a"),strtmp);
	strtmp = _T("");
	
	m_pCM->GetControlMgrData(_T("ũ��"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("300$c"),strtmp);
	strtmp = _T("");
	
	m_pCM->GetControlMgrData(_T("�����"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("300$a"),strtmp);
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
	
	//�Է�����, ���̼�����  �Է¸�忡���� ���� 
	if(m_nOpenMode>0) 
	{
		//���� ���� (40�ڸ�)
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@39"),_T(" "));
		
		CTime t = CTime::GetCurrentTime();
		CString tmp,inputdate;
		tmp.Format(_T("%04d%02d%02d"),t.GetYear(),t.GetMonth(),t.GetDay());
		inputdate = tmp.Mid(2);
		m_pInfo->pMarcMgr->DeleteItem(&m_Marc,_T("008@0-5"));
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@0-5"),inputdate);
	}
	
	//���������(�������)�� ���� �󰣳⵵�� ���� ������ ���ش� 
	CString strPubType;
	m_pCM->GetControlMgrData(_T("�������"),strPubType);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@6"),strPubType);
	
	
	//â���� 
	m_pCM->GetControlMgrData(_T("â����"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@7-10"),strtmp);
	strtmp = _T("");

	
	//�󰣳� 
	if(strPubType == _T("d")) //�󰣵� ��� 
	{
		m_pCM->GetControlMgrData(_T("�󰣳�"),strtmp);
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@11-14"),strtmp);
		strtmp = _T("");
	}
	else 
	{
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@11-14"),_T("9999"));
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


	//���Լ�
	m_pCM->GetControlMgrData(_T("���Լ�"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@19"),strtmp);
	strtmp = _T("");
	
	
	m_pInfo->pMarcMgr->DeleteEmptySubfield(&m_Marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(&m_Marc);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&m_Marc);

	return 0;
}

INT CSeSpeciesForm::SetControlDataByMarc()
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

	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("030$a"),strtmp);
	m_pCM->SetControlMgrData(_T("CODEN"),strtmp);
	strtmp = _T("");
	
	
	//������2(550 tag �� �Ǹ����� ������)
	CArray<CString,CString&> arrayPublisher;
	CString strPublisher550 = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("550$a"),strPublisher550,&arrayPublisher);
	
	INT nSize = arrayPublisher.GetSize();
	if(nSize>0)
	{
		strPublisher550 = arrayPublisher.GetAt(nSize-1);
	}

	if(!strPublisher550.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("������2"),strPublisher550);
	}

	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("300$c"),strtmp);
	m_pCM->SetControlMgrData(_T("ũ��"),strtmp);
	strtmp = _T("");

	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("300$a"),strtmp);
	m_pCM->SetControlMgrData(_T("�����"),strtmp);
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

	
	
	//�����(310$a)ó��: �������� �Է��ϴ°�?


	//008 TAG ó��
	////////////////////////////////////////////////////////////////////////////
	
	//���������(�������)�� ���� �󰣳⵵�� ���� ������ ���ش� 
	CString strPubType = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@6"),strPubType);
	m_pCM->SetControlMgrData(_T("�������"),strPubType);
	
	
	//â���� -- ��ĭ�� ���� trim�ؼ� ��Ʈ�ѿ� ����
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@7-10"),strtmp);
	strtmp.TrimRight();
	strtmp.TrimLeft();
	m_pCM->SetControlMgrData(_T("â����"),strtmp);
	strtmp = _T("");

	
	//�󰣳� 
	if(strPubType == _T("d")) //�󰣵� ��츸 �󰣳� ����   -- ��ĭ�� ���� trim�ؼ� ��Ʈ�ѿ� ����
	{
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@11-14"),strtmp);
		strtmp.TrimRight();
		strtmp.TrimLeft();
		m_pCM->SetControlMgrData(_T("�󰣳�"),strtmp);		
		strtmp = _T("");
	}


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


	//���Լ�
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@19"),strtmp);
	m_pCM->SetControlMgrData(_T("���Լ�"),strtmp);
	strtmp = _T("");
	
	

	return 0;
}


INT CSeSpeciesForm::SaveSpeciesDataToHistoryDM(CString strType, CString strOldData, CString strNewData, CString strReason, CString strDate)
{
	CString strSPECIES_KEY;
	strSPECIES_KEY = m_pDM->GetCellData(_T("SS_��KEY"),0);
	
	CString strREC_KEY;
	m_pDM->MakeRecKey(strREC_KEY);
	
	// 2008.05.16 ADD BY PDJ
	// ������ ������ ���� �����Ͱ� �ߺ����� ����Ǹ鼭 ����.
	m_pDM_SPE_HISTORY->FreeData();
	//
	m_pDM_SPE_HISTORY->InsertRow(-1);
	INT nRow = m_pDM_SPE_HISTORY->GetRowCount()-1;
	
	m_pDM_SPE_HISTORY->SetCellData(_T("REC_KEY"),strREC_KEY,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("��KEY"),strSPECIES_KEY,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("Ÿ��"),strType,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("��������"),strOldData,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("��������"),strNewData,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("��������"),strDate,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("�۾���"),m_pInfo->USER_ID,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("�������"),strReason,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("�����۾�"),GetWorkLogMsg(WORK_LOG_SESPECIES),nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("�������۾�"),GetWorkLogMsg(WORK_LOG_SESPECIES),nRow);


	return 0;
}

INT CSeSpeciesForm::SaveSpeciesDataToDM()
{
	//DM���� ����������� data��
	const INT cnt = 19;
	CString alias[ cnt ] = {
		_T("SS_����"),					//	0
		_T("SS_���๰��Ϲ�ȣ"),		//	1
		_T("SS_�����_�ڵ�"),			//	2
		_T("SS_�������_�ڵ�"),			//	3
		_T("SS_��������_�ڵ�"),			//	4
		_T("SS_��ϱ���_�ڵ�"),			//	5
		_T("SS_��ü����_�ڵ�"),			//	6
		_T("SS_�谡��ġ_�ڵ�"),			//	7
		_T("SS_������ϱ���_�ڵ�"),		//	8
		_T("SS_�̿��󱸺�_�ڵ�"),		//	9
		_T("SS_�̿����ѱ���_�ڵ�"),		// 10
		_T("SS_�ڷᱸ��_�ڵ�"),			// 11
		_T("SS_�����ȣ"),				// 12
		_T("SS_�����ڷᱸ��_�ڵ�"),		// 13
		_T("SS_���ı���_�ڵ�"),			// 14	
		_T("SS_ȭ�󱸺�_�ڵ�"),			// 15
		_T("SS_���Ӻз��ڵ�1"),			// 16
		_T("SS_���Ӻз��ڵ�2"),			// 17
		_T("SS_���Ӻз��ڵ�3")};		// 18
	


	CString tmp;

	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");	
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM->SetCellData(alias[i],tmp,0);
	}
	
	UpdateData(TRUE);

	//���ݼ���
	tmp = _T("");
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbSPE_PRICE_PERSONALITY );
	pCombo->GetWindowText(tmp);
	m_pDM->SetCellData(_T("SS_���ݼ���"),tmp,0);

	
	//�������� 
	if(m_bBINDING_YN)
		m_pDM->SetCellData(_T("SS_��������"),_T("Y"),0);
	else 
		m_pDM->SetCellData(_T("SS_��������"),_T("N"),0);

	//�޸� 
	m_pDM->SetCellData(_T("SS_�޸�"),m_strMEMO,0);

	return 0;
}

INT CSeSpeciesForm::SaveAllData()
{
	//API �Լ����� �θ��� �Է½� ���������� �ִ°�� ���������� �����Ѵ�
	INT ids = 0;
	CSeApi api(this);	
	
	if(m_nOpenMode>0) 
	{

		//�Է� ��忡���� rec_key���� 
		CString strSPECIES_KEY,strACQ_KEY,strPUB_EXP_KEY,strACQ_EXP_KEY;
		m_pDM->MakeRecKey(strSPECIES_KEY);
		m_pDM->MakeRecKey(strACQ_KEY);
		m_pDM->MakeRecKey(strPUB_EXP_KEY);
		m_pDM->MakeRecKey(strACQ_EXP_KEY);

		if(strSPECIES_KEY.IsEmpty() || strACQ_KEY.IsEmpty() || strPUB_EXP_KEY.IsEmpty() || strACQ_EXP_KEY.IsEmpty())
		{
			AfxMessageBox(_T("������ Ű�� ������ �����Ͽ����ϴ�. �ٽ� ������ �ֽʽÿ�!")); 
			return -1;
		}
		
		m_pDM->SetCellData(_T("SS_��KEY"),strSPECIES_KEY,0);

		if(1 == m_nACQ_TYPE) { //�����Է�
			
			
			m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_��KEY"),strSPECIES_KEY,0);
			m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_���԰���KEY"),strACQ_KEY,0);
			m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_���࿹������KEY"),strPUB_EXP_KEY,0);
			m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_�Լ���������KEY"),strACQ_EXP_KEY,0);
			
			//���� ������ �Ǿ��� ��츸 �Է�
			if(m_nAcqExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_�Լ���������"),GetExpType(m_nAcqExpType),0);
			if(m_nPubExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_���࿹������"),GetExpType(m_nPubExpType),0);

			ids = api.SPECIES_PURCHASE_INSERT(m_pDM,m_pDM_IDX,m_pPurchaseDlg->m_pDM,m_pDM_SUB_HISTORY,&m_Marc);
	
		
		} else if(2 == m_nACQ_TYPE) { //�����Է�
			
			m_pDonateDlg->m_pDM->SetCellData(_T("SD_��KEY"),strSPECIES_KEY,0);
			m_pDonateDlg->m_pDM->SetCellData(_T("SD_��������KEY"),strACQ_KEY,0);
			m_pDonateDlg->m_pDM->SetCellData(_T("SD_���࿹������KEY"),strPUB_EXP_KEY,0);
			m_pDonateDlg->m_pDM->SetCellData(_T("SD_�Լ���������KEY"),strACQ_EXP_KEY,0);
			
			//���� ������ �Ǿ��� ��츸 �Է�(�������� �Է� ���� UPDATE������ �ٲٴ� �� ���)
			if(m_nAcqExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_�Լ���������"),GetExpType(m_nAcqExpType),0);
			if(m_nPubExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_���࿹������"),GetExpType(m_nPubExpType),0);

			ids = api.SPECIES_DONATE_INSERT(m_pDM,m_pDM_IDX,m_pDonateDlg->m_pDM,&m_Marc);
			
		}
		

		//�����ÿ��� ���� ���� �Է�
		if(ids<0) return ids;

		if(m_bIsExpVerified) //���������� ���� �Ǿ��� ���
		{
			INT ids2;
			CString strACQ_TYPE;
			strACQ_TYPE.Format(_T("%d"),m_nACQ_TYPE);

			ids2 = SaveExpectData(strPUB_EXP_KEY,strACQ_EXP_KEY,strACQ_KEY,strACQ_TYPE);
			if(ids2<0)
			{
				AfxMessageBox(_T("�������� �ʱ� �Է¿� ���� �Ͽ����ϴ�!"));
				return -2;
			}
		}

	}


	if(m_nOpenMode<0) //������� 
	{
		
 
		if(m_bIsDiscontinued) //��ó�� �Ǿ��� ��� 
		{
			ids = SaveDiscontinueData();
			return ids;
		}



		if(1 == m_nACQ_TYPE)  //���� 
		{

			if(_T("Y") == m_strPURCHASE_YN) //���� ���� 
			{
				//���� ������ �Ǿ��� ��츸 �Է�
				if(m_nAcqExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_�Լ���������"),GetExpType(m_nAcqExpType),0);
				if(m_nPubExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_���࿹������"),GetExpType(m_nPubExpType),0);
			
				ids = api.SPECIES_PURCHASE_UPDATE(m_pDM,
												  m_pDM_IDX,
												  m_pPurchaseDlg->m_pDM,
												  m_pDM_SUB_HISTORY,
												  m_pDM_SPE_HISTORY,
												  &m_Marc);

				if(ids<0) return ids;
		
				//���� ������ ���� �Ǿ��� ���� ���� ������ �Է��Ѵ�.
				if(m_pExpectDlg)
				{
					if(m_bIsExpVerified) 
					{
						//2004-12-24
						//modifield by loveisverb
						//ids = m_pExpectDlg->SaveAllData();
						ids = m_pExpectDlg->SaveAllData(m_nPubExpType,m_nAcqExpType);
					}
				}
				
				if(ids<0)
				{
					AfxMessageBox(_T("�������� ������ ���� �Ͽ����ϴ�!"));
					return -2;
				}
			
			}
			else if(_T("N") == m_strPURCHASE_YN) //���� ������ ���� ������ �Է�
			{
				
				m_pDM->SetCellData(_T("SS_���Կ���"),_T("Y"),0);
				CString strSPECIES_KEY,strACQ_KEY,strPUB_EXP_KEY,strACQ_EXP_KEY;
				strSPECIES_KEY = m_pDM->GetCellData(_T("SS_��KEY"),0);
				m_pDM->MakeRecKey(strACQ_KEY);
				m_pDM->MakeRecKey(strPUB_EXP_KEY);
				m_pDM->MakeRecKey(strACQ_EXP_KEY);
				

				m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_��KEY"),strSPECIES_KEY,0);
				m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_���԰���KEY"),strACQ_KEY,0);
				m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_���࿹������KEY"),strPUB_EXP_KEY,0);
				m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_�Լ���������KEY"),strACQ_EXP_KEY,0);
			

				//���� ������ �Ǿ��� ��츸 �Է�
				if(m_nAcqExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_�Լ���������"),GetExpType(m_nAcqExpType),0);
				if(m_nPubExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_���࿹������"),GetExpType(m_nPubExpType),0);

				ids = api.SPECIES_PURCHASE_UPDATE_ACQ_CHANGE(m_pDM,
															 m_pDM_IDX,
															 m_pPurchaseDlg->m_pDM,
															 m_pDM_SUB_HISTORY,
															 m_pDM_SPE_HISTORY,
															 &m_Marc);
				
				if(ids<0) return ids;
	
				if(m_bIsExpVerified) //���������� ���� �Ǿ��� ���
				{
					INT ids2;
					CString strACQ_TYPE;
					strACQ_TYPE.Format(_T("%d"),m_nACQ_TYPE);

					ids2 = SaveExpectData(strPUB_EXP_KEY,strACQ_EXP_KEY,strACQ_KEY,strACQ_TYPE);
					if(ids2<0)
					{
						AfxMessageBox(_T("�������� �ʱ� �Է¿� ���� �Ͽ����ϴ�!"));
						return -2;
					}
				}

			}
		
		}
		else if(2 == m_nACQ_TYPE) //���� 
		{

			if(_T("Y") == m_strDONATE_YN) //���� ���� 
			{
				//���� ������ ������ �Ǿ��� ��츸 �Է�(�������� �Է� ���� UPDATE������ �ٲٴ� �� ���)
				if(m_nAcqExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_�Լ���������"),GetExpType(m_nAcqExpType),0);
				if(m_nPubExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_���࿹������"),GetExpType(m_nPubExpType),0);
					
				ids = api.SPECIES_DONATE_UPDATE(m_pDM,
												m_pDM_IDX,
												m_pDonateDlg->m_pDM,
												m_pDM_SPE_HISTORY,
												&m_Marc);
				
				if(ids<0) return ids;

				//���� ������ ���� �Ǿ��� ���� ���� ������ �Է��Ѵ�.
				if(m_pExpectDlg)
				{
					//2004-12-24
					//modifield by loveisverb
					//ids = m_pExpectDlg->SaveAllData();
					ids = m_pExpectDlg->SaveAllData(m_nPubExpType,m_nAcqExpType);
				}
				
				if(ids<0)
				{
					AfxMessageBox(_T("�������� ������ ���� �Ͽ����ϴ�!"));
					return -2;
				}

			}
			else if(_T("N") == m_strDONATE_YN) //���� ������ ���� ������ �Է�
			{
				
				m_pDM->SetCellData(_T("SS_��������"),_T("Y"),0);
				CString strSPECIES_KEY,strACQ_KEY,strPUB_EXP_KEY,strACQ_EXP_KEY;
				strSPECIES_KEY = m_pDM->GetCellData(_T("SS_��KEY"),0);
				m_pDM->MakeRecKey(strACQ_KEY);
				m_pDM->MakeRecKey(strPUB_EXP_KEY);
				m_pDM->MakeRecKey(strACQ_EXP_KEY);
				
				m_pDonateDlg->m_pDM->SetCellData(_T("SD_��KEY"),strSPECIES_KEY,0);
				m_pDonateDlg->m_pDM->SetCellData(_T("SD_��������KEY"),strACQ_KEY,0);
				m_pDonateDlg->m_pDM->SetCellData(_T("SD_���࿹������KEY"),strPUB_EXP_KEY,0);
				m_pDonateDlg->m_pDM->SetCellData(_T("SD_�Լ���������KEY"),strACQ_EXP_KEY,0);
				
				//���� ������ �Ǿ��� ��츸 �Է�(�������� �Է� ���� UPDATE������ �ٲٴ� �� ���)
				if(m_nAcqExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_�Լ���������"),GetExpType(m_nAcqExpType),0);
				if(m_nPubExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_���࿹������"),GetExpType(m_nPubExpType),0);

				ids = api.SPECIES_DONATE_UPDATE_ACQ_CHANGE(m_pDM,
														   m_pDM_IDX,
														   m_pDonateDlg->m_pDM,
														   m_pDM_SPE_HISTORY,
														   &m_Marc);
				
				if(ids<0) return ids;

				if(m_bIsExpVerified) //���������� ���� �Ǿ��� ���
				{
					INT ids2;
					CString strACQ_TYPE;
					strACQ_TYPE.Format(_T("%d"),m_nACQ_TYPE);

					ids2 = SaveExpectData(strPUB_EXP_KEY,strACQ_EXP_KEY,strACQ_KEY,strACQ_TYPE);
					if(ids2<0)
					{
						AfxMessageBox(_T("�������� �ʱ� �Է¿� ���� �Ͽ����ϴ�!"));
						return -2;
					}
				}

			}
		}
	}
	

	//�μ��� �ֹ��ڷ� ����
	if(1 == m_nACQ_TYPE)  
	{
		m_pPurchaseDlg->SaveDepartmentOrderInfo();
	}

	return ids;
}


INT CSeSpeciesForm::SaveDiscontinueData()
{

	if(!m_pDiscontinueDlg) {
		AfxMessageBox(_T("�������� ã�� �� �����ϴ�!"));	
		return -1;
	}

	INT ids = 0;
	CSeApi api(this);

	CString strSPECIES_KEY,strACQ_KEY,strPUB_EXP_KEY,strACQ_EXP_KEY;
	m_pDM->MakeRecKey(strSPECIES_KEY);
	m_pDM->MakeRecKey(strACQ_KEY);
	m_pDM->MakeRecKey(strPUB_EXP_KEY);
	m_pDM->MakeRecKey(strACQ_EXP_KEY);
		
	m_pDM->SetCellData(_T("SS_��KEY"),strSPECIES_KEY,0);

	if(1 == m_nACQ_TYPE) //�����Է�
	{ 
			
		m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_��KEY"),strSPECIES_KEY,0);
		m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_���԰���KEY"),strACQ_KEY,0);
		m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_���࿹������KEY"),strPUB_EXP_KEY,0);
		m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_�Լ���������KEY"),strACQ_EXP_KEY,0);
				
		//���� ������ �Ǿ��� ��츸 �Է�
		if(m_nAcqExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_�Լ���������"),GetExpType(m_nAcqExpType),0);
		if(m_nPubExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_���࿹������"),GetExpType(m_nPubExpType),0);

		ids = api.SPECIES_PURCHASE_INSERT_DISCONTINUE(m_pDM,
													  m_pDM_IDX,
													  m_pPurchaseDlg->m_pDM,
													  m_pDM_SUB_HISTORY,
													  &m_Marc,
													  &m_pDiscontinueDlg->m_OldMarc,
													  m_pDiscontinueDlg->m_strSPECIES_KEY,
													  m_pDiscontinueDlg->m_strDiscontinueReason);
		
		
	} 
	else if(2 == m_nACQ_TYPE) //�����Է�
	{ 
			
		m_pDonateDlg->m_pDM->SetCellData(_T("SD_��KEY"),strSPECIES_KEY,0);
		m_pDonateDlg->m_pDM->SetCellData(_T("SD_��������KEY"),strACQ_KEY,0);
		m_pDonateDlg->m_pDM->SetCellData(_T("SD_���࿹������KEY"),strPUB_EXP_KEY,0);
		m_pDonateDlg->m_pDM->SetCellData(_T("SD_�Լ���������KEY"),strACQ_EXP_KEY,0);
				
		//���� ������ �Ǿ��� ��츸 �Է�(�������� �Է� ���� UPDATE������ �ٲٴ� �� ���)
		if(m_nAcqExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_�Լ���������"),GetExpType(m_nAcqExpType),0);
		if(m_nPubExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_���࿹������"),GetExpType(m_nPubExpType),0);
		
		
		ids = api.SPECIES_DONATE_INSERT_DISCONTINUE(m_pDM,
													m_pDM_IDX,
													m_pDonateDlg->m_pDM,
													&m_Marc,
													&m_pDiscontinueDlg->m_OldMarc,
													m_pDiscontinueDlg->m_strSPECIES_KEY,
													m_pDiscontinueDlg->m_strDiscontinueReason);											
			
	
	}
	
	//�����ÿ��� ���� ���� �Է�
	if(ids<0) return ids;

	if(m_bIsExpVerified) //���������� ���� �Ǿ��� ���
	{
		INT ids2;
		CString strACQ_TYPE;
		strACQ_TYPE.Format(_T("%d"),m_nACQ_TYPE);

		ids2 = SaveExpectData(strPUB_EXP_KEY,strACQ_EXP_KEY,strACQ_KEY,strACQ_TYPE);
		if(ids2<0)
		{
			AfxMessageBox(_T("�������� �ʱ� �Է¿� ���� �Ͽ����ϴ�!"));
			return -2;
		}
	}


	return 0;
}

CString CSeSpeciesForm::GetExpType(INT ExpType)
{
	CString strType = _T("");
	
	switch(ExpType)
	{
	case 0:
		strType = _T("M");
		break;
	case 1:
		strType = _T("W");
		break;
	case 2:
		strType = _T("D");
		break;
	case 3:
		strType = _T("E");
		break;
	default:
		break;
	}

	return strType;
}

INT CSeSpeciesForm::GetIntExpType(CString strExpType)
{
	
	switch(strExpType.GetAt(0))
	{
	case 'M':
		return 0;
		break;
	case 'W':
		return 1;
		break;
	case 'D':
		return 2;
		break;
	case 'E':
		return 3;
		break;
	default:
		break;
	}

	return -1;
}

CString CSeSpeciesForm::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CSeSpeciesForm::SaveExpectData(CString strPUB_EXP_KEY, CString strACQ_EXP_KEY, CString strACQ_KEY,CString strACQ_TYPE)
{
	//�Է¸�忡���� ����ϴ� �Լ��� 
	//������������ �ϰ� �Է� �Ѵ� 
	//�̹� �������� ������ ���� ��⿡�� �����ٴ� �����̴�
	CESL_DataMgr* pDM_DATE = NULL;
	CString strTBL_NAME = _T("");

	switch(m_nAcqExpType)
	{
	case 0:
		pDM_DATE = FindDM(_T("DM_��_��������"));
		strTBL_NAME = _T("SE_MONTH_EXP_TBL");
		break;
	case 1:
		pDM_DATE = FindDM(_T("DM_��_�ְ�����"));
		strTBL_NAME = _T("SE_WEEK_EXP_TBL");
		break;
	case 2:
		pDM_DATE = FindDM(_T("DM_��_�ϰ�����"));
		strTBL_NAME = _T("SE_DAY_EXP_TBL");
		break;
	case 3:
		pDM_DATE = FindDM(_T("DM_��_��Ÿ����"));
		strTBL_NAME = _T("SE_ETC_EXP_TBL");
		break;
	}

	
	CESL_DataMgr* pDM_VOL = FindDM(_T("DM_����_��_����_��ȣ"));
	CESL_DataMgr* pDM_VOL_SUPPORT = FindDM(_T("DM_����_��_����_��������ȣ"));
	CESL_DataMgr* pDM_APPENDIX = FindDM(_T("DM_����_��_����_�η�"));

	if(!pDM_DATE || !pDM_VOL || !pDM_VOL_SUPPORT || !pDM_APPENDIX) {
		AfxMessageBox(_T("�� ���� DM ���� ã�� �� �����ϴ�"));
		return -1;
	}

	pDM_DATE->StartFrame();
	CArray<CString,CString> RemoveAliasList;
	CString strEXP_CLASS = _T("");

	for(INT i=0;i<pDM_DATE->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_DATE->InitDBFieldData();
		strEXP_CLASS = pDM_DATE->GetCellData(_T("EXP_CLASS"),i);
	
		//�ش�Ǵ� KEY�� �Է� �Ѵ�.
		if(_T("0") == strEXP_CLASS) pDM_DATE->SetCellData(_T("REC_KEY"),strACQ_EXP_KEY,i);
		else if(_T("1") == strEXP_CLASS) pDM_DATE->SetCellData(_T("REC_KEY"),strPUB_EXP_KEY,i);

		pDM_DATE->MakeQueryByDM(RemoveAliasList, pDM_DATE , strTBL_NAME , i, pDM_DATE);
		
	}		
	
	CString strREC_KEY = _T("");
	INT nEXP_LEVEL_NO = 0;
	CString strEXP_LEVEL_NO;
	

	for(i=0;i<pDM_VOL->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_DATE->InitDBFieldData();
		pDM_DATE->MakeRecKey(strREC_KEY);			
		strEXP_LEVEL_NO.Format(_T("%d"),i+1);

		//���Ա��а�, ���Ա���KEY, ��ȣ����KEY(REC_KEY),�ܰ��ȣ�� �־��ش�
		pDM_VOL->SetCellData(_T("�ܰ��ȣ"),strEXP_LEVEL_NO,i);
		pDM_VOL->SetCellData(_T("���Ա���"),strACQ_TYPE,i);
		pDM_VOL->SetCellData(_T("���Ա���KEY"),strACQ_KEY,i);
		pDM_VOL->SetCellData(_T("��ȣ����KEY"),strREC_KEY,i);
		pDM_DATE->MakeQueryByDM(RemoveAliasList, pDM_VOL , _T("SE_VOL_EXP_TBL") , i, pDM_DATE);
		
		strREC_KEY = _T("");	
		nEXP_LEVEL_NO++;
	}		
	
	nEXP_LEVEL_NO = 0;

	for(i=0;i<pDM_VOL_SUPPORT->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_DATE->InitDBFieldData();
		pDM_DATE->MakeRecKey(strREC_KEY);
		strEXP_LEVEL_NO.Format(_T("%d"),i+1);
	
		//���Ա��а� ���Ա���KEY,��ȣ����KEY(REC_KEY),�ܰ��ȣ�� �־��ش�
		pDM_VOL_SUPPORT->SetCellData(_T("�ܰ��ȣ"),strEXP_LEVEL_NO,i);
		pDM_VOL_SUPPORT->SetCellData(_T("���Ա���"),strACQ_TYPE,i);
		pDM_VOL_SUPPORT->SetCellData(_T("���Ա���KEY"),strACQ_KEY,i);
		pDM_VOL_SUPPORT->SetCellData(_T("��ȣ����KEY"),strREC_KEY,i);
		pDM_DATE->MakeQueryByDM(RemoveAliasList, pDM_VOL_SUPPORT , _T("SE_VOL_EXP_TBL") , i, pDM_DATE);

		strREC_KEY = _T("");
		nEXP_LEVEL_NO++;
	}	
	
	for(i=0;i<pDM_APPENDIX->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_DATE->InitDBFieldData();
		pDM_DATE->MakeRecKey(strREC_KEY);	
	
		//���Ա��а� ���Ա���KEY,�ηϿ���KEY(REC_KEY)�� �־��ش�
		pDM_APPENDIX->SetCellData(_T("���Ա���"),strACQ_TYPE,i);
		pDM_APPENDIX->SetCellData(_T("���Ա���KEY"),strACQ_KEY,i);
		pDM_APPENDIX->SetCellData(_T("�ηϿ���KEY"),strREC_KEY,i);
		
		pDM_DATE->MakeQueryByDM(RemoveAliasList, pDM_APPENDIX , _T("SE_APPENDIX_EXP_TBL") , i, pDM_DATE);
		strREC_KEY = _T("");
	}	
	
	

	//���࿹���� �Լ������� ��ġ���� �ʴ� ��� �����Ͽ����߰��Է�
	if(m_nAcqExpType != m_nPubExpType) 
	{
		switch(m_nPubExpType)
		{
		case 0:
			pDM_DATE = FindDM(_T("DM_��_��������"));
			strTBL_NAME = _T("SE_MONTH_EXP_TBL");
			break;
		case 1:
			pDM_DATE = FindDM(_T("DM_��_�ְ�����"));
			strTBL_NAME = _T("SE_WEEK_EXP_TBL");
			break;
		case 2:
			pDM_DATE = FindDM(_T("DM_��_�ϰ�����"));
			strTBL_NAME = _T("SE_DAY_EXP_TBL");
			break;
		case 3:
			pDM_DATE = FindDM(_T("DM_��_��Ÿ����"));
			strTBL_NAME = _T("SE_ETC_EXP_TBL");
			break;
		}

		strEXP_CLASS = _T("");

		for(i=0;i<pDM_DATE->GetRowCount();i++)
		{
			RemoveAliasList.RemoveAll();
			pDM_DATE->InitDBFieldData();
			strEXP_CLASS = pDM_DATE->GetCellData(_T("EXP_CLASS"),i);
		
			//�ش�Ǵ� KEY�� �Է� �Ѵ�.
			if(_T("0") == strEXP_CLASS) pDM_DATE->SetCellData(_T("REC_KEY"),strACQ_EXP_KEY,i);
			else if(_T("1") == strEXP_CLASS) pDM_DATE->SetCellData(_T("REC_KEY"),strPUB_EXP_KEY,i);

			pDM_DATE->MakeQueryByDM(RemoveAliasList, pDM_DATE , strTBL_NAME , i, pDM_DATE);
		
		}		
	}
	
	INT ids;
	ids = pDM_DATE->SendFrame();
	pDM_DATE->EndFrame();

	return ids;
}

VOID CSeSpeciesForm::OnbtnMARCEDIT() 
{
	// TODO: Add your control notification handler code here

	SetMarcByControlData();
	CSeMarcEdit dlg(this);
	dlg.m_pMarc = &m_Marc;
	if(dlg.DoModal() == IDOK) SetControlDataByMarc();
	
}

VOID CSeSpeciesForm::OnbtnMEMO() 
{
	// TODO: Add your control notification handler code here
	
	CSeMemoEdit dlg(this);
	dlg.m_strMEMO = m_strMEMO;
	if(dlg.DoModal() == IDOK) m_strMEMO = dlg.m_strMEMO;

}

VOID CSeSpeciesForm::OnbtnEXPECT() 
{
	// TODO: Add your control notification handler code here
	CString strPUB_FREQ = _T("");
	CString strTITLE = _T("");
	m_pCM->GetControlMgrData(_T("�����"),strPUB_FREQ);
	m_pCM->GetControlMgrData(_T("����"),strTITLE);
	INT nOpenMode;
	BOOL bCopyType = FALSE;

	if(m_nOpenMode<0)
	{
		if(1 == m_nACQ_TYPE)  //���� 
		{
			if(_T("Y") == m_strPURCHASE_YN) nOpenMode = m_nOpenMode;
			else if(_T("N") == m_strPURCHASE_YN) 
			{ 
				nOpenMode = 1;
				bCopyType = TRUE;
			}
		}
		else if(2 == m_nACQ_TYPE) //���� 
		{
			if(_T("Y") == m_strDONATE_YN) nOpenMode = m_nOpenMode;
			else if(_T("N") == m_strDONATE_YN)
			{ 
				nOpenMode = 1;
				bCopyType = TRUE;
			}
		}
	}
	else 
	{
		nOpenMode = m_nOpenMode;
	}
	

	if(false == m_bIsExpCreated) {
		
		m_pExpectDlg = new CSeExpectForm(this);
		m_pExpectDlg->m_nOpenMode = nOpenMode;
		m_pExpectDlg->m_nACQ_TYPE = m_nACQ_TYPE;
		m_pExpectDlg->m_strSPECIES_KEY = m_strSpeciesKey;
		m_pExpectDlg->m_strPUB_FREQ = strPUB_FREQ;
		m_pExpectDlg->m_strTITLE = strTITLE;
		m_pExpectDlg->m_bIsVerified = m_bIsExpVerified; 

		if(bCopyType) {
			m_pExpectDlg->m_nAcqType = m_nAcqExpType;
			m_pExpectDlg->m_nPubType = m_nPubExpType;
		}

		if(m_pExpectDlg->DoModal() == IDOK)
		{
			//���� �������� ������ �����´�
			m_nPubExpType = m_pExpectDlg->m_nPubType;
			m_nAcqExpType = m_pExpectDlg->m_nAcqType;
			m_bIsExpVerified = TRUE;
		}
		
		m_bIsExpCreated = true;
	}
	else  
	{
		m_pExpectDlg->m_bIsFirstCreated = false;
		m_pExpectDlg->m_strPUB_FREQ = strPUB_FREQ;
		m_pExpectDlg->m_strTITLE = strTITLE;
		m_pExpectDlg->m_strSPECIES_KEY = m_strSpeciesKey;
		m_pExpectDlg->m_bIsVerified = m_bIsExpVerified; //���� ���� 

		if(m_pExpectDlg->DoModal() == IDOK)
		{
			//���� �������� ������ �����´�
			m_nPubExpType = m_pExpectDlg->m_nPubType;
			m_nAcqExpType = m_pExpectDlg->m_nAcqType;
			m_bIsExpVerified = TRUE;
		}
	}
	
}

VOID CSeSpeciesForm::OnbtnHISTORY() 
{
	// TODO: Add your control notification handler code here
	CSE_HISTORY_MAIN dlg(this);
	dlg.sCurrentKey = m_pDM_IDX->GetCellData(_T("ISS_��KEY"),0);
	dlg.sCurrentTitle = m_pDM_IDX->GetCellData(_T("ISS_ǥ������"),0);
	dlg.DoModal();

}

VOID CSeSpeciesForm::OnbtnMARCIMPORT() 
{
	// TODO: Add your control notification handler code here
	if(m_nOpenMode<0) return;

	CSeMarcImport dlg(this);
	if(dlg.DoModal() != IDOK) return;
	
	
	INT nType = dlg.m_nImportOption;
	
	if(0 == nType) //�⺻���� ���� 
	{

		m_Marc = dlg.m_Marc;
		SetControlDataByMarc();
		m_Marc.Init();
		SetMarcByControlData();

	}
	else if(1 == nType) //��ü ���� ����
	{

		m_Marc = dlg.m_Marc;
		SetControlDataByMarc();

	}

}

VOID CSeSpeciesForm::OnOK() 
{
	// 2007.04.13 ADD BY PDJ
	// ������ ����
	/*
	CString strDonator;
	CString strQuery;
	CString Result;
	INT		nCount;
	m_pDonateDlg->m_pCM->GetControlMgrData(_T("SD_�����ڸ�"),strDonator);
	strDonator.Replace(_T(" "),_T(""));
	if ( !strDonator.IsEmpty() )
	{
		strQuery.Format(_T("SELECT COUNT(1) FROM CO_DONATOR_TBL WHERE DONATOR = '%s'"), strDonator);
		m_pDM->GetOneValueQuery(strQuery, Result);

		nCount = _ttoi(Result.GetBuffer(0));
		if ( nCount == 0 )
		{
			AfxMessageBox(_T("�������� �ʴ� ������ ������ ������ �� �����ϴ�."));
			return;
		}
		else if ( nCount > 1 )
		{
			AfxMessageBox(_T("�������̸��� �ߺ��˴ϴ�. \n �˻��Ͽ� ������ �ֽʽÿ�"));
			return;
		}
	}
	*/
	
	//�ʼ� �Է� ����

	//����
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

	
	//�ڷ��
	CString strSHELF_LOC_CODE;
	m_pCM->GetControlMgrData(_T("SS_�谡��ġ_�ڵ�"),strSHELF_LOC_CODE);

	if(strSHELF_LOC_CODE.IsEmpty()) 
	{
		AfxMessageBox(_T("�ڷ���� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}
	
	//����� 
	CString strPUB_FREQ;
	m_pCM->GetControlMgrData(_T("�����"),strPUB_FREQ);
	strPUB_FREQ.TrimLeft();
	strPUB_FREQ.TrimRight();

	if(strPUB_FREQ.IsEmpty())
	{
		AfxMessageBox(_T("����󵵰� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}
	
	//������� 
	CString strPUB_STATUS;
	m_pCM->GetControlMgrData(_T("�������"),strPUB_STATUS);
	strPUB_STATUS.TrimLeft();
	strPUB_STATUS.TrimRight();

	if(strPUB_STATUS.IsEmpty())
	{
		AfxMessageBox(_T("������°� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}

	//â����,�󰣳� (�����Ұ�� 4�ڸ� check)
	CString strFoundationYear, strDiscontinueYear;
	m_pCM->GetControlMgrData(_T("â����"),strFoundationYear);
	m_pCM->GetControlMgrData(_T("�󰣳�"),strDiscontinueYear);
	
	strFoundationYear.TrimLeft();
	strFoundationYear.TrimRight();
	strDiscontinueYear.TrimLeft();
	strDiscontinueYear.TrimRight();
	INT nLength = 0;

	if(!strFoundationYear.IsEmpty())
	{
		nLength = strFoundationYear.GetLength();
		if(4 != nLength) 
		{
			AfxMessageBox(_T("â���⿡ 4�ڸ��� ���� �־� �ֽʽÿ�!"));
			return;
		}
	}

	if(!strDiscontinueYear.IsEmpty())
	{
		nLength = strDiscontinueYear.GetLength();
		if(4 != nLength) 
		{
			AfxMessageBox(_T("�󰣳⿡ 4�ڸ��� ���� �־� �ֽʽÿ�!"));
			return;
		}
	}


	//�Լ�å��
	CString strACQ_BOOK_CNT;
	if(1 == m_nACQ_TYPE) m_pPurchaseDlg->m_pCM->GetControlMgrData(_T("SP_�Լ�å��"),strACQ_BOOK_CNT);
	else if(2 == m_nACQ_TYPE) m_pDonateDlg->m_pCM->GetControlMgrData(_T("SD_�Լ�å��"),strACQ_BOOK_CNT);

	strACQ_BOOK_CNT.TrimLeft();
	strACQ_BOOK_CNT.TrimRight();

	if(strACQ_BOOK_CNT.IsEmpty())
	{
		AfxMessageBox(_T("�Լ�å���� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}
	else 
	{
		INT nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
		if(nACQ_BOOK_CNT<1)
		{
			AfxMessageBox(_T("�Լ�å���� 1 �̻��� �Ǿ� �մϴ�!"));
			return;
		}
	}

	// 2008.05.15 ADD BY PDJ
	// �����Ⱓ�� ��¥�� ���������� �Էµǵ�����.
	if ( m_pPurchaseDlg != NULL )
	{
		CString strStartDate;
		CString strEndDate;
		m_pPurchaseDlg->m_pCM->GetControlMgrData(_T("SP_����������"),strStartDate);
		m_pPurchaseDlg->m_pCM->GetControlMgrData(_T("SP_����������"),strEndDate);
		if ( strStartDate > strEndDate )
		{
			AfxMessageBox(_T("�����Ⱓ�� �߸��Ǿ����ϴ�."));
			return;
		}
	}

	//DM�� ��ũ ����ü�� �����͸� �ִ´�.
	SaveAllDataToDM();

	//API�� ���� DB operation�� �Ѵ�	
	if(SaveAllData()<0) 
	{
		AfxMessageBox(_T("���� ����!"));
	}
	else 
	{
		AfxMessageBox(_T("����Ǿ����ϴ�"));	
		//�θ�ȭ�� refresh
		RefreshParent();
	}
	
	if(m_nOpenMode>0) //�Է�
	{
		//��� ������ �ʱ�ȭ �Ѵ�. 
		
		//ȭ���� �����
		OnbtnALLCLEAR();

		//��ũ�� �����.
		m_Marc.Init();
		
		//�޸� ����� 
		m_strMEMO = _T("");

		//�����ȣ 
		m_pCM->SetControlMgrData(_T("SS_�����ȣ"),_T("<<�ڵ� �ο�>>")); 

		//���������� �����.
		if(m_pExpectDlg) delete m_pExpectDlg;
		m_pExpectDlg = NULL;
		m_bIsExpCreated = FALSE;
		m_bIsExpVerified = FALSE;
		m_nPubExpType = -1;  
		m_nAcqExpType = -1;  
		
		//DM �� ��� ����� 
		FreeAllDM();

		return;
	}
	else //���� 
	{
		//�󰣿���
		if(m_pDiscontinueDlg)
		{
			delete m_pDiscontinueDlg;
			m_pDiscontinueDlg = NULL;
		}

		m_bIsDiscontinued = FALSE;
	
		//������ ���� 
		m_bIsTitleChanged = FALSE;

		//������ ���� ����
		m_bIsPublisherChanged = FALSE;

		
		//loveisverb
		//���������� �����.
		if(m_pExpectDlg) delete m_pExpectDlg;
		m_pExpectDlg = NULL;
		m_bIsExpCreated = FALSE;
		m_bIsExpVerified = FALSE;
		m_nPubExpType = -1;  
		m_nAcqExpType = -1; 
	
		//�������� DM �� ��� ����� 
		FreeAllDM();

		//���� ���� ����
		m_strPURCHASE_YN = m_pDM->GetCellData(_T("SS_���Կ���"),0);
		m_strDONATE_YN = m_pDM->GetCellData(_T("SS_��������"),0);

		return;
	}

	CDialog::OnOK();
}

VOID CSeSpeciesForm::OnbtnPREV() 
{
	// TODO: Add your control notification handler code here
	
	
	//�Է¸���� ���� �������� �ҷ�����
	if(m_nOpenMode>0)
	{
		BringPrevData();
		return;
	}
	
	
	
	if(1 == m_nMoveOption)
	{
		m_nCurrentIdx--;

		if(m_nCurrentIdx<0)
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

	m_strSpeciesKey = m_pParentDM->GetCellData(_T("ISS_��KEY"),m_nCurrentIdx);
	if(!m_strSpeciesKey.IsEmpty())
			RefreshAll(m_strSpeciesKey);

	m_pParentGrid->SetReverse(m_nCurrentIdx);
	
}

VOID CSeSpeciesForm::OnbtnNEXT() 
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


	m_strSpeciesKey = m_pParentDM->GetCellData(_T("ISS_��KEY"),m_nCurrentIdx);
	if(!m_strSpeciesKey.IsEmpty())
			RefreshAll(m_strSpeciesKey);

	m_pParentGrid->SetReverse(m_nCurrentIdx);

}

VOID CSeSpeciesForm::OnbtnALLCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();
	m_bBINDING_YN = FALSE;
	UpdateData(FALSE);

	
	if(1 == m_nACQ_TYPE) m_pPurchaseDlg->ClearAll();
	else if(2 == m_nACQ_TYPE) m_pDonateDlg->ClearAll();

}

VOID CSeSpeciesForm::OnbtnSPETITLECHANGE() 
{
	// TODO: Add your control notification handler code here
	// ���� ���� 
	// ���� �� ���θ� ���´� 
	
	CString msg;
	msg.Format(_T("���� �������� ���� ���Ͻðڽ��ϱ�?"));
	
	INT nReturn = AfxMessageBox(msg,MB_YESNOCANCEL);

	//��ũ �׷� ����
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

	if(IDYES == nReturn) //�� ó�� 
	{
		
		//������¸� �ľ��� �̹� �� ó�� �� �������� Ȯ���Ѵ�
		CString tmp;
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@6"),tmp); 
		if(_T("d") == tmp) {
			AfxMessageBox(_T("�̹� �󰣵� ���Դϴ�!"));
			return;
		}

		m_pDiscontinueDlg = new CSeTitleChangeDiscontinueDlg(this);

		//DM
		m_pDiscontinueDlg->m_pDM = m_pDM;

		CString strOldTitle = _T("");
		CString strOldISSN1 = _T("");
		CString strOldISSN2 = _T("");
		CString strNewTitle = _T("");
		CString strNewISSN2 = _T("");
		CString strNewISSN1 = _T("");
		CString strISSN = _T("");
		
		//���� 
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("245$a"),strOldTitle);
		m_pDiscontinueDlg->m_strOldTitle = strOldTitle;
		
		//ISSN
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("022$a"),strISSN);
		INT npos = strISSN.Find(_T("-"));
		if(npos>0)
		{
			strOldISSN1 = strISSN.Left(npos);
			strOldISSN2 = strISSN.Mid(npos+1);

			strOldISSN1.TrimLeft();	strOldISSN1.TrimRight();
			strOldISSN2.TrimLeft();	strOldISSN2.TrimRight();
			
			m_pDiscontinueDlg->m_strOldISSN1 = strOldISSN1;
			m_pDiscontinueDlg->m_strOldISSN2 = strOldISSN2;
		}

		//��ũ 
		m_pDiscontinueDlg->m_OldMarc = m_Marc;
		
		//��ũ ������ 
		m_pDiscontinueDlg->m_pNewMarc = &m_Marc;

		//��Ÿ ����.
		m_pDiscontinueDlg->m_nACQ_TYPE = m_nACQ_TYPE;

		if(m_pDiscontinueDlg->DoModal() != IDOK) return;
		
		//���� ���� ���� 
		m_pCM->SetControlMgrData(_T("����"),m_pDiscontinueDlg->m_strNewTitle);
		m_pCM->SetControlMgrData(_T("ISSN1"),m_pDiscontinueDlg->m_strNewISSN1);
		m_pCM->SetControlMgrData(_T("ISSN2"),m_pDiscontinueDlg->m_strNewISSN2);

		//�� ó�� �Ѵ�
		m_bIsDiscontinued = TRUE;

		//���� ���������� ������ �´� 
		BringExpData(m_pDiscontinueDlg->m_strSPECIES_KEY,m_nACQ_TYPE);
		
		AfxMessageBox(_T("���ο� �� ������ �Է��� �ֽʽÿ�!"));
		
	}
	else if(IDNO == nReturn) //�Ϲ� ���� ���� 
	{

		CSeTitleChangeDlg dlg(this);
		dlg.m_pDM = m_pDM;

		CString strOldData = _T("");
		CString strNewData = _T("");

		//���� ������ 
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("245$a"),strOldData);
		dlg.m_strOldData = strOldData;

		if(dlg.DoModal() != IDOK) return;
		
		//���� ������
		strNewData = dlg.m_strNewData;
		
		strNewData.TrimLeft();
		strNewData.TrimRight();

		//���� ���� �ɼ��� �ľ��Ѵ� 
		INT nOption = dlg.m_nRadioOption;
		
		if(0 == nOption) //245$a�� ��� 
		{
			m_pCM->SetControlMgrData(_T("����"),strNewData);
			m_pInfo->pMarcMgr->DeleteItem(&m_Marc,_T("245$a"));	
			
			if(strNewData.GetAt(0) == '(') //��Ī�� ���� ��쿡�� ���ñ�ȣ 20
			{
				m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a!20"),strNewData);
			}
			else
			{
				m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a"),strNewData);
			}
			
			m_bIsTitleChanged = TRUE;
		}
		else if(1 == nOption) //247$a�� ���� ���� �̵�
		{
			m_pCM->SetControlMgrData(_T("����"),strNewData);
			m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("247$a"),strOldData);
			m_pInfo->pMarcMgr->DeleteItem(&m_Marc,_T("245$a"));	

			if(strNewData.GetAt(0) == '(') //��Ī�� ���� ��쿡�� ���ñ�ȣ 20
			{
				m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a!20"),strNewData);
			}
			else
			{
				m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a"),strNewData);
			}

			SaveSpeciesDataToHistoryDM(_T("1"),strOldData,strNewData,dlg.m_strReason,dlg.m_strDate);
			m_bIsTitleChanged = TRUE;
		}
		else if(2 == nOption) //246$a�� ���� ǥ�� ���
		{
			m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("246$a"),strNewData);	
			SaveSpeciesDataToHistoryDM(_T("1"),strOldData,strNewData,dlg.m_strReason,dlg.m_strDate);
		}

	}


}

VOID CSeSpeciesForm::OnbtnSPEMEDIACODECHANGE() 
{
	// TODO: Add your control notification handler code here
	// ��ü ���� ���� 	
	CSeMediaChangeDlg dlg(this);
	dlg.m_pDM = m_pDM;

	CString strOldData = _T("");
	CString strNewData = _T("");

	//���� ������ 
	strOldData = m_pDM->GetCellData(_T("SS_��ü����_�ڵ�"),0);
	dlg.m_strOldData = strOldData;

	if(dlg.DoModal() != IDOK) return;
	
	//���� ������
	strNewData = dlg.m_strNewData;
	m_pCM->SetControlMgrData(_T("SS_��ü����_�ڵ�"),strNewData);

	SaveSpeciesDataToHistoryDM(_T("3"),strOldData,strNewData,dlg.m_strReason,dlg.m_strDate);


}


VOID CSeSpeciesForm::OnbtnSPEPUBLISHERCHANGE() 
{
	// TODO: Add your control notification handler code here
	// ������ ���� 
	CSePublisherChangeDlg dlg(this);
	dlg.m_pDM = m_pDM;

	CString strOldData = _T("");
	CString strNewData = _T("");
	

	//���� ������ (550�� ���� ���� 550 �ƴϸ� 260) 
	//550 tag �� �Ǹ����� ������
	CArray<CString,CString&> arrayPublisher;
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("550$a"),strOldData,&arrayPublisher);
	
	INT nSize = arrayPublisher.GetSize();
	
	if(nSize>0)
	{
		strOldData = arrayPublisher.GetAt(nSize-1);
	}
	else 
	{
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("260$b"),strOldData);
	}
		
	dlg.m_strOldData = strOldData;

	if(dlg.DoModal() != IDOK) return;
	
	//���� ������
	strNewData = dlg.m_strNewData;
	m_pCM->SetControlMgrData(_T("������2"),strNewData);
	m_bIsPublisherChanged = TRUE;

	SaveSpeciesDataToHistoryDM(_T("2"),strOldData,strNewData,dlg.m_strReason,dlg.m_strDate);

}


VOID CSeSpeciesForm::OnbtnSPEPUBFREQCHANGE() 
{
	// TODO: Add your control notification handler code here
	// ����� ���� 	
	CSePubfreqChangeDlg dlg(this);
	dlg.m_pDM = m_pDM;

	CString strOldData = _T("");
	CString strNewData = _T("");

	//���� ������ 
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@18"),strOldData);
	dlg.m_strOldData = strOldData;

	if(dlg.DoModal() != IDOK) return;
	
	//���� ������
	strNewData = dlg.m_strNewData;
	m_pCM->SetControlMgrData(_T("�����"),strNewData);

	SaveSpeciesDataToHistoryDM(_T("4"),strOldData,strNewData,dlg.m_strReason,dlg.m_strDate);
		

	//�������������� �����
	DeleteExpData(m_strSpeciesKey,m_nACQ_TYPE);

	//modified by loveisverb
	//2004-12-27
	//���������� �����.
	if(m_pExpectDlg) delete m_pExpectDlg;
	m_pExpectDlg = NULL;
	m_bIsExpCreated = FALSE;
	m_bIsExpVerified = FALSE;
	m_nPubExpType = -1;  
	m_nAcqExpType = -1; 
	
	//�������� DM �� ��� ����� 
	FreeAllDM();


	AfxMessageBox(_T("������� �������� ���� \n ���� ���������� ���� �Ǿ����ϴ�!"));

}

VOID CSeSpeciesForm::FreeAllDM()
{
	//�Է°��� ��� DM�� ã�Ƽ� ����� 

	//loveisverb
	if(m_nOpenMode>0)
	{
		m_pDM->FreeData();
		m_pDM_IDX->FreeData();

		if(1 == m_nACQ_TYPE) m_pPurchaseDlg->m_pDM->FreeData();
		else if(2 == m_nACQ_TYPE) m_pDonateDlg->m_pDM->FreeData();
	}

	CESL_DataMgr* pDM_MONTH = FindDM(_T("DM_��_��������"));
	CESL_DataMgr* pDM_WEEK = FindDM(_T("DM_��_�ְ�����"));
	CESL_DataMgr* pDM_DAY = FindDM(_T("DM_��_�ϰ�����"));
	CESL_DataMgr* pDM_ETC = FindDM(_T("DM_��_��Ÿ����"));
	CESL_DataMgr* pDM_VOL = FindDM(_T("DM_����_��_����_��ȣ"));
	CESL_DataMgr* pDM_OTHER_VOL = FindDM(_T("DM_����_��_����_��������ȣ"));
	CESL_DataMgr* pDM_APPENDIX = FindDM(_T("DM_����_��_����_�η�"));


	if(!pDM_MONTH || !pDM_WEEK || !pDM_DAY || !pDM_ETC || !pDM_VOL || !pDM_OTHER_VOL || !pDM_APPENDIX)
	{
		AfxMessageBox(_T("���������� ������ DM�� ã�� ���߽��ϴ�!")); 	
		return;
	}

	pDM_MONTH->FreeData();
	pDM_WEEK->FreeData();
	pDM_DAY->FreeData();	
	pDM_ETC->FreeData();	
	pDM_VOL->FreeData();
	pDM_OTHER_VOL->FreeData();	
	pDM_APPENDIX->FreeData();
}

HBRUSH CSeSpeciesForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO: Change any attributes of the DC here	

	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	INT nID = pWnd->GetDlgCtrlID();
	if (IsColorControl(nID)) 
	{
		return SetColor(pDC);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

VOID CSeSpeciesForm::OnbtnCOMMONLIST() 
{
	// TODO: Add your control notification handler code here
	
	// ������� ���� 
	CUC_MARC_IMPORT dlg(this);
	if (dlg.DoModal() != IDOK) return;
	
	CMarc tmpmarc;
	if(dlg.sMarc.IsEmpty()) return;

	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	INT ids = m_pInfo->pMarcMgr->Decoding(dlg.sMarc, &tmpmarc);
	
	if (ids < 0) {
		AfxMessageBox(_T("���Կ� �����Ͽ����ϴ�!"));
		return;
	}
	
	//���Կ� ���ʿ��� ��ũ �±� ������ �������ش�
	const INT cnt = 5;
	CString strfiled[cnt] = {
		_T("001"),	//0
		_T("049"),	//1	
		_T("090"),	//2
		_T("980"),	//3	
		_T("052")	//4	
	};	
	
	for(INT i=0;i<cnt;i++)
	{
		m_pInfo->pMarcMgr->DeleteField(&tmpmarc,strfiled[i]);
	}

	m_Marc.Init();
	m_Marc = tmpmarc;
	SetControlDataByMarc();

}


VOID CSeSpeciesForm::OnKillfocuscmbSPESHELPLOCCODE() 
{
	// TODO: Add your control notification handler code here

	m_bIsLastCtrlFocused = FALSE;

}

VOID CSeSpeciesForm::OnChildLastKillFocus(WPARAM w,LPARAM l)
{
	//child�� ������ ��Ʈ���� killfocus�Ǹ� ù��° ��Ʈ�ѿ� setfocus 
	GetDlgItem(IDC_cmbSPE_CONTROL_TYPE)->SetFocus();

}

VOID CSeSpeciesForm::OnbtnSPETITLEDUPINSPECT() 
{
	// TODO: Add your control notification handler code here
	// �Է½� ���Ǵ� ���� ��������
	CString strTitle;
	m_pCM->GetControlMgrData(_T("����"),strTitle);
	
	if(strTitle.IsEmpty())
	{
		AfxMessageBox(_T("������ �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}

	CSeDuplicatedTitleDlg dlg(this);
	dlg.m_strCurrentTitle =  strTitle;
	dlg.DoModal();
}

VOID CSeSpeciesForm::OnSetfocuscmbSPESHELPLOCCODE() 
{
	// TODO: Add your control notification handler code here
	m_bIsLastCtrlFocused = TRUE;	
}

BOOL CSeSpeciesForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_TAB)
		{
			if(m_bIsLastCtrlFocused)
			{
				//�ڷ�ǿ��� killfocus �Ǹ� ���� Ȥ�� �����ڷ��� ù��° ��Ʈ�ѿ� setfocus�Ѵ�
				if(1 == m_nACQ_TYPE) m_pPurchaseDlg->SetFocusOnFirstCtrl();
				else if(2 == m_nACQ_TYPE) m_pDonateDlg->SetFocusOnFirstCtrl();

				return TRUE;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSeSpeciesForm::ShowUserDefineCode()
{
	CString strYN, strAlias;

	CWnd *pWnd = NULL;

	CESL_DataMgr * TmpDM;
	TmpDM = FindDM(_T("DM_����_��"));

	// ����� ���� �ڵ�1 ��� ����.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 103"), strYN);
	if(strYN == _T("Y"))
	{
		// 2005.09.14�ڵ�� �ٲ��ش�.
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 103"), strAlias);
		//=====================================================
		//2009.01.29 UPDATE BY PJW : strAlias�� ��������� �߰����� �ʴ´�.
// 		pWnd = GetDlgItem(IDC_STATIC_SPE13);        pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
// 		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE1);		pWnd->ShowWindow(SW_SHOW);
		if(!strAlias.IsEmpty())
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE13);        pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE1);		pWnd->ShowWindow(SW_SHOW);
		}
		else
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE13);		pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE1);		pWnd->ShowWindow(SW_HIDE);
		}
		//=====================================================
		
	}
	else
	{
		pWnd = GetDlgItem(IDC_STATIC_SPE13);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE1);		pWnd->ShowWindow(SW_HIDE);
	}

	// ����� ���� �ڵ�2 ��� ����.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 101"), strYN);
	if(strYN == _T("Y"))
	{
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 101"), strAlias);
		//=====================================================
		//2009.01.29 UPDATE BY PJW : strAlias�� ��������� �߰����� �ʴ´�.
// 		pWnd = GetDlgItem(IDC_STATIC_SPE11);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
// 		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE2);		pWnd->ShowWindow(SW_SHOW);
		if(!strAlias.IsEmpty())
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE11);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE2);		pWnd->ShowWindow(SW_SHOW);
		}
		else
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE11);		pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE2);		pWnd->ShowWindow(SW_HIDE);
		}
		//=====================================================
		
	}
	else
	{
		pWnd = GetDlgItem(IDC_STATIC_SPE11);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE2);		pWnd->ShowWindow(SW_HIDE);
	}

	// ����� ���� �ڵ�3 ��� ����.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 102"), strYN);
	if(strYN == _T("Y"))
	{
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 102"), strAlias);
		//=====================================================
		//2009.01.29 UPDATE BY PJW : strAlias�� ��������� �߰����� �ʴ´�.
// 		pWnd = GetDlgItem(IDC_STATIC_SPE12);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
// 		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE3);		pWnd->ShowWindow(SW_SHOW);
		if(!strAlias.IsEmpty())
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE12);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE3);		pWnd->ShowWindow(SW_SHOW);
		}
		else
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE12);		pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE3);		pWnd->ShowWindow(SW_HIDE);
		}
		//=====================================================
		
	}
	else
	{
		pWnd = GetDlgItem(IDC_STATIC_SPE12);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE3);		pWnd->ShowWindow(SW_HIDE);
	}
}
