// BO_CAT_GROUP_INSERT.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_GROUP_INSERT.h"
#include "BO_CAT_MAKE_preGROUP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_GROUP_INSERT dialog


CBO_CAT_GROUP_INSERT::CBO_CAT_GROUP_INSERT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_GROUP_INSERT)
	m_Year = _T("");
	//}}AFX_DATA_INIT
}



CBO_CAT_GROUP_INSERT::~CBO_CAT_GROUP_INSERT()
{
	
}

BOOL CBO_CAT_GROUP_INSERT::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}
VOID CBO_CAT_GROUP_INSERT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_GROUP_INSERT)
	DDX_Text(pDX, IDC_GROUP_YEAR, m_Year);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_GROUP_INSERT, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_GROUP_INSERT)
	ON_BN_CLICKED(IDC_GROUP_SEARCH, OnGroupSearch)
	ON_BN_CLICKED(IDC_bSaveNo, OnbSaveNo)
	ON_BN_CLICKED(IDC_NEW_GROUP, OnNewGroup)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_GROUP_INSERT message handlers

BOOL CBO_CAT_GROUP_INSERT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	if(OpenMode == 0) //������ �ۼ� 
	{
		if (InitESL_Mgr(_T("SM_����_����_�����δ���_�߰�")) < 0)
		{
			AfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}

		m_DM_ALIAS = _T("DM_����_����_���_����"); 
		m_CM_ALIAS = _T("CM_����_����_���_����");
	}
	else if(OpenMode == 1) // ������ �ڷ� - ������ �߰� ���� ���� 
	{
		if (InitESL_Mgr(_T("SM_����_����_�����δ���_�߰�")) < 0)
		{
			AfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}

		m_DM_ALIAS = _T("DM_����_����_������_����");
		m_CM_ALIAS = _T("CM_����_����_������_����");
	}
	else if(OpenMode == 2) // ���� ���� ���� - ���� �߰� ���� ����
	{
		if (InitESL_Mgr(_T("SM_����_����_���δ���_�߰�")) < 0)
		{
			AfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}
		
		GetDlgItem(IDC_NEW_GROUP)->ShowWindow(FALSE);

		m_DM_ALIAS = _T("DM_����_����_����_����"); 
		m_CM_ALIAS = _T("CM_����_����_����_����");
	}

	INT idx;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));

	if(idx < 0)
	{
		//����ó��
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	if(OpenMode == 0) //����ΰ� �ڷ����� Ȯ�� 
	{
		CString strStatus;
		CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		if ( strStatus != _T("BOR111N"))
		{
			ESLAfxMessageBox(_T("��ϵ� �ڷᰡ �ֽ��ϴ�."));
			EndDialog(IDCANCEL);
			return FALSE;
		}

		while ( idx != -1 ) {
			
			strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );
			
			if ( strStatus != _T("BOR111N"))
			{
				ESLAfxMessageBox(_T("��ϵ� �ڷᰡ �ֽ��ϴ�."));
				EndDialog(IDCANCEL);
				return FALSE;
			}
			
			
			idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
		}


	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 		
	DisplayData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBO_CAT_GROUP_INSERT::DisplayData()
{
	CESL_DataMgr *pDM;

	pDM = FindDM(m_DM_ALIAS);

	// 040226_HSS ���� LocalTime���� Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	UpdateData(FALSE);

	OnGroupSearch();

	return 0;
}

BEGIN_EVENTSINK_MAP(CBO_CAT_GROUP_INSERT, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_GROUP_INSERT)
	ON_EVENT(CBO_CAT_GROUP_INSERT, IDC_GROUPGRID, -600 /* Click */, OnClickGroupgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_GROUP_INSERT::OnClickGroupgrid() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid;

	if (OpenMode == 2)
		pGrid=( CESL_Grid * )FindControl(_T("CM_����_����_���δ���_�߰�"), _T("GRID"));
	else
		pGrid=( CESL_Grid * )FindControl(_T("CM_����_����_�����δ���_�߰�"), _T("GRID"));
		
		
	INT row = pGrid->GetRow();
	
	if (row == 0) return;
	if (row > 0) {
		pGrid->SetReverse(row-1);
		m_GROUP_NO = pGrid->GetAt(row-1,0);
		m_GROUP_REC_KEY = pGrid->GetAt(row-1,4);
	}
	
}

VOID CBO_CAT_GROUP_INSERT::OnGroupSearch() 
{
	// TODO: Add your control notification handler code here
	CString strQry,tmpQry;
	INT ids;

	CESL_DataMgr *pDM;

	if( OpenMode == 0 )
	{
		pDM = FindDM(_T("DM_����_����_�����δ���_�߰�"));
	}
	else if(OpenMode == 1 )
	{
		pDM = FindDM(_T("DM_����_����_�����δ���_�߰�"));
	}
	else if(OpenMode == 2 )
	{
		pDM = FindDM(_T("DM_����_����_���δ���_�߰�"));
	}

	UpdateData(TRUE);

	m_Year.TrimLeft();
	m_Year.TrimRight();


	if( m_Year != _T("") )
	{
		tmpQry.Format(_T(" ACCESSION_REC_MAKE_YEAR = %s "), m_Year);
		strQry = strQry + tmpQry;
	}	

	ids = pDM->RefreshDataManager(strQry);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ���� �ּ���"));
		return;
	}

	CESL_ControlMgr *pCM;
	if( OpenMode == 0 )
	{
		pCM = FindCM(_T("CM_����_����_�����δ���_�߰�"));
	}
	else if(OpenMode == 1 )
	{
		pCM = FindCM(_T("CM_����_����_�����δ���_�߰�"));
	}
	else if(OpenMode == 2 )
	{
		pCM = FindCM(_T("CM_����_����_���δ���_�߰�"));
	}



	pCM->AllControlDisplay();

}

