// DMDP_IMPORT1.cpp : implementation file
//

#include "stdafx.h"
#include "DMDP_IMPORT1.h"
#include "BO_CAT_AUTO_REGNO_GIVE.h"

// ���� - ���ں�ȯ�� 
#include "..\..\..\����\K2UP_Common/MakeSearchData.h"

#include "..\..\..\ESL\MARC\MarcToolKit\CreateAutorCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMDP_IMPORT dialog
CMarcAdjLauncher m_MarcAdjLaunCher;

CDMDP_IMPORT::CDMDP_IMPORT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDMDP_IMPORT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pDM_Species = NULL;
	m_pDM_Index = NULL;
	m_pDM_Book = NULL;

//	m_MarcAdjLaunCher = NULL;
}


CDMDP_IMPORT::~CDMDP_IMPORT()
{
	
}


VOID CDMDP_IMPORT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMDP_IMPORT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDMDP_IMPORT, CDialog)
	//{{AFX_MSG_MAP(CDMDP_IMPORT)
	ON_BN_CLICKED(IDC_bSearch, OnbSearch)
	ON_BN_CLICKED(IDC_bImport, OnbImport)
	ON_BN_CLICKED(IDC_bClose, OnbClose)
	ON_WM_CTLCOLOR() 
	ON_CBN_SELCHANGE(IDC_cmbSHELF_CODE_MA, OnSelchangecmbSHELFCODEMA)
	ON_BN_CLICKED(IDC_btnREQUES, OnbtnREQUES)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMDP_IMPORT message handlers

BOOL CDMDP_IMPORT::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CDMDP_IMPORT::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	m_pCatApi = new CCatApi_BO(this);
	
	if (InitESL_Mgr(_T("SM_����_����_����������")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	
	m_pCM = FindCM(_T("CM_����_����_����������"));
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox(_T("FindCM Error"));
		return FALSE;
	}
	

	m_pDM = FindDM(_T("DM_����_����_����������"));
	if ( m_pDM == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}
	
	
	m_pDM_Species = FindDM ( _T("DM_����_����DM_������") );
	if ( m_pDM_Species == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}		
	
	m_pDM_Index = FindDM (_T("DM_����_����DM_��������") );
	if ( m_pDM_Index == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}	

	m_pDM_Book = FindDM (_T("DM_����_����DM_å����") );
	if ( m_pDM_Book == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}		
	
	m_pGrid = static_cast<CESL_Grid*>(FindControl(_T("CM_����_����_����������"), _T("GRID")));
	if (m_pGrid == NULL) return FALSE;
		
	InitComboControl();	


	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
//	m_MarcAdjLaunCher = new CMarcAdjLauncher(this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDMDP_IMPORT::InitComboControl()
{
	m_pMDataStatusCombo = new CJComboBox;
	m_pMDataStatusCombo->SubclassDlgItem( IDC_cDataStatus, this );
	m_pMDataStatusCombo->FormatList( 1, DT_LEFT, false , EDGE );
	m_pMDataStatusCombo->RequireFlag = FALSE;

	m_pMDataStatusCombo->AddItem( _T("��û"), 0, 0 ); //A01
	m_pMDataStatusCombo->AddItem( _T("�Ϸ�"), 0, 1 ); //B01	

	/****************************************************************
	 ****************************************************************/
	//20080229 ADD BY PJW : �������� �߰� IDC_ACADEMIC_CLASS
	m_pMDataAcademicClass = new CJComboBox;
	m_pMDataAcademicClass->SubclassDlgItem( IDC_ACADEMIC_CLASS, this );
	m_pMDataAcademicClass->FormatList( 1, DT_LEFT, false , EDGE );
	m_pMDataAcademicClass->RequireFlag = FALSE;

	m_pMDataAcademicClass->AddItem( _T("�������"), 0, 0 );
	m_pMDataAcademicClass->AddItem( _T("�Ⱥ�"), 0, 1 ); 
	m_pMDataAcademicClass->AddItem( _T("����"), 0, 2 ); 
	m_pMDataAcademicClass->AddItem( _T("�߰�����"), 0, 3 ); 
	/****************************************************************
	 ****************************************************************/
	
	//�⺻���� ��û���� ����
	m_pMDataStatusCombo->SetCurSel( 0 );

	GetDlgItem(IDC_btnREQUES)->ShowWindow(SW_HIDE);
	m_pMDataAcademicClass->SetCurSel( 2 );

	/***************************************************************************
	 ***************************************************************************/
	//20080225 ADD BY PJW : ��� ��ȣ �ڵ� DM���� �ʱ�ȭ
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbREGCODE_CLASS2);
	pCMB->SelectString(_T("DM"));
	CESL_ControlMultiComboBox* pCMB1 = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbSHELF_CODE_MA);
	pCMB1->SelectString(_T("DM"));
	/***************************************************************************
	 ***************************************************************************/

	return true;
}


HBRUSH CDMDP_IMPORT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


VOID CDMDP_IMPORT::OnbSearch() 
{
	CString strWhere = MakeWhereQuery();
	m_pDM->RefreshDataManager( strWhere );
	INT i = m_pDM->GetRowCount();

	/*****************************************************************
	 *****************************************************************/
	//20080225 ADD BY PJW : DM�� �ʱⰪ ����
	CString strShelfCode;
	CString strPage;
	CString strTempPage;
	INT nFindChar;
	CESL_ControlMultiComboBox* pCMB1 = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbSHELF_CODE_MA);
	pCMB1->GetData(strShelfCode);

	for( INT j = 0 ; j < i ; j++ )
	{
		m_pDM->GetCellData(_T("������")  , j	       ,strPage);
		strTempPage.Format(_T("%sp.")	 , strPage);
		m_pDM->SetCellData(_T("������")  , strTempPage , j);
		m_pDM->SetCellData(_T("�з���ȣ"), _T("393.8")     , j);
		m_pDM->SetCellData(_T("���ڱ�ȣ"), _T("��384��")   , j);
		m_pDM->SetCellData(_T("��ġ��ȣ"), strShelfCode, j);
		
	}
	/*****************************************************************
	 *****************************************************************/

	m_pCM->AllControlDisplay();	
}

