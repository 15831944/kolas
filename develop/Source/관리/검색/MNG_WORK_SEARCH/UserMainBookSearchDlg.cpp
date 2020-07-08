// UserMainBookSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserMainBookSearchDlg.h"
#include "UserMainBookDetailSearchDlg.h"
#include "UserMainDetailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserMainBookSearchDlg dialog


CUserMainBookSearchDlg::CUserMainBookSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserMainBookSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pCmdProc = NULL;
	m_bIsRunningThread = FALSE;
	m_bSearchMode = FALSE;

	m_bkR = 224;
	m_bkG = 173;
	m_bkB = 173;
	m_BkBrush.CreateSolidBrush(RGB(m_bkR, m_bkG, m_bkB));
}

CUserMainBookSearchDlg::~CUserMainBookSearchDlg()
{
	if(NULL != m_pCmdProc)
	{
		delete m_pCmdProc;
		m_pCmdProc = NULL;
	}

	if(NULL != m_pUserMainWait)
	{
		delete m_pUserMainWait;
		m_pUserMainWait = NULL;
	}		

	m_BkBrush.DeleteObject();
}

void CUserMainBookSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserMainBookSearchDlg)
	DDX_Control(pDX, IDC_BOOK_SEARCH_stcEX2, m_MarkStaticStaticEX2);
	DDX_Control(pDX, IDC_BOOK_SEARCH_stcResult, m_MarkStaticResult);
	DDX_Control(pDX, IDC_BOOK_SEARCH_stcWhere, m_MarkStaticSWhere);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserMainBookSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CUserMainBookSearchDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BOOK_SEARCH_btnSHOW, OnBOOKSEARCHbtnSHOW)
	ON_BN_CLICKED(IDC_SEARCH_btnSearch2, OnSEARCHbtnSearch2)
	ON_BN_CLICKED(IDC_BOOK_SEARCH_btnDetail, OnBOOKSEARCHbtnDetail)
	ON_BN_CLICKED(IDC_BOOK_SEARCH_btnCallNoReceiptPrint, OnBOOKSEARCHbtnCallNoReceiptPrint)
	ON_BN_CLICKED(IDC_SEARCH_btnReSearch, OnSEARCHbtnReSearch)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SEARCH_btnClear, OnSEARCHbtnClear)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_GRID_DISPLAY, OnGridDisplay)
	ON_MESSAGE(WM_USER_SET_MSG_WHERE, OnSetMsgWhere)
	ON_MESSAGE(WM_USER_SHOWRESEARCH, OnShowReSearchControl)
	ON_MESSAGE(MSG_SHOW_WAITDISPLAY, OnShowWaitDisplay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserMainBookSearchDlg message handlers

BOOL CUserMainBookSearchDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

void CUserMainBookSearchDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// 2013.11.11 UPDATE BY PWR : 2013���������ռ���Ȯ�뱸��װ��� (�ٸ� �޴��� ��Ŀ�� �������� ���� ����)
	if (TRUE == bShow) {
		((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal2))->Invalidate(TRUE);
		((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal2))->SetFocus();
	}
//	((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal2))->Invalidate(TRUE);
//	((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal2))->SetFocus();

//	if(TRUE == bShow)
//	{
//		((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal2))->SetFocus();
//	}
//	else
//	{
//		CESL_DataMgr *pDM = FindDM(_T("DM_USERMAIN_BOOKSEARCH"));
//		pDM->FreeData();
//		GridDisplay(_T(""));
//		ShowReSearchControl(FALSE);
//		ControlClear();
//
//		m_strMsgWhere = _T("");
//		m_strMsgWhereBefore = _T("");
//		m_strBoWhere = _T("");
//		m_strSeWhere = _T("");
//	}
	
}

void CUserMainBookSearchDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_BOOK_SEARCH_gridB_Grid);
	if(pGrid->GetSafeHwnd() == NULL)	return;
	CMSHFlexGrid * pGrid2 = (CMSHFlexGrid*)GetDlgItem(IDC_BOOK_SEARCH_gridS_Grid);
	if(pGrid2->GetSafeHwnd() == NULL)	return;
	CWnd* pBtnReceipt = GetDlgItem(IDC_BOOK_SEARCH_btnCallNoReceiptPrint);
	if(pBtnReceipt->GetSafeHwnd() == NULL)	return;	
	CWnd* pBtnDetail = GetDlgItem(IDC_BOOK_SEARCH_btnDetail);
	if(pBtnDetail->GetSafeHwnd() == NULL)	return;
	CWnd* pStcEX2 = GetDlgItem(IDC_BOOK_SEARCH_stcEX2);
	if(pStcEX2->GetSafeHwnd() == NULL)	return;
	CWnd* pEdtWhere = GetDlgItem(IDC_SEARCH_edtWhere);
	if(pEdtWhere->GetSafeHwnd() == NULL)	return;
	CWnd* pEdtResult = GetDlgItem(IDC_SEARCH_edtResult);
	if(pEdtResult->GetSafeHwnd() == NULL)	return;
	CWnd* pStcGuide = GetDlgItem(IDC_SEARCH_stcGuide);
	if(pStcGuide->GetSafeHwnd() == NULL)	return;
	CWnd* pStcRowCnt = GetDlgItem(IDC_SEARCH_stcRowCnt);
	if(pStcRowCnt->GetSafeHwnd() == NULL)	return;
	CWnd* pEdtRowCnt= GetDlgItem(IDC_SEARCH_edtRowCnt);
	if(pEdtRowCnt->GetSafeHwnd() == NULL)	return;

	CRect btnReceiptRect, stcEX2Rect, gridRect, btnDetailRect , btnReservationRect , rWhere, rResult, rGuide, rDlg, rStcRowCnt, rEdtRowCnt;

	GetWindowRect(rDlg);
	pEdtWhere->GetWindowRect(&rWhere);
	pEdtResult->GetWindowRect(&rResult);
	pStcGuide->GetWindowRect(&rGuide);
	pBtnReceipt->GetWindowRect(&btnReceiptRect);
	pBtnDetail->GetWindowRect(&btnDetailRect);
	pStcEX2->GetWindowRect(&stcEX2Rect);
	pStcRowCnt->GetWindowRect(&rStcRowCnt);
	pEdtRowCnt->GetWindowRect(&rEdtRowCnt);		

	ScreenToClient(rWhere);
	ScreenToClient(rResult);
	ScreenToClient(rGuide);
	ScreenToClient(btnReceiptRect);
	ScreenToClient(btnReservationRect);
	ScreenToClient(btnDetailRect);
	ScreenToClient(stcEX2Rect);
	ScreenToClient(rStcRowCnt);
	ScreenToClient(rEdtRowCnt);

	pEdtWhere->MoveWindow(rWhere.left, rWhere.top, rDlg.Width()-rWhere.left, rWhere.Height());
	pEdtResult->MoveWindow(rResult.left, rResult.top, rDlg.Width()-rResult.left, rResult.Height());
	pGrid->MoveWindow(rGuide.left, rGuide.bottom, rDlg.Width(), rDlg.Height()-(rGuide.bottom+40));
	pGrid2->MoveWindow(rGuide.left, rGuide.bottom, rDlg.Width(), rDlg.Height()-(rGuide.bottom+40));
	pGrid->GetWindowRect(&gridRect);

	ScreenToClient(gridRect);
	pBtnDetail->MoveWindow(btnDetailRect.left, gridRect.bottom+10, btnDetailRect.Width(), btnDetailRect.Height());
	pBtnReceipt->MoveWindow(btnReceiptRect.left, gridRect.bottom+10, btnReceiptRect.Width(), btnReceiptRect.Height());
	pStcEX2->MoveWindow(stcEX2Rect.left, gridRect.bottom+16, stcEX2Rect.Width(), stcEX2Rect.Height());	
	pStcRowCnt->MoveWindow(gridRect.right-rStcRowCnt.Width()-rEdtRowCnt.Width()-10, gridRect.bottom+16, rStcRowCnt.Width(), rStcRowCnt.Height());
	pEdtRowCnt->MoveWindow(gridRect.right-rEdtRowCnt.Width()-5, gridRect.bottom+13, rEdtRowCnt.Width(), rEdtRowCnt.Height());
}

BOOL CUserMainBookSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SM_USERMAIN_BOOKSEARCH")) < 0)
 	{
 		AfxMessageBox(_T("InitESL_Mgr Error!"));
 		return FALSE;
 	}
	
	m_pDM.SetCONNECTION_INFO(_T(""));

	INT ids;
	m_pCmdProc = new CCommonProc(this, &m_pDM);
	
	ids = m_pCmdProc->LoadManageValue();
	if(0 > ids)
	{
		return FALSE;
	}
 
	// û����ȣ��� ��뿩�ο����� ó��
	// 2010.05.14 UPDATE BY PWR : �������˻������� �����ϵ���
	// ������ �˻������� �󼼺��⿡�� ����
// 	if(_T("Y") == m_pCmdProc->m_strReceiptUseYN && _T("0") == m_pCmdProc->m_strSearchMode)
	if(_T("Y") == m_pCmdProc->m_strReceiptUseYN)
	{
		GetDlgItem(IDC_BOOK_SEARCH_btnCallNoReceiptPrint)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BOOK_SEARCH_stcEX2)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_BOOK_SEARCH_btnCallNoReceiptPrint)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BOOK_SEARCH_stcEX2)->ShowWindow(SW_HIDE);
	}
	
	// 2010.05.14 UPDATE BY PWR : ������ ����ϸ� ��� �ʵ� �߰�
 	if(_T("Y") == m_pCmdProc->m_strReceiptUseYN)
 	{
		CString strGridName;
		if(_T("0") == m_pCmdProc->m_strSearchMode)
		{
			strGridName = _T("B_Grid");
		}
		else
		{
			strGridName = _T("S_Grid");
		}
 		CESL_ControlMgr *pCM = FindCM(_T("CM_USERMAIN_BOOKSEARCH_GRID"));
 		if(NULL == pCM){return FALSE;}
 		CESL_Control_Element *pCE;
 		pCE = pCM->FindControlElement(strGridName);
 		if(NULL == pCE){return FALSE;}
 		CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_USERMAIN_BOOKSEARCH_GRID"), strGridName);
 		if(NULL == pGrid){return FALSE;}
 
 		// 16/07/01 ������ : ������ ��±� ������ ������� ��� �����ϵ��� ��� ����
/*// BEFORE ------------------------------------------------------------------------
		CString strLinkData, strInitValue;
 		strLinkData = _T(",") + pCE->LINK_DATA;
 		strInitValue = _T("[���:4:30],") + pCE->INIT_VALUE;
 		pCE->CTRL_STYLE = _T("[Selected:0:V]");
 		pCE->LINK_DATA = strLinkData;
*/// AFTER -------------------------------------------------------------------------
		CString strInitValue;
		strInitValue = pCE->INIT_VALUE;
		strInitValue.Replace(_T("[����:4:30]"),_T("[���:4:30]"));
//*/ END ---------------------------------------------------------------------------
 		pCE->INIT_VALUE = strInitValue;
		pGrid->m_bCheck = TRUE;
		pGrid->FreeColumnInfo();
 		pGrid->SetColumnInfo();
 	}
//  	if(_T("Y") != m_pCmdProc->m_strReceiptUseYN)
//  	{
//  		CESL_ControlMgr *pCM = FindCM(_T("CM_USERMAIN_BOOKSEARCH_GRID"));
//  		CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(_T("B_Grid")));
//  		pGrid->SetColWidth(1,0,0);
//  	}

	GridDisplay(_T(""));

	ShowReSearchControl(FALSE);

	ControlClear();

	m_pUserMainWait = new CUserMainWait(this);
 	m_pUserMainWait->Create(CUserMainWait::IDD, this);

	GetDlgItem(IDC_SEARCH_edtRowCnt)->SetWindowText(SEARCH_MAX_COUNT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CUserMainBookSearchDlg::ControlClear() 
{
	INT ids;
	CESL_ControlMgr *pCM = FindCM(_T("CM_USERMAIN_BOOKSEARCH"));
	if(NULL == pCM)
	{
		return;
	}
	ids = pCM->AllControlClear();
	if(0 > ids)
	{
		return ;
	}
}

INT CUserMainBookSearchDlg::GridDisplay(CString sResult)
{
	INT ids;
	// �ڷ�˻���� (0:å�����˻�, 1:�������˻�)
	if(_T("0") == m_pCmdProc->m_strSearchMode)
	{  // å����
		GetDlgItem(IDC_BOOK_SEARCH_gridB_Grid)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BOOK_SEARCH_gridS_Grid)->ShowWindow(SW_HIDE);
		CESL_Grid *pGrid1 = (CESL_Grid*)GetDlgItem(IDC_BOOK_SEARCH_gridB_Grid);
		pGrid1->m_nHeight = 30;
		ids = ControlDisplay(_T("CM_USERMAIN_BOOKSEARCH_GRID"), _T("B_Grid"));	
	}
	else
	{ // ������
		GetDlgItem(IDC_BOOK_SEARCH_gridB_Grid)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BOOK_SEARCH_gridS_Grid)->ShowWindow(SW_SHOW);
		
		CESL_Grid *pGrid2 = (CESL_Grid*)GetDlgItem(IDC_BOOK_SEARCH_gridS_Grid);
		pGrid2->m_nHeight = 30;
		ids = ControlDisplay(_T("CM_USERMAIN_BOOKSEARCH_GRID"), _T("S_Grid"));
	}
	if(0 > ids)
	{
		return -1;
	}

	GetDlgItem(IDC_SEARCH_edtResult)->SetWindowText(sResult);
	GetDlgItem(IDC_SEARCH_edtWhere)->SetWindowText(m_strMsgWhereView);
	
	return 0;
}

VOID CUserMainBookSearchDlg::ShowReSearchControl(BOOL bShow)
{
	LockWindowUpdate();
	CRect rGuide;
	CWnd* pGuideLine = GetDlgItem(IDC_SEARCH_stcGuide);
	if(TRUE == bShow)
	{
		pGuideLine->GetWindowRect(rGuide);
		ScreenToClient(rGuide);
		rGuide.bottom = 120;
// 		rGuide.bottom = 168;
		pGuideLine->MoveWindow(rGuide);

		// 2009.12.07 DEL BY PWR : ������˻� ����
// 		GetDlgItem(IDC_BOOK_SEARCH_stcDetail21)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_BOOK_SEARCH_stcDetail22)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_SEARCH_edtTotal3)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_SEARCH_btnReSearch)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BOOK_SEARCH_stcWhere)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SEARCH_edtWhere)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BOOK_SEARCH_stcResult)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SEARCH_edtResult)->ShowWindow(SW_SHOW);

		((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal2))->SetWindowText(_T(""));
		((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal3))->SetWindowText(_T(""));
		((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal3))->SetFocus();
	}
	else
	{
		pGuideLine->GetWindowRect(rGuide);
		ScreenToClient(rGuide);
		rGuide.bottom = 60;
		pGuideLine->MoveWindow(rGuide);

		GetDlgItem(IDC_BOOK_SEARCH_stcDetail21)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BOOK_SEARCH_stcDetail22)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SEARCH_edtTotal3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SEARCH_btnReSearch)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BOOK_SEARCH_stcWhere)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SEARCH_edtWhere)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BOOK_SEARCH_stcResult)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SEARCH_edtResult)->ShowWindow(SW_HIDE);

		((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal2))->SetWindowText(_T(""));
		((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal3))->SetWindowText(_T(""));
		((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal2))->SetFocus();
	}
	UnlockWindowUpdate();
 	CRect rc;
 	this->GetClientRect(rc);
 	this->PostMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));

	return;
}

