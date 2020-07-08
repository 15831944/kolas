


#include "stdafx.h"
#include "ILL_SEARCH_BOOK.h"
#include "Imm.h"
#include "..\..\..\..\����\K2UP_Common\MakeSearchData.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"
#include "ILL_SEARCH_BOOK_LIST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CILL_SEARCH_BOOK::CILL_SEARCH_BOOK(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
		
	

	
	
	
	m_nSearchMode = 0;
	
	m_strSelectedBookRegNo = _T("");

	
	InitSearchBookAppendixFlag();
}

CILL_SEARCH_BOOK::~CILL_SEARCH_BOOK()
{
}

VOID CILL_SEARCH_BOOK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	
}


BEGIN_MESSAGE_MAP(CILL_SEARCH_BOOK, CDialog)
	//{{AFX_MSG_MAP(CILL_SEARCH_BOOK)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnUSERFIND1, OnbtnUSERFIND1)
	ON_BN_CLICKED(IDC_btnUSERCLOSE, OnbtnUSERCLOSE)
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnCLEAR)
	ON_BN_CLICKED(IDC_btnSEARCH_DETAIL, OnbtnSEARCHDETAIL)
	ON_EN_SETFOCUS(IDC_edtUSERFIND1, OnSetfocusedtUSERFIND1)
	ON_WM_SHOWWINDOW()
	ON_EN_SETFOCUS(IDC_edtSREG_NO, OnSetfocusedtSREGNO)
	ON_EN_CHANGE(IDC_edtUSERFIND9, OnChangeedtUSERFIND9)
	ON_EN_CHANGE(IDC_edtUSERFIND10, OnChangeedtUSERFIND10)
	ON_EN_SETFOCUS(IDC_edtUSERFIND2, OnSetfocusedtUSERFIND2)
	ON_EN_SETFOCUS(IDC_edtUSERFIND3, OnSetfocusedtUSERFIND3)
	ON_EN_SETFOCUS(IDC_edtUSERFIND4, OnSetfocusedtUSERFIND4)
	ON_EN_SETFOCUS(IDC_edtUSERFIND5, OnSetfocusedtUSERFIND5)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MENU, OnSelchangeTabMenu)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SHOWED, OnShowed)	
END_MESSAGE_MAP()




