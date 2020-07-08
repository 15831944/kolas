// Applicant_LoanUser.cpp : implementation file
//

#include "stdafx.h"
#include "Applicant_LoanUser.h"
#include "../BO_ACQ_LOAN_USER_SEARCH_MGR/BO_ACQ_LOAN_USER_SEARCH_DLG.h"
#include "../BO_ACQ_REQUEST_DATA_MANAGER/REQUEST_DATAMGR.h"
#include "../BO_ACQ_REQUEST_DATA_MANAGER/BO_ACQ_REQUESTDATAMGR.h"
#include "BO_LOC_3000Api.h"
#include "..\\..\\..\\..\\����\\SearchZipCodeAPI\\SearchZipCodeDlgOpenAPI.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApplicant_LoanUser dialog


CApplicant_LoanUser::CApplicant_LoanUser(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CApplicant_LoanUser)
	m_bSMSUseYN = FALSE;
	m_bReservationYN = FALSE;
	//}}AFX_DATA_INIT

	m_pHandPhoneNo = NULL;
	m_pCM = NULL;
	m_pDM = NULL;
	m_pUSERDM = NULL;
	m_brush.CreateSolidBrush(RGB(205,229,255));
	m_hApplicantTime = NULL;
	
	m_pParent = pParent;
	nThisIDX = -1;

	m_bUserNoChange = FALSE;
	m_bLoanUserSearch = FALSE;
	m_OriginCivilNo = _T("");
	m_OriginUserNo = _T("");

	m_strRequestStatus = _T("");

	m_nFurnishCnt = 0;
}

BOOL CApplicant_LoanUser::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CApplicant_LoanUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CApplicant_LoanUser)
	DDX_Check(pDX, IDC_chkSMS_RECEIPT_YN, m_bSMSUseYN);
	DDX_Check(pDX, IDC_chkRESERVATION_YN, m_bReservationYN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CApplicant_LoanUser, CDialog)
	//{{AFX_MSG_MAP(CApplicant_LoanUser)
	ON_BN_CLICKED(IDC_btnFINDZIPCODE, OnbtnFINDZIPCODE)
	ON_BN_CLICKED(IDC_btnREQUEST_FIND_APPLICANT, OnbtnREQUESTFINDAPPLICANT)
	ON_EN_KILLFOCUS(IDC_edtAPPLICANT, OnKillfocusedtAPPLICANT)
	ON_BN_CLICKED(IDC_chkSMS_RECEIPT_YN, OnchkSMSRECEIPTYN)
	ON_EN_SETFOCUS(IDC_edtREQUEST_EMAIL_ID, OnSetfocuseREQUESTEMAILID)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_chkRESERVATION_YN, OnchkRESERVATIONYN)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_edtNID, OnKillfocusedtNID)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApplicant_LoanUser message handlers

