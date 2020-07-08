// ECO_NOTICE_MANAGER1.cpp : implementation file
//

#include "stdafx.h"
#include "ECO_NOTICE_MANAGER1.h"
#include "ECO_NOTICE_LIB_CODE.h"
#include "..\..\..\�������\check_dup_api\RS_SQL.h"
#include <direct.h>
#include "..\\..\\include\\�������\\RS_Acc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECO_NOTICE_MANAGER dialog


CECO_NOTICE_MANAGER::CECO_NOTICE_MANAGER(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CECO_NOTICE_MANAGER)
	//}}AFX_DATA_INIT
	m_FILE_INFO.FileCnt = 0;
}


VOID CECO_NOTICE_MANAGER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CECO_NOTICE_MANAGER)
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressCtrl);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CECO_NOTICE_MANAGER, CDialog)
	//{{AFX_MSG_MAP(CECO_NOTICE_MANAGER)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bIDOK, OnbIDOK)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bINCLUDE_FILE, OnbINCLUDEFILE)
	ON_BN_CLICKED(IDC_bAPPEND, OnbAPPEND)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDCANCEL2, OnCancel2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECO_NOTICE_MANAGER message handlers
BOOL CECO_NOTICE_MANAGER::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	m_pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB1);
	m_pTabCtrl->InsertItem(0,_T("�������׹߼�"));
	m_pTabCtrl->InsertItem(1,_T("������������"));
	m_pTabCtrl->SetCurSel(0);

	if(InitESL_Mgr(_T("SM_ECO_NOTICE_INPUT"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
    m_SERVER_TYPE = _T("LS") ;

	// TODO: Add extra initialization here
	m_button1.AutoLoad( IDC_bINCLUDE_FILE, this );
    m_SelectLibCnt = 0 ;

	m_pDM = (CRS_DM_Mgr*)FindDM(_T("DM_ECO_NOTICE_INPUT"));

	m_pDM->m_dbkind =  4; 
	if ( m_pDM == NULL )
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����(DM_ECO_NOTICE)"));
		return FALSE;
	}

	INT ids;

	ids = m_pDM->RefreshDataManager(_T(""));

	// CM �ʱ�ȭ
	CESL_ControlMgr *pCM = FindCM(_T("CM_ECO_NOTICE_INPUT"));
	if ( pCM == NULL )
	{
		AfxMessageBox(_T("CM �ʱ�ȭ ����(CM_ECO_NOTICE_INPUT)"));
		return FALSE;
	}
	// Grid �ʱ�ȭ
	m_pGrid = (CESL_Grid*)pCM->FindControl( _T("MainGrid"));
	if ( m_pGrid == NULL )
	{
		AfxMessageBox(_T("GRID �ʱ�ȭ ����(CM_ECO_NOTICE_INPUT)"));
		return FALSE;
	}
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	// grid display

	if( m_pGrid->Display() < 0 ) return FALSE;

	Display(0);

	AllControlDisplay (_T("CM_ECO_NOTICE_INPUT") ,-1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

INT CECO_NOTICE_MANAGER::Display(INT nMode)
{
	if( nMode == 0 )
	{
		(CWnd*)GetDlgItem(IDC_eRECEIPT_LIB)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_STATIC3)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_STATIC5)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_eTHEME)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_LIST1)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_bINCLUDE_FILE)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_bSEARCH)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_PROS)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_PROGRESS)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_MainGrid)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_bAPPEND)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_bIDOK)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDCANCEL2)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_eVIEW)->ShowWindow(SW_SHOW);
		
	}
	else
	{
		(CWnd*)GetDlgItem(IDC_eRECEIPT_LIB)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_STATIC5)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_eTHEME)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_LIST1)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_bINCLUDE_FILE)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_bSEARCH)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_PROS)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_PROGRESS)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_bAPPEND)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_bIDOK)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDCANCEL2)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_eVIEW)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_MainGrid)->ShowWindow(SW_SHOW);

		m_pDM = (CRS_DM_Mgr*)FindDM(_T("DM_ECO_NOTICE_INPUT"));

		m_pDM->m_dbkind =  4; 
		if ( m_pDM == NULL )
		{
			AfxMessageBox(_T("DM �ʱ�ȭ ����(DM_ECO_NOTICE)"));
			return FALSE;
		}

		INT ids;

		ids = m_pDM->RefreshDataManager(_T(""));

		CESL_ControlMgr *pCM = FindCM(_T("CM_ECO_NOTICE_INPUT"));
		if ( pCM == NULL )
		{
			AfxMessageBox(_T("CM �ʱ�ȭ ����(CM_ECO_NOTICE_INPUT)"));
			return FALSE;
		}

		m_pGrid->DisplayLine(-1);
			
	}

	INT nCurSel = m_pGrid->GetTopRow();
	m_pGrid->SetReverse(0);
	m_pGrid->SetTopRow(nCurSel);
	return 0;
}

