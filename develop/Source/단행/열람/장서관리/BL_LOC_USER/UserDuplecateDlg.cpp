#include "stdafx.h"
#include "UserDuplecateDlg.h"

#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUserDuplecateDlg::CUserDuplecateDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CUserDuplecateDlg::IDD, pParent)
{
	m_strCertifyCode = _T("");
	m_strCivilNo = _T("");
	m_strUserID = _T("");
	m_strName = _T("");
	m_strBirthDay = _T("");
	m_nMode = 0;
	m_strUserKey = _T("");
	m_bCivilNoDuplecateSkip = FALSE;
}
CUserDuplecateDlg::~CUserDuplecateDlg()
{
	
}

void CUserDuplecateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CUserDuplecateDlg)
	//}}AFX_DATA_MAP	
	
}


BEGIN_MESSAGE_MAP(CUserDuplecateDlg, CDialog)
	//{{AFX_MSG_MAP(CUserDuplecateDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnUserDuplecateNew, OnbtnUserDuplecateNew)
	ON_BN_CLICKED(IDC_btnUserDuplecateClose, OnbtnUserDuplecateClose)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CUserDuplecateDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnUserDuplecateClose();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnUserDuplecateNew)->m_hWnd == pMsg->hwnd )
			{
				OnbtnUserDuplecateNew();
			}
			else if(GetDlgItem(IDC_btnUserDuplecateClose)->m_hWnd == pMsg->hwnd )
			{
				OnbtnUserDuplecateClose();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CUserDuplecateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	if(0 > InitESL_Mgr(_T("SM_USER_DUPLECATE")))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	if(0 != m_nMode)
	{
		GetDlgItem(IDC_btnUserDuplecateNew)->SetWindowText(_T("����"));
	}
	else
	{
		GetDlgItem(IDC_btnUserDuplecateNew)->SetWindowText(_T("�ű�"));
	}

	Search();
	
	return TRUE;  
	              
}

HBRUSH CUserDuplecateDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcUserDuplecateMsg == pWnd->GetDlgCtrlID() || IDC_stcUserDuplecateMsg2 == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

void CUserDuplecateDlg::OnbtnUserDuplecateNew() 
{
	OnOK();
}

void CUserDuplecateDlg::OnbtnUserDuplecateClose() 
{
	OnCancel();
}

