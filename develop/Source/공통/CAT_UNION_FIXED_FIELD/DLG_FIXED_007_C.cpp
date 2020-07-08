// DLG_FIXED_007_C.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_C.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_C dialog


CDLG_FIXED_007_C::CDLG_FIXED_007_C(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_C)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CDLG_FIXED_007_C::~CDLG_FIXED_007_C()
{
}

void CDLG_FIXED_007_C::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_C)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_C, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_C)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_C message handlers

INT CDLG_FIXED_007_C::InitData()
{
	m_strSubGroupName = _T("<�����ڷ�(c)>");
	m_pCM->SetControlMgrData(_T("�ڷ����ǥ��"), _T("c"));

	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("�ڷ����ǥ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("Ư���ڷ�����"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("��ä"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("�԰�"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6-8"),_T("�̹����Ǻ�Ʈ����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@9"),_T("��������������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@10"),_T("ǰ��������ǥ"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@11"),_T("�����ڷ���������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@12"),_T("�������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@13"),_T("��������ǰ��"));

	// Control ����
	m_mapControlAlias_ItemName.SetAt(_T("Ư���ڷ�����")			, _T("(Ư���ڷ�����)")	);
	m_mapControlAlias_ItemName.SetAt(_T("��ä")					, _T("(��ä)")		);
	m_mapControlAlias_ItemName.SetAt(_T("�԰�")					, _T("(�԰�)"));
	m_mapControlAlias_ItemName.SetAt(_T("����")					, _T("(����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�̹����Ǻ�Ʈ����")		, _T("(�̹����� ��Ʈ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������������")		, _T("(���������� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("ǰ��������ǥ")			, _T("(ǰ��������ǥ)"));
	m_mapControlAlias_ItemName.SetAt(_T("�����ڷ���������")		, _T("(�����ڷ��� ������)"));
	m_mapControlAlias_ItemName.SetAt(_T("�������")				, _T("(�������)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������ǰ��")			, _T("(�������� ǰ��)"));

	// Control�� ��������
	Display();

	return 0;
}

INT CDLG_FIXED_007_C::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("Ư���ڷ�����"));
	arrStrControlAlias.Add(_T("��ä"));
	arrStrControlAlias.Add(_T("�԰�"));
	arrStrControlAlias.Add(_T("����"));
	arrStrControlAlias.Add(_T("�̹����Ǻ�Ʈ����"));
	arrStrControlAlias.Add(_T("��������������"));
	arrStrControlAlias.Add(_T("ǰ��������ǥ"));
	arrStrControlAlias.Add(_T("�����ڷ���������"));
	arrStrControlAlias.Add(_T("�������"));
	arrStrControlAlias.Add(_T("��������ǰ��"));
	arrStrControlAlias.Add(_T("�ʸ��˻�����"));
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// ������ 007 �����ش�.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_C message handlers

BOOL CDLG_FIXED_007_C::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_C")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_C"));
	if (!m_pCM) goto ERR;

	if (Init() < 0) goto ERR;
	if (InitData() < 0) goto ERR;

	( (CEdit*) GetDlgItem (IDC_eFIXED008_5) )->SetLimitText(3);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;
}

BOOL CDLG_FIXED_007_C::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_C::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_C::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
