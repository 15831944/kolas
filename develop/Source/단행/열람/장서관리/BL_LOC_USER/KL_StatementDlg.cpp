#include "stdafx.h"
#include "KL_StatementDlg.h"
#include "locale.h"
#include "CertifyApi.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"
#include "FrameManager.h"
#include "KL_STATEMENT_IMAGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KL_StatementDlg::KL_StatementDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(KL_StatementDlg::IDD, pParent)
{
	
	m_strStatementAlias = _T("�������� ����/�̿�");
	m_strIsCollectYN = _T("N");
	m_strIsProvideYN = _T("N");
	m_strAgentName = _T("");
	m_strAgentRelation = _T("");
	m_strAgentNote = _T("");
	m_bUnder14YN = FALSE;
	m_bIsAgreeYN = FALSE;
	m_bIsAgentCertYN = FALSE;
	m_bIsAgreeUpdated = FALSE;
	m_bIsAgentUpdated = FALSE;
	m_bIsCertifyMode = FALSE;
	m_bIsKLMode = FALSE; 
	m_bIsKLMemberYN = FALSE;
	m_strUserKey = _T("");
	m_nMode = 0;
	
	m_bBeforeAgreeYN = FALSE;
	m_strAgreeType = _T("0");
	m_bRealNameCertYN = FALSE;
	m_strExpireDate = _T("");
	m_bConfirm = FALSE;
	m_bManageStatus = FALSE;
	m_strGpinHash = _T("");

	m_strTmpImgUserKey = _T("");
}
KL_StatementDlg::~KL_StatementDlg()
{
	m_pParent = NULL;
}

void KL_StatementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(KL_StatementDlg) 
	//}}AFX_DATA_MAP	
	
}


