// BO_CAT_PROC_1000.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_1000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_1000 dialog


CBO_CAT_PROC_1000::CBO_CAT_PROC_1000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_1000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_PROC_1000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_1000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_1000, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_1000)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_1000 message handlers

BOOL CBO_CAT_PROC_1000::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}