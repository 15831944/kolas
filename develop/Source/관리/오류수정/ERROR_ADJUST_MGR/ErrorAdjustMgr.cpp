// ErrorAdjustMgr.cpp: implementation of the CErrorAdjustMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorAdjustMgr.h"
#include "ErrorAdjustFormView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorAdjustMgr::CErrorAdjustMgr()
{
	m_pErrorAdjustResouceMgr = NULL;
	m_pFrameWnd = NULL;
	m_pErrorAdjustDlgMgr = NULL;
}

CErrorAdjustMgr::~CErrorAdjustMgr()
{
	if(m_pErrorAdjustDlgMgr)
	{
		delete m_pErrorAdjustDlgMgr;
		m_pErrorAdjustDlgMgr = NULL;
	}
	if(m_pErrorAdjustResouceMgr)
	{
		delete m_pErrorAdjustResouceMgr;
		m_pErrorAdjustResouceMgr = NULL;
	}
}

INT CErrorAdjustMgr::CreateResource(CFrameWnd *pFrameWnd, ERROR_ADJUST_TYPE nErrorAdjustType/*=PURCHASEINFO_ERROR_ADJUST*/)
{
	
	m_pFrameWnd = pFrameWnd;
	
	if (m_pErrorAdjustDlgMgr == NULL) return -1;
	m_pErrorAdjustDlgMgr->SetMainWnd(pFrameWnd);
		
	// ���ҽ� �Ŵ��� ����
	if (m_pErrorAdjustResouceMgr) delete m_pErrorAdjustResouceMgr;
	
	m_pErrorAdjustResouceMgr = new CErrorAdjustResourceMgr(m_pErrorAdjustDlgMgr->GetErrorAdjustDlg());
	m_pErrorAdjustResouceMgr->CreateResource(pFrameWnd, nErrorAdjustType);
	
	
	// ���ҽ� �Ŵ����� �����͸� ESL���� �Ѱ��ش�. 
	if(m_pErrorAdjustDlgMgr->GetErrorAdjustDlg())
	{
		m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->SetResourceMgr(m_pErrorAdjustResouceMgr);
	}	
	
	return 0;
}

BOOL CErrorAdjustMgr::OnCmdMsg(UINT nID)
{
	if (m_pFrameWnd == NULL) return FALSE;
	
	return m_pErrorAdjustResouceMgr->OnCmdMsg(nID);
}

BOOL CErrorAdjustMgr::OnCommand(UINT nID)
{
	if (m_pFrameWnd == NULL) return FALSE;
	
	CString strCommandAlias;
	
	BOOL bReturn = m_pErrorAdjustResouceMgr->OnCommand(nID, strCommandAlias);
	
	if (strCommandAlias.IsEmpty()) return bReturn;
	
	INT ids = -1;
	
	//����������
	CErrorAdjustFormView *pView = (CErrorAdjustFormView *) m_pFrameWnd->GetActiveView();
	if (strCommandAlias == _T("PURCHASEINFO_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(PURCHASEINFO_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, PURCHASEINFO_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("VOLINDEX_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(VOLINDEX_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, VOLINDEX_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("INDEX_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(INDEX_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, INDEX_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("WASTEDATA_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(WASTEDATA_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, WASTEDATA_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("RECYCLE_DATA_RESTORE"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(RECYCLE_DATA_RESTORE);
		CreateResource(m_pFrameWnd, RECYCLE_DATA_RESTORE);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("REQUEST_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(REQUEST_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, REQUEST_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	
	else if (strCommandAlias == _T("SPECIESINFO_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(SPECIESINFO_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, SPECIESINFO_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("MANAGENO_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(MANAGENO_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, MANAGENO_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
	else if (strCommandAlias == _T("MARCDATA_ERROR_ADJUST"))
	{
		m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg(MARCDATA_ERROR_ADJUST);
		CreateResource(m_pFrameWnd, MARCDATA_ERROR_ADJUST);
		pView->CreateChildWindow();
		
		ids = 0;
	}
		
	if (strCommandAlias == _T("�˻�"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_SEARCH();
	else if (strCommandAlias == _T("������������"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("�����籸��"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("å���ε���ȭ"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("å�η��÷��׼���"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST1();
	else if (strCommandAlias == _T("�������ڷ����"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("��Ȱ���ڷẹ��"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("��ġ��������籸��"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("��������������"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("������ȣ��������"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("��ũ����"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_ERRORADJUST();
	else if (strCommandAlias == _T("���ΰ�ħ"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_REFRESH();
	else if (strCommandAlias == _T("�󼼺���"))
		ids = m_pErrorAdjustDlgMgr->GetErrorAdjustDlg()->CMD_VIEWERRORINFO();
	else if (strCommandAlias == _T("�ݱ�"))
		if(m_pFrameWnd) 
			::PostMessage( m_pFrameWnd->GetSafeHwnd(), WM_CLOSE, 0,0);	
		
	if (ids < 0)
		return FALSE;
	else
		return TRUE;
}

INT CErrorAdjustMgr::InitManager(CESL_Mgr *pParentESLMgr)
{
	// Check 
	if (pParentESLMgr == NULL) 
	{
		AfxMessageBox(_T("�θ� ESL Mgr�� NULL �Դϴ�!"));
		return -1;
	}
	if (pParentESLMgr->m_pInfo == NULL) return -1;
	if (pParentESLMgr->m_pInfo->pMarcMgr == NULL) return -1;
	
	INT ids = -1;
	
	// ErrorAdjustDlg ���� (new �� �Ѵ�)
	if (CreateErrorAdjustDlg(pParentESLMgr) < 0) return -1;
	
	return 0;
}

INT CErrorAdjustMgr::CreateErrorAdjustDlg(CESL_Mgr *pParentESLMgr)
{
	if (pParentESLMgr == NULL) return -1;
	
	// ErrorAdjustDlg
	if (m_pErrorAdjustDlgMgr)
	{
		delete m_pErrorAdjustDlgMgr;
		m_pErrorAdjustDlgMgr = NULL;
	}
	
	m_pErrorAdjustDlgMgr = new CErrorAdjustDlgMgr(pParentESLMgr);
	m_pErrorAdjustDlgMgr->CreateErrorAdjustDlg();
	return 0;
}
