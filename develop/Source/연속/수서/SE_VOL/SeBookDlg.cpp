// SeBookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeBookDlg.h"
#include "..\SE_API\CheckinReceipt.h"
#include "..\SE_API\SeApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeBookDlg dialog


CSeBookDlg::CSeBookDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeBookDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeBookDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_nCurrentIdx = -1;
	m_nACQ_TYPE = 1; //default
	m_bIsModifyStatus = FALSE;
	

	m_strACQ_VOL_KEY = _T("");
	m_strVOL_TITLE = _T(""); 
	m_strTITLE_INFO = _T(""); 
	m_strAUTHOR_INFO = _T("");
	m_strPUB_INFO = _T(""); 

	m_bIsLightVersion = FALSE;
	m_bIsDeleteProc = FALSE;
}

CSeBookDlg::~CSeBookDlg()
{
}

VOID CSeBookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeBookDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeBookDlg, CDialog)
	//{{AFX_MSG_MAP(CSeBookDlg)
	ON_BN_CLICKED(IDC_btnBOOK_RECEIPT, OnbtnBOOKRECEIPT)
	ON_BN_CLICKED(IDC_btnLINE_MODFY, OnbtnLINEMODFY)
	ON_BN_CLICKED(IDC_btnBOOK_DELETE, OnbtnBOOKDELETE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeBookDlg message handlers

BOOL CSeBookDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( InitESL_Mgr(_T("����_����_å")) < 0 ) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_å") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_����_å"));
	m_pDM = FindDM(_T("DM_����_å"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���")); 

	if(!m_pCM || !m_pDM || !m_pGrid) {
		AfxMessageBox( _T("CM,DM,Gird Error : ����_����_å") );
		return FALSE;
	}

	m_pCM->SetControlMgrData(_T("��ȣ��"),m_strVOL_TITLE);
	
	if(m_strACQ_VOL_KEY.IsEmpty())
	{
		AfxMessageBox(_T("���Ա��к���KEY �� ã�� �� �����ϴ�!"));
		return FALSE;
	}
	

	//�ڷ� ���¸� ���� ���ش�.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("�ǽð��ΰ��ڷ�") );
		pCombo->InsertString( 1, _T("�����ΰ��ڷ�") );
		pCombo->InsertString( 2, _T("����ΰ��ڷ�") );
		pCombo->InsertString( 3, _T("��Ÿ�ڷ����") );
		pCombo->SetCurSel( 0 );
	}

	//Data Selection
	RefreshDM();
			
	//����Ʈ ���� setting 
	if(m_bIsLightVersion)
	{
		SetLightVersion();
	}

	m_pGrid->Display();
	ShowAllData();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSeBookDlg::SetLightVersion()
{
	//����Ʈ ������ �ڷ������ ���� ��Ʈ���� ���ְ� �Լ��� ��Ʈ���� ��ġ�� ���ġ �Ѵ� 
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_STATIC_BOOK_WORKING_STATUS);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	CRect rect;
	
	pWnd = GetDlgItem(IDC_STATIC_BOOK_ACQ_DATE);
	if(pWnd)
	{
		rect.left = 280;
		rect.right = 330;
		rect.top = 230;
		rect.bottom = 250;

		pWnd->MoveWindow(rect);
	}
	
	pWnd = GetDlgItem(IDC_edtBOOK_ACQ_DATE);
	if(pWnd)
	{
		rect.left = 335;
		rect.right = 520;
		rect.top = 225;
		rect.bottom = 247;		
		
		pWnd->MoveWindow(rect);
	}
	
}

BEGIN_EVENTSINK_MAP(CSeBookDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeBookDlg)
	ON_EVENT(CSeBookDlg, IDC_gridBOOK, -600 /* Click */, OnClickgridBOOK, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeBookDlg::OnClickgridBOOK() 
{
	// TODO: Add your control notification handler code here

	
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		m_nCurrentIdx = row-1;
		m_pGrid->SetReverse(row-1);
		ShowAllData();
		
		// color refresh
		RefreshGridColor();
	}
	
}

