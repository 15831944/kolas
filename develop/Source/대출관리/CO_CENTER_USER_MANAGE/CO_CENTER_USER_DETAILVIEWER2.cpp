// CO_CENTER_USER_DETAILVIEWER2.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_DETAILVIEWER2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_DETAILVIEWER2 dialog


CCO_CENTER_USER_DETAILVIEWER2::CCO_CENTER_USER_DETAILVIEWER2(CESL_Mgr* pParent , CString sParentCMAlias , CString sParentGridAlias )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_DETAILVIEWER2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sParentCMAlias = sParentCMAlias;
	m_sParentGridAlias = sParentGridAlias;

	m_pRS_SQL = NULL;
}

CCO_CENTER_USER_DETAILVIEWER2::~CCO_CENTER_USER_DETAILVIEWER2()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
	delete m_pRS_SQL;
}

VOID CCO_CENTER_USER_DETAILVIEWER2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_DETAILVIEWER2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_DETAILVIEWER2, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_DETAILVIEWER2)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnDPREV, OnbtnDPREV)
	ON_BN_CLICKED(IDC_btnDNEXT, OnbtnDNEXT)
	ON_BN_CLICKED(IDC_btnDCLOSE, OnbtnDCLOSE)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabTITLE, OnSelchangetabTITLE)
	ON_BN_CLICKED(IDC_radVIEWER_MODE, OnradVIEWERMODE)
	ON_BN_CLICKED(IDC_radVIEWER_MODE2, OnradVIEWERMODE2)
	ON_BN_CLICKED(IDC_bPRINT, OnbPRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_DETAILVIEWER2 message handlers

BOOL CCO_CENTER_USER_DETAILVIEWER2::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr(_T("CO_CENTER_USER_DETIALVIEWER2"));
	if( ids < 0 )
	{
		AfxMessageBox(_T("Error Init EslMgr[]"));
		return FALSE;
	}
	
	m_pRS_SQL = new CRS_SQL(this);

	// 2009.10.08 ADD BY KSJ : ����ȭ���ֹι�ȣǥ������
	ids = GetManageValue( _T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), m_strSecurityUserNoYN );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ChangeSecuriyUserNo") );

	m_strSecurityUserNoYN.TrimLeft();	m_strSecurityUserNoYN.TrimRight();
	m_strSecurityUserNoYN.MakeUpper();
	if( _T("Y") != m_strSecurityUserNoYN )
	{
		m_strSecurityUserNoYN = _T("N");
	}

	InitControl();

	bFirstView = FALSE;
	SetScreen();
	bFirstView = TRUE;
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CCO_CENTER_USER_DETAILVIEWER2::InitControl()
{
	((CButton*)GetDlgItem(IDC_radVIEWER_MODE))->SetCheck(1);

	m_MasterUserInfoGrid.InitViewer( IDC_gridDCENTERUSER, this );
	m_MasterUserInfoGrid.SetViewerWidth( 35 , 60 );	


	GetDlgItem(IDC_gridDCENTERUSER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_gridDCENTERUSER2)->ShowWindow(SW_HIDE);

	InitTabControl();

	return 0;

}

INT CCO_CENTER_USER_DETAILVIEWER2::InitTabControl()
{
	INT ids;

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTITLE);
	pTab->DeleteAllItems();
	pTab->InsertItem(  0 , _T("��������")  );	
	
	if( ((CButton*)GetDlgItem(IDC_radVIEWER_MODE))->GetCheck() == 1 )
	{
		CESL_DataMgr* pLocalUserDM = FindDM(_T("DM_CO_CENTER_DETAIL_VIEWER2"));
		if( pLocalUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitTabControl") );

		CString sLibName;

		for( INT i = 0 ; i < pLocalUserDM->GetRowCount() ; i++ )
		{
			ids = pLocalUserDM->GetCellData( _T("LIB_CODE") , i , sLibName );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitTabControl") );

			pTab->InsertItem( i+1 , sLibName );
		}
	}
	else
	{
		pTab->InsertItem( 1 , _T("�Ҽ� �������� ������ ����") );	
	}

	pTab->SetCurSel(0);

	return 0;
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	
}