void CUserMainBookSearchDlg::OnBOOKSEARCHbtnSHOW() 
{
	
	GetDlgItem(IDC_SEARCH_edtTotal2)->SetWindowText(_T(""));
	GetDlgItem(IDC_SEARCH_edtTotal3)->SetWindowText(_T(""));

	CRect rDlg, rEdtTotal;
	CWnd* pEdtTotal = GetDlgItem(IDC_SEARCH_edtTotal2);
	pEdtTotal->GetWindowRect(rEdtTotal);
	//ScreenToClient(rEdtTotal);

	SetMaxRowCnt();
	CUserMainBookDetailSearchDlg dlg(this, m_pCmdProc, rEdtTotal);
	if(IDOK == dlg.DoModal())
	{
		((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal3))->SetFocus();
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_SEARCH_edtTotal2))->SetFocus();
	}

	return;	
}

// �ִ���ȸ�Ǽ� ����
VOID CUserMainBookSearchDlg::SetMaxRowCnt()
{
	CString strMaxRowCnt = _T("");	
	GetDlgItem(IDC_SEARCH_edtRowCnt)->GetWindowText(strMaxRowCnt);
	if( _ttoi(strMaxRowCnt) < 1 )
	{
		strMaxRowCnt = SEARCH_MAX_COUNT;	
		GetDlgItem(IDC_SEARCH_edtRowCnt)->SetWindowText(strMaxRowCnt);
	}	
	m_pCmdProc->m_strMaxRowCnt = strMaxRowCnt;
}

LRESULT CUserMainBookSearchDlg::OnGridDisplay(WPARAM wParam, LPARAM lParam)
{
	CString *sResult = (CString*)lParam;
	GridDisplay(*sResult);
	return 0;
}

LRESULT CUserMainBookSearchDlg::OnShowReSearchControl(WPARAM wParam, LPARAM lParam)
{
	BOOL bShow = (BOOL)wParam;
	ShowReSearchControl(bShow);
	return 0;
}

LRESULT CUserMainBookSearchDlg::OnSetMsgWhere(WPARAM wParam, LPARAM lParam)
{
	CString *sWhere = (CString*)lParam;
	m_strMsgWhereView = *sWhere;
	return 0;
}

LRESULT CUserMainBookSearchDlg::OnShowWaitDisplay(WPARAM wParam, LPARAM lParam)
{	
	if(TRUE == wParam)
	{		
		m_pUserMainWait->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pUserMainWait->ShowWindow(SW_HIDE);
	}
	return 0;
}

void CUserMainBookSearchDlg::OnSEARCHbtnSearch2() 
{
	m_bSearchMode = FALSE;
   
   	CWinThread * Thread;
   	Thread = AfxBeginThread(SearchThread, this);		
}

UINT CUserMainBookSearchDlg::SearchThread(LPVOID pParam)
{
	CUserMainBookSearchDlg* pDlg = (CUserMainBookSearchDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread)
	{
		return 0;
	}
	pDlg->m_bIsRunningThread = TRUE;	
	
	pDlg->SendMessage(MSG_SHOW_WAITDISPLAY, TRUE, NULL);
	pDlg->EnableWindow(FALSE);

	pDlg->SearchBookInfo(pDlg->m_bSearchMode);

	pDlg->EnableWindow(TRUE);
	pDlg->SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
	
	pDlg->m_bIsRunningThread = FALSE;

	return 0;
}

