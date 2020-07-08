// MNG_CODE_SHELF_04.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CODE_SHELF_04.h"

#include "MNG_CODE_SHELF_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_SHELF_04 dialog


CMNG_CODE_SHELF_04::CMNG_CODE_SHELF_04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_SHELF_04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_CODE_SHELF_04::~CMNG_CODE_SHELF_04()
{	
	if( m_checkedDMRow != NULL )
		free( m_checkedDMRow );
	m_checkedDMRow = NULL;
}

BOOL CMNG_CODE_SHELF_04::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CODE_SHELF_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_SHELF_04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_SHELF_04, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_SHELF_04)
	ON_BN_CLICKED(IDC_BEFORE, OnBefore)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_rTYPE_1, OnrTYPE1)
	ON_BN_CLICKED(IDC_rTYPE_2, OnrTYPE2)
	ON_BN_CLICKED(IDC_rTYPE_3, OnrTYPE3)
	ON_WM_SHOWWINDOW()	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_SHELF_04 message handlers

BOOL CMNG_CODE_SHELF_04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CODE_SHELF_04") ) < 0 )
	{
		AfxMessageBox( _T("Error : InitESL_Mgr - MNG_CODE_SHELF_04") );
		return false;
	}
	
	m_type = 0;
	m_checkedDMRow = NULL;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ����
VOID CMNG_CODE_SHELF_04::OnOK() 
{
	// TODO: Add extra validation here
	
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	CString data[ 7 ];
	//CString data[ 8 ];
	//--2008.10.14 UPDATE BY PWR --}}
	GetData( data );

	INT ids;
	switch( m_type )
	{
		case 1 :		// ���
			ids = SaveRecordData( data );
			break;
		case 2 :
			ids = SaveSepData( data );
			break;
		case 3 :
			ids = SaveClsData( data );
			break;
		default :
			AfxMessageBox( _T(" ���, ��ġ, �з��� �����Ͽ� �ֽʽÿ�") );
			return;
			break;
	}
	
	if( -1 == ids )
		return;
	
	CMNG_CODE_SHELF_01 *pParent = ( CMNG_CODE_SHELF_01 * )pParentMgr;
	pParent->ShowGrid( m_type );
	
	if( 1 == m_mode )
	{
		AfxMessageBox( _T("����Ǿ����ϴ�.") );
		CDialog::OnOK();
	}
	else
	{
		CString cmAlias[ 3 ] = {_T("���"), _T("��ġ"), _T("�з�")};
		SetReverse(_T("CM_MNG_CODE_SHELF_01_01"), cmAlias[ m_type - 1 ], m_checkedDMRow[ m_index ] );
		AfxMessageBox( _T("�����Ǿ����ϴ�.") );
		// 040318_HSS ���� �� �ݱ� - redisplay�Ǳ� ������ �������� ���� -> �ݴ� ���� ���
		CDialog::OnOK();
	}
}

// ����
VOID CMNG_CODE_SHELF_04::OnBefore() 
{
	// TODO: Add your control notification handler code here
	m_index--;
	
	if( m_index < 0 )
	{
		AfxMessageBox( _T("�� ó�� ����Դϴ�.") );
		m_index = 0;
		return;
	}
	else
		ViewModifyData();

}

// ����
VOID CMNG_CODE_SHELF_04::OnNext() 
{
	// TODO: Add your control notification handler code here

	m_index++;

	if( m_index > m_rowCnt - 1 )
	{
		AfxMessageBox( _T("������ ����Դϴ�.") );
		m_index = m_rowCnt - 1;
		return;
	}
	else
		ViewModifyData();
	
}

VOID CMNG_CODE_SHELF_04::OnrTYPE1() 
{
	// TODO: Add your control notification handler code here
	
	SetControlStat();
}

VOID CMNG_CODE_SHELF_04::OnrTYPE2() 
{
	// TODO: Add your control notification handler code here
	
	SetControlStat();
}

