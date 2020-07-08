// RelationForm.cpp : implementation file
//

#include "stdafx.h"
#include "RelationForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRelationForm dialog


CRelationForm::CRelationForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CRelationForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pCM = NULL;

	m_strBasicParam = _T("");
	m_strOperator = _T("");
	m_strParam = _T("");
	m_strDescrip = _T("");

}


VOID CRelationForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRelationForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRelationForm, CDialog)
	//{{AFX_MSG_MAP(CRelationForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRelationForm message handlers

BOOL CRelationForm::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CRelationForm::ResizeWindow(CRect rectParent)
{
	
	
	//�ڱ� �ڽ�
	rectParent.left += 305;
	this->MoveWindow(rectParent);

	
	
	CWnd *pWnd = NULL;

	CRect rect;
	this->GetClientRect(rect);
	

	//static �ڽ�
	pWnd = (CWnd*)GetDlgItem(IDC_stcBox);
	if(pWnd->GetSafeHwnd() == NULL) return;

	pWnd->MoveWindow(rect);	
	


	//edit static
	pWnd = (CWnd*)GetDlgItem(IDC_stcRelationDescrip);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rect.top += 100;
	rect.bottom -= 10;
	rect.left += 10;
	rect.right -= 10;	

	pWnd->MoveWindow(rect);	


	//edit
	pWnd = (CWnd*)GetDlgItem(IDC_edtRelationDescrip);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rect.top += 15;
	rect.bottom -= 15;
	rect.left += 15;
	rect.right -= 15;	

	pWnd->MoveWindow(rect);	

}
BOOL CRelationForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr( _T("��ũ����_����") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ��ũ����_����") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_��ũ����_����"));
	
	if(!m_pCM)
	{
		AfxMessageBox( _T("reference Error : ��ũ����_����") );
		return FALSE;		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CRelationForm::SetAllData(CString strBasicParam,
							   CString strOperator,
							   CString strParam,
							   CString strDescrip)
{

	m_strBasicParam = strBasicParam;
	m_strOperator = strOperator;
	m_strParam = strParam;
	m_strDescrip = strDescrip;

}

VOID CRelationForm::ShowAllData()
{
	m_pCM->SetControlMgrData(_T("��������"),m_strBasicParam);
	m_pCM->SetControlMgrData(_T("�����ȣ"),m_strOperator);
	m_pCM->SetControlMgrData(_T("��������"),m_strParam);
	m_pCM->SetControlMgrData(_T("����"),m_strDescrip);

}
