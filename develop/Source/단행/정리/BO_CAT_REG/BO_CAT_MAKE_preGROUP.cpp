// BO_CAT_MAKE_preGROUP.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_MAKE_preGROUP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_MAKE_preGROUP dialog


CBO_CAT_MAKE_preGROUP::CBO_CAT_MAKE_preGROUP(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_MAKE_preGROUP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_MAKE_preGROUP::~CBO_CAT_MAKE_preGROUP()
{
	
}

BOOL CBO_CAT_MAKE_preGROUP::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}

VOID CBO_CAT_MAKE_preGROUP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_MAKE_preGROUP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_MAKE_preGROUP, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_MAKE_preGROUP)
	ON_BN_CLICKED(IDC_CREAT_preGROUPNO, OnCREATpreGROUPNO)
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_MAKE_preGROUP message handlers

BOOL CBO_CAT_MAKE_preGROUP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nCreateYN = 0;
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_�����λ���")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_�����λ���"));

	CString m_Year;
	// 040304_HSS ���� LocalTime���� Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_����_����_�����λ���"), _T("�Լ��⵵"), m_Year);

	CString ACCESSION_REC_NO,tmp;
	pDM->GetOneValueQuery(_T("Select max(ACCESSION_REC_NO) from CO_TEMP_ACCESSION_REC_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE"), ACCESSION_REC_NO);

	tmp.Format(_T("%d"), _ttoi(ACCESSION_REC_NO.GetBuffer(0)) +1 );
	SetControlData(_T("CM_����_����_�����λ���"), _T("�����ι�ȣ"), tmp);

	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_MAKE_preGROUP::OnCREATpreGROUPNO() 
{
	// TODO: Add your control notification handler code here

	CString	strGroupNo,aquCode,acqYear;

	GetControlData(_T("CM_����_����_�����λ���"), _T("�����ι�ȣ"), strGroupNo);
	GetControlData(_T("CM_����_����_�����λ���"), _T("���Ա���"), aquCode);
	GetControlData(_T("CM_����_����_�����λ���"), _T("�Լ��⵵"), acqYear);
	
	if( strGroupNo == _T("") )
	{
		AfxMessageBox(_T("������ ��ȣ�� �Է��� �ֽʽÿ� !"));
		return;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_�����λ���"));

	CString tmp,Qry;
	Qry.Format(_T("Select COUNT(*) from CO_TEMP_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR='%s' AND ACCESSION_REC_NO = %s AND MANAGE_CODE = UDF_MANAGE_CODE "),acqYear,strGroupNo);
	pDM->GetOneValueQuery(Qry, tmp);
	
	if ( tmp == _T("1"))
	{
		AfxMessageBox(_T("�̹� ������ ������ �Դϴ� !"));
		return;
	}
	

	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return;
	}

	pDM->InitDBFieldData();

	CString strREC_KEY;
	ids = pDM->MakeRecKey(strREC_KEY);
	if ( 0 > ids ) return;
	
	pDM->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strREC_KEY);
	pDM->AddDBFieldData(_T("PUBLISH_FORM_CODE"),_T("STRING"),_T("MO")); 
	pDM->AddDBFieldData(_T("ACCESSION_REC_MAKE_YEAR"),_T("STRING"),acqYear); 
	pDM->AddDBFieldData(_T("ACQUISIT_CODE"),_T("STRING"),aquCode); 
	pDM->AddDBFieldData(_T("ACCESSION_REC_NO"),_T("NUMERIC"),strGroupNo);
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );

	ids = pDM->MakeInsertFrame(_T("CO_TEMP_ACCESSION_REC_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame �� Ȯ���ϼ���"));
		return;
	}

	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame �� Ȯ���ϼ���"));
		return;
	}

	ids = pDM->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame �� Ȯ���ϼ���"));
		return;
	}

	m_nCreateYN = 1;

	EndDialog(0);

}

HBRUSH CBO_CAT_MAKE_preGROUP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}