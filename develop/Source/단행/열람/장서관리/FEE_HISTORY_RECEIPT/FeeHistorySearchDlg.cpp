// FeeHistorySearchDlg.cpp : implementation file
// MAKE BY PWR

#include "stdafx.h"
#include "FeeHistorySearchDlg.h"
#include "FeeHistoryReceiptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeHistorySearchDlg dialog


CFeeHistorySearchDlg::CFeeHistorySearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFeeHistorySearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeHistorySearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CFeeHistorySearchDlg::~CFeeHistorySearchDlg()
{
}

void CFeeHistorySearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeHistorySearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeHistorySearchDlg, CDialog)
	//{{AFX_MSG_MAP(CFeeHistorySearchDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_chkFEE_ALL, OnchkFEE_ALL)
	ON_BN_CLICKED(IDC_chkLOAN, OnchkLOAN)
	ON_BN_CLICKED(IDC_chkRETURN, OnchkRETURN)
	ON_BN_CLICKED(IDC_chkRECEIPT_ALL, OnchkRECEIPT_ALL)
	ON_BN_CLICKED(IDC_chkRECEIPT_Y, OnchkRECEIPT_Y)
	ON_BN_CLICKED(IDC_chkRECEIPT_N, OnchkRECEIPT_N)
	ON_BN_CLICKED(IDC_btnSEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_chkFEE_ARREAR, OnchkFEE_ARREAR)
	ON_BN_CLICKED(IDC_chkFEE_LOANSTOPCANCEL, OnchkFEE_LOANSTOPCANCEL)
	ON_BN_CLICKED(IDC_chkFEE_RECARD, OnchkFEE_RECARD)
	ON_BN_CLICKED(IDC_chkPAYMENT, OnchkPAYMENT)
	ON_BN_CLICKED(IDC_chkPAYMENT_Y, OnchkPAYMENTY)
	ON_BN_CLICKED(IDC_chkPAYMENT_N, OnchkPAYMENTN)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeHistorySearchDlg message handlers

BOOL CFeeHistorySearchDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFeeHistorySearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFeeHistorySearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("FEE_HISTORY_SEARCH") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_HISTORY_SEARCH") );
		return false;
	}

	// ��Ʈ�� �ʱⰪ ( ��ü��, ��������, �������Y/N )
	EnableControl( 1, FALSE );
	EnableControl( 2, FALSE );
	// ���� ���� = Y
	((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_chkPAYMENT_ALL))->SetCheck(FALSE);
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

void CFeeHistorySearchDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

// 2008.03.03 ADD BY PWR
// ** ���������� : ��ü
void CFeeHistorySearchDlg::OnchkFEE_ALL() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkFEE_ALL))->GetCheck();
	if( bCheck )
	{//äũ��
		// 1. ���������� ����üũ
		EnableControl( 0, TRUE );
		// 2. �������ڸ� Ȱ��ȭ
		EnableControl( 1, FALSE );
	}
	else
	{//äũ����
		// 1. ��ü�Ḹ üũ
		((CButton*)GetDlgItem(IDC_chkFEE_ARREAR))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_chkFEE_RECARD))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_chkFEE_LOANSTOPCANCEL))->SetCheck(FALSE);
		// 2. �Ⱓ���� ���� Ȱ��ȭ
		EnableControl( 1, TRUE );
	}
}

// 2008.03.03 ADD BY PWR
// ** ���������� : ��ü��
void CFeeHistorySearchDlg::OnchkFEE_ARREAR() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkFEE_ARREAR))->GetCheck();
	if( bCheck )
	{//äũ��
		// 1. ȸ������߱ް� ���������������� ��Ȱ��ȭ���� �˻�
		BOOL bCheckReCard			= ((CButton*)GetDlgItem(IDC_chkFEE_RECARD))->GetCheck();
		BOOL bCheckLoanStopCancel	= ((CButton*)GetDlgItem(IDC_chkFEE_LOANSTOPCANCEL))->GetCheck();
		// 2. �Ѵ� ��Ȱ��ȭ�϶� �Ⱓ��� Ȱ��ȭ
		if( !bCheckReCard && !bCheckLoanStopCancel )
		{
			EnableControl( 1, TRUE );
		}
		// 3. �������� Ȱ��ȭ
		((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(TRUE);
		// 4. �ڷ�� Ȱ��ȭ
		((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(TRUE);
	}
	else
	{//äũ����
		EnableControl( 0, FALSE );
	}
}

// 2008.03.03 ADD BY PWR
// ** ���������� : ȸ������߱�
void CFeeHistorySearchDlg::OnchkFEE_RECARD() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkFEE_RECARD))->GetCheck();
	if( bCheck )
	{//äũ��
		// �Ⱓ���� �������ڸ� Ȱ��ȭ
		EnableControl( 1, FALSE );
	}
	else
	{//äũ����
		EnableControl( 0, FALSE );
	}
}

// 2008.03.03 ADD BY PWR
// ** ���������� : ��������������
void CFeeHistorySearchDlg::OnchkFEE_LOANSTOPCANCEL() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkFEE_LOANSTOPCANCEL))->GetCheck();
	if( bCheck )
	{//äũ��
		// �Ⱓ���� �������ڸ� Ȱ��ȭ
		EnableControl( 1, FALSE );
	}
	else
	{//äũ����
		EnableControl( 0, FALSE );
	}
}

