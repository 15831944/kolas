// SE_CENTERUPLOAD_Search.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CENTERUPLOAD_Search.h"
#include "SE_CENTERUPLOAD_01.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CENTERUPLOAD_Search dialog


CSE_CENTERUPLOAD_Search::CSE_CENTERUPLOAD_Search(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CENTERUPLOAD_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CENTERUPLOAD_Search::~CSE_CENTERUPLOAD_Search()
{
	
}

BOOL CSE_CENTERUPLOAD_Search::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CSE_CENTERUPLOAD_Search::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CENTERUPLOAD_Search)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CENTERUPLOAD_Search, CDialog)
	//{{AFX_MSG_MAP(CSE_CENTERUPLOAD_Search)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CENTERUPLOAD_Search message handlers

BOOL CSE_CENTERUPLOAD_Search::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_SE_CENTERUPLOAD_SEARCH")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
    
	m_ParentCM = _T("CM_SE_CENTERUPLOAD_01") ;
	m_CurrentCM = _T("CM_SE_CENTERUPLOAD_SEARCH");

	AllControlDisplay (m_CurrentCM,-1) ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CENTERUPLOAD_Search::OnOK() 
{
    INT ids;
    CString sWhere;
	strBookWhere.Empty();

    // ��Ʈ���� ����Ÿ�� �о�´�.
    const alias_cnt = 6;
    CString  alias[alias_cnt] = {
        _T("UPLOAD����"), _T("�谡����-����"), _T("�谡����-����"), _T("�谡��ġ"),
        _T("��ȣ����"),   _T("��ȣ")
    };
     
    CString data[6]  ;

    CESL_ControlMgr *pCM = FindCM(_T("CM_SE_CENTERUPLOAD_SEARCH")) ;
    
	for(INT i = 0 ; i < 6 ; i ++) 
	{
       ids = pCM->GetControlMgrData (alias[i],data[i],-1,0) ;
	   data[i].TrimLeft() ;
	   data[i].TrimRight () ;

	   if(ids <0 ) AfxMessageBox (_T("GetControlMgrData() Error")) ;
	}
	
    

    // ��ü�ڷῡ�� ã�°�� �ٸ� ������ �� �Է��ؾ��Ѵ�.
    if (  data[0] == _T("��ü") ) {
        if ((  data[1].IsEmpty () || data[2].IsEmpty () ) && (  data[3].IsEmpty () ) && (  data[5].IsEmpty () )) {
            AfxMessageBox (_T("��ü�ڷῡ�� �˻��� �ٸ� ������ �� �Է��ؾ� �մϴ�."));
		  	return ;
        }
    }

  
    if      (  data[0] == _T("��ü")) sWhere = _T("(s.center_upload_status = '0' OR s.CENTER_UPLOAD_STATUS IS NULL OR s.CENTER_UPLOAD_STATUS ='1' OR s.CENTER_UPLOAD_STATUS = '2' OR s.CENTER_UPLOAD_STATUS = '3' OR s.CENTER_UPLOAD_STATUS = '4' ) ");
	else if (  data[0] == _T("�ȿø��ڷ�") ) sWhere = _T("s.CENTER_UPLOAD_STATUS ='1' OR s.CENTER_UPLOAD_STATUS IS NULL ");
    else if (  data[0] == _T("�ȿø��ڷ�(Ȯ�ο��)")) sWhere = _T("s.CENTER_UPLOAD_STATUS = '2' ");
    else if (  data[0] == _T("�ȿø��ڷ�(MARC����)") ) sWhere = _T("s.CENTER_UPLOAD_STATUS = '3' ");
    else if (  data[0] == _T("�ȿø��ڷ�(��Ÿ)")) sWhere = _T("s.CENTER_UPLOAD_STATUS = '4' ");
	else if (  data[0] == _T("�ø��ڷ�") ) sWhere = _T("s.CENTER_UPLOAD_STATUS = '0' ");
    
   

	if( !data[1].IsEmpty() )
	{
		if( sWhere.GetLength() > 1) sWhere += _T(" AND S.REC_KEY IN ( select distinct species_key from se_book_tbl b where ");
		if(data[2].IsEmpty () )
		{
			if(!data[3].IsEmpty ())
			{
            sWhere += _T(" b.shelf_Date >= to_date  ('") +data[1] + _T("' , 'yyyy/mm/dd' ) ")  ;
			sWhere += _T(" AND  b.SHELF_LOC_CODE = '") + data[3] + _T("')") ;	
            strBookWhere += _T("b.shelf_Date >= to_date  ('") +data[1] + _T("' , 'yyyy/mm/dd' ) ") ;
            strBookWhere += _T(" AND b.SHELF_LOC_CODE = '") + data[3] + _T("')") ;
			}
			else
			{
             sWhere += _T(" b.shelf_Date >= to_date  ('") +data[1] + _T("' , 'yyyy/mm/dd' )) ")  ;
             strBookWhere +=  _T("b.shelf_Date >= to_date  ('") +data[1] + _T("' , 'yyyy/mm/dd' ) ")  ;
			}
		}
		else
		{    
		
			if(!data[3].IsEmpty ())
			{
                 sWhere += _T(" b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[2]+ _T("', 'yyyy/mm/dd' ) ") ;
			     sWhere += _T(" AND  b.SHELF_LOC_CODE = ' ") + data[3] + _T("')") ;	
                 strBookWhere += _T("b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[2]+ _T("', 'yyyy/mm/dd' ) ") ;
                 strBookWhere += _T(" AND  b.SHELF_LOC_CODE = '") + data[3] + _T("')") ;	
			}
			else
			{
               	sWhere += _T("b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[2]+ _T("', 'yyyy/mm/dd' ) )") ;
                strBookWhere += _T("b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[2]+ _T("', 'yyyy/mm/dd' ) ") ;
			}
		}
       
	}
    else
    {
		if(!data[3].IsEmpty ())
		{
			sWhere += _T(" AND S.REC_KEY IN ( select distinct species_key from se_book_tbl b where ");
			sWhere += _T(" b.SHELF_LOC_CODE = '") + data[3] + _T("')") ;	
            strBookWhere += _T(" b.SHELF_LOC_CODE = '") + data[3] + _T("')") ;
		}
    }

    if(data[5].GetLength ()>12)
	{
       AfxMessageBox (_T("[��Ϲ�ȣ OR �����ȣ] ���̸� Ȯ���Ͽ� �ֽʽÿ�.")) ;
	}
	

    if (data[5].GetLength () > 0 ) {
        if (  data[4] == _T("��Ϲ�ȣ") ) {
            CString sTemp;
			TCHAR *tempdata = _T("") ;
			tempdata = (TCHAR*)malloc(sizeof(TCHAR)*(data[5].GetLength ()+sizeof(TCHAR))) ;
			_tcscpy(tempdata,data[5].GetBuffer (0)) ;
          
			ids = MakeAccessionNoWhere( _T("b.reg_NO"), tempdata, sTemp, 100 );
            if (ids) {
                if ( ids == -100 ) AfxMessageBox (_T("��Ϲ�ȣ�� 100�������� �˻��� �����մϴ�."));     
            }
       

            sWhere += _T(" AND ");
			sWhere += _T("s.rec_key in (select distinct species_key from se_book_tbl b where ")+sTemp+_T(")") ;
			strBookWhere += _T(" and ") + sTemp ;
        }
        else if ( data[4] == _T("�����ȣ") ) {
            sWhere +=  _T("and s.CONTROL_NO = '") + data[5] + _T("'") ;
        }
    }
    

	CESL_DataMgr * m_pBaseDataMgr = FindDM(_T("DM_SE_CENTERUPLOAD_01")) ;
    if(m_pBaseDataMgr == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return  ;
	}


    ids = m_pBaseDataMgr->RefreshDataManager(sWhere)  ;
    if(ids < 0) AfxMessageBox (_T("RefreshDataManager Error ")) ;
     
    
    if ( 0 == m_pBaseDataMgr->GetRowCount() ) AfxMessageBox (_T("���ǿ� �´� �ڷᰡ �����ϴ�."));

     
    CString temp ;
	CString value ;
    for(i = 0 ; i < m_pBaseDataMgr->GetRowCount () ; i ++) {

		   ids = m_pBaseDataMgr->GetCellData (_T("UPLOAD����"),i, temp ) ;
            
		   INT m_dir ; 

		   if(temp.IsEmpty ())
		   { 
			   m_dir = 5 ;
		   }
			
		   else {
		       m_dir = _ttoi(temp.GetBuffer (0)) ;
		   }

		   switch(m_dir) {
		        case 0 :
                    value = _T("�ø��ڷ�") ;
			    	break;
			    
				case  1 : 
				    value = _T("�ȿø��ڷ�") ;
			    	break;
				case 2 :
				   value = _T("�ȿø��ڷ�(Ȯ�ο��)");
			    	break;
				case 3 :
				   value = _T("�ȿø��ڷ�(MARC����)") ;
				   break;
				case 4 :
				    value = _T("�ȿø��ڷ�(��Ÿ)") ;

				    break ;
				case 5 :
                    value = _T("�ȿø��ڷ�") ;
			    	break;
		   }
         
		   ids = m_pBaseDataMgr->SetCellData (_T("UPLOAD����"),value ,i) ;

	   }


    AllControlDisplay (m_ParentCM ,-1) ;
    
    INT nTotalCnt ;
	nTotalCnt = m_pBaseDataMgr->GetRowCount () ;
	CString str ;//= _T("���⿡ �̷��� �޽����� ������ �� �ֽ��ϴ�.");
	str.Format (_T("%d �� �˻� �Ǿ����ϴ�."),nTotalCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));
    
     
    ((CSE_CENTERUPLOAD_01*)this->pParentMgr)->m_strBookWhere = strBookWhere ;


}

VOID CSE_CENTERUPLOAD_Search::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


INT  CSE_CENTERUPLOAD_Search::MakeAccessionNoWhere ( TCHAR *lpFieldName , TCHAR *lpValue , CString & sWhere , INT MaxArrayCount )
{
    TCHAR   *TokenStr = NULL;
	TCHAR    szTmp[128] , szAccessionNo[128];
	INT     DummyCount , AccessionNo , i;
	INT     AccessionCount = 0;

	TCHAR   *mValue = (TCHAR *)malloc( sizeof(TCHAR) * _tcsclen(lpValue) +sizeof(TCHAR) );

	_tcscpy ( mValue , lpValue );
	

	////////////////////////////////////////////////////////
	// 1. '+' , _T("-' �����ڸ� üũ�Ѵ�.
	////////////////////////////////////////////////////////
	// 1.1 '+'  �����ڸ� üũ�Ѵ�.
    TokenStr = _tcstok( mValue, _T("+") );
	if ( _tcscmp ( TokenStr , lpValue ) !=0 ) {
        _tcscpy ( szAccessionNo , TokenStr );
		TokenStr = _tcstok( NULL, _T("+") );
		if ( TokenStr != NULL ) {
			DummyCount = _ttoi(TokenStr);
			if ( DummyCount > MaxArrayCount && MaxArrayCount > 0 ) {
				free ( mValue );
				return -100;
			}
		}
		if ( szAccessionNo[0] >= 97 && szAccessionNo[0] < 123 ) szAccessionNo[0] -= 32;
		if ( szAccessionNo[1] >= 97 && szAccessionNo[1] < 123 ) szAccessionNo[1] -= 32;

		//  ��ϱ����� ���� ��Ϲ�ȣ ó��.
		if ( ( szAccessionNo[0] >= '0' && szAccessionNo[0] <= '9' ) &&
			 ( szAccessionNo[1] >= '0' && szAccessionNo[1] <= '9' ) )
		{
			_tcscpy( szTmp, szAccessionNo );
			AccessionNo = _ttoi( szAccessionNo );
		} else {
			szTmp[0] = szAccessionNo[0];
			szTmp[1] = szAccessionNo[1];
			_stprintf( &szTmp[2], _T("%010d"), _ttoi( &szAccessionNo[2] ) );
			AccessionNo = _ttoi( &szAccessionNo[2] );
		}

		sWhere.Format( _T(" %s in (") , lpFieldName );
		sWhere += _T("'");
		sWhere +=  szTmp;
		sWhere += _T("'");

		for ( i = 0; i < DummyCount; i ++ ) {
			//  ��ϱ����� ���� ��Ϲ�ȣ ó��.
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
			// �����Һκ�
// 			INT nRegCodeLength;
// 			nRegCodeLength = 
			//===================================================================
			if ( ( szAccessionNo[0] >= '0' && szAccessionNo[0] <= '9' ) &&
				 ( szAccessionNo[1] >= '0' && szAccessionNo[1] <= '9' ) )
			{
				_stprintf( szTmp, _T("%d"), AccessionNo +i+1  );
			} else {
				_stprintf( &szTmp[2], _T("%010d"), AccessionNo +i+1  );
			}

			sWhere += _T(",");
			sWhere += _T("'");
			sWhere +=  szTmp;
			sWhere += _T("'");
		}
		sWhere += _T(")");
		free ( mValue );
		return 0;
	}

	// 1.2 '-'  �����ڸ� üũ�Ѵ�.
	TokenStr = NULL;
    TokenStr = _tcstok( mValue, _T("-") );
	if ( _tcscmp ( TokenStr , lpValue ) !=0 ) {
        _tcscpy ( szAccessionNo , TokenStr );
		TokenStr = _tcstok( NULL, _T("-") );
		if ( TokenStr != NULL ) {
			DummyCount = _ttoi(TokenStr);
			if ( DummyCount > MaxArrayCount && MaxArrayCount > 0 ) {
				free ( mValue );
				return -100;
			}
		}
		if ( szAccessionNo[0] >= 97 && szAccessionNo[0] < 123 ) szAccessionNo[0] -= 32;
		if ( szAccessionNo[1] >= 97 && szAccessionNo[1] < 123 ) szAccessionNo[1] -= 32;

		//  ��ϱ����� ���� ��Ϲ�ȣ ó��.
		if ( ( szAccessionNo[0] >= '0' && szAccessionNo[0] <= '9' ) &&
			 ( szAccessionNo[1] >= '0' && szAccessionNo[1] <= '9' ) )
		{
			_tcscpy( szTmp, szAccessionNo );
			AccessionNo = _ttoi( szAccessionNo );
		} else {
			szTmp[0] = szAccessionNo[0];
			szTmp[1] = szAccessionNo[1];
			_stprintf( &szTmp[2], _T("%010d"), _ttoi( &szAccessionNo[2] ) );
			AccessionNo = _ttoi( &szAccessionNo[2] );
		}

		sWhere.Format( _T(" %s in (") , lpFieldName );
		sWhere += _T("'");
		sWhere +=  szTmp;
		sWhere += _T("'");

		for ( i = 0; i < DummyCount; i ++ ) {
			if ( AccessionNo - i <= 0 ) break;
			if ( ( szAccessionNo[0] >= '0' && szAccessionNo[0] <= '9' ) &&
				 ( szAccessionNo[1] >= '0' && szAccessionNo[1] <= '9' ) )
			{
				_stprintf( szTmp, _T("%d"), AccessionNo - i -1 );
			} else {
				_stprintf( &szTmp[2], _T("%010d"), AccessionNo - i -1 );
			}

			sWhere += _T(",");
			sWhere += _T("'");
			sWhere +=  szTmp;
			sWhere += _T("'");
		}
		sWhere += _T(")");
		free ( mValue );
		return 0;
	}

	////////////////////////////////////////////////////////
	// 2. ' '  �����ڸ� üũ�Ѵ�.
	////////////////////////////////////////////////////////
	TokenStr = NULL;
    TokenStr = _tcstok( mValue, _T(" ") );
	if ( _tcscmp ( TokenStr , lpValue ) !=0 ) {
		if ( TokenStr[0] >= 97 && TokenStr[0] < 123 ) TokenStr[0] -= 32;
		if ( TokenStr[1] >= 97 && TokenStr[1] < 123 ) TokenStr[1] -= 32;

		//  ��ϱ����� ���� ��Ϲ�ȣ ó��.
		if ( ( TokenStr[0] >= '0' && TokenStr[0] <= '9' ) &&
			 ( TokenStr[1] >= '0' && TokenStr[1] <= '9' ) )
		{
			_tcscpy( szTmp, TokenStr );
		} else {
			szTmp[0] = TokenStr[0];
			szTmp[1] = TokenStr[1];

			_stprintf( &szTmp[2], _T("%010d"), _ttoi( &TokenStr[2] ) );
			szTmp[12] = 0;
		}

		sWhere.Format( _T(" %s in (") , lpFieldName );
		sWhere += _T("'");
		sWhere +=  szTmp;
		sWhere += _T("'");
		AccessionCount++;

		while ( 1 ) {
			TokenStr = _tcstok( NULL , _T(" ") );
			if ( TokenStr == NULL ) break;
			if ( AccessionCount > MaxArrayCount && MaxArrayCount > 0 ) {
				free ( mValue );
				return -100;
			}
			if ( NULL != TokenStr ) {
				if ( TokenStr[0] >= 97 && TokenStr[0] < 123 ) TokenStr[0] -= 32;
				if ( TokenStr[1] >= 97 && TokenStr[1] < 123 ) TokenStr[1] -= 32;

				//  ��ϱ����� ���� ��Ϲ�ȣ ó��.
				if ( ( TokenStr[0] >= '0' && TokenStr[0] <= '9' ) &&
					 ( TokenStr[1] >= '0' && TokenStr[1] <= '9' ) )
				{
					_stprintf( szTmp, _T("%s"), TokenStr );
				} else {
					szTmp[0] = TokenStr[0];
					szTmp[1] = TokenStr[1];

					_stprintf( &szTmp[2], _T("%010d"), _ttoi( &TokenStr[2] ) );
					szTmp[12] = 0;
				}
				sWhere += _T(",");
				sWhere += _T("'");
				sWhere +=  szTmp;
				sWhere += _T("'");
				AccessionCount++;
			}
		}
		if ( AccessionCount > 0 ) {
			sWhere += _T(")");
		}
		free ( mValue );
		return 0;
	}
	if ( lpValue[0] >= 97 && lpValue[0] < 123 ) lpValue[0] -= 32;
	if ( lpValue[1] >= 97 && lpValue[1] < 123 ) lpValue[1] -= 32;

	//  ��ϱ����� ���� ��Ϲ�ȣ ó��.
	if ( ( TokenStr[0] >= '0' && TokenStr[0] <= '9' ) &&
		 ( TokenStr[1] >= '0' && TokenStr[1] <= '9' ) )
	{
		_tcscpy( szTmp, lpValue );
	} else {
		szTmp[0] = lpValue[0];
		szTmp[1] = lpValue[1];
		_stprintf( &szTmp[2], _T("%010d"), _ttoi( &lpValue[2] ) );
	}

	sWhere.Format( _T(" %s in ( '%s' )") , lpFieldName , szTmp );
	free ( mValue );

	return 0;
}
HBRUSH CSE_CENTERUPLOAD_Search::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
