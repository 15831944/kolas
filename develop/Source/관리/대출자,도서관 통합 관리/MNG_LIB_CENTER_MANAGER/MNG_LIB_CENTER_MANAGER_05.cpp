// MNG_LIB_CENTER_MANAGER_05.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_LIB_CENTER_MANAGER_05.h"
#include "MNG_LIB_CENTER_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_05 dialog


CMNG_LIB_CENTER_MANAGER_05::CMNG_LIB_CENTER_MANAGER_05(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_CENTER_MANAGER_05)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CMNG_LIB_CENTER_MANAGER_05::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_LIB_CENTER_MANAGER_05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_CENTER_MANAGER_05)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_CENTER_MANAGER_05, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_CENTER_MANAGER_05)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, OnSelchangeCombo4)
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER1, OnKillfocusDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_CENTER_MANAGER_05 message handlers

BOOL CMNG_LIB_CENTER_MANAGER_05::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_LIB_CENTER_MANAGER_05") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_LIB_CENTER_MANAGER_05") );
		return false;
	}
	
	SetCtrlProperty();
	SetCtrlPropertyByPType();
	ViewLibEdtBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_LIB_CENTER_MANAGER_05::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::DestroyWindow();
}

VOID CMNG_LIB_CENTER_MANAGER_05::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	OnClose();
}

VOID CMNG_LIB_CENTER_MANAGER_05::SetCtrlProperty()
{
	CComboBox *pCombo;
	
	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	pCombo->InsertString( 0, _T("+") );
	pCombo->InsertString( 1, _T("-") );
	pCombo->SetCurSel( 0 );

	pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	pCombo->InsertString( 0, _T("�� ����") );
	pCombo->InsertString( 1, _T("�л�") );
	pCombo->InsertString( 2, _T("������") );
	pCombo->SetCurSel( 0 );
	
	MakeMailList();
	MakeLibList();
}

CString CMNG_LIB_CENTER_MANAGER_05::GetDate( INT type )
{
	/*
	type 1 : ����
	type 2 : �����
	*/

	CString date;

	UINT dateCtrlID[ 2 ] = {
		IDC_DATETIMEPICKER1, IDC_DATETIMEPICKER2 };

	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( dateCtrlID[ type - 1 ] );
	CTime time;
	UpdateData( false );
	
	pDate->GetTime( time );	
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

	return date;
}

VOID CMNG_LIB_CENTER_MANAGER_05::SetDefaultCivilNum()
{
	CString date = GetDate( 1 );
	INT year, mon, day, tmp;
	_stscanf( date, _T("%2d%2d/%2d/%2d"), &tmp, &year, &mon, &day );
	date.Format( _T("%02d%02d%02d"), year, mon, day );

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->SetWindowText( date );
}

// �źб����� ����Ǿ�����,
VOID CMNG_LIB_CENTER_MANAGER_05::OnSelchangeCombo3() 
{
	// TODO: Add your control notification handler code here
	
	SetCtrlPropertyByPType();
}

VOID CMNG_LIB_CENTER_MANAGER_05::SetCtrlPropertyByPType()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	INT type = pCombo->GetCurSel();

	CString caption[ 4 ] = { _T("�б� / ���� ��"), _T("�й� / ������ȣ"), _T("�� / ����"), _T("�г� / �μ�") };
	switch( type )
	{
		case 1 :
			caption[ 0 ] = _T("�� ��");
			caption[ 1 ] = _T("�� ��");
			caption[ 2 ] = _T("��");
			caption[ 3 ] = _T("�� ��");
			break;
		case 2 :
			caption[ 0 ] = _T("ȸ���");
			caption[ 1 ] = _T("������ȣ");
			caption[ 2 ] = _T("�� ��");
			caption[ 3 ] = _T("�� ��");
			break;
	}
	
	CWnd *pWnd;
	UINT wndID[ 4 ] = { IDC_STATIC_1, IDC_STATIC_2, IDC_STATIC_3, IDC_STATIC_4 };
	for( INT i = 0; i < 4; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->SetWindowText( caption[ i ] );
	}

	CEdit *pEdt;
	const INT edtCnt = 7;
	UINT edtID[ edtCnt ] = {
		IDC_EDIT9, IDC_EDIT10, IDC_EDIT11, IDC_EDIT12, IDC_EDIT15, IDC_EDIT13, IDC_EDIT14 };
	bool flag;
	if( type )
		flag = true;
	else
		flag = false;

	for( i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->EnableWindow( flag );
	}
}

