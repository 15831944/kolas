// KL_CryptoExecPage.cpp : implementation file
//

#include "stdafx.h"
#include "KL_CryptoManager.h"
#include "KL_CryptoExecPage.h"
#include "KL_CryptoInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��ȣȭ ���� ����
#define CRYPTO_SHA_LENGTH	256
// ��ȣȭ üũ ����
#define CRYPTO_CHK_LENGTH	64
// �ѹ��� ó���ϴ� ��� ���� 
#define CRYPTO_SQL_ROWNUM	100

extern CKL_CryptoInfo* theInfo;

COLORREF clrText[CRYPTO_MSG_MAX] = {
	RGB(0x00, 0x00, 0x00),
	RGB(0x00, 0x00, 0xFF),
	RGB(0xFF, 0x00, 0x00),
	RGB(0x80, 0x80, 0x80)
};

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoExecPage dialog
IMPLEMENT_DYNAMIC(CKL_CryptoExecPage, CPropertyPage)

CKL_CryptoExecPage::CKL_CryptoExecPage()
	: CPropertyPage(CKL_CryptoExecPage::IDD)
{	
	m_bRunThread = FALSE;
	m_pThread = NULL;
}


void CKL_CryptoExecPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_CryptoExecPage)
	DDX_Control(pDX, IDC_LIST_PROGRESS, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_CryptoExecPage, CPropertyPage)
	//{{AFX_MSG_MAP(CKL_CryptoExecPage)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PROGRESS, OnNMCustomdrawList)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoExecPage message handlers

/*=============================================================
	@ Function    :	OnInitDialog
	@ Description :
=============================================================*/
BOOL CKL_CryptoExecPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0, _T(""), 0, 280);

	//{{��ȣȭ Ŭ���� �ʱ� ����
	m_hash.SetHashFormat(UPPERCASE_NOSPACES);
	m_hash.SetHashOperation(STRING_HASH);
	m_hash.SetHashAlgorithm(SHA2);
	m_hash.SetSHA2Strength(CRYPTO_SHA_LENGTH);
	//}}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*=============================================================
	@ Function    :	OnNMCustomdrawList
	@ Description :
=============================================================*/
void CKL_CryptoExecPage::OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	*pResult = CDRF_DODEFAULT;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		{
			CRYPTO_MSG_TYPE enType = (CRYPTO_MSG_TYPE)m_list.GetItemData(pLVCD->nmcd.dwItemSpec);
			
			pLVCD->clrText = clrText[enType];
		}
		break;
	}
}

/*=============================================================
	@ Function    :	OnBtnStart
	@ Description :
=============================================================*/
void CKL_CryptoExecPage::OnBtnStart() 
{
	if (m_bRunThread)
		return; // ������ �߿� �ٽ� ȣ���

	m_bRunThread = TRUE;

	m_pThread = AfxBeginThread(ThreadEntry, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (m_pThread == NULL)
	{
		// �����尡 �������� �ʾҴ�
		m_bRunThread = FALSE;
	}
	else
	{
		m_pThread->m_bAutoDelete = FALSE;
		m_pThread->ResumeThread();
	}
}

/*=============================================================
	@ Function    :	AddMessage
	@ Description :
=============================================================*/
void CKL_CryptoExecPage::AddMessage(CRYPTO_MSG_TYPE enType, CString strMsg)
{
	int nIdx = strMsg.Find(_T("\n"), 0);
	while (nIdx >= 0)
	{
		CString strLineMsg = strMsg.Mid(0, nIdx);
		AddMessage(enType, strLineMsg);

		strMsg = strMsg.Mid(nIdx+1);

		nIdx = strMsg.Find(_T("\n"), 0);
	}

	int nCount = m_list.GetItemCount();
	int nItem  = m_list.InsertItem(nCount, strMsg);

	m_list.ShowScrollBar(SB_HORZ, FALSE);
	m_list.EnsureVisible(nItem, FALSE);
	m_list.SetItemData(nItem, (DWORD)enType);
}

/*=============================================================
	@ Function    :	ThreadEntry
	@ Description :
=============================================================*/
UINT CKL_CryptoExecPage::ThreadEntry(LPVOID pVoid)
{
	return ((CKL_CryptoExecPage*)pVoid)->ThreadProcess();
}

/*=============================================================
	@ Function    :	ThreadProcess
	@ Description :
=============================================================*/
UINT CKL_CryptoExecPage::ThreadProcess()
{
	BuildUserLog();

	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�̿��� �αױ�� �۾��� �Ϸ��߽��ϴ�."));
	m_bRunThread = FALSE;
	return 0;
}

/*=============================================================
	@ Function    :	StringToSHA256
	@ Description :
=============================================================*/
CString CKL_CryptoExecPage::StringToSHA256(CString strInputData)
{
	CHAR szSetHashString[CRYPTO_SHA_LENGTH];
	WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)strInputData, -1, szSetHashString, CRYPTO_SHA_LENGTH, NULL, NULL);
	
	m_hash.SetHashString(szSetHashString);

	WCHAR szDoHashString[CRYPTO_SHA_LENGTH];
	MultiByteToWideChar(CP_ACP, 0, (LPSTR)(LPCTSTR)m_hash.DoHash(), -1, szDoHashString, CRYPTO_SHA_LENGTH);
	
	CString strOutputData = szDoHashString;
	return strOutputData;
}

