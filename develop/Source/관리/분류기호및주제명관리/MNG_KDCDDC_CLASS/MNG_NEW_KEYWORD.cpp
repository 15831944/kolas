// MNG_NEW_KEYWORD.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_NEW_KEYWORD.h"
#include "MNG_NEW_RELATED.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INSERT_MODE 0
#define UPDATE_MODE 1

/////////////////////////////////////////////////////////////////////////////
// CMNG_NEW_KEYWORD dialog


CMNG_NEW_KEYWORD::CMNG_NEW_KEYWORD(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_NEW_KEYWORD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMNG_NEW_KEYWORD::~CMNG_NEW_KEYWORD()
{
	
}


BOOL CMNG_NEW_KEYWORD::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}

VOID CMNG_NEW_KEYWORD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_NEW_KEYWORD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_NEW_KEYWORD, CDialog)
	//{{AFX_MSG_MAP(CMNG_NEW_KEYWORD)
	ON_BN_CLICKED(IDC_CODE_PRE, OnCodePre)
	ON_BN_CLICKED(IDC_CODE_NEXT, OnCodeNext)
	ON_BN_CLICKED(IDC_SAVE_CODE, OnSaveCode)
	ON_BN_CLICKED(IDC_RWORD_INSERT, OnRwordInsert)
	ON_BN_CLICKED(IDC_RWORD_UPDATE, OnRwordUpdate)
	ON_BN_CLICKED(IDC_RWORD_DETELE, OnRwordDetele)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_NEW_KEYWORD message handlers

