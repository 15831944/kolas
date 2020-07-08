// UnionUserInfoViewer.cpp : implementation file
//

#include "stdafx.h"
#include "UnionUserInfoViewer.h"
#include <afxsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnionUserInfoViewer dialog
CUnionUserInfoViewer::CUnionUserInfoViewer( CESL_Mgr *pParent /*=NULL*/ )
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CUnionUserInfoViewer)
	//}}AFX_DATA_INIT

	m_pParent             = pParent;
	m_pLoanLogInfoTypeDlg = NULL;		
	m_pNoticeMessage      = NULL;

	m_loanLogView  = FALSE;
	m_loanUserView = TRUE;	

	m_bThreadWorking = FALSE;
	
	m_bIsUnionLoanLimit = FALSE;

	m_bMoveWindow = FALSE;
	m_bFirstShow = FALSE;

	EnableThemeDialogTexture(GetSafeHwnd()); 
}

BOOL CUnionUserInfoViewer::Create( CWnd *pParentWnd )
{	
EFS_BEGIN

	return CDialog::Create( IDD, pParentWnd ); 

EFS_END
return FALSE;
}

void CUnionUserInfoViewer::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnionUserInfoViewer)
	DDX_Control(pDX, IDC_STATIC_TITLE, m_sM);
	DDX_Control(pDX, IDC_bS_M, m_bS_M);
	DDX_Control(pDX, IDC_bDELAY_BOOK_RETURN, m_bD);
	DDX_Control(pDX, IDC_bLOAN_LOG_VIEW, m_bS);
	DDX_Control(pDX, IDC_bRELOAD, m_bR);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_bC);
	//}}AFX_DATA_MAP

EFS_END
}

BEGIN_MESSAGE_MAP(CUnionUserInfoViewer, CDialog)
	//{{AFX_MSG_MAP(CUnionUserInfoViewer)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bLOAN_LOG_VIEW, OnbLOANLOGVIEW)
	ON_BN_CLICKED(IDC_bDELAY_BOOK_RETURN, OnbDELAYBOOKRETURN)
	ON_BN_CLICKED(IDC_bRELOAD, OnbRELOAD)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bS_M, OnbSM)
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_UNION_USER_INFO, OnSelchangetabUnionUserInfo)
	ON_BN_CLICKED(IDC_BTN_MOVE_WINODW, OnBtnMoveWinodw)
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_MESSAGE( SHOW_GRID, ShowGrid )
	ON_MESSAGE( SHOW_GRID2, ShowCenterUserTotalInfo )	
	ON_BN_CLICKED(IDC_STATIC_MOVE, OnStaticMove)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+2000, OnShowMessage)
	ON_MESSAGE(WM_USER+2001, OnShowMessage1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnionUserInfoViewer message handlers

BOOL CUnionUserInfoViewer::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	CDialog::SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );

	EnableThemeDialogTexture(GetSafeHwnd());

	if( InitESL_Mgr( _T("UnionUserInfoViewer") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : UnionUserInfoViewer") );
		return false;
	}

	// 2006.12.14 ADD BY PDJ 
	// �α����
	ProcessTimeLog(_T("Start"));
	bEmergencyStopFlag = FALSE;
	
	ViewLoanLogInfoTypeDlg();
	SetDefaultSize();
	
	m_libCode = GetLibCode();
	m_grid.InitViewer( IDC_gUSER_INFO, this );
	m_grid.SetViewerWidth( 35 , 60 );	

	( CButton * )GetDlgItem( IDC_bLOAN_LOG_VIEW		)->ShowWindow( m_UseCenterUserLoanLog );
	( CButton * )GetDlgItem( IDC_bDELAY_BOOK_RETURN )->ShowWindow( m_UseCenterUserLoanLog );
	( CButton * )GetDlgItem( IDC_bS_M				)->ShowWindow( m_UseCenterUserLoanLog );

	if( !m_UseCenterUserLoanLog )
	{
		// �׸����� ���۷��� ������ �����Ѵ�.
		// �����ڸ� ������ ��쿡�� ���� å���� �ʿ䰡 ����.
		CESL_Grid* pLibraryGrid = (CESL_Grid*)FindControl( _T("CM_UnionUserInfoViewer") , _T("GRID2") );
		if( pLibraryGrid == NULL ) 
		{
			AfxMessageBox( _T("FindControl Error") );
			return false;
		}

		// 2009.10.28 UPDATE BY PWR : �÷��� 4�� ������ �ٲ��ָ� ControlDisplay���� �����߻���
		pLibraryGrid->SetColWidth(5, 0, 0);
//		pLibraryGrid->m_arrColumnInfo.RemoveTail();
//		pLibraryGrid->SetCols(0,4);
	}
		
	CRect rect;
	GetCtrlRect( IDC_sMAX_DEFAULT_SIZE, rect );
	m_maxWidth = rect.Width();

	MakeToolTip();
	MakeBitmapBtns();
	ViewResizeBtns();

	///{{++
	/// 2008.05.09 UPDATE BY PDW
	/// �����츦 ���ʾƷ����� �����ֵ����Ѵ�
//	CenterWindow();
	CRect rcFrame;
	CRect rc;
	pMain->GetParentFrame()->GetWindowRect(&rcFrame);
	this->GetWindowRect(&rc);
	this->MoveWindow(rcFrame.left, rcFrame.bottom - rc.Height(), rc.Width(), rc.Height());
	///}}--

	///{{ 2008.05.06 add by pdw++
	/// ���� �߰��Ͽ���
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_UNION_USER_INFO);
	if( NULL != pTab )
	{
		if( NULL != pTab->GetSafeHwnd() )
		{
			pTab->InsertItem(0, _T("���� ������ ����"));
			pTab->InsertItem(1, _T("���� ������ �󼼺���"));
		}
	}

	m_sM.ModifyStyle(0,SS_BITMAP);
	///{{ 2008.05.06 add by pdw--
	
	// 2006-05-25 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++
	// ���մ������� å���� �������� ���� ���� ����
	CString strData;
	GetManageValue( _T("��Ÿ"), _T("����"), _T("�����ڰ�������"), _T("���մ���å������"), strData );
	if ( _T("Y") == strData )
	{
		m_bIsUnionLoanLimit = TRUE;
	}
	else
	{
		m_bIsUnionLoanLimit = FALSE;
	}

	if( m_bIsUnionLoanLimit )	
	{
		CRS_SQL centerDBSelector( this );
		CString DMAlias = _T("DM_UnionESL_ManageInfo");
		CESL_DataMgr* pDM = FindDM( DMAlias );

		if( 0 == pDM->GetRowCount() )
		{
//			centerDBSelector.LOAN_Select( pDM, _T(""), DMAlias, 1, 1 );
			
			for ( INT i = 0;i < pDM->GetRowCount();i++ )
			{
				pDM->GetCellData( _T("CLASS_ALIAS"), i , strData );
				if ( strData == _T("���մ������") )
				{
					pDM->GetCellData( _T("VALUE_NAME"), i , strData );
					if ( strData == _T("���մ�������å��") )
					{
						/// default ���̴�
						pDM->GetCellData( _T("VALUE_1"), i , m_sUnionLoanLimitCnt );
						break;
					}
				}
			}
		}

		/// 2008.02.28 add by pdw 
		/// ���޺� ���մ���å�� ���� ����
		DMAlias = _T("DM_UnionLoanEnvInfo");
		CESL_DataMgr* pLoanEnvDM = FindDM( DMAlias );
		if( 0 == pLoanEnvDM->GetRowCount() )
		{		
//			centerDBSelector.LOAN_Select( pLoanEnvDM, _T(""), DMAlias, 1, 1 );
		}
	}	
	// ----------------------------------------------------------------------

	// 2008.05.29 ADD BY PDW {{++
	// ���մ��� ����ȭ ���� �ð� �⺻�� ����
	CEdit* pBox = (CEdit*)GetDlgItem(IDC_EDT_DELAY_TIME);
	if( pBox->GetSafeHwnd() )
	{
		pBox->LimitText(3);
		pBox->SetWindowText(_T("3"));
	}
	// 2008.05.29 ADD BY PDW --}}

	// 2010.09.08 ADD BY KSJ : ���յ������� ��뿩��
	GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");

	return TRUE;

EFS_END
	return FALSE;
}

void CUnionUserInfoViewer::OnOK() 
{
	EFS_BEGIN

	// TODO: Add extra validation here
	
	
	this->ShowWindow( false );
	//CDialog::OnOK();

	EFS_END
}

void CUnionUserInfoViewer::OnDestroy() 
{
	EFS_BEGIN

	
	this->ShowWindow( false );
	//CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

	EFS_END
}

void CUnionUserInfoViewer::OnCancel() 
{
	EFS_BEGIN

	// TODO: Add extra cleanup here
	
	this->ShowWindow( false );
	//CDialog::OnCancel();

	EFS_END
}

void CUnionUserInfoViewer::OnbLOANLOGVIEW() 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	ViewUserLoanLogGrid();
//	Resize();
	ViewResizeBtns();

	EFS_END
}

