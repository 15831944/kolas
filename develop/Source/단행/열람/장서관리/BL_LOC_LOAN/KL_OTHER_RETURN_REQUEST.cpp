// KL_OTHER_RETURN_REQUEST.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_RETURN_REQUEST.h"
#include "FrameManager.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"
#include "KL_OTHER_RETURN_DELIVERY.h"

#include "..\\..\\..\\..\\����\\BO_ACQ_PRINT_MGR\\BasicPrintMgr.h"
#include <map>
#include "KL_OTHER_RETURN_EDIT_MEMO.h"
#include "KL_OTHER_RETURN_DETAILVIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_REQUEST dialog


CKL_OTHER_RETURN_REQUEST::CKL_OTHER_RETURN_REQUEST(CESL_Mgr* pParent /*=NULL*/)
	: CKL_OTHER_RETURN_DEFAULT(pParent)
{
	//{{AFX_DATA_INIT(CKL_OTHER_RETURN_REQUEST)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strSMAlias = _T("SM_KL_OTHER_RETURN_MNG_REQUEST");
	m_strCMAlias = _T("CM_KL_OTHER_RETURN_MNG_REQUEST");
	m_strDMAlias = _T("DM_KL_OTHER_RETURN_MNG_REQUEST");
	m_pParent = pParent;
	m_API = new CGipyoPrintAPI();
}

CKL_OTHER_RETURN_REQUEST::~CKL_OTHER_RETURN_REQUEST()
{
	m_API->FreePrintLibrary();
	delete m_API;
	m_API = NULL;
}

BOOL CKL_OTHER_RETURN_REQUEST::Create(CWnd* pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

void CKL_OTHER_RETURN_REQUEST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_RETURN_REQUEST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_RETURN_REQUEST, CKL_OTHER_RETURN_DEFAULT)
	//{{AFX_MSG_MAP(CKL_OTHER_RETURN_REQUEST)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_cmb, OnSelchangecmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_REQUEST message handlers

BOOL CKL_OTHER_RETURN_REQUEST::OnInitDialog() 
{
	CKL_OTHER_RETURN_DEFAULT::OnInitDialog();
	
	ResetComboBox();
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)FindControl(	m_strCMAlias, _T("��Ƽ�޺�_�������"));
	if(NULL == pCMB) return FALSE;	
	
	CString strData = _T("");
	int ROW = pCMB->GetCount();
	for(int nCnt = 0; nCnt < ROW; nCnt++)
	{
		strData = pCMB->GetItemDataString(nCnt, 1);
		if(_T("�ݳ�") == strData)
		{
			pCMB->SetCurSel(nCnt);
			break;
		}
	}

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("Ÿ���ݳ������߼�");
	}

	if (m_pLoanManageValue->m_sLoanUserAccessMode.IsEmpty() || m_pLoanManageValue->m_sLoanUserAccessMode == _T("1") )
	{
		m_pGrid->SetColumnEncryption(_T("�̿��ڸ�"),TRUE);
		m_pGrid->SetColumnEncryption(_T("�������"),TRUE);
	}

	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKL_OTHER_RETURN_REQUEST::ResizeControl()
{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_grid);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return FALSE;
	
	CRect rDlg = CRect(0, 0, 0, 0);
	GetClientRect(rDlg);

	rDlg.top += 70;
	
	pWnd->MoveWindow(rDlg);

	return TRUE;
}

INT CKL_OTHER_RETURN_REQUEST::ResetComboBox()
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

	const int STATE_COUNT = 3;
	CString strCmbData[STATE_COUNT][COL_COUNT] = 
	{		
		{ _T("0"), _T("�ݳ�")},
		{ _T("1"), _T("�߼�")},
		{ _T("2"), _T("����")}
	};

	m_strDescription[0] = _T("�ڰ����� Ÿ�� å�� �ݳ��Ͽ� �߼� ������� ���");
	m_strDescription[1] = _T("�ڰ����� Ÿ�� å�� �ݳ��Ͽ� ���嵵�������� �߼��� ���");
	m_strDescription[2] = _T("�ڰ����� Ÿ�� å�� �ݳ��Ͽ� ���嵵�������� �߼۵� �ڷᰡ ���͵� ���");

	for(int nRow = 0; nRow < STATE_COUNT; nRow++)
	{
		for(int nCol = 0; nCol < 2; nCol++)
		{
			pCMB->AddItem(strCmbData[nRow][nCol], nCol, nRow);
		}
	}

	return 0;
}