BOOL CILL_SEARCH_BOOK::Create(CWnd* pParentWnd) 
{
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CILL_SEARCH_BOOK::OnClose() 
{
	

	



		CDialog::OnClose();

	
}

BOOL CILL_SEARCH_BOOK::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	if(InitESL_Mgr(_T("ILL_SEARCH_BOOK")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	((CButton*)GetDlgItem(IDC_chkUSE_LIMIT))->SetCheck(1);


	SetWindowSize( CString(_T("�����˻�")) );
	((CComboBox*)GetDlgItem(IDC_cmbUSERFIND13))->SetCurSel(0);

	InitTabControl();

	
	
	CString strQuery;
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE VALUE_NAME = 'å�η���뿩��' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
	CESL_DataMgr pDM;
	pDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	CString strResult; 
	pDM.GetOneValueQuery(strQuery, strResult);
	if(_T("Y") == strResult)
	{
		GetDlgItem(IDC_chkUSE_ILL_LIMIT)->ShowWindow(SW_SHOW);
	}

	return TRUE;  
}

VOID CILL_SEARCH_BOOK::OnbtnUSERFIND1() 
{	
	INT ids = 0;
	CString strFuncName = _T("CILL_SEARCH_BOOK::OnbtnUSERFIND1()");

	
	
	CString sBOWhere, sSEWhere;
	sBOWhere = _T("");
	sSEWhere = _T("");
	



	{
		
		CILL_SEARCH_BOOK_LIST Dlg(this);		
		CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)FindControl(
			_T("CM_ILL_SEARCH_BOOK"), _T("�ڷᱸ��"));
		if(NULL == pCMB) ERROR_MSG_RETURN_VOID2(-5, strFuncName);
		
		CString strResult;
		pCMB->GetWindowText(strResult);
		if(_T("����") == strResult)
		{
			Dlg.m_bIs_BO_Search = TRUE;			
			ids = GetSqlSearchBookInfo(sBOWhere, 0);
		}
		else if(_T("����") == strResult)
		{
			Dlg.m_bIs_SE_Search = TRUE;			
			ids = GetSqlSearchBookInfo(sSEWhere, 1);
		}
		else if(_T("��ü") == strResult)
		{
			Dlg.m_bIs_BO_Search = TRUE;
			Dlg.m_bIs_SE_Search = TRUE;			
			ids = GetSqlSearchBookInfo(sBOWhere, 0);
			ids = GetSqlSearchBookInfo(sSEWhere, 1);
		}
		else ERROR_MSG_RETURN_VOID2(-6, strFuncName);
		
		
		
		if(0 != ids) return;

		
		
		Dlg.m_pInfo = m_pInfo;
		Dlg.m_strSearch_Option_BO = sBOWhere;
		Dlg.m_strSearch_Option_SE = sSEWhere;
		
		
		Dlg.m_sBookCooperativeMode = m_sBookCooperativeMode;

		Dlg.m_nSearchMode = m_nSearchMode;

		if(IDOK == Dlg.DoModal())
		{
			
			
			if(0 == m_nSearchMode)
			{
				
				int nIdx = Dlg.m_nSelectIdx;
				
				CESL_DataMgr* pBookDM = Dlg.m_pSelectDM;
				if(NULL == pBookDM) ERROR_MSG_RETURN_VOID2(-1, strFuncName);
				
				CESL_ControlMgr* pAplCM = FindCM(_T("CM_ILL_APL"));
				if(NULL == pAplCM) ERROR_MSG_RETURN_VOID2(-2, strFuncName);
				
				const int GET_COUNT = 12;
				CString strDMAlias[GET_COUNT] = 
				{
					_T("��Ϲ�ȣ"),
						_T("û����ȣ"),
						_T("����"),
						_T("������"),
						_T("������"),
						_T("����⵵"),
						_T("�����"),
						_T("������_�̸�"),
						_T("������_�ڵ�"),
						_T("�������±���"),
						_T("åKEY"),
						_T("��������")
						
				};
				CString strEditAlias[GET_COUNT] = 
				{
					_T("å_��Ϲ�ȣ"),
						_T("å_û����ȣ"),
						_T("å_����"),
						_T("å_����"),
						_T("å_������"),
						_T("å_����⵵"),
						_T("å_��"),
						_T("��û_�����������̸�"),
						_T("��û_�����������ڵ�"),
						_T("å_�������±���"),
						_T("å_Ű"),
						_T("å_��������")
				};
				
				for(int nCnt = 0; nCnt < GET_COUNT; nCnt++)
				{
					CString strGet;
					ids = pBookDM->GetCellData(strDMAlias[nCnt], nIdx, strGet);
					if(0 > ids) ERROR_MSG_RETURN_VOID2(-3, strFuncName);
					
					CESL_ControlEdit* pEdit = (CESL_ControlEdit*)pAplCM->FindControl(
						strEditAlias[nCnt]);
					if(NULL == pEdit) ERROR_MSG_RETURN_VOID2(-4, strFuncName);
					
					ids = pEdit->SetData(strGet);
					if(0 > ids) ERROR_MSG_RETURN_VOID2(-5, strFuncName);					
				}
				
				ids = pBookDM->GetCellData(_T("�η��ڷ�"), nIdx, m_strAppendixFlag);
			}
			else
			{
				
				int nIdx = Dlg.m_nSelectIdx;
				
				CESL_DataMgr* pBookDM = Dlg.m_pSelectDM;
				if(NULL == pBookDM) ERROR_MSG_RETURN_VOID2(-1, strFuncName);
				
				ids = pBookDM->GetCellData(_T("��Ϲ�ȣ"), nIdx, m_strSelectedBookRegNo);

				
				ids = pBookDM->GetCellData(_T("�η��ڷ�"), nIdx, m_strAppendixFlag);
			}			

























































			CDialog::OnOK();
		}
	}

	
}

VOID CILL_SEARCH_BOOK::OnbtnUSERCLOSE() 
{
	

	OnCancel();

	
}

VOID CILL_SEARCH_BOOK::OnbtnCLEAR() 
{
	

	INT ids;
	CString sCMAlias = _T("CM_ILL_SEARCH_BOOK");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnCLEAR") );

	ids = pCM->AllControlClear();
	if( ids < 0 ) return ;

	
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_ILL_SEARCH_BOOK"), _T("��������"));
	if(NULL != pCMB)
	{
		const int CMB_COUNT = pCMB->GetRowCount();
		for(int nCnt = 0; nCnt < CMB_COUNT; nCnt++)
		{
			CString strData;
			pCMB->GetData(strData, nCnt, 1);
			if(_T("�������") == strData)
			{
				pCMB->SetCurSel(nCnt);
				break;
			}
		}
	}

	
	GetDlgItem(IDC_edtUSERFIND1)->SetFocus();
	
}









VOID CILL_SEARCH_BOOK::SetWindowSize( CString &sCaption )
{
	

	CRect ChangeWndPos;		
	CRect WndPos;				  
	
	INT nIDC;
	
	TCHAR* szCaption[2] = { _T("�󼼰˻�") , _T("�����˻�") };

	
	if( sCaption.Compare( szCaption[0] ) == 0 )
	{
		GetDlgItem( IDC_btnSEARCH_DETAIL )->SetWindowText( CString(szCaption[1]) );
		nIDC = IDC_staSEARCH_VIEWDETAIL;
	}
	
	else
	{
		GetDlgItem( IDC_btnSEARCH_DETAIL )->SetWindowText( CString(szCaption[0]) );
		nIDC = IDC_staSEARCH_VIEWSIMPLE;
	}

	
	GetWindowRect( &WndPos );
	
	
	GetDlgItem( nIDC )->GetWindowRect( &ChangeWndPos );
	
	ClientToScreen(&WndPos);
	ClientToScreen(&ChangeWndPos);

	
	WndPos.SetRect( WndPos.left , WndPos.top , WndPos.right , ChangeWndPos.bottom );
	
	ScreenToClient(&WndPos);

	MoveWindow( WndPos );	

	
}

BOOL CILL_SEARCH_BOOK::PreTranslateMessage(MSG* pMsg) 
{
	

	DWORD lstyle = GetStyle();
	if ( WS_POPUP == (lstyle & WS_POPUP) ) {
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
		{
			
			if ( GetDlgItem(IDC_edtSREG_NO)->m_hWnd == pMsg->hwnd ) {				
				INT ids = -1;
				CString strGetValue = _T(""), strLibCode = _T(""), strRegNo12 = _T(""), strRegCode = _T("");
				GetDlgItem(IDC_edtSREG_NO)->GetWindowText(strGetValue);		
				ids = ConvertILLRegNoTONormal(strGetValue, strLibCode, strRegNo12, strRegCode);
				if ( ids < 0 ) {
					AfxMessageBox(_T("ConvertILLRegNoTONormal() Error!!!"));
					return TRUE;
				}				
				else if ( ids == 0 ) {
					GetDlgItem(IDC_edtSREG_NO)->SetWindowText(strRegNo12);					
				}
			}

			OnbtnUSERFIND1();
			return TRUE;
		}
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
		{
			OnbtnUSERCLOSE();
			return TRUE;
		}
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);

}

VOID CILL_SEARCH_BOOK::OnbtnSEARCHDETAIL() 
{
	

	CString sCaption;
	GetDlgItem( IDC_btnSEARCH_DETAIL )->GetWindowText( sCaption );
	SetWindowSize( sCaption );		

	
}

VOID CILL_SEARCH_BOOK::OnSetfocusedtUSERFIND1() 
{
	
    HIMC hIME;
    DWORD dwConversion, dwSentence;
	
    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND1)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;
		
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		
        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND1)->m_hWnd,hIME); 
    } 
	
	
}

