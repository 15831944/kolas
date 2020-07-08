// KL_OTHER_LOAN_OFFER.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_LOAN_OFFER.h"
#include "FrameManager.h"

// 16/02/03 ���湫 : 2016�� �����߾ӵ����� å��������
// MainHeader �߰�
/*//NEW------------------------------------------------------------------------*/
#include "KL_OTHER_LOAN_MAIN.h"
#include "..\\..\\..\\..\\����\\BO_ACQ_PRINT_MGR\\BasicPrintMgr.h"
#include <map>
#include "KL_OTHER_LOAN_EDIT_MEMO.h"
#include "KL_OTHER_LOAN_USER_DETAILVIEW.h"

#include "..\\..\\..\\..\\SMS\\CO_SMS\\SMS_MakeMsg.h"
#include "..\\..\\..\\..\\SMS\\CO_SMS\\SMS_Env_Info.h"
/*//END------------------------------------------------------------------------*/

// 16/02/25 �ڿ�� : 2016�� �����߾ӵ����� å��������
// �η� �߼�
/*//NEW------------------------------------------------------------------------*/
#include "KL_OTHER_APPENDIX_LIST.h"
/*//END------------------------------------------------------------------------*/

// KOL.RED.2018.224 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CString g_strKakaoUsed1;
// KOL.RED.2018.224 ---------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_OFFER dialog


CKL_OTHER_LOAN_OFFER::CKL_OTHER_LOAN_OFFER(CESL_Mgr* pParent /*=NULL*/)
: CKL_OTHER_LOAN_DEFAULT(pParent)
{
	m_strSMAlias = _T("SM_KL_OTHER_LOAN_MNG_OFFER");
	m_strCMAlias = _T("CM_KL_OTHER_LOAN_MNG_OFFER");
	m_strDMAlias = _T("DM_KL_OTHER_LOAN_MNG_OFFER");
	m_pParent = pParent;
	//{{AFX_DATA_INIT(CKL_OTHER_LOAN_OFFER)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}
CKL_OTHER_LOAN_OFFER::~CKL_OTHER_LOAN_OFFER()
{
}

void CKL_OTHER_LOAN_OFFER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_LOAN_OFFER)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_LOAN_OFFER, CKL_OTHER_LOAN_DEFAULT)
//{{AFX_MSG_MAP(CKL_OTHER_LOAN_OFFER)
ON_WM_SIZE()
ON_CBN_SELCHANGE(IDC_cmb, OnSelchangecmb)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_OFFER message handlers

BOOL CKL_OTHER_LOAN_OFFER::Create(CWnd* pParent) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParent);
}

BOOL CKL_OTHER_LOAN_OFFER::OnInitDialog() 
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
		if(_T("��û") == strData)
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
		m_pGrid->m_strPrintInfo = _T("Ÿ�������������");
	}
	//-------------------------------------------------------------------------

	if (m_pLoanManageValue->m_sLoanUserAccessMode.IsEmpty() || m_pLoanManageValue->m_sLoanUserAccessMode == _T("1") )
	{
		m_pGrid->SetColumnEncryption(_T("�̿��ڸ�"),TRUE);
		m_pGrid->SetColumnEncryption(_T("�������"),TRUE);
	}
	
	Refresh();
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKL_OTHER_LOAN_OFFER::ResizeControl()
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
INT CKL_OTHER_LOAN_OFFER::ResetComboBox()
{
	int ids = -1;
	
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(m_strCMAlias, _T("��Ƽ�޺�_�������"));
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
		{ _T("3"), _T("�Լ�")},
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
	m_strDescription[0] = _T("�ڰ� å�� Ÿ������ �����ϱ� ���� �̿��ڰ� ��û�� ���");
	m_strDescription[1] = _T("�ڰ� å�� ������ ���� ������ ���������� �߼��� ���");
	m_strDescription[2] = _T("��û �ڷᰡ ��� ������ ���� �߼۰����� ���");
	m_strDescription[3] = _T("�ڰ����� �߼��� å�� ������ �������� �Լ��� ���");
	m_strDescription[4] = _T("�ڰ� å�� Ÿ������ �������� ���");
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


void CKL_OTHER_LOAN_OFFER::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	ResizeControl();
	// TODO: Add your message handler code here
	
}
void CKL_OTHER_LOAN_OFFER::OnSelchangecmb() 
{
	// TODO: Add your control notification handler code here
	Refresh();
}