void CUnionUserInfoViewer::ViewUserLoanLogGrid()
{
	EFS_BEGIN

	CRect rect;	
/*	CRect UserViewRect;
	CRect UserGroupRect;

	GetCtrlRect( IDC_gUSER_INFO2, UserViewRect );	
	GetCtrlRect( IDC_sUSER_INFO, UserGroupRect );	
	if( m_loanLogView && m_loanUserView == true)
	{
		m_loanUserView = false;			
		m_loanLogView = false;
		((CESL_Grid*) GetDlgItem (IDC_gUSER_INFO))->ShowWindow(SW_HIDE);		

		UserViewRect.top = UserViewRect.top - 255;
		UserViewRect.bottom = UserViewRect.bottom - 245;
		
		UserGroupRect.bottom = UserGroupRect.bottom - 255;

		( (CESL_Grid*) GetDlgItem (IDC_gUSER_INFO2) )->MoveWindow(UserViewRect);
		( (CStatic*) GetDlgItem (IDC_sUSER_INFO) )->MoveWindow(UserGroupRect);
	}
	else if( m_loanLogView == false && m_loanUserView == false)
	{		
		m_loanUserView = true;		
		((CESL_Grid*) GetDlgItem (IDC_gUSER_INFO))->ShowWindow(SW_SHOW);		

		UserViewRect.top = UserViewRect.top + 255;
		UserViewRect.bottom = UserViewRect.bottom + 245;		

		UserGroupRect.bottom = UserGroupRect.bottom + 255;

		( (CESL_Grid*) GetDlgItem (IDC_gUSER_INFO2) )->MoveWindow(UserViewRect);
		( (CStatic*) GetDlgItem (IDC_sUSER_INFO) )->MoveWindow(UserGroupRect);

	}
	else
	{		
		m_loanLogView = true;

		this->GetWindowRect( rect );
		this->MoveWindow( rect.left, rect.top, m_maxWidth, rect.Height() );
	}
*/
	if( m_loanLogView )
	{
		m_loanLogView = false;
		CRect rcDefault, rect;
		GetCtrlRect( IDC_sDEFAULT_SIZE, rcDefault );

		this->GetWindowRect( rect );
		this->MoveWindow( rect.left, rect.top, rcDefault.Width(), rect.Height() );
	}
	else
	{
		m_loanLogView = true;
		this->GetWindowRect( rect );
		this->MoveWindow( rect.left, rect.top, m_maxWidth, rect.Height() );
	}

	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [ Func Desc ]
//   ���մ����� ���� �˻� ������
// [ Mini Spec ]
/////////////////////////////////////////////////////////////////////////////
unsigned int ViewUnionUserInfoThread( LPVOID pParam )
{
EFS_BEGIN

	// =====================================================================
	// Variable Declare
	// =====================================================================	
	int ids;
	CUnionUserInfoViewer *pParent = ( CUnionUserInfoViewer * )pParam;
	pParent->m_bThreadWorking = true;
	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.1"));

	CString userKey = pParent->m_userKey;
	CString DMAlias = _T("DM_UnionUserInfoViewer");
	CESL_DataMgr *pDM = pParent->FindDM( DMAlias );
	
	DMAlias = _T("DM_UnionUserInfoViewer_LibUserInfo");
	CESL_DataMgr* pLibUserDM = pParent->FindDM( DMAlias );
	INT nTotalLoanCnt = 0;

	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.2"));

	if( !pParent->m_userKey.IsEmpty() )
	{
		/// ���� ������ ���� SELECT
		CString where;
		where.Format( _T("REC_KEY = ( SELECT MASTER_USER_KEY FROM CT_LIBRARY_LOAN_USER_TBL WHERE CLIENT_REC_KEY = %s AND LIB_CODE = '%s' )"), pParent->m_userKey, pParent->m_libCode );		
		
		CRS_SQL centerDBSelector( pParent );
//		centerDBSelector.LOAN_Select( pDM, where, DMAlias, 1, 1 );
		pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.3"));
		
		/// �������� ������ ������ SELECT
		where.Format( _T("MASTER_USER_KEY = ( SELECT MASTER_USER_KEY FROM CT_LIBRARY_LOAN_USER_TBL WHERE CLIENT_REC_KEY = %s AND LIB_CODE = '%s' )"), pParent->m_userKey, pParent->m_libCode );		
//		centerDBSelector.LOAN_Select( pLibUserDM, where, DMAlias, 1, 1 );
		pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.4"));

		/// ���� �������� ���� �̿��� ����ȸ������ ����
		CString sLoanStopDate;
		CString sInferiorClass;	
		pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.5"));

		CString sDelayCnt;
		CString sLoanCnt;
		CString strLibCode;
		for( int i = 0 ; i < pLibUserDM->GetRowCount() ; i++ )
		{			
			///{{++
			/// 2008.05.02 update by pdw
			/// �ڰ��� ������ Ÿ�������� ����Ǽ��� Ȯ���Ѵ�
			ids = pLibUserDM->GetCellData( _T("�Ѵ���å��") , i , sLoanCnt );
			if( ids < 0 ) 
			{
				AfxMessageBox(_T("ViewUnionUserInfoThread ERROR -3"));
				return -1;
			}
			
			ids = pLibUserDM->GetCellData( _T("��������ȣ") , i , strLibCode );
			if( ids < 0 ) 
			{
				AfxMessageBox(_T("ViewUnionUserInfoThread ERROR -31"));
				return -1;
			}
			
			if( pParent->m_libCode != strLibCode )
				nTotalLoanCnt += _ttoi( sLoanCnt );
			///}}--

			///{{ 2008.05.06 rem by pdw++
			/// ����� ȸ�����·� �����ֵ��� ����Ǿ���
			/*
			ids = pLibUserDM->GetCellData( "ȸ������" , i , sInferiorClass );
			if( ids < 0 ) 
			{
				AfxMessageBox("ViewUnionUserInfoThread ERROR -1");
				return -1;
			}
			
			// ��ü�� �̷��� �ִٸ� ȸ���̶�� ��������ȸ������ �����ؾ��Ѵ�.
			ids = pLibUserDM->GetCellData( "��üå��" , i , sDelayCnt );
			if( ids < 0 ) 
			{
				AfxMessageBox("ViewUnionUserInfoThread ERROR -2");
				return -1;
			}			
			
			// ������������ �����Ϻ��� �۴ٸ� ����ȸ������ �����Ѵ�.
			ids = pLibUserDM->GetCellData( "����������" , i , sLoanStopDate );
			if( ids < 0 ) 
			{
				AfxMessageBox("ERROR -2");
				return -2;
			}			
			
			pParent->ProcessTimeLog("ViewUnionUserInfoThread.6");
			CString strTmpInferioClass;
			if ( !sLoanStopDate.IsEmpty() && 10 == sLoanStopDate.GetLength() )
			{
				tLoanStopDate = COleDateTime( atoi(sLoanStopDate.Mid(0,4)) , atoi(sLoanStopDate.Mid(5,2)) , atoi(sLoanStopDate.Mid(8,2) ) , 0 , 0 , 0);

				COleDateTimeSpan  tS = tCurrent - tLoanStopDate;				
				
				if( tCurrent > tLoanStopDate && 1 <= tS.GetDays() )
				{
					strTmpInferioClass = "����ȸ��";
						
					ids = pLibUserDM->SetCellData( "����������" , "" , i );
					if( ids < 0 ) 
					{
						AfxMessageBox("ViewUnionUserInfoThread -2");
						return -4;
					}					
				}
				else
				{
					strTmpInferioClass = "��������ȸ��";
				}				
			}
			else
			{
				strTmpInferioClass = "����ȸ��";
			}
					
			if(atoi(sDelayCnt) > 0 )
			{
				strTmpInferioClass = "��������ȸ��";
			}
			if( sInferiorClass.Compare("����ȸ��") != 0 )
			{
				sInferiorClass = strTmpInferioClass;
			}


			ids = pLibUserDM->SetCellData( "ȸ������" ,  sInferiorClass , i );
			if( ids < 0 )
			{
				AfxMessageBox("ViewUnionUserInfoThread ERROR -4");
				return -3;
			}							
			pParent->ProcessTimeLog("ViewUnionUserInfoThread.7");
			*/
			///{{ 2008.05.06 rem by pdw--
		}
	}
	else
	{
		pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.8"));
		(( CStatic*) pParent->GetDlgItem (IDC_sMESSAGE))->SetWindowText(_T("�˻��Ϸ�"));
		pDM->FreeData();
		pLibUserDM->FreeData();
		pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.9"));
	}

	// 2006-11-14 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++	
	// �����ڹ�ȣ�� ������ID�� ������������ �ƴ� �ڰ� ������ �����ش�.
	// ���մ��������� ���ٸ� �н�..
	if ( pDM->GetRowCount() )
	{
		pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.10"));
		CString strUserNo, strUserID;
		CESL_DataMgr *pParentDM = pParent->FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
		if ( !pParentDM )
		{
			AfxMessageBox(_T("ViewUnionUserInfoThread ERROR -5"));
			return -3;
		}
		ids = pParentDM->GetCellData( _T("�����ڹ�ȣ") , 0 , strUserNo );
		if (ids < 0 )
		{
			AfxMessageBox(_T("ViewUnionUserInfoThread ERROR -4"));
			return -3;
		}
		ids = pParentDM->GetCellData( _T("������ID") , 0 , strUserID );
		if (ids < 0 )
		{
			AfxMessageBox(_T("ViewUnionUserInfoThread ERROR -4"));
			return -3;
		}
		ids = pDM->SetCellData( _T("�����ڹ�ȣ") , strUserNo, 0 );
		if (ids < 0 )
		{
			AfxMessageBox(_T("ViewUnionUserInfoThread ERROR -4"));
			return -3;
		}
		ids = pDM->SetCellData( _T("������ID") , strUserID, 0 );
		if (ids < 0 )
		{
			AfxMessageBox(_T("ViewUnionUserInfoThread ERROR -4"));
			return -3;
		}
		pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.11"));
	}
	// ------------------------------------------------------------------------------
	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.12"));
	pParent->ShowCenterUserInfo( pDM );		
	
	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.13"));
	int gridShowType = 0;
	if( pParent->m_UseCenterUserLoanLog )
	{
		pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.14"));
		pParent->SearchCenterUserLoanInfo( pParent->m_userKey );
		gridShowType = 1;
		pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.15"));
	}

	gridShowType = 1;

	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.16"));
	::PostMessage( pParent->m_hWnd, SHOW_GRID, 0, gridShowType );	
	::PostMessage( pParent->m_hWnd, SHOW_GRID2, 0, 0 );	
	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.17"));
	
	
	// 2006-05-24 UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++
	// GetLoanStopuserStatus() ���濡 ����
	/*
	BOOL bLoanStop;

	bLoanStop = pParent->GetLoanStopUserStatus();
	if ( bLoanStop ) AfxMessageBox("Ÿ ������ �������� ȸ���Դϴ�.");
	*/
	CString strData;

	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.18"));
	CESL_DataMgr* pMemberInfo = pParent->FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
	if( pMemberInfo->GetRowCount() )
	{
		if ( !pMemberInfo ) 
		{
			AfxMessageBox(_T("ERROR -6"));
			return -6;
		}
		pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.19"));
		
		// 2008.02.28 add by pdw {{++
		// ���޺� ���մ���å�� ���� ��� �߰�
		// ���� �˻��� �̿����� ������ �������� �Ѵ�	
		CString strUserClassCode;
		ids = pMemberInfo->GetCellData( _T("����������"), 0 , strUserClassCode );
		if (ids < 0 )
		{
			AfxMessageBox(_T("ViewUnionUserInfoThread ERROR -41"));
			return -3;
		}
		strUserClassCode.TrimLeft();
		strUserClassCode.TrimRight();

		///{{++
		/// 2008.05.02 rem by pdw
		/// ���� �ֽ������� ������Ʈ �ǵ��� ����
		/// Ÿ���ݳ� ��� ����
		/// 2008.02.29 add by pdw 
		/// ���޺� ���մ��� å�� ���� ��� �߰� 
		/// ����ݳ������� �����߿��� ���մ���å�� ���� ������.
		/// �����¸� �����ϱ� ���� ���մ���å�� �����Ͱ� ���� ��쿡�� �����Ѵ�
		if( pParent->m_bIsUnionLoanLimit )
		{		
			ids = pMemberInfo->GetCellData( _T("���մ���å��"), 0, strData );
			if (ids < 0 )
			{
				AfxMessageBox(_T("ViewUnionUserInfoThread ERROR -48"));
				return -3;
			}

//			if( strData.IsEmpty() )
			{
				CString strLimitCnt;
				pParent->GetUnionLoanLimitBookCnt(strUserClassCode, strLimitCnt);
				strLimitCnt.TrimLeft();
				strLimitCnt.TrimRight();

				if( strLimitCnt.IsEmpty() ) strLimitCnt = pParent->m_sUnionLoanLimitCnt;	
				// ��ü ����å���� ��������
				strData.Format( _T("%d"), ( _ttoi(strLimitCnt) - nTotalLoanCnt ) );
				ids = pMemberInfo->SetCellData( _T("���մ���å��"), strData, 0 );
				if (ids < 0 )
				{
					AfxMessageBox(_T("ViewUnionUserInfoThread ERROR -49"));
					return -3;
				}
			}
		}
		
		pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.20"));
		//}}--
	}
	else
	{
		pParent->m_bThreadWorking = FALSE;
		return 0;
	}
	
	// 2009.03.17 UPDATE BY PWR : ����ȸ���� ���� ��������
	CString strLoanStopUserMSG, strRemoveUserMSG;
 	ids = pParent->GetManageValue( _T("��Ÿ"), _T("����"), _T("�����ڰ�������"), _T("��������ȸ���޽������"), strLoanStopUserMSG );
 	if ( ids < 0 ) 
 	{
 		AfxMessageBox(_T("ERROR -2"));
 		return -5;
 	}
 	ids = pParent->GetManageValue( _T("��Ÿ"), _T("����"), _T("�����ڰ�������"), _T("����ȸ���޽������"), strRemoveUserMSG );
 	if ( ids < 0 ) 
 	{
 		AfxMessageBox(_T("ERROR -2"));
 		return -5;
 	}

	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.21"));

	// ������
	if ( strLoanStopUserMSG == _T("0") ) 
	{
		( (CButton*) pParent->GetDlgItem (IDC_bRELOAD) )->EnableWindow(TRUE);
		return 0;
	}
	// �޼�����°� ���������� ��������� ������ ���������� ���� �����̴�.
	// �޼��� ����� ȸ�� ���¸� DM�� ����~
	CString strUserStatus = _T("����ȸ��");
	ids = pParent->GetLoanStopUserStatus();
	if ( 1 == ids ) 
	{	
		pParent->SendMessage(WM_USER+2000, NULL, NULL );
		strUserStatus = _T("��������");
	}

	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.22"));
	
	ids = pMemberInfo->SetCellData( _T("���մ���ȸ������"), strUserStatus, 0 );
	if ( !pMemberInfo ) 
	{
		AfxMessageBox(_T("ERROR -7"));
		return -7;
	}

	if ( strRemoveUserMSG == _T("0") ) 
	{
		( (CButton*) pParent->GetDlgItem (IDC_bRELOAD) )->EnableWindow(TRUE);
		return 0;
	}

	// �޼�����°� ���������� ��������� ������ ���������� ���� �����̴�.
	// �޼��� ����� ȸ�� ���¸� DM�� ����~
	ids = pParent->GetLoanStopUserStatus();
	if ( 2 == ids ) 
	{	
		pParent->SendMessage(WM_USER+2001, NULL, NULL );
		strUserStatus = _T("����ȸ��");
	}
	// 2009.05.25 ADD BY CJY : Ż��ȸ�� �����߰�
	else if ( 3 == ids ) 
	{
		strUserStatus = _T("Ż��ȸ��");
	}

	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.22"));
	
	ids = pMemberInfo->SetCellData( _T("���մ���ȸ������"), strUserStatus, 0 );
	if ( !pMemberInfo ) 
	{
		AfxMessageBox(_T("ERROR -7"));
		return -7;
	}

// 	ids = pParent->GetManageValue( _T("��Ÿ"), _T("����"), _T("�����ڰ�������"), _T("��������ȸ���޽������"), strData );
// 	if ( ids < 0 ) 
// 	{
// 		AfxMessageBox(_T("ERROR -2"));
// 		return -5;
// 	}
// 
// 	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.21"));
// 
// 	// ������
// 	if ( strData == _T("0") ) 
// 	{
// 		( (CButton*) pParent->GetDlgItem (IDC_bRELOAD) )->EnableWindow(TRUE);
// 		return 0;
// 	}
// 
// 	// �޼�����°� ���������� ��������� ������ ���������� ���� �����̴�.
// 	// �޼��� ����� ȸ�� ���¸� DM�� ����~
// 	BOOL bLoanStop;
// 	CString strUserStatus = _T("����ȸ��");
// 
// 	bLoanStop = pParent->GetLoanStopUserStatus();
// 	if ( bLoanStop ) 
// 	{	
// 		pParent->SendMessage(WM_USER+2000, NULL, NULL );
// 		//AfxMessageBox("Ÿ ������ �������� ȸ���Դϴ�.");
// 		strUserStatus = _T("��������");
// 	}
// 
// 	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.22"));
// 	
// 	ids = pMemberInfo->SetCellData( _T("���մ���ȸ������"), strUserStatus, 0 );
// 	if ( !pMemberInfo ) 
// 	{
// 		AfxMessageBox(_T("ERROR -7"));
// 		return -7;
// 	}

	pParent->ProcessTimeLog(_T("ViewUnionUserInfoThread.23"));

	( (CButton*) pParent->GetDlgItem (IDC_bRELOAD) )->EnableWindow(TRUE);
 
	// ----------------------------------------------------------------------------
	pParent->m_bThreadWorking = FALSE;
	return 0;

EFS_END
	return -1;
}

void CUnionUserInfoViewer::SearchCenterUserInfo(CString pk, BOOL bDelaySync/*=FALSE*/)
{	
	m_userKey = pk;	

	KillTimer(1);

	if ( CheckLocalDaemonToDB() == false ) return;
// 	(( CStatic*) GetDlgItem (IDC_sMESSAGE))->SetWindowText(_T("����ȭ���Դϴ�."));	
	
	// 2008.05.29 ADD BY PDW {{++
	// �����̷� �����ð� ���� �߰���
	// �ڵ�����ȭ �ð��� �ƴ� ����/�ݳ�/������� �̷º����� �� �ʵڿ� ����ȭ�� �� �������� �����ϴ� ����
	INT nDelay = 1;
	if( bDelaySync )
	{
		CString strDelay;
		GetDlgItemText(IDC_EDT_DELAY_TIME, strDelay);
		if( strDelay.IsEmpty() )
		{
			strDelay = _T("1");
		}
		
		nDelay = _ttoi(strDelay);

		if( 0 == nDelay )
			nDelay = 1;
	}
	// 2008.05.29 ADD BY PDW --}}

	SetTimer(1, nDelay*1000, NULL);
}

CString CUnionUserInfoViewer::GetLibCode()
{
	EFS_BEGIN

	const int nFieldCnt = 1;
	CString sField[ nFieldCnt ] = { "LIB_CODE" };
	CString sLibCodes[ nFieldCnt ];

	CLocCommonAPI::GetLibInfo( sField, sLibCodes, nFieldCnt, pParentMgr );
	return sLibCodes[ 0 ];

	EFS_END
	return "";

}

void CUnionUserInfoViewer::ShowCenterUserInfo( CESL_DataMgr *pDM )
{
	EFS_BEGIN

	m_grid.Clear();	
	BOOL	bShowData;	

	if( !pDM->GetRowCount() )
	{
		bShowData = FALSE;
	}
	else
	{
		bShowData = TRUE;
	}

	
	const int fieldCnt = 32;
	CString fieldAlias[ fieldCnt ];
	CString fieldData[ fieldCnt ];
	GetUserInfoFieldAlias( fieldAlias );

	if ( bShowData )
	{
		pDM->GetCellData( fieldAlias, fieldCnt, fieldData, 0 );
	}
	
	CString desc;
	for( int i = 0; i < fieldCnt; i++ )
		switch( i )
		{
			case 0 :	// ��������ȣ
			case 2 :
				if ( bShowData )
				{
					m_grid.SetControlData( fieldAlias[ i ], GetLibName( fieldData[ i ] ) );
				}
				else
				{
					m_grid.SetControlData( fieldAlias[ i ], "" );
				}
				
				break;
			case 6 :
			case 26 :
				continue;
				break;
			case 27 :
				if( "H" == desc ) desc = "��";
				else desc = "����";

				if ( bShowData )
				{
					m_grid.SetControlData( fieldAlias[ i ], desc );
				}
				else
				{
					m_grid.SetControlData( fieldAlias[ i ], "");
				}
				break;
			case 29 :	// �ڷ���ڵ�
				if ( bShowData )
				{
					m_pInfo->pCodeMgr->ConvertCodeToDesc( "�ڷ�Ǳ���", fieldData[ i ], desc );
					m_grid.SetControlData( fieldAlias[ i ], desc );
				}
				else
				{					
					m_grid.SetControlData( fieldAlias[ i ], "" );
				}
				
				break;
			// 2007.08.10 add by pdw
			// �ֹε�Ϲ�ȣ * ó�� 
			case 9:
				{
					// 2009.10.20 UPDATE BY KSJ : �ֹι�ȣ ����ȭ ��� ����
//DEL 					CString strCivilNo = fieldData[i];
//DEL 					INT     nPos = 0;
//DEL 					// - �� �ִٸ�
//DEL 					if( -1 < (nPos=strCivilNo.Find(_T("-"))) )
//DEL 					{
//DEL 						CString strBefore = strCivilNo.Mid(0,nPos);
//DEL 						CString strAfter  = strCivilNo.Mid(nPos+1);
//DEL 						
//DEL 						strCivilNo = strBefore + _T("-");
//DEL 
//DEL 						for( int i=0;i<strAfter.GetLength();i++ )
//DEL 							strCivilNo += _T("*");
//DEL 					}
//DEL 					else
//DEL 					{
//DEL 						if( 6 < strCivilNo.GetLength() )
//DEL 						{
//DEL 							CString strBefore = strCivilNo.Mid(0,6);
//DEL 							CString strAfter  = strCivilNo.Mid(6);
//DEL 							
//DEL 							strCivilNo = strBefore + _T("-");
//DEL 
//DEL 							for( int i=0;i<strAfter.GetLength();i++ )
//DEL 								strCivilNo += _T("*");
//DEL 						}
//DEL 					}
//DEL 					m_grid.SetControlData( fieldAlias[ i ], strCivilNo );
					CString strCivilNo = fieldData[i];
					CLocCommonAPI::ChangeSecuriyCivilNo( this , strCivilNo , m_sIsUnityLoanService );
					m_grid.SetControlData( fieldAlias[ i ], strCivilNo );
				}
				break;
			default :
				if ( bShowData )
				{
					m_grid.SetControlData( fieldAlias[ i ], fieldData[ i ] );
				}
				else
				{
					m_grid.SetControlData( fieldAlias[ i ], _T("") );
				}
				
				break;
		}
	m_grid.Display();

	EFS_END
}

void CUnionUserInfoViewer::GetUserInfoFieldAlias( CString alias[] )
{
	EFS_BEGIN

	alias[  0 ] = _T("����������");
	alias[  1 ] = _T("������");
	alias[  2 ] = _T("��������������");
	alias[  3 ] = _T("����������");
	alias[  4 ] = _T("�����ڹ�ȣ");
	alias[  5 ] = _T("������ID");
	alias[  6 ] = _T("��й�ȣ");
	alias[  7 ] = _T("�̸�");
	alias[  8 ] = _T("�������");
	alias[  9 ] = _T("�ֹε�Ϲ�ȣ");
	alias[ 10 ] = _T("�ּ�_��");
	alias[ 11 ] = _T("�ּ�_�ٹ���");
	alias[ 12 ] = _T("�����ȣ_��");
	alias[ 13 ] = _T("�����ȣ_�ٹ���");
	alias[ 14 ] = _T("��ȭ��ȣ_��");
	alias[ 15 ] = _T("��ȭ��ȣ_�ٹ���");
	alias[ 16 ] = _T("������ȣ");
	alias[ 17 ] = _T("�ٹ����μ���");
	alias[ 18 ] = _T("�ٹ������޸�");
	alias[ 19 ] = _T("�ڵ���");
	alias[ 20 ] = _T("E-MAIL");
	alias[ 21 ] = _T("�����");
	alias[ 22 ] = _T("������");
	alias[ 23 ] = _T("�б�");
	alias[ 24 ] = _T("�г�");
	alias[ 25 ] = _T("��");
	alias[ 26 ] = _T("����");
	alias[ 27 ] = _T("����߼�����");
	alias[ 28 ] = _T("���");
	alias[ 29 ] = _T("�ڷ���ڵ�");
	alias[ 30 ] = _T("ȸ�����нǿ���");
	alias[ 31 ] = _T("�ѿ�üȽ��");

	EFS_END
}

CString CUnionUserInfoViewer::GetLibName( CString code )
{
	EFS_BEGIN

/*
	CString query, result;
	query.Format( "SELECT NAME FROM CT_LOCAL_LIBRARY_TBL WHERE CODE = '%s'", code );

	CRS_SQL remoteDBMgr( this );
	remoteDBMgr.LOAN_GetOneValue( query, result );

	return result;
*/
	return code;

	EFS_END
	return _T("");

}

void CUnionUserInfoViewer::SearchCenterUserLoanInfo( CString pk )
{
	EFS_BEGIN

	CString DMAlias = _T("DM_UnionUserInfoViewer_LoanInfo");
	CESL_DataMgr *pDM = FindDM( DMAlias );

	if( !pk.IsEmpty() )
	{
		CString where;
		MakeUserLoanLogWhere( where, pk );

		CRS_SQL centerDBSelector( this );
//		centerDBSelector.SetFetchCount( 0, 100 );
//		centerDBSelector.LOAN_Select( pDM, where, DMAlias, 1, 1 );		
	}
	else
		pDM->FreeData();

	CString status[ 6 ] = {
		_T("����"), _T("�ݳ�"), _T("�ݳ�����"), _T("����"), _T("�������"), _T("����") };
	for( int i = 0; i < pDM->GetRowCount(); i++ )
	{
//		pDM->SetCellData( "��������ȣ_����", GetLibName( pDM->GetCellData( "��������ȣ", i ) ), i );
		pDM->SetCellData( _T("����"), status[ _ttoi( pDM->GetCellData( _T("�������"), i ) ) ], i  );
	}

	EFS_END
}

/*
[ ���� ����ݳ� �ʵ� �˸�� ]
���ڵ�KEY
��������ȣ
Client���ڵ�KEY
�̿����ֽ�����������KEY
�������±���
����å��KEY
��������KEY
����ݳ�KEY_�η��ڷ�
��������
�ݳ�����
������
�ݳ�������
������
���ุ����
�ݳ���
�������
�����ڷ��
��ü��
����
�з���ȣ����
�з���ȣ
��������
��ϱ���
ȸ������
������Ű
��ġ��ȣ
��Ϲ�ȣ
����
������
������
û����ȣ
�����������ڵ�
�ݳ�����Ƚ��
�����ڼҼ��ڵ�
���������
�ݳ�����ο���
�����ݳ�������
���ιݳ����̸�
���ιݳ����̸�
���δ�����̸�
�����۾�
�������۾�
*/

// �ݳ�����
void CUnionUserInfoViewer::OnbDELAYBOOKRETURN() 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	DelayRetrunBook();

	EFS_END
}