VOID CILL_SEARCH_BOOK::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	PostMessage( WM_SHOWED );

}


LRESULT CILL_SEARCH_BOOK::OnShowed(WPARAM wParam, LPARAM lParam)
{
	if( IsWindowVisible() )
	{
		OnbtnCLEAR();
		GetDlgItem(IDC_edtUSERFIND1)->SetFocus();
	}

	return 0;
}
VOID CILL_SEARCH_BOOK::OnSetfocusedtSREGNO() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtSREG_NO)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
		dwConversion |= IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtSREG_NO)->m_hWnd,hIME); 
    }
}

VOID CILL_SEARCH_BOOK::OnChangeedtUSERFIND9() 
{
	
	
	
	
	
	
	
	CheckClassNo( IDC_edtUSERFIND9 );
}

VOID CILL_SEARCH_BOOK::OnChangeedtUSERFIND10() 
{
	
	
	
	
	
	
	
	CheckClassNo( IDC_edtUSERFIND10 );
}

VOID CILL_SEARCH_BOOK::CheckClassNo( UINT ID )
{
	return;
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( ID );
	CString sClassNo, sNewClassNo;
	pEdt->GetWindowText( sClassNo );

	sClassNo.TrimLeft();
	sClassNo.TrimRight();
	sClassNo.Replace( _T(" "), _T("") );
	INT len = sClassNo.GetLength();
	if( !len )
		return;

	TCHAR cClassNo;
	for( INT i = 0; i < len; i++ )
	{
		cClassNo = sClassNo.GetAt( i );

		if( ( cClassNo >= '0' && cClassNo <= '9' ) || cClassNo == '.' )
			sNewClassNo += cClassNo;
	}
		
	pEdt->SetWindowText( sNewClassNo );
	len = sNewClassNo.GetLength();
	pEdt->SetSel( len, len );
}

VOID CILL_SEARCH_BOOK::OnSetfocusedtUSERFIND2() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND2)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND2)->m_hWnd,hIME); 
    }
}

VOID CILL_SEARCH_BOOK::OnSetfocusedtUSERFIND3() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND3)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND3)->m_hWnd,hIME); 
    }
}

VOID CILL_SEARCH_BOOK::OnSetfocusedtUSERFIND4() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND4)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND4)->m_hWnd,hIME); 
    }
}

VOID CILL_SEARCH_BOOK::OnSetfocusedtUSERFIND5() 
{
	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND5)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND5)->m_hWnd,hIME); 
    }
}





void CILL_SEARCH_BOOK::OnSelchangeTabMenu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_MENU);
	if( pTab )
	{
		
		INT nFlag = pTab->GetCurSel();

		CButton* pBtn = (CButton*)GetDlgItem(IDC_chkUSE_UserSearch);
		if( pBtn )
		{
			pBtn->SetCheck(nFlag);
		}
	}

	*pResult = 0;
}

void CILL_SEARCH_BOOK::InitTabControl()
{


	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_MENU);
	if( NULL == pTab )	return;

	TC_ITEM item;
	item.mask = TCIF_IMAGE | TCIF_TEXT;
	item.iImage = 0;
	
	
	item.pszText = _T("������ �ڷ�ã��");
	pTab->InsertItem(0, &item);
	
	
	item.pszText = _T("�̿��ڿ� �ڷ�ã��");
	pTab->InsertItem(1, &item);

	pTab->SetCurSel(0);


}




