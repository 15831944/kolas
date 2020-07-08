// KlUserSyncDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KlUserSyncDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKlUserSyncDlg dialog


CKlUserSyncDlg::CKlUserSyncDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKlUserSyncDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKlUserSyncDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLoan = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_bIsRunningThread = FALSE;
	m_strUserNo = _T("");
	m_strUserName = _T("");
	m_strCryptoCivilNo = _T("");
	m_strLibCode = _T("");
	m_strGPinHash = _T("");
	m_strGPinSex = _T("");
	m_strRequestLibCode = _T("");
	m_strHandPhone = _T("");
	m_strUserClass = _T("");
	m_strMemberClass = _T("");
	m_strBirthYear = _T("");

	m_strCI = _T("");
}
CKlUserSyncDlg::~CKlUserSyncDlg()
{
	if(NULL != m_pDM)
	{
		m_pDM->FreeData();
	}
}


void CKlUserSyncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKlUserSyncDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKlUserSyncDlg, CDialog)
	//{{AFX_MSG_MAP(CKlUserSyncDlg)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnKUSY_LibSearch, OnbtnKUSYLibSearch)
	ON_BN_CLICKED(IDC_btnKUSY_UserSearch, OnbtnKUSYUserSearch)
	ON_BN_CLICKED(IDC_btnKUSY_Switch, OnbtnKUSYSwitch)
	ON_BN_CLICKED(IDC_btnKUSY_Close, OnbtnKUSYClose)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_GRID_DISPLAY, OnGridDisplay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKlUserSyncDlg message handlers

