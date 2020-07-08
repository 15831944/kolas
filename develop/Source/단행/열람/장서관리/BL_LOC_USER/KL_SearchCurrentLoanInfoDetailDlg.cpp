#include "stdafx.h"
#include "KL_SearchCurrentLoanInfoDetailDlg.h"
#include "FrameManager.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CKL_SearchCurrentLoanInfoDetailDlg::CKL_SearchCurrentLoanInfoDetailDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_bIsRunningThread = FALSE;	
	m_strUserNo = _T("");
	m_strLibCode = _T("");
}
CKL_SearchCurrentLoanInfoDetailDlg::~CKL_SearchCurrentLoanInfoDetailDlg()
{
}

void CKL_SearchCurrentLoanInfoDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
			
	//{{AFX_DATA_MAP(CKL_SearchCurrentLoanInfoDetailDlg) 
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKL_SearchCurrentLoanInfoDetailDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_SearchCurrentLoanInfoDetailDlg)
	ON_BN_CLICKED(IDC_btnLOC_USERCLOSE, OnbtnCLOSE)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnRetry, OnbtnRetry)
	ON_MESSAGE(WM_RESULT_VIEW,ResultView)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CKL_SearchCurrentLoanInfoDetailDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());
					
	if(0>InitESL_Mgr(_T("SM_KL_SearchCurrentLoanInfoDetail")))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	CESL_Grid*	pGrid	=	(CESL_Grid*)GetDlgItem( IDC_gridCurrentLoanInfoDetail );
	INT nReserveDateIdx, nReserveExpireDateIdx;
	nReserveDateIdx = -1;
	nReserveExpireDateIdx = -1;
	CLocCommonAPI::GetGridColumnIndexs( this , pGrid , _T("������") , nReserveDateIdx );
	CLocCommonAPI::GetGridColumnIndexs( this , pGrid , _T("���ุ����") , nReserveExpireDateIdx );
	pGrid->SetColWidth( nReserveDateIdx+1 , 0 , 0 );
	pGrid->SetColWidth( nReserveExpireDateIdx+1 , 0 , 0 );

	
	if(!m_pInfo->IsConnectUnityLoanService())
	{
		SetDlgItemText(IDC_staStatus, _T("���Ϳ� ���ӽõ� �� �Դϴ�."));
		GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnDetailView)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(TRUE);
		return TRUE; 
	}

	SetTimer(0, 100, NULL);
	::SetFocus(this->GetSafeHwnd());

	return TRUE;  
	              
}

BOOL CKL_SearchCurrentLoanInfoDetailDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN )
		{
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE )
		{
			if(TRUE == m_bIsRunningThread) return TRUE;
		}
	}	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

void CKL_SearchCurrentLoanInfoDetailDlg::OnbtnRetry() 
{
	SetTimer(0, 100, NULL);
	::SetFocus(this->GetSafeHwnd());
}

void CKL_SearchCurrentLoanInfoDetailDlg::OnbtnCLOSE() 
{
	if(TRUE == m_bIsRunningThread) return;

	CDialog::OnCancel();
}

void CKL_SearchCurrentLoanInfoDetailDlg::OnClose()
{
	if(TRUE == m_bIsRunningThread) return;
	
	CDialog::OnClose();
}


VOID CKL_SearchCurrentLoanInfoDetailDlg::SetInfo(CString strUserNo, CString strLibCode) 
{
	m_strUserNo = strUserNo;
	m_strLibCode = strLibCode;
}

void CKL_SearchCurrentLoanInfoDetailDlg::OnTimer(UINT nIDEvent) 
{
	if(0==nIDEvent)
	{
		KillTimer(nIDEvent);
		AfxBeginThread(SearchLoanInfoDetailThread, this);
	}
	
	CDialog::OnTimer(nIDEvent);
}

UINT CKL_SearchCurrentLoanInfoDetailDlg::SearchLoanInfoDetailThread(LPVOID pParam)
{
	CKL_SearchCurrentLoanInfoDetailDlg* pDlg = (CKL_SearchCurrentLoanInfoDetailDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	if(!pDlg->m_pInfo->IsConnectUnityLoanService())
	{
		pDlg->SetDlgItemText(IDC_staStatus, _T("���Ϳ� ���ӽõ� �� �Դϴ�."));
		pDlg->GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_btnDetailView)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(TRUE);
		return TRUE; 
	}

	pDlg->m_bIsRunningThread = TRUE;

	INT ids = pDlg->StartSearch();
	if(0==ids) pDlg->GetDlgItem(IDC_btnRetry)->ShowWindow(SW_HIDE);
	else pDlg->GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);

	pDlg->m_bIsRunningThread = FALSE;

	return 0;
}