void CKL_OTHER_RETURN_REQUEST::OnSize(UINT nType, int cx, int cy) 
{
	CKL_OTHER_RETURN_DEFAULT::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeControl();
}

INT CKL_OTHER_RETURN_REQUEST::Return()
{
	int ids = 0;
	CString strFuncName = _T("CKL_OTHER_RETURN_REQUEST::Return()");

	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}
		
	CStringArray aCheckArray;
	aCheckArray.Add(_T("Ʈ�����ǻ���|!=|0"));
	aCheckArray.Add(_T("�ݳ���������ȣ|!=|") + m_pInfo->LIB_CODE);
	
	ids = UnCheckGrid(aCheckArray);
		
	}
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("������ �׸��� �߼� ������ �׸��� �����ϴ�.\r\n")
			_T("�߼��� �Ұ����� ����� �Ʒ��� �����ϴ�.\r\n\r\n")			
			_T("1. ������°� �ݳ�(0)�� �ƴ� �ڷ�\r\n")
			);
		return -1;
	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
	
	CFrameWriter Writer(m_pInfo);
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	CString strData;
	
	m_pGrid->SelectMakeList();
	if(0 < m_pGrid->SelectGetCount())
	{
		CString strReturnDate, strLoanLibCode, strLoanKey;
		CTime Today = CLocCommonAPI::GetCurrentDateTime(this, strReturnDate);

		INT nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{
			Writer.clear();
			Writer.setCommand(_T("IL_K19_LOC_B01_SERVICE"));
			Writer.addRecord();
 			Writer.addElement(_T("Ʈ�����ǻ���"), _T("1"));
			strLoanLibCode = m_pDM->GetCellData(_T("���⵵������ȣ"), nIdx);
 			Writer.addElement(_T("���⵵����"), strLoanLibCode);
			strLoanKey = m_pDM->GetCellData(_T("����KEY"), nIdx);
			Writer.addElement(_T("����KEY"), strLoanKey);
 			Writer.addElement(_T("Ÿ���ݳ��߼���"), strReturnDate);
 			Writer.addElement(_T("������"), _T(""));
 			Writer.addElement(_T("�߼���"), m_pInfo->USER_ID);
 			Writer.addElement(_T("�Լ���"), _T(""));
			Writer.addElement(_T("�������۾�"), GetWorkLogMsg(_T("�߼�"),__WFILE__ ,__LINE__));

			BOOL bSuccese = FALSE;
			Reader.clear();
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if(0 > ids)
			{
				 bSuccese = FALSE;
			}
			else
			{
 				CESL_DataMgr TempDM;
 				Reader.makeDM(&TempDM);
 				CString strTemp = Reader.getCommand();
 				if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
				{
					bSuccese = FALSE;
				}
				else
				{
					TempDM.GetCellData(_T("ó�����"), 0, strData);
					if(_T("OK") == strData)
					{
						bSuccese = TRUE;
					}
					else
					{
						bSuccese = FALSE;
					}
				}
			}
			if(TRUE == bSuccese)
			{
				m_pDM->SetCellData(_T("ó�����"), _T("����"), nIdx);
			}
			else
			{
				m_pDM->SetCellData(_T("ó�����"), _T("����"), nIdx);
			}
			nIdx = m_pGrid->SelectGetNext();
		}
		while(0 <= nIdx);
	}

	if(0 < m_pGrid->SelectGetCount())
	{
		INT nIdx = m_pGrid->SelectGetTailPosition();
		do
		{
			m_pDM->GetCellData(_T("ó�����"), nIdx, strData);
			if(_T("����") == strData)
			{
				m_pDM->DeleteRow(nIdx);
			}
			nIdx = m_pGrid->SelectGetPrev();
		}
		while(0 <= nIdx);
	}
	m_pGrid->Display();

	return ids;
}