VOID CMNG_CODE_SHELF_04::OnrTYPE3() 
{
	// TODO: Add your control notification handler code here
	
	SetControlStat();
}

VOID CMNG_CODE_SHELF_04::SetControlStat( INT type )
{	
	UINT ID[ 5 ] = { IDC_cINPUT_4, IDC_cINPUT_5, IDC_cINPUT_6, IDC_eINPUT_7, IDC_eINPUT_8 };
	bool flag[ 5 ] = { false, false, false, false, false };

	if( !type )
		type = GetSelectedRadio();

	m_type = type;
	
	switch( type )
	{
		case 1 :	// ���
			flag[ 0 ] = true;			
			break;
		case 2 :	// ��ġ
			flag[ 1 ] = true;
			break;
		case 3 :	// �з�
			flag[ 2 ] = true;
			flag[ 3 ] = true;
			flag[ 4 ] = true;
			break;
	}

	CWnd *pWnd;
	for( INT i = 0; i < 5; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( ID[ i ] );
		pWnd->EnableWindow( flag[ i ] );
	}

	CMNG_CODE_SHELF_01 *pParent = ( CMNG_CODE_SHELF_01 * )pParentMgr;
	pParent->m_tab.SetCurSel( type - 1 );
	pParent->ViewTab( type );
}

INT CMNG_CODE_SHELF_04::GetSelectedRadio()
{
	CButton *pRdo;
	UINT ID[ 3 ] = { IDC_rTYPE_1, IDC_rTYPE_2, IDC_rTYPE_3 };

	for( INT i = 0; i < 3; i++ )
	{
		pRdo = ( CButton * )GetDlgItem( ID[ i ] );
		if( pRdo->GetCheck() )
			return i + 1;
	}

	return 0;
}

INT CMNG_CODE_SHELF_04::GetData( CString data[] )
{
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	CString alias[ 7 ] = {
		_T("�������±���"), _T("�ڷ��"), _T("��ϱ���"), _T("��ġ��ȣ"), _T("�з�ǥ����"), _T("�з���ȣ_����"), _T("�з���ȣ_����") };

	for( INT i = 0; i < 7; i++ )
		data[ i ] = TakeControlData( alias[ i ] );
	/*
	CString alias[ 8 ] = {
		_T("�������±���"), _T("��������"), _T("�ڷ��"), _T("��ϱ���"), _T("��ġ��ȣ"), _T("�з�ǥ����"), _T("�з���ȣ_����"), _T("�з���ȣ_����") };

	for( INT i = 0; i < 8; i++ )
		data[ i ] = TakeControlData( alias[ i ] );
	*/
	//--2008.10.14 UPDATE BY PWR --}}
	
	return 0;
}