// 2008.03.03 ADD BY PWR
// ** �Ⱓ���� : ��������
void CFeeHistorySearchDlg::OnchkPAYMENT()
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkPAYMENT))->GetCheck();
	if( bCheck )
	{//äũ��
		((CButton*)GetDlgItem(IDC_datPAYMENT_DATE1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_datPAYMENT_DATE2))->EnableWindow(TRUE);
	}
	else
	{//äũ����
		((CButton*)GetDlgItem(IDC_datPAYMENT_DATE1))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_datPAYMENT_DATE2))->EnableWindow(FALSE);
	}
}

// 2008.03.03 ADD BY PWR
// ** �Ⱓ���� : ��������
void CFeeHistorySearchDlg::OnchkLOAN() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkLOAN))->GetCheck();
	if( bCheck )
	{//äũ��
		((CButton*)GetDlgItem(IDC_datLOAN_DATE1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_datLOAN_DATE2))->EnableWindow(TRUE);
	}
	else
	{//äũ����
		((CButton*)GetDlgItem(IDC_datLOAN_DATE1))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_datLOAN_DATE2))->EnableWindow(FALSE);
	}
}

// 2008.03.03 ADD BY PWR
// ** �Ⱓ���� : �ݳ�����
void CFeeHistorySearchDlg::OnchkRETURN() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkRETURN))->GetCheck();
	if( bCheck )
	{//äũ��
		((CButton*)GetDlgItem(IDC_datRETURN_DATE1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_datRETURN_DATE2))->EnableWindow(TRUE);
	}
	else
	{//äũ����
		((CButton*)GetDlgItem(IDC_datRETURN_DATE1))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_datRETURN_DATE2))->EnableWindow(FALSE);
	}
}

// 2008.03.03 ADD BY PWR
// ** ��������¿��� : ����
void CFeeHistorySearchDlg::OnchkRECEIPT_ALL() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkRECEIPT_ALL))->GetCheck();
	if( bCheck )
	{//äũ��
		EnableControl( 2, TRUE );
	}
	else
	{//äũ����
		EnableControl( 2, FALSE );
	}
}

// 2008.03.03 ADD BY PWR
// ** ��������¿��� : Y
void CFeeHistorySearchDlg::OnchkRECEIPT_Y() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkRECEIPT_Y))->GetCheck();
	if( bCheck )
	{//äũ��
		
	}
	else
	{//äũ����
		((CButton*)GetDlgItem(IDC_chkRECEIPT_ALL))->SetCheck(FALSE);
	}
}

// 2008.03.03 ADD BY PWR
// ** ��������¿��� : N
void CFeeHistorySearchDlg::OnchkRECEIPT_N() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkRECEIPT_N))->GetCheck();
	if( bCheck )
	{//äũ��

	}
	else
	{//äũ����
		((CButton*)GetDlgItem(IDC_chkRECEIPT_ALL))->SetCheck(FALSE);
	}
}

// 2008.04.29 ADD BY PWR
// ** ������ �������� : Y
void CFeeHistorySearchDlg::OnchkPAYMENTY() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->GetCheck();
	if( bCheck )
	{//äũ��
		
	}
	else
	{//äũ����
		((CButton*)GetDlgItem(IDC_chkPAYMENT_ALL))->SetCheck(FALSE);
	}
}

// 2008.04.29 ADD BY PWR
// ** ������ �������� : N
void CFeeHistorySearchDlg::OnchkPAYMENTN() 
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->GetCheck();
	if( bCheck )
	{//äũ��
		
	}
	else
	{//äũ����
		((CButton*)GetDlgItem(IDC_chkPAYMENT_ALL))->SetCheck(FALSE);
	}
}

