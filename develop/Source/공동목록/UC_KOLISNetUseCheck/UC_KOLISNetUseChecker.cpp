// UC_KOLISNetUseChecker.cpp : implementation file
//

#include "stdafx.h"
#include "UC_KOLISNetUseChecker.h"
#include "ESL_DataMgr.h"
#include "ReadCfgFile.h"
#include "KOLISNetMessageDlg.h"

#include "KolisNetService.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// UC_KOLISNetUseChecker

UC_KOLISNetUseChecker::UC_KOLISNetUseChecker(CESL_Mgr* pParent)
: CESL_Mgr(IDD, pParent)	
{
	m_strUserID = _T("");
}

UC_KOLISNetUseChecker::~UC_KOLISNetUseChecker()
{
}


BEGIN_MESSAGE_MAP(UC_KOLISNetUseChecker, CWnd)
	//{{AFX_MSG_MAP(UC_KOLISNetUseChecker)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// UC_KOLISNetUseChecker message handlers

BOOL UC_KOLISNetUseChecker::GetUseFlag(BOOL bMessageShow) 
{
    CESL_DataMgr pDM;
    CString sPath ;
	CReadCfgFile ReadCfg;	
	ReadCfg.ReadCfgFile(sPath);

	pDM.SetCONNECTION_INFO (ReadCfg.m_sConnection) ;
	
	if( m_strUserID.IsEmpty() )
		m_strUserID = GetUserID () ;
	
	//++2008.10.15 UPDATE BY PWR {{++
	// ���սý��� ����
	CString sQuery_libcode ;
	sQuery_libcode.Format (_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE") );   
     
    CString tmp_Lib_Code ;
	pDM.GetOneValueQuery (sQuery_libcode,tmp_Lib_Code) ; 

    if(tmp_Lib_Code.IsEmpty ())
	{
		if ( bMessageShow == TRUE )	AfxMessageBox (_T("������ ��ȣ�� ���� �Ǿ� ���� �ʽ��ϴ�.")) ;
		return FALSE ;
	}	

	CString strFlag;
	if (!CKolisNetService::GetLibraryKolisNetUseFlag(this, tmp_Lib_Code, strFlag))
	{
		if (bMessageShow == TRUE)
		{
			AfxMessageBox(_T("������� ������ ���� �� �� �����ϴ�. �����ڿ��� �����Ͻʽÿ�."));
		}
		return FALSE;
	}

	if (strFlag.CompareNoCase(_T("N")) == 0 || strFlag.IsEmpty())
	{
		if (bMessageShow == TRUE)
		{
			CKOLISNetMessageDlg dlg;
			CString strMsg;
			strMsg.Format(_T("��������ȣ [%s]�� �ش��ϴ� ������ ������ �����ϴ�.\n���Ϳ� �����Ͻʽÿ�."), tmp_Lib_Code);
			dlg.SetMsg(strMsg);		
			dlg.DoModal();
		}
		return FALSE; 
	}

	return TRUE;
}