CString CMNG_CODE_SHELF_04::TakeControlData(CString alias, INT option)
{
	// option == 0 : ��Ƽ�޺��� �ڵ尪�� �����´�. 
	// option == 1 : ��Ƽ�޺��� �ڵ弳���� �����´�. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_MNG_CODE_SHELF_04_01"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_MNG_CODE_SHELF_04_01"), _T("K2Up") );
		return _T("");
	}
	
	// �ڵ尪�� _T("0") �̰ų� ������ _T("�������")�϶� _T("")�� ��ȯ�Ѵ�. 
	switch( option )
	{
		case 0 :
			if( _T("0") == result )
				return _T("");
			break;
		case 1 :
			if( _T("�������") == result )
				return _T("");
			break;
		default :
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_MNG_CODE_SHELF_04_01"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

INT CMNG_CODE_SHELF_04::SaveRecordData( CString data[] )
{
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	const INT fieldCnt = 7;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),				//  0
		_T("MANAGE_CODE"),			//  1
		_T("REG_CODE"),				//  2
		_T("SHELF_LOC_CODE"),		//	3
		_T("PUBLISH_FORM_CODE"),	//	4
		_T("FIRST_WORK"),			//  5
		_T("LAST_WORK") };			//	6

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_1") );

	CString realData[ fieldCnt ];
	
	if( CheckOverlapData( data[ 0 ], m_pInfo->MANAGE_CODE, data[ 2 ] ) )
	{
		MessageBox( _T("�ߺ��� ����Ÿ�� �����մϴ�."), _T("KOLAS 2 Upgrade"), MB_OK );
		return -1;
	}
		
	if( 1 == m_mode )
	{
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 5 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 6 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );

	realData[ 1 ] = m_pInfo->MANAGE_CODE;
	realData[ 2 ] = data[ 2 ];
	realData[ 3 ] = data[ 1 ];
	realData[ 4 ] = data[ 0 ];
	/*
	const INT fieldCnt = 7;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),				//  0
		_T("MANAGE_CODE"),			//  1
		_T("REG_CODE"),				//  2
		_T("SHELF_LOC_CODE"),		//	3
		_T("PUBLISH_FORM_CODE"),	//	4
		_T("FIRST_WORK"),			//  5
		_T("LAST_WORK") };			//	6

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_1") );

	CString realData[ fieldCnt ];
	
	if( CheckOverlapData( data[ 0 ], data[ 1 ], data[ 3 ] ) )
	{
		MessageBox( _T("�ߺ��� ����Ÿ�� �����մϴ�."), _T("KOLAS 2 Upgrade"), MB_OK );
		return -1;
	}
		
	if( 1 == m_mode )
	{
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 5 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 6 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );

	realData[ 1 ] = data[ 1 ];
	realData[ 2 ] = data[ 3 ];
	realData[ 3 ] = data[ 2 ];
	realData[ 4 ] = data[ 0 ];
	*/
	//--2008.10.14 UPDATE BY PWR --}}
	
	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( realData[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], realData[ i ] );

	if( 1 == m_mode )
	{
		pDM->MakeInsertFrame( _T("MN_REG_CODE_SHELF_RULE_MNG_TBL") );
		pDM->SetCellData( fieldName, fieldCnt, realData, -1 );
	}
	else if( 2 == m_mode )
	{
		pDM->MakeUpdateFrame( _T("MN_REG_CODE_SHELF_RULE_MNG_TBL"), _T("REC_KEY"), _T("NUMERIC"), realData[ 0 ], m_checkedDMRow[ m_index ] );
		pDM->SetCellData( fieldName, fieldCnt, realData, m_checkedDMRow[ m_index ] );
	}

	pDM->SendFrame();
	pDM->EndFrame();
			
	return 0;
}

