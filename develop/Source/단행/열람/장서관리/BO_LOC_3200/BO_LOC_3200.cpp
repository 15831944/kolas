// BO_LOC_3200.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "BO_LOC_32001.H"
#include "BO_LOC_3210.H"
#include "BO_LOC_3220.H"
#include "BO_LOC_3230.H"
#include "BO_LOC_3240.H"
#include "BO_LOC_3250.H"
#include "BO_LOC_3260.h"
#include "BO_LOC_3270.h"
#include "BO_LOC_3280.h"
#include "BO_LOC_3290.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_LOC_3200DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_3200.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_LOC_3200DLL, hInstance))
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

		new CDynLinkLibrary(BO_LOC_3200DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_3200.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_LOC_3200DLL);
	}
	return 1;   // ok
}


__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){
	CBO_LOC_3200* pWindow = NULL;
	pWindow = new CBO_LOC_3200(NULL);
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
	
	CBO_LOC_3200 *pTmpWindow = (CBO_LOC_3200*)pWindow;

	if (strSM == _T("BO_LOC_3210"))
	{
		CBO_LOC_3210 *pDlg = NULL;
		pDlg = new CBO_LOC_3210(pTmpWindow,pTmpWindow->m_pLoanMangeValue,pTmpWindow->m_pLoanShareManager );
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		CString sSelfReturnStatusUseYN;
		pTmpWindow->GetSelfReturnStatusUseYN(sSelfReturnStatusUseYN);
		pDlg->SetSelfReturnStatusUseYN(sSelfReturnStatusUseYN);
		if (pDlg->Create(pTmpWindow))
		{
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			pDlg->UpdateWindow();			
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}
	else if (strSM == _T("BO_LOC_3220"))	{

		CBO_LOC_3220 Dlg(pTmpWindow,pTmpWindow->m_pLoanMangeValue , pTmpWindow->m_pLoanShareManager );
		if( Dlg.CheckParentDMCount() < 1 )
		{
			return NULL;
		}

		Dlg.DoModal();

	}
	else if (strSM == _T("�ϰ�����"))	{
		
		CBO_LOC_3260 Dlg(pTmpWindow,pTmpWindow->m_pLoanMangeValue , pTmpWindow->m_pLoanShareManager);

		if( Dlg.CheckParentDMCount() < 1 )
		{
			return NULL;
		}

		Dlg.DoModal();

	}
	else if (strSM == _T("BO_LOC_3230"))	{
		CBO_LOC_3230 *pDlg = NULL;
		pDlg = new CBO_LOC_3230(pTmpWindow,pTmpWindow->m_pLoanMangeValue, pTmpWindow->m_pLoanShareManager);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;

		if( pDlg->CheckParentDMCount() < 1 )
		{
			delete pDlg;
			return NULL;
		}

		if (pDlg->Create(pTmpWindow))
		{
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			pDlg->UpdateWindow();
			
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}		
	}
	else if (strSM == _T("BO_LOC_3240"))
	{
		CBO_LOC_3240 Dlg(pTmpWindow,pTmpWindow->m_pLoanMangeValue, pTmpWindow->m_pLoanShareManager);
		CESL_DataMgr*	pDM	=	pTmpWindow->FindDM( _T("DM_BO_LOC_3200") );
		if( pDM == NULL ) return NULL;

		INT	nRowCnt = pDM->GetRowCount();
		
		if( 1 > nRowCnt )
		{
			return 0;
		}
		Dlg.DoModal();
	}
	else if (strSM == _T("ȸ������"))
	{
		CBL_LOC_LOAN_MNG_PROC SpfWork( pTmpWindow , pTmpWindow->m_pLoanMangeValue , pTmpWindow->m_pLoanShareManager);
		SpfWork.SPFExcutePath( _T("ȸ������") );
	}
	else if (strSM == _T("���"))
	{
		if (!pTmpWindow->m_pInfo->m_bUserInfoExportYN)
		{
			AfxMessageBox(_T("�̿��� ������ ����� ������ �����ϴ�.\r\n�ش� ������ �����ڸ� ���ؼ��� �ο����� �� �ֽ��ϴ�."), MB_ICONWARNING);
			return NULL;
		}
		CBO_LOC_3270 *pDlg = NULL;
		pDlg = new CBO_LOC_3270(pTmpWindow);
		pDlg->m_sIsUnityLoanService = pTmpWindow->m_pLoanMangeValue->m_sIsUnityLoanService;
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
		{
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			pDlg->UpdateWindow();			
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}		
	}
	else if (strSM == _T("������"))
	{
		if (!pTmpWindow->m_pInfo->m_bUserInfoExportYN)
		{
			AfxMessageBox(_T("�̿��� ������ ����� ������ �����ϴ�.\r\n�ش� ������ �����ڸ� ���ؼ��� �ο����� �� �ֽ��ϴ�."), MB_ICONWARNING);
			return NULL;
		}

		pTmpWindow->PrintList();
	}
	else if (strSM == _T("���ιݳ��ڷ�ó��"))
	{
		pTmpWindow->StationBookReturn();
	}
	else if (strSM == _T("��������߱�"))
	{
		pTmpWindow->ReissueReceipt();
	}
	else if ( strSM == _T("�а��ݳ��ڷ�ó��") )
	{
		pTmpWindow->OtherManageCodeBookReturn();
	}
	else if ( strSM == _T("�߰����⿹����º���") )
	{
		pTmpWindow->ChangeNightLoanReservationStatus();		
	}
	else if ( strSM == "Ÿ���ݳ��ڷ�ó��" )
	{
		pTmpWindow->OtherLibCodeBookReturn();
	}
	else if ( strSM == _T("��ü�ϰ��") )
	{
		pTmpWindow->GetTotalDelayCount();		
	}
	return NULL;
}	

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow){
	if(pWindow){
		pWindow->MoveWindow(rect);
	}
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow){
	
	CBO_LOC_3200 *pTmpWindow = (CBO_LOC_3200*)pWindow;
	if(pTmpWindow){
		delete pTmpWindow;
	}
}
