// StatisticsTermDlg.cpp : implementation file
// Make by pwr

#include "stdafx.h"
#include "StatisticsTermDlg.h"
#include "FeeStatisticsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsTermDlg dialog


CStatisticsTermDlg::CStatisticsTermDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CStatisticsTermDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatisticsTermDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CStatisticsTermDlg::~CStatisticsTermDlg()
{
}

void CStatisticsTermDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsTermDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsTermDlg, CDialog)
	//{{AFX_MSG_MAP(CStatisticsTermDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_rALL, OnrALL)
	ON_BN_CLICKED(IDC_rARREAR, OnrARREAR)
	ON_BN_CLICKED(IDC_rRECARD, OnrRECARD)
	ON_BN_CLICKED(IDC_rLOANSTOPCANCEL, OnrLOANSTOPCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsTermDlg message handlers

BOOL CStatisticsTermDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CStatisticsTermDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CStatisticsTermDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("FEE_STATISTICS_SEARCH") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_STATISTICS_SEARCH") );
		return FALSE;
	}

	SetDefaultCheck();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticsTermDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

// **�������� ���ϱ� (CM���� �ش� ���� ��ȯ�Ѵ�)
CString CStatisticsTermDlg::TakeControlData( CString alias, INT option )
{
EFS_BEGIN

	CString sDate;
	CESL_ControlMgr* pCM = FindCM( _T("CM_FEE_STATISTICS_SEARCH") );
	if( !pCM ) return _T("");
	CESL_Control_Element *pCE = (CESL_Control_Element*)pCM->FindControlElement( alias );
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )this->GetDlgItem( pCE->CTRL_ID );

	SYSTEMTIME tTime;
	DWORD dwResult = pDate->GetTime( &tTime );
	if (dwResult != GDT_VALID ) return _T("");
	sDate.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	
	return sDate;

EFS_END
return _T("");

}

// ** ������ư ����Ʈ ����
VOID CStatisticsTermDlg::SetDefaultCheck()
{
	// �˻����� : �Ϻ�
	(( CButton * )GetDlgItem( IDC_rD ))->SetCheck( TRUE );		
	// ���������� : ��ü
	(( CButton * )GetDlgItem( IDC_rALL ))->SetCheck( TRUE );	
}

// ** ���õ� �˻����� ��ȯ : �Ϻ�(D), ����(M), �⺰(Y)
CHAR CStatisticsTermDlg::GetSearchType()
{
	const int cnt = 3;
	unsigned int id[ cnt ] = {
		IDC_rD, IDC_rM, IDC_rY };
	CHAR flag[ cnt ] = {
		'D', 'M', 'Y' };

	CButton *pRidio = NULL;
	for( INT i = 0; i < cnt; i++ )
	{
		pRidio = ( CButton *)GetDlgItem( id[ i ] );
		if( pRidio->GetCheck() )
			return flag[ i ];
	}

	return ' ';
}

// ** ���õ� ���������� ��ȯ : ��ü(T), ��ü��(A), ȸ������߱�(R), ��������������(C)
CHAR CStatisticsTermDlg::GetFeeType()
{
	const int cnt = 4;
	unsigned int id[ cnt ] = {
		IDC_rALL, IDC_rARREAR, IDC_rRECARD, IDC_rLOANSTOPCANCEL };
	CHAR flag[ cnt ] = {
		'T', 'A', 'R', 'C' };

	CButton *pRidio = NULL;
	for( int i = 0; i < cnt; i++ )
	{
		pRidio = ( CButton *)GetDlgItem( id[ i ] );
		if( pRidio->GetCheck() )
			return flag[ i ];
	}

	return ' ';
}

