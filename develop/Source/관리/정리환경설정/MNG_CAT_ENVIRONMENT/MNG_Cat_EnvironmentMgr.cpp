// MNG_Cat_EnvironmentMgr.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_Cat_EnvironmentMgr.h"
#include "../../../����/����/BO_ACQ_EVIRONMENT_MANAGER/EnvironComboBox_TwoState.h"
#include "../../../����/����/BO_ACQ_EVIRONMENT_MANAGER/EnvironComboBox_MultiState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_Cat_EnvironmentMgr dialog


CMNG_Cat_EnvironmentMgr::CMNG_Cat_EnvironmentMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_Cat_EnvironmentMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_Cat_EnvironmentMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;
}

CMNG_Cat_EnvironmentMgr::~CMNG_Cat_EnvironmentMgr()
{
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
}


VOID CMNG_Cat_EnvironmentMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_Cat_EnvironmentMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_Cat_EnvironmentMgr, CDialog)
	//{{AFX_MSG_MAP(CMNG_Cat_EnvironmentMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_eCAT_MGR, OnKillfocuseCATMGR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_Cat_EnvironmentMgr message handlers

VOID CMNG_Cat_EnvironmentMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

BOOL CMNG_Cat_EnvironmentMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_����ȯ�漳��")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_����ȯ�漳��"));
	m_pDM = FindDM(_T("DM_����ȯ�漳��"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
/*	m_pGrid->m_bGridEdit = TRUE;
	m_pGrid->SELEDITOR_RemoveAllColumns();
	m_pGrid->SELEDITOR_AddColumn(4);
*/	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	

	// ���� �߰� 2005.01.28 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_pEditCtrl = new CEdit();
	m_pEditCtrl->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN, CRect(0, 0, 0, 0), this, IDC_eCAT_MGR);
	m_pEditCtrl->ShowWindow(SW_HIDE);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	SetTimer(0, 500, NULL);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_Cat_EnvironmentMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridCAT_ENVIRONMENT_MGR);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 5, cx-10, cy-10);
	pGrid->MoveWindow(0, 0, cx, cy);
}

BOOL CMNG_Cat_EnvironmentMgr::Init()
{
	CString strQuery;
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	strQuery.Format(_T("GROUP_2='����' AND SYS_SETUP_YN='N' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE );// AND GROUP_3='����'_T(");
	//strQuery.Format(_T("GROUP_2='����' AND SYS_SETUP_YN = 'N'"));// AND GROUP_3='����'_T(");
	//--2008.10.14 UPDATE BY PWR --}}
	INT ids = m_pDM->RefreshDataManager(strQuery);
	m_pDM->SORT(_T("�����׸�"));
	if(ids < 0) return FALSE;
	m_pGrid->Display();

//	CreateComboBoxInGridSell();
	return TRUE;
}

INT CMNG_Cat_EnvironmentMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("������"))
	{
		if(str == _T("Y")) str = _T("�����");
		else if(str == _T("N")) str = _T("������");
	}
	return 0;
}

BOOL CMNG_Cat_EnvironmentMgr::Create(CWnd *pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

VOID CMNG_Cat_EnvironmentMgr::SaveManageData()
{
	INT cnt = m_pDM->GetRowCount();
	m_pDM->StartFrame();
	CString strKey;
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	for(INT i=0 ; i<cnt ; i++)
	{
		strKey = m_pDM->GetCellData(_T("�����׸�KEY"), i);
		m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, i, m_pDM, _T("REC_KEY"), _T("NUMERIC"), strKey);
	}
	INT ids = m_pDM->SendFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�������� ���� ������ �� �����ϴ�."));
		return ;
	}
	ESLAfxMessageBox(_T("������������ �����Ͽ����ϴ�."));
}

BEGIN_EVENTSINK_MAP(CMNG_Cat_EnvironmentMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_Cat_EnvironmentMgr)
	ON_EVENT(CMNG_Cat_EnvironmentMgr, IDC_gridCAT_ENVIRONMENT_MGR, -600 /* Click */, OnClickgridCATENVIRONMENTMGR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_Cat_EnvironmentMgr::OnClickgridCATENVIRONMENTMGR() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return ;
	INT col = m_pGrid->GetCol();
	if(col == 5)
	{
		CreateComboBoxInGridSell();
	}	
}

VOID CMNG_Cat_EnvironmentMgr::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0) {
		KillTimer(0);
		if(!Init())
		{
			ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
			EndDialog(IDCANCEL);
			return ;
		}		
	}	

	CDialog::OnTimer(nIDEvent);
}

