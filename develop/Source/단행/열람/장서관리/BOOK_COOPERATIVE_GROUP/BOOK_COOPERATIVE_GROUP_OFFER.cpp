#include "stdafx.h"
#include "resource.h"
#include "BOOK_COOPERATIVE_GROUP_OFFER.h"
#include "ILL_VIEW_BOOK_INFO_DLG.h"
#include "ILL_VIEW_LIB_INFO_DLG.h"
#include "ILL_VIEW_STATE_INFO_DLG.h"
#include "ILL_MODIFY.H"
#include "ILL_SEARCH.h"
#include "ILL_COMPARE.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_3170.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_3170_ReserveInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBOOK_COOPERATIVE_GROUP_OFFER::CBOOK_COOPERATIVE_GROUP_OFFER(CESL_Mgr* pParent /*=NULL*/)
	: CBookCooperativeGroup(pParent)
{
	
	m_pGrid = NULL;
	m_pDM = NULL;


	m_pLoanShareManager = NULL;
	m_pLoanMangeValue = NULL;
}


void CBOOK_COOPERATIVE_GROUP_OFFER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOOK_COOPERATIVE_GROUP_OFFER)
		
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOOK_COOPERATIVE_GROUP_OFFER, CDialog)
	//{{AFX_MSG_MAP(CBOOK_COOPERATIVE_GROUP_OFFER)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_cmb_OFFER, OnSelchangecmbOFFER)
	ON_CBN_DROPDOWN(IDC_cmb_OFFER, OnDropdowncmbOFFER)
	ON_BN_CLICKED(IDC_BTN_OFFER_STATE, OnBtnOfferState)		
	ON_BN_CLICKED(IDC_btnSelect, OnbtnSelect)	
	ON_MESSAGE(WM_SEARCH, Search)
	ON_MESSAGE(WM_EXCUTE, OnExcute)
	ON_CONTROL_RANGE(BN_CLICKED, 4500, 4505, OnBtnState)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




void CBOOK_COOPERATIVE_GROUP_OFFER::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	ResizeControl();
}

BOOL CBOOK_COOPERATIVE_GROUP_OFFER::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
 	if(0 > InitESL_Mgr(_T("BOOK_COOPERATIVE_GROUP_OFFER")))
 	{
 		AfxMessageBox(_T("ERROR : InitESL_Mgr(\"BOOK_COOPERATIVE_GROUP_OFFER\")"));
 	}
 	m_pGrid = (CESL_Grid*)FindControl(
		_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("�׸���_�����ڷ�"));
		
	
	if( !m_pInfo->m_bUserInfoExportYN )
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}	
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("å�η� ����");
	}

	m_pDM = FindDM(_T("DM_BOOK_COOPERATIVE_GROUP_OFFER"));	

	ResetComboBox();
	{ 
	
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)FindControl(
		_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("��Ƽ�޺�_�������"));
	if(NULL == pCMB) return FALSE;	
	
	CString strData = _T("");
	int ROW = pCMB->GetCount();
	for(int nCnt = 0; nCnt < ROW; nCnt++)
	{
		strData = pCMB->GetItemDataString(nCnt, 1);
		if(_T("��û���ڷ�") == strData)
		{
			pCMB->SetCurSel(nCnt);
			break;
		}
	} 
	} 
	
	Refresh(_T(""), _T(""));

	m_strState[0][0] = _T("��û���ڷ�");
	m_strState[0][1] = _T("0002");
	m_strState[1][0] = _T("�߼۵��ڷ�");
	m_strState[1][1] = _T("0004");
	m_strState[2][0] = _T("�Լ����ڷ�");
	m_strState[2][1] = _T("0014");
	m_strState[3][0] = _T("�������ڷ�");
	m_strState[3][1] = _T("0008");
	m_strState[4][0] = _T("�߼۰������ڷ�");
	m_strState[4][1] = _T("0011");

	INT nCnt = 0;
	INT nBtnStateCnt = 5;
	for(nCnt=0; nCnt < nBtnStateCnt; nCnt++)
	{
		m_pBtnState[nCnt]->Create(
			m_strState[nCnt][0], 
			WS_VISIBLE, 
			CRect(10, 50+(30*nCnt)+(15*nCnt), 160, 50+(30*nCnt)+(15*nCnt)+30),
			this, 
			4500+nCnt);
		m_pBtnState[nCnt]->SetFont(GetFont());
	}	


	return TRUE;  
	              
}

BOOL CBOOK_COOPERATIVE_GROUP_OFFER::Create(CWnd *pParent)
{
	return CDialog::Create(IDD, pParent);
}

BOOL CBOOK_COOPERATIVE_GROUP_OFFER::ResizeControl()
{	
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_grid_OFFER);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return FALSE;
	
	CRect rDlg = CRect(0, 0, 0, 0);
	GetClientRect(rDlg);

	UINT dwFlag = SW_HIDE;
	CString strCaption;
	GetDlgItemText(IDC_BTN_OFFER_STATE, strCaption);
	if(_T("��") == strCaption)
	{
		rDlg.left += 175;
		dwFlag = SW_SHOW;
	}
	else if(_T("��") == strCaption)
	{
		dwFlag = SW_HIDE;
	}

	INT nCnt = 0;
	INT nBtnStateCnt = 5;
	for(nCnt=0; nCnt < nBtnStateCnt; nCnt++)
	{
		m_pBtnState[nCnt]->ShowWindow(dwFlag);
	}

	rDlg.top += 40;
	pWnd->MoveWindow(rDlg);

	return TRUE;
}

void CBOOK_COOPERATIVE_GROUP_OFFER::OnSelchangecmbOFFER() 
{
	
 	Refresh(_T(""), _T(""));
}

