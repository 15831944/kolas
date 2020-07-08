// FeeEditArrear.cpp : implementation file
// MAKE BY PWR

#include "stdafx.h"
#include "FeeEditArrear.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeEditArrear dialog


CFeeEditArrear::CFeeEditArrear(CESL_Mgr* pParent)
	: CESL_Mgr(CFeeEditArrear::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeEditArrear)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bSuccess = FALSE;
	// KOL.UDF.022
	m_nIndex = 0;
	m_strEdtFee = _T("");

}
CFeeEditArrear::~CFeeEditArrear()
{
}

void CFeeEditArrear::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeEditArrear)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_EDT_ARREAR_PAYMENT, m_edtFee);
	DDX_Text(pDX, IDC_EDT_ARREAR_PAYMENT, m_strEdtFee);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeEditArrear, CDialog)
	//{{AFX_MSG_MAP(CFeeEditArrear)
	ON_EN_CHANGE(IDC_EDT_ARREAR_PAYMENT, OnChangeEdtArrearPayment)
	ON_BN_CLICKED(IDC_BTN_ARREAR_EDIT, OnBtnArrearEdit)
	ON_BN_CLICKED(IDC_BTN_ARREAR_CANCEL, OnBtnArrearCancel)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeEditArrear message handlers

BOOL CFeeEditArrear::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFeeEditArrear::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitControl();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

// **��Ʈ���� �ʱ�ȭ
VOID CFeeEditArrear::InitControl()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeEditArrear::InitControl()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return;
	}

	CString sPayment;
	pDM->GetCellData( _T("���αݾ�"), m_nIndex, sPayment );
	sPayment.Replace( _T(","), _T("") );
	((CEdit*)GetDlgItem(IDC_EDT_ARREAR_PAYMENT))->SetWindowText( sPayment );

	((CEdit*)GetDlgItem( IDC_EDT_ARREAR_PAYMENT ))->SetFocus();
	//���αݾ׿� �����
	INT nLen = m_strEdtFee.GetLength();
	m_edtFee.SetSel(0, nLen);

}

// **õ������ǥ��
VOID CFeeEditArrear::OnChangeEdtArrearPayment() 
{
	UpdateData(TRUE);

	m_strEdtFee.Replace( _T(","), _T("") );

	CString strBuf = m_strEdtFee;
	strBuf.Replace( _T(","), _T("") );

	INT nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize-1);

	m_strEdtFee = pszFormattedNumber;

	if (pszFormattedNumber)
	delete [] pszFormattedNumber;

	UpdateData(FALSE);

	INT nLen = m_strEdtFee.GetLength();
	m_edtFee.SetSel(nLen, -1);
}

// **����
VOID CFeeEditArrear::OnBtnArrearEdit() 
{
	INT ids;
	m_bSuccess = FALSE;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeEditReCard::InitControl()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return;
	}

	// �ڷ����
	CString sPaymentKey, sLoanKey, sPayment, sMoneyPaid, sEditor, sEditContent;
	pDM->GetCellData( _T("PAYMENT_KEY"), m_nIndex, sPaymentKey );
	pDM->GetCellData( _T("LOAN_INFO_KEY"), m_nIndex, sLoanKey );
	pDM->GetCellData( _T("���αݾ�"), m_nIndex, sPayment );
	((CEdit*)GetDlgItem(IDC_EDT_ARREAR_PAYMENT))->GetWindowText( sMoneyPaid );
	sEditor.Format( _T("%s::%s"), m_pInfo->USER_ID, m_pInfo->LOCAL_IP );
	sEditContent.Format( _T("���αݾ�:%s��->%s��")
						, sPayment, sMoneyPaid );

	sPayment.Replace( _T(","), _T("") );
	sMoneyPaid.Replace( _T(","), _T("") );

	CString sLastWork = GetWorkLogMsg( _T("��ü�����"), __WFILE__, __LINE__ );
	// ��������
	CString sQuery;
	sQuery.Format(	_T("UPDATE CO_FEE_PAYMENT_INFO_TBL ")
					_T("SET MONEY_PAID=%s, EDIT_DATE=SYSDATE, ")
					_T("EDITOR='%s', EDIT_CONTENT='%s', LAST_WORK='%s' ")
					_T("WHERE REC_KEY=%s;")
					, sMoneyPaid, sEditor, sEditContent, sLastWork, sPaymentKey );

	pDM->StartFrame();
	pDM->InitDBFieldData();	
	pDM->AddFrame(sQuery);

	sQuery.Format(	_T("UPDATE LS_WORK_T01 ")
					_T("SET ARREAR='%s', LAST_WORK='%s' WHERE REC_KEY=%s;")
					, sMoneyPaid, sLastWork, sLoanKey );

	pDM->AddFrame(sQuery);
	ids = pDM->SendFrame(TRUE);	
	pDM->EndFrame();
	if( ids < 0 )
	{
		CString sErrorMsg;
		sErrorMsg = _T("SendFrame Error::��ü�����");
		if( ids == -4004 )
			sErrorMsg = _T("SendFrame Error::��ü�����\r\n-���ӵ�������");
		else if( ids == -4005 )
			sErrorMsg = _T("SendFrame Error::��ü�����\r\n-��������� �����߻�");
		else if( ids == -4999 )
			sErrorMsg = _T("SendFrame Error::��ü�����\r\n-���� Oracle �� �������� ����");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );

		// UPDATE ����
		m_bSuccess = FALSE;
		return;
	}

	// UPDATE ����
	m_bSuccess = TRUE;
	OnCancel();
}

VOID CFeeEditArrear::OnBtnArrearCancel() 
{
	OnCancel();	
}

BOOL CFeeEditArrear::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			HWND MsgH = pMsg->hwnd;
			if( GetDlgItem(IDC_EDT_ARREAR_PAYMENT)->m_hWnd == MsgH )
			{
				GetDlgItem(IDC_BTN_ARREAR_EDIT)->SetFocus();
			}
			else if( GetDlgItem(IDC_BTN_ARREAR_EDIT)->m_hWnd == MsgH )
			{
				OnBtnArrearEdit();
			}
			else if( GetDlgItem(IDC_BTN_ARREAR_CANCEL)->m_hWnd == MsgH )
			{
				OnCancel();
			}
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CFeeEditArrear::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
