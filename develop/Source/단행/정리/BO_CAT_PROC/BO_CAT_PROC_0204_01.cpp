// BO_CAT_PROC_0204_01.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0204_01.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0204_01 dialog


CBO_CAT_PROC_0204_01::CBO_CAT_PROC_0204_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0204_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}

CBO_CAT_PROC_0204_01::~CBO_CAT_PROC_0204_01()
{
}

VOID CBO_CAT_PROC_0204_01::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0204_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0204_01, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0204_01)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_b0204_01CANCEL, Onb020401CANCEL)
	ON_BN_CLICKED(IDC_b0204_01PRINT, Onb020401PRINT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0204_01 message handlers

BOOL CBO_CAT_PROC_0204_01::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton * pPrint01;
	pPrint01=(CButton *)GetDlgItem(IDC_r0204_01PRINT1);
	pPrint01->SetCheck(1);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0204_01::Onb020401CANCEL() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	EndDialog(IDCANCEL);

EFS_END
}

VOID CBO_CAT_PROC_0204_01::Onb020401PRINT() 
{
EFS_BEGIN

	CESL_DataMgr *m_pDM;
	CString strRptAlias;
	CString sClassType;

	CButton *pLINE_PRINT;
	pLINE_PRINT = (CButton *)GetDlgItem(IDC_r0204_01PRINT1);

	m_pParent->GetControlData( _T("CM_BO_CAT_PROC_0204"), _T("�з���ȣ����"), sClassType );
	if ( sClassType == _T("7") )
	{
		m_pDM = FindDM(_T("DM_BO_CAT_PROC_0204_RESULT_LC"));
		if ( pLINE_PRINT->GetCheck() == 1 )
		{
			strRptAlias = _T("����_�ڷ�з����ǥ_LC");
		}
		else
		{
			strRptAlias = _T("����_�ڷ������μ��ΰ��_LC");			
		}
	}
	else
	{
		m_pDM = FindDM(_T("DM_BO_CAT_PROC_0204_RESULT"));
		if ( pLINE_PRINT->GetCheck() == 1 )
		{
			strRptAlias = _T("����_�ڷ�з����ǥ");
		}
		else
		{
			strRptAlias = _T("����_�ڷ������μ��ΰ��");			
		}
	}	
	if ( m_pDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0204 error") );
		return;
	}

	INT iRowCount = m_pDM->GetRowCount();
	CString sTmp = m_pDM->GetCellData( _T("��Ϲ�ȣ") ,0 );

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	CString strSearchClass;
	GetControlData(_T("CM_BO_CAT_PROC_0204"), _T("�˻�����"), strSearchClass);
	if (strSearchClass == _T("��ġ��ȣ"))
		strSearchClass = _T("_��ġ��ȣ");
	else
		strSearchClass.Empty();	
	
	ids = pSIReportManager->GetSIReportFileFromDB( strRptAlias+ strSearchClass);	
	
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return;
	}

	pSIReportManager->SetDataMgr(0, m_pDM);
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

EFS_END
}

INT CBO_CAT_PROC_0204_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

EFS_END
return -1;

}
HBRUSH CBO_CAT_PROC_0204_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