INT CKL_OTHER_LOAN_OFFER::Refresh()
{
	// 16/02/01 ���湫 : 2016�� �����߾ӵ����� å��������
	// �̿��ڸ� ǥ��
	/*//NEW------------------------------------------------------------------------*/
	CString strUserNo = _T("");
	/*//END------------------------------------------------------------------------*/

	CString strState = GetComboBoxState();

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
	m_pParent->SendMessage(WM_KL_ACTIVE_BUTTON, NULL, (LPARAM)nState);
	/*//END------------------------------------------------------------------------*/
	
	SetDateControlView();
	CString strDate1,strDate2;

	strDate1 = GetBeforeDate();
	strDate2 = GetAfterDate();
	
	if(m_bIsSearched)
	{
		return 1000;
	}
	
	m_bIsSearched = TRUE;
	
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("TRANSACTION_WORK"), _T("SEARCH"));
	Writer.addElement(_T("SEARCH_OPTION"), _T("OFFER"));
	Writer.addElement(_T("TRANSACTION_CODE"), strState);
	Writer.addElement(_T("LIB_CODE"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("PAGE_NO"), _T("1"));
	Writer.addElement(_T("DISPLAY"), _T("1000"));

	// 15/02/18 ���湫 : 2016�� �����߾ӵ����� å��������
	// �Լ��÷� ����
	/*//BEFORE-----------------------------------------------------------------------
	if(_T("2") == strState)
	*///AFTER------------------------------------------------------------------------
	//if(_T("3") == strState)
	/*//END------------------------------------------------------------------------*/	
	//2018-11-14 EDIT BY KYJ
	//JOB.2018.0082 ��û���µ� ���ڷ� ���� �߰� �� �� �ֵ���
	//=======================================================
	if(_T("0") == strState || _T("3") == strState)
	//=======================================================
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

INT CKL_OTHER_LOAN_OFFER::SendBook()
{

	// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
	// �߼ۼ����޽��� ó�� �� �߼۱Ǽ� ó��
	/*//NEW------------------------------------------------------------------------*/
	BOOL bSuccess = FALSE;
	m_listUserBookCnt.RemoveAll();
	/*//END------------------------------------------------------------------------*/

	CString strState = GetComboBoxState();
	if(_T("0") != strState)
	{
		AfxMessageBox(_T("��û������ ���������� ��û ���¿����� �߼��� �����մϴ�."));
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
	CString strLoanKey;

	// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
	// �߼۱Ǽ� ó��
	/*//NEW------------------------------------------------------------------------*/
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

	// 16/02/25 �ڿ�� : 2016�� �����߾ӵ����� å��������
	// �ηϽ�û����, �ηϹ߼�����, �ηϵ�Ϲ�ȣ����Ʈ
	/*//NEW------------------------------------------------------------------------*/
	CString strAppendixRctYN = _T(""), strAppendixSendYN = _T("N"), strAppendixRegNoList = _T("");
	/*//END------------------------------------------------------------------------*/

	CFrameWriter Writer(m_pInfo);
	for(i=0;i<m_pGrid->SelectGetCount();i++)
	{
		// 16/08/25 ������ : �η��ڷ� Ÿ���߼۽� �ηϹ߼������� �ʱ�ȭ ���� �ʾ� ����
//*/ ADD -----------------------------------------------------------------------------------------------
		strAppendixSendYN = _T("N");  // �ηϹ߼����� ����
		strAppendixRegNoList = _T(""); //�ηϵ�Ϲ�ȣ ����Ʈ 
//*/ END -----------------------------------------------------------------------------------------------

		// 2013.12.19 ADD BY PWR : 2013���������ռ���Ȯ�뱸��װ��� (�߼�ó�� ������ å���� ����)
		CString strRegNo;
		m_pDM->GetCellData(_T("��Ϲ�ȣ"), nIdx, strRegNo);
		if(strRegNo.IsEmpty()) {
			m_pDM->SetCellData(_T("ó�����"),_T("FAIL (REG_NO IS NULL)"),nIdx);
			// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
			// index �̵� ���� �߰� (�ηϹ߼��� �ٸ��ڷ�� ó���Ǵ� ���� ����)
			/*//NEW------------------------------------------------------------------------*/
			nIdx=m_pGrid->SelectGetPrev();
			/*//END------------------------------------------------------------------------*/
			continue;
		}

		m_pDM->GetCellData(_T("����KEY"), nIdx, strLoanKey);

		// 17/06/29 ������ : å������û�� å�� �ڰ�����Ǿ����� Ÿ���� �߼۵��� �ʵ��� ����
//*/ ADD ----------------------------------------------------------------------------------
		CString strBookKey = _T("");
		CString strWorkingStatus = _T("");
		CString strQuery = _T("");
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(_T(""));		
		m_pDM->GetCellData(_T("åKEY"), nIdx, strBookKey);		
		strQuery.Format(_T("SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE REC_KEY='%s'"), strBookKey);
		TmpDM.GetOneValueQuery(strQuery,strWorkingStatus);
		
		if(strWorkingStatus != _T("BOL112N")) //�ڷ���°� ��ġ�ڷ����� Ȯ��
		{
			m_pDM->SetCellData(_T("ó�����"),_T("FAIL (��ġ�ڷ� ���°� �ƴ�)"),nIdx);			
			nIdx=m_pGrid->SelectGetPrev();			
			continue;
		}
//*/ END ----------------------------------------------------------------------------------

		// 16/02/25 �ڿ�� : 2016�� �����߾ӵ����� å��������
		// �������������� ���⵵�������� �߼۽� �̿��ڰ� �ηϽ�û�� �� �ڷ�� �ηϴ��� ȭ���� ǥ��
		/*//NEW------------------------------------------------------------------------*/
		m_pDM->GetCellData(_T("�ηϽ�û����"), nIdx, strAppendixRctYN);
		if (_T("Y") == strAppendixRctYN) {
			// �ηϹ߼� ȭ��
			CESL_DataMgr *pDM = FindDM(_T("DM_KL_OTHER_APPENDIX_LIST"));
			if (pDM == NULL) {
				m_pDM->SetCellData(_T("ó�����"),_T("FAIL (DM_KL_OTHER_APPENDIX_LIST)"),nIdx);
				// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
				// index �̵� ���� �߰� (�ηϹ߼��� �ٸ��ڷ�� ó���Ǵ� ���� ����)
				/*//NEW------------------------------------------------------------------------*/
				nIdx=m_pGrid->SelectGetPrev();
				/*//END------------------------------------------------------------------------*/
				continue;
			}
			CString strWhere;
			strWhere.Format(_T("B.SPECIES_KEY = (SELECT BB.SPECIES_KEY FROM BO_BOOK_TBL BB WHERE BB.REG_NO='%s')"), strRegNo);
			pDM->FreeData();
			INT ids = pDM->RefreshDataManager(strWhere);
			if (0 > ids) {
				m_pDM->SetCellData(_T("ó�����"),_T("FAIL (�η���ȸ ����)"),nIdx);
				// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
				// index �̵� ���� �߰� (�ηϹ߼��� �ٸ��ڷ�� ó���Ǵ� ���� ����)
				/*//NEW------------------------------------------------------------------------*/
				nIdx=m_pGrid->SelectGetPrev();
				/*//END------------------------------------------------------------------------*/
				continue;
			}
			CKL_OTHER_APPENDIX_LIST AppendixList(this);
			ids = AppendixList.DoModal();
			if (IDOK == ids) {
				if (0 < pDM->GetRowCount()) {
					// �ηϹ߼����� ����
					strAppendixSendYN = _T("Y");			
					// 16/08/05 ������ : �η��ڷ� Ÿ���߼۽� �ηϵ�Ϲ�ȣ ����Ʈ�� �ʱ�ȭ ���� �ʾ� ����
//*/ ADD -----------------------------------------------------------------------------------------------
					strAppendixRegNoList = _T("");
//*/ END -----------------------------------------------------------------------------------------------
					// �ηϵ�Ϲ�ȣ����Ʈ ����
					CString strRegNo;
					for (INT i = 0; i < pDM->GetRowCount(); i++) {
						if (FALSE == strAppendixRegNoList.IsEmpty()) strAppendixRegNoList += _T(",");
						pDM->GetCellData(_T("��Ϲ�ȣ"), i, strRegNo);
						strAppendixRegNoList += strRegNo;
					}
				}
				// 16/02/26 ���湫 : 2016�� �����߾ӵ����� å��������
				// �߼ۼ����޽��� ó��
				/*//NEW------------------------------------------------------------------------*/
				if(!bSuccess)
					bSuccess = TRUE;
				/*//END------------------------------------------------------------------------*/
			} else {
				m_pDM->SetCellData(_T("ó�����"),_T("�߼����"),nIdx);
				// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
				// index �̵� ���� �߰� (�ηϹ߼��� �ٸ��ڷ�� ó���Ǵ� ���� ����)
				/*//NEW------------------------------------------------------------------------*/
				nIdx=m_pGrid->SelectGetPrev();
				/*//END------------------------------------------------------------------------*/
				continue;
			}
		}
		/*//END------------------------------------------------------------------------*/

		Writer.clear();
		Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("TRANSACTION_WORK"), _T("UPDATE"));
		Writer.addElement(_T("TRANSACTION_CODE"), _T("1"));
		Writer.addElement(_T("LOAN_KEY"),strLoanKey);
		Writer.addElement(_T("CANCEL"),_T("0"));
		
		// 16/02/25 �ڿ�� : 2016�� �����߾ӵ����� å��������
		// �߼۽� �ηϹ߼�����, �ηϵ�Ϲ�ȣ����Ʈ ����
		/*//NEW------------------------------------------------------------------------*/
		Writer.addElement(_T("APPENDIX_SEND_YN"), strAppendixSendYN);
		Writer.addElement(_T("APPENDIX_REGNO_LIST"), strAppendixRegNoList);
		/*//END------------------------------------------------------------------------*/

		CFrameSender Sender(m_pInfo);	
		CFrameReader Reader; 
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
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
					// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
					/*//NEW------------------------------------------------------------------------*/
					m_pGrid->Display();
					/*//END------------------------------------------------------------------------*/
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
			// 16/02/26 ���湫 : 2016�� �����߾ӵ����� å��������
			// �߼ۼ����޽��� ó��
			/*//NEW------------------------------------------------------------------------*/
			if(!bSuccess)
				bSuccess = TRUE;
			/*//END------------------------------------------------------------------------*/

			// 16/03/02 ���湫 : 2016�� �����߾ӵ����� å��������
			// �߼� SMS
			/*//NEW------------------------------------------------------------------------*/
			SendBook_SendSMS(nIdx);
			m_pDM->DeleteRow(nIdx);
				// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
				/*//REM--------------------------------------------------------------------------
				nIdx=m_pGrid->SelectGetPrev();
				*///END--------------------------------------------------------------------------
			/*//END------------------------------------------------------------------------*/
			// 2013.12.19 ADD BY PWR : 2013���������ռ���Ȯ�뱸��װ��� (�߼�ó�� ������ å���� ����)
			/*
			m_pDM->StartFrame();
			CString strQuery;
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKING_STATUS='BOL611O',PREV_WORKING_STATUS=WORKING_STATUS WHERE REG_NO='%s';"), strRegNo);
			m_pDM->AddFrame(strQuery);
			m_pDM->SendFrame();
			m_pDM->EndFrame();

			m_pDM->DeleteRow(nIdx);
			*/
		}
		// 16/03/03 ���湫 : 2016�� �����߾ӵ����� å��������
		// �ٸ� �ڵ�� ���� ������� �̵�
		/*//REM--------------------------------------------------------------------------
		//nIdx=m_pGrid->SelectGetPrev();
		*///END--------------------------------------------------------------------------

		// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
		// index �̵� ���� �߰� (�ηϹ߼��� �ٸ��ڷ�� ó���Ǵ� ���� ����)
		/*//NEW------------------------------------------------------------------------*/
		nIdx=m_pGrid->SelectGetPrev();
		/*//END------------------------------------------------------------------------*/
	}

	m_pGrid->Display();

	// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
	// �߼ۼ����޽��� ó�� �� �߼۱Ǽ� ó��
	/*//BEFORE-----------------------------------------------------------------------
	AfxMessageBox(_T("�߼� ó�� �Ϸ�"));
	*///AFTER------------------------------------------------------------------------
	if(bSuccess)
		AfxMessageBox(_T("�߼� ó�� �Ϸ�"));

	m_listUserBookCnt.RemoveAll();
	/*//END------------------------------------------------------------------------*/
	
	return ids;
}


