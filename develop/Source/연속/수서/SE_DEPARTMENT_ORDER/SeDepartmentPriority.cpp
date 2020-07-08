// SeDepartmentPriority.cpp : implementation file
//

#include "stdafx.h"
#include "SeDepartmentPriority.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeDepartmentPriority dialog


CSeDepartmentPriority::CSeDepartmentPriority(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeDepartmentPriority)
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_nCurrentIdx = -1;
	m_arrayDeleteKey.RemoveAll();
}

CSeDepartmentPriority::~CSeDepartmentPriority()
{
}

VOID CSeDepartmentPriority::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeDepartmentPriority)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeDepartmentPriority, CDialog)
	//{{AFX_MSG_MAP(CSeDepartmentPriority)
	ON_BN_CLICKED(IDC_btnDepPriorityDelete, OnbtnDepPriorityDelete)
	ON_BN_CLICKED(IDC_btnDepPriorityUp, OnbtnDepPriorityUp)
	ON_BN_CLICKED(IDC_btnDepPriorityDown, OnbtnDepPriorityDown)
	ON_BN_CLICKED(IDC_btnDepPrioritySave, OnbtnDepPrioritySave)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeDepartmentPriority message handlers

BOOL CSeDepartmentPriority::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		if(InitESL_Mgr(_T("����_����_�μ������ڷ�_�켱����"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_�μ������ڷ�_�켱����") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_����_�μ������ڷ�_�켱����"));
	m_pDM = FindDM(_T("DM_����_����_�μ������ڷ�_�켱����"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));

	if(!m_pDM || !m_pCM || !m_pGrid ) {
		AfxMessageBox( _T("DM,CM,Grid Error : ����_����_�μ������ڷ�_�켱����") );
		return FALSE;
	}	
	
	RefreshAll();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSeDepartmentPriority::RefreshAll()
{
	
	m_pDM->RefreshDataManager(_T(""));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("�ڷ�Ǳ���"),_T("UDF_�μ��ڵ�"));
	m_pGrid->Display();

}


VOID CSeDepartmentPriority::OnOK() 
{
	// TODO: Add extra validation here
	// �߰� ��ư
	CString strCode;
	CString strCompare;
	m_pCM->GetControlMgrData(_T("�μ��ڵ�"),strCode);
	
	INT nRowCnt = m_pDM->GetRowCount();

	for(INT i=0;i<nRowCnt;i++)
	{
		strCompare = m_pDM->GetCellData(_T("�μ��ڵ�"),i);
		if(strCompare == strCode)
		{
			AfxMessageBox(_T("�̹� �߰��Ǿ� �ִ� �μ��Դϴ�!"));
			return;
		}

	}
	
	m_pDM->InsertRow(-1);
	nRowCnt = m_pDM->GetRowCount();
	INT nRowIdx = nRowCnt -1;
	CString strDesc;
	

	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�ڷ�Ǳ���"),strCode,strDesc);
	m_pDM->SetCellData(_T("�μ��ڵ�"),strCode,nRowIdx);
	m_pDM->SetCellData(_T("UDF_�μ��ڵ�"),strDesc,nRowIdx);
	m_pDM->SetCellData(_T("UDF_����"),_T("I"),nRowIdx);
	
	m_pGrid->Display();

	return;
	CDialog::OnOK();
}

VOID CSeDepartmentPriority::OnbtnDepPriorityDelete() 
{
	// TODO: Add your control notification handler code here
	// ���� ��ư
	
	CString strUDF;
	CString strREC_KEY;
	INT nRowCnt = m_pDM->GetRowCount();
	if(0 == nRowCnt) return;

	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("������ �׸��� �����ϴ�!"));
		return;
	}

	strUDF = m_pDM->GetCellData(_T("UDF_����"),m_nCurrentIdx);

	if(_T("I") == strUDF)
	{
		m_pDM->DeleteRow(m_nCurrentIdx);
	}
	else
	{
		strREC_KEY = m_pDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);

		if(!strREC_KEY.IsEmpty()) m_arrayDeleteKey.Add(strREC_KEY);

		m_pDM->DeleteRow(m_nCurrentIdx);
	}


	nRowCnt = m_pDM->GetRowCount();

	if(nRowCnt>0)
	{
		m_nCurrentIdx = 0;
	}
	else if(0 == nRowCnt)
	{
		m_nCurrentIdx = -1;
	}

	m_pGrid->Display();

	AfxMessageBox(_T("�����Ͽ����ϴ�!"));


}