VOID CDMDP_IMPORT::OnbImport() 
{
	ExecuteDMDPImport();		
}

VOID CDMDP_IMPORT::OnbClose() 
{
	CDialog::OnCancel();
}

CString CDMDP_IMPORT::MakeWhereQuery()
{
	CString strWhere;
	// 0 - ����
	// 1 - ��û
	// 2 - �Ϸ�
	INT nDataStatusidx = m_pMDataStatusCombo->GetCurSel();
	switch ( nDataStatusidx )
	{
	case 0:
			strWhere += _T("STATUS = '0' ");
			( (CButton*) GetDlgItem (IDC_bImport) )->EnableWindow(TRUE);
			break;
	case 1:
			strWhere += _T("STATUS = '1' ");
			( (CButton*) GetDlgItem (IDC_bImport) )->EnableWindow(FALSE);
			break;	
	}

	return strWhere;
}

INT CDMDP_IMPORT::ExecuteDMDPImport()
{
	CString strQuery;
	CString REC_KEY;
	CString strStatus;
	INT nRowCount;

	m_pGrid->SelectMakeList();	
	nRowCount = m_pGrid->SelectGetCount();

	if ( nRowCount <= 0 ) 
	{
		AfxMessageBox(_T("������ �ڷḦ ������ �ּ���"));
		return FALSE;
	}
		
	INT		i;
	INT		nIndex;	
	INT		ids;

	m_pDM_Species->InsertRow(-1);
	m_pDM_Index->InsertRow(-1);
	m_pDM_Book->InsertRow(-1);
	
	INT nGridIdx;
	m_pGrid->SelectGetHeadPosition();
	for ( i=0 ; i<nRowCount ; i++ )
	{	
		nGridIdx = m_pGrid->SelectGetIdx();
		m_pDM->GetCellData(_T("����"), nGridIdx, strStatus);
		if (  strStatus == _T("�Ϸ�") ) continue;

		CMarc *pMarc = NULL;
		pMarc = new CMarc;

		ids = m_pDM->StartFrame();
		if ( 0 > ids ) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		// ��ũ ����
		if ( !MakeImportMarc( pMarc , nGridIdx ) ) 
		{		
			m_pDM->EndFrame();
			return FALSE;
		}
		// ���� DM ����
		if ( !ExecuteMarcImport( nGridIdx ) ) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		/*
		// ����ȭ�� ����
		CESL_Grid *pGrid = NULL;

		pGrid = m_pGrid;
		m_MarcAdjLaunCher.SetParentInfo(m_pDM_Species, m_pDM_Index, pGrid, m_pGrid->GetRow() - 1);
		m_MarcAdjLaunCher.LaunchMarcAdjust(this, this, _T(""), MNG_IMPORT_WORKING, MODE_MONO);		
		*/
		m_pDM->GetCellData(_T("REC_KEY"), nGridIdx, REC_KEY );		
		strQuery.Format(_T("UPDATE DMDP_IMPORT_TMP_TBL SET STATUS='1' WHERE REC_KEY = %s;"), REC_KEY);
		m_pDM->InitDBFieldData();
		ids = m_pDM->AddFrame(strQuery);
		if (ids < 0) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		m_pDM->SendFrame();

		// ����ȭ�鿡 �Ϸ� ó��
		m_pGrid->SetTextMatrix(nGridIdx+1, 2, _T("�Ϸ�"));
		m_pDM->SetCellData(_T("����"), _T("�Ϸ�"), nGridIdx);

		// �⺻ȭ�� �׸��忡 �Է�
		
		
		// �����ڷ�� �̵�
		nGridIdx = m_pGrid->SelectGetNext();
	}

	/*
	m_pDM->InitDBFieldData();
	ids = m_pDM->SendFrame(TRUE);
	if ( ids )
	{
		AfxMessageBox(_T("���� ����."));
		m_pDM->EndFrame();
	}
	*/
	m_pDM->EndFrame();
	
	// �˻�ȭ�� �׸��� �����ڷ� �����
	m_pGrid->SelectGetTailPosition();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		nGridIdx = m_pGrid->SelectGetIdx();

		m_pDM->DeleteRow(nGridIdx);
		m_pGrid->RemoveAt(nGridIdx);
		
		nGridIdx = m_pGrid->SelectGetPrev();
	}	


	AfxMessageBox(_T("������ �ڷ�� ���ԵǾ����ϴ�.\n�����۾��� �Ͽ� �ֽʽÿ�."));
		
	return 0;
}

BOOL CDMDP_IMPORT::MakeImportMarc(CMarc *pMarc, INT nGridIdx)
{
	//���Կ� ��ũ�� �����Ѵ�
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapMarcAlias_DMAlias;
	InitMapMarcTag( mapMarcAlias_DMAlias );

	CString strStreamMarc = MakeStreamMarc( pMarc, mapMarcAlias_DMAlias, nGridIdx );
	if ( strStreamMarc.IsEmpty() ) return FALSE;	

	m_pInfo->pMarcMgr->Decoding( strStreamMarc, pMarc );	
	m_pInfo->pMarcMgr->Encoding( pMarc, strStreamMarc );
	
	//�ڵ尪���� ��������
	CodeApply ();

	//������ StreamMarc�� �� Tbl�� �ִ´�
	m_pDM_Species->SetCellData(_T("BS_MARC"), strStreamMarc, 0 ) ;
	
	return TRUE;
}