INT CKL_OTHER_LOAN_OFFER::OfferCancel()
{
	CString strState = GetComboBoxState();
	// 16/01/29 ���湫 : 2016�� �����߾ӵ����� å��������
	// ������� �߰��� ��� �Ұ��� �߰�
	/*//BEFORE-----------------------------------------------------------------------
	if(_T("0") == strState || _T("3") == strState)
	*///AFTER------------------------------------------------------------------------
	if(_T("0") == strState || _T("3") == strState || _T("4") == strState)
	{
		AfxMessageBox(_T("�����ڷ���������� �߼� �� �߼۰��� ���¿����� �۾���Ұ� �����մϴ�."));
		return -1;
	}
	/*//END------------------------------------------------------------------------*/
	INT ids = 0;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	// JMJ KOL.RED.003 �۾���ҽ� �ηϵ� ���� ����
	CString strAppendixRegNoList, strAppendixSendYN;

	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}
	INT nIdx = m_pGrid->SelectGetTailPosition();
	CString strLoanKey;
	
	CString strTransactionCode;
	
	if(_T("1")==strState) strTransactionCode = _T("0");
	if(_T("2")==strState) strTransactionCode = _T("0");
	if(_T("3")==strState) strTransactionCode = _T("1");
	
	for(int i=0;i<m_pGrid->SelectGetCount();i++)
	{
		m_pDM->GetCellData(_T("����KEY"), nIdx, strLoanKey);
		CFrameWriter Writer(m_pInfo);
		Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("TRANSACTION_WORK"), _T("UPDATE"));
		Writer.addElement(_T("TRANSACTION_CODE"), strTransactionCode);
		Writer.addElement(_T("LOAN_KEY"),strLoanKey);
		Writer.addElement(_T("CANCEL"),_T("1"));

		m_pDM->GetCellData(_T("�ηϹ߼�����"), nIdx, strAppendixSendYN);
		m_pDM->GetCellData(_T("�ηϵ�Ϲ�ȣ����Ʈ"), nIdx, strAppendixRegNoList);
	
		if(strAppendixSendYN.Compare(_T("Y")) == 0){
			Writer.addElement(_T("APPENDIX_SEND_YN"), strAppendixSendYN);
			Writer.addElement(_T("APPENDIX_REGNO_LIST"), strAppendixRegNoList);
		}
		
		CFrameSender Sender(m_pInfo);	
		CFrameReader Reader; 
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
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
		
		if(_T("2")==strState)
		{
			if(_T("OK") != Reader.getElement(0,0))
			{
			CString strRefuseCode, strRefuseMsg;
			strRefuseCode = Reader.getElement(i,_T("REASON"));			
			strRefuseMsg = TransCodeToMsg(strRefuseCode);
			AfxMessageBox(strRefuseMsg);
			return -1;
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

//INT CKL_OTHER_LOAN_OFFER::SendRefuse(CString strMessage)
INT CKL_OTHER_LOAN_OFFER::SendRefuse()
{	
	INT ids = 0;	
	CString strState = GetComboBoxState();
	if(_T("0") != strState)
	{
		AfxMessageBox(_T("��û ���¿����� �߼۰����� �����մϴ�."));
		return -1;
	}	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}		
	else if(1 < m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("�������� ������ �Ͻ� ��� �Է��� �߼� ���� �޽����� �ϰ� ����˴ϴ�."));
	}	
	
	// 17/08/30 ������ : ����>����ݳ�>Ÿ������������� �߼۰����� �޸𸮴��� ���� �߻�
/*//BEFORE-----------------------------------------------------------------------------
	CKL_OTHER_LOAN_SEND_REFUSE dlg = new CKL_OTHER_LOAN_SEND_REFUSE(this);
	ids = dlg.DoModal();
	
	CString strMessage = _T("");
	INT nSuccess;
	// 16.10.20 JMJ KOL.RED.008
	BOOL bReasonSendYN = FALSE;
	
	
	nSuccess = dlg.m_nSuccess;
	strMessage = dlg.m_strMessage;
	// 16.10.20 JMJ KOL.RED.008
	bReasonSendYN = dlg.m_bMessageSendYN;
*///AFTER------------------------------------------------------------------------------
	CKL_OTHER_LOAN_SEND_REFUSE *dlg = new CKL_OTHER_LOAN_SEND_REFUSE(this);
	ids = dlg->DoModal();
	
	CString strMessage = _T("");
	INT nSuccess;
	// 16.10.20 JMJ KOL.RED.008
	BOOL bReasonSendYN = FALSE;
	
	nSuccess = dlg->m_nSuccess;
	strMessage = dlg->m_strMessage;
	// 16.10.20 JMJ KOL.RED.008
	bReasonSendYN = dlg->m_bMessageSendYN;
	if (dlg != NULL)
	{
		delete dlg;
		dlg = NULL;
	}
/*//END-------------------------------------------------------------------------------*/
	
	INT nIdx = m_pGrid->SelectGetTailPosition();
	
	// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
	// �߼۱Ǽ� ó��
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

	// 17/08/28 ������ : �ڰ������ �ڷḦ �߼۰����� �� ��û�������ΰ� ������ Y�� ����Ǵ� ���� ����
//*/ ADD ----------------------------------------------------------------------------------
	CString strBookKey = _T("");					
	CString strManageCode = m_pInfo->MANAGE_CODE;
	CString strLILLYN = _T("");
	CString strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
//*/ END ----------------------------------------------------------------------------------

	CString strLoanKey;
	if(1 == nSuccess)
	{
		for(int i=0;i<m_pGrid->SelectGetCount();i++)
		{
			m_pDM->GetCellData(_T("����KEY"), nIdx, strLoanKey);
			CFrameWriter Writer(m_pInfo);
			Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("TRANSACTION_WORK"), _T("UPDATE"));
			Writer.addElement(_T("TRANSACTION_CODE"), _T("2"));
			Writer.addElement(_T("LOAN_KEY"),strLoanKey);
			Writer.addElement(_T("SEND_REJECT_REASON"),strMessage);
			Writer.addElement(_T("CANCEL"),_T("0"));
			
			// 17/08/28 ������ : �ڰ������ �ڷḦ �߼۰����� �� ��û�������ΰ� ������ Y�� ����Ǵ� ���� ����
//*/ ADD ----------------------------------------------------------------------------------
			m_pDM->GetCellData(_T("åKEY"), nIdx, strBookKey);	
			if(strBookKey != _T("")) 
			{
				strQuery.Format(_T("SELECT GET_ASK_LILL_LOAN(WORKING_STATUS, REC_KEY, USE_LIMIT_CODE, SEPARATE_SHELF_CODE, REG_CODE, SHELF_LOC_CODE, REG_NO) FROM BO_BOOK_TBL WHERE REC_KEY ='%s' AND MANAGE_CODE ='%s' "), strBookKey, strManageCode);
				TmpDM.GetOneValueQuery(strQuery,strLILLYN);
				if(strLILLYN != _T("O"))
				{
					strLILLYN = _T("N");
				}
				else
				{
					strLILLYN = _T("Y");
				}
				Writer.addElement(_T("SERVICE_YN"),strLILLYN);
			}
//*/ END ----------------------------------------------------------------------------------

			CFrameSender Sender(m_pInfo);	
			CFrameReader Reader; 
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
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
				// ��������
				/*//BEFORE-----------------------------------------------------------------------
				ids = Refuse_SendSMS(nIdx);
				*///AFTER------------------------------------------------------------------------
				// 16.10.20 JMJ KOL.RED.008
				ids = Refuse_SendSMS(nIdx, strMessage, bReasonSendYN);
				/*//END------------------------------------------------------------------------*/
							
				m_pDM->DeleteRow(nIdx);
			}

			nIdx=m_pGrid->SelectGetPrev();
		}
		
		m_pGrid->Display();
		AfxMessageBox(_T("�߼� ���� �Ϸ�"));		
	}	

	// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
	// �߼۱Ǽ� ó��
	/*//NEW------------------------------------------------------------------------*/	
	m_listUserBookCnt.RemoveAll();
	/*//END------------------------------------------------------------------------*/
	return ids;
}

