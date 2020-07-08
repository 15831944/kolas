// MngLocMgr08Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr08Dlg.h"
#include "ChangeValue.h"
#include "SettingGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr08Dlg dialog


CMngLocMgr08Dlg::CMngLocMgr08Dlg(CESL_Mgr* pParent , CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr08Dlg)
	m_bCivilNoDisplayFlag = FALSE;
	m_bUserNoDisplayFlag = FALSE;
	//}}AFX_DATA_INIT
	m_pLoanShareManager = pLoanShareManager;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;

	m_bIsUnityLoanService = FALSE;

	// 2012.09.27 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
	m_pEditCtrl[0] = NULL;
}
CMngLocMgr08Dlg::~CMngLocMgr08Dlg()
{	
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	// 2012.09.27 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
	if(m_pEditCtrl[0])
	{
		delete m_pEditCtrl[0];		
		m_pEditCtrl[0] = NULL;
	}
}

VOID CMngLocMgr08Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr08Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngLocMgr08Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr08Dlg)
	ON_WM_CTLCOLOR()  
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(100, OnKillfocusedt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr08Dlg message handlers

BOOL CMngLocMgr08Dlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CMngLocMgr08Dlg::ViewScreen()
{
EFS_BEGIN	

	// 2012.10.24 EDIT BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
	// 2012.09.27 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
	if(m_pInfo->m_bKLUse || m_pInfo->m_bAgreeInfoRecordYN)
	{
		INT ids;
		const int nCnt = 3;

		// ����
		CString sSection[nCnt] = 
				{	_T("�ֹε�Ϲ�ȣ"), 
					_T("�����ڹ�ȣ"),
					_T("ȸ������")};
		// ���
		CString sAbility[nCnt] = 
				{	_T("�ֹε�Ϲ�ȣ ǥ�ÿ���"), 
					_T("�����ڹ�ȣ ǥ�ÿ���"),
					_T("ȸ������ ���űⰣ ����")};
		// ����
		CString sExplain[nCnt] = 
				{	_T("�ֹε�Ϲ�ȣ�� ǥ�õǴ� ��� ȭ�鿡 �ֹε�Ϲ�ȣ�� ǥ�ÿ���"), 
					_T("����/�ݳ� ȭ���� �����ڹ�ȣ�� �ֹι�ȣ�϶� �����ڹ�ȣ�� ǥ�ÿ��� (ǥ�þ���:���ڸ�*ǥ��)"),
					_T("ȸ������ ���űⰣ�� �ϴ����� �����Ѵ� (0:������)")};
		// ������
		CString sManage[nCnt] = 
				{	_T("�ֹε�Ϲ�ȣ�������"),
					_T("����ȭ���ֹι�ȣǥ������"),
					_T("ȸ���������űⰣ")};

		// Display
		m_pDM->FreeData();
		for( INT i = 0; i < nCnt; i++ )
		{
			m_pDM->InsertRow(-1);
			m_pDM->SetCellData( _T("����"), sSection[i], i ); 
			m_pDM->SetCellData( _T("���"), sAbility[i], i );
			m_pDM->SetCellData( _T("����"), sExplain[i], i );

			CString sGetData;
			ids = this->GetManageValue( _T("����"), _T("����"), _T("�̿��ڰ���"), sManage[i], sGetData);
			if( ids < 0 ) 
			{
				ESLAfxMessageBox(_T("������������ �������µ� �����Ͽ����ϴ�."));
				return -1;
			}

			// 13/10/07 �ڴ�� : 2012�⵵ ��ɰ�����ġ �����۾�(20130604005)
/*//REM--------------------------------------------------------------------------
			// 2010.07.28 ADD BY KSJ : ���� ȸ�� ���� �ý��� ��� �� ��� �����ڹ�ȣ �ڵ��ο��� Y(�ڵ��ο�)
			if(m_bIsUnityLoanService)
			{
				if(0==i) sGetData = _T("Y");
			}
*///END--------------------------------------------------------------------------
			m_pDM->SetCellData( _T("������"), sGetData, i );
		}
	}
	else
	{
		// 2012.09.27 DEL BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 3ȸ
		INT ids;
		const int nCnt = 2;

		// ����
		CString sSection[nCnt] = 
				{	_T("�ֹε�Ϲ�ȣ"), 
					_T("�����ڹ�ȣ") };
		// ���
		CString sAbility[nCnt] = 
				{	_T("�ֹε�Ϲ�ȣ ǥ�ÿ���"), 
					_T("�����ڹ�ȣ ǥ�ÿ���")	};
		// ����
		CString sExplain[nCnt] = 
				{	_T("�ֹε�Ϲ�ȣ�� ǥ�õǴ� ��� ȭ�鿡 �ֹε�Ϲ�ȣ�� ǥ�ÿ���"), 
					_T("����/�ݳ� ȭ���� �����ڹ�ȣ�� �ֹι�ȣ�϶� �����ڹ�ȣ�� ǥ�ÿ��� (ǥ�þ���:���ڸ�*ǥ��)") };
		// ������
		CString sManage[nCnt] = 
				{	_T("�ֹε�Ϲ�ȣ�������"),
					_T("����ȭ���ֹι�ȣǥ������") };

		// Display
		m_pDM->FreeData();
		for( INT i = 0; i < nCnt; i++ )
		{
			m_pDM->InsertRow(-1);
			m_pDM->SetCellData( _T("����"), sSection[i], i ); 
			m_pDM->SetCellData( _T("���"), sAbility[i], i );
			m_pDM->SetCellData( _T("����"), sExplain[i], i );

			CString sGetData;
			ids = this->GetManageValue( _T("����"), _T("����"), _T("�̿��ڰ���"), sManage[i], sGetData);
			if( ids < 0 ) 
			{
				ESLAfxMessageBox(_T("������������ �������µ� �����Ͽ����ϴ�."));
				return -1;
			}

			// 13/10/07 �ڴ�� : 2012�⵵ ��ɰ�����ġ �����۾�(20130604005)
/*//REM--------------------------------------------------------------------------
			// 2010.07.28 ADD BY KSJ : ���� ȸ�� ���� �ý��� ��� �� ��� �����ڹ�ȣ �ڵ��ο��� Y(�ڵ��ο�)
			if(m_bIsUnityLoanService)
			{
				if(0==i) sGetData = _T("Y");
			}
*///END--------------------------------------------------------------------------
			m_pDM->SetCellData( _T("������"), sGetData, i );
		}
	}
	m_pCM->AllControlDisplay();

	// �׸��� ���� 
	CSettingGrid *pSet = new CSettingGrid();
	pSet->SetIsUnityLoanService(m_bIsUnityLoanService);
	pSet->SettingGrid08(m_pDM, m_pGrid);
	delete pSet;
	pSet = NULL;

	// �ڵ尪 �������� ����
	/*
	CString sCode;
	for( i = 0; i < nCnt; i++ )
	{
		m_pDM->GetCellData( _T("������"), i, sCode );
		if( _T("Y") == sCode ) 
			m_pGrid->SetAt(i, 3, _T("������") );
		else 
			m_pGrid->SetAt(i, 3, _T("�������") );
	}
	*/
	/*
	CString strData = _T("");
	INT ids = 0;

	//ù ��° 
	ids = this->GetManageValue(_T("����"),_T("����"),_T("�̿��ڰ���"),_T("�ֹε�Ϲ�ȣ�������"),strData);
	if(_T("Y") == strData) m_bCivilNoDisplayFlag = TRUE;
	else  m_bCivilNoDisplayFlag = FALSE;
	strData.Empty();
	if(ids<0) return -1;
	
	if ( m_bCivilNoDisplayFlag )
	{
		((CButton*) GetDlgItem (IDC_radSETTING_DISPLAY_TYPE_Y))->SetCheck(1);
		((CButton*) GetDlgItem (IDC_radSETTING_DISPLAY_TYPE_N))->SetCheck(0);
	}
	else
	{
		((CButton*) GetDlgItem (IDC_radSETTING_DISPLAY_TYPE_Y))->SetCheck(0);
		((CButton*) GetDlgItem (IDC_radSETTING_DISPLAY_TYPE_N))->SetCheck(1);
	}
	
	// 2007.08.29 add by pdw
	ids = this->GetManageValue( _T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), strData );
	if(_T("Y") == strData) m_bUserNoDisplayFlag = TRUE;
	else  m_bUserNoDisplayFlag = FALSE;
	strData.Empty();
	
	if ( m_bUserNoDisplayFlag )
	{
		((CButton*) GetDlgItem (IDC_radSETTING_DISPLAY_USER_TYPE_Y))->SetCheck(1);
		((CButton*) GetDlgItem (IDC_radSETTING_DISPLAY_USER_TYPE_N))->SetCheck(0);
	}
	else
	{
		((CButton*) GetDlgItem (IDC_radSETTING_DISPLAY_USER_TYPE_Y))->SetCheck(0);
		((CButton*) GetDlgItem (IDC_radSETTING_DISPLAY_USER_TYPE_N))->SetCheck(1);
	}

	return 0;
	*/

EFS_END
return -1;
}

INT CMngLocMgr08Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	// 2012.10.24 EDIT BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 3ȸ
	// 2012.09.27 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
	// ȸ������ ���űⰣ �߰�
	if(m_pInfo->m_bKLUse || m_pInfo->m_bAgreeInfoRecordYN)
	{
		const int nCnt = 3;

		// ������
		CString sManage[nCnt] = 
				{	_T("�ֹε�Ϲ�ȣ�������"),
					_T("����ȭ���ֹι�ȣǥ������"),
					_T("ȸ���������űⰣ")};

		CString sCode;
		for( INT i = 0; i < nCnt; i++ )
		{
			m_pDM->GetCellData( _T("������"), i, sCode );
			ids = this->SetManageValue( _T("����"), _T("����"), _T("�̿��ڰ���"), sManage[i], sCode );
			if( ids < 0 )
			{
				ESLAfxMessageBox(_T("������������ �����ϴµ� �����Ͽ����ϴ�."));
				return -1;
			}
		}
		
		// 2012.10.19 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 3ȸ
		CString strTmp;
		m_pDM->GetCellData(_T("������"), 2, strTmp);
		m_pInfo->m_nUserInfoRenewalDay = _ttoi(strTmp);
	}
	else
	{
		const int nCnt = 2;

		// ������
		CString sManage[nCnt] = 
				{	_T("�ֹε�Ϲ�ȣ�������"),
					_T("����ȭ���ֹι�ȣǥ������") };

		CString sCode;
		for( INT i = 0; i < nCnt; i++ )
		{
			m_pDM->GetCellData( _T("������"), i, sCode );
			ids = this->SetManageValue( _T("����"), _T("����"), _T("�̿��ڰ���"), sManage[i], sCode );
			if( ids < 0 )
			{
				ESLAfxMessageBox(_T("������������ �����ϴµ� �����Ͽ����ϴ�."));
				return -1;
			}
		}
	}
	return 0;
	/*
	CString	strData;
	INT		ids;

	m_bCivilNoDisplayFlag = ((CButton*) GetDlgItem (IDC_radSETTING_DISPLAY_TYPE_Y))->GetCheck();

	if ( m_bCivilNoDisplayFlag ) strData=_T("Y");
	else strData=_T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�̿��ڰ���"),_T("�ֹε�Ϲ�ȣ�������"),strData);
	if(ids<0) return -2;

	// 2007.08.29 add by pdw
	m_bUserNoDisplayFlag = ((CButton*) GetDlgItem (IDC_radSETTING_DISPLAY_USER_TYPE_Y))->GetCheck();
	if ( m_bUserNoDisplayFlag ) strData=_T("Y");
	else strData=_T("N");
	ids = this->SetManageValue(_T("����"),_T("����"),_T("�̿��ڰ���"),_T("����ȭ���ֹι�ȣǥ������"),strData);
	if(ids<0) return -2;

	return 0;
	*/

EFS_END
return -1;	
}