/// �θ� �׸��� ������
CESL_Grid* CCO_CENTER_USER_DETAILVIEWER2::GetMainGridPtr()
{
	CESL_ControlMgr *pCM = FindCM( m_sParentCMAlias );
	if( pCM == NULL ) return NULL;
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(m_sParentGridAlias));
	if( pGrid == NULL ) return NULL;
	
	return pGrid;
}


/// ���� ���� �̵�
INT CCO_CENTER_USER_DETAILVIEWER2::MoveIndex( INT nMode )
{
		
	if( m_nViewMode == 0 )
	{
		if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;
	}
	else
	{
		CESL_Grid *pGrid = GetMainGridPtr();
		if( nMode == 0 )
		{
			m_nCurrentIndex = pGrid->SelectGetPrev();
		}
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
		{
			m_nCurrentIndex = pGrid->SelectGetNext();
		}
	}
	
	SetPreNextButton();
	
	return 0;
	
}

/// ���� �ε����� ó�� �ε��� , ������ �ε��� ����
INT CCO_CENTER_USER_DETAILVIEWER2::SetCurrentIndex()
{
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) return -1;
	
	// ���� ������
	// 'V'üũ�� �� ���� �ִٸ� 'V'üũ �Ȱ͸� �����ָ�
	// 'V'üũ�� �� ���� ���ٸ� ��� �����ش�.
	INT nTmpIndex;
	if( bFirstView ) nTmpIndex = pGrid->GetIdx();
	else nTmpIndex = pGrid->GetIdx();
	
	pGrid->SelectMakeList();
	INT nSelectCount = pGrid->SelectGetCount();
	if( nSelectCount > 0 ) m_nViewMode = 1;
	else m_nViewMode = 0;
	
	if( m_nViewMode == 0 )
	{
		m_nCurrentIndex = pGrid->GetIdx();
		m_nLastIndex = pGrid->GetCount() - 1;
		m_nFirstIndex = 0;
	}
	else
	{
		m_nLastIndex = pGrid->SelectGetTailPosition();
		m_nFirstIndex = pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCount-1 ; i++ )
		{
			if( nTmpIndex == pGrid->SelectGetIdx() )
				break;
			pGrid->SelectGetNext();
		}
		if( i==nSelectCount ) m_nCurrentIndex = pGrid->SelectGetHeadPosition();
		else m_nCurrentIndex = pGrid->SelectGetIdx();
		
	}

	SetPreNextButton();
	
	return 0;
}


/// ����,���� ��ư Ȱ��ȭ ��Ȱ��ȭ
VOID CCO_CENTER_USER_DETAILVIEWER2::SetPreNextButton()
{
	
	GetDlgItem(IDC_btnDPREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDNEXT)->EnableWindow(TRUE);
	
	if( m_nCurrentIndex == m_nFirstIndex )
	{
		GetDlgItem(IDC_btnDPREV)->EnableWindow(FALSE);
	}
	if( m_nCurrentIndex == m_nLastIndex )
	{
		GetDlgItem(IDC_btnDNEXT)->EnableWindow(FALSE);
	}
}


/// ������ ȭ�鿡 ������
INT CCO_CENTER_USER_DETAILVIEWER2::DisplayWindow()
{
	INT ids;

	ids = SearchUserInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayWindow") );

	InitTabControl();

	ids = ShowUserInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DisplayWindow") );

	return 0;
}