void CUserDuplecateDlg::Search()
{
	int ids = 0;

	CString strName, strBirthDay, strReason, strHandphone;	
	CString strRecKey, strPrevRecKey;

	CESL_DataMgr *pDM = FindDM(_T("DM_USER_DUPLECATE"));
	pDM->FreeData();
	CString strWhere = _T("");
	CString strQuery, strValue;

	CESL_DataMgr *pDMCopy = FindDM(_T("DM_USER_DUPLECATE_COPY"));
	pDMCopy->FreeData();

	if(!m_strCertifyCode.IsEmpty())
	{
		
		if(0 == m_nMode) strWhere.Format(_T("IPIN_HASH='%s'"), m_strCertifyCode);

		else strWhere.Format(_T("IPIN_HASH='%s' AND user_class <> '3' AND  REC_KEY<>%s"), m_strCertifyCode, m_strUserKey);

		ids = pDM->RefreshDataManager(strWhere);
		if(0 > ids)
		{
			SetTimer(0, 100, NULL);
			return;
		}

		for(int i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->SetCellData(_T("�Ǹ������ߺ�"), _T("Y"), i);
		}
	}

	ids = CopyDMToDM(pDM, _T("DM_USER_DUPLECATE_COPY"));
	if(0 > ids)	{
		SetTimer(0, 100, NULL);
		return;
	}

	if(!m_strUserID.IsEmpty())
	{
		if(0 == m_nMode) strWhere.Format(_T("USER_ID='%s'"), m_strUserID);
		else strWhere.Format(_T("(USER_ID='%s' AND REC_KEY<>%s)"), m_strUserID, m_strUserKey);

		ids = pDM->RefreshDataManager(strWhere);
		if(0 > ids)
		{
			SetTimer(0, 100, NULL);
			return;
		}
	}

	ids = CopyDMToDM(pDM, _T("DM_USER_DUPLECATE_COPY"));
	if(0 > ids)	{
		SetTimer(0, 100, NULL);
		return;
	}

	if(!m_strHandphone.IsEmpty())
	{
		if(0 == m_nMode) strWhere.Format(_T("NAME='%s' AND IDX_HANDPHONE = '%s'"), m_strName, m_strHandphone);
		else strWhere.Format(_T("(NAME='%s' AND IDX_HANDPHONE = '%s' AND REC_KEY<>%s)"),m_strName, m_strHandphone, m_strUserKey);
		
		ids = pDM->RefreshDataManager(strWhere);
		if(0 > ids)
		{
			SetTimer(0, 100, NULL);
			return;
		}
	}

	ids = CopyDMToDM(pDM, _T("DM_USER_DUPLECATE_COPY"));
	if(0 > ids)	{
		SetTimer(0, 100, NULL);
		return;
	}

	if(!m_strName.IsEmpty() && !m_strBirthDay.IsEmpty())
	{
		if(0 == m_nMode) strWhere.Format(_T("(NAME='%s' AND TO_CHAR(BIRTHDAY,'YYYY/MM/DD')='%s')"), m_strName, m_strBirthDay);
		else strWhere.Format(_T("(NAME='%s' AND TO_CHAR(BIRTHDAY,'YYYY/MM/DD')='%s' AND REC_KEY<>%s)"), m_strName, m_strBirthDay, m_strUserKey);

		ids = pDM->RefreshDataManager(strWhere);
		if(0 > ids)
		{
			SetTimer(0, 100, NULL);
			return;
		}
	}

	ids = CopyDMToDM(pDM, _T("DM_USER_DUPLECATE_COPY"));
	if(0 > ids)	{
		SetTimer(0, 100, NULL);
		return;
	}

	ids = pDM->CopyDM(pDMCopy);
	if(0 > ids)	{
		SetTimer(0, 100, NULL);
		return;
	}

	pDM->SORT(_T("REC_KEY"));
	for (int k = 0 ; k < pDM->GetRowCount() -1 ; k++)
	{
		if( pDM->GetCellData(_T("REC_KEY"),k) == pDM->GetCellData(_T("REC_KEY"), k+1 ) ){
			pDM->DeleteRow(k+1);
			k--;
		}
		
	}
	
	BOOL bCheck = FALSE;
	CString strGetData;
	CString strUserClass
		,strPhoneNum
		;
	for(int i = 0; i < pDM->GetRowCount(); i++)
	{
		strReason = _T("");
		pDM->GetCellData(_T("ȸ������"), i, strUserClass);
		pDM->GetCellData(_T("�Ǹ������ڵ�"), i, strGetData);
		if(!strGetData.IsEmpty())
		{
			if(strGetData == m_strCertifyCode)
			{
				strReason =_T("���������ڵ�");

				if(_T("Ż��ȸ��") != strUserClass) bCheck = TRUE;
			}
		}
		pDM->GetCellData(_T("���̵�"), i, strGetData);
		if(!strGetData.IsEmpty())
		{
			if(strGetData == m_strUserID)
			{
				if(strReason.IsEmpty())
				{
					strReason = _T("ID");
				}
				else 
				{					
					strReason += _T(", ID");
				}
				
				if(0 == m_nMode)
				{
					if(_T("Ż��ȸ��") != strUserClass) bCheck = TRUE;
				}
			}
		}
		pDM->GetCellData(_T("�̸�"), i, strName);
		pDM->GetCellData(_T("�������"), i, strBirthDay);
		if(!strName.IsEmpty() && !strBirthDay.IsEmpty())
		{
			if(10 < strBirthDay.GetLength()) strBirthDay = strBirthDay.Left(10);
			if(strName == m_strName && strBirthDay == m_strBirthDay)
			{
				if(strReason.IsEmpty())
				{
					strReason = _T("�̸�+�������");
				}
				else
				{
					strReason += _T(", �̸�+�������");
				}

			}
		}

		pDM->GetCellData(_T("�̸�"), i, strName);
		pDM->GetCellData(_T("�޴���"), i, strPhoneNum);
		if (!strName.IsEmpty() && !strPhoneNum.IsEmpty() )
		{
			strPhoneNum.Replace(_T("-"),_T(""));
			if(strName == m_strName && strPhoneNum == m_strHandphone)
			{
				if(strReason.IsEmpty())
				{
					strReason = _T("�̸�+�޴�����ȣ");
				}
				else
				{
					strReason += _T(", �̸�+�޴�����ȣ");
				}

			}
		}

		if(!strReason.IsEmpty()) strReason += _T(" ����");

		pDM->SetCellData(_T("�ߺ�����"), strReason, i);
	}

	if(TRUE == bCheck)
	{
		GetDlgItem(IDC_stcUserDuplecateMsg)->SetWindowText(_T("������ �̿��ڰ� ��ȸ�Ǿ����ϴ�."));
		GetDlgItem(IDC_stcUserDuplecateMsg2)->SetWindowText(_T("��ȸ�� �̿��ڿ��� �̹� ���������� �޾Ұų� �̸�, �������, �ڵ�����ȣ, ���̵� ������ �̿����Դϴ�."));
		GetDlgItem(IDC_btnUserDuplecateNew)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_stcUserDuplecateMsg)->SetWindowText(_T("��ȸ�� �̿��� �߿��� ������ �̿��ڰ� �ִ��� Ȯ���� �ֽʽÿ�."));
		
		if (m_nMode!=0)
		{
			GetDlgItem(IDC_stcUserDuplecateMsg2)->SetWindowText(_T("��ġ�ϴ� �̿��ڰ� ���ٸ� ���� ��ư�� ���� �̿� �� �� �ֽ��ϴ�."));
		}else
		{
			GetDlgItem(IDC_stcUserDuplecateMsg2)->SetWindowText(_T("��ġ�ϴ� �̿��ڰ� ���ٸ� �ű� ��ư�� ���� �ű԰��� �� �� �ֽ��ϴ�."));
		}
		
		GetDlgItem(IDC_btnUserDuplecateNew)->ShowWindow(TRUE);
	}


	// Display
	CESL_ControlMgr *pCM = FindCM(_T("CM_USER_DUPLECATE"));

	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
	if(1 == m_pInfo->m_nPrivacyPrintLevel)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("�ߺ��̿��� �������");
	} 

	pCM->ControlDisplay(_T("MainGrid"));
}