INT CKL_OTHER_RETURN_REQUEST::ReturnCancel()
{
	int ids = 0;
	CString strFuncName = _T("ReturnCancel");
	CString strState = GetComboBoxState();
	if(_T("0") != strState && _T("1") != strState)
	{
		AfxMessageBox(_T("�߼��ڷ���������� �ݳ��� �߼��� ���¸� ��� �۾��� �����մϴ�."));
		return -1;
	}
	m_pGrid->SelectMakeList();
	if(1 > m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}
	if(1 < m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("�۾���Ҵ� �ѰǾ� ó�������մϴ�.\r\n����� �ڷ� �ϳ��� �������ֽʽÿ�."));
		return -1;
	}
	INT nIdx = m_pGrid->SelectGetHeadPosition();
	CString strReturnPlanDate = m_pDM->GetCellData(_T("�ݳ�������"), nIdx);

	CString strReturnLib = m_pDM->GetCellData(_T("�ݳ�������"), nIdx);
	CString strOtherLoanKey = m_pDM->GetCellData(_T("Ÿ������KEY"), nIdx);
	if(strReturnLib.IsEmpty())
	{
		ids = ReturnCancel_Send(strOtherLoanKey);
		return 0;
	}

	if(10 > strReturnPlanDate.GetLength())
	{
		AfxMessageBox(_T("�ݳ������Ͽ� ������ �ֽ��ϴ�."));
		return -1;
	}
	CString strReturnDate = m_pDM->GetCellData(_T("�ݳ���"), nIdx);
	if(10 > strReturnDate.GetLength())
	{
		AfxMessageBox(_T("�ݳ��Ͽ� ������ �ֽ��ϴ�."));
		return -1;
	}

	CString strCurrentDate;
	CTime t = CLocCommonAPI::GetCurrentDateTime(this, strCurrentDate);
	if(_T("0") == strState)
	{
		if(strReturnDate.Mid(0,10) != strCurrentDate.Mid(0,10))
		{
			AfxMessageBox(_T("���� ó���� �ڷḸ ��� �۾��� �����մϴ�."));
			return -1;
		}
	}
	else
	{
		CString strRequestDate = m_pDM->GetCellData(_T("Ÿ���ݳ��߼���"), nIdx);
		if(10 > strRequestDate.GetLength())
		{
			AfxMessageBox(_T("�߼��Ͽ� ������ �ֽ��ϴ�."));
			return -1;
		}
		if(strRequestDate.Mid(0,10) != strCurrentDate.Mid(0,10))
		{
			AfxMessageBox(_T("���� ó���� �ڷḸ ��� �۾��� �����մϴ�."));
			return -1;
		}

	}

	if(_T("0") == strState)
	{
		CString strDate1 = strReturnPlanDate.Left(10);
		strDate1.Replace(_T("/"), _T(""));
		CString strDate2 = strReturnDate.Left(10);
		strDate2.Replace(_T("/"), _T(""));
		if(strDate1 < strDate2)
		{
			AfxMessageBox(_T("��ü �ڷ�� Ÿ���ݳ� ����� �� �����ϴ�."));
			return -1;
		}
	}

	CString strOfferLibCode;
	strOfferLibCode = m_pDM->GetCellData(_T("������������ȣ"),nIdx);
	if(!strOfferLibCode.IsEmpty())
	{
		CString strLoanKey;
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
		CFrameWriter Writer(m_pInfo);
		CFrameSender Sender(m_pInfo);
		CFrameReader Reader;
		CString strData;
		CString strReturnLibCode, strLoanLibCode;

		CString strTargetStatus = _T("");
		if ( strState == _T("0") ) {
			strTargetStatus = _T("4");
		}
		else if ( strState == _T("1") ) {
			strTargetStatus = _T("5");
		}

		Writer.clear();
		Writer.setCommand(_T("LILL_K02_LOC_B02_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("LOAN_KEY"), strOtherLoanKey);
		Writer.addElement(_T("TRANSACTION_CODE"), strTargetStatus);
		
		BOOL bSuccese = FALSE;
		Reader.clear();
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0 > ids)
		{
			bSuccese = FALSE;
		}
		else
		{
			CESL_DataMgr TempDM;
			Reader.makeDM(&TempDM);
			CString strTemp = Reader.getCommand();
			if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
			{
				bSuccese = FALSE;
			}
			else
			{
				TempDM.GetCellData(_T("RESULT"), 0, strData);
				if(_T("OK") == strData)
				{
					bSuccese = TRUE;
				}
				else
				{
					bSuccese = FALSE;
				}
			}
		}
		if(TRUE == bSuccese)
		{
			m_pDM->SetCellData(_T("ó�����"), _T("����"), nIdx);
			Refresh();

			if ( strState == _T("0") ) {
				AfxMessageBox(_T("Ÿ���ݳ��� ��� �Ǿ����ϴ�."));
			}
			else if ( strState == _T("1") ) {
				AfxMessageBox(_T("�߼��� ��� �Ǿ����ϴ�."));
			}			
		}
		else
		{
			m_pDM->SetCellData(_T("ó�����"), _T("����"), nIdx);
			m_pGrid->Display();
		}
		
		
		
	}
	else
	{
		
		
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
		CFrameWriter Writer(m_pInfo);
		CFrameSender Sender(m_pInfo);
		CFrameReader Reader;
		CString strData;
		CString strReturnLibCode, strLoanLibCode, strLoanKey;
		Writer.clear();
		Writer.setCommand(_T("IL_K41_LOC_B01_SERVICE"));
		Writer.addRecord();
		strLoanLibCode = m_pDM->GetCellData(_T("���⵵������ȣ"), nIdx);
		Writer.addElement(_T("���⵵������ȣ"), strLoanLibCode);
		strReturnLibCode = m_pDM->GetCellData(_T("�ݳ���������ȣ"), nIdx);
		Writer.addElement(_T("�ݳ���������ȣ"), strReturnLibCode);
		CString strPaymentKey = m_pDM->GetCellData(_T("����ó��KEY"), nIdx);
		strLoanKey = m_pDM->GetCellData(_T("����KEY"), nIdx);
		Writer.addElement(_T("����KEY"), strLoanKey);
		if(!strPaymentKey.IsEmpty()) Writer.addElement(_T("����ó��KEY"), strPaymentKey);
		Writer.addElement(_T("Ʈ������"), strState);
		Writer.addElement(_T("�������۾�"), GetWorkLogMsg(_T("Ÿ���ݳ��������"),__WFILE__ ,__LINE__));
		BOOL bSuccese = FALSE;
		Reader.clear();
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0 > ids)
		{
			bSuccese = FALSE;
		}
		else
		{
			CESL_DataMgr TempDM;
			Reader.makeDM(&TempDM);
			CString strTemp = Reader.getCommand();
			if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
			{
				bSuccese = FALSE;
			}
			else
			{
				TempDM.GetCellData(_T("ó�����"), 0, strData);
				if(_T("OK") == strData)
				{
					bSuccese = TRUE;
				}
				else
				{
					bSuccese = FALSE;
				}
			}
		}
		if(TRUE == bSuccese)
		{
			m_pDM->SetCellData(_T("ó�����"), _T("����"), nIdx);
			Refresh();
			if ( strState == _T("0") ) {
				AfxMessageBox(_T("Ÿ���ݳ��� ��� �Ǿ����ϴ�."));
			}
			else if ( strState == _T("1") ) {
				AfxMessageBox(_T("�߼��� ��� �Ǿ����ϴ�."));
			}			
		}
		else
		{
			m_pDM->SetCellData(_T("ó�����"), _T("����"), nIdx);
			m_pGrid->Display();
		}
	}

	return ids;
}

