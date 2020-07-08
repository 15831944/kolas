// UNION_RETURN_HIS_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "UNION_RETURN_HIS_MAIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_HIS_MAIN dialog


CUNION_RETURN_HIS_MAIN::CUNION_RETURN_HIS_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pResultGrid		=	NULL;
	m_strSearchLibCode	=	_T("");
	m_nHistoryMode		=	0;	
}

CUNION_RETURN_HIS_MAIN::~CUNION_RETURN_HIS_MAIN()
{
	if( NULL != m_pResultGrid )
	{
		delete	m_pResultGrid;
		m_pResultGrid	=	NULL;
	}
}

void CUNION_RETURN_HIS_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUNION_RETURN_HIS_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CUNION_RETURN_HIS_MAIN::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}


BEGIN_MESSAGE_MAP(CUNION_RETURN_HIS_MAIN, CDialog)
	//{{AFX_MSG_MAP(CUNION_RETURN_HIS_MAIN)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUNION_RETURN_HIS_MAIN message handlers

BOOL CUNION_RETURN_HIS_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( 0 > InitESL_Mgr(_T("UNION_RETURN_HISTORY_MAIN")) )
	{
		AfxMessageBox(_T("InitESL_Mgr ERROR !!"));
		return FALSE;
	}

	INT ids;

	// ���Ϳ��� ������ List ��������
	ids = InitLibList();
	if( 0 > ids )
	{
		return FALSE;
	}
	
	// Grid Control �ʱ�ȭ
	ids = InitHistoryGrid();
	if( 0 > ids )
	{
		AfxMessageBox(_T("InitHistoryGrid ERROR"));
		return FALSE;
	}

	if( m_pResultGrid )
	{
		delete m_pResultGrid;
		m_pResultGrid = NULL;
	}
	
	//m_pResultGrid = new CESL_Grid(NULL);
	//m_pResultGrid->SubclassDlgItem(IDC_gridUNION_RETURN_HIS_MAIN, this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUNION_RETURN_HIS_MAIN::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid* pGridRequest	=	(CMSHFlexGrid*)GetDlgItem(IDC_gridUNION_RETURN_HIS_MAIN_REQUEST);
	if( NULL == pGridRequest->GetSafeHwnd() ) 
	{
		return;
	}

	CMSHFlexGrid* pGridUpdate	=	(CMSHFlexGrid*)GetDlgItem(IDC_gridUNION_RETURN_HIS_MAIN_UPDATE);
	if( NULL == pGridUpdate->GetSafeHwnd() ) 
	{
		return;
	}

	m_nCX	=	cx + 4;
	m_nCY	=	cy + 4;

	pGridRequest->MoveWindow( -2 , -2 , m_nCX , m_nCY );
	pGridUpdate->MoveWindow( -3 , -3 , 0 , 0 );
}

BOOL CUNION_RETURN_HIS_MAIN::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{			
			return TRUE;
		}
	}

	return CESL_Mgr::PreTranslateMessage(pMsg);
}

