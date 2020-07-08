// DLG_006_MU.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_006_MU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_MU dialog
CDLG_006_MU::CDLG_006_MU(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat006UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_006_MU)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CDLG_006_MU::~CDLG_006_MU()
{
}

void CDLG_006_MU::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_006_MU)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_006_MU, CDialog)
	//{{AFX_MSG_MAP(CDLG_006_MU)
		// NOTE: the ClassWizard will add message map macros here
		ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_MU message handlers
INT CDLG_006_MU::InitData()
{
	m_strSubGroupName = _T("<�����ڷ�(a)>");
	m_pCM->SetControlMgrData(_T("�ڷ����ǥ��"), _T("a"));

	// Alias ����
	// �ڷẰ	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@1-2"),_T("�۰�����"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@3"),_T("�Ǻ�����"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@5"),_T("�̿����ڼ���"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@6"),_T("�ڷ�������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@7"),_T("�����ڷ�1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@8"),_T("�����ڷ�2"));			
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@10"),_T("�����ڷ��ǳ���1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@11"),_T("�����ڷ��ǳ���2"));		

	// Control ����		
	m_mapControlAlias_ItemName.SetAt(_T("�۰�����")		, _T("(�۰�����)"));	
	m_mapControlAlias_ItemName.SetAt(_T("�Ǻ�����")			, _T("(�Ǻ�����)"));	
	m_mapControlAlias_ItemName.SetAt(_T("�̿����ڼ���")	, _T("(�̿����� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�ڷ�������")		, _T("(�ڷ��� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�����ڷ�1")		, _T("(�����ڷ�)"));
	m_mapControlAlias_ItemName.SetAt(_T("�����ڷ�2")		, _T("(�����ڷ�)"));	
	m_mapControlAlias_ItemName.SetAt(_T("�����ڷ��ǳ���1")	, _T("(�����ڷ��� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�����ڷ��ǳ���2")	, _T("(�����ڷ��� ����)"));	

	// Control�� ��������
	Display();

	return 0;
}

INT CDLG_006_MU::Get006()
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
// CDLG_006_MU message handlers

BOOL CDLG_006_MU::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_006_MU")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_006_MU"));
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

BOOL CDLG_006_MU::PreTranslateMessage(MSG* pMsg) 
{
	return CCat006UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_006_MU::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}
HBRUSH CDLG_006_MU::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}