// 2008.03.04 ADD BY PWR
// ** ��Ʈ���� Ȱ��ȭ �� ��Ȱ��ȭ
// nType ( 0:����������, 1:�Ⱓ����, 2:������������� )
VOID CFeeHistorySearchDlg::EnableControl( INT nType, BOOL bEnable )
{
	BOOL bCheckArrear		  = ((CButton*)GetDlgItem(IDC_chkFEE_ARREAR))->GetCheck();
	BOOL bCheckReCard		  = ((CButton*)GetDlgItem(IDC_chkFEE_RECARD))->GetCheck();
	BOOL bCheckLoanStopCancel = ((CButton*)GetDlgItem(IDC_chkFEE_LOANSTOPCANCEL))->GetCheck();

	////////////////////////////////////////////////////////////////////////////
	// ���������� ��Ʈ��
	if( nType == 0 )
	{
		if( bEnable )
		{
			// ���������� ����üũ
			((CButton*)GetDlgItem(IDC_chkFEE_ARREAR))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_chkFEE_RECARD))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_chkFEE_LOANSTOPCANCEL))->SetCheck(TRUE);
		}
		else
		{	
			((CButton*)GetDlgItem(IDC_chkFEE_ALL))->SetCheck(FALSE);

			// �����Ҷ� ������������ ���� üũ�� �ȵǾ��ٸ�
			if( !bCheckArrear && !bCheckReCard && !bCheckLoanStopCancel )
			{
				// ��ü�˻�
				// ���αⰣ�� Ȱ��ȭ
				EnableControl( 1, FALSE );
			}
			// ȸ������߱��̳� ���������������� �ϳ��� Ȱ��ȭ�� ������
			else if( bCheckReCard || bCheckLoanStopCancel )
			{
				// ���αⰣ�� Ȱ��ȭ
				EnableControl( 1, FALSE );
			}
			// ��ü�Ḹ Ȱ��ȭ
			else if( bCheckArrear && !bCheckReCard && !bCheckLoanStopCancel )
			{
				// �Ⱓ���� Ȱ��ȭ
				EnableControl( 1, TRUE );
			}
			// ��ü�� Ȱ��ȭ
			if( bCheckArrear || ( !bCheckArrear && !bCheckReCard && !bCheckLoanStopCancel ) )
			{
				((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(TRUE);
				((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(TRUE);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(FALSE);
				((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(FALSE);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////
	// �Ⱓ���� ��Ʈ��
	else if( nType == 1 )
	{
		// �Ⱓ������ ���� Ȱ��ȭ�Ѵ�. + ���������� Ȱ��ȭ
		if( bEnable )
		{
			// 1. �������� Ȱ��ȭ
			((CButton*)GetDlgItem(IDC_chkPAYMENT))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_chkPAYMENT))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_datPAYMENT_DATE1))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_datPAYMENT_DATE2))->EnableWindow(TRUE);
			// 2. �������� Ȱ��ȭ
			((CButton*)GetDlgItem(IDC_chkLOAN))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_chkLOAN))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_datLOAN_DATE1))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_datLOAN_DATE2))->EnableWindow(FALSE);
			// 3. �ݳ����� Ȱ��ȭ
			((CButton*)GetDlgItem(IDC_chkRETURN))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_chkRETURN))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_datRETURN_DATE1))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_datRETURN_DATE2))->EnableWindow(FALSE);
			// 4. �������� Ȱ��ȭ
			((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(TRUE);
			((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(TRUE);
		}
		// �Ⱓ�������� �������ڸ� Ȱ��ȭ�ϰ� �������� ��Ȱ��ȭ�Ѵ�. + ���������� ��Ȱ��ȭ
		else
		{
			// 1. �������� Ȱ��ȭ
			((CButton*)GetDlgItem(IDC_chkPAYMENT))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_chkPAYMENT))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_datPAYMENT_DATE1))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_datPAYMENT_DATE2))->EnableWindow(TRUE);
			// 2. �������� ��Ȱ��ȭ
			((CButton*)GetDlgItem(IDC_chkLOAN))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_chkLOAN))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_datLOAN_DATE1))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_datLOAN_DATE2))->EnableWindow(FALSE);
			// 3. �ݳ����� ��Ȱ��ȭ
			((CButton*)GetDlgItem(IDC_chkRETURN))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_chkRETURN))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_datRETURN_DATE1))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_datRETURN_DATE2))->EnableWindow(FALSE);
			// 4. �������� ��Ȱ��ȭ
			if( bCheckArrear || ( !bCheckArrear && !bCheckReCard && !bCheckLoanStopCancel ) )
			{
				((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(TRUE);
				((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(TRUE);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_chkPAYMENT_Y))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_chkPAYMENT_N))->EnableWindow(FALSE);
				((CWnd*)GetDlgItem(IDC_cmbHistorySearchShelfCode))->EnableWindow(FALSE);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////
	//������������� ��Ʈ��
	else if( nType == 2 )
	{
		if( bEnable )
		{
			((CButton*)GetDlgItem(IDC_chkRECEIPT_ALL))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_chkRECEIPT_Y))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_chkRECEIPT_N))->SetCheck(TRUE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_chkRECEIPT_ALL))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_chkRECEIPT_Y))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_chkRECEIPT_N))->SetCheck(FALSE);
		}
	}
	else
	{
		return;
	}
}

// 2008.03.03 ADD BY PWR
// ** �˻���ư ( ���� �����ؼ� �˻��Լ��� ������. )
void CFeeHistorySearchDlg::OnbtnSEARCH() 
{
	AfxGetApp()->DoWaitCursor(1);
	
	CFeeHistoryReceiptDlg *pParent = ( CFeeHistoryReceiptDlg* )pParentMgr;
	CString sQuery = MakeQuery( pParent );
	if( !sQuery.IsEmpty() )
	{
		pParent->SearchData( sQuery );
	}
	
	AfxGetApp()->DoWaitCursor(-1);
}

// 2008.03.03 ADD BY PWR
// ** �ݱ��ư
void CFeeHistorySearchDlg::OnbtnCLOSE() 
{
	this->ShowWindow(FALSE);
}

