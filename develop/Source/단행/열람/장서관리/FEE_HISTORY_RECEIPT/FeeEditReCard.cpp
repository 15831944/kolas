// FeeEditReCard.cpp : implementation file
// MAKE BY PWR

#include "stdafx.h"
#include "FeeEditReCard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeEditReCard dialog


CFeeEditReCard::CFeeEditReCard(CESL_Mgr* pParent)
	: CESL_Mgr(CFeeEditReCard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeEditReCard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sCard = _T("");
	m_sClass = _T("");
	m_bSuccess = FALSE;

	// KOL.UDF.022
	m_nIndex = 0;
	m_strEdtFee = _T("");
}

CFeeEditReCard::~CFeeEditReCard()
{
}

void CFeeEditReCard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeEditReCard)
	DDX_Control(pDX, IDC_EDT_RECARD_PAYMENT, m_edtFee);
	DDX_Text(pDX, IDC_EDT_RECARD_PAYMENT, m_strEdtFee);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeEditReCard, CDialog)
	//{{AFX_MSG_MAP(CFeeEditReCard)
	ON_BN_CLICKED(IDC_RAD_RECARD_CLASS1, OnRadRecardClass1)
	ON_BN_CLICKED(IDC_RAD_RECARD_CLASS2, OnRadRecardClass2)
	ON_BN_CLICKED(IDC_RAD_RECARD_CLASS3, OnRadRecardClass3)
	ON_BN_CLICKED(IDC_RAD_RECARD_CARD1, OnRadRecardCard1)
	ON_BN_CLICKED(IDC_RAD_RECARD_CARD2, OnRadRecardCard2)
	ON_BN_CLICKED(IDC_RAD_RECARD_CARD3, OnRadRecardCard3)
	ON_BN_CLICKED(IDC_BTN_RECARD_EDIT, OnBtnRecardEdit)
	ON_BN_CLICKED(IDC_BTN_RECARD_CANCEL, OnBtnRecardCancel)
	ON_EN_CHANGE(IDC_EDT_RECARD_PAYMENT, OnChangeEdtRecardPayment)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeEditReCard message handlers

BOOL CFeeEditReCard::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFeeEditReCard::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitControl();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

// **��Ʈ���� �ʱ�ȭ
VOID CFeeEditReCard::InitControl()
{
	CString sPayment, sCardClass;

	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeEditReCard::InitControl()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return;
	}

	pDM->GetCellData( _T("���αݾ�"), m_nIndex, sPayment );
	pDM->GetCellData( _T("ȸ��������"), m_nIndex, sCardClass );

	sPayment.Replace( _T(","), _T("") );
	if( sCardClass == _T("����ȸ��ī��") || sCardClass == _T("�Ϲ�ī����") )
	{
		// ȸ�����
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->SetCheck(FALSE);
		m_sClass = _T("����");
		// ȸ��������
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD3))->SetCheck(FALSE);
		m_sCard = _T("�Ϲ�ī����");
		// ���αݾ�
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("0") );
	}
	else if( sCardClass == _T("�Ϲ��ö�ƽȸ��ī��") || sCardClass == _T("�ö�ƽī����") )
	{
		// ȸ�����
		if( sPayment == _T("1000") )
		{//����
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->SetCheck(FALSE);
			m_sClass = _T("����");
			// ���αݾ�
			((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("1000") );
		}
		else if( sPayment == _T("800") )
		{//��,����л�
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->SetCheck(FALSE);
			m_sClass = _T("��,����л�");
			// ���αݾ�
			((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("800") );
		}
		else if( sPayment == _T("500") )
		{//�ʵ��л�����
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->SetCheck(TRUE);
			m_sClass = _T("�ʵ��л�����");
			// ���αݾ�
			((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("500") );
		}
		// ȸ��������
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD2))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD3))->SetCheck(FALSE);
		m_sCard = _T("�ö�ƽī����");
	}
	else if( sCardClass == _T("RFIDȸ��ī��") || sCardClass == _T("RFī����") )
	{
		// ȸ�����
		if( sPayment == _T("2000") )
		{//����
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->SetCheck(FALSE);
			m_sClass = _T("����");
			// ���αݾ�
			((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("2000") );
		}
		else if( sPayment == _T("1500") )
		{//��,����л�
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->SetCheck(FALSE);
			m_sClass = _T("��,����л�");
			// ���αݾ�
			((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("1500") );
		}
		else if( sPayment == _T("1000") )
		{//�ʵ��л�����
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->SetCheck(TRUE);
			m_sClass = _T("�ʵ��л�����");
			// ���αݾ�
			((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("1000") );
		}
		// ȸ��������
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD3))->SetCheck(TRUE);
		m_sCard = _T("RFī����");
	}
}