void CUnionUserInfoViewer::GetCheckedGridRow( CArray< int, int >&checkedRow )
{
	EFS_BEGIN

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gLOAN_LOG );
	for( int i = 1; i < pGrid->GetRows(); i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
			checkedRow.Add( i - 1 );

		EFS_END
}

int CUnionUserInfoViewer::GetReturnBookDelayInfo( CESL_DataMgr *pDM, CRS_SQL *pRemoteDBMgr, int row, int delayInfo[], CString &libKey )
{
	EFS_BEGIN

	/*

		0 : �ݳ�����켱
		1 : ����켱
	*/
	
	CString libCode		= pDM->GetCellData( _T("��������ȣ"),	  row );
	CString userClass	= pDM->GetCellData( _T("�����������ڵ�"), row );
	CString regCode		= pDM->GetCellData( _T("��ϱ���"),		  row );
	CString	query,	// QUERY
			result;	// result;

	// ������ key ��������
	query.Format( _T("SELECT REC_KEY FROM CT_LOCAL_LIBRARY_TBL WHERE CODE = '%s'"), libCode );
//	pRemoteDBMgr->LOAN_GetOneValue( query, libKey );
	if( libKey.IsEmpty() )
		return -1;
	
	// �ݳ����Ⱑ��Ƚ�� ��������
	query.Format( _T("SELECT VALUE_1 FROM CT_COMMON_LOAN_INFO_TBL WHERE LIB_INFO_KEY = '%s' AND VALUE_NAME = '�ݳ����Ⱑ��Ƚ��'"), libKey );
//	pRemoteDBMgr->LOAN_GetOneValue( query, result );
	delayInfo[ 0 ] = _ttoi( result );
	
	// �ݳ�����Ƚ�� ��������
	query.Format( _T("SELECT POSTPONEMENT_PERIOD FROM CT_POSREG_LOAN_ENV_TBL WHERE LIB_INFO_KEY = %s AND LOAN_CLASS_CODE = '%s' AND REG_CODE = '%s'"), libKey, userClass, regCode );
//	pRemoteDBMgr->LOAN_GetOneValue( query, result );
	if( result.IsEmpty() )
	{
		query.Format( _T("SELECT POSTPONEMENT_PERIOD FROM CT_CD_CODE_TBL WHERE LIB_INFO_KEY = %s AND CODE = '%s'"), libKey, userClass );
//		pRemoteDBMgr->LOAN_GetOneValue( query, result );
	}
	delayInfo[ 1 ] = _ttoi( result );

	// �ݳ������� Ƚ��
	delayInfo[ 2 ] = _ttoi( pDM->GetCellData( _T("�ݳ�����Ƚ��"), row ) );

	// �ݳ���������, ����켱���� ��������
	query.Format( _T("SELECT VALUE_1 FROM CT_COMMON_LOAN_INFO_TBL WHERE LIB_INFO_KEY = '%s' AND VALUE_NAME = '�ݳ����⿹��켱����'"), libKey );
//	pRemoteDBMgr->LOAN_GetOneValue( query, result );
	if( _ttoi( result ) )	// ����켱�϶� �����ѻ���� �ִ��� ã�´�. 
	{
		query.Format( _T("SELECT COUNT( * ) FROM CT_CO_LOAN_INFO_TBL WHERE LIB_CODE = '%s' AND STATUS = 3 AND CLIENT_REC_KEY = %s"), 
			libCode, pDM->GetCellData( _T("Client���ڵ�KEY"), row ) );
//		pRemoteDBMgr->LOAN_GetOneValue( query, result );
		delayInfo[ 3 ] = _ttoi( result );
	}
	else
		delayInfo[ 3 ] = 0;
	
	// �����ڰ� �ش� �������� �ݳ���ü�� ������ �ִ��� ����. 
	query.Format( _T("SELECT COUNT( * ) FROM CT_CO_LOAN_INFO_TBL WHERE LIB_CODE = '%s' AND STATUS IN( '0', '2' ) AND RETURN_PLAN_DATE < TRIM(SYSDATE) AND MASTER_USER_KEY = %s"),
		libCode, pDM->GetCellData( _T("�̿����ֽ�����������KEY"), row ) );
//	pRemoteDBMgr->LOAN_GetOneValue( query, result );
	delayInfo[ 4 ] = _ttoi( result );

	// �ݳ������� �ο� ����
	query.Format( _T("SELECT VALUE_1 FROM CT_COMMON_LOAN_INFO_TBL WHERE LIB_INFO_KEY = '%s' AND VALUE_NAME = '�ݳ�����ο�����'"), libKey );
//	pRemoteDBMgr->LOAN_GetOneValue( query, result );
	if( _ttoi( result ) )	// 0 : �ݳ������Ͽ� +
		delayInfo[ 5 ] = 1;
	else					// 1 : ���ÿ� +
		delayInfo[ 5 ] = 0;


	return 0;

	EFS_END
	return -1;

}