void CKL_OTHER_RETURN_REQUEST::OnSelchangecmb() 
{
	// TODO: Add your control notification handler code here
	Refresh();
}


INT CKL_OTHER_RETURN_REQUEST::Refresh()
{
	CString strState = GetComboBoxState();

	int nState = _wtoi(strState);
	if(nState < 0)
	{
		nState = 0;		
	}
	SetDlgItemText(IDC_stext_state_description,m_strDescription[nState]);	

	BOOL bShow = FALSE;
	if(_T("0") == strState || _T("1") == strState) bShow = TRUE;
	m_pParent->SendMessage(WM_KL_RETURN_CANCEL_CTL, NULL, (LPARAM)bShow);
	m_pParent->SendMessage(WM_KL_ACTIVE_BUTTON, NULL, (LPARAM)nState);
	
	SetDateControlView();

	if(m_bIsSearched)
	{
		return 1000;
	}	

	m_bIsSearched = TRUE;
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K18_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("Ʈ�����ǻ���"), strState);
 	Writer.addElement(_T("�ݳ�������"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("������ȣ�������⵵����"), m_pInfo->LIB_CODE);
	if(_T("0") == strState)
	{
		Writer.addElement(_T("ORDER_BY"), _T("RETURN_DATE DESC"));
	}
	else if(_T("1") == strState)
	{
		Writer.addElement(_T("ORDER_BY"), _T("TR_SEND_DATE DESC"));
	}
	else if(_T("2") == strState)
	{
		Writer.addElement(_T("�����Ϻ���"), GetBeforeDate());
		Writer.addElement(_T("�����ϱ���"), GetAfterDate());
		Writer.addElement(_T("ORDER_BY"), _T("TR_RETURN_DATE DESC"));
	}
	CString szData = Writer.getFrame();
	CFrameSender Sender(m_pInfo);	
	Sender.CommMiddleWareDaemon(szData, szData);
	m_pDM->FreeData();
	CFrameReader Reader;
	Reader.parseFrame(szData);
	Reader.makeDM(m_pDM);
	SetDescDM();
	for(int i = 0; i<10; i++)
	{
	CString strReturnDate= m_pDM->GetCellData(_T("�ݳ���"), i);
	CString strLocalKey= m_pDM->GetCellData(_T("����KEY"), i);
	TRACE(strReturnDate);
	TRACE(strLocalKey);
}

	CString strUserNo = _T("");
	for(i=0; i<m_pDM->GetRowCount(); i++)
	{
		strUserNo = m_pDM->GetCellData(_T("�̿��ڹ�ȣ"), i);
		if(strUserNo.Compare(_T(""))!=0)
		{
			m_pDM->SetCellData(_T("�̿��ڸ�"), GetUserName(strUserNo), i);
			m_pDM->SetCellData(_T("�������"), GetBirthDay(strUserNo), i);
			m_pDM->SetCellData(_T("���������޼���"), GetUserClassDesc(strUserNo), i);
		}		
	}

	SetTimer(1, 100, NULL);

	return 0;
}