BOOL CDMDP_IMPORT::ExecuteMarcImport(INT nGridIdx)
{
	// ��DM
	CESL_DataMgr DM_Species;
	DM_Species.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Species.InitDMPKFromFile(_T("DM_����_����DM_������"));
	DM_Species.SetMarcMgr(m_pInfo->pMarcMgr);

	// ����DM
	CESL_DataMgr DM_Index;
	DM_Index.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Index.InitDMPKFromFile(_T("DM_����_����DM_��������"));
	DM_Index.SetMarcMgr(m_pInfo->pMarcMgr);
	
	// åDM
	CESL_DataMgr DM_Book;
	DM_Book.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Book.InitDMPKFromFile(_T("DM_����_����DM_å����"));
	DM_Book.SetMarcMgr(m_pInfo->pMarcMgr);

	// ��DM
	CESL_DataMgr DM_Volume;
	DM_Volume.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Volume.InitDMPKFromFile(_T("DM_����_����DM_������"));
	DM_Volume.SetMarcMgr(m_pInfo->pMarcMgr);

	INT ids = -1;
	INT idxDM = -1;
	INT nCount = -1;

	INT nColCount = -1;

	INT nRowTo = 0;
	INT nRowsTo = 0;

	CString strData;
	CString strErrorMsg;
    CString nTotalCnt;
    CString strTotalMsg;
    CString nFalseCnt;
	CString nTrueCnt;

	// HSS_040524 �������
	CString strHD_Book, strHD_Species, strHD_Index;
	strHD_Species = _T("BS");
	strHD_Book = _T("BB");
	strHD_Index = _T("IBS");

	// ��ũ���� DM���� �����Ѵ�.
	MakeMarcAdjustDM(m_pDM_Species, 0, m_pDM_Index, 0, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);


	//����� ����
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	ids = DM_Book.SetCellData( strHD_Book + _T("_�����"), result, 0);

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//��Ϲ�ȣ 12�ڸ� �ǵ��
	CString strRegNo;
	ids = m_pDM_Book->GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), 0, strRegNo);
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 		if ( strRegNo.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// 			strRegNo = strRegNo.Left(2) + sRight;
// 		}
// 	}

	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;

	if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
		if ( strRegNo.GetLength() > nRegCodeLength ) {
			strRegNo = m_pParent->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength) );
		}
	}
	//=====================================================
	
	
	ids = DM_Book.SetCellData( strHD_Book + _T("_��Ϲ�ȣ"), strRegNo, 0 );

	//��������
	DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("0") , 0 );

	//REC_KEY ó��
	//Web ���Խÿ��� web ���� TBL�� Rec_key�� ���ƾ� �Ѵ�.
	DM_Species.SetCellData( strHD_Species + _T("_��KEY"), m_pDM->GetCellData(_T("REC_KEY"), nGridIdx ), 0 );

	//��ġ��ȣ ��ϱ��� ����
	CString strRegCode;
	strRegCode.Empty();
	m_pCM->GetControlMgrData( _T("BB_��ϱ���"), strRegCode);
	DM_Book.SetCellData(_T("BB_��ϱ���_�ڵ�"), strRegCode , 0);

	CString strShelfCode;
	strShelfCode.Empty();
	m_pDM->GetCellData( _T("��ġ��ȣ"), nGridIdx, strShelfCode );
	m_pDM_Book->SetCellData( _T("BB_��ġ��ȣ_�ڵ�"), strShelfCode, 0 );
	DM_Book.SetCellData( _T("BB_��ġ��ȣ_�ڵ�"), strShelfCode, 0 );

	m_pDM_Index->SetCellData( _T("IBS_��ġ��ȣ_�ڵ�"), strShelfCode, 0 );
	DM_Index.SetCellData( _T("IBS_��ġ��ȣ_�ڵ�"), strShelfCode, 0 );
		
	//���� ����
	ids = m_pCatApi->DMDPImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, NULL, NULL, NULL, MNG_IMPORT_WORKING );
	if ( ids < 0 ) 
	{
		AfxMessageBox( m_pCatApi->GetLastError() );
		return FALSE;
	}	

	// �����ʷϹ���
	CString strSpeciesKey;
	DM_Species.GetCellData(_T("BS_��KEY"), 0, strSpeciesKey);
	ids = MakeContents(nGridIdx, strSpeciesKey);
	if ( ids ) 
	{
		AfxMessageBox(_T("����,�ʷ� ���� ����"));
		return ids;
	}

	return TRUE;
}

