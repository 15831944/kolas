// MarcRefMain.cpp : implementation file
//

#include "stdafx.h"
#include "MarcRefMain.h"
#include "GroupDlg.h"
#include "TagDlg.h"
#include "../../esl/kolas3_main/kolas3_mainapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcRefMain dialog


CMarcRefMain::CMarcRefMain(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcRefMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM_TAG = NULL;
	m_pDM_RELATION = NULL;
	m_pGridTag = NULL;
	m_pGridRelation = NULL;


	m_pFirstForm = NULL;
	m_pSecondForm = NULL;
	m_pThirdForm = NULL;
	m_pRelationForm = NULL;
	m_nBeforeTabCursel = -1;

	m_strMarcGroup = _T("");

	m_nCurrentIdxTag = -1;
	m_nCurrentIdxRelation = -1;
}

CMarcRefMain::~CMarcRefMain()
{
	FreeChildForm();
}

VOID CMarcRefMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcRefMain)
	DDX_Control(pDX, IDC_tabSearch, m_ctrlTabSearch);
	DDX_Control(pDX, IDC_tabMain, m_ctrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcRefMain, CDialog)
	//{{AFX_MSG_MAP(CMarcRefMain)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabMain, OnSelchangetabMain)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabSearch, OnSelchangetabSearch)
	ON_BN_CLICKED(IDC_btnSearch, OnbtnSearch)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcRefMain message handlers

