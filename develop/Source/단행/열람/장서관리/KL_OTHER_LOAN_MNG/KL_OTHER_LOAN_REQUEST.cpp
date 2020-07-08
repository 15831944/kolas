// KL_OTHER_LOAN_REQUEST.cpp : implementation file
//

#include "stdafx.h"
#include "FrameManager.h"
#include "KL_OTHER_LOAN_REQUEST.h"

// 16/02/03 ���湫 : 2016�� �����߾ӵ����� å��������
// MainHeader �߰�
/*//NEW------------------------------------------------------------------------*/
#include "KL_OTHER_LOAN_EDIT_MEMO.h"
#include "KL_OTHER_LOAN_USER_DETAILVIEW.h"

#include "..\\..\\..\\..\\SMS\\CO_SMS\\SMS_MakeMsg.h"
#include "..\\..\\..\\..\\SMS\\CO_SMS\\SMS_Env_Info.h"
/*//END------------------------------------------------------------------------*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CString g_strKakaoUsed2;
// KOL.RED.2018.224 ---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_REQUEST dialog


CKL_OTHER_LOAN_REQUEST::CKL_OTHER_LOAN_REQUEST(CESL_Mgr* pParent /*=NULL*/)
	: CKL_OTHER_LOAN_DEFAULT(pParent)
{
	m_strSMAlias = _T("SM_KL_OTHER_LOAN_MNG_REQUEST");
	m_strCMAlias = _T("CM_KL_OTHER_LOAN_MNG_REQUEST");
	m_strDMAlias = _T("DM_KL_OTHER_LOAN_MNG_REQUEST");

	m_pParent = pParent;
	//{{AFX_DATA_INIT(CKL_OTHER_LOAN_REQUEST)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CKL_OTHER_LOAN_REQUEST::~CKL_OTHER_LOAN_REQUEST()
{
}

void CKL_OTHER_LOAN_REQUEST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_LOAN_REQUEST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_LOAN_REQUEST, CKL_OTHER_LOAN_DEFAULT)
	//{{AFX_MSG_MAP(CKL_OTHER_LOAN_REQUEST)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_cmb, OnSelchangecmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_REQUEST message handlers

BOOL CKL_OTHER_LOAN_REQUEST::Create(CWnd* pWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pWnd);
}

BOOL CKL_OTHER_LOAN_REQUEST::OnInitDialog() 
{
	CKL_OTHER_LOAN_DEFAULT::OnInitDialog();
		
	ResetComboBox();
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)FindControl(m_strCMAlias, _T("��Ƽ�޺�_�������"));
	if(NULL == pCMB) return FALSE;	
	
	CString strData = _T("");
	int ROW = pCMB->GetCount();
	for(int nCnt = 0; nCnt < ROW; nCnt++)
	{
		strData = pCMB->GetItemDataString(nCnt, 1);
		if(_T("�߼�") == strData)
		{
			pCMB->SetCurSel(nCnt);
			break;
		}
	}

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	//-------------------------------------------------------------------------
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("Ÿ�����������û");
	}

	if (m_pLoanManageValue->m_sLoanUserAccessMode.IsEmpty() || m_pLoanManageValue->m_sLoanUserAccessMode == _T("1") )
	{
		m_pGrid->SetColumnEncryption(_T("�̿��ڸ�"),TRUE);
		m_pGrid->SetColumnEncryption(_T("�������"),TRUE);
	}
	//-------------------------------------------------------------------------

	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKL_OTHER_LOAN_REQUEST::ResizeControl()
{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_grid);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return FALSE;
	
	CRect rDlg = CRect(0, 0, 0, 0);
	GetClientRect(rDlg);

	// 16/02/01 ���湫 : 2016�� �����߾ӵ����� å��������
	// ������¿� ���� ���� �߰��� ���� ��ġ �̵�
	/*//BEFORE-----------------------------------------------------------------------
	rDlg.top += 40;
	*///AFTER------------------------------------------------------------------------	
	rDlg.top += 70;
	/*//END------------------------------------------------------------------------*/	
	pWnd->MoveWindow(rDlg);

	return TRUE;
}


// �޺��ڽ� �ʱ�ȭ
INT CKL_OTHER_LOAN_REQUEST::ResetComboBox()
 {
	int ids = -1;

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)FindControl(m_strCMAlias, _T("��Ƽ�޺�_�������"));
	if(!pCMB)
	{
		return -1000;
	}

	const int COL_COUNT = 2;
	pCMB->ResetContent();
	pCMB->FreeComboData();
	pCMB->InitMultiComboBox(COL_COUNT);	

	// 16/01/29 ���湫 : 2016�� �����߾ӵ����� å��������
	// ��������߰�
	/*//BEFORE-----------------------------------------------------------------------
	const int STATE_COUNT = 4;
	CString strCmbData[STATE_COUNT][COL_COUNT] = 
	{		
		{ _T("0"), _T("��û")},
		{ _T("1"), _T("�߼�")},
		{ _T("2"), _T("�߼۰���")},
		{ _T("3"), _T("�Լ�")}
	};
	*///AFTER------------------------------------------------------------------------
	const int STATE_COUNT = 5;
	CString strCmbData[STATE_COUNT][COL_COUNT] = 
	{		
		{ _T("0"), _T("��û")},
		{ _T("1"), _T("�߼�")},
		{ _T("2"), _T("�߼۰���")},
		{ _T("3"), _T("�Լ�")},
		{ _T("4"), _T("Ÿ������")}
	};
	/*//END------------------------------------------------------------------------*/

	// 16/02/02 ���湫 : 2016�� �����߾ӵ����� å��������
	// ���� �߰�
	/*//NEW------------------------------------------------------------------------*/
	m_strDescription[0] = _T("Ÿ�� å�� �ڰ����� �����ϱ� ���� �̿��ڰ� ��û�� ���");
	m_strDescription[1] = _T("Ÿ�� å�� ������ ���� �ڰ��� �߼۵� ���");
	m_strDescription[2] = _T("�ڰ����� ������ å�� ���嵵�������� ��� ������ ���� �߼۰����� ���");
	m_strDescription[3] = _T("���嵵�������� �߼��� å�� �ڰ��� �Լ��� ���");
	m_strDescription[4] = _T("Ÿ�� å�� �ڰ����� �������� ���");
	/*//END------------------------------------------------------------------------*/

	for(int nRow = 0; nRow < STATE_COUNT; nRow++)
	{
		for(int nCol = 0; nCol < 2; nCol++)
		{
			pCMB->AddItem(strCmbData[nRow][nCol], nCol, nRow);
		}
	}

	return 0;
}

