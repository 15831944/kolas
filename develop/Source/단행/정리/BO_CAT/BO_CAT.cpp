// BO_CAT.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "MainDefaultDlg.h"

#include "BO_CAT_SearchDlg.h"

#include "..\..\..\ESL\Marc\MarcToolKit\MarcDefaultValueConfigDlg.h"

#include "..\BO_CAT_PROC\BO_CAT_PROC_0400_08.h"

#include "..\BO_CAT_PROC\BO_CAT_PROC_0500_09.h"

#include "..\BO_CAT_PROC\BO_CAT_PROC_0801.h"

#include "..\BO_CAT_PROC\BO_CAT_PROC_1300.h"

#include "..\BO_CAT_MARC_EXPORT\MarcExportDlg.h"

#include "..\BO_CAT_MARC_IMPORT\MarcImportDlg.h"

#include "BO_ClassCodeVolumeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE BO_CATDLL = { NULL, NULL };

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_CAT.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_CATDLL, hInstance))
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

		new CDynLinkLibrary(BO_CATDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_CAT.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_CATDLL);
	}
	return 1;   // ok
}

CString g_strMenuAlias;

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias) 
{
	CMainDefaultDlg* pWindow = new CMainDefaultDlg(NULL);

	g_strMenuAlias = strMenuAlias;

	pWindow->VIEW_STYLE = VIEW_STYLE;

	pWindow->pMain = pParent;
	pWindow->m_pInfo = pInfo;
	pWindow->m_strMenuAlias = strMenuAlias;
	pWindow->Create(pParent);
	pWindow->ShowWindow(SW_SHOW);
	pWindow->UpdateWindow();

	return pWindow;
}

__declspec(dllexport) VOID ResizeForm(CRect rect, CWnd *pWindow) 
{
	if( pWindow )
		pWindow->MoveWindow( rect );
}

__declspec(dllexport) VOID DeleteForm(CWnd *pWindow)
{
	if( pWindow )
	{
		pWindow->DestroyWindow();
		delete pWindow;
	}
}

