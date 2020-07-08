// SynchronizedSearchEngineMgr.cpp : implementation file
//

#include "stdafx.h"
#include "SynchronizedSearchEngineMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSynchronizedSearchEngineMgr dialog

// ���� �׸��� ID
//	IDC_gridMONO_DATA
// ���� �׸��� ID
//	IDC_gridSERIAL_DATA
// ��� �׸��� ID
//	IDC_gridARTICLE_DATA

CSynchronizedSearchEngineMgr::CSynchronizedSearchEngineMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSynchronizedSearchEngineMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSynchronizedSearchEngineMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	// DM List
	m_pMonoDM    = NULL;
	m_pSerialDM	 = NULL;
	m_pArticleDM = NULL;
	
	m_pAllMonoDM    = NULL;
	m_pAllSerialDM  = NULL;
	m_pAllArticleDM = NULL;

	m_pCM = NULL;
	
	// Grid Lisst
	m_pMonoGrid	   = NULL;
	m_pSerialGrid  = NULL;
	m_pArticleGrid = NULL;
	
	m_pSearchDlg = NULL;
	m_pSpeciesApi = NULL;
	m_pDataProcessingDlg =  NULL;


	m_pArticleSDlg = NULL;
}

CSynchronizedSearchEngineMgr::~CSynchronizedSearchEngineMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(m_pSpeciesApi)
	{
		//delete m_pSpeciesApi;
		m_pSpeciesApi = NULL;
	}
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}

	if( m_pArticleSDlg )
	{
		delete m_pArticleSDlg;
		m_pArticleSDlg = NULL;
	}
}

VOID CSynchronizedSearchEngineMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSynchronizedSearchEngineMgr)
	DDX_Control(pDX, IDC_tabDATA_CLASS, m_tabDataClass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSynchronizedSearchEngineMgr, CDialog)
	//{{AFX_MSG_MAP(CSynchronizedSearchEngineMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabDATA_CLASS, OnSelchangetabDATACLASS)
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE( SEARCH_ARTICLE, OnSearchArticleData )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSynchronizedSearchEngineMgr message handlers

VOID CSynchronizedSearchEngineMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	DestroyWindow();
}

BOOL CSynchronizedSearchEngineMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_�˻���������ȭ")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_�˻���������ȭ"));
	m_pMonoDM = FindDM(_T("DM_�˻���������ȭ_����"));
	m_pSerialDM = FindDM(_T("DM_�˻���������ȭ_����"));
	m_pAllMonoDM = FindDM(_T("DM_��ü�籸��_����")); 
	m_pAllSerialDM = FindDM(_T("DM_��ü�籸��_����")); 
	m_pArticleDM = FindDM( _T("DM_�˻���������ȭ_���") );
	m_pMonoGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Mono"));
	m_pMonoGrid->m_fpEventHandler   = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pSerialGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Serial"));
	m_pSerialGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pArticleGrid = ( CESL_Grid * )m_pCM->FindControl( _T("MainGrid_Article") );
	m_pArticleGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	m_tabDataClass.InsertItem(0, _T("����"));
	m_tabDataClass.InsertItem(1, _T("����"));
	m_tabDataClass.InsertItem(2, _T("���"));
	m_tabDataClass.SetCurSel(0);
	
	// ���� �߰�
	m_pArticleSDlg = new CArticleSearchDlg( this );
	m_pArticleSDlg->Create( IDD_ARTICLE_SEARCH, this );

	
	
	m_pSpeciesApi = new CBO_SPECIES(this);

	CString strMsg = _T("���� ����/�����ڷ��� �˻����� ����ȭ�� �����մϴ�.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	InitPopupMenuItem();		
	SetTabSelect(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSynchronizedSearchEngineMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tabDATA_CLASS);
	if(pTab->GetSafeHwnd() == NULL) return;
	//pTab->MoveWindow(5, 5, cx-10, cy-10);	
	pTab->MoveWindow(0, 0, cx+4, cy);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMONO_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(15, 32, cx-30, cy-45);
	pGrid->MoveWindow(1, 60, cx-1, cy-75);

	pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridSERIAL_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(15, 32, cx-30, cy-45);
	pGrid->MoveWindow(1, 60, cx-1, cy-75);

	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridARTICLE_DATA );
	//pGrid->MoveWindow( 15, 32, cx - 30, cy - 45 );
	pGrid->MoveWindow(1, 60, cx-1, cy-75);
}