void CKL_OTHER_LOAN_REQUEST::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	ResizeControl();
	// TODO: Add your message handler code here
	
}

void CKL_OTHER_LOAN_REQUEST::OnSelchangecmb() 
{
	// TODO: Add your control notification handler code here
	Refresh();
}


INT CKL_OTHER_LOAN_REQUEST::Refresh()
{
	CString strState = GetComboBoxState();

	// 16/02/01 ���湫 : 2016�� �����߾ӵ����� å��������
	// �̿��ڸ� ǥ��
	/*//NEW------------------------------------------------------------------------*/
	CString strUserNo = _T("");
	/*//END------------------------------------------------------------------------*/
	// 16/02/02 ���湫 : 2016�� �����߾ӵ����� å��������
	// ���� �߰�
	/*//NEW------------------------------------------------------------------------*/
	int nState = _wtoi(strState);
	if(nState < 0)
	{
		nState = 0;		
	}
	SetDlgItemText(IDC_stext_state_description,m_strDescription[nState]);	
	/*//END------------------------------------------------------------------------*/

	BOOL bShow = FALSE;
	if(_T("0") == strState) bShow = TRUE;
	m_pParent->SendMessage(WM_KL_RETURN_CANCEL_CTL, NULL, (LPARAM)bShow);

	// 16/02/03 ���湫 : 2016�� �����߾ӵ����� å��������
	// ���¿� ���� ��ư Ȱ��ȭ
	/*//NEW------------------------------------------------------------------------*/
	m_pParent->SendMessage(WM_KL_ACTIVE_BUTTON, NULL, (LPARAM)nState+10);
	/*//END------------------------------------------------------------------------*/
	
	SetDateControlView();

	if(m_bIsSearched)
	{
		return 1000;
	}
		
	CString strDate1,strDate2;

	strDate1 = GetBeforeDate();
	strDate2 = GetAfterDate();


	m_bIsSearched = TRUE;

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("TRANSACTION_WORK"), _T("SEARCH"));
	Writer.addElement(_T("SEARCH_OPTION"), _T("REQUEST"));
	Writer.addElement(_T("TRANSACTION_CODE"), strState);
	Writer.addElement(_T("LIB_CODE"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("PAGE_NO"), _T("1"));
	Writer.addElement(_T("DISPLAY"), _T("1000"));

	//2018-11-14 EDIT BY KYJ
	//JOB.2018.0082 ��û���µ� ���ڷ� ���� �߰� �� �� �ֵ���
	//BEFORE================================================
	//if(_T("3") == strState)
	//AFTER=================================================
	if(_T("0") == strState || _T("3") == strState)
	{
		Writer.addElement(_T("START_DATE"), strDate1);
		Writer.addElement(_T("END_DATE"), strDate2);
	}

	CFrameSender Sender(m_pInfo);	
	CFrameReader Reader; 
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if (0 > ids) 
	{
		while(true) 
		{
			if(IDYES == MessageBox(_T("���յ������� �̵������� ��ſ� �����Ͽ����ϴ�.\r\n��õ� �Ͻðڽ��ϱ�?"), _T("��ȸ����"), MB_YESNO | MB_ICONQUESTION)) 
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

	m_pDM->FreeData();
	
	Reader.parseFrame(Reader.getFrame());
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	for(int i = 1; i < Reader.getRecordCount(); i++)
	{
		m_pDM->InsertRow(-1);
		for (int j = 1; j < m_pDM->GetColCount(); j++)
		{
			m_pDM->GetColumnRefInfo(j, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			// 17/11/06 ������ : ��ȣ���� Ÿ��������������� �÷��� �ѱ۷� ��ü
/*//BEFORE-----------------------------------------------------------------------------
			m_pDM->SetCellData(FIELD_ALIAS, Reader.getElement(i, FIELD_ALIAS), i-1);
*///AFTER------------------------------------------------------------------------------
			m_pDM->SetCellData(FIELD_ALIAS, Reader.getElement(i, FIELD_NAME), i-1);
/*//END-------------------------------------------------------------------------------*/
		}

		// 16/02/01 ���湫 : 2016�� �����߾ӵ����� å��������
		// �̿��ڸ� ǥ��
		/*//NEW------------------------------------------------------------------------*/
		
		strUserNo = _T("");
		strUserNo = m_pDM->GetCellData(_T("�̿��ڹ�ȣ"), i-1);
		if(strUserNo.Compare(_T(""))!=0)
		{
			m_pDM->SetCellData(_T("�̿��ڸ�"), GetUserName(strUserNo), i-1);
			// 17/09/25 ������ :  ��ȣ���� Ÿ��������������� �̿��� ������� �ʵ� �߰� 
//*/ ADD ---------------------------------------------------------------------------
			m_pDM->SetCellData(_T("�������"), GetBirthDay(strUserNo), i-1);
//*/ END ---------------------------------------------------------------------------
			// 17/11/06 ������ : ��ȣ���� Ÿ��������������� �̿��� ���� �ʵ� �߰� 
//*/ ADD ---------------------------------------------------------------------------
			m_pDM->SetCellData(_T("�̿������޼���"), GetUserClassDesc(strUserNo), i-1);
//*/ END ---------------------------------------------------------------------------
		}
		
		/*//END------------------------------------------------------------------------*/
	}
	

	SetTimer(1, 300, NULL);
	return 0;
}

INT CKL_OTHER_LOAN_REQUEST::ReturnBook()
{
	CString strState = GetComboBoxState();
	if(_T("3") != strState )
	{
		AfxMessageBox(_T("��û�ڷ���������� �߼� ���¿����� ���͸� ���� �߼��� �����մϴ�."));
		return -1;
	}
	INT ids = 0;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}
	
	//2020.01.29 ADD BY KYJ JOB.2020.0008 : ������ �̰�� �ڷ� ���� ��� �ȳ� �޼��� �߰�
	//----------------------------------------------------------------------------------------------------------------
	int gridIdx = m_pGrid->SelectGetTailPosition();
	CString strDate1 = _T("");	
	COleDateTime date1, date2;
	date2 = COleDateTime::GetCurrentTime();
	
	while( gridIdx > -1 ) 
	{
		m_pDM->GetCellData(_T("���⸸����"), gridIdx, strDate1);
		if( strDate1.IsEmpty() ) continue;
		
		date1.SetDate(_ttoi(strDate1.Mid(0,4)) , _ttoi(strDate1.Mid(5,2)), _ttoi(strDate1.Mid(8,2)) );
		if (date2 < date1 )
		{
			if( AfxMessageBox(_T("���⸸������ ������ ���� �ڷᰡ �ֽ��ϴ�.\r\n�߼� �����Ͻðڽ��ϱ�?\r\n"), MB_YESNO) == IDNO ) 			
				return -1;			
			else 			
				break;		
		}
		
		gridIdx = m_pGrid->SelectGetPrev();
	}	
	//----------------------------------------------------------------------------------------------------------------
	
	CString strLoanKey;	
	CFrameSender Sender(m_pInfo);	
	CFrameReader Reader;
	CFrameWriter Writer(m_pInfo);
	
	INT nIdx = m_pGrid->SelectGetTailPosition();

	// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
	// �߼ۼ����޽��� ó�� �� �߼۱Ǽ� ó��
	/*//NEW------------------------------------------------------------------------*/	
	m_listUserBookCnt.RemoveAll();

	int nBookCnt = 0;
	CString strUserNo = _T("");

	for(int i=0;i<m_pGrid->SelectGetCount();i++)
	{
		m_pDM->GetCellData(_T("�̿��ڹ�ȣ"), nIdx, strUserNo);
		strUserNo += m_pDM->GetCellData(_T("������ȣ�������⸸����"), nIdx);

		if(strUserNo.Compare(_T("")) != 0)
		{
			if(m_listUserBookCnt.Lookup(strUserNo, nBookCnt))
			{
				nBookCnt += 1;
			}
			else
			{
				nBookCnt = 1;
			}

			m_listUserBookCnt.SetAt(strUserNo, nBookCnt);
		}				
		nIdx=m_pGrid->SelectGetPrev();
	}

	nIdx = m_pGrid->SelectGetTailPosition();
	/*//END------------------------------------------------------------------------*/

	for( i=0;i<m_pGrid->SelectGetCount();i++)
	{
		Writer.clear();
		Reader.clear();
		
		m_pDM->GetCellData(_T("����KEY"), nIdx, strLoanKey);	
		Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("TRANSACTION_WORK"), _T("UPDATE"));
		Writer.addElement(_T("TRANSACTION_CODE"), _T("7"));
		Writer.addElement(_T("LOAN_KEY"),strLoanKey);
		Writer.addElement(_T("CANCEL"),_T("0"));
		
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if (0 > ids) 
		{
			while(true) 
			{
				if(IDYES == MessageBox(_T("���յ������� �̵������� ��ſ� �����Ͽ����ϴ�.\r\n��õ� �Ͻðڽ��ϱ�?"), _T("��ȸ����"), MB_YESNO | MB_ICONQUESTION)) 
				{
					ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
					if(0 <= ids) break;
				}
				else
				{
					m_pGrid->Display();
					return 0;
				}
			}
		}		
		Reader.parseFrame(Reader.getFrame());
		if(_T("OK") != Reader.getElement(0,0))
		{
			if(_T("")==Reader.getElement(0,0))
			{
				m_pDM->SetCellData(_T("ó�����"),_T("FAIL"),nIdx);
			}
			else
			{
				// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
				// index ���� �߻� ���� ����
				/*//BEFORE-----------------------------------------------------------------------
				m_pDM->SetCellData(_T("RESULT"), Reader.getElement(nIdx, _T("RESULT")), nIdx);
				*///AFTER------------------------------------------------------------------------
				m_pDM->SetCellData(_T("ó�����"), Reader.getElement(0, _T("REASON")), nIdx);
				/*//END------------------------------------------------------------------------*/
			}
			AfxMessageBox(_T("���� �߻�"));
		}		
		else
		{
			ReturnBook_SendSMS(nIdx);
			m_pDM->DeleteRow(nIdx);
		}
		
		nIdx=m_pGrid->SelectGetPrev();
	}

	m_pGrid->Display();
	AfxMessageBox(_T("�߼� ó�� �Ϸ�"));

	// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
	// �߼ۼ����޽��� ó�� �� �߼۱Ǽ� ó��
	/*//NEW------------------------------------------------------------------------*/	
	m_listUserBookCnt.RemoveAll();
	/*//END------------------------------------------------------------------------*/
	
	return ids;
}

// 15/02/16 ���湫 : 2016�� �����߾ӵ����� å��������
// �Լ��� ��ȣ���� �̴��⸸���� ����
/*//BEFORE-----------------------------------------------------------------------
INT CKL_OTHER_LOAN_REQUEST::GetBook()
*///AFTER------------------------------------------------------------------------
INT CKL_OTHER_LOAN_REQUEST::GetBook(CBL_LOC_LOAN_PROC *pLoan)
/*//END------------------------------------------------------------------------*/
{
	
	CString strState = GetComboBoxState();
	if(_T("1") != strState)
	{
		AfxMessageBox(_T("��û�ڷ���������� �߼� ���¿����� �Լ� ó���� �����մϴ�."));
		return -1;
	}
	INT ids = 0;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}
	INT nIdx = m_pGrid->SelectGetTailPosition();

	// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
	// �߼ۼ����޽��� ó�� �� �߼۱Ǽ� ó��
	/*//NEW------------------------------------------------------------------------*/	
	m_listUserBookCnt.RemoveAll();
	
	int nBookCnt = 0;
	CString strUserNo = _T("");
	
	for(int i=0;i<m_pGrid->SelectGetCount();i++)
	{
		m_pDM->GetCellData(_T("�̿��ڹ�ȣ"), nIdx, strUserNo);

		if(strUserNo.Compare(_T("")) != 0)
		{
			if(m_listUserBookCnt.Lookup(strUserNo, nBookCnt))
			{
				nBookCnt += 1;
			}
			else
			{
				nBookCnt = 1;
			}

			m_listUserBookCnt.SetAt(strUserNo, nBookCnt);
		}				
		nIdx=m_pGrid->SelectGetPrev();
	}

	nIdx = m_pGrid->SelectGetTailPosition();
	/*//END------------------------------------------------------------------------*/

	CString strLoanKey;
	CString strLoanExpiry = _T("7"),
			strLoanExpiryFile = _T("");
	CString strGetDate = m_pDM->GetDate();

	CTime tRenewalTime(_ttoi(strGetDate.Left(4)),_ttoi(strGetDate.Mid(5,2)),_ttoi(strGetDate.Mid(8,2)),0,0,0);		
	
	// 16.11.18 JMJ KOL.RED.002
	CString strHolyDayUseYN = _T("");
	// 17/05/08 ������ : cfg ������ �Ľ��Ͽ� �����ö� �ӵ� ������ �޸𸮷� ��ü����
/*//BEFORE-----------------------------------------------------------------------------
	pLoan->m_CfgFileApi.GetData(_T("���⸸���Ͽ��ް������Կ���"), strHolyDayUseYN);
	pLoan->m_CfgFileApi.GetData(_T("������ȣ�������⸸����"), strLoanExpiryFile);
*///AFTER------------------------------------------------------------------------------
	pLoan->GetTempData(_T("���⸸���Ͽ��ް������Կ���"), strHolyDayUseYN);
	pLoan->GetTempData(_T("������ȣ�������⸸����"), strLoanExpiryFile);
/*//END-------------------------------------------------------------------------------*/

	for(i=0;i<m_pGrid->SelectGetCount();i++)
	{
		m_pDM->GetCellData(_T("������ȣ�������⸸����"), nIdx, strLoanExpiry);

		if(strLoanExpiry.Compare(_T("")) == 0)
		{
			// 16.11.18 JMJ KOL.RED.002
			//strLoanExpiry = _T("7");
			strLoanExpiry = strLoanExpiryFile;
		}
		int nExp = _ttoi(strLoanExpiry);
		// 16.11.18 JMJ KOL.RED.002
		if(strHolyDayUseYN.Compare(_T("Y")) == 0){
			pLoan->m_pCheckLoan->GetAddTotalHolyDay( nExp , tRenewalTime);		
		}
		CTime tReturnDate = tRenewalTime + CTimeSpan(nExp, 0, 0, 0);
		

		m_pDM->GetCellData(_T("����KEY"), nIdx, strLoanKey);
		CFrameWriter Writer(m_pInfo);
		Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("TRANSACTION_WORK"), _T("UPDATE"));
		Writer.addElement(_T("TRANSACTION_CODE"), _T("3"));
		Writer.addElement(_T("LOAN_KEY"),strLoanKey);
		Writer.addElement(_T("CANCEL"),_T("0"));		

		CString strTmp = tReturnDate.Format(_T("%Y/%m/%d"));
		Writer.addElement(_T("RETURN_EXPIRE_DATE"),tReturnDate.Format(_T("%Y/%m/%d")));
		
		CFrameSender Sender(m_pInfo);	
		CFrameReader Reader; 
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if (0 > ids) 
		{
			while(true) 
			{
				if(IDYES == MessageBox(_T("���յ������� �̵������� ��ſ� �����Ͽ����ϴ�.\r\n��õ� �Ͻðڽ��ϱ�?"), _T("��ȸ����"), MB_YESNO | MB_ICONQUESTION)) 
				{
					ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
					if(0 <= ids) break;
				}
				else
				{
					m_pGrid->Display();
					return 0;
				}
			}
		}
		
		Reader.parseFrame(Reader.getFrame());
		if(_T("OK") != Reader.getElement(0,0))
		{
			if(_T("")==Reader.getElement(0,0))
			{
				m_pDM->SetCellData(_T("ó�����"),_T("FAIL"),nIdx);
			}
			else
			{
				// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
				// index ���� �߻� ���� ����
				/*//BEFORE-----------------------------------------------------------------------
				m_pDM->SetCellData(_T("RESULT"), Reader.getElement(nIdx, _T("RESULT")), nIdx);
				*///AFTER------------------------------------------------------------------------
				m_pDM->SetCellData(_T("ó�����"), Reader.getElement(0, _T("REASON")), nIdx);
				/*//END------------------------------------------------------------------------*/
			}
			AfxMessageBox(_T("���� �߻�"));
		}
		else
		{

			// 16/03/02 ���湫 : 2016�� �����߾ӵ����� å��������
			// �Լ��� SMS ������ 
			/*//BEFORE-----------------------------------------------------------------------
			ids = GetBook_SendSMS(nIdx);
			*///AFTER------------------------------------------------------------------------
			CString strReturnDate = tReturnDate.Format(_T("%Y/%m/%d"));
			ids = GetBook_SendSMS(nIdx, strReturnDate);
			/*//END------------------------------------------------------------------------*/
			
			m_pDM->DeleteRow(nIdx);
		}

		nIdx = m_pGrid->SelectGetPrev();
	}

	m_pGrid->Display();
	AfxMessageBox(_T("�Լ� ó���� �Ϸ�Ǿ����ϴ�."));

	// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
	// �߼ۼ����޽��� ó�� �� �߼۱Ǽ� ó��
	/*//NEW------------------------------------------------------------------------*/	
	m_listUserBookCnt.RemoveAll();
	/*//END------------------------------------------------------------------------*/
		
	return ids;
}


