// MngLocMgr10Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr10Dlg.h"
#include "ChangeValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr10Dlg dialog


CMngLocMgr10Dlg::CMngLocMgr10Dlg(CESL_Mgr* pParent , CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr10Dlg)
	//}}AFX_DATA_INIT
	m_pLoanShareManager = pLoanShareManager;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;
	m_pEditCtrl = NULL;
	// 16/10/11 ������ : ����>����>����ȯ�漳��>����ȯ�漳�� �޴� ���� �� ����� �޸𸮴��� ���� ����
//*/ ADD ---------------------------------------------------------------------------
	m_pEditCtrl1 = NULL;
//*/ END ---------------------------------------------------------------------------
}
CMngLocMgr10Dlg::~CMngLocMgr10Dlg()
{
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	if(m_pEditCtrl)
	{
		delete m_pEditCtrl;
		m_pEditCtrl = NULL;
	}
	// 16/10/11 ������ : ����>����>����ȯ�漳��>����ȯ�漳�� �޴� ���� �� ����� �޸𸮴��� ���� ����
//*/ ADD ---------------------------------------------------------------------------
	if(m_pEditCtrl1)
	{
		delete m_pEditCtrl1;
		m_pEditCtrl1 = NULL;
	}
//*/ END ---------------------------------------------------------------------------
	
}

void CMngLocMgr10Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr10Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngLocMgr10Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr10Dlg)
		ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_eMNG10_BASIC_ARREAR, OnKillfocuseMNG10BASICARREAR)
	ON_EN_KILLFOCUS(IDC_eMNG10_MAX_LOAN_STOP_DAY, OnKillfocuseMNG10MAXLOANSTOPDAY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr10Dlg message handlers

BOOL CMngLocMgr10Dlg::Create(CWnd* pParentWnd) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
	return FALSE;
}
INT CMngLocMgr10Dlg::ViewScreen()
{
EFS_BEGIN	

	INT ids;
	// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 4;
*///AFTER------------------------------------------------------------------------------
	const int nCnt = 5;
/*//END-------------------------------------------------------------------------------*/
	// ����
	CString sSection[nCnt] = 
			{	_T("��ü��"), _T("����������"), _T("����������"), _T("����������") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
				, _T("��ü����")
//*/ END ---------------------------------------------------------------------------
			};
	// ���
	CString sAbility[nCnt] = 
			{	_T("�⺻ ��ü��"), 
				_T("�ηϿ�ü�� ���������� �ο�"),
				_T("���������� �ް��� ��������"),
				_T("�ִ���������ο��ϼ�") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
			   , _T("��ü�Ⱓ �ް��� ��������") 
//*/ END ---------------------------------------------------------------------------
			};
	// ����
	CString sExplain[nCnt] = 
			{	_T("��ü�� �Ϸ�� �����ؾ� �ϴ� �ݾ�"), 
				_T("�η��ڷᰡ ��ü�Ǿ��� ��� ������������ �ο����� ����"),
				_T("���������Ͽ� �ް����� �����Ѵ�"),
				_T("���������� �ο��� �ο��� �� �ִ� �ִ��ϼ� (0��:���Ѿ���)") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
			   , _T("��ü�Ͽ� �ް����� ������ �Ⱓ�� �����Ѵ�") 
//*/ END ---------------------------------------------------------------------------
			};
	// Alias
	CString sAlias[nCnt] = 
			{	_T("��ü����"), _T("�ηϰ���"), _T("��ü����"), _T("��ü����") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
				, _T("��ü����")
//*/ END ---------------------------------------------------------------------------
			};
	// ������
	CString sManage[nCnt] = 
			{	_T("��ü��"),
				_T("�ηϿ�ü���뿩��"),
				_T("���������ް�����������"),
				_T("�ִ���������ο��ϼ�") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
			   , _T("��ü�Ⱓ�ް�����������") 
//*/ END ---------------------------------------------------------------------------
			};

	// Display
	m_pDM->FreeData();
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData( _T("����"), sSection[i], i ); 
		m_pDM->SetCellData( _T("���"), sAbility[i], i );
		m_pDM->SetCellData( _T("����"), sExplain[i], i );

		CString sGetData;
		ids = this->GetManageValue( _T("����"), _T("����"), sAlias[i], sManage[i], sGetData);
		if( ids < 0 ) 
		{
			ESLAfxMessageBox(_T("������������ �������µ� �����Ͽ����ϴ�."));
			return -1;
		}

		m_pDM->SetCellData( _T("������"), sGetData, i );
	}
	m_pCM->AllControlDisplay();
	/*
	CString sBasicArrear;
	CString sSupplement;
	CString sHolyday;
	INT ids = 0;
 
	ids = this->GetManageValue( "����", "����", "��ü����", "��ü��", sBasicArrear );
	if(ids<0) return -1;
	ids = this->GetManageValue( "����", "����", "�ηϰ���", "�ηϿ�ü���뿩��", sSupplement );
	if(ids<0) return -2;
	ids = this->GetManageValue( "����", "����", "��ü����", "���������ް�����������", sHolyday );
	if(ids<0) return -3;

	((CEdit*)GetDlgItem(IDC_edtBASIC_ARREAR))->SetWindowText( sBasicArrear );
	
	if( sSupplement == "Y")
	{
		((CButton*)GetDlgItem(IDC_radSUPPLEMENT_Y))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_radSUPPLEMENT_N))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_radSUPPLEMENT_Y))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_radSUPPLEMENT_N))->SetCheck(1);
	}

	if( sHolyday == "Y" )
	{
		((CButton*)GetDlgItem(IDC_radHOLYDAY_Y))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_radHOLYDAY_N))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_radHOLYDAY_Y))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_radHOLYDAY_N))->SetCheck(1);
	}
	*/
		
	return 0;