INT CDMDP_IMPORT::MakeMarcAdjustDM(	CESL_DataMgr *pDM_From_Species, 
										INT nFromSpeciesRow, 
										CESL_DataMgr *pDM_From_Index, 
										INT nFromIndexRow, 
										CESL_DataMgr *pDM_To_Species, 
										CESL_DataMgr *pDM_To_Index, 
										CESL_DataMgr *pDM_To_Book, 
										CESL_DataMgr *pDM_To_Volume)
{
	EFS_BEGIN

	if (!pDM_From_Species ||
		!pDM_From_Index ||
		!pDM_To_Index ||
		!pDM_To_Species ||
		!pDM_To_Book ||
		!pDM_To_Volume ||
		nFromSpeciesRow < 0 ||
		nFromIndexRow < 0) return -1;

	// ������ �ʱ�ȭ
	pDM_To_Species->FreeData();
	pDM_To_Index->FreeData();
	pDM_To_Book->FreeData();
	pDM_To_Volume->FreeData();

	pDM_To_Species->InsertRow(-1);
	pDM_To_Index->InsertRow(-1);
	pDM_To_Book->InsertRow(-1);
	pDM_To_Volume->InsertRow(-1);
	

	// �� ī��
	INT ii = pDM_To_Species->GetRowCount();
	m_pCatApi->CopyDM(pDM_From_Species, nFromSpeciesRow, pDM_To_Species, 0);
	// ���� ī��
	m_pCatApi->CopyDM(pDM_From_Index, nFromIndexRow, pDM_To_Index, 0);

	// ��ũ��ũ�� ���߱� ���� 049�� �ش��ϴ� ������ å DM�� �ִ´�...
	CMarc marc;
	CString strStreamMarc = pDM_To_Species->GetCellData( _T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	CString strRegNo;
	CString strItemData;
	CArray<CString, CString&> arrItem;
	m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arrItem);
	INT nRowCount = arrItem.GetSize();

	// 049 Tag�� ������ �ٽ� �����.
	if (nRowCount >= 0) 
	{	
		pDM_To_Book->FreeData();
	}

	if ( nRowCount <= 0 ) return 0;

	// 049 Tag�� ������ŭ å�� �����.
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pDM_To_Book->InsertRow(-1);

		// å�η� �÷���
		pDM_To_Book->SetCellData(_T("BB_å�η��÷���"), _T("B"), idx);

		// DB FLAG ����
		pDM_To_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);

		// �� -> å
		m_pCatApi->CopyDM(pDM_To_Species, 0, pDM_To_Book, idx, _T("BB"));
		// ���� -> å
		m_pCatApi->CopyDM(pDM_To_Index, 0, pDM_To_Book, idx, _T("BB"));
	}

	// ��ǥå ����
	INT nRepBookIdx = m_pCatApi->SetRepresentationBook(pDM_To_Book, _T("BB"));

	// ��ũ�� ��ũ����
	m_pCatApi->ConvertMarcToCatDM(&marc, pDM_To_Species, pDM_To_Index, pDM_To_Book, pDM_To_Volume);

	return 0;

	EFS_END
	return -1;

}

BOOL CDMDP_IMPORT::ExtraQuery(INT nGridIdx)
{
	return TRUE;
}


VOID CDMDP_IMPORT::InitMapMarcTag( CMap<CString, LPCTSTR, CString, LPCTSTR>& mapMarcAlias_DMAlias )
{
	//pmapMarcAlias_DMAlias->RemoveAll( );
	//mapMarcAlias_DMAlias.SetAt(_T("008@0-39"),_T("008�����"));
	mapMarcAlias_DMAlias.SetAt(_T("245$a"),_T("������"));
	/*************************************************************************
	 *************************************************************************/
	//20080228 UPDATE BY PJW : 245$h -> 245$x �� ���Եǵ��� ���� 
	//mapMarcAlias_DMAlias.SetAt("245$h","������_����");
	mapMarcAlias_DMAlias.SetAt(_T("245$x"),_T("������_����"));
	/*************************************************************************
	 *************************************************************************/
	mapMarcAlias_DMAlias.SetAt(_T("245$d"),_T("����"));
	/*************************************************************************
	 *************************************************************************/
	//20080225 DELETE PJW : 1. ����� ��û 650 -> 653 ���� ����
	//mapMarcAlias_DMAlias.SetAt("650$a","������");
	//mapMarcAlias_DMAlias.SetAt("650$a","������_����");
	/*************************************************************************
	 *************************************************************************/
	mapMarcAlias_DMAlias.SetAt(_T("260$b"),_T("����ó"));
	mapMarcAlias_DMAlias.SetAt(_T("260$c"),_T("�����⵵"));
	mapMarcAlias_DMAlias.SetAt(_T("300$a"),_T("������"));
	mapMarcAlias_DMAlias.SetAt(_T("502$a"),_T("��������"));	
	mapMarcAlias_DMAlias.SetAt(_T("502$a"),_T("������"));
	mapMarcAlias_DMAlias.SetAt(_T("502$c"),_T("����������_����"));
	mapMarcAlias_DMAlias.SetAt(_T("502$d"),_T("�����⵵"));	
	/*************************************************************************
	 *************************************************************************/
	//20080225 ADD PJW : 1. ����� ��û 650 -> 653 ���� ����
	//					 2. 056,090 ����Ʈ�� ����(056 : 393.8 ,���ڱ�ȣ : �� 384�� )
	//                   3. ����ġ��ȣ(049$f ) DM����
	//					 4. �������� 500$a, 653$a ����
	//					 5. DM�� �з���ȣ,���ڱ�ȣ,��ġ��ȣ �߰� 
	//.Lookup
	//1.
	//mapMarcAlias_DMAlias.SetAt("653$a","������");
	//mapMarcAlias_DMAlias.Lookup("653$a","������_����");
	//2.
	mapMarcAlias_DMAlias.SetAt(_T("056$a"),_T("�з���ȣ"));
	mapMarcAlias_DMAlias.SetAt(_T("090$a"),_T("�з���ȣ"));
	mapMarcAlias_DMAlias.SetAt(_T("090$b"),_T("���ڱ�ȣ"));
	//3.
	mapMarcAlias_DMAlias.SetAt(_T("049$f"),_T("��ġ��ȣ"));
	//4.
	mapMarcAlias_DMAlias.SetAt(_T("500$a"),_T("��������"));
	//mapMarcAlias_DMAlias.Lookup("653$a","��������");
	/*************************************************************************
	 *************************************************************************/
	/*************************************************************************
	 *************************************************************************/
	//20080229 ADD BY PJW : 090$c, 049$v �� ���� �߰�
	mapMarcAlias_DMAlias.SetAt(_T("090$c"),_T("����"));
	mapMarcAlias_DMAlias.SetAt(_T("049$v"),_T("����"));

	/*************************************************************************
	 *************************************************************************/
}