BOOL CApplicant_LoanUser::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	if (InitESL_Mgr(_T("SM_��ġ����ڷ�_��û����_ȸ��")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_��ġ����ڷ�_��û����_ȸ��"));	
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox ( _T("Open CM Error ") );
		return FALSE;
	}


	m_pDM = FindDM(_T("DM_��ġ����ڷ�_��û����_ȸ��"));
	if ( m_pDM == NULL ) 
	{
		AfxMessageBox ( _T("Open DM Error ") );
		return FALSE;
	}
	
	m_pUSERDM = FindDM(_T("DM_BO_LOC_3300"));
	if ( m_pUSERDM == NULL ) 
	{
		AfxMessageBox ( _T("Open DM Error ") );
		return FALSE;
	}
	
	m_pHandPhoneNo = new CString[6];
	m_pHandPhoneNo[0] = _T("010");
	m_pHandPhoneNo[1] = _T("011");
	m_pHandPhoneNo[2] = _T("016");
	m_pHandPhoneNo[3] = _T("017");
	m_pHandPhoneNo[4] = _T("018");
	m_pHandPhoneNo[5] = _T("019");

	CMulitComboBoxManager mgr(this);
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("010"), m_pHandPhoneNo, _T("�ڵ���1"), 6);

	if(m_pInfo->GetSMSUseMode() < 1)
	{
		GetDlgItem(IDC_chkSMS_RECEIPT_YN)->ShowWindow(SW_HIDE);
		CRequestManager mgr(this);
		mgr.SetUDFCheck(m_pDM, _T("SMS���ſ���"));
	}

	GetDlgItem(IDC_STC_USERNO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDT_USERNO)->ShowWindow(SW_SHOW);

	CString query, VALUE_1;
	query.Format( _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='�������' AND VALUE_NAME='�����ɻ�뿩��' AND MANAGE_CODE = UDF_MANAGE_CODE"));
	m_pDM->GetOneValueQuery( query, VALUE_1 );
	if(VALUE_1 == _T("0"))
	{
		GetDlgItem(IDC_chkRESERVATION_YN)->EnableWindow(FALSE);
	}

	m_hApplicantTime = new CHourEditControl;
	m_hApplicantTime->SubclassWindow( GetDlgItem(IDC_hApplicantHour)->GetSafeHwnd() );
	m_hApplicantTime->InitEdit();

	if(_T("Y")==m_sIsUnityLoanService)
	{
		((CButton*)GetDlgItem(IDC_chkID))->SetCheck(FALSE);
		GetDlgItem(IDC_chkID)->ShowWindow(SW_HIDE);
	}

	if(TRUE == m_pInfo->m_bCertifyUse)
	{
		((CButton*)GetDlgItem(IDC_chkID))->SetCheck(FALSE);
		GetDlgItem(IDC_chkID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcAL_CIVILNO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtNID)->ShowWindow(SW_HIDE);
	}

	if(m_pInfo->m_bFurnishWishLimitYN || m_pInfo->m_bFurnishLimitByTheWeek)
	{
		GetDlgItem(IDC_stcAL_REQUESTCNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDT_REQUESTCNT)->ShowWindow(SW_SHOW);
	}

	return TRUE;
}
CApplicant_LoanUser::~CApplicant_LoanUser()
{
	if(m_pHandPhoneNo)
	{
		delete[] m_pHandPhoneNo;
		m_pHandPhoneNo = NULL;
	}

	if(m_hApplicantTime)
	{
		delete m_hApplicantTime;
		m_hApplicantTime = NULL;
	}
}

VOID CApplicant_LoanUser::Diaply(INT ids)
{

	m_pCM->AllControlDisplay(m_idx);
	
	CString strEmail, strHp, strFurnishStatus, strSMSReceipYN, strReservationYN, strApplicantDate;
	m_pDM->GetCellData(_T("�ڵ���"), m_idx, strHp);
	m_pDM->GetCellData(_T("EMAIL"), m_idx, strEmail);
	m_pDM->GetCellData(_T("SMS���ſ���"), m_idx, strSMSReceipYN);
	m_pDM->GetCellData(_T("�켱���⿹��"), m_idx, strReservationYN);
	m_pDM->GetCellData(_T("��û����"), m_idx, strApplicantDate);

	SetHandPhoneInfo(strHp);
	SetEmailInfo(strEmail);
	strApplicantTime(strApplicantDate);

	UpdateData(FALSE);
}