BOOL CMarcRefMain::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CMarcRefMain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	if(InitESL_Mgr( _T("��ũ����_����") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ��ũ����_����") );
		return false;
	}

	m_pCM = FindCM(_T("CM_��ũ����_����"));
	m_pGridTag = (CESL_Grid*)m_pCM->FindControl(_T("�±װ˻��׸���"));
	m_pGridRelation = (CESL_Grid*)m_pCM->FindControl(_T("����˻��׸���"));
	m_pDM_TAG = FindDM(_T("DM_��ũ����_�±װ˻�"));
	m_pDM_RELATION = FindDM(_T("DM_��ũ����_����˻�"));

	if(!m_pCM || !m_pGridTag || !m_pGridRelation || !m_pDM_TAG || !m_pDM_RELATION) {
		AfxMessageBox( _T("Reference Error : ��ũ����_����") );
		return false;
	}
	

	m_ctrlTab.InsertItem(0,_T("�±������� ���ñ�ȣ"));
	m_ctrlTab.InsertItem(1,_T("�ĺ���ȣ"));
	m_ctrlTab.InsertItem(2,_T("����� ����"));

	m_ctrlTabSearch.InsertItem(0,_T("�±�"));
	m_ctrlTabSearch.InsertItem(1,_T("����"));

	CreateChildForm();

	ToolBarChange(0);

	//Ÿ�̸� ������ ���� �׷��� �����ϰ� �Ѵ� 
	SetTimer(0,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CMarcRefMain::CreateChildForm()
{
	
	m_pFirstForm = new CTabFirstForm(this);
	m_pFirstForm->Create(this);
	
	m_pSecondForm = new CTabSecondForm(this);
	m_pSecondForm->Create(this);

	m_pThirdForm = new CTabThirdForm(this);
	m_pThirdForm->Create(this);

	m_pRelationForm = new CRelationForm(this);
	m_pRelationForm->Create(this);
	
	CRect rect;
	this->GetClientRect(rect);
	
	rect.top += 20;
	rect.bottom -= 20;
	rect.left += 310;
	rect.right -= 10;

	
	m_pFirstForm->MoveWindow(rect);
	m_pSecondForm->MoveWindow(rect);
	m_pThirdForm->MoveWindow(rect);
	

	rect.top -= 20;
	rect.bottom += 20; 
	rect.right += 10;
	
	m_pRelationForm->MoveWindow(rect);


	m_pFirstForm->ShowWindow(TRUE);
	m_pSecondForm->ShowWindow(FALSE);
	m_pThirdForm->ShowWindow(FALSE);
	m_pRelationForm->ShowWindow(FALSE);

}

VOID CMarcRefMain::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	//���� �� 
	CWnd *pWnd = (CWnd*)GetDlgItem(IDC_tabMain);
	if(pWnd->GetSafeHwnd() == NULL) return;

	pWnd->MoveWindow(300, 0, cx, cy);	


	//�˻� �� 
	pWnd = (CWnd*)GetDlgItem(IDC_tabSearch);
	if(pWnd->GetSafeHwnd() == NULL) return;
	 
	pWnd->MoveWindow(0, 0, 295, cy);	


	CRect rect,tmprect;
	this->GetClientRect(rect);
	

	//�±� �˻� �׸��� 
	tmprect = rect;
	
	pWnd = (CWnd*)GetDlgItem(IDC_gridTagSearch);
	if(pWnd->GetSafeHwnd() == NULL) return;

	pWnd->MoveWindow(10, 110, 273, cy-120);	
	

	//���� �˻� �׸���
	pWnd = (CWnd*)GetDlgItem(IDC_gridRelationSearch);
	if(pWnd->GetSafeHwnd() == NULL) return;

	pWnd->MoveWindow(10, 110, 273, cy-120);	


	//child form
	tmprect = rect;

	tmprect.top += 20;
	tmprect.bottom -= 20;
	tmprect.left += 310;
	tmprect.right -= 10;


	m_pFirstForm->ResizeWindow(tmprect);
	m_pSecondForm->ResizeWindow(tmprect);
	m_pThirdForm->ResizeWindow(tmprect);
	m_pRelationForm->ResizeWindow(rect);


}

VOID CMarcRefMain::FreeChildForm()
{
	if(m_pFirstForm) delete m_pFirstForm;
	m_pFirstForm = NULL;

	if(m_pSecondForm) delete m_pSecondForm;
	m_pSecondForm = NULL;

	if(m_pThirdForm) delete m_pThirdForm;
	m_pThirdForm = NULL;

	if(m_pRelationForm) delete m_pRelationForm;
	m_pRelationForm = NULL;

}

VOID CMarcRefMain::OnSelchangetabMain(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	INT nCurSel = m_ctrlTab.GetCurSel();
	switch (nCurSel)
	{

	case 0:
			m_pFirstForm->ShowWindow(TRUE);
			m_pSecondForm->ShowWindow(FALSE);
			m_pThirdForm->ShowWindow(FALSE);
			break;

	case 1:
			m_pFirstForm->ShowWindow(FALSE);
			m_pSecondForm->ShowWindow(TRUE);
			m_pThirdForm->ShowWindow(FALSE);
			break;

	case 2:
			m_pFirstForm->ShowWindow(FALSE);
			m_pSecondForm->ShowWindow(FALSE);
			m_pThirdForm->ShowWindow(TRUE);
			break;

	}	

	*pResult = 0;
}

VOID CMarcRefMain::OnSelchangetabSearch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	//�±� - ���� ���� �ٲ� 
	
	INT nCurSel = m_ctrlTabSearch.GetCurSel();
	
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_tabMain);

	if(0 == nCurSel)
	{
		if(2 == m_nBeforeTabCursel)
		{
			m_pFirstForm->ShowWindow(FALSE);
			m_pSecondForm->ShowWindow(FALSE);
			m_pThirdForm->ShowWindow(TRUE);
		}
		else if(1 == m_nBeforeTabCursel)
		{
			m_pFirstForm->ShowWindow(FALSE);
			m_pSecondForm->ShowWindow(TRUE);
			m_pThirdForm->ShowWindow(FALSE);
		}
		else
		{
			m_pFirstForm->ShowWindow(TRUE);
			m_pSecondForm->ShowWindow(FALSE);
			m_pThirdForm->ShowWindow(FALSE);
		}


		m_pRelationForm->ShowWindow(FALSE);
		pWnd->ShowWindow(TRUE);

		GetDlgItem(IDC_stcMainCode)->SetWindowText(_T("�ڵ�"));
        GetDlgItem(IDC_stcMainName)->SetWindowText(_T("�̸�"));
		
		m_pGridTag->ShowWindow(TRUE);
		m_pGridRelation->ShowWindow(FALSE);

	}
	else if(1 == nCurSel)
	{
		m_nBeforeTabCursel = m_ctrlTab.GetCurSel();
		
		m_pFirstForm->ShowWindow(FALSE);
		m_pSecondForm->ShowWindow(FALSE);
		m_pThirdForm->ShowWindow(FALSE);
		m_pRelationForm->ShowWindow(TRUE);
		pWnd->ShowWindow(FALSE);


		GetDlgItem(IDC_stcMainCode)->SetWindowText(_T("��ȣ"));
        GetDlgItem(IDC_stcMainName)->SetWindowText(_T("����"));

		m_pGridTag->ShowWindow(FALSE);
		m_pGridRelation->ShowWindow(TRUE);

	}
	
	ToolBarChange(nCurSel);

	*pResult = 0;
}