/*
@Create
 - 2009.02.24 BY KSJ
@Description
 - Grid Control �ʱ�ȭ
@Parameter
 - 
@Return Value
 - 0 : ����
@Mini Spec
 1. Grid�� Row�� Column �ʱ� Set
 2. �̷� ������ ����, Grid SHOW/HIDE
*/
INT CUNION_RETURN_HIS_MAIN::InitHistoryGrid()
{
EFS_BEGIN

	// Ÿ���ݳ���� Grid
	CMSHFlexGrid* pGridRequest	=	(CMSHFlexGrid*)GetDlgItem(IDC_gridUNION_RETURN_HIS_MAIN_REQUEST);
	if( NULL == pGridRequest->GetSafeHwnd() ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitHistoryGrid") );
	}
	// Ÿ���ݳ������� Grid
	CMSHFlexGrid* pGridUpdate	=	(CMSHFlexGrid*)GetDlgItem(IDC_gridUNION_RETURN_HIS_MAIN_UPDATE);
	if( NULL == pGridUpdate->GetSafeHwnd() ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitHistoryGrid") );
	}

	// 1. Grid�� Row�� Column �ʱ� Set
	pGridRequest = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_Ÿ���ݳ�") );
	pGridRequest->SetRows(2);
	pGridRequest->SetCols(0,11);
	
	pGridUpdate = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_Ÿ���ݳ�����") );
	pGridUpdate->SetRows(2);
	pGridUpdate->SetCols(0,11);

	// 2. �̷� ������ ����, Grid SHOW/HIDE
	if( 0 == m_nHistoryMode )
	{
		pGridRequest->MoveWindow( -2 , -2 , m_nCX , m_nCY );
		pGridUpdate->MoveWindow( -3 , -3 , 0 , 0 );
	}
	else if( 1 == m_nHistoryMode )
	{
		pGridUpdate->MoveWindow( -2 , -2 , m_nCX , m_nCY );
		pGridRequest->MoveWindow( -3 , -3 , 0 , 0 );
	}

	CESL_ControlMgr* pCM = FindCM(_T("CM_UNION_RETURN_HISTORY_MAIN"));
	pCM->AllControlDisplay();

	return 0;

EFS_END
	return -1;
}

