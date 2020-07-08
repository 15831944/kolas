// SE_CAT_PREGROUP_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_PREGROUP_MAIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SE_CAT_GIVE_REGNO.h"
#include "MultiSortDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_PREGROUP_MAIN dialog


CSE_CAT_PREGROUP_MAIN::CSE_CAT_PREGROUP_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_PREGROUP_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CSE_CAT_PREGROUP_MAIN::~CSE_CAT_PREGROUP_MAIN()
{
}
BOOL CSE_CAT_PREGROUP_MAIN::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}

INT CSE_CAT_PREGROUP_MAIN::DoSort()
{
	CString strDMAlias;
	CString strCMAlias;
	
	strDMAlias = _T("DM_����_����_������_����");
	strCMAlias = _T("CM_����_����_������_����");
	

	CESL_DataMgr *pDM = FindDM(strDMAlias);
	if (pDM == NULL) return -1;

	CMultiSortDlg dlg(this);

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(strCMAlias, _T("MAIN_GRID"));
	if(pGrid == NULL)
		return -1;
	
	if (dlg.DoModal() == IDOK)
	{
		pDM->SORT(dlg.m_nSortCount, dlg.m_arrStrSortAlias, dlg.m_arrIntSortCond, ESL_DATAMGR_SORT_ASC);

		pGrid->Display();
	}
	return 0;
}

VOID CSE_CAT_PREGROUP_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_PREGROUP_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_PREGROUP_MAIN, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_PREGROUP_MAIN)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_PREGROUP_MAIN message handlers

BOOL CSE_CAT_PREGROUP_MAIN::OnInitDialog() 
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

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



INT CSE_CAT_PREGROUP_MAIN::GiveAllRegNo()
{

	CSE_CAT_GIVE_REGNO dlg(this);
	dlg.DoModal();

	return 0;
}

INT CSE_CAT_PREGROUP_MAIN::BackToRegData()
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

	CString BookKey,strStatus;
	strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

	if (strStatus != _T("SER113O")) //������ 
	{
		//����ó��
		ESLAfxMessageBox(_T("������ �ڷḸ ó�� �� �� �ֽ��ϴ�."));
		return -1;
	}

	idx = SelectGetNext(_T("CM_����_����_������_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		if (strStatus != _T("SER113O")) //������ 
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
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("STRING"), _T("") );
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SER111N") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER113O") );
	pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	
	
	pDM->DeleteRow(idx);
	Dm_index++;
	
	idx = SelectGetNext(_T("CM_����_����_������_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("STRING"), _T("") );
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SER111N") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER113O") );
		pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));

		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;


		idx = SelectGetNext(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
	}

	pDM->SendFrame();

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid_MainGrid_All->Display();

	AfxMessageBox(_T("���� �Ǿ����ϴ�."));

	return 0;


}
INT CSE_CAT_PREGROUP_MAIN::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_�ڷ����_�ڵ�")) {
            if ( str == _T("SER111N")) {
				str = _T("����ΰ��ڷ�");
			}
			else if ( str == _T("SER112N") ) {
				str = _T("����ڷ�");
			}
			else if ( str == _T("SER113O") ) {
				str = _T("�������ڷ�");
			}
        }
    }
	return 0;


EFS_END
return -1;

}

VOID CSE_CAT_PREGROUP_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(-2, 35, cx+2, cy);	
	
}

HBRUSH CSE_CAT_PREGROUP_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