INT CMNG_CODE_SHELF_04::SaveSepData(CString data[])
{
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	const INT fieldCnt = 7;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),
		_T("MANAGE_CODE"),
		_T("SEPARATE_SHELF_CODE"),
		_T("SHELF_LOC_CODE"),
		_T("PUBLISH_FORM_CODE"),
		_T("FIRST_WORK"),
		_T("LAST_WORK") };

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_2") );

	CString realData[ fieldCnt ];
	
	if( CheckOverlapData( data[ 0 ], m_pInfo->MANAGE_CODE, data[ 3 ] ) )
	{
		MessageBox( _T("�ߺ��� ����Ÿ�� �����մϴ�."), _T("KOLAS 2 Upgrade"), MB_OK );
		return -1;
	}

	if( 1 == m_mode )
	{
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 5 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 6 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );
	
	realData[ 1 ] = m_pInfo->MANAGE_CODE;
	realData[ 2 ] = data[ 3 ];
	realData[ 3 ] = data[ 1 ];
	realData[ 4 ] = data[ 0 ];

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( realData[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], realData[ i ] );
	/*
	const INT fieldCnt = 7;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),
		_T("MANAGE_CODE"),
		_T("SEPARATE_SHELF_CODE"),
		_T("SHELF_LOC_CODE"),
		_T("PUBLISH_FORM_CODE"),
		_T("FIRST_WORK"),
		_T("LAST_WORK") };

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_2") );

	CString realData[ fieldCnt ];
	
	if( CheckOverlapData( data[ 0 ], data[ 1 ], data[ 4 ] ) )
	{
		MessageBox( _T("�ߺ��� ����Ÿ�� �����մϴ�."), _T("KOLAS 2 Upgrade"), MB_OK );
		return -1;
	}

	if( 1 == m_mode )
	{
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 5 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 6 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );
	
	realData[ 1 ] = data[ 1 ];
	realData[ 2 ] = data[ 4 ];
	realData[ 3 ] = data[ 2 ];
	realData[ 4 ] = data[ 0 ];

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( realData[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], realData[ i ] );
	*/
	//--2008.10.14 UPDATE BY PWR --}}
	if( 1 == m_mode )
	{
		pDM->MakeInsertFrame( _T("MN_SEP_CODE_SHELF_RULE_MNG_TBL") );
		pDM->SetCellData( fieldName, fieldCnt, realData, -1 );
	}
	else if( 2 == m_mode )
	{
		pDM->MakeUpdateFrame( _T("MN_SEP_CODE_SHELF_RULE_MNG_TBL"), _T("REC_KEY"), _T("NUMERIC"), realData[ 0 ],  m_checkedDMRow[ m_index ] );
		pDM->SetCellData( fieldName, fieldCnt, realData, m_checkedDMRow[ m_index ] );
	}

	pDM->SendFrame();
	pDM->EndFrame();

	return 0;
}

INT CMNG_CODE_SHELF_04::SaveClsData(CString data[])
{
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	const INT fieldCnt = 9;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"), 
		_T("MANAGE_CODE"), 
		_T("CLASS_NO_TYPE"), 
		_T("START_CLASS_NO"), 
		_T("END_CLASS_NO"), 
		_T("PUBLISH_FORM_CODE"), 
		_T("SHELF_LOC_CODE"), 
		_T("FIRST_WORK"), 
		_T("LAST_WORK") };

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_3") );

	CString realData[ fieldCnt ];
	
	if( 1 == m_mode )
	{
		
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 7 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 8 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );

	realData[ 1 ] = m_pInfo->MANAGE_CODE;
	realData[ 2 ] = data[ 4 ];
	realData[ 3 ] = data[ 5 ];
	realData[ 4 ] = data[ 6 ];
	realData[ 5 ] = data[ 0 ];
	realData[ 6 ] = data[ 1 ];
	/*
	const INT fieldCnt = 9;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"), 
		_T("MANAGE_CODE"), 
		_T("CLASS_NO_TYPE"), 
		_T("START_CLASS_NO"), 
		_T("END_CLASS_NO"), 
		_T("PUBLISH_FORM_CODE"), 
		_T("SHELF_LOC_CODE"), 
		_T("FIRST_WORK"), 
		_T("LAST_WORK") };

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_3") );

	CString realData[ fieldCnt ];
	
	
	//if( CheckOverlapData( data[ 0 ], data[ 1 ], data[ 5 ] ) )
	//{
	//	MessageBox( _T("�ߺ��� ����Ÿ�� �����մϴ�."), _T("KOLAS 2 Upgrade"), MB_OK );
	//	return -1;
	//}

	if( 1 == m_mode )
	{
		
		pDM->MakeRecKey( realData[ 0 ] );
		realData[ 7 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );
	}
	else if( 2 == m_mode )
		pDM->GetCellData( _T("REC_KEY"), m_checkedDMRow[ m_index ], realData[ 0 ] );	

	realData[ 8 ] = GetWorkLogMsg( _T("�谡���ذ���"), __WFILE__, __LINE__ );

	realData[ 1 ] = data[ 1 ];
	realData[ 2 ] = data[ 5 ];
	realData[ 3 ] = data[ 6 ];
	realData[ 4 ] = data[ 7 ];
	realData[ 5 ] = data[ 0 ];
	realData[ 6 ] = data[ 2 ];
	*/
	//--2008.10.14 UPDATE BY PWR --}}

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( realData[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], realData[ i ] );

	if( 1 == m_mode )
	{
		pDM->MakeInsertFrame( _T("MN_CLS_NUM_SHELF_RULE_MNG_TBL") );
		pDM->SetCellData( fieldName, fieldCnt, realData, -1 );
	}
	else if( 2 == m_mode )
	{
		pDM->MakeUpdateFrame( _T("MN_CLS_NUM_SHELF_RULE_MNG_TBL"), _T("REC_KEY"), _T("NUMERIC"), realData[ 0 ],  m_checkedDMRow[ m_index ] );
		pDM->SetCellData( fieldName, fieldCnt, realData, m_checkedDMRow[ m_index ] );
	}

	pDM->SendFrame();
	pDM->EndFrame();

	return 0;
}

