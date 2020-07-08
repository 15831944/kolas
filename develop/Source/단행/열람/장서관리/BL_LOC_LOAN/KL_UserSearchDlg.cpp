#include "stdafx.h"
#include "KL_UserSearchDlg.h"
#include "KL_UserSettingDlg.h"
#include "KL_PaymentListDlg.h"
#include "..\\..\\..\\..\\����\\K2UP_Common\\MakeSearchData.h"
#include "CertifyApi.h"
#include "..\\BL_LOC_USER\\UserCertifyDlg.h"
#include "KlCertifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\\BL_LOC_USER\\MypinCertifyDlg.h"

CString g_strIpinFlag;
CString g_strMypinFlag;
/////////////////////////////////////////////////////////////////////////////
// CKL_UserSearchDlg dialog


CKL_UserSearchDlg::CKL_UserSearchDlg(CESL_Mgr* pParent, CBL_LOC_LOAN_PROC *pLoan)
	: CESL_Mgr(CKL_UserSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_UserSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pLoan = pLoan;
	m_bIsRunningThread = FALSE;
	m_bUserSearchComplate = FALSE;
	m_strUserNo = _T("");
	m_strGpinHash = _T("");
	m_strWorkNo = _T("");
	m_strRegNo = _T("");
	m_strLibCode = _T("");
	m_strEtcMsg = _T("");
	m_strMaxLoanCount = _T("");
	m_strLocalFreeYN = _T("");
	m_strOtherFreeYN = _T("");
	m_nMode = 0;
	m_nIndex = 0;
	m_bLoanStopMode = FALSE;
	m_pEtcWriter = NULL;
	m_pEtcDM = NULL;

	m_bKLUserYN = TRUE;
	m_bShowBtn = FALSE;
	m_bOtherReturnFirst = FALSE;

	m_rBtn[0] = CRect(220, 195, 305, 235);
 	m_rBtn[1] = CRect(305, 195, 390, 235);
	m_nBtnMode[0] = 0;
	m_nBtnMode[1] = 0;

	m_bIsRetry = FALSE;
	m_pApi = NULL;

	m_SearchLibCode = _T("");
	m_strBookLibReloanYN = _T("");
}
CKL_UserSearchDlg::~CKL_UserSearchDlg()
{
	if(NULL != m_pApi)
	{
		delete m_pApi;
		m_pApi = NULL;
	}
}

void CKL_UserSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_UserSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_UserSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_UserSearchDlg)
	ON_BN_CLICKED(IDC_btnKLUserSearchAgain, OnbtnKLUserSearchAgain)
	ON_BN_CLICKED(IDC_btnKLUserSearchClose, OnbtnKLUserSearchClose)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MESSAGEBOX, OnMessageBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_UserSearchDlg message handlers

