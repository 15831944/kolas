// BO_ACQ_OrderSerialNoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_OrderSerialNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_OrderSerialNoDlg dialog


CBO_ACQ_OrderSerialNoDlg::CBO_ACQ_OrderSerialNoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_OrderSerialNoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_OrderSerialNoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_arrSort1.Add(_T("����ó"));
	m_arrSort1.Add(_T("������"));
	m_arrSort1.Add(_T("������"));
	m_arrSort1.Add(_T("����"));
	m_arrSort1.Add(_T("�̿��󱸺�"));
	m_arrSort1.Add(_T("��������"));
	m_arrSort1.Add(_T("������"));
	m_arrSort2.Add(_T("������"));
	m_arrSort2.Add(_T("������"));
	m_arrSort2.Add(_T("����"));
	m_arrSort2.Add(_T("�̿��󱸺�"));
	m_arrSort2.Add(_T("��������"));
	m_arrSort2.Add(_T("������"));
	m_arrSort3.Add(_T("������"));
	m_arrSort3.Add(_T("����"));
	m_arrSort3.Add(_T("�̿��󱸺�"));
	m_arrSort3.Add(_T("��������"));
	m_arrSort3.Add(_T("������"));
	m_arrSort4.Add(_T("����"));
	m_arrSort4.Add(_T("�̿��󱸺�"));
	m_arrSort4.Add(_T("��������"));
	m_arrSort4.Add(_T("������"));
	m_pComboMgr = new CMulitComboBoxManager(this);
	m_arrSortCond.Add(_T("��������"));
	m_arrSortCond.Add(_T("��������"));
	m_MapSortData.SetAt(_T("����"), _T("IBS_��ǥ��"));
	m_MapSortData.SetAt(_T("������"), _T("BP_��������"));
	m_MapSortData.SetAt(_T("������"), _T("IBS_������"));
	m_MapSortData.SetAt(_T("�̿��󱸺�"), _T("IBS_�̿��󱸺�"));
	m_MapSortData.SetAt(_T("��������"), _T("BB_��������"));
	m_MapSortData.SetAt(_T("����ó"), _T("BP_����ó��ȣ"));
}

CBO_ACQ_OrderSerialNoDlg::~CBO_ACQ_OrderSerialNoDlg()
{
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
}

VOID CBO_ACQ_OrderSerialNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_OrderSerialNoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_OrderSerialNoDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_OrderSerialNoDlg)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_1, OnSelchangecmbORDERSORT1)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_2, OnSelchangecmbORDERSORT2)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_3, OnSelchangecmbORDERSORT3)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_4, OnSelchangecmbORDERSORT4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_OrderSerialNoDlg message handlers

BOOL CBO_ACQ_OrderSerialNoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_�ֹ��Ϸù�ȣ")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_�ֹ��Ϸù�ȣ"));	
	if(!Init())
	{
		ESLAfxMessageBox(_T("ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_OrderSerialNoDlg::OnSelchangecmbORDERSORT1() 
{
	// TODO: Add your control notification handler code here
	CString strCurSel, strTmpData;
	m_pCM->GetControlMgrData(_T("��1����"), strCurSel);
	m_arrSort2.RemoveAll();
	INT cnt = m_arrSort1.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = m_arrSort1.GetAt(i);
		if(strTmpData == strCurSel && strTmpData != _T("������")) continue;
		else m_arrSort2.Add(strTmpData);
	}
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort2.GetAt(0), &m_arrSort2, _T("��2����"));
	OnSelchangecmbORDERSORT2();
}

VOID CBO_ACQ_OrderSerialNoDlg::OnSelchangecmbORDERSORT2() 
{
	// TODO: Add your control notification handler code here
	CString strCurSel, strTmpData;
	m_pCM->GetControlMgrData(_T("��2����"), strCurSel);
	m_arrSort3.RemoveAll();
	INT cnt = m_arrSort2.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = m_arrSort2.GetAt(i);
		if(strTmpData == strCurSel && strTmpData != _T("������")) continue;
		else m_arrSort3.Add(strTmpData);
	}
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort3.GetAt(0), &m_arrSort3, _T("��3����"));
	OnSelchangecmbORDERSORT3();	
}

