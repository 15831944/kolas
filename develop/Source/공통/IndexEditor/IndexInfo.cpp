// IndexInfo.cpp : implementation file
//

#include "stdafx.h"
#include "IndexInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexInfo dialog


CIndexInfo::CIndexInfo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexInfo)
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM_TAG = NULL;

    m_strTagIndexType.Format(_T("A - Ư�� �����ʵ��� ������ �ٿ��� ���� �ۼ�($a || $b || $f)\r\n\r\n")
				 _T("B - Ư�� �����ʵ��� ������ �ٿ��� ���� �ۼ�($a || $b || $x || $y || $z)\r\n\r\n")
                 _T("C - Ư�� �����ʵ��� ������ �ٿ��� ���� �ۼ�($a || $p || $x || $y || $z)\r\n\r\n")
                 _T("D - Ư�� �����ʵ��� ������ �ٿ��� ���� �ۼ�($a || $x || $y || $z)\r\n\r\n")
                 _T("E - Ư�� �����ʵ� ($p)�� �ִ� ��� ���� 2�� �ۼ� ($a || $p, $p)\r\n\r\n")
                 _T("F - Ư�� �����ʵ� ($b)�� �ִ� ��� ���� 2�� �ۼ� ($a || $b || $k || $g, $b || $k || $g)\r\n\r\n")
                 _T("G - �ĺ���ȣ�� ���� �±�"));                  
		


	m_strIndicatorIndexType.Format(_T("A - �״�� ���λ���\r\n\r\n")
				 _T("B - ����ȣ�� �����ϰ� ���λ���\r\n\r\n") 
				 _T("C - ó���� ")_T("(")_T(" �� ���۵Ǹ� ����ȣ(�ϳ��� ��ȣ)�� ������ �����ϰ� ���λ���\r\n\r\n")
				 _T("D - ����ȣ�� �����ؼ� ���� ����\r\n\r\n")
				 _T("E - ���� 2�� �ۼ� (����ȣ�� �����ϰ� �����ۼ� + ����ȣ�� �����ؼ� �����ۼ�)\r\n\r\n")
				 _T("F - ǥ��� �����ϰ� �����ۼ�, ����ȣ�� �����ϰ� ���� ����\r\n\r\n")
				 _T("G - ǥ��� �����ϰ� �����ۼ�, ����ȣ�� �����ϰ� ���� ����\r\n\r\n")
				 _T("H - ǥ��� �����ϰ� �����ۼ�, ���� 2�� �ۼ�(����ȣ ����,����)\r\n\r\n")
				 _T("I - ��Ȱ� �����ϰ� �����ۼ�\r\n\r\n")
				 _T("J - ����ȭ ���� ����\r\n\r\n"));

}


VOID CIndexInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexInfo)
	DDX_Control(pDX, IDC_tabIndexTypeInfo, m_tabIndexInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexInfo, CDialog)
	//{{AFX_MSG_MAP(CIndexInfo)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabIndexTypeInfo, OnSelchangetabIndexTypeInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexInfo message handlers

BOOL CIndexInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("�ε���������_��������") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : �ε���������_��������") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_�ε���������_��������"));
	m_pDM_TAG = FindDM(_T("DM_�ε���������_�±�"));


	if(!m_pCM || !m_pDM_TAG)
	{
		AfxMessageBox( _T("reference Error : CM_�ε���������_��������,DM_�ε���������_�±�") );
		return FALSE;		
	}


	m_tabIndexInfo.InsertItem(0,_T("�±� ��������"));
	m_tabIndexInfo.InsertItem(1,_T("���ñ�ȣ ��������"));
	m_tabIndexInfo.SetCurSel(0);
	m_pCM->SetControlMgrData(_T("������������"),m_strTagIndexType);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CIndexInfo::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}


BOOL CIndexInfo::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CIndexInfo::ResizeWindow(CRect rectParent)
{
	
	//�ڱ� �ڽ�
	rectParent.left = rectParent.left + 455;
	//rectParent.left = rectParent.right - 450;
	rectParent.right -= 5;
	
	this->MoveWindow(rectParent);

	CWnd *pWnd = NULL;

	CRect rect,tmprect;
	this->GetClientRect(rect);
		
	tmprect = rect;

	//tab ���� ����
	pWnd = (CWnd*)GetDlgItem(IDC_tabIndexTypeInfo);
	if(pWnd->GetSafeHwnd() == NULL) return;
	
	tmprect.top += 10;
	tmprect.bottom = tmprect.top + 230;
	tmprect.left += 10;
	tmprect.right -= 10;	

	pWnd->MoveWindow(tmprect);	
	

	//edit ���� ����
	pWnd = (CWnd*)GetDlgItem(IDC_edtIndexTypeDesc);
	if(pWnd->GetSafeHwnd() == NULL) return;

	tmprect.top += 25;
	tmprect.bottom -= 10;
	tmprect.left += 10;
	tmprect.right -= 10;	

	pWnd->MoveWindow(tmprect);	

	tmprect = rect;

	//static �±� ����
	pWnd = (CWnd*)GetDlgItem(IDC_stcTagInfo);
	if(pWnd->GetSafeHwnd() == NULL) return;
	
	tmprect.top += 250;
	tmprect.bottom -= 10;
	tmprect.left += 10;
	tmprect.right -= 10;	

	pWnd->MoveWindow(tmprect);	
	

	//edit �±� ����
	pWnd = (CWnd*)GetDlgItem(IDC_edtTagDesc);
	if(pWnd->GetSafeHwnd() == NULL) return;

	tmprect.top += 25;
	tmprect.bottom -= 10;
	tmprect.left += 10;
	tmprect.right -= 10;	

	pWnd->MoveWindow(tmprect);	

}

VOID CIndexInfo::OnSelchangetabIndexTypeInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT nCursel = m_tabIndexInfo.GetCurSel();
	
	CString strMsg;

	if(0 == nCursel)
	{
		strMsg = m_strTagIndexType;
	}
	else if(1 == nCursel)
	{
		strMsg = m_strIndicatorIndexType;
	}

	m_pCM->SetControlMgrData(_T("������������"),strMsg);

	*pResult = 0;
}

VOID CIndexInfo::ShowTagDesc(CString strGroup, CString strTagCode)
{
	if(strGroup.IsEmpty() || strTagCode.IsEmpty()) return;

	CString strQuery,strResult;
	strQuery.Format(_T("SELECT DESCRIP FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s' AND TAG_CODE = '%s'"),
					strGroup,strTagCode);
	
	m_pDM_TAG->GetOneValueQuery(strQuery,strResult);
	m_pCM->SetControlMgrData(_T("�±׼���"),strResult);

	
	CString strMsg;
	strMsg.Format(_T("[  %s  ]"),strTagCode);
	GetDlgItem(IDC_stcTagInfo)->SetWindowText(strMsg);
}

VOID CIndexInfo::ShowIndexTypeDesc(INT nType)
{
	m_tabIndexInfo.SetCurSel(nType);
	UpdateData(FALSE);

	
	CString strMsg;

	if(0 == nType)
	{
		strMsg = m_strTagIndexType;
	}
	else if(1 == nType)
	{
		strMsg = m_strIndicatorIndexType;
	}

	m_pCM->SetControlMgrData(_T("������������"),strMsg);
	
}