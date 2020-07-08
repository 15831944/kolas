// SeVolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeVolDlg.h"
#include "SeBookDlg.h"
#include "SeAppendixDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeVolDlg dialog


CSeVolDlg::CSeVolDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeVolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeVolDlg)
	m_bBINDING_YN = FALSE;
	m_bREAD_FIRST_YN = FALSE;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pParentGrid = NULL;
	m_nCurrentParentIdx = -1;
	m_nOpenMode = 1; //default
	m_nACQ_TYPE = 1; //default
	m_strUDF = _T("");
	m_bIsNonPublished = FALSE;
	
	m_strTITLE_INFO = _T(""); 
	m_strAUTHOR_INFO = _T("");
	m_strPUB_INFO = _T("");

	m_nMoveOption = 1; //default

	m_bIsLightVersion = FALSE;

	m_bKillfocusLack = FALSE;

	m_bIsBookCntIncreased = FALSE;
}

CSeVolDlg::~CSeVolDlg()
{
}

VOID CSeVolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeVolDlg)
	DDX_Check(pDX, IDC_chkVOL_BINDING_YN, m_bBINDING_YN);
	DDX_Check(pDX, IDC_chkVOL_READ_FIRST_YN, m_bREAD_FIRST_YN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeVolDlg, CDialog)
	//{{AFX_MSG_MAP(CSeVolDlg)
	ON_BN_CLICKED(IDC_btnVOL_PREV, OnbtnVOLPREV)
	ON_BN_CLICKED(IDC_btnVOL_NEXT, OnbtnVOLNEXT)
	ON_EN_KILLFOCUS(IDC_edtVOL_ACQ_BOOK_CNT, OnKillfocusedtVOLACQBOOKCNT)
	ON_EN_KILLFOCUS(IDC_edtVOL_MISS_BOOK_CNT, OnKillfocusedtVOLMISSBOOKCNT)
	ON_BN_CLICKED(IDC_btnVOL_BOOK_INFO, OnbtnVOLBOOKINFO)
	ON_BN_CLICKED(IDC_btnVOL_APPENDIX_INFO, OnbtnVOLAPPENDIXINFO)
	ON_CBN_SELCHANGE(IDC_cmbVOL_VOLUME_CLASS, OnSelchangecmbVOLVOLUMECLASS)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeVolDlg message handlers

BOOL CSeVolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( InitESL_Mgr(_T("����_����_��")) < 0 ) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_��") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_����_��"));
	m_pDM = FindDM(_T("DM_üũ��_�۾�"));
	
	if(!m_pCM || !m_pDM || !m_pParentGrid) {
		AfxMessageBox( _T("CM,DM,Gird Error : ����_��,����_��_�۾�") );
		return FALSE;
	}
	
	InitControls();

	if(InitOpenMode()<0)
	{
		EndDialog(0);
		return FALSE;
	}
	
	//����Ʈ ���� ����
	if(m_bIsLightVersion)
	{
		SetLightVersion();
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSeVolDlg::SetLightVersion()
{
	//����Ʈ ������ ��� �����켱���ο� �������� ��Ʈ���� ���ش� 
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_chkVOL_READ_FIRST_YN);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_chkVOL_BINDING_YN);
	if(pWnd) pWnd->ShowWindow(FALSE);

}

INT CSeVolDlg::InitControls()
{
	//���� ����
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_PRICE_CHARACTER );
	
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("����") );
		pCombo->InsertString( 1, _T("���ǰ") );
		pCombo->InsertString( 2, _T("���ݺҸ�") );
		pCombo->SetCurSel( 0 );
	}

	//��ȣ ����
	//1: �Ϲ� 2: �պ�ȣ 3: �̹���
	pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );

	if(pCombo)
	{
		pCombo->InsertString( 0, _T("�Ϲ�") );
		pCombo->InsertString( 1, _T("�պ�ȣ") );
		pCombo->InsertString( 2, _T("�̹���") );
		pCombo->SetCurSel( 0 );
	}
	
	return 0;
}

