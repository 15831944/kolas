// Request_DataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Request_DataMgr.h"

/// ���� ����
#include "../../../�������/LOAN_INFO_SHARE_MANAGER/LoanShareManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequest_DataMgr dialog


CRequest_DataMgr::CRequest_DataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequest_DataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequest_DataMgr)
	m_bSMSUseYN = FALSE;
	//}}AFX_DATA_INIT
	m_idx = -1;
	m_firstidx = -1;
	m_lastidx = -1;
	m_pNoteCtrl = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
	m_pMainCM = NULL;
	m_pHandPhoneNo = NULL;
	m_pMainGrid = NULL;
	m_pUSERDM = NULL;
	m_bGridSelectYN = NULL;
	m_nOpType = OPERATION_TYPE_INVALID;
	pMakeSearchData = new CMakeSearchData(pParent); 
	m_pLoanMangeValue = NULL;
}

CRequest_DataMgr::~CRequest_DataMgr()
{
	if(m_pHandPhoneNo)
	{
		delete[] m_pHandPhoneNo;
		m_pHandPhoneNo = NULL;
	}
	if(pMakeSearchData)
	{
		delete pMakeSearchData; 
		pMakeSearchData = NULL; 
	}
	if(m_pLoanMangeValue)
	{
		delete m_pLoanMangeValue;
		m_pLoanMangeValue = NULL;
	}
}

VOID CRequest_DataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequest_DataMgr)
	DDX_Check(pDX, IDC_chkSMS_RECEIPT_YN, m_bSMSUseYN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequest_DataMgr, CDialog)
	//{{AFX_MSG_MAP(CRequest_DataMgr)
	ON_BN_CLICKED(IDC_btnFINDZIPCODE, OnbtnFINDZIPCODE)
	ON_EN_KILLFOCUS(IDC_edtWANTED_TITLE, OnKillfocusedtWANTEDTITLE)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnREQUEST_PREV, OnbtnREQUESTPREV)
	ON_BN_CLICKED(IDC_btnREQUEST_NEXT, OnbtnREQUESTNEXT)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnREQ_TITLE_DUP_SEARCH, OnbtnREQTITLEDUPSEARCH)
	ON_BN_CLICKED(IDC_btnREQUEST_FIND_APPLICANT, OnbtnREQUESTFINDAPPLICANT)
	ON_BN_CLICKED(IDC_btnREQ_NEW_SAVE, OnbtnREQNEWSAVE)
	ON_EN_KILLFOCUS(IDC_edtNID, OnKillfocusedtNID)
	ON_MESSAGE(NOTE_CLICKED, OnNoteClicked)
	ON_EN_KILLFOCUS(IDC_edtREQUEST_PRICE, OnKillfocusedtREQUESTPRICE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequest_DataMgr message handlers

BOOL CRequest_DataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_��ġ����ڷ�")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_��ġ����ڷ�"));	
	m_pDM = FindDM(_T("DM_��ġ����ڷ����"));	
	m_pMainCM = FindCM(_T("CM_��ġ����ڷ����"));	
	m_pMainGrid = (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	m_pUSERDM = FindDM(_T("DM_BO_LOC_3300"));
	
	//�ʼ� �Է��׸� ��Ʈ�� ���� ����
	AddColorControl(IDC_edtAPPLICANT, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtWANTED_TITLE, _T(""), _T(""), 'E');

	m_pHandPhoneNo = new CString[6];
	m_pHandPhoneNo[0] = _T("010");
	m_pHandPhoneNo[1] = _T("011");
	m_pHandPhoneNo[2] = _T("016");
	m_pHandPhoneNo[3] = _T("017");
	m_pHandPhoneNo[4] = _T("018");
	m_pHandPhoneNo[5] = _T("019");
	
	CMulitComboBoxManager mgr(this);
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("011"), m_pHandPhoneNo, _T("�ڵ���1"), 6);

	if(m_pInfo->GetSMSUseMode() < 1)
	{
		GetDlgItem(IDC_chkSMS_RECEIPT_YN)->ShowWindow(SW_HIDE);
		CRequestManager mgr(this);
		mgr.SetUDFCheck(m_pDM, _T("SMS���ſ���"));
	}

	GetWindowRect(m_rcWindow);
	ShowCodeResource();

	if(m_nOpType == SINSERT)
	{
		CString strDate;
		CTime t = CTime::GetCurrentTime();
		CString strHour,strMin,strSec;
		strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());		// �Է���

		CButton * pBtn = (CButton*)GetDlgItem(IDC_btnREQUEST_PREV);
		CButton * pBtn1 = (CButton*)GetDlgItem(IDC_btnREQUEST_NEXT);
		CButton * pBtn2 = (CButton*)GetDlgItem(IDC_btnREQ_NEW_SAVE);
		pBtn->ShowWindow(SW_HIDE);
		pBtn1->ShowWindow(SW_HIDE);
		pBtn2->ShowWindow(SW_HIDE);
	}
	else
	{
		Display(0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequest_DataMgr::InitFirstLastIndex()
{
	INT ids = m_pMainGrid->SelectMakeList();
	ids = m_pMainGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pDM->GetRowCount();
		m_firstidx = 0;
		m_lastidx = cnt-1;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_lastidx = m_pMainGrid->SelectGetTailPosition();
		m_firstidx = m_pMainGrid->SelectGetHeadPosition();
	}
}

VOID CRequest_DataMgr::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnREQUEST_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnREQUEST_NEXT)->EnableWindow(TRUE);

	if(m_firstidx >= idx || idx < 0)
		GetDlgItem(IDC_btnREQUEST_PREV)->EnableWindow(FALSE);
	if(m_lastidx <= idx || idx < 0)
		GetDlgItem(IDC_btnREQUEST_NEXT)->EnableWindow(FALSE);
}