INT CSeBookDlg::RefreshDM()
{
	CString strWhere;
	strWhere.Format(_T("BOOK_TYPE = 'C' AND ACQ_TYPE_VOL_KEY = %s"),m_strACQ_VOL_KEY);
	m_pDM->RefreshDataManager(strWhere);
	m_nCurrentIdx = 0;
	m_pGrid->SetReverse(m_nCurrentIdx);
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("�ڷ�Ǳ���"),_T("UDF_�ڷ��"));
	
	//�ڷ� ���� ó��, RFID ����
	CString strCode,strDesc,strRFIDSerial;
	CSeApi api(this);

	for(INT i=0;i<m_pDM->GetRowCount();i++)
	{
		strCode = _T("");
		strDesc = _T("");
		strRFIDSerial = _T("");

		strCode = m_pDM->GetCellData(_T("SB_�ڷ����"),i);
		strDesc = api.GetWorkingStatusDesc(strCode);
		m_pDM->SetCellData(_T("UDF_�ڷ����"),strDesc,i);


		strRFIDSerial = m_pDM->GetCellData(_T("SB_RFID"),i);
		strRFIDSerial.TrimLeft();
		strRFIDSerial.TrimRight();
		
		if(strRFIDSerial.IsEmpty())
		{
			m_pDM->SetCellData(_T("UDF_RFID"),_T("N"),i);
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_RFID"),_T("Y"),i);
		}
	}

	return 0;
}

INT CSeBookDlg::ShowAllData()
{
	//�׸��� ���� 
	m_pCM->ControlDisplay(_T("�Լ���"),m_nCurrentIdx);
	m_pCM->ControlDisplay(_T("���"),m_nCurrentIdx);
	m_pCM->ControlDisplay(_T("�ڷ��"),m_nCurrentIdx);


	//�ڷ���°� �ǽð��ΰ�-�����ΰ�-����ΰ��� �ϳ��� �ڷ���� �޺��� �����Ϳ� ���߾� �ش�
	//�� �ܿ��� ��Ȱ��ȭ 
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
	if(!pCombo) return -1;

	CString strWorkingStatus;
	strWorkingStatus = m_pDM->GetCellData(_T("SB_�ڷ����"),m_nCurrentIdx);
	
	if(_T("SEL111O") == strWorkingStatus) //�ǽð�
	{
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);
		m_bIsModifyStatus = TRUE;

	}
	else if(_T("SEB111N") == strWorkingStatus) //���� 
	{
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(1);
		m_bIsModifyStatus = TRUE;
	}
	else if(_T("SER111N") == strWorkingStatus) //���
	{
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(2);
		m_bIsModifyStatus = TRUE;
	}
	else  
	{
		pCombo->EnableWindow(FALSE);
		pCombo->SetCurSel(3);
		m_bIsModifyStatus = FALSE;
	}

	return 0;
}

VOID CSeBookDlg::OnbtnBOOKRECEIPT() 
{
	// TODO: Add your control notification handler code here
	
	m_pGrid->SelectMakeList();
	INT idx = m_pGrid->SelectGetHeadPosition();

	if(idx<0) {
		AfxMessageBox(_T("������ ������ �����ϴ�!"));
			return;
	}
	
	CString strACQ_YEAR;
	strACQ_YEAR = m_pDM->GetCellData(_T("SB_���Գ⵵"),idx);
	CCheckinReceipt dlg(this);
	dlg.m_strACQ_YEAR = strACQ_YEAR;
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	UpdateData(FALSE);
	

	if(dlg.DoModal() != IDOK) return;

	CString strRECEIPT_NO;
	CString strLAST_SERIAL_NO;
	CString strNEW_ACQ_YEAR = dlg.m_strACQ_YEAR;
	CString strNEW_RECEIPT_NO = dlg.m_strRECEIPT_NO;
	CSeApi api(this);

	while(idx >= 0)
	{
			
		//���� ���Գ⵵ Ȥ�� ������ȣ�� ���ؼ� �ٸ� ��� ���� �ο��Ѵ�. 
		strACQ_YEAR = _T("");
		strRECEIPT_NO = _T("");
		strLAST_SERIAL_NO = _T("");

		strACQ_YEAR = m_pDM->GetCellData(_T("SB_���Գ⵵"),idx);
		strRECEIPT_NO = m_pDM->GetCellData(_T("SB_������ȣ"),idx);
			

		if(strACQ_YEAR.Compare(strNEW_ACQ_YEAR) || strRECEIPT_NO.Compare(strNEW_RECEIPT_NO))
		{
			//���� �Է� ��ȣ�� �ο��Ѵ� 
			strLAST_SERIAL_NO = api.GetReceiptSerialNo(m_pDM,m_nACQ_TYPE,strNEW_ACQ_YEAR,strNEW_RECEIPT_NO);
			if(!strLAST_SERIAL_NO.IsEmpty())
			{
				m_pDM->SetCellData(_T("SB_���Գ⵵"),dlg.m_strACQ_YEAR,idx);
				m_pDM->SetCellData(_T("SB_������ȣ"),dlg.m_strRECEIPT_NO,idx);
				m_pDM->SetCellData(_T("SB_�����Ϸù�ȣ"),strLAST_SERIAL_NO,idx);
			}
		}		

		idx = m_pGrid->SelectGetNext(); 
	}

	m_pGrid->Display();
	m_pGrid->SetReverse(m_nCurrentIdx);
}