VOID CSeDepartmentPriority::OnbtnDepPriorityUp() 
{
	// TODO: Add your control notification handler code here
	
	//���� 
	INT nRowCnt = m_pDM->GetRowCount();
	if(0 == nRowCnt) return;

	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("������ �׸��� �����ϴ�!"));
		return;
	}

		
	if(0 == m_nCurrentIdx)
	{
		AfxMessageBox(_T("�ֻ��� �׸��Դϴ�!"));
		return;
	}

	m_pDM->SORT_SwapRow(m_nCurrentIdx,m_nCurrentIdx-1);
	m_nCurrentIdx--;
	m_pGrid->Display();
	m_pGrid->SetReverse(m_nCurrentIdx);


}

VOID CSeDepartmentPriority::OnbtnDepPriorityDown() 
{
	// TODO: Add your control notification handler code here
	
	//�Ʒ���
	INT nRowCnt = m_pDM->GetRowCount();
	if(0 == nRowCnt) return;

	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("������ �׸��� �����ϴ�!"));
		return;
	}

		
	if(nRowCnt -1 == m_nCurrentIdx)
	{
		AfxMessageBox(_T("������ �׸��Դϴ�!"));
		return;
	}

	m_pDM->SORT_SwapRow(m_nCurrentIdx,m_nCurrentIdx+1);
	m_nCurrentIdx++;
	m_pGrid->Display();
	m_pGrid->SetReverse(m_nCurrentIdx);
}

BEGIN_EVENTSINK_MAP(CSeDepartmentPriority, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeDepartmentPriority)
	ON_EVENT(CSeDepartmentPriority, IDC_gridDepOrderPriority, -600 /* Click */, OnClickgridDepOrderPriority, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeDepartmentPriority::OnClickgridDepOrderPriority() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			m_nCurrentIdx = row-1;
		}
	
	}
}

VOID CSeDepartmentPriority::OnbtnDepPrioritySave() 
{
	// TODO: Add your control notification handler code here
	
	INT nRowCnt = m_pDM->GetRowCount();
	INT nSize = m_arrayDeleteKey.GetSize();
	if(0 == nRowCnt && 0 == nSize) 
	{
		AfxMessageBox(_T("������ ������ �����ϴ�!"));
		return;

	}

	CString strPriority;
	CString strUDF;
	CString strREC_KEY;
	CArray<CString,CString> RemoveAliasList;
	RemoveAliasList.Add(_T("UDF_�μ��ڵ�"));

	m_pDM->StartFrame();

	for(INT i=0;i<nRowCnt;i++)
	{
		strPriority.Format(_T("%d"),i+1);
		m_pDM->SetCellData(_T("�켱����"),strPriority,i);

		strUDF = m_pDM->GetCellData(_T("UDF_����"),i);

		if(_T("I") == strUDF)
		{
			m_pDM->MakeRecKey(strREC_KEY);
			if(!strREC_KEY.IsEmpty())
			{
				m_pDM->SetCellData(_T("REC_KEY"),strREC_KEY,i);
				m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("SE_DEPARTMENT_PRIORITY_TBL"), i , m_pDM);
			}
		}
		else
		{
			strREC_KEY = m_pDM->GetCellData(_T("REC_KEY"),i);
			if(!strREC_KEY.IsEmpty())
			{
				m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("SE_DEPARTMENT_PRIORITY_TBL"), i , m_pDM, _T("REC_KEY"), _T("NUMERIC"), strREC_KEY);
			
			}

		}


	}

	CString strQuery;

	for(i=0;i<nSize;i++)
	{
		strREC_KEY = m_arrayDeleteKey.GetAt(i);
		if(!strREC_KEY.IsEmpty())
		{
			strQuery.Format(_T("DELETE FROM SE_DEPARTMENT_PRIORITY_TBL WHERE REC_KEY = %s;"),strREC_KEY);
			m_pDM->AddFrame(strQuery);
		}

	}

	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();

	if(ids<0)
	{
		AfxMessageBox(_T("���忡 �����Ͽ����ϴ�!"));
	
	}
	else
	{
		AfxMessageBox(_T("�����Ͽ����ϴ�!"));

	}

	RefreshAll();
}

HBRUSH CSeDepartmentPriority::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