INT CKL_OTHER_RETURN_REQUEST::DoRefresh()
{
	if(m_bIsSearched)
	{
		return 1000;
	}
	EnableWindow(FALSE);

	AfxBeginThread(OtherReturnRequestThread, this);

	return 0;
}

UINT CKL_OTHER_RETURN_REQUEST::OtherReturnRequestThread(LPVOID lParam)
{
	CKL_OTHER_RETURN_REQUEST* pDlg = (CKL_OTHER_RETURN_REQUEST*)lParam;
	pDlg->Refresh();

	return 0;
}


INT CKL_OTHER_RETURN_REQUEST::PrintGipyo()
{
	int ids = 0;
	CString strFuncName = _T("CKL_OTHER_RETURN_REQUEST::PrintGipyo()");

	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}

	AfxMessageBox(_T("������ �׸��� �������� ���� �׸��� ��µ��� ������, ����ó��KEY�� ���� �׸��� �ϰ� ��� �˴ϴ�."));
	
	m_pGrid->SelectMakeList();
	INT nIdx = m_pGrid->SelectGetHeadPosition();	
	   
	CString strarrData[57];

	while(0 <= nIdx)
	{
		
		CString strPaymentKey = m_pDM->GetCellData(_T("����ó��KEY"), nIdx);
		if(_T("") == strPaymentKey) 
		{
			continue;	
		}
		else 
		{

			CFrameWriter Writer(m_pInfo);

			Writer.setCommand(_T("IL_K37_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("����ó��KEY"), strPaymentKey);
			Writer.addElement(_T("���"), _T("Request"));

			CFrameReader Reader;
			CFrameSender Sender(m_pInfo);
			INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if(0 > ids)
			{
				//�����ڵ� ó��
				CString strMsg = Sender.getError(ids);
				if(strMsg.IsEmpty())
				{
					AfxMessageBox(_T("�̵���� ��� ����"));
				}
				else if(_T("[MW95TCTB01DT01]") == strMsg.Right(16))
				{
					AfxMessageBox(_T("��Ȯ���� ���� ��������ȣ�Դϴ�."));
				}
				else if(_T("[MK95LOCR01DB01]") == strMsg.Right(16))
				{
					AfxMessageBox(_T("�˻������ �����ϴ�."));
				}		
				else
				{
					AfxMessageBox(_T("Error!!!"));
				}
				return -100;
			}

			CESL_DataMgr TmpDM;
			TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
			ids = Reader.makeDM(&TmpDM);
			
			CString strData;
			INT nInsertRow = 0;
			INT nRowCnt = TmpDM.GetRowCount();
			CString strTemp = Reader.getCommand();

			if(1 > TmpDM.GetRowCount())
			{
				return -200;
			}		
			

			CString strBook1 = TmpDM.GetCellData(_T("��ǰ��1"), 0);
			CString strBook2 = TmpDM.GetCellData(_T("��ǰ��2"), 0);
			CString strBook3 = TmpDM.GetCellData(_T("��ǰ��3"), 0);
			CString strBook4 = TmpDM.GetCellData(_T("��ǰ��4"), 0);
			CString strBook5 = TmpDM.GetCellData(_T("��ǰ��5"), 0);
			CString strGood1, strGood2;

			if(_T("") != strBook1 && _T("") == strBook2 && _T("") == strBook3 && _T("") == strBook4 && _T("") == strBook5)
			{
				strGood1 = strBook1;
				strGood2 = _T("");
			}
			else if(_T("") != strBook1 && _T("") != strBook2 && _T("") == strBook3 && _T("") == strBook4 && _T("") == strBook5)
			{
				strGood1 = strBook1 + _T(",") + strBook2;
				strGood2 = _T("");
			}
			else if(_T("") != strBook1 && _T("") != strBook2 && _T("") != strBook3 && _T("") == strBook4 && _T("") == strBook5)
			{
				strGood1 = strBook1 + _T(",") + strBook2 + _T(",") + strBook3;
				strGood2 = _T("");
			}
			else if(_T("") != strBook1 && _T("") != strBook2 && _T("") != strBook3 && _T("") != strBook4 && _T("") == strBook5)
			{
				strGood1 = strBook1 + _T(",") + strBook2 + _T(",") + strBook3;
				strGood2 = strBook4;
			}
			else if(_T("") != strBook1 && _T("") != strBook2 && _T("") != strBook3 && _T("") != strBook4 && _T("") != strBook5)
			{
				strGood1 = strBook1 + _T(",") + strBook2 + _T(",") + strBook3;
				strGood2 = strBook4 + _T(",") + strBook5;
			}
			else
			{
				return -300;
			}

			strarrData[0] = TmpDM.GetCellData(_T("�ֹ��ڼ���"), 0);
			strarrData[1] = TmpDM.GetCellData(_T("�ֹ�����ȭ��ȣ"), 0);
			strarrData[2] = TmpDM.GetCellData(_T("�ֹ��ڱ�Ÿ��ȭ��ȣ"), 0);
			strarrData[3] = TmpDM.GetCellData(_T("�ֹ��ڿ����ȣ"), 0);
			strarrData[4] = TmpDM.GetCellData(_T("�ֹ����ּ�"), 0);
			strarrData[5] = TmpDM.GetCellData(_T("�ֹ��ڻ��ּ�"), 0);
			strarrData[6] = TmpDM.GetCellData(_T("�����μ���"), 0);
			strarrData[7] = TmpDM.GetCellData(_T("��������ȭ��ȣ"), 0);
			strarrData[8] = TmpDM.GetCellData(_T("�����α�Ÿ��ȭ��ȣ"), 0);
			strarrData[9] = TmpDM.GetCellData(_T("�����ο����ȣ"), 0);
			strarrData[10] = TmpDM.GetCellData(_T("�������ּ�"), 0);
			strarrData[11] = TmpDM.GetCellData(_T("�����λ��ּ�"), 0);
			strarrData[12] = TmpDM.GetCellData(_T("�����߷�"), 0);
			strarrData[13] = TmpDM.GetCellData(_T("��������"), 0);
			strarrData[14] = TmpDM.GetCellData(_T("��ۺ�"), 0);
			strarrData[15] = _T("16");									
			strarrData[16] = TmpDM.GetCellData(_T("����ȣ"), 0);
			strarrData[17] = strGood1;									
			strarrData[18] = strGood2;									
			strarrData[19] = TmpDM.GetCellData(_T("��۸޽���"), 0);
			strarrData[20] = TmpDM.GetCellData(_T("��۸޽���"), 0);
			strarrData[21] = _T("");									
			strarrData[22] = _T("1");									
			strarrData[23] = _T("");									
			strarrData[24] = TmpDM.GetCellData(_T("�ֹ��ڼ���"), 0);
			strarrData[25] = _T("");								    
			strarrData[26] = _T("");							 			
			strarrData[27] = _T("");							 			
			strarrData[28] = _T("");										
			strarrData[29] = _T("");										
			strarrData[30] = _T("");										
			strarrData[31] = _T("");										
			strarrData[32] = _T("");										
			strarrData[33] = TmpDM.GetCellData(_T("�ֹ�����ȭ��ȣ"), 0);	
			strarrData[34] = _T("");										
			strarrData[35] = TmpDM.GetCellData(_T("�ֹ����ּ�"), 0);
			strarrData[36] = TmpDM.GetCellData(_T("�ֹ��ڻ��ּ�"), 0);
			strarrData[37] = _T("");										
			strarrData[38] = _T("");										
			strarrData[39] = _T("");										
			strarrData[40] = _T("");										
			strarrData[41] = _T("");										
			strarrData[42] = _T("");										
			strarrData[43] = _T("");										
			strarrData[44] = _T("");										
			strarrData[45] = _T("");										
			strarrData[46] = _T("");										
			strarrData[47] = _T("0");									    
			strarrData[48] = _T("");										
			strarrData[49] = _T("N");										
			strarrData[50] = _T("");										
			strarrData[51] = _T("");										
			strarrData[52] = _T("");										
			strarrData[53] = _T("");										
			strarrData[54] = _T("");										
			strarrData[55] = TmpDM.GetCellData(_T("�ֹ��ڻ��ּ�"), 0);	
			strarrData[56] = _T("N");										

			m_API->LoadPrintLibrary();
			
			CString strTmpData = m_API->MakeDataFormat(strarrData);

			if(_T("") != strTmpData)
			{
				m_API->ConvertUnicodeToMBCS(strTmpData);
				m_API->PutEmsData(m_API->m_postData);
			}
		}
		nIdx = m_pGrid->SelectGetNext();
	}
	m_API->ExecPrint();
	m_API->FreePrintLibrary();

	return 0;
}

INT CKL_OTHER_RETURN_REQUEST::ExeDeliveryInfo()
{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -100;
	}

	INT nIdx = m_pGrid->SelectGetIdx();
	if(0 <= nIdx)
	{
		DeliveryInfo(nIdx, 0);
	}
	else
	{
		return -200;
	}
	return 0;
}