int CBOOK_COOPERATIVE_GROUP_OFFER::View_BookInfo()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() == 0)
	{
		m_pGrid->SelectRemoveAll();
		INT nRows = m_pDM->GetRowCount();		
		for (int i = 0; i < nRows; i++)
		{
			m_pGrid->m_arraySelect.AddTail(i);
		}
		int nIdx = m_pGrid->GetIdx();
		if(0 > nIdx) nIdx = 0;
		m_pGrid->m_arrayPos = m_pGrid->m_arraySelect.FindIndex(nIdx);
	}
 	int nCnt = m_pGrid->SelectGetCount();
 	if(0 >= nCnt) 
 	{
 		AfxMessageBox(_T("������ ������ �������� �ʽ��ϴ�."));
 		return -1;
 	}

	CILL_VIEW_BOOK_INFO_DLG Dlg(this);
	Dlg.m_pInfo = m_pInfo;
	Dlg.m_pParentDM = m_pDM;
	Dlg.m_pParentGrid = m_pGrid;
	Dlg.m_nIdx = m_pGrid->SelectGetIdx();
	Dlg.m_bIsShowChangeRegNo = TRUE;
	Dlg.m_sBookCooperativeMode = m_sBookCooperativeMode;
	Dlg.m_pLoanShareManager = m_pLoanShareManager;	
	Dlg.m_strState = _T("����");

	Dlg.DoModal();
		
	Refresh();
	RefreshState();

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::ExcutePath(CString strSM)
{
	int ids = -1;

	if( _T("����_�߼�") == strSM)
	{
		m_pGrid->SelectMakeList();

		CString strMessage;
		CDataMgr DataMgr;
		ids = PreSend(&DataMgr, strMessage);
		if(0 > ids)
		{
			return ids;
		}
		ids = Send(&DataMgr, &strMessage);
		if(0 > ids)
		{
			return ids;
		}

		Refresh();	
		RefreshState();

		return ids;
	}
	else if( _T("����_�߼۰���") == strSM)
	{
		ids = Send_Refuse();
		Refresh();	
		RefreshState();

		return ids;
	}
	else if( _T("����_�߼����") == strSM)
	{
		ids = Send_Cancel();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("����_����(�Ϸ�)") == strSM)
	{
		m_pGrid->SelectMakeList();

		ids = Return_Complete();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("����_���") == strSM)
	{
		ids = Cancel();
		Refresh();
		RefreshState();

		return ids;
	}
	else if( _T("����_��������") == strSM)
	{
		return View_BookInfo();
	}
	else if( _T("����_�̿�������") == strSM)
	{
		return View_UserInfo();
	}
	else if( _T("����_���������") == strSM)
	{
		return View_LibInfo();
	}
	else if( _T("����_��������") == strSM)
	{
		return View_StateInfo();
	}
	else if(_T("�ϰ�����") == strSM)
	{
  		CILL_MODIFY Dlg;
  		Dlg.m_pInfo = m_pInfo;
  		Dlg.m_pParentDM = m_pDM;
  		Dlg.m_pParentGrid = m_pGrid;			
  		Dlg.m_strLibCode = m_strLibCode;
		Dlg.m_pLoanShareManager = m_pLoanShareManager;
		Dlg.m_sBookCooperativeMode = m_sBookCooperativeMode;

  		Dlg.DoModal();

		m_pGrid->SetProtectSelectColumn(TRUE);
		Refresh();		
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
	else if(_T("���忩������") == strSM)
	{
		ids = Compare();
	}
	else if(_T("RFID���ȼ���") == strSM)
	{
		ids = RFID_SecurityChange();
	}	
	else if( _T("������") == strSM)
	{			
		if( !m_pInfo->m_bUserInfoExportYN )
		{
			AfxMessageBox(_T("�������� ����� ���ѵǾ��ֽ��ϴ�."));
			return 0;
		}
		ids = PrintList();
		return ids;
	}
	else if(_T("����_�߼۰������") == strSM)
	{
		ids = Send_Refuse_Cancel();
		Refresh();	
		RefreshState();
		return ids;
	}	
	else if( _T("����_�����߼��������") == strSM)	
	{						
		if( !m_pInfo->m_bUserInfoExportYN )
		{
			AfxMessageBox(_T("�������� ����� ���ѵǾ��ֽ��ϴ�."));
			return 0;
		}
		ids = PrintOfferList();
		return ids;
	}

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::View_LibInfo()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() == 0)
	{
		m_pGrid->SelectRemoveAll();
		INT nRows = m_pDM->GetRowCount();		
		for (int i = 0; i < nRows; i++)
		{
			m_pGrid->m_arraySelect.AddTail(i);
		}
		int nIdx = m_pGrid->GetIdx();
		if(0 > nIdx) nIdx = 0;
		m_pGrid->m_arrayPos = m_pGrid->m_arraySelect.FindIndex(nIdx);
	}
 	int nCnt = m_pGrid->SelectGetCount();
 	if(0 >= nCnt)
 	{
 		AfxMessageBox(_T("������ ������ �������� �ʽ��ϴ�."));
 		return -1;
 	}	

	CILL_VIEW_LIB_INFO_DLG Dlg;
	Dlg.m_pParentDM = m_pDM;
	Dlg.m_pParentGrid = m_pGrid;
	Dlg.m_strLibAlias = _T("��û������_�ڵ�");
	Dlg.m_nIdx = m_pGrid->SelectGetIdx();

	Dlg.DoModal();

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::PreSend(CDataMgr* pDataMgr, CString& strMessage)
{
		
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1000;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("����|!=|0002"));
	aCheckArray.Add(_T("����������_�ڵ�|!=|%m_strLibCode%"));

	
	UnCheckGrid(aCheckArray);
	}
		
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("������ �׸��� �߼� ������ �׸��� �����ϴ�.\r\n")
			_T("�߼� ������ ����� �Ʒ��� �����ϴ�.\r\n\r\n")
			_T("1. ������������ �ڰ��� ���\r\n")
			_T("2. ������°� ��û���ڷ�(0002)�� Ʈ�����\r\n")
			);
		return -1010;
	}
	else
	{
		if(NULL == pDataMgr)
		{
			return -1020;
		}

		CString strGet;
		INT nSetIdx = 0;
		INT nIdx = m_pGrid->SelectGetHeadPosition();
		INT nIdx2 = 0;
		do
		{	
			pDataMgr->InsertData(-1);				
			nSetIdx = pDataMgr->GetCount()-1;
			m_pDM->GetCellData(_T("Ʈ����ǹ�ȣ"), nIdx, strGet);			
			pDataMgr->SetData(_T("Ʈ����ǹ�ȣ"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("åKEY"), nIdx, strGet);
			pDataMgr->SetData(_T("åKEY"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("�̿���KEY"), nIdx, strGet);			
			pDataMgr->SetData(_T("�̿���KEY"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("�������±���"), nIdx, strGet);
			pDataMgr->SetData(_T("�������±���"), strGet, nSetIdx);

			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);

	}

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Send(CDataMgr* pDataMgr, CString* pstrMessage)
{
	INT ids = 0;
 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	if(_T("I") == m_sBookCooperativeMode)
	{
		CString strNO;
				
		MakeTransNoList(pDataMgr, strNO, TRUE);


		if(_T("") == strNO)
		{
			return -1000;
		}
				
		CString strDate, strTemp;
		pDataMgr->GetData(_T("�߼���"), 0, strTemp); 
		if(_T("SYSDATE") != strTemp && _T("") != strTemp)
		{			
			strDate.Format(_T("TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS')"), strTemp);
		}
		
		
		else if(_T("SYSDATE") != strTemp && _T("") == strTemp)
		{
			strDate = _T("SYSDATE");
		}
		else
		{
			strDate = strTemp;
		}

		TmpDM.StartFrame();
		CString strQuery;
		strQuery.Format(
			_T("UPDATE ILL_TRANS_TBL ITL SET ")
			_T("ITL.STATE = '0004', ")
			_T("ITL.GIVE_USERID = '%s', ") 
			
			
			_T("ITL.GIVE_DATE = %s, ")

			_T("ITL.WORKNO = ")
				_T("(SELECT ")
				_T("CASE ITL2.PUBLISH_FORM_CODE ")
				_T("WHEN 'MO' THEN (SELECT BB.WORKNO FROM BO_BOOK_TBL BB WHERE BB.REC_KEY = ITL2.BOOK_KEY) ")
				_T("WHEN 'SE' THEN (SELECT BB.WORKNO FROM SE_BOOK_TBL BB WHERE BB.REC_KEY = ITL2.BOOK_KEY) ")
				_T("END ")
				_T("FROM ILL_TRANS_TBL ITL2 ")
				_T("WHERE ITL2.TRANS_NO = ITL.TRANS_NO), ")
			_T("ITL.LAST_WORK = '%s' ")
			_T("WHERE ")
			_T("ITL.TRANS_NO IN (%s) AND ITL.STATE = '0002' AND ITL.GIVE_LIBCODE = '%s';"),
			m_pInfo->USER_ID,
			
			
			strDate,
			GetWorkLogMsg( _T("�߼�"), __WFILE__, __LINE__ ),
			strNO,
			m_strLibCode
			);
	
		TmpDM.AddFrame(strQuery);
		ids = TmpDM.SendFrame();
		TmpDM.EndFrame();

		if(0 > ids)
		{
			pstrMessage->Format(_T("DB ������Ʈ�� �����Ͽ����ϴ�.[%d]"), ids);
			return ids;
		}

	}
	else if(_T("D") == m_sBookCooperativeMode)
	{
		
		CString strLoanKey, strGet, strTransNo;


		const INT COUNT = pDataMgr->GetCount();
		for(INT nIdx = 0; nIdx < COUNT; nIdx++)
		{	

			pDataMgr->GetData(_T("Ʈ����ǹ�ȣ"), nIdx, strGet);

			TmpDM.StartFrame();			

			StruMakeInsertTbl Stru;
			Stru.pDM = &TmpDM;
			
			TmpDM.MakeRecKey(strLoanKey);
			Stru.strInLoanKey= strLoanKey;


			pDataMgr->GetData(_T("åKEY"), nIdx, strGet);
			Stru.strInBookKey = strGet;


			pDataMgr->GetData(_T("�̿���KEY"), nIdx, strGet);
			Stru.strInUserKey = strGet;
			

			pDataMgr->GetData(_T("�������±���"), nIdx, strGet);
			Stru.strInPublishFormCode = strGet;

			Make_InsertLoanTblQuery(&Stru);

			CString strQuery;
			strQuery.Format(
				_T("UPDATE ILL_TRANS_TBL ITL SET ")
				_T("ITL.STATE = '0004', ")
				_T("ITL.GIVE_USERID = '%s', ") 
				_T("ITL.GIVE_DATE = SYSDATE, ")
				_T("ITL.LOAN_KEY = %s, ")
				_T("ITL.WORKNO = ")
					_T("(SELECT ")
					_T("CASE ITL2.PUBLISH_FORM_CODE ")
					_T("WHEN 'MO' THEN (SELECT BB.WORKNO FROM BO_BOOK_TBL BB WHERE BB.REC_KEY = ITL2.BOOK_KEY) ")
					_T("WHEN 'SE' THEN (SELECT BB.WORKNO FROM SE_BOOK_TBL BB WHERE BB.REC_KEY = ITL2.BOOK_KEY) ")
					_T("END ")
					_T("FROM ILL_TRANS_TBL ITL2 ")
					_T("WHERE ITL2.TRANS_NO = ITL.TRANS_NO), ")
				_T("ITL.LAST_WORK = '%s' ")
				_T("WHERE ")
				_T("ITL.TRANS_NO = '%s' AND ITL.STATE = '0002' AND ITL.GIVE_LIBCODE = '%s';"),
				m_pInfo->USER_ID,
				strLoanKey,
				GetWorkLogMsg( _T("�߼�"), __WFILE__, __LINE__ ),
				strTransNo,
				m_strLibCode
				);

			TmpDM.AddFrame(strQuery);
			ids = TmpDM.SendFrame();
			TmpDM.EndFrame();

			if(0 > ids)
			{
				pstrMessage->Format(_T("DB ������Ʈ�� �����Ͽ����ϴ�.[%d]"), ids);
				return ids;
			}
			
			ShareInfoCenterUpload(strLoanKey, _T("I"), _T("LH_STORAGE_V01"), _T(""));
			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,GIVE_USERID,GIVE_DATE,LOAN_KEY,WORKNO"));

		}
	}
	if(0 > ids)
	{
		return ids;
	}

	CString strNO;
	MakeTransNoList(pDataMgr, strNO);
	

	if(0 != m_pGrid->SelectGetCount())
	{
		return RFID_SecurityChange(strNO, FALSE, TRUE);
	}
	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Send_Cancel()
{
	
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("����|!=|0004"));
	aCheckArray.Add(_T("����������_�ڵ�|!=|%m_strLibCode%"));
	
	UnCheckGrid(aCheckArray);
	}	
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("������ �׸��� �߼���� ������ �׸��� �����ϴ�.\r\n")
			_T("�߼���� ������ ����� �Ʒ��� �����ϴ�.\r\n\r\n")
			_T("1. ������������ �ڰ��� ���\r\n")
			_T("2. ������°� �߼۵��ڷ�(0004)�� Ʈ�����\r\n")
			);
		return -1;
	}

	CString strNO = Get_Select_TRANS_NO_List(TRUE);

	if(_T("") == strNO) return -1;

 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	if(_T("I") == m_sBookCooperativeMode)
	{
		TmpDM.StartFrame();

		CString strQuery;
		strQuery.Format(
			_T("UPDATE ILL_TRANS_TBL SET ")
			_T("LOAN_KEY = NULL, ")
			_T("STATE = '0002', ")
			_T("GIVE_USERID = '%s', ") 
			_T("GIVE_DATE = SYSDATE, ")
			_T("WORKNO = NULL, ")
			_T("LAST_WORK = '%s' ")
			_T("WHERE ")
			_T("TRANS_NO IN (%s) AND STATE = '0004' AND GIVE_LIBCODE = '%s';"),
			m_pInfo->USER_ID,
			GetWorkLogMsg( _T("�߼����"), __WFILE__, __LINE__ ),
			strNO,
			m_strLibCode
			);

		TmpDM.AddFrame(strQuery);
		int ids = TmpDM.SendFrame();
		TmpDM.EndFrame();
		

	}	
	else if(_T("D") == m_sBookCooperativeMode)
	{

		CString strLoanKey, strBookKey, strUserKey, strTransNo;
		int nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{	
			m_pDM->GetCellData(_T("Ʈ����ǹ�ȣ"), nIdx, strTransNo);
			m_pDM->GetCellData(_T("��������_Ű"), nIdx, strLoanKey);

			TmpDM.StartFrame();				
			CString strQuery;
			strQuery.Format(
				_T("UPDATE ILL_TRANS_TBL SET ")
				_T("STATE = '0002', ")
				_T("GIVE_USERID = '%s', ") 
				_T("GIVE_DATE = SYSDATE, ")
				_T("LOAN_KEY = NULL, ")
				_T("WORKNO = NULL, ")
				_T("LAST_WORK = '%s' ")
				_T("WHERE ")
				_T("TRANS_NO = '%s' AND STATE = '0004' AND GIVE_LIBCODE = '%s';"),
				m_pInfo->USER_ID,
				GetWorkLogMsg( _T("�߼����"), __WFILE__, __LINE__ ),
				strTransNo,
				m_strLibCode
				);

			TmpDM.AddFrame(strQuery);

			TmpDM.MakeDeleteFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey);

			int ids = TmpDM.SendFrame();
			TmpDM.EndFrame();

			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,GIVE_USERID,GIVE_DATE"));

			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);

	}	
	strNO = Get_Select_TRANS_NO_List();		
	
	return RFID_SecurityChange(strNO, TRUE, TRUE);

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Send_Refuse()
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::Send_Refuse()");
	
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("����|!=|0002"));
	aCheckArray.Add(_T("����������_�ڵ�|!=|%m_strLibCode%"));

	
	ids = UnCheckGrid(aCheckArray);
	}
	
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("������ �׸��� �߼۰��� ������ �׸��� �����ϴ�.\r\n")
			_T("�߼۰��� ������ ����� �Ʒ��� �����ϴ�.\r\n\r\n")
			_T("1. ������������ �ڰ��� �ڷ�\r\n")
			_T("2. ������°��� ��û���ڷ�(0002)�� Ʈ�����")			
			);
		return -1;
	}
	
	CREFUSE_DLG Dlg;
	Dlg.m_DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	Dlg.m_DM.SetManageCode(m_pInfo->MANAGE_CODE);
	Dlg.m_strClass = _T("i03");	
	if(IDOK != Dlg.DoModal()) { return -2; }
	
	CString strGIVE_CANCEL = Dlg.m_strCancelCode;
	
	CString strContent = Dlg.m_strContent;

 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	int nIdx = m_pGrid->SelectGetHeadPosition();
	do
	{	
		TmpDM.StartFrame();
		CString strTransNo = _T("");
		
		{
		ids = m_pDM->GetCellData(_T("Ʈ����ǹ�ȣ"), nIdx, strTransNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);		
		
		CString strLastWork = GetWorkLogMsg( _T("�߼۰���"), __WFILE__, __LINE__ );
		
		CString strQuery;		
		strQuery.Format(		
			_T("UPDATE ILL_TRANS_TBL SET ")		
			_T("STATE = '0011', ")		
			_T("GIVE_USERID = '%s', ") 
			_T("GIVE_DATE = SYSDATE, ")		
			_T("GIVE_CANCEL = '%s', ")	
			_T("GIVE_CONTENT = '%s', ")	
			_T("LAST_WORK = '%s' ")		
			_T("WHERE ")			
			_T("TRANS_NO = '%s';"),
			m_pInfo->USER_ID,	
			strGIVE_CANCEL,	
			strContent,		
			strLastWork,		
			strTransNo	

			);		
		TmpDM.AddFrame(strQuery);
		}
				
		{

		CString strQuery = _T("");
		ids = Get_UpdateBookTblQuery_For_CompleteTransaction(
			nIdx,
			GetWorkLogMsg( _T("�߼۰���"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
		}
		TmpDM.AddFrame(strQuery);

		ids = Get_UpdateIdxTblQuery(
			nIdx,
			GetWorkLogMsg( _T("�߼۰���"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids)
		{
			return ids;
		}
		else if(0 == ids)
		{
			TmpDM.AddFrame(strQuery);
		}

		}		

		ids = TmpDM.SendFrame(TRUE);
		TmpDM.EndFrame();

		if(0 > ids)
		{
			return ids;
		}
		
		if(_T("D") == m_sBookCooperativeMode)
		{
			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,GIVE_USERID,GIVE_DATE,GIVE_CANCEL,GIVE_CONTENT"));	
		}

		Reversion(nIdx);

		nIdx = m_pGrid->SelectGetNext();
	}while(0 <= nIdx);

	return ids;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Return_Complete()
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::Return_Complete()");
	CString strQuery = _T("");
	
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("����|NOT IN|0004,0008,0014"));
	aCheckArray.Add(_T("�������|==|������"));
	aCheckArray.Add(_T("����������_�ڵ�|!=|%m_strLibCode%"));

	
	ids = UnCheckGrid(aCheckArray);
	}
		
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("������ �׸��� ����(�Ϸ�) ������ �׸��� �����ϴ�.\r\n")
			_T("����(�Ϸ�) ������ ����� �Ʒ��� �����ϴ�.\r\n\r\n")
			_T("1. �ڰ����� ������ �ڷ�\r\n")
			_T("2. ������°� �߼۵��ڷ�(0004), �Լ����ڷ�(0014), �������ڷ�(0008)�� Ʈ�����\r\n")
			_T("3. ���� �������� �ƴ� �ڷ�")
			);
		return -1;
	}

	CString strMessage;
	strMessage.Format(
		_T("������ �ڷ���� ����(�Ϸ�) ó���մϴ�.\r\n")
		_T("������´� �Ϸ���ڷ�(0009)�� ����Ǹ� \r\n")
		_T("å�� ���´� å�η�����ڷῡ�� �������·� ���͵˴ϴ�.\r\n")
		_T("����Ͻðڽ��ϱ�?\r\n\r\n")
		_T("[������ ������ ���� ��쿡�� '��'�� �����Ͻʽÿ�.]\r\n")
		);
	UINT uResult = AfxMessageBox(strMessage, MB_YESNO);
	if(IDNO == uResult) return -1;

	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	SetCheckAppendixAndMother();
	m_pGrid->SelectMakeList();

	int nIdx = m_pGrid->SelectGetHeadPosition();
	do
	{
		TmpDM.StartFrame();
		CString strTransNo = _T("");
		
		{
		
		ids = m_pDM->GetCellData(_T("Ʈ����ǹ�ȣ"), nIdx, strTransNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

		CString strQuery = _T("");
		
		strQuery.Format(
			_T("UPDATE ")
				_T("ILL_TRANS_TBL ")
			_T("SET ")
				_T("STATE = '0009', ")	
				_T("END_USERID = '%s', ") 
				_T("END_DATE = SYSDATE, ")
				_T("END_DIV = '0010', ") 
				_T("LAST_WORK = '%s' ")
			_T("WHERE ")
				_T("TRANS_NO = '%s';"),
				m_pInfo->USER_ID,
				GetWorkLogMsg( _T("����(�Ϸ�)"), __WFILE__, __LINE__ ),
				strTransNo
			);
		TmpDM.AddFrame(strQuery);
		}
				
		{
		
		ids = Get_UpdateBookTblQuery_For_CompleteTransaction(
			nIdx,
			GetWorkLogMsg( _T("����(�Ϸ�)"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
		TmpDM.AddFrame(strQuery);
		
		ids = Get_UpdateIdxTblQuery(
			nIdx,
			GetWorkLogMsg( _T("����(�Ϸ�)"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids)
		{
			return ids;
		}
		else if(0 == ids)
		{
			TmpDM.AddFrame(strQuery);
		}

		}
	
		Reversion(nIdx);
		
		nIdx = m_pGrid->SelectGetNext();
	}while(0 <= nIdx);


	CString strNO = Get_Select_TRANS_NO_List();
	RFID_SecurityChange(strNO, TRUE, TRUE);

	return 0;
}


VOID CBOOK_COOPERATIVE_GROUP_OFFER::KL_Transaction_end(INT nIdx) 
{
	
	CString strQuery, strGetData, strLoanKey;
	CString strKLUserYN, strBookAppendixFlag;
	
	CESL_DataMgr TmpDM;
	INT ids = 0;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	m_pDM->GetCellData(_T("å����ȸ������"), nIdx, strKLUserYN);
	m_pDM->GetCellData(_T("�ηϿ���"), nIdx, strBookAppendixFlag);
	
	m_pDM->GetCellData(_T("��������_Ű"), nIdx, strLoanKey);

	
	if(_T("Y") == strKLUserYN && strBookAppendixFlag != _T("V") && strLoanKey.IsEmpty() == FALSE)
	{
		ids = m_pDM->GetCellData(_T("�̿���_��ȣ"), nIdx, strGetData);

		strQuery.Format(_T("SELECT TO_CHAR(LOAN_STOP_DATE,'YYYY/MM/DD HH24:MI:SS') AS LOAN_STOP_DATE, TO_CHAR(LOAN_STOP_SET_DATE,'YYYY/MM/DD HH24:MI:SS') AS LOAN_STOP_SET_DATE ")
						_T("FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'"), strGetData);
		TmpDM.RestructDataManager(strQuery);

		CFrameWriter Writer(m_pInfo);
		Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
		Writer.addRecord();
		
		Writer.addElement(_T("���մ����ڹ�ȣ"), strGetData);
		Writer.addElement(_T("���⵵������ȣ"), m_pInfo->LIB_CODE);
		Writer.addElement(_T("�ݳ���������ȣ"), m_pInfo->LIB_CODE);
		ids = m_pDM->GetCellData(_T("�ݳ���_�ð�"), nIdx, strGetData);
		Writer.addElement(_T("�ݳ���"), strGetData);

		CString strRegNo;
		ids = m_pDM->GetCellData(_T("��Ϲ�ȣ"), nIdx, strRegNo);
		Writer.addElement(_T("��Ϲ�ȣ"), strRegNo);

		Writer.addElement(_T("����"), _T("1"));
		ids = m_pDM->GetCellData(_T("��������_Ű"), nIdx, strGetData);
		Writer.addElement(_T("����KEY"), strGetData);
		if(TmpDM.GetCellData(0, 0) != _T(""))
		{
			Writer.addElement(_T("ȸ������"), _T("1"));
			Writer.addElement(_T("����������"), TmpDM.GetCellData(0, 0));
			Writer.addElement(_T("���������ο���"), TmpDM.GetCellData(0, 1));
		}
		else
		{
			Writer.addElement(_T("ȸ������"), _T("0"));
		}
		Writer.addElement(_T("��������ȣ��������"), _T("Y"));

		CFrameSender Sender(m_pInfo);
		Sender.SendTempTbl(Writer.getFrame(), GetWorkLogMsg( _T("å�η�����(�Ϸ�)"), __WFILE__, __LINE__ ));
	}
}

BOOL CBOOK_COOPERATIVE_GROUP_OFFER::PreTranslateMessage(MSG* pMsg) 
{
	
	if(WM_KEYDOWN == pMsg->message)
	{
		
		if(VK_ESCAPE == pMsg->wParam)

		{
			return TRUE;
		}
		else if(VK_F10 == pMsg->wParam)
		{	
			PrintList();
			return TRUE;
		}
		
		
		else if( VK_RETURN == pMsg->wParam )
		{
			HWND MsgH = pMsg->hwnd;
			
			if( GetDlgItem(IDC_edtRegNo)->m_hWnd==MsgH )
			{
				OnbtnSelect();
			}
			
			return TRUE;
		}

	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Cancel()
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::Cancel()");

	CString strMessage;
	strMessage.Format(
		_T("������ Ʈ������� �����ŵ�ϴ�.\r\n")
		_T("����� Ʈ������� ��������� ������ �Ұ����ϸ� \r\n")
		_T("å�� ���´� å�η�����ڷῡ�� �������·� ���͵˴ϴ�.\r\n")
		_T("����Ͻðڽ��ϱ�?")
		);
	UINT uResult = AfxMessageBox(strMessage, MB_YESNO);
	if(IDNO == uResult) return -1;

	
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}
		
	SetCheckAppendixAndMother();
	m_pGrid->SelectMakeList();

	CStringArray aCheckArray;
	aCheckArray.Add(_T("�Ϸ�����|!=|%NULL%"));
	if(_T("I") == m_sBookCooperativeMode)
	{
		aCheckArray.Add(_T("��������_Ű|!=|%NULL%"));
	}
	else if(_T("D") == m_sBookCooperativeMode)
	{
		aCheckArray.Add(_T("������|!=|%NULL%"));
	}
	aCheckArray.Add(_T("����������_�ڵ�|!=|%m_strLibCode%"));

	
	ids = UnCheckGrid(aCheckArray);
	}
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("������ �׸��� ��� ������ �׸��� �����ϴ�.\r\n")
			_T("��Ұ� �Ұ����� �׸��� ������ �����ϴ�.\r\n\r\n")
			_T("1. �̹� �Ϸ�� �׸�\r\n")
			_T("  1.1 �Ϸ�� �ڷ�\r\n")
			_T("  1.2 ��û������ �ڷ�\r\n")
			_T("  1.3 �߼۰���Ȯ�ε� �ڷ�\r\n")
			_T("  1.4 ��û��ҵ� �ڷ�\r\n")
			_T("  1.5 ��ҵ� �ڷ�\r\n")
			_T("2. ������ �Ͼ ������ �ڷ�\r\n")
			_T("  2.1 �Լ��� �ڷ��� ������ �Ͼ �ڷ�\r\n")
			_T("  2.2 �Լ��� �ڷ��� ����/�ݳ��� �Ͼ �ڷ�\r\n")
			_T("  2.3 ������ �ڷ�\r\n")
			_T("3. ������������ �ڰ��� �ƴ� �ڷ�")
			);
		return -1;
	}
	
	CREFUSE_DLG Dlg;
	Dlg.m_DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	Dlg.m_DM.SetManageCode(m_pInfo->MANAGE_CODE);
	Dlg.m_strClass = _T("i04");		
	Dlg.m_strCaption = _T("��һ���");
	Dlg.m_strCauseCaption = _T("��һ���");

	if(IDOK != Dlg.DoModal()) { return -2; }
	
	CString strEndDiv = Dlg.m_strCancelCode;
	
	CString strEndContent = Dlg.m_strContent;
	
	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	CString strLoanKey;
	int nIdx = m_pGrid->SelectGetHeadPosition();
	do
	{	
		TmpDM.StartFrame();
		CString strTransNo = _T("");
		
		{
		
		ids = m_pDM->GetCellData(_T("Ʈ����ǹ�ȣ"), nIdx, strTransNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

		CString strQuery = _T("");
		
		strQuery.Format(
			_T("UPDATE ")
				_T("ILL_TRANS_TBL ")
			_T("SET ")
				_T("LOAN_KEY = NULL, ")
				_T("STATE = '0009', ")	
				_T("END_USERID = '%s', ") 
				_T("END_DATE = SYSDATE, ")
				_T("END_DIV = '%s', ") 
				_T("END_CONTENT= '%s', ")
				_T("LAST_WORK = '%s' ")
			_T("WHERE ")
				_T("TRANS_NO = '%s';"),

				m_pInfo->USER_ID,
				strEndDiv,
				strEndContent,
				GetWorkLogMsg( _T("���"), __WFILE__, __LINE__ ),
				strTransNo

			);
		TmpDM.AddFrame(strQuery);
		}
		
		
		{

		CString strQuery = _T("");
		ids = Get_UpdateBookTblQuery_For_CompleteTransaction(
			nIdx,
			GetWorkLogMsg( _T("���"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);

		TmpDM.AddFrame(strQuery);

		ids = Get_UpdateIdxTblQuery(
			nIdx,
			GetWorkLogMsg( _T("���"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids)
		{
			return ids;
		}
		else if(0 == ids)
		{
			TmpDM.AddFrame(strQuery);
		}

		}
		
		if(_T("D") == m_sBookCooperativeMode)
		{
			m_pDM->GetCellData(_T("��������_Ű"), nIdx, strLoanKey);
			if(FALSE == strLoanKey.IsEmpty())
			{
				TmpDM.MakeDeleteFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey);
			}
		}
		ids = TmpDM.SendFrame(TRUE);
		TmpDM.EndFrame();

		if(0 > ids)
		{
			return ids;
		}

		if(_T("D") == m_sBookCooperativeMode)
		{
			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,END_USERID,END_DATE,END_DIV,END_CONTENT"));
		}		

		Reversion(nIdx);

		nIdx = m_pGrid->SelectGetNext();
	}while(0 <= nIdx);

	return 0;
}
int CBOOK_COOPERATIVE_GROUP_OFFER::Reversion(int nIdx)
{
	EFS_BEGIN

	if(NULL == m_pDM) return -1;
	if(0 > nIdx || m_pDM->GetRowCount() <= nIdx) return -2;

	CString strPublishFormCode;
	m_pDM->GetCellData(_T("�������±���"), nIdx, strPublishFormCode);

	CString strManageCode;
	m_pDM->GetCellData(_T("����������_��������"), nIdx, strManageCode);

 	CString strRegNo;
 	m_pDM->GetCellData(_T("��Ϲ�ȣ"), nIdx, strRegNo);

 	CString strBookKey;
 	m_pDM->GetCellData(_T("åKEY"), nIdx, strBookKey);

	CString strVolInfoColumn;
	CString strBookTbl;
	CString strReverseUserCnt;
	if(_T("MO") == strPublishFormCode)
	{
		strBookTbl = _T("BO_BOOK_TBL BB");
		strVolInfoColumn = _T("VOL_SORT_NO");
		strReverseUserCnt.Format(
			_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 WHERE CL2.BOOK_KEY = BB.REC_KEY AND CL2.STATUS = '3' )"));
	}
	else if(_T("SE") == strPublishFormCode)
	{
		strBookTbl = _T("SE_BOOK_TBL BB");
		strVolInfoColumn = _T("VOL_KEY");
		strReverseUserCnt.Format(
			_T("(SELECT COUNT(*) FROM LS_WORK_T01 CL2 WHERE CL2.BOOK_KEY = BB.REC_KEY AND CL2.STATUS = '3' )"));
	}
	else if(_T("NB") == strPublishFormCode)
	{
		return 1;
	}
	else return -3;
	
	CString strQuery;
	strQuery.Format(
		_T("SELECT BB.SPECIES_KEY, %s, %s ")
		_T("FROM %s ")
		_T("WHERE BB.REC_KEY = %s AND BB.MANAGE_CODE = '%s' AND ROWNUM = 1"),

		strVolInfoColumn,
		strReverseUserCnt,
		strBookTbl,
		strBookKey,
		strManageCode
		);

	int ids = -1;

	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	ids = TmpDM.RestructDataManager(strQuery);
	
	CString strSpeciesKey;
	TmpDM.GetCellData(0, 0, strSpeciesKey);
	
	CString strVolInfo;
 	TmpDM.GetCellData(0, 1, strVolInfo);
	
	CString strReversionCnt;
	TmpDM.GetCellData(0, 2, strReversionCnt);

		
	if ( m_pLoanMangeValue->m_bVolReserve && FALSE == strVolInfo.IsEmpty())
	{		
		CString sQuery;
		sQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL ") 
			_T(" WHERE CL.BOOK_KEY IN ( SELECT REC_KEY FROM %s WHERE SPECIES_KEY = %s ) ")
			_T("   AND CL.REG_NO IS NULL ")
			_T("   AND CL.STATUS = '3' ")
			_T("   AND CL.VOL_INFO = %s "),
			strBookTbl, strSpeciesKey, strVolInfo);

		TmpDM.GetOneValueQuery(sQuery, strReversionCnt);
	}	
	
	if( 0 == _ttoi(strReversionCnt))  return 0;
	if ( !(m_pLoanMangeValue->m_bVolReserve) )
	{
		CString sSQL;
		sSQL.Format( _T("SELECT REC_KEY FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3' AND RESERVATION_DATE = ")
			_T("(SELECT MIN(RESERVATION_DATE) FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3')") , strBookKey, strBookKey);
		
		CString sLoanKey;
		TmpDM.GetOneValueQuery(sSQL, sLoanKey);
		
		
		if( sLoanKey.IsEmpty() )
			return 0;
	}
	
	INT nYes = MessageBox(_T("�� �ڷ�� ����� �ڷ��Դϴ�. ���� ó���� �Ͻðڽ��ϱ�?"), _T("���� ó��"), MB_YESNO | MB_ICONQUESTION);
	if( nYes == IDYES )
	{		
		if(!(m_pLoanMangeValue->m_bVolReserve))
		{
			CBO_LOC_3170 Dlg(this, m_pLoanShareManager , m_pLoanMangeValue , 0);
			Dlg.pMain = pMain;
			Dlg.pParentMgr = (CESL_Mgr*)this;
			Dlg.m_pInfo = m_pInfo;
			Dlg.SetInfo(strSpeciesKey, strVolInfo, strPublishFormCode, strBookKey);
			Dlg.m_nReserveInfoMode	=	1;
			Dlg.DoModal();
		}
		else 
		{
			CBO_LOC_3170_ReserveInfo Dlg(this, m_pLoanShareManager , m_pLoanMangeValue , 0);
			Dlg.pMain = pMain;
			Dlg.pParentMgr = (CESL_Mgr*)this;
			Dlg.m_pInfo = m_pInfo;
			Dlg.SetInfo(strSpeciesKey, strVolInfo, strPublishFormCode, _T(""));
			Dlg.m_nReserveInfoMode	=	1;		
			Dlg.m_strReturnBookKey	=	strBookKey;
			Dlg.m_strReturnBookRegNo	=	strRegNo;			

			Dlg.DoModal();
		}
	}
	
	return 0;

	EFS_END
		return -1;
}


int CBOOK_COOPERATIVE_GROUP_OFFER::Reversion(CDataMgr* pDataMgr, int nIdx)
{
	EFS_BEGIN

	if(NULL == pDataMgr) return -1;
	if(0 > nIdx || pDataMgr->GetCount() <= nIdx) return -2;

	CString strPublishFormCode;
	pDataMgr->GetData(_T("�������±���"), nIdx, strPublishFormCode);

	CString strManageCode;
	pDataMgr->GetData(_T("����������_��������"), nIdx, strManageCode);

 	CString strRegNo;
 	pDataMgr->GetData(_T("��Ϲ�ȣ"), nIdx, strRegNo);

 	CString strBookKey;
 	pDataMgr->GetData(_T("åKEY"), nIdx, strBookKey);

	CString strVolInfoColumn;
	CString strBookTbl;
	CString strReverseUserCnt;
	if(_T("MO") == strPublishFormCode)
	{
		strBookTbl = _T("BO_BOOK_TBL BB");
		strVolInfoColumn = _T("VOL_SORT_NO");
		strReverseUserCnt.Format(
			_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 WHERE CL2.BOOK_KEY = BB.REC_KEY AND CL2.STATUS = '3' )"));
	}
	else if(_T("SE") == strPublishFormCode)
	{
		strBookTbl = _T("SE_BOOK_TBL BB");
		strVolInfoColumn = _T("VOL_KEY");
		strReverseUserCnt.Format(
			_T("(SELECT COUNT(*) FROM LS_WORK_T01 CL2 WHERE CL2.BOOK_KEY = BB.REC_KEY AND CL2.STATUS = '3' )"));
	}
	else if(_T("NB") == strPublishFormCode)
	{
		return 1;
	}
	else return -3;
	
	CString strQuery;
	strQuery.Format(
		_T("SELECT BB.SPECIES_KEY, %s, %s ")
		_T("FROM %s ")
		_T("WHERE BB.REC_KEY = %s AND BB.MANAGE_CODE = '%s' AND ROWNUM = 1"),
		strVolInfoColumn,
		strReverseUserCnt,
		strBookTbl,
		strBookKey,
		strManageCode
		);

	int ids = -1;

	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	ids = TmpDM.RestructDataManager(strQuery);
	
	CString strSpeciesKey;
	TmpDM.GetCellData(0, 0, strSpeciesKey);

	CString strVolInfo;
 	TmpDM.GetCellData(0, 1, strVolInfo);

	CString strReversionCnt;
	TmpDM.GetCellData(0, 2, strReversionCnt);
	
	if ( m_pLoanMangeValue->m_bVolReserve && FALSE == strVolInfo.IsEmpty())
	{		
		CString sQuery;
		sQuery.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL ") 
			_T(" WHERE CL.BOOK_KEY IN ( SELECT REC_KEY FROM %s WHERE SPECIES_KEY = %s ) ")
			_T("   AND CL.REG_NO IS NULL ")
			_T("   AND CL.STATUS = '3' ")
			_T("   AND CL.VOL_INFO = %s "),
			strBookTbl, strSpeciesKey, strVolInfo);

		TmpDM.GetOneValueQuery(sQuery, strReversionCnt);
	}
		
	if( 0 == _ttoi(strReversionCnt))  return 0;
	if ( !(m_pLoanMangeValue->m_bVolReserve) )
	{
		CString sSQL;
		sSQL.Format( _T("SELECT REC_KEY FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3' AND RESERVATION_DATE = ")
			_T("(SELECT MIN(RESERVATION_DATE) FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3')") , strBookKey, strBookKey);
		
		CString sLoanKey;
		TmpDM.GetOneValueQuery(sSQL, sLoanKey);
		
		
		if( sLoanKey.IsEmpty() )
			return 0;
	}
	
	INT nYes = MessageBox(_T("�� �ڷ�� ����� �ڷ��Դϴ�. ���� ó���� �Ͻðڽ��ϱ�?"), _T("���� ó��"), MB_YESNO | MB_ICONQUESTION);
	if( nYes == IDYES )
	{		
		if(!(m_pLoanMangeValue->m_bVolReserve))
		{
			CBO_LOC_3170 Dlg(this, m_pLoanShareManager , m_pLoanMangeValue , 0);
			Dlg.pMain = pMain;
			Dlg.pParentMgr = (CESL_Mgr*)this;
			Dlg.m_pInfo = m_pInfo;
			Dlg.SetInfo(strSpeciesKey, strVolInfo, strPublishFormCode, strBookKey);
			Dlg.m_nReserveInfoMode	=	1;
			Dlg.DoModal();
		}
		else 
		{
			CBO_LOC_3170_ReserveInfo Dlg(this, m_pLoanShareManager , m_pLoanMangeValue , 0);
			Dlg.pMain = pMain;
			Dlg.pParentMgr = (CESL_Mgr*)this;
			Dlg.m_pInfo = m_pInfo;
			Dlg.SetInfo(strSpeciesKey, strVolInfo, strPublishFormCode, _T(""));
			Dlg.m_nReserveInfoMode	=	1;
			Dlg.m_strReturnBookKey	=	strBookKey;
			Dlg.m_strReturnBookRegNo	=	strRegNo;
			

			Dlg.DoModal();
		}
	}
	
	return 0;

	EFS_END
		return -1;
}

void CBOOK_COOPERATIVE_GROUP_OFFER::OnDropdowncmbOFFER() 
{	
	int ids = -1;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::OnDropdowncmbREQUEST");

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("��Ƽ�޺�_�������"));
	if(NULL == pCMB) ERROR_MSG_RETURN_VOID2(-1, strFuncName);

	int nRow = pCMB->GetCurSel();
	if(-1 != nRow)
	{
		CString strData = pCMB->GetItemDataString(nRow, 0);
		if(_T("    ") == strData)
		{
			ResetComboBox();
		}
	}		
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Get_TblName(CString &strTblName, CString strMode, CString strSearchState)
{
	int ids = -1;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::Get_TblName");

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("��Ƽ�޺�_�������"));
	if(NULL == pCMB) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CString strStateCode, strStateDesc;
	if(strSearchState.IsEmpty())
	{
		const int nIdx = pCMB->GetCurSel();

		strStateCode = pCMB->GetItemDataString(nIdx, 0);
		if(_T("") == strStateCode) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
		
		strStateDesc = pCMB->GetItemDataString(nIdx, 1);
		if(_T("") == strStateCode) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
	}
	else
	{
		strStateCode = strSearchState;
	}
	
	CString strTableName = _T("");	
	if(	_T("0002") == strStateCode		
		|| _T("0004") == strStateCode	
		|| _T("0014") == strStateCode	
		|| _T("0008") == strStateCode	
		|| _T("0011") == strStateCode	
 		|| _T("0012") == strStateCode	
 		|| _T("0009") == strStateCode	
	)
	{		
		CString strStateQuery;
		if(_T("0011") == strStateCode && _T("�߼۰������ڷ�") == strStateDesc)
		{
			strStateQuery = _T(" AND END_DIV IS NULL");
		}
		else if( 
			(_T("0011") == strStateCode && _T("�ֱٹ߼۰���Ȯ�ε��ڷ�") == strStateDesc)
			|| (_T("0012") == strStateCode && _T("�ֱٿ�û�������ڷ�") == strStateDesc)
			|| _T("0009") == strStateCode
			)
		{
			strStateQuery = _T("AND END_DATE BETWEEN TO_CHAR(ADD_MONTHS(SYSDATE, -3), 'YYYY/MM/DD') AND SYSDATE AND END_DIV IS NOT NULL");

		}	
			
		strTableName.Format(
			_T("(SELECT ITL.*, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")
			_T("FROM ILL_TRANS_TBL ITL WHERE ITL.GIVE_LIBCODE = '%s' AND ITL.STATE = '%s'%s) A"),
			m_strLibCode, strStateCode, strStateQuery);
	}
	else if(_T("----") == strStateCode)
	{
		CString strState = _T("'0002', '0004', '0008', '0011', '0014'");

		strTableName.Format(
			_T("(SELECT ITL.*, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")			
			_T("FROM ILL_TRANS_TBL ITL WHERE ITL.GIVE_LIBCODE = '%s' ")
			_T("AND ITL.STATE IN (%s) AND ITL.END_DIV IS NULL) A")
			, m_strLibCode, strState);
	}
	else if(_T("    ") == strStateCode)
	{
		strTableName = m_strTableName;
	}
	else if(_T("ALL") == strStateCode)
	{
		strTableName = 
			_T("(SELECT ITL.*, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.APL_LIBCODE AND ROWNUM = 1) AS APL_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.RETURN_LIBCODE AND ROWNUM = 1) AS RETURN_MANAGE_CODE, ")
				_T("(SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ITL.GIVE_LIBCODE AND ROWNUM = 1) AS GIVE_MANAGE_CODE ")
			_T("FROM ILL_TRANS_TBL  ITL ) A");
	}
	else
	{
		CString strFormat = _T("");
		strFormat.Format(_T("�� ������ �ڵ� : %s"), strStateCode);
		AfxMessageBox(strFormat);

		ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);		
	}
	if(_T("MO") == strMode)
	{
		strTableName += _T(", BO_BOOK_TBL BB, IDX_BO_TBL IB, CO_LOAN_USER_TBL CLU");
	}
	else if(_T("SE") == strMode)
	{
		strTableName += _T(", SE_BOOK_TBL BB, IDX_SE_TBL IB, SE_VOL_TBL V, CO_LOAN_USER_TBL CLU");
	}
	else if(_T("NB") == strMode)
	{
		strTableName += _T(", CO_NON_DB_BOOK_TBL BB, CO_LOAN_USER_TBL CLU");
	}
	strTblName = strTableName;

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::ResetComboBox()
{
	int ids = -1;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_OFFER::ResetComboBox");

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("��Ƽ�޺�_�������"));
	if(NULL == pCMB) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	const int COL_COUNT = 2;
	pCMB->ResetContent();
	pCMB->FreeComboData();
	pCMB->InitMultiComboBox(COL_COUNT);	

	const int STATE_COUNT = 8;
	CString strCmbData[STATE_COUNT][COL_COUNT] = 
	{
		{ _T("----"), _T("��ü(������ �ڷ�)") },		
		{ _T("0002"), _T("��û���ڷ�")},
		{ _T("0004"), _T("�߼۵��ڷ�")},
		{ _T("0008"), _T("�������ڷ�")},
		{ _T("0014"), _T("�Լ����ڷ�")},
		{ _T("0009"), _T("�ֱٿϷ���ڷ�")},
		{ _T("0011"), _T("�߼۰������ڷ�")},
		{ _T("0011"), _T("�ֱٹ߼۰���Ȯ�ε��ڷ�")}
	};
	
	for(int nRow = 0; nRow < STATE_COUNT; nRow++)
	{
		for(int nCol = 0; nCol < 2; nCol++)
			pCMB->AddItem(strCmbData[nRow][nCol], nCol, nRow);
	}

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::Compare()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() == 0)
	{
		m_pGrid->SelectRemoveAll();
		INT nRows = m_pDM->GetRowCount();		
		for (int i = 0; i < nRows; i++)
		{
			m_pGrid->m_arraySelect.AddTail(i);
		}
		m_pGrid->m_arrayPos = m_pGrid->m_arraySelect.GetHeadPosition();
	}
 	int nCnt = m_pGrid->SelectGetCount();
 	if(0 >= nCnt) 
 	{
 		AfxMessageBox(_T("�˻��� ������ �������� �ʽ��ϴ�."));
 		return -1;
 	}

	CILL_COMPARE Dlg(this);
	Dlg.m_pInfo = m_pInfo;
	Dlg.SetMode(CILL_COMPARE::IDX_OFFER);

	if(IDOK == Dlg.DoModal())
	{
		ExcutePath(_T("����_�߼۰���"));
	}
	return 0;
}

void CBOOK_COOPERATIVE_GROUP_OFFER::Search(WPARAM wParam, LPARAM lParam)
{
	CILL_SEARCH* pDlg = (CILL_SEARCH*)wParam;

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("��Ƽ�޺�_�������"));
	if(NULL == pCMB) ERROR_MSG_RETURN_VOID2(-1, _T("Search"));
	
	const int ROW = pCMB->GetRowCount();
	pCMB->AddItem(_T("    "), 0, ROW);
	pCMB->AddItem(_T("�˻����ڷ�"), 1, ROW);
	pCMB->SetCurSel(ROW);
	
 	Refresh(_T("ALL"), pDlg->m_strSQLWhere);

}