INT CSeVolDlg::InitOpenMode()
{

	if(m_nOpenMode>0) //�Է� 
	{
		m_nCurrentParentIdx = m_pDM->GetRowCount()-1;
		
		m_strUDF = m_pDM->GetCellData(_T("UDF_����"),m_nCurrentParentIdx);
		HideOrShowControl();
		ShowAllData();
	}
	else if(-1 == m_nOpenMode)  //�Ϲ� ���� 
	{
		
		if(m_nCurrentParentIdx<0) {
			AfxMessageBox(_T("������ ������ �����ϴ�!"));
			return -1;
		}
		
		m_strUDF = m_pDM->GetCellData(_T("UDF_����"),m_nCurrentParentIdx);
		HideOrShowControl();
		ShowAllData();
	}
	else if(-2 == m_nOpenMode) //�պ�ȣ
	{
		m_pParentGrid->SelectMakeList();
		m_nCurrentParentIdx = m_pParentGrid->SelectGetHeadPosition();
		m_strUDF = m_pDM->GetCellData(_T("UDF_����"),m_nCurrentParentIdx);

		HideOrShowControl();
		
		//�ϴ� ������ �ڷḦ �������� ��� ������ �������� 
		ShowAllData();
		
		//��ȣ������ �պ�ȣ�� �����ش�
		CComboBox * pCombo;
		pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
		pCombo->SetCurSel(1);
		pCombo->EnableWindow(FALSE);

		//��ȣ, ������ ��ȣ�� �����ؼ� �����ش�.
		CString strVOL_TITLE = _T("");
		CString strOTHER_VOL_TITLE = _T("");
		CString tmp;
		INT idx = m_pParentGrid->SelectGetTailPosition();
		
		while(idx>=0)
		{
			tmp = _T("");
			tmp = m_pDM->GetCellData(_T("SV_��ȣ��"),idx);
			
			if(!tmp.IsEmpty()) {
				
				if(strVOL_TITLE.IsEmpty()) 
				{
					strVOL_TITLE += tmp;
				}
				else
				{
					strVOL_TITLE += _T("-");
					strVOL_TITLE += tmp;
				}
            }
			
			tmp = _T("");
			tmp = m_pDM->GetCellData(_T("SV_��������ȣ��"),idx);
			
			if(!tmp.IsEmpty()) {
				
				if(strOTHER_VOL_TITLE.IsEmpty()) 
				{
					strOTHER_VOL_TITLE += tmp;
				}
				else
				{
					strOTHER_VOL_TITLE += _T("-");
					strOTHER_VOL_TITLE += tmp;
				}
            }
	
			idx = m_pParentGrid->SelectGetPrev();
		}
	

		m_pCM->SetControlMgrData(_T("SV_��ȣ��"),strVOL_TITLE,m_nCurrentParentIdx);
		m_pCM->SetControlMgrData(_T("SV_��������ȣ��"),strOTHER_VOL_TITLE,m_nCurrentParentIdx);
	}
	


	return 0;
}