INT CUserMainBookSearchDlg::SearchBookInfo(BOOL bReSearch)
{
	INT ids;
	CString sBoWhere,sSeWhere;

	BeginWaitCursor();

	if(TRUE == bReSearch)
	{
		// �������ǻ���
		ids = GetSqlReSearch(sBoWhere, 0);	
		if(0 > ids) 
		{
			EndWaitCursor();
			return -1;
		}
		if(ids > 0 )
		{
			EndWaitCursor();
			return ids;
		}
		// �������ǻ���
		ids = GetSqlReSearch(sSeWhere, 1);	
		if(0 > ids)
		{
			EndWaitCursor();
			return -2;
		}
	}
	else
	{
		// �������ǻ���
		ids = GetSqlSearchALLBookInfo(sBoWhere, 0);	
		if(0 > ids) 
		{
			EndWaitCursor();
			return -1;
		}
		if(ids > 0 )
		{
			EndWaitCursor();
			return ids;
		}
		// �������ǻ���
		ids = GetSqlSearchALLBookInfo(sSeWhere, 1);	
		if(0 > ids)
		{
			EndWaitCursor();
			return -2;
		}

	}
	// �˻�:DM����
	CESL_DataMgr *pDM;
	if(_T("0") == m_pCmdProc->m_strSearchMode)
	{
		pDM = FindDM(_T("DM_USERMAIN_BOOKSEARCH_B_BO"));
		if(NULL == pDM)
		{
			EndWaitCursor();
			return -2;
		}
	}
	else
	{
		pDM = FindDM(_T("DM_USERMAIN_BOOKSEARCH_S_BO"));
		if(NULL == pDM)
		{
			EndWaitCursor();
			return -2;
		}
	}

	CString sPublishForm = _T("��ü");
	CString sResult;
	pDM->FreeData();
	SetMaxRowCnt();
	ids = m_pCmdProc->SendSearchBookInfoSQL(pDM, sPublishForm, sBoWhere, sSeWhere, sResult, bReSearch);
	if(0 > ids)
	{
		EndWaitCursor();
		return -3;
	}
	if(1000 == ids)
	{
		SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
		MessageBox(_T("�ڷᰡ �������� �ʽ��ϴ�. \r\n�˻�� Ȯ���ϼ���."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
		return -1;
	}

	// 16/08/22 ������ : Grid�� �Ѽ����� �÷� �߰�
//*/ ADD -----------------------------------------------------------------------
	CString strTmpData;
	CMarcInfoConversionMgr pMarcConvertMgr;
	pMarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
//*/ END -----------------------------------------------------------------------

//+++++ 2011.9.16 ä��ȣ [�߰�]
// ���� : ����ñ���û ��ɰ������
// �䱸 : �ڷ�ã�⿡�� �����ڷ��� ��� �������� ǥ�õǵ��� ����
	CString sData, sData2;
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		ids = pDM->GetCellData(_T("���������̷°Ǽ�"), i, sData);
		if(0 > ids) { return -12; }

		// 16/08/22 ������ : Grid�� �Ѽ����� �÷� �߰�
//*/ ADD -----------------------------------------------------------------------
		CMarc marc;		
		strTmpData = pDM->GetCellData(_T("BS_MARC"), i);
		if ( 0 <= m_pInfo->pMarcMgr->Decoding(strTmpData, &marc) )
		{
			strTmpData.Empty();	
			ids = pMarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
			if( ids >= 0 )	pDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, i);
		}
//*/ END -----------------------------------------------------------------------

		// �������˻��� ��� '�����-å��' ������ ��� å�� ����Ǿ����� '0'���� ����
		// å�����˻��� ��� ������� ����.
		if ( 
			(_T("0") == m_pCmdProc->m_strSearchMode && sData > _T("0")) ||
			(_T("0") != m_pCmdProc->m_strSearchMode && sData == _T("0")) 
		) 
		{
			sData2.Format(_T("X(������(%s��))"), sData);
			ids = pDM->SetCellData(_T("���Ⱑ�ɿ���"), sData2, i);
			if(0 > ids) { return -15; }

			ids = pDM->GetCellData(_T("�ڷ���¼���"), i, sData);
			if(0 > ids) { return -12; }

			if ( sData != _T("������") ) {
				ids = pDM->SetCellData(_T("�ڷ���¼���"), _T("������"), i);
				if(0 > ids) { return -15; }
			} 
		}
	}
//-----

	m_strMsgWhereView = m_strMsgWhere;

	this->SendMessage(WM_USER_GRID_DISPLAY, 0, (LPARAM)&sResult);
	this->SendMessage(WM_USER_SHOWRESEARCH, TRUE);

	EndWaitCursor();

	return 0;
}