VOID CSeBookDlg::OnbtnLINEMODFY() 
{
	// TODO: Add your control notification handler code here
	
	CString strDeleteYn;
	strDeleteYn = m_pDM->GetCellData(_T("UDF_��������"),m_nCurrentIdx);
	if(_T("Y") == strDeleteYn)
	{
		AfxMessageBox(_T("���� ó���� å�� ������ �� �����ϴ�!"));
		return;
	}


	if(m_bIsModifyStatus)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
		if(!pCombo) return;
		INT nCurSel = pCombo->GetCurSel();
		
		if(3 == nCurSel)
		{
			AfxMessageBox(_T("��Ÿ�ڷ���´� ������ �Ұ����� �ڷ���� �Դϴ�!"));
			return;
		}
		else if(1 == nCurSel)
		{
			//���� �ΰ��ڷ�� �̹� ������ �Ǿ� �ִ� ���� �����Ѵ� 
			if(VerifyBindingBook()<0) return;
		}
	}
	
	

	//�Լ��� 
	CString strData = _T("");
	m_pCM->GetControlMgrData(_T("�Լ���"),strData);
	m_pDM->SetCellData(_T("SB_�Լ���"),strData,m_nCurrentIdx);
	strData = _T("");
	
	//��� 
	m_pCM->GetControlMgrData(_T("���"),strData);
	m_pDM->SetCellData(_T("SB_���"),strData,m_nCurrentIdx);
	strData = _T("");
	
	//�ڷ��
	m_pCM->GetControlMgrData(_T("�ڷ��"),strData);
	m_pDM->SetCellData(_T("SB_�谡��ġ_�ڵ�"),strData,m_nCurrentIdx);
	m_pDM->SetCellData(_T("UDF_�ڷ��"),strData,m_nCurrentIdx);
	strData = _T("");
	

	//�������۾�
	m_pDM->SetCellData(_T("SB_�������۾�"),GetWorkLogMsg(WORK_LOG_SEBOOK),m_nCurrentIdx);


	CSeApi api(this);
	CString strToday;
	strToday = api.GetTodayDate();
	CString strDB_Working_Status;
	strDB_Working_Status = m_pDM->GetCellData(_T("DB_�ڷ����"),m_nCurrentIdx);


	//�ڷ���°� ������ ������ ������ ���
	if(m_bIsModifyStatus)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
		if(!pCombo) return;
		
		INT nCurSel = pCombo->GetCurSel();
		CString strUDF = _T("");

		if(0 == nCurSel)
		{
			strData = _T("SEL111O");
			strUDF = _T("�ǽð��ΰ��ڷ�");

			if(strDB_Working_Status != strData) //�ٸ� ������ �ΰ� �Ǿ����� �ΰ����� �������ش�
			{
				m_pDM->SetCellData(_T("SB_�ǽð��ΰ���"), strToday,m_nCurrentIdx);
			}
		}
		else if(1 == nCurSel)
		{
			strData = _T("SEB111N");
			strUDF = _T("�����ΰ��ڷ�");

			if(strDB_Working_Status != strData)
			{
				m_pDM->SetCellData(_T("SB_�����ΰ���"), strToday,m_nCurrentIdx);
			}

		}
		else if(2 == nCurSel)
		{
			strData = _T("SER111N");
			strUDF = _T("����ΰ��ڷ�");

			if(strDB_Working_Status != strData)
			{
				m_pDM->SetCellData(_T("SB_����ΰ���"), strToday,m_nCurrentIdx);
			}
		}

			
		m_pDM->SetCellData(_T("SB_�ڷ����"), strData,m_nCurrentIdx);
		m_pDM->SetCellData(_T("UDF_�ڷ����"), strUDF,m_nCurrentIdx);

	}
	

	//������ �۾�
	CString strWorkLogMsg;
	strWorkLogMsg = this->GetWorkLogMsg(WORK_LOG_SEBOOK);
	m_pDM->SetCellData(_T("SB_�������۾�"), strWorkLogMsg,m_nCurrentIdx);
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("�ڷ�Ǳ���"),_T("UDF_�ڷ��"));
	m_pGrid->Display();
	m_pGrid->SetReverse(m_nCurrentIdx);
	
}