/*
�������ڵ�
	141028
����
	004
��ϱ���
	EM
������key
	123

*/

// �ݳ��������� �ް������� �ƴ��� ����. 
CString CUnionUserInfoViewer::GetReturnPlanDate( CESL_DataMgr *pDM, int row, int flag, int cnt, CString libKey )
{
	EFS_BEGIN

	CStringList sYearList;
	CStringList sMonthList;
	CStringList sDayList;
	GetLibHoliDayInfo( libKey, sYearList , sMonthList , sDayList );

	// DM_UnionUserInfoViewer_HoliDay;

	CString returnPlanDate;
	int nY, nM, nD;
	if( flag == 0 )
	{
		returnPlanDate = pDM->GetCellData( _T("�ݳ�������"), row );
		_stscanf( returnPlanDate, _T("%d/%d/%d"), &nY, &nM, &nD );
	}
	else
	{
		// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
		CString strGetData;
		CTime t = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
// 		CTime t = pDM->GetDBTime();
		nY = t.GetYear();
		nM = t.GetMonth();
		nD = t.GetDay();
	}

	CTime time( nY, nM, nD, 0, 0, 0 );
	time += CTimeSpan( cnt, 0, 0, 0 );
	nY = time.GetYear();
	nM = time.GetMonth();
	nD = time.GetDay();

	POSITION yPos = sYearList.GetHeadPosition();
	POSITION mPos = sMonthList.GetHeadPosition();
	POSITION dPos = sDayList.GetHeadPosition();
	CString y, m, d;
	
	CStringList days;
	CString tmp;
	
	POSITION POS;
	while( yPos )
	{
		y = sYearList.GetNext( yPos );
		m = sMonthList.GetNext( mPos );
		d = sDayList.GetNext( dPos );

		GetLibDays( d, days );
		POS = days.GetHeadPosition();
		while( POS )
		{
			tmp = days.GetNext( POS );

			if( _ttoi( y ) == nY && _ttoi( m ) == nM && _ttoi( tmp ) == nD )
			{
				nD++;
				CheckLeapYear( nY, nM, nD );
			}
		}
	}	
	
	returnPlanDate.Format( _T("%04d/%02d/%02d"), nY, nM, nD );
	return returnPlanDate;

	EFS_END
	return _T("");

}


void CUnionUserInfoViewer::GetLibHoliDayInfo( CString libKey, CStringList &sYearList, CStringList &sMonthList, CStringList &sDayList )
{
	EFS_BEGIN

	CString where, DMAlias;
	DMAlias = _T("DM_UnionUserInfoViewer_HoliDay");
	where.Format( _T("LIB_INFO_KEY = '%s'"), libKey );
	
	CESL_DataMgr *pDM = FindDM( DMAlias );
	CRS_SQL RemoteDBMgr( this );
//	RemoteDBMgr.SetFetchCount( 0, 100 );
//	RemoteDBMgr.LOAN_Select( pDM, where, DMAlias, 1, 1 );
	
	int locate = 0;
	CString year, mon, day, tmp;
	for( int i = 0; i < pDM->GetRowCount(); i++ )
	{
		sYearList.AddTail(  pDM->GetCellData( _T("YEAR"), i  ) );
		sMonthList.AddTail( pDM->GetCellData( _T("MONTH"), i ) );
		sDayList.AddTail(	pDM->GetCellData( _T("DAYS"), i  ) );
	}

	EFS_END
}

void CUnionUserInfoViewer::GetLibDays( CString dd, CStringList &days )
{
	EFS_BEGIN

	dd.Replace( _T(" "), _T("") );
	days.RemoveAll();

	int locate = 0;
	CString tmp;

	do
	{
		locate = dd.Find( _T(","), locate );
	
		if( locate > -1 )
		{
			tmp = dd.Left( locate + 1 );
			dd = dd.Right( dd.GetLength() - tmp.GetLength() );
			tmp.Replace( _T(","), _T("") );
			days.AddTail( tmp );
		}
		else
			days.AddTail( dd );
	}
	while( locate > -1 );

	EFS_END
}