BOOL CMNG_NEW_KEYWORD::OnInitDialog() 
{
	CDialog::OnInitDialog();


	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_�з���ȣ_������_�Է�")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	

	// UPDATE
	if( OpenMode == UPDATE_MODE )
	{

		SelectMakeList(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"));
		m_INDEX = SelectGetHeadPosition(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"));

		if(m_INDEX < 0)
		{
			//����ó��
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			EndDialog(0);
			return FALSE;
		}	
	
		GetDlgItem(IDC_CODE_PRE)->ShowWindow(true);
		GetDlgItem(IDC_CODE_NEXT)->ShowWindow(true);

		CESL_DataMgr *pDM;
		pDM = FindDM(_T("DM_����_�з���ȣ_������_����"));
		m_strSubjectKey=pDM->GetCellData(_T("REC_KEY"),m_INDEX);
		
		m_pDMFrame = new CESL_DataMgr();
		m_pDMFrame->SetCONNECTION_INFO(pDM->CONNECTION_INFO);// DM for Frame


		Load_Related(m_strSubjectKey);
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2004.06.08 ����� ����
		SetReverse(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"), m_INDEX);
		AllControlDisplay(_T("CM_����_�з���ȣ_������_�Է�"), m_INDEX);

	}
	else	// INSERT
	{
		CESL_DataMgr *pDM;
		pDM = FindDM(_T("DM_����_�з���ȣ_������_����"));
		pDM->MakeRecKey(m_strSubjectKey);
		
		//CESL_DataMgr *m_pDMFrame;
		m_pDMFrame = new CESL_DataMgr();
		m_pDMFrame->SetCONNECTION_INFO(pDM->CONNECTION_INFO);// DM for Frame
	}


	// Start Frame +++++++++++++++++++++++++++++++++++
	INT ids = -1;
	ids = m_pDMFrame->StartFrame();
	if ( ids<0 )
	{
		AfxMessageBox(_T("StartFrame�� Ȯ���ϼ���."));
		return FALSE;
	}
	// +++++++++++++++++++++++++++++++++++++++++++++++++


	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



VOID CMNG_NEW_KEYWORD::OnSaveCode() 
{
EFS_BEGIN
	if( OpenMode == INSERT_MODE )
	{
		if ( INSERT() <0 ) return;
		AfxMessageBox(_T("�Է� �Ͽ����ϴ�."));
	}
	else
	{
		if ( UPDATE() < 0 ) return;
		AfxMessageBox(_T("���� �Ͽ����ϴ�."));

	}
	
	// DM����
	if ( m_pDMFrame != NULL)
	{
		delete m_pDMFrame;
		m_pDMFrame= NULL;
	}

	if ( OpenMode == UPDATE_MODE ) CDialog::OnOK();


	//RealtedDataSave();

EFS_END
}
INT CMNG_NEW_KEYWORD::RealtedDataSave()
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.07 ����� ����
//Frame�� Insert/Update�� �ϳ��� ��ħ
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_����_�з���ȣ_������_���þ�"));
	//FIND REC_KEY INDEX
	INT nCount = pDM->GetRowCount();
	if(nCount == 0 ) return 0;

	CString WorkFlag,strQry;

//	pDM->StartFrame();
	for ( INT i = 0 ; i<nCount ; i++)
	{
		WorkFlag=pDM->GetCellData(_T("�۾�����"),i);

		if(WorkFlag == _T("I"))
		{
			strQry=_T("INSERT INTO CO_RELATED_SUBJECT_NAME_TBL(REC_KEY,SUBJECT_NAME_KEY,RELATED_SUBJECT_NAME,RELATED_SUBJECT_CODE) VALUES(");
			strQry +=pDM->GetCellData(_T("REC_KEY"),i) + _T(",");
			strQry +=pDM->GetCellData(_T("������Ű"),i) + _T(",");
			strQry +=_T("'")+pDM->GetCellData(_T("���þ�"),i) + _T("',");
			strQry +=_T("'")+pDM->GetCellData(_T("���þ��ڵ�"),i) + _T("');");
			
			//pDM->AddFrame(strQry);
			m_pDMFrame->AddFrame(strQry);

		}
		if(WorkFlag == _T("U"))
		{
			strQry=_T("UPDATE CO_RELATED_SUBJECT_NAME_TBL SET ");
			strQry +=_T(" RELATED_SUBJECT_NAME ='")+pDM->GetCellData(_T("���þ�"),i) + _T("',");
			strQry +=_T(" RELATED_SUBJECT_CODE ='")+pDM->GetCellData(_T("���þ��ڵ�"),i)+_T("'");
			strQry +=_T(" WHERE REC_KEY=")+pDM->GetCellData(_T("REC_KEY"),i)+_T(";") ;
			//pDM->AddFrame(strQry);
			m_pDMFrame->AddFrame(strQry);

		}
		/*if(WorkFlag == _T("D"))	
		{
			strQry=_T("DELETE FROM CO_RELATED_SUBJECT_NAME_TBL ");
			strQry +=_T(" WHERE REC_KEY=")+pDM->GetCellData(_T("REC_KEY"),i)+_T(";") ;
			//pDM->AddFrame(strQry);
			m_pDMFrame->AddFrame(strQry);
		}*/
	}
	
	if ( pDM != NULL ) pDM = NULL;

	return 0;
EFS_END
	return -1;
}

INT CMNG_NEW_KEYWORD::INSERT()
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.07 ����� ����
EFS_BEGIN
	CString strName, strDESC, strRecKey;
	INT ids=-1;


	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_�з���ȣ_������_����"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return -1;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_�з���ȣ_������_�Է�"));
	if ( NULL == pCM ) 
	{
		AfxMessageBox(_T("CM �ʱ�ȭ ����"));
		return -1;
	}
	
	// get data from CM
	GetControlData( _T("CM_����_�з���ȣ_������_�Է�"), _T("������"), strName );
	GetControlData( _T("CM_����_�з���ȣ_������_�Է�"), _T("��������"), strDESC );
	strRecKey = pDM->GetCellData(_T("REC_KEY"), m_INDEX);

	strRecKey = m_strSubjectKey;

	INT n_index = pDM->GetRowCount();
	
	// set Date To DM
	pDM->InsertRow(-1);
	pDM->SetCellData(_T("REC_KEY"), strRecKey, n_index);
	pDM->SetCellData(_T("������"), strName, n_index);
	pDM->SetCellData(_T("��������"), strDESC, n_index);
	// ��������
	CTime t = CTime::GetCurrentTime();
	CString sDate;
	sDate.Format ( _T("%4d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
	pDM->SetCellData(_T("��������"), sDate, n_index);


	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, n_index, m_pDMFrame);
	

	// ���þ�
	ids = RealtedDataSave();
	if ( ids<0 )
	{
		AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return -1;
	}

	ids = m_pDMFrame->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return -1;
	}

	ids = m_pDMFrame->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame �� Ȯ���ϼ���"));
		return -1;
	}

	AllControlDisplay(_T("CM_����_�з���ȣ_������_����"), n_index);


	return 0;
EFS_END
	return -1;
}


INT CMNG_NEW_KEYWORD::UPDATE()
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.07 ����� ����
EFS_BEGIN
	CString strName, strDESC, strRecKey;
	INT ids = -1;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_�з���ȣ_������_����"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return -1;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_�з���ȣ_������_�Է�"));
	if ( NULL == pCM ) 
	{
		AfxMessageBox(_T("CM �ʱ�ȭ ����"));
		return -1;
	}

	// get data from CM
	GetControlData( _T("CM_����_�з���ȣ_������_�Է�"), _T("������"), strName );
	GetControlData( _T("CM_����_�з���ȣ_������_�Է�"), _T("��������"), strDESC );
	strRecKey = pDM->GetCellData(_T("REC_KEY"), m_INDEX);

	// set Date To DM
	pDM->SetCellData(_T("������"), strName, m_INDEX);
	pDM->SetCellData(_T("��������"), strDESC, m_INDEX);
	// ��������
	CTime t = CTime::GetCurrentTime();
	CString sDate;
	sDate.Format ( _T("%4d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
	pDM->SetCellData(_T("��������"), sDate, m_INDEX);

	// Start Frame ++++++++++++++++++++++++
	/*ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return -1;
	}*/
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, m_INDEX, m_pDMFrame, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	
	// ���þ�
	ids = RealtedDataSave();
	if ( ids<0 )
	{
		AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return -1;
	}

	ids = m_pDMFrame->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return -1;
	}

	ids = m_pDMFrame->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame �� Ȯ���ϼ���"));
		return -1;
	}
	// End Frame ++++++++++++++++++++++++

	AllControlDisplay(_T("CM_����_�з���ȣ_������_����"), m_INDEX);

	return 0;
