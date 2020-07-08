#include "stdafx.h"
#include "KL_SecedeDlg.h"
#include "FrameManager.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CKL_SecedeDlg::CKL_SecedeDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{	
	m_bIsSelfLib = FALSE;
	m_bIsRunningThread = FALSE;
	m_strUserNo = _T("");
	m_strGpinHash = _T("");
	m_bIsChange = FALSE;
	
	m_pDM = NULL;
	m_pGrid = NULL;
	m_strLibCodeList = _T("");
}
CKL_SecedeDlg::~CKL_SecedeDlg()
{
}

void CKL_SecedeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CKL_SecedeDlg) 
	//}}AFX_DATA_MAP	
	
}

BEGIN_MESSAGE_MAP(CKL_SecedeDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_SecedeDlg)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnRetry, OnbtnRetry)
	ON_BN_CLICKED(IDC_btnSecede, OnbtnSecede)
	ON_BN_CLICKED(IDC_btnLOC_USERCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CKL_SecedeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());

	if(0 > InitESL_Mgr(_T("SM_KL_Secede")))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	m_pDM = FindDM(_T("DM_KL_Secede"));
	if(NULL == m_pDM) return FALSE;
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_KL_Secede"), _T("grid"));
	if(NULL == m_pGrid) return FALSE;
	
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	CString strQuery = _T("SELECT DISTINCT LIB_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE IS NOT NULL");
	INT ids = TmpDM.RestructDataManager(strQuery);
	if(0 > ids) return FALSE;
	if(1 > TmpDM.GetRowCount()) return FALSE;
	CString strGetData;
	m_strLibCodeList = _T("");
	for(INT i = 0; i < TmpDM.GetRowCount(); i++)
	{
		if(0 < i)
		{
			m_strLibCodeList += _T(",");
		}
		TmpDM.GetCellData(i, 0, strGetData);
		m_strLibCodeList += _T("'") + strGetData + _T("'");
	}
	if(6 > m_strLibCodeList.GetLength()) return FALSE;
	
	if(!m_pInfo->IsConnectUnityLoanService())
	{
		SetDlgItemText(IDC_staStatus, _T("���Ϳ� ���ӽõ� �� �Դϴ�."));
		GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnSecede)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(TRUE);
		return TRUE; 
	}
	
	OnbtnRetry();

	return TRUE;
}

BOOL CKL_SecedeDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnCLOSE();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnRetry)->m_hWnd == pMsg->hwnd)
			{
				OnbtnRetry();
			}
			else if(GetDlgItem(IDC_btnSecede)->m_hWnd == pMsg->hwnd)
			{
				OnbtnSecede();
			}
			else if(GetDlgItem(IDC_btnLOC_USERCLOSE)->m_hWnd == pMsg->hwnd)
			{
				OnbtnCLOSE();
			}
			return TRUE;
		}
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

void CKL_SecedeDlg::OnbtnRetry() 
{
	if(TRUE == m_bIsRunningThread) return;
	SetTimer(0, 100, NULL);
}

