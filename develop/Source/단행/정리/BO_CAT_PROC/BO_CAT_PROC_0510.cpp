// BO_CAT_PROC_0510.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0510.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0510 dialog


CBO_CAT_PROC_0510::CBO_CAT_PROC_0510(CESL_Mgr* pParent , CString sRecKey /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0510)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	sSpeciesKey = sRecKey;
	m_pBOSpeciesApi = NULL;
	m_nDividType = -1;
	m_bMultiVol = FALSE;
}

CBO_CAT_PROC_0510::~CBO_CAT_PROC_0510()
{
	if (m_pBOSpeciesApi != NULL)
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}	
}


VOID CBO_CAT_PROC_0510::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0510)
	DDX_Control(pDX, IDC_TAB1, m_ctlTAB);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0510, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0510)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bCat0510INSERT, OnbCat0510INSERT)
	ON_BN_CLICKED(IDC_bObjectINSERT, OnbObjectINSERT)
	ON_BN_CLICKED(IDC_bNewObjectINSERT, OnbNewObjectINSERT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_bNewObjectINSERT2, OnbNewObjectINSERT2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0510 message handlers

BOOL CBO_CAT_PROC_0510::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0510::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0510")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0510"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}
	INT iMainCount = pDM->GetRowCount();

	m_ctlTAB.InsertItem(0, _T("å�и�"));
	m_ctlTAB.InsertItem(1, _T("�Ǻи�"));

	m_nOldDividType = m_nDividType;	

	if ( m_nDividType == -1 || m_nDividType == 0 ) 
	{		
		m_ctlTAB.SetCurSel(0);
		GetDlgItem(IDC_GRD_OBJECT2)->ShowWindow(FALSE);
	}
	else
	{
		m_ctlTAB.SetCurSel(1);
		GetDlgItem(IDC_GRD_OBJECT)->ShowWindow(FALSE);
	}

	CString sYn;
	pDM->GetCellData( _T("BB_åKEY"), 0 , sYn );
	if ( pDM->GetRowCount() == 0 ) {
		CString strWhere;
		strWhere.Format(_T("BOOK_APPENDIX_FLAG = 'B' AND SPECIES_KEY = %s ORDER BY VOL_SORT_NO"), sSpeciesKey);	
		if (pDM->RefreshDataManager(strWhere) < 0)	return FALSE;
	}
	
	INT iRowCount = pDM->GetRowCount();
	iMainCount = iRowCount;
	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}	

	CESL_DataMgr *pDM_0510_OBJECT;
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));
	INT iObjCount = pDM_0510_OBJECT->GetRowCount();
	
	if ( m_nDividType == 0 || m_nDividType == -1) 
	{
		if ( iObjCount > 0 ) 
		{
			GetDlgItem(IDC_bCat0510INSERT)->EnableWindow(true);
			GetDlgItem (IDC_bNewObjectINSERT)->EnableWindow(FALSE);
		}
		else 
		{		
			GetDlgItem(IDC_bCat0510INSERT)->EnableWindow(false);			
		}
		GetDlgItem(IDOK)->EnableWindow(true);
		
	}
	else 
	{
		GetDlgItem(IDC_bCat0510INSERT)->EnableWindow(false);
		GetDlgItem(IDC_bObjectINSERT)->EnableWindow(false);

		CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_����_��_������"));		
		INT nRowCount = pDM_VolumeHold->GetRowCount();

		if ( nRowCount > 0 )
		{
			GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(FALSE);
			GetDlgItem(IDC_bNewObjectINSERT2)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(TRUE);
			GetDlgItem(IDC_bNewObjectINSERT2)->EnableWindow(FALSE);			
		}
		

		GetDlgItem(IDOK)->EnableWindow(false);
		if ( pDM->GetRowCount() < 2 ) {
			GetDlgItem(IDC_bObjectINSERT)->EnableWindow(false);
		}
	}
	
	if ( m_nDividType == 0 )
	{
		CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_����_��_������"));		
		pDM_VolumeHold->FreeData();
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(pDM, pDM_VolumeHold);
		if ( pDM_VolumeHold->GetRowCount() > 1 )
		{
			m_bMultiVol = TRUE;		
		}
		else
		{
			( (CButton*) GetDlgItem (IDC_bNewObjectINSERT) )->EnableWindow(FALSE);
		}
	}

	iObjectCount = iObjCount;

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0510"));
	pCM->AllControlDisplay();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0510::OnbCat0510INSERT() 
{
EFS_BEGIN
	
	INT m_idx;
	CESL_DataMgr *pDM_0510;
	pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));
	iMainCount = pDM_0510->GetRowCount();

	CESL_DataMgr *pDM_0510_OBJECT;
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));

	SelectMakeList(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_OBJECT"));
	m_idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_OBJECT"));

	if ( m_idx == -1 ) return;
	
	if ( m_nOldDividType == -1 )
	{
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);	
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);	
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	
	}
	

	CArray < INT, INT > aIdx;
	while ( m_idx != -1 ) {
		CString sItem;
		pDM_0510_OBJECT->GetCellData( _T("BB_��Ϲ�ȣ") , m_idx , sItem );					
		pDM_0510->InsertRow(-1);	
		iMainCount ++ ;
		pDM_0510->SetCellData( _T("BB_��Ϲ�ȣ") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_�����") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_�����") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_������ηϸ�") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_������ηϸ�") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_��ü����_�ڵ�") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_��ü����_�ڵ�") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_���ı���_�ڵ�") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_���ı���_�ڵ�") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_�����") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_�����") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_����ISBN") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_����ISBN") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_����ISBN�ΰ���ȣ") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_����ISBN�ΰ���ȣ") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_�����") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_�����") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_ũ��") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_ũ��") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_���ݼ���") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_���ݼ���") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_����") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_����") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_����������") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_����������") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_�����ڷ�") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_�����ڷ�") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_������Ư��") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_������Ư��") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_åKEY") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_åKEY") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_��ǥå����") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_��ǥå����") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_���Ϸù�ȣ") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_���Ϸù�ȣ") , sItem, iMainCount-1 );
		
		aIdx.Add( m_idx );
		iObjectCount -- ;

		m_idx = SelectGetNext(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_OBJECT"));
	} 
	
	for( INT i = aIdx.GetSize()-1 ; i >=0 ; i-- ) {
		pDM_0510_OBJECT->DeleteRow( aIdx.GetAt(i) );
	}
	if ( pDM_0510_OBJECT->GetRowCount() == 0 ) 
	{		
		GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(TRUE);		
		GetDlgItem(IDC_bCat0510INSERT)->EnableWindow(FALSE);
		if ( m_bMultiVol == TRUE ) GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(TRUE);
		m_nDividType = -1;		
	}
	else if ( pDM_0510->GetRowCount() > 0 ) {
		GetDlgItem(IDOK)->EnableWindow(true);
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0510"));
	pCM->AllControlDisplay();

EFS_END
}

