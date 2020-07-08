// StatisticsPrintManager.cpp : implementation file
//

#include "stdafx.h"
#include "StatisticsPrintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsPrintManager dialog


CStatisticsPrintManager::CStatisticsPrintManager( CESL_Mgr *pParent, CMSHFlexGrid *pGrid, INT statisticsType, TCHAR colType, CString searchCondition )
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CStatisticsPrintManager)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGrid = pGrid;
	m_statisticsType = statisticsType;
	m_colType = colType;
	m_searchCondition = searchCondition;

	InitESL_Mgr( _T("StatisticsPrintManager") );
	// DM_StatisticsPrintManager_D
	// DM_StatisticsPrintManager_M
	// DM_StatisticsPrintManager_Y
	// DM_StatisticsPrintManager_C
}

CStatisticsPrintManager::~CStatisticsPrintManager()
{
	
}

BOOL CStatisticsPrintManager::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	

VOID CStatisticsPrintManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsPrintManager)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsPrintManager, CDialog)
	//{{AFX_MSG_MAP(CStatisticsPrintManager)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsPrintManager message handlers

/*
�������� ������ ������
	�Ϻ� ~ �հ�, %(��/���մ����ڼ�)
	���� ~ �հ�, %(��/���մ����ڼ�)
	�⺰ ~ �հ�, %(��/���մ����ڼ�)
�������� ������ �̿� ���
	�Ϻ� ~ �հ�
	���� ~ �հ�
	�⺰ ~ �հ�	
�������� ����/�ݳ� �̿� ���
	�Ϻ� ~ �հ�
	���� ~ �հ�
	�⺰ ~ �հ�
	�з���ȣ
		( �ѷ�, ö��, ����, ��ȸ����, ��������, �������, ����, ����, ����, ����, ������� )
*/

INT CStatisticsPrintManager::MakePrintDM()
{
	INT returnValue = 0;
	switch( m_colType )
	{
		case 'D' :
			returnValue = MakeDPrintDM();
			break;
		case 'M' :
			returnValue = MakeMPrintDM();
			break;
		case 'Y' :
			returnValue = MakeYPrintDM();
			break;
		case 'C' :
			returnValue = MakeCPrintDM();
			break;
		default :
			returnValue = -1;
			break;
	}
	
	return returnValue;
}

// �÷� Ÿ���� _T("�Ϻ�") �� DM �����
INT CStatisticsPrintManager::MakeDPrintDM()
{
	INT y, m, d;
	CString sDate = m_pGrid->GetTextMatrix( 0, 1 );
	_stscanf( sDate, _T("%d/%d/%d"), &y, &m, &d );
	
	bool percent;
	if( 1 == m_statisticsType )
		percent = true;
	else
		percent = false;

	CString colTitle[ DAY_DM_FIELD_CNT - 1 ];
	MakeDayColTitle( y, m, colTitle, percent );

	CString alias[ DAY_DM_FIELD_CNT ];
	GetDayAlias( alias );

	CESL_DataMgr *pDM = FindDM( _T("DM_StatisticsPrintManager_D") );
	CString data[ DAY_DM_FIELD_CNT ];
	
	INT rows = m_pGrid->GetRows();
	for( INT i = 1; i < rows; i++ )
	{
		GetDayRecordData( data, colTitle, i, percent );
		pDM->SetCellData( alias, DAY_DM_FIELD_CNT, data, -1 );		
	}
	
	return 0;
}

// �÷� Ÿ���� _T("����") �� DM �����
INT CStatisticsPrintManager::MakeMPrintDM()
{
	INT y, m;
	CString sDate = m_pGrid->GetTextMatrix( 0, 1 );
	_stscanf( sDate, _T("%d/%d"), &y, &m );

	bool percent;
	if( 1 == m_statisticsType )
		percent = true;
	else
		percent = false;

	CString colTitle[ MONTH_DM_FIELD_CNT - 1 ];
	MakeMonthColTitle( y, colTitle, percent );
	
	CString alias[ MONTH_DM_FIELD_CNT ];
	GetMonthDMAlias( alias );

	CESL_DataMgr *pDM = FindDM( _T("DM_StatisticsPrintManager_M") );
	CString data[ MONTH_DM_FIELD_CNT ];
	
	INT rows = m_pGrid->GetRows();
	for( INT i = 1; i < rows; i++ )
	{
		GetMonthRecordData( data, colTitle, i, percent );
		pDM->SetCellData( alias, MONTH_DM_FIELD_CNT, data, -1 );		
	}
	
	return 0;
}