INT CKL_OTHER_LOAN_REQUEST::RequestCancel()
{
	CString strState = GetComboBoxState();
	
	if(_T("3") != strState )
	{
		AfxMessageBox(_T("��û�ڷ���������� �Լ� ���¿����� �۾���Ұ� �����մϴ�."));
		return -1;
	}
	
	INT ids = 0;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}

	CString strLoanKey;

	CString strTransactionCode;

	if(_T("3")==strState) strTransactionCode = _T("1");

	CFrameSender Sender(m_pInfo);	
	CFrameReader Reader;
	CFrameWriter Writer(m_pInfo);

	INT nIdx = m_pGrid->SelectGetTailPosition();
	for(int i=0;i<m_pGrid->SelectGetCount();i++)
	{
		Writer.clear();
		Reader.clear();

		m_pDM->GetCellData(_T("����KEY"), nIdx, strLoanKey);	
		Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("TRANSACTION_WORK"), _T("UPDATE"));
		Writer.addElement(_T("TRANSACTION_CODE"), strTransactionCode);
		Writer.addElement(_T("LOAN_KEY"),strLoanKey);
		Writer.addElement(_T("CANCEL"),_T("1"));
	
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if (0 > ids) 
		{
			while(true) 
			{
				if(IDYES == MessageBox(_T("���յ������� �̵������� ��ſ� �����Ͽ����ϴ�.\r\n��õ� �Ͻðڽ��ϱ�?"), _T("��ȸ����"), MB_YESNO | MB_ICONQUESTION)) 
				{
					ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
					if(0 <= ids) break;
				}
				else
				{
					m_pGrid->Display();
					return 0;
				}
			}
		}
		
		Reader.parseFrame(Reader.getFrame());
		if(_T("OK") != Reader.getElement(0,0))
		{				
			if(_T("")==Reader.getElement(0,0))
			{
				m_pDM->SetCellData(_T("ó�����"),_T("FAIL"),nIdx);
			}
			else
			{
				// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
				// index ���� �߻� ���� ����
				/*//BEFORE-----------------------------------------------------------------------
				m_pDM->SetCellData(_T("RESULT"), Reader.getElement(nIdx, _T("RESULT")), nIdx);
				*///AFTER------------------------------------------------------------------------
				m_pDM->SetCellData(_T("ó�����"), Reader.getElement(0, _T("REASON")), nIdx);
				/*//END------------------------------------------------------------------------*/
			}
			AfxMessageBox(_T("���� �߻�"));
		}
		else
		{
			m_pDM->DeleteRow(nIdx);
		}
		
		nIdx=m_pGrid->SelectGetPrev();
	}

	m_pGrid->Display();
	AfxMessageBox(_T("�۾� ��� �Ϸ�"));
	
	return ids;

}
//�Լ� ó���� ���ÿ� SMS �߼�
// 16/03/02 ���湫 : 2016�� �����߾ӵ����� å��������
// �Լ��� ������ ����
/*//BEFORE-----------------------------------------------------------------------
INT CKL_OTHER_LOAN_REQUEST::GetBook_SendSMS(INT nIdx)
*///AFTER------------------------------------------------------------------------
INT CKL_OTHER_LOAN_REQUEST::GetBook_SendSMS(INT nIdx, CString strReturnDate)
/*//END------------------------------------------------------------------------*/
{
	INT ids=0;
	CString strUserNo;
	CString strMessage;
	CString strQuery;
	// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
	// �߼ۼ����޽��� ó�� �� �߼۱Ǽ� ó��
	/*//NEW------------------------------------------------------------------------*/
	int nBookCnt = 0;
	/*//END------------------------------------------------------------------------*/	

	m_pDM->GetCellData(_T("�̿��ڹ�ȣ"), nIdx, strUserNo);
	CString strUserNoKey;
	strUserNoKey = strUserNo + m_pDM->GetCellData(_T("������ȣ�������⸸����"), nIdx);
	
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	// 15/12/10 �ڴ�� : ��ȣ���� �Լ��Ϸ�� �⺻���� ���� ��ȸ ����
/*// BEFORE ----------------------------------------------------------------------------------
	strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS12'"));
*/// AFTER -----------------------------------------------------------------------------------
	CString strManageCode = m_pInfo->MANAGE_CODE;
	if (strManageCode.IsEmpty())
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS12'"));
	else
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS12' AND MANAGE_CODE='%s'"), strManageCode);
//*/ END -------------------------------------------------------------------------------------
	TmpDM.RestructDataManager(strQuery);
	TmpDM.GetCellData(0,0,strMessage);

	//�������� SMS ��� ���θ� üũ�Ѵ�.
	INT nIsSMS = 0;
	nIsSMS = m_pInfo->GetSMSUseMode(); // 0:N, 1:Y
	
	if(1 == nIsSMS)
	{		
		//�̿����� SMS ��� ���θ� üũ�Ѵ�.
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(_T(""));
		
		CString strQuery;
		CString strUserKey;
		strQuery.Format(_T("SELECT CLU.SMS_USE_YN, CLU.HANDPHONE, CLU.NAME, CLU.REC_KEY FROM CO_LOAN_USER_TBL CLU WHERE USER_NO='%s'"),strUserNo);
		dm.RestructDataManager(strQuery);
		CString strSMSYN, strHandPhone, strName;
		dm.GetCellData(0, 0, strSMSYN);
		dm.GetCellData(0, 1, strHandPhone);
		dm.GetCellData(0, 2, strName);
		dm.GetCellData(0, 3, strUserKey);

		strMessage = strName+strMessage;
				
		//SMS ��� ��
		if(_T("Y")==strSMSYN)
		{
			// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
			// �߼ۼ����޽��� ó�� �� �߼۱Ǽ� ó��
			/*//NEW------------------------------------------------------------------------*/	
			if(!m_listUserBookCnt.Lookup(strUserNoKey, nBookCnt))
			{
				nBookCnt= 0;
			}

			if(nBookCnt > 0)
			{
			/*//END------------------------------------------------------------------------*/

				//���ν��� ȣ��!
				if(IDYES == MessageBox(_T("�Լ� ó���� �Ϸ�Ǿ����ϴ�.\r\nSMS�� �߼��Ͻðڽ��ϱ�?"), _T("SMS�߼�Ȯ��"), MB_YESNO | MB_ICONQUESTION)) 
				{
					CString strType = _T("SMS12");

					// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
					// �߼۰��� �߰� �� �߼۱Ǽ� ó��
					/*//NEW------------------------------------------------------------------------*/
					SMS_CONNECTION_INFO		SMS_ConnectionInfo;
					SMS_MSG_INFO	SMS_MsgInfo;

					CSMS_MakeMsg MakeSMS( this );
					CSMS_Env_Info SMSEnvInfo(this);
					SMSEnvInfo.MakeSMSEnv();
					SMSEnvInfo.GetSMSEnv( strType, &SMS_ConnectionInfo, &SMS_MsgInfo );

					CString strTitleInfo;
					m_pDM->GetCellData(_T("����"), nIdx, strTitleInfo);

					CString strBookCnt;
					strBookCnt.Format(_T("%d"),nBookCnt);
					MakeSMS.MakeSMSMsg( strType , &SMS_MsgInfo , strName , strReturnDate, strTitleInfo , strBookCnt , strMessage );

					CString strReplace = strMessage;
					strReplace.Replace(_T("'"),_T("''"));								
					CString strResult = _T("");
					// 17/11/01 ������ : LMS�߰��� ���� ó��
/*//BEFORE-----------------------------------------------------------------------------
					strQuery.Format(_T("SELECT SEND_MESSAGE FROM MN_SMS_LOG_TBL WHERE to_char(SEND_TIME, 'yyyymmdd') = to_char(SYSDATE, 'yyyymmdd') AND SEND_MESSAGE = '%s'"),strReplace);
*///AFTER------------------------------------------------------------------------------
					strQuery.Format(_T("SELECT SEND_MESSAGE FROM MN_SMS_LOG_VIEW WHERE to_char(SEND_TIME, 'yyyymmdd') = to_char(SYSDATE, 'yyyymmdd') AND SEND_MESSAGE = '%s'"),strReplace);
/*//END-------------------------------------------------------------------------------*/						
					dm.GetOneValueQuery(strQuery, strResult);

					if(strResult.Compare(_T("")) != 0)
					{
						if(IDYES == MessageBox(_T("���� �߼��� �޽����Դϴ�.\r\nSMS�� �߼��Ͻðڽ��ϱ�?"), _T("SMS�߼�Ȯ��"), MB_YESNO | MB_ICONQUESTION)) 
						{
							ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);
						}
					}
					else
					{
						ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);
					}
											
					/*//END------------------------------------------------------------------------*/
				}

			// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
			// �߼۱Ǽ� ó��
			/*//NEW------------------------------------------------------------------------*/				
			}
			/*//END------------------------------------------------------------------------*/
			if(0 > ids)
			{				
				AfxMessageBox(_T("�޼��� �߼ۿ� �����Ͽ����ϴ�."));
			}
			else
			{
				// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
				// �߼۱Ǽ� ó��
				/*//NEW------------------------------------------------------------------------*/				
				m_listUserBookCnt.RemoveKey(strUserNoKey);
				/*//END------------------------------------------------------------------------*/
				return 0;
			}
		}
		else
		{
			AfxMessageBox(_T("�ش� �̿��ڴ� SMS ���񽺸� ������� �ʽ��ϴ�."));
		}
		
	}
	else
	{
		AfxMessageBox(_T("�ش� �������� SMS ���񽺸� ������� �ʽ��ϴ�."));
	}	
	return ids;	
}

