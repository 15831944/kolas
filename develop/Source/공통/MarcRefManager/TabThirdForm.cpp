// TabThirdForm.cpp : implementation file
//

#include "stdafx.h"
#include "TabThirdForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabThirdForm dialog


CTabThirdForm::CTabThirdForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabThirdForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strPK = _T("");
	m_pCM = NULL;
	m_pDM = NULL;
}


VOID CTabThirdForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabThirdForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabThirdForm, CDialog)
	//{{AFX_MSG_MAP(CTabThirdForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabThirdForm message handlers
BOOL CTabThirdForm::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CTabThirdForm::ResizeWindow(CRect rectParent)
{
	//�ڱ� �ڽ�
	this->MoveWindow(rectParent);

	CWnd *pWnd = NULL;

	CRect rect,tmprect;
	this->GetClientRect(rect);

	//���� static
	pWnd = (CWnd*)GetDlgItem(IDC_stcDescrip);
	if(pWnd->GetSafeHwnd() == NULL) return;

	tmprect = rect;

	rect.top += 10;
	rect.bottom -= 110;
	rect.left += 10;
	rect.right -= 10;	

	pWnd->MoveWindow(rect);	

	//���� edit
	pWnd = (CWnd*)GetDlgItem(IDC_edtDescrip);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rect.top += 15;
	rect.bottom -= 15;
	rect.left += 15;
	rect.right -= 15;	

	pWnd->MoveWindow(rect);	
	
	//���� static
	pWnd = (CWnd*)GetDlgItem(IDC_stcExample);
	if(pWnd->GetSafeHwnd() == NULL) return;
	
	tmprect.top = tmprect.bottom - 100;
	tmprect.bottom -= 10;
	tmprect.left += 10;
	tmprect.right -= 10;	

	pWnd->MoveWindow(tmprect);	

	//���� edit
	pWnd = (CWnd*)GetDlgItem(IDC_edtExample);
	if(pWnd->GetSafeHwnd() == NULL) return;

	tmprect.top += 15;
	tmprect.bottom -= 15;
	tmprect.left += 15;
	tmprect.right -= 15;	

	pWnd->MoveWindow(tmprect);	


}

BOOL CTabThirdForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("��ũ����_��3") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ��ũ����_��3") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_��ũ����_��3"));
	m_pDM = FindDM(_T("DM_��ũ����_��3"));

	if(!m_pCM || !m_pDM)
	{
		AfxMessageBox( _T("reference Error : ��ũ����_��3") );
		return FALSE;		
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CTabThirdForm::SetAllData(CString strPK)
{
	m_strPK = strPK;
}

VOID CTabThirdForm::ShowAllData()
{
	CString strWhere;
	strWhere.Format(_T("PK = '%s'"),m_strPK);
	
	m_pDM->RefreshDataManager(strWhere);
	m_pCM->AllControlDisplay(0);
}
