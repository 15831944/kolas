// SeAppendixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeAppendixDlg.h"
#include "..\SE_API\SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeAppendixDlg dialog


CSeAppendixDlg::CSeAppendixDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeAppendixDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeAppendixDlg)
	m_bDISCARD_YN = FALSE;
	m_bREG_FLAG = FALSE;
	//}}AFX_DATA_INIT
	
	m_pDM = NULL;
	m_pDM_BOOK = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	strVOL_TITLE = _T("");
	strACQ_VOL_KEY = _T("");
	m_nCurrentIdx = -1;
	m_nACQ_TYPE = 1; //default
	m_arrayDeleteAppendixKey.RemoveAll();
	m_arrayDeleteBookKey.RemoveAll();
	m_nCurrentParentIdx = 0;
	m_strNewArticleCnt = _T("");
	m_bIsModifyStatus = FALSE;
}

CSeAppendixDlg::~CSeAppendixDlg()
{
}

VOID CSeAppendixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeAppendixDlg)
	DDX_Check(pDX, IDC_chkAPPENDIX_DISCARD_YN, m_bDISCARD_YN);
	DDX_Check(pDX, IDC_chkAPPENDIX_REG_FLAG, m_bREG_FLAG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeAppendixDlg, CDialog)
	//{{AFX_MSG_MAP(CSeAppendixDlg)
	ON_BN_CLICKED(IDC_btnAPPENDIX_INSERT, OnbtnAPPENDIXINSERT)
	ON_BN_CLICKED(IDC_btnAPPENDIX_MODIFY, OnbtnAPPENDIXMODIFY)
	ON_BN_CLICKED(IDC_btnAPPENDIX_DELETE, OnbtnAPPENDIXDELETE)
	ON_BN_CLICKED(IDC_btnAPPENDIX_CLEAR, OnbtnAPPENDIXCLEAR)
	ON_BN_CLICKED(IDC_chkAPPENDIX_DISCARD_YN, OnchkAPPENDIXDISCARDYN)
	ON_BN_CLICKED(IDC_chkAPPENDIX_REG_FLAG, OnchkAPPENDIXREGFLAG)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeAppendixDlg message handlers