INT CKL_SearchCurrentLoanInfoDetailDlg::StartSearch()
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("StartSearch");
	SetMessage(FALSE, _T("���� ��Ȳ�� ��ȸ�մϴ�."));

	
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K04_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), m_strUserNo);
	Writer.addElement(_T("��������ȣ"), m_strLibCode);
	Writer.addElement(_T("��ü�˻�"), _T("Y"));

	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0>ids)
	{
		CString strMsg;
		strMsg.Format(_T("�̵������� ��ſ� �����Ͽ����ϴ�.[Error:%d]"),ids);
		SetMessage(TRUE, strMsg);
		return -1030;
	}
	
	CESL_DataMgr TempDM;
	Reader.makeDM(&TempDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR")==strTemp)
	{
		CString strMsg,strTemp;
		TempDM.GetCellData(0, 0, strTemp);
		strMsg.Format(_T("Error�� �߻��Ͽ����ϴ�. [%s]"),strTemp);
		SetMessage(TRUE, strMsg);
		return -1040;
	}
	SetMessage(FALSE, _T("���� ��Ȳ�� �ҷ����� ���Դϴ�."));

	
	ids = CopyDM(&TempDM);
	if(0>ids) 
	{
		CString strMsg;
		strMsg.Format(_T("Error�� �߻��Ͽ����ϴ�."));
		SetMessage(FALSE, strMsg);
		return -1040;
	}

	
	this->SendMessage(WM_RESULT_VIEW, 0, 0);
	SetMessage(TRUE, _T(""));

	return 0;

EFS_END
return -1;
}

VOID CKL_SearchCurrentLoanInfoDetailDlg::SetMessage(BOOL bButtonShow, CString strMsg)
{
	SetDlgItemText(IDC_staStatus, strMsg);
	GetDlgItem(IDC_btnRetry)->EnableWindow(bButtonShow);
	GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(bButtonShow);
}

INT CKL_SearchCurrentLoanInfoDetailDlg::CopyDM(CESL_DataMgr* TmpDm)
{
EFS_BEGIN

	INT ids;
	CString strData;
	
	const INT nColCnt = 14;
	CString strALias[nColCnt] =
	{
		_T("���⵵����"),	_T("��Ϲ�ȣ"),		_T("����"),			_T("û����ȣ"),		_T("�����"),
		_T("������"),		_T("�ݳ�������"),	_T("�ݳ�������"),	_T("������"),		_T("���ุ����"),
		_T("����"),			_T("����"),			_T("������"),		_T("Ʈ������ڵ�")
	};

	CESL_DataMgr* pDM = FindDM(_T("DM_KL_SearchCurrentLoanInfoDetail"));
	if(NULL==pDM) return -1010;

	pDM->FreeData();

	INT nCopyCnt = TmpDm->GetRowCount();
	for(INT i=0; i<nCopyCnt; i++)
	{
		pDM->InsertRow(-1);
		
		CString strTranCode;
		TmpDm->GetCellData( strALias[13] , i , strTranCode );

		for( INT j=0; j<nColCnt; j++ )
		{
			ids = TmpDm->GetCellData( strALias[j] , i , strData );
			
			
			ids = TmpDm->GetCellData( strALias[j] , i , strData );			
			
			if(10==j)
			{
				if(strTranCode.IsEmpty())
				{
					if("0"==strData)
					{
						strData = "����";
					}
					else if("2"==strData)
					{
						strData = "�ݳ�����";
					}
					
					else if("L"==strData)
					{
						strData = "å�η�����";
					}
					else if("D"==strData)
					{
						strData = "å�η��ݳ�����";
					}
					else
					{
						strData = "�˼�����";
					}
				}
				else
				{
					if("0"==strTranCode)
					{
						strData = "��û";
					}
					else if("1"==strTranCode)
					{
						strData = "�߼�";
					}
					else if("3"==strTranCode)
					{
						strData = "�Լ�";
					}
					else if("4"==strTranCode)
					{
						strData = "Ÿ������";
					}
					else
					{
						strData = "�˼�����";
					}
				}
			}


			ids = pDM->SetCellData( strALias[j] , strData , i );
			if(0>ids) return -1030;
		}
	}

	return 0;

EFS_END
return -1;
}

VOID CKL_SearchCurrentLoanInfoDetailDlg::ResultView()
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_KL_SearchCurrentLoanInfoDetail"), _T("grid"));
	pGrid->Display();
}