INT CBOOK_COOPERATIVE_GROUP_OFFER::Make_InsertLoanTblQuery(pStruMakeInsertTbl pStru)
{
	EFS_BEGIN

	if(NULL == pStru->pDM)
	{
		return -1000;
	}

	pStru->pDM->InitDBFieldData();

	
	INT ids = 0;
	const INT nSrcFieldCnt = 15;
	CString StrSrcField[nSrcFieldCnt];
	TCHAR *sDstFieldAlias[nSrcFieldCnt] = 
	{
		_T("BOOK_KEY")	  , _T("SHELF_LOC_CODE"), _T("PUBLISH_FORM_CODE"), _T("CLASS_NO_TYPE"), _T("CLASS_NO")			, 
		_T("MANAGE_CODE"), _T("REG_CODE")		 , _T("SPECIES_KEY")	   , _T("REG_NO")		 , _T("SEPARATE_SHELF_CODE")	, 
		_T("TITLE")		  , _T("AUTHOR")		 , _T("PUBLISHER")		   , _T("CALL_NO")		 , _T("VOL_INFO")		
	};
	TCHAR *sDstFieldTypeAlias[nSrcFieldCnt] = 
	{
		_T("NUMERIC")	  , _T("STRING")         , _T("STRING")	           , _T("STRING")        , _T("STRING"), 
		_T("STRING")	  , _T("STRING")         , _T("NUMERIC")	       , _T("STRING")        , _T("STRING"),
		_T("STRING")	  , _T("STRING")         , _T("STRING")	           , _T("STRING")        , _T("NUMERIC")
	};

	CString strSQL;
	if(_T("MO") == pStru->strInPublishFormCode)
	{		
		strSQL.Format(_T("SELECT ")
			_T(" BB.REC_KEY, BB.SHELF_LOC_CODE, 'MO', BB.CLASS_NO_TYPE, BB.CLASS_NO, ")
			_T(" BB.MANAGE_CODE, BB.REG_CODE, BB.SPECIES_KEY, BB.REG_NO, BB.SEPARATE_SHELF_CODE, ")
			_T(" IB.TITLE_INFO, IB.AUTHOR_INFO, IB.PUB_INFO, ECO$F_MAKE_CALLNO$I(BB.SEPARATE_SHELF_CODE, BB.CLASS_NO, BB.BOOK_CODE, BB.VOL_CODE, BB.COPY_CODE), BB.VOL_SORT_NO ")
			_T("FROM BO_BOOK_TBL BB, IDX_BO_TBL IB WHERE BB.SPECIES_KEY = IB.REC_KEY AND BB.REC_KEY = %s"),
			pStru->strInBookKey);
	}
	else if(_T("SE") == pStru->strInPublishFormCode)
	{		
		strSQL.Format(_T("SELECT ")
			_T(" BB.REC_KEY, BB.SHELF_LOC_CODE, 'SE', BB.CLASS_NO_TYPE, BB.CLASS_NO, ")
			_T(" BB.MANAGE_CODE, BB.REG_CODE, BB.SPECIES_KEY, BB.REG_NO, BB.SEPARATE_SHELF_CODE, ")
			_T(" IB.TITLE_INFO, IB.AUTHOR_INFO, IB.PUB_INFO, ECO$F_MAKE_CALLNO$I(BB.SEPARATE_SHELF_CODE, BB.CLASS_NO, BB.BOOK_CODE, BB.VOL_CODE, BB.COPY_CODE), BB.VOL_KEY ")
			_T("FROM SE_BOOK_TBL BB, IDX_SE_TBL IB WHERE BB.SPECIES_KEY = IB.REC_KEY AND BB.REC_KEY = %s"),
			pStru->strInBookKey);
	}
	pStru->pDM->GetValuesQuery(strSQL, StrSrcField, nSrcFieldCnt);
	
	for( INT i= 0 ; i < nSrcFieldCnt ;i++ )
	{
		if( StrSrcField[i].IsEmpty() && _T("NUMERIC") == sDstFieldTypeAlias[i])
		{
			StrSrcField[i] = _T("NULL");
		}

		ids = pStru->pDM->AddDBFieldData( sDstFieldAlias[i] , sDstFieldTypeAlias[i] , StrSrcField[i], TRUE );
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("Make_InsertLoanTblQuery"));
		}
	}

	
	const INT nSrcUserFieldCnt = 4;
	CString StrUserSrcField[nSrcUserFieldCnt];
	TCHAR *sDstUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("USER_KEY")  , _T("USER_POSITION_CODE"), _T("USER_CLASS_CODE"), _T("MEMBER_CLASS") 
	};
	TCHAR *sDstUserFieldTypeAlias[nSrcUserFieldCnt] = 
	{
		_T("NUMERIC")   , _T("STRING")                  , _T("STRING")               , _T("STRING") 
	};
	strSQL.Format(_T("SELECT ")
		_T(" CLU.REC_KEY, CLU.USER_POSITION_CODE, CLU.USER_CLASS_CODE, CLU.MEMBER_CLASS ")
		_T(" FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY = %s"), pStru->strInUserKey);
	pStru->pDM->GetValuesQuery(strSQL, StrUserSrcField, nSrcUserFieldCnt);

	for( i= 0 ; i < nSrcUserFieldCnt ;i++ )
	{
		ids = pStru->pDM->AddDBFieldData( sDstUserFieldAlias[i] , sDstUserFieldTypeAlias[i] , StrUserSrcField[i], TRUE );
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("InsertLoanRecord"));
		}
	}

	pStru->pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), _T("O"));
	pStru->pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), pStru->strInLoanKey);
	pStru->pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�߼�"), __WFILE__, __LINE__));
	pStru->pDM->MakeInsertFrame(_T("LS_WORK_T01"));

	return 0;

	EFS_END
		return -1;
}