// 2008.03.04 ADD BY PWR
// ** Query ����
CString CFeeHistorySearchDlg::MakeQuery( VOID *pParent )
{
	CString sQuery, sQuery1, sQuery1_1, sQuery2, sQuery3, sQuery4;
	CString sFeeWhere1, sFeeWhere2, sFeeWhere3, sFeeWhere4;
	CString sDateWhere, sUserNoWhere, sUserNameWhere, sReceiptWhere, sPaymentWhere, sSearchWhere;
	CString strManageCodeWhere, strShelfLocCodeWhere;

	// 0:��ü�Ḹ���õǾ�������(CASE1)
	// 1:��ü��Ͱ���(UNION)
	// 2:ȸ������߱��̳� ��������������(CASE2)
	INT nType = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 1. üũ�ڽ��� ���������� �����´�.
	INT	 nCheckCnt = 13;
	BOOL bCheck[13];
	INT	 nCheckName[13] = { IDC_chkFEE_ALL				, IDC_chkFEE_ARREAR	, IDC_chkFEE_RECARD	, 
							IDC_chkFEE_LOANSTOPCANCEL	, IDC_chkPAYMENT	, IDC_chkLOAN		, 
							IDC_chkRETURN				, IDC_chkRECEIPT_ALL, IDC_chkRECEIPT_Y	, 
							IDC_chkRECEIPT_N			, IDC_chkPAYMENT_ALL, IDC_chkPAYMENT_Y	,
							IDC_chkPAYMENT_N	};

	for(INT i = 0; i < nCheckCnt; i++)
	{
		bCheck[i] = ((CButton*)GetDlgItem(nCheckName[i]))->GetCheck();
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 2. �����ڹ�ȣ�� �����ڸ��� �����´�.
	const int nCnt = 4;
	CString sControlAlias[nCnt] = 
				{ _T("�����ڹ�ȣ1") , _T("�����ڹ�ȣ2") , _T("�����ڸ�1") , _T("�����ڸ�2") };
	
	CString sGetData[nCnt];
	for( i = 0 ; i < nCnt; i++ )
	{
		GetControlData( _T("CM_FEE_HISTORY_SEARCH") , sControlAlias[i] , sGetData[i] , -1 , 0 );
		sGetData[i].TrimRight();
	}

	
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 3. ������������ ���� ���ǻ���
	// 3.1 ��ü��
	sQuery1 =	_T("SELECT CLU.USER_NO, CLU.IDX_USER_NAME, CLU.USER_CLASS, CLU.USER_POSITION_CODE, ")
				_T("CLU.USER_CLASS_CODE, CFPI.RECEIPT_PRINT_YN, CFPI.CLASS_CODE, CFPI.MONEY_PAID, ")
				_T("TO_CHAR( CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS' ) AS PAYMENT_DATE, ")
				_T("TO_CHAR( CL.LOAN_DATE, 'YYYY/MM/DD' ), TO_CHAR( CL.RETURN_PLAN_DATE, 'YYYY/MM/DD' ), ")
				_T("TO_CHAR( CL.RETURN_DATE, 'YYYY/MM/DD' ), CL.REG_NO, CL.TITLE, ") // 2008.03.19 ADD BY PWR ( ����, ��Ϲ�ȣ )
				_T("CL.SHELF_LOC_CODE, CFPI.RECEIPT_KEY, CFPI.CARD_CLASS, CFPI.USER_ID, ")
				_T("CFPI.REC_KEY, CFPI.LOAN_INFO_KEY, CFPI.LOAN_STOP_DATE, CFPI.RETRO_YN, CFPI.MANAGE_CODE ")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE CL.REC_KEY=CFPI.LOAN_INFO_KEY AND CL.USER_KEY=CLU.REC_KEY AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.RETRO_YN IS NULL ");
	sQuery1_1 =	_T("SELECT CLU.USER_NO, CLU.IDX_USER_NAME, CLU.USER_CLASS, CLU.USER_POSITION_CODE, ")
				_T("CLU.USER_CLASS_CODE, CFPI.RECEIPT_PRINT_YN, CFPI.CLASS_CODE, CFPI.MONEY_PAID, ")
				_T("TO_CHAR( CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS' ) AS PAYMENT_DATE, ")
				_T("TO_CHAR( CL.LOAN_DATE, 'YYYY/MM/DD' ), TO_CHAR( CL.RETURN_PLAN_DATE, 'YYYY/MM/DD' ), ")
				_T("TO_CHAR( CL.RETURN_DATE, 'YYYY/MM/DD' ), CL.REG_NO, CL.TITLE, ") // 2008.03.19 ADD BY PWR ( ����, ��Ϲ�ȣ )
				_T("CL.SHELF_LOC_CODE, CFPI.RECEIPT_KEY, CFPI.CARD_CLASS, CFPI.USER_ID, ")
				_T("CFPI.REC_KEY, CFPI.LOAN_INFO_KEY, CFPI.LOAN_STOP_DATE, CFPI.RETRO_YN, CFPI.MANAGE_CODE ")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE CL.REC_KEY=CFPI.LOAN_INFO_KEY AND CL.USER_KEY=CLU.REC_KEY AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.RETRO_YN IS NULL ");
	// 3.2 ȸ������߱�, ��������������
	sQuery2 =	_T("SELECT CLU.USER_NO, CLU.IDX_USER_NAME, CLU.USER_CLASS, CLU.USER_POSITION_CODE, ")
				_T("CLU.USER_CLASS_CODE, CFPI.RECEIPT_PRINT_YN, CFPI.CLASS_CODE, CFPI.MONEY_PAID, ")
				_T("TO_CHAR( CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS' ) AS PAYMENT_DATE, ")
				_T("NULL, NULL, NULL, NULL, NULL, NULL, ")
				_T("CFPI.RECEIPT_KEY, CFPI.CARD_CLASS, CFPI.USER_ID, ")
				_T("CFPI.REC_KEY, CFPI.LOAN_INFO_KEY, CFPI.LOAN_STOP_DATE, CFPI.RETRO_YN, CFPI.MANAGE_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN IS NULL ");
	// 2009.03.06 ADD BY PWR : �ұ��Է� �߰�
	// ��ü��
	sQuery3 =	_T("SELECT CLU.USER_NO, CLU.IDX_USER_NAME, CLU.USER_CLASS, CLU.USER_POSITION_CODE, ")
				_T("CLU.USER_CLASS_CODE, CFPI.RECEIPT_PRINT_YN,CFPI.CLASS_CODE, CFPI.MONEY_PAID, ")
				_T("TO_CHAR( CFPI.PAYMENT_DATE, 'YYYY/MM/DD HH24:MI:SS' ) AS PAYMENT_DATE,")
				_T("NULL,NULL,NULL,NULL,NULL,NULL,CFPI.RECEIPT_KEY,CFPI.CARD_CLASS,CFPI.USER_ID,")
				_T("CFPI.REC_KEY,CFPI.LOAN_INFO_KEY,CFPI.LOAN_STOP_DATE, CFPI.RETRO_YN, CFPI.MANAGE_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE CLU.REC_KEY=CFPI.LOAN_INFO_KEY AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS IN('Y')");
	
	// ȸ������߱�, ��������������
	sQuery4 = sQuery3;

	MakeFeeWhere( bCheck, sFeeWhere1, sFeeWhere2, sFeeWhere3, sFeeWhere4, nType );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 4. �Ⱓ������ ���� ���ǻ���
	sDateWhere = _T("");
	MakeDateWhere( bCheck, sDateWhere );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 5. �����ڹ�ȣ
	sUserNoWhere = _T("");
	MakeUserNoWhere( sGetData, sUserNoWhere );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 6. �����ڸ�
	sUserNameWhere = _T("");
	MakeUserNameWhere( sGetData, sUserNameWhere );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 7. �������������
	sReceiptWhere = _T("");
	MakeReceiptWhere( bCheck, sReceiptWhere );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 8. �����ᳳ������
	sPaymentWhere = _T("");
	MakePaymentWhere( bCheck, sPaymentWhere );

	// 2009.03.09 ADD BY PWR : �������� ���� �߰�
	strManageCodeWhere = _T("");
	MakeManageCodeWhere( strManageCodeWhere );

	// 2009.03.09 ADD BY PWR : �ڷ�� ���� �߰�
	strShelfLocCodeWhere = _T("");
	MakeShelfLocCodeWhere( strShelfLocCodeWhere );

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 10. ������ ���� ��ġ��
	// 10.1 ��ü�Ḹ���õǾ�������(CASE1)
	if( nType == 0 )
	{
		// 2009.10.07 UPDATE BY KSJ : �������� / �ݳ����� ������ �ִٸ� �ұ��� �����ؼ� �˻�
		//DEL if(strShelfLocCodeWhere.IsEmpty())
		if( TRUE == strShelfLocCodeWhere.IsEmpty() && TRUE != bCheck[5] && TRUE != bCheck[6] )
		{
			sQuery =	sQuery1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere 
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") +
						sQuery1_1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere 
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") +
						sQuery3 + sFeeWhere3 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + 
						_T(" ORDER BY PAYMENT_DATE DESC, USER_NO");
		}
		else // �ڷ�� ������ �ִٸ� �ұ��� �����ؼ� �˻�
		{
			sQuery =	sQuery1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere 
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") +
						sQuery1_1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere 
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" ORDER BY PAYMENT_DATE DESC, USER_NO");
		}
	}
	// 10.2 ��ü��Ͱ���(UNION)
	else if( nType == 1 )
	{
		if(strShelfLocCodeWhere.IsEmpty())
		{
			sQuery =	sQuery1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") + 
						sQuery1_1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") + 
						sQuery2 + sFeeWhere2 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + 
						_T(" UNION ALL ") +
						sQuery3 + sFeeWhere3 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + 
						_T(" UNION ALL ") +
						sQuery4 + sFeeWhere4 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere +
						_T(" ORDER BY PAYMENT_DATE DESC, USER_NO");
		}
		else // �ڷ�� ������ �ִٸ� �ұ��� �����ؼ� �˻�
		{
			sQuery =	sQuery1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") + 
						sQuery1_1 + sFeeWhere1 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + strShelfLocCodeWhere + 
						_T(" UNION ALL ") + 
						sQuery2 + sFeeWhere2 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere + 
						_T(" UNION ALL ") +
						sQuery4 + sFeeWhere4 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
						+ sPaymentWhere + strManageCodeWhere +
						_T(" ORDER BY PAYMENT_DATE DESC, USER_NO");
		}
	}
	// 10.3 ȸ������߱��̳� ��������������(CASE2)
	else if( nType == 2 )
	{ 
		sQuery =	sQuery2 + sFeeWhere2 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
					+ sPaymentWhere + strManageCodeWhere + 
					_T(" UNION ALL ") +
					sQuery4 + sFeeWhere4 + sDateWhere + sUserNoWhere + sUserNameWhere + sReceiptWhere
					+ sPaymentWhere + strManageCodeWhere + 
					_T(" ORDER BY PAYMENT_DATE DESC, USER_NO");
	}

	return sQuery;
}

