// CONTENTMGR_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "CONTENTMGR_SEARCH.h"
#include "..\..\..\����\K2UP_Common\MakeSearchData.h"
#include "CommonDefine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include _T("..\..\..\SmartID\BarcodeToRFID2\BarcodeToRFIDDlg.h")

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SEARCH dialog


CCONTENTMGR_SEARCH::CCONTENTMGR_SEARCH(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCONTENTMGR_SEARCH::~ CCONTENTMGR_SEARCH()
{	
	
} 

BOOL CCONTENTMGR_SEARCH::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CCONTENTMGR_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCONTENTMGR_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CCONTENTMGR_SEARCH)
	ON_BN_CLICKED(IDC_bSearchToc, OnbSearchToc)
	ON_WM_CTLCOLOR()
//	ON_BN_CLICKED(IDC_bSmartID, OnbSmartID)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SEARCH message handlers



BOOL CCONTENTMGR_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("SM_CONTENTMGR_SEARCH")) <0 )
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}

    m_CurrentCM = _T("CM_CONTENTMGR_SEARCH") ;
    m_ParentDM  = _T("DM_BO_CONTENTMGR_SPECIES") ;
    m_ParentCM  = _T("CM_CONTENTMGR_MAIN") ;

	EnableThemeDialogTexture(GetSafeHwnd());


/*
	CRect WndPos;
	GetWindowRect( WndPos );
	ClientToScreen(WndPos);
	MoveWindow(WndPos.left , WndPos.top , WndPos.Width() , 200);

*/
//	AddColorControl (IDC_eTITLE,_T(""),_T(""),'E') ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
VOID CCONTENTMGR_SEARCH::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CCONTENTMGR_SEARCH::OnOK() 
{   
	CMakeSearchData *pMakeData = NULL;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003.11.11 ������ ���� 
    pMakeData = new CMakeSearchData(this) ;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++ 
	CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
    if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return ;
	}

	CString sTitle ,sAuthor , sPublisher, sPubyear ;
    //TOC_YN_Y (����) ,ABS_YN_Y (�ʷ�) 
	
	CButton *pCheck_TOC = (CButton *)GetDlgItem(IDC_bCONTENTS) ;
	CString sWhere ,strQry ;
	BOOL	bEqualSearch;    

    CESL_DataMgr *pDM = FindDM(m_ParentDM) ;
    if(pDM == NULL) {
		AfxMessageBox (_T("FindDM() Error~!")) ;
		return ;
	}

	INT ids ;
	ids = pCM->GetControlMgrData (_T("����"),sTitle,-1) ;
    ids = pCM->GetControlMgrData (_T("����"),sAuthor,-1) ;
	ids = pCM->GetControlMgrData (_T("������"),sPublisher ,-1) ;
	ids = pCM->GetControlMgrData (_T("�����"),sPubyear ,-1) ;	

	if( sTitle.IsEmpty () && sAuthor.IsEmpty())
	{
		AfxMessageBox(_T("���� �Ǵ� ���ڸ� �Է� �ϼž� �մϴ�.")) ;
		return ;
	}

	if( !sTitle.IsEmpty() )
	{
		sTitle.TrimLeft();
		sTitle.TrimRight();
	}

	if( !sAuthor.IsEmpty() )
	{
		sAuthor.TrimLeft();
		sAuthor.TrimRight();
	}

	sWhere = _T("I.SPECIES_CLASS <> '3' AND I.WORKING_STATUS NOT IN ('BOT212O', 'BOT211O') ");

	if ( sTitle != _T("") ) 
	{
		INT iCh = pMakeData->ChangeHanjaToHangul( sTitle );	//���� �ѱ� ��ȯ
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
			return ;
		}			

		if ( sTitle.Left(1) == _T("\"") && sTitle.Right(1) == _T("\"") ) bEqualSearch = TRUE;
		else bEqualSearch = FALSE;		

		iCh = pMakeData->GetKeyWord( sTitle );				//����, Ư������ ����
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
			return ;
		}
		
		// 2005.12.12  ADD BY PDJ
		// ������ �˻�
		CString sWhereTitle;
		if ( bEqualSearch == FALSE )
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("AND CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("AND &ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
		}
		else
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("AND CATSEARCH(I.IDX_TITLE,'%s',NULL)>0") , sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("AND &ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s')"), sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
		}
		strQry = sWhere + sWhereTitle;
	}
    