INT CSeVolDlg::HideOrShowControl()
{
	if(m_nOpenMode>0) //�Է�
	{
		GetDlgItem(IDC_btnVOL_PREV)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_NEXT)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(FALSE);
		
	}
	else if(-1 == m_nOpenMode) //�Ϲ� ���� 
	{
		CEdit* pEdit;
		pEdit = (CEdit*)GetDlgItem(IDC_edtVOL_ACQ_EXP_BOOK_CNT);
		if(pEdit) pEdit->SetReadOnly();

		pEdit = (CEdit*)GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT);
		if(pEdit) pEdit->SetReadOnly(FALSE);
			
		pEdit = (CEdit*)GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT);
		if(pEdit) pEdit->SetReadOnly(FALSE);

		
		CString strVOLUME_CLASS = _T("");
		strVOLUME_CLASS = m_pDM->GetCellData(_T("SV_��ȣ����"),m_nCurrentParentIdx);	
		if(_T("3") ==strVOLUME_CLASS) //�̹���
		{
			//�̹��� ��쿡�� å, �η�  ��ư�� ������ �Լ�, ��ȣ å������ read-onlyȭ �Ѵ�
			GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(FALSE);
			GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(FALSE);
			
			pEdit = (CEdit*)GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT);
			if(pEdit) pEdit->SetReadOnly();
			
			pEdit = (CEdit*)GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT);
			if(pEdit) pEdit->SetReadOnly();
			
			return 0;
		}

		
		if(_T("E") == m_strUDF || _T("C") == m_strUDF || _T("M") == m_strUDF)
		{
			//���� ������ ��쿡�� å, �η�  ��ư�� ����� 
			GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(FALSE);
			GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(FALSE);
			
		}
		else 
		{
			CString strCHECKIN_STATUS;
			strCHECKIN_STATUS = m_pDM->GetCellData(_T("SA_�Լ�����"),m_nCurrentParentIdx);	
			if(_T("2") == strCHECKIN_STATUS) //��ȣ�ڷ� 
			{
				GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(FALSE);
				GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(FALSE);
		
			}
			else //�Լ� �Ǵ� �̴��ڷ�  
			{
				GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(TRUE);
				GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(TRUE);
			}
		}
	}
	else if(-2 == m_nOpenMode) //�պ�ȣ 
	{
		GetDlgItem(IDC_btnVOL_PREV)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_NEXT)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(FALSE);
	}


	return 0;
}

INT CSeVolDlg::ShowAllData()
{
	//��ȣ���� 
	CString strVOLUME_CLASS;
	strVOLUME_CLASS = m_pDM->GetCellData(_T("SV_��ȣ����"),m_nCurrentParentIdx);
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
	BOOL bBookCntSetReadOnly = FALSE;
	
	if(_T("1") == strVOLUME_CLASS) 	
	{
		pCombo->SetCurSel( 0 );
		pCombo->EnableWindow(TRUE);
	}
	else if(_T("2") == strVOLUME_CLASS)
	{
		pCombo->SetCurSel( 1 );
		pCombo->EnableWindow(TRUE);
	}
	else if(_T("3") == strVOLUME_CLASS) //�̹��� �ڷ�
	{
		pCombo->SetCurSel( 2 );
		pCombo->EnableWindow(FALSE);
		bBookCntSetReadOnly = TRUE;
	}
	
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT);

	if(bBookCntSetReadOnly) //�̹����ڷ�� �Լ�å���� ��ȣå���� read only�Ѵ�
	{
		pEdit1->SetReadOnly(TRUE);
		pEdit2->SetReadOnly(TRUE);
	}
	else
	{
		pEdit1->SetReadOnly(FALSE);
		pEdit2->SetReadOnly(FALSE);
	}


	//���� ����
	CString strPRICE_CHARACTER;
	strPRICE_CHARACTER = m_pDM->GetCellData(_T("SV_���ݼ���"),m_nCurrentParentIdx);
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_PRICE_CHARACTER );
	if(_T("���ǰ") == strPRICE_CHARACTER) 	pCombo->SetCurSel( 1 );
	else if(_T("���ݺҸ�") == strPRICE_CHARACTER) 	pCombo->SetCurSel( 2 );
	else pCombo->SetCurSel( 0 );
	
	
	//�����켱����
	CString strREAD_FIRST_YN;
	strREAD_FIRST_YN = m_pDM->GetCellData(_T("UDF_�����켱����"),m_nCurrentParentIdx);
	
	if(_T("Y") == strREAD_FIRST_YN)
	{
		m_bREAD_FIRST_YN = TRUE;
	}
	else if(_T("N") == strREAD_FIRST_YN)
	{
		m_bREAD_FIRST_YN = FALSE;
	}
	
	//��������
	CString strBINDING_YN;
	strBINDING_YN = m_pDM->GetCellData(_T("SS_��������"),m_nCurrentParentIdx);
	
	if(_T("Y") == strBINDING_YN)
	{
		m_bBINDING_YN = TRUE;
	}
	else if(_T("N") == strBINDING_YN)
	{
		m_bBINDING_YN = FALSE;
	}
	

	m_pCM->AllControlDisplay(m_nCurrentParentIdx);
	
	if(m_nOpenMode<0) {
		SetGridRowBoldFont(m_pParentGrid, m_nCurrentParentIdx, TRUE);
	}
		
	UpdateData(FALSE);

	return 0;
}