void CKL_SecedeDlg::OnbtnSecede() 
{
	CString strLibCode;
	m_pGrid->SelectMakeList();
	if(0 == m_pGrid->SelectGetCount())
	{
		SetMessage(TRUE, _T("Ż��ó���� �������� �����Ͽ� �ֽʽÿ�."));
		return;
	}
	else if(1 < m_pGrid->SelectGetCount())
	{
		SetMessage(TRUE,_T("�ѹ��� �ϳ��� �������� ������ �� �ֽ��ϴ�."));
		return;
	}
	INT nIdx = m_pGrid->SelectGetIdx();
	CString strSecedeYN;
	m_pDM->GetCellData(_T("Ż�𰡴ɿ���"), nIdx, strSecedeYN);
	strSecedeYN.TrimLeft();	strSecedeYN.TrimRight();
	if(_T("O") != strSecedeYN)
	{
		SetMessage(TRUE,_T("Ż�� ������ �������� �ƴմϴ�.\r\nŻ�𰡴��� �������� ȸ�����°� ����ȸ���̰� ������Ȳ�� ������մϴ�."));
		return;
	}
	
	if(IDYES == MessageBox(_T("å���� Ż�� �� �ڰ� ȸ������ ��ȯ �Ͻðڽ��ϱ�?"), _T("��ȯó��"), MB_ICONQUESTION|MB_YESNO))
	{
		m_bIsChange = TRUE;
		SetTimer(100, 100, NULL);
		return;
	}
	

	CString strGroupCode;
	m_pDM->GetCellData(_T("�������׷��ڵ�"), nIdx, strGroupCode);
	for(INT i = 0; i < m_pDM->GetRowCount(); i++)
	{
		CString strGetData;
		m_pDM->GetCellData(_T("�������׷��ڵ�"), i, strGetData);
		if(strGroupCode == strGetData)
		{
			m_pDM->GetCellData(_T("Ż�𰡴ɿ���"), i, strGetData);
			if(_T("O") != strGetData)
			{
				SetMessage(TRUE,_T("����DB �������߿� Ż�� �������� ���� �������� �ֽ��ϴ�.\r\nŻ�𰡴��� �������� ȸ�����°� ����ȸ���̰� ������Ȳ�� ������մϴ�."));
				return;
			}
		}
	}
	if(IDYES != MessageBox(_T("Ż��ó���� �����ϼ̽��ϴ�.\r\n\r\n�� ������ ���������� Ż��ó���� �̿��ڰ�\r\n    �������� �̿��ϱ� ���ؼ��� �簡���� �ؾ߸��մϴ�.\r\n\r\n�� ������ �������� ���� ��������DB��\r\n    ����ϴ� �������鵵 Ż��ó�� �˴ϴ�.\r\n\r\n������ �������� �̿��ڸ� Ż��ó�� �Ͻðڽ��ϱ�?"), _T("Ż��ó��"), MB_ICONQUESTION|MB_YESNO))
	{
		SetMessage(TRUE, _T("Ż��ó���� �����Ǿ����ϴ�."));
		return;
	}
	
	
	m_bIsChange = FALSE;
	SetTimer(100, 100, NULL);	
}

void CKL_SecedeDlg::OnbtnCLOSE()
{
	if(TRUE == m_bIsRunningThread) return;
	OnCancel();
}

void CKL_SecedeDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;
	CDialog::OnClose();
}

