// Cat007FixedField_M.cpp : implementation file
//

#include "stdafx.h"
#include "Cat007FixedField_M.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedField_M dialog


CCat007FixedField_M::CCat007FixedField_M(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007FixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat007FixedField_M)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CCat007FixedField_M::~CCat007FixedField_M()
{
}

VOID CCat007FixedField_M::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat007FixedField_M)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CCat007FixedField_M::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CCat007FixedField_M, CDialog)
	//{{AFX_MSG_MAP(CCat007FixedField_M)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedField_M Operations
INT CCat007FixedField_M::InitData()
{
	m_strSubGroupName = _T("<��ȭ(m)>");
	m_pCM->SetControlMgrData(_T("�ڷ����ǥ��"), _T("m"));

	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("�ڷ����ǥ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("�ڷ�ĺ�ǥ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@2"),_T("��������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("��ä"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("��ũ������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("��ü���������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6"),_T("������ϸ�ü"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@7"),_T("�ʸ�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@8"),_T("�������������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@9"),_T("���ۿ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@10"),_T("��������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@11"),_T("���ۻ���"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@12"),_T("�ʸ��Ǳ⺻���"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@13"),_T("�����ǹ���"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@14"),_T("õ������ȭ����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@15"),_T("��������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@16"),_T("������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@17-22"),_T("�ʸ��ǰ˻�����"));

	// Control ����
	m_mapControlAlias_ItemName.SetAt(_T("�ڷ�ĺ�ǥ��"), _T("(�ڷ�ĺ�ǥ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ä"), _T("(��ä)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ũ������"), _T("(��ũ������)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ü���������"), _T("(��ü���������)"));
	m_mapControlAlias_ItemName.SetAt(_T("������ϸ�ü"), _T("(������ϸ�ü)"));
	m_mapControlAlias_ItemName.SetAt(_T("�ʸ�����"), _T("(�ʸ�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�������������"), _T("(�������������)"));
	m_mapControlAlias_ItemName.SetAt(_T("���ۿ��"), _T("(���ۿ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("���ۻ���"), _T("(���ۻ���)"));
	m_mapControlAlias_ItemName.SetAt(_T("�ʸ��Ǳ⺻���"), _T("(�ʸ��Ǳ⺻���)"));
	m_mapControlAlias_ItemName.SetAt(_T("�����ǹ���"), _T("(��ä�ǹ���)"));
	m_mapControlAlias_ItemName.SetAt(_T("õ������ȭ����"), _T("(õ������ȭ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("������"), _T("(������)"));
	m_mapControlAlias_ItemName.SetAt(_T("�ʸ��ǰ˻�����"), _T("(�ʸ��ǰ˻�����)"));

	// Control�� ��������
	Display();

	return 0;
}

INT CCat007FixedField_M::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("�ڷ�ĺ�ǥ��"));
	arrStrControlAlias.Add(_T("��ä"));
	arrStrControlAlias.Add(_T("��ũ������"));
	arrStrControlAlias.Add(_T("��ü���������"));
	arrStrControlAlias.Add(_T("������ϸ�ü"));
	arrStrControlAlias.Add(_T("�ʸ�����"));
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
// CCat007FixedField_M message handlers

BOOL CCat007FixedField_M::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_FORM_06")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_BO_CAT_PROC_0320_FORM_06"));
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

BOOL CCat007FixedField_M::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007FixedFieldMgr::PreTranslateMessage(pMsg);
}

HBRUSH CCat007FixedField_M::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
