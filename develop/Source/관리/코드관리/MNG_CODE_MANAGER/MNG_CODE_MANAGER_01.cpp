// MNG_CODE_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CODE_MANAGER_01.h"
#include "..\..\�谡���ذ���\ShelfCodeGetter\ShelfCodeGetterDlg.h"
//#include _T("../../../esl/kolas2up_main/kolas2up_mainapi.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_01 dialog


CMNG_CODE_MANAGER_01::CMNG_CODE_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_MANAGER_01)
	//}}AFX_DATA_INIT

	m_pMenu = NULL;
}
CMNG_CODE_MANAGER_01::~CMNG_CODE_MANAGER_01()
{
	delete m_pLoanShareManager;
}

BOOL CMNG_CODE_MANAGER_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	


VOID CMNG_CODE_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_MANAGER_01)
	DDX_Control(pDX, IDC_COMBO3, m_combo_2);
	DDX_Control(pDX, IDC_COMBO2, m_codePattern);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_MANAGER_01)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(4000,4003,OnMSGGrid1)
	ON_COMMAND_RANGE(5000,5003,OnMSGGrid2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_01 message handlers

BOOL CMNG_CODE_MANAGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CODE_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CODE_MANAGER_01") );
		return false;
	}

	m_pLoanShareManager = new CLoanShareManager(this);
	m_pLoanShareManager->Create(this);
	
	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();

	m_codeClass = 0;
	
	INT width[ 14 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 };
	SetGridWidth( width );
	
	const INT gridCnt = 2;
	CString gridAlias[ gridCnt ] = { 
		_T("�׸���"), _T("�׸���_2") };
	CArray< CString, CString >removeListArray;
	CESL_Grid *pGrid = NULL;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_CODE_MANAGER_01"), gridAlias[ i ] );
		if( !pGrid )
			return -1;

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );
	}

	SetTabControl();
	ViewCodeClass();
	ViewCodePattern();
	ViewTabControl( 0 );
	ViewToolBarBtns();

	if(m_pInfo->MODE==10000)
	{
		CString strButtonAlias ;
		strButtonAlias = _T("�ڵ��Է�");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
		strButtonAlias = _T("�ڵ����");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
		strButtonAlias = _T("�ڵ����");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
		strButtonAlias = _T("��������");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);	
		strButtonAlias = _T("�����߰�");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);
		strButtonAlias = _T("���ϼ���");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);
		strButtonAlias = _T("���ϻ���");
		pMain->SendMessage(WM_USER+2000, (WPARAM)&strButtonAlias, TRUE);
	}

	// 2009.07.02 ADD BY PWR : å�η����� ������ �߰�
	CString strValue;
	INT ids = GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η�����"), strValue);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("OnInitDialog"));
	}
	if(strValue.IsEmpty()) 
	{
		m_sBookCooperativeMode = _T("I");
	}
	else
	{
		m_sBookCooperativeMode = strValue;
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CMNG_CODE_MANAGER_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_CODE_MANAGER_01)
	ON_EVENT(CMNG_CODE_MANAGER_01, IDC_mngGRID_1, -605 /* MouseDown */, OnMouseDownmngGRID1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMNG_CODE_MANAGER_01, IDC_mngGRID_2, -605 /* MouseDown */, OnMouseDownmngGRID2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

//VOID CMNG_CODE_MANAGER_01::OnClickmngGRID1() 
//{
//	// TODO: Add your control notification handler code here
///*
//	INT row, col;
//	GetCurrentGridCell( row, col );
//
//	// click�� �÷��� 2 �϶� ����Ʈ ���� üũ�Ѵ�. 
//	
//	switch( col )
//	{
//		case 2 :
//			CheckActiveValue( row );
//			break;
//		case 3 :
//			CheckDefaultValue( row );
//			break;
//		default :
//			break;
//	}	
//*/
//}

VOID CMNG_CODE_MANAGER_01::GetCurrentGridCell( INT &row, INT &col, INT option )
{
	CMSHFlexGrid *pGrid;
	
	if( option )
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	else
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	row = pGrid->GetRow();
	col = pGrid->GetCol();
}

// default value�� �ϳ��� ���õǵ��� �Ѵ�. 
VOID CMNG_CODE_MANAGER_01::CheckDefaultValue( INT row, INT option )
{
	CMSHFlexGrid *pGrid;
	CESL_DataMgr *pDM;
	CString alias;
	if( option )
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
		pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_codePatternClass") );
		alias.Format( _T("P_DEFAULT_VALUE") );
	}
	else
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
		pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
		alias.Format( _T("U_DEFAULT_VALUE") );
	}	 
	
	if( _T("V") == pGrid->GetText() )
	{
		pGrid->SetTextMatrix( row, 3, _T("") );
		pDM->SetCellData( _T("�⺻"), _T(""), row - 1 );
		pDM->SetCellData( alias, _T("N"), row - 1 );
		return;
	}
	else
	{
		pDM->SetCellData( _T("�⺻"), _T("V"), row - 1 );
		pDM->SetCellData( alias, _T("Y"), row - 1 );
		pGrid->SetTextMatrix( row, 3, _T("V") );
	}
	
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		if( row == i )
			continue;
		
		pGrid->SetTextMatrix( i, 3, _T("") );
		pDM->SetCellData( _T("�⺻"), _T(""), i - 1 );
		pDM->SetCellData( alias, _T("N"), i - 1 );
	}

	pGrid->SetRow( row );
}

// �׸����� ���̸� �����Ѵ�. 
INT CMNG_CODE_MANAGER_01::SetGridWidth( INT width[], INT option )
{
	CMSHFlexGrid *pGrid;
	if( option )
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	else
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	for( INT i = 0; i < 14; i++ )
		pGrid->SetColWidth( i + 6, 0, width[ i ] );
	
	return 0;
}

VOID CMNG_CODE_MANAGER_01::CheckActiveValue( INT row, INT option )
{
	CMSHFlexGrid *pGrid;
	CESL_DataMgr *pDM;
	CString alias;
	if( option )
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
		pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_codePatternClass") );
		alias.Format( _T("P_ACTIVE") );
	}
	else
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
		pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
		alias.Format( _T("U_ACTIVE") );
	}	 
	
	if( _T("V") == pGrid->GetText() )
	{
		pGrid->SetTextMatrix( row, 2, _T("") );
		pDM->SetCellData( _T("����"), _T(""), row - 1 );
		pDM->SetCellData( alias, _T("N"), row - 1 );
	}
	else
	{
		pGrid->SetTextMatrix( row, 2, _T("V") );
		pDM->SetCellData( _T("����"), _T("V"), row - 1 );
		pDM->SetCellData( alias, _T("Y"), row - 1 );
	}
}