//�߼۰��� SMS ���ڹ߼�
// 16/03/02 ���湫 : 2016�� �����߾ӵ����� å��������
// �߼۰��� �����߰�
/*//BEFORE-----------------------------------------------------------------------
INT CKL_OTHER_LOAN_OFFER::Refuse_SendSMS(INT nIdx)
*///AFTER------------------------------------------------------------------------
INT CKL_OTHER_LOAN_OFFER::Refuse_SendSMS(INT nIdx, CString strReason, BOOL bReasonSendYN)
/*//END------------------------------------------------------------------------*/
{
	INT ids=0;
	CString strUserNo;
	CString strMessage;
	CString strQuery;
	m_pDM->GetCellData(_T("�̿��ڹ�ȣ"), nIdx, strUserNo);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
	// 15/12/10 �Լ��� ������ ���� ������ ��� �� �߼۱Ǽ� ó��
	/*//BEFORE-----------------------------------------------------------------------
	strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS13'"));
	*///AFTER------------------------------------------------------------------------
	CString strManageCode = m_pInfo->MANAGE_CODE;
	if (strManageCode.IsEmpty())
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS13'"));
	else
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS13' AND MANAGE_CODE='%s'"), strManageCode);

	int nBookCnt = 0;
	/*//END------------------------------------------------------------------------*/
	
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

		// 16/03/02 ���湫 : 2016�� �����߾ӵ����� å��������
		// �߼۰��� ���� �߰�
		/*//BEFORE-----------------------------------------------------------------------
		strMessage = strName+strMessage;
		*///AFTER------------------------------------------------------------------------
		//strMessage = strName+strMessage;		
		/*//END------------------------------------------------------------------------*/
				
		//SMS ��� ��
		if(_T("Y")==strSMSYN)
		{
			// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
			// �߼۱Ǽ� ó��
			/*//NEW------------------------------------------------------------------------*/
			if(!m_listUserBookCnt.Lookup(strUserNo, nBookCnt))
			{
				nBookCnt= 0;
			}

			if(nBookCnt > 0)
			{		
			/*//END------------------------------------------------------------------------*/
				//���ν��� ȣ��!
				CString strType = _T("SMS13");
				
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
				
				MakeSMS.MakeSMSMsg( strType , &SMS_MsgInfo , strName , _T(""), strTitleInfo , strBookCnt , strMessage );

				/*
				if(strReason.Compare(_T("")) != 0)
				{
					strMessage += _T(",����:")+strReason;
				}
				*/
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

				//JOB.2018.0093 ���� ���� �޼����� Ȯ���ؼ� �ٽ� ���� �� �ֵ��� ó��
				/*//BEFORE==========================================================================================
				if(strResult.Compare(_T("")) == 0)
				{
					ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);					
				}
				// 16.10.20 JMJ KOL.RED.008
				// ���� �����߼�
				if(strReason.Compare(_T("")) != 0 && bReasonSendYN == TRUE)
				{
					strMessage = _T("����:")+strReason;
					ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);					
				}			
				*///AFTER===========================================================================================
				if(strResult.Compare(_T("")) != 0)
				{
					if(IDYES == MessageBox(_T("���� �߼��� �޽����Դϴ�.\r\nSMS�� �߼��Ͻðڽ��ϱ�?"), _T("SMS�߼�Ȯ��"), MB_YESNO | MB_ICONQUESTION)) 
					{
						ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);
						if(strReason.Compare(_T("")) != 0 && bReasonSendYN == TRUE)
						{
							strMessage = _T("����:")+strReason;
							ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);					
						}
					}
				}
				else 
				{
					ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);	
					if(strReason.Compare(_T("")) != 0 && bReasonSendYN == TRUE)
					{
						strMessage = _T("����:")+strReason;
						ids = CallProcedure(strUserKey, strHandPhone, strName, strMessage, strType);					
					}
				}
				//================================================================================================

				/*//END------------------------------------------------------------------------*/
				if(0 > ids)
				{
					AfxMessageBox(_T("�޼��� �߼ۿ� �����Ͽ����ϴ�."));
				}
				// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
				// �߼۱Ǽ� ó��
				/*//NEW------------------------------------------------------------------------*/
				else
				{
					m_listUserBookCnt.RemoveKey(strUserNo);
				}
				/*//END------------------------------------------------------------------------*/						
			// 16/03/07 ���湫 : 2016�� �����߾ӵ����� å��������
			// �߼۱Ǽ� ó��
			/*//NEW------------------------------------------------------------------------*/
			}
			/*//END------------------------------------------------------------------------*/						
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