bool CUnionUserInfoViewer::CheckLeapYear( int &y, int &m, int &d )
{
	EFS_BEGIN

   int maxDay[]= { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

   if( maxDay[ m - 1 ] < d )
		d = maxDay[ m - 1 ];

   if( ( 2 == m ) &&
	   ( 0 == y % 4 ) &&
	   ( ( 0 != y % 100 ) || ( 0 == y % 400 ) ) )
   {
		if( 28 < d )
		{
			d = 1;
			if( 12 == m )
			{
				y++;
				m = 1;
			}
			else
				m++;
				
			return false;
		}
   }
   else if( maxDay[ m - 1 ] < d )
   {
		d = 1;
		if( 12 == m )
		{
			y++;
			m = 1;
		}
		else
			m++;

	   return false;
   }

   return true;

   EFS_END
	return FALSE;

}

void CUnionUserInfoViewer::ViewLoanLogInfoTypeDlg()
{
	EFS_BEGIN
	
	m_pLoanLogInfoTypeDlg = new CLoanLogInfoTypeDlg( this );
	m_pLoanLogInfoTypeDlg->Create( this );
	
	CStatic *pStatic = ( CStatic * )GetDlgItem( IDC_sLOAN_LOG_INFO_TYPE );
	CRect rect;
	pStatic->GetWindowRect( &rect );
	ScreenToClient( &rect );
	m_pLoanLogInfoTypeDlg->MoveWindow( rect.left, rect.top, rect.Width(), rect.Height() );

	m_pLoanLogInfoTypeDlg->ShowWindow( true );

	EFS_END
}

CUnionUserInfoViewer::~CUnionUserInfoViewer()
{
	if( m_bThreadWorking )
		TerminateThread( m_pThread->m_hThread, 0 );
		
	if( m_pLoanLogInfoTypeDlg )
	{
		delete m_pLoanLogInfoTypeDlg;
		m_pLoanLogInfoTypeDlg = NULL;
	}

	if ( m_pNoticeMessage != NULL)
	{
		delete m_pNoticeMessage;
	}

}

void CUnionUserInfoViewer::GetLibList()
{
	EFS_BEGIN
	
	CESL_DataMgr *pDM = FindDM( _T("DM_UnionUserInfoViewer_LIB_LIST") );
	CRS_SQL RemoteDBMgr( this );
//	RemoteDBMgr.SetFetchCount( 0, 100 );
//	RemoteDBMgr.LOAN_Select( pDM, _T(""), _T(""), 1, 1 );

	EFS_END
}

void CUnionUserInfoViewer::MakeUserLoanLogWhere(CString &where, CString pk)
{
	EFS_BEGIN

	where.Format( _T("MASTER_USER_KEY = ( SELECT MASTER_USER_KEY FROM CT_LIBRARY_LOAN_USER_TBL WHERE CLIENT_REC_KEY = %s AND LIB_CODE = '%s' )"), pk, m_libCode );

	const int cnt = 6;
	int type[ cnt ];
	m_pLoanLogInfoTypeDlg->GetLoanLogInfoType( type );

	CString tmp, buf;
	for( int i = 0; i < cnt; i++ )
		if( type[ i ] )
		{
			tmp.Format( _T("'%d', "), i );
			buf += tmp;
		}
	if( !buf.IsEmpty() )
	{
		tmp = buf.Left( buf.GetLength() - 2 );
		buf.Format( _T(" AND STATUS IN( %s )"), tmp );
		where += buf;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_UnionUserInfoViewer_LIB_LIST") );
	int libIndex = m_pLoanLogInfoTypeDlg->GetLibDMIndex();
	if( -1 != libIndex )
	{
		tmp.Format( _T(" AND LIB_CODE = '%s'"), pDM->GetCellData( _T("CODE"), libIndex ) );
		where += tmp;
	}

	EFS_END
}

BOOL CUnionUserInfoViewer::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	m_tBtnToolTip.RelayEvent( pMsg );
	
	if( pMsg->message == WM_KEYDOWN )
	{
		switch( pMsg->wParam )
		{
			case VK_ESCAPE :
				CloseWindow();
				return NULL;
				break;
			default :
				break;
		}
	}

	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}

void CUnionUserInfoViewer::CloseWindow()
{
	EFS_BEGIN

	this->ShowWindow( false );

	EFS_END
}

BEGIN_EVENTSINK_MAP(CUnionUserInfoViewer, CDialog)
    //{{AFX_EVENTSINK_MAP(CUnionUserInfoViewer)
	ON_EVENT(CUnionUserInfoViewer, IDC_gLOAN_LOG, -600 /* Click */, OnClickgLOANLOG, VTS_NONE)
	ON_EVENT(CUnionUserInfoViewer, IDC_gUSER_INFO2, -600 /* Click */, OnClickgUSERINFO2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CUnionUserInfoViewer::OnClickgLOANLOG() 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gLOAN_LOG );
	int row = pGrid->GetRow();
	int col = pGrid->GetCol();

	if( 1 != col )
		return;

	CESL_DataMgr *pDM = FindDM( _T("DM_UnionUserInfoViewer_LoanInfo") );
	if( !pDM->GetRowCount() )
		return;

	int status = _ttoi( pDM->GetCellData( _T("�������"), row - 1) );
	if( 2 != status && 0 != status )
	{
		AfxMessageBox( _T("�����Ͻ� ������ �������� �ƴϹǷ� ������ �� �����ϴ�.") );
		return;
	}

	if( _T("V") == pGrid->GetTextMatrix( row, col ) )
		pGrid->SetTextMatrix( row, col, _T("") );
	else
		pGrid->SetTextMatrix( row, col, _T("V") );

	EFS_END
}

void CUnionUserInfoViewer::DelayRetrunBook()
{
	EFS_BEGIN

	CArray< int, int >checkedRow;
	GetCheckedGridRow( checkedRow );
	int cnt = checkedRow.GetSize();
	if( !cnt )
	{
		AfxMessageBox( _T("�ݳ������� ������ �����Ͽ� �ֽʽÿ�") );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_UnionUserInfoViewer_LoanInfo") );
	CRS_SQL RemoteDBMgr( this );
	// KOL.UDF.022
	//	int delayInfo[ 5 ];		
	int delayInfo[ 6 ];		// 0 : �ݳ����Ⱑ��Ƚ��
							// 1 : �ݳ������ϼ�
							// 2 : �ݳ������� Ƚ��
							// 3 : ����켱�϶� ������ �������
							// 4 : �ݳ������Ϸ��� ������ �������� �ݳ���ü�� ������ ����
							// 5 : �ݳ������Ϻο����� 
	int DMRow;				// ���õ� grid�� DM row;
	CString insertQuery,	// INSERT QUERY
			requestDate,	// �����
			libCode,		// ��������ȣ
			libKey,			// ������ KEY
			recKey,			// UPDATE �� ���ڵ� key
			updateSetQry,	// UPDATE SET QUERY
			firstWork;		// FIRST WORK

	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	CTime time = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
// 	CTime time = pDM->GetDBTime();
	requestDate.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	firstWork = GetWorkLogMsg( _T("���͵��������ݳ������û�ϱ�"), __WFILE__, __LINE__ );

	int successCnt[ 6 ] = { 0, 0, 0, 0, 0, 0 };	
	for( int i = 0; i < cnt; i++ )
	{
		DMRow = checkedRow.GetAt( i );
		switch( GetReturnBookDelayInfo( pDM, &RemoteDBMgr, DMRow, delayInfo, libKey ) )
		{
			case -1 :
				successCnt[ 4 ]++;
				continue;
				break;
			default :
				break;
		}

		if( delayInfo[ 3 ] )	// ����켱�������� ���� ������ ����� ������
		{
			successCnt[ 1 ]++;
			continue;
		}

		if( delayInfo[ 4 ] )	// �ݳ������Ϸ��� �������� �ݳ���ü�� ������ ������
		{
			successCnt[ 2 ]++;
			continue;
		}

		if( delayInfo[ 0 ] > delayInfo[ 2 ] )	// �ݳ����� ���ɱǼ��� �ʰ��Ͽ����ϴ�. 
		{
			// 2009.04.06 UPDATE BY CJY : �ݳ������ϼ��� 1���� 30�ϻ��̷� �Ѵ�.
			if(1 <= delayInfo[1] && delayInfo[1] <= 30)
			{
				libCode = pDM->GetCellData( _T("��������ȣ"),		DMRow );
				recKey  = pDM->GetCellData( _T("Client���ڵ�KEY"),  DMRow );

				// TO_DATE( '%s', 'YYYY/MM/DD' ) + %d
				updateSetQry.Format( _T("DELAY_CNT = %d, RETURN_PLAN_DATE = TO_DATE( '%s', 'YYYY/MM/DD' ), DELAY_RETURN_DATE = SYSDATE, PRE_RETURN_PLAN_DATE = TO_DATE( '%s', 'YYYY/MM/DD' ), STATUS = '2' "),
					delayInfo[ 2 ] + 1,															// �ݳ����� Ƚ��
					GetReturnPlanDate( pDM, DMRow, delayInfo[ 5 ], delayInfo[ 1 ], libKey ),	// ����� �ݳ�������
					pDM->GetCellData( _T("�ݳ�������"), DMRow ) );									// ���� �ݳ� ������
				updateSetQry.Replace( _T("'"), _T("''") );

				insertQuery.Format( _T("INSERT INTO CT_TEMP_DELAY_INFO_TBL( REC_KEY, REQUEST_LIB_CODE, UPDATE_LIB_CODE, UPDATED_TABLE_REC_KEY, UPDATED_SET_QUERY, REQUEST_DATE, STATUS, FIRST_WORK ) VALUES( ESL_SEQ.NEXTVAL, '%s', '%s', %s, '%s', '%s', '0', '%s' );"),
					m_libCode,		// ��û ������
					libCode,		// ������ ������
					recKey ,		// LOAN Table�� REC_KEY
					updateSetQry,	// UPDATE Query
					requestDate,	// ��û����
					firstWork );	// FIRST WORK
				
//				RemoteDBMgr.LOAN_UnSelect( insertQuery );

				successCnt[ 0 ]++;
			}
			else
			{
				successCnt[ 5 ]++;
			}
		}
		else
			successCnt[ 3 ]++;
	}
	
// 	CString msg;
// 	if( !successCnt[ 1 ] && !successCnt[ 2 ] && !successCnt[ 3 ] )
// 		msg.Format( _T("%d ���� �ڷᰡ ���������� �ݳ����� �Ǿ����ϴ�."), successCnt[ 0 ] );
// 	else
// 		msg.Format( _T("������ �ݳ� ����			: %d\r\n")
// 					_T("������������ ã�� �� ����		: %d\r\n")
// 					_T("������ ����			: %d\r\n")
// 					_T("��ü��������			: %d\r\n")
// 					_T("�ݳ����Ⱑ��Ƚ���ʰ�		: %d\r\n"), 
// 					successCnt[ 0 ], successCnt[ 4 ], successCnt[ 1 ], successCnt[ 2 ], successCnt[ 3 ] );
	CString msg;
	if(    0 != successCnt[ 1 ] 
		&& 0 != successCnt[ 2 ] 
		&& 0 != successCnt[ 3 ] 
		&& 0 != successCnt[ 4 ] 
		&& 0 != successCnt[ 5 ]
		)
	{
		msg.Format( _T("%d ���� �ڷᰡ ���������� �ݳ����� �Ǿ����ϴ�."), successCnt[ 0 ] );
	}
	else
	{
		msg.Format( _T("������ �ݳ� ����		    	: %d\r\n")
					_T("������������ ã�� �� ����		: %d\r\n")
					_T("������ ����	            		: %d\r\n")
					_T("��ü��������	        		: %d\r\n")
					_T("�ݳ����Ⱑ��Ƚ���ʰ�	    	: %d\r\n")
					_T("�ݳ������ϼ��� 0������ 31���̻�	: %d\r\n"), 
					successCnt[ 0 ], successCnt[ 4 ], successCnt[ 1 ], successCnt[ 2 ], successCnt[ 3 ], successCnt[ 5 ] );
	}

	MessageBox( msg );	

	EFS_END
}

void CUnionUserInfoViewer::OnbRELOAD() 
{
	EFS_BEGIN	
	
	( (CButton*) GetDlgItem (IDC_bRELOAD) )->EnableWindow(FALSE);

	if( !m_userKey.IsEmpty() )
		SearchCenterUserInfo( m_userKey );

	//CheckLocalDaemonToDB();

	EFS_END
}

void CUnionUserInfoViewer::SetDefaultSize()
{
	EFS_BEGIN

	CRect rect;
	GetCtrlRect( IDC_sDEFAULT_SIZE, rect );

	this->MoveWindow( rect.left, rect.top, rect.Width(), rect.Height() );

	EFS_END
}

void CUnionUserInfoViewer::GetCtrlRect( unsigned int id, CRect &rect )
{
	EFS_BEGIN

	CWnd *pWnd = ( CWnd * )GetDlgItem( id );
	if( !pWnd->GetSafeHwnd() )
		return;

	pWnd->GetWindowRect( &rect );
	ScreenToClient( &rect );

	EFS_END
}

void CUnionUserInfoViewer::OnSize(UINT nType, int cx, int cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	if( !this->GetSafeHwnd() )
		return;
	
//	Resize();	

	EFS_END
}

void CUnionUserInfoViewer::Resize()
{
EFS_BEGIN

	CRect wndRect, sRect, mRect;
	
	this->GetWindowRect( &wndRect );
	CWnd *pWnd = NULL; 
		
	int cx = wndRect.Width();
	int cy = wndRect.Height();	
	
	GetCtrlRect( IDC_sDEFAULT_SIZE,		sRect );
	GetCtrlRect( IDC_sMAX_DEFAULT_SIZE, mRect );	

	if ( m_loanLogView == false && m_loanUserView == false)
	{			
		if( wndRect.Height() < sRect.Height() )
		{			
			this->MoveWindow( wndRect.left, wndRect.top, sRect.Width(), 360 );
		}
		else
		{
			this->MoveWindow( wndRect.left, wndRect.top, sRect.Width(), 360 );
			MoveUserInfoCtrls( sRect, 360 );			
		}		
	}
	else if ( m_loanLogView == false && m_loanUserView == true)
	{
		if( wndRect.Height() < sRect.Height() )
			this->MoveWindow( wndRect.left, wndRect.top, sRect.Width(), sRect.Height() );
		else
		{
			this->MoveWindow( wndRect.left, wndRect.top, sRect.Width(), wndRect.Height() );
			MoveUserInfoCtrls( sRect, cy );			
		}
	}
	else
	{
		if( wndRect.Height() < mRect.Height() )
			this->MoveWindow( wndRect.left, wndRect.top, wndRect.Width(), mRect.Height() );
		else if( wndRect.Width() < mRect.Width() )
			this->MoveWindow( wndRect.left, wndRect.top, mRect.Width(), wndRect.Height() );
		else
		{
			MoveUserInfoCtrls( sRect, cy );
			MoveUserLoanLogCtrls( sRect, cy );
		}

		this->GetWindowRect( &wndRect );
		m_maxWidth = wndRect.Width();
	}
EFS_END
}

void CUnionUserInfoViewer::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);	
	
	if( bShow )
	{
		if( !m_bFirstShow )
		{
			m_bFirstShow = TRUE;

			CRect rcFrame;
			CRect rc;
			pMain->GetParentFrame()->GetWindowRect(&rcFrame);
			this->GetWindowRect(&rc);
			this->MoveWindow(rcFrame.left, rcFrame.bottom - rc.Height(), rc.Width(), rc.Height());

		}

		SearchCenterUserInfo( m_userKey );
	}

	EFS_END
}