// �÷� Ÿ���� _T("�⺰") �� DM �����
INT CStatisticsPrintManager::MakeYPrintDM()
{
	bool percent;
	if( 1 == m_statisticsType )
		percent = true;
	else
		percent = false;

	INT y = _ttoi( m_pGrid->GetTextMatrix( 0, 1 ) );
	m_startYear = y;
	CString colTitle[ YEAR_DM_FIELD_CNT - 1 ];
	MakeYearColTitle( colTitle, y, percent );
	
	CString alias[ YEAR_DM_FIELD_CNT ];
	GetYearDMAlias( alias );

	CESL_DataMgr *pDM = FindDM( _T("DM_StatisticsPrintManager_Y") );
	CString data[ YEAR_DM_FIELD_CNT ];	
	
	INT rows = m_pGrid->GetRows();
	for( INT i = 1; i < rows; i++ )
	{
		GetYearRecoedData( data, colTitle, i, percent );
		pDM->SetCellData( alias, MONTH_DM_FIELD_CNT, data, -1 );
	}
	
	return 0;
}

// �÷� Ÿ���� _T("�з���ȣ��") �� DM �����
INT CStatisticsPrintManager::MakeCPrintDM()
{
	CString alias[ CLASSNO_DM_FIELD_CNT ];
	GetClassNoDMAlias( alias );

	CESL_DataMgr *pDM = FindDM( _T("DM_StatisticsPrintManager_C") );
	CString data[ CLASSNO_DM_FIELD_CNT ];	

	INT rows = m_pGrid->GetRows();
	for( INT i = 1; i < rows; i++ )
	{
		GetClassNoRecordData( data, alias, i );
		pDM->SetCellData( alias, CLASSNO_DM_FIELD_CNT, data, -1 );
	}	
	
	return 0;
}

VOID CStatisticsPrintManager::GetDayAlias( CString alias[] )
{
	alias[  0 ] = _T("row");
	alias[ 32 ] = _T("�հ�");
	alias[ 33 ] = _T("p");

	for( INT i = 1; i < 32; i++ )
		alias[ i ].Format( _T("%d"), i );
}

VOID CStatisticsPrintManager::GetDayRecordData( CString data[], CString colTitle[], INT row, bool percent )
{
	data[ 0 ] = m_pGrid->GetTextMatrix( row, 0 );
	
	INT gridColIdx = 1;
	for( INT i = 0; i < 31; i++ )
		if( m_pGrid->GetTextMatrix( 0, gridColIdx ) == colTitle[ i ] )
			data[ i + 1 ] = m_pGrid->GetTextMatrix( row, gridColIdx++ );
		else
			data[ i + 1 ] = _T("-");

	INT cols = m_pGrid->GetCols( 0 );
	if( percent )
	{
		data[ 32 ] = m_pGrid->GetTextMatrix( row, cols - 2 );
		data[ 33 ] = m_pGrid->GetTextMatrix( row, cols - 1 );
	}
	else
		data[ 32 ] = m_pGrid->GetTextMatrix( row, cols - 1 );
	
}

VOID CStatisticsPrintManager::GetYearRecoedData( CString data[], CString colTitle[], INT row, bool percent )
{
	data[ 0 ] = m_pGrid->GetTextMatrix( row, 0 );
	
	INT gridColIdx = 1;
	for( INT i = 0; i < 10; i++ )
		if( m_pGrid->GetTextMatrix( 0, gridColIdx ) == colTitle[ i ] )
			data[ i + 1 ] = m_pGrid->GetTextMatrix( row, gridColIdx++ );
		else
			data[ i + 1 ] = _T("_");

	INT cols = m_pGrid->GetCols( 0 );
	if( percent )
	{
		data[ 11 ] = m_pGrid->GetTextMatrix( row, cols - 2 );
		data[ 12 ] = m_pGrid->GetTextMatrix( row, cols - 1 );
	}
	else
		data[ 11 ] = m_pGrid->GetTextMatrix( row, cols - 1 );
		
}