BOOL CKL_UserSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN )
		{
			if (m_bIsRunningThread == TRUE)
			{
				return TRUE;
			}

			if(GetDlgItem(IDC_btnKLUserSearchAgain)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKLUserSearchAgain();
			}
			else if(GetDlgItem(IDC_btnKLUserSearchClose)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKLUserSearchClose();
			}
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE )
		{
			if(TRUE == m_bIsRunningThread)
			{
				return TRUE;
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CKL_UserSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());

	if(InitESL_Mgr(_T("SM_KL_USER_SEARCH")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}


	if(!m_MemDC.GetSafeHdc())
	{
		CDC* pDC = GetDC();
		m_MemDC.CreateCompatibleDC(pDC);
		m_MemBitmap.CreateCompatibleBitmap(pDC, 400, 250);
		m_MemDC.SelectObject(&m_MemBitmap);
		m_MemDC.SetBkMode(TRANSPARENT);

		m_BGDC.CreateCompatibleDC(pDC);
		m_BGBitmap.LoadBitmap(IDB_LOAN_KL_MSG);		
		m_BGDC.SelectObject(&m_BGBitmap);
		
		ReleaseDC(pDC);
	}

	SetWindowPos(NULL, 0, 0, 400, 250, SWP_NOMOVE|SWP_NOZORDER);
	
	CRgn Rgn;
	Rgn.CreateRoundRectRgn(2, 2, 399, 248, 9, 9);
	SetWindowRgn(Rgn, TRUE);

	StartSearch();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKL_UserSearchDlg::OnbtnKLUserSearchAgain() 
{
	m_bIsRetry = TRUE;
	StartSearch();
}

INT CKL_UserSearchDlg::StartSearch()
{
	CString strFuncName = _T("StartSearch");

	 if(m_bIsRetry)
	 {
		 m_pInfo->SetConnectUnityLoanService(TRUE);
	 }

	if(0 == m_nMode)
	{
		SetTimer(1, 100, NULL);
	}
	else if(1 == m_nMode)
	{
		SetTimer(100, 100, NULL);
	}
	else if(2 == m_nMode)
	{
		SetTimer(110, 100, NULL);
	}
	else if(3 == m_nMode)
	{
		SetTimer(200, 100, NULL);
	}
	else if(4 == m_nMode)
	{
		SetTimer(120, 100, NULL);
	}
	else if(5 == m_nMode)
	{
		SetTimer(130, 100, NULL);
	}
	else if(6 == m_nMode)
	{
		SetTimer(140, 100, NULL);
	}
	else if(10 == m_nMode)
	{
		SetTimer(1000, 100, NULL);
	}
	else if(_MODE_LIB_CON_MANAGE_ == m_nMode)
	{
		SetTimer(_MODE_LIB_CON_MANAGE_TIMER, 100, NULL);
	}

	return 0;
}

void CKL_UserSearchDlg::OnbtnKLUserSearchClose() 
{
	if(TRUE == m_bIsRunningThread) return;

	SetTimer(3, 100, NULL);
}

void CKL_UserSearchDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;

	CDialog::OnClose();
}

VOID CKL_UserSearchDlg::SetMessage(BOOL bButtonShow, CString strMsg)
{
	m_strMsg = strMsg;
	m_bShowBtn = bButtonShow;

	Invalidate();
}

INT CKL_UserSearchDlg::InsertUser(CESL_DataMgr *pDM, CString &strRecKey)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("InsertUser");

	if ( m_pLoan->m_pManageValue->m_sLoanUserAccessMode.IsEmpty() || m_pLoan->m_pManageValue->m_sLoanUserAccessMode == _T("1") ) return 500;
	
	CString strCivilNoUseYN;
	m_pLoan->m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("�ֹε�Ϲ�ȣ�������"), strCivilNoUseYN);
	m_pLoan->m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("����������ȣ"), _T("�����ɻ�뿩��"), g_strIpinFlag);
	m_pLoan->m_pManageValue->GetManageValue(_T("����"), _T("����"), _T("����������ȣ"), _T("MYPIN��뿩��"), g_strMypinFlag);

	if (strCivilNoUseYN.CompareNoCase(_T("Y")) != 0)
	{
		if (g_strIpinFlag.CompareNoCase(_T("Y")) != 0 && g_strMypinFlag.CompareNoCase(_T("Y")) != 0)
		{
			if (m_pInfo->m_bCertifyUse == FALSE && m_pInfo->m_bKLUse == TRUE)
			{
				strCivilNoUseYN = _T("Y");
			}
		}
	}

	CString strCryptoCivilNo = _T("");
	CString strCI = _T("");	
	CString strUserName = _T("");

 	ids = pDM->GetCellData(_T("��ȣȭ�ֹε�Ϲ�ȣ"), 0, strCryptoCivilNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = pDM->GetCellData(_T("IPIN_HASH"), 0, strCI);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = pDM->GetCellData(_T("�������̸�"), 0, strUserName);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName); 	

	if (strCI.IsEmpty())
	{
		if (m_pInfo->m_bCertifyUse == TRUE || g_strMypinFlag.CompareNoCase(_T("Y")) == 0)
		{
			CString strMsg;
			strMsg.Format(_T("�ڵ� ������ ���ؼ��� ���������� �����ؾ� �մϴ�.\r\n")
						  _T("���������� �����Ͽ� ������ �����Ͻðڽ��ϱ�?\r\n")
						  _T(" - ��     : ���������� �����Ͽ� ������ �����մϴ�.\r\n")
						  _T(" - �ƴϿ� : ���������� �������� �ʰ� �ű�ȸ������ ���Թ޽��ϴ�.\r\n")
						  _T(" - ���   : ������ �����մϴ�.\r\n"));
			int nResult = MessageBox(strMsg, _T("��������Ȯ��"), MB_YESNOCANCEL|MB_ICONQUESTION);
			switch (nResult)
			{
			case IDYES:
				{
					if (g_strMypinFlag.CompareNoCase(_T("Y")) == 0)
					{
						CUserCertifyDlg dlg(this);
						dlg.m_nMode = 2;
						dlg.m_strName = strUserName;
						dlg.m_strCivilNoDisplayCheck = strCivilNoUseYN;
						dlg.DoModal();
						if (!dlg.m_strCertifyCode.IsEmpty())
						{
							strCI = dlg.m_strCertifyCode;
						}
					}
					else
					{
						CMypinCertifyDlg dlg(this);
						dlg.SetUserMode(2);
						dlg.SetUserName(strUserName);
						dlg.DoModal();
						if (!dlg.GetCertifyCI().IsEmpty())
						{
							strCI = dlg.GetCertifyCI();
						}
					}

					if (strCI.IsEmpty())
					{
						if(IDYES == MessageBox(_T("���������� �����Ͽ����ϴ�.\r\n")
											   _T("�ű�ȸ������ ���Թ����ðڽ��ϱ�?\r\n")
											   _T(" - ��     : �ű�ȸ������ ������ �����մϴ�.\r\n")
											   _T(" - �ƴϿ� : ������ �����մϴ�.\r\n"), _T("��������Ȯ��"), MB_YESNO|MB_ICONQUESTION))
						{
							//
						}
						else
						{
							return 100;
						}
					}
				}
				break;
			case IDNO:
				{
					//
				}
				break;
			default:
				{
					return 100;
				}
				break;
			}
		}
		else
		{
			if(strCryptoCivilNo.IsEmpty())
			{
				if(!m_pLoan->m_strSerchCivilNo.IsEmpty())
				{
					CString strTemp = m_pLoan->m_strSerchCivilNo.Left(6) + _T("-") + m_pLoan->m_strSerchCivilNo.Right(7);
					this->ESL_DES_Encryption(strTemp, strCryptoCivilNo);
					pDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), strTemp, 0);
					pDM->SetCellData(_T("��ȣȭ�ֹε�Ϲ�ȣ"), strCryptoCivilNo, 0);
				}
				else
				{
					CString strMsg;
					strMsg.Format(	_T("%s���� ���� å����ȸ�� ���Ե��������� ����������ȣ�� ������ ����\r\n")
									_T("�ֹε�Ϲ�ȣ�� ����Ͽ����� �ڵ� ������ ���ؼ��� �Ǹ������� �����ؾ� �մϴ�.\r\n\r\n")
									_T("�Ǹ������� �����Ͽ� ���Ը� �����Ͻðڽ��ϱ�?\r\n")
									_T(" - �� : �Ǹ������� �����Ͽ� ������ �����մϴ�.\r\n")
									_T(" - �ƴϿ� : ������ �����մϴ�.\r\n"), strUserName);
					if(IDYES == MessageBox(strMsg, _T("�Ǹ�����Ȯ��"), MB_YESNO|MB_ICONQUESTION))
					{
						CKlCertifyDlg dlg(this);
						dlg.m_nMode = 1;
						if(IDOK == dlg.DoModal())
						{
							CString strTemp = dlg.m_strSuccessCivilNo.Left(6) + _T("-") + dlg.m_strSuccessCivilNo.Right(7);
							this->ESL_DES_Encryption(strTemp, strCryptoCivilNo);
							pDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), strTemp, 0);
							pDM->SetCellData(_T("��ȣȭ�ֹε�Ϲ�ȣ"), strCryptoCivilNo, 0);
						}
						else
						{
							return 100;
						}
					}
					else
					{
						return 100;
					}
				}
			}
		}
	}	
	
	CString strQuery;
	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	if (strRecKey.IsEmpty() && !strCI.IsEmpty())
	{		
		strQuery.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE IPIN_HASH='%s'"), strCI);
		TmpDM.GetOneValueQuery(strQuery, strRecKey);
	}

	if (strRecKey.IsEmpty() && strCivilNoUseYN.CompareNoCase(_T("Y")) == 0 && !strCryptoCivilNo.IsEmpty())
	{
		strQuery.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE CRYPTO_CIVIL_NO='%s'"), strCryptoCivilNo);
		TmpDM.GetOneValueQuery(strQuery, strRecKey);
	}
	if (!strRecKey.IsEmpty())
	{
		CString strValue;
		strQuery.Format(_T("SELECT USER_CLASS FROM CO_LOAN_USER_TBL WHERE REC_KEY='%s'"), strRecKey);
		TmpDM.GetOneValueQuery(strQuery, strValue);
		if (strValue.CompareNoCase(_T("3")) == 0)
		{
			return 300;
		}
	}
	INT nMode = 0;
	if(strRecKey.IsEmpty())
	{
		nMode = 0;
	}
	else
	{
		nMode = 1;
	}
	const INT nSrcFieldCnt = 38;
	TCHAR* sSrcFieldAlias[nSrcFieldCnt] = 
	{
		_T("���մ����ڹ�ȣ")	, _T("�������̸�")		, _T("����"), 
		_T("����Ÿ��")			, _T("�ֹε�Ϲ�ȣ")	, _T("������ȣ"), 
		_T("�ٹ����μ���")		, _T("�ٹ������޸�")	, _T("�ڵ���"), 
		_T("�˻����ڵ���")		, _T("EMAIL")			, _T("�б�"), 
		_T("�г�")				, _T("��")				, _T("�й�"), 
		_T("����߼�����")		, _T("����������")		, _T("��������������"), 
		_T("���")				, _T("�ٹ���")			, _T("ȸ����RFID"), 
		_T("���ּ�")			, _T("����ȭ��ȣ")		, _T("�������ȣ"), 
		_T("�����ڻ����̸�")	, _T("�ٹ��������ȣ")	, _T("�ٹ�����ȭ��ȣ"), 
		_T("�ٹ����ּ�")		, _T("�޸������")		, _T("G-PINASCII�����ڵ�"), 
		_T("G-PINHASH�����ڵ�")	, _T("G-PIN����")		, _T("����ȸ������"),
		_T("��ȣȭ�ֹε�Ϲ�ȣ")
		, _T("IPIN_HASH"), _T("IPIN_DATE"), _T("IPIN_RESULT") , _T("CERTIFY_WORKER")
	};
	TCHAR* sDstFieldAlias[nSrcFieldCnt] = 
	{
		_T("USER_NO")			, _T("NAME")			, _T("BIRTHDAY"), 
		_T("BIRTHDAY_TYPE")		, _T("CIVIL_NO")		, _T("EXCHANGE_NO"), 
		_T("DEPARTMENT_NAME")	, _T("CLASS_NAME")		, _T("HANDPHONE"), 
		_T("IDX_HANDPHONE")		, _T("E_MAIL")			, _T("SCHOOL"), 
		_T("GRADE")				, _T("CLASS")			, _T("CLASS_NO"), 
		_T("MAIL_SEND_AREA")	, _T("LAST_LOAN_DATE")	, _T("LAST_MODIFY_DATE"), 
		_T("REMARK")			, _T("OFFICE")			, _T("WORKNO"), 
		_T("H_ADDR1")			, _T("H_PHONE")			, _T("H_ZIPCODE"), 
		_T("IDX_USER_NAME")		, _T("W_ZIPCODE")		, _T("W_PHONE"), 
		_T("W_ADDR1")			, _T("REMARK_EDIT_DATE"), _T("GPIN_ASCII"), 
		_T("GPIN_HASH")			, _T("GPIN_SEX")		, _T("KL_MEMBER_YN"),
		_T("CRYPTO_CIVIL_NO")
		, _T("IPIN_HASH"), _T("IPIN_DATE"), _T("IPIN_RESULT"), _T("CERTIFY_WORKER")
	};
	TCHAR *sDstFieldTypeAlias[nSrcFieldCnt] = 
	{
		_T("STRING")	, _T("STRING")	, _T("STRING"), 
		_T("STRING")	, _T("STRING")	, _T("STRING"), 
		_T("STRING")	, _T("STRING")	, _T("STRING"), 
		_T("STRING")	, _T("STRING")	, _T("STRING"),
		_T("STRING")	, _T("STRING")	, _T("STRING"), 
		_T("STRING")	, _T("NUMERIC")	, _T("NUMERIC"), 
		_T("STRING")	, _T("STRING")	, _T("STRING"), 
		_T("STRING")	, _T("STRING")	, _T("STRING"),
		_T("STRING")	, _T("STRING")	, _T("STRING"), 
		_T("STRING")	, _T("NUMERIC")	, _T("STRING"), 
		_T("STRING")	, _T("STRING")	, _T("STRING"),
		_T("STRING")
		, _T("STRING"), _T("NUMERIC"), _T("STRING"), _T("STRING")
	};
	CString strGetData;
	CESL_DataMgr *pSettingDM = FindDM(_T("DM_KL_USER_SETTING"));
	if(NULL == pSettingDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	pSettingDM->FreeData();
	pSettingDM->InsertRow(-1);
	pDM->GetCellData(_T("�ڵ���"), 0, strGetData);
	pSettingDM->SetCellData(_T("�ڵ���"), strGetData, 0);
	pDM->GetCellData(_T("EMAIL"), 0, strGetData);
	pSettingDM->SetCellData(_T("EMAIL"), strGetData, 0);
	pDM->GetCellData(_T("���ּ�"), 0, strGetData);
	pSettingDM->SetCellData(_T("���ּ�"), strGetData, 0);
	pDM->GetCellData(_T("���"), 0, strGetData);
	pSettingDM->SetCellData(_T("���"), strGetData, 0);
	pDM->GetCellData(_T("����������"), 0, strGetData);
	if(10 < strGetData.GetLength())
	{
		strGetData = strGetData.Left(10);
	}
	pSettingDM->SetCellData(_T("����������"), strGetData, 0);
	pDM->GetCellData(_T("��������������"), 0, strGetData);
	if(10 < strGetData.GetLength())
	{
		strGetData = strGetData.Left(10);
	}
	pSettingDM->SetCellData(_T("��������������"), strGetData, 0);

	CKL_UserSettingDlg SetDlg(this);
	pDM->GetCellData(_T("�������̸�"), 0, strGetData);
	SetDlg.m_strUserName = strGetData;
	pDM->GetCellData(_T("��������ȣ"), 0, strGetData);
	SetDlg.m_strGetLibCode = strGetData;
	pDM->GetCellData(_T("��������"), 0, strGetData);
	SetDlg.m_strGetLibName = strGetData;
	SetDlg.m_strLibCodeList = m_pLoan->m_strLibCodeList;
	if(IDOK != SetDlg.DoModal())
	{
		return 100;
	}
	CString strIdxHandPhone;
	if(TRUE == SetDlg.m_bEditHandPhone)
	{
		CMakeSearchData MakeSearchData(this);
		strIdxHandPhone = SetDlg.m_strHandPhone;
		MakeSearchData.GetKeyWord(strIdxHandPhone);
	}
	pDM->StartFrame();
	pDM->InitDBFieldData();
	if(0 == nMode)
	{
		ids = pDM->MakeRecKey(strRecKey);
		if(0 > ids)  ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
		ids = pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
		ids = pDM->AddDBFieldData(_T("USER_CLASS"), _T("STRING"), _T("0"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
		ids = pDM->AddDBFieldData(_T("MEMBER_CLASS"), _T("STRING"), _T("0"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
		ids = pDM->AddDBFieldData(_T("LOAN_STOP_DATE"), _T("NUMERIC"), _T("NULL"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
		ids = pDM->AddDBFieldData(_T("LOAN_STOP_SET_DATE"), _T("NUMERIC"), _T("NULL"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	}
	CStringArray DataArray;
	BOOL bConnect = FALSE;
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		if(NULL != m_pApi)
		{
			delete m_pApi;
			m_pApi = NULL;
		}
		m_pApi = new CBO_LOC_3000Api(this);
		if(NULL != m_pApi)
		{
			if(TRUE == m_pApi->CheckConnection())
			{
				bConnect = TRUE;
			}
		}
	}

	CString strTemp;
	for(INT i = 0; i < nSrcFieldCnt; i++)
	{
		pDM->GetCellData(sSrcFieldAlias[i], 0, strGetData);
		strTemp = sDstFieldAlias[i];
		if(0 < strTemp.Find(_T("DATE")))
		{
			if(10 == strGetData.GetLength())
			{
				strTemp = _T("TO_DATE('") + strGetData + _T("','YYYY/MM/DD')");
				strGetData = strTemp;
			}
			else if(19 == strGetData.GetLength())
			{
				strTemp = _T("TO_DATE('") + strGetData + _T("','YYYY/MM/DD HH24:MI:SS')");
				strGetData = strTemp;
			}
		}
		if(_T("CIVIL_NO") == strTemp)
		{ 
			if(_T("Y") == m_pLoan->m_pManageValue->m_strIsKLSecurityCivilNo)
			{
				m_pLoan->m_pLocCommonAPI->ChangeSecuriyCivilNo(this, strGetData, _T("Y"));
			}
		}
		if(_T("E_MAIL") == strTemp)
		{
			if(TRUE == SetDlg.m_bEditEmail)
			{
				strGetData = SetDlg.m_strEmail;
			}
		}
		if(_T("HANDPHONE") == strTemp)
		{
			if(TRUE == SetDlg.m_bEditHandPhone)
			{
				strGetData = SetDlg.m_strHandPhone;
			}
		}
		if(_T("IDX_HANDPHONE") == strTemp)
		{
			if(TRUE == SetDlg.m_bEditHandPhone)
			{
				strGetData = strIdxHandPhone;
			}
		}
		if(_T("H_ADDR1") == strTemp)
		{
			if(TRUE == SetDlg.m_bEditHomeAddress)
			{
				strGetData = SetDlg.m_strHomeAddress;
			}
		}
		if(_T("REMARK") == strTemp)
		{
			if(TRUE == SetDlg.m_bEditRemark)
			{
				strGetData = SetDlg.m_strRemark;
			}
		}
		if(strGetData.IsEmpty()) continue;
		if(NULL != m_pApi && TRUE == bConnect)
		{
			if(_T("CIVIL_NO") == sDstFieldAlias[i] || _T("HANDPHONE") == sDstFieldAlias[i] || _T("IDX_HANDPHONE") == sDstFieldAlias[i] || _T("E_MAIL") == sDstFieldAlias[i] || _T("H_ADDR1") == sDstFieldAlias[i] || _T("H_PHONE") == sDstFieldAlias[i] || _T("W_PHONE") == sDstFieldAlias[i] || _T("W_ADDR1") == sDstFieldAlias[i])
			{
				if(TRUE == m_pApi->ConnectSocket())
				{
					DataArray.Add(strGetData);
					if(TRUE == m_pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), sDstFieldAlias[i], DataArray))
					{
						strGetData = DataArray.GetAt(0);
					}
					DataArray.RemoveAll(); DataArray.FreeExtra();	
					m_pApi->CloseSocket();
				}
			}
		}

		if(_T("Y") != strCivilNoUseYN) if(_T("CIVIL_NO") == sDstFieldAlias[i] || _T("CRYPTO_CIVIL_NO") == sDstFieldAlias[i]) continue;

		ids = pDM->AddDBFieldData(sDstFieldAlias[i], sDstFieldTypeAlias[i], strGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	}
	if(NULL != m_pApi)
	{
		delete m_pApi;
		m_pApi = NULL;
	}
	pSettingDM->GetCellData(_T("����"), 0, strGetData);
	pDM->AddDBFieldData(_T("USER_CLASS_CODE"), _T("STRING"), strGetData);
	pSettingDM->GetCellData(_T("�Ҽ�"), 0, strGetData);
	pDM->AddDBFieldData(_T("USER_POSITION_CODE"), _T("STRING"), strGetData);
	pSettingDM->GetCellData(_T("SMS���ſ���"), 0, strGetData);
	pDM->AddDBFieldData(_T("SMS_USE_YN"), _T("STRING"), strGetData);
	pSettingDM->GetCellData(_T("MAIL���ſ���"), 0, strGetData);
	pDM->AddDBFieldData(_T("MAILING_USE_YN"), _T("STRING"), strGetData);
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), m_pInfo->MANAGE_CODE);
	CString strSysDate;
	CTime tCurrent = m_pLoan->m_pLocCommonAPI->GetCurrentDateTime(this, strSysDate);

	pDM->AddDBFieldData(_T("KL_REG_DATE"), _T("NUMERIC"), _T("SYSDATE"));

	if(0 == nMode)
	{
		ids = pDM->AddDBFieldData(_T("KL_IMPORT_YN"), _T("STRING"), _T("Y"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
		ids = pDM->AddDBFieldData(_T("REG_DATE"), _T("NUMERIC"), _T("SYSDATE"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
		ids = pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("����ȸ���ڵ�����"), __WFILE__, __LINE__));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
		ids = pDM->MakeInsertFrame(_T("CO_LOAN_USER_TBL"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	}
	else
	{
		ids = pDM->AddDBFieldData(_T("KL_IMPORT_YN"), _T("STRING"), _T("N"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
		ids = pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("����ȸ���ڵ���ȯ"), __WFILE__, __LINE__));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
		ids = pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	}
	pDM->InitDBFieldData();
	// �����ӻ���
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K02_LOC_B01_SERVICE"));
	Writer.addRecord();
	CString strUserNo;
	ids = pDM->GetCellData(_T("���մ����ڹ�ȣ"), 0, strUserNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
	Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
	ids = pDM->GetCellData(_T("�������̸�"), 0, strGetData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
	Writer.addElement(_T("�������̸�"), strGetData);
	Writer.addElement(_T("������������ȣ"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("���ʻ�����"), strSysDate);
	CString strHandPhone;
	ids = pDM->GetCellData(_T("�ڵ���"), 0, strHandPhone);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
	Writer.addElement(_T("�ڵ���"), strHandPhone);
	ids = pDM->GetCellData(_T("G-PINASCII�����ڵ�"), 0, strGetData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
	Writer.addElement(_T("G-PINASCII�����ڵ�"), strGetData);
	CString strGpinHash;
	ids = pDM->GetCellData(_T("G-PINHASH�����ڵ�"), 0, strGpinHash);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
	Writer.addElement(_T("G-PINHASH�����ڵ�"), strGpinHash);
	ids = pDM->GetCellData(_T("G-PIN����"), 0, strGetData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
	Writer.addElement(_T("G-PIN����"), strGetData);
	Writer.addElement(_T("ȸ������"), _T("0"));
	ids = pDM->GetCellData(_T("����"), 0, strGetData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
	if(!strGetData.IsEmpty() && 3 < strGetData.GetLength())
	{
		Writer.addElement(_T("����"), strGetData.Left(4));
	}
	Writer.addElement(_T("ȸ������"), _T("0"));

	pDM->GetCellData(_T("IPIN_HASH"), 0, strGetData);
	if(!strGetData.IsEmpty()) Writer.addElement(_T("��������"), strGetData);

	CString strLastWork;
	if(0 == nMode)
	{
		strLastWork = GetWorkLogMsg(_T("����ȸ���ڵ������Ļ���"), __WFILE__, __LINE__);
	}
	else
	{
		strLastWork = GetWorkLogMsg(_T("����ȸ���ڵ���ȯ�Ļ���"), __WFILE__, __LINE__);
	}

 	CFrameSender Sender(m_pInfo);
 	CFrameReader Reader;
	INT nRetryCnt = 3;
	BOOL bComplete = FALSE;
	for(INT j = 0; j < nRetryCnt; j++)
	{
 		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0 > ids)
		{
			bComplete = FALSE;
			break;
		}
		else
		{
			CESL_DataMgr TempDM;
 			Reader.makeDM(&TempDM);
 			strTemp = Reader.getCommand();
 			if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
 			{
 				bComplete = FALSE;
 			}
			else
			{
				TempDM.GetCellData(_T("ó�����"), 0, strTemp);
				if(_T("OK") == strTemp)
				{
					bComplete = TRUE;
					break;
				}
				else
				{
					bComplete = FALSE;
				}
			}
		}
	}
	if(FALSE == bComplete)
	{
 	 	Sender.SendTempTbl(&Writer, strLastWork);
	}
	CString strAgreementInfo, strAgentInfo;
	pDM->GetCellData(_T("��������"), 0, strAgreementInfo);
	if(FALSE == strAgreementInfo.IsEmpty())
	{
		CStringArray asAgreementInfo;
		m_pLoan->m_pLocCommonAPI->MakeArrayList(strAgreementInfo, _T("��"), asAgreementInfo);
		if(6 == asAgreementInfo.GetSize())
		{
			strQuery.Format(_T("DELETE CO_AGREEMENT_INFO_TBL WHERE USER_KEY=%s;"), strRecKey);
			pDM->AddFrame(strQuery);

			if(0 < m_pInfo->m_nUserInfoRenewalDay)
			{
				CString strExpireDate;
				CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime((CESL_Mgr*)this, strExpireDate);
				tCurrentTime += CTimeSpan(m_pInfo->m_nUserInfoRenewalDay, 0,0,0);
				strExpireDate = tCurrentTime.Format("%Y/%m/%d");
				strQuery.Format(_T("INSERT INTO CO_AGREEMENT_INFO_TBL(USER_KEY,AGREE_TYPE,STATEMENT_ALIAS,AGREE_YN,AGREE_DATE,LIB_CODE,LIB_NAME,WORKER,FIRST_WORK, EXPIRE_DATE) VALUES(%s,'%s','å��������','%s',TO_DATE('%s','YYYY/MM/DD HH24:MI:SS'),'%s','%s','%s','%s','%s');"), strRecKey, asAgreementInfo.GetAt(0), asAgreementInfo.GetAt(1), asAgreementInfo.GetAt(2), asAgreementInfo.GetAt(3), asAgreementInfo.GetAt(4), asAgreementInfo.GetAt(5), strLastWork, strExpireDate);
			}
			else
			{
				strQuery.Format(_T("INSERT INTO CO_AGREEMENT_INFO_TBL(USER_KEY,AGREE_TYPE,STATEMENT_ALIAS,AGREE_YN,AGREE_DATE,LIB_CODE,LIB_NAME,WORKER,FIRST_WORK) VALUES(%s,'%s','å��������','%s',TO_DATE('%s','YYYY/MM/DD HH24:MI:SS'),'%s','%s','%s','%s');"), strRecKey, asAgreementInfo.GetAt(0), asAgreementInfo.GetAt(1), asAgreementInfo.GetAt(2), asAgreementInfo.GetAt(3), asAgreementInfo.GetAt(4), asAgreementInfo.GetAt(5), strLastWork);
			}
		
			pDM->AddFrame(strQuery);
		}
	}
	pDM->GetCellData(_T("�븮������"), 0, strAgentInfo);
	if(FALSE == strAgentInfo.IsEmpty())
	{
		CStringArray asAgentInfo;
		m_pLoan->m_pLocCommonAPI->MakeArrayList(strAgentInfo, _T("��"), asAgentInfo);
		if(7 == asAgentInfo.GetSize())
		{
			strQuery.Format(_T("DELETE CO_AGENT_INFO_TBL WHERE USER_KEY=%s;"), strRecKey);
			pDM->AddFrame(strQuery);
			strQuery.Format(_T("INSERT INTO CO_AGENT_INFO_TBL(USER_KEY,NAME,RELATION,NOTE,CERT_DATE,LIB_CODE,LIB_NAME,WORKER,FIRST_WORK) VALUES(%s,'%s','%s','%s',TO_DATE('%s','YYYY/MM/DD HH24:MI:SS'),'%s','%s','%s','%s');"), strRecKey, asAgentInfo.GetAt(0), asAgentInfo.GetAt(1), asAgentInfo.GetAt(2), asAgentInfo.GetAt(3), asAgentInfo.GetAt(4), asAgentInfo.GetAt(5), asAgentInfo.GetAt(6), strLastWork);
			pDM->AddFrame(strQuery);
		}
	}
	
	ids = pDM->SendFrame();
	if(0 > ids) return -1;
	pDM->EndFrame();

	pDM->FreeData();

	if(TRUE == SetDlg.m_bEditEmail || TRUE == SetDlg.m_bEditHandPhone || TRUE == SetDlg.m_bEditHomeAddress)
	{
		Writer.clear();
		Writer.setCommand(_T("IL_K24_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
		Writer.addElement(_T("G-PINHASH�����ڵ�"), strGpinHash);
		if(TRUE == SetDlg.m_bEditEmail)
		{
			Writer.addElement(_T("EMAIL"), SetDlg.m_strEmail);
		}
		if(TRUE == SetDlg.m_bEditHandPhone)
		{
			Writer.addElement(_T("�ڵ���"), SetDlg.m_strHandPhone);
			Writer.addElement(_T("�˻����ڵ���"), strIdxHandPhone);
		}
		if(TRUE == SetDlg.m_bEditHomeAddress)
		{
			Writer.addElement(_T("���ּ�"), SetDlg.m_strHomeAddress);
		}
		Writer.addElement("��������", strSysDate);
		Writer.addElement("������������ȣ", m_pInfo->LIB_CODE);
		Writer.addElement("�˻���CI", strCI);
		strLastWork = GetWorkLogMsg(_T("����ȸ���ڵ������ļ���"), __FILE__, __LINE__);
 	 	Sender.SendTempTbl(&Writer, strLastWork);
	}
	SetMessage(FALSE, _T("�ڰ��� ��Ȳ ������ �ø��� ���Դϴ�."));
	if(1 == nMode)
	{
		ids = m_pLoan->m_pLocCommonAPI->KL_CreateCurrentLoanInfo(this, strRecKey, strUserNo);
		if(0 > ids) return 200;
	}
	return 0;

EFS_END
return -1;
}

INT CKL_UserSearchDlg::StartUserSearch()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("StartUserSearch");

	INT nSearchMode = 0;
	CString strWhere;
	if(!m_strUserNo.IsEmpty())
	{
		nSearchMode = 0;
		strWhere.Format(_T("USER_NO='%s'"), m_strUserNo);
	}
	else if(!m_strWorkNo.IsEmpty())
	{
		nSearchMode = 1;
		strWhere.Format(_T("WORKNO='%s'"), m_strWorkNo);
	}
	else if(!m_strRegNo.IsEmpty())
	{
		if(m_strLibCode.IsEmpty())
		{
			AfxMessageBox(_T("������ �����մϴ�."));
			strFuncName.Empty();
			strWhere.Empty();
			return -1;
		}
		nSearchMode = 3;
	}
	else
	{
		AfxMessageBox(_T("������ �����մϴ�."));
		strFuncName.Empty();
		strWhere.Empty();
		return -1;
	}
	SetMessage(FALSE, _T("å����ȸ�� ������ ��ȸ�մϴ�."));
	CString strRecKey;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	if(3 != nSearchMode)
 	{
		CString strQuery;
		strQuery.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE %s"), strWhere);	
		TmpDM.GetOneValueQuery(strQuery, strRecKey);
		strQuery.Empty();
 	}
	strWhere.Empty();
	
	if(m_pInfo->IsConnectUnityLoanService())
	{
		if(strRecKey.IsEmpty())
		{
			CFrameWriter Writer(m_pInfo);
			Writer.setCommand(_T("IL_K08_LOC_B01_SERVICE"));
			Writer.addRecord();
			if(!m_strUserNo.IsEmpty())
			{
				Writer.addElement(_T("���մ����ڹ�ȣ"), m_strUserNo);
			}
			else if(!m_strWorkNo.IsEmpty())
			{
				Writer.addElement(_T("����ȸ����RFIDUID"), m_strWorkNo);
			}
			else if(!m_strRegNo.IsEmpty())
			{
				Writer.addElement(_T("���⵵������ȣ"), m_strLibCode);
				Writer.addElement(_T("��Ϲ�ȣ"), m_strRegNo);
			}
			else
			{
				AfxMessageBox(_T("������ �����մϴ�."));
				strFuncName.Empty();
				strRecKey.Empty();
				return -1;
			}
			CFrameReader Reader;
			CFrameSender Sender(m_pInfo);
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if(0 > ids)
			{
				CString strMsg = Sender.getError(ids);
				if(strMsg.IsEmpty())
				{
					SetMessage(TRUE, _T("�̵������� ��ſ� �����Ͽ����ϴ�."));
				}
				else
				{
					SetMessage(TRUE, strMsg);
				}
				strFuncName.Empty();
				strRecKey.Empty();
				return -1;
			}
			ids = Reader.makeDM(&TmpDM);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

			if(0 < TmpDM.GetRowCount() && TRUE == m_pInfo->m_bLibSecuUse)
			{
				CString strGetData;
				CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7,asDes8;
				for(INT i = 0; i < TmpDM.GetRowCount(); i++)
				{
					TmpDM.GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strGetData);
					asDes1.Add(strGetData);
					TmpDM.GetCellData(_T("���ּ�"), i, strGetData);
					asDes2.Add(strGetData);
					TmpDM.GetCellData(_T("�ٹ����ּ�"), i, strGetData);
					asDes3.Add(strGetData);
					TmpDM.GetCellData(_T("����ȭ��ȣ"), i, strGetData);
					asDes4.Add(strGetData);
					TmpDM.GetCellData(_T("�ٹ�����ȭ��ȣ"), i, strGetData);
					asDes5.Add(strGetData);
					TmpDM.GetCellData(_T("�ڵ���"), i, strGetData);
					asDes6.Add(strGetData);
					TmpDM.GetCellData(_T("�˻����ڵ���"), i, strGetData);
					asDes7.Add(strGetData);
					TmpDM.GetCellData(_T("EMAIL"), i, strGetData);
					asDes8.Add(strGetData);
				}
				if(NULL != m_pApi)
				{
					delete m_pApi;
					m_pApi = NULL;
				}
				m_pApi = new CBO_LOC_3000Api(this);
				if(NULL != m_pApi)
				{
					if(TRUE == m_pApi->CheckConnection())
					{
						if(TRUE == m_pApi->ConnectSocket()) 
						{
							m_pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
							m_pApi->CloseSocket();
						}
						if(TRUE == m_pApi->ConnectSocket()) 
						{
							m_pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes2);
							m_pApi->CloseSocket();
						}
						if(TRUE == m_pApi->ConnectSocket()) 
						{
							m_pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes3);
							m_pApi->CloseSocket();
						}
						if(TRUE == m_pApi->ConnectSocket()) 
						{
							m_pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes4);
							m_pApi->CloseSocket();
						}
						if(TRUE == m_pApi->ConnectSocket()) 
						{
							m_pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes5);
							m_pApi->CloseSocket();
						}
						if(TRUE == m_pApi->ConnectSocket()) 
						{
							m_pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes6);
							m_pApi->CloseSocket();
						}
						if(TRUE == m_pApi->ConnectSocket()) 
						{
							m_pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("IDX_HANDPHONE"), asDes7);
							m_pApi->CloseSocket();
						}
						if(TRUE == m_pApi->ConnectSocket()) 
						{
							m_pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes8);
							m_pApi->CloseSocket();
						}
						for(i = 0; i < TmpDM.GetRowCount(); i++)
						{
							TmpDM.SetCellData(_T("�ֹε�Ϲ�ȣ"), asDes1.GetAt(i), i);
							TmpDM.SetCellData(_T("���ּ�"), asDes2.GetAt(i), i);
							TmpDM.SetCellData(_T("�ٹ����ּ�"), asDes3.GetAt(i), i);
							TmpDM.SetCellData(_T("����ȭ��ȣ"), asDes4.GetAt(i), i);
							TmpDM.SetCellData(_T("�ٹ�����ȭ��ȣ"), asDes5.GetAt(i), i);
							TmpDM.SetCellData(_T("�ڵ���"), asDes6.GetAt(i), i);
							TmpDM.SetCellData(_T("�˻����ڵ���"), asDes7.GetAt(i), i);
							TmpDM.SetCellData(_T("EMAIL"), asDes8.GetAt(i), i);
						}
					}
					delete m_pApi;
					m_pApi = NULL;
				}
				asDes1.RemoveAll(); asDes1.FreeExtra();
				asDes2.RemoveAll(); asDes2.FreeExtra();
				asDes3.RemoveAll(); asDes3.FreeExtra();
				asDes4.RemoveAll(); asDes4.FreeExtra();
				asDes5.RemoveAll(); asDes5.FreeExtra();
				asDes6.RemoveAll(); asDes6.FreeExtra();
				asDes7.RemoveAll(); asDes7.FreeExtra();
				asDes8.RemoveAll(); asDes8.FreeExtra();
			}

			CString strTemp = Reader.getCommand();
			BOOL bNotSearch = FALSE;
			if(_T("ERROR") == strTemp)
			{
				CString strMsg,strTemp;
				Reader.getError(strTemp, strMsg);
				if(0 < strTemp.Find(_T("DT01")))
				{
					bNotSearch = TRUE;
				}
				else if(0 < strTemp.Find(_T("LCTB01DB03")))
				{
					bNotSearch = TRUE;
				}
				else if(0 < strTemp.Find(_T("DT02")))
				{
					SetMessage(TRUE, _T("å����ȸ�� �ߺ��� �߻��߽��ϴ�."));
					strFuncName.Empty();
					strRecKey.Empty();
					return 300;
				}
				else
				{
					if(strMsg.IsEmpty())
					{
						SetMessage(TRUE, _T("�̵����� ������ �߻��Ͽ����ϴ�."));
					}
					else
					{
						SetMessage(TRUE, strMsg);
					}
					strMsg.Format(_T("����ȸ����ȸ(8) ERROR CODE : %s"), strTemp);
					TmpDM.QueryLog(strMsg.GetBuffer(0));
					return -1;
				}
			}
			if(1 > TmpDM.GetRowCount() || TRUE == bNotSearch)
			{
				if(m_strRegNo.IsEmpty())
				{
					Writer.clear();
					Writer.setCommand(_T("IL_K48_LOC_B01_SERVICE"));
					Writer.addRecord();
					if(!m_strUserNo.IsEmpty())
					{
						Writer.addElement(_T("���մ����ڹ�ȣ"), m_strUserNo);
					}
					else if(!m_strWorkNo.IsEmpty())
					{
						Writer.addElement(_T("����ȸ����RFIDUID"), m_strWorkNo);
					}
					Reader.clear();
					ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
					if(0 > ids)
					{
						CString strMsg = Sender.getError(ids);
						if(strMsg.IsEmpty())
						{
							SetMessage(TRUE, _T("�̵������� ��ſ� �����Ͽ����ϴ�."));
						}
						else
						{
							SetMessage(TRUE, strMsg);
						}
						strFuncName.Empty();
						strRecKey.Empty();
						return -1;
					}
					CESL_DataMgr TempDM;
					Reader.makeDM(&TempDM);
					CString strTemp = Reader.getCommand();
					if(_T("ERROR")==strTemp)
					{
						CString strMsg,strTemp;
						Reader.getError(strTemp, strMsg);
						if(0 < strTemp.Find(_T("DT")) || 0 < strTemp.Find(_T("LCTB01DB03")))
						{
							SetMessage(TRUE, _T("��ȸ�� å����ȸ���� �����ϴ�."));
							strFuncName.Empty();
							strRecKey.Empty();
							return 300;
						}
						else
						{
							if(strMsg.IsEmpty())
							{
								SetMessage(TRUE, _T("�̵����� ������ �߻��Ͽ����ϴ�."));
							}
							else
							{
								SetMessage(TRUE, strMsg);
							}
							strMsg.Format(_T("���Ե��������(5) ERROR CODE : %s"), strTemp);
							TempDM.QueryLog(strMsg.GetBuffer(0));
							return -1;
						}
					}
					BOOL bIsSecede = FALSE;
					for(INT i = 0; i < TempDM.GetRowCount(); i++)
					{
						CString strValue;
						TempDM.GetCellData(_T("���Ե�������ȣ"), i, strValue);
						strValue.TrimLeft(); strValue.TrimRight();
						if(strValue == m_pInfo->LIB_CODE)
						{
							TempDM.GetCellData(_T("ȸ������"), i, strValue);
							strValue.TrimLeft(); strValue.TrimRight();
							if(_T("3") == strValue)
							{
								bIsSecede = TRUE;
								break;
							}
						}
					}
					if(TRUE == bIsSecede)
					{
						SetMessage(FALSE, _T("Ż��ó���� ȸ���Դϴ�."));
					}
					else
					{
						SetMessage(FALSE, _T("��ȸ�� å����ȸ���� �����ϴ�."));
					}
				}
				else
				{
					SetMessage(FALSE, _T("��ȸ�� å����ȸ���� �����ϴ�."));
				}
				strFuncName.Empty();
				strRecKey.Empty();
				return 300;
			}
			if(3 == nSearchMode)
 			{
				CString strUserNo;
				ids = TmpDM.GetCellData(_T("���մ����ڹ�ȣ"), 0, strUserNo);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
				CString strQuery;
				strQuery.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE USER_NO='%s'"), strUserNo);	
				TmpDM.GetOneValueQuery(strQuery, strRecKey);
				strQuery.Empty();
 			}
			if(strRecKey.IsEmpty())
			{
				ids = InsertUser(&TmpDM, strRecKey);		
				if(0 > ids)
				{
					SetMessage(TRUE, _T("���� ȸ�� ������ �����Ͽ����ϴ�."));
					strFuncName.Empty();
					strRecKey.Empty();
					return -1;
				}
				else if(100 == ids)
				{
					SetMessage(TRUE, _T("�ڰ� ���� ȸ�� ������ �����Ͽ����ϴ�."));
					strFuncName.Empty();
					strRecKey.Empty();
					return -1;
				}
				else if(200 == ids)
				{
					SetMessage(TRUE, _T("������Ȳ �ø��⿡ �����Ͽ����ϴ�."));
					strFuncName.Empty();
					strRecKey.Empty();
					return -1;
				}
				else if(300 == ids)
				{
					SetMessage(TRUE, _T("Ż��ó���� ȸ���Դϴ�."));
					strFuncName.Empty();
					strRecKey.Empty();
					return -1;
				}
				else if(400 == ids)
				{
					SetMessage(TRUE, _T("�Ǹ����� �̵���� ���ӽ���"));
					strFuncName.Empty();
					strRecKey.Empty();
					return -1;
				}
				else if(500 == ids)
				{
					SetMessage(TRUE, _T("���Ѿ��� ����ڴ� ������ �Ұ����մϴ�."));
					strFuncName.Empty();
					strRecKey.Empty();
					return -1;
				}
			}
		}
	}
	
	if(strRecKey.IsEmpty()) return 0;
	
	SetMessage(FALSE, _T(""));
	ids = m_pParent->SendMessage(WM_SEARCH_USER_FROM_KEY, 0, (WPARAM)&strRecKey);
	strRecKey.Empty();
	if(0 > ids)
	{
		SetMessage(TRUE, _T("ȸ���� �ҷ����� �߿� ������ �߻��Ͽ����ϴ�."));
		strFuncName.Empty();
		return -1;
	}
	strFuncName.Empty();

	return 0;

EFS_END
return -1;
}

INT CKL_UserSearchDlg::StartUserLoanInfo()
{
	INT ids;
	CString strFuncName = _T("StartUserLoanInfo");

	SetMessage(FALSE, _T("���� ��Ȳ�� ��ȸ�մϴ�."));
	CString strErrorMsg;
	ids = m_pLoan->KL_SearchLoanInfo(strErrorMsg);
	if(0 > ids)
	{
		SetMessage(TRUE, _T("������Ȳ ��ȸ�� �����Ͽ����ϴ�."));
		strFuncName.Empty();
		return -1;
	}
	else if(100 == ids)
	{
		SetMessage(FALSE, _T("å����ȸ���� �ƴմϴ�."));
		strFuncName.Empty();
		return 200;
	}
	else if(200 == ids)
	{
		SetMessage(TRUE, strErrorMsg);
		strFuncName.Empty();
		return -1;
	}

	strFuncName.Empty();

	return 0;
}

INT CKL_UserSearchDlg::IsLoanStopDate()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsLoanStopDate");

	SetMessage(FALSE, _T("������ �������� ������ Ȯ�����Դϴ�."));
	CESL_DataMgr *pMemberDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	if(1 > pMemberDM->GetRowCount())
	{
		return 0;
	}
	if(m_strUserNo.IsEmpty())
	{
		ids = pMemberDM->GetCellData(_T("�����ڹ�ȣ"), 0, m_strUserNo);
		if(0 > ids || m_strUserNo.IsEmpty()) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	}
	CString strGpinHash;
	pMemberDM->GetCellData(_T("G-PINHASH�����ڵ�"), 0, strGpinHash);
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K29_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), m_strUserNo);
	Writer.addElement(_T("G-PINHASH�����ڵ�"), strGpinHash);
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	INT nRetryCnt = 3;
	BOOL bComplete = FALSE;
	CString strGetData;
	for(INT i = 0; i < nRetryCnt; i++)
	{
 		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0 > ids)
		{
			bComplete = FALSE;
			break;
		}
		else
		{
			CESL_DataMgr TempDM;
 			Reader.makeDM(&TempDM);
 			CString strTemp = Reader.getCommand();
 			if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
 			{
 				bComplete = FALSE;
 			}
			else
			{
				strGetData = _T("");
				TempDM.GetCellData(_T("������������"), 0, strGetData);
				if(!strGetData.IsEmpty())
				{
					bComplete = TRUE;
					break;
				}
				else
				{
					bComplete = FALSE;
				}
			}
		}
	}
	if(FALSE == bComplete)
	{
 	 	SetMessage(TRUE, _T("������������ ��ȸ�� �����Ͽ����ϴ�."));
		strFuncName.Empty();
		return -1;
	}
	else
	{
		ids = pMemberDM->SetCellData(_T("������������"), strGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	}

	strFuncName.Empty();
	return 0;

EFS_END
return -1;
}

INT CKL_UserSearchDlg::StartMaxLoanCount()
{
	INT ids;
	CString strFuncName = _T("StartMaxLoanCount");

	m_strMaxLoanCount = _T("20");
	SetMessage(FALSE, _T("���� ���Ⱑ�� �Ǽ��� ��ȸ�մϴ�."));
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K27_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), m_pInfo->LIB_CODE);
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		CString strMsg = Sender.getError(ids);
		if(strMsg.IsEmpty())
		{
			SetMessage(TRUE, _T("�̵������� ��ſ� �����Ͽ����ϴ�."));
		}
		else
		{
			SetMessage(TRUE, strMsg);
		}
		strFuncName.Empty();
		return -1;
	}
	CESL_DataMgr TmpDM;
	Reader.makeDM(&TmpDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strMsg,strTemp;
		Reader.getError(strTemp, strMsg);
		
		if(strMsg.IsEmpty())
		{
			SetMessage(TRUE, _T("�̵����� ������ �߻��Ͽ����ϴ�."));
		}
		else
		{
			SetMessage(TRUE, strMsg);
		}	
		strMsg.Format(_T("���մ��Ⱑ��å����ȸ(27) ERROR CODE : %s"), strTemp);
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return -1;
	}
	if(1 > TmpDM.GetRowCount())
	{
		SetMessage(TRUE, _T("���������մ��Ⱑ��å�� ������ �����ϴ�."));
		return -1;
	}
	else
	{
		TmpDM.GetCellData(_T("���������մ��Ⱑ��å��"), 0, m_strMaxLoanCount);
		if(!m_strMaxLoanCount.IsEmpty())
		{
			m_pLoan->SetTempData(_T("�⺻���մ��Ⱑ�ɱǼ�"), CComVariant(m_strMaxLoanCount));
		}
	}

	strFuncName.Empty();

	return 0;
}

