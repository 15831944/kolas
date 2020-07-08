// DonatorNameGetter_Search.cpp : implementation file
//

#include "stdafx.h"
#include "DonatorNameGetter_Search.h"
#include "../K2UP_Common/MakeSearchData.h"
#include "../K2UP_Common/MulitComboBoxManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetter_Search dialog


CDonatorNameGetter_Search::CDonatorNameGetter_Search(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonatorNameGetter_Search::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorNameGetter_Search)
	m_nSearchType = -1;
	m_bDataSortYN = FALSE;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_bCreate = FALSE;
	pMakeSearchData = new CMakeSearchData(pParent); 
	m_pSortItem = NULL;
}

CDonatorNameGetter_Search::~CDonatorNameGetter_Search()
{
	if (pMakeSearchData)
	{
		delete pMakeSearchData; 
		pMakeSearchData = NULL; 
	}
}


VOID CDonatorNameGetter_Search::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorNameGetter_Search)
	DDX_Radio(pDX, IDC_radID, m_nSearchType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorNameGetter_Search, CDialog)
	//{{AFX_MSG_MAP(CDonatorNameGetter_Search)
	ON_BN_CLICKED(IDC_radID, OnradID)
	ON_BN_CLICKED(IDC_radNAME, OnradNAME)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetter_Search message handlers


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - _T("������ �˻�â")�� �ʱ�ȭ �Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. SM/CM/DM ����
// 2. �˻�Ÿ�Կ� ���� ���ҽ��� ǥ���Ѵ�.
//		- �⺻���� 0 : ID�� �˻��̴�.
// 3. ������ OpenMode�� 0���� ������ ���ĺκп� ���ҽ��� Disable ��Ų��.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CDonatorNameGetter_Search::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//=======================================================================
	// 1. SM/CM/DM ����
	//=======================================================================
	if( InitESL_Mgr(_T("K2UPGRADE_�����ڰ˻�")) < 0 )
	{
		ESLAfxMessageBox( _T("InitESL_Mgr Error : K2UPGRADE_�����ڰ˻�") );
		return FALSE;
	}
	m_pCM = FindCM(_T("CM_�����ڰ˻�"));
	m_pDM = FindDM(_T("DM_�����ڰ���"));
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox( _T("[CM_�����ڰ˻�]�� ������ �� �����ϴ�.") );
		return FALSE;
	}
	if( m_pDM == NULL )
	{
		ESLAfxMessageBox( _T("[DM_�����ڰ���]�� ������ �� �����ϴ�.") );
		return FALSE;
	}

	
	//=======================================================================
	// 2. �˻�Ÿ�Կ� ���� ���ҽ��� ǥ���Ѵ�.
	//		- �⺻���� 0 : ID�� �˻��̴�.
	//=======================================================================
	m_nSearchType = 0;
	UpdateData(FALSE);
	SetConditionContrl();

	//=======================================================================
	// 3. ������ OpenMode�� 0���� ������ ���ĺκп� ���ҽ��� Disable ��Ų��.
	//=======================================================================
	if(m_nOpenMode < 0)
	{
		GetDlgItem(IDC_chkSORT_YN)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSET_SORT_CONDITION)->EnableWindow(FALSE);
	}

	//�����ڱ��� ��ȣ�������� �� �� �ֵ��� �߰�
	/*
	CString arrDonatorType[2] = {_T("PN"),_T("GR")};	
	CMulitComboBoxManager mgr(this);
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("PN"), arrDonatorType, _T("�����ڱ���"), 2);
	*/

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �˻�Ÿ�Կ� ���� ���ҽ��� ǥ���Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. �˻�Ÿ�Կ� ���� ���ҽ��� ǥ�ÿ��θ� �����Ѵ�.
//		- 0 : ID�� �˻�
//		- 1 : �̸����� �˻�
// 2. ������ ���ҽ��� ǥ�ÿ��η� ȭ�鿡 ǥ���Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
VOID CDonatorNameGetter_Search::SetConditionContrl()
{
	//=======================================================================
	// ��������
	//=======================================================================
	CWnd *pWnd;
	INT i;
	UINT wndID[ 6 ] = { IDC_cmbDONATOR_CLASS, 
						IDC_edtDONATOR_REG_YEAR, 
						IDC_edtDONATOR_SERIALNO1, 
						IDC_edtDONATOR_SERIALNO2, 
						IDC_edtDONATOR_NAME1, 
						IDC_edtDONATOR_NAME2 };
	bool wndOption[ 6 ] = { false, false, false, false, false, false };
	
	UpdateData(TRUE);
	//=======================================================================
	// 1. �˻�Ÿ�Կ� ���� ���ҽ��� ǥ�ÿ��θ� �����Ѵ�.
	//		- 0 : ID�� �˻�
	//		- 1 : �̸����� �˻�
	//=======================================================================
	switch( m_nSearchType )
	{
		case 0 :
		{
			for( i=0; i<4; i++ )	wndOption[ i ] = true;
		}
			break;
		case 1 :
		{
			for( i=4; i<6; i++ )	wndOption[ i ] = true;
		}
			break;
	default :		return;
			break;
	}


	//=======================================================================
	// 2. ������ ���ҽ��� ǥ�ÿ��η� ȭ�鿡 ǥ���Ѵ�.
	//=======================================================================
	for( i=0; i<6; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->EnableWindow( wndOption[ i ] );
	}
	//=====================================================
	//2009.01.13 ADD BY PJW : ������ �����ڵ�� ������ �ϼ������� �Ѵ�.
	//2009.02.03 REM BY PJW : ������ �����ڵ�� MANAGE_CODE �� ������ �ּ�ó����
// 	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_cmbDONATOR_CLASS);
// 	pEdit->EnableWindow(FALSE);
	//=====================================================
}

