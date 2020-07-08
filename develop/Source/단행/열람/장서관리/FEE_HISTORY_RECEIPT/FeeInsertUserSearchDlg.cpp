// FeeInsertUserSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FeeInsertUserSearchDlg.h"
#include "..\..\..\..\����\IndexToolKit\Index.h"
#include "imm.h"
// 2011.09.07 ADD : 3000Api
#include "BO_LOC_3000Api.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeInsertUserSearchDlg dialog


CFeeInsertUserSearchDlg::CFeeInsertUserSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFeeInsertUserSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeInsertUserSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFeeInsertUserSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeInsertUserSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeInsertUserSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CFeeInsertUserSearchDlg)
	ON_BN_CLICKED(IDC_btnInsertUserSearchClose, OnbtnInsertUserSearchClose)
	ON_BN_CLICKED(IDC_btnInsertUserSearchSearch, OnbtnInsertUserSearchSearch)
	ON_EN_SETFOCUS(IDC_edtInsertUserSearchUserName1, OnSetfocusedtInsertUserSearchUserName1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeInsertUserSearchDlg message handlers

BOOL CFeeInsertUserSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// XP THEME
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	if( InitESL_Mgr( _T("FEE_INSERT_USER_SEARCH") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_INSERT_USER_SEARCH") );
		return FALSE;
	}

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtInsertUserSearchCivilNo);
	pEdit->SetLimitText(13);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFeeInsertUserSearchDlg::OnbtnInsertUserSearchClose() 
{
	OnCancel();
}

void CFeeInsertUserSearchDlg::OnbtnInsertUserSearchSearch() 
{
	INT ids;
	CString strWhere;
	ids = GetSearchWhereSql(strWhere);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_VOID2( -1000, _T("OnbtnInsertUserSearchSearch") );
		return;
	}
	else if(1 == ids)
	{
		return;
	}

	ids = SearchLoanUser(strWhere);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_VOID2( -1010, _T("OnbtnInsertUserSearchSearch") );
		return;
	}

	ids = CodeToDesc();
	if( ids < 0 ) 
	{
		ERROR_MSG_RETURN_VOID2( -1020, _T("OnbtnInsertUserSearchSearch") );
		return;
	}

	ids = AllControlDisplay( _T("CM_FEE_INSERT_USER") );
	if( ids < 0 ) 
	{
		ERROR_MSG_RETURN_VOID2( -1030, _T("OnbtnInsertUserSearchSearch") );
		return;
	}
}