VOID CMarcRefMain::ToolBarChange(INT nCurSel)
{
	
	const INT tagcnt = 11;
	CString strTagButton[tagcnt] = {
		_T("�±��Է�"),
		_T("�±׼���"),
		_T("�±׻���"),
		_T("���ñ�ȣ�Է�"),
		_T("���ñ�ȣ����"),
		_T("���ñ�ȣ����"),
		_T("�ĺ���ȣ�Է�"),
		_T("�ĺ���ȣ����"),
		_T("�ĺ���ȣ����"),
		_T("S1"),
		_T("S2")};


	const INT relationcnt = 3;
	CString strRelationButton[relationcnt] = {
		_T("�����Է�"),
		_T("�������"),
		_T("�������")};



	if(0 == nCurSel)
	{
		for(INT i=0;i<tagcnt;i++)
		{
			pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strTagButton[i] , FALSE);
		}
		
		for(INT j=0;j<relationcnt;j++)
		{
			pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strRelationButton[j] , TRUE);
		}

	}
	else
	{

		for(INT i=0;i<tagcnt;i++)
		{
			pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strTagButton[i] , TRUE);
		}
		
		for(INT j=0;j<relationcnt;j++)
		{
			pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strRelationButton[j] , FALSE);
		}
	}



/*
		CString strRemoveButton;
		strRemoveButton = _T("RFID���");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strRemoveButton , TRUE);
		CArray <CString, CString> RemoveListArray;
		RemoveListArray.RemoveAll();
		m_pGrid->InitPopupMenuItem();
		RemoveListArray.Add(_T("RFID���"));
		m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
*/


}


VOID CMarcRefMain::OnbtnSearch() 
{
	// TODO: Add your control notification handler code here

	if(m_strMarcGroup.IsEmpty())
	{
		AfxMessageBox(_T("���� ��ũ �׷��� ������ �ֽʽÿ�!"));
		return;
	}
	
	
	INT nCurSel = m_ctrlTabSearch.GetCurSel();
	
	CString strWhere,strTmp;
	
	INT nRowCnt = -1; 


	if(0 == nCurSel) //�±� �˻�
	{
	
		CString strTagCode,strTagName;
		m_pCM->GetControlMgrData(_T("�ڵ�"),strTagCode);
		m_pCM->GetControlMgrData(_T("�̸�"),strTagName);
	
       
		strTmp.Format(_T("MARC_GROUP_CODE = '%s'"),m_strMarcGroup);
		strWhere += strTmp;
	
	
		if(!strTagCode.IsEmpty())
		{
			strTmp.Format(_T(" AND TAG_CODE like '%%%s%%' "),strTagCode);
			strWhere += strTmp;
		}
		
		if(!strTagName.IsEmpty() )
		{
			strTmp.Format(_T("AND TAG_NAME like '%%%s%%' "),strTagName);
			strWhere += strTmp;
		}    
	
    
		m_pDM_TAG->RefreshDataManager(strWhere);
		m_pGridTag->Display();

		nRowCnt = m_pDM_TAG->GetRowCount();

		m_nCurrentIdxTag  = -1;
	
	}
	else if(1 == nCurSel) //���� �˻�
	{

		CString strTagCode,strTagName;
		m_pCM->GetControlMgrData(_T("�ڵ�"),strTagCode);
		m_pCM->GetControlMgrData(_T("�̸�"),strTagName);
	
       
		strTmp.Format(_T("MARC_GROUP_CODE = '%s'"),m_strMarcGroup);
		strWhere += strTmp;
	
	
		if(!strTagCode.IsEmpty())
		{
			strTmp.Format(_T(" AND OPERATOR like '%%%s%%' "),strTagCode);
			strWhere += strTmp;
		}
		
		if(!strTagName.IsEmpty() )
		{
			strTmp.Format(_T("AND BASIC_PARAM like '%%%s%%' "),strTagName);
			strWhere += strTmp;
		}    
	
    
		m_pDM_RELATION->RefreshDataManager(strWhere);
		m_pGridRelation->Display();

		nRowCnt = m_pDM_RELATION->GetRowCount();

		m_nCurrentIdxRelation = -1;

	}

	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("�˻������ �����ϴ�!"));
	}


}