INT CSeBookDlg::VerifyBindingBook()
{
	CString strBookKey;
	strBookKey = m_pDM->GetCellData(_T("SB_åKEY"),m_nCurrentIdx);

	CString strQuery;
	CString strResult;
	strQuery.Format(_T("SELECT BINDED_BOOK_YN FROM SE_BOOK_TBL WHERE REC_KEY = %s"),strBookKey);
	
	m_pDM->GetOneValueQuery(strQuery,strResult);

	strResult.TrimLeft();
	strResult.TrimRight();

	if(_T("Y") == strResult)
	{

		AfxMessageBox(_T("�ش� å�� �̹� �����Ǿ� �����Ƿ�\n")
						_T("���� �ΰ�ó�� �� �� �����ϴ�!"));
		
		return -1;
	}

	return 0;
}

VOID CSeBookDlg::OnOK() 
{
	// TODO: Add extra validation here
	// �ϰ����� �Լ� 
	// �� row �� �����ϴ� ������� ��ȯ
	
	INT nRowCnt = m_pDM->GetRowCount();
	CString strBOOK_KEY;
	CString strDeleteYn;
	CString msg;
	CArray<CString,CString> RemoveAliasList;
	CSeApi api(this);
	CESL_DataMgr* pDM_VOL = NULL;
	CESL_DataMgr* pDM_VOL_DELETE = NULL;
	INT ids = 0;

	pDM_VOL	= FindDM(_T("DM_����_å����_��"));
	pDM_VOL_DELETE = FindDM(_T("DM_����_å����_������"));
	

	CString strACQ_TYPE_VOL_KEY,strVOL_KEY,strSPECIES_KEY;
	CString strtitle;
	strtitle = m_strTITLE_INFO + _T(" ") + m_strVOL_TITLE;


	if(!pDM_VOL || !pDM_VOL_DELETE)
	{
		AfxMessageBox(_T("������� ������ ��ü�� ã�� �� �����ϴ�!"));
	}


	for(INT i=0;i<nRowCnt;i++)
	{
		strDeleteYn = _T("");
		strDeleteYn = m_pDM->GetCellData(_T("UDF_��������"),nRowCnt-1-i);
		
		if(_T("Y") == strDeleteYn)
		{
			
			strBOOK_KEY = m_pDM->GetCellData(_T("SB_åKEY"),nRowCnt-1-i);
			strACQ_TYPE_VOL_KEY = m_pDM->GetCellData(_T("SB_���Ա��к���KEY"),nRowCnt-1-i);
			strVOL_KEY = m_pDM->GetCellData(_T("SB_��KEY"),nRowCnt-1-i);
			strSPECIES_KEY = m_pDM->GetCellData(_T("SB_��KEY"),nRowCnt-1-i);

			CSeApi api(this);
			ids = api.BOOK_CAT_DELETE(m_pDM,
									  strBOOK_KEY,
									  strACQ_TYPE_VOL_KEY,
									  strVOL_KEY,
									  strSPECIES_KEY,
									  _T("C"),
									  strtitle,
									  FALSE,
									  FALSE);
			
	
			if(ids<0)
			{
				msg.Format(_T(" [ %d ] ��° �۾��� �����ϴ� �� ������ ���� �۾��� �ߴ��մϴ�!"),nRowCnt-1-i);
				AfxMessageBox(msg);
				return;
			}
			else
			{
				m_bIsDeleteProc = TRUE;
				m_pDM->DeleteRow(nRowCnt-1-i);

			}

		}
		else 
		{

		
			m_pDM->StartFrame();

			RemoveAliasList.RemoveAll();
			RemoveAliasList.Add(_T("SB_åKEY"));
			RemoveAliasList.Add(_T("UDF_�ڷ��"));
			RemoveAliasList.Add(_T("DB_�ڷ����"));
			
			strBOOK_KEY = _T("");
			strBOOK_KEY = m_pDM->GetCellData(_T("SB_åKEY"),nRowCnt-1-i);
			m_pDM->InitDBFieldData();
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("SE_BOOK_TBL") , nRowCnt-1-i , m_pDM, _T("REC_KEY") , _T("NUMERIC") , strBOOK_KEY);
				

			ids = m_pDM->SendFrame();
			m_pDM->EndFrame();
			
			if(ids<0)
			{
				msg.Format(_T(" [ %d ] ��° �۾��� �����ϴ� �� ������ ���� �۾��� �ߴ��մϴ�!"),nRowCnt-1-i);
				AfxMessageBox(msg);
				return;
			}
		}

	}

	
	AfxMessageBox(_T("�����Ͽ����ϴ�!"));
	
	//���� ���۷��̼� ����� ���� ȭ�� refresh 
	if(m_bIsDeleteProc)
	{
		EndDialog(11);
	}

	//â�� ���� �ʴ´� 
	return;
	
	CDialog::OnOK();
}