INT CKL_UserSearchDlg::IsPaymentFree()
{
	INT ids;
	CString strFuncName = _T("IsPaymentFree");

	SetMessage(FALSE, _T("�ù�� ���Ῡ�θ� Ȯ�����Դϴ�."));
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K32_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), m_pInfo->LIB_CODE);
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		CString strMsg = Sender.getError(ids);
		if(strMsg.IsEmpty())
		{
			SetMessage(TRUE, _T("�̵������� ��ſ� �����Ͽ����ϴ�."));
		}
		else
		{
			SetMessage(TRUE, strMsg);
		}
		strFuncName.Empty();
		m_strLocalFreeYN = _T("N");
		m_strOtherFreeYN = _T("N");
		return -1;
	}
	CESL_DataMgr TmpDM;
	Reader.makeDM(&TmpDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp || 1 > TmpDM.GetRowCount())
	{
		CString strMsg,strTemp;
		Reader.getError(strTemp, strMsg);
		
		if(strMsg.IsEmpty())
		{
			SetMessage(TRUE, _T("�̵����� ������ �߻��Ͽ����ϴ�."));
		}
		else
		{
			SetMessage(TRUE, strMsg);
		}
		strMsg.Format(_T("�ù�񹫷Ῡ��(32) ERROR CODE : %s"), strTemp);
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		m_strLocalFreeYN = _T("N");
		m_strOtherFreeYN = _T("N");
		strFuncName.Empty();
		return -1;
	}
	CString strGetData;
	TmpDM.GetCellData(_T("�������ù�񹫷Ῡ��"), 0, strGetData);
	if(_T("Y") != strGetData)
	{
		strGetData = _T("N");
	}
	m_strLocalFreeYN = strGetData;
	TmpDM.GetCellData(_T("�������ù�񹫷Ῡ��"), 0, strGetData);
	if(_T("Y") != strGetData)
	{
		strGetData = _T("N");
	}
	m_strOtherFreeYN = strGetData;

	Writer.clear();
	Writer.setCommand(_T("IL_K17_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), m_pInfo->LIB_CODE);
	Reader.clear();
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		SetMessage(TRUE, _T("���� ������ ��ȸ�� �����Ͽ����ϴ�."));
		return ids;
	}
	CESL_DataMgr *pDM = m_pLoan->m_pParentMgr->FindDM(_T("DM_KL_LIB_LIST"));
	Reader.makeDM(pDM);

	strFuncName.Empty();

	return 0;
}

