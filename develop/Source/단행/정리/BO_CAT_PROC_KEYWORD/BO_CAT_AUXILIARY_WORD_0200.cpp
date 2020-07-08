// BO_CAT_AUXILIARY_WORD_0200.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_AUXILIARY_WORD_0200.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_AUXILIARY_WORD_0200 dialog


CBO_CAT_AUXILIARY_WORD_0200::CBO_CAT_AUXILIARY_WORD_0200(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_AUXILIARY_WORD_0200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_AUXILIARY_WORD_0200::~CBO_CAT_AUXILIARY_WORD_0200()
{
}

VOID CBO_CAT_AUXILIARY_WORD_0200::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_AUXILIARY_WORD_0200)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_AUXILIARY_WORD_0200, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_AUXILIARY_WORD_0200)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_AUXILIARY_WORD_0200 message handlers

BOOL CBO_CAT_AUXILIARY_WORD_0200::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_AUXILIARY_WORD_0200::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_AUXILIARY_0200")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_AUXILIARY_WORD_0200::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_AUXILIARY_0200"), _T("����"), sWord );
	if ( sWord.IsEmpty() ) {
		AfxMessageBox( _T("���縦 �Է��ϼ���.") );
		return;
	}
	
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_AUXILIARY"));
	if (!pDM) return;

	INT m_idx;
	m_idx = pDM->GetRowCount();

	//���� �ߺ�üũ
	for ( INT i = 0 ; i < m_idx ; i++ ) {
		CString sDmWord = pDM->GetCellData( _T("����"), i );
		if ( sDmWord == sWord ) {
			AfxMessageBox( _T("�̹� �Է��� �����Դϴ�.") );
			return;
		}
	}

	//rec_key �����
	CString sRecKey;
	pDM->MakeRecKey( sRecKey );
	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return;
	}

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKey);
	pDM->AddDBFieldData(_T("AUX_WORD"), _T("STRING"), sWord );

	ids = pDM->MakeInsertFrame(_T("CO_AUXILIARY_WORD_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame �� Ȯ���ϼ���"));
		return;
	}

	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return;
	}
	ids = pDM->EndFrame();	
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame �� Ȯ���ϼ���"));
		return;
	}

	AfxMessageBox(_T("����Ϸ�"));

	pDM->InsertRow(-1);
	pDM->SetCellData( _T("����"), sWord, m_idx );

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_AUXILIARY_WORD_0100"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_AUXILIARY_WORD_0100 error") );
		return;
	}
	pCM->AllControlDisplay();

	CDialog::OnOK();

EFS_END
}

HBRUSH CBO_CAT_AUXILIARY_WORD_0200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}