VOID CSeBookDlg::OnbtnBOOKDELETE() 
{
	// TODO: Add your control notification handler code here
	CString strWorkingStatus;
	CString strMsg;

	strWorkingStatus = m_pDM->GetCellData(_T("UDF_�ڷ����"),m_nCurrentIdx);
	strMsg.Format(_T("[ %s ] ������ å�� ���� �Ͻðڽ��ϱ�?"), strWorkingStatus);

	if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return; 

	m_pDM->SetCellData(_T("UDF_��������"),_T("Y"),m_nCurrentIdx);
	


	if(VerifyDeleteProc()<0)
	{
		m_pDM->SetCellData(_T("UDF_��������"),_T(""),m_nCurrentIdx);
		return;
	}
	

	
	//������ ������ ������� �ѹ��� ���� ����
	CString strBOOK_KEY,strACQ_TYPE_VOL_KEY,strVOL_KEY,strSPECIES_KEY;
	strBOOK_KEY = m_pDM->GetCellData(_T("SB_åKEY"),m_nCurrentIdx);
	strACQ_TYPE_VOL_KEY = m_pDM->GetCellData(_T("SB_���Ա��к���KEY"),m_nCurrentIdx);
	strVOL_KEY = m_pDM->GetCellData(_T("SB_��KEY"),m_nCurrentIdx);
	strSPECIES_KEY = m_pDM->GetCellData(_T("SB_��KEY"),m_nCurrentIdx);

	INT ids = -1;

  	CString strtitle;
	strtitle = m_strTITLE_INFO + _T(" ") + m_strVOL_TITLE;

	CSeApi api(this);
	ids = api.BOOK_CAT_DELETE(m_pDM,
		                      strBOOK_KEY,
						      strACQ_TYPE_VOL_KEY,
						      strVOL_KEY,
						      strSPECIES_KEY,
						      _T("C"),
						      strtitle,
						      FALSE,
						      TRUE);
   
	if(ids<0)
	{
		m_pDM->SetCellData(_T("UDF_��������"),_T(""),m_nCurrentIdx);
		return;
	}


	// color refresh
	RefreshGridColor();

}