VOID CMarcRefMain::ShowGroupMgr()
{
	CGroupDlg dlg(this);
	if(dlg.DoModal() != IDOK) return;
	
	m_strMarcGroup = dlg.m_strGroupCode;
	m_pCM->SetControlMgrData(_T("�׷��ڵ�"),m_strMarcGroup);

}

VOID CMarcRefMain::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0)
	{
        KillTimer(0);
		ShowGroupMgr();
	}

	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CMarcRefMain, CDialog)
    //{{AFX_EVENTSINK_MAP(CMarcRefMain)
	ON_EVENT(CMarcRefMain, IDC_gridTagSearch, -600 /* Click */, OnClickgridTagSearch, VTS_NONE)
	ON_EVENT(CMarcRefMain, IDC_gridRelationSearch, -600 /* Click */, OnClickgridRelationSearch, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMarcRefMain::OnClickgridTagSearch() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM_TAG->GetRowCount()) return;

	INT row = m_pGridTag->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdxTag = row-1;
	}

	ShowAllTagData();
}

VOID CMarcRefMain::ShowAllTagData()
{
	if(m_nCurrentIdxTag<0) return;
	
	CString strTagAlias;
	CString strCode = m_pDM_TAG->GetCellData(_T("TAG_CODE"),m_nCurrentIdxTag);
	strTagAlias.Format(_T("%s_%s"),m_strMarcGroup,strCode);

	if(m_pFirstForm)
	{
		CString strName,strRepeat,strEssential;
		strName = m_pDM_TAG->GetCellData(_T("TAG_NAME"),m_nCurrentIdxTag);
		strRepeat = m_pDM_TAG->GetCellData(_T("REPEAT_TYPE"),m_nCurrentIdxTag);
		strEssential = m_pDM_TAG->GetCellData(_T("ESSENTIAL_TYPE"),m_nCurrentIdxTag);

		m_pFirstForm->SetAllData(strTagAlias,
								 strCode,
								 strName,
								 strRepeat,
								 strEssential);
	
		m_pFirstForm->ShowAllData();
	}
	
	if(m_pSecondForm)
	{
		m_pSecondForm->SetAllData(strTagAlias);
		m_pSecondForm->ShowAllData();
	}

	if(m_pThirdForm)
	{
		CString strPK;
		strPK = m_pDM_TAG->GetCellData(_T("PK"),m_nCurrentIdxTag);
		m_pThirdForm->SetAllData(strPK);
		m_pThirdForm->ShowAllData();
	}
	

}
VOID CMarcRefMain::OnClickgridRelationSearch() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM_RELATION->GetRowCount()) return;

	INT row = m_pGridRelation->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdxRelation = row-1;
	}

	ShowAllRelationData();

}