// ** WHERE�� ����
CString CStatisticsTermDlg::MakeWhere( VOID *pParent )
{
EFS_BEGIN

	CString sWhere;	
	CString sPaymentFrom;
	CString sPaymentTo;
	
	CFeeStatisticsDlg *pParent = ( CFeeStatisticsDlg * )pParentMgr;

	//������ ����
	sPaymentFrom = TakeControlData( _T("�����Ϻ���") );
	//������ ��
	sPaymentTo = TakeControlData( _T("�����ϱ���") );

	//�˻�����
	pParent->m_cSearchType	= GetSearchType();
	//����������
	pParent->m_cFeeType		= GetFeeType();

	CFeeStatisticsDlg *parent = ( CFeeStatisticsDlg* )pParent;
	if( !sPaymentFrom.IsEmpty() && !sPaymentTo.IsEmpty() )
	{
		sWhere.Format(	
			_T("CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') ")
			_T("AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') ")
			, sPaymentFrom, sPaymentTo );
		parent->m_searchCondition.Format( _T("������ : %s ���� %s ����"), sPaymentFrom, sPaymentTo );
	}
	else if( !sPaymentFrom.IsEmpty() )
	{
		sWhere.Format(	
			_T("CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') ")
						_T("AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') ")
						, sPaymentFrom, sPaymentFrom );
		parent->m_searchCondition.Format( _T("������ : %s"), sPaymentFrom );
	}
	else
	{
		sWhere = _T("");
	}

	// ��������
	CString strManageCodeWhere, strManageCode;
	GetControlData( _T("CM_FEE_STATISTICS_SEARCH"), _T("��������"), strManageCode );
	strManageCode.TrimRight();
	if( strManageCode.IsEmpty() || (strManageCode.Compare(_T("0"))==0) || (strManageCode.Compare(_T("�������"))==0) ) 
	{	
		parent->m_strManageCodeWhere = _T("");
	}	
	else
	{
		strManageCodeWhere = _T("AND CFPI.MANAGE_CODE='") + strManageCode + _T("' ");
		parent->m_strManageCodeWhere = strManageCodeWhere;
	}

	// �ڷ��
	CString strShelfLocCodeWhere, strShelfLocCode;
	GetControlData( _T("CM_FEE_STATISTICS_SEARCH"), _T("�ڷ��"), strShelfLocCode );
	strShelfLocCode.TrimRight();
	if( strShelfLocCode.IsEmpty() || (strShelfLocCode.Compare(_T("0"))==0) || (strShelfLocCode.Compare(_T("�������"))==0) ) 
	{	
		parent->m_strShelfLocCodeWhere = _T("");
	}	
	else
	{
		strShelfLocCodeWhere = _T("AND CL.SHELF_LOC_CODE='") + strShelfLocCode + _T("' ");
		parent->m_strShelfLocCodeWhere = strShelfLocCodeWhere;
	}

	return sWhere;

EFS_END
return _T("");
}

// ** WHERE�� �����ؼ� �˻��Լ��� ������.
void CStatisticsTermDlg::OnOK() 
{
	AfxGetApp()->DoWaitCursor(1);
	
	CFeeStatisticsDlg *pParent = ( CFeeStatisticsDlg* )pParentMgr;
	CString sWhere = MakeWhere( pParent );
	if( !sWhere.IsEmpty() )
	{
		pParent->SearchData( sWhere );
	}
	
	AfxGetApp()->DoWaitCursor(-1);
}

// **�ݱ�
void CStatisticsTermDlg::OnCancel() 
{
	this->ShowWindow(FALSE);
}

HBRUSH CStatisticsTermDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CStatisticsTermDlg::OnrALL() 
{
	((CWnd*)GetDlgItem(IDC_cmbStatisticsTermShelfCode))->EnableWindow(TRUE);
}

void CStatisticsTermDlg::OnrARREAR() 
{
	((CWnd*)GetDlgItem(IDC_cmbStatisticsTermShelfCode))->EnableWindow(TRUE);
}

void CStatisticsTermDlg::OnrRECARD() 
{
	((CWnd*)GetDlgItem(IDC_cmbStatisticsTermShelfCode))->EnableWindow(FALSE);
}

void CStatisticsTermDlg::OnrLOANSTOPCANCEL() 
{
	((CWnd*)GetDlgItem(IDC_cmbStatisticsTermShelfCode))->EnableWindow(FALSE);
}
