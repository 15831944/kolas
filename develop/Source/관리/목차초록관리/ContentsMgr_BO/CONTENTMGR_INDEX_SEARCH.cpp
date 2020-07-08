// CONTENTMGR_INDEX_SEARCH.cpp : implementation file
//

#include "stdafx.h"
//#include _T("contentsmgr_bo.h")
#include "CONTENTMGR_INDEX_SEARCH.h"
#include "..\..\..\����\K2UP_Common\MakeSearchData.h"
#include "CommonDefine.h"
#include "sbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_INDEX_SEARCH dialog


CCONTENTMGR_INDEX_SEARCH::CCONTENTMGR_INDEX_SEARCH(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCONTENTMGR_INDEX_SEARCH::~CCONTENTMGR_INDEX_SEARCH()
{
	
}
BOOL CCONTENTMGR_INDEX_SEARCH::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}



VOID CCONTENTMGR_INDEX_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCONTENTMGR_INDEX_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_INDEX_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CCONTENTMGR_INDEX_SEARCH)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_INDEX_SEARCH message handlers

BOOL CCONTENTMGR_INDEX_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("SM_CONTENTMGR_INDEX_SEARCH"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}
    
	m_CurrentCM = _T("CM_CONTENTMGR_INDEX_SEARCH") ;
    m_ParentCM  = _T("CM_CONTENTMGR_MAIN") ;
	AllControlDisplay (m_CurrentCM,-1) ;

	AddColorControl(IDC_eCON_INDEX,_T(""),_T(""),'E');
    AddColorControl(IDC_eABS_INDEX,_T(""),_T(""),'E'); 
	
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CCONTENTMGR_INDEX_SEARCH::OnOK() 
{
	// TODO: Add extra validation here
	CMakeSearchData *pMakeData = NULL;
	 pMakeData = new CMakeSearchData(this);


	CString strUseLimit;
	CString strContents;
	CString strAbstracts;
	CString strConCDateF,strConCDateT;
	CString strConUDateF,strConUDateT;
	CString strAbsCDateF,strAbsCDateT;
	CString strAbsUDateF,strAbsUDateT;
	CString strQuery;
	BOOL	bEqualSearch;
	 
    CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
	if(pCM == NULL) 
	{
		AfxMessageBox(_T("FindCM() Error~!"));
		return ;
	}

	pCM->GetControlMgrData (_T("����_�̿����ѱ���"),strUseLimit ,-1) ;
	pCM->GetControlMgrData (_T("�������ξ�"),strContents ,-1) ;
	pCM->GetControlMgrData (_T("�ʷϻ��ξ�"),strAbstracts ,-1) ;
	pCM->GetControlMgrData (_T("�����Է½���"),strConCDateF ,-1) ;
	pCM->GetControlMgrData (_T("�����Է³�"),strConCDateT ,-1) ;
	pCM->GetControlMgrData (_T("������������"),strConUDateF ,-1) ;
	pCM->GetControlMgrData (_T("����������"),strConUDateT ,-1) ;
	pCM->GetControlMgrData (_T("�ʷ��Է½���"),strAbsCDateF ,-1) ;	
	pCM->GetControlMgrData (_T("�ʷ��Է³�"),strAbsCDateT ,-1) ;
	pCM->GetControlMgrData (_T("�ʷϼ�������"),strAbsUDateF ,-1) ;
	pCM->GetControlMgrData (_T("�ʷϼ�����"),strAbsUDateT ,-1) ;


	
	CString pszFields;
	CString pszCollection;


	INT nTotalCnt;
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
		strQuery += _T("TOC_USE_LIMIT_CODE = '") + strUseLimit + _T("'") ;
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
		
		if ( strContents.Left(1) == _T("\"") && strContents.Right(1) == _T("\"") ) bEqualSearch = TRUE;
		else bEqualSearch = FALSE;		

		iCh = pMakeData->GetKeyWord( strContents );				//����, Ư������ ����
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strContents ) error") );
			return ;
		}

		// 2005.12.12  ADD BY PDJ
		// ������ �˻�
		if ( bEqualSearch == FALSE )
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(IDX_TOC_KEYWORD,'*%s*',NULL)>0") , strContents );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(REC_KEY, BOTOC_IDX_TOC_KEYWORD, '%s*')"), strContents );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) 
			{
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
		}
		else 
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(IDX_TOC_KEYWORD,'%s',NULL)>0") , strContents );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(REC_KEY, BOTOC_IDX_TOC_KEYWORD, '%s')"), strContents );				
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) 
			{
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
			//strQuery += _T(" AND IDX_TOC_KEYWORD = '")+strContents+ _T("'") ; 
		}
	}

    if(!strAbstracts.IsEmpty ())
	{    
	
		INT iCh = pMakeData->ChangeHanjaToHangul( strAbstracts );	//���� �ѱ� ��ȯ
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strAbstracts ) error") );
			return ;
		}			
		if ( strAbstracts.Left(1) == _T("\"") && strAbstracts.Right(1) == _T("\"") ) bEqualSearch = TRUE;
		else bEqualSearch = FALSE;		

		iCh = pMakeData->GetKeyWord( strAbstracts );				//����, Ư������ ����
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strAbstracts ) error") );
			return ;
		}

		//sWhereTitle.Format( _T("CATSEARCH(IDX_TOC_KEYWORD,'*%s*',NULL)>0") , strContents );
		
		// 2005.12.12  ADD BY PDJ
		// ������ �˻�
		if ( bEqualSearch == FALSE )
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(IDX_ABS_KEYWORD,'*%s*',NULL)>0") , strAbstracts );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(REC_KEY, BOTOC_IDX_ABS_KEYWORD, '%s*')"), strAbstracts );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) 
			{
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
		}
		else 
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(IDX_ABS_KEYWORD,'%s',NULL)>0") , strAbstracts );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(REC_KEY, BOTOC_IDX_ABS_KEYWORD, '%s')"), strAbstracts );				
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) 
			{
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
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

	//===================================================
	//2009.05.20 UPDATE BY PJW : ������ ��¥�鿡���� ó���� ���ش�.
//	if(!strConCDateF.IsEmpty ())
//	{
//		if(strConCDateT .IsEmpty ())
//		{
//			strQuery+= _T(" AND TOC_CREATE_DATE >= to_date('")+strConCDateF +_T("','yyyy/mm/dd')") ;
//		}
//		else
//		{
//			if(!strQuery.IsEmpty ())
//			{
//				strQuery +=_T(" AND (TOC_CREATE_DATE between to_date( '")+strConCDateF +_T("', 'yyyy/mm/dd' ) and to_date( '")+strConCDateT +_T("', 'yyyy/mm/dd' ) )");
//			}
//			else
//			{
//	            strQuery +=_T(" (TOC_CREATE_DATE between to_date( '")+strConCDateF +_T("', 'yyyy/mm/dd' ) and to_date( '")+strConCDateT +_T("', 'yyyy/mm/dd' ) )");
//			}
//		
//		}
//	}
	if(!strConCDateF.IsEmpty ())
	{
		if(strConCDateT .IsEmpty ())
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery+= _T(" AND TOC_CREATE_DATE >= TO_DATE('")+strConCDateF +_T("','YYYY/MM/DD')") ;
			}
			else
			{
				strQuery+= _T(" TOC_CREATE_DATE >= TO_DATE('")+strConCDateF +_T("','YYYY/MM/DD')") ;
			}
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (TOC_CREATE_DATE BETWEEN TO_DATE( '")+strConCDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strConCDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
			else
			{
	            strQuery +=_T(" (TOC_CREATE_DATE BETWEEN TO_DATE( '")+strConCDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strConCDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}		
		}
	}

	if(!strConUDateF.IsEmpty ())
	{
		if(strConUDateT .IsEmpty ())
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery+= _T(" AND TOC_UPDATE_DATE >= TO_DATE('")+strConUDateF +_T("','YYYY/MM/DD')") ;
			}
			else
			{
				strQuery+= _T(" TOC_UPDATE_DATE >= TO_DATE('")+strConUDateF +_T("','YYYY/MM/DD')") ;
			}			
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (TOC_UPDATE_DATE BETWEEN TO_DATE( '")+strConUDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strConUDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
			else
			{
	            strQuery +=_T(" (TOC_UPDATE_DATE BETWEEN TO_DATE( '")+strConUDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strConUDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
		
		}
	}

	if(!strAbsCDateF.IsEmpty ())
	{
		if(strAbsCDateT .IsEmpty ())
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery+= _T(" AND ABS_CREATE_DATE >= TO_DATE('")+strAbsCDateF +_T("','YYYY/MM/DD')") ;
			}
			else
			{
				strQuery+= _T(" ABS_CREATE_DATE >= TO_DATE('")+strAbsCDateF +_T("','YYYY/MM/DD')") ;
			}
			
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (ABS_CREATE_DATE BETWEEN TO_DATE( '")+strAbsCDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strAbsCDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
			else
			{
	            strQuery +=_T(" (ABS_CREATE_DATE BETWEEN TO_DATE( '")+strAbsCDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strAbsCDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
		
		}
	}

	if(!strAbsUDateF.IsEmpty ())
	{
		if(strAbsUDateT .IsEmpty ())
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery+= _T(" AND ABS_UPDATE_DATE >= TO_DATE('")+strAbsUDateF +_T("','YYYY/MM/DD')") ;
			}
			else
			{
				strQuery+= _T(" ABS_UPDATE_DATE >= TO_DATE('")+strAbsUDateF +_T("','YYYY/MM/DD')") ;
			}
			
		}
		else
		{
			if(!strQuery.IsEmpty ())
			{
				strQuery +=_T(" AND (ABS_UPDATE_DATE BETWEEN TO_DATE( '")+strAbsUDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strAbsUDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
			else
			{
	            strQuery +=_T(" (ABS_UPDATE_DATE BETWEEN TO_DATE( '")+strAbsUDateF +_T("', 'YYYY/MM/DD' ) AND TO_DATE( '")+strAbsUDateT +_T("', 'YYYY/MM/DD' )+0.99999 )");
			}
		
		}
	}
	//===================================================


	BEGIN_SBL();
	pBaseDataMgr->RefreshDataManager (strQuery) ;
    nTotalCnt = pBaseDataMgr->GetRowCount () ;
	END_SBL(nTotalCnt);
	
	if( nTotalCnt > 2000 ) 
	{
		CString strMsg ;
		strMsg.Format (_T("�˻� ����� �ʹ� �����ϴ�. ( �� %d �� )"),nTotalCnt);
		AfxMessageBox(strMsg) ;
		return ;
	}
	INT ids ;
    
	if( nTotalCnt > 0 )
	{     
		 CESL_DataMgr *pMainDM = FindDM(_T("DM_BO_CONTENTMGR_SPECIES")) ;
		 if(pMainDM == NULL) {
			 AfxMessageBox(_T("FindDM() Error~!")) ;
			 return ;
		 }
         
         ids = FromIndexToMainDM(pBaseDataMgr, pMainDM) ;
         if(ids <0) 
		 {
			 AfxMessageBox(_T("FromIndexToMainDM() Error~!"));
			 return ;
		 }

 
	}
	else
	{
		AfxMessageBox(_T("�˻��� ������ �����ϴ�."));
		return ;
	}

    AllControlDisplay (m_ParentCM,-1) ;


	CString str ;//= _T("���⿡ �̷��� �޽����� ������ �� �ֽ��ϴ�.");
	str.Format (_T("%d �� �˻� �Ǿ����ϴ�."),nTotalCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));
     
	delete pMakeData ;

	return;
//ERR_RETURN:
//	AfxMessageBox(_T("�˻��� �����Ͽ����ϴ�."));
//	CDialog::OnOK();
}



