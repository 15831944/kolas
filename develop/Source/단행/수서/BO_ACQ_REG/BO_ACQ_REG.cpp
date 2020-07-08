// BO_ACQ_REG.cpp : Defines the initialization routines for the DLL.
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
#include "BO_CAT_REG_0410.h"
#include "BO_CAT_REG_0420.h"

#include "../../����/BO_CAT_REG/BO_CAT_REG_0400.h"
#include "../../����/BO_CAT_REG/BO_CAT_SEARCH_MGR_PRE.h"
#include "../../����/BO_CAT_REG/BO_REG_CODE.h"
#include "../../����/BO_CAT_REG/BO_CAT_GROUP_INSERT.h"
#include "../../����/BO_CAT_REG/BO_CAT_REG_0500.h"
#include "../../����/BO_CAT_REG/BO_CAT_PROC_1100.h"
#include "../../����/BO_CAT_REG/MultiSortDlg.h"
#include "../../����/BO_CAT_REG/BO_CAT_REG_RFID.h"

#include "..\..\..\SmartID\BarCodeToRFID2\SmartIDScannerDlg.h"
#include "..\..\..\SmartID\BarCodeToRFID2\BarcodeToRFIDDlg.h"

static AFX_EXTENSION_MODULE BO_ACQ_REGDLL = { NULL, NULL };

static INT OpenMode;

extern "C" INT APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BO_ACQ_REG.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BO_ACQ_REGDLL, hInstance))
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

		new CDynLinkLibrary(BO_ACQ_REGDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BO_ACQ_REG.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BO_ACQ_REGDLL);
	}
	return 1;   // ok
}

CString g_strMenuAlias;

__declspec(dllexport) CWnd* CreateForm(CWnd *pParent, CESL_Information* pInfo, TCHAR VIEW_STYLE ,CString strMenuAlias) 
{

	g_strMenuAlias = strMenuAlias;

	//����ڷ���� 
	if ( strMenuAlias == _T("����_���_����ڷ����") )
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
		CBO_CAT_SEARCH *pDlg = NULL;
		pDlg = new CBO_CAT_SEARCH(m_pWindow);
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
	
	else if (strSM == _T("����ڷ��Է�"))
	{
		m_pWindow->InputBook();
	}
	else if (strSM == _T("����ڷ����"))
	{
		m_pWindow->ModifyBook();
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
/*
		pDlg->CancelRegNo(); 
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
/*		
		pDlg->CancelRegNo();
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
		if (g_strMenuAlias == _T("����_���_����ڷ����"))
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
		if (pDlg->Create(m_pWindow))
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

	else if (strSM == _T("������"))
	{
		CESL_DataMgr *pDM = m_pWindow->FindDM(_T("DM_����_����_���_����"));
		if (pDM == NULL) return NULL;

		CSIReportManager SIReportManager(m_pWindow);
		SIReportManager.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

		INT ids = SIReportManager.GetSIReportFileFromDB(_T("���_�˻����_����"));
		if (ids < 0) 
		{
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
		CESL_ControlMgr * pCM = static_cast<CESL_Mgr*>(pWindow)->FindCM(strCMAlias);
		if (pCM == NULL) return NULL;
		m_pWindow->RegDataOrder(pCM, pDM);
//�������߰�===================================================================
		
/*
		CESL_DataMgr *pDM = static_cast<CESL_Mgr*>(pWindow)->FindDM(strDMAlias);
		if (pDM == NULL) return NULL;

		CMultiSortDlg dlg(pWindow);
		dlg.AddSortInfo(_T("BS_��������"), _T("��������"));
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
	
	else if (strSM == _T("RFID"))
	{
		m_pWindow->GetRegnoTitle();
		//CBO_CAT_REG_RFID RfidRegno;
		//RfidRegno.GetRegnoTitle();
		//CSmartIDScannerDlg dlg;
		//INT ids = dlg.DoModal();
/*
		CBarcodeToRFIDDlg dlg;
		dlg.CONNECTION_INFO = _T("kolas2@kolas2/kolas2#[]");
		CRFIDBookInfo *pBook;

		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = _T("EM0000030180");
			pBook->strTitle = _T("����1987");
		}
		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = _T("EM0000030181");
			pBook->strTitle = _T("�𳪸���");
		}
		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = _T("EM0000030182");
			pBook->strTitle = _T("��");
		}
		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = _T("EM0000030183");
			pBook->strTitle = _T("���� �׷����� ���ڳ�");
		}
		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = _T("EM0000030184");
			pBook->strTitle = _T("����ϴ� �״뿡��");
		}

		dlg.DoModal();
		*/
	}
	return NULL;
}

