// BO_CAT_REG.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_CAT_REG_MAIN.h"
#include "BO_CAT_SEARCH.h"
#include "BO_CAT_SEARCH_MGR_PRE.h"
#include "BO_CAT_SEARCH_MGR_GROUP.h"
#include "BO_CAT_REG_0400.h"
#include "BO_CAT_REG_0410.h"
#include "BO_CAT_REG_0420.h"
#include "BO_REG_CODE.h"
#include "BO_CAT_PREGROUP_MAIN.h"
#include "BO_CAT_GROUP_MAIN.h"
#include "BO_CAT_GROUP_INSERT.h"
#include "BO_CAT_REG_0500.h"
#include "BO_preGROUP_MNG.h"
#include "BO_CAT_PROC_1100.h"
#include "BO_CAT_REG_0800.h"
#include "MultiSortDlg.h"

static AFX_EXTENSION_MODULE BO_CAT_REGDLL = { NULL, NULL };

static INT OpenMode;

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_CAT_REG.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_CAT_REGDLL, hInstance))
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

		new CDynLinkLibrary(BO_CAT_REGDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_CAT_REG.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_CAT_REGDLL);
	}
	return 1;   // ok
}

CString g_strMenuAlias;

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias) 
{

	g_strMenuAlias = strMenuAlias;

	//����ڷ���� 
	if ( strMenuAlias == _T("1410") ) 
	{
		CBO_CAT_REG_MAIN*	m_pWindow = new CBO_CAT_REG_MAIN(NULL);
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
	if ( strMenuAlias == _T("����_���_�������ۼ�") )
	{
		CBO_CAT_PREGROUP_MAIN*	m_pWindow = new CBO_CAT_PREGROUP_MAIN(NULL);
		m_pWindow->VIEW_STYLE = VIEW_STYLE;
		m_pWindow->pMain = pParent;
		m_pWindow->m_pInfo = pInfo;
		m_pWindow->Create(pParent);
		m_pWindow->ShowWindow(SW_SHOW);
		m_pWindow->UpdateWindow();

		OpenMode =  1;

		return m_pWindow;
	}

	//���δ������
	if ( strMenuAlias == _T("1420") )
	{
		CBO_CAT_GROUP_MAIN*	m_pWindow = new CBO_CAT_GROUP_MAIN(NULL);
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
	CBO_CAT_REG_MAIN* m_pWindow = (CBO_CAT_REG_MAIN*)pWindow;


	if (strSM == _T("�˻�"))
	{
		m_pWindow->ShowSearchDlg();
	}
	if (strSM == _T("�������ڷ�˻�"))
	{
	
		CBO_CAT_PREGROUP_MAIN*	preWindow = (CBO_CAT_PREGROUP_MAIN*)pWindow;		

		CBO_CAT_SEARCH_MGR_PRE *pDlg = NULL;
		pDlg = new CBO_CAT_SEARCH_MGR_PRE(preWindow);
	
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

	else if (strSM == _T("���δ������_�˻�"))
	{
		CBO_CAT_SEARCH_MGR_GROUP *pDlg = NULL;
		pDlg = new CBO_CAT_SEARCH_MGR_GROUP(m_pWindow);
		pDlg->OpenMode = 2;

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
	else if (strSM == _T("����ڷ��Է�"))
	{
		m_pWindow->InputBook();

	}
	else if (strSM == _T("����ڷ����"))
	{
		m_pWindow->ModifyBook(SUPDATE);

	}
	else if (strSM == _T("����ڷ����"))
	{
		m_pWindow->DeleteBook();

	}

	else if (strSM == _T("��ȣ�ο�"))
	{
		m_pWindow->CreateRegNO();
	}
	else if (strSM == _T("��ȣ���"))
	{
//		INT Type = 0;
		CBO_CAT_REG_0420 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0420(m_pWindow);
		pDlg->OperationType = 0; 
		pDlg->DoModal();
		return NULL;

//		pDlg->CancelRegNo(); 
/*		
		CBO_CAT_REG_0400 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0400(m_pWindow);
		pDlg->OpenMode =0;
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
*/
	}

// '����ΰ����' ��ư �߰� [2003/10/17/������/]
	else if (strSM == _T("����ΰ����"))
	{
//		INT Type = 1;
		CBO_CAT_REG_0420 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0420(m_pWindow);
		pDlg->OperationType = 1; 
		pDlg->DoModal();
				
//		pDlg->CancelRegNo(); 
	}

// '�˼����' ��ư �߰� [2003/09/30/������] 
	else if (strSM == _T("�˼����"))
	{
//		INT Type = 1;
		CBO_CAT_REG_0420 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0420(m_pWindow);
//		pDlg->CancelType =1;
		pDlg->OperationType = 2; 
		pDlg->DoModal();
		
//		pDlg->CancelRegNo();
/*
		CBO_CAT_REG_0400 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0400(m_pWindow);
		pDlg->OpenMode =0;
		pDlg->CancelType =1; 
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
*/
	}

	else if (strSM == _T("�ڷẹ��"))
	{
		CBO_CAT_GROUP_MAIN*	preWindow = (CBO_CAT_GROUP_MAIN*)pWindow;		
		
		CBO_CAT_REG_0400 *pDlg = NULL;
		pDlg = new CBO_CAT_REG_0400(preWindow);
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

	else if (strSM == _T("��ϱ��к���"))
	{
/*		
		CBO_REG_CODE Dlg(m_pWindow);
		Dlg.DoModal();
*/	
//�������߰�===================================================================		
		CString strDMAlias;
		CString strCMAlias;
		CString strGridAlias;
		//==========================================================
		//20080603 UPDATE BY PJW : �ڵ庯�� ���̾�α� �ߵ��� ���� 
		//if (g_strMenuAlias == _T("����_���_����ڷ����"))
		if ( g_strMenuAlias == _T("1410") ) 
		//=====================================================
		{
			strDMAlias = _T("DM_����_����_���_����");
			strCMAlias = _T("CM_����_����_���_����");
			strGridAlias = _T("MAIN_GRID");
		}
		CESL_DataMgr *pDM = static_cast<CESL_Mgr*>(pWindow)->FindDM(strDMAlias);
		if (pDM == NULL) return NULL;
		CESL_ControlMgr * pCM = static_cast<CESL_Mgr*>(pWindow)->FindCM(strCMAlias);
		if (pCM == NULL) return NULL;
		CESL_Grid * pGrid = (CESL_Grid*)pCM->FindControl(strGridAlias);
		m_pWindow->CodeChange(pCM, pDM, pGrid);
//�������߰�===================================================================
	}
	else if (strSM == _T("�������ۼ�"))
	{
		
		CBO_CAT_GROUP_INSERT Dlg(m_pWindow);
		Dlg.OpenMode = 0;
		Dlg.DoModal();
		
	}
	else if (strSM == _T("�������ڷ��߰�"))
	{
		CBO_CAT_PREGROUP_MAIN*	preWindow = (CBO_CAT_PREGROUP_MAIN*)pWindow;		
		CBO_CAT_GROUP_INSERT Dlg(preWindow);
		Dlg.OpenMode = 1;
		Dlg.DoModal();
		
	}
	else if (strSM == _T("�������ڷẹ��"))
	{
		
		CBO_CAT_PREGROUP_MAIN*	preWindow = (CBO_CAT_PREGROUP_MAIN*)pWindow;		
		preWindow->BackToRegData();
		
	}
	else if (strSM == _T("�����ڷ��߰�"))
	{
		
		CBO_CAT_GROUP_INSERT Dlg(m_pWindow);
		Dlg.OpenMode = 2;
		Dlg.DoModal();
		
	}
	else if (strSM == _T("�����ΰ���"))
	{
		
		CBO_preGROUP_MNG Dlg(m_pWindow);
		Dlg.DoModal();
		
	}
	else if (strSM == _T("�������ϰ����"))
	{
		CBO_CAT_PREGROUP_MAIN*	preWindow = (CBO_CAT_PREGROUP_MAIN*)pWindow;
		preWindow->GiveAllRegNo();
	}
	else if (strSM == _T("���ΰ���"))
	{
		CBO_CAT_GROUP_MAIN*	preWindow = (CBO_CAT_GROUP_MAIN*)pWindow;

		
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
	else if (strSM == _T("�������ó��"))
	{
		
		m_pWindow->DoFindCopy();
			
	}
	else if (strSM == _T("�з����ó��"))
	{
		
		m_pWindow->DoClassify();
			
	}
	else if (strSM == _T("�������ó��"))
	{
		
		m_pWindow->DoCatalog();
			
	}
	else if (strSM == _T("�ڷ����"))
	{
		
		m_pWindow->FileImport();
			
	}
	else if (strSM == _T("�ڷ����"))
	{

		CBO_CAT_PROC_1100 *pDlg = new CBO_CAT_PROC_1100(m_pWindow);
		pDlg->MakeFileSelected( _T("CM_����_����_���_����"), _T("MAIN_GRID") );
	}
	else if (strSM == _T("�������"))
	{
		CBO_CAT_REG_0800 Dlg(m_pWindow);
		Dlg.DoModal();

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
//�������߰�===================================================================
		CString strDMAlias;
		CString strCMAlias;
		//=====================================================
		//20080603 UPDATE BY PJW : �ڵ庯�� ���̾�α� �ߵ��� ���� 
		//if (g_strMenuAlias == _T("����_���_����ڷ����"))
		if ( g_strMenuAlias == _T("1410") ) 
		//=====================================================
		{
			strDMAlias = _T("DM_����_����_���_����");
			strCMAlias = _T("CM_����_����_���_����");
			m_pWindow->ShowSortDlg();	
		}
		else
		{
			strDMAlias = _T("DM_����_����_������_����");
			strCMAlias = _T("CM_����_����_������_����");
			CBO_CAT_PREGROUP_MAIN*	preWindow = (CBO_CAT_PREGROUP_MAIN*)pWindow;
			preWindow->ShowSortDlg();
		}
//�������߰�===================================================================		
	}
	else if(strButtonAlias == _T("�����׷����"))
	{
		m_pWindow->ShowGroupMgrFrame();
	}
	else if(strButtonAlias == _T("��������"))
	{
		m_pWindow->ChangeAcqInfo();
	}
	else if(strButtonAlias == _T("��������"))
	{
		m_pWindow->ShowChangeDonateInfoDlg();
	}
	else if(strButtonAlias == _T("��ũ��ü"))
	{
		m_pWindow->ChangeMarc();		
	}
	return NULL;
}


__declspec(dllexport) void KalendarSearch(CString WorkType, CString strAcqCode, CString strAcqYear, CString strWorkCode, CString strSeqNo, CWnd *pWindow)
{
	CBO_CAT_REG_MAIN * m_pWindow = NULL;
	m_pWindow = (CBO_CAT_REG_MAIN*)pWindow;

	m_pWindow->KSearch(WorkType, strAcqCode, strAcqYear, strWorkCode, strSeqNo);
}