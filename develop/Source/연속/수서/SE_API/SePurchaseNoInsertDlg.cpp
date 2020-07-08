// SePurchaseNoInsertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseNoInsertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseNoInsertDlg dialog


CSePurchaseNoInsertDlg::CSePurchaseNoInsertDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchaseNoInsertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseNoInsertDlg)
	m_bUSE_WORK_NO = FALSE;
	//}}AFX_DATA_INIT

	m_strYEAR = _T("");
	m_pDM = NULL;
	m_pDM_WORK = NULL;
	m_pCM = NULL;

}

CSePurchaseNoInsertDlg::~CSePurchaseNoInsertDlg()
{
}

VOID CSePurchaseNoInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseNoInsertDlg)
	DDX_Check(pDX, IDC_chkPUR_NO_INSERT_USE_WORKNO, m_bUSE_WORK_NO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseNoInsertDlg, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseNoInsertDlg)
	ON_BN_CLICKED(IDC_chkPUR_NO_INSERT_USE_WORKNO, OnchkPURNOINSERTUSEWORKNO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseNoInsertDlg message handlers

BOOL CSePurchaseNoInsertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("����_����_������ȣ_�Է�"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_������ȣ_�Է�") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_������ȣ_�Է�"));
	m_pDM = FindDM(_T("DM_����_������ȣ_�Է�"));
	m_pDM_WORK = FindDM(_T("DM_����_������ȣ_�Է�_�۾���ȣ"));

	if(!m_pDM || !m_pCM || !m_pDM_WORK) {
		AfxMessageBox( _T("DM,CM Error : ����_����_������ȣ_�Է�") );
		return FALSE;
	}	
	
	if(m_strYEAR.IsEmpty()) {
		AfxMessageBox( _T("�����⵵�� �Էµ��� �ʾҽ��ϴ�!") );
		return FALSE;
	}	


	m_bUSE_WORK_NO = TRUE;
	
	CString strPurchaseNo;
	strPurchaseNo = GetPurchaseNo(m_pDM, m_strYEAR);
	m_pCM->SetControlMgrData(_T("�����⵵"),m_strYEAR);
	m_pCM->SetControlMgrData(_T("������ȣ"),strPurchaseNo);
	m_pCM->SetControlMgrData(_T("�۾�����"),_T("���Դ��"));
	m_pCM->SetControlMgrData(_T("������"),m_pInfo->USER_ID);
	m_pCM->SetControlMgrData(_T("�������۾���ȣ"),_T("0"));


	UpdateData(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CSePurchaseNoInsertDlg::GetPurchaseNo(CESL_DataMgr* pDM, CString strYear)
{
	pDM->StartFrame();
	
	//�ش��
	CString strLastNumber,strQuery,strPurchaseNo;
	
	strQuery.Format(_T("LOCK TABLE CO_LAST_NUMBER_TBL IN EXCLUSIVE MODE;"));
	pDM->ExecuteQuery(strQuery,0,FALSE);
	

	//�ش�⵵�� �ش� ������ȣ�� SELECT�ϰ� ���� ���� INSERT�Ѵ� 
	//++2008.10.17 UPDATE BY PWR {{++
	// ���սý��� ����
	strQuery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					 _T("WHERE KIND_CODE='PUR_NO' AND PUBLISH_FORM_CODE='SE' AND MAKE_YEAR='%s' ")
					 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strYear );
	// strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'PUR_NO' AND PUBLISH_FORM_CODE = 'SE' AND MAKE_YEAR = '%s'"),strYear);
	//--2008.10.17 UPDATE BY PWR --}}
	INT ids =  pDM->GetOneValueQuery(strQuery,strLastNumber,FALSE);

	INT nPurchaseNo = 0;
	if(!strLastNumber.IsEmpty())
	{
		nPurchaseNo = _ttoi(strLastNumber.GetBuffer(0));
	}

	nPurchaseNo++;

	if( -4007 == ids)	//�˻������ ������ INSERT 
	{
		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR,DIVISION_VALUE,LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK,MANAGE_CODE)")
						_T("VALUES	(%s.NEXTVAL,'PUR_NO','SE','%s','����_������ȣ',1,'%s','%s','%s',UDF_MANAGE_CODE);"), DEFAULT_SEQ_NAME, strYear,GetTodayDate(),m_pInfo->USER_ID,GetWorkLogMsg(WORK_LOG_SEPURCHASENO));
		pDM->ExecuteQuery(strQuery,1);
		nPurchaseNo = 1;	
	
	}
	// �ʱ�ȭ�� ���������̺� ���� ������Ʈ �ϹǷ�
	// ������ �������� �ʰ� ����Ͽ��� ��쿡�� ������ ���� �ö�
	// �ش� �κ� �ּ�ó��
	// ������ ��ȣ ������Ʈ ������ OnOK()���� ����.
	// 2005-09-30 ������
	//
	/*
	else	
	{
		//������ UPDATE
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %d WHERE PUBLISH_FORM_CODE = 'SE' AND MAKE_YEAR = '%s' AND KIND_CODE = 'PUR_NO';"),nPurchaseNo,strYear);
		pDM->ExecuteQuery(strQuery,1);
	}
	*/

	pDM->EndFrame();
	strPurchaseNo.Format(_T("%d"),nPurchaseNo);
	
	return strPurchaseNo;
}