VOID CMNG_CODE_SHELF_04::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	m_index = 0;
	if( m_checkedDMRow != NULL )
		free( m_checkedDMRow );
	m_checkedDMRow = NULL;
	m_rowCnt = 0;

	CButton *pBtn;
	INT i;
	UINT rdoID[ 3 ] = { IDC_rTYPE_1, IDC_rTYPE_2, IDC_rTYPE_3 };

	CMNG_CODE_SHELF_01 *pParent = ( CMNG_CODE_SHELF_01 * )pParentMgr;
	
	switch( m_mode )
	{
	case 1 :		// �Է�
		
		pBtn = ( CButton * )GetDlgItem( IDC_BEFORE );
		pBtn->ShowWindow( false );
		pBtn = ( CButton * )GetDlgItem( IDC_NEXT );
		pBtn->ShowWindow( false );

		SetWindowText( _T("�Է�") );

		break;

	case 2 :		// ���� 
	
		m_type = pParent->m_tab.GetCurSel() + 1;
		pBtn = ( CButton * )GetDlgItem( rdoID[ m_type - 1 ] );

		m_checkedDMRow = pParent->GetCheckedDMRow( m_rowCnt, m_type );

		SetWindowText( _T("����") );

		if( !m_rowCnt )
		{
			AfxMessageBox( _T("���ø���� �����ϴ�") );
			CDialog::EndDialog( 1 );
			return;
		}
		else
		{
			for( i = 0; i < 3; i++ )
			{
				pBtn = ( CButton * )GetDlgItem( rdoID[ i ] );
				
				if( m_type - 1 == i )
				{
					pBtn->EnableWindow( true );
					pBtn->SetCheck( true );
					SetControlStat();
				}
				else
				{
					pBtn->EnableWindow( false );
					pBtn->SetCheck( false );
				}
			}				

			ViewModifyData();
		}

		break;
	}	
}