// code ����
INT CMNG_CODE_MANAGER_01::DeleteCode()
{
	TCHAR permition = GetUserPermition();
	CString error;
	
	//===================================================
	//2009.10.16 ADD BY LKS : CD_CODE_TBL ��������
	if( (m_pInfo->MODE != 10000) && (m_pInfo->MODE != 30000) )
	{
	//===================================================
		// 2010.01.25 UPDATE BY PWR : ���հ����� �߰�
		if( 'A' != permition && 'S' != permition )
// 		if( 'A' != permition && 'M' != permition )
		{
			error.Format( _T("%s ���� ���� ������ �����ϴ�."), GetUserID() );
			AfxMessageBox( error );
			return -1;
		}
	}
	
	INT *checkedDMRow, checkedRowCnt;
	checkedDMRow = GetCheckedRowInfo( checkedRowCnt, 1 );

	if( !checkedRowCnt )
	{
		AfxMessageBox( _T("������ �ڵ尡 �����ϴ�.") );
		return -2;
	}	

	CString strCodeClass;
	CString strCodeList;
	CString strCode;
	CString strQuery;
	CString strCount;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );

	strCodeClass = _T("");
	pDM->GetCellData( _T("C_CLASS")	, checkedDMRow[ 0 ], strCodeClass );
	for( INT i = checkedRowCnt - 1; i > -1; i-- )
	{		
		pDM->GetCellData( _T("C_CODE"), checkedDMRow[ i ], strCode );
		if  (strCodeList != _T("") ) strCodeList += _T(",");
		strCodeList += _T("'")+strCode+_T("'");
	}

	CString msg;
	msg = _T("");
	if ( strCodeClass == _T("19") )
	{
		strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE SHELF_LOC_CODE IN ( %s )"), strCodeList);
		pDM->GetOneValueQuery(strQuery, strCount);
		if ( _ttoi(strCount) > 0 ) 
		{
			msg.Format( _T("������ %d ���� �ڵ带 ���� �Ͻðڽ��ϱ�?\n")
			_T("�� ���ǰ� �ִ� �ڵ��Դϴ�. \n")
			_T("   ������ ��� ȭ�鿡�� �ش� �ڵ尡 ���������� �������� �ʰԵ˴ϴ�.\n"), checkedRowCnt );			
			if( IDNO == AfxMessageBox( msg, MB_YESNO ) )
			return -3;	
		}
	}
	else
	{
		// 2009.07.15 ADD BY PWR : �л����սý��ۿ����� ���������� �Է�,����,���� �Ұ�
		if(_T("D") == m_sBookCooperativeMode)
		{
			if(_T("1") == strCodeClass)
			{
				msg = _T("�л����սý��ۿ����� ���������� ������ �Ұ��մϴ�.");
				AfxMessageBox(msg);
				return -3;
			}
		}
		msg.Format( _T("������ %d ���� �ڵ带 ���� �Ͻðڽ��ϱ�?\n")
		_T("�� ���ǰ� �ִ� �ڵ尡 ������ ��� ȭ�鿡�� �ش� �ڵ尡 ���������� �������� �ʰԵ˴ϴ�.\n")
		_T("   ��������� Ȯ���Ͻð� �����Ͽ� �ֽʽÿ�."), checkedRowCnt );
		if( IDNO == AfxMessageBox( msg, MB_YESNO ) )
		return -3;	
	}		
	
	CString query, pk, sCode, sDesc;
	
	pDM->StartFrame();
	pDM->InitDBFieldData();

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("62");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("MN_LOAN_ENV_POSREG_TBL");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pInfo->LIB_CODE;
	for( i = checkedRowCnt - 1; i > -1; i-- )
	{
		pDM->GetCellData(_T("C_REC_KEY"), checkedDMRow[i], pk);
		pDM->GetCellData(_T("C_CODE"), checkedDMRow[i], sCode);
		pDM->GetCellData(_T("C_DESCRIPTION"), checkedDMRow[i], sDesc);
		for( INT j = 0; j < 3; j++ )
		{
			query = MakeCodeDelQuery(pk, j + 1);
			query += _T(";");

			pDM->AddFrame( query );
			// 2008.12.10 ADD BY PWR : ������������(CLASS_CODE=31)������ ���������������̺�(CD_INDIVIDUAL_CLASS_TBL)���� �����ϵ����Ѵ�.
			if(m_codeClass == 31 && j==0)
			{
				query.Format(_T("DELETE FROM CD_INDIVIDUAL_CLASS_TBL WHERE CODE='%s';"), sCode);
				pDM->AddFrame(query);
				
				CESL_DataMgr TmpDM;
				TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
				query.Format(_T("SELECT REC_KEY FROM MN_LOAN_ENV_POSREG_TBL WHERE LOAN_CLASS_CODE_KEY=")
							 _T("(SELECT REC_KEY FROM CD_CODE_TBL WHERE REC_KEY=%s)"), pk);
				TmpDM.RestructDataManager(query);
				for(INT i = 0; i < TmpDM.GetRowCount(); i++)
				{
					UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = TmpDM.GetCellData(i, 0);
					m_pLoanShareManager->SendLoanInfoUpload(UploadShareInfo);
				}

				query.Format(_T("DELETE FROM MN_LOAN_ENV_POSREG_TBL WHERE LOAN_CLASS_CODE_KEY=%s;"), pk);
				pDM->AddFrame(query);
			}
			// 2009.07.22 ADD BY PWR : ��ϱ���(4)������ MN_LOAN_ENV_POSREG_TBL�� �ش�Ű�� ����
			if(m_codeClass == 4 && j==0)
			{
				CESL_DataMgr TmpDM;
				TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
				query.Format(_T("SELECT REC_KEY FROM MN_LOAN_ENV_POSREG_TBL WHERE REG_CODE_KEY=")
							 _T("(SELECT REC_KEY FROM CD_CODE_TBL WHERE REC_KEY=%s)"), pk);
				TmpDM.RestructDataManager(query);
				for(INT i = 0; i < TmpDM.GetRowCount(); i++)
				{
					UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = TmpDM.GetCellData(i, 0);
					m_pLoanShareManager->SendLoanInfoUpload(UploadShareInfo);
				}

				query.Format(_T("DELETE FROM MN_LOAN_ENV_POSREG_TBL WHERE REG_CODE_KEY=%s;"), pk);
				pDM->AddFrame(query);
			}
		}
		pDM->DeleteRow( checkedDMRow[i] );
	}
	if( pDM->SendFrame() )
	{
		AfxMessageBox( _T("�ڵ� ������ �����Ͽ����ϴ�.") );
		return -1;
	}
	pDM->EndFrame();

	// 2009.07.07 ADD BY PWR : ���� �������� �˸��� 
	if(_T("D") == m_sBookCooperativeMode)
	{
		if(4 == m_codeClass || 19 == m_codeClass || 30 == m_codeClass || 31 == m_codeClass)
		{
			CString sClass;
			sClass.Format(_T("%d"), m_codeClass);
			CString sUpdateFields = sClass+_T(",'")+sCode+_T("','")+sDesc+_T("','")+m_pInfo->USER_ID+_T("','")+m_pInfo->MANAGE_CODE+_T("'");
			UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
			UploadShareInfo.UploadShareLoanInfo.sCommand = _T("91");
			UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pInfo->LIB_CODE;
			UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = pk;
			UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;
			m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		}
	}
	// 2009.08.12 ADD BY PWR : ���� ������ 61�� Ŀ�ǵ�� ���Ϳ� �˸���
	if(31 == m_codeClass)
	{
		UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("61");
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pInfo->LIB_CODE;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = pk;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("");
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	}
	
	AfxMessageBox( _T("�����Ǿ����ϴ�.") );
	ShowGrid();
	
	free( checkedDMRow );
	
	return 0;
}