BOOL CKlUserSyncDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	if(InitESL_Mgr(_T("SM_KL_USER_SYNC")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	m_pDM = FindDM(_T("DM_KL_USER_SYNC"));
	if(NULL == m_pDM)
	{
		AfxMessageBox(_T("DM_KL_USER_SYNC"));
		return FALSE;
	}
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_KL_USER_SYNC"), _T("MainGrid"));
	if(NULL == m_pGrid)
	{
		AfxMessageBox(_T("CM_KL_USER_SYNC-MainGrid"));
		return FALSE;
	}

	SetTimer(0, 100, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKlUserSyncDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnKUSYClose();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnKUSY_LibSearch)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUSYLibSearch();
			}
			else if(GetDlgItem(IDC_btnKUSY_UserSearch)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUSYUserSearch();
			}
			else if(GetDlgItem(IDC_btnKUSY_Switch)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUSYSwitch();
			}
			else if(GetDlgItem(IDC_btnKUSY_Close)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUSYClose();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CKlUserSyncDlg::OnTimer(UINT nIDEvent) 
{
	if(0==nIDEvent)
	{
		KillTimer(0);
		AfxBeginThread(AccessionLibThread, this);
	}
	else if(2 == nIDEvent)
	{
		KillTimer(2);
		CString strMsg;
		strMsg.Format(_T("%s���� ���� ������ ���"), m_strUserName);
		GetDlgItem(IDC_stcKUSY_LibList)->SetWindowText(strMsg);
 		GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(_T("�������� �����ϰ� �̿��ڸ� �˻��Ͻʽÿ�.\r\n�� ����ȭ ��� �������� ����Ŭ���ϼ���."));
		GetDlgItem(IDC_btnKUSY_LibSearch)->ShowWindow(SW_HIDE);
		m_strLibCode = _T("");
		GetDlgItem(IDC_edtKUSY_LibCode)->SetWindowText(_T(""));
		GetDlgItem(IDC_btnKUSY_UserSearch)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUSY_Switch)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtKUSY_LibCode)->EnableWindow(TRUE);
		GetDlgItem(IDC_edtKUSY_LibCode)->SetFocus();
	}
	else if(3 == nIDEvent || 4 == nIDEvent)
	{
		KillTimer(nIDEvent);
		CString strMsg;
		if(4 == nIDEvent)
		{
 			strMsg = _T("���Ե����� ��Ͽ� �ڰ� ������ �����ϴ�.");
		}
		else
		{
			strMsg = _T("���� ������ ��ȸ�� ��õ� �Ͻʽÿ�.");
		}
		GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_btnKUSY_LibSearch)->ShowWindow(SW_SHOW);
		CRect rect;
		GetDlgItem(IDC_btnKUSY_LibSearch)->GetWindowRect(rect);
		SetCursorPos((rect.left+rect.right)/2, (rect.top+rect.bottom)/2);
		GetDlgItem(IDC_btnKUSY_LibSearch)->SetFocus();
		GetDlgItem(IDC_edtKUSY_LibCode)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUSY_UserSearch)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUSY_Switch)->EnableWindow(FALSE);
	}
	else if(5 == nIDEvent)
	{
		KillTimer(5);
		AfxBeginThread(UserSearchThread, this);
	}
	else if(6 == nIDEvent)
	{
		KillTimer(6);
		AfxBeginThread(UserSwitchThread, this);
	}
	else if(7 == nIDEvent)
	{
		KillTimer(7);
		GetDlgItem(IDC_btnKUSY_Switch)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnKUSY_Switch)->SetFocus();
		GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(_T("������ �������� �̿��ڴ� �Ϲ�ȸ���Դϴ�.\r\n�� å����ȸ���� ���ؼ��� ����ȭ�� ������ �� �ֽ��ϴ�."));
	}
	else if(8 == nIDEvent)
	{
		KillTimer(8);
		GetDlgItem(IDC_btnKUSY_Switch)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUSY_UserSearch)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnKUSY_UserSearch)->SetFocus();
		GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(_T("������ ���������� �˻��� �̿��ڰ� �����ϴ�."));
	}
	else if(9 == nIDEvent)
	{
		KillTimer(9);
		GetDlgItem(IDC_btnKUSY_Switch)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUSY_UserSearch)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnKUSY_UserSearch)->SetFocus();
		GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(_T("�̿��� ��ȸ�� �����Ͽ����ϴ�.\r\n�� �̿�����ȸ ��ư�� ���� ��õ��Ͻʽÿ�."));
	}
	else if(10 == nIDEvent)
	{
		KillTimer(10);
		MessageBox(_T("  ����ȭ �۾��� ��ϵǾ����ϴ�.\r\n\r\n �� ���Ϳ� ����Ǳ���� ������ �ð��� �ҿ�˴ϴ�."), _T("å����ȸ������ȭ"), MB_OK|MB_ICONINFORMATION);
		OnOK();
	}
	else if(11 == nIDEvent)
	{
		KillTimer(11);
		GetDlgItem(IDC_btnKUSY_Switch)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnKUSY_Switch)->SetFocus();
		GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(_T("å����ȸ�� ����ȭ�� �����Ͽ����ϴ�.\r\n�� ����ȭ ��ư�� ���� ��õ��Ͻʽÿ�."));
	}
	else if(12 == nIDEvent)
	{
		KillTimer(12);
		GetDlgItem(IDC_btnKUSY_UserSearch)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUSY_Switch)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnKUSY_Switch)->SetFocus();
		GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(_T("����ȭ ��ư�� �����ϸ� å����ȸ�� ������ ����ȭ�մϴ�."));
	}
	else if(14 == nIDEvent)
	{
		KillTimer(14);
		INT ids = 0;
		while(TRUE)
		{
			if(IDYES == MessageBox(_T("���� �ڰ� å����ȸ�� ���࿡ �����Ͽ����ϴ�.\r\n��õ��Ͻðڽ��ϱ�?"), _T("å����ȸ������"), MB_YESNO|MB_ICONQUESTION))
			{
				ids = SetCenterUser();
			}
			else
			{
				break;
			}
			if(0 == ids)
			{
				break;
			}
		}
		if(0 == ids)
		{
			SetTimer(2, 100, NULL);
		}
		else
		{
			SetTimer(4, 100, NULL);
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

VOID CKlUserSyncDlg::SetLibCodeControl(CString strLibCode, CString strLibName)
{
	if(6 != strLibCode.GetLength())
	{
		Step2(6, strLibCode);
		return;
	}
	if(TRUE == m_pLoan->IsNumericText(strLibCode))
	{
		if(strLibName.IsEmpty())
		{
			SetDlgItemText(IDC_edtKUSY_LibCode, _T("�������� ��ȸ���Դϴ�."));
			CFrameWriter Writer(m_pInfo);
			Writer.setCommand(_T("IL_K16_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("��������ȣ"), strLibCode);
			CFrameReader Reader;
			CFrameSender Sender(m_pInfo);
			INT ids;
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if(0 > ids)
			{
				Step2(2);
				return;
			}
			CESL_DataMgr tmpDM;
			tmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
			Reader.makeDM(&tmpDM);
			if(1 > tmpDM.GetRowCount())
			{
				Step2(3, strLibCode);
				return;
			}
			CString strTemp = Reader.getCommand();
			if(_T("ERROR") == strTemp)
			{
				CString strMsg;
				Reader.getError(strTemp, strMsg);
				if(0 < strTemp.Find(_T("DT")))
				{
					Step2(3, strLibCode);
					return;
				}
				else
				{
					Step2(2);
					return;
				}
			}
			tmpDM.GetCellData(_T("��������"), 0, strLibName);
			tmpDM.FreeData();
		}
		if(strLibName.IsEmpty())
		{
			Step2(3, strLibCode);
			return;
		}
		else
		{
			CString strValue;
			strValue.Format(_T("%s(%s)"), strLibCode, strLibName);
			SetDlgItemText(IDC_edtKUSY_LibCode, strValue);
			Step2(0, strLibCode);
			SetTimer(5, 100, NULL);
			return;
		}
	}
	else
	{ 
		Step2(4, strLibCode);
		return;
	}
}

VOID CKlUserSyncDlg::Step2(INT nValue, CString strLibcode/*=_T("")*/)
{
	m_strLibCode = strLibcode;
	CString strMsg;
	if(0 == nValue)
	{
	}
	else
	{
		if(1 == nValue)
		{
			strMsg = _T("������ �������� �̹� å����ȸ���Դϴ�.");
		}
		else if(2 == nValue)
		{
			strMsg = _T("������ ��ȸ�� �����Ͽ����ϴ�.");
		}
		else if(3 == nValue)
		{
			strMsg.Format(_T("������ ��ȸ ����� �����ϴ�.\r\n�Է��� ��������ȣ : %s"), strLibcode);
		}
		else if(4 == nValue)
		{
			strMsg.Format(_T("��Ȯ�� ��������ȣ�� �Է��Ͻʽÿ�.\r\n�Է��� ��������ȣ : %s"), strLibcode);
		}
		else if(5 == nValue)
		{
			strMsg = _T("�ڰ� ��������ȣ�� ������ �� �����ϴ�.");
		}
		else if(6 == nValue)
		{
			strMsg.Format(_T("��������ȣ 6�ڸ��� �Է��Ͻʽÿ�.\r\n�Է��� ��������ȣ : %s"), strLibcode);
		}
		else
		{
			strMsg = _T("������ ��ȸ�� �����Ͽ����ϴ�.\r\n��õ� �Ͻʽÿ�.");
		}
		GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_btnKUSY_UserSearch)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUSY_Switch)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtKUSY_LibCode)->SetWindowText(_T(""));
		GetDlgItem(IDC_edtKUSY_LibCode)->SetFocus();
	}
}

HBRUSH CKlUserSyncDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcKUSY_Status == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

UINT CKlUserSyncDlg::AccessionLibThread(LPVOID pParam)
{
	CKlUserSyncDlg* pDlg = (CKlUserSyncDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;

	INT ids = 0;
	ids = pDlg->SearchAccessionLib();
	if(0 == ids) 
	{
		pDlg->SetTimer(2, 100, NULL);
	}
	else if(100 == ids)
	{
		ids = pDlg->SetCenterUser();
		if(0 != ids)
		{
			pDlg->SetTimer(14, 100, NULL);
		}
		else
		{
			ids = pDlg->SearchAccessionLib();
			if(0 == ids) 
			{
				pDlg->SetTimer(2, 100, NULL);
			}
			else
			{
				pDlg->SetTimer(3, 100, NULL);
			}
		}
	}
	else
	{
		pDlg->SetTimer(3, 100, NULL);
	}

	pDlg->m_bIsRunningThread = FALSE;
	return 0;
}

UINT CKlUserSyncDlg::UserSearchThread(LPVOID pParam)
{
	CKlUserSyncDlg* pDlg = (CKlUserSyncDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;

	INT ids = pDlg->UserSearch();
	if(0 == ids) 
	{
		pDlg->SetTimer(7, 100, NULL);
	}
	else if(100 == ids)
	{
		pDlg->SetTimer(8, 100, NULL);
	}
	else if(200 == ids)
	{
		pDlg->SetTimer(12, 100, NULL);
	}
	else
	{
		pDlg->SetTimer(9, 100, NULL);
	}

	pDlg->m_bIsRunningThread = FALSE;

	return 0;
}

UINT CKlUserSyncDlg::UserSwitchThread(LPVOID pParam)
{
	CKlUserSyncDlg* pDlg = (CKlUserSyncDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	pDlg->m_bIsRunningThread = TRUE;

	INT ids = pDlg->UserSwitch();
	if(0 == ids) 
	{
		pDlg->SetTimer(10, 100, NULL);
	}
	else
	{
		pDlg->SetTimer(11, 100, NULL);
	}

	pDlg->m_bIsRunningThread = FALSE;

	return 0;
}

INT CKlUserSyncDlg::SearchAccessionLib()
{
	INT ids;
	GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(_T("�̿����� ���� ������ ����� �������� ���Դϴ�."));

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K48_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), m_strUserNo);
	Writer.addElement(_T("G-PINHASH�����ڵ�"), m_strGPinHash);
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) return -1;

	m_pDM->FreeData();
	Reader.makeDM(m_pDM);	

	CString strTmpData = _T("");
	for(int j=0; j<m_pDM->GetRowCount();j++)
	{
		m_pDM->GetCellData(_T("ȸ������"), j, strTmpData);
		if( strTmpData.Compare(_T("0")) == 0)
		{
			strTmpData = _T("����");
		}
		else if( strTmpData.Compare(_T("1")) == 0)
		{
			strTmpData = _T("����");
		}
		else if( strTmpData.Compare(_T("2")) == 0)
		{
			strTmpData = _T("����");
		}
		else if( strTmpData.Compare(_T("3")) == 0)
		{
			strTmpData = _T("Ż��");
		}
		m_pDM->SetCellData(_T("ȸ������"), strTmpData, j);
	}

	SendMessage(WM_GRID_DISPLAY, 0, 0);

	if(1 > m_pDM->GetRowCount()) return -2;
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strMsg;
		Reader.getError(strTemp, strMsg);
		if(0 < strTemp.Find(_T("DT")))
		{
			return 100;
		}
		else
		{
			return -3;
		}
	}

	BOOL bExist = FALSE;
	CString strLibCode;
	for(INT i = 0; i < m_pDM->GetRowCount(); i++)
	{
		m_pDM->GetCellData(_T("���Ե�������ȣ"), i, strLibCode);
		if(strLibCode == m_pInfo->LIB_CODE)
		{
			bExist = TRUE;
		}
	}
	if(FALSE == bExist) return 100;

	return 0;
}

