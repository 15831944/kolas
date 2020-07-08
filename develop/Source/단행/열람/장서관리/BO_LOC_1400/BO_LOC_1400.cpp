// BO_LOC_1400.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "BO_LOC_14001.h"
#include "BO_LOC_1410.h"
#include "BO_LOC_1420.h"
#include "BO_LOC_1430.h"
#include "BO_LOC_1440.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_LOC_1400DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_1400.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_LOC_1400DLL, hInstance))
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

		new CDynLinkLibrary(BO_LOC_1400DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_1400.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_LOC_1400DLL);
	}
	return 1;   // ok
}


__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){
	CBO_LOC_1400* pWindow = NULL;

	pWindow = NULL;
	pWindow = new CBO_LOC_1400(NULL);
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
	
	CBO_LOC_1400 *m_pWindow = (CBO_LOC_1400*)pWindow;
	
	if (strSM == _T("BO_LOC_1410"))
	{
		CBO_LOC_1410 *pDlg = NULL;
		pDlg = new CBO_LOC_1410(m_pWindow);
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
	}else if (strSM == _T("BO_LOC_1420"))	{

		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 ) 
		{
			AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
			return NULL;
		}

		CBO_LOC_1420 Dlg(m_pWindow);
		Dlg.pMain = m_pWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) Dlg.SM_STYLE = 'R';
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		Dlg.DoModal();
	}else if (strSM == _T("BO_LOC_1430"))	{
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 ) 
		{
			AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
			return NULL;
		}

		CBO_LOC_1430 Dlg(m_pWindow);
		Dlg.pMain = m_pWindow->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)m_pWindow;
		if (bReadOnly) Dlg.SM_STYLE = 'R';
		Dlg.m_pInfo = m_pWindow->m_pInfo;
		Dlg.DoModal();
	}else if (strSM == _T("�����Ϸ�"))
	{
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 ) 
		{
			AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
			return NULL;
		}
		
		m_pWindow->m_pSpfWork->SPFExcutePath( COMPLETE_REPAIR );
	}
	else if (strSM == _T("������"))
	{
		if( m_pWindow->m_pSpfWork->GetSelectMainGridCnt() < 1 ) 
		{
			AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
			return NULL;
		}

		m_pWindow->m_pSpfWork->SPFExcutePath( CANCEL_REPAIR );

	}
	else if (strSM == _T("���"))
	{
		m_pWindow->PrintList();
	}
	else if (strSM == _T("���"))
	{
		CBO_LOC_1440 Dlg(m_pWindow);
		Dlg.DoModal();
	}
	return NULL;
}	

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){
	CBO_LOC_1400 *m_pWindow = (CBO_LOC_1400*)pWindow;
	if(m_pWindow){
		if( m_pWindow->m_pSpfWork )
			delete m_pWindow->m_pSpfWork;
		delete m_pWindow;
	}
}