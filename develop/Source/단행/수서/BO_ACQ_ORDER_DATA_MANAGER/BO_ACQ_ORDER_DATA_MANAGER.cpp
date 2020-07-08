// BO_ACQ_ORDER_DATA_MANAGER.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "BO_ACQ_OrderDataMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_ACQ_ORDER_DATA_MANAGERDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_ACQ_ORDER_DATA_MANAGER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_ACQ_ORDER_DATA_MANAGERDLL, hInstance))
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

		new CDynLinkLibrary(BO_ACQ_ORDER_DATA_MANAGERDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_ACQ_ORDER_DATA_MANAGER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_ACQ_ORDER_DATA_MANAGERDLL);
	}
	return 1;   // ok
}

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CBO_ACQ_OrderDataMgr * m_pWindow = new CBO_ACQ_OrderDataMgr(NULL);
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
	CBO_ACQ_OrderDataMgr * m_pWindow = NULL;
	if (!pWindow) return NULL;
	else
		 m_pWindow = (CBO_ACQ_OrderDataMgr*)pWindow;

	if(strButtonAlias == _T("�˻�"))
	{
		m_pWindow->ShowSearchDlg();
		//=============================================
		//20080608 UPDATE BY PJW : �˼��ڷ�������� �˻�â�� ���� �������� ���� 
		return NULL;
		//return m_pWindow->GetSearchDlg();
		//=============================================
	}
	else if(strButtonAlias == _T("���"))
	{
		m_pWindow->ShowPrintDlg();
		return NULL;		
	}
	else if(strButtonAlias == _T("�Է�"))
	{
		m_pWindow->ShowOrderDataDlg(SINSERT);
		return NULL;
	}
	else if(strButtonAlias == _T("����"))
	{
		m_pWindow->ShowOrderDataDlg(SUPDATE);
		return NULL;
	}
	else if(strButtonAlias == _T("����"))
	{
		m_pWindow->DeleteOrderData();
		return NULL;
	}
	else if(strButtonAlias == _T("�ֹ����"))
	{
		m_pWindow->OrderCancel();
		return NULL;
	}
	else if(strButtonAlias == _T("�˼�ó��"))
	{
		m_pWindow->CheckInOrderData();
		return NULL;
	}
	else if(strButtonAlias == _T("�˼��Ϸ�"))
	{
		m_pWindow->CheckInComplete();
		return NULL;
	}
	// 2004.10.29 �ڷ����, ���Ϲ��� �߰�
	else if(strButtonAlias == _T("�ڷ����"))
	{
		m_pWindow->FileExport();
		return NULL;
	}
	else if(strButtonAlias == _T("���Ϲ���"))
	{
		m_pWindow->FileImport();
		return NULL;
	}
	else if(strButtonAlias == _T("��������"))
	{
		m_pWindow->ShowChangePurchaseInfoDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("��������"))
	{
		m_pWindow->ShowDivNoMgrDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("�����ջ�"))
	{
		m_pWindow->ShowPriceSumDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("�ڵ庯��"))
	{
		m_pWindow->ShowChangeCodeDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("��ǰó��"))
	{
		m_pWindow->SelDataDeliveryProc();
		return NULL;
	}
	else if(strButtonAlias == _T("��ǰ���"))
	{
		m_pWindow->SelDataDeliveryCancelProc();
		return NULL;
	}
	else if(strButtonAlias == _T("����ΰ�"))
	{
		m_pWindow->RegTransfer();
		return NULL;
	}
	else if(strButtonAlias == _T("�˼��Ϸ��ڷ�"))
	{
		m_pWindow->ViewCheckInCompleteData();
		return NULL;
	}
	//JOB.2019.0073 : �浹 License ����
// 	else if(strButtonAlias == _T("�ֹ�����"))
// 	{
// 		m_pWindow->ShowOrderCancelMailMgr(1000, -1000, NULL);
// 		return NULL;
// 	}
	else if(strButtonAlias == _T("����"))
	{
		m_pWindow->ShowSortDlg();
		return NULL;
	}
	else if(strButtonAlias == _T("��ũ��ü"))
	{
		m_pWindow->ChangeMarc();
		return NULL;
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

__declspec(dllexport) void KalendarSearch(CString WorkType, CString strAcqCode, CString strAcqYear, CString strWorkCode, CString strSeqNo, CWnd *pWindow)
{
	CBO_ACQ_OrderDataMgr * m_pWindow = NULL;
	m_pWindow = (CBO_ACQ_OrderDataMgr*)pWindow;

	m_pWindow->KSearch(WorkType, strAcqCode, strAcqYear, strWorkCode, strSeqNo);
}