__declspec(dllexport) CESL_Mgr* CreateModelessDlg(CString strSM, CString strButtonAlias, BOOL bReadOnly, CWnd *pWindow) 
{	
	if (!pWindow) return NULL;

	INT ids = -1;
	CMainDefaultDlg *pParentDlg = static_cast<CMainDefaultDlg*>(pWindow);
	
	if (strSM.Find(_T("�˻�")) > 0)
	{
		CBO_CAT_SearchDlg *pDlg = new CBO_CAT_SearchDlg(pParentDlg, g_strMenuAlias);
		if (pDlg->Create(pParentDlg))
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->CenterWindow();
			return pDlg;
		}
	}
	else if (strSM.Find(_T("�������ۼ�")) > 0)
	{
		pParentDlg->PRINT(_T("������"));
	}

	else if (strSM.Find(_T("������װ���")) > 0)
	{
		CMarcDefaultValueConfigDlg dlg(pParentDlg);

		CString strFormCode;
		INT nIds;
		nIds = pParentDlg->m_pInfo->pCodeMgr->GetDefaultCode(_T("���ı���"),strFormCode);
		strFormCode.TrimLeft();	strFormCode.TrimRight();
		if( strFormCode.IsEmpty() || nIds < 0)
		{
			dlg.SetMarcGroupFormCode(_T("BK"), _T("���ո�ũ"));
		}
		else
		{
			dlg.SetMarcGroupFormCode(strFormCode, _T("���ո�ũ"));			
		}	
		dlg.DoModal();
	}

	else if (strSM == _T("��������_��������"))
	{
		pParentDlg->DUP_Search();
	}
	else if (strSM == _T("��������_��������"))
	{
		ids = pParentDlg->DUP_Free();
		if (ids == 1) pParentDlg->SelectAgain();
	}
	else if (strSM == _T("��������_Ű����"))
	{
		pParentDlg->DUP_KeySetting();
	}
	else if (strSM == _T("�з�_����"))
	{
		pParentDlg->MarcAdjust_Modify();
	}
	else if (strSM == _T("��Ͽϼ�_������"))
	{
		pParentDlg->WebImport();
	}
	else if (strSM == _T("��Ͽϼ�_����������"))
	{
		pParentDlg->DMDPImport();
	}
	else if (strSM == _T("��Ͽϼ�_�Է�"))
	{
		pParentDlg->MarcAdjust_Insert();
	}

	else if (strSM == _T("��Ͽϼ�_����"))
	{
		pParentDlg->MarcAdjust_Modify();
	}
	else if (strSM == _T("��Ͽϼ�_����"))
	{
		pParentDlg->MarcAdjust_Delete();
	}
	else if (strSM == _T("��Ͽϼ�_������"))
	{
		CBO_CAT_PROC_0400_08  pDlg(pParentDlg);
		pDlg.SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		pDlg.DoModal();		
		return NULL;
	}
	else if (strSM == _T("��Ͽϼ�_���и�"))
	{		
		CBO_CAT_PROC_0500_09 pDlg(pParentDlg);
		pDlg.SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		pDlg.DoModal();		
		return NULL;
		
	}
	else if (strSM == _T("��Ͽϼ�_�������ϰ�����"))
	{
		CBO_CAT_PROC_0801 *pDlg = new CBO_CAT_PROC_0801(pParentDlg);
		pDlg->SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		pDlg->DoModal();
		pParentDlg->RecoverySelect();
	}
	else if (strSM == _T("��Ͽϼ�_�ڷ��ϰ�Ȯ��"))
	{
		CBO_CAT_PROC_1300 pDlg(pParentDlg);
		pDlg.SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		pDlg.DoModal();		
	}
	else if (strSM == _T("��Ͽϼ�_��ũ����"))
	{
		CMarcImportDlg dlg(pParentDlg);
		dlg.SetOpenMode(4000);
		dlg.DoModal();
	}
	else if (strSM == _T("��Ͽϼ�_��ũ����"))
	{
		pParentDlg->MarcExport();
		pParentDlg->RecoverySelect();
	}
	else if (strSM == _T("�з�_�з��ǻ���"))
	{
		CBO_ClassCodeVolumeDlg dlg(pParentDlg);
		dlg.DoModal();
		return NULL;
	}
	else if (strSM == _T("����ڷẸ��"))
	{
		pParentDlg->PRINT(_T("����ڷẸ��"));
	}
	else if (strSM == _T("�������ڷ����"))
	{
		pParentDlg->PRINT(_T("�������ڷ����"));
	}
	else if (strSM == _T("�����_������"))
	{
		pParentDlg->PRINT(_T("���"));
	}
	else if (strSM == _T("�����_ī����"))
	{
		pParentDlg->PRINT(_T("ī����"));
	}
	else if (strSM == _T("�����_���η������"))
	{
		pParentDlg->PRINT(_T("���η�"));
	}
	else if (strSM == _T("�����ΰ�ó��_���"))
	{
		pParentDlg->PRINT(_T("�����ΰ���"));
	}
	else if (strSM == _T("��������_��������Ϸ�_�������"))
	{
		ids = pParentDlg->Complete_Dup();
		if (ids >= 0) pParentDlg->SelectAgain();
	}
	else if (strSM == _T("��������_��������Ϸ�_�з����"))
	{
		ids = pParentDlg->Complete_Dup2();
		if (ids >= 0) pParentDlg->SelectAgain();
	}
	else if (strSM == _T("�з�_�з��Ϸ�"))
	{
		ids = pParentDlg->Complete_Classification();
		if (ids >= 0) pParentDlg->SelectAgain();
	}
	else if (strSM == _T("��Ͽϼ�_����ڷẹ��"))
	{
        ids = pParentDlg->CheckReturnRegist() ;
		if ( ids == 0 )
		{
			ids = pParentDlg->UpdateWorkingStatus(_T(""), _T("BOR112N"));
			if (ids >= 0) 
			{
				pParentDlg->Delete049Tag () ;	
					AfxMessageBox(_T("����ڷ�� ���� ó���Ͽ����ϴ�."));
				pParentDlg->SelectAgain();
			}
		}
	}
	else if (strSM == _T("��Ͽϼ�_�����Ϸ�_�����ΰ���"))
	{
		ids = pParentDlg->Complete_Cataloging( CATALOG_COMPLETE );
		if (ids >= 0) 
		{
			AfxMessageBox(_T("���� �Ϸ� ó���Ͽ����ϴ�."));
			pParentDlg->SelectAgain();
		}
	}
	else if (strSM == _T("��Ͽϼ�_�����Ϸ�_�����ΰ��ڷ�"))
	{
		ids = pParentDlg->Complete_Cataloging(LOC_TRANSFER);
		if (ids >= 0) 
		{
			AfxMessageBox(_T("�����ΰ� ó���Ͽ����ϴ�."));
			pParentDlg->SelectAgain();
		}
	}
	else if (strSM == _T("��Ͽϼ�_�����Ϸ�_�谡�ڷ�"))
	{
		ids = pParentDlg->Complete_Cataloging( SHELF_DATE_WRITE, FALSE);
		if (ids >= 0) 
		{
			pParentDlg->Delete_DupFlag_Species() ;

			AfxMessageBox(_T("�谡��� ó���Ͽ����ϴ�."));
			pParentDlg->SelectAgain();
		}
	}
	else if (strSM == _T("�����ΰ�ó��_�����ΰ��ڷ�"))
	{
		ids = pParentDlg->Complete_Loc();
		if (ids >= 0) 
		{
			pParentDlg->SelectAgain();
			AfxMessageBox(_T("�����ΰ� ó���Ͽ����ϴ�."));
		}
	}
	else if (strSM == _T("�����ΰ�ó��_�谡�ڷ�"))
	{
		ids = pParentDlg->Complete_Cataloging( SHELF_DATE_WRITE, FALSE);
		if (ids >= 0) 
		{
			pParentDlg->SelectAgain();
			AfxMessageBox(_T("�谡��� ó���Ͽ����ϴ�."));
		}
	}
	else if (strSM == _T("�谡����_�谡���ڱ��"))
	{
		ids = pParentDlg->Loc_Record();
		if (ids >= 0) pParentDlg->SelectAgain();
	}
	else if (strSM == _T("��Ͽϼ�_�ڵ庯��"))
	{
		ids = pParentDlg->ModifyCode() ;
	}
	else if (strSM == _T("��Ͽϼ�_��Ϲ�ȣ��ü"))
	{
		ids = pParentDlg->RegNoChang() ;
	}
	else if (strSM == _T("��Ͽϼ�_�����Է�"))
	{
		ids = pParentDlg->InputDcm();
	}
	else if (strSM == _T("��Ͽϼ�_�з��ǻ���"))
	{
		CBO_ClassCodeVolumeDlg dlg(pParentDlg);
		dlg.DoModal();
		return NULL;
	}
	else if (strSM == _T("�����ȣ���"))
	{
		pParentDlg->PRINT(_T("�����ȣ���"));
	}

	return NULL;
}