VOID CBO_CAT_PROC_0510::OnbObjectINSERT() 
{
EFS_BEGIN

	m_ctlTAB.SetCurSel(0);
	GetDlgItem(IDC_GRD_OBJECT)->ShowWindow(TRUE);
	GetDlgItem(IDC_GRD_OBJECT2)->ShowWindow(FALSE);	

	INT m_idx;
	CESL_DataMgr *pDM_0510;
	pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));

	m_ctlTAB.SetCurSel(0);

	CESL_DataMgr *pDM_0510_OBJECT;
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));

	SelectMakeList(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));
	m_idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));	

	if ( m_idx == -1 ) return;
	
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	

	while ( m_idx != -1 ) {
		CString sItem;
		pDM_0510->GetCellData( _T("BB_��Ϲ�ȣ") , m_idx , sItem );					
		pDM_0510_OBJECT->InsertRow(-1);	
		iObjectCount ++ ;
		pDM_0510_OBJECT->SetCellData( _T("BB_��Ϲ�ȣ") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�����") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_�����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_������ηϸ�") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_������ηϸ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_��ü����_�ڵ�") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_��ü����_�ڵ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_���ı���_�ڵ�") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_���ı���_�ڵ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�����") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_�����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_����ISBN") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_����ISBN") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_����ISBN�ΰ���ȣ") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_����ISBN�ΰ���ȣ") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�����") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_�����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_ũ��") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_ũ��") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_���ݼ���") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_���ݼ���") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_����") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_����������") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_����������") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�����ڷ�") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_�����ڷ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_������Ư��") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_������Ư��") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_åKEY") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_åKEY") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_��ǥå����") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_��ǥå����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�̿����ѱ���_�ڵ�") , m_idx , sItem );	
		pDM_0510_OBJECT->SetCellData( _T("BB_�̿����ѱ���_�ڵ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_��ϱ���_�ڵ�") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_��ϱ���_�ڵ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_��������_�ڵ�") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_��������_�ڵ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�ڷ����") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_�ڷ����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_��ġ��ȣ_�ڵ�") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_��ġ��ȣ_�ڵ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_���Ϸù�ȣ") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_���Ϸù�ȣ") , sItem, iObjectCount-1 );		

		m_idx = SelectGetNext(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));
	} 

	SelectMakeList(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));
	m_idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));

	CArray < INT, INT > aIdx;
	while ( m_idx != -1 ) {
		aIdx.Add( m_idx );
		iMainCount -- ;
		m_idx = SelectGetNext(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));
	}
	for( INT i = aIdx.GetSize()-1 ; i >=0 ; i-- ) {
		pDM_0510->DeleteRow( aIdx.GetAt(i) );
	}
	
	if ( pDM_0510_OBJECT->GetRowCount() > 0 ) {
		GetDlgItem(IDC_bCat0510INSERT)->EnableWindow(true);
		GetDlgItem(IDOK)->EnableWindow(true);
		GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(false);
		GetDlgItem(IDC_bNewObjectINSERT2)->EnableWindow(false);
		m_nDividType = 0;
	}

	if ( pDM_0510->GetRowCount() < 1 ) {
		GetDlgItem(IDOK)->EnableWindow(false);
		GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(false);
		GetDlgItem(IDC_bNewObjectINSERT2)->EnableWindow(false);
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0510"));
	pCM->AllControlDisplay();

EFS_END
}