int CBOOK_COOPERATIVE_GROUP_OFFER::RefreshState()
{
	CString strCaption;
	GetDlgItemText(IDC_BTN_OFFER_STATE, strCaption);
	if(_T("��") == strCaption)
	{
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(_T(""));

		CString strQuery;


		strQuery.Format(
			_T("SELECT T.STATE, COUNT(1) ")
			_T("FROM ( ")
				_T("SELECT A.STATE ")
				_T("FROM ILL_TRANS_TBL A, BO_BOOK_TBL BB, IDX_BO_TBL IB, CO_LOAN_USER_TBL CLU ")
				_T("WHERE A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY = IB.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.END_DIV IS NULL AND GIVE_LIBCODE = '%s' ")
				_T("UNION ALL ")
				_T("SELECT A.STATE ")
				_T("FROM ILL_TRANS_TBL A, SE_BOOK_TBL BB, IDX_SE_TBL IB, SE_VOL_TBL V, CO_LOAN_USER_TBL CLU ")
				_T("WHERE A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY=IB.REC_KEY AND BB.VOL_KEY=V.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.PUBLISH_FORM_CODE = 'SE' AND GIVE_LIBCODE = '%s' ")
				_T("UNION ALL ")
				_T("SELECT A.STATE ")
				_T("FROM ILL_TRANS_TBL A, CO_NON_DB_BOOK_TBL BB, CO_LOAN_USER_TBL CLU ")
				_T("WHERE A.BOOK_KEY = BB.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.PUBLISH_FORM_CODE = 'NB' AND GIVE_LIBCODE = '%s' ")
			_T(") T ")
			_T("GROUP BY T.STATE"),
			m_pInfo->LIB_CODE, m_pInfo->LIB_CODE, m_pInfo->LIB_CODE);

		TmpDM.RestructDataManager(strQuery);

		CString strState, strCnt, strCaption;
		INT nRow = 0;
		INT ROW_COUNT = TmpDM.GetRowCount();
		INT nCnt = 0;
		for(nCnt = 0; nCnt < 5; nCnt++)
		{
			for(nRow = 0; nRow < ROW_COUNT; nRow++)
			{
				strState = TmpDM.GetCellData(nRow, 0);
				strCnt = TmpDM.GetCellData(nRow, 1);

				if(strState == m_strState[nCnt][1])
				{
					strCaption = m_strState[nCnt][0] + _T("(") + strCnt + _T(")");
					SetDlgItemText(4500+nCnt,strCaption);
					break;
				}
			}
			if(ROW_COUNT <= nRow)
			{
				SetDlgItemText(4500+nCnt,m_strState[nCnt][0]);
			}
		}

	}

	return 0;
}
void CBOOK_COOPERATIVE_GROUP_OFFER::OnBtnOfferState() 
{
	
	CString strCaption;
	GetDlgItemText(IDC_BTN_OFFER_STATE, strCaption);
	if(_T("��") == strCaption)
	{
		SetDlgItemText(IDC_BTN_OFFER_STATE, _T("��"));
	}
	else if(_T("��") == strCaption)
	{
		SetDlgItemText(IDC_BTN_OFFER_STATE, _T("��"));

		RefreshState();
	}

	ResizeControl();
}
void CBOOK_COOPERATIVE_GROUP_OFFER::OnBtnState(UINT uID)
{
	CString strState = m_strState[uID-4500][1];

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("��Ƽ�޺�_�������"));
	if(NULL == pCMB) ERROR_MSG_RETURN_VOID2(-1, _T("OnBtnState"));

	CString strData = _T("");		
	int ROW = pCMB->GetCount();
	for(int nCnt = 0; nCnt < ROW; nCnt++)
	{
		strData = pCMB->GetItemDataString(nCnt, 0);
		if(strState == strData)
		{
			strData = pCMB->GetItemDataString(nCnt, 1);
			if(0 > strData.Find(_T("�ֱ�")))
			{
				pCMB->SetCurSel(nCnt);
				break;
			}
		}
	} 	

	OnSelchangecmbOFFER();
}

