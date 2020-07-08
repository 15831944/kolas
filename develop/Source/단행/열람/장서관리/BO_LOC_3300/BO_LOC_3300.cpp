// BO_LOC_3300.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "..\\BL_LOC_USER\\BL_LOC_USER_PROC.h"
#include "BO_LOC_33001.h"
#include "BO_LOC_3310.h"
#include "..\\BL_LOC_USER\\BO_LOC_3320.h"
#include "BO_LOC_3370.h"
#include "BO_LOC_3380.h"
#include "BO_LOC_3390.h"
#include "BO_LOC_3395.h"
#include "BO_LOC_3397.h"
#include "BO_LOC_3398.h"
#include "BO_LOC_3399.h"
#include "BO_LOC_3399.h"
#include "UserDelSearchDlg.h"
#include "..\\BO_LOC_ARREAR\\ArrearMainDlg.h"
#include "ExpireUserSms.h" 

#include "..\..\..\..\�������\CO_CENTER_USER_MANAGE\CO_CENTER_USER_MANAGER_01.h"

#include "../CO_USER_CARD/UserCardSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE BO_LOC_3300DLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_LOC_3300.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_LOC_3300DLL, hInstance))
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

		new CDynLinkLibrary(BO_LOC_3300DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_LOC_3300.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_LOC_3300DLL);
	}
	return 1;   // ok
}