void CUnionUserInfoViewer::MoveUserInfoCtrls( CRect rect, int cy )
{
	EFS_BEGIN

	const int cnt = 3;
	unsigned int id[ cnt ] = {
		IDC_sUSER_INFO,			// 0 ������ ���� �ڽ�
		IDC_gUSER_INFO,
		IDC_gUSER_INFO2			// �������� ������ ���� 
	};		// 1 ������ ���� �׸���
	int height[ cnt ] = { 77, 105 };

	int nLibUserGridPos;
	CWnd *pWnd = NULL;
	for( int i = 0; i < cnt; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( id[ i ] );
		if( pWnd->GetSafeHwnd() == NULL ) return ;
			
		GetCtrlRect( id[ i ], rect );
		if( i == 1 )
		{
			nLibUserGridPos =  rect.top + rect.Height() + ( cy - rect.Height() ) - height[ i ] - 260;
			pWnd->MoveWindow( rect.left, rect.top, rect.Width(), rect.Height() + ( cy - rect.Height() ) - height[ i ] - 260 );		
		}
		else if( i == 0 )
		{
			pWnd->MoveWindow( rect.left, rect.top, rect.Width(), rect.Height() + ( cy - rect.Height() ) - height[ i ] );					
		}
		else if( i == 2 )
			pWnd->MoveWindow( rect.left, nLibUserGridPos+10 , rect.Width(), 250 );					
	}

	EFS_END
}

void CUnionUserInfoViewer::MoveUserLoanLogCtrls( CRect sRect, int cy )
{
	EFS_BEGIN

	CRect wndRect, rect, rectAllInfo;
	this->GetWindowRect( &wndRect );	
	CWnd *pWnd = NULL;

	const int cnt = 3;
	unsigned int id[ cnt ] = {
		IDC_sUSER_LOAN_LOG,		// 0 �����̷� ���� �ڽ�
		IDC_gLOAN_LOG,
		IDC_gUSER_TOTAL_INFO };		// 1 �����̷� ���� �׸���
	int width[ cnt ] = {
		5, 19, 19};
	int height[ cnt ] = {
		38, 122, 80 };

	int nLibUserGridPos;
	GetCtrlRect( IDC_gUSER_INFO2, rectAllInfo );
	
	for( int i = 0; i < cnt; i++ )
	{
		GetCtrlRect( id[ i ], rect );
		pWnd = ( CWnd * )GetDlgItem( id[ i ] );
		if( pWnd->GetSafeHwnd() )
		{
			if ( i == 0 )
			{
				pWnd->MoveWindow( rect.left, rect.top, wndRect.Width() - sRect.Width() - width[ i ], wndRect.Height() - height[ i ] );
			}
			else if ( i == 1 )
			{
				// 2005.06.22 UPDATE BY PDJ
				// �������������� �����ֱ� ���� ����Ȯ��
				nLibUserGridPos = rect.top + rect.Height() + ( cy - rect.Height() ) - 231;
				pWnd->MoveWindow( rect.left, rect.top, wndRect.Width() - sRect.Width() - width[ i ], wndRect.Height() - height[ i ] -100 );
			}
			else if ( i == 2 )
			{
				// 2005.06.22 UPDATE BY PDJ				
				pWnd->MoveWindow( rect.left, nLibUserGridPos+10, wndRect.Width() - sRect.Width() - width[ i ], 90  );
			}
		}
	}

	

	EFS_END
}

void CUnionUserInfoViewer::MakeToolTip()
{
	EFS_BEGIN

	const int cnt = 6;
	unsigned int id[ cnt ] = {
		IDC_bRELOAD, IDC_bS_M, IDC_bDELAY_BOOK_RETURN  , IDC_bLOAN_LOG_VIEW , IDC_STATIC_MOVE, IDC_BTN_CANCEL};
	LPCTSTR lpszText[ cnt ] = {
		_T("�ٽ��б�"), _T("�����̷º���"), _T("�ݳ�����") , _T("�����̷¼����") , _T("���콺 �巡�׷� �̵��� �����մϴ�") , _T("�ݱ�") };

	CWnd *pWnd = NULL;
	m_tBtnToolTip.Create( this );
	m_tBtnToolTip.SetMaxTipWidth(400);
	m_tBtnToolTip.Activate(TRUE);

	for( int i = 0; i < cnt; i++ )
	{
		pWnd = GetDlgItem( id[ i ] );
		m_tBtnToolTip.AddTool( pWnd , lpszText[ i ] );
	}


	m_tBtnToolTip.SendMessage(TTM_SETDELAYTIME,(WPARAM)TTDT_INITIAL,MAKELONG(0,0));
	m_tBtnToolTip.SendMessage(TTM_SETDELAYTIME,(WPARAM)TTDT_AUTOPOP,MAKELONG(20000,0));

	EFS_END
}

/*
LRESULT CUnionUserInfoViewer::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	switch( message )
	{
		case WM_LBUTTONDOWN :
		case WM_RBUTTONDOWN :
		case WM_MBUTTONDOWN :
		case WM_LBUTTONUP	:
		case WM_RBUTTONUP	:
		case WM_MBUTTONUP	:
		case WM_MOUSEMOVE	:
			MSG msg;
			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;

			m_tBtnToolTip.RelayEvent( &msg );			
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}
*/

void CUnionUserInfoViewer::MakeBitmapBtns()
{
	EFS_BEGIN

	m_bitmap[ 0 ].LoadBitmap( IDB_bR );	
	m_bitmap[ 1 ].LoadBitmap( IDB_bS );	
	m_bitmap[ 2 ].LoadBitmap( IDB_bS_M );	
	m_bitmap[ 3 ].LoadBitmap( IDB_bD );	
	m_bitmap[ 4 ].LoadBitmap( IDB_bM2 );
	m_bitmap[ 5 ].LoadBitmap( IDB_bC2 );

	m_bR.SetBitmap(   m_bitmap[ 0 ] );
	m_bS.SetBitmap(	  m_bitmap[ 1 ] );
	m_bS_M.SetBitmap( m_bitmap[ 2 ] );
	m_bD.SetBitmap(   m_bitmap[ 3 ] );
	m_sM.SetBitmap(  m_bitmap[ 4 ] );
	m_bC.SetBitmap(   m_bitmap[ 5 ] );

	EFS_END
}

void CUnionUserInfoViewer::OnbSM() 
{
	EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	ViewUserLoanLogGrid();	
//	Resize();
	ViewResizeBtns();
	InitTotalGrid();

	EFS_END
}

void CUnionUserInfoViewer::ViewResizeBtns()
{
	EFS_BEGIN

	if( !m_UseCenterUserLoanLog )
		return;
	
	const int cnt = 2;
	bool flag[ cnt ] = { true, true };
	unsigned int id[ cnt ] = {
		IDC_bLOAN_LOG_VIEW, IDC_bS_M };
	
	CButton *pBtn = NULL;
	if( m_loanLogView )
		flag[ 1 ] = false;
	else
		flag[ 0 ] = false;

	for( int i = 0; i < cnt; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( id[ i ] );
		pBtn->ShowWindow( flag[ i ] );
	}

	CWnd *pWnd = ( CWnd * )GetDlgItem( IDC_sDEFAULT_SIZE );
	pWnd->SetFocus();

	EFS_END
}

void CUnionUserInfoViewer::ShowGrid( WPARAM w, LPARAM l )
{
	EFS_BEGIN

	if( l )
	{
		ControlDisplay( _T("CM_UnionUserInfoViewer"), _T("GRID") );
	}
	ControlDisplay( _T("CM_UnionUserInfoViewer"),_T("GRID2") );

	//--------------------------------------------------------
	// 2005-11-25 
	// WORK ADD BY PDW(Jr)
	// ������������� �߰� ������ ����.
	if ( m_UseCenterUserLoanLog )
		ShowCenterUserAddInfo();		
	//--------------------------------------------------------

	/// �׸����� ���� �����ϱ�
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_UnionUserInfoViewer") , _T("GRID2") );
	if( pGrid == NULL ) return ;	
	
	GridColoring();

	m_bThreadWorking = false;

	EFS_END
}