VOID CApplicant_LoanUser::SetHandPhoneInfo(CString strHP)
{
	strHP.Replace(_T("-"), _T(""));
	if( 7 < strHP.GetLength() )
	{
		strHP = strHP.Left(3) + _T("-") + strHP.Mid( 3, strHP.GetLength() - 7 ) + _T("-") + strHP.Mid( strHP.GetLength() - 4 , 4);
	}

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

VOID CApplicant_LoanUser::SetEmailInfo(CString strEMail)
{
	CString strEmailID, strEmailSite;
	CStringArray arrStr;
	CRequestManager mgr(this);
	mgr.ParsingString(_T("@"), strEMail, &arrStr, FALSE);
	INT cnt = arrStr.GetSize();
	if(cnt > 0) strEmailID = arrStr.GetAt(0);
	if(cnt > 1) strEmailSite = arrStr.GetAt(1);

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

VOID CApplicant_LoanUser::OnbtnFINDZIPCODE() 
{
	CSearchZipCodeDlgOpenAPI dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		m_pCM->SetControlMgrData(_T("�ּ�"), dlg.GetApplyAddress());
		m_pCM->SetControlMgrData(_T("�����ȣ"), dlg.GetApplyZipCode());		
	}
}

VOID CApplicant_LoanUser::OnbtnREQUESTFINDAPPLICANT() 
{
	CString strApplicant, strUserNo, strUserKey, strQuery;
	CBO_ACQ_LOAN_USER_SEARCH_DLG UserSearchDlg(this);
	strApplicant = strUserNo = strUserKey = strQuery = _T("");
	
	m_pCM->GetControlMgrData(_T("��û��"), strApplicant);
	m_pCM->GetControlMgrData(_T("�����ڹ�ȣ"), strUserNo);

	UserSearchDlg.SetUserName(strApplicant);
	UserSearchDlg.SetOpenMode(1);
	
	if(m_bUserNoChange)
	{		
		UserSearchDlg.SetUserID(m_OriginUserNo);
	}
	else
	{
		UserSearchDlg.SetUserID(strUserNo);
	}	
	
	if( IDOK == UserSearchDlg.DoModal() )
	{
		strUserKey = UserSearchDlg.m_sSelectRecKey;
		if( strUserKey.IsEmpty() == TRUE )	
		{
			m_pUSERDM->FreeData();
			m_pUSERDM->SetCellData(_T("�̸�"), strApplicant, -1);
			m_pUSERDM->SetCellData(_T("�����ڹ�ȣ"), strUserNo, -1);
		}
		else
		{
			strQuery.Format(_T("REC_KEY = %s"), strUserKey);
			m_pUSERDM->RefreshDataManager(strQuery);

			if(TRUE == m_pInfo->m_bLibSecuUse && 0 < m_pUSERDM->GetRowCount())
			{
				CString strGetData;
				CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7,asDes8,asDes9,asDes10,asDes11;
				for(INT i = 0; i < m_pUSERDM->GetRowCount(); i++)
				{
					m_pUSERDM->GetCellData(_T("�����ּ�"), i, strGetData);
					asDes1.Add(strGetData);
					m_pUSERDM->GetCellData(_T("�ٹ����ּ�"), i, strGetData);
					asDes2.Add(strGetData);
					m_pUSERDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strGetData);
					asDes3.Add(strGetData);
					m_pUSERDM->GetCellData(_T("E_MAIL"), i, strGetData);
					asDes4.Add(strGetData);
					m_pUSERDM->GetCellData(_T("�޴���"), i, strGetData);
					asDes5.Add(strGetData);
					m_pUSERDM->GetCellData(_T("��й�ȣ"), i, strGetData);
					asDes6.Add(strGetData);
					m_pUSERDM->GetCellData(_T("������ȭ"), i, strGetData);
					asDes7.Add(strGetData);
					m_pUSERDM->GetCellData(_T("�ٹ�����ȭ"), i, strGetData);
					asDes8.Add(strGetData);
					m_pUSERDM->GetCellData(_T("IDX_�ڵ���"), i, strGetData);
					asDes9.Add(strGetData);
					m_pUSERDM->GetCellData(_T("ȸ������й�ȣ"), i, strGetData);
					asDes10.Add(strGetData);
					m_pUSERDM->GetCellData(_T("��2����ó"), i, strGetData);
					asDes11.Add(strGetData);
				}
				CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
				if(NULL != pApi)
				{
					if(TRUE == pApi->CheckConnection())
					{
						if(TRUE == pApi->ConnectSocket()) 
						{
							pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes1);
							pApi->CloseSocket();	
						}
						if(TRUE == pApi->ConnectSocket()) 
						{
							pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes2);
							pApi->CloseSocket();	
						}
						if(TRUE == pApi->ConnectSocket()) 
						{
							pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes3);
							pApi->CloseSocket();	
						}
						if(TRUE == pApi->ConnectSocket()) 
						{
							pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes4);
							pApi->CloseSocket();	
						}
						if(TRUE == pApi->ConnectSocket()) 
						{
							pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
							pApi->CloseSocket();	
						}
						if(TRUE == pApi->ConnectSocket()) 
						{
							pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("PASSWORD"), asDes6);
							pApi->CloseSocket();	
						}
						if(TRUE == pApi->ConnectSocket()) 
						{
							pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes7);
							pApi->CloseSocket();	
						}
						if(TRUE == pApi->ConnectSocket()) 
						{
							pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes8);
							pApi->CloseSocket();	
						}
						if(TRUE == pApi->ConnectSocket()) 
						{
							pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("IDX_HANDPHONE"), asDes9);
							pApi->CloseSocket();	
						}
						if(TRUE == pApi->ConnectSocket()) 
						{
							pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD"), asDes10);
							pApi->CloseSocket();	
						}
						if(TRUE == pApi->ConnectSocket()) 
						{
							pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("SECOND_PHONE"), asDes11);
							pApi->CloseSocket();	
						}
						for(i = 0; i < m_pUSERDM->GetRowCount(); i++)
						{
							m_pUSERDM->SetCellData(_T("�����ּ�"), asDes1.GetAt(i), i);
							m_pUSERDM->SetCellData(_T("�ٹ����ּ�"), asDes2.GetAt(i), i);
							m_pUSERDM->SetCellData(_T("E_MAIL"), asDes4.GetAt(i), i);
							m_pUSERDM->SetCellData(_T("�޴���"), asDes5.GetAt(i), i);
							m_pUSERDM->SetCellData(_T("��й�ȣ"), asDes6.GetAt(i), i);
							m_pUSERDM->SetCellData(_T("������ȭ"), asDes7.GetAt(i), i);
							m_pUSERDM->SetCellData(_T("�ٹ�����ȭ"), asDes8.GetAt(i), i);
							m_pUSERDM->SetCellData(_T("IDX_�ڵ���"), asDes9.GetAt(i), i);
							m_pUSERDM->SetCellData(_T("ȸ������й�ȣ"), asDes10.GetAt(i), i);
							m_pUSERDM->SetCellData(_T("��2����ó"), asDes11.GetAt(i), i);
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
				asDes8.RemoveAll(); asDes8.FreeExtra();
				asDes9.RemoveAll(); asDes9.FreeExtra();
				asDes10.RemoveAll(); asDes10.FreeExtra();
				asDes11.RemoveAll(); asDes11.FreeExtra();
			}
		}
		m_bLoanUserSearch = TRUE;

		CString strFurnishCnt = UserSearchDlg.m_strFurnishCnt;
		m_nFurnishCnt = _ttoi(strFurnishCnt);
		strFurnishCnt.Format(_T("%d / %d"), m_nFurnishCnt, m_pInfo->m_nFurnishWishMaxCnt);
		m_pCM->SetControlMgrData(_T("��û�Ǽ�"), strFurnishCnt);

		SetApplicantInfo();
	}
}