CString CMNG_CODE_MANAGER_01::MakeCodeDelQuery( CString pk, INT option )
{
	CString query, tmp, fieldName;
		
	switch( option )
	{
		case 1 :
			query.Format( _T("DELETE FROM CD_CODE_TBL WHERE ") );
			fieldName.Format( _T("REC_KEY") );
			break;
		case 2 :
			query.Format( _T("DELETE FROM MN_CODE_PATTERN_TBL WHERE ") );
			fieldName.Format( _T("CODE_KEY") );
			break;
		case 3 :
			query.Format( _T("DELETE FROM MN_USER_CODE_TBL WHERE ") );
			fieldName.Format( _T("CODE_KEY") );
			break;
	}

	tmp.Format( _T("%s = %s OR "), fieldName, pk );
	query += tmp;
	
	return query.Left( query.GetLength() - 3 );
}

INT * CMNG_CODE_MANAGER_01::GetCheckedRowInfo( INT &rowCnt, INT option )
{
	// 1 == option : ���� �÷�
	// 2 == option : ������ �÷�
	// 3 == option : �⺻�� �÷�

	INT *checkedRow, i;
	rowCnt = 0;

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	INT allRowCount = pGrid->GetRows();
	for( i = 1; i < allRowCount; i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, option ) )
			rowCnt++;

	checkedRow = ( INT * )calloc( rowCnt, sizeof( INT ) );

	INT idx = 0;
	for( i = 1; i < allRowCount; i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, option ) )
			checkedRow[ idx++ ] = i - 1;

	return checkedRow;
}

TCHAR CMNG_CODE_MANAGER_01::GetUserPermition()
{
	// 2010.01.25 UPDATE BY PWR : �̹� ������ ���� Ȱ��
	return m_pInfo->USER_PRIVILEGE.GetAt(0);
// 	CString query, result;
// 	query.Format( _T("SELECT PERMITION FROM MN_USER_TBL WHERE ID = '%s'"), GetUserID() );
// 	
// 	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
// 	pDM->GetOneValueQuery( query, result );
// 
// 	TCHAR permition;
// 	result.MakeUpper();
// 	wmemcpy( &permition, &result.GetBuffer( 0 )[ 0 ], 1 );
// 		
// 	return permition;
}

INT CMNG_CODE_MANAGER_01::ShowGrid( INT option )
{
	ConvertSelectType( option );
	
	CMSHFlexGrid *pGrid;
	
	if( option )
	{
		ControlDisplay( _T("CM_MNG_CODE_MANAGER_01"), _T("�׸���_2") );
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	}
	else
	{
		ControlDisplay( _T("CM_MNG_CODE_MANAGER_01"), _T("�׸���") );
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	}
	
	INT width[ 14 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0 };
	switch( GetCodeClassNum( option ) )
	{
/*
		case 1 : 
			width[ 13 ] = 1500;
			SetGridText( 0, 13, _T("��������ȣ"), option );
*/			
			break;
		case 2 :	// ����ó ����
		
			width[  5 ] = 1500;
			width[  6 ] = 1500;
			width[  7 ] = 1500;
			width[  8 ] = 1500;
			width[  9 ] = 1500;
			width[ 10 ] = 1500;
			width[ 11 ] = 1500;
			width[ 12 ] = 1500;
			SetGridText( 0, 11, _T("�����ȣ"), option );

			break;
	
		case 11 :	// ��ġ��ȣ

			width[ 4 ] = 1500;

			break;
/*
		case 31 :	// ������������
			
			width[ 0 ] = 1500;
			width[ 1 ] = 1500;
			width[ 2 ] = 1500;
			width[ 3 ] = 1500;
			width[ 5 ] = 1500;
			SetGridText( 0, 11, _T("�̿����ڷ�"), option );

			break;
*/
		default :	// �׿�

			break;
	}
	SetGridWidth( width, option );
	
	return 0;
}

VOID CMNG_CODE_MANAGER_01::SaveModifyedUserCode()
{
	CopyGridDataToDM();

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
	pDM->StartFrame();
	pDM->InitDBFieldData();
	
	CString activeFlag, defaultValue, pk;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("U_ACTIVE"), i, activeFlag );
		pDM->GetCellData( _T("U_DEFAULT_VALUE"), i, defaultValue );
		pDM->GetCellData( _T("U_REC_KEY"), i, pk );

		pDM->AddDBFieldData( _T("ACTIVE"), _T("STRING"), activeFlag );
		pDM->AddDBFieldData( _T("DEFAULT_VALUE"), _T("STRING"), defaultValue );
	
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("�ڵ����"), __WFILE__, __LINE__ ), TRUE );
		pDM->MakeUpdateFrame( _T("MN_USER_CODE_TBL"), _T("REC_KEY"), _T("NUMERIC"), pk );
	}

	CString error;
	INT ids = pDM->SendFrame();
	if( ids < 0 )
	{
		error.Format( _T("Error %d : SendFrame()"), ids );
		AfxMessageBox( error );
		return;
	}

	pDM->EndFrame();

	AfxMessageBox( _T("���� ������ ����Ǿ����ϴ�.") );
}

VOID CMNG_CODE_MANAGER_01::CopyGridDataToDM()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );

	INT rowCnt = pDM->GetRowCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		if( _T("V") == pGrid->GetTextMatrix( i + 1, 2 ) )
			pDM->SetCellData( _T("U_ACTIVE"), _T("Y"), i );
		else
			pDM->SetCellData( _T("U_ACTIVE"), _T("N"), i );

		if( _T("V") == pGrid->GetTextMatrix( i + 1, 3 ) )
			pDM->SetCellData( _T("U_DEFAULT_VALUE"), _T("Y"), i );
		else
			pDM->SetCellData( _T("U_DEFAULT_VALUE"), _T("N"), i );
	}
}

// code pattern�� �߰��Ѵ�. 
VOID CMNG_CODE_MANAGER_01::AddCodePattern()
{
	CMNG_CODE_MANAGER_04 dlg( this );
	dlg.DoModal();

	m_patternName = dlg.m_patternName;	
}

// �ڵ� Ŭ���� ����� �ٲ������,
VOID CMNG_CODE_MANAGER_01::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here

	if( !m_combo.GetCurSel() )
		return;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
	bool fix = false;
	INT codeClass = GetCodeClassInfo( fix );
	pDM->RefreshDataManager( MakeCodeSelectQuery( codeClass ) );
	
	m_mode = 1;
	m_codeClass = codeClass;
	ShowGrid();	

	ViewToolBarBtns();
}

VOID CMNG_CODE_MANAGER_01::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	
}

VOID CMNG_CODE_MANAGER_01::ViewCodeClass()
{
	SelectCodeClass();
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_CODECLASS") );
	
	m_combo.InsertString( 0, _T(" - �� �� - ") );
	m_combo_2.InsertString( 0, _T(" - �� �� - ") );	
	
	CString className;
	INT rowCnt = pDM->GetRowCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		pDM->GetCellData( _T("CLASS_NAME"), i, className );
		m_combo.InsertString( i + 1, className );
		m_combo_2.InsertString( i + 1, className );
	}

	m_combo.SetCurSel( 0 );
	m_combo_2.SetCurSel( 0 );
}

INT CMNG_CODE_MANAGER_01::SelectCodeClass()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_CODECLASS") );
	CString where = _T(" USE_YN = 'Y'");
	pDM->RefreshDataManager( where );

	return 0;
}