CString CDMDP_IMPORT::MakeStreamMarc( CMarc *pMarc , 
									   CMap<CString, LPCTSTR, 
									   CString, LPCTSTR>& mapMarcAlias_DMAlias, 
									   INT nGridIdx )
{
	INT ids = -1;
	CString strMarcAlias;
	CString strDMAlias;
	CString strItemData;
	CString strStreamMarc = _T("");
	INT nCode;
	CString strCode;
	CString strAcademic;
	nCode = 31;
	strCode.Format( _T("%ca"), (TCHAR)nCode );

	POSITION pos = mapMarcAlias_DMAlias.GetStartPosition();

	/***************************************************************
	 ***************************************************************/
	//20080225 ADD BY PJW : 653$a Ű����ȭ �ڵ�
	CString strSub;
	CString strSubUSA;
	CString strProfessor;
	CString strConvertData;
	
	m_pDM->GetCellData( _T("������"), nGridIdx, strSub );
	m_pDM->GetCellData( _T("������_����"), nGridIdx, strSubUSA );
	m_pDM->GetCellData( _T("��������"), nGridIdx, strProfessor );

	
	strSub.TrimLeft();		 strSub.TrimRight();
	strSubUSA.TrimLeft();	 strSubUSA.TrimRight();
	strProfessor.TrimLeft(); strProfessor.TrimRight();
	strSub.MakeUpper();
	strSubUSA.MakeUpper();
	strProfessor.MakeUpper();

	strConvertData = ConvertStrData( strSub		 );
	strSub = strConvertData;
	strConvertData = ConvertStrData( strSubUSA	 );
	strSubUSA = strConvertData;
	strConvertData = ConvertStrData( strProfessor );
	strProfessor = strConvertData;

	if( "" != strSub )
	{
		m_pInfo->pMarcMgr->SetItem(pMarc, "653$a", strSub);
	}
	if( "" != strSubUSA )
	{
		m_pInfo->pMarcMgr->SetItem(pMarc, "653$a", strSubUSA);
	}
	if( "" != strProfessor )
	{
		m_pInfo->pMarcMgr->SetItem(pMarc, "653$a", strProfessor);
	}
	//"�����", "������", "����������",������,strCode
	CString strMajor; 
	m_pDM->GetCellData( _T("����������_����"), nGridIdx, strMajor );
	strAcademic.Format(_T("�����%s������%s����������%s%s"), strCode, strCode, strCode,strMajor);
	m_pInfo->pMarcMgr->SetItem(pMarc, _T("653$a"), strAcademic);
	m_pInfo->pMarcMgr->SetItem(pMarc, _T("300$c"), _T("26cm"));

	/***************************************************************
	 ***************************************************************/
	while (pos)
	{		
		mapMarcAlias_DMAlias.GetNextAssoc(pos, strMarcAlias, strDMAlias);
		ids = m_pDM->GetCellData(strDMAlias, nGridIdx, strItemData);
		if ( strDMAlias == _T("008�����") )
		{
			CString result;
			CTime t = CTime::GetCurrentTime();
			CString strTmpYear;
			strTmpYear.Format(_T("%d"), t.GetYear());
			strTmpYear = strTmpYear.Mid(2);
			strItemData.Format(_T("%s%02d%02d"), strTmpYear, t.GetMonth(), t.GetDay());
			ids = 0;
		}
		
		if ( strDMAlias == _T("����") ) 
		{
			if ( strItemData.Left(1) != _T("\\") )
			{
				CString strTmp;
				strTmp.Format (_T("\\%s"), strItemData );
				strItemData = strTmp;
			}
		}		
		

		if (ids >= 0)
			ids = m_pInfo->pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData);
	}

	//��Ϲ�ȣ �ο��Ͽ� 049 Tag ����
	MakeRegNO049Tag( pMarc, nGridIdx );
	m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc );

	return strStreamMarc;
}

VOID CDMDP_IMPORT::CodeApply()
{
	EFS_BEGIN

	BeginWaitCursor();
	
	INT nRowCount = m_pDM_Index->GetRowCount();
	for (INT row = 0; row < nRowCount; row++)
		CodeApply(row);
	
	EndWaitCursor();


	EFS_END
}

VOID CDMDP_IMPORT::CodeApply(INT nIdxDM)
{
	EFS_BEGIN

	// HSS_040524 �������
	CString strHD_Species, strHD_Index;
	strHD_Species = _T("BS");
	strHD_Index = _T("IBS");

	// TODO : ��ũ ���к�~ �ڵ� ����
	INT ids = -1;
	
	CStringArray arrIgnorAlias;
	arrIgnorAlias.Add( strHD_Index + _T("_���ı���_�ڵ�"));
	arrIgnorAlias.Add( strHD_Index + _T("_��ü����_�ڵ�"));

	// ���� ����, ��ü������ �˸°� �־��ش�.

	CMarc marc;
	CString strStreamMarc;
	CString strFormCode;
	CString strMatCode;

	// ���� ����, ��ü������ �˸°� �־��ش�.
	m_pCM->GetControlMgrData(_T("IBS_���ı���_�ڵ�"), strFormCode ) ;
	m_pCM->GetControlMgrData(_T("IBS_��ü����_�ڵ�"), strMatCode ) ;


	m_pDM_Index->SetCellData( strHD_Index + _T("_���ı���_�ڵ�"), strFormCode, nIdxDM);
	m_pDM_Index->SetCellData( strHD_Index + _T("_��ü����_�ڵ�"), strMatCode, nIdxDM);

	
	MakeDMFromCM(m_pCM, m_pDM_Species, -1, nIdxDM, strHD_Species, &arrIgnorAlias);
	MakeDMFromCM(m_pCM, m_pDM_Index, -1 , nIdxDM, strHD_Index, &arrIgnorAlias);

	EFS_END
}


