// ACMARCGetterDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "ACMARCGetterDlg.h"
#include "..\..\..\����\����\MNG_AC_MANAGER\MNG_AC_MANAGER_02.h"
#include "..\..\..\����\����\MNG_AC_MANAGER\MNG_AC_MANAGER_03.h"
#include "..\..\..\����\����\MNG_AC_MANAGER\MNG_AC_MANAGER_04.h"


#include "MarcExportDlg.h"
#include "MarcImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CACMARCGetterDlg dialog


CACMARCGetterDlg::CACMARCGetterDlg( CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CACMARCGetterDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_marc = NULL;
	m_marcMgr = NULL;
	//}}AFX_DATA_INIT
}

CACMARCGetterDlg::~CACMARCGetterDlg()
{
/*	if( m_marcMgr )
	{
		delete m_marcMgr;
		m_marcMgr = NULL;
	}

	if( m_marc )
	{
		delete m_marc;
		m_marc = NULL;
	}
*/
}

BOOL CACMARCGetterDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CACMARCGetterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACMARCGetterDlg)
	DDX_Control(pDX, IDC_BOOK_AC_MARK, m_bookACMarc);
	DDX_Control(pDX, IDC_BOOL_FLAG, m_bookTag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CACMARCGetterDlg, CDialog)
	//{{AFX_MSG_MAP(CACMARCGetterDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	ON_BN_CLICKED(IDC_bDETAIL_VIEW, OnbDETAILVIEW)
	ON_BN_CLICKED(IDC_bInput, OnbInput)
	ON_BN_CLICKED(IDC_bUpdate, OnbUpdate)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACMARCGetterDlg message handlers

BOOL CACMARCGetterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initia lization here
	
	if( InitESL_Mgr( _T("ACMARCGetterDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : ACMARCGetterDlg") );
		return false;
	}

	//����, ����, �󼼺��� ��ư ��Ȱ��ȭ
	GetDlgItem(IDC_bUpdate)->EnableWindow(false);
	GetDlgItem(IDC_bDelete)->EnableWindow(false);
	GetDlgItem(IDC_bDETAIL_VIEW)->EnableWindow(false);

	//������ Ÿ��Ʋ�� ����
	CString sSetWindowText = _T("");
	if ( m_ACType == 1) 
		sSetWindowText = _T("���� ��������");
	if ( m_ACType == 2) 
		sSetWindowText = _T("���� ��������");
	if ( m_ACType == 3) 
		sSetWindowText = _T("������ ��������");

	SetWindowText(sSetWindowText);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CACMARCGetterDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_ACEdit );
	pEdt->SetWindowText( m_ACData );

	EnableCombo( false );

	// test
	//m_ACType = 1;
}

// ���� ���������� �����Ѵ�. 
INT CACMARCGetterDlg::SetACInfo( CMarcMgr *marcMgr, CMarc *marc, INT ACType, CString acData )
{
	m_marcMgr = marcMgr;
	m_marc = marc;
	m_ACType = ACType;
	m_ACData = acData;
	return 0;
}

// long����Ÿ�� DB�� update�Ѵ�. 
INT CACMARCGetterDlg::UpdateLongData(CString tblNane, CString pkName, CString pkType, CString pk, CString fieldName, CString fieldType, CString data)
{
	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	pDM->StartFrame();
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( fieldName, fieldType, data );
	pDM->ORACLE_OCIMgr_MakeUpdateFrame( tblNane, pkName, pkType, pk );

	pDM->SendFrame();
	pDM->EndFrame();
	
	return 0;
}

// Ȯ��
VOID CACMARCGetterDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	if( !m_ok )
	{
		AfxMessageBox( _T("ä��ǥ���� �����Ͽ� �ֽʽÿ�") );
		return;
	}
	
	CString tag, idx;
	GetSelectedBookInfo( tag, idx );
	if( !tag.GetLength() || !idx.GetLength() )
	{
		AfxMessageBox( _T("���õ� ���� ������ �ֽ��ϴ�.") );
		return;
	}

	if( AddACTagToBookMarc( tag, idx ) )
	{
		AfxMessageBox( _T("Error : AddACTagToBookMarc( CString tag, CString idx )") );
		return;
	}
	
	CDialog::OnOK();	
}