VOID CDonatorNameGetter_Search::OnradID() 
{
	SetConditionContrl();
}

VOID CDonatorNameGetter_Search::OnradNAME() 
{
	SetConditionContrl();
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���ǿ� �ش��ϴ� �����ڸ� �˻��Ѵ�.
//
// [ �̴Ͻ��� ]
//
/////////////////////////////////////////////////////////////////////////////
VOID CDonatorNameGetter_Search::OnOK() 
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT ids, nTmpData = -1;
	CString strQuery, strOption, strTmpData, strTmpData1, strData;	

	
	//=======================================================================
	// 1. �˻�Ÿ�Կ� ���� �������� �ۼ��Ѵ�.
	//		- 0 : ID�� �˻�
	//		- 1 : �̸����� �˻�
	//=======================================================================	
	switch( m_nSearchType )
	{
	case 0 :	// ID �˻�
		{
			// �����ڱ��� �����߰�
			m_pCM->GetControlMgrData(_T("�����ڱ���"), strTmpData);
			if( strTmpData.IsEmpty() == FALSE )
			{
				strOption.Format(_T("DONATOR_FLAG = '%s' AND "), strTmpData);			
				strQuery += strOption;
			}				
			
			// ��Ͽ��� ���� �߰�
			strOption.Empty();		strTmpData.Empty();
			m_pCM->GetControlMgrData(_T("��Ͽ���"), strTmpData);
			if( strTmpData.IsEmpty() == FALSE )		
			{
				strOption.Format(_T("YEAR = '%s' AND "), strTmpData);
				strQuery += strOption;
			}
			
			// �Ϸù�ȣ �����߰�
			strOption.Empty();		strTmpData.Empty();		strTmpData1.Empty();
			m_pCM->GetControlMgrData(_T("�Ϸù�ȣ1"), strTmpData);
			m_pCM->GetControlMgrData(_T("�Ϸù�ȣ2"), strTmpData1);
			if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == FALSE )
			{
				strOption.Format(_T("SERIAL_NUM BETWEEN %s AND %s AND "), strTmpData, strTmpData1);
				strQuery += strOption;
			}
			else if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == TRUE )
			{
				strOption.Format(_T("SERIAL_NUM = %s AND "), strTmpData);
				strQuery += strOption;
			}
			else if( strTmpData.IsEmpty() == TRUE && strTmpData1.IsEmpty() == FALSE )
			{
				ESLAfxMessageBox(_T("���� �Ϸù�ȣ�� ������ �ֽʽÿ�."));
				return;	
			}
			else
			{
				
			}
		}
		break;

	case 1 :	// �̸��˻�
		{
			m_pCM->GetControlMgrData(_T("�����ڸ�1"), strTmpData);
			m_pCM->GetControlMgrData(_T("�����ڸ�2"), strTmpData1);
			if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == FALSE && strTmpData1 == strTmpData )
			{
				pMakeSearchData->GetKeyWord(strTmpData);
				strOption.Format( _T("INDEX_DONATOR = '%s' AND "), strTmpData );
				strQuery += strOption;
			}
			else if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == FALSE && strTmpData1 != strTmpData )
			{
				pMakeSearchData->GetKeyWord(strTmpData);
				pMakeSearchData->GetKeyWord(strTmpData1);
				strOption.Format( _T("INDEX_DONATOR BETWEEN '%s' AND '%s' AND "), strTmpData, strTmpData1 );
				strQuery += strOption;
			}
			else if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == TRUE )
			{
				pMakeSearchData->GetKeyWord(strTmpData);
				strOption.Format( _T("INDEX_DONATOR BETWEEN '%s' AND '%s��' AND "), strTmpData, strTmpData );
				strQuery += strOption;
			}
			else if( strTmpData.IsEmpty() == TRUE && strTmpData1.IsEmpty() == FALSE )
			{
				ESLAfxMessageBox(_T("���� �����ڸ��� �Է��� �ֽʽÿ�."));
				return ;
			}
			else ; // ������ �߰����� ����

		}
			break;

	default :	return;
			break;
	}
	

	//=======================================================================
	// 2. 1������ �˻��� �������� ������� �������� �������� �߰��Ѵ�.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("�����1"), strTmpData);
	m_pCM->GetControlMgrData(_T("�����2"), strTmpData1);
	//===============================================================
	//20080522 UPDATE BY PJW : ��¥ �˻� ����.
	/*
	if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == FALSE && strTmpData1 == strTmpData )
	{
		strOption.Format(_T("INPUT_DATE = '%s' "), strTmpData );
		strQuery += strOption;
	}
	else if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == FALSE && strTmpData1 != strTmpData )
	{
		strOption.Format( _T("INPUT_DATE BETWEEN '%s' AND '%s' "), strTmpData, strTmpData1 );
		strQuery += strOption;
	}
	else if( strTmpData.IsEmpty() == FALSE && strTmpData1.IsEmpty() == TRUE )
	{
		strOption.Format( _T("INPUT_DATE = '%s' "), strTmpData );
		strQuery += strOption;
	}
	else if( strTmpData.IsEmpty() == TRUE && strTmpData1.IsEmpty() ==FALSE )
	{
		ESLAfxMessageBox(_T("���� ������� �Է��� �ֽʽÿ�."));
		return ;
	}
	*/
	if( !strTmpData.IsEmpty() && !strTmpData1.IsEmpty() )
	{
		strOption.Format(_T("INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s', 'YYYY/MM/DD')+0.99999 "), strTmpData, strTmpData1 );		
		strQuery += strOption;
	}
	else if( !strTmpData.IsEmpty() )
	{
		strOption.Format(_T("INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s', 'YYYY/MM/DD')+0.99999 "), strTmpData, strTmpData );
		strQuery += strOption;
	}
	else if( !strTmpData1.IsEmpty() )
	{
		strOption.Format(_T("INPUT_DATE BETWEEN TO_DATE('%s','YYYY/MM/DD') AND TO_DATE('%s', 'YYYY/MM/DD')+0.99999 "), strTmpData1, strTmpData1 );
		strQuery += strOption;
	}	
	//===============================================================

	if( _T("AND ") == strQuery.Right( 4 ) )
		strQuery = strQuery.Left( strQuery.GetLength() - 4 );

	//=====================================================
	//2009.01.12 ADD BY PJW : ������ �˻��Ұ�� MANAGE_CODE�� ���ǿ� �ִ´�.
	//2010.05.24 ADD BY PJW : �������븮 �䱸�������� �����ڰ˻��� MANAGE_CODE������
//	if( strQuery.IsEmpty() )
//	{
//		strQuery = _T("MANAGE_CODE = UDF_MANAGE_CODE");
//	}
//	else
//	{
//		strQuery += _T(" AND MANAGE_CODE = UDF_MANAGE_CODE");
//	}
	//=====================================================
	

	if( m_nOpenMode < 0 )
	{
		ids = m_pDM->RefreshDataManager(strQuery);
		if( ids < 0 ) 
		{
			ESLAfxMessageBox(_T("������������ �˻��� �� �����ϴ�."));
			return ;
		}

		//===================================================
		//2009.03.09 ADD BY PJW : ����å���� ����å���� ���� UDF_å���� �ִ´�.
		INT nDmRowCnt;
		INT nSumBookCnt;
		CString strSumBookCnt;
		nDmRowCnt = m_pDM->GetRowCount();
		for ( INT i = 0 ; i < nDmRowCnt ; i++ )
		{
			nSumBookCnt = _ttoi(m_pDM->GetCellData(_T("����å��"), i ));
			nSumBookCnt += _ttoi(m_pDM->GetCellData(_T("����å��"), i ));
			strSumBookCnt.Format( _T("%d"), nSumBookCnt );
			m_pDM->SetCellData(_T("UDF_å��"),strSumBookCnt, i );
		}
		//===================================================

		CDialog::OnOK();
	}
	else
	{
		m_strQuery = strQuery;
		::PostMessage(pParentMgr->GetSafeHwnd(), DONATOR_QUERY_CREATE, 0, 0);
	}
}

BOOL CDonatorNameGetter_Search::Create(CWnd *pParenWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParenWnd);
}

BOOL CDonatorNameGetter_Search::GetCreate()
{
	return m_bCreate;
}

CString CDonatorNameGetter_Search::GetWhereStmt()
{
	return m_strQuery;
}

BOOL CDonatorNameGetter_Search::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		if (GetDlgItem(IDCANCEL)->m_hWnd != pMsg->hwnd)
		{
			GetDlgItem(IDOK)->SetFocus();
			PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		}
		else
		{
			GetDlgItem(IDCANCEL)->SetFocus();
			PostMessage(WM_COMMAND, IDCANCEL, BN_CLICKED);
		}
		return TRUE;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CDonatorNameGetter_Search::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	if(m_pSortItem == NULL) return;
	CDataSortDlg dlg(this);
	dlg.SetSortItemSet(m_pSortItem);
	dlg.SetOpenMode(-1);
	dlg.DoModal();	
}

VOID CDonatorNameGetter_Search::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton*)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;
}

HBRUSH CDonatorNameGetter_Search::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