// **�Ⱓ���� ���� ���ϱ� (CM���� �ش� ���� ��ȯ�Ѵ�)
CString CFeeHistorySearchDlg::TakeControlData( CString sAlias )
{
EFS_BEGIN

	CString sDate;
	CESL_ControlMgr* pCM = FindCM( _T("CM_FEE_HISTORY_SEARCH") );
	if( !pCM ) return _T("");
	CESL_Control_Element *pCE = (CESL_Control_Element*)pCM->FindControlElement( sAlias );
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )this->GetDlgItem( pCE->CTRL_ID );

	SYSTEMTIME tTime;
	DWORD dwResult = pDate->GetTime( &tTime );
	if (dwResult != GDT_VALID ) return _T("");
	sDate.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	
	return sDate;

EFS_END
return _T("");
}

// 2008.03.04 ADD BY PWR
// ** ������������ ���� WHERE����
VOID CFeeHistorySearchDlg::MakeFeeWhere( BOOL bCheck[], CString &sFeeWhere1, CString &sFeeWhere2, CString &sFeeWhere3, CString &sFeeWhere4, INT &nType )
{
	// 000 ��� ���þ���
	if( !bCheck[1] && !bCheck[2] && !bCheck[3] )
	{
		sFeeWhere1 = _T(" AND CFPI.CLASS_CODE IN('0')");
		//sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN(1, 2)");
		sFeeWhere2 = _T(" AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1'))");// 2008.06.02 ADD BY PWR ( ����������0������ )
		// 2009.03.09 ADD BY PWR : �ұ��Է�
		sFeeWhere3 = _T(" AND CFPI.CLASS_CODE IN('0')");
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('1','2')");
		nType = 1;//(UNION)
	}
	// 001 ���������������� ����
	else if( !bCheck[1] && !bCheck[2] && bCheck[3] )
	{
		//sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN(2)");
		sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0");			// 2008.06.02 ADD BY PWR ( 0������ )
		// 2009.03.09 ADD BY PWR : �ұ��Է�
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('2')");
		nType = 2;//(CASE2)
	}
	// 010 ȸ������߱޸� ����
	else if( !bCheck[1] && bCheck[2] && !bCheck[3] )
	{
		sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN('1')");
		// 2009.03.09 ADD BY PWR : �ұ��Է�
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('1')");
		nType = 2;//(CASE2)
	}
	// 011 ȸ������߱ް� �������������� ����
	else if( !bCheck[1] && bCheck[2] && bCheck[3] )
	{
		//sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN(1, 2)");
		sFeeWhere2 = _T(" AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1'))");// 2008.06.02 ADD BY PWR ( ����������0������ )
		// 2009.03.09 ADD BY PWR : �ұ��Է�
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('1','2')");
		nType = 2;//(CASE2)
	}
	// 100 ��ü�Ḹ ����
	else if( bCheck[1] && !bCheck[2] && !bCheck[3] )
	{
		sFeeWhere1 = _T(" AND CFPI.CLASS_CODE IN('0')");
		// 2009.03.09 ADD BY PWR : �ұ��Է�
		sFeeWhere3 = _T(" AND CFPI.CLASS_CODE IN('0')");
		nType = 0;//(CASE1)
	}
	// 101 ��ü��� �������������� ����
	else if( bCheck[1] && !bCheck[2] && bCheck[3] )
	{
		sFeeWhere1 = _T(" AND CFPI.CLASS_CODE IN('0')");
		//sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN(2)");
		sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0");			// 2008.06.02 ADD BY PWR ( 0������ )
		// 2009.03.09 ADD BY PWR : �ұ��Է�
		sFeeWhere3 = _T(" AND CFPI.CLASS_CODE IN('0')");
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('2')");
		nType = 1;//(UNION)
	}
	// 110 ��ü��� ȸ������߱� ����
	else if( bCheck[1] && bCheck[2] && !bCheck[3] )
	{
		sFeeWhere1 = _T(" AND CFPI.CLASS_CODE IN('0')");
		sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN('1')");
		// 2009.03.09 ADD BY PWR : �ұ��Է�
		sFeeWhere3 = _T(" AND CFPI.CLASS_CODE IN('0')");
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('1')");
		nType = 1;//(UNION)
	}
	// 111 ��μ���
	else if( bCheck[1] && bCheck[2] && bCheck[3] )
	{
		sFeeWhere1 = _T(" AND CFPI.CLASS_CODE IN('0')");
		//sFeeWhere2 = _T(" AND CFPI.CLASS_CODE IN(1, 2)");
		sFeeWhere2 = _T(" AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1'))");// 2008.06.02 ADD BY PWR ( ����������0������ )
		// 2009.03.09 ADD BY PWR : �ұ��Է�
		sFeeWhere3 = _T(" AND CFPI.CLASS_CODE IN('0')");
		sFeeWhere4 = _T(" AND CFPI.CLASS_CODE IN('1','2')");
		nType = 1;//(UNION)
	} 
}

