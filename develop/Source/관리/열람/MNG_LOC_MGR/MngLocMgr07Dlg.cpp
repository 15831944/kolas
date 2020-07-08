// MngLocMgr07Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MngLocMgr07Dlg.h"
#include "ChangeValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr07Dlg dialog


CMngLocMgr07Dlg::CMngLocMgr07Dlg(CESL_Mgr* pParent , CLoanShareManager* pLoanShareManager )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngLocMgr07Dlg)
	m_bLoanStopUserMessageFlag = FALSE;
	//}}AFX_DATA_INIT	

	m_pLoanShareManager = pLoanShareManager;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;

}
CMngLocMgr07Dlg::~CMngLocMgr07Dlg()
{
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}
}


VOID CMngLocMgr07Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngLocMgr07Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngLocMgr07Dlg, CDialog)
	//{{AFX_MSG_MAP(CMngLocMgr07Dlg)
	ON_WM_CTLCOLOR() 
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngLocMgr07Dlg message handlers

BOOL CMngLocMgr07Dlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CMngLocMgr07Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 2006-05-24 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++
	// ������� ���� ��� ����
	//if ( InitESL_Mgr(_T("SM_MNG_LOC_MGR_07_DLG")) < 0 )
	//	ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMngLocMgr07Dlg::OnInitDialog()") );
	// ----------------------------------------------------------------------

	if( InitESL_Mgr(_T("SM_����ȯ�漳��_���մ������")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_����ȯ�漳��_���մ������"));
	m_pDM = FindDM(_T("DM_����ȯ�漳��_���մ������"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// [ Func Desc ]
//   �������� �����ش�.
// [ Mini Spec ]
// 1. ��������ȸ�� �޼��� ��� Ÿ��
// 2. ����å�� ���� ��뿩�� 
/////////////////////////////////////////////////////////////////////////////
INT CMngLocMgr07Dlg::ViewScreen()
{
EFS_BEGIN	

	INT ids;
	const int nCnt = 4;
	// ����
	CString sSection[nCnt] = 
			{	_T("���մ������"), _T("���մ������"), _T("���մ������"), _T("���մ������") };
	// ���
	CString sAbility[nCnt] = 
			{	_T("�̿��ڰ˻��� ��������ȸ�� �ȳ�"), 
				_T("�̿��ڰ˻��� ����ȸ�� �ȳ�"), 
				_T("���͹��Խ� ID, ��й�ȣ ���Կ���"),
				_T("���մ���å�� ��뿩��") };
	// ����
	CString sExplain[nCnt] = 
			{	_T("�̿��� �˻��� Ÿ�������� �������� ȸ���� ��� �޽��� ���"), 
				_T("�̿��� �˻��� Ÿ�������� ����ȸ���� ��� �޽��� ���"), 
				_T("���͹��Խ� �ߺ��Ǿ������ ID�� ��й�ȣ�� ���ܰ� �Ǹ� �޽����� ���ؼ� Ȯ���� �����մϴ�"),
				_T("����� Ÿ�������� ������ ��ü ����å���� ���մ���å���� �ʰ�������� ��������") };
	// ������
	CString sManage[nCnt] = 
			{	_T("��������ȸ���޽������"),
				_T("����ȸ���޽������"),
				_T("ID���͹��Լ���"),
				_T("���մ���å������") };

	// Display
	m_pDM->FreeData();
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData( _T("����"), sSection[i], i ); 
		m_pDM->SetCellData( _T("���"), sAbility[i], i );
		m_pDM->SetCellData( _T("����"), sExplain[i], i );

		CString sGetData;
		ids = this->GetManageValue( _T("��Ÿ"), _T("����"), _T("�����ڰ�������"), sManage[i], sGetData);
		if( ids < 0 ) 
		{
			ESLAfxMessageBox(_T("������������ �������µ� �����Ͽ����ϴ�."));
			return -1;
		}

		// ���� ������ ������ ����ɼ� �ְ�
		if( sManage[i] == _T("��������ȸ���޽������") || sManage[i] == _T("����ȸ���޽������") )
		{
			if( sGetData == _T("N") )		sGetData = _T("0");
			else if ( sGetData == _T("Y") )	sGetData = _T("1");
		}
		m_pDM->SetCellData( _T("������"), sGetData, i );
	}
	m_pCM->AllControlDisplay();
	/*
	// ======================================================================
	// Variable Declare
	// ======================================================================
	INT ids = 0;
	CString strData = _T("");	

	// ======================================================================
	// 1. ��������ȸ�� �޼��� ��� Ÿ��
	// ======================================================================
	// 2006-05-24 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++
	// ������ ���� ������ ���� ��Ʈ�� ����
	
	//ids = this->GetManageValue(_T("��Ÿ"),_T("����"),_T("�����ڰ�������"),_T("��������ȸ���޽������"),strData);
	//if(_T("Y") == strData) m_bLoanStopUserMessageFlag = TRUE;
	//else  m_bLoanStopUserMessageFlag = FALSE;
	//strData.Empty();
	//if(ids<0) return -1;
	//
	//if ( m_bLoanStopUserMessageFlag )
	//{
	//	((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->SetCheck(1);
	//	((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_N))->SetCheck(0);
	//}
	//else
	//{
	//	((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->SetCheck(0);
	//	((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_N))->SetCheck(1);
	//}
	
	ids = this->GetManageValue(_T("��Ÿ"),_T("����"),_T("�����ڰ�������"),_T("��������ȸ���޽������"),strData);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSetting_MESSAGE_TYPE);
	if ( !pCombo ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	
	// ���� ������ ������ ����ɼ� �ְ�
	// N -> 0 , Y -> 1
	if( strData == _T("N") )		strData = _T("0");
	else if ( strData == _T("Y") )	strData = _T("1");

	pCombo->SetCurSel( _ttoi( strData ) );
	// ----------------------------------------------------------------------

	// ======================================================================
	// 2. ����å�� ���� ��뿩�� 
	// ======================================================================
	ids = this->GetManageValue(_T("��Ÿ"),_T("����"),_T("�����ڰ�������"),_T("���մ���å������"),strData);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	
	// ����...
	if ( ids == 100 )	strData = _T("N");

	if(_T("Y") == strData) m_bLoanStopUserMessageFlag = TRUE;
	else  m_bLoanStopUserMessageFlag = FALSE;
	strData.Empty();
	if(ids<0) return -1;
	
	if ( m_bLoanStopUserMessageFlag )
	{
		((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->SetCheck(1);
		((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_N))->SetCheck(0);
	}
	else
	{
		((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->SetCheck(0);
		((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_N))->SetCheck(1);
	}

	// 2006.12.07 ADD BY PDJ
	// ======================================================================
	// 3. ���͹��Խ� ID,PASSWORD ���� ����	
	// ======================================================================
	ids = this->GetManageValue(_T("��Ÿ"),_T("����"),_T("�����ڰ�������"),_T("ID���͹��Լ���"), strData);	   
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	
	BOOL bCheck;	
	bCheck = ((CButton*) GetDlgItem (IDC_radSETTING_ID_IMPORT_Y))->GetCheck();
	if ( strData == _T("Y") ) ((CButton*) GetDlgItem (IDC_radSETTING_ID_IMPORT_Y))->SetCheck(TRUE);
	else ((CButton*) GetDlgItem (IDC_radSETTING_ID_IMPORT_N))->SetCheck(TRUE);
	*/
	return 0;
EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [ Func Desc ]
//   �������� �����Ѵ�.
// [ Mini Spec ]
// 1. ��������ȸ�� �޼��� ��� Ÿ�� ����
// 2. ����å�� ���� ��뿩�� ����
/////////////////////////////////////////////////////////////////////////////
INT CMngLocMgr07Dlg::SaveScreen()
{
EFS_BEGIN

	INT ids;
	const int nCnt = 4;

	// ������
	CString sManage[nCnt] = 
			{	_T("��������ȸ���޽������"),
				_T("����ȸ���޽������"),
				_T("ID���͹��Լ���"),
				_T("���մ���å������") };

	CString sCode;
	for( INT i = 0; i < nCnt; i++ )
	{
		m_pDM->GetCellData( _T("������"), i, sCode );
		ids = this->SetManageValue( _T("��Ÿ"), _T("����"), _T("�����ڰ�������"), sManage[i], sCode );
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("������������ �����ϴµ� �����Ͽ����ϴ�."));
			return -1;
		}
	}
	/*
	// ======================================================================
	// Variable Declare
	// ======================================================================
	INT ids = 0;
	CString strData = _T("");	

	// ======================================================================
	// 1. ��������ȸ�� �޼��� ��� Ÿ�� ����
	// ======================================================================
	// 2006-05-24 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++
	// ������ ���� ������ ���� ��Ʈ�� ����
	
	//m_bLoanStopUserMessageFlag = ((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->GetCheck();
	//
	//if ( m_bLoanStopUserMessageFlag ) strData=_T("Y");
	//else strData=_T("N");
	//ids = this->SetManageValue(_T("��Ÿ"),_T("����"),_T("�����ڰ�������"),_T("��������ȸ���޽������"),strData);
	//if(ids<0) return -2;
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSetting_MESSAGE_TYPE);
	if ( !pCombo ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	strData.Format( _T("%d") , pCombo->GetCurSel() );

	ids = this->SetManageValue(_T("��Ÿ"),_T("����"),_T("�����ڰ�������"),_T("��������ȸ���޽������"),strData);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	// ----------------------------------------------------------------------

	// ======================================================================
	// 2. ����å�� ���� ��뿩�� 
	// ======================================================================
	m_bLoanStopUserMessageFlag = ((CButton*) GetDlgItem (IDC_radSETTING_MESSAGE_TYPE_Y))->GetCheck();

	if ( m_bLoanStopUserMessageFlag ) strData=_T("Y");
	else strData=_T("N");
	ids = this->SetManageValue(_T("��Ÿ"),_T("����"),_T("�����ڰ�������"),_T("���մ���å������"),strData);
	if ( ids < 0 && ids != -1) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	// ����...
	if ( ids == -1 )
	{
		CString strQuery;
		strQuery.Format ( _T(" INSERT INTO ESL_MANAGE_TBL( REC_KEY,GROUP_1, GROUP_2,GROUP_3, GROUP_4, GROUP_5, CLASS_ALIAS,CLASS_DESC,") 
			              _T(" VALUE_NAME, VALUE_1, VALUE_2, VALUE_3, FIRST_WORK, LAST_WORK, SYS_SETUP_YN) ")
                          _T(" VALUES( ESL_SEQ.NEXTVAL	, 'L', '��Ÿ', '����', '', '', '�����ڰ�������'	, 'Y:���մ���å������ ��� N:������', ") 
						  _T(" '���մ���å������', '%s', '', '', '', '', 'Y' );"),strData) ;
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
		ids = dm.ExecuteQuery(strQuery, TRUE);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	}

	// ======================================================================
	// 2. ����å�� ���� ��뿩�� 
	// ======================================================================
	BOOL bCheck;
 	bCheck = ((CButton*) GetDlgItem (IDC_radSETTING_ID_IMPORT_Y))->GetCheck();
	if ( bCheck ) strData = _T("Y");
	else strData = _T("N");
	
	ids = this->SetManageValue(_T("��Ÿ"),_T("����"),_T("�����ڰ�������"),_T("ID���͹��Լ���"),strData);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CMngLocMgr07Dlg::ViewScreen()") );
	*/
	return 0;

EFS_END
	return -1;	
}

HBRUSH CMngLocMgr07Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


BEGIN_EVENTSINK_MAP(CMngLocMgr07Dlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngLocMgr07Dlg)
	ON_EVENT(CMngLocMgr07Dlg, IDC_gridMNG_LOC_MGR_07, -600 /* Click */, OnClickgridMNGLOCMGR07, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMngLocMgr07Dlg::OnClickgridMNGLOCMGR07() 
{
	if( m_pGrid->GetMouseRow() == 0 ) return;
	INT col = m_pGrid->GetCol();
	if( col == 4 ) CreateComboBoxInGridSell();
}

void CMngLocMgr07Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_LOC_MGR_07);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 1, cx, cy-10);		
}

INT CMngLocMgr07Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
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

// ** �޺��ڽ� ���� �� �� ����
VOID CMngLocMgr07Dlg::CreateComboBoxInGridSell()
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
	
	CStringArray ArrCmbData;	ArrCmbData.RemoveAll();
	
	// 1:��������ȸ���޽������, 2:����ȸ���޽������
	if( nRow == 1 || nRow == 2 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("������"));
		ArrCmbData.Add(_T("���޽���"));
		ArrCmbData.Add(_T("��������"));
	}
	// 3:ID���͹��Լ���, 4:���մ���å������
	else if( nRow == 3 || nRow == 4 )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("�����"));
		ArrCmbData.Add(_T("������"));
	}
	else
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		ArrCmbData.Add(_T("������"));
		ArrCmbData.Add(_T("�������"));
	}
	
	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450);
	m_pComboBox->Init(&ArrCmbData);
	m_pComboBox->InitMngLocMgr(7);
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