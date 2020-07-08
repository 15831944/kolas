// Cat008FixedField_TX.cpp : implementation file
//

#include "stdafx.h"
#include "Cat008FixedField_TX.h"
#include "Cat007FieldContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_TX dialog


CCat008FixedField_TX::CCat008FixedField_TX(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat008FixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat008FixedField_TX)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CCat008FixedField_TX::~CCat008FixedField_TX()
{
}

VOID CCat008FixedField_TX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat008FixedField_TX)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCat008FixedField_TX, CDialog)
	//{{AFX_MSG_MAP(CCat008FixedField_TX)
	ON_BN_CLICKED(IDC_b007EDIT, Onb007EDIT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_TX Operations

INT CCat008FixedField_TX::InitData()
{
	m_strGroupName = _T("[TX]");

	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@0-5"),_T("�Է³�¥"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@6"),_T("���������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@7-10"),_T("�����1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@11-14"),_T("�����2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@15-17"),_T("���౹��ȣ"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@18"),_T("��ǥ��1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@19"),_T("��ǥ��2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@20"),_T("��ǥ��3"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@21"),_T("��ǥ��4"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@22"),_T("�̿����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@23"),_T("�ڷ�ĺ�"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@24"),_T("��������1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@25"),_T("��������2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@26-27"),_T("�ѱ����к�ȣ"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@28"),_T("����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@29"),_T("ȸ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@30"),_T("����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@31"),_T("����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@32"),_T("�������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@33"),_T("��������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@34"),_T("����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@35-37"),_T("����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@38-39"),_T("���α����ȣ"));

	// Control ����
	m_mapControlAlias_ItemName.SetAt(_T("���������"), _T("(���������)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ǥ��1"), _T("(��ǥ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ǥ��2"), _T("(��ǥ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ǥ��3"), _T("(��ǥ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ǥ��4"), _T("(��ǥ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("�̿����"), _T("(�̿����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�ڷ�ĺ�"), _T("(�ڷ�ĺ�)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������1"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������2"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("����"), _T("(����)"));
	m_mapControlAlias_ItemName.SetAt(_T("ȸ��"), _T("(ȸ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("����"), _T("(����)"));
	m_mapControlAlias_ItemName.SetAt(_T("����"), _T("(����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�������"), _T("(�������)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("����"), _T("(����)"));
	
	m_mapControlAlias_ItemName.SetAt(_T("���ڵ����"), _T("(���ڵ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("���ڵ�����"), _T("(���ڵ�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������"), _T("(��������)����"));
	m_mapControlAlias_ItemName.SetAt(_T("�Է¼���"), _T("(�Է¼���)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ϱ������"), _T("(��ϱ������)"));
	m_mapControlAlias_ItemName.SetAt(_T("�������ڵ�����"), _T("(�������ڵ�����)"));

	m_mapControlAlias_ItemName.SetAt(_T("���౹��ȣ"), _T("���౹��ȣ"));
	m_mapControlAlias_ItemName.SetAt(_T("�ѱ����к�ȣ"), _T("�ѱ����к�ȣ"));
	m_mapControlAlias_ItemName.SetAt(_T("����"), _T("���"));
	m_mapControlAlias_ItemName.SetAt(_T("���α����ȣ"), _T("���α����ȣ"));


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
	CString strInputLevel;
	CString strMok;
	CString strRelation;

	strStatus = strLeader.Mid(5,1);
	strForm = strLeader.Mid(6,1);
	strSeoji = strLeader.Mid(7,1);
	strInputLevel = strLeader.Mid(17,1);
	strMok = strLeader.Mid(18, 1);
	strRelation = strLeader.Mid(19, 1);

	if (strStatus != _T(" "))
		m_pCM->SetControlMgrData(_T("���ڵ����"), strStatus);	// ����->���ڵ���� (�������� 2004.10.08)

	if (strForm != _T(" "))
		m_pCM->SetControlMgrData(_T("���ڵ�����"), strForm);
	
	if (strSeoji != _T(" "))
		m_pCM->SetControlMgrData(_T("��������"), strSeoji);
	
	if (strInputLevel != _T(" "))
		m_pCM->SetControlMgrData(_T("�Է¼���"), strInputLevel);
	
	if (strMok != _T(" "))
		m_pCM->SetControlMgrData(_T("��ϱ������"), strMok);

	if (strRelation != _T(" "))
		m_pCM->SetControlMgrData(_T("�������ڵ�����"), strRelation);

	// ���������� ������ �����̸� s �ƴϸ� m�� �־��ش�.
	if (strSeoji == _T(" "))
	{
		if (m_pInfo->pMarcMgr->GetMarcGroup() == _T("����"))
			strSeoji = _T("s");
		else
			strSeoji = _T("m");
		
		m_pCM->SetControlMgrData(_T("��������"), strSeoji);
	}


	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_TX message handlers

BOOL CCat008FixedField_TX::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_01")) < 0)
	{
		m_strLastErrMsg = _T("InitESL_Mgr Error");
		goto ERR;
	}
	
	m_pCM = FindCM(_T("CM_BO_CAT_PROC_0320_01"));
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

VOID CCat008FixedField_TX::OnOK() 
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("�Է³�¥"));
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
	CString strInputLevel;
	CString strMok;
	CString strRelation;

	m_pCM->GetControlMgrData(_T("���ڵ����"), strStatus);
	m_pCM->GetControlMgrData(_T("���ڵ�����"), strForm);
	m_pCM->GetControlMgrData(_T("��������"), strSeoji);
	m_pCM->GetControlMgrData(_T("�Է¼���"), strInputLevel);
	m_pCM->GetControlMgrData(_T("��ϱ������"), strMok);
	m_pCM->GetControlMgrData(_T("�������ڵ�����"), strRelation);


	CString sTmp;
	sTmp.Format(_T("%5s%1s%1s%1s%s%1s%1s%1s%s"), 
				strLeader.Left(5), 
				strStatus, 
				strForm, 
				strSeoji, 
				strLeader.Mid(8, 9), 
				strInputLevel, 
				strMok,
				strRelation,
				strLeader.Right(4));

	m_pMarc->SetLeader( sTmp.GetBuffer(0) );
	
	CDialog::OnOK();
}

BOOL CCat008FixedField_TX::PreTranslateMessage(MSG* pMsg) 
{
	return CCat008FixedFieldMgr::PreTranslateMessage(pMsg);
}

VOID CCat008FixedField_TX::Onb007EDIT() 
{
	CCat007FieldContainer dlg(this);
	dlg.m_pMarc = m_pMarc;
	dlg.m_strGroupName = m_strGroupName;
	dlg.DoModal();
}

HBRUSH CCat008FixedField_TX::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
