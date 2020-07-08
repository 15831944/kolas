// FeeStatisticsDlg.cpp : implementation file
// Make by pwr

#include "stdafx.h"
#include "FeeStatisticsDlg.h"

#include "..\..\..\ESL\ESL\ESLDlg_ExcelExportWait.h"
#include "XLEzAutomation.h"

// SBL
#include "SBL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeStatisticsDlg dialog


CFeeStatisticsDlg::CFeeStatisticsDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFeeStatisticsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeStatisticsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFeeStatisticsDlg::~CFeeStatisticsDlg()
{
}

void CFeeStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeStatisticsDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeStatisticsDlg, CDialog)
	//{{AFX_MSG_MAP(CFeeStatisticsDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeStatisticsDlg message handlers

BOOL CFeeStatisticsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("FEE_STATISTICS") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_STATISTICS") );
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;
}

/*
BOOL CFeeStatisticsDlg::PreTranslateMessage(MSG* pMsg) 
{
	
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_F12 )
		{
			// F12 : ����ȭ������ �׸����� ������ ǥ���Ѵ�.
			CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
			pGrid->OnExcelExport();
			return TRUE;
		}
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
	
}
*/

BOOL CFeeStatisticsDlg::Create(CWnd* pParentWnd) 
{	
	return CDialog::Create(IDD, pParentWnd);
}

void CFeeStatisticsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	const int term = 0;	
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( term, term, cx - ( term * 2 ), cy - ( term * 2 ) );

}

// ** �˻� (sWhere�� �˻� ������ ���´�. ������ ?���� ?����)
VOID CFeeStatisticsDlg::SearchData( CString sWhere )
{
EFS_BEGIN

////////////////////////////////////////////////////////////////////////////////////////////////////
// 1. DM ����
////////////////////////////////////////////////////////////////////////////////////////////////////
	INT ids;
/// 1.1 ���� ���� 
/// 1.1.1 ( �˻����а� ������������ ���� ���ΰǼ��� ���αݾ��� �˻��Ѵ�. ) 
	CString sQuery1 = MakeQuery( sWhere );
/// 1.1.2 ( ROW�� �����ϱ����� ���޸��� ���� �˻��Ѵ�. )
	CString sQuery2 = MakeRowQuery( sWhere );
/// 1.1.3 ( COL�� �����ϱ����� ��¥���� ���� �˻��Ѵ�. )
	CString sQuery3 = MakeColQuery( sWhere );

	BEGIN_SBL();

/// 1.2 ������ ������ DM���� 
/// 1.2.1 ( �˻����DM : ���ΰǼ�, ���αݾ� )
	CESL_DataMgr *pBodyDM	= FindDM( _T("DM_STATISTICS_BODY") );
	if( !pBodyDM ) AfxMessageBox( _T("CFeeStatisticsDlg::SearchData( CString sWhere )") );
	ids = pBodyDM->RestructDataManager( sQuery1 );
	if( !( pBodyDM->GetRowCount() > 0 )) 
	{
		AfxMessageBox( _T("�˻������ �����ϴ�.") );
		return;
	}
/// 1.2.2 ( ROW DM : ���� )
	CESL_DataMgr *pRowDM	= FindDM( _T("DM_STATISTICS_ROW") );
	if( !pRowDM ) AfxMessageBox( _T("CFeeStatisticsDlg::SearchData( CString sWhere )") );
	ids = pRowDM->RestructDataManager( sQuery2 );

/// 1.2.3 ( COL DM : ��¥ )
	CESL_DataMgr *pColDM	= FindDM( _T("DM_STATISTICS_COL") );
	if( !pColDM ) AfxMessageBox( _T("CFeeStatisticsDlg::SearchData( CString sWhere )") );
	ids = pColDM->RestructDataManager( sQuery3 );

	END_SBL(pRowDM->GetRowCount());

/// 1.3 ������ ���޸����� �����Ѵ�.
/// 1.3.1 ( CodeDM���� �����ڵ�� ���޸��� �����´�. )
	INT nCodeCount;
	CString strCode[256], strDesc[256];
	GetCode( strCode, strDesc, nCodeCount );
/// 1.3.2 ( �˻����DM �����ڵ� ���޸����� ��ȯ )
	ids = ConvertCodeToDescInDM( pBodyDM, strCode, strDesc, nCodeCount );
/// 1.3.3 ( ROW DM �����ڵ� ���޸����� ��ȯ )
	ids = ConvertCodeToDescInDM( pRowDM, strCode, strDesc, nCodeCount );

////////////////////////////////////////////////////////////////////////////////////////////////////
// 2. �׸��� �ʱ�ȭ ( Row�� Col ũ�⼳�� )
////////////////////////////////////////////////////////////////////////////////////////////////////
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );

	InitGrid( pRowDM, pColDM, pGrid );

