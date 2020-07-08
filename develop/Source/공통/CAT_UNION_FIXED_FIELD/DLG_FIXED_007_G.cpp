// DLG_FIXED_007_G.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_G.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_G dialog


CDLG_FIXED_007_G::CDLG_FIXED_007_G(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CDLG_FIXED_007_G::~CDLG_FIXED_007_G()
{
}

void CDLG_FIXED_007_G::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_G)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_G, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_G)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_G message handlers

INT CDLG_FIXED_007_G::InitData()
{
	m_strSubGroupName = _T("<��鿵���ڷ�(g)>");
	m_pCM->SetControlMgrData(_T("�ڷ����ǥ��"), _T("g"));

	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("�ڷ����ǥ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("Ư���ڷ�����"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("��ä"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("��������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("��������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6"),_T("������ϸ�ü"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@7"),_T("�ʸ�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@8"),_T("Ʋ������"));

	// Control ����
	m_mapControlAlias_ItemName.SetAt(_T("Ư���ڷ�����")		, _T("(Ư���ڷ�����)"));	
	m_mapControlAlias_ItemName.SetAt(_T("��ä")				, _T("(��ä)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������")			, _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������")			, _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("������ϸ�ü")		, _T("(������ϸ�ü)"));
	m_mapControlAlias_ItemName.SetAt(_T("�ʸ�����")			, _T("(�ʸ��� ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("Ʋ������")			, _T("(Ʋ�� ����)"));

	// Control�� ��������
	Display();

	return 0;
}

INT CDLG_FIXED_007_G::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("Ư���ڷ�����"));
	arrStrControlAlias.Add(_T("��ä"));
	arrStrControlAlias.Add(_T("��������"));
	arrStrControlAlias.Add(_T("��������"));
	arrStrControlAlias.Add(_T("������ϸ�ü"));
	arrStrControlAlias.Add(_T("�ʸ�����"));
	arrStrControlAlias.Add(_T("Ʋ������"));

	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// ������ 007 �����ش�.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_G message handlers

BOOL CDLG_FIXED_007_G::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_FIXED_007_G")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_G"));
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

BOOL CDLG_FIXED_007_G::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_G::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_G::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