INT CCO_CENTER_USER_DETAILVIEWER2::SetScreen()
{

	AfxGetApp()->DoWaitCursor(1);
	
	SetCurrentIndex();
	DisplayWindow();	

	AfxGetApp()->DoWaitCursor(-1);
	
	return 0;
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnbtnDPREV() 
{
	AfxGetApp()->DoWaitCursor(1);

	MoveIndex(0);	
	DisplayWindow();

	AfxGetApp()->DoWaitCursor(-1);
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnbtnDNEXT() 
{
	AfxGetApp()->DoWaitCursor(1);

	MoveIndex(1);	
	DisplayWindow();	

	AfxGetApp()->DoWaitCursor(-1);
	
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnbtnDCLOSE() 
{
	OnCancel();	
}


INT CCO_CENTER_USER_DETAILVIEWER2::SearchUserInfo()
{
	INT ids;

	CString sWhere;
	ids = GetWhere( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchUserInfo") );
	if( ids > 0 ) return 1;

	CESL_DataMgr* pLibUserInfo = FindDM(_T("DM_CO_CENTER_DETAIL_VIEWER2"));
	if( pLibUserInfo == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchUserInfo") );

//	m_pRS_SQL->SetFetchCount( 0, 100 );
//	ids = m_pRS_SQL->LOAN_Select( pLibUserInfo , sWhere , _T("") , 1 , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchUserInfo") );	

	return 0;
}

INT CCO_CENTER_USER_DETAILVIEWER2::ShowUserInfo()
{
	INT ids;
	
	/// ���� �����ֱ�
	ids = ChangeView();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowUserInfo") );


	/// 3. �Ʒ� �׸��忡 �������ϱ�
	CESL_Grid* pParentGrid = GetMainGridPtr();
	if( pParentGrid == NULL ) return 0;

	pParentGrid->SetReverse(m_nCurrentIndex);

	return 0;
}


INT CCO_CENTER_USER_DETAILVIEWER2::GetWhere( CString &sWhere )
{
	CESL_DataMgr* pMasterUserDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );
	if( pMasterUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetWhere") );

	CString sMasterUserKey;
	INT ids = pMasterUserDM->GetCellData( _T("���ڵ�KEY") , m_nCurrentIndex , sMasterUserKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetWhere") );

	sWhere.Format(_T("MASTER_USER_KEY = %s") , sMasterUserKey );

	return 0;
}


INT CCO_CENTER_USER_DETAILVIEWER2::ShowMasterUserInfo()
{
	INT ids;

	const INT nFieldCnt = 34;
	CString sDMFieldAlias[nFieldCnt];

	sDMFieldAlias[  0 ] = _T("�����������̸�");
	sDMFieldAlias[  1 ] = _T("������");
	sDMFieldAlias[  2 ] = _T("���������������̸�");
	sDMFieldAlias[  3 ] = _T("����������");
	sDMFieldAlias[  4 ] = _T("�����ڹ�ȣ");
	sDMFieldAlias[  5 ] = _T("������ID");
	sDMFieldAlias[  6 ] = _T("��й�ȣ");
	sDMFieldAlias[  7 ] = _T("�̸�");
	sDMFieldAlias[  8 ] = _T("�̸�����");
	sDMFieldAlias[  9 ] = _T("�������");
	sDMFieldAlias[ 10 ] = _T("������");
	sDMFieldAlias[ 11 ] = _T("�ֹε�Ϲ�ȣ");
	sDMFieldAlias[ 12 ] = _T("�ּ�_��");
	sDMFieldAlias[ 13 ] = _T("�ּ�_�ٹ���");
	sDMFieldAlias[ 14 ] = _T("�����ȣ_��");
	sDMFieldAlias[ 15 ] = _T("�����ȣ_�ٹ���");
	sDMFieldAlias[ 16 ] = _T("��ȭ��ȣ_��");
	sDMFieldAlias[ 17 ] = _T("��ȭ��ȣ_�ٹ���");
	sDMFieldAlias[ 18 ] = _T("������ȣ");
	sDMFieldAlias[ 19 ] = _T("�ٹ����μ���");
	sDMFieldAlias[ 20 ] = _T("�ٹ������޸�");
	sDMFieldAlias[ 21 ] = _T("�ڵ���");
	sDMFieldAlias[ 22 ] = _T("E-MAIL");
	sDMFieldAlias[ 23 ] = _T("�����");
	sDMFieldAlias[ 24 ] = _T("������");
	sDMFieldAlias[ 25 ] = _T("�б�");
	sDMFieldAlias[ 26 ] = _T("�г�");
	sDMFieldAlias[ 27 ] = _T("��");
	sDMFieldAlias[ 28 ] = _T("����");
	sDMFieldAlias[ 29 ] = _T("����߼�����");
	sDMFieldAlias[ 30 ] = _T("���");
	sDMFieldAlias[ 31 ] = _T("�ڷ���ڵ�");
	sDMFieldAlias[ 32 ] = _T("�̿��ڻ����̸�");
	sDMFieldAlias[ 33 ] = _T("ȸ�����нǿ���");

	m_MasterUserInfoGrid.LIST.RemoveAll();
	m_MasterUserInfoGrid.Clear();
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowMasterUserInfo") );

	CString tmpTitle;
	CString sData;
	for( INT i = 0; i < nFieldCnt; i++ )
	{
		ids = pDM->GetCellData( sDMFieldAlias[i], m_nCurrentIndex, sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowMasterUserInfo") );
		
		switch( i )
		{
			case  6 :
			case  8 :
			case 18 :
			case 31 :
			case 32 :
				continue;
				break;
			case  4 :
				// 2009.10.08 ADD BY KSJ : �����ڹ�ȣ�� �ֹι�ȣ�� ��� ó��
				ChangeSecuriyUserNo( sData );
				m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ], sData );
				break;
			case  5 :
				m_MasterUserInfoGrid.SetControlData( _T("�����ID"), sData );
				break;
			case 10 :
				m_MasterUserInfoGrid.SetControlData( _T("���(+), ����(-)"), sData );
				break;
			case 11 :
				CLocCommonAPI::ChangeSecuriyCivilNo( this , sData , m_sIsUnityLoanService );
				m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ],  sData );
				break;
			case 12 :
			case 13 :
			case 14 :
			case 15 :
			case 16 :
			case 17 :
				tmpTitle = sDMFieldAlias[ i ];
				tmpTitle.Replace( _T("_"), _T(" ") );
				m_MasterUserInfoGrid.SetControlData( tmpTitle, sData );
				break;
			case 29 :
				m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ], _T("H") == sData ? _T("����") : _T("�ٹ���") );
				break;
			default :
				m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ], sData );
				break;
		}		
	}
	m_MasterUserInfoGrid.Display();
	
	return 0;
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnSelchangetabTITLE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ChangeView();
	
	*pResult = 0;
}