//���ν��� ȣ��
INT CKL_OTHER_LOAN_REQUEST::CallProcedure(CString strUserNo, CString strHandPhone, CString strName, CString strMessage, CString strType)
{	
	INT ids;
	// 1. DM SETTING 
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
	
	// FIRST_WORK
	CString sFirst_Work;
	sFirst_Work.Format( _T("%s"), GetWorkLogMsg( _T("�弭����"), __WFILE__, __LINE__ ) );
	// SEND PHONE
	CString sQuery, sDividCnt, sSendMsg, sSendNum, sRecevNum, sUserKey, sLoanKey, sUserName;
	sRecevNum = strHandPhone;
	CCfgFileApi cfgFileApi;
	//2019.08.26 JOB.2019.0076 ���ϰ�� ����
	//CString strCfgFilePath = _T("..\\..\\..\\..\\..\\..\\cfg\\SMS_Callback.cfg");
	CString strCfgFilePath = _T("..\\cfg\\SMS_Callback.cfg");
	cfgFileApi.SetFile(strCfgFilePath);
	CFileFind ff;
	if(ff.FindFile(strCfgFilePath))
	{
		CString strCallbackTarget;
		cfgFileApi.GetData(_T("ȸ�Ź�ȣ����������"), strCallbackTarget);
		CString strValue;
		if(_T("1") == strCallbackTarget)
		{
			cfgFileApi.GetData(_T("ȸ�Ź�ȣ"), strValue);
			if(!strValue.IsEmpty()) sSendNum = strValue;
		}
	}
	sRecevNum.Replace(_T("-"), _T(""));
	
	sSendMsg = strMessage;
	sSendMsg.Replace(_T("'"), _T("''"));
	sUserKey = strUserNo;
	sLoanKey = _T("0");
	sUserName = strName;
	// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*	// (1:USER_KEY,2:LOAN_KEY,3:SEND_TEL,4:RECV_TEL,5:MSG,6:MSG_GB,7:MANAGE_CODE,8:SMS_TYPE,9:SEND_WORKER,10:FIRST_WORK,11:USER_NAME)
	sQuery.Format( _T("SMS_TOT_SEND_PROC(%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s');")
		, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	// 1,2,3,4,5
		, _T(""), m_pInfo->MANAGE_CODE, strType				// 6,7,8
		, m_pInfo->USER_NAME, sFirst_Work, sUserName );			// 9,10,11
		*/
	if (g_strKakaoUsed2.IsEmpty())
	{
		sQuery.Format( 
			_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KAKAO�÷���ģ���ý�������' AND value_name='KAKAO�÷���ģ���������' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		TmpDM.GetOneValueQuery(sQuery, g_strKakaoUsed2);
		
		// �������� ���ٸ� N���� ����
		if (g_strKakaoUsed2.IsEmpty())
		{
			g_strKakaoUsed2 = _T("N");
		}
	}
	if (g_strKakaoUsed2.CompareNoCase(_T("Y")) == 0)
	{
		// īī�� �÷��� ģ�� �߼�
		sQuery.Format( _T("KAKAOPLUS_TOT_SEND_PROC(%s, %s, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');")
								, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg // 1,2,3,4,5
								, _T(""), m_pInfo->MANAGE_CODE, strType				// 6,7,8
								, m_pInfo->USER_NAME, sFirst_Work, sUserName,		// 9,10,11
								m_pInfo->LIB_NAME , sSendMsg );
	}
	else
	{
		// SMS �߼�
		sQuery.Format( _T("SMS_TOT_SEND_PROC(%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s');")
				, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	// 1,2,3,4,5
				, _T(""), m_pInfo->MANAGE_CODE, strType				// 6,7,8
				, m_pInfo->USER_NAME, sFirst_Work, sUserName );			// 9,10,11
	}
	// KOL.RED.2018.224 ---------------------------------------------------------------------------

	ids = TmpDM.ExecuteQuery( sQuery, TRUE );
	if(0>ids)
	{ 
		AfxMessageBox(_T("�޽��� �߼ۿ� �����Ͽ����ϴ�."));
	}
	return 0;
	
}


void CKL_OTHER_LOAN_REQUEST::SelectGetBook()
{
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)FindControl(m_strCMAlias, _T("��Ƽ�޺�_�������"));
	pCMB->SetCurSel(3);
	Refresh();
}