////////////////////////////////////////////////////////////////////////////////////////////////////
// 3. �׸��� �� �Է�
////////////////////////////////////////////////////////////////////////////////////////////////////
	INT nRow, nCol;
	nRow = pRowDM->GetRowCount();
	nCol = pColDM->GetRowCount();

	CString sClass;	//����
	CString sMoney;	//���αݾ�
	CString sCnt;	//���ΰǼ�
	CString sDate;	//��¥
	for( INT i = 0; i < pBodyDM->GetRowCount(); i++ )
	{
		pBodyDM->GetCellData( _T("����")	, i, sClass );
		pBodyDM->GetCellData( _T("���αݾ�"), i, sMoney );
		pBodyDM->GetCellData( _T("���ΰǼ�"), i, sCnt );
		pBodyDM->GetCellData( _T("��¥")	, i, sDate );

		InserToGrid( pGrid, sClass, sMoney, sCnt, sDate );
	}
///�հ躸���ֱ�
	ViewSum( pGrid );
///�׸��� ��ũ�� �� ����
	SetFixProperty();

EFS_END
}

// 2008.02.29 add by pwr
// ** DM�� �����ڵ带 ���޸����� �����Ѵ�.
// -1 : Target DM NULL
// -2 : Target DM RowCount==0
INT CFeeStatisticsDlg::ConvertCodeToDescInDM ( CESL_DataMgr *pTargetDM, CString strCode[], CString strDesc[], INT nCodeCount )
{
	INT i, j, ids;

	if (!pTargetDM) return -1;
	
	INT nRowCount = pTargetDM->GetRowCount();
	if( nRowCount == 0 ) return -2;
	
	CString strTmpCode;
	for (i = 0; i < nRowCount; i++) 
	{
		ids = pTargetDM->GetCellData( _T("����"), i, strTmpCode );
		for (j = 0; j < nCodeCount; j++) 
		{
			if (strTmpCode == strCode[j]) 
			{
				pTargetDM->SetCellData( _T("����"), strDesc[j], i);
				break;
			}
		}
	}
	return 0;
}

// 2008.02.29 add by pwr
// ** �����ڵ�, ���޸��� �����´�.
// -1 : �����ڵ�DM Reference ���� ���� �ʴ´�.
// -2 : CodeDM Restruct���� 
// -3 : �����ڵ�� ���� ���� ���� �ʴ´�. 
INT CFeeStatisticsDlg::GetCode( CString strCode[], CString strDesc[], INT &nCodeCount )
{
	INT ids;
	CString sQuery = _T("SELECT CODE, DESCRIPTION FROM CD_CODE_TBL WHERE CLASS=31");
	CESL_DataMgr *pCodeDM	= FindDM( _T("DM_STATISTICS_CLASS") );
	if( !pCodeDM ) return -1;
	ids = pCodeDM->RestructDataManager( sQuery );
	if( ids < 0 ) return -2;

	nCodeCount = pCodeDM->GetRowCount();
	if( nCodeCount == 0 ) return -3;

	for(INT i = 0; i< nCodeCount; i++)
	{
		pCodeDM->GetCellData(i, 0, strCode[i] ); //�����ڵ�
		pCodeDM->GetCellData(i, 1, strDesc[i] ); //���޸�
	}
	return 0;
}

// 2008.02.28 add by pwr
// **�׸��带 �ʱ�ȭ�Ѵ�
// (�˻��������� ���ް� ��¥�� ���ؼ� Row�� Col�� �����Ѵ�.)
INT CFeeStatisticsDlg::InitGrid(CESL_DataMgr *pRowDM, CESL_DataMgr *pColDM, CESL_Grid *pGrid)
{
	INT nRow, nCol;
	nRow = pRowDM->GetRowCount();
	nCol = pColDM->GetRowCount();

/// �׸��� �ʱ�ȭ
	pGrid->Clear();

/// DM�� 0���� �����̶� 1�� ���Ѵ�.
	pGrid->SetRows( nRow + 1 );
	pGrid->SetCols( 0, nCol + 1 );

	CString sClass;
	CString sDate;
/// X�� ROW : ����
	for(INT i = 0; i < nRow; i++)
	{
		pRowDM->GetCellData( _T("����"), i, sClass ); 
		pGrid->SetTextMatrix( i+1, 0, sClass );
	}
/// Y�� COL : ��¥ 
	for(i = 0; i < nCol; i++)
	{
		pColDM->GetCellData( _T("��¥"), i, sDate ); 
		pGrid->SetTextMatrix( 0, i+1, sDate );
	}

	return 0;
}

// 2008.02.28 add by pwr
// ** �׸��忡 DM�� �˻��� �����͸� INSERT�Ѵ�.
VOID CFeeStatisticsDlg::InserToGrid( CESL_Grid *pGrid, CString sClass, CString sMoney, CString sCnt, CString sDate )
{
	INT nX, nY;
	CString sData;
	CString sSumCnt;
	CString sSumMoney;

/// �ش� ���޿� ��¥�� �ش��ϴ� �׸����� x,y�� ���Ѵ�.
	GetLocate( nX, nY, sClass, sDate );
	
/// �ش� ���� �����ͼ� ���� ���� ��ģ��. ->   (��)\n���αݾ�
	sData = pGrid->GetTextMatrix( nX, nY );
	if( !sData.IsEmpty() )
	{
		// ���� �׸����� ���� ��ģ��.
		INT nCnt, nMoney;
		GetSeperationValue( nCnt, nMoney, sData );
		nCnt += _ttoi( sCnt );
		nMoney += _ttoi( sMoney );
		sSumCnt.Format( _T("(%d)\n"), nCnt );
		sSumMoney.Format( _T("%d"), nMoney );
		SetMoneyFormat( sSumMoney );
		sData = sSumCnt + sSumMoney;
	}
	else
	{
		// ���ΰǼ��� ���αݾ��� ��ģ��.
		SetMoneyFormat( sMoney );
		sData = _T("(") + sCnt + _T(")") + _T("\n") + sMoney;
	}

/// �ش� ��ǥ�� �� �Է�
	pGrid->SetTextMatrix( nX, nY, sData );
}

