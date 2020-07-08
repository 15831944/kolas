// DLG_FIXED_007_Q.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_Q.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_Q dialog


CDLG_FIXED_007_Q::CDLG_FIXED_007_Q(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CDLG_FIXED_007_Q::~CDLG_FIXED_007_Q()
{
}

void CDLG_FIXED_007_Q::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_Q)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_Q, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_Q)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_Q message handlers

INT CDLG_FIXED_007_Q::InitData()
{
	m_strSubGroupName = _T("<�Ǻ�(q)>");
	m_pCM->SetControlMgrData(_T("�ڷ����ǥ��"), _T("q"));

	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("�ڷ����ǥ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("Ư���ڷ�����"));
	
	// Control ����
	m_mapControlAlias_ItemName.SetAt(_T("Ư���ڷ�����"), _T("(Ư���ڷ�����)"));

	// Control�� ��������
	Display();

	return 0;
}

INT CDLG_FIXED_007_Q::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("Ư���ڷ�����"));
	
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// ������ 007 �����ش�.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_Q message handlers

BOOL CDLG_FIXED_007_Q::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_Q")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_Q"));
	if (!m_pCM) goto ERR;

	if (Init() < 0) goto ERR;
	if (InitData() < 0) goto ERR;
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;
}

BOOL CDLG_FIXED_007_Q::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_Q::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_Q::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