VOID CMNG_LIB_CENTER_MANAGER_05::MakeLibList()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_05_libList") );
	pDM->RefreshDataManager( _T("") );
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO4 );
	INT cnt = pDM->GetRowCount();
	CString desc;

	pCombo->InsertString( 0, _T("�� ����") );
	for( INT i = 0; i < cnt; i++ )
	{
		pDM->GetCellData( _T("NAME"), i, desc );
		pCombo->InsertString( i + 1, desc );
	}
	pCombo->SetCurSel( 0 );	
}

VOID CMNG_LIB_CENTER_MANAGER_05::MakeMailList()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_05_mailList") );
	pDM->RefreshDataManager( _T("") );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	INT cnt = pDM->GetRowCount();
	CString desc;

	pCombo->InsertString( 0, _T("") );
	for( INT i = 0; i < cnt; i++ )
	{
		pDM->GetCellData( _T("DESCRIPTION"), i, desc );
		pCombo->InsertString( i + 1, desc );
	}
	pCombo->SetCurSel( 0 );
}

VOID CMNG_LIB_CENTER_MANAGER_05::OnSelchangeCombo4() 
{
	// TODO: Add your control notification handler code here
	
	ViewLibEdtBox();
}

VOID CMNG_LIB_CENTER_MANAGER_05::ViewLibEdtBox()
{
	CWnd *pWnd;
	const INT wndCnt = 5;
	UINT wndID[ wndCnt ] = { IDC_EDIT16, IDC_EDIT17, IDC_EDIT18, IDC_EDIT19, IDC_DATETIMEPICKER2 };
		
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO4 );
	bool flag = false;
	if( pCombo->GetCurSel() )
		flag = true;

	for( INT i = 0; i < wndCnt; i++ )
	{
		pWnd = GetDlgItem( wndID[ i ] );
		pWnd->EnableWindow( flag );
	}
}

// CO_MASTER_LOAN_USER_TBL �� ���� �� ������
VOID CMNG_LIB_CENTER_MANAGER_05::GetCenterData( CString data[] )
{
	CWnd *pWnd;

	pWnd = ( CWnd * )GetDlgItem( IDC_EDIT1 );
	pWnd->GetWindowText( data[ 0 ] );

	CString tmp1, tmp2;
	
	pWnd = ( CWnd * )GetDlgItem( IDC_EDIT2 );
	pWnd->GetWindowText( tmp1 );
	pWnd = ( CWnd * )GetDlgItem( IDC_EDIT3 );
	pWnd->GetWindowText( tmp2 );
	data[ 1 ].Format( _T("%s-%s"), tmp1, tmp2 );

	pWnd = ( CWnd * )GetDlgItem( IDC_EDIT4 );
	pWnd->GetWindowText( tmp1 );
	pWnd = ( CWnd * )GetDlgItem( IDC_COMBO2 );
	pWnd->GetWindowText( tmp2 );
	data[ 2 ].Format( _T("%s%s"), tmp1, tmp2 );

	const INT cnt = 11;
	UINT wndID[ cnt ] = {
		IDC_EDIT5,  IDC_EDIT6,  IDC_EDIT7,  IDC_EDIT8,  IDC_EDIT9, 
		IDC_EDIT10, IDC_EDIT11, IDC_EDIT12, IDC_EDIT15, IDC_EDIT13, 
		IDC_EDIT14 };
	
	for( INT i = 0; i < cnt; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->GetWindowText( data[ i + 3 ] );
	}

	data[ 14 ] = GetDate( 1 );
	
	pWnd = ( CWnd * )GetDlgItem( IDC_COMBO1 );
	pWnd->GetWindowText( data[ 15 ] );

	pWnd = ( CWnd * )GetDlgItem( IDC_COMBO3 );
	pWnd->GetWindowText( tmp1 );
	if( _T("�л�") == tmp1 )
		data[ 16 ].Format( _T("S") );
	else if( _T("������") == tmp1 )
		data[ 16 ].Format( _T("C") );
	else
		data[ 16 ].Empty();
}