VOID CStatisticsPrintManager::MakeDayColTitle( INT y, INT m, CString colTitle[], bool percent )
{
	for( INT i = 1; i < 32; i++ )
		colTitle[ i - 1 ].Format( _T("%04d/%02d/%02d"), y, m, i );

	INT cols = m_pGrid->GetCols( 0 );
	if( percent )
	{
		colTitle[ 31 ] = m_pGrid->GetTextMatrix( 0, cols - 2 );
		colTitle[ 32 ] = m_pGrid->GetTextMatrix( 0, cols - 1 );
	}
	else
		colTitle[ 31 ] = m_pGrid->GetTextMatrix( 0, cols - 1 );
}

VOID CStatisticsPrintManager::MakeMonthColTitle(INT y, CString colTitle[], bool percent)
{
	for( INT i = 1; i < 13; i++ )
		colTitle[ i - 1 ].Format( _T("%04d/%02d"), y, i );

	INT cols = m_pGrid->GetCols( 0 );
	if( percent )
	{
		colTitle[ 12 ] = m_pGrid->GetTextMatrix( 0, cols - 2 );
		colTitle[ 13 ] = m_pGrid->GetTextMatrix( 0, cols - 1 );
	}
	else
		colTitle[ 12 ] = m_pGrid->GetTextMatrix( 0, cols - 1 );
}

VOID CStatisticsPrintManager::GetYearDMAlias( CString alias[] )
{
	alias[  0 ] = _T("row");
	alias[ 11 ] = _T("�հ�");
	alias[ 12 ] = _T("p");

	for( INT i = 1; i < 11; i++ )
		alias[ i ].Format( _T("%d"), i );
}

VOID CStatisticsPrintManager::GetMonthDMAlias( CString alias[] )
{
	alias[  0 ] = _T("row");
	alias[ 13 ] = _T("�հ�");
	alias[ 14 ] = _T("p");

	for( INT i = 1; i < 13; i++ )
		alias[ i ].Format( _T("%d"), i );
}

VOID CStatisticsPrintManager::GetMonthRecordData(CString data[], CString colTitle[], INT row, bool percent)
{
	data[ 0 ] = m_pGrid->GetTextMatrix( row, 0 );

	INT gridColIdx = 1;
	for( INT i = 0; i < 12; i++ )
		if( m_pGrid->GetTextMatrix( 0, gridColIdx ) == colTitle[ i ] )
			data[ i + 1 ] = m_pGrid->GetTextMatrix( row, gridColIdx++ );
		else
			data[ i + 1 ] = _T("-");

	INT cols = m_pGrid->GetCols( 0 );
	if( percent )
	{
		data[ 13 ] = m_pGrid->GetTextMatrix( row, cols - 2 );
		data[ 14 ] = m_pGrid->GetTextMatrix( row, cols - 1 );
	}
	else
		data[ 13 ] = m_pGrid->GetTextMatrix( row, cols - 1 );	
}

VOID CStatisticsPrintManager::MakeYearColTitle( CString colTitle[], INT y, bool percent )
{
	for( INT i = 0; i < 10; i++ )
		colTitle[ i ].Format( _T("%04d"), y + i );
	
	
	INT cols = m_pGrid->GetCols( 0 );
	if( percent )
	{
		colTitle[ 10 ] = m_pGrid->GetTextMatrix( 0, cols - 2 );
		colTitle[ 11 ] = m_pGrid->GetTextMatrix( 0, cols - 1 );
	}
	else
		colTitle[ 10 ] = m_pGrid->GetTextMatrix( 0, cols - 1 );
}

VOID CStatisticsPrintManager::GetClassNoDMAlias( CString alias[] )
{
	alias[  0 ] = _T("row");
	alias[  1 ] = _T("�ѷ�");
	alias[  2 ] = _T("ö��");
	alias[  3 ] = _T("����");
	alias[  4 ] = _T("��ȸ����");
	alias[  5 ] = _T("��������");
	alias[  6 ] = _T("�������");
	alias[  7 ] = _T("����");
	alias[  8 ] = _T("����");
	alias[  9 ] = _T("����");
	alias[ 10 ] = _T("����");
	alias[ 11 ] = _T("�������");
	alias[ 12 ] = _T("�հ�");
}

VOID CStatisticsPrintManager::GetClassNoRecordData( CString data[], CString alias[], INT row )
{
	INT cols = m_pGrid->GetCols( 0 );
	
	for( INT i = 1; i < 12; i++ )
	{
		data[ i ] = _T("-");
		
		for( INT j = 1; j < cols - 1; j++ )
		{			
			if( m_pGrid->GetTextMatrix( 0, j ) == alias[ i ] )
			{
				data[ i ] = m_pGrid->GetTextMatrix( row, j );
				if( data[ i ].IsEmpty() )
				{
					data[ i ] = _T("-");
				}
				continue;
			}
		}
	}
	
	data[  0 ] = m_pGrid->GetTextMatrix( row, 0 );
	data[ 12 ] = m_pGrid->GetTextMatrix( row, cols - 1 );
}

