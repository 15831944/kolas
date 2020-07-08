// SE_CAT_REGNO_GIVE_LOST.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_REGNO_GIVE_LOST.h"
#include "SE_CAT_GROUP_INSERT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REGNO_GIVE_LOST dialog


CSE_CAT_REGNO_GIVE_LOST::CSE_CAT_REGNO_GIVE_LOST(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_nCount = -1;
	m_nSelected = -1;

	m_pRegNoArray = NULL;
}

CSE_CAT_REGNO_GIVE_LOST::~CSE_CAT_REGNO_GIVE_LOST()
{
}


VOID CSE_CAT_REGNO_GIVE_LOST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_REGNO_GIVE_LOST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_REGNO_GIVE_LOST, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_REGNO_GIVE_LOST)
	ON_BN_CLICKED(IDC_BTN_Search, OnBTNSearch)
	ON_BN_CLICKED(IDC_CHANGE_GROUP, OnChangeGroup)
	ON_BN_CLICKED(IDC_bINSERT_DROP_REG_NO, OnbINSERTDROPREGNO)
	ON_CBN_SELCHANGE(IDC_cREG_CODE_DROP, OnSelchangecREGCODEDROP)
	ON_CBN_SELCHANGE(IDC_cREG_Change_CLASS, OnSelchangecREGChangeCLASS)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REGNO_GIVE_LOST message handlers

BOOL CSE_CAT_REGNO_GIVE_LOST::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_������Ϲ�ȣ_�ο�")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(_T("CM_����_����_������Ϲ�ȣ"), _T("MAIN_GRID"));
    m_nCount = pGrid->SelectGetCount();
		

	m_nSelected = 0;
	
	CString strbuf;
	strbuf.Format(_T(" %d/%d "),m_nSelected,m_nCount);
/*
	for( INT i = 0 ; i < m_pRegNoArray->GetSize() ; i++ ) {
		CString sTmp = m_pRegNoArray->GetAt(i);
	}
	*/


//
	
	CString strRegGroup;
	INT row = pGrid->GetRow();
	INT icount=0;
	INT iRowCount = pGrid->GetCount();

	CESL_DataMgr *pTmpDM = FindDM(_T("DM_����_����DM_å����"));
	if ( NULL == pTmpDM ) 
	{
		AfxMessageBox ( _T("DM_����_����DM_å����") );
		return FALSE;
	}
	CString strRegCode;
	//������ �ڷ��� ���� üũ
	for (INT i = 1; i<=iRowCount; i++)
	{
		if (pGrid->GetAt(i-1,0) == _T("V"))	
			strRegCode = pTmpDM->GetCellData(_T("SB_��ϱ���_�ڵ�"), i-1);
	}

	SetControlData( _T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("��ϱ���"), strRegCode );
//


	GetDlgItem(IDC_EDT_COUNT)->SetWindowText(strbuf);

	// ���ι�ȣ ��������
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_������Ϲ�ȣ_�ο�"));
	if (!pDM) return FALSE;

	CString strQuery;
	m_strYear = CTime::GetCurrentTime().Format(_T("%Y"));
		
	strQuery.Format(_T("SELECT MAX(ACCESSION_REC_NO) FROM CO_ACCESSION_REC_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACCESSION_REC_MAKE_YEAR = %s"), m_strYear);
	
	CString strAccNo;
	pDM->GetOneValueQuery(strQuery, strAccNo);

	SetControlData(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("���ι�ȣ"), strAccNo);

	// ��Ϲ�ȣ �о����
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003.09.16 ������ ���� 
	//ó�� ȭ�鿡 �˻� ���� ��Ÿ���� �ʰ� 

	//Load_REGNO(_T(""));

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CAT_REGNO_GIVE_LOST::Load_REGNO(CString mQry)
{
	CString strQry;
	INT ids;

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_������Ϲ�ȣ_�ο�"));
	
	strQry=_T(" PUBLISH_FORM_CODE = 'MO' ");
	
	if( mQry != _T(""))
	{
		strQry += mQry;
	}

	ids = pDM->RefreshDataManager(strQry);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ���� �ּ���"));
		return;
	}

	ids = pDM->GetRowCount();
	if ( ids == 0 )
	{
		AfxMessageBox(_T("�˻��� ������Ϲ�ȣ�� �����ϴ�."));
		return;
	}

	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("REGNO_GRID"));
	pGrid->Display();	
}