BEGIN_MESSAGE_MAP(KL_StatementDlg, CDialog)
	//{{AFX_MSG_MAP(KL_StatementDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_chkKL_STATEMENT, OnchkKLSTATEMENT)
	ON_BN_CLICKED(IDC_btnCONFIRM, OnbtnCONFIRM)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_dateAGREE, OnDatetimechangedateAGREE)
	ON_EN_SETFOCUS(IDC_edtRELATIONNOTE, OnSetfocusedtRELATIONNOTE)
	ON_EN_KILLFOCUS(IDC_edtRELATIONNOTE, OnKillfocusedtRELATIONNOTE)
	ON_BN_CLICKED(IDC_IMAGE_MNG, OnImageMng)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL KL_StatementDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnCancel();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDOK)->m_hWnd == pMsg->hwnd )
			{
				OnOK();
			}
			else if(GetDlgItem(IDCANCEL)->m_hWnd == pMsg->hwnd )
			{
				OnCancel();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL KL_StatementDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CEdit*)GetDlgItem(IDC_edtAGENTNAME))->SetLimitText(20);
	((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->SetLimitText(200);

	EnableThemeDialogTexture(GetSafeHwnd());


	((CEdit*)GetDlgItem(IDC_edtAGENTNAME))->SetLimitText(20);
	((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->SetLimitText(200);

	if(0 > InitESL_Mgr("SM_���Ǳ��"))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_AGREEMENT"));

	CString strQuery = _T(""), strData;
	
	strQuery.Format(_T("SELECT TERMS_TITLE, AMEND_DATE, IMPL_DATE, ESSENTIAL, FIRST_WORK, LAST_WORK, REC_KEY, TERMS_LENGTH FROM MN_AGREEMENT_TBL WHERE REC_KEY = (SELECT MAX(REC_KEY) KEEP(DENSE_RANK FIRST ORDER BY IMPL_DATE DESC) FROM MN_AGREEMENT_TBL WHERE TERMS_TITLE = '%s') "), m_strStatementAlias);
	
	pDM->RestructDataManager(strQuery);

	CString strStatement, strEssential; 

	
	pDM->GetCellData(_T("ESSENTIAL"), 0, strEssential);

	if(strEssential.Compare(_T("Y")) != 0)
	{
		GetDlgItem(IDC_stcKL_STATEMENT_ES)->ShowWindow(SW_HIDE);
	}

	strData = pDM->GetCellData(_T("REC_KEY"), 0);
	
	CString strLength = pDM->GetCellData(_T("TERMS_LENGTH"), 0);

	INT nLength = _ttoi(strLength);

	if(nLength > 0)
	{
		BYTE* pBuffer = new BYTE[nLength ];
		memset(pBuffer, 0x00, nLength);

		INT ids = pDM->LOBGet(_T("MN_AGREEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strData, _T("TERMS_MEMO"), pBuffer, &nLength);
		
		((CEdit*)GetDlgItem(IDC_edtKL_STATEMENT_1))->SetWindowText((LPCTSTR)pBuffer);

		delete [] pBuffer;
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_edtKL_STATEMENT_1))->SetWindowText(_T(""));
	}
	
	

	ShowScreen();

	return TRUE;  
}

void KL_StatementDlg::OnOK() 
{
	if(TRUE == ((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->GetCheck())
	{
		m_bIsAgreeYN = TRUE;
	}
	
	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_AGREEMENT"));
	CString strQuery = _T(""), strStatement = _T("") ;
	strQuery.Format(_T("SELECT ESSENTIAL FROM MN_AGREEMENT_TBL WHERE REC_KEY = (SELECT MAX(REC_KEY) KEEP(DENSE_RANK FIRST ORDER BY IMPL_DATE DESC) FROM MN_AGREEMENT_TBL WHERE TERMS_TITLE = '%s') "), m_strStatementAlias);	
	pDM->GetOneValueQuery(strQuery, strStatement);

	
	if( (0 == m_nMode || 1 == m_nMode) && (strStatement.Compare(_T("Y")) == 0))
	{
		CString strAgentName = _T("");
		CString strAgentNote = _T("");
		((CEdit*)GetDlgItem(IDC_edtAGENTNAME))->GetWindowText(strAgentName);
		((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->GetWindowText(strAgentNote);
		if(0 == m_nMode)
		{
			SetData();
		}
		if(FALSE == ((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->GetCheck())
		{
			if(FALSE == m_bIsCertifyMode)
			{
				MessageBox(_T("���� Ȯ���� �ʿ��մϴ�."), _T("����Ȯ��"), MB_OK|MB_ICONINFORMATION);
				return;
			}
			else
			{
				if(IDYES != MessageBox(_T("���� Ȯ���� �ʿ��ϸ� ���ǰ� Ȯ�ε��� ������ �Է��Ͻ� ������ ������� �ʽ��ϴ�.\r\n���� Ȯ�� ���� �����Ͻðڽ��ϱ�?\r\n - YES : ����\r\n - NO : ����"), _T("����Ȯ��"), MB_YESNO|MB_ICONINFORMATION))
				{
					return;
				}
				else
				{
					m_bIsAgreeYN = FALSE;
					m_strAgentName = _T("");
					m_strAgentRelation = _T("");
					m_strAgentNote = _T("");
				}
			}		
		}
		else if(m_bUnder14YN && _T("") == strAgentName)
		{
			MessageBox(_T("��14�� �̸� �̿����Դϴ�.\r\n ��14�� �̸� �̿��ڴ� �����븮���� ������ �ʿ��մϴ�."), _T("�����븮�� Ȯ��"), MB_OK|MB_ICONINFORMATION);
			return;
		}
		else if((_T("") != strAgentNote &&  _T("EX) �θ�� ���� / �����븮�� ����Ȯ��") != strAgentNote) && _T("") == strAgentName)
		{
			MessageBox(_T("�����븮���� �̸��� �Է��ϼ���."), _T("�����븮�� Ȯ��"), MB_OK|MB_ICONINFORMATION);
			return;
		}
	}


	if(m_pInfo->m_bAgreeInfoRecordYN || (m_pInfo->m_bKLUse && m_bIsKLMemberYN))
	{
		INT ids = SaveScreen();
		if(400 == ids)
		{
			AfxMessageBox(_T("ȸ�������� ���ŵǾ����ϴ�."));
		}
		else if(0 < ids && 1 == m_nMode)
		{
			AfxMessageBox(_T("��������� ����Ǿ����ϴ�."));
		}
		else if(0 > ids)
		{
			AfxMessageBox(_T("�������� ���忡 �����Ͽ����ϴ�."));
		}
	}
	CDialog::OnOK();
}

void KL_StatementDlg::OnCancel() 
{
	CDialog::OnCancel();
}

HBRUSH KL_StatementDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcKL_STATEMENT == pWnd->GetDlgCtrlID() || IDC_stcKL_STATEMENT_ES == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(242,59,12));
        pDC->SetBkMode(TRANSPARENT);
    }

	CString strAgreeStatus, strRealNameStatus, strAgentStatus, strAgentNote;
	((CStatic*)GetDlgItem(IDC_staticAGREESTATUS))->GetWindowText(strAgreeStatus);
	((CStatic*)GetDlgItem(IDC_staticREALNAMESTATUS))->GetWindowText(strRealNameStatus);
	((CEdit*)GetDlgItem(IDC_staticAGENTSTATUS))->GetWindowText(strAgentStatus);
	((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->GetWindowText(strAgentNote);

	if(IDC_staticAGREESTATUS == pWnd->GetDlgCtrlID())
    {
		if(_T("�̵���") == strAgreeStatus || _T("�絿���ʿ�") == strAgreeStatus)
		{
			pDC->SetTextColor(RGB(242,59,12));
			pDC->SetBkMode(TRANSPARENT);
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,255));
			pDC->SetBkMode(TRANSPARENT);
		}
    }

	if(IDC_staticREALNAMESTATUS == pWnd->GetDlgCtrlID())
    {
		if(_T("������") == strRealNameStatus || _T("�������ʿ�") == strRealNameStatus)
		{
			pDC->SetTextColor(RGB(242,59,12));
			pDC->SetBkMode(TRANSPARENT);
		}
		else if(_T("�ش����") == strRealNameStatus)
		{
			pDC->SetTextColor(RGB(200,200,200));
			pDC->SetBkMode(TRANSPARENT);
		}
		else if(_T("�����Ϸ�") == strRealNameStatus)
		{
			pDC->SetTextColor(RGB(0,0,255));
			pDC->SetBkMode(TRANSPARENT);
		}
    }
	if(IDC_staticAGENTSTATUS == pWnd->GetDlgCtrlID())
    {

		if(_T("�����Ϸ�") == strAgentStatus)
		{
			pDC->SetTextColor(RGB(0,0,255));
			pDC->SetBkMode(TRANSPARENT);
		}
		else if(_T("�ش����") == strAgentStatus)
		{
			pDC->SetTextColor(RGB(200,200,200));
			pDC->SetBkMode(TRANSPARENT);
		}
		else if(_T("������") == strAgentStatus || _T("�������ʿ�") == strAgentStatus)
		{
			pDC->SetTextColor(RGB(242,59,12));
			pDC->SetBkMode(TRANSPARENT);
		}
	}

	if(IDC_edtRELATIONNOTE == pWnd->GetDlgCtrlID() && _T("EX) �θ�� ���� / �����븮�� ����Ȯ��") == strAgentNote)
    {
        pDC->SetTextColor(RGB(200,200,200));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

void KL_StatementDlg::ViewErrorMsg(int nMode, CString strCode, CString strMsg) 
{
	
	CString strValue;
	if(0 == nMode)
	{ 
		strValue.Format(_T("�̵������� ��ſ� �����Ͽ����ϴ�.[%s]"), strCode);
		AfxMessageBox(strValue, MB_ICONSTOP);
	}
	else if(1 == nMode)
	{ 
		AfxMessageBox(strMsg, MB_ICONSTOP);
	}
	else if(2 == nMode)
	{ 
		strValue.Format(_T("%d"), m_pInfo->m_nCertifyAgency);
		CString strDesc = _T(""), strAction = _T("");
		m_pInfo->GetCertifyFailCodeToDesc(strValue, strCode, strDesc, strAction);
		AfxMessageBox(strDesc, MB_ICONSTOP);
	}
}

void KL_StatementDlg::OnchkKLSTATEMENT() 
{
	if(((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->GetCheck())
	{
		((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->SetCurSel(3);
		
		if((_T("0") == m_strAgreeType || _T("") == m_strAgreeType) || m_strStatementAlias != m_strStatementAliasBefore)
		{
			((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->EnableWindow(TRUE);
			((CDateTimeCtrl*)GetDlgItem(IDC_dateAGREE))->EnableWindow(TRUE);	
			SetStringDateToDateTimeCtrl(IDC_dateEXPIRE, SetExpireDate());
		}
	}
	else
	{
		((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->SetCurSel(0);
		
		if(_T("0") == m_strAgreeType || _T("") == m_strAgreeType)
		{
			((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->EnableWindow(FALSE);
			((CDateTimeCtrl*)GetDlgItem(IDC_dateAGREE))->EnableWindow(FALSE);	
		}
	}
}


INT KL_StatementDlg::SaveAgreementInfo() 
{
	
	if(1 == m_nMode)
	{
		CString strWork = this->GetWorkLogMsg(_T("������������"),__WFILE__,__LINE__);
		CString strQuery;
		CString strGetDuplication;
		CString strDate;

		INT nType = ((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->GetCurSel();
		m_strAgreeType.Format(_T("%d"), nType);

		if(_T("") != m_strStatementAlias)
		{
			CESL_DataMgr *pDM = FindDM(_T("DM_���Ǳ��"));
			pDM->StartFrame();
			strQuery.Format(_T("SELECT COUNT(1) FROM CO_AGREEMENT_INFO_TBL WHERE USER_KEY = %s"), m_strUserKey);
			pDM->GetOneValueQuery(strQuery, strGetDuplication);				
			if(_T("0") != strGetDuplication)
			{	
				
				if(0 < m_pInfo->m_nUserInfoRenewalDay)
				{
					strQuery.Format(_T("UPDATE CO_AGREEMENT_INFO_TBL SET STATEMENT_ALIAS = '%s', AGREE_YN = 'Y', AGREE_TYPE = '%s', AGREE_DATE = '%s', EXPIRE_DATE = '%s', LIB_CODE = '%s', LIB_NAME = '%s', WORKER = 'KOLASIII', LAST_WORK = '%s' WHERE USER_KEY = %s;"), m_strStatementAlias, m_strAgreeType, m_strAgreeDate,  m_strExpireDate, m_pInfo->LIB_CODE, m_pInfo->LIB_NAME, strWork, m_strUserKey);
				}
				else
				{
					strQuery.Format(_T("UPDATE CO_AGREEMENT_INFO_TBL SET STATEMENT_ALIAS = '%s', AGREE_YN = 'Y', AGREE_TYPE = '%s', AGREE_DATE = '%s', EXPIRE_DATE = NULL, LIB_CODE = '%s', LIB_NAME = '%s', WORKER = 'KOLASIII', LAST_WORK = '%s' WHERE USER_KEY = %s;"), m_strStatementAlias, m_strAgreeType, m_strAgreeDate,  m_pInfo->LIB_CODE, m_pInfo->LIB_NAME, strWork, m_strUserKey);
				}
			}
			else
			{				
				if(0 < m_pInfo->m_nUserInfoRenewalDay)
				{
					strQuery.Format(_T("INSERT INTO CO_AGREEMENT_INFO_TBL(USER_KEY, STATEMENT_ALIAS, AGREE_YN, AGREE_TYPE, AGREE_DATE, EXPIRE_DATE, LIB_CODE, LIB_NAME, WORKER, FIRST_WORK, LAST_WORK) VALUES(%s, '�������� ����/�̿�', 'Y', '%s', '%s', '%s', '%s', '%s', 'KOLASIII', '%s', '%s');"), m_strUserKey, m_strAgreeType, m_strAgreeDate, m_strExpireDate, m_pInfo->LIB_CODE, m_pInfo->LIB_NAME, strWork, strWork);
				}
				else
				{
					strQuery.Format(_T("INSERT INTO CO_AGREEMENT_INFO_TBL(USER_KEY, STATEMENT_ALIAS, AGREE_YN, AGREE_TYPE, AGREE_DATE, LIB_CODE, LIB_NAME, WORKER, FIRST_WORK, LAST_WORK) VALUES(%s, '�������� ����/�̿�', 'Y', '%s', '%s', '%s', '%s', 'KOLASIII', '%s', '%s');"), m_strUserKey, m_strAgreeType, m_strAgreeDate, m_pInfo->LIB_CODE, m_pInfo->LIB_NAME, strWork, strWork);
				}
			}
			pDM->AddFrame(strQuery);
			pDM->SendFrame();
			pDM->EndFrame();
			m_bIsAgreeUpdated = TRUE;
			return 100;
		}
	}

	return 0;
}

INT KL_StatementDlg::SaveAgentInfo() 
{
	
	if(1 == m_nMode && (m_bUnder14YN || _T("") != m_strAgentName))
	{
		CString strWork = this->GetWorkLogMsg(_T("������������"),__WFILE__,__LINE__);
		CString strQuery;
		CString strGetDuplication;
		CString strWhere;
		CString strDate;
		CString strExpireDate;
		strWhere.Format(_T("USER_KEY = %s"), m_strUserKey);
		RefreshDataMgr(_T("DM_�븮������"), strWhere);
		CESL_DataMgr *pDM = FindDM(_T("DM_�븮������"));
		pDM->StartFrame();
		strQuery.Format(_T("SELECT COUNT(1) FROM CO_AGENT_INFO_TBL WHERE USER_KEY = %s"), m_strUserKey);
		pDM->GetOneValueQuery(strQuery, strGetDuplication);
		if(_T("0") != strGetDuplication)
		{	
			strQuery.Format(_T("UPDATE CO_AGENT_INFO_TBL SET NAME = '%s', RELATION = '%s', NOTE = '%s', CERT_DATE = SYSDATE, LIB_CODE = '%s', LIB_NAME = '%s', WORKER = 'KOLASIII', LAST_WORK = '%s' WHERE USER_KEY = %s;"), m_strAgentName, m_strAgentRelation, m_strAgentNote, m_pInfo->LIB_CODE, m_pInfo->LIB_NAME, strWork, m_strUserKey);
		}
		else
		{	
			strQuery.Format(_T("INSERT INTO CO_AGENT_INFO_TBL(USER_KEY, NAME, RELATION, NOTE, CERT_DATE, LIB_CODE, LIB_NAME, WORKER, FIRST_WORK) VALUES(%s, '%s', '%s', '%s', SYSDATE, '%s', '%s', 'KOLASIII', '%s');"), m_strUserKey, m_strAgentName, m_strAgentRelation, m_strAgentNote, m_pInfo->LIB_CODE, m_pInfo->LIB_NAME, strWork);
		}
		pDM->AddFrame(strQuery);
		pDM->SendFrame();
		pDM->EndFrame();
		return 100;
	}
	return 0;
}

INT KL_StatementDlg::RequestEditService(CString strUserNo, CString strGpinHash) 
{
	CString strGetData;
	CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime((CESL_Mgr*)this, strGetData);
	CString strAgreeInfo;
	strAgreeInfo.Format(_T("%s��Y��%s��%s��%s��KOLASIII"),m_strAgreeType, strGetData, m_pInfo->LIB_CODE, m_pInfo->LIB_NAME);

	CString strCommand = _T("IL_K24_LOC_B01_SERVICE");
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
	Writer.addElement(_T("��������"), strGetData);
	Writer.addElement(_T("������������ȣ"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("G-PINHASH�����ڵ�"), strGpinHash);
	Writer.addElement(_T("��������"), strAgreeInfo);
	if(_T("") != m_strAgentName)
	{
		CString strAgentInfo;
		strAgentInfo.Format(_T("%s��%s��%s��%s��%s��%s��KOLASIII"), m_strAgentName, m_strAgentRelation, m_strAgentNote, strGetData, m_pInfo->LIB_CODE, m_pInfo->LIB_NAME);
		Writer.addElement(_T("�븮������"), strAgentInfo);
	}
	
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	CESL_DataMgr TempDM;
	TempDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	CString strViewMsg;
 	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		strViewMsg.Format(_T("%d"), ids);
		ViewErrorMsg(0, strViewMsg, _T(""));
		return ids;
	}
	else
	{
 		Reader.makeDM(&TempDM);
		CString strResult;
 		strResult = Reader.getCommand();
 		if(_T("ERROR") == strResult)
 		{
			CString strMsg;
			TempDM.GetCellData(_T("ID"), 0, strResult);
			TempDM.GetCellData(_T("MSG"), 0, strMsg);
			ViewErrorMsg(1, strResult, strMsg);
			return 1;
 		}
		else
		{
			TempDM.GetCellData(_T("ó�����"), 0, strResult);
			if(_T("OK") != strResult)
			{
				ViewErrorMsg(2, strResult, _T(""));
				return 2;
			}
		}
	}

	return 0;
}

INT KL_StatementDlg::ShowScreen() 
{	
	((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->AddString(_T("�̵���"));
	((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->AddString(_T("���鵿��"));	
	((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->AddString(_T("���ڵ���"));
	((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->AddString(_T("���鵿�� �� ���ڰ���"));
	((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->SetCurSel(3);

	((CComboBox*)GetDlgItem(IDC_comboRELATION))->AddString(_T("ģ����"));
	((CComboBox*)GetDlgItem(IDC_comboRELATION))->AddString(_T("�İ���"));	
	((CComboBox*)GetDlgItem(IDC_comboRELATION))->AddString(_T("��Ÿ"));	
	((CComboBox*)GetDlgItem(IDC_comboRELATION))->SetCurSel(0);
	
	if(0 == m_pInfo->m_nUserInfoRenewalDay)
	{
		((CButton*)GetDlgItem(IDC_btnCONFIRM))->ShowWindow(SW_HIDE);
		((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->ShowWindow(SW_HIDE);	
		((CStatic*)GetDlgItem(IDC_staticEXPIRE))->ShowWindow(SW_HIDE);		
	}

	if(0 == m_nMode)
	{
		((CDateTimeCtrl*)GetDlgItem(IDC_dateAGREE))->EnableWindow(FALSE);
		((CDateTimeCtrl*)GetDlgItem(IDC_dateAGREE))->EnableWindow(FALSE);
		((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->EnableWindow(FALSE);
		SetStringDateToDateTimeCtrl(IDC_dateEXPIRE, SetExpireDate());
		((CButton*)GetDlgItem(IDC_btnCONFIRM))->ShowWindow(SW_HIDE);
		
		if(m_bUnder14YN && _T("") == m_strAgentNote)
		{
			((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->SetWindowText(_T("EX) �θ�� ���� / �����븮�� ����Ȯ��"));		
		}

		SetManageStatus(m_bIsAgreeYN, m_bRealNameCertYN, m_bUnder14YN, m_strAgentName);

		if(!m_strUserKey.IsEmpty())
		{
			CString strWhere;
			strWhere.Format(_T("USER_KEY = %s"), m_strUserKey);
			RefreshDataMgr(_T("DM_���Ǳ��"), strWhere);
			RefreshDataMgr(_T("DM_�븮������"), strWhere);
			CESL_DataMgr *pDM = FindDM(_T("DM_���Ǳ��"));
			CESL_DataMgr *pDMAgent = FindDM(_T("DM_�븮������"));
			
			m_strStatementAliasBefore = pDM->GetCellData(_T("�������"), 0);
			m_strAgreeType = pDM->GetCellData(_T("���ǹ��"), 0);
			m_strAgreeDate = pDM->GetCellData(_T("��������"), 0);
			m_strExpireDate = pDM->GetCellData(_T("��������"), 0);
			m_strAgentName = pDM->GetCellData(_T("�븮�θ�"), 0);
			m_strAgentRelation = pDM->GetCellData(_T("�븮�ΰ���"), 0);
			m_strAgentNote = pDM->GetCellData(_T("�븮�κ��"), 0);

			
			if(_T("") != m_strAgreeDate)
			{
				SetStringDateToDateTimeCtrl(IDC_dateAGREE, m_strAgreeDate);
			}
			if(_T("") != m_strExpireDate)
			{
				SetStringDateToDateTimeCtrl(IDC_dateEXPIRE, m_strExpireDate);
			}
			else
			{

				((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->EnableWindow(FALSE);		
			}
			
			
			if(m_strStatementAliasBefore == m_strStatementAlias)
			{
				((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->SetCheck(1);
				((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->EnableWindow(FALSE);
				((CDateTimeCtrl*)GetDlgItem(IDC_dateAGREE))->EnableWindow(FALSE);
				((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->EnableWindow(FALSE);
			}
			((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->SetCurSel(_ttoi(m_strAgreeType));
			((CEdit*)GetDlgItem(IDC_edtAGENTNAME))->SetWindowText(m_strAgentName);
			((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->SetWindowText(m_strAgentNote);
			if(_T("�İ���") == m_strAgentRelation)
			{
				((CComboBox*)GetDlgItem(IDC_comboRELATION))->SetCurSel(1);
			}
			else if(_T("��Ÿ") == m_strAgentRelation)
			{
				((CComboBox*)GetDlgItem(IDC_comboRELATION))->SetCurSel(2);
			}
			else
			{
				((CComboBox*)GetDlgItem(IDC_comboRELATION))->SetCurSel(0);
			}
		}
		
		((CStatic*)GetDlgItem(IDC_staticAGREE))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staticREALNAME))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staticAGENT))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staticAGREESTATUS))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staticREALNAMESTATUS))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staticAGENTSTATUS))->ShowWindow(SW_HIDE);
		
		((CStatic*)GetDlgItem(IDC_staticStatement))->SetWindowText(_T("ȸ������ �ȳ�"));
		
		CRect rect;
		((CEdit*)GetDlgItem(IDC_edtMANAGEINFO))->GetWindowRect(&rect);
		rect.top = rect.top - 25;
		ScreenToClient(rect);
		((CEdit*)GetDlgItem(IDC_edtMANAGEINFO))->MoveWindow(rect);

	}
	else if(1 == m_nMode)
	{
		
		
		
		if(_T("0") == m_strAgreeType)
		{
			((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->EnableWindow(FALSE);	
			((CDateTimeCtrl*)GetDlgItem(IDC_dateAGREE))->EnableWindow(FALSE);		
			((CButton*)GetDlgItem(IDC_btnCONFIRM))->ShowWindow(SW_HIDE);
			
		}

		if(!m_strUserKey.IsEmpty())
		{
			CString strWhere;
			strWhere.Format(_T("USER_KEY = %s"), m_strUserKey);
			RefreshDataMgr(_T("DM_���Ǳ��"), strWhere);
			RefreshDataMgr(_T("DM_�븮������"), strWhere);
			CESL_DataMgr *pDM = FindDM(_T("DM_���Ǳ��"));
			CESL_DataMgr *pDMAgent = FindDM(_T("DM_�븮������"));
			
			m_strStatementAliasBefore = pDM->GetCellData(_T("�������"), 0);
			m_strAgreeType = pDM->GetCellData(_T("���ǹ��"), 0);
			m_strAgreeDate = pDM->GetCellData(_T("��������"), 0);
			m_strExpireDate = pDM->GetCellData(_T("��������"), 0);
			m_strAgentName = pDM->GetCellData(_T("�븮�θ�"), 0);
			m_strAgentRelation = pDM->GetCellData(_T("�븮�ΰ���"), 0);
			m_strAgentNote = pDM->GetCellData(_T("�븮�κ��"), 0);

			
			if(_T("") != m_strAgreeDate)
			{
				SetStringDateToDateTimeCtrl(IDC_dateAGREE, m_strAgreeDate);
			}
			if(_T("") != m_strExpireDate)
			{
				SetStringDateToDateTimeCtrl(IDC_dateEXPIRE, m_strExpireDate);
			}
			else
			{

				((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->EnableWindow(FALSE);	
			}
			
			
			if(m_strStatementAliasBefore == m_strStatementAlias)
			{
				((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->SetCheck(1);
				((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->EnableWindow(FALSE);
				((CDateTimeCtrl*)GetDlgItem(IDC_dateAGREE))->EnableWindow(FALSE);
				((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->EnableWindow(FALSE);
			}
			((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->SetCurSel(_ttoi(m_strAgreeType));
			((CEdit*)GetDlgItem(IDC_edtAGENTNAME))->SetWindowText(m_strAgentName);
			((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->SetWindowText(m_strAgentNote);
			if(_T("�İ���") == m_strAgentRelation)
			{
				((CComboBox*)GetDlgItem(IDC_comboRELATION))->SetCurSel(1);
			}
			else if(_T("��Ÿ") == m_strAgentRelation)
			{
				((CComboBox*)GetDlgItem(IDC_comboRELATION))->SetCurSel(2);
			}
			else
			{
				((CComboBox*)GetDlgItem(IDC_comboRELATION))->SetCurSel(0);
			}
		}
		else
		{
			AfxMessageBox(_T("������ �ҷ����µ� �����Ͽ����ϴ�."));
			return -100;
		}
	}
	else if(2 == m_nMode)
	{
		
		if(_T("") == m_strUserKey)
		{
			AfxMessageBox(_T("������ �ҷ����µ� �����Ͽ����ϴ�."));
			return -100;
		}
	}

	
	if(_T("1") == m_strAgreeType)
	{
		((CDateTimeCtrl*)GetDlgItem(IDC_dateAGREE))->ShowWindow(SW_HIDE);
		((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staticAGREEDATE))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staticEXPIRE))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_chkKL_STATEMENT))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->SetCheck(1);
	}


	
	if(!m_bUnder14YN && _T("") == m_strAgentName)
	{
		((CEdit*)GetDlgItem(IDC_edtAGENTNAME))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_comboRELATION))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->EnableWindow(FALSE);
	}

	AllControlDisplay(_T("CM_���Ǳ��"));
	AllControlDisplay(_T("CM_�븮������"));
	SetManageMsg();

	return 0;
}


INT KL_StatementDlg::SaveScreen() 
{
	INT ids = CheckUpdate();
	
	if(m_bConfirm)
	{
		SetData();
		if(0 > SaveAgreementInfo())
		{
			return -100;
		}
		if(0 > SaveAgentInfo())
		{
			return -200;
		}
		return 400;
	}
	else if(100 == ids)
	{
		SetData();
		if(0 > SaveAgreementInfo())
		{
			return -100;
		}
		return 100;
	}
	else if(200 == ids)
	{
		SetData();
		if(0 > SaveAgentInfo())
		{
			return -200;
		}
		return 200;
	}
	else if(300 == ids)
	{
		SetData();
		if(0 > SaveAgreementInfo())
		{
			return -100;
		}
		if(0 > SaveAgentInfo())
		{
			return -200;
		}
		return 300;
	}
	return 0;
}


INT KL_StatementDlg::CheckUpdate() 
{
	CString strAgreeType;
	CString strAgreeDate;
	CString strExpireDate;
	CString strAgentName;
	CString strAgentRelation;
	CString strAgentNote;

	
	strAgreeType.Format(_T("%d"), ((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->GetCurSel());
	
	strAgreeDate = GetStringDateFromDateTimeCtrl(IDC_dateAGREE);
	
	strExpireDate = GetStringDateFromDateTimeCtrl(IDC_dateEXPIRE);
	
	((CEdit*)GetDlgItem(IDC_edtAGENTNAME))->GetWindowText(strAgentName);
	
	strAgentRelation.Format(_T("%d"), ((CComboBox*)GetDlgItem(IDC_comboRELATION))->GetCurSel());
	if(_T("1") == strAgentRelation)
	{
		strAgentRelation = _T("�İ���");
	}
	else if(_T("2") == strAgentRelation)
	{
		strAgentRelation = _T("��Ÿ");
	}
	else
	{
		strAgentRelation = _T("ģ����");
	}
	
	((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->GetWindowText(strAgentNote);
	if(_T("EX) �θ�� ���� / �����븮�� ����Ȯ��") == strAgentNote)
	{
		strAgentNote = _T("");
	}
	INT ids1 = 0; 
	INT	ids2 = 0;
	

	if((m_strStatementAliasBefore != m_strStatementAlias) && 1 == ((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->GetCheck() && ((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->IsWindowEnabled())
	{
		ids1 = 100;
	}
	else if((strAgreeDate != m_strAgreeDate && ((CDateTimeCtrl*)GetDlgItem(IDC_dateAGREE))->IsWindowEnabled()) || (strExpireDate != m_strExpireDate && ((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->IsWindowEnabled()))
	{
		ids1 = 100;
	}

	if(strAgentName != m_strAgentName)
	{
		ids2 = 200;
	}
	else if(_T("") != strAgentName && (strAgentRelation != m_strAgentRelation || strAgentNote != m_strAgentNote))
	{
		ids2 = 200;
	}
	return ids1 + ids2;
}

INT KL_StatementDlg::SetData() 
{	
	if(!m_bIsAgreeYN)
	{
		m_strAgentName = _T("");
		m_strAgentRelation = _T("");
		m_strAgentNote = _T("");
		return 0;
	}
	
	m_strAgreeType.Format(_T("%d"), ((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->GetCurSel());
	
	m_strAgreeDate = GetStringDateFromDateTimeCtrl(IDC_dateAGREE);
	
	m_strExpireDate = GetStringDateFromDateTimeCtrl(IDC_dateEXPIRE);
	
	((CEdit*)GetDlgItem(IDC_edtAGENTNAME))->GetWindowText(m_strAgentName);
	
	m_strAgentRelation.Format(_T("%d"), ((CComboBox*)GetDlgItem(IDC_comboRELATION))->GetCurSel());
	if(_T("1") == m_strAgentRelation)
	{
		m_strAgentRelation = _T("�İ���");
	}
	else if(_T("2") == m_strAgentRelation)
	{
		m_strAgentRelation = _T("��Ÿ");
	}
	else
	{
		m_strAgentRelation = _T("ģ����");
	}
	
	((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->GetWindowText(m_strAgentNote);
	if(_T("EX) �θ�� ���� / �����븮�� ����Ȯ��") == m_strAgentNote)
	{
		m_strAgentNote = _T("");
	}

	return 0;
}

CString KL_StatementDlg::GetStringDateFromDateTimeCtrl(INT nID) 
{
   CTime time; 
   CString strDate;
   DWORD dwResult = ((CDateTimeCtrl*)GetDlgItem(nID))->GetTime(time);
   strDate = time.Format("%Y/%m/%d");
   return strDate;
}

INT KL_StatementDlg::SetStringDateToDateTimeCtrl(INT nID, CString strDate /*EX: YYYY/MM/DD*/) 
{
	if(10 > strDate.GetLength())
	{
		return -100;
	}
	strDate = strDate.Left(10);
	INT nYear, nMonth, nDay;
	nYear = _ttoi(strDate.Left(4));
	nMonth = _ttoi(strDate.Mid(5, 2));
	nDay = _ttoi(strDate.Right(2));
	CTime time(nYear, nMonth, nDay, 0, 0, 0);
	((CDateTimeCtrl*)GetDlgItem(nID))->SetTime(&time);

	return 0;
}


CString KL_StatementDlg::SetExpireDate() 
{
	CString strDate;
	CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime((CESL_Mgr*)this, strDate);
	tCurrentTime += CTimeSpan(m_pInfo->m_nUserInfoRenewalDay, 0,0,0);
	strDate = tCurrentTime.Format("%Y/%m/%d");

	return strDate;
}

INT KL_StatementDlg::SetManageMsg() 
{
	CString strDate;
	CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime((CESL_Mgr*)this, strDate);

	BOOL bAgreeYN;
	BOOL bRealNameCertYN;

	
	bAgreeYN = m_bIsAgreeYN;
	
	if(m_bIsKLMode)
	{
		bRealNameCertYN = m_bRealNameCertYN;
	}
	else if(!m_pInfo->m_bCertifyUse && m_pInfo->m_bKLUse && m_bIsKLMemberYN)
	{
		if(_T("") != m_strGpinHash || 1 == m_nMode)
		{
			bRealNameCertYN = TRUE;
		}
		else
		{
			bRealNameCertYN = FALSE;
		}
	}
	else
	{
		bRealNameCertYN = FALSE;
	}

	SetManageStatus(bAgreeYN, bRealNameCertYN, m_bUnder14YN, m_strAgentName);
	
	
	return 0;
}

void KL_StatementDlg::OnbtnCONFIRM() 
{
	((CStatic*)GetDlgItem(IDC_staticAGREEDATE))->ShowWindow(SW_SHOW);
	((CStatic*)GetDlgItem(IDC_staticEXPIRE))->ShowWindow(SW_SHOW);
	((CDateTimeCtrl*)GetDlgItem(IDC_dateAGREE))->ShowWindow(SW_SHOW);
	((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->ShowWindow(SW_SHOW);
	((CDateTimeCtrl*)GetDlgItem(IDC_dateAGREE))->EnableWindow(TRUE);
	((CDateTimeCtrl*)GetDlgItem(IDC_dateEXPIRE))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_chkKL_STATEMENT))->SetCheck(0);
	((CComboBox*)GetDlgItem(IDC_comboAGREETYPE))->SetCurSel(0);

	CString strDate;
	CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime((CESL_Mgr*)this, strDate);
	strDate = strDate.Left(10);
	SetStringDateToDateTimeCtrl(IDC_dateAGREE, strDate);
	SetStringDateToDateTimeCtrl(IDC_dateEXPIRE, SetExpireDate());
	m_bConfirm = TRUE;
}

void KL_StatementDlg::OnDatetimechangedateAGREE(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	*pResult = 0;
}

void KL_StatementDlg::OnSetfocusedtRELATIONNOTE() 
{
	if(0 == m_nMode)
	{
		CString strMsg;
		((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->GetWindowText(strMsg);
		if(_T("EX) �θ�� ���� / �����븮�� ����Ȯ��") == strMsg)
		{
			((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->SetWindowText(_T(""));	
		}
	}
}

void KL_StatementDlg::OnKillfocusedtRELATIONNOTE() 
{
	if(0 == m_nMode)
	{
		CString strMsg;
		((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->GetWindowText(strMsg);
		if(_T("") == strMsg)
		{
			((CEdit*)GetDlgItem(IDC_edtRELATIONNOTE))->SetWindowText(_T("EX) �θ�� ���� / �����븮�� ����Ȯ��"));	
		}
	}
}

VOID KL_StatementDlg::SetManageStatus(BOOL bAgreeYN, BOOL bRealNameCertYN, BOOL bUnder14YN, CString strAgentName) 
{
	CString strMsg, strMsg1, strMsg2, strMsg3, strMsg4, strMsg5;

	if(bAgreeYN)
	{
		((CStatic*)GetDlgItem(IDC_staticAGREESTATUS))->SetWindowText(_T("���ǿϷ�"));
	}
	else
	{
		((CStatic*)GetDlgItem(IDC_staticAGREESTATUS))->SetWindowText(_T("�̵���"));
	}
	if(bRealNameCertYN)
	{
		((CStatic*)GetDlgItem(IDC_staticREALNAMESTATUS))->SetWindowText(_T("�����Ϸ�"));
	}
	else
	{
		((CStatic*)GetDlgItem(IDC_staticREALNAMESTATUS))->SetWindowText(_T("������"));
	}
	
	if(!m_pInfo->m_bCertifyUse && m_pInfo->m_bKLUse && m_bIsKLMemberYN && 0 == m_nMode)
	{
		((CStatic*)GetDlgItem(IDC_staticREALNAMESTATUS))->SetWindowText(_T("�ش����"));
		bRealNameCertYN = TRUE;
	}

	if(bUnder14YN)
	{
		if(_T("") != strAgentName)
		{
			((CStatic*)GetDlgItem(IDC_staticAGENTSTATUS))->SetWindowText(_T("�����Ϸ�"));
		}
		else
		{
			((CStatic*)GetDlgItem(IDC_staticAGENTSTATUS))->SetWindowText(_T("������"));
		}
	}
	else
	{
		((CStatic*)GetDlgItem(IDC_staticAGENTSTATUS))->SetWindowText(_T("�ش����"));
	}
	
	
	if(bAgreeYN && bRealNameCertYN && ((bUnder14YN && _T("") != strAgentName) || !bUnder14YN))
	{
		strMsg1 = _T("�������� ���� ���°� �����Դϴ�.");
	}	
	
	else if(bAgreeYN && !bRealNameCertYN && ((bUnder14YN && _T("") != strAgentName) || !bUnder14YN))
	{

		strMsg1 = _T("1. �ϴ��� [Ȯ��] ��ư�� Ŭ���ϸ� ���������� ������ �� �ֽ��ϴ�. \r\n");	
	}	
	
	else if(bAgreeYN && bRealNameCertYN && (bUnder14YN && _T("") == strAgentName))
	{
		strMsg1 = _T("1. �̿����� �����븮�� ���ǰ� �ʿ��մϴ�. \r\n   - �����븮���� ������ ���� �Ǵ� ���� ���� ������� Ȯ���մϴ�.\r\n");
		strMsg2 = _T("2. �����븮���� �̸� �� ���踦 �Է��ϼ���.\r\n");
		strMsg3 = _T("3. [Ȯ��] ��ư�� Ŭ���ϸ� �Էµ� ������ ����˴ϴ�.");
	}	
	
	else if(bAgreeYN && !bRealNameCertYN && (bUnder14YN && _T("") == strAgentName))
	{
		strMsg1 = _T("1. �̿����� �����븮�� ���ǰ� �ʿ��մϴ�. \r\n   - �����븮���� ������ ���� �Ǵ� ���� ���� ������� Ȯ���մϴ�.\r\n");
		strMsg2 = _T("2. �����븮���� �̸� �� ���踦 �Է��ϼ���.\r\n");
		strMsg3 = _T("3. [Ȯ��] ��ư�� Ŭ���ϸ� �Է��� ������ ����Ǹ� ���������� ������ �� �ֽ��ϴ�.");	
	}
	
	else if(!bAgreeYN && bRealNameCertYN && ((bUnder14YN && _T("") != strAgentName) || !bUnder14YN))
	{
		strMsg1 = _T("1. �Ʒ� �̿����� �̿��ڿ��� �������� �ȳ��ϼ���. \r\n");
		strMsg2 = _T("2. �̿��ڿ��� ���� Ȯ���� �������� ��������.\r\n");
		strMsg3 = _T("3. �������� ���� ���� �׸��� üũ�ڽ��� Ŭ���ϼ���.\r\n");
		strMsg4 = _T("4. [Ȯ��] ��ư�� Ŭ���ϸ� �Էµ� ������ ����˴ϴ�.");
	}
	
	else if(!bAgreeYN && !bRealNameCertYN && ((bUnder14YN && _T("") != strAgentName) || !bUnder14YN))
	{
		strMsg1 = _T("1. �Ʒ� �̿����� �̿��ڿ��� �������� �ȳ��ϼ���. \r\n");
		strMsg2 = _T("2. �̿��ڿ��� ���� Ȯ���� �������� ��������.\r\n");
		strMsg3 = _T("3. �������� ���� ���� �׸��� üũ�ڽ��� Ŭ���ϼ���.\r\n");
		strMsg4 = _T("4. [Ȯ��] ��ư�� Ŭ���ϸ� �Է��� ������ ����Ǹ� ���������� ������ �� �ֽ��ϴ�.");
	}
	
	else if(!bAgreeYN && bRealNameCertYN && (bUnder14YN && _T("") == strAgentName))
	{
		strMsg1 = _T("1. �Ʒ� �̿����� �̿��ڿ��� �������� �ȳ��ϼ���. \r\n");
		strMsg2 = _T("2. �̿����� �����븮�� ���ǰ� �ʿ��մϴ�. \r\n   - �����븮���� ������ ���� �Ǵ� ���� ���� ������� Ȯ���մϴ�.\r\n");
		strMsg3 = _T("3. �������� ���� ���� �׸��� üũ�ڽ��� Ŭ���ϼ���.\r\n");
		strMsg4 = _T("4. �����븮���� �̸� �� ���踦 �Է��ϼ���.\r\n");
		strMsg5 = _T("5. [Ȯ��] ��ư�� Ŭ���ϸ� �Էµ� ������ ����˴ϴ�.");
	}
	
	else if(bAgreeYN && bRealNameCertYN && (bUnder14YN && _T("") == strAgentName))
	{
		strMsg1 = _T("1. �̿����� �����븮�� ���ǰ� �ʿ��մϴ�. \r\n   - �����븮���� ������ ���� �Ǵ� ���� ���� ������� Ȯ���մϴ�.\r\n");
		strMsg2 = _T("2. �����븮���� �̸� �� ���踦 �Է��ϼ���.\r\n");
		strMsg3 = _T("3. [Ȯ��] ��ư�� Ŭ���ϸ� �Էµ� ������ ����˴ϴ�.");
	}
	
	else if(!bAgreeYN && !bRealNameCertYN && (bUnder14YN && _T("") == strAgentName))
	{
		strMsg1 = _T("1. �Ʒ� �̿����� �̿��ڿ��� �������� �ȳ��ϼ���. \r\n");
		strMsg2 = _T("2. �̿����� �����븮�� ���ǰ� �ʿ��մϴ�. \r\n   - �����븮���� ������ ���� �Ǵ� ���� ���� ������� Ȯ���մϴ�.\r\n");
		strMsg3 = _T("3. �������� ���� ���� �׸��� üũ�ڽ��� Ŭ���ϼ���.\r\n");
		strMsg4 = _T("4. �����븮���� �̸� �� ���踦 �Է��ϼ���.\r\n");
		strMsg5 = _T("5. [Ȯ��] ��ư�� Ŭ���ϸ� �Է��� ������ ����Ǹ� ���������� ������ �� �ֽ��ϴ�.");	
	}

	if(_T("1") == m_strAgreeType)
	{
		((CStatic*)GetDlgItem(IDC_staticAGREESTATUS))->SetWindowText(_T("���ǿϷ�"));
		((CStatic*)GetDlgItem(IDC_staticAGENTSTATUS))->SetWindowText(_T("�����Ϸ�"));
		strMsg = _T("���������� �������� �����Ǵ� �̿����Դϴ�.");
	}
	else
	{
		strMsg.Format(_T("%s%s%s%s%s"), strMsg1, strMsg2, strMsg3, strMsg4, strMsg5);
	}	

	if(0 < m_pInfo->m_nUserInfoRenewalDay && (m_pInfo->m_bKLUse && m_bIsKLMemberYN))
	{
		
		if(10 <= m_strExpireDate.GetLength())
		{
			CString strGetData;
			m_strExpireDate = m_strExpireDate.Left(10);
			CTime tRenewalTime(_ttoi(m_strExpireDate.Left(4)),_ttoi(m_strExpireDate.Mid(5, 2)),_ttoi(m_strExpireDate.Right(2)),0,0,0);
			CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime((CESL_Mgr*)this, strGetData);
			CTimeSpan tSpan =  tRenewalTime - tCurrentTime;
			INT nElapseDay = (INT)tSpan.GetDays();
			if(0 > nElapseDay)
			{	
				strMsg = _T("ȸ������ �����Ⱓ�� ����Ǿ����ϴ�.\r\n[������������]��ư�� Ŭ���� �� �絿�Ǹ� �޾��ּ���.");				
				((CStatic*)GetDlgItem(IDC_staticAGREESTATUS))->SetWindowText(_T("�絿���ʿ�"));
				((CStatic*)GetDlgItem(IDC_staticAGENTSTATUS))->SetWindowText(_T("�������ʿ�"));	
				if(m_pInfo->m_bCertifyUse)
				{
					((CStatic*)GetDlgItem(IDC_staticREALNAMESTATUS))->SetWindowText(_T("�������ʿ�"));
				}
				else
				{
					((CStatic*)GetDlgItem(IDC_staticREALNAMESTATUS))->SetWindowText(_T("�ش����"));
				}
			}
		}
		else if(_T("") == m_strExpireDate)
		{
			strMsg = _T("�������� �������� �Էµ��� �ʾҽ��ϴ�.\r\n[������������]��ư�� Ŭ���� �� �絿�Ǹ� �޾��ּ���.");			
			((CStatic*)GetDlgItem(IDC_staticAGREESTATUS))->SetWindowText(_T("�絿���ʿ�"));
			((CStatic*)GetDlgItem(IDC_staticAGENTSTATUS))->SetWindowText(_T("�������ʿ�"));
			((CButton*)GetDlgItem(IDC_btnCONFIRM))->ShowWindow(SW_SHOW);
			if(m_pInfo->m_bCertifyUse)
			{
				((CStatic*)GetDlgItem(IDC_staticREALNAMESTATUS))->SetWindowText(_T("�������ʿ�"));
			}
			else
			{
				((CStatic*)GetDlgItem(IDC_staticREALNAMESTATUS))->SetWindowText(_T("�ش����"));
			}
		}
		
		if(1 == m_nMode)
		{
			((CButton*)GetDlgItem(IDC_btnCONFIRM))->ShowWindow(SW_SHOW);
		}
	}
	GetDlgItem(IDC_edtMANAGEINFO)->SetWindowText(strMsg);
}


void KL_StatementDlg::OnImageMng() 
{
	CKL_STATEMENT_IMAGE* StatementImgDlg = NULL;
	StatementImgDlg = new CKL_STATEMENT_IMAGE(this);	
	if ( m_strUserKey.GetLength() <= 0 ) StatementImgDlg->MakeTmpUserKey();
	else StatementImgDlg->SetUserKey(m_strUserKey);
	StatementImgDlg->DoModal();	

	StatementImgDlg->GetTmpUserKey(m_strTmpImgUserKey);

	if ( NULL != StatementImgDlg )
	{
		delete StatementImgDlg;
		StatementImgDlg = NULL;
	}
}

void KL_StatementDlg::GetTmpUserKey(CString& strKey)
{
	strKey = m_strTmpImgUserKey;
}




