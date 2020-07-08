// BO_CAT_GROUP_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_GROUP_MAIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_GROUP_MAIN dialog


CBO_CAT_GROUP_MAIN::CBO_CAT_GROUP_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_GROUP_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_GROUP_MAIN::~CBO_CAT_GROUP_MAIN()
{
	

}

BOOL CBO_CAT_GROUP_MAIN::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}
VOID CBO_CAT_GROUP_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_GROUP_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CBO_CAT_GROUP_MAIN, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_GROUP_MAIN)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_GROUP_MAIN message handlers

BOOL CBO_CAT_GROUP_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_����_����")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	CESL_Grid *pGrid;
	pGrid = static_cast<CESL_Grid*>(FindControl(_T("CM_����_����_����_����"), _T("MAIN_GRID")));
	if (pGrid == NULL) return FALSE;
	pGrid->InitPopupMenuItem();
	pGrid->AddPopupMenuItem(static_cast<CView*>(pMain), this->VIEW_STYLE);

	//========================================================
	//20080613 ADD BY PJW
	CString strMsg;
	strMsg = _T("���� �ڷḦ �����մϴ�.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	strMsg.ReleaseBuffer();
	//========================================================

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_GROUP_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(0, 35, cx, cy-31);	
	
}

INT CBO_CAT_GROUP_MAIN::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_�ڷ����_�ڵ�")) {
            if ( str == _T("BOR111N")) {
				str = _T("����ΰ��ڷ�");
			}
			else if ( str == _T("BOR112N") ) {
				str = _T("����ڷ�");
			}
			else if ( str == _T("BOR113O") ) {
				str = _T("�������ڷ�");
			}

			CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
			if (pDM)
			{
				pDM->SetCellData(_T("BB_�ڷ����_����"), str, nRow);
			}
        }
    }
	return 0;

EFS_END
return -1;

}

HBRUSH CBO_CAT_GROUP_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