INT CSeVolDlg::SaveAllDataToDM()
{
	const INT cnt  = 19;
	CString alias[cnt] = {
			_T("SV_����"),
			_T("SA_��ȣå��"),
			_T("SV_��ȣ��"),
			_T("SV_��ȣ������"),
			_T("SV_��ȣ�Ϸù�ȣ"),
			_T("SV_�����ڷ�"),
			_T("SV_��ü����_�ڵ�"),
			_T("SV_�����"),
			_T("SV_������Ư��"),
			_T("SV_������"),
			_T("SV_��ȣǥ��"),
			_T("SV_��������ȣ��"),
			_T("SV_���"),
			_T("UDF_�Լ���"),
			_T("SA_�Լ�å��"),
			_T("SV_åũ��"),
			_T("SV_���ı���_�ڵ�"),
			_T("SV_ȭ�󱸺�"),
			_T("SA_�Լ�������")};

	CString tmp;
	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");	
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM->SetCellData(alias[i],tmp,m_nCurrentParentIdx);
	}
	
	//���� ���� 
	CString strPRICE_CHARACTER = _T("");
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_PRICE_CHARACTER );
	if(pCombo) pCombo->GetWindowText(strPRICE_CHARACTER);
	m_pDM->SetCellData(_T("SV_���ݼ���"),strPRICE_CHARACTER,m_nCurrentParentIdx);
	

	//��ȣ����
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
	if(pCombo) {
		
		INT nCurSel = pCombo->GetCurSel();
		
		if(0 == nCurSel) //�Ϲ�
		{
			m_pDM->SetCellData(_T("SV_��ȣ����"),_T("1"),m_nCurrentParentIdx);
			m_pDM->SetCellData(_T("UDF_��ȣ����"),_T("�Ϲ�"),m_nCurrentParentIdx);
		}
		else if(1 == nCurSel) //�պ�ȣ
		{
	
			m_pDM->SetCellData(_T("SV_��ȣ����"),_T("2"),m_nCurrentParentIdx);
			m_pDM->SetCellData(_T("UDF_��ȣ����"),_T("�պ�ȣ"),m_nCurrentParentIdx);
		}
	}
	
	if(m_bIsNonPublished) //�űԷι̹��� ó�� �� ��� 
	{
		m_pDM->SetCellData(_T("UDF_�Լ�����"),_T("�Լ�"),m_nCurrentParentIdx);
		m_pDM->SetCellData(_T("UDF_��ȣ����"),_T("�̹���"),m_nCurrentParentIdx);
		m_pDM->SetCellData(_T("SV_��ȣ����"),_T("3"),m_nCurrentParentIdx); //�̹���
		m_pDM->SetCellData(_T("SA_�Լ�å��"),_T("0"),m_nCurrentParentIdx);
		m_pDM->SetCellData(_T("SA_��ȣå��"),_T("0"),m_nCurrentParentIdx);

		m_pCM->ControlDisplay(_T("SA_�Լ�å��"),m_nCurrentParentIdx);
		m_pCM->ControlDisplay(_T("SA_��ȣå��"),m_nCurrentParentIdx);

		m_bIsNonPublished = FALSE;
	}


	//UDF_���� ó��
	CString strACQ_BOOK_CNT;
	CString strMISS_BOOK_CNT;
	m_pCM->GetControlMgrData(_T("SA_�Լ�å��"),strACQ_BOOK_CNT);
	m_pCM->GetControlMgrData(_T("SA_��ȣå��"),strMISS_BOOK_CNT);
	INT nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	INT nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));
	


	if(_T("D") == m_strUDF || _T("U") == m_strUDF) //�����ڷ�
	{
		if(0 == nACQ_BOOK_CNT)
		{
			if(0 == nMISS_BOOK_CNT)
			{
				
				CString strVOLUME_CLASS = _T("");
				strVOLUME_CLASS = m_pDM->GetCellData(_T("SV_��ȣ����"),m_nCurrentParentIdx);
				if(_T("3") != strVOLUME_CLASS) //�̹���
				{
					AfxMessageBox(_T("���� ��ȣ�� �ƴ� ��쿡��  �Լ� å���� ��ȣ å���� \n ���ÿ� 0 �� �� �� �����ϴ�!"));
					GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT)->SetFocus();
					return -1;	
				}
			}
			else //��ȣ�ڷ� 
			{
				m_pDM->SetCellData(_T("UDF_�Լ�����"),_T("��ȣ"),m_nCurrentParentIdx);
			}

		}
		else 
		{
			if(0 == nMISS_BOOK_CNT) //�Լ��ڷ� 
				m_pDM->SetCellData(_T("UDF_�Լ�����"),_T("�Լ�"),m_nCurrentParentIdx);
			else //�̴��ڷ� 
				m_pDM->SetCellData(_T("UDF_�Լ�����"),_T("�̴�"),m_nCurrentParentIdx);

		}

		m_pDM->SetCellData(_T("UDF_����"), _T("U"),m_nCurrentParentIdx);

	}
	else if(_T("E") == m_strUDF || _T("C") == m_strUDF || _T("M") == m_strUDF) //�����ڷ�  
	{
	
		if(0 == nACQ_BOOK_CNT)
		{
			if(0 == nMISS_BOOK_CNT)
			{
				m_pDM->SetCellData(_T("UDF_�Լ�����"),_T("����"),m_nCurrentParentIdx);
				m_pDM->SetCellData(_T("UDF_����"), _T("E"),m_nCurrentParentIdx);
	
			}
			else //��ȣ�ڷ� 
			{
				m_pDM->SetCellData(_T("UDF_�Լ�����"),_T("��ȣ(����)"),m_nCurrentParentIdx);
				m_pDM->SetCellData(_T("UDF_����"), _T("M"),m_nCurrentParentIdx);
			}
		}
		else 
		{
			if(0 == nMISS_BOOK_CNT) //�Լ��ڷ� 
			{
				m_pDM->SetCellData(_T("UDF_�Լ�����"),_T("�Լ�(����)"),m_nCurrentParentIdx);
				m_pDM->SetCellData(_T("UDF_����"), _T("C"),m_nCurrentParentIdx);
			}
			else //�̴��ڷ� 
			{
				m_pDM->SetCellData(_T("UDF_�Լ�����"),_T("�̴�(����)"),m_nCurrentParentIdx);
				m_pDM->SetCellData(_T("UDF_����"), _T("C"),m_nCurrentParentIdx);
			}

		}	

	}
	
	m_pCM->ControlDisplay(_T("UDF_�Լ�����"),m_nCurrentParentIdx);
	m_pParentGrid->DisplayLine(m_nCurrentParentIdx);
	if(-2 != m_nOpenMode) AfxMessageBox(_T("�����Ͽ����ϴ�!")); //�պ�ȣ ó���� �޽����� ������ �ʴ´�

	return 0;
}

