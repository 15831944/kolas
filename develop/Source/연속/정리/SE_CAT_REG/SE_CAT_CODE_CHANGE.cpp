// SE_CAT_CODE_CHANGE.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_CODE_CHANGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_CODE_CHANGE dialog


CSE_CAT_CODE_CHANGE::CSE_CAT_CODE_CHANGE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_CODE_CHANGE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CAT_CODE_CHANGE::~CSE_CAT_CODE_CHANGE()
{
}

VOID CSE_CAT_CODE_CHANGE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_CODE_CHANGE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_CODE_CHANGE, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_CODE_CHANGE)
	ON_BN_CLICKED(IDC_bCHANGE_CODE, OnbCHANGECODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_CODE_CHANGE message handlers

BOOL CSE_CAT_CODE_CHANGE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_���_�ڵ�")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	INT idx;

	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		EndDialog(IDCANCEL);
		return FALSE;
	}


	// 040304_HSS ��Ϲ�ȣ �ִ� �ڷᰡ ���õ� ��� �޼��� �����ֱ� ++++++++++++++++++
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_���� Error") );
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CString strRegNo;
	CString sRegCode = _T("");
	CString sSelfCode = _T("");
	CString sManageCode = _T("");
	// 040402_HSS ��Ϲ�ȣ �ִ� �ڷ�� ��ϱ��и� ���� ����
	/*
	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
		// 040324_HSS �⺻���� ���� - ���� ���� �ִ� ���� ++++++++++++++++++
		if (sRegCode.GetLength()==0){
			sRegCode = pDM->GetCellData( _T("BB_��ϱ���_�ڵ�") , idx );
			SetControlData(_T("CM_����_����_���_�ڵ�"), _T("BB_��ϱ���"), sRegCode);
		}
		if (sSelfCode.GetLength()==0){
			sSelfCode = pDM->GetCellData( _T("BB_��ġ����_�ڵ�") , idx );
			SetControlData(_T("CM_����_����_���_�ڵ�"), _T("BB_��ġ����"), sSelfCode);
		}
		if (sManageCode.GetLength()==0){
			sManageCode = pDM->GetCellData( _T("BB_��������_�ڵ�") , idx );
			SetControlData(_T("CM_����_����_���_�ڵ�"), _T("BB_��������"), sManageCode);
		}
		// +++++++++++++++++++++++++++++++++++++++++++

		if (strRegNo.GetLength()>0){
			AfxMessageBox(_T("��Ϲ�ȣ�� �ο��� �ڷᰡ ���õǾ����ϴ�.\n")
						_T("��Ϲ�ȣ�� �ο��� �ڷ�� ��ϱ����� ������ �� �����ϴ�.\n")
						_T("��ϱ����� ������ ���Ͻø� ��ȣ��� �� �����Ͽ� �ֽñ� �ٶ��ϴ�."));
			// 040402_HSS ��Ϲ�ȣ �ִ� �ڷ�� ��ϱ��� ���� ����
			//EndDialog(IDCANCEL);
			//return;
			break;
		}
		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}
	*/
	// �׳� ù��° �ڷ��� �ڵ带 ����
	sRegCode = pDM->GetCellData( _T("BB_��ϱ���_�ڵ�") , idx );
	SetControlData(_T("CM_����_����_���_�ڵ�"), _T("BB_��ϱ���"), sRegCode);
	sSelfCode = pDM->GetCellData( _T("BB_��ġ����_�ڵ�") , idx );
	SetControlData(_T("CM_����_����_���_�ڵ�"), _T("BB_��ġ����"), sSelfCode);
	sManageCode = pDM->GetCellData( _T("BB_��������_�ڵ�") , idx );
	SetControlData(_T("CM_����_����_���_�ڵ�"), _T("BB_��������"), sManageCode);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// 040324_HSS ��ġ, �������� �߰�
VOID CSE_CAT_CODE_CHANGE::OnbCHANGECODE() 
{
EFS_BEGIN

	INT idx;
	CString strBookKey,strRegNo,strSpeciesKey,strQuery,sTmp;
	CString sRegCode, sSelfCode, sManageCode;
	
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_����_���_����"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_����_����_���_���� error") );
		return ;
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return ;
	}

	// ������ �ڵ� �������� 
	GetControlData(_T("CM_����_����_���_�ڵ�"), _T("BB_��ϱ���"), sRegCode);
	GetControlData(_T("CM_����_����_���_�ڵ�"), _T("BB_��ġ����"), sSelfCode);
	GetControlData(_T("CM_����_����_���_�ڵ�"), _T("BB_��������"), sManageCode);

	// ��� åŰ ����Ʈ �����
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	// ���� spl ����
	pDM->StartFrame();

	BOOL bSendMsg = FALSE;
	while ( idx != -1 ) {
		strBookKey = pDM->GetCellData( _T("BB_åKEY") , idx );
		// 040402_HSS ��Ϲ�ȣ �ִ� �ڷ�� ��ϱ��� ���� ����
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
		if (strRegNo.GetLength()>0 && !bSendMsg)
		{
			AfxMessageBox(_T("���õ� �ڷ� �� ��Ϲ�ȣ�� �ο��� �ڷ��ֽ��ϴ�.\n")
						_T("��Ϲ�ȣ�� �ο��� �ڷ��� ��ϱ����� ������ �� �����ϴ�.\n")
						_T("��ϱ����� ������ ���Ͻø� ��ȣ��� �� �����Ͽ� �ֽñ� �ٶ��ϴ�."));
			bSendMsg = TRUE;
		}

		pDM->InitDBFieldData();
		if (strRegNo.GetLength()<=0)
			pDM->AddDBFieldData(_T("REG_CODE"), _T("STRING"), sRegCode );
		pDM->AddDBFieldData(_T("SEPARATE_SHELF_CODE"), _T("STRING"), sSelfCode );
		pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), sManageCode );
		pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(WORK_LOG) );
		pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), strBookKey, idx);
		
		if (strRegNo.GetLength()<=0)
			pDM->SetCellData(_T("BB_��ϱ���_�ڵ�"), sRegCode, idx);
		pDM->SetCellData(_T("BB_��ġ����_�ڵ�"), sSelfCode, idx);
		pDM->SetCellData(_T("BB_��������_�ڵ�"), sManageCode, idx);

		// 040305_HSS ���κ����߰� ++++++++++++++++++++++++++++++
		// 040402_HSS ������ ���� �������� ���̸� ������ ������Ʈ ������ ��� ���� ����
		CString strRegCode, strSeparateCode, strManageCode;
		strSpeciesKey = pDM->GetCellData( _T("IBS_��KEY") , idx );
		strRegCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'REG_CODE') FROM DUAL)"),strSpeciesKey);
		strSeparateCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'SEPARATE_SHELF_CODE') FROM DUAL)"),strSpeciesKey);
		strManageCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'MANAGE_CODE') FROM DUAL)"),strSpeciesKey);
		//=========================================================
		//2008.10.17 UPDATE BY PJW : MANAGE_CODE �� ����Ʈ�� ����.
//		strQuery.Format(_T("UPDATE IDX_SE_TBL SET REG_CODE = %s, IDX_SE_SHELF_CODE = %s, MANAGE_CODE = %s WHERE REC_KEY = %s;"),
//					strRegCode, strSeparateCode, strManageCode, strSpeciesKey);
		strQuery.Format(_T("UPDATE IDX_SE_TBL SET REG_CODE = %s, IDX_SE_SHELF_CODE = %s, MANAGE_CODE = UDF_MANAGE_CODE WHERE REC_KEY = %s;"),
					strRegCode, strSeparateCode,  strSpeciesKey);
		pDM->AddFrame( strQuery );
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++

		pGrid->DisplayLine(idx);
		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}
	INT ids = pDM->SendFrame();
	if (ids<0){
		AfxMessageBox(_T("������Ʈ�� �����Ͽ����ϴ�."));
	}
	else 
	{
		AfxMessageBox(_T("���� �Ǿ����ϴ�."));
		ids = pDM->EndFrame();
	}

	pGrid->ReshowSelectedRows();

	return ;	
EFS_END
}

HBRUSH CSE_CAT_CODE_CHANGE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
