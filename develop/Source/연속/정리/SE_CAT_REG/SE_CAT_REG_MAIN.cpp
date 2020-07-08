// SE_CAT_REG_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_REG_MAIN.h"

#include "..\..\..\����\����\BO_CAT_REGNO_GIVE\BO_CAT_REGNO_GIVE1.h"
#include "SE_CAT_REG_VIEW.h"
#include "MultiSortDlg.h"

// ���Ӽ���api
#include "..\..\����\SE_API\SeApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REG_MAIN dialog


CSE_CAT_REG_MAIN::CSE_CAT_REG_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_REG_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CAT_REG_MAIN::~CSE_CAT_REG_MAIN()
{
}

BOOL CSE_CAT_REG_MAIN::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}

VOID CSE_CAT_REG_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_REG_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_REG_MAIN, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_REG_MAIN)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REG_MAIN message handlers

BOOL CSE_CAT_REG_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_���_����")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	if(pGrid) pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CAT_REG_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	//pGrid->MoveWindow(-2, 35, cx+2, cy-35);	
	pGrid->MoveWindow(-2, 35, cx+2, cy);
	
}

INT CSE_CAT_REG_MAIN::CreateRegNO()
{

	INT idx;
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		return FALSE;			
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG error") );
		return FALSE;
	}

	// 040305_HSS ��ϱ��� ���� ���� ��Ϲ�ȣ �ο� �ȵǵ���
	//				��ϿϷ��ڷ�(SER112N)�� ó�� �ȵǵ���
	CString strTmp;
	
	while ( idx != -1 ) {
		// ��ϿϷ��ڷ� ������ ����
		strTmp = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );
		if (strTmp == _T("SER112N")) //��ϿϷ��ڷ� 
		{
			ESLAfxMessageBox(_T("��ϴ�� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
			return FALSE;
		}
		// ��ϱ��� ������ ����
		strTmp = pDM->GetCellData( _T("BB_��ϱ���_�ڵ�") , idx );
		if (strTmp == _T(""))
		{
			ESLAfxMessageBox(_T("��ϱ����� ���� �ڷᰡ �ֽ��ϴ�."));
			return FALSE;
		}

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	BO_CAT_REGNO_GIVE dlg(this, _T(""));
	dlg.m_strParentCMAlias = _T("CM_����_����_���_����");
	dlg.m_strParentDMAlias = _T("DM_����_����_���_����");
	dlg.m_nOpenMode = 3;
	dlg.DoModal();
	
	return 0;

}

VOID CSE_CAT_REG_MAIN::DoCatalog() //���� ���ó��
{
	INT idx, ids;
	
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	if(idx < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return ;
	}

	CString BookKey,strStatus;

	// Confirm Registed
	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		if (strStatus == _T("SER111N")) // ��ϴ���ڷ� 
		{
			ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
			return;
		}

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	// Confirm
	if(IDCANCEL == ESLAfxMessageBox(_T("�������ó�� �۾��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return;

	// �ð� �����
	CTime t = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	// �ٽ� ����Ʈ �����
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	BookKey = pDM->GetCellData( _T("BB_åKEY") , idx);

	ids = pDM->StartFrame();
	if( ids < 0 ){
		ESLAfxMessageBox(_T("������� �ڷ�� ������ ������ �߻��߽��ϴ�."));
		return;
	}

	CString strQuery;
	INT Dm_index = 0;

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , idx - Dm_index);
		strQuery.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS='SEC111N', PREV_WORKING_STATUS='SER112N'")
						_T(", REG_TRANSFER_DATE = '%s', TRANSFER_YN='Y' WHERE REC_KEY=%s or BINDING_BOOK_KEY=%s;")
						, strTime, BookKey, BookKey );
		ids = pDM->AddFrame( strQuery );

		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;
		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	ids = pDM->SendFrame();
	if( ids < 0 ){
		ESLAfxMessageBox(_T("������� �ڷ�� ������ ������ �߻��߽��ϴ�."));
		return;
	}

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	pGrid_MainGrid_All->Display();

	ESLAfxMessageBox(_T("������� �ڷ�� ���� ó���Ǿ����ϴ�."));
	
	return;

}

/*
VOID CSE_CAT_REG_MAIN::DoCatalog() //���� ���ó��
{
	INT idx;
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	if(idx < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return ;
	}

	CString BookKey,strStatus,sRec_Key,strRegNo,strCopyCode,strVolTitle;

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		if (strStatus == _T("SER111N")) // ��ϴ���ڷ� 
		{
			ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
			return;
		}

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	if(IDCANCEL == ESLAfxMessageBox(_T("�������ó�� �۾��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return;


	//��� å Ű ��������
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

	CTime t = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	// ���� spl ����
	INT Dm_index=0;
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SEC111N") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER112N") );

	//ȫ�� ���� +++
	pDM->AddDBFieldData(_T("REG_TRANSFER_DATE"), _T("STRING"), strTime );
	pDM->AddDBFieldData(_T("TRANSFER_YN"), _T("STRING"), _T("Y") );
	//ȫ�� ���� ---

	pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	
	
	//049 tag ����
	sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , idx );
	strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
	strCopyCode = pDM->GetCellData( _T("BB_������ȣ") , idx );  
	strVolTitle = pDM->GetCellData( _T("BB_������ηϸ�") , idx );


//	GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);

	//�۾� �ο� ����� 	
	pDM->DeleteRow(idx);
	Dm_index++;
	
	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SEC111N") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER112N") );
		
		//ȫ�� ���� +++
		pDM->AddDBFieldData(_T("REG_TRANSFER_DATE"), _T("STRING"), strTime );
		pDM->AddDBFieldData(_T("TRANSFER_YN"), _T("STRING"), _T("Y") );
		//ȫ�� ���� ---
		
		pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));

		//049 tag ����
		sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , idx - Dm_index);
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx - Dm_index);
		strCopyCode = pDM->GetCellData( _T("BB_������ȣ") , idx - Dm_index);  
		strVolTitle = pDM->GetCellData( _T("BB_������ηϸ�") , idx - Dm_index);

//		GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);

		//�۾� �ο� ����� 	
		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	INT ids = pDM->SendFrame();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("������� �ڷ�� ������ ������ �߻��߽��ϴ�."));
		return;
	}


	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	pGrid_MainGrid_All->Display();

	ESLAfxMessageBox(_T("������� �ڷ�� ���� ó���Ǿ����ϴ�."));
	
	return;

}
*/
VOID CSE_CAT_REG_MAIN::DoRealTimeShelf()
{
	INT idx;
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));


	if(idx < 0)
	{
		//����ó��
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return ;
	}

	CString BookKey,strStatus,sRec_Key,strRegNo,strCopyCode,strVolTitle;
	strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

	if (strStatus != _T("SER111N")) //����ΰ��ڷ� 
	{
		//����ó��
		ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
		return;
	}

	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		if (strStatus != _T("SER111N")) //����ڷ� 
		{
			//����ó��
			ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
			return;
		}

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	if(IDCANCEL == ESLAfxMessageBox(_T("�ǽð� ó�� �۾��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return;

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_����_���_����"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_����_����_���_���� error") );
		return ;
	}


	//��� å Ű ��������
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

	// ���� spl ����
	INT Dm_index=0;
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SEL111O") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER111N") );
	pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	
	

	//�۾� �ο� ����� 	
	pDM->DeleteRow(idx);
	Dm_index++;
	
	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SEL111O") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER111N") );
		pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));

		//�۾� �ο� ����� 	
		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	if( pDM->SendFrame() >= 0 )
	{

	
	}


	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	pGrid_MainGrid_All->Display();

	AfxMessageBox(_T("�ǽð� �ΰ� �ڷ�� ó�� �Ǿ����ϴ�."));

	return;


}

