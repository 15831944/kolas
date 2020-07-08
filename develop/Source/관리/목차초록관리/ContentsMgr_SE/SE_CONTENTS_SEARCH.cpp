// SE_CONTENTS_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CONTENTS_SEARCH.h"
#include "..\..\..\����\K2UP_Common\MakeSearchData.h"
#include "SE_CONTENTS_01.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CONTENTS_SEARCH dialog


CSE_CONTENTS_SEARCH::CSE_CONTENTS_SEARCH(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CONTENTS_SEARCH::~CSE_CONTENTS_SEARCH()
{
	
}


BOOL CSE_CONTENTS_SEARCH::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CSE_CONTENTS_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CONTENTS_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CONTENTS_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CSE_CONTENTS_SEARCH)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CONTENTS_SEARCH message handlers
/*
VOID CSE_CONTENTS_SEARCH::OnbSearchToc() 
{
	// TODO: Add your control notification handler code here
	
}
*/

VOID CSE_CONTENTS_SEARCH::OnOk() 
{
	// TODO: Add extra validation here
    
	CMakeSearchData *pMakeData = NULL;
    pMakeData = new CMakeSearchData(this);

	CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
    if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return ;
	}
	
	CString sTitle,sPublisher,sPubfreq,sPublisdate_Start,sPublisdate_End ,sVol_Title;   
	CString sWhereTitle ,strQry ;
	BOOL	bEqualSearch;

    CESL_DataMgr *pDM = FindDM(m_ParentDM) ;
    if(pDM == NULL) {
		AfxMessageBox (_T("FindDM() Error~!")) ;
		return ;
	}

	INT ids ;    
	ids = pCM->GetControlMgrData (_T("����"),sTitle,-1) ;
	ids = pCM->GetControlMgrData (_T("������"),sPublisher,-1) ;
    ids = pCM->GetControlMgrData (_T("�����"),sPubfreq,-1) ;
	ids = pCM->GetControlMgrData (_T("�����Ͻ���"),sPublisdate_Start,-1) ;
    ids = pCM->GetControlMgrData (_T("�����Ͻ���"),sPublisdate_End,-1) ;
    ids = pCM->GetControlMgrData (_T("��ȣ��"),sVol_Title,-1) ;

	if( sTitle.IsEmpty () && sPublisher.IsEmpty() )
	{
		AfxMessageBox(_T("���� �Ǵ� ������ ������ �Է� �ϼž� �մϴ�.")) ;
		return ;
	}

	sTitle.TrimLeft();
	sTitle.TrimRight();

	if ( sTitle != _T("") ) {
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
		if ( bEqualSearch == FALSE )
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//2003.11.12 ������ ���� 
				//sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_TITLE, '%s*')"), sTitle );
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
		}
		else
		{
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s',NULL)>0") , sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_TITLE, '%s')"), sTitle );			
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return;
			}
		}
		
		//strQry = strQry + _T(" AND ") + sWhereTitle;
		strQry = strQry + _T(" ") + sWhereTitle;
	}
    

	if (sPublisher != _T("") && sTitle != _T(""))
	{
		strQry += _T(" AND I.IDX_IPUBLISHER = '")+sPublisher +_T("'") ;
	}
	else if (sPublisher != _T("") && sTitle == _T(""))
	{
		strQry += _T(" I.IDX_IPUBLISHER = '")+sPublisher +_T("'") ;
	}

 // ����� 
	if(sPubfreq != _T(""))
	{
		strQry += _T(" AND PUB_FREQ = '")+sPubfreq +_T("'") ;
	}

    if(sVol_Title!=_T(""))
	{
       strQry += _T(" AND V.VOL_TITLE = '")+sVol_Title +_T("'") ;
	}

	if(!sPublisdate_Start.IsEmpty ())
	{
		if(sPublisdate_End.IsEmpty ())
		{
		strQry+= _T(" AND V.PUBLISH_DATE >= to_date('")+sPublisdate_Start +_T("','yyyy/mm/dd')") ;
		}
		else
		{
			if(!strQry.IsEmpty ())
			{
				strQry +=_T(" AND (V.PUBLISH_DATE between to_date( '")+sPublisdate_Start +_T("', 'yyyy/mm/dd' ) and to_date( '")+sPublisdate_End +_T("', 'yyyy/mm/dd' ) )");
			}
			else
			{
	            strQry +=_T(" (V.PUBLISH_DATE between to_date( '")+sPublisdate_Start +_T("', 'yyyy/mm/dd' ) and to_date( '")+sPublisdate_End +_T("', 'yyyy/mm/dd' ) )");
			}
		
		}
	}

	// 2006.01.16 ADD BY PDJ
	// ������ȣ�� �������� �ȵ�
	// ����, �ʷ��� ������ȣ������ �ο��Ǵ°��� �ƴ�
	strQry += _T("AND VOL_TYPE='V'");

	//2007.01.15 ��ȣ�ڷ�� �����ϰ� �˻��ǵ��� ���� by HJY
	strQry += _T("AND V.REC_KEY NOT IN( SELECT SB.VOL_KEY from SE_BOOK_TBL SB WHERE SB.VOL_KEY = V.REC_KEY AND SB.BOOK_TYPE = 'M')");
	
	// 2005.12.08 ADD BY PDJ
	// �ֱ� �Լ� ���� ��� 
	// ��ȣ �������ķ� ��ȣ�� ��Ʈ �Ұ�.
	strQry += _T(" ORDER BY I.CONTROL_NO, V.VOL_SORT_NO DESC");
    
	AfxGetMainWnd()->SendMessage(WM_USER+3000,0,0);
	pDM->RefreshDataManager (strQry) ;
    INT temp = pDM->GetRowCount() ;
	AfxGetMainWnd()->SendMessage(WM_USER+3001,temp);
    if(temp == 0)
	{
		AfxMessageBox(_T("�˻� ����� �����ϴ�."));
	}
    ((CSE_CONTENTS_01*)pParentMgr)->m_TabCtrl.SetCurFocus(0);
	((CSE_CONTENTS_01*)pParentMgr)->m_TabCtrl.SetCurSel (0);

	AllControlDisplay (m_ParentCM ,-1) ;
     
	CString str ;//= _T("���⿡ �̷��� �޽����� ������ �� �ֽ��ϴ�.");
	str.Format (_T("%d �� �˻� �Ǿ����ϴ�."),temp);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));

	delete pMakeData;
          
	//CDialog::OnOK();
}