// 2008.02.28 add by pwr
// ** õ������ ǥ��
VOID CFeeStatisticsDlg::SetMoneyFormat(CString &sMoney)
{
	sMoney.Replace(_T(","), _T(""));

	CString strBuf = sMoney;
	strBuf.Replace(_T(","), _T(""));

	INT nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize-1);

	sMoney = pszFormattedNumber;

	if (pszFormattedNumber)
		delete [] pszFormattedNumber;
}

// 2008.02.28 add by pwr
// ** �����Ͱ� �� �׸����� ��ġ�� ã�´�.
VOID CFeeStatisticsDlg::GetLocate( INT &nX, INT &nY, CString sClass, CString sDate )
{
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
	INT i = 0;
	nX = nY = 0;

/// X�� ROW
	INT rows = pGrid->GetRows();
	for( i = 1; i < rows; i++ )
	{
		if( sClass == pGrid->GetTextMatrix( i, 0 ) )
		{
			nX = i;
		}
	}
/// Y�� COL
	INT cols = pGrid->GetCols( 0 );
	for( i = 1; i < cols; i++ )
	{
		if( sDate == pGrid->GetTextMatrix( 0, i ) )
		{
			nY = i;
		}
	}
}

// 2008.02.28 add by pwr
// ** �׸��� ��ũ�� �� ���� 
VOID CFeeStatisticsDlg::SetFixProperty()
{
EFS_BEGIN

	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
	if( pGrid->GetRows() > 1 )
		pGrid->SetFixedRows( 1 );
	if( pGrid->GetCols( 0 ) > 1 )
		pGrid->SetFixedCols( 1 );

/// �׸����� ���� ���Ľ�Ű��
	INT nColCnt = pGrid->GetCols(0);
	INT nRowCnt = pGrid->GetRows();
	
/// COL
	for( INT i = 0 ; i < nColCnt ; i++ )
	{
		if( i == 0 )
		{
			pGrid->SetColAlignment(i,1);
		}
		else
		{
			pGrid->SetColAlignment(i,4);
		}

		pGrid->SetRow(0);
		pGrid->SetCol(i);
		pGrid->SetCellFontBold(TRUE);
		pGrid->SetCellAlignment(4);
		pGrid->SetColWidth(i,0,1250);
	}
	
/// ROW
	for( INT j = nRowCnt-1 ; j >= 0 ; j-- )
	{
		pGrid->SetCol(0);
		pGrid->SetRow(j);
		pGrid->SetCellFontBold(TRUE);	
		pGrid->SetCellAlignment(1);
		pGrid->SetRowHeight( j , 500 );	
	}

	/*
	INT ids = ControlDisplay( _T("CM_FEE_STATISTICS"), _T("MainGrid") );	
	if( ids < 0 )
	{
		CString sErrorMsg = _T("ControlDisplay Error::CM_FEE_STATISTICS-MainGrid");
		if( ids == -1 )
			sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control�� ã�� �� ����");
		else if( ids == -1006 )
			sErrorMsg = _T("ControlDisplay Error::'CM_FEE_STATISTICS' CM�� ã�� �� ����");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );
	}
	*/

EFS_END
}

