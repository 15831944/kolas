// BO_CAT_LAST_NUMBER_0000.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_LAST_NUMBER_0000.h"
#include "BO_CAT_LAST_NUMBER_0100.h"
#include "BO_CAT_LAST_NUMBER_0200.h"

#include "BO_CAT_REG_NUMBER_0100.h"
#include "BO_CAT_REG_NUMBER_0200.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BO_CAT_LAST_NUMBER_0000 dialog


BO_CAT_LAST_NUMBER_0000::BO_CAT_LAST_NUMBER_0000(INT nType, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(BO_CAT_LAST_NUMBER_0000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// CHILD
	if (nType == CHILD)
	{
		//m_lpszTemplateName = MAKEINTRESOURCE(IDD_POPUP);
		//m_nIDHelp = IDD_POPUP;

		//m_lpszTemplateName = MAKEINTRESOURCE(IDD);
		//m_nIDHelp = IDD;
		
		//child
		m_lpszTemplateName = MAKEINTRESOURCE(IDD_BO_CAT_LAST_NUMBER_0000_CHILD);
		m_nIDHelp = IDD_BO_CAT_LAST_NUMBER_0000_CHILD;	
	}
	// POPUP
	else if (nType == POPUP)
	{
		m_lpszTemplateName = MAKEINTRESOURCE(IDD);
		m_nIDHelp = IDD;		
		
		
	}
}

BO_CAT_LAST_NUMBER_0000::~BO_CAT_LAST_NUMBER_0000()
{
}

BOOL BO_CAT_LAST_NUMBER_0000::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID BO_CAT_LAST_NUMBER_0000::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BO_CAT_LAST_NUMBER_0000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(BO_CAT_LAST_NUMBER_0000, CDialog)
	//{{AFX_MSG_MAP(BO_CAT_LAST_NUMBER_0000)
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	ON_BN_CLICKED(IDC_bINPUT, OnbINPUT)
	ON_BN_CLICKED(IDC_bUPDATE, OnbUPDATE)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_CBN_SELCHANGE(IDC_cTYPE, OnSelchangecTYPE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BO_CAT_LAST_NUMBER_0000 message handlers

BOOL BO_CAT_LAST_NUMBER_0000::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL BO_CAT_LAST_NUMBER_0000::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_LAST_NUMBER")) < 0)
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

VOID BO_CAT_LAST_NUMBER_0000::OnbFIND() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_LAST_NUMBER"), _T("��ȣ����"), sWord );
	if ( sWord.IsEmpty() ){		
		return;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_BO_CAT_LAST_NUMBER") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_LAST_NUMBER error") );
		return;
	}

	CESL_DataMgr *pDM_REG;
	pDM_REG = FindDM( _T("DM_BO_CAT_REG_NUMBER") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_REG_NUMBER error") );
		return;
	}
	
	CString strWhere;
	if ( sWord == _T("(����)��Ͽ��ι�ȣ") ) {
		//strWhere = _T("UNLESS_WORD LIKE '") + sWord + _T("%'");
		strWhere.Format( _T("KIND_CODE = '%s'"), _T("ACC_NO") );
		//GetDlgItem(IDC_GRD_LAST_NUMBER)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_GRD_LAST_NUMBER2)->ShowWindow(SW_HIDE);
		if (pDM->RefreshDataManager(strWhere) < 0)	return;
		if ( pDM->GetRowCount() < 1 ) {
			AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
		}
		pDM->SORT( _T("�⵵"), FALSE );
	}
	else if ( sWord == _T("(����)��Ϲ�ȣ") ) {
		strWhere.Format( _T("KIND_CODE = '%s'"), _T("REG_NO") );
		//GetDlgItem(IDC_GRD_LAST_NUMBER)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_GRD_LAST_NUMBER2)->ShowWindow(SW_SHOW);
		if (pDM_REG->RefreshDataManager(strWhere) < 0)	return;
		if ( pDM_REG->GetRowCount() < 1 ) {
			AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
		}
		pDM_REG->SORT( _T("���а�"), FALSE );
	}
	

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_LAST_NUMBER"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_LAST_NUMBER error") );
		return;
	}
	pCM->AllControlDisplay();	

EFS_END
}

VOID BO_CAT_LAST_NUMBER_0000::OnbINPUT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_LAST_NUMBER"), _T("��ȣ����"), sWord );
	if ( sWord.IsEmpty() ){		
		return;
	}

	if ( sWord == _T("(����)��Ͽ��ι�ȣ") ) {
		CBO_CAT_LAST_NUMBER_0100 dlg(this);
		dlg.DoModal();
	}
	else {
		CBO_CAT_REG_NUMBER_0100 dlg(this);
		dlg.DoModal();
	}
	OnbFIND();