// �ݱ�
VOID CACMARCGetterDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel( );
}

// ������ȸ ǥ�� �˻�
VOID CACMARCGetterDlg::OnbFIND()
{
	// TODO: Add your control notification handler code here

	CString keyWord = GetIndexText();
	keyWord.TrimLeft();
	keyWord.TrimRight();
	if( !keyWord.GetLength() )
	{
		AfxMessageBox( _T("������ȸǥ���� �Է��Ͽ� �ֽʽÿ�") );
		return;
	}

	SearchMasterMarcByKeyword( m_ACType, keyWord );
	EnableCombo( false );
}

// ACType���� �ش� Ű���带 ã�´�. 
INT CACMARCGetterDlg::SearchMasterMarcByKeyword( INT flag, CString keyWord )
{
	CString where;
	where.Format( _T("KEYWORD = '%s'"), keyWord );

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_ACMARCGetterDlg"));

	CESL_DataMgr *pDM;
	CString sTblAcType;	
	CString sTblName;
	switch( flag )
	{
		case 1 :
			pDM = FindDM( _T("DM_ACMARCGetterDlg_Author") );
			sTblAcType = _T("AC_TYPE IN ('0','1','2')");
			sTblName = _T("AC_INDEX_AUTHOR_TBL");
			break;
		case 2 :
			pDM = FindDM( _T("DM_ACMARCGetterDlg_Title") );
			sTblAcType = _T("AC_TYPE IN ('3')");
			sTblName = _T("AC_INDEX_TITLE_TBL");
			break;
		case 3 :
			pDM = FindDM( _T("DM_ACMARCGetterDlg_Subject") );
			sTblAcType = _T("AC_TYPE IN ('4')");
			sTblName = _T("AC_INDEX_SUBJECT_TBL");
			break;
		default :
			return -1;
			break;
	}

	CString strQuery;
	strQuery.Format ( _T(" SELECT REC_KEY, AC_MASTER_MAT_KEY, KEYWORD, FIRST_WORK, LAST_WORK, SELECT_YN FROM %s WHERE KEYWORD = '%s' "), sTblName, keyWord ) ;
	
	INT ids = pDM->RestructDataManager( strQuery );

	INT keyCnt = pDM->GetRowCount();

	if( !keyCnt )
	{
		AfxMessageBox( _T("�˻� ����� �����ϴ�.") );
		return -1;
	}
	CString *ACKey = new CString[ keyCnt ];

	for( INT i = 0; i < keyCnt; i++ )
	{
		if ( flag != 3 ) 
			pDM->GetCellData( _T("AC_MASTER_MAT_KEY"), i, ACKey[ i ] );
		else
			pDM->GetCellData( i, 1, ACKey[ i ] );
	}
		

	SearchMasterACMarc( ACKey, keyCnt );

	delete [] ACKey;
	
	return 0;
}

// �־��� keyword�� ����Ÿ��ũ�� ã�´�. 
INT CACMARCGetterDlg::SearchMasterACMarc( CString *pk, INT cnt )
{
	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	pDM->RefreshDataManager( MakeSelectQuery( pk, cnt ) );
	
	CString marc;
	CString sTmP;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("MARC"), i, marc );
		pDM->SetCellData( _T("ä��ǥ��"), GetIndexText(), i );
	}
	
	AllControlDisplay( _T("CM_ACMARCGetterDlg") );
	
	//ShowGrid();

	return 0;
}

CString CACMARCGetterDlg::MakeSelectQuery( CString *pk, INT cnt )
{
	CString where, tmp;
	for( INT i = 0; i < cnt; i++ )
		if( pk[ i ].GetLength() )
		{
			//tmp.Format( _T(" AC_CONTROL_NO = %s OR "), pk[ i ] );
			tmp.Format( _T(" REC_KEY = %s OR "), pk[ i ] );
			where += tmp;
		}

	if( _T("OR ") == where.Right( 3 ) )
	{
		tmp = where.Left( where.GetLength() - 3 );
		where = tmp;
	}

	where = where + _T("ORDER BY AC_CONTROL_NO");
	return where;
}

