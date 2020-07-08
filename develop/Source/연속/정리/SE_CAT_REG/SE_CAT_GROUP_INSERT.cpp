// SE_CAT_GROUP_INSERT.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_GROUP_INSERT.h"
#include "SE_CAT_MAKE_preGROUP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_GROUP_INSERT dialog


CSE_CAT_GROUP_INSERT::CSE_CAT_GROUP_INSERT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_GROUP_INSERT)
	m_Year = _T("");
	//}}AFX_DATA_INIT
}
CSE_CAT_GROUP_INSERT::~CSE_CAT_GROUP_INSERT()
{
}

BOOL CSE_CAT_GROUP_INSERT::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}

VOID CSE_CAT_GROUP_INSERT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_GROUP_INSERT)
	DDX_Text(pDX, IDC_GROUP_YEAR, m_Year);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_GROUP_INSERT, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_GROUP_INSERT)
	ON_BN_CLICKED(IDC_GROUP_SEARCH, OnGroupSearch)
	ON_BN_CLICKED(IDC_NEW_GROUP, OnNewGroup)
	ON_BN_CLICKED(IDC_bSaveNo, OnbSaveNo)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_GROUP_INSERT message handlers

BOOL CSE_CAT_GROUP_INSERT::OnInitDialog() 
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

		m_DM_ALIAS = _T("DM_����_����_����_����"); 
		m_CM_ALIAS = _T("CM_����_����_����_����");


		GetDlgItem(IDC_NEW_GROUP)->ShowWindow(FALSE);

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

		while ( idx != -1 ) {
			
			strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );
			
			if ( strStatus != _T("SER111N"))
			{
				ESLAfxMessageBox(_T("��ϵ� �ڷᰡ �ֽ��ϴ�."));
				EndDialog(IDCANCEL);
				return FALSE;
			}
			
			
			idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
		}


	}

		
	DisplayData();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
INT CSE_CAT_GROUP_INSERT::DisplayData()
{
	CESL_DataMgr *pDM;

	pDM = FindDM(m_DM_ALIAS);

	// 040304_HSS ���� LocalTime���� Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	UpdateData(FALSE);

	OnGroupSearch();

	return 0;
}


BEGIN_EVENTSINK_MAP(CSE_CAT_GROUP_INSERT, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_CAT_GROUP_INSERT)
	ON_EVENT(CSE_CAT_GROUP_INSERT, IDC_GROUPGRID, -600 /* Click */, OnClickGroupgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


VOID CSE_CAT_GROUP_INSERT::OnClickGroupgrid() 
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

VOID CSE_CAT_GROUP_INSERT::OnGroupSearch() 
{
	// TODO: Add your control notification handler code here
	CString strQry,tmpQry;

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

	pDM->RefreshDataManager(strQry);

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

VOID CSE_CAT_GROUP_INSERT::OnbSaveNo() 
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

	// ���� spl ����
	pDM->StartFrame();
	pDM->InitDBFieldData();

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

		pDM->InitDBFieldData();
		
		if( OpenMode == 0 )
		{
			pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SER113O") );
		}
		pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("NUMERIC"), m_GROUP_REC_KEY );
		pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(WORK_LOG) );
		pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(WORK_LOG) );

		pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
	
		if ( OpenMode == 0)
		{
			pDM->SetCellData(_T("BB_�ڷ����_�ڵ�"), _T("SER113O"), idx);
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

	pDM->SendFrame();
	pDM->EndFrame();
	
	if ( OpenMode == 0)
	{
		pGrid->Display();
	}

	AfxMessageBox(_T("���� �Ǿ����ϴ�."));

}

VOID CSE_CAT_GROUP_INSERT::OnNewGroup() 
{
	// TODO: Add your control notification handler code here
	CSE_CAT_MAKE_preGROUP Dlg(this);
	Dlg.DoModal();

	DisplayData();
}

HBRUSH CSE_CAT_GROUP_INSERT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