// 2008.03.04 ADD BY PWR
// ** �Ⱓ������ ���� WHERE����
VOID CFeeHistorySearchDlg::MakeDateWhere( BOOL bCheck[], CString &sDateWhere )
{
	CString sPaymentDate1	= TakeControlData( _T("������1") );
	CString sPaymentDate2	= TakeControlData( _T("������2") );
	CString sLoanDate1		= TakeControlData( _T("������1") );
	CString sLoanDate2		= TakeControlData( _T("������2") );
	CString sReturnDate1	= TakeControlData( _T("�ݳ���1") );
	CString sReturnDate2	= TakeControlData( _T("�ݳ���2") );

	// 000 ��� ���� ���� ( ���Ǿ��� )
	if( !bCheck[4] && !bCheck[5] && !bCheck[6] )
	{
		sDateWhere = _T("");
	}
	// 001 �ݳ��ϸ� ���� ( �ݳ��� �˻� )
	else if( !bCheck[4] && !bCheck[5] && bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query ���� ����
		if( TRUE != sReturnDate1.IsEmpty() )
		{
			if( TRUE != sReturnDate2.IsEmpty() )
			{
				sDateWhere.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate2 );
			}
			else
			{
				sDateWhere.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate1 );
			}
		}
	}
	// 010 �����ϸ� ���� ( ������ �˻� )
	else if( !bCheck[4] && bCheck[5] && !bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query ���� ����
		if( TRUE != sLoanDate1.IsEmpty() )
		{
			if( TRUE != sLoanDate2.IsEmpty() )
			{
				sDateWhere.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate2 );
			}
			else
			{
				sDateWhere.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate1 );
			}
		}			
	}
	// 011 �����ϰ� �ݳ��� ���� ( ������,�ݳ��� �˻� )
	else if( !bCheck[4] && bCheck[5] && bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query ���� ����
		// sDateWhere.Format(	
		// 	_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
		// 	_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
		// 	_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
		// 	_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
		// 	, sLoanDate1, sLoanDate2, sReturnDate1, sReturnDate2 );
		CString strWhereLoanDate , strWhereReturnDate;
		strWhereLoanDate = _T("");		strWhereReturnDate = _T("");
		if( TRUE != sLoanDate1.IsEmpty() )
		{
			if( TRUE != sLoanDate2.IsEmpty() )
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate2 );
			}
			else
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate1 );
			}
		}

		if( TRUE != sReturnDate1.IsEmpty() )
		{
			if( TRUE != sReturnDate2.IsEmpty() )
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate2 );
			}
			else
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate1 );
			}
		}
		
		sDateWhere = strWhereLoanDate + strWhereReturnDate;
	}
	// 100 �����ϸ� ���� ( ������ �˻� )
	else if( bCheck[4] && !bCheck[5] && !bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query ���� ����
		if( TRUE != sPaymentDate1.IsEmpty() )
		{
			if( TRUE != sPaymentDate2.IsEmpty() )
			{
				sDateWhere.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate2 );
			}
			else
			{
				sDateWhere.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate1 );				
			}
		}
	}
	// 101 �����ϰ� �ݳ��ϼ��� ( ������,�ݳ��� �˻� )
	else if( bCheck[4] && !bCheck[5] && bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query ���� ����
//DEL 		sDateWhere.Format(	
//DEL 			_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			, sPaymentDate1, sPaymentDate2, sReturnDate1, sReturnDate2 );
		CString strWherePaymentDate , strWhereReturnDate;
		strWherePaymentDate = _T("");		strWhereReturnDate = _T("");
		if( TRUE != sPaymentDate1.IsEmpty() )
		{
			if( TRUE != sPaymentDate2.IsEmpty() )
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate2 );
			}
			else
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate1 );				
			}
		}
		
		if( TRUE != sReturnDate1.IsEmpty() )
		{
			if( TRUE != sReturnDate2.IsEmpty() )
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate2 );
			}
			else
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate1 );
			}
		}

		sDateWhere = strWherePaymentDate + strWhereReturnDate;
	}
	// 110 �����ϰ� �����ϼ��� ( ������,������ �˻� )
	else if( bCheck[4] && bCheck[5] && !bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query ���� ����
//DEL 		sDateWhere.Format(	
//DEL 			_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			, sPaymentDate1, sPaymentDate2, sLoanDate1, sLoanDate2 );
		CString strWherePaymentDate , strWhereLoanDate;
		strWherePaymentDate = _T("");		strWhereLoanDate = _T("");
		if( TRUE != sPaymentDate1.IsEmpty() )
		{
			if( TRUE != sPaymentDate2.IsEmpty() )
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate2 );
			}
			else
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate1 );				
			}
		}
		
		if( TRUE != sLoanDate1.IsEmpty() )
		{
			if( TRUE != sLoanDate2.IsEmpty() )
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate2 );
			}
			else
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate1 );
			}
		}

		sDateWhere = strWherePaymentDate + strWhereLoanDate;
	}
	// 111 ��� ���� ( ������, ������, �ݳ��� �˻� )
	else if( bCheck[4] && bCheck[5] && bCheck[6] )
	{
		// 2009.11.05 UPDATE BY KSJ : Query ���� ����
//DEL 		sDateWhere.Format(	
//DEL 			_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			//===============================================================================
//DEL 			// 2009.10.16 UPDATE BY KSJ : Where ���ǿ� AS �� �� ����
//DEL 			//DEL _T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AS ORDER_BY_DATE")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') ")
//DEL 			//===============================================================================
//DEL 			_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
//DEL 			, sPaymentDate1, sPaymentDate2, sLoanDate1, sLoanDate2, sReturnDate1, sReturnDate2 );
		CString strWherePaymentDate , strWhereLoanDate , strWhereReturnDate;
		strWherePaymentDate = _T(""); strWhereLoanDate = _T(""); strWhereReturnDate = _T("");
		if( TRUE != sPaymentDate1.IsEmpty() )
		{
			if( TRUE != sPaymentDate2.IsEmpty() )
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate2 );
			}
			else
			{
				strWherePaymentDate.Format(	
				_T(" AND CFPI.PAYMENT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sPaymentDate1, sPaymentDate1 );				
			}
		}

		if( TRUE != sLoanDate1.IsEmpty() )
		{
			if( TRUE != sLoanDate2.IsEmpty() )
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate2 );
			}
			else
			{
				strWhereLoanDate.Format(	
				_T(" AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sLoanDate1, sLoanDate1 );
			}
		}

		if( TRUE != sReturnDate1.IsEmpty() )
		{
			if( TRUE != sReturnDate2.IsEmpty() )
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate2 );
			}
			else
			{
				strWhereReturnDate.Format(	
				_T(" AND CL.RETURN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
				_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')")
				, sReturnDate1, sReturnDate1 );
			}
		}

		sDateWhere = strWherePaymentDate + strWhereLoanDate + strWhereReturnDate;
	}
}