// �׸��忡 ä��ǥ�� ���� ������ �����ش�. 
VOID CACMARCGetterDlg::ShowGrid()
{
	ControlDisplay( _T("CM_ACMARCGetterDlg"), _T("�׸���") );
	
	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();
	
	CString marc, idx;

	pGrid->SetCol( 2 );
	INT idxxxxxx = 2;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		
		pGrid->SetRow( i + 1 );

		pDM->GetCellData( _T("MARC"), i, marc );
		CString sACControlNo;
		pDM->GetCellData( _T("AC_CONTROL_NO"), i, sACControlNo );
		
		if( GetACIndex( marc, idx, i ) > 0 )
		{
			pGrid->SetText( idx );
			pGrid->SetTextMatrix( idxxxxxx , 2, idx );
			pGrid->SetTextMatrix( idxxxxxx , 3, sACControlNo );
			idxxxxxx++;

		}
	}	

	pGrid->SetRow( row );
	pGrid->SetCol( col );
}

CString CACMARCGetterDlg::GetIndexText()
{
	CString text;
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_ACEdit );
	pEdt->GetWindowText( text );

	return text;
}

BEGIN_EVENTSINK_MAP(CACMARCGetterDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CACMARCGetterDlg)
	ON_EVENT(CACMARCGetterDlg, IDC_mngGRID_1, -600 /* Click */, OnClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CACMARCGetterDlg::OnClickmngGRID1() 
{
	// TODO: Add your control notification handler code here
	
	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	if( !pDM->GetRowCount() )
		return;
	
	INT checkedRow = SetCheckGrid();

	if( checkedRow )
	{
		SetBookTagList( checkedRow - 1 );
		SetBookIdxList( checkedRow - 1 );
	}	
}

// marc���� ä�� ǥ���� �����Ѵ�.
INT CACMARCGetterDlg::GetACIndex( CString marc, CString &idx, INT dmRow )
{
	CMarc marcS;
	if( this->m_pInfo->pMarcMgr->Decoding( marc, &marcS ) < 0 )
	{
		AfxMessageBox( _T("Error : Marc Decoding") );
		return -1;
	}

	INT idxCnt = 0;
	switch( GetACTypeFromDM( dmRow ) )
	{
		case 0 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("100$a"), idx );
			if( idxCnt > 0 )
				return idxCnt;
			else
				return -1;

			break;

		case 1 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("110$a"), idx );
			if( idxCnt > 0 )
				return idxCnt;
			else
				return -1;

			break;
			
		case 2 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("111$a"), idx );
			if( idxCnt > 0 )
				return idxCnt;
			return -1;
			
			break;

		case 3 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("130$a"), idx );
			if( idxCnt > 0 )
				return idxCnt;
			else
				return -1;

			break;

		case 4 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("150$a"), idx );
			if( idxCnt > 0 )
				return idxCnt;
			else
				return -1;

			break;

		default :
			return -1;
			break;
	}

	return -1;
}

// �׸����� ����row�� �ϳ��� �Ѵ�. 
INT CACMARCGetterDlg::SetCheckGrid()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 1 != col )
		return 0;

	if( _T("V") == pGrid->GetText() )
	{
		pGrid->SetText( _T("") );
		EnableCombo( false );
		m_dmRow = -1;
		return 0;
	}

	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );

		if( row == i )
		{
			pGrid->SetText( _T("V") );
			m_dmRow = i - 1;
		}
		else if( _T("V") == pGrid->GetText() )
			pGrid->SetText( _T("") );
	}

	pGrid->SetRow( row );
	EnableCombo( true );

	return row;
}

// �������� �޺��� ���� UI
INT CACMARCGetterDlg::EnableCombo( bool flag )
{
	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_BOOL_FLAG );
	pCombo->EnableWindow( flag );

	pCombo = ( CComboBox * )GetDlgItem( IDC_BOOK_AC_MARK );
	pCombo->EnableWindow( flag );
	
	CButton *pEdt = ( CButton * )GetDlgItem( IDOK );
	pEdt->EnableWindow( flag );
	
	GetDlgItem(IDC_bUpdate)->EnableWindow(flag);
	GetDlgItem(IDC_bDelete)->EnableWindow(flag);
	GetDlgItem(IDC_bDETAIL_VIEW)->EnableWindow(flag);

	m_ok = flag;
	
	return 0;
}

