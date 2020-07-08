// SE_LOC_1160.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1160.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1160 dialog


CSE_LOC_1160::CSE_LOC_1160(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1160)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pStatistics = NULL;
	
	// **************************************************************************
	// ����� ���� �÷��� �� ����
	// **************************************************************************
	CString sDefenceFlag = _T("");								
	INT ids = GetManageValue( _T("��Ÿ") , _T("����") , _T("�����") , _T("") , m_sDefenceFlag );
}

CSE_LOC_1160::~CSE_LOC_1160()
{
	if( m_pStatistics != NULL )
	{
		for( INT i = 0 ; i < m_saShelfLoc.GetSize() ; i++ )
			delete m_pStatistics[i];
		delete []m_pStatistics;
	}
	m_pStatistics = NULL;

}


VOID CSE_LOC_1160::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1160)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1160, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1160)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabSHELF_LOC, OnSelchangetabSHELFLOC)
	ON_BN_CLICKED(IDC_btnSPRINT, OnbtnSPRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1160 message handlers

BOOL CSE_LOC_1160::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	INT ids;
	ids = InitESL_Mgr(_T("SE_LOC_1160"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
		
	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );

	CRect WndPos;
	GetWindowRect( WndPos );
	
	MoveWindow( WndPos.left , WndPos.top , WndPos.Width() , WndPos.Height() );
	MoveWindow( WndPos.left , WndPos.top , WndPos.Width()+1 , WndPos.Height() );
	
	ids = MakeStatistics();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
	if( ids > 0 ) EndDialog(1);

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END

	return FALSE;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-22 ~ 2005-12-22
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   �Լ�����   ] 
	  ȭ�� ��Ʈ�ѵ��� ���ġ �Ѵ�.

   [   �̴Ͻ���   ]
      1. ��Ʈ�� �����͸� ���Ѵ�.
      2. �⺻ ��Ʈ�� ��ġ�� �����Ѵ�. 
	  3. �׸��� ��Ʈ�� ��ġ�� �����Ѵ�.
--------------------------------------------------------------------------------------------*/
VOID CSE_LOC_1160::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
		
	if( GetSafeHwnd() == NULL ) return;
	
/*	// ������ ����
	const INT nCnt = 5;
	CRect WndPos[nCnt];
	CWnd* Wnd[nCnt];
	
	INT nIDC[nCnt] = 
	{
		IDC_staTITLE2 , IDC_gridSTATISTICS ,  IDC_tabSHELF_LOC , IDC_btnClose , IDC_btnSPRINT
	};
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		Wnd[i] = GetDlgItem(nIDC[i]);
		if( Wnd[i]->GetSafeHwnd() == NULL ) return;
		Wnd[i]->GetWindowRect(WndPos[i]);
		ScreenToClient(WndPos[i]);
	}
	
	// Ÿ��Ʋ
	Wnd[0]->MoveWindow( 0 , WndPos[0].top , cx , WndPos[0].Height() );
	
	// �׸���
	Wnd[1]->MoveWindow( 0 , WndPos[1].top , cx , cy - WndPos[1].top  - 10  );
	

	// ����Ʈ��
	Wnd[2]->MoveWindow( 0 , WndPos[2].top , cx , WndPos[2].Height() );

	// �ݱ� ��ư
	Wnd[3]->MoveWindow( cx - WndPos[3].Width() - 5 , WndPos[3].top , WndPos[3].Width() , WndPos[3].Height() );
	
	// �ݱ� ��ư
	Wnd[4]->MoveWindow( cx - WndPos[4].Width() - WndPos[3].Width() - 10 , WndPos[4].top , WndPos[4].Width() , WndPos[4].Height() );
	
*/
	// ----------------------------------------------------------------------------
	// 1. ��Ʈ�� �����͸� ���Ѵ�.
	// ----------------------------------------------------------------------------
	const INT nCnt = 5;
	CRect WndPos[nCnt];
	CWnd* Wnd[nCnt];
	
	INT nIDC[nCnt] = 
	{
		IDC_staTITLE2 , IDC_gridSTATISTICS ,  IDC_tabSHELF_LOC , IDC_btnClose , IDC_btnSPRINT
	};
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		Wnd[i] = GetDlgItem(nIDC[i]);
		if( Wnd[i]->GetSafeHwnd() == NULL ) return;
		Wnd[i]->GetWindowRect(WndPos[i]);
		ScreenToClient(WndPos[i]);
	}
	
	// ----------------------------------------------------------------------------
	// 2. �⺻ ��Ʈ�� ��ġ�� �����Ѵ�. 
	// ----------------------------------------------------------------------------
	// Ÿ��Ʋ
	Wnd[0]->MoveWindow( 0 , WndPos[0].top , cx , WndPos[0].Height() );	
	// ����Ʈ��
	Wnd[2]->MoveWindow( 0 , WndPos[2].top , cx , WndPos[2].Height() );
	// ��� ��ư
	Wnd[3]->MoveWindow( cx - WndPos[3].Width() - 5 , WndPos[3].top , WndPos[3].Width() , WndPos[3].Height() );
	// �ݱ� ��ư
	Wnd[4]->MoveWindow( cx - WndPos[4].Width() - WndPos[3].Width() - 10 , WndPos[4].top , WndPos[4].Width() , WndPos[4].Height() );

	// 2005-12-07 ADD BY PDW(Jr) ==================================================
	// ----------------------------------------------------------------------------
	// 3. �׸��� ��Ʈ�� ��ġ�� �����Ѵ�.
	// ----------------------------------------------------------------------------
	INT nPos = cy - WndPos[1].top  - 10;
	Wnd[1]->MoveWindow( 0 , WndPos[1].top , cx , nPos );
	
	if ( m_sDefenceFlag == _T("Y") )
	{		
		// 1) �⺻ �׸��� �ٽ� ����
		Wnd[1]->MoveWindow( 0 , WndPos[1].top , cx , nPos/2  );

		// 2) ������ ��ġ ����
		CWnd* pWnd = GetDlgItem( IDC_gridSTATISTICS2 );
		if ( !pWnd ) return;

		pWnd->MoveWindow( 0 , WndPos[1].top + (nPos/2) + 5 , cx  , nPos/2 - 5  );
	}	
	// ============================================================================