INT CDMDP_IMPORT::MakeDMFromCM(CESL_ControlMgr *pCM, 
								  CESL_DataMgr *pDM, 
								  INT nAppMode /*= -1*/,
								  INT nIdx /*= -1*/, 
								  CString strToAliasHeader /*= _T("")*/, 
								  CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	EFS_BEGIN

	if (pCM == NULL || pDM == NULL) return -1;
	
	INT i = 0;
	INT ids = -1;
	INT nFind = -1;

	if (nIdx == -1)
	{
		ids = pDM->InsertRow(-1);
		if (ids < 0) return -1;
		nIdx = pDM->GetRowCount() - 1;
	}

	CString strAlias;
	CString strItemData;
	CESL_Control_Element *pCE = NULL;
	
	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		
		strAlias = pCE->GetAlias();
		if (strAlias.IsEmpty()) continue;

		// �Ⱥ��̴��� üũ
		if (!reinterpret_cast<CWnd*>(pCM->FindControl(strAlias))->IsWindowEnabled()) continue;

		// Control ���� �����´�
		ids = pCM->GetControlMgrData(strAlias, strItemData);
		if (ids < 0) continue;

		// ��� ����
		if (!strToAliasHeader.IsEmpty())
		{
			nFind = strAlias.Find(_T("_"));
			if (nFind >= 0)
				strAlias = strAlias.Mid(nFind + 1);
			strAlias = strToAliasHeader + _T("_") + strAlias;
		}

		// Default Ignore List
		// ����
		if ( strAlias == _T("IBS_���Ա���_�ڵ�") ) continue;

		// �ٸ��� DB FLAG�� �ٲپ��ش�.
		if (pDM->GetCellData(strAlias, nIdx) != strItemData)
		{
			ids = pDM->SetCellData(strAlias, strItemData, nIdx);
			ids = pDM->SetCellData(strToAliasHeader + _T("_DB_FLAG"), _T("I"), nIdx);
		}
		if (ids < 0) continue;
	}
	return 0;

	EFS_END
	return -1;
}


BOOL CDMDP_IMPORT::MakeRegNO049Tag( CMarc *pMarc, INT nGridIdx )
{
	
	INT ids = -1;
	INT nTotalSelectGridCnt = m_pGrid->SelectGetCount();
	CString strRegCode;
	m_pCM->GetControlMgrData(_T("BB_��ϱ���"), strRegCode );

	//��������ȣ�� ������.
	CString strGetLastNumQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '") + strRegCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE");
	CString strLasuNum;
	ids = m_pDM->GetOneValueQuery( strGetLastNumQuery, strLasuNum );
	if ( ids < 0 ) return FALSE;

	//������ ��ȣ TBL�� Lock�� �Ǵ�
//	CString strTblLockQuery = _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = LAST_NUMBER WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '") + strRegCode + _T("';");
//	ids = m_pDM->AddFrame( strTblLockQuery );
//	if ( ids < 0 ) return FALSE;

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	INT iLastNum = _ttoi( strLasuNum.GetBuffer(0) );
	iLastNum ++;		

	CString strTmp;	
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	strTmp.Format(_T("%010d"), iLastNum );
// 
// 	//��Ϲ�ȣ �����
// 	CString strRegNo = strRegCode + strTmp;
// 
// 	m_pDM->SetCellData(_T("��Ϲ�ȣ"), strRegNo, nGridIdx );


	//��Ϲ�ȣ �����
	CString strRegNo;
	strTmp.Format(_T("%d"), iLastNum );
	strRegNo = m_pParent->m_pInfo->MakeRegNo(strRegCode , strTmp);
	strTmp = strRegNo.Mid(nRegCodeLength);
	//=====================================================
	
	m_pDM->SetCellData(_T("��Ϲ�ȣ"), strRegNo, nGridIdx );
	
	//��Ϲ�ȣ �ֱ�
	m_pInfo->pMarcMgr->SetItem( pMarc, _T("049$l"), strRegNo );
	m_pDM_Book->SetCellData(_T("BB_��Ϲ�ȣ"), strRegNo , 0 );


	//��ġ��ȣ ���� �ֱ�
	CString strShelfCode;
	m_pDM->GetCellData( _T("��ġ��ȣ"), nGridIdx, strShelfCode );
	m_pDM_Book->SetCellData( _T("BB_��ġ��ȣ_�ڵ�"), strShelfCode, 0 );
	m_pInfo->pMarcMgr->SetItem( pMarc, _T("049$f"), strShelfCode );

	/***************************************************
	 ***************************************************/
	//20080228 ADD BY PJW : ���� ��ũ �߰�
	CString strschoolNo;
	m_pDM->GetCellData( "����", nGridIdx, strschoolNo );
	m_pInfo->pMarcMgr->SetItem( pMarc, "049$v", strschoolNo );
	/***************************************************
	 ***************************************************/

	m_pDM_Book->SetCellData( _T("BB_��ϱ���_�ڵ�"), strRegCode, 0 );

	CString strUpdateLastNum;
	strUpdateLastNum.Format(_T("%d"), iLastNum );
	CString strUpdateQuery = _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = ") + strUpdateLastNum  +_T(" WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '") + strRegCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE;");
	ids = m_pDM->AddFrame( strUpdateQuery );
	if ( ids < 0 ) return FALSE;

	m_pDM->SendFrame(FALSE);

	return TRUE;
}

