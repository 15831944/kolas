// BO_CAT_HANJA_WORD_0000.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_HANJA_WORD_0000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_HANJA_WORD_0000 dialog


CBO_CAT_HANJA_WORD_0000::CBO_CAT_HANJA_WORD_0000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_HANJA_WORD_0000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_HANJA_WORD_0000::~CBO_CAT_HANJA_WORD_0000()
{
}

VOID CBO_CAT_HANJA_WORD_0000::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_HANJA_WORD_0000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_HANJA_WORD_0000, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_HANJA_WORD_0000)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_HANJA_WORD_0000 message handlers

BOOL CBO_CAT_HANJA_WORD_0000::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}
VOID CBO_CAT_HANJA_WORD_0000::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

//	CButton *pButton = (CButton*)GetDlgItem(IDC_bDELETE);
//	if (pButton->GetSafeHwnd() == NULL) return;
	
//	pButton->SetWindowPos(NULL, cx/2, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	pGrid->MoveWindow(0, 0, cx, cy);
	

EFS_END
}

BOOL CBO_CAT_HANJA_WORD_0000::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_MNG_HANJA")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

INT CBO_CAT_HANJA_WORD_0000::Delete()
{
EFS_BEGIN

	//CESL_DataMgr *pCM;
	CESL_ControlMgr *pCM;
	pCM = FindCM( _T("CM_BO_CAT_MNG_HANJA") );
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_MNG_HANJA ERROR") );
		return -1;
	}

	SelectMakeList(_T("CM_BO_CAT_MNG_HANJA"), _T("GRD_HANJA"));
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_MNG_HANJA"), _T("GRD_HANJA"));
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		return -1;
	}

	if (MessageBox(_T("������ �ڷḦ �����Ͻðڽ��ϱ�?"), _T("���� Ȯ��"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_MNG_HANJA"));
		pDM->StartFrame();

		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//Ű ����

			pDM->MakeDeleteFrame(_T("CO_HANJA_WORD_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_MNG_HANJA"), _T("GRD_HANJA"));			
		}		
		while (idx >= 0);
/*
		sAccRecKey = sAccRecKeyTmp.Mid(1);
		CString sQuery;
		sQuery.Format( _T("DELETE FROM CO_HANJA_WORD_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = pDM->AddFrame(sQuery);
*/
		INT ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
			return -1;
		}
		pDM->EndFrame();
	}
	AllControlDisplay(_T("CM_BO_CAT_MNG_HANJA"));

	return 0;

EFS_END
return -1;

}
HBRUSH CBO_CAT_HANJA_WORD_0000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