VOID CSynchronizedSearchEngineMgr::ShowSearchDlg()
{
	
	
	if( 2 != m_tabDataClass.GetCurSel() )
		m_pSearchDlg->ShowSearchDlg();
	else
		m_pArticleSDlg->ShowWindow( true );
}

VOID CSynchronizedSearchEngineMgr::RestructAllData()
{
	CESL_DataMgr *pDM = GetActiveAllDM();
	CString idxTbl, tmpTbl, query, menu, result;
	
	query.Format( _T("SELECT PERMITION FROM MN_USER_TBL WHERE ID = '%s'"), GetUserID() );
	pDM->GetOneValueQuery( query, result );

	if( _T("A") != result )
	{
		AfxMessageBox( _T("��ü ����ȭ �۾��� ���� ������ �����ϴ�.") );
		return;
	}
	
	const INT cnt = 3;
	CString initQuery[ cnt ] = {
		_T("INSERT INTO IDX_TMP_BO_TBL( REC_KEY, SPECIES_KEY, IDX_WORK_FLAG ) VALUES( 0, 0, 'A' );"),
		_T("INSERT INTO IDX_TMP_SE_TBL( REC_KEY, SPECIES_KEY, IDX_WORK_FLAG ) VALUES( 0, 0, 'A' );"),
		_T("INSERT INTO IDX_TMP_SE_TOC_TBL( REC_KEY, ARTICLE_KEY, IDX_WORK_FLAG ) VALUES( 0, 0, 'A' );") };
	CString insertQuery[ cnt ] = {
		_T("INSERT INTO IDX_TMP_BO_TBL( SELECT ESL_SEQ.NEXTVAL, REC_KEY, 'N', SYSDATE FROM IDX_BO_TBL WHERE SPECIES_CLASS IN ( '0', '1' ) );"),
		_T("INSERT INTO IDX_TMP_SE_TBL( SELECT ESL_SEQ.NEXTVAL, REC_KEY, 'N', SYSDATE FROM IDX_SE_TBL WHERE SPECIES_CLASS IN ( '0', '1' ) );"),
		_T("INSERT INTO IDX_TMP_SE_TOC_TBL( SELECT ESL_SEQ.NEXTVAL, REC_KEY, 'N', SYSDATE FROM IDX_SE_TOC_TBL );") };

	for( INT i = 0; i < cnt; i++ )
	{
		pDM->ExecuteQuery( initQuery[ i ], true );
		pDM->ExecuteQuery( insertQuery[ i ], true );
	}

	AfxMessageBox( _T("�˻����� ����ȭ ��ü �籸���� �����Ͽ����ϴ�.") );	

	
	/*
	CTabCtrl *pTab = ( CTabCtrl * )GetDlgItem( IDC_tabDATA_CLASS );
	switch( pTab->GetCurSel() )
	{
		case 0 :	// ����
			menu = _T("����");
			idxTbl = ;
			tmpTbl = _T("IDX_TMP_BO_TBL");
			pDM->ExecuteQuery( _T("INSERT INTO IDX_TMP_BO_TBL( REC_KEY, SPECIES_KEY, IDX_WORK_FLAG ) VALUES( 0, 0, 'A' );"), true );
			query.Format( _T("INSERT INTO %s ( SELECT ESL_SEQ.NEXTVAL, REC_KEY, 'N', SYSDATE FROM %s WHERE SPECIES_CLASS IN ( '0', '1' ) );"), tmpTbl, idxTbl );
			break;
		case 1 :	// ����
			menu = _T("����");
			idxTbl = ;
			tmpTbl = _T("IDX_TMP_SE_TBL");
			pDM->ExecuteQuery( _T("INSERT INTO IDX_TMP_SE_TBL( REC_KEY, SPECIES_KEY, IDX_WORK_FLAG ) VALUES( 0, 0, 'A' );"), true );
			query.Format( _T("INSERT INTO %s ( SELECT ESL_SEQ.NEXTVAL, REC_KEY, 'N', SYSDATE FROM %s WHERE SPECIES_CLASS IN ( '0', '1' ) );"), tmpTbl, idxTbl );
			break;
		case 2 :	// ���
			menu = _T("���");
			idxTbl = _T("IDX_SE_TOC_TBL");
			tmpTbl = _T("IDX_TMP_SE_TOC_TBL");
			pDM->ExecuteQuery( _T("INSERT INTO IDX_TMP_SE_TOC_TBL( REC_KEY, ARTICLE_KEY, IDX_WORK_FLAG ) VALUES( 0, 0, 'A' );"), true );
			query.Format( _T("INSERT INTO %s ( SELECT ESL_SEQ.NEXTVAL, REC_KEY, 'N', SYSDATE FROM %s );"), tmpTbl, idxTbl );
			break;
		default :
			return;
			break;
	}
	
	CString error;
	if( pDM->ExecuteQuery( query, true ) )
		error.Format( _T("%s �����Ͽ� �˻����� ����ȭ �۾��� ������ ������ �߻��Ͽ����ϴ�."), menu );
	else
		error.Format( _T("%s �� ���� �˻����� ����ȭ �۾��� �����Ͽ����ϴ�."), menu );
	AfxMessageBox( error );
	*/
	/*
	CString strQueryFmt, sInsertSQL;
	strQueryFmt = _T("INSERT INTO %s ")
//			   _T("(SELECT esl_seq.NEXTVAL, rec_key, 'I', SYSDATE ")
			   _T("(SELECT rec_key , rec_key, 'I', SYSDATE ")
			   _T("FROM %s ")
			   _T("WHERE species_class IN ('0', '1'));");

	CString strTblName, strMsg;
	INT nCurSel = m_tabDataClass.GetCurSel();
	if(nCurSel == 0)
	{
		strTblName = _T("IDX_TMP_BO_TBL");
	}
	else if(nCurSel == 1)
	{
		strTblName = _T("IDX_TMP_SE_TBL");
	}	
	
	sInsertSQL.Format(strQueryFmt, strTblName, GetActiveAllDM()->TBL_NAME);
	INT ids = GetActiveAllDM()->ExecuteQuery(sInsertSQL, TRUE);
	if(ids < 0) ESLAfxMessageBox(_T("��ü�ڷῡ ���Ͽ� �˻����� ����ȭ �۾��� ������ ������ �߻��Ͽ����ϴ�."));
	else ESLAfxMessageBox(_T("��ü�ڷῡ ���Ͽ� �˻����� ����ȭ �۾��� �����Ͽ����ϴ�."));
	return ;*/
}

