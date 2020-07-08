// KlUserSwitchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KlUserSwitchDlg.h"
#include "LibListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKlUserSwitchDlg dialog


CKlUserSwitchDlg::CKlUserSwitchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKlUserSwitchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKlUserSwitchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLoan = NULL;
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
}
CKlUserSwitchDlg::~CKlUserSwitchDlg()
{
}

void CKlUserSwitchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKlUserSwitchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKlUserSwitchDlg, CDialog)
	//{{AFX_MSG_MAP(CKlUserSwitchDlg)
	ON_BN_CLICKED(IDC_btnKUS_LibCode, OnbtnKUSLibCode)
	ON_BN_CLICKED(IDC_btnKUS_UserSearch, OnbtnKUSUserSearch)
	ON_BN_CLICKED(IDC_btnKUS_Switch, OnbtnKUSSwitch)
	ON_BN_CLICKED(IDC_btnKUS_Close, OnbtnKUSClose)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_EN_SETFOCUS(IDC_edtKUS_LibCode, OnSetfocusedtKUSLibCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKlUserSwitchDlg message handlers

BOOL CKlUserSwitchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	CString strStatement;
	strStatement.Format(_T("ó�� �湮�ϴ� ������ ��� ������ǥ�������� ���������� �����˴ϴ�.\r\n")
			_T("������ ���������� å����ȸ�� ����� ���Ͽ� �������� ������ �����Ͻðڽ��ϱ�?"));
	GetDlgItem(IDC_edtKL_SWITCH_STATEMENT)->SetWindowText(strStatement);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKlUserSwitchDlg::OnbtnKUSLibCode() 
{
	if(TRUE == m_bIsRunningThread) return;
	LibSearch();
}

void CKlUserSwitchDlg::OnbtnKUSUserSearch() 
{
	if(TRUE == m_bIsRunningThread) return;
	SetTimer(5, 100, NULL);
}

void CKlUserSwitchDlg::OnbtnKUSSwitch() 
{
	if(TRUE == m_bIsRunningThread) return;

	if(TRUE != ((CButton*)GetDlgItem(IDC_chkKUS_Confirm))->GetCheck())
	{
		SetTimer(14, 100, NULL);
		return;
	}

	SetTimer(6, 100, NULL);
}

void CKlUserSwitchDlg::OnbtnKUSClose() 
{
	if(TRUE == m_bIsRunningThread) return;
	OnCancel();
}

BOOL CKlUserSwitchDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnKUSClose();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnKUS_LibCode)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUSLibCode();
			}
			else if(GetDlgItem(IDC_edtKUS_LibCode)->m_hWnd == pMsg->hwnd )
			{
				CString strGetData;
				GetDlgItemText(IDC_edtKUS_LibCode, strGetData);
				strGetData.Replace(_T("#"), _T(""));
				strGetData.TrimLeft(); strGetData.TrimRight();
				if(!strGetData.IsEmpty())
				{
					SetLibCodeControl(strGetData);
				}
				return TRUE;
			}
			else if(GetDlgItem(IDC_btnKUS_UserSearch)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUSUserSearch();
			}
			else if(GetDlgItem(IDC_btnKUS_Switch)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUSSwitch();
			}
			else if(GetDlgItem(IDC_btnKUS_Close)->m_hWnd == pMsg->hwnd )
			{
				OnbtnKUSClose();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CKlUserSwitchDlg::OnTimer(UINT nIDEvent) 
{
	if(5 == nIDEvent)
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
		GetDlgItem(IDC_btnKUS_UserSearch)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUS_Switch)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnKUS_Switch)->SetFocus();
		GetDlgItem(IDC_stcKUS_Status)->SetWindowText(_T("�Ϲ� �̿��ڰ� �˻��Ǿ����ϴ�.\r\n�� ��ȯ ��ư�� �����ϸ� ����ȸ������ ��ȯ�մϴ�."));
	}
	else if(8 == nIDEvent)
	{
		KillTimer(8);
		GetDlgItem(IDC_btnKUS_Switch)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUS_UserSearch)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnKUS_UserSearch)->SetFocus();
		GetDlgItem(IDC_stcKUS_Status)->SetWindowText(_T("������ ���������� �˻��� �̿��ڰ� �����ϴ�."));
	}
	else if(9 == nIDEvent)
	{
		KillTimer(9);
		GetDlgItem(IDC_btnKUS_Switch)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUS_UserSearch)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnKUS_UserSearch)->SetFocus();
		GetDlgItem(IDC_stcKUS_Status)->SetWindowText(_T("�̿��� ��ȸ�� �����Ͽ����ϴ�.\r\n�� �̿�����ȸ ��ư�� ���� ��õ��Ͻʽÿ�."));
	}
	else if(10 == nIDEvent)
	{
		KillTimer(10);
		MessageBox(_T("  å����ȸ�� ��ȯ �۾��� ��ϵǾ����ϴ�.\r\n\r\n �� ���Ϳ� ����Ǳ���� ������ �ð��� �ҿ�˴ϴ�."), _T("����ȸ����ȯ"), MB_OK|MB_ICONINFORMATION);
		OnOK();
	}
	else if(11 == nIDEvent)
	{
		KillTimer(11);
		GetDlgItem(IDC_btnKUS_Switch)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnKUS_Switch)->SetFocus();
		GetDlgItem(IDC_stcKUS_Status)->SetWindowText(_T("å����ȸ�� ��ȯ�� �����Ͽ����ϴ�.\r\n�� ��ȯ ��ư�� ���� ��õ��Ͻʽÿ�."));
	}
	else if(12 == nIDEvent)
	{
		KillTimer(12);
		GetDlgItem(IDC_btnKUS_Switch)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUS_UserSearch)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnKUS_UserSearch)->SetFocus();
		GetDlgItem(IDC_stcKUS_Status)->SetWindowText(_T("������ �������� �̿��ڴ� �̹� å����ȸ���Դϴ�."));
	}
	else if(13 == nIDEvent)
	{
		KillTimer(13);
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_edtKUS_LibCode);
		CString strGetData;
		pEdit->GetWindowText(strGetData);
		INT nLen = strGetData.GetLength();
		if(0 < nLen)
		{
			pEdit->SetSel(0, nLen);
		}
	}
	else if(14 == nIDEvent)
	{
		KillTimer(14);
		GetDlgItem(IDC_stcKUS_Status)->SetWindowText(_T("�������� ��3�������� ���� �̿����� ���Ǹ� Ȯ���Ͻʽÿ�."));
	}

	CDialog::OnTimer(nIDEvent);
}

