// IndexGroup.cpp : implementation file
//

#include "stdafx.h"
#include "IndexGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexGroup dialog


CIndexGroup::CIndexGroup(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexGroup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_strGroupCode = _T("");

}


VOID CIndexGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexGroup)
	DDX_Control(pDX, IDC_listGroup, m_listGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexGroup, CDialog)
	//{{AFX_MSG_MAP(CIndexGroup)
	ON_LBN_DBLCLK(IDC_listGroup, OnDblclklistGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexGroup message handlers

BOOL CIndexGroup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("��ũ����_�׷�") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ��ũ����_�׷�") );
		return FALSE;
	}

	m_pDM = FindDM(_T("DM_��ũ����_�׷�"));
	if(!m_pDM)
	{
		AfxMessageBox( _T("reference Error : DM_��ũ����_�׷�") );
		return FALSE;		
	}

	ShowGroupList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CIndexGroup::ShowGroupList()
{
	m_pDM->RefreshDataManager(_T(""));

	// List Box�ʱ�ȭ 
	m_listGroup.ResetContent();

	INT nRow;
	nRow = m_pDM->GetRowCount();
	if (nRow == 0) {
		AfxMessageBox(_T("�׷� ������ �����ϴ�."));
		return;
	}

	// ����Ÿ�� ���
	CString groupcode;
	for(INT i=0;i<nRow;i++) {
	    groupcode.Format(_T("�� %s"), m_pDM->GetCellData(_T("MARC_GROUP_CODE"),i));
		m_listGroup.InsertString(i,groupcode);  
	}
}

VOID CIndexGroup::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	INT nRow = m_pDM->GetRowCount();
	INT SelectedRow = m_listGroup.GetCurSel();
	if (nRow <= SelectedRow-1) {
		AfxMessageBox(_T("������ �߸��Ǿ����ϴ�."));
		return;
	}
  
	m_strGroupCode = m_pDM->GetCellData(_T("MARC_GROUP_CODE"),SelectedRow);
	
	if(m_strGroupCode.IsEmpty())
	{
		AfxMessageBox(_T("������ �߸��Ǿ����ϴ�."));
		return;
	}

	CDialog::OnOK();
}

VOID CIndexGroup::OnDblclklistGroup() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}
