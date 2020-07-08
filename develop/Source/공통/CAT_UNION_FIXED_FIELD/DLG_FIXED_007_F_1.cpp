// DLG_FIXED_007_F_1.cpp : implementation file
// 2014/03/28 �ڿ�� : ���ո�ũ ���� ����

#include "stdafx.h"
#include "DLG_FIXED_007_F_1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_F_1 dialog


CDLG_FIXED_007_F_1::CDLG_FIXED_007_F_1(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CDLG_FIXED_007_F_1::~CDLG_FIXED_007_F_1()
{
}

void CDLG_FIXED_007_F_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_F_1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_F_1, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_F_1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_F_1 message handlers

INT CDLG_FIXED_007_F_1::InitData()
{
	m_strSubGroupName = _T("<�˰��ڷ�(f)>");
	m_pCM->SetControlMgrData(_T("�ڷ����ǥ��"), _T("f"));

	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("�ڷ����ǥ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("Ư���ڷ�����"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("����ǥ��������1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("����ǥ��������2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6"),_T("���ھǺ�����1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@7"),_T("���ھǺ�����2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@8"),_T("���ھǺ�����3"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@9"),_T("������Ư¡"));	

	// Control ����	
	m_mapControlAlias_ItemName.SetAt(_T("Ư���ڷ�����"), _T("(Ư���ڷ�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("����ǥ��������1"), _T("(����ǥ���� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("����ǥ��������2"), _T("(����ǥ���� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("������"), _T("(������)"));
	m_mapControlAlias_ItemName.SetAt(_T("���ھǺ�����"), _T("(���ھǺ�����)"));	
	m_mapControlAlias_ItemName.SetAt(_T("���ھǺ�����2"), _T("(���ھǺ�����)"));	
	m_mapControlAlias_ItemName.SetAt(_T("���ھǺ�����3"), _T("(���ھǺ�����)"));	
	m_mapControlAlias_ItemName.SetAt(_T("������Ư¡"), _T("(������Ư¡)"));

	// Control�� ��������
	Display();

	return 0;
}

INT CDLG_FIXED_007_F_1::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("Ư���ڷ�����"));
	arrStrControlAlias.Add(_T("����ǥ��������1"));
	arrStrControlAlias.Add(_T("����ǥ��������2"));
	arrStrControlAlias.Add(_T("������"));
	arrStrControlAlias.Add(_T("���ھǺ�����"));
	arrStrControlAlias.Add(_T("���ھǺ�����2"));
	arrStrControlAlias.Add(_T("���ھǺ�����3"));
	arrStrControlAlias.Add(_T("������Ư¡"));
	
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// ������ 007 �����ش�.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_F_1 message handlers

BOOL CDLG_FIXED_007_F_1::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_F_1")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_F_1"));
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

BOOL CDLG_FIXED_007_F_1::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_F_1::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_F_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