EFS_END
}


/// ��� ���ϱ�
INT CSE_LOC_1160::MakeStatistics()
{
	EFS_BEGIN

	INT ids;

	// �ڷᰡ �����ϴ��� �˻�
	CESL_DataMgr* pParentDM = GetParentDM();
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeStatistics") );

	INT nSelectCnt = pParentDM->GetRowCount();
	if( nSelectCnt < 1 ) 
	{
		AfxMessageBox( _T("�ڷḦ �˻��� �ֽʽÿ�") );
		return 1;
	}

	// �׸��忡 ���õ� �ڷ� ��������
	CESL_DataMgr SrcDM;
	ids = MakeSrcDM(&SrcDM);
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeStatistics") );

	// �ڷ�� ��� �����ϱ�
	ids = MakeShelfLocList(&SrcDM);
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeStatistics") );
	
	ids = MakeStatisticsList(&SrcDM);
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeStatistics") );
	
	// ȭ�鿡 �����ֱ�
	ids = ShowScreen();
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeStatistics") );
	

	return 0;


	EFS_END

	return -1;
}

CESL_Grid* CSE_LOC_1160::GetParentGridPtr()
{
	EFS_BEGIN

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1100") , _T("MainGrid") );

	return pGrid;

	EFS_END

	return NULL;
}

CESL_DataMgr* CSE_LOC_1160::GetParentDM()
{
	EFS_BEGIN

	CESL_DataMgr* pDM = FindDM( _T("DM_SE_LOC_1100") );

	return pDM;

	EFS_END

	return NULL;
}


INT CSE_LOC_1160::MakeSrcDM(CESL_DataMgr *pDstDM)
{
	EFS_BEGIN

	
	CESL_DataMgr *pSrcDM = GetParentDM();
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	
	CESL_Grid* pGrid = GetParentGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
	pGrid->SelectMakeList();
	
	// 0 : ��ü , 1 : �׸��忡 ������ �ڷḸ
	INT nMode;
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		nSelectCnt = pSrcDM->GetRowCount();
		nMode = 0;
	}
	else
	{
		nMode =1;
	}
	
	// �׸��忡 ���õ� �ڷ�� PrintDM�� �����Ѵ�.
	CLocCommonAPI::MakeCopyDM( this , pSrcDM , pDstDM );
	
	if( nMode == 0 )
	{
		pDstDM->FreeData();
		CLocCommonAPI::AddDM( pSrcDM , pDstDM , this );
	}
	else
	{
		INT nIndex;
		for( INT i = 0 ; i < nSelectCnt ; i++ )
		{
			nIndex = pGrid->SelectGetIdx();
			CLocCommonAPI::AddDM( pSrcDM , pDstDM , nIndex , -1 , this );
			pGrid->SelectGetNext();
		}
	}

	return 0;

	EFS_END

	return -1;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-21 ~ 2005-12-21
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Sort
 	 
   [   �Լ�����   ] 
      �ڷ���� ���� ��� �����ϱ�
   [  PARAMETERS  ]
      pSrcDM : [I N] �׸��� ���ø�� DM  
             
   [ RETURN VALUE ]
      0    : ���� 
      ���� : ����

   [   �̴Ͻ���   ]
      1. DM�� �ڷ���ʵ带 ������ ���ٸ� ��Ͽ� �߰� ��Ų��.