INT CKL_UserSearchDlg::GetManage()
{
	INT ids;
	CString strFuncName = _T("GetManage");

	m_strMaxLoanCount = _T("");
	m_strLocalFreeYN = _T("");
	m_strOtherFreeYN = _T("");
	m_strAutoSendYN = _T("");
	m_strLibCodeList = _T("");
	m_ExpireAddHolyday = _T("");
	SetMessage(FALSE, _T("å���� ������ ��ȸ���Դϴ�."));
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K43_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), m_pInfo->LIB_CODE);
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	CESL_DataMgr TmpDM;
	if(1 == ids)
	{
		Reader.makeDM(&TmpDM);
		TmpDM.GetCellData(_T("�⺻���մ��Ⱑ�ɱǼ�"), 0, m_strMaxLoanCount);
		if(!m_strMaxLoanCount.IsEmpty()) m_pLoan->SetTempData(_T("�⺻���մ��Ⱑ�ɱǼ�"), CComVariant(m_strMaxLoanCount));
		TmpDM.GetCellData(_T("�������ù�񹫷Ῡ��"), 0, m_strLocalFreeYN);
		if(!m_strLocalFreeYN.IsEmpty()) m_pLoan->SetTempData(_T("�������ù�񹫷Ῡ��"), CComVariant(m_strLocalFreeYN));
		TmpDM.GetCellData(_T("�������ù�񹫷Ῡ��"), 0, m_strOtherFreeYN);
		if(!m_strOtherFreeYN.IsEmpty()) m_pLoan->SetTempData(_T("�������ù�񹫷Ῡ��"), CComVariant(m_strOtherFreeYN));
		TmpDM.GetCellData(_T("�ڵ��߼ۿ���"), 0, m_strAutoSendYN);
		if(!m_strAutoSendYN.IsEmpty()) m_pLoan->SetTempData(_T("�ڵ��߼ۿ���"), CComVariant(m_strAutoSendYN));
		TmpDM.GetCellData(_T("������ȣ�������⸸����"), 0, m_strLoanExpiry);
		if(!m_strLoanExpiry.IsEmpty()) m_pLoan->SetTempData(_T("������ȣ�������⸸����"), CComVariant(m_strLoanExpiry));
		TmpDM.GetCellData(_T("������ȣ����Ÿ���ݳ�����"), 0, m_strOtherReturnYN);
		if(!m_strLoanExpiry.IsEmpty()) m_pLoan->SetTempData(_T("������ȣ����Ÿ���ݳ�����"), CComVariant(m_strOtherReturnYN));
		TmpDM.GetCellData(_T("���⸸���Ͽ��ް������Կ���"), 0, m_ExpireAddHolyday);
		if(!m_ExpireAddHolyday.IsEmpty()) m_pLoan->SetTempData(_T("���⸸���Ͽ��ް������Կ���"), CComVariant(m_ExpireAddHolyday));
	}
	else
	{
		m_pLoan->GetTempData(_T("�⺻���մ��Ⱑ�ɱǼ�"), m_strMaxLoanCount);
		m_pLoan->GetTempData(_T("�������ù�񹫷Ῡ��"), m_strLocalFreeYN);
		m_pLoan->GetTempData(_T("�������ù�񹫷Ῡ��"), m_strOtherFreeYN);
		m_pLoan->GetTempData(_T("�ڵ��߼ۿ���"), m_strAutoSendYN);
		m_pLoan->GetTempData(_T("������ȣ����Ÿ���ݳ�����"), m_strOtherReturnYN);
		m_pLoan->GetTempData(_T("������ȣ�������⸸����"), m_strLoanExpiry);
		m_pLoan->GetTempData(_T("���⸸���Ͽ��ް������Կ���"), m_ExpireAddHolyday);
	}

	SetMessage(FALSE, _T("������ ����������Ʈ �������Դϴ�."));
	Writer.clear();
	Writer.setCommand(_T("IL_K17_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), m_pInfo->LIB_CODE);
	Reader.clear();
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(1 == ids)
	{
		TmpDM.FreeData();
		TmpDM.FreeDMRef();
		Reader.makeDM(&TmpDM);
		CString strGetData = _T("");
		for(int i = 0; i < TmpDM.GetRowCount(); i++)
		{
			TmpDM.GetCellData(_T("��������ȣ"), i, strGetData);
			if(!strGetData.IsEmpty()) 
			{
				if(!m_strLibCodeList.IsEmpty()) m_strLibCodeList += _T(",");
				m_strLibCodeList += _T("%") + strGetData + _T("%");
			}
		}
		strGetData.Empty();
		if(!m_strLibCodeList.IsEmpty())	m_pLoan->SetTempData(_T("��������������ȣ����Ʈ"), CComVariant(m_strLibCodeList));

	}
	else
	{
		if(!m_strLibCodeList.IsEmpty())	m_pLoan->GetTempData(_T("��������������ȣ����Ʈ"), m_strLibCodeList);	
	}

	strFuncName.Empty();

	return 0;
}

INT CKL_UserSearchDlg::GetLibConMng()
{
	INT ids;
	CString strFuncName = _T("GetLibConMng");
	if ( m_SearchLibCode.GetLength() <= 0 ) {
		AfxMessageBox(_T("�������ȣ�� �����ϴ�."));
		return 0;
	}
	
	SetMessage(FALSE, _T("������� �ݳ�������뿩�θ� ��ȸ���Դϴ�."));
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K43_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), m_SearchLibCode);
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);	
	if(1 != ids)  {
		m_strBookLibReloanYN = _T("N");
		return 0;
	}

	CESL_DataMgr TmpDM;
	Reader.makeDM(&TmpDM);
	TmpDM.GetCellData(_T("��ȣ�����ڷ�ݳ�������뿩��"), 0, m_strBookLibReloanYN);				
	
	return 0;
}

