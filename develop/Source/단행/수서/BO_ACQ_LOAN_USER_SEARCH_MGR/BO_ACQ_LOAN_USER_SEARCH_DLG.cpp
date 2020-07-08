// BO_ACQ_LOAN_USER_SEARCH_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_LOAN_USER_SEARCH_DLG.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_LOAN_USER_SEARCH_DLG dialog


CBO_ACQ_LOAN_USER_SEARCH_DLG::CBO_ACQ_LOAN_USER_SEARCH_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_LOAN_USER_SEARCH_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_LOAN_USER_SEARCH_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentMgr = pParent;
	pMakeSearchData = new CMakeSearchData(pParent);	
	m_pDM = NULL ;
	m_pCM = NULL ;
	m_pGrid = NULL ;
	m_bAutoSearch = FALSE;  // 2005-01-05
	m_sSelectRecKey.Empty();
	m_strUserID.Empty();  // 2005-01-05
	m_nOpenMode = 0;

	// 2012.11.06 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2��
	m_strFurnishCnt = _T("0");
}

// �Ҹ���
CBO_ACQ_LOAN_USER_SEARCH_DLG::~CBO_ACQ_LOAN_USER_SEARCH_DLG()
{
	if (pMakeSearchData)
	{
		delete pMakeSearchData; 
		pMakeSearchData = NULL;
	}

}


VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_LOAN_USER_SEARCH_DLG)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_LOAN_USER_SEARCH_DLG, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_LOAN_USER_SEARCH_DLG)
	ON_BN_CLICKED(IDC_btnSearch, OnbtnSearch)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_LOAN_USER_SEARCH_DLG message handlers

BOOL CBO_ACQ_LOAN_USER_SEARCH_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("K2UPGRADE_�����ڰ�����ȸ")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pDM = FindDM(_T("DM_�����ڰ�����ȸ"));
	m_pCM = FindCM(_T("CM_�����ڰ�����ȸ"));

	m_pGrid = (CESL_Grid*)FindControl( _T("CM_�����ڰ�����ȸ") , _T("MainGrid") );
	if( m_pGrid == NULL ) 
	{
		AfxMessageBox(_T("InitESL_Grid Error!"));
		return FALSE;
	}

	// 2005-01-24 
	// 5163�� ��쿡�� �����ڹ�ȣ�� �ְ� ������������ ��쿡�� �����ڸ� �ִ´�.
	//
	// ���翡���� 5163�� _T("Y")������, �������̸��� �����ڹ�ȣ�� ������ �ʴ´�.
	// 2005-09-28 ������
	//
	CString strIs5163, strIs64;
	GetManageValue(_T("��Ÿ"), _T("����"), _T("5163"), _T("5163"), strIs5163);
	GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T("����"), strIs64);
	//=====================================================
	//2008.11.25 ADD BY PJW : �����ڹ�ȣ �ڵ��˻������ϵ��� �߰�
// 	if( _T("Y") == strIs5163 && _T("Y") != strIs64 )
// 		m_pCM->SetControlMgrData(_T("�����ڹ�ȣ"), m_strUserID);
// 	else
// 		m_pCM->SetControlMgrData(_T("�����ڸ�"), m_strUserID);
	if( _T("Y") == strIs5163 && _T("Y") != strIs64 )
	{
		m_pCM->SetControlMgrData(_T("�����ڹ�ȣ"), m_strUserID);
	}
	else
	{
		m_pCM->SetControlMgrData(_T("�����ڸ�"), m_strUserName);
		m_pCM->SetControlMgrData(_T("�����ڹ�ȣ"), m_strUserID);
	}
	//=====================================================

	if( m_bAutoSearch == TRUE )		OnbtnSearch();

	//===================================================
	//2010.05.17 ADD BY PJW : �����ڹ�ȣ�� ���̾�α� �ʱ�ȭ���� �������� �ʴ´�.
	CEdit* pEdtUserNO = (CEdit*)GetDlgItem(IDC_edtUserNO);
	pEdtUserNO->SetWindowText(_T(""));
	//===================================================

	// 2010.09.03 ADD BY KSJ : ���յ������� ��뿩��
	GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::OnbtnSearch() 
{
	INT ids;
	CString strMsg, sWhere;		
	strMsg = sWhere = _T("");

	// 2007.04.06 update by pdw {{++++++++++++++++++++++++++++++++++++++++++++++++
	// �˻����� ���� Ȯ�� ��� ����
	CString strUserNo, strUserName;
	m_pCM->GetControlMgrData(_T("�����ڹ�ȣ"), strUserNo);
	strUserNo.TrimLeft();strUserNo.TrimRight();
	m_pCM->GetControlMgrData(_T("�����ڸ�"), strUserName);
	strUserName.TrimLeft();strUserName.TrimRight();
	if( strUserNo.GetLength() == 0 && strUserName.GetLength() == 0 )
	{
		ids = AfxMessageBox( _T("�˻� ������ �����Ͽ� �˻� �ð��� ���� �ɸ� �� �ֽ��ϴ�.\r\n")
							 _T("�����ϰ� �˻��Ͻðڽ��ϱ�? \r\n( �� : �˻� , �ƴϿ� : �˻� ���� ) ")
							 , MB_ICONWARNING | MB_YESNO );
		if( ids == IDNO )	return ;
	}
	// ----------------------------------------------------------------------}}
	// sql�� �����
	ids = GetSearchWhereSql( sWhere );
	if( ids < 0 ) return ;

	m_Progress.SetText(_T(""));
	m_Progress.SetPos(0);	
	ids = m_pDM->RefreshDataManager( sWhere, &m_Progress );
	CString strValue1;
	ChangeSecuriyCivilNo(m_pDM,-1,_T("�ֹε�Ϲ�ȣ"));
	GetManageValue( _T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), strValue1 );
	if( _T("Y") != strValue1 )
	{
		// 2010.09.03 ADD BY KSJ : �����Լ��� �����ϵ��� ����
		//DEL ChangeUserNo(m_pDM,_T("�����ڹ�ȣ"));
		CLocCommonAPI::ChangeSecuriyUserNo( this , m_pDM , -1 , _T("�����ڹ�ȣ") , m_sIsUnityLoanService );
	}

	// 2011.09.08 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < m_pDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1, asDes2, asDes3, asDes4;
		for(INT i = 0; i < m_pDM->GetRowCount(); i++)
		{
			m_pDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
			m_pDM->GetCellData(_T("�ٹ�����ȭ"), i, strEnc);
			asDes2.Add(strEnc);
			m_pDM->GetCellData(_T("������ȭ"), i, strEnc);
			asDes3.Add(strEnc);
			m_pDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strEnc);
			asDes4.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes4);
					pApi->CloseSocket();	
				}
				for(i = 0; i < m_pDM->GetRowCount(); i++)
				{
					m_pDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
					m_pDM->SetCellData(_T("�ٹ�����ȭ"), asDes2.GetAt(i), i);
					m_pDM->SetCellData(_T("������ȭ"), asDes3.GetAt(i), i);
					m_pDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), asDes4.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
	}

	m_Progress.SetPos(100);
	strMsg.Format(_T("�˻���� : %d ��"), m_pDM->GetRowCount());
	m_Progress.SetText(strMsg);	
	
	// �Ҽ�, ������ desc �� �����ش�.
	// 2006.01.03 ������ DM/CM����
	//ids = UserCodeToDesc();	
	if( ids >= 0 )	m_pGrid->Display();	
}