// 2008.03.04 ADD BY PWR
// ** �����ڹ�ȣ�� ���� WHERE����
VOID CFeeHistorySearchDlg::MakeUserNoWhere( CString sGetData[], CString &sUserNoWhere )
{
	if( !sGetData[0].IsEmpty() )
	{
		sUserNoWhere = _T(" AND");
		if( !sGetData[1].IsEmpty() )
			sUserNoWhere += _T(" CLU.USER_NO BETWEEN '") + sGetData[0] + _T("' AND '") + sGetData[1]+ _T("'");
		else
			sUserNoWhere += _T(" CLU.USER_NO = '") + sGetData[0] + _T("'");
	}
}

// 2008.03.04 ADD BY PWR
// ** �����ڸ� ���� WHERE����
VOID CFeeHistorySearchDlg::MakeUserNameWhere( CString sGetData[], CString &sUserNameWhere )
{
	if( !sGetData[2].IsEmpty() )
	{
		sUserNameWhere = _T(" AND");
		if( !sGetData[3].IsEmpty() )
			sUserNameWhere += _T(" CLU.IDX_USER_NAME BETWEEN '") + sGetData[2] + _T("' AND '") + sGetData[3]+ _T("'");
		else
			sUserNameWhere += _T(" CLU.IDX_USER_NAME BETWEEN '") + sGetData[2] + _T("' AND '") + sGetData[2]+ _T("��'");
	}
}