// ����ǥ�ñ�ȣ �޺� ����Ʈ�� �����Ѵ�. 
INT CACMARCGetterDlg::SetBookTagList( INT dmRow )
{
	m_bookTag.ResetContent();
	
	switch( GetACTypeFromDM( dmRow ) )
	{
		case 0 :
			m_bookTag.InsertString( 0, _T("100") );
			m_bookTag.InsertString( 1, _T("700") );
			break;
		case 1 :
			m_bookTag.InsertString( 0, _T("110") );
			m_bookTag.InsertString( 1, _T("710") );
			break;
		case 2 :
			m_bookTag.InsertString( 0, _T("111") );
			m_bookTag.InsertString( 1, _T("711") );
			break;
		case 3 :
			m_bookTag.InsertString( 0, _T("130") );
			m_bookTag.InsertString( 1, _T("240") );
			m_bookTag.InsertString( 2, _T("730") );
			m_bookTag.InsertString( 3, _T("740") );
			m_bookTag.InsertString( 4, _T("830") );
			m_bookTag.InsertString( 5, _T("940") );
			break;
		case 4 :
			m_bookTag.InsertString( 0, _T("600") );
			m_bookTag.InsertString( 1, _T("610") );
			m_bookTag.InsertString( 2, _T("611") );
			m_bookTag.InsertString( 3, _T("630") );
			m_bookTag.InsertString( 4, _T("650") );
			m_bookTag.InsertString( 5, _T("651") );
			break;
	}
			
	return 0;
}

// ������ũ����ǥ�� �޺� ����Ʈ�� �����Ѵ�. 
INT CACMARCGetterDlg::SetBookIdxList( INT dmRow )
{
	m_bookACMarc.ResetContent();

	CString idx;
	INT num = 0;
	INT idxCnt = 0;

	CMarc marcS;
	if( m_pInfo->pMarcMgr->Decoding( GetMarcFromDM( dmRow ), &marcS ) < 0 )
	{
		AfxMessageBox( _T("Error : Marc Decoding") );
		return -1;
	}

	CArray < CString, CString& > idxArray;
	m_idxSignArray.RemoveAll();
	INT i;	

	switch( GetACTypeFromDM( dmRow ) )
	{
		case 0 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("100$a"), idx );
			if( idxCnt > 0 )
				m_bookACMarc.InsertString( num++, idx );
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("400$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("500$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			break;

		case 1 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("110$a"), idx );
			if( idxCnt > 0 )
				m_bookACMarc.InsertString( num++, idx );

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("410$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("510$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			break;
			
		case 2 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("111$a"), idx, &idxArray, &m_idxSignArray );
			if( idxCnt > 0 )
				m_bookACMarc.InsertString( num++, idx );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("411$a"), idx, &idxArray, &m_idxSignArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("511$a"), idx, &idxArray, &m_idxSignArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			break;

		case 3 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("130$a"), idx );
			if( idxCnt > 0 )
				m_bookACMarc.InsertString( num++, idx );

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("430$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("530$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			break;

		case 4 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("150$a"), idx );
			if( idxCnt > 0 )
				m_bookACMarc.InsertString( num++, idx );

			m_title450Cnt = idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("450$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("530$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			break;
	}
	
	return 0;
}

// ���õ� ����ǥ�ñ�ȣ�� ������ũ����ǥ�� ���� ��ȯ�Ѵ�. 
VOID CACMARCGetterDlg::GetSelectedBookInfo( CString &tag, CString &idx )
{
	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_BOOL_FLAG );
	pCombo->GetWindowText( tag );

	pCombo = ( CComboBox * )GetDlgItem( IDC_BOOK_AC_MARK );
	pCombo->GetWindowText( idx );
}