// CO_LIBRARY_USER_TBL �� ���� �� ������
VOID CMNG_LIB_CENTER_MANAGER_05::GetLibData( CString data[] )
{
	CEdit *pEdt;
	const INT edtCnt = 4;
	UINT edtID[ edtCnt ] = {
		IDC_EDIT16, IDC_EDIT17, IDC_EDIT18, IDC_EDIT19 };
	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->GetWindowText( data[ i ] );
	}

	data[ 4 ] = GetCurrentLibKey();

	data[ 5 ] = GetDate( 2 );
}

// ����
VOID CMNG_LIB_CENTER_MANAGER_05::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT cCnt = 20;
	const INT lCnt = 9;
	
	CString cData[ cCnt ];
	CString lData[ lCnt ];

	GetCenterData( cData );
	GetLibData( lData );
	
	for( INT i = 0; i < 20; i++ )
		if( i < 9 )
		{
			AllTrim( cData[ i ] );
			AllTrim( lData[ i ] );
		}
		else
			AllTrim( cData[ i ] );

/*
Error Check List

1. �ֹι�ȣ �ߺ�, ID�ߺ�
	-1 : �ش� �������� ���� ID�� �������
	 0 : ���� �� ������ϰ��
	 1 : master table�� �ߺ��� �ֹι�ȣ�� ���� ���, Update
	 2 : master table�� �ߺ��� �ֹι�ȣ�� ���� ���, �׳� �д�.
	-2 : �Է� ���

2. ����� �� �ֹε�Ϲ�ȣ���� �˻�

3. �������� �����Ͽ����� �˻�
*/

	if( !lData[ 4 ].GetLength() )
	{
		AfxMessageBox( _T("��ϵ� �������� �����Ͽ� �ֽʽÿ�") );
		return;
	}

	if( !lData[ 1 ].GetLength() )
	{
		AfxMessageBox( _T("ID�� �Է��Ͽ��ֽʽÿ�") );
		return;
	}

	if( ErrorCheckCivilNum( cData[ 1 ] ) )
	{
		AfxMessageBox( _T("�߸��� �ֹε�Ϲ�ȣ �Դϴ�.") );
		return;
	}
	
	INT type = GetInputType( lData[ 1 ], lData[ 4 ], cData[ 1 ], cData[ 17 ] );
	if( -1 == type )
	{
		AfxMessageBox( _T("���� ���õ� �������� �ߺ��� ID�� �����մϴ�.") );
		return;
	}
	else if( -2 == type )
		return;

	if( SaveData( cData, lData, type ) )
	{
		AfxMessageBox( _T("Error : INT CMNG_LIB_CENTER_MANAGER_05::SaveData( CString cData[], CString, lData[], INT type )") );
		return;
	}

	AfxMessageBox( _T("����Ǿ����ϴ�.") );
	ShowGrid();
	
	CDialog::OnOK();
}

VOID CMNG_LIB_CENTER_MANAGER_05::OnKillfocusDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	SetDefaultCivilNum();

	*pResult = 0;
}

VOID CMNG_LIB_CENTER_MANAGER_05::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	SetDefaultCivilNum();

	*pResult = 0;
}


CString CMNG_LIB_CENTER_MANAGER_05::GetCurrentLibKey()
{
	CString pk;
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO4 );
	INT idx = pCombo->GetCurSel();
	if( !idx )
		return _T("");
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_05_libList") );
	pDM->GetCellData( _T("REC_KEY"), idx - 1, pk );

	return pk;
}