LRESULT CBOOK_COOPERATIVE_GROUP_OFFER::OnExcute(WPARAM wParam, LPARAM lParam)
{
	CString* strMessage = (CString*)lParam;
	CDataMgr* pDataMgr = (CDataMgr*)wParam;

	if(_T("�߼�") == pDataMgr->GetDataAlias())
	{
		return Send(pDataMgr, strMessage);
	}
	else if(_T("���ͿϷ�") == pDataMgr->GetDataAlias())
	{
		return Return_Complete(pDataMgr, strMessage);
	}

	return 1000;
}


INT CBOOK_COOPERATIVE_GROUP_OFFER::PreReturn_Complete(CDataMgr* pDataMgr, CString* pstrMessage)
{
	int ids = 0;
	CString strFuncName = _T("PreReturn_Complete");
	
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("����|NOT IN|0004,0008,0014"));
	aCheckArray.Add(_T("�������|==|������"));
	aCheckArray.Add(_T("����������_�ڵ�|!=|%m_strLibCode%"));
	
	ids = UnCheckGrid(aCheckArray);
	}
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("������ �׸��� ����(�Ϸ�) ������ �׸��� �����ϴ�.\r\n")
			_T("����(�Ϸ�) ������ ����� �Ʒ��� �����ϴ�.\r\n\r\n")
			_T("1. �ڰ����� ������ �ڷ�\r\n")
			_T("2. ������°� �߼۵��ڷ�(0004), �Լ����ڷ�(0014), �������ڷ�(0008)�� Ʈ�����\r\n")
			_T("3. ���� �������� �ƴ� �ڷ�")
			);
		return -1;
	}
	else
	{
		if(NULL == pDataMgr)
		{
			return -1020;
		}

		CString strGet;
		INT nSetIdx = 0;
		INT nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{	
			pDataMgr->InsertData(-1);	
			nSetIdx = pDataMgr->GetCount();
			m_pDM->GetCellData(_T("Ʈ����ǹ�ȣ"), nIdx, strGet);			
			pDataMgr->SetData(_T("Ʈ����ǹ�ȣ"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("åKEY"), nIdx, strGet);
			pDataMgr->SetData(_T("åKEY"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("�̿���KEY"), nIdx, strGet);			
			pDataMgr->SetData(_T("�̿���KEY"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("�������±���"), nIdx, strGet);
			pDataMgr->SetData(_T("�������±���"), strGet, nSetIdx);

			m_pDM->GetCellData(_T("����������_��������"), nIdx, strGet);
			pDataMgr->SetData(_T("����������_��������"), strGet, nSetIdx);

			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);

	}
	
	return 0;
}

INT CBOOK_COOPERATIVE_GROUP_OFFER::Return_Complete(CDataMgr* pDataMgr, CString* pstrMessage)
{
	int ids = 0;
	CString strFuncName = _T("Return_Complete");

	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	INT CNT = pDataMgr->GetCount();

	for(INT nIdx = 0; nIdx < CNT; nIdx++)
	{
		TmpDM.StartFrame();
		CString strTransNo = _T("");
		
		{
		
		pDataMgr->GetData(_T("Ʈ����ǹ�ȣ"), nIdx, strTransNo);

		CString strQuery = _T("");
		
		strQuery.Format(
			_T("UPDATE ")
				_T("ILL_TRANS_TBL ")
			_T("SET ")
				_T("STATE = '0009', ")	
				_T("END_USERID = '%s', ") 
				_T("END_DATE = SYSDATE, ")
				_T("END_DIV = '0010', ") 
				_T("LAST_WORK = '%s' ")
			_T("WHERE ")
				_T("TRANS_NO = '%s';"),
				m_pInfo->USER_ID,
				GetWorkLogMsg( _T("����(�Ϸ�)"), __WFILE__, __LINE__ ),
				strTransNo
			);
		TmpDM.AddFrame(strQuery);
		}
		
		
		{

		CString strQuery = _T("");
		ids = Get_UpdateBookTblQuery_For_CompleteTransaction(
			pDataMgr,
			nIdx,
			GetWorkLogMsg( _T("����(�Ϸ�)"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
		TmpDM.AddFrame(strQuery);
		
		ids = Get_UpdateIdxTblQuery(
			pDataMgr,
			nIdx,
			GetWorkLogMsg( _T("����(�Ϸ�)"), __WFILE__, __LINE__ ),
			strQuery
			);
		if(0 > ids)
		{
			pstrMessage->Format(_T("DB ������Ʈ�� �����Ͽ����ϴ�.[%d]"), ids);
			return ids;
		}
		else if(0 == ids)
		{
			TmpDM.AddFrame(strQuery);
		}

		}
		
		ids = TmpDM.SendFrame(TRUE);
		TmpDM.EndFrame();

		if(0 > ids)
		{
			pstrMessage->Format(_T("DB ������Ʈ�� �����Ͽ����ϴ�.[%d]"), ids);
			return ids;
		}

		if(_T("D") == m_sBookCooperativeMode)
		{
			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,END_USERID,END_DATE,END_DIV"));				
		}		

		Reversion(pDataMgr, nIdx);
	}

	CString strNO;
	MakeTransNoList(pDataMgr, strNO);
	

	if(0 != m_pGrid->SelectGetCount())
	{
		return RFID_SecurityChange(strNO, TRUE, TRUE);
	}

	return 0;
}


INT CBOOK_COOPERATIVE_GROUP_OFFER::MakeValidRegNo(CString &strGetRegNo)
{
EFS_BEGIN

	
	if( strGetRegNo.IsEmpty() )
	{
		AfxMessageBox( _T("�ݳ� ��� �ڷ��� ��Ϲ�ȣ�� �Է��ؾ� �մϴ�") );
		return -1001;
	}

	CString strTmp;
	INT ids = 0;
	CPreRegCodeMgr *preRegCodeMgr = new CPreRegCodeMgr(this);
	ids = preRegCodeMgr->IsInPreRegNo( strGetRegNo , m_pInfo->MANAGE_CODE, strTmp );
	if(1 == ids)
	{
		CLocCommonAPI::MakeRegNoSpaceToZero(this, strGetRegNo, NULL);
	}
	else
	{
		
		INT nLoopCount = m_pManageCodeList->GetRowCount();
		CString strManageCode = _T("");
		for(INT i=0; i<nLoopCount; i++)
		{
			strManageCode = m_pManageCodeList->GetCellData(i, 0);
			ids = preRegCodeMgr->IsInPreRegNo( strGetRegNo , strManageCode, strTmp );
			if(1 == ids)
			{
				CLocCommonAPI::MakeRegNoSpaceToZero(this, strGetRegNo, strManageCode, NULL);
				break;
			}
		}
	}
	if ( preRegCodeMgr != NULL ) 
	{
		delete preRegCodeMgr;
	}

	strTmp = m_pManageCodeList->GetCellData(0, 0);

	
	INT		nRegNoLength = strGetRegNo.GetLength();

	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;

	if( 12 < nRegNoLength || nRegNoLength < nRegCodeLength)
	{
		AfxMessageBox(_T("�߸��� ��Ϲ�ȣ�Դϴ�."));
		strGetRegNo.Empty();
		return -1010;
	}
	else if( 12 > nRegNoLength )
	{		
		CString strRegCode = strGetRegNo.Mid(0,nRegCodeLength);
		CString strRegNum  = strGetRegNo.Mid(nRegCodeLength);

		strGetRegNo = m_pInfo->MakeRegNo(strRegCode, strRegNum);
	}
	
	
	
	return 0;

EFS_END
return -1;
}

INT CBOOK_COOPERATIVE_GROUP_OFFER::SelectSearchHistory()
{
EFS_BEGIN

	INT ids;
	CString strBookRegNo;
	
	CString strInBarcodeLibCode, strInBarcode, strQuery, strDMLibCode;
	GetDlgItemText(IDC_edtRegNo, strInBarcode);
	
	BOOL bSelected = FALSE;

	if( strInBarcode.IsEmpty() )
	{
		AfxMessageBox( _T("�����ڷ��� ��Ϲ�ȣ�� �Է��ؾ� �մϴ�") );
		return -1001;
	}

	
	if ( strInBarcode.GetLength() == 19 ) {
		strInBarcodeLibCode = strInBarcode.Mid(1, 6);	
		strBookRegNo = strInBarcode.Mid(7);		
	}
	else {
		strBookRegNo = strInBarcode;
		
		ids = MakeValidRegNo( strBookRegNo );
		if( 0 > ids )	return -1010;
		strInBarcodeLibCode = _T("");
	}	

	
	CESL_DataMgr *pDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SelectSearchHistory()") );
			
	if( pDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("�˻��� ����Ÿ�� �����ϴ�") );
		return -1001;
	}
	else
	{
		for( int i = 0; i < pDM->GetRowCount(); i++ )
		{
			CString strGridBookRegNo;
			pDM->GetCellData( _T("��Ϲ�ȣ"), i, strGridBookRegNo );
			if( strGridBookRegNo == strBookRegNo )
			{
				
				if ( strInBarcodeLibCode.GetLength() > 0 ) {
					pDM->GetCellData( _T("����������_�ڵ�"), i, strDMLibCode );
					if ( strInBarcodeLibCode == strDMLibCode ) {
						
						CESL_Grid* pgrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("�׸���_�����ڷ�") );
						pgrid->SetAt(i, pgrid->m_nSelectIdx, pgrid->m_sSelectString);
						
						
						
						pDM->SetCellData(_T("����"), pgrid->m_sSelectString, i);
						bSelected = TRUE;
						break;
					}
				}
				else {
					
					CESL_Grid* pgrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("�׸���_�����ڷ�") );
					pgrid->SetAt(i, pgrid->m_nSelectIdx, pgrid->m_sSelectString);
					
					
					
					pDM->SetCellData(_T("����"), pgrid->m_sSelectString, i);
					bSelected = TRUE;
					break;
				}
			}
		}
		
		
		for( int j = 0; j< pDM->GetRowCount(); j++ )
		{
			CString strCheck;
			CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("�׸���_�����ڷ�") );
			pGrid->GetAt(j,0,strCheck);
			if(strCheck == _T(""))
				pDM->SetCellData(_T("����"),_T(""), j);
		}
	}
	
	
	
	if(bSelected)
	{
		pDM->SORT(_T("����"), TRUE);
		CESL_Grid* pgrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER"), _T("�׸���_�����ڷ�") );
		pgrid->Display();
	}
	else
	{
		AfxMessageBox( _T("������ �����Ͱ� �����ϴ�") );		
	}
	return 0;
EFS_END
return -1;
}

void CBOOK_COOPERATIVE_GROUP_OFFER::OnbtnSelect() 
{
EFS_BEGIN

	SelectSearchHistory();
	
	SetDlgItemText(IDC_edtRegNo, _T(""));
	( CEdit*) GetDlgItem (IDC_edtRegNo)->SetFocus();

EFS_END
}

INT CBOOK_COOPERATIVE_GROUP_OFFER::PrintOfferList()
{
EFS_BEGIN
	
	INT ids;
	
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}
		
	if( PrivacyPrintLogRecord(pSrcDM, _T("å�η����� �߼� �������"), _T("E"), 0 ) <0 ) return 1;
		
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER") , _T("�׸���_�����ڷ�") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();

	if ( nSelectCnt > 6 ) {
		AfxMessageBox( _T("�ִ� 6���� �ڷ������ �����մϴ�.") );
		return 1;
	}

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	pPrintDM->FreeData();
	
	const INT nCopyCnt = 7;
	TCHAR *sField[nCopyCnt][2] =
	{		
		{_T("�̿���_��ȣ"),_T("�̿���_��ȣ")},
		{_T("�̿��ڸ�"),_T("�̿��ڸ�")},		
		{_T("����"),_T("����")},		
		{_T("��������"),_T("��������")},
		{_T("��û������_�̸�"),_T("��û������_�̸�")},
		{_T("����������_�̸�"),_T("����������_�̸�")},
		{_T("�߼���"),_T("�߼���")}
	};
		
	CString sData;
	INT nIndex;

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("��ȣ") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] ,j, sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , j, sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}

		if( nMode == 1)
			pGrid->SelectGetNext();
	}

	
	BOOL bRight = TRUE;
	ids = IsRightPrint(bRight);
	if ( ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("PrintList") );

	if ( bRight == FALSE ) {
		AfxMessageBox( _T("������ �ڷ��߿��� �߼��� �Ǵ� ���⵵������ �ٸ� �ڷᰡ �����մϴ�. Ȯ���Ͽ� �ֽʽÿ�.") );
		return 1;
	}
		
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);

	
	ids = pSIReportManager->GetSIReportFileFromDB(_T("å�η�_�߼�����"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 1;
	}
	INT iddd = pPrintDM->GetColCount();

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->PrintOfferList();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;
	
EFS_END
	return -1;
}