INT CMNG_CODE_MANAGER_01::GetCodeClassNum( INT option )
{
	CString classNum;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_CODECLASS") );
	
	INT index = 0;
	if( option )
		index = m_combo_2.GetCurSel() - 1;
	else
		index = m_combo.GetCurSel() - 1;

	pDM->GetCellData( _T("CLASS_CODE"), index, classNum );

	return _ttoi( classNum );
}

CString CMNG_CODE_MANAGER_01::MakeCodeSelectQuery(INT codeClass)
{
	CString where;
	where.Format( _T(" C.CLASS = %d AND U.CODE_KEY = C.REC_KEY AND U.USER_KEY = %s"), codeClass, GetUserKey() );	

	return where;
}

CString CMNG_CODE_MANAGER_01::GetUserKey()
{
	CString query, result;
	query.Format( _T(" SELECT REC_KEY FROM MN_USER_TBL WHERE ID = '%s' "), GetUserID() );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
	pDM->GetOneValueQuery( query, result );
	
	return result;
}

VOID CMNG_CODE_MANAGER_01::SetTabControl()
{
	m_tab.InsertItem( 0, _T(" �ڵ� ���� ") );
	m_tab.InsertItem( 1, _T(" �ڵ� ���� ���� ") );
}

VOID CMNG_CODE_MANAGER_01::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	ViewTabControl( m_tab.GetCurSel() );
	ViewToolBarBtns();
	
	*pResult = 0;
}

VOID CMNG_CODE_MANAGER_01::ViewTabControl( INT idx )
{
	m_tab.SetCurSel( idx );

	CComboBox *pCombo_1 = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	CComboBox *pCombo_2 = ( CComboBox * )GetDlgItem( IDC_COMBO2 );			// 15001
	CComboBox *pCombo_3 = ( CComboBox * )GetDlgItem( IDC_COMBO3 );			// 15002
	CMSHFlexGrid *pGrid_1 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	CMSHFlexGrid *pGrid_2 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );	// 19001
	CStatic *pStatic_1 = (CStatic*)GetDlgItem( IDC_STATIC_CODE_1 );
	CStatic *pStatic_2 = (CStatic*)GetDlgItem( IDC_STATIC_CODE_2 );

	switch( idx )
	{
		case 0 :
			
			pCombo_1->ShowWindow( true );
			pCombo_2->ShowWindow( false );
			pCombo_3->ShowWindow( false );
			pGrid_1->ShowWindow( true );
			pGrid_2->ShowWindow( false );
			pStatic_1->SetWindowText( _T("�ڵ� Ŭ����") );
			pStatic_2->SetWindowText( _T("") );

			break;

		case 1 :

			pCombo_1->ShowWindow( false );
			pCombo_2->ShowWindow( true );
			pCombo_3->ShowWindow( true );
			pGrid_1->ShowWindow( false );
			pGrid_2->ShowWindow( true );
			pStatic_1->SetWindowText( _T("�ڵ� ����") );
			pStatic_2->SetWindowText( _T("�ڵ� Ŭ����") );

			break;

		default :
			break;
	}
}

VOID CMNG_CODE_MANAGER_01::ViewCodePattern()
{
	m_codePattern.ResetContent();
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_codePattern") );
	pDM->RefreshDataManager( _T("") );
	
	CString codePatternName;
	m_codePattern.InsertString( 0, _T(" - �� �� - ") );
	
	for( INT i = 0 ;i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("DISTINCT PATTERN_NAME"), i, codePatternName );

		m_codePattern.InsertString( i + 1, codePatternName );
	}
	m_codePattern.SetCurSel( 0 );	
}

// code pattern tab���� code class cobobox�� ����Ǿ�����.
VOID CMNG_CODE_MANAGER_01::OnSelchangeCombo3() 
{
	// TODO: Add your control notification handler code here
	
	SearchCodePatternClass();
	ViewToolBarBtns();
}

VOID CMNG_CODE_MANAGER_01::SearchCodePatternClass()
{
	if( !m_tab.GetCurSel() )
	{
		AfxMessageBox( _T("�ڵ� ���� TAB�� ���� �����Ͽ� �ֽʽÿ�") );
		return;
	}
	
	if( !m_codePattern.GetCurSel() )
		return;
	
	if( !m_combo_2.GetCurSel() )
		return;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_codePatternClass") );
	pDM->RefreshDataManager( MakeCodePatternClassSelectQuery() );

	m_mode = 2;
	ShowGrid( 1 );	
}

CString CMNG_CODE_MANAGER_01::MakeCodePatternClassSelectQuery()
{
	CString where, tmp, codePattern;
	m_codePattern.GetWindowText( codePattern );
	where.Format( _T(" C.CLASS = %d AND P.CODE_KEY = C.REC_KEY AND P.PATTERN_NAME = '%s'"), GetCodeClassNum( 1 ), codePattern );
	
	return where;
}

// code pattern gird�� click�Ǿ�����
//VOID CMNG_CODE_MANAGER_01::OnClickmngGRID2() 
//{
///*
//	// TODO: Add your control notification handler code here
//	INT row, col;
//	GetCurrentGridCell( row, col, 1 );
//
//	// click�� �÷��� 2 �϶� ����Ʈ ���� üũ�Ѵ�. 
//	
//	switch( col )
//	{
//		case 2 :
//			CheckActiveValue( row, 1 );
//			break;
//		case 3 :
//			CheckDefaultValue( row, 1 );
//			break;
//		default :
//			break;
//	}		
//*/
//}

// ���� ���� ����
VOID CMNG_CODE_MANAGER_01::SaveModifyedPattern()
{
	CopyPatternGridDataToDM();

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_codePatternClass") );
	if( !pDM->GetRowCount() )
		return;

	pDM->StartFrame();
	pDM->InitDBFieldData();
	
	CString activeFlag, defaultValue, pk;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("P_ACTIVE"), i, activeFlag );
		pDM->GetCellData( _T("P_DEFAULT_VALUE"), i, defaultValue );
		pDM->GetCellData( _T("P_REC_KEY"), i, pk );

		pDM->AddDBFieldData( _T("ACTIVE"), _T("STRING"), activeFlag );
		pDM->AddDBFieldData( _T("DEFAULT_VALUE"), _T("STRING"), defaultValue );
	
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("�ڵ����"), __WFILE__, __LINE__ ), TRUE );
		pDM->MakeUpdateFrame( _T("MN_CODE_PATTERN_TBL"), _T("REC_KEY"), _T("NUMERIC"), pk );
	}

	CString error;
	INT ids = pDM->SendFrame();
	if( ids < 0 )
	{
		error.Format( _T("Error %d : SendFrame()"), ids );
		AfxMessageBox( error );
		return;
	}

	pDM->EndFrame();

	AfxMessageBox( _T("���� ������ ����Ǿ����ϴ�.") );
}

VOID CMNG_CODE_MANAGER_01::CopyPatternGridDataToDM()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_codePatternClass") );
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );

	INT rowCnt = pDM->GetRowCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		if( _T("V") == pGrid->GetTextMatrix( i + 1, 2 ) )
			pDM->SetCellData( _T("P_ACTIVE"), _T("Y"), i );
		else
			pDM->SetCellData( _T("P_ACTIVE"), _T("N"), i );

		if( _T("V") == pGrid->GetTextMatrix( i + 1, 3 ) )
			pDM->SetCellData( _T("P_DEFAULT_VALUE"), _T("Y"), i );
		else
			pDM->SetCellData( _T("P_DEFAULT_VALUE"), _T("N"), i );
	}
}