VOID CSeVolDlg::OnbtnVOLPREV() 
{
	// TODO: Add your control notification handler code here
	INT m_nBeforeIdx = m_nCurrentParentIdx;

	if(1 == m_nMoveOption)
	{
		m_nCurrentParentIdx--;

		if(m_nCurrentParentIdx < 0)
		{
			AfxMessageBox(_T("ó�� �׸��Դϴ�!"));
			m_nCurrentParentIdx++;
			return;
		}
		
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetPrev();

		if(nIdx<0) 
		{
			INT nHeadIdx = m_pParentGrid->SelectGetHeadPosition();
			
			if(m_nCurrentParentIdx == nHeadIdx)
			{
				AfxMessageBox(_T("ó�� �׸��Դϴ�!"));
				m_pParentGrid->SelectGetHeadPosition();
				return;
			}
			else //���� ���� ������ ���� ��� �� ���� ���������� ���Ѵ� 
			{
				m_nCurrentParentIdx = nHeadIdx;
			}
		
		}
		else //���� ������ �������� �����δ�
		{
			m_nCurrentParentIdx = nIdx;
		}

	}

	if(m_nBeforeIdx != m_nCurrentParentIdx) //���� �ε����� �۾� �β��� ������� �Ѵ� 
	{
		if(m_nBeforeIdx>=0) SetGridRowBoldFont(m_pParentGrid, m_nBeforeIdx, FALSE);
	}
	
	m_strUDF = m_pDM->GetCellData(_T("UDF_����"),m_nCurrentParentIdx);
	HideOrShowControl();
	ShowAllData();

}