VOID CSE_CONTENTS_SEARCH::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CSE_CONTENTS_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("SM_CONTENTMGR_SE_SEARCH"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}

    m_CurrentCM = _T("CM_CONTENTMGR_SE_SEARCH") ;
    m_ParentDM  = _T("DM_CONTENTMGR_SE_MAIN") ; 
    m_ParentCM  = _T("CM_CONTENTMGR_SE_MAIN") ;

	AllControlDisplay (m_CurrentCM,-1) ;
     

//	AddColorControl (IDC_eTITLE,_T(""),_T(""),'E') ;


	//2004-07-21 �׽�Ʈ �� ���� 
/*
	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_pInfo->CONNECTION_INFO ) ;
	CString strQuery ;
	strQuery.Format (_T("select rec_key , id from mn_user_Tbl where rownum <10 ")) ;
	dm.RestructDataManager(strQuery) ;
	dm.JSSORT (0,TRUE) ;
	CString strRecKey,strID;
	for(INT i = 0 ;i < dm.GetRowCount () ;i++)
	{
		dm.GetCellData (i,0,strRecKey) ;
		dm.GetCellData (i,1,strID) ;
	}

*/
	EnableThemeDialogTexture(GetSafeHwnd());


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSE_CONTENTS_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN ) 
	{
		OnOk() ;
		return TRUE ;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CSE_CONTENTS_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