VOID CSE_CAT_REG_MAIN::DoBinding()
{
	INT idx;
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	if(idx < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return ;
	}

	CString BookKey,strStatus;
	strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

	// HSS_040308 ��Ϲ�ȣ �ִ� ���� �ȵ�
	
	// ����ΰ��ڷ����� Ȯ��
	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		//����ڷ�(SER112N)�� ���� ���ͽ� (SER111N)���� �ٲ��� �Ѵ�.
		if (strStatus == _T("SER112N"))
		{
			ESLAfxMessageBox(_T("��ϿϷ�� �ڷᰡ �ֽ��ϴ�.\n��Ϲ�ȣ�� ����� �ּ���."));
			return;
		}
		// �����ڷ����� Ȯ��
		strStatus = pDM->GetCellData( _T("BB_��ȣ����") , idx );
		if (strStatus == _T("����å"))
		{
			ESLAfxMessageBox(_T("������ �ڷ� �� ����å�� �ֽ��ϴ�.\n����å�� �ٽ� ������ �� �����ϴ�."));
			return;
		}
		// HSS_040401 �η��ڷ�� �ȵ�
		else if (strStatus == _T("�η�å"))
		{
			ESLAfxMessageBox(_T("������ �ڷ� �� �η�å�� �ֽ��ϴ�.\n�η�å�� ������ �� �����ϴ�."));
			return;
		}

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	// �ٽ� Selected List �����
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

	if(IDCANCEL == ESLAfxMessageBox(_T("����ó�� �۾��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return;

	//���� ��¥ ���ϱ�
	CTime t = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	pDM->StartFrame();
	INT Dm_index = 0;

	// ���� spl ����
	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , idx - Dm_index );

		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SEB111N") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER111N") );
		pDM->AddDBFieldData(_T("REG_TRANSFER_DATE"), _T("STRING"), strTime );
		pDM->AddDBFieldData(_T("TRANSFER_YN"), _T("STRING"), _T("Y") );

		pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx - Dm_index);
		pDM->InitDBFieldData();

		//�۾� Row ����� 	
		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	INT ids = pDM->SendFrame();
	if( ids < 0 ){
		ESLAfxMessageBox(_T("���� �ΰ��۾��� ������ �߻��߽��ϴ�."));	
		return;
	}

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	pGrid_MainGrid_All->Display();

	ESLAfxMessageBox(_T("���� �ΰ� �ڷ�� ó�� �Ǿ����ϴ�."));

	return;

}