//	if(sAuthor != _T("")) 
//	{
//		strQry+= _T(" AND I.AUTHOR = '") + sAuthor + _T("'") ;
//	}

	if ( sAuthor != _T("") )
	{		
		INT iCh = pMakeData->ChangeHanjaToHangul( sAuthor );	//���� �ѱ� ��ȯ
		if ( 0 != iCh ) 
		{			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sAuthor ) error") );
			return;
		}
		iCh = pMakeData->GetKeyWord( sAuthor );				//����, Ư������ ����
		if ( 0 != iCh ) 
		{
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sAuthor ) error") );
			return;
		}
		
		CString sWhereAuthor;

		if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) 
		{
			sWhereAuthor.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
		}
		else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) 
		{
			sWhereAuthor.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
		}
		else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
			AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
			return;
		}

		if( sTitle != _T("") )
		{
			strQry = strQry + _T(" AND ") + sWhereAuthor;	
		}
		else
		{
			strQry = sWhere + _T("AND ") + sWhereAuthor;
		}
	}
    
	if(sPublisher  != _T("")) 
	{
		strQry+= _T(" AND I.PUBLISHER = '") + sPublisher + _T("'") ;
	}
    
    if(sPubyear  != _T("")) 
	{
		strQry+= _T(" AND I.PUB_YEAR = '") + sPubyear + _T("'") ;
	}

   
	AfxGetMainWnd()->SendMessage(WM_USER+3000,0,0);
	pDM->RefreshDataManager (strQry) ;
    
	INT nTotalCnt ;
	nTotalCnt = pDM->GetRowCount () ;
	AfxGetMainWnd()->SendMessage(WM_USER+3001,nTotalCnt);
	if(nTotalCnt == 0)
	{
		AfxMessageBox(_T("�˻� ����� �����ϴ�."));
	}


	AllControlDisplay (m_ParentCM,-1) ;
	CString str ;//= _T("���⿡ �̷��� �޽����� ������ �� �ֽ��ϴ�.");
	str.Format (_T("%d �� �˻� �Ǿ����ϴ�."),nTotalCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));
    
	//CDialog::OnOK();
}