VOID CBO_CAT_PROC_0510::OnOK() 
{
EFS_BEGIN

	CDialog::OnOK();

EFS_END
}

void CBO_CAT_PROC_0510::OnbNewObjectINSERT() 
{
	m_ctlTAB.SetCurSel(1);
	GetDlgItem(IDC_GRD_OBJECT)->ShowWindow(FALSE);
	GetDlgItem(IDC_GRD_OBJECT2)->ShowWindow(TRUE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	

	CESL_DataMgr *pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));
	CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_����_��_������"));		
	pDM_VolumeHold->FreeData();
	m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(pDM_0510, pDM_VolumeHold);

	if ( pDM_VolumeHold->GetRowCount() > 0 )
	{
		( (CButton*) GetDlgItem ( IDC_bObjectINSERT) )->EnableWindow(FALSE);
		( (CButton*) GetDlgItem ( IDC_bNewObjectINSERT) )->EnableWindow(FALSE);		
		( (CButton*) GetDlgItem ( IDC_bNewObjectINSERT2) )->EnableWindow(TRUE);
		( (CButton*) GetDlgItem ( IDOK) )->EnableWindow(TRUE);
		m_nDividType = 1;
	}
	else
	{
		( (CButton*) GetDlgItem ( IDC_bObjectINSERT) )->EnableWindow(TRUE);
	}

	PasteMultiVol();
	
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0510"));
	pCM->AllControlDisplay();
}

void CBO_CAT_PROC_0510::OnbNewObjectINSERT2() 
{
	if ( m_nOldDividType == -1 )
	{
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);	
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);	
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	
	}
	

	CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_����_��_������"));
	pDM_VolumeHold->FreeData();		
	iObjectCount = 0;

	RecoveryBook();

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0510"));
	pCM->AllControlDisplay();	

	( (CButton*) GetDlgItem ( IDC_bObjectINSERT) )->EnableWindow(TRUE);
	( (CButton*) GetDlgItem ( IDC_bNewObjectINSERT) )->EnableWindow(TRUE);		
	( (CButton*) GetDlgItem ( IDC_bNewObjectINSERT2) )->EnableWindow(FALSE);	
	m_nDividType = -1;
}

void CBO_CAT_PROC_0510::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	INT nCurSel = m_ctlTAB.GetCurSel();

	if ( nCurSel == 0)
	{
		GetDlgItem(IDC_GRD_OBJECT)->ShowWindow(TRUE);
		GetDlgItem(IDC_GRD_OBJECT2)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_GRD_OBJECT)->ShowWindow(FALSE);
		GetDlgItem(IDC_GRD_OBJECT2)->ShowWindow(TRUE);
	}
		
	*pResult = 0;
}