INT CUserMainBookSearchDlg::GetSqlReSearch(CString &sWhere, INT nMode)
{
	CString sGetData;
	GetControlData(_T("CM_USERMAIN_BOOKSEARCH"), _T("������˻�"), sGetData);
	sGetData.TrimRight();
	if(sGetData.IsEmpty())
	{
		SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
		MessageBox(_T("�˻�� �Է��ϼ���."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
		return 1;
	}

	// ���� ���� ����
	if(0 == nMode)
	{
		INT idx;
		idx = m_strMsgWhereView.Find(_T(" �˻���� �� \""));
		if(0 < idx)
		{
			CString sTemp;
			m_strMsgWhereBefore = m_strMsgWhereView.Mid(idx+8);
			m_strMsgWhere = m_strMsgWhereBefore + _T(" �˻���� �� \"");
		}
		else
		{
			m_strMsgWhereBefore = m_strMsgWhereView;
			m_strMsgWhere = m_strMsgWhereBefore + _T(" �˻���� �� \"");
		}
	}

	BOOL bIsEqualSearch;
	CString sEcoSearchWhere;

	bIsEqualSearch = FALSE;
	if('\"' == sGetData.GetAt(0))
	{
		bIsEqualSearch = TRUE;
	}
	
	((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(sGetData);

	if(!bIsEqualSearch) 
	{
		sGetData += _T("*");
	}
	else
	{
		sGetData.Replace(_T("\""), _T(""));
	}
	if(!sEcoSearchWhere.IsEmpty())
	{
		sEcoSearchWhere += _T(" INTERSECT ");
	}
	if(0 == nMode)
	{
		sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_ALLITEM , '") + sGetData + _T("') ");
		m_strMsgWhere += sGetData;
	}
	else
	{
		sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_ALLITEM , '") + sGetData + _T("') ");
	}		

 	// ����
	if(0 == nMode)
	{
		m_strMsgWhere += _T("\"");
	}

 	sWhere = sEcoSearchWhere;

	return 0;
}

INT CUserMainBookSearchDlg::GetSqlSearchALLBookInfo(CString &sWhere, INT nMode)
{
	INT nEcoSearchCnt = 0;
	INT nSearchField = 0;
	CString sEcoSearchWhere;

	CString sGetData;
	GetControlData(_T("CM_USERMAIN_BOOKSEARCH"), _T("��ü_����"), sGetData);
	sGetData.TrimRight();
		
	BOOL bIsEqualSearch;

	if(0 == nMode)
	{
		m_strMsgWhere = _T("\"");
	}
	
	// 4. ��ü
	if(!sGetData.IsEmpty() )
	{
		bIsEqualSearch = FALSE;
		if('\"' == sGetData.GetAt(0))
		{
			bIsEqualSearch = TRUE;
		}
		
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(sGetData);

		if(!bIsEqualSearch ) 
		{
			sGetData += _T("*");
		}
		else
		{
			sGetData.Replace(_T("\""), _T(""));
		}

		if(!sEcoSearchWhere.IsEmpty() )
		{
			sEcoSearchWhere += _T(" INTERSECT ");
		}
		if(0 == nMode)
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_ALLITEM , '") + sGetData + _T("') ");
			if(_T("\"") != m_strMsgWhere)
			{
				m_strMsgWhere += _T(",  ");
			}
			m_strMsgWhere += sGetData;
		}
		else
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_ALLITEM , '") + sGetData + _T("') ");
		}		
		nEcoSearchCnt++;
		nSearchField++;
	}

	// ��������
	CString sManageCode = m_pCmdProc->m_strManageCode;
	if(!sManageCode.IsEmpty())
	{
		if(!sWhere.IsEmpty())
		{
			sWhere += _T(" AND ");
		}
		sWhere += _T("BB.MANAGE_CODE='") + sManageCode + _T("' ");
		if(0 == nMode)
		{
			CString sDesc;
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������"), sManageCode, sDesc);
			if(_T("\"") != m_strMsgWhere)
			{
				m_strMsgWhere += _T(",  ");
			}
			m_strMsgWhere += sDesc;
		}
	}

	// �缭����
	//===================================================
	//2009.10.22 REM BY PJW : �������ڷ�ã��� �缭���ѵ� �˻��ǵ��� �Ѵ�.