VOID CECO_NOTICE_MANAGER::OnbIDOK() 
{
	CESL_DataMgr * m_pDataMgr;
	CString        ParentRecKey, LibraryCode, LibraryName, LocalFileRecKey, tmpData;
	INT            ids, i;

	//////////////////////////////////////////////////////////////////////////////////
	// 1. ������ ���ÿ��θ� üũ�Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////
	if ( m_SelectLibCnt == 0 ) {
		MessageBox( _T("���������� ���� �������� �����Ͻʽÿ�"), _T("Ȯ��"), MB_OK );
		return;
	}

    m_pDataMgr = FindDM(_T("DM_ECO_NOTICE_INPUT")) ;
    if(m_pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error"));
		return ;
	}

    CRS_SQL m_rs_sql(this) ;
	//m_rs_sql.GetLibraryInfo (LibraryCode);
	CString sWhere ;
	sWhere.Format (_T("select LIB_NAME from LIB_INFO_tbl where lib_CODE = '%s'"), _T("999999") ) ;
    m_rs_sql.ES_GetOneFieldValue (sWhere,LibraryName ) ;


	//////////////////////////////////////////////////////////////////////////////////
	// 1. center�� ����
	//////////////////////////////////////////////////////////////////////////////////

	for ( i = 0; i < m_SelectLibCnt; i++ ) {
		
		//////////////////////////////////////////////////////////////////////////////////
		// 2. ���������� �����Ѵ�.
		//////////////////////////////////////////////////////////////////////////////////
				
		m_SERVER_TYPE =_T("ES") ;
        if(m_SERVER_TYPE == _T("ES"))
		{
			ids = Input( m_ParentRecKey, i, LibraryCode, LibraryName );
			if ( ids<0 ) {
				AfxMessageBox( _T("�Է��� �����߽��ϴ�."), MB_OK);
				return;
			}
			if (ids == 100 )
			{
				return ;
			}
			
			//////////////////////////////////////////////////////////////////////////////////
			// 3. ���������� ������ center�� �����Ѵ�.
			//////////////////////////////////////////////////////////////////////////////////
			ids = AllInputLibraryInfo(  m_select_Lib_Code.GetAt (i) , 
										m_select_Lib_Name.GetAt (i) , 
										m_ParentRecKey );
		

			ids = InsertFile_Proc( m_pDataMgr, LocalFileRecKey, m_ParentRecKey ,m_SERVER_TYPE );
			if ( ids<0 ) {
				AfxMessageBox( _T("LocalInsertFile_Proc() ERROR"), MB_OK);
				return;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////
		// 4. local�� ����
		//////////////////////////////////////////////////////////////////////////////////
		
	    m_SERVER_TYPE =_T("LS") ;
		/*
		if(m_SERVER_TYPE == _T("LS"))
		{
			ids = m_pDataMgr->StartFrame();
			m_pDataMgr->InitDBFieldData () ;
        
			ids = Input( ParentRecKey, i, LibraryCode, LibraryName );
			if ( ids<0 ) {
				m_pDataMgr->EndFrame();
				AfxMessageBox ( _T("�Է��� �����߽��ϴ�."), MB_OK);
				return;
			}

			ids = m_pDataMgr->SendFrame();
			ids = m_pDataMgr->EndFrame();
		
	
			ids = InsertFile_Proc( m_pDataMgr, LocalFileRecKey, ParentRecKey ,m_SERVER_TYPE );
			if ( ids<0 ) {
				AfxMessageBox( _T("LocalInsertFile_Proc() ERROR"), MB_OK);
				return;
			}

		}*/
/*
		tmpData.Format ( _T("%d/%d ������"), i+1,  );
		m_pProgressBar.SetWindowText( tmpData );
		m_pProgressBar.StepIt();
*/
	}



	//ids = AllControlDisplay(_T("CM_ECO_NOTICE_MAIN"),-1);

	//m_pProgressBar.SetWindowText( _T("�Ϸ�") );

    MessageBox( _T("���������� ���½��ϴ�."), _T("�Է�"), MB_OK );
	
	CESL_ControlMgr *pCM = FindCM(_T("CM_ECO_NOTICE_INPUT"));
	pCM->AllControlClear () ;
}
////////////////////////////////////////////////////////////////////////////////////
// Input
//
// [PURPOSE]
// [MINISPEC]
// 1. �Է��׸��� üũ�Ѵ�.
// 2. Local �����϶��� ������������
// 3. center �����϶��� ������������
// 4. Local DB �����϶��� Momory�� Insert ��
// 5. center DB �����϶��� Momory�� Insert ����
////////////////////////////////////////////////////////////////////////////////////
INT CECO_NOTICE_MANAGER::Input( CString RecKey, INT index, CString LibraryCode, CString LibraryName )
{
	CESL_DataMgr  * m_pDataMgr;
	CString         Subject, Contents;
	CString         CurrentDate;
	// KOL.UDF.022
	INT             ids = 0, RowCnt = 0;

    CString AttrData [10];
    CString AttrName [10] = { _T("REC_KEY")        , // ���ڵ�KEY
		                    _T("send_libid")      , // ���� ������ �ڵ�
							_T("send_libname")    , // ���� ������ ��
							_T("recv_libid")      , // �޴� ������ �ڵ�
							_T("recv_libname")    , // �޴� ������ ��
							_T("title")           , // ����
							_T("contents")        , // ����
							_T("confirm")         , // Ȯ��
							_T("srflag")          , // ������������
							_T("trans_date")      };
    CString    FieldType[10] = {_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

    m_pDataMgr = FindDM(_T("DM_ECO_NOTICE_INPUT")) ;
	if(m_pDataMgr == NULL){
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1 ;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// 1. �Է��׸��� üũ�Ѵ�.
	/////////////////////////////////////////////////////////////////////////////////////
	ids = GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("����"),Subject,-1) ;
	ids = GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("����"),Contents,-1) ;
	
	
	if ( m_select_Lib_Code.GetAt (0).IsEmpty () ) {
		MessageBox( _T("������������ �����Ͻʽÿ�."), _T("������ã��üũ"), MB_OK );
		return 100;
	}
	if ( m_arSelectLib.GetAt (index).IsEmpty () ||
		 Subject.IsEmpty () || 
		 Contents.IsEmpty () ) {
		MessageBox( _T("����Ÿ�� �Է��Ͻʽÿ�."), _T("�Է��׸�üũ"), MB_OK );
		return 100;
	}
	/*if(m_SERVER_TYPE == _T("LS"))
	{
		m_pDataMgr->MakeRecKey (RecKey) ;
		m_ParentRecKey = RecKey ;
	}
    else*/
	if( m_SERVER_TYPE == _T("ES"))
	{
		CRS_SQL m_rs_sql(this) ;
        CString sqlBuffer;
		CString temp ;
		sqlBuffer.Format (_T("select max(rec_key) from ECO_NOTICE_tbl")) ;
		m_rs_sql.ES_GetOneFieldValue (sqlBuffer,temp);
		
        INT itmp ;
		itmp = _ttoi(temp.GetBuffer (0)) ;
		RecKey.Format (_T("%d"),itmp+1) ;

		m_ParentRecKey = RecKey ;
	}


	AttrData[0] = RecKey;
	AttrData[1] = LibraryCode;
	AttrData[2] = LibraryName;
	AttrData[3] = m_select_Lib_Code.GetAt (index);
	AttrData[4] = m_select_Lib_Name.GetAt (index);
	AttrData[5] = Subject;
	AttrData[6] = Contents;
	AttrData[7] = _T("0");
	/////////////////////////////////////////////////////////////////////////////////////
	// 2. Local �����϶��� ������������
	/////////////////////////////////////////////////////////////////////////////////////
	if ( _T("LS") == m_SERVER_TYPE ) {
		AttrData[8] = _T("1");
	/////////////////////////////////////////////////////////////////////////////////////
	// 3. center �����϶��� ������������
	/////////////////////////////////////////////////////////////////////////////////////
	} else {
		AttrData[8] = _T("1");
	}

    CTime t = CTime::GetCurrentTime () ;
	CurrentDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
	AttrData[9] = CurrentDate;
     
    RowCnt ;
/*
	/////////////////////////////////////////////////////////////////////////////////////
	// 4. Local DB �����϶��� Momory�� Insert ��
	/////////////////////////////////////////////////////////////////////////////////////
	if ( _T("LS") == m_SERVER_TYPE  ) {
		for(INT i =0 ; i <10;i++) {
        m_pDataMgr->AddDBFieldData (AttrName[i],FieldType[i],AttrData[i]) ;
        }

		ids = m_pDataMgr->MakeInsertFrame (_T("ECO_NOTICE_TBL")) ;
	/////////////////////////////////////////////////////////////////////////////////////
	// 5. center DB �����϶��� Momory�� Insert 
	/////////////////////////////////////////////////////////////////////////////////////
	} else */
	if( m_SERVER_TYPE == _T("ES")){
		 CRS_SQL m_rs_sql(this);
		 INT FieldType[10] = { 2,1,1,1,1,1,1,1,1,1};
		 m_rs_sql.ES_InsertRow (_T("ECO_NOTICE_TBL"),10,AttrName,AttrData,FieldType,TRUE) ;

	}

	return 0;
}

VOID CECO_NOTICE_MANAGER::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	CComboBox * pCombo = (CComboBox*)GetDlgItem(IDC_eRECEIPT_LIB) ;
	CECO_NOTICE_LIB_CODE dlg(this) ;
	if(IDOK == dlg.DoModal ()) 
	{
		//dlg.m_arLibrary ;
        INT Cnt ;
        Cnt = dlg.m_arLibrary.GetSize () ;
		m_SelectLibCnt = Cnt ;
        for(INT i=0; i<Cnt; i++)
		{   
			pCombo->AddString (dlg.m_arLibrary.GetAt (i));
			m_arSelectLib.Add (dlg.m_arLibrary.GetAt (i));
			m_select_Lib_Code.Add (dlg.m_Lib_Code.GetAt (i));
			m_select_Lib_Name.Add (dlg.m_Lib_Name.GetAt (i));
		}
		pCombo->SetCurSel (0) ;

		CEdit* sEdit = (CEdit*)GetDlgItem(IDC_PROS) ;
		
		CString msg ;
		msg.Format (_T("�� %d ����"),Cnt-1) ; 
		sEdit->SetWindowText (msg) ;
	}

}

INT CECO_NOTICE_MANAGER::AllInputLibraryInfo(CString pLibraryCode, CString pLibraryName, CString pParentRecKey)
{
//	CESL_DataMgr  * m_pDataMgr;
	CString         RecKey;
	//INT             ids;

    CString AttrData [4];
    CString AttrName [4] = {  _T("REC_KEY")    ,
		                     _T("PARENT_KEY") ,
							 _T("LIBID")      ,
	                         _T("LIBNAME")    };
	INT    FieldType[4] = { 2, 2, 1, 1 };
  
	CRS_SQL m_rs_sql(this);
    CString sqlBuffer ;
	CString temp ;
	sqlBuffer = _T("select max(rec_key) from ECO_NOTICE_RECV_LIBRARY_TBL");
	m_rs_sql.ES_GetOneFieldValue (sqlBuffer,temp) ;

	INT MaxKey ;
	MaxKey = _ttoi(temp.GetBuffer (0));

	RecKey.Format (_T("%d"),MaxKey+1);
/*  
	ids = MakePrimaryKey( RecKey );
	if ( ids ) return -1;
*/
	AttrData[0] = RecKey;
	AttrData[1] = pParentRecKey;
	AttrData[2] = pLibraryCode;
	AttrData[3] = pLibraryName;

	m_rs_sql.ES_InsertRow (_T("ECO_NOTICE_RECV_LIBRARY_TBL"),4,AttrName,AttrData,FieldType ,TRUE);


	return 0;
}

VOID CECO_NOTICE_MANAGER::OnbINCLUDEFILE() 
{
	FILE    * fp;
	CString   strPathName;
	TCHAR      FileName[256];
	TCHAR      currentpath[256], filesize[256];
	long      position;
	INT       numread, filelen;	

	_tgetcwd( currentpath, MAX_PATH );

	/////////////////////////////////////////////////////////////////////////////////////
	// 1. ���� Dialog�� ����.
	/////////////////////////////////////////////////////////////////////////////////////
	CFileDialog dlg( TRUE, _T(""), _T("*.*"),
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("�������(*.*)|*.*|") );
	dlg.m_ofn.lpstrTitle = _T("÷�� ���� ����");
	dlg.m_ofn.lStructSize;

	if(IDOK == dlg.DoModal () )
	{
		
		m_FILE_INFO.FileCnt = m_FILE_INFO.FileCnt+1;
		strPathName = dlg.GetPathName();

		/////////////////////////////////////////////////////////////////////////////////////
		// 2. ������ open�Ѵ�.
		/////////////////////////////////////////////////////////////////////////////////////
		fp = _tfopen( strPathName, _T("rb") );
		_tchdir( currentpath );

		/////////////////////////////////////////////////////////////////////////////////////
		// 3. Open�� ������ ũ�⸦ ���Ѵ�.
		/////////////////////////////////////////////////////////////////////////////////////
		fseek( fp, 0, SEEK_END);
		position = ftell( fp );
		fseek( fp, 0, SEEK_SET);

		incfile = (TCHAR*)malloc( position );
		numread = fread( incfile, sizeof( TCHAR ), position, fp );
		_stprintf( filesize, _T("%d"), numread );

		/////////////////////////////////////////////////////////////////////////////////////
		// 4. ����ü�� �����̸��� ����� �����Ѵ�.
		/////////////////////////////////////////////////////////////////////////////////////
		_tcscpy( m_FILE_INFO.FilePathBuf[m_FILE_INFO.FileCnt-1], strPathName.GetBuffer(0) );
		_tcscpy( m_FILE_INFO.FileNameBuf[m_FILE_INFO.FileCnt-1], dlg.GetFileName() );
		_tcscpy( m_FILE_INFO.filesize   [m_FILE_INFO.FileCnt-1], filesize );

		/////////////////////////////////////////////////////////////////////////////////////
		// 5. ���� �������� KB������ ����Ѵ�.
		/////////////////////////////////////////////////////////////////////////////////////
		filelen = _tcsclen( filesize );
		if ( filelen < 4 ) {
			_stprintf( FileName, _T("%s(1KB)"), dlg.GetFileName() );
		} else if ( filelen == 4 ) {
			_stprintf( FileName, _T("%s(%cKB)"), dlg.GetFileName(), filesize[0] );
		} else if ( filelen == 5 ) {
			_stprintf( FileName, _T("%s(%c%cKB)"), dlg.GetFileName(), filesize[0], filesize[1] );
		} else if ( filelen == 6 ) {
			_stprintf( FileName, _T("%s(%c%c%cKB)"), dlg.GetFileName(), filesize[0], filesize[1], filesize[2] );
		} else if ( filelen == 7 ) {
			_stprintf( FileName, _T("%s(%c.%c%c%cKB)"), dlg.GetFileName(), filesize[0], filesize[1], filesize[2], filesize[3] );
		} else if ( filelen == 8 ) {
			_stprintf( FileName, _T("%s(%c%c.%c%c%cKB)"), dlg.GetFileName(), filesize[0], filesize[1], filesize[2], filesize[3], filesize[4] );
		} else if ( filelen == 9 ) {
			_stprintf( FileName, _T("%s(%c%c%c.%c%c%cKB)"), dlg.GetFileName(), filesize[0], filesize[1], filesize[2], filesize[3], filesize[4], filesize[5] );
		}

		/////////////////////////////////////////////////////////////////////////////////////
		// 6. ����Ʈ ��Ʈ�ѿ� �߰��Ѵ�.
		/////////////////////////////////////////////////////////////////////////////////////
		m_List.InsertColumn (0,_T(""));
		m_List.InsertItem (0,FileName) ;

		fclose( fp );
	}

}


INT CECO_NOTICE_MANAGER::InsertFile_Proc( CESL_DataMgr * pDataMgr, CString pLocalFileRecKey, CString pParentRecKey ,CString ServerType ) 
{
	INT  i, ids;
    

    if(ServerType == _T("ES"))
    {
        CRS_SQL m_rs_sql (this) ;
		CString sqlBuf ;
		CString tmp_Value ;
        INT tmpReckey ;
		
		sqlBuf = _T("select max(rec_key) from ECO_NOTICE_FILE_TBL ") ;
	     
		CString Field[5] = {_T("REC_KEY"),_T("FILENAME"),_T("FILE_SIZE"),_T("PARENT_KEY") } ;
		CString FieldData[4] ;
		INT     FieldType[4] = { 2,1,2,2} ;

		for ( i = 0; i < m_FILE_INFO.FileCnt; i++ ) {

			m_rs_sql.ES_GetOneFieldValue (sqlBuf,tmp_Value);
			tmpReckey = _ttoi(tmp_Value) ;
            pLocalFileRecKey.Format (_T("%d"),tmpReckey+1) ;
          
            FieldData[0] = pLocalFileRecKey;
            FieldData[1] = m_FILE_INFO.FileNameBuf[i] ;
			FieldData[2] = m_FILE_INFO.filesize[i] ;
			FieldData[3] = pParentRecKey ;


			m_rs_sql.ES_InsertRow (_T("ECO_NOTICE_FILE_TBL"),4,Field,FieldData,FieldType,TRUE) ;
            

			ids = RS_InsertFile( 
							_T("ECO_NOTICE_FILE_TBL")  ,
							 pLocalFileRecKey.GetBuffer (0) ,
							 m_FILE_INFO.FileNameBuf[i], 
							 m_FILE_INFO.FilePathBuf[i],    
							 4
							 ) ;
			if ( ids<0 ) {				
				return ids;
			}
	
		}

	}
/*	else if(ServerType == _T("LS"))
	{
        CString tmpName ;
		CString tmpSize ;

		for ( i = 0; i < m_FILE_INFO.FileCnt; i++ ) {
		    tmpName.Format (_T("%s"),m_FILE_INFO.FileNameBuf[i]);
			tmpSize.Format (_T("%d"),m_FILE_INFO.filesize[i]) ;


			pDataMgr->MakeRecKey(pLocalFileRecKey) ;
		
		    pDataMgr->StartFrame () ;
			pDataMgr->InitDBFieldData () ;
			pDataMgr->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),pLocalFileRecKey);
			pDataMgr->AddDBFieldData (_T("FILENAME"),_T("STRING"),tmpName) ;
            pDataMgr->AddDBFieldData (_T("FILE_SIZE"),_T("NUMERIC"),tmpSize) ;
            pDataMgr->AddDBFieldData (_T("PARENT_KEY"),_T("NUMERIC"),pParentRecKey) ;

		    pDataMgr->MakeInsertFrame (_T("ECO_NOTICE_FILE_TBL"));
			pDataMgr->SendFrame ();
			pDataMgr->EndFrame () ;

			
			ids = RS_InsertFile( 
							_T("ECO_NOTICE_FILE_TBL")  ,
							 pLocalFileRecKey.GetBuffer (0) ,
							 m_FILE_INFO.FileNameBuf[i], 
							 m_FILE_INFO.FilePathBuf[i],    
							 0
							 ) ;
			if ( ids<0 ) {				
				return ids;
			}
		}
	}
*/
	return 0;
}
VOID CECO_NOTICE_MANAGER::OnbAPPEND() 
{
	CESL_DataMgr * m_pDataMgr;
	CESL_ControlMgr * m_pControlMgr;
	CString           LibraryCode, LibraryName, MsqBuf, ParentRecKey;
	CString           LocalLibraryCode, LocalLibraryName, LocalFileRecKey;
	CString           tmpData, CenterParentRecKey, Subject, Contents;
	INT            ids, RecCnt, i;

	/////////////////////////////////////////////////////////////////////////////////////
	// 1. �Է��׸��� üũ�Ѵ�.
	/////////////////////////////////////////////////////////////////////////////////////
	
	GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("����"),Subject,-1) ;
	GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("����"),Contents,-1) ;


	if ( Subject.IsEmpty () || Contents.IsEmpty () ) {
		MessageBox( _T("����Ÿ�� �Է��Ͻʽÿ�."), _T("�Է��׸�üũ"), MB_OK );
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// 1. ��ü������ ������ ���Ѵ�.
	////////////////////////////////////////////////////////////////////////////////////
	
	m_pDataMgr = FindDM(_T("DM_ECO_NOTICE_LIB_CODE")) ;
	if(m_pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}
	
	m_SERVER_TYPE = _T("ES") ;

    CRS_SQL m_rs_sql(this) ;
	
	CString sWhere ;
	sWhere = _T("LIB_CODE IS NOT NULL");
	m_rs_sql.ES_Select (m_pDataMgr,sWhere,_T("DM_ECO_NOTICE_LIB_CODE"),100,3) ;

    RecCnt = m_pDataMgr->GetRowCount () ;

	if ( RecCnt == 0 ) {
		MessageBox( _T("������������ �����ϴ�."), _T("��������ü������"), MB_OK );
		return;
	}
   
    m_SERVER_TYPE = _T("LS") ;

	m_rs_sql.GetLibraryInfo (LocalLibraryCode ) ;
	CString tmp_Value;
    tmp_Value.Format (_T("select lib_name from lib_info_tbl where LIB_CODE = '%s'"),LocalLibraryCode ) ;
	m_rs_sql.ES_GetOneFieldValue (tmp_Value,LocalLibraryName ) ;



	MsqBuf.Format ( _T("��ü %d�� �������� �� ���������� �����ðڽ��ϱ�?"), RecCnt );
	
	ids = MessageBox( MsqBuf, _T("��������ü������"), MB_OKCANCEL|MB_ICONQUESTION );
	if ( IDCANCEL == ids ) return;

    m_pControlMgr = FindCM(_T("CM_ECO_NOTICE_INPUT")) ;
	if(m_pControlMgr == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return ;
	}
    
    
    MsqBuf.Format(_T("��ü %d�� ������"), RecCnt );

    CComboBox * pCombo = (CComboBox*)GetDlgItem(IDC_eRECEIPT_LIB) ;
    pCombo->AddString (MsqBuf) ;

	GetDlgItem(IDC_PROS)->SetWindowText( _T("") );

	/////////////////////////////////////////////////////////////////////////////////////////
	// 2. Center�������׿� ������ Ű���� ���Ѵ�.
	/////////////////////////////////////////////////////////////////////////////////////////
    m_SERVER_TYPE = _T("ES") ;
     

	/*if(m_SERVER_TYPE == _T("LS"))
	{
		CString RecKey ;
		m_pDataMgr->MakeRecKey (RecKey) ;
		ParentRecKey = RecKey ;
	}
    else*/
	if (m_SERVER_TYPE == _T("ES"))
	{
		CRS_SQL m_rs_sql(this) ;
        CString sqlBuffer;
		CString temp ;
		sqlBuffer.Format (_T("select max(rec_key) from ECO_NOTICE_tbl")) ;
		m_rs_sql.ES_GetOneFieldValue (sqlBuffer,temp);
		
        INT itmp ;
		CString RecKey ;
		itmp = _ttoi(temp.GetBuffer (0)) ;
		RecKey.Format (_T("%d"),itmp+1) ;

		ParentRecKey = RecKey ;
	}
      

	InitProgress (RecCnt) ;

	////////////////////////////////////////////////////////////////////////////////////
	// 3. Center�� ���������� Server�� ������ �����Ѵ�.
	////////////////////////////////////////////////////////////////////////////////////
	ids = InsertFile_Proc( m_pDataMgr, LocalFileRecKey, ParentRecKey ,m_SERVER_TYPE );
	if ( ids<0 )
	{
		m_pDataMgr->EndFrame();
		AfxMessageBox ( _T("InsertFile_Proc"), MB_OK);
		return;
	}

    CenterParentRecKey =  ParentRecKey ;

	////////////////////////////////////////////////////////////////////////////////////
	// 4. Local�� ���������� �����Ѵ�.
	////////////////////////////////////////////////////////////////////////////////////
	/*m_SERVER_TYPE = _T("LS") ;


    m_pDataMgr->StartFrame();
	m_pDataMgr->InitDBFieldData () ;
    m_pDataMgr->MakeRecKey (ParentRecKey) ;

	ids = AllInput( ParentRecKey, LocalLibraryCode, LocalLibraryName );
	if ( ids<0 ) {
		m_pDataMgr->EndFrame();
		AfxMessageBox ( _T("�Է��� �����߽��ϴ�."), MB_OK);
		return;
	}

	ids = m_pDataMgr->SendFrame(  );
	ids = m_pDataMgr->EndFrame();
	


	////////////////////////////////////////////////////////////////////////////////////
	// 5. local�� �����������Ѵ�.
	////////////////////////////////////////////////////////////////////////////////////
	m_SERVER_TYPE = _T("LS");
	ids = InsertFile_Proc( m_pDataMgr, LocalFileRecKey, ParentRecKey ,m_SERVER_TYPE);
	if ( ids<0 ) {
		AfxMessageBox( _T("LocalInsertFile_Proc() ERROR"), MB_OK);
		return;
	}
*/
	/////////////////////////////////////////////////////////////////////////////////////////
	// 6. ���������� ������ Center�� ����
	/////////////////////////////////////////////////////////////////////////////////////////
	m_SERVER_TYPE = _T("ES") ;


	for ( i = 0; i < RecCnt; i++ ) {
	     
		 ChangeProgressCtrl(i) ;

		 GetDataMgrData (_T("DM_ECO_NOTICE_LIB_CODE"),_T("LIB_CODE"),LibraryCode ,i) ;
         GetDataMgrData (_T("DM_ECO_NOTICE_LIB_CODE"),_T("LIB_NAME"),LibraryName  ,i) ;

	   	ids = AllInputLibraryInfo( LibraryCode, LibraryName, CenterParentRecKey );
		if ( ids<0 ) {
			if ( ids == 100 ) return;
			AfxMessageBox ( _T("�Է��� �����߽��ϴ�."), MB_OK);
			return;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// ���������� Center�� ���������� �����Ѵ�.
	/////////////////////////////////////////////////////////////////////////////////////////

	ids = AllInput( CenterParentRecKey, LocalLibraryCode, LocalLibraryName );
	

	

	MsqBuf.Format ( _T("��ü %d�� �������� ���������� ���½��ϴ�."), RecCnt );
	MessageBox( MsqBuf, _T("��������ü������"), MB_OK );

    CESL_ControlMgr *pCM = FindCM(_T("CM_ECO_NOTICE_INPUT"));
	if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!"));
		return ;
	}

	pCM->AllControlClear();
//	EndDialog(1);
}


INT CECO_NOTICE_MANAGER::AllInput( CString pParentRecKey, CString pLocalLibraryCode, CString pLocalLibraryName )
{
	CESL_DataMgr  * m_pDataMgr;
	
	CString         Subject, Contents;
	CString         CurrentDate;
	INT             RowCnt;

    CString AttrData [10];
    CString AttrName [10] = { _T("REC_KEY")         , // ���ڵ�KEY
		                    _T("send_libid")      , // ���� ������ �ڵ�
							_T("send_libname")    , // ���� ������ ��
							_T("recv_libid")      , // �޴� ������ �ڵ�
							_T("recv_libname")    , // �޴� ������ ��
							_T("title")           , // ����
							_T("contents")        , // ����
							_T("confirm")         , // Ȯ��
							_T("srflag")          , // ������������
							_T("trans_date")      };
	CString    FieldType[10] = { _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	/////////////////////////////////////////////////////////////////////////////////////
	// 1. �Է��׸��� üũ�Ѵ�.
	/////////////////////////////////////////////////////////////////////////////////////
	
	GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("����"),Subject,-1) ;
	
	GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("����"),Contents ,-1) ;


	if ( Subject.IsEmpty () || Contents.IsEmpty () ) {
		MessageBox( _T("����Ÿ�� �Է��Ͻʽÿ�."), _T("�Է��׸�üũ"), MB_OK );
		return 100;
	}

	AttrData[0] = pParentRecKey;
	AttrData[1] = pLocalLibraryCode;
	AttrData[2] = pLocalLibraryName;
	AttrData[3] = _T("");
	AttrData[4] = _T("��ü������");
	AttrData[5] = Subject;
	AttrData[6] = Contents;
	AttrData[7] = _T("0");

	/////////////////////////////////////////////////////////////////////////////////////
	// 3. Local �����϶��� ���� ������������ ����
	/////////////////////////////////////////////////////////////////////////////////////
    if ( m_SERVER_TYPE == _T("LS") ) {
		AttrData[8] = _T("1");
	/////////////////////////////////////////////////////////////////////////////////////
	// 4. Center �����϶��� ���� ������������ ����
	/////////////////////////////////////////////////////////////////////////////////////
	} else {
		AttrData[8] = _T("1");
	}
   
	CTime t = CTime::GetCurrentTime ();
	CurrentDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;
	AttrData[9] = CurrentDate;

    m_pDataMgr = FindDM(_T("DM_ECO_NOTICE_INPUT")) ;
	if(m_pDataMgr == NULL) {
		AfxMessageBox (_T("FindDM() Error~!"));
		return - 5 ;
	}

    RowCnt = m_pDataMgr->GetRowCount ();

/*
	/////////////////////////////////////////////////////////////////////////////////////
	// 5. Local DB �����϶��� Memory���� ����
	/////////////////////////////////////////////////////////////////////////////////////
	if ( m_SERVER_TYPE == _T("LS") ) {
	
		for(INT i =0 ; i <10;i++) {
        m_pDataMgr->AddDBFieldData (AttrName[i],FieldType[i],AttrData[i]) ;
        }
		m_pDataMgr->MakeInsertFrame (_T("ECO_NOTICE_TBL"));
	   
	/////////////////////////////////////////////////////////////////////////////////////
	// 6. Center DB �����϶��� DB�� ����
	/////////////////////////////////////////////////////////////////////////////////////
	} 
	else */
	if( m_SERVER_TYPE == _T("ES"))
	{
	   	 CRS_SQL m_rs_sql(this);
		 INT FieldType[10] = { 2,1,1,1,1,1,1,1,1,1};
		 m_rs_sql.ES_InsertRow (_T("ECO_NOTICE_TBL"),10,AttrName,AttrData,FieldType,TRUE) ;
	}

	return 0;
}