INT CBOOK_COOPERATIVE_GROUP_OFFER::IsRightPrint(BOOL& bIsRight)
{
EFS_BEGIN
	INT i = -1, nRowCnt = 0;
	CString strFuncName = _T("IsRightPrint");	
	CString strGiveDate1 = _T(""), strGiveDate2 = _T("");
	CString strAplLib1 = _T(""), strAplLib2 = _T("");

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , strFuncName );

	bIsRight = TRUE;

	nRowCnt = pPrintDM->GetRowCount();
	for ( i=0; i<nRowCnt; i ++ ) {

		
		if ( i == 0 ) pPrintDM->GetCellData(_T("�߼���"), i, strGiveDate1);
		pPrintDM->GetCellData(_T("�߼���"), i, strGiveDate2);

		if ( strGiveDate1 != strGiveDate2) {
			bIsRight = FALSE;
			break;
		}

		if ( strGiveDate1.GetLength() == 0 ) {
			bIsRight = FALSE;
			break;
		}

		
		if ( i == 0 ) pPrintDM->GetCellData(_T("��û������_�̸�"), i, strAplLib1);
		pPrintDM->GetCellData(_T("��û������_�̸�"), i, strAplLib2);
		
		if ( strAplLib1 != strAplLib2) {
			bIsRight = FALSE;
			break;
		}

		if ( strAplLib1.GetLength() == 0 ) {
			bIsRight = FALSE;
			break;
		}
	}

	return 0;
