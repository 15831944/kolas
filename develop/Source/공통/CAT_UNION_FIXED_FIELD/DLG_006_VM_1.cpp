// DLG_006_VM_1.cpp : implementation file
// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
// ���ո�ũ ���� ����

#include "stdafx.h"
#include "DLG_006_VM_1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_VM_1 dialog

CDLG_006_VM_1::CDLG_006_VM_1(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat006UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_006_VM_1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMarc = pMarc;
}

CDLG_006_VM_1::~CDLG_006_VM_1()
{
}

void CDLG_006_VM_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_006_VM_1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_006_VM_1, CDialog)
	//{{AFX_MSG_MAP(CDLG_006_VM_1)
		// NOTE: the ClassWizard will add message map macros here
		ON_EN_KILLFOCUS(IDC_eFIXED008_11, OnKillfocuseFIXED00811)
		ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_VM_1 message handlers
INT CDLG_006_VM_1::InitData()
{
	m_strSubGroupName = _T("<�����ڷ�(a)>");
	m_pCM->SetControlMgrData(_T("�ڷ����ǥ��"), _T("a"));

	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@1-3"),_T("�󿵽ð�"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@5"),_T("�̿����ڼ���"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@9"),_T("�����ڷ�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@12"),_T("��û���ڷ�������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@13"),_T("�������"));

	// Control ����		
	m_mapControlAlias_ItemName.SetAt(_T("�󿵽ð�")				, _T("(��ȭ�� ���� ��ȭ�ڷ��� �󿵽ð�)"));
	m_mapControlAlias_ItemName.SetAt(_T("�̿����ڼ���")		, _T("(�̿����� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�����ڷ�����")			, _T("(�����ڷ� ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("��û���ڷ�������")		, _T("(��û���ڷ��� ����)"));	
	m_mapControlAlias_ItemName.SetAt(_T("�������")				, _T("(�������)"));	

	// Control�� ��������
	Display();

	return 0;
}

INT CDLG_006_VM_1::Get006()
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
// CDLG_006_VM_1 message handlers

BOOL CDLG_006_VM_1::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_006_VM_1")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_006_VM_1"));
	if (!m_pCM) goto ERR;

	if (Init() < 0) goto ERR;
	if (InitData() < 0) goto ERR;

	( (CEdit*) GetDlgItem (IDC_eFIXED008_11) )->SetLimitText(3);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;
}

BOOL CDLG_006_VM_1::PreTranslateMessage(MSG* pMsg) 
{
	return CCat006UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

void CDLG_006_VM_1::OnKillfocuseFIXED00811() 
{
	// �󿵽ð��� ���ڷ� �Ǿ� �ְ� 3�ڸ��� �ƴѰ�� 0���� ä���ֱ�
	CString strText;
	CString strNum;
	INT nNum;

	( (CEdit*) GetDlgItem (IDC_eFIXED008_11) )->GetWindowText(strText);

	nNum = _ttoi(strText.GetBuffer(0));
	if ( nNum > 0 && strText.GetLength() < 3 )
	{
		strNum.Format(_T("%03d"), nNum );
		( (CEdit*) GetDlgItem (IDC_eFIXED008_11) )->SetWindowText(strNum);

	}		
}


BOOL CDLG_006_VM_1::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}
HBRUSH CDLG_006_VM_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}