/// ��忡 ���� �ٸ���.
INT CCO_CENTER_USER_DETAILVIEWER2::ChangeView()
{

	INT ids , nTabIndex;

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTITLE);
	nTabIndex = pTab->GetCurSel();

	GetDlgItem(IDC_gridDCENTERUSER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_gridDCENTERUSER2)->ShowWindow(SW_HIDE);
	/// �������� ��������
	if( ((CButton*)GetDlgItem(IDC_radVIEWER_MODE))->GetCheck() == 1 )
	{
		if( nTabIndex > 0 )
		{
			ids = ShowLibUserInfo(nTabIndex-1);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ChangeView") );		
		}
		else
		{
			ShowMasterUserInfo();
		}
	}
	/// �ѹ��� ��� ����
	else
	{
		if( nTabIndex > 0 )
		{
			GetDlgItem(IDC_gridDCENTERUSER)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_gridDCENTERUSER2)->ShowWindow(SW_SHOW);

			// 2009.10.08 UPDATE BY KSJ : �ֹι�ȣ ���⵵�� ����
			CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_DETAIL_VIEWER2") );
			if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ChangeView") );	
			
			ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , pDM , -1  , _T("CIVIL_NO") , m_sIsUnityLoanService );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ChangeView") );
			
			CString strUserNo;
			INT i , nRowCnt = pDM->GetRowCount();
			for( i=0 ; i<nRowCnt ; i++ )
			{
				ids = pDM->GetCellData( _T("USER_NO") , i , strUserNo );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ChangeView") );

				ChangeSecuriyUserNo(strUserNo);

				ids = pDM->SetCellData( _T("USER_NO") , strUserNo , i );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ChangeView") );
			}
		}
		else
		{
			ShowMasterUserInfo();
		}

		ids = AllControlDisplay(_T("CM_CO_CENTER_DETAIL_VIEWER2"));
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ChangeView") );
	}
	
	return 0;
}