VOID CSeVolDlg::OnbtnVOLNEXT() 
{
	// TODO: Add your control notification handler code here
	INT m_nBeforeIdx = m_nCurrentParentIdx;

	if(1 == m_nMoveOption)
	{
		m_nCurrentParentIdx++;
		INT nMaxIdx = m_pDM->GetRowCount()-1;
		if(m_nCurrentParentIdx > nMaxIdx)
		{
			AfxMessageBox(_T("������ �׸��Դϴ�!"));
			m_nCurrentParentIdx--;
			return;
		}
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetNext();

		if(nIdx<0) 
		{
			INT nTailIdx = m_pParentGrid->SelectGetTailPosition();
			
			if(m_nCurrentParentIdx == nTailIdx)
			{
				AfxMessageBox(_T("������ �׸��Դϴ�!"));
				m_pParentGrid->SelectGetTailPosition();
				return;
			}
			else //���� ���� ������ ���� ��� �ǵ��� ���������� ���Ѵ� 
			{
				m_nCurrentParentIdx = nTailIdx;
			}
		}
		else //���� ������ �������� �����δ�
		{
			m_nCurrentParentIdx = nIdx;
		}
	}

	if(m_nBeforeIdx != m_nCurrentParentIdx) //���� �ε����� �۾� �β��� ������� �Ѵ� 
	{
		if(m_nBeforeIdx>=0) SetGridRowBoldFont(m_pParentGrid, m_nBeforeIdx, FALSE);
	}
	
	m_strUDF = m_pDM->GetCellData(_T("UDF_����"),m_nCurrentParentIdx);
	HideOrShowControl();
	ShowAllData();

}

VOID CSeVolDlg::OnOK() 
{
	if( VerifyAllData() >= 0 )	SaveAllDataToDM();
	
	// 2005-10-11 Ȳ���� : �Ʒ��ڵ� ����
	//	if(-1 == m_nOpenMode) return; //�Ϲ� ���� 

	CDialog::OnOK();
}

INT CSeVolDlg::VerifyAllData()
{
	
	//�Լ� å���� �����ߴ����� Ȯ���� ������ȣ�� �ʿ� ������ Ȯ���Ѵ�. 
	CString strNewAcqBookCnt;
	CString strOldAcqBookCnt;
	INT nNewAcqBookCnt = 0;
	INT nOldAcqBookCnt = 0;

	m_pCM->GetControlMgrData(_T("SA_�Լ�å��"),strNewAcqBookCnt);
	strOldAcqBookCnt = m_pDM->GetCellData(_T("SA_�Լ�å��"),m_nCurrentParentIdx);

	nNewAcqBookCnt = _ttoi(strNewAcqBookCnt.GetBuffer(0));
	nOldAcqBookCnt = _ttoi(strOldAcqBookCnt.GetBuffer(0));

	if(nNewAcqBookCnt>nOldAcqBookCnt) m_bIsBookCntIncreased = TRUE;
	
	return 0;
}