INT CKlUserSyncDlg::SetCenterUser()
{
	GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(_T("���Ϳ� �ڰ� å����ȸ�������� �����մϴ�."));
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K02_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), m_strUserNo);
	Writer.addElement(_T("�������̸�"), m_strUserName);
	Writer.addElement(_T("������������ȣ"), m_pInfo->LIB_CODE);
	CString strSysDate;
	CTime t = m_pLoan->m_pLocCommonAPI->GetCurrentDateTime(this, strSysDate);
	Writer.addElement(_T("���ʻ�����"), strSysDate);
	Writer.addElement(_T("�ڵ���"), m_strHandPhone);
	Writer.addElement(_T("G-PINHASH�����ڵ�"), m_strGPinHash);
	Writer.addElement(_T("G-PIN����"), m_strGPinSex);
	Writer.addElement(_T("ȸ������"), m_strUserClass);
	Writer.addElement(_T("ȸ������"), m_strMemberClass);
	Writer.addElement(_T("����"), m_strBirthYear);
	Writer.addElement(_T("��������"), m_strCI);

 	CFrameSender Sender(m_pInfo);
 	CFrameReader Reader;
 	Sender.CommMiddleWareDaemon(&Writer, &Reader);
 	CESL_DataMgr TempDM;
 	Reader.makeDM(&TempDM);
 	CString strTemp = Reader.getCommand();
 	if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
 	{
 		return 1;
 	}
	return 0;
}

void CKlUserSyncDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;	

	CDialog::OnClose();
}

LRESULT CKlUserSyncDlg::OnGridDisplay(WPARAM wParam, LPARAM lParam)
{
	if(NULL != m_pGrid)
	{
		m_pGrid->m_nHeight = 25;
		m_pGrid->Display();
		m_pGrid->SetReverse(0);
	}

	return 0;
}

INT CKlUserSyncDlg::UserSearch()
{
	GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(_T("�̿��� ��ȸ���Դϴ�."));

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K33_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), m_strLibCode);
	Writer.addElement(_T("�������̸�"), m_strUserName);
	Writer.addElement(_T("��ȣȭ�ֹε�Ϲ�ȣ"), m_strCryptoCivilNo);
	Writer.addElement(_T("�����ڹ�ȣ"), m_strUserNo);

	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	INT ids;
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) return -1;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	Reader.makeDM(&tmpDM);
	if(1 > tmpDM.GetRowCount()) 
	{
		return -1;
	}
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strMsg;
		Reader.getError(strTemp, strMsg);
		if(0 < strTemp.Find(_T("DT")))
		{
			return 100;
		}
		else
		{
			return -2;
		}
	}
	CString strValue;
	tmpDM.GetCellData(_T("ó�����"), 0, strValue);
	if(_T("OK") != strValue)
	{
		return -3;
	}

	CString strResultUserNo;
	tmpDM.GetCellData(_T("�����ڹ�ȣ"), 0, strResultUserNo);
	if (strResultUserNo.CompareNoCase(m_strUserNo) == 0)
	{
		return 200;
	}

	return 0;
}

