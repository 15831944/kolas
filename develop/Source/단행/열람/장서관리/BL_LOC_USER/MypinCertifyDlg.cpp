#include "Stdafx.h"
#include "MypinCertifyDlg.h"
#include "CertifyApi.h"
#include "UserDuplecateDlg.h"
#include "KL_UserManagerDlg.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"
#include "..\\..\\..\\..\\����\\IndexToolKit\\Index.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MYPIN_SCREEN_BASIC	10
#define MYPIN_SCREEN_FAIL	20

CMypinCertifyDlg::CMypinCertifyDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMypinCertifyDlg::IDD, pParent)
{
	m_pDateCtrl = NULL;

	m_nUserMode = 0;
}

CMypinCertifyDlg::~CMypinCertifyDlg()
{
	if (m_pDateCtrl)
	{
		delete m_pDateCtrl;
		m_pDateCtrl = NULL;
	}
}

void CMypinCertifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CMypinCertifyDlg) 
	//}}AFX_DATA_MAP		
	
}

BEGIN_MESSAGE_MAP(CMypinCertifyDlg, CDialog)
	//{{AFX_MSG_MAP(CMypinCertifyDlg)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnCertify, OnbtnCertify)
	ON_BN_CLICKED(IDC_btnCertifyClose, OnbtnCertifyClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMypinCertifyDlg::PreTranslateMessage(MSG* pMsg) 
{	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CMypinCertifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	
	m_pDateCtrl = new CECO_ControlDateEdit(NULL);
	m_pDateCtrl->SubclassDlgItem(IDC_edtBirthDate, this);
	m_pDateCtrl->InitEdit();
	m_pDateCtrl->ECO_SetReadOnly(FALSE);

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtMypinNo);
	ASSERT(pEdit);
	pEdit->SetLimitText(13); 

	SetScreenMode(MYPIN_SCREEN_BASIC);

	
	CString strName = GetUserName();
	if (!strName.IsEmpty())
	{
		GetDlgItem(IDC_edtName)->SetWindowText(strName);
	}
	CString strBirthDate = GetBirthDate();
	if (!strBirthDate.IsEmpty())
	{
		m_pDateCtrl->SetData(strBirthDate);
	}

	CString strSex = GetCertifySex();
	if (!strSex.IsEmpty() && strSex.CompareNoCase(_T("1")) == 0)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RAD_WOMAN);
		ASSERT(pButton);
		pButton->SetCheck(TRUE);
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RAD_MAN);
		ASSERT(pButton);
		pButton->SetCheck(TRUE);
	}

	return TRUE;  
	              
}

HBRUSH CMypinCertifyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int nID = pWnd->GetDlgCtrlID();
	switch (nID)
	{
	case IDC_edtFailDesc:
		{
			pDC->SetTextColor(RGB(242,59,12));
			pDC->SetBkMode(TRANSPARENT);
		}
		break;
	case IDC_edtName:
	case IDC_edtMypinNo:
		{	
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(205, 229, 255));			
			return m_BkBrush;
		}
		break;
	}

	return hbr;
}

void CMypinCertifyDlg::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case 0:
		{
			KillTimer(0);
			OnOK();
		}
		break;
	case 1:
		{
			KillTimer(1);
			OnCancel();
		}
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

VOID CMypinCertifyDlg::SetScreenMode(INT nMode)
{
	CWnd* pWnd = NULL;
	switch (nMode)
	{
	case MYPIN_SCREEN_BASIC:
		pWnd = GetDlgItem(IDC_STATIC_BASIC);
		break;
	case MYPIN_SCREEN_FAIL:
		pWnd = GetDlgItem(IDC_STATIC_FAIL);
		break;
	default:
		ASSERT(FALSE); 
		break;
	}

	ASSERT(pWnd);

	CRect rcNew;
	pWnd->GetWindowRect(&rcNew);
	CRect rcOld;
	GetWindowRect(&rcOld); 

	ClientToScreen(&rcOld);
	ClientToScreen(&rcNew);

	rcOld.SetRect(rcOld.left, rcOld.top, rcNew.right, rcNew.bottom);

	ScreenToClient(&rcOld);
	MoveWindow(rcOld);
}