EFS_END
	return -1;
}

INT CMngLocMgr10Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
/*//BEFORE-----------------------------------------------------------------------------
	const int nCnt = 4;
*///AFTER------------------------------------------------------------------------------
	const int nCnt = 5;
/*//END-------------------------------------------------------------------------------*/
	
	// Alias
	CString sAlias[nCnt] = 
			{	_T("��ü����"), _T("�ηϰ���"), _T("��ü����"), _T("��ü����") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
				, _T("��ü����")
//*/ END ---------------------------------------------------------------------------
			};
	// ������
	CString sManage[nCnt] = 
			{	_T("��ü��"),
				_T("�ηϿ�ü���뿩��"),
				_T("���������ް�����������"),
				_T("�ִ���������ο��ϼ�") 
				// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
				, _T("��ü�Ⱓ�ް�����������")
//*/ END ---------------------------------------------------------------------------
			};

	CString sCode;
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->GetCellData( _T("������"), i, sCode );
		ids = this->SetManageValue( _T("����"), _T("����"), sAlias[i], sManage[i], sCode );
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("������������ �����ϴµ� �����Ͽ����ϴ�."));
			return -1;
		}
	}
	/*
	CString sBasicArrear;
	CString sSupplement;
	CString sHolyday;
	INT		ids;
	
	((CEdit*)GetDlgItem(IDC_edtBASIC_ARREAR))->GetWindowText(sBasicArrear);

	if( ((CButton*)GetDlgItem(IDC_radSUPPLEMENT_Y))->GetCheck())
		sSupplement = "Y";
	else
		sSupplement = "N";

	if( ((CButton*)GetDlgItem(IDC_radHOLYDAY_Y))->GetCheck() )
		sHolyday = "Y";
	else
		sHolyday = "N";

	ids = this->SetManageValue( "����", "����", "��ü����", "��ü��", sBasicArrear );
	if(ids<0) return -1;
	ids = this->SetManageValue( "����", "����", "�ηϰ���", "�ηϿ�ü���뿩��", sSupplement );
	if(ids<0) return -2;
	ids = this->SetManageValue( "����", "����", "��ü����", "���������ް�����������", sHolyday );
	if(ids<0) return -3;
	*/
	return 0;

EFS_END
	return -1;	
}

BOOL CMngLocMgr10Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_����ȯ�漳��_���������")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_����ȯ�漳��_���������"));
	m_pDM = FindDM(_T("DM_����ȯ�漳��_���������"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	// �����������
	m_pEditCtrl = new CEdit();
	m_pEditCtrl->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_eMNG10_BASIC_ARREAR);
	m_pEditCtrl->ShowWindow(SW_HIDE);
	// �ִ���������ο��ϼ�
	m_pEditCtrl1 = new CEdit();
	m_pEditCtrl1->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, IDC_eMNG10_MAX_LOAN_STOP_DAY);
	m_pEditCtrl1->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE; 
}

HBRUSH CMngLocMgr10Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CMngLocMgr10Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_10);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);	
}