VOID CSeVolDlg::OnKillfocusedtVOLACQBOOKCNT() 
{
	// TODO: Add your control notification handler code here
	// �Լ� å���� 0 �� �Ǹ� �� ���� ��ȣ �ڷᰡ �ȴ� 
	// ��ȣ å�� ������ ������ �Ѵ� 0 �� �ƴϸ� �� �ڷ�� å�� �̴��� �ȴ�. 
	// �̹����� ���� �� ���ǿ��� �����ȴ�
	
	
	CString strVOLUME_CLASS;
	strVOLUME_CLASS = m_pDM->GetCellData(_T("SV_��ȣ����"),m_nCurrentParentIdx);

	if(_T("3") != strVOLUME_CLASS) //�̹����� �ƴ� ���
	{

		CString strACQ_BOOK_CNT;
		CString strMISS_BOOK_CNT;
		
		m_pCM->GetControlMgrData(_T("SA_�Լ�å��"),strACQ_BOOK_CNT);
		m_pCM->GetControlMgrData(_T("SA_��ȣå��"),strMISS_BOOK_CNT);
		
		if(strACQ_BOOK_CNT.IsEmpty())
		{
			AfxMessageBox(_T("�Լ�å������ ��ĭ�� �ɼ� �����ϴ�!"));
			GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT)->SetFocus();
			return;
		}

		INT nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
		INT nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));
		

		if(_T("D") == m_strUDF || _T("U") == m_strUDF) //�����ڷ�
		{
			if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT)
			{
				if(!m_bKillfocusLack)
				{
					AfxMessageBox(_T("���� ��ȣ�� �ƴ� ��쿡��  �Լ� å���� ��ȣ å���� \n ���ÿ� 0 �� �� �� �����ϴ�!"));
					GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT)->SetFocus();
					m_bKillfocusLack = TRUE;
					return;	
				}

				m_bKillfocusLack = FALSE;
			
			}
		}
	}

}

VOID CSeVolDlg::OnKillfocusedtVOLMISSBOOKCNT() 
{
	// TODO: Add your control notification handler code here
	// ��ȣ �ڷᰡ 0 �� �Ǹ� �� ���� �Լ��ڷᰡ �ȴ�.
	// ��ȣ å�� ������ ������ �Ѵ� 0 �� �ƴϸ� �� �ڷ�� å�� �̴��� �ȴ�. 
	// �̹����� ���� �� ���ǿ��� �����ȴ�
	
	
	CString strVOLUME_CLASS;
	strVOLUME_CLASS = m_pDM->GetCellData(_T("SV_��ȣ����"),m_nCurrentParentIdx);

	if(_T("3") != strVOLUME_CLASS) //�̹����� �ƴ� ���
	{
		CString strACQ_BOOK_CNT;
		CString strMISS_BOOK_CNT;
		
		m_pCM->GetControlMgrData(_T("SA_�Լ�å��"),strACQ_BOOK_CNT);
		m_pCM->GetControlMgrData(_T("SA_��ȣå��"),strMISS_BOOK_CNT);
		
		if(strMISS_BOOK_CNT.IsEmpty())
		{
			AfxMessageBox(_T("��ȣå������ ��ĭ�� �ɼ� �����ϴ�!"));
			GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT)->SetFocus();
			return;
		}

		INT nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
		INT nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));
		

		if(_T("D") == m_strUDF || _T("U") == m_strUDF) //�����ڷ�
		{
			if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT)
			{
				if(!m_bKillfocusLack)
				{

					AfxMessageBox(_T("���� ��ȣ�� �ƴ� ��쿡��  �Լ� å���� ��ȣ å���� \n ���ÿ� 0 �� �� �� �����ϴ�!"));
					GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT)->SetFocus();
					m_bKillfocusLack = TRUE;
					return;		
				}

				m_bKillfocusLack = FALSE;
			}
		}
	}
}

VOID CSeVolDlg::OnbtnVOLBOOKINFO() 
{
	// TODO: Add your control notification handler code here
	
	CSeBookDlg dlg(this);
	dlg.m_strVOL_TITLE = m_pDM->GetCellData(_T("SV_��ȣ��"),m_nCurrentParentIdx);
	dlg.m_strACQ_VOL_KEY = m_pDM->GetCellData(_T("SA_���Ա��к���KEY"),m_nCurrentParentIdx);
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_strTITLE_INFO = m_strTITLE_INFO;
	dlg.m_strAUTHOR_INFO = m_strAUTHOR_INFO;
	dlg.m_strPUB_INFO = m_strPUB_INFO;
	dlg.m_bIsLightVersion = m_bIsLightVersion;
	
	INT ids = dlg.DoModal();

	//�� ���� operation ����� â�� �ݴ´�
	if(11 == ids)
	{
		EndDialog(11);
	}

}