INT CApplicant_LoanUser::FindHandPhoneNo(CString strHpNo)
{
	for( INT i=0 ; i<6 ; i++ )
	{
		if( m_pHandPhoneNo[i] == strHpNo )	return i;
	}

	return -1;
}

VOID CApplicant_LoanUser::OnKillfocusedtAPPLICANT() 
{
}

INT CApplicant_LoanUser::SetApplicantInfo()
{
	if(m_pUSERDM == NULL) return -1;
	if(m_pUSERDM->GetRowCount() == 0) return -1;

	m_pCM->SetControlMgrData(_T("��û��"), m_pUSERDM->GetCellData(_T("�̸�"), 0));
	m_pCM->SetControlMgrData(_T("�����ڹ�ȣ"), m_pUSERDM->GetCellData(_T("�����ڹ�ȣ"), 0));

	m_OriginCivilNo = m_pUSERDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), 0);
	CString strCvtCivilNo = m_OriginCivilNo;
	((CBO_ACQ_RequestDataMgr*)((CRequest_DataMgr*)m_pParent)->m_pParent)->ChangeSecuriyCivilNo(strCvtCivilNo);
	m_pCM->SetControlMgrData(_T("�ֹε�Ϲ�ȣ"), strCvtCivilNo, 0);
	if(m_bUserNoChange)
	{
		m_OriginUserNo = m_pUSERDM->GetCellData(_T("�����ڹ�ȣ"), 0);
		CString strCvtUserNo = m_OriginUserNo;
		((CBO_ACQ_RequestDataMgr*)((CRequest_DataMgr*)m_pParent)->m_pParent)->ChangeUserNo(strCvtUserNo);
		m_pCM->SetControlMgrData(_T("�����ڹ�ȣ"), strCvtUserNo, 0);
	}

	m_pCM->SetControlMgrData(_T("�����ȣ"), m_pUSERDM->GetCellData(_T("���ÿ����ȣ"), 0));
	m_pCM->SetControlMgrData(_T("��ȭ��ȣ"), m_pUSERDM->GetCellData(_T("������ȭ"), 0));
	m_pCM->SetControlMgrData(_T("�ּ�"), m_pUSERDM->GetCellData(_T("�����ּ�"), 0));

	CString strHP = m_pUSERDM->GetCellData(_T("�޴���"), 0);
	CString strEMail = m_pUSERDM->GetCellData(_T("E_MAIL"), 0);

	SetHandPhoneInfo(strHP);
	SetEmailInfo(strEMail);

	return 0;
}