/// 
INT CCO_CENTER_USER_DETAILVIEWER2::ShowLibUserInfo(INT nIndex)
{
	INT ids;
	
	const INT nFieldCnt = 44;
	CString sDMFieldAlias[nFieldCnt][2] = 
	{
		{ _T("LIB_CODE                 "),  _T("�Ҽ� ��������                                    ")},		//0
		{ _T("USER_NO                  "),  _T("�����ڹ�ȣ                                       ")},		//1
		{ _T("USER_ID                  "),  _T("�����ID                                         ")},		//2
		{ _T("PASSWORD                 "),  _T("��й�ȣ                                         ")},		//3
		{ _T("NAME                     "),  _T("�̸�                                             ")},		//4
		{ _T("BIRTHDAY                 "),  _T("�������                                         ")},		//5
		{ _T("BIRTHDAY_TYPE            "),  _T("���� ����                              ")},					//6
		{ _T("CIVIL_NO                 "),  _T("�ֹε�Ϲ�ȣ                                     ")},
		{ _T("H_ADDR1             "),  _T("�ּ�(��)                                         ")},
		{ _T("W_ADDR1             "),  _T("�ּ�(�ٹ���)                                     ")},
		{ _T("H_ZIPCODE            "),  _T("�����ȣ(��)                                     ")},		//10
		{ _T("W_ZIPCODE            "),  _T("�����ȣ(�ٹ���)                                 ")},
		{ _T("H_PHONE               "),  _T("��ȭ��ȣ(��)                                     ")},
		{ _T("W_PHONE               "),  _T("��ȭ��ȣ(�ٹ���)                                 ")},
		{ _T("EXCHANGE_NO              "),  _T("������ȣ                                         ")},
		{ _T("DEPARTMENT_NAME          "),  _T("�ٹ��� �μ���                                    ")},		//15
		{ _T("CLASS_NAME               "),  _T("�ٹ��� ���޸�                                    ")},
		{ _T("HANDPHONE                "),  _T("�ڵ���                                           ")},
		{ _T("IDX_HANDPHONE            "),  _T("�ڵ��� ����                                      ")},
		{ _T("E_MAIL                   "),  _T("E-MAIL                                           ")},
		{ _T("REG_DATE                 "),  _T("�����                                           ")},		//20
		{ _T("REMOVE_DATE              "),  _T("������                                           ")},
		{ _T("USER_CLASS           "),  _T("ȸ�� ����  ")},
		{ _T("INFERIOR_REASON          "),  _T("�������� ����                                     ")},
		{ _T("MEMBER_CLASS             "),  _T("ȸ������                     ")},
		{ _T("SCHOOL                   "),  _T("�б�                                             ")},		//25
		{ _T("GRADE                    "),  _T("�г�                                             ")},
		{ _T("CLASS                    "),  _T("��                                               ")},
		{ _T("CLASS_NO                 "),  _T("����                                             ")},
		{ _T("USER_POSITION_CODE  "),  _T("����Ҽ������ڵ�                                 ")},
		{ _T("USER_CLASS_CODE     "),  _T("�������������ڵ�                                 ")},		//30
		{ _T("MAIL_SEND_AREA           "),  _T("����߼�����                     ")},
		{ _T("LOAN_COUNT               "),  _T("���� �Ѵ���å��                                  ")},
		{ _T("RESERVATION_COUNT        "),  _T("���� �ѿ���å��                                  ")},
		{ _T("DELAY_COUNT              "),  _T("��üȽ��                                         ")},
		{ _T("LOAN_STOP_DATE           "),  _T("����������                                       ")},		//35
		{ _T("LAST_LOAN_DATE           "),  _T("����������                                       ")},
		{ _T("LAST_MODIFY_DATE         "),  _T("��������������                                   ")},
		{ _T("REMARK                   "),  _T("���                                             ")},
		{ _T("REISSUE_CNT              "),  _T("��߱�Ƚ��                                       ")},
		{ _T("LOAN_STOP_SET_DATE       "),  _T("�������� �ο���                                  ")},		//40
		{ _T("SHELF_LOC_CODE           "),  _T("�ڷ���ڵ�                                       ")},
		{ _T("LOST_USER_CARD           "),  _T("ȸ�����нǿ��� ('Y','N')                         ")},
		{ _T("DELAYDAY_COUNT           "),  _T("������ü�ϼ�                                     ")}
	};
	
	
	
	m_MasterUserInfoGrid.LIST.RemoveAll();
	m_MasterUserInfoGrid.Clear();
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_DETAIL_VIEWER2") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowMasterUserInfo") );	
	
	CString tmpTitle;
	CString sData;
	for( INT i = 0; i < nFieldCnt; i++ )
	{
		sDMFieldAlias[i][0].TrimLeft(); sDMFieldAlias[i][0].TrimRight(); 
		sDMFieldAlias[i][1].TrimLeft(); sDMFieldAlias[i][1].TrimRight(); 
		ids = pDM->GetCellData( sDMFieldAlias[i][0], nIndex, sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowMasterUserInfo") );
		
		switch( i )
		{
		case 3 :
		case 18 :
			break;
		case 6 :
			m_MasterUserInfoGrid.SetControlData( _T("���(+), ����(-)"), sData );
			break;
		case 7 :
			CLocCommonAPI::ChangeSecuriyCivilNo( this , sData , m_sIsUnityLoanService );
			m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ][1],  sData );
			break;
		case 22 :
			if( sData.Compare(_T("0"))==0 ) sData = _T("����ȸ��");
			else if( sData.Compare(_T("1"))==0 ) sData = _T("��������ȸ��");
			else if( sData.Compare(_T("2"))==0 ) sData = _T("����ȸ��");
			// 2009.05.18 ADD BY CJY : Ż��ȸ�� �߰�
			else if( sData.Compare(_T("3"))==0 ) sData = _T("Ż��ȸ��");
			m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ][1], sData );
			break;
		case 24 :
			if( sData.Compare(_T("0"))==0 ) sData = _T("��ȸ��");
			else sData = _T("��ȸ��");
			m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ][1], sData );
			break;
		case 31 :
			m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ][1], _T("H") == sData ? _T("����") : _T("�ٹ���") );
			break;
		default :
			m_MasterUserInfoGrid.SetControlData( sDMFieldAlias[ i ][1], sData );
			break;
		}		
	}
	m_MasterUserInfoGrid.Display();
	
	return 0;
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnradVIEWERMODE() 
{
	InitTabControl();
	ChangeView();
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnradVIEWERMODE2() 
{
	InitTabControl();
	ChangeView();
}

VOID CCO_CENTER_USER_DETAILVIEWER2::OnbPRINT() 
{
	EFS_BEGIN

	INT ids;
	CString sPrintAlias = _T("��������������");
	CString	sPrintDM = _T("DM_CO_CENTER_DETAIL_VIEWER2");
	CESL_DataMgr* pPrintDM = FindDM( sPrintDM );		

	// ����Ʈ ��ü �ʱ�ȭ
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	// KOL.UDF.022
	//if (!pSIReportManager) {
	if (&pSIReportManager == NULL) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return ;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	
	ids = pSIReportManager->GetSIReportFileFromDB(sPrintAlias);
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return ;
	}
	
	ids = pSIReportManager->DeleteTempPageFiles();

	pSIReportManager->SetDataMgr(0, pPrintDM, 0);

	for( INT i = 0 ; i < pPrintDM->GetRowCount() ; i++ )
	{
		ids = SetUserDetailInfoCondition( pSIReportManager , pPrintDM , i );
		if( ids < 0 ) return;
		
		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// ��ũ�� Manual �ݺ� ���
		if( ids < 0 ) return;
	}
	
	
	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return;

	EFS_END

	return;
}