VOID CMNG_CODE_MANAGER_01::OnSelchangeCombo2() 
{
	// TODO: Add your control notification handler code here
	
	SearchCodePatternClass();
	ViewToolBarBtns();
}

VOID CMNG_CODE_MANAGER_01::AddOrModifyCode()
{
	//m_type -> 1 : �Է�, 2 : ����

	TCHAR permition = GetUserPermition();
	CString error;
	
	
	//===================================================
	//2009.10.16 ADD BY LKS : CD_CODE_TBL ��������
	if( (m_pInfo->MODE != 10000) && (m_pInfo->MODE != 30000) )
	{
	//===================================================
		// 2010.01.25 UPDATE BY PWR : ���հ����� �߰�
		if( 'A' != permition && 'S' != permition )
// 		if( 'A' != permition && 'M' != permition )
		{
			error.Format( _T("%s ���� �ڵ忡 ���� �Է�/���� ������ �����ϴ�."), GetUserID() );
			AfxMessageBox( error );
			return;
		}	
	}
	
	INT codeClass = GetCodeClassNum();
	INT *fixedCodeClass, fixedCodeClassCnt;
	
	fixedCodeClass = GetFixedCodeClass( fixedCodeClassCnt );

	for( INT i = 0; i < fixedCodeClassCnt; i++ )
		if( fixedCodeClass[ i ] == codeClass )
		{
			AfxMessageBox( _T("���õ� �ڵ� Ŭ������ �Է� �� ������ �Ұ��� �մϴ�.") );
			free( fixedCodeClass );
			return;
		}
	free( fixedCodeClass );
	
	INT *checkedDMRow, checkedRowCnt;
	checkedDMRow = GetCheckedRowInfo( checkedRowCnt, 1 );

	if( 2 == m_type && !checkedRowCnt )
	{
		AfxMessageBox( _T("���� �׸��� �����Ͽ� �ֽʽÿ�") );
		return;
	}	

	CMNG_CODE_MANAGER_05 dlg_5( this );
	CMNG_CODE_MANAGER_06 dlg_6( this );
	CMNG_CODE_MANAGER_07 dlg_7( this );
	CMNG_CODE_MANAGER_08 dlg_8( this );

	bool IsModifyed = false;
	switch( codeClass )
	{
		case 0 :	// �ƹ��͵� �ƴҶ�
			AfxMessageBox( _T("�ڵ� Ŭ������ �����Ͽ� �ֽʽÿ�") );
			return;

		case 2 :

			dlg_8.m_type = m_type;
			dlg_8.m_codeClass = codeClass;
			dlg_8.m_dmRow = checkedDMRow;
			dlg_8.m_rowCnt = checkedRowCnt;
			dlg_8.DoModal();
			IsModifyed = dlg_8.m_bModify;
			break;
		
		case 11 :	// ��ġ��ȣ

			dlg_6.m_type = m_type;
			dlg_6.m_codeClass = codeClass;
			dlg_6.m_dmRow = checkedDMRow;
			dlg_6.m_rowCnt = checkedRowCnt;
			dlg_6.DoModal();
			IsModifyed = dlg_6.m_bModify;
			break;
/*
		case 31 :	// ������������
			
			dlg_7.m_type = m_type;
			dlg_7.m_codeClass = codeClass;
			dlg_7.m_dmRow = checkedDMRow;
			dlg_7.m_rowCnt = checkedRowCnt;
			dlg_7.DoModal();

			break;
*/
		default :	// �׿�
			
			dlg_5.m_type = m_type;
			dlg_5.m_codeClass = codeClass;
			dlg_5.m_dmRow = checkedDMRow;
			dlg_5.m_rowCnt = checkedRowCnt;
			dlg_5.DoModal();
			IsModifyed = dlg_5.m_bModify;
			break;
	}

	free( checkedDMRow );

	if( IsModifyed )
		RefreshCodeData();
}

INT CMNG_CODE_MANAGER_01::AddCodeToPatternTbl( CString pk, CESL_DataMgr *pDM )
{
	const INT fieldCnt = 5;
	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"), _T("CODE_KEY"), _T("PATTERN_NAME"), _T("ACTIVE"), _T("DEFAULT_VALUE") };
	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") };
	
	CString data[ fieldCnt ];


	CESL_DataMgr *pPtrnDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_codePattern") );


	data[ 1 ] = pk;
	data[ 3 ] = _T("Y");
	data[ 4 ] = _T("N");	

	
	INT ids = 0;
	if( !pDM )
	{
		ids = pPtrnDM->StartFrame();
		pPtrnDM->InitDBFieldData();
	}
	
	for( INT i = 0; i < pPtrnDM->GetRowCount(); i++ )
	{
		if( pPtrnDM->m_dbkind == 1 )
			data[ 0 ] = _T("ESL_SEQ.NEXTVAL");
		else
		{
			pPtrnDM->MakeRecKey( data[0] );
		}

		pPtrnDM->GetCellData( _T("DISTINCT PATTERN_NAME"), i, data[ 2 ] );
		for( INT j = 0; j < fieldCnt; j++ )
			if( pDM )
				pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ] );
			else
				pPtrnDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ] );

		if( pDM )
		{
			pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("�ڵ����"), __WFILE__, __LINE__ ), TRUE );
			ids = pDM->MakeInsertFrame( _T("MN_CODE_PATTERN_TBL") );
		}
		else
		{
			pPtrnDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("�ڵ����"), __WFILE__, __LINE__ ), TRUE );
			ids = pPtrnDM->MakeInsertFrame( _T("MN_CODE_PATTERN_TBL") );
		}
	}

	if( !pDM )
	{
		ids = pPtrnDM->SendFrame();
		ids = pPtrnDM->EndFrame();
	}
	
	return 0;
}

INT CMNG_CODE_MANAGER_01::AddCodeToUserTbl( CString pk, CESL_DataMgr *pDM )
{
	CESL_DataMgr *pUserDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_USER_TBL") );
	pUserDM->RefreshDataManager( _T("") );
	
	const INT fieldCnt = 6;
	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"), _T("USER_KEY"), _T("CODE_KEY"), _T("PATTERN"), _T("ACTIVE"), _T("DEFAULT_VALUE") };
	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") };
	
	CString data[ fieldCnt ];


	data[ 2 ] = pk;
	data[ 4 ] = _T("Y");
	data[ 5 ] = _T("N");
	
	
	INT rowCnt = pUserDM->GetRowCount();

	INT ids = 0;

	if( !pDM )
	{
		pUserDM->StartFrame();
		pUserDM->InitDBFieldData();
	}

	for( INT i = 0; i < rowCnt; i++ )
	{

		if( pUserDM->m_dbkind == 1 )
			data[ 0 ] = _T("ESL_SEQ.NEXTVAL");
		else
			pUserDM->MakeRecKey( data[0] );

		pUserDM->GetCellData( _T("USER_KEY"), i, data[ 1 ] );
		pUserDM->GetCellData( _T("PATTERN"), i, data[ 3 ] );

		for( INT j = 0; j < fieldCnt; j++ )
			if( pDM )
				pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ] );
			else
				pUserDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ] );

		if( pDM )
		{
			pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("�ڵ����"), __WFILE__, __LINE__ ), TRUE );
			pDM->MakeInsertFrame( _T("MN_USER_CODE_TBL") );			
		}
		else
		{
			pUserDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("�ڵ����"), __WFILE__, __LINE__ ), TRUE );
			pUserDM->MakeInsertFrame( _T("MN_USER_CODE_TBL") );			
		}
	}

	if( !pDM )
	{
		ids = pUserDM->SendFrame();
		ids = pUserDM->EndFrame();
	}
	
	return 0;
}

