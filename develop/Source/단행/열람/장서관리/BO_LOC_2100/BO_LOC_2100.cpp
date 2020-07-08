// BO_LOC_2100.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "BO_LOC_21001.h"
#include "BO_LOC_2110.h"
#include "BO_LOC_2120.h"
#include "BO_LOC_2130.h"
#include "BO_LOC_2140.h"
#include "BO_LOC_2150.h"
#include "BO_LOC_2160.h"
#include "BO_LOC_2170.h"
#include "BO_LOC_2180.h"
#include "BO_LOC_2190.h"
#include "..\BL_LOC_2100\BLBookCheckProc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_LOC_2100DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_2100.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_LOC_2100DLL, hInstance))
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

		new CDynLinkLibrary(BO_LOC_2100DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_2100.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_LOC_2100DLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias ){
	CBO_LOC_2100* pWindow = NULL;
	pWindow = new CBO_LOC_2100(strMenuAlias,NULL);
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow){
	if (!pWindow) return NULL;
	
	CBO_LOC_2100 *pTmpWindow = (CBO_LOC_2100*)pWindow;

	if (strSM == _T("BO_LOC_2110"))
	{
		CBO_LOC_2110 *pDlg = NULL;
		pDlg = new CBO_LOC_2110(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if (strSM == _T("BO_LOC_2120"))	{		
		INT ids = MessageBox( pTmpWindow->m_hWnd , _T("������ڷ�(�Էµ��� ���� ��Ϲ�ȣ)�� �˻��մϴ�.") , _T("�����˻�") , MB_YESNO | MB_ICONQUESTION );
		if( ids == IDYES )
		{
			CBLBookCheckProc SpfWork( pTmpWindow );
			SpfWork.SPFExcutePath(SEARCH_BOOK_CHECK_N);

			return NULL;
		}
	}
	else if (strSM == _T("BO_LOC_2130"))	{
		CBO_LOC_2130 *pDlg = NULL;
		pDlg = new CBO_LOC_2130(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if (strSM == _T("BO_LOC_2140"))	{
		INT ids = pTmpWindow->IsShelfLocNoSelect();
		if( ids == 1 )
		{
			ids = MessageBox( pTmpWindow->m_hWnd ,  _T("�ڷ���� ������ �Ǿ� ���� �ʽ��ϴ�. \r\n�����Ͻð� �����Ͻðڽ��ϱ�?\r\n\r\n(��:�ڷ�Ǽ��������� , �ƴϿ�:�����ϰ�����)") 
				, _T("�弭 ����") 
				, MB_YESNO | MB_ICONQUESTION );			
			if( ids == IDYES ) return NULL;
		}

		CBO_LOC_2140 Dlg;
		Dlg.pMain = pTmpWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)pTmpWindow;
		Dlg.m_pInfo = pTmpWindow->m_pInfo;
		Dlg.DoModal();

		pTmpWindow->ResultGridDisplay();
	}
	else if (strSM == _T("�ʱ�ȭ"))	{
		CBO_LOC_2150 *pDlg = NULL;
		pDlg = new CBO_LOC_2150(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if ( strSM == _T("���˿Ϸ�") )
	{		
		INT ids = MessageBox( pTmpWindow->m_hWnd , _T("���˹�ȣ�� ���� �����ϰڽ��ϱ�?\r\n\r\n(��:���ο� ���˹�ȣ �ο�,�ƴϿ�:������ ���Ǿ��� ���˹�ȣ ���� �� ����,���:���)") , _T("�������") , MB_YESNOCANCEL | MB_ICONQUESTION );		
		if( ids == IDCANCEL ) return NULL;
		if( ids == IDNO )
		{
			CBO_LOC_2160 NumberDlg(pTmpWindow);
			NumberDlg.m_nLoadSPF = 1;
			NumberDlg.DoModal();
		}
		else if( ids == IDYES )
		{
			pTmpWindow->m_pSpfWork->SPFExcutePath(INIT_BOOK_CHECK_NUMBER);
		}

		CBO_LOC_2170 Dlg(pTmpWindow);
		Dlg.pMain = pTmpWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) Dlg.SM_STYLE = 'R';
		Dlg.m_pInfo = pTmpWindow->m_pInfo;

		Dlg.DoModal();
	}
	else if (strSM == _T("������"))	{
		CBO_LOC_2180 *pDlg = NULL;
		pDlg = new CBO_LOC_2180(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if (strSM == _T("������"))	{
		pTmpWindow->PrintList();
	}
	else if (strSM == _T("���"))	{
		CBO_LOC_2190 *pDlg = NULL;
		pDlg = new CBO_LOC_2190(pTmpWindow,pTmpWindow->m_sStrMenuName);
		pDlg->m_pAxisVal_Statics = &pTmpWindow->m_cAxisVal_Statics;
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
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
	else if (strSM == _T("����"))	
	{
		return	pTmpWindow->ShowSortDlg();	
	}

	return NULL;
}	

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){
	if(pWindow){
		delete pWindow;
	}
}