VOID CSePurchaseNoInsertDlg::OnOK() 
{	
	//save
	SaveAllDataToDM();
	
	//������ȣ
	CString strPurchaseNo;
	INT nPurchaseNo = 0;
	m_pCM->GetControlMgrData(_T("������ȣ"),strPurchaseNo);
	
	if(!strPurchaseNo.IsEmpty())
		nPurchaseNo = _ttoi(strPurchaseNo.GetBuffer(0));
	
	
	//�����⵵
	CString strYear;
	m_pCM->GetControlMgrData(_T("�����⵵"),strYear);

	//frame
	CArray<CString,CString> RemoveAliasList;
	
	m_pDM->StartFrame();

	// ������ȣ 
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_�۾�����"));
	m_pDM->InitDBFieldData();
	m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("SE_PURCHASE_SEQ_NO_TBL"), 0 , m_pDM);
	
	//�۾� ��ȣ 
	RemoveAliasList.RemoveAll();
	m_pDM->InitDBFieldData();
	m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM_WORK, _T("SE_PURCHASE_WORK_NO_TBL"), 0 , m_pDM);
	
	//������ ���� ��ȣ 
	if(nPurchaseNo>0)
	{
		CString strQuery;
		//++2008.10.17 UPDATE BY PWR {{++
		// ���սý��� ����
		strQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
						 _T("WHERE PUBLISH_FORM_CODE='SE' AND MAKE_YEAR='%s' AND KIND_CODE='PUR_NO' ")
						 _T("AND MANAGE_CODE=UDF_MANAGE_CODE;"), nPurchaseNo, strYear );
		// strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %d WHERE PUBLISH_FORM_CODE = 'SE' AND MAKE_YEAR = '%s' AND KIND_CODE = 'PUR_NO';"),nPurchaseNo,strYear);
		//--2008.10.17 UPDATE BY PWR --}}
		m_pDM->AddFrame(strQuery);
	}
	

	INT ids = 0;
	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	
	if(ids<0)
	{
		AfxMessageBox( _T("�Է¿� �����Ͽ����ϴ�!") );
		return;
	} 
	else 
	{
		AfxMessageBox( _T("�Է��Ͽ����ϴ�!") );
	}


	CDialog::OnOK();
}

VOID CSePurchaseNoInsertDlg::SaveAllDataToDM()
{
	m_pDM->FreeData();
	m_pDM->InsertRow(-1);

	
	const INT cnt = 6;
	CString strAlias[cnt] = {
		_T("�����⵵"),
		_T("������ȣ"),
		_T("������"),	
		_T("������"),
		_T("�����⵵"),
		_T("�������۾���ȣ"),
	};
	
	CString strREC_KEY = _T("");
	m_pDM->MakeRecKey(strREC_KEY);
	m_pDM->SetCellData(_T("REC_KEY"),strREC_KEY,0);

	CString strData;
	for(INT i=0;i<cnt;i++)
	{
		strData = _T("");
		m_pCM->GetControlMgrData(strAlias[i],strData);
		m_pDM->SetCellData(strAlias[i],strData,0);
	}
	
	CString strWorkLogMsg = _T("");
	strWorkLogMsg = GetWorkLogMsg(WORK_LOG_SEPURCHASENO);
	m_pDM->SetCellData(_T("�����۾�"),strWorkLogMsg,0);
	m_pDM->SetCellData(_T("�������۾�"),strWorkLogMsg,0);
	m_pDM->SetCellData(_T("�۾�����"),_T("0"),0); //���Դ��
	
	
	CString strWorkNo = _T("");
	m_pCM->GetControlMgrData(_T("�������۾���ȣ"),strWorkNo);
	m_pDM_WORK->FreeData();
	

	//�۾���ȣ ���� ���̺� INSERT
	m_pDM_WORK->InsertRow(-1);
		
	CString strWorkKey;
	m_pDM_WORK->MakeRecKey(strWorkKey);
	m_pDM_WORK->SetCellData(_T("REC_KEY"),strWorkKey,0);
	m_pDM_WORK->SetCellData(_T("��������KEY"),strREC_KEY,0);

	CString tmp = _T("");
	m_pCM->GetControlMgrData(_T("������"),tmp);
	m_pDM_WORK->SetCellData(_T("������"),tmp,0);
		
	tmp = _T("");
	m_pCM->GetControlMgrData(_T("������"),tmp);
	m_pDM_WORK->SetCellData(_T("������"),tmp,0);
		
	m_pDM_WORK->SetCellData(_T("�������Ϸù�ȣ"),_T("0"),0);
	m_pDM_WORK->SetCellData(_T("�����۾�"),strWorkLogMsg,0);
	m_pDM_WORK->SetCellData(_T("�������۾�"),strWorkLogMsg,0);
	m_pDM_WORK->SetCellData(_T("�۾���ȣ"),strWorkNo,0);


}

VOID CSePurchaseNoInsertDlg::OnCancel() 
{
	CDialog::OnCancel();
}

VOID CSePurchaseNoInsertDlg::OnchkPURNOINSERTUSEWORKNO() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_bUSE_WORK_NO)
	{
		m_pCM->SetControlMgrData(_T("�������۾���ȣ"),_T("0"));
	}
	else
	{
		m_pCM->SetControlMgrData(_T("�������۾���ȣ"),_T("1"));
	}

}

CString CSePurchaseNoInsertDlg::GetTodayDate()
{
	CString result;
	COleDateTime t = COleDateTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}
HBRUSH CSePurchaseNoInsertDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