VOID CCONTENTMGR_SEARCH::OnbSearchToc() 
{

    CMakeSearchData *pMakeData = NULL;


	CString strUseLimit;
	CString strContents;
	CString strAbstracts;
	CString strConCDateF,strConCDateT;
	CString strConUDateF,strConUDateT;
	CString strAbsCDateF,strAbsCDateT;
	CString strAbsUDateF,strAbsUDateT;
	CString strQuery;

 
    CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
	if(pCM == NULL) 
	{
		AfxMessageBox(_T("FindCM() Error~!"));
		return ;
	}

	pCM->GetControlMgrData (_T("�̿����ѱ���"),strUseLimit ,-1) ;
	pCM->GetControlMgrData (_T("�������ξ�"),strContents ,-1) ;
	pCM->GetControlMgrData (_T("�ʷϻ��ξ�"),strAbstracts ,-1) ;
	pCM->GetControlMgrData (_T("�����Է�����_����"),strConCDateF ,-1) ;
	pCM->GetControlMgrData (_T("�����Է�����_����"),strConCDateT ,-1) ;
	pCM->GetControlMgrData (_T("������������_����"),strConUDateF ,-1) ;
	pCM->GetControlMgrData (_T("������������_����"),strConUDateT ,-1) ;
	pCM->GetControlMgrData (_T("�ʷ��Է�����_����"),strAbsCDateF ,-1) ;	
	pCM->GetControlMgrData (_T("�ʷ��Է�����_����"),strAbsCDateT ,-1) ;
	pCM->GetControlMgrData (_T("�ʷϼ�������_����"),strAbsUDateF ,-1) ;
	pCM->GetControlMgrData (_T("�ʷϼ�������_����"),strAbsUDateT ,-1) ;


	
	CString pszFields;
	CString pszCollection;

	CESL_DataMgr* pBaseDataMgr ;


	CString m_TOC_INDEX_DM ;
     
	m_TOC_INDEX_DM = _T("DM_CONTENTMGR_INDEX") ;

	pBaseDataMgr = FindDM(m_TOC_INDEX_DM );
	if(pBaseDataMgr == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}

    
	if(!strUseLimit.IsEmpty ())
	{
		strQuery += _T(" USE_LIMIT_CODE = '") + strUseLimit + _T("'") ;
	}

 
	strContents.TrimLeft () ;
	strContents.TrimRight () ;
    
	CString sWhereTitle ;

	if(!strContents.IsEmpty ())
	{    
	
		INT iCh = pMakeData->ChangeHanjaToHangul( strContents );	//���� �ѱ� ��ȯ
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strContents ) error") );
			return ;
		}						
		iCh = pMakeData->GetKeyWord( strContents );				//����, Ư������ ����
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strContents ) error") );
			return ;
		}

		//sWhereTitle.Format( _T("CATSEARCH(IDX_TOC_KEYWORD,'*%s*',NULL)>0") , strContents );
				
		if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
			sWhereTitle.Format( _T("CATSEARCH(IDX_TOC_KEYWORD,'*%s*',NULL)>0") , strContents );
		}
		else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
			sWhereTitle.Format( _T("&ECOSEARCH(REC_KEY, BOTOC_IDX_TOC_KEYWORD, '%s*')"), strContents );
		}
		else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
			AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
			return;
		}
		
		//strQry = strQry + _T(" ") + sWhereTitle;

		if(strQuery.IsEmpty ())
		{    
			strQuery = strQuery + _T(" ") + sWhereTitle ;
		}
		else
		{
            strQuery = strQuery + _T(" AND ") + sWhereTitle ;
			//strQuery += _T(" AND IDX_TOC_KEYWORD = '")+strContents+ _T("'") ; 
		}
	}

    if(!strAbstracts.IsEmpty ())
	{
		if(strQuery.IsEmpty ())
		{
			strQuery += _T(" IDX_ABS_KEYWORD = '")+strAbstracts+ _T("'") ;
		}
		else
		{
			strQuery += _T(" AND IDX_ABS_KEYWORD = '")+strAbstracts+ _T("'") ; 
		}
	}

   // AND (A.CREATE_DAT between to_date( ' 0111/11/11', 'yyyy/mm/dd' ) and to_date( ' 1111/11/11', 'yyyy/mm/dd' ) )
   // AND  A.CREATE_DATE >= to_date  ('1111/11/01' , 'yyyy/mm/dd' )

    if(!strConCDateF.IsEmpty ())
	{
		if(strConCDateT .IsEmpty ())
		{
		strQuery+= _T(" AND TOC_CREATE_DATE >= to_date('")+strConCDateF +_T("','yyyy/mm/dd')") ;
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (TOC_CREATE_DATE between to_date( '")+strConCDateF +_T("', 'yyyy/mm/dd' ) and to_date( '")+strConCDateT +_T("', 'yyyy/mm/dd' ) )");
			}
			else
			{
	            strQuery +=_T(" (TOC_CREATE_DATE between to_date( '")+strConCDateF +_T("', 'yyyy/mm/dd' ) and to_date( '")+strConCDateT +_T("', 'yyyy/mm/dd' ) )");
			}
		
		}
	}



	pBaseDataMgr->RefreshDataManager (strQuery) ;