VOID CMNG_Cat_EnvironmentMgr::CreateComboBoxInGridSell()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	INT nRow = m_pGrid->GetRow();
	CString strData = m_pDM->GetCellData(_T("������"), nRow - 1);
	CString strElement = m_pDM->GetCellData(_T("�����׸�"), nRow - 1);

	if(strElement == _T("���̺��å��ȣ����"))	//050128_�����߰�
	{
		// ���̺��̸� �޺��ڽ� �ƴ� ����Ʈ �ڽ� ������
		ShowEditBox(m_pGrid);
		return;
	}

	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	
	if(strElement == _T("�谡�ǿ켱����") || strElement == _T("������ȣ�ο����") || strElement == _T("���ڱ�ȣ�������"))
		m_pComboBox = new CEnvironComboBox_MultiState;
	else 
		m_pComboBox = new CEnvironComboBox_TwoState;
	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450);
	if(strElement == _T("�谡�ǿ켱����"))
	{
		CStringArray tmpArray;
		tmpArray.Add(_T("1"));	tmpArray.Add(_T("2"));	tmpArray.Add(_T("3"));
		m_pComboBox->Init(&tmpArray);
	}
	else if(strElement == _T("������ȣ�ο����"))
	{
		CStringArray tmpArray;
		tmpArray.Add(_T("�������")); tmpArray.Add(_T("������")); tmpArray.Add(_T("��ϱ��к�")); tmpArray.Add(_T("��ġ��ȣ��"));
		m_pComboBox->Init(&tmpArray);
	}
	else if(strElement == _T("���ڱ�ȣ�������"))
	{
		CStringArray ArrCmbData;
		ArrCmbData.Add(_T("����ù��"));		ArrCmbData.Add(_T("�����ʼ�"));
		m_pComboBox->Init(&ArrCmbData);
	}
	else
	{
		CStringArray ArrCmbData;
		ArrCmbData.Add(_T("�����"));		ArrCmbData.Add(_T("������"));
		m_pComboBox->Init(&ArrCmbData);
	}
		
	m_pComboBox->SetIndex(nRow-1);
	m_pComboBox->SetDataMgr(m_pDM);
	m_pComboBox->SetParent(m_pGrid);
	m_pComboBox->SetCurSelData(strData);

	m_pComboBox->SetHorizontalExtent(10);
	CFont * pFont = this->GetFont();
	m_pComboBox->SetFont(pFont);
	
	CRect rect,gridRect;
	RECT parentRect;
	m_pGrid->GetWindowRect(&gridRect);
	this->GetClientRect(&parentRect);

	::ClientToScreen(this->m_hWnd,(POINT*)&(parentRect.left));
	

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;
	//rect.left = gridRect.left + (long)((float)m_pGrid->GetCellLeft()/15.0f);
	//rect.top = gridRect.top + (long)((float)m_pGrid->GetCellTop()/15.0f);
	rect.left = (long)((float)m_pGrid->GetCellLeft()/15.0f) - gridRect.left;
	rect.top = (long)((float)m_pGrid->GetCellTop()/15.0f) - gridRect.top;
	rect.right = rect.left + (long)((float)m_pGrid->GetCellWidth()/15.0f);
	rect.bottom = rect.top + (long)((float)m_pGrid->GetCellHeight()/15.0f);

	//ScreenToClient(&rect);
	m_pComboBox->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());

	m_pComboBox->ShowWindow(TRUE);
	m_pComboBox->SetFocus();
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 050128_�����߰� 
INT CMNG_Cat_EnvironmentMgr::ShowEditBox(CESL_Grid *pGrid)
{
	EFS_BEGIN



	m_pEditCtrl->GetSafeHwnd();

	if (pGrid == NULL) return -1;

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();



	if (nRow < 1 || nCol < 1) return -1;

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	// �ؽ�Ʈ ����
	m_pEditCtrl->SetWindowText(pGrid->GetTextMatrix(nRow, nCol));

	m_pEditCtrl->MoveWindow(rcCell);
	m_pEditCtrl->ShowWindow(SW_SHOW);
	m_pEditCtrl->SetFocus();
	m_pEditCtrl->BringWindowToTop();

	
	return 0;

	EFS_END
	return -1;
}

INT CMNG_Cat_EnvironmentMgr::GetCellRect(CESL_Grid *pGrid, CRect &rect)
{
	EFS_BEGIN

	if (pGrid == NULL) return -1;

	CRect rcGrid;
	CRect rcClient;

	pGrid->GetWindowRect(rcGrid);

	GetClientRect(rcClient);

	ClientToScreen((POINT*)&(rcClient.left));

	rcGrid.top -= rcClient.top;
	rcGrid.left -= rcClient.left;

	rect.left	= rcGrid.left	+ (long)((float)pGrid->GetCellLeft() / 15.0f);
	rect.top	= rcGrid.top	+ (long)((float)pGrid->GetCellTop() / 15.0f);
	rect.right	= rect.left		+ (long)((float)pGrid->GetCellWidth() / 15.0f) - 1;
	rect.bottom = rect.top		+ (long)((float)pGrid->GetCellHeight() / 15.0f) - 1;

	return 0;

	EFS_END
	return -1;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




INT CMNG_Cat_EnvironmentMgr::ApplyDataChange(LPCTSTR lpszData)
{
	EFS_BEGIN

	if (lpszData == NULL) return -1;


	if (m_pDM == NULL || m_pGrid == NULL) return -1;

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();

	// �׸��忡 ������ �����Ų��.
	m_pGrid->SetFocus();
	m_pGrid->SetTextMatrix(nRow, nCol, lpszData);
		

	// ������ �޴����� ������ �����Ų��.
	m_pDM->SetCellData(_T("������"), lpszData, nRow - 1);
	
	m_pGrid->Display();

	return 0;

	EFS_END
	return -1;
}

VOID CMNG_Cat_EnvironmentMgr::OnKillfocuseCATMGR() 
{
	SetFocus();

	if (m_pEditCtrl == NULL) return ;
	
	if (!::IsWindowVisible(m_pEditCtrl->GetSafeHwnd())) return ;

	m_pEditCtrl->ShowWindow(SW_HIDE);
			

	CString strData;
	m_pEditCtrl->GetWindowText(strData);


	ApplyDataChange(strData);

	return ;	
}

VOID CMNG_Cat_EnvironmentMgr::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pEditCtrl)
	{
		delete m_pEditCtrl;
		m_pEditCtrl = NULL;
	}	
}

HBRUSH CMNG_Cat_EnvironmentMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
