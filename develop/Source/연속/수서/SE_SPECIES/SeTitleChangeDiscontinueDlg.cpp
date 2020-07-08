// SeTitleChangeDiscontinueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeTitleChangeDiscontinueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeTitleChangeDiscontinueDlg dialog


CSeTitleChangeDiscontinueDlg::CSeTitleChangeDiscontinueDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeTitleChangeDiscontinueDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeTitleChangeDiscontinueDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pCM = NULL;
	m_OldMarc.Init();
	m_pNewMarc = NULL;
	m_strOldTitle = _T("");
	m_strNewTitle = _T("");
	m_strOldISSN1 = _T("");
	m_strOldISSN2 = _T("");
	m_strNewISSN1 = _T("");
	m_strNewISSN2 = _T("");
	m_strNewFirstVol = _T("");

	m_strSPECIES_KEY = _T("");
	m_nACQ_TYPE = 1;
	
    m_strOldSecondIndicator = _T("");
	m_strNewSecondIndicator = _T("");

	m_strDiscontinueYear = _T("");
	m_strDiscontinueReason = _T("");
}

CSeTitleChangeDiscontinueDlg::~CSeTitleChangeDiscontinueDlg()
{
}

VOID CSeTitleChangeDiscontinueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeTitleChangeDiscontinueDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeTitleChangeDiscontinueDlg, CDialog)
	//{{AFX_MSG_MAP(CSeTitleChangeDiscontinueDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeTitleChangeDiscontinueDlg message handlers
BOOL CSeTitleChangeDiscontinueDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

BOOL CSeTitleChangeDiscontinueDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(InitESL_Mgr(_T("����_����_��_������_��"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_��_������_��") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_����_��_������_��"));

	if(!m_pDM || !m_pCM ) {
		AfxMessageBox( _T("DM,CM Error : ����_����_��_������_��") );
		return FALSE;
	}	

	InitControls();


	//���� ���� �����ֱ� 
	CString strQuery;
	CString strResult;
	CString strFIRST_VOL;
	CString strLAST_VOL;
	CString strSPECIES_KEY;
	strSPECIES_KEY = m_pDM->GetCellData(_T("SS_��KEY"),0);
	m_strSPECIES_KEY = strSPECIES_KEY; //update�� ���� ��KEY�� ��������� ������ �ִ´� 

	strQuery.Format(_T("SELECT VOL_TITLE FROM SE_VOL_TBL WHERE SPECIES_KEY = %s AND VOL_SORT_NO = (SELECT MIN(VOL_SORT_NO) FROM SE_VOL_TBL WHERE SPECIES_KEY = %s)"),strSPECIES_KEY,strSPECIES_KEY);
	m_pDM->GetOneValueQuery(strQuery,strFIRST_VOL);
	if(!strFIRST_VOL.IsEmpty()) m_pCM->SetControlMgrData(_T("�������Լ���ȣ"),strFIRST_VOL);
	

	strQuery.Format(_T("SELECT VOL_TITLE FROM SE_VOL_TBL WHERE SPECIES_KEY = %s AND VOL_SORT_NO = (SELECT MAX(VOL_SORT_NO) FROM SE_VOL_TBL WHERE SPECIES_KEY = %s)"),strSPECIES_KEY,strSPECIES_KEY);
	m_pDM->GetOneValueQuery(strQuery,strLAST_VOL);
	if(!strLAST_VOL.IsEmpty()) m_pCM->SetControlMgrData(_T("�������Լ���ȣ"),strLAST_VOL);
	

	m_pCM->SetControlMgrData(_T("������"),m_strOldTitle);
	m_pCM->SetControlMgrData(_T("��ISSN1"),m_strOldISSN1);	
	m_pCM->SetControlMgrData(_T("��ISSN2"),m_strOldISSN2);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSeTitleChangeDiscontinueDlg::InitControls()
{
	//�� 2 ���ñ�ȣ ���� 
	CComboBox * pCombo = NULL;
	

	//780 tag
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbDISCONTINUE_OLD_INDICATOR );
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("0 - ...��(��) ����") );
		pCombo->InsertString( 1, _T("1 - ...��(��) �Ϻ� ����") ); 
		pCombo->InsertString( 2, _T("4 - ...�� ...��(��) �պ�") );
		pCombo->InsertString( 3, _T("5 - ...��(��) ���") );
		pCombo->InsertString( 4, _T("6 - ...��(��) �Ϻ� ���") );
		pCombo->InsertString( 5, _T("7 - ...���κ��� �и�") );
		pCombo->SetCurSel( 0 );
	}

	
	//785 tag
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbDISCONTINUE_NEW_INDICATOR );
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("0 - ...����(��) ����") );
		pCombo->InsertString( 1, _T("1 - ...�� �� �κ����� ����") ); 
		pCombo->InsertString( 2, _T("4 - ...����(��) ���") );
		pCombo->InsertString( 3, _T("5 - ...���� �� �κ����� ���") );
		pCombo->InsertString( 4, _T("6 - ...��(��) ...����(��) �и�") );
		pCombo->InsertString( 5, _T("7 - ...�� �պ�") );
		pCombo->InsertString( 6, _T("8 - ...����(��) �ұ޺���") );
		pCombo->SetCurSel( 0 );
	}


}

VOID CSeTitleChangeDiscontinueDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	m_pCM->GetControlMgrData(_T("������"),m_strNewTitle);

	if(m_strNewTitle.IsEmpty())
	{
		AfxMessageBox(_T("�������� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}

	if(m_strOldTitle == m_strNewTitle) 
	{
		AfxMessageBox(_T("�������� ������� �����մϴ�!"));
		return;
	}
	
	
	CString strFIRST_VOL,strLAST_VOL,strTmp;

	//��ũ �׷� ����
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

	m_pCM->GetControlMgrData(_T("��ISSN1"),m_strNewISSN1);	
	m_pCM->GetControlMgrData(_T("��ISSN2"),m_strNewISSN2);
	m_pCM->GetControlMgrData(_T("�󰣳⵵"),m_strDiscontinueYear);
	m_pCM->GetControlMgrData(_T("�󰣻���"),m_strDiscontinueReason);
	m_pCM->GetControlMgrData(_T("�������Լ���ȣ"),m_strNewFirstVol);
	m_pCM->GetControlMgrData(_T("�������Լ���ȣ"),strFIRST_VOL);
	m_pCM->GetControlMgrData(_T("�������Լ���ȣ"),strLAST_VOL);
	
	
	//�������� ó��(780)
	//���� ��ȣ
	CComboBox * pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbDISCONTINUE_OLD_INDICATOR );
	INT nCurSel = 0;
	if(pCombo) nCurSel = pCombo->GetCurSel();
	
	CString strIndicator = _T("");
	CString strMarcTag = _T("");

	switch(nCurSel)
	{
	case 0:
		strIndicator = _T("00");
		break;
	case 1:
		strIndicator = _T("01");
		break;
	case 2:
		strIndicator = _T("04");
		break;
	case 3:
		strIndicator = _T("05");
		break;
	case 4:
		strIndicator = _T("06");
		break;
	case 5:
		strIndicator = _T("07");
		break;
	}
	
	//������ 780tag�� �����.
	m_pInfo->pMarcMgr->DeleteField(m_pNewMarc,_T("780"));


	//���� 
	strMarcTag = _T("780$t!") + strIndicator;
	m_pInfo->pMarcMgr->SetItem(m_pNewMarc,strMarcTag,m_strOldTitle,_T(""),ADD_SUBFIELD);
	

	//����
	if(!strFIRST_VOL.IsEmpty() || !strLAST_VOL.IsEmpty())
	{
		strTmp = _T("");
		strTmp = strFIRST_VOL + _T("-") + strLAST_VOL;
		strMarcTag = _T("780$g!") + strIndicator;
		m_pInfo->pMarcMgr->SetItem(m_pNewMarc,strMarcTag,strTmp,_T(""),ADD_SUBFIELD);
	}
	
	//ISSN
	if(!m_strOldISSN1.IsEmpty())
	{
		CString strISSN;
		strISSN = m_strOldISSN1 + _T("-") + m_strOldISSN2;
		strMarcTag = _T("780$x!") + strIndicator;
		m_pInfo->pMarcMgr->SetItem(m_pNewMarc,strMarcTag,strISSN,_T(""),ADD_SUBFIELD);
	}


	//�ļ����� ó��(785) �� ��ó�� 
	//���� ��ȣ
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbDISCONTINUE_NEW_INDICATOR );
	nCurSel = 0;
	if(pCombo) nCurSel = pCombo->GetCurSel();
	
	switch(nCurSel)
	{
	case 0:
		strIndicator = _T("00");
		break;
	case 1:
		strIndicator = _T("01");
		break;
	case 2:
		strIndicator = _T("04");
		break;
	case 3:
		strIndicator = _T("05");
		break;
	case 4:
		strIndicator = _T("06");
		break;
	case 5:
		strIndicator = _T("07");
		break;
	case 6:
		strIndicator = _T("08");
		break;
	}


	//���� 
	strMarcTag = _T("785$t!") + strIndicator;
	m_pInfo->pMarcMgr->SetItem(&m_OldMarc,strMarcTag,m_strNewTitle,_T(""),ADD_SUBFIELD);


	//����
	if(!m_strNewFirstVol.IsEmpty())
	{
		strTmp = _T("");
		strTmp = m_strNewFirstVol + _T("-"); 
		strMarcTag = _T("785$g!") + strIndicator;
		m_pInfo->pMarcMgr->SetItem(&m_OldMarc,strMarcTag,strTmp,_T(""),ADD_SUBFIELD);
	}
	
	//ISSN
	if(!m_strNewISSN1.IsEmpty())
	{
		CString strISSN;
		strISSN = m_strNewISSN1 + _T("-") + m_strNewISSN2;
		strMarcTag = _T("785$x!") + strIndicator;		
		m_pInfo->pMarcMgr->SetItem(&m_OldMarc,strMarcTag,strISSN,_T(""),ADD_SUBFIELD);
	}
	
	//008TAGó�� 
	//�������
	m_pInfo->pMarcMgr->SetItem(&m_OldMarc,_T("008@6"),_T("d")); 
	
	//�󰣳� 
	if(!m_strDiscontinueYear.IsEmpty())
	{
		m_pInfo->pMarcMgr->SetItem(&m_OldMarc,_T("008@11-14"),m_strDiscontinueYear);
	}
	
	CDialog::OnOK();
}

HBRUSH CSeTitleChangeDiscontinueDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