BOOL CApplicant_LoanUser::PreTranslateMessage(MSG* pMsg) 
{
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
	else if (pMsg->message == WM_KEYDOWN ) 
	{
		if		( pMsg->wParam == VK_ESCAPE )	return TRUE;
		else if	( pMsg->wParam == VK_RETURN )
		{
			if( GetDlgItem(IDC_edtAPPLICANT)->m_hWnd == pMsg->hwnd )	OnbtnREQUESTFINDAPPLICANT();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CApplicant_LoanUser::CheckID()
{
	INT nCheck = 0;
	CButton *b1 = (CButton*)GetDlgItem(IDC_chkID);
	nCheck = b1->GetCheck();

	CRequestManager pMgr(this);
	pMgr.InitTempData();

	CString sRRN;

	if( m_bLoanUserSearch == TRUE )
	{
		sRRN =m_pUSERDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), 0);
	}
	else
	{
		sRRN =m_ReceiveDM->GetCellData(_T("��ġ���_�ֹε�Ϲ�ȣ"), nThisIDX);
	}
	m_OriginCivilNo = sRRN;

	if(nCheck != 0)
	{
		if(!pMgr.IsRRN(sRRN))
		{
			ESLAfxMessageBox(_T("�Է��Ͻ� �ֹε�� ��ȣ�� �ùٸ��� �ʽ��ϴ�."));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CApplicant_LoanUser::CheckUserNo()
{
	INT nCheck = 0;
	CString strUserNo;	

	CButton* pEditReservationYn = (CButton*)GetDlgItem(IDC_chkRESERVATION_YN);
	INT nReservationYn = pEditReservationYn->GetCheck();
	if( nReservationYn == 1 )
	{
		CEdit* pEditUserNo = (CEdit*)GetDlgItem(IDC_EDT_USERNO);
		pEditUserNo->GetWindowText(strUserNo);
		strUserNo.TrimLeft(); strUserNo.TrimRight();
		if( strUserNo.IsEmpty() )
		{
			ESLAfxMessageBox(_T("�������� ������ ��Ȯ���� �ʽ��ϴ�.\r\n�˻��� ���ؼ� �Է��� �ּ���."));
			return FALSE;
		}
	}

	return TRUE;
}

VOID CApplicant_LoanUser::SetDialog( CESL_DataMgr *pDM, INT nIdx )
{
	m_ReceiveDM = pDM;
	nThisIDX = nIdx;
	
	m_pCM->SetControlMgrData(_T("��û��"), pDM->GetCellData(_T("��û��"), nIdx));
	m_pCM->SetControlMgrData(_T("�����ڹ�ȣ"), pDM->GetCellData(_T("�����ڹ�ȣ"), nIdx));
	m_pCM->SetControlMgrData(_T("�����ȣ"), pDM->GetCellData(_T("�����ȣ"), nIdx));
	m_pCM->SetControlMgrData(_T("��ȭ��ȣ"), pDM->GetCellData(_T("��ȭ��ȣ"), nIdx));
	m_pCM->SetControlMgrData(_T("�ֹε�Ϲ�ȣ"), pDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), nIdx));
	m_pCM->SetControlMgrData(_T("�ּ�"), pDM->GetCellData(_T("�ּ�"), nIdx));
	m_pCM->SetControlMgrData(_T("��û����"), pDM->GetCellData(_T("��û����"), nIdx));

	CString strHP = pDM->GetCellData(_T("�ڵ���"), nIdx);
	CString strEMail = pDM->GetCellData(_T("EMAIL"), nIdx);
	
	CButton *b1 = (CButton*)GetDlgItem(IDC_chkSMS_RECEIPT_YN);
	CString strSMSUseYN = pDM->GetCellData(_T("SMS���ſ���"),nIdx);
	if(strSMSUseYN == _T("Y"))  b1->SetCheck(TRUE);
	else						b1->SetCheck(FALSE);	

	CString strApplicantDate = pDM->GetCellData(_T("��û����"), nIdx);	

	CButton *b2 = (CButton*)GetDlgItem(IDC_chkRESERVATION_YN);
	CString strReservationYN = pDM->GetCellData(_T("�켱���⿹�࿩��"),nIdx);
	if(strReservationYN == _T("Y"))  b2->SetCheck(TRUE);
	else							 b2->SetCheck(FALSE);

	SetHandPhoneInfo(strHP);
	SetEmailInfo(strEMail);
	strApplicantTime(strApplicantDate);

	m_OriginCivilNo = pDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), nIdx);
	CString strCvtCivilNo = m_OriginCivilNo;
	((CBO_ACQ_RequestDataMgr*)((CRequest_DataMgr*)m_pParent)->m_pParent)->ChangeSecuriyCivilNo(strCvtCivilNo);
	m_pCM->SetControlMgrData(_T("�ֹε�Ϲ�ȣ"), strCvtCivilNo, nIdx);
	
	if(m_bUserNoChange)
	{
		m_OriginUserNo = pDM->GetCellData(_T("��ġ���_�����ڹ�ȣ"), nIdx);		
		CString strCvtUserNo = m_OriginUserNo;		
		((CBO_ACQ_RequestDataMgr*)((CRequest_DataMgr*)m_pParent)->m_pParent)->ChangeUserNo(strCvtUserNo);
		m_pCM->SetControlMgrData(_T("�����ڹ�ȣ"), strCvtUserNo, nIdx);	
	}

	return;
}