/*

	// Making Query


	strQuery+=MakeS97Statement(_T("TOC_CREATE_DATE"),strConCDateF.GetBuffer(0),strConCDateT.GetBuffer(0),SST_FIELD,_T("%s and "));
	strQuery+=MakeS97Statement(_T("TOC_UPDATE_DATE"),strConUDateF.GetBuffer(0),strConUDateT.GetBuffer(0),SST_FIELD,_T("%s and "));

	strQuery+=MakeS97Statement(_T("ABS_CREATE_DATE"),strAbsCDateF.GetBuffer(0),strAbsCDateT.GetBuffer(0),SST_FIELD,_T("%s and "));
	strQuery+=MakeS97Statement(_T("ABS_UPDATE_DATE"),strAbsUDateF.GetBuffer(0),strAbsUDateT.GetBuffer(0),SST_FIELD,_T("%s and "));

	if(strQuery.Right(4)==_T("and "))strQuery=strQuery.Left(strQuery.GetLength()-4);
	else {
		AfxMessageBox(_T("�Էµ� ������ �����մϴ�."));
		return;
	}

    // KOLIS �ڷḸ ���δ�.( ���ε� �ǰ� ������ �������� ������)
	strQuery+=_T(" and (LIB_CODE=011001) ");

	// Search
	if(MakeQueryInfo(pBaseDataMgr,pszFields,pszCollection))goto ERR_RETURN;
	nIds=s97_RequestDM(pszFields,1,2000,_T(""),pszCollection,strQuery.GetBuffer(0),&nRowCnt,&nColCnt,&nTotalCnt,&pData,2000,CS);
	if(nIds==TOO_MANY){
		CString strMsg;
		strMsg.Format(_T("�˻� ����� �ʹ� �����ϴ�. ( �� %d �� )"),nTotalCnt);
		AfxMessageBox(strMsg);
		return;
	}
	if(nRowCnt>0){
		pBaseDataMgr->ClearDataMemory();
		pBaseDataMgr->AppendSearchArray(pData,nRowCnt);
		EndDialog(IDOK);
	}
	else{
		AfxMessageBox(_T("�˻� ����� �����ϴ�."));
		return;
	}
*/

	return;
//ERR_RETURN:
//	AfxMessageBox(_T("�˻��� �����Ͽ����ϴ�."));
}



BOOL CCONTENTMGR_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN ) 
	{
		OnOK() ;
		return TRUE ;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CCONTENTMGR_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}



/*
VOID CCONTENTMGR_SEARCH::OnbSmartID() 
{
	// TODO: Add your control notification handler code here
	CBarcodeToRFIDDlg dlg;
	dlg.CONNECTION_INFO = _T("kolas2@kolas2/kolas2#[]");
	CRFIDBookInfo *pBook;

	pBook = dlg.AddBook();
	if (pBook) {
		pBook->strAccessionNo = _T("EM0000030180");
		pBook->strTitle = _T("����1987");
	}
	pBook = dlg.AddBook();
	if (pBook) {
		pBook->strAccessionNo = _T("EM0000030181");
		pBook->strTitle = _T("�𳪸���");
	}
	pBook = dlg.AddBook();
	if (pBook) {
		pBook->strAccessionNo = _T("EM0000030182");
		pBook->strTitle = _T("��");
	}
	pBook = dlg.AddBook();
	if (pBook) {
		pBook->strAccessionNo = _T("EM0000030183");
		pBook->strTitle = _T("���� �׷����� ���ڳ�");
	}
	pBook = dlg.AddBook();
	if (pBook) {
		pBook->strAccessionNo = _T("EM0000030184");
		pBook->strTitle = _T("����ϴ� �״뿡��");
	}

	dlg.DoModal();

}
*/