/*
@Create
 - 2009.02.24 BY KSJ
@Description
 - �̷� ��ȸ
@Parameter
 - 
@Return Value
 - 0 : ����
@Mini Spec
 1. ���� �Է�ȭ�� �˾�
 2. �̷� ��� ���� - 0 : Ÿ�� �ݳ� ��� 1 : Ÿ�� �ݳ� ���� ���
 3. �̷� ��ȸ ������ ����
 4. �̷� ��ȸ ���� ��¥
 5. Grid �ʱ�ȭ
 6. ���� ����
 7. �̷� ��ȸ
 8. DM�� ����� �Է�
 9. Grid�� ��� ���
*/
INT CUNION_RETURN_HIS_MAIN::UnionReturnHistory()
{
EFS_BEGIN

	INT		ids;

	// 1. ���� �Է�ȭ�� �˾�
	CUNION_RETURN_HIS_RUN	dlg(this);
	if( IDOK != dlg.DoModal() )
	{
		return 0;
	}

	// 2. �̷� ��� ���� - 0 : Ÿ�� �ݳ� ��� 1 : Ÿ�� �ݳ� ���� ���
	m_nHistoryMode			=	dlg.GetHistoryMode();

	// 3. �̷� ��ȸ ������ ����
	m_strSearchLibCode		=	dlg.GetLibCode();
	m_strSearchLibName		=	dlg.GetLibName();

	// 4. �̷� ��ȸ ���� ��¥
	m_strReturnDateStart	=	dlg.GetReturnDateStart();
	m_strReturnDateEnd		=	dlg.GetReturnDateEnd();

	// 5. Grid �ʱ�ȭ
	InitHistoryGrid();

	// 6. ���� ����
	CString strQuery;
	ids = MakeHistoryQuery( strQuery );
	if( 0 > ids ) 
	{
		return ids;
	}

	// 7. �̷� ��ȸ
	DB_DATA_ARRAY_TYPE* RESULT = NULL;
	
	INT		nRowCnt = 0;
	INT     nColCnt = 0;

	ids = RS_SelectProc( strQuery.GetBuffer( 0 ), &nRowCnt, &nColCnt, &RESULT, LOAN, false );
	if( 0 > ids )
	{
		AfxMessageBox( _T("RS_SelectProc ERROR") , MB_ICONINFORMATION );
		return ids;
	}

	if( 0 == nRowCnt )
	{
		AfxMessageBox( _T("�˻������ �����ϴ�.") , MB_ICONINFORMATION );
		delete RESULT;		
		return 0;
	}

	// 8. DM�� ����� �Է�
	DB_DATA_ARRAY_TYPE_SEARCH *DATA = ( DB_DATA_ARRAY_TYPE_SEARCH * )RESULT;

	CESL_DataMgr* pHistoryDM = FindDM(_T("DM_UNION_RETURN_HISTORY"));
	if( NULL == pHistoryDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("UnionReturnHistory") );
	}

	CESL_DataMgr* pLibListDM = FindDM(_T("DM_UNION_RETURN_LIBINFO"));
	if( NULL == pLibListDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("UnionReturnHistory") );
	}
	INT		nLibListCnt		=	pLibListDM->GetRowCount();

	const INT nCnt = 9;
	CString sDMFieldAlias[nCnt] =
	{
		_T("�������̸�"),	_T("�����ڹ�ȣ"),	_T("��Ϲ�ȣ"), 
		_T("����"),			_T("������"),		_T("�ݳ���"), 
		_T("�ݳ�������"),	_T("�������"),		_T("������")
	};

	pHistoryDM ->FreeData();

	INT i,j;
	CString strData;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		DWORD* row = NULL;
		pHistoryDM ->InsertRow(-1);
		row = pHistoryDM ->MakeRow(nColCnt);
		pHistoryDM ->InitRow(row);

		for( j=0 ; j<nColCnt ; j++ )
		{
			strData = CString(DATA[ j ].db_data[ i ].stringData);
			
			ids = pHistoryDM ->SetCellData( i , j , strData );
			if( 0 > ids )
			{
				return ids;
			}

			if( 7 == j )
			{
				if( _T("0") == strData )
				{
					strData	= _T("����");
				}
				else if( _T("1") == strData )
				{
					strData	= _T("�ݳ�");
				}
				else if( _T("2") == strData )
				{		
					strData = _T("�ݳ�����");
				}
				else if( _T("3") == strData )
				{
					strData = _T("����");
				}
				else if( _T("4") == strData )
				{
					strData = _T("�������");
				}
				else if( _T("5") == strData )
				{
					strData	= _T("���ιݳ���");
				}
				else if( _T("6") == strData )
				{
					strData	= _T("�а��ݳ���");
				}
				else if( _T("7") == strData )
				{
					strData	= _T("Ÿ���ݳ���");
				}
				else
				{
					strData	= _T("�ݳ�");
				}

				ids = pHistoryDM->SetCellData( _T("������¼���") , strData , i );
				if( 0 > ids )
				{
					return ids;
				}
			}

			if( 8 == j )
			{
				CString		strLibCode;
				INT			k;
				for( k=0 ; k<nLibListCnt ; k++ )
				{
					pLibListDM->GetCellData( _T("�������ڵ�") , k , strLibCode );
					if( 0 > ids )
					{
						return ids;
					}

					if( 0 == strData.Compare(strLibCode) )
					{	
						pLibListDM->GetCellData( _T("��������") , k , strData );
						if( 0 > ids )
						{
							return ids;
						}

						ids = pHistoryDM->SetCellData( _T("����������") , strData , i );
						if( 0 > ids )
						{
							return ids;
						}
					}
				}
			}
		}
	}

	// 9. Grid�� ��� ���
	CESL_Grid* pGrid;
	
	if( 0 == m_nHistoryMode )
	{
		pGrid = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_Ÿ���ݳ�") );
		pGrid->Display();
	}
	else if( 1 == m_nHistoryMode )
	{
		pGrid = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_Ÿ���ݳ�����") );
		pGrid->Display();
	}

	return 0;

EFS_END
	return -1;
}