// **����
void CFeeEditReCard::OnRadRecardClass1() 
{
	m_sClass = _T("����");

	((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->SetCheck(FALSE);
	
	if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD1))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("0") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD2))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("1000") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD3))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("2000") );
	}
}

// **��,����л�
void CFeeEditReCard::OnRadRecardClass2() 
{
	m_sClass = _T("��,����л�");

	((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->SetCheck(FALSE);

	if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD1))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("0") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD2))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("800") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD3))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("1500") );
	}
}

// **�ʵ��л�����
void CFeeEditReCard::OnRadRecardClass3() 
{
	m_sClass = _T("�ʵ��л�����");

	((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->SetCheck(TRUE);

	if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD1))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("0") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD2))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("500") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD3))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("1000") );
	}
}

// **�Ϲ�ī����
void CFeeEditReCard::OnRadRecardCard1() 
{
	m_sCard = _T("�Ϲ�ī����");

	((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD3))->SetCheck(FALSE);

	((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("0") );
}

// **�ö�ƽī����
void CFeeEditReCard::OnRadRecardCard2() 
{
	m_sCard = _T("�ö�ƽī����");

	((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD3))->SetCheck(FALSE);

	if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("1000") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("800") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("500") );
	}
}

// **RFī����
void CFeeEditReCard::OnRadRecardCard3() 
{
	m_sCard = _T("RFī����");

	((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RAD_RECARD_CARD3))->SetCheck(TRUE);

	if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS1))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("2000") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS2))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("1500") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_RAD_RECARD_CLASS3))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->SetWindowText( _T("1000") );
	}
}

// **����
void CFeeEditReCard::OnBtnRecardEdit() 
{

	m_bSuccess = FALSE;

	INT ids;

	CString sPaymentKey, sPayment, sCardClass;

	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeEditReCard::InitControl()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return;
	}

	// �ڷ����
	pDM->GetCellData( _T("PAYMENT_KEY"), m_nIndex, sPaymentKey );
	pDM->GetCellData( _T("���αݾ�"), m_nIndex, sPayment );
	pDM->GetCellData( _T("ȸ��������"), m_nIndex, sCardClass );

	CString sMoneyPaid, sEditor, sEditContent;
	((CEdit*)GetDlgItem(IDC_EDT_RECARD_PAYMENT))->GetWindowText( sMoneyPaid );
	sEditor.Format( _T("%s::%s"), m_pInfo->USER_ID, m_pInfo->LOCAL_IP );
	sEditContent.Format( _T("���αݾ�:%s��->%s��, ȸ��������:%s->%s")
						, sPayment, sMoneyPaid, sCardClass, m_sCard );

	sPayment.Replace( _T(","), _T("") );
	sMoneyPaid.Replace( _T(","), _T("") );

	CString sLastWork = GetWorkLogMsg( _T("ȸ������߱޼��������"), __WFILE__, __LINE__ );

	// ��������
	CString sQuery;
	sQuery.Format(	_T("UPDATE CO_FEE_PAYMENT_INFO_TBL ")
					_T("SET MONEY_PAID=%s, CARD_CLASS='%s', EDIT_DATE=SYSDATE, ")
					_T("EDITOR='%s', EDIT_CONTENT='%s', LAST_WORK='%s' ")
					_T("WHERE REC_KEY=%s;")
					, sMoneyPaid, m_sCard, sEditor, sEditContent, sLastWork, sPaymentKey );

	pDM->StartFrame();
	pDM->InitDBFieldData();	
	pDM->AddFrame(sQuery);
	ids = pDM->SendFrame(TRUE);	
	pDM->EndFrame();
	if( ids < 0 )
	{
		CString sErrorMsg;
		sErrorMsg = _T("SendFrame Error::ȸ������߱޼���");
		if( ids == -4004 )
			sErrorMsg = _T("SendFrame Error::ȸ������߱޼���\r\n-���ӵ�������");
		else if( ids == -4005 )
			sErrorMsg = _T("SendFrame Error::ȸ������߱޼���\r\n-��������� �����߻�");
		else if( ids == -4999 )
			sErrorMsg = _T("SendFrame Error::ȸ������߱޼���\r\n-���� Oracle �� �������� ����");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );
		// UPDATE ����
		m_bSuccess = FALSE;
		return;
	}
	// UPDATE ����
	m_bSuccess = TRUE;
	OnCancel();
}

// **���
void CFeeEditReCard::OnBtnRecardCancel() 
{
	OnCancel();
}

// **õ������ǥ��
void CFeeEditReCard::OnChangeEdtRecardPayment() 
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

HBRUSH CFeeEditReCard::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