VOID CBO_CAT_GROUP_INSERT::OnbSaveNo() 
{
	// TODO: Add your control notification handler code here
	INT idx,Dm_index=0;
	CString BookKey;


	m_GROUP_REC_KEY.TrimLeft();
	m_GROUP_REC_KEY.TrimRight();

	if( m_GROUP_REC_KEY == _T("") )
	{
		AfxMessageBox(_T("���θ� �����Ͽ� �ֽʽÿ�!"));	
		return ;
	}

	CESL_DataMgr *pDM ;
	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_����_����_����_���� error") );
		return ;
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	pDM = FindDM(m_DM_ALIAS);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_����_����") );
		return ;
	}

	//��� å Ű ��������
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

	// ���� spl ����
	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return;
	}

	pDM->InitDBFieldData();
	if( OpenMode == 0 )
	{
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR113O") );
	}

	pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("NUMERIC"), m_GROUP_REC_KEY );
	ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
	if ( 0 > ids ) return;

	// 040226_HSS IDX_BO_TBL, IDX_TMP_BO_TBL �߰� +++++++++++++++++++++++++++++++
	CString sRec_Key;
	if( OpenMode == 0 )
	{
		sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , idx );

		pDM->InitDBFieldData ();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
		pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, idx);

		CString strToday, sQuery;
		INT ids;
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 				_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', sysdate);") ,sRec_Key);
// 
// 		ids = pDM->AddFrame( sQuery );
// 		if ( 0 > ids ) 
// 		{
// 			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
// 			return;
// 		}
		//=====================================================

		
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	/*	
	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame �� Ȯ���ϼ���"));
		return ;
	}
	*/
	if ( OpenMode == 0)
	{
		pDM->SetCellData(_T("BB_�ڷ����_�ڵ�"), _T("BOR113O"), idx);
		pGrid->DisplayLine(idx);
	}
	else if ( OpenMode == 1)
	{
		pDM->SetCellData(_T("�����ι�ȣ"), m_GROUP_NO, idx);
		pGrid->DisplayLine(idx);

	}
	else if( OpenMode == 2)
	{
		pDM->SetCellData(_T("���ι�ȣ"), m_GROUP_NO, idx);
		pGrid->DisplayLine(idx);

	}
		
	pGrid->DisplayLine(idx);
	
	idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

		/*
		ids = pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
			return;
		}
		*/

		pDM->InitDBFieldData();
		
		if( OpenMode == 0 )
		{
			pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR113O") );
		}
	
		pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("NUMERIC"), m_GROUP_REC_KEY );
		ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
		if ( 0 > ids ) return;

		// 040226_HSS IDX_BO_TBL, IDX_TMP_BO_TBL �߰� +++++++++++++++++++++++++++++++
		if( OpenMode == 0 )
		{
			sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , idx );

			pDM->InitDBFieldData ();
			pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
			pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, idx);

			CString strToday, sQuery;
			INT ids;

			//=====================================================
			//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 			sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 					_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', sysdate);") ,sRec_Key);
// 
// 			ids = pDM->AddFrame( sQuery );
// 			if ( 0 > ids ) 
// 			{
// 				AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
// 				return;
// 			}
			//=====================================================
			
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
		if ( OpenMode == 0)
		{
			pDM->SetCellData(_T("BB_�ڷ����_�ڵ�"), _T("BOR113O"), idx);
			pGrid->DisplayLine(idx);

		}else if( OpenMode == 1)
		{
			pDM->SetCellData(_T("�����ι�ȣ"), m_GROUP_NO, idx);
			pGrid->DisplayLine(idx);

		}else if( OpenMode == 2) 
		{
			pDM->SetCellData(_T("���ι�ȣ"), m_GROUP_NO, idx);
			pGrid->DisplayLine(idx);

		}

		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}

	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame �� Ȯ���ϼ���"));
		return;
	}
	
	if ( OpenMode == 0)
	{
		pGrid->Display();
	}

	AfxMessageBox(_T("���� �Ǿ����ϴ�."));

}

VOID CBO_CAT_GROUP_INSERT::OnNewGroup() 
{
	// TODO: Add your control notification handler code here
	CBO_CAT_MAKE_preGROUP Dlg(this);
	Dlg.DoModal();

	INT ids = Dlg.GetCreateYN();
	if (ids>0) DisplayData();

}
HBRUSH CBO_CAT_GROUP_INSERT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