CString CMNG_CODE_MANAGER_01::GetPK()
{
	CString pk;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_codePattern") );
	pDM->MakeRecKey( pk );

	return pk;	
}

INT CMNG_CODE_MANAGER_01::SaveCodeData( CString data[], INT idx )
{
	/*
		31�� �������������϶� _T("���޺� �̿� ��� �ڷ� ���� ����") �� ZIP_CODE �ʵ忡 �ִ´�.  
	*/

	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );

	CString strQuery;
	CString strValue;
	const INT fieldCnt = 22;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),					//	0
		_T("CLASS"),					//	1
		_T("CODE"),						//	2
		_T("DESCRIPTION"),				//	3
		_T("MAT_NAME"),					//	4
		_T("LOAN_PERIOD"),				//	5
		_T("POSTPONEMENT_PERIOD"),		//	6
		_T("LOAN_BOOK_CNT"),			//	7
		_T("PREENGAGE_BOOK_CNT"),		//	8
		_T("LOAN_YN"),					//	9
		_T("REGISTER"),					// 10
		_T("FIRST_WORK"),				// 11
		_T("LAST_WORK"),				// 12
		_T("ZIP_CODE"),					// 13
		_T("ADDRESS"),					// 14
		_T("TEL"),						// 15
		_T("FAX"),						// 16
		_T("EMAIL"),					// 17
		_T("REMARK"),					// 18
		_T("DEPART"),					// 19
		_T("CHERK"),					// 20
		_T("PRESIDENT") };				// 21

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC"), 
		_T("NUMERIC"), _T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), };

	CString fieldData[ fieldCnt ];

	switch( m_type )
	{
		case 1 :
			fieldData[ 0 ] = GetPK();
			fieldData[ 11 ] = GetWorkLogMsg( _T("�ڵ����"), __WFILE__, __LINE__ );
			break;
		case 2 :
			pDM->GetCellData( _T("C_REC_KEY"), idx, fieldData[ 0 ] );
			break;
	}
	fieldData[ 12 ] = GetWorkLogMsg( _T("�ڵ����"), __WFILE__, __LINE__ );
	
	fieldData[ 1 ].Format( _T("%d"), m_codeClass );
	
	for( INT i = 2; i < fieldCnt; i++ )
		if( 11 == i || 12 == i )
			continue;
		else
			fieldData[ i ] = data[ i - 2 ];

	/// 2003�� 12�� 13�� ������ ����
	/// �ڵ尡 ���̰� 0�� ���� 12�ڸ��� ������� ������ش�.
	fieldData[2].TrimLeft(); fieldData[2].TrimRight();
	if( fieldData[2].IsEmpty() )
		fieldData[2] = _T("            ");

	// 2009.07.15 ADD BY PWR : �л����սý��ۿ����� ���������� �Է�,����,���� �Ұ�
	if(_T("D") == m_sBookCooperativeMode)
	{
		if(1 == m_codeClass)
		{
			CString msg;
			if(1 == m_type)
			{
				msg = _T("�л����սý��ۿ����� ���������� �Է��� �Ұ��մϴ�.");
			}
			else
			{
				msg = _T("�л����սý��ۿ����� ���������� ������ �Ұ��մϴ�.");
			}
			AfxMessageBox(msg);
			return -1;
		}
	}

	// 2008.12.08 ADD BY PDJ : �Է½� �ڵ� �ߺ�����
	if ( m_type == 1 )
	{
		strQuery.Format(_T("SELECT COUNT(1) FROM CD_CODE_TBL WHERE CLASS=%s AND CODE='%s'"), fieldData[1], fieldData[2]);
		pDM->GetOneValueQuery(strQuery, strValue);
		if ( _ttoi(strValue) > 0 )
		{	
			return 100;
		}
	}

	INT ids = pDM->StartFrame();
	pDM->InitDBFieldData();

	if( m_codeClass == 2 ){
		for( i = 0; i < fieldCnt; i++ )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], fieldData[ i ] );		
	}else {
		
		for( i = 0; i < fieldCnt; i++ )
			if( fieldData[ i ].GetLength() )
				pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], fieldData[ i ] );
	}

	switch( m_type )
	{
		case 1 :
			ids = pDM->MakeInsertFrame( _T("CD_CODE_TBL") );
			break;
		case 2 :
			ids = pDM->MakeUpdateFrame( _T("CD_CODE_TBL"), _T("REC_KEY"), _T("NUMERIC"), fieldData[ 0 ], idx );
			break;	
	}
	if( ids < 0 ) return ids;

	if( 1 == m_type )
	{
		AddCodeToPatternTbl( fieldData[ 0 ], pDM );
		AddCodeToUserTbl(    fieldData[ 0 ], pDM );
	}	
	
	// 2008.12.09 ADD BY PWR : ������������(CLASS_CODE=31)�Է�, ����� ���������������̺�(CD_INDIVIDUAL_CLASS_TBL)���� �Է�,�����ϵ����Ѵ�.
	if(m_codeClass == 31)
	{
		const INT fieldCnt = 7;
		CString fieldName[ fieldCnt ] = {
			_T("REC_KEY"),				// 0
			_T("CODE"),					// 1
			_T("DESCRIPTION"),			// 2
			_T("LOAN_BOOK_CNT"),		// 3
			_T("MANAGE_CODE"),			// 4
			_T("FIRST_WORK"),			// 5
			_T("LAST_WORK") };			// 6

		CString fieldType[ fieldCnt ] = {
			_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") };

		//CString fieldData[ fieldCnt ];

		CESL_DataMgr *pManageCodeDM = FindDM(_T("DM_MNG_CODE_MANAGER_01_MANAGE_CODE"));
		pManageCodeDM->RefreshDataManager(_T(""));

		pDM->InitDBFieldData();

		if(m_type == 1)
		{ // �Է� INSERT
			fieldData[ 1 ] = data[0];
			fieldData[ 2 ] = data[1];
			fieldData[ 3 ] = _T("0");
			fieldData[ 5 ] = GetWorkLogMsg( _T("�ڵ����"), __WFILE__, __LINE__ );

			for(INT i = 0; i < pManageCodeDM->GetRowCount(); i++)
			{
				fieldData[ 4 ] = pManageCodeDM->GetCellData(_T("MANAGE_CODE"), i);
				
				for( INT j = 0; j < fieldCnt; j++ )
				{
					if(j==6) continue;
					if(0 == j)
					{
						pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], GetPK() );
					}
					else
					{
						pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], fieldData[ j ] );
					}
				}
				ids = pDM->MakeInsertFrame( _T("CD_INDIVIDUAL_CLASS_TBL") );
			}		
		}
		else if(m_type == 2)
		{ // ���� UPDATE
			CString sCode;
			fieldData[ 1 ] = data[0];
			fieldData[ 2 ] = data[1];
			fieldData[ 4 ] = pManageCodeDM->GetCellData(_T("MANAGE_CODE"), i);
			fieldData[ 6 ] = GetWorkLogMsg( _T("�ڵ����"), __WFILE__, __LINE__ );

			for( INT j = 0; j < fieldCnt; j++ )
			{
				if(j==0 || j==3 || j==4 || j==5) continue;
				pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], fieldData[ j ] );
			}
			sCode = ("'") + pDM->GetCellData(_T("C_CODE"), idx) + _T("'");
 			ids = pDM->MakeUpdateFrame(_T("CD_INDIVIDUAL_CLASS_TBL"), _T("CODE"), _T("STRING"), sCode);
		}
		else
		{ // ����
			return -1;
		}
		if( ids < 0 ) return ids;
	}

	if( pDM->SendFrame() )
		return -1;
	ids = pDM->EndFrame();

	// 2009.07.07 ADD BY PWR : ���� �������� �˸��� 
	CString sUpdateFields;
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pInfo->LIB_CODE;
	if(_T("D") == m_sBookCooperativeMode)
	{
		if(4 == m_codeClass || 19 == m_codeClass || 30 == m_codeClass || 31 == m_codeClass)
		{
			CString sClass;
			sClass.Format(_T("%d"), m_codeClass);
			
			UploadShareInfo.UploadShareLoanInfo.sCommand = _T("91");
			if(m_type == 1)
			{
				sUpdateFields = sClass+_T(",'")+data[0]+_T("','")+data[1]+_T("','")+m_pInfo->USER_ID+_T("','")+m_pInfo->MANAGE_CODE+_T("'");	
				UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
			}
			else
			{
				CString sCode = pDM->GetCellData(_T("C_CODE"), idx);
				CString sDesc = pDM->GetCellData(_T("C_DESCRIPTION"), idx);
				sUpdateFields = sClass+_T(",'")+sCode+_T("','")+sDesc+_T("','")+data[0]+_T("','")+data[1]+_T("','")+m_pInfo->USER_ID+_T("','")+m_pInfo->MANAGE_CODE+_T("'");	
				UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
			}
			UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;
			m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		}
	}
	// 2009.08.12 ADD BY PWR : ���� ������ 61�� Ŀ�ǵ�� ���Ϳ� �˸���
	if(31 == m_codeClass)
	{
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("61");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = fieldData[0];
		if(m_type == 1)
		{
			UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
			UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("");
		}
		else
		{
			UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
			UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("CODE,DESCRIPTION,POSTPONEMENT_PERIOD");
		}
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	}

	OnSelchangeCombo1();
	
	return 0;
}