INT CFeeInsertUserSearchDlg::GetSearchWhereSql( CString &strWhere )
{
	INT ids;

	INT nSFieldCnt = 0;
	
	strWhere.Empty();

	const INT nControlCnt = 5;
	TCHAR *sControlAlias[nControlCnt] =
	{
		_T("�����ڹ�ȣ1"),	_T("�����ڹ�ȣ2"), 
		_T("�������̸�1"),	_T("�������̸�2"),
		_T("�ֹε�Ϲ�ȣ")
	};
	
	CString sGetData[ nControlCnt ];

	// Ư�� ���� ����
	CIndex index;
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		ids = GetControlData( _T("CM_FEE_INSERT_USER_SEARCH"), sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1000, _T("GetSearchWhereSql") );
		}
		sGetData[i].TrimRight();
		index.RemoveSpace( sGetData[i] );
		index.RemoveChar( sGetData[i], '\'' );
	}

	// �����ڹ�ȣ
	if( !sGetData[0].IsEmpty() )
	{
		if( strWhere.GetLength() > 1 )
		{
			strWhere += _T(" AND ");
		}
		if( !sGetData[1].IsEmpty() )
		{
			strWhere += _T(" USER_NO BETWEEN '") + sGetData[0] + _T("' AND '") + sGetData[1]+ _T("' ");
		}
		else
		{
			strWhere += _T(" USER_NO = '") + sGetData[0] + _T("' ");
		}

		nSFieldCnt++;
	}

	// �����ڸ�
	if( !sGetData[2].IsEmpty() )
	{
		if( strWhere.GetLength() > 1 ) 
		{
			strWhere += _T(" AND ");
		}
		if( !sGetData[3].IsEmpty() )
		{
			strWhere += _T(" IDX_USER_NAME BETWEEN '") + sGetData[2] + _T("' AND '") + sGetData[3]+ _T("' ");
		}
		else
		{
			strWhere += _T(" IDX_USER_NAME BETWEEN '") + sGetData[2] + _T("' AND '") + sGetData[2]+ _T("��' ");
		}
		nSFieldCnt++;
	}
	
	// �ֹε�Ϲ�ȣ
	CString strBuf, strTmpWhere;
	if( !sGetData[4].IsEmpty() )
	{
		if( strWhere.GetLength() > 1 ) 
		{
			strWhere += _T(" AND ");
		}
		
		sGetData[4];
		TCHAR cTmp;
		for( INT j = 0; j < sGetData[4].GetLength(); j++ )
		{
			cTmp = sGetData[4].GetAt( j );
			if( cTmp > 47 && cTmp < 58 )
				strBuf += cTmp;
		}

		if(_T("Y")==m_sIsUnityLoanService)
		{
			if(13!=strBuf.GetLength())
			{
				AfxMessageBox(_T("�߸��� �ֹε�Ϲ�ȣ�Դϴ�.\r\n")
							  _T("�ùٸ� �ֹε�Ϲ�ȣ�� �Է����ֽʽÿ�."));
				GetDlgItem(IDC_edtInsertUserSearchCivilNo)->SetFocus();
				return 1;
			}
		}
		
		if( 13 == strBuf.GetLength() )
		{
			if(_T("Y")==m_sIsUnityLoanService)
			{
				CString strCivilNo;
				strCivilNo.Format( _T("%s-%s"), strBuf.Left(6), strBuf.Right(7) );
				this->ESL_DES_Encryption(strCivilNo,strCivilNo);				
				strTmpWhere.Format( _T(" CRYPTO_CIVIL_NO = '%s' "), strCivilNo );
			}
			else
			{
				strTmpWhere.Format( _T(" CIVIL_NO IN( '%s', '%s-%s' ) "), strBuf, strBuf.Left( 6 ), strBuf.Right( 7 ) );
			}
		}
		else
		{
			strTmpWhere.Format( _T(" CIVIL_NO LIKE '%s%%' "), strBuf );
		}

		strWhere += strTmpWhere;

		nSFieldCnt++;
	}

	// �˻����� �������� ���޽���
	if( nSFieldCnt == 0 )
	{
		ids = AfxMessageBox(_T("�˻� ������ �Էµ��� �ʾ� �˻� �ð��� ���� �ɸ� �� �ֽ��ϴ�.\r\n")
							_T("�����ϰ� �˻��Ͻðڽ��ϱ�? \r\n( �� : �˻� , �ƴϿ� : �˻� ���� ) "), 
							MB_ICONWARNING | MB_YESNO );
		if( ids == IDNO )
		{
			return 1;
		}
	}

	return 0;
}

INT CFeeInsertUserSearchDlg::SearchLoanUser(CString strWhere)
{
	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_INSERT_USER") );
	if( pDM == NULL ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1000, _T("SearchLoanUser") );
	}

	ids = pDM->RefreshDataManager( strWhere );
	if( ids < 0 ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010, _T("SearchLoanUser") );
	}

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox(_T("���ǿ� ��ġ�ϴ� �̿��ڰ� �������� �ʽ��ϴ�."));
	}

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strEnc);
			asDes1.Add(strEnc);
			pDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes2.Add(strEnc);
			pDM->GetCellData(_T("������ȭ"), i, strEnc);
			asDes3.Add(strEnc);
			pDM->GetCellData(_T("���ּ�"), i, strEnc);
			asDes4.Add(strEnc);
			pDM->GetCellData(_T("�ڵ���"), i, strEnc);
			asDes5.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes4);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket())
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					pApi->CloseSocket();
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("E_MAIL"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("������ȭ"), asDes3.GetAt(i), i);
					pDM->SetCellData(_T("���ּ�"), asDes4.GetAt(i), i);
					pDM->SetCellData(_T("�ڵ���"), asDes5.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
	}

	return 0;
}

