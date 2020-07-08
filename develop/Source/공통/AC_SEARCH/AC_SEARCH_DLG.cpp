// AC_SEARCH_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "AC_SEARCH_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAC_SEARCH_DLG dialog


CAC_SEARCH_DLG::CAC_SEARCH_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CAC_SEARCH_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strSearchItem = _T("");
	m_pCM = NULL;
	m_pDM = NULL;
	m_pRefDM = NULL;
	m_strSearchText = _T("");

}

CAC_SEARCH_DLG::~CAC_SEARCH_DLG()
{
}

VOID CAC_SEARCH_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAC_SEARCH_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAC_SEARCH_DLG, CDialog)
	//{{AFX_MSG_MAP(CAC_SEARCH_DLG)
	ON_BN_CLICKED(IDC_bClose, OnbClose)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAC_SEARCH_DLG message handlers


VOID CAC_SEARCH_DLG::OnbClose() 
{
	CDialog::OnCancel();	
}


BOOL CAC_SEARCH_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_AC_SEARCH")) < 0)
		return FALSE;

	m_pCM = FindCM( _T("CM_AC_SEARCH") );
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox( _T("CM �ʱ�ȭ ����") );
		return FALSE;
	}

	m_pDM = FindDM ( _T("DM_AC_SEARCH") );
	if ( m_pDM == NULL ) 
	{
		AfxMessageBox( _T("DM �ʱ�ȭ ����") );
		return FALSE;
	}

	m_pRefDM = FindDM ( _T("DM_AC_REFITEM") );
	if ( m_pRefDM == NULL ) 
	{
		AfxMessageBox( _T("DM �ʱ�ȭ ����") );
		return FALSE;
	}


	if ( !InitSearchAC() ) 
	{
		CDialog::OnCancel();
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAC_SEARCH_DLG::InitSearchAC()
{
	if ( m_strSearchItem == _T("") ) 
	{
		AfxMessageBox( _T("�˻�� �Էµ��� �ʾҽ��ϴ�.") );
		return FALSE;
	}

	if ( m_strSearchGroup == _T("") ) 
	{
		AfxMessageBox( _T("����˻����� ���� �˻����� ���õ��� �ʾҽ��ϴ�.") );
		return FALSE;
	}

	INT ids = -1;
	CString strTBLNAME;
	strTBLNAME.Empty();
	if ( m_strSearchGroup == _T("����") )
	{	
		m_nAcSearchOpt = 0;
		strTBLNAME = _T("AC_INDEX_TITLE_TBL");
	}
	else if ( m_strSearchGroup == _T("����") )
	{
		m_nAcSearchOpt = 1;
		strTBLNAME = _T("AC_INDEX_AUTHOR_TBL");
	}
	else 
		strTBLNAME = _T("");


	//ä��ǥ�� �˻�
	if ( !SetGrid1( strTBLNAME ) )
		return FALSE;

	//����ǥ�� �˻�
	if ( !SetGrid2( strTBLNAME ) ) 
		return FALSE;

	//DM �� ����
	SetDM();

	m_pCM->AllControlDisplay();

	return TRUE;
}

BEGIN_EVENTSINK_MAP(CAC_SEARCH_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CAC_SEARCH_DLG)
	ON_EVENT(CAC_SEARCH_DLG, IDC_GRID1, -600 /* Click */, OnClickGrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CAC_SEARCH_DLG::OnClickGrid1() 
{
	//Grid1�� �����Ǿ��� ��� �ڵ� �˻� �ǵ���..
	INT checkedRow = SetCheckGrid();

	INT ids = -1;
	CString strTBLNAME;
	strTBLNAME.Empty();
	if ( m_strSearchGroup == _T("����") )
	{	
		m_nAcSearchOpt = 0;
		strTBLNAME = _T("AC_INDEX_TITLE_TBL");
	}
	else if ( m_strSearchGroup == _T("����") )
	{
		m_nAcSearchOpt = 1;
		strTBLNAME = _T("AC_INDEX_AUTHOR_TBL");
	}
	else 
		strTBLNAME = _T("");

	if( checkedRow > 0 )
		SetGrid2( strTBLNAME, checkedRow-1 );

	
	CESL_Grid *pGrid = (CESL_Grid *)FindControl( m_pCM->GetAlias(), (_T("GRID2")) );	
	pGrid->Display();
	//m_pCM->AllControlDisplay();
	//AutoSearchProc();
	
}

VOID CAC_SEARCH_DLG::SetDM()
{
	CString strStreamMarc;
	INT ids = -1;
	CMarc* pMarc;
	pMarc = new CMarc;

	for ( INT idx = 0; idx < m_pDM->GetRowCount(); idx++ )
	{
		m_pDM->GetCellData( _T("MARC"), idx, strStreamMarc );

		
		ids = m_pInfo->pMarcMgr->Decoding ( strStreamMarc , pMarc );
		if ( ids < 0 ) return;

		//�ֱ� DM�� �Է�
		CString strItemData;
		m_pInfo->pMarcMgr->GetItem( pMarc , _T("678$a"), strItemData );
		m_pDM->SetCellData ( _T("�ֱ�"), strItemData , idx );

		//ä��ǥ��
		m_pDM->SetCellData ( _T("ä��ǥ��"), m_strSearchItem , idx );

		//������
		m_pDM->SetCellData ( _T("ä��ǥ��"), m_strSearchItem , idx );
	}

	if ( pMarc ) 
	{
		delete pMarc;
		pMarc = NULL;
	}

	return;

}

BOOL CAC_SEARCH_DLG::SetGrid1(CString strTBLNAME)
{
	
	CString strQuery;
	strQuery.Empty();
	
	// KEYWORD���� ������ ������ TBL�� ������ ������.
	// ������ ����, ���� �˻��� ���� - K2���� �װ͸� �����ϱ�...
	strQuery = _T("REC_KEY IN ( SELECT AC_MASTER_MAT_KEY FROM ") + strTBLNAME + _T(" WHERE KEYWORD = '") + m_strSearchItem + _T("')");

	CString strAC_MASTER_KEY;
	strAC_MASTER_KEY.Empty();
	INT ids = -1;
//	ids = m_pDM->GetOneValueQuery( strQuery, strAC_MASTER_KEY );
//	if ( ids < 0 || strAC_MASTER_KEY.IsEmpty() ) 
//	{
//		CString strTmpMsg;
//		strTmpMsg = m_strSearchGroup + _T("(��)�� �˻��� ���Ű� �����ϴ�.") ;
//		AfxMessageBox( strTmpMsg );
//		return FALSE;
//	}
	
	// �˻��� AC_MASTER_MAT_KEY �� ������ Marc�� Rec_key ���ؿ�..
	//strQuery = _T("REC_KEY = ") + strAC_MASTER_KEY + _T("");
	ids = m_pDM->RefreshDataManager ( strQuery );
	if( ids < 0 ) return FALSE;

	if ( m_pDM->GetRowCount() < 1 )
	{
		AfxMessageBox(_T("�˻��� ���Ű� �����ϴ�."));
		return FALSE;
	}
	return TRUE;
}

BOOL CAC_SEARCH_DLG::SetGrid2( CString strTBLNAME, INT nGridIdx /*=0*/ )
{
	//�˻��� ä�� ǥ���� ������ ���� ǥ���� �˻��Ѵ�. 
	if ( m_pDM == NULL || strTBLNAME == _T("") ) 
		return FALSE;

	CString strStreamMarc;
	INT ids = -1;
	CMarc* pMarc;
	pMarc = new CMarc;
	CArray<CString , CString& >  arrItem;
	//pArray->RemoveAll();


	m_pDM->GetCellData( _T("MARC"), nGridIdx , strStreamMarc );

	ids = m_pInfo->pMarcMgr->Decoding ( strStreamMarc , pMarc );
	if ( ids < 0 ) return FALSE;

	m_pRefDM->FreeData();

	//0��° DM MARC���� ���ͼ� ����ǥ�� �Է�
	CString strItemData;
	if ( m_strSearchGroup == _T("����") )
	{
		m_pInfo->pMarcMgr->GetItem( pMarc , _T("430$a"), strItemData, &arrItem );
	}
	else if ( m_strSearchGroup == _T("����") )
	{
		m_pInfo->pMarcMgr->GetItem( pMarc , _T("400$a"), strItemData , &arrItem );
		if ( strItemData.IsEmpty() )
			m_pInfo->pMarcMgr->GetItem( pMarc , _T("410$a"), strItemData , &arrItem );
		if ( strItemData.IsEmpty() )
			m_pInfo->pMarcMgr->GetItem( pMarc , _T("411$a"), strItemData , &arrItem );
	}

	for ( INT idx = 0; idx < arrItem.GetSize(); idx++ )
	{
		strItemData = arrItem.GetAt( idx );
		m_pRefDM->InsertRow(-1);
		m_pRefDM->SetCellData ( _T("����ǥ��"), strItemData , idx );
	}

	return TRUE;
}

VOID CAC_SEARCH_DLG::AutoSearchProc()
{
	//�׸��忡 �����Ǿ��� ��� �ڵ����� �˻�

}

VOID CAC_SEARCH_DLG::OnbOK() 
{
	m_strSearchText = _T("");
	CESL_Grid *pGrid;
	pGrid = (CESL_Grid*)m_pCM->FindControl(_T("GRID2"));

	//������ �ڷᰡ �ִ��� Ȯ��
	if ( !isCheckSelCnt( pGrid ) ) return ;

	//������ �ڷ�� �˻� Key ����
	MakeSearchKeyword( pGrid );

	CDialog::OnOK();

}

BOOL CAC_SEARCH_DLG::isCheckSelCnt( CESL_Grid *pGrid  )
{
	if ( pGrid == NULL ) return FALSE;

	if ( pGrid->SelectGetCount() < 0 ) 
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return FALSE;
	}

	return TRUE;

}

VOID CAC_SEARCH_DLG::MakeSearchKeyword( CESL_Grid *pGrid )
{
	if ( pGrid == NULL ) return;
	pGrid->SelectMakeList();

	CString strGrid2Item;
	INT idx = pGrid->SelectGetHeadPosition();
	m_strSearchText = m_strSearchItem + _T(" ");
	while ( idx > -1 )
	{
		strGrid2Item = pGrid->GetTextMatrix(idx+1, 2);

		m_strSearchText += strGrid2Item + _T(" ");

		idx = pGrid->SelectGetNext();
	}

	m_strSearchText.TrimRight();
}

INT CAC_SEARCH_DLG::SetCheckGrid()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_GRID1 );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 1 != col )		return 0;
	
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );
		pGrid->SetText( _T("") );
	}
	for( i = 1; i < pGrid->GetRows(); i++ )
	{
		if( row == i )
		{
			pGrid->SetRow( i );
			pGrid->SetText( _T("V") );
			break;
		}	
	}

	return row;
}

HBRUSH CAC_SEARCH_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