VOID CRequest_DataMgr::Display(INT ids)
{
	switch(ids)
	{
	case 0:
		InitFirstLastIndex();
		m_idx = GetFirstSelectIndex();
		ShowNextPrevButton(m_idx);
		break;
	case -1:
		m_idx = GetPrevSelectIndex();
		ShowNextPrevButton(m_idx);
		break;
	case 1:
		m_idx = GetNextSelectIndex();
		ShowNextPrevButton(m_idx);
		break;
	}
	
	m_pCM->AllControlDisplay(m_idx);
	
	CString strEmail, strHp, strFurnishStatus, strSMSReceipYN;
	m_pDM->GetCellData(_T("�ڵ���"), m_idx, strHp);
	m_pDM->GetCellData(_T("EMAIL"), m_idx, strEmail);
	m_pDM->GetCellData(_T("��ġ����"), m_idx, strFurnishStatus);
	m_pDM->GetCellData(_T("SMS���ſ���"), m_idx, strSMSReceipYN);
	//20070615 UPDATE PJW : �켱���⿹�࿩�� �߰�
	SetHandPhoneInfo(strHp);
	SetEmailInfo(strEmail);
	SetFurnishStatus(strFurnishStatus);
	
	if(strSMSReceipYN == _T("Y")) m_bSMSUseYN = TRUE;
	else m_bSMSUseYN = FALSE;
	UpdateData(FALSE);

	SetReverse(m_pMainCM->CM_ALIAS, _T("MainGrid"), m_idx);
}

INT CRequest_DataMgr::FindHandPhoneNo(CString strHpNo)
{
	for(INT i=0 ; i<6 ; i++)
	{
		if(m_pHandPhoneNo[i] == strHpNo)
			return i;
	}
	return -1;
}


VOID CRequest_DataMgr::OnbtnFINDZIPCODE() 
{
	// TODO: Add your control notification handler code here
	CSearchZipCodeDlg SearchZipCodeDlg(this);
	SearchZipCodeDlg.DoModal();

	// �����
	if(SearchZipCodeDlg.m_nCloseMode != 0)
	{
		m_pCM->SetControlMgrData(_T("�����ȣ"), SearchZipCodeDlg.m_sZipCode);
		m_pCM->SetControlMgrData(_T("�ּ�"), SearchZipCodeDlg.m_sAddress);
	}	
}

VOID CRequest_DataMgr::OnKillfocusedtWANTEDTITLE() 
{
	// TODO: Add your control notification handler code here
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtWANTED_TITLE);
	if (pEdit == NULL) return;