INT CDMDP_IMPORT::MakeContents(INT nGridIdx, CString strSpeciesKey)
{
	CString strContentsYN, strAbstractsYN;
	CString strContents, strAbstracts;
	CString strTocKey;	
	CString strConKey;
	CString strAbsKey;
	INT		ids;

	ids = m_pDM->GetCellData(_T("��������"), nGridIdx, strContentsYN );
	if ( ids ) return ids;
	ids = m_pDM->GetCellData(_T("�ʷ�����"), nGridIdx, strAbstractsYN );
	if ( ids ) return ids;

	ids = m_pDM->GetCellData(_T("����"), nGridIdx, strContents );
	if ( ids ) return ids;
	ids = m_pDM->GetCellData(_T("�ʷ�"), nGridIdx, strAbstracts );
	if ( ids ) return ids;

	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();

	if ( strContentsYN == _T("Y")  || !strContents.IsEmpty()  || 
		 strAbstractsYN == _T("Y") || !strAbstracts.IsEmpty() )
	{
		m_pDM->MakeRecKey(strTocKey);
	}
	else
	{
		// ������ ���� �ʷ��� ������ RETURN
		return 0;
	}	

	// TOC_COMMON ����
	m_pDM->InitDBFieldData();
	m_pDM->AddDBFieldData( _T("REC_KEY"), _T("STRING"), strTocKey);	
	m_pDM->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
	if ( strContentsYN == _T("Y")  || !strContents.IsEmpty() )
	{
		m_pDM->MakeRecKey(strConKey);
		m_pDM->AddDBFieldData( _T("CONTENTS_KEY"), _T("STRING"), strConKey);
	}

	if ( strAbstractsYN == _T("Y") || !strAbstracts.IsEmpty() )
	{
		m_pDM->MakeRecKey(strAbsKey);
		m_pDM->AddDBFieldData( _T("ABSTRACTS_KEY"), _T("STRING"), strAbsKey);
	}	
	m_pDM->AddDBFieldData( _T("SPECIES_CLASS"), _T("STRING"), _T("0"));
	m_pDM->AddDBFieldData( _T("VOL_SORT_NO"), _T("STRING"), _T("10"));
	m_pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );
	m_pDM->MakeInsertFrame(_T("BO_TOC_COMMON_TBL"));

	// ���� ����
	if ( strContentsYN == _T("Y")  || !strContents.IsEmpty() )
	{		
		m_pDM->InitDBFieldData();
		m_pDM->AddDBFieldData( _T("REC_KEY"), _T("STRING"), strConKey);	
		m_pDM->AddDBFieldData( _T("TOC_COMMON_KEY"), _T("STRING"), strTocKey);	
		m_pDM->AddDBFieldData( _T("CONTENTS"), _T("STRING"), strContents);	
		m_pDM->AddDBFieldData( _T("USE_LIMIT_CODE"), _T("STRING"), _T("GM"));	
		m_pDM->AddDBFieldData( _T("CREATE_DATE"), _T("NUMERIC"), _T("SYSDATE"));
		m_pDM->MakeInsertFrame(_T("BO_CONTENTS_TBL"));		
	}		

	// �ʷ� ����
	if ( strAbstractsYN == _T("Y")  || !strAbstracts.IsEmpty() )
	{		
		m_pDM->InitDBFieldData();
		m_pDM->AddDBFieldData( _T("REC_KEY"), _T("STRING"), strAbsKey);	
		m_pDM->AddDBFieldData( _T("TOC_COMMON_KEY"), _T("STRING"), strTocKey);	
		m_pDM->AddDBFieldData( _T("ABSTRACTS"), _T("STRING"), strAbstracts);	
		m_pDM->AddDBFieldData( _T("USE_LIMIT_CODE"), _T("STRING"), _T("GM"));	
		m_pDM->AddDBFieldData( _T("CREATE_DATE"), _T("NUMERIC"), _T("SYSDATE"));
		m_pDM->MakeInsertFrame(_T("BO_ABSTRACTS_TBL"));		
	}

	/*
	ids = m_pDM->SendFrame();
	if ( ids < 0 )
	{
		AfxMessageBox(_T("����, �ʷ� ���� ����"));
	}
	*/

	return 0;

}

/******************************************************************
 ******************************************************************/
//20080225 ADD BY PJW : ���ξ� ��������� ������ ��ȯ�Լ�
CString CDMDP_IMPORT::ConvertStrData( CString strData)
{
	INT nFindChar;
	INT ids;
	INT nLength;
	CString strConvertData;
	CString strConvertTempData;
	CString strFindData;
	CString strNextFindData;
	INT nCode;
	CString strCode;

	CMakeSearchData MakeData(m_pParent);

	nCode = 31;
	strCode.Format( _T("%ca"), (TCHAR)nCode );

	strData.Replace(_T(","),_T(" "));
	strData.Replace(_T("."),_T(" "));
	strData.Replace(_T("/"),_T(" "));
	strData.Replace(_T("("),_T(" "));
	strData.Replace(_T(")"),_T(" "));
	strData.Replace(_T("{"),_T(" "));
	strData.Replace(_T("}"),_T(" "));

	nFindChar = strData.Find(_T(" "));
	nLength = strData.GetLength();
	
	if( 1 < nFindChar )
	{
		strFindData = strData.Mid(0,nFindChar);
		strNextFindData = strData.Mid(nFindChar, nLength - nFindChar);
		strFindData.TrimLeft();		strFindData.TrimRight();
		strNextFindData.TrimLeft(); strNextFindData.TrimRight();
	}
	else
	{
		strFindData = strData;
		strNextFindData.Format(_T(""));
		strFindData.TrimLeft();		strFindData.TrimRight();
	}

	while( 1 )
	{
		//���� �ѱ� ��ȯ
		ids = MakeData.ChangeHanjaToHangul( strFindData );	
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			strConvertData.Format(_T("ERROR"));
			return strConvertData;
		}
		
		//����, Ư������ ����
		ids = MakeData.GetKeyWord( strFindData );				
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			strConvertData.Format(_T("ERROR"));
			return strConvertData;
		}

		if( "" != strConvertData)
		{
			strConvertTempData = strConvertData;
			strConvertData.Format( _T("%s%s%s"), strConvertTempData, strCode, strFindData );
		}
		else 
		{
			strConvertData.Format( _T("%s"), strFindData );
		}

		if( "" != strNextFindData )
		{
			nFindChar = strNextFindData.Find(_T(" "));
			nLength = strNextFindData.GetLength();
			if( 1 < nFindChar )
			{
				strFindData = strNextFindData.Mid( 0, nFindChar);
				strConvertTempData = strNextFindData.Mid( nFindChar, nLength - nFindChar );
				strNextFindData = strConvertTempData;
				strFindData.TrimLeft();		strFindData.TrimRight();
				strNextFindData.TrimLeft(); strNextFindData.TrimRight();
			}
			else
			{
				strFindData = strNextFindData;
				strNextFindData.Format(_T(""));
				strFindData.TrimLeft();		strFindData.TrimRight();
			}
			
			
		}
		else 
		{
			break;
		}
	}
	
	return strConvertData;
}

