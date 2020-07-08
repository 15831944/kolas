// KL_OTHER_LOAN_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_LOAN_MAIN.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"
#include "..\\BL_LOC_LOAN\\CheckLoanBook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_MAIN dialog


CKL_OTHER_LOAN_MAIN::CKL_OTHER_LOAN_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKL_OTHER_LOAN_MAIN::IDD, pParent)
{
	m_nMode = 0;
	mCtrl_pDlg_Request = NULL;
	mCtrl_pDlg_Offer = NULL;
	m_bShow = FALSE;

	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_pKLRegnoEdit = NULL;
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	
}
CKL_OTHER_LOAN_MAIN::~CKL_OTHER_LOAN_MAIN()
{
		{
		delete mCtrl_pDlg_Offer;
		mCtrl_pDlg_Offer = NULL;
	}
	if(NULL != mCtrl_pDlg_Request)
	{
		delete mCtrl_pDlg_Request;
		mCtrl_pDlg_Request = NULL;
	}

	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (m_pKLRegnoEdit != NULL)
	{
		delete m_pKLRegnoEdit;
		m_pKLRegnoEdit = NULL;
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	
}

void CKL_OTHER_LOAN_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_LOAN_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_TAB_MAIN, mCtrl_TabMain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_LOAN_MAIN, CDialog)
	//{{AFX_MSG_MAP(CKL_OTHER_LOAN_MAIN)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnSelchangeTabMain)
	ON_BN_CLICKED(IDC_BUTTON1, OnSend)
	ON_BN_CLICKED(IDC_BUTTON2, OnGetBook)
	ON_BN_CLICKED(IDC_btnKL_ReturnCancel, OnWorkCancel)
	ON_BN_CLICKED(IDC_SendRefuse, RefuseReason)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBtnRefresh)
	ON_BN_CLICKED(IDC_BUTTON3, OnSelectGrid)
	ON_BN_CLICKED(IDC_btnKL_print_waybill, OnbtnKLprintwaybill)		
	ON_BN_CLICKED(IDC_btnKL_loan_edit_memo, OnbtnKLloaneditmemo)
	ON_BN_CLICKED(IDC_btnKL_loan_user_view, OnbtnKLloanuserview)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KL_ACTIVE_BUTTON, OnActivateButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_MAIN message handlers

BOOL CKL_OTHER_LOAN_MAIN::Create(CWnd* pParent) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParent);
}

BOOL CKL_OTHER_LOAN_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());
	// TODO: Add extra initialization here	
	mCtrl_TabMain.InsertItem(0,	_T("�����ڷ����"));
	mCtrl_TabMain.InsertItem(1,	_T("��û�ڷ����"));

 	mCtrl_pDlg_Offer = new CKL_OTHER_LOAN_OFFER(this);
 	mCtrl_pDlg_Request = new CKL_OTHER_LOAN_REQUEST(this);

	mCtrl_pDlg_Offer->m_pInfo = m_pInfo;
	mCtrl_pDlg_Request->m_pInfo = m_pInfo;

	mCtrl_pDlg_Offer->m_pLoanManageValue = m_pLoan->m_pManageValue;
	mCtrl_pDlg_Request->m_pLoanManageValue = m_pLoan->m_pManageValue;

	mCtrl_pDlg_Offer->Create(this);
	mCtrl_pDlg_Request->Create(this);

	// 16/02/01 ���湫 : 2016�� �����߾ӵ����� å��������
	// �÷� �߰��� ������ ����
	/*//BEFORE-----------------------------------------------------------------------
	SetWindowPos(NULL, 0, 0, 1070, 600, SWP_NOMOVE|SWP_NOZORDER);
	*///AFTER------------------------------------------------------------------------
	SetWindowPos(NULL, 0, 0, 1370, 600, SWP_NOMOVE|SWP_NOZORDER);
	/*//END------------------------------------------------------------------------*/
// 	ResizeControl();	
	CenterWindow();
	INT ids = CheckGetDate();
	if(0 > ids)
	{
		SetMode(1);
		mCtrl_pDlg_Request->SelectGetBook();
	}
	else
	{
		SetMode(0);
	}

	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	{
		// CM�� ���� ��Ʈ���� ���� SUBCLASSINGó��
		m_pKLRegnoEdit = new CESL_ControlEdit(NULL);
		ASSERT(m_pKLRegnoEdit);

		if (m_pKLRegnoEdit->SubclassDlgItem(IDC_edt_KL_RETURN_REG_NO, this))
		{
			m_pKLRegnoEdit->SetKLRegnoMode(TRUE);	

			m_pKLRegnoEdit->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
		}
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKL_OTHER_LOAN_MAIN::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeControl();
	
}
void CKL_OTHER_LOAN_MAIN::OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT nMode = mCtrl_TabMain.GetCurSel();
	SetMode(nMode);

	*pResult = 0;
}