VOID CMNG_CODE_SHELF_04::ViewModifyData()
{
	CESL_DataMgr *pDM;
	CString dmAlias[ 3 ] = {
		_T("DM_MNG_CODE_SHELF_01_1"), _T("DM_MNG_CODE_SHELF_01_2"), _T("DM_MNG_CODE_SHELF_01_3") };

	// 040318_HSS �׸��� Alias �߰�
	CString cmAlias[ 3 ] = {
		_T("���"), _T("��ġ"), _T("�з�") };

	pDM = FindDM( dmAlias[ m_type - 1 ] );

	const INT ctrlCnt = 2;
	CString ctrlAlias[ ctrlCnt ] = {
		_T("PUBLISH_FORM_CODE"), _T("SHELF_LOC_CODE") };

	CString ctrlData[ ctrlCnt ];
	pDM->GetCellData( ctrlAlias, ctrlCnt, ctrlData, m_checkedDMRow[ m_index ] );

	ctrlAlias[ 0 ] = _T("�������±���");
	ctrlAlias[ 1 ] = _T("�ڷ��");

	for( INT i = 0; i < 2; i++ )
		SendControlData( ctrlAlias[ i ], ctrlData[ i ] );

	// 040318_HSS �⺻�� �־��ֱ� +++++++++++++++++++++++++++
	//�������±���
	m_DefaultValue[0] = ctrlData[0];
	//��������
	m_DefaultValue[1] = m_pInfo->MANAGE_CODE;
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++

	switch( m_type )
	{
		case 1 :

			ctrlAlias[ 0 ] = _T("REG_CODE");
			pDM->GetCellData( ctrlAlias[ 0 ], m_checkedDMRow[ m_index ], ctrlData[ 0 ] );
			ctrlAlias[ 0 ] = _T("��ϱ���");
			SendControlData( ctrlAlias[ 0 ], ctrlData[ 0 ] );

			// 040318_HSS �⺻�� �־��ֱ� - ��ϱ���
			m_DefaultValue[2] = ctrlData[ 0 ];

			break;

		case 2 :

			ctrlAlias[ 0 ] = _T("SEPARATE_SHELF_CODE");
			pDM->GetCellData( ctrlAlias[ 0 ], m_checkedDMRow[ m_index ], ctrlData[ 0 ] );
			ctrlAlias[ 0 ] = _T("��ġ��ȣ");
			SendControlData( ctrlAlias[ 0 ], ctrlData[ 0 ] );

			// 040318_HSS �⺻�� �־��ֱ� - ��ġ��ȣ
			m_DefaultValue[2] = ctrlData[ 0 ];

			break;

		case 3 :

			CString ctrlAlias1[ 3 ] = {
				_T("CLASS_NO_TYPE"), _T("START_CLASS_NO"), _T("END_CLASS_NO") };

			pDM->GetCellData( ctrlAlias1, 3, ctrlData, m_checkedDMRow[ m_index ] );

			ctrlAlias1[ 0 ] = _T("�з�ǥ����");
			ctrlAlias1[ 1 ] = _T("�з���ȣ_����");
			ctrlAlias1[ 2 ] = _T("�з���ȣ_����");

			// 040318_HSS �⺻�� �־��ֱ� - �з�ǥ����
			m_DefaultValue[2] = ctrlData[ 0 ];

			for( i = 0; i < 3; i++ )
				SendControlData( ctrlAlias1[ i ], ctrlData[ i ] );

			break;
	}
	/*
	CESL_DataMgr *pDM;
	CString dmAlias[ 3 ] = {
		_T("DM_MNG_CODE_SHELF_01_1"), _T("DM_MNG_CODE_SHELF_01_2"), _T("DM_MNG_CODE_SHELF_01_3") };

	// 040318_HSS �׸��� Alias �߰�
	CString cmAlias[ 3 ] = {
		_T("���"), _T("��ġ"), _T("�з�") };

	pDM = FindDM( dmAlias[ m_type - 1 ] );

	const INT ctrlCnt = 3;
	CString ctrlAlias[ ctrlCnt ] = {
		_T("PUBLISH_FORM_CODE"), _T("MANAGE_CODE"), _T("SHELF_LOC_CODE") };

	CString ctrlData[ ctrlCnt ];
	pDM->GetCellData( ctrlAlias, ctrlCnt, ctrlData, m_checkedDMRow[ m_index ] );

	ctrlAlias[ 0 ] = _T("�������±���");
	ctrlAlias[ 1 ] = _T("��������");
	ctrlAlias[ 2 ] = _T("�ڷ��");

	for( INT i = 0; i < 3; i++ )
		SendControlData( ctrlAlias[ i ], ctrlData[ i ] );

	// 040318_HSS �⺻�� �־��ֱ� +++++++++++++++++++++++++++
	//�������±���
	m_DefaultValue[0] = ctrlData[0];
	//��������
	m_DefaultValue[1] = ctrlData[1];
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++

	switch( m_type )
	{
		case 1 :

			ctrlAlias[ 0 ] = _T("REG_CODE");
			pDM->GetCellData( ctrlAlias[ 0 ], m_checkedDMRow[ m_index ], ctrlData[ 0 ] );
			ctrlAlias[ 0 ] = _T("��ϱ���");
			SendControlData( ctrlAlias[ 0 ], ctrlData[ 0 ] );

			// 040318_HSS �⺻�� �־��ֱ� - ��ϱ���
			m_DefaultValue[2] = ctrlData[ 0 ];

			break;

		case 2 :

			ctrlAlias[ 0 ] = _T("SEPARATE_SHELF_CODE");
			pDM->GetCellData( ctrlAlias[ 0 ], m_checkedDMRow[ m_index ], ctrlData[ 0 ] );
			ctrlAlias[ 0 ] = _T("��ġ��ȣ");
			SendControlData( ctrlAlias[ 0 ], ctrlData[ 0 ] );

			// 040318_HSS �⺻�� �־��ֱ� - ��ġ��ȣ
			m_DefaultValue[2] = ctrlData[ 0 ];

			break;

		case 3 :

			ctrlAlias[ 0 ] = _T("CLASS_NO_TYPE");
			ctrlAlias[ 1 ] = _T("START_CLASS_NO");
			ctrlAlias[ 2 ] = _T("END_CLASS_NO");
			pDM->GetCellData( ctrlAlias, 3, ctrlData, m_checkedDMRow[ m_index ] );

			ctrlAlias[ 0 ] = _T("�з�ǥ����");
			ctrlAlias[ 1 ] = _T("�з���ȣ_����");
			ctrlAlias[ 2 ] = _T("�з���ȣ_����");

			// 040318_HSS �⺻�� �־��ֱ� - �з�ǥ����
			m_DefaultValue[2] = ctrlData[ 0 ];

			for( i = 0; i < 3; i++ )
				SendControlData( ctrlAlias[ i ], ctrlData[ i ] );

			break;
	}
	*/

	SetReverse(_T("CM_MNG_CODE_SHELF_01_01"), cmAlias[ m_type - 1 ], m_checkedDMRow[ m_index ] );
}