INT CSE_CAT_REG_MAIN::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
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
			else if ( str == _T("SER1130") ) {
				str = _T("�������ڷ�");
			}
        }
    }
	return 0;


EFS_END
return -1;

}

BEGIN_EVENTSINK_MAP(CSE_CAT_REG_MAIN, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_CAT_REG_MAIN)
	ON_EVENT(CSE_CAT_REG_MAIN, IDC_MSHFLEXGRID, -601 /* DblClick */, OnDblClickMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_CAT_REG_MAIN::OnDblClickMshflexgrid() 
{
	CSE_CAT_REG_VIEW Dlg(this);
	Dlg.DoModal();
}

VOID CSE_CAT_REG_MAIN::DoSort()
{
	CString strDMAlias;
	CString strCMAlias;
	
	strDMAlias = _T("DM_����_����_���_����");
	strCMAlias = _T("CM_����_����_���_����");
	
	CESL_DataMgr *pDM = FindDM(strDMAlias);
	if (pDM == NULL) return ;

	CMultiSortDlg dlg(this);

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(strCMAlias, _T("MAIN_GRID"));
	if(pGrid == NULL)
		return;
	
	if (dlg.DoModal() == IDOK)
	{
		pDM->SORT(dlg.m_nSortCount, dlg.m_arrStrSortAlias, dlg.m_arrIntSortCond, ESL_DATAMGR_SORT_ASC);

		pGrid->Display();
	}
}

// 040309_HSS ���� �����
VOID CSE_CAT_REG_MAIN::DeleteRegNo()
{
	INT idx, ids;
	CString strQuery, sCount;
	CString strRegNo, strRegCode, strRecKey;

	// DM, CM, GRID �����ڷ� üũ 
	CESL_DataMgr *pDM;	
	pDM = FindDM(_T("DM_����_����_���_����"));		

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_����_���_����"));		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	// ����Ʈ �����
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	if(idx < 0)	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );

	// Confirm
	if(IDCANCEL == ESLAfxMessageBox(_T("�����Ͻ� �ڷ��� ��Ϲ�ȣ�� ����Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return;
	
	// StartFrame
	ids = pDM->StartFrame();
	if( ids < 0 ){
		ESLAfxMessageBox(_T("��Ϲ�ȣ�� ����� ������ �߻��߽��ϴ�."));
		return;
	}

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
 	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	// AddFrame
	// 1. ���� ��� ���̺�(CO_DROP_ACCESSION_NO_TBL)�� �ִ� �� ���� 
	// 2. ������ ���� ��� ���̺� �Է�
	// 3. SE_BOOK_TBL�� ������Ʈ(�����ڷῡ�� ����)
	// 4. DM�� ������� ����
	// 5. �׸��忡 �ݿ�
	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
		
		if (strRegNo != _T("")) //����ڷ� 
		{
			strRegCode = pDM->GetCellData( _T("BB_��ϱ���_�ڵ�") , idx );
			strRecKey = pDM->GetCellData( _T("BB_åKEY") , idx );

			// Update SE_BOOK_TBL 
			strQuery.Format( _T("UPDATE SE_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL, REG_DATE = ''")
						_T(", WORKING_STATUS = 'SER111N', PREV_WORKING_STATUS = 'SER112N', TRANSFER_YN = 'Y'")
						_T(" WHERE REC_KEY =%s or BINDING_BOOK_KEY=%s;")
						, strRecKey, strRecKey);
			ids = pDM->AddFrame( strQuery );

			// ���� ��� ���̺� ���� ��Ϲ�ȣ�� �ִ��� ���� 
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			strQuery.Format( _T("SELECT COUNT(REG_NO) FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_CODE ='%s' AND REG_NO =%s")
// 							, strRegCode , strRegNo.Mid(2,10));
			strQuery.Format( _T("SELECT COUNT(REG_NO) FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_CODE ='%s' AND REG_NO =%s")
							, strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ));
			//=====================================================				
			pDM->GetOneValueQuery( strQuery , sCount );
			
			// Insert into CO_DROP_ACCESSION_NO_TBL
			if ( sCount == _T("0") ) {
				//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			strQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, ACCESSION_REC_NO, REG_CODE, REG_NO, MANAGE_CODE)")
// 					_T(" VALUES (ESL_SEQ.NEXTVAL, 'SE', '', '%s', %s, UDF_MANAGE_CODE );"), strRegCode, strRegNo.Mid(2,10) );
			strQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, ACCESSION_REC_NO, REG_CODE, REG_NO, MANAGE_CODE)")
					_T(" VALUES (ESL_SEQ.NEXTVAL, 'SE', '', '%s', %s, UDF_MANAGE_CODE );"), strRegCode, strRegNo.Mid(12 - nRegCodeLength ) );
			//=====================================================		
				
				ids = pDM->AddFrame( strQuery );		
			}		
			
			pDM->SetCellData( _T("���ι�ȣ") ,_T(""), idx );
			pDM->SetCellData( _T("BB_��Ϲ�ȣ") ,_T(""), idx );
			pDM->SetCellData( _T("BB_�ڷ����_�ڵ�") ,_T("SER111N"), idx );

			// �ش� �׸��� �ݿ� 
			pGrid->DisplayLine(idx);
		}
		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}
	
	// SendFrame
	ids = pDM->SendFrame();
	if (ids < 0 )
	{
		ESLAfxMessageBox(_T("��Ϲ�ȣ ����� ������ �߻��Ͽ����ϴ�."));
		return;
	}
	else{
		ESLAfxMessageBox(_T("��Ϲ�ȣ ��Ұ� ���������� ����Ǿ����ϴ�."));	
	}

	// Display
	//pGrid->Display();

	return;

}