VOID CMarcRefMain::ShowAllRelationData()
{
	if(m_nCurrentIdxRelation<0) return;

	CString strBasicParam,strOperator,strParam,strDesc;
	strBasicParam = m_pDM_RELATION->GetCellData(_T("BASIC_PARAM"),m_nCurrentIdxRelation);
	strOperator = m_pDM_RELATION->GetCellData(_T("OPERATOR"),m_nCurrentIdxRelation);
	strParam = m_pDM_RELATION->GetCellData(_T("PARAM"),m_nCurrentIdxRelation);
	strDesc = m_pDM_RELATION->GetCellData(_T("DESCRIP"),m_nCurrentIdxRelation);

	m_pRelationForm->SetAllData(strBasicParam,
								strOperator,
								strParam,
								strDesc);

	m_pRelationForm->ShowAllData();

}

VOID CMarcRefMain::InsertTag()
{
	if(m_strMarcGroup.IsEmpty())
	{
		AfxMessageBox(_T("�׷��� ���� ���� �ʾҽ��ϴ�!"));
		return;
	}

	//�±� �Է�
	CTagDlg dlg;
	dlg.m_nOpenMode = 1;
	dlg.m_strMarcGroup = m_strMarcGroup;
	dlg.m_pParentDM = m_pDM_TAG;
	dlg.m_pParentGrid = m_pGridTag;
	dlg.DoModal();
}

VOID CMarcRefMain::ModifyTag()
{
	if(m_nCurrentIdxTag<0)
	{
		AfxMessageBox(_T("������ �±װ� �����ϴ�!"));
		return;
	}

	CString strPK = m_pDM_TAG->GetCellData(_T("PK"),m_nCurrentIdxTag);

	//�±� ����
	CTagDlg dlg;
	dlg.m_nOpenMode = -1;
	dlg.m_strPK = strPK;
	dlg.m_pParentDM = m_pDM_TAG;
	dlg.m_pParentGrid = m_pGridTag;
	dlg.m_nCurrentParentIdx = m_nCurrentIdxTag;
	
	if(IDOK == dlg.DoModal())
	{
		ShowAllTagData();
	}

}

VOID CMarcRefMain::DeleteTag()
{
	if(m_nCurrentIdxTag<0)
	{
		AfxMessageBox(_T("������ �׸��� �����ϴ�!"));
		return;
	}

	CString strPK,strMARC_TAG_ALIAS,strTAG_CODE,strTAG_NAME;
   
	strPK = m_pDM_TAG->GetCellData(_T("PK"),m_nCurrentIdxTag);
	strTAG_CODE = m_pDM_TAG->GetCellData(_T("TAG_CODE"),m_nCurrentIdxTag);
    strTAG_NAME = m_pDM_TAG->GetCellData(_T("TAG_NAME"),m_nCurrentIdxTag);
	
	strMARC_TAG_ALIAS.Format(_T("%s_%s"),m_strMarcGroup,strTAG_CODE);

	CString msg;
	msg.Format(_T("�̸��� '%s' ��  %s �ڵ带  '%s' �׷쿡�� �����Ͻðڽ��ϱ�?"),strTAG_NAME,strTAG_CODE,m_strMarcGroup);
    if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;
	
	
	//delete query
	CString strQuery;
    m_pDM_TAG->StartFrame();
    
	strQuery.Format(_T("DELETE FROM MARC_FIRST_INDICATOR_TBL WHERE MARC_TAG_ALIAS = '%s';"),strMARC_TAG_ALIAS);
    m_pDM_TAG->AddFrame(strQuery);
    
	strQuery.Format(_T("DELETE FROM MARC_SECOND_INDICATOR_TBL WHERE MARC_TAG_ALIAS = '%s';"),strMARC_TAG_ALIAS);
    m_pDM_TAG->AddFrame(strQuery);
	
	strQuery.Format(_T("DELETE FROM MARC_SUBFIELD_TBL WHERE MARC_TAG_ALIAS = '%s';"),strMARC_TAG_ALIAS);
    m_pDM_TAG->AddFrame(strQuery);
	
	strQuery.Format(_T("DELETE FROM MARC_TAG_TBL WHERE PK = %s;"),strPK);
    m_pDM_TAG->AddFrame(strQuery);
    
	INT ids = m_pDM_TAG->SendFrame();
    m_pDM_TAG->EndFrame();

	if(ids<0) AfxMessageBox(_T("�����ϴ� �� ������ �߻��Ͽ����ϴ�"));
	else AfxMessageBox(_T("�����Ͽ����ϴ�!"));
	
	OnbtnSearch();  //refresh
}