//	if(!sWhere.IsEmpty())
//	{
//		sWhere += _T(" AND ");
//	}
//	sWhere += _T(" BB.USE_LIMIT_CODE != 'CL' ");
	//===================================================

	// ECOSEARCH
	if(!sEcoSearchWhere.IsEmpty())
	{
		if(!sWhere.IsEmpty())
		{
			sWhere += _T(" AND ");
		}
		if(1 == nEcoSearchCnt)
		{
			sWhere += _T(" IB.REC_KEY IN ") + sEcoSearchWhere;
		}
		else
		{
			sWhere += _T(" IB.REC_KEY IN ( ") + sEcoSearchWhere + _T(" ) ");
		}
	}

	if(0 == nMode)
	{
		if(_T("\"") == m_strMsgWhere)
		{
			m_strMsgWhere = _T("��ü�˻�");
		}
		else
		{
			m_strMsgWhere += _T("\"");
		}
	}

	// ����Ȯ��
	if(sGetData.IsEmpty())
	{
		SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
		MessageBox(_T("�˻�� �Է��ϼ���."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
		return 1;
	}

	// ������ �ڷ�		
	if(0 == nMode )
	{	
		if(!sWhere.IsEmpty() )
		{
			sWhere += _T(" AND ");
		}
		sWhere += _T(" BB.WORKING_STATUS NOT IN ('BOT211O','BOT212O') ");
	}

	return 0;
}

BOOL CUserMainBookSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{		
		if(VK_RETURN == pMsg->wParam)
		{
			if(GetDlgItem(IDC_SEARCH_edtTotal2)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnSEARCHbtnSearch2();
			}
			else if(GetDlgItem(IDC_SEARCH_edtTotal3)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnSEARCHbtnReSearch();
			}
			else if(GetDlgItem(IDC_SEARCH_btnSearch2)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnSEARCHbtnSearch2();
			}
			else if(GetDlgItem(IDC_BOOK_SEARCH_btnSHOW)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnBOOKSEARCHbtnSHOW();
			}
			else if(GetDlgItem(IDC_SEARCH_btnReSearch)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnSEARCHbtnReSearch();
			}
			else if(GetDlgItem(IDC_BOOK_SEARCH_btnDetail)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnBOOKSEARCHbtnDetail();
			}
			else if(GetDlgItem(IDC_BOOK_SEARCH_btnCallNoReceiptPrint)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnBOOKSEARCHbtnCallNoReceiptPrint();
			}
			
			return TRUE;
		}
		else if(VK_ESCAPE == pMsg->wParam)
		{
			OnSEARCHbtnClear();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CUserMainBookSearchDlg::OnBOOKSEARCHbtnDetail() 
{
	CString sGridAlias;
	// �ڷ�˻���� (0:å�����˻�, 1:�������˻�)
	if(_T("0") == m_pCmdProc->m_strSearchMode)
	{  // å����
		sGridAlias = _T("B_Grid");
	}
	else
	{
		sGridAlias = _T("S_Grid");
	}
	CESL_ControlMgr *pCM = FindCM(_T("CM_USERMAIN_BOOKSEARCH_GRID"));
	if(NULL == pCM) return;
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(NULL == pGrid) return;

	if(0 < pGrid->GetMouseRow())
	{
		ShowBookDetailInfo();
		return ;
	}
}	

void CUserMainBookSearchDlg::OnBOOKSEARCHbtnCallNoReceiptPrint() 
{
	// 2010.05.14 UPDATE BY PWR : �������˻������� �����ϵ���
// 	if(_T("0") != m_pCmdProc->m_strSearchMode)
// 	{
// 		EnableButton(TRUE);
// 		return;
// 	}
	CString strGridName;
	CString strDMName;
	if(_T("0") == m_pCmdProc->m_strSearchMode)
	{
		strGridName = _T("B_Grid");
		strDMName = _T("DM_USERMAIN_BOOKSEARCH_B_BO");
	}
	else
	{
		strGridName = _T("S_Grid");
		strDMName = _T("DM_USERMAIN_BOOKSEARCH_S_BO");
	}
	INT ids;
	CESL_ControlMgr *pCM = FindCM(_T("CM_USERMAIN_BOOKSEARCH_GRID"));
	if(NULL == pCM) 
	{
		return;
	}
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(strGridName));
	if(NULL == pGrid)
	{
		return;
	}
	CESL_DataMgr *pDM = FindDM(strDMName);
	if(NULL == pDM)
	{
		return;
	}
	if(1 > pDM->GetRowCount())
	{
		SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
		MessageBox(_T("�˻��� �ڷᰡ �����ϴ�."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
		return;
	}
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	INT nMode = 0;
	if(0 < nSelectCnt)
	{ // �����Ȱ� ������
		nMode = 1;
	}
	else
	{ // �����Ȱ� ������ 
		nMode = 0;
		nSelectCnt = 1;
	}
	m_pCmdProc->m_CallNoPrnMgr.Init();
	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if(0 == nMode)
		{ // ���������� ���� ���õǾ��ִ� �ڷ�
			nIndex = pGrid->GetIdx();
		}
		else
		{ // �����Ȱ� ������ 
			nIndex = pGrid->SelectGetIdx();	
		}

// 		if(_T("0") == m_pCmdProc->m_strSearchMode)
// 		{
// 			CString strLoanPossible;
// 			pDM->GetCellData(_T("���Ⱑ�ɿ���"), nIndex, strLoanPossible);
// 			if(_T("O") != strLoanPossible)
// 			{
// 				SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
// 				MessageBox(_T("���Ⱑ���� å�� ����� �� �ֽ��ϴ�."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
// 				return;
// 			}
// 		}
 		CString strTitle;
 		CString strAuthor;
 		CString strCallNo;
 		CString strShelfLocCode;
		CString strRegNo;
		pDM->GetCellData(_T("ǥ������"), nIndex, strTitle);
		pDM->GetCellData(_T("�����ڻ���"), nIndex, strAuthor);
		pDM->GetCellData(_T("û����ȣ"), nIndex, strCallNo);
// 		if(strCallNo.IsEmpty())
// 		{
// 			SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
// 			MessageBox(_T("û����ȣ�� ���� �ڷ��Դϴ�."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
// 			return;
// 		}
		pDM->GetCellData(_T("�ڷ���ڵ弳��"), nIndex, strShelfLocCode);
		if(_T("0") == m_pCmdProc->m_strSearchMode)
		{
			pDM->GetCellData(_T("��Ϲ�ȣ"), nIndex, strRegNo);
		}

		ids = m_pCmdProc->m_CallNoPrnMgr.GetReady();
		if(0 != ids)
		{
			SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
			MessageBox(_T("������ ��±���� ���ῡ �����Ͽ����ϴ�."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
			return;
		}
		
		if(_T("0") == m_pCmdProc->m_strSearchMode)
		{
			m_pCmdProc->m_CallNoPrnMgr.Add(strTitle, strAuthor, strCallNo, strShelfLocCode, strRegNo);
		}
		else
		{
			m_pCmdProc->m_CallNoPrnMgr.AddSe(strTitle, strAuthor, strCallNo, strShelfLocCode);
		}
		if(1 == nMode)
		{
			pGrid->SelectGetNext();
		}
	}

	// ���
	m_pCmdProc->m_CallNoPrnMgr.Print();
	
}

void CUserMainBookSearchDlg::OnSEARCHbtnReSearch() 
{
	 	//SearchBookInfo(TRUE);
  	m_bSearchMode = TRUE; 
  
  	CWinThread * Thread;
  	Thread = AfxBeginThread(SearchThread, this);	
}



INT CUserMainBookSearchDlg::ShowBookDetailInfo()
{
	CString sDMAlias;
	if(_T("0") == m_pCmdProc->m_strSearchMode)
	{
		sDMAlias = _T("DM_USERMAIN_BOOKSEARCH_B_BO");
	}
	else
	{
		sDMAlias = _T("DM_USERMAIN_BOOKSEARCH_S_BO");
	}

	CESL_DataMgr* pDM = FindDM(sDMAlias);
	if(NULL == pDM)
	{
		return -1;
	}

	if(0 == pDM->GetRowCount())
	{
		SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
		MessageBox(_T("�󼼺����� ����� �������� �ʽ��ϴ�."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
		return 0;
	}

	CString sGridAlias;
	if(_T("0") == m_pCmdProc->m_strSearchMode)
	{ // å�����˻�
		sGridAlias = _T("B_Grid");
	}
	else
	{ // �������˻�
		sGridAlias = _T("S_Grid");
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_USERMAIN_BOOKSEARCH_GRID"), sGridAlias);
	if(NULL == pGrid)
	{
		return -1;
	}

	INT nStartIndex = pGrid->GetIdx();

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("IDX_KEY") , i , sSpeciesKey );
		sSpeciesKeyList.AddTail( sSpeciesKey );
		asGridRow.Add( i );
	}
	
	CUserMainDetailDlg Dlg(this, m_pCmdProc);
	Dlg.SetPKList(sDMAlias, sSpeciesKeyList, pGrid, asGridRow, nStartIndex);
	if(IDABORT == Dlg.DoModal())
	{
//		Login_Client(this, this);
	}
	
	return 0;
}

HBRUSH CUserMainBookSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_SEARCH_edtTotal2 == pWnd->GetDlgCtrlID())
 	{
 		pDC->SetBkMode(OPAQUE);
 		pDC->SetBkColor(RGB(m_bkR, m_bkG, m_bkB));
 		return m_BkBrush;
 	}

	return hbr;
}

BEGIN_EVENTSINK_MAP(CUserMainBookSearchDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CUserMainBookSearchDlg)
	ON_EVENT(CUserMainBookSearchDlg, IDC_BOOK_SEARCH_gridB_Grid, -601 /* DblClick */, OnDblClickBOOKSEARCHgridBGrid, VTS_NONE)
	ON_EVENT(CUserMainBookSearchDlg, IDC_BOOK_SEARCH_gridS_Grid, -601 /* DblClick */, OnDblClickBOOKSEARCHgridSGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CUserMainBookSearchDlg::OnDblClickBOOKSEARCHgridBGrid() 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_USERMAIN_BOOKSEARCH_GRID"));
	if(NULL == pCM) return;
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(_T("B_Grid")));
	if(NULL == pGrid) return;

	if(0 < pGrid->GetMouseRow())
	{
		ShowBookDetailInfo();
		return ;
	}
	else
	{
		LockWindowUpdate();
		pGrid->m_nHeight = 30;
		pCM->ControlDisplay(_T("B_Grid"));
		UnlockWindowUpdate();
	}
}

void CUserMainBookSearchDlg::OnDblClickBOOKSEARCHgridSGrid() 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_USERMAIN_BOOKSEARCH_GRID"));
	if(NULL == pCM) return;
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(_T("S_Grid")));
	if(NULL == pGrid) return;

	if(0 < pGrid->GetMouseRow())
	{
		ShowBookDetailInfo();
		return ;
	}
	else
	{
		LockWindowUpdate();
		pGrid->m_nHeight = 30;
		pCM->ControlDisplay(_T("S_Grid"));
		UnlockWindowUpdate();
	}
}

void CUserMainBookSearchDlg::OnSEARCHbtnClear() 
{
	CESL_DataMgr *pDM;
	if(_T("0") == m_pCmdProc->m_strSearchMode)
	{
		pDM = FindDM(_T("DM_USERMAIN_BOOKSEARCH_B_BO"));
	}
	else
	{
		pDM = FindDM(_T("DM_USERMAIN_BOOKSEARCH_S_BO"));
	}

	pDM->FreeData();
	GridDisplay(_T(""));
	ShowReSearchControl(FALSE);
	ControlClear();

	m_strMsgWhere = _T("");
	m_strMsgWhereBefore = _T("");
	m_strBoWhere = _T("");
	m_strSeWhere = _T("");
	m_strMsgWhereView = _T("");
}
