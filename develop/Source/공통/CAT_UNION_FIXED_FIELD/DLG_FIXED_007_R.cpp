// DLG_FIXED_007_R.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_R.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_R dialog


CDLG_FIXED_007_R::CDLG_FIXED_007_R(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CDLG_FIXED_007_R::~CDLG_FIXED_007_R()
{
}

void CDLG_FIXED_007_R::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_R)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_R, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_R)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_R message handlers

INT CDLG_FIXED_007_R::InitData()
{
	m_strSubGroupName = _T("<����Ž�絵(r)>");
	m_pCM->SetControlMgrData(_T("�ڷ����ǥ��"), _T("r"));

	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("Ư���ڷ�����"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@2"),_T("Ž����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("Ž��Ⱒ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("��������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("ž�������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6"),_T("ž���"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@7"),_T("Ž�������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@8"),_T("����������"));

	// Control ����
	m_mapControlAlias_ItemName.SetAt(_T("Ư���ڷ�����")	, _T("(Ư���ڷ�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("Ž����")	, _T("(Ž��� ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("Ž��Ⱒ��")	, _T("(Ž��� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������")		, _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("ž�������")	, _T("(ž��� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("ž���")		, _T("(ž���)"));
	m_mapControlAlias_ItemName.SetAt(_T("Ž�������")	, _T("(Ž��� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("����������")	, _T("(������ ����)"));

	// Control�� ��������
	Display();

	return 0;
}

INT CDLG_FIXED_007_R::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("(Ư���ڷ�����)"));
	arrStrControlAlias.Add(_T("(Ž����)"));
	arrStrControlAlias.Add(_T("(Ž��Ⱒ��)"));
	arrStrControlAlias.Add(_T("(��������)"));
	arrStrControlAlias.Add(_T("(ž�������)"));
	arrStrControlAlias.Add(_T("(ž���)"));
	arrStrControlAlias.Add(_T("(Ž�������)"));
	arrStrControlAlias.Add(_T("(����������)"));

	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// ������ 007 �����ش�.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_R message handlers

BOOL CDLG_FIXED_007_R::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_R")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_R"));
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

BOOL CDLG_FIXED_007_R::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_R::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_R::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