// ������ũ�� ���õ� �����±׸� �߰��Ѵ�.  
INT CACMARCGetterDlg::AddACTagToBookMarc( CString tag, CString idx )
{
	// m_marcMgr, m_marc

	CString sign;
	switch( GetACTypeFromDM( m_dmRow ) )
	{
		case 0 :
			sign = _T("1 ");
			break;
		case 1 :
			sign = _T("  ");
			break;
		case 2 :
			sign = _T("  ");
			break;
		case 3 :
			sign = _T(" 0");
			break;
		case 4 :
			sign = GetTitleSign();
			break;
	}

	CString strSubfielda;
	strSubfielda.Format(_T("%ca"), SUBFIELD_CODE);
	m_marcMgr->InsertField( m_marc, tag + sign + strSubfielda + idx );

	return 0;
}

// �ش����� marc�� �����´�.
CString CACMARCGetterDlg::GetMarcFromDM( INT dmRow )
{
	CString marc;
	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	pDM->GetCellData( _T("MARC"), dmRow, marc );

	return marc;
}


INT CACMARCGetterDlg::GetACTypeFromDM( INT dmRow )
{
	CString ACType;

	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	pDM->GetCellData( _T("AC_TYPE"), dmRow, ACType );
	
	return _ttoi( ACType );
}

VOID CACMARCGetterDlg::OnbDETAILVIEW() 
{
	// TODO: Add your control notification handler code here

	if( -1 == m_dmRow )
		return;

	CString strStreamMarc = GetMarcFromDM( m_dmRow );
	CMarc marc;
	m_pInfo->pMarcMgr->Decoding( strStreamMarc, &marc );
	CShowACMarcDlg dlg( this );

	dlg.SetMarc( &marc );
	dlg.DoModal();
}

CString CACMARCGetterDlg::GetTitleSign()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_BOOK_AC_MARK );

	return m_idxSignArray.GetAt( pCombo->GetCurSel() );
}

VOID CACMARCGetterDlg::OnbInput() 
{
EFS_BEGIN

	ACInput();
	return;
	
EFS_END
}

VOID CACMARCGetterDlg::OnbUpdate() 
{
EFS_BEGIN

	ACUpdate();
	return;

EFS_END
}

VOID CACMARCGetterDlg::OnbDelete() 
{
EFS_BEGIN

	//����
	ACDelete();
	
EFS_END
}

//==========================================================
// ���� �Է� : bMNGCALL �������� ���ϸ� TRUE, �������� FALSE
//==========================================================
VOID CACMARCGetterDlg::ACInput(BOOL bMNGCALL)
{
EFS_BEGIN

	// bMNGCALL �� TRUE �̸� �������� ȣ��
	// FALSE �̸� ���� Marc���� ȣ��

	//�Է�
	CMNG_AC_MANAGER_03 Dlg(this);

	//����ǥ�� �Է¿� ����ǥ�� �׸� ����
	//���� �޴����� ȣ���� ��� �������� ����
	if ( !bMNGCALL ) 
	{
		CString sTmpRefItem;
		GetDlgItem(IDC_ACEdit)->GetWindowText(sTmpRefItem);
		Dlg.SetRefItem(sTmpRefItem);
	}

	//�������� ��� ����
	if ( m_ACType == 1) 
		Dlg.SetACMode(AUTHOR_MODE);
	if ( m_ACType == 2) 
		Dlg.SetACMode(TITLE_MODE);
	if ( m_ACType == 3) 
		Dlg.SetACMode(SUBJECT_MODE);

	Dlg.DoModal();

	return;

EFS_END
}

