#include "stdafx.h"
#include "BL_LOC_LOAN_PROC.h"
#include "LoanBookListDlg.h"
#include "BO_LOC_3170.h"
#include "BO_LOC_3170_ReserveInfo.h"
#include "InputReservatoinExpireDay.h"
#include "BO_LOC_3130.h"
#include "ReservationInfoDlg.h"
#include "TestMsgDlg.h"
#include "SearchBookListDlg.h"
#include "BL_LOC_RESERVATOIN_PROC.h"
#include "LoanAppendixListDlg.h"
#include "LoanHistory.h"
#include "BO_LOC_LOAN_DATE.h"
#include "FeeLoanStop.h"
#include "SBL.h"
#include "ReserveLinkBookInfoDlg.h"
#include "ReserveComplete.h"
#include "ILL_RETURN_SEARCH_BOOK.h"
#include "ILL_LOAN.h"
#include "ILL_RETURN.h"

#include "InsertPasswordDlg.h"
#include "..\\BL_LOC_USER\\LoanUserListDlg.h"
#include "..\\BL_LOC_USER\\FAMILY_ID.h"
#include "..\\BL_LOC_USER\\BO_LOC_3320.h"
#include "KL_SearchLoanInfoDlg.h"
#include "KL_UserSearchDlg.h"
#include "..\\BO_LOC_ARREAR\\ArrearInfoDlg.h"
#include "..\\BO_LOC_ARREAR\\ArrearViewDlg.h"
#include "LibListDlg.h"
#include "KL_UserImportDlg.h"
#include "KlOtherLoanListDlg.h"
#include "BO_LOC_3000Api.h"
#include "ILL_APL_3100.h"
#include "ILL_RESERVE_CONVERT_LIB_DLG.h"
#include "..\\..\\..\\..\\����\\K2UP_Common\\MakeSearchData.h"
#include "..\\..\\..\\..\\����\\�˻�\\BOBookDetailViewer\\BOBookDetailViewerDlg.h"
#include "..\\..\\..\\..\\����\\�˻�\\SEBookDetailViewer\\SEBookDetailViewerDlg.h"
#include "../../../����/COMMON_CONVERSION_MGR/MarcInfoConversionMgr.h"
#include "..\\RFID_JOB\\RFID_Interface.h"
#include "..\\RFID_JOB\\RFID_Change.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CBO_LOC_3320 *m_EditLoanUser;

CBL_LOC_LOAN_PROC::CBL_LOC_LOAN_PROC(CESL_Mgr *pWnd, CLoanShareManager* pLoanShareManager, CLonaManageValue *pManageValue, CPreRegCodeMgr* pPreRegNoMgr)
	: CSPF_Work(pWnd)
{
	m_bStopState = 3;
	m_bShowMessage = TRUE;
	m_pManageValue = pManageValue;
	m_pCheckLoan = NULL;
	m_pCheckLoan = new CCheckLoanBook(m_pParentMgr, m_pManageValue);
	m_pLocCommonAPI = NULL;
	m_pLocCommonAPI = new CLocCommonAPI(m_pParentMgr);
	m_sResultCMAlias = _T("CM_BO_LOC_3191_AUTO_JOB");
	m_sResultProgressAlias = _T("�����Ȳ");
	m_sResultDMAlias = _T("DM_BO_LOC_3191_AUTO_JOB");

	m_pMessageDlg[0] = NULL;
	m_pMessageDlg[1] = NULL;
	m_pMessageDlg[2] = NULL;

	m_pLoanShareManager = pLoanShareManager;

	m_pPreRegNoMgr = pPreRegNoMgr;

	m_lp3000ApiSocket = NULL;
	m_h3000ApiDLL = AfxLoadLibrary(_T("BO_LOC_3000Api.dll"));
	if(m_h3000ApiDLL != NULL)
	{
		m_lp3000ApiSocket = (LP3000APISOCKET)GetProcAddress(m_h3000ApiDLL, "BO_LOC_3000Api_Socket");
	}

	m_ReceiptDlg = NULL;

	m_strUserID=pWnd->GetUserID();
	m_strLibCode=_T("");

	m_nMessageLoop = 0;
	m_bReturnBookNonFree = FALSE;
	m_obj3170DlgList = new CObList();
	m_bLoanCallReserve = FALSE;
	m_bBeep = FALSE;
	m_bSimpleGrid = FALSE;
	m_bSimpleCmd  = FALSE;
	m_EditLoanUser = NULL;
	m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("���е��뿩��"), _T(""), m_strPinpadYN);
	m_strPinpadYN.TrimLeft();		m_strPinpadYN.TrimRight();
	m_strPinpadYN.MakeUpper();
	if(m_strPinpadYN.IsEmpty())
	{
		m_strPinpadYN = _T("N");
	}
 
	m_strReturnBookLibCode = _T("");
	
	m_pOtherReturnMgr = NULL;
	
	m_strMaxLoanCount = _T("");
	
	m_strLocalFreeYN = _T("");
	m_strOtherFreeYN = _T("");
	m_strUserNo = _T("");

	
	m_strSerchCivilNo = _T("");

	m_bManageCodeColorUseYN = FALSE;

	m_bReferenceColorUseYN = FALSE;

	m_nILLReturnProcCnt = 0;
	m_IsReqPrior = FALSE;
}

CBL_LOC_LOAN_PROC::~CBL_LOC_LOAN_PROC()
{
	
	INT nObjCount;
	nObjCount = m_obj3170DlgList->GetCount();
	if(!m_pManageValue->m_bVolReserve)
	{ 
		CBO_LOC_3170 *p3170Dlg;
		for(INT i = 0; i < nObjCount; i++)
		{
			p3170Dlg = NULL;
			p3170Dlg = (CBO_LOC_3170*)m_obj3170DlgList->RemoveHead();
			if(p3170Dlg != NULL) 
			{
				delete p3170Dlg;
			}
			p3170Dlg = NULL;
		}
	}
	else
	{ 
		CBO_LOC_3170_ReserveInfo *p3170Dlg;
		for(INT i = 0; i < nObjCount; i++)
		{
			p3170Dlg = NULL;
			p3170Dlg = (CBO_LOC_3170_ReserveInfo*)m_obj3170DlgList->RemoveHead();
			if(p3170Dlg != NULL) 
			{
				delete p3170Dlg;
			}
			p3170Dlg = NULL;
		}
	}
	m_obj3170DlgList->RemoveAll();
	delete m_obj3170DlgList;
	m_obj3170DlgList = NULL;

	
	if(m_ReceiptDlg != NULL)
	{
		delete m_ReceiptDlg;
	}
	m_ReceiptDlg = NULL;

	if(m_pCheckLoan != NULL) 
	{
		delete m_pCheckLoan;
	}
	m_pCheckLoan = NULL;

	if(m_pLocCommonAPI != NULL) 
	{
		delete m_pLocCommonAPI;
	}
	m_pLocCommonAPI = NULL;

	if(m_EditLoanUser != NULL)
	{
		delete m_EditLoanUser;
	}
	m_EditLoanUser = NULL;

	for(INT i = 0; i < 3; i++) 
	{
		if(m_pMessageDlg[i] != NULL)
		{
			if(m_pMessageDlg[i]->GetSafeHwnd() != NULL)
			{
				m_pMessageDlg[i]->DestroyWindow();
			}
			if(m_pMessageDlg[i] != NULL)
			{
				delete m_pMessageDlg[i];
			}
		}
		m_pMessageDlg[i] = NULL;
	}

	if(m_h3000ApiDLL != NULL)
	{
		AfxFreeLibrary(m_h3000ApiDLL);
		m_h3000ApiDLL = NULL;
	}
}

INT CBL_LOC_LOAN_PROC::SPFExcutePath(CString sPathAlias)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SPFExcutePath");

	DWORD nSTART, nFINISH;
	nSTART = GetTickCount();
	
	if(NULL == this)
	{
		return -1000;
	}

	
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");
	CString sGridAlias2 = _T("MainGrid2");	

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_Grid *pGrid2 = (CESL_Grid*)(pCM->FindControl(sGridAlias2));
	if(pGrid2 == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	if(sPathAlias.Compare(_T("ȸ��_�˻�")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = SearchLoanInfo();
	}
	else if(sPathAlias.Compare(_T("ȸ��_�˻�_NOASK")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = SearchLoanInfo(FALSE);
	}
	else if(sPathAlias.Compare(_T("ȸ�����ε�")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReLoadLoanInfo();
	}
	else if(sPathAlias.Compare(_T("ȸ�����ε�_NOASK")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReLoadLoanInfo(FALSE);
	}
	else if(sPathAlias.Compare(_T("ȸ�����������ε�")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReLoadMemberInfo();
	}
	else if(sPathAlias.Compare(_T("�����ڰ���_ȸ������")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ChangeMember();
	}
	else if(sPathAlias.Compare(_T("����")) == 0)
	{
		m_bSimpleCmd = FALSE;
		if(m_bSimpleGrid == TRUE) 
		{	
			m_pParentMgr->LockWindowUpdate();		
			pGrid->LockWindowUpdate();
			pGrid2->LockWindowUpdate();
		}
		ids = Loan(0);
		if(m_bSimpleGrid == TRUE) 
		{
			m_pParentMgr->UnlockWindowUpdate();			
			pGrid->UnlockWindowUpdate();
			pGrid2->UnlockWindowUpdate();
		}
	}
	
	else if(sPathAlias.Compare(_T("Ÿ������")) == 0)
	{
		m_bSimpleCmd = FALSE;
		if(m_bSimpleGrid == TRUE) 
		{	
			m_pParentMgr->LockWindowUpdate();		
			pGrid->LockWindowUpdate();
			pGrid2->LockWindowUpdate();
		}
		ids = OtherLoan(0);
		if(m_bSimpleGrid == TRUE) 
		{
			m_pParentMgr->UnlockWindowUpdate();			
			pGrid->UnlockWindowUpdate();
			pGrid2->UnlockWindowUpdate();
		}
	}
	else if(sPathAlias.Compare(_T("��޴���")) == 0)
	{
		m_bSimpleCmd = FALSE;
		if(m_bSimpleGrid == TRUE) 
		{	
			m_pParentMgr->LockWindowUpdate();		
			pGrid->LockWindowUpdate();
			pGrid2->LockWindowUpdate();
		}
		ids = NoDBLoan();
		if(m_bSimpleGrid == TRUE) 
		{
			m_pParentMgr->UnlockWindowUpdate();			
			pGrid->UnlockWindowUpdate();
			pGrid2->UnlockWindowUpdate();			
		}
	}
	else if(sPathAlias.Compare(_T("�ݳ�")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReturnBook();
	}

	else if(sPathAlias.Compare(_T("å�η�����")) == 0)
	{			
		ids = Loan(2);
	}
	else if(sPathAlias.Compare(_T("å�η��������")) == 0)
	{
		ids = ILL_ReservationLoan();
	}
	else if(sPathAlias.Compare(_T("å�η��ݳ�")) == 0)
	{		
		m_bSimpleCmd = FALSE;
		ids = ReturnBook(TRUE, FALSE, TRUE);
	}	

	else if(sPathAlias.Compare(_T("����")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = Reservation();		
		m_IsReqPrior = FALSE;
	}
	else if(sPathAlias.Compare(_T("�����ڷ�����")) == 0)
	{		
		m_bSimpleCmd = FALSE;
		ids = ReserveLinkBookInfo();
	}
	else if(sPathAlias.Compare(_T("�������")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReservationLoan();
	}
	else if(sPathAlias.Compare(_T("�������")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReservationCancel();
	}
	else if(sPathAlias.Compare(_T("�������")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReservationStatus();
	}
	else if(sPathAlias.Compare(_T("���")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ExcuteUNDO();
	}
	else if(sPathAlias.Compare(_T("�ݳ�����")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = DelayLoanDate();
	}
	else if(sPathAlias.Compare(_T("ȭ���ʱ�ȭ")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = InitDM();
		Send3000ApiSocket(_T("�ʱ�ȭ"));
	}
	else if(sPathAlias.Compare(_T("�̿����ڷ�ã��")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = SearchBookInfo();
	}
	else if(sPathAlias.Compare(_T("����_����ȸ��")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ViewFamilyMember();
	}
	else if(sPathAlias.Compare(_T("�̿���_����������")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = EditLoanUserInfo();
	}
	else if(sPathAlias.Compare(_T("�̿���_������")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = SaveLoanUserEtcInfo();
	}
	else if(sPathAlias.Compare(_T("�ݳ����ڷΌ�ุ���Ϻο�")) == 0)
	{
		m_bSimpleCmd = FALSE;		
		 
		ids = RecordReserveExpireDate();
	}
	else if(sPathAlias.Compare(_T("���ڵ����")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = InputBarCode();
	}
	else if(sPathAlias.Compare(_T("ȭ��ε�_�ڵ���ɼ���")) == 0) 
	{
		m_bSimpleCmd = FALSE;
		ids = RunAutoProc();
	}
	else if(sPathAlias.Compare(_T("��������")) == 0) 
	{
		m_bSimpleCmd = FALSE;
		ids =  ViewDetail();
	}
	else if(sPathAlias.Compare(_T("���������")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = Receipt_Print(0);
	}
	else if(sPathAlias.Compare(_T("��������߱�")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = Receipt_Print(1);
	}
	else if(sPathAlias.Compare(_T("���������üũ")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = Receipt_PrintCheck();		
	}	
	else if(sPathAlias.Compare(_T("�̿��ڰ˻��ٱ����ʱ�ȭ")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = ClearSearchUserBasket();		
	}	
	else if(sPathAlias.Compare(_T("�˻��̿��ڹٱ����߰�")) == 0)
	{
		m_bSimpleCmd = FALSE;		
		ids = AddSearchUserBasket();
	}
	else if(sPathAlias.Compare(_T("��������������")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = LoanStopDateCancel();		
	}
	
	else if(sPathAlias.Compare(_T("å�η�����ȭ��")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = 0;
		
		CILL_LOAN Dlg(m_pParentMgr);
		Dlg.m_pInfo = m_pParentMgr->m_pInfo;
		Dlg.m_pLoan = this;
		Dlg.m_pManageValue = m_pManageValue;
		Dlg.m_pPreRegNoMgr = m_pPreRegNoMgr;

		Dlg.DoModal(); 
	}
	else if(sPathAlias.Compare(_T("å�η��ݳ�ȭ��")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = 0;
	
		CILL_RETURN Dlg(m_pParentMgr);
		Dlg.m_pInfo = m_pParentMgr->m_pInfo;
		Dlg.m_pLoan = this;
		Dlg.m_pPreRegNoMgr = m_pPreRegNoMgr;
		Dlg.DoModal();
	}
	
	
	else if(sPathAlias.Compare(_T("å�η�_��ȯ")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ILL_ReservationConvertToAPL();
	}
	else if(sPathAlias.Compare(_T("�ηϴ���")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = AppendixLoanProc();		
	}
	else if(sPathAlias.Compare(_T("���ռ��ʹ����̷���ȸ")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = KL_SearchLoanHistoryInfo();
	}
	else if(sPathAlias.Compare(_T("Ÿ���ݳ�����")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = KL_OtherReturnMgr();
	}
	
	else if(sPathAlias.Compare(_T("RFID_����")) == 0) 
	{	
		m_bSimpleCmd = TRUE;		
		if(m_bSimpleGrid == TRUE) 
		{	
			m_pParentMgr->LockWindowUpdate();		
			pGrid->LockWindowUpdate();
			pGrid2->LockWindowUpdate();
		}
		ids = RFID_Loan();	
		if(m_bSimpleGrid == TRUE) 
		{
			m_pParentMgr->UnlockWindowUpdate();			
			pGrid->UnlockWindowUpdate();
			pGrid2->UnlockWindowUpdate();
			InitGridColor();
			ViewGrid();
		}
	}
	else if(sPathAlias.Compare(_T("RFID_�ݳ�")) == 0) 
	{
		m_bSimpleCmd = TRUE;		
		if(m_bSimpleGrid == TRUE) 
		{	
			m_pParentMgr->LockWindowUpdate();		
			pGrid->LockWindowUpdate();
			pGrid2->LockWindowUpdate();
		}
		ids = RFID_Return();
		if(m_bSimpleGrid == TRUE) 
		{
			m_pParentMgr->UnlockWindowUpdate();			
			pGrid->UnlockWindowUpdate();
			pGrid2->UnlockWindowUpdate();
			InitGridColor();
			ViewGrid();
		}
	}
	else if(sPathAlias.Compare(_T("RFID_ȸ�������")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = RecordRFIDUserInfo();
	}

	if(0 > ids)
	{
		ViewMessage(_T("����� ���������� �������� ���Ͽ����ϴ�."));
		return -1;
	}	

	nFINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - [%s] �ɸ� �ð� :  [%d.%d sec]"), sPathAlias, (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	TRACE(strMsg.GetBuffer(0));
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	pDM->QueryLog(strMsg.GetBuffer(0));	
	
	return ids;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchLoanInfo(BOOL bAskPasswd/*=TRUE*/, CString strUserKey/*NULL*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SearchLoanInfo");
	
	StartLog(_T("ȸ���˻�:��������"));

	m_strUserNo = _T("");
	
	CString sWhere;
	
	MakeSearchMemberSql(sWhere, strUserKey);
	
	if(sWhere.IsEmpty()) 
	{
		EndLog();
		return 1;
	}
	EndLog();
	
	StartLog(_T("ȸ���˻�:�̿����������"));
	ids = SearchLoanInfo(_T(""), sWhere, bAskPasswd);
	if(0 > ids) return ids;
	EndLog();
	if(ids == 1)
	{
		SelfCloseMessageBox(_T("�������� �ʴ� �̿����Դϴ�."));
		
		SPFExcutePath(_T("ȭ���ʱ�ȭ"));
		return 1;
	}
	
	if(ids == 2)
	{
		
		SPFExcutePath(_T("ȭ���ʱ�ȭ"));
		return 1;
	}

	CString sMemberState;
	CString sLostUserCard;
	CString sArrear;	
	
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM(sMemberDMAlias);
	ids = pTmpDM->GetCellData(_T("ȸ������"), 0, sMemberState);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if(NULL != m_lp3000ApiSocket)
	{
		CString strUserKey = _T("");
		pTmpDM->GetCellData(_T("������KEY"), 0, strUserKey);
		
		Send3000ApiSocket(_T("ȸ��_�˻�"), strUserKey);
	}
	
	ids = pTmpDM->GetCellData(_T("ȸ�����н�"), 0, sLostUserCard);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);	

	
	CString sLoanUserKey;
	pTmpDM->GetCellData(_T("������KEY"), 0, sLoanUserKey);
	if(_T("Y") == m_pManageValue->m_ArrearCheckYN)
	{
		pTmpDM->GetCellData(_T("�ѿ�ü��"), 0, sArrear);
		INT nArrear = _ttoi(sArrear);

		if(0 < nArrear && !sLoanUserKey.IsEmpty())
		{
			CArrearInfoDlg Arreardlg(m_pParentMgr);
			Arreardlg.m_strIsIncludeLoanCnt = m_pManageValue->m_sIsIncludeLoanCnt;
			Arreardlg.m_strUserKey = sLoanUserKey;
			Arreardlg.m_strLoanStopDate = pTmpDM->GetCellData(_T("����������"), 0);
			Arreardlg.DoModal();
		}
	}

	if(sMemberState == _T("1"))
	{
		if(sLostUserCard == _T("Y"))
		{
			SelfCloseMessageBox(_T("�нǵ� ȸ��ī�� �Դϴ�."), m_pParentMgr);
		}
		else
		{
			SelfCloseMessageBox(_T("�˻��� �̿��ڴ� ��������ȸ���Դϴ�."), m_pParentMgr);
		}
	}
	else if(sMemberState == _T("2"))
	{
		SelfCloseMessageBox(_T("�˻��� �̿��ڴ� ���������Դϴ�."), m_pParentMgr);
	}
	
	else if(sMemberState == _T("3"))
	{
		SelfCloseMessageBox(_T("�˻��� �̿��ڴ� Ż���ûȸ���Դϴ�."), m_pParentMgr);
	}
	else 
	{
		if(sLostUserCard == _T("Y"))
		{
			SelfCloseMessageBox(_T("�нǵ� ȸ��ī�� �Դϴ�."), m_pParentMgr);
		}
		else
		{
			SelfCloseMessageBox(_T("�̿��ڰ� �˻��Ǿ����ϴ�."), m_pParentMgr, 2000);	
		}
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::SearchLoanInfo(CString sLoanUserKey, CString sSearchWhere, BOOL bAskPasswd/*=TRUE*/)
{
EFS_BEGIN

	INT ids;	
	CString strFuncName = _T("SearchLoanInfo");

	this->SetBEGIN_SBL();

	CString sWhere, sLoanStopDate, strQuery, strCardPasswd = _T("");
	CESL_DataMgr* pMemberDM;
	
	sWhere.Format(_T("CLU.REC_KEY = %s "), sLoanUserKey);

	if(sSearchWhere.IsEmpty() == FALSE)
	{
		sWhere = sSearchWhere;
	}

	BOOL bSearchUser = TRUE;
	if(FALSE == sLoanUserKey.IsEmpty() && sSearchWhere.IsEmpty() && 2 == bAskPasswd)
	{
		CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(1 == pTmpDM->GetRowCount())
		{
			CString strUserKey;
			pTmpDM->GetCellData(_T("������KEY"), 0, strUserKey);
			if(sLoanUserKey == strUserKey)
			{
				bSearchUser = FALSE;
			}
		}
	}

	if(bSearchUser)
	{
		ids = SearchMemberInfo(sWhere);
		if(0 > ids)	return ids;
		
		if(ids > 1)		
		{
 			CLoanUserListDlg user(m_pParentMgr, sWhere);
			user.m_sIsUnityLoanService = m_pManageValue->m_sIsUnityLoanService;
			user.m_strUserAccessAuth = m_pManageValue->m_sLoanUserAccessMode;
 			user.DoModal();
						
			CString sUserkey;
			ids = user.GetSelectUserKey(sUserkey);
			
			
			if(ids == 1)	return 2;
		}	
		else if(ids < 1)	
		{
 			return 1;
		}
	}

	if(bAskPasswd == TRUE)
	{
		if(m_strPinpadYN == _T("Y"))
		{
			CString strMainMemberClass;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("ȸ������"), strMainMemberClass, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			if(_T("2") != strMainMemberClass)
			{				
				CCardpasswdDlg dlg(m_pParentMgr);

				pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
				strCardPasswd = pMemberDM->GetCellData(_T("ȸ������й�ȣ"), 0);

				if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && !strCardPasswd.IsEmpty())
				{
					CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
					if(NULL != pApi)
					{
						if(TRUE == pApi->ConnectSocket()) 
						{
							CStringArray DataArray;
							DataArray.Add(strCardPasswd);
							if(TRUE == pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD"), DataArray))
							{
								strCardPasswd = DataArray.GetAt(0);
							}
							DataArray.RemoveAll();
							DataArray.FreeExtra();
							pApi->CloseSocket();	
						}
						delete pApi;
						pApi = NULL;
					}
				}

				INT		nLength		=	strCardPasswd.GetLength();

				if(strCardPasswd.IsEmpty() == TRUE)
				{
					AfxMessageBox(_T("��й�ȣ�� �Էµ��� �ʾҽ��ϴ�.\n\n���ο� ��й�ȣ�� ��� �ϼ���"));
				}				
				
				else if((4 > nLength || 20 < nLength ) && FALSE == m_pParentMgr->m_pInfo->m_bCardPwEncryption)
				{
					AfxMessageBox(_T("�߸��� ��й�ȣ�� �ԷµǾ����ϴ�.\n\n���ο� ��й�ȣ�� ��� �ϼ���"));
				}
				else
				{
					dlg.m_strCardpasswd = strCardPasswd;
				}

				if(IDOK != dlg.DoModal())	
				{
					return 2;
				}
				else
				{
					switch(dlg.m_nStatus)
					{
						case 0:
							break;
						case 1:
							{
								
								if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
								{
									CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
									if(NULL != pApi)
									{
										if(TRUE == pApi->ConnectSocket()) 
										{
											CStringArray DataArray;
											DataArray.Add(dlg.m_strCardpasswd);
											if(TRUE == pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD"), DataArray))
											{
												dlg.m_strCardpasswd = DataArray.GetAt(0);
											}
											DataArray.RemoveAll();
											DataArray.FreeExtra();
											pApi->CloseSocket();
										}
										delete pApi;
										pApi = NULL;
									}
								}
														
								CString strLastWork = m_pParentMgr->GetWorkLogMsg(_T("ȸ������й�ȣ����"), __WFILE__, __LINE__);
								strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET CARD_PASSWORD='%s',LAST_WORK='%s' WHERE REC_KEY=%s;"), dlg.m_strCardpasswd, strLastWork, pMemberDM->GetCellData(_T("������KEY"), 0));
								pMemberDM->StartFrame();
								pMemberDM->AddFrame(strQuery);

								ids = pMemberDM->SendFrame();
								if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
								pMemberDM->EndFrame();
							}
							break;
					}
				}
			}	
		}
	}
	
	ids = SearchLoanBookInfo();
	if(0 > ids) return ids;

	
	ids = AddMemberInfo();
	if(0 > ids) return ids;

	
	ids = ViewLoanScreen();
	if(0 > ids) return ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	INT nRowCount = pDM->GetRowCount();
	this->SetEND_SBL(nRowCount);

	
	ids = BasketRegNoProc();
	if(0 > ids) return ids;

	
	if(m_ReceiptDlg != NULL) m_ReceiptDlg->initData();

	
	ids = AddSearchUserBasket();
	if(0 > ids) return ids;

	ids = ViewSimpleFamilyMember();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchMemberInfo(CString &sWhere)
{
EFS_BEGIN
	
	INT i, ids;
	CString strFuncName = _T("SearchMemberInfo");

	DWORD nSTART,nFINISH;
	CString sLoanStopDate, strMsg, sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr *pMemberDM, *pDM;

	pMemberDM = m_pParentMgr->FindDM(sMemberDMAlias);
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	if(m_pManageValue->m_sIs64lib == _T("Y"))
	{
	
		CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
		for(i=0; i<pMemberDM->GetColCount(); i++)
		{
			pMemberDM->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(FIELD_ALIAS == _T("�а�"))
			{
				pMemberDM->SetDataMgrRef(i, _T("�а�"), _T("COURSE"), _T("str"), _T(""), _T(""));
			}
			else if(FIELD_ALIAS == _T("�к�"))
			{
				pMemberDM->SetDataMgrRef(i, _T("�к�"), _T("CLASS_GROUP"), _T("str"), _T(""), _T(""));
			}
		}
	}

	nSTART = GetTickCount();

	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pMemberDM, sWhere);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����������"), sLoanStopDate, 0);	

	nFINISH = GetTickCount();
	
	strMsg.Format(_T("(Finish) - [%s] �ɸ� �ð� :  [%d.%d sec]"), _T("SearchMemberInfo"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	pDM->QueryLog(strMsg.GetBuffer(0));

	
	CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7;

	
	for(i = 0; i < pMemberDM->GetRowCount(); i++)
	{
		CString sCode, sDesc;
		pMemberDM->GetCellData(_T("�����ڼҼ�"), i, sCode);
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����Ҽ�����"), sCode, sDesc);
		pMemberDM->SetCellData(_T("�����ڼҼӼ���"), sDesc, i);

		pMemberDM->GetCellData(_T("����������"), i, sCode);
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("������������"), sCode, sDesc);
		pMemberDM->SetCellData(_T("���������޼���"), sDesc, i);

		pMemberDM->GetCellData(_T("��������"), i, sCode);
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������"), sCode, sDesc);
		pMemberDM->SetCellData(_T("�������м���"), sDesc, i);

	
		sDesc = _T("");
		pMemberDM->GetCellData(_T("ȸ������"), i, sCode);	
		if(sCode.Compare(_T("0")) == 0)
			sDesc = _T("����ȸ��");
		else if(sCode.Compare(_T("1")) == 0)
			sDesc = _T("��������");
		else if(sCode.Compare(_T("2")) == 0)
			sDesc = _T("����ȸ��");
		else if(sCode.Compare(_T("3")) == 0)
			sDesc = _T("Ż��ȸ��");
		pMemberDM->SetCellData(_T("ȸ�����¼���"), sDesc, i);

		
		sDesc = _T("");
		pMemberDM->GetCellData( _T("ȸ������"), i, sCode);
		if( sCode.Compare(_T("0"))==0 )
			sDesc = _T("��ȸ��");
		else if( sCode.Compare(_T("1"))==0 )
			sDesc = _T("��ȸ��");
		else if( sCode.Compare(_T("2"))==0 )
			sDesc = _T("��ȸ��");
		pMemberDM->SetCellData( _T("ȸ�����м���"),  sDesc, i);

		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			
			CString strGetData;
			pMemberDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strGetData);
			asDes1.Add(strGetData);
			pMemberDM->GetCellData(_T("�̸���"), i, strGetData);
			asDes2.Add(strGetData);
			pMemberDM->GetCellData(_T("�޴���"), i, strGetData);
			asDes3.Add(strGetData);
			pMemberDM->GetCellData(_T("�������ּ�"), i, strGetData);
			asDes4.Add(strGetData);
			pMemberDM->GetCellData(_T("������ȭ"), i, strGetData);
			asDes5.Add(strGetData);
			pMemberDM->GetCellData(_T("�ٹ�����ȭ"), i, strGetData);
			asDes6.Add(strGetData);
			pMemberDM->GetCellData(_T("ȸ������й�ȣ"), i, strGetData);
			asDes7.Add(strGetData);
		}
	}
	
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && 0 < pMemberDM->GetRowCount())
	{
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes3);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes4);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes5);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes6);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD"), asDes7);
					pApi->CloseSocket();
				}
				for(i = 0; i < pMemberDM->GetRowCount(); i++)
				{
					pMemberDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), asDes1.GetAt(i), i);
					pMemberDM->SetCellData(_T("�̸���"), asDes2.GetAt(i), i);
					pMemberDM->SetCellData(_T("�޴���"), asDes3.GetAt(i), i);
					pMemberDM->SetCellData(_T("�������ּ�"), asDes4.GetAt(i), i);
					pMemberDM->SetCellData(_T("������ȭ"), asDes5.GetAt(i), i);
					pMemberDM->SetCellData(_T("�ٹ�����ȭ"), asDes6.GetAt(i), i);
					pMemberDM->SetCellData(_T("ȸ������й�ȣ"), asDes7.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
		asDes6.RemoveAll(); asDes6.FreeExtra();
		asDes7.RemoveAll(); asDes7.FreeExtra();
	}
	
	return pMemberDM->GetRowCount();

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchLoanBookInfo()
{
EFS_BEGIN

	INT ids;  
	CString strFuncName = _T("SearchLoanBookInfo");

	CString sLoanUserRecKey;
	CString sWhere;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("������KEY"), sLoanUserRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	sWhere.Format(_T("CL.USER_KEY=%s"), sLoanUserRecKey);   
	
	CString sUnionQuery; 
	CString sBOVol,sSEVol;
	CString sBookCode		= _T("");
	CString sClassNo		= _T("");
	CString sCopyCode		= _T("");
	CString sManageCode		= _T("");
	CString sPrevWorkStat	= _T("");
	CString sSeShelfCode	= _T("");
	CString sVolCode		= _T("");
	CString sWorkStat		= _T("");
	CString sBOTitle,sSETitle;
	CString sLoanTypeCode;
	CString sAppendixFlag;
	CString sBOReserveUserCnt,sSEReserveUserCnt,sILLReserveUserCnt;
	if(m_pManageValue->m_bVolReserve)
	{ 
		sBookCode		= _T("DECODE(CL.REG_NO,NULL,NULL,BB.BOOK_CODE)");
		sClassNo		= _T("DECODE(CL.REG_NO,NULL,NULL,BB.CLASS_NO)");
		sCopyCode		= _T("DECODE(CL.REG_NO,NULL,NULL,BB.COPY_CODE)");
		sPrevWorkStat	= _T("DECODE(CL.REG_NO,NULL,NULL,BB.PREV_WORKING_STATUS)");
		sSeShelfCode	= _T("DECODE(CL.REG_NO,NULL,NULL,BB.SEPARATE_SHELF_CODE)");
		sVolCode		= _T("DECODE(CL.REG_NO,NULL,NULL,BB.VOL_CODE)");
		sWorkStat		= _T("DECODE(CL.REG_NO,NULL,NULL,BB.WORKING_STATUS)");
		sBOReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 ")
									_T("WHERE CL2.SPECIES_KEY=BB.SPECIES_KEY AND ((CL2.VOL_INFO=BB.VOL_SORT_NO) OR (CL2.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL)) AND CL2.STATUS='3')"));
		sSEReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 ")
									_T("WHERE CL2.SPECIES_KEY=BB.SPECIES_KEY AND CL2.VOL_INFO=BB.VOL_KEY AND CL2.STATUS='3')"));
		sILLReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 ")
									_T("WHERE CL2.SPECIES_KEY=CL.SPECIES_KEY AND ((CL2.VOL_INFO=CL.VOL_INFO) OR (CL2.VOL_INFO IS NULL AND CL.VOL_INFO IS NULL)) AND CL2.STATUS='3')"));
	}
	else
	{ 
		sBookCode		= _T("BB.BOOK_CODE");
		sClassNo		= _T("BB.CLASS_NO");
		sCopyCode		= _T("BB.COPY_CODE");
		sPrevWorkStat	= _T("BB.PREV_WORKING_STATUS");
		sSeShelfCode	= _T("BB.SEPARATE_SHELF_CODE");
		sVolCode		= _T("BB.VOL_CODE");
		sWorkStat		= _T("BB.WORKING_STATUS");
		sBOReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2")
									_T(" WHERE CL2.BOOK_KEY=BB.REC_KEY AND CL2.STATUS='3')"));
		sSEReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2")
									_T(" WHERE CL2.BOOK_KEY=BB.REC_KEY AND CL2.STATUS='3')"));
		sILLReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2")
									_T(" WHERE CL2.BOOK_KEY=CL.BOOK_KEY AND CL2.STATUS='3')"));
	}
	sManageCode		= _T("BB.MANAGE_CODE");
	sBOVol			= _T("DECODE(BB.BOOK_APPENDIX_FLAG,'A',NULL,'B',CONCAT(CONCAT(BB.VOL, '/'),BB.VOL_TITLE))");
	sBOTitle		= _T("DECODE(BB.BOOK_APPENDIX_FLAG,'A',BB.VOL_TITLE,'B',IB.TITLE_INFO)");
	sLoanTypeCode	= _T("DECODE(CL.LOAN_TYPE_CODE,'0','�Ϲ�','1','Ư��','2','����','3','����','4','���')");
	sSEVol			= _T("DECODE(BB.BOOK_TYPE,'A',V.VOL_TITLE,'B',V.VOL_TITLE,'C',V.VOL_TITLE)");
	sSETitle		= _T("DECODE(BB.BOOK_TYPE,'A',IB.TITLE_INFO,'B',IB.TITLE_INFO,'C',IB.TITLE_INFO)");
	sAppendixFlag	= _T("DECODE(CL.LOAN_KEY,NULL,'B','A')");
	
	CString strOderBy, strValue;
	ids = m_pManageValue->GetManageValue(_T("��Ÿ"), _T("����"), _T("���ı���"), _T("��Ȳ���ı���"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	strValue.TrimLeft(); strValue.TrimRight();
	if(strValue.IsEmpty())
	{
		strValue = _T("0");
	}
	if(strValue == _T("1"))
	{
		strOderBy = _T("ORDER BY SORT_MANAGE_CODE, STATUS, RETURN_PLAN_DATE, PK");
	}
	else
	{
		strOderBy = _T("ORDER BY STATUS, RETURN_PLAN_DATE, PK");
	}

	
	sUnionQuery.Format(	
	
	_T("SELECT BB.BOOK_APPENDIX_FLAG,%s,%s,%s,%s,%s,BB.PRICE,BB.REC_KEY,CL.REG_NO,%s,%s,%s,")
		_T("CL.PUBLISH_FORM_CODE,CL.REC_KEY AS PK,TO_CHAR(CL.RESERVATION_EXPIRE_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') AS RETURN_PLAN_DATE,CL.SHELF_LOC_CODE,CL.STATUS,")
		_T("%s,%s,IB.REC_KEY,TO_CHAR(CL.LOAN_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RESERVATION_DATE,'YYYY/MM/DD HH24:MI'),CL.LOAN_MANAGE_CODE,%s,BB.WORKNO,")
		_T("CL.DELAY_CNT,%s,BB.MEDIA_CODE,BB.VOL_SORT_NO,BB.REG_CODE,CL.UNMANNED_RESERVATION_LOAN,") 
		_T("CL.L_DEVICE,CL.R_DEVICE,CL.L_WORKER,CL.R_WORKER,NULL,")
		_T("(SELECT DESCRIPTION FROM CD_CODE_TBL WHERE CLASS = 19 AND CODE = CL.SHELF_LOC_CODE AND ROWNUM=1), TO_CHAR(CL.CREATE_DATE, 'YYYY/MM/DD HH24:MI:SS'),")
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")		
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = BB.MANAGE_CODE AND ROWNUM = 1), ")
		_T("(SELECT DECODE(COUNT(1),0,'N','Y') FROM ILL_TRANS_TBL WHERE LOAN_KEY = CL.REC_KEY AND ROWNUM = 1), ")
		_T("(SELECT MAX(L.LIB_NAME) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")
		_T("CL.LILL_LOAN_YN,CL.LCT_KEY,CL.APPENDIX_SEND_YN,CL.APPENDIX_REGNO_LIST, H_LIB_CODE, ")
		_T("REPLACE(BB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE ")			
	_T("FROM LS_WORK_T01 CL, BO_BOOK_TBL BB, IDX_BO_TBL IB ")
	_T("WHERE CL.BOOK_KEY=BB.REC_KEY ")
		_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
		_T("AND CL.STATUS IN('0','2','3','L','D') ")
		_T("AND CL.PUBLISH_FORM_CODE='MO' ")
		_T("AND %s")
	_T(" UNION ")	
	_T("SELECT BB.BOOK_TYPE,%s,%s,%s,%s,%s,V.PRICE,BB.REC_KEY,CL.REG_NO,%s,%s,%s,")
		_T("CL.PUBLISH_FORM_CODE,CL.REC_KEY AS PK,TO_CHAR(CL.RESERVATION_EXPIRE_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') AS RETURN_PLAN_DATE,CL.SHELF_LOC_CODE,CL.STATUS,")
		_T("%s,%s,IB.REC_KEY,TO_CHAR(CL.LOAN_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RESERVATION_DATE,'YYYY/MM/DD HH24:MI'),CL.LOAN_MANAGE_CODE,%s,BB.WORKNO,")
		_T("CL.DELAY_CNT,%s,V.MEDIA_CODE,BB.VOL_KEY,BB.REG_CODE,CL.UNMANNED_RESERVATION_LOAN,")
		_T("CL.L_DEVICE,CL.R_DEVICE,CL.L_WORKER,CL.R_WORKER,NULL,")
		_T("(SELECT DESCRIPTION FROM CD_CODE_TBL WHERE CLASS = 19 AND CODE = CL.SHELF_LOC_CODE AND ROWNUM=1),TO_CHAR(CL.CREATE_DATE, 'YYYY/MM/DD HH24:MI:SS'),")
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = BB.MANAGE_CODE AND ROWNUM = 1), ")
		_T("(SELECT DECODE(COUNT(1),0,'N','Y') FROM ILL_TRANS_TBL WHERE LOAN_KEY = CL.REC_KEY AND ROWNUM = 1), ")
		_T("(SELECT MAX(L.LIB_NAME) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")
		_T("CL.LILL_LOAN_YN,CL.LCT_KEY,CL.APPENDIX_SEND_YN,CL.APPENDIX_REGNO_LIST, H_LIB_CODE, ")
		_T("REPLACE(BB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE ")				
	_T("FROM LS_WORK_T01 CL, SE_BOOK_TBL BB, IDX_SE_TBL IB, SE_VOL_TBL V ")
	_T("WHERE CL.BOOK_KEY=BB.REC_KEY ")
		_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
		_T("AND BB.VOL_KEY=V.REC_KEY ")
		_T("AND CL.STATUS IN('0','2','3','L','D') ")
		_T("AND CL.PUBLISH_FORM_CODE='SE' ")
		_T("AND %s")
	_T(" UNION ")
	_T("SELECT 'B',NULL,NULL,NULL,NB.MANAGE_CODE,NB.PREV_WORKING_STATUS,NB.PRICE,NB.REC_KEY,CL.REG_NO,")
		_T("NB.SEPARATE_SHELF_CODE,NULL,NB.WORKING_STATUS,CL.PUBLISH_FORM_CODE,CL.REC_KEY AS PK,")
		_T("TO_CHAR(CL.RESERVATION_EXPIRE_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') AS RETURN_PLAN_DATE,CL.SHELF_LOC_CODE,CL.STATUS,")
		_T("NULL,NB.TITLE,1,TO_CHAR(CL.LOAN_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RESERVATION_DATE,'YYYY/MM/DD HH24:MI'),CL.LOAN_MANAGE_CODE,0,'',CL.DELAY_CNT,")
		_T("%s,NULL,NULL,NULL,CL.UNMANNED_RESERVATION_LOAN,CL.L_DEVICE,CL.R_DEVICE,CL.L_WORKER,")
		_T("CL.R_WORKER,CL.CALL_NO,")
		_T("(SELECT DESCRIPTION FROM CD_CODE_TBL WHERE CLASS = 19 AND CODE = CL.SHELF_LOC_CODE AND ROWNUM=1),TO_CHAR(CL.CREATE_DATE, 'YYYY/MM/DD HH24:MI:SS'),")
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")		
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = NB.MANAGE_CODE AND ROWNUM = 1), ")
		_T("(SELECT DECODE(COUNT(1),0,'N','Y') FROM ILL_TRANS_TBL WHERE LOAN_KEY = CL.REC_KEY AND ROWNUM = 1), ")
		_T("(SELECT MAX(L.LIB_NAME) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")
		_T("CL.LILL_LOAN_YN,CL.LCT_KEY,CL.APPENDIX_SEND_YN,CL.APPENDIX_REGNO_LIST, '', ")
		_T("REPLACE(NB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE ")			
	_T("FROM LS_WORK_T01 CL, CO_NON_DB_BOOK_TBL NB ")
	_T("WHERE CL.BOOK_KEY=NB.REC_KEY AND CL.STATUS IN('0','2','3','L','D') ")
		_T("AND CL.PUBLISH_FORM_CODE='NB' ")
		_T("AND %s ")
	_T("%s"),	
	sBookCode, sClassNo, sCopyCode, sManageCode, sPrevWorkStat,	sSeShelfCode, sVolCode, 
	sWorkStat, sBOVol, sBOTitle, sBOReserveUserCnt, sLoanTypeCode, sWhere,		
	sBookCode, sClassNo, sCopyCode, sManageCode, sPrevWorkStat, sSeShelfCode, sVolCode, 
	sWorkStat, sSEVol, sSETitle, sSEReserveUserCnt, sLoanTypeCode, sWhere, 	
	sLoanTypeCode, sWhere, 
	strOderBy);	
	
	DWORD nSTART,nFINISH;
	nSTART = GetTickCount();

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pParentMgr->m_pInfo->MODE));
	ids = TmpDM.RestructDataManager(sUnionQuery);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	CString sLoanBookDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pLoanBookDM = m_pParentMgr->FindDM(sLoanBookDMAlias);
	if(NULL == pLoanBookDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	if(!m_bReturnBookNonFree)
	{ 
		CESL_DataMgr* pReturnBookDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_RETURN_INFO"));
		if(pReturnBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		pReturnBookDM->FreeData();
	}
	m_bReturnBookNonFree = FALSE;

	CString sData;

	const INT nColCnt = 48;
	CString sCAlias[nColCnt] =
	{
		_T("�ηϿ���")				, _T("IBS_û��_������ȣ")	, _T("IBS_û��_�з���ȣ")	, 
		_T("IBS_û��_������ȣ")		, _T("��������")			, _T("�����ڷ����")		, 
		_T("å��")					, _T("åKEY")				, _T("��Ϲ�ȣ")			, 
		_T("IBS_û��_��ġ��ȣ_�ڵ�"), _T("IBS_û��_��å��ȣ")	, _T("����")				, 
		_T("����/���ӱ���")			, _T("����/�ݳ�����KEY")	, _T("���ุ����")			,
		_T("�ݳ�������")			, _T("�ڷ��")				, _T("�������")			, 
		_T("��ȣ��")				, _T("��ǥ��")				, _T("��KEY")				, 
		_T("������")				, _T("������")				, _T("�������")			, 
		_T("�����̿��ڼ�")			, _T("RFID�ø���")			, _T("�ݳ�����Ƚ��")		, 
		_T("��������")				, _T("��ü����")			, _T("��ȣ����KEY")			,
		_T("��ϱ���")				, _T("���δ��⿹�����")	, _T("L_DEVICE")			, 
		_T("R_DEVICE")				, _T("L_WORKER")			, _T("R_WORKER")			,
		_T("û����ȣ")				, _T("�ڷ��_����")						, _T("������")					,
		_T("���⵵������ȣ")		, _T("�������������ȣ")				, _T("å�η������Ϳ���")		, 
		_T("���⵵����")			, _T("Ÿ���������")					, _T("LCT_KEY")					,
		_T("�ηϹ߼�����")			, _T("�ηϵ�Ϲ�ȣ����Ʈ")				, _T("������������ȣ")			

	};

	CString strManageCode, strLocalKey;
	pLoanBookDM->FreeData();

	for(INT i = 0; i < TmpDM.GetRowCount(); i++)
	{
		pLoanBookDM->InsertRow(-1);
		for(INT j = 0; j < nColCnt; j++)
		{
			sData.Empty();
			ids = TmpDM.GetCellData(i, j, sData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			ids = pLoanBookDM->SetCellData(sCAlias[j], sData, i);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			
			if ( _tcscmp(sCAlias[j], _T("LCT_KEY")) == 0 && sData.IsEmpty() == FALSE ) {
					ids = pLoanBookDM->SetCellData(_T("Ÿ������KEY"), sData, i);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
				}				
			if ( _tcscmp(sCAlias[j], _T("������������ȣ")) == 0 && sData.IsEmpty() == FALSE) {
				ids = pLoanBookDM->SetCellData(_T("�������������ȣ"), sData, i);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			}
		}
		pLoanBookDM->SetCellData(_T("��Ȳ����"), _T("����"), i);
		strLocalKey = pLoanBookDM->GetCellData(_T("����/�ݳ�����KEY"), i);
		pLoanBookDM->SetCellData(_T("����KEY"), strLocalKey, i);

	}
	nFINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - [%s] �ɸ� �ð� :  [%d.%d sec]"), sLoanBookDMAlias, (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	pLoanBookDM->QueryLog(strMsg.GetBuffer(0));
	
	ids = MakeShelfCode(sLoanBookDMAlias);
	if(0 > ids) return ids;
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		CString strGetData;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����ȸ������"), strGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, _T("SearchLoanBookInfo"));
		if(_T("Y") == strGetData)
		{
				ids = KL_UserLoanInfo(strGetData);
				if(0 > ids) return ids;
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::FreeLoanInfoDM()
{
EFS_BEGIN

	CString strFuncName = _T("FreeLoanInfoDM");

	CString sLoanBookDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pLoanBookDM = m_pParentMgr->FindDM(sLoanBookDMAlias);
	if(pLoanBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pLoanBookDM->FreeData();

	if(!m_bReturnBookNonFree)
	{ 
		CESL_DataMgr* pReturnBookDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_RETURN_INFO"));
		if(pReturnBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		pReturnBookDM->FreeData();
	}
	m_bReturnBookNonFree = FALSE;
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddLoanInfo(TCHAR **sSrcLoanBookInfoFieldAlias, CESL_DataMgr *pSrcDM)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AddLoanInfo");

	const INT nCopyColumnCnt = 42;
	TCHAR *sSrcLoanInfoFieldAlias[nCopyColumnCnt] = 
	{
		_T("����/�ݳ�����KEY"), _T("��KEY"), _T("åKEY"), _T("��Ϲ�ȣ"), _T("MARC"),
		_T("������"),_T("�ݳ�������"),_T("������"),_T("���ุ����"),_T("��ü�ϼ�"),
		_T("�������"),				_T("�ڷ��"),				_T("����/���ӱ���"),		_T("IBS_û��_��ġ��ȣ_�ڵ�"),	_T("IBS_û��_�з���ȣ"),
		_T("IBS_û��_������ȣ"),	_T("IBS_û��_��å��ȣ"),	_T("IBS_û��_������ȣ"),	_T("å��"),						_T("��ǥ��"),
		_T("�η��ڷ��"),			_T("�ݳ�����Ƚ��"), 		_T("�����ڷ����"),			_T("����"),						_T("��ȣ��"), 
		_T("��������"),				_T("�ηϿ���"),				_T("�����ڷ����"),			_T("�������"),					_T("RFID�ø���"),
		_T("��ü����"),				_T("��������"),				_T("��ȣ����KEY"),			_T("�����̿��ڼ�"),				_T("L_DEVICE"), 
		_T("R_DEVICE"),				_T("L_WORKER"),				_T("R_WORKER"),				_T("û����ȣ"),					_T("������"),
		_T("�������������ȣ"),		_T("���⵵����")
	};

	CString sLoanBookDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pLoanBookDM = m_pParentMgr->FindDM(sLoanBookDMAlias);
	if(pLoanBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);


	CString sTmpSrcData;
	CString sAppendixCnt;	
	CString sSql;			
	INT nInsertRowIndex;
	for(INT i = 0 ; i < pSrcDM->GetRowCount() ; i++)
	{
		for(INT j = 0 ; j < nCopyColumnCnt ; j++)
		{			
			if(!(m_pManageValue->m_bVolReserve) && (32==j || 33==j))
				continue;

			sTmpSrcData.Empty();
			ids = pSrcDM->GetCellData(sSrcLoanBookInfoFieldAlias[j], i, sTmpSrcData);
			if(0 > ids)
			{
				CString sEMsg;
				sEMsg.Format(_T("pSrcDM->GetCellData(%s,%d,sTmpSrcData)"), sSrcLoanBookInfoFieldAlias[j], i);
				AfxMessageBox(sEMsg);
			}
			if(0 > ids&& ids != -4012) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(j == 0)
			{
				ids = pLoanBookDM->InsertRow(-1);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
				nInsertRowIndex = pLoanBookDM->GetRowCount() -1;
			}

			ids = pLoanBookDM->SetCellData(sSrcLoanInfoFieldAlias[j], sTmpSrcData, nInsertRowIndex);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewLoanScreen(INT nMode/*=-1*/)
{
EFS_BEGIN	
	
	INT ids;
	DWORD nSTART,nFINISH;

	nSTART = GetTickCount();

	if(m_bSimpleGrid == FALSE || m_bSimpleCmd == FALSE) 
	{
		ids = AllControlDisplayNotGrid(nMode);
		if(0 > ids) return ids;
	}
	
	if(nMode == 0)
	{
		ids = AddLoanInfoGrid();
		if(0 > ids) return ids;
	}
	else
	{		
		ids = AllControlDisplayGrid();
		if(0 > ids) return ids;
	}

	if(m_bSimpleGrid == FALSE || m_bSimpleCmd == FALSE) 
	{
		InitGridColor();
		ViewGrid();
	}
	nFINISH = GetTickCount();
	
	CString strMsg;
	strMsg.Format(_T("(Finish) - [%s] �ɸ� �ð� :  [%d.%d sec]"), _T("ViewLoanScreen"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	pDM->QueryLog(strMsg.GetBuffer(0));	
	
	return 0;

EFS_END
return -1;
}

int CBL_LOC_LOAN_PROC::AllControlDisplayNotGrid(INT nMode/*=-1*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllControlDisplayNotGrid");

	const INT nCMCnt = 2;
	TCHAR *sCMAliasList[nCMCnt] = 
	{
		_T("CM_BO_LOC_3100_MEMBER_INFO"), _T("CM_BO_LOC_3100_SEARCH_INFO")
	};

	CESL_ControlMgr *pCM;
	INT nIndex = -1;
	for(INT i = 0 ; i < nCMCnt ;i++)
	{
		if(i == 0 || i == 1) nIndex = 0;
		else nIndex = -1;

		pCM = m_pParentMgr->FindCM(sCMAliasList[i]);
		if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName); 

		pCM->AllControlClear();

		ids = m_pParentMgr->AllControlDisplay(sCMAliasList[i], nIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	
	CString strValue1;
	ids = m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), strValue1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();

	if(_T("Y") != strValue1)
	{
		CString strUserNo;	
		pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_MEMBER_INFO"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		ids = pCM->GetControlMgrData(_T("�����ڹ�ȣ"), strUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pManageValue->m_sIsUnityLoanService );
		ids = pCM->SetControlMgrData(_T("�����ڹ�ȣ"), strUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
	
	ids = InitUserManageValue();
	if(0 > ids) return ids;
	
	BOOL bDelaySync = FALSE;
	if(-1 != nMode) bDelaySync = TRUE;
	
	CESL_Mgr* pMgr = m_pParentMgr->FindSM(_T("BO_LOC_3100"));
	if(NULL != pMgr)
	{
		pMgr->SendMessage(WM_USER_LOAD, (WPARAM)bDelaySync, 0);
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::MakeSearchMemberSql(CString &sWhere, CString strUserKey/*_T("")*/)
{
EFS_BEGIN

	INT i, ids;
	CString strFuncName = _T("MakeSearchMemberSql");

	const INT nControlCnt = 3;	
	TCHAR *sFieldAlias[nControlCnt] = 
	{
		_T("�������̸�"), _T("�ݳ�������"), _T("�����ڹ�ȣ") 
	};
	
	CString tmp, sBuf, sGetData[nControlCnt];
	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	CMakeSearchData MakeSearchData(m_pParentMgr);
	
	if ( strUserKey.GetLength() > 0 ) {
		if(sWhere.IsEmpty() == FALSE)			sWhere += _T(" AND ");
		tmp.Format(_T(" (CLU.REC_KEY = %s ) "), strUserKey);		
		sWhere += tmp;
		return 0;
	}
	
	for(i=0; i<nControlCnt; i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sFieldAlias[i], sGetData[i]);
		if(0 > ids)	ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		sGetData[i].TrimLeft(); sGetData[i].TrimRight();
	}

	if(sGetData[0].IsEmpty() == FALSE && sGetData[2].IsEmpty() == TRUE)
	{
		if(sWhere.IsEmpty() == FALSE)		sWhere += _T(" AND ") ;
		
		MakeSearchData.GetKeyWord(sGetData[0]);
		tmp.Format(_T("CLU.IDX_USER_NAME BETWEEN '%s' AND '%s��'"), sGetData[0], sGetData[0]);
		sWhere += tmp;		
		return 0;
	}
	
	if(sGetData[2].IsEmpty() == FALSE)
	{	
		sBuf = sGetData[ 2 ];
		
		CString sValue;
		m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("�����ڹ�ȣ�ڵ��ο�����"), sValue);
		sValue.TrimLeft();	sValue.TrimRight();	sValue.MakeUpper();
		if(sValue!=_T("U"))
		{
			sBuf.MakeUpper();
		}
		
		CString strName = sGetData[2];
		MakeSearchData.GetKeyWord(strName);
		
		if(sWhere.IsEmpty() == FALSE)			sWhere += _T(" AND ");

		if( m_pManageValue->m_strScccardNoYN.CompareNoCase(_T("Y")) == 0 ){
			tmp.Format(_T("(CLU.USER_NO = '%s' OR CLU.USER_ID  = '%s' OR (CLU.IDX_USER_NAME = '%s') OR (CLU.SCCCARD_NO = '%s') )"), sGetData[2], sBuf, strName, strName, sGetData[2]);
		} else {			
			tmp.Format(_T("(CLU.USER_NO = '%s' OR CLU.USER_ID  = '%s' OR (CLU.IDX_USER_NAME = '%s'))"), sGetData[2], sBuf, strName, strName);			
		}
		
		sWhere += tmp;

		return 0;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::Loan(INT nMode)
{
EFS_BEGIN
	
	this->SetBEGIN_SBL();	
	
	INT ids;
	BOOL IsLoan = false;
	CString sRegNo;
	
	StartLog(_T("����:��Ϲ�ȣ��ȯ"));
	
	if(2 == nMode)
	{		
		ids = MakeRegNoSpaceToZero(FALSE);
		if(0 > ids) return ids;
	}
	else
	{
	ids = MakeRegNoSpaceToZero(TRUE);
	if(0 > ids) return ids;
	}	
	EndLog();

	StartLog(_T("����:�̿��ڰ˻��Ǿ�����Ȯ��"));
	ids = IsSearchedUser();
	EndLog();
	if(0 > ids) return ids;
	if(ids > 0)
	{		
		if(2 != nMode)
		{
		
		MoveRegNoToNextProcBasket(EXP_LOAN);
		}
		return 0;
	}

	StartLog(_T("����:�����ڰ˻�"));
	ids = IsLoanMember(IsLoan);
	EndLog();
	if(0 > ids) return ids;

	if(!IsLoan) return 0;
	
	ids = IsRegNo(sRegNo);
	if(0 > ids) return ids;
	
	if(sRegNo.IsEmpty())
	{
		SelfCloseMessageBox(_T("������ å�� ������ �ֽʽÿ�"));
		return 0;
	}

	StartLog(_T("����:�����۾�"));
	ids = Loan(sRegNo, nMode);
	EndLog();

	if(1==ids) return ids;
	else if(ids > 0)
	{
		InitExpDM();
		return ids;
	}
	else if(0 > ids)
	{
		ids = InitExpDM();
		return ids;
	}

	if(2 != nMode)
	{
		StartLog(_T("����:�ηϴ���"));
		ids = AppendixLoan();
		EndLog();
		if(0 > ids) return ids;
	}
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	INT nRowCount = pDM->GetRowCount();
	this->SetEND_SBL(nRowCount);

	ids = InitExpDM();
 	if(0 > ids) return ids;
	
 	StartLog(_T("����:��������������"));
 	ids = AddMemberInfo();
 	EndLog();
 	if(0 > ids) return ids;	
 	
 	StartLog(_T("����:ȭ�鱸��"));
 	ids = ViewLoanScreen();
 	EndLog();
 	if(0 > ids) return ids;

	SelfCloseMessageBox(_T("����Ǿ����ϴ�."));
	
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");	
	CESL_Grid* pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"),_T("MainGrid"));
	pMainGrid->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);	
	
	if(m_ReceiptDlg != NULL)
	{
		
		m_pParentMgr->PostMessage(WM_RECEIPT_CHANGE_TAB, 0, 1);
	}
	
	return 0;

EFS_END
	return -1;
}


INT CBL_LOC_LOAN_PROC::OtherLoan(INT nMode)
{	
	CString strOtherLoanKey;
	CESL_Grid* pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"),_T("MainGrid"));

	CString strLoanBookCnt;

	
	INT nSelectCnt;
	pMainGrid->SelectMakeList();
	nSelectCnt = pMainGrid->SelectGetCount();
	if(0 >= nSelectCnt)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return 0;
	}

	INT ids;
	BOOL IsLoan = FALSE;
	
	StartLog(_T("����:�����ڰ˻�"));
	ids = IsLoanMember(IsLoan);
	EndLog();
	if(0 > ids) return ids;

	if(!IsLoan) return 0;

	INT nLoanCnt = 0;
	
	if(NULL != pMainGrid)
	{
	
		CString sMsg = _T("������ �����͸� Ÿ������ �Ͻðڽ��ϱ�?");
		ids = MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("Ÿ������"), MB_ICONWARNING | MB_YESNO);
		if(ids == IDNO)
		{
			return 0;
		}
		pMainGrid->SelectMakeList();
		
		CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		if(pMainGrid->SelectGetCount())
		{
			INT nRowCount = pMainGrid->SelectGetCount();
			
			INT nIdx2 = pMainGrid->SelectGetTailPosition();

			CString strMsg;
			for(int j=0;j<nRowCount;j++)
			{
				CString strOtherLoanKey2 = pDM->GetCellData(_T("Ÿ������KEY"),nIdx2);
				CString strTranCode = pDM->GetCellData(_T("Ʈ����ǻ���"),nIdx2);
				CString strRegNo = pDM->GetCellData(_T("��Ϲ�ȣ"),nIdx2);

				if(strOtherLoanKey2.IsEmpty() || (!strOtherLoanKey2.IsEmpty() && _T("3")!=strTranCode))
				{
					strMsg.Format(_T("��Ϲ�ȣ�� '%s'�� �ڷ�� �Լ� ���°� �ƴմϴ�."), strRegNo);
					SelfCloseMessageBox(strMsg);
					return 0;
				}
								
		
				CString strLoanLibCode;

				ids = pDM->GetCellData(_T("���⵵������ȣ"), nIdx2, strLoanLibCode);
				if (_T("3") == strTranCode && strLoanLibCode != m_pParentMgr->m_pInfo->LIB_CODE) {
					strMsg.Format(_T("��Ϲ�ȣ�� '%s'�� �ڷ�� �ڰ����� �Լ��� �ڷᰡ �ƴմϴ�."), strRegNo);
					SelfCloseMessageBox(strMsg);
					return 0;
				}

				nIdx2 = pMainGrid->SelectGetPrev();
			}

			CString strRegNo, strTitle, strAppendixSendYN, strAppendixRegNoList;					
			CString strReturnExpireDate = _T("");
			CTime tCurrent = CTime::GetCurrentTime();

			CString strLoanExpiryFile;
			m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("�������"), _T("���⸸���ڷ���⿩��"), strLoanExpiryFile);
			strLoanExpiryFile.TrimLeft();strLoanExpiryFile.TrimRight();
			if (strLoanExpiryFile.IsEmpty())
			{
				strLoanExpiryFile = _T("Y");
			}

			INT nIdx = pMainGrid->SelectGetHeadPosition();
			for(int i=0;i<nRowCount;i++)
			{
				CString sData;
				strOtherLoanKey = pDM->GetCellData(_T("Ÿ������KEY"),nIdx);
				if(0!=strOtherLoanKey.GetLength())
				{
				
					pDM->GetCellData(_T("��Ϲ�ȣ"), nIdx, strRegNo);
					pDM->GetCellData(_T("��ǥ��"), nIdx, strTitle);
					pDM->GetCellData(_T("�ηϹ߼�����"), nIdx, strAppendixSendYN);
					pDM->GetCellData(_T("�ηϵ�Ϲ�ȣ����Ʈ"), nIdx, strAppendixRegNoList);
					if (_T("Y") == strAppendixSendYN) {
						strMsg.Format(
							_T("�η��ڷ�� �Բ� �Լ��� �ڷ��Դϴ�.\r\n")
							_T("\r\n")
							_T("[ %s:%s ]\r\n")
							_T("�η��ڷḦ Ȯ�� �ϼ̽��ϱ�?\r\n")
							_T(" - �ηϵ�Ϲ�ȣ : %s\r\n")
							_T("( �� : ����,  �ƴϿ� : ������� )")
							, strRegNo, strTitle, strAppendixRegNoList);
						if (IDYES != MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("Ÿ������>�η�Ȯ��"), MB_ICONINFORMATION|MB_YESNO)) {
							continue;
						}
					}
				
					pDM->GetCellData(_T("���⸸����"), nIdx, strReturnExpireDate);
					if(_T("Y") != strLoanExpiryFile && !strReturnExpireDate.IsEmpty())
					{						
						COleDateTime tReturnExpireDate(_ttoi(strReturnExpireDate.Mid(0,4)), _ttoi(strReturnExpireDate.Mid(5,2)), _ttoi(strReturnExpireDate.Mid(8,2)), 0, 0, 0);
						if(COleDateTime(tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(), 0, 0, 0) > tReturnExpireDate)
						{
							if(_T("S") == strLoanExpiryFile)
							{
								strMsg.Format(
								_T("��Ϲ�ȣ�� '%s'�� �ڷ�� ���⸸������ [%s]���� �Դϴ�.\r\n")
								_T("���⸸������ ���� �ڷḦ �����Ͻðڽ��ϱ�?\r\n")
								_T("( �� : ����,  �ƴϿ� : ������� )")
								, strRegNo, strReturnExpireDate);
								if (IDYES != MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("Ÿ������>���⸸���ϸ���Ȯ��"), MB_ICONINFORMATION|MB_YESNO)) {
									continue;	
								}
							}
							else if(_T("N") == strLoanExpiryFile)
							{
								CString strMsg = _T("");
								strMsg.Format( _T("[%s] �ڷᰡ ���� �������� ���� ���� ��ҵǾ����ϴ�."), strRegNo );
								SelfCloseMessageBox(strMsg);
								continue;
							}
						}
					}
					m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����Ѵ���å��"), strLoanBookCnt, 0);
				
					CString sLoanType;
					ids = GetLoanType(sLoanType);	
					if(0 > ids) return ids;
					if( _T("1") != sLoanType && _ttoi(strLoanBookCnt) > _ttoi(m_strMaxLoanCount)) 		
					{
						SelfCloseMessageBox(_T("���մ��Ⱑ�ɱǼ� �ѵ��� �ʰ��Ͽ� ������ �� �����ϴ�."));
						return 0;			
					}

					CString strHoldLibCode;
					strHoldLibCode = pDM->GetCellData(_T("������������ȣ"), nIdx);
					CString strLoanLibCode;
					strLoanLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
					CString strUserNo;
					m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڹ�ȣ"), strUserNo, 0);

					CString strLoanDate;					
					CTime tLoanDate = CTime::GetCurrentTime();

					CString strStringLoanDate = _T("");
					ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("�ұ޴�����"), strStringLoanDate);
					if(0 > ids || strStringLoanDate.IsEmpty()) return ids;
					strLoanDate.Format(_T("%s %02d:%02d:%02d"), strStringLoanDate, tLoanDate.GetHour(), tLoanDate.GetMinute(), tLoanDate.GetSecond());
					
					CFrameWriter Writer(m_pParentMgr->m_pInfo);
					Writer.setCommand(_T("LILL_K01_LOC_B01_SERVICE"));
					Writer.addRecord();
					Writer.addElement(_T("HOLD_LIB_CODE"), strHoldLibCode);
					Writer.addElement(_T("LOAN_LIB_CODE"), strLoanLibCode);
					Writer.addElement(_T("USER_NO"),strUserNo);
					Writer.addElement(_T("LOAN_KEY"),strOtherLoanKey);
					Writer.addElement(_T("L_DEVICE"),_T("KOLAS III"));
					Writer.addElement(_T("L_WORKER"),m_pParentMgr->m_pInfo->USER_ID);
					Writer.addElement(_T("LOAN_DATE"),strLoanDate);
					
					CFrameSender Sender(m_pParentMgr->m_pInfo);	
					CFrameReader Reader; 
					ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
					if (0 > ids) 
					{
						while(true) 
						{
							
							if(IDYES == MessageBox(m_pParentMgr->m_hWnd,_T("å�������� �̵������� ��ſ� �����Ͽ����ϴ�.\r\n��õ� �Ͻðڽ��ϱ�?"), _T("��ȸ����"), MB_YESNO | MB_ICONQUESTION)) 
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
					if(_T("OK") == Reader.getElement(0,0))
					{
						Send3000ApiSocket(_T("Ÿ������"));
						nLoanCnt++;
					}			
				}
				nIdx=pMainGrid->SelectGetNext();	
			}	
			if( nLoanCnt > 0 ) {
				SelfCloseMessageBox(_T("Ÿ�������� �Ϸ�Ǿ����ϴ�."));				
				m_bReturnBookNonFree=TRUE;
				SPFExcutePath(_T("ȸ�����ε�_NOASK"));
			}
		}
		
	}
	return 0;
}
INT CBL_LOC_LOAN_PROC::IsLoanMember(BOOL &IsLoan)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsLoanMember");

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO") ;
	CString sLoanType;
	CString sInferiorClass;
	CString sLoanStopDate;
	CString sMemberClass;
	CString sUnionInferioClass;
	CString sLostUserCard;

	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;

	if(0 == sLoanType.Compare(_T("1")))
	{
	}	
	else
	{
		IsLoan = FALSE;
		
		if(_T("Y") == m_pManageValue->m_ArrearCheckYN)
		{
			CString strArrear;
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("�ѿ�ü��"), strArrear, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(0 < _ttoi(strArrear))
			{
				SelfCloseMessageBox(_T("�̳��� ��ü�ᰡ �ֽ��ϴ�."), m_pParentMgr, 2000);
				return 0;
			}
		}

		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("ȸ������"), sInferiorClass, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(0 != sInferiorClass.Compare(_T("0")))
		{
			SelfCloseMessageBox(_T("���� ȸ������ ������ �����մϴ�."));
			return 0;
		}
		
		if(m_pManageValue->m_sLoanIsLostUserCard == _T("Y"))
		{
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("ȸ�����н�"), sLostUserCard, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(sLostUserCard == _T("Y"))
			{
				SelfCloseMessageBox(_T("ȸ�����н� ȸ���� ������ �Ұ��� �մϴ�."));
				return 0;
			}
		}
		
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("����������"), sLoanStopDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		if(0 == sLoanStopDate.Find(_T("0000")))
		{
			sLoanStopDate = _T("");
			ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("����������"), sLoanStopDate, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		}

		if(!sLoanStopDate.IsEmpty())
		{
			COleDateTime tLoanStopDate(_ttoi(sLoanStopDate.Mid(0,4)), _ttoi(sLoanStopDate.Mid(5,2)), _ttoi(sLoanStopDate.Mid(8,2)), 0, 0, 0);
			
			
			CString strGetData;
			CTime tCurrentCTime = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strGetData);

			COleDateTime tCurrent(tCurrentCTime.GetYear(), tCurrentCTime.GetMonth(), tCurrentCTime.GetDay(), 0, 0, 0);
			
			if(tCurrent <= tLoanStopDate)
			{
				SelfCloseMessageBox(_T("���� �������� ������ �ʾ����Ƿ� ������ �Ұ����մϴ�."));
				return 0;
			}
		}
		
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("ȸ������"), sMemberClass, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

		if(0 == sMemberClass.Compare(_T("1")))
		{
			SelfCloseMessageBox(_T("��ȸ���� ������ ���ѵ˴ϴ�."));			
			return 0;
		}
		
		else if(0 == sMemberClass.Compare(_T("2")))
		{
			if(IDYES == (AfxMessageBox(_T("ȸ�� ������ ���� �ʾҽ��ϴ�.\r\n")
				                         _T("ȸ�� ������ �Ͻðڽ��ϱ�?"), MB_ICONQUESTION | MB_YESNO)))
			{
				SPFExcutePath(_T("�̿���_����������"));
			}
			return 0;
		}
		
		if(0 == m_pManageValue->m_sIsUnionUserStatus.Compare(_T("Y")))
		{
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("���մ���ȸ������"), sUnionInferioClass, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
			if(0 == sUnionInferioClass.Compare(_T("��������")))
			{
				SelfCloseMessageBox(_T("Ÿ ������ �������� ȸ���Դϴ�."));
				return 0;
			}
			else if(sUnionInferioClass.IsEmpty())
			{
				SelfCloseMessageBox(_T("���մ��������� ����ȭ ���Դϴ�."));
				return 0;
			}
		}
		
		if(m_pManageValue->m_sIsUnionUserRemoveStatus == _T("Y"))
 		{
 			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("���մ���ȸ������"), sUnionInferioClass, 0);
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
 			if(sUnionInferioClass == _T("����ȸ��"))
 			{
 				SelfCloseMessageBox(_T("Ÿ ������ ����ȸ���Դϴ�."));
 				return 0;
 			}
 			else if(sUnionInferioClass.IsEmpty())
 			{
 				SelfCloseMessageBox(_T("���մ��������� ����ȭ ���Դϴ�."));
 				return 0;
 			}
 		}
	}
	IsLoan = TRUE;
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsRegNo(CString &sRegNo)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsRegNo");

	sRegNo.Empty();
	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");

	ids = m_pParentMgr->GetControlData(sCMAlias, _T("�����ڹ�ȣ"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::Loan(CString &sRegNo, INT nMode /*=0*/ )
{
EFS_BEGIN

	INT ids;

	INT nBookCnt = 0;

	ids = SearchBookInfo(sRegNo, nBookCnt, nMode);
	if(0 > ids) return ids;

	if(nBookCnt == 0) 
	{		
		nMode = 1;
		ids = SearchBookInfo(sRegNo, nBookCnt, nMode);
		if(0 > ids) return ids;
		
		if(nBookCnt == 0)
		{
			ids = MessageBox(m_pParentMgr->m_hWnd, _T("�Է��� ��Ϲ�ȣ�� ���� �ڷᰡ �������� �ʽ��ϴ�. ��޴����Ͻðڽ��ϱ�?"), _T("����/�ݳ�"), MB_YESNO | MB_ICONQUESTION);
			if(ids == IDYES)
				return 1;
			return 101;
		}
	}
	if(nBookCnt > 1)
	{
		CLoanBookListDlg BookList(m_pParentMgr);
		BookList.DoModal();
		
		if(BookList.GetCloseStatus() == 0)
		{
			SelfCloseMessageBox(_T("��ҵǾ����ϴ�."));
			return 0;
		}

	}

	CString sStatus;
	CString sBookKey;
	ids = IsBookRelationCurrentUser(sStatus,sBookKey);
	if(ids == 1)
	{
		ids = MessageBox(m_pParentMgr->m_hWnd,_T("�̹� �̿��ڰ� ������ å�Դϴ�. �ݳ��Ͻðڽ��ϱ�?"), _T("����/�ݳ�"), MB_YESNO | MB_ICONQUESTION);
		if(ids == IDYES)
		{
			ids = BarCodeReturnBookCheck(sBookKey);
			if(0 > ids) return ids;
			
			ReturnBook(TRUE);
		}
		return 2;

	}
	else if(ids == 2)
	{
		CString strControlFlag;
		GetTempData(_T("����ݳ��帧����"), strControlFlag);

		INT nYesNo = -1;
		if (strControlFlag.CompareNoCase(_T("Y")) != 0){
			nYesNo = AfxMessageBox(_T("�̹� �̿��ڰ� ������ å�Դϴ�.")_T("\r\n������� �Ͻðڽ��ϱ�?"), MB_YESNO);
		}

		if ( strControlFlag.CompareNoCase(_T("Y")) == 0 || nYesNo == IDYES ) 
		{
			ids = BarCodeReturnBookCheck(sBookKey);
			if(0 > ids) return ids;

			ids = SPFExcutePath( _T("�������") );
			if(0 > ids) return ids;
		}

		return 3;
	}

	BOOL IsLoan = false;

	ids = IsLoanBook(IsLoan, nMode);
	if(0 > ids) return ids;
	if(!IsLoan) return 2;
	
	ids = LoanDBProc(nMode);
	if(ids == 100 || ids == 101) return ids;
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchBookInfo(CString &sRegNo, INT &nBookCnt, INT nMode/*=0*/, BOOL bRegNo/*=FALSE*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SearchBookInfo");
	
	CESL_DataMgr* pExpLoanInfoDM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_BOOK_INFO"));
	if(pExpLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(nMode == 0)
	{
		if(pExpLoanInfoDM->GetRowCount() > 0)
		{
			CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);

			nBookCnt = pExpLoanInfoDM->GetRowCount();
			return 0;
		}
		
		ids = MakeInDBSearchBookInfo(sRegNo, m_pParentMgr->m_pInfo->MANAGE_CODE, nBookCnt);
		if(0 > ids) return ids;
	}
	else if(nMode == 1)
	{
		if(pExpLoanInfoDM->GetRowCount() > 0)
		{
			CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);

			nBookCnt = pExpLoanInfoDM->GetRowCount();
			return 0;
		}

		ids = MakeNotInDBSearchBookInfo(sRegNo, bRegNo, nBookCnt);
		if(0 > ids) return ids;
	}
	else if(2 == nMode)
	{
		if(pExpLoanInfoDM->GetRowCount() > 0)
		{
			CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);

			nBookCnt = pExpLoanInfoDM->GetRowCount();
			return 0;
		}

		CESL_DataMgr* pILLDM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_ILL_BOOK_INFO"));
		if(NULL == pILLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		CString strFindRegNo;
		CString strBookManageCode;		

		int nRowCnt = 0;
		const int ROW_COUNT = pILLDM->GetRowCount();
		
		for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
		{
			ids = pILLDM->GetCellData(_T("��Ϲ�ȣ"), 0, strFindRegNo);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(strFindRegNo == sRegNo)
			{
				ids = pILLDM->GetCellData(_T("��������_å"), nRowCnt, strBookManageCode);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

				ids = MakeInDBSearchBookInfo(sRegNo, strBookManageCode, nBookCnt);
				if(0 > ids) return ids;
				break;
			}

		}
	}

	if(nBookCnt > 0)
	{
		ids = AddExpLoanInfo();
		if(0 > ids) return ids;
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitExpDM()
{
EFS_BEGIN

	CString strFuncName = _T("InitExpDM");

	
	const INT nDMCount = 5;
	TCHAR *sDMAlias[nDMCount] = 
	{
		_T("DM_EXP_LOAN_BO_BOOK_INFO"), _T("DM_EXP_LOAN_SE_BOOK_INFO"), _T("DM_EXP_LOAN_NON_DB_BOOK_INFO"),
		_T("DM_EXP_LOAN_BOOK_INFO"), _T("DM_EXP_LOAN_ILL_BOOK_INFO")
	};

	CESL_DataMgr *pDM;
	for(INT i = 0 ; i < nDMCount ; i++)
	{
		pDM = m_pParentMgr->FindDM(sDMAlias[i]);
		if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		pDM->FreeData();
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::MakeInDBSearchBookInfo(CString sRegNo, CString strManageCode, INT &nBookCnt)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("MakeInDBSearchBookInfo");

	const int EXP_DM_COUNT = 2;
	
	CString DMAlias[EXP_DM_COUNT] =
	{
		_T("DM_EXP_LOAN_BO_BOOK_INFO"), _T("DM_EXP_LOAN_SE_BOOK_INFO")
	};
		
 	CString sWhere[EXP_DM_COUNT];
 	sWhere[0].Format(
 		_T("BB.MANAGE_CODE='%s' AND BB.REG_NO = '%s' AND BB.WORKING_STATUS IN (") _T(HOLD_SEARCH_CONDITION)	
		_T(",'BOL217O','BOL218O', 'BOL219O') AND BB.BOOK_APPENDIX_FLAG IN ('B','A') "),
 		strManageCode, sRegNo);
	sWhere[1].Format(
		_T("BB.MANAGE_CODE='%s' AND BB.REG_NO = '%s' AND BB.WORKING_STATUS IN ('SEL112N',") _T(HOLD_SE_SEARCH_CONDITION)
		_T(",'SEL317O','SEL318O') AND BB.BOOK_TYPE IN ('B','C','A') ")
		_T("AND BB.BINDED_BOOK_YN != 'Y' "),
 		strManageCode, sRegNo);
	
	CString strMessage[EXP_DM_COUNT] = 
	{
		_T("��������� �ڷ�"), _T("���Ӵ����� �ڷ�")
	};

	int nCnt = 0;
	CString strMsg;
	DWORD nSTART, nFINISH;
	CESL_DataMgr *pExpDM = NULL;
	for(nCnt = 0; nCnt < EXP_DM_COUNT; nCnt++)
	{
		pExpDM = m_pParentMgr->FindDM(DMAlias[nCnt]);
		if(NULL == pExpDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		nSTART = GetTickCount();
		ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pExpDM, sWhere[nCnt]);
		if(0 > ids)
		{
			return ids;
		}

		nFINISH = GetTickCount();

		strMsg.Format(_T("(Finish) - [%s] �ɸ� �ð� :  [%d.%d sec]"), strMessage[nCnt], (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
		TRACE(strMsg.GetBuffer(0));
		pExpDM->QueryLog(strMsg.GetBuffer(0));

		nBookCnt += pExpDM->GetRowCount();

		if(nBookCnt > 0)
		{
			return 0;
		}
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::MakeNotInDBSearchBookInfo(CString sRegNo, BOOL bRegNo, INT &nBookCnt)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("MakeNotInDBSearchBookInfo");

	CString sLoanNonDBBookDMAlias = _T("DM_EXP_LOAN_NON_DB_BOOK_INFO");

	CString sDBFieldAlias;

	sDBFieldAlias = _T("REG_NO");

	CString sWhere;

	sWhere.Format(_T("NB.%s = '%s' AND ")
						_T("NB.WORKING_STATUS IN ")
						_T("(")
						_T(HOLD_SEARCH_CONDITION)
						_T(",'BOL217O','BOL218O', 'BOL219O')")
						, sDBFieldAlias, sRegNo);
	

	CESL_DataMgr *pBookDM = m_pParentMgr->FindDM(sLoanNonDBBookDMAlias);
	if(pBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	DWORD nSTART, nFINISH;
	nSTART = GetTickCount();
	
	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pBookDM, sWhere);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	nFINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - [��� ������ �ڷ�] �ɸ� �ð� :  [%d.%d sec]"),  (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	TRACE(strMsg.GetBuffer(0));
	pBookDM->QueryLog(strMsg.GetBuffer(0));

	nBookCnt += pBookDM->GetRowCount();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddExpLoanInfo()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AddExpLoanInfo");

	CString sDstDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CESL_DataMgr *pDstDM = m_pParentMgr->FindDM(sDstDMAlias);
	if(pDstDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	const INT nFieldCnt = 39;
	TCHAR *sDstDMFieldAlias[nFieldCnt] = 
	{
		_T("åKEY"), _T("����"), _T("�ڷ�Ǳ���"), _T("����/���ӱ���"), _T("�з���ȣ����"), 
		_T("�з���ȣ"), _T("��������"), _T("��ϱ���"), _T("������Ű"), _T("��Ϲ�ȣ"), 
		_T("MARC"), _T("��ġ��ȣ"), _T("�̿����ѱ���"), 
		_T("IBS_û��_��ġ��ȣ_�ڵ�"),	
		_T("IBS_û��_�з���ȣ"),		
		_T("IBS_û��_������ȣ"),		
		_T("IBS_û��_��å��ȣ"),		
		_T("IBS_û��_������ȣ")	,	
		_T("å��"), _T("���ı���"), _T("�̿��󱸺�"), _T("��ǥ��"), _T("������"), _T("������"),
		_T("��ȣ��"), 
		_T("��������"),
		_T("�ηϿ���"), 
		_T("�����ڷ����"), 
		_T("�������"),
		_T("������KEY"), 
		_T("���ιݳ��߿���"),
		_T("����Ǽ�"),
		_T("�η�å��"),
		_T("RFID�ø���"),
		_T("��������"),
		_T("��ü����"),
		_T("��ȣ����KEY"),
		_T("�����̿��ڼ�"),
		_T("û����ȣ") 
	};

	const INT nLoanInfoDMCnt = 3;
	TCHAR *sLoanInfoDMAlias[nLoanInfoDMCnt] =
	{
		_T("DM_EXP_LOAN_BO_BOOK_INFO"), _T("DM_EXP_LOAN_NON_DB_BOOK_INFO"), _T("DM_EXP_LOAN_SE_BOOK_INFO")
	};
	
	INT nCnt = 3;

	CESL_DataMgr* pDM;

	for(INT i = 0 ; i < nCnt ; i++)
	{
		
		ids = AddLoanBookDM(sDstDMAlias, sDstDMFieldAlias, sLoanInfoDMAlias[i], sDstDMFieldAlias, nFieldCnt);
		if(0 > ids) return ids;

		pDM = m_pParentMgr->FindDM(sLoanInfoDMAlias[i]);
	}

	
 	MakeShelfCode(sDstDMAlias);

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::IsLoanBook(BOOL &IsLoanBook, INT nMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsLoanBook");

	CString sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CString sBookState;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("����"), sBookState, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sPublishForm;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("����/���ӱ���"), sPublishForm, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString strBookManageCode;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("��������"), strBookManageCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	if(2 != nMode)
	{
	if(sPublishForm.Compare(_T("MO"))==0 || (sPublishForm.Compare(_T("NB"))==0))
	{
		if(!(sBookState.Compare(_T("BOL112N"))==0)) 
		{
			
			CString sReserveMode;
			ids = m_pCheckLoan->GetReserveMode(sReserveMode);
			if(0 > ids) return ids;			
#ifdef	__K2UP__
			
			if((sBookState.Compare(_T("BOL211O"))==0)
			  ||(sBookState.Compare(_T("BOL212O"))==0)
			  ||(sBookState.Compare(_T("BOL213O"))==0)
			  ||(sBookState.Compare(_T("BOL214O"))==0)
			  ||(sBookState.Compare(_T("BOL215O"))==0)
			  ||(sBookState.Compare(_T("BOL217O"))==0)
			  ||(sBookState.Compare(_T("BOL218O"))==0)
			  ||(sBookState.Compare(_T("BOL219O"))==0)
			  
			  ||(sBookState.Compare(_T("BOL411O"))==0)
			  ||(sBookState.Compare(_T("BOL511O"))==0)
			  
			  ||(sBookState.Compare(_T("BOL611O"))==0)
			 )
#else
			if((sBookState.Compare(_T("BOL221O"))==0)
	     	  ||(sBookState.Compare(_T("BOL222O"))==0)
		  	  ||(sBookState.Compare(_T("BOL223O"))==0)
			  ||(sBookState.Compare(_T("BOL224O"))==0)
			  ||(sBookState.Compare(_T("BOL225O"))==0)
			 )			
#endif
			{
				if(_ttoi(sReserveMode)==0)
				{
					IsLoanBook = FALSE;
					SelfCloseMessageBox(_T("�̹� ����� å�̹Ƿ� ���� �� �� �����ϴ�."));
					return 0;
				}
				CString sMsg;
				sMsg.Format(_T("�����Ϸ��� å�� �̹� ������ �� ������ å�Դϴ�. \r\n�����Ͻðڽ��ϱ�?"));
				ids = MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("�����ϱ�"), MB_ICONQUESTION | MB_YESNO);
				if(IDYES == ids)
				{
					SPFExcutePath(_T("����"));			
				}
				IsLoanBook = FALSE;				
				return 0;
			}
			
			CString sMsg;
			GetWorkingStatusDescToCode(sBookState, 1);
			sMsg.Format(_T("���� å�� ���°� (%s)�̱� ������ ���� �� �� �����ϴ�."), sBookState);
			SelfCloseMessageBox(sMsg);
			IsLoanBook = FALSE;

			return 0;
		}
	}
	else
	{
		if(!(sBookState.Compare(_T("SEL112N"))==0) && !(sBookState.Compare(_T("SEL212N"))==0)) 
		{
			
			CString sReserveMode;
			ids = m_pCheckLoan->GetReserveMode(sReserveMode);
			if(0 > ids) return ids;			
			
			if((sBookState.Compare(_T("SEL311O"))==0)
			  ||(sBookState.Compare(_T("SEL312O"))==0)
 			  ||(sBookState.Compare(_T("SEL411O"))==0)
			  ||(sBookState.Compare(_T("SEL511O"))==0)
			  ||(sBookState.Compare(_T("SEL313O"))==0)
			  ||(sBookState.Compare(_T("SEL314O"))==0)
			  ||(sBookState.Compare(_T("SEL315O"))==0)
			)
			{				
				if(_ttoi(sReserveMode)==0)
				{
					IsLoanBook = FALSE;
					SelfCloseMessageBox(_T("�̹� ����� å�̹Ƿ� ���� �� �� �����ϴ�."));
					return 0;
				}

				CString sMsg;
				sMsg.Format(_T("�����Ϸ��� å�� �̹� ������ �� ������ å�Դϴ�. \r\n �����Ͻðڽ��ϱ�?"));
				ids = MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("�����ϱ�"), MB_ICONQUESTION | MB_YESNO);
				if(ids == IDYES) 
					SPFExcutePath(_T("����"));			
				IsLoanBook = FALSE;
				return 0;
			}
			
			CString sMsg;
			GetWorkingStatusDescToCode(sBookState, 1);
			sMsg.Format(_T("���� å�� ���°� (%s)�̱� ������ ���� �� �� �����ϴ�."), sBookState);
			SelfCloseMessageBox(sMsg);
			IsLoanBook = FALSE;
			return 0;
		}
		
		else if(_T("SEL112N") == sBookState)
		{
			CString strRegNo;
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("��Ϲ�ȣ"), strRegNo, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			if(strRegNo.IsEmpty())
			{
				SelfCloseMessageBox(_T("��Ϲ�ȣ�� �ο����� ���� �ڷ�� ���� �� �� �����ϴ�."));
				IsLoanBook = FALSE;
				return 0;
			}
		}
	}
	
	CString sBookRecKey;
	CString sReserveCount;
	CString sVolReserveCnt;
	CString sVolInfoKey;
	CString sSpeciesKey;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("åKEY"), sBookRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("������Ű"), sSpeciesKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("��ȣ����KEY"), sVolInfoKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	ids = IsReservatedBook(sBookRecKey, sSpeciesKey, sVolInfoKey, sReserveCount, sVolReserveCnt);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	if((_ttoi(sReserveCount)>0) || (_ttoi(sVolReserveCnt)>0)) 
	{
		CString sMsg;
		sMsg.Format(_T("�����Ϸ��� å�� �ݳ��� �Ǿ����� �ٸ� �̿��ڿ� ���� ������ �� ������ å�Դϴ�. \r\n �����Ͻðڽ��ϱ�?"));
		ids = MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("�����ϱ�"), MB_ICONQUESTION | MB_YESNO);
		if(IDYES==ids)
		{
			m_bLoanCallReserve = TRUE;
			SPFExcutePath(_T("����"));			
		}
		IsLoanBook = FALSE;
		return 0;
	}
	}	
	
	else if(nMode == 2)
	{
		CString strCompareState;
		if(sPublishForm.Compare(_T("MO"))==0 || (sPublishForm.Compare(_T("NB"))==0))
		{
			strCompareState = _T("BOL411O");
		}
		else
		{
			strCompareState = _T("SEL411O");
		}
		if(!(sBookState.Compare(strCompareState)==0)) 
		{
			
			CString sMsg;
			GetWorkingStatusDescToCode(sBookState, 1);
			sMsg.Format(
				_T("å�η� ����ڷᰡ �ƴմϴ�.\r\n")
				_T("���� å�� ���°� (%s)�̱� ������ ���� �� �� �����ϴ�."), sBookState);
			SelfCloseMessageBox(sMsg);
			IsLoanBook = FALSE;
			
			return 0;
		}
				
		CString strBookKey = _T("");
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("åKEY"), strBookKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
		
		CString strQuery;
		CString strValue;
		strQuery.Format(
			_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS IN ('L', 'D')"),
			strBookKey
			);
		
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		ids = TmpDM.GetOneValueQuery(strQuery, strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
		
		if(_T("0") != strValue)
		{
			CString sMsg;		
			sMsg.Format(			
				_T("���� å�� �̹� å�η��������̱� ������ ���� �� �� �����ϴ�."));
			SelfCloseMessageBox(sMsg);
			IsLoanBook = FALSE;
		}	
	}

	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;

	if(_ttoi(sLoanType) != 1)
	{		
		if(2 != nMode )
		{
		ids = IsUserBookLoan();
		}
		else
		{			
			ids = IsUserBookLoan(EXP_LOAN, TRUE, strBookManageCode);
		}
		if(0 > ids) return ids;
		if(ids > 0)
		{
			IsLoanBook = FALSE;
			return 0;
		}
	}

	if(2 == nMode)
	{
		IsLoanBook = TRUE;
		return 0;
	}

	ids = IsReLoanBook();

	if(_ttoi(sLoanType) == 1 && ids != 0)
	{		
		if(11 == ids)
			ids = 1;
		else if(22 == ids)
			ids = 2;
	}
	if(0 > ids) return ids;
	if(ids > 0)
	{
		if(m_pCheckLoan->IsViewReLoanHistory() == 0)
		{
			ids = ViewLoanHistory(ids,0);
			if(ids == 1)
			{
				IsLoanBook = FALSE;
				return 0;
			}
		}
		else
		{
			if(ids == 11 || (ids==22))
			{
				CString sMsg;
				if(ids == 11) sMsg.Format(_T("������ ���� �Ǿ��� å�Դϴ�. �����ϽǼ� �����ϴ�. "));
				if(ids == 22) sMsg.Format(_T("������ �Ѹ��� ������ ���� �߾��� å�Դϴ�. �����ϽǼ� �����ϴ�. "));
				SelfCloseMessageBox(sMsg);
				IsLoanBook = FALSE;
				return 0;
			}			
		}
	}
	
	IsLoanBook = TRUE;
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::LoanDBProc(INT nMode/*=0*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("LoanDBProc");
	
	CString strBookManageCode, strBookLibCode;

	BOOL bIsILL = (2 == nMode) ? TRUE : FALSE;

	CString sMode;
	sMode.Format(_T("%d"), nMode);
	
	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CKolas2up_MobileApi *pMobileApi;
	pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);	
	
	CESL_DataMgr *pKLDM = m_pParentMgr->FindDM(_T("DM_KL_LOAN"));
	if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr *pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CString strKLUserYN;
	pMemberDM->GetCellData(_T("����ȸ������"), 0, strKLUserYN);
	if(_T("Y") != strKLUserYN)
	{
		strKLUserYN = _T("N");
	}
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		pKLDM->FreeData();
		pKLDM->InsertRow(-1);
	}
	
	ids = InsertLoanRecord(pMobileApi, bIsILL, strKLUserYN, nMode);

	if(ids == 100 || ids == 101) return ids;
	
	if(0 > ids) return ids;
	
	if(FALSE == bIsILL)
	{
		ids = UpdateBookInfo(pMobileApi,sMode);
		if(0 > ids) return ids;
	}
	else
	{
		CESL_DataMgr* pILLDM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_ILL_BOOK_INFO"));
		if(NULL == pILLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		CString strTransNo;
		ids = pILLDM->GetCellData(_T("Ʈ����ǹ�ȣ"), 0, strTransNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = ILL_UpdateTransInfo(TRUE, strTransNo);
		if(0 > ids) return ids;
	}

	ids = UpdateLoanUserUpdate(pMobileApi,  strKLUserYN);
	if(0 > ids) return ids;
	
	ids = pMobileApi->SendFrame();
	if(0 > ids) return ids;	
	pMobileApi->EndFrame();
	
	CString sSrcDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("��������"), strBookManageCode, 0);

	CESL_DataMgr tdm;
	tdm.SetCONNECTION_INFO(m_pManageValue->m_pInfo->CONNECTION_INFO);
	CString strQuery;
	
	CString strBookLibKLUse;
	strQuery.Format(_T("SELECT LIB_CODE, (SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE VALUE_NAME = '���յ�������' AND MANAGE_CODE = ML.MANAGE_CODE) FROM MN_LIBINFO2_TBL ML WHERE MANAGE_CODE = '%s' AND ROWNUM = 1 "), strBookManageCode);
	ids = tdm.RestructDataManager(strQuery);
	if ( ids < 0) return ids;

	strBookLibCode = tdm.GetCellData(0, 0);
	strBookLibKLUse = tdm.GetCellData(0, 1);
	
	if( (bIsILL == TRUE && strBookLibKLUse.Compare(_T("Y")) == 0) || bIsILL == FALSE){
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
		{
			
			CFrameWriter Writer(m_pParentMgr->m_pInfo);
			Writer.setCommand(_T("IL_K10_LOC_B01_SERVICE"));
			Writer.addRecord();
			CString strGetData;
			ids = pKLDM->GetCellData(_T("���մ����ڹ�ȣ"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("���մ����ڹ�ȣ"), strGetData);
			
			CString strGpinHash;
			pMemberDM->GetCellData(_T("G-PINHASH�����ڵ�"), 0, strGpinHash);
			Writer.addElement(_T("G-PINHASH�����ڵ�"), strGpinHash);
			Writer.addElement(_T("���⵵����"), strBookLibCode);
	
			ids = pKLDM->GetCellData(_T("������"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("������"), strGetData);
			ids = pKLDM->GetCellData(_T("�ݳ�������"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("�ݳ�������"), strGetData);
			ids = pKLDM->GetCellData(_T("����"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			
			if ( bIsILL == TRUE ) {
				if ( strGetData.Compare(_T("L")) == 0 || strGetData.Compare(_T("0")) == 0 ) strGetData = _T("0");
				else if ( strGetData.Compare(_T("R")) == 0 || strGetData.Compare(_T("1")) == 0 ) strGetData = _T("1");
				else if ( strGetData.Compare(_T("D")) == 0 || strGetData.Compare(_T("2")) == 0 ) strGetData = _T("2");
			}
			Writer.addElement(_T("����"), strGetData);
	
			ids = pKLDM->GetCellData(_T("��Ϲ�ȣ"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("��Ϲ�ȣ"), strGetData);
			ids = pKLDM->GetCellData(_T("����"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("����"), strGetData);
	 	 	ids = pKLDM->GetCellData(_T("����"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	 	 	Writer.addElement(_T("����"), strGetData);
	 	 	ids = pKLDM->GetCellData(_T("������"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	 	 	Writer.addElement(_T("������"), strGetData);
			ids = pKLDM->GetCellData(_T("û����ȣ"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("û����ȣ"), strGetData);
			ids = pKLDM->GetCellData(_T("����������"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("����������"), strGetData);
			ids = pKLDM->GetCellData(_T("��å��ȣ"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("�����"), strGetData);
			ids = pKLDM->GetCellData(_T("����KEY"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("����KEY"), strGetData);
			
			ids = pKLDM->GetCellData(_T("�ηϿ���"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("�ηϿ���"), strGetData);
			
			if ( bIsILL == TRUE) {
				Writer.addElement(_T("��������ȣ��������"), _T("Y"));
			}
				
	 	 	CFrameSender Sender(m_pParentMgr->m_pInfo);
	 	 	Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("����ȸ������"), __WFILE__, __LINE__));
		}
	}
	
	CLocCommonAPI::MobileApiEndLog(pMobileApi);

	INT nMode = 0;
	if(_T("Y") == m_pManageValue->m_sIsShareInsert)
	{
		nMode = 1;
	}
	ShareInfoCenterUpload(_T("I"), _T("U"), nMode);	
	ids = UpdateSuccessDM();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InsertLoanRecord(CKolas2up_MobileApi *pMobileApi, BOOL bIsILLMode, CString strKLUserYN, INT i_nMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("InsertLoanRecord");
	
	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();
	pDM->StartFrame();
	
	CString sSrcDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	const INT nSrcFieldCnt = 15;
	TCHAR *sSrcFieldAlias[nSrcFieldCnt] = 
	{
		_T("åKEY"), _T("�ڷ�Ǳ���"), _T("����/���ӱ���"), _T("�з���ȣ����"), _T("�з���ȣ"), 
		_T("��������"), _T("��ϱ���"), _T("������Ű"), _T("��Ϲ�ȣ"), _T("��ġ��ȣ"), 
		_T("��ǥ��"), _T("������"), _T("������"), _T("û����ȣ"), _T("��ȣ����KEY")		
	};
	TCHAR *sDstFieldAlias[nSrcFieldCnt] = 
	{
		_T("BOOK_KEY"), _T("SHELF_LOC_CODE"), _T("PUBLISH_FORM_CODE"), _T("CLASS_NO_TYPE"), _T("CLASS_NO"), 
		_T("MANAGE_CODE"), _T("REG_CODE"), _T("SPECIES_KEY"), _T("REG_NO"), _T("SEPARATE_SHELF_CODE"), 
		_T("TITLE"), _T("AUTHOR"), _T("PUBLISHER"), _T("CALL_NO"), _T("VOL_INFO")
	};
	TCHAR *sDstFieldTypeAlias[nSrcFieldCnt] = 
	{
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("NUMERIC"), _T("STRING"), _T("STRING"),
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC")
	};

	CESL_DataMgr *pKLDM = m_pParentMgr->FindDM(_T("DM_KL_LOAN"));
	if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sGetData;
	for(INT i= 0 ; i < nSrcFieldCnt ;i++)
	{
		if(!(m_pManageValue->m_bVolReserve) && (i+1)==nSrcFieldCnt) break;

		ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, sSrcFieldAlias[i], sGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		if(CString(_T("NUMERIC")).Compare(sDstFieldTypeAlias[i])==0)
		{
			if(sGetData.IsEmpty()) sGetData = _T("NULL");
		}
 		
 		if(_T("MANAGE_CODE") == sDstFieldAlias[i])
 		{
			sGetData = m_pParentMgr->m_pInfo->MANAGE_CODE;
 		}
		ids = pDM->AddDBFieldData(sDstFieldAlias[i], sDstFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
		{
			if(_T("REG_NO") == sDstFieldAlias[i])
			{
				ids = pKLDM->SetCellData(_T("��Ϲ�ȣ"), sGetData, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			}
			else if(_T("TITLE") == sDstFieldAlias[i])
			{
				ids = pKLDM->SetCellData(_T("����"), sGetData, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			}
 			else if(_T("AUTHOR") == sDstFieldAlias[i])
 			{
 				ids = pKLDM->SetCellData(_T("����"), sGetData, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
 			}
 			else if(_T("PUBLISHER") == sDstFieldAlias[i])
 			{
 				ids = pKLDM->SetCellData(_T("������"), sGetData, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
 			}
			else if(_T("CALL_NO") == sDstFieldAlias[i])
			{
				ids = pKLDM->SetCellData(_T("û����ȣ"), sGetData, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			}
		}
	}	
	
	CString sSrcUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	const INT nSrcUserFieldCnt = 4;
	TCHAR *sSrcUserFieldAlias[nSrcUserFieldCnt] = 
	{_T("������KEY"), _T("�����ڼҼ�"), _T("����������"), _T("ȸ������")};
	TCHAR *sDstUserFieldAlias[nSrcUserFieldCnt] = 
	{_T("USER_KEY"), _T("USER_POSITION_CODE"), _T("USER_CLASS_CODE"), _T("MEMBER_CLASS")};
	TCHAR *sDstUserFieldTypeAlias[nSrcUserFieldCnt] = 
	{_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING")};
	for(i= 0 ; i < nSrcUserFieldCnt ;i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sSrcUserDMAlias, sSrcUserFieldAlias[i], sGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
		ids = pDM->AddDBFieldData(sDstUserFieldAlias[i], sDstUserFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
	}

	CString sLoanType;
	ids = GetLoanType(sLoanType);	
	if(0 > ids) return ids;
	ids = pDM->AddDBFieldData(_T("LOAN_TYPE_CODE"), _T("STRING"), sLoanType, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);

	CString sRecKey;
	ids = pMobileApi->MakeRecKey(sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, strFuncName);

	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		ids = pKLDM->SetCellData(_T("����KEY"), sRecKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1070, strFuncName);
	}
	
	SetTempData(SHARE_CO_LOAN_TBL_INSERT_REC_KEY, CComVariant(sRecKey));

	Send3000ApiSocket(_T("����"), sRecKey);	
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("����/�ݳ�����KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1070, strFuncName);	
	
	CString sReturnExpDate;
	CString sRegCode;
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("��ϱ���"), sRegCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, strFuncName);
	ids = GetReturnExpDate(sReturnExpDate, sRegCode);	
	
	if(ids == 100 || ids == 101) return ids;
	if(0 > ids) return ids;
	
	CString sTempDate;
	if(!sReturnExpDate.IsEmpty())
	{
		sTempDate = _T("TO_DATE('")+sReturnExpDate+_T(" 23:59:59")+_T("', 'YYYY/MM/DD HH24:MI:SS')");
	}
	else
	{
		sTempDate = _T("NULL");
	}
	ids = pDM->AddDBFieldData(_T("RETURN_PLAN_DATE"), _T("NUMERIC"), sTempDate, TRUE);
	
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, strFuncName);

	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		CString strReturnPlanDate = _T("");
		if(!sReturnExpDate.IsEmpty())
		{
			strReturnPlanDate = sReturnExpDate + _T(" 23:59:59");
		}
		ids = pKLDM->SetCellData(_T("�ݳ�������"), strReturnPlanDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1090, strFuncName);
	}
	
	CString sLoanStatus = (FALSE == bIsILLMode) ? _T("0") : _T("L");
	
	ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), sLoanStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1100, strFuncName);
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		ids = pKLDM->SetCellData(_T("����"), sLoanStatus, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1100, strFuncName);
	}
	
	CTime tLoanDate = CTime::GetCurrentTime();
	CString sLoanDate;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("�ұ޴�����"), sLoanDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1110, strFuncName);
	CString strStringLoanDate;
	strStringLoanDate.Format(_T("%s %02d:%02d:%02d"), sLoanDate, tLoanDate.GetHour(), tLoanDate.GetMinute(), tLoanDate.GetSecond());
	CString sInLoanDate;
	sInLoanDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strStringLoanDate);
	ids = pDM->AddDBFieldData(_T("LOAN_DATE"), _T("NUMERIC"), sInLoanDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1110, strFuncName);	
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		ids = pKLDM->SetCellData(_T("������"), strStringLoanDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);
		ids = pKLDM->SetCellData(_T("����������"), strStringLoanDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);
		CString strVolCode;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("IBS_û��_��å��ȣ"), strVolCode, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);
		ids = pKLDM->SetCellData(_T("��å��ȣ"), strVolCode, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);
	}
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("�������"), sLoanStatus, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1130, strFuncName);
	sInLoanDate.Format(_T("%s %02d:%02d"), sLoanDate, tLoanDate.GetHour(), tLoanDate.GetMinute());
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("������"), sInLoanDate, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1130, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("�ݳ�������"), sReturnExpDate, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1130, strFuncName);
	if(sLoanType == _T("0"))
	{
		sLoanType = _T("�Ϲ�");
	}
	else if(sLoanType == _T("1"))	
	{
		sLoanType = _T("Ư��");
	}
	else if(sLoanType == _T("2"))
	{
		sLoanType = _T("����");
	}
	else if(sLoanType == _T("3"))	
	{
		sLoanType = _T("����");
	}
	else if(sLoanType == _T("4"))
	{ 
		sLoanType = _T("���");
	}
	else
	{
		sLoanType = _T("�Ϲ�");
	}
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("��������"), sLoanType, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1140, strFuncName);

	
	CString sIsAppendix;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("�ηϿ���"), sIsAppendix, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1150, strFuncName);
	if(sIsAppendix.Compare(_T("A")) == 0)
	{
		if(m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0)
		{
			pDM->AddDBFieldData(_T("LOAN_KEY"), _T("NUMERIC"), _T("1"));
		}
	}

	if(_T("Y") == m_pManageValue->m_sUseReceipt)
	{
		
		CString strLoanFlag, strReturnFlag;
		if(m_pManageValue->m_sUseReceiptType.GetAt(0) == 'Y') 
		{
			strLoanFlag = _T("N");
		}
		else
		{
			strLoanFlag = _T("C");
		}
		ids = pDM->AddDBFieldData(_T("RECEIPT_PRINT_LOAN_YN"), _T("STRING"), strLoanFlag);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1160, strFuncName);
	}
 	
 	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII-�̵�������"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
		
		ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_DEVICE"), _T("KOLASIII-�̵�������"), 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
	}
	else
	{
		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
		
		ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_DEVICE"), _T("KOLASIII"), 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
	}

	
	ids = pDM->AddDBFieldData(_T("L_WORKER"), _T("STRING"), m_pParentMgr->m_pInfo->USER_ID);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_WORKER"), m_pParentMgr->m_pInfo->USER_ID, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("�������������ȣ"), m_pParentMgr->m_pInfo->LIB_CODE, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);

	
	CString sLog;
	sLog.Format(_T("%s"), m_pParentMgr->GetWorkLogMsg(_T("�弭����"), __WFILE__, __LINE__));
	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), sLog, TRUE);

	
	ids = pMobileApi->MakeInsertFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1180, strFuncName);

	pDM->InitDBFieldData();

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::UpdateBookInfo(CKolas2up_MobileApi *pMobileApi, CString sMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateBookInfo");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();

	CString sTableName;
	CString sWorkingStatus;

	CString sLoanInfoDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("����/���ӱ���"), sTableName, 0);
	
	INT nBookType;
	if(sTableName.Compare(_T("MO"))== 0)
	{
		sTableName = _T("BO_BOOK_TBL");
		ids = GetBOWorkingStatus(sWorkingStatus, _ttoi(sMode));
		if(0 > ids) return ids;

		nBookType = 1;

	}
	else if(sTableName.Compare(_T("SE"))== 0)
	{
		sTableName = _T("SE_BOOK_TBL");
		ids = GetSEWorkingStatus(sWorkingStatus, _ttoi(sMode));
		if(0 > ids) return ids;

		nBookType = 2;
	}
	else if(sTableName.Compare(_T("NB")) == 0)
	{
		sTableName = _T("CO_NON_DB_BOOK_TBL");
		ids = GetBOWorkingStatus(sWorkingStatus, _ttoi(sMode));
		if(0 > ids) return ids;
		nBookType = 0;
	}
	
	CString sRecKey;
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("åKEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

#ifdef __K2UP__
	CString sPreWorkingStatus;
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("����"), sPreWorkingStatus, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CLocCommonAPI::GetWorkingStatusDescToCode(sPreWorkingStatus, 0);

	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), sPreWorkingStatus, TRUE);
#endif
	
	ids = pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), sWorkingStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("����"), __WFILE__, __LINE__), TRUE);
		
	ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	pDM->InitDBFieldData();
	
	CString sSpeciesKey;
	m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("������Ű"), sSpeciesKey, 0);
	AddIdxQueryFrame(pDM, nBookType, sSpeciesKey);
	
	ids = m_pParentMgr->SetDataMgrData(sLoanInfoDMAlias, _T("����"), sWorkingStatus, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -15, strFuncName);

#ifdef __K2UP__
	ids = m_pParentMgr->SetDataMgrData(sLoanInfoDMAlias, _T("�����ڷ����"), sPreWorkingStatus, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -15, strFuncName);
#endif

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateLoanUserUpdate(CKolas2up_MobileApi *pMobileApi,  CString strKLUserYN)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateLoanUserUpdate");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	pDM->InitDBFieldData();
	
	CString sRecKey;
	CString sLoanBookCnt;
	CString sDMUserInfoAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("������KEY"), sRecKey, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("�Ѵ���å��"), sLoanBookCnt, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	sLoanBookCnt.Format(_T("%d"), _ttoi(sLoanBookCnt) + 1);

	CString sIsAppendix;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("�ηϿ���"), sIsAppendix, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	if(sIsAppendix.Compare(_T("A")) == 0 )
	{
		if(m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0)
		{
			sLoanBookCnt.Format(_T("%d"), _ttoi(sLoanBookCnt) - 1);
		}
	}
	
	if(1 > _ttoi(sLoanBookCnt))
	{
		sLoanBookCnt = _T("0");
	}
	ids = pDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sLoanBookCnt, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	CString sLastLoanDate = _T("SYSDATE");
	ids = pDM->AddDBFieldData(_T("LAST_LOAN_DATE "), _T("NUMERIC"), sLastLoanDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("����"), __WFILE__, __LINE__), TRUE);
	
	ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		CESL_DataMgr *pKLDM = m_pParentMgr->FindDM(_T("DM_KL_LOAN"));
		if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		CString strUserNo;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڹ�ȣ"), strUserNo, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pKLDM->SetCellData(_T("���մ����ڹ�ȣ"), strUserNo, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		ids = pKLDM->SetCellData(_T("�ηϿ���"), sIsAppendix, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
	
	CString sUpdateFields = _T("LOAN_COUNT,LAST_LOAN_DATE");
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(sUpdateFields));

	pDM->InitDBFieldData();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddLoanBookDM(CString sDstDMAlias, TCHAR **sDstDMFieldAlias, CString sSrcDMAlias, TCHAR **sSrcDMFieldAlias, INT nFieldCnt)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AddLoanBookDM");	

	CESL_DataMgr *pDstDM = m_pParentMgr->FindDM(sDstDMAlias);
	if(pDstDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_DataMgr *pSrcDM = m_pParentMgr->FindDM(sSrcDMAlias);
	if(pSrcDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sGetData;
	INT nSrcRowCount = pSrcDM->GetRowCount();
	for(INT i = 0 ; i < nSrcRowCount ; i++)
	{
		ids = pDstDM->InsertRow(-1);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		for(INT j = 0 ; j < nFieldCnt ; j++)
		{
			if(!(m_pManageValue->m_bVolReserve) && nFieldCnt-2<j)
				break;

			sGetData.Empty();

			ids = pSrcDM->GetCellData(sSrcDMFieldAlias[j], i, sGetData);
			if(0 > ids&& (ids !=-4012) && (ids!=-111)) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if((sGetData == -111) || (ids == -4012)) sGetData = _T("");

			ids = pDstDM->SetCellData(sDstDMFieldAlias[j], sGetData, pDstDM->GetRowCount()-1); 
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetLoanType(CString &sLoanType)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetLoanType");

	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("��������"), sLoanType);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	if(sLoanType.Compare(_T("3")) == 0)
		sLoanType = _T("4");

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetBOWorkingStatus(CString &sWorkingStatus, INT nMode/*=0*/)
{
EFS_BEGIN

	INT ids;

	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;
	
	if(sLoanType.Compare(_T("0"))==0)
	{
		sWorkingStatus = _T("BOL211O");
	}
	
	else if(sLoanType.Compare(_T("1"))==0)
	{
		sWorkingStatus = _T("BOL213O");
	}
	
	else if(sLoanType.Compare(_T("2"))==0)
	{
		sWorkingStatus = _T("BOL212O");
	}
	
	else if(sLoanType.Compare(_T("4"))==0)
	{
		sWorkingStatus = _T("BOL217O");
	}

	if(nMode == 1)
		sWorkingStatus = _T("BOL214O");

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetSEWorkingStatus(CString &sWorkingStatus, INT nMode/*=0*/)
{
EFS_BEGIN

	INT ids;

	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;

	if(sLoanType.Compare(_T("0"))==0)
	{
		sWorkingStatus = _T("SEL311O");
	}
	
	else if(sLoanType.Compare(_T("1"))==0)
	{
		sWorkingStatus = _T("SEL313O");
	}
	
	else if(sLoanType.Compare(_T("2"))==0)
	{
		sWorkingStatus = _T("SEL312O");
	}
	
	else if(sLoanType.Compare(_T("4"))==0)
	{
		sWorkingStatus = _T("SEL317O");
	}

	if(nMode == 1)
		sWorkingStatus = _T("SEL314O");

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddMemberInfo(BOOL bReturn/*=FALSE*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AddMemberInfo");

	if(m_pManageValue->m_sUnityLoanCntYN == _T("Y"))
	{ 
		ids = SetBookStatusCnt_Unity();
		if(0 > ids) return ids;
	}
	else
	{ 
		ids = SetBookStatusCnt();
		if(0 > ids) return ids;
	}
	
	ids = SettingDelayDay(bReturn);
	if(0 > ids) return ids;
	
	CString sMemberState;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("ȸ������"), sMemberState, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	if(sMemberState.Compare(_T("0")) == 0)
		sMemberState = _T("����");
	else if(sMemberState.Compare(_T("1")) == 0)
		sMemberState = _T("��������");
	else if(sMemberState.Compare(_T("2")) == 0)
		sMemberState = _T("����");
	else if(sMemberState.Compare(_T("3")) == 0)
		sMemberState = _T("Ż��");

	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("ȸ�����¼���"), sMemberState, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString strQuery;
	CString strResult;
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString strUserKey;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("������KEY"), strUserKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	if(!strUserKey.IsEmpty())
	{
		strQuery.Format(
			_T("SELECT ")
			_T("((SELECT NVL(SUM(ARREAR),0) FROM LS_WORK_T01 CL WHERE CL.STATUS IN ('1','R') AND CL.USER_KEY=%s)")
			_T("+(SELECT NVL(SUM(ARREAR),0) FROM CO_LOAN_TBL CL WHERE CL.STATUS IN ('1','R') AND CL.USER_KEY=%s)) ")
			_T("FROM DUAL"), strUserKey, strUserKey);
		ids = pDM->GetOneValueQuery(strQuery, strResult);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�ѿ�ü��"), strResult, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	
	INT nReservationCancelLimitCnt;
	ids = m_pCheckLoan->GetReservationCancelLimitCnt(nReservationCancelLimitCnt);
	if(0 > ids) return ids;

	INT nReservationCancelLimitPeriod;
	ids = m_pCheckLoan->GetReservationCancelLimitPeriod(nReservationCancelLimitPeriod);
	if(0 > ids) return ids;
	
	
	if(0 < nReservationCancelLimitCnt && 0 < nReservationCancelLimitPeriod)
	{	
		if(!strUserKey.IsEmpty())
		{
			strQuery.Format(_T("SELECT (SELECT COUNT(1) FROM LS_WORK_T01 ")
							_T("WHERE STATUS='4' AND USER_KEY=%s AND RESERVATION_CANCEL_TYPE='0' ")
							_T("AND RESERVE_CANCEL_DATE ")
							_T("BETWEEN TO_DATE(SYSDATE-%d,'YYYY/MM/DD') ")
							_T("AND TO_DATE(SYSDATE,'YYYY/MM/DD')+0.99999)")
							_T("+")
							_T("(SELECT COUNT(1) FROM CO_LOAN_TBL ")
							_T("WHERE STATUS='4' AND USER_KEY=%s AND RESERVATION_CANCEL_TYPE='0' ")
							_T("AND RESERVE_CANCEL_DATE ")
							_T("BETWEEN TO_DATE(SYSDATE-%d,'YYYY/MM/DD') ")
							_T("AND TO_DATE(SYSDATE,'YYYY/MM/DD')+0.99999) FROM DUAL")
							, strUserKey, nReservationCancelLimitPeriod
							, strUserKey, nReservationCancelLimitPeriod);
			
			ids = pDM->GetOneValueQuery(strQuery, strResult);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
					
			ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("������Ұ���"), strResult, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(_ttoi(strResult) >= nReservationCancelLimitCnt)
			{
				ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���డ������"), _T("X"), 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			}
		}
	}
		
	CESL_Mgr* pMgr = m_pParentMgr->FindSM(_T("BO_LOC_3100"));
	if(NULL != pMgr)
	{
		pMgr->SendMessage(WM_SHOWWINDOW, SW_SHOW, 0);
		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			CString strGetData;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����ȸ������"), strGetData, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			if(_T("Y") == strGetData)
			{
				pMgr->SendMessage(WM_UNITY_USER_ICON_SHOW, (WPARAM)TRUE, 0);
			}
			else
			{
				pMgr->SendMessage(WM_UNITY_USER_ICON_SHOW, (WPARAM)FALSE, 0);
			}
		}
	
		CString strIpinFlag;
		GetTempData(_T("�����ɻ�뿩��"), strIpinFlag);
		CString strMypinFlag;
		GetTempData(_T("MYPIN��뿩��"), strMypinFlag);
		CString strPhoneCertifyFlag;
		GetTempData(_T("�޴���������뿩��"), strPhoneCertifyFlag);
		if(TRUE == m_pParentMgr->m_pInfo->m_bCertifyUse || strIpinFlag.CompareNoCase(_T("Y")) == 0 || strMypinFlag.CompareNoCase(_T("Y")) == 0 || strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
		{
			pMgr->SendMessage(WM_CERTIFY_ICON_SHOW, 0, 0);
		}	
		
		pMgr->SendMessage(WM_USER_INFO_RENEWAL, (WPARAM)TRUE, 0);
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse || TRUE == m_pParentMgr->m_pInfo->m_bAgreeInfoRecordYN)
		{
			CString strIsKLmemberYN;
			CString strAgentName;
			CString strBirth;
			CString strCertCode = _T("");
			CString strStatementAlias;
			CString strCertifyCode;
			INT nCode = 0;  
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����"), strBirth, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1021, strFuncName);
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����ȸ������"), strIsKLmemberYN, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1022, strFuncName);

			CString strGetData;
	
			CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);

			CString sCurrentTime = strGetData.Left(10);
			BOOL bIsUnder14YN = FALSE;
			if(10 == strBirth.GetLength())
			{
				INT nBirthYear = _ttoi(strBirth.Left(4));
				INT nBirthMonth = _ttoi(strBirth.Mid(5, 2));
				INT nBirthDay = _ttoi(strBirth.Right(2));
				INT nCurrentYear = _ttoi(sCurrentTime.Left(4));
				INT nCurrentMonth = _ttoi(sCurrentTime.Mid(5, 2));
				INT nCurrentDay = _ttoi(sCurrentTime.Right(2));
				if(14 > (nCurrentYear-nBirthYear))
				{
					bIsUnder14YN = TRUE;
				}
				else if(14 == (nCurrentYear-nBirthYear) && 0 > (nCurrentMonth-nBirthMonth))
				{
					bIsUnder14YN = TRUE;	
				}	
				else if(14 == (nCurrentYear-nBirthYear) && 0 == (nCurrentMonth-nBirthMonth) && 0 > (nCurrentDay-nBirthDay))
				{
					bIsUnder14YN = TRUE;	
				}	
				else
				{
					bIsUnder14YN = FALSE;
				}
			}

			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���Ǿ��"), strStatementAlias, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1025, strFuncName);
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�븮�θ�"), strAgentName, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1026, strFuncName);
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�Ǹ������ڵ�"), strCertifyCode, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1027, strFuncName);
		
			if(TRUE == m_pParentMgr->m_pInfo->m_bAgreeInfoRecordYN && _T("Y") != strIsKLmemberYN)
			{				
				if(bIsUnder14YN)
				{
					if(_T("�������� ����/�̿�") != strStatementAlias || _T("") == strAgentName)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}				
				else
				{
					if(_T("�������� ����/�̿�") != strStatementAlias)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}
				
				if(m_pManageValue->m_pInfo->m_bCertifyUse && _T("") != strCertifyCode)
				{
					pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
				}
			}
			else if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strIsKLmemberYN)
			{
				if(bIsUnder14YN)
				{
					if(_T("å��������") != strStatementAlias || _T("") == strAgentName)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}
				else
				{
					if(_T("å��������") != strStatementAlias)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}
				
				if(m_pManageValue->m_pInfo->m_bCertifyUse && _T("") != strCertifyCode)
				{
					pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
				}
			}
			else if(TRUE == m_pParentMgr->m_pInfo->m_bAgreeInfoRecordYN && _T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") != strIsKLmemberYN)
			{
				
				if(bIsUnder14YN)
				{
					if(_T("�������� ����/�̿�") != strStatementAlias || _T("") == strAgentName)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}
				if(bIsUnder14YN)
				{
					if(_T("�������� ����/�̿�") != strStatementAlias)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}
				if(m_pManageValue->m_pInfo->m_bCertifyUse && _T("") != strCertifyCode)
				{
					pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
				}
			}
			else
			{
				pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)0, 0);
			}
		}
	}

	return 0;
 
EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::UpdateSuccessDM(INT nMode /* = 0*/, BOOL bIsILL)
{
EFS_BEGIN

	INT ids = 0;

	ids = UpdateLoanBookInfoDM(nMode);
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateUserInfoDM(INT nMode /*= 0*/, BOOL bIsILL)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateUserInfoDM");

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	
	CString sBookCnt;

	if((m_pManageValue->m_bVolReserve))
	{
		TestCode();
		return 0;
	}

	if(nMode == 0)	
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("�Ѵ���å��"), sBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
		CString sIsAppendix;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("�ηϿ���"), sIsAppendix, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(sIsAppendix.Compare(_T("A")) == 0)
		{
			if(m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0)
			{
				if(FALSE == bIsILL)
				{
					INT nBookCnt = _ttoi(sBookCnt);		
					nBookCnt = (0 < nBookCnt) ? nBookCnt-1 : nBookCnt;
					sBookCnt.Format(_T("%d"), nBookCnt);
				}
				
			}
		}

		sBookCnt.Format(_T("%d"), _ttoi(sBookCnt)+1);
		
		if(bIsILL == TRUE)
			ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("Ÿ���Ѵ���å��"), sBookCnt, 0);
		else
		ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("�Ѵ���å��"), sBookCnt, 0);		
		
		if(_T("Y") == m_pManageValue->m_sIsUnionLoanCnt)
		{
			CString strUnionCnt;

			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("���մ���å��"), strUnionCnt, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(!strUnionCnt.IsEmpty())
			{
				if(0 == sIsAppendix.Compare(_T("A")))
				{
					if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt())
					{
						strUnionCnt.Format(_T("%d"), _ttoi(strUnionCnt)+1);
					}
				}

				strUnionCnt.Format(_T("%d"), _ttoi(strUnionCnt)-1);


			}
		}	
		
	}
	else if(nMode == 1) 
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("�ѿ���å��"), sBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		sBookCnt.Format(_T("%d"), _ttoi(sBookCnt)+1);
		ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("�ѿ���å��"), sBookCnt, 0);
	}
	else if(nMode == 2)	
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("�ѿ���å��"), sBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
		INT nBookCnt = _ttoi(sBookCnt);		
		nBookCnt = (0 < nBookCnt) ? nBookCnt-1 : nBookCnt;
		sBookCnt.Format(_T("%d"), nBookCnt);
		
		
		ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("�ѿ���å��"), sBookCnt, 0);
	}
	else if(nMode == 3)  
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("�ѿ���å��"), sBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		INT nBookCnt = _ttoi(sBookCnt);		
		nBookCnt = (0 < nBookCnt) ? nBookCnt-1 : nBookCnt;
		sBookCnt.Format(_T("%d"), nBookCnt);
		
		ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("�ѿ���å��"), sBookCnt, 0);

		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("�Ѵ���å��"), sBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		sBookCnt.Format(_T("%d"), _ttoi(sBookCnt)+1);
		ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("�Ѵ���å��"), sBookCnt, 0);

		
		
		if(_T("Y") == m_pManageValue->m_sIsUnionLoanCnt)
		{
			CString strUnionCnt;

			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("���մ���å��"), strUnionCnt, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(!strUnionCnt.IsEmpty())
			{
				strUnionCnt.Format(_T("%d"), _ttoi(strUnionCnt)-1);


			}
		}			
	}
	else
	{
		ids = 0;
	}

	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	return 0;	

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::UpdateLoanBookInfoDM(INT nMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateLoanBookInfoDM");

	CString sDstDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sSrcDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	
	CESL_DataMgr *pSrcDM = m_pParentMgr->FindDM(sSrcDMAlias);
	if(pSrcDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	const INT nCopyColumnCnt = 42;
	TCHAR *sSrcFieldAlias[nCopyColumnCnt] = 
	{
		_T("����/�ݳ�����KEY"), _T("������Ű"), _T("åKEY"),_T("��Ϲ�ȣ"),_T("MARC"),
		_T("������"),_T("�ݳ�������"),_T("������"),_T("���ุ����"),_T("��ü�ϼ�"),
		_T("�������"),_T("�ڷ�Ǳ���"),_T("����/���ӱ���"), 
		_T("IBS_û��_��ġ��ȣ_�ڵ�"),	
		_T("IBS_û��_�з���ȣ"),		
		_T("IBS_û��_������ȣ"),		
		_T("IBS_û��_��å��ȣ"),		
		_T("IBS_û��_������ȣ"),		
		_T("å��"), 
		_T("��ǥ��"),
		_T("�η��ڷ��"), 
		_T("�ݳ�����Ƚ��"),
		_T("�����ڷ����"),
		_T("����"),
		_T("��ȣ��"),
		_T("��������"), 
		_T("�ηϿ���"), 
		_T("�����ڷ����"),
		_T("�������"),
		_T("RFID�ø���"),
		_T("��ü����"),
		_T("��������"),
		_T("��ȣ����KEY"),
		_T("�����̿��ڼ�"),		
		_T("L_DEVICE"),
		_T("R_DEVICE"),		
		_T("L_WORKER"),
		_T("R_WORKER"),
		_T("û����ȣ"),		
		_T("������"),
		_T("�������������ȣ"),
		_T("���⵵����")
	};

	ids = AddLoanInfo(sSrcFieldAlias, pSrcDM);
	if(0 > ids) return ids;
	
	if(0 != nMode)
	{
		ids = MakeShelfCode(sDstDMAlias);
		if(0 > ids)
		{
			return ids;
		}
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::NoDBLoan(CString sBookRecKey/*=_T("")*/)
{
EFS_BEGIN

	INT ids;

	ids = IsNoDBLoan();
	if(0 > ids) return ids;
	if(ids > 0) return 1;
	
	if(sBookRecKey.IsEmpty())
	{
		return 0;
	}

	ids = NoDBLoanDBProc(sBookRecKey);
	if(0 > ids) return ids;
	if(ids > 0) return ids;

	m_bReturnBookNonFree = TRUE;
	ids = SearchLoanBookInfo();
	if(0 > ids) return ids;
	
	ids = AddMemberInfo();
	if(0 > ids) return ids;
	
	ids = ViewLoanScreen();
	if(0 > ids) return ids;
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::NoDBLoanDBProc(CString &sBookRecKey)
{
EFS_BEGIN

	INT ids;
	ids = Loan(sBookRecKey, 1);
	if(0 > ids) return ids;
	if(ids > 0) 
	{
		InitExpDM();
		
		return ids;
	}
	
	ids = InitExpDM();
	if(0 > ids) return ids;
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsNoDBLoan()
{	
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsNoDBLoan");
	
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sMemberDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(pDM->GetRowCount() < 1) 
	{
		SelfCloseMessageBox(_T("�����ڸ� ���� �������ֽʽÿ�"));
		return 1;
	}

	BOOL IsLoan = false;
	
	ids = IsLoanMember(IsLoan);
	if(0 > ids) return ids;

	if(!IsLoan) return 1;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReturnBook(BOOL bShowWindow/*=TRUE*/,BOOL bAllReturn/*=FALSE*/, BOOL bIsILL)
{	
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReturnBook");
	
	StartLog(_T("�ݳ�:�̿��ڰ˻����ΰ˻�"));
	ids = IsSearchedUser();
	EndLog();
	if(0 > ids) return ids;
	if(ids > 0) return 0;
	
	ids = IsSearchedLoanInfo();
	if(0 > ids) return -1;
	if(ids > 0) return 0;

	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		CESL_Grid* pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"),_T("MainGrid"));
		if(NULL != pMainGrid)
		{
			pMainGrid->SelectMakeList();
			INT nIdx = pMainGrid->SelectGetHeadPosition();
			if(pMainGrid->SelectGetCount())
			{
				do
				{
					CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));

					CString sPaymentKey, sPaymentKey2;
					sPaymentKey = pDM->GetCellData(_T("����ó��KEY"), nIdx);
					if(!sPaymentKey.IsEmpty())
					{
						for(INT j = nIdx+1; j < pDM->GetRowCount(); j++)
						{
							sPaymentKey2 = pDM->GetCellData(_T("����ó��KEY"), j);
							if(sPaymentKey == sPaymentKey2)
							{
								pMainGrid->SetAt(j, pMainGrid->m_nSelectIdx, _T(""));
							}
						}
					}
					nIdx = pMainGrid->SelectGetNext();
				}while(nIdx >= 0);
			}
		}
	}

	if(_T("Y") == m_pManageValue->m_sIsUnionLoanCnt && FALSE == bAllReturn)
	{
		CString sUnionCnt;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���մ���å��"), sUnionCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(sUnionCnt.IsEmpty())
		{
			SelfCloseMessageBox(_T("���մ��������� ����ȭ ���Դϴ�."));
			return 0;
		}
	}	
	
	INT nMode = InitReturnMode();
	if(nMode < 0) return ids;

	INT IDD_MODE;

	if(nMode == 0)
		IDD_MODE = IDD_BO_LOC_3130;
	else if(nMode == 1)
		IDD_MODE = IDD_BO_LOC_3131;
	else if(nMode == 2)
		IDD_MODE = IDD_BO_LOC_3132;
	
	if(m_pManageValue->m_sFeeMngSys == _T("Y") && FALSE == bIsILL)
 	{
 		IDD_MODE = IDD_BO_LOC_3130_2;
 	}
	
	if(_T("Y") == m_pManageValue->m_ArrearCheckYN)
	{
		IDD_MODE = IDD_BO_LOC_3130_3;
	}
	CopyGridCheckToDM();
	EndLog();

	BOOL bLastWork;
	BOOL bIsReturnCancel;	
	
	CString strQuery, strValue;

	strQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL ")
				_T("WHERE CLASS_ALIAS='å�η��ý���' AND VALUE_NAME='å�η��ݳ�Ȯ��' ")
				_T("AND MANAGE_CODE='%s'"), m_pParentMgr->m_pInfo->MANAGE_CODE);
				
	CESL_DataMgr pDM;
	pDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	pDM.GetOneValueQuery(strQuery, strValue);
	
	StartLog(_T("�ݳ�:�ݳ�����"));
	CBO_LOC_3130 ReturnDlg(m_pParentMgr, IDD_MODE, this, bShowWindow, bIsILL);
	
	ReturnDlg.m_n3130ILLReturnProcCnt = 0;

	if(strValue == _T("Y"))
		ReturnDlg.ILL_ALERT = TRUE;
		ReturnDlg.m_AllReturn = bAllReturn;				

	ReturnDlg.DoModal();
	EndLog();

	bIsReturnCancel = ReturnDlg.m_bIsReturnCancel;		
	bLastWork = ReturnDlg.m_bLastWork;

	
	if(TRUE == bIsReturnCancel) return 1;
	
	if ( ReturnDlg.m_n3130ILLReturnProcCnt > 0  ) {
		m_nILLReturnProcCnt = m_nILLReturnProcCnt + ReturnDlg.m_n3130ILLReturnProcCnt;
	}

	if(m_bSimpleGrid == FALSE || m_bSimpleCmd == FALSE)
	{
		StartLog(_T("�ݳ�:�׸�����������Ʈ�Ѱ���"));
		ids = AllControlDisplayNotGrid(1);
		EndLog();
		if(0 > ids) return ids;
	}	
	
	if(m_ReceiptDlg != NULL) 
	{		
		m_pParentMgr->PostMessage(WM_RECEIPT_CHANGE_TAB, 0, 2);
	}
	
	if(m_bSimpleGrid == FALSE || m_bSimpleCmd == FALSE)
	{
		StartLog(_T("�ݳ�:�׸��尻��"));
		InitGridColor();
		ViewGrid();	
		EndLog();
	}

	if(!bLastWork)
		return 1;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitReturnMode()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("InitReturnMode");

	CString sReturnType;

	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("�ݳ�����"), sReturnType);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(sReturnType.Compare(_T("0"))==0)
	{
		return 0;
	}
	else if(sReturnType.Compare(_T("1"))==0)
	{
		return 1;
	}
	else if(sReturnType.Compare(_T("2"))==0)
	{
		return 2;
	}
	else 
	{
		return -1;
	}

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::Reservation()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("Reservation");
	
	CString sValue;
	ids = m_pCheckLoan->GetReserveMode(sValue);
	if(0 > ids) return ids;

	if(_ttoi(sValue) == 0)
	{
		ViewMessage(_T("���� ����� ��� �Ұ����� �����Դϴ�. �������� �����Ͻʽÿ�"));
		return 0;
	}
	
	BOOL IsReserve;
	ids = IsReserveMember(IsReserve);
	if(0 > ids) return ids;

	if(!IsReserve) return 0;

	CString strData;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���డ������"), strData, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(_T("X") == strData)
	{
		INT nReservationCancelLimitPeriod;
		ids = m_pCheckLoan->GetReservationCancelLimitPeriod(nReservationCancelLimitPeriod);
		if(0 > ids) return ids;

		CString strReservationCancelCnt;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("������Ұ���"), strReservationCancelCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		CString strMsg;
		strMsg.Format(_T("������� �Ǽ��� '%d'�� ���� '%s'ȸ �Ͽ��� ������ ������ �� �� �����ϴ�"), nReservationCancelLimitPeriod, strReservationCancelCnt);
		SelfCloseMessageBox(strMsg);
		return 0;
	}
	
	CString sRegNo;
	ids = GetRegNo(sRegNo);
	if(0 > ids) return ids;
	if(ids > 0) return 0;

	ids = SearchBookListSelect(sRegNo);
	if(0 > ids) return ids;
	if(ids > 0) 
	{
		InitExpDM();
		return 0;
	}
	
	ids = IsReservation();
	INT nMode = ids;
	if(0 > ids) return ids;
	
	CString sStatus, sBookKey;
	ids = IsBookRelationCurrentUser(sStatus,sBookKey);
	if(0 > ids) return ids;
	if(ids > 0)
	{
		SelfCloseMessageBox(_T("�� å�� ���� �����ڰ� �̿����� å�Դϴ�. �����Ͻ� �� �����ϴ�."));
		InitExpDM();
		return 0; 
	}
	
	ids = IsUserBookLoan(EXP_RESERVE);
	if(0 > ids) return ids;
	if(ids > 0)
	{
		InitExpDM();
		return 0;
	}

	ids = ViewReservtionInfo(nMode);
	if(0 > ids) return ids;
	if(ids > 0) 
	{
		if(ids != 2) SelfCloseMessageBox(_T("��ҵǾ����ϴ�."));
		InitExpDM();
		return 0;
	}

	ids = IsReLoanBook();
	if(0 > ids) return ids;
	if(ids > 0)
	{
		if(m_pCheckLoan->IsViewReLoanHistory() == 0)
		{
			ids = ViewLoanHistory(ids,1);
			if(ids == 1)
			{
				InitExpDM();

				return 0;
			}
		}
		else
		{
			if(ids == 11 || ids == 22 || ids == 33)
			{
				CString sMsg;
				if(ids == 11) sMsg.Format(_T("������ ���� �Ǿ��� å�Դϴ�. �����Ͻ� �� �����ϴ�. "));
				if(ids == 22) sMsg.Format(_T("������ �Ѹ��� ������ ���� �߾��� å�Դϴ�. �����Ͻ� �� �����ϴ�. "));
				if(ids == 33) sMsg.Format(_T("������ �Ѹ��� ������ å�Դϴ�. ���� �Ͻ� �� �����ϴ�."));
				SelfCloseMessageBox(sMsg);
				InitExpDM();

				return 0;
			}			
		}
	}

	this->SetBEGIN_SBL();
	
	ids = ReservationBook();
	if(1001==ids) return ids;
	else if(0 > ids) return ids;
	
	ids = UpdateSuccessDM(1);
	if(0 > ids) return ids;
	
	ids = AddMemberInfo();
	if(0 > ids) return ids;

	ids = ViewLoanScreen(3);
	if(ids != 0) return ids;

	SelfCloseMessageBox(_T("����Ǿ����ϴ�."));

	InitExpDM();
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	INT nRowCount = pDM->GetRowCount();
	this->SetEND_SBL(nRowCount);
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchBookListSelect(CString &sRegNo)
{
EFS_BEGIN

	INT ids;

	INT nBookCnt = 0;

	InitExpDM();

	ids = SearchBookInfo(sRegNo, nBookCnt);
	if(0 > ids) return ids;

	if(nBookCnt == 0) 
	{
		SelfCloseMessageBox(_T("�ڷᰡ �������� �ʽ��ϴ�. "));
		return 1;
	}
	if(nBookCnt > 1)
	{
		CLoanBookListDlg BookList(m_pParentMgr);
		BookList.DoModal();
		
		if(BookList.GetCloseStatus() == 0)
		{
			SelfCloseMessageBox(_T("��ҵǾ����ϴ�."));
			return 1;
		}

	}
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetRegNo(CString &sRegNo)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetRegNo");

	CString sParentCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	CString sRegFieldAlias = _T("�����ڹ�ȣ");

	ids = MakeRegNoSpaceToZero(TRUE);
	if(0 > ids) return ids;

	ids = m_pParentMgr->GetControlData(sParentCMAlias, sRegFieldAlias, sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(sRegNo.IsEmpty())
	{
		SelfCloseMessageBox(_T("������ å�� ��Ϲ�ȣ�� �Է��Ͻʽÿ�."));
		return 1;
	}
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsReservation()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsReservation");

	CString sWorkingStatus;

	CString sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CString sFieldAlias = _T("����");

	CString sReserveMode;
	ids = m_pCheckLoan->GetReserveMode(sReserveMode);
	if(0 > ids) return ids;

	ids = m_pParentMgr->GetDataMgrData(sDMAlias, sFieldAlias, sWorkingStatus, 0);
	if(0 > ids) return ids;

#ifdef __K2UP__
	if(
		(sWorkingStatus.Compare(_T("BOL211O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL212O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL213O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL214O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL215O")) == 0)  	
 	||	(sWorkingStatus.Compare(_T("BOL411O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL511O")) == 0) 	
	||	(sWorkingStatus.Compare(_T("BOL611O")) == 0) 	
	||	(sWorkingStatus.Compare(_T("BOL217O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL317O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL218O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL219O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL318O")) == 0) 	
	||	(sWorkingStatus.Compare(_T("SEL311O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL312O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL313O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL314O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL315O")) == 0) 	
 	||	(sWorkingStatus.Compare(_T("SEL411O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL511O")) == 0) 	
	)
#else
	if(
	(sWorkingStatus.Compare(_T("BOL221O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL222O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL223O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL224O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL225O")) == 0) 	
 	||	(sWorkingStatus.Compare(_T("BOL411O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL511O")) == 0) 	
	||	(sWorkingStatus.Compare(_T("BOL611O")) == 0)  	
	||	(sWorkingStatus.Compare(_T("SEL311O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL312O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL313O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL314O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL315O")) == 0) 		
 	||	(sWorkingStatus.Compare(_T("SEL411O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL511O")) == 0)  	
	)		
#endif
	{
		return 2;
	}
	else
	{
		if(_ttoi(sReserveMode) == 2)
		{
			if(sWorkingStatus.Compare(_T("BOL112N"))==0 || 
				(sWorkingStatus.Compare(_T("SEL212N"))==0) ||
				(sWorkingStatus.Compare(_T("SEL112N"))==0)
				)
				return 2;
		}		
		else if(1 == _ttoi(sReserveMode))
		{
			if(_T("BOL112N") == sWorkingStatus || 
				_T("SEL212N") == sWorkingStatus ||
				_T("SEL112N") == sWorkingStatus)
			{
				CString strBookKey;
				ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("åKEY"), strBookKey, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
				CString strQuery, strReserveCnt;
				strQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS='3'"), strBookKey);
				CESL_DataMgr TmpDM;
				TmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
				ids = TmpDM.GetOneValueQuery(strQuery, strReserveCnt);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
				if(0 < _ttoi(strReserveCnt))
				{
					return 2;
				}
			}
		}

		return 1;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewReservtionInfo(INT nMode)
{
EFS_BEGIN

	INT ids = 0;
	CString strFuncName = _T("ViewReservtionInfo");

	INT nReserveUserCnt = 0;
	INT nCanReserveBookCnt = 1;
	CString sBookKey = _T("");
	CString sSpeciesKey = _T("");
	CString sVolInfoKey = _T("");
	CString sPublishFormCode = _T("");
	CString sWhere = _T("");
	CString sReserveUserCnt = _T("");
	CString sBookCnt = _T("");
	CString sBookCntSql = _T("");
	CString sReservationDMAlias = _T("DM_RESERVATION_INFO");

	CESL_DataMgr *pReservationDM = m_pParentMgr->FindDM(sReservationDMAlias);
	if(pReservationDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("åKEY"), sBookKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if(!(m_pManageValue->m_bVolReserve))
	{
		sWhere.Format(_T("CL.BOOK_KEY=%s AND CL.STATUS='3' "), sBookKey);	
	}
	else
	{
		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("������Ű"), sSpeciesKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("��ȣ����KEY"), sVolInfoKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("����/���ӱ���"), sPublishFormCode, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(sPublishFormCode == _T("NB"))
		{
			return 0;	
		}

		if(sVolInfoKey.IsEmpty())
		{
			sWhere.Format(	_T("CL.BOOK_KEY=%s AND CL.STATUS='3' AND CL.VOL_INFO IS NULL"), sBookKey);
		}
		else
		{
			sWhere.Format(	_T("CL.SPECIES_KEY=%s AND CL.VOL_INFO=%s AND CL.STATUS='3' AND CL.PUBLISH_FORM_CODE='%s' "), 
							sSpeciesKey, sVolInfoKey, sPublishFormCode);
		}
	}	

	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pReservationDM, sWhere);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	nReserveUserCnt = pReservationDM->GetRowCount();

	ids = m_pCheckLoan->GetReserveUserCnt(sReserveUserCnt);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if((_ttoi(sReserveUserCnt)*nCanReserveBookCnt) < (nReserveUserCnt+1))
	{
		ViewMessage(_T("���ڷῡ ���� ���� ��� �ο��� �ʰ��Ͽ� ������ �Ұ����մϴ�."));
		return 2;
	}
	
	CString strValue1;
	ids = m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), strValue1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();
		
	CString strEnc;
	CStringArray asDes1,asDes2;

	INT i;
	CString strUserNo;
	for(i = 0; i < nReserveUserCnt; i++)
	{
		
		if(_T("Y") == strValue1 && FALSE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			break;
		}
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			pReservationDM->GetCellData(_T("�ڵ���"), i, strEnc);
			asDes1.Add(strEnc);
			pReservationDM->GetCellData(_T("��ȭ��ȣ"), i, strEnc);
			asDes2.Add(strEnc);
		}

		if(_T("Y") != strValue1)
		{
			ids = pReservationDM->GetCellData(_T("�����ڹ�ȣ"), i, strUserNo);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pManageValue->m_sIsUnityLoanService );
			ids = pReservationDM->SetCellData(_T("�����ڹ�ȣ"), strUserNo, i);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
	}

	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && 0 < nReserveUserCnt)
	{
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != pApi)
		{
			if(TRUE == pApi->ConnectSocket()) 
			{
				pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes1);
				pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes2);
				for(i = 0; i < nReserveUserCnt; i++)
				{
					pReservationDM->SetCellData(_T("�ڵ���"), asDes1.GetAt(i), i);
					pReservationDM->SetCellData(_T("��ȭ��ȣ"), asDes2.GetAt(i), i);
				}
				pApi->CloseSocket();
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	SetTempData(_T("�������"), _T("0"));

	if(pReservationDM->GetRowCount() > 0)
	{
		CReservationInfoDlg List(m_pParentMgr);
		List.DoModal();
		if(List.GetCloseMode() == 0) 
		{
			return 1;
		}

		SetTempData(_T("�������"), _T("1"));
	}
	
	
	else if(nMode == 1 && m_IsReqPrior == FALSE )
	{
		CString sMode;
		m_pCheckLoan->GetReserveMode(sMode);

		if(_ttoi(sMode) == 1)
		{
			ViewMessage(_T("�������� �ڷḸ ������ �����մϴ�."));
		}
		else 
		{
			ViewMessage(_T("��ġ���̰ų� �������� �ڷḸ ������ �����մϴ�."));
		}
		return 2;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationBook()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReservationBook");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CKolas2up_MobileApi *pMobileApi;
	pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

	ids = ReservationInfoDBUpdate(pMobileApi);
	if(1001 == ids) return ids;
	else if(0 > ids) return ids;
	
	ids = ReservationUserInfoDBUpdate(pMobileApi);
	if(0 > ids) return ids;
	
	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	pMobileApi->EndFrame();
	
	CLocCommonAPI::MobileApiEndLog(pMobileApi);
	
	ShareInfoCenterUpload(_T("I"), _T("U"));
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationInfoDBUpdate(CKolas2up_MobileApi *pMobileApi, CString strKLUserYN/*=_T("N")*/)
{
EFS_BEGIN

	INT ids = 0;
	CString strFuncName = _T("ReservationInfoDBUpdate");

	const INT nSrcFieldCnt = 15;
	const INT nSrcUserFieldCnt = 4;
	const INT nDeleteCnt = 10;
	
	INT nLinkYN = IDNO;
	INT nExpireDay = 0;
	CString sGetData = _T("");

	CString sPublishFormCode = _T("");
	CString sSpecieskey = _T("");
	CString sVolinfo = _T("");
	CString sMoSeDelim = _T("");
	CString sALineQuery = _T("");

	CString sHoldCnt = _T("");
	CString sRecKey = _T("");
	CString sLoanStatus = _T("3");
	CString sWorkingStatus = _T("");
	CString sReservationOrder = _T("");
	CString sExpireDate = _T("");
	CString sReservationDate = _T("");
	CString sLog = _T("");
	
	CString sDMAlias = _T("DM_BO_LOC_3100_DB");	
	CString sSrcDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CString sSrcUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	
	
	TCHAR* sSrcFieldAlias[nSrcFieldCnt] = 
	{
		_T("åKEY")			, _T("����/���ӱ���")	, _T("������Ű")	, _T("��ǥ��")	, _T("������")	,
		_T("��ȣ����KEY")	, _T("��������")		, _T("������")     , _T("�ڷ�Ǳ���"), _T("�з���ȣ"),
		_T("��ϱ���")		, _T("��Ϲ�ȣ")		, _T("��ġ��ȣ")	, _T("�з���ȣ����"), _T("û����ȣ")
	};
	TCHAR* sDstFieldAlias[nSrcFieldCnt] = 
	{
		_T("BOOK_KEY"), _T("PUBLISH_FORM_CODE")	, _T("SPECIES_KEY")			, _T("TITLE")		, _T("AUTHOR")			,
		_T("VOL_INFO"), _T("MANAGE_CODE")		, _T("PUBLISHER")		,     _T("SHELF_LOC_CODE")	, _T("CLASS_NO")	,
		_T("REG_CODE"), _T("REG_NO")			, _T("SEPARATE_SHELF_CODE")	, _T("CLASS_NO_TYPE")	, _T("CALL_NO")
	};
	TCHAR *sDstFieldTypeAlias[nSrcFieldCnt] = 
	{
		_T("NUMERIC")	, _T("STRING"), _T("NUMERIC")	, _T("STRING"), _T("STRING"),
		_T("NUMERIC")	, _T("STRING"), _T("STRING")	, _T("STRING"), _T("STRING"),
		_T("STRING")	, _T("STRING"), _T("STRING")	, _T("STRING"), _T("STRING") 
	};
	
	
	TCHAR *sSrcUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("������KEY"), _T("�����ڼҼ�"), _T("����������"), _T("ȸ������") 
	};
	TCHAR *sDstUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("USER_KEY"), _T("USER_POSITION_CODE"), _T("USER_CLASS_CODE"), _T("MEMBER_CLASS") 
	};
	TCHAR *sDstUserFieldTypeAlias[nSrcUserFieldCnt] = 
	{
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") 
	};
	
	CString sDMFieldAlias[nDeleteCnt] = 
	{
		_T("�з���ȣ"), _T("�з���ȣ����"), _T("��ϱ���"),	_T("��Ϲ�ȣ"), _T("��ġ��ȣ"), 
		_T("û����ȣ"), _T("����"), _T("å��"),	_T("�����ڷ����"), _T("���ุ����")
	};
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();
	pDM->StartFrame();


	CESL_DataMgr *pKLDM = m_pParentMgr->FindDM(_T("DM_KL_LOAN"));
	if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	for(INT i= 0 ; i < nSrcFieldCnt ;i++)
	{		
		ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, sSrcFieldAlias[i], sGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if((m_pManageValue->m_bVolReserve) && 1==i)
		{
			sPublishFormCode = sGetData;
		}
		else if((m_pManageValue->m_bVolReserve) && 2==i)
		{
			sSpecieskey = sGetData;
		}
		else if((m_pManageValue->m_bVolReserve) && 5==i)
		{
			sVolinfo = sGetData;
			
			if(_T("MO")==sPublishFormCode)
			{
				sMoSeDelim.Format(	_T(" FROM BO_BOOK_TBL BB,")
									_T(" IDX_BO_TBL IB")
									_T(" WHERE IB.REC_KEY=BB.SPECIES_KEY")
									_T(" AND BB.WORKING_STATUS = 'BOL112N'")
									_T(" AND BB.VOL_SORT_NO"));
			}
			else if(_T("SE")== sPublishFormCode)
			{
				sMoSeDelim.Format(	_T(" FROM SE_BOOK_TBL BB,")
									_T(" IDX_SE_TBL IB,")
									_T(" SE_VOL_TBL V")
									_T(" WHERE IB.REC_KEY=BB.SPECIES_KEY")
									_T(" AND BB.VOL_KEY=V.REC_KEY")
									_T(" AND BB.WORKING_STATUS IN ('SEL112N', 'SEL212N')")
									_T(" AND BB.VOL_KEY"));	
			}

			if(FALSE == sVolinfo.IsEmpty())
			{
				sALineQuery.Format(_T("SELECT COUNT(BB.REC_KEY)%s = %s AND BB.SPECIES_KEY = %s"), sMoSeDelim, sVolinfo, sSpecieskey); 
			}
			else
			{
				sALineQuery.Format(_T("SELECT COUNT(BB.REC_KEY)%s IS NULL AND BB.SPECIES_KEY = %s"), sMoSeDelim, sSpecieskey); 
			}

			sALineQuery	+=	_T(" AND BB.USE_LIMIT_CODE NOT IN ('CL','CD','CA','IZ')");

			ids = pDM->GetOneValueQuery(sALineQuery, sHoldCnt);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
					
			if(FALSE == m_bLoanCallReserve)
			{
				INT		nHoldCnt	=	_ttoi(sHoldCnt);
				
				if(0 < nHoldCnt)
				{
					INT			nReserveMarkCnt, nPossibleLoanCnt;
					CString		strReserveMarkCnt, strReserveMarkCntQuery;
					
					CString		strBookTbl;
					if(_T("MO")==sPublishFormCode)
					{
						strBookTbl	=	_T("BO_BOOK_TBL");
					}
					else if(_T("SE")== sPublishFormCode)
					{
						strBookTbl	=	_T("SE_BOOK_TBL");
					}

					strReserveMarkCntQuery.Format(_T("SELECT COUNT(CL.REC_KEY) FROM LS_WORK_T01 CL, %s BB WHERE CL.BOOK_KEY=BB.REC_KEY AND CL.SPECIES_KEY=%s AND CL.STATUS='3' "), strBookTbl, sSpecieskey);

					if(FALSE == sVolinfo.IsEmpty())
					{
						CString		strTemp;	
						strTemp.Format(_T("AND CL.VOL_INFO=%s "), sVolinfo);
						strReserveMarkCntQuery	+=	strTemp;
					}
					else
					{
						strReserveMarkCntQuery	+=	_T("AND CL.VOL_INFO IS NULL ");
					}
					
					strReserveMarkCntQuery	+=	_T("AND CL.REG_NO IS NOT NULL AND BB.USE_LIMIT_CODE NOT IN ('CL','CD','CA','IZ')");
					
					ids = pDM->GetOneValueQuery(strReserveMarkCntQuery, strReserveMarkCnt);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

					nReserveMarkCnt		=	_ttoi(strReserveMarkCnt);		
					
					nPossibleLoanCnt	=	(nHoldCnt - nReserveMarkCnt);	
					if(0 < nPossibleLoanCnt)
					{
						sLog.Format(_T("������ �ڷ� �� ��ġ���� ������ �ڷᰡ �����մϴ�.(%d��)\n")
									 _T("�׷��� ���� �Ͻðڽ��ϱ�?\n\n"), nPossibleLoanCnt);
						nLinkYN = AfxMessageBox(sLog, MB_ICONQUESTION | MB_YESNO);
						if(IDNO == nLinkYN)
						{
							pDM->InitDBFieldData();
							pDM->EndFrame();
							return 1001;
						}
					}
				}
			}
			else
			{
				m_bLoanCallReserve = FALSE;
			}
			
		}
		

		else if((m_pManageValue->m_bVolReserve) && 9==i)
		{
			break;
		}
		
		if(sGetData.GetLength() > 500)
		{
			sGetData = sGetData.Mid(0,500);
			if(sGetData.GetAt(499) < 0)
			{
				if(IsDBCSLeadByteEx(0, (BYTE)sGetData.GetAt(499)) != 0)
					sGetData = sGetData.Mid(0,499);
			}
		}
 
		ids = pDM->AddDBFieldData(sDstFieldAlias[i], sDstFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	}

	for(i= 0 ; i < nSrcUserFieldCnt ;i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sSrcUserDMAlias, sSrcUserFieldAlias[i], sGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

		ids = pDM->AddDBFieldData(sDstUserFieldAlias[i], sDstUserFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	}

	
	ids = pMobileApi->MakeRecKey(sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("����/�ݳ�����KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	
	ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), sLoanStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	if(FALSE == m_pManageValue->m_bVolReserve)
	{		
		ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("����"), sWorkingStatus, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

		GetTempData(_T("�������"), sReservationOrder);

		if(sReservationOrder.Compare(_T("0")) == 0)
		{
			if(	     sWorkingStatus.Compare(_T("BOL112N"))==0 
				|| 	(sWorkingStatus.Compare(_T("SEL212N"))==0) 
				||	(sWorkingStatus.Compare(_T("SEL112N"))==0)		)
			{
				ids = GetReservationExpireDay(nExpireDay);
				if(0 > ids) return ids;

				
				ids = MakeReservationExpireDate(nExpireDay, sExpireDate);
				if(0 > ids) return ids;
				
			}

			if(!sExpireDate.IsEmpty())
			{
				m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("���ุ����"), sExpireDate, 0);
				CString strStringDate;
				strStringDate.Format(_T("%s 23:59:59"), sExpireDate);
				
				sExpireDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strStringDate);

				pDM->AddDBFieldData(_T("RESERVATION_EXPIRE_DATE"), _T("NUMERIC"), sExpireDate, TRUE);

			}
		}
	}

	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("�������"), sLoanStatus, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);	

	CString strSysdate;
	CTime tCurTime = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strSysdate);
	sReservationDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strSysdate);

	ids = pDM->AddDBFieldData(_T("RESERVATION_DATE"), _T("NUMERIC"), sReservationDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("������"), strSysdate, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	
	sLog.Format(_T("%s"), m_pParentMgr->GetWorkLogMsg(_T("����"), __WFILE__, __LINE__));
	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), sLog, TRUE);
	
	ids = pMobileApi->MakeInsertFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	pDM->InitDBFieldData();
	
	if(m_pManageValue->m_bVolReserve)
	{
		for(INT m = 0 ; m < nDeleteCnt ; m++)
		{
			ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, sDMFieldAlias[m], _T(""), 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
		}
	}

	SetTempData(SHARE_CO_LOAN_TBL_INSERT_REC_KEY, CComVariant(sRecKey));
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationUserInfoDBUpdate(CKolas2up_MobileApi *pMobileApi, CString strKLUserYN/*=_T("N")*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReservationUserInfoDBUpdate");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	pDM->InitDBFieldData();
	
	CString sRecKey;
	CString sLoanBookCnt;
	CString sDMUserInfoAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("������KEY"), sRecKey, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("�ѿ���å��"), sLoanBookCnt, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	sLoanBookCnt.Format(_T("%d"), _ttoi(sLoanBookCnt) + 1);

	
	ids = pDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sLoanBookCnt, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("�弭����"), __WFILE__, __LINE__), TRUE);

	
	ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();	
	
	CString sUpdateFields = _T("RESERVATION_COUNT");
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(sUpdateFields));
	

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsReserveMember(BOOL &IsReserve)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsReserveMember");

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO") ;

	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(pDM->GetRowCount() != 1)
	{
		SelfCloseMessageBox(_T("�����ڸ� ���� �����Ͻʽÿ�"));

		
		MoveRegNoToNextProcBasket(EXP_RESERVE);
		IsReserve = FALSE;
		return 0;
	}

	CString sIsLoanBookCnt;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("���డ��å��"), sIsLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if(_ttoi(sIsLoanBookCnt) < 1 && m_IsReqPrior == FALSE )
	{
		SelfCloseMessageBox(_T("���� �ѵ��� �ʰ��Ͽ� ������ �� �����ϴ�."));
		IsReserve = FALSE;
		return 0;
	}

	CString sInferiorClass;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("ȸ������"), sInferiorClass, 0);
	if(0 > ids) 
	{
		IsReserve = FALSE;
		return -1;
	}

	if(sInferiorClass.Compare(_T("0")) != 0)
	{
		SelfCloseMessageBox(_T("���� ȸ������ ������ �����մϴ�."));
		IsReserve = FALSE;
		return 0;
	}	
	
	if(m_pManageValue->m_sLoanIsLostUserCard == _T("Y"))
	{		
		CString sLostUserCard;
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("ȸ�����н�"), sLostUserCard, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(sLostUserCard == _T("Y"))
		{
			SelfCloseMessageBox(_T("ȸ�����н� ȸ���� ������ �Ұ��� �մϴ�."));
			IsReserve = FALSE;
			return 0;
		}
	}
	

	IsReserve = TRUE;
	return 0;

EFS_END	
return -1;
}


INT CBL_LOC_LOAN_PROC::IsBookRelationCurrentUser(CString &sStatus, CString &sBookKey)
{
EFS_BEGIN

	INT ids = 0;
	CString strFuncName = _T("IsBookRelationCurrentUser");

	BOOL bEqual = FALSE;
	CString sUserKey = _T("");
	CString sSpeciesKey = _T("");
	CString sVolInfoKey = _T("");
	CString sTmpBookKey = _T("");
	CString sTmpSpeciesKey = _T("");
	CString sTmpVolInfoKey = _T("");

	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("������KEY"), sUserKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("åKEY"), sBookKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if((m_pManageValue->m_bVolReserve))
	{
		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("������Ű"), sSpeciesKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("��ȣ����KEY"), sVolInfoKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		ids = pDM->GetCellData(_T("åKEY"), i, sTmpBookKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = pDM->GetCellData(_T("�������"), i, sStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		if(m_pManageValue->m_bVolReserve)
		{
			ids = pDM->GetCellData(_T("��ȣ����KEY"), i, sTmpVolInfoKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			ids = pDM->GetCellData(_T("��KEY"), i, sTmpSpeciesKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		}

		if(!m_pManageValue->m_bVolReserve && sTmpBookKey == sBookKey)
		{
			
			if(_T("0") == sStatus || _T("2") == sStatus || _T("����") == sStatus || _T("�ݳ�����") == sStatus ||
				_T("L") == sStatus || _T("D") == sStatus || _T("O") == sStatus) 
			{
				bEqual = TRUE;
			}
			
			else if(_T("3") == sStatus || _T("����") == sStatus || 0 != sStatus.Find(_T("����(")))
			{
				return 2;
			}
		}

		if(m_pManageValue->m_bVolReserve &&	sSpeciesKey == sTmpSpeciesKey && sVolInfoKey == sTmpVolInfoKey)
		{
			
			if(_T("0") == sStatus || _T("2") == sStatus || _T("����") == sStatus || _T("�ݳ�����") == sStatus ||
				_T("L") == sStatus || _T("D") == sStatus || _T("O") == sStatus)	
			{
				if(sTmpBookKey == sBookKey)
				{
					return 1;	
				}
				else		
				{
					return 3;
				}
			}
			
			else if(_T("3") == sStatus || _T("����") == sStatus || 0 != sStatus.Find(_T("����(")))
			{
				return 2;
			}
			
			
		}
	}

	if(bEqual)	
	{
		return 1;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationCancel(BOOL bOption)
{
EFS_BEGIN

	CString strFuncName = _T("ReservationCancel");

	INT ids, nIndex, nCount, nUnmennedCnt_Y, nUnmennedCnt_O;
	CString sRecKey, sStatus, sReservationBookCnt, sUserRecKey, sMailSql;

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pGrid->SelectMakeList();
	INT nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	INT nStatusColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, _T("�������"), nStatusColIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	//JOB.2019.0073 : �浹 License
// 	CLocMailMgrAPI* pMailMgr;
// 	pMailMgr = NULL;
// 	if((m_pParentMgr->m_pInfo->MODE==10000) || (m_pParentMgr->m_pInfo->MODE==30000))
// 	{
// 	}
// 	else
// 	{		
// 		if(m_pCheckLoan->IsSendCancelMail())
// 		{
// 			pMailMgr = new CLocMailMgrAPI(m_pParentMgr);
// 			pMailMgr->CreateMailInfo();
// 		}
// 	}
// 	
	nCount = 0;
	nUnmennedCnt_Y = 0;
	nUnmennedCnt_O = 0;
	CString sValue;
	for(INT i = 0 ; i < nRowCount ;i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		ids = pDM->GetCellData(_T("����/�ݳ�����KEY"), nIndex, sRecKey);
		if(0 > ids)
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		ids = pDM->GetCellData(_T("�������"), nIndex, sStatus);
		if(0 > ids)
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		
		
		if(m_pManageValue->m_sUnmannedReserveSys == _T("Y"))
		{
			ids = pDM->GetCellData(_T("���δ��⿹�����"), nIndex, sValue);
			if(0 > ids) 
			{
				//JOB.2019.0073 : �浹 License
// 				if(pMailMgr != NULL)
// 					delete pMailMgr;
				ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}

			
			if(sStatus.Compare(_T("3")) != 0)
			{
				pGrid->SelectGetPrev();
				continue;
			}
			
			else if(sStatus.Compare(_T("3")) == 0 && sValue.Compare(_T("O")) == 0)
			{
				pGrid->SelectGetPrev();
				nUnmennedCnt_O++;
				continue;
			}
			
			if(sStatus.Compare(_T("3")) == 0 && sValue.Compare(_T("Y")) == 0)
			{
				pGrid->SelectGetPrev();
				nUnmennedCnt_Y++;
				continue;
			}
			
			else if(sStatus.Compare(_T("3")) == 0 && sValue.IsEmpty())
			{
				pGrid->SelectGetPrev();
				nCount++;
				continue;
			}
		}
		
		else
		{
			
			if(sStatus.Compare(_T("3")) != 0)
			{
				pGrid->SelectGetPrev();
				continue;
			}
			else
			{
				nCount++;
				continue;
			}
		}
	}



	if(m_pManageValue->m_sUnmannedReserveSys == _T("Y"))
	{
		if(nCount + nUnmennedCnt_Y > 0)
		{
			if(!bOption)
			{
				CString sMsg;
				sMsg.Format(_T("������ �ڷḦ ��������Ͻðڽ��ϱ�?\r\n\r\n")
							 _T("           �Ϲݿ��� : [ %d ]��\r\n")
							 _T("     ���δ����û : [ %d ]��\r\n"), nCount, nUnmennedCnt_Y);
				if(IDYES!=MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("���� ���"), MB_YESNO | MB_ICONQUESTION))
				{
					//JOB.2019.0073 : �浹 License
// 					if(pMailMgr != NULL)
// 						delete pMailMgr;
					return 0;
				}
			}
		}
		else
		{
			if(nUnmennedCnt_O > 0)
			{
				MessageBox(m_pParentMgr->m_hWnd, _T("���δ����� �� ���δ������� �ڷ�� ������� �� �� �����ϴ�.")
												, _T("������ҽ���"), MB_OK | MB_ICONINFORMATION);
				//JOB.2019.0073 : �浹 License
// 				if(pMailMgr != NULL)
// 					delete pMailMgr;
				return 0;
			}
			else
			{
				MessageBox(m_pParentMgr->m_hWnd, _T("������ �ڷ��� ����� �ڷᰡ �������� �ʽ��ϴ�.")
												, _T("������ҽ���"), MB_OK | MB_ICONINFORMATION);
				//JOB.2019.0073 : �浹 License
// 				if(pMailMgr != NULL)
// 					delete pMailMgr;
				return 0;
			}
		}
	}
	else
	{
		if(nCount > 0)
		{
			
			if(!bOption)
			{
				if(IDYES != MessageBox(m_pParentMgr->m_hWnd,  _T("������ �ڷḦ ��������Ͻðڽ��ϱ�?"),
																_T("���� ���"), MB_YESNO | MB_ICONQUESTION))
				{
					//JOB.2019.0073 : �浹 License
// 					if(pMailMgr != NULL)
// 						delete pMailMgr;
					return 0;
				}
			}
		}
		else
		{


			SelfCloseMessageBox(_T("������ �ڷ��� ����� �ڷᰡ �������� �ʽ��ϴ�."));
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			return 0;
		}
	}
	
	pGrid->SelectMakeList();
	nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	nCount = 0;
	for(i = 0; i < nRowCount; i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		ids = pDM->GetCellData(_T("����/�ݳ�����KEY"), nIndex, sRecKey);
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		ids = pDM->GetCellData(_T("�������"), nIndex, sStatus);
		if(0 > ids)
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		
		
		if(m_pManageValue->m_sUnmannedReserveSys == _T("Y"))
		{
			ids = pDM->GetCellData(_T("���δ��⿹�����"), nIndex, sValue);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			
			if(sStatus.Compare(_T("3")) != 0)
			{
				pGrid->SelectGetPrev();
				continue;
			}
			
			else if(sStatus.Compare(_T("3")) == 0 && sValue.Compare(_T("O")) == 0)
			{
				pGrid->SelectGetPrev();
				continue;
			}
		}
		
		else
		{
			
			if(sStatus.Compare(_T("3")) != 0)
			{
				pGrid->SelectGetPrev();
				continue;
			}
		}

		
		TCHAR *sFieldAlias[6] = 
		{
			_T("����/���ӱ���"), _T("åKEY"), _T("�����̿��ڼ�"), _T("��KEY"), _T("��ȣ����KEY"), _T("��Ϲ�ȣ")
		};
		CString sGetData[6];
		
		for(INT i = 0 ; i < 6 ; i++)
		{
			sGetData[i].Empty();
			ids = pDM->GetCellData(sFieldAlias[i], nIndex, sGetData[i]);
		}

		
		CKolas2up_MobileApi *pMobileApi;
		pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

		this->SetBEGIN_SBL();

		
		
		pDM->StartFrame();
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), _T("4"), TRUE);
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

 		CString sReserveCancelDate = _T("SYSDATE");

		
		
		ids = pDM->AddDBFieldData(_T("RESERVATION_CANCEL_TYPE"), _T("STRING"), _T("1"), TRUE);
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
	
		ids = pDM->AddDBFieldData(_T("RESERVE_CANCEL_DATE"), _T("NUMERIC"), sReserveCancelDate, TRUE);
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}	
		
		if(bOption)
		{
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("å�η� ��ȯ ��û"), __WFILE__, __LINE__), TRUE);
		}
		else
		{
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("�������"), __WFILE__, __LINE__), TRUE);
		}
		
		pDM->AddDBFieldData(_T("R_DEVICE"), _T("STRING"), _T("KOLASIII"), TRUE);
			
		ids = pMobileApi->MakeUpdateFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}		
		
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�ѿ���å��"), sReservationBookCnt, 0);
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		sReservationBookCnt.Format(_T("%d"), _ttoi(sReservationBookCnt) - 1);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("������KEY"), sUserRecKey, 0);
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
	
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sReservationBookCnt, TRUE);
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}		
		
		if(bOption)
		{
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("å�η� ��ȯ ��û"), __WFILE__, __LINE__), TRUE);
		}
		else
		{
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("�������"), __WFILE__, __LINE__), TRUE);
		}
		
		ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserRecKey);
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		ids = pMobileApi->SendFrame();
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		pDM->EndFrame();

		SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
		SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("STATUS,RESERVE_CANCEL_DATE,RESERVATION_CANCEL_TYPE,R_DEVICE")));
		
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sUserRecKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("RESERVATION_COUNT")));

		ShareInfoCenterUpload(_T("U"), _T("U"));
				
		CLocCommonAPI::MobileApiEndLog(pMobileApi);
		
		if(nStatusColIndex != -1)
		{
			ids = pGrid->SetAt(nIndex, nStatusColIndex, _T("�������"));
			if(0 > ids) 
			{
				//JOB.2019.0073 : �浹 License
// 				if(pMailMgr != NULL)
// 					delete pMailMgr;
				ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}
		}

		m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("�������"), _T("4"), nIndex);

		if(1 < _ttoi(sGetData[2]) && FALSE == bOption)

		{
			CString strReserveMsg;
			if(sGetData[5].IsEmpty())
			{
				strReserveMsg.Format(
					_T("������ �̿��ڰ� �ֽ��ϴ�. ���� ó���� �Ͻðڽ��ϱ�?"), sGetData[5]);
			}
			else
			{
				strReserveMsg.Format(
					_T("[%s]�� ������ �̿��ڰ� �ֽ��ϴ�. ���� ó���� �Ͻðڽ��ϱ�?"), sGetData[5]);
			}
			if(IDYES == AfxMessageBox(strReserveMsg, MB_YESNO))
			{
				ReservationStatus(sGetData);
			}
		}
		
		ids = AddMemberInfo();
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			return ids;
		}

		ids = AllControlDisplayNotGrid();
		if(0 > ids) 
		{
			//JOB.2019.0073 : �浹 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			return ids;
		}
		sMailSql.Format(_T(" CL.REC_KEY= %s "), sRecKey);	
		
		if(m_pParentMgr->m_pInfo->MODE == 10000 || (m_pParentMgr->m_pInfo->MODE == 30000))
		{
		}
		else
		{
			if(m_pCheckLoan->IsSendCancelMail())
			{
				//JOB.2019.0073 : �浹 License
				// pMailMgr->MakeRecord(6, sMailSql, FALSE); 
			}
		}
	
		pDM->DeleteRow(nIndex);		

		pGrid->SelectGetPrev();
		nCount++;

		AllControlDisplayGrid();
		InitGridColor();
		ViewGrid();	
	}
		
	if(bOption)
	{
		SelfCloseMessageBox(_T("å�η� ��ȯ ��û�� �Ϸ�Ǿ����ϴ�."));
	}
	else
	{
		if(nCount > 0) 
		{
			SelfCloseMessageBox(_T("������ ��� �Ǿ����ϴ�.\r\nüũ�� �κ��� ������ �ڷḸ ���� ��Ұ� �Ǿ����ϴ�."));
		}
		else 
			SelfCloseMessageBox(_T("������ �ڷ��� ����� �ڷᰡ �������� �ʽ��ϴ�."));
		}
	//JOB.2019.0073 : �浹 License
// 	if(pMailMgr != NULL)
// 		delete pMailMgr;
// 	pMailMgr = NULL;
		
	INT nLoanRowCount = pDM->GetRowCount();
	this->SetEND_SBL(nLoanRowCount);	
	
	return 0;

EFS_END
return -1;
}

VOID CBL_LOC_LOAN_PROC::ViewMessage(CString sMsg)
{
EFS_BEGIN

	MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("����/�ݳ�"), MB_ICONINFORMATION | MB_OK);

	InputMessageBox(sMsg);

EFS_END
}

INT CBL_LOC_LOAN_PROC::DelayLoanDate(INT nMode /*=0*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("DelayLoanDate");
	
	CString sDMAlias	= _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias	= _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias	= _T("MainGrid");
	CString sTableName	= _T("LS_WORK_T01");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	ids = pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nRowCount = pGrid->SelectGetCount();
	if(1 > nRowCount)
	{
		SelfCloseMessageBox(_T("������ �ݳ����� �ڷᰡ �����ϴ�."));
		return 0;
	}
	
	CString strOtherLoanKey = _T(""), strLCTKey = _T(""), strLoanLib = _T(""), strBookLib = _T(""), strReloanYN = _T("N"), strMessage = _T("");
	BOOL bOtherLoan = FALSE;
	INT nIdx = -1, j = 0, nSuccCnt = 0;
	nIdx = pGrid->SelectGetHeadPosition();
	for( j=0 ; j < pGrid->SelectGetCount() ; j++) {
		ids = pDM->GetCellData(_T("Ÿ������KEY"), nIdx , strOtherLoanKey);
		ids = pDM->GetCellData(_T("LCT_KEY"), nIdx , strLCTKey);		
		ids = pDM->GetCellData(_T("���⵵������ȣ"), nIdx , strLoanLib);
		ids = pDM->GetCellData(_T("������������ȣ"), nIdx , strBookLib);	
		
		if ( !strOtherLoanKey.IsEmpty() ) {
			bOtherLoan = TRUE;
			BOOL bSuccess = FALSE;
			ids = KL_Reloan_Proc(nIdx, bSuccess);
			
			if ( ids < 0 ) {
				pGrid->SetAt(nIdx, pGrid->m_nSelectIdx, _T(""));
				return ids;
			}
			pGrid->SetAt(nIdx, pGrid->m_nSelectIdx, _T(""));
			if ( bSuccess == TRUE ) nSuccCnt++;
		}
		nIdx = pGrid->SelectGetNext();
	}

	if ( nSuccCnt > 0 ) {
		strMessage.Format(_T("%d���� ������ȣ�����ڷᰡ �ݳ����� �Ǿ����ϴ�."), nSuccCnt);
		SelfCloseMessageBox(strMessage);
	}	
	
	ids = pGrid->SelectMakeList();
	
	nRowCount = pGrid->SelectGetCount();
	if(1 > nRowCount) {		
		return 0;
	}
		
	CString sDelayDays		= _T("");
	CString sMemberState	= _T("");
	
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		ids = pDM->GetCellData(_T("��ü�ϼ�"), i, sDelayDays);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(0 < _ttoi(sDelayDays))
		{
			SelfCloseMessageBox(_T("��ü �̿��ڴ� �ݳ����Ⱑ �Ұ����մϴ�."));
			return 0;
		}
	}
	
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("ȸ������"), sMemberState, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(_T("0") != sMemberState)
	{		
		SelfCloseMessageBox(_T("����ȸ���� �ݳ����Ⱑ �����մϴ�."));
		return 0;
	}
	
	if(_T("Y") == m_pManageValue->m_sLoanIsLostUserCard)
	{
		CString sLostUserCard;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("ȸ�����н�"), sLostUserCard, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(_T("Y") == sLostUserCard)
		{
			SelfCloseMessageBox(_T("ȸ�����н� ȸ���� �ݳ����Ⱑ �Ұ��� �մϴ�."));
			return 0;
		}
		
	}

	CString sDelayDay		= _T("");	
	CString sDelayCanCnt	= _T("");	
	INT nDelayMode			= -1;		

	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�ݳ�������"), sDelayDay);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(sDelayDay.IsEmpty())
	{
		SelfCloseMessageBox(_T("�ݳ� �������� �Է��Ͻʽÿ�"));
		return 0;
	}
	
	ids = m_pCheckLoan->GetDelayCnt(sDelayCanCnt);
	if(0 > ids) return ids;

	ids = GetDelayLoanMode(nDelayMode);
	if(0 > ids) return ids;
	
	TCHAR *sLinkDMFieldAlias[2] = {_T("�ݳ�������"), _T("�������")};
	INT nColIndex[2] = {-1, -1};
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, sLinkDMFieldAlias, 2, nColIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sSelected			= _T("");	
	CString sGetStatus			= _T("");	
	CString sUserManageCode		= _T("");   
	CString strGetManageValue	= _T("");	
	CString sRecKey				= _T("");	
	CString sLoanDate			= _T("");	
	CString sDelayOptionDate	= _T("");	
	CString sLoanDelayDate		= _T("");	
	CString strRegNo			= _T("");	
	CString sDelayCnt			= _T("");	
	CString strMsg				= _T("");	
	CString sRegCode			= _T("");	
	CString sTmpData			= _T("");	
		
	BOOL bSkipCheck = FALSE;				
	INT nCount		= 0;
	INT nDelayDay	= 0;
	INT nIndex		= 0;
		
	CString strLoanDate;
	CString sType;

	CString strGetData;
	CTime tCurTime = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strGetData);
	CString strCurDate = strGetData.Left(10);

	CESL_DataMgr tDM;
	tDM.SetCONNECTION_INFO(m_pManageValue->m_pInfo->CONNECTION_INFO);
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		KL_StartTemp();
	}

	pGrid->SelectGetHeadPosition();	

	for(i = 0; i < nRowCount; i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(0 > nIndex) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pDM->GetCellData(_T("��Ϲ�ȣ"), nIndex, strRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			
			ids = pDM->GetCellData(_T("��Ȳ����"), nIndex, sType);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(_T("����") == sType || _T("����") == sType)
			{
				strMsg.Format(_T("%s ��Ȳ�� �ݳ����� �� �� �����ϴ�."), sType);
				ViewMessage(strMsg);
				pGrid->SelectGetNext();
				continue;
			}
		}

		CString strFlag;
		m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("�ݳ��������"), _T("���Ϲݳ������������"), strFlag);
		strFlag.TrimLeft();	strFlag.TrimRight();
		strFlag.MakeUpper();
		if(_T("N") == strFlag)
		{
			ids = pDM->GetCellData(_T("������"), nIndex, strLoanDate);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(strCurDate == strLoanDate.Mid(0,10))
			{
				strMsg.Format(_T("��Ϲ�ȣ�� '%s'�� �ڷ�� ���� ������ �ڷ��̹Ƿ� �ݳ����� �� �� �����ϴ�."), strRegNo);
				ViewMessage(strMsg);
				pGrid->SelectGetNext();
				continue;
			}
		}

		ids = pDM->GetCellData(_T("����/�ݳ�����KEY"), nIndex, sRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		ids = pDM->GetCellData(_T("�ݳ�������"), nIndex, sLoanDate);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		INT nRegCodeLength;
		nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;

		if(strRegNo.GetLength() > nRegCodeLength)
		{
			sRegCode = strRegNo.Left(nRegCodeLength);
		}
		
		CString sDescRegCode;
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��ϱ���"), sRegCode, sDescRegCode);
		if(sDescRegCode.IsEmpty())
		{
			ids = pDM->GetCellData(_T("��ϱ���"), nIndex, sRegCode);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		ids = pDM->GetCellData(_T("�������"), nIndex, sGetStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		
		if(_T("1") == sGetStatus || _T("3") == sGetStatus || _T("4") == sGetStatus || 
			_T("R") == sGetStatus || _T("O") == sGetStatus) 
		{
			pGrid->SelectGetNext();
			continue;
		}

		if(_T("Y") == m_pManageValue->m_sBookCooperativeSys)
		{
			ids = m_pCheckLoan->GetUserManageCode(sUserManageCode);

			CString strBookManageCode;
			ids = pDM->GetCellData(_T("��������"), nIndex, strBookManageCode);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			CString strBookManageCodeDesc;
			ids = pDM->GetCellData(_T("�������м���"), nIndex, strBookManageCodeDesc);
			
			if(strBookManageCode != sUserManageCode)
			{
				CESL_DataMgr TmpDM;
				TmpDM.SetCONNECTION_INFO(m_pManageValue->m_pInfo->CONNECTION_INFO);	
				CString strQuery;
				strQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL ")
								_T("WHERE CLASS_ALIAS='å�η��ý���' AND VALUE_NAME='Ÿ���ݳ������������' ")
								_T("AND MANAGE_CODE='%s'"), strBookManageCode);
				CString strValue;
				TmpDM.GetOneValueQuery(strQuery, strValue);
				if(_T("Y") != strValue)
				{
					strMsg.Format(_T("'%s'������ �ڰ��ڷῡ ���� �ٸ� ���������� �ݳ������� �� ������ �����ϰ� �ֽ��ϴ�.")
									, strBookManageCodeDesc);
					ViewMessage(strMsg);
					pGrid->SelectGetNext();
					continue;
				}
			}
		}
		else 
		{
			if(_T("N") == m_pManageValue->m_sIsReturnManageCode)
			{
				ids = m_pCheckLoan->GetUserManageCode(sUserManageCode);
				ids = pDM->GetCellData(_T("��������"), nIndex, strGetManageValue);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				if(strGetManageValue != sUserManageCode)
				{			
					strMsg.Format(_T("��Ϲ�ȣ�� '%s'�� �ڷ�� ���������� �ٸ��Ƿ� �ݳ����� �� �� �����ϴ�."), strRegNo);
					ViewMessage(strMsg);
					pGrid->SelectGetNext();
					continue;
				}
			}
		}

		if(1 == nDelayMode)
		{
			CString sBookRecKey;
			CString sSpeciesKey;
			CString sVolInfoKey;
			ids = pDM->GetCellData(_T("åKEY"), nIndex, sBookRecKey);
			ids = pDM->GetCellData(_T("��KEY"), nIndex, sSpeciesKey);
			ids = pDM->GetCellData(_T("��ȣ����KEY"), nIndex, sVolInfoKey);

			CString sReserveBookCnt;
			CString sVolReserveCnt = _T("");
			
			ids = IsReservatedBook(sBookRecKey, sSpeciesKey, sVolInfoKey, sReserveBookCnt, sVolReserveCnt, nIndex);
			if(0 > ids) return ids;

			if(0 < _ttoi(sReserveBookCnt) || 0 < _ttoi(sVolReserveCnt)) 
			{
				strMsg.Format(_T("��Ϲ�ȣ�� '%s'�� �ڷ�� �ٸ� ����ڰ� �������̹Ƿ� �ݳ����Ⱑ �Ұ����մϴ�."), strRegNo);
				ViewMessage(strMsg);
				pGrid->SelectGetNext();
				continue;
			}		
		}
		
		ids = pDM->GetCellData(_T("�ݳ�����Ƚ��"), nIndex, sDelayCnt);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(sDelayCnt.IsEmpty())
		{
			sDelayCnt = _T("0");
		}
		if(_ttoi(sDelayCanCnt) <= _ttoi(sDelayCnt))
		{
			strMsg.Format(_T("��Ϲ�ȣ�� '%s'�� �ڷ�� �ݳ����� Ƚ��('%s'ȸ)�� �ʰ��Ͽ� �ݳ����Ⱑ �Ұ����մϴ�."), strRegNo, sDelayCnt);
			ViewMessage(strMsg);
			pGrid->SelectGetNext();
			continue;
		}

		sDelayCnt.Format(_T("%d"), _ttoi(sDelayCnt) + 1);
		
		CString sDelayReturnDate;
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			
			sDelayOptionDate = strCurDate;

 			CTime tCurrentTime = CTime::GetCurrentTime();
 			sDelayReturnDate = sDelayOptionDate + tCurrentTime.Format(_T(" %H:%M:%S"));
			if(0 == m_pCheckLoan->GetDelayOption())
			{
				sDelayOptionDate = sLoanDate;
			}
		}
		else
		{
			if(0 == m_pCheckLoan->GetDelayOption())
			{
				sDelayOptionDate = sLoanDate;
			}
			else
			{
				
				sDelayOptionDate = strCurDate;

			}
			sDelayReturnDate = _T("SYSDATE");
		}

		CTime tLoanDate(_ttoi(sDelayOptionDate.Mid(0,4)), _ttoi(sDelayOptionDate.Mid(5,2)), _ttoi(sDelayOptionDate.Mid(8,2)), 0, 0, 0);

		m_pCheckLoan->GetReturnDelayDay(sTmpData, sRegCode);
		if(!bSkipCheck)
		{
			nDelayDay = _ttoi(sTmpData);
			
			
			if(1 > nDelayDay || 30 < nDelayDay)
			{
				strMsg.Format(	_T("�ݳ��������� 1���� 30���� ������ �ش����� �ʽ��ϴ�.\r\n")
								_T("�ش������� �ݳ������ϼ��� Ȯ���Ͽ� �ֽʽÿ�."));
				ViewMessage(strMsg);
				pGrid->SelectGetNext();
				continue;
			}
			
		}
		if(1 == m_pCheckLoan->GetAutoDelayFunc())
		{
			if(!bSkipCheck)
			{
				CInputReservatoinExpireDay InputDlg(m_pParentMgr, _T("�ݳ� ������ �Է�"));
				InputDlg.SetDay(nDelayDay);
				if(IDOK != InputDlg.DoModal())
				{
					return 0;
				}
				InputDlg.GetDay(nDelayDay);
				bSkipCheck = InputDlg.GetAllApply();
			}
		}
		
		m_pCheckLoan->GetAddHolyDay(nDelayDay, tLoanDate);

		CTimeSpan tDelayDay(nDelayDay, 0, 0, 0);
		tLoanDate += tDelayDay;
		sLoanDelayDate = tLoanDate.Format(_T("%Y/%m/%d"));
		
		
		CString sStatus = _T("2");  
		
		
		if(_T("L") == sGetStatus || _T("D") == sGetStatus)
		{
			sStatus = _T("D");
		}
		
		CKolas2up_MobileApi *pMobileApi;
		pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

		this->SetBEGIN_SBL();

		pDM->StartFrame();
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData(_T("DELAY_CNT"), _T("NUMERIC"), sDelayCnt, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		
		CString strReturnPlanDate;
		if(!sLoanDelayDate.IsEmpty())
		{
			strReturnPlanDate = _T("TO_DATE('") + sLoanDelayDate + _T(" 23:59:59") + _T("', 'YYYY/MM/DD HH24:MI:SS')");
		}
		else
		{
			strReturnPlanDate = _T("NULL");
		}
		ids = pDM->AddDBFieldData(_T("RETURN_PLAN_DATE"), _T("NUMERIC"), strReturnPlanDate, TRUE);
		
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
		CString sDBDelayReturnDate;
		if ( sDelayReturnDate == _T("SYSDATE") )
		{
			sDBDelayReturnDate.Format(_T("TO_DATE(%s, 'YYYY/MM/DD HH24:MI:SS')"), sDelayReturnDate);
		}
		else
		{
			sDBDelayReturnDate.Format(_T("TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS')"), sDelayReturnDate);
		}
		

		ids = pDM->AddDBFieldData(_T("DELAY_RETURN_DATE"), _T("NUMERIC"), sDBDelayReturnDate, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = pDM->AddDBFieldData(_T("PRE_RETURN_PLAN_DATE"), _T("STRING"), sLoanDate, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), sStatus, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		
		
		CString strBookLibCode, strQuery;
		strQuery.Format(_T("SELECT GET_BOOK_LIB_CODE(PUBLISH_FORM_CODE, BOOK_KEY ) FROM LS_WORK_T01 WHERE REC_KEY = %s "), sRecKey);
		ids = tDM.GetOneValueQuery(strQuery, strBookLibCode);
		
		if ( strBookLibCode.GetLength() == 0 ) {
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		
		CString strLastWork;
		if(1 == nMode)
		{
			strLastWork = m_pParentMgr->GetWorkLogMsg(_T("[ET]�ݳ�����"), __WFILE__, __LINE__);
		}
		else
		{
			strLastWork = m_pParentMgr->GetWorkLogMsg(_T("�ݳ�����"), __WFILE__, __LINE__);
		}
		pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), strLastWork, TRUE);
		ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		

		
		CString strTransNo;
		
		{
		if(_T("L") == sGetStatus || _T("D") == sGetStatus)
		{			
			BOOL bInsertTrans = FALSE;
			BOOL bIsEndTrans = FALSE; 
			CString strQuery = _T("");		
			CString strValue[2];
			
			strQuery.Format(_T("SELECT MAX(TRANS_NO), COUNT(1) FROM ILL_TRANS_TBL WHERE LOAN_KEY=%s"), sRecKey);
			ids = pMobileApi->GetValuesQuery(strQuery, strValue, 2);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			
			strTransNo = strValue[0];
			if(_T("1") != strValue[1]) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET RETURN_PLAN_DATE=%s, LAST_WORK='%s' ")
							_T("WHERE TRANS_NO='%s';")
							, strReturnPlanDate
							, m_pParentMgr->GetWorkLogMsg(_T("å�η��ݳ�����"), __WFILE__, __LINE__)
							, strTransNo);
			pMobileApi->AddFrame(strQuery);
		}
		}
	

		ids = pMobileApi->SendFrame();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		pDM->EndFrame();

		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			
			CString strGetData;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����ȸ������"), strGetData, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(_T("Y") == strGetData)
			{
				CString strLoanLibCode;
 				ids = pDM->GetCellData(_T("���⵵������ȣ"), nIndex, strLoanLibCode);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

 				pDM->GetCellData(_T("��������"), nIndex, strGetData);
				if(strGetData == m_pParentMgr->m_pInfo->MANAGE_CODE)
				{
					
					strLoanLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
					CString strUserNo;
					ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڹ�ȣ"), strUserNo, 0);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					CString strTempDate = _T("");
					if(!sLoanDelayDate.IsEmpty())
					{
						strTempDate = sLoanDelayDate + _T(" 23:59:59");
					}

					CFrameWriter Writer(m_pParentMgr->m_pInfo);
					Writer.setCommand(_T("IL_K23_LOC_B01_SERVICE"));
					Writer.addRecord();
					
					
					
					Writer.addElement(_T("���⵵����"), strBookLibCode);
					
					Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
					Writer.addElement(_T("�ݳ�������"), strTempDate);
					Writer.addElement(_T("�ݳ�������"), sDelayReturnDate);
					
					
					
					if ( _T("D") == sStatus || _T("2") == sStatus ) {
						Writer.addElement(_T("����"), _T("2"));
					}else{
						Writer.addElement(_T("����"), sStatus);
					}
										
					Writer.addElement(_T("����KEY"), sRecKey);
					
					if ( _T("D") == sStatus ) {
						Writer.addElement(_T("��������ȣ��������"), _T("Y"));
					}

 					CString strFrame = Writer.getFrame();
 					KL_AddTemp(strFrame);
				}
			}
		}
		
		CLocCommonAPI::MobileApiEndLog(pMobileApi);
		
		SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
		SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("DELAY_CNT,RETURN_PLAN_DATE,DELAY_RETURN_DATE,PRE_RETURN_PLAN_DATE,STATUS")));
				
		ShareInfoCenterUpload(_T("U"), _T("N"));
		
		if(-1 != nColIndex[0])
		{
			ids = pGrid->SetAt(nIndex, nColIndex[0], sLoanDelayDate);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
	
		m_pParentMgr->SetDataMgrData(sDMAlias, _T("�ݳ�������"), sLoanDelayDate, nIndex);

		if(-1 != nColIndex[1])
		{
			ids = pGrid->SetAt(nIndex, nColIndex[1], sStatus);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		m_pParentMgr->SetDataMgrData(sDMAlias, _T("�������"), sStatus, nIndex);
		m_pParentMgr->SetDataMgrData(sDMAlias, _T("�ݳ�����Ƚ��"), sDelayCnt, nIndex);
		pGrid->SelectGetNext();
		ViewGrid();

		nCount++;		
	}

	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		KL_EndTemp(m_pParentMgr->GetWorkLogMsg(_T("����ȸ���ݳ�����"), __WFILE__, __LINE__));
	}

	if(0 < nCount) 
	{
		SelfCloseMessageBox(_T("�ݳ��������� ����Ǿ����ϴ�."));
		
		ids = SearchLoanBookInfo();
		if(0 > ids) return ids;
		
		ids = AddMemberInfo();
		if(0 > ids) return ids;
		
		ids = ViewLoanScreen();
		if(0 > ids) return ids;
		
	}
	else
	{ 
		SelfCloseMessageBox(_T("�ݳ� ������ �ڷᰡ �������� �ʽ��ϴ�."));
	}
	
	INT nLoanRowCount = pDM->GetRowCount();
	this->SetEND_SBL(nLoanRowCount);
	

	return 0;
	
EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::SettingDelayDay(BOOL bReturn/*=FALSE*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SettingDelayDay");

	
	CString sReturnType;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("�ݳ�����"), sReturnType);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sReturnDate;
	CString sReturnDateControlAlias = _T("�ݳ���");
	if(sReturnType.Compare(_T("2"))== 0)
	{
		sReturnDateControlAlias = _T("�ұ޹ݳ���");
	}

	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, sReturnDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CTime tCurrent = CTime(_ttoi(sReturnDate.Mid(0,4)), _ttoi(sReturnDate.Mid(5,2)), _ttoi(sReturnDate.Mid(8,2)), 0, 0, 0);
	
	INT nRowCount = pDM->GetRowCount();

	CString sLoanDate;
	CString sDelayDay;	
	CString sStatus;	

	INT nMaxDelayDay = 0; 
	INT nTotalDelayDay = 0; 
	CTime tTemp; 

	CString sLoanPlace,sUserManageCode,sAppendixYN;

	for(INT i = 0 ; i < nRowCount ; i++)
	{
		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
 			CString strGetData;
 			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("��Ȳ����"), strGetData, i);
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(_T("����") == strGetData)
			{
				continue;
			}
			else if(_T("����") == strGetData)
			{
				if(_T("1") != m_pManageValue->m_strUserClassShare)
				{
					continue;
				}
			}
		}
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("�������"), sStatus, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);		
		
		if(0 == sStatus.Compare(_T("3")))
		{
			ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("��ü�ϼ�"), _T(""), i);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		if((sStatus.Compare(_T("1"))==0) || (sStatus.Compare(_T("3"))==0) || (sStatus.Compare(_T("4"))==0))
		{
			continue;
		}

		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("�ݳ�������"), sLoanDate, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		sLoanDate.TrimLeft(); 
		sLoanDate.TrimRight();
			
		if(sLoanDate.IsEmpty()) 
		{
			continue;
		}

		CTime tLoanDate(_ttoi(sLoanDate.Mid(0,4)), _ttoi(sLoanDate.Mid(5,2)), _ttoi(sLoanDate.Mid(8,2)), 0, 0, 0);
		CTimeSpan tDelayDay(0, 0, 0, 0);

		if(tLoanDate < tCurrent)
		{
			tDelayDay = tCurrent - tLoanDate; 
		}
		INT nDelayDay = tDelayDay.GetDays();
		sDelayDay.Format(_T("%d"), nDelayDay);

	
		CString strValue = _T("");
		m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("��ü�Ⱓ�ް�����������"), strValue);
		if( !strValue.IsEmpty() && _T("Y") != strValue)		
		{
			CString strQuery = _T("");
			CString strHolidayCount  = _T("");
			INT nHolidayCount = 0;
			CESL_DataMgr pTempDM;
			pTempDM.SetCONNECTION_INFO( m_pParentMgr->m_pInfo->CONNECTION_INFO );

			strQuery.Format(_T("SELECT GET_HOLIDAYCOUNT(TO_DATE('%s','YYYY/MM/DD'), TO_DATE('%s','YYYY/MM/DD'), '%s') FROM DUAL"), sLoanDate , sReturnDate, m_pParentMgr->m_pInfo->MANAGE_CODE);
			pTempDM.GetOneValueQuery(strQuery, strHolidayCount );
			nHolidayCount = _ttoi( strHolidayCount );

			if(nHolidayCount < nDelayDay)
			{
				sDelayDay.Format(_T("%d"), nDelayDay - nHolidayCount);
			}			
		}

		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("�������"), sLoanPlace, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("�ηϿ���"), sAppendixYN, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = m_pCheckLoan->GetUserManageCode(sUserManageCode);		
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		if(sAppendixYN.Compare(_T("A")) == 0 )
		{
			if(_T("Y") != m_pCheckLoan->m_pManageValue->m_sIsAppendixLoanStop)
			{
				continue;
			}
		}	

		ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("��ü�ϼ�"), sDelayDay, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(nMaxDelayDay < nDelayDay) 
		{
			nMaxDelayDay = nDelayDay;
		}
		nTotalDelayDay += nDelayDay;
	}

	INT nDelayMode = 0;
	if(nDelayMode == 1)
	{
		nTotalDelayDay = nMaxDelayDay;
	}

	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sInferiorClassAlias = _T("ȸ������");
	CString sInferiorClassDBName = _T("USER_CLASS");
	CString sRecKey;

	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("������KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sInferiorClass = _T("0");
	CString sLoanStopDate;
	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("����������"), sLoanStopDate, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	sLoanStopDate.TrimLeft();
	sLoanStopDate.TrimRight();

	if(sLoanStopDate.Find(_T("0000")) == 0)
	{
		sLoanStopDate = _T("");
		ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("����������"), sLoanStopDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	if(!sLoanStopDate.IsEmpty())
	{
		COleDateTime tLoanStopDate(_ttoi(sLoanStopDate.Mid(0,4)), _ttoi(sLoanStopDate.Mid(5,2)), _ttoi(sLoanStopDate.Mid(8,2)), 0, 0, 0);		

		if(COleDateTime(tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(), 0, 0, 0) > tLoanStopDate)
		{
			sInferiorClass = _T("0");
			sLoanStopDate = _T("");
			ids = m_pParentMgr->SetDataMgrData(sMemberDMAlias, _T("����������"), _T(""), 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		else
		{
			sInferiorClass = _T("1");	
		}
	}
	else
	{
		sInferiorClass = _T("0");
	}

	
	if(nTotalDelayDay > 0)
	{
		sInferiorClass = _T("1");
	}

	
	CString sPreInferiorClass;
	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, sInferiorClassAlias, sPreInferiorClass, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	BOOL bMessageView = true;
	
	if((sPreInferiorClass.Compare(sInferiorClass)==0) || (sPreInferiorClass.Compare(_T("2"))==0) || (sPreInferiorClass.Compare(_T("3"))==0) || (sPreInferiorClass.IsEmpty()))
	{
		bMessageView = false;
	}
	
	if((sPreInferiorClass.Compare(_T("2"))==0) || (sPreInferiorClass.Compare(_T("3"))==0))
	{
		sInferiorClass = sPreInferiorClass;
	}
	
	CESL_DataMgr *pMemberDM = m_pParentMgr->FindDM(sMemberDMAlias);
	if(pMemberDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1150, strFuncName);

	
	ids = pMemberDM->SetCellData(sInferiorClassAlias, sInferiorClass, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
 	if(bMessageView)
 	{
		if(_T("0") == sInferiorClass)
		{
 			
 			CKolas2up_MobileApi *pMobileApi;
 			pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pMemberDM);
 			pMemberDM->InitDBFieldData();
 			pMemberDM->StartFrame();
 			
 			CString strSysDate, strLoanStopSetDate, strType;
 			if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 			{
 				
 				CString strSysDate;
 				CTime t = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strSysDate);
 				strLoanStopSetDate = _T("TO_DATE('") + strSysDate + _T("','YYYY/MM/DD HH24:MI:SS')");
 			}
 			else
 			{
 				strLoanStopSetDate = _T("SYSDATE");
 			}
 			pMemberDM->AddDBFieldData(sInferiorClassDBName, _T("STRING"), sInferiorClass, TRUE);		
 			
 			pMemberDM->AddDBFieldData(_T("LOAN_STOP_DATE"), _T("STRING"), sLoanStopDate, TRUE);
 			
 			if(!sLoanStopDate.IsEmpty())
 			{
 				pMemberDM->AddDBFieldData(_T("LOAN_STOP_SET_DATE"), _T("NUMERIC"), strLoanStopSetDate, TRUE);
 			}
 			
 			CString sLog;
 			pMemberDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("��ü�ϼ���"), __WFILE__, __LINE__), TRUE);
 			ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey); 
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 			ids = pMobileApi->SendFrame();
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 			pMemberDM->EndFrame();
 
  			if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
  			{
  				CString strKLUserYN;
  				ids = pMemberDM->GetCellData(_T("����ȸ������"), 0, strKLUserYN);
  				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1190, strFuncName);
				CString strUserNo;
 				ids = pMemberDM->GetCellData(_T("�����ڹ�ȣ"), 0, strUserNo);
  				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1190, strFuncName);
  				if(_T("Y") == strKLUserYN && !strUserNo.IsEmpty())
  				{
 					
  					CFrameWriter Writer(m_pParentMgr->m_pInfo);
  					Writer.setCommand(_T("IL_K28_LOC_B01_SERVICE"));
  					Writer.addRecord();
 					Writer.addElement(_T("������������ȣ"), m_pParentMgr->m_pInfo->LIB_CODE);
  					Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
 					Writer.addElement(_T("ȸ������"), sInferiorClass);
 					CString strGetData;
 					Writer.addElement(_T("��������������"), strSysDate);
 					ids = pMemberDM->GetCellData(_T("ȸ������"), 0, strGetData);
  					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1190, strFuncName);
 					Writer.addElement(_T("ȸ������"), strGetData);
 					if(!sLoanStopDate.IsEmpty())
 					{
 						if(10 == sLoanStopDate.GetLength())
 						{
 							CString strDate;
 							strDate = sLoanStopDate + _T(" 00:00:00");
 							Writer.addElement(_T("����������"), strDate);
 							Writer.addElement(_T("���������ο���"), strSysDate);
 						}
 					}
  					
 					CFrameSender Sender(m_pParentMgr->m_pInfo);
 					Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("����ȸ��������������������"), __WFILE__, __LINE__));
  				}
  			}
 
 			
 			CLocCommonAPI::MobileApiEndLog(pMobileApi);
 			
 			CString sUpdateField;
 			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
 			
 			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS,  CComVariant(_T("USER_CLASS,LOAN_STOP_DATE")));
 			ShareInfoCenterUpload(_T("N"), _T("U"));
		}
 	}

	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		
		CString strGetData;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����ȸ������"), strGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("Y") == strGetData)
		{
 			
			CString strGetUserNo;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڹ�ȣ"), strGetUserNo, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, _T("CBL_LOC_LOAN_PROC::SearchLoanInfo( CString &sLoanUserKey )"));
			

				if((_T("2") != sInferiorClass || _T("3") != sInferiorClass) && !sInferiorClass.IsEmpty())
				{
					CString strGetData;
					ids = pMemberDM->GetCellData(_T("������������"), 0, strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(_T("Y") == strGetData)
					{
						
						if(_T("1") == m_pManageValue->m_strUserClassShare)
						{	
							bMessageView = TRUE;
							sInferiorClass = _T("1");
							ids = pMemberDM->SetCellData(_T("ȸ������"), sInferiorClass, 0);
							if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
						}
						else if(_T("2") == m_pManageValue->m_strUserClassShare)
						{
							SelfCloseMessageBox(_T("������ �������� ȸ���Դϴ�."));
						}
					}
				}
				m_strUserNo = strGetUserNo;
			
		}
	}
	
	if(bMessageView)
	{
		if(sInferiorClass.Compare(_T("1"))==0) 
		{
			

		}
		else
		{
			SelfCloseMessageBox(_T("����ȸ������ ����Ǿ����ϴ�."));
		}
	}

	
	if(_T("Y") == m_pManageValue->m_sBookCooperativeSys)
	{
		CString strAplStopDate;
		ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("å�η�_��û������"), strAplStopDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(_T("") != strAplStopDate)
		{
			COleDateTime oleAplStopDate;
			int nYear = 1, nMonth = 1, nDay = 1;
			_stscanf(strAplStopDate.GetBuffer(0), _T("%d/%d/%d"), &nYear, &nMonth, &nDay);
			
			oleAplStopDate.SetDate(nYear, nMonth, nDay);

			COleDateTime oleToday = COleDateTime::GetCurrentTime();
			oleToday.SetDate(oleToday.GetYear(), oleToday.GetMonth(), oleToday.GetDay());

			if(oleToday > oleAplStopDate)
			{
				CESL_DataMgr TmpDM;
				TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);		
				
 				CString strQuery;
 				strQuery.Format(
 					_T("UPDATE CO_LOAN_USER_TBL CLU ")
 					_T("SET CLU.ILL_EXPIRE_RETURN_CNT = 0, ")
 						_T("CLU.ILL_APL_STOP_SET_DATE = NULL, ")
 						_T("CLU.ILL_APL_STOP_DATE = NULL ")
 					_T("WHERE REC_KEY = %s AND ")
 						_T("TO_DATE(CLU.ILL_APL_STOP_DATE, 'YYYY/MM/DD') < TO_DATE(SYSDATE, 'YYYY/MM/DD');"),
						sRecKey);
 
 					TmpDM.StartFrame();
 					TmpDM.AddFrame(strQuery);
 					TmpDM.SendFrame();
					TmpDM.EndFrame();
			}
		}
	}

	return 0;
	
EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitDM()
{
EFS_BEGIN

	StartLog(_T("ȭ���ʱ�ȭ:DM�ʱ�ȭ"));

	INT ids;

	const INT nDMCnt = 5;

	CString sDMAlias[nDMCnt] =
	{
		_T("DM_BO_LOC_3100_MEMBER_INFO"),	
		_T("DM_BO_LOC_3100_LOAN_INFO"),	
		_T("DM_BO_LOC_3100_BASKET")	,		
		_T("DM_BO_LOC_3100_RETURN_INFO"),  
		_T("DM_FAMILY_ID_INFO")				
	};

	CESL_DataMgr *pDM;
	for(INT i = 0 ; i < nDMCnt ; i++)
	{
		pDM = m_pParentMgr->FindDM(sDMAlias[i]);
		if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("InitDM"));
		if (m_bReturnBookNonFree && sDMAlias[i].CompareNoCase(_T("DM_BO_LOC_3100_RETURN_INFO")) == 0)
			continue;
		pDM->FreeData();
	}

	EndLog();
	
	CESL_Mgr* pMgr = m_pParentMgr->FindSM(_T("BO_LOC_3100"));
	if(NULL != pMgr)
	{
		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			pMgr->SendMessage(WM_UNITY_USER_ICON_SHOW, (WPARAM)FALSE, 0);
		}
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bCertifyUse) pMgr->SendMessage(WM_CERTIFY_ICON_SHOW, 0, 0);
	}

	
	m_strUserNo = _T("");

	StartLog(_T("ȭ���ʱ�ȭ:ȭ�鱸��"));
	ids = ViewLoanScreen();
	if(0 > ids) return ids;
	EndLog();

	StartLog(_T("ȭ���ʱ�ȭ:�ٱ����ʱ�ȭ"));
	ClearBasket();
	EndLog();
	
	pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)0, 0);
	
	pMgr->SendMessage(WM_USER_INFO_RENEWAL, (WPARAM)FALSE, 0);
	
	if(m_ReceiptDlg != NULL) m_ReceiptDlg->initData();
	
	StartLog(_T("ȭ���ʱ�ȭ:���������ʱ�ȭ"));
	ClearSimpleFamilyMember();
	EndLog();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationLoan()
{
EFS_BEGIN

	INT ids;
	
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(!pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("ReservationLoan"));
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(!pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("ReservationLoan"));
	pGrid->SelectMakeList();

	INT nRowCount = pGrid->SelectGetCount();
	if(!nRowCount)
	{
		SelfCloseMessageBox(_T("������ å�� ������ �ֽʽÿ�"));
		return 0;
	}
	
	BOOL IsLoan = FALSE;	
	ids = IsLoanMember(IsLoan);
	if(0 > ids) return ids;
	if(!IsLoan) return 0;
	
	INT nIndex;
	CString sStatus, sIsAppendix, sRFIDSerial, sRegNo;
	INT nProcessingCnt = 0;
	pGrid->SelectGetHeadPosition();

	CString sValue, sTmp;
	
 	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 	{
 		KL_StartTemp();
 	}
	
	CString sType;
	for (INT i = 0 ; i < nRowCount ; i++)
	{
		nIndex = pGrid->SelectGetIdx();
		
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("�������"), sStatus, nIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("ReservationLoan"));

		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("�ηϿ���"), sIsAppendix, nIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("ReservationLoan"));
			
		if(m_pManageValue->m_sUnmannedReserveSys == _T("Y"))
		{
			CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
			
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("��Ϲ�ȣ"), sRegNo, nIndex);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -7, _T("ReservationLoan"));
			
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("���δ��⿹�����"), sValue, nIndex);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("ReservationLoan"));

			
			if(sStatus.Compare(_T("3")))
			{
				pGrid->SelectGetNext();
				continue;
			}
			
			else if(sStatus == _T("3") && sValue == _T("Y"))
			{
				sTmp.Format(_T("��Ϲ�ȣ�� ['%s']�� �ڷ�� ���δ����û �ڷ��Դϴ�.\r\n")
							 _T("����ó�� �Ͻðڽ��ϱ�?"), sRegNo);
				if(IDYES != MessageBox(m_pParentMgr->m_hWnd, sTmp, _T("���"), MB_ICONWARNING | MB_YESNO))
				{
					pGrid->SelectGetNext();
					continue;
				}
			}
			
			else if(sStatus == _T("3") && sValue == _T("O"))
			{
				sTmp = _T("���δ����� �� ���δ������� �ڷ�� ���� �� �� �����ϴ�.");
				MessageBox(m_pParentMgr->m_hWnd, sTmp, _T("���"), MB_ICONWARNING);
				pGrid->SelectGetNext();
				continue;
			}
		}
		else
		{
			if(sStatus.Compare(_T("3")))
			{
				pGrid->SelectGetNext();
				continue;
			}
		}
		
		CString strBookManageCode;
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("��������"), strBookManageCode, nIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -15, _T("ReservationLoan"));
		
		if(strBookManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE && _T("") != strBookManageCode)
		{
			CString sEMsg;
			
			if(sRegNo.IsEmpty())
			
			{
				sEMsg.Format(_T("Ÿ�� �����ڷ��Դϴ�."));
			}
			else
			{
				sEMsg.Format(_T("��Ϲ�ȣ['%s'] : Ÿ�� �����ڷ��Դϴ�."), sRegNo);
			}
			
			SelfCloseMessageBox(sEMsg);
			pGrid->SelectGetNext();
			continue;
		}
		
		ids = IsReservationMember(nIndex);
		if(0 > ids) return ids;
		if(ids > 0)
		{
			pGrid->SelectGetNext();
			continue;
		}
		
		ids = IsReservationLoan(nIndex);
		if(0 > ids) return ids;
		if(ids > 0)
		{
			pGrid->SelectGetNext();
			continue;
		}				
		
		ids = CheckLoanBookCnt(sIsAppendix);
		if(ids > 0)
		{
			pGrid->SelectGetNext();
			continue;			
		}

		this->SetBEGIN_SBL();		
		
		ids = ReservationDBProc(nIndex);
		if(0 > ids) return ids;
		
		
		if(ids == 100 || ids == 101)
		{
			pGrid->SelectGetNext();
			continue;
		}			
			
		ids = ReservationScreen(nIndex);
		if(0 > ids) return ids;

		nProcessingCnt++;
		pGrid->SelectGetNext();
		
		ids = AppendixLoan(nIndex);
		if(0 > ids) return ids;
				
		
		ids = InitExpDM();
		if(0 > ids) return ids;
	}
 	
 	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 	{
 		KL_EndTemp(m_pParentMgr->GetWorkLogMsg(_T("����ȸ���������"), __WFILE__, __LINE__));
 	}
	if(nProcessingCnt > 0) 
	{
		m_bReturnBookNonFree = TRUE;
		ids = SearchLoanBookInfo();
		if(0 > ids) return ids;
		
		ids = AddMemberInfo();
		if(0 > ids) return ids;
		
		ids = ViewLoanScreen();
		if(0 > ids) return ids;
		
		CString sMsg;
		sMsg.Format(_T("å�� ���� �Ǿ����ϴ�."),nProcessingCnt);
		SelfCloseMessageBox(sMsg);

		
		CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		INT nLoanRowCount = pDM->GetRowCount();
		this->SetEND_SBL(nLoanRowCount);
		
	}

	return 0;

EFS_END
	return -1;
}
INT CBL_LOC_LOAN_PROC::AllControlDisplayGrid()
{
EFS_BEGIN	

	INT ids;
	CString strFuncName = _T("AllControlDisplayGrid");

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("�ڷ�Ǳ���"), _T("�ڷ��"));

	INT nCnt = pDM->GetRowCount();
	CString sData, sDesc;
	
	CString strCurrentTime;
	CTime tCurrent = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strCurrentTime);

	for(INT i = 0; i < nCnt; i++)
	{
		
		sData.Empty();		sDesc.Empty();
		ids = pDM->GetCellData("��������", i, sData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������"), sData, sDesc);
		ids = pDM->SetCellData(_T("�������м���"), sDesc, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		sData.Empty();		sDesc.Empty();
		ids = pDM->GetCellData(_T("����/���ӱ���"), i, sData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		CString strLILLLoanKey;
		pDM->GetCellData(_T("Ÿ������KEY"), i, strLILLLoanKey);
		CString strLILLTran;
		pDM->GetCellData(_T("Ʈ����ǻ���"), i, strLILLTran);

		if(sData.Compare(_T("MO")) == 0)
		{
			sDesc = _T("����");
		}
		else if(sData.Compare(_T("SE")) == 0)
		{
			sDesc = _T("����");
		}
		else
		{
			if(!strLILLLoanKey.IsEmpty())
			{
				sDesc = _T("");
			}
			else
			{
				sDesc = _T("���");
			}

		}

		pDM->GetCellData(_T("��Ȳ����"), i, sData);
		if(_T("����") == sData || _T("����") == sData)
		{
			sDesc = _T("");
		}

		ids = pDM->SetCellData(_T("�ڷᱸ�м���"), sDesc, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		
		sData.Empty();		sDesc.Empty();
		ids = pDM->GetCellData(_T("�ηϿ���"), i, sData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("A") == sData)
		{
			sDesc = _T("�η�");
		}
		else
		{
			sDesc = _T("å");
		}
		ids = pDM->SetCellData(_T("�ηϿ��μ���"), sDesc, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		
		sData.Empty();		sDesc.Empty();
		CString sDelayDay;
		ids = pDM->GetCellData(_T("�������"), i, sData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("��ü�ϼ�"), i, sDelayDay);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		
		if(!strLILLLoanKey.IsEmpty())
		{
			if(_T("0") == strLILLTran)
			{
				sDesc = _T("��û");
			}
			else if(_T("1") == strLILLTran)
			{
				sDesc = _T("�߼�");
			}
			else if(_T("3") == strLILLTran)
			{
				sDesc = _T("�Լ�");
			}
			else if(_T("4") == strLILLTran)
			{
				sDesc = _T("Ÿ������");
			}
			else 
			{
				
				if ( _T("0") == sData || _T("2") == sData ) sDesc = _T("Ÿ������");
				else sDesc = _T("�˼�����");
			}
		}
		else
		{
			if(_T("0") == sData)	
			{
				if(0 < _ttoi(sDelayDay))
				{
					sDesc = _T("��ü");
				}
				else
				{
					sDesc = _T("����");
				}
			}
			else if(_T("1") == sData)
			{
				sDesc = _T("�ݳ�");
			}
			else if(_T("2") == sData)
			{
				sDesc = _T("�ݳ�����");
			}
			else if(_T("3") == sData)
			{
				CString sRecKey, sValue, sReservationExpireDate;
				if(_T("Y") == m_pManageValue->m_sUnmannedReserveSys)
				{
					ids = pDM->GetCellData(_T("���δ��⿹�����"), i, sValue);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
					if(_T("Y") == sValue)
					{
						sDesc = _T("����(���δ����û)");
					}
					else if(_T("O") == sValue)
					{
						ids = pDM->GetCellData(_T("���ุ����"), i, sReservationExpireDate);
						if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
						if(sReservationExpireDate.IsEmpty())
						{
							sDesc = _T("����");
						}
						else
						{
							CTime tExpireDate(_ttoi(sReservationExpireDate.Mid(0,4)), _ttoi(sReservationExpireDate.Mid(5,2)), 
												_ttoi(sReservationExpireDate.Mid(8,2)),_ttoi(sReservationExpireDate.Mid(11,2)), 0, 0); 
							
							if(tExpireDate < CTime(tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(), tCurrent.GetHour(), 0, 0))
							{
								sDesc = _T("����(���δ�������)"); 
							}
							else
							{
								sDesc = _T("����(���δ�����)"); 
							}
						}
					}
					else
					{
						sDesc = _T("����");
					}
				}
				else
				{
					sDesc = _T("����");
				}
			}
			else if(_T("4") == sData)	
			{
				sDesc = _T("�������");
			}
			
			else if(_T("O") == sData) 
			{
				sDesc = _T("å�η�����");
			}
			else if(_T("L") == sData) 
			{
				sDesc = _T("å�η�����");
			}
			else if(_T("D") == sData)	
			{
				sDesc = _T("å�η��ݳ�����");
			}
			else if(_T("R") == sData)	
			{
				sDesc = _T("å�η��ݳ�");
			}
			else
			{
				sDesc = _T("");
			}
		}
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse)
		{
			ids = pDM->GetCellData(_T("��������ȣ��������"), i, sData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			if(_T("Y") == sData)
			{
				sDesc = _T("��������ȣ����");
			}
		}
		
		ids = pDM->SetCellData(_T("������¼���"), sDesc, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
	
	
	
	ids = m_pParentMgr->ControlDisplay(sCMAlias, _T("MainGrid"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	ids = m_pParentMgr->ControlDisplay(sCMAlias, _T("MainGrid2"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewGrid()
{
EFS_BEGIN	

	DWORD nSTART,nFINISH;
	nSTART = GetTickCount();

	ViewGridThread();

	nFINISH = GetTickCount();
	
	CString strMsg;
	strMsg.Format(_T("(Finish) - [%s] �ɸ� �ð� :  [%d.%d sec]"), _T("ViewGrid"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	pDM->QueryLog(strMsg.GetBuffer(0));
	
	return 0;

EFS_END
return -1;
}

UINT RunViewGridThread(LPVOID lParam)
{
	CBL_LOC_LOAN_PROC *pObj = (CBL_LOC_LOAN_PROC *)lParam;
	if(pObj == NULL)
		return -1;
	
	pObj->ViewGridThread();
	return 0;
}

UINT RunLoanBeep(LPVOID lParam)
{
	::Beep(2100,20);	
	return 0;
}
UINT RunReturnBeep(LPVOID lParam)
{
	::Beep(1900,20);	
	return 0;
}
UINT RunErrorBeep(LPVOID lParam)
{
	::Beep(2100,200);
	return 0;
}

INT CBL_LOC_LOAN_PROC::ViewGridThread()
{
EFS_BEGIN

	INT ids;
	CString sStatusDesc;
	CString sBgColor;
	CString sFontColor;
	COLORREF crBgColor, crFontColor;

	InitColorDM();

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");
	CString sGridAlias2 = _T("MainGrid2");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, _T("ViewGridThread"));
	}
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(NULL == pGrid)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, _T("ViewGridThread"));
	}
	CESL_Grid *pGrid2 = (CESL_Grid*)(pCM->FindControl(sGridAlias2));
	if(NULL == pGrid2)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, _T("ViewGridThread"));
	}

	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, _T("ViewGridThread"));
	}
	
	if(0 >= pDM->GetRowCount()) 
	{
		pGrid2->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);

		return 0;
	}

	CString sColorDMAlias = _T("DM_BO_LOC_3100_COLOR_SETTING");
	CESL_DataMgr *pColorDM = m_pParentMgr->FindDM(sColorDMAlias);
	if(NULL == pColorDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, _T("ViewGridThread"));
	}

	pGrid->SetRedraw(FALSE);

	INT nPreRow = pGrid->m_nReverse+1;
	BOOL bFirst = false;

	pColorDM->GetCellData(_T("ȭ��_������"), 0, sBgColor); 
	pGrid->SetBackColorBkg(_ttoi(sBgColor));

	INT nStatusColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, _T("�������"), nStatusColIndex);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, _T("ViewGridThread")); 
	}

	BOOL bGridColorView = TRUE;

	INT nPreCol = pGrid->GetCol();
	for(INT i = 1; i < pDM->GetRowCount() + 1; i++)
	{
		pGrid->SetRow(i);

		CString sType;
		CString sStatus;
		CString sDelayDay;
		ids = pDM->GetCellData(_T("�ηϿ���"), i-1, sType);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, _T("ViewGridThread"));
		}
		ids = pDM->GetCellData(_T("�������"), i-1, sStatus);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, _T("ViewGridThread"));
		}
		ids = pDM->GetCellData(_T("��ü�ϼ�"), i-1, sDelayDay);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1070, _T("ViewGridThread"));
		}

		if(_T("0") == sStatus || _T("����") == sStatus)	
		{
			if(_T("A") == sType)
			{
				pColorDM->GetCellData(_T("�ηϴ���_���ڻ�"), 0, sFontColor); 
				pColorDM->GetCellData(_T("�ηϴ���_������"), 0, sBgColor); 
			}
			else
			{
				pColorDM->GetCellData(_T("����_���ڻ�"), 0, sFontColor); 
				pColorDM->GetCellData(_T("����_������"), 0, sBgColor); 
			}
		}
		else if(_T("1") == sStatus || _T("�ݳ�") == sStatus)	
		{
			pColorDM->GetCellData(_T("�ݳ�_���ڻ�"), 0, sFontColor); 
			pColorDM->GetCellData(_T("�ݳ�_������"), 0, sBgColor); 
		}
		else if(_T("2") == sStatus || _T("�ݳ�����") == sStatus)	
		{
			if(_T("A") == sType)
			{
				pColorDM->GetCellData(_T("�ηϹݳ�����_���ڻ�"), 0, sFontColor); 
				pColorDM->GetCellData(_T("�ηϹݳ�����_������"), 0, sBgColor); 
			}
			else
			{
				pColorDM->GetCellData(_T("�ݳ�����_���ڻ�"), 0, sFontColor); 
				pColorDM->GetCellData(_T("�ݳ�����_������"), 0, sBgColor); 
			}
		}
		else if(_T("3") == sStatus || _T("����") == sStatus)	
		{
			pColorDM->GetCellData(_T("����_���ڻ�"), 0, sFontColor); 
			pColorDM->GetCellData(_T("����_������"), 0, sBgColor); 
		}
		else if(_T("4") == sStatus || _T("�������") == sStatus)	
		{
			pColorDM->GetCellData(_T("�������_���ڻ�"), 0, sFontColor); 
			pColorDM->GetCellData(_T("�������_������"), 0, sBgColor); 
		}
		
		else if(_T("L") == sStatus || _T("å�η�����") == sStatus)
		{
			pColorDM->GetCellData(_T("å�η�����_���ڻ�"), 0, sFontColor);
			pColorDM->GetCellData(_T("å�η�����_������"), 0, sBgColor);

			if(_T("") == sFontColor)
			{
				pColorDM->GetCellData(_T("����_���ڻ�"), 0, sFontColor);
			}
			if(_T("") == sBgColor)
			{
				COLORREF _BK = RGB(250, 200, 127);
				sBgColor.Format(_T("%d"), _BK);
			}
		}
		else if(_T("D") == sStatus || _T("å�η��ݳ�����") == sStatus)
		{
			pColorDM->GetCellData(_T("å�η��ݳ�����_���ڻ�"), 0, sFontColor);
			pColorDM->GetCellData(_T("å�η��ݳ�����_������"), 0, sBgColor);
			if(_T("") == sFontColor) 
			{
				pColorDM->GetCellData(_T("����_���ڻ�"), 0, sFontColor);
			}
			if(_T("") == sBgColor)
			{
				COLORREF _BK = RGB(230, 200, 127);
				sBgColor.Format(_T("%d"), _BK);
			}
		}
		else if(_T("O") == sStatus || _T("å�η�����") == sStatus)
		{
			pColorDM->GetCellData(_T("å�η�����_���ڻ�"), 0, sFontColor);
			pColorDM->GetCellData(_T("å�η�����_������"), 0, sBgColor);
			if(_T("") == sFontColor)
			{
				pColorDM->GetCellData(_T("����_���ڻ�"), 0, sFontColor);
			}
			if(_T("") == sBgColor)
			{
				COLORREF _BK = RGB(210, 200, 127);
				sBgColor.Format(_T("%d"), _BK);
			}
		}
		else
		{
			COLORREF cr = RGB(255, 255, 255);
			sBgColor.Format(_T("%d"), cr);

			cr = RGB(0,0,0);
			sFontColor.Format(_T("%d"), cr);
		}

		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			CString sType;
			pDM->GetCellData(_T("��Ȳ����"), i-1, sType);
			if(_T("����") == sType)
			{
				CString sFontColor2, sBgColor2;
				if(_T("0") == sStatus || _T("����") == sStatus)	
				{
					pColorDM->GetCellData(_T("��������_���ڻ�"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("��������_������"), 0, sBgColor2); 
				}
				else if(_T("2") == sStatus || _T("�ݳ�����") == sStatus)	
				{
					pColorDM->GetCellData(_T("�����ݳ�����_���ڻ�"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("�����ݳ�����_������"), 0, sBgColor2); 
				}
				else if(_T("3") == sStatus || _T("����") == sStatus)	
				{
					pColorDM->GetCellData(_T("��������_���ڻ�"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("��������_������"), 0, sBgColor2); 
				}
				if(!sFontColor2.IsEmpty())
				{
					sFontColor = sFontColor2;
				}
				if(!sBgColor2.IsEmpty())
				{
					sBgColor = sBgColor2;
				}
			}
			else if(_T("����") == sType)
			{
				CString sFontColor2, sBgColor2;
				if(_T("0") == sStatus || _T("����") == sStatus)	
				{
					pColorDM->GetCellData(_T("���ʹ���_���ڻ�"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("���ʹ���_������"), 0, sBgColor2); 
				}
				else if(_T("2") == sStatus || _T("�ݳ�����") == sStatus)	
				{
					pColorDM->GetCellData(_T("���͹ݳ�����_���ڻ�"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("���͹ݳ�����_������"), 0, sBgColor2); 
				}
				else if(_T("3") == sStatus || _T("����") == sStatus)	
				{
					pColorDM->GetCellData(_T("���Ϳ���_���ڻ�"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("���Ϳ���_������"), 0, sBgColor2); 
				}
				if(!sFontColor2.IsEmpty())
				{
					sFontColor = sFontColor2;
				}
				if(!sBgColor2.IsEmpty())
				{
					sBgColor = sBgColor2;
				}
			}
			
			
			CString sOtherLoanKey;
			CString sFontColor3, sBgColor3;
			pDM->GetCellData(_T("Ÿ������KEY"), i-1, sOtherLoanKey);
			CString strLILLTran;
			pDM->GetCellData(_T("Ʈ����ǻ���"), i-1, strLILLTran);
			if(!sOtherLoanKey.IsEmpty())
			{
				if(_T("0") == strLILLTran)	
				{
					pColorDM->GetCellData(_T("��û_���ڻ�"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("��û_������"), 0, sBgColor3); 
				}
				else if(_T("1") == strLILLTran)	
				{
					pColorDM->GetCellData(_T("�߼�_���ڻ�"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("�߼�_������"), 0, sBgColor3); 
				}
				else if(_T("3") == strLILLTran)	
				{
					pColorDM->GetCellData(_T("�Լ�_���ڻ�"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("�Լ�_������"), 0, sBgColor3); 
				}
				else if(_T("4") == strLILLTran || _T("1") == sStatus )
				{
					pColorDM->GetCellData(_T("Ÿ������_���ڻ�"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("Ÿ������_������"), 0, sBgColor3); 
				}
				else if(_T("4") == strLILLTran || _T("2") == sStatus )
				{
					pColorDM->GetCellData(_T("Ÿ������_�ݳ�����_���ڻ�"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("Ÿ������_�ݳ�����_������"), 0, sBgColor3); 
				}
				else 
				{
					pColorDM->GetCellData(_T("����_���ڻ�"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("����_������"), 0, sBgColor3); 
				}
				if(!sFontColor3.IsEmpty())
				{
					sFontColor = sFontColor3;
				}
				if(!sBgColor3.IsEmpty())
				{
					sBgColor = sBgColor3;
				}
			}

		}
		
		if(0 < _ttoi(sDelayDay)	&& 
		
			(_T("0") == sStatus || _T("����") == sStatus || _T("2") == sStatus || _T("�ݳ�����") == sStatus
			|| _T("L") == sStatus || _T("å�η�����") == sStatus || _T("D") == sStatus || _T("å�η��ݳ�����") == sStatus))
		{
			pColorDM->GetCellData(_T("��ü_���ڻ�"), 0, sFontColor); 
			pColorDM->GetCellData(_T("��ü_������"), 0, sBgColor);
		}

		if(!bGridColorView)
		{
			continue;
		}
		
		crFontColor = _ttoi(sFontColor);
		crBgColor = _ttoi(sBgColor);

		INT cols;
		cols = pGrid->GetCols(0);

		if(pGrid->m_nReverse+1 != i)
		{
			pGrid->SetCol(1);
			pGrid->GetCellFontBold();
			if(crBgColor != pGrid->GetCellBackColor() || (pGrid->GetCellFontBold())) 
			{				
				for (INT j = 1 ; j < cols ; j++) 
				{
					pGrid->SetCol(j);
					pGrid->SetCellBackColor(crBgColor);
					pGrid->SetCellForeColor(crFontColor);
					pGrid->SetCellFontBold(FALSE);
					pGrid->SetCellFontItalic(FALSE);					
					ids = SetColumnColor(pDM, pGrid, i-1, j);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("ViewGridThread"));
					}
				}
			}
			if(i == pDM->GetRowCount())
			{
				bFirst = true;
				i = nPreRow-1;
				if(i < 0)
				{	
					for (INT j = 1 ; j < cols ; j++) 
					{
						pGrid->SetCol(j);
						ids = SetColumnColor(pDM, pGrid, i+1, j);
						if(0 > ids)
						{
							ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("ViewGridThread"));
						}
					}

					pGrid->SetRedraw(TRUE);
					pGrid->Refresh();
					pGrid2->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
		
					return 0;
				}
			}
		}
		else
		{
			if(!bFirst && (i != pDM->GetRowCount())) 
			{
				continue;			
			}
			if(crBgColor != pGrid->GetCellBackColor() || (!pGrid->GetCellFontBold())) 
			{
				for (INT k = cols-1 ; k > 0 ; k--)
				{
					pGrid->SetCol(k);
					pGrid->SetCellForeColor(crFontColor);
					pGrid->SetCellBackColor(crBgColor);
					pGrid->SetCellFontBold(TRUE);
					
					ids = SetColumnColor(pDM, pGrid, i-1, k);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("ViewGridThread"));
					}
				}
			}			
			pGrid->SetCol(nPreCol);
			pGrid->SetCellBackColor(crBgColor);
			pGrid->SetRedraw(TRUE);
			pGrid->Refresh();		
			
			pGrid2->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);

			return 0;
		}
	}
	pGrid->Refresh();
	pGrid->SetRedraw(TRUE);
	
	pGrid2->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
		
	if(m_ReceiptDlg != NULL)
	{		
		m_pParentMgr->PostMessage(WM_RECEIPT_CHANGE_TAB, 0, 1);
	}

	return 0;

EFS_END

return -1;
}


INT CBL_LOC_LOAN_PROC::IsReservationLoan(INT nIndex)
{
EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sBookRecKey; 
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("åKEY"), sBookRecKey, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("IsReservationLoan"));
	CString sPublishForm;	
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("����/���ӱ���"), sPublishForm, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("IsReservationLoan"));

	CString sBookDMAlias;	
	CString sWhere;			
	CString sRightStatus;	
	if(sPublishForm.Compare(_T("MO")) == 0)
	{
		sBookDMAlias = _T("DM_EXP_LOAN_BO_BOOK_INFO");
		sWhere.Format(_T("BB.REC_KEY = '%s' AND ")
						_T("BB.WORKING_STATUS IN ")
						_T("(")
						_T(HOLD_SEARCH_CONDITION)
						_T(",'BOL217O','BOL218O','BOL219O','BOL411O')"),
						sBookRecKey);
		sRightStatus = _T("BOL112N");
	}
	else
	{
		sBookDMAlias = _T("DM_EXP_LOAN_SE_BOOK_INFO");
		sWhere.Format(_T("IB.REC_KEY = '%s' AND ")
						_T("IB.WORKING_STATUS IN ")
						_T("(")
						_T(HOLD_SE_SEARCH_CONDITION)
						
						_T(",'SEL317O','SEL318O','SEL411O')"),
						sBookRecKey);
		sRightStatus = _T("SEL212N");
	}


	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sBookDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("IsReservationLoan"));
	
	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pDM, sWhere);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("IsReservationLoan"));

	INT nBookCnt = pDM->GetRowCount();

	CString sWorkingStatus;
	ids = pDM->GetCellData(_T("����"), 0, sWorkingStatus);	
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("IsReservationLoan"));

	CString sRegNo;
	ids = pDM->GetCellData(_T("��Ϲ�ȣ"), 0, sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -6, _T("IsReservationLoan"));

	if(sRightStatus.Find(sWorkingStatus) < 0 )
	{
		CString sMsg;
		sMsg.Format(_T("���� �����Ͻ� '%s'�� å���°� ���� ���� ���°� �ƴϾ �� å�� ���� �� �� �����ϴ�."), sRegNo);
		SelfCloseMessageBox(sMsg);
		return 1;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsReservationMember(INT nIndex)
{
EFS_BEGIN

	INT ids = 0;
	CString sRegNo = _T("");
	CString sBookRecKey = _T("");
	CString sLoanRecKey = _T("");
	CString sWhere = _T("");
	CString sCurrentLoanRecKey = _T("");
	CString sMsg = _T("");

	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("IsReservationMember"));
	
	ids = pDM->GetCellData(_T("��Ϲ�ȣ"), nIndex, sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("IsReservationMember"));
	
	if((m_pManageValue->m_bVolReserve) && sRegNo.IsEmpty())
	{
		sMsg.Format(_T("%d��° �׸��� �����ڷ������� ���� ���ֽʽÿ�."), nIndex+1);
		SelfCloseMessageBox(sMsg);
		return 1;
	}

	if(!(m_pManageValue->m_bVolReserve))
	{
		ids = pDM->GetCellData(_T("åKEY"), nIndex, sBookRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("IsReservationMember"));

		sWhere.Format(_T(" SELECT REC_KEY FROM LS_WORK_T01"
						_T(" WHERE")
						_T(" RESERVATION_DATE = (SELECT MIN(RESERVATION_DATE) FROM LS_WORK_T01 WHERE STATUS='3' AND BOOK_KEY=%s)")
						_T(" AND STATUS='3' AND BOOK_KEY = %s"))
						, sBookRecKey, sBookRecKey);
		ids = CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pDM, sWhere, sLoanRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("IsReservationMember"));

		ids = pDM->GetCellData(_T("����/�ݳ�����KEY"), nIndex, sCurrentLoanRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("IsReservationMember"));

		if(sLoanRecKey.Compare(sCurrentLoanRecKey) != 0)
		{
			sMsg.Format(_T("��Ϲ�ȣ�� '%s'�� å�� ���� �ٸ� ����ڰ� ���� ������ �س��� �����Դϴ�."), sRegNo);
			SelfCloseMessageBox(sMsg);
			return 1;
		}
	}

	else
	{

	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::ReservationDBProc(INT nIndex, BOOL bIsILLMode)
{
EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("ReservationDBProc"));

	CKolas2up_MobileApi *pMobileApi;
	pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

	ids = UpdateLoanRecord(nIndex, pMobileApi, bIsILLMode);
	
	if(ids == 100 || ids == 101) return ids;
	
	if(ids != 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("ReservationDBProc"));

	if(FALSE == bIsILLMode)
	{
		ids = UpdateBookInfo(nIndex, pMobileApi);
		if(ids != 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("ReservationDBProc"));
	}
	else
	{
		CString strLoanKey;
		CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		pDM->GetCellData(_T("����/�ݳ�����KEY"), nIndex, strLoanKey);
		ILL_UpdateTransInfo(FALSE, strLoanKey);
	}
	
	ids = UpdateLoanUserUpdate(nIndex, pMobileApi);
	if(ids != 0) return ids;
	
	ids = pMobileApi->SendFrame();
	if(0 > ids) 
	{
		pDM->ExecuteQuery(_T("rollback;"), FALSE, FALSE);	
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("ReservationDBProc"));
	}

	pMobileApi->EndFrame();

	CLocCommonAPI::MobileApiEndLog(pMobileApi);
	
	ShareInfoCenterUpload(_T("U"), _T("U"));	
	
	return 0;

EFS_END
	return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateLoanRecord(INT nIndex, CKolas2up_MobileApi *pMobileApi, BOOL bIsILLMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateLoanRecord");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");	
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	pDM->InitDBFieldData();
	pDM->StartFrame();
	
	CString sRecKey;
	CString sSrcDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");	
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("����/�ݳ�����KEY"), sRecKey, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sRegCode;
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("��Ϲ�ȣ"), sRegCode, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;

	sRegCode = sRegCode.Left(nRegCodeLength);
	
	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;

	ids = pDM->AddDBFieldData(_T("LOAN_TYPE_CODE"), _T("STRING"), sLoanType, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString strAppendYN, strLoanKey;
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("�ηϿ���"), strAppendYN, nIndex);
	if(_T("A")==strAppendYN)
	{
		strLoanKey = _T("1");
		ids = pDM->AddDBFieldData(_T("LOAN_KEY"), _T("NUMERIC"), strLoanKey, 1);
	}

	CString sReturnExpDate;	

	ids = GetReturnExpDate(sReturnExpDate, sRegCode);
	
	
	if(ids == 100 || ids == 101) return ids;
	
	if(0 > ids) return ids;

	
	CString sTempDate;
	if(!sReturnExpDate.IsEmpty())
	{
		sTempDate = _T("TO_DATE('")+sReturnExpDate+_T(" 23:59:59")+_T("', 'YYYY/MM/DD HH24:MI:SS')");
	}
	else
	{
		sTempDate = _T("NULL");
	}
	ids = pDM->AddDBFieldData(_T("RETURN_PLAN_DATE"), _T("NUMERIC"), sTempDate, TRUE);
	
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("�ݳ�������"), sReturnExpDate, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sLoanStatus = (TRUE != bIsILLMode) ? _T("0") : _T("L");

	ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), sLoanStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	CTime tLoanDate = CTime::GetCurrentTime();
	
	CString sLoanDate;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("�ұ޴�����"), sLoanDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString strLoanDateString;
	strLoanDateString.Format(_T("%s %02d:%02d:%02d"), sLoanDate, tLoanDate.GetHour(), tLoanDate.GetMinute(), tLoanDate.GetSecond());
	CString sInLoanDate;
	sInLoanDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strLoanDateString);
	
	ids = pDM->AddDBFieldData(_T("LOAN_DATE"), _T("NUMERIC"), sInLoanDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("�������"), sLoanStatus, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString strDMLoanDate;
	strDMLoanDate.Format(_T("%s %02d:%02d"), sLoanDate, tLoanDate.GetHour(), tLoanDate.GetMinute());
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("������"), strDMLoanDate, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(_T("Y") == m_pManageValue->m_sUseReceipt)
	{
		ids = pDM->AddDBFieldData(_T("RECEIPT_PRINT_LOAN_YN"), _T("STRING"), _T("N"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = pDM->AddDBFieldData(_T("RECEIPT_PRINT_RETURN_YN"), _T("STRING"), _T("N"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	

	if(sLoanType.Compare(_T("0")) == 0)		sLoanType = _T("�Ϲ�");
	else if(sLoanType.Compare(_T("1")) == 0)	sLoanType = _T("Ư��");
	else if(sLoanType.Compare(_T("2")) == 0)	sLoanType = _T("����");
	else if(sLoanType.Compare(_T("3")) == 0)	sLoanType = _T("����");
	else if(sLoanType.Compare(_T("4")) == 0)	sLoanType = _T("���");
	else										sLoanType = _T("�Ϲ�");

	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("��������"), sLoanType, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	CString sGetData = _T("");
	CString sTotalReserveCnt = _T("");
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("�����̿��ڼ�"), sGetData, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(_ttoi(sGetData)<1)	sTotalReserveCnt = _T("0");
	else					sTotalReserveCnt.Format(_T("%d"), (_ttoi(sGetData)-1));
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("�����̿��ڼ�"), sTotalReserveCnt, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("�������"), __WFILE__, __LINE__), TRUE);

 	
 	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
		m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_DEVICE"), _T("KOLASIII-�̵�������"), nIndex);
	}
	else
	{
 		m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_DEVICE"), _T("KOLASIII"), nIndex);
	}
 	
 	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
 		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII-�̵�������"));
	}
	else
	{
		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII"));
	}
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_WORKER"), m_pParentMgr->m_pInfo->USER_ID, nIndex);
	pMobileApi->AddDBFieldData(_T("L_WORKER"), _T("STRING"), m_pParentMgr->m_pInfo->USER_ID);

	
	
	
	ids = pMobileApi->MakeUpdateFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	pDM->InitDBFieldData();

	Send3000ApiSocket(_T("����"), sRecKey);
 	
 	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 	{
 		
 		CString strGetData;
		CString sType;
 		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����ȸ������"), strGetData, 0);
 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 		if(_T("Y") == strGetData)
 		{
			CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
			if(NULL == pLoanDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 			CString strLoanLibCode;
			ids = pLoanDM->GetCellData(_T("��Ȳ����"), nIndex, sType);
 			ids = pLoanDM->GetCellData(_T("���⵵������ȣ"), nIndex, strLoanLibCode);
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 			if(strLoanLibCode.IsEmpty() || strLoanLibCode == m_pParentMgr->m_pInfo->LIB_CODE)
			{
				CFrameWriter Writer(m_pParentMgr->m_pInfo);
 				Writer.setCommand(_T("IL_K10_LOC_B01_SERVICE"));
 				Writer.addRecord();
				
				CString strGpinHash;
				m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("G-PINHASH�����ڵ�"), strGpinHash, 0);
				Writer.addElement(_T("G-PINHASH�����ڵ�"), strGpinHash);
				Writer.addElement(_T("���⵵����"), m_pParentMgr->m_pInfo->LIB_CODE);
 				CString strRegNo;
 				ids = pLoanDM->GetCellData(_T("��Ϲ�ȣ"), nIndex, strRegNo);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("��Ϲ�ȣ"), strRegNo);
 				CString strUserNo;
 				ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڹ�ȣ"), strUserNo, 0);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
				Writer.addElement(_T("������"), strLoanDateString);
				Writer.addElement(_T("����������"), strLoanDateString);
				CString strReturnPlanDate;
				if(!sReturnExpDate.IsEmpty())
				{
					strReturnPlanDate = sReturnExpDate+_T(" 23:59:59");
				}
 				Writer.addElement(_T("�ݳ�������"), strReturnPlanDate);
 				Writer.addElement(_T("����"), sLoanStatus);
				ids = pLoanDM->GetCellData(_T("��ǥ��"), nIndex, strGetData);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("����"), strGetData);
				ids = pLoanDM->GetCellData(_T("û����ȣ"), nIndex, strGetData);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("û����ȣ"), strGetData);
				ids = pLoanDM->GetCellData(_T("IBS_û��_��å��ȣ"), nIndex, strGetData);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("�����"), strGetData);
				Writer.addElement(_T("����KEY"), sRecKey);
				
				ids = pLoanDM->GetCellData(_T("�ηϿ���"), nIndex, strGetData);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("�ηϿ���"), strGetData);

				CESL_DataMgr tempDM;
				tempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
				CString strQuery;
				strQuery.Format(_T("SELECT AUTHOR,PUBLISHER FROM LS_WORK_T01 WHERE REC_KEY=%s"), sRecKey);
				tempDM.RestructDataManager(strQuery);
				tempDM.GetCellData(0, 0, strGetData);
				Writer.addElement(_T("����"), strGetData);
				tempDM.GetCellData(0, 1, strGetData);
				Writer.addElement(_T("������"), strGetData);				
  				CString strFrame = Writer.getFrame();
  				KL_AddTemp(strFrame);
 			}
 		}
 	}
	
	SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
	SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_TYPE_CODE,RETURN_PLAN_DATE,STATUS,LOAN_DATE,L_DEVICE,L_WORKER")));
	
	
	return 0;

EFS_END
	return -1;
}


INT CBL_LOC_LOAN_PROC::UpdateBookInfo(INT nIndex, CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateBookInfo");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();

	CString sTableName;
	CString sWorkingStatus;

	CString sLoanInfoDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("����/���ӱ���"), sTableName, nIndex);
	
	INT nBookType = 0;

	if(sTableName.Compare(_T("MO"))== 0)
	{
		sTableName = _T("BO_BOOK_TBL");
		ids = GetBOWorkingStatus(sWorkingStatus);
		if(0 > ids) return ids;

		nBookType = 1;
	}
	else
	{
		sTableName = _T("SE_BOOK_TBL");
		ids = GetSEWorkingStatus(sWorkingStatus);
		if(0 > ids) return ids;

		nBookType = 2;
	}

	
	CString sRecKey;
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("åKEY"), sRecKey, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
#ifdef __K2UP__
	CString sPreWorkingStatus;
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("����"), sPreWorkingStatus, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), sPreWorkingStatus, TRUE);
#endif

	ids = pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), sWorkingStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("�����۾�"), __WFILE__, __LINE__), TRUE);

	ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	pDM->InitDBFieldData();
	
	CString sSpeciesKey;
	m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("��KEY"), sSpeciesKey, 0);
	AddIdxQueryFrame(pDM, nBookType, sSpeciesKey);
	
	ids = m_pParentMgr->SetDataMgrData(sLoanInfoDMAlias, _T("����"), sWorkingStatus, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateLoanUserUpdate(INT nIndex, CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateLoanUserUpdate");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	pDM->InitDBFieldData();
	
	CString sRecKey;
	CString sLoanBookCnt;
	CString sReservationCnt;
	CString sDMUserInfoAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("������KEY"), sRecKey, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("�Ѵ���å��"), sLoanBookCnt, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("�ѿ���å��"), sReservationCnt, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);

	sLoanBookCnt.Format(_T("%d"), _ttoi(sLoanBookCnt) + 1);

	if(1 > _ttoi(sLoanBookCnt))
	{
		sLoanBookCnt = _T("0");
	}
	ids = pDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sLoanBookCnt, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
	
	CString sLastLoanDate = _T("SYSDATE");
	ids = pDM->AddDBFieldData(_T("LAST_LOAN_DATE"), _T("NUMERIC"), sLastLoanDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, strFuncName);
	
	INT nReservationCnt = _ttoi(sReservationCnt);
	nReservationCnt = (0 < nReservationCnt) ? nReservationCnt-1 : nReservationCnt;
	sReservationCnt.Format(_T("%d"), nReservationCnt);
	
	ids = pDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sReservationCnt, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1070, strFuncName);

	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("�������"), __WFILE__, __LINE__), TRUE);

	
	ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, strFuncName);

	pDM->InitDBFieldData();

	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_COUNT,LAST_LOAN_DATE,RESERVATION_COUNT")));
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationScreen(INT nIndex)
{
EFS_BEGIN

	INT ids;

	ids = UpdateLoanInfoDMToCM(nIndex);
	if(0 > ids) return ids;

	ids = AddMemberInfo();
	if(0 > ids) return ids;

	ids = AllControlDisplayNotGrid(0);
	if(0 > ids) return ids;
	
	AllControlDisplayGrid();
	ViewGrid();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateLoanInfoDMToCM(INT nIndex)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateLoanInfoDMToCM");

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));

	const INT nFieldCnt = 4;
	TCHAR *sFieldAlias[nFieldCnt] =
	{
		_T("������"), _T("�ݳ�������"), _T("�������"), _T("�������")
	};
	
	INT nCol[nFieldCnt] =
	{
		-1, -1, -1, -1
	};

	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, sFieldAlias, nFieldCnt, nCol);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName); 

	CString sData;
	for(INT i = 0 ; i < 3 ;i++)
	{
		ids = pDM->GetCellData(sFieldAlias[i], nIndex, sData);

		if(nCol[i] !=  -1)
		{
			ids = pGrid->SetAt(nIndex, nCol[i], sData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
		}
		
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetReturnExpDate(CString &sReturnExpDate, CString sRegCode)
{
EFS_BEGIN

	INT ids;
	
	CString sLoanDate;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("�ұ޴�����"), sLoanDate);
	
	CTime tReturnExpDate(_ttoi(sLoanDate.Mid(0,4)), _ttoi(sLoanDate.Mid(5,2)), _ttoi(sLoanDate.Mid(8,2)), 0, 0, 0);
	
	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;
	
	INT nExpDay;
	ids = m_pCheckLoan->GetReturnExpDay(nExpDay, sRegCode, tReturnExpDate);
	
	if(nExpDay == 0)
	{
		if(_T("1") != sLoanType)
		{
			SelfCloseMessageBox(_T("��������� '0'���� �ڷ�� ������ �� �����ϴ�. \r\n�����ϱ� ���ؼ��� �������� �������ֽʽÿ�"));
			return 101;
		}
		else
		{
			ids = m_pCheckLoan->GetReturnExpDay(nExpDay, sRegCode, tReturnExpDate, FALSE);
		}
	}
	
	if(sLoanType.Compare(_T("2")) == 0) 
		nExpDay = 0;
	
	else if(sLoanType.Compare(_T("4")) == 0) 
	{		
		CBO_LOC_LOAN_DATE dlg;
		dlg.SetLoanExpDate(nExpDay);
		if(dlg.DoModal() == IDOK)
			nExpDay = dlg.GetLoanExpDate();
		else 
		{
			SelfCloseMessageBox(_T("�۾��� ����Ͽ����ϴ�."));
			return 100;
		}
	}
	
	CTimeSpan tExpDay(nExpDay, 0, 0, 0);
	tReturnExpDate += tExpDay;
	sReturnExpDate = tReturnExpDate.Format(_T("%Y/%m/%d"));
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsReservatedBook(CString &sBookRecKey, CString &sSpeciesKey, CString &sVolInfoKey, CString &sReserveBookCnt, CString &sVolReserveCnt, INT nIndex)
{
EFS_BEGIN

	INT ids = 0;
	CString strFuncName = _T("IsReservatedBook");

	CString sWhere = _T("");
	
	CString sDMAlias;
	sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	if(nIndex == -1)
	{
		nIndex = 0;

		ids = pDM->GetCellData(_T("����Ǽ�"), nIndex, sReserveBookCnt);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
	
	else
	{	
		CESL_DataMgr *pDM_Loan = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		CString strOtherLoanYN;
		ids = pDM_Loan->GetCellData(_T("Ÿ������KEY"),nIndex,strOtherLoanYN);
		if(!strOtherLoanYN.IsEmpty())
		{
		}
		else
		{
		if(!(m_pManageValue->m_bVolReserve))
			sWhere.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS='3'"), sBookRecKey);
		else if((m_pManageValue->m_bVolReserve))
		{
			sWhere.Format(	_T(" SELECT COUNT(1) FROM LS_WORK_T01"
							   _T(" WHERE SPECIES_KEY=%s AND STATUS='3' AND REG_NO IS NULL AND VOL_INFO")), sSpeciesKey);
			if(sVolInfoKey.IsEmpty())
				sWhere += _T(" IS NULL");
			else
				sWhere += _T(" = ") + sVolInfoKey;
		}
		ids = CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pDM, sWhere, sVolReserveCnt);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
	}

	if((m_pManageValue->m_bVolReserve))
	{
		sWhere.Format(_T(" SELECT COUNT(1) FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS='3' AND REG_NO IS NOT NULL"), sBookRecKey);
		ids = CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pDM, sWhere, sReserveBookCnt);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
		

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetDelayLoanMode(INT &nDelayMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetDelayLoanMode");

	ids = m_pCheckLoan->GetDelayLoanMode(nDelayMode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::LoanReservationInSearchBookList(CString strRegNo, CString strPublisherFormCode, INT nMode, BOOL IsReqPrior)
{
EFS_BEGIN
	
	INT	ids;
	CString strFuncName = _T("LoanReservationInSearchBookList");

	ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), strRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	ids = IsSearchedUser();
	if(0 > ids) return ids;
	if(ids > 0)
	{
		if(nMode == 11)
			MoveRegNoToNextProcBasket(EXP_LOAN);
		else if(nMode == 21)
			MoveRegNoToNextProcBasket(EXP_RESERVE);

		return 0;
	}

	if(nMode == 11)
	{
		ids = SPFExcutePath(_T("����"));
		if(0 > ids) return ids;
	}
	
	else if(nMode == 21)
	{		
		m_IsReqPrior = IsReqPrior;
		
		ids = SPFExcutePath(_T("����"));
		if(0 > ids) return ids;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchBookInfo()
{
EFS_BEGIN

	INT ids;
	CString sRegNo,sPublishForm;
	ids = SearchUserBook(0, sRegNo, sPublishForm);
	if(0 > ids) return ids;

	
	if(ids == 3) return 0;
	
	if(ids == 1) return 0;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo(CString &sWhere, INT nMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetSqlSearchBookInfo");

	INT nSearchField = 0;

	CString sCMAlias = _T("CM_BO_LOC_3110");
	const INT nFieldCnt = 4;
	
	TCHAR *sFieldAlias[nFieldCnt] = 
	{
		_T("å����"), _T("���ڸ�"), _T("�����ڸ�"), _T("��ü")
	};

	CString sGetData[nFieldCnt];

	for(INT i = 0 ; i < nFieldCnt ;i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sFieldAlias[i], sGetData[i]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		sGetData[i].TrimRight();
	}

	CMakeSearchData MakeSearchData(m_pParentMgr);

	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("rb"));

	CString sEcoSearchWhere;	
	INT nEcoSearchCnt = 0;
		
	BOOL bIsEqualSearch;
	if(!sGetData[0].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
		
		bIsEqualSearch = FALSE;
		
		if(sGetData[0].GetAt(0) == '\"')
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sGetData[0]);
		if(!bIsEqualSearch) 
			sGetData[0] += _T("*");
		
		if(1 == m_pParentMgr->m_pInfo->GetSearchIndexType())
		{
			if(fd != NULL)
			{
				
				if(nMode == 0)
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, BO_IDX_TITLE, '") + sGetData[0] + _T("') ");
				else
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, SE_IDX_TITLE, '") + sGetData[0] + _T("') ");
			}
			else
			{
				if(nMode == 0)
					sEcoSearchWhere += _T(" &ECOSEARCH(, BO_IDX_TITLE, '") + sGetData[0] + _T("') ");
				else
					sEcoSearchWhere += _T(" &ECOSEARCH(, SE_IDX_TITLE, '") + sGetData[0] + _T("') ");
				
				nEcoSearchCnt++;
				
			}
		}
		else
			sWhere += _T(" CATSEARCH(IB.IDX_TITLE, '") + sGetData[0] + _T("',NULL) > 0 ");

		nSearchField++;
	}

	if(!sGetData[1].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");

		bIsEqualSearch = FALSE;
		
		if(sGetData[1].GetAt(0) == '\"')
			bIsEqualSearch = TRUE;
		MakeSearchData.GetKeyWord(sGetData[1]);
		
		if(!bIsEqualSearch) 
			sGetData[1] += _T("*");

		if(1 == m_pParentMgr->m_pInfo->GetSearchIndexType())
		{
			if(fd != NULL)
			{
				
				if(nMode == 0)
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, BO_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
				else
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, SE_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
				
			}
			else
			{
				if(!sEcoSearchWhere.IsEmpty())
					sEcoSearchWhere += _T(" INTERSECT ");
				
				if(nMode == 0)
					sEcoSearchWhere += _T(" &ECOSEARCH(, BO_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
				else
					sEcoSearchWhere += _T(" &ECOSEARCH(, SE_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
				
				nEcoSearchCnt++;
			}
			
		}
		else
			sWhere += _T(" CATSEARCH(IB.IDX_AUTHOR, '") + sGetData[1] + _T("',NULL) > 0 ");

		nSearchField++;
	}

	if(!sGetData[2].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");

		bIsEqualSearch = FALSE;
		
		if(sGetData[2].GetAt(0) == '\"')
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sGetData[2]);
		if(!bIsEqualSearch) 
			sGetData[2] += _T("*");
		if(1 == m_pParentMgr->m_pInfo->GetSearchIndexType())
		{
			
			if(fd != NULL)
			{
				if(nMode == 0)
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, BO_IDX_PUBLISHER, '") + sGetData[2] + _T("') ");
				else
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, SE_IDX_PUBLISHER, '") + sGetData[2] + _T("') ");
			}
			else
			{
				if(!sEcoSearchWhere.IsEmpty())
					sEcoSearchWhere += _T(" INTERSECT ");
				
				if(nMode == 0)
					sEcoSearchWhere += _T(" &ECOSEARCH(, BO_IDX_PUBLISHER, '") + sGetData[2] + _T("') ");
				else
					sEcoSearchWhere += _T(" &ECOSEARCH(, SE_IDX_PUBLISHER, '") + sGetData[2] + _T("') ");
				
				nEcoSearchCnt++;
			}
		}
		else			
			sWhere += _T(" CATSEARCH(IB.IDX_PUBLISHER, '") + sGetData[2] + _T("',NULL) > 0 ");
		
		nSearchField++;
	}

	
	if(!sGetData[3].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");

		bIsEqualSearch = FALSE;
		
		if(sGetData[3].GetAt(0) == '\"')
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sGetData[3]);
		if(!bIsEqualSearch) 
			sGetData[3] += _T("*");
		if(1 == m_pParentMgr->m_pInfo->GetSearchIndexType())
		{
			
			if(fd != NULL)
			{
				if(nMode == 0)
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, BO_IDX_ALLITEM, '") + sGetData[3] + _T("') ");
				else
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, SE_IDX_ALLITEM, '") + sGetData[3] + _T("') ");
			}
			else
			{
				if(!sEcoSearchWhere.IsEmpty())
					sEcoSearchWhere += _T(" INTERSECT ");
				
				if(nMode == 0)
					sEcoSearchWhere += _T(" &ECOSEARCH(, BO_IDX_ALLITEM, '") + sGetData[3] + _T("') ");
				else
					sEcoSearchWhere += _T(" &ECOSEARCH(, SE_IDX_ALLITEM, '") + sGetData[3] + _T("') ");
				
				nEcoSearchCnt++;
			}
		}
		else			
			sWhere += _T(" CATSEARCH(IB.IDX_ALL_ITEM, '") + sGetData[3] + _T("',NULL) > 0 ");
		
		nSearchField++;
	}

	
	const INT nComboFieldCnt = 3;
	TCHAR *sComboAlias[nComboFieldCnt] =
	{
		_T("�ڷ�����"), _T("��ü����"), _T("�ڷ��")
	};

	TCHAR *sDBFieldAlias[ nComboFieldCnt ] =
	{
		_T("IB.FORM_CODE"), _T("IB.MEDIA_CODE"), _T("BB.SHELF_LOC_CODE")
	};

	CString sComboGetData[nComboFieldCnt];

	for(i = 0 ; i < nComboFieldCnt ; i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sComboAlias[i], sComboGetData[i]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		sComboGetData[i].TrimRight();
		if(sComboGetData[i].IsEmpty() || (sComboGetData[i].Compare(_T("0"))==0) || (sComboGetData[i].Compare(_T("�������"))==0)) continue;
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
		sWhere += sDBFieldAlias[i] + CString(_T(" = '")) + sComboGetData[i] + CString(_T("' "));
	}	
	
	if(nMode == 1)
	{ 
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
		
		sWhere += _T(" BB.BINDED_BOOK_YN != 'Y' ");
	}	
	
	const INT nKeyWordFieldCnt = 5;
	TCHAR *sKeyWordAlias[nKeyWordFieldCnt] =
	{
		_T("Ű����1"), _T("Ű����2"), _T("Ű����3"), _T("Ű����4"), _T("Ű����5")
	};
	CString sKeyWordGetData[nKeyWordFieldCnt];

	for(i = 0 ; i < nKeyWordFieldCnt ; i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sKeyWordAlias[i], sKeyWordGetData[i]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		sKeyWordGetData[i].TrimRight();
	}
	
	CString sKeyWordWhere;
	if(!sKeyWordGetData[0].IsEmpty())
	{
		if(fd != NULL)
			sWhere += _T("(");

		bIsEqualSearch = FALSE;
		
		if(sKeyWordGetData[0].GetAt(0) == '\"')
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sKeyWordGetData[0]);
		if(!bIsEqualSearch) 
			sKeyWordGetData[0] += _T("*");
		sKeyWordWhere = sKeyWordGetData[0];

		if(!sKeyWordGetData[2].IsEmpty())
		{
			bIsEqualSearch = FALSE;
			
			if(sKeyWordGetData[2].GetAt(0) == '\"')
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sKeyWordGetData[2]);
			if(!bIsEqualSearch) 
				sKeyWordGetData[2] += _T("*");
							
			sKeyWordGetData[1].MakeUpper();
			if(sKeyWordGetData[1].Compare(_T("OR")) == 0)
				sKeyWordWhere += _T(" | ");
			else
				sKeyWordWhere += _T(" ");
			sKeyWordWhere += sKeyWordGetData[2];				
		}
		if(!sKeyWordGetData[4].IsEmpty())
		{
			bIsEqualSearch = FALSE;
			
			if(sKeyWordGetData[4].GetAt(0) == '\"')
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sKeyWordGetData[4]);
			if(!bIsEqualSearch) 
				sKeyWordGetData[4] += _T("*");

			sKeyWordGetData[3].MakeUpper();
			if(sKeyWordGetData[3].Compare(_T("OR")) == 0)
				sKeyWordWhere += _T(" | ");
			else
				sKeyWordWhere += _T(" ");
			sKeyWordWhere += sKeyWordGetData[4];				
		}

		if(!sKeyWordWhere.IsEmpty())
		{

			if(1 == m_pParentMgr->m_pInfo->GetSearchIndexType())
			{
				if(fd != NULL)
				{
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, BO_IDX_KEYWORd, '") + sKeyWordWhere + _T("') ");
				}
				else
				{
					if(!sEcoSearchWhere.IsEmpty())
						sEcoSearchWhere += _T(" INTERSECT ");
					
					sEcoSearchWhere += _T(" &ECOSEARCH(, BO_IDX_KEYWORd, '") + sKeyWordWhere + _T("') ");
				}
				nEcoSearchCnt++;
			}
			else
			{
				if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
				sWhere += CString(_T(" CATSEARCH(IB.IDX_KEYWORD,'")) + sKeyWordWhere + CString(_T("',NULL)>0 "));
			}
			nSearchField++;
		}

		if(fd != NULL)
		sWhere += _T(")");
	}
	
	
	const INT nPublishYearFieldCnt = 2;
	TCHAR *sPublishYearAlias[nPublishYearFieldCnt] =
	{
		_T("����⵵1"), _T("����⵵2")
	};
	CString sPublishYearGetData[nPublishYearFieldCnt];

	for(i = 0 ; i < nPublishYearFieldCnt ; i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sPublishYearAlias[i], sPublishYearGetData[i]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		sPublishYearGetData[i].TrimRight();
	}

	if(!sPublishYearGetData[0].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
		if(sPublishYearGetData[1].IsEmpty())
		{
			sWhere += CString(_T(" IB.IDX_IPUB_YEAR = '")) + sPublishYearGetData[0] + CString(_T("' "));
		}
		else
			sWhere += CString(_T(" (IB.IDX_IPUB_YEAR >= '")) + sPublishYearGetData[0] + CString(_T("' AND "))
						+ CString(_T(" IB.IDX_IPUB_YEAR <= '")) + sPublishYearGetData[1] + CString(_T("')"));
	}

	
	const INT nClassNoCnt = 2;
	TCHAR* sClassNoAlias[nClassNoCnt] =
	{
		_T("�з���ȣ1"), _T("�з���ȣ2")
	};
	CString sClassNoData[nClassNoCnt];

	for(i = 0 ; i < nClassNoCnt ; i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sClassNoAlias[i], sClassNoData[i]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		sClassNoData[i].TrimRight();
	}

	if(!sClassNoData[0].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");

		if(sClassNoData[1].IsEmpty())
		{
			sWhere += _T(" BB.CLASS_NO = '") + sClassNoData[0] + _T("' ");
		}
		else
		{
			sWhere += _T(" BB.CLASS_NO BETWEEN '") + sClassNoData[0] + _T("' AND '") + sClassNoData[1] + _T("' ") ;
		}
	}
	
	
	CString sRegNo;
	ids = m_pParentMgr->GetControlData(sCMAlias, _T("��Ϲ�ȣ"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	if(!sRegNo.IsEmpty())
	{
		CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);

		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
		sWhere += _T(" BB.REG_NO = '") + sRegNo +_T("'");

		nSearchField++;
	}

	if(fd != NULL) fclose(fd);

	if(!sEcoSearchWhere.IsEmpty())
	{
		if(!sWhere.IsEmpty())
			sWhere += _T(" AND ");
		if(nEcoSearchCnt == 1)
			sWhere += _T(" IB.REC_KEY IN ") + sEcoSearchWhere;
		else
			sWhere += _T(" IB.REC_KEY IN (") + sEcoSearchWhere + _T(") ");
	}

	
	CString sUseLimit;
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	CESL_Control_Element *pCE = pCM->FindControlElement(_T("�缭����"));
	if(pCE == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	
	CButton* pUseLimit = (CButton*)(m_pParentMgr->GetDlgItem(pCE->CTRL_ID));
	if(pUseLimit == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	if(pUseLimit->GetCheck() == 1)
	{
		if(!sWhere.IsEmpty())
			sWhere += _T(" AND ");

		sWhere += _T(" BB.USE_LIMIT_CODE != 'CL' ");
	}

	if(sWhere.IsEmpty())
	{
		CString sMsg = _T("������ �Է��Ͻð� �˻��Ͻʽÿ�.");
		SelfCloseMessageBox(sMsg);
		return 1;
	}

	if(nSearchField == 0 && (nMode == 0))
	{
		CString sMsg = _T("�˻� ������ �����Ͽ� �ð��� ���� �ɸ��ų� ����� ����� �������� ���Ҽ� �ֽ��ϴ�. \r\n�˻��Ͻðڽ��ϱ�?");
		ids = MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("���"), MB_ICONWARNING | MB_YESNO);
		if(ids == IDNO)
			return 1;
	}

	pCE = pCM->FindControlElement(_T("�̿��ڿ�˻�"));
	if(pCE == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	
	CButton* pUserSearch = (CButton*)(m_pParentMgr->GetDlgItem(pCE->CTRL_ID));
	if(pUserSearch == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	if(pUserSearch->GetCheck() == 1)
	{				
		if(!sWhere.IsEmpty())
			sWhere += _T(" AND ");

		if(0 == nMode)
		{
			sWhere += _T(" BB.WORKING_STATUS IN ('BOL112N','BOL113O','BOL114O','BOL115O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL217O','BOL218O','BOL219O','BOL411O','BOL511O','BOL611O') ");
			sWhere += _T(" AND BB.REG_NO IS NOT NULL ") ;
		}
		else if(1 == nMode)
		{
			
			sWhere += _T(" BB.WORKING_STATUS IN ('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL317O','SEL318O','SEL411O','SEL511O') ");
		}
	}	
	else
	{
		if(0 == nMode)
		{
			if(!sWhere.IsEmpty())
				sWhere += _T(" AND ");

			sWhere += _T(" BB.WORKING_STATUS NOT IN ('BOT211O','BOT212O') ");				
		}
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SendSearchBookInfoSQL(CString &sBoWhere, CString &sSeWhere)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SendSearchBookInfoSQL");

	CString sPublishForm;
	CString sCMAlias = _T("CM_BO_LOC_3110");
	ids = m_pParentMgr->GetControlData(sCMAlias, _T("�ڷᱸ��"), sPublishForm);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sDMFromDBAlias[2] = 
	{
		_T("DM_BO_LOC_3110_BO"), _T("DM_BO_LOC_3110_SE")
	};

	INT nStartDMIndex = 0;
	INT nFinishDMIndex = 2;
	if(sPublishForm.Compare(_T("����")) == 0)
	{
		nStartDMIndex = 0;
		nFinishDMIndex = 1;
	}
	else if(sPublishForm.Compare(_T("����")) == 0)
	{
		nStartDMIndex = 1;
		nFinishDMIndex = 2;
		
	}
	else if(sPublishForm.Compare(_T("��ü")) == 0)
	{
		nStartDMIndex = 0;
		nFinishDMIndex = 2;
	}
	else
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sBookDMAlias = _T("DM_BO_LOC_3110");

	CESL_DataMgr *pBODM = m_pParentMgr->FindDM(sDMFromDBAlias[0]);
	CESL_DataMgr *pSEDM = m_pParentMgr->FindDM(sDMFromDBAlias[0]);
	pBODM->FreeData();
	pSEDM->FreeData();
	
	CESL_DataMgr *pDM;

	CESL_DataMgr *pBookDM = m_pParentMgr->FindDM(sBookDMAlias);
	if(pBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	pBookDM->FreeData();

	INT nTotalCount = 0;
	CString sWhere2;
	for(INT k = nStartDMIndex ; k < nFinishDMIndex ; k++)
	{
		if(k == 0) 
			sWhere2 = sBoWhere;
		else
			sWhere2 = sSeWhere;
		
		pDM = m_pParentMgr->FindDM(sDMFromDBAlias[k]);
		if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		DWORD nSTART, nFINISH;
		nSTART = GetTickCount();


		ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pDM, sWhere2);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		nFINISH = GetTickCount();
		CString strMsg;
		strMsg.Format(_T("(Finish) - �̿��� �ڷ� ã�� �˻��� �ɸ� �ð� :  [%d.%d sec]\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
		TRACE(strMsg.GetBuffer(0));
		pDM->QueryLog(strMsg.GetBuffer(0));
		
		nTotalCount += pDM->GetRowCount();

		nSTART = GetTickCount();	
		
		ids = MakeShelfCode(sDMFromDBAlias[k]);

 		if(0 > ids)
		{
			return ids;
		}
		
		CString sWorkingStatus;
		CString sShelfLocCode, sBoxName;
		const INT nCheckIsLoanCnt = 7;
		CString sCheckIsLoan[nCheckIsLoanCnt][2] =
		{
			{ _T("�ڷ����"), _T("") },
			{ _T("��������"), _T("") },
			{ _T("����Ǽ�"), _T("") },
			{ _T("�̿����ѱ���"), _T("") },
			{ _T("IBS_û��_��ġ��ȣ_�ڵ�"), _T("") },
			{ _T("��ϱ���"), _T("") },
			{ _T("�ڷ�Ǹ�"), _T("") } 
		};
		CString sIsLoan;

		CString strTmpData;
		CMarcInfoConversionMgr pMarcConvertMgr;
		pMarcConvertMgr.SetMarcMgr( m_pParentMgr->m_pInfo->pMarcMgr);

		for(INT i = 0 ; i < pDM->GetRowCount() ; i++)
		{
		
			sIsLoan = _T("O");
			for(INT j = 0 ; j < nCheckIsLoanCnt ; j++)
			{
				ids = pDM->GetCellData(sCheckIsLoan[j][0], i, sCheckIsLoan[j][1]);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			}

			
			if(sCheckIsLoan[0][1].Compare(_T("BOL112N"))==0 || 
				sCheckIsLoan[0][1].Compare(_T("SEL112N"))==0 ||
				sCheckIsLoan[0][1].Compare(_T("SEL212N"))==0
				)
			{
				
				CString sRegNo;
				pDM->GetCellData(_T("��Ϲ�ȣ"), i, sRegNo);
				if(sRegNo.IsEmpty())
				{
					sIsLoan = _T("X");
				}
			}
			else
			{
				sIsLoan = _T("X");
			}

			
			ids = m_pCheckLoan->IsUseManageCode();
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			
			if(ids == 1) 
			{
				ids = m_pCheckLoan->IsLoanManageCode(sCheckIsLoan[1][1]);
				if(0 > ids) return ids;
				if(ids == 0) 
				{
					sIsLoan = _T("X");
				}
			}
			
			
			if(_ttoi(sCheckIsLoan[2][1]) > 0)
				sIsLoan = _T("X");

			
			ids = m_pCheckLoan->IsLoan(_T("�̿����ѱ���"), sCheckIsLoan[3][1]);
			if(ids > 0)			
				sIsLoan = _T("X");

			
			ids = m_pCheckLoan->IsLoan(_T("��ġ��ȣ"), sCheckIsLoan[4][1]);
			if(ids > 0)			
				sIsLoan = _T("X");
			
			
			ids = m_pCheckLoan->IsLoan(_T("��ϱ���"), sCheckIsLoan[5][1]);
			if(ids > 0)			
				sIsLoan = _T("X");
			
			
			ids = m_pCheckLoan->IsLoan(_T("�ڷ�Ǳ���"), sCheckIsLoan[6][1]);
			if(ids > 0)			
				sIsLoan = _T("X");

			pDM->SetCellData(_T("���Ⱑ�ɿ���"), sIsLoan, i);
			
			
			pDM->GetCellData(_T("�ڷ����"), i, sWorkingStatus);
			GetWorkingStatusDescToCode(sWorkingStatus, 1);
			pDM->SetCellData(_T("�ڷ����"), sWorkingStatus, i);

			pDM->GetCellData(_T("û����ȣ"), i, sShelfLocCode);
			pDM->GetCellData(_T("�Թ�ȣ"), i, sBoxName);
			if(sWorkingStatus.Compare(_T("�ǽð��谡�ڷ�"))==0 && !sBoxName.IsEmpty())
			{
				sBoxName = sShelfLocCode + _T("(") + sBoxName + _T(")");
				pDM->SetCellData(_T("û����ȣ"), sBoxName, i);
			}

			
			
			
			CString sTFormInfo;
			CString sFormInfo[ 4 ];
			sFormInfo[0] = pDM->GetCellData(_T("PAGE"), i);
			sFormInfo[1] = pDM->GetCellData(_T("PHYSICAL_PROPERTY"), i);
			sFormInfo[2] = pDM->GetCellData(_T("BOOK_SIZE"), i);
			sFormInfo[3] = pDM->GetCellData(_T("ACCOMP_MAT"), i);
			CString output;
			if(!sFormInfo[0].IsEmpty())
				sTFormInfo += sFormInfo[0];
			if(!sFormInfo[1].IsEmpty())
				sTFormInfo = sTFormInfo + _T(":") + sFormInfo[1];
			if(!sFormInfo[2].IsEmpty())
				sTFormInfo = sTFormInfo + _T(";") + sFormInfo[2];
			if(!sFormInfo[3].IsEmpty())
				sTFormInfo = output + _T("+") + sFormInfo[3];
			pDM->SetCellData(_T("���»���"), sTFormInfo, i);

			
			CMarc marc;		
			strTmpData = pDM->GetCellData(_T("MARC"), i);
			if ( 0 <= m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strTmpData, &marc) )
			{
				strTmpData.Empty();	
				ids = pMarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
				if( ids >= 0 )	pDM->SetCellData(_T("�Ѽ�����"), strTmpData, i);
			}		
		}

		
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("�ڷ�Ǳ���"), _T("�ڷ�Ǹ�"));

		
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("�����"), _T("�����"));

		nFINISH = GetTickCount();
		TRACE(_T("(Finish) - �̿��� �ڷ� ã�� �ڷ����,�ڷ��,�����,�Թ�ȣ,û����ȣ ���濡 �ɸ� �ð� : [%d.%d sec]\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);	
	}

	if(nTotalCount < 1)
	{
		SelfCloseMessageBox(_T("�ڷᰡ �������� �ʽ��ϴ�. \r\n�ٽ� �˻����ֽʽÿ�"));
		return 3;
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::ViewSerachBookList(INT nEntryPoint, CString &sRegNo, CString &sPublishForm)
{
EFS_BEGIN

	INT ids;

	CSearchBookListDlg BookListDlg(m_pParentMgr, nEntryPoint);

	CString sDMAlias = _T("DM_BO_LOC_3110");
	ids = MakeShelfCode(sDMAlias);
	if(0 > ids)
	{
		return ids;
	}


	BookListDlg.SetParent(this);
	BookListDlg.DoModal();

	BookListDlg.GetSelectRegNo(sRegNo);
	BookListDlg.GetSelectPublishForm(sPublishForm);
	
	if(sRegNo.IsEmpty())
	{
		return 3;
	}
	else
	{
		
		if(BookListDlg.m_nCloseMode == 0)
			return 11;
		
		else if(BookListDlg.m_nCloseMode == 2)
			return 21;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsSearchedUser()
{
EFS_BEGIN

	
	
	
	CString strFuncName = _T("IsSearchedUser");

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(!pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	if(1 > pDM->GetRowCount())
	{
		SelfCloseMessageBox(_T("�����ڸ� ���� �������ֽʽÿ�"));
		return 1;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetWorkingStatusDescToCode(CString &sCodeValue, INT nMode)
{
EFS_BEGIN

	m_pLocCommonAPI->GetWorkingStatusDescToCode(sCodeValue, nMode);
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewFamilyMember()
{
EFS_BEGIN

	INT ids;

	ids = IsSearchedUser();
	if(0 > ids) return ids;
	if(0 < ids) return 0;

	ids = SearchFamilyMember();
	if(0 > ids) return ids;

	
	if(0 == ids) return 0;	

	CFAMILY_ID dlg(m_pParentMgr);
	dlg.m_strUserAccessAuth = m_pManageValue->m_sLoanUserAccessMode;
	CPoint point;
	GetCursorPos(&point);
	dlg.m_point = point;
	dlg.DoModal();

	CString sRecKey;
	dlg.GetMemberRecKey(sRecKey);

	if(sRecKey.IsEmpty())
		return 0;

	
	ids = SearchLoanInfo(sRecKey, _T(""));
	if(0 > ids) return ids;

	return 0;

EFS_END	
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchFamilyMember()
{
EFS_BEGIN

	INT ids, i, nRowCnt;
	CString strFuncName = _T("SearchFamilyMember");
	
	CString sParentDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sDMAlias = _T("DM_FAMILY_ID_INFO");

	CString sFamilyKey;
	ids = m_pParentMgr->GetDataMgrData(sParentDMAlias, _T("����KEY"), sFamilyKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	pDM->FreeData();

	if(sFamilyKey.IsEmpty()) return 0;

	CString sWhere;
	sWhere.Format(_T("CLU.FID_KEY = %s"), sFamilyKey);

	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pDM, sWhere);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	nRowCnt = pDM->GetRowCount();
	CString strValue, strUserNo;
	ids = m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	strValue.TrimLeft();	strValue.TrimRight();
	strValue.MakeUpper();

	
	CString strEnc;
	CStringArray asDes1,asDes2;

	for(i=0 ; i<nRowCnt ; i++)
	{
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			pDM->GetCellData(_T("����ȭ"), i, strEnc);
			asDes1.Add(strEnc);
			pDM->GetCellData(_T("�ڵ���"), i, strEnc);
			asDes2.Add(strEnc);
		}

		ids = pDM->GetCellData(_T("�����ڹ�ȣ"), i, strUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("Y") != strValue)
		{	
			CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pManageValue->m_sIsUnityLoanService );
		}
		ids = pDM->SetCellData(_T("�����ڹ�ȣǥ��"), strUserNo, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && 0 < nRowCnt)
	{
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();	
				}
				for(i = 0; i < nRowCnt; i++)
				{
					pDM->SetCellData(_T("����ȭ"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("�ڵ���"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	return pDM->GetRowCount();


EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::IsSearchedLoanInfo()
{
EFS_BEGIN

	CString strFuncName = _T("IsSearchedLoanInfo");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT nRowCount = pDM->GetRowCount();

	if(nRowCount < 0) return 2;
	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::CopyDMToDM(CESL_DataMgr *pDM, CString sDMAlias)
{
EFS_BEGIN

	INT ids;
	
	INT nRowCount= pDM->GetRowCount();

	CString sGetData;
	CString *sColAlias;
	INT nColCnt = pDM->GetColCount();
	sColAlias = new CString[nColCnt];
	ids = pDM->GetAllAlias(sColAlias, nColCnt);
	if(0 > ids) 
	{
		delete []sColAlias;
		return -1;
	}

	CESL_DataMgr *pDstDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDstDM == NULL) 
	{
		delete []sColAlias;
		return -1;
	}

	for(INT i = 0 ; i < nRowCount ; i++)
	{
		pDstDM->InsertRow(-1);
		for(INT j = 0 ; j < nColCnt ; j++)
		{
			ids = pDM->GetCellData(i, j, sGetData);
			if(0 > ids&& (ids != -4012)) 
			{
				delete []sColAlias;
				return -1;
			}
			if(ids == -4012) sGetData.Empty();
			
			ids = pDstDM->SetCellData(sColAlias[j], sGetData, pDstDM->GetRowCount()-1);
			if(0 > ids) 
			{
				delete []sColAlias;
				return -1;
			}
		}
	}

	delete []sColAlias;

	return 0;

EFS_END	
return -1;
}


INT CBL_LOC_LOAN_PROC::ReLoadLoanInfo(BOOL bAskPasswd /* =TRUE */)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReLoadLoanInfo");

	CString sRecKey;
	
	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("������KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, strFuncName);

	if(sRecKey.IsEmpty())
		return 0;

	
	ids = SearchLoanInfo(sRecKey, _T(""), bAskPasswd);
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}



INT CBL_LOC_LOAN_PROC::EditLoanUserInfo()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("EditLoanUserInfo");

	
	ids = IsSearchedUser();
	if(0 > ids) return ids;
	if(ids > 0) return 1;
	
	CString sRecKey;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("������KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	INT nMode;
	m_pCheckLoan->GetLoanUserAccessMode(nMode);

	
	if(0 == nMode || 2 == nMode) 
	{
		nMode = 3;
	}
	else if(nMode == 1)
	{
		nMode = 6;
	}
	
	
	if(m_EditLoanUser == NULL)
	{
		m_EditLoanUser = new CBO_LOC_3320(m_pParentMgr,m_pLoanShareManager, m_pManageValue);
		
	}
	
	m_EditLoanUser->m_nAction = ACTION_READ_F_LOANINFO;
	m_EditLoanUser->SetMode(nMode,TRUE);
	m_EditLoanUser->SetRecKey(sRecKey);
	m_EditLoanUser->DoModal();
	BOOL bIsSelfLibSecede = m_EditLoanUser->KL_IsSelfLibSecede();
	delete m_EditLoanUser;
	m_EditLoanUser = NULL;
	
	
	if(bIsSelfLibSecede)
	{
		::PostMessage(m_pParentMgr->GetSafeHwnd(),WM_KEYDOWN,VK_ESCAPE,1);
		SelfCloseMessageBox(_T("Ż�� ó���� �Ϸ� �Ǿ����ϴ�."));
	}
	else
	{
		
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse)
		{
			SPFExcutePath(_T("ȸ�����ε�_NOASK"));
		}
		else
		{
			SPFExcutePath(_T("ȸ�����������ε�"));
		}
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::SaveLoanUserEtcInfo()
{
EFS_BEGIN

	INT ids;

	
	ids = IsSearchedUser();
	if(0 > ids) return ids;
	if(ids > 0) return 1;
	
	
	ids = SaveLoanUserEtcInfoDBProc();
	if(0 > ids) return ids;

	ids = UpdateEtcDM();
	if(0 > ids) return ids;

	SelfCloseMessageBox(_T("�����Ǿ����ϴ�."));

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::SaveLoanUserEtcInfoDBProc()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SaveLoanUserEtcInfoDBProc");

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sTableName = _T("CO_LOAN_USER_TBL");

	CString sRecKey;
	CString sEtcInfo;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("������KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);



	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_MEMBER_INFO"), _T("���"), sEtcInfo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL)  ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	CKolas2up_MobileApi *pMobileApi;
	pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

	pDM->StartFrame();
	pDM->InitDBFieldData();
	ids= pDM->AddDBFieldData(_T("REMARK"), _T("STRING"), sEtcInfo, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	ids= pDM->AddDBFieldData(_T("REMARK_EDIT_DATE"), _T("NUMERIC"), _T("SYSDATE"), TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("�̿��ں�����"), __WFILE__, __LINE__), TRUE);

	ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	pDM->EndFrame();

	CLocCommonAPI::MobileApiEndLog(pMobileApi);
	
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("REMARK,REMARK_EDIT_DATE")));
	
	ShareInfoCenterUpload(_T("N"), _T("U"));
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateEtcDM()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateEtcDM");

	CString sEtcInfo;
	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_MEMBER_INFO"), _T("���"), sEtcInfo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���"), sEtcInfo, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsUserBookLoan(CString sMode, BOOL bIsILL, CString strBookManageCode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsUserBookLoan");

	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sBookDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CString sMediaCode;
	ids = m_pParentMgr->GetDataMgrData(sBookDMAlias, _T("��ü����"), sMediaCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(sMediaCode.Compare(_T("OF")) == 0)
	{
		SelfCloseMessageBox(_T("��ü������ 'OF'�� �ڷ�� ������ �Ұ����մϴ�."), m_pParentMgr);
		return 5;
	}
	
	CString sIsAppendix;
	
	ids = m_pParentMgr->GetDataMgrData(sBookDMAlias, _T("�ηϿ���"), sIsAppendix, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(sIsAppendix.Compare(_T("A")) == 0)
	{
		if(m_pCheckLoan->IsAppendixLoan() !=0)
		{
			SelfCloseMessageBox(_T("�η��ڷ�� ������ �Ұ����մϴ�."), m_pParentMgr);
			return 1;
		}
	}
	
	CString strUserClassCode;
	CString strLoanPeriod;

	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("����������"), strUserClassCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	ids = m_pManageValue->GetUserPosition(strUserClassCode, _T("�������"), strLoanPeriod);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sRegCode, sValue;
	ids = m_pParentMgr->GetDataMgrData(sBookDMAlias, _T("��ϱ���"), sRegCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	ids = m_pManageValue->GetRegClass(strUserClassCode, sRegCode, _T("�������"), sValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(!sValue.IsEmpty()) strLoanPeriod = sValue;
	
	if(0 == _ttoi(strLoanPeriod))
	{
		if(EXP_LOAN == sMode)
		{
			SelfCloseMessageBox(_T("��������� '0'���� �ڷ�� ������ �� �����ϴ�. \r\n�����ϱ� ���ؼ��� �������� �������ֽʽÿ�"), m_pParentMgr);
			return 1;
		}
		else
		{
			SelfCloseMessageBox(_T("��������� '0'���� �ڷ�� ������ �� �����ϴ�. \r\n�����ϱ� ���ؼ��� �������� �������ֽʽÿ�"), m_pParentMgr);
			return 1;
		}
	}

	if(sMode == EXP_LOAN)
	{
		
		ids = CheckLoanBookCnt(sIsAppendix, bIsILL, strBookManageCode);
		if(ids > 0) return 2;
	}

	if(FALSE == bIsILL)
	{
		ids = m_pCheckLoan->IsUseManageCode();
		if(0 > ids) return ids;
		
		if(ids == 1) 
		{
			ids = m_pCheckLoan->IsLoanManageCode();
			if(0 > ids) return ids;
			if(ids == 0) 
			{
				SelfCloseMessageBox(_T("�������а��� �ٸ��Ƿ� ������ �� �� �����ϴ�. "));
				return 1;
			}
		}
	}
	
	const INT nCnt = 4;
	TCHAR *sFieldAlias[nCnt] = {
		_T("��ϱ���"), _T("��ġ��ȣ"), _T("�̿����ѱ���"), _T("�ڷ�Ǳ���")
	};
	CString sCode[nCnt];
	for(INT i = 0 ; i < nCnt ; i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sBookDMAlias, sFieldAlias[i], sCode[i], 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("IsUserBookLoan"));
	}
	for(i = 0 ; i < nCnt ; i++)
	{
		ids = m_pCheckLoan->IsLoan(sFieldAlias[i], sCode[i]);
		if(0 > ids) return ids;
		if(ids > 0) 
		{
			CString sCodeDesc;
			ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(sFieldAlias[i], sCode[i], sCodeDesc);
			if(0 > ids) return ids;

			CString sMsg;
			sMsg.Format(_T("�����Ϸ��� å�� %s�� ������ �� ���� %s(%s)�Դϴ�.")
							, sFieldAlias[i], sFieldAlias[i], sCodeDesc);
			SelfCloseMessageBox(sMsg);
			return 2;
		}
	}

	CString sUserClassCode;
	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("����������"), sUserClassCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("IsUserBookLoan"));
	
	CString sUseObjectCode;
	ids = m_pCheckLoan->IsUseObject(sUseObjectCode);
	if(0 > ids) return ids;
	if(ids > 0) 
	{
		CString sMsg;
		sMsg.Format(_T("�̿����� '%s'�� �ڷ�� ������ �� �����ϴ�. \r\n�����ϱ� ���ؼ��� �������� �������ֽʽÿ�"), sUseObjectCode);
		SelfCloseMessageBox(sMsg);
		return 4;
	}	

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::RecordReserveExpireDate()
{
EFS_BEGIN

	const INT CHECK_CNT = 6;
	INT ids;

	
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	TCHAR *sFieldAlias[CHECK_CNT] = 
	{
		_T("����/���ӱ���"), _T("åKEY"), _T("�����̿��ڼ�"), _T("��KEY"), _T("��ȣ����KEY"), _T("��Ϲ�ȣ")
	};
	CString sGetData[CHECK_CNT];

	INT nRowIndex;
	ids = GetCurrentGridPos(nRowIndex);
	if(0 > ids) return ids;

	for(INT i = 0 ; i < CHECK_CNT ; i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, sFieldAlias[i], sGetData[i], nRowIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("RecordReserveExpireDate"));
	}
	
	if(m_pManageValue->m_bVolReserve && !sGetData[4].IsEmpty())
	{
		
		CString sVolReserveTable;
		
		if(sGetData[0] == _T("SE"))
			sVolReserveTable = _T("SE_BOOK_TBL");
		else if(sGetData[0] == _T("NB"))
			sVolReserveTable = _T("CO_NON_DB_BOOK_TBL");
		else
			sVolReserveTable = _T("BO_BOOK_TBL");			

		CString sQuery;
		sQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 CL ") 
			           _T(" WHERE CL.BOOK_KEY IN (SELECT REC_KEY FROM %s WHERE SPECIES_KEY = %s) ")
					   _T("   AND CL.REG_NO IS NULL ")
					   _T("   AND CL.STATUS = '3' ")
					   _T("   AND CL.VOL_INFO = %s "),
					   sVolReserveTable, sGetData[3], sGetData[4]);

		
		CESL_DataMgr* pTmpDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
		if(!pTmpDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -11, _T("RecordReserveExpireDate"));

		ids = pTmpDM->GetOneValueQuery(sQuery, sGetData[2]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -12, _T("RecordReserveExpireDate"));		
	}
	

	
	if(_ttoi(sGetData[2]) == 0)  return 0;

	if(!(m_pManageValue->m_bVolReserve))
	{
		CString sSQL;
		sSQL.Format(_T("SELECT REC_KEY FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3' AND RESERVATION_DATE = ")
			_T("(SELECT MIN(RESERVATION_DATE) FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3')"), sGetData[1], sGetData[1]);

		CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
		if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("RecordReserveExpireDate"));

		CString sReturnDate;
		CString sReturnType;
		ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("�ݳ�����"), sReturnType);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("RecordReserveExpireDate"));

		CString sReturnDateControlAlias = _T("�ݳ���");
		if(sReturnType.Compare(_T("2"))== 0)
			sReturnDateControlAlias = _T("�ұ޹ݳ���");

		ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, sReturnDate);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("RecordReserveExpireDate"));

		CString sLoanKey;
		ids = CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pDM, sSQL, sLoanKey);
		if(0 > ids&& ids != -4007) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("RecordReserveExpireDate"));

		
		if(sLoanKey.IsEmpty())
			return 0;
	}

	if(_T("Y") == m_pManageValue->m_sUnmannedReserveSys)
	{
		
		CString strQuery;
		if(!m_pManageValue->m_bVolReserve)
		{
 			strQuery.Format(	
 				_T("SELECT COUNT(1) FROM LS_WORK_T01 LS1")
 				_T(" WHERE LS1.BOOK_KEY=%s AND LS1.STATUS='3'")
 				_T(" AND LS1.UNMANNED_RESERVATION_LOAN IS NOT NULL")
 				_T(" AND LS1.RESERVATION_DATE=")
 						_T("(SELECT MIN(LS2.RESERVATION_DATE) FROM LS_WORK_T01 LS2")
 						_T(" WHERE LS1.BOOK_KEY=LS2.BOOK_KEY AND LS2.STATUS='3'")
 						_T(" GROUP BY LS2.BOOK_KEY)"), sGetData[1]);
 			CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
 			if(NULL == pDM){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("RecordReserveExpireDate"));}
 			CString strResult;
 			ids = CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pDM, strQuery, strResult);
 			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("RecordReserveExpireDate"));}
 			if(0 < _ttoi(strResult))
 			{
 				CString strMsg;
 				strMsg.Format(_T("[%s]�� ���ο��� �ڷ��Դϴ�."), sGetData[5]);
 				MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("���� ó��"), MB_OK | MB_ICONINFORMATION);
 				return 0;
 			}
		}
	}

	ids = GetReservationAutoMode();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("RecordReserveExpireDate"));

	CString strReserveMsg;
	strReserveMsg.Format(_T("[%s]�� ����� �ڷ��Դϴ�. ���� ó���� �Ͻðڽ��ϱ�?"), sGetData[5]);
	INT nYes = MessageBox(m_pParentMgr->m_hWnd, strReserveMsg, _T("���� ó��"), MB_YESNO | MB_ICONQUESTION);
	if(nYes == IDYES)
	{		
		if(!(m_pManageValue->m_bVolReserve))
		{ 
			CBO_LOC_3170 *Dlg = new CBO_LOC_3170(m_pParentMgr, m_pLoanShareManager, m_pManageValue, 0);
			Dlg->pMain = m_pParentMgr->pMain;
			Dlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
			Dlg->m_pInfo = m_pParentMgr->m_pInfo;
			Dlg->SetInfo(sGetData[3], sGetData[4], sGetData[0], sGetData[1]);
			
			POSITION pos;
			pos = m_obj3170DlgList->AddTail((CObject*)Dlg);
			
			if(Dlg->Create(m_pParentMgr))
 			{
 				Dlg->CenterWindow();
 				Dlg->ShowWindow(SW_SHOWNOACTIVATE);
 				Dlg->UpdateWindow();				
 			}
 			else
			{
				m_obj3170DlgList->RemoveAt(pos);
			}
		}
		else 
		{ 
			CBO_LOC_3170_ReserveInfo *Dlg = new CBO_LOC_3170_ReserveInfo(m_pParentMgr, m_pLoanShareManager, m_pManageValue, 0);
			Dlg->pMain = m_pParentMgr->pMain;
			Dlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
			Dlg->m_pInfo = m_pParentMgr->m_pInfo;
			Dlg->SetInfo(sGetData[3], sGetData[4], sGetData[0], sGetData[1]);						
			Dlg->m_strReturnBookKey = sGetData[1];
			Dlg->m_strReturnBookRegNo = sGetData[5];			
			Dlg->m_nReserveInfoMode = 1;			
						
			POSITION pos;
			pos = m_obj3170DlgList->AddTail((CObject*)Dlg);
			if(Dlg->Create(m_pParentMgr))
 			{
 				Dlg->CenterWindow();
 				Dlg->ShowWindow(SW_SHOWNOACTIVATE);
 				Dlg->UpdateWindow();				
 			}
 			else
			{
				m_obj3170DlgList->RemoveAt(pos);
			}
		}
	}
	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::GetCurrentGridPos(INT &nRowIndex)
{
EFS_BEGIN

	CString strFuncName = _T("GetCurrentGridPos");

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	nRowIndex = pGrid->GetIdx();
	if(nRowIndex < 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	return 0;

EFS_END
return -1;
}



INT CBL_LOC_LOAN_PROC::RecordReserveExpireDateDBProc(CESL_DataMgr *pDM, CString &sLoanKey, CString &sReturnDate)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("RecordReserveExpireDateDBProc");

	
	CString sTableName = _T("LS_WORK_T01");
	CBL_LOC_RESERVATOIN_PROC  ReservationInfo(m_pParentMgr, m_pLoanShareManager,  m_pManageValue);

	CString sPositionCode;
	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pUserDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = pUserDM->GetCellData(_T("����������"), 0, sPositionCode);

	INT nExpireDay = 0;
	ids = ReservationInfo.GetReservationExpireDay(nExpireDay, sPositionCode);
	if(0 > ids) return ids;
	CTimeSpan tExpireDay(nExpireDay, 0, 0, 0);

	CTime tDate;
	if(!sReturnDate.IsEmpty())
	{
		CTime tReturnDate(_ttoi(sReturnDate.Mid(0,4)), _ttoi(sReturnDate.Mid(5,2)), _ttoi(sReturnDate.Mid(8,2)), 0, 0, 0);
		tDate = tReturnDate;
	}else
	{
		
		CString strCurrentTime;
		tDate = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strCurrentTime);

	}
	
	tDate = tDate + tExpireDay;
	CString sReservationExpireDate;
	sReservationExpireDate = tDate.Format(_T("%Y/%m/%d"));
	sReservationExpireDate = _T("TO_DATE('")+sReservationExpireDate+_T(" 23:59:59")+_T("', 'YYYY/MM/DD HH24:MI:SS')");

	CKolas2up_MobileApi *pMobileApi;
	pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("RESERVATION_EXPIRE_DATE"), _T("NUMERIC"), sReservationExpireDate, TRUE);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("���ุ���Ϻο�"), __WFILE__, __LINE__), TRUE);
	
	ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sLoanKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName); 
	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName); 

	pDM->EndFrame();
	
	CLocCommonAPI::MobileApiEndLog(pMobileApi);
	
	SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sLoanKey));
	SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("RESERVATION_EXPIRE_DATE")));
	
	ShareInfoCenterUpload(_T("N"), _T("U"));
		
	if((m_pParentMgr->m_pInfo->MODE==10000) || (m_pParentMgr->m_pInfo->MODE==30000))
		return 0;

	CString sWhere;
	sWhere.Format(_T(" CL.LOAN_KEY = %s "), sLoanKey);
	
	
	if((m_pParentMgr->m_pInfo->MODE==10000) || (m_pParentMgr->m_pInfo->MODE==30000))
		return 0;

	//JOB.2019.0073 : �浹 License
// 	CLocMailMgrAPI MailMgr(m_pParentMgr);
// 	MailMgr.MakeRecord(0, sWhere, FALSE);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetReservationAutoMode()
{
EFS_BEGIN

	INT ids;

	CBL_LOC_RESERVATOIN_PROC  ReservationInfo(m_pParentMgr, m_pLoanShareManager, m_pManageValue);
	INT nExpireDay = 0;
	ids = ReservationInfo.GetReservationAutoMode();

	if(ids == 0) return 2;
	else return 1;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AppendixLoanProc()
{
EFS_BEGIN

	INT	ids;
	CString strFuncName = _T("AppendixLoanProc");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_ControlMgr	*pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
	pGrid->SelectMakeList();
	INT	nRowCount = pGrid->SelectGetCount();
	
	pGrid->SelectGetTailPosition();
	
	if(0 >= nRowCount)
	{
		SelfCloseMessageBox(_T("�ηϴ����Ϸ��� å�� ������ �ֽʽÿ�."));
		return 1;
	}
	
	if(1 < nRowCount)
	{
		SelfCloseMessageBox(_T("�Ѱ��� �ڷḸ �����Ͽ� �ֽʽÿ�."));
		return 2;
	}

	
	INT nIndex = pGrid->SelectGetIdx();
	if(0 > nIndex) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	CString	sStatus;
	ids = pDM->GetCellData(_T("�������"), nIndex, sStatus);
	if(_T("0") != sStatus && _T("2") != sStatus)
	{
		
		if(_T("O") != sStatus && _T("L") != sStatus && _T("D") != sStatus)
		{
			SelfCloseMessageBox(_T("������ �ڷ�� ������°� �ƴմϴ�. �������� �ڷḦ �����Ͽ� �ֽʽÿ�"));
		}
		else
		{
			SelfCloseMessageBox(_T("å�η������ �ڷῡ ���ؼ� �ηϴ����� �������� �ʽ��ϴ�."));
		}
		return 3;
	}
	
	CString sIsBook;
	pDM->GetCellData(_T("�ηϿ���"), nIndex, sIsBook);
	if(_T("B") != sIsBook) 
	{
		SelfCloseMessageBox(_T("�η��ڷ�� �ηϴ����� �� �� �����ϴ�."));
		return 4;
	}

	ids = IsLoanAppendix();
	if(0 != ids) 
	{
		SelfCloseMessageBox(_T("�ηϴ����� ������� �ʴ� �������Դϴ�."));
		return 5;
	}
	
	CESL_DataMgr *pAppendixDM;
	pAppendixDM = MakeAppendixDM(nIndex);
	if(NULL == pAppendixDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName); 
	}
	if(1 > pAppendixDM->GetRowCount()) 
	{
		SelfCloseMessageBox(_T("�η��ڷᰡ �������� �ʴ� �ڷ��Դϴ�."));
		return 6;
	}

	CLoanAppendixListDlg AppendixList(m_pParentMgr);
	AppendixList.m_pPreRegNoMgr = m_pPreRegNoMgr;
	AppendixList.DoModal();

	if(1 > pAppendixDM->GetRowCount())
	{
		return 0;
	}
	
	CString sRegNo;
	for(INT i = 0; i < pAppendixDM->GetRowCount(); i++)
	{
		pAppendixDM->GetCellData(_T("��Ϲ�ȣ"), i, sRegNo);
		
		ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName); 

		MakeRegNoSpaceToZero(TRUE);
		InitExpDM();
		SPFExcutePath(_T("����"));
	}

	return 0;
EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AppendixLoan(INT nIndex)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AppendixLoan");

	CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName); 
	
	INT nMode = 0;

	if(nIndex == -1)
	{
		nIndex = pLoanInfoDM->GetRowCount()-1;
		nMode = 1;
	}

	CString sIsBook;
	pLoanInfoDM->GetCellData(_T("�ηϿ���"), nIndex, sIsBook);

	CString strPubFormCode = _T("");
	
	if ( !(sIsBook.Compare(_T("C")) == 0 || sIsBook.Compare(_T("B")) == 0  ) ) return 0;
	
	ids = IsLoanAppendix();
	if(0 > ids) return ids;
	
	if(ids != 0) return 0;
	
	if(nMode == 1)
	{
		CESL_DataMgr* pLoanExpInfoDM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_BOOK_INFO"));
		if(pLoanExpInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 
		
		if(pLoanExpInfoDM->GetRowCount() == 0)
		{
			AfxMessageBox(_T("�̹� ��������"));
			return 1;
		}

		CString sACnt;
		ids = pLoanExpInfoDM->GetCellData(_T("�η�å��"), 0, sACnt);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 
		
		if(_ttoi(sACnt) == 0) return 0;
	}
	
	CESL_DataMgr *pDM;
	pDM = MakeAppendixDM(nIndex);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 
	if(pDM->GetRowCount() < 1) return 0;
	
	ids = AddMemberInfo();
	if(0 > ids) return ids;
	
	ids = ViewLoanScreen(0);
 	if(ids != 0) return ids;

	CString strIsLoan;
	int nRowCnt = 0;
	const int ROW_COUNT = pDM->GetRowCount();
	for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
	{
		ids = pDM->GetCellData(_T("���Ⱑ�ɿ���"), nRowCnt, strIsLoan);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("O") == strIsLoan)
		{
			break;
		}
	}

	
	if(_T("O") != strIsLoan)
	{
		return 0;
	}

	ids = MessageBox(m_pParentMgr->m_hWnd, _T("���Ⱑ���� �η��ڷᰡ �����մϴ�. ���� ������ �Ͻðڽ��ϱ�?"), _T("�η� �ڷ� ����"), MB_ICONQUESTION | MB_YESNO);
	if(ids == IDNO) return 0;

	CLoanAppendixListDlg AppendixList(m_pParentMgr);
	AppendixList.m_pPreRegNoMgr = m_pPreRegNoMgr;
	AppendixList.DoModal();

	if(pDM->GetRowCount() < 1 ) return 0;

	
	CString sRegNo;
	for(INT i=0 ;i < pDM->GetRowCount() ; i++)
	{
		pDM->GetCellData(_T("��Ϲ�ȣ"), i, sRegNo);
		
		ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName); 

		MakeRegNoSpaceToZero(TRUE);

		InitExpDM();

		ids = Loan(sRegNo);
		if(0 > ids) return ids;		
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsLoanAppendix()
{
EFS_BEGIN

	return m_pCheckLoan->IsAppendixLoan();

EFS_END
return -1;
}

CESL_DataMgr* CBL_LOC_LOAN_PROC::MakeAppendixDM(INT nIndex)
{
EFS_BEGIN

	INT ids;
	CString sBookLoanDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sAppedixLoanDMAlias = _T("DM_BO_LOC_3100_APPENDIX_INFO");
	CString sAppedixLoanSEDMAlias = _T("DM_BO_LOC_3100_APPENDIX_INFO_SE");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sAppedixLoanDMAlias);
	if(pDM == NULL) return NULL;

	CESL_DataMgr *pSEDM = m_pParentMgr->FindDM(sAppedixLoanSEDMAlias);
	if(pSEDM == NULL) return NULL;
	
	
	CESL_DataMgr *pLoanBookDM = m_pParentMgr->FindDM(sBookLoanDMAlias);
	if(pLoanBookDM == NULL) return NULL;

	CString sLoanBookSpeciesKey;
	
	ids = m_pParentMgr->GetDataMgrData(sBookLoanDMAlias, _T("��KEY"), sLoanBookSpeciesKey, nIndex);
	if(0 > ids) return NULL;

	
	CString sWhere;
	sWhere.Format(_T("B.SPECIES_KEY = %s"), sLoanBookSpeciesKey);
	pDM->FreeData();

	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pDM, sWhere);
	if(0 > ids) return NULL;

	CString sLoanBookVolKey;
	ids = m_pParentMgr->GetDataMgrData(sBookLoanDMAlias, _T("��ȣ����KEY"), sLoanBookVolKey, nIndex);
	
	sWhere.Format(_T("B.SPECIES_KEY = %s AND B.VOL_KEY = '%s' "), sLoanBookSpeciesKey, sLoanBookVolKey);

	pSEDM->FreeData();
	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pSEDM, sWhere);
	if(0 > ids) return NULL;

	if((pDM->GetRowCount()==0) && (pSEDM->GetRowCount()>0))
	{
		CLocCommonAPI::AddDM(pSEDM, pDM, m_pParentMgr);
	}	
		CString sShelfLocCode, sBoxName;
		const INT nCheckIsLoanCnt = 7;
		CString sCheckIsLoan[nCheckIsLoanCnt][2] =
		{
			{ _T("�ڷ����"), _T("") },
			{ _T("��������"), _T("") },
			{ _T("����Ǽ�"), _T("") },
			{ _T("�̿����ѱ���"), _T("") },
			{ _T("IBS_û��_��ġ��ȣ_�ڵ�"), _T("") },
			{ _T("��ϱ���"), _T("") },
			{ _T("�ڷ��"), _T("") } 
		};
		CString sIsLoan;
		
		CString sWorkingStatus;
		for(INT i = 0 ; i < pDM->GetRowCount(); i++)
		{		
			sIsLoan = _T("O");
			for(INT j = 0 ; j < nCheckIsLoanCnt ; j++)
			{
				ids = pDM->GetCellData(sCheckIsLoan[j][0], i, sCheckIsLoan[j][1]);				
			}

			
			if(sCheckIsLoan[0][1].Compare(_T("BOL112N"))==0 || 
				sCheckIsLoan[0][1].Compare(_T("SEL112N"))==0 ||
				sCheckIsLoan[0][1].Compare(_T("SEL212N"))==0
				)
			{		
				
				CString sRegNo;
				pDM->GetCellData(_T("��Ϲ�ȣ"), i, sRegNo);
				if(sRegNo.IsEmpty())
				{
					sIsLoan = _T("X");
				}
			}
			else
			{
				sIsLoan = _T("X");
			}

			
			ids = m_pCheckLoan->IsUseManageCode();			
			
			if(ids == 1) 
			{
				ids = m_pCheckLoan->IsLoanManageCode(sCheckIsLoan[1][1]);				
				if(ids == 0) 
				{
					sIsLoan = _T("X");
				}
			}
			
			
			if(_ttoi(sCheckIsLoan[2][1]) > 0)
			{
				sIsLoan = _T("X");
			}

			
			ids = m_pCheckLoan->IsLoan(_T("�̿����ѱ���"), sCheckIsLoan[3][1]);
			if(ids > 0)			
			{
				sIsLoan = _T("X");
			}

			
			ids = m_pCheckLoan->IsLoan(_T("��ġ��ȣ"), sCheckIsLoan[4][1]);
			if(ids > 0)			
			{
				sIsLoan = _T("X");
			}
			
			
			ids = m_pCheckLoan->IsLoan(_T("��ϱ���"), sCheckIsLoan[5][1]);
			if(ids > 0)		
			{
				sIsLoan = _T("X");
			}
			
			
			ids = m_pCheckLoan->IsLoan(_T("�ڷ�Ǳ���"), sCheckIsLoan[6][1]);
			if(ids > 0)			
			{
				sIsLoan = _T("X");
			}

			pDM->SetCellData(_T("���Ⱑ�ɿ���"), sIsLoan, i);

			pDM->GetCellData(_T("�ڷ����"), i, sWorkingStatus);			
			CLocCommonAPI::GetWorkingStatusDescToCode(sWorkingStatus, 1);			
			pDM->SetCellData(_T("�ڷ����"), sWorkingStatus, i);
		}

	
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("�ڷ�Ǳ���"), _T("�ڷ��"));	
	m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("��ü����"), _T("��ü����"));

	return pDM;

EFS_END
return NULL;
}

INT CBL_LOC_LOAN_PROC::AppendixLoanDBProc(CESL_DataMgr *pDM)
{
EFS_BEGIN

	INT ids;

	for(INT i=0 ;i < pDM->GetRowCount() ; i++)
	{
		
		CKolas2up_MobileApi *pMobileApi;
		pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

		pDM->StartFrame();
		
		ids = AppendixLoanBookDBProc(pDM, i, pMobileApi);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AppendixLoanDBProc")); 
	
		
		ids = AppendixLoanInfoDBProc(pDM,i, pMobileApi);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("AppendixLoanDBProc")); 
		
		
		ids = AppendixLoanUserDBProc(pDM,i, pMobileApi);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("AppendixLoanDBProc")); 

		ids = pMobileApi->SendFrame();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("AppendixLoanDBProc")); 
	
		pDM->EndFrame();

		
		CLocCommonAPI::MobileApiEndLog(pMobileApi);
	}
	return 0;

EFS_END	
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateAppendixLoanProc(CESL_DataMgr *pDM)
{
EFS_BEGIN

	INT ids;

	INT nAppendixCnt = pDM->GetRowCount();

	CString sAppendixCnt;
	sAppendixCnt.Format(_T("%d"), nAppendixCnt);
	
	
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(sDMAlias);
	if(pLoanDM == NULL)ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("UpdateAppendixLoanProc"));
	
	ids = m_pParentMgr->SetDataMgrData( sDMAlias, _T("�η��ڷ��"), sAppendixCnt, pLoanDM->GetRowCount()-1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("UpdateAppendixLoanProc")); 

	
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("UpdateAppendixLoanProc"));

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("UpdateAppendixLoanProc"));

	INT nStatusColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, _T("�η��ڷ��"), nStatusColIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("UpdateAppendixLoanProc")); 
	
	if(nStatusColIndex != -1)
		pGrid->SetAt(pLoanDM->GetRowCount()-1, nStatusColIndex, sAppendixCnt);
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AppendixLoanBookDBProc(CESL_DataMgr *pDM, INT nIndex, CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;

	CString sTableName = _T("BO_BOOK_TBL");
	CString sWorkingStatus;
	CString sRecKey;

	CString sPublishForm;
	ids = pDM->GetCellData(_T("����/���ӱ���"), nIndex, sPublishForm);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AppendixLoanBookDBProc")); 


	if(sPublishForm.Compare(_T("SE")) == 0)
		sTableName = _T("SE_BOOK_TBL");
	
	ids = GetBOWorkingStatus(sWorkingStatus);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AppendixLoanBookDBProc")); 
	
	ids = pDM->GetCellData(_T("�η�KEY"), nIndex, sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("AppendixLoanBookDBProc")); 

	pDM->InitDBFieldData();

	
	ids = pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), sWorkingStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("AppendixLoanBookDBProc")); 

	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("�η��ڷ�"), __WFILE__, __LINE__), TRUE);

	ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("AppendixLoanBookDBProc")); 

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AppendixLoanInfoDBProc(CESL_DataMgr *pDM, INT nIndex, CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;

	pDM->InitDBFieldData();

	
	CString sLoanKey;
	CString sLoanDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(sLoanDMAlias);
	if(pLoanDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AppendixLoanInfoDBProc"));

	ids = pLoanDM->GetCellData(_T("����/�ݳ�����KEY"), pLoanDM->GetRowCount()-1, sLoanKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("AppendixLoanInfoDBProc"));
	
	ids = pDM->AddDBFieldData(_T("LOAN_KEY"), _T("NUMERIC"), sLoanKey, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("AppendixLoanInfoDBProc")); 

	
	const INT nSrcFieldCnt = 3;
	TCHAR *sSrcFieldAlias[nSrcFieldCnt] = 
	{	_T("�η�KEY"), _T("�ڷ��"), _T("����/���ӱ���") };
	TCHAR *sDstFieldAlias[nSrcFieldCnt] = 
	{	_T("BOOK_KEY"), _T("SHELF_LOC_CODE"), _T("PUBLISH_FORM_CODE") };
	TCHAR *sDstFieldTypeAlias[nSrcFieldCnt] = 
	{	_T("NUMERIC"), _T("STRING"), _T("STRING") };

	CString sGetData;
	for(INT i= 0 ; i < nSrcFieldCnt ;i++)
	{
		ids = pDM->GetCellData(sSrcFieldAlias[i], nIndex, sGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("AppendixLoanInfoDBProc"));

		ids = pDM->AddDBFieldData(sDstFieldAlias[i], sDstFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -6, _T("AppendixLoanInfoDBProc"));

	}
	
	CString sSrcUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	const INT nSrcUserFieldCnt = 4;
	TCHAR *sSrcUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("������KEY"), _T("�����ڼҼ�"), _T("����������"), _T("ȸ������") 
	};
	TCHAR *sDstUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("USER_KEY"), _T("USER_POSITION_CODE"), _T("USER_CLASS_CODE"), _T("MEMBER_CLASS") 
	};
	TCHAR *sDstUserFieldTypeAlias[nSrcUserFieldCnt] = 
	{
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") 
	};

	for(i= 0 ; i < nSrcUserFieldCnt ;i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sSrcUserDMAlias, sSrcUserFieldAlias[i], sGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -7, _T("AppendixLoanInfoDBProc"));

		if(sGetData.IsEmpty() && (CString(_T("NUMERIC")).Compare(sDstUserFieldTypeAlias[i])==0)) sGetData = _T("NULL");
		ids = pDM->AddDBFieldData(sDstUserFieldAlias[i], sDstUserFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -8, _T("AppendixLoanInfoDBProc"));

	}

	
	
	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;


	ids = pDM->AddDBFieldData(_T("LOAN_TYPE_CODE"), _T("STRING"), sLoanType, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -10, _T("AppendixLoanInfoDBProc"));

	
	CString sRecKey;
	ids = pDM->MakeRecKey(sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -11, _T("AppendixLoanInfoDBProc"));

	Send3000ApiSocket(_T("����"), sRecKey);

	CString sLoanStatus = _T("0");
	ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), sLoanStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -13, _T("AppendixLoanInfoDBProc"));
	CString sLoanDate = _T("SYSDATE");

	ids = pDM->AddDBFieldData(_T("LOAN_DATE"), _T("NUMERIC"), sLoanDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -14, _T("AppendixLoanInfoDBProc"));
	
 	
 	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
 		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII-�̵�������"));
	}
	else
	{
		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII"));
	}
 	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -15, _T("AppendixLoanInfoDBProc"));
	}
	
	
	ids = pDM->AddDBFieldData(_T("L_WORKER"), _T("STRING"), m_pParentMgr->m_pInfo->USER_ID);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -15, _T("AppendixLoanInfoDBProc"));
	}

	
	CString sLog;
	sLog.Format(_T("%s"), m_pParentMgr->GetWorkLogMsg(_T("�η�ó��"), __WFILE__, __LINE__));
	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), sLog, TRUE);	
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), m_pParentMgr->m_pInfo->MANAGE_CODE, TRUE);	

	ids = pMobileApi->MakeInsertFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -16, _T("AppendixLoanInfoDBProc"));

	return 0;
	

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AppendixLoanUserDBProc(CESL_DataMgr *pDM, INT nIndex, CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;

	ids = IsAppendixPlusLoanBookCnt();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AppendixLoanUserDBProc"));
	if(ids > 0) return 0;

	pDM->InitDBFieldData();	
	CString sRecKey;
	CString sLoanBookCnt;
	CString sDMUserInfoAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("������KEY"), sRecKey, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("AppendixLoanUserDBProc"));
	
	sLoanBookCnt.Format(_T("%d"), _ttoi(sLoanBookCnt) + 1);

	if(1 > _ttoi(sLoanBookCnt))
	{
		sLoanBookCnt = _T("0");
	}
	ids = pDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sLoanBookCnt, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("AppendixLoanUserDBProc"));
	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("�η�ó��"), __WFILE__, __LINE__), TRUE);
	ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("AppendixLoanUserDBProc"));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsAppendixPlusLoanBookCnt()
{
EFS_BEGIN

	return 1;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InputBarCode()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("InputBarCode");
	
	CString strLoanKey;
	BOOL bIllProcMat, bIllReturnWill;

	CESL_DataMgr* pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pMemberDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
	CString strGetBarcodeData;
 	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), strGetBarcodeData);
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
 	strGetBarcodeData.TrimLeft(); strGetBarcodeData.TrimRight();
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_SEARCH_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CButton *pOtherCheck = (CButton*)pCM->FindControl(_T("Ÿ���ݳ��켱"));
	if(NULL == pOtherCheck) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse && 10000 != m_pParentMgr->m_pInfo->MODE)
	{	
 		if(14 <= strGetBarcodeData.GetLength() && TRUE == IsNumericText(strGetBarcodeData.Left(6)))
 		{ 
			
			CKL_UserSearchDlg dlg(m_pParentMgr, this);
			dlg.m_nMode = 0;
			dlg.m_strUserNo = strGetBarcodeData;
			if(IDOK == dlg.DoModal())
			{
				ids = KL_UserInfoView();
				if(0 > ids) return ids;				
			}			
			m_strSerchCivilNo = _T("");

			return 0;
		}
		
		strGetBarcodeData.MakeUpper();
		CString strRegNoData = strGetBarcodeData;
		
		BOOL bUnionDBLoan = FALSE;
		ISUnionDBLoan(strGetBarcodeData, m_strReturnBookLibCode, bUnionDBLoan);
		
		if(bUnionDBLoan == FALSE || (bUnionDBLoan == TRUE && m_pManageValue->m_sBookCooperativeSys != _T("Y")) ){
			if(!m_strReturnBookLibCode.IsEmpty() && m_strReturnBookLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
			{ 				
				CKL_UserSearchDlg dlg(m_pParentMgr, this);
				CString strPrevRegNoFromOther;
				ids = m_pManageValue->GetManageValue(_T("��Ÿ"), _T("����"), _T("Ÿ��������ϱ���"), _T("Ÿ��������ϱ��л�뿩��"), strPrevRegNoFromOther);			
				if( 0 == strPrevRegNoFromOther.Compare(_T("Y")) )
				{
					StartLog(_T("���ڵ��Է�:��Ϲ�ȣ��ȯ"));
					ids = CLocCommonAPI::MakeRegNoSpaceToZeroFromLib(m_pParentMgr, strRegNoData, m_pPreRegNoMgr, m_strReturnBookLibCode);
					EndLog();
					if( 0 <= ids )
					{
						strGetBarcodeData = strRegNoData;				
					}
				}		
				dlg.m_strRegNo = strGetBarcodeData;
				dlg.m_strLibCode = m_strReturnBookLibCode;
				dlg.m_nMode = 3;
				
				if(TRUE == pOtherCheck->GetCheck())
				{
					dlg.m_bOtherReturnFirst = TRUE;
				}
				if(IDOK == dlg.DoModal())
				{
					ids = KL_UserInfoView();
					if(0 > ids) return ids;
					if(TRUE == dlg.m_bLoanStopMode)
					{ 
						CESL_Grid* pGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid"));
						if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
						pGrid->SetAt(dlg.m_nIndex, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
						pGrid->m_nReverse = dlg.m_nIndex;
						pGrid->SetReverse(dlg.m_nIndex);
					}
				}
				
				if(TRUE == dlg.m_bKLUserYN)
				{
					return 0;
				}
			}
		}
		
		
		BOOL bFind = FALSE;
		if(0 < pMemberDM->GetRowCount())
		{
			
			strGetBarcodeData.MakeUpper();
			CString strRegNoData = strGetBarcodeData;
			CLocCommonAPI::MakeRegNoSpaceToZeroAuto(strRegNoData);

			CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
 			if(NULL != pLoanDM)
			{
				CString strGetData;
 				for(INT i = 0; i < pLoanDM->GetRowCount(); i++)
 				{
 					pLoanDM->GetCellData(_T("��Ϲ�ȣ"), i, strGetData);
 					if(strGetData == strRegNoData)
 					{
 						bFind = TRUE;
						break;
 					}
 				}
			}
		}
		if(FALSE == bFind)
		{
			if(TRUE == pOtherCheck->GetCheck())
			{ 
				if(!m_pParentMgr->m_pInfo->IsConnectUnityLoanService())
				{
					
					SelfCloseMessageBox(_T("å�������� ���� �õ����Դϴ�."));
					return 0;
				}
				
				strGetBarcodeData.MakeUpper();
				CString strRegNoData = strGetBarcodeData;
				CLocCommonAPI::MakeRegNoSpaceToZeroAuto(strRegNoData);
				
				CKlOtherLoanListDlg dlg(m_pParentMgr);
				dlg.m_strRegNo = strRegNoData;
				
				ids = dlg.RefreshLoanData();
				if (0 == ids) { 
					
					if (IDOK == dlg.DoModal()) {
						
						CESL_DataMgr *pKL_LoanDM = m_pParentMgr->FindDM(_T("DM_KL_OTHER_LOAN_VIEW"));
						if(NULL == pKL_LoanDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
						CString strLoanLibCode;
						pKL_LoanDM->GetCellData(_T("���⵵������ȣ"), dlg.m_nIndex, strLoanLibCode);
						
						
						pKL_LoanDM->GetCellData(_T("����KEY"), dlg.m_nIndex, strLoanKey);

						
						ISUnionDBLoan(strGetBarcodeData, strLoanLibCode, bUnionDBLoan);
						
						
						if(bUnionDBLoan == FALSE || (bUnionDBLoan == TRUE && m_pManageValue->m_sBookCooperativeSys != _T("Y")) ){					
							ids = ISIllProcMat(strLoanKey, bIllProcMat);
							
							if ( ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
							
							if ( bIllProcMat == TRUE ) {
								AfxMessageBox(_T("������ �ڷ�� å�η������ڷ��Դϴ�. å�η��ݳ��� �Ͽ� �ֽʽÿ�."));
								return 0;
							}						
							
							CString strUserNo, strRegNo;
							pKL_LoanDM->GetCellData(_T("���մ����ڹ�ȣ"), dlg.m_nIndex, strUserNo);
							m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), strUserNo);
							ids = SPFExcutePath(_T("���ڵ����"));
							if (0 != ids) return 0;
						}
					}
				}
				else if (200 == ids) {
					
					SelfCloseMessageBox(_T("��ȸ�� ���� ��Ȳ�� �����ϴ�."));
				}
				else {
					
					SelfCloseMessageBox(_T("������Ȳ ��ȸ�� �����Ͽ����ϴ�."));
				}

				if(bUnionDBLoan == FALSE || (bUnionDBLoan == TRUE && m_pManageValue->m_sBookCooperativeSys != _T("Y")) )
				return 0;
			}
		}
	}
	
	if(m_pCheckLoan->IsSunchang())
	{		
		if(pMemberDM->GetRowCount() == 0)
		{
			CString sRegNo;
			ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), sRegNo);

			ids = SPFExcutePath(_T("ȸ��_�˻�_NOASK"));
			
			if(pMemberDM->GetRowCount() > 0)
				return 0;

			m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), sRegNo);
		}
	}

	CString sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("InputBarCode"));

	if(pMemberDM->GetRowCount() > 0)
	{
		CString sRegNo;		
		ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -32, _T("InputBarCode"));

		CString	strBarcodeReceiptPrint, strBarcodeReceiptPrintAll;
		CString	strReceiptUseFlag;
		CString strPrintItem;
		CString strPrintMode;
		CString strPrintDevice;
		CString strPreview;				
		CString strNotify;
			
		m_pManageValue->GetReceiptManageValue(strBarcodeReceiptPrint, strBarcodeReceiptPrintAll, strPrintItem,strPrintMode, strPrintDevice, strPreview, strNotify);
		
		if(sRegNo == strBarcodeReceiptPrint || sRegNo == strBarcodeReceiptPrintAll)
		{
			StartLog(_T("���ڵ��Է�:���������"));
			if(m_ReceiptDlg->GetSafeHwnd() != NULL)
			{
				if(sRegNo == strBarcodeReceiptPrint)
				{
					m_ReceiptDlg->BarcodePrintCommand();
				}
				else
				{
					m_ReceiptDlg->BarcodePrintCommand(2);
				}				
			}
			else
			{
				m_ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG(0, m_pParentMgr, m_pManageValue);
				m_ReceiptDlg->pMain = m_pParentMgr->pMain;
				m_ReceiptDlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
				m_ReceiptDlg->m_pInfo = m_pParentMgr->m_pInfo;				
				if(m_ReceiptDlg->Create(m_pParentMgr))
				{
					if(sRegNo == strBarcodeReceiptPrint)
					{
						m_ReceiptDlg->BarcodePrintCommand(1);
					}
					else
					{
						m_ReceiptDlg->BarcodePrintCommand(2);
					}	
				}
			}	
			EndLog();
			return 0;
		}

		if(!m_strReturnBookLibCode.IsEmpty() && m_strReturnBookLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
		{ 				
			CString strPrevRegNoFromOther;
			ids = m_pManageValue->GetManageValue(_T("��Ÿ"), _T("����"), _T("Ÿ��������ϱ���"), _T("Ÿ��������ϱ��л�뿩��"), strPrevRegNoFromOther);			
			if(0 == strPrevRegNoFromOther.Compare(_T("Y")) )
			{
				StartLog(_T("���ڵ��Է�:��Ϲ�ȣ��ȯ"));
				ids = CLocCommonAPI::MakeRegNoSpaceToZeroFromLib(m_pParentMgr, sRegNo, m_pPreRegNoMgr, m_strReturnBookLibCode);
				EndLog();			
			}
		}
		else
		{
			StartLog(_T("���ڵ��Է�:��Ϲ�ȣ��ȯ"));
			CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);
			EndLog();
		}
		
		CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		if(pLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -33, _T("InputBarCode"));

		CString sTmpRegNo,sBookKey,sPublishForm,sWorkingStatus,sPrevWorkingStatus,sLoanStatus;
		
		CString sBookLibILLUseYN, sBookLibKLUseYN, sLocal;		

		if(!sRegNo.IsEmpty())
		{
			for(INT i = 0 ; i < pLoanInfoDM->GetRowCount(); i++)
			{
				ids = pLoanInfoDM->GetCellData(_T("��Ϲ�ȣ"), i, sTmpRegNo);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -34, _T("InputBarCode"));

				ids = pLoanInfoDM->GetCellData(_T("����/���ӱ���"), i, sPublishForm);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -35, _T("InputBarCode"));
				ids = pLoanInfoDM->GetCellData(_T("����"), i, sWorkingStatus);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -36, _T("InputBarCode"));
				ids = pLoanInfoDM->GetCellData(_T("�������"), i, sLoanStatus);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, _T("InputBarCode"));
				ids = pLoanInfoDM->GetCellData(_T("�����ڷ����"), i, sPrevWorkingStatus);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -38, _T("InputBarCode"));
				ids = pLoanInfoDM->GetCellData(_T("����/�ݳ�����KEY"), i, strLoanKey);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -39, _T("InputBarCode"));
				ids = pLoanInfoDM->GetCellData(_T("��Ȳ����"), i, sLocal);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -42, _T("InputBarCode"));

				if(sLoanStatus.Compare(_T("1")) == 0) continue;
				if(sLoanStatus.Compare(_T("3")) == 0) continue;
				if(sLoanStatus.Compare(_T("4")) == 0) continue;
				
				if(sRegNo.Compare(sTmpRegNo) == 0)
				{
					CESL_ControlMgr *pLoanCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
					if(NULL == pLoanCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
					CESL_Grid *pGrid = (CESL_Grid*)(pLoanCM->FindControl(_T("MainGrid")));
					if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
					
					CString strGetData, strMessage;
					CString strOtherReturnKey, strLCTKEY, strLoanLibCode;
					pLoanInfoDM->GetCellData(_T("Ÿ������KEY"), i, strOtherReturnKey);
					pLoanInfoDM->GetCellData(_T("LCT_KEY"), i, strLCTKEY);
					pLoanInfoDM->GetCellData(_T("���⵵������ȣ"), i, strLoanLibCode);
					if(sLoanStatus.IsEmpty()) {
											
						pGrid->GetAt(i, pGrid->m_nSelectIdx, strGetData);
						if(pGrid->m_sSelectString != strGetData) { 
							pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
							pGrid->m_nReverse = i;
							pGrid->SetReverse(i);
						}
						
							CString strTransCode;
							pLoanInfoDM->GetCellData(_T("Ʈ����ǻ���"), i, strTransCode);
							if (_T("0") == strTransCode) { 
								SelfCloseMessageBox(_T("������ȣ���� ��û���� �ڷ��Դϴ�."));
							} else if (_T("1") == strTransCode) { 
								SelfCloseMessageBox(_T("������ȣ���� �߼����� �ڷ��Դϴ�."));
							} else if (_T("3") == strTransCode) { 
								SPFExcutePath(_T("Ÿ������"));
							} else { 
								SelfCloseMessageBox(_T("�˼����� ������ȣ���� �ڷ��Դϴ�."));
							}
				
						return 0;
					}
					
					BOOL bReturn = FALSE;
					int nOtherType = 0;
					ids = BookReturnTypeCheck(i);
															
					switch(ids)
					{
						case _RETURN_KL_TO_KL:
							m_strReturnBookLibCode = strLoanLibCode;
							if(!m_strReturnBookLibCode.IsEmpty() && m_strReturnBookLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
							{ 
								CKL_UserSearchDlg dlg(m_pParentMgr, this);
								strGetBarcodeData.MakeUpper();
								CLocCommonAPI::MakeRegNoSpaceToZero(strGetBarcodeData);

								dlg.m_strRegNo = strGetBarcodeData;
								dlg.m_strLibCode = m_strReturnBookLibCode;
								dlg.m_nMode = 3;
								
								if(TRUE == pOtherCheck->GetCheck())
								{
									dlg.m_bOtherReturnFirst = TRUE;
								}
								if(IDOK == dlg.DoModal())
								{
									ids = KL_UserInfoView();
									if(0 > ids) return ids;
									if(TRUE == dlg.m_bLoanStopMode)
									{ 
										pGrid->SetAt(dlg.m_nIndex, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
										pGrid->m_nReverse = dlg.m_nIndex;
										pGrid->SetReverse(dlg.m_nIndex);
									}
								}
							
								bReturn = TRUE;
							}
							break;
						case _RETURN_LILL_TO_BOOKLIB:
						
						
						
							pGrid->GetAt(i, pGrid->m_nSelectIdx, strGetData);
							if(pGrid->m_sSelectString != strGetData) { 
								pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
								pGrid->m_nReverse = i;
								pGrid->SetReverse(i);
							}			
						
							break;
						case _RETURN_LILL_TO_LILL:
						
							if(strOtherReturnKey.IsEmpty() == FALSE){
								nOtherType = 0; 
							}else if(strLCTKEY.IsEmpty() == FALSE && strLoanLibCode.IsEmpty() == FALSE && strLoanLibCode != m_pParentMgr->m_pInfo->LIB_CODE){
								nOtherType = 1; 
							}
							
							pGrid->GetAt(i, pGrid->m_nSelectIdx, strGetData);
							if(pGrid->m_sSelectString != strGetData) { 
								pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
								pGrid->m_nReverse = i;
								pGrid->SetReverse(i);
								
								pGrid->SelectMakeList();
								INT nIdx = pGrid->SelectGetIdx();
								if(nIdx != i)
								{
									POSITION pos = pGrid->m_arraySelect.GetHeadPosition();
									while (pos)
									{
										pGrid->m_arrayPos = pos;
										if (i == pGrid->m_arraySelect.GetNext(pos))
											break;				
									}
								}
								if(!m_pManageValue->m_nSetloanStopMode) { 
									ids = OtherReturn(nOtherType);
								}
							}	else {
								INT nIdx = pGrid->SelectGetIdx();
								if(nIdx != i)
								{
									POSITION pos = pGrid->m_arraySelect.GetHeadPosition();
									while (pos)
									{
										pGrid->m_arrayPos = pos;
										if (i == pGrid->m_arraySelect.GetNext(pos))
											break;				
									}
								}
								ids = OtherReturn(nOtherType);
							}	

							bReturn = TRUE;
							break;
						case _MESSAGE_NO_USE_KL_LILL:
							strMessage.Format(	_T("å������ȣ���� �ڷ��Դϴ�.\r\n")
												_T("���� �������� �ݳ��Ͻñ� �ٶ��ϴ�."));
							SelfCloseMessageBox(strMessage, NULL, 4000);
							bReturn = TRUE;
							break;
						case _MESSAGE_NO_USE_ILL:
							strMessage.Format(	_T("å�η� �ڷ��Դϴ�.\r\n")
												_T("���� �������� �ݳ��Ͻñ� �ٶ��ϴ�."));
							SelfCloseMessageBox(strMessage, NULL, 4000);
							bReturn = TRUE;
							break;
						case _MESSAGE_NO_USE_KL:
							strMessage.Format(	_T("å���� �ڷ��Դϴ�.\r\n")
												_T("���� �������� �ݳ��Ͻñ� �ٶ��ϴ�."));
							SelfCloseMessageBox(strMessage, NULL, 4000);
							bReturn = TRUE;
							break;
						case _RETURN_FAIL:
							bReturn = TRUE;
							break;
						default:
							break;
					}
					
					if(bReturn == TRUE){
						return 0;
					}
				
					ids = pLoanInfoDM->GetCellData(_T("åKEY"), i, sBookKey);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -38, _T("InputBarCode"));

					pDM->InsertRow(-1);
					ids = pDM->SetCellData(_T("åKEY"),sBookKey,0);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -39, _T("InputBarCode"));

					ids = pDM->SetCellData(_T("��Ϲ�ȣ"),sTmpRegNo,0);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -40, _T("InputBarCode"));

					BarCodeReturn(pDM);
					if(0 > ids) return ids;
					pDM->FreeData();
					return 0;
				}
			}
		}
	}
		
	CString sRegNo;	
	CString strUserNo;
	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -50, _T("InputBarCode"));
	
	strUserNo = sRegNo;

	StartLog(_T("���ڵ��Է�:��Ϲ�ȣ��ȯ"));
	CString strBookManageCode;

	if(sRegNo.GetLength() == 19){
		CString strBarcodeLibCode	= sRegNo.Mid(1,6);
		CString strBarcode			= sRegNo.Mid(7);
		if(pMemberDM->GetRowCount() > 0){	
			if(m_pManageValue->GetLibDMValue(strBarcodeLibCode, _T("")) == FALSE){
				CString strMsg;
				strMsg.Format(_T("�ٸ������� �ڷ�� ó�� �� �� �����ϴ�.\r\n\r\n��Ϲ�ȣ : [%s]"), strBarcode);
				m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), _T(""));
				AfxMessageBox(strMsg);
				return 0;
			}
		}else{

			CKL_UserSearchDlg dlg(m_pParentMgr, this);
			dlg.m_strRegNo = strBarcode;
			dlg.m_strLibCode = strBarcodeLibCode;
			dlg.m_nMode = 3;
			if(TRUE == pOtherCheck->GetCheck())
			{
				dlg.m_bOtherReturnFirst = TRUE;
			}		
			if(IDOK == dlg.DoModal())
			{
				ids = KL_UserInfoView();
				if(0 > ids) return ids;
				if(TRUE == dlg.m_bLoanStopMode)
				{ 
					CESL_Grid* pGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid"));
					if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
					pGrid->SetAt(dlg.m_nIndex, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
					pGrid->m_nReverse = dlg.m_nIndex;
					pGrid->SetReverse(dlg.m_nIndex);
				}
		
		
			}	
			return 0;
		}
	}
	
	GetBookManageCode(strBookManageCode);
	
	CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, strBookManageCode, m_pPreRegNoMgr);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -51, _T("InputBarCode"));
	EndLog();

	if (m_pCheckLoan->IsOtherManageReturn() > 0)
	{
		CString strRegNo = strUserNo;
		strRegNo.MakeUpper();
 		ids = CLocCommonAPI::MakeRegNoSpaceToZero(strRegNo);
 		if(0 > ids) return -2;
		
		if(!m_strReturnBookLibCode.IsEmpty() && m_strReturnBookLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
		{ 				
			CString strPrevRegNoFromOther;
			ids = m_pManageValue->GetManageValue(_T("��Ÿ"), _T("����"), _T("Ÿ��������ϱ���"), _T("Ÿ��������ϱ��л�뿩��"), strPrevRegNoFromOther);			
			if(0 == strPrevRegNoFromOther.Compare(_T("Y")) )
			{
				CString strRegNoData = strUserNo;
				StartLog(_T("���ڵ��Է�:��Ϲ�ȣ��ȯ"));
				ids = CLocCommonAPI::MakeRegNoSpaceToZeroFromLib(m_pParentMgr, strRegNoData, m_pPreRegNoMgr, m_strReturnBookLibCode);
				EndLog();
				
				if( 0 <= ids )
				{
					strRegNo = strRegNoData;				
				}
			}
		}

		CString strQuery;
		CString strValue;
		strQuery.Format(_T("SELECT USER_KEY, REC_KEY, MANAGE_CODE, STATUS, BOOK_KEY FROM LS_WORK_T01 ")
						_T("WHERE STATUS IN ('0', '2') AND REG_NO = '%s'"),	strRegNo);

		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);	
		TmpDM.RestructDataManager(strQuery);
	
		if (TmpDM.GetRowCount() == 1)
		{
			CString strUserKey;
			CString strBookManageCode;
			CString strStatus;
			CString strBookKey;

			TmpDM.GetCellData(0, 0, strUserKey);
			TmpDM.GetCellData(0, 2, strBookManageCode);
			TmpDM.GetCellData(0, 3, strStatus);
			TmpDM.GetCellData(0, 4, strBookKey);

			if (strBookManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE)
			{
				SearchUserBookFromKey(strUserKey);					
				m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), strRegNo);

				
				if(!m_pManageValue->m_nSetloanStopMode)
				{ 
					
					ids = BarCodeReturnBookCheck( strBookKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -5 , _T("BarCodeReturn") ); 
					
					ReturnBook(FALSE);
				}
				else
				{ 
					
					ids = BarCodeReturnBookCheck( strBookKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -5 , _T("BarCodeReturn") );
				}
				return 0;
			}
		}
	}
	
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	
	if(sRegNo.GetLength() > nRegCodeLength)
	{
		CString sDescRegCode;
		CString sRegCode = sRegNo.Left(nRegCodeLength);
	
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��ϱ���"), sRegCode, sDescRegCode);
		
		if(sDescRegCode.IsEmpty() && sRegNo.GetLength() != 12)
		{
			StartLog(_T("���ڵ��Է�:�̿��ڰ˻��غ�"));
			CString sSql;
			CString sWhere;
			MakeSearchMemberSql(sWhere);

			sSql.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL CLU WHERE (CLU.USER_CLASS!='3' OR CLU.MEMBER_CLASS!='1') AND %s"), sWhere);

			CESL_DataMgr DM;
			DM.SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pParentMgr->m_pInfo->MODE));

			CString sCount;
			CLocCommonAPI::GetOneValueQuery(m_pParentMgr, &DM, sSql, sCount);
			EndLog();

			if(_ttoi(sCount) > 0)
			{
				ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), strUserNo);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("InputBarCode"));
				
				ids = SPFExcutePath(_T("ȸ��_�˻�"));				
				
				if(1 == ids)
				{					
					return 0;
				}				

				ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), sRegNo);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("InputBarCode"));

				if(pMemberDM->GetRowCount() > 0)
					return 0;
			}
		}
	}

	StartLog(_T("���ڵ��Է�:å�����˻�"));
	ids = GetBarCodeBookInfo(pDM);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("InputBarCode"));
	EndLog();
	
	CESL_DataMgr ILLDM;
	ILLDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);	
	CString strILLRegNo;

	StartLog(_T("���ڵ��Է�:å�η��ݳ�"));

	INT nOtherManageReturn = m_pCheckLoan->IsOtherManageReturn();
	INT nILLCnt = -1;
	BOOL bIsILLWork = FALSE;
	if(_T("Y") == m_pManageValue->m_sBookCooperativeSys || nOtherManageReturn == 1)
	{ 
		int ids = 0;
		CString  strRegNo;
		ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), strILLRegNo);
 		strILLRegNo.MakeUpper();

		ids = CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, strILLRegNo, strBookManageCode, m_pPreRegNoMgr);		
 		if(0 > ids) return -2;

		CString strQuery;
		CString strValue;
		if(nOtherManageReturn == 1)
		{
			strQuery.Format(
				_T("SELECT USER_KEY, REC_KEY, MANAGE_CODE, STATUS, BOOK_KEY FROM LS_WORK_T01 ")
				_T("WHERE REG_NO = '%s' AND MANAGE_CODE NOT IN ('%s') AND STATUS IN ('0', '2') "),
				strILLRegNo, m_pParentMgr->m_pInfo->MANAGE_CODE);
		}
		else
		{
			if(_T("I") == m_pManageValue->m_sBookCooperativeMode)
			{
				strQuery.Format(
					_T("SELECT USER_KEY, REC_KEY, MANAGE_CODE, STATUS, BOOK_KEY FROM LS_WORK_T01 ")
					_T("WHERE REG_NO = '%s' AND ((MANAGE_CODE NOT IN ('%s') AND STATUS IN ('0', '2')) OR (STATUS IN ('L', 'D')))"),
					strILLRegNo, m_pParentMgr->m_pInfo->MANAGE_CODE);
			}
			else if(_T("D") == m_pManageValue->m_sBookCooperativeMode)
			{
				strQuery.Format(
					_T("SELECT USER_KEY, REC_KEY, MANAGE_CODE, STATUS, BOOK_KEY FROM LS_WORK_T01 ")
					_T("WHERE REG_NO = '%s' AND ((MANAGE_CODE NOT IN ('%s') AND STATUS IN ('0', '2')) OR (STATUS IN ('L', 'D'))) UNION ")
					_T("SELECT LS.USER_KEY, LS.REC_KEY, LS.MANAGE_CODE, LS.STATUS, LS.BOOK_KEY ")
					_T("FROM LS_WORK_T01 LS, ILL_TRANS_TBL ITL ")
					_T("WHERE LS.REC_KEY = ITL.LOAN_KEY AND ITL.REG_NO = '%s' AND LS.STATUS = 'O' AND ITL.APL_LIBCODE = '%s' AND ITL.STATE = '0014'"),
					strILLRegNo, m_pParentMgr->m_pInfo->MANAGE_CODE, strILLRegNo, m_pParentMgr->m_pInfo->LIB_CODE);
			}
		}
		
		ILLDM.RestructDataManager(strQuery);
		nILLCnt = ILLDM.GetRowCount();

	} 
	
	EndLog();
	if(ids > 0 && 0 >= nILLCnt)
	{
		ids = BarCodeMemberSearch();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("InputBarCode"));

		if(ids > 0) 
		{
			ids = IsSearchedUser();
			if(0 > ids) return ids;
			if(ids > 0) 
			{
				MoveRegNoToNextProcBasket(EXP_LOAN);
			}

			if(ids == 0) 
			{
				ids = MessageBox(m_pParentMgr->m_hWnd, _T("�Է��� ��Ϲ�ȣ�� ���� �ڷᰡ �������� �ʽ��ϴ�. ��޴����Ͻðڽ��ϱ�?"), _T("����/�ݳ�"), MB_YESNO | MB_ICONQUESTION);
				if(ids == IDYES)
				{
					return 1;
				}
				return 0;
			}
		}

		return 0;
	}
	else if(ids > 0 && nILLCnt > 0)
	{
		bIsILLWork = TRUE;
	}
	
	
	else if(ids == 0 && nILLCnt > 0)
	{
		CString sBookWorkingStatus;
		ids = pDM->GetCellData(_T("����"), 0, sBookWorkingStatus);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("InputBarCode"));
		}
	
		
		if(pMemberDM->GetRowCount() == 0 && 
			((sBookWorkingStatus.Compare(_T("BOL112N"))==0) ||
			(sBookWorkingStatus.Compare(_T("SEL212N"))==0) ||
			(sBookWorkingStatus.Compare(_T("SEL112N"))==0) ||
			(sBookWorkingStatus.Compare(_T("BOL411O"))==0) || 
			(sBookWorkingStatus.Compare(_T("SEL411O"))==0)))
		{
				bIsILLWork = TRUE;
		}
		else if((sBookWorkingStatus.Compare(_T("BOL411O"))==0) || 
			(sBookWorkingStatus.Compare(_T("SEL411O"))==0))
		{
				bIsILLWork = TRUE;
		}
		else
		{
			ids = MessageBox(m_pParentMgr->m_hWnd, _T("�Է��� ��Ϲ�ȣ�� ���� �ڷᰡ �ڰ��� �ڷ��Դϱ�?\r\n\r\nYES : �ڰ��ڷ�� ó��, NO : Ÿ���ڷ�� ó��, CANCEL : ���"), _T("����/�ݳ�"), MB_YESNOCANCEL | MB_ICONQUESTION);
			if(IDYES == ids)
			{
				bIsILLWork = FALSE;
			}
			else if(IDNO == ids)
			{
				bIsILLWork = TRUE;
			}
			else
			{
				return 1000;
			}
		}
	}

	if(FALSE == bIsILLWork)
	{
	CString sLoanUserKey, sIsSelfReturning;
	CString sBookWorkingStatus;
	ids = pDM->GetCellData(_T("����"), 0, sBookWorkingStatus);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("InputBarCode"));
	
	ids = pDM->GetCellData(_T("������KEY"), 0, sLoanUserKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("InputBarCode"));
	ids = pDM->GetCellData(_T("���ιݳ��߿���"), 0, sIsSelfReturning);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("InputBarCode"));
	
	
	if(_ttoi(sIsSelfReturning) > 0)
	{
		CString sMsg;
		sMsg.Format(_T("'���ιݳ���'�� �ڷ��̱� ������ ó�� �� �� �����ϴ�. "));
		SelfCloseMessageBox(sMsg);		
		pDM->FreeData();

		return 3;
	}
	
	if(sLoanUserKey.IsEmpty())
	{
		if(sBookWorkingStatus.Compare(_T("BOL112N"))==0 ||
			(sBookWorkingStatus.Compare(_T("SEL212N"))==0) ||
			(sBookWorkingStatus.Compare(_T("SEL112N"))==0)
			)
		{
			ids = BarCodeLoan();
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("InputBarCode"));
			pDM->FreeData();
			return 0;			
		}
		else
		{
			CLocCommonAPI::GetWorkingStatusDescToCode(sBookWorkingStatus, 1);
			CString sMsg;
			sMsg.Format(_T("'%s'�̱� ������ ���� �� �� �����ϴ�. "), sBookWorkingStatus);
			SelfCloseMessageBox(sMsg);
			
			pDM->FreeData();
			return 0;
		}
	}
	
	else
	{
		ids = BarCodeReturn(pDM);
		if(0 > ids) return ids;
		pDM->FreeData();
		return 0;		
	}
	}
	
	else
	{
		CString strUserKey;
		CString strBookManageCode;
		CString strStatus;
		CString strBookKey;
		BOOL bUseSearchUser = FALSE;
		
		if(1 == nILLCnt)
		{
			ILLDM.GetCellData(0, 0, strUserKey);
			
			ILLDM.GetCellData(0, 2, strBookManageCode);
			ILLDM.GetCellData(0, 3, strStatus);
			ILLDM.GetCellData(0, 4, strBookKey);
			bUseSearchUser = TRUE;
		}
		else if(1 < nILLCnt)
		{
			CString strRecKeyList;
			
			CString strRecKey;			
			const int ROW_COUNT = ILLDM.GetRowCount();
			for(int nRow = 0; nRow < ROW_COUNT; nRow++)
			{
				ILLDM.GetCellData(nRow, 1, strRecKey);
				strRecKeyList = strRecKeyList + _T(", ") + strRecKey;
			}
			strRecKeyList.TrimLeft(_T(", "));
			
			CILL_RETURN_SEARCH_BOOK Dlg(m_pParentMgr);
			Dlg.m_sBookCooperativeMode = m_pManageValue->m_sBookCooperativeMode;
			Dlg.m_strOption = _T("A.REC_KEY IN (") + strRecKeyList + _T(")");
			if(IDOK == Dlg.DoModal())
			{
				strUserKey = Dlg.m_strUserKey;
				strStatus = Dlg.m_strStatus;
				strBookKey = Dlg.m_strBookKey;
				strBookManageCode = Dlg.m_strBookManageCode;
				bUseSearchUser = TRUE;
			}
		}	

		if(TRUE == bUseSearchUser)
		{
		
		
			CString strTmpQuery, strBarcodeUseYN, strGetData, strMessage, strOtherReturnKey, strLCTKEY, strLoanLibCode, sTmpRegNo;
			BOOL bReturn = FALSE;
			int nOtherType = 0, nids = 0, i = 0;

			CESL_ControlMgr *pLoanCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
			if(NULL == pLoanCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
			CESL_Grid *pGrid = (CESL_Grid*)(pLoanCM->FindControl(_T("MainGrid")));
			if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
			CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
			if(pLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -33, strFuncName);
		
			ids = BookReturnTypeCheck(strILLRegNo);
			if(ids == _RETURN_FAIL){
				
				SearchLoanInfo(FALSE, strUserKey);
				ids = BookReturnTypeCheck(strILLRegNo);
			}

			for(i = 0 ; i < pLoanInfoDM->GetRowCount(); i++)
			{
				nids = pLoanInfoDM->GetCellData(_T("��Ϲ�ȣ"), i, sTmpRegNo);
				if(0 > nids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -34, _T("InputBarCode"));

				if(sTmpRegNo.Compare(strILLRegNo) == 0){
					pLoanInfoDM->GetCellData(_T("Ÿ������KEY"), i, strOtherReturnKey);
					pLoanInfoDM->GetCellData(_T("LCT_KEY"), i, strLCTKEY);
					pLoanInfoDM->GetCellData(_T("���⵵������ȣ"), i, strLoanLibCode);
					break;
				}
			}

			switch(ids)
			{
				case _RETURN_KL_TO_KL:
					m_strReturnBookLibCode = strLoanLibCode;
					if(!m_strReturnBookLibCode.IsEmpty() && m_strReturnBookLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
					{ 
						CKL_UserSearchDlg dlg(m_pParentMgr, this);
						strGetBarcodeData.MakeUpper();
						CLocCommonAPI::MakeRegNoSpaceToZero(strGetBarcodeData);
	
						dlg.m_strRegNo = strGetBarcodeData;
						dlg.m_strLibCode = m_strReturnBookLibCode;
						dlg.m_nMode = 3;
						
						if(TRUE == pOtherCheck->GetCheck())
						{
							dlg.m_bOtherReturnFirst = TRUE;
						}
						if(IDOK == dlg.DoModal())
						{
							ids = KL_UserInfoView();
							if(0 > ids) return ids;
							if(TRUE == dlg.m_bLoanStopMode)
							{ 
								pGrid->SetAt(dlg.m_nIndex, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
								pGrid->m_nReverse = dlg.m_nIndex;
								pGrid->SetReverse(dlg.m_nIndex);
							}
						}
						bReturn = TRUE;
					}
					break;
				case _RETURN_LILL_TO_BOOKLIB:
					pGrid->GetAt(i, pGrid->m_nSelectIdx, strGetData);
					if(pGrid->m_sSelectString != strGetData) { 
						pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
						pGrid->m_nReverse = i;
						pGrid->SetReverse(i);
					}				
					
					break;
				case _RETURN_LILL_TO_LILL:
					pGrid->GetAt(i, pGrid->m_nSelectIdx, strGetData);
					if(pGrid->m_sSelectString != strGetData) { 
						pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
						pGrid->m_nReverse = i;
						pGrid->SetReverse(i);
						
						INT nIdx = pGrid->SelectGetIdx();
						if(nIdx != i)
						{
							POSITION pos = pGrid->m_arraySelect.GetHeadPosition();
							while (pos)
							{
								pGrid->m_arrayPos = pos;
								if (i == pGrid->m_arraySelect.GetNext(pos))
									break;				
							}
						}
						if(!m_pManageValue->m_nSetloanStopMode) { 
							ids = OtherReturn();
						}
					}	else {
						
						INT nIdx = pGrid->SelectGetIdx();
						if(nIdx != i)
						{
							POSITION pos = pGrid->m_arraySelect.GetHeadPosition();
							while (pos)
							{
								pGrid->m_arrayPos = pos;
								if (i == pGrid->m_arraySelect.GetNext(pos))
									break;				
							}
						}
						ids = OtherReturn();
					}																				
					bReturn = TRUE;
					break;

				case _RETURN_OTHER_MANAGE:	
				case _RETURN_OTHER_MANAGE_HOLD:	
				
					SearchUserBookFromKey(strUserKey);		
					m_pParentMgr->SetControlData(	_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), strILLRegNo);
					
					if(!m_pManageValue->m_nSetloanStopMode)
					{ 
						
						nids = BarCodeReturnBookCheck(strBookKey);
						if(0 >nids) {
							bReturn = TRUE;
							break;
						}
						
						ReturnBook(FALSE);					
					}
					else
					{ 
						
						nids = BarCodeReturnBookCheck(strBookKey);
						if(0 >nids) {
							bReturn = TRUE;
							break;
						}

					}
					bReturn = TRUE;
					break;
					
				case _RETURN_NORMAL_TO_ILL:	
				case _RETURN_ILL_TO_BOOKLIB:	
				case _RETURN_ILL_TO_ILL:	
					SearchUserBookFromKey(strUserKey);	
					m_pParentMgr->SetControlData(	_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), strILLRegNo);
										
					strTmpQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE VALUE_NAME='���ڵ�ݳ���뿩��' AND MANAGE_CODE = '%s'"),m_pParentMgr->m_pInfo->MANAGE_CODE);
					pDM->GetOneValueQuery(strTmpQuery,strBarcodeUseYN);
					
					if(strBarcodeUseYN == _T("N"))
					{
						SPFExcutePath(_T("å�η��ݳ�ȭ��"));
					}
					else{
						if(!m_pManageValue->m_nSetloanStopMode)
						{
							ILL_BarCodeReturn(strBookKey);
						}
						else
						{
							BarCodeReturnBookCheck(strBookKey);
						}
					}
					
					bReturn = TRUE;
					break;

				case _MESSAGE_NO_USE_KL_LILL:
					strMessage.Format(	_T("å������ȣ���� �ڷ��Դϴ�.\r\n")
										_T("���� �������� �ݳ��Ͻñ� �ٶ��ϴ�."));
					SelfCloseMessageBox(strMessage, NULL, 4000);
					bReturn = TRUE;
					break;
				case _MESSAGE_NO_USE_ILL:
					strMessage.Format(	_T("å�η� �ڷ��Դϴ�.\r\n")
										_T("���� �������� �ݳ��Ͻñ� �ٶ��ϴ�."));
					SelfCloseMessageBox(strMessage, NULL, 4000);
					bReturn = TRUE;
					break;
				case _MESSAGE_NO_USE_KL:
					strMessage.Format(	_T("å���� �ڷ��Դϴ�.\r\n")
										_T("���� �������� �ݳ��Ͻñ� �ٶ��ϴ�."));
					SelfCloseMessageBox(strMessage, NULL, 4000);
					bReturn = TRUE;
					break;
				case _RETURN_FAIL:
					bReturn = TRUE;
					break;
				default:
					break;
			}
			
			if(bReturn == TRUE){
				return nids;
			}
			
		}
	}
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetBarCodeBookInfo(CESL_DataMgr *pDM)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetBarCodeBookInfo");

	pDM->FreeData();

	CString sRegNo;

	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	
	if(sRegNo.GetLength() <= nRegCodeLength) return 2;
	
	CString strBookManageCode;
	GetBookManageCode(strBookManageCode);
	CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, strBookManageCode, m_pPreRegNoMgr);
	
	ids = InitExpDM();
	if(0 > ids) return ids;

	INT nBookCnt = 0;
	BOOL bRegNo = TRUE;

	for(INT i = 0 ; i < 2 ; i++)
	{
		ids = SearchBookInfo(sRegNo, nBookCnt, i, bRegNo);
		if(0 > ids) return ids;

		if(nBookCnt > 0) i = 2;
	}

	if(nBookCnt < 1) return 1;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::BarCodeMemberSearch()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("BarCodeMemberSearch");

	CString sRegNo;
	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sSql;
	
	CString sTmp = sRegNo;
	sTmp.MakeUpper();

	CString sWhere;
	MakeSearchMemberSql(sWhere);

	sSql.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL CLU WHERE (CLU.USER_CLASS!='3' OR CLU.MEMBER_CLASS!='1') AND %s"), sWhere);

	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pParentMgr->m_pInfo->MODE));

	CString sCount;
	CLocCommonAPI::GetOneValueQuery(m_pParentMgr, &DM, sSql, sCount);

	if(_ttoi(sCount) < 1) return 1;	
	
	ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	ids = SPFExcutePath(_T("ȸ��_�˻�"));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::BarCodeLoan()
{
EFS_BEGIN

	INT ids;
	
	ids = IsSearchedUser();
	if(0 > ids) return ids;
	if(ids > 0)
	{
		MoveRegNoToNextProcBasket(EXP_LOAN);
		return 0;
	}
	
	SPFExcutePath(_T("����"));
	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::BarCodeReturn(CESL_DataMgr *pDM)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("BarCodeReturn");

	CESL_DataMgr* pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pMemberDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sUserKey;
	ids = pDM->GetCellData(_T("������KEY"), 0, sUserKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CString sBookKey;
	ids = pDM->GetCellData(_T("åKEY"), 0, sBookKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	
	
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_SEARCH_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CButton *pOtherCheck = (CButton*)pCM->FindControl(_T("Ÿ���ݳ��켱"));
	if(NULL == pOtherCheck) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	
	if(pMemberDM->GetRowCount() == 0)
	{
		
		INT ids = SearchUserBookFromKey(sUserKey);
		if(0 != ids)
		{
			
			return ids;
		}
		if(!m_pManageValue->m_nSetloanStopMode)
		{ 
			ids = BarCodeReturn(pDM);
			if(0 > ids) return ids;
		}
		else
		{			
			ids = BarCodeReturnBookCheck(sBookKey);
			if(0 > ids) return ids;
		}
		return 0;
	}
	
	CString sStatus;
	
	CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	
	CString sTmpBookKey;
	CString sDelayDay;
	CString sSelect;
	
	CString strTmpQuery;
	BOOL    bDelayFlag = FALSE;
	if(!sBookKey.IsEmpty())
	{
		for(INT i = 0 ; i < pLoanInfoDM->GetRowCount(); i++)
		{
			ids = pLoanInfoDM->GetCellData(_T("�������"), i, sStatus);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);

			if(sStatus.Compare(_T("3")) == 0) continue;
			if(sStatus.Compare(_T("1")) == 0) continue;
			if(sStatus.Compare(_T("4")) == 0) continue;

			ids = pLoanInfoDM->GetCellData(_T("åKEY"), i, sTmpBookKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
			
			if(sBookKey.Compare(sTmpBookKey) == 0)
			{
				CString strTmpQuery, strMessage;
				CString strBarcodeUseYN = _T("N");
				BOOL bReturn = FALSE;
				int nOtherType = 0;
				ids = BookReturnTypeCheck(i);
				
				switch(ids)
				{
					case _RETURN_NORMAL_TO_ILL:	
					case _RETURN_ILL_TO_BOOKLIB:	
					case _RETURN_ILL_TO_ILL:	
					
						if(TRUE == pOtherCheck->GetCheck()) {							
							if(IDNO == AfxMessageBox(_T("Ÿ���ݳ��켱�� üũ�Ǿ� ������, å�η�ó���� �����մϴ�. ����Ͻðڽ��ϱ�?"), MB_ICONQUESTION|MB_YESNO) ) {
								bReturn = TRUE;
								break;
							}
						}
						
						strTmpQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE VALUE_NAME='���ڵ�ݳ���뿩��' AND MANAGE_CODE = '%s'"),m_pParentMgr->m_pInfo->MANAGE_CODE);
						pDM->GetOneValueQuery(strTmpQuery,strBarcodeUseYN);
						if(strBarcodeUseYN == _T("N"))
						{
							SPFExcutePath(_T("å�η��ݳ�ȭ��"));
						}
						else
						{
							CESL_Grid* pGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid"));
							if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
							CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
							
							CString strCompBookKey = _T("");
							CString strCheck = _T("");
							for(INT i = 0 ; i < pLoanDM->GetRowCount() ; i++)
							{
								pLoanDM->GetCellData(_T("åKEY"),i,strCompBookKey);
								if(strCompBookKey.Compare(sBookKey)==0)
								{
									
									if(!m_pManageValue->m_nSetloanStopMode){
										BarCodeReturnBookCheck(sBookKey);												
									}
									pGrid->SelectMakeList();
									pGrid->GetAt(i,0,strCheck);

									if(strCheck == _T("V"))
									{
										ILL_BarCodeReturn(sBookKey);
									}
									else
									{
										BarCodeReturnBookCheck(sBookKey);
									}
								}
							}
							
						}
						
						bReturn = TRUE;
						break;
					case _RETURN_LILL_TO_BOOKLIB:
						break;
					case _MESSAGE_NO_USE_KL_LILL:
						strMessage.Format(	_T("å������ȣ���� �ڷ��Դϴ�.\r\n")
											_T("���� �������� �ݳ��Ͻñ� �ٶ��ϴ�."));
						SelfCloseMessageBox(strMessage, NULL, 4000);
						bReturn = TRUE;
						break;
					case _MESSAGE_NO_USE_ILL:
						strMessage.Format(	_T("å�η� �ڷ��Դϴ�.\r\n")
											_T("���� �������� �ݳ��Ͻñ� �ٶ��ϴ�."));
						SelfCloseMessageBox(strMessage, NULL, 4000);
						bReturn = TRUE;
						break;
					case _MESSAGE_NO_USE_KL:
						strMessage.Format(	_T("å���� �ڷ��Դϴ�.\r\n")
											_T("���� �������� �ݳ��Ͻñ� �ٶ��ϴ�."));
						SelfCloseMessageBox(strMessage, NULL, 4000);
						bReturn = TRUE;
						break;
					case _RETURN_FAIL:

						bReturn = TRUE;
						break;
					default:
						break;
				}
				
				if(bReturn == TRUE){
					return 0;
				}
				
				ids = BarCodeReturnBookCheck(sBookKey);
				if(0 > ids) return ids;
				
				if(m_pManageValue->m_sFeeMngSys == _T("Y") || m_pManageValue->m_ArrearCheckYN == _T("Y"))
				{ 
					
					CString strTempBookKey;
					CString sDelayDay;
					
					
					
					for (INT y=0 ; y<pLoanInfoDM->GetRowCount() ; y++)
					{
						pLoanInfoDM->GetCellData(_T("åKEY"), y, strTempBookKey);					
						if(strTempBookKey == sBookKey)
						{
							ids = pLoanInfoDM->GetCellData(_T("��ü�ϼ�"), y, sDelayDay);
							if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
							if(_ttoi(sDelayDay) > 0) bDelayFlag = TRUE;
							break;
						}					
					}
					ReturnBook(bDelayFlag);
				}
				else
				{ 
					ReturnBook(FALSE);
				}

				return 0;
			}
		}
	}

	if(pMemberDM->GetRowCount()>0) 
	{
		::MessageBeep(0xFFFFFFFF);
	}

	SearchUserBookFromKey(sUserKey);
	
 	if(!m_pManageValue->m_nSetloanStopMode)
 	{ 
 		ids = BarCodeReturn(pDM);
 		if(0 > ids) return ids;
 	}
 	else
 	{
 		
 		ids = BarCodeReturnBookCheck(sBookKey);
 		if(0 > ids) return ids;
 	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::BarCodeReturnBookCheck(CString sBookKey, INT nFlag/*=0*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("BarCodeReturnBookCheck");

	CString sLoanDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(sLoanDMAlias);
	if(pLoanDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT nCheckColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, _T("����"), nCheckColIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName); 


	CString sLoanBookKey;
	BOOL bCheck = FALSE;
	for(INT i = 0 ; i < pLoanDM->GetRowCount() ; i++)
	{
		if(nCheckColIndex == -1) continue;

		pGrid->SetAt(i, nCheckColIndex, _T(""));
		
		if(nFlag == 0)
		{
			ids = pLoanDM->GetCellData(_T("åKEY"), i, sLoanBookKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
		else if(nFlag == 1)
		{
			ids = pLoanDM->GetCellData(_T("��Ϲ�ȣ"), i, sLoanBookKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
		
		if(sLoanBookKey.Compare(sBookKey) != 0) continue;

		pGrid->SetAt(i, nCheckColIndex, _T("V"));
		pGrid->m_nReverse = i;
		pGrid->SetReverse(i);
		bCheck = TRUE;
	}

	
	if(!bCheck) return 2;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::MakeRegNoSpaceToZero(BOOL bIsUsePrevRegNo)
{
EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	CString sRegNo;

	ids = m_pParentMgr->GetControlData(sCMAlias, _T("�����ڹ�ȣ"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("MakeRegNoSpaceToZero")); 
	
	if(bIsUsePrevRegNo)
	{
		CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);
	}
	else
	{
		CLocCommonAPI::MakeRegNoSpaceToZero(sRegNo);
	}

	ids = m_pParentMgr->SetControlData(sCMAlias, _T("�����ڹ�ȣ"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("MakeRegNoSpaceToZero")); 
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::MakeShelfCode(CString sDMAlias)
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, _T("CBL_LOC_LOAN_PROC::MakeShelfCode(CString sDMAlias)"));
	}

	INT nRowCount = pDM->GetRowCount();
	
	INT i = 0;
	CString strPublishFormCode;
	CString sShelfCode;
	for(i = 0; i < nRowCount; i++)
	{
		pDM->GetCellData(_T("����/���ӱ���"), i, strPublishFormCode);
		if(_T("NB") == strPublishFormCode || _T("���") == strPublishFormCode)
		{
			continue;
		}

		ids = CLocCommonAPI::MakeShelfCode(m_pParentMgr, pDM, i, sShelfCode);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("CBL_LOC_LOAN_PROC::MakeShelfCode(CString sDMAlias)"));
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitRegNoControl()
{
EFS_BEGIN

	INT ids;

	ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("InitRegNoControl")); 
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitUserManageValue()
{
EFS_BEGIN

	INT ids;

	CString sReturnDelayDay;
	ids = m_pCheckLoan->GetReturnDelayDay(sReturnDelayDay, _T("NO_REG_CODE"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("InitUserManageValue")); 
	if(ids > 0) return 0;
	
	ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�ݳ�������"), sReturnDelayDay);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("InitUserManageValue")); 

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsReLoanBook()
{
EFS_BEGIN

	INT ids;
		
	CString sLoanDate;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("�ұ޴�����"), sLoanDate);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, _T("IsReLoanBook")); 
	}

	ids = m_pCheckLoan->IsReLoanBook(sLoanDate);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, _T("IsReLoanBook")); 
	}
	if(ids > 0)
	{
		return ids;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::CopyGridCheckToDM()
{
EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("CopyGridCheckToDM")); 


	CESL_Grid *pGrid = GetMainGridPtr();
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("CopyGridCheckToDM")); 

	INT nCheckColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, _T("����"), nCheckColIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("CopyGridCheckToDM")); 

	CString sCheck;
	for(INT i = 0 ; i < pDM->GetRowCount() ; i++)
	{
		if(nCheckColIndex == -1) continue;

		ids = pGrid->GetAt(i, nCheckColIndex, sCheck);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("CopyGridCheckToDM"));

		pDM->SetCellData(_T("����"), _T(""), i);

		if(sCheck.Compare(_T("V")) != 0) continue;

		pDM->SetCellData(_T("����"), _T("V"), i);
	}
	
	return 0;

EFS_END
return -1;
}

CESL_Grid* CBL_LOC_LOAN_PROC::GetMainGridPtr()
{
EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) return NULL;
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(pGrid == NULL) return NULL;

	return pGrid;

EFS_END
return NULL;
}

INT CBL_LOC_LOAN_PROC::RunAutoProc()
{
EFS_BEGIN

	AfxBeginThread(RunAutoThreadProc, this);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GoAutoProc()
{
	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3191_AUTO_JOB");
	CString sGridAlias = _T("�׸���α�");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -11, _T("GoAutoProc"));
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -12, _T("GoAutoProc"));

	while(pGrid->GetSafeHwnd==NULL) ;

	ids = InitResultScreen();
	if(0 > ids) return ids;
	
	if(m_bBreak) return 100;

	
	ids = AllChangeUserClass();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("GoAutoProc"));

	ids = SetResultScreen(50, _T("���� ���� ȸ�� �ڵ� �ϰ� ���� �۾� �Ϸ�"));
	if(0 > ids) return ids;

	
	ids = SetResultScreen(50, _T("50% �Ϸ�"));
	if(0 > ids) return ids;

	
	if(m_bBreak) return 100;

	
	ids = AllChangeReserveCancel();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("GoAutoProc()"));

	ids = SetResultScreen(100, _T("���� ������ ���� ���� �ڷ� �ڵ� ���� ��� �۾� �Ϸ�"));
	if(0 > ids) return ids;

	ids = SetResultScreen(100, _T("�۾� �Ϸ�"));
	if(0 > ids) return ids;

	return 0;
}

UINT RunAutoThreadProc(LPVOID lParam)
{
	INT ids;
	CString sErrorMsg;

	CBL_LOC_LOAN_PROC *pObj = (CBL_LOC_LOAN_PROC *)lParam;
	if(pObj == NULL)
		return -1;

	pObj->m_bStop = FALSE;
	pObj->m_bBreak = FALSE;

	ids = pObj->GoAutoProc();
	if(ids == 100)
	{
		pObj->m_bStopState = 1;
		sErrorMsg = _T("�����Ǿ����ϴ�.");
	}
	else if(0 > ids)
	{
		pObj->m_bStopState = 1;
		sErrorMsg.Format(_T("���� �߻�-%d"),ids);
	}
	else if(ids == 0)
	{
		sErrorMsg = _T("�Ϸ�Ǿ����ϴ�.");
	}

	pObj->SetResultScreen(100, sErrorMsg);
	
	pObj->m_bStopState = 0;
	pObj->m_bStop = TRUE;

	return 0;
}

INT CBL_LOC_LOAN_PROC::InitResultScreen()
{
EFS_BEGIN

	INT ids;
	
	ids = InitResultProgressBar();
	if(0 > ids) return ids;

	ids = InitResultLog();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitResultProgressBar()
{
EFS_BEGIN

	CProgressCtrl *pProgressCtrl;

	pProgressCtrl = GetProgressPtr(m_sResultCMAlias, m_sResultProgressAlias);
	if(pProgressCtrl == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("InitResultProgressBar"));

	pProgressCtrl->SetRange(0,100);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitResultLog()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("InitResultLog");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(m_sResultDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	pDM->FreeData();

	ids = ResultLogAdd(_T("�۾�����"));
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SetResultScreen(INT nPos, CString sLog)
{
	INT ids;

	ids = ResultPorgressSetPos(nPos);
	if(0 > ids) return ids;

	ids = ResultLogAdd(sLog);
	if(0 > ids) return ids;

	return 0;
}

INT CBL_LOC_LOAN_PROC::ResultPorgressSetPos(INT nPos)
{
EFS_BEGIN

	CProgressCtrl *pProgressCtrl;

	pProgressCtrl = GetProgressPtr(m_sResultCMAlias, m_sResultProgressAlias);
	if(pProgressCtrl == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("ResultPorgressSetPos"));

	pProgressCtrl->SetPos(nPos);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ResultLogAdd(CString sLog)
{
	INT ids;
	CString strFuncName = _T("ResultLogAdd");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(m_sResultDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InsertRow(-1);

	CString sCurrentTime;
	CTime tCurrentTime = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, sCurrentTime);

	ids = pDM->SetCellData(_T("�ð�"), sCurrentTime, pDM->GetRowCount()-1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	ids = pDM->SetCellData(_T("����"), sLog, pDM->GetRowCount()-1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	INT nIndex = pDM->GetRowCount()-1;

	CString sGridAlias = _T("�׸���α�");

	ids = CLocCommonAPI::InsertGridRow(m_pParentMgr, m_sResultCMAlias, sGridAlias, nIndex, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName); 

	return 0;
}

CProgressCtrl* CBL_LOC_LOAN_PROC::GetProgressPtr(CString sCMAlias, CString sProgressAlias)
{
EFS_BEGIN

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) return NULL;

	CESL_Control_Element *pProgressElemnet;
	CProgressCtrl *pProgressCtrl;
	
	pProgressElemnet = pCM->FindControlElement(sProgressAlias);
	if(pProgressElemnet == NULL) return NULL;

	pProgressCtrl = (CProgressCtrl*)(m_pParentMgr->GetDlgItem(pProgressElemnet->CTRL_ID));
	if(pProgressCtrl == NULL) return NULL;

	return pProgressCtrl;	

EFS_END
return NULL;
}

INT CBL_LOC_LOAN_PROC::AllChangeUserClass()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeUserClass");

	ids = SetResultScreen(10, _T("���� ���� ȸ�� �ϰ� ���� ����"));
	if(0 > ids) return ids;

	ids = m_pCheckLoan->GetAutoChangeUserClass();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(ids > 0) 
	{
		ids = SetResultScreen(30, _T("������ ���� ���� ȸ�� �ڵ� �ϰ� ���� �������� �����������Ƿ� �۾��� �������� ����"));
		if(0 > ids) return ids;

		return 1;
	}

	ids = SetResultScreen(40, _T("DB�۾���"));
	if(0 > ids) return ids;

	ids = AllChangeUserClassDBProc();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeUserClassDBProc()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeUserClassDBProc");
	CESL_DataMgr *pUserDM;
	CString	sUpdateSql;
	CString	strUserUpdateKeyList[2048];
	CString	strManageValue;
	
	if(_T("Y") == m_pManageValue->m_sBookCooperativeSys)
	{
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
		CString strQuery;
		strQuery.Format(
			_T("UPDATE CO_LOAN_USER_TBL CLU ")
			_T("SET CLU.ILL_EXPIRE_RETURN_CNT=0,CLU.ILL_APL_STOP_SET_DATE=NULL,CLU.ILL_APL_STOP_DATE=NULL ")
			_T("WHERE CLU.ILL_APL_STOP_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD');")
		);
		TmpDM.StartFrame();
		TmpDM.AddFrame(strQuery);	
		TmpDM.SendFrame();
		TmpDM.EndFrame();
	}
	
	
	strManageValue = m_pManageValue->m_strUserShare;
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{
		pUserDM = m_pParentMgr->FindDM(_T("DM_SHARE_LOAN_USER_LIST"));
		if(pUserDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		sUpdateSql.Format(_T("CLU.USER_CLASS='1' AND (CLU.LOAN_STOP_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') OR CLU.LOAN_STOP_DATE IS NULL) AND CLU.REC_KEY NOT IN (SELECT DISTINCT CL.USER_KEY FROM LS_WORK_T01 CL WHERE CLU.USER_CLASS='1' AND CLU.REC_KEY=CL.USER_KEY AND CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD'))"));
		pUserDM->RefreshDataManager(sUpdateSql);
	}	
	
	sUpdateSql.Format(
		_T("UPDATE CO_LOAN_USER_TBL CLU ")
		_T("SET CLU.USER_CLASS='0',CLU.LOAN_STOP_DATE=NULL,CLU.LAST_WORK='%s' ")
 		_T("WHERE CLU.USER_CLASS='1' AND (CLU.LOAN_STOP_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') OR CLU.LOAN_STOP_DATE IS NULL) AND CLU.REC_KEY NOT IN (SELECT DISTINCT CL.USER_KEY FROM LS_WORK_T01 CL WHERE CLU.USER_CLASS='1' AND CLU.REC_KEY=CL.USER_KEY AND CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD'));")
		, m_pParentMgr->GetWorkLogMsg(_T("���������ϰ�����"),__WFILE__,__LINE__));
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	pDM->InitDBFieldData();
	pDM->StartFrame();
	pDM->AddFrame(sUpdateSql);
	ids = pDM->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	ids = pDM->EndFrame();
	
	INT i;
	INT nRowCount;
	CString	strUserKey;
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{
		nRowCount = pUserDM->GetRowCount();	
		for(i = 0; i < nRowCount; i++)
		{
			pUserDM->GetCellData(i, 0, strUserKey);
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(strUserKey));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("USER_CLASS,LOAN_STOP_DATE")));
			ShareInfoCenterUpload(_T("N"),_T("U"));
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeUserStopClass()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeUserStopClass");

	ids = SetResultScreen(10, _T("���� ���� ȸ�� �ϰ� ���� ����"));
	if(0 > ids) return ids;

	ids = m_pCheckLoan->GetAutoChangeUserClass();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(ids > 0) 
	{
		ids = SetResultScreen(30, _T("������ ���� ���� ȸ�� �ڵ� �ϰ� �ο� �������� �����������Ƿ� �۾��� �������� ����"));
		if(0 > ids) return ids;

		return 1;
	}

	ids = SetResultScreen(40, _T("DB�۾���"));
	if(0 > ids) return ids;

	ids = AllChangeUserStopClassDBProc();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::AllChangeUserStopClassDBProc()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeUserStopClassDBProc");

	CESL_DataMgr *pUserDM;
	CString	sUpdateSql;
	CString	strUserUpdateKeyList[2048];
	CString	strManageValue;

	strManageValue = m_pManageValue->m_strLoanShare;
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{
		pUserDM = m_pParentMgr->FindDM(_T("DM_SHARE_LOAN_STOP_USER_LIST"));	
		if(pUserDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		sUpdateSql.Format(	
			_T("CLU.USER_CLASS='0' AND CLU.REC_KEY IN ")
			_T("(SELECT DISTINCT CL.USER_KEY FROM LS_WORK_T01 CL ")
			_T("WHERE CLU.USER_CLASS='0' AND CLU.REC_KEY=CL.USER_KEY AND CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < TO_DATE(SYSDATE,'YYYY/MM/DD'))"));

		pUserDM->RefreshDataManager(sUpdateSql);
	}	
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AllChangeUserStopClassDBProc"));
	
	sUpdateSql.Format( _T("UPDATE CO_LOAN_USER_TBL CLU SET USER_CLASS='1', LAST_WORK='%s' ")
						_T("WHERE REC_KEY IN ")
						_T("(SELECT DISTINCT USER_KEY FROM LS_WORK_T01 CL ")
						_T("WHERE CLU.USER_CLASS='0' AND CLU.REC_KEY IN ")
						_T("(SELECT DISTINCT CL.USER_KEY FROM LS_WORK_T01 CL ")
						_T("WHERE CLU.USER_CLASS='0' AND CLU.REC_KEY=CL.USER_KEY AND CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < TO_DATE(SYSDATE, 'YYYY/MM/DD'));")
						, m_pParentMgr->GetWorkLogMsg(_T("���������ϰ�����"), __WFILE__, __LINE__));
	
	pDM->InitDBFieldData();
	pDM->StartFrame();
	pDM->AddFrame(sUpdateSql);

	ids = pDM->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	ids = pDM->EndFrame();	
	
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{
		CString	strUserKey;	
		INT		nRowCount;
		INT		i;

		nRowCount = pUserDM->GetRowCount();
		for (i=0 ; i<nRowCount ; i++)
		{				
			pUserDM->GetCellData(_T("����Ű"), i, strUserKey);
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(strUserKey));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("USER_CLASS")));		
			ShareInfoCenterUpload(_T("N"),_T("U"));
		}
	}	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeReserveCancel()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeReserveCancel");

	ids = SetResultScreen(60, _T("���� �������� ���� ���� �ڷ� ���� ��� �۾� ����"));
	if(0 > ids) return ids;

	ids = m_pCheckLoan->GetAutoChangeReserveCancel();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(ids > 0) 
	{
		ids = SetResultScreen(80, _T("������ ���� �ڵ� ��� �������� �����������Ƿ� �۾��� �������� ����"));
		if(0 > ids) return ids;

		return 1;
	}

	ids = SetResultScreen(90, _T("DB�۾���"));
	if(0 > ids) return ids;

	ids = AllChangeReserveCancelDBProc();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeReserveCancelDBProc()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeReserveCancelDBProc");
	CESL_DataMgr *pLoanDM;
	CString strManageValue;
	CString sUpdateSql;
		
	strManageValue = m_pManageValue->m_strLoanShare;
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{	
		pLoanDM = m_pParentMgr->FindDM(_T("DM_SHARE_LOAN_INFO_LIST"));	
		if(pLoanDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(m_pManageValue->m_sUnmannedReserveSys == _T("Y"))
		{
			sUpdateSql.Format(_T("RESERVATION_EXPIRE_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') AND STATUS='3' AND UNMANNED_RESERVATION_LOAN IS NULL AND MANAGE_CODE=UDF_MANAGE_CODE"));
		}
		else
		{
			sUpdateSql.Format(_T("RESERVATION_EXPIRE_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') AND STATUS='3' AND MANAGE_CODE=UDF_MANAGE_CODE"));
		}
		
		pLoanDM->RefreshDataManager(sUpdateSql);
	}	
	if(m_pManageValue->m_sUnmannedReserveSys == _T("Y")) 
	{
		sUpdateSql.Format( 
			_T("UPDATE LS_WORK_T01 ")
			_T("SET STATUS='4',RESERVE_CANCEL_DATE=SYSDATE,LAST_WORK='%s',RESERVATION_CANCEL_TYPE='0',R_DEVICE='KOLASIII' ")
			_T("WHERE RESERVATION_EXPIRE_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') AND STATUS='3' AND UNMANNED_RESERVATION_LOAN IS NULL AND MANAGE_CODE=UDF_MANAGE_CODE;")
			, m_pParentMgr->GetWorkLogMsg(_T("�����ڵ����"),__FILE__,__LINE__));
	}
	else 
	{
		sUpdateSql.Format( 
			_T("UPDATE LS_WORK_T01 ")
			_T("SET STATUS='4',RESERVE_CANCEL_DATE=SYSDATE,LAST_WORK='%s',RESERVATION_CANCEL_TYPE='0',R_DEVICE='KOLASIII' ")
			_T("WHERE RESERVATION_EXPIRE_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') AND STATUS='3' AND MANAGE_CODE=UDF_MANAGE_CODE;")
			, m_pParentMgr->GetWorkLogMsg(_T("�����ڵ����"),__FILE__,__LINE__));
	}
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	pDM->StartFrame();
	pDM->AddFrame(sUpdateSql);	
	sUpdateSql.Empty();
	
	sUpdateSql.Format(	
		_T("UPDATE CO_LOAN_USER_TBL CLU1 ")
		_T("SET RESERVATION_COUNT=(SELECT COUNT(1) FROM LS_WORK_T01 LW WHERE CLU1.REC_KEY=LW.USER_KEY AND LW.STATUS='3') ")
		_T("WHERE REC_KEY IN (SELECT UU.REC_KEY FROM CO_LOAN_USER_TBL UU,(SELECT USER_KEY AS PK,COUNT(1) AS CNT FROM LS_WORK_T01 LW,CO_LOAN_USER_TBL CLU WHERE LW.USER_KEY=CLU.REC_KEY AND STATUS='3' GROUP BY USER_KEY) CNT_TBL WHERE UU.REC_KEY=CNT_TBL.PK AND CNT <> UU.RESERVATION_COUNT);"));
	pDM->AddFrame(sUpdateSql);
	ids = pDM->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = pDM->EndFrame();
	
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{
		INT i;
		INT nRowCount;
		CString	strLoanKey;
		nRowCount = pLoanDM->GetRowCount();
		for(i = 0; i < nRowCount; i++)
		{
			pLoanDM->GetCellData(_T("�̷�Ű"), i, strLoanKey);
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(strLoanKey));
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("STATUS,RESERVE_CANCEL_DATE")));			
			ShareInfoCenterUpload(_T("U"),_T("N"));
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeReserveUpdate()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeReserveUpdate");

	ids = SetResultScreen(60, _T("���� �������� ���� ���� ���� �ڷ� ���� ������ �ο� �۾� ����"));
	if(0 > ids) return ids;

	ids = m_pCheckLoan->GetAutoChangeReserveCancel();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(ids > 0) 
	{
		ids = SetResultScreen(80, _T("������ ���� �ڵ� ��� �������� �����������Ƿ� �۾��� �������� ����"));
		if(0 > ids) return ids;

		return 1;
	}

	ids = SetResultScreen(90, _T("DB�۾���"));
	if(0 > ids) return ids;

	ids = AllChangeReserveUpdateDBProc();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeReserveUpdateDBProc()
{
EFS_BEGIN	

	if(!(m_pManageValue->m_bVolReserve)) return 0;

	INT ids;	
	CString strFuncName = _T("AllChangeReserveUpdateDBProc");

	CString strMonoUpdateSql;
	CString strMonoUpdateSql2;
	CString strSerialUpdateSql;
	CString strSerialUpdateSql2;	
	
	strMonoUpdateSql.Format(
		_T("UPDATE LS_WORK_T01 A SET A.RESERVATION_EXPIRE_DATE = ")
		
		_T("TO_DATE(TO_CHAR(TO_DATE(ECO$F_G_ISHOLIDAY$I")
		
		_T("(")
		_T("  SYSDATE + (SELECT MIN(RESERVATION_EXPIRE_DAY) FROM CD_CODE_TBL ")
		_T("  WHERE CLASS=31 AND CODE = (SELECT B.USER_CLASS_CODE FROM CO_LOAN_USER_TBL B ")
		_T("                             WHERE B.REC_KEY=A.USER_KEY)), '%s' ")
		
		_T("), 'YYYY/MM/DD')||' 23:59:59'), 'YYYY/MM/DD HH24:MI:SS') ")
		
		_T("WHERE A.REC_KEY IN (SELECT MIN(CL.REC_KEY) FROM LS_WORK_T01 CL ")
		_T("                     WHERE CL.STATUS='3' AND CL.RESERVATION_EXPIRE_DATE is NULL AND ")
		_T("                           (SELECT BB.WORKING_STATUS FROM BO_BOOK_TBL BB ")
		_T("                             WHERE BB.REC_KEY=CL.BOOK_KEY AND BB.MANAGE_CODE = '%s')='BOL112N' ") 

		_T("                     GROUP BY CL.SPECIES_KEY, CL.VOL_INFO, CL.REG_NO ")
		_T("                  ) ")
		_T("AND 0 = (SELECT COUNT(1) FROM LS_WORK_T01 CL ")
		_T("          WHERE CL.STATUS='3' AND CL.RESERVATION_EXPIRE_DATE is NOT NULL ")
		_T("                AND A.SPECIES_KEY = CL.SPECIES_KEY AND A.VOL_INFO = CL.VOL_INFO ")
		_T("                AND (SELECT BB.WORKING_STATUS FROM BO_BOOK_TBL BB ")
		_T("                  WHERE BB.REC_KEY=CL.BOOK_KEY AND BB.MANAGE_CODE = '%s')='BOL112N' ")

		_T("                  GROUP BY CL.SPECIES_KEY, CL.VOL_INFO, CL.REG_NO ")
		_T("       ) ")
		_T("AND (SELECT B.USER_CLASS_CODE FROM CO_LOAN_USER_TBL B WHERE B.REC_KEY=A.USER_KEY) is NOT NULL; "),
		m_pParentMgr->m_pInfo->MANAGE_CODE, m_pParentMgr->m_pInfo->MANAGE_CODE, m_pParentMgr->m_pInfo->MANAGE_CODE);	
	
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;	

		if(m_pManageValue->m_bVolReserve) {
		strMonoUpdateSql2.Format(
			_T("UPDATE LS_WORK_T01 A SET A.REG_NO=")
			_T("(SELECT MIN(B.REG_NO) FROM BO_BOOK_TBL B ")
			_T("WHERE B.WORKING_STATUS='BOL112N' AND B.MANAGE_CODE='%s' ")
			_T("AND B.USE_LIMIT_CODE NOT IN ('CL','CD','CA','IZ') AND B.SEPARATE_SHELF_CODE NOT IN ")
			_T("(SELECT CODE FROM CD_CODE_TBL WHERE CLASS=11 AND LOAN_YN='N') ")
			_T("AND SUBSTR(B.REG_NO,1,%d) IN ")
			_T("(SELECT CODE FROM CD_CODE_TBL WHERE CLASS=4 AND LOAN_YN='Y') ")
			_T("AND B.SHELF_LOC_CODE IN ")
			_T("(SELECT CODE FROM CD_CODE_TBL WHERE CLASS=19 AND LOAN_YN='Y') ")
			_T("AND B.SPECIES_KEY=A.SPECIES_KEY AND NVL(B.VOL_SORT_NO,-100)=NVL(A.VOL_INFO,-100)) ")
			_T("WHERE A.STATUS='3' AND A.RESERVATION_EXPIRE_DATE IS NOT NULL AND REG_NO IS NULL;") 
			, nRegCodeLength, m_pParentMgr->m_pInfo->MANAGE_CODE);
	}
	
	strSerialUpdateSql.Format(
		_T("UPDATE LS_WORK_T01 A ")
		_T("SET A.RESERVATION_EXPIRE_DATE = ")
		_T("TO_DATE(TO_CHAR(TO_DATE(ECO$F_G_ISHOLIDAY$I ")
		_T("(SYSDATE + ")
		_T("(SELECT MIN(RESERVATION_EXPIRE_DAY) FROM CD_CODE_TBL ")
		_T("WHERE CLASS=31 AND CODE = (SELECT B.USER_CLASS_CODE FROM CO_LOAN_USER_TBL B WHERE B.REC_KEY=A.USER_KEY) ")
		_T("), '%s'), 'YYYY/MM/DD')||' 23:59:59'), 'YYYY/MM/DD HH24:MI:SS') ")
		_T("WHERE A.REC_KEY IN (SELECT MIN(CL.REC_KEY) FROM LS_WORK_T01 CL ")
		_T("            WHERE CL.STATUS='3' ")
		_T("                  AND CL.RESERVATION_EXPIRE_DATE IS NULL ")
		_T("                  AND (SELECT BB.WORKING_STATUS FROM SE_BOOK_TBL BB WHERE BB.REC_KEY=CL.BOOK_KEY AND BB.MANAGE_CODE='%s') IN ('SEL212N','SEL112N') ")
		_T("            GROUP BY CL.SPECIES_KEY, CL.VOL_INFO, CL.REG_NO) ")
		_T("AND      0 = (SELECT COUNT(1) FROM LS_WORK_T01 CL ")
		_T("            WHERE CL.STATUS='3' ")
		_T("                  AND CL.RESERVATION_EXPIRE_DATE IS NOT NULL ")
		_T("                AND A.SPECIES_KEY = CL.SPECIES_KEY AND A.VOL_INFO = CL.VOL_INFO ")
		_T("                  AND (SELECT BB.WORKING_STATUS FROM SE_BOOK_TBL BB WHERE BB.REC_KEY=CL.BOOK_KEY AND BB.MANAGE_CODE='%s') IN ('SEL212N','SEL112N') ")
		_T("            GROUP BY CL.SPECIES_KEY, CL.VOL_INFO, CL.REG_NO) ")
		_T("AND (SELECT B.USER_CLASS_CODE FROM CO_LOAN_USER_TBL B WHERE B.REC_KEY=A.USER_KEY) is NOT NULL;"),
		m_pParentMgr->m_pInfo->MANAGE_CODE, m_pParentMgr->m_pInfo->MANAGE_CODE, m_pParentMgr->m_pInfo->MANAGE_CODE);

	if(m_pManageValue->m_bVolReserve)
	{
		strSerialUpdateSql2.Format(
			_T("UPDATE LS_WORK_T01 A SET ")
			_T("A.REG_NO = (SELECT MIN(B.REG_NO) ")
			_T("FROM SE_BOOK_TBL B, SE_VOL_TBL C ")
			_T("WHERE B.VOL_KEY=C.REC_KEY AND B.MANAGE_CODE = '%s' AND ")

			_T("B.WORKING_STATUS in ('SEL212N','SEL112N') AND ")
			_T("B.USE_LIMIT_CODE NOT IN ('CL','CD','CA','IZ') AND ")
			_T("B.SEPARATE_SHELF_CODE NOT IN (SELECT CODE FROM CD_CODE_TBL WHERE CLASS = 11 AND LOAN_YN='N') AND ")
			_T("SUBSTR(B.REG_NO,1,%d) IN (SELECT CODE FROM CD_CODE_TBL WHERE CLASS = 4 AND LOAN_YN='Y') AND ")
			_T("B.SHELF_LOC_CODE IN (SELECT CODE FROM CD_CODE_TBL WHERE CLASS = 19 AND LOAN_YN='Y') AND ")
			_T("B.SPECIES_KEY=A.SPECIES_KEY AND ")
			_T("NVL(C.VOL_SORT_NO,-100)=NVL(A.VOL_INFO,-100)) ")
			_T("WHERE A.STATUS='3' AND A.RESERVATION_EXPIRE_DATE IS NOT NULL AND REG_NO IS NULL;")
			, nRegCodeLength, m_pParentMgr->m_pInfo->MANAGE_CODE);
	}
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM("DM_BO_LOC_3100_DB");
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();
	pDM->StartFrame();
	pDM->AddFrame(strMonoUpdateSql);
	if(m_pManageValue->m_bVolReserve) pDM->AddFrame(strMonoUpdateSql2);
	pDM->AddFrame(strSerialUpdateSql);
	if(m_pManageValue->m_bVolReserve) pDM->AddFrame(strSerialUpdateSql2);
	ids = pDM->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	ids = pDM->EndFrame();
	
	return 0;

EFS_END
return -1;
}


BOOL CBL_LOC_LOAN_PROC::GetThreadState()
{
EFS_BEGIN

	return m_bStop;

EFS_END
return FALSE;
}


INT CBL_LOC_LOAN_PROC::SetStopThread()
{
EFS_BEGIN

	m_bBreak = TRUE;
	return 0;	

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetThreadEndState()
{
EFS_BEGIN

	return m_bStopState;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ChangeMember()
{
EFS_BEGIN

	if(m_sMemberKey.IsEmpty()) return 0;

	INT ids;
	
	ids = SearchLoanInfo(m_sMemberKey, _T(""));
	if(0 > ids)	return ids;
	
	m_sMemberKey.Empty();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SetChangeMemberKey(CString sMemberKey)
{
EFS_BEGIN

	m_sMemberKey = sMemberKey;
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewDetail()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ViewDetail");

	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM(sDMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(pParentDM->GetRowCount() < 1) return 0;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sPublishForm;

	ids = pParentDM->GetCellData(_T("����/���ӱ���"), pGrid->GetIdx(), sPublishForm);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, strFuncName);

	if(sPublishForm.Compare(_T("MO")) != 0 && (sPublishForm.Compare(_T("SE")) != 0)) return 0;

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT, INT > asGridRow;

	INT nIndex = -1;
	nIndex = pGrid->GetIdx();
	pParentDM->GetCellData(_T("��KEY"), nIndex, sSpeciesKey);
	sSpeciesKeyList.AddTail(sSpeciesKey);
	asGridRow.Add(nIndex);

	if(sPublishForm.Compare(_T("MO")) == 0)
	{
		CBOBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList(sSpeciesKeyList, pGrid, asGridRow);
		Dlg.DoModal();
	}
	else
	{
		CSEBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList(sSpeciesKeyList, pGrid, asGridRow);
		Dlg.DoModal();
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::ReLoadMemberInfo()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReLoadMemberInfo");

	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pUserDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sLoanUserKey;
	ids = pUserDM->GetCellData(_T("������KEY"), 0, sLoanUserKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CString sWhere;
	sWhere.Format(_T("CLU.REC_KEY = %s "), sLoanUserKey);
	ids = SearchMemberInfo(sWhere);
	if(0 > ids) return ids;
	
	ids = AddMemberInfo();
	if(0 > ids) return ids;
	
	ids = AllControlDisplayNotGrid();
	if(0 > ids) return ids;

	ids = ViewSimpleFamilyMember();
	if(0 > ids) return ids;
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SelfCloseMessageBox(CString sMsg, CESL_Mgr *pParentMgr/*=NULL*/, INT nDelayTime/*=1000*/)
{
EFS_BEGIN		

	if(FALSE == m_bShowMessage)
	{
		return 0;
	}
	if(NULL == pParentMgr)
	{
		if(NULL == m_pParentMgr)
		{
			return 0;
		}
		pParentMgr = m_pParentMgr;
	}
	INT nMode=0;
	if(sMsg.Find(_T("\n")) >= 0)
	{
		nMode = 1;
	}
	else if(sMsg.GetLength() > 30)
	{
		nMode = 1;
	}
	else
	{
		nMode = 0;
	}
	if(m_nMessageLoop == 3) 
	{
		m_nMessageLoop = 0;
	}
	if(m_pMessageDlg[m_nMessageLoop] == NULL)
	{
		m_pMessageDlg[m_nMessageLoop] = new CMessageDlg(pParentMgr);		
	}
	for(int i = 0; i < 3; i++)
	{
		if(m_pMessageDlg[i] != NULL)
		{
			if(m_pMessageDlg[i]->m_sMsg == sMsg)
			{
				m_nMessageLoop = i;
				break;
			}
		}
	}
	if(m_pMessageDlg[m_nMessageLoop] != NULL)
	{
		m_pMessageDlg[m_nMessageLoop]->DestroyWindow();
		delete m_pMessageDlg[m_nMessageLoop];
		m_pMessageDlg[m_nMessageLoop] = NULL;
		m_pMessageDlg[m_nMessageLoop] = new CMessageDlg(pParentMgr);
	}
	m_pMessageDlg[m_nMessageLoop]->m_nDelayTime = nDelayTime;
	m_pMessageDlg[m_nMessageLoop]->Create(AfxGetMainWnd());
	m_pMessageDlg[m_nMessageLoop]->SetMsg(sMsg, nMode);
	m_pMessageDlg[m_nMessageLoop]->CenterWindow(pParentMgr);	
	
	CRect WndPos;
	m_pMessageDlg[m_nMessageLoop]->GetWindowRect(WndPos);
	m_pMessageDlg[m_nMessageLoop]->MoveWindow(WndPos.left, WndPos.top + (WndPos.Height()*m_nMessageLoop), WndPos.Width(), WndPos.Height());
	InputMessageBox(sMsg);
	m_pMessageDlg[m_nMessageLoop]->ShowWindow(SW_SHOWNOACTIVATE);
	m_nMessageLoop++;

	if(m_bBeep == TRUE) 
	{
		if(sMsg == _T("����Ǿ����ϴ�."))
		{
			AfxBeginThread(RunLoanBeep, this);			
		}
		else if(sMsg == _T("�ݳ��� �Ϸ�Ǿ����ϴ�."))
		{
			AfxBeginThread(RunReturnBeep, this);						
		}
		else if(sMsg == _T("�����ڸ� ���� �������ֽʽÿ�") ||
			      sMsg == _T("�˻��� �̿��ڴ� ��������ȸ���Դϴ�.") ||
				  sMsg == _T("ȸ�� ������ ��ȸ�ϴµ� �����߽��ϴ�.") ||
				  sMsg == _T("�������� �ʴ� �̿����Դϴ�.") ||
				  sMsg == _T("�нǵ� ȸ��ī�� �Դϴ�.") ||
				  sMsg == _T("�˻��� �̿��ڴ� Ż���ûȸ���Դϴ�.") ||
				  sMsg == _T("���� ȸ������ ������ �����մϴ�.") ||
				  sMsg == _T("ȸ�����н� ȸ���� ������ �Ұ��� �մϴ�.") ||
				  sMsg == _T("���� �������� ������ �ʾ����Ƿ� ������ �Ұ����մϴ�.") ||
				  sMsg == _T("��ȸ���� ������ ���ѵ˴ϴ�.") ||
				  sMsg == _T("Ÿ ������ �������� ȸ���Դϴ�.") ||
				  sMsg == _T("Ÿ ������ ����ȸ���Դϴ�.") ||
				  sMsg == _T("�����ڸ� ���� �������ֽʽÿ�") ||
				  sMsg == _T("���� �ѵ��� �ʰ��Ͽ� ������ �� �����ϴ�.") ||
				  sMsg == _T("���մ��Ⱑ�ɱǼ� �ѵ��� �ʰ��Ͽ� ������ �� �����ϴ�.") ||
				  sMsg == _T("������ ���Ⱑ�ɱǼ� �ѵ��� �ʰ��Ͽ� ������ �� �����ϴ�.") ||
				  sMsg == _T("������ å�� ������ �ֽʽÿ�") )
		{
			AfxBeginThread(RunErrorBeep, this);
		}
		
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetDelayDay(INT nIndex, INT &nDelayDay)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetDelayDay");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sDelayDay;
	CString sReturnExpDate;

	ids = pDM->GetCellData(_T("�ݳ�������"), nIndex, sReturnExpDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if(sReturnExpDate.GetLength() < 9) return 0;

	
	
	CString sReturnType;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("�ݳ�����"), sReturnType);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	CString sReturnDate;
	CString sReturnDateControlAlias = _T("�ݳ���");
	if(sReturnType.Compare(_T("2"))== 0)
		sReturnDateControlAlias = _T("�ұ޹ݳ���");

	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, sReturnDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CTime tCurrent = CTime(_ttoi(sReturnDate.Mid(0,4)), _ttoi(sReturnDate.Mid(5,2)), _ttoi(sReturnDate.Mid(8,2)), 0, 0, 0);
	
	CTime tReturnExpDate(_ttoi(sReturnExpDate.Mid(0,4)), _ttoi(sReturnExpDate.Mid(5,2)), _ttoi(sReturnExpDate.Mid(8,2)), 0, 0, 0);

	CTimeSpan tDelayDay = tCurrent - tReturnExpDate;

	nDelayDay =  tDelayDay.GetDays();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetReservationExpireDay(INT &nExpireDay)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetReservationExpireDay");
	
	CString sPositionCode;

	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����������"), sPositionCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	ids = m_pCheckLoan->GetReservationExpireDay(sPositionCode, nExpireDay);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	ids = m_pCheckLoan->GetAutoSetReserveExpireDay();
	if(ids ==  0) return 0;

	CInputReservatoinExpireDay InputDlg(m_pParentMgr);
	InputDlg.SetDay(nExpireDay);
	
	InputDlg.DoModal();

	InputDlg.GetDay(nExpireDay);

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::MakeReservationExpireDate(INT nExpireDay, CString &sExpireDate)
{
EFS_BEGIN

	
	CString strGetData;
	CTime tExpireDate = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strGetData);

	CTimeSpan tExpireDay(nExpireDay, 0, 0, 0);
	
	tExpireDate += tExpireDay;

	sExpireDate = tExpireDate.Format(_T("%Y/%m/%d"));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchUserBook(INT nEntryPoint, CString &sRegNo, CString &sPublishForm)
{
EFS_BEGIN

	INT ids;

	CString strFuncName = _T("SearchUserBook");
	DWORD nSTART, nFINISH;
	nSTART = GetTickCount();

	CString sBoWhere,sSeWhere;
	ids = GetSqlSearchBookInfo(sBoWhere, 0);	
	if(0 != ids) return ids;
	
	ids = GetSqlSearchBookInfo(sSeWhere, 1);	
	if(0 != ids) return ids;
	
	ids = SendSearchBookInfoSQL(sBoWhere, sSeWhere);
	if(0 != ids) return ids;
	
	nFINISH = GetTickCount();
	TRACE(_T("(Finish) - �̿��� �ڷ�ã�� �� �޸� �����ϴ� �ð� : [%d.%d sec]\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	
	ids = ViewSerachBookList(nEntryPoint,sRegNo,sPublishForm);
	if(0 != ids) return ids;
	
	return ids;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::MoveRegNoToNextProcBasket(CString sMode,BOOL bIsRFID)
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("MoveRegNoToNextProcBasket");

	CString sRegNo;
	CString sBasketDMAlias = _T("DM_BO_LOC_3100_BASKET");
	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	INT nInsertIndex;

	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_SEARCH_INFO"));
	if(!pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_Control_Element* pCEB = pCM->FindControlElement(_T("ó�����ڷ�"));
	if(!pCEB) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CButton* pExpDataButton = (CButton*)(m_pParentMgr->GetDlgItem(pCEB->CTRL_ID));
	if(!pExpDataButton) return 0;

	if((pExpDataButton->GetCheck() != 1) && !bIsRFID) return 0;
	
	if(_T("L") == sMode)
	{
		
		MakeRegNoSpaceToZero(FALSE);
	}
	else
	{		
	MakeRegNoSpaceToZero(TRUE);
	}
	
	ids = IsRegNo(sRegNo);
	if(0 > ids) return ids;
	
	if(sRegNo.IsEmpty()) return 0;
	
	CESL_DataMgr *pBasketDM = m_pParentMgr->FindDM(sBasketDMAlias);
	if(NULL == pBasketDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	pBasketDM->InsertRow(-1);

	nInsertIndex = pBasketDM->GetRowCount()-1;

	ids = pBasketDM->SetCellData(_T("��Ϲ�ȣ"), sRegNo, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	ids = pBasketDM->SetCellData(_T("����ó��"), sMode, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	CESL_Control_Element* pCE = pCM->FindControlElement(_T("�ٱ���"));
	if(NULL == pCE) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	CComboBox* pCombo = NULL;
	CESL_Mgr* pParentMgr;
	pParentMgr = m_pParentMgr;
	while(pParentMgr != NULL)
	{
		pCombo = (CComboBox*)(pParentMgr->GetDlgItem(pCE->CTRL_ID));
		pParentMgr = pParentMgr->pParentMgr;
	}
	
	if(NULL == pCombo) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sTmpRegNo;
	BOOL bIsInsert = TRUE;
	for(INT i = 0 ; i < pCombo->GetCount() ; i++)
	{
		pCombo->GetLBText(i, sTmpRegNo);
		if(sTmpRegNo.Compare(sRegNo) == 0) bIsInsert = FALSE;
	}

	if(bIsInsert)
		pCombo->AddString(sRegNo);

	pCombo->SetCurSel(pCombo->GetCount()-1);

	m_pParentMgr->SetControlData(sCMAlias, _T("�����ڹ�ȣ"), _T(""));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::BasketRegNoProc()
{
EFS_BEGIN

	INT ids, i = 0, j = 0;
	CString strFuncName = _T("BasketRegNoProc");

	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sBasketDMAlias = _T("DM_BO_LOC_3100_BASKET");

	
	CESL_DataMgr *pBasketDM = m_pParentMgr->FindDM(sBasketDMAlias);
	if(NULL == pBasketDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	
	CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
	CESL_ControlMgr *pLoanCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pLoanCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)(pLoanCM->FindControl(_T("MainGrid")));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);

	CString strBookKey;
	CString sRegNo, sMode;	
	CString sSerialNo;
	CString strBookLibCode;
	for(i = 0; i < pBasketDM->GetRowCount(); i++)
	{
		ids = pBasketDM->GetCellData(_T("åŰ"), i, strBookKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pBasketDM->GetCellData(_T("��Ϲ�ȣ"), i, sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pBasketDM->GetCellData(_T("����ó��"), i, sMode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pCM->SetControlMgrData(_T("�����ڹ�ȣ"), sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pBasketDM->GetCellData(_T("�������������ȣ"), i, strBookLibCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		
		if(EXP_RESERVELOAN == sMode)
		{
			
			ids = BarCodeReturnBookCheck(strBookKey);

			if(0 > ids) return ids;
			if(2 == ids)
			{
				sMode = EXP_LOAN;
			}
		}
		
		if(EXP_RESERVELOAN == sMode)
		{
			InitExpDM();
			ids = SPFExcutePath(_T("�������"));
		}
		
		else if(EXP_LOAN == sMode)
		{
			InitExpDM();
			ids = SPFExcutePath(_T("����"));
		}
		
		else if(EXP_RESERVE == sMode)
		{
			InitExpDM();
			SPFExcutePath(_T("����"));
			continue;
		}
	
		else if(_T("L") == sMode)
		{
			
			InitExpDM();

			CString strSQL;
			CString strTransNo, strBookManageCode, strRegNo, strSpeciesKey;
			BOOL bIsAppendixLoan = FALSE;

			ids = pBasketDM->GetCellData(_T("å�η�_Ʈ����ǹ�ȣ"), i, strTransNo);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

			CESL_DataMgr* pILLDM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_ILL_BOOK_INFO"));
			strSQL.Format(	_T("SELECT ILL.TRANS_NO, (SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ILL.GIVE_LIBCODE) AS GIVE_MANAGE_CODE, ILL.REG_NO, ILL.BOOK_APPENDIX_FLAG, ILL.SPECIES_KEY ")
							_T("FROM ILL_TRANS_TBL ILL ")
							_T("WHERE ILL.STATE = '0014' AND ILL.LOAN_KEY IS NULL AND ILL.SPECIES_KEY = (SELECT SPECIES_KEY FROM ILL_TRANS_TBL WHERE TRANS_NO = '%s') ORDER BY LENGTH(TRANS_NO), TRANS_NO")
							, strTransNo);
			TmpDM.RestructDataManager(strSQL);

			for(j = 0; j < TmpDM.GetRowCount(); j++){
				if(strTransNo == TmpDM.GetCellData(j, 0)){
					strSpeciesKey = TmpDM.GetCellData(j, 4);
				}
				if(_T("Y") == m_pManageValue->m_sIsLoanAppendix){
					if(strSpeciesKey == TmpDM.GetCellData(j, 4) && TmpDM.GetCellData(j, 3) == _T("A")){
						bIsAppendixLoan = m_pParentMgr->MessageBox(_T("�����Ϸ��� å�� ���� �Լ��� �η��ڷᰡ �����մϴ�.\r\n���� ������ �Ͻðڽ��ϱ�?"), _T("�η��ڷ����"), MB_ICONQUESTION | MB_YESNO);
						break;
					}
				}
			}
			for(j = 0; j < TmpDM.GetRowCount(); j++){
				if(strSpeciesKey == TmpDM.GetCellData(j, 4) && strSpeciesKey.IsEmpty() == FALSE){
					
					if(bIsAppendixLoan == FALSE && TmpDM.GetCellData(j, 3) == _T("A")){
						break;
					}
					pILLDM->FreeData();
					pILLDM->InsertRow(-1);
					pILLDM->SetCellData(_T("Ʈ����ǹ�ȣ"), TmpDM.GetCellData(j, 0), 0);
					pILLDM->SetCellData(_T("��������_å"), TmpDM.GetCellData(j, 1), 0);
					pILLDM->SetCellData(_T("��Ϲ�ȣ"), TmpDM.GetCellData(j, 2), 0);
					
					m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), TmpDM.GetCellData(j, 2));

					SPFExcutePath(_T("å�η�����"));
				}
			}

		}

		if(0 < ids || 0 > ids) 
		{
			continue;
		}
		
		ids = pBasketDM->GetCellData(_T("�ø���"), i, sSerialNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
		if(sSerialNo.IsEmpty())
		{
			continue;
		}
		
		ids = RFID_UpdateLoan(0, sSerialNo);			
		if(0 > ids) {
			CString sEMsg;
			sEMsg.Format(_T("��Ϲ�ȣ['%s'] : RFID ���� �Է¿� �����Ͽ����ϴ�."), sRegNo);
			SelfCloseMessageBox(sEMsg);
		}	
		
		CString sLoanType;
		ids = GetLoanType(sLoanType);
		if(0 > ids) return ids;
	}
	pBasketDM->FreeData();

	ids = ClearBasket();
	if(0 > ids) return ids;

	m_pParentMgr->SetControlData(sCMAlias, _T("�����ڹ�ȣ"), _T(""));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ClearBasket()
{
EFS_BEGIN

	CString strFuncName = _T("ClearBasket");

	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_Control_Element* pCE = pCM->FindControlElement(_T("�ٱ���"));
	if(pCE == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
	CComboBox* pCombo = NULL;
	CESL_Mgr* pParentMgr;
	pParentMgr = m_pParentMgr;
	while(pParentMgr != NULL)
	{
		pCombo = (CComboBox*)(pParentMgr->GetDlgItem(pCE->CTRL_ID));
		pParentMgr = pParentMgr->pParentMgr;
	}
	
	if(pCombo == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT nCnt = pCombo->GetCount();
	for(INT i = nCnt-1 ; i > -1 ; i--)
		pCombo->DeleteString(i);

	pCombo->Clear();

	pCombo->SetCurSel(0);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewLoanHistory(INT nMode, INT nEntryPoint)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ViewLoanHistory");

	CString sUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sExpLoanDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	
	CESL_DataMgr *pExpLoanDM = m_pParentMgr->FindDM(sExpLoanDMAlias);
	if(NULL == pExpLoanDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd,  -1000, strFuncName);
	
	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM(sUserDMAlias);
	if(NULL == pUserDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd,  -1000, strFuncName);

	CString sBookRecKey;
	CString sUserRecKey;
	CString sFamilyKey;

	ids = pExpLoanDM->GetCellData(_T("åKEY"), 0, sBookRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 
	
	ids = pUserDM->GetCellData(_T("������KEY"), 0, sUserRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 

	ids = pUserDM->GetCellData(_T("����KEY"), 0, sFamilyKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 

	CLoanHistory Dlg(m_pParentMgr);
	Dlg.SetInitData(nMode, nEntryPoint, sUserRecKey, sFamilyKey, sBookRecKey , m_pManageValue->m_sIsUnityLoanService );
	Dlg.DoModal();

	return Dlg.GetCloseMode();

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::TestCode()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("TestCode");

	CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pLoanInfoDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(sMemberDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_MEMBER_INFO"));	
	
	if(pDM->GetRowCount() == 0) return 0;

	INT nLoanCnt = 0; 
	INT nUnityLoanCnt = 0; 
	INT nOtherLoanCnt = 0; 
	INT nReserveCnt = 0;

	INT nDMCnt = pLoanInfoDM->GetRowCount();

	CString strUserManageCode = _T("");
	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("��������"), strUserManageCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sStatus,sAppendixYN;
	CString sManageCode = _T("");
	for(INT i = 0 ; i < pLoanInfoDM->GetRowCount() ; i++)
	{
		ids = pLoanInfoDM->GetCellData(_T("�������"), i, sStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanInfoDM->GetCellData(_T("�ηϿ���"), i, sAppendixYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanInfoDM->GetCellData(_T("��������"), i, sManageCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(sStatus.Compare(_T("0")) == 0 || sStatus.Compare(_T("2")) == 0)
		{
			if(sAppendixYN.Compare(_T("A")) == 0)
			{
				if(m_pCheckLoan->IsAppendixIncludeLoanCnt() == 0)
				{
					nUnityLoanCnt++;
				}
			}
			else
				nUnityLoanCnt++;	
		}
		
		if(sStatus.Compare(_T("3")) == 0)
		{
			CString sValue;
			ids = pLoanInfoDM->GetCellData(_T("���δ��⿹�����"), i, sValue);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			if(sValue.IsEmpty())
			{
				nReserveCnt++;
			}
		}
		
		if((_T("0") == sStatus || _T("2") == sStatus) 
 			&& strUserManageCode == sManageCode)
		{
			if(sAppendixYN.Compare(_T("A")) == 0) {
				if(m_pCheckLoan->IsAppendixIncludeLoanCnt() == 0)
					nLoanCnt++;
			}
			else {
				nLoanCnt++;	
			}
		}
	}
	
	CString sUserKey, sLoanBookCnt;
	CString sDBLoanBookCnt, sTotalLoanBookCnt, sCanLoanBookCnt;
	
	sDBLoanBookCnt.Format(_T("%d"), nUnityLoanCnt);

	pDM->GetCellData(_T("������KEY"), 0, sUserKey);
	pDM->GetCellData(_T("�Ѵ���å��"), 0, sLoanBookCnt); 

	if(_ttoi(sLoanBookCnt) != _ttoi(sDBLoanBookCnt))
	{
		if(!(m_pManageValue->m_bVolReserve))
		{
			pDM->StartFrame();
			pDM->InitDBFieldData();
			
			if(1 > _ttoi(sDBLoanBookCnt))
			{
				sDBLoanBookCnt = _T("0");
			}
			pDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sDBLoanBookCnt, TRUE);
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("����å��"), __WFILE__, __LINE__), TRUE);
			pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
			ids = pDM->SendFrame();
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			pDM->EndFrame();
		}
		sLoanBookCnt.Format(_T("%d"), nLoanCnt);
		pDM->SetCellData(_T("�Ѵ���å��"), sLoanBookCnt, 0);
		pDM->GetCellData(_T("����å������"), 0, sTotalLoanBookCnt);
		sTotalLoanBookCnt = sTotalLoanBookCnt.Mid(sTotalLoanBookCnt.Find(_T("/"))+1);
		sCanLoanBookCnt.Format(_T("%d"), _ttoi(sTotalLoanBookCnt) - nLoanCnt);
		sLoanBookCnt += _T("/") + sTotalLoanBookCnt;
		pDM->SetCellData(_T("���Ⱑ��å��"), sCanLoanBookCnt, 0);
		pDM->SetCellData(_T("����å������"), sLoanBookCnt, 0);
		pCM->SetControlMgrData(_T("���Ⱑ��å��"), sLoanBookCnt);
	}

	CString sReserveBookCnt, sDBReserveBookCnt, sTotalReservationBookCnt, sCanReservationBookCnt ;
	sDBReserveBookCnt.Format(_T("%d"), nReserveCnt);

	pDM->GetCellData(_T("�ѿ���å��"), 0, sReserveBookCnt);

	if(_ttoi(sReserveBookCnt) != _ttoi(sDBReserveBookCnt))
	{
		if(!(m_pManageValue->m_bVolReserve))
		{
			pDM->StartFrame();
			pDM->InitDBFieldData();
			pDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sDBReserveBookCnt, TRUE);
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("����å��"), __WFILE__, __LINE__), TRUE);
			pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
			pDM->SendFrame();
			pDM->EndFrame();
		}
		pDM->SetCellData(_T("�ѿ���å��"), sDBReserveBookCnt, 0);	
		
		pDM->GetCellData(_T("����å������"), 0, sTotalReservationBookCnt);
		sTotalReservationBookCnt = sTotalReservationBookCnt.Mid(sTotalReservationBookCnt.Find(_T("/"))+1);
		sCanReservationBookCnt.Format(_T("%d"), _ttoi(sTotalReservationBookCnt) - _ttoi(sDBReserveBookCnt));
		sDBReserveBookCnt += _T("/") + sTotalReservationBookCnt;
		pDM->SetCellData(_T("���డ��å��"), sCanReservationBookCnt, 0);
		pDM->SetCellData(_T("����å������"), sDBReserveBookCnt, 0);
		
		pCM->SetControlMgrData(_T("���డ��å��"), sDBReserveBookCnt);
	}

	return 0;	

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InputMessageBox(CString sMsg)
{
EFS_BEGIN

	return 0;

	CTime tTime = CTime::GetCurrentTime();
	
	CString sTime;
	sTime = tTime.Format(_T("%H�� %M�� %S��"));

	CESL_DataMgr* pMessageDM = m_pParentMgr->FindDM(_T("DM_�޽����ڽ�"));
	if(pMessageDM == NULL)	return 0;

	if(pMessageDM->GetRowCount() == 1000)
	{
		pMessageDM->FreeData();
	}

	pMessageDM->InsertRow(-1);

	INT nInsertRow = pMessageDM->GetRowCount() - 1;

	pMessageDM->SetCellData(_T("�ð�"), sTime, nInsertRow);
	pMessageDM->SetCellData(_T("�޽���"), sMsg, nInsertRow);
	
	m_pParentMgr->SendMessage(WM_INSERT_MESSAGE_BOX);

	if(m_pParentMgr->pParentMgr != NULL)
	{
		m_pParentMgr->pParentMgr->SendMessage(WM_INSERT_MESSAGE_BOX);
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ShareInfoCenterUpload(CString sLoanInfoType, CString sUserInfoType, INT nMode/*=0*/)
{
EFS_BEGIN

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);	
	
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;

	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pParentMgr->m_pInfo->LIB_CODE;

	
	if(sLoanInfoType.Compare(_T("I")) == 0)
	{
		CString sRecKey;
		GetTempData(SHARE_CO_LOAN_TBL_INSERT_REC_KEY, sRecKey);
		
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}
	else if(sLoanInfoType.Compare(_T("U")) == 0)
	{
		CString sRecKey;
		CString sUpdateFields;
		GetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY , sRecKey);
		GetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS , sUpdateFields);
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;				
	}
	else if(sLoanInfoType.Compare(_T("D")) == 0 )
	{
		CString sRecKey;
		GetTempData(SHARE_CO_LOAN_TBL_DELETE_REC_KEY , sRecKey);
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}

	if(sLoanInfoType.Compare(_T("N")) != 0)
		m_pLoanShareManager->SendLoanInfoUpload(UploadShareInfo, nMode);
	
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		return 0;
	}

	
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	
	
	if(sUserInfoType.Compare(_T("I")) == 0)
	{
		CString sRecKey;
		GetTempData(SHARE_CO_LOAN_USER_TBL_INSERT_REC_KEY, sRecKey);

		UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}
	else if(sUserInfoType.Compare(_T("U")) == 0)
	{
		CString sRecKey;
		CString sUpdateFields;
		GetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY , sRecKey);
		GetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS , sUpdateFields);
				
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;		
	}
	else if(sUserInfoType.Compare(_T("D")) == 0)
	{
		CString sRecKey;
		GetTempData(SHARE_CO_LOAN_USER_TBL_DELETE_REC_KEY , sRecKey);
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}

	
	
	if(UploadShareInfo.UploadShareLoanInfo.sUpdatedFields.Find(_T("USER_CLASS")) >= 0 ||
		 UploadShareInfo.UploadShareLoanInfo.sUpdatedFields.Find(_T("LOAN_STOP_DATE")) >= 0)
	{
		UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("N");
	}

	if(sUserInfoType.Compare(_T("N")) != 0)
		m_pLoanShareManager->SendLoanInfoUpload(UploadShareInfo);
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchUserBookFromKey(CString sUserKey, BOOL bMsgShow/*=TRUE*/, BOOL bPasswd/*=FALSE*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SearchUserBookFromKey");

	ids = SearchLoanInfo(sUserKey, _T(""), bPasswd);
	if(0 > ids) return ids;
	if(ids == 1)
	{
		SelfCloseMessageBox(_T("�������� �ʴ� �̿����Դϴ�."));
		
		SPFExcutePath(_T("ȭ���ʱ�ȭ"));
		return 1;		
	}
	
	if(ids == 2)
	{
		
		SPFExcutePath(_T("ȭ���ʱ�ȭ"));
		return 1;
	}

	Send3000ApiSocket(_T("ȸ��_�˻�"), sUserKey);
	
	CString sMemberState;
	CString sLostUserCard;
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM(sMemberDMAlias);
	ids = pTmpDM->GetCellData(_T("ȸ������"), 0, sMemberState);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(TRUE == bMsgShow)
	{		
		ids = pTmpDM->GetCellData(_T("ȸ�����н�"), 0, sLostUserCard);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(sMemberState == _T("1"))
		{
			if(sLostUserCard == _T("Y"))
			{
				SelfCloseMessageBox(_T("�нǵ� ȸ��ī�� �Դϴ�."), m_pParentMgr);
			}
			else
			{
				SelfCloseMessageBox(_T("�˻��� �̿��ڴ� ��������ȸ���Դϴ�."), m_pParentMgr);
			}
		}
		
		else if(sMemberState == _T("3"))
		{
			SelfCloseMessageBox(_T("�˻��� �̿��ڴ� Ż���ûȸ���Դϴ�."), m_pParentMgr);
		}
		
		else
		{
			if(sLostUserCard == _T("Y"))
			{
				SelfCloseMessageBox(_T("�нǵ� ȸ��ī�� �Դϴ�."), m_pParentMgr);
			}
			else
			{
				SelfCloseMessageBox(_T("�̿��ڰ� �˻��Ǿ����ϴ�."), m_pParentMgr, 2000);
			}
		}
		
		
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddLoanInfoGrid()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AddLoanInfoGrid");

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("�ڷ�Ǳ���"), _T("�ڷ��"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_Grid* pGrid = (CESL_Grid*)(m_pParentMgr->FindControl(sCMAlias, _T("MainGrid")));
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	INT nInsertIndex = pDM->GetRowCount() - 1;

	
	CString strGetData;
	CTime tCurrent = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strGetData);

	CString sData, sDesc;
	
	ids = pDM->GetCellData(_T("��������"), nInsertIndex, sData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������"), sData, sDesc);
	ids = pDM->SetCellData(_T("�������м���"), sDesc, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	sData.Empty();		sDesc.Empty();
	ids = pDM->GetCellData(_T("����/���ӱ���"), nInsertIndex, sData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if(_T("MO") == sData)
	{
		sDesc = _T("����");
	}
	else if(_T("SE") == sData)
	{
		sDesc = _T("����");
	}
	else
	{
		
		CString sData2;
		pDM->GetCellData(_T("Ÿ������KEY"),nInsertIndex, sData2);
		if(0 != sData2.GetLength())
		{
			sDesc = _T("");
		}
		else
			sDesc = _T("���");
	}

	ids = pDM->SetCellData(_T("�ڷᱸ�м���"), sDesc, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	sData.Empty();		sDesc.Empty();
	ids = pDM->GetCellData(_T("�ηϿ���"), nInsertIndex, sData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	if(_T("A") == sData)
	{
		sDesc = _T("�η�");
	}
	else
	{
		sDesc = _T("å");
	}
	ids = pDM->SetCellData(_T("�ηϿ��μ���"), sDesc, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	sData.Empty();		sDesc.Empty();
	CString sDelayDay;
	ids = pDM->GetCellData(_T("�������"), nInsertIndex, sData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	ids = pDM->GetCellData(_T("��ü�ϼ�"), nInsertIndex, sDelayDay);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	
	CString strLILLLoanKey;
	pDM->GetCellData(_T("Ÿ������KEY"), nInsertIndex, strLILLLoanKey);
	CString strLILLTran;
	pDM->GetCellData(_T("Ʈ����ǻ���"), nInsertIndex, strLILLTran);
	if(!strLILLLoanKey.IsEmpty())
	{ 
		if(_T("0") == strLILLTran)
		{
			sDesc = _T("��û");
		}
		else if(_T("1") == strLILLTran)
		{
			sDesc = _T("�߼�");
		}
		else if(_T("3") == strLILLTran)
		{
			sDesc = _T("�Լ�");
		}
		else if(_T("4") == strLILLTran)
		{
			sDesc = _T("Ÿ������");
		}
		else 
		{
			sDesc = _T("�˼�����");
		}
	}
	else
	{ 
		if(_T("0") == sData)	
		{
			if(0 < _ttoi(sDelayDay))	
			{
				sDesc = _T("��ü");
			}
			else 
			{
				sDesc = _T("����");
			}
		}
		else if(_T("1") == sData)	
		{
			sDesc = _T("�ݳ�");
		}
		else if(_T("2") == sData)	
		{
			sDesc = _T("�ݳ�����");
		}
		else if(_T("3") == sData)
		{
			CString sRecKey, sValue, sReservationExpireDate;
			if(_T("Y") == m_pManageValue->m_sUnmannedReserveSys)
			{
				ids = pDM->GetCellData(_T("���δ��⿹�����"), nInsertIndex, sValue);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
				if(_T("Y") == sValue)
				{
					sDesc = _T("����(���δ����û)");
				}
				else if(_T("O") == sValue)
				{
					ids = pDM->GetCellData(_T("���ุ����"), nInsertIndex, sReservationExpireDate);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
					if(sReservationExpireDate.IsEmpty())
					{
						sDesc = _T("����");
					}
					else
					{
						CTime tExpireDate(_ttoi(sReservationExpireDate.Mid(0,4)), _ttoi(sReservationExpireDate.Mid(5,2)), 
											_ttoi(sReservationExpireDate.Mid(8,2)),_ttoi(sReservationExpireDate.Mid(11,2)), 0, 0); 
						
						if(tExpireDate < CTime(tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(), tCurrent.GetHour(), 0, 0))
						{
							sDesc = _T("����(���δ�������)"); 
						}
						else
						{
							sDesc = _T("����(���δ�����)"); 
						}
					}
				}
				else
				{
					sDesc = _T("����");
				}
			}
			else
			{
				sDesc = _T("����");
			}
		}
		else if(_T("4") == sData)	
		{
			sDesc = _T("�������");
		}
		
		else if(_T("O") == sData)	
		{
			sDesc = _T("å�η�����");
		}
		else if(_T("L") == sData) 
		{
			sDesc = _T("å�η�����");
		}
		else if(_T("D") == sData)	
		{
			sDesc = _T("å�η��ݳ�����");
		}
		else	
		{
			sDesc = _T("");
		}
	}
	
	if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse)
	{
		ids = pDM->GetCellData(_T("��������ȣ��������"), nInsertIndex, sData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("Y") == sData)
		{
			sDesc = _T("��������ȣ����");
		}
	}
	
	ids = pDM->SetCellData(_T("������¼���"), sDesc, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	ids = pGrid->DisplayLine(nInsertIndex);	
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	if(5 < nInsertIndex)
	{
		pGrid->SetTopRow(nInsertIndex);
	}

	CString sIndex;
	sIndex.Format(_T("%d"), nInsertIndex + 1);
	pGrid->SetTextMatrix(pDM->GetRowCount(), 0, sIndex); 

	ids = m_pParentMgr->ControlDisplay(sCMAlias, _T("MainGrid"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->ControlDisplay(sCMAlias, _T("MainGrid2"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	return 0;
	
EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::CheckLoanBookCnt(CString sIsAppendix, BOOL bIsILL, CString strBookManageCode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("CheckLoanBookCnt");

	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;
	if(_T("1") == sLoanType)
	{
		return 0;
	}
	
	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{ 
		
		CString sIsLoanBookCnt;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�̵����Ⱑ��å��"), sIsLoanBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(1 > _ttoi(sIsLoanBookCnt)) 
		{	
			if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt() && _T("A") == sIsAppendix)
			{ 
			}
			else
			{ 
				SelfCloseMessageBox(_T("���� �ѵ��� �ʰ��Ͽ� ������ �� �����ϴ�."));
				return 2;
			}			
		}
	} 
	else
	{
		
		if(_T("Y") == m_pManageValue->m_sUnityLoanCntYN)
		{ 
			
			CString sIsLoanBookCnt;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���մ��Ⱑ��å��"), sIsLoanBookCnt, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			
			CString sIsIndividualLoanBookCnt;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���Ⱑ��å��"), sIsIndividualLoanBookCnt, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			
			
			
			BOOL bOnlyAppendix = FALSE;

			if(1 > _ttoi(sIsLoanBookCnt))
			{
				if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt() && _T("A") == sIsAppendix)
				{ 
				}
				else
				{ 
					SelfCloseMessageBox(_T("���մ��Ⱑ�ɱǼ� �ѵ��� �ʰ��Ͽ� ������ �� �����ϴ�."));
					return 2;
				}			
			}
			
			if(TRUE == bIsILL)
			{
				CESL_DataMgr TmpDM;
				TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);

				CString strUserClassCode;
				m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����������"), strUserClassCode, 0);
				CString strQuery;
				CString strLibTotalLoanCanCnt;
				strQuery.Format(_T("SELECT LOAN_BOOK_CNT FROM CD_INDIVIDUAL_CLASS_TBL ")
								_T("WHERE CODE='%s' AND MANAGE_CODE='%s'")
								, strUserClassCode, strBookManageCode);
				TmpDM.GetOneValueQuery(strQuery, strLibTotalLoanCanCnt);

				CString strIsAppendixCnt;

				strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
								_T("WHERE CLASS_ALIAS='�ηϰ���' AND VALUE_NAME='�ηϴ���Ǽ����Կ���' ")
								_T("AND MANAGE_CODE='%s'"), strBookManageCode);
				TmpDM.GetOneValueQuery(strQuery, strIsAppendixCnt);

				CESL_DataMgr* pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
				INT nLibLoanCnt = 0;

				CString strLoanStatus, strLDeveice;
				const int ROW_COUNT = pLoanDM->GetRowCount();

				for(INT nRow = 0; nRow < ROW_COUNT; nRow++)
				{					
					pLoanDM->GetCellData(_T("�������"), nRow, strLoanStatus);					
					if(_T("0") == strLoanStatus || _T("2") == strLoanStatus || 
						_T("L") == strLoanStatus || _T("D") == strLoanStatus)
					{
						pLoanDM->GetCellData(_T("L_DEVICE"), nRow, strLDeveice);
						if(_T("KOLASIII-�̵�������") == strLDeveice)
						{
							continue;
						}
						CString strGet;
						pLoanDM->GetCellData(_T("��������"), nRow, strGet);
						CString strType;
						pLoanDM->GetCellData(_T("�ηϿ���"), nRow, strType);

						if(strGet == strBookManageCode)
						{
							if(_T("A") == strType)
							{
								if(_T("Y") == strIsAppendixCnt)
								{
									nLibLoanCnt++;
								}

								
								else bOnlyAppendix = TRUE;
							}
							else
							{
								nLibLoanCnt++;
							}
						}
					}
					else 
					{
						continue;
					}
				}	

				if(nLibLoanCnt >= _ttoi(strLibTotalLoanCanCnt))
				{
					if(bOnlyAppendix == 0)
					{					
						SelfCloseMessageBox(_T("������ ���Ⱑ�ɱǼ� �ѵ��� �ʰ��Ͽ� ������ �� �����ϴ�."));
						return 2;
					}
				}
			}
			else
			{
			
				if(1 > _ttoi(sIsIndividualLoanBookCnt))
				{
					if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt() && _T("A") == sIsAppendix)
					{ 
					}
					else
					{ 
						
							if(bOnlyAppendix == 0)
							{	
								SelfCloseMessageBox(_T("������ ���Ⱑ�ɱǼ� �ѵ��� �ʰ��Ͽ� ������ �� �����ϴ�."));
			
								return 2;
							}
					}
				}
			}
		} 
		else
		{ 
			
			CString sUserManageCode;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("��������"), sUserManageCode, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);

			CString sIsLoanBookCnt;
			
			
			if(TRUE == bIsILL || sUserManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE)
			{ 
				ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("Ÿ�����Ⱑ��å��"), sIsLoanBookCnt, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
			}
			else		
			{ 
				ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���Ⱑ��å��"), sIsLoanBookCnt, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
			}

			
			
			if(1 > _ttoi(sIsLoanBookCnt)) 
			{	
				if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt() && _T("A") == sIsAppendix)
				{ 
				}
				else
				{ 
					SelfCloseMessageBox(_T("���� �ѵ��� �ʰ��Ͽ� ������ �� �����ϴ�."));
					return 2;
				}			
			}
		} 
	} 
	
	if(_T("Y") == m_pManageValue->m_sIsUnionLoanCnt)
	{
		CString sUnionCnt;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���մ���å��"), sUnionCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
		if(sUnionCnt.IsEmpty())
		{
			SelfCloseMessageBox(_T("���մ��������� ����ȭ ���Դϴ�."));
			return 2;
		}

		CString strTotalLoanCnt;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�Ѵ���å��"), strTotalLoanCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);

		if(1 > (_ttoi(sUnionCnt) - _ttoi(strTotalLoanCnt)))
		{
			
			if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt() && _T("A") == sIsAppendix)
			{
			}
			else
			{
				SelfCloseMessageBox(_T("���� ���� �ѵ��� �ʰ��Ͽ� ������ �� �����ϴ�."));
				return 2;
			}
		}	
	}

	return 0;

EFS_END
return -1;
}

CString CBL_LOC_LOAN_PROC::GetSpeciesKey(CESL_DataMgr *pDM, CString recKey, INT type)
{
	EFS_BEGIN

	CString tblName;
	switch(type)
	{
		case 1 :
			tblName = _T("BO_BOOK_TBL");
			break;
		case 2 :
			tblName = _T("SE_BOOK_TBL");
			break;
		default :
			return _T("");
			break;
	}

	CString query, speciesKey;
	query.Format(_T("SELECT SPECIES_KEY FROM %s WHERE REC_KEY = %s"), tblName, recKey);
	pDM->GetOneValueQuery(query, speciesKey);
	return speciesKey;

	EFS_END

	return _T("");
}

INT CBL_LOC_LOAN_PROC::ReserveLinkBookInfo()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReserveLinkBookInfo");

	INT nSIndex = 0;
	CString sStatus = _T("");
	CString sLoanKey = _T("");
	CString sMsg = _T("");
	CReserveInfo RI;
	CReserveComplete RC;

	
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_Grid *pGrid = (CESL_Grid*)m_pParentMgr->FindControl(sCMAlias, sGridAlias);
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	nSIndex = pGrid->GetIdx();
	
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("�������"), sStatus, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(sStatus.Compare(_T("3")) != 0)
	{
		AfxMessageBox(_T("�� �ڷ�� �������� �ڷᰡ �ƴմϴ�."));
		return 1;
	}
	
	
	
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("����/���ӱ���"), RI.sPublishFormCode, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("��KEY"), RI.sSpeciesKey, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("��ȣ����KEY"), RI.sVolInfo, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	CReserveLinkBookInfoDlg BookDlg(m_pParentMgr);
	BookDlg.SetIsAutoSetReservationDay(m_pManageValue->m_sIsAutoSetReservationDay);
	BookDlg.SetInfo(RI);
	ids = BookDlg.DoModal();
	
	
	if(5 == ids)
	{
		AfxMessageBox(_T("�������� �� �� �ִ� �ڷᰡ �����ϴ�."), MB_ICONINFORMATION);
		return 0;
	}
	
	
	ids = BookDlg.GetInfo(RI);

	
	if(ids > 0) return 0;


	
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("����/�ݳ�����KEY"), sLoanKey, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	
	CESL_DataMgr* pUpdateDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pUpdateDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = RC.DBReserveComplete(RI, m_pParentMgr, sLoanKey, pUpdateDM);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		
	
	
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("����"), RI.sWorkingStatus, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("�����ڷ����"), RI.sPrevWorkingStatus, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("��Ϲ�ȣ"), RI.sRegNo, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("IBS_û��_��ġ��ȣ_�ڵ�"), RI.sSeparateShelfCode, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("�ڷ��"), RI.sShelfLocCode, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("IBS_û��_��å��ȣ"), RI.sVolCode, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("û����ȣ"), RI.sCallNo, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("RFID�ø���"), RI.sRFIDSerial, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("åKEY"), RI.sBookKey, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	pGrid->DisplayLine(nSIndex);	

	sMsg.Format(_T("���������Ϸ�Ǿ����ϴ�."));
	AfxMessageBox(sMsg);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::Receipt_Print(INT nType)
{
	if(m_pManageValue->m_sUseReceipt != _T("Y")) return 0;
	
	if(m_ReceiptDlg->GetSafeHwnd() == NULL)
	{
		m_ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG(nType, m_pParentMgr, m_pManageValue);
		m_ReceiptDlg->pMain = m_pParentMgr->pMain;
		m_ReceiptDlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
		m_ReceiptDlg->m_pInfo = m_pParentMgr->m_pInfo;				
		if(m_ReceiptDlg->Create(m_pParentMgr))
		{
			m_ReceiptDlg->CenterWindow();
			m_ReceiptDlg->ShowWindow(SW_SHOW);
			m_ReceiptDlg->UpdateWindow();

			m_ReceiptDlg->initData();
		}
		else
		{
			delete m_ReceiptDlg;
		}
	}
	else
	{
		m_ReceiptDlg->ShowWindow(SW_SHOW);
		m_ReceiptDlg->initData();
	}		
	

	return 0;

}

INT CBL_LOC_LOAN_PROC::Receipt_PrintCheck()
{	
	CString	strMsg;
	CString	strMode;
	INT nLoanReceiptCnt;
	INT nReturnReceiptCnt;		

	if(m_pManageValue->m_sUseReceipt != _T("Y")) return 0;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pDM->GetRowCount() <= 0) return 0;	
	
	if(m_ReceiptDlg->GetSafeHwnd() == NULL)
	{
		m_ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG(0, m_pParentMgr, m_pManageValue);
		if(m_ReceiptDlg == NULL) return 0;
		m_ReceiptDlg->pMain = m_pParentMgr->pMain;
		m_ReceiptDlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
		m_ReceiptDlg->m_pInfo = m_pParentMgr->m_pInfo;				
		if(m_ReceiptDlg->Create(m_pParentMgr))
		{			
			m_ReceiptDlg->CenterWindow();			
			m_ReceiptDlg->UpdateWindow();			
		}
		else
		{
			delete m_ReceiptDlg;
		}
	}
	
	if(!m_ReceiptDlg->IsWindowVisible())	return 0;

	StartLog(_T("���������üũ"));
	nLoanReceiptCnt = m_ReceiptDlg->GetLoanReceiptPrintCnt();
	nReturnReceiptCnt = m_ReceiptDlg->GetReturnReceiptPrintCnt();

	if(nLoanReceiptCnt == 0 && nReturnReceiptCnt == 0) 
	{
		EndLog();
		return 0;
	}
	else
	{
		if(m_ReceiptDlg->m_strMode == _T("0"))
		{
			if(AfxMessageBox(_T("����� �������� �����մϴ�.\n����Ͻðڽ��ϱ�?"), MB_YESNO) == IDYES)
			{
				m_ReceiptDlg->ReceiptAutoPrint(TRUE);							
			}
			else
			{
				
			}
		} else if(m_ReceiptDlg->m_strMode == _T("1"))
		{
			m_ReceiptDlg->ReceiptAutoPrint(TRUE);
		} else 
		{
			m_ReceiptDlg->ReceiptAutoPrint(FALSE);
		}
	}	

	EndLog();
	return 0;
}
INT CBL_LOC_LOAN_PROC::InitGridColor()
{	
	CString			sGridAlias = _T("MainGrid");
	CString			sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString			sLoanInfoDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	INT				nCols, nRowCount, i;

	CESL_DataMgr	*pLoanInfoDM = m_pParentMgr->FindDM(sLoanInfoDMAlias);
	nRowCount = pLoanInfoDM->GetRowCount();	

	if(m_bSimpleGrid == TRUE)
	{
		if(nRowCount > 0) return 0; 
	}

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));

	nCols = pGrid->GetCols(0);
	for (i = 1 ; i < nCols ; i++) 
	{
		pGrid->SetCol(i);
		pGrid->SetCellBackColor(RGB(255,255,255));
		pGrid->SetCellForeColor(RGB(0,0,0));
		pGrid->SetCellFontBold(FALSE);
		pGrid->SetCellFontItalic(FALSE);
	}
	return 0;
}

INT CBL_LOC_LOAN_PROC::SetBBankMember()
{
EFS_BEGIN

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InsertBBankRecord(CKolas2up_MobileApi *pMobileApi, INT nMode)
{
EFS_BEGIN

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InsertBBankRecord(CKolas2up_MobileApi *pMobileApi, CString strLoan_Key, INT nMode)
{
EFS_BEGIN

	return 0;

EFS_END
return -1;
}

INT	CBL_LOC_LOAN_PROC::ClearSearchUserBasket()
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("ClearSearchUserBasket");
	
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_SEARCH_USER_BASKET"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	pDM->FreeData();

	ids = m_pParentMgr->ControlDisplay(_T("CM_BO_LOC_3100_SEARCH_USER_BASKET"), _T("�̿��ڰ˻��ٱ���"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_SEARCH_USER_BASKET"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	

	pCM->AllControlClear();

	return 0;
EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddSearchUserBasket()
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("AddSearchUserBasket");

	
	CESL_DataMgr *pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	if(pMemberDM->GetRowCount() <= 0) return 0;	
	
	CESL_DataMgr* pBasketDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_SEARCH_USER_BASKET"));
	if(NULL == pBasketDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	CString strRecKey, strName, strUserNo;

	ids = pMemberDM->GetCellData(_T("������KEY"), 0, strRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	ids = pMemberDM->GetCellData(_T("�������̸�"), 0, strName);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	ids = pMemberDM->GetCellData(_T("�����ڹ�ȣ"), 0, strUserNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString strValue1;

	ids = m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), strValue1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();

	if(_T("Y") != strValue1)
	{
		CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pManageValue->m_sIsUnityLoanService );
	}
	
	CString strTime, strView;

	if( m_pManageValue->m_sLoanUserAccessMode.IsEmpty() || m_pManageValue->m_sLoanUserAccessMode == _T("1") )
		strName = _T("***");
	
	CString strGetData;
	CTime t = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strGetData);
	
	strTime.Format(_T("%02d�� %02d�� %02d��"),t.GetHour(),t.GetMinute(),t.GetSecond());
	strView.Format(_T("%s(%s)"),strName,strUserNo);

	if(0 == pBasketDM->GetRowCount())
		pBasketDM->InsertRow(-1);
	else
		pBasketDM->InsertRow(0);	

	pBasketDM->SetCellData(_T("�˻��ð�"),strTime,0);
	pBasketDM->SetCellData(_T("����������"),strView,0);

	pBasketDM->SetCellData(_T("REC_KEY"),strRecKey,0);
	pBasketDM->SetCellData(_T("�����ڸ�"),strName,0);
	pBasketDM->SetCellData(_T("�����ڹ�ȣ"),strUserNo,0);

	
	INT nRowCnt = pBasketDM->GetRowCount();
	
	if(m_pManageValue->m_nUserBasketCnt < nRowCnt)	
	{
		for(INT i = nRowCnt ; i > m_pManageValue->m_nUserBasketCnt ; i--)
		{
			pBasketDM->DeleteRow(i-1);
		}
	}

	m_pParentMgr->ControlDisplay(_T("CM_BO_LOC_3100_SEARCH_USER_BASKET"), _T("�̿��ڰ˻��ٱ���"));

	
	if(1 < pBasketDM->GetRowCount())
	{
		m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_USER_BASKET"), _T("�����̿���"), pBasketDM->GetCellData(_T("����������"),1));		
	}

	return 0;
EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewSimpleFamilyMember()
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("ViewSimpleFamilyMember");

	ids = ClearSimpleFamilyMember();
	if(0 > ids) return ids;

	ids = SearchFamilyMember();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	if(0 == ids)
	{
		return 0;
	}
	
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CKolas2up_ViewerControl* pCtrl = (CKolas2up_ViewerControl*)m_pParentMgr->GetDlgItem(1975);
	if(NULL == pCtrl) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_FAMILY_ID_INFO"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr* pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT	nRowCount = pDM->GetRowCount();
	for(INT i = 0; i < nRowCount; i++)
	{		
		CString strKey;
		CString strData;
		CString strUserNo;
		CString strUserName;
		CString strLoanCnt;

		ids = pDM->GetCellData(_T("������KEY"), i, strKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = pDM->GetCellData(_T("�����ڹ�ȣ"), i, strUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		strUserNo.TrimLeft();
		strUserNo.TrimRight();

		
		CString sValue;
		ids = m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), sValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		sValue.TrimLeft();
		sValue.TrimRight();
		sValue.MakeUpper();

		if(_T("Y") != sValue)
		{
			CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pManageValue->m_sIsUnityLoanService );
		}

		ids = pDM->SetCellData(_T("�����ڹ�ȣǥ��"), strUserNo, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = pDM->GetCellData(_T("�̸�"), i, strUserName);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		strUserName.TrimLeft();
		strUserName.TrimRight();
		
		if(m_pParentMgr->m_pInfo->MODE == 10000)
		{
			ids = pDM->GetCellData(_T("�̵����Ⱑ��å��"), i, strLoanCnt);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			strLoanCnt.TrimLeft();
			strLoanCnt.TrimRight();
		}
		else
		{
			
			if(m_pManageValue->m_sUnityLoanCntYN == _T("Y"))
			{
				ids = pDM->GetCellData(_T("���Ⱑ��å��_����ȯ��"), i, strLoanCnt);
			}
			else
			{
				ids = pDM->GetCellData(_T("���Ⱑ��å��"), i, strLoanCnt);
			}
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			strLoanCnt.TrimLeft();
			strLoanCnt.TrimRight();
		}
		
		if(!strUserNo.IsEmpty())
		{
			strData = strUserNo;
		}

		if(!strUserName.IsEmpty())
		{
			if(!strData.IsEmpty())
			{
				strData += _T("/");
			}
			if ( m_pManageValue->m_sLoanUserAccessMode.IsEmpty() || m_pManageValue->m_sLoanUserAccessMode == _T("1") ) strUserName = _T("***");

			strData += strUserName;
		}
		
		if(!strLoanCnt.IsEmpty())
		{
			if(0 > _ttoi(strLoanCnt))
			{
				strLoanCnt = _T("0");
			}

			if(!strData.IsEmpty())
			{
				strData += _T("(");
			}

			strData += strLoanCnt + _T(")");
		}

		pCtrl->SetControlData(strKey, strData);
	}

	while(3 > nRowCount)
	{
		pCtrl->SetControlData(_T(" "), _T(" "));
		nRowCount++;
	}

	pCtrl->Display();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ClearSimpleFamilyMember()
{
EFS_BEGIN
	
	CString strFuncName = _T("ClearSimpleFamilyMember");

	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	
	CKolas2up_ViewerControl* pCtrl = (CKolas2up_ViewerControl*)m_pParentMgr->GetDlgItem(1975);

	
	
	if(NULL == pCtrl && m_pParentMgr->pParentMgr != NULL ) 
		pCtrl = (CKolas2up_ViewerControl*)m_pParentMgr->pParentMgr->GetDlgItem(1975);		
	
	if(NULL == pCtrl) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
	pCtrl->FreeLIST();
	pCtrl->Clear();
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::LoanStopDateCancel()
{
EFS_BEGIN
	
	CESL_DataMgr	*pDM;
	CString			strRecKey;
	CString			strUserNo;
	CString			strName;
	CString			strLoanStopDate;
	
	pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));

	pDM->GetCellData(_T("������KEY")	, 0, strRecKey);
	pDM->GetCellData(_T("�����ڹ�ȣ")	, 0, strUserNo);
	pDM->GetCellData(_T("�������̸�")	, 0, strName);
	pDM->GetCellData(_T("����������")	, 0, strLoanStopDate);

	if(strLoanStopDate.IsEmpty())
	{
		SelfCloseMessageBox(_T("������ ������������ �����ϴ�."));
	}
	else
	{
		CTime tCurrentTime;	
		tCurrentTime = CTime::GetCurrentTime();
		tCurrentTime = CTime(tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 0, 0, 0);		
		
		CTime tLoanStopTime(_ttoi(strLoanStopDate.Mid(0,4)), _ttoi(strLoanStopDate.Mid(5,2)), _ttoi(strLoanStopDate.Mid(8,2)), 0, 0, 0);
		
		CTime tLimitTime(tCurrentTime.GetYear()+1, tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 0, 0, 0);
		
		if(tLimitTime < tLoanStopTime)
		{
			AfxMessageBox(	_T("������������ 1���� ���� �� �����ϴ�.\r\n")
							_T("������������ Ȯ���� �ֽʽÿ�."));
			return 0;
		}
		
		if(tLoanStopTime < tCurrentTime)
		{
			AfxMessageBox(_T("������ ������������ �����ϴ�."));
		}
		else
		{	
			CFeeLoanStop dlg(m_pParentMgr, m_pLoanShareManager, m_pManageValue);
			dlg.SetUserKey(pDM, strRecKey, strUserNo, strName, strLoanStopDate);
			dlg.m_strLibCode = m_strLibCode;
			dlg.DoModal();
			if(dlg.m_bPrint == TRUE)
			{
				ReLoadMemberInfo();
			}
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::KL_OtherReturnMgr()
{
	if(NULL == m_pOtherReturnMgr)
	{
		m_pOtherReturnMgr = new CKL_OTHER_RETURN_MAIN(m_pParentMgr);
		m_pOtherReturnMgr->pMain = m_pParentMgr->pMain;
		m_pOtherReturnMgr->pParentMgr = (CESL_Mgr*)m_pParentMgr;
		m_pOtherReturnMgr->m_pInfo = m_pParentMgr->m_pInfo;				
		
		
		m_pOtherReturnMgr->m_pLoanManageValue = m_pManageValue;
		m_pOtherReturnMgr->m_pLoanShareManager = m_pLoanShareManager;
		
		if(m_pOtherReturnMgr->Create(m_pParentMgr))
		{			
			m_pOtherReturnMgr->CenterWindow();
			m_pOtherReturnMgr->ShowWindow(SW_SHOW);
			m_pOtherReturnMgr->UpdateWindow();
			m_pOtherReturnMgr->pMain->SendMessage(WM_USER+1028, (WPARAM)m_pOtherReturnMgr, 0);
		}
		else
		{
			delete m_pOtherReturnMgr;
			m_pOtherReturnMgr = NULL;
		}
	}
	else
	{
		if(TRUE == m_pOtherReturnMgr->IsWindowVisible() && TRUE == m_pOtherReturnMgr->m_bShow)
		{
			m_pOtherReturnMgr->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pOtherReturnMgr->ShowWindow(SW_RESTORE);
			m_pOtherReturnMgr->SetActiveWindow();
			m_pOtherReturnMgr->SetFocus();
		}
	}

	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_UserLoanInfo(CString strUserNo)
{
	CString strFuncName = _T("KL_UserLoanInfo");

	if(10000 != m_pParentMgr->m_pInfo->MODE)
	{
		CString strSearchMode = _T("");
		GetTempData(_T("�⺻�˻�����"), strSearchMode);

		if(_T("1") == strSearchMode || _T("2") == strSearchMode)
		{
			if(m_pParentMgr->m_pInfo->IsConnectUnityLoanService())
			{
				CKL_UserSearchDlg dlg(m_pParentMgr, this);
				dlg.m_strUserNo = strUserNo;
				dlg.m_strWorkNo = _T("");
				dlg.m_nMode = 1;
				dlg.DoModal();
			}
	
	
		}
	}

	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_UserInfoView()
{
	INT ids;
	CString strFuncName = _T("KL_UserInfoView");

	ids = ViewLoanScreen();
	if(0 > ids) return ids;

	return 0;
}


INT CBL_LOC_LOAN_PROC::KL_GetMaxLoanCount()
{
	CKL_UserSearchDlg dlg(m_pParentMgr, this);
	dlg.m_nMode = 2;

	if(m_pParentMgr->m_pInfo->IsConnectUnityLoanService())
	{
		dlg.DoModal();
	}

	if(dlg.m_strMaxLoanCount.IsEmpty())
	{
		
		CString strGetData = _T("");
		GetTempData(_T("�⺻���մ��Ⱑ�ɱǼ�"), strGetData);

		if(strGetData.IsEmpty())
		{
			m_strMaxLoanCount = _T("20");
		}
		else
		{
			m_strMaxLoanCount = strGetData;
		}
	}
	else
	{
		m_strMaxLoanCount = dlg.m_strMaxLoanCount;
	}

	return 0;
}


INT CBL_LOC_LOAN_PROC::KL_GetManage()
{
	m_strMaxLoanCount = _T("");
	m_strLocalFreeYN = _T("");
	m_strOtherFreeYN = _T("");
	m_strAutoSendYN = _T("");
	m_strLibCodeList = _T("");
	if(m_pParentMgr->m_pInfo->IsConnectUnityLoanService())
	{
		CKL_UserSearchDlg dlg(m_pParentMgr, this);
		dlg.m_nMode = 6;
		if(IDOK == dlg.DoModal())
		{
			m_strMaxLoanCount = dlg.m_strMaxLoanCount;
			m_strLocalFreeYN = dlg.m_strLocalFreeYN;
			m_strOtherFreeYN = dlg.m_strOtherFreeYN;
			m_strAutoSendYN = dlg.m_strAutoSendYN;
			m_strLibCodeList = dlg.m_strLibCodeList;
		}
	}

	CString strGetData = _T("");;
	if(m_strMaxLoanCount.IsEmpty())
	{
		strGetData.Empty();
		GetTempData(_T("�⺻���մ��Ⱑ�ɱǼ�"), strGetData);
		if(strGetData.IsEmpty())
		{
			m_strMaxLoanCount = _T("20");
		}
		else
		{
			m_strMaxLoanCount = strGetData;
		}
	}
	if(m_strLocalFreeYN.IsEmpty())
	{
		strGetData.Empty();
		GetTempData(_T("�������ù�񹫷Ῡ��"), strGetData);
		if(strGetData.IsEmpty())
		{
			m_strLocalFreeYN = _T("N");
		}
		else
		{
			m_strLocalFreeYN = strGetData;
		}
	}
	if(m_strOtherFreeYN.IsEmpty())
	{
		strGetData.Empty();
		GetTempData(_T("�������ù�񹫷Ῡ��"), strGetData);
		if(strGetData.IsEmpty())
		{
			m_strOtherFreeYN = _T("N");
		}
		else
		{
			m_strOtherFreeYN = strGetData;
		}
	}
	
	if(m_strAutoSendYN.IsEmpty())
	{
		
		GetTempData(_T("�ڵ��߼ۿ���"), strGetData);
		if(strGetData.IsEmpty())
		{
			m_strAutoSendYN = _T("N");
		}
		else
		{
			m_strAutoSendYN = strGetData;
		}
	}
	
	if(m_strLibCodeList.IsEmpty())
	{
		strGetData.Empty();
		GetTempData(_T("��������������ȣ����Ʈ"), strGetData);
		if(strGetData.IsEmpty())
		{
			m_strLibCodeList = _T("");
		}
		else
		{
			m_strLibCodeList = strGetData;
		}
	}

	return 0;
}


INT CBL_LOC_LOAN_PROC::KL_GetManage_Condition(CString strLibCode, CString& strBookLibReloanYN)
{
	strBookLibReloanYN = _T("N");
	if(!m_pParentMgr->m_pInfo->IsConnectUnityLoanService()) return 0;
	if ( strLibCode.GetLength() <= 0 ) return 0;

	CKL_UserSearchDlg dlg(m_pParentMgr, this);
	dlg.m_nMode = _MODE_LIB_CON_MANAGE_;
	dlg.m_SearchLibCode = strLibCode;
	dlg.DoModal();

	strBookLibReloanYN = dlg.m_strBookLibReloanYN;
	
	dlg.m_SearchLibCode = _T("");
	dlg.m_strBookLibReloanYN = _T("N");
	
	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_Reloan_Proc(INT nIdx, BOOL& bSuccess)
{	
	INT ids = -1, nExp = 0, nSuccCnt = 0 ;
	CString strBookLibCode = _T(""), strBookLibReloanYN = _T("N"), strFuncName = _T("KL_Reloan_Proc"), strReturnPlanDate = _T("");
	CString strStatus = _T(""), strReloanPeriod = _T(""), strReloanPlanDate = _T(""), strLocalKey = _T(""), strFailReason = _T("");

	CString sDMAlias	= _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias	= _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias	= _T("MainGrid");

	bSuccess = FALSE;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	

	
	ids = pDM->GetCellData(_T("������������ȣ"), nIdx , strBookLibCode);
	ids = KL_GetManage_Condition(strBookLibCode, strBookLibReloanYN);
	if ( ids < 0 ) return ids;

	if ( strBookLibReloanYN != _T("Y") ) {
		AfxMessageBox(_T("��������� ������ȣ�����ڷ��� �ݳ����⸦ ������� �ʽ��ϴ�."));				
		return 0;
	}
	
	
	ids = pDM->GetCellData(_T("�������"), nIdx, strStatus);
	if(  !(_T("0") == strStatus || _T("2") == strStatus )) {
		AfxMessageBox(_T("�������̰ų� �ݳ����� ������ �ڷḸ �ݳ����� �����մϴ�."));		
		return 0;
	}
	
	ids = pDM->GetCellData(_T("�ݳ�������"), nIdx, strReturnPlanDate);
	m_pCheckLoan->GetReturnDelayDay(strReloanPeriod, _T("NO_REG_CODE"));
	CTime tRenewalTime(_ttoi(strReturnPlanDate.Left(4)),_ttoi(strReturnPlanDate.Mid(5,2)),_ttoi(strReturnPlanDate.Mid(8,2)),0,0,0);
	nExp = _ttoi(strReloanPeriod);
	m_pCheckLoan->GetAddTotalHolyDay( nExp , tRenewalTime);		
	CTime tReLoanPlanDate = tRenewalTime + CTimeSpan(nExp, 0, 0, 0);
	strReloanPlanDate = tReLoanPlanDate.Format(_T("%Y/%m/%d"));

	
	pDM->GetCellData(_T("����KEY"), nIdx, strLocalKey);
	CFrameWriter Writer(m_pParentMgr->m_pInfo);
	Writer.setCommand(_T("LILL_K18_LOC_B01_SERVICE"));
	Writer.addRecord();	
	Writer.addElement(_T("������������ȣ"),strBookLibCode);
	Writer.addElement(_T("������������ȣ"),strBookLibCode);
	Writer.addElement(_T("����KEY"),strLocalKey);	
	Writer.addElement(_T("�ݳ�������"),strReloanPlanDate);
	Writer.addElement(_T("�������۾�"), m_pParentMgr->GetWorkLogMsg(_T("������ȣ�����ݳ�����"), __WFILE__, __LINE__));
	
	CFrameSender Sender(m_pParentMgr->m_pInfo);	
	CFrameReader Reader; 
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if (0 > ids) {
		AfxMessageBox(_T("å�������� �̵������� ��ſ� �����Ͽ����ϴ�."));
		pGrid->Display();
		return 0;		
	}
	
	Reader.parseFrame(Reader.getFrame());
	if(_T("OK") != Reader.getElement(0,0)) {
		if(_T("")==Reader.getElement(0,0)) {
			pDM->SetCellData(_T("RESULT"),_T("FAIL"),nIdx);
			AfxMessageBox(_T("���� �߻�"));
		}
		else {
			strFailReason.Format(_T("%s"), Reader.getElement(0, _T("ó���������")));
			pDM->SetCellData(_T("RESULT"), strFailReason, nIdx);
			AfxMessageBox(strFailReason);		
		}		
		return 0;
	}

	
	TCHAR *sLinkDMFieldAlias[2] = {_T("�ݳ�������"), _T("�������")};
	INT nColIndex[2] = {-1, -1};
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, sLinkDMFieldAlias, 2, nColIndex);
	if(0 > ids) return ids;
	
	ids = pDM->SetCellData(_T("�ݳ�������"), strReloanPlanDate, nIdx );	
	ids = pGrid->SetAt(nIdx, nColIndex[0], strReloanPlanDate);

	ids = pDM->SetCellData(_T("�������"), _T("2"), nIdx );
	ids = pGrid->SetAt(nIdx, nColIndex[1], _T("2"));
	
	bSuccess = TRUE;

	return 0;
}


INT CBL_LOC_LOAN_PROC::IsKLReloanUser(CESL_DataMgr* pDM, BOOL& bUser)
{
	INT ids = -1, i = 0;
	CString strDelayDays = _T(""), strUserClass = _T(""), strLostUserCard = _T("");
	bUser = TRUE;

	
	for( i = 0; i < pDM->GetRowCount(); i++) {
		ids = pDM->GetCellData(_T("��ü�ϼ�"), i, strDelayDays);
		if(0 > ids) return ids;
		if(0 < _ttoi(strDelayDays)) {
			bUser = FALSE;
			SelfCloseMessageBox(_T("��ü �̿��ڴ� �ݳ����Ⱑ �Ұ����մϴ�."));
			return 0;
		}
	}
	
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("ȸ������"), strUserClass, 0);
	if(0 > ids) return ids;
	if(_T("0") != strUserClass) {	
		bUser = FALSE;
		SelfCloseMessageBox(_T("����ȸ���� �ݳ����Ⱑ �����մϴ�."));
		return 0;
	}	
	
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("ȸ�����н�"), strLostUserCard, 0);
	if(0 > ids) return ids;
	if(_T("Y") == strLostUserCard) {
		bUser = FALSE;
		SelfCloseMessageBox(_T("ȸ�����н� ȸ���� �ݳ����Ⱑ �Ұ��� �մϴ�."));
		return 0;		
	}

	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_GetPaymentFree()
{
	if(!m_pParentMgr->m_pInfo->IsConnectUnityLoanService())
	{
		m_strLocalFreeYN = _T("N");
		m_strOtherFreeYN = _T("N");
		return 0;
	}
	CKL_UserSearchDlg dlg(m_pParentMgr, this);
	dlg.m_nMode = 5;
	dlg.DoModal();
	m_strLocalFreeYN = dlg.m_strLocalFreeYN;
	m_strOtherFreeYN = dlg.m_strOtherFreeYN;

	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_StartTemp()
{
	m_asFrames.RemoveAll();
	m_asFrames.FreeExtra();

	return 0;
}
INT CBL_LOC_LOAN_PROC::KL_AddTemp(CString strFrame)
{
	m_asFrames.Add(strFrame);

	return 0;
}
INT CBL_LOC_LOAN_PROC::KL_EndTemp(CString strFirstWork)
{
	if(0 < m_asFrames.GetSize())
	{
		CFrameSender Sender(m_pParentMgr->m_pInfo);
		Sender.SendTempTbl(m_asFrames, strFirstWork);
	}

	return 0;
}


BOOL CBL_LOC_LOAN_PROC::IsNumericText(CString strValue)
{
	INT nLength = strValue.GetLength();
	if(1 > nLength) return FALSE;
	BOOL bCheck = FALSE;
	for(INT i = 0; i < strValue.GetLength(); i++)
	{
		TCHAR cValue = strValue.Mid(i, 1).GetAt(0);
		if(cValue >= '0' && cValue <= '9')
		{
			bCheck = TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return bCheck;
}

INT CBL_LOC_LOAN_PROC::KL_SearchLoanHistoryInfo()
{
	if(0 == IsSearchedUser())
	{
		CString strKLUserYN;
		CESL_DataMgr* pUserDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(pUserDM)
		{
			pUserDM->GetCellData(_T("����ȸ������"), 0, strKLUserYN);
			if(_T("Y") != strKLUserYN)
			{
				SelfCloseMessageBox(_T("����ȸ������ ��ȯ�� ȸ���� �˻��� �� �ֽ��ϴ�."));
				return 1000;
			}

			CKL_SearchLoanInfoDlg Dlg(m_pParentMgr);
			Dlg.DoModal();
		}
	}

	return 0;
}

VOID CBL_LOC_LOAN_PROC::Send3000ApiSocket(CString strType, CString strMsg/*=_T("NULL")*/)
{
	if(NULL != m_lp3000ApiSocket)
	{
		m_lp3000ApiSocket(strType, strMsg);
	}
}
INT CBL_LOC_LOAN_PROC::KL_SearchLoanInfo(INT nMode, CString &strErrorMsg)
{
EFS_BEGIN

	INT	ids;
	CString strFuncName = _T("KL_SearchLoanInfo");

	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(1 > pDM->GetRowCount()) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString strCommand, strType;
	if(0 == nMode)
	{
		ids = SearchLoanBookInfo();
		if(0 > ids) return ids;
		return 0;
	}
	else if(1 == nMode)
	{	
		strCommand = _T("IL_K09_LOC_B01_SERVICE");
		strType = _T("����");
	}
	else if(2 == nMode)
	{	
		strCommand = _T("IL_K04_LOC_B01_SERVICE");
		strType = _T("����");

		
		ids = KL_SearchLoanInfo(1, strErrorMsg);
		if(0 != ids) return ids;
	}
	else
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	CString strIsUnityUser;
	pDM->GetCellData(_T("����ȸ������"), 0, strIsUnityUser);
	if(_T("Y") != strIsUnityUser) return 100;

	
	
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pLoanDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	CString strData;
	INT nRowCnt = pLoanDM->GetRowCount();
	for(INT nRow = nRowCnt; nRow >= 0; nRow--)
	{
		ids = pLoanDM->GetCellData(_T("��Ȳ����"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(
			(1 == nMode && (_T("����") == strData || _T("����") == strData)) ||
			(2 == nMode && _T("����") == strData)
			)
		{
			pLoanDM->DeleteRow(nRow);
		}
	}
	
	CString strUserNo;
	ids = pDM->GetCellData(_T("�����ڹ�ȣ"), 0, strUserNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CString strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;

	CFrameWriter Writer(m_pParentMgr->m_pInfo);
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
	
	CString strGpinHash;
	ids = pDM->GetCellData(_T("G-PINHASH�����ڵ�"), 0, strGpinHash);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	Writer.addElement(_T("G-PINHASH�����ڵ�"), strGpinHash);
	
	
	if((1 == nMode) && _T("N") == m_strLocalFreeYN)
	{	
		
		Writer.addElement(_T("��������ȣ"), _T("000000"));
	}
	else
	{
		Writer.addElement(_T("��������ȣ"), strLibCode);
	}

	CFrameReader Reader;
	CFrameSender Sender(m_pParentMgr->m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) return ids;
	
	CESL_DataMgr TmpDM;
	ids = Reader.makeDM(&TmpDM);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strMsg,strTemp;
		TmpDM.GetCellData(0, 0, strTemp);
		strErrorMsg.Format(_T("�̵����� ������ �߻��Ͽ����ϴ�."));
		if(1 == nMode)
		{
			strMsg.Format(_T("�������ʹ�����Ȳ��ȸ(9) ERROR CODE : %s"), strTemp);
		}
		else
		{
			strMsg.Format(_T("���ռ��ʹ�����Ȳ��ȸ(4) ERROR CODE : %s"), strTemp);
		}
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return 200;
	}

	if((1 == nMode) && _T("N") == m_strLocalFreeYN)
	{
		CString strKey1, strKey2;
		CString strLibCode;
		CString strPaymentKey;
		
		if(0 < TmpDM.GetRowCount())
		{
			INT nRowCnt = pLoanDM->GetRowCount();
			for(INT nRow = 0; nRow < nRowCnt; nRow++)
			{
				ids = pLoanDM->GetCellData(_T("��Ȳ����"), nRow, strData);
				if(_T("����") == strData && _T("����") == strData)
				{
					continue;
				}
				
				pLoanDM->GetCellData(_T("����KEY"), nRow, strKey1);				
				if(!strKey1.IsEmpty())
				{
					for(INT nRow2 = 0; nRow2 < TmpDM.GetRowCount(); nRow2++)
					{
						TmpDM.GetCellData(_T("����KEY"), nRow2, strKey2);
						if(strKey1 == strKey2)
						{
							TmpDM.GetCellData(_T("���⵵������ȣ"), nRow2, strLibCode);
							if(!strLibCode.IsEmpty())
							{
								if(-1 < m_pManageValue->m_strLibCodeList.Find(strLibCode))
								{
									TmpDM.GetCellData(_T("����ó��KEY"), nRow, strPaymentKey); 							
									pLoanDM->SetCellData(_T("����ó��KEY"), strPaymentKey, nRow);
									continue;
								}
							}
						}
					}
				}
			}
			
			nRowCnt = TmpDM.GetRowCount();
			for(nRow = nRowCnt; nRow >= 0; nRow--)
			{
				TmpDM.GetCellData(_T("���⵵������ȣ"), nRow, strData);
				if(!strData.IsEmpty())
				{
					if(-1 < m_pManageValue->m_strLibCodeList.Find(strData))
					{
						TmpDM.DeleteRow(nRow);
					}
				}
			}	
		}
	}

	if(1 > TmpDM.GetRowCount())
	{
		return 0;
	}

	INT nInsertRow = 0;
	nRowCnt = TmpDM.GetRowCount();
	for(nRow = 0; nRow < nRowCnt; nRow++)
	{
		pLoanDM->InsertRow(-1);
		nInsertRow = pLoanDM->GetRowCount() - 1;

		ids = TmpDM.GetCellData(_T("���⵵������ȣ"), nRow, strData);

		ids = pLoanDM->SetCellData(_T("���⵵������ȣ"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("������"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("������"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("�ݳ�������"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("�ݳ�������"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("������"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("������"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("���ุ����"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("���ุ����"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("����"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("�������"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("��Ϲ�ȣ"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("��Ϲ�ȣ"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("����"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("��ǥ��"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("û����ȣ"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("û����ȣ"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = pLoanDM->SetCellData(_T("��Ȳ����"), strType, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

 		ids = TmpDM.GetCellData(_T("����ó��KEY"), nRow, strData);
 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
 		ids = pLoanDM->SetCellData(_T("����ó��KEY"), strData, nInsertRow);
 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("���⵵����"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("���⵵����"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("����KEY"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("����KEY"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		
		TmpDM.GetCellData(_T("�ݳ�������������ȣ"), nRow, strData);
		ids = pLoanDM->SetCellData(_T("�ݳ�������������ȣ"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		TmpDM.GetCellData(_T("�ݳ�������������"), nRow, strData);
		ids = pLoanDM->SetCellData(_T("�ݳ�������������"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::KL_SearchLoanInfo(CString &strErrorMsg)
{
EFS_BEGIN

	INT	ids;
	CString strFuncName = _T("KL_SearchLoanInfo");

	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(1 > pDM->GetRowCount()) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CString strIsUnityUser;
	pDM->GetCellData(_T("����ȸ������"), 0, strIsUnityUser);
	if(_T("Y") != strIsUnityUser) return 100;
	
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pLoanDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CString strData;
	INT nRowCnt = pLoanDM->GetRowCount();
	for(INT i = nRowCnt; i >= 0; i--)
	{
		ids = pLoanDM->GetCellData(_T("��Ȳ����"), i, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("����") == strData || _T("����") == strData)
		{
			pLoanDM->DeleteRow(i);
		}
	}	
	
	CFrameWriter Writer(m_pParentMgr->m_pInfo);
	Writer.setCommand(_T("IL_K42_LOC_B01_SERVICE"));
	Writer.addRecord();
	
	CString strUserNo;
	pDM->GetCellData(_T("�����ڹ�ȣ"), 0, strUserNo);
 	if(strUserNo.IsEmpty())
 
 	{
 		strErrorMsg.Format(_T("�����ڹ�ȣ�� �����ϴ�."));
 		return 200;
 	}
	Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);

	CString strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	Writer.addElement(_T("��������ȣ"), strLibCode);
	Writer.addElement(_T("�������ù�񹫷�"), m_strLocalFreeYN);
	
	CFrameReader Reader;
	CFrameSender Sender(m_pParentMgr->m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		strErrorMsg.Format(_T("�̵���� ������ �����Ͽ����ϴ�."));
		return 200;
	}
	
	CESL_DataMgr TmpDM;
	ids = Reader.makeDM(&TmpDM);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strMsg, strTemp;
		strTemp = Reader.getElement(0, 0);
		strErrorMsg.Format(_T("���� ����(%s)"), strTemp);
		strMsg.Format(_T("������Ȳ��ȸ(42) ERROR CODE : %s"), strTemp);
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return 200;
	}
	if(1 > TmpDM.GetRowCount())
	{
		return 0;
	}
	
	if(_T("N") == m_strLocalFreeYN && 0 < TmpDM.GetRowCount())
	{
		CString strKey1, strKey2, strLibCode, strPaymentKey;
		INT nRowCnt = pLoanDM->GetRowCount();
		for(INT i = 0; i < nRowCnt; i++)
		{
			ids = pLoanDM->GetCellData(_T("��Ȳ����"), i, strData);
			if(_T("����") == strData && _T("����") == strData) continue;
			pLoanDM->GetCellData(_T("����KEY"), i, strKey1);
			if(strKey1.IsEmpty()) continue;
			for(INT j = 0; j < TmpDM.GetRowCount(); j++)
			{
				TmpDM.GetCellData(_T("����KEY"), j, strKey2);
				if(strKey1 == strKey2)
				{
					TmpDM.GetCellData(_T("����ó��KEY"), i, strPaymentKey);
					if(strPaymentKey.IsEmpty()) break;
					TmpDM.GetCellData(_T("���⵵������ȣ"), j, strLibCode);
					if(strLibCode.IsEmpty()) break;
					if(-1 < m_pManageValue->m_strLibCodeList.Find(strLibCode))
					{	
						pLoanDM->SetCellData(_T("����ó��KEY"), strPaymentKey, i);
					}
					break;
				}
			}
		}
		
		nRowCnt = TmpDM.GetRowCount();
		for(i = nRowCnt; i >= 0; i--)
		{
			TmpDM.GetCellData(_T("���⵵������ȣ"), i, strData);
			if(strData.IsEmpty()) continue;			
			if(-1 < m_pManageValue->m_strLibCodeList.Find(strData))
			{
				TmpDM.DeleteRow(i);
			}
		}
	}
	
	INT nInsertRow = 0;
	nRowCnt = TmpDM.GetRowCount();
	for(i = 0; i < nRowCnt; i++)
	{
		
		if(0 == i)
		{
			TmpDM.GetCellData(_T("������������������"), i, strData);
			if(_T("Y") == strData)
			{
				pDM->SetCellData(_T("������������"), strData, 0);
			}
			else
			{
				pDM->SetCellData(_T("������������"), _T("N"), 0);
			}
			TmpDM.GetCellData(_T("����KEY"), i, strData);
			
			
			CString strData2;
			TmpDM.GetCellData(_T("Ÿ������KEY"), i, strData2);
			if(0 == strData2.GetLength() && strData.IsEmpty()) break;
			
		}
		
		pLoanDM->InsertRow(-1);
		nInsertRow = pLoanDM->GetRowCount() - 1;
		TmpDM.GetCellData(_T("���⵵������ȣ"), i, strData);
		pLoanDM->SetCellData(_T("���⵵������ȣ"), strData, nInsertRow);
		TmpDM.GetCellData(_T("������"), i, strData);
		pLoanDM->SetCellData(_T("������"), strData, nInsertRow);
		TmpDM.GetCellData(_T("�ݳ�������"), i, strData);
		
		if ( strData.GetLength() > 10 ) strData = strData.Left(10);
		pLoanDM->SetCellData(_T("�ݳ�������"), strData, nInsertRow);
		TmpDM.GetCellData(_T("������"), i, strData);
		pLoanDM->SetCellData(_T("������"), strData, nInsertRow);
		TmpDM.GetCellData(_T("���ุ����"), i, strData);
		pLoanDM->SetCellData(_T("���ุ����"), strData, nInsertRow);
		TmpDM.GetCellData(_T("����"), i, strData);
		pLoanDM->SetCellData(_T("�������"), strData, nInsertRow);
		TmpDM.GetCellData(_T("��Ϲ�ȣ"), i, strData);
		pLoanDM->SetCellData(_T("��Ϲ�ȣ"), strData, nInsertRow);
		TmpDM.GetCellData(_T("����"), i, strData);
		pLoanDM->SetCellData(_T("��ǥ��"), strData, nInsertRow);
		TmpDM.GetCellData(_T("û����ȣ"), i, strData);
		pLoanDM->SetCellData(_T("û����ȣ"), strData, nInsertRow);
		TmpDM.GetCellData(_T("��Ȳ����"), i, strData);
		pLoanDM->SetCellData(_T("��Ȳ����"), strData, nInsertRow);
 		TmpDM.GetCellData(_T("����ó��KEY"), i, strData);
 		pLoanDM->SetCellData(_T("����ó��KEY"), strData, nInsertRow);
		TmpDM.GetCellData(_T("���⵵����"), i, strData);
		pLoanDM->SetCellData(_T("���⵵����"), strData, nInsertRow);
		TmpDM.GetCellData(_T("����KEY"), i, strData);
		pLoanDM->SetCellData(_T("����KEY"), strData, nInsertRow);
		TmpDM.GetCellData(_T("�ݳ�������������ȣ"), i, strData);
		pLoanDM->SetCellData(_T("�ݳ�������������ȣ"), strData, nInsertRow);
		TmpDM.GetCellData(_T("�ݳ�������������"), i, strData);
		pLoanDM->SetCellData(_T("�ݳ�������������"), strData, nInsertRow);
		
		
		TmpDM.GetCellData(_T("�ηϿ���"), i, strData);
		if(strData.IsEmpty()) strData = _T("B");
		pLoanDM->SetCellData(_T("�ηϿ���"), strData, nInsertRow);
		if(_T("A") == strData)
		{
			strData = _T("�η�");
		}
		else
		{
			strData = _T("å");
		}
		pLoanDM->SetCellData(_T("�ηϿ��μ���"), strData, nInsertRow);
		
		TmpDM.GetCellData(_T("��������ȣ��������"), i, strData);
		if(strData.IsEmpty()) strData = _T("N");
		pLoanDM->SetCellData(_T("��������ȣ��������"), strData, nInsertRow);

		
		TmpDM.GetCellData(_T("����������"), i, strData);
		pLoanDM->SetCellData(_T("����������"), strData, nInsertRow);
		TmpDM.GetCellData(_T("������������ȣ"), i, strData);
		pLoanDM->SetCellData(_T("������������ȣ"), strData, nInsertRow);
		
		pLoanDM->SetCellData(_T("�������������ȣ"), strData, nInsertRow);
		TmpDM.GetCellData(_T("Ÿ������KEY"), i, strData);
		pLoanDM->SetCellData(_T("Ÿ������KEY"), strData, nInsertRow);
		if(!strData.IsEmpty())
		{
			pLoanDM->SetCellData(_T("LCT_KEY"), strData, nInsertRow);

			TmpDM.GetCellData(_T("����"), i, strData);
			pLoanDM->SetCellData(_T("Ʈ����ǻ���"), strData, nInsertRow);
			if(_T("4") == strData)
			{
				pLoanDM->SetCellData(_T("�������"), _T("0"), nInsertRow);
			}
			else
			{
				pLoanDM->SetCellData(_T("�������"), _T(""), nInsertRow);
			}
		}
		
		TmpDM.GetCellData(_T("�ηϹ߼�����"), i, strData);
		pLoanDM->SetCellData(_T("�ηϹ߼�����"), strData, nInsertRow);
		TmpDM.GetCellData(_T("�ηϵ�Ϲ�ȣ����Ʈ"), i, strData);
		pLoanDM->SetCellData(_T("�ηϵ�Ϲ�ȣ����Ʈ"), strData, nInsertRow);
		TmpDM.GetCellData(_T("���⸸����"), i, strData);
		pLoanDM->SetCellData(_T("���⸸����"), strData, nInsertRow);
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationStatus(CString sGetData[])
{	
	if(!(m_pManageValue->m_bVolReserve))
	{ 
		CBO_LOC_3170 *Dlg = new CBO_LOC_3170(m_pParentMgr, m_pLoanShareManager, m_pManageValue, 0);
		Dlg->pMain = m_pParentMgr->pMain;
		Dlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
		Dlg->m_pInfo = m_pParentMgr->m_pInfo;
		Dlg->SetInfo(sGetData[3], sGetData[4], sGetData[0], sGetData[1]);
		
		POSITION pos;
		pos = m_obj3170DlgList->AddTail((CObject*)Dlg);
		if(Dlg->Create(m_pParentMgr))
		{
			Dlg->CenterWindow();
			Dlg->ShowWindow(SW_SHOWNOACTIVATE);
			Dlg->UpdateWindow();				
		}
		else
		{
			m_obj3170DlgList->RemoveAt(pos);
		}
	}
	else 
	{ 
		CBO_LOC_3170_ReserveInfo *Dlg = new CBO_LOC_3170_ReserveInfo(m_pParentMgr, m_pLoanShareManager, m_pManageValue, 0);
		Dlg->pMain = m_pParentMgr->pMain;
		Dlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
		Dlg->m_pInfo = m_pParentMgr->m_pInfo;
		Dlg->SetInfo(sGetData[3], sGetData[4], sGetData[0], _T(""));
		
		
		POSITION pos;
		pos = m_obj3170DlgList->AddTail((CObject*)Dlg);
		if(Dlg->Create(m_pParentMgr))
		{
			Dlg->CenterWindow();
			Dlg->ShowWindow(SW_SHOWNOACTIVATE);
			Dlg->UpdateWindow();				
		}
		else
		{
			m_obj3170DlgList->RemoveAt(pos);
		}
	}

	return 0;
}

INT CBL_LOC_LOAN_PROC::ReservationStatus()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReservationStatus");

	INT			nIndex		=	-1;				
	BOOL		bEdit		=	FALSE;			
	CString		strStatus	=	_T("");			
	CString		sDMAlias	=	_T("DM_BO_LOC_3100_LOAN_INFO");	

	
	CESL_DataMgr	*pDM	=	m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_ControlMgr	*pCM	=	m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_Grid	*pGrid		=	(CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	pGrid->SelectMakeList();
	
	INT	nRowCount	=	pGrid->SelectGetCount();
	
	pGrid->SelectGetTailPosition();
	
	if(0	>=	nRowCount)
	{
		SelfCloseMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return	1;
	}
	
	if(1	<	nRowCount)
	{
		SelfCloseMessageBox(_T("�� ���� �ڷḸ �����Ͽ� �ֽʽÿ�."));
		return	2;
	}

	
	nIndex = pGrid->SelectGetIdx();
	if(0	>	nIndex) 
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, strFuncName);
	}

	CString sType;
	m_pParentMgr->GetDataMgrData(sDMAlias, _T("��Ȳ����"), sType, nIndex);
	if(_T("����") == sType || _T("����") == sType)
	{
		SelfCloseMessageBox(sType + _T("��Ȳ�� ������¸� Ȯ���� �� �����ϴ�."));
		return 3;
	}

	
	TCHAR *sFieldAlias[5] = 
	{
		 _T("����/���ӱ���"), _T("åKEY"), _T("�����̿��ڼ�"), _T("��KEY"), _T("��ȣ����KEY")
	};
	CString sGetData[5];

	for(INT i = 0 ; i < 5 ; i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, sFieldAlias[i], sGetData[i], nIndex);
		if(0 > ids) return 5;
	}

	ReservationStatus(sGetData);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SetBookStatusCnt_Unity()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SetBookStatusCnt_Unity");

	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pLoanDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr* pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pMemberDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT		nLoanCount		= 0; 
	INT		nUnityLoanCount	= 0; 
	INT		nTLLoanCount	= 0; 
	INT		nReserveCnt		= 0; 
	CString sAppendixYN		= _T(""); 
	CString sStatus			= _T(""); 
	CString sManageCode		= _T(""); 
	CString sUnmennedYN		= _T(""); 
	CString sL_Device		= _T(""); 
	CString sType			= _T("");	
	CString strHoldLibCode	= _T("");	
	const int nRow = pLoanDM->GetRowCount();
	for(INT i = 0; i < nRow; i++)
	{
		ids = pLoanDM->GetCellData(_T("�������"), i, sStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("�ηϿ���"), i, sAppendixYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("��������"), i, sManageCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("���δ��⿹�����"), i, sUnmennedYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("L_DEVICE"), i, sL_Device);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			ids = pLoanDM->GetCellData(_T("��Ȳ����"), i, sType);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
		else
		{
			sType = _T("����");
		}
		if((_T("0") == sStatus || _T("2") == sStatus || _T("L") == sStatus || _T("D") == sStatus) 
			&& m_pParentMgr->m_pInfo->MANAGE_CODE == sManageCode && _T("KOLASIII-�̵�������") != sL_Device && _T("����") == sType)
		{
			if(_T("A") == sAppendixYN) 
			{ 
				if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
				{
					nLoanCount++;
				}
			}
			else 
			{
				nLoanCount++;	
			}
		}
		
		CString strLILLLoanKey;
		pLoanDM->GetCellData(_T("Ÿ������KEY"), i, strLILLLoanKey);

		if(!strLILLLoanKey.IsEmpty() && sType != _T("����"))
		{
			nUnityLoanCount++;
		
			pLoanDM->GetCellData(_T("������������ȣ"), i, strHoldLibCode);			
			if(m_pParentMgr->m_pInfo->LIB_CODE == strHoldLibCode)
			{
				if(_T("A") == sAppendixYN) 
				{ 
					
					if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
					{
						nLoanCount++;
					}
				}
				else 
				{
					nLoanCount++;	
				}
			}					
		}
		else
		{
			if((_T("0") == sStatus || _T("2") == sStatus) && _T("KOLASIII-�̵�������") != sL_Device)
			{
				if(_T("A") == sAppendixYN) 
				{ 
					
					if(FALSE == m_pParentMgr->m_pInfo->m_bKLUse)
					{
						if(m_pParentMgr->m_pInfo->MANAGE_CODE == sManageCode)
						{
							if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
							{
								nUnityLoanCount++;
							}
						}
					}
				}
				else 
				{
					nUnityLoanCount++;	
				}
			}
		}
				
		if((_T("0") == sStatus || _T("2") == sStatus || _T("L") == sStatus || _T("D") == sStatus) 
			&& m_pParentMgr->m_pInfo->MANAGE_CODE == sManageCode && _T("KOLASIII-�̵�������") == sL_Device && _T("����") == sType)
		{
			if(_T("A") == sAppendixYN) 
			{ 
				if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
				{
					nTLLoanCount++;
				}
			}
			else 
			{
				nTLLoanCount++;	
			}
		}
		if(_T("3") == sStatus && sUnmennedYN.IsEmpty())
		{ 
			nReserveCnt++;
		}
	}

	CString sLoanCount, sUserKey;
	pMemberDM->GetCellData(_T("������KEY"), 0, sUserKey);
	pMemberDM->GetCellData(_T("�Ѵ���å��"), 0, sLoanCount); 
	
	INT nTotalLoanCount = nUnityLoanCount;
	if(_ttoi(sLoanCount) != nTotalLoanCount)
	{
		sLoanCount.Format(_T("%d"), nTotalLoanCount); 
		pMemberDM->StartFrame();
		pMemberDM->InitDBFieldData();
		
		if(1 > _ttoi(sLoanCount))
		{
			sLoanCount = _T("0");
		}
		pMemberDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sLoanCount, TRUE);
		pMemberDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("����å��UPDATE"), __WFILE__, __LINE__), TRUE);
		pMemberDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
		pMemberDM->SendFrame();
		pMemberDM->EndFrame();
		
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sUserKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_COUNT")));
		ShareInfoCenterUpload(_T("N"), _T("U"));
	}

	INT nMaxLoanBookCnt;
	ids = m_pCheckLoan->GetIndividualLoanBookCnt(nMaxLoanBookCnt);
	if(0 > ids) return ids;
	
	CString sViewLoanBookCnt;
	sViewLoanBookCnt.Format(_T("%d/%d"), nLoanCount, nMaxLoanBookCnt);
	
	sLoanCount.Format(_T("%d"), nLoanCount);
	
	CString sIsLoanBookCnt;
	sIsLoanBookCnt.Format(_T("%d"), nMaxLoanBookCnt - nLoanCount);
	
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�Ѵ���å��"), sLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����å������"), sViewLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���Ⱑ��å��"), sIsLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	
	nMaxLoanBookCnt = 0;
	
	CString strKLUserYN;
	pMemberDM->GetCellData(_T("����ȸ������"), 0, strKLUserYN);
	if(_T("Y") != strKLUserYN) strKLUserYN = _T("N");
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN && 10000 != m_pParentMgr->m_pInfo->MODE)
	{
		if(m_strMaxLoanCount.IsEmpty())
		{
		
			CString strGetData = _T("");
			GetTempData(_T("�⺻���մ��Ⱑ�ɱǼ�"), strGetData);
			if(strGetData.IsEmpty())
			{
				m_strMaxLoanCount = _T("20");
			}
			else
			{
				m_strMaxLoanCount = strGetData;
			}
		}
		nMaxLoanBookCnt = _ttoi(m_strMaxLoanCount);
	}
	else
	{
		ids = m_pCheckLoan->GetUnityLoanBookCnt(nMaxLoanBookCnt);
		if(0 > ids) return ids;
	}
	
	sViewLoanBookCnt = _T("");
	sViewLoanBookCnt.Format(_T("%d/%d"), nUnityLoanCount, nMaxLoanBookCnt);
	
	sLoanCount.Format(_T("%d"), nUnityLoanCount);
	
	sIsLoanBookCnt = _T("");
	sIsLoanBookCnt.Format(_T("%d"), nMaxLoanBookCnt - nUnityLoanCount);
	
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����Ѵ���å��"), sLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���մ���å������"), sViewLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���մ��Ⱑ��å��"), sIsLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);	
	
	INT nMaxTLLoanBookCnt;
	ids = m_pCheckLoan->GetTLLoanBookCnt(nMaxTLLoanBookCnt);
	if(0 > ids) return ids;
	
	CString sViewTLLoanBookCnt;
	
	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
		sViewTLLoanBookCnt.Format(_T("%d/%d"), nTLLoanCount, nMaxTLLoanBookCnt);
	}
	else
	{
		sViewTLLoanBookCnt.Format(_T("%d"), nTLLoanCount);
	}

	CString sIsTLLoanBookCnt;
	sIsTLLoanBookCnt.Format(_T("%d"), nMaxTLLoanBookCnt - nTLLoanCount);
	sLoanCount.Format(_T("%d"), nTLLoanCount);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�̵��Ѵ���å��"), sLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�̵�����å������"), sViewTLLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�̵����Ⱑ��å��"), sIsTLLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	CString sReserveCnt;
	pMemberDM->GetCellData(_T("�ѿ���å��"), 0, sReserveCnt); 
	
	if(_ttoi(sReserveCnt) != nReserveCnt)
	{
		sReserveCnt.Format(_T("%d"), nReserveCnt); 
		pMemberDM->StartFrame();
		pMemberDM->InitDBFieldData();
		pMemberDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sReserveCnt, TRUE);
		pMemberDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("����å��UPDATE"), __WFILE__, __LINE__), TRUE);
		pMemberDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
		pMemberDM->SendFrame();
		pMemberDM->EndFrame();
		
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sUserKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("RESERVATION_COUNT")));
		ShareInfoCenterUpload(_T("N"), _T("U"));
	}
	
	INT nMaxReservationBookCnt;
	ids = m_pCheckLoan->GetReserveBookCnt(nMaxReservationBookCnt);
	if(0 > ids) return ids;
	
	CString sViewReserveBookCnt;
	sViewReserveBookCnt.Format(_T("%d/%d"), nReserveCnt, nMaxReservationBookCnt);
	
	CString sIsReserveBookCnt;
	sIsReserveBookCnt.Format(_T("%d"), nMaxReservationBookCnt - nReserveCnt);

	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�ѿ���å��"), sReserveCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����å������"), sViewReserveBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���డ��å��"), sIsReserveBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);


	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SetBookStatusCnt()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SetBookStatusCnt");

	CESL_DataMgr *pLoanDM  = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pLoanDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr* pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pMemberDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT		nLoanCount		= 0; 
	INT		nOtherLoanCount	= 0; 
	INT		nTLLoanCount	= 0; 
	INT		nReserveCnt		= 0; 
	CString sAppendixYN		= _T(""); 
	CString sStatus			= _T(""); 
 	CString sManageCode		= _T(""); 
	CString sUserManageCode	= _T(""); 
	CString sUnmennedYN		= _T(""); 
	CString sL_Device		= _T(""); 
	CString sType			= _T("");

	
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("��������"), sUserManageCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);


	const int nRow = pLoanDM->GetRowCount();
	for(INT i = 0; i < nRow; i++)
	{
		ids = pLoanDM->GetCellData(_T("�������"), i, sStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("�ηϿ���"), i, sAppendixYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
 		ids = pLoanDM->GetCellData(_T("��������"), i, sManageCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("���δ��⿹�����"), i, sUnmennedYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("L_DEVICE"), i, sL_Device);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if((_T("0") == sStatus || _T("2") == sStatus) && sUserManageCode == sManageCode && _T("KOLASIII-�̵�������") != sL_Device)
		{
			if(_T("A") == sAppendixYN) 
			{ 
				if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
				{
					nLoanCount++;
				}
			}
			else 
			{
				nLoanCount++;	
			}
		}
		
		if(((_T("0") == sStatus || _T("2") == sStatus) && sUserManageCode != sManageCode) ||
			(_T("L") == sStatus || _T("D") == sStatus) && _T("KOLASIII-�̵�������") != sL_Device)
		{
			if(_T("A") == sAppendixYN) 
			{ 
				if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
				{
					nOtherLoanCount++;
				}
			}
			else 
			{
				nOtherLoanCount++;	
			}
		}

		if((_T("0") == sStatus || _T("2") == sStatus) && sUserManageCode == sManageCode && _T("KOLASIII-�̵�������") == sL_Device)
		{
			if(_T("A") == sAppendixYN) 
			{ 
				if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
				{
					nTLLoanCount++;
				}
			}
			else 
			{
				nTLLoanCount++;	
			}
		}
		
		if(_T("3") == sStatus && sUnmennedYN.IsEmpty())
		{ 
			nReserveCnt++;
		}
	}

	CString sLoanCount, sUserKey;
	pMemberDM->GetCellData(_T("������KEY"), 0, sUserKey);
	pMemberDM->GetCellData(_T("�Ѵ���å��"), 0, sLoanCount); 
	
	INT nTotalLoanCount = nLoanCount + nOtherLoanCount;
	if(_ttoi(sLoanCount) != nTotalLoanCount)
	{
		sLoanCount.Format(_T("%d"), nTotalLoanCount); 
		pMemberDM->StartFrame();
		pMemberDM->InitDBFieldData();
		
		if(1 > _ttoi(sLoanCount))
		{
			sLoanCount = _T("0");
		}
		pMemberDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sLoanCount, TRUE);
		pMemberDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("����å��UPDATE"), __WFILE__, __LINE__), TRUE);
		pMemberDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
		pMemberDM->SendFrame();
		pMemberDM->EndFrame();
		
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sUserKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_COUNT")));
		ShareInfoCenterUpload(_T("N"), _T("U"));
	}

	INT nMaxLoanBookCnt;
	ids = m_pCheckLoan->GetLoanBookCnt(nMaxLoanBookCnt);
	if(0 > ids) return ids;
	
	CString sViewLoanBookCnt;
	sViewLoanBookCnt.Format(_T("%d/%d"), nLoanCount, nMaxLoanBookCnt);
	
	CString sIsLoanBookCnt;
	sIsLoanBookCnt.Format(_T("%d"), nMaxLoanBookCnt - nLoanCount);
	sLoanCount.Format(_T("%d"), nLoanCount);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�Ѵ���å��"), sLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����å������"), sViewLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���Ⱑ��å��"), sIsLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);	
	
	INT nMaxOtherLoanBookCnt;	
	ids = m_pCheckLoan->GetOtherLoanBookCnt(nMaxOtherLoanBookCnt);
	if(0 > ids) return ids;
	
	CString sViewOtherLoanBookCnt;
	sViewOtherLoanBookCnt.Format(_T("%d/%d"), nOtherLoanCount, nMaxOtherLoanBookCnt);
	
	CString sIsOtherLoanBookCnt;
	sIsOtherLoanBookCnt.Format(_T("%d"), nMaxOtherLoanBookCnt - nOtherLoanCount);
	CString sOtherLoanCount;
	sOtherLoanCount.Format(_T("%d"), nOtherLoanCount);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("Ÿ���Ѵ���å��"), sOtherLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("Ÿ������å������"), sViewOtherLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("Ÿ�����Ⱑ��å��"), sIsOtherLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	INT nMaxTLLoanBookCnt;
	ids = m_pCheckLoan->GetTLLoanBookCnt(nMaxTLLoanBookCnt);
	if(0 > ids) return ids;
	
	CString sViewTLLoanBookCnt;
	
	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
		sViewTLLoanBookCnt.Format(_T("%d/%d"), nTLLoanCount, nMaxTLLoanBookCnt);
	}
	else
	{
		sViewTLLoanBookCnt.Format(_T("%d"), nTLLoanCount);
	}	
	
	CString sIsTLLoanBookCnt;
	sIsTLLoanBookCnt.Format(_T("%d"), nMaxTLLoanBookCnt - nTLLoanCount);
	CString sTLLoanCount;
	sTLLoanCount.Format(_T("%d"), nTLLoanCount);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�̵��Ѵ���å��"), sTLLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�̵�����å������"), sViewTLLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�̵����Ⱑ��å��"), sIsTLLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);

	CString sReserveCnt;
	pMemberDM->GetCellData(_T("�ѿ���å��"), 0, sReserveCnt); 
	
	if(_ttoi(sReserveCnt) != nReserveCnt)
	{
		sReserveCnt.Format(_T("%d"), nReserveCnt); 
		pMemberDM->StartFrame();
		pMemberDM->InitDBFieldData();
		pMemberDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sReserveCnt, TRUE);
		pMemberDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("����å��UPDATE"), __WFILE__, __LINE__), TRUE);
		pMemberDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
		pMemberDM->SendFrame();
		pMemberDM->EndFrame();
		
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sUserKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("RESERVATION_COUNT")));
		ShareInfoCenterUpload(_T("N"), _T("U"));
	}
	
	INT nMaxReservationBookCnt;
	ids = m_pCheckLoan->GetReserveBookCnt(nMaxReservationBookCnt);
	if(0 > ids) return ids;
	
	CString sViewReserveBookCnt;
	sViewReserveBookCnt.Format(_T("%d/%d"), nReserveCnt, nMaxReservationBookCnt);
	
	CString sIsReserveBookCnt;
	sIsReserveBookCnt.Format(_T("%d"), nMaxReservationBookCnt - nReserveCnt);

	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�ѿ���å��"), sReserveCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����å������"), sViewReserveBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("���డ��å��"), sIsReserveBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);


	return 0;

EFS_END
return -1;
}

VOID CBL_LOC_LOAN_PROC::SetBEGIN_SBL()
{
	BEGIN_SBL();
}

VOID CBL_LOC_LOAN_PROC::SetEND_SBL(INT nRowCount)
{
	END_SBL(nRowCount);
}
INT CBL_LOC_LOAN_PROC::GetBookManageCode(CString &strBookManageCode)
{
	CString strValue;
	m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("��ϱ���"), _T("�������� ��ϱ��� �ߺ�����"), strValue);
	if(_T("N") == strValue || TRUE == strValue.IsEmpty())
	{
		strBookManageCode = m_pParentMgr->m_pInfo->MANAGE_CODE;
		return 0;
	}
	else
	{
		INT ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("å��������"), strBookManageCode);	
		return ids;
	}
	return 0;
}
INT CBL_LOC_LOAN_PROC::AddIdxQueryFrame(CESL_DataMgr *pDM, INT type, CString sSpeciesKey)
{
	EFS_BEGIN	

 	return 0;
	
	if((m_pParentMgr->m_pInfo->MODE == 10000) || (m_pParentMgr->m_pInfo->MODE == 30000))
		return 0;

	CString sIDXTblName, sProcName;
	switch(type)
	{
		case 1 :
			sIDXTblName = _T("IDX_BO_TBL");			
			sProcName = _T("TESTWORKSTATUS");
			break;
		case 2 :
			sIDXTblName = _T("IDX_SE_TBL");			
			sProcName = _T("TESTSEWORKSTATUS");
			break;
		default :
			return -1;
			break;
	}
	

	CString sQuery;
	sQuery.Format(_T("UPDATE %s SET WORKING_STATUS = %s(%s) WHERE REC_KEY = %s;"),sIDXTblName, sProcName, sSpeciesKey, sSpeciesKey);	
	pDM->AddFrame(sQuery);

	return 0;

	EFS_END

	return -1;
}


CString CBL_LOC_LOAN_PROC::getLimitWord(TCHAR* szToken, INT nCutCnt, BOOL bIsAddSpace)
{
	
	
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte(szToken, &szTmp, &len);
	if(0 < len)
	{


	}
	if(szTmp == NULL)
	{
		CString sTemp = _T("");
		for(INT i = 0; i < nCutCnt; i++)
		{
			sTemp += _T(" ");
		}
		return sTemp;
	}

	BOOL bIsCut = FALSE;

	INT nTokenLen = 0 ;
	INT nCompIndexCnt = 0 ;

	CString sReturnWord = _T("");
	CString sPeriods = _T("��");
	CHAR szPeriods[5];
	sprintf(szPeriods, "%s", "��");

	CHAR chComp = '\0' ;
	CHAR chExtra = '\0' ;

	CHAR szTmpWord[3] ; 
	CHAR szReturnWord[100] = "" ; 	

	nTokenLen = strlen(szTmp);

	if(nTokenLen > nCutCnt)
	{
		bIsCut = TRUE;
		nCutCnt -= 3;
	}

	for (INT i = 0 ; i < nCutCnt ; i++)
	{
		if(i > (nTokenLen-1))
		{
			if(bIsAddSpace)
			{
				chComp = ' ';
				sprintf (szTmpWord, "%c", chComp);
				strcat (szReturnWord, szTmpWord);
				continue;
			}
			else
				break;
		}
		else
		{
			chComp = szTmp[i];
			chExtra = szTmp[i + 1];
		}

		
		if(chComp & 0x80)
		{
			if((i + 2) > nCutCnt)
			{				
				sprintf(szPeriods, "%s", "... ");
				break;
			}

			sprintf (szTmpWord, "%c%c", chComp, chExtra) ;			
			strcat (szReturnWord, szTmpWord) ;
			i++;
		}
		
		else
		{
			sprintf (szTmpWord, "%c", chComp) ;
			strcat (szReturnWord, szTmpWord) ;
		}
	}

	if(bIsCut)
	{
		sprintf(szTmpWord, "%s", szPeriods);
		strcat(szReturnWord, szTmpWord);
	}
	
	if(0 < len)
	{
		delete [] szTmp;
	}

	
	
	TCHAR* pszDest = NULL;
	cv.MultiByteToUnicode(szReturnWord, &pszDest, &len);	
	sReturnWord.Format(_T("%s"), pszDest);

	if(0 < len)
	{
		delete [] pszDest;
	}

	return sReturnWord;
}

INT CBL_LOC_LOAN_PROC::StartLog(CString strLogTitle)
{
	CFileFind filefind;
	if(!filefind.FindFile(_T("..\\cfg\\_SpeedCheck"))){return 0;}

	m_strLogTitle = strLogTitle;
	m_nSTART = GetTickCount();
	return 0;
}

INT CBL_LOC_LOAN_PROC::EndLog()
{
	CFileFind filefind;
	if(!filefind.FindFile(_T("..\\cfg\\_SpeedCheck"))){return 0;}

	m_nFINISH = GetTickCount();
	double timeSec;
	timeSec = (m_nFINISH - m_nSTART) / 1000.0;
	CString strLog;
	SYSTEMTIME t;
	GetLocalTime(&t);
	strLog.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d.%03d][%s] - %g sec"), t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds, m_strLogTitle, timeSec);
	WriteLog(strLog);
	return 0;
}

INT CBL_LOC_LOAN_PROC::WriteLog(CString strLog)
{
	CFileFind filefind;
	if(!filefind.FindFile(_T("..\\cfg\\_SpeedCheck"))){return 0;}

	CString filename;
	filename = filename + _T("..\\tunning\\SpeedCheck.txt");
	FILE *fp;	
	fp = _tfopen(filename, _T("a+b"));
	if(NULL == fp){return 0;}
	fseek(fp, 0, SEEK_END);		
	if(0 == ftell(fp))
	{
		fputc(0xFF, fp);
		fputc(0xFE, fp);
	}	
	_ftprintf(fp, _T("%s\r\n"), strLog);
	fclose(fp);
	return 0;
}

LRESULT CBL_LOC_LOAN_PROC::ReceiptChangeTab(WPARAM wParam, LPARAM lParam)
{
	INT nStatus = lParam;
	m_ReceiptDlg->CheckTab(nStatus);

	return 0;
}



INT CBL_LOC_LOAN_PROC::ExcuteUNDO()
{
EFS_BEGIN

	CString strFuncName = _T("ExcuteUNDO");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pCM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(NULL == pGrid) 
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	pGrid->SelectMakeList();
	INT nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	INT nIndex,ids=-1;
	CString sRecKey,sStatus,sTempStatus,sManageCode,sLDevice;
	if(0 == nRowCount)
	{
		SelfCloseMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return 0;
	}
	
	for(INT i = 0; i < nRowCount; i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) 
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}

		ids = pDM->GetCellData(_T("�������"), nIndex, sStatus);
		ids = pDM->GetCellData(_T("��������"), nIndex, sManageCode);
		ids = pDM->GetCellData(_T("L_DEVICE"), nIndex, sLDevice);

		
		CString sOtherLoanKey;
		ids = pDM->GetCellData(_T("Ÿ������KEY"),nIndex, sOtherLoanKey);
		
		if(sManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE)
		{
			
			if(sOtherLoanKey.IsEmpty())
			{
				SelfCloseMessageBox(_T("�ٸ� �������� �ڷᰡ �ֽ��ϴ�.\r\n����� �� �����ϴ�."));
				return 0;
			}
		}
		if(0 != i)
		{
			if(sTempStatus != sStatus)
			{
				SelfCloseMessageBox(_T("���õ� �ڷ��� ����(����/�ݳ�����/����)��\r\n��ġ�ϴ� ��쿡�� ��� �����մϴ�."));
				return 0;
			}
		}

		
		if(_T("0") == sStatus || _T("2") == sStatus)
		{
			if(sOtherLoanKey.IsEmpty())
			{
				if(	_T("KOLASIII") == sLDevice || _T("KOLASIII-�̵�������") == sLDevice ||
					_T("KOLASYS") == sLDevice  || _T("SMARTID") == sLDevice )					
				{
				}
				else
				{
					SelfCloseMessageBox(_T("KOLASIII���� ������ �ڷḸ ��� �����մϴ�."));
					return 0;
				}
			}
		}

		sTempStatus=sStatus;
		pGrid->SelectGetPrev();
	}

	
	CString sOtherLoanKey;
	ids = pDM->GetCellData(_T("Ÿ������KEY"),nIndex, sOtherLoanKey);
	
	if(_T("0") == sStatus)
	{
		if(!sOtherLoanKey.IsEmpty())
		{
			ids = UNDO_OtherLoan();
			if(0 == ids)
			{
				SelfCloseMessageBox(_T("Ÿ�������� ��ҵǾ����ϴ�."));
			}
		}
		else
		{
			ids = UNDO_Loan();
			if(0 == ids)
			{
				SelfCloseMessageBox(_T("������ ��� �Ǿ����ϴ�."));
			}
		}
	}	
	else if(_T("2") == sStatus)
	{
		ids = UNDO_Postphonement_Return();
		if(0 == ids)
		{
			SelfCloseMessageBox(_T("�ݳ� ���Ⱑ ��� �Ǿ����ϴ�."));
		}
		else if(15 == ids && 1 != nRowCount)
		{
			SelfCloseMessageBox(_T("�ݳ� ���Ⱑ ��� �Ǿ����ϴ�.\r\n(�̹� 1�� �̻� �ݳ����� ��Ұ� ����� �ڷᰡ �ֽ��ϴ�.)"));
		}
		else if(15 == ids && 1 == nRowCount)
		{
			SelfCloseMessageBox(_T("�̹� 1�� �̻� �ݳ����� ��Ұ� ����� �ڷ��Դϴ�."));
		}
	}
	else if(_T("3") == sStatus)
	{
		ids = UNDO_Reservation();
		if(0 == ids)
		{
			SelfCloseMessageBox(_T("������ ��� �Ǿ����ϴ�."));
		}
	}
	else if(_T("L") == sStatus || _T("D") == sStatus)
	{
		SelfCloseMessageBox(_T("å�η����� �ڷ�� �������� �ʽ��ϴ�."));
	}
	else if(_T("R") == sStatus)
	{
		SelfCloseMessageBox(_T("å�η��ݳ� �ڷ�� �������� �ʽ��ϴ�."));
	}
	else if(_T("O") == sStatus)
	{
		SelfCloseMessageBox(_T("å�η����� �ڷ�� �������� �ʽ��ϴ�."));
	}
	return ids;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UNDO_Loan()
{
EFS_BEGIN
	
	INT ids = 0;
	CString strFuncName = _T("UNDO_Loan");

	CString sMsg;
	sMsg.Format(_T("\r\n������ ���� �ڷḦ ����Ͻðڽ��ϱ�?\r\n"));
	if(IDYES!=MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("���� ���"), MB_YESNO | MB_ICONQUESTION))
	{
		return 5;
	}

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pGrid->SelectMakeList();
	INT nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	INT nIndex;
	CString sCLRecKey, sCLURecKey, sBookKey, sReservationDate, strPublishFormCode, strRegNo, sQuery, strSpeciesKey;

	for(INT i = 0 ; i < nRowCount ;i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

 		
 		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 		{
 			KL_StartTemp();
 		}

		CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);
		pMobileApi->StartFrame();
		pMobileApi->InitDBFieldData();
		
		ids = pDM->GetCellData(_T("����/�ݳ�����KEY"), nIndex, sCLRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("åKEY"), nIndex, sBookKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("������KEY"), sCLURecKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("������"), nIndex, sReservationDate);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("����/���ӱ���"), nIndex, strPublishFormCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("��Ϲ�ȣ"), nIndex, strRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("��KEY"), nIndex, strSpeciesKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		CString strLastWork;
		if(sReservationDate.IsEmpty()) 
		{
			
			pMobileApi->MakeDeleteFrame(_T("LS_WORK_T01"),_T("REC_KEY"),_T("NUMERIC"),sCLRecKey);
			
			pMobileApi->InitDBFieldData();
			
			CString strLoanCount;
			strLoanCount.Format(_T("(SELECT DECODE(SIGN(NVL(LOAN_COUNT,0)-1),1,LOAN_COUNT-1,0) ")
								_T("FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s)"), sCLURecKey);
			pMobileApi->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), strLoanCount);

			CString strTmp;
	
			strTmp.Format(_T("(SELECT MAX(LOAN_DATE) FROM LS_WORK_T01 WHERE USER_KEY=%s) "), sCLURecKey);
			
			
			pMobileApi->AddDBFieldData(_T("LAST_LOAN_DATE"),_T("NUMERIC"),strTmp);
			strLastWork = m_pParentMgr->GetWorkLogMsg(_T("����(UNDO)"), __WFILE__, __LINE__);
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"), strLastWork);
			pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"),_T("REC_KEY"),_T("NUMERIC"),sCLURecKey);

 			
 			if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 			{
 				
 				CString strGetData;
 				ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����ȸ������"), strGetData, 0);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
 				if(_T("Y") == strGetData)
 				{
 					CString strLoanLibCode;
 					ids = pDM->GetCellData(_T("���⵵������ȣ"), nIndex, strLoanLibCode);
 					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
 					if(strLoanLibCode.IsEmpty() || strLoanLibCode == m_pParentMgr->m_pInfo->LIB_CODE)
 					{
						strLoanLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
 						CString strUserNo;
 						ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڹ�ȣ"), strUserNo, 0);
 						if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
 
 						CFrameWriter Writer(m_pParentMgr->m_pInfo);
 						Writer.setCommand(_T("IL_K21_LOC_B01_SERVICE"));
 						Writer.addRecord();
 						Writer.addElement(_T("���⵵������ȣ"), strLoanLibCode);
 						Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
						Writer.addElement(_T("����KEY"), sCLRecKey);
  						CString strFrame = Writer.getFrame();
  						KL_AddTemp(strFrame);
 					}
 				}
 			}

			
			SetTempData(SHARE_CO_LOAN_TBL_DELETE_REC_KEY, CComVariant(sCLRecKey));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sCLURecKey));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_COUNT,LAST_LOAN_DATE")));
			ShareInfoCenterUpload(_T("D"), _T("U"));
		}
		else if(!sReservationDate.IsEmpty())
		{
			
			pMobileApi->AddDBFieldData(_T("STATUS"),_T("STRING"),_T("3"));
			pMobileApi->AddDBFieldData(_T("LOAN_DATE"),_T("NUMERIC"),_T("NULL"));
			pMobileApi->AddDBFieldData(_T("RETURN_PLAN_DATE"),_T("NUMERIC"),_T("NULL"));
			pMobileApi->AddDBFieldData(_T("L_DEVICE"),_T("NUMERIC"),_T("NULL"));
			
			pMobileApi->AddDBFieldData(_T("L_WORKER"),_T("NUMERIC"),_T("NULL"));
			
			pMobileApi->AddDBFieldData(_T("LOAN_TYPE_CODE"),_T("NUMERIC"),_T("NULL"));
			strLastWork = m_pParentMgr->GetWorkLogMsg(_T("�������(UNDO)"), __WFILE__, __LINE__);
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"), strLastWork);
			pMobileApi->MakeUpdateFrame(_T("LS_WORK_T01"),_T("REC_KEY"),_T("NUMERIC"),sCLRecKey);

			
			pMobileApi->InitDBFieldData();
			
			CString strLoanCount;
			strLoanCount.Format(_T("(SELECT DECODE(SIGN(NVL(LOAN_COUNT,0)-1),1,LOAN_COUNT-1,0) ")
								_T("FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s)"), sCLURecKey);
			pMobileApi->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), strLoanCount);

			pMobileApi->AddDBFieldData(_T("RESERVATION_COUNT"),_T("NUMERIC"),_T("RESERVATION_COUNT+1"));
			CString strTmp;
			
			strTmp.Format(_T("(SELECT MAX(LOAN_DATE) FROM LS_WORK_T01 WHERE USER_KEY=%s AND REC_KEY NOT IN (%s))"), sCLURecKey, sCLRecKey);			

			pMobileApi->AddDBFieldData(_T("LAST_LOAN_DATE"),_T("NUMERIC"),strTmp);
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"),m_pParentMgr->GetWorkLogMsg(_T("����(UNDO)"), __WFILE__, __LINE__));
			pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"),_T("REC_KEY"),_T("NUMERIC"),sCLURecKey);

 			
 			if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 			{
 				
 				CString strGetData;
 				ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����ȸ������"), strGetData, 0);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
 				if(_T("Y") == strGetData)
 				{
 					CString strLoanLibCode;
 					ids = pDM->GetCellData(_T("���⵵������ȣ"), nIndex, strLoanLibCode);
 					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
 					if(strLoanLibCode.IsEmpty() || strLoanLibCode == m_pParentMgr->m_pInfo->LIB_CODE)
 					{
						strLoanLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
 						CString strUserNo;
 						ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڹ�ȣ"), strUserNo, 0);
 						if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
 
 						CFrameWriter Writer(m_pParentMgr->m_pInfo);
 						Writer.setCommand(_T("IL_K21_LOC_B01_SERVICE"));
 						Writer.addRecord();
 						Writer.addElement(_T("���⵵������ȣ"), strLoanLibCode);
 						Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
						Writer.addElement(_T("����KEY"), sCLRecKey);
  						CString strFrame = Writer.getFrame();
  						KL_AddTemp(strFrame);
 					}
 				}
 			}
			
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sCLRecKey));
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("STATUS,LOAN_DATE,RETURN_PLAN_DATE,L_DEVICE,L_WORKER,LOAN_TYPE_CODE")));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sCLURecKey));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_COUNT,RESERVATION_COUNT,LAST_LOAN_DATE")));
			ShareInfoCenterUpload(_T("U"), _T("U"));
		}
		
		CString strTblName;
		CString strWorkingStatus;
		if(_T("MO") == strPublishFormCode || _T("����") == strPublishFormCode)
		{
			strTblName = _T("BO_BOOK_TBL");
		}
		else if(_T("SE") == strPublishFormCode || _T("����") == strPublishFormCode)
		{
			strTblName = _T("SE_BOOK_TBL");
		}
		
		else if(_T("NB") == strPublishFormCode || _T("���") == strPublishFormCode)
		{
			strTblName = _T("CO_NON_DB_BOOK_TBL");
		}
		
		sQuery.Format(
			_T("SELECT WORKING_STATUS FROM %s WHERE REC_KEY = %s"), 
			strTblName, sBookKey);		
		ids = pDM->GetOneValueQuery(sQuery,strWorkingStatus);

		if(FALSE == strTblName.IsEmpty())
		{
			pMobileApi->InitDBFieldData();
			pMobileApi->AddDBFieldData(_T("WORKING_STATUS"),_T("NUMERIC"),_T("PREV_WORKING_STATUS"));
			pMobileApi->AddDBFieldData(_T("PREV_WORKING_STATUS"),_T("STRING"),strWorkingStatus);
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"),m_pParentMgr->GetWorkLogMsg(_T("����(UNDO)"), __WFILE__, __LINE__));
			pMobileApi->MakeUpdateFrame(strTblName,_T("REC_KEY"),_T("NUMERIC"),sBookKey);

			
			int nBookType = -1;
			if(_T("BO_BOOK_TBL") == strTblName)
			{
				nBookType = 1;
			}
			else if(_T("SE_BOOK_TBL") == strTblName)
			{
				nBookType = 2;
			}
			else if(_T("CO_NON_DB_BOOK_TBL") == strTblName)
			{
				nBookType = 0;
			}			
			AddIdxQueryFrame(pDM, nBookType, strSpeciesKey);
		}

		ids = pMobileApi->SendFrame();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);

		pMobileApi->EndFrame();
		CLocCommonAPI::MobileApiEndLog(pMobileApi);
		
 		
 		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 		{
 			KL_EndTemp(strLastWork);
 		}

		pGrid->SelectGetPrev();
	}

	m_bReturnBookNonFree=TRUE;
	SPFExcutePath(_T("ȸ�����ε�_NOASK"));
	
	return 0;

EFS_END
return -1;
}



INT CBL_LOC_LOAN_PROC::UNDO_Postphonement_Return()
{
EFS_BEGIN

	CString strFuncName = _T("UNDO_Postphonement_Return");

	CString sMsg;
	sMsg.Format(_T("\r\n������ �ݳ����� �ڷḦ ����Ͻðڽ��ϱ�?\r\n"));
	if(IDYES!=MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("�ݳ����� ���"), MB_YESNO | MB_ICONQUESTION))
	{
		return 5;
	}

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pGrid->SelectMakeList();
	INT nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	INT nIndex,ids=-1;
	CString sCLRecKey,sCLURecKey,sBookKey,sQuery,sDelayCnt,sReturnDate,sPreReturnDate;
	
	BOOL bIncludePassData = FALSE;

	for(INT i = 0 ; i < nRowCount ;i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);
		pMobileApi->StartFrame();
		pMobileApi->InitDBFieldData();

		ids = pDM->GetCellData(_T("����/�ݳ�����KEY"), nIndex, sCLRecKey);
		ids = pDM->GetCellData(_T("åKEY"), nIndex, sBookKey);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("������KEY"), sCLURecKey, 0);
		ids = pDM->GetCellData(_T("�ݳ�����Ƚ��"), nIndex, sDelayCnt);
		ids = pDM->GetCellData(_T("�ݳ�������"), nIndex, sReturnDate);

		CESL_DataMgr tempDM;
		tempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
		sQuery.Format(_T("SELECT PRE_RETURN_PLAN_DATE FROM LS_WORK_T01 WHERE REC_KEY = %s"),sCLRecKey);
		tempDM.GetOneValueQuery(sQuery,sPreReturnDate);

		
		
		if(sPreReturnDate==sReturnDate) 
		{
			bIncludePassData=TRUE;
			pGrid->SelectGetPrev();
			pDM->EndFrame();
			continue;
		}

		CString strStatus;
		if(sDelayCnt==_T("1"))
		{
			
			strStatus = _T("0");
			pMobileApi->AddDBFieldData(_T("STATUS"),_T("STRING"),_T("0"));
			pMobileApi->AddDBFieldData(_T("DELAY_CNT"),_T("NUMERIC"),_T("DELAY_CNT-1"));
			pMobileApi->AddDBFieldData(_T("RETURN_PLAN_DATE"),_T("NUMERIC"),_T("PRE_RETURN_PLAN_DATE"));

			
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sCLRecKey));
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("STATUS,DELAY_CNT,RETURN_PLAN_DATE")));
			ShareInfoCenterUpload(_T("U"), _T("N"));
		}
		else
		{
			
			strStatus = _T("2");
			pMobileApi->AddDBFieldData(_T("STATUS"),_T("STRING"),_T("2"));
			pMobileApi->AddDBFieldData(_T("DELAY_CNT"),_T("NUMERIC"),_T("DELAY_CNT-1"));
			pMobileApi->AddDBFieldData(_T("RETURN_PLAN_DATE"),_T("NUMERIC"),_T("PRE_RETURN_PLAN_DATE"));

			
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sCLRecKey));
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("STATUS,DELAY_CNT,RETURN_PLAN_DATE")));
			ShareInfoCenterUpload(_T("U"), _T("N"));
		}

		
		CString strLastWork;
		strLastWork = m_pParentMgr->GetWorkLogMsg(_T("�ݳ�����(UNDO)"), __WFILE__, __LINE__);
		pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"), strLastWork);
		pMobileApi->MakeUpdateFrame(_T("LS_WORK_T01"),_T("REC_KEY"),_T("NUMERIC"),sCLRecKey);

		ids = pMobileApi->SendFrame();
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, _T("UNDO_Postphonement_Return"));
		}

		pMobileApi->EndFrame();
		CLocCommonAPI::MobileApiEndLog(pMobileApi);

		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			
			CString strGetData;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("����ȸ������"), strGetData, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(_T("Y") == strGetData)
			{
				CString strLoanLibCode;
				ids = pDM->GetCellData(_T("���⵵������ȣ"), nIndex, strLoanLibCode);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			
 				pDM->GetCellData(_T("��������"), nIndex, strGetData);
				if(strGetData == m_pParentMgr->m_pInfo->MANAGE_CODE)			
				{
					strLoanLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
					CString strUserNo;
					ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڹ�ȣ"), strUserNo, 0);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					CFrameWriter Writer(m_pParentMgr->m_pInfo);
					Writer.setCommand(_T("IL_K23_LOC_B01_SERVICE"));
					Writer.addRecord();
					Writer.addElement(_T("���⵵����"), strLoanLibCode);
					Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
					Writer.addElement(_T("�ݳ�������"), sPreReturnDate);
					Writer.addElement(_T("����"), strStatus);
					Writer.addElement(_T("����KEY"), sCLRecKey);
 					CString strFrame = Writer.getFrame();
 					KL_StartTemp();
 					KL_AddTemp(strFrame);
 					KL_EndTemp(m_pParentMgr->GetWorkLogMsg(_T("����ȸ���ݳ��������"), __WFILE__, __LINE__));
				}
			}
		}

		pGrid->SelectGetPrev();
	}


	m_bReturnBookNonFree=TRUE;
	SPFExcutePath(_T("ȸ�����ε�_NOASK"));

	if(bIncludePassData)
	{
		return 15;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UNDO_Reservation()
{
EFS_BEGIN

	CString strFuncName = _T("UNDO_Reservation");

	CString sMsg;
	sMsg.Format(_T("\r\n������ ���� �ڷḦ ����Ͻðڽ��ϱ�?\r\n"));
	if(IDYES!=MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("���� ���"), MB_YESNO | MB_ICONQUESTION))
	{
		return 5;
	}

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pGrid->SelectMakeList();
	INT nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	INT nIndex,ids=-1;
	CString sCLRecKey,sCLURecKey,sBookKey,sQuery,strPublishFormCode;

	for(INT i = 0 ; i < nRowCount ;i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		
		if(_T("Y") == m_pManageValue->m_sUnmannedReserveSys)
		{
			CString strUnmanned;
			ids = pDM->GetCellData(_T("���δ��⿹�����"), nIndex, strUnmanned);
			if(_T("Y") ==strUnmanned)
			{ 
				sMsg.Format(_T("���ο����û �ڷ��Դϴ�.\r\n����Ͻðڽ��ϱ�?\r\n"));
				if(IDYES != MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("���� ���"), MB_YESNO|MB_ICONQUESTION))
				{
					return 5;
				}
			} 
			else if(_T("O") == strUnmanned)
			{
				sMsg.Format(_T("���ο����� �ڷ�� �������� �ʽ��ϴ�."));
				MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("���� ���"), MB_ICONINFORMATION);
				return 5;
			}
		}

		CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);
		pMobileApi->StartFrame();
		pMobileApi->InitDBFieldData();

		ids = pDM->GetCellData(_T("����/�ݳ�����KEY"), nIndex, sCLRecKey);
		ids = pDM->GetCellData(_T("åKEY"), nIndex, sBookKey);
		ids = pDM->GetCellData(_T("����/���ӱ���"), nIndex, strPublishFormCode);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("������KEY"), sCLURecKey, 0);

		
		pMobileApi->MakeDeleteFrame(_T("LS_WORK_T01"),_T("REC_KEY"),_T("NUMERIC"),sCLRecKey);

		
		pMobileApi->AddDBFieldData(_T("RESERVATION_COUNT"),_T("NUMERIC"),_T("RESERVATION_COUNT-1"));
		pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"),m_pParentMgr->GetWorkLogMsg(_T("����(UNDO)"), __WFILE__, __LINE__));
		pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"),_T("REC_KEY"),_T("NUMERIC"),sCLURecKey);

		
		pMobileApi->InitDBFieldData();
		if(_T("MO") == strPublishFormCode || _T("����") == strPublishFormCode)
		{
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"),m_pParentMgr->GetWorkLogMsg(_T("����(UNDO)"), __WFILE__, __LINE__));
			pMobileApi->MakeUpdateFrame(_T("BO_BOOK_TBL"),_T("REC_KEY"),_T("NUMERIC"),sBookKey);
		}
		else
		{
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"),m_pParentMgr->GetWorkLogMsg(_T("����(UNDO)"), __WFILE__, __LINE__));
			pMobileApi->MakeUpdateFrame(_T("SE_BOOK_TBL"),_T("REC_KEY"),_T("NUMERIC"),sBookKey);
		}
		
		ids = pMobileApi->SendFrame();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		pMobileApi->EndFrame();
		CLocCommonAPI::MobileApiEndLog(pMobileApi);
		
		
		SetTempData(SHARE_CO_LOAN_TBL_DELETE_REC_KEY, CComVariant(sCLRecKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sCLURecKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("RESERVATION_COUNT")));
		ShareInfoCenterUpload(_T("D"), _T("U"));

		pGrid->SelectGetPrev();
	}
	
	m_bReturnBookNonFree=TRUE;
	SPFExcutePath(_T("ȸ�����ε�_NOASK"));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::KL_OtherLibUserImport(CString strLibCode, CString strLibName, CString &strUserNo)
{
	INT ids = 0;
	CString strGetLibCode=_T(""), strGetLibName=_T("");
	INT nMode = 0;
	if(!strLibCode.IsEmpty() && !strLibName.IsEmpty()) nMode = 1;
	
	if(0 == nMode)
	{
		ids = KL_GetSelectLibInfo(strUserNo, strGetLibCode, strGetLibName);
		if(0 != ids) return ids;
	}
	else
	{
		strGetLibCode = strLibCode;
		strGetLibName = strLibName;
	}
	
	const int nCnt = 15;
	CString strGetData[nCnt] = {_T(""),};
	ids = KL_OtherLibUserSearch(nMode, strGetLibCode, strUserNo, strGetData);
	if(0 != ids) return ids;
	
	CKL_UserImportDlg dlg(m_pParentMgr);
	for(INT i = 0; i < nCnt; i++)
	{
		dlg.m_strGetData[nCnt] = strGetData[nCnt];
	}
	dlg.m_strLibName = strGetLibName;
	if(IDOK != dlg.DoModal()) return 1;
	strUserNo = dlg.m_strUserNo;

	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_GetSelectLibInfo(CString strUserNo, CString &strLibCode, CString &strLibName)
{
	INT ids;
	if(14 <= strUserNo.GetLength())
	{
		strLibCode = strUserNo.Left(6);
		if(TRUE == IsNumericText(strLibCode))
		{
			CESL_DataMgr mwDM;
			CFrameReader Reader;
			CFrameWriter Writer(m_pParentMgr->m_pInfo);
			CFrameSender Sender(m_pParentMgr->m_pInfo);
			Writer.setCommand(_T("IL_K16_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("��������ȣ"), strLibCode);
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if(0 > ids) 
			{
				
				SelfCloseMessageBox(_T("å�������� ���� �õ����Դϴ�."), m_pParentMgr);
				return 100;
			}
			Reader.makeDM(&mwDM);
			CString strTemp = Reader.getCommand();
			if(_T("ERROR") == strTemp)
			{
				CString strMsg, strTemp;
				strTemp = Reader.getElement(0, 0);
				strMsg.Format(_T("������������ȸ(16) ERROR CODE : %s"), strTemp);
				mwDM.QueryLog(strMsg.GetBuffer(0));
				strMsg = _T("���������� ��ȸ�� �����Ͽ����ϴ�.\r\n")
						 _T("��õ� �Ͻðڽ��ϱ�?");
				if(IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION|MB_YESNO))
				{
					return KL_GetSelectLibInfo(strUserNo, strLibCode, strLibName);
				}
				return 101;
			}
			if(0 < mwDM.GetRowCount())
			{
				mwDM.GetCellData(_T("��������"), 0, strLibName);
			}
		}
	}
	if(strLibName.IsEmpty())
	{ 
		CLibListDlg dlg(m_pParentMgr, this);
		dlg.SetMode(2);
		if(IDOK != dlg.DoModal()) return 102;
		strLibCode = dlg.m_strLibCode;
		strLibName = dlg.m_strLibName;
	}
	
	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_OtherLibUserSearch(INT nMode, CString strLibCode, CString strUserNo, CString strGetData[])
{
	INT ids;
	CESL_DataMgr mwDM;
	CFrameReader Reader;
	CFrameWriter Writer(m_pParentMgr->m_pInfo);
	CFrameSender Sender(m_pParentMgr->m_pInfo);
	Writer.setCommand(_T("IL_K33_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), strLibCode);
	Writer.addElement(_T("�����ڹ�ȣ"), strUserNo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) 
	{
		
		if(0 == nMode) SelfCloseMessageBox(_T("å�������� ���� �õ����Դϴ�."), m_pParentMgr);
		return 100;
	}
	Reader.makeDM(&mwDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strMsg, strTemp;
		strTemp = Reader.getElement(0, 0);
		if(-1 < strTemp.Find(_T("DT01")))
		{
			if(0 == nMode) SelfCloseMessageBox(_T("������ ���������� ��ȸ�� �̿��ڰ� �����ϴ�."), m_pParentMgr, 2000);
			return 102;
		}
		strMsg.Format(_T("Ÿ�������Ϲ�ȸ����ȸ(33) ERROR CODE : %s"), strTemp);
		mwDM.QueryLog(strMsg.GetBuffer(0));
		if(0 == nMode)
		{
			strMsg = _T("Ÿ������ �Ϲ�ȸ�� ��ȸ�� �����Ͽ����ϴ�.\r\n")
					 _T("��õ� �Ͻðڽ��ϱ�?");
			if(IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION|MB_YESNO))
			{
				return KL_OtherLibUserSearch(nMode, strLibCode, strUserNo, strGetData);
			}
		}
		return 101;
	}
	if(1 > mwDM.GetRowCount())
	{
		if(0 == nMode) SelfCloseMessageBox(_T("������ ���������� ��ȸ�� �̿��ڰ� �����ϴ�."), m_pParentMgr, 2000);
		return 102;
	}
	if(1 < mwDM.GetRowCount())
	{
		if(0 == nMode) SelfCloseMessageBox(_T("2�� �̻��� �̿��ڰ� �˻��Ǿ����ϴ�."), m_pParentMgr);
		return 103;
	}
	mwDM.GetCellData(_T("�����ڹ�ȣ"), 0, strGetData[0]);
	mwDM.GetCellData(_T("�̸�"), 0, strGetData[1]);
	mwDM.GetCellData(_T("�ֹε�Ϲ�ȣ"), 0, strGetData[2]);
	mwDM.GetCellData(_T("�ڵ�����ȣ"), 0, strGetData[3]);
	mwDM.GetCellData(_T("��2����ó"), 0, strGetData[4]);
	mwDM.GetCellData(_T("�̸���"), 0, strGetData[5]);
	mwDM.GetCellData(_T("���"), 0, strGetData[6]);
	mwDM.GetCellData(_T("�������ȣ"), 0, strGetData[7]);
	mwDM.GetCellData(_T("���ּ�"), 0, strGetData[8]);
	mwDM.GetCellData(_T("����ȭ��ȣ"), 0, strGetData[9]);
	mwDM.GetCellData(_T("��������ȣ"), 0, strGetData[10]);
	mwDM.GetCellData(_T("�����ּ�"), 0, strGetData[11]);
	mwDM.GetCellData(_T("������ȭ��ȣ"), 0, strGetData[12]);
	mwDM.GetCellData(_T("������������"), 0, strGetData[13]);
	mwDM.GetCellData(_T("������������"), 0, strGetData[14]);

	return 0;
}


INT CBL_LOC_LOAN_PROC::SetColumnColor(CESL_DataMgr *pDM, CESL_Grid *pGrid, INT row, INT col)
{
	
	if(!m_bManageCodeColorUseYN && !m_bReferenceColorUseYN)
	{
		return 100;
	}
	CString strDMAlias, strFieldAlias, strAlias, strColor, strTmp;
	pGrid->FindLinkDMAndAlias(col, strDMAlias, strFieldAlias);
	if(m_bManageCodeColorUseYN)
	{
		if(_T("�������м���") == strFieldAlias)
		{
			
			pDM->GetCellData(_T("�������м���"), row, strTmp);
			if(_T("") == strTmp)
			{
				strTmp = pGrid->GetAt(row, col);
			}
		
			strAlias.Format(_T("%s_���ڻ�"), strTmp);
			m_pManageCodeColorDM->GetCellData(strAlias, 0, strColor);
			pGrid->SetCol(col);
			
			if(_T("") != strColor)
			{
				pGrid->SetCellForeColor(_ttoi(strColor));
			}
			strAlias.Format(_T("%s_������"), strTmp);
			m_pManageCodeColorDM->GetCellData(strAlias, 0, strColor);
			
			if(_T("") != strColor)
			{
				pGrid->SetCellBackColor(_ttoi(strColor));
			}
		}
	}
	if(m_bReferenceColorUseYN)
	{	
		if(_T("�ڷ��") == strFieldAlias)
		{
			
			pDM->GetCellData(_T("�ڷ��"), row, strTmp);
			if(_T("") == strTmp)
			{
				strTmp = pGrid->GetAt(row, col);
			}
			strAlias.Format(_T("%s_���ڻ�"), strTmp);
			m_pReferenceColorDM->GetCellData(strAlias, 0, strColor);
			pGrid->SetCol(col);
			
			if(_T("") != strColor)
			{
				pGrid->SetCellForeColor(_ttoi(strColor));
			}
			strAlias.Format(_T("%s_������"), strTmp);
			m_pReferenceColorDM->GetCellData(strAlias, 0, strColor);
			
			if(_T("") != strColor)
			{
				pGrid->SetCellBackColor(_ttoi(strColor));
			}
		}
	}
	return 0;
}


INT CBL_LOC_LOAN_PROC::SetColorDM(CString strAlias)
{
	CCfgFileApi api;
	INT nDefaultValue = 0;
	INT nCount = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(strAlias);
	INT nFiledCnt = nCount * 2;
	nCount++;
	if(_T("��������") == strAlias)
	{	
		m_pManageCodeColorDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_COLOR_SETTING_MANAGE"));	
		CString* strCode = new CString[nCount];
		CString* strDesc = new CString[nCount];
		INT ids = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(strAlias, strCode, strDesc, nCount, nDefaultValue);
		if(0 > ids)
		{
			return -100;
		}
		api.SetFile(_T("..\\cfg\\LoanReturn\\ȭ�������_��������.txt"));
		INT col = 0;
		m_pManageCodeColorDM->InitDataMgrRef(nFiledCnt);
		m_pManageCodeColorDM->InsertRow(-1);
		for(INT i=0; i<nCount; i++)
		{
			CString strTmp, strColorCode;
			strTmp.Format(_T("%s_���ڻ�"), strDesc[i]);
			
			m_pManageCodeColorDM->SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
			
			strTmp.Format(_T("%s_������"), strDesc[i]);
			
			m_pManageCodeColorDM->SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
		}
		
		CString strTmp, strColor;
		for(INT h=0; h<nCount; h++)
		{
			strTmp.Format(_T("%s_���ڻ�"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_pManageCodeColorDM->SetCellData(strTmp, strColor, 0);
			strTmp.Format(_T("%s_������"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_pManageCodeColorDM->SetCellData(strTmp, strColor, 0);
		}
		delete []strCode;
		strCode = NULL;
		delete []strDesc;
		strDesc = NULL;
	}
	else if(_T("�ڷ�Ǳ���") == strAlias)
	{		
		m_pReferenceColorDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_COLOR_SETTING_REF"));
		CString* strCode = new CString[nCount];
		CString* strDesc = new CString[nCount];
		INT ids = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(strAlias, strCode, strDesc, nCount, nDefaultValue);
		if(0 > ids)
		{
			return -100;
		}
		api.SetFile(_T("..\\cfg\\LoanReturn\\ȭ�������_�ڷ��.txt"));
		INT col = 0;
		m_pReferenceColorDM->InitDataMgrRef(nFiledCnt);
		m_pReferenceColorDM->InsertRow(-1);
		for(INT i=0; i<nCount; i++)
		{
			CString strTmp, strColorCode;
			strTmp.Format(_T("%s_���ڻ�"), strDesc[i]);
			
			m_pReferenceColorDM->SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
			
			strTmp.Format(_T("%s_������"), strDesc[i]);
			
			m_pReferenceColorDM->SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
		}
		
		CString strTmp, strColor;
		for(INT h=0; h<nCount; h++)
		{
			strTmp.Format(_T("%s_���ڻ�"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_pReferenceColorDM->SetCellData(strTmp, strColor, 0);
			strTmp.Format(_T("%s_������"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_pReferenceColorDM->SetCellData(strTmp, strColor, 0);
		}
		delete []strCode;
		strCode = NULL;
		delete []strDesc;
		strDesc = NULL;
	}
	return 0;
}


VOID CBL_LOC_LOAN_PROC::FreeColorDM(CString strAlias)
{
	if(_T("��������") == strAlias)
	{
		m_pManageCodeColorDM = NULL;
	}
	else if(_T("�ڷ�Ǳ���") == strAlias)
	{
		m_pReferenceColorDM = NULL;
	}
}


VOID CBL_LOC_LOAN_PROC::InitColorDM()
{
	
	CCfgFileApi api;
	CString strTmp;
	api.SetFile(_T("..\\cfg\\LoanReturn\\ȭ�������_��뿩��.txt"));
	api.GetData(_T("��������"), strTmp);
	if(_T("Y") == strTmp)
	{
		m_bManageCodeColorUseYN = TRUE;
		if(0 > SetColorDM(_T("��������")))
		{
			AfxMessageBox(_T("���� �ʱ�ȭ�� �����Ͽ����ϴ�."));
		}
	}
	api.GetData(_T("�ڷ��"), strTmp);
	if(_T("Y") == strTmp)
	{
		m_bReferenceColorUseYN = TRUE;
		if(0 > SetColorDM(_T("�ڷ�Ǳ���")))
		{
			AfxMessageBox(_T("���� �ʱ�ȭ�� �����Ͽ����ϴ�."));
		}
	}
}

INT CBL_LOC_LOAN_PROC::UNDO_OtherLoan()
{
	INT ids;

	if(IDNO == AfxMessageBox(_T("�����Ͻ� �ڷ�� Ÿ������ �ڷ��Դϴ�. ������ ����Ͻðڽ��ϱ�?"), MB_ICONQUESTION|MB_YESNO))
	{
		return 0;
	}

	CESL_Grid* pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"),_T("MainGrid"));
	
	if(NULL != pMainGrid)
	{
		
		pMainGrid->SelectMakeList();
		INT nIdx = pMainGrid->SelectGetTailPosition();
		CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		for(int i=0;i<pMainGrid->SelectGetCount();i++)
		{
			CString strLoanKey;
			strLoanKey = pDM->GetCellData(_T("Ÿ������KEY"),nIdx);
			CFrameWriter Writer(m_pParentMgr->m_pInfo);
			Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("TRANSACTION_WORK"), _T("UPDATE"));
			Writer.addElement(_T("TRANSACTION_CODE"), _T("3"));
			Writer.addElement(_T("LOAN_KEY"),strLoanKey);
			Writer.addElement(_T("CANCEL"),_T("1"));
			
			CFrameSender Sender(m_pParentMgr->m_pInfo);	
			CFrameReader Reader; 
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if (0 > ids) 
			{
				while(true) 
				{
					
					if(IDYES == MessageBox(m_pParentMgr->m_hWnd,_T("å�������� �̵������� ��ſ� �����Ͽ����ϴ�.\r\n��õ� �Ͻðڽ��ϱ�?"), _T("��ȸ����"), MB_YESNO | MB_ICONQUESTION)) 
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
			nIdx=pMainGrid->SelectGetPrev();			
		}
		m_bReturnBookNonFree=TRUE;
		SPFExcutePath(_T("ȸ�����ε�_NOASK"));
	}
	return 0;
}

INT CBL_LOC_LOAN_PROC::OtherReturn(int nType)
{
	INT ids = 0;
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));

	if(IDNO == AfxMessageBox(_T("�����Ͻ� �ڷ�� Ÿ������ �ڷ��Դϴ�. �ݳ��Ͻðڽ��ϱ�?"), MB_ICONQUESTION|MB_YESNO))
	{
		return 0;
	}

	CESL_Grid* pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"),_T("MainGrid"));


	INT nIdx = pMainGrid->SelectGetIdx();
	
	
	
	if(nIdx < 0){
		pMainGrid->SelectMakeList();
		pMainGrid->SelectGetHeadPosition();
		nIdx = pMainGrid->SelectGetIdx();
	}

	if(NULL != pMainGrid)
	{
		CString strReturnDate;
		CString strOtherLoanKey;

		CString strMsg, strRegNo, strTitle, strAppendixSendYN, strAppendixRegNoList;
		
		CTime tLoanDate = CTime::GetCurrentTime();

		INT nReturnType = InitReturnMode();		
		CString strDate, sReturnDateControlAlias = _T("�ݳ���");
		if(nReturnType == 2)	
		{
			sReturnDateControlAlias = _T("�ұ޹ݳ���");
		}
		ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, strDate);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("OtherReturn"));
		strReturnDate.Format(_T("%s %02d:%02d:%02d"), strDate, tLoanDate.GetHour(), tLoanDate.GetMinute(), tLoanDate.GetSecond());

			strOtherLoanKey = pDM->GetCellData(_T("Ÿ������KEY"),nIdx);
			
			if(nType == 1)
			{				
				strOtherLoanKey = pDM->GetCellData(_T("LCT_KEY"),nIdx);
			}

			pDM->GetCellData(_T("��Ϲ�ȣ"), nIdx, strRegNo);
			pDM->GetCellData(_T("��ǥ��"), nIdx, strTitle);
			pDM->GetCellData(_T("�ηϹ߼�����"), nIdx, strAppendixSendYN);
			pDM->GetCellData(_T("�ηϵ�Ϲ�ȣ����Ʈ"), nIdx, strAppendixRegNoList);
			if (_T("Y") == strAppendixSendYN) {
				strMsg.Format(
					_T("�η��ڷ�� �Բ� ����� �ڷ��Դϴ�.\r\n")
					_T("\r\n")
					_T("[ %s:%s ]\r\n")
					_T("�η��ڷḦ Ȯ�� �ϼ̽��ϱ�?\r\n")
					_T(" - �ηϵ�Ϲ�ȣ : %s\r\n")
					_T("( �� : �ݳ�,  �ƴϿ� : �ݳ���� )")
					, strRegNo, strTitle, strAppendixRegNoList);
				if (IDYES != MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("Ÿ���ݳ�>�η�Ȯ��"), MB_ICONINFORMATION|MB_YESNO)) {
					return 0;
				}
			}
			
			CFrameWriter Writer(m_pParentMgr->m_pInfo);
			Writer.setCommand(_T("LILL_K07_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("LOAN_KEY"),strOtherLoanKey);
			Writer.addElement(_T("RETURN_DATE"),strReturnDate);
			Writer.addElement(_T("RETURN_LIB_CODE"),m_pParentMgr->m_pInfo->LIB_CODE);
			Writer.addElement(_T("R_DEVICE"),_T("KOLAS III"));
			Writer.addElement(_T("R_WORKER"),m_pParentMgr->m_pInfo->USER_ID);
			CFrameSender Sender(m_pParentMgr->m_pInfo);	
			CFrameReader Reader; 
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if (0 > ids) 
			{
				while(true) 
				{
					if(IDYES == MessageBox(m_pParentMgr->m_hWnd,_T("å�������� �̵������� ��ſ� �����Ͽ����ϴ�.\r\n��õ� �Ͻðڽ��ϱ�?"), _T("��ȸ����"), MB_YESNO | MB_ICONQUESTION)) 
					{
						ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
						if(0 <= ids) continue;
					}
					else
					{
						return 0;
					}
				}
			}
			if(_T("OK") == Reader.getElement(0,0))
			{
				CESL_DataMgr* pReturnBasket = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_RETURN_INFO"));	
				if (NULL == pReturnBasket) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("OtherReturn"));
				INT nInsertIdx = pReturnBasket->GetRowCount();
				pReturnBasket->InsertRow(-1);
				INT nColCnt = pReturnBasket->GetRefColumnCount();
				CString strField_Alias, strField_Name, strField_Type, strInit_Value, strData_Type, strResult;
				for (INT i = 0; i < nColCnt; i++) {
					pReturnBasket->GetColumnRefInfo(i, strField_Alias, strField_Name, strField_Type, strInit_Value, strData_Type);
					if (_T("R_DEVICE") == strField_Alias) {
						strResult = _T("KOLASIII");
					} else if (_T("R_WORKER") == strField_Alias) {
						strResult = m_pParentMgr->m_pInfo->USER_ID;
					} else if (_T("�ݳ���") == strField_Alias) {
						strResult = strReturnDate;
					} else {
						pDM->GetCellData(strField_Alias, nIdx, strResult);
					}
					pReturnBasket->SetCellData(strField_Alias, strResult, nInsertIdx);
				}

				CString strHoldLibCode,
					strQry,
					strHoldManageCode,
					strLoanStopDate,
					strLocalKey,
					strReturnPlanDate;
				pDM->GetCellData(_T("�ݳ�������"), nIdx, strReturnPlanDate);
				COleDateTime t1 (_ttoi(strDate.Mid(0,4)), _ttoi(strDate.Mid(5,2)), _ttoi(strDate.Mid(8,2)), 0, 0, 0);
				COleDateTime t2 (_ttoi(strReturnPlanDate.Mid(0,4)), _ttoi(strReturnPlanDate.Mid(5,2)), _ttoi(strReturnPlanDate.Mid(8,2)), 0, 0, 0);
				if ( t1 > t2 )
				{
					pDM->GetCellData(_T("�������������ȣ"), nIdx, strHoldLibCode);
					strQry.Format(_T("SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = '%s'"),strHoldLibCode);
					if ( !strHoldLibCode.IsEmpty() && pDM->GetOneValueQuery(strQry,strHoldManageCode) != -4007 ){
						pDM->GetCellData( _T("����KEY"), nIdx, strLocalKey );					
						strQry.Format(_T("SELECT ECO$F_GET_LOANSTOPDATE( %s, TO_DATE('%s'), '%s' ) FROM DUAL"), strLocalKey, strDate, strHoldManageCode);				
						pDM->GetOneValueQuery( strQry, strLoanStopDate );
						if( !strLoanStopDate.IsEmpty() ) {
							CESL_DataMgr* pMemDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
							if (pMemDM != NULL)
							{
								pMemDM->SetCellData(_T("����������"), strLoanStopDate ,0 );
								pMemDM->SetCellData(_T("ȸ������"), _T("1"),0 );
							}
						}
					}					
				}
				
				AddMemberInfo();

				pDM->DeleteRow(nIdx);

				
				Send3000ApiSocket(_T("Ÿ���ݳ�"));
				if (0 != pDM->GetRowCount()) {
					((CMSHFlexGrid*)pMainGrid)->RemoveItem(nIdx+1);
					CString sIndex;
					for (INT i = 0; i < pDM->GetRowCount(); i++) {
						sIndex.Format(_T("%d"), i+1);
						pMainGrid->SetTextMatrix(i+1, 0, sIndex);
					}
				}
				SetBookStatusCnt_Unity();
			}
			else {
				AddMemberInfo();	
			}
		}

	ViewLoanScreen();
	
	return ids;
	
}

INT CBL_LOC_LOAN_PROC::RFID_Loan(){
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("RFID_Loan");

	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	ClearBasket();
	
	CESL_DataMgr* pRFID_DM = m_pParentMgr->FindDM(_T("DM_RFID���"));
	if(NULL == pRFID_DM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sBasketDMAlias = _T("DM_BO_LOC_3100_BASKET");
	
	CESL_DataMgr *pBasketDM = m_pParentMgr->FindDM(sBasketDMAlias);
	if(NULL == pBasketDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sPublishForm;
	CString sLoanYN;	
	CString sRegNo;	
	CString sSerialNo;	
	CString sSql;	
	CString sTableName;
	CString sWorkingStatus;

	INT nInsertIndex;
	INT nLoanCnt = 0;
	for(INT i = 0; i < pRFID_DM->GetRowCount(); i++)
	{
		ids = pRFID_DM->GetCellData(_T("��������"), i, sPublishForm);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if(_T("MO") == sPublishForm || _T("B") == sPublishForm)
		{
			sTableName = _T("BO_BOOK_TBL");
		}
		else if(_T("SE") == sPublishForm || _T("S") == sPublishForm)
		{
			sTableName = _T("SE_BOOK_TBL");
		}
		else if(_T("NB") == sPublishForm || _T("N") == sPublishForm)
		{
			sTableName = _T("CO_NON_DB_BOOK_TBL");
		}
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = pRFID_DM->GetCellData(_T("�ø���"), i, sSerialNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = pRFID_DM->GetCellData(_T("å����"), i, sWorkingStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(sWorkingStatus != _T("BOL611O") && sWorkingStatus != _T("BOL319O")){
			CString strIsILL;
			pRFID_DM->GetCellData(_T("å�η�����"), i, strIsILL);

			if(_T("Y") != strIsILL)
			{
				sSql.Format(_T("SELECT MAX(REG_NO) FROM %s WHERE WORKNO='%s' AND MANAGE_CODE='%s' ")
							, sTableName, sSerialNo, m_pParentMgr->m_pInfo->MANAGE_CODE);
			}
			else
			{
				sSql.Format(_T("SELECT MAX(REG_NO) FROM ILL_TRANS_TBL WHERE WORKNO='%s' AND APL_LIBCODE='%s'")
							, sSerialNo, m_pParentMgr->m_pInfo->LIB_CODE);
			}
			CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pRFID_DM, sSql, sRegNo);
			sRegNo.TrimRight(); sRegNo.TrimLeft();
			if(sRegNo.IsEmpty())
			{
				CString sEMsg;
				sEMsg.Format(_T("�ø��� ��ȣ�� '%s'�� �ڷ��� ��Ϲ�ȣ�� �������� �ʽ��ϴ�."), sSerialNo);
				ViewMessage(sEMsg);
				continue;
			}
		}

		ids = pRFID_DM->GetCellData(_T("�̿��ڹ�ȣ"), i, sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		

		ids = pRFID_DM->GetCellData(_T("���⿩��"), i, sLoanYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if(_T("1") == sLoanYN)
		{
			CString sEMsg;
			sEMsg.Format(_T("��� ��ȣ�� '%s'�� �ڷ�� ���� �������� �ڷ��Դϴ�."), sRegNo);
			ViewMessage(sEMsg);
			continue;			
		}
		
		
		m_pParentMgr->SetControlData(sCMAlias, _T("�����ڹ�ȣ"), sRegNo);

		if(_T("0") == sLoanYN)
		{
			MoveRegNoToNextProcBasket(EXP_LOAN, TRUE);
		}
		else if(_T("2") == sLoanYN)
		{
			MoveRegNoToNextProcBasket(EXP_RESERVELOAN, TRUE);
		}
		else if(_T("L") == sLoanYN)
		{
			MoveRegNoToNextProcBasket(_T("L"), TRUE);
		}	
		else if(_T("LILL") == sLoanYN)
		{
			MoveRegNoToNextProcBasket(_T("LILL"), TRUE);
		}			

		nInsertIndex = pBasketDM->GetRowCount() - 1;
		ids = pBasketDM->SetCellData(_T("�ø���"), sSerialNo, nInsertIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		CString strBookKey;
		ids = pRFID_DM->GetCellData(_T("åKEY"), i, strBookKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pBasketDM->SetCellData(_T("åŰ"), strBookKey, nInsertIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		nLoanCnt++;
	}

	if(0 == nLoanCnt)
	{
		SelfCloseMessageBox(_T("������ �ڷᰡ �������� �ʽ��ϴ�."));
		return 1;
	}
	BasketRegNoProc();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::RFID_Return()
{
EFS_BEGIN

	INT ids;
	TCHAR szMsg[256];
	CString strFuncName = _T("RFID_Return");
	
	
	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	ClearBasket();
	
	
	CESL_DataMgr* pRFID_DM = m_pParentMgr->FindDM(_T("DM_RFID���"));
	if(NULL == pRFID_DM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sBasketDMAlias = _T("DM_BO_LOC_3100_BASKET");
	
	
	CESL_DataMgr *pBasketDM = m_pParentMgr->FindDM(sBasketDMAlias);
	if(NULL == pBasketDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			
	CString sPublishForm;	
	CString sLoanYN;	
	CString sRegNo;	
	CString sSerialNo;	
	CString sSql;
	
	CString sTableName;
	CString sUserNo;
	CString sBookKey;
	BOOL	bDelayFlag;
	
	for(INT i = 0 ; i < pRFID_DM->GetRowCount() ; i++)
	{
		bDelayFlag = FALSE;

		ids = pRFID_DM->GetCellData(_T("��������"), i, sPublishForm);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if(sPublishForm.Compare(_T("B")) == 0)
			sTableName = _T("BO_BOOK_TBL");
		else if(sPublishForm.Compare(_T("S")) == 0)
			sTableName = _T("SE_BOOK_TBL");
		else if(sPublishForm.Compare(_T("N")) == 0)
			sTableName = _T("CO_NON_DB_BOOK_TBL");
		else
			sTableName = _T("BO_BOOK_TBL");
		
		ids = pRFID_DM->GetCellData(_T("�ø���"), i, sSerialNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = pRFID_DM->GetCellData(_T("�̿��ڹ�ȣ"), i, sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		ids = pRFID_DM->GetCellData(_T("���⿩��"), i, sLoanYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if(sLoanYN.Compare(_T("0")) == 0)
			continue;			
	
		ids = pRFID_DM->GetCellData(_T("�ݳ�����̿��ڹ�ȣ"), i, sUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		
		CString sTmpUserNo;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("�����ڹ�ȣ"), sTmpUserNo, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		
		if(sTmpUserNo.Compare(sUserNo) != 0)
		{
			SPFExcutePath(_T("ȭ���ʱ�ȭ"));
			
			ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("�����ڹ�ȣ"), sUserNo);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			
			SPFExcutePath(_T("ȸ��_�˻�_NOASK"));	
			if(m_pManageValue->m_nSetloanStopMode)
			{
				return 0;
			}

			sTmpUserNo = sUserNo;
		}	

		
		CESL_DataMgr *pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		if(NULL == pLoanInfoDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		
		if(sTmpUserNo.Compare(sUserNo) == 0)
		{
			ids = pRFID_DM->GetCellData(_T("åKEY"), i, sBookKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			
			ids = BarCodeReturnBookCheck(sBookKey);
			if(0 > ids) return ids;

			CString strIsILL = pRFID_DM->GetCellData(_T("å�η�����"), i);
			BOOL bIsILL = (_T("Y") == strIsILL) ? TRUE : FALSE;

			if(m_pManageValue->m_sFeeMngSys == _T("Y") || m_pManageValue->m_ArrearCheckYN == _T("Y"))
			{
				if(bDelayFlag == FALSE)
				{
					CString strTempBookKey;
					CString sDelayDay;					
										
					for(INT y = 0 ; y<pLoanInfoDM->GetRowCount() ; y++)
					{
						pLoanInfoDM->GetCellData(_T("åKEY"), y, strTempBookKey);					
						if(strTempBookKey == sBookKey)
						{
							ids = pLoanInfoDM->GetCellData(_T("��ü�ϼ�"), y, sDelayDay);
							if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
							if(_ttoi(sDelayDay) > 0) bDelayFlag = TRUE;
							break;
						}					
					}		
				}
				ids = ReturnBook(bDelayFlag, FALSE);
				if(0 > ids) return ids;
			}
			else
			{
				ids = ReturnBook(FALSE, FALSE);
				if(0 > ids) return ids;
			}

			BOOL bIsUpdate = TRUE;
			CString strRfidWorkingStatus = pRFID_DM->GetCellData(_T("å����"), i);

			if(strRfidWorkingStatus != _T("BOL611O")){
				CString strGiveLibCode;
				strGiveLibCode = GetLIB_CODE(pRFID_DM,sBookKey,sTableName);
				if(m_pCheckLoan->IsOtherManageReturn() == FALSE){
					if(strGiveLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
					{
						if ( m_pManageValue->m_strSetRFIDSecuReturnLibYN == _T("N")) {
							bIsUpdate = FALSE;
						}
					}
				}
			}

			if(ids == 0)
			{
				DWORD nSTART,nFINISH;

				nSTART = GetTickCount();
				
			
				RFID_UpdateLoan(bIsUpdate, sSerialNo);
				nFINISH = GetTickCount();

				CString strMsg;
				strMsg.Format(_T("��[��������] �����ڹ�ȣ : %s / ��Ϲ�ȣ : %s / å�η����� : %s / ��� å���� : %s"),sUserNo, sRegNo, strIsILL, strRfidWorkingStatus);
				if(bIsUpdate == TRUE)
				{
					strMsg += _T(" / RFID���� : ����");
				}else{
					strMsg += _T(" / RFID���� : ����");
				}
				pRFID_DM->QueryLog(strMsg.GetBuffer(0));

				strMsg.Format(_T("(Finish) - [%s] �ɸ� �ð� :  [%d.%d sec]"), _T("RFIDTaggin�ð�"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
				
				pRFID_DM->QueryLog(strMsg.GetBuffer(0));
			}
		}
	}
	return 0;
EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::RecordRFIDUserInfo()
{
EFS_BEGIN
	INT	ids;
	CString strFuncName = _T("RecordRFIDUserInfo");

	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	INT nCount = pDM->GetRowCount();

	if(1 > nCount)
	{
		SelfCloseMessageBox(_T("�̿��ڸ� �˻��� �ֽʽÿ�."));
		return 1010;
	}

	CString strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	CString strUserNo = _T("");
	CString strPassWord = _T("");
	CString strEncryptionPassWord = _T("");
	CString strRecKey;
	CString strUserName;	
	CString strKlUserYN;

	ids = pDM->GetCellData(_T("�����ڹ�ȣ"), 0, strUserNo);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);}
	ids = pDM->GetCellData(_T("������KEY"), 0, strRecKey);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);}
	ids = pDM->GetCellData(_T("�������̸�"), 0, strUserName);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);}
	
	ids = pDM->GetCellData(_T("����ȸ������"), 0, strKlUserYN);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);}

	CRFID_Interface RFID(m_pParentMgr);
	INT nTagCnt;
	CStringArray DSFID_LIST;
	CStringArray SERIAL_LIST;
	CString strFuncInfo;
	CString strQuery, strValue;
	
	ids = RFID.GET_STATUS();
	
	CLocCommonAPI::MakeRFIDLog(_T("GET_STATUS()"), _T("RFID ����ȭ"), __WFILE__, __LINE__, ids);
	if(0 > ids)
	{
		SelfCloseMessageBox(_T("�����⸦ ã�� �� �����ϴ�."));
		return 1020;
	}

	ids = RFID.GetTagSerial(SERIAL_LIST);
	nTagCnt = SERIAL_LIST.GetSize();
	
	strFuncInfo.Format(_T("RFID_GetTagSerials(SerialList, %d)"), nTagCnt);
	CLocCommonAPI::MakeRFIDLog(strFuncInfo, _T("RFID Tag ����ȭ"), __WFILE__, __LINE__, ids);

	if(1 != nTagCnt)
	{
		if(0 == nTagCnt)
		{
			SelfCloseMessageBox(_T("ȸ������ �������� �ʽ��ϴ�."));
			return 1030;
		}
		else if(1 < nTagCnt)
		{
			SelfCloseMessageBox(_T("�ϳ��� ȸ������ �÷��ֽʽÿ�."));
			return 1040;
		}	
		
		return 1050;
	}	
	
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	TmpDM.CheckManageCode();
	strQuery.Format(
		_T("SELECT BB.MANAGE_CODE, '�������̺� : '||IB.TITLE_INFO||'('||BB.REG_NO||')', NULL ")
		_T("FROM BO_BOOK_TBL BB, IDX_BO_TBL IB ")
		_T("WHERE BB.SPECIES_KEY=IB.REC_KEY AND BB.WORKNO='%s' AND BB.REC_KEY <> %s ")
		_T("UNION ")
		_T("SELECT SB.MANAGE_CODE, '�������̺� : '||SS.TITLE_INFO||'('||SB.REG_NO||')', NULL ")
		_T("FROM SE_BOOK_TBL SB, IDX_SE_TBL SS ")
		_T("WHERE SB.SPECIES_KEY=SS.REC_KEY AND SB.WORKNO='%s' AND SB.REC_KEY <> %s ")
		_T("UNION ")
		_T("SELECT CLU.MANAGE_CODE, '�̿������̺� : '||CLU.NAME||'('||CLU.USER_NO||')', CLU.WORKNO ")
		_T("FROM CO_LOAN_USER_TBL CLU ")
		_T("WHERE CLU.WORKNO='%s' AND CLU.REC_KEY <> %s")
		, SERIAL_LIST.GetAt(0), strRecKey, SERIAL_LIST.GetAt(0), strRecKey, SERIAL_LIST.GetAt(0), strRecKey);

	ids = TmpDM.RestructDataManager(strQuery);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);}

	INT nRowCnt = 0;	
	const INT ROW_COUNT = TmpDM.GetRowCount();
	const INT COL_CNT = 3;
		
	CString strMsg, strSerial, strManageCode, strSerialList;
	TmpDM.StartFrame();
	if(0 < ROW_COUNT)
	{
		for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
		{
			ids = TmpDM.GetCellData(nRowCnt, 0, strManageCode);
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);}
			ids = TmpDM.GetCellData(nRowCnt, 1, strSerial);
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);}
			
			
			if(0 > strSerial.Find(_T("�̿������̺�")))
			{ 
				if(strManageCode != TmpDM.MANAGE_CODE)
				{
					TmpDM.EndFrame();
					strMsg.Format(_T("�� �±״� �ٸ� ���������� ��ϵ� �±��̱� ������ ����� �� �����ϴ�."));
					SelfCloseMessageBox(strMsg);
					return 1060;
				}
			}
			strSerialList = strSerialList + _T("\r\n") + strSerial;
		}
		
		strMsg.Format(_T("���� ������� �±��Դϴ�. �ٽ� �Է��Ͻðڽ��ϱ�?\r\n%s"), strSerialList);
		if(IDYES == AfxMessageBox(strMsg, MB_YESNO)) 
		{
			
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKNO=NULL WHERE WORKNO='%s';"), SERIAL_LIST.GetAt(0));
			TmpDM.AddFrame(strQuery);
			strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKNO=NULL WHERE WORKNO='%s';"), SERIAL_LIST.GetAt(0));
			TmpDM.AddFrame(strQuery);

			CString strWorkNo;
			ids = TmpDM.GetCellData(0, 2, strWorkNo);
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, strFuncName);}
			
			if(TRUE != strWorkNo.IsEmpty())
			{ 
				if(SERIAL_LIST.GetAt(0) == strWorkNo)
				{
					strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO=NULL WHERE WORKNO='%s';"), strWorkNo);
				}
			}
			else
			{
				if(SERIAL_LIST.GetAt(0) == strWorkNo)
				{
					strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO=NULL, CARD_PASSWORD=NULL WHERE WORKNO='%s';"), strWorkNo);
				}				
			}
			TmpDM.AddFrame(strQuery);
			ids = TmpDM.SendFrame();
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, strFuncName);}
			TmpDM.EndFrame();
		}
		else
		{
			TmpDM.EndFrame();
			return 1070;
		}
	}

	TmpDM.EndFrame();

	strQuery.Format(_T("SELECT WORKNO FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), strRecKey);
	ids = pDM->GetOneValueQuery(strQuery, strValue);
	if(0 > ids && -4007 != ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);}
	
	CInsertPasswordDlg InsertPasswordDlg(m_pParentMgr);
	if(!strValue.IsEmpty())
	{
		if(strValue != SERIAL_LIST.GetAt(0))
		{
			InsertPasswordDlg.m_nViewMode = 1;
		}
	}

	InsertPasswordDlg.DoModal();

	INT nCloseMode = InsertPasswordDlg.GetCloseMode();
	if(nCloseMode == 0)
	{
		return 0;
	}
	strPassWord = InsertPasswordDlg.GetPassword();

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bCardPwEncryption && 64 > strPassWord.GetLength())
	{
		strEncryptionPassWord = CLocCommonAPI::StringToSHA256(strPassWord);
	}

	CString strUserNoAndName;
	CString sTempUserName;
	if(strUserName.GetLength() > 8)
	{
		sTempUserName = strUserName.Mid(0,8);
	}
	else
	{
		sTempUserName = strUserName;
	}
	strUserNoAndName.Format(_T("%s:%s"), strUserNo, sTempUserName);

	
	CString strRFID_Serial;	
	ids = RecordRFIDFromUserInfo(strLibCode, strUserNoAndName, strPassWord, strRecKey, strRFID_Serial);
	if(0 > ids)
	{
		strMsg.Format(_T("��Ͽ� �����Ͽ����ϴ�."), strUserNo, strUserName);
		SelfCloseMessageBox(strMsg);
		return 1;
	}
	else if(1 == ids)
	{
		return 1;
	}
	else if(2 == ids)
	{
		return 2;
	}
	else if(3 == ids)
	{
		SelfCloseMessageBox(_T("å�� �˻��Ǿ� ���� �� �� �����ϴ�."));
		return 3;
	}
	
	CESL_DataMgr UpdateDM;
	UpdateDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	CKolas2up_MobileApi* pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, &UpdateDM);
	if(NULL == pMobileApi){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1140, strFuncName);}
	pMobileApi->StartFrame();
	pMobileApi->InitDBFieldData();

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bCardPwEncryption && 64 > strPassWord.GetLength()) {
		pMobileApi->AddDBFieldData(_T("CARD_PASSWORD"), _T("STRING"), strEncryptionPassWord);
	}
	else {
		pMobileApi->AddDBFieldData(_T("CARD_PASSWORD"), _T("STRING"), strPassWord);
	}
	
	pMobileApi->AddDBFieldData(_T("WORKNO"), _T("STRING"), strRFID_Serial);
	ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1150, strFuncName);}
	ids = pMobileApi->SendFrame();
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1160, strFuncName);}
	pMobileApi->EndFrame();
	CLocCommonAPI::MobileApiEndLog(pMobileApi);
	
	if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse && _T("Y") == strKlUserYN){
		CFrameWriter Writer(m_pParentMgr->m_pInfo);
		CString strModifyDate;
		CTime t = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strModifyDate);
		Writer.setCommand(_T("IL_K24_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
		Writer.addElement(_T("ȸ����RFID"), SERIAL_LIST.GetAt(0));
		Writer.addElement(_T("��������"), strModifyDate);
		Writer.addElement(_T("������������ȣ"), m_pParentMgr->m_pInfo->LIB_CODE);
		
		CFrameSender Sender(m_pParentMgr->m_pInfo);
		CFrameReader Reader;
		Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg( _T("����RFID���"), __WFILE__, __LINE__ ));	
	}
	
	strMsg.Format(_T("�̿��ڹ�ȣ : [ %s ],  �̸� : [ %s ]\r\n���������� ����Ͽ����ϴ�."), strUserNo, strUserName);
	SelfCloseMessageBox(strMsg, m_pParentMgr, 4000);

	return 0;

EFS_END	
return -1;
}
INT CBL_LOC_LOAN_PROC::RecordRFIDFromUserInfo(CString strLibCode, CString strUserNo, CString strPassWord, CString strRecKey, CString &strRFID_Serail)
{
EFS_BEGIN
	INT ids;
	CRFID_Change RFID_Job;
	RFID_Job.m_pParentMgr = m_pParentMgr;
	ids = RFID_Job.RecordRFIDFromUserInfo(strLibCode, strUserNo, strPassWord, strRFID_Serail);
	return ids;

EFS_END
return -1;
}

CString CBL_LOC_LOAN_PROC::GetLIB_CODE(CESL_DataMgr *pDM,CString sBookKey,CString sTableName)
{
	EFS_BEGIN
	
	CString strQuery, strResult = _T(""),strTemp;


	if(sTableName == _T("")){
 		strQuery.Format(_T("SELECT MANAGE_CODE FROM BO_BOOK_TBL WHERE REC_KEY = %s ")
						_T(" UNION ")
						_T("SELECT MANAGE_CODE FROM SE_BOOK_TBL WHERE REC_KEY = %s ")
						,sBookKey,sBookKey);
		pDM->GetOneValueQuery(strQuery, strTemp);

		if(strTemp == _T(""))
			return _T("");

 		strQuery.Format(_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE ")
						_T("MANAGE_CODE='%s'")
						, strTemp);

	}else{
 		strQuery.Format(_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE ")
						_T("MANAGE_CODE=(SELECT MANAGE_CODE FROM %s WHERE REC_KEY=%s)")
						, sTableName, sBookKey);
	}
	pDM->GetOneValueQuery(strQuery, strResult);
		return strResult;

	EFS_END
		return _T("");
}

INT CBL_LOC_LOAN_PROC::ILL_ReservationLoan()
{
	EFS_BEGIN

	CString strFuncName = _T("ILL_ReservationLoan");
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(!pCM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(!pGrid)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
	pGrid->SelectMakeList();

	INT nRowCount = pGrid->SelectGetCount();
	if(!nRowCount)
	{
		MakeRegNoSpaceToZero(FALSE);

		CString sRegNo;
		IsRegNo(sRegNo);

		if(FALSE == sRegNo.IsEmpty())
		{
			BarCodeReturnBookCheck(sRegNo, 1);
		}

		pGrid->SelectMakeList();
		nRowCount = pGrid->SelectGetCount();
		if(!nRowCount)
		{
			SelfCloseMessageBox(_T("������ å�� ������ �ֽʽÿ�"));
			return 0;
		}		
	}
	
	BOOL IsLoan = FALSE;	
	INT ids = IsLoanMember(IsLoan);
	if(0 > ids) return ids;
	if(!IsLoan) return 0;
	
	INT nIndex;
	CString sStatus, sIsAppendix, sRegNo, strSQL, strLoanKey, strAplLibCode;
	INT nProcessingCnt = 0;
	pGrid->SelectGetHeadPosition();

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	for (INT i = 0 ; i < nRowCount ; i++)
	{
		nIndex = pGrid->SelectGetIdx();
		
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("�������"), sStatus, nIndex);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
		}
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("����/�ݳ�����KEY"), strLoanKey, nIndex);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
		}
				
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("��Ϲ�ȣ"), sRegNo, nIndex);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, strFuncName);
		}

		if(sStatus.Compare(_T("O")))
		{
			pGrid->SelectGetNext();
			continue;
		}

		
		CString strBookManageCode;
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("��������"), strBookManageCode, nIndex);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1070, strFuncName);
		}
		

		ids = IsUserBookLoan(EXP_LOAN, TRUE, strBookManageCode);
		if(ids > 0)
		{
			pGrid->SelectGetNext();
			continue;			
		}
		

		strSQL.Format(_T("SELECT APL_LIBCODE FROM ILL_TRANS_TBL WHERE LOAN_KEY = %s"), strLoanKey);
		TmpDM.GetOneValueQuery(strSQL, strAplLibCode);

		if(strAplLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
		{
			CString sEMsg;			
			if(sRegNo.IsEmpty())			
			{
				sEMsg.Format(_T("Ÿ�� �����ڷ��Դϴ�."));
			}
			else
			{
				sEMsg.Format(_T("��Ϲ�ȣ['%s'] : Ÿ�� �����ڷ��Դϴ�."), sRegNo);
			}
			
			SelfCloseMessageBox(sEMsg);
			pGrid->SelectGetNext();
			continue;
		}

		this->SetBEGIN_SBL();
		
		ids = ReservationDBProc(nIndex, TRUE);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, strFuncName);
		}
			
		if(ids == 100 || ids == 101)
		{
			pGrid->SelectGetNext();
			continue;
		}			
		
		ids = ReservationScreen(nIndex);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1090, strFuncName);
		}

		nProcessingCnt++;
		pGrid->SelectGetNext();
		
		
		ids = InitExpDM();

	if(0 > ids) return ids;
	}
	if(nProcessingCnt > 0) 
	{
		m_bReturnBookNonFree = TRUE;
		ids = SearchLoanBookInfo();
		if(0 > ids) return ids;
		ids = AddMemberInfo();
		if(0 > ids) return ids;
		
		ids = ViewLoanScreen();
		if(0 > ids) return ids;
		CString sMsg;
		sMsg.Format(_T("å�� ���� �Ǿ����ϴ�."),nProcessingCnt);
		SelfCloseMessageBox(sMsg);

		CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		INT nLoanRowCount = pDM->GetRowCount();
		this->SetEND_SBL(nLoanRowCount);

	}

	return 0;	

	EFS_END
		return -1;
}

INT CBL_LOC_LOAN_PROC::RFID_UpdateLoan(INT nMode, CString sSerialNo)
{
EFS_BEGIN
		
	INT				ids=0, nCnt=0;
	CRFID_Interface RFID(this->m_pParentMgr);	
	CString			sFuncInfo;

	
	for( nCnt = 0; nCnt < 2; nCnt++)
	{
		
		if(2 == nMode)	{ ids = RFID.ChangeTagLoanProcDataByUID(sSerialNo, nMode);	}
		else			{ ids = RFID.ChangeTagLoanProcDataByUID(sSerialNo, !nMode);	}

		if(0 == ids) break;
	}
	sFuncInfo.Format(_T("RFID_UpdateLoan 1-1 (%s, %d)"), sSerialNo, nMode);
	CLocCommonAPI::MakeRFIDLog(sFuncInfo, _T("RFID ����/���Ȼ��� ������Ʈ"), __WFILE__, __LINE__, ids);

	return ids;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::MoveGrid(CString strBookKey)
{
	EFS_BEGIN

	INT ids = 0;
	CString strFuncName = _T("MoveGrid");

	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	INT nRowCount = pDM->GetRowCount();

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_Grid* pGrid = (CESL_Grid*)(m_pParentMgr->FindControl( sCMAlias , sGridAlias ));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sWorkingStatus;
	for( INT i = nRowCount-1 ; i > -1 ; i-- )
	{
		CString strValue;
		ids = pDM->GetCellData( _T("åKEY"), i , strValue );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if( strValue.Compare(strBookKey)==0 )
		{
			CESL_DataMgr* pReturnBasket = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_RETURN_INFO"));	
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			INT nInsertIdx = pReturnBasket->GetRowCount();
			pReturnBasket->InsertRow(-1);

			INT nColCnt = pReturnBasket->GetRefColumnCount();
			for( INT j=0; j<nColCnt; j++ )
			{
				CString strField_Alias, strField_Name, strField_Type, strInit_Value, strData_Type;
				CString strResult;

				ids = pReturnBasket->GetColumnRefInfo(j, strField_Alias, strField_Name, strField_Type, strInit_Value, strData_Type);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

				ids = pDM->GetCellData(strField_Alias, i, strResult);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

				ids = pReturnBasket->SetCellData(strField_Alias, strResult, nInsertIdx);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			}			

			
			if( m_pParentMgr->m_pInfo->MODE == 10000 )
			{
				ids = pReturnBasket->SetCellData(_T("R_DEVICE"), _T("KOLASIII-�̵�������"), nInsertIdx);
			}
			else
			{
				ids = pReturnBasket->SetCellData(_T("R_DEVICE"), _T("KOLASIII"), nInsertIdx);
			}
			if(0 > ids)
			{
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			}

			
			ids = pReturnBasket->SetCellData(_T("R_WORKER"), m_pParentMgr->m_pInfo->USER_ID, nInsertIdx);
			if(0 > ids) 
			{
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			}
			pDM->DeleteRow(i);
			if( pDM->GetRowCount() != 0 )
			{
				((CMSHFlexGrid*)pGrid)->RemoveItem(i+1);
				
				
				CString sIndex;
				for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
				{
					sIndex.Format(_T("%d"),i+1);
					pGrid->SetTextMatrix( i+1 , 0 , sIndex);
				}
			}
		}
	}

	if( pDM->GetRowCount() == 0 )
	{
		m_pParentMgr->AllControlDisplay( _T("CM_BO_LOC_3100_LOAN_INFO") );
	}
	else
	{
		m_pParentMgr->ControlDisplay(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid2"));
	}

	CESL_Grid* pGrid2 = (CESL_Grid*)(m_pParentMgr->FindControl( sCMAlias , _T("MainGrid2") ));
	pGrid2->SendMessage(WM_VSCROLL, SB_BOTTOM, 0 );
	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_PROC::ISIllLoanMat(CString strLoanKey, BOOL& bIllReturnWill)
{
EFS_BEGIN
	INT ids;
	CESL_DataMgr TmpDM;
	TCHAR szSql[128];
	CString strValue=_T("");
	CString strFuncName = _T("ISIllLoanMat");

	bIllReturnWill = FALSE;

	
	if ( strLoanKey.GetLength() == 0 ) return 0;

	TmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	_stprintf( szSql, _T("SELECT IS_ILL_RETURN(%s, '%s', '', '' , '', '', '0') FROM DUAL"), strLoanKey, m_pParentMgr->m_pInfo->MANAGE_CODE);

	ids = TmpDM.GetOneValueQuery(szSql, strValue);
	if(0 > ids && ids != -4007){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);}

	
	if ( strValue != _T("0") ) {
		bIllReturnWill = FALSE;	
		return 0;
	}

	bIllReturnWill = TRUE;	
	return 0;	

EFS_END
	return -1;
}


INT CBL_LOC_LOAN_PROC::ISIllProcMat(CString strLoanKey, BOOL& bIllProcMat)
{
EFS_BEGIN
	INT ids;
	CESL_DataMgr TmpDM;
	TCHAR szSql[512];
	CString strValue=_T("");
	CString strFuncName = _T("ISIllProcMat");

	bIllProcMat = FALSE;

	TmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	_stprintf( szSql, _T("SELECT WORKING_STATUS FROM BO_BOOK_TBL ")
		              _T("WHERE REC_KEY=(SELECT L.BOOK_KEY FROM LS_WORK_T01 L, ILL_TRANS_TBL I WHERE L.REC_KEY=I.LOAN_KEY AND L.PUBLISH_FORM_CODE='MO' AND REC_KEY=%s AND ROWNUM < 2) ")
					  _T("UNION ")
					  _T("SELECT WORKING_STATUS FROM SE_BOOK_TBL ")
		              _T("WHERE REC_KEY=(SELECT L.BOOK_KEY FROM LS_WORK_T01 L, ILL_TRANS_TBL I WHERE L.REC_KEY=I.LOAN_KEY AND L.PUBLISH_FORM_CODE='SE' AND REC_KEY=%s AND ROWNUM < 2) ")
					  , strLoanKey, strLoanKey);

	ids = TmpDM.GetOneValueQuery(szSql, strValue);
	if(0 > ids && ids != -4007){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);}

	
	if ( strValue == _T("BOL411O") || strValue == _T("SEL411O") ) {
		bIllProcMat = TRUE;			
	}
	
	return 0;	

EFS_END
	return -1;
}


INT CBL_LOC_LOAN_PROC::ISUnionDBLoan(CString strBarcode, CString strLibCode, BOOL& bUnionDBLoan)
{
	EFS_BEGIN
	INT ids;
	CESL_DataMgr TmpDM;
	TCHAR szSql[512];
	CString strValue=_T("");
	CString strFuncName = _T("ISUnionDBLoan");
	
	bUnionDBLoan = FALSE;	
	
	
	if ( strBarcode.GetLength() < 12 || strLibCode.GetLength() < 6 ) return 0;

	return 0;	
	
	EFS_END
		return -1;
}


INT CBL_LOC_LOAN_PROC::KLService11(CString strUserNo,     CString strBookLibCode, 
								   CString strReturnDate, CString strRetrunStatus,
								   CString strLoanKey,    CString strUserStatus, 
								   CString strLoanStopDate,  CString strLoanStopSetDate )
{
	
	
	CFrameWriter Writer(m_pParentMgr->m_pInfo);
	Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
	Writer.addRecord();	
	Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
	
	
	Writer.addElement(_T("���⵵������ȣ"), strBookLibCode);
	Writer.addElement(_T("�ݳ���������ȣ"), strBookLibCode);
	
	
	Writer.addElement(_T("�ݳ���"), strReturnDate);			
	Writer.addElement(_T("����"), strRetrunStatus);
	Writer.addElement(_T("����KEY"), strLoanKey);

	if ( strUserStatus != _T("0"))	
	{
		Writer.addElement(_T("ȸ������"), strUserStatus);
		if (strLoanStopDate == _T("NULL")) {
			Writer.addElement(_T("����������"), _T(""));
		}
		else Writer.addElement(_T("����������"), strLoanStopDate);
		Writer.addElement(_T("���������ο���"), strLoanStopSetDate);
	}	
	else {
		Writer.addElement(_T("ȸ������"), strUserStatus);
	}
	Writer.addElement(_T("��������ȣ��������"), _T("Y"));
	
	
	CFrameSender Sender(m_pParentMgr->m_pInfo);
	Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("����ȸ��å�η��ݳ�"), __WFILE__, __LINE__));

	return 0;	
}

INT CBL_LOC_LOAN_PROC::ILL_ReservationConvertToAPL()
{
EFS_BEGIN
		
	INT ids;
	CString strFuncName = _T("DelayLoanDate");
	
	CString sDMAlias	= _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias	= _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias	= _T("MainGrid");
	CString sTableName	= _T("LS_WORK_T01");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	ids = pGrid->SelectMakeList();
	INT nIdx;
	nIdx = pGrid->SelectGetHeadPosition();
	INT nRowCount = pGrid->SelectGetCount();
	if(1 > nRowCount)
	{
		SelfCloseMessageBox(_T("������ ���� �ڷᰡ �����ϴ�."));
		return 0;
	}
	if(1 != nRowCount)
	{
		SelfCloseMessageBox(_T("1���� �̷¸� �������ּ���."));
		return 0;
	}
		
	CString strStatus, strLoanManageCode;
	strStatus = pDM->GetCellData(_T("�������"), nIdx);
	strLoanManageCode = pDM->GetCellData(_T("��������"), nIdx);
	if(_T("3") != strStatus)
	{
		
		SelfCloseMessageBox(_T("�������� �ڷḸ �������ּ���."));
		
		return 0;
	}
		
	CString strRequestLibCode = _T("");
	CString strRequestLibName = _T("");
	CString strRequestManageCode = _T("");
	BOOL bLocalRequest = FALSE;
	
	CString strBookLibCode = _T("");
	CString strBookKey = _T("");
	strBookLibCode = pDM->GetCellData(_T("�������������ȣ"), nIdx);
	strBookKey = pDM->GetCellData(_T("åKEY"), nIdx);	

	CILL_RESERVE_CONVERT_LIB_DLG dlg(m_pParentMgr);
	dlg.SetBookParm(strBookLibCode, strBookKey);
	if(IDOK != dlg.DoModal())
	{
		SelfCloseMessageBox(_T("��ҵǾ����ϴ�."));
		return 0;
	}
	else
	{
		bLocalRequest = TRUE;
		strRequestLibCode = dlg.m_strRequestLibCode;
		strRequestManageCode = dlg.m_strRequestManageCode;
		strRequestLibName = dlg.m_strRequestLibName;
	}

	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	
	CILL_APL_3100 Dlg(m_pParentMgr);
	Dlg.m_pInfo = m_pParentMgr->m_pInfo;
	
	Dlg.m_nMode = 2;
	Dlg.m_strReservationKey = pDM->GetCellData(_T("����/�ݳ�����KEY"), nIdx);
	Dlg.m_strUserNo = pUserDM->GetCellData(_T("�����ڹ�ȣ"), 0);
	Dlg.m_strUserNo2 = pUserDM->GetCellData(_T("�����ڹ�ȣ"), 0);
	Dlg.m_strUserKey = pUserDM->GetCellData(_T("������KEY"), 0);
	Dlg.m_strUserEmail = pUserDM->GetCellData(_T("�̸���"), 0);
	Dlg.m_strUserCode = pUserDM->GetCellData(_T("����������"), 0);
	Dlg.m_strUserName = pUserDM->GetCellData(_T("�������̸�"), 0);
	
	Dlg.m_strBookKey = pDM->GetCellData(_T("åKEY"), nIdx);
	Dlg.m_strBookTitle = pDM->GetCellData(_T("��ǥ��"), nIdx);
	Dlg.m_strBookRegNo = pDM->GetCellData(_T("��Ϲ�ȣ"), nIdx);
	Dlg.m_strPublishFormCode = pDM->GetCellData(_T("����/���ӱ���"), nIdx);
	Dlg.m_strBookManageCode = strLoanManageCode;
	
	Dlg.m_strRequestLibName = strRequestLibName;
	Dlg.m_strRequestLibCode = strRequestLibCode;

	if(IDCANCEL == Dlg.DoModal())
	{
		
		if(Dlg.m_bAplOK)
		{
			ReservationCancel(TRUE);
			SelfCloseMessageBox(_T("å�η� ��û ��ȯó���� �Ϸ�Ǿ����ϴ�."));
		}
	}

	return 0;
EFS_END
return 0;
}

INT CBL_LOC_LOAN_PROC::GetTransNo(CString &sTransNo)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetTransNo");

	CESL_DataMgr TmpDM;
  	TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	CString sQuery;
 	sQuery.Format(	_T("SELECT ECO$F_GET_TRANS_NO$S('%s','%s') FROM DUAL")
 					, m_pParentMgr->m_pInfo->USER_ID, m_pParentMgr->m_pInfo->MANAGE_CODE);
 	ids = TmpDM.GetOneValueQuery(sQuery, sTransNo);
 	if(0 > ids || sTransNo.IsEmpty()) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(_T("MAX LAST_NUMBER COUNT") == sTransNo)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	else if(_T("LIB_CODE IS NOT FOUND") == sTransNo) 
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::BookReturnTypeCheck(INT nIdx)
{

	CString strFuncName = _T("BookReturnTypeCheck");
	CString strManageDesc,								
			strMessage,									
			strBookLibCode,								
			strReturnLibCode,							
			strKLBookLibCode,							
			strLocal,									
			strTemp,
			strTemp2,
			strReturnOption								
			;
	
	BOOL	bBookLibILLUse					= FALSE,	
			bBookLibKLUse					= FALSE,	
			bReturnLibILLUse				= FALSE,	
			bReturnLibKLUse					= FALSE,	
			bReturnLibILLOtherReturnUse		= FALSE,	
			bKLUser							= FALSE,	
			bLILLUse						= FALSE,	
			bLILLData						= FALSE,	
			bILLData						= FALSE,	
			bBookLibOtherReturnUse			= FALSE,	
			bBookLibOtherReturnHoldUse		= FALSE,	
			bReturnLibOtherReturnUse		= FALSE,	
			bReturnLibOtherReturnHoldUse	= FALSE,	
			bOtherReturnOptionUse			= FALSE
			;
	
	int i = 0,
		idx = 0,
		nLoanStatus = 0;

	CESL_DataMgr	*pDM		= m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	CESL_DataMgr	*pMemberDM	= m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	CESL_ControlMgr *pCM		= m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_SEARCH_INFO"));


	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1001, strFuncName);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1002, strFuncName);

	if(pDM->GetRowCount() == 0){
		
		pDM			= m_pParentMgr->FindDM(_T("DM_LR2_BO_BOOK_LIST"));
		pMemberDM	= m_pParentMgr->FindDM(_T("DM_LR2_BO_BOOK_LIST"));
		idx = nIdx;
		if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1003, strFuncName);
		if(pDM->GetRowCount() == 0){
			return _RETURN_FAIL;
		}
	}

	if(pMemberDM->GetRowCount() == 0){
		return _RETURN_FAIL;
	}
	
	strBookLibCode		 = pDM->GetCellData( _T("�������������ȣ"), nIdx);						
	strReturnLibCode	 = m_pParentMgr->m_pInfo->LIB_CODE;

	
	bBookLibILLUse				 = m_pManageValue->GetLibDMValue(strBookLibCode, _T("ILL_USE_YN"));
	bBookLibOtherReturnUse		 = m_pManageValue->GetLibDMValue(strBookLibCode, _T("�а��ݳ���뿩��"));
	bBookLibOtherReturnHoldUse	 = m_pManageValue->GetLibDMValue(strBookLibCode, _T("�а��ݳ�����뿩��"));
	bBookLibKLUse				 = m_pManageValue->GetLibDMValue(strBookLibCode, _T("å������뿩��"));

	bReturnLibILLUse			 = m_pManageValue->GetLibDMValue(strReturnLibCode, _T("ILL_USE_YN"));
	bReturnLibILLOtherReturnUse	 = m_pManageValue->GetLibDMValue(strReturnLibCode, _T("ILL_OTHER_RETURN_YN"));
	bReturnLibOtherReturnUse	 = m_pManageValue->GetLibDMValue(strReturnLibCode, _T("�а��ݳ���뿩��"));
	bReturnLibOtherReturnHoldUse = m_pManageValue->GetLibDMValue(strReturnLibCode, _T("�а��ݳ�����뿩��"));
	bReturnLibKLUse				 = m_pManageValue->GetLibDMValue(strReturnLibCode, _T("å������뿩��"));

	m_pManageValue->GetLibDMValue(strReturnLibCode, _T("Ÿ���ݳ�����"), strReturnOption);
	if(strReturnOption.IsEmpty() == TRUE)
		strReturnOption = _T("KL");

	strTemp				 = pDM->GetCellData( _T("å�η������Ϳ���"), nIdx);
	if(strTemp.Compare(_T("Y")) == 0)	 
		bILLData = TRUE;

	
	strTemp				 = pMemberDM->GetCellData( _T("����ȸ������"), idx);			
	if(strTemp.Compare(_T("Y")) == 0)		 
		bKLUser = TRUE;

	strKLBookLibCode	 = pDM->GetCellData( _T("���⵵������ȣ"), nIdx);	

	
	bLILLUse			 = m_pParentMgr->m_pInfo->m_bLillUse;
	
	strTemp				 = pDM->GetCellData( _T("LCT_KEY"), nIdx);						
	if(strTemp.IsEmpty() == FALSE)		 
		bLILLData = TRUE;

	
	strManageDesc		 = pDM->GetCellData( _T("�������м���"), nIdx);				
	strLocal			 = pDM->GetCellData( _T("��Ȳ����"), nIdx);
	
	CString strLillOtherReturnYN = _T("");
	BOOL bLillOtherReturnYN = FALSE;
	GetTempData(_T("������ȣ����Ÿ���ݳ�����"),strLillOtherReturnYN);
	bLillOtherReturnYN = strLillOtherReturnYN == _T("Y");

	if(bLILLData == TRUE){
		nLoanStatus = _STATUS_LILL;
	}else if(bILLData == TRUE){
		nLoanStatus = _STATUS_ILL;
	}else if(strLocal.Compare(_T("����")) != 0){
		nLoanStatus = _STATUS_KL;
	}else{
		nLoanStatus = _STATUS_NORMAL;
	}

	if(nLoanStatus == _STATUS_LILL){
		if(bReturnLibKLUse == TRUE && bKLUser == TRUE){
			if(strReturnLibCode.Compare(strBookLibCode) == 0){
				return _RETURN_LILL_TO_BOOKLIB;
			}else{
				
				if(bLillOtherReturnYN == TRUE){
					return _RETURN_LILL_TO_LILL;
				} else {
					return _MESSAGE_NO_USE_KL_LILL;
				}
			}
		}else{
			return _MESSAGE_NO_USE_KL_LILL;

		}
	}

	if(nLoanStatus == _STATUS_ILL){
		if(bReturnLibILLUse == TRUE){
			if(strReturnLibCode.Compare(strBookLibCode) == 0){
				return _RETURN_ILL_TO_BOOKLIB;
			}else{
				return _RETURN_ILL_TO_ILL;
			}
		}else{
			return _MESSAGE_NO_USE_ILL;
		}
	}

	if(nLoanStatus == _STATUS_KL){
		if(bReturnLibKLUse == TRUE && bKLUser == TRUE){
			if(strReturnLibCode.Compare(strKLBookLibCode) == 0){
				return _RETURN_KL_TO_BOOKLIB;
			}else{
				
				if(bLillOtherReturnYN == TRUE){
					return _RETURN_KL_TO_KL;
				} else {
					return _MESSAGE_NO_USE_KL;
				}
			}
		}else{
			return _MESSAGE_NO_USE_KL;
		}
	}
	
	CButton *pOtherCheck = (CButton*)pCM->FindControl(_T("Ÿ���ݳ��켱"));
	if(&pOtherCheck != NULL){
		if(TRUE == pOtherCheck->GetCheck()){
			bOtherReturnOptionUse = TRUE;
		}
	}	
	if(nLoanStatus == _STATUS_NORMAL){	
		if(strReturnLibCode.Compare(strBookLibCode) == 0){ 
			return _RETURN_NORMAL; 

		}else{
			if(bReturnLibILLUse == TRUE && bBookLibILLUse == TRUE && bReturnLibILLOtherReturnUse == TRUE){ 
				if(bOtherReturnOptionUse == TRUE && strReturnOption.Compare(_T("KL")) == 0 &&
					bReturnLibKLUse == TRUE && bBookLibKLUse == TRUE && bKLUser == TRUE	
					
					&& bLillOtherReturnYN == TRUE
					){
					return _RETURN_NORMAL_TO_KL; 
				}else{
					return _RETURN_NORMAL_TO_ILL; 
				}

			}else if(bReturnLibKLUse == TRUE && bBookLibKLUse == TRUE && bKLUser == TRUE 
				
				&& bLillOtherReturnYN == TRUE
				){ 
				if(bOtherReturnOptionUse == TRUE && strReturnOption.Compare(_T("ILL")) == 0 &&
					bReturnLibILLUse == TRUE && bBookLibILLUse == TRUE && bReturnLibILLOtherReturnUse == TRUE){
					return _RETURN_NORMAL_TO_ILL; 
				}else{
					return _RETURN_NORMAL_TO_KL; 
				}
			}else if(bReturnLibOtherReturnUse == TRUE && bBookLibOtherReturnUse == TRUE){ 
				if(bReturnLibOtherReturnHoldUse == TRUE && bBookLibOtherReturnHoldUse == TRUE){ 
					return _RETURN_OTHER_MANAGE_HOLD;	

				}else{
					return _RETURN_NORMAL; 

				}
			}
		}
	}

	return _RETURN_FAIL;
}

INT CBL_LOC_LOAN_PROC::BookReturnTypeCheck(CString strInputRegNo)
{
	int i = 0, ids = _RETURN_FAIL;
	CString strDMRegNo, strRegNo, strLogMsg;
	CESL_DataMgr	*pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));

	if(pDM->GetRowCount() == 0){

		return _RETURN_FAIL;
	}

	if(strInputRegNo.GetLength() > 12){
		strRegNo = strInputRegNo.Right(12);
	}else if(strInputRegNo.GetLength() < 12){
		strRegNo = strInputRegNo;
		CLocCommonAPI::MakeRegNoSpaceToZero(strRegNo);
	}else{
		strRegNo = strInputRegNo;
	}

	strLogMsg.Format(_T("�Է�:%s / ��ȯ:%s\r\n"), strInputRegNo, strRegNo);
	pDM->QueryLog(strLogMsg.GetBuffer(0));

	for(i=0;i<pDM->GetRowCount();i++){
		strDMRegNo = pDM->GetCellData( _T("��Ϲ�ȣ"), i);	

		if(strDMRegNo.Compare(strRegNo) == 0){
			ids = BookReturnTypeCheck(i);
			break;
		}
	}

	return ids;
}


int CBL_LOC_LOAN_PROC::ILL_UpdateTransInfo(INT bIsModeI, CString strData)
{
	CString strFuncName = _T("ILL_UpdateTransInfo");
	CString strTransNo;
	CString strLoanKey;
	CString strUpdateKeyAlias;
	INT ids = 0;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
	if(NULL == pDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	
	if(TRUE == bIsModeI)
	{
		CESL_DataMgr* pLOAN_DM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_BOOK_INFO"));
		if(NULL == pLOAN_DM)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
	
		ids = pLOAN_DM->GetCellData(_T("����/�ݳ�����KEY"), 0, strLoanKey);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		}

		strTransNo = strData;
		strData = strLoanKey;
	}
	else
	{
		strLoanKey = strData;
		CString strSQL;
		strSQL.Format(_T("SELECT TRANS_NO FROM ILL_TRANS_TBL WHERE LOAN_KEY = %s"), strLoanKey);
		ids = pDM->GetOneValueQuery(strSQL, strTransNo);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
		}
	}

	CString strSQL;
	strSQL.Format(
		_T("UPDATE ILL_TRANS_TBL ITL ")
		_T("SET ")
		_T("(ITL.LS_LOAN_DATE, ITL.RETURN_PLAN_DATE, ITL.LAST_WORK, ITL.LOAN_KEY) = ")
		_T("(SELECT LS.LOAN_DATE, LS.RETURN_PLAN_DATE, '%s', LS.REC_KEY FROM LS_WORK_T01 LS WHERE LS.REC_KEY = %s)")
		_T("WHERE ITL.TRANS_NO = '%s';"),
		m_pParentMgr->GetWorkLogMsg(_T("����"), __WFILE__, __LINE__),
		strData, strTransNo);
	
	pDM->AddFrame(strSQL);

	SetTempData(_T("SHARE_ILL_TRANS_TBL_INSERT_TRANS_NO"), CComVariant(strTransNo));

	return 0;
}


INT CBL_LOC_LOAN_PROC::ILL_BarCodeReturn(CString sBookKey)
{
	int ids = 0;
	INT i, j;
	CString strFuncName = _T("ILL_BarCodeReturn");

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	CString strQuery = _T("");
	CString strAppendixFlag = _T("");
	CString strDevice = _T("KOLASIII");
	CString strWorker = m_pParentMgr->m_pInfo->USER_ID;
	CString strLoanLibManageCode = _T("");

	CString sReturnType;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("�ݳ�����"), sReturnType);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);	
	
	CString sReturnDate, strReturnDateString;
	CString sReturnDateControlAlias = _T("�ݳ���");
	if(sReturnType.Compare(_T("2"))== 0)
		sReturnDateControlAlias = _T("�ұ޹ݳ���");
	
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, strReturnDateString);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	sReturnDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strReturnDateString);

	CESL_DataMgr TempDM;
	TempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	
	strQuery.Format(_T("SELECT CL.USER_NO, (SELECT ML.LIB_CODE FROM MN_LIBINFO2_TBL ML, BO_BOOK_TBL BB WHERE BB.REC_KEY = %s AND ML.MANAGE_CODE = BB.MANAGE_CODE AND ROWNUM < 2), ")
		
		_T("LW.REC_KEY, LW.LOAN_DATE, LW.RETURN_PLAN_DATE, LW.USER_KEY, LW.BOOK_KEY, LW.PUBLISH_FORM_CODE, ")
		
		_T("LW.REG_NO, LW.CALL_NO, LW.TITLE, LW.AUTHOR, LW.PUBLISHER, BB.VOL_TITLE, BB.BOOK_APPENDIX_FLAG, TO_CHAR(SYSDATE, 'YYYY/MM/DD'), CL.KL_MEMBER_YN ")
		
		_T(", (SELECT APL_LIBCODE FROM ILL_TRANS_TBL IT WHERE LW.REC_KEY=IT.LOAN_KEY AND ROWNUM =1 )")
		_T("FROM CO_LOAN_USER_TBL CL, LS_WORK_T01 LW, BO_BOOK_TBL BB ")
		_T("WHERE CL.REC_KEY = LW.USER_KEY AND LW.BOOK_KEY = BB.REC_KEY AND LW.STATUS IN ('0','2','L','D') AND LW.BOOK_KEY = %s"),sBookKey, sBookKey);
	TempDM.RestructDataManager(strQuery);
	
	const INT nColCnt = 18;
	
	CString strValue[nColCnt] = 
	{
		_T(""), _T(""), _T(""), _T(""), _T(""), 
		_T(""), _T(""), _T(""), _T(""), _T(""), 
		_T(""), _T(""), _T(""), _T(""), _T(""), 
		_T(""), _T(""), _T("")
	};
	for( i=0; i<nColCnt;i++)
	{
		strValue[i] = TempDM.GetCellData(0,i);
	}

	CString strUserNo, strLoanLibCode, strReturnDate, strRetrunStatus, strLoanKey, strUserStatus, strLoanStopDate, strLoanStopSetDate, strKLMemberYN;
	CString strUserKey;
	CString strBookLibCode;
	strBookLibCode = strValue[1];

	strUserNo = _T("");
	strLoanLibCode = _T("");
	strReturnDate = _T("");
	strRetrunStatus = _T("");
	strLoanKey = _T("");
	strUserStatus = _T("0");
	strLoanStopDate = _T("NULL");
	strLoanStopSetDate = _T("");
	strKLMemberYN      = strValue[16];
	strUserKey = strValue[5];

	
	strLoanLibCode = strValue[17];
	
	if ( strLoanLibCode.GetLength() == 0 ) {
		strLoanLibCode = strBookLibCode;
	}	
	if ( strBookLibCode.GetLength() == 0 ) {
		AfxMessageBox(_T("����������� ���� å�η��ݳ� ó���Ұ�. ILL_BarCodeReturn() Error!"));
		return 0;
	}	

	strUserNo = strValue[0];	
	strReturnDate = strValue[15];
	strRetrunStatus = _T("R");
	strLoanKey = strValue[2];

	TmpDM.StartFrame();	
	
	if(strBookLibCode.Compare(m_pParentMgr->m_pInfo->LIB_CODE) == 0 )
	{
		strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = %s, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN ('L','D');"),
						sReturnDate, m_pParentMgr->GetWorkLogMsg(_T("å�η� �ڰ��ݳ�"), __WFILE__, __LINE__),strDevice, strWorker, sBookKey );		
		TmpDM.AddFrame(strQuery);
		
		strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET LS_RETURN_DATE = %s, RETURN_LIBCODE = '%s', LAST_WORK = '%s',  END_DATE = SYSDATE, END_USERID = '%s', END_DIV = '0010', STATE = '0009' WHERE STATE = '0014' AND BOOK_KEY = %s;"),
						sReturnDate, m_pParentMgr->m_pInfo->LIB_CODE, m_pParentMgr->GetWorkLogMsg(_T("å�η� �ڰ��ݳ�"), __WFILE__, __LINE__), strWorker, sBookKey);
		
		TmpDM.AddFrame(strQuery);

		
		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET PREV_WORKING_STATUS = WORKING_STATUS, WORKING_STATUS = 'BOL112N' WHERE REC_KEY = %s;"),sBookKey);
		TmpDM.AddFrame(strQuery);
		
	}
	else
	{
		CESL_DataMgr Temp;
		CString strIllCnt = _T("");
		Temp.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
		strQuery.Format(_T("SELECT TRANS_NO FROM ILL_TRANS_TBL WHERE BOOK_KEY = %s AND STATE = '0014'"),sBookKey);
		Temp.RestructDataManager(strQuery);

		INT nRowCnt = 0;
		nRowCnt = Temp.GetRowCount();
		
		
		if(nRowCnt == 0)
		{
			CESL_DataMgr TempDM;
			TempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
			
			strQuery.Format(_T("SELECT CL.USER_NO, (SELECT ML.LIB_CODE FROM MN_LIBINFO2_TBL ML, BO_BOOK_TBL BB WHERE BB.REC_KEY = %s AND ML.MANAGE_CODE = BB.MANAGE_CODE), ")
			
				_T("LW.REC_KEY, LW.LOAN_DATE, LW.RETURN_PLAN_DATE, LW.USER_KEY, LW.BOOK_KEY, LW.PUBLISH_FORM_CODE, ")
			
				_T("LW.REG_NO, LW.CALL_NO, LW.TITLE, LW.AUTHOR, LW.PUBLISHER, BB.VOL_TITLE, BB.BOOK_APPENDIX_FLAG ")
				_T("FROM CO_LOAN_USER_TBL CL, LS_WORK_T01 LW, BO_BOOK_TBL BB ")
				_T("WHERE CL.REC_KEY = LW.USER_KEY AND LW.BOOK_KEY = BB.REC_KEY AND LW.STATUS IN ('0','2') AND LW.BOOK_KEY = %s"),sBookKey, sBookKey);
			TempDM.RestructDataManager(strQuery);
			
			const INT nColCnt = 15;
			CString strValue[nColCnt] = 
			{
				_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), 
				_T(""), _T(""), _T(""), _T(""), _T("")
			};
			for( j=0; j<nColCnt;j++)
			{
				strValue[j] = TempDM.GetCellData(0,j);
			}					
			
			
			strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = %s, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN ('0','2');"),
							sReturnDate, m_pParentMgr->GetWorkLogMsg(_T("å�η� Ÿ���ݳ�"), __WFILE__, __LINE__),strDevice, strWorker, sBookKey );

			TmpDM.AddFrame(strQuery);
			
			strQuery.Format(_T("INSERT INTO ILL_TRANS_TBL(TRANS_NO, STATE, USER_NO, GIVE_LIBCODE, RETURN_LIBCODE, ")
				_T("LOAN_KEY, LS_LOAN_DATE, RETURN_PLAN_DATE, LS_RETURN_DATE, USER_KEY, BOOK_KEY, PUBLISH_FORM_CODE, ")
				_T("REG_NO, CALL_NO, TITLE_INFO, AUTHOR_INFO, PUBLISHER_INFO, VOL_INFO, FIRST_WORK, LAST_WORK, BOOK_APPENDIX_FLAG) ")
				_T("SELECT TRANS_NO_SEQ.NEXTVAL, '0014', '%s', '%s', '%s', LS.REC_KEY, LS.LOAN_DATE, SYSDATE, LS.RETURN_DATE, LS.USER_KEY, LS.BOOK_KEY, LS.PUBLISH_FORM_CODE, LS.REG_NO, ")
				_T("LS.CALL_NO, LS.TITLE, LS.AUTHOR, LS.PUBLISHER, LS.VOL_INFO, '%s', '%s', '%s' FROM LS_WORK_T01 LS WHERE LS.REC_KEY = %s; "),
				strValue[0], strValue[1], m_pParentMgr->m_pInfo->LIB_CODE,
				m_pParentMgr->GetWorkLogMsg(_T("å�η� Ÿ���ݳ�"), __WFILE__, __LINE__), m_pParentMgr->GetWorkLogMsg(_T("å�η� Ÿ���ݳ�"), __WFILE__, __LINE__), strValue[14],strValue[2]);

			TmpDM.AddFrame(strQuery);
			
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET PREV_WORKING_STATUS = 'BOL112N' , WORKING_STATUS = 'BOL411O' WHERE REC_KEY = %s;"),sBookKey);
			TmpDM.AddFrame(strQuery);
		}		
		else
		{
			strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = %s, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN('L','D');"),
							sReturnDate, m_pParentMgr->GetWorkLogMsg(_T("å�η� ���ڵ�ݳ�"), __WFILE__, __LINE__),strDevice, strWorker, sBookKey );

			TmpDM.AddFrame(strQuery);

			
			strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET LS_RETURN_DATE = %s, RETURN_PLAN_DATE = SYSDATE, RETURN_LIBCODE = '%s', LAST_WORK = '%s' WHERE STATE = '0014' AND BOOK_KEY = %s;"),
							sReturnDate, m_pParentMgr->m_pInfo->LIB_CODE, m_pParentMgr->GetWorkLogMsg(_T("å�η� ���ڵ�ݳ�"), __WFILE__, __LINE__), sBookKey);

			TmpDM.AddFrame(strQuery);
		}
	}

	TmpDM.SendFrame();
	TmpDM.EndFrame();

	TmpDM.StartFrame();
	
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	
	CString strDelayDay;
	INT nDelayDay = 0;
	CString strTmpKey;
	

	for( i=0;i<pLoanDM->GetRowCount();i++)
	{
		pLoanDM->GetCellData(_T("åKEY"),i,strTmpKey);
		if(strTmpKey == sBookKey)
		{
			pLoanDM->GetCellData(_T("��ü�ϼ�"),i,strDelayDay);
			pLoanDM->GetCellData(_T("����/�ݳ�����KEY"),i,strLoanKey);
			strQuery.Format(_T("SELECT USER_KEY FROM LS_WORK_T01 WHERE REC_KEY = %s"),strLoanKey);
			TmpDM.GetOneValueQuery(strQuery,strUserKey);
		}
	}
		
	if(_T("1") == m_pManageValue->m_sLoanStopMode)
	{ 
		nDelayDay += _ttoi(strDelayDay);
	}
	else
	{ 
		if(nDelayDay < _ttoi(strDelayDay))
		{
			nDelayDay = _ttoi(strDelayDay);
		}
	}
		
	CString strLoanCount;
	strQuery.Format(_T("(SELECT DECODE(SIGN(NVL(LOAN_COUNT,0)-1),1,LOAN_COUNT-1,0) ")
		_T("FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s)"),strUserKey);
	TmpDM.GetOneValueQuery(strQuery,strLoanCount);
		
	
	if(0 < nDelayDay)
	{
		CString strQuery, strValue;
		if(_T("1") == m_pManageValue->m_sLoanStopMode)
		{ 
			strQuery.Format(_T("SELECT TO_CHAR(LOAN_STOP_DATE,'YYYY/MM/DD') FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), strUserKey);
			TmpDM.GetOneValueQuery(strQuery, strValue);
			if(strValue.IsEmpty())
			{
				strQuery.Format(_T("SELECT TO_CHAR(ECO$F_GET_LOANSTOPDATE(0,TO_DATE(SYSDATE),'%s',%d),'YYYY/MM/DD') FROM DUAL"),m_pParentMgr->m_pInfo->MANAGE_CODE, nDelayDay);
			}
			else
			{
				CString strReturnDateQuery;
				strReturnDateQuery.Format(_T("SELECT RETURN_DATE FROM LS_WORK_T01 WHERE REC_KEY = %s"),strLoanKey);
				TmpDM.GetOneValueQuery(strReturnDateQuery,m_strReturnDate);
				CTime tPreLSD(_ttoi(strValue.Mid(0, 4)), _ttoi(strValue.Mid(5, 2)), _ttoi(strValue.Mid(8, 2)), 0, 0, 0);
				CTime tReturn(_ttoi(m_strReturnDate.Mid(0, 4)), _ttoi(m_strReturnDate.Mid(5, 2)), _ttoi(m_strReturnDate.Mid(8, 2)), 0, 0, 0);
				if(tReturn <= tPreLSD)
				{
					CTimeSpan tOne(1,0,0,0);
					CTime tSetDate = tPreLSD + tOne;
					CString strSetDate;
					strSetDate.Format(_T("%04d/%02d/%02d"), tSetDate.GetYear(), tSetDate.GetMonth(), tSetDate.GetDay());
					strQuery.Format(_T("SELECT TO_CHAR(ECO$F_GET_LOANSTOPDATE(0,TO_DATE('%s','YYYY/MM/DD'),'%s',%d),'YYYY/MM/DD') FROM DUAL"), strSetDate,m_pParentMgr->m_pInfo->MANAGE_CODE,nDelayDay);
				}
				else
				{
					strQuery.Format(_T("SELECT TO_CHAR(ECO$F_GET_LOANSTOPDATE(0,TO_DATE(SYSDATE),'%s',%d),'YYYY/MM/DD') FROM DUAL"), m_pParentMgr->m_pInfo->MANAGE_CODE,nDelayDay);
				}
			}
		}
		else
		{ 
			strQuery.Format(_T("SELECT TO_CHAR(ECO$F_GET_LOANSTOPDATE(0,TO_DATE(SYSDATE),'%s',%d),'YYYY/MM/DD') FROM DUAL"), m_pParentMgr->m_pInfo->MANAGE_CODE, nDelayDay);
		}
		CString strLoanStopDate;
		ids = TmpDM.GetOneValueQuery(strQuery, strValue);
		if(0 > ids && ids != -4007){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);}
		if(strValue.IsEmpty())
		{
			strLoanStopDate = _T("NULL");
		}
		else
		{
			strLoanStopDate.Format(_T("TO_DATE('%s','YYYY/MM/DD')"), strValue);
		}
	
		if(_T("NULL") != strLoanStopDate)
		{
			
			CString strReturnExpireDateQuery, strPreLoanStopDate;
			strReturnExpireDateQuery.Format(_T("SELECT LOAN_STOP_DATE FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s"),strUserKey);
			TmpDM.GetOneValueQuery(strReturnExpireDateQuery, strPreLoanStopDate);
			
			if(strPreLoanStopDate != _T(""))
			{
				CTime tPreLSD(_ttoi(strPreLoanStopDate.Mid(0, 4)), _ttoi(strPreLoanStopDate.Mid(5, 2)), _ttoi(strPreLoanStopDate.Mid(8, 2)), 0, 0, 0);
				CTime tNewLSD(_ttoi(strValue.Mid(0, 4)), _ttoi(strValue.Mid(5, 2)), _ttoi(strValue.Mid(8, 2)), 0, 0, 0);
				
				if(tNewLSD >= tPreLSD)
				{
					strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET USER_CLASS = DECODE(USER_CLASS,'0','1',USER_CLASS), ")
						_T("LOAN_STOP_DATE = %s, LOAN_STOP_SET_DATE = SYSDATE, DELAYDAY_COUNT = DELAYDAY_COUNT+%d, DELAY_COUNT = DELAY_COUNT+1 ")
						_T("WHERE REC_KEY = %s;"),strLoanStopDate, nDelayDay, strUserKey);
					TmpDM.AddFrame(strQuery);
				}
				
				strUserStatus = _T("1");				
				strLoanStopSetDate = strReturnDate;				
			}
			
			else
			{
				strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET USER_CLASS = DECODE(USER_CLASS,'0','1',USER_CLASS), ")
					_T("LOAN_STOP_DATE = %s, LOAN_STOP_SET_DATE = SYSDATE, DELAYDAY_COUNT = DELAYDAY_COUNT+%d, DELAY_COUNT = DELAY_COUNT+1 ")
					_T("WHERE REC_KEY = %s;"),strLoanStopDate, nDelayDay, strUserKey);
					TmpDM.AddFrame(strQuery);
				
				
				strUserStatus = _T("1");				
				strLoanStopSetDate = strReturnDate;
			}
		}
	}

	TmpDM.SendFrame();
	TmpDM.EndFrame();
	

	if ( strKLMemberYN == _T("Y") && m_pManageValue->m_sIsUnityLoanService == _T("Y") ) {
		
		ids = KLService11(strUserNo, strBookLibCode, strReturnDate, strRetrunStatus, strLoanKey, strUserStatus, strLoanStopDate, strLoanStopSetDate);	
	}

	if(strBookLibCode.Compare(m_pParentMgr->m_pInfo->LIB_CODE) == 0 ) {
		SPFExcutePath(_T("�ݳ����ڷΌ�ุ���Ϻο�"));
	}
	
	ILL_APPENDIX_RETURN(sBookKey, strUserStatus, strLoanStopDate, strLoanStopSetDate, strUserKey);
	
	
	ILL_APPENDIX_REMAIN(sBookKey);	

	if ( strBookLibCode.Compare(m_pParentMgr->m_pInfo->LIB_CODE) != 0) {
		SelfCloseMessageBox(_T("������� �ٸ� å�� å�η� �ݳ��� �Ϸ�Ǿ����ϴ�."), m_pParentMgr, 3000);	
	}	
	
	
	ids = MoveGrid(sBookKey);
	if( 0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);	

	
	return ids;
}

INT CBL_LOC_LOAN_PROC::ILL_APPENDIX_RETURN(CString sBookKey, CString strpUserStatus, CString strpLoanStopDate, CString strpLoanStopSetDate, CString strUserKey)
{
	INT ids, i, j, k;
	CString strQuery;
	CString strAppendixFlag;
	CString strDevice = _T("KOLASIII");
	CString strWorker = m_pParentMgr->m_pInfo->USER_ID;
	CString strFuncName = _T("ILL_APPENDIX_RETURN");

	CESL_DataMgr TmpDM;	
	TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	
	
	strQuery.Format(_T("SELECT BOOK_APPENDIX_FLAG FROM BO_BOOK_TBL WHERE REC_KEY = %s"),sBookKey);
	TmpDM.GetOneValueQuery(strQuery,strAppendixFlag);

	if(strAppendixFlag.Compare(_T("B"))!=0) return 0;
	
	
	
	CString strSpeciesKey = _T("");
	strQuery.Format(_T("SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY = %s"),sBookKey);
	TmpDM.GetOneValueQuery(strQuery,strSpeciesKey);
	
	
	INT nAppendixCnt = 0;
	
	CESL_DataMgr AppendixTmpDM;
	AppendixTmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	
	
	strQuery.Format(_T("SELECT LW.BOOK_KEY FROM LS_WORK_T01 LW, BO_BOOK_TBL BO WHERE BO.BOOK_APPENDIX_FLAG='A' AND LW.BOOK_KEY = BO.REC_KEY AND LW.STATUS IN ('0','2','L','D') AND BO.SPECIES_KEY = '%s' AND LW.RETURN_DATE IS NULL AND LW.USER_KEY = %s"),strSpeciesKey,strUserKey);
			
	AppendixTmpDM.RestructDataManager(strQuery);
	nAppendixCnt = AppendixTmpDM.GetRowCount();

	
	if ( nAppendixCnt <= 0 ) return 0;			
	
	CString strMessage = _T("");
	strMessage.Format(_T("�ݳ� ��� �η��ڷᰡ %d�� �ֽ��ϴ�.\r\n �ϰ� �ݳ��Ͻðڽ��ϱ�?"),nAppendixCnt);
	if(IDYES != (AfxMessageBox(strMessage, MB_ICONQUESTION | MB_YESNO))) return 0;
			
	CESL_DataMgr SecTmpDM;
	SecTmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);	
	
	for( i=0;i<nAppendixCnt;i++)
	{
		CString strAppendixBookKey = AppendixTmpDM.GetCellData(i,0);
		
		CString strLoanLibManageCode = _T("");				
		strQuery.Format(_T("SELECT MANAGE_CODE FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS IN ('0','2','L','D') "),strAppendixBookKey);
		TmpDM.GetOneValueQuery(strQuery,strLoanLibManageCode);

		if (strLoanLibManageCode.GetLength() <= 0 ) {
			AfxMessageBox(_T("����������� ���� å�η��ݳ� ó���Ұ�. ILL_APPENDIX_RETURN() Error!"));
			return 0;
		}

		CESL_DataMgr TempDM;
		TempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
		
		strQuery.Format(_T("SELECT CL.USER_NO, (SELECT ML.LIB_CODE FROM MN_LIBINFO2_TBL ML, BO_BOOK_TBL BB WHERE BB.REC_KEY = %s AND ML.MANAGE_CODE = BB.MANAGE_CODE AND ROWNUM < 2), ")
			
			_T("LW.REC_KEY, LW.LOAN_DATE, LW.RETURN_PLAN_DATE, LW.USER_KEY, LW.BOOK_KEY, LW.PUBLISH_FORM_CODE, ")
			
			_T("LW.REG_NO, LW.CALL_NO, LW.TITLE, LW.AUTHOR, LW.PUBLISHER, BB.VOL_TITLE, BB.BOOK_APPENDIX_FLAG, TO_CHAR(SYSDATE, 'YYYY/MM/DD'), KL_MEMBER_YN ")
			
		    _T(", (SELECT APL_LIBCODE FROM ILL_TRANS_TBL IT WHERE LW.REC_KEY=IT.LOAN_KEY AND ROWNUM =1 )")
			_T("FROM CO_LOAN_USER_TBL CL, LS_WORK_T01 LW, BO_BOOK_TBL BB ")
			_T("WHERE CL.REC_KEY = LW.USER_KEY AND LW.BOOK_KEY = BB.REC_KEY AND LW.STATUS IN ('0','2','L','D') AND LW.BOOK_KEY = %s"),strAppendixBookKey, strAppendixBookKey);
		TempDM.RestructDataManager(strQuery);
		
		const INT nColCnt = 18;
		
		CString strValue[nColCnt] = 
		{
			_T(""), _T(""), _T(""), _T(""), _T(""), 
			_T(""), _T(""), _T(""), _T(""), _T(""), 
			_T(""), _T(""), _T(""), _T(""), _T(""), 
			_T(""), _T(""), _T("")
		};
		for( j=0; j<nColCnt;j++)
		{
			strValue[j] = TempDM.GetCellData(0,j);
		}
		
		CString strUserNo, strLoanLibCode, strReturnDate, strRetrunStatus, strLoanKey, strUserStatus, strLoanStopDate, strLoanStopSetDate, strKLMemberYN;
		
		strUserNo = _T("");
		strLoanLibCode = _T("");
		strReturnDate = _T("");
		strRetrunStatus = _T("");
		strLoanKey = _T("");
		strUserStatus = strpUserStatus;
		strLoanStopDate = strpLoanStopDate;
		strLoanStopSetDate = strpLoanStopSetDate;
		strKLMemberYN = strValue[16];

		CString strBookLibCode;
		strBookLibCode = strValue[1];
		strLoanLibCode = strValue[17];
		
		if ( strLoanLibCode.GetLength() == 0 ) {
			strLoanLibCode = strBookLibCode;
		}		
		
		if ( strLoanLibCode.GetLength() == 0 ) {
			AfxMessageBox(_T("����������� ���� å�η��ݳ� ó���Ұ�. ILL_BarCodeReturn() Error!"));
			return 0;
		}
		
		strUserNo = strValue[0];		
		strReturnDate = strValue[15];
		strRetrunStatus = _T("R");
		strLoanKey = strValue[2];

		SecTmpDM.StartFrame();
		
		if(strBookLibCode.Compare(m_pParentMgr->m_pInfo->LIB_CODE) == 0 )		
		{
			strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = SYSDATE, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN ('L','D');"),
			m_pParentMgr->GetWorkLogMsg(_T("å�η� �ڰ��ݳ�"), __WFILE__, __LINE__),strDevice, strWorker, strAppendixBookKey );
			
			SecTmpDM.AddFrame(strQuery);
			
			
			strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET LS_RETURN_DATE = SYSDATE, RETURN_PLAN_DATE = SYSDATE, RETURN_LIBCODE = '%s', LAST_WORK = '%s',  END_DATE = SYSDATE, END_USERID = '%s', END_DIV = '0010', STATE = '0009' WHERE STATE = '0014' AND BOOK_KEY = %s;"),
				m_pParentMgr->m_pInfo->LIB_CODE, m_pParentMgr->GetWorkLogMsg(_T("å�η� �ڰ��ݳ�"), __WFILE__, __LINE__), strWorker, strAppendixBookKey);
			SecTmpDM.AddFrame(strQuery);

			
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET PREV_WORKING_STATUS = WORKING_STATUS, WORKING_STATUS = 'BOL112N' WHERE REC_KEY = %s;"),strAppendixBookKey);
			SecTmpDM.AddFrame(strQuery);
			
		}
		else
		{
			CESL_DataMgr Temp;
			CString strIllCnt = _T("");
			Temp.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
			strQuery.Format(_T("SELECT TRANS_NO FROM ILL_TRANS_TBL WHERE BOOK_KEY = %s AND STATE = '0014'"),strAppendixBookKey);
			Temp.RestructDataManager(strQuery);

			INT nRowCnt = 0;
			nRowCnt = Temp.GetRowCount();
			
			
			if(nRowCnt == 0)
			{
				CESL_DataMgr TempDM;
				TempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
				
				strQuery.Format(_T("SELECT CL.USER_NO, (SELECT ML.LIB_CODE FROM MN_LIBINFO2_TBL ML, BO_BOOK_TBL BB WHERE BB.REC_KEY = %s AND ML.MANAGE_CODE = BB.MANAGE_CODE), ")
				
					_T("LW.REC_KEY, LW.LOAN_DATE, LW.RETURN_PLAN_DATE, LW.USER_KEY, LW.BOOK_KEY, LW.PUBLISH_FORM_CODE, ")
				
					_T("LW.REG_NO, LW.CALL_NO, LW.TITLE, LW.AUTHOR, LW.PUBLISHER, BB.VOL_TITLE, BB.BOOK_APPENDIX_FLAG ")
					_T("FROM CO_LOAN_USER_TBL CL, LS_WORK_T01 LW, BO_BOOK_TBL BB ")
					_T("WHERE CL.REC_KEY = LW.USER_KEY AND LW.BOOK_KEY = BB.REC_KEY AND LW.STATUS IN ('0','2') AND LW.BOOK_KEY = %s"),strAppendixBookKey, strAppendixBookKey);
				TempDM.RestructDataManager(strQuery);
				
				const INT nColCnt = 15;
				CString strValue[nColCnt] = 
				{
					_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), 
					_T(""), _T(""), _T(""), _T(""), _T("")
				};
				for( k=0; k<nColCnt;k++)
				{
					strValue[k] = TempDM.GetCellData(0,k);
				}	
				
				strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = SYSDATE, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN ('0','2');"),
					m_pParentMgr->GetWorkLogMsg(_T("å�η� Ÿ���ݳ�"), __WFILE__, __LINE__),strDevice, strWorker, strAppendixBookKey );
				SecTmpDM.AddFrame(strQuery);

				strQuery.Format(_T("INSERT INTO ILL_TRANS_TBL(TRANS_NO, STATE, USER_NO, GIVE_LIBCODE, RETURN_LIBCODE, ")
					_T("LOAN_KEY, LS_LOAN_DATE, RETURN_PLAN_DATE, LS_RETURN_DATE, USER_KEY, BOOK_KEY, PUBLISH_FORM_CODE, ")
					_T("REG_NO, CALL_NO, TITLE_INFO, AUTHOR_INFO, PUBLISHER_INFO, VOL_INFO, FIRST_WORK, LAST_WORK, BOOK_APPENDIX_FLAG) ")
					_T("SELECT TRANS_NO_SEQ.NEXTVAL, '0014', '%s', '%s', '%s', LS.REC_KEY, LS.LOAN_DATE, LS.RETURN_DATE, LS.RETURN_DATE, LS.USER_KEY, LS.BOOK_KEY, LS.PUBLISH_FORM_CODE, LS.REG_NO, ")
					_T("LS.CALL_NO, LS.TITLE, LS.AUTHOR, LS.PUBLISHER, LS.VOL_INFO, '%s', '%s', '%s' FROM LS_WORK_T01 LS WHERE LS.REC_KEY = %s; "),strValue[0], strValue[1], m_pParentMgr->m_pInfo->LIB_CODE,
					m_pParentMgr->GetWorkLogMsg(_T("å�η� Ÿ���ݳ�"), __WFILE__, __LINE__), m_pParentMgr->GetWorkLogMsg(_T("å�η� Ÿ���ݳ�"), __WFILE__, __LINE__), strValue[14],strValue[2]);
				SecTmpDM.AddFrame(strQuery);

				
				strQuery.Format(_T("UPDATE BO_BOOK_TBL SET PREV_WORKING_STATUS = 'BOL112N' , WORKING_STATUS = 'BOL411O' WHERE REC_KEY = %s;"),strAppendixBookKey);
				SecTmpDM.AddFrame(strQuery);
			}
			else
			{				
				strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = SYSDATE, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN('L','D');"),
					m_pParentMgr->GetWorkLogMsg(_T("å�η� ���ڵ�ݳ�"), __WFILE__, __LINE__),strDevice, strWorker, strAppendixBookKey );
				SecTmpDM.AddFrame(strQuery);

				strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET LS_RETURN_DATE = SYSDATE, RETURN_PLAN_DATE = SYSDATE, RETURN_LIBCODE = '%s', LAST_WORK = '%s' WHERE STATE = '0014' AND BOOK_KEY = %s;"),
					m_pParentMgr->m_pInfo->LIB_CODE, m_pParentMgr->GetWorkLogMsg(_T("å�η� ���ڵ�ݳ�"), __WFILE__, __LINE__), strAppendixBookKey);		
				SecTmpDM.AddFrame(strQuery);
			}			
		}

		SecTmpDM.SendFrame();
		SecTmpDM.EndFrame();

		if ( strKLMemberYN == _T("Y") && m_pManageValue->m_sIsUnityLoanService == _T("Y") ) {			
			ids = KLService11(strUserNo, strBookLibCode, strReturnDate, strRetrunStatus, strLoanKey, strUserStatus, strLoanStopDate, strLoanStopSetDate);
			
		}

		ids = MoveGrid(strAppendixBookKey);
		if( 0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
	}				

	return ids;
}
INT CBL_LOC_LOAN_PROC::ILL_APPENDIX_REMAIN(CString sBookKey)
{
	INT ids;
	CString strQuery = _T(""), strMsg = _T(""), strResult = _T("");

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO( m_pParentMgr->m_pInfo->CONNECTION_INFO );
	strQuery.Format(_T("SELECT COUNT(1) FROM ILL_TRANS_TBL WHERE SPECIES_KEY = ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY=%s ) AND STATE IN ('0001', '0002','0004', '0014') AND LOAN_KEY IS NULL AND BOOK_APPENDIX_FLAG='A'"), sBookKey);
	ids = TmpDM.GetOneValueQuery( strQuery, strResult );
	
	if ( _ttoi(strResult) > 0 ) {
		CString strMsg;
		strMsg.Format(_T("�ݳ�ó���� ��ü �ڷ��߿� %s���� �η��ڷᰡ ������ ���� å�η��������Դϴ�.\n�ηϸ� ������ �����Ϸ��� ��찡 �ƴ϶�� Ȯ���Ͻʽÿ�."), strResult);
		AfxMessageBox(strMsg);
	}

	return 0;
}