VOID CMarcRefMain::InsertIndicator()
{
	INT nCurSel = m_ctrlTabSearch.GetCurSel();
	if(1 == nCurSel) {
		AfxMessageBox(_T("���ñ�ȣ�� �±� �ǿ��� �Է��� �� �ֽ��ϴ�"));
	}
	
	if(m_nCurrentIdxTag<0)
	{
		AfxMessageBox(_T("������ �±� �׸��� �����ϴ�!"));
		return;
	}

	if(m_pFirstForm) m_pFirstForm->InsertIndicator();


}

VOID CMarcRefMain::ModifyIndicator()
{
	INT nCurSel = m_ctrlTabSearch.GetCurSel();
	if(1 == nCurSel) {
		AfxMessageBox(_T("���ñ�ȣ�� �±� �ǿ��� ������ �� �ֽ��ϴ�"));
	}
	
	if(m_nCurrentIdxTag<0)
	{
		AfxMessageBox(_T("������ �±� �׸��� �����ϴ�!"));
		return;
	}

	if(m_pFirstForm) m_pFirstForm->ModifyIndicator();

}

VOID CMarcRefMain::DeleteIndicator()
{
	INT nCurSel = m_ctrlTabSearch.GetCurSel();
	if(1 == nCurSel) {
		AfxMessageBox(_T("���ñ�ȣ�� �±� �ǿ��� ������ �� �ֽ��ϴ�"));
	}

	if(m_nCurrentIdxTag<0)
	{
		AfxMessageBox(_T("������ �±� �׸��� �����ϴ�!"));
		return;
	}

	if(m_pFirstForm) m_pFirstForm->DeleteIndicator();

}

VOID CMarcRefMain::InsertSubfield()
{
	INT nCurSel = m_ctrlTabSearch.GetCurSel();
	if(1 == nCurSel) {
		AfxMessageBox(_T("�ĺ���ȣ�� �±� �ǿ��� �Է��� �� �ֽ��ϴ�"));
	}
	
	if(m_nCurrentIdxTag<0)
	{
		AfxMessageBox(_T("������ �±� �׸��� �����ϴ�!"));
		return;
	}

	if(m_pSecondForm) m_pSecondForm->InsertSubfield();


}

VOID CMarcRefMain::ModifySubfield()
{
	INT nCurSel = m_ctrlTabSearch.GetCurSel();
	if(1 == nCurSel) {
		AfxMessageBox(_T("�ĺ���ȣ�� �±� �ǿ��� ������ �� �ֽ��ϴ�"));
	}
	
	if(m_nCurrentIdxTag<0)
	{
		AfxMessageBox(_T("������ �±� �׸��� �����ϴ�!"));
		return;
	}

	if(m_pSecondForm) m_pSecondForm->ModifySubfield();

}

VOID CMarcRefMain::DeleteSubfield()
{
	INT nCurSel = m_ctrlTabSearch.GetCurSel();
	if(1 == nCurSel) {
		AfxMessageBox(_T("�ĺ���ȣ�� �±� �ǿ��� ������ �� �ֽ��ϴ�"));
	}

	if(m_nCurrentIdxTag<0)
	{
		AfxMessageBox(_T("������ �±� �׸��� �����ϴ�!"));
		return;
	}

	if(m_pSecondForm) m_pSecondForm->DeleteSubfield();

}