INT CKL_OTHER_LOAN_REQUEST::SelectGrid(CString sRegNo)
{
	CString strRegNo;
	// 17/06/28 ������ : Ÿ������������� ���ڵ�� �Էµ� ��Ϲ�ȣ�� �׸��忡 �������� ������ �޽��� ó��
//*/ ADD ---------------------------------------------------------------------------
	BOOL bExsistRegNo = FALSE;
//*/ END ---------------------------------------------------------------------------
	for(int i=0; i< m_pDM->GetRowCount() ; i++)
	{
		m_pDM->GetCellData(_T("��Ϲ�ȣ"), i, strRegNo);
		if(sRegNo == strRegNo)
		{
			// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
			// �÷� ���� ����� ���� �߻� ���� ����
			/*//BEFORE-----------------------------------------------------------------------
			m_pGrid->SetAt(i, 1, _T("V"));
			*///AFTER------------------------------------------------------------------------
			m_pGrid->SetAt(i, m_pGrid->m_nSelectIdx, m_pGrid->m_sSelectString);
			m_pGrid->m_nReverse = i;
			m_pGrid->SetReverse(i);
			/*//END------------------------------------------------------------------------*/
			//m_pGrid->DisplayLine(i);
			// 17/05/19 ������ : Ÿ���������/Ÿ���ݳ������� GRID �����÷����� ����
//*/ ADD ---------------------------------------------------------------------------
			m_pDM->SetCellData(_T("����"), m_pGrid->m_sSelectString, i);
//*/ END ---------------------------------------------------------------------------

			// 17/06/28 ������ : Ÿ������������� ���ڵ�� �Էµ� ��Ϲ�ȣ�� �׸��忡 �������� ������ �޽��� ó��
//*/ ADD ---------------------------------------------------------------------------
			bExsistRegNo = TRUE;
//*/ END ---------------------------------------------------------------------------
		}
		// 17/02/23 ������ : Ÿ������������� ���ڵ�� �ڷ� ������ �ǵ��� ��� ����
		// ù��° row�� �˻��ϰ� �����ϵ��� �Ǿ� �־ ������
/*//REM--------------------------------------------------------------------------
		else
		{
			return -1;
		}
*///END--------------------------------------------------------------------------	
	}
	// 17/06/28 ������ : Ÿ������������� ���ڵ�� �Էµ� ��Ϲ�ȣ�� �׸��忡 �������� ������ �޽��� ó��
//*/ ADD ---------------------------------------------------------------------------
	if(bExsistRegNo != TRUE)
	{
		return 1;
	}
//*/ END ---------------------------------------------------------------------------

	// 17/05/19 ������ : Ÿ���������/Ÿ���ݳ������� GRID �����÷����� ����
//*/ ADD ---------------------------------------------------------------------------
	m_pDM->SORT(_T("����"),TRUE);
	m_pGrid->Display();
//*/ END ---------------------------------------------------------------------------
	return 0;
}

