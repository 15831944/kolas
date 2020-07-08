// UNION_RETURN_STATICS_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "UNION_RETURN_STATICS_MAIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_STATICS_MAIN dialog


CUNION_RETURN_STATICS_MAIN::CUNION_RETURN_STATICS_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUNION_RETURN_STATICS_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_pResultGrid = NULL;
}

CUNION_RETURN_STATICS_MAIN::~CUNION_RETURN_STATICS_MAIN()
{
	if( m_pResultGrid )
	{
		delete m_pResultGrid;
		m_pResultGrid = NULL;
	}
}

void CUNION_RETURN_STATICS_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUNION_RETURN_STATICS_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUNION_RETURN_STATICS_MAIN, CDialog)
	//{{AFX_MSG_MAP(CUNION_RETURN_STATICS_MAIN)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_STATICS_MAIN message handlers

BOOL CUNION_RETURN_STATICS_MAIN::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CUNION_RETURN_STATICS_MAIN::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{			
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CUNION_RETURN_STATICS_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( 0 > InitESL_Mgr(_T("UnionReturnStatics")) )
	{
		AfxMessageBox(_T("InitESL_Mgr ERROR !!"));
		return false;
	}
	
	if( 0 > InitLibraryinfo() )
	{
		AfxMessageBox(_T("InitLibraryinfo ERROR"));
		return false;
	}

	if( 0 > InitLibraryList() )
	{
		AfxMessageBox(_T("InitLibraryList ERROR"));
		return false;
	}
	
	if( 0 > InitStaticsGrid() )
	{
		AfxMessageBox(_T("InitStaticsGrid ERROR"));
		return false;
	}

	if( m_pResultGrid )
	{
		delete m_pResultGrid;
		m_pResultGrid = NULL;
	}

	m_pResultGrid = new CESL_Grid(NULL);
	m_pResultGrid->SubclassDlgItem(IDC_GRID_UNION_RETURN_MAIN, this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUNION_RETURN_STATICS_MAIN::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_UNION_RETURN_MAIN);
	
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	pGrid->MoveWindow(-2, -2, cx+4, cy+4);
	
}

///////////////////////////////////////////////////////////////////////
// ������ �ڵ� �Է�
///////////////////////////////////////////////////////////////////////
INT	CUNION_RETURN_STATICS_MAIN::InitLibraryinfo()
{
	INT		  ids;
	const int nFieldCnt = 1;
	CString	  strField[ nFieldCnt ] = { _T("LIB_CODE") };
	CString   strLibCode[ nFieldCnt ];

	ids = CLocCommonAPI::GetLibInfo( strField, strLibCode, nFieldCnt, this );
	if( 0 > ids ) return ids;

	m_strLibraryCode = strLibCode[0];

	return 0;
}

///////////////////////////////////////////////////////////////////////
// ������ ������ ����
///////////////////////////////////////////////////////////////////////
INT	CUNION_RETURN_STATICS_MAIN::InitLibraryList()
{
	INT		ids;
	// ���մ�������������� ���������� ��������	
	CRS_SQL		  RemoteDBMgr(this);
	CESL_DataMgr* pdmLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));

	ids = RemoteDBMgr.SetFetchCount(0, 100);
	if( 0 > ids ) return ids;
	ids = RemoteDBMgr.LOAN_Select(pdmLibrary, _T(""), _T("DM_UNION_RETURN_LIBINFO"), 1, 1);
	if( 0 > ids ) return ids;
	
	return 0;
}

///////////////////////////////////////////////////////////////////////
// ��� ��� �׸��带 �ʱ�ȭ �Ѵ�.
// * InitLibraryList()�� ����Ǿ�� �Ѵ�.
// 1. �׸��带 �ʱ�ȭ �Ѵ�.
// 2. ������������ �ο� ������ �����Ѵ�.
///////////////////////////////////////////////////////////////////////
INT CUNION_RETURN_STATICS_MAIN::InitStaticsGrid()
{
	// 1. �׸��带 �ʱ�ȭ �Ѵ�.
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_UNION_RETURN_MAIN);
	pGrid->SetRows(2);
	pGrid->SetCols(0, 2);

	pGrid->SetTextMatrix(0, 1, _T(""));
	pGrid->SetTextMatrix(1, 0, _T(""));
	pGrid->SetTextMatrix(1, 1, _T(""));

	// 2. ������������ �ο� ������ �����Ѵ�.
	CESL_DataMgr* pdmLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
	INT           nRowCount  = pdmLibrary->GetRowCount();

	if( 0 < nRowCount )
	{		
		pGrid->SetRows(nRowCount+2);
		CString strLibraryName;

		for(INT i = 0; i < nRowCount; i++ )
		{
			pdmLibrary->GetCellData(_T("��������"), i, strLibraryName);			

			pGrid->SetTextMatrix(i+1, 0, strLibraryName);
		}

		pGrid->SetTextMatrix(nRowCount+1, 0, _T("�հ�"));
	}

	SetGridProperty();
	
	return 0;
}