INT CKlUserSyncDlg::UserSwitch()
{
	GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(_T("å����ȸ�� ����ȭ���Դϴ�."));

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K34_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), m_strLibCode);
	Writer.addElement(_T("��û��������ȣ"), m_strRequestLibCode);
	Writer.addElement(_T("�������̸�"), m_strUserName);
	Writer.addElement(_T("���մ����ڹ�ȣ"), m_strUserNo);
	Writer.addElement(_T("G-PINHASH�����ڵ�"), m_strGPinHash);
	Writer.addElement(_T("G-PIN����"), m_strGPinSex);
	Writer.addElement(_T("��ȣȭ�ֹε�Ϲ�ȣ"), m_strCryptoCivilNo);
	Writer.addElement(_T("��û��������ȣ"), m_strRequestLibCode);
	
	Writer.addElement(_T("��������"), m_strCI);

	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	INT ids;
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) return -1;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	Reader.makeDM(&tmpDM);
	if(1 > tmpDM.GetRowCount()) return 100;
	CString strValue;
	tmpDM.GetCellData(_T("ó�����"), 0, strValue);
	if(_T("OK") != strValue)
	{
		return -3;
	}

	GetDlgItem(IDC_stcKUSY_Status)->SetWindowText(_T("å����ȸ�� ����ȭ ����"));
	return 0;
}

BEGIN_EVENTSINK_MAP(CKlUserSyncDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CKlUserSyncDlg)
	ON_EVENT(CKlUserSyncDlg, IDC_gridKUSY_Lib, -601 /* DblClick */, OnDblClickgridKUSYLib, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKlUserSyncDlg::OnDblClickgridKUSYLib() 
{
	if(TRUE == m_bIsRunningThread) return;
	if(1 > m_pDM->GetRowCount()) return;
	if(1 > m_pGrid->GetMouseRow()) return;
	CString strLibCode;
	m_pDM->GetCellData(_T("���Ե�������ȣ"), m_pGrid->GetMouseRow()-1, strLibCode);
	if(FALSE == strLibCode.IsEmpty())
	{
		GetDlgItem(IDC_edtKUSY_LibCode)->SetWindowText(strLibCode);
		SetLibCodeControl(strLibCode);
	}
}

void CKlUserSyncDlg::OnbtnKUSYLibSearch() 
{
	if(TRUE == m_bIsRunningThread) return;
	SetTimer(0, 100, NULL);	
}

void CKlUserSyncDlg::OnbtnKUSYUserSearch() 
{
	if(TRUE == m_bIsRunningThread) return;
	SetTimer(5, 100, NULL);
}

void CKlUserSyncDlg::OnbtnKUSYSwitch() 
{
	if(TRUE == m_bIsRunningThread) return;
	SetTimer(6, 100, NULL);
}

void CKlUserSyncDlg::OnbtnKUSYClose() 
{
	if(TRUE == m_bIsRunningThread) return;
	OnCancel();
}