INT CMNG_CODE_SHELF_04::SendControlData( CString alias, CString data )
{
	SetControlData( _T("CM_MNG_CODE_SHELF_04_01"), alias, data, -1, 0 );

	return 0;
}

INT CMNG_CODE_SHELF_04::CheckOverlapData( CString publishFormCode, CString manageCode, CString expData )
{
	const INT cnt = 3;
	CString table[ cnt ] = {
		_T("MN_REG_CODE_SHELF_RULE_MNG_TBL"), 
		_T("MN_SEP_CODE_SHELF_RULE_MNG_TBL"),
		_T("MN_CLS_NUM_SHELF_RULE_MNG_TBL") };
	
	CString expField[ cnt ] = {
		_T("REG_CODE"), _T("SEPARATE_SHELF_CODE"), _T("CLASS_NO_TYPE") };

	// 040318_HSS ó�� �Է� �����Ͷ� �ٸ��� ���� ��� ���� 
	//	-> �ȱ׷��� �ڷ�Ǹ� ������ ���� ����
	if (m_DefaultValue[0] == publishFormCode		// �������±���
		&& m_DefaultValue[1] == manageCode			// ��������
		&& m_DefaultValue[2] == expData)			// ���/��ġ/�з�ǥ����
		return 0;
	
	CString query, result;
	query.Format( _T("SELECT COUNT( * ) FROM %s WHERE PUBLISH_FORM_CODE = '%s' AND MANAGE_CODE = '%s' AND %s = '%s'"),
		table[ m_type - 1 ], publishFormCode, manageCode, expField[ m_type - 1 ], expData );
			
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_SHELF_01_1") );
	pDM->GetOneValueQuery( query, result );
		
	return _ttoi( result );
}
HBRUSH CMNG_CODE_SHELF_04::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