void CMngLocMgr10Dlg::OnKillfocuseMNG10BASICARREAR() 
{
	SetFocus();
	if (m_pEditCtrl == NULL) return ;
	if (!::IsWindowVisible(m_pEditCtrl->GetSafeHwnd())) return ;

	m_pEditCtrl->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

void CMngLocMgr10Dlg::OnKillfocuseMNG10MAXLOANSTOPDAY() 
{
	SetFocus();
	if (m_pEditCtrl1 == NULL) return ;
	if (!::IsWindowVisible(m_pEditCtrl1->GetSafeHwnd())) return ;

	m_pEditCtrl1->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl1->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}

BEGIN_EVENTSINK_MAP(CMngLocMgr10Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr10Dlg)
	ON_EVENT(CMngLocMgr10Dlg, IDC_gridMNG_LOC_MGR_10, -600 /* Click */, OnClickgridMNGLOCMGR10, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMngLocMgr10Dlg::OnClickgridMNGLOCMGR10() 
{
	if( m_pGrid->GetMouseRow() == 0 ) return;
	INT col = m_pGrid->GetCol();
	if( col == 4 ) CreateComboBoxInGridSell();
}

// ** �޺��ڽ� ���� �� �� ����
VOID CMngLocMgr10Dlg::CreateComboBoxInGridSell()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	INT nRow = m_pGrid->GetRow();
	CString strData = m_pDM->GetCellData( _T("������"), nRow - 1 );
	CString strElement = m_pDM->GetCellData( _T("���"), nRow - 1 );

	if( m_pComboBox )
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	
	CStringArray ArrCmbData;	ArrCmbData.RemoveAll();

	// 1:�⺻��ü��
	if( nRow == 1 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl);
		return;
	}
	// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
/*//BEFORE-----------------------------------------------------------------------------
	// 2:�ηϿ�ü���뿩��, 3:���������ް�����������
	else if( nRow == 2 || nRow == 3 )
*///AFTER------------------------------------------------------------------------------
	// 2:�ηϿ�ü���뿩��, 3:���������ް�����������, 5:��ü�Ⱓ�ް�����������
	else if( nRow == 2 || nRow == 3 || nRow == 5 )
/*//END-------------------------------------------------------------------------------*/	
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("�����"));
		ArrCmbData.Add(_T("������"));
	}
	// 4:�ִ���������ο��ϼ�
	else if( nRow == 4 )
	{
		ShowEditBox(m_pGrid, m_pEditCtrl1);
		return;
	}
	else
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("������"));
		ArrCmbData.Add(_T("�������"));
	}

	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450);
	m_pComboBox->Init(&ArrCmbData);
	m_pComboBox->InitMngLocMgr(10);
	m_pComboBox->SetIndex(nRow-1);
	m_pComboBox->SetDataMgr(m_pDM, m_pGrid);
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
	rect.left = (long)((float)m_pGrid->GetCellLeft()/15.0f) - gridRect.left;
	rect.top = (long)((float)m_pGrid->GetCellTop()/15.0f) - gridRect.top;
	rect.right = rect.left + (long)((float)m_pGrid->GetCellWidth()/15.0f);
	rect.bottom = rect.top + (long)((float)m_pGrid->GetCellHeight()/15.0f);

	m_pComboBox->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	m_pComboBox->ShowWindow(TRUE);
	m_pComboBox->SetFocus();
}

INT CMngLocMgr10Dlg::ShowEditBox(CESL_Grid *pGrid, CEdit* pEditCtrl)
{
EFS_BEGIN

	pEditCtrl->GetSafeHwnd();

	if (pGrid == NULL) return -1;

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if (nRow < 1 || nCol < 1) return -1;

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	// �ؽ�Ʈ ����
	CString sValue = pGrid->GetTextMatrix(nRow, nCol);
	sValue.Replace( _T("��"), _T("") );
	sValue.Replace( _T("��"), _T("") );
	pEditCtrl->SetWindowText(sValue);

	pEditCtrl->MoveWindow(rcCell);
	pEditCtrl->ShowWindow(SW_SHOW);
	pEditCtrl->SetFocus();
	pEditCtrl->BringWindowToTop();
	
	return 0;

EFS_END
return -1;
}

INT CMngLocMgr10Dlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
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

INT CMngLocMgr10Dlg::ApplyDataChange(LPCTSTR lpszData)
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
	m_pDM->SetCellData( _T("������"), lpszData, nRow - 1 );
	m_pGrid->Display();

	return 0;

EFS_END
return -1;
}

INT CMngLocMgr10Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
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


