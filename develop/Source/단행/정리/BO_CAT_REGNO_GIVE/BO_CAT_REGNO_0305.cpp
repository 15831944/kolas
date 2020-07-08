// BO_CAT_REGNO_0305.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_REGNO_0305.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REGNO_0305 dialog


CBO_CAT_REGNO_0305::CBO_CAT_REGNO_0305(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REGNO_0305)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//2019.07.25 JOB.2019.0065 ADD BY KYJ �Էµ� ���� ���� �������� �ʵ��� ����
	strAccNo = _T("");
	strAccYear = _T("");
}

CBO_CAT_REGNO_0305::~CBO_CAT_REGNO_0305()
{	
}

VOID CBO_CAT_REGNO_0305::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REGNO_0305)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REGNO_0305, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REGNO_0305)
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REGNO_0305 message handlers

BOOL CBO_CAT_REGNO_0305::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_REGNO_0305::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_0305")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_REG_0305"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG error") );
		return FALSE;
	}

	//2019.07.25 JOB.2019.0065 ADD BY KYJ �Էµ� ���� ���� �������� �ʵ��� ����
	if( !strAccNo.IsEmpty() && !strAccYear.IsEmpty() ) {
		CString strWhere = _T("");
		strWhere.Format(_T(" ACCESSION_REC_NO = '%s' AND ACCESSION_REC_MAKE_YEAR = '%s' AND ROWNUM = '1' "),strAccNo,strAccYear);
		pDM->RefreshDataManager(strWhere);
	}	

	CString sACCESSION_REC_MAKE_YEAR, sACCESSION_REC_NO, sACQUISIT_CODE, sDIVISION_NAME, sCLASS_NAME, sPROVIDER;
	CString sREGISTRANT, sREG_DATE, sCLASS_WORKER, sCLASS_DATE, sCATALOG_WORKER, sCATALOG_DATE;
	CString sLABELING_WORKER, sLABELING_DATE, sPROCESSING_WORKER, sPROCESSING_COMPLETE_DATE;
	CString sYBK_TRANSFER_DATE, sSHELF_WORKER, sSHELF_DATE;

	INT ids;
	ids = pDM->GetCellData( _T("�з��۾���"), 0, sCLASS_WORKER );
	ids = pDM->GetCellData( _T("�з�����"), 0, sCLASS_DATE );
	ids = pDM->GetCellData( _T("����۾���"), 0, sCATALOG_WORKER );
	ids = pDM->GetCellData( _T("�������"), 0, sCATALOG_DATE );
	ids = pDM->GetCellData( _T("����۾���"), 0, sLABELING_WORKER );
	ids = pDM->GetCellData( _T("�������"), 0, sLABELING_DATE );
	ids = pDM->GetCellData( _T("�����Ϸ���"), 0, sPROCESSING_WORKER );
	ids = pDM->GetCellData( _T("�����Ϸ�����"), 0, sPROCESSING_COMPLETE_DATE );
	ids = pDM->GetCellData( _T("�ΰ�����"), 0, sYBK_TRANSFER_DATE );
	ids = pDM->GetCellData( _T("�谡�۾���"), 0, sSHELF_WORKER );
	ids = pDM->GetCellData( _T("�谡����"), 0, sSHELF_DATE );
	ids = pDM->GetCellData( _T("����"), 0, sCLASS_NAME );

	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("����"), sCLASS_NAME);
	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("�з���"), sCLASS_WORKER);
	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("�з��۾�����"), sCLASS_DATE);
	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("�����"), sCATALOG_WORKER);
	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("�������"), sCATALOG_DATE);
	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("�����"), sLABELING_WORKER);
	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("�������"), sLABELING_DATE);
	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("�����Ϸ���"), sPROCESSING_WORKER);
	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("�����Ϸ�����"), sPROCESSING_COMPLETE_DATE);
	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("�ΰ�����"), sYBK_TRANSFER_DATE);
	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("�谡��"), sSHELF_WORKER);
	SetControlData(_T("CM_BO_CAT_REG_0305"), _T("�谡����"), sSHELF_DATE);



	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_REGNO_0305::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	CString sACCESSION_REC_MAKE_YEAR, sACCESSION_REC_NO, sACQUISIT_CODE, sDIVISION_NAME, sCLASS_NAME, sPROVIDER;
	CString sREGISTRANT, sREG_DATE, sCLASS_WORKER, sCLASS_DATE, sCATALOG_WORKER, sCATALOG_DATE;
	CString sLABELING_WORKER, sLABELING_DATE, sPROCESSING_WORKER, sPROCESSING_COMPLETE_DATE;
	CString sYBK_TRANSFER_DATE, sSHELF_WORKER, sSHELF_DATE;

	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("����"), sCLASS_NAME);
	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("�з���"), sCLASS_WORKER);
	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("�з��۾�����"), sCLASS_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("�����"), sCATALOG_WORKER);
	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("�������"), sCATALOG_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("�����"), sLABELING_WORKER);
	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("�������"), sLABELING_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("�����Ϸ���"), sPROCESSING_WORKER);
	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("�����Ϸ�����"), sPROCESSING_COMPLETE_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("�ΰ�����"), sYBK_TRANSFER_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("�谡��"), sSHELF_WORKER);
	GetControlData(_T("CM_BO_CAT_REG_0305"), _T("�谡����"), sSHELF_DATE);

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_REG_0305"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG error") );
		return;
	}

	INT ids;
	//2019.07.25 JOB.2019.0065 ADD BY KYJ �Էµ� ���� ���� �������� �ʵ��� ����
	//ids = pDM->InsertRow(-1);
	if(pDM->GetRowCount() == 0 ) ids = pDM->InsertRow(-1);
	ids = pDM->SetCellData( _T("�з��۾���"), sCLASS_WORKER, 0 );
	ids = pDM->SetCellData( _T("�з�����"), sCLASS_DATE, 0 );
	ids = pDM->SetCellData( _T("����۾���"), sCATALOG_WORKER, 0 );
	ids = pDM->SetCellData( _T("�������"), sCATALOG_DATE, 0 );
	ids = pDM->SetCellData( _T("����۾���"), sLABELING_WORKER, 0 );
	ids = pDM->SetCellData( _T("�������"), sLABELING_DATE, 0 );
	ids = pDM->SetCellData( _T("�����Ϸ���"), sPROCESSING_WORKER, 0 );
	ids = pDM->SetCellData( _T("�����Ϸ�����"), sPROCESSING_COMPLETE_DATE, 0 );
	ids = pDM->SetCellData( _T("�ΰ�����"), sYBK_TRANSFER_DATE, 0 );
	ids = pDM->SetCellData( _T("�谡�۾���"), sSHELF_WORKER, 0 );
	ids = pDM->SetCellData( _T("�谡����"), sSHELF_DATE, 0 );
	ids = pDM->SetCellData( _T("����"), sCLASS_NAME, 0 );

	CDialog::OnOK();

EFS_END
}


HBRUSH CBO_CAT_REGNO_0305::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

