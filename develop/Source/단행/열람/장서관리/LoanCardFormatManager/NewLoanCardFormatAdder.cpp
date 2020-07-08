// NewLoanCardFormatAdder.cpp : implementation file
//

#include "stdafx.h"
#include "NewLoanCardFormatAdder.h"
#include "LoanCardFormatSetter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewLoanCardFormatAdder dialog


CNewLoanCardFormatAdder::CNewLoanCardFormatAdder(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CNewLoanCardFormatAdder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewLoanCardFormatAdder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// 2009.04.21 ADD BY CJY : ��ġ����Ʈ ��Ʈ�� �ʱ�ȭ
	AfxInitRichEdit();

	m_pParent = pParent;
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CNewLoanCardFormatAdder::~CNewLoanCardFormatAdder()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CNewLoanCardFormatAdder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewLoanCardFormatAdder)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewLoanCardFormatAdder, CDialog)
	//{{AFX_MSG_MAP(CNewLoanCardFormatAdder)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewLoanCardFormatAdder message handlers

VOID CNewLoanCardFormatAdder::OnOK() 
{
	// TODO: Add extra validation here
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cPTRN_LIST );
	if( pCombo->GetCurSel() )
		pCombo->GetWindowText( m_sCopyFromPtrn );
	else
		m_sCopyFromPtrn.Empty();
	
	GetDlgItemText( IDC_ePTRN_NAME, m_sPatternName );
	m_sPatternName.TrimLeft();
	m_sPatternName.TrimRight();
	if( m_sPatternName.IsEmpty() )
	{
		AfxMessageBox( _T("����̸��� �־��ֽʽÿ�") );
		return;
	}

	CDialog::OnOK();
}

BOOL CNewLoanCardFormatAdder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	
	MakePtrnList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CNewLoanCardFormatAdder::MakePtrnList()
{
	CStringList ptrnList;
	CLoanCardFormatSetter *pParent = ( CLoanCardFormatSetter * )m_pParent;
	pParent->GetPtrnList( ptrnList );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cPTRN_LIST );
	pCombo->InsertString( 0, _T("�������� ����") );
	for( INT i = 0; i < ptrnList.GetCount(); i++ )
		pCombo->InsertString( i + 1, ptrnList.GetAt( ptrnList.FindIndex( i ) ) );
	pCombo->SetCurSel( 0 );
}

HBRUSH CNewLoanCardFormatAdder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