EFS_END
	return -1;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


}
VOID CMNG_NEW_KEYWORD::DELETE_CODE()
{
EFS_BEGIN
	INT idx;
	SelectMakeList(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"));

	if(idx < 0)
	{
		//����ó��
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}	

	if(IDCANCEL == ESLAfxMessageBox(_T("���� �۾��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return;

	CESL_DataMgr *pDM = FindDM(_T("DM_����_�з���ȣ_������_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_�з���ȣ_������_����") );
		return ;
	}

	INT ids = -1;

	ids = pDM->StartFrame();
	if ( ids<0 )
	{
		AfxMessageBox(_T("StartFrame�� Ȯ���ϼ���."));
		return;
	}

	INT delete_Count=0;
	CString strRecKey,strQry;
	strRecKey = pDM->GetCellData( _T("REC_KEY") , idx );

	strQry.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"), pDM->TBL_NAME ,strRecKey);
	pDM->AddFrame(strQry);
	strQry.Format(_T("DELETE FROM %s WHERE SUBJECT_NAME_KEY = %s;"), _T("CO_RELATED_SUBJECT_NAME_TBL") ,strRecKey);
	pDM->AddFrame(strQry);
	pDM->DeleteRow(idx);
	delete_Count++;

	idx = SelectGetNext(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strRecKey = pDM->GetCellData( _T("REC_KEY") , idx - delete_Count);

		strQry.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"), pDM->TBL_NAME ,strRecKey);
		pDM->AddFrame(strQry);
		strQry.Format(_T("DELETE FROM %s WHERE SUBJECT_NAME_KEY = %s;"), _T("CO_RELATED_SUBJECT_NAME_TBL") ,strRecKey);
		pDM->AddFrame(strQry);

		pDM->DeleteRow(idx);
		delete_Count++;

		idx = SelectGetNext(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"));
	}

	ids = pDM->SendFrame();
	if ( ids<0 )
	{
		AfxMessageBox(_T("�����۾� �� ������ �߻��Ͽ����ϴ�."));
		return;
	}

	ids = pDM->EndFrame();
	if ( ids<0 )
	{
		AfxMessageBox(_T("EndFrame�� Ȯ���ϼ���."));
		return;
	}

	AllControlDisplay(_T("CM_����_�з���ȣ_������_����"));
	AfxMessageBox(_T("�����Ͽ����ϴ�."));
	
	return;
EFS_END
}


VOID CMNG_NEW_KEYWORD::OnCodePre() 
{
EFS_BEGIN
	
	INT idx;

	idx = SelectGetPrev(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"));
	if (idx < 0 )
	{
		ESLAfxMessageBox(_T("ó�� �ڷ� �Դϴ�.."));
		SelectGetHeadPosition(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"));
		return;
	}

	m_INDEX = idx ;
	
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_�з���ȣ_������_����"));
	m_strSubjectKey=pDM->GetCellData(_T("REC_KEY"),m_INDEX);

	Load_Related(m_strSubjectKey);

	SetReverse(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"), m_INDEX);
	AllControlDisplay(_T("CM_����_�з���ȣ_������_�Է�"), m_INDEX);

EFS_END
}

VOID CMNG_NEW_KEYWORD::OnCodeNext() 
{
EFS_BEGIN
	
	INT idx;

	idx = SelectGetNext(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"));
	if (idx < 0 )
	{
		ESLAfxMessageBox(_T("������ �ڷ� �Դϴ�.."));
		SelectGetTailPosition(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"));
		return;
	}

	m_INDEX = idx ;
	

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_�з���ȣ_������_����"));
	m_strSubjectKey=pDM->GetCellData(_T("REC_KEY"),m_INDEX);

	Load_Related(m_strSubjectKey);

	SetReverse(_T("CM_����_�з���ȣ_������_����"), _T("MAIN_GRID"), m_INDEX);
	AllControlDisplay(_T("CM_����_�з���ȣ_������_�Է�"), m_INDEX);


EFS_END
}


VOID CMNG_NEW_KEYWORD::Load_Related(CString strQry)
{
EFS_BEGIN

	CString Qry;
	Qry.Format(_T(" SUBJECT_NAME_KEY = %s "),strQry);
	CESL_DataMgr *pDM = FindDM(_T("DM_����_�з���ȣ_������_���þ�"));
	pDM->RefreshDataManager(Qry);

EFS_END
}
	
/// ���þ� �Է�
VOID CMNG_NEW_KEYWORD::OnRwordInsert() 
{
EFS_BEGIN
	
	CMNG_NEW_RELATED Dlg(this);
	Dlg.OpenMode = 0;
	Dlg.strSubjectKey  = m_strSubjectKey;
	Dlg.DoModal();

	//AllControlDisplay(_T("CM_����_�з���ȣ_������_�Է�"));

EFS_END
}

/// ���þ� ����
VOID CMNG_NEW_KEYWORD::OnRwordUpdate() 
{
EFS_BEGIN
	INT n_idx = -1;
	SelectMakeList(_T("CM_����_�з���ȣ_������_�Է�"), _T("RELATED_GRID"));
	n_idx = SelectGetHeadPosition(_T("CM_����_�з���ȣ_������_�Է�"), _T("RELATED_GRID"));

	if(n_idx < 0)
	{
		//����ó��
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	/*CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_�з���ȣ_������_�Է�"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return ;
	}

	CString strTitle, strCode, strSubjectKey, strRecKey;

	strRecKey = pDM->GetCellData(_T("REC_KEY"), n_idx);
	strTitle = pDM->GetCellData(_T("���þ�"), n_idx);
	strCode = pDM->GetCellData(_T("���þ��ڵ�"), n_idx);
	strSubjectKey = pDM->GetCellData(_T("������Ű"), n_idx);;
*/
	CMNG_NEW_RELATED Dlg(this);
	Dlg.OpenMode = 1;
//	Dlg.m_TITLE=strTitle;
//	Dlg.m_strCode=strCode;
//	Dlg.strSubjectKey  = strSubjectKey;
//	Dlg.m_RECKEY  = strRecKey;
	Dlg.DoModal();
	
	//AllControlDisplay(_T("CM_����_�з���ȣ_������_�Է�"));

EFS_END
}

/// ���þ� ����
VOID CMNG_NEW_KEYWORD::OnRwordDetele() 
{
EFS_BEGIN
	INT n_idx = -1;
	SelectMakeList(_T("CM_����_�з���ȣ_������_�Է�"), _T("RELATED_GRID"));
	n_idx = SelectGetHeadPosition(_T("CM_����_�з���ȣ_������_�Է�"), _T("RELATED_GRID"));

	if(n_idx < 0)
	{
		//����ó��
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CESL_DataMgr *pDM = FindDM(_T("DM_����_�з���ȣ_������_���þ�"));
	if ( NULL == pDM )
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����."));
		return ;
	}

	CESL_Grid* p_grid;
	p_grid = (CESL_Grid*)FindControl(_T("CM_����_�з���ȣ_������_�Է�"), _T("RELATED_GRID"));
	if ( p_grid == NULL )
	{
		AfxMessageBox(_T("GRID �ʱ�ȭ ����"));
		return ;
	}
	
	//FIND REC_KEY INDEX
	INT nSelectCount = 0;
	nSelectCount = p_grid->SelectGetCount();
	if ( nSelectCount <= 0 ) return ;

	CString strRecKey;
	INT ids=-1;

	for ( INT i=0; i<nSelectCount; i++ )
	{
		strRecKey = pDM->GetCellData(_T("REC_KEY"), n_idx);
		
		// Add Frame
		m_pDMFrame->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("STRING"), strRecKey);

		ids = p_grid->RemoveAt(n_idx);
		if ( ids<0 ) return;

		ids = pDM->DeleteRow(n_idx);
		if ( ids<0 ) return;

		// ������ ����ŭ �׸��� row ���� ������.
		n_idx = p_grid->SelectGetNext() - (i+1);

	}// end of for
	

EFS_END
}

BEGIN_EVENTSINK_MAP(CMNG_NEW_KEYWORD, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_NEW_KEYWORD)
//	ON_EVENT(CMNG_NEW_KEYWORD, IDC_REALATED_GRID, -600 /* Click */, OnClickRealatedGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


VOID CMNG_NEW_KEYWORD::OnCancel() 
{
EFS_BEGIN
	// End Frame
	// DM����
	if ( m_pDMFrame != NULL)
	{
		m_pDMFrame->EndFrame();
		delete m_pDMFrame;
		m_pDMFrame= NULL;
	}
	CDialog::OnCancel();

EFS_END
}

HBRUSH CMNG_NEW_KEYWORD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
