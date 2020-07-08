// BO_CAT.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "MainDefaultDlg.h"

// �˻�
#include "SE_CAT_SearchDlg.h"

// �⺻�� ����
#include "..\..\..\ESL\Marc\MarcToolKit\MarcDefaultValueConfigDlg.h"

// ������ �ϰ�����
#include "..\..\..\����\����\BO_CAT_PROC\BO_CAT_PROC_0801.h"

// ������ �ϰ�Ȯ��
#include "..\..\..\����\����\BO_CAT_PROC\BO_CAT_PROC_1300.h"

// ��ũ ����
//#include _T("..\..\..\����\����\BO_CAT_PROC\BO_CAT_PROC_1100.h")
#include "..\SE_CAT_MARC_EXPORT\MarcExportDlg_SE.h"

// ��ũ ����
#include "..\SE_CAT_MARC_IMPORT\MarcImportDlg_SE.h"

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
		TRACE0("SE_CAT.DLL Initializing!\n");
		
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
		TRACE0("SE_CAT.DLL Terminating!\n");
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

	// �˻�
	if (strSM.Find(_T("�˻�")) > 0)
	{
		CSE_CAT_SearchDlg *pDlg = new CSE_CAT_SearchDlg(pParentDlg, g_strMenuAlias);
		if (pDlg->Create(pParentDlg))
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->CenterWindow();
			return pDlg;
		}
	}

	// ������ �ۼ�
	else if (strSM.Find(_T("�������ۼ�")) > 0)
	{
		pParentDlg->PRINT(_T("������"));
	}

	// ������װ���
	else if (strSM.Find(_T("������װ���")) > 0)
	{
		CMarcDefaultValueConfigDlg dlg(pParentDlg);
		dlg.SetMarcGroupFormCode(_T("TX"), _T("����"));
		dlg.DoModal();
	}

	// ��������
	else if (strSM == _T("��������_��������"))
	{
		pParentDlg->DUP_Search();
	}
	else if (strSM == _T("��������_��������"))
	{
		pParentDlg->DUP_Free();
	}
	else if (strSM == _T("��������_Ű����"))
	{
		pParentDlg->DUP_KeySetting();
	}

	// ��Ͽϼ�
	else if (strSM == _T("��Ͽϼ�_�Է�"))
	{
		pParentDlg->MarcAdjust_Insert();
	}
	else if (strSM == _T("��Ͽϼ�_����"))
	{
		pParentDlg->MarcAdjust_Modify();
	}
	else if (strSM == _T("��Ͽϼ�_�������ϰ�����"))
	{
		CBO_CAT_PROC_0801 *pDlg = new CBO_CAT_PROC_0801(pParentDlg);
		pDlg->SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		if (pDlg->Create(pParentDlg) && pDlg->m_bInitDlg)
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}		
	}
	else if (strSM == _T("��Ͽϼ�_�ڷ��ϰ�Ȯ��"))
	{
		CBO_CAT_PROC_1300 *pDlg = new CBO_CAT_PROC_1300(pParentDlg);
		pDlg->SetParentInfo(pParentDlg->m_strMainDMAlias, pParentDlg->m_strMainCMAlias, pParentDlg->m_strMainGridAlias);
		if (pDlg->Create(pParentDlg) && pDlg->m_bInitDlg)
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}		
	}
	else if (strSM == _T("��Ͽϼ�_��ũ����"))
	{
		CMarcImportDlg_SE dlg(pParentDlg);
		dlg.SetOpenMode(4000);
		dlg.DoModal();
	}
	else if (strSM == _T("��Ͽϼ�_��ũ����"))
	{
		//CBO_CAT_PROC_1100 *pDlg = new CBO_CAT_PROC_1100(pParentDlg);
		CMarcExportDlg_SE *pDlg = new CMarcExportDlg_SE(pParentDlg);
		if (pDlg->Create(pParentDlg))
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->CenterWindow();
			return pDlg;
		}
		else
		{
			delete pDlg;
			return NULL;
		}
	}


	// �����
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

	// �����ΰ�
	else if (strSM == _T("�����ΰ�ó��_���"))
	{
		pParentDlg->PRINT(_T("�����ΰ���"));
	}

	// ���� �̵�
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
	else if (strSM == _T("��Ͽϼ�_�����Ϸ�_�����ΰ���"))
	{
		ids = pParentDlg->Complete_Cataloging();
		if (ids >= 0) 
		{
			AfxMessageBox(_T("���� �Ϸ� ó���Ͽ����ϴ�."));
			pParentDlg->SelectAgain();
		}
	}
	else if (strSM == _T("��Ͽϼ�_�����Ϸ�_�����ΰ��ڷ�"))
	{
		ids = pParentDlg->Complete_Cataloging();
		if (ids < 0) return NULL;
		ids = pParentDlg->Complete_Loc();
		if (ids >= 0) 
		{
			AfxMessageBox(_T("���� �Ϸ� ó���Ͽ����ϴ�."));
			pParentDlg->SelectAgain();
		}
	}
	else if (strSM == _T("��Ͽϼ�_�����Ϸ�_�谡�ڷ�"))
	{
		ids = pParentDlg->Complete_Cataloging(FALSE);
		if (ids < 0) return NULL;
		ids = pParentDlg->Complete_Loc2();
		if (ids >= 0) 
		{
			AfxMessageBox(_T("���� �Ϸ� ó���Ͽ����ϴ�."));
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
		ids = pParentDlg->Complete_Loc2();
		if (ids >= 0) 
		{
			pParentDlg->SelectAgain();
			AfxMessageBox(_T("�����ΰ� ó���Ͽ����ϴ�."));
		}
	}

	// �谡���� ���
	else if (strSM == _T("�谡����_�谡���ڱ��"))
	{
		pParentDlg->Loc_Record();
		if (ids >= 0) pParentDlg->SelectAgain();
	}
	
	// 040319_HSS ������ �߰�
	if(strButtonAlias == _T("����"))
	{
		pParentDlg->DeleteSpecies();
	}

	return NULL;
}