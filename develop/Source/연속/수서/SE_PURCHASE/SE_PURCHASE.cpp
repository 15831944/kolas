// SE_PURCHASE.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "PURCHASE.h" 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE SE_PURCHASEDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SE_PURCHASE.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SE_PURCHASEDLL, hInstance))
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

		new CDynLinkLibrary(SE_PURCHASEDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SE_PURCHASE.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SE_PURCHASEDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE,CString strMenuAlias) 
{
	CPURCHASE* m_pWindow = new CPURCHASE(NULL);
	m_pWindow->VIEW_STYLE = VIEW_STYLE;
	m_pWindow->pMain = pParent;
	m_pWindow->m_pInfo = pInfo;
	m_pWindow->Create(pParent);
	m_pWindow->ShowWindow(SW_SHOW);
	m_pWindow->UpdateWindow();
	return m_pWindow;
}


__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow)
{
	CPURCHASE * m_pWindow = NULL;
	if (!pWindow) return NULL;
	else
		 m_pWindow = (CPURCHASE*)pWindow;
	

	if(strButtonAlias == _T("�˻�"))
	{
		m_pWindow->DoSearch();
	}
	

	if(strButtonAlias == _T("�Է�"))
	{
		m_pWindow->InsertPurchaseSpecies();
	}


	if(strButtonAlias == _T("����"))
	{
		m_pWindow->ModifyPurchaseSpecies();
	}
	

	if(strButtonAlias == _T("����"))
	{
		m_pWindow->DeletePurchaseSpecies();
	}

	if(strButtonAlias == _T("�ֹ�ó��"))
	{

		m_pWindow->OrderProcess();
	}

	if(strButtonAlias == _T("�ֹ����"))
	{

		m_pWindow->OrderCancel();
	}

	if(strButtonAlias == _T("���Ϲ���"))
	{

		m_pWindow->FileImport();
	}

	if(strButtonAlias == _T("���"))
	{
		m_pWindow->PrintGridData();
	}
	
	if(strButtonAlias == _T("�ֹ����"))
	{
		m_pWindow->OrderList();
	}
	
	//JOB.2019.0073
	/*
	if(strButtonAlias == _T("�ֹ�����"))
	{

		m_pWindow->OrderMail();
	}
	*/
	if(strButtonAlias == _T("��������"))
	{

		m_pWindow->OrderSeqNo();
	}
	if(strButtonAlias == _T("���԰����ϰ�����"))
	{

		m_pWindow->PriceChange();
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

