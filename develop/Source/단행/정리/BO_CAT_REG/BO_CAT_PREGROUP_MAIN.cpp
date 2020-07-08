// BO_CAT_PREGROUP_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PREGROUP_MAIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_CAT_GIVE_REGNO.h"
/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PREGROUP_MAIN dialog


CBO_CAT_PREGROUP_MAIN::CBO_CAT_PREGROUP_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PREGROUP_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSortItemSet = NULL;

}

CBO_CAT_PREGROUP_MAIN::~CBO_CAT_PREGROUP_MAIN()
{
	

}

BOOL CBO_CAT_PREGROUP_MAIN::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}
VOID CBO_CAT_PREGROUP_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PREGROUP_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PREGROUP_MAIN, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PREGROUP_MAIN)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PREGROUP_MAIN message handlers

BOOL CBO_CAT_PREGROUP_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_������_����")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
	if(pGrid) pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	if (pGrid == NULL) return FALSE;
	pGrid->InitPopupMenuItem();
	pGrid->AddPopupMenuItem(static_cast<CView*>(pMain), this->VIEW_STYLE);

	InitSortItem();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CBO_CAT_PREGROUP_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(-2, 35, cx + 4, cy-35);	

}

INT CBO_CAT_PREGROUP_MAIN::GiveAllRegNo()
{

	CBO_CAT_GIVE_REGNO dlg(this);
	dlg.DoModal();

	return 0;
}

INT CBO_CAT_PREGROUP_MAIN::BackToRegData()
{
	INT idx;
	SelectMakeList(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_������_����"), _T("MAIN_GRID"));

	if(idx < 0)
	{
		//����ó��
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return -1;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_������_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_������_����") );
		return -1;
	}

	CString BookKey,strStatus,strSpeciesKey;
	strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

	if (strStatus != _T("BOR113O")) //����ڷ� 
	{
		//����ó��
		ESLAfxMessageBox(_T("������ �ڷḸ ó�� �� �� �ֽ��ϴ�."));
		return -1;
	}

	idx = SelectGetNext(_T("CM_����_����_������_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		if (strStatus != _T("BOR113O")) //����ڷ� 
		{
			//����ó��
			ESLAfxMessageBox(_T("������ �ڷḸ ó�� �� �� �ֽ��ϴ�."));
			return -1;
		}

		idx = SelectGetNext(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
	}



	if(IDCANCEL == ESLAfxMessageBox(_T("����ΰ��ڷ� ���·� ���� �۾��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return -1;



	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_����_������_����"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_����_����_������_���� error") );
		return -1;
	}


	//��� å Ű ��������
	SelectMakeList(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_������_����"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

	// ���� spl ����
	INT Dm_index=0;
	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return -1;
	}
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("STRING"), _T("") );
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR111N") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR113O") );
	ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	
	if ( 0 > ids ) return -1;

	// 040225_HSS IDX_BO_TBL �߰� ++++++++++++++
	strSpeciesKey = pDM->GetCellData( _T("IBS_��KEY") , idx );
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR111N") );
	ids = pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey, idx);	
	if ( 0 > ids ) return -1;
	// +++++++++++++++++++++++++++++++++++++++++

	// 040226_HSS IDX_TMP_BO_TBL �߰� ++++++++++++++
	CString sQuery;
	CString strToday;
	CTime t = CTime::GetCurrentTime();
	strToday.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 	sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 				_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', '%s');")
// 				,strSpeciesKey, strToday);
// 	ids = pDM->AddFrame( sQuery );
// 	if ( 0 > ids ) return -1;
	//=====================================================
	
	// +++++++++++++++++++++++++++++++++++++++++

	pDM->DeleteRow(idx);
	Dm_index++;
	
	idx = SelectGetNext(_T("CM_����_����_������_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("STRING"), _T("") );
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR111N") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR113O") );
		ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));
		if ( 0 > ids ) return -1;

		// 040225_HSS IDX_BO_TBL �߰� ++++++++++++++
		strSpeciesKey = pDM->GetCellData( _T("IBS_��KEY") , (idx - Dm_index) );
		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR111N") );
		ids = pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey, (idx - Dm_index));	
		if ( 0 > ids ) return -1;
		// +++++++++++++++++++++++++++++++++++++++++

		// 040226_HSS IDX_TMP_BO_TBL �߰� ++++++++++++++
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 					_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', '%s');")
// 					,strSpeciesKey, strToday);
// 		ids = pDM->AddFrame( sQuery );
// 		if ( 0 > ids ) return -1;
		//=====================================================
		
		// +++++++++++++++++++++++++++++++++++++++++

		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;


		idx = SelectGetNext(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
	}

	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame �� Ȯ���ϼ���"));
		return -1;
	}

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid_MainGrid_All->Display();

	AfxMessageBox(_T("���� �Ǿ����ϴ�."));

	return 0;


}
INT CBO_CAT_PREGROUP_MAIN::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_�ڷ����_�ڵ�")) {
            if ( str == _T("BOR111N")) {
				str = _T("����ΰ��ڷ�");
			}
			else if ( str == _T("BOR112N") ) {
				str = _T("����ڷ�");
			}
			else if ( str == _T("BOR113O") ) {
				str = _T("�������ڷ�");
			}
        }
    }
	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_PREGROUP_MAIN::ShowSortDlg()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_������_����"));
	
	if(m_pSortItemSet == NULL || pDM == NULL) 
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return;
	}
	if(pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(pDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(1);
	if(IDOK == dlg.DoModal())
		((CESL_Grid*)(FindCM(_T("CM_����_����_������_����"))->FindControl(_T("MAIN_GRID"))))->Display();
}

BOOL CBO_CAT_PREGROUP_MAIN::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	CESL_Grid * pGrid = (CESL_Grid *)FindControl(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
	CESL_DataMgr * pDM = FindDM(_T("DM_����_����_������_����"));
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(pGrid);
	m_pSortItemSet->InitDataManager(pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MAIN_GRID"));
/*
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BB_���Ա���"));
	pTmpData->Add(_T("BP_���Գ⵵"));
	pTmpData->Add(_T("BP_�׷��ȣ"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("UDF_�Ϸù�ȣ"));
	pTmpData->Add(_T("BB_�Ϸù�ȣ"));
	
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("BB_�ڷ����_����"), _T("BB_�ڷ����_�ڵ�"));
*/
	
	m_pSortItemSet->ReadSortItem();
	return TRUE;
}

HBRUSH CBO_CAT_PREGROUP_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}