// 041323_HSS å/�ηϻ��� �߰�
VOID CSE_CAT_REG_MAIN::DeleteBook()
{
	// DM, CM, GRID �����ڷ� üũ 
	CESL_DataMgr *pDM;	
	pDM = FindDM(_T("DM_����_����_���_����"));		

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_����_���_����"));		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));
	pGrid->SelectMakeList();

	INT idx = pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
	    return;
	}
	
	CString msg;
	INT nCount = pGrid->SelectGetCount();
	msg.Format(_T("[ %d ] ���� �ڷḦ ���� �Ͻðڽ��ϱ�?"),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return; 

	BeginWaitCursor();

	CSeApi api(this);
	CString strBOOK_KEY, strACQ_TYPE_VOL_KEY, strVOL_KEY;
	CString strSPECIES_KEY, strBOOK_TYPE, strBooktitle;

	INT ids = 0;
	CArray<INT,INT> arrayDeletedRow;

	ids = pDM->StartFrame();
	if (ids < 0)
	{
		AfxMessageBox(_T("StartFrame Error!"));
	    return;
	}

	while(idx>=0)
	{
		strBOOK_KEY = pDM->GetCellData(_T("BB_åKEY"),idx);
		strACQ_TYPE_VOL_KEY = pDM->GetCellData(_T("BB_���Ա��к���KEY"),idx);
		strVOL_KEY = pDM->GetCellData(_T("��ȣŰ"),idx);
		strSPECIES_KEY = pDM->GetCellData(_T("IBS_��KEY"),idx);
		strBOOK_TYPE = pDM->GetCellData(_T("å����"),idx);
		strBooktitle = pDM->GetCellData(_T("IBS_��ǥ��"),idx);
		strBooktitle += _T(" - ") + pDM->GetCellData(_T("BB_������ηϸ�"),idx);
		
		ids = api.BOOK_CAT_DELETE(pDM,
						strBOOK_KEY,
						strACQ_TYPE_VOL_KEY,
						strVOL_KEY,
						strSPECIES_KEY,
						strBOOK_TYPE,
						strBooktitle,
						TRUE, FALSE);

		if(ids<0)
		{
			EndWaitCursor();

			if(-1002 == ids) //cancel
			{
				msg.Format(_T("[ %s ] �ڷḦ ������ �� �����ϴ�!"), strBooktitle);
				AfxMessageBox(msg);
				idx = pGrid->SelectGetNext();
				continue;
			}
			else
			{
				msg.Format(_T("[ %s ] �ڷḦ �����ϴ� �� ������ �߻��� �۾��� �ߴ��մϴ�!"), strBooktitle);
				AfxMessageBox(msg);
				ids = pDM->EndFrame();
				return;
			}
		}
		else 
		{
			arrayDeletedRow.Add(idx);
		}

		idx = pGrid->SelectGetNext();
	}

	ids = pDM->SendFrame();
	if (ids < 0)
	{
		AfxMessageBox(_T("SendFrame Error!"));
	    return;
	}
	
	ids = pDM->EndFrame();
	if (ids < 0)
	{
		AfxMessageBox(_T("SendFrame Error!"));
	    return;
	}

	INT nSize = arrayDeletedRow.GetSize();
	INT nDeletedRow;

	for(INT i=0;i<nSize;i++)
	{
		nDeletedRow = arrayDeletedRow.GetAt(nSize-i-1);
		pDM->DeleteRow(nDeletedRow);
	}
	
	pGrid->Display();
	EndWaitCursor();

	msg.Format(_T("[ %d ] ���� �ڷḦ �����Ͽ����ϴ�!"),nSize);
	AfxMessageBox(msg);

	return;

}

HBRUSH CSE_CAT_REG_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