EFS_END
	return -1;
}


INT CBOOK_COOPERATIVE_GROUP_OFFER::PrintList()
{
EFS_BEGIN
	
	INT ids;
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}
	
	PrivacyPrintLogRecord(pSrcDM, _T("å�η� ���� ������"), _T("E"), 0 );
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BOOK_COOPERATIVE_GROUP_OFFER") , _T("�׸���_�����ڷ�") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
	CESL_DataMgr *pPrintDM = FindDM( _T("DM_BOOK_COOPERATIVE_GROUP_OFFER_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	pPrintDM->FreeData();
	
	const INT nCopyCnt = 12;

	TCHAR *sField[nCopyCnt][2] =
	{		
		{_T("�̿���_��ȣ"),_T("�̿���_��ȣ")},
		{_T("�̿��ڸ�"),_T("�̿��ڸ�")},
		{_T("��Ϲ�ȣ"),_T("��Ϲ�ȣ")},
		{_T("����"),_T("����")},
		{_T("�ηϿ���"),_T("�ηϿ���")},
		
		
		{_T("��ü����"),_T("��ü����")},
		{_T("û����ȣ"),_T("û����ȣ")},
		{_T("�ڷ��_����"),_T("�ڷ��_����")},
		{_T("�������"),_T("�������")},
		{_T("��û��"),_T("��û��")},
		{_T("��û������_�̸�"),_T("��û������_�̸�")},
		{_T("�ݳ�������_�̸�"),_T("�ݳ�������_�̸�")}
	};
		
	CString sData;
	INT nIndex;

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("��ȣ") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , j,  sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] ,j , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}

		if( nMode == 1)
			pGrid->SelectGetNext();
	}	
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pPrintDM , _T("��������") , _T("��������") );
	
	pPrintDM->SORT(_T("û����ȣ"));

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);

	
	ids = pSIReportManager->GetSIReportFileFromDB(_T("å�η�_����"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 1;
	}
	INT iddd = pPrintDM->GetColCount();

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;
	
EFS_END
	return -1;
}