/*
@Create
 - 2009.02.24 BY KSJ
@Description
 - �̷� ��ȸ Query �����
@Parameter
 - strMakeHistoryQuery[OUT] : �˻� ����
@Return Value
 - 0 : ����
@Mini Spec
 1. �ڰ� ��������ȣ ���ϱ�
 2. �̷� ��� ������, ������ ������ �����ϱ�
 3. ��¥���� ����
 4. �̷� ��� ������ ����
*/
INT CUNION_RETURN_HIS_MAIN::MakeHistoryQuery( CString &strMakeHistoryQuery )
{
	INT		ids;

	// 1.�ڰ� ��������ȣ ���ϱ�
	CString		strMyLibCode;
	ids = GetMyLibCode(strMyLibCode);
	if( 0 > ids )
	{
		return ids;
	}

	// 2. �̷� ��� ������, ������ ������ �����ϱ�
	CString strHistoryField;
	CString strWhereField;

	if( 0 == m_nHistoryMode )
	{/// Ÿ�� �ݳ� �̷�
		strHistoryField =	_T("REQUEST_LIB_CODE");
		strWhereField   =	_T("UPDATE_LIB_CODE");
	}
	else if( 1 == m_nHistoryMode )
	{/// Ÿ�� �ݳ� ���� �̷�
		strHistoryField =	_T("UPDATE_LIB_CODE");
		strWhereField   =	_T("REQUEST_LIB_CODE");
	}
	
	// 3. ��¥���� ����
	CString strStartDate;
	CString strEndDate;

	strStartDate.Format(_T("TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')"), m_strReturnDateStart);
	strEndDate.Format(_T("TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')"), m_strReturnDateEnd);

	strMakeHistoryQuery.Format(	_T("SELECT CTCLU.NAME, ")		//�̿��� �̸�
								_T("CTCLU.USER_NO, ")			//�̿��� ��ȣ
								_T("CTCL.REG_NO, ")				//�ڷ� ��Ϲ�ȣ
								_T("CTCL.TITLE, ")				//�ڷ��
								_T("TO_CHAR(CTCL.LOAN_DATE,'YYYY/MM/DD HH24:MI:SS'), ")		//������
								_T("TO_CHAR(CTCL.RETURN_DATE,'YYYY/MM/DD HH24:MI:SS'), ")	//�ݳ���
								_T("CTCL.RETURN_PLAN_DATE, ")	//�ݳ�������
								_T("CTCL.STATUS, ")				//���� ����
								_T("CTRI.%s ")					//m_nHistoryMode - 0 : ��û�� ������ 1 :ó���� ������
								_T("FROM CT_TEMP_RETURN_INFO_TBL CTRI, CT_CO_LOAN_INFO_TBL CTCL, CT_MASTER_LOAN_USER_TBL CTCLU ")
								_T("WHERE CTRI.UPDATED_TABLE_REC_KEY=CTCL.CLIENT_REC_KEY ")
								_T("AND CTRI.UPDATE_LIB_CODE=CTCL.LIB_CODE ")
								_T("AND CTCL.MASTER_USER_KEY=CTCLU.REC_KEY ")
								_T("AND CTRI.STATUS IN ('0','1') ")
								_T("AND CTRI.%s='%s' "),
								strHistoryField, 
								strWhereField,
								strMyLibCode
								);

	// 4. �̷� ��� ������ ����
	CString strTemp;
	
	m_strSearchLibCode.TrimLeft();		
	m_strSearchLibCode.TrimRight();		
	m_strSearchLibCode.Replace( _T(" ") , _T("") );

	if(  _T("") == m_strSearchLibCode )
	{
		// ������ ���� �� ���� ���, ��ü�� �������� �˻�
		strTemp.Format( _T("AND CTRI.REQUEST_DATE BETWEEN %s AND %s ")	//�˻� �Ⱓ
						_T("ORDER BY CTRI.REQUEST_DATE ASC "),			//��û��¥������ ����
						strStartDate,
						strEndDate );
		strMakeHistoryQuery += strTemp;

		m_strSearchLibName	=	_T("�� ü");
	}
	else
	{
		strTemp.Format( _T("AND CTRI.%s='%s' ")//m_nHistoryMode - 0 : ó���� ������ , 1 : ��û�� ������
						_T("AND CTRI.REQUEST_DATE BETWEEN %s AND %s ")	//�˻� �Ⱓ
						_T("ORDER BY CTRI.REQUEST_DATE ASC "),			//��û��¥������ ����
						strHistoryField,
						m_strSearchLibCode,
						strStartDate, 
						strEndDate );
		strMakeHistoryQuery += strTemp;		
	}
	
	return 0;
}