--------------------------------------------------------------------------------------------*/
INT CSE_LOC_1160::MakeShelfLocList( CESL_DataMgr* pSrcDM )
{
EFS_BEGIN

	INT ids;

	// ----------------------------------------------------------------------------
	// 1. DM�� �ڷ���ʵ带 ������ ���ٸ� ��Ͽ� �߰� ��Ų��.     
	// ----------------------------------------------------------------------------
	m_saShelfLoc.RemoveAll();

	pSrcDM->SORT(_T("�ڷ��"));

	CString sData;
	CString sShelfLoc;
	CString sTmpShelfLoc;
	BOOL bIsInsert;
	
	for( INT i = 0;i < pSrcDM->GetRowCount();i++ )
	{
		ids = pSrcDM->GetCellData(_T("�ڷ��") , i , sShelfLoc);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeDivFromShelfLoc") );

		bIsInsert = TRUE;
		for( INT j = 0;j < m_saShelfLoc.GetSize();j++ )
		{
			if( sShelfLoc.Compare( m_saShelfLoc.GetAt(j) ) == 0 )
				bIsInsert = FALSE;
		}

		if( bIsInsert )
			m_saShelfLoc.Add(sShelfLoc);
	}

	return 0;

EFS_END
	return -1;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-21 ~ 2005-12-21
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   �Լ�����   ] 
      �ڷ�Ǻ� ��� �����ϱ� 

   [  PARAMETERS  ]
      pSrcDM : [I N] ������ �˻���� DM
             
   [ RETURN VALUE ]
      0    : ���� 
      ���� : ����

   [   �̴Ͻ���   ]
      1. �ڷ�Ǻ� DM�� �����Ѵ�. 
	  2. ȭ�� DM�� ��� DM���� �����Ѵ�.