VOID CMNG_CODE_MANAGER_01::DeleteCodePattern()
{
	if( !m_codePattern.GetCurSel() )
	{
		AfxMessageBox( _T("������ �ڵ� ������ �����Ͽ� �ֽʽÿ�") );
		return;
	}
	
	CString notice, codePattern;
	m_codePattern.GetWindowText( codePattern );
	notice.Format( _T("�ڵ����� : %s �� �����Ͻðڽ��ϱ�?"), codePattern );
	if( IDNO == AfxMessageBox( notice, MB_YESNO ) )
		return;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
	
	CString delQuery;
	delQuery.Format( _T("DELETE FROM MN_CODE_PATTERN_TBL WHERE PATTERN_NAME = '%s';"), codePattern );
	pDM->ExecuteQuery( delQuery, 1 );

	delQuery.Format( _T("%s �ڵ� ������ �����Ͽ����ϴ�."), codePattern );
	AfxMessageBox( delQuery );

	ViewCodePattern();
}

VOID CMNG_CODE_MANAGER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CMSHFlexGrid *pGrid_1 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	CMSHFlexGrid *pGrid_2 = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );

	if( NULL == pGrid_1->GetSafeHwnd() )
		return;

	if( NULL == pGrid_2->GetSafeHwnd() )
		return;

	if( NULL == m_tab.GetSafeHwnd() )
		return;
	
	//pGrid_1->MoveWindow( 15, 100, cx - 25, cy - 110 );
	//pGrid_2->MoveWindow( 15, 100, cx - 25, cy - 110 );
	//m_tab.MoveWindow( 5, 5, cx - 5, cy - 5 );
	m_tab.MoveWindow( 1, 2, cx - 1, cy - 2 );
	pGrid_1->MoveWindow( 10, 90, cx - 20, cy - 100 );
	pGrid_2->MoveWindow( 10, 90, cx - 20, cy - 100 );
	
	/*
	CComboBox *pCombo1 = ( CComboBox * )GetDlgItem( IDC_COMBO1 );
	CComboBox *pCombo2 = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	CComboBox *pCombo3 = ( CComboBox * )GetDlgItem( IDC_COMBO3 );
	*/
}

INT * CMNG_CODE_MANAGER_01::GetFixedCodeClass( INT &fixedCodeClassCnt )
{
	INT *fixedCodeClass, i;
	fixedCodeClassCnt = 0;
	CString fix, codeClass;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_CODECLASS") );
	for( i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("FIX_YN"), i, fix );

		if( _T("Y") == fix )
			fixedCodeClassCnt++;
	}

	fixedCodeClass = ( INT * )calloc( fixedCodeClassCnt, sizeof( INT ) );

	INT index = 0;
	for( i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("FIX_YN"), i, fix );

		if( _T("Y") == fix )
		{
			pDM->GetCellData( _T("CLASS_CODE"), i, codeClass );
			fixedCodeClass[ index++ ] = _ttoi( codeClass );
		}
	}

	return fixedCodeClass;
}

VOID CMNG_CODE_MANAGER_01::SetGridText( INT row, INT col, CString text, INT option )
{
	CMSHFlexGrid *pGrid;
	if( option )
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	else
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	pGrid->SetTextMatrix( row, col, text );
}


VOID CMNG_CODE_MANAGER_01::ConvertSelectType( INT option )
{
	const INT cnt = 2;
	CString fromAlias[ cnt ] = { _T("U_ACTIVE"), _T("U_DEFAULT_VALUE") };
	CString toAlias[   cnt ] = { _T("����"), _T("�⺻") };
	CString output[ cnt ];
	
	CESL_DataMgr *pDM;
	if( option )
	{
		pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_codePatternClass") );
		fromAlias[ 0 ] = _T("P_ACTIVE");
		fromAlias[ 1 ] = _T("P_DEFAULT_VALUE");
	}
	else
		pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
	
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( fromAlias, cnt, output, i );

		for( INT j = 0; j < cnt; j++ )
			if( _T("Y") == output[ j ] )
				output[ j ].Format( _T("V") );
			else
				output[ j ].Format( _T("") );

		pDM->SetCellData( toAlias, cnt, output, i );
	}
}

VOID CMNG_CODE_MANAGER_01::RefreshCodeData()
{
	CString pk, query;
	query.Format( _T("SELECT REC_KEY FROM MN_USER_TBL WHERE ID = '%s'"), GetUserID() );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
	pDM->GetOneValueQuery( query, pk );
	
	m_pInfo->pCodeMgr->ReadCodeData( pk );	
}