VOID CSeVolDlg::OnbtnVOLAPPENDIXINFO() 
{
	// TODO: Add your control notification handler code here
	CSeAppendixDlg dlg(this);
	dlg.strVOL_TITLE = m_pDM->GetCellData(_T("SV_��ȣ��"),m_nCurrentParentIdx);
	dlg.strACQ_VOL_KEY = m_pDM->GetCellData(_T("SA_���Ա��к���KEY"),m_nCurrentParentIdx);
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_nCurrentParentIdx = m_nCurrentParentIdx;
	dlg.DoModal();
	
	//�η� ���� update(WORK DM, DB DM);
	CString strArticleCnt;
	strArticleCnt = dlg.m_strNewArticleCnt;
	m_pDM->SetCellData(_T("UDF_�ηϼ�"),strArticleCnt,m_nCurrentParentIdx);

	CString strACQ_VOL_KEY_WORK;
	CString strACQ_VOL_KEY_DB;
	strACQ_VOL_KEY_WORK = m_pDM->GetCellData(_T("SA_���Ա��к���KEY"),m_nCurrentParentIdx);
	
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_üũ��_DB"));
	if(!pDM) return;
	

	//�ϴ��� �˻� ������� 
	for(INT i=0;i<pDM->GetRowCount();i++)
	{
		strACQ_VOL_KEY_DB = _T("");
		strACQ_VOL_KEY_DB = pDM->GetCellData(_T("SA_���Ա��к���KEY"),i);
					
		if(strACQ_VOL_KEY_WORK == strACQ_VOL_KEY_DB) {
			pDM->SetCellData(_T("UDF_�ηϼ�"),strArticleCnt,i);
		}

	}
	
	//�θ� �׸��� refresh
	m_pParentGrid->DisplayLine(m_nCurrentParentIdx);

}

VOID CSeVolDlg::SetGridRowBoldFont(CESL_Grid* pGrid, INT nRowIdx, BOOL bNewValue)
{
	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	
	if (nRowIdx >= 0)
	{
		pGrid->SetRow(nRowIdx + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellFontBold(bNewValue);
		}
	}
	pGrid->SetCol(nCol);
}

VOID CSeVolDlg::OnSelchangecmbVOLVOLUMECLASS() 
{
	// TODO: Add your control notification handler code here
	// ��ȣ���� sel change
	
	//�Ϲ� ���� ��忡���� ����
	if(-1 != m_nOpenMode) return;
	
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
	if(!pCombo) return;
	
	INT nCurSel = pCombo->GetCurSel();
	CString strCurClass;
	CString strNewClass;
	strCurClass = m_pDM->GetCellData(_T("SV_��ȣ����"),m_nCurrentParentIdx);
	

	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT);


	if(0 == nCurSel || 1 == nCurSel) //�Ϲ� , �պ�ȣ
	{
		pEdit1->SetReadOnly(FALSE);
		pEdit2->SetReadOnly(FALSE);

	}
	if(2 == nCurSel) //�̹��� (ó��)
	{
	
		CString msg;
		msg.Format(_T("������ ��ȣ�� �̹��� ó�� �Ͻðڽ��ϱ�?")); 
		
		if(AfxMessageBox(msg,MB_YESNO) == IDYES)
		{
			pCombo->SetCurSel(2);
			m_bIsNonPublished = TRUE;
			
			pEdit1->SetReadOnly(TRUE);
			pEdit2->SetReadOnly(TRUE);

		}
		else
		{
			if(_T("1") == strCurClass) pCombo->SetCurSel(0);
			else if(_T("2") == strCurClass) pCombo->SetCurSel(1);
			
			pEdit1->SetReadOnly(FALSE);
			pEdit2->SetReadOnly(FALSE);
		}

	}


}

HBRUSH CSeVolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