BOOL CMngLocMgr08Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_����ȯ�漳��_����������ȣ��å")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	// 2010.07.28 ADD BY KSJ : ���� ȸ�� ���� �ý��� ��뿩��
	CString strValue;
	INT ids = pParentMgr->GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), strValue);
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("OnInitDialog"));
	strValue.TrimLeft(); strValue.TrimRight();
	if(_T("Y")==strValue)
	{
		m_bIsUnityLoanService = TRUE;
	}
	else
	{
		m_bIsUnityLoanService = FALSE;
	}

	m_pCM = FindCM(_T("CM_����ȯ�漳��_����������ȣ��å"));
	m_pDM = FindDM(_T("DM_����ȯ�漳��_����������ȣ��å"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	// 2012.10.24 EDIT BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 3ȸ
	// 2012.09.27 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
	if(m_pInfo->m_bKLUse || m_pInfo->m_bAgreeInfoRecordYN)
	{
		m_pEditCtrl[0] = new CEdit();
		m_pEditCtrl[0]->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, 100);
		m_pEditCtrl[0]->ShowWindow(SW_HIDE);
	}
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CMngLocMgr08Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CMngLocMgr08Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_08);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);	
}

// ** �޺��ڽ� ���� �� �� ����
VOID CMngLocMgr08Dlg::CreateComboBoxInGridSell()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	INT nRow = m_pGrid->GetRow();
	CString strData = m_pDM->GetCellData(_T("������"), nRow - 1);
	CString strElement = m_pDM->GetCellData(_T("���"), nRow - 1);

	if( m_pComboBox )
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
	
	if(m_bIsUnityLoanService)
	{
		if(1==nRow)
		{
			return;
		}
	}
	
	// 2012.10.24 EDIT BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 3ȸ
	// 2012.09.05 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
	if( 3 == nRow && m_pInfo->m_bKLUse || m_pInfo->m_bAgreeInfoRecordYN)
	{
		ShowEditBox(m_pGrid, m_pEditCtrl[0]);
		return;
	}

	CStringArray ArrCmbData;	ArrCmbData.RemoveAll();
	
	m_pComboBox = new CEnvironComboBox_MultiState;
	ArrCmbData.Add(_T("ǥ����"));
	ArrCmbData.Add(_T("ǥ�þ���"));
	
	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450);
	m_pComboBox->Init(&ArrCmbData);
	m_pComboBox->InitMngLocMgr(8);
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

