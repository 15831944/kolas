// CheckinVolMgr.cpp: implementation of the CCheckinVolMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckinVolMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckinVolMgr::CCheckinVolMgr()
{
	m_pCheckinVolESLMgr = NULL;
	m_pCheckinVolResouceMgr = NULL;

	m_pParentDM = NULL;
	m_pParentGrid = NULL;
	m_nACQ_TYPE = 1; //default
	m_strACQ_YEAR = _T("");
	m_strRECEIPT_NO = _T("");
	m_bIsLightVersion = FALSE;

}

CCheckinVolMgr::~CCheckinVolMgr()
{
	// Resource Mgr
	if (m_pCheckinVolResouceMgr) 
	{
		delete m_pCheckinVolResouceMgr;
		m_pCheckinVolResouceMgr = NULL;
	}
	
	// ESL Mgr
	if (m_pCheckinVolESLMgr)
	{
		delete m_pCheckinVolESLMgr;
		m_pCheckinVolESLMgr = NULL;
	}
	
}

INT CCheckinVolMgr::SetParentInfo(CESL_DataMgr *pParentDM, 
								  CESL_Grid *pParentGrid,
								  INT nACQ_TYPE,
								  CString strACQ_YEAR,
								  CString strRECEIPT_NO,
								  BOOL bIsLightVersion)
{
	
	m_pParentDM = pParentDM;
	m_pParentGrid = pParentGrid;
	m_nACQ_TYPE = nACQ_TYPE;
	m_strACQ_YEAR = strACQ_YEAR;
	m_strRECEIPT_NO = strRECEIPT_NO;
	m_bIsLightVersion = bIsLightVersion;

	return 0;
}


INT CCheckinVolMgr::InitManager(CESL_Mgr *pParentESLMgr)
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

	// �Ǻ� üũ�� ESL Mgr ���� (new �� �Ѵ�)
	if (CreateCheckinVolESLMgr(pParentESLMgr) < 0) return -1;


	// �θ� ���� üũ -- �⺻���� ������ �Ѵ� 
	if (!m_pParentDM || !m_pParentGrid)
		return -1;
	
	
	//�θ� �׸��忡�� ���������� ���´�
	if(!m_pParentGrid) {
		AfxMessageBox(_T("�θ� �׸��带 ã�� �� �����ϴ�!"));
		return -1;
	}

	if(!m_pParentDM) {
		AfxMessageBox(_T("�θ� DM�� ã�� �� �����ϴ�!"));
		return -2;
	}

	
	INT nHead;
	m_pParentGrid->SelectMakeList();
	nHead = m_pParentGrid->SelectGetHeadPosition();
	if(nHead<0) {
		nHead = m_pParentGrid->GetIdx();
		//AfxMessageBox(_T("������ ������ �����ϴ�!"));
		//return -3;
	}
	
	//ESL dialog���� �θ� ������ �Ѱ��ش�
	if(!m_pCheckinVolESLMgr) {
		AfxMessageBox(_T("ESL dialog new fail"));
		return -4;
	}

	m_pCheckinVolESLMgr->SetParentInfo(m_pParentDM, 
									   m_pParentGrid, 
									   m_nACQ_TYPE,
									   m_strACQ_YEAR,
									   m_strRECEIPT_NO,
									   m_bIsLightVersion);
		
	return 0;
}



INT CCheckinVolMgr::CreateCheckinVolESLMgr(CESL_Mgr *pParentESLMgr)
{
	if (pParentESLMgr == NULL) return -1;

	// CheckIn Vol ESL Mgr
	if (m_pCheckinVolESLMgr)
	{
		m_pCheckinVolESLMgr->DestroyWindow();
		delete m_pCheckinVolESLMgr;
	}
	
	m_pCheckinVolESLMgr = new CCheckinVolESL(pParentESLMgr);
	

	return 0;
}

INT CCheckinVolMgr::CreateResource(CFrameWnd *pFrameWnd)
{
	
	m_pFrameWnd = pFrameWnd;

	if (m_pCheckinVolESLMgr == NULL) return -1;
	m_pCheckinVolESLMgr->pMain = pFrameWnd;


	// ���ҽ� �Ŵ��� ����
	if (m_pCheckinVolResouceMgr) delete m_pCheckinVolResouceMgr;

	m_pCheckinVolResouceMgr = new CCheckinVolResouceMgr(m_pCheckinVolESLMgr);
	m_pCheckinVolResouceMgr->CreateResource(pFrameWnd);
	
	
	// ���ҽ� �Ŵ����� �����͸� ESL���� �Ѱ��ش�. ���õ� �׸��� �������� �����ش� 
	if(m_pCheckinVolESLMgr)
	{
		m_pCheckinVolESLMgr->m_pResourceMgr = m_pCheckinVolResouceMgr;

		if(m_pCheckinVolESLMgr->m_nCurrentParentIdx >= 0)
		{
			m_pCheckinVolResouceMgr->m_dlgSpeciesInfo.m_nACQ_TYPE = m_nACQ_TYPE;
			m_pCheckinVolResouceMgr->m_dlgSpeciesInfo.m_strRECEIPT_NO = m_strRECEIPT_NO;
			m_pCheckinVolResouceMgr->m_dlgSpeciesInfo.m_strACQ_YEAR = m_strACQ_YEAR;
			m_pCheckinVolResouceMgr->m_dlgSpeciesInfo.ShowSpeciesInfo(m_pCheckinVolESLMgr->m_nCurrentParentIdx);
		}
		CString strQuery, strUseCoverImage;
		strQuery.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USECOVERIMG'"));
		m_pParentDM->GetOneValueQuery(strQuery,strUseCoverImage);
		if( strUseCoverImage.IsEmpty() || strUseCoverImage == _T("N"))
			m_pCheckinVolESLMgr->m_pResourceMgr->HideToolBarButton(_T("ǥ������"));
	}


	return 0;
}