BEGIN_EVENTSINK_MAP(CSE_CAT_REGNO_GIVE_LOST, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_CAT_REGNO_GIVE_LOST)
	ON_EVENT(CSE_CAT_REGNO_GIVE_LOST, IDC_GRID_REGNO, -600 /* Click */, OnClickGridRegno, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_CAT_REGNO_GIVE_LOST::OnClickGridRegno() 
{
	// TODO: Add your control notification handler code here
	
	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("REGNO_GRID"));
	if (pGrid == NULL) return;

	CString strbuf;
	
	INT nRow =  pGrid->GetRow();
	INT nCol =	pGrid->GetCol();

	// �����ɼ� �ִ��� üũ�Ѵ�.
	if (nCol == 1)
	{
		pGrid->GetAt(nRow-1, 0 ,strbuf);
		
		if (strbuf == _T("V") )
		{
			pGrid->SetAt(nRow-1, 0 , _T(""));

			m_nSelected--;
		}
		else
		{			
			m_nSelected++;

			// ���� üũ
			if (m_nSelected > m_nCount)
			{
				AfxMessageBox(_T("���� �Ͻ� å �� ���� ���� ���� ��Ϲ�ȣ�� ���� �� �� �����ϴ�."));
				m_nSelected--;
				return;
			}
			pGrid->SetAt(nRow-1, 0 , _T("V"));
		}

		strbuf.Format(_T(" %d/%d "),m_nSelected,m_nCount);
		GetDlgItem(IDC_EDT_COUNT)->SetWindowText(strbuf);
	}
}

VOID CSE_CAT_REGNO_GIVE_LOST::OnBTNSearch() 
{
	CString sRegCode,sRegNO_s,sRegNO_e,strBuf,strQry;

	GetControlData(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("��ϱ���"), sRegCode);
	GetControlData(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("��Ϲ�ȣ����"), sRegNO_s);
	GetControlData(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("��Ϲ�ȣ��"), sRegNO_e);

	sRegCode.TrimLeft();
	sRegCode.TrimRight();

	sRegNO_s.TrimLeft();
	sRegNO_s.TrimRight();
	
	sRegNO_e.TrimLeft();
	sRegNO_e.TrimRight();

	if ( sRegCode != _T("") )
	{
		strBuf.Format(_T(" AND REG_CODE = '%s' "),sRegCode); 
		strQry += strBuf;  
	}

	if ( sRegNO_s != _T("") && sRegNO_e != _T("") )
	{
		strBuf.Format(_T(" AND REG_NO >= %s AND REG_NO <= %s "),sRegNO_s,sRegNO_e); 
		strQry += strBuf;  
	}

	if ( sRegNO_s != _T("") && sRegNO_e == _T("") )
	{
		strBuf.Format(_T(" AND REG_NO >= %s "),sRegNO_s); 
		strQry += strBuf;  
	}

	if ( sRegNO_s == _T("") && sRegNO_e != _T("") )
	{
		strBuf.Format(_T(" AND REG_NO <= %s "),sRegNO_e); 
		strQry += strBuf;  
	}

	Load_REGNO(strQry);
	
}