VOID CStatisticsPrintManager::PrintData()
{
	if( MakePrintDM() )
		return;
	
	CESL_DataMgr *pDM;
	switch( m_colType )
	{
		case 'D' :
			pDM = FindDM( _T("DM_StatisticsPrintManager_D") );
			break;
		case 'M' :
			pDM = FindDM( _T("DM_StatisticsPrintManager_M") );
			break;
		case 'Y' :
			pDM = FindDM( _T("DM_StatisticsPrintManager_Y") );
			break;
		case 'C' :
			pDM = FindDM( _T("DM_StatisticsPrintManager_C") );
			break;
	}

	if( !pDM->GetRowCount() )
	{
		MessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return;
	}
	
	CString title, fileName, colType; 
	GetRptInfo( title, fileName, colType );

	CSIReportManager *pSIReportManager = new CSIReportManager( this );
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if( !pSIReportManager )
	{
		AfxMessageBox( _T("Report Manager�� �������� ���Ͽ����ϴ�.") );
		return;
	}
	
	pSIReportManager->SetCONNECTION_INFO( pDM->CONNECTION_INFO );
	if( pSIReportManager->GetSIReportFileFromDB( fileName ) < 0 )
	{
		AfxMessageBox( _T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�.") );
		return;
	}

	pSIReportManager->SetDataMgr( 0, pDM , 0 );

	pSIReportManager->SetTempVariable( _T("����"),		title );
	pSIReportManager->SetTempVariable( _T("��"),		_T("������") );
	pSIReportManager->SetTempVariable( _T("��"),		colType );
	pSIReportManager->SetTempVariable( _T("�˻�����"),  m_searchCondition );

	if( 'Y' == m_colType )
		ViewYearCaption( pSIReportManager );
	
	pSIReportManager->Print();
	if( pSIReportManager )
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
}

VOID CStatisticsPrintManager::GetRptInfo( CString &title, CString &fileName, CString &colType )
{
	switch( m_colType )
	{
		case 'D' :
			if( 1 == m_statisticsType )
				fileName = _T("�������������ڵ�����-��");		// prt
			else
				fileName = _T("���մ��������-��");				// prt
			colType = _T("��");
			break;
		case 'M' :
			if( 1 == m_statisticsType )
				fileName = _T("�������������ڵ�����-��");		// prt
			else
				fileName = _T("���մ��������-��");				// prt
			colType = _T("��");
			break;
		case 'Y' :
			if( 1 == m_statisticsType )
				fileName = _T("�������������ڵ�����-��");		// prt
			else
				fileName = _T("���մ��������-��");	
			colType = _T("��");
			break;
		case 'C' :
			fileName = _T("���մ��������-�з���ȣ");
			colType = _T("�з���ȣ");
			break;
		default :
			break;;
	}
	
	switch( m_statisticsType )
	{
		case 1 :
			title.Format( _T("�������� ������ ������ (%s��)"), colType );
			break;
		case 2 :
			title.Format( _T("�������� ������ �̿� ��� (%s��)"), colType );
			break;
		case 3 :
			title.Format( _T("�������� ���� �̿� ��� (%s��)"), colType );
			break;
		case 4 :
			title.Format( _T("�������� �ݳ� �̿� ��� (%s��)"), colType );
			break;
		default :
			break;
	}
}

INT CStatisticsPrintManager::SIReportEventHandler( CSIReportEventInfo *EventInfo )
{
	CSIReportManager *pSIReportManager = ( CSIReportManager * )EventInfo->m_pSIReportManager;
	if( !pSIReportManager )
		return -1;
	else
		return 0;
}

VOID CStatisticsPrintManager::ViewYearCaption( CSIReportManager *pSIReportManager )
{
	CString title, alias;
	for( INT i = 0; i < 10; i++ )
	{
		alias.Format( _T("%d"), i + 1 );
		title.Format( _T("%04d��"), m_startYear++ );
		pSIReportManager->SetTempVariable( alias,  title );
	}
}

BOOL CStatisticsPrintManager::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



HBRUSH CStatisticsPrintManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