INT CKL_OTHER_RETURN_REQUEST::DeliveryInfo(INT nIdx, INT nSwitch)
{
	int ids = 0;
	CString strFuncName = _T("CKL_OTHER_RETURN_REQUEST::DeliveryInfo()");

	if(0 > nIdx)
	{
		return -100;
	}
	
	CString strPaymentKey = m_pDM->GetCellData(_T("����ó��KEY"), nIdx);
	if(_T("") == strPaymentKey)
	{
		AfxMessageBox(_T("��ȸ��� �׸��� �ƴմϴ�."));
		return -200;
	}

	CString strLoanLibcode = m_pDM->GetCellData(_T("���⵵������ȣ"), nIdx);
	CString strReturnLibcode = m_pDM->GetCellData(_T("�ݳ���������ȣ"), nIdx);
	if(_T("") == strLoanLibcode || _T("") == strReturnLibcode)
	{
		AfxMessageBox(_T("�׸��� �����ϴ�."));
		return -300;
	}
	if(0 != nSwitch)
	{
		CKL_OTHER_RETURN_DELIVERY dlg(this, nIdx);
		dlg.DoModal();
	}
	else 
	{
		CKL_OTHER_RETURN_DELIVERY dlg(this);
		dlg.DoModal();
	}
	
	return 0;
}