// 16/02/01 ���湫 : 2016�� �����߾ӵ����� å��������
// ����ڸ� ��������
/*//NEW------------------------------------------------------------------------*/
CString CKL_OTHER_LOAN_REQUEST::GetUserName(CString strUserNo)
{
	int ids = 0;
	CString strUserName = _T(""), strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	strQuery.Format(_T("SELECT name FROM co_loan_user_tbl WHERE user_no = '%s'"), strUserNo);
	ids = TmpDM.GetOneValueQuery(strQuery,strUserName);
	if(ids < 0)
	{
		strUserName = _T("");
	}
	return strUserName;
}
/*//END------------------------------------------------------------------------*/

// 17/09/25 ������ :  ��ȣ���� Ÿ��������������� �̿��� ������� �ʵ� �߰� 
//*/ ADD ---------------------------------------------------------------------------
CString CKL_OTHER_LOAN_REQUEST::GetBirthDay(CString strUserNo)
{
	int ids = 0;
	CString strBirthDay = _T(""), strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	strQuery.Format(_T("SELECT BIRTHDAY FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'"), strUserNo);
	ids = TmpDM.GetOneValueQuery(strQuery,strBirthDay);
	if(ids < 0)
	{
		strBirthDay = _T("");
	}
	return strBirthDay;
}
//*/ END ---------------------------------------------------------------------------