/*
	// ���� �¸��̸� üũ���� �ʴ´�.
	if (pEdit->GetStyle() & ES_READONLY) return;

	SetTimer(0, 1000, NULL);	
*/
}

VOID CRequest_DataMgr::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0)
	{
		KillTimer(0);

		CString strTitle;
		CString strSearchCount, strSpeciesKey;

		INT ids = m_pCM->GetControlMgrData(_T("��ǥ��"), strTitle);
		if (ids < 0) return;

		if (strTitle.IsEmpty()) return;

		CDuplicateSearch_Interface impl(this);
		if(m_nOpType != SINSERT)
		{
			m_pDM->GetCellData(_T("��KEY"), m_idx, strSpeciesKey);
		}
		ids = impl.ShowOnlyTitleDuplicateSearch(_T("��ġ�����������"), strTitle, strSearchCount, strSpeciesKey);
		if (ids < 0) return;

		if (strSearchCount == _T("0"))	return;
		

		if (m_pNoteCtrl != NULL)
		{
			delete m_pNoteCtrl;
		}
		
		m_pNoteCtrl = new CNoteCtrl;
		CString strNote;
		strNote.Format(_T("%s���� ������ �߰ߵǾ����ϴ�."), strSearchCount);
		m_pNoteCtrl->SetNote(strNote);

		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		point.y += 5;
		
		m_pNoteCtrl->Create(point, 200, 50, this);
		m_pNoteCtrl->ShowWindow(SW_SHOW);
	}		
	CDialog::OnTimer(nIDEvent);
}