BOOL CKL_OTHER_LOAN_MAIN::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}


BOOL CKL_OTHER_LOAN_MAIN::ResizeControl()
{
	CRect rDlg = CRect(0, 0, 0, 0);
	GetClientRect(rDlg);

	if(NULL == mCtrl_TabMain.GetSafeHwnd())
	{
		return FALSE;
	}
	CRect rTab;
	mCtrl_TabMain.GetWindowRect(rTab);
	ScreenToClient(rTab);
	
	rTab.right = rDlg.Width();
	mCtrl_TabMain.MoveWindow(rTab);

	CRect rSubDlg = rDlg;
	rSubDlg.top = rTab.bottom+5;
	rSubDlg.right -= 2;
// 	rSubDlg.bottom -= 20;
// 	rSubDlg.OffsetRect(2, rTab.top+20);

	if(NULL == mCtrl_pDlg_Offer || NULL == mCtrl_pDlg_Offer->GetSafeHwnd()) return FALSE;
	mCtrl_pDlg_Offer->MoveWindow(rSubDlg);

	if(NULL == mCtrl_pDlg_Request || NULL == mCtrl_pDlg_Request->GetSafeHwnd()) return FALSE;
	mCtrl_pDlg_Request->MoveWindow(rSubDlg);
	
	return TRUE;
}

// 17/02/23 ������ : Ÿ������������� ���ڵ�� �ڷ� ������ �ǵ��� ��� ����
//*/ ADD ---------------------------------------------------------------------------
BOOL CKL_OTHER_LOAN_MAIN::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			CWnd* pWnd1 = GetFocus();
			CWnd* pWnd2 = GetDlgItem(IDC_edt_KL_RETURN_REG_NO);
			if(pWnd1->GetSafeHwnd() == pWnd2->GetSafeHwnd())
			{
				OnSelectGrid();
				pWnd2->SetFocus();
				return TRUE;
			}
		}
	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}
//*/ END ---------------------------------------------------------------------------

BOOL CKL_OTHER_LOAN_MAIN::SetMode(int nMode)
{
	mCtrl_TabMain.SetCurSel(nMode);
	ShowDlg(nMode);
	m_nMode = nMode;
	ChangeToolBarButton(nMode);

	// 16/02/11 ���湫 : 2016�� �����߾ӵ����� å��������
	// ������ư ��Ȱ��ȭ
	/*//NEW------------------------------------------------------------------------*/
	if(m_nMode == 1)
	{
		GetDlgItem(IDC_btnKL_print_waybill)->EnableWindow(FALSE);
	}
	else
	{
		CString strState = mCtrl_pDlg_Offer->GetComboBoxState();
		if(strState.Compare(_T("1"))==0)
		{
			GetDlgItem(IDC_btnKL_print_waybill)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_btnKL_print_waybill)->EnableWindow(FALSE);
		}
	}
	/*//END------------------------------------------------------------------------*/	

	return 0;
}


INT CKL_OTHER_LOAN_MAIN::ShowDlg(INT nIdx)
{
	CString strState;
	BOOL bShow = TRUE;
	if(0 == nIdx)
	{	
		mCtrl_pDlg_Offer->ShowWindow(SW_SHOW);
		mCtrl_pDlg_Request->ShowWindow(SW_HIDE);
		strState = mCtrl_pDlg_Request->GetComboBoxState();
		if(_T("0") == strState || _T("1") == strState) bShow = TRUE;
	}
	else if(1 == nIdx)
	{		
		mCtrl_pDlg_Offer->ShowWindow(SW_HIDE);
		mCtrl_pDlg_Request->ShowWindow(SW_SHOW);
		strState = mCtrl_pDlg_Offer->GetComboBoxState();
		if(_T("2") == strState) bShow = TRUE;
	}
	else
		return -1;

	GetDlgItem(IDC_btnKL_ReturnCancel)->EnableWindow(bShow);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	return 0;
}

int CKL_OTHER_LOAN_MAIN::GetMode()
{
	return m_nMode;
}