/*=============================================================
	@ Function    :	EncryptUserPassWD
	@ Description :
=============================================================*/
void CKL_CryptoExecPage::EncryptUserPassWD()
{
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("KOLAS III ����� ��й�ȣ�� ��ȣȭ ���Դϴ�."));

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( theInfo->GetConnectInfo() );

	// ��й�ȣ ��� KOLAS III ����� Ȯ��
	CString strResult;
	int ids = dm.GetOneValueQuery(_T("SELECT COUNT(1) FROM MN_USER_TBL WHERE PASSWD IS NOT NULL"), strResult);
	if (ids < 0)
	{
		// ������ ���̽� �۾��߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}

	CString strMsg;
	strMsg.Format(_T("     KOLAS III ��ü ����� : [%d]��\n")	
				  _T("��й�ȣ�� ����ϴ� ����� : [%s]��"), theInfo->GetUserCount(), strResult);
	AddMessage(CRYPTO_MSG_TEXT, strMsg);

	ids = dm.RestructDataManager(_T("SELECT REC_KEY, PASSWD FROM MN_USER_TBL WHERE PASSWD IS NOT NULL"));
	if (ids < 0)
	{
		// ������ ���̽� �۾��߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}

	// ��� �ʱ�ȭ
	m_result.Clear();

#ifdef CRYPTO_DEV
	if (!EncrptDM(&dm, 0, 1, _T("MN_USER_TBL"), _T("PASSWD_TEST")))
	{
		// ��ȣȭ ó���߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}
#else
	if (!EncrptDM(&dm, 0, 1, _T("MN_USER_TBL"), _T("PASSWD")))
	{
		// ��ȣȭ ó���߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}
#endif
	strMsg.Empty();
	if (m_result.nFail == 0 && m_result.nSkip == 0)
	{
		strMsg.Format(_T("\n")
					  _T("KOLAS III ������� ��й�ȣ��\n")
					  _T("          ���������� ��ȣȭ �Ͽ����ϴ�\n")
				      _T("-------------------------------------------\n"));
		AddMessage(CRYPTO_MSG_GRAY, strMsg);
	}
	else
	{
		AddMessage(CRYPTO_MSG_GRAY, _T(""));
		if (m_result.nSkip)
		{
			strMsg.Format(_T("������ ��ȣȭ�Ǽ� �ǳʶ� ����� : [%d]��"), m_result.nSkip);			
			AddMessage(CRYPTO_MSG_INFO, strMsg);
		}
		if (m_result.nFail)
		{
			strMsg.Format(_T("         ��ȣȭ�� ������ ����� : [%d]��"), m_result.nFail);
			AddMessage(CRYPTO_MSG_WARN, strMsg);
		}		
		AddMessage(CRYPTO_MSG_GRAY, _T("-------------------------------------------\n"));
	}

	// �۾��� ����Ǹ� DB���� FLAG�� �����Ѵ�.
	// UPDATE CO_UTIL_LOCK_TBL SET USE_YN='Y' WHERE UTIL_CODE='USERPWENCRYPTION';
	dm.StartFrame();
#ifdef CRYPTO_DEV
	dm.AddFrame(_T("UPDATE CO_UTIL_LOCK_TBL SET USE_YN_TEST=\'Y\' WHERE UTIL_CODE=\'USERPWENCRYPTION\';"));
#else
	dm.AddFrame(_T("UPDATE CO_UTIL_LOCK_TBL SET USE_YN=\'Y\' WHERE UTIL_CODE=\'USERPWENCRYPTION\';"));
#endif
	ids = dm.SendFrame();
	if (ids < 0)
	{
		// DBó�� ���� �߿� ������ �߻��Ͽ���!!
		ASSERT(0);
	}
	dm.EndFrame();
}

/*=============================================================
	@ Function    :	EncryptLoanUserPassWD
	@ Description :
=============================================================*/
void CKL_CryptoExecPage::EncryptLoanUserPassWD()
{
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�̿��� ��й�ȣ�� ��ȣȭ ���Դϴ�."));

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( theInfo->GetConnectInfo() );

	// ��й�ȣ ��� KOLAS III ����� Ȯ��
	CString strResult;
	int ids = dm.GetOneValueQuery(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE PASSWORD IS NOT NULL"), strResult);
	if (ids < 0)
	{
		// ������ ���̽� �۾��߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}

	CString strMsg;
	strMsg.Format(_T("        ������ ��ü �̿��� : [%d]��\n")	
				  _T("��й�ȣ�� ����ϴ� �̿��� : [%s]��"), theInfo->GetLoanUserCount(), strResult);
	AddMessage(CRYPTO_MSG_TEXT, strMsg);

	ids = dm.RestructDataManager(_T("SELECT REC_KEY, PASSWORD FROM CO_LOAN_USER_TBL WHERE PASSWORD IS NOT NULL"));
	if (ids < 0)
	{
		// ������ ���̽� �۾��߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}

	// ��� �ʱ�ȭ
	m_result.Clear();

#ifdef CRYPTO_DEV
	if (!EncrptDM(&dm, 0, 1, _T("CO_LOAN_USER_TBL"), _T("PASSWORD_TEST")))
	{
		// ��ȣȭ ó���߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}
#else
	if (!EncrptDM(&dm, 0, 1, _T("CO_LOAN_USER_TBL"), _T("PASSWORD")))
	{
		// ��ȣȭ ó���߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}
#endif
	strMsg.Empty();
	if (m_result.nFail == 0 && m_result.nSkip == 0)
	{
		strMsg.Format(_T("\n")
					  _T("   ������ �̿����� ��й�ȣ��\n")
					  _T("          ���������� ��ȣȭ �Ͽ����ϴ�\n")
					  _T("-------------------------------------------\n"));
		AddMessage(CRYPTO_MSG_GRAY, strMsg);
	}
	else
	{
		AddMessage(CRYPTO_MSG_GRAY, _T(""));
		if (m_result.nSkip)
		{
			strMsg.Format(_T("������ ��ȣȭ�Ǽ� �ǳʶ� �̿��� : [%d]��"), m_result.nSkip);			
			AddMessage(CRYPTO_MSG_INFO, strMsg);
		}
		if (m_result.nFail)
		{
			strMsg.Format(_T("         ��ȣȭ�� ������ �̿��� : [%d]��"), m_result.nFail);
			AddMessage(CRYPTO_MSG_WARN, strMsg);
		}		
		AddMessage(CRYPTO_MSG_GRAY, _T("-------------------------------------------\n"));
	}

	// �۾��� ����Ǹ� DB���� FLAG�� �����Ѵ�.
	// UPDATE CO_UTIL_LOCK_TBL SET USE_YN='Y' WHERE UTIL_CODE='VISITORPWENCRYPTION';
	dm.StartFrame();
#ifdef CRYPTO_DEV
	dm.AddFrame(_T("UPDATE CO_UTIL_LOCK_TBL SET USE_YN_TEST=\'Y\' WHERE UTIL_CODE=\'VISITORPWENCRYPTION\';"));
#else
	dm.AddFrame(_T("UPDATE CO_UTIL_LOCK_TBL SET USE_YN=\'Y\' WHERE UTIL_CODE=\'VISITORPWENCRYPTION\';"));
#endif
	ids = dm.SendFrame();
	if (ids < 0)
	{
		// DBó�� ���� �߿� ������ �߻��Ͽ���!!
		ASSERT(0);
	}
	dm.EndFrame();
}

/*=============================================================
	@ Function    :	EncryptLoanUserCardPassWD
	@ Description :
=============================================================*/
void CKL_CryptoExecPage::EncryptLoanUserCardPassWD()
{
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�̿��� ȸ���� ��й�ȣ�� ��ȣȭ ���Դϴ�."));

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( theInfo->GetConnectInfo() );

	// ��й�ȣ ��� KOLAS III ����� Ȯ��
	CString strResult;
	int ids = dm.GetOneValueQuery(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE CARD_PASSWORD IS NOT NULL"), strResult);
	if (ids < 0)
	{
		// ������ ���̽� �۾��߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}

	CString strMsg;
	strMsg.Format(_T("               ������ ��ü �̿��� : [%d]��\n")	
				  _T("ȸ���� ��й�ȣ�� ����ϴ� �̿��� : [%s]��"), theInfo->GetLoanUserCount(), strResult);
	AddMessage(CRYPTO_MSG_TEXT, strMsg);

	ids = dm.RestructDataManager(_T("SELECT REC_KEY, CARD_PASSWORD FROM CO_LOAN_USER_TBL WHERE CARD_PASSWORD IS NOT NULL"));
	if (ids < 0)
	{
		// ������ ���̽� �۾��߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}

	// ��� �ʱ�ȭ
	m_result.Clear();

#ifdef CRYPTO_DEV
	if (!EncrptDM(&dm, 0, 1, _T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD_TEST")))
	{
		// ��ȣȭ ó���߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}
#else
	if (!EncrptDM(&dm, 0, 1, _T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD")))
	{
		// ��ȣȭ ó���߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}
#endif
	strMsg.Empty();
	if (m_result.nFail == 0 && m_result.nSkip == 0)
	{
		strMsg.Format(_T("\n")
					  _T("   ������ �̿����� ȸ���� ��й�ȣ��\n")
					  _T("          ���������� ��ȣȭ �Ͽ����ϴ�\n")
					  _T("-------------------------------------------\n"));
		AddMessage(CRYPTO_MSG_GRAY, strMsg);
	}
	else
	{
		AddMessage(CRYPTO_MSG_GRAY, _T(""));
		if (m_result.nSkip)
		{
			strMsg.Format(_T("������ ��ȣȭ�Ǽ� �ǳʶ� �̿��� : [%d]��"), m_result.nSkip);			
			AddMessage(CRYPTO_MSG_INFO, strMsg);
		}
		if (m_result.nFail)
		{
			strMsg.Format(_T("         ��ȣȭ�� ������ �̿��� : [%d]��"), m_result.nFail);
			AddMessage(CRYPTO_MSG_WARN, strMsg);
		}		
		AddMessage(CRYPTO_MSG_GRAY, _T("-------------------------------------------\n"));
	}

	// �۾��� ����Ǹ� DB���� FLAG�� �����Ѵ�.
	// UPDATE CO_UTIL_LOCK_TBL SET USE_YN='Y' WHERE UTIL_CODE='CARDPWCRYPTION';
	dm.StartFrame();
#ifdef CRYPTO_DEV
	dm.AddFrame(_T("UPDATE CO_UTIL_LOCK_TBL SET USE_YN_TEST=\'Y\' WHERE UTIL_CODE=\'CARDPWCRYPTION\';"));
#else
	dm.AddFrame(_T("UPDATE CO_UTIL_LOCK_TBL SET USE_YN=\'Y\' WHERE UTIL_CODE=\'CARDPWCRYPTION\';"));
#endif
	ids = dm.SendFrame();
	if (ids < 0)
	{
		// DBó�� ���� �߿� ������ �߻��Ͽ���!!
		ASSERT(0);
	}
	dm.EndFrame();
}

/*=============================================================
	@ Function    :	EncrptDM
	@ Description :
=============================================================*/
BOOL CKL_CryptoExecPage::EncrptDM(CESL_DataMgr* pDM, UINT nRecKeyColumn, UINT nInputDataColumn, CString strTableName, CString strOutputColumn)
{
	if (pDM == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	BOOL bShowWarnMsg = FALSE;
	BOOL bFirstWork   = TRUE;
	
	int nCount = pDM->GetRowCount();
	for (int i=0; i<nCount; i++)
	{
		CString strInputData  = pDM->GetCellData(i, nInputDataColumn);

		int nLength = strInputData.GetLength();
		if (nLength == 0)
		{
			// ��� �ִٸ� ��ȣȭ�� �� �� ����.
			// �� ����� DM�� ������ ������ ��������.
			ASSERT(0); 
			continue; 
		}
		if (nLength == CRYPTO_CHK_LENGTH)
		{
			// �̹� ��ȣȭ�� ������ ������
			if (!bShowWarnMsg)
			{
				bShowWarnMsg = TRUE;

				AddMessage(CRYPTO_MSG_INFO, _T("\n")
											_T("�̹� ��ȣȭ�� ����� �����͸� �ǳʶپ����ϴ�"));
			}
			// ��ŵ ��� �ݿ�
			m_result.nSkip++;
			continue;
		}

		if (bFirstWork)
		{
			// �ܿ� �۾� �ð� ���� 
		}

		CString strOutputData = StringToSHA256(strInputData);
		if (strOutputData.IsEmpty())
		{
			AddMessage(CRYPTO_MSG_WARN, _T("\n")
									    _T("��ȣȭ �� �� ���� ��й�ȣ�� �߰ߵǾ����ϴ�"));

			// ���� ��� �ݿ�
			m_result.nFail++;
			continue;
		}

		CString strQurey;
		strQurey.Format(_T("UPDATE %s SET %s=\'%s\' WHERE REC_KEY=%s;"), strTableName, strOutputColumn, strOutputData, pDM->GetCellData(i, nRecKeyColumn));
		TRACE(_T("EncrptDM : %s\n"), strQurey);

		pDM->StartFrame();
		pDM->AddFrame(strQurey);
		int ids = pDM->SendFrame();
		if (ids < 0)
		{
			// DBó�� ���� �߿� ������ �߻��Ͽ���!!
			ASSERT(0);
			pDM->EndFrame();
			continue;
		}
		pDM->EndFrame();

		if (bFirstWork)
		{
			// �ܿ� �۾� �ð� ���� 
			bFirstWork = FALSE;
		}
	}

	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T(""));
	return TRUE;
}

/*=============================================================
	@ Function    :	BuildUserLog
	@ Description :
=============================================================*/
void CKL_CryptoExecPage::BuildUserLog()
{
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�̿��� �αױ�� �۾��� �Դϴ�."));

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( theInfo->GetConnectInfo() );

	CString strQuery;
	strQuery.Format(_T("SELECT COUNT(1) ")
					  _T("FROM %s.CO_LOAN_USER_TBL CLU ")
					 _T("WHERE CLU.USER_NO IS NOT NULL AND ")
					       _T("CLU.REG_DATE IS NOT NULL AND ")
						   _T("NOT EXISTS (SELECT CE.USER_KEY FROM %s.CO_LOAN_USER_EDIT_TBL CE WHERE CE.USER_KEY=CLU.REC_KEY) "), theInfo->GetKolasUserInfo(), theInfo->GetKolasUserInfo());

	CString strResult;
	int ids = dm.GetOneValueQuery(strQuery, strResult);
	if (ids < 0)
	{
		// ������ ���̽� �۾��߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return;
	}

	CString strMsg;
	strMsg.Format(_T("�αױ�� ��� �̿��� : [%s]��"), strResult);
	AddMessage(CRYPTO_MSG_TEXT, strMsg);
	
	strQuery.Format(_T("SELECT CLU.USER_NO, ")												// �����ڹ�ȣ		0
						   _T("CLU.NAME, ")													// �̿���			1
						   _T("TO_CHAR(CLU.REG_DATE,\'YYYY/MM/DD HH24:MI:SS\'), ")			// �����			2
						   _T("TO_CHAR(CLU.LAST_MODIFY_DATE,\'YYYY/MM/DD HH24:MI:SS\'), ")	// ������ ������	3
						   _T("CLU.FIRST_WORK, ")											// �����۾�			4
						   _T("CLU.LAST_WORK, ")											// �������۾�		5
						   _T("CLU.REC_KEY ")												// �̿���KEY		6
					  _T("FROM %s.CO_LOAN_USER_TBL CLU ")
					 _T("WHERE CLU.USER_NO IS NOT NULL AND ")
					       _T("CLU.REG_DATE IS NOT NULL AND ")
						   _T("NOT EXISTS (SELECT CE.USER_KEY FROM %s.CO_LOAN_USER_EDIT_TBL CE WHERE CE.USER_KEY=CLU.REC_KEY) AND ")
						   _T("ROWNUM < 1001 ")
				  _T("ORDER BY CLU.REG_DATE"), theInfo->GetKolasUserInfo(), theInfo->GetKolasUserInfo());

	while (TRUE)
	{
//		TRACE(_T("\n%s\n"), strQuery);
		ids = dm.RestructDataManager(strQuery);
		if (ids < 0)
		{
			// ������ ���̽� �۾��߿� ������ �߻��Ͽ���.
			ASSERT(0);
			return;
		}

		INT nCount = dm.GetRowCount();
		if (nCount == 0)
			break; // �۾��� ����� ����

		if (!BuildUserLog(&dm))
		{
			// ó���߿� ������ �߻��Ͽ���.
			ASSERT(0);
			return;
		}

		strMsg.Format(_T("\n")
					  _T("   ������ �̿��� %d���� �α׸� ����߽��ϴ�"), nCount);
		AddMessage(CRYPTO_MSG_GRAY, strMsg);
	}

	strMsg.Format(_T("\n")
				  _T("   ������ �̿����� �α׸�\n")
				  _T("          ���������� ����߽��ϴ�\n")
				  _T("-------------------------------------------\n"));
	AddMessage(CRYPTO_MSG_TEXT, strMsg);
}

/*=============================================================
	@ Function    :	BuildUserLog
	@ Description :
=============================================================*/
BOOL CKL_CryptoExecPage::BuildUserLog(CESL_DataMgr* pSrcDM)
{
	if (pSrcDM == NULL)
	{
		// TODO : DM�� ã�� �� ����
		return FALSE;
	}

	int nCount = pSrcDM->GetRowCount();
	if (nCount <= 0)
	{
		// TODO : �α� ������ ��ϵ��� ���� �̿��ڰ� �����ϴ�.
		return FALSE;
	}

	// ���� �޽��� ����� ���ؼ� ���� Ȯ�ο�.
	int nFrameCount = 0;

	// ������ �̿��� �αױ��
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( theInfo->GetConnectInfo() );
	dm.StartFrame();

	for (int i=0; i<nCount; i++)
	{
		CString strUserKey;
		CString strWorkDate;

		// 1. ��� �̿��� PK
		pSrcDM->GetCellData(i, 6, strUserKey);
		
		// 2. �̿����� �Է� �α׸� ����ϱ� ���� �۾����� ���Ѵ�
		{
			// �̿����� ����Ϸ� �۾����� �Է��Ѵ�
			CString strRegDate;
			pSrcDM->GetCellData(i, 2, strRegDate);
			if (!strRegDate.IsEmpty())
			{
				// ������� TO_CHAR(CLU.REG_DATE,'YYYY/MM/DD HH24:MI:SS') �������� �����Դ�.
				strWorkDate.Format(_T("TO_DATE(\'%s\', \'YYYY/MM/DD HH24:MI:SS\')"), strRegDate);
			}
			else
			{			
				// ������� ���ٸ� FIRST_WORK�� �м��Ѵ�.
				CString strFirstWork;
				pSrcDM->GetCellData(i, 4, strFirstWork);

				WORK_LOGMSG logMsg;
				if ( AnalyzeWorkLogMsg(strFirstWork, logMsg) )
				{
					int nDateLength = logMsg.logDate.GetLength();
					switch (nDateLength)
					{
					case 6:
						strWorkDate.Format(_T("TO_DATE(\'%s %s\', \'YY/MM/DD HH24:MI:SS\')"), logMsg.logDate, logMsg.logTime);
						break;
					case 8:
						strWorkDate.Format(_T("TO_DATE(\'%s %s\', \'YYYY/MM/DD HH24:MI:SS\')"), logMsg.logDate, logMsg.logTime);
						break;
					// �� �� ó���� AnalyzeWorkLogMsg()���� FALSE�� �ȴ�.
					}
				}
			}

			// �̿����� �Է� �α׸� ������ �� ���ٸ� ���� �α׵� ������� �ʴ´�.
			if (strWorkDate.IsEmpty())
			{
				// TODO : ��� �̿����� �Է� �α׸� ������ �� ����.
				continue;
			}
		}

		// 3. �̿����� �Է� �α׸� �����Ѵ�
		{
			CString strQuery;
			MakeLogQuery(strUserKey, strWorkDate, _T("C"), strQuery);
			dm.AddFrame(strQuery);
			nFrameCount++;
		}

		// 4. �̿����� �������� �ִٸ� ���� �α׸� �����Ѵ�
		{
			strWorkDate.Empty();

			CString strLastModify;
			pSrcDM->GetCellData(i, 3, strLastModify);
			if (!strLastModify.IsEmpty())
			{
				// �������� TO_CHAR(CLU.REG_DATE,'YYYY/MM/DD HH24:MI:SS') �������� �����Դ�.
				strWorkDate.Format(_T("TO_DATE(\'%s\', \'YYYY/MM/DD HH24:MI:SS\')"), strLastModify);
			}
			else
			{
				// �������� ���ٸ� LAST_WORK�� �м��Ѵ�
				CString strLastWork;
				pSrcDM->GetCellData(i, 5, strLastWork);

				WORK_LOGMSG logMsg;
				if ( AnalyzeWorkLogMsg(strLastWork, logMsg) )
				{
					int nDateLength = logMsg.logDate.GetLength();
					switch (nDateLength)
					{
					case 6:
						strWorkDate.Format(_T("TO_DATE(\'%s %s\', \'YY/MM/DD HH24:MI:SS\')"), logMsg.logDate, logMsg.logTime);
						break;
					case 8:
						strWorkDate.Format(_T("TO_DATE(\'%s %s\', \'YYYY/MM/DD HH24:MI:SS\')"), logMsg.logDate, logMsg.logTime);
						break;
					// �� �� ó���� AnalyzeWorkLogMsg()���� FALSE�� �ȴ�.
					}
				}
			}

			// ���� �α׸� ������� �ʴ´�.
			if (strWorkDate.IsEmpty())
			{
				continue;
			}
		}
		
		// 5. �̿����� ���� �α׸� �����Ѵ�.
		{
			CString strQuery;
			MakeLogQuery(strUserKey, strWorkDate, _T("U"), strQuery);
			dm.AddFrame(strQuery);
			nFrameCount++;
		}
	}

	int ids = dm.SendFrame();
	dm.EndFrame();
	if (ids < 0)
	{
		// ������ ���̽� �۾��߿� ������ �߻��Ͽ���.
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

/*=============================================================
	@ Function    :	AnalyzeWorkLogMsg
	@ Description :
=============================================================*/
BOOL CKL_CryptoExecPage::AnalyzeWorkLogMsg(CString strWorkLogMsg, WORK_LOGMSG &logMsg)
{
	// strWorkLogMsg = 20120126 1345 MA@192.168.0.207 >bl_loc_user_proc.cpp#1527;

	int nStartPos = 0;
	int nEndPos   = 0;

	nEndPos = strWorkLogMsg.Find(_T(" "), nStartPos);
	if (nEndPos < 0)
		return FALSE; // KOLASIII ������ �ƴϴ�.
	CString strDate = strWorkLogMsg.Mid(nStartPos, nEndPos-nStartPos);
	nStartPos = nEndPos+1;

	int nDateLength = strDate.GetLength();
	switch (nDateLength)
	{
	case 6: // ��¥ ������ YYMMDD
		break;
	case 8: // ��¥ ������ YYYYMMDD		
		break;
	default:
		return FALSE; // KOLASIII ������ �ƴϴ�
		break;
	}
	
	nEndPos = strWorkLogMsg.Find(_T(" "), nStartPos);
	if (nEndPos < 0)
		return FALSE; // KOLASIII ������ �ƴϴ�.
	CString strTime = strWorkLogMsg.Mid(nStartPos, nEndPos-nStartPos);
	nStartPos = nEndPos+1;	

	nEndPos = strWorkLogMsg.Find(_T(" "), nStartPos);
	if (nEndPos < 0)
		return FALSE; // KOLASIII ������ �ƴϴ�.
	CString strWorkerInfo = strWorkLogMsg.Mid(nStartPos, nEndPos-nStartPos);
	nStartPos = nEndPos+1;

	int nPos = strWorkerInfo.Find(_T("@"), 0);
	if (nPos < 0)
		return FALSE; // KOLASIII ������ �ƴϴ�.
	
	logMsg.logDate     = strDate;
	logMsg.logTime     = strTime;
	logMsg.logWorkerID = strWorkerInfo.Mid(0, nPos);
	logMsg.logWorkerIP = strWorkerInfo.Mid(nPos+1);
	return TRUE;
}

/*=============================================================
	@ Function    :	MakeLogQuery
	@ Description :
=============================================================*/
BOOL CKL_CryptoExecPage::MakeLogQuery(CString strUserKey, CString strWorkDate, CString strType, CString &strQuery)
{
	CESL_Mgr mgr(0);
	strQuery.Format(_T("INSERT INTO %s.CO_LOAN_USER_EDIT_TBL(REC_KEY, TYPE, WORK_DATE, WORKER_IP, WORKER_ID, WORKER_NAME, WORKER_SYS, USER_KEY, FIRST_WORK) ")
		            _T("VALUES ")
					_T("(%s.ESL_SEQ.NEXTVAL, \'%s\', %s, \'%s\', \'%s\', \'%s\', \'KULM\', %s, \'%s\');"), theInfo->GetKolasUserInfo(), theInfo->GetKolasUserInfo(), strType, strWorkDate, theInfo->GetLocalIP(), theInfo->GetUserID(), theInfo->GetUserName(), strUserKey, mgr.GetWorkLogMsg(_T("�αױ��"),__WFILE__,__LINE__));

#ifdef _KULDEV
	TRACE(_T("==========================================================================================\n"));
	TRACE(_T("%s\n"), strQuery);
	TRACE(_T("==========================================================================================\n"));
#endif
	return TRUE;
}

/*=============================================================
	@ Function    :	PreTranslateMessage
	@ Description :
=============================================================*/
BOOL CKL_CryptoExecPage::PreTranslateMessage(MSG* pMsg) 
{
	if (IsRunThread() && pMsg->message == WM_KEYDOWN)
	{
		// ������ �߿��� �Է��� ���� �ʴ´�
		return TRUE;
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}