VOID CRequest_DataMgr::OnbtnREQUESTPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID CRequest_DataMgr::OnbtnREQUESTNEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CRequest_DataMgr::OnOK() 
{
	// TODO: Add extra validation here
	CRequestManager pMgr(this);
	pMgr.InitTempData();
	CString sRRN, pubYear, strPrice;
	m_pCM->GetControlMgrData(_T("�ֹε�Ϲ�ȣ"), sRRN);
	m_pCM->GetControlMgrData(_T("�����"), pubYear);
	m_pCM->GetControlMgrData(_T("����"), strPrice);
	if(strPrice.IsEmpty()) 
	{
		strPrice = _T("0");
		m_pCM->SetControlMgrData(_T("����"), strPrice);
	}
	INT ids;
	CString errstr;
	if(!pMgr.IsRRN(sRRN))
	{
		ESLAfxMessageBox(_T("�Է��Ͻ� �ֹε�� ��ȣ�� �ùٸ��� �ʽ��ϴ�."));
		return;
	}
	
	ids = pMakeSearchData->IsValidYearData(pubYear);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�Է��Ͻ� ����� ������ �ùٸ��� �ʽ��ϴ�."));
		return;
	}
	if(!pMakeSearchData->CheckDigit(strPrice))
	{
		ESLAfxMessageBox(_T("�Է��Ͻ� ���������� �ùٸ��� �ʽ��ϴ�."));
		return ;
	}
	
	ids = CheckInputFieldData(m_pCM);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�ʼ��Է��׸��� ä���ֽʽÿ�."));
		return;
	}
	
	CString strSMSUseYN;
	UpdateData(TRUE);

	if(m_nOpType == SINSERT)
	{
		pMgr.SetTempData(_T("CMAlias"), m_pCM->CM_ALIAS.GetBuffer(0));
		pMgr.SetTempData(_T("DMAlias"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
		pMgr.SetTempData(_T("SMSUSEYN"), m_bSMSUseYN);
		ids = pMgr.SPFExecutePath(_T("��ġ����Է�"));
	}
	else
	{
		pMgr.SetTempData(_T("CMAlias"), m_pCM->CM_ALIAS.GetBuffer(0));
		pMgr.SetTempData(_T("DMAlias"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
		pMgr.SetTempData(_T("MainCMAlias"), m_pMainCM->CM_ALIAS.GetBuffer(0));
		pMgr.SetTempData(_T("SMSUSEYN"), m_bSMSUseYN);
		pMgr.SetTempData(_T("IDX"), m_idx);
		ids = pMgr.SPFExecutePath(_T("��ġ�������"));
	}
	if(ids < 0)
	{
		switch(ids)
		{
		case -1000:
			errstr = _T("��ġ����ڷ� �Է�/������ ���� CM/DM�� ã�� �� �����ϴ�.");
			break;
		case -1001:
			errstr = _T("�Է�/���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	errstr = _T("��ġ��� �ڷḦ �����Ͽ����ϴ�.");
	ESLAfxMessageBox(errstr.GetBuffer(0));
	
	if(m_nOpType == SINSERT)
	{
		m_pMainGrid->DisplayLine(m_pDM->GetRowCount()-1);
		m_pCM->AllControlClear();
		CString strDate;
		CString strHour,strMin,strSec;
		CTime t = CTime::GetCurrentTime();
		strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());		// �Է���
		//20070607 UPDATE PJW : �ð� �߰�
		SetControlData(m_pCM->CM_ALIAS, _T("��û����"), strDate);
	}
	else
	{
		m_pMainGrid->DisplayLine(m_idx);
		Display(1);
	}
	return;
}

INT CRequest_DataMgr::CheckInputFieldData(CESL_ControlMgr *pCM)
{
	if(!pCM) return -1;
	CString applicant, socialid, title, author, publisher;
	pCM->GetControlMgrData(_T("��û��"), applicant);
	//pCM->GetControlMgrData(_T("�ֹε�Ϲ�ȣ"), socialid);
	pCM->GetControlMgrData(_T("��ǥ��"), title);
	//pCM->GetControlMgrData(_T("������"), author);
	//pCM->GetControlMgrData(_T("������"), publisher);
	if(applicant.IsEmpty() || title.IsEmpty())// || author.IsEmpty() || publisher.IsEmpty() || socialid.IsEmpty())
		return -1;
	return 0;
}

UINT CRequest_DataMgr::OnNoteClicked(LPARAM lParam, WPARAM wParam)
{
	// ���� ���� ����
	CString strTmpData;

	CDuplicateSearch_Interface impl(this);
	impl.ShowTitleDuplicateSearchDlg(_T("�����ڵ���������"), strTmpData);
	
	return 0;
}

HBRUSH CRequest_DataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (IsColorControl(pWnd->GetDlgCtrlID())) {
		return SetColor(pDC);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

VOID CRequest_DataMgr::OnbtnREQTITLEDUPSEARCH() 
{
	// TODO: Add your control notification handler code here
	CString strTitle, strAuthor, strPublisher, strPubYear, strSearchCount, strTmpData;

	m_pCM->GetControlMgrData(_T("��ǥ��"), strTitle);
	m_pCM->GetControlMgrData(_T("������"), strPublisher);
	m_pCM->GetControlMgrData(_T("�����"), strPubYear);
	m_pCM->GetControlMgrData(_T("������"), strAuthor);
	
	if (strTitle.IsEmpty())
	{
		if(strAuthor.IsEmpty() && strPublisher.IsEmpty()) return;
	}
	
	CString strMarc;	strMarc.Empty();
	CMarc marc;
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("245$a"), strTitle);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("245$d"), strAuthor);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("260$b"), strPublisher);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("260$c"), strPubYear);
	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);

	if(strMarc.IsEmpty()) return;
	
	CDuplicateSearch_Interface impl(this);

	//INT ids = impl.ShowMarcDuplicateSearch(_T("��ġ����Է�ȭ�麹������"), strMarc, strSearchCount);
	INT ids = impl.SearchDuplicateDataByIndividually(_T("�Ǻ���������"), strMarc, _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), strSearchCount);
	if (ids < 0) return;
	
	if (strSearchCount == _T("0"))
	{
		ESLAfxMessageBox(_T("�������� ����� �����ϴ�."));
		return;	
	}

	//impl.ShowTitleDuplicateSearchDlg(_T("��ġ����Է�ȭ�麹������"), strTmpData);
	impl.ShowTitleDuplicateSearchDlg(_T("��ġ����Է�ȭ�麹������"), strTmpData);
}