INT CBO_ACQ_LOAN_USER_SEARCH_DLG::GetSearchWhereSql( CString &sWhere ) 
{
	// 2007.04.02 add by pdw {{++++++++++++++++++++++++++++++++++++++++++++++++
	// Ż��ȸ�� �˻����� ���ܵǵ��� ����
	// 2007.07.26 UPDATE BY PJW : �̿��� �˻��� �ֹι�ȣ ���������� �Է����� ��� ��ġ������� �˻��ǵ��� ����
	// sWhere += _T(" USER_NO IS NOT NULL AND CIVIL_NO IS NOT NULL ");
	/*	
	CString strQuery, strValue;
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '����' AND GROUP_3 = '����' AND CLASS_ALIAS = '�̿��ڰ���' AND VALUE_NAME = '�ֹε�Ϲ�ȣ�������'"));
	m_pDM->GetOneValueQuery(strQuery, strValue);
	if ( strValue == 'N' )
	{
		sWhere += _T(" USER_NO IS NOT NULL");
	}
	else
	{
		sWhere += _T(" USER_NO IS NOT NULL AND CIVIL_NO IS NOT NULL ");		
	}
	*/
	// 2007.07.26 UPDATE BY PJW : ���κ�ȣ ��å ������ �˻����ǿ��� �ֹι�ȣ ����
	//++2008.05.13 UPDATE BY PWR {{+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//sWhere += _T(" USER_NO IS NOT NULL");
	//=====================================================
	//2008.11.25 UPDATE BY PJW : ���սý��� Ÿ������ �̿��ڵ� ��ġ��û�Ҽ� �������� MANAGE_CODE ���� �Ѵ�.
// 	sWhere += _T(" (CLU.USER_CLASS!='3' OR CLU.MEMBER_CLASS!='1') AND MANAGE_CODE = UDF_MANAGE_CODE");
	sWhere += _T(" (CLU.USER_CLASS!='3' OR CLU.MEMBER_CLASS!='1')");
	//=====================================================
	//=====================================================
	//--2008.05.13 UPDATE BY PWR -----------------------------------------------------}}
	
	// ----------------------------------------------------------------------}}
	// �����ڹ�ȣ
	// 20070618 ADD PJW : ������ ��ȣ 	if( sWhere.GetLength() > 1 )	sWhere += _T(" AND "); �߰�
	CString strData;
	m_pCM->GetControlMgrData(_T("�����ڹ�ȣ"), strData);
	
	if( !strData.IsEmpty() )
	{
		//=====================================================
		//2008.11.25 REM BY PJW : ������ ��ȣ�� KEYWORD�� �������� �ʴ´�.
// 		pMakeSearchData->GetKeyWord(strData);
		//=====================================================

		if( sWhere.GetLength() > 1 )	sWhere += _T(" AND ");
		sWhere += _T(" USER_NO = '") + strData + _T("' ");
	}
	//=====================================================
	// �����ڸ�
	m_pCM->GetControlMgrData(_T("�����ڸ�"), strData);
	if( !strData.IsEmpty() )
	{
		pMakeSearchData->GetKeyWord(strData);
		if( sWhere.GetLength() > 1 )	sWhere += _T(" AND ");
		sWhere += _T(" IDX_USER_NAME LIKE ('") + strData + _T("%') ");		
	}
	

	return 0;
}


VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::OnOK() 
{
	// ���õ� �׸����� rec_key ���� ������� m_sSelectRecKey�� �����Ѵ�.
	INT nRowIdx = -1;
	nRowIdx = m_pGrid->GetIdx();

	CString strSelectRecKey = _T("");
	strSelectRecKey = m_pDM->GetCellData( _T("REC_KEY"), nRowIdx );
	//===================================================
	//2010.11.17 ADD BY PJW : ��ġ������� �߰�
	INT nOpenMode = 0;
	nOpenMode = GetOpenMode();
	if( 1 == nOpenMode && _T("") != strSelectRecKey )
	{
		CString strQuery = _T("");
		CString strLimitPeriod = _T("");
		CString strLimitCnt = _T("");
		CString strReservation = _T("");
		INT nLimitPeriod = 0;
		INT nLimitCnt = 0;
		INT nReservation = 0;
		//1. ���������������Ⱓ ���ϱ�
		strQuery.Format(_T("SELECT NVL(PREENGAGE_CANCEL_LIMIT_PERIOD, 0) FROM CD_CODE_TBL WHERE CLASS=31 AND CODE=(SELECT USER_CLASS_CODE FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s)"), strSelectRecKey);
		m_pDM->GetOneValueQuery(strQuery, strLimitPeriod);
		//2. ����������Ѱ��� ���ϱ�
		strQuery.Format(_T("SELECT NVL(PREENGAGE_CANCEL_LIMIT_COUNT, 0) FROM CD_CODE_TBL WHERE CLASS=31 AND CODE=(SELECT USER_CLASS_CODE FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s)"), strSelectRecKey);
		m_pDM->GetOneValueQuery(strQuery, strLimitCnt);
		//3. ����������� ����Ⱓ������ ��������� �Ǽ� ���ϱ�
		//*���ǻ���:1����2�����α��Ѱ��� �Ѵ� 0�̻��϶��� �����Ѵ�.
		//(1����2�� ���� �ϳ��� 0�̸� �Ʒ� ���� ������ϰ� ���Ѿ���)
		nLimitPeriod = _ttoi(strLimitPeriod);
		nLimitCnt = _ttoi(strLimitCnt);
		if ( 0 < nLimitPeriod && 0 < nLimitCnt )
		{
			CString strSubQuery_1 = _T("");
			CString strSubQuery_2 = _T("");
			strSubQuery_1.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE STATUS='4' AND USER_KEY=%s AND RESERVATION_CANCEL_TYPE='0' AND RESERVE_CANCEL_DATE BETWEEN TO_DATE(SYSDATE-%s, 'YYYY/MM/DD') AND TO_DATE(SYSDATE, 'YYYY/MM/DD')+0.99999"), strSelectRecKey, strLimitPeriod);
			strSubQuery_2.Format(_T("SELECT COUNT(1) FROM CO_LOAN_TBL WHERE STATUS='4' AND USER_KEY=%s AND RESERVATION_CANCEL_TYPE='0' AND RESERVE_CANCEL_DATE BETWEEN TO_DATE(SYSDATE-%s, 'YYYY/MM/DD') AND TO_DATE(SYSDATE, 'YYYY/MM/DD')+0.99999"), strSelectRecKey, strLimitPeriod);
			strQuery.Format(_T("SELECT (%s) + (%s) FROM DUAL"), strSubQuery_1, strSubQuery_2);
			m_pDM->GetOneValueQuery(strQuery, strReservation);
			nReservation = _ttoi(strReservation);
			//4. 3������ ���� ��������� �Ǽ��� ����������Ѱ������� ���ų� ũ�� �����Ѵ�.
			//EX: 3���������ѿ�������ѰǼ� >= 2���������ѿ���������Ѱ��� �̸� ����
			if( nReservation >= nLimitCnt )
			{
				AfxMessageBox(_T("��ġ��� �����û�� ���ѵ� �̿��� �Դϴ�."));
				return;
			}
		}
	}
	//===================================================	
	
	//JOB.2020.0004 ADD BY KYJ : ȸ������ ��ġ�����û ���ѱ�� �߰�
	CString strMemberClass = _T("");
	
	if( m_pInfo->m_bRestictionToApply == TRUE ){
		strMemberClass = m_pDM->GetCellData( _T("ȸ������"), nRowIdx );
		if (strMemberClass != _T("��ȸ��"))
		{
			AfxMessageBox(_T("��ȸ��,��ȸ���� ����ڷ� ��û�� ���ѵ˴ϴ�."));
			return;
		}
	}

	m_sSelectRecKey = strSelectRecKey;
	//job.2018.0071
	//before
	//==============================================================================
	//m_strFurnishCnt = m_pDM->GetCellData( _T("��û�Ǽ�"), nRowIdx );
	//==============================================================================
	//after 
	//==============================================================================
	if ( m_pInfo->m_bFurnishWishLimitYN )
	{
		m_strFurnishCnt = m_pDM->GetCellData( _T("��û�Ǽ�"), nRowIdx );
	}
	else if (  m_pInfo->m_bFurnishLimitByTheWeek )
	{
		m_strFurnishCnt = m_pDM->GetCellData( _T("�̹��ֽ�û�Ǽ�"), nRowIdx );
	}
	//==============================================================================

	if( m_sSelectRecKey.IsEmpty() )
		CDialog::OnCancel();	
	else
		CDialog::OnOK();
}