__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE  , CString strMenuAlias ){
	CBO_LOC_3300* pWindow = NULL;
	pWindow = new CBO_LOC_3300(NULL);
	pWindow->VIEW_STYLE = VIEW_STYLE;
	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();
	return pWindow;
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly , CWnd *pWindow){
	if (!pWindow) return NULL;
	
	CBO_LOC_3300 *pTmpWindow = (CBO_LOC_3300*)pWindow;

	if (strSM == _T("BO_LOC_3310"))
	{
		CBO_LOC_3310 *pDlg = NULL;
		pDlg = new CBO_LOC_3310(pTmpWindow, pTmpWindow->m_pLoanShareManager );
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
		{			
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}

	}
	else if (strSM == _T("BO_LOC_3320"))	{
		
		CBO_LOC_3320 *pDlg = NULL;
		pDlg = new CBO_LOC_3320( pTmpWindow, pTmpWindow->m_pLoanShareManager, pTmpWindow->m_pLoanMangeValue);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		pDlg->SetMode( 0 , FALSE );
		if (pDlg->Create(pTmpWindow))
		{	
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}		

	}
	else if (strSM == _T("BO_LOC_3330"))	{

		CBO_LOC_3320 *pDlg = NULL;
		pDlg = new CBO_LOC_3320( pTmpWindow, pTmpWindow->m_pLoanShareManager, pTmpWindow->m_pLoanMangeValue);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		pDlg->SetMode(1);
		pDlg->m_nAction = ACTION_MOD_F_USERINFO;

		if( pDlg->CheckParentDMCount() < 1 )
		{
			delete pDlg;
			return NULL;
		}
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
	else if (strSM == _T("BO_LOC_3350"))	{		
		CBO_LOC_3320 *pDlg = NULL;
		pDlg = new CBO_LOC_3320(pTmpWindow, pTmpWindow->m_pLoanShareManager, pTmpWindow->m_pLoanMangeValue);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		pDlg->SetMode(2, TRUE);
		pDlg->m_nAction = ACTION_READ_F_USERINFO;
		if( pDlg->CheckParentDMCount() < 1 )
		{
			delete pDlg;
			return NULL;
		}
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
	else if (strSM == _T("����"))	{
		BL_LOC_USER_PROC SpfWork( pTmpWindow ,pTmpWindow->m_pLoanShareManager);
		SpfWork.SPFExcutePath( _T("�̿���_����") );
	}
	else if (strSM == _T("����ó��"))	{
		BL_LOC_USER_PROC SpfWork( pTmpWindow ,pTmpWindow->m_pLoanShareManager);
		SpfWork.SPFExcutePath( _T("�̿���_����ó��") );
	}
	else if (strSM == _T("BO_LOC_3370"))	{
		CBO_LOC_3370 *pDlg = NULL;
		pDlg = new CBO_LOC_3370(pTmpWindow,pTmpWindow->m_pLoanShareManager);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
		{
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}		
	}
	else if (strSM == _T("���Թ���"))	{
		CBO_LOC_3380 Dlg(pTmpWindow);
		Dlg.DoModal();
		if( Dlg.GetCloseMode() == 0 ) return NULL;

		CBO_LOC_3390 fDlg( pTmpWindow , pTmpWindow->m_pLoanShareManager );

		if( Dlg.GetRunMode() == 0 )
			fDlg.SetMode(0);
		else
		{
			if (!pTmpWindow->m_pInfo->m_bUserInfoExportYN)
			{
				AfxMessageBox(_T("�̿��� ������ ������ ������ �����ϴ�.\r\n�ش� ������ �����ڸ� ���ؼ��� �ο����� �� �ֽ��ϴ�."), MB_ICONWARNING);
				return NULL;
			}
			fDlg.SetMode(1);
		}
		fDlg.DoModal();
			
	}
	else if (strSM == _T("���Ϲ���"))	{
		CBO_LOC_3390 fDlg( pTmpWindow , pTmpWindow->m_pLoanShareManager );
		fDlg.SetMode(0);
		fDlg.DoModal();
			
	}
	else if (strSM == _T("���Ϲ���"))	{
		if (!pTmpWindow->m_pInfo->m_bUserInfoExportYN)
		{
			AfxMessageBox(_T("�̿��� ������ ������ ������ �����ϴ�.\r\n�ش� ������ �����ڸ� ���ؼ��� �ο����� �� �ֽ��ϴ�."), MB_ICONWARNING);
			return NULL;
		}

		CBO_LOC_3390 fDlg( pTmpWindow , pTmpWindow->m_pLoanShareManager );
		fDlg.SetMode(1);
		fDlg.DoModal();
			
	}
	else if (strSM == _T("���"))	{
		if (!pTmpWindow->m_pInfo->m_bUserInfoExportYN)
		{
			AfxMessageBox(_T("�̿��� ������ ����� ������ �����ϴ�.\r\n�ش� ������ �����ڸ� ���ؼ��� �ο����� �� �ֽ��ϴ�."), MB_ICONWARNING);
			return NULL;
		}
		CBO_LOC_3395 *pDlg = NULL;
		pDlg = new CBO_LOC_3395(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
		{			
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}					
	}
	else if (strSM == _T("���;��ε�"))	{
		CCO_CENTER_USER_USE CenterUse( pTmpWindow );
		
		CString sLibKey,sLibCode;
		INT ids = CenterUse.IsUseCenterUser(sLibKey,sLibCode);

		if( ids != 0 )
		{
			AfxMessageBox( _T("�޴��� ����� �� ���� �������Դϴ�.") );
			return NULL;
		}

		CBO_LOC_3398 Dlg(pTmpWindow,sLibKey,sLibCode,pTmpWindow->m_pLoanShareManager);
		Dlg.DoModal();
	}
	else if (strSM == _T("���͹���"))	{
		
		CCO_CENTER_USER_MANAGER_01 *pDlg = NULL;
		pDlg = new CCO_CENTER_USER_MANAGER_01( pTmpWindow );
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = ( CESL_Mgr * )pTmpWindow;
		if( bReadOnly )
			pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if( pDlg->Create( pTmpWindow ) )
		{
			pDlg->m_mode = 1;			
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow( SW_SHOW );
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}

	}
	else if (strSM == _T("������"))	{
		if (!pTmpWindow->m_pInfo->m_bUserInfoExportYN)
		{
			AfxMessageBox(_T("�̿��� ������ ����� ������ �����ϴ�.\r\n�ش� ������ �����ڸ� ���ؼ��� �ο����� �� �ֽ��ϴ�."), MB_ICONWARNING);
			return NULL;
		}
		pTmpWindow->PrintList();
	}
	else if (strSM == _T("��������°���"))	{
		CBO_LOC_3397 *pDlg = NULL;
		pDlg = new CBO_LOC_3397(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
		{			
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
	}				}
	else if (strSM == _T("��������¼���"))	{
		CUserCardSetting *pDlg = NULL;
		pDlg = new CUserCardSetting(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
		{			
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}				
	}
	else if (strSM == _T("Ż��ó��"))
	{
		pTmpWindow->SecedeLoanUser();
	}
	else if(_T("�����̷º���") == strSM)	
	{
		CString sValue;
		pTmpWindow->m_pLoanMangeValue->GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), sValue);
		if(_T("Y") != sValue) sValue = _T("N");
		CUserDelSearchDlg dlg(pTmpWindow);
		dlg.m_strCivilNoHideYN = sValue;
		dlg.m_sIsUnityLoanService = pTmpWindow->m_pLoanMangeValue->m_sIsUnityLoanService;
		dlg.DoModal();					
	}
	else if(_T("��ü��������") == strSM)	
	{
		CArrearMainDlg dlg(pTmpWindow);
		dlg.DoModal();					
	}
	else if (strSM == _T("����ȸ��SMS�ȳ�"))	{
		CExpireUserSms *pDlg = NULL;
		pDlg = new CExpireUserSms(pTmpWindow);
		pDlg->pMain = pTmpWindow->pMain;
		pDlg->pParentMgr = (CESL_Mgr*)pTmpWindow;
		if (bReadOnly) pDlg->SM_STYLE = 'R';
		pDlg->m_pInfo = pTmpWindow->m_pInfo;
		if (pDlg->Create(pTmpWindow))
		{			
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->ShowWindow(SW_SHOW);
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
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