VOID CSE_CAT_REGNO_GIVE_LOST::OnOK() 
{
	INT idx,Book_idx;
	SelectMakeList(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("REGNO_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("REGNO_GRID"));

	if (idx < 0)
	{
		//����ó��
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_������Ϲ�ȣ_�ο�"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_������Ϲ�ȣ_�ο�") );
		return ;
	}

	CESL_DataMgr *pBookDM = FindDM(_T("DM_����_����DM_å����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����DM_å����") );
		return ;
	}
	
	Book_idx = SelectGetHeadPosition(_T("CM_����_����_������Ϲ�ȣ"), _T("MAIN_GRID"));

	CString strRegNo,strRegCode,strReg,strRecKey,strQry,ACC_KEY;
	// ���� ��ȣ�� ���Ѵ�. 
	GetControlData( _T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("���ι�ȣ"), ACC_KEY );
	ACC_KEY.TrimLeft();
	ACC_KEY.TrimRight();
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2003.10.09 ������ ���� 
	//���ι�ȣ�� ������� ACC_KEY = NULL �� �Է��Ѵ�.
    /*
	if ( ACC_KEY==_T(""))
	{
		AfxMessageBox ( _T("���� ��ȣ�� �����ϴ�!!!") );
		return ;		
	}

	if( m_strYear == _T(""))
	{
		AfxMessageBox ( _T("�ٽ� ���θ� ���� �� �ֽʽÿ�!!!") );
		return ;		
	}

    */
	if(!ACC_KEY.IsEmpty () && !m_strYear.IsEmpty ())
	{
		strQry = _T("SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACCESSION_REC_NO =")+ACC_KEY+_T("AND ACCESSION_REC_MAKE_YEAR =")+m_strYear;	
		pDM->GetOneValueQuery(strQry,ACC_KEY);
	}
	else
	{
		ACC_KEY = _T("") ;
	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	//��� ��ȣ�� �־� �ذ� �������� ���� �Ѵ�.
	strRegNo = pDM->GetCellData( _T("��Ϲ�ȣ") , idx );
	strRegCode = pDM->GetCellData( _T("��ϱ���") , idx );
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	strReg.Format(_T("%s%010d"),strRegCode,_ttoi(strRegNo.GetBuffer(0)));
	
	strReg = pParentMgr->m_pInfo->MakeRegNo(strRegCode,strRegNo);
	//=====================================================
	

	pBookDM->SetCellData(_T("SB_��Ϲ�ȣ"),strReg,Book_idx);
	pBookDM->SetCellData(_T("SB_����KEY"),ACC_KEY,Book_idx);

	//�������� ���� 
	m_pRegNoArray->Add(strReg);
	
	INT ids = pDM->StartFrame();
	if ( 0 > ids ) return;

	strRecKey = pDM->GetCellData( _T("REC_KEY") , idx );
	strQry.Format(_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE REC_KEY = %s ;"),strRecKey);
	ids = pDM->AddFrame( strQry );
	if ( 0 > ids ) return;
	
	idx = SelectGetNext(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("REGNO_GRID"));
	Book_idx = SelectGetNext(_T("CM_����_����_������Ϲ�ȣ"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("��Ϲ�ȣ") , idx );
		strRegCode = pDM->GetCellData( _T("��ϱ���") , idx );
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		strReg.Format(_T("%s%010d"),strRegCode,_ttoi(strRegNo.GetBuffer(0)));
		//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
		strReg = pParentMgr->m_pInfo->MakeRegNo(strRegCode,strRegNo);
		//=====================================================
		
		pBookDM->SetCellData(_T("SB_��Ϲ�ȣ"),strReg,Book_idx);
		pBookDM->SetCellData(_T("SB_����KEY"),ACC_KEY,Book_idx);
		//�������� ���� 
		m_pRegNoArray->Add(strReg);

		strRecKey = pDM->GetCellData( _T("REC_KEY") , idx );
		strQry.Format(_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE REC_KEY = %s ;"),strRecKey);
		pDM->AddFrame( strQry );
		//
		idx = SelectGetNext(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("REGNO_GRID"));
		Book_idx = SelectGetNext(_T("CM_����_����_������Ϲ�ȣ"), _T("MAIN_GRID"));
	}


	ids = pDM->SendFrame();
	if ( 0 > ids ) return;	
	ids = pDM->EndFrame();
	if ( 0 > ids ) return;

	CDialog::OnOK();
}

VOID CSE_CAT_REGNO_GIVE_LOST::OnChangeGroup() 
{
	CSE_CAT_GROUP_INSERT Dlg(this);
	Dlg.DoModal();
	
	m_strYear = Dlg.m_strYear;

}

VOID CSE_CAT_REGNO_GIVE_LOST::OnbINSERTDROPREGNO() 
{
	INT ids = -1;
	CString strRegCode;
	CString strRegNo;
	CString strQuery;
	CString strResult;

	GetControlData(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("�Է�_��ϱ���"), strRegCode);
	GetControlData(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("�Է�_��Ϲ�ȣ"), strRegNo);
	

	strRegCode.TrimLeft();
	strRegCode.TrimRight();

	strRegNo.TrimLeft();
	strRegNo.TrimRight();
	
	if (strRegCode.IsEmpty())
	{
		AfxMessageBox(_T("��ϱ����� �����ϼ���"));
		return;
	}

	if (strRegNo.IsEmpty())
	{
		AfxMessageBox(_T("��Ϲ�ȣ�� �Է��ϼ���"));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_������Ϲ�ȣ_�ο�"));
	if (!pDM) return;
	// ������ ��ȣ���� ū�� �˻�
	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '%s'")
				    ,strRegCode);
	ids = pDM->GetOneValueQuery(strQuery, strResult);
	if (ids < 0 || _ttoi(strRegNo) > _ttoi(strResult))
	{
		AfxMessageBox(_T("�Է��Ͻ� ��Ϲ�ȣ�� ������ ��ȣ���� Ů�ϴ�.."));
		return;
	}

	// �ߺ� �˻� 1. å TABLE���� 2. ������Ϲ�ȣ TABLE����
	CString strRegNoField;
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	strRegNoField.Format(_T("%s%010s"), strRegCode, strRegNo);
	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	strRegNoField = pParentMgr->m_pInfo->MakeRegNo(strRegCode,strRegNo);
	//=====================================================
	
	strQuery.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_NO = '%s'"), strRegNoField);
	ids = pDM->GetOneValueQuery(strQuery, strResult);
	if (ids < 0 || strResult != _T("0"))
	{
		AfxMessageBox(_T("�Է��Ͻ� ��Ϲ�ȣ�� �ߺ��� ��ȣ �Դϴ�."));
		return;
	}

	strQuery.Format(_T("SELECT COUNT(*) FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND PUBLISH_FORM_CODE = 'MO' AND REG_CODE = '%s' AND REG_NO = %d")
					, strRegCode, _ttoi(strRegNo));
	ids = pDM->GetOneValueQuery(strQuery, strResult);
	if (ids < 0 || strResult != _T("0"))
	{
		AfxMessageBox(_T("�Է��Ͻ� ��Ϲ�ȣ�� �ߺ��� ��ȣ �Դϴ�."));
		return;
	}

	// DB�� Insert�Ѵ�.
	CString strRecKey;
	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return;
	}

	pDM->MakeRecKey(strRecKey);
	
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strRecKey);
	pDM->AddDBFieldData(_T("REG_CODE"), _T("STRING"), strRegCode);
	pDM->AddDBFieldData(_T("REG_NO"), _T("NUMERIC"), strRegNo);
	pDM->AddDBFieldData(_T("PUBLISH_FORM_CODE"), _T("STRING"), _T("MO"));
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"));

	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG));
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG));


	ids = pDM->MakeInsertFrame(_T("CO_DROP_ACCESSION_NO_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame �� Ȯ���ϼ���"));
		return;
	}
	
	ids = pDM->SendFrame();
	if (ids < 0)
	{
		AfxMessageBox(_T("DB���� �۾��� �����߽��ϴ�."));
		return;
	}

	ids = pDM->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame �� Ȯ���ϼ���"));
		return;
	}

	// DM�� ������ Insert�Ѵ�.
	pDM->InsertRow(-1);
	INT nRow = pDM->GetRowCount() - 1;
	pDM->SetCellData(_T("REC_KEY"), strRecKey, nRow);
	pDM->SetCellData(_T("��ϱ���"), strRegCode, nRow);
	pDM->SetCellData(_T("��Ϲ�ȣ"), strRegNo, nRow);


	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("REGNO_GRID"));
	pGrid->DisplayLine(nRow);
}