INT CECO_NOTICE_MANAGER::InitProgress (INT nRange)
{
	m_ProgressCtrl.SetRange (0,nRange) ;
	m_ProgressCtrl.SetPos (0) ;
    
	pCnt = (CStatic *)GetDlgItem(IDC_sCount) ;
	pCnt->SetWindowText (_T("�������: 0%"));

	return 0 ;
}

INT CECO_NOTICE_MANAGER::ChangeProgressCtrl(INT nRange)
{
	INT nLower, nUpper ;

	m_ProgressCtrl.GetRange (nLower,nUpper) ;
	if(nRange <= 0 ) nRange = 1 ;

	INT nPos = ((nRange*100)/nUpper) ;
	
	m_ProgressCtrl.SetPos (nRange) ;
    
    CString sPos ;
	sPos.Format (_T("������� : %d(%d/%d)") , nPos,nRange,nUpper  );
    
    pCnt = (CStatic *)GetDlgItem(IDC_sCount) ;
	pCnt->SetWindowText (sPos);

	return 0 ;
}

VOID CECO_NOTICE_MANAGER::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	INT nIndex = m_pTabCtrl->GetCurSel();
	Display(nIndex);	
	*pResult = 0;
}

INT CECO_NOTICE_MANAGER::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData, strPriceInfo, strPrice, strCurrencyCode;

	if(strDMFieldAlias == _T("Ȯ��"))
	{
		strTmpData = m_pDM->GetCellData(_T("Ȯ��"), nRow);
		
		if		( strTmpData == _T("0") )
		{
			str = _T("��Ȯ��");
		}
		else 
		{
			str = _T("Ȯ��");
		}
		m_pDM->SetCellData(_T("Ȯ�ο���"), str, nRow);
	}

	return 0;
}

VOID CECO_NOTICE_MANAGER::OnCancel2() 
{
	EndDialog(1);	
}

INT CECO_NOTICE_MANAGER::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DoModal();
}