INT CKlUserSwitchDlg::LibSearch()
{
	m_bIsRunningThread = TRUE;
	
	CLibListDlg dlg(this, m_pLoan);
	dlg.SetMode(1);
	if(IDOK == dlg.DoModal())
	{
		SetLibCodeControl(dlg.m_strLibCode, dlg.m_strLibName);
	}

	m_bIsRunningThread = FALSE;

	return 0;
}

VOID CKlUserSwitchDlg::SetLibCodeControl(CString strLibCode, CString strLibName)
{
	if(6 != strLibCode.GetLength())
	{
		Step2(6, strLibCode);
		return;
	}
	if(m_pInfo->LIB_CODE == strLibCode)
	{
		Step2(5, strLibCode);
		return;
	}
	if(TRUE == m_pLoan->IsNumericText(strLibCode))
	{
		if(strLibName.IsEmpty())
		{
			SetDlgItemText(IDC_edtKUS_LibCode, _T("�������� ��ȸ���Դϴ�."));
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
			SetDlgItemText(IDC_edtKUS_LibCode, strValue);
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

VOID CKlUserSwitchDlg::Step2(INT nValue, CString strLibcode/*=_T("")*/)
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
			strMsg = _T("������ �������� �̹� ����ȸ���Դϴ�.");
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
		GetDlgItem(IDC_stcKUS_Status)->SetWindowText(strMsg);
		GetDlgItem(IDC_btnKUS_UserSearch)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnKUS_Switch)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtKUS_LibCode)->SetWindowText(_T(""));
		GetDlgItem(IDC_edtKUS_LibCode)->SetFocus();
	}
}

HBRUSH CKlUserSwitchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcKUS_Status == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }
	else if(IDC_stcKL_USER_SWITCH == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(242,59,12));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

UINT CKlUserSwitchDlg::UserSearchThread(LPVOID pParam)
{
	CKlUserSwitchDlg* pDlg = (CKlUserSwitchDlg*)pParam;
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

UINT CKlUserSwitchDlg::UserSwitchThread(LPVOID pParam)
{
	CKlUserSwitchDlg* pDlg = (CKlUserSwitchDlg*)pParam;
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

INT CKlUserSwitchDlg::SetCenterUser()
{
	GetDlgItem(IDC_stcKUS_Status)->SetWindowText(_T("���Ϳ� �ڰ� ����ȸ�������� �����մϴ�."));
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

void CKlUserSwitchDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;

	CDialog::OnClose();
}

INT CKlUserSwitchDlg::UserSearch()
{
	GetDlgItem(IDC_stcKUS_Status)->SetWindowText(_T("�̿��� ��ȸ���Դϴ�."));

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K33_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), m_strLibCode);
	Writer.addElement(_T("�������̸�"), m_strUserName);
	Writer.addElement(_T("��ȣȭ�ֹε�Ϲ�ȣ"), m_strCryptoCivilNo);
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
	CString strKlMemberYN;
	tmpDM.GetCellData(_T("����ȸ������"), 0, strKlMemberYN);
	if(_T("Y") == strKlMemberYN)
	{
		CString strGpinHash;
		tmpDM.GetCellData(_T("G-PINHASH�����ڵ�"), 0, strGpinHash);
		CString strGpinSex;
		tmpDM.GetCellData(_T("G-PINHASH�����ڵ�"), 0, strGpinSex);
		if(strGpinHash.IsEmpty() || strGpinSex.IsEmpty() || 1 < tmpDM.GetRowCount())
		{
			return 0;
		}
		else
		{
			return 200;
		}
	}

	return 0;
}

INT CKlUserSwitchDlg::UserSwitch()
{
	GetDlgItem(IDC_stcKUS_Status)->SetWindowText(_T("å����ȸ�� ��ȯ���Դϴ�."));

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

	GetDlgItem(IDC_stcKUS_Status)->SetWindowText(_T("å����ȸ�� ��ȯ ����"));
	return 0;
}

void CKlUserSwitchDlg::OnSetfocusedtKUSLibCode() 
{
	if(TRUE == m_bIsRunningThread) return;
	SetTimer(13, 100, NULL);
}

BEGIN_EVENTSINK_MAP(CKlUserSwitchDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CKlUserSwitchDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