BEGIN_EVENTSINK_MAP(CKL_OTHER_RETURN_REQUEST, CKL_OTHER_RETURN_DEFAULT)
    //{{AFX_EVENTSINK_MAP(CKL_OTHER_RETURN_REQUEST)
	ON_EVENT(CKL_OTHER_RETURN_REQUEST, IDC_grid, -601 /* DblClick */, OnDblClickgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKL_OTHER_RETURN_REQUEST::OnDblClickgrid() 
{
	INT nIdx = m_pGrid->GetIdx();

	if(0 <= nIdx)
	{
		DeliveryInfo(nIdx, 1);
	}
	else
	{
		return;
	}

	// TODO: Add your control notification handler code here
	
}

INT CKL_OTHER_RETURN_REQUEST::ReturnCancel_Send(CString strOtherLoanKey)
{
	CFrameSender Sender(m_pInfo);	
	CFrameReader Reader;
	CFrameWriter Writer(m_pInfo);
	
	INT ids=0;

	INT nIdx = m_pGrid->SelectGetTailPosition();
	for(int i=0;i<m_pGrid->SelectGetCount();i++)
	{
		Writer.clear();
		Reader.clear();
		
		Writer.setCommand(_T("LILL_K02_LOC_B04_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("Ÿ������KEY"),strOtherLoanKey);

		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if (0 > ids) 
		{
			while(true) 
			{
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
		m_pDM->DeleteRow(nIdx);
		nIdx=m_pGrid->SelectGetPrev();
	}
	AfxMessageBox(_T("�۾� ��� �Ϸ�"));
	m_pGrid->Display();
	return 0;		
}

CString CKL_OTHER_RETURN_REQUEST::GetUserName(CString strUserNo)
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

CString CKL_OTHER_RETURN_REQUEST::GetBirthDay(CString strUserNo)
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

CString CKL_OTHER_RETURN_REQUEST::GetUserClassDesc(CString strUserNo)
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

BOOL CKL_OTHER_RETURN_REQUEST::PrintWaybill()
{

EFS_BEGIN

	CESL_DataMgr* pSrcDM = FindDM(_T("DM_KL_OTHER_RETURN_MNG_REQUEST"));
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
	
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_KL_OTHER_RETURN_MNG_REQUEST") , _T("GRID") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nSelectCnt = pGrid->SelectGetCount();

	if(nSelectCnt <= 0)
	{
		AfxMessageBox( _T("���� ������ �����ϴ�.") );
		return 1;
	}

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_KL_OTHER_RETURN_MNG_REQUEST_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
	pPrintDM->FreeData();

	const INT nCopyCnt = 8;
	TCHAR *sField[nCopyCnt][2] =
	{
		{_T("�̿��ڹ�ȣ"),_T("�̿��ڹ�ȣ")},
		{_T("�̿��ڸ�"),_T("�̿��ڸ�")},
		{_T("��Ϲ�ȣ"),_T("��Ϲ�ȣ")},
		{_T("����"),_T("����")},
		{_T("������"),_T("������")},
		{_T("û����ȣ"),_T("û����ȣ")},				
		{_T("�ηϹ߼�����"),_T("�ηϹ߼�����")},
		{_T("�޸�"),_T("�޸�")},		
	};

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

		ids = pSrcDM->GetCellData( _T("������������ȣ") , nIndex , sData );

		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );

		if(sData.Compare(_T("")) == 0)
		{
			ids = pSrcDM->GetCellData( _T("���⵵������ȣ") , nIndex , sData );

			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );

			ids = pSrcDM->GetCellData( _T("���⵵����") , nIndex , sData2 );

			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
		}
		else
		{
			ids = pSrcDM->GetCellData( _T("����������") , nIndex , sData2 );

			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
		}		

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
			nIndex = pGrid->SelectGetIdx();

			ids = pSrcDM->GetCellData( _T("������������ȣ") , nIndex , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
			
			if(sData.Compare(_T("")) == 0)
			{
				ids = pSrcDM->GetCellData( _T("���⵵������ȣ") , nIndex , sData );

				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
			}

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

			pGrid->SelectGetNext();
			nRowNum ++;
		}

		pPrintDM->SORT(_T("�̿��ڹ�ȣ"));
		pPrintDM->SORT(_T("�̿��ڸ�"));

		for(i=0; i<pPrintDM->GetRowCount();i++)
		{
			sData.Format( _T("%d") , i+1 );
			ids = pPrintDM->SetCellData( _T("��ȣ") , sData , i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );	
		}

		CString strAcqYear, strGroupInfo;
		CBasicPrintMgr PrintMgr(this);
		PrintMgr.InitPrintData();

		PrintMgr.AddPrintData(_T("HOLD_LIB"), iter->second);
		PrintMgr.AddPrintData(_T("LOAN_LIB"), m_pInfo->LIB_NAME);
		PrintMgr.AddPrintData(_T("SEND_DATE"), strDate);
		PrintMgr.SetPrintDM(pPrintDM);
		PrintMgr.SetPrintType(0);
		PrintMgr.SetSIReportFilename(_T("�ݳ������"));
		PrintMgr.SetPrivacyPrintLogParm( TRUE, m_pInfo->USER_PK, m_pInfo->USER_ID, _T("Ÿ���ݳ����� �ݳ������"), m_pInfo->MANAGE_CODE, _T("Ÿ���ݳ����� �ݳ������"),	0 );
		PrintMgr.Print();
	}	

	mapLib.clear();
	
	return TRUE;
EFS_END
		
	return FALSE;
}

void CKL_OTHER_RETURN_REQUEST::ModifyMemo()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return;
	}

	CKL_OTHER_RETURN_EDIT_MEMO dlgEditMemo(m_pParent);
	dlgEditMemo.SetGrid(m_pGrid);
	dlgEditMemo.SetDM(m_pDM);

	dlgEditMemo.DoModal();

	if(dlgEditMemo.m_BModify)
		Refresh();
}

void CKL_OTHER_RETURN_REQUEST::ViewLoanUser()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return;
	}

	CKL_OTHER_RETURN_DETAILVIEW dlgViewLoanUser(m_pParent);
	dlgViewLoanUser.SetGrid(m_pGrid);
	dlgViewLoanUser.SetDM(m_pDM);

	dlgViewLoanUser.DoModal();	
}
