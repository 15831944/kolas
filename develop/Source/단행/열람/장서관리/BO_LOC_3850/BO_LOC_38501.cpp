// BO_LOC_38501.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_38501.h"
#include "BO_LOC_3852.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3850 dialog


CBO_LOC_3850::CBO_LOC_3850(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3850)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3850::~CBO_LOC_3850()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CBO_LOC_3850::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3850)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3850, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3850)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3850 message handlers

BOOL CBO_LOC_3850::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("BO_LOC_3850") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	SearchAllInfo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}




VOID CBO_LOC_3850::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN


	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return;

	CWnd* pWnd = GetDlgItem(IDC_grid3850_Main);

	if( pWnd == NULL ) return ;

	if( pWnd->GetSafeHwnd() == NULL ) return;

	pWnd->MoveWindow( 0 , 0 , cx , cy );
	


EFS_END
}


BOOL CBO_LOC_3850::Create(CWnd* pParentWnd) 
{
EFS_BEGIN


	return CDialog::Create(IDD, pParentWnd);


EFS_END
return FALSE;

}



INT CBO_LOC_3850::SearchAllInfo()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchAllInfo") );

	ids = pDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchAllInfo") ); 

	ids = AllControlDisplay( _T("CM_BO_LOC_3850") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchAllInfo") ); 


	return 0;

EFS_END
return -1;

}

BEGIN_EVENTSINK_MAP(CBO_LOC_3850, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3850)
	ON_EVENT(CBO_LOC_3850, IDC_grid3850_Main, -601 /* DblClick */, OnDblClickgrid3850Main, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_3850::OnDblClickgrid3850Main() 
{
EFS_BEGIN

	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3850") , _T("���α׸���") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnDblClickgrid3850Main") );

	if( pGrid->GetMouseRow() < 1 ) return;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("SearchAllInfo") );

	if( pDM->GetRowCount() < 1 ) return ;

	pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );

	CBO_LOC_3852 Dlg(this);
	Dlg.SetDlgType( __ECO_MODIFY_DBCLICK_MODE , _T("CM_BO_LOC_3850") , _T("���α׸���") , _T("DM_BO_LOC_3850") );
	Dlg.DoModal();
	

EFS_END
}

INT CBO_LOC_3850::DeleteGroup()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DeleteGroup") );

	if( pDM->GetRowCount() < 1 ) return 1;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3850") , _T("���α׸���") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DeleteGroup") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt < 1 )
	{
		AfxMessageBox( _T("������ ����� �������ֽʽÿ�.") );
		return 2;
	}


	// �����Ȱ� �����ϱ�
	CArray <INT,INT> aDIndexs;
	CString sCode;
	CString sRecKey;

	pGrid->SelectGetTailPosition();

	pDM->StartFrame();

	INT nIndex;
	INT nDCnt=0;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		
		ids = pDM->GetCellData( _T("�ڵ�") , nIndex , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DeleteGroup") );
		sCode.TrimLeft(); sCode.TrimRight();

		if( sCode.IsEmpty() )
		{
			AfxMessageBox( _T("������� �׷��� �����Ҽ� �����ϴ�.") );
			pGrid->SelectGetPrev();

			continue;
		}

		// 2005.10.13 ADD BY PDW(Jr)/////////////////////////////////////////////
		// �ش� �ڵ��� ȸ�� ����Ʈ�� �����Ѵٸ� ȸ�� ����� ���� �Ѵ�.
		/////////////////////////////////////////////////////////////////////////
		CString query = _T(""), result=_T(""), cnt=_T("");

		/////////////////////////////////////////////////////////////////////////
		// 1. ��������� ���ο� ���� ���� ���̺��� �޶�����.
		/////////////////////////////////////////////////////////////////////////														
		query.Format(_T("select loan_user_yn from mn_loan_user_group_tbl where code = '%s' "), sCode);
		pDM->GetOneValueQuery(query, result);
		result.TrimLeft();result.TrimRight();

		if (result == _T("Y"))
			query.Format(_T("select count(*) from mn_loan_user_group_list_tbl where group_code ='%s'"), sCode);
		else
			query.Format(_T("select count(*) from co_nonmember_tbl where group_code ='%s'"), sCode);

		pDM->GetOneValueQuery(query, cnt);
		if ( _ttoi(cnt) > 0 )						// ����Ʈ�� �����Ѵ�.
		{
			CString mng;
			mng.Format(_T("���õ� �׷��� ȸ������� ������ �ֽ��ϴ�.\r\n")
				       _T("�׷��� ������ �׷��� ȸ����ϵ� ������ �˴ϴ�.\r\n")
					   _T("��� �����Ͻðڽ��ϱ�?"));
			if ( IDYES == AfxMessageBox(mng, MB_YESNO) )	// ���� ����
			{
				/////////////////////////////////////////////////////////////////////////
				// 2. ��������� ���ο� ���� ���� ���̺��� �޶�����.
				/////////////////////////////////////////////////////////////////////////
				if (result == _T("Y"))
					ids = pDM->MakeDeleteFrame( _T("MN_LOAN_USER_GROUP_LIST_TBL") , _T("GROUP_CODE") ,
					                         _T("STRING") , sCode );
				else
					ids = pDM->MakeDeleteFrame( _T("CO_NONMEMBER_TBL") , _T("GROUP_CODE") ,
					                         _T("STRING") , sCode );

				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("DeleteGroup") );

				pDM->SendFrame();
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("DeleteGroup") );
			}
			else 
				return 0;
		}

		ids = pDM->GetCellData( _T("�׷������ڵ�Ű") , nIndex , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DeleteGroup") );


		ids = pDM->MakeDeleteFrame( _T("MN_LOAN_USER_GROUP_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("DeleteGroup") );
		nDCnt++;

		aDIndexs.Add(nIndex);

		pGrid->SelectGetPrev();

	}


	if( nDCnt > 0 )
	{
		ids = pDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("DeleteGroup") );
	}

	pDM->EndFrame();

	if( nDCnt == 0 )
	{
		AfxMessageBox( _T("������ �׷��� �������� �ʽ��ϴ�. ") );
		return 1;
	}

	/// DM ����
	for( i = 0 ; i < aDIndexs.GetSize() ; i++ )
	{
		ids = pDM->DeleteRow( aDIndexs.GetAt(i) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("DeleteGroup") );
	}

	/// �׸��� ����
	ids = AllControlDisplay( _T("CM_BO_LOC_3850") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("DeleteGroup") ); 

	AfxMessageBox( _T("�����Ǿ����ϴ�.") );

	return 0;

EFS_END
return -1;

}

HBRUSH CBO_LOC_3850::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