INT CMNG_LIB_CENTER_MANAGER_05::ErrorCheckCivilNum( CString num )
{
	if( 14 != num.GetLength() )
		return -1;

	INT locate = num.Find( '-' );
	if( 6 != locate )
		return -2;

	CString frontNum = num.Left( 6 );
	
	INT n;
	n = _ttoi( frontNum.Mid( 2, 2 ) );
	if( n > 12 || n < 1 )
		return -3;

	n = _ttoi( frontNum.Mid( 4, 2 ) );
	if( n > 31 || n < 1 )
		return -4;
	
	return 0;
}

INT CMNG_LIB_CENTER_MANAGER_05::GetInputType( CString id, CString libKey, CString num, CString &mPK )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_05_libList") );
	
	CString query, result;
	query.Format( _T("SELECT COUNT( * ) FROM CO_LIBRARY_USER_TBL WHERE REC_KEY = '%s' AND USER_ID = '%s'"), libKey, id );
	pDM->GetOneValueQuery( query, result );
	if( _ttoi( result ) )
		return -1;

	query.Format( _T("SELECT REC_KEY FROM CO_MASTER_LOAN_USER_TBL WHERE CIVIL_NO = '%s'"), num );
	pDM->GetOneValueQuery( query, result );
	if( _ttoi( result ) )
	{
		query.Format( _T("MASTER TABLE�� �ߺ��� �ֹε�Ϲ�ȣ�� �����մϴ�.\r\n\r\nMASTER TABLE�� UPDATE �Ͻðڽ��ϱ�?\t[    ��    ]\r\n������ ������ �Է��Ͻðڽ��ϱ�?\t\t[ �ƴϿ� ]\r\n�Է��� ����Ͻðڽ��ϱ�?\t\t\t[   ���  ]") );
		UINT flag = AfxMessageBox( query, MB_YESNOCANCEL );
		
		switch( flag )
		{
			case IDYES :
				mPK = result;
				return 1;
				break;
			case IDNO :
				return 2;
				break;
			case IDCANCEL :
				return -2;
				break;
		}	
	}
	else
		pDM->MakeRecKey( mPK );
	
	return 0;
}

VOID CMNG_LIB_CENTER_MANAGER_05::AllTrim(CString &string)
{
	string.TrimLeft();
	string.TrimRight();
}

INT CMNG_LIB_CENTER_MANAGER_05::SaveData( CString cData[], CString lData[], INT type )
{
	if( 2 != type && SaveMasterData( cData, type ) )
		return -1;

	if( SaveLibData( lData, cData[ 17 ] ) )
		return -2;
	
	return 0;
}

/*
	_T("NAME"),				//	0
	_T("CIVIL_NO"),			//	1
	_T("E_MAIL"),			//	2
	_T("H_ZIPCODE"),	//	3
	_T("H_ADDR1"),		//	4
	_T("H_PHONE"),		//	5
	_T("HANDPHONE"),		//	6
	_T("P_NAME"),			//	7
	_T("P_NUM"),			//	8
	_T("P_GROUP"),			//	9
	_T("P_LEVEL"),			// 10
	_T("W_PHONE"),		// 11
	_T("W_ZIPCODE"),	// 12
	_T("W_ADDR1"),		// 13
	_T("BIRTHDAY"),			// 14
	_T("DATE_TYPE"),		// 15
	_T("P_TYPE"),			// 16
	_T("REC_KEY"),			// 17
	_T("MAKE_DATE"),		// 18
	_T("FIRST_WORK"),		// 19
*/

/*
	_T("USER_NO"),		//	0 
	_T("USER_ID"),		//	1
	_T("PASSWORD"),		//	2
	_T("FAMILY_ID"),	//	3
	_T("LIB_KEY"),		//	4
	_T("MAKE_DATE"),	//	5
	_T("FIRST_WORK"),	//	6
	_T("REC_KEY"),		//	7
	_T("USER_KEY")		//	8
*/