INT CKL_OTHER_LOAN_OFFER::CallProcedure(CString strUserNo, CString strHandPhone, CString strName, CString strMessage, CString strType)
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
	CString strCfgFilePath = _T("..\\Cfg\\SMS_Callback.cfg");
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

	ids = TmpDM.ExecuteQuery( sQuery, TRUE );
*/	
	if (g_strKakaoUsed1.IsEmpty())
	{
		sQuery.Format( 
			_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KAKAO�÷���ģ���ý�������' AND value_name='KAKAO�÷���ģ���������' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		TmpDM.GetOneValueQuery(sQuery, g_strKakaoUsed1);
		
		// �������� ���ٸ� N���� ����
		if (g_strKakaoUsed1.IsEmpty())
		{
			g_strKakaoUsed1 = _T("N");
		}
	}

	if (g_strKakaoUsed1.CompareNoCase(_T("Y")) == 0)
	{
		// īī�� �÷��� ģ�� �߼�
		sQuery.Format( _T("KAKAOPLUS_TOT_SEND_PROC(%s, %s, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');")
								, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	// 1,2,3,4,5
								, _T(""), m_pInfo->MANAGE_CODE, strType				// 6,7,8
								, m_pInfo->USER_NAME, sFirst_Work, sUserName,				    // 9,10
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
	ids = TmpDM.ExecuteQuery(sQuery, TRUE);
	// KOL.RED.2018.224 ---------------------------------------------------------------------------
	if(0>ids)
	{ 
		AfxMessageBox(_T("�޽��� �߼ۿ� �����Ͽ����ϴ�."));
	}
	return 0;
	
}

INT CKL_OTHER_LOAN_OFFER::SelectGrid(CString sRegNo)
{
	CString strRegNo;
	// 17/06/28 ������ : Ÿ������������� ���ڵ�� �Էµ� ��Ϲ�ȣ�� �׸��忡 �������� ������ �޽��� ó��
//*/ ADD ---------------------------------------------------------------------------
	BOOL bExsistRegNo = FALSE;
//*/ END ---------------------------------------------------------------------------	
	for(int i=0; i<m_pDM->GetRowCount();i++)
	{
		m_pDM->GetCellData(_T("��Ϲ�ȣ"), i, strRegNo);
		if(sRegNo == strRegNo)
		{
			// 16/06/21 �ڿ�� : 2016�⵵ ���������� ����������� ��Ź�
			// �÷� ���� ����� ���� �߻� ���� ����
			/*//BEFORE-----------------------------------------------------------------------
			m_pGrid->SetAt(i, 0, _T("V"));
			m_pGrid->DisplayLine(i);
			*///AFTER------------------------------------------------------------------------
			m_pGrid->SetAt(i, m_pGrid->m_nSelectIdx, m_pGrid->m_sSelectString);
			m_pGrid->m_nReverse = i;
			m_pGrid->SetReverse(i);
			/*//END------------------------------------------------------------------------*/

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
//*/ END --------------------------------------------------------------------------

	// 17/05/19 ������ : Ÿ���������/Ÿ���ݳ������� GRID �����÷����� ����
//*/ ADD ---------------------------------------------------------------------------
	m_pDM->SORT(_T("����"),TRUE);
	m_pGrid->Display();
//*/ END ---------------------------------------------------------------------------
	return 0;
}

CString CKL_OTHER_LOAN_OFFER::TransCodeToMsg(CString transCode)
{
	CString msg;

	if(_T("100") == transCode)
	{
		msg = _T("����ȸ���� �ƴմϴ�.");
	}
	else if(_T("200") == transCode)
	{
		msg = _T("��û�ϴ� �������� ȸ�������� �Ǿ� ���� �ʽ��ϴ�.");
	}
	else if(_T("300") == transCode)
	{
		// 2015�� ���������� �ý��� ���� 2015/12/11 : ���յ������� > å��������
		msg = _T("å���� �ִ� ����Ǽ��� �ʰ��Ͽ����ϴ�.");
	}
	else if(_T("400") == transCode)
	{
		msg = _T("���� ������ ������ �� �ִ� ���°� �ƴմϴ�.");
	}
	else if(_T("500") == transCode)
	{
		msg = _T("�ش絵������ ��������ȸ������ ��ϵǾ� �ֽ��ϴ�.");
	}
	else if(_T("600") == transCode)
	{
		msg = _T("�ش絵������ ����ȸ������ ��ϵǾ� �ֽ��ϴ�.");
	}
	else if(_T("700") == transCode)
	{
		msg = _T("�ش絵������ Ż��ȸ������ ��ϵǾ� �ֽ��ϴ�.");
	}
	else if(_T("800") == transCode)
	{
		msg = _T("�ش絵������ ��ü���� �ڷᰡ �ֽ��ϴ�.");
	}
	else if(_T("900") == transCode)
	{
		msg = _T("�ش絵�������� �����ִ�ġ�� �ʰ��Ͽ����ϴ�.");
	}
	else
	{
		msg = _T("�� �� ���� ����");
	}
	return msg;
}

// 16/02/01 ���湫 : 2016�� �����߾ӵ����� å��������
// ����ڸ� ��������
/*//NEW------------------------------------------------------------------------*/
CString CKL_OTHER_LOAN_OFFER::GetUserName(CString strUserNo)
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
CString CKL_OTHER_LOAN_OFFER::GetBirthDay(CString strUserNo)
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
CString CKL_OTHER_LOAN_OFFER::GetUserClassDesc(CString strUserNo)
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

// 16/02/04 ���湫 : 2016�� �����߾ӵ����� å��������
// ����� ���
/*//NEW------------------------------------------------------------------------*/
BOOL CKL_OTHER_LOAN_OFFER::PrintWaybill()
{
EFS_BEGIN

	CESL_DataMgr* pSrcDM = FindDM(_T("DM_KL_OTHER_LOAN_MNG_OFFER"));
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
	
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_KL_OTHER_LOAN_MNG_OFFER") , _T("GRID") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	//INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();

	if(nSelectCnt <= 0)
	{
		AfxMessageBox( _T("���� ������ �����ϴ�.") );
		return 1;
	}

	/*
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	*/

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_KL_OTHER_LOAN_MNG_OFFER_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
	pPrintDM->FreeData();

	const INT nCopyCnt = 10;
	// 17/11/06 ������ : ��ȣ���� Ÿ��������������� �÷��� �ѱ۷� ��ü
/*//BEFORE-----------------------------------------------------------------------------
	TCHAR *sField[nCopyCnt][2] =
	{
		{_T("USER_NO"),_T("USER_NO")},
		{_T("USER_NAME"),_T("USER_NAME")},
		{_T("REG_NO"),_T("REG_NO")},
		{_T("TITLE_INFO"),_T("TITLE_INFO")},
		{_T("PUB_INFO"),_T("PUB_INFO")},
		{_T("CALL_NO"),_T("CALL_NO")},
		{_T("SHELF_LOC_DESC"),_T("SHELF_LOC_DESC")},
		{_T("APPENDIX_RCT_YN"),_T("APPENDIX_RCT_YN")},
		{_T("APPENDIX_SEND_YN"),_T("APPENDIX_SEND_YN")},
		{_T("MEMO"),_T("MEMO")},		
	};
*///AFTER------------------------------------------------------------------------------
	TCHAR *sField[nCopyCnt][2] =
	{
		{_T("�̿��ڹ�ȣ"),_T("USER_NO")},
		{_T("�̿��ڸ�"),_T("USER_NAME")},
		{_T("��Ϲ�ȣ"),_T("REG_NO")},
		{_T("����"),_T("TITLE_INFO")},
		{_T("������"),_T("PUB_INFO")},
		{_T("û����ȣ"),_T("CALL_NO")},
		{_T("�ڷ�Ǹ�"),_T("SHELF_LOC_DESC")},
		{_T("�ηϽ�û����"),_T("APPENDIX_RCT_YN")},
		{_T("�ηϹ߼�����"),_T("APPENDIX_SEND_YN")},
		{_T("�޸�"),_T("MEMO")},		
	};
/*//END-------------------------------------------------------------------------------*/
	
	CString sData, sData2;
	INT nIndex;
	INT ids;

	pGrid->SelectGetHeadPosition();


	std::map<CString, CString> mapLib;
	std::map<CString, CString>::iterator iter;
	
	CString strDate = pSrcDM->GetDate().Left(10);
	
	for( INT i = 0 ; i < nSelectCnt ; i++)
	{
		nIndex = pGrid->SelectGetIdx();

		ids = pSrcDM->GetCellData( _T("��û��������ȣ") , nIndex , sData );

		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );

		ids = pSrcDM->GetCellData( _T("��û������") , nIndex , sData2 );

		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );

		mapLib.insert(std::map<CString, CString>::value_type(sData, sData2));
				
		pGrid->SelectGetNext();
	}

	for(iter = mapLib.begin(); iter != mapLib.end(); iter++)
	{
		int nRowNum = 0;

		pPrintDM->FreeData();

		pGrid->SelectGetHeadPosition();

		for( i = 0 ; i < nSelectCnt ; i++ )
		{
			/*
			if( nMode == 0 )
				nIndex = i;
			else
				nIndex = pGrid->SelectGetIdx();
			*/
			nIndex = pGrid->SelectGetIdx();

			ids = pSrcDM->GetCellData( _T("��û��������ȣ") , nIndex , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );			

			if(sData.Compare(iter->first) != 0)
			{
				pGrid->SelectGetNext();
				continue;
			}

			pPrintDM->InsertRow(-1);
			
			sData.Format( _T("%d") , nRowNum+1 );
			ids = pPrintDM->SetCellData( _T("��ȣ") , sData , nRowNum);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );			
			
			for( INT j = 0 ; j < nCopyCnt ; j++ )
			{
				ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
				if( ids < 0 ) 
				{
					CString sEMsg;
					sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
					AfxMessageBox( sEMsg );
				}
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
				
				ids = pPrintDM->SetCellData( sField[j][1] , sData , nRowNum );
				if( ids < 0 ) 
				{
					CString sEMsg;
					sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nRowNum , sField[j][0] , sData );
					AfxMessageBox( sEMsg );
				}
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
			}

			//if( nMode == 1)
			pGrid->SelectGetNext();
			nRowNum ++;
		}

		pPrintDM->SORT(_T("USER_NO"));
		pPrintDM->SORT(_T("USER_NAME"));

		for(i=0; i<pPrintDM->GetRowCount();i++)
		{
			sData.Format( _T("%d") , i+1 );
			ids = pPrintDM->SetCellData( _T("��ȣ") , sData , i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );	
		}

		CString strAcqYear, strGroupInfo;
		CBasicPrintMgr PrintMgr(this);
		PrintMgr.InitPrintData();
		
		PrintMgr.AddPrintData(_T("HOLD_LIB"), m_pInfo->LIB_NAME);
		PrintMgr.AddPrintData(_T("LOAN_LIB"), iter->second);
		PrintMgr.AddPrintData(_T("SEND_DATE"), strDate);
		PrintMgr.SetPrintDM(pPrintDM);
		PrintMgr.SetPrintType(0);
		PrintMgr.SetSIReportFilename(_T("��������"));
		//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
		PrintMgr.SetPrivacyPrintLogParm( TRUE, m_pInfo->USER_PK, m_pInfo->USER_ID, _T("Ÿ��������� ��������"), m_pInfo->MANAGE_CODE, _T("Ÿ��������� ��������"),	0 );

		PrintMgr.Print();
	}	

	mapLib.clear();
	
	return TRUE;
EFS_END
		
	return FALSE;
}
/*//END------------------------------------------------------------------------*/