INT CCO_CENTER_USER_DETAILVIEWER2::SetUserDetailInfoCondition(  CSIReportManager* pSIReportManager , CESL_DataMgr* pPrintDM , INT nIndex )
{
	EFS_BEGIN
	
	const INT nCnt = 26;

	CString sInputTmpAlias[nCnt] =
	{
		_T("�̿��ڹ�ȣ"),	_T("����ID"),		_T("�̸�"),			_T("�ֹε�Ϲ�ȣ"),		_T("����") , 
		_T("�޴���"),		_T("E_MAIL"),		_T("�Ҽ�"),			_T("����"),				_T("�ڷ��") , 
		_T("����_�����ȣ"),_T("����_�ּ�"),	_T("����_��ȭ"),	_T("�ٹ���_�����ȣ"),	_T("�ٹ���_��ȭ") , 
		_T("�ٹ���_�ּ�"),	_T("�б���"),		_T("�г�"),			_T("��"),				_T("���"), 
	   _T("�����Ѵ���å��"),_T("����å��"),		_T("��üȽ��"),		_T("����������"),	_T("����������"),
		_T("������ü��")
	};

	const INT nGetFieldCnt = 26;
	CString sValue[nGetFieldCnt];
	CString sGetDMFieldAlias[nGetFieldCnt] =
	{
		_T("USER_NO"),		_T("FID_KEY"),			_T("NAME"),			_T("CIVIL_NO"),			_T("BIRTHDAY") , 
		_T("HANDPHONE"),	_T("E_MAIL"),	_T("USER_POSITION_CODE"),	_T("USER_CLASS_CODE"),	_T("SHELF_LOC_CODE") ,
		_T("H_ZIPCODE"),	_T("H_ADDR1"),			_T("H_PHONE"),		_T("W_ZIPCODE"),		_T("W_PHONE") ,
		_T("W_ADDR1"),		_T("SCHOOL"),			_T("GRADE"),		_T("CLASS"),			_T("REMARK"), 
		_T("LOAN_COUNT"),	_T("RESERVATION_COUNT"),_T("DELAY_COUNT"),	_T("LOAN_STOP_DATE"),	_T("LAST_LOAN_DATE"), 
		_T("DELAYDAY_COUNT") 
	};

	CString	sPrintDM = _T("DM_CO_CENTER_DETAIL_VIEWER2");
	CESL_DataMgr* pPrintDM = FindDM( sPrintDM );		

	pPrintDM->GetCellData( sGetDMFieldAlias , nGetFieldCnt , sValue , nIndex );	

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		sValue[i].TrimLeft();

		// 2009.05.18 UPDATE BY CJY : �ֹε�Ϲ�ȣ ���ڸ� ����
		if( _T("CIVIL_NO") == sGetDMFieldAlias[i] )
		{
			CLocCommonAPI::ChangeSecuriyCivilNo( this , sValue[i] , m_sIsUnityLoanService );
		}

		// 2009.10.09 ADD BY KSJ : �����ڹ�ȣ�� �ֹε�Ϲ�ȣ�� �� ó��
		if( _T("USER_NO") == sGetDMFieldAlias[i] )
		{
			ChangeSecuriyUserNo( sValue[i] );
		}

		sValue[i] = _T(" ") + sValue[i];
		pSIReportManager->SetTempVariable( sInputTmpAlias[i] , sValue[i] );
	}
	

	return 0;


	EFS_END

	return -1;
}

