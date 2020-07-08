// DLG_006_CR.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_006_CR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_CR dialog
CDLG_006_CR::CDLG_006_CR(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat006UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_006_CR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CDLG_006_CR::~CDLG_006_CR()
{
}

void CDLG_006_CR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_006_CR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_006_CR, CDialog)
	//{{AFX_MSG_MAP(CDLG_006_CR)
		// NOTE: the ClassWizard will add message map macros here
		ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_CR message handlers
INT CDLG_006_CR::InitData()
{
	//m_strSubGroupName = _T("<�����ڷ�(a)>");
	m_pCM->SetControlMgrData(_T("�ڷ����ǥ��"), _T("a"));

	// Alias ����
	// �ڷẰ
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@1"),_T("�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@2"),_T("���Լ�"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@4"),_T("����ڷ�������"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@5"),_T("���ڷ��ǹ߰�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@6"),_T("�ڷ�������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@7"),_T("��������1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@8"),_T("��������2"));		
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@9"),_T("ȸ�ǰ��๰"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@12"),_T("ǥ���Ǿ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@13"),_T("ǥ���⺻ǥ�񺯰�÷��ڵ�������"));

	// Control ����		
	// �ڷẰ		
	m_mapControlAlias_ItemName.SetAt(_T("�����"), _T("(�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("���Լ�"), _T("(���Լ�)"));	
	m_mapControlAlias_ItemName.SetAt(_T("����ڷ�������"), _T("(����ڷ��� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("���ڷ��ǹ߰�����"), _T("(���ڷ��� �߰�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�ڷ�������"), _T("(�ڷ��� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������1"), _T("(��������)"));	
	m_mapControlAlias_ItemName.SetAt(_T("��������2"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("ȸ�ǰ��๰"), _T("(ȸ�ǰ��๰)"));
	m_mapControlAlias_ItemName.SetAt(_T("ǥ���Ǿ��"), _T("(ǥ���� ���)"));	
	m_mapControlAlias_ItemName.SetAt(_T("ǥ���⺻ǥ�񺯰�÷��ڵ�������"), _T("(ǥ�� �⺻ǥ�� ����� ���ڵ�������)"));

	// Control�� ��������
	Display();

	return 0;
}

INT CDLG_006_CR::Get006()
{
	CStringArray arrStrControlAlias;	

	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// ������ 007 �����ش�.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("006"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_CR message handlers

BOOL CDLG_006_CR::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_006_CR")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_006_CR"));
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

BOOL CDLG_006_CR::PreTranslateMessage(MSG* pMsg) 
{
	return CCat006UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_006_CR::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}
HBRUSH CDLG_006_CR::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}