// 16/02/19 ���湫 : 2016�� �����߾ӵ����� å��������
// �޸����
/*//NEW------------------------------------------------------------------------*/
void CKL_OTHER_LOAN_OFFER::ModifyMemo()
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


// 16/03/02 ���湫 : 2016�� �����߾ӵ����� å��������
// �����߼�
/*//NEW------------------------------------------------------------------------*/
INT CKL_OTHER_LOAN_OFFER::SendBook_SendSMS(INT nIdx)
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
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS14'"));
	else
		strQuery.Format(_T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE='SMS14' AND MANAGE_CODE='%s'"), strManageCode);
	
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
						
		//SMS ��� ��
		if(_T("Y")==strSMSYN)
		{
			if(!m_listUserBookCnt.Lookup(strUserNo, nBookCnt))
			{
				nBookCnt= 0;
			}

			if(nBookCnt > 0)
			{
				if(IDYES == MessageBox(_T("�߼� ó���� �Ϸ�Ǿ����ϴ�.\r\nSMS�� �߼��Ͻðڽ��ϱ�?"), _T("SMS�߼�Ȯ��"), MB_YESNO | MB_ICONQUESTION)) 
				{

					//���ν��� ȣ��!
					CString strType = _T("SMS14");
								
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


// 16/02/19 ���湫 : 2016�� �����߾ӵ����� å��������
// �̿��ڻ󼼺���
/*//NEW------------------------------------------------------------------------*/
void CKL_OTHER_LOAN_OFFER::ViewLoanUser()
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