// 2008.03.04 ADD BY PWR
// ** ��������������� ���� WHERE����
VOID CFeeHistorySearchDlg::MakeReceiptWhere( BOOL bCheck[], CString &sReceiptWhere )
{
	// Y, N �Ѵ� ���þ��� ( ���Ǿ��� )
	if( !bCheck[8] && !bCheck[9] )
	{
		sReceiptWhere = _T("");
	}
	// N�� ���� ( N�� �˻� )
	else if( !bCheck[8] && bCheck[9] )
	{
		sReceiptWhere = _T(" AND CFPI.RECEIPT_PRINT_YN IN('N')");
	}
	// Y�� ���� (Y�� �˻� )
	else if( bCheck[8] && !bCheck[9] )
	{
		sReceiptWhere = _T(" AND CFPI.RECEIPT_PRINT_YN IN('Y')");
	}
	// Y, N �Ѵ� ���� ( Y,N �˻� )
	else if( bCheck[8] && bCheck[9] )
	{
		sReceiptWhere = _T(" AND CFPI.RECEIPT_PRINT_YN IN('Y', 'N')");
	}
}

// 2008.04.29 ADD BY PWR
// ** ������ ���������� ���� WHERE����
VOID CFeeHistorySearchDlg::MakePaymentWhere( BOOL bCheck[], CString &sPaymentWhere )
{
	// Y, N �Ѵ� ���þ��� ( ���Ǿ��� )
	if( !bCheck[11] && !bCheck[12] )
	{
		sPaymentWhere = _T("");
	}
	// N�� ���� ( N�� �˻� )
	else if( !bCheck[11] && bCheck[12] )
	{
		sPaymentWhere = _T(" AND CFPI.STATUS IN('N')");
	}
	// Y�� ���� (Y�� �˻� )
	else if( bCheck[11] && !bCheck[12] )
	{
		sPaymentWhere = _T(" AND CFPI.STATUS IN('Y')");
	}
	// Y, N �Ѵ� ���� ( Y,N �˻� )
	else if( bCheck[11] && bCheck[12] )
	{
		sPaymentWhere = _T(" AND CFPI.STATUS IN('Y', 'N')");
	}
}

VOID CFeeHistorySearchDlg::MakeManageCodeWhere( CString &strManageCodeWhere )
{
	CString strManageCode;
	GetControlData( _T("CM_FEE_HISTORY_SEARCH"), _T("��������"), strManageCode );
	strManageCode.TrimRight();
	if( strManageCode.IsEmpty() || (strManageCode.Compare(_T("0"))==0) || (strManageCode.Compare(_T("�������"))==0) ) 
	{
		return;
	}	
	strManageCodeWhere = _T(" AND CFPI.MANAGE_CODE='") + strManageCode + _T("' ");
}

VOID CFeeHistorySearchDlg::MakeShelfLocCodeWhere( CString &strShelfLocCodeWhere )
{	
	CString strShelfLocCode;
	GetControlData( _T("CM_FEE_HISTORY_SEARCH"), _T("�ڷ��"), strShelfLocCode );
	strShelfLocCode.TrimRight();
	if( strShelfLocCode.IsEmpty() || (strShelfLocCode.Compare(_T("0"))==0) || (strShelfLocCode.Compare(_T("�������"))==0) ) 
	{
		return;
	}
	
	strShelfLocCodeWhere = _T(" AND CL.SHELF_LOC_CODE='") + strShelfLocCode + _T("' ");
}

HBRUSH CFeeHistorySearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}