VOID CRequest_DataMgr::OnbtnREQUESTFINDAPPLICANT() 
{
	// TODO: Add your control notification handler code here

	CLoanShareManager LoanShareManager(this);
	
	if(m_pLoanMangeValue)
	{
		delete m_pLoanMangeValue;
		m_pLoanMangeValue = NULL;
	}

	m_pLoanMangeValue = new CLonaManageValue(this);
	CLoanUserManage UserDlg(this,&LoanShareManager,m_pLoanMangeValue,TRUE);
	UserDlg.pMain = this->pMain;
	UserDlg.pParentMgr = this;
	UserDlg.m_pInfo = this->m_pInfo;
	UserDlg.DoModal();
	
	CString sUserKey = UserDlg.m_sSelectRecKey;

	if( sUserKey.IsEmpty() ) return ;
	
	CString strQuery;
	strQuery.Format(_T("REC_KEY = %s"), sUserKey);
	m_pUSERDM->RefreshDataManager(strQuery);

	SetApplicantInfo();
}

INT CRequest_DataMgr::SetApplicantInfo()
{
	if(m_pUSERDM == NULL) return -1;
	if(m_pUSERDM->GetRowCount() == 0) return -1;

	m_pCM->SetControlMgrData(_T("��û��"), m_pUSERDM->GetCellData(_T("�̸�"), 0));
	m_pCM->SetControlMgrData(_T("�����ȣ"), m_pUSERDM->GetCellData(_T("���ÿ����ȣ"), 0));
	m_pCM->SetControlMgrData(_T("��ȭ��ȣ"), m_pUSERDM->GetCellData(_T("������ȭ"), 0));
	m_pCM->SetControlMgrData(_T("�ֹε�Ϲ�ȣ"), m_pUSERDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), 0));
	m_pCM->SetControlMgrData(_T("�ּ�"), m_pUSERDM->GetCellData(_T("�����ּ�"), 0));
	CString strHP = m_pUSERDM->GetCellData(_T("�޴���"), 0);
	CString strEMail = m_pUSERDM->GetCellData(_T("E_MAIL"), 0);

	SetHandPhoneInfo(strHP);
	SetEmailInfo(strEMail);
	return 0;
}

VOID CRequest_DataMgr::SetHandPhoneInfo(CString strHP)
{
	CString strHpNo1, strHpNo2, strHpNo3;
	CStringArray arrStr;
	CRequestManager mgr(this);
	mgr.ParsingString(_T("-"), strHP, &arrStr);
	INT cnt = arrStr.GetSize();
	if(cnt > 0) strHpNo1 = arrStr.GetAt(0);
	if(cnt > 1) strHpNo2 = arrStr.GetAt(1);
	if(cnt > 2) strHpNo3 = arrStr.GetAt(2);

	m_pCM->SetControlMgrData(_T("�ڵ���2"), strHpNo2);
	m_pCM->SetControlMgrData(_T("�ڵ���3"), strHpNo3);
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("�ڵ���1"));
	INT nPos = FindHandPhoneNo(strHpNo1);
	pCmb->SetCurSel(nPos);
}

VOID CRequest_DataMgr::SetEmailInfo(CString strEMail)
{
	CString strEmailID, strEmailSite;
	CStringArray arrStr;
	CRequestManager mgr(this);
	mgr.ParsingString(_T("@"), strEMail, &arrStr, FALSE);
	INT cnt = arrStr.GetSize();
	if(cnt > 0) strEmailID = arrStr.GetAt(0);
	if(cnt > 1) strEmailSite = arrStr.GetAt(1);
	strEmailID.MakeLower();		strEmailSite.MakeLower();
	m_pCM->SetControlMgrData(_T("EMAIL_ID"), strEmailID);
	
	CString strEmailSiteCode;
	m_pInfo->pCodeMgr->ConvertDescToCode(_T("���ϸ��"), strEmailSite, strEmailSiteCode);
	if(strEmailSiteCode.IsEmpty() && !strEmailSite.IsEmpty())
	{
		CMulitComboBoxManager mgr(this);
		CString strData[1][2] = {strEmailSite, _T("")};
		mgr.AddMultiComboBoxStrData(m_pCM->CM_ALIAS, strData, _T("EMAIL_SITE"));
	}
	m_pCM->SetControlMgrData(_T("EMAIL_SITE"), strEmailSite);
}