void CDMDP_IMPORT::OnbtnREQUES() 
{
	RrquestHandling();	
}

INT CDMDP_IMPORT::RrquestHandling()
{
	CString strQuery;
	CString REC_KEY;
	CString strStatus;
	INT nRowCount;

	m_pGrid->SelectMakeList();	
	nRowCount = m_pGrid->SelectGetCount();

	if ( nRowCount <= 0 ) 
	{
		AfxMessageBox(_T("��ûó���� �ڷḦ �������ּ���."));
		return FALSE;
	}
		
	INT		i;
	INT		nIndex;	
	INT		ids;

	m_pDM_Species->InsertRow(-1);
	m_pDM_Index->InsertRow(-1);
	m_pDM_Book->InsertRow(-1);
	
	INT nGridIdx;
	m_pGrid->SelectGetHeadPosition();
	for ( i=0 ; i<nRowCount ; i++ )
	{	
		nGridIdx = m_pGrid->SelectGetIdx();
		m_pDM->GetCellData(_T("����"), nGridIdx, strStatus);
		if (  strStatus == _T("��û") ) continue;
		
		m_pDM->GetCellData(_T("REC_KEY"), nGridIdx, REC_KEY );		
		CString strQuery;
		CString strCount;
		CString strSub;
		CString strMessage;

		strQuery.Format(_T("SELECT COUNT(1) FROM BO_SPECIES_TBL WHERE REC_KEY = %s"), REC_KEY);
		m_pDM->GetCellData( _T("������"), nGridIdx, strSub); 
		m_pDM->GetOneValueQuery( strQuery, strCount );
		if( 1 <= _ttoi( strCount ) )
		{
			strMessage.Format(_T("[ ������ : %s ]\n�ڷḦ DB������ ��ûó�� ���ּ���."), strSub);
			AfxMessageBox(strMessage);
			m_pGrid->SetTextMatrix(nGridIdx+1, 1, _T(""));
			m_pGrid->SelectMakeList();
			nRowCount = m_pGrid->SelectGetCount();

			if ( 0 >= nRowCount ) 
			{
				AfxMessageBox(_T("��ûó���� �ڷḦ �������ּ���."));
				return FALSE;
			}
			m_pGrid->SelectGetHeadPosition();
			i--;
			continue;
		}
		strQuery.Format(_T("SELECT COUNT(1) FROM BO_DELETE_SPECIES_TBL WHERE REC_KEY = %s"), REC_KEY);
		m_pDM->GetOneValueQuery( strQuery, strCount );
		if( 1 <= _ttoi( strCount ) )
		{
			strMessage.Format(_T("[ ������ : %s ]\n��Ȱ���ڷ�������� ���� �� ��ûó�� ���ּ���."), strSub);
			AfxMessageBox(strMessage);
			m_pGrid->SetTextMatrix(nGridIdx+1, 1, _T(""));
			m_pGrid->SelectMakeList();			
			nRowCount = m_pGrid->SelectGetCount();

			if ( 0 >=  nRowCount) 
			{
				AfxMessageBox(_T("��ûó���� �ڷḦ �������ּ���."));
				return FALSE;
			}
			m_pGrid->SelectGetHeadPosition();
			i--;
			
			continue;
		}

		ids = m_pDM->StartFrame();
		strQuery.Format(_T("UPDATE DMDP_IMPORT_TMP_TBL SET STATUS='0' WHERE REC_KEY = %s;"), REC_KEY);
		m_pDM->InitDBFieldData();
		ids = m_pDM->AddFrame(strQuery);
		if (ids < 0) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		m_pDM->SendFrame();

		// ����ȭ�鿡 ��û ó��
		m_pGrid->SetTextMatrix(nGridIdx+1, 2, _T("��û"));
		m_pDM->SetCellData(_T("����"), _T("��û"), nGridIdx);

		// �⺻ȭ�� �׸��忡 �Է�
		
		
		// �����ڷ�� �̵�
		nGridIdx = m_pGrid->SelectGetNext();
	}

	m_pDM->EndFrame();
	
	// �˻�ȭ�� �׸��� �����ڷ� �����
	m_pGrid->SelectGetTailPosition();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		nGridIdx = m_pGrid->SelectGetIdx();
		
		m_pDM->DeleteRow(nGridIdx);
		if( 0 != nGridIdx)
		{
			m_pGrid->RemoveAt(nGridIdx);
		}
		
		nGridIdx = m_pGrid->SelectGetPrev();	
	}	

	m_pGrid->Display();
	


	AfxMessageBox(_T("��ûó�� �Ǿ����ϴ�.\n��û���� ��˻� �Ͽ��ֽʽÿ�."));
		
	return 0;
}

/*****************************************************************
 *****************************************************************/
//20080225 ADD BY PJW : DM�� ����� ��ġ��ȣ ����
void CDMDP_IMPORT::OnSelchangecmbSHELFCODEMA() 
{
	INT i = m_pDM->GetRowCount();

	CString strShelfCode;
	CESL_ControlMultiComboBox* pCMB1 = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbSHELF_CODE_MA);
	pCMB1->GetData(strShelfCode);

	for( INT j = 0 ; j < i ; j++ )
	{
		m_pDM->SetCellData(_T("��ġ��ȣ"), strShelfCode, j);
	
	}	
}
/*****************************************************************
 *****************************************************************/