// 17/11/06 ������ : ��ȣ���� Ÿ��������������� �̿��� ���� �ʵ� �߰� 
//*/ ADD ---------------------------------------------------------------------------
CString CKL_OTHER_LOAN_REQUEST::GetUserClassDesc(CString strUserNo)
{
	int ids = 0;
	CString strUserClassDesc = _T(""), strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	strQuery.Format(_T("SELECT ECO$F_GET_CODE2DESC('31', USER_CLASS_CODE) FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'"), strUserNo);
	ids = TmpDM.GetOneValueQuery(strQuery,strUserClassDesc);
	if(ids < 0)
	{
		strUserClassDesc = _T("");
	}
	return strUserClassDesc;
}
//*/ END ---------------------------------------------------------------------------

// 16/02/19 ���湫 : 2016�� �����߾ӵ����� å��������
// �޸����
/*//NEW------------------------------------------------------------------------*/
void CKL_OTHER_LOAN_REQUEST::ModifyMemo()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return;
	}

	CKL_OTHER_LOAN_EDIT_MEMO dlgEditMemo(m_pParent);
	dlgEditMemo.SetGrid(m_pGrid);
	dlgEditMemo.SetDM(m_pDM);

	dlgEditMemo.DoModal();

	if(dlgEditMemo.m_BModify)
		Refresh();
}
/*//END------------------------------------------------------------------------*/

// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
// �̴���߼� SMS
/*//NEW------------------------------------------------------------------------*/
INT CKL_OTHER_LOAN_REQUEST::ReturnBook_SendSMS(INT nIdx)
{
	INT ids=0;
	CString strUserNo;
	CString strMessage;
	CString strQuery;
	int nBookCnt = 0;
	m_pDM->GetCellData(_T("�̿��ڹ�ȣ"), nIdx, strUserNo);
	
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	
	CString strManageCode = m_pInfo->MANAGE_CODE;
	if (strManageCode.IsEmpty())
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS12'"));
	else
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS12' AND MANAGE_CODE='%s'"), strManageCode);

	TmpDM.RestructDataManager(strQuery);
	TmpDM.GetCellData(0,0,strMessage);

	//�������� SMS ��� ���θ� üũ�Ѵ�.
	INT nIsSMS = 0;
	nIsSMS = m_pInfo->GetSMSUseMode(); // 0:N, 1:Y
	
	if(1 == nIsSMS)
	{		
		//�̿����� SMS ��� ���θ� üũ�Ѵ�.
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(_T(""));
		
		CString strQuery;
		CString strUserKey;
		strQuery.Format(_T("SELECT CLU.SMS_USE_YN, CLU.HANDPHONE, CLU.NAME, CLU.REC_KEY FROM CO_LOAN_USER_TBL CLU WHERE USER_NO='%s'"),strUserNo);
		dm.RestructDataManager(strQuery);
		CString strSMSYN, strHandPhone, strName;
		dm.GetCellData(0, 0, strSMSYN);
		dm.GetCellData(0, 1, strHandPhone);
		dm.GetCellData(0, 2, strName);
		dm.GetCellData(0, 3, strUserKey);

		strMessage = strName+strMessage;
				
		//SMS ��� ��
		if(_T("Y")==strSMSYN)
		{
			if(!m_listUserBookCnt.Lookup(strUserNo, nBookCnt))
			{
				nBookCnt= 0;
			}

			if(nBookCnt > 0)
			{

				//���ν��� ȣ��!
				if(IDYES == MessageBox(_T("�߼� ó���� �Ϸ�Ǿ����ϴ�.\r\nSMS�� �߼��Ͻðڽ��ϱ�?"), _T("SMS�߼�Ȯ��"), MB_YESNO | MB_ICONQUESTION)) 
				{
					CString strType = _T("SMS15");
					
					SMS_CONNECTION_INFO		SMS_ConnectionInfo;
					SMS_MSG_INFO	SMS_MsgInfo;

					CSMS_MakeMsg MakeSMS( this );
					CSMS_Env_Info SMSEnvInfo(this);
					SMSEnvInfo.MakeSMSEnv();
					SMSEnvInfo.GetSMSEnv( strType, &SMS_ConnectionInfo, &SMS_MsgInfo );

					CString strTitleInfo;
					m_pDM->GetCellData(_T("����"), nIdx, strTitleInfo);

					CString strBookCnt;
					strBookCnt.Format(_T("%d"),nBookCnt);
					MakeSMS.MakeSMSMsg( strType , &SMS_MsgInfo , strName , _T(""), strTitleInfo , strBookCnt , strMessage );

					CString strReplace = strMessage;
					strReplace.Replace(_T("'"),_T("''"));								
					CString strResult = _T("");
					// 17/11/01 ������ : LMS�߰��� ���� ó��
/*//BEFORE-----------------------------------------------------------------------------
					strQuery.Format(_T("SELECT SEND_MESSAGE FROM MN_SMS_LOG_TBL WHERE to_char(SEND_TIME, 'yyyymmdd') = to_char(SYSDATE, 'yyyymmdd') AND SEND_MESSAGE = '%s'"),strReplace);
*///AFTER------------------------------------------------------------------------------
					strQuery.Format(_T("SELECT SEND_MESSAGE FROM MN_SMS_LOG_VIEW WHERE to_char(SEND_TIME, 'yyyymmdd') = to_char(SYSDATE, 'yyyymmdd') AND SEND_MESSAGE = '%s'"),strReplace);
/*//END-------------------------------------------------------------------------------*/					
					dm.GetOneValueQuery(strQuery, strResult);

					if(strResult.Compare(_T("")) != 0)
					{
						if(IDYES == MessageBox(_T("���� �߼��� �޽����Դϴ�.\r\nSMS�� �߼��Ͻðڽ��ϱ�?"), _T("SMS�߼�Ȯ��"), MB_YESNO | MB_ICONQUESTION)) 
						{
							ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);
						}
					}
					else
					{
						ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);
					}				
				}
			}

			if(0 > ids)
			{
				
				AfxMessageBox(_T("�޼��� �߼ۿ� �����Ͽ����ϴ�."));
			}
			else
			{
				m_listUserBookCnt.RemoveKey(strUserNo);
				return 0;
			}
		}
		else
		{
			AfxMessageBox(_T("�ش� �̿��ڴ� SMS ���񽺸� ������� �ʽ��ϴ�."));
		}
		
	}
	else
	{
		AfxMessageBox(_T("�ش� �������� SMS ���񽺸� ������� �ʽ��ϴ�."));
	}	
	return ids;	
}
/*//END------------------------------------------------------------------------*/

// 16/03/08 ���湫 : 2016�� �����߾ӵ����� å��������
// �̿��ڻ󼼺���
/*//NEW------------------------------------------------------------------------*/
void CKL_OTHER_LOAN_REQUEST::ViewLoanUser()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return;
	}

	CKL_OTHER_LOAN_USER_DETAILVIEW dlgViewLoanUser(m_pParent);
	dlgViewLoanUser.SetGrid(m_pGrid);
	dlgViewLoanUser.SetDM(m_pDM);

	dlgViewLoanUser.DoModal();	
}
/*//END------------------------------------------------------------------------*/