--------------------------------------------------------------------------------------------*/
INT CSE_LOC_1160::MakeStatisticsList( CESL_DataMgr* pSrcDM )
{
EFS_BEGIN

	INT ids;

	// ----------------------------------------------------------------------------
	// 1. �ڷ�Ǻ� DM�� �����Ѵ�.      
	// ----------------------------------------------------------------------------
	INT nShelfCnt = m_saShelfLoc.GetSize();

	m_pStatistics = new CESL_DataMgr*[nShelfCnt];
	// 2005-12-21 UPD BY PDW(Jr) ==================================================
	// ������� ��� DM ����
	CString sDMAlias = _T("DM_SE_LOC_1160");
	if ( m_sDefenceFlag == _T("Y") ) sDMAlias = _T("DM_SE_LOC_1160_ND");
	// ============================================================================

	CESL_DataMgr *pShapeDM = FindDM( sDMAlias );
	if( !pShapeDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1160::MakeStatisticsList") );

	for( INT i = 0;i < nShelfCnt;i++ )
	{
		m_pStatistics[i] = new CESL_DataMgr;
		CLocCommonAPI::MakeCopyDM( this , pShapeDM , m_pStatistics[i] );
		ids = MakeStatisticsShape(m_pStatistics[i]);
		if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSE_LOC_1160::MakeStatisticsList") );
	}

	// ----------------------------------------------------------------------------
	// 2. ȭ�� DM�� ��� DM���� �����Ѵ�.     
	// ----------------------------------------------------------------------------
	CString sCnt;
	CString sShelfLoc;
	CString sClassNo;
	CString sClassNoType;
	INT nDMIndex;
	INT nRowIndex;
	CString sColAlias;

	for( i = 0 ; i < pSrcDM->GetRowCount() ; i++ )
	{
		ids = pSrcDM->GetCellData( _T("�ڷ��") , i , sShelfLoc );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSE_LOC_1160::MakeStatisticsList") );
		GetDMIndex( sShelfLoc , nDMIndex );

		ids = pSrcDM->GetCellData( _T("�з���ȣ����") , i , sClassNoType );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CSE_LOC_1160::MakeStatisticsList") );
		GetRowIndex( sClassNoType , nRowIndex );

		ids = pSrcDM->GetCellData( _T("�з���ȣ") , i , sClassNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CSE_LOC_1160::MakeStatisticsList") );
		GetColAlias( sClassNo , sColAlias , nRowIndex);

		m_pStatistics[nDMIndex]->GetCellData(sColAlias,nRowIndex , sCnt);

		sCnt.Format( _T("%d") , _ttoi(sCnt) + 1 );
		
		m_pStatistics[nDMIndex]->SetCellData(sColAlias,sCnt,nRowIndex);

		// 2005-12-27 ADD BY PDW(Jr) ========================================================
		// �հ踦 �����Ѵ�.
		m_pStatistics[nDMIndex]->GetCellData( _T("��"), nRowIndex, sCnt );
		sCnt.Format( _T("%d") , _ttoi(sCnt) + 1 );		
		m_pStatistics[nDMIndex]->SetCellData( _T("��"), sCnt, nRowIndex );
		// ==================================================================================
	}

	return 0;

EFS_END
	return -1;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-21 ~ 2005-12-21
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   �Լ�����   ] 
      �з���ȣ ���� ������ �Ѵ�.

   [  PARAMETERS  ]
      pDM : [I N] ȭ�鿡 ��Ÿ�� DM  
             
   [ RETURN VALUE ]
      0    : ���� 
      ���� : ����

   [   �̴Ͻ���   ]
      1. DM�� KDC, DDC, ��Ÿ �� �з���ȣ ������ �����Ѵ�.
	  2. ������ϰ�� LC�� �߰��ȴ�.
--------------------------------------------------------------------------------------------*/
INT CSE_LOC_1160::MakeStatisticsShape( CESL_DataMgr* pDM )
{
EFS_BEGIN

	const INT nCnt = 3;
	pDM->FreeData();
	INT nColCnt = pDM->GetColCount();
	
	for( INT i = 0;i < 3;i++ )
	{
		pDM->InsertRow(-1);

		for( INT j = 0;j < nColCnt;j++ )
		{
			pDM->SetCellData( i , j , _T("0") );
		}
	}
	
	CString sCodeAlias[nCnt];	

	sCodeAlias[0] = _T("KDC");
	sCodeAlias[1] = _T("DDC");
	sCodeAlias[2] = _T("��Ÿ");

	for (i = 0;i < nCnt;i++ )
		pDM->SetCellData( _T("�з���ȣ����") , sCodeAlias[i], i  );

	// 2005-12-21 ADD BY PDW(Jr) ==================================================
	// ----------------------------------------------------------------------------
	// 2. ������ϰ�� LC�� �߰��ȴ�. 
	// ----------------------------------------------------------------------------
	if ( m_sDefenceFlag == _T("Y") )
	{
		pDM->InsertRow(-1);
		for( INT j = 0;j < nColCnt;j++ )
		{
			pDM->SetCellData( i , j , _T("0"));
		}

		pDM->SetCellData( _T("�з���ȣ����") , _T("LC"), i  );
	}
	// ============================================================================
	return 0;

EFS_END	
	return -1;	
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-21 ~ 2005-12-21
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Sort
 	 
   [   �Լ�����   ] 
      �ڷ�Ǹ��� ���Ͽ� DM �ε����� ���Ѵ�.

   [  PARAMETERS  ]
      sShelfLoc : [I N] �ڷ�Ǹ�
	  nDMIndex  : [OUT] DM �ε��� 	
             
   [ RETURN VALUE ]
      0    : ���� 
      ���� : ���� 
--------------------------------------------------------------------------------------------*/
INT CSE_LOC_1160::GetDMIndex( CString sShelfLoc , INT &nDMIndex )
{
EFS_BEGIN

	for( INT i = 0;i < m_saShelfLoc.GetSize();i++ )
	{
		if( sShelfLoc.Compare(m_saShelfLoc.GetAt(i)) == 0 )
		{
			nDMIndex = i;
			return 0;
		}
	}

	return -1;

EFS_END
	return -1;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-21 ~ 2005-12-21
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   �Լ�����   ] 
      �з���ȣ���� �ڵ带 ���� ROW �ε����� ���Ѵ�.

   [  PARAMETERS  ]
      sClassNoType : [I N] �з���ȣ���� �ڵ� 
	  nRowIndex    : [OUT] ROW �ε��� 	
             
   [ RETURN VALUE ]
      0    : ���� 
      ���� : ���� 
--------------------------------------------------------------------------------------------*/
INT CSE_LOC_1160::GetRowIndex( CString sClassNoType , INT &nRowIndex )
{
EFS_BEGIN

	sClassNoType.TrimLeft(); sClassNoType.TrimRight();

	if( sClassNoType.Compare(_T("1")) == 0 )
		nRowIndex = 0;
	else if( sClassNoType.Compare(_T("2")) == 0 )
		nRowIndex = 1;
	// 2005-12-21 ADD BY PDW(Jr) ==================================================
	// LC �߰� 
	else if( sClassNoType.Compare(_T("3")) == 0 || sClassNoType.Compare(_T("7")) == 0 )
	{
		if ( m_sDefenceFlag == _T("Y") )
			nRowIndex = 3;
		else
			nRowIndex = 2;
	}
	// ============================================================================	
	else
		nRowIndex = 2;
	
	return 0;

EFS_END	
	return -1;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-21 ~ 2005-12-21
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   �Լ�����   ] 
      �з���ȣ�� ���� COL �ε����� ���Ѵ�.

   [  PARAMETERS  ]
      sClassNoType : [I N] �з���ȣ
	  sColAlias    : [OUT] COL �ε��� 	
             
   [ RETURN VALUE ]
      0    : ���� 
      ���� : ���� 
   [   �̴Ͻ���   ]
      1. �з���ȣ�� ���ٸ� ��Ÿ�� ����Ѵ�.
	  2. �з���ȣ ù���ڰ� �����ϰ��
	  3. �з���ȣ ù���ڰ� �����ϰ��
--------------------------------------------------------------------------------------------*/
INT CSE_LOC_1160::GetColAlias( CString sClassNo , CString &sColAlias , INT nRowIndex /* = 0 */)
{
EFS_BEGIN

	sClassNo.TrimLeft(); sClassNo.TrimRight();
		
	// ----------------------------------------------------------------------------
	// 1. �з���ȣ�� ���ٸ� ��Ÿ�� ����Ѵ�.     
	// ----------------------------------------------------------------------------	
	if( sClassNo.IsEmpty() ) 
	{
		sColAlias = _T("��Ÿ");
		return 0;
	}

	// 2005-12-08 UPD BY PDW(Jr) ==================================================
	// �������� �۾����� ���� �񱳹��� �ٲپ���.
	/* ������ 
	// ----------------------------------------------------------------------------
	// 2. �з���ȣ ù���ڰ� _T("0") �ϰ�� 
	// ----------------------------------------------------------------------------	
	sClassNo = sClassNo.Mid(0,1);

	if( sClassNo.Compare(_T("0")) == 0 )
	{
		sColAlias = _T("0");
		return 0;
	}

	// ----------------------------------------------------------------------------
	// 3. �з���ȣ ù���ڰ� �����ϰ��
	// ----------------------------------------------------------------------------		
	if( _ttoi( sClassNo ) == 0 )
	{
		sColAlias = _T("��Ÿ");
		return 0;			
	}

	sColAlias = sClassNo;	
	*/
	TCHAR chClassCode = sClassNo.GetAt(0);
	
	// ----------------------------------------------------------------------------
	// 2. �з���ȣ ù���ڰ� �����ϰ��     
	// ----------------------------------------------------------------------------	
	if ( chClassCode > 47 && chClassCode < 58 )
	{
		if ( m_sDefenceFlag == _T("Y") )
		{
			// LC��� ���ڴ� ��Ÿ�� ����.
			if ( nRowIndex == 3 )
			{
				sColAlias = _T("��Ÿ");
				return 0;
			}
			// ����� DM�� �°� �����Ѵ�.
			INT nFlag = _ttoi( &chClassCode );
			// ����뿡���� �з���ȣ�� 'I'�� ����.
			if ( chClassCode < 56 )
				sColAlias.Format( _T("%c"), 'A' + nFlag );
			else
				sColAlias.Format( _T("%c"), 'A' + nFlag + 1 );			
		}
		else
		{
			sColAlias = sClassNo.Mid( 0 , 1 );
		}
	}
	// ----------------------------------------------------------------------------
	// 3. �з���ȣ ù���ڰ� �����ϰ��     
	// ----------------------------------------------------------------------------	
	else
	{
		sColAlias.Empty();

		if ( m_sDefenceFlag == _T("Y") )
		{
			CString sLCCode = _T("ABCDEFGHJKLMNPQRSTUVZ");
			TCHAR* pchResult = _tcsstr( sLCCode, sClassNo.Mid( 0 , 1 ) );
			if ( pchResult != NULL )
				sColAlias = sClassNo.Mid( 0 , 1 );
		}
		
		if ( sColAlias.IsEmpty() )
			sColAlias = _T("��Ÿ");
		
	}

	// ============================================================================

	return 0;

EFS_END
	return -1;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-21 ~ 2005-12-21
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Sort
 	 
   [   �Լ�����   ] 
      ���õ� �ڷ�ǿ� �´� ȭ���� �����ش�.
             
   [ RETURN VALUE ]
      0    : ���� 
      ���� : ���� 

   [   �̴Ͻ���   ]
      1. �ڷ�Ǻ� ���� �����.
	  2. ���õ� �ڷ���� ����� �����ش�.
--------------------------------------------------------------------------------------------*/
INT CSE_LOC_1160::ShowScreen()
{
EFS_BEGIN

	// ----------------------------------------------------------------------------
	// 1. �ڷ�Ǻ� ���� �����.  
	// ----------------------------------------------------------------------------	
	MakeTabCtrl();

	// ----------------------------------------------------------------------------
	// 2. ���õ� �ڷ���� ����� �����ش�.   
	// ----------------------------------------------------------------------------	
	ShowGridFromDM();

	return 0;

EFS_END	
	return -1;
}

INT CSE_LOC_1160::MakeTabCtrl()
{
	EFS_BEGIN
		
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabSHELF_LOC);
	pTab->DeleteAllItems();

	for( INT i = 0 ; i < m_saShelfLoc.GetSize() ; i++ )
	{
		pTab->InsertItem( i , m_saShelfLoc.GetAt(i) );
	}
	
	return 0;
	
	EFS_END

	return -1;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-21 ~ 2005-12-21
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   �Լ�����   ] 
      ���õ� �ڷ���� ��� ����� �׸���� �����ش�.

   [ RETURN VALUE ]
      0    : ���� 
      ���� : ���� 

   [   �̴Ͻ���   ]
      1. �ڷ�� �ε����� DM�� �ε����� �����ϴ�.
	  2. �׸��� �÷��� �з���ȣ�� �����ֱ�
	  3. DM�� ������ �׸��忡 �Է��Ѵ�.
--------------------------------------------------------------------------------------------*/
INT CSE_LOC_1160::ShowGridFromDM()
{
EFS_BEGIN

	INT ids;
	
	// ----------------------------------------------------------------------------
	// 1. �ڷ�� �ε����� DM�� �ε����� �����ϴ�.
	// ----------------------------------------------------------------------------	
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabSHELF_LOC);
	if ( !pTab ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1160::ShowGridFromDM") );

	INT nDMIndex = pTab->GetCurSel();

	// ----------------------------------------------------------------------------
	// 2. �׸��� �÷��� �з���ȣ�� �����ֱ�
	// ----------------------------------------------------------------------------	
	const INT nCnt = 13;
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridSTATISTICS);
	pGrid->SetRows(4);
	pGrid->SetCols(0,nCnt);
	
	// 2005-12-22 UPD BY PDW(Jr) =================================================
	// �������� ���� 
	CString colDesc[ nCnt ] = {
		_T("�з���ȣ����"), _T("�ѷ�"), _T("ö��"), _T("����"), _T("��ȸ����"), _T("��������"), _T("�������"), _T("����"), _T("����"), _T("����"), _T("����"), _T("��Ÿ"), _T("��") };
	// ===========================================================================
	for( INT k = 0;k < nCnt;k++ )
		pGrid->SetTextMatrix( 0 , k , colDesc[k] );
	
	// 2005-12-21 ADD BY PDW(Jr) ==================================================
	// ����� LC�� �׸��尡 �߰��Ǿ���.
	const INT nLCCnt = 24;
	CMSHFlexGrid* pLCGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridSTATISTICS2);
	if ( !pLCGrid ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSE_LOC_1160::ShowGridFromDM") );
	pLCGrid->SetRows( 2 );
	pLCGrid->SetCols( 0, nLCCnt);
	
	CString colLCDesc[ nLCCnt ] = {
		_T("�з���ȣ����"), _T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F"), _T("G"), _T("H"), _T("J"), _T("K"), _T("L"), _T("M"), _T("N"), _T("P"), 
			_T("Q"), _T("R"), _T("S"), _T("T"), _T("U"), _T("V"), _T("Z"), _T("��Ÿ"), _T("��") };

	for ( k = 0;k < nLCCnt;k++ )
		pLCGrid->SetTextMatrix( 0 , k , colLCDesc[k] );
	// ============================================================================

	CString sColumnName[ nCnt ] =
	{
		_T("�з���ȣ����") , _T("0")    , _T("1") , _T("2") , _T("3") , 
		_T("4")            , _T("5")    , _T("6") , _T("7") , _T("8") , 
		_T("9")            , _T("��Ÿ") , _T("��")
	};

	// ----------------------------------------------------------------------------
	// 3. DM�� ������ �׸��忡 �Է��Ѵ�.
	//    �հ赵 �Էµȴ�.
	// ----------------------------------------------------------------------------	
	// 3.1) �Ϲ� �׸��� �Է� 
	CString sResult;
	INT nSum;
	for( INT i = 0 ; i < 3; i++ )
	{
		nSum = 0;
		for( INT j = 0 ; j < nCnt ; j++ )
		{
			// 2005-12-21 UPD BY PDW(Jr) ==================================================
			// ������ DM�÷����� Ʋ����.
			/* ������
			ids = m_pStatistics[nDMIndex]->GetCellData( sColumnName[j] , i , sResult );
			*/
			CString sCurColName = sColumnName[j];
			if ( m_sDefenceFlag == _T("Y") ) sCurColName = colLCDesc[j];
			ids = m_pStatistics[nDMIndex]->GetCellData( sCurColName , i , sResult );	
			// ============================================================================
			if( ids < 0 )
			{
				CString sMsg;
				sMsg.Format( _T("m_pStatistics[%d]->GetCellData( sColumnName[%d] , %d , sResult )") , nDMIndex , j , i );
				AfxMessageBox( sMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSE_LOC_1160::ShowGridFromDM") );

			if( (j != 0) && (j != (nCnt-1)) )
			{
				nSum += _ttoi(sResult);
			}

			// 2005-12-22 ADD BY PDW(Jr) ==================================================
			// ������ DM�� Ʋ����.
			if ( j == ( nCnt - 2 ) )
			{
				ids = m_pStatistics[nDMIndex]->GetCellData( _T("��Ÿ") , i , sResult );	
				m_pStatistics[nDMIndex]->SetCellData( _T("��Ÿ") , sResult , i );
			}
		
			// ============================================================================

			if( j == (nCnt-1) )
			{
				sResult.Format( _T("%d") , nSum );
				m_pStatistics[nDMIndex]->SetCellData( _T("��") , sResult , i );
			}
			
			pGrid->SetTextMatrix( i+1 , j , sResult );	
		}
	}
	
	// 2005-12-21 ADD BY PDW(Jr) ==================================================
	// 3.2) ����� LC �Է�
	if ( m_sDefenceFlag == _T("Y") )
	{
		nSum = 0;
		for( INT j = 0;j < nLCCnt;j++ )
		{
			ids = m_pStatistics[nDMIndex]->GetCellData( colLCDesc[j] , i , sResult );				
			if( ids < 0 )
			{
				CString sMsg;
				sMsg.Format( _T("m_pStatistics[%d]->GetCellData( sColumnName[%d] , %d , sResult )") , nDMIndex , j , i );
				AfxMessageBox( sMsg );
				ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CEO_LOC_1160::ShowGridFromDM") );
			}			

			if( (j != 0) && (j != ( nLCCnt - 1 )) )
				nSum += _ttoi( sResult );			

			if( j == (nLCCnt-1) )
			{
				sResult.Format( _T("%d") , nSum );
				m_pStatistics[nDMIndex]->SetCellData( _T("��") , sResult , i );
			}
			
			pLCGrid->SetTextMatrix( 1 , j , sResult );	
		}
	}
	// ============================================================================

	return 0;

EFS_END
	return -1;
}

VOID CSE_LOC_1160::OnbtnClose() 
{
	OnCancel();	
}

VOID CSE_LOC_1160::OnSelchangetabSHELFLOC(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowGridFromDM();
	
	*pResult = 0;
}

VOID CSE_LOC_1160::OnbtnSPRINT() 
{
	Print();	
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-21 ~ 2005-12-21
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   �Լ�����   ] 
      ��� ��� ���
              
   [ RETURN VALUE ]
      0    : ���� 
      ���� : ���� 

   [   �̴Ͻ���   ]
      1. SIReport ���� 
	  2. ���
--------------------------------------------------------------------------------------------*/
INT CSE_LOC_1160::Print()
{
EFS_BEGIN

	INT ids;

	// ----------------------------------------------------------------------------
	// 1. SIReport ���� 
	// ----------------------------------------------------------------------------
	CString sDMAlias = _T("DM_SE_LOC_1160");
	if ( m_sDefenceFlag == _T("Y") ) sDMAlias = _T("DM_SE_LOC_1160_ND");	
	
	CESL_DataMgr* pPrintDM = FindDM( sDMAlias );
	if( !pPrintDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1160::Print()") );
		
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return NULL;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	
	pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;
	
	CString sSIReportAlias = _T("����_�ڷ�Ǻ�_�ΰ��ڷ����");
	if ( m_sDefenceFlag == _T("Y") ) sSIReportAlias = _T("����_�ڷ�Ǻ�_�ΰ��ڷ����_�����");
	ids = pSIReportManager->GetSIReportFileFromDB( sSIReportAlias );
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return NULL;
	}
	
	ids = pSIReportManager->DeleteTempPageFiles();	
	pSIReportManager->SetDataMgr(0, pPrintDM, 0);

	// ----------------------------------------------------------------------------
	// 2. ���
	// ----------------------------------------------------------------------------
	CString sPassDate;
	ids = GetControlData( _T("CM_SE_LOC_1160") , _T("�ΰ�����") , sPassDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSE_LOC_1160::Print()") );

	INT nSum;
	CString sBookCnt;
	// ============================================================================
	
/*	for( INT i = 0 ; i < m_saShelfLoc.GetSize() ; i++ )
	{	
		// ����Ѵ�.
		// ��� ��Ÿ ���� ����
		sBookCnt = _T("");
		nSum = 0;
		pPrintDM->FreeData();

		CLocCommonAPI::AddDM( m_pStatistics[i] ,pPrintDM, this );

		for( INT m = 0 ; m < 3 ; m++ )
		{
			pPrintDM->GetCellData( _T("��") , m , sBookCnt );
			nSum += _ttoi(sBookCnt);
		}
		sBookCnt.Format( _T("%d") , nSum );

		ids = pSIReportManager->SetTempVariable( _T("�ڷ��") , m_saShelfLoc.GetAt(i) );
		ids = pSIReportManager->SetTempVariable( _T("�ΰ�����") , sPassDate );
		ids = pSIReportManager->SetTempVariable( _T("å��") , sBookCnt );
		
		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// ��ũ�� Manual �ݺ� ���
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintUserList") );
	}
*/	
	CString sPrintPosition;
	for ( INT i = 0;i < m_saShelfLoc.GetSize();i++ )
	{
		sBookCnt = _T("");
		nSum = 0;

		for ( INT j = 0;j < m_pStatistics[i]->GetColCount();j++ )
		{
			for ( INT r = 0;r < m_pStatistics[i]->GetRowCount();r++ )
			{
				sPrintPosition.Empty();
				sPrintPosition.Format( _T("%d_%d"), r, j );
				m_pStatistics[i]->GetCellData( r, j , sBookCnt );
				ids = pSIReportManager->SetTempVariable( sPrintPosition , sBookCnt );			
			}
		}

		for( INT m = 0;m < m_pStatistics[i]->GetRowCount();m++ )
		{
			m_pStatistics[i]->GetCellData( _T("��") , m , sBookCnt );
			nSum += _ttoi(sBookCnt);
		}

		// 2006-01-02 ADD BY PDW(Jr) ===================================================
		// �հ�
		sBookCnt.Format( _T("%d"), nSum );
		// =============================================================================
		
		ids = pSIReportManager->SetTempVariable( _T("�ڷ��") , m_saShelfLoc.GetAt(i) );
		ids = pSIReportManager->SetTempVariable( _T("�ΰ�����") , sPassDate );
		ids = pSIReportManager->SetTempVariable( _T("å��") , sBookCnt );
		
		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// ��ũ�� Manual �ݺ� ���
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSE_LOC_1160::Print()") );		
	}
	// ============================================================================
	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	
	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1160::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-23 ~ 2005-12-23
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
    	 
   [   �Լ�����   ] 
      [����] �谡���� - �ΰ� ��� ���� ��½� ���Ǵ� ��� �Լ�

   [  PARAMETERS  ]
      pSIReportMgr : SIReport ������
	  index        : �� �ε���

   [ RETURN VALUE ]
      0    : ���� 
      ���� : ����

   [   �̴Ͻ���   ]
      1. SIReport�� �ΰ� ���ڸ� �Է��Ѵ�.
	  2. SIReport�� �ڷ���� �Է��Ѵ�.
	  3. SIReport�� �з��� ���� �Է��Ѵ�.
	  4. SIReport�� å���� �Է��Ѵ�.
--------------------------------------------------------------------------------------------*/
INT CSE_LOC_1160::MakePrintPage(CSIReportManager *pSIReportMgr, INT index)
{
	INT ids;

	// ----------------------------------------------------------------------------
	// 1. SIReport�� �ΰ� ���ڸ� �Է��Ѵ�.     
	// ----------------------------------------------------------------------------
	CString sPassDate;
	ids = GetControlData( _T("CM_SE_LOC_1160") , _T("�ΰ�����") , sPassDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1160::MakePrintPage()") );

	pSIReportMgr->SetTempVariable( _T("�ΰ�����") , sPassDate );
	
	// ----------------------------------------------------------------------------
	// 2. SIReport�� �ڷ���� �Է��Ѵ�.    
	// ----------------------------------------------------------------------------
	pSIReportMgr->SetTempVariable( _T("�ڷ��") , m_saShelfLoc.GetAt( index ) );	

	// ----------------------------------------------------------------------------
	// 3. SIReport�� �з��� ���� �Է��Ѵ�.
	// ----------------------------------------------------------------------------
	const INT nColCnt = 13;
	CString alias[ nColCnt ] = {
		_T("�з���ȣ����"), _T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("��Ÿ"), _T("��") };
	
	CString data;
		
	// �����DM�� ������ Ʋ����.
	const INT nLCColCnt = 24;
	CString colLCDesc[ nLCColCnt ] = {
		_T("�з���ȣ����"), _T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F"), _T("G"), _T("H"), _T("J"), _T("K"), _T("L"), _T("M"), _T("N"), _T("P"), 
			_T("Q"), _T("R"), _T("S"), _T("T"), _T("U"), _T("V"), _T("Z"), _T("��Ÿ"), _T("��") };
	
	CString sColAlias;

	for( INT i = 0;i < m_pStatistics[ index ]->GetRowCount();i++ )
	{
		INT nCnt = nColCnt;
		if ( m_sDefenceFlag == _T("Y") )	nCnt = nLCColCnt;

		for( INT j = 0;j < nCnt;j++ )
		{
			if ( m_sDefenceFlag == _T("Y") )	sColAlias = colLCDesc[j];
			else							sColAlias = alias[j];

			ids = m_pStatistics[ index ]->GetCellData( sColAlias, i, data );

			sColAlias.Format( _T("%d_%s"), i, sColAlias );
			pSIReportMgr->SetTempVariable( sColAlias, data );
		}
	}

	// ----------------------------------------------------------------------------
	// 4. SIReport�� å���� �Է��Ѵ�. 
	// ----------------------------------------------------------------------------
	INT nSum = 0;
	CString sBookCnt;
	for( INT m = 0;m < 3;m++ )
	{
		m_pStatistics[ index ]->GetCellData( _T("��") , m , sBookCnt );
		nSum += _ttoi( sBookCnt );
	}
	
/*	// 2005-12-07 UPD BY PDW(Jr) ==================================================
	// ����� ��/å ���� �߰� 
	if ( m_sDefenceFlag == _T("Y") )
	{
		CString sSpeciecsCnt;
		GetDlgItemText( IDC_edtSpeciesCnt, sSpeciecsCnt );
		sBookCnt.Format( _T("%s/%d") , sSpeciecsCnt , nSum );
	}
	else
	{
		sBookCnt.Format( _T("%d") , nSum );	
	}
	// ============================================================================
*/
	ids = pSIReportMgr->SetTempVariable( _T("å��") , sBookCnt );
		
	return 0;
}

HBRUSH CSE_LOC_1160::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