//==========================================================
// ���� ����
//==========================================================
VOID CACMARCGetterDlg::ACUpdate(BOOL bMNGCALL)
{
EFS_BEGIN

	SelectMakeList(m_sCmAlias, _T("�׸���"));
	INT idx = SelectGetHeadPosition(m_sCmAlias, _T("�׸���"));

	// bMNGCALL �� TRUE �̸� �������� ȣ��
	// FALSE �̸� ���� Marc���� ȣ��

	CString sTmpItem = _T("");
	if ( m_ACType != 1 && m_ACType != 2 && m_ACType != 3  )
	{
		m_ACType = 0;
		CESL_DataMgr *pDM = NULL;
		pDM = FindDM(m_sDmAlias);
		pDM->GetCellData(_T("AC_TYPE"), idx, sTmpItem);
	}
	
	if ( sTmpItem != _T("") )
	{
		if ( sTmpItem == _T("0") ||  sTmpItem == _T("1") || sTmpItem == _T("2") ) m_ACType = 1;
		else if ( sTmpItem == _T("3") ) m_ACType = 2;
		else if ( sTmpItem == _T("4")) m_ACType = 3;
	}

	if ( m_ACType == 0) return;

	//����
	CMNG_AC_MANAGER_04 Dlg(this);
	//�������� ��� ����
	if ( m_ACType == 1) 
		Dlg.SetACMode(AUTHOR_MODE);
	if ( m_ACType == 2) 
		Dlg.SetACMode(TITLE_MODE);
	if ( m_ACType == 3) 
		Dlg.SetACMode(SUBJECT_MODE);

	//�����ؾ� ��
	//Dlg.SetACMode(AUTHOR_MODE);

	//������ �׸��� Ű�� �ѱ�
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT checkedRow = pGrid->GetRow() -1 ;

	if( checkedRow )
	{
		if ( !bMNGCALL )
		{
			SetBookTagList( checkedRow - 1 );
			SetBookIdxList( checkedRow - 1 );
		}
	}


	
	CESL_DataMgr *pDM = NULL;
	pDM = FindDM( m_sDmAlias );
	if ( pDM == NULL ) return;

	CString sModifyRecKey;
	pDM->GetCellData(_T("REC_KEY"), idx, sModifyRecKey);

	Dlg.SetModifyRecKey(sModifyRecKey);	
	
	
	Dlg.DoModal();

EFS_END
}

VOID CACMARCGetterDlg::ACMarcExport()
{
EFS_BEGIN

	CMarcExportDlg Dlg( this );
	Dlg.DoModal();

	return;

EFS_END
}

VOID CACMARCGetterDlg::ACMarcImport()
{
EFS_BEGIN

	CMarcImportDlg Dlg( this );
	Dlg.DoModal();

	return;
	
EFS_END
}

VOID CACMARCGetterDlg::ACDelete()
{
EFS_BEGIN


	SelectMakeList(m_sCmAlias, _T("�׸���"));
	INT idx = SelectGetHeadPosition(m_sCmAlias, _T("�׸���"));

	CESL_DataMgr *pDM = NULL;
	pDM = FindDM( m_sDmAlias );
	if ( pDM == NULL ) return;

	CString sMasterRecKey;
	pDM->GetCellData(_T("REC_KEY"), idx, sMasterRecKey);

	CString sTblName;
	switch( m_ACType )
	{
	case 1 :
		sTblName = _T("AC_INDEX_AUTHOR_TBL");
		break;
	case 2 :
		sTblName = _T("AC_INDEX_TITLE_TBL");
		break;
	case 3 :
		sTblName = _T("AC_INDEX_SUBJECT_TBL");
		break;
	default :
		return;
		break;
	}

	CESL_DataMgr pDMTmp;

	pDMTmp.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	pDMTmp.StartFrame();
	
	CString strQuery;
	strQuery.Format(_T("DELETE %s WHERE AC_MASTER_MAT_KEY = %s; "), sTblName, sMasterRecKey );
	INT ids = pDMTmp.AddFrame(strQuery);
	if ( ids < 0 ) return;

	strQuery.Format(_T("DELETE AC_MASTER_MAT_TBL WHERE REC_KEY = %s; "), sMasterRecKey );
	ids = pDMTmp.AddFrame(strQuery);
	if ( ids < 0 ) return;

	ids = pDMTmp.SendFrame();
	if ( ids < 0 ) return;
	
	ids = pDMTmp.EndFrame();
	if ( ids < 0 ) return;


	
	AfxMessageBox(_T("�����Ǿ����ϴ�."));
	
	AllControlDisplay(m_sCmAlias);
	return;

EFS_END
}

VOID CACMARCGetterDlg::ACSearch()
{
EFS_BEGIN
	
	CMNG_AC_MANAGER_02 Dlg(this);
	Dlg.DoModal();
	
EFS_END
}

HBRUSH CACMARCGetterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