VOID CMypinCertifyDlg::OnbtnCertify()
{	
	CString strName;
	CString strMypinNo;
	CString strBirthDate;
	CString strReplaceBirthDate;

	
	GetDlgItem(IDC_edtName)->GetWindowText(strName);
	GetDlgItem(IDC_edtMypinNo)->GetWindowText(strMypinNo);
	m_pDateCtrl->GetData(strBirthDate);
	
	if (strName.IsEmpty())
	{
		SetScreenMessage(_T("�̸��� �Է��Ͻʽÿ�."));
		GetDlgItem(IDC_edtName)->SetFocus();
		return;
	}
	if (strMypinNo.IsEmpty())
	{
		SetScreenMessage(_T("My-PIN��ȣ�� �Է��Ͻʽÿ�."));
		GetDlgItem(IDC_edtMypinNo)->SetFocus();
		return;
	}

	
	strReplaceBirthDate = strBirthDate;
	strReplaceBirthDate.Replace(_T("/"), _T(""));

	CString strCommand = _T("MW_SERVICE_B02");
	CCertifyFrameWriter Writer(m_pInfo);
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("�̸�"), strName);
	Writer.addElement(_T("MYPIN��ȣ"), strMypinNo);
	Writer.addElement(_T("�������"), strReplaceBirthDate);

	CCertifyFrameReader Reader;
	CCertifyFrameSender Sender(m_pInfo);
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		CString strCode;
		strCode.Format(_T("%d"), ids);
		SetErrorMessage(0, strCode, _T(""));
		return;
	}

	CESL_DataMgr TempDM;
	TempDM.SetCONNECTION_INFO(_T(""));

	Reader.makeDM(&TempDM);

	CString strResult = Reader.getCommand();
	if (strResult.CompareNoCase(_T("ERROR")) == 0)
	{
		CString strMsg;
		TempDM.GetCellData(_T("ID"), 0, strResult);
		TempDM.GetCellData(_T("MSG"), 0, strMsg);
		SetErrorMessage(1, strResult, strMsg);
		return;
	}

	
	SetUserName(strName);
	SetBirthDate(strBirthDate);

	TempDM.GetCellData(_T("ó�����"), 0, strResult);
	if (strResult.CompareNoCase(_T("OK")) != 0)
	{
		SetErrorMessage(2, strResult, _T(""));		

		if (GetUserMode() == 1 && m_pInfo->m_bKLUse && GetKLMemeberYN() && !GetCertifyCI().IsEmpty())
		{
			
		}
		else
		{
			SetCertifyFailCode(strResult);
		}

		return;
	}

	CString strCI;
	TempDM.GetCellData(_T("�����ڵ�"), 0, strCI);
	if (strCI.IsEmpty())
	{
		SetErrorMessage(1, strResult, _T("���������ڵ� ����"));
		return;
	}
	
	SetCertifyCI(strCI);
	
	CButton* pButton = (CButton*)GetDlgItem(IDC_RAD_MAN);
	ASSERT(pButton);
	SetCertifySex(pButton->GetCheck() ? _T("0") : _T("1"));
	
	CString strCertifyDate;
	CLocCommonAPI::GetCurrentDateTime(this, strCertifyDate);
	SetCertifyDate(strCertifyDate);
	
	SetCertifyWorker(m_pInfo->USER_ID);

	if (GetUserMode() != 2)
	{
		
		if (IsDuplicateUser())
		{
			SetCertifyCI(_T(""));
			SetCertifyFailCode(_T(""));
			SetCertifyDate(_T(""));
			SetCertifyWorker(_T(""));

			
			SetTimer(1, 100, NULL);
			return;
		}

		
		if (m_pInfo->m_bKLUse)
		{
			SetKLUserNo(_T(""));
			SetKLMemeberSearch(FALSE);

			CString strUserNo;
			CKL_UserManagerDlg dlg(this);
			if (dlg.GetKlUserSearch(strCI, _T("000000-0000000"), strUserNo) == 0)
			{
				
				SetKLUserNo(strUserNo);
				SetKLMemeberSearch(TRUE);
			}
			else
			{
				SetKLMemeberSearch(FALSE);
			}
		}

		SetScreenMessage(_T("���������� �����Ͽ����ϴ�."));

		if (GetUserMode() == 1)
		{
			TempDM.StartFrame();
			TempDM.InitDBFieldData();
			TempDM.AddDBFieldData(_T("IPIN_HASH"), _T("STRING"), GetCertifyCI());
			
			
			CString strDateFormat;
			strDateFormat.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), GetCertifyDate());
			TempDM.AddDBFieldData(_T("IPIN_DATE"), _T("NUMERIC"), strDateFormat);

			SetCertifyFailCode(_T(""));
			TempDM.AddDBFieldData(_T("IPIN_RESULT"), _T("STRING"), _T(""));
			
			TempDM.AddDBFieldData(_T("CERTIFY_WORKER"), _T("STRING"), m_pInfo->USER_ID);

			TempDM.AddDBFieldData(_T("NAME"), _T("STRING"), strName);

			TempDM.AddDBFieldData(_T("GPIN_SEX"), _T("STRING"), GetCertifySex());

			CIndex index;
			index.MakeIndex(strName);
			TempDM.AddDBFieldData(_T("IDX_USER_NAME"), _T("STRING"), strName);

			if (!strBirthDate.IsEmpty())
			{
				TempDM.AddDBFieldData(_T("BIRTHDAY"), _T("STRING"), strBirthDate);
			}
			
			TempDM.AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("My-PIN����(�̿��ڼ���)"),__WFILE__,__LINE__));
			TempDM.MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), GetUserKey());
			
			ids = TempDM.SendFrame();
			if (0 > ids)
			{
				SetErrorMessage(1, strResult, _T("�������� ���� ���� ����"));
				return;
			}
			TempDM.EndFrame();
		}
				
		SetTimer(0, 1500, NULL);
	}
}