VOID CBO_ACQ_OrderSerialNoDlg::OnSelchangecmbORDERSORT3() 
{
	// TODO: Add your control notification handler code here
	CString strCurSel, strTmpData;
	m_pCM->GetControlMgrData(_T("��3����"), strCurSel);
	m_arrSort4.RemoveAll();
	INT cnt = m_arrSort3.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = m_arrSort3.GetAt(i);
		if(strTmpData == strCurSel && strTmpData != _T("������")) continue;
		else m_arrSort4.Add(strTmpData);
	}
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort4.GetAt(0), &m_arrSort4, _T("��4����"));
}

VOID CBO_ACQ_OrderSerialNoDlg::OnSelchangecmbORDERSORT4() 
{
	// TODO: Add your control notification handler code here
	
}

VOID CBO_ACQ_OrderSerialNoDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strTmpData, strAlias, strTmpCond;
	m_pCM->GetControlMgrData(_T("��1����"), strTmpData);
	if(strTmpData != _T("������")) 
	{
		m_pCM->GetControlMgrData(_T("��1��������"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strAlias);
		m_SortCondition.m_strFirstAlias = strAlias;
		if(strTmpCond == _T("��������")) m_SortCondition.m_nFirstType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nFirstType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nFirstType = 0;

	m_pCM->GetControlMgrData(_T("��2����"), strTmpData);
	if(strTmpData != _T("������")) 
	{
		m_pCM->GetControlMgrData(_T("��2��������"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strAlias);
		m_SortCondition.m_strSecondAlias= strAlias;
		if(strTmpCond == _T("��������")) m_SortCondition.m_nSecondType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nSecondType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nSecondType = 0;

	m_pCM->GetControlMgrData(_T("��3����"), strTmpData);
	if(strTmpData != _T("������")) 
	{
		m_pCM->GetControlMgrData(_T("��3��������"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strAlias);
		m_SortCondition.m_strThirdAlias = strAlias;
		if(strTmpCond == _T("��������")) m_SortCondition.m_nThirdType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nThirdType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nThirdType = 0;
	
	m_pCM->GetControlMgrData(_T("��4����"), strTmpData);
	if(strTmpData != _T("������")) 
	{
		m_pCM->GetControlMgrData(_T("��4��������"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strAlias);
		m_SortCondition.m_strFourthAlias = strAlias;
		if(strTmpCond == _T("��������")) m_SortCondition.m_nFourthType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nFourthType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nFourthType = 0;
	
	CDialog::OnOK();
}

BOOL CBO_ACQ_OrderSerialNoDlg::Init()
{
	SetComboData();
	return TRUE;
}

VOID CBO_ACQ_OrderSerialNoDlg::SetComboData()
{
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort1.GetAt(0), &m_arrSort1, _T("��1����"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort2.GetAt(0), &m_arrSort2, _T("��2����"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort3.GetAt(0), &m_arrSort3, _T("��3����"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSort4.GetAt(0), &m_arrSort4, _T("��4����"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSortCond.GetAt(0), &m_arrSortCond, _T("��1��������"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSortCond.GetAt(0), &m_arrSortCond, _T("��2��������"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSortCond.GetAt(0), &m_arrSortCond, _T("��3��������"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrSortCond.GetAt(0), &m_arrSortCond, _T("��4��������"));
	
}

CString CBO_ACQ_OrderSerialNoDlg::GetFieldName(CString strAlias)
{
	CString strFieldName;
	if(strAlias == _T("������")) strFieldName = _T("P.SUBJECT_CODE");
	else if(strAlias == _T("����ó")) strFieldName = _T("P.PROVIDER_CODE");
	else if(strAlias == _T("������")) strFieldName = _T("I.PUBLISHER");
	else if(strAlias == _T("����")) strFieldName = _T("I.TITLE");
	else if(strAlias == _T("�̿��󱸺�")) strFieldName = _T("I.USE_OBJ_CODE");
	else if(strAlias == _T("��������")) strFieldName = _T("I.MANAGE_CODE");
	else strFieldName.Empty();
	return strFieldName;
}

CString CBO_ACQ_OrderSerialNoDlg::GetSortCondition(CString strAlias)
{
	CString strCondition;
	if(strAlias == _T("��������")) strCondition = _T("DESC");
	else if(strAlias == _T("��������")) strCondition = _T("ASC");
	else strCondition.Empty();
	return strCondition;
}