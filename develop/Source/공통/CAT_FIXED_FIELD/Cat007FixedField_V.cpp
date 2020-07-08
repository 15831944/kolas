// Cat007FixedField_V.cpp : implementation file
//

#include "stdafx.h"
#include "Cat007FixedField_V.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedField_V dialog


CCat007FixedField_V::CCat007FixedField_V(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007FixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat007FixedField_V)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CCat007FixedField_V::~CCat007FixedField_V()
{
}
VOID CCat007FixedField_V::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat007FixedField_V)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CCat007FixedField_V::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CCat007FixedField_V, CDialog)
	//{{AFX_MSG_MAP(CCat007FixedField_V)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedField_V Operations
INT CCat007FixedField_V::InitData()
{
	m_strSubGroupName = _T("<������ȭ�ڷ�(v)>");
	m_pCM->SetControlMgrData(_T("�ڷ����ǥ��"), _T("v"));

	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("�ڷ����ǥ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("�ڷ�ĺ�ǥ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@2"),_T("��������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("��ä"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("������ȭ����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("��ü���������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6"),_T("������ϸ�ü"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@7"),_T("ũ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@8"),_T("�������������"));

	// Control ����
	m_mapControlAlias_ItemName.SetAt(_T("�ڷ�ĺ�ǥ��"), _T("(�ڷ�ĺ�ǥ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ä"), _T("(��ä)"));
	m_mapControlAlias_ItemName.SetAt(_T("������ȭ����"), _T("(������ȭ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ü���������"), _T("(��ü���������)"));
	m_mapControlAlias_ItemName.SetAt(_T("������ϸ�ü"), _T("(������ϸ�ü)"));
	m_mapControlAlias_ItemName.SetAt(_T("ũ��"), _T("(007ũ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("�������������"), _T("(�������������)"));

	// Control�� ��������
	Display();

	return 0;
}

INT CCat007FixedField_V::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("�ڷ�ĺ�ǥ��"));
	arrStrControlAlias.Add(_T("��ä"));
	arrStrControlAlias.Add(_T("������ȭ����"));
	arrStrControlAlias.Add(_T("��������"));
	arrStrControlAlias.Add(_T("��ü���������"));
	arrStrControlAlias.Add(_T("������ϸ�ü"));
	arrStrControlAlias.Add(_T("ũ��"));
	arrStrControlAlias.Add(_T("�������������"));
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// ������ 007 �����ش�.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedField_V message handlers

BOOL CCat007FixedField_V::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_FORM_09")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_BO_CAT_PROC_0320_FORM_09"));
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

BOOL CCat007FixedField_V::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007FixedFieldMgr::PreTranslateMessage(pMsg);
}

HBRUSH CCat007FixedField_V::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