VOID CMNG_CODE_MANAGER_01::OnButton1() 
{
	// TODO: Add your control notification handler code here

	CShelfCodeGetterDlg dlg( this );
	if( IDOK == dlg.DoModal() )
		AfxMessageBox( dlg.m_code );
}

INT CMNG_CODE_MANAGER_01::GetDMRow()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
	return pDM->GetRowCount();
}

VOID CMNG_CODE_MANAGER_01::OnMouseDownmngGRID1(short Button, short Shift, long x, long y) 
{
	INT row, col;
	GetCurrentGridCell( row, col );

	// click�� �÷��� 2 �϶� ����Ʈ ���� üũ�Ѵ�. 
	if (Button == 1) {		// ���� ��ư�϶�
		switch( col )
		{
			case 2 :
				CheckActiveValue( row );
				break;
			case 3 :
				CheckDefaultValue( row );
				break;
			default :
				break;
		}	
	}
	if (Button == 2) {	// ������ ��ư�϶�
		GetCurrentGridCell(row, col, 0);
		if (col == 2) {
			FreeMenu();
			m_pMenu = new CMenu;
			if (m_pMenu == NULL) return;
			m_pMenu->CreatePopupMenu();

			m_pMenu->AppendMenu( MF_STRING , 4000 , _T("����") );
			m_pMenu->AppendMenu( MF_STRING , 4001 , _T("����") );
			m_pMenu->AppendMenu( MF_STRING , 4002 , _T("��ü����") );
			m_pMenu->AppendMenu( MF_STRING , 4003 , _T("��ü����") );
			CPoint pos;
			GetCursorPos(&pos);
			SetForegroundWindow();
			m_pMenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);
		}
	}
}


VOID CMNG_CODE_MANAGER_01::FreeMenu(VOID)
{
	if (m_pMenu)
	{
		delete m_pMenu;
		m_pMenu = NULL;
	}

	return;
}

BOOL CMNG_CODE_MANAGER_01::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	FreeMenu();
	return CDialog::DestroyWindow();
}

VOID CMNG_CODE_MANAGER_01::GetCurrentGridMouseCell( INT &row, INT &col, INT option )
{
	CMSHFlexGrid *pGrid;
	
	if( option )
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	else
		pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	row = pGrid->GetMouseRow();
	col = pGrid->GetMouseCol();

//	pGrid->SetRow(row);
//	pGrid->SetCol(col);
}

VOID CMNG_CODE_MANAGER_01::OnMSGGrid1( UINT  nIDC )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	if (pGrid == NULL) return;

	INT nRows = pGrid->GetRows();
	INT row = pGrid->GetRow();
	INT col = 2;
	// ����
	if (nIDC == 4000) {
		pGrid->SetTextMatrix(row, col, _T("V"));
	}
	// ����
	if (nIDC == 4001) {
		pGrid->SetTextMatrix(row, col, _T(""));
	}
	INT i;
	// ��ü����
	if (nIDC == 4002) {
		for (i = 1; i < nRows; i++) pGrid->SetTextMatrix(i, col, _T("V"));
	}
	// ��ü����
	if (nIDC == 4003) {
		for (i = 1; i < nRows; i++) pGrid->SetTextMatrix(i, col, _T(""));
	}

	return;
}

VOID CMNG_CODE_MANAGER_01::OnMSGGrid2( UINT  nIDC )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	if (pGrid == NULL) return;

	INT nRows = pGrid->GetRows();
	INT row = pGrid->GetRow();
	INT col = 2;
	// ����
	if (nIDC == 5000) {
		pGrid->SetTextMatrix(row, col, _T("V"));
	}
	// ����
	if (nIDC == 5001) {
		pGrid->SetTextMatrix(row, col, _T(""));
	}
	INT i;
	// ��ü����
	if (nIDC == 5002) {
		for (i = 1; i < nRows; i++) pGrid->SetTextMatrix(i, col, _T("V"));
	}
	// ��ü����
	if (nIDC == 5003) {
		for (i = 1; i < nRows; i++) pGrid->SetTextMatrix(i, col, _T(""));
	}

	return;
}


VOID CMNG_CODE_MANAGER_01::OnMouseDownmngGRID2(short Button, short Shift, long x, long y) 
{
	INT row, col;
	GetCurrentGridCell( row, col, 1 );

	// click�� �÷��� 2 �϶� ����Ʈ ���� üũ�Ѵ�. 
	if (Button == 1) {
		switch( col )
		{
			case 2 :
				CheckActiveValue( row, 1 );
				break;
			case 3 :
				CheckDefaultValue( row, 1 );
				break;
			default :
				break;
		}		
	}
	if (Button == 2) {	// ������ ��ư�϶�
		GetCurrentGridCell(row, col, 1);
		if (col == 2) {
			FreeMenu();
			m_pMenu = new CMenu;
			if (m_pMenu == NULL) return;
			m_pMenu->CreatePopupMenu();

			m_pMenu->AppendMenu( MF_STRING , 5000 , _T("����") );
			m_pMenu->AppendMenu( MF_STRING , 5001 , _T("����") );
			m_pMenu->AppendMenu( MF_STRING , 5002 , _T("��ü����") );
			m_pMenu->AppendMenu( MF_STRING , 5003 , _T("��ü����") );
			CPoint pos;
			GetCursorPos(&pos);
			SetForegroundWindow();
			m_pMenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);
		}
	}
}

INT CMNG_CODE_MANAGER_01::GetCodeClassInfo( bool &fix )
{
	CString classNum;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01_CODECLASS") );
	
	INT index = m_combo.GetCurSel() - 1;

	pDM->GetCellData( _T("CLASS_CODE"), index, classNum );
	if( _T("Y") == pDM->GetCellData( _T("FIX_YN"), index ) )
		fix = true;

	return _ttoi( classNum );
}

VOID CMNG_CODE_MANAGER_01::ViewToolBarBtns()
{
	if(m_pInfo->MODE!=10000)
	{
	const INT cnt = 7;
	CString alias[ cnt ] = { 
		_T("�ڵ��Է�"), _T("�ڵ����"), _T("�ڵ����"), _T("��������"), _T("�����߰�"), _T("���ϼ���"), _T("���ϻ���") };
	bool flag[ cnt ] = {
		true, true, true, true, true, true, true };		
	
	bool fix = false;
	INT codeClass = GetCodeClassInfo( fix );
		
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_COMBO2 );
	CString codeCls;
	pCombo->GetWindowText( codeCls );
	
	INT currentTab = m_tab.GetCurSel();
	if( currentTab )
	{
		if( _T("DEFAULT") == codeCls || _T(" - �� �� - ") == codeCls )
			flag[ 6 ] = true;
		else
			flag[ 6 ] = false;
		flag[ 4 ] = false;
		flag[ 5 ] = false;
	}
	else
	{
		if( !fix )
		{
			flag[ 0 ] = false;
			flag[ 1 ] = false;
			flag[ 2 ] = false;
		}
		flag[ 3 ] = false;
	}

	for( INT i = 0; i < cnt; i++ )
		pMain->SendMessage( USER_TOOLBAR_HIDE_SHOW, ( WPARAM )&alias[ i ], flag[ i ] );
	}
}