INT CSynchronizedSearchEngineMgr::CheckSelectDataValid()
{
	if(GetActiveDM()->GetRowCount() == 0) return -1;

	INT ids = GetActiveGrid()->SelectMakeList();
	if(ids < 0) return -1;
	ids = GetActiveGrid()->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CSynchronizedSearchEngineMgr::SynchornizedSearchEngine()
{
	if(CheckSelectDataValid() < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CString strKey, strKeyAlias, strTblName;
	INT nCurSel = m_tabDataClass.GetCurSel();
	
	if( 2 == nCurSel )
	{
		SynchronizeArticleSearchEngine();
		return;
	}
	
	if(nCurSel == 0)
	{
		strKeyAlias = _T("IBS_��KEY");
		strTblName = _T("IDX_TMP_BO_TBL");
	}
	else if(nCurSel == 1)
	{
		strKeyAlias = _T("ISS_��KEY");
		strTblName = _T("IDX_TMP_SE_TBL");
	}
	else if( 2 == nCurSel )
	{
		strKeyAlias = _T("REC_KEY");
		strTblName = _T("IDX_TMP_SE_TOC_TBL");
	}
	
	INT i = GetActiveGrid()->SelectGetHeadPosition();
	GetActiveDM()->StartFrame();

	CString strMsg;
	INT nPos = 1;
	ShowDataProcessingDlg();
	strMsg.Format(_T("%d���� �ڷḦ �˻����� ����ȭ ó���ϰ� �ֽ��ϴ�."), GetActiveGrid()->SelectGetCount());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, GetActiveGrid()->SelectGetCount());

	while(i >= 0)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nPos++);

		strKey = GetActiveDM()->GetCellData(strKeyAlias, i);
		if(!strKey.IsEmpty())
		{
			m_pSpeciesApi->BO_INDEX_TEMP_INSERT(GetActiveDM(), strTblName, strKey, _T("I"));
		}
		i = GetActiveGrid()->SelectGetNext();
	}
	
	strMsg.Format(_T("�˻����� ����ȭ ���Դϴ�."));
	m_pDataProcessingDlg->SetMessage(strMsg);
	INT ids = m_pSpeciesApi->SendFrame(GetActiveDM(), 100);
	DeleteDataProcessingDlg();

	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�˻����� ����ȭ �۾��� ������ �� �����ϴ�."));
		return ;
	}
	ESLAfxMessageBox(_T(" �����Ͻ� �ڷῡ ���Ͽ� �˻����� ����ȭ �۾��� �����Ͽ����ϴ�."));
}