INT CKL_UserSearchDlg::StartOtherLoanInfo(INT &nIndex)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("StartOtherLoanInfo");

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K20_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), m_strUserNo);
	Writer.addElement(_T("G-PINHASH�����ڵ�"), m_strGpinHash);
	Writer.addElement(_T("���⵵������ȣ"), m_strLibCode);
	Writer.addElement(_T("��Ϲ�ȣ"), m_strRegNo);
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	INT nRetryCnt = 3;
	BOOL bComplete = FALSE;
	CString strGetData;
	CESL_DataMgr TmpDM;
	for(INT i = 0; i < nRetryCnt; i++)
	{
 		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0 > ids)
		{
			bComplete = FALSE;
			break;
		}
		else
		{
 			Reader.makeDM(&TmpDM);
 			CString strTemp = Reader.getCommand();
 			if(_T("ERROR") == strTemp)
 			{
 				if(0 < strTemp.Find(_T("DT")))
				{
					return 100;
				}
				else
				{
 					bComplete = FALSE;
				}
 			}
			else if(1 > TmpDM.GetRowCount())
			{
				return 100;
			}
			else
			{
				bComplete = TRUE;
				break;
			}
		}
	}
	if(FALSE == bComplete)
	{
 	 	SetMessage(TRUE, _T("Ÿ���ݳ� ��Ȳ ��ȸ�� �����Ͽ����ϴ�."));
		strFuncName.Empty();
		return -1;
	}

	CESL_DataMgr *pLoanDM = FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
 	if(NULL == pLoanDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

	INT nInsertRow = 0;
	CString strData;
	for(INT j = 0; j < TmpDM.GetRowCount(); j++)
	{
		pLoanDM->InsertRow(-1);
		nInsertRow = pLoanDM->GetRowCount() - 1;

		ids = TmpDM.GetCellData(_T("���⵵����"), j, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		ids = pLoanDM->SetCellData(_T("���⵵������ȣ"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		ids = TmpDM.GetCellData(_T("������"), j, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		ids = pLoanDM->SetCellData(_T("������"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		ids = TmpDM.GetCellData(_T("�ݳ�������"), j, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		ids = pLoanDM->SetCellData(_T("�ݳ�������"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		ids = TmpDM.GetCellData(_T("����"), j, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		ids = pLoanDM->SetCellData(_T("�������"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		ids = TmpDM.GetCellData(_T("��Ϲ�ȣ"), j, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		ids = pLoanDM->SetCellData(_T("��Ϲ�ȣ"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		ids = TmpDM.GetCellData(_T("����"), j, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		ids = pLoanDM->SetCellData(_T("��ǥ��"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		ids = TmpDM.GetCellData(_T("û����ȣ"), j, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		ids = pLoanDM->SetCellData(_T("û����ȣ"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		ids = TmpDM.GetCellData(_T("����ó��KEY"), j, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		ids = pLoanDM->SetCellData(_T("����ó��KEY"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		ids = TmpDM.GetCellData(_T("����KEY"), j, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		ids = pLoanDM->SetCellData(_T("����KEY"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		ids = pLoanDM->SetCellData(_T("��Ȳ����"), _T("����"), nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		TmpDM.GetCellData(_T("�ݳ�������������ȣ"), j, strData);
		ids = pLoanDM->SetCellData(_T("�ݳ�������������ȣ"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		TmpDM.GetCellData(_T("�ݳ�������������"), j, strData);
		ids = pLoanDM->SetCellData(_T("�ݳ�������������"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		TmpDM.GetCellData(_T("��������ȣ��������"), j, strData);
		if(strData.IsEmpty()) strData = _T("N");
		ids = pLoanDM->SetCellData(_T("��������ȣ��������"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);

		TmpDM.GetCellData(_T("Ÿ���������"), j, strData);
		ids = pLoanDM->SetCellData(_T("Ÿ���������"), strData, nInsertRow);
		
		nIndex = nInsertRow;
	}

	strFuncName.Empty();
	strData.Empty();

	return 0;

EFS_END
return -1;
}

INT CKL_UserSearchDlg::StartOtherReturn()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("StartOtherReturn");
	SetMessage(FALSE, _T("Ÿ���ݳ��� ���� ��Ȳ�� Ȯ�� �մϴ�."));
	CESL_DataMgr *pMemberDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	
	INT nIndex = 0;
	BOOL bExist = IsSearchLoanInfo(nIndex);
	if(FALSE == bExist)
	{
		CString strPrevUserNo, strUserNo;
		ids = pMemberDM->GetCellData(_T("�����ڹ�ȣ"), 0, strPrevUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		ids = StartUserSearch();
		if(0 > ids) 
		{
			strFuncName.Empty();
			return ids;
		}
		CString strKLUserYN;
		pMemberDM->GetCellData(_T("����ȸ������"), 0, strKLUserYN);
		if(_T("Y") != strKLUserYN)
		{
			m_bKLUserYN = FALSE;
			return 0;
		}
		ids = pMemberDM->GetCellData(_T("�����ڹ�ȣ"), 0, strUserNo);
		if(0 > ids || strUserNo.IsEmpty()) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
		bExist = IsSearchLoanInfo(nIndex);
		if(FALSE == bExist)
		{
			pMemberDM->GetCellData(_T("�����ڹ�ȣ"), 0, m_strUserNo);
			pMemberDM->GetCellData(_T("G-PINHASH�����ڵ�"), 0, m_strGpinHash);
			ids = StartOtherLoanInfo(nIndex);
			if(0 > ids) 
			{
				strFuncName.Empty();
				return ids;
			}
			if(100 == ids)
			{
				strFuncName.Empty();
				SetMessage(FALSE, _T("���� ��Ȳ�� ã�� �� �����ϴ�."));
				return 300;
			}
		}

		if(m_pLoan->m_pManageValue->m_nSetloanStopMode && strUserNo!=strPrevUserNo)
		{
			SetMessage(FALSE, _T("Ȯ�� �� �ݳ� ó���մϴ�."));
			m_bLoanStopMode = TRUE;
			m_nIndex  = nIndex;
			return 0;
		}
		else
		{
			CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid"));
			if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
			pGrid->SetAt(nIndex, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
			pGrid->m_nReverse = nIndex;
			pGrid->SetReverse(nIndex);
		}
	}

	CESL_DataMgr *pLoanDM = FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pLoanDM) return FALSE;
	CString strInterlibraryLoan = pLoanDM->GetCellData(_T("��������ȣ��������"), nIndex);
	if(_T("Y") == strInterlibraryLoan)
	{
		strFuncName.Empty();
		SetMessage(TRUE, _T("������ ��ȣ���� �ڷ��Դϴ�."));
		return 200;
	}
		
	CString strOtherReturnYN = _T("");
	m_pLoan->GetTempData(_T("������ȣ����Ÿ���ݳ�����"), strOtherReturnYN);
	if(strOtherReturnYN.IsEmpty())
	{
		strOtherReturnYN = _T("N");
	}
	if(_T("N")==strOtherReturnYN)
	{
		strFuncName.Empty();
		SetMessage(TRUE, _T("Ÿ���ݳ� �Ұ� ������."));
		return 200;
	}

	if(!IsLibCodeList())
	{
		strFuncName.Empty();
		SetMessage(TRUE, _T("Ÿ ���� �ڷ�� �ݳ��� �� �����ϴ�."));		
		return 200;
	}

	SetMessage(FALSE, _T("�������� Ȯ�����Դϴ�."));
	ids = AskPayment(nIndex);
	if(0 > ids) 
	{
		strFuncName.Empty();
		SetMessage(TRUE, _T("������ �߻��Ͽ����ϴ�."));
		return ids;
	}
	else if(100 == ids)
	{
		strFuncName.Empty();
		SetMessage(TRUE, _T("�����Ŀ� �ݳ�ó�� �Ͻʽÿ�."));
		return 200;
	}
	else if(200 == ids)
	{
		strFuncName.Empty();
		SetMessage(TRUE, _T("�̵���� ���ӿ���"));
		return 200;
	}
	else if(300 == ids)
	{
		strFuncName.Empty();
		SetMessage(TRUE, _T("Ÿ���ݳ��� ��ҵǾ����ϴ�."));
		return 300;
	}
	else if(400 == ids)
	{
		strFuncName.Empty();
		return 300;
	}
	SetMessage(FALSE, _T("Ÿ���ݳ� ó�����Դϴ�."));

	CString strMessage;
	CString strRegNo =  pLoanDM->GetCellData(_T("��Ϲ�ȣ"), nIndex);
	CStringArray AppendixNum;
	int nAppendixNo = -1;
	this->GetAppendix( pLoanDM, nIndex, AppendixNum );
	int nAppendixCnt = AppendixNum.GetSize();
	if ( nAppendixCnt > 0 )
	{
		CString strAppendixRegNo = _T("");
		for (int i = 0 ; i < AppendixNum.GetSize() ; i++ )
		{
			nAppendixNo = FindIdxFromBookKey( pLoanDM , AppendixNum.GetAt(i) );
			if( nAppendixNo < 0 ) return nAppendixNo;
			if(i == 0 ){
				strAppendixRegNo = _T("[") + pLoanDM->GetCellData(_T("��Ϲ�ȣ"), nAppendixNo ) +_T(":") + pLoanDM->GetCellData(_T("��ǥ��"), nAppendixNo ) + _T("]");
			}
			else {
				strAppendixRegNo += _T("\r\n[") + pLoanDM->GetCellData(_T("��Ϲ�ȣ"), nAppendixNo  ) +_T(":") + pLoanDM->GetCellData(_T("��ǥ��"), nAppendixNo ) + _T("]");
			}			
		}
		strMessage.Format(_T("�η� �ڷ�� �Բ� ����� �����Դϴ�.\r\n%s\r\n\r\n�Բ� �ݳ��Ͻðڽ��ϱ�?\r\n(�� : ���̹ݳ� , �ƴϿ� : �����ڷḸ�ݳ�)"),strAppendixRegNo);

		UINT uResult = this->SendMessage(WM_MESSAGEBOX, NULL, (LPARAM)&strMessage);

		if(0 != uResult)
		{
			AppendixNum.RemoveAll();
		}
	}
	else if(FALSE == m_bOtherReturnFirst)
	{
		strMessage.Format(_T("[%s] �ڷῡ ���� Ÿ���ݳ��Ͻðڽ��ϱ�?"), strRegNo);
		UINT uResult = this->SendMessage(WM_MESSAGEBOX, NULL, (LPARAM)&strMessage);
		if(0 != uResult)
		{
			return uResult;
		}
	}
	AppendixNum.Add(pLoanDM->GetCellData(_T("åKEY"),nIndex));
	for(int j = 0 ; j < AppendixNum.GetSize() ; j++ ){

	nIndex = FindIdxFromBookKey( pLoanDM , AppendixNum.GetAt(j) );
	if(nIndex < 0) return nIndex;
	CString strKey = pLoanDM->GetCellData(_T("����ó��KEY"), nIndex);
	CString strLoanLibCode = pLoanDM->GetCellData(_T("���⵵������ȣ"), nIndex);
	CString strLocalKey = pLoanDM->GetCellData(_T("����KEY"), nIndex);

	CFrameWriter Writer(m_pInfo);
 	Writer.setCommand(_T("IL_K46_LOC_B01_SERVICE"));
 	Writer.addRecord();
	CString strGetData;
	
	pMemberDM->GetCellData(_T("�����ڹ�ȣ"), 0, strGetData);
	Writer.addElement(_T("���մ����ڹ�ȣ"), strGetData);
 	Writer.addElement(_T("���⵵������ȣ"), strLoanLibCode);
 	Writer.addElement(_T("�ݳ���������ȣ"), m_pInfo->LIB_CODE);
	CString strReturnType = _T("�ݳ���");
	ids = GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("�ݳ�����"), strGetData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
	if(_T("2") == strGetData)  strReturnType = _T("�ұ޹ݳ���");
	CString strReturnDate;
	ids = GetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), strReturnType, strReturnDate );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
	CTime tCurrentTime = CTime::GetCurrentTime();
	CString strBuf;
	strBuf.Format(_T("%s %02d:%02d:%02d"), strReturnDate, tCurrentTime.GetHour(), tCurrentTime.GetMinute(), tCurrentTime.GetSecond());
 	Writer.addElement(_T("�ݳ���"), strBuf);
	Writer.addElement(_T("����ó��KEY"), strKey);
	Writer.addElement(_T("����KEY"), strLocalKey);
	Writer.addElement(_T("�۾���"), m_pInfo->USER_ID);
	Writer.addElement(_T("�������۾�"), GetWorkLogMsg(_T("���յ���Ÿ���ݳ�"), __WFILE__, __LINE__));

  	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0>ids)
	{
		strFuncName.Empty();
		SetMessage(TRUE, _T("�̵���� ���ӿ���"));
		return 200;
	}
 	CESL_DataMgr TempDM;
 	Reader.makeDM(&TempDM);
 	CString strTemp = Reader.getCommand();
 	if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
	{
		CString strMsg,strTemp;
		TempDM.GetCellData(0, 0, strTemp);
		SetMessage(TRUE, _T("Ÿ���ݳ��� �����Ͽ����ϴ�"));
		strMsg.Format(_T("���յ���Ÿ���ݳ�(46) ERROR CODE : %s"), strTemp);
		TempDM.QueryLog(strMsg.GetBuffer(0));
		strFuncName.Empty();
		return 200;
	}
	pLoanDM->SetCellData(_T("�ݳ���"), strBuf, nIndex);
	if(!strKey.IsEmpty())
	{
		CString strData;
		const INT ROW_COUNT = pLoanDM->GetRowCount();
		for(INT nRow = ROW_COUNT-1; nRow >= 0; nRow--)
		{
			strData = pLoanDM->GetCellData(_T("����ó��KEY"), nRow);
			if(strData == strKey)
			{
				AddReturnBookList(nRow);
				pLoanDM->DeleteRow(nRow);
			}
		}
	}
	else
	{
		AddReturnBookList(nIndex);
		pLoanDM->DeleteRow(nIndex);
	}

	}//for end
	
	SetTimer(2000, 200, NULL);
	strFuncName.Empty();
	return 0;

EFS_END
return -1;
}

INT CKL_UserSearchDlg::StartEtcService()
{
	INT ids;
	CString strFuncName = _T("StartEtcService");

	CString strMsg;
	strMsg.Format(_T("%s ó�����Դϴ�."), m_strEtcMsg);
	SetMessage(FALSE, strMsg);

	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	ids = Sender.CommMiddleWareDaemon(m_pEtcWriter, &Reader);
	if(0 > ids)
	{
		strMsg.Format(_T("%s ó���� �����Ͽ����ϴ�."), m_strEtcMsg);
		SetMessage(TRUE, strMsg);
		strFuncName.Empty();
		strMsg.Empty();
		return -1;
	}
	Reader.makeDM(m_pEtcDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp || 1 > m_pEtcDM->GetRowCount())
	{
		CString strTemp;
		m_pEtcDM->GetCellData(0, 0, strTemp);
		if(1 > strTemp.Find(_T("DT")))
		{
			strMsg.Format(_T("%s ó���� �����Ͽ����ϴ�."), m_strEtcMsg);
			SetMessage(TRUE, strMsg);
			strFuncName.Empty();
			strMsg.Format(_T("%s(%s) ERROR CODE : %s"), m_strEtcMsg, m_pEtcWriter->getCommand(), strTemp);
			m_pEtcDM->QueryLog(strMsg.GetBuffer(0));
			strMsg.Empty();
			strTemp.Empty();
			return -1;
		}
		strTemp.Empty();
	}

	strFuncName.Empty();
	strMsg.Empty();

	return 0;
}

VOID CKL_UserSearchDlg::SetEtcData(CFrameWriter *Writer, CESL_DataMgr *pDM, CString strEtcMsg)
{
	m_pEtcWriter = Writer;
	m_pEtcDM = pDM;
	m_strEtcMsg = strEtcMsg;
}

INT CKL_UserSearchDlg::AddReturnBookList(INT nIndex)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AddReturnBookList");
	
	CESL_DataMgr *pReturnDM = FindDM(_T("DM_BO_LOC_3100_RETURN_INFO"));
	if(NULL == pReturnDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	CESL_DataMgr *pLoanDM = FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
 	if(NULL == pLoanDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	
	pReturnDM->InsertRow(-1);
	INT nAddCnt = pReturnDM->GetRowCount()-1;

	CString strRegNo;
	ids = pLoanDM->GetCellData(_T("��Ϲ�ȣ"), nIndex, strRegNo);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = pReturnDM->SetCellData(_T("��Ϲ�ȣ"), strRegNo, nAddCnt);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	CString strTitle;
	ids = pLoanDM->GetCellData(_T("��ǥ��"),nIndex, strTitle);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = pReturnDM->SetCellData(_T("��ǥ��"), strTitle, nAddCnt);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	CString strCallNo;
	ids = pLoanDM->GetCellData(_T("û����ȣ"), nIndex, strCallNo);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = pReturnDM->SetCellData(_T("û����ȣ"), strCallNo, nAddCnt);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	CString strLoanDate;
	ids = pLoanDM->GetCellData(_T("������"), nIndex, strLoanDate);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = pReturnDM->SetCellData(_T("������"), strLoanDate, nAddCnt);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	CString strReturnDate;
	ids = pLoanDM->GetCellData(_T("�ݳ���"), nIndex, strReturnDate);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = pReturnDM->SetCellData(_T("�ݳ���"), strReturnDate, nAddCnt);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	CString strReturnPlanDate;
	ids = pLoanDM->GetCellData(_T("�ݳ�������"), nIndex, strReturnPlanDate);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = pReturnDM->SetCellData(_T("�ݳ�������"), strReturnPlanDate, nAddCnt);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	CString strDelayDay;
	ids = pLoanDM->GetCellData(_T("��ü�ϼ�"), nIndex, strDelayDay);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = pReturnDM->SetCellData(_T("��ü�ϼ�"), strDelayDay, nAddCnt);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	ids = pReturnDM->SetCellData(_T("R_WORKER"), m_pInfo->USER_ID, nAddCnt);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	ids = pReturnDM->SetCellData(_T("R_DEVICE"), _T("KOLASIII"), nAddCnt);
	if(0 > ids)ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid2"));

	return 0;

EFS_END
return -1;
}

BOOL CKL_UserSearchDlg::IsLibCodeList()
{
	CString strGetLibCode;
	strGetLibCode = _T("%") + m_strLibCode +_T("%");
	if(-1 < m_pLoan->m_strLibCodeList.Find(strGetLibCode))
	{
		return TRUE;
	}

	return FALSE;	
}

INT CKL_UserSearchDlg::AskPayment(INT &nIndex)
{
	INT ids;
	CString strFuncName = _T("AskPayment");

	CESL_DataMgr *pLoanDM = FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pLoanDM) return -1010;
	CString strPaymentKey;
	ids = pLoanDM->GetCellData(_T("����ó��KEY"), nIndex, strPaymentKey);
	if(0 > ids)
	{
		return ids;
	}
	if(strPaymentKey.IsEmpty())
	{
		BOOL bIsFind = FALSE;
		CString strGetLibCode;
		strGetLibCode = _T("%") + m_strLibCode +_T("%");
		if(-1 < m_pLoan->m_strLibCodeList.Find(strGetLibCode))
		{
			bIsFind = TRUE;
		}


		if(_T("Y") == m_pLoan->m_strLocalFreeYN && TRUE == bIsFind)
		{
		}
		else if(_T("Y") == m_pLoan->m_strOtherFreeYN && FALSE == bIsFind)
		{
		}
		else
		{
			return 100;
		}
	}
	else
	{
		CString strLibCode;
		pLoanDM->GetCellData(_T("�ݳ�������������ȣ"), nIndex, strLibCode);
		strLibCode.TrimLeft(); strLibCode.TrimRight();
		if(strLibCode != m_pInfo->LIB_CODE)
		{
			CString strLibName;
			pLoanDM->GetCellData(_T("�ݳ�������������"), nIndex, strLibName);
			CString strMsg;
			strMsg.Format(_T("�ݳ������������� [%s]�Դϴ�."), strLibName);
			SetMessage(TRUE, strMsg);
			return 400;
		}
	}

	if(!strPaymentKey.IsEmpty())
	{
		CString strData;
		INT nFindCnt = 0;
		if(pLoanDM)
		{
			const INT ROW_COUNT = pLoanDM->GetRowCount();
			for(INT nRow = 0; nRow < ROW_COUNT; nRow++)
			{
				pLoanDM->GetCellData(_T("����ó��KEY"), nRow, strData);
				if(strData == strPaymentKey)
				{				
					nFindCnt++;
				}
			}
		}

		UINT uID = IDOK;
		if(1 < nFindCnt++)
		{
			CKL_PaymentListDlg Dlg(this);
			Dlg.m_strPaymentKey = strPaymentKey;
			Dlg.m_nSelectRow = nIndex;

			uID = Dlg.DoModal();
		}
		if(IDOK != uID)
		{
			return 300;
		}

	}

	return 0;
}

BOOL CKL_UserSearchDlg::IsSearchLoanInfo(INT &nIndex)
{
	INT ids;
	CESL_DataMgr* pMemberDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pMemberDM == NULL) return FALSE;
	if(0 < pMemberDM->GetRowCount())
	{
		CESL_DataMgr *pLoanDM = FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
 		if(NULL == pLoanDM) return FALSE;
 		for(INT i = 0; i < pLoanDM->GetRowCount(); i++)
 		{
 			CString strGetData;
 			ids = pLoanDM->GetCellData(_T("���⵵������ȣ"), i, strGetData);
 			if(0 > ids) return FALSE;
 			if(strGetData == m_strLibCode)
 			{
 				ids = pLoanDM->GetCellData(_T("��Ϲ�ȣ"), i, strGetData);
 				if(0 > ids) return FALSE;
 				if(strGetData == m_strRegNo)
 				{
					nIndex = i;
 					return TRUE;
 				}
 			}
 		}
	}
	return FALSE;
}

void CKL_UserSearchDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		AfxBeginThread(UserSearchThread, this);
	}
	else if(2 == nIDEvent)
	{
		KillTimer(2);
 		OnOK();
	}
	else if(3 == nIDEvent)
	{
		KillTimer(3);
 		OnCancel();
	}
	else if(100 == nIDEvent)
	{
		KillTimer(100);
		AfxBeginThread(UserLoanInfoThread, this);
	}
	else if(110 == nIDEvent)
	{
		KillTimer(110);
		AfxBeginThread(MaxLoanCountThread, this);
	}
	else if(200 == nIDEvent)
	{
		KillTimer(200);
		AfxBeginThread(OtherReturnThread, this);
	}
	else if(120 == nIDEvent)
	{
		KillTimer(120);
		AfxBeginThread(IsLoanStopDateThread, this);
	}
	else if(130 == nIDEvent)
	{
		KillTimer(130);
		AfxBeginThread(IsPaymentFreeThread, this);
	}
	else if(140 == nIDEvent)
	{
		KillTimer(140);
		AfxBeginThread(GetManageThread, this);
	}
	else if(1000 == nIDEvent)
	{
		KillTimer(1000);
		AfxBeginThread(EtcThread, this);
	}
	else if(2000 == nIDEvent)
	{
		KillTimer(2000);
		CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid"));
		if(pGrid)
		{
			pGrid->Display();
		}
	}
	else if(_MODE_LIB_CON_MANAGE_TIMER == nIDEvent) { 
		KillTimer(_MODE_LIB_CON_MANAGE_TIMER);
		AfxBeginThread(GetLibConMngThread, this);
	}	

	CDialog::OnTimer(nIDEvent);
}

UINT CKL_UserSearchDlg::IsLoanStopDateThread(LPVOID pParam)
{
	CKL_UserSearchDlg* pDlg = (CKL_UserSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	pDlg->m_bUserSearchComplate = FALSE;
	INT ids = pDlg->IsLoanStopDate();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		return 0;
	}
	pDlg->m_bUserSearchComplate = TRUE;
	pDlg->SetTimer(2, 200, NULL);

	return 0;
}

UINT CKL_UserSearchDlg::IsPaymentFreeThread(LPVOID pParam)
{
	CKL_UserSearchDlg* pDlg = (CKL_UserSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	pDlg->m_bUserSearchComplate = FALSE;
	INT ids = pDlg->IsPaymentFree();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		return 0;
	}
	pDlg->m_bUserSearchComplate = TRUE;
	pDlg->SetTimer(2, 200, NULL);

	return 0;
}

UINT CKL_UserSearchDlg::GetManageThread(LPVOID pParam)
{
	CKL_UserSearchDlg* pDlg = (CKL_UserSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->GetManage();
	pDlg->m_bIsRunningThread = FALSE;
	pDlg->SetTimer(2, 200, NULL);

	return 0;
}

UINT CKL_UserSearchDlg::GetLibConMngThread(LPVOID pParam)
{
	CKL_UserSearchDlg* pDlg = (CKL_UserSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;
	
	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->GetLibConMng();
	pDlg->m_bIsRunningThread = FALSE;
	pDlg->SetTimer(2, 200, NULL);
	
	return 0;
}

UINT CKL_UserSearchDlg::EtcThread(LPVOID pParam)
{
	CKL_UserSearchDlg* pDlg = (CKL_UserSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	pDlg->m_bUserSearchComplate = FALSE;
	INT ids = pDlg->StartEtcService();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		return 0;
	}
	pDlg->m_bUserSearchComplate = TRUE;
	pDlg->SetTimer(2, 200, NULL);

	return 0;
}

UINT CKL_UserSearchDlg::UserSearchThread(LPVOID pParam)
{
	CKL_UserSearchDlg* pDlg = (CKL_UserSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	pDlg->m_bUserSearchComplate = FALSE;
	INT ids = pDlg->StartUserSearch();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		return 0;
	}
	else if(100 == ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		pDlg->SetTimer(2, 1000, NULL);
	}
	else if(200 == ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		return 0;
	}
	else if(300 == ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		pDlg->SetTimer(3, 2000, NULL);
		return 0;
	}
	pDlg->m_bUserSearchComplate = TRUE;
	pDlg->SetTimer(2, 200, NULL);

	return 0;
}

UINT CKL_UserSearchDlg::UserLoanInfoThread(LPVOID pParam)
{
	CKL_UserSearchDlg* pDlg = (CKL_UserSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	pDlg->m_bUserSearchComplate = FALSE;
	INT ids = pDlg->StartUserLoanInfo();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		return 0;
	}
	else if(200 == ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		pDlg->SetTimer(2, 1000, NULL);
		return 0;
	}
	pDlg->m_bUserSearchComplate = TRUE;
	pDlg->SetTimer(2, 200, NULL);

	return 0;
}

UINT CKL_UserSearchDlg::OtherReturnThread(LPVOID pParam)
{
	CKL_UserSearchDlg* pDlg = (CKL_UserSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	pDlg->m_bUserSearchComplate = FALSE;
	INT ids = pDlg->StartOtherReturn();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		return 0;
	}
	else if(100 == ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		pDlg->SetTimer(2, 1000, NULL);
		return 0;
	}
	else if(200 == ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		return 0;
	}
	else if(300 == ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		pDlg->SetTimer(3, 1000, NULL);
		return 0;
	}
	pDlg->m_bUserSearchComplate = TRUE;
	pDlg->SetTimer(2, 200, NULL);

	return 0;
}

UINT CKL_UserSearchDlg::MaxLoanCountThread(LPVOID pParam)
{
	CKL_UserSearchDlg* pDlg = (CKL_UserSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;
	pDlg->m_bUserSearchComplate = FALSE;
	INT ids = pDlg->StartMaxLoanCount();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		pDlg->m_bUserSearchComplate = FALSE;
		return 0;
	}
	pDlg->m_bUserSearchComplate = TRUE;
	pDlg->SetTimer(2, 200, NULL);

	return 0;
}

BOOL CKL_UserSearchDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	if(!m_MemDC.GetSafeHdc())
	{
		return FALSE;
	}

	m_MemDC.BitBlt(0, 0, 400, 250, &m_BGDC, 0, 0, SRCCOPY);
	m_MemDC.DrawText(m_strMsg, CRect(22, 152, 382, 170), DT_SINGLELINE|DT_VCENTER|DT_CENTER);
  	if(m_bShowBtn)
	{
		m_MemDC.BitBlt(m_rBtn[0].left, m_rBtn[0].top, m_rBtn[0].Width(), m_rBtn[0].Height(), &m_BGDC, m_nBtnMode[0] * 85, 250, SRCCOPY);
		m_MemDC.BitBlt(m_rBtn[1].left, m_rBtn[1].top, m_rBtn[1].Width(), m_rBtn[1].Height(), &m_BGDC, m_nBtnMode[1] * 85, 290, SRCCOPY);
	}

	pDC->BitBlt(0, 0, 400, 250, &m_MemDC, 0, 0, SRCCOPY);
	
	return TRUE;
}

BOOL CKL_UserSearchDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	InvalidateRect(CRect(220, 195, 390, 235));

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CKL_UserSearchDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bFind = FALSE;
	for(INT nCnt = 0; nCnt < 2; nCnt++)
	{
		if(TRUE == m_rBtn[nCnt].PtInRect(point))
		{
			bFind = TRUE;
			m_nBtnMode[nCnt] = 1;
		}
		else
		{
			m_nBtnMode[nCnt] = 0;
		}
	}
	if(bFind)
	{
		return;
	}

	CRect	rDlg;
	GetClientRect(rDlg);
	if( (nFlags & MK_LBUTTON) && FALSE == bFind ) 
	{
		CRect	rCap		=	CRect(0, 0, rDlg.right, 60);
		if(rCap.PtInRect(point))
		{
			HCURSOR cur;
			cur = ::LoadCursor(NULL,IDC_SIZEALL);
			SetCursor(cur);
			this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
			this->UpdateWindow();
			return;
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CKL_UserSearchDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bFind = FALSE;
	for(INT nCnt = 0; nCnt < 2; nCnt++)
	{
		if(TRUE == m_rBtn[nCnt].PtInRect(point))
		{
			bFind = TRUE;
			m_nBtnMode[nCnt] = 2;
		}
		else
		{
			m_nBtnMode[nCnt] = 0;
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CKL_UserSearchDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bFind = FALSE;
	for(INT nCnt = 0; nCnt < 2; nCnt++)
	{
		if(TRUE == m_rBtn[nCnt].PtInRect(point))
		{
			if(0 == nCnt)
			{
				OnbtnKLUserSearchAgain();
			}
			else
			{
				OnbtnKLUserSearchClose();
			}
			m_nBtnMode[nCnt] = 0;
		}
	}
	CDialog::OnLButtonUp(nFlags, point);
}

LRESULT CKL_UserSearchDlg::OnMessageBox(WPARAM wParam, LPARAM lParam)
{
EFS_BEGIN

	CString strResult = *(CString*)lParam;
	UINT uResult = AfxMessageBox(strResult, MB_YESNO);
	if(IDYES != uResult)
	{
		return 300;
	}
		
	return 0;

EFS_END
return -1;
}


int CKL_UserSearchDlg::GetAppendix(CESL_DataMgr* pDM, int nIndex, CStringArray &strArr)
{
	EFS_BEGIN
		
		if(pDM->GetRowCount() <= 1) return -2;
		CString strAppendixFlag = pDM->GetCellData(_T("�ηϿ���"), nIndex);
		if(strAppendixFlag == _T("A") ) return -3;
		CString strSpeciesKey = pDM->GetCellData(_T("��KEY"), nIndex);	
		if(strSpeciesKey.IsEmpty()) return -4;
		CString strLoanLibCode = pDM->GetCellData(_T("���⵵������ȣ"), nIndex);
		if(strLoanLibCode.IsEmpty()) return -4;
		CString strVal = _T("");
		for (int i = 0; i < pDM->GetRowCount(); i++ )
		{
			if( i == nIndex ) continue;
			
			if(		pDM->GetCellData(_T("��KEY"), i ) == strSpeciesKey 
				&&	pDM->GetCellData(_T("���⵵������ȣ"), i ) == strLoanLibCode 
				&&	pDM->GetCellData(_T("�ηϿ���"), i ) == ( strAppendixFlag == _T("B") ? _T("A") : _T("B") )
				)
			{
				strArr.Add( pDM->GetCellData(_T("åKEY"),i) );
			}
		}
		
		return 0;
		
		EFS_END
			return -1;
}

INT CKL_UserSearchDlg::FindIdxFromBookKey(CESL_DataMgr* pDM, CString strBookKey)
{
	
	for (int i = 0 ; i < pDM->GetRowCount() ; i ++)
	{
		if( pDM->GetCellData(_T("åKEY"),i) == strBookKey ){
			return i;
		}
	}
	
	return -1;
}