EFS_END
}

VOID BO_CAT_LAST_NUMBER_0000::OnbUPDATE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_LAST_NUMBER"), _T("��ȣ����"), sWord );
	if ( sWord.IsEmpty() ){		
		return;
	}

	if ( sWord == _T("(����)��Ͽ��ι�ȣ") ) {
		CBO_CAT_LAST_NUMBER_0200 dlg(this);
		dlg.DoModal();
	}
	else {
		CBO_CAT_REG_NUMBER_0200 dlg(this);
		dlg.DoModal();
	}
	OnbFIND();

EFS_END
}

VOID BO_CAT_LAST_NUMBER_0000::OnbDelete() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_ControlMgr *pCM;
	INT ids;
	pCM = FindCM( _T("CM_BO_CAT_LAST_NUMBER") );
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_LAST_NUMBER ERROR") );
		return;
	}

	CString sWord;
	GetControlData( _T("CM_BO_CAT_LAST_NUMBER"), _T("��ȣ����"), sWord );
	if ( sWord.IsEmpty() ){		
		return;
	}
	if ( sWord == _T("(����)��Ͽ��ι�ȣ") ) {	
		SelectMakeList(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));
		INT idx = SelectGetTailPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));
		if (idx < 0)
		{
			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
			return;
		}

		if (MessageBox(_T("������ �ڷḦ �����Ͻðڽ��ϱ�?"), _T("���� Ȯ��"), MB_YESNO) == IDYES)
		{
			CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_LAST_NUMBER"));
			ids = pDM->StartFrame();
			if ( ids < 0 ) 
			{
				AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
				return;
			}


			CString sAccRecKey, sAccRecKeyTmp;
			do
			{
				CString strCondition;
				pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

				sAccRecKeyTmp += _T(",") + strCondition;		//Ű ����

				pDM->MakeDeleteFrame(_T("CO_LAST_NUMBER_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
				pDM->DeleteRow(idx);

				idx = SelectGetPrev(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));			
			}		
			while (idx > 0);
/*
			sAccRecKey = sAccRecKeyTmp.Mid(1);
			CString sQuery;
			sQuery.Format( _T("DELETE FROM CO_LAST_NUMBER_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
			INT iCh = pDM->AddFrame(sQuery);
*/
			ids = pDM->SendFrame();
			if (ids < 0) {
				AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
				return;
			}
			ids = pDM->EndFrame();
			if (ids < 0) {
				AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
				return;
			}
		}
	}
	else {
		SelectMakeList(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		INT idx = SelectGetTailPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		if (idx < 0)
		{
			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
			return;
		}

		if (MessageBox(_T("������ �ڷḦ �����Ͻðڽ��ϱ�?"), _T("���� Ȯ��"), MB_YESNO) == IDYES)
		{
			CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_NUMBER"));
			ids = pDM->StartFrame();
			if ( ids < 0 ) 
			{
				AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
				return;
			}


			CString sAccRecKey, sAccRecKeyTmp;
			do
			{
				CString strCondition;
				pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

				sAccRecKeyTmp += _T(",") + strCondition;		//Ű ����

				pDM->MakeDeleteFrame(_T("CO_LAST_NUMBER_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
				pDM->DeleteRow(idx);

				idx = SelectGetPrev(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));			
			}		
			while (idx > 0);

			sAccRecKey = sAccRecKeyTmp.Mid(1);
			CString sQuery;
			sQuery.Format( _T("DELETE FROM CO_LAST_NUMBER_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
			INT iCh = pDM->AddFrame(sQuery);

			ids = pDM->SendFrame();
			if (ids < 0) {
				AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
				return;
			}
			ids = pDM->EndFrame();
			if (ids < 0) {
				AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
				return;
			}

		}
	}

	AllControlDisplay(_T("CM_BO_CAT_LAST_NUMBER"));

	return;

EFS_END
}

VOID BO_CAT_LAST_NUMBER_0000::OnSelchangecTYPE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_LAST_NUMBER"), _T("��ȣ����"), sWord );

	if ( sWord == _T("(����)��Ͽ��ι�ȣ") ) {
		GetDlgItem(IDC_GRD_LAST_NUMBER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_GRD_LAST_NUMBER2)->ShowWindow(SW_HIDE);
	}
	else if ( sWord == _T("(����)��Ϲ�ȣ") ) {
		GetDlgItem(IDC_GRD_LAST_NUMBER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GRD_LAST_NUMBER2)->ShowWindow(SW_SHOW);
	}
	//�ٽ� �˻�
	OnbFIND();

EFS_END
}

HBRUSH BO_CAT_LAST_NUMBER_0000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