BEGIN_EVENTSINK_MAP(CMngLocMgr08Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr08Dlg)
	ON_EVENT(CMngLocMgr08Dlg, IDC_gridMNG_LOC_MGR_08, -600 /* Click */, OnClickgridMNGLOCMGR08, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMngLocMgr08Dlg::OnClickgridMNGLOCMGR08() 
{
	if( m_pGrid->GetMouseRow() == 0 ) return;
	INT col = m_pGrid->GetCol();
	INT row = m_pGrid->GetRow();
	if( col == 4 ) CreateComboBoxInGridSell();
}

INT CMngLocMgr08Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
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



// 2012.09.27 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
INT CMngLocMgr08Dlg::ShowEditBox(CESL_Grid *pGrid, CEdit* pEditCtrl)
{
EFS_BEGIN

	pEditCtrl->GetSafeHwnd();
	if ( NULL == pGrid ) 
	{
		return -1;
	}

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if ( 1 > nRow || 1 > nCol ) 
	{
		return -1;
	}

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	// �ؽ�Ʈ ����
	CString sValue = pGrid->GetTextMatrix(nRow, nCol);
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


// 2012.09.27 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
INT CMngLocMgr08Dlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
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

// 2012.09.27 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
INT CMngLocMgr08Dlg::ApplyDataChange(LPCTSTR lpszData)
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

	// �׸��� ����
	CSettingGrid *pSet = new CSettingGrid();
	pSet->SetIsUnityLoanService(m_bIsUnityLoanService);
	pSet->SettingGrid01(m_pDM, m_pGrid);
	delete pSet;
	pSet = NULL;

	return 0;

EFS_END
return -1;
}


// 2012.09.27 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
void CMngLocMgr08Dlg::OnKillfocusedt()
{
	SetFocus();
	if (NULL == m_pEditCtrl[0]) return ;
	if (!::IsWindowVisible(m_pEditCtrl[0]->GetSafeHwnd())) return ;

	m_pEditCtrl[0]->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl[0]->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}