INT CBO_ACQ_LOAN_USER_SEARCH_DLG::UserCodeToDesc() 
{
	// ����Ҽ����� , ������������
	const INT nCodeCnt = 2;
	TCHAR *sCodeAlias[nCodeCnt] =
	{
		_T("����Ҽ�����") , _T("������������") 
	};
	TCHAR *sFieldAlias[nCodeCnt] =
	{
		_T("����Ҽ�����") , _T("������������") 
	};

	INT ids;
	for( INT i = 0 ; i < nCodeCnt ; i++ )
	{
		ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pDM , sCodeAlias[i] , sFieldAlias[i] );
		if( ids < 0 ) return -1;
	}

	return 0;
}

VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::SetUserID(CString strUserID)
{
	m_strUserID = strUserID;
	if( m_strUserID.IsEmpty() == FALSE )	m_bAutoSearch = TRUE;
}
//=====================================================
//2008.11.25 ADD BY PJW : �����ڹ�ȣ �ڵ��˻������ϵ��� �߰�
VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::SetUserName(CString strUserName)
{
	m_strUserName = strUserName;
	if( m_strUserName.IsEmpty() == FALSE )	m_bAutoSearch = TRUE;
}
//=====================================================
// 2005.01.05 ����Ű�� ġ�� �˻��� �ǵ��� �߰��Ͽ���. �߰��ϴϱ�.. ������?
BOOL CBO_ACQ_LOAN_USER_SEARCH_DLG::PreTranslateMessage(MSG* pMsg) 
{
 	if( pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		// 2006-01-03 ������ �˻��� ����Ʈ�ڽ��� �������� �ؾ��Ѵ�.(��ư�� �ȴ�����)
		//GetDlgItem(IDOK)->SetFocus();
		if( GetDlgItem(IDC_edtUserName)->m_hWnd == pMsg->hwnd ||
			GetDlgItem(IDC_edtUserNO)->m_hWnd == pMsg->hwnd )
		{
			OnbtnSearch();
		//PostMessage(WM_COMMAND, IDC_btnSearch, BN_CLICKED);
		return TRUE;
	}		
		else if( GetDlgItem(IDC_gBO_LOAN_USER)->m_hWnd == pMsg->hwnd )	return TRUE;
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CBO_ACQ_LOAN_USER_SEARCH_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

//===================================================
//2009.10.08 ADD BY LKS : �ֹι�ȣ/�����ڹ�ȣ ġȯ
INT CBO_ACQ_LOAN_USER_SEARCH_DLG::CivilNoCheck( CString sCivilNo )
{
	EFS_BEGIN

	INT ids;
	INT nDashIndex;
	CString sCivilNo1,sCivilNo2;

	if( sCivilNo.GetLength() != 14 )
		return 1;
	
	nDashIndex = sCivilNo.Find(_T("-"));

	if( nDashIndex != 6 ) 
		return 2;

	sCivilNo1 = sCivilNo.Mid(0,nDashIndex);
	sCivilNo2 = sCivilNo.Mid(nDashIndex+1);

	ids = CivilNoCheck( sCivilNo1 , sCivilNo2 );
	if( ids != 0 ) return 3;

	return 0;


	EFS_END
	return -1;

}

INT CBO_ACQ_LOAN_USER_SEARCH_DLG::CivilNoCheck( CString sCivilNo1, CString sCivilNo2 )
{
	EFS_BEGIN

	// 2007.2.9 add by pdw
	// �ڸ��� üũ
	if( 6 != sCivilNo1.GetLength() || 7 != sCivilNo2.GetLength() )
	{
		return -4;
	}

	INT sum, i; 

	sum = 0;

	// ���� ���� üũ
    // 3.��������� ��üũ
    // 3.1.������� üũ
    INT nMonth = _ttoi(sCivilNo1.Mid(2,2));
    if(nMonth < 1 || nMonth > 12)
	{
        return -1;
    }

    // 3.2.�������� üũ
    INT nDay = _ttoi(sCivilNo1.Mid(4,2));
    if(nMonth < 1 || nMonth > 31)
	{
        return -2;
    }
    
    // 4. �������� �������� üũ. 2002.08.06 ������ �߰� --> �ܱ��� �ֹε�� ���� ��ȣ : 5, 6 2005.02.17�����߰�: 7,8
    if( (sCivilNo2.GetAt(0)!='1') && (sCivilNo2.GetAt(0)!= '2') && (sCivilNo2.GetAt(0)!='3') && (sCivilNo2.GetAt(0)!='4') 
		&& (sCivilNo2.GetAt(0)!= '5') && (sCivilNo2.GetAt(0)!= '6') && (sCivilNo2.GetAt(0)!= '7') && (sCivilNo2.GetAt(0)!= '8') )
	{
        return -3;
    }

	if( sCivilNo2.GetAt(0)=='5' || (sCivilNo2.GetAt(0) == '6') || (sCivilNo2.GetAt(0)== '7') || (sCivilNo2.GetAt(0)== '8'))
	{
		INT ids;
		ids = CheckForeignCivilNum( sCivilNo1 + sCivilNo2 );
		if( ids != 0 ) return -1;
	}
	else
	{
		for( i = 0 ; i < 12 ; i++) 
		{ 
			if(i < 6) 
				sum += (sCivilNo1.GetAt(i)-'0') * (i+2); 
			if( i > 5 && i < 8 ) 
				sum += (sCivilNo2.GetAt(i-6)-'0') * (i+2); 
			
			if( i > 7 ) 
				sum += (sCivilNo2.GetAt(i-6)-'0') * (i-6); 
		} 
		
		INT nCheckDigit = 11-(sum%11);
		nCheckDigit = nCheckDigit%10;
		if( nCheckDigit != (sCivilNo2.GetAt(6)-'0') ) 
			return -1;
	}
	
	return 0;

	EFS_END
	return -1;

}

// �ܱ��� �ֹε�� ��ȣ üũ�� �˻�
INT CBO_ACQ_LOAN_USER_SEARCH_DLG::CheckForeignCivilNum( CString civilNum )
{
/*
���� �ܱ��� �ֹ� ��ȣ
641103-6220195
671210-6220196
670124-6100292
660515-6220197
650730-6220192
	*/
	
	// civilNum : �������� ���� ���ӵ� 13�� ����
	
	
	const INT cnt = 13;
	INT num[ cnt ];
	INT sum = 0, odd = 0;
	for( INT i = 0; i < cnt; i++ )
		num[ i ] = _ttoi( civilNum.Mid( i, 1 ) );
	
	odd = num[ 7 ] * 10 + num[ 8 ];
	
	if( odd % 2 != 0 )
		return 1;
	
	if( 
		num[ 11 ] != 6 && 
		num[ 11 ] != 7 && 
		num[ 11 ] != 8 &&
		num[ 11 ] != 9 )
		return 2;
	
	INT checkNum[] = { 2, 3, 4, 5, 6, 7, 8, 9, 2, 3, 4, 5 };
	
	for( i = 0, sum = 0; i < 12; i++ )
		sum += ( num[ i ] *= checkNum[ i ] );
	sum = 11 - ( sum % 11 );
	
	if( sum >= 10 )
		sum -= 10;
	sum += 2;
	
	if( sum >= 10 )
		sum -= 10;
	
	if( sum != num[ 12 ] )
		return 3;
	
	return 0;
}

INT CBO_ACQ_LOAN_USER_SEARCH_DLG::ChangeSecuriyCivilNo( CESL_DataMgr* pDM , INT nIndex , CString sAlias )
{
	EFS_BEGIN

	// 2010.09.13 ADD BY KSJ : �ֹι�ȣ ǥ����� ����
//DEL 	CString sCivilNo;
//DEL 	INT nStartIndex = 0;
//DEL 	INT nFinishIndex = pDM->GetRowCount();
//DEL 	
//DEL 	if( nIndex != -1 )
//DEL 	{
//DEL 		nStartIndex = nIndex;
//DEL 		nFinishIndex = nIndex + 1;
//DEL 	}
//DEL 
//DEL 	for( INT i = nStartIndex ; i < nFinishIndex ; i++ )
//DEL 	{
//DEL 		ids = pDM->GetCellData( sAlias , i , sCivilNo );
//DEL 
//DEL 		ChangeSecuriyCivilNo( sCivilNo );
//DEL 
//DEL 		pDM->SetCellData( sAlias , sCivilNo , i );
//DEL 		
//DEL 	}
	CLocCommonAPI::ChangeSecuriyCivilNo(this,pDM,nIndex,sAlias,m_sIsUnityLoanService);

	return 0;

	EFS_END

	return -1;
}

INT CBO_ACQ_LOAN_USER_SEARCH_DLG::ChangeSecuriyCivilNo( CString &sCivilNo )
{
	EFS_BEGIN

	// 2010.09.13 ADD BY KSJ : �ֹι�ȣ ǥ����� ����
//DEL 	INT nDIndex = sCivilNo.Find(_T("-"));
//DEL 	INT nStartIndex = 6;
//DEL 
//DEL 	if( nDIndex > -1 )
//DEL 	{
//DEL 		nStartIndex = nDIndex + 1;
//DEL 	}
//DEL 
//DEL 	INT nLength = sCivilNo.GetLength();
//DEL 
//DEL 	for( INT i = nStartIndex ; i < nLength ; i++ )
//DEL 	{
//DEL 		sCivilNo.SetAt( i , '*' );
//DEL 	}
	CLocCommonAPI::ChangeSecuriyCivilNo(this,sCivilNo,m_sIsUnityLoanService);

	return 0;

	EFS_END

	return -1;

}

VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::ChangeUserNo( CESL_DataMgr* pDM,CString sAlias)
{
	INT nCnt = pDM->GetRowCount();
	CString strUserNo;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		INT ids = pDM->GetCellData( sAlias , i , strUserNo );

//DEL 		CString strBuf = strUserNo;
//DEL 		if( 13 <= strUserNo.GetLength() )
//DEL 			strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 
//DEL 		ids = CivilNoCheck( strBuf );
//DEL 		if( 0 == ids )
//DEL 		{
//DEL 			// 2009.12.14 ADD BY CJY : �ֹι�ȣ ǥ�ù�� ����
//DEL 			//ids = CLocCommonAPI::ChangeSecuriyCivilNo( NULL, strUserNo );
//DEL 			strUserNo.Format(_T("%s%s******"),strBuf.Left(6),strBuf.Mid(7,1));
//DEL 
//DEL 			pDM->SetCellData( sAlias , strUserNo , i );
//DEL 		}
		CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
		pDM->SetCellData( sAlias , strUserNo , i );
	}
}
//===================================================