INT CSeBookDlg::VerifyDeleteProc()
{
	
	//1) ��� �Լ� å�� ���� �Ǿ��� ��� -- ���, �η� ���� �Ǻ�
	
	INT nRowCnt = m_pDM->GetRowCount();
	CString strDeleteYn;
	INT nDeleteCnt = 0;
	CString strQuery;
	CString msg;

	for(INT i=0;i<nRowCnt;i++)
	{
		strDeleteYn = _T("");	
		strDeleteYn = m_pDM->GetCellData(_T("UDF_��������"),i);
		if(_T("Y") == strDeleteYn) nDeleteCnt++;
	}

	if(nDeleteCnt == nRowCnt)
	{
		CString strAcqVolKey;
		CString strVolKey;
		strAcqVolKey = m_pDM->GetCellData(_T("SB_���Ա��к���KEY"),m_nCurrentIdx);
		strVolKey = m_pDM->GetCellData(_T("SB_��KEY"),m_nCurrentIdx);
		
		//��ȣ å�� ����
		INT nMissBookCnt = 0;
		CString strMissBookCnt;
		strQuery.Format(_T("SELECT MISS_BOOK_CNT FROM SE_ACQ_TYPE_VOL_TBL WHERE REC_KEY = %s"),strAcqVolKey);
		m_pDM->GetOneValueQuery(strQuery,strMissBookCnt);
		if(!strMissBookCnt.IsEmpty()) nMissBookCnt = _ttoi(strMissBookCnt.GetBuffer(0));
		
		
		if(0 == nMissBookCnt) //�ش� ��ȣ ���� 
		{
			//�ٸ� ���� ������ �ִ����� ����
			CString strResult;
			INT nAcqVolCnt = 0;
			strQuery.Format(_T("SELECT COUNT(*) FROM SE_ACQ_TYPE_VOL_TBL WHERE VOL_KEY = %s"),strVolKey);
			m_pDM->GetOneValueQuery(strQuery,strResult);			
			if(!strResult.IsEmpty()) nAcqVolCnt = _ttoi(strResult.GetBuffer(0));
			
			if(nAcqVolCnt>1) //�ٸ� ���Ա����� �ִ� ��� (�ش� ��ȣ ���� �޽���)
			{
				msg.Format(_T("�ش�å ������ ���� å�� ���� �Ǿ�\n") 
							_T("��ȣ ������ ������ϴ�\n")
							_T("�׷��� ���� �Ͻðڽ��ϱ�?"));
				
				if(AfxMessageBox(msg,MB_YESNO) != IDYES) return -1001;

			}
			else  //�ٸ� ���Ա����� ���� ��� (�ش� ��ȣ�� ���� �η��� Ȯ�� �� ���� �޽���)
			{
				
				CString strArticleCnt;
				INT nArticleCnt;
				strQuery.Format(_T("SELECT COUNT(*) FROM SE_ARTICLE_TBL WHERE VOL_KEY = %s"),strVolKey);
				m_pDM->GetOneValueQuery(strQuery,strArticleCnt);
				nArticleCnt = _ttoi(strArticleCnt.GetBuffer(0));
					
				if(nArticleCnt>0)
				{
					AfxMessageBox(_T("�ش� ��ȣ��  ��������� �־� �ش� å�� ���� �Ҽ� �����ϴ�!\n")
							   _T("���� ��������� �����Ͻ� �� ���� �Ͻ� �� �ֽ��ϴ�."));					  
					return -1002;
				}
				
				CString strAppendixCnt;
				INT nAppendixCnt;
				strQuery.Format(_T("SELECT COUNT(*) FROM SE_BOOK_TBL WHERE VOL_KEY = %s AND BOOK_TYPE = 'A'"),strVolKey);
				m_pDM->GetOneValueQuery(strQuery,strAppendixCnt);
				nAppendixCnt = _ttoi(strAppendixCnt.GetBuffer(0));
	
				if(nAppendixCnt>0)
				{
					AfxMessageBox(_T("�ش� ��ȣ�� �η� ������ �־� �ش� å�� ���� �Ҽ� �����ϴ�!\n")
									_T("���� �η� ������ �����Ͻ� �� ���� �Ͻ� �� �ֽ��ϴ�."));	
					return -1003;
				}

				msg.Format(_T("�ش�å ������ ���� å�� ���� �Ǿ�\n") 
				_T("��ȣ ������ ������ϴ�\n")
				_T("�׷��� ���� �Ͻðڽ��ϱ�?"));
				
				if(AfxMessageBox(msg,MB_YESNO) != IDYES) return -1001;
			}

		}
	}

	
	return 0;
}

VOID CSeBookDlg::RefreshGridColor()
{
	INT nRowCnt = m_pDM->GetRowCount();
	
	CString strDeleteYn;
	
	for(INT i=0;i<nRowCnt;i++)
	{
		strDeleteYn = _T("");
		strDeleteYn = m_pDM->GetCellData(_T("UDF_��������"),i);
		
		if(_T("Y") == strDeleteYn)
		{
			SetGridRowUniqueColor(i);
		}

	}

}

VOID CSeBookDlg::SetGridRowUniqueColor(INT nRow)
{
	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = m_pGrid->GetCol();
	INT nCols = m_pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		m_pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGrid->SetCol(i);
			m_pGrid->SetCellBackColor(red);
			m_pGrid->SetCellForeColor(black);
		}
	}
	
	m_pGrid->SetCol(nCol);
}

VOID CSeBookDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	if(m_bIsDeleteProc)
	{
		EndDialog(11);
	}

	CDialog::OnCancel();
}

HBRUSH CSeBookDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