BOOL CMypinCertifyDlg::IsDuplicateUser()
{
	CString strCertifyCI = GetCertifyCI();
	if (strCertifyCI.IsEmpty())
	{
		
		return FALSE;
	}

	CString strWhere; 
	if (GetUserMode() == 0)
	{
		strWhere.Format(_T("IPIN_HASH='%s'"), strCertifyCI);		
	}
	else
	{
		CString strUserKey = GetUserKey();
		strWhere.Format(_T("(IPIN_HASH='%s' AND REC_KEY<>%s)"), strCertifyCI, strUserKey);
	}

	CString strQuery;
	strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE %s"), strWhere);

	CESL_DataMgr TempDM;
	TempDM.SetCONNECTION_INFO(_T(""));

	CString strValue;
	int ids = TempDM.GetOneValueQuery(strQuery, strValue);
	if(0 > ids) return FALSE; 
	if(0 < _ttoi(strValue))
	{
		CUserDuplecateDlg dlg(this);
		dlg.m_nMode = GetUserMode();
		dlg.m_strUserKey = GetUserKey();
		dlg.m_strCertifyCode = strCertifyCI;

		if (IDOK != dlg.DoModal())
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

VOID CMypinCertifyDlg::OnbtnCertifyClose()
{
	OnCancel();
}

VOID CMypinCertifyDlg::SetScreenMessage(CString strMessage)
{
	CWnd* pWnd = GetDlgItem(IDC_STATIC_MYPIN_MSG);
	ASSERT(pWnd);

	if (strMessage.IsEmpty())
	{
		pWnd->ShowWindow(FALSE);
	}
	else
	{
		CString strMsg;
		strMsg.Format(_T("�� %s"), strMessage);

		pWnd->ShowWindow(TRUE);
		pWnd->SetWindowText(strMsg);
	}
}

VOID CMypinCertifyDlg::SetErrorMessage(INT nMode, CString strCode, CString strMessage)
{
	SetScreenMode(MYPIN_SCREEN_FAIL);
	SetScreenMessage(_T("My-PIN������ �����Ͽ����ϴ�."));

	switch (nMode)
	{
	case 0:
		{
			CString strMsg;
			strMsg.Format(_T("�̵������� ��ſ� �����Ͽ����ϴ�.[%s]"), strCode);
			GetDlgItem(IDC_edtFailDesc)->SetWindowText(strMsg);
		}
		break;
	case 1:
		{
			GetDlgItem(IDC_edtFailDesc)->SetWindowText(strMessage);
		}
		break;
	case 2:
		{
			CString strMsg;

			
			int nIds = _ttoi(strCode);
			switch (nIds)
			{
			case 1:		strMsg = _T("I-PIN�� �߱��� �޾����� My-PIN�� ������ �����Դϴ�");	break;
			case 2:		strMsg = _T("���Ե��� ���� �̿����Դϴ�");	break;
			case 3:		strMsg = _T("I-PIN�� �����Ǿ� ����� �� ���� �����Դϴ�");	break;
			case 4:		strMsg = _T("�Է��� ����(�̸� �Ǵ� �������)�� ���� ���� �����Դϴ�");	break;
			case 5:		strMsg = _T("My-PIN�� �����ϳ� ��ȿ�Ⱓ�� ���� �����Դϴ�");	break;
			case 6:		strMsg = _T("�ý��� �����Դϴ�");	break;
			case 11:	strMsg = _T("��ſ��� �����Դϴ�");	break;
			case 12:	strMsg = _T("�ɹ�ȣ ���� �����Դϴ�");	break;
			case 13:	strMsg = _T("���� ������ �����Դϴ�");	break;
			case 14:	strMsg = _T("����Ʈ ID �����Դϴ�");	break;
			case 15:	strMsg = _T("SEED ��,��ũ��Ʈ �����Դϴ�");	break;
			case 16:	strMsg = _T("BASE64 ��,��ũ��Ʈ �����Դϴ�");	break;
			case 17:	strMsg = _T("�Է°� �����Դϴ�");	break;
			case 18:	strMsg = _T("��ϵ��� ���� IP�Դϴ�");	break;
			default:	strMsg = _T("�˼� ���� �����Դϴ�");	break;
			}
			GetDlgItem(IDC_edtFailDesc)->SetWindowText(strMsg);
		}
		break;
	}
}