/*
@Create
 - 2009.02.25 BY KSJ
@Description
 - �ڰ� ������ �ڵ� ���ϱ�
@Parameter
 - strMyLibCode : ������ �ڵ� ���� �� ����
@Return Value
 - 0 : ����
@Mini Spec
 1. MN_LIBINFO2_TBL���� �ڰ� ������ �ڵ带 �����´�.
*/
INT CUNION_RETURN_HIS_MAIN::GetMyLibCode(CString &strMyLibCode)
{
	INT		  ids;
	const int nFieldCnt = 1;
	CString	  strField[ nFieldCnt ] = { _T("LIB_CODE") };
	CString   strLibCode[ nFieldCnt ];

	ids = CLocCommonAPI::GetLibInfo( strField, strLibCode, nFieldCnt, this );
	if( 0 > ids ) 
	{
		return ids;
	}

	strMyLibCode = strLibCode[0];
	return 0;
}

/*
@Create
 - 2009.02.24 BY KSJ
@Description
 - ������ List ��������
@Parameter
 - 
@Return Value
 - 0 : ����
@Mini Spec
 1. ���� DB�� �����Ͽ� ������ List�� �����´�
*/
INT CUNION_RETURN_HIS_MAIN::InitLibList()
{
EFS_BEGIN

	INT ids;
	CRS_SQL			RemoteDBMgr(this);
	CESL_DataMgr*	pLibraryDM = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
	if( NULL == pLibraryDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitLibList") );
	}

	ids = RemoteDBMgr.SetFetchCount(0, 100);
	if( 0 > ids ) 
	{
		return ids;
	}

	ids = RemoteDBMgr.LOAN_Select(pLibraryDM, _T(""), _T("DM_UNION_RETURN_LIBINFO"), 1, 1);
	if( 0 > ids ) 
	{
		return ids;
	}

	return 0;

EFS_END
	return -1;
}