VOID CSE_CAT_REGNO_GIVE_LOST::OnSelchangecREGCODEDROP() 
{
	// TODO: Add your control notification handler code here
	CString strSelRegCode;
	CString strRegCode;

	CESL_DataMgr *pTmpDM = FindDM(_T("DM_����_����DM_å����"));
	if ( NULL == pTmpDM ) 
	{
		AfxMessageBox ( _T("DM_����_����DM_å����") );
		return ;
	}

	//������ ��Ϲ�ȣ
	strRegCode = pTmpDM->GetCellData(_T("SB_��ϱ���_�ڵ�"), 0);

	//��Ƽ��� ���õ� �׸�
	GetControlData( _T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("�Է�_��ϱ���"), strSelRegCode );
	

	if ( strSelRegCode != strRegCode )
	{
		AfxMessageBox(_T("��ϱ����� Ȯ���� �ּ���"));
		SetControlData( _T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("�Է�_��ϱ���"), strRegCode );
		return;
	}
	
}

VOID CSE_CAT_REGNO_GIVE_LOST::OnSelchangecREGChangeCLASS() 
{
	// TODO: Add your control notification handler code here
	CString strSelRegCode;
	CString strRegCode;

	CESL_DataMgr *pTmpDM = FindDM(_T("DM_����_����DM_å����"));
	if ( NULL == pTmpDM ) 
	{
		AfxMessageBox ( _T("DM_����_����DM_å����") );
		return ;
	}

	//������ ��Ϲ�ȣ
	strRegCode = pTmpDM->GetCellData(_T("SB_��ϱ���_�ڵ�"), 0);

	//��Ƽ��� ���õ� �׸�
	GetControlData( _T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("��ϱ���"), strSelRegCode );
	

	if ( strSelRegCode != strRegCode )
	{
		AfxMessageBox(_T("��ϱ����� Ȯ���� �ּ���"));
		SetControlData( _T("CM_����_����_������Ϲ�ȣ_�ο�"), _T("��ϱ���"), strRegCode );
		return;
	}
	
}

HBRUSH CSE_CAT_REGNO_GIVE_LOST::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