VOID CApplicant_LoanUser::SetInitInputDate()
{	
	CString strDate, strTime;
	CTime t = CTime::GetCurrentTime();
	strDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	SetControlData(m_pCM->CM_ALIAS, _T("��û����"), strDate);
	strApplicantTime(strDate);	
}

VOID CApplicant_LoanUser::OnchkSMSRECEIPTYN() 
{
	m_bSMSUseYN = !m_bSMSUseYN;
	
	UpdateData(FALSE);
}

VOID CApplicant_LoanUser::OnchkRESERVATIONYN() 
{
	if( _T("3") == m_strRequestStatus )
	{
		AfxMessageBox(_T("�������� �ڷῡ ���ؼ��� �켱���⿹���� ���� �� �� �����ϴ�.."));
	}
	else
	{
		m_bReservationYN = !m_bReservationYN;
	}	
	UpdateData(FALSE);
}

HBRUSH CApplicant_LoanUser::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}


VOID CApplicant_LoanUser::SetIME(INT nIDC, INT nMode)
{
	EFS_BEGIN

    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(nIDC)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		if ( nMode == 0 ) dwConversion=IME_CMODE_ALPHANUMERIC;
		else dwConversion=IME_CMODE_HANGUL;		

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(nIDC)->m_hWnd,hIME); 
    } 
			

	EFS_END
}

VOID CApplicant_LoanUser::OnSetfocuseREQUESTEMAILID()
{
	SetIME(IDC_edtREQUEST_EMAIL_ID,0);
}

VOID CApplicant_LoanUser::strApplicantTime(CString strDateTime)
{

	m_hApplicantTime->SetData(strDateTime.Mid(strDateTime.Find(_T(" "))));
}

void CApplicant_LoanUser::OnKillfocusedtNID() 
{
	CString strIdNo;
	CString strTemp;

	INT nChk;
	INT nLength;
	m_pCM->GetControlMgrData(_T("�ֹε�Ϲ�ȣ"), strIdNo);
	if( strIdNo.IsEmpty())
	{
		return;
	}
	nChk = strIdNo.Find(_T("-"));
	if( 0 > nChk )
	{
		strTemp = strIdNo;
		nLength = strIdNo.GetLength();
		if( 6 < nLength)
		{
			strIdNo.Format(_T("%s-%s"), strTemp.Mid(0,6), strTemp.Mid(6,nLength-6));
		}
		else
		{
			strIdNo.Format(_T("%s-"), strTemp);
		}
	}
	m_pCM->SetControlMgrData(_T("�ֹε�Ϲ�ȣ"), strIdNo);	
	
}

void CApplicant_LoanUser::HideRequestCntControl() 
{
	GetDlgItem(IDC_stcAL_REQUESTCNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDT_REQUESTCNT)->ShowWindow(SW_HIDE);
}