void CKL_SecedeDlg::OnTimer(UINT nIDEvent) 
{
	if(0 == nIDEvent)
	{
		KillTimer(0);
		SetMessage(FALSE, _T("���� �� �������� ��ȸ�մϴ�."));
		AfxBeginThread(SearchJoinLibThread, this);
	}
	else if(100 == nIDEvent)
	{
		KillTimer(100);
		
		if(m_bIsChange)
			SetMessage(FALSE, _T("������ å���� Ż�� �� �ڰ�ȸ�� ��ȯ�� ��û�ϴ� ���Դϴ�."));
		else
			SetMessage(FALSE, _T("������ �������� å���� Ż��ó���� ��û�ϴ� ���Դϴ�."));
		
		AfxBeginThread(SecedeThread, this);
	} 	
	else if(200 == nIDEvent)
	{
		KillTimer(200);
		
		if(m_bIsChange)
			MessageBox(_T("��ȯó���� �Ϸ�Ǿ����ϴ�."), _T("��ȯó��"), MB_ICONINFORMATION|MB_OK);
		else
			MessageBox(_T("Ż��ó���� �Ϸ�Ǿ����ϴ�."), _T("Ż��ó��"), MB_ICONINFORMATION|MB_OK);
		OnCancel();
	}	
	else if(300 == nIDEvent)
	{
		KillTimer(300);
		
		if(m_bIsChange)
			SetMessage(TRUE, _T("�˼����� ������ �߻��Ͽ� ��ȯó���� �����Ͽ����ϴ�."));
		else
			SetMessage(TRUE, _T("�˼����� ������ �߻��Ͽ� Ż��ó���� �����Ͽ����ϴ�."));
	}
	else if(301 == nIDEvent)
	{
		KillTimer(301);
		
		if(m_bIsChange)
			SetMessage(TRUE, _T("��ȯó���� �����Ͽ����ϴ�.\r\n��ȯó�� ��ư�� �����Ͽ� ��õ� �Ͻʽÿ�."));
		else
			SetMessage(TRUE, _T("Ż��ó���� �����Ͽ����ϴ�.\r\nŻ��ó�� ��ư�� �����Ͽ� ��õ� �Ͻʽÿ�."));
	}
	else if(302 == nIDEvent)
	{
		KillTimer(302);
		SetMessage(TRUE, _T("�˼����� ������ �߻��Ͽ� ������ ��ȸ�� �����Ͽ����ϴ�."));
	}
	else if(303 == nIDEvent)
	{
		KillTimer(303);
		SetMessage(TRUE, _T("�̵������� ��ſ� �����Ͽ����ϴ�."));
	}
	else if(304 == nIDEvent)
	{
		KillTimer(304);
		SetMessage(TRUE, _T("���Ե����� ��ȸ�� �����Ͽ����ϴ�.\r\n��������ȸ ��ư�� �����Ͽ� ��õ� �Ͻʽÿ�."));
	}
	else if(305 == nIDEvent)
	{
		KillTimer(305);
		SetMessage(TRUE, _T("������ ���� ������ �����Ͽ����ϴ�."));
	}
	else if(306 == nIDEvent)
	{
		KillTimer(306);
		SetMessage(TRUE, _T("Ż�𰡴ɿ��� ������ �����Ͽ����ϴ�."));
	}
	else if(307 == nIDEvent)
	{
		KillTimer(307);
		GetDlgItem(IDC_btnRetry)->ShowWindow(SW_HIDE);
		SetMessage(TRUE, _T("��ȸ�� �Ϸ�Ǿ����ϴ�.\r\nŻ��ó�� ��ư�� �����ϸ� ������ �������� Ż�� ��û�� �մϴ�."));
		ResultView();
	}
	else if(308 == nIDEvent)
	{
		KillTimer(308);
		SetDlgItemText(IDC_staStatus, _T("���Ϳ� ���ӽõ� �� �Դϴ�."));
		GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnSecede)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(TRUE);
	}
	else if(309 == nIDEvent)
	{
		KillTimer(309);
		SetDlgItemText(IDC_staStatus, _T("���Ϳ� ���ӽõ� �� �Դϴ�."));
		GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnSecede)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(TRUE);
	}
	else if(310 == nIDEvent)
	{
		KillTimer(310);
		SetMessage(TRUE, _T("������ ���������� ���ͷ� ������� ���� ��Ȳ�� �����մϴ�."));
	}
	else if(400 == nIDEvent)
	{
		KillTimer(400);
		
		if(m_bIsChange)
			SetMessage(TRUE, _T("Ż��ó���� �Ϸ��Ͽ����ϴ�."));
		else
			SetMessage(TRUE, _T("��ȯó���� �Ϸ��Ͽ����ϴ�."));
		ResultView();
	}
	
	CDialog::OnTimer(nIDEvent);
}

UINT CKL_SecedeDlg::SearchJoinLibThread(LPVOID pParam)
{
EFS_BEGIN

	CKL_SecedeDlg* pDlg = (CKL_SecedeDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;
	if(!pDlg->m_pInfo->IsConnectUnityLoanService())
	{
		pDlg->SetTimer(308, 100, NULL);
		return 0; 
	}
	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->SearchJoinLib();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		pDlg->SetTimer(302, 100, NULL);
	}
	else if(1 == ids)
	{
		pDlg->SetTimer(303, 100, NULL);
	}
	else if(2 == ids)
	{
		pDlg->SetTimer(304, 100, NULL);
	}
	else if(3 == ids)
	{
		pDlg->SetTimer(305, 100, NULL);
	}
	else if(4 == ids)
	{
		pDlg->SetTimer(306, 100, NULL);
	}
	else
	{
		pDlg->SetTimer(307, 100, NULL);
	}

	return 0;

EFS_END
return -1;
}