INT CCONTENTMGR_INDEX_SEARCH::FromIndexToMainDM(CESL_DataMgr *pIndexDM, CESL_DataMgr *pMainDM)
{
	INT nRowCnt ;
    nRowCnt = pIndexDM->GetRowCount () ;
    
	CString sSpecies_Key ;
	CString sTemp ;
    CString sWhere ;
	//===================================================
	//2010.01.06 UPDATE BY PJW : 1000�� �̻��ϰ��� ����� �Ѵ�.
//  for(INT i =0 ; i < nRowCnt ;i++) 
//	{
//	   pIndexDM->GetCellData (_T("SPECIES_KEY"),i,sSpecies_Key ) ;
//	   if ( sSpecies_Key == _T("") ) continue;
//
//	   if(!sTemp.IsEmpty ())
//	   {
//			sTemp+= _T(",") + sSpecies_Key ; 
//
//	   }
//	   else
//	   {
//			sTemp += sSpecies_Key ;
//	   }
//       
//	   sSpecies_Key.Empty () ;
//
//    }
//
//	sWhere = _T("I.REC_KEY IN( ") + sTemp + _T(")") ;

	sWhere = _T("");
	for(INT i =0 ; i < nRowCnt ;i++) 
	{
	   pIndexDM->GetCellData ( _T("SPECIES_KEY"), i, sSpecies_Key ) ;
	   if ( sSpecies_Key == _T("") ) continue;

	   if(!sTemp.IsEmpty ())
	   {
		   if( i%1000 == 999)
		   {
			   if( sWhere.IsEmpty() )
			   {
				   sWhere = _T("I.REC_KEY IN( ") + sTemp + _T(",") + sSpecies_Key + _T(" )");
			   }
			   else
			   {
					sWhere += _T(" OR I.REC_KEY IN( ") + sTemp + _T(",") + sSpecies_Key + _T(" )");
			   }
			   sTemp = _T("");
		   }
		   else
		   {
			   sTemp+= _T(",") + sSpecies_Key ; 
		   }
	   }
	   else
	   {
			sTemp = sSpecies_Key ;
	   }
       
	   sSpecies_Key.Empty () ;
    }

	if ( !sTemp.IsEmpty() )
	{
		if( sWhere.IsEmpty() )
		{
			sWhere = _T("I.REC_KEY IN( ") + sTemp + _T(" )");
		}
		else
		{
			sWhere += _T(" OR I.REC_KEY IN( ") + sTemp + _T(" )");
		}	
	}

	sWhere = _T("( ") + sWhere + _T(" )");
	//===================================================
	

    pMainDM->RefreshDataManager (sWhere) ;
    
	return 0 ;
}
//===================================================
//2009.12.14 ADD BY PJW : 
//	CString strTemp;
// 	CString strRecKey;
// 	CString strBookKeyTempList;
// 	CString strQueryWhere;
// 	INT nCnt;
// 	INT nLocation;
// 	INT nStartLocation;
//	INT i ; 
//	strBookKeyTempList = strRecKeyList;
//	strBookKeyTempList.Replace(_T("'"), _T(""));
//	nCnt = 0;
//	nLocation = 0;
//	strQueryWhere.Format(_T(""));
//	while( 1 )
//	{
//		nLocation = strBookKeyTempList.Find(_T(","), nLocation+1);
//		++nCnt;
//		if( 0 > nLocation )
//		{
//			break;			
//		}
//	}
//	if ( (nCnt % 1000) == 0 )
//	{
//		nCnt = (nCnt/1000);
//	}
//	else
//	{
//		nCnt = (nCnt/1000)+1;
//	}
//	nStartLocation = strBookKeyTempList.GetLength()/nCnt;
//
//	for( i = 0 ; i < nCnt ; i++)
//	{
//		if( i == (nCnt -1) )
//		{
//			strRecKey = strBookKeyTempList;
//		}
//		else
//		{
//			nLocation = strBookKeyTempList.Find(_T(","), nStartLocation);
//			strRecKey = strBookKeyTempList.Mid(0, nLocation );
//			strTemp.Format(_T("%s,"), strRecKey);
//			strBookKeyTempList.Replace( strTemp, _T("") );
//		}
//	
//		if( strQueryWhere.IsEmpty() )
//		{
//			strQueryWhere.Format(_T("%s IN (%s)"), strKeyName, strRecKey);
//		}
//		else
//		{
//			strTemp.Format(_T("%s OR %s IN( %s )"), strQueryWhere,strKeyName, strRecKey );
//			strQueryWhere = strTemp;
//		}
//	}
//	strRecKeyList.Format(_T("SELECT %s FROM %s WHERE %s"), strKeyName, strTableName,strQueryWhere);
//
//	return 0;
//===================================================

BOOL CCONTENTMGR_INDEX_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN ) 
	{
		OnOK() ;
		return TRUE ;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CCONTENTMGR_INDEX_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