INT CBO_CAT_PROC_0510::PasteMultiVol()
{	
	CESL_DataMgr *pDM_0510;
	pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));

	m_ctlTAB.SetCurSel(1);

	CESL_DataMgr *pDM_0510_OBJECT;
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));	

	INT nRowCount;
	CString sItem;
	CString sVolSortNo;
	nRowCount = pDM_0510->GetRowCount();
	pDM_0510->GetCellData( _T("BB_���Ϸù�ȣ") , 0 , sVolSortNo );
	for ( int i=0 ; i<nRowCount ; i++ )
	{
		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_���Ϸù�ȣ") , i , sItem );
		if ( sItem == sVolSortNo ) continue;

		pDM_0510_OBJECT->InsertRow(-1);
		iObjectCount ++ ;
		pDM_0510_OBJECT->SetCellData( _T("BB_���Ϸù�ȣ") , sItem, iObjectCount-1 );
		
		pDM_0510->GetCellData( _T("BB_��Ϲ�ȣ") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_��Ϲ�ȣ") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�����") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_�����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_������ηϸ�") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_������ηϸ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_��ü����_�ڵ�") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_��ü����_�ڵ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_���ı���_�ڵ�") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_���ı���_�ڵ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�����") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_�����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_����ISBN") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_����ISBN") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_����ISBN�ΰ���ȣ") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_����ISBN�ΰ���ȣ") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�����") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_�����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_ũ��") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_ũ��") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_���ݼ���") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_���ݼ���") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_����") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_����������") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_����������") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�����ڷ�") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_�����ڷ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_������Ư��") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_������Ư��") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_åKEY") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_åKEY") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_��ǥå����") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_��ǥå����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�̿����ѱ���_�ڵ�") , i , sItem );	
		pDM_0510_OBJECT->SetCellData( _T("BB_�̿����ѱ���_�ڵ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_��ϱ���_�ڵ�") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_��ϱ���_�ڵ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_��������_�ڵ�") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_��������_�ڵ�") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_�ڷ����") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_�ڷ����") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_��ġ��ȣ_�ڵ�") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_��ġ��ȣ_�ڵ�") , sItem, iObjectCount-1 );	
	} 
	
	for ( i=0 ; i<nRowCount ; i++ )
	{
		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_���Ϸù�ȣ") , nRowCount - i - 1 , sItem );
		if ( sItem == sVolSortNo ) continue;

		pDM_0510->DeleteRow(nRowCount - i - 1);
	}

	CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_����_��_������"));		
	nRowCount = pDM_VolumeHold->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		sItem = _T("");
		pDM_VolumeHold->GetCellData( _T("BB_���Ϸù�ȣ") , nRowCount - i - 1 , sItem );
		if ( sItem == sVolSortNo ) pDM_VolumeHold->DeleteRow(nRowCount - i - 1);				
	}

	return 0;	
}

INT CBO_CAT_PROC_0510::RecoveryBook()
{
	INT iMainCount;
	CESL_DataMgr *pDM_0510;
	pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));	
	iMainCount = pDM_0510->GetRowCount();

	CESL_DataMgr *pDM_0510_OBJECT;
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));
	
	CString sItem;
	for ( int i=0 ; i<pDM_0510_OBJECT->GetRowCount() ; i++ )	
	{		
		pDM_0510_OBJECT->GetCellData( _T("BB_��Ϲ�ȣ") , i , sItem );					
		pDM_0510->InsertRow(-1);	
		iMainCount ++ ;
		pDM_0510->SetCellData( _T("BB_��Ϲ�ȣ") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_�����") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_�����") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_������ηϸ�") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_������ηϸ�") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_��ü����_�ڵ�") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_��ü����_�ڵ�") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_���ı���_�ڵ�") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_���ı���_�ڵ�") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_�����") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_�����") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_����ISBN") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_����ISBN") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_����ISBN�ΰ���ȣ") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_����ISBN�ΰ���ȣ") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_�����") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_�����") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_ũ��") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_ũ��") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_���ݼ���") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_���ݼ���") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_����") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_����") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_����������") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_����������") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_�����ڷ�") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_�����ڷ�") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_������Ư��") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_������Ư��") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_åKEY") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_åKEY") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_��ǥå����") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_��ǥå����") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_���Ϸù�ȣ") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_���Ϸù�ȣ") , sItem, iMainCount-1 );
	} 	

	pDM_0510_OBJECT->FreeData();

	return 0;
}

void CBO_CAT_PROC_0510::OnCancel() 
{
	m_nDividType = m_nOldDividType;	
	CDialog::OnCancel();
}

HBRUSH CBO_CAT_PROC_0510::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
