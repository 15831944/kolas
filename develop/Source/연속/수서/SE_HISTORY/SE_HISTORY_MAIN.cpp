// SE_HISTORY_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "SE_HISTORY_MAIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_HISTORY_MAIN dialog


CSE_HISTORY_MAIN::CSE_HISTORY_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_HISTORY_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_HISTORY_MAIN::~CSE_HISTORY_MAIN()
{
}


VOID CSE_HISTORY_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_HISTORY_MAIN)
	DDX_Control(pDX, IDC_TAB, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_HISTORY_MAIN, CDialog)
	//{{AFX_MSG_MAP(CSE_HISTORY_MAIN)
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_BN_CLICKED(IDC_CHAGE_CURRENT, OnChageCurrent)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(MSG_VIEW_GRID,VIEW_GRID)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_HISTORY_MAIN message handlers

BOOL CSE_HISTORY_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_���̷º���")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	



	//TAB ���� 
	TC_ITEM item;
	item.mask = TCIF_TEXT;
	//���� 
	item.pszText = _T("����");
	m_tab.InsertItem(0,&item);
	//������
	item.pszText = _T("������");
	m_tab.InsertItem(1,&item);
	//��ü
	item.pszText = _T("��ü");
	m_tab.InsertItem(2,&item);
	//�����
	item.pszText = _T("�����");
	m_tab.InsertItem(3,&item);	


	m_TabIndex = 0;


	//VIEW ���� �� ���� 
	pView = new CSE_HISTORY_VIEW();
	pView->pParentWnd = (CWnd*)this;

	CRect rect(10,10,735,325);

	pView->Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,this,NULL,NULL);
	
	SetTimer(0,500,NULL);
	
	//�׸��� ����
	SetGrid();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_HISTORY_MAIN::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(0);

	pView->sCurrentKey = sCurrentKey;
	pView->sCurrentTitle = sCurrentTitle;
	pView->sOriginalKey = sCurrentKey;


	pView->pPrevDM	= FindDM(_T("DM_����_���̷º���_��������"));
	pView->pNextDM	= FindDM(_T("DM_����_���̷º���_�ļ�����"));

	pView->DrawData();
	LoadGridData(0);

	CDialog::OnTimer(nIDEvent);
}

VOID CSE_HISTORY_MAIN::SetGrid()
{
	CMSHFlexGrid *pGrid1 = (CMSHFlexGrid*)GetDlgItem(IDC_GRID1);
	if (pGrid1->GetSafeHwnd() == NULL) return;

	CMSHFlexGrid *pGrid2 = (CMSHFlexGrid*)GetDlgItem(IDC_GRID2);
	if (pGrid2->GetSafeHwnd() == NULL) return;

	CMSHFlexGrid *pGrid3 = (CMSHFlexGrid*)GetDlgItem(IDC_GRID3);
	if (pGrid3->GetSafeHwnd() == NULL) return;

	CMSHFlexGrid *pGrid4 = (CMSHFlexGrid*)GetDlgItem(IDC_GRID4);
	if (pGrid4->GetSafeHwnd() == NULL) return;

	pGrid2->ShowWindow(FALSE);
	pGrid3->ShowWindow(FALSE);
	pGrid4->ShowWindow(FALSE);

	CRect rect;
	pGrid1->GetWindowRect(rect);
	ScreenToClient(rect);

	pGrid2->MoveWindow(rect);
	pGrid3->MoveWindow(rect);
	pGrid4->MoveWindow(rect);

}

