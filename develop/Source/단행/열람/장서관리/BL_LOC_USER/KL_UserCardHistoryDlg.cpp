#include "stdafx.h"
#include "KL_UserCardHistoryDlg.h"
#include "FrameManager.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"
#include "KL_UserManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CKL_UserCardHistoryDlg::CKL_UserCardHistoryDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
		
	m_nMode = 0;
	m_strGpinHash = _T("");
}

CKL_UserCardHistoryDlg::~CKL_UserCardHistoryDlg()
{
}

void CKL_UserCardHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CKL_UserCardHistoryDlg) 
	//}}AFX_DATA_MAP		
	
}

BEGIN_MESSAGE_MAP(CKL_UserCardHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_UserCardHistoryDlg)
	ON_BN_CLICKED(IDC_btnRegister, OnbtnRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CKL_UserCardHistoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	EnableThemeDialogTexture(GetSafeHwnd());	
	InitESL_Mgr(_T("SM_KL_USER_CARD_HISTORY"));

	
	CString strText = m_strUserName + _T("���� å����ȸ���� �߱��̷��Դϴ�");
	SetDlgItemText(IDC_STC_USER_CARD, strText);

	if(!m_pInfo->IsConnectUnityLoanService())
	{
		SetDlgItemText(IDC_STC_USER_CARD, _T("���Ϳ� ���ӽõ� �� �Դϴ�."));
		GetDlgItem(IDC_btnRegister)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return TRUE; 
	}
	
	
	if(1 != m_nMode && 3 != m_nMode)
	{
		GetDlgItem(IDC_btnRegister)->ShowWindow(FALSE);
	}

	CFrameWriter Writer(m_pInfo);
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;

	Writer.setCommand(_T("IL_K30_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), m_strUserNo);
	
	Writer.addElement(_T("G-PINHASH�����ڵ�"), m_strGpinHash);
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);

	CESL_DataMgr* pDM = FindDM(_T("DM_KL_USER_CARD_HISTORY"));
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_KL_USER_CARD_HISTORY"), _T("�׸���"));

	Reader.makeDM(pDM);
	pGrid->Display();

	return TRUE;  
	              
}

void CKL_UserCardHistoryDlg::OnbtnRegister() 
{
	CardHistoryRegister();
}

VOID CKL_UserCardHistoryDlg::CardHistoryRegister()
{
	
	CString strTemp;
	CESL_DataMgr *pDM = FindDM(_T("DM_KL_USER_CARD_HISTORY"));
	INT nRowCnt = pDM->GetRowCount();
	for(INT i=0; i<nRowCnt; i++)
	{
		strTemp = pDM->GetCellData(_T("���մ����ڹ�ȣ"),i);
		if(m_strUserNo==strTemp)
		{
			AfxMessageBox(_T("�̹� ����Ͻ� ȸ�����Դϴ�."));
			return;
		}
	}	
	CString strReissueDate;
	CTime t = CLocCommonAPI::GetCurrentDateTime(this, strReissueDate);
		
	pDM->InsertRow(-1);
	pDM->SetCellData(_T("��������"), this->m_pInfo->LIB_NAME, nRowCnt);
	pDM->SetCellData(_T("��������ȣ"), this->m_pInfo->LIB_CODE, nRowCnt);
	pDM->SetCellData(_T("���մ����ڹ�ȣ"), m_strUserNo, nRowCnt);
	pDM->SetCellData(_T("�߱���"), strReissueDate.Left(16), nRowCnt);

	CKL_UserManagerDlg dlg(this);

	dlg.SetMode(6);
	dlg.SetUserNo(m_strUserNo);
	dlg.m_strGPIN_HASH = m_strGpinHash;
	dlg.KL_Membershipcard();

	
	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_KL_USER_CARD_HISTORY"), _T("�׸���"));
	pGrid->DisplayLine(nRowCnt);
}