UINT CKL_SecedeDlg::SecedeThread(LPVOID pParam)
{
EFS_BEGIN

	CKL_SecedeDlg* pDlg = (CKL_SecedeDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	if(!pDlg->m_pInfo->IsConnectUnityLoanService())
	{
		pDlg->SetTimer(309, 100, NULL);
		return 0;
	}
	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->Secede();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		pDlg->SetTimer(300, 100, NULL);
	}
	else if(1 == ids)
	{
		pDlg->SetTimer(301, 100, NULL);
	}
	else if(2 == ids)
	{
		pDlg->SetTimer(200, 100, NULL);
	}
	else if(3 == ids)
	{
		pDlg->SetTimer(310, 100, NULL);
	}
	else
	{
		pDlg->SetTimer(400, 100, NULL);
	}

	return 0;

EFS_END
return -1;
}

INT CKL_SecedeDlg::SearchJoinLib()
{
EFS_BEGIN

	INT ids;
	
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	CFrameWriter Writer(m_pInfo);
	CString strCommand = _T("IL_K48_LOC_B01_SERVICE");
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), m_strUserNo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) 
	{
		return 1;
	}
	CESL_DataMgr TempDM;
	Reader.makeDM(&TempDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		return 2;
	}
	
	ids = CopyDM(&TempDM);
	if(0 > ids) 
	{
		return 3;
	}
	
	ids = CheckSecede();
	if(0 > ids)
	{
		return 4;
	}

	return 0;
	
EFS_END
return -1;
}

INT CKL_SecedeDlg::Secede()
{
EFS_BEGIN

	INT ids;
	m_bIsSelfLib = FALSE;
	
	CString strLibCode;
	INT nIdx = m_pGrid->SelectGetIdx();
	if(0 > nIdx) return -3;
	CString strCreateLibCode;
	m_pDM->GetCellData(_T("���Ե�������ȣ"), nIdx, strCreateLibCode);
	if(strCreateLibCode.IsEmpty()) return -3;
	if(0 <= m_strLibCodeList.Find(strCreateLibCode))
	{
		m_bIsSelfLib = TRUE;
	}
	CString strSysdate;
	CTime tSysdate = CLocCommonAPI::GetCurrentDateTime(this, strSysdate);
	CString strLog = GetWorkLogMsg(_T("����ȸ��Ż��ó��"), __WFILE__, __LINE__);
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K06_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), m_strUserNo);
	Writer.addElement(_T("G-PINHASH�����ڵ�"), m_strGpinHash);
	Writer.addElement(_T("��������ȣ"), strCreateLibCode);
	Writer.addElement(_T("��û��������ȣ"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("��û��������ȣ����Ʈ"), m_strLibCodeList);
	Writer.addElement(_T("Ż������"), strSysdate);
	Writer.addElement(_T("�������۾�"), strLog);

	if(m_bIsChange)
	{
		Writer.addElement(_T("�ڰ���ȯ����"), "Y");
	}
	
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) 
	{
		return 1;
	}
	CESL_DataMgr TempDM;
	Reader.makeDM(&TempDM);
	if(1 > TempDM.GetRowCount())
	{
		return 1;
	}
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strErrorCode = Reader.getElement(0, 0);
		if(0 < strErrorCode.Find(_T("06LOCB01DT01")) || 0 < strErrorCode.Find(_T("06LOCR01DT02")))
		{
			return 3;
		}
		else
		{
			return 1;
		}
	}
	TempDM.GetCellData(_T("ó�����"), 0, strTemp);
	if(_T("OK") != strTemp)
	{
		return 1;
	}
	
	m_pDM->StartFrame();
	CString strQuery;
	strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET KL_SECEDE_DATE = TO_DATE('%s','YYYY/MM/DD HH24:MI:SS'), LAST_WORK = TO_CHAR(SYSDATE,'YYYY/MM/DD HH24:MI:SS') || ' å����Ż��' WHERE USER_NO='%s';"), strSysdate, m_strUserNo);
	m_pDM->AddFrame(strQuery);
	m_pDM->SendFrame();
	m_pDM->EndFrame();
	
	if(TRUE == m_bIsSelfLib) 
	{ 
		return 2;
	}
	else
	{ 		
		if(!m_bIsChange)
		{
			CString strGroupCode;
			m_pDM->GetCellData(_T("�������׷��ڵ�"), nIdx, strGroupCode);
			for(INT i = m_pDM->GetRowCount(); i >= 0; i--)
			{
				CString strData;
				m_pDM->GetCellData(_T("�������׷��ڵ�"), i, strData);
				if(strGroupCode == strData)
				{
					m_pDM->DeleteRow(i);
				}
			}
		}
		else
		{
			CString strGroupCode;
			m_pDM->GetCellData(_T("�������׷��ڵ�"), nIdx, strGroupCode);
			for(INT i = m_pDM->GetRowCount(); i >= 0; i--)
			{
				CString strData;
				m_pDM->GetCellData(_T("�������׷��ڵ�"), i, strData);
				if(strGroupCode == strData)
				{
					m_pDM->SetCellData(_T("ȸ������"), _T("Ż��ȸ��"), i);
					m_pDM->SetCellData(_T("Ż�𰡴ɿ���"), _T("X"), i);
				}
			}			
		}		
	}

	return 0;

