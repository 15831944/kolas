// DLG_FIXED_007_V.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_V.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_V dialog


CDLG_FIXED_007_V::CDLG_FIXED_007_V(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CDLG_FIXED_007_V::~CDLG_FIXED_007_V()
{
}

void CDLG_FIXED_007_V::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_V)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_V, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_V)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_V message handlers

INT CDLG_FIXED_007_V::InitData()
{
	m_strSubGroupName = _T("<������ȭ�ڷ�(v)>");
	m_pCM->SetControlMgrData(_T("�ڷ����ǥ��"), _T("v"));

	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"), _T("�ڷ����ǥ��")	);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"), _T("Ư���ڷ�����")	);	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"), _T("��ä")			);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"), _T("������ȭ����")	);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"), _T("��������")		);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6"), _T("������ϸ�ü")	);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@7"), _T("�ʸ�����")		);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@8"), _T("�������������")	);

	// Control ����	
	m_mapControlAlias_ItemName.SetAt(_T("Ư���ڷ�����")		, _T("(Ư���ڷ�����)")	);
	m_mapControlAlias_ItemName.SetAt(_T("��ä")				, _T("(��ä)")			);
	m_mapControlAlias_ItemName.SetAt(_T("������ȭ����")	, _T("(���� ��ȭ����)")	);
	m_mapControlAlias_ItemName.SetAt(_T("��������")			, _T("(��������)")		);
	m_mapControlAlias_ItemName.SetAt(_T("������ϸ�ü")		, _T("(������� ��ü)")	);
	m_mapControlAlias_ItemName.SetAt(_T("�ʸ�����")			, _T("(�ʸ��� ��)")		);
	m_mapControlAlias_ItemName.SetAt(_T("�������������")	, _T("(������ �������)")	);
	// Control�� ��������
	Display();

	return 0;
}

INT CDLG_FIXED_007_V::Get007()
{
	CStringArray arrStrControlAlias;	
	arrStrControlAlias.Add(_T("Ư���ڷ�����")	);
	arrStrControlAlias.Add(_T("��ä")			);
	arrStrControlAlias.Add(_T("������ȭ����")	);
	arrStrControlAlias.Add(_T("��������")		);
	arrStrControlAlias.Add(_T("������ϸ�ü")	);
	arrStrControlAlias.Add(_T("�ʸ�����")		);
	arrStrControlAlias.Add(_T("�������������")	);

	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// ������ 007 �����ش�.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_V message handlers

BOOL CDLG_FIXED_007_V::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_V")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_V"));
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

BOOL CDLG_FIXED_007_V::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_V::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_V::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
