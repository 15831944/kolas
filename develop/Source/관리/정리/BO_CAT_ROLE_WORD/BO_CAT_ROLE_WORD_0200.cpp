// BO_CAT_ROLE_WORD_0200.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_ROLE_WORD_0200.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_ROLE_WORD_0200 dialog


CBO_CAT_ROLE_WORD_0200::CBO_CAT_ROLE_WORD_0200(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_ROLE_WORD_0200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_ROLE_WORD_0200::~CBO_CAT_ROLE_WORD_0200()
{
}

VOID CBO_CAT_ROLE_WORD_0200::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_ROLE_WORD_0200)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_ROLE_WORD_0200, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_ROLE_WORD_0200)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_ROLE_WORD_0200 message handlers

BOOL CBO_CAT_ROLE_WORD_0200::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}
/*
VOID CBO_CAT_ROLE_WORD_0200::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

//	CButton *pButton = (CButton*)GetDlgItem(IDC_bDELETE);
//	if (pButton->GetSafeHwnd() == NULL) return;
	
//	pButton->SetWindowPos(NULL, cx/2, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	pGrid->MoveWindow(0, 0, cx, cy);

EFS_END
}
	
*/
VOID CBO_CAT_ROLE_WORD_0200::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_MNG_ROLE_0200"), _T("���Ҿ�"), sWord );
	if ( sWord.IsEmpty() ) {
		AfxMessageBox( _T("���Ҿ �Է��ϼ���.") );
		return;
	}
	
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_MNG_ROLE"));
	if (!pDM) return;

	INT m_idx;
	m_idx = pDM->GetRowCount();

	//���Ҿ� �ߺ�üũ
	for ( INT i = 0 ; i < m_idx ; i++ ) {
		CString sDmWord = pDM->GetCellData( _T("���Ҿ�"), i );
		if ( sDmWord == sWord ) {
			AfxMessageBox( _T("�̹� �Է��� ���Ҿ��Դϴ�.") );
			return;
		}
	}

	//rec_key �����
	CString sRecKey;
	//pDM->MakeRecKey( sRecKey, table_name=_T(""), _T("CO_ACCESSION_REC_TBL"), _T("REC_KEY"));
	pDM->MakeRecKey( sRecKey );
	pDM->StartFrame();

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKey);
	pDM->AddDBFieldData(_T("ROLE_WORD"), _T("STRING"), sWord );

	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("���Ҿ����"), __WFILE__, __LINE__ ), TRUE);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("���Ҿ����"), __WFILE__, __LINE__ ), TRUE);

	pDM->MakeInsertFrame(_T("CO_ROLE_WORD_TBL"));

	INT ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return;
	}
	pDM->EndFrame();

	/*
	//���Ͽ� �ݿ�
	//CString strWhere = _T("");
	//if (pDM->RefreshDataManager(strWhere) < 0)	return;

	//����̺� ���ؿ���
	INT curdrive;	
	curdrive = _getdrive();
	CString sPath;
	if(curdrive==3){
		sPath=_T("C:\\KOLAS2UP\\CFG\\008TAG�ڵ�.txt");
	}
	if(curdrive==4){
		sPath=_T("D:\\KOLAS2UP\\CFG\\008TAG�ڵ�.txt");
	}
	if(curdrive==5){
		sPath=_T("E:\\KOLAS2UP\\CFG\\008TAG�ڵ�.txt");
	}

	//�����о����
	FILE *fp;
	TCHAR cLine[256];
	fp = _tfopen( sPath , _T("a+") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("008TAG�ڵ� ������ �����ϴ�.") );
		return TRUE;
	}	
	*/

	AfxMessageBox(_T("����Ϸ�"));

	pDM->InsertRow(-1);
	pDM->SetCellData( _T("���Ҿ�"), sWord, m_idx );

	/*
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_UNLESS_WORD_0000"));
	AllControlDisplay(_T("DM_BO_CAT_UNLESS_WORD_0000"));	
	pCM1->AllControlDisplay(m_idx);
	*/
	/*
	CString sTmp = pDM->GetCellData( _T("�ܾ�"), m_idx );
	sTmp = pDM->GetCellData( _T("����"), m_idx+1 );
	pDM->SetCellData( _T("����"), sWord, m_idx+1 );
	*/

	//AllControlDisplay(_T("CM_BO_CAT_AUXILIARY"));

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_MNG_ROLE"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_MNG_ROLE error") );
		return;
	}
	pCM->AllControlDisplay();



	//���Ͽ� ����
	//CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_BO_CAT_MNG_ROLE_FILE") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_MNG_ROLE error") );
		return;
	}
	//strWhere = _T("ROLE_WORD LIKE '") + sWord + _T("%'");
	CString strWhere = _T("");
	if (pDM->RefreshDataManager(strWhere) < 0)	return;

	//����̺� ���ؿ���
	/*
	INT curdrive;	
	curdrive = _getdrive();
	CString sPath;
	if(curdrive==3){
		sPath=_T("C:\\KOLAS2UP\\CFG\\CO_ROLE_WORD_TBL.txt");
	}
	if(curdrive==4){
		sPath=_T("D:\\KOLAS2UP\\CFG\\CO_ROLE_WORD_TBL.txt");
	}
	if(curdrive==5){
		sPath=_T("E:\\KOLAS2UP\\CFG\\CO_ROLE_WORD_TBL.txt");
	}
*/
	CString sPath = _T("..\\CFG\\CO_ROLE_WORD_TBL.txt");
	//�����о����
	FILE *fp;
	//TCHAR cLine[256];
	fp = _tfopen( sPath , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CO_ROLE_WORD_TBL.txt ������ �����ϴ�.") );
		return;
	}		
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}

	INT iRowCount = pDM->GetRowCount();
	_ftprintf( fp, _T(".FILE_VERSION = 1 \r\n") );
	for ( i = 0 ; i < iRowCount ; i++ ) {
		CString sRoleWord = pDM->GetCellData( _T("���Ҿ�"), i );
		CString sLine = sRoleWord + _T("                          |Y\r\n");
		_ftprintf( fp, sLine );
	}
	fclose( fp );

	//DestroyWindow();
	CDialog::OnOK();

EFS_END
}

BOOL CBO_CAT_ROLE_WORD_0200::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_MNG_ROLE_0200")) < 0)
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

HBRUSH CBO_CAT_ROLE_WORD_0200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