/*
@Create
 - 2009.02.27 BY KSJ
@Description
 - SIReport ���
@Parameter
 - 
@Return Value
 - 0 : ����
@Mini Spec
 1. ��� DM �� ��� DM �ʱ�ȭ
 2. ��� �ʵ� ����
 3. ��� Row ����->������ �ο찡 ���ٸ� ��ü ���
 4. pSrcDM -> pPrintDM���� ����
 5. Data ���
*/
INT CUNION_RETURN_HIS_MAIN::PrintList()
{
EFS_BEGIN

	INT ids;
	
	// =========================================================================================
	// 1. ��� DM �� ��� DM �ʱ�ȭ 
	// =========================================================================================
	CESL_DataMgr* pSrcDM = FindDM( _T("DM_UNION_RETURN_HISTORY") );	
	if( NULL == pSrcDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	}
	INT		nRowCnt = pSrcDM->GetRowCount();
	if( 1 > nRowCnt )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}
	
	CESL_DataMgr* pPrintDM = FindDM( _T("DM_UNION_RETURN_HISTORY_PRINT") );
	if( NULL == pPrintDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("PrintList") );
	}
	pPrintDM->FreeData();

	// =========================================================================================
	// 2. ��� �ʵ� ����
	// =========================================================================================	
	const int nCopyCnt = 9;
	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("�������̸�"), _T("�������̸�") },
		{ _T("�����ڹ�ȣ"), _T("�����ڹ�ȣ") },
		{ _T("��Ϲ�ȣ"),	_T("��Ϲ�ȣ") },
		{ _T("����"),		_T("����") },
		{ _T("������"),		_T("������") },
		{ _T("�ݳ���"),		_T("�ݳ���") },
		{ _T("�ݳ�������"), _T("�ݳ�������") },
		{ _T("������¼���"),_T("������¼���") },
		{ _T("����������"),	_T("����������") },
	};
	
	// =========================================================================================
	// 3. ��¸�� ����
	//    ������ �ο찡 ���ٸ� ��ü ���
	// =========================================================================================
	CESL_Grid* pGrid;
	if( 0 == m_nHistoryMode )
	{
		pGrid = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_Ÿ���ݳ�") );
	}
	else if( 1 == m_nHistoryMode )
	{
		pGrid = (CESL_Grid*)FindControl( _T("CM_UNION_RETURN_HISTORY_MAIN") , _T("Grid_Ÿ���ݳ�����") );
	}
				
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	
	INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();
	
	if( 0 < nSelectCnt )
	{
		nMode = 1;
	}
	else
	{
		nSelectCnt = pSrcDM->GetRowCount();
	}
	
	// =========================================================================================
	// 4. pSrcDM -> pPrintDM���� ����
	// =========================================================================================
	CString strData;
	INT nIndex;
	INT i;
	for( i=0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
		{
			nIndex = i;
		}
		else
		{
			nIndex = pGrid->SelectGetIdx();		
		}

		pPrintDM->InsertRow(-1);
		
		strData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("����") , strData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("PrintList") );
		
		for( int j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , strData );
			if( 0 > ids ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , strData : %s ") , nIndex , sField[j][0] , strData );
				AfxMessageBox( sEMsg );
			}

			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
			}
			
			ids = pPrintDM->SetCellData( sField[j][1] , strData , i );
			if( 0 > ids )
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , strData : %s ") , i , sField[j][0] , strData );
				AfxMessageBox( sEMsg );
			}

			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("PrintList") );
			}
		}
		if( nMode == 1)
		{
			pGrid->SelectGetNext();
		}
	}
	
	// =========================================================================================	
	// 5. Data ���
	// =========================================================================================	
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( int( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if( NULL == pSIReportManager )
	{
		AfxMessageBox( _T("Report Manager�� �������� ���Ͽ����ϴ�.") );
		return 1;
	}

	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	if( 0 == m_nHistoryMode )
	{
		ids = pSIReportManager->GetSIReportFileFromDB( _T("Ÿ���ݳ����") );
		if( 0 > ids ) 
		{
			AfxMessageBox( _T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�.") );
			return 1;
		}
	}
	else if( 1 == m_nHistoryMode )
	{
		ids = pSIReportManager->GetSIReportFileFromDB( _T("Ÿ���ݳ�������") );
		if( 0 > ids ) 
		{
			AfxMessageBox( _T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�.") );
			return 1;
		}
	}

	// ���� ġȯ : �˻� �Ⱓ
	CString		strDate;
	strDate.Format( _T("%s ~ %s") , m_strReturnDateStart , m_strReturnDateEnd );
	ids = pSIReportManager->SetTempVariable( _T("�Ⱓ") , strDate );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("PrintList") );
	}
	
	// ���� ġȯ : �˻� ��������
	ids = pSIReportManager->SetTempVariable( _T("��������") , m_strSearchLibName );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintList") );
	}

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if( NULL != pSIReportManager )
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;
	
EFS_END
	return -1;
}


/*
@Create
 - 2009.02.27 BY KSJ
@Description
 - SIReport Event Handle ���
@Parameter
 - EventInfo[OUT] : SIReport Event Handle
@Return Value
 - 0 : ����
@Mini Spec
 1.
*/
INT CUNION_RETURN_HIS_MAIN::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if( NULL == pSIReportManager )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SIReportEventHandler") );
	}
	
	return 0;
	
EFS_END
	return -1;	
}