///////////////////////////////////////////////////////////////////////
// ����� �÷��� �ʱ�ȭ �Ѵ�.
// 1. ��¥�� ����Ͽ� �÷� ������ ���Ѵ�.
// 2. �׸��� �ݿ�
///////////////////////////////////////////////////////////////////////
INT CUNION_RETURN_STATICS_MAIN::InitColumn(CString strStartDate, CString strEndDate)
{
	// 1. ��¥�� ����Ͽ� �÷� ������ ���Ѵ�.
/*	CTime tStartDate( atoi(strStartDate.Mid(0,4)), atoi(strStartDate.Mid(5,2)), atoi(strStartDate.Mid(8,2)), 0, 0, 0);
	CTime tEndDate( atoi(strEndDate.Mid(0,4)), atoi(strEndDate.Mid(5,2)), atoi(strEndDate.Mid(8,2)), 0, 0, 0);
	

	while( tStartDate < tEndDate )
	{
		m_arColumnDate.Add(tStartDate.Format("%Y/%m/%d"));

		tStartDate += CTimeSpan(1,0,0,0);
	}
*/
	/*
	// 2. �׸��� �ݿ�
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_UNION_RETURN_MAIN);
	INT		nColCount = m_arColumnDate.GetSize();
	
	pGrid->SetCols(0, nColCount+1);

	for( INT i = 0; i < nColCount; i++ )
	{
		pGrid->SetTextMatrix(0, i + 1, m_arColumnDate.GetAt(i));
	}

	SetGridProperty();
	*/

	return 0;
}

