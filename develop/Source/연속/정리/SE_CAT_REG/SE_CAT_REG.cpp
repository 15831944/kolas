// SE_CAT_REG.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE SE_CAT_REGDLL = { NULL, NULL };
static INT OpenMode;

#include "SE_CAT_SEARCH_MGR.h"
#include "SE_CAT_SEARCH_MGR_GROUP.h"
#include "SE_CAT_REG_MAIN.h"
#include "SE_CAT_PREGROUP_MAIN.h"
#include "SE_CAT_GROUP_MAIN.h"
#include "SE_CAT_CODE_CHANGE.h"
#include "SE_CAT_REG_0400.h"
#include "..\..\..\����\����\BO_CAT_REG\BO_CAT_REG_0500.h"
#include "SE_CAT_GROUP_INSERT.h"
#include "SE_preGROUP_MNG.h"
#include "SE_CAT_SEARCH_MGR_PRE.h"
#include "SE_CAT_REG_VIEW.h"
#include "BO_CAT_REG_0800.h"
#include "MultiSortDlg.h"

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SE_CAT_REG.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SE_CAT_REGDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  _tremove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(SE_CAT_REGDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SE_CAT_REG.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SE_CAT_REGDLL);
	}
	return 1;   // ok
}

CString g_strMenuAlias;
__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias) 
{
	g_strMenuAlias = strMenuAlias;

	//����ڷ���� 
	if ( strMenuAlias == _T("2410") )
	{
		CSE_CAT_REG_MAIN*	m_pWindow = new CSE_CAT_REG_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);	
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();

		OpenMode =  0;
		return m_pWindow;
	}

	//�����ΰ���
	/*
	if ( strMenuAlias == _T("2420") )
	{
		CSE_CAT_PREGROUP_MAIN*	m_pWindow = new CSE_CAT_PREGROUP_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();

		OpenMode =  1;
		return m_pWindow;
	}
	*/

	//���δ������
	if ( strMenuAlias == _T("2420") )
	{
		CSE_CAT_GROUP_MAIN*	m_pWindow = new CSE_CAT_GROUP_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();

		OpenMode =  2;
		return m_pWindow;
	}
	return NULL;
}

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow) 
{
	if (pWindow) 
	{
		pWindow->MoveWindow(rect);
	}
}
	