HBRUSH CILL_SEARCH_BOOK::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}



INT CILL_SEARCH_BOOK::GetSqlSearchBookInfo( CString &sWhere , INT nMode )
{


	INT ids;

	INT nSearchField = 0;

	CString sCMAlias = _T("CM_ILL_SEARCH_BOOK");
	const INT nFieldCnt = 4;
	
	
	
	TCHAR *sFieldAlias[nFieldCnt] = 
	{
		_T("å����") , _T("���ڸ�") , _T("�����ڸ�") , _T("��ü")
	};

	CString sGetData[nFieldCnt];

	for( INT i = 0 ; i < nFieldCnt ;i++ )
	{
		ids = GetControlData( sCMAlias , sFieldAlias[i] , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
		sGetData[i].TrimRight();
	}

	CMakeSearchData MakeSearchData(this);

	
	

	
	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("rb"));

	CString sEcoSearchWhere;	
	INT nEcoSearchCnt = 0;
		
	BOOL bIsEqualSearch;

	
	if( !sGetData[0].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		






		


			bIsEqualSearch = FALSE;
			
			if( sGetData[0].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sGetData[0]);
			if( !bIsEqualSearch ) 
				sGetData[0] += _T("*");
			
			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fd != NULL )
				{
					
					if( nMode == 0 )
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , BO_IDX_TITLE, '") + sGetData[0] + _T("') ");
					else
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_TITLE, '") + sGetData[0] + _T("') ");
				}
				else
				{
					if( nMode == 0 )
						sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_TITLE, '") + sGetData[0] + _T("') ");
					else
						sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_TITLE, '") + sGetData[0] + _T("') ");
					
					nEcoSearchCnt++;
					
				}
			}
			else
				sWhere += _T(" CATSEARCH( IB.IDX_TITLE, '") + sGetData[0] + _T("',NULL) > 0 ");

		nSearchField++;
	}

	if( !sGetData[1].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		






		


			bIsEqualSearch = FALSE;
			
			if( sGetData[1].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			MakeSearchData.GetKeyWord(sGetData[1]);
			
			if( !bIsEqualSearch ) 
				sGetData[1] += _T("*");

			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fd != NULL )
				{
					
					if( nMode == 0 )
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , BO_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
					else
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
					
				}
				else
				{
					if( !sEcoSearchWhere.IsEmpty() )
						sEcoSearchWhere += _T(" INTERSECT ");
					
					if( nMode == 0 )
						sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
					else
						sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
					
					nEcoSearchCnt++;
				}
				
			}
			else
				sWhere += _T(" CATSEARCH( IB.IDX_AUTHOR, '") + sGetData[1] + _T("',NULL) > 0 ");


		nSearchField++;
	}

	if( !sGetData[2].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		






		


			bIsEqualSearch = FALSE;
			
			if( sGetData[2].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sGetData[2]);
			if( !bIsEqualSearch ) 
				sGetData[2] += _T("*");
			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				
				if( fd != NULL )
				{
					if( nMode == 0 )
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , BO_IDX_PUBLISHER , '") + sGetData[2] + _T("') ");
					else
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_PUBLISHER , '") + sGetData[2] + _T("') ");
				}
				else
				{
					if( !sEcoSearchWhere.IsEmpty() )
						sEcoSearchWhere += _T(" INTERSECT ");
					
					if( nMode == 0 )
						sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_PUBLISHER , '") + sGetData[2] + _T("') ");
					else
						sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_PUBLISHER , '") + sGetData[2] + _T("') ");
					
					nEcoSearchCnt++;
				}
			}
			else			
				sWhere += _T(" CATSEARCH( IB.IDX_PUBLISHER, '") + sGetData[2] + _T("',NULL) > 0 ");

		
		nSearchField++;
	}

	
	
	if( !sGetData[3].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		






		


			bIsEqualSearch = FALSE;
			
			if( sGetData[3].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sGetData[3]);
			if( !bIsEqualSearch ) 
				sGetData[3] += _T("*");
			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				
				if( fd != NULL )
				{
					if( nMode == 0 )
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , BO_IDX_ALLITEM , '") + sGetData[3] + _T("') ");
					else
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_ALLITEM , '") + sGetData[3] + _T("') ");
				}
				else
				{
					if( !sEcoSearchWhere.IsEmpty() )
						sEcoSearchWhere += _T(" INTERSECT ");
					
					if( nMode == 0 )
						sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_ALLITEM , '") + sGetData[3] + _T("') ");
					else
						sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_ALLITEM , '") + sGetData[3] + _T("') ");
					
					nEcoSearchCnt++;
				}
			}
			else			
				sWhere += _T(" CATSEARCH( IB.IDX_ALL_ITEM, '") + sGetData[3] + _T("',NULL) > 0 ");

		
		nSearchField++;
	}

	
 	const INT nComboFieldCnt = 3;
 	TCHAR *sComboAlias[nComboFieldCnt] =
 	{
 		_T("�ڷ�����") , _T("��ü����") , _T("�ڷ��")
 	};
 
 	TCHAR *sDBFieldAlias[ nComboFieldCnt ] =
 	{
 		_T("IB.FORM_CODE") , _T("IB.MEDIA_CODE") , _T("BB.SHELF_LOC_CODE")
 	};
 
 	CString sComboGetData[nComboFieldCnt];
 
 	for( i = 0 ; i < nComboFieldCnt ; i++ )
 	{
 		ids = GetControlData( sCMAlias , sComboAlias[i] , sComboGetData[i] );
 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
 		sComboGetData[i].TrimRight();
 		if( sComboGetData[i].IsEmpty() || (sComboGetData[i].Compare(_T("0"))==0) || (sComboGetData[i].Compare(_T("�������"))==0) ) continue;
 		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
 		sWhere += sDBFieldAlias[i] + CString(_T(" = '")) + sComboGetData[i] + CString(_T("' "));
 	}

	
	
	const INT nKeyWordFieldCnt = 5;
	TCHAR *sKeyWordAlias[nKeyWordFieldCnt] =
	{
		_T("Ű����1") , _T("Ű����2") , _T("Ű����3") , _T("Ű����4") , _T("Ű����5")
	};
	CString sKeyWordGetData[nKeyWordFieldCnt];

	for( i = 0 ; i < nKeyWordFieldCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sKeyWordAlias[i] , sKeyWordGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
		sKeyWordGetData[i].TrimRight();
	}
	
	CString sKeyWordWhere;
	if( !sKeyWordGetData[0].IsEmpty() )
	{


		
		if( fd != NULL )
			sWhere += _T("(");
		








		


			bIsEqualSearch = FALSE;
			
			if( sKeyWordGetData[0].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sKeyWordGetData[0]);
			if( !bIsEqualSearch ) 
				sKeyWordGetData[0] += _T("*");
			
			sKeyWordWhere = sKeyWordGetData[0];


		if( !sKeyWordGetData[2].IsEmpty() )
		{
			







			


				bIsEqualSearch = FALSE;
				
				if( sKeyWordGetData[2].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sKeyWordGetData[0]);
				if( !bIsEqualSearch ) 
					sKeyWordGetData[2] += _T("*");
				


				
				sKeyWordGetData[1].MakeUpper();
				if( sKeyWordGetData[1].Compare(_T("OR")) == 0 )
					sKeyWordWhere += _T(" | ");
				else
					sKeyWordWhere += _T(" ");
				sKeyWordWhere += sKeyWordGetData[2];				

		}
		if( !sKeyWordGetData[4].IsEmpty() )
		{
			







			



				bIsEqualSearch = FALSE;
				
				if( sKeyWordGetData[4].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sKeyWordGetData[4]);
				if( !bIsEqualSearch ) 
					sKeyWordGetData[4] += _T("*");
				
				sKeyWordGetData[3].MakeUpper();
				if( sKeyWordGetData[3].Compare(_T("OR")) == 0 )
					sKeyWordWhere += _T(" | ");
				else
					sKeyWordWhere += _T(" ");
				sKeyWordWhere += sKeyWordGetData[4];				

		}

		


			if( !sKeyWordWhere.IsEmpty() )
			{

				if( 1 == m_pInfo->GetSearchIndexType() )
				{
					if( fd != NULL )
					{
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , BO_IDX_KEYWORD , '") + sKeyWordWhere + _T("') ");
					}
					else
					{
						if( !sEcoSearchWhere.IsEmpty() )
							sEcoSearchWhere += _T(" INTERSECT ");
						
						sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_KEYWORD , '") + sKeyWordWhere + _T("') ");
					}
					nEcoSearchCnt++;
				}
				else
				{
					if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
					sWhere += CString(_T(" CATSEARCH(IB.IDX_KEYWORD,'")) + sKeyWordWhere + CString(_T("',NULL)>0 "));
				}
				nSearchField++;
			}


		if( fd != NULL )
		sWhere += _T(" )");
	}
	
	
	const INT nPublishYearFieldCnt = 2;
	TCHAR *sPublishYearAlias[nPublishYearFieldCnt] =
	{
		_T("����⵵1") , _T("����⵵2")
	};
	CString sPublishYearGetData[nPublishYearFieldCnt];

	for( i = 0 ; i < nPublishYearFieldCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sPublishYearAlias[i] , sPublishYearGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
		sPublishYearGetData[i].TrimRight();
	}

	if( !sPublishYearGetData[0].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( sPublishYearGetData[1].IsEmpty() )
		{
				sWhere += CString(_T(" IB.IDX_IPUB_YEAR = '")) + sPublishYearGetData[0] + CString(_T("' "));
		}
		else
			sWhere += CString(_T(" ( IB.IDX_IPUB_YEAR >= '")) + sPublishYearGetData[0] + CString(_T("' AND "))
						+ CString(_T(" IB.IDX_IPUB_YEAR <= '")) + sPublishYearGetData[1] + CString(_T("')"));
	}

	const INT nClassNoCnt = 2;
	TCHAR* sClassNoAlias[nClassNoCnt] =
	{
		_T("�з���ȣ1") , _T("�з���ȣ2")
	};
	CString sClassNoData[nClassNoCnt];

	for( i = 0 ; i < nClassNoCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sClassNoAlias[i] , sClassNoData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
		sClassNoData[i].TrimRight();
	}

	if( !sClassNoData[0].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		if( sClassNoData[1].IsEmpty() )
		{
			sWhere += _T(" BB.CLASS_NO = '") + sClassNoData[0] + _T("' ");
		}
		else
		{
			sWhere += _T(" BB.CLASS_NO BETWEEN '") + sClassNoData[0] + _T("' AND '") + sClassNoData[1] + _T("' ") ;
		}
	}
	

	
	CString sRegNo;
	ids = GetControlData( sCMAlias , _T("��Ϲ�ȣ") , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );

	if( !sRegNo.IsEmpty() )
	{
		CLocCommonAPI::MakeRegNoSpaceToZero( this , sRegNo , NULL);

		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.REG_NO = '") + sRegNo +_T("'");

		nSearchField++;
	}

	if( fd != NULL ) fclose(fd);


	if( !sEcoSearchWhere.IsEmpty() )
	{
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		if( nEcoSearchCnt == 1 )
			sWhere += _T(" IB.REC_KEY IN ") + sEcoSearchWhere;
		else
			sWhere += _T(" IB.REC_KEY IN ( ") + sEcoSearchWhere + _T(" ) ");
	}

	
	CString sUseLimit;
	CESL_ControlMgr* pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );

	CESL_Control_Element *pCE = pCM->FindControlElement( _T("�缭����") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	
	CButton* pUseLimit = (CButton*)(GetDlgItem( pCE->CTRL_ID ));
	if( pUseLimit == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );

	if( pUseLimit->GetCheck() == 1 )
	{
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		
		
		
		
		sWhere += _T(" BB.USE_LIMIT_CODE != 'CL' ");

		
		
	}


	CButton *pChkILLUse = (CButton*)GetDlgItem(IDC_chkUSE_ILL_LIMIT);
	if(pChkILLUse->GetCheck() == 1 && 0 == nMode)
	{
		CString strTargetManageCode;
		CString strQuery, strRegCodeList, strSeparateCodeList, strLocCodeList;
		CString strAddQuery;
		CString strAddFullQuery;
		GetControlData(sCMAlias, _T("��������"), strTargetManageCode);
		CESL_DataMgr *pDM = FindDM(_T("DM_USERMAIN_DETAIL_SEARCH_ILL_MANAGE_CODE_LIST"));
		if(strTargetManageCode.IsEmpty() || _T("0") == strTargetManageCode || _T("�������") == strTargetManageCode)
		{
			
			pDM->RefreshDataManager(_T("EMT.VALUE_NAME = 'å�η���뿩��' AND EMT.VALUE_1 = 'Y'"));
			for(INT i=0; i<pDM->GetRowCount(); i++)
			{
				strAddQuery = _T("");
				strTargetManageCode = pDM->GetCellData(_T("��������"), i);
				strRegCodeList = pDM->GetCellData(_T("å�η�����ϱ��и��"), i);
				strSeparateCodeList = pDM->GetCellData(_T("å�η���뺰ġ��ȣ���"), i);
				strLocCodeList = pDM->GetCellData(_T("å�η�����ڷ�Ǹ��"), i);
				if(_T(",") == strRegCodeList || _T(",") == strSeparateCodeList || _T(",") == strLocCodeList)
				{
					if(0 == strTargetManageCode.Compare(m_pInfo->MANAGE_CODE))	
					{						
						if(!sWhere.IsEmpty())sWhere += _T(" AND ");
						strAddQuery.Format(_T(" BB.MANAGE_CODE != '%s'"), strTargetManageCode);
						sWhere += strAddQuery;	
						continue;
					}
				}
				
				CString strAddTempQuery = _T("");
				if(_T("") != strRegCodeList)
				{
					strRegCodeList.TrimLeft();
					strRegCodeList.TrimRight();			
					strRegCodeList.Replace(_T(", "), _T(","));
					strRegCodeList.TrimRight(_T(","));	
					strRegCodeList.Replace(_T(","), _T("','"));
					strRegCodeList = _T("'") + strRegCodeList + _T("'");
					strAddTempQuery.Format(_T(" BB.REG_CODE IN (%s) "), strRegCodeList);
					if(_T("") != strAddQuery) strAddQuery += _T(" AND ");
					strAddQuery += strAddTempQuery;
				}
				if(_T("") != strLocCodeList)
				{
					strLocCodeList.TrimLeft();
					strLocCodeList.TrimRight();			
					strLocCodeList.Replace(_T(", "), _T(","));
					strLocCodeList.TrimRight(_T(","));	
					strLocCodeList.Replace(_T(","), _T("','"));
					strLocCodeList = _T("'") + strLocCodeList + _T("'");
					strAddTempQuery.Format(_T(" BB.SHELF_LOC_CODE IN (%s) "), strLocCodeList);	
					if(_T("") != strAddQuery) strAddQuery += _T(" AND ");
					strAddQuery += strAddTempQuery;				
				}
				if(_T("") != strSeparateCodeList)
				{
					strSeparateCodeList.TrimLeft();
					strSeparateCodeList.TrimRight();			
					strSeparateCodeList.Replace(_T(", "), _T(","));
					strSeparateCodeList.TrimRight(_T(","));	
					strSeparateCodeList.Replace(_T(","), _T("','"));
					strSeparateCodeList = _T("'") + strSeparateCodeList + _T("'");
					
					if ( strSeparateCodeList.Find(_T("�������")) >= 0 ) {
						strAddTempQuery.Format(_T(" ( BB.SEPARATE_SHELF_CODE IN (%s) OR TRIM(BB.SEPARATE_SHELF_CODE) IS NULL ) "), strSeparateCodeList);
					}
					else {
						strAddTempQuery.Format(_T(" BB.SEPARATE_SHELF_CODE IN (%s) "), strSeparateCodeList);
					}
					
					if(_T("") != strAddQuery) strAddQuery += _T(" AND ");
					strAddQuery += strAddTempQuery;	
				}

				if(_T("") != strAddQuery)
				{					
					strAddQuery += _T(" AND BB.MANAGE_CODE = '") + strTargetManageCode + _T("' ");
					CString strTemp;
					strTemp = strAddQuery;
					
					strAddQuery.Format(_T(" (%s) "), strTemp);
				}
				else if(_T("") == strAddQuery)
				{
					strAddQuery += _T(" BB.MANAGE_CODE = '") + strTargetManageCode + _T("' ");
					CString strTemp;
					strTemp = strAddQuery;
					
					strAddQuery.Format(_T(" (%s) "), strTemp);
				}
				
				if(_T("") != strAddFullQuery) 
				{
					if(_T("") != strAddQuery)	strAddFullQuery += _T(" OR ");
				}
				strAddFullQuery += strAddQuery;
			}
			
			CString strTemp;
			strTemp = strAddFullQuery;
			if(strTemp != _T(""))
			{
				strAddFullQuery.Format(_T(" (%s) "), strTemp);
				if(!sWhere.IsEmpty()) sWhere += _T(" AND ");
			}
			sWhere += strAddFullQuery;

		}
		else
		{			
			
			strQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE MANAGE_CODE = '%s' AND VALUE_NAME = 'å�η�����ϱ��и��'"), strTargetManageCode);
			pDM->GetOneValueQuery(strQuery, strRegCodeList);
			if(_T(",") == strRegCodeList)
			{
				AfxMessageBox(_T("å�η� ��û������ ��ϱ����� �����ϴ�."));
				return 1;
			}
			
			strQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE MANAGE_CODE = '%s' AND VALUE_NAME = 'å�η�����ڷ�Ǹ��'"), strTargetManageCode);
			pDM->GetOneValueQuery(strQuery, strLocCodeList);
			if(_T(",") == strLocCodeList)
			{
				AfxMessageBox(_T("å�η� ��û������ �ڷ���� �����ϴ�."));
				return 1;
			}
			
			strQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE MANAGE_CODE = '%s' AND VALUE_NAME = 'å�η���뺰ġ��ȣ���'"), strTargetManageCode);
			pDM->GetOneValueQuery(strQuery, strSeparateCodeList);
			if(_T(",") == strSeparateCodeList)
			{
				AfxMessageBox(_T("å�η� ��û������ ��ġ��ȣ�� �����ϴ�."));
				return 1;
			}
			if(_T("") != strRegCodeList)
			{
				strRegCodeList.TrimLeft();
				strRegCodeList.TrimRight();			
				strRegCodeList.Replace(_T(", "), _T(","));
				strRegCodeList.TrimRight(_T(","));	
				strRegCodeList.Replace(_T(","), _T("','"));
				strRegCodeList = _T("'") + strRegCodeList + _T("'");
				if(!sWhere.IsEmpty())sWhere += _T(" AND ");
				strAddQuery.Format(_T(" BB.REG_CODE IN (%s) "), strRegCodeList);
				sWhere += strAddQuery;

			}
			if(_T("") != strLocCodeList)
			{
				strLocCodeList.TrimLeft();
				strLocCodeList.TrimRight();			
				strLocCodeList.Replace(_T(", "), _T(","));
				strLocCodeList.TrimRight(_T(","));	
				strLocCodeList.Replace(_T(","), _T("','"));
				strLocCodeList = _T("'") + strLocCodeList + _T("'");
				if(!sWhere.IsEmpty())sWhere += _T(" AND ");
				strAddQuery.Format(_T(" BB.SHELF_LOC_CODE IN (%s) "), strLocCodeList);
				sWhere += strAddQuery;

			}
			if(_T("") != strSeparateCodeList)
			{
				strSeparateCodeList.TrimLeft();
				strSeparateCodeList.TrimRight();			
				strSeparateCodeList.Replace(_T(", "), _T(","));
				strSeparateCodeList.TrimRight(_T(","));	
				strSeparateCodeList.Replace(_T(","), _T("','"));
				strSeparateCodeList = _T("'") + strSeparateCodeList + _T("'");
				if(!sWhere.IsEmpty())sWhere += _T(" AND ");
				
				if ( strSeparateCodeList.Find(_T("�������")) >= 0 ) {
					strAddQuery.Format(_T(" ( BB.SEPARATE_SHELF_CODE IN (%s) OR TRIM(BB.SEPARATE_SHELF_CODE) IS NULL ) "), strSeparateCodeList);					
				}
				else {
					strAddQuery.Format(_T(" BB.SEPARATE_SHELF_CODE IN (%s) "), strSeparateCodeList);
				}				
				sWhere += strAddQuery;
				
			}
			if(_T("") != strAddQuery)
			{					
				strAddQuery += _T(" AND BB.MANAGE_CODE = '") + strTargetManageCode + _T("' ");
				CString strTemp;
				strTemp = strAddQuery;
				
			}
			else if(_T("") == strAddQuery)
			{
				strAddQuery += _T(" BB.MANAGE_CODE = '") + strTargetManageCode + _T("' ");
				CString strTemp;
				strTemp = strAddQuery;		

			}
		}
	}

	
	CString strISBN, strISBNWhere;
	strISBNWhere = _T("");

	GetControlData(sCMAlias, _T("ISBN"), strISBN);
	strISBN.Remove(_T(' '));
	strISBN.Remove(_T('-'));
	strISBN.MakeUpper();

	if ( nMode == 0 ) {
		if(!strISBN.IsEmpty()) {
			CLocCommonAPI::MakeISBNSearchQuery(strISBN, _T("IB.REC_KEY"), strISBNWhere);

			if ( strISBNWhere.GetLength() > 0 ) {
				if(1 < sWhere.GetLength()) {
					sWhere += _T(" AND ");
				}
				
				sWhere += strISBNWhere;
				nSearchField++;
			}
		}
	}

	if( sWhere.IsEmpty() )
	{
		CString sMsg = _T("������ �Է��Ͻð� �˻��Ͻʽÿ�.");
		AfxMessageBox( sMsg );
		return 1;
	}

	if( nSearchField == 0 && (nMode == 0) )
	{
		CString sMsg = _T("�˻� ������ �����Ͽ� �ð��� ���� �ɸ��ų� ����� ����� �������� ���Ҽ� �ֽ��ϴ�. \r\n�˻��Ͻðڽ��ϱ�?");
		ids = MessageBox( sMsg , _T("���") , MB_ICONWARNING | MB_YESNO );
		if( ids == IDNO )
			return 1;
	}
		
	
	
	
	
	pCE = pCM->FindControlElement( _T("�̿��ڿ�˻�") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	
	CButton* pUserSearch = (CButton*)(GetDlgItem( pCE->CTRL_ID ));
	if( pUserSearch == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );

	if( pUserSearch->GetCheck() == 1 )
	{				
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		
		
		
			
		if( 0 == nMode )
			if(pChkILLUse->GetCheck() == 1)
				sWhere += _T(" BB.WORKING_STATUS IN ('BOL112N') ");
			else
				sWhere += _T(" BB.WORKING_STATUS IN ('BOL112N','BOL113O','BOL114O','BOL115O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL217O','BOL218O','BOL411O','BOL511O') ");
		else if( 1 == nMode )
			sWhere += _T(" BB.WORKING_STATUS IN ('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL317O','SEL318O','SEL411O','SEL511O') ");
	}	
	else
	{
		if( 0 == nMode )
		{
			if( !sWhere.IsEmpty() )
				sWhere += _T(" AND ");
			
			
			
			if(pChkILLUse->GetCheck() == 1)
				sWhere += _T(" BB.WORKING_STATUS IN ('BOL112N') ");
			else
				sWhere += _T(" BB.WORKING_STATUS NOT IN ('BOT211O','BOT212O') ");				
		}
	}
	


	sWhere += _T(" AND BB.REG_NO IS NOT NULL ") ;

	
	
   	CString sComboGetData2;
  	ids = GetControlData(sCMAlias, _T("��������"), sComboGetData2);
  	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
  	sComboGetData2.TrimRight();
  	if( sComboGetData2.IsEmpty() || (sComboGetData2.Compare(_T("�������"))==0) )
 	{
		if(_T("D") == m_sBookCooperativeMode)
		{
			AfxMessageBox(_T("å�� ���������� �����Ͽ� �ֽʽÿ�."));
			return 1;
		}
 	}
 	else
 	{ 	 		

		sWhere = CString(_T("BB.MANAGE_CODE = '")) + sComboGetData2 + CString(_T("' AND ")) + sWhere;
 	}
	
	

	return 0;
}


void CILL_SEARCH_BOOK::InitSearchBookAppendixFlag()
{
	m_strAppendixFlag = _T("");
}

void CILL_SEARCH_BOOK::GetSearchBookAppendixFlag(CString& strFlag)
{
	strFlag = m_strAppendixFlag;
}