///////////////////////////////////////////////////////////////////////
// �׸����� ���� �Ӽ��� �ش�.
// * ���� �Ӽ�
// * 
///////////////////////////////////////////////////////////////////////
INT CUNION_RETURN_STATICS_MAIN::SetGridProperty()
{

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_UNION_RETURN_MAIN);
	if( pGrid->GetRows() > 1 )
		pGrid->SetFixedRows( 1 );
	if( pGrid->GetCols( 0 ) > 1 )
		pGrid->SetFixedCols( 1 );

	int nColCnt = pGrid->GetCols(0);
	int nRowCnt = pGrid->GetRows();
	
	for( int i = 0 ; i < nColCnt ; i++ )
	{
		pGrid->SetColAlignment(i,4);
		
		pGrid->SetRow(0);
		pGrid->SetCol(i);
		pGrid->SetCellAlignment(4);
		pGrid->SetCellFontBold(TRUE);		
		pGrid->SetColWidth(i,0,1500);
	}
	
	for( int j = nRowCnt-1 ; j >= 0 ; j-- )
	{
		pGrid->SetCol(0);
		pGrid->SetRow(j);
		pGrid->SetCellAlignment(4);
		pGrid->SetCellFontBold(TRUE);	
		pGrid->SetRowHeight( j , 400 );		
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////
// DM�� ������ �Է�
// 1. �Է��� �ο� �ε����� ã�´�.
// 2. DM�� �Է� 
///////////////////////////////////////////////////////////////////////
INT	CUNION_RETURN_STATICS_MAIN::InsertData(CESL_DataMgr* pDM, CString strLibCode, CString strDate, CString strCount)
{
	INT		nRowIdx;	

	// 1. �Է��� �ο� �ε����� ã�´�.
	CESL_DataMgr* pdmLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
	INT           nRowCount  = pdmLibrary->GetRowCount();

	CString strLibraryCode;

	for(INT i = 0; i < nRowCount; i++ )
	{
		pdmLibrary->GetCellData(_T("�������ڵ�"), i, strLibraryCode);

		strLibraryCode.TrimLeft();
		strLibraryCode.TrimRight();

		if( strLibraryCode == strLibCode )
		{
			nRowIdx = i;
			break;
		}
	}

	// 2. DM�� �Է�
	pDM->SetCellData( strDate, strCount, nRowIdx );

	return 0;
}

///////////////////////////////////////////////////////////////////////
// Ÿ���ݳ� ���
// 1. ���� �Է�ȭ�� �˾�
// 2. ���� ����
// 3. ���
// 4. ����� DM���� �ű��.
// 5. �հ� ���
// 6. ��� ���
///////////////////////////////////////////////////////////////////////
INT CUNION_RETURN_STATICS_MAIN::UnionReturnStatics()
{
	INT		ids;

	// 1. ���� �Է�ȭ�� �˾�
	CUNION_RETURN_STATICS_RUN	dlg;
	if( IDOK != dlg.DoModal() )
	{
		return 0;
	}

	InitStaticsGrid();

	// 2. ���� ����
	CString strQuery;
	ids = MakeStaticsQuery( dlg.GetStaticsMode(), 
		                    dlg.GetReturnDateStart(), 
							dlg.GetReturnDateEnd(),
							strQuery );
	if( 0 > ids ) return ids;

	// 3. ���
	DB_DATA_ARRAY_TYPE* RESULT = NULL;

	INT		nRow = 0;
	INT     nCol = 0;
	
	ids = RS_SelectProc( strQuery.GetBuffer( 0 ), &nRow, &nCol, &RESULT, LOAN, false );
	if( 0 > ids )
	{
		AfxMessageBox(_T("RS_SelectProc ERROR"));
		return ids;
	}

	if( 0 == nRow )
	{
		AfxMessageBox(_T("�˻������ �����ϴ�."));
		delete RESULT;		
		return 0;
	}	

	// 4. ����� DM���� �ű��.
	//    ���� �׸��� ��� ���¸� ���ϱ� ���� ����������
	//    DM���� ���� ó���� ���Ŀ� ����Ѵ�.
	CESL_DataMgr dm;
	MakeResultDM(&dm, dlg.GetReturnDateStart(), dlg.GetReturnDateEnd());
	
	DB_DATA_ARRAY_TYPE_SEARCH *DATA = ( DB_DATA_ARRAY_TYPE_SEARCH * )RESULT;

	for( INT i = 0; i < nRow; i++ )
	{
		InsertData(&dm, CString(DATA[ 0 ].db_data[ i ].stringData),
						CString(DATA[ 1 ].db_data[ i ].stringData),
						CString(DATA[ 2 ].db_data[ i ].stringData) );
	}

	// 5. �հ� ���
	INT		nRowCount = dm.GetRowCount();
	INT		nColCount = dm.GetColCount();
	//    ���ο� �հ�
	for( i = 0;i < nRowCount;i++ )
	{
		CString strData;
		INT		nTotal = 0;

		for( INT j = 0; j < nColCount; j++ )
		{
			dm.GetCellData(i, j, strData);
			strData.TrimLeft();
			strData.TrimRight();

			if( strData.IsEmpty() )
			{
				strData = _T("0");
				dm.SetCellData(i, j, strData);
			}

			nTotal += _ttoi(strData);
		}

		strData.Format(_T("%d"), nTotal);
		dm.SetCellData(i, nColCount-1, strData);
	}
	
	//    ���ο� �հ�
	for( i = 0;i < nColCount;i++ )
	{
		CString strData;
		INT		nTotal = 0;

		for( INT j = 0; j < nRowCount; j++ )
		{
			dm.GetCellData(j, i, strData);
			strData.TrimLeft();
			strData.TrimRight();

			if( strData.IsEmpty() )
			{
				strData = _T("0");
				dm.SetCellData(i, j, strData);
			}

			nTotal += _ttoi(strData);
		}

		strData.Format(_T("%d"), nTotal);
		dm.SetCellData(nRowCount-1, i, strData);
	}
	
	// 6. ��� ���	
	//    ���ο� �հ谡 0 �̶�� �Է����� �ʴ´�.
	//    �Էµ� ��¥�� ���Ѵ�.
	CStringArray arRealColumnDate;
	CString      strTotal;

	for( i = 0;i < nColCount;i++ )
	{
		dm.GetCellData(nRowCount-1, i, strTotal);

		if( 0 < _ttoi(strTotal) )
		{
			CString strFieldAlias, strFieldName, strFieldType, strInitValue, strDataType;
			dm.GetColumnRefInfo(i, strFieldAlias, strFieldName, strFieldType, strInitValue, strDataType);
			arRealColumnDate.Add( strFieldAlias );
		}
	}

	//    �÷��� �����Ѵ�
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_UNION_RETURN_MAIN);
	nColCount = arRealColumnDate.GetSize();
	
	pGrid->SetCols(0, nColCount+1);

	for( i = 0; i < nColCount; i++ )
	{
		pGrid->SetTextMatrix(0, i + 1, arRealColumnDate.GetAt(i));
	}

	SetGridProperty();

	//    �����͸� �Է��Ѵ�.
	CString strResult;

	for( i = 0;i < nRowCount;i++ )
	{
		for( INT j = 0; j < nColCount; j++ )
		{
			dm.GetCellData(arRealColumnDate.GetAt(j), i, strResult);
			pGrid->SetTextMatrix(i + 1, j + 1, strResult);
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////
// ��� DM�� �����Ѵ�.
// 1. �÷��� ������ ���Ѵ�.
// 2. �÷� ALIAS�� �Է��Ѵ�.
// 3. �ο� ���� �����ȴ�.
///////////////////////////////////////////////////////////////////////
INT	CUNION_RETURN_STATICS_MAIN::MakeResultDM(CESL_DataMgr* pDM, 
											 CString       strReturnDateStart, 
											 CString       strReturnDateEnd)
{	
	pDM->FreeData();
	pDM->FreeDMRef();

	// 1. �÷��� ������ ���Ѵ�.
	CTime tStartDate( _ttoi(strReturnDateStart.Mid(0,4)), _ttoi(strReturnDateStart.Mid(5,2)), _ttoi(strReturnDateStart.Mid(8,2)), 0, 0, 0);
	CTime tEndDate( _ttoi(strReturnDateEnd.Mid(0,4)), _ttoi(strReturnDateEnd.Mid(5,2)), _ttoi(strReturnDateEnd.Mid(8,2)), 0, 0, 0);
	CStringArray arColumnDate;

	while( tStartDate <= tEndDate )
	{
		arColumnDate.Add(tStartDate.Format("%Y/%m/%d"));

		tStartDate += CTimeSpan(1,0,0,0);
	}

	// 2. �÷� ALIAS�� �Է��Ѵ�.
	INT		nColumnDateCount = arColumnDate.GetSize();
	pDM->InitDataMgrRef(nColumnDateCount+1);
	pDM->m_nCols = nColumnDateCount+1;

	for( INT i = 0; i<nColumnDateCount; i++ )
	{
		pDM->SetDataMgrRef(i, arColumnDate.GetAt(i), "", "", "", "");
	}
	
	// �հ迭 �߰�
	pDM->SetDataMgrRef(nColumnDateCount, "�հ�", "", "", "", "");

	// 3. �ο� ���� �����ȴ�.
	CESL_DataMgr* pdmLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
	INT           nRowCount  = pdmLibrary->GetRowCount();

	for( i = 0; i < nRowCount; i++ )
	{
		pDM->InsertRow(-1);
	}

	// �հ�ο� �߰�
	pDM->InsertRow(-1);

	return 0;
}

///////////////////////////////////////////////////////////////////////
// ���� ����
// 1. ��� ��� �ʵ�, ������ �ʵ带 ���Ѵ�.
// 2. ��¥���� ����
// 3. ������ ��ȣ ���� ����
// 4. ���� ����
///////////////////////////////////////////////////////////////////////
INT	CUNION_RETURN_STATICS_MAIN::MakeStaticsQuery(INT     nStaticsMode, 
												 CString strReturnDateStart, 
												 CString strReturnDateEnd,
												 CString &strMakeQuery)
{
	// 1. ��� ��� �ʵ�, ������ �ʵ带 ���Ѵ�.
	CString strStaticsField;
	CString strWhereField;

	if( 0 == nStaticsMode )
	{
		/// Ÿ���ݳ����
		strStaticsField = _T("REQUEST_LIB_CODE");
		strWhereField   = _T("UPDATE_LIB_CODE");
	}
	else if( 1 == nStaticsMode )
	{
		/// Ÿ���ݳ��������
		strStaticsField = _T("UPDATE_LIB_CODE");
		strWhereField   = _T("REQUEST_LIB_CODE");
	}

	// 2. ��¥���� ����
	CString strStartDate;
	CString strEndDate;

	strStartDate.Format(_T("TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')"), strReturnDateStart);
	strEndDate.Format(_T("TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')"), strReturnDateEnd);

	// 3. ������ ��ȣ ���� ����
	// 4. ���� ����
	/*
	SELECT REQUEST_LIB_CODE, REQUEST_DATE, COUNT(1) 
 FROM CT_TEMP_RETURN_INFO_TBL
WHERE 1=1 
GROUP BY REQUEST_LIB_CODE, REQUEST_DATE;

SELECT UPDATE_LIB_CODE, REQUEST_DATE, COUNT(1) 
 FROM CT_TEMP_RETURN_INFO_TBL
WHERE 1=1 
GROUP BY UPDATE_LIB_CODE, REQUEST_DATE;

  nStaticsMode = 0; 1
	*/
	// 2009.02.23 UPDATE BY KSJ : ���� �߰�- Ÿ�� �ݳ� ��û(STATUS=0)�� �Ϸ�(STATUS=1)�� ��迡 ����
	strMakeQuery.Format(_T("SELECT %s, TO_CHAR(REQUEST_DATE,'YYYY/MM/DD'), COUNT(1) ")
						_T("FROM CT_TEMP_RETURN_INFO_TBL ")
						_T("WHERE %s='%s' AND STATUS IN ('0','1') AND REQUEST_DATE BETWEEN %s AND %s ")
						_T("GROUP BY %s, TO_CHAR(REQUEST_DATE,'YYYY/MM/DD')"), 
						strStaticsField, strWhereField, m_strLibraryCode, 
						strStartDate, strEndDate, strStaticsField);
	
	return 0;
}