// 2008.02.28 add by pwr
// ** �հ� �����ֱ�
VOID CFeeStatisticsDlg::ViewSum( CESL_Grid *pGrid )
{
	INT nRows = pGrid->GetRows();
	INT nCols = pGrid->GetCols( 0 );
	INT nSumCnt;
	INT nSumMoney;
	CString percent;
	
	pGrid->SetCols( 0, nCols + 1 );
	pGrid->SetRows( nRows + 1 );
	pGrid->SetTextMatrix( 0, nCols, _T("�հ�") );
	pGrid->SetTextMatrix( nRows, 0, _T("�հ�") );
	
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���� �հ�
	CString sData;
	for( INT i = 1; i < nRows; i++ )
	{
		nSumCnt = 0;
		nSumMoney = 0;
		for( INT j = 1; j < nCols; j++ )
		{
			// ���� ���ΰǼ��� ���αݾ��� �и��ؼ� ���
			sData = pGrid->GetTextMatrix( i, j );
			INT nCnt, nMoney; 
			nCnt = nMoney = 0;
			if( !sData.IsEmpty() )
				GetSeperationValue( nCnt, nMoney, sData );
			nSumCnt += nCnt;
			nSumMoney += nMoney;
		}	
		// �и��� ���� ��ġ��
		CString sMoney;
		sMoney.Format( _T("%d"), nSumMoney );
		SetMoneyFormat( sMoney );
		sData.Format( _T("(%d)\n") , nSumCnt );
		sData += sMoney;
		pGrid->SetTextMatrix( i, nCols, sData );
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���� �հ� ( �����հ� ���� )
	for( i = 1; i < nCols+1; i++)
	{
		nSumCnt = 0;
		nSumMoney = 0;
		for( INT j = 1; j < nRows; j++)
		{
			// ���� ���ΰǼ��� ���αݾ��� �и��ؼ� ���
			sData = pGrid->GetTextMatrix( j, i );
			INT nCnt, nMoney;
			nCnt = nMoney = 0;
			if( !sData.IsEmpty() )
				GetSeperationValue( nCnt, nMoney, sData );
			nSumCnt += nCnt;
			nSumMoney += nMoney;
		}
		// �и��� ���� ��ġ��
		CString sMoney;
		sMoney.Format( _T("%d"), nSumMoney );
		SetMoneyFormat( sMoney );
		sData.Format( _T("(%d)\n") , nSumCnt );
		sData += sMoney;
		pGrid->SetTextMatrix( nRows, i, sData );
	}
}

// 2008.02.28 add by pwr
// ** ���ΰǼ��� ���αݾ����� ������ ����Ÿ�� �и��Ͽ� ������ ��ȯ
VOID CFeeStatisticsDlg::GetSeperationValue( INT &nCnt, INT &nMoney, CString sData )
{
	CString sClass;
	CString sMoney;
	INT nLen = sData.GetLength() - 1;

/// ���ΰǼ�
	INT nBase = 0;
	nBase = sData.Find( _T(")") );
	sClass = sData.Left( nBase );
	sClass.Replace( _T("("), _T("") );
	sClass.Replace( _T(")"), _T("") );
	nCnt = _ttoi( sClass );

/// ���αݾ�
	sMoney = sData.Right( nLen - nBase );
	sMoney.Replace( _T("\n"), _T("") );
	sMoney.Replace( _T(","), _T("") );
	nMoney = _ttoi( sMoney );
}

// 2008.02.28 add by pwr
// **�˻� ���к��� ������ �����Ѵ�.
CString CFeeStatisticsDlg::GetSearchType()
{
	CString sSearchType;
	if( m_cSearchType == 'Y' )
	{//�⺰
		sSearchType = _T("TO_CHAR( CFPI.PAYMENT_DATE , 'YYYY' ) ");
	}
	else if( m_cSearchType == 'M' )
	{//����
		sSearchType = _T("TO_CHAR( CFPI.PAYMENT_DATE , 'YYYY/MM' ) ");
	}
	else if( m_cSearchType == 'D' )
	{//�Ϻ�
		sSearchType = _T("TO_CHAR( CFPI.PAYMENT_DATE , 'YYYY/MM/DD' ) ");
	}
	return sSearchType;
}

// 2008.02.28 add by pwr
// ** ���� ����1 
// (�˻����а� ������������ ���� ���ΰǼ��� ���αݾ��� �˻��Ѵ�.)
CString CFeeStatisticsDlg::MakeQuery( CString sWhere )
{
	CString sQuery;

/// �˻����к��� ������ �޴´�.
	CString sSearchType = GetSearchType();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��ü (��ü��, ȸ������߱�, ��������������)
	if( m_cFeeType == 'T' )
	{
		if(m_strShelfLocCodeWhere.IsEmpty()) //�ڷ�� ���Ǿ���
		{
			sQuery.Format(	
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( ����������0������ )
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")						// 2008.04.21 UPDATE BY PWR ( UNION -> UNION ALL )
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")						// 2008.04.21 UPDATE BY PWR ( UNION -> UNION ALL )
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE,SUM(CFPI.MONEY_PAID),COUNT(CFPI.LOAN_INFO_KEY),%s ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE,SUM(CFPI.MONEY_PAID),COUNT(CFPI.LOAN_INFO_KEY),%s ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("AND CFPI.CLASS_CODE IN('1','2') ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
		}
		else // �ڷ�� ������ �ִٸ� �ұ��Է�(��ü��)�� �����ؼ� �˻�
		{
			sQuery.Format(	
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( ����������0������ )
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") //��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")						// 2008.04.21 UPDATE BY PWR ( UNION -> UNION ALL )
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
			_T("%s ") // �ڷ��
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")						// 2008.04.21 UPDATE BY PWR ( UNION -> UNION ALL )
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
			_T("%s ") // �ڷ��
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE,SUM(CFPI.MONEY_PAID),COUNT(CFPI.LOAN_INFO_KEY),%s ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("AND CFPI.CLASS_CODE IN('1','2') ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��ü��
	else if( m_cFeeType == 'A' )
	{
		if(m_strShelfLocCodeWhere.IsEmpty()) // �ڷ�� ���Ǿ���
		{
			sQuery.Format(
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s")
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
		}
		else // �ڷ�� ������ �ִٸ� �ұ��Է�(��ü��)�� �����ؼ� �˻�
		{
			sQuery.Format(
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // �ڷ��
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			_T("UNION ALL ")
			_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
			_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // �ڷ��
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE, %s ")
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType );
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ȸ������߱�
	else if( m_cFeeType == 'R' )
	{
		sQuery.Format(
		_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
		_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE %s")
		_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
		_T("AND CFPI.CLASS_CODE IN('1') ")
		_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
		_T("AND CFPI.RETRO_YN IS NULL ")
		_T("%s ") // ��������
		_T("GROUP BY CLU.USER_CLASS_CODE, %s")
		// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
		_T("UNION ALL ")
		_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
		_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE %s")
		_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
		_T("AND CFPI.RETRO_YN='Y' ")
		_T("AND CFPI.STATUS='Y' ")
		_T("AND CFPI.CLASS_CODE IN('1') ")
		_T("%s ") // ��������
		_T("GROUP BY CLU.USER_CLASS_CODE, %s")
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���������� ����
	else if( m_cFeeType == 'C' )
	{
		sQuery.Format(
		_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
		_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE %s")
		_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
		_T("AND CFPI.CLASS_CODE IN('2') ")
		_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
		_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.06.02 ADD BY PWR ( 0������ )
		_T("AND CFPI.RETRO_YN IS NULL ")
		_T("%s ") // ��������
		_T("GROUP BY CLU.USER_CLASS_CODE, %s")
		// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
		_T("UNION ALL ")
		_T("SELECT CLU.USER_CLASS_CODE, SUM(CFPI.MONEY_PAID), COUNT(CFPI.LOAN_INFO_KEY), %s")
		_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
		_T("WHERE %s")
		_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
		_T("AND CFPI.RETRO_YN='Y' ")
		_T("AND CFPI.STATUS='Y' ")
		_T("AND CFPI.CLASS_CODE IN('2') ")
		_T("AND CFPI.MONEY_PAID!=0 ")
		_T("%s ") // ��������
		_T("GROUP BY CLU.USER_CLASS_CODE, %s")
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
	}
		
	return sQuery;
}

// 2008.02.28 add by pwr
// ** ���� ����2
// (ROW�� �����ϱ����� ���޸��� ���� �˻��Ѵ�.)
CString CFeeStatisticsDlg::MakeRowQuery( CString sWhere )
{
	CString sQuery;
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��ü (��ü��, ȸ������߱�, ��������������)
	if( m_cFeeType == 'T' )
	{
		if(m_strShelfLocCodeWhere.IsEmpty()) //�ڷ�� ���Ǿ���
		{
			sQuery.Format(
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( ����������0������ )
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.CLASS_CODE IN('1','2') ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				, sWhere, m_strManageCodeWhere
				, sWhere, m_strManageCodeWhere
				, sWhere, m_strManageCodeWhere
				, sWhere, m_strManageCodeWhere
				, sWhere, m_strManageCodeWhere );
		}
		else // �ڷ�� ������ �ִٸ� �ұ��Է�(��ü��)�� �����ؼ� �˻�
		{
			sQuery.Format(
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( ����������0������ )
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // �ڷ��
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // �ڷ��
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.CLASS_CODE IN('1','2') ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				, sWhere, m_strManageCodeWhere
				, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere
				, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere
				, sWhere, m_strManageCodeWhere );
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��ü��
	else if( m_cFeeType == 'A' )
	{
		if(m_strShelfLocCodeWhere.IsEmpty()) //�ڷ�� ���Ǿ���
		{
			sQuery.Format(
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
			, sWhere, m_strManageCodeWhere
			, sWhere, m_strManageCodeWhere
			, sWhere, m_strManageCodeWhere );
		}
		else // �ڷ�� ������ �ִٸ� �ұ��Է�(��ü��)�� �����ؼ� �˻�
		{
			sQuery.Format(
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // �ڷ��
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
				_T("UNION ")
				_T("SELECT CLU.USER_CLASS_CODE ")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // �ڷ��
				_T("%s ") // ��������
				_T("GROUP BY CLU.USER_CLASS_CODE ")
			, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere
			, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere );
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ȸ������߱�
	else if( m_cFeeType == 'R' )
	{
		sQuery.Format(
			_T("SELECT CLU.USER_CLASS_CODE ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.CLASS_CODE IN('1') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE ")
			// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
			_T("UNION ")
			_T("SELECT CLU.USER_CLASS_CODE ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('1') ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE ")
		, sWhere, m_strManageCodeWhere
		, sWhere, m_strManageCodeWhere );
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���������� ����
	else if( m_cFeeType == 'C' )
	{
		sQuery.Format(
			_T("SELECT CLU.USER_CLASS_CODE ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.CLASS_CODE IN('2') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.06.02 ADD BY PWR ( 0������ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE ")
			// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
			_T("UNION ")
			_T("SELECT CLU.USER_CLASS_CODE ")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('2') ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("%s ") // ��������
			_T("GROUP BY CLU.USER_CLASS_CODE ")
		, sWhere, m_strManageCodeWhere
		, sWhere, m_strManageCodeWhere );
	}

	return sQuery;
}

// 2008.02.28 add by pwr
// ** ���� ����3
// (COL�� �����ϱ����� ��¥���� ���� �˻��Ѵ�.)
CString CFeeStatisticsDlg::MakeColQuery( CString sWhere )
{
	CString sQuery;

/// �˻����к��� ������ �޴´�.
	CString sSearchType = GetSearchType();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��ü (��ü��, ȸ������߱�, ��������������)
	if( m_cFeeType == 'T' )
	{
		if(m_strShelfLocCodeWhere.IsEmpty()) //�ڷ�� ���Ǿ���
		{
			sQuery.Format(
				_T("SELECT %s")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( ����������0������ )
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // ��������
				_T("GROUP BY %s")
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // ��������
				_T("GROUP BY %s")
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // ��������
				_T("GROUP BY %s")
				// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("%s ") // ��������
				_T("GROUP BY %s")
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.CLASS_CODE IN('1','2') ")
				_T("%s ") // ��������
				_T("GROUP BY %s")
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
		}
		else
		{
			sQuery.Format(
				_T("SELECT %s")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND (CFPI.CLASS_CODE IN('2') AND CFPI.MONEY_PAID!=0 OR CFPI.CLASS_CODE IN('1')) ")// 2008.06.02 ADD BY PWR ( ����������0������ )
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // ��������
				_T("GROUP BY %s")
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // �ڷ��
				_T("%s ") // ��������
				_T("GROUP BY %s")
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CL.USER_KEY=CLU.REC_KEY ")
				_T("AND CFPI.CLASS_CODE IN('0') ")
				_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
				_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
				_T("AND CFPI.RETRO_YN IS NULL ")
				_T("%s ") // �ڷ��
				_T("%s ") // ��������
				_T("GROUP BY %s")
				// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
				_T("UNION ")
				_T("SELECT %s")
				_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
				_T("WHERE %s")
				_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
				_T("AND CFPI.RETRO_YN='Y' ")
				_T("AND CFPI.STATUS='Y' ")
				_T("AND CFPI.MONEY_PAID!=0 ")
				_T("AND CFPI.CLASS_CODE IN('1','2') ")
				_T("%s ") // ��������
				_T("GROUP BY %s")
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strShelfLocCodeWhere, m_strManageCodeWhere, sSearchType
			, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��ü��
	else if( m_cFeeType == 'A' )
	{
		sQuery.Format(
			_T("SELECT %s")
			_T("FROM CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY %s")
			_T("UNION ")
			_T("SELECT %s")
			_T("FROM LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CL.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CL.USER_KEY=CLU.REC_KEY ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.05.27 ADD BY PWR ( ��ü���0������ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY %s")
			// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
			_T("UNION ")
			_T("SELECT %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('0') ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("%s ") // ��������
			_T("GROUP BY %s")
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ȸ������߱�
	else if( m_cFeeType == 'R' )
	{
		sQuery.Format(
			_T("SELECT %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.CLASS_CODE IN('1') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY %s")
			// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
			_T("UNION ")
			_T("SELECT %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('1') ")
			_T("%s ") // ��������
			_T("GROUP BY %s")
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���������� ����
	else if( m_cFeeType == 'C' )
	{
		sQuery.Format(
			_T("SELECT %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.CLASS_CODE IN('2') ")
			_T("AND CFPI.STATUS='Y' ")				// 2008.05.27 ADD BY PWR ( ���οϷ��ڷḸ )
			_T("AND CFPI.MONEY_PAID!=0 ")			// 2008.06.02 ADD BY PWR ( 0������ )
			_T("AND CFPI.RETRO_YN IS NULL ")
			_T("%s ") // ��������
			_T("GROUP BY %s")
			// 2009.03.09 ADD BY PWR : �ұ��Է� �߰�
			_T("UNION ")
			_T("SELECT %s")
			_T("FROM CO_LOAN_USER_TBL CLU, CO_FEE_PAYMENT_INFO_TBL CFPI ")
			_T("WHERE %s")
			_T("AND CLU.REC_KEY=CFPI.LOAN_INFO_KEY ")
			_T("AND CFPI.RETRO_YN='Y' ")
			_T("AND CFPI.STATUS='Y' ")
			_T("AND CFPI.CLASS_CODE IN('2') ")
			_T("AND CFPI.MONEY_PAID!=0 ")
			_T("%s ") // ��������
			_T("GROUP BY %s")
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType
		, sSearchType, sWhere, m_strManageCodeWhere, sSearchType );
	}

	return sQuery;
}

// 2008.02.29 add by pwr
// ** ���� ���Ϸ� ����
// ���� 3���� ������� �ּ� ó���� �ٷ� ��밡���ϴ�.
// ��� 1 : Excel Automation (�ӵ��� ������, ������ ��ġ�Ǿ��־���Ѵ�.)
// ��� 2 : CESL_Grid�� F12�̸� �̰͵� Automation����Ѱ��̴�. ( ���� �ʺ� �⺻�̶� �߷����δ�.)
// ��� 3 : ExcelManager (�ӵ��� ������. OCX�� ��ġ�Ǿ������� �ȴ�.)
INT CFeeStatisticsDlg::MakeGridToExcelFile()
{
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��� 1 : EXCEL AUTOMATION
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
/// ��賽 �ڷᰡ �ִ����� Ȯ���Ѵ�.
	CESL_DataMgr *pBodyDM	= FindDM( _T("DM_STATISTICS_BODY") );
	if( pBodyDM->GetRowCount() <= 0 ) 
	{
		return -1;
	}
	
	TCHAR TmpDir[1024] ;
	ZeroMemory(TmpDir, 1024);
	GetCurrentDirectory(1024,TmpDir);

/// ���� ���̾�α�
	BOOL bMode = FALSE;
	CFileDialog dlg( bMode, _T("xls"), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					_T("��������(*.xls)|*.xls;|All Files(*.*)|*.*||") );

	if ( IDOK == dlg.DoModal() ) 
	{
		// ��ο� �̸� ����
		CString sFileName = dlg.GetPathName();
		
		INT nWidth, nHeight, i, j, k, nRow, nCol;
		
		TCHAR szLineData[10000];
		CRect rect, DlgRect;
		CString strPathName, strData, strTmpData ;
		CFile file;

		// �����Ȳ ǥ�� ( ���α׷����� )		
		GetClientRect(&rect);		
		nWidth	= DlgRect.Width();
		nHeight = DlgRect.Height();
		
		DlgRect = rect;
		DlgRect.top		= (nHeight - 50) / 2;
		DlgRect.bottom	= DlgRect.top + 50;
		DlgRect.left	= (nWidth - 175) / 2;
		DlgRect.right	= DlgRect.left + 175;
		
		CESLDlg_ExcelExportWait WaitDlg(this);
		WaitDlg.Create(IDD_DLG_MSG_EXCELEXPORTWAIT, this);
		WaitDlg.MoveWindow( DlgRect );
		WaitDlg.CenterWindow(NULL);
		//WaitDlg.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		WaitDlg.ShowWindow(TRUE);	
		WaitDlg.m_Progress1.SetPos(0);
		WaitDlg.m_Progress1.SetRange(0,100);
		CString strTotalData;
		CString strTmp2;
		FLOAT	nProgressStep=0;
		FLOAT	nProgress =0;
		CStringArray strTotalExportLine;
		
		nRow = pGrid->GetRows();
		nCol = pGrid->GetCols(0);	
		nProgressStep=(float)100/nRow;
		BOOL bRemainder;	
		
		strTotalExportLine.RemoveAll();
		bRemainder = FALSE;
		for( i = 0; i < nRow; i++ )
		{
			strData.Empty();	strTmpData.Empty();	
			memset(szLineData, 0, 10000);
			for( j = 0; j < nCol; j++ )
			{			
				strData = pGrid->GetTextMatrix(i, j);
				//��¥�� ǥ�� �ϱ� ���ؼ� �⵵�� ���ڸ��� ǥ���ϰ� �տ� '�� �ٿ���.
				//#######���� ǥ�õǴ°��� �������� ��ġ
				if( !(strData.Find( _T("/") ) < 0) )
				{
					strData = _T("'") + strData.Right(8);
				}
				strTmpData = strTmpData + _T("\"") + strData + _T("\"") + _T("\t");						
			}		
			nProgress += nProgressStep;
			WaitDlg.m_Progress1.SetPos((INT)nProgress);
			
			strTmpData = strTmpData + _T("\n");
			
			_tprintf(szLineData, _T("%s"), strTmpData );
			
			INT nLen = _tcsclen(szLineData);
			for ( k=0 ; k < nLen ; k++)
			{
				if ( szLineData[k] == 13 ) 
				{
					szLineData[k] = 32;						
				}
			}		
			strTotalData += szLineData;
			if ( i%1000 == 0 ) 
			{
				strTotalExportLine.Add(strTotalData);
				strTotalData = _T("");
				bRemainder = FALSE;
			}
			else if ( i%1000 == 1)
			{
				bRemainder = TRUE;
			}		
		}
		
		if ( bRemainder )
		{
			strTotalExportLine.Add(strTotalData);
			strTotalData = _T("");
		}
		
		for ( i = 0 ; i < strTotalExportLine.GetSize() ; i++ )
		{
			strTotalData += strTotalExportLine.GetAt(i);
		}
		
		WaitDlg.m_Progress1.SetPos(100);
		
		//TRUE : ��������, FALSE : ����ȭ�� �Ⱥ����ش�.
		CXLEzAutomation XL(FALSE);
		XL.ExportCString( strTotalData );
		//XL.CreateXYChart(2);	
		//XL.Create3YChart(0,13);
		XL.SaveFileAs(sFileName);
		//XL.OpenExcelFile(sFileName);
		//Close Excel and release OLE. 
		XL.ReleaseExcel();
		WaitDlg.ShowWindow(FALSE);

		SetCurrentDirectory(TmpDir);

		CString sMsg;
		sMsg.Format( _T("'%s' ������ �����Ǿ����ϴ�.") , sFileName );
		AfxMessageBox( sMsg );
	}
	else
	{
		return -2;
	}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��� 2 : ESL_Grid�� F12�� ���� Automation��� ȭ�鿡 �ٷ� ����.
////////////////////////////////////////////////////////////////////////////////////////////////////////
	pGrid->OnExcelExport();

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ��� 3 : ExcelManager
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*	
	INT i, j;
/// ��賽 �ڷᰡ �ִ����� Ȯ���Ѵ�.
	CESL_DataMgr *pBodyDM	= FindDM( _T("DM_STATISTICS_BODY") );
	if( pBodyDM->GetRowCount() <= 0 ) 
	{
		return -1;
	}
	
	TCHAR TmpDir[1024] ;
	ZeroMemory(TmpDir, 1024);
	GetCurrentDirectory(1024,TmpDir);

/// ���� ���̾�α�
	BOOL bMode = FALSE;
	CFileDialog dlg( bMode, _T("xls"), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					_T("��������(*.xls)|*.xls;|All Files(*.*)|*.*||") );

	if ( IDOK == dlg.DoModal() ) 
	{
		// ��ο� �̸� ����
		CString sFileName = dlg.GetPathName();

	//////////////////////////////////////////////////////////////////////////////
	// 4. ���� �ʱ�ȭ
	//////////////////////////////////////////////////////////////////////////////
	CRect rc;
	GetClientRect(&rc);	
	
	C_ExcelManControl ExcelCtrl;
	ExcelCtrl.Create( _T("Excel Control"), BS_DEFPUSHBUTTON, rc, this, 8805);
	if (ExcelCtrl.m_hWnd == NULL)
	{
		MessageBox( _T("Excelmanager.ocx�� ��ġ�Ǿ� ���� �ʽ��ϴ�. ��ġ�Ͽ� �ֽʽÿ�."), _T("Warnning!"), MB_ICONEXCLAMATION );
		return -2;
	}

    CExcelManager ExcelManager(&ExcelCtrl);
    if(ExcelManager.OpenManager() == FALSE) 
	{
        MessageBox( _T("NOT Installed Excelmanager.ocx or No Excel!!!"), _T("Warnning!"), MB_ICONEXCLAMATION );
		return -3;
	}

	// �����Ȳ ǥ�� ( ���α׷����� )
	CRect rect, DlgRect;
	INT nWidth, nHeight;

	GetClientRect(&rect);		
	nWidth	= DlgRect.Width();
	nHeight = DlgRect.Height();
	
	DlgRect = rect;
	DlgRect.top		= (nHeight - 50) / 2;
	DlgRect.bottom	= DlgRect.top + 50;
	DlgRect.left	= (nWidth - 175) / 2;
	DlgRect.right	= DlgRect.left + 175;
	
	CESLDlg_ExcelExportWait WaitDlg(this);
	WaitDlg.Create(IDD_DLG_MSG_EXCELEXPORTWAIT, this);
	WaitDlg.MoveWindow( DlgRect );
	WaitDlg.CenterWindow(NULL);
	//WaitDlg.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	WaitDlg.ShowWindow(TRUE);	
	WaitDlg.m_Progress1.SetPos(0);
	WaitDlg.m_Progress1.SetRange(0,100);
	FLOAT	nProgressStep=0;
	FLOAT	nProgress =0;
	

	// 4.1) ��Ʈ�� �����Ѵ�.
	ExcelManager.Font(1, 1, 40, 1, TRUE, FALSE, RGB(0, 0, 0), 11);

	// 4.2) ���� ������ �����.
	//      Horizontal (1 - ǥ��),      (2 - ����),      (3 - ���),               (4 - ������), 
	//                 (5 - ä���),    (6 - ���� ����), (7 - ���� ������ �����), (8 - ���)
    ExcelManager.Horizontal_Alignment(1, 1, 40, 1, 3);

	// ���� ( ROW )
	CESL_DataMgr *pRowDM	= FindDM( _T("DM_STATISTICS_ROW") );
	INT nRowCnt = pRowDM->GetRowCount();
	nProgressStep=(float)100/nRowCnt;
	for( i = 0; i < nRowCnt+1; i++ )
	{
		CString sData;
		pRowDM->GetCellData( _T("����"), i, sData );
		if( i == nRowCnt )
		{
			sData = _T("�հ�");
		}
		ExcelManager.SetTextMatrix( i+2, 1, sData );
	}
	// ��¥ ( COL )
	CESL_DataMgr *pColDM	= FindDM( _T("DM_STATISTICS_COL") );
	INT nColCnt = pColDM->GetRowCount();
	for( i = 0; i < nColCnt+1; i++ )
	{
		CString sData;
		pColDM->GetCellData( _T("��¥"), i, sData );
		if( !(sData.Find( _T("/") ) < 0) )
		{
			sData = _T("'") + sData.Right(8);
		}
		if( i == nColCnt )
		{
			sData = _T("�հ�");
		}
		ExcelManager.SetTextMatrix( 1, i+2, sData );
	}

	INT nRow, nCol;
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
	nRow = pGrid->GetRows();
	nCol = pGrid->GetCols(0);
	for( i = 1; i < nRow; i++ )
	{
		for( j = 1; j < nCol; j++)
		{
			CString sData;
			sData = pGrid->GetTextMatrix( i, j );
			ExcelManager.SetTextMatrix( i+1, j+1, sData );
		}
		nProgress += nProgressStep;
		WaitDlg.m_Progress1.SetPos((INT)nProgress);
	}
	ExcelManager.SaveAs( sFileName );
	WaitDlg.m_Progress1.SetPos(100);
	// 5.3) ����� ���� ��ü�� ��ȯ�Ѵ�.
    ExcelManager.CloseManager();

	//////////////////////////////////////////////////////////////////////////////
	// 6. ������
	//////////////////////////////////////////////////////////////////////////////
	SetCurrentDirectory(TmpDir);

	CString sMsg;
	sMsg.Format( _T("'%s' ������ �����Ǿ����ϴ�.") , sFileName );
	AfxMessageBox( sMsg );
	}
	else
	{
		return -4;
	}
*/
	return 0;
}

HBRUSH CFeeStatisticsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}