INT CBOOK_COOPERATIVE_GROUP_OFFER::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	
	return 0;
	
	EFS_END
	return -1;	
}
BEGIN_EVENTSINK_MAP(CBOOK_COOPERATIVE_GROUP_OFFER, CDialog)
//{{AFX_EVENTSINK_MAP(CBOOK_COOPERATIVE_GROUP_OFFER)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

INT CBOOK_COOPERATIVE_GROUP_OFFER::Send_Refuse_Cancel()
{
		
	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return -1;
	}

	CStringArray aCheckArray;
	aCheckArray.Add(_T("����|!=|0011"));
	aCheckArray.Add(_T("����������_�ڵ�|!=|%m_strLibCode%"));

	UnCheckGrid(aCheckArray);
	}
		
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("������ �׸��� �߼���� ������ �׸��� �����ϴ�.\r\n")
			_T("�߼���� ������ ����� �Ʒ��� �����ϴ�.\r\n\r\n")
			_T("1. ������������ �ڰ��� ���\r\n")
			_T("2. ������°� �߼۰����� �ڷ�(0011)�� Ʈ�����\r\n")
			);
		return -1;
	}


	CString strNO = Get_Select_TRANS_NO_List(TRUE);

	if(_T("") == strNO) return -1;

 	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	if(_T("I") == m_sBookCooperativeMode)
	{
		CString strNO = Get_Select_TRANS_NO_List(TRUE);		
		if(_T("") == strNO) return -1;

		int nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{	

			TmpDM.StartFrame();

			CString strQuery;
			strQuery.Format(
				_T("UPDATE ILL_TRANS_TBL SET ")
				_T("LOAN_KEY = NULL, ")
				_T("STATE = '0002', ")
				_T("GIVE_USERID = '%s', ")  
				_T("GIVE_DATE = NULL, ")	
				_T("GIVE_CANCEL = NULL, ")	
				_T("WORKNO = NULL, ")
				_T("LAST_WORK = '%s' ")
				_T("WHERE ")
				_T("TRANS_NO IN (%s) AND STATE = '0011' AND GIVE_LIBCODE = '%s';"),
				m_pInfo->USER_ID,
				GetWorkLogMsg( _T("�߼۰������"), __WFILE__, __LINE__ ),
				strNO,
				m_strLibCode
				);

			TmpDM.AddFrame(strQuery);
			int ids = TmpDM.SendFrame();
			TmpDM.EndFrame();

			
			CString strBookKey = _T("");
			CString strUserKey = _T("");
			
			CString strWorkingStatus = _T("BOL112N");

			m_pDM->GetCellData(_T("åKEY"), nIdx, strBookKey);
			m_pDM->GetCellData(_T("�̿���KEY"), nIdx, strUserKey);

			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ")
				_T("WORKING_STATUS = PREV_WORKING_STATUS, ")
				_T("PREV_WORKING_STATUS = '%s' ")
				_T("WHERE ")
				_T("REC_KEY = %s AND MANAGE_CODE = '%s';"),
				strWorkingStatus,strBookKey,m_pInfo->MANAGE_CODE );

			TmpDM.StartFrame();
			TmpDM.AddFrame(strQuery);
			TmpDM.SendFrame();
			TmpDM.EndFrame();
			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);

	}	
	else if(_T("D") == m_sBookCooperativeMode)
	{

		CString strLoanKey, strBookKey, strUserKey, strTransNo;
		int nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{	
			m_pDM->GetCellData(_T("Ʈ����ǹ�ȣ"), nIdx, strTransNo);
			m_pDM->GetCellData(_T("��������_Ű"), nIdx, strLoanKey);

			TmpDM.StartFrame();				
			CString strQuery;
			strQuery.Format(
				_T("UPDATE ILL_TRANS_TBL SET ")
				_T("STATE = '0002', ")
				_T("GIVE_USERID = '%s', ") 
				_T("GIVE_DATE = SYSDATE, ")
				_T("LOAN_KEY = NULL, ")
				_T("WORKNO = NULL, ")
				_T("LAST_WORK = '%s' ")
				_T("WHERE ")
				_T("TRANS_NO = '%s' AND STATE = '0011' AND GIVE_LIBCODE = '%s';"),
				m_pInfo->USER_ID,
				GetWorkLogMsg( _T("�߼۰������"), __WFILE__, __LINE__ ),
				strTransNo,
				m_strLibCode
				);

			TmpDM.AddFrame(strQuery);

			TmpDM.MakeDeleteFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey);

			int ids = TmpDM.SendFrame();
			TmpDM.EndFrame();

			
			CString strBookKey = _T("");
			CString strUserKey = _T("");
			
			CString strWorkingStatus = _T("BOL112N");
			
			m_pDM->GetCellData(_T("åKEY"), nIdx, strBookKey);
			m_pDM->GetCellData(_T("�̿���KEY"), nIdx, strUserKey);
			
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ")
				_T("WORKING_STATUS = PREV_WORKING_STATUS, ")
				_T("PREV_WORKING_STATUS = '%s' ")
				_T("WHERE ")
				_T("REC_KEY = %s AND MANAGE_CODE = '%s';"),
				strWorkingStatus,strBookKey,m_pInfo->MANAGE_CODE );
			
			TmpDM.StartFrame();
			TmpDM.AddFrame(strQuery);
			TmpDM.SendFrame();
			TmpDM.EndFrame();
			nIdx = m_pGrid->SelectGetNext();

			ShareInfoCenterUpload(strTransNo, _T("U"), _T("ILL_TRANS_TBL"), _T("STATE,GIVE_USERID,GIVE_DATE"));
			
			nIdx = m_pGrid->SelectGetNext();
		}while(0 <= nIdx);

	}	
	
	return RFID_SecurityChange(strNO, FALSE, TRUE);

	return 0;
}