EFS_END
return -1;
}

VOID CKL_SecedeDlg::SetMessage(BOOL bButtonShow, CString strMsg)
{
	SetDlgItemText(IDC_staStatus, strMsg);
	GetDlgItem(IDC_btnRetry)->EnableWindow(bButtonShow);
	GetDlgItem(IDC_btnSecede)->EnableWindow(bButtonShow);
	GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(bButtonShow);
}

INT CKL_SecedeDlg::CopyDM(CESL_DataMgr* TmpDm)
{
EFS_BEGIN

	CString strData;
	const INT nColCnt = 6;
	CString strALias[nColCnt] = {_T("���Ե�������ȣ"),_T("��������"),_T("��������å��"),_T("����������"),_T("ȸ������"),_T("�������׷��ڵ�")};
	INT nCopyCnt = TmpDm->GetRowCount();
	for(INT i = 0; i < nCopyCnt; i++)
	{
		m_pDM->InsertRow(-1);
		for(INT j = 0; j < nColCnt; j++)
		{
			TmpDm->GetCellData(strALias[j], i, strData);
			if(4 == j)
			{
				if(_T("0") == strData) 
				{
					strData = _T("����ȸ��");
				}
				else if(_T("1") == strData)
				{
					strData = _T("��������");
				}
				else if(_T("2") == strData)
				{
					strData = _T("����ȸ��");
				}
				else if(_T("3") == strData)
				{
					strData = _T("Ż��ȸ��");
				}
			}
			m_pDM->SetCellData(strALias[j], strData, i);
		}
	}
	return 0;

EFS_END
return -1;
}

INT CKL_SecedeDlg::CheckSecede()
{
EFS_BEGIN

	CString strData;
	const INT nColCnt = 4;
	CString strALias[nColCnt] = {_T("��������å��"),_T("��������å��"),_T("����������"),_T("ȸ������")};
	INT nCnt = m_pDM->GetRowCount();
	for(INT i = 0; i < nCnt; i++)
	{
		CString strCheck = _T("O");
		m_pDM->GetCellData(_T("��������å��"), i, strData);
		if(0 < _ttoi(strData))
		{
			strCheck = _T("X");
		}
		m_pDM->GetCellData(_T("��������å��"), i, strData);
		if(0 < _ttoi(strData))
		{
			strCheck = _T("X");
		}
		m_pDM->GetCellData(_T("����������"), i, strData);
		if(FALSE == strData.IsEmpty())
		{
			strCheck = _T("X");
		}
		m_pDM->GetCellData(_T("ȸ������"), i, strData);
		if(_T("����ȸ��") != strData)
		{
			strCheck = _T("X");
		}
		m_pDM->SetCellData(_T("Ż�𰡴ɿ���"), strCheck, i);
	}

	return 0;

EFS_END
return -1;
}

VOID CKL_SecedeDlg::ResultView()
{
	if(NULL != m_pGrid)
	{
		m_pGrid->Display();
	}
}

HBRUSH CKL_SecedeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_staStatus == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

BEGIN_EVENTSINK_MAP(CKL_SecedeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CKL_SecedeDlg)
	ON_EVENT(CKL_SecedeDlg, IDC_gridSecede, -604 /* KeyUp */, OnKeyUpgridSecede, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKL_SecedeDlg::OnKeyUpgridSecede(short FAR* KeyCode, short Shift) 
{
	if( *KeyCode == 27 )
	{
		::PostMessage(this->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 1);
	}
}