BOOL CSeAppendixDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr(_T("����_����_�η�")) < 0 ) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_�η�") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_����_�η�"));
	m_pDM = FindDM(_T("DM_����_�η�"));
	m_pDM_BOOK = FindDM(_T("DM_����_�η�_å"));
	m_pDM_WORK = FindDM(_T("DM_üũ��_�۾�"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���")); 

	if(!m_pCM || !m_pDM || !m_pGrid || !m_pDM_BOOK || !m_pDM_WORK) {
		AfxMessageBox( _T("CM,DM,Gird Error : ����_����_�η�") );
		return FALSE;
	}

	m_pCM->SetControlMgrData(_T("��ȣ��"),strVOL_TITLE);
	
	if(strACQ_VOL_KEY.IsEmpty())
	{
		AfxMessageBox(_T("���Ա��к���KEY �� ã�� �� �����ϴ�!"));
		return FALSE;
	}
	

	
	//�ڷ� ���¸� ���� ���ش�.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("�������") );
		pCombo->InsertString( 1, _T("�ǽð��ΰ��ڷ�") );
		pCombo->InsertString( 2, _T("����ΰ��ڷ�") );
		pCombo->InsertString( 3, _T("��Ÿ�ڷ����") );
		pCombo->SetCurSel( 0 );
	}

	
	CString strWhere;
	strWhere.Format(_T("A.BOOK_KEY = B.REC_KEY AND B.BOOK_TYPE = 'A' AND B.ACQ_TYPE_VOL_KEY = %s"),strACQ_VOL_KEY);
	m_pDM->RefreshDataManager(strWhere);
	
	if(m_pDM->GetRowCount()>0) {
		m_nCurrentIdx = 0;
		m_pGrid->SetReverse(m_nCurrentIdx);
		ShowAllData();
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CSeAppendixDlg::ShowAllData()
{
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("���ı���"),_T("UDF_���ı���"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("��ü����"),_T("UDF_��ü����"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("��ϱ���"),_T("UDF_��ϱ���"));

	CString strREG_FLAG;
	CString strWorkingStatus;
	CString strUDF_Working_Status;

	INT nRow = m_pDM->GetRowCount();
	CSeApi api(this);

	//��ü �ڷ����ó��
	for(INT i=0;i<nRow;i++)
	{
		strREG_FLAG = m_pDM->GetCellData(_T("���ົ��Ͽ���"),i);
		strWorkingStatus = m_pDM->GetCellData(_T("�ڷ����"),i);
		strWorkingStatus.TrimLeft();
		strWorkingStatus.TrimRight();

		
		if(_T("Y") == strREG_FLAG) 
		{
			strUDF_Working_Status = _T("���ົ����ڷ�");
		}
		else
		{
			if(strWorkingStatus.IsEmpty())
			{
				strUDF_Working_Status = _T("�������");
			}
			else
			{
				strUDF_Working_Status = api.GetWorkingStatusDesc(strWorkingStatus);
			}
		}

		m_pDM->SetCellData(_T("UDF_�ڷ����"),strUDF_Working_Status,i);
	}
	
	
	
	m_pCM->AllControlDisplay(m_nCurrentIdx);
	

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS);
	if(!pCombo) return -1;


	//���ົ��Ͽ��� (���ົ ��� ���·� �Ǿ� ������ �ڷ���´� ��Ÿ�� �ؼ� disable�Ѵ�
	strREG_FLAG = m_pDM->GetCellData(_T("���ົ��Ͽ���"),m_nCurrentIdx);
	if(_T("Y") == strREG_FLAG) {

		m_bREG_FLAG = TRUE;

	}
	else 
	{
		m_bREG_FLAG = FALSE;
	}

	//�ڷ���°� �������(��ĭ)-�ǽð��ΰ�-����ΰ��� �ϳ��� �ڷ���� �޺��� �����Ϳ� ���߾� �ش�
	//�� �ܿ��� ��Ȱ��ȭ 

	strWorkingStatus = m_pDM->GetCellData(_T("�ڷ����"),m_nCurrentIdx);
	strWorkingStatus.TrimLeft();
	strWorkingStatus.TrimRight();

	if(m_bREG_FLAG) //���ົ ��� ���� 
	{
		pCombo->EnableWindow(FALSE);
		pCombo->SetCurSel(3);
		m_bIsModifyStatus = FALSE;
		GetDlgItem(IDC_chkAPPENDIX_REG_FLAG)->EnableWindow(TRUE);
	}
	else //�����ڷ�� ������ ó����
	{

		if(strWorkingStatus.IsEmpty()) //�������
		{
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(0);
			m_bIsModifyStatus = TRUE;
			GetDlgItem(IDC_chkAPPENDIX_REG_FLAG)->EnableWindow(TRUE);

		}
		else if(_T("SEL111O") == strWorkingStatus) //�ǽð�
		{
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(1);
			m_bIsModifyStatus = TRUE;
			GetDlgItem(IDC_chkAPPENDIX_REG_FLAG)->EnableWindow(TRUE);

		}
		else if(_T("SER111N") == strWorkingStatus) //���
		{
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(2);
			m_bIsModifyStatus = TRUE;
			GetDlgItem(IDC_chkAPPENDIX_REG_FLAG)->EnableWindow(TRUE);
		}
		else  //��Ÿ �ڷ� ����(�ǽð� �Ǵ� ��� �� �ڷ� -> ���ົ���� ��� ���� ���ϰ� �Ѵ�)
		{
			pCombo->EnableWindow(FALSE);
			pCombo->SetCurSel(3);
			m_bIsModifyStatus = FALSE;
			GetDlgItem(IDC_chkAPPENDIX_REG_FLAG)->EnableWindow(FALSE);
		}
	}

	
	//��⿩�� 
	CString strDISCARD_YN;
	strDISCARD_YN = m_pDM->GetCellData(_T("��⿩��"),m_nCurrentIdx);
	if(_T("Y") == strDISCARD_YN)  m_bDISCARD_YN = TRUE;
	else m_bDISCARD_YN = FALSE;

	UpdateData(FALSE);
	m_pGrid->SetReverse(m_nCurrentIdx);

	return 0;
}

BEGIN_EVENTSINK_MAP(CSeAppendixDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeAppendixDlg)
	ON_EVENT(CSeAppendixDlg, IDC_gridAPPENDIX, -600 /* Click */, OnClickgridAPPENDIX, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeAppendixDlg::OnClickgridAPPENDIX() 
{
	// TODO: Add your control notification handler code here

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			m_nCurrentIdx = row-1;
			ShowAllData();
		}
	
	}

}

VOID CSeAppendixDlg::OnbtnAPPENDIXINSERT() 
{
	// TODO: Add your control notification handler code here
	CString tmp;
	m_pCM->GetControlMgrData(_T("�ηϸ�"),tmp);
	if(tmp.IsEmpty()) {
		AfxMessageBox(_T("�ηϸ��� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}

	//���ົ ��� ���ΰ� ǥ�õ��� ������ ��Ÿ �ڷ���´� ������ �� ����
	UpdateData(TRUE);
	if(!m_bREG_FLAG) {
		
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS);
		if(pCombo)
		{
			INT nCurSel = -1;
			nCurSel = pCombo->GetCurSel();
			if(3 == nCurSel) 
			{
				AfxMessageBox(_T("��Ÿ�ڷ���´� ����ڰ� ������ �� �����ϴ�!"));
				return;
				
			}
		}
	}



	m_pDM->InsertRow(-1);
	INT row = m_pDM->GetRowCount()-1;
	SetAllData(row, _T("I")); //insert
	OnOK();
	
}

VOID CSeAppendixDlg::SetAllData(INT row, CString strUDF)
{
	const INT cnt = 6;
	CString alias[cnt];
	CString data[cnt];

	alias[0] = _T("�ηϸ�");
	alias[1] = _T("���ı���");
	alias[2] = _T("��ü����");
	alias[3] = _T("���");
	alias[4] = _T("�����");
	alias[5] = _T("������");
	

	for(INT i=0;i<cnt;i++)
	{
		m_pCM->GetControlMgrData(alias[i],data[i]);
	}

	m_pDM->SetCellData(alias,6,data,row);
	
	CSeApi api(this);
	
	//UDFó�� 
	m_pDM->SetCellData(_T("UDF_���ı���"),data[1],row);
	m_pDM->SetCellData(_T("UDF_��ü����"),data[2],row);


	//���ົ��Ͽ���
	//���ົ ��� ���ΰ� ǥ�ø� �ڷ���¸� �����ְ�, ���� ������ �ڷ���¸� �������ش�
	UpdateData(TRUE);
	CString strREG_FLAG;
	CString strWorking_Status;
	CString strDB_Working_Status;
	CString strToday = api.GetTodayDate();
	strDB_Working_Status = m_pDM->GetCellData(_T("DB�ڷ����"),row);
	
	if(m_bREG_FLAG) {
		
		strREG_FLAG = _T("Y");
		strWorking_Status = _T("");
	}
	else
	{
		
		strREG_FLAG = _T("N");

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS);
		if(pCombo)
		{
			INT nCurSel = -1;
			nCurSel = pCombo->GetCurSel();
			if(0 == nCurSel) 
			{
				strWorking_Status = _T("");

			}
			else if(1 == nCurSel) 
			{
				strWorking_Status = _T("SEL111O");
				//DB�ڷ���¿� �ٸ��� �ǽð��ΰ���(IMM_LOC_TRANSFER_DATE) ����
				
				if(strWorking_Status != strDB_Working_Status)
				{
					m_pDM->SetCellData(_T("�ǽð��ΰ���"),strToday,row);
				}

			}
			else if(2 == nCurSel)
			{
			
				strWorking_Status = _T("SER111N");
				//DB�ڷ���¿� �ٸ��� ����ΰ���(REG_TRANSFER_DATE) ����
				if(strWorking_Status != strDB_Working_Status)
				{
					m_pDM->SetCellData(_T("����ΰ���"),strToday,row);
				}

			}

		}
	}

	
	m_pDM->SetCellData(_T("�ڷ����"),strWorking_Status,row);
	m_pDM->SetCellData(_T("���ົ��Ͽ���"),strREG_FLAG,row);
	


	//��⿩��
	CString strDISCARD_YN;
	if(m_bDISCARD_YN) strDISCARD_YN = _T("Y");
	else strDISCARD_YN = _T("N");
	
	m_pDM->SetCellData(_T("��⿩��"),strDISCARD_YN,row);
	m_pDM->SetCellData(_T("UDF_����"),strUDF,row); 

	
	//��⿩�ο� ���� ��� ���� ����
	if(_T("N") == strDISCARD_YN) {
		CString strREG_CODE;
		m_pCM->GetControlMgrData(_T("��ϱ���"),strREG_CODE);
		m_pDM->SetCellData(_T("��ϱ���"),strREG_CODE,row);
		m_pDM->SetCellData(_T("UDF_��ϱ���"),strREG_CODE,row);
	}


	m_nCurrentIdx =	row;
	ShowAllData();
}

VOID CSeAppendixDlg::OnbtnAPPENDIXMODIFY() 
{
	// TODO: Add your control notification handler code here
	if( 0 == m_pDM->GetRowCount() || m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return;
	}

	CString tmp;
	m_pCM->GetControlMgrData(_T("�ηϸ�"),tmp);
	if(tmp.IsEmpty()) {
		AfxMessageBox(_T("�ηϸ��� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}

	//���ົ ��� ���ΰ� ǥ�õ��� ������ ��Ÿ �ڷ���´� ������ �� ����
	UpdateData(TRUE);
	if(!m_bREG_FLAG) {
		
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS);
		if(pCombo)
		{
			INT nCurSel = -1;
			nCurSel = pCombo->GetCurSel();
			if(3 == nCurSel) 
			{
				AfxMessageBox(_T("��Ÿ�ڷ���´� ����ڰ� ������ �� �����ϴ�!"));
				return;
				
			}
		}
	}

	
	CString strUDF;
	strUDF = m_pDM->GetCellData(_T("UDF_����"),m_nCurrentIdx);

	if(_T("I") == strUDF)
	{
		SetAllData(m_nCurrentIdx, _T("I")); //insert
	}
	else 
	{
		SetAllData(m_nCurrentIdx, _T("U")); //update
	}
	OnOK();

}

VOID CSeAppendixDlg::OnbtnAPPENDIXDELETE() 
{
	// TODO: Add your control notification handler code here
	if( 0 == m_pDM->GetRowCount() || m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return;
	}
	
	
	CString strAPPENDIX_NAME;
	strAPPENDIX_NAME = m_pDM->GetCellData(_T("�ηϸ�"),m_nCurrentIdx);

	CString msg;
	msg.Format(_T("[ %s ] �η��� �����Ͻðڽ��ϱ�?"), strAPPENDIX_NAME);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;
	
		

	//�ڷ���¸� ������ �ڷ���°� �ΰ���� �̻��̸� ������ �����Ѵ�(������������ �ʴ´�)
	
	
						
	
	//������µ��� �����Ѵ�
	CString strBOOK_KEY,strVOL_KEY;

	strBOOK_KEY = m_pDM->GetCellData(_T("åKEY"),m_nCurrentIdx);
	strVOL_KEY = m_pDM->GetCellData(_T("��KEY"),m_nCurrentIdx);
	

	CSeApi api(this);
	
	INT ids = -1;

	ids = api.BOOK_DELETE_VERIFY(m_pDM,
							     strBOOK_KEY,
							     strVOL_KEY,
							     _T("A"),
							     strAPPENDIX_NAME);

	if(ids<0) return;





	CString strUDF;
	strUDF = m_pDM->GetCellData(_T("UDF_����"),m_nCurrentIdx);
	if(_T("I") == strUDF)
	{
		m_pDM->DeleteRow(m_nCurrentIdx);
	
	}
	else 
	{
		CString strAPPENDIX_KEY;
		strAPPENDIX_KEY = m_pDM->GetCellData(_T("�η�KEY"),m_nCurrentIdx);
		m_arrayDeleteAppendixKey.Add(strAPPENDIX_KEY);
		
		CString strBOOK_KEY;
		strBOOK_KEY = m_pDM->GetCellData(_T("åKEY"),m_nCurrentIdx);
		m_arrayDeleteBookKey.Add(strBOOK_KEY);

		m_pDM->DeleteRow(m_nCurrentIdx);
	}
	
	INT nRowCnt = m_pDM->GetRowCount();
	

	if(nRowCnt>0)
	{
		m_nCurrentIdx = 0;
		ShowAllData();
	}
	else if(0 == nRowCnt)
	{
		m_nCurrentIdx = -1;
		OnbtnAPPENDIXCLEAR();
		m_pGrid->Display();
	}
	OnOK();
	
}

VOID CSeAppendixDlg::OnbtnAPPENDIXCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();
	m_bREG_FLAG = FALSE;
	m_bDISCARD_YN = FALSE;
	m_pCM->SetControlMgrData(_T("��ȣ��"),strVOL_TITLE);
	UpdateData(FALSE);

}

VOID CSeAppendixDlg::OnOK() 
{
	// TODO: Add extra validation here
	// �ϰ� ���� �Լ� 
	INT nSize;
	nSize = m_arrayDeleteAppendixKey.GetSize();
	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();
	if(0 == nSize && 0 == nRowCnt)
	{
		AfxMessageBox(_T("��ȭ�� ������ �����ϴ�!"));
		return;
	}


	CString strUDF;
	CString strQuery;
	m_pDM->StartFrame();
	CSeApi api(this);
	
	//�� ������ ����� foreign KEY�� �����Ѵ�
	CString strSPECIES_KEY;
	strSPECIES_KEY = m_pDM_WORK->GetCellData(_T("ISS_��KEY"),m_nCurrentParentIdx);
	CString strVOL_KEY;
	strVOL_KEY = m_pDM_WORK->GetCellData(_T("SV_��KEY"),m_nCurrentParentIdx);
	CString strACQ_VOL_KEY;
	strACQ_VOL_KEY = m_pDM_WORK->GetCellData(_T("SA_���Ա��к���KEY"),m_nCurrentParentIdx);
	

	INT nIdx_book;
	CString strREMARK;
	CString strREG_CODE;
	CString strWorking_Status;
	CArray<CString,CString> RemoveAliasList;
	CString strAPPENDIX_KEY;
	CString strBOOK_KEY;
	CString strIMM_LOC_TRANSFER_DATE;
	CString strREG_TRANFER_DATE;
	CString strWorkLogMsg = this->GetWorkLogMsg(WORK_LOG_SEAPPENDIX);
	
	for(INT i=0;i<nRowCnt;i++)
	{
		strUDF = _T("");
		strREMARK = _T("");
		strAPPENDIX_KEY = _T("");
		strBOOK_KEY = _T("");
		strUDF = m_pDM->GetCellData(_T("UDF_����"),i);

		if(_T("I") == strUDF) //insert
		{
			
			api.MAKE_APPENDIX_BOOK_by_WORK(m_pDM_WORK,
										   m_nCurrentParentIdx,
										   m_nACQ_TYPE,
										   m_pDM,
										   i,
										   m_pDM_BOOK,
										   TRUE);
			
			nIdx_book = m_pDM_BOOK->GetRowCount()-1;
			
			//��KEY , ���Ա��к���KEY, ��� 
			m_pDM_BOOK->SetCellData(_T("SB_��KEY"),strVOL_KEY,nIdx_book);
			m_pDM_BOOK->SetCellData(_T("SB_���Ա��к���KEY"),strACQ_VOL_KEY,nIdx_book);
			strREMARK = m_pDM->GetCellData(_T("���"),i);
			strREG_CODE = m_pDM->GetCellData(_T("��ϱ���"),i);
			strWorking_Status = m_pDM->GetCellData(_T("�ڷ����"),i);
			strIMM_LOC_TRANSFER_DATE = m_pDM->GetCellData(_T("�ǽð��ΰ���"),i);
			strREG_TRANFER_DATE = m_pDM->GetCellData(_T("����ΰ���"),i);
			m_pDM_BOOK->SetCellData(_T("SB_���"),strREMARK,nIdx_book);
			m_pDM_BOOK->SetCellData(_T("SB_��ϱ���_�ڵ�"),strREG_CODE,nIdx_book);
			m_pDM_BOOK->SetCellData(_T("SB_�ڷ����"),strWorking_Status,nIdx_book);
			m_pDM_BOOK->SetCellData(_T("SB_�ǽð��ΰ���"),strIMM_LOC_TRANSFER_DATE,nIdx_book);
			m_pDM_BOOK->SetCellData(_T("SB_����ΰ���"),strREG_TRANFER_DATE,nIdx_book);
	
		
			//�η�frame
			RemoveAliasList.RemoveAll();
			RemoveAliasList.Add(_T("UDF_���ı���"));
			RemoveAliasList.Add(_T("UDF_��ü����"));
			RemoveAliasList.Add(_T("UDF_��ϱ���"));
			RemoveAliasList.Add(_T("UDF_�ڷ����"));
			RemoveAliasList.Add(_T("��ϱ���"));
			RemoveAliasList.Add(_T("DB�ڷ����"));
			RemoveAliasList.Add(_T("�ǽð��ΰ���"));
			RemoveAliasList.Add(_T("����ΰ���"));
			RemoveAliasList.Add(_T("�ڷ����"));
			RemoveAliasList.Add(_T("���"));
			m_pDM->InitDBFieldData();
			m_pDM->MakeQueryByJoinedDM(RemoveAliasList, m_pDM, _T("SE_APPENDIX_BOOK_TBL"), i , m_pDM);
			

			//åframe
			RemoveAliasList.RemoveAll();
			m_pDM->InitDBFieldData();
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM_BOOK, _T("SE_BOOK_TBL"), nIdx_book , m_pDM);
	
		}
		else if(_T("U") == strUDF) //update
		{
			
			//�η�frame
			RemoveAliasList.RemoveAll();
			RemoveAliasList.Add(_T("UDF_���ı���"));
			RemoveAliasList.Add(_T("UDF_��ü����"));
			RemoveAliasList.Add(_T("UDF_��ϱ���"));
			RemoveAliasList.Add(_T("UDF_�ڷ����"));
			RemoveAliasList.Add(_T("��ϱ���"));
			RemoveAliasList.Add(_T("DB�ڷ����"));
			RemoveAliasList.Add(_T("�ǽð��ΰ���"));
			RemoveAliasList.Add(_T("����ΰ���"));
			RemoveAliasList.Add(_T("�ڷ����"));
			RemoveAliasList.Add(_T("���"));
			RemoveAliasList.Add(_T("�η�KEY"));
			strAPPENDIX_KEY = m_pDM->GetCellData(_T("�η�KEY"),i);
			strBOOK_KEY = m_pDM->GetCellData(_T("åKEY"),i);
			m_pDM->InitDBFieldData();
			m_pDM->MakeQueryByJoinedDM(RemoveAliasList, m_pDM, _T("SE_APPENDIX_BOOK_TBL"), i , m_pDM, _T("REC_KEY"), _T("NUMERIC"), strAPPENDIX_KEY);
			

			//å(���,��ϱ���) frame 
			strREMARK = m_pDM->GetCellData(_T("���"),i);
			strREG_CODE = m_pDM->GetCellData(_T("��ϱ���"),i);
			strWorking_Status = m_pDM->GetCellData(_T("�ڷ����"),i);
			strIMM_LOC_TRANSFER_DATE = m_pDM->GetCellData(_T("�ǽð��ΰ���"),i);
			strREG_TRANFER_DATE = m_pDM->GetCellData(_T("����ΰ���"),i);

			strQuery.Format(_T("UPDATE SE_BOOK_TBL SET REMARK = '%s',REG_CODE = '%s',")
							_T("WORKING_STATUS = '%s',IMM_LOC_TRANSFER_DATE = '%s',")
							_T("REG_TRANSFER_DATE = '%s',LAST_WORK = '%s' WHERE REC_KEY = %s;"),
							strREMARK,strREG_CODE,strWorking_Status,strIMM_LOC_TRANSFER_DATE,
							strREG_TRANFER_DATE,strWorkLogMsg,strBOOK_KEY);
			m_pDM->AddFrame(strQuery);
			
		}

	}
	
	//delete
	for(i=0;i<nSize;i++)
	{
		strAPPENDIX_KEY = m_arrayDeleteAppendixKey.GetAt(i);
		strBOOK_KEY = m_arrayDeleteBookKey.GetAt(i);


		//������ ������ ������� ����
		api.BOOK_CAT_DELETE_APPENDIX(m_pDM,
									 strBOOK_KEY,
									 strSPECIES_KEY,
									 FALSE);
		
	}
	
	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();

	if(ids<0)
	{
		AfxMessageBox(_T("���忡 �����Ͽ����ϴ�!"));
	}
	else
	{
		
		for(i=0;i<nRowCnt;i++)
		{
			//���°� refresh
			m_pDM->SetCellData(_T("UDF_����"),_T(""),i);
			m_arrayDeleteAppendixKey.RemoveAll();
			m_arrayDeleteBookKey.RemoveAll();

		}
		
		AfxMessageBox(_T("�����Ͽ����ϴ�!"));


	}

	return;

	CDialog::OnOK();
}

VOID CSeAppendixDlg::OnchkAPPENDIXDISCARDYN() 
{
	// TODO: Add your control notification handler code here
	// ��⿩�� click event : ��⿩�ΰ� ���õǸ� ��ϱ����� ��Ȱ��ȭ ��Ų��
	UpdateData(TRUE);

	if(m_bDISCARD_YN) GetDlgItem(IDC_cmbAPPENDIX_REG_CODE)->EnableWindow(FALSE);
	else GetDlgItem(IDC_cmbAPPENDIX_REG_CODE)->EnableWindow(TRUE);

}

VOID CSeAppendixDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	// ó���� �ηϼ� 
	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();
	m_strNewArticleCnt.Format(_T("%d"),nRowCnt);


	CDialog::OnCancel();
}

VOID CSeAppendixDlg::OnchkAPPENDIXREGFLAG() 
{
	// TODO: Add your control notification handler code here
	
	// ���ົ ��� ���°� Ǯ���� �ڷ���¸� ������ �� �ְ� �Ѵ�
	UpdateData(TRUE);

	if(m_bREG_FLAG) 
	{
		GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_cmbAPPENDIX_WORKING_STATUS)->EnableWindow(TRUE);
	}
}

HBRUSH CSeAppendixDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