BOOL CCheckinVolMgr::OnCmdMsg(UINT nID)
{
	if (m_pFrameWnd == NULL) return FALSE;

	return m_pCheckinVolResouceMgr->OnCmdMsg(nID);
}

BOOL CCheckinVolMgr::OnCommand(UINT nID)
{
	if (m_pFrameWnd == NULL) return FALSE;

	CString strCommandAlias;

	BOOL bReturn = m_pCheckinVolResouceMgr->OnCommand(nID, strCommandAlias);

	if (strCommandAlias.IsEmpty()) return bReturn;

	INT ids = -1;
	
	//����������
	if (strCommandAlias == _T("VIEW_IDD_CHECKIN_VOL_SPECIES"))
	{
		if (m_pCheckinVolResouceMgr->m_wndSpeciesInfo.IsWindowVisible())
			m_pCheckinVolResouceMgr->m_wndSpeciesInfo.ShowWindow(SW_HIDE);
		else
			m_pCheckinVolResouceMgr->m_wndSpeciesInfo.ShowWindow(SW_SHOW);

		m_pFrameWnd->DockControlBar(&m_pCheckinVolResouceMgr->m_wndSpeciesInfo, AFX_IDW_DOCKBAR_BOTTOM);
		m_pFrameWnd->RecalcLayout();

		ids = 0;
	}
	

	if (strCommandAlias == _T("�Լ�ó��"))
		ids = m_pCheckinVolESLMgr->CMD_CHECKIN();
	else if (strCommandAlias == _T("��ȣó��"))
		ids = m_pCheckinVolESLMgr->CMD_MISS();
	else if (strCommandAlias == _T("��ȣ�Է�"))
		ids = m_pCheckinVolESLMgr->CMD_VOL_INSERT();
	else if (strCommandAlias == _T("��ȣ����"))
		ids = m_pCheckinVolESLMgr->CMD_VOL_MODIFY();
	else if (strCommandAlias == _T("��ȣ����"))
		ids = m_pCheckinVolESLMgr->CMD_VOL_DELETE();
	else if (strCommandAlias == _T("�պ�ȣ"))
		ids = m_pCheckinVolESLMgr->CMD_VOL_MERGE();
	else if (strCommandAlias == _T("�̹���"))
		ids = m_pCheckinVolESLMgr->CMD_VOL_NON_PUBLISH();
	else if (strCommandAlias == _T("��������"))
		ids = m_pCheckinVolESLMgr->CMD_EXP_INFO();
	else if (strCommandAlias == _T("��������"))
		ids = m_pCheckinVolESLMgr->CMD_CONTENTS_INFO();
	else if (strCommandAlias == _T("���ΰ�ħ"))
		ids = m_pCheckinVolESLMgr->CMD_REFRESH();
	else if (strCommandAlias == _T("�߰�����"))
		ids = m_pCheckinVolESLMgr->CMD_MORE_EXP();
	else if (strCommandAlias == _T("����"))
		ids = m_pCheckinVolESLMgr->CMD_MORE_EXP_SETTING();
	else if (strCommandAlias == _T("������ȣ"))
		ids = m_pCheckinVolESLMgr->CMD_RECEIPT_NO();
	else if (strCommandAlias == _T("ǥ������"))
		ids = m_pCheckinVolESLMgr->CMD_COVER_INFO();
	else if (strCommandAlias == _T("����"))
		ids = m_pCheckinVolESLMgr->CMD_PREV();
	else if (strCommandAlias == _T("����"))
		ids = m_pCheckinVolESLMgr->CMD_NEXT();
	else if (strCommandAlias == _T("����"))
		ids = m_pCheckinVolESLMgr->CMD_SAVE();
	else if (strCommandAlias == _T("���"))
		ids = m_pCheckinVolESLMgr->CMD_PRINT();
	else if (strCommandAlias == _T("�Լ���Ȳ"))
		ids = m_pCheckinVolESLMgr->CMD_PROCESSPRINT();
	else if (strCommandAlias == _T("�ݱ�"))
		if(m_pFrameWnd) ::PostMessage( m_pFrameWnd->GetSafeHwnd(), WM_CLOSE, 0,0);	
			
	
	
	
	if (ids < 0)
		return FALSE;
	else
		return TRUE;
}