INT CKL_OTHER_LOAN_MAIN::ChangeToolBarButton(INT nIdx)
{
	if(0==m_nMode)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_SendRefuse)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		GetDlgItem(IDC_SendRefuse)->EnableWindow(FALSE);
	}

	return 0;
}
void CKL_OTHER_LOAN_MAIN::OnBtnRefresh()
{
	if(0==m_nMode)
	{
		mCtrl_pDlg_Offer->Refresh();
	}
	else
		mCtrl_pDlg_Request->Refresh();

}
void CKL_OTHER_LOAN_MAIN::OnSelectGrid() 
{
	// TODO: Add your control notification handler code here
	CString sRegNo;
	GetDlgItemText(IDC_edt_KL_RETURN_REG_NO, sRegNo);
	
	sRegNo.TrimLeft();
	sRegNo.TrimRight();
	if(sRegNo.IsEmpty())
	{
		return;
	}

	CLocCommonAPI::MakeRegNoSpaceToZero(sRegNo);
	INT ids = 0;

	if(0 == GetMode())
	{
		ids = mCtrl_pDlg_Offer->SelectGrid(sRegNo);
	}
	else if(1 == GetMode())
	{
		ids = mCtrl_pDlg_Request->SelectGrid(sRegNo);
	}

	if(0 < ids)
	{
		CString strMessage;
		strMessage.Format(_T("��Ϲ�ȣ�� [%s]�� �ڷᰡ �������� �ʽ��ϴ�."), sRegNo);
		AfxMessageBox(strMessage);
	}
	SetDlgItemText(IDC_edt_KL_RETURN_REG_NO, _T(""));
}



void CKL_OTHER_LOAN_MAIN::OnSend() 
{
	if(0==m_nMode)
	{
		mCtrl_pDlg_Offer->SendBook();	
	}
	else
		mCtrl_pDlg_Request->ReturnBook();
}

void CKL_OTHER_LOAN_MAIN::OnGetBook() 
{
	// 15/02/16 ���湫 : 2016�� �����߾ӵ����� å��������
	// �Լ��� ��ȣ���� �̴��⸸���� ����
	/*//BEFORE-----------------------------------------------------------------------
	mCtrl_pDlg_Request->GetBook();	
	*///AFTER------------------------------------------------------------------------
	mCtrl_pDlg_Request->GetBook(m_pLoan);
	/*//END------------------------------------------------------------------------*/
}
void CKL_OTHER_LOAN_MAIN::OnWorkCancel() 
{
	if(0==m_nMode)
	{
		mCtrl_pDlg_Offer->OfferCancel();	
	}
	else
		mCtrl_pDlg_Request->RequestCancel();	
}
void CKL_OTHER_LOAN_MAIN::RefuseReason() 
{
	if(0==m_nMode)	mCtrl_pDlg_Offer->SendRefuse();
}

INT CKL_OTHER_LOAN_MAIN::CheckGetDate()
{
	CString strLoanExpiry = _T("");
	// 17/05/08 ������ : cfg ������ �Ľ��Ͽ� �����ö� �ӵ� ������ �޸𸮷� ��ü����
/*//BEFORE-----------------------------------------------------------------------------
	CString strCfgFilePath = _T("..\\cfg\\KL_User.cfg");
	m_pLoan->m_CfgFileApi.SetFile(strCfgFilePath);
	m_pLoan->m_CfgFileApi.GetData(_T("������ȣ�������⸸����"), strLoanExpiry);
*///AFTER------------------------------------------------------------------------------
	m_pLoan->GetTempData(_T("������ȣ�������⸸����"), strLoanExpiry);
/*//END-------------------------------------------------------------------------------*/
	if(strLoanExpiry.IsEmpty())
	{
		strLoanExpiry = _T("7");
	}

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("TRANSACTION_WORK"), _T("SEARCH"));
	Writer.addElement(_T("SEARCH_OPTION"), _T("REQUEST"));
	Writer.addElement(_T("TRANSACTION_CODE"), _T("3"));
	Writer.addElement(_T("LIB_CODE"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("PAGE_NO"), _T("1"));
	Writer.addElement(_T("DISPLAY"), _T("1000"));

	CFrameSender Sender(m_pInfo);	
	CFrameReader Reader; 
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if (0 > ids) 
	{
		while(true) 
		{
			// 2015�� ���������� �ý��� ���� 2015/12/11 : ���յ������� > å��������
			if(IDYES == MessageBox(_T("å�������� �̵������� ��ſ� �����Ͽ����ϴ�.\r\n��õ� �Ͻðڽ��ϱ�?"), _T("��ȸ����"), MB_YESNO | MB_ICONQUESTION)) 
			{
				ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
				if(0 <= ids) break;
			}
			else
			{
				return 0;
			}
		}
	}
	
	for(int i=1;i<Reader.getRecordCount();i++)
	{		
		CTime tCurDate = CTime::GetCurrentTime();	
		CString strGetDate = Reader.getElement(i,_T("RECEIVE_DATE"));

		// 15/02/16 ���湫 : 2016�� �����߾ӵ����� å��������
		// �Լ��� ��ȣ���� �̴��⸸���� ����
		/*//BEFORE--------------------------
		CTime tRenewalTime(_ttoi(strGetDate.Left(4)),_ttoi(strGetDate.Mid(5,2)),_ttoi(strGetDate.Mid(8,2)),0,0,0);
		double nElapseDay = tCurDate.GetTime() - tRenewalTime.GetTime();
		nElapseDay = nElapseDay / (24 * 60 * 60);
	
		//���⸸������ ���� �ڷῡ ���� �ȳ� ó��
		if(nElapseDay > _ttoi(strLoanExpiry))
		{	
			AfxMessageBox(_T("���⸸������ ���� �ڷᰡ �ֽ��ϴ�.\r\n�߼� ó�����ֽñ� �ٶ��ϴ�."));
			return -1;
		}
		*///AFTER------------------------------------------------------------------------
		CString strExpireDate = Reader.getElement(i,_T("RETURN_EXPIRE_DATE"));

		if(!strExpireDate.IsEmpty())			
		{
			CTime tRenewalTime(_ttoi(strExpireDate.Left(4)),_ttoi(strExpireDate.Mid(5,2)),_ttoi(strExpireDate.Mid(8,2)),0,0,0);
						
			double nElapseDay = tCurDate.GetTime() - tRenewalTime.GetTime();
			nElapseDay = nElapseDay / (24 * 60 * 60);
			
			//���⸸������ ���� �ڷῡ ���� �ȳ� ó��			
			if(nElapseDay > 1 )
			{	
				AfxMessageBox(_T("���⸸������ ���� �ڷᰡ �ֽ��ϴ�.\r\n�߼� ó�����ֽñ� �ٶ��ϴ�."));
				return -1;
			}			
		}
		/*//END------------------------------------------------------------------------*/
	}
	return 0;
}