BOOL CSynchronizedSearchEngineMgr::Create(CWnd *pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

CESL_DataMgr * CSynchronizedSearchEngineMgr::GetActiveDM()
{
	CESL_DataMgr * pDM = NULL;
	INT nCurSel = m_tabDataClass.GetCurSel();
	if(nCurSel == 0)
		pDM = m_pMonoDM;
	else if(nCurSel == 1)
		pDM = m_pSerialDM;
	else if( 2 == nCurSel )
		pDM = m_pArticleDM;
	return pDM;
}

CESL_Grid * CSynchronizedSearchEngineMgr::GetActiveGrid()
{
	CESL_Grid * pGrid = NULL;
	INT nCurSel = m_tabDataClass.GetCurSel();
	if(nCurSel == 0)
		pGrid = m_pMonoGrid;
	else if(nCurSel == 1)
		pGrid = m_pSerialGrid;
	else if( 2 == nCurSel )
		pGrid = m_pArticleGrid;
	return pGrid;
}

VOID CSynchronizedSearchEngineMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pMonoGrid->InitPopupMenuItem();
	m_pSerialGrid->InitPopupMenuItem();
	m_pSerialGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	m_pMonoGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	
	m_pArticleGrid->InitPopupMenuItem();
	m_pArticleGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

VOID CSynchronizedSearchEngineMgr::OnSelchangetabDATACLASS(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT nCurSel = m_tabDataClass.GetCurSel();
	SetTabSelect(nCurSel);
	*pResult = 0;
}

VOID CSynchronizedSearchEngineMgr::SetTabSelect(INT nCurSel)
{
	m_pMonoGrid->ShowWindow(SW_HIDE);
	m_pSerialGrid->ShowWindow(SW_HIDE);
	m_pArticleGrid->ShowWindow( SW_HIDE );

	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(nCurSel == 0)
	{
		m_pSearchDlg = new CSearchMgr(this, BO_DATA_SEARCH, ACQ_TYPE_INVALID, SPECIES_TYPE_INVALID, GetActiveDM()->CONNECTION_INFO);
		m_pMonoGrid->ShowWindow(SW_SHOW);
	}
	else if(nCurSel == 1)
	{
		m_pSearchDlg = new CSearchMgr(this, SE_DATA_SEARCH, ACQ_TYPE_INVALID, SPECIES_TYPE_INVALID, GetActiveDM()->CONNECTION_INFO);
		m_pSerialGrid->ShowWindow(SW_SHOW);
	}
	else if( 2 == nCurSel )
	{
		// 
		m_pArticleGrid->ShowWindow( SW_SHOW );
	}
	SetSearchDataInfo();
}

UINT CSynchronizedSearchEngineMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	CString strQuery;
	m_pSearchDlg->GetWhereStmt(strQuery);
	GetActiveDM()->RefreshDataManager(strQuery);
	
	SetSearchDataInfo();
	return 0;
}

INT CSynchronizedSearchEngineMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData;
	if(strDMFieldAlias == _T("UDF_�ڷ���¼���"))
	{
		strTmpData = GetActiveDM()->GetCellData(_T("IBS_�ڷ����"), nRow);
		strTmpData = strTmpData.Left(3);
		if(strTmpData == _T("BOT")) str = _T("��������ڷ�");
		else if(strTmpData == _T("BOA")) str = _T("�����ڷ�");
		else if(strTmpData == _T("BOR")) str = _T("����ڷ�");
		else if(strTmpData == _T("BOC")) str = _T("�����ڷ�");
		else if(strTmpData == _T("BOL")) str = _T("�����ڷ�");
		else str = _T("��������ڷ�");
		GetActiveDM()->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_������¼���"))
	{
		strTmpData = GetActiveDM()->GetCellData(_T("ISS_�������"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������ڵ�"), strTmpData, str);
		GetActiveDM()->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_����󵵼���"))
	{
		strTmpData = GetActiveDM()->GetCellData(_T("ISS_�����"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�����"), strTmpData, str);
		GetActiveDM()->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CSynchronizedSearchEngineMgr::SetSearchDataInfo()
{	
	INT nCurSel = m_tabDataClass.GetCurSel();
	CString strMsg;
	if(nCurSel == 0)
		strMsg.Format(_T("%d���� �����ڷḦ �˻��Ͽ����ϴ�."), GetActiveDM()->GetRowCount());
	else if(nCurSel == 1)
		strMsg.Format(_T("%d���� �����ڷḦ �˻��Ͽ����ϴ�."), GetActiveDM()->GetRowCount());
	else if( 2 == nCurSel )
		strMsg.Format( _T("%d���� ����ڷḦ �˻��Ͽ����ϴ�."), GetActiveDM()->GetRowCount() );
	
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	GetActiveGrid()->Display();
}

CESL_DataMgr * CSynchronizedSearchEngineMgr::GetActiveAllDM()
{
	CESL_DataMgr * pDM = NULL;
	INT nCurSel = m_tabDataClass.GetCurSel();
	if(nCurSel == 0)
		pDM = m_pAllMonoDM;
	else if(nCurSel == 1)
		pDM = m_pAllSerialDM;
	return pDM;
}

VOID CSynchronizedSearchEngineMgr::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(this);

	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(this))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//���α׷��� ��Ʈ�� �ʱ�ȭ
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CSynchronizedSearchEngineMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

VOID CSynchronizedSearchEngineMgr::OnSearchArticleData()
{
	m_pArticleDM->RefreshDataManager( m_articleSCond );
	m_pArticleGrid->Display();
}

VOID CSynchronizedSearchEngineMgr::SynchronizeArticleSearchEngine()
{
	const INT fieldCnt = 4;
	CString fieldAlias[ fieldCnt ] = {
		_T("REC_KEY"), _T("ARTICLE_KEY"), _T("IDX_WORK_FLAG"), _T("INPUT_DATE") };
	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") };
	CString fieldData[ fieldCnt ] = {
		_T("ESL_SEQ.NEXTVAL"), _T(""), _T("I"), _T("") };
	
	CTime t = m_pArticleDM->GetDBTime();
	fieldData[ 3 ].Format( _T("%04d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );

	m_pArticleDM->StartFrame();
	m_pArticleDM->InitDBFieldData();
		
	INT rowCnt = m_pArticleGrid->GetRows();
	for( INT i = 1; i < rowCnt; i++ )
		if( _T("V") == m_pArticleGrid->GetTextMatrix( i, 1 ) )
		{
			fieldData[ 1 ].Format( _T("%s"), m_pArticleDM->GetCellData( _T("REC_KEY"), i - 1 ) );

			for( INT j = 0; j < fieldCnt; j++ )
				m_pArticleDM->AddDBFieldData( fieldAlias[ j ], fieldType[ j ], fieldData[ j ] );
			m_pArticleDM->MakeInsertFrame( _T("IDX_TMP_SE_TOC_TBL") );
		}
	m_pArticleDM->SendFrame();
	m_pArticleDM->EndFrame();

	AfxMessageBox( _T(" �����Ͻ� �ڷῡ ���Ͽ� �˻����� ����ȭ �۾��� �����Ͽ����ϴ�.") );
}
