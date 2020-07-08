// CONTENTMGR_SE_INDEX_SEARCH.cpp : implementation file
//

#include "stdafx.h"
//#include _T("contentsmgr_se.h")
#include "CONTENTMGR_SE_INDEX_SEARCH.h"
#include "..\..\..\����\K2UP_Common\MakeSearchData.h"
#include "SE_CONTENTS_01.h"
#include "CommonDefine.h"
#include "sbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SE_INDEX_SEARCH dialog


CCONTENTMGR_SE_INDEX_SEARCH::CCONTENTMGR_SE_INDEX_SEARCH(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCONTENTMGR_SE_INDEX_SEARCH::~CCONTENTMGR_SE_INDEX_SEARCH()
{

}

BOOL CCONTENTMGR_SE_INDEX_SEARCH::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CCONTENTMGR_SE_INDEX_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCONTENTMGR_SE_INDEX_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_SE_INDEX_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CCONTENTMGR_SE_INDEX_SEARCH)
	ON_WM_CTLCOLOR()
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SE_INDEX_SEARCH message handlers



BOOL CCONTENTMGR_SE_INDEX_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("SM_CONTENTMGR_SE_INDEX_SEARCH"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
    
	m_CurrentCM = _T("CM_CONTENTMGR_SE_INDEX_SEARCH");

	AllControlDisplay (m_CurrentCM,-1) ;
   

	AddColorControl (IDC_eCON_INDEX,_T(""),_T(""),'E') ;
	AddColorControl (IDC_eABS_INDEX,_T(""),_T(""),'E') ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
VOID CCONTENTMGR_SE_INDEX_SEARCH::OnOK() 
{
	// TODO: Add extra validation here
	
     CMakeSearchData *pMakeData = NULL;
	 pMakeData = new CMakeSearchData(this);
   
	//�̿����ѱ���,�����ξ�,�������ξ�,��������Է�����,����������,���������Է�����,������������

	CString strUseLimit;
	CString strKisaKeyWord;
	CString strMokchaKeyWord;
	CString strKisaInsertDate_F,strKisaInsertDate_T ;
	CString strKisaUpDate_F,strKisaUpDate_T;
    CString strMokInsertDate_F,strMokInsertDate_T ;
	CString strMokUpDate_F,strMokUpDate_T;	
	CString strQuery;
	BOOL	bEqualSearch;
 
    CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
	if(pCM == NULL) 
	{
		AfxMessageBox(_T("FindCM() Error~!"));
		return ;
	}

	pCM->GetControlMgrData (_T("�̿����ѱ���"),strUseLimit ,-1) ;
	pCM->GetControlMgrData (_T("�����ξ�"),strKisaKeyWord ,-1) ;
	pCM->GetControlMgrData (_T("�������ξ�"),strMokchaKeyWord ,-1) ;
	pCM->GetControlMgrData (_T("����Է�����_����"),strKisaInsertDate_F ,-1) ;
	pCM->GetControlMgrData (_T("����Է�����_����"),strKisaInsertDate_T ,-1) ;
	pCM->GetControlMgrData (_T("����������_����"),strKisaUpDate_F ,-1) ;
	pCM->GetControlMgrData (_T("����������_����"),strKisaUpDate_T ,-1) ;
	pCM->GetControlMgrData (_T("�����Է�����_����"),strMokInsertDate_F ,-1) ;	
	pCM->GetControlMgrData (_T("�����Է�����_����"),strMokInsertDate_T ,-1) ;
	pCM->GetControlMgrData (_T("������������_����"),strMokUpDate_F ,-1) ;
	pCM->GetControlMgrData (_T("������������_����"),strMokUpDate_T ,-1) ;


	CString m_TOC_INDEX_DM ;
     
	m_TOC_INDEX_DM = _T("DM_CONTENTMGR_SE_INDEX_TBL") ;

	CESL_DataMgr *pBaseDataMgr ;
	pBaseDataMgr = FindDM(m_TOC_INDEX_DM );
	if(pBaseDataMgr == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}

    
	strQuery = _T("");
	if(!strUseLimit.IsEmpty ())
	{
		strQuery += _T("I.USE_LIMIT_CODE = '") + strUseLimit + _T("'") ;
	}


	strKisaKeyWord.TrimLeft () ;
	strKisaKeyWord.TrimRight () ;
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //�����ξ� �˻� ���� ����� 
    //////////////////////////////////////////////////////////////////////////////////////////
	CString sWhereTitle ;
	if(!strKisaKeyWord.IsEmpty ())
	{    	
		INT iCh = pMakeData->ChangeHanjaToHangul( strKisaKeyWord );	//���� �ѱ� ��ȯ
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strKisaKeyWord ) error") );
			return ;
		}				

		if ( strKisaKeyWord.Left(1) == _T("\"") && strKisaKeyWord.Right(1) == _T("\"") ) bEqualSearch = TRUE;
		else bEqualSearch = FALSE;

		iCh = pMakeData->GetKeyWord( strKisaKeyWord );				//����, Ư������ ����
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strKisaKeyWord ) error") );
			return ;
		}

		//sWhereTitle.Format( _T("CATSEARCH(IDX_ARTICLE_KEYWORD,'*%s*',NULL)>0") , strKisaKeyWord );
				
		// 2005.12.12  ADD BY PDJ
		// ������ �˻�
		if ( bEqualSearch == FALSE )
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(IDX_ARTICLE_KEYWORD,'*%s*',NULL)>0") , strKisaKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(i.REC_KEY, SETOC_IDX_ARTICLE_KEYWORD, '%s*')"), strKisaKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
		}
		else
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(IDX_ARTICLE_KEYWORD,'%s',NULL)>0") , strKisaKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(i.REC_KEY, SETOC_IDX_ARTICLE_KEYWORD, '%s')"), strKisaKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
		}

		if(strQuery.IsEmpty ())
		{    
			strQuery = strQuery + _T(" ") + sWhereTitle ;
		}
		else
		{
            strQuery = strQuery + _T(" AND ") + sWhereTitle ;		
		}
	}
     
	//////////////////////////////////////////////////////////////////////////////////////////
	//�������ξ� �˻� ���� ����� 
	//////////////////////////////////////////////////////////////////////////////////////////
    CString sWhereTitle_2 ;
	if(!strMokchaKeyWord.IsEmpty ())
	{    
	
		INT iCh = pMakeData->ChangeHanjaToHangul( strMokchaKeyWord );	//���� �ѱ� ��ȯ
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strMokchaKeyWord ) error") );
			return ;
		}					
		
		if ( strMokchaKeyWord.Left(1) == _T("\"") && strMokchaKeyWord.Right(1) == _T("\"") ) bEqualSearch = TRUE;
		else bEqualSearch = FALSE;

		iCh = pMakeData->GetKeyWord( strMokchaKeyWord );				//����, Ư������ ����
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strMokchaKeyWord ) error") );
			return ;
		}

		// 2005.12.12  ADD BY PDJ
		// ������ �˻�
		if ( bEqualSearch == FALSE )
		{				
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				//sWhereTitle_2.Format( _T("CATSEARCH(IDX_ABS_TOC_KEYWORD,'*%s*',NULL)>0") , strMokchaKeyWord );
				//2004-05-06 ������ ���� 
				sWhereTitle_2.Format( _T("CATSEARCH(IDX_ABS_KEYWORD,'*%s*',NULL)>0") , strMokchaKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle_2.Format( _T("&ECOSEARCH(i.REC_KEY, ECO$V_SETOCIDXTOCKEYWORD$I, '%s*')"), strMokchaKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
		}
		else
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle_2.Format( _T("CATSEARCH(IDX_ABS_KEYWORD,'%s',NULL)>0") , strMokchaKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle_2.Format( _T("&ECOSEARCH(i.REC_KEY, ECO$V_SETOCIDXTOCKEYWORD$I, '%s')"), strMokchaKeyWord );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
		}

		if(strQuery.IsEmpty ())
		{    
			strQuery = strQuery + _T(" ") + sWhereTitle_2 ;
		}
		else
		{
            strQuery = strQuery + _T(" AND ") + sWhereTitle_2 ;
			//strQuery += _T(" AND IDX_TOC_KEYWORD = '")+strContents+ _T("'") ; 
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////
	//��������Է����� �˻� ���� ����� 
	//////////////////////////////////////////////////////////////////////////////////////////
   
	if(!strKisaInsertDate_F.IsEmpty ())
	{				
		if(strKisaInsertDate_T .IsEmpty ())
		{
			if ( !strQuery.IsEmpty() ) strQuery += _T(" AND");
			strQuery+= _T(" A.CREATE_DATE >= to_date('")+strKisaInsertDate_F +_T("','yyyy/mm/dd')") ;
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (A.CREATE_DATE between to_date( '")+strKisaInsertDate_F +_T("', 'yyyy/mm/dd' ) and to_date( '")+strKisaInsertDate_T +_T("', 'yyyy/mm/dd' ) )");
			}
			else
			{
	            strQuery +=_T(" (A.CREATE_DATE between to_date( '")+strKisaInsertDate_F +_T("', 'yyyy/mm/dd' ) and to_date( '")+strKisaInsertDate_T +_T("', 'yyyy/mm/dd' ) )");
			}
		
		}
	}
   
	//////////////////////////////////////////////////////////////////////////////////////////
	//���������� �˻� ���� ����� 
	//////////////////////////////////////////////////////////////////////////////////////////

    if(!strKisaUpDate_F.IsEmpty ())
	{

		if(strKisaUpDate_T .IsEmpty ())
		{
			if ( !strQuery.IsEmpty() ) strQuery += _T(" AND");
			strQuery+= _T(" A.UPDATE_DATE >= to_date('")+strKisaUpDate_F +_T("','yyyy/mm/dd')") ;
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (A.UPDATE_DATE between to_date( '")+strKisaUpDate_F +_T("', 'yyyy/mm/dd' ) and to_date( '")+strKisaUpDate_T +_T("', 'yyyy/mm/dd' ) )");
			}
			else
			{
	            strQuery +=_T(" (A.UPDATE_DATE between to_date( '")+strKisaUpDate_F +_T("', 'yyyy/mm/dd' ) and to_date( '")+strKisaUpDate_T +_T("', 'yyyy/mm/dd' ) )");
			}
		
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	//���������Է����� �˻� ���� ����� 
	//////////////////////////////////////////////////////////////////////////////////////////
   
	if(!strMokInsertDate_F.IsEmpty ())
	{				
		if(strMokInsertDate_T .IsEmpty ())
		{
			if ( !strQuery.IsEmpty() ) strQuery += _T(" AND");
			strQuery+= _T(" A.CREATE_DATE >= to_date('")+strKisaInsertDate_F +_T("','yyyy/mm/dd')") ;
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (A.CREATE_DATE between to_date( '")+strMokInsertDate_F +_T("', 'yyyy/mm/dd' ) and to_date( '")+strMokInsertDate_T +_T("', 'yyyy/mm/dd' ) )");
			}
			else
			{
	            strQuery +=_T(" (A.CREATE_DATE between to_date( '")+strMokInsertDate_F +_T("', 'yyyy/mm/dd' ) and to_date( '")+strMokInsertDate_T +_T("', 'yyyy/mm/dd' ) )");
			}
		
		}
	}
   
	//////////////////////////////////////////////////////////////////////////////////////////
	//������������ �˻� ���� ����� 
	//////////////////////////////////////////////////////////////////////////////////////////

    if(!strMokUpDate_F.IsEmpty ())
	{

		if(strMokUpDate_T .IsEmpty ())
		{
			if ( !strQuery.IsEmpty() ) strQuery += _T(" AND");
			strQuery+= _T(" A.UPDATE_DATE >= to_date('")+strMokUpDate_F +_T("','yyyy/mm/dd')") ;
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (A.UPDATE_DATE between to_date( '")+strMokUpDate_F +_T("', 'yyyy/mm/dd' ) and to_date( '")+strMokUpDate_T +_T("', 'yyyy/mm/dd' ) )");
			}
			else
			{
	            strQuery +=_T(" (A.UPDATE_DATE between to_date( '")+strMokUpDate_F +_T("', 'yyyy/mm/dd' ) and to_date( '")+strMokUpDate_T +_T("', 'yyyy/mm/dd' ) )");
			}
		
		}
	}

  



	// Table �� Index ���� �Ǹ� ���� �Ұ�......
  //  strQuery.Format (_T(" ARTICLE_INFO = '%s'"),strKisaKeyWord) ;

	BEGIN_SBL();
	pBaseDataMgr->RefreshDataManager (strQuery) ;
    INT nTotalCnt = pBaseDataMgr->GetRowCount () ;
	END_SBL(nTotalCnt);	
	if(nTotalCnt == 0)
	{
		AfxMessageBox(_T("�˻� ����� �����ϴ�."));
	}

	if( nTotalCnt > 2000 ) 
	{
		CString strMsg ;
		strMsg.Format (_T("�˻� ����� �ʹ� �����ϴ�. ( �� %d �� )"),nTotalCnt);
		AfxMessageBox(strMsg) ;
		return ;
	}
    

	CESL_ControlMgr *pMainCM = FindCM(_T("CM_CONTENTMGR_SE_MAIN"));
	if(pMainCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return ;
	}
    

	((CSE_CONTENTS_01*)pParentMgr)->m_TabCtrl.SetCurFocus(1);
	((CSE_CONTENTS_01*)pParentMgr)->m_TabCtrl.SetCurSel (1);

	CESL_Grid *pGrid = (CESL_Grid*)pMainCM->FindControl (_T("MainGrid_Kisa")) ;
	pGrid->Display () ;


	CString str ;//= _T("���⿡ �̷��� �޽����� ������ �� �ֽ��ϴ�.");
	str.Format (_T("%d �� �˻� �Ǿ����ϴ�."),nTotalCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));

    delete pMakeData ;
//	CDialog::OnOK();
}

BOOL CCONTENTMGR_SE_INDEX_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN ) 
	{
		OnOK() ;
		return TRUE ;
	}
    
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CCONTENTMGR_SE_INDEX_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}