// 16/02/03 ���湫 : 2016�� �����߾ӵ����� å��������
// ��ư Ȱ��ȭ �߰�
/*//NEW------------------------------------------------------------------------*/
LRESULT CKL_OTHER_LOAN_MAIN::OnActivateButton(WPARAM wParam, LPARAM lParam)
{
	int nState = (int)lParam;

	GetDlgItem(IDC_btnKL_print_waybill)->EnableWindow(FALSE);
	
	if(nState == 1)
	{
		GetDlgItem(IDC_btnKL_print_waybill)->EnableWindow(TRUE);
	}

	return 0;
}
/*//END------------------------------------------------------------------------*/

// 16/02/19 ���湫 : 2016�� �����߾ӵ����� å��������
// ��������
/*//NEW------------------------------------------------------------------------*/
void CKL_OTHER_LOAN_MAIN::OnbtnKLprintwaybill() 
{
	// TODO: Add your control notification handler code here
	if(mCtrl_pDlg_Offer)
		mCtrl_pDlg_Offer->PrintWaybill();
	
}
/*//END------------------------------------------------------------------------*/

// 16/02/19 ���湫 : 2016�� �����߾ӵ����� å��������
// �޸����
/*//NEW------------------------------------------------------------------------*/
void CKL_OTHER_LOAN_MAIN::OnbtnKLloaneditmemo() 
{
	// TODO: Add your control notification handler code here	
	if(0==m_nMode)
	{
		mCtrl_pDlg_Offer->ModifyMemo();	
	}
	else
	{
		mCtrl_pDlg_Request->ModifyMemo();
	}
}
/*//END------------------------------------------------------------------------*/

// 16/02/19 ���湫 : 2016�� �����߾ӵ����� å��������
// �̿��ڻ󼼺���
/*//NEW------------------------------------------------------------------------*/
void CKL_OTHER_LOAN_MAIN::OnbtnKLloanuserview() 
{
	// TODO: Add your control notification handler code here

	CString m_strUserAccessAuth = m_pLoan->m_pManageValue->m_sLoanUserAccessMode;
	if( m_strUserAccessAuth.IsEmpty() || m_strUserAccessAuth == _T("1") )
	{
		AfxMessageBox(_T("�̿��� ���� ���ٱ����� �����ϴ�."));
		return;
	}

	if(0==m_nMode)
	{
		mCtrl_pDlg_Offer->ViewLoanUser();	
	}
	else
	{
		mCtrl_pDlg_Request->ViewLoanUser();
	}	
}
/*//END------------------------------------------------------------------------*/