INT CFeeInsertUserSearchDlg::CodeToDesc()
{
	INT ids;
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_INSERT_USER") );
	if(NULL==pDM) ERROR_MSG_RETURN_ERROR_CODE2( -1000, _T("CodeToDesc") );
	
	CString sCodeData, sCode, sDesc;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		// ��������
		ids = pDM->GetCellData( _T("ȸ����������"), i, sCode );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1010, _T("CodeToDesc") );
		ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("��������"), sCode, sDesc );
		if(0>ids && -2004!=ids) ERROR_MSG_RETURN_ERROR_CODE2( -1020, _T("CodeToDesc") );
		ids = pDM->SetCellData( _T("ȸ���������м���"), sDesc, i );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1030, _T("CodeToDesc") );

		// ������������
		sCode.Empty(); sDesc.Empty();
		ids = pDM->GetCellData( _T("������������"), i, sCode );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1040, _T("CodeToDesc") );
		ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("������������"), sCode, sDesc );
		if(0>ids && -2004!=ids) ERROR_MSG_RETURN_ERROR_CODE2( -1050, _T("CodeToDesc") );
		ids = pDM->SetCellData( _T("����������������"), sDesc, i );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1060, _T("CodeToDesc") );

		// ����Ҽ�����
		sCode.Empty(); sDesc.Empty();
		ids = pDM->GetCellData( _T("����Ҽ�����"), i, sCode );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1070, _T("CodeToDesc") );
		ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("����Ҽ�����"), sCode, sDesc );
		if(0>ids && -2004!=ids) ERROR_MSG_RETURN_ERROR_CODE2( -1080, _T("CodeToDesc") );
		ids = pDM->SetCellData( _T("����Ҽ���������"), sDesc, i );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1090, _T("CodeToDesc") );

		// ȸ������
		sCode.Empty(); sDesc.Empty();
		ids = pDM->GetCellData( _T("ȸ������"), i, sCode );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1100, _T("CodeToDesc") );
		if( sCode.IsEmpty() ) continue;

		if( sCode == _T("0") )
		{
			sDesc = _T("����ȸ��");
		}
		else if( sCode == _T("1") )
		{
			sDesc = _T("��������");
		}
		else if( sCode == _T("2") )
		{
			sDesc = _T("����ȸ��");
		}
		else if( sCode == _T("3") )
		{
			sDesc = _T("Ż��ȸ��");
		}
		else
		{
			sDesc = _T("");
		}
		
		ids = pDM->SetCellData( _T("ȸ�����¼���"), sDesc, i );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1110, _T("CodeToDesc") );

		// �����ڹ�ȣ : �ֹι�ȣ�϶� *ǥ��
		sCode.Empty(); sDesc.Empty();
		ids = pDM->GetCellData( _T("�����ڹ�ȣ"), i, sCode );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1070, _T("CodeToDesc") );
		// 2010.09.08 ADD BY KSJ : ���յ������� ��� �� ��� ǥ�ù�� ����
//DEL 		CString strBuf = sCode;
//DEL 		if( 13 <= sCode.GetLength() )
//DEL 		{
//DEL 			strBuf = sCode.Mid(0,6) + _T("-") + sCode.Mid(6);
//DEL 		}
//DEL 		ids = CLocCommonAPI::CivilNoCheck( strBuf );
//DEL 		if( 0 == ids )
//DEL 		{
//DEL 			sDesc.Format(_T("%s**%s******"),strBuf.Left(4),strBuf.Mid(7,1));
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			sDesc = sCode;
//DEL 		}
		CLocCommonAPI::ChangeSecuriyUserNo( this , sCode , m_sIsUnityLoanService );
		ids = pDM->SetCellData( _T("�����ڹ�ȣ����"), sCode, i );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1090, _T("CodeToDesc") );
	}

	return 0;
}

void CFeeInsertUserSearchDlg::OnSetfocusedtInsertUserSearchUserName1() 
{
	// �������̸��� �ѱ��Է��� �⺻ 
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtInsertUserSearchUserName1)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_HANGUL;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
        ImmReleaseContext(GetDlgItem(IDC_edtInsertUserSearchUserName1)->m_hWnd,hIME); 
    }
}

BOOL CFeeInsertUserSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( WS_CHILD == (GetStyle() & WS_POPUP) )
	{
		if( VK_RETURN == pMsg->wParam )
		{
			if( WM_KEYDOWN == pMsg->message || WM_CHAR == pMsg->message )
			{
				if( GetDlgItem(IDC_edtInsertUserSearchUserNo1)->GetSafeHwnd() == pMsg->hwnd ||
					GetDlgItem(IDC_edtInsertUserSearchUserNo2)->GetSafeHwnd() == pMsg->hwnd ||
					GetDlgItem(IDC_edtInsertUserSearchUserName1)->GetSafeHwnd() == pMsg->hwnd ||
					GetDlgItem(IDC_edtInsertUserSearchUserName2)->GetSafeHwnd() == pMsg->hwnd ||
					GetDlgItem(IDC_edtInsertUserSearchCivilNo)->GetSafeHwnd() == pMsg->hwnd || 
					GetDlgItem(IDC_btnInsertUserSearchSearch)->GetSafeHwnd() == pMsg->hwnd )
				{
					OnbtnInsertUserSearchSearch();
				}
				return TRUE;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
