// MngLocMgr09Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr09Dlg.h"
#include "ChangeValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr09Dlg dialog


CMngLocMgr09Dlg::CMngLocMgr09Dlg(CESL_Mgr* pParent , CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr09Dlg)
	//}}AFX_DATA_INIT
	m_pLoanShareManager = pLoanShareManager;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pEditCtrl = NULL;

}
CMngLocMgr09Dlg::~CMngLocMgr09Dlg()
{
	if (m_pEditCtrl)
	{
		delete m_pEditCtrl;
		m_pEditCtrl = NULL;
	}
}

void CMngLocMgr09Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr09Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngLocMgr09Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr09Dlg)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_eMNG09_LOANSTOPDAY, OnKillfocuseMNG09LOANSTOPDAY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr09Dlg message handlers

BOOL CMngLocMgr09Dlg::Create(CWnd* pParentWnd) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
	return FALSE;
}
INT CMngLocMgr09Dlg::ViewScreen()
{
EFS_BEGIN	

	INT ids;
	const int nCnt = 1;
	// ����
	CString sSection[nCnt] = 
			{	_T("��������") };
	// ���
	CString sAbility[nCnt] = 
			{	_T("����������") };
	// ����
	CString sExplain[nCnt] = 
			{	_T("���δ��⿹�����ϰ� ���ุ������ ����Ǹ� �ο��Ǵ� ���������� ����") };
	// ������
	CString sManage[nCnt] = 
			{	_T("���δ���������") };

	// Display
	m_pDM->FreeData();
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData( _T("����"), sSection[i], i ); 
		m_pDM->SetCellData( _T("���"), sAbility[i], i );
		m_pDM->SetCellData( _T("����"), sExplain[i], i );

		CString sGetData;
		ids = this->GetManageValue( _T("����"), _T("����"), _T("�����ڷ����"), sManage[i], sGetData);
		if( ids < 0 ) 
		{
			ESLAfxMessageBox(_T("������������ �������µ� �����Ͽ����ϴ�."));
			return -1;
		}
		m_pDM->SetCellData( _T("������"), sGetData, i );
	}
	m_pCM->AllControlDisplay();

	return 0;
	/*
	CString strData = "";
	INT ids = 0;
 
	ids = this->GetManageValue( "����", "����", "�����ڷ����", "���δ���������", strData );
	if(ids<0) return -1;
	
	((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(strData);
		
	return 0;
	*/

EFS_END
	return -1;
}

INT CMngLocMgr09Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	const int nCnt = 1;

	// ������
	CString sManage[nCnt] = 
			{	_T("���δ���������") };

	CString sCode;
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->GetCellData( _T("������"), i, sCode );
		ids = this->SetManageValue( _T("����"), _T("����"), _T("�����ڷ����"), sManage[i], sCode );
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("������������ �����ϴµ� �����Ͽ����ϴ�."));
			return -1;
		}
	}
	return 0;
	/*
	CString	strData;
	INT		ids;
	
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(strData);

	ids = this->SetManageValue( "����", "����", "�����ڷ����", "���δ���������", strData );
	if(ids<0) return -2;

	return 0;
	*/
EFS_END
	return -1;	
}

BOOL CMngLocMgr09Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( InitESL_Mgr(_T("SM_����ȯ�漳��_���δ��⿹�༳��")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_����ȯ�漳��_���δ��⿹�༳��"));
	m_pDM = FindDM(_T("DM_����ȯ�漳��_���δ��⿹�༳��"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	m_pEditCtrl = new CEdit();
	m_pEditCtrl->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_eMNG09_LOANSTOPDAY);
	m_pEditCtrl->ShowWindow(SW_HIDE);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CMngLocMgr09Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CMngLocMgr09Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_09);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);	
}

BEGIN_EVENTSINK_MAP(CMngLocMgr09Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr09Dlg)
	ON_EVENT(CMngLocMgr09Dlg, IDC_gridMNG_LOC_MGR_09, -600 /* Click */, OnClickgridMNGLOCMGR09, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMngLocMgr09Dlg::OnClickgridMNGLOCMGR09() 
{
	if( m_pGrid->GetMouseRow() == 0 ) return;
	INT col = m_pGrid->GetCol();
	if( col == 4 ) CreateComboBoxInGridSell();	
}

// ** �޺��ڽ�(����Ʈ�ڽ�) ���� �� �� ����
VOID CMngLocMgr09Dlg::CreateComboBoxInGridSell()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	// ���̺��̸� �޺��ڽ� �ƴ� ����Ʈ �ڽ� ������
	ShowEditBox(m_pGrid);
}

// ** ����Ʈ�ڽ� ų��Ŀ���̺�Ʈ
void CMngLocMgr09Dlg::OnKillfocuseMNG09LOANSTOPDAY() 
{
	SetFocus();

	if (m_pEditCtrl == NULL) return ;
	
	if (!::IsWindowVisible(m_pEditCtrl->GetSafeHwnd())) return ;

	m_pEditCtrl->ShowWindow(SW_HIDE);
			

	CString strData;
	m_pEditCtrl->GetWindowText(strData);


	ApplyDataChange(strData);
}

// ** �Է��� �������� ���� DM����
INT CMngLocMgr09Dlg::ApplyDataChange(LPCTSTR lpszData)
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

// ** �����Ҽ��ִ� ����Ʈ�ڽ� Ȱ��ȭ
INT CMngLocMgr09Dlg::ShowEditBox(CESL_Grid *pGrid)
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
	CString sValue = pGrid->GetTextMatrix(nRow, nCol);
	sValue.Replace( _T("��"), _T("") );
	m_pEditCtrl->SetWindowText(sValue);

	m_pEditCtrl->MoveWindow(rcCell);
	m_pEditCtrl->ShowWindow(SW_SHOW);
	m_pEditCtrl->SetFocus();
	m_pEditCtrl->BringWindowToTop();
	
	return 0;

EFS_END
return -1;
}

// ** ����Ʈ�ڽ� ��ġ����
INT CMngLocMgr09Dlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
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

INT CMngLocMgr09Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("������"))
	{
		CString sAbility;
		m_pDM->GetCellData( _T("���"), nRow, sAbility );

		// �ش����� �������� �ڵ带 �������� �����Ѵ�
		CChangeValue *pMng = new CChangeValue();
		str = pMng->CodeToDesc( sAbility, str );

		delete pMng;
		pMng = NULL;
	}
	return 0;
}
