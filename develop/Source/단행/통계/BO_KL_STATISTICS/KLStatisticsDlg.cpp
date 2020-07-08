// KLStatisticsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KLStatisticsDlg.h"
#include "SBL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKLStatisticsDlg dialog


CKLStatisticsDlg::CKLStatisticsDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKLStatisticsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKLStatisticsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKLStatisticsDlg::~CKLStatisticsDlg()
{
	
}

void CKLStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKLStatisticsDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKLStatisticsDlg, CDialog)
	//{{AFX_MSG_MAP(CKLStatisticsDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKLStatisticsDlg message handlers

void CKLStatisticsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(-2, 35, cx+2, cy-35);	

}

VOID CKLStatisticsDlg::SearchData( CString sWhere )
{
	EFS_BEGIN
	EFS_END
}

BOOL CKLStatisticsDlg::Create(CWnd* pParentWnd) 
{	
	return CDialog::Create(IDD, pParentWnd);
}


BOOL CKLStatisticsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;

	ids = InitESL_Mgr( _T("SM_KL_STATISTICS") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT	CKLStatisticsDlg::GetStatisticsType()
{
	CComboBox* pWnd = (CComboBox*)GetDlgItem(IDC_cmbSTATICS_CLASS);
	ASSERT(pWnd);

	INT nIdx = pWnd->GetCurSel();
	if (0 > nIdx) return KL_STAT_TYPE_NO_SUPPORT;

	CString strStatType;
	pWnd->GetLBText(nIdx, strStatType);

	if		(strStatType.CompareNoCase(_T("å����ȸ��������(����)")) == 0)
		return KL_STAT_TYPE_REGUSER_GENDER;
	else if (strStatType.CompareNoCase(_T("å����ȸ��������(���ɺ�)")) == 0)
		return KL_STAT_TYPE_REGUSER_AGE;
	else if (strStatType.CompareNoCase(_T("å����ȸ��Ż�����(����)")) == 0)
		return KL_STAT_TYPE_SECUSER_GENDER;
	else if (strStatType.CompareNoCase(_T("å����ȸ��Ż�����(���ɺ�)")) == 0)
		return KL_STAT_TYPE_SECUSER_AGE;
	else if (strStatType.CompareNoCase(_T("ù����ȸ�����(����)")) == 0)
		return KL_STAT_TYPE_FIRSTREG_GENDER;
	else if (strStatType.CompareNoCase(_T("ù����ȸ�����(���ɺ�)")) == 0)
		return KL_STAT_TYPE_FIRSTREG_AGE;
	else if (strStatType.CompareNoCase(_T("Ÿ������ȸ�����(����)")) == 0)
		return KL_STAT_TYPE_OTHERIMP_GENDER;
	else if (strStatType.CompareNoCase(_T("Ÿ������ȸ�����(���ɺ�)")) == 0)
		return KL_STAT_TYPE_OTHERIMP_AGE;
	else if (strStatType.CompareNoCase(_T("å���� Ÿ�������� �ݳ�ó�����")) == 0)
		return KL_STAT_TYPE_OTHERRETURN_OTHER;
	else if (strStatType.CompareNoCase(_T("å���� �ڰ������� Ÿ���ݳ����")) == 0)
		return KL_STAT_TYPE_OTHERRETURN_SELF;
	else if (strStatType.CompareNoCase(_T("å���� �ڰ������� Ÿ���������")) == 0)
		return KL_STAT_TYPE_OTHERLOAN_SELF;
	else if (strStatType.CompareNoCase(_T("å���� Ÿ�������� ����ó�����")) == 0)
		return KL_STAT_TYPE_OTHERLOAN_OTHER;

	return KL_STAT_TYPE_NO_SUPPORT;
}