INT CCO_CENTER_USER_DETAILVIEWER2::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN
		
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	
	return 0;
	
	EFS_END
	return -1;	
}

HBRUSH CCO_CENTER_USER_DETAILVIEWER2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// ������� �߰�

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

/*
@Create
 - 2009.10.08 BY KSJ
@Description
 - �����ڹ�ȣ�� �ֹι�ȣ�� ���, �ɼǰ��� ���� ����ó���Ѵ�.
@Parameter
 - CString &strUserNo : �����ڹ�ȣ
@Return Value
 - 0 : ����
@Mini Spec
 1. 
*/
INT CCO_CENTER_USER_DETAILVIEWER2::ChangeSecuriyUserNo(CString &strUserNo)
{
EFS_BEGIN

	if( _T("Y") != m_strSecurityUserNoYN )
	{
		// 2010.09.03 ADD BY KSJ : ���յ������� ��뿡 ���� ó��
//DEL 		CString strBuf = strUserNo;
//DEL 		if( 13 <= strUserNo.GetLength() )
//DEL 		{
//DEL 			strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 		}
//DEL 
//DEL 		INT ids = CLocCommonAPI::CivilNoCheck( strBuf );
//DEL 		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ChangeSecuriyUserNo") );
//DEL 		if( 0 == ids )
//DEL 		{
//DEL 			// 2009.12.14 ADD BY CJY : �ֹι�ȣ ǥ�ù�� ����
//DEL 			strUserNo.Format(_T("%s%s******"),strBuf.Left(6),strBuf.Mid(7,1));
//DEL 			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ChangeSecuriyUserNo") );
//DEL 		}
		CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
	}

	return 0;

EFS_END
	return -1;
}