void CUserDuplecateDlg::OnTimer(UINT nIDEvent) 
{
	if(0 == nIDEvent)
	{
		KillTimer(0);
		MessageBox(_T("�ߺ���ȸ�� �����Ͽ����ϴ�."), _T("�ߺ���ȸ"), MB_OK|MB_ICONERROR);
	}
	
	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CUserDuplecateDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CUserDuplecateDlg)
	ON_EVENT(CUserDuplecateDlg, IDC_gridUserDuplecate, -604 /* KeyUp */, OnKeyUpgridUserDuplecate, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CUserDuplecateDlg::OnKeyUpgridUserDuplecate(short FAR* KeyCode, short Shift) 
{
	if( *KeyCode == 27 )
	{
		::PostMessage(this->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 1);
	}
}


//2020.03.19 JOB.2020.0028 add by kyj : �̿��� �ߺ����� ����
INT CUserDuplecateDlg::CopyDMToDM(CESL_DataMgr *pSourceDM, CString sDestDMAlias)
{
	EFS_BEGIN
		
		INT ids;
	
	INT nRowCount= pSourceDM->GetRowCount();
	
	CString sGetData;
	CString *sColAlias;
	INT nColCnt = pSourceDM->GetColCount();
	sColAlias = new CString[nColCnt];
	ids = pSourceDM->GetAllAlias(sColAlias, nColCnt);
	if(0 > ids) 
	{
		delete []sColAlias;
		return -1;
	}
	
	CESL_DataMgr *pDstDM = FindDM(sDestDMAlias);
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
			ids = pSourceDM->GetCellData(i, j, sGetData);
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