INT CRequest_DataMgr::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;
	if(m_nOpType == GUPDATE)
	{
		nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	else if(m_nOpType == SUPDATE)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_firstidx;
		else
			nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	return nFirstSelIdx;
}

INT CRequest_DataMgr::GetPrevSelectIndex()
{
	INT nPrevSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nPrevSelIndex = m_pMainGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pMainGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_idx != m_firstidx)
			nPrevSelIndex = m_idx - 1;
	}
	return nPrevSelIndex;
}

INT CRequest_DataMgr::GetNextSelectIndex()
{
	INT nNextSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nNextSelIndex = m_pMainGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pMainGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_idx != m_lastidx)
			nNextSelIndex = m_idx + 1;
		else nNextSelIndex = m_idx;
	}
	return nNextSelIndex;
}

VOID CRequest_DataMgr::ShowCodeResource()
{
	INT i = 0;
	INT nCodeResourcesHeight = 115;
	CWnd *pWnd = NULL;
	
	CRect rcWindow;
	GetWindowRect(rcWindow);
	if (m_nOpType != SINSERT && rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	nArrCodeResourceID[nCount++] = IDC_stcREQ_DATA_STATUS;
	nArrCodeResourceID[nCount++] = IDC_stcREQ_FURNISH_STATUS;
	nArrCodeResourceID[nCount++] = IDC_edtREQ_FURNISH_STATUS;
	nArrCodeResourceID[nCount++] = IDC_stcREQ_CANCEL_REASON;
	nArrCodeResourceID[nCount++] = IDC_richREQ_CANCEL_REASON;
	
	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (m_nOpType != SINSERT)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	// Move Other Resources
	nCount = 0;
	INT nArrOtherResources[256];
	nArrOtherResources[nCount++] = IDC_stcREQ_BUTTON_GROUP;
	nArrOtherResources[nCount++] = IDC_btnREQUEST_PREV;
	nArrOtherResources[nCount++] = IDC_btnREQUEST_NEXT;
	nArrOtherResources[nCount++] = IDC_btnREQ_TITLE_DUP_SEARCH;
	nArrOtherResources[nCount++] = IDOK;
	nArrOtherResources[nCount++] = IDCANCEL;

	CRect rcResource;
	for (i = 0; i <nCount ; i++)
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			if (m_nOpType != SINSERT)
			{
				rcResource.top += nCodeResourcesHeight;
				rcResource.bottom += nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
			else
			{
				rcResource.top -= nCodeResourcesHeight;
				rcResource.bottom -= nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
		}
	}

	// Resize Window Size
	GetWindowRect(rcResource);
	if (m_nOpType != SINSERT)
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();
}

VOID CRequest_DataMgr::SetFurnishStatus(CString strFurnishStatus)
{
	CString strDesc;
	if(strFurnishStatus.IsEmpty()) strDesc = _T("��û��");
	else if(strFurnishStatus == _T("1")) strDesc = _T("��û��");
	else if(strFurnishStatus == _T("2")) strDesc = _T("ó����");
	else if(strFurnishStatus == _T("3")) strDesc = _T("������");
	else if(strFurnishStatus == _T("4")) strDesc = _T("��ҵ�");
	m_pCM->SetControlMgrData(_T("��ġ����"), strDesc);

/*
	if(strFurnishStatus != _T("4"))
	{
		((CEdit*)GetDlgItem(IDC_richREQ_CANCEL_REASON))->SetReadOnly(TRUE);
	}
*/
}

VOID CRequest_DataMgr::OnbtnREQNEWSAVE() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add extra validation here
	CRequestManager pMgr(this);
	pMgr.InitTempData();
	CString sRRN, pubYear, strPrice;
	m_pCM->GetControlMgrData(_T("�ֹε�Ϲ�ȣ"), sRRN);
	m_pCM->GetControlMgrData(_T("�����"), pubYear);
	m_pCM->GetControlMgrData(_T("����"), strPrice);
	if(strPrice.IsEmpty()) 
	{
		strPrice = _T("0");
		m_pCM->SetControlMgrData(_T("����"), strPrice);
	}
	INT ids;
	CString errstr;
	if(!pMgr.IsRRN(sRRN))
	{
		ESLAfxMessageBox(_T("�Է��Ͻ� �ֹε�� ��ȣ�� �ùٸ��� �ʽ��ϴ�."));
		return;
	}
	
	ids = pMakeSearchData->IsValidYearData(pubYear);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�Է��Ͻ� ����� ������ �ùٸ��� �ʽ��ϴ�."));
		return;
	}
	if(!pMakeSearchData->CheckDigit(strPrice))
	{
		ESLAfxMessageBox(_T("�Է��Ͻ� ���������� �ùٸ��� �ʽ��ϴ�."));
		return ;
	}
	
	ids = CheckInputFieldData(m_pCM);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�ʼ��Է��׸��� ä���ֽʽÿ�."));
		return;
	}
	pMgr.SetTempData(_T("CMAlias"), m_pCM->CM_ALIAS.GetBuffer(0));
	pMgr.SetTempData(_T("DMAlias"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
	ids = pMgr.SPFExecutePath(_T("��ġ����Է�"));
	if(ids < 0)
	{
		switch(ids)
		{
		case -1000:
			errstr = _T("��ġ����ڷ� �Է�/������ ���� CM/DM�� ã�� �� �����ϴ�.");
			break;
		case -1001:
			errstr = _T("�Է�/���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	errstr = _T("��ġ��� �ڷḦ �����Ͽ����ϴ�.");
	ESLAfxMessageBox(errstr.GetBuffer(0));
	
	m_pMainGrid->DisplayLine(m_pDM->GetRowCount()-1);	
	Display(1);
	return;	
}

VOID CRequest_DataMgr::OnKillfocusedtNID() 
{
	// TODO: Add your control notification handler code here
	CRequestManager pMgr(this);
	CString strCivilNo;
	m_pCM->GetControlMgrData(_T("�ֹε�Ϲ�ȣ"), strCivilNo);
	if(strCivilNo.IsEmpty()) return ;
	((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strCivilNo);
	if(strCivilNo.GetLength() != 13) 
	{
		ESLAfxMessageBox(_T("�ùٸ� �ֹε�Ϲ�ȣ�� �ƴմϴ�."));
		GetDlgItem(IDC_edtNID)->SetFocus();
		return;
	}
	CString strHighNum, strLowNum;
	strHighNum = strCivilNo.Left(6);
	strLowNum = strCivilNo.Right(7);
	strCivilNo = strHighNum + _T("-") + strLowNum;
	if(!pMgr.IsRRN(strCivilNo))
	{
		ESLAfxMessageBox(_T("�ùٸ� �ֹε�Ϲ�ȣ�� �ƴմϴ�."));
		m_pCM->SetControlMgrData(_T("�ֹε�Ϲ�ȣ"), _T(""));
	}
	else
		m_pCM->SetControlMgrData(_T("�ֹε�Ϲ�ȣ"), strCivilNo);
	return;
}

BOOL CRequest_DataMgr::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_CHAR)
    {
		if(GetDlgItem(IDC_edtNID)->m_hWnd == pMsg->hwnd || GetDlgItem(IDC_edtTEL_NO)->m_hWnd == pMsg->hwnd)
		{
			INT nVirtKey = (INT) pMsg->wParam;
			if ((nVirtKey < '0' || nVirtKey > '9') && (nVirtKey != '-') && nVirtKey != 8)
			{
				ESLAfxMessageBox(_T("���ڸ� �Է��Ͻ� �� �ֽ��ϴ�.")); 
				return TRUE;
			}
		}
    }	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CRequest_DataMgr::OnKillfocusedtREQUESTPRICE() 
{
	// TODO: Add your control notification handler code here
	CString strPrice;
	m_pCM->GetControlMgrData(_T("����"), strPrice);
	CString strTmpData;
	INT nPos = strPrice.Find(_T("."));
	if(nPos > 0) strTmpData = strPrice.Left(nPos);
	else strTmpData = strPrice;
	if(strTmpData.GetLength() > 8)
	{
		ESLAfxMessageBox(_T("���������� �Ҽ����� �ڷᰡ 8�ڸ� �̻��� �� �����ϴ�."));
		GetDlgItem(IDC_edtREQUEST_PRICE)->SetFocus();
		return ;
	}
}
