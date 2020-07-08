// SeMemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeMemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SeMemoDlg dialog


SeMemoDlg::SeMemoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(SeMemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SeMemoDlg)
	m_strMEMO = _T("");
	//}}AFX_DATA_INIT
	AfxInitRichEdit();

	m_pParentGrid = NULL;
	m_pDM = NULL;
	m_nCurrentIdx = -1;
	m_strSPECIES_KEY = _T("");
	
	m_nMoveOption = 1; //default

}
SeMemoDlg::~SeMemoDlg()
{
}

VOID SeMemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SeMemoDlg)
	DDX_Text(pDX, IDC_richeditSE_MEMO, m_strMEMO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SeMemoDlg, CDialog)
	//{{AFX_MSG_MAP(SeMemoDlg)
	ON_BN_CLICKED(IDC_btnMEMO_PREV, OnbtnMEMOPREV)
	ON_BN_CLICKED(IDC_btnMEMO_NEXT, OnbtnMEMONEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SeMemoDlg message handlers

BOOL SeMemoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	
	/*
	m_pParentGrid->SelectMakeList();
	m_nCurrentIdx = m_pParentGrid->SelectGetHeadPosition();
	*/

	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		EndDialog(0);
		return FALSE;
	}
	
	if(!m_pDM)
	{
		AfxMessageBox(_T("DM�� ã�� �� �����ϴ�!"));
		EndDialog(0);
		return FALSE;
	}

	ShowData();

	CRichEditCtrl* pRichEditCtrl = (CRichEditCtrl*)GetDlgItem(IDC_richeditSE_MEMO);
	pRichEditCtrl->LineLength(48);


	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT SeMemoDlg::ShowData()
{
	m_strSPECIES_KEY = m_pDM->GetCellData(_T("ISS_��KEY"),m_nCurrentIdx);
	
	if(m_strSPECIES_KEY.IsEmpty()) {
		AfxMessageBox(_T("��KEY�� ã�� �� �����ϴ�!"));
		return -1;
	}

	CString strQuery;
	strQuery.Format(_T("SELECT NOTE FROM SE_SPECIES_TBL WHERE REC_KEY = %s"),m_strSPECIES_KEY);
	m_pDM->GetOneValueQuery(strQuery,m_strMEMO);
	UpdateData(FALSE);
	
	m_pParentGrid->SetReverse(m_nCurrentIdx);

	return 0;
}

VOID SeMemoDlg::OnbtnMEMOPREV() 
{
	// TODO: Add your control notification handler code here
	
	if(1 == m_nMoveOption)
	{
		m_nCurrentIdx--;

		if(m_nCurrentIdx < 0)
		{
			AfxMessageBox(_T("ó�� �׸��Դϴ�!"));
			m_nCurrentIdx++;
			return;
		}
		
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetPrev();

		if(nIdx<0) 
		{
			INT nHeadIdx = m_pParentGrid->SelectGetHeadPosition();
			
			if(m_nCurrentIdx == nHeadIdx)
			{
				AfxMessageBox(_T("ó�� �׸��Դϴ�!"));
				m_pParentGrid->SelectGetHeadPosition();
				return;
			}
			else //���� ���� ������ ���� ��� �� ���� ���������� ���Ѵ� 
			{
				m_nCurrentIdx = nHeadIdx;
			}
		
		}
		else //���� ������ �������� �����δ�
		{
			m_nCurrentIdx = nIdx;
		}

	}
	
	ShowData();

}

VOID SeMemoDlg::OnbtnMEMONEXT() 
{
	// TODO: Add your control notification handler code here
	
	if(1 == m_nMoveOption)
	{
		m_nCurrentIdx++;
		INT nMaxIdx = m_pDM->GetRowCount()-1;
		if(m_nCurrentIdx > nMaxIdx)
		{
			AfxMessageBox(_T("������ �׸��Դϴ�!"));
			m_nCurrentIdx--;
			return;
		}
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetNext();

		if(nIdx<0) 
		{
			INT nTailIdx = m_pParentGrid->SelectGetTailPosition();
			
			if(m_nCurrentIdx == nTailIdx)
			{
				AfxMessageBox(_T("������ �׸��Դϴ�!"));
				m_pParentGrid->SelectGetTailPosition();
				return;
			}
			else //���� ���� ������ ���� ��� �ǵ��� ���������� ���Ѵ� 
			{
				m_nCurrentIdx = nTailIdx;
			}
		}
		else //���� ������ �������� �����δ�
		{
			m_nCurrentIdx = nIdx;
		}
	}
	
	ShowData();

}

VOID SeMemoDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CString strQuery;
	
	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();
	strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET NOTE = '%s' WHERE REC_KEY = %s;"),m_strMEMO,m_strSPECIES_KEY);
	m_pDM->AddFrame(strQuery);
		
	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();

	if(ids<0) AfxMessageBox(_T("���忡 �����Ͽ����ϴ�!"));
	else AfxMessageBox(_T("�����Ͽ����ϴ�!"));
	
	return;

	CDialog::OnOK();
}

HBRUSH SeMemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
