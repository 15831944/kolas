// GroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GroupDlg.h"
#include "GourpInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupDlg dialog


CGroupDlg::CGroupDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_strGroupCode = _T("");
}


VOID CGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupDlg)
	DDX_Control(pDX, IDC_listGourp, m_listGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupDlg)
	ON_LBN_DBLCLK(IDC_listGourp, OnDblclklistGourp)
	ON_BN_CLICKED(IDC_btnGroupModify, OnbtnGroupModify)
	ON_BN_CLICKED(IDC_btnGroupInsert, OnbtnGroupInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupDlg message handlers

BOOL CGroupDlg::OnInitDialog() 
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

VOID CGroupDlg::ShowGroupList()
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



VOID CGroupDlg::OnOK() 
{
	// TODO: Add extra validation here
	// �׷� ����

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

VOID CGroupDlg::OnDblclklistGourp() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

VOID CGroupDlg::OnbtnGroupInsert() 
{
	// TODO: Add your control notification handler code here
	
	//�׷� �Է�
	CGourpInfoDlg dlg;
	dlg.m_nOpenMode = 1;
	if(IDOK == dlg.DoModal()) ShowGroupList();

}

VOID CGroupDlg::OnbtnGroupModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	INT nRow = m_pDM->GetRowCount();
	INT SelectedRow = m_listGroup.GetCurSel();
	if (nRow <= SelectedRow-1) {
		AfxMessageBox(_T("������ �߸��Ǿ����ϴ�."));
		return;
	}

	CString strPK = m_pDM->GetCellData(_T("PK"),SelectedRow);

	//�׷� ����
	CGourpInfoDlg dlg;
	dlg.m_nOpenMode = -1;
	dlg.m_strPK = strPK;
	if(IDOK == dlg.DoModal()) ShowGroupList();

}


BOOL CGroupDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE)
	{
        DeleteMarcGroup();
	}

	return CDialog::PreTranslateMessage(pMsg);
}

INT CGroupDlg::DeleteMarcGroup()
{
    
	INT nRow = m_pDM->GetRowCount();
	INT SelectedRow = m_listGroup.GetCurSel();
	if (nRow <= SelectedRow-1) {
		AfxMessageBox(_T("������ �߸��Ǿ����ϴ�."));
		return 0;
	}

    if(nRow == 0) return 0;
   
	CString tmpGroupCode,tmpPK;
	tmpPK = m_pDM->GetCellData(_T("PK"),SelectedRow);
	tmpGroupCode = m_pDM->GetCellData(_T("MARC_GROUP_CODE"),SelectedRow);
  
	CString msg;
	msg.Format(_T("�׷� ������ ��� ���� �±׿� ���赵 �����˴ϴ�. '%s' �׷��� ������ �����Ͻðڽ��ϱ�?"),tmpGroupCode);
    if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;
  
	
	CString query;

	m_pDM->StartFrame();
	
	query.Format(_T("DELETE FROM MARC_FIRST_INDICATOR_TBL WHERE MARC_TAG_ALIAS IN ")
				 _T("(SELECT ALIAS FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s');"),tmpGroupCode);
	m_pDM->AddFrame(query);


	query.Format(_T("DELETE FROM MARC_SECOND_INDICATOR_TBL WHERE MARC_TAG_ALIAS IN ")
				 _T("(SELECT ALIAS FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s');"),tmpGroupCode);
	m_pDM->AddFrame(query);

	query.Format(_T("DELETE FROM MARC_SUBFIELD_TBL WHERE MARC_TAG_ALIAS IN ")
				 _T("(SELECT ALIAS FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s');"),tmpGroupCode);
	m_pDM->AddFrame(query);
 

	query.Format(_T("DELETE FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s';"), tmpGroupCode);
    m_pDM->AddFrame(query);
      

    query.Format(_T("DELETE FROM MARC_RELATION_TBL WHERE MARC_GROUP_CODE = '%s';"), tmpGroupCode);
    m_pDM->AddFrame(query);
      
 
	query.Format(_T("DELETE FROM MARC_GROUP_TBL WHERE PK = %s;"), tmpPK);
    m_pDM->AddFrame(query);
	
	INT ids = m_pDM->SendFrame();
    m_pDM->EndFrame();
	if(ids<0) AfxMessageBox(_T("�����ϴ� �� ������ �߻��Ͽ����ϴ�"));
	else AfxMessageBox(_T("�����Ͽ����ϴ�"));
	
	ShowGroupList(); //refresh


    return 0;
}
