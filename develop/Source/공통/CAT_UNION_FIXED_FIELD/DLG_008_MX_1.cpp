// DLG_008_MX_1.cpp : implementation file
// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
// ���ո�ũ ���� ����

#include "stdafx.h"
#include "resource.h"
#include "DLG_008_MX_1.h"
#include "DLG_FIXED_007.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_008_MX_1 dialog


CDLG_008_MX_1::CDLG_008_MX_1(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat008UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_008_MX_1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CDLG_008_MX_1::~CDLG_008_MX_1()
{
}
void CDLG_008_MX_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_008_MX_1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_008_MX_1, CDialog)
	//{{AFX_MSG_MAP(CDLG_008_MX_1)
	ON_BN_CLICKED(IDC_b007EDIT, Onb007EDIT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_008_MX_1 message handlers

BOOL CDLG_008_MX_1::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

INT CDLG_008_MX_1::InitData()
{
	m_strGroupName = _T("[MX]");

	// Alias ����
	// ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@0-5"),_T("�Է�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@6"),_T("���������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@7-10"),_T("�����1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@11-14"),_T("�����2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@15-17"),_T("���౹��ȣ"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@26-27"),_T("�ѱ����к�ȣ"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@28"),_T("�������ڵ�"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@32"),_T("�������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@35-37"),_T("���"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@38-39"),_T("���α����ȣ"));

	// �ڷẰ	
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@23"),_T("�����ڷ�����"));				

	// Control ����
	// ����
	m_mapControlAlias_ItemName.SetAt(_T("���������"), _T("(���������)"));
	m_mapControlAlias_ItemName.SetAt(_T("���౹��ȣ"), _T("���౹��ȣ"));
	m_mapControlAlias_ItemName.SetAt(_T("�ѱ����к�ȣ"), _T("�ѱ����к�ȣ"));
	m_mapControlAlias_ItemName.SetAt(_T("�������ڵ�"), _T("(�������ڵ�)"));
	m_mapControlAlias_ItemName.SetAt(_T("�������"), _T("(�������)"));
	m_mapControlAlias_ItemName.SetAt(_T("���"), _T("���"));	
	m_mapControlAlias_ItemName.SetAt(_T("���α����ȣ"), _T("���α����ȣ"));

	// �ڷẰ
	m_mapControlAlias_ItemName.SetAt(_T("�����ڷ�����"), _T("(�����ڷ� ����)"));	

	// ����
	m_mapControlAlias_ItemName.SetAt(_T("����_���ڵ����"), _T("(���ڵ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("����_���ڵ�����"), _T("(���ڵ�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("����_��������"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("����_��������"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("����_���ں�ȣü��"), _T("(���ں�ȣü��)"));
	m_mapControlAlias_ItemName.SetAt(_T("����_�Է¼���"), _T("(�Է¼���)"));
	m_mapControlAlias_ItemName.SetAt(_T("����_��ϱ������"), _T("(��ϱ������)"));
	m_mapControlAlias_ItemName.SetAt(_T("����_�ٱǺ��ڿ��Ƿ��ڵ����"), _T("(�ٱǺ� �ڿ��� ���ڵ� ����)"));

	// Extra ����
	INT ids = -1;
	CString strItemData;

	// 1. �Է³��ڰ� ������ ���� ���ڸ� �ִ´�.
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008@0-5"), strItemData);
	strItemData.TrimLeft();
	strItemData.TrimRight();
	if (ids < 0 || strItemData.IsEmpty())
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@0-5"), CTime::GetCurrentTime().Format(_T("%y%m%d")));
	// 2. ����� ������ ������ s�� �ִ´�.
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008@6"), strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		strItemData = _T("s");
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@6"), strItemData);
	}

	// s�� ���
	if (strItemData == _T("s"))
	{
		// 3. ������� marc�� 260$c�� �⵵�� �ִ´�. ������ ���� �⵵�� �ִ´�.
		ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("260$c"), strItemData);

		// �⵵ ���ڷ�
		CString strYear;
		INT nLength = strItemData.GetLength();
		TCHAR tch = 0;
		for (INT idx = 0; idx < nLength; idx++)
		{
			tch = strItemData.GetAt(idx);
			if (_istdigit(tch))
				strYear += tch;
		}

		if (ids < 0 || strYear.IsEmpty())
			strYear = CTime::GetCurrentTime().Format(_T("%Y"));
		
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@7-10"), strYear);
	}
	// 008 �ڸ����� 40���� �����.
	CString s008;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008*"), s008);
	if ( s008.GetLength() < 40 ) {
		while ( s008.GetLength() < 40 ) {
			s008 = s008 + _T(" ");
		}
	}
	m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008*"), s008 );

	// Control�� ��������
	Display();		

	// ����
	CString strLeader;
	m_pMarc->GetLeader(strLeader);

	CString strStatus;
	CString strForm;
	CString strSeoji;
	CString strControlType;
	CString strCharset;
	CString strInputLevel;
	CString strMok;
	CString strRelation;

	strStatus = strLeader.Mid(5,1);
	strForm = strLeader.Mid(6,1);
	strSeoji = strLeader.Mid(7,1);
	strControlType = strLeader.Mid(8,1);
	strCharset = strLeader.Mid(9,1);
	strInputLevel = strLeader.Mid(17,1);
	strMok = strLeader.Mid(18, 1);
	strRelation = strLeader.Mid(19, 1);

	if (strStatus != _T(" "))
		m_pCM->SetControlMgrData(_T("����_���ڵ����"), strStatus);	// ����->���ڵ���� (�������� 2004.10.08)

	if (strForm != _T(" "))
		m_pCM->SetControlMgrData(_T("����_���ڵ�����"), strForm);
	
	if (strSeoji != _T(" "))
		m_pCM->SetControlMgrData(_T("����_��������"), strSeoji);
	
	if (strControlType != _T(" "))
		m_pCM->SetControlMgrData(_T("����_��������"), strControlType);

	if (strCharset != _T(" "))
		m_pCM->SetControlMgrData(_T("����_���ں�ȣü��"), strCharset);
	
	if (strMok != _T(" "))
		m_pCM->SetControlMgrData(_T("����_��ϱ������"), strMok);

	if (strRelation != _T(" "))
		m_pCM->SetControlMgrData(_T("����_�ٱǺ��ڿ��Ƿ��ڵ����"), strRelation);
	

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_008_MX_1 message handlers

BOOL CDLG_008_MX_1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_FIXED_008_MX_1")) < 0)
	{
		m_strLastErrMsg = _T("InitESL_Mgr Error");
		goto ERR;
	}
	
	m_pCM = FindCM(_T("CM_FIXED_008_MX_1"));
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

VOID CDLG_008_MX_1::OnOK() 
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("�Է�����"));
	arrStrControlAlias.Add(_T("�����1"));
	arrStrControlAlias.Add(_T("���������"));
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return;
	
	ApplyControlData();

	// ���� �Է��ϱ�
	CString strLeader;
	m_pMarc->GetLeader(strLeader);

	CString strStatus;
	CString strForm;
	CString strSeoji;
	CString strControlType;
	CString strCharset;
	CString strInputLevel;
	CString strMok;
	CString strRelation;

	m_pCM->GetControlMgrData(_T("����_���ڵ����"), strStatus);
	m_pCM->GetControlMgrData(_T("����_���ڵ�����"), strForm);
	m_pCM->GetControlMgrData(_T("����_��������"), strSeoji);
	m_pCM->GetControlMgrData(_T("����_��������"), strControlType);
	m_pCM->GetControlMgrData(_T("����_���ں�ȣü��"), strCharset);
	m_pCM->GetControlMgrData(_T("����_�Է¼���"), strInputLevel);
	m_pCM->GetControlMgrData(_T("����_��ϱ������"), strMok);
	m_pCM->GetControlMgrData(_T("����_�ٱǺ��ڿ��Ƿ��ڵ����"), strRelation);


	CString sTmp;
	sTmp.Format(_T("%5s%1s%1s%1s%1s%1s%s%1s%1s%1s%s"), 
				strLeader.Left(5), 
				strStatus, 
				strForm, 
				strSeoji, 
				strControlType,
				strCharset,
				strLeader.Mid(10, 7), 
				strInputLevel, 
				strMok,
				strRelation,
				strLeader.Right(4));

	m_pMarc->SetLeader( sTmp.GetBuffer(0) );
	
	CDialog::OnOK();
}

BOOL CDLG_008_MX_1::PreTranslateMessage(MSG* pMsg) 
{
	return CCat008UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

void CDLG_008_MX_1::Onb007EDIT() 
{
	CDLG_FIXED_007 dlg(this);
	dlg.m_pMarc = m_pMarc;
	dlg.m_strGroupName = m_strGroupName;
	dlg.DoModal();		
}
HBRUSH CDLG_008_MX_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}