int CUnionUserInfoViewer::GridColoring()
{
	EFS_BEGIN

	/// �׸����� ���� �����ϱ�
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_UnionUserInfoViewer") , _T("GRID2") );
	if( pGrid == NULL ) return -1;

	int nRowCnt = pGrid->GetCount();

	CString sStatus;
	for( int i = 0 ; i < nRowCnt ; i++ )
	{
		sStatus = pGrid->GetTextMatrix( i+1 , 2 );

		if( sStatus.Compare(_T("��������ȸ��")) == 0 )
		{
			pGrid->SetRow(i+1);
			pGrid->SetCol(2);
			pGrid->SetCellForeColor(255);
		}
		else
		{
			pGrid->SetRow(i+1);
			pGrid->SetCol(2);
			pGrid->SetCellForeColor(0);
		}
	}
	

	return 0;
	EFS_END
	return -1;

}

void CUnionUserInfoViewer::OnClickgUSERINFO2() 
{
	EFS_BEGIN

	GridColoring();	

	EFS_END
}


void CUnionUserInfoViewer::ShowCenterUserTotalInfo()
{	
	EFS_BEGIN

	CString DMAlias = _T("DM_UnionUserInfoViewer_LoanInfo");
	CESL_DataMgr *pDM = FindDM( DMAlias );
	CString		strDate;
	CString		strStatus;
	CString		strTmp, strTmp2;
	INT			nRowCount;
	INT			i;
	INT			nDisplayCnt[8] = {0,0,0,0,0,0,0,0};	
	
	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	CTime tCurrentCTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	strDate = strGetData.Left(10);
// 	CTime tCurrentCTime = pDM->GetDBTime();
// 	strDate.Format(_T("%04d/%02d/%02d"),tCurrentCTime.GetYear() , tCurrentCTime.GetMonth() , tCurrentCTime.GetDay() );	

	CMSHFlexGrid* grid2 = NULL;	
	if ( grid2 == NULL )
	{
		grid2 = ( (CMSHFlexGrid*) GetDlgItem (IDC_gUSER_TOTAL_INFO) );
		if ( grid2 == NULL ) return;
	}	

	nRowCount = pDM->GetRowCount();

	if ( nRowCount == 0 ) 
	{
		for ( i=0 ; i<5 ; i++ )
		{
			grid2->SetTextMatrix(i,1, _T(""));
		}
//		(( CStatic*) GetDlgItem (IDC_sMESSAGE))->SetWindowText("�˻��Ϸ�");
//		return;
	}
	else
	{
		for ( i=0 ; i<nRowCount ; i++ )
		{
			// ���� & ��ü �Ǽ����ϱ�
			pDM->GetCellData(_T("�������"), i, strStatus );
			
			switch ( _ttoi(strStatus) )
			{
				case 0:
				case 2:
				case 5:
					pDM->GetCellData(_T("������"), i, strTmp );
					pDM->GetCellData(_T("�ݳ�������"), i, strTmp2 );				
					if ( strDate <= strTmp2 ) 
					{
						// ������ 
						nDisplayCnt[0] ++;
					}
					else 
					{
						// ������ 
						nDisplayCnt[0] ++;
						// ������ ��ü
						nDisplayCnt[1] ++;
					}
					break;
				case 1:
					pDM->GetCellData(_T("�ݳ���"), i, strTmp );
					pDM->GetCellData(_T("�ݳ�������"), i, strTmp2 );
					if ( strTmp <= strTmp2 ) 
					{
						// �ݳ�
						nDisplayCnt[2] ++;
					}
					else 
					{
						// �ݳ�
						nDisplayCnt[2] ++;
						// �ݳ� ��ü
						nDisplayCnt[3] ++;
					}

					break;
				case 3:
					// ����
					nDisplayCnt[4] ++;
					break;		
				case 4:
					// �������
					nDisplayCnt[5] ++;
					break;			
			}
			// �� ��ü�� �� 
			pDM->GetCellData(_T("��ü��"), i, strTmp );
			nDisplayCnt[6] += _ttoi(strTmp);

			// �� ��ü�� ��
			pDM->GetCellData(_T("��ü��"), i, strTmp );
			nDisplayCnt[7] += _ttoi(strTmp);
		}
	}

	CString	strCnt;

	for ( i=0 ; i<5 ; i++ )
	{			
		switch (i) 
		{
			///{{++
			/// 2008.05.09 UPDATE BY PDW
			/// �׸��忡�� ����Ʈ �ڽ��� ����
			case 0:
				strCnt.Format(_T(" %d ( %d ) "), nDisplayCnt[0], nDisplayCnt[1]);
//				grid2->SetTextMatrix(i,1,strCnt);
				SetDlgItemText(IDC_EDT_LOANCNT, strCnt);
				break;
			case 1:
				strCnt.Format(_T(" %d ( %d ) "), nDisplayCnt[2], nDisplayCnt[3]);
//				grid2->SetTextMatrix(i,1, strCnt);
				SetDlgItemText(IDC_EDT_RETURNCNT, strCnt);
				break;
			case 2:
				strCnt.Format(_T(" %d ( %d ) "), nDisplayCnt[4], nDisplayCnt[5]);
//				grid2->SetTextMatrix(i,1, strCnt);
				SetDlgItemText(IDC_EDT_RESERVATIONCNT, strCnt);
				break;
			case 3:
				strCnt.Format(_T(" %d "), nDisplayCnt[6]);
//				grid2->SetTextMatrix(i,1, strCnt);
				SetDlgItemText(IDC_EDT_DELAYCNT, strCnt);
				break;			
			case 4:
				strCnt.Format(_T(" %d "), nDisplayCnt[7]);
//				grid2->SetTextMatrix(i,1, strCnt);
				SetDlgItemText(IDC_EDT_DELAYMONEY, strCnt);
				break;		
			///}}--
		}	

		grid2->SetRowHeight(i, 250);
	}	

	(( CStatic*) GetDlgItem (IDC_sMESSAGE))->SetWindowText(_T("�˻��Ϸ�"));

	return;	
	EFS_END
	return;	
}

