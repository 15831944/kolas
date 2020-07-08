// BO_CAT_REGNO_DLG1.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_REGNO_DLG1.h"
#include "BO_CAT_REGNO_GIVE_LOST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REGNO_DLG dialog


CBO_CAT_REGNO_DLG::CBO_CAT_REGNO_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REGNO_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pRegNoArray = NULL;
}


CBO_CAT_REGNO_DLG::~CBO_CAT_REGNO_DLG()
{
	
}

VOID CBO_CAT_REGNO_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REGNO_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_REGNO_DLG, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REGNO_DLG)
	ON_BN_CLICKED(IDC_GIVE_LOSTREGNO, OnGiveLostregno)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_edtOmitRegNoCnt, OnKillfocusedtOmitRegNoCnt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REGNO_DLG message handlers

BOOL CBO_CAT_REGNO_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_������Ϲ�ȣ")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_Pre_RegGroup = _T("");
	AllControlDisplay(_T("CM_����_����_������Ϲ�ȣ"));


	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_REGNO_DLG::OnGiveLostregno() 
{
	INT idx = -1;
	SelectMakeList(_T("CM_����_����_������Ϲ�ȣ"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_������Ϲ�ȣ"), _T("MAIN_GRID"));
	if (idx < 0)
	{
		//����ó��
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����DM_å����"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox ( _T("DM_����_����DM_å����") );
		return ;
	}

	CString strRegNo;
	CString strRegCode;
	while ( idx >= 0 ) {
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
		strRegCode = pDM->GetCellData(_T("BB_��ϱ���_�ڵ�"), idx);
		if (!strRegNo.IsEmpty())
		{
			AfxMessageBox(_T("��� ��ȣ �ο��� �ڷᰡ �ֽ��ϴ�."));
			return;
		}
		if (strRegCode.IsEmpty())
		{
			AfxMessageBox(_T("��ϱ����� ���� �ڷᰡ �ֽ��ϴ�."));
			return;
		}

		idx = SelectGetNext(_T("CM_����_����_������Ϲ�ȣ"), _T("MAIN_GRID"));
	}

	//pDM->SetCellData( _T("��ϱ���"), strRegCode, 0 );
	// ��Ϲ�ȣ �ο� ȭ���� ����.
	CBO_CAT_REGNO_GIVE_LOST Dlg(this);
	Dlg.m_pRegNoArray = m_pRegNoArray;
	if( Dlg.DoModal() == IDOK )
	{
		AllControlDisplay(_T("CM_����_����_������Ϲ�ȣ"));
	}
}

BEGIN_EVENTSINK_MAP(CBO_CAT_REGNO_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_REGNO_DLG)
	ON_EVENT(CBO_CAT_REGNO_DLG, IDC_MSHFLEXGRID, -600 /* Click */, OnClickMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_REGNO_DLG::OnClickMshflexgrid() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid=( CESL_Grid * )FindControl(_T("CM_����_����_������Ϲ�ȣ"), _T("MAIN_GRID"));;

	CString strRegGroup;
	INT row = pGrid->GetRow();
	INT icount=0;
	INT iRowCount = pGrid->GetCount();

	//������ �ڷ��� ���� üũ
	for (INT i = 1; i<=iRowCount; i++)
	{
		if (pGrid->GetAt(i-1,0) == _T("V"))	icount++;
	}
	
	if (row == 0) return;
	if (icount < 2) m_Pre_RegGroup = _T("");
	
	if (row > 0) {
		pGrid->SetReverse(row-1);
		strRegGroup = pGrid->GetAt(row-1,2);
		if ( strRegGroup != m_Pre_RegGroup && m_Pre_RegGroup != _T(""))
		{
			pGrid->SetAt(row-1,0,_T(""));
			AfxMessageBox(_T("��ϱ����� ���� �ڷḸ �������ּ���."));
			return;
		}
	}
	m_Pre_RegGroup = strRegGroup;
}

VOID CBO_CAT_REGNO_DLG::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}



HBRUSH CBO_CAT_REGNO_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

//===================================================
//2009.04.13 ADD BY PJW : ������Ϲ�ȣ�� �����ö� ���� ������ ��
void CBO_CAT_REGNO_DLG::OnKillfocusedtOmitRegNoCnt() 
{
	CString strOmitRegNoCnt;
	INT nOmitRegNoCnt;
	CESL_ControlMgr * pCm = FindCM(_T("CM_����_����_������Ϲ�ȣ"));
	pCm->GetControlMgrData(_T("������ȣ����"), strOmitRegNoCnt);
	if( !strOmitRegNoCnt.IsEmpty() )
	{
		nOmitRegNoCnt = _ttoi(strOmitRegNoCnt); 
		if( 1 > nOmitRegNoCnt || 10000 < strOmitRegNoCnt )
		{
			AfxMessageBox(_T("�ִ�˻��Ǽ��� 1���� 10000���� �������� �մϴ�."));
			pCm->SetControlMgrData(_T("������ȣ����"), _T("100"));
			CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtOmitRegNoCnt);
			pEdit->SetFocus();
		}
	}
	else
	{
		AfxMessageBox(_T("�ִ�˻��Ǽ��� �־� �ּ���."));
	}
}
//===================================================