VOID CSE_HISTORY_MAIN::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMSHFlexGrid *pGrid1 = (CMSHFlexGrid*)GetDlgItem(IDC_GRID1);
	if (pGrid1->GetSafeHwnd() == NULL) return;

	CMSHFlexGrid *pGrid2 = (CMSHFlexGrid*)GetDlgItem(IDC_GRID2);
	if (pGrid2->GetSafeHwnd() == NULL) return;

	CMSHFlexGrid *pGrid3 = (CMSHFlexGrid*)GetDlgItem(IDC_GRID3);
	if (pGrid3->GetSafeHwnd() == NULL) return;

	CMSHFlexGrid *pGrid4 = (CMSHFlexGrid*)GetDlgItem(IDC_GRID4);
	if (pGrid4->GetSafeHwnd() == NULL) return;

	INT m_TabIndex = m_tab.GetCurSel();
	
	if(0==m_TabIndex)
	{
		pGrid1->ShowWindow(TRUE);
		pGrid2->ShowWindow(FALSE);
		pGrid3->ShowWindow(FALSE);
		pGrid4->ShowWindow(FALSE);
	}
	if(1==m_TabIndex)
	{
		pGrid1->ShowWindow(FALSE);
		pGrid2->ShowWindow(TRUE);
		pGrid3->ShowWindow(FALSE);
		pGrid4->ShowWindow(FALSE);
	}
	if(2==m_TabIndex)
	{
		pGrid1->ShowWindow(FALSE);
		pGrid2->ShowWindow(FALSE);
		pGrid3->ShowWindow(TRUE);
		pGrid4->ShowWindow(FALSE);
	}
	if(3==m_TabIndex)
	{
		pGrid1->ShowWindow(FALSE);
		pGrid2->ShowWindow(FALSE);
		pGrid3->ShowWindow(FALSE);
		pGrid4->ShowWindow(TRUE);
	}

	LoadGridData(m_TabIndex);

	*pResult = 0;
}

VOID CSE_HISTORY_MAIN::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

VOID CSE_HISTORY_MAIN::LoadGridData(INT nIndex)
{

	CString strQry;
	CESL_DataMgr *pDM;
	CESL_Grid * pGrid;

	pDM = FindDM(_T("DM_����_���̷º���"));

	if(nIndex == 0)
	{
		strQry = _T(" TYPE = '1' AND SPECIES_KEY=")+pView->sCurrentKey;
		pDM->RefreshDataManager(strQry);
		pGrid = ( CESL_Grid * )FindControl(_T("CM_����_���̷º���"), _T("GRID_����"));

	}
	if(nIndex == 1)
	{
		strQry = _T(" TYPE = '2' AND SPECIES_KEY=")+pView->sCurrentKey;
		pDM->RefreshDataManager(strQry);
		pGrid = ( CESL_Grid * )FindControl(_T("CM_����_���̷º���"), _T("GRID_������"));
	}
	if(nIndex == 2)
	{
		strQry = _T(" TYPE = '3' AND SPECIES_KEY=")+pView->sCurrentKey;
		pDM->RefreshDataManager(strQry);
		pGrid = ( CESL_Grid * )FindControl(_T("CM_����_���̷º���"), _T("GRID_��ü"));
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM,_T("��ü����"),_T("��������"));
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM,_T("��ü����"),_T("��������"));
	}
	if(nIndex == 3)
	{
		strQry = _T(" TYPE = '4' AND SPECIES_KEY=")+pView->sCurrentKey;
		pDM->RefreshDataManager(strQry);
		pGrid = ( CESL_Grid * )FindControl(_T("CM_����_���̷º���"), _T("GRID_�����"));
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM,_T("������ڵ�"),_T("��������"));
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM,_T("������ڵ�"),_T("��������"));
	}


	pGrid->Display();
}

VOID CSE_HISTORY_MAIN::OnChageCurrent() 
{
	// TODO: Add your control notification handler code here
	if ( pView->sCurrentKey == pView->sParentKey)
	{
		return;	
	}

	pView->UnDrawData();	
	pView->DrawData();
	pView->m_CurObject = NULL;
}

VOID CSE_HISTORY_MAIN::VIEW_GRID( LPARAM l ,WPARAM w)
{
	LoadGridData(m_TabIndex);
}

HBRUSH CSE_HISTORY_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