void CUnionUserInfoViewer::InitTotalGrid()
{		
	EFS_BEGIN

	CString	sFiledArray[] = { _T("����Ǽ�(������ ��ü�Ǽ�)"),_T("�ݳ��Ǽ�(�ݳ��� ��ü�Ǽ�)"),_T("����Ǽ�(������ҰǼ�)"),_T("�ѿ�ü�ϼ�"),_T("�ѿ�ü��") };	
	CString	strDate;
	INT	i;	

	CMSHFlexGrid* grid2 = NULL;
	if ( grid2 == NULL )
	{
		grid2 = ( (CMSHFlexGrid*) GetDlgItem (IDC_gUSER_TOTAL_INFO) );
		if ( grid2 == NULL ) return;
	}

	grid2->SetColWidth(0,0,2400);
	grid2->SetColWidth(1,0,900);

	for ( i=0 ; i<5 ; i++ )
	{		
		grid2->SetTextMatrix(i, 0, sFiledArray[i] );
	}		

	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [ Func Desc ]
//   ������ : ��������ȸ���޽������ ������ �����Ѵ�.
//   ������ : �˻��� �̿��ڰ� Ÿ�������������� ���������������� üũ
//   ESL_MANAGE �������� 'YN'���� '012'���� ����Ǹ鼭 �ܼ� BOOL������� 
//   �����Ҽ� ���⿡ �Լ� ����� ����
// [ Return Value ]
//   TRUE  : �˻��� ȸ�� �������� 
//   FALSE : �˻��� ȸ�� ����
/////////////////////////////////////////////////////////////////////////////
INT CUnionUserInfoViewer::GetLoanStopUserStatus()
{
EFS_BEGIN

	//=======================================================================
	// Variable Declare
	//=======================================================================
	INT	ids;
	CString	strLibCode;
	CString	strData;
	CString	sStatus;	
	INT	i;
	INT	nRowCnt;
	CESL_DataMgr* pLibUserDM = FindDM( _T("DM_UnionUserInfoViewer_LibUserInfo") );
	// 2006-05-24 REMOVE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++
	// �Լ���ɺ���
	/*
	ids = GetManageValue("��Ÿ","����","�����ڰ�������","��������ȸ���޽������",strData);
	if ( ids ) return FALSE;

	if ( strData != "Y" ) return FALSE;
	*/
	// ---------------------------------------------------------------------
	// 2009.03.23 UPDATE BY CJY : ������������ ȸ�����°� �ٸ���� ���������� ������ �����ش�.
	int nNormal = 0;
	int nLoanStop = 0;
	int nRemove = 0;
	nRowCnt = pLibUserDM->GetRowCount();

	for ( i=0 ; i<nRowCnt ; i++ )
	{
		ids = pLibUserDM->GetCellData( _T("��������ȣ") , i , strLibCode );
		if ( ids ) return -1;

		// Ÿ���� ���¸� üũ��
		if ( strLibCode == m_libCode ) 
		{
			continue;
		}

		ids = pLibUserDM->GetCellData( _T("ȸ������") , i , sStatus );
		if ( ids ) return -2;
 		if ( sStatus == _T("����ȸ��") ) 
 		{
 			nNormal++;
 		}
 		else if(sStatus == _T("��������") || sStatus == _T("��������ȸ��"))
 		{
			nLoanStop++;
 		}
 		else if(sStatus == _T("����ȸ��"))
 		{
			nRemove++;
 		}
 		else
 		{
 			ids = 3;
 			break;
 		}
	}

	if(0 < nLoanStop)	// 1���� ��������
	{
		ids = 1;
	}
	else if(0 < nRemove) // 2���� ����ȸ��
	{
		ids = 2;
	}
	else if(0 < nNormal) // 3���� ����ȸ��
	{
		ids = 0;
	}
	else // Ż��ȸ��
	{
		ids = 3;
	}
// 	nRowCnt = pLibUserDM->GetRowCount();
// 	for ( i = 0; i < nRowCnt; i++ )
// 	{
// 		ids = pLibUserDM->GetCellData( _T("��������ȣ"), i, strLibCode );
// 		if (0 > ids) 
// 		{
// 			return -1;
// 		}
// 
// 		// Ÿ���� ���¸� üũ��
// 		if ( strLibCode == m_libCode ) 
// 		{
// 			continue;
// 		}
// 
// 		ids = pLibUserDM->GetCellData( _T("ȸ������"), i, sStatus );
// 		if (0 > ids) 
// 		{
// 			return -2;
// 		}
// 		if (sStatus == _T("����ȸ��")) 
// 		{
// 			ids = 0;
// 			break;
// 		}
// 		else if(sStatus == _T("��������"))
// 		{
// 			ids = 1;
// 			break;
// 		}
// 		else if(sStatus == _T("����ȸ��"))
// 		{
// 			ids = 2;
// 			break;
// 		}
// 		else
// 		{
// 			ids = 3;
// 			break;
// 		}
// 	}

	return ids;

EFS_END
	return FALSE;
}

INT CUnionUserInfoViewer::ShowCenterUserAddInfo()
{
	INT ids;

	CString DMAlias = _T("DM_UnionUserInfoViewer_LibUserInfo");
	CESL_DataMgr* pDM = FindDM( DMAlias );
	if ( pDM == NULL )
	{			
		AfxMessageBox(_T(" ShowCenterUserAddInfo ERROR -1 "));
		return -1;
	}

	CESL_Grid* Grid2 = (CESL_Grid*)FindControl( _T("CM_UnionUserInfoViewer") , _T("GRID2") );		
	if ( Grid2 == NULL )
	{			
		AfxMessageBox(_T(" ShowCenterUserAddInfo ERROR -2 "));
		return -2;
	}

	///{{++
	/// 2008.04.23 update by pdw
	/// �Ѵ���å�� �� ��üȽ���� ���۷��������� �Ѵ�
/*					
	CString strCnt;
	INT nRowCnt = pDM->GetRowCount();
	for(int i = 0 ; i < nRowCnt ; i++ )
	{
		ids = pDM->GetCellData( "�Ѵ���å��" , i ,  strCnt);
		if( ids < 0 ) 
		{
			AfxMessageBox(" ShowCenterUserAddInfo ERROR -3 ");
			return -3;
		}
		Grid2->SetTextMatrix( i + 1, 4, strCnt);

		ids = pDM->GetCellData( "�ѿ�üȽ��" , i , strCnt );
		if( ids < 0 ) 
		{
			AfxMessageBox(" ShowCenterUserAddInfo ERROR -4 ");
			return -4;
		}
		Grid2->SetTextMatrix( i + 1, 5, strCnt);
	}
*/
	ControlDisplay(_T("CM_UnionUserInfoViewer") , _T("GRID2"));

	/// �Ѵ���å�� �߰�
	CString strTmp;
	INT nTotal = 0;

	INT nRowCnt = pDM->GetRowCount();
	for(int i = 0 ; i < nRowCnt ; i++ )
	{
		ids = pDM->GetCellData( _T("�Ѵ���å��") , i ,  strTmp);
		if( ids < 0 ) 
		{
			AfxMessageBox(_T(" ShowCenterUserAddInfo ERROR -3 "));
			return -3;
		}

		nTotal += _ttoi(strTmp);
	}

	strTmp.Format(_T("%d"), nTotal);
	SetDlgItemText(IDC_EDT_TOTAL, strTmp);
	///}}--
	return 0;
}

INT CUnionUserInfoViewer::SelfCloseMessageBox(CString sMsg, CESL_Mgr *pParentMgr, INT nDelayTime )
{
	EFS_BEGIN	
		

	if( pParentMgr == NULL ) pParentMgr = this;

	int nMode=0;

	if( sMsg.Find(_T("\n")) >= 0 )
		nMode = 1;

	if( sMsg.GetLength() > 30 )
		nMode = 1;


	if( m_pNoticeMessage == NULL )
	{
		m_pNoticeMessage = new CNoticeMessage( m_pParent );		
	}

	if( m_pNoticeMessage->GetSafeHwnd() != NULL )
	{
		m_pNoticeMessage->DestroyWindow();
		delete m_pNoticeMessage;
		m_pNoticeMessage = NULL;
		m_pNoticeMessage = new CNoticeMessage( m_pParent );
	}

	CRect pRect;

	m_pNoticeMessage->m_nDelayTime = nDelayTime;	
	m_pNoticeMessage->Create( m_pParent );
	m_pNoticeMessage->SetMsg( sMsg , nMode );
	m_pNoticeMessage->CenterWindow(m_pParent);		
	
	m_pNoticeMessage->GetWindowRect(pRect);

	m_pNoticeMessage->SetWindowPos(&wndTop, pRect.left, pRect.top, pRect.Width(), pRect.Height() , SWP_NOSIZE|SWP_NOMOVE);
	m_pNoticeMessage->ShowWindow( SW_SHOW );	
	

	return 0;
  
	EFS_END
	
	return -1;
}

BOOL CUnionUserInfoViewer::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CUnionUserInfoViewer::OnShowMessage()
{
	SelfCloseMessageBox(_T("Ÿ ������ �������� ȸ���Դϴ�."), NULL, 2000);	
}
VOID CUnionUserInfoViewer::OnShowMessage1()
{
	SelfCloseMessageBox(_T("Ÿ ������ ����ȸ���Դϴ�."), NULL, 2000);	
}

INT CUnionUserInfoViewer::CheckLocalDaemon()
{
	CSocket *pSocket = new CSocket();
	BOOL bConnect;

	bConnect = pSocket->Create();

	int j = pSocket->Connect(_T("192.168.1.140"), 43200);
	int i = pSocket->GetLastError();
	pSocket->Close();	   

	return 0;
}

BOOL CUnionUserInfoViewer::CheckLocalDaemonToDB()
{
	CString DMAlias = _T("DM_UnionESL_ManageInfo");
	CESL_DataMgr* pDM = FindDM( DMAlias );

	CString strLastLogDate;	
	INT		nInteval;
	pDM->GetOneValueQuery(_T("SELECT (SYSDATE - LAST_LOG_DATE)*24*60 FROM CO_DAEMON_LOG_TBL WHERE DAEMON_ID=1"), strLastLogDate);
	nInteval = _ttoi(strLastLogDate.GetBuffer(0));
	if ( nInteval >= 10 ) 
	{
		::Beep(1000,50);
		SelfCloseMessageBox(_T("������������ ���񽺰� �����Ǿ����ϴ�. �����ڿ��� �����Ͻʽÿ�."), NULL, 4000);
		( (CButton*) GetDlgItem (IDC_bRELOAD) )->EnableWindow(TRUE);
		return FALSE;
	}	

	return TRUE;	
}


void CUnionUserInfoViewer::ProcessTimeLog(CString strProcessID)
{
EFS_BEGIN	

	if (strProcessID == "") return;
	CTime t = CTime::GetCurrentTime();
	CString filename, msg;	
	filename.Format(_T("..\\tunning\\LOAN_SHARE_%04d%02d%02d.txt"), t.GetYear(), t.GetMonth(), t.GetDay());	
	msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d] %s;"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), strProcessID);

	FILE* fp;

	fp = _tfopen (filename, _T("a+"));
	if (fp == NULL)
	{
		return ;
	}
	_ftprintf (fp, _T("%s\n"), msg);

	fclose(fp);

EFS_END
}

void CUnionUserInfoViewer::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == 1 && m_bThreadWorking == FALSE )
	{
		KillTimer(1);
		SearchTimer();
	}
		
	CDialog::OnTimer(nIDEvent);
}


VOID CUnionUserInfoViewer::SearchTimer()
{
	EFS_BEGIN	

	ProcessTimeLog("SearchTimer.1");
	(( CStatic*) GetDlgItem (IDC_sMESSAGE))->SetWindowText(_T("����ȭ���Դϴ�."));
	m_pThread = AfxBeginThread( ViewUnionUserInfoThread, ( LPVOID )this );
	ProcessTimeLog("SearchTimer.2");

	EFS_END	
}

// 2008.02.28 add by pdw
// ���޺� ���մ��� å���� Ȯ���ϱ� ���� �Լ� �߰�
INT CUnionUserInfoViewer::GetUnionLoanLimitBookCnt(CString strLoanUserClassCode, CString &strGetLimitCnt)
{
EFS_BEGIN

	INT		ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_UnionLoanEnvInfo"));
	if( NULL == pDM )
	{
		AfxMessageBox(_T("CUnionUserInfoViewer::GetUnionLoanLimitBookCnt ERROR -1"), MB_ICONSTOP);
		return -1011;
	}

	INT nRowCount = pDM->GetRowCount();
	CString strData;

	for( INT i=0; i<nRowCount; i++ )
	{
		ids = pDM->GetCellData(_T("����������"), i, strData);
		if( 0 > ids )
		{
			AfxMessageBox(_T("CUnionUserInfoViewer::GetUnionLoanLimitBookCnt ERROR -2"), MB_ICONSTOP);
			return -1021;
		}
		
		strData.TrimLeft();
		strData.TrimRight();

		if( 0 == strData.Compare(strLoanUserClassCode) )
		{
			ids = pDM->GetCellData(_T("��������å��"), i, strData);
			if( 0 > ids )
			{
				AfxMessageBox(_T("CUnionUserInfoViewer::GetUnionLoanLimitBookCnt ERROR -3"), MB_ICONSTOP);
				return -1022;
			}

			strData.TrimLeft();
			strData.TrimRight();
/*
			/// ������ �����Ͱ� �������� Ȯ���Ѵ�
			INT nLen = strData.GetLength();
			for( INT j=0; j<nLen; j++ )
			{
				TCHAR ch = strData.GetAt(j);

				/// ���ڰ� ���ԵǾ� �ִ�
				if( '0' > ch || '9' < ch )
				{
					AfxMessageBox(_T("CUnionUserInfoViewer::GetUnionLoanLimitBookCnt ERROR -4"), MB_ICONSTOP);
					return -1031;
				}
			}
*/			strGetLimitCnt = strData;

			return 0;
		}
	}

	return 0;
	
EFS_END	
return -1;
}

void CUnionUserInfoViewer::OnSelchangetabUnionUserInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
EFS_BEGIN

	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_UNION_USER_INFO);

	if( 0 == pTab->GetCurSel() )
	{
		GetDlgItem(IDC_gUSER_INFO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gUSER_INFO2)->ShowWindow(SW_SHOW);			
	}
	else if( 1 == pTab->GetCurSel() )
	{
		GetDlgItem(IDC_gUSER_INFO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_gUSER_INFO2)->ShowWindow(SW_HIDE);			
	}		

	*pResult = 0;

EFS_END
return;
}

void CUnionUserInfoViewer::OnBtnMoveWinodw() 
{

}

UINT CUnionUserInfoViewer::OnNcHitTest(CPoint point) 
{
	return CDialog::OnNcHitTest(point);
}

void CUnionUserInfoViewer::OnLButtonDown(UINT nFlags, CPoint point) 
{	
// 	GetCursorPos(&m_CusorPoint);
// 	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(m_CusorPoint.x,m_CusorPoint.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CUnionUserInfoViewer::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonUp(nFlags, point);
}

void CUnionUserInfoViewer::OnMouseMove(UINT nFlags, CPoint point) 
{

	CDialog::OnMouseMove(nFlags, point);
}

/*

void CRCDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
EFS_BEGIN
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
	ScreenToClient(rcTitle);
	if (rcTitle.PtInRect(point))
	{
		m_bISMoveWindow = TRUE;
		SetCapture();
		GetCursorPos(&m_CusorPoint);
		GetWindowRect(m_rcWnd);
	}
	
	CDialog::OnLButtonDown(nFlags, point);
EFS_END
}

void CRCDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	if (m_bISMoveWindow)
	{
		m_bISMoveWindow = FALSE;
		ReleaseCapture();
	}
	
	CDialog::OnLButtonUp(nFlags, point);

	EFS_END
}

void CRCDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	if (m_bISMoveWindow)
	{
		CPoint point;
		GetCursorPos(&point);

		INT x = point.x - m_CusorPoint.x;
		INT y = point.y - m_CusorPoint.y;


		SetWindowPos(NULL, m_rcWnd.left + x, m_rcWnd.top + y, 1, 1, SWP_NOSIZE | SWP_NOZORDER);

		GetWindowRect(m_rcWnd);
		m_CusorPoint = point;

		return;
	}
	
	CDialog::OnMouseMove(nFlags, point);

	EFS_END
}

*/

void CUnionUserInfoViewer::OnBtnCancel() 
{
EFS_BEGIN

	// TODO: Add extra cleanup here
	
	this->ShowWindow( false );
	//CDialog::OnCancel();

EFS_END	
}

void CUnionUserInfoViewer::OnStaticMove() 
{
	GetCursorPos(&m_CusorPoint);
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(m_CusorPoint.x,m_CusorPoint.y));
}

HBRUSH CUnionUserInfoViewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