INT CMNG_LIB_CENTER_MANAGER_05::SaveMasterData( CString data[], INT type )
{
	const INT fieldCnt = 20;
	CString fieldName[ fieldCnt ] = {
		_T("NAME"),				//	0
		_T("CIVIL_NO"),			//	1
		_T("E_MAIL"),			//	2
		_T("H_ZIPCODE"),	//	3
		_T("H_ADDR1"),		//	4
		_T("H_PHONE"),		//	5
		_T("HANDPHONE"),		//	6
		_T("P_NAME"),			//	7
		_T("P_NUM"),			//	8
		_T("P_GROUP"),			//	9
		_T("P_LEVEL"),			// 10
		_T("W_PHONE"),		// 11
		_T("W_ZIPCODE"),	// 12
		_T("W_ADDR1"),		// 13
		_T("BIRTHDAY"),			// 14
		_T("DATE_TYPE"),		// 15
		_T("P_TYPE"),			// 16
		_T("REC_KEY"),			// 17
		_T("MAKE_DATE"),		// 18
		_T("FIRST_WORK") };		// 19
	
	CString fieldType[ fieldCnt ] = {
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("NUMERIC"), _T("STRING"), _T("STRING") };		

	if( 1 == type )
	{
		fieldName[ 18 ] = _T("UPDATE_DATE");
		fieldName[ 19 ] = _T("LAST_WORK");
	}

	data[ 18 ] = GetCurrentDate();
	data[ 19 ] = GetWorkLogMsg( _T("���������Ͱ���"), __WFILE__, __LINE );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_01") );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( data[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i ] );

	if( 1 == type )
		pDM->MakeUpdateFrame( _T("CO_MASTER_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 17 ] );
	else
		pDM->MakeInsertFrame( _T("CO_MASTER_LOAN_USER_TBL") );

	AddDataToMasterDM( pDM, fieldName, data, fieldCnt, type );

	pDM->SendFrame();
	pDM->EndFrame();
	
	return 0;
}

INT CMNG_LIB_CENTER_MANAGER_05::SaveLibData( CString data[], CString userKey )
{
	const INT fieldCnt = 9;

	CString fieldName[ fieldCnt ] = {
		_T("USER_NO"),		//	0 
		_T("USER_ID"),		//	1
		_T("PASSWORD"),		//	2
		_T("FID_KEY"),	//	3
		_T("LIB_KEY"),		//	4
		_T("MAKE_DATE"),	//	5
		_T("FIRST_WORK"),	//	6
		_T("REC_KEY"),		//	7
		_T("USER_KEY") };	//	8

	CString fieldType[ fieldCnt ] = {
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC"), _T("NUMERIC") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_CENTER_MANAGER_05_libList") );
	
	data[ 5 ] = GetCurrentDate();
	data[ 6 ] = GetWorkLogMsg( _T("���������Ͱ���"), __WFILE__, __LINE );
	pDM->MakeRecKey( data[ 7 ] );
	data[ 8 ] = userKey;

	for( INT i = 0; i < fieldCnt; i++ )
		if( data[ i ].GetLength() )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i ] );

	pDM->MakeInsertFrame( _T("CO_LIBRARY_USER_TBL") );

	pDM->SendFrame();
	pDM->EndFrame();	

	return 0;

}

CString CMNG_LIB_CENTER_MANAGER_05::GetCurrentDate()
{
	CString date;

	CTime time = CTime::GetCurrentTime();
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );

	return date;
}

INT CMNG_LIB_CENTER_MANAGER_05::AddDataToMasterDM( CESL_DataMgr *pDM, CString fieldName[], CString data[], const INT cnt, INT type )
{
	INT row = -1;
	CString tmp;
	if( 1 == type )
		for( INT i = 0; i < pDM->GetRowCount(); i++ )
		{
			pDM->GetCellData( _T("REC_KEY"), i, tmp );
			if( tmp == data[ 17 ] )
			{
				row = i;
				break;
			}
		}

	pDM->SetCellData( fieldName, cnt, data, row );
	
	return 0;
}

VOID CMNG_LIB_CENTER_MANAGER_05::ShowGrid()
{
	CMNG_LIB_CENTER_MANAGER_01 *pParent = ( CMNG_LIB_CENTER_MANAGER_01 * )pParentMgr;
	pParent->ShowGrid();
}
