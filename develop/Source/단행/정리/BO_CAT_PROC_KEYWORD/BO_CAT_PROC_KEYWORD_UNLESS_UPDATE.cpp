// BO_CAT_PROC_KEYWORD_UNLESS_UPDATE.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_PROC_KEYWORD_UNLESS_UPDATE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE dialog


CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE::CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE::~CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE()
{
}

VOID CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE message handlers

BOOL CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_UNLESS_WORD_0300")) < 0)	
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	Display(0);		

	//CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_UNLESS_WORD_0000"));
	if ( pDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_UNLESS_WORD_0000 ERROR") );
		m_bInitDlg = FALSE;
		return FALSE;
	}
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

	CString sWord;
	sWord = pDM->GetCellData( _T("�ҿ��"), m_idx );
	SetControlData( _T("CM_BO_CAT_UNLESS_WORD_0300"), _T("�ҿ��"), sWord );

	if ( sRecKey.IsEmpty() ) {
		m_bInitDlg = FALSE;
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(_T("CM_BO_CAT_PROC_KEYWORD_UNLESS"), _T("GRD_UNLESS"));
		idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_KEYWORD_UNLESS"), _T("GRD_UNLESS"));
		if (idx < 0)
		{
			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		idx = SelectGetPrev(_T("CM_BO_CAT_PROC_KEYWORD_UNLESS"), _T("GRD_UNLESS"));
		if (idx == -1) {
			idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_KEYWORD_UNLESS"), _T("GRD_UNLESS"));
			AfxMessageBox(_T("ó�� �ڷ��Դϴ�."));
			return;
		}
		break;
	case 1:
		idx = SelectGetNext(_T("CM_BO_CAT_PROC_KEYWORD_UNLESS"), _T("GRD_UNLESS"));
		if (idx == -1) {
			idx = SelectGetTailPosition(_T("CM_BO_CAT_PROC_KEYWORD_UNLESS"), _T("GRD_UNLESS"));
			AfxMessageBox(_T("������ �ڷ��Դϴ�."));
			return;
		}
		break;
	}
	
	m_idx = idx; 
	AllControlDisplay(_T("CM_BO_CAT_PROC_KEYWORD_UNLESS"), m_idx);
	SetReverse(_T("CM_BO_CAT_PROC_KEYWORD_UNLESS"), _T("GRD_UNLESS"), m_idx);

EFS_END
}

VOID CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE::OnbPREV() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(-1);		
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

	CString sWord;
	sWord = pDM->GetCellData( _T("�ҿ��"), m_idx );
	SetControlData( _T("CM_BO_CAT_UNLESS_WORD_0300"), _T("�ҿ��"), sWord );

EFS_END
}

VOID CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE::OnbNEXT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(1);
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

	CString sWord;
	sWord = pDM->GetCellData( _T("�ҿ��"), m_idx );
	SetControlData( _T("CM_BO_CAT_UNLESS_WORD_0300"), _T("�ҿ��"), sWord );

EFS_END
}

VOID CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	INT ids;
    CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_UNLESS_WORD_0000"));	
	if (!pDM) return;
	
	//�ҿ�� �ߺ��˻�
	CString sWord;
	GetControlData( _T("CM_BO_CAT_UNLESS_WORD_0300"), _T("�ҿ��"), sWord );
	INT iRowCount = pDM->GetRowCount();		
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sDmWord = pDM->GetCellData( _T("�ҿ��"), i );
		if ( sDmWord == sWord ) {
			AfxMessageBox( _T("�̹� �Է��� �ҿ���Դϴ�.") );
			return;
		}
	}

	if ( sWord == _T("") ) {
		AfxMessageBox( _T("�ҿ� �Է��ϼ���.") );
		return;
	}

	///���� �۾� ���� 
	CString strID;
	//pCESLMgr->GetControlData(_T("CM_BO_CAT_REG_0520"), _T("����۾���"), strID);
	strID = pDM->GetCellData ( m_idx , 0 );

	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return;
	}

	
	///���� �۾� ����  
	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_UNLESS_WORD_0300"));
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
  
	/*
	CBO_CAT_REG_0520 * pREG_0520;
	INT tmp = pREG_0520->m
	*/
	
	//pCESLMgr->AllControlDisplay(_T("DM_BO_CAT_REG_0500"));
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_PROC_KEYWORD_UNLESS"));
	AllControlDisplay(_T("DM_BO_CAT_UNLESS_WORD_0000"));	
	pCM1->AllControlDisplay(m_idx);
 
	//DestroyWindow();
	CDialog::OnOK();

EFS_END
}

HBRUSH CBO_CAT_PROC_KEYWORD_UNLESS_UPDATE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}