__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)
{
	if (pWindow) 
	{
		delete pWindow;
	}
}
__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow )
{
	if (!pWindow) return NULL;
	CSE_CAT_REG_MAIN* m_pWindow = (CSE_CAT_REG_MAIN*)pWindow;

	if (strSM == _T("�˻�"))
	{
		CSE_CAT_SEARCH_MGR *pDlg = NULL;
		pDlg = new CSE_CAT_SEARCH_MGR(m_pWindow);
		pDlg->OpenMode = 0;

		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		if (pDlg->Create(m_pWindow))
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}
	if (strSM == _T("����_�������ڷ�˻�"))
	{
		CSE_CAT_SEARCH_MGR_PRE *pDlg = NULL;
		pDlg = new CSE_CAT_SEARCH_MGR_PRE(m_pWindow);

		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		if (pDlg->Create(m_pWindow))
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}
	else if (strSM == _T("����_���δ������_�˻�"))
	{
		CSE_CAT_SEARCH_MGR_GROUP *pDlg = NULL;
		pDlg = new CSE_CAT_SEARCH_MGR_GROUP(m_pWindow);

		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		if (pDlg->Create(m_pWindow))
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}
	else if (strSM == _T("��ȣ�ο�"))
	{
		m_pWindow->CreateRegNO();
	}
	else if (strSM == _T("��ϱ��к���"))
	{
		CSE_CAT_CODE_CHANGE Dlg(m_pWindow);
		Dlg.DoModal();
	}
	else if (strSM == _T("��ȣ���"))
	{
		// 040309_HSS _tmain �Լ� �̿�
		m_pWindow->DeleteRegNo();
		/*
		CSE_CAT_REG_0400 *pDlg = NULL;
		pDlg = new CSE_CAT_REG_0400(m_pWindow);
		pDlg->OpenMode =0;
		pDlg->CancelRegNo(); 
		*/
	}
	// 040323_HSS å���� �߰�
	else if(strSM == _T("�ڷ����"))
	{
		m_pWindow->DeleteBook();
	}
	else if (strSM == _T("�������ۼ�"))
	{
		
		CSE_CAT_GROUP_INSERT Dlg(m_pWindow);
		Dlg.OpenMode = 0;
		Dlg.DoModal();
		
	}
	else if (strSM == _T("����_�������ڷ��߰�"))
	{
		CSE_CAT_PREGROUP_MAIN*	preWindow = (CSE_CAT_PREGROUP_MAIN*)pWindow;		
		CSE_CAT_GROUP_INSERT Dlg(preWindow);
		Dlg.OpenMode = 1;
		Dlg.DoModal();
		
	}
	else if (strSM == _T("�������ó��"))
	{
		
		m_pWindow->DoCatalog();
			
	}
	else if (strSM == _T("����_�������ϰ����"))
	{
		CSE_CAT_PREGROUP_MAIN*	preWindow = (CSE_CAT_PREGROUP_MAIN*)pWindow;
		preWindow->GiveAllRegNo();
	}
	else if (strSM == _T("����_�������ڷẹ��"))
	{
		
		CSE_CAT_PREGROUP_MAIN*	preWindow = (CSE_CAT_PREGROUP_MAIN*)pWindow;		
		preWindow->BackToRegData();
		
	}
	else if (strSM == _T("����_�����ΰ���"))
	{
		
		CSE_preGROUP_MNG Dlg(m_pWindow);
		Dlg.DoModal();
		
	}
	else if (strSM == _T("����_�����ڷ��߰�"))
	{
		
		CSE_CAT_GROUP_INSERT Dlg(m_pWindow);
		Dlg.OpenMode = 2;
		Dlg.DoModal();
		
	}
	else if (strSM == _T("����_�ڷẹ��"))
	{
		CSE_CAT_GROUP_MAIN*	preWindow = (CSE_CAT_GROUP_MAIN*)pWindow;		
		
		CSE_CAT_REG_0400 *pDlg = NULL;
		pDlg = new CSE_CAT_REG_0400(preWindow);
		pDlg->OpenMode =2;
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		if (pDlg->Create(m_pWindow) && pDlg->m_bInitDlg)
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}		
	}
	else if (strSM == _T("����_���ΰ���"))
	{
		CSE_CAT_GROUP_MAIN*	preWindow = (CSE_CAT_GROUP_MAIN*)pWindow;
		
		CBO_CAT_REG_0500 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0500(preWindow);
		pDlg->pMain = m_pWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)preWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = m_pWindow->m_pInfo;
		if (pDlg->Create(m_pWindow))
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}		
		
	}
	else if (strSM == _T("�ڷ����"))
	{
		
		CSE_CAT_REG_VIEW Dlg(m_pWindow);
		Dlg.DoModal();
		
	}
	else if (strSM == _T("����_�������"))
	{
		CBO_CAT_REG_0800 Dlg(m_pWindow);
		Dlg.DoModal();

	}
	else if (strSM == _T("���ӽǽð�ó��"))
	{
		
		m_pWindow->DoRealTimeShelf();
			
	}
	else if (strSM == _T("��������ó��"))
	{
		
		m_pWindow->DoBinding();
			
	}
	else if (strSM == _T("������"))
	{
		CESL_DataMgr *pDM = m_pWindow->FindDM(_T("DM_����_����_���_����"));
		if (pDM == NULL) return NULL;

		CSIReportManager SIReportManager(m_pWindow);
		SIReportManager.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

		INT ids = SIReportManager.GetSIReportFileFromDB(_T("���_�˻����_����"));
		if (ids < 0) {
			AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
			return NULL;
		}

		SIReportManager.SetDataMgr(0, pDM, 0);
		
		ids = SIReportManager.Print();
	}
	else if (strSM == _T("����"))
	{

		m_pWindow->DoSort();

		/*
		CString strDMAlias;
		CString strCMAlias;
		if (g_strMenuAlias == _T("����_���_����ڷ����"))
		{
			strDMAlias = _T("DM_����_����_���_����");
			strCMAlias = _T("CM_����_����_���_����");
		}
		else
		{
			strDMAlias = _T("DM_����_����_������_����");
			strCMAlias = _T("CM_����_����_������_����");
		}

		CESL_DataMgr *pDM = static_cast<CESL_Mgr*>(pWindow)->FindDM(strDMAlias);
		if (pDM == NULL) return NULL;

		CMultiSortDlg dlg(pWindow);
		dlg.AddSortInfo(_T("IBS_��ǥ��"), _T("��ǥ��"));
		dlg.AddSortInfo(_T("IBS_������"), _T("����"));
		dlg.AddSortInfo(_T("IBS_������"), _T("������"));
		dlg.AddSortInfo(_T("BB_������ȣ"), _T("������ȣ"));
		if (dlg.DoModal() == IDOK)
		{
			pDM->SORT(dlg.m_nSortCount, dlg.m_arrStrSortAlias, dlg.m_arrIntSortCond, ESL_DATAMGR_SORT_ASC);

			static_cast<CESL_Mgr*>(pWindow)->AllControlDisplay(strCMAlias);
		}
		*/
	}

	return NULL;
}

