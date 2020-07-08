// BO_CAT_REG_NUMBER_0200.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_REG_NUMBER_0200.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_NUMBER_0200 dialog


CBO_CAT_REG_NUMBER_0200::CBO_CAT_REG_NUMBER_0200(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_NUMBER_0200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_REG_NUMBER_0200::~CBO_CAT_REG_NUMBER_0200()
{
}

VOID CBO_CAT_REG_NUMBER_0200::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_NUMBER_0200)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_NUMBER_0200, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_NUMBER_0200)
	ON_BN_CLICKED(IDC_bIDOK, OnbIDOK)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_NUMBER_0200 message handlers

BOOL CBO_CAT_REG_NUMBER_0200::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_NUMBER_0200")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	Display(0);
	if ( m_idx < 0 ) {
		EndDialog(IDCANCEL);
		m_bInitDlg = FALSE;		
		return FALSE;
	}

	CESL_DataMgr *pDM_REG;
	pDM_REG = FindDM(_T("DM_BO_CAT_REG_NUMBER"));
	if ( pDM_REG == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG_NUMBER ERROR") );
		m_bInitDlg = FALSE;
		return FALSE;
	}
	sRecKey = pDM_REG->GetCellData( _T("REC_KEY") , m_idx );

	CString sWord, sHanjaWord;
	sWord = pDM_REG->GetCellData( _T("���а�"), m_idx );
	sHanjaWord = pDM_REG->GetCellData( _T("��ȣ"), m_idx );
	SetControlData( _T("CM_BO_CAT_REG_NUMBER_0200"), _T("���а�"), sWord );
	SetControlData( _T("CM_BO_CAT_REG_NUMBER_0200"), _T("��ȣ"), sHanjaWord );

	if ( sRecKey.IsEmpty() ) {
		AfxMessageBox( _T("REC_KEY�� �����ϴ�.") );
		EndDialog(IDCANCEL);
		m_bInitDlg = FALSE;		
		return FALSE;
	}

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ����ڵ带 6�ڸ� ���Ϸ� ����
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_eRegNumberCode);
	pEdit->SetLimitText(6);
	//=====================================================

// 	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239));

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_NUMBER_0200::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		idx = SelectGetHeadPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		if (idx < 0)
		{
			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
			//EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		idx = SelectGetPrev(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		if (idx == -1) {
			idx = SelectGetHeadPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
			AfxMessageBox(_T("ó�� �ڷ��Դϴ�."));
			return;
		}
		break;
	case 1:
		idx = SelectGetNext(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		if (idx == -1) {
			idx = SelectGetTailPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
			AfxMessageBox(_T("������ �ڷ��Դϴ�."));
			return;
		}
		break;
	}
	
	m_idx = idx; 
	//AllControlDisplay(_T("CM_BO_CAT_LAST_NUMBER"), m_idx);
	SetReverse(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"), m_idx);

EFS_END
}

VOID CBO_CAT_REG_NUMBER_0200::OnbIDOK() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	INT ids;
    CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_NUMBER"));	
	if (!pDM) return;
		
	///���� �۾� ���� 
	CString strID;
	//pCESLMgr->GetControlData(_T("CM_BO_CAT_REG_0520"), _T("����۾���"), strID);
	strID = pDM->GetCellData ( _T("REC_KEY") ,m_idx );

	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return;
	}

	
	///���� �۾� ����  
	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_REG_NUMBER_0200"));
	ids = pDM->MakeControlMgrFiledData((CObject*)pCM);
	ids = pDM->MakeControlMgrUpdateFrame(_T("REC_KEY"), _T("STRING"), strID, m_idx);
	//ids = pDM->MakeControlMgrUpdateFrame(_T("REGISTRANT"), _T("STRING"), strID, idx);
	//ids = pDM->MakeControlMgrUpdateFrame(_T("����۾���"), _T("STRING"), strID);
   	///���� �۾� �Ϸ� 


	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return;
	}
	else {
		AfxMessageBox(_T("�����Ͽ����ϴ�."));
	}
  
	/*
	CBO_CAT_REG_0520 * pREG_0520;
	INT tmp = pREG_0520->m
	*/
	
	//pCESLMgr->AllControlDisplay(_T("DM_BO_CAT_REG_0500"));
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_LAST_NUMBER"));
	AllControlDisplay(_T("DM_BO_CAT_REG_NUMBER"));	
	pCM1->AllControlDisplay(m_idx);

EFS_END
}

HBRUSH CBO_CAT_REG_NUMBER_0200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
