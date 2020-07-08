// Cat008FixedField_SE.cpp : implementation file
//

#include "stdafx.h"
#include "Cat008FixedField_SE.h"
#include "Cat007FieldContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_SE dialog


CCat008FixedField_SE::CCat008FixedField_SE(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat008FixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat008FixedField_SE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CCat008FixedField_SE::~CCat008FixedField_SE()
{
}

VOID CCat008FixedField_SE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat008FixedField_SE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCat008FixedField_SE, CDialog)
	//{{AFX_MSG_MAP(CCat008FixedField_SE)
	ON_BN_CLICKED(IDC_b007EDIT, Onb007EDIT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_SE Operations

INT CCat008FixedField_SE::InitData()
{
	m_strGroupName = _T("[S]");

	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@0-5"),_T("�Է³�¥"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@6"),_T("���������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@7-10"),_T("�����1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@11-14"),_T("�����2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@15-17"),_T("���౹��ȣ"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@18"),_T("�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@19"),_T("���Լ�"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@20"),_T("ISSN���ͺ�ȣ"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@21"),_T("���Ӱ��๰����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@22"),_T("���ڷ��ǹ߰�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@23"),_T("�ش��ڷ��ǹ߰�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@24"),_T("��������1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@25"),_T("��������2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@26-27"),_T("�ѱ����к�ȣ"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@28"),_T("����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@29"),_T("ȸ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@32"),_T("�������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@33"),_T("ǥ���Ǿ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@34"),_T("��ϱ�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@35-37"),_T("����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@38-39"),_T("���α����ȣ"));

	// Control ����
	m_mapControlAlias_ItemName.SetAt(_T("���������"), _T("(���������)"));
	m_mapControlAlias_ItemName.SetAt(_T("�����"), _T("(�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("���Լ�"), _T("(���Լ�)"));
	m_mapControlAlias_ItemName.SetAt(_T("ISSN���ͺ�ȣ"), _T("(ISSN���ͺ�ȣ)"));
	m_mapControlAlias_ItemName.SetAt(_T("���Ӱ��๰����"), _T("(����������)"));
	m_mapControlAlias_ItemName.SetAt(_T("���ڷ��ǹ߰�����"), _T("(���ڷ�߰�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�ش��ڷ��ǹ߰�����"), _T("(�ش��ڷ�߰�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������1"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������2"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("����"), _T("(�������ڵ�)"));
	m_mapControlAlias_ItemName.SetAt(_T("ȸ��"), _T("(ȸ�ǰ��๰)"));
	m_mapControlAlias_ItemName.SetAt(_T("�������"), _T("(�������)"));
	m_mapControlAlias_ItemName.SetAt(_T("ǥ���Ǿ��"), _T("(ǥ���Ǿ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ϱ�����"), _T("(��ϱ�����)"));
	
	m_mapControlAlias_ItemName.SetAt(_T("���ڵ����"), _T("(���ڵ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("���ڵ�����"), _T("(���ڵ�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������"), _T("(��������)����"));
	m_mapControlAlias_ItemName.SetAt(_T("�Է¼���"), _T("(�Է¼���)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ϱ������"), _T("(��ϱ������)"));

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

	// 1) ������� - ����� ��������
	CString str260CData;
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("260$c"), str260CData);

	// 2) �⵵ ���ڷ�
	CString strYear1,strYear2,strTmp;
	strYear1 = strYear2 = _T("");
	BOOL bIsFirst = TRUE;
	INT nLength = str260CData.GetLength();
	for (INT idx = 0; idx < nLength; idx++)
	{
		strTmp = str260CData.Mid(idx, 1);
		if ( !strTmp.Compare(_T("u")) || (strTmp >= '0' && strTmp <= '9') )
		{
			if (bIsFirst) strYear1 += strTmp;
			else strYear2 += strTmp;
		}
		else if ( strYear1.GetLength() >0 && !strTmp.Compare(_T("-")) ) bIsFirst = FALSE;
	}

	// 2. ����� ������ ������ �󰣳��� ������ c, ������ d�� �ִ´�.
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008@6"), strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		// �󰣳��� ������ 
		if ( strYear2.GetLength()>0) strItemData = _T("d");
		else strItemData = _T("c");
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@6"), strItemData);
	}

	// 3. ������� marc�� 260$c�� �⵵�� sync

	if (strItemData == _T("c") || strItemData == _T("u"))	// ������ ���๰ / �̻�
	{
		// ������ ���� �⵵�� �ִ´�.
		if (ids < 0 || strYear1.IsEmpty()) strYear1 = CTime::GetCurrentTime().Format(_T("%Y"));
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@7-10"), strYear1);

		// ��Ӱ���/�̻��� ��� �����2�� �׻� 9999�̴�
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@11-14"), _T("9999"));
	}
	else if (strItemData == _T("d"))	// �󰣵� ���๰
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@7-10"), strYear1);
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@11-14"), strYear2);
	}

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

	strStatus = strLeader.Mid(5,1);
	strForm = strLeader.Mid(6,1);
	strSeoji = strLeader.Mid(7,1);
	strInputLevel = strLeader.Mid(17,1);
	strMok = strLeader.Mid(18, 1);

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
// CCat008FixedField_SE message handlers

BOOL CCat008FixedField_SE::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("BO_CAT_PROC_0326")) < 0)
	{
		m_strLastErrMsg = _T("InitESL_Mgr Error");
		goto ERR;
	}

	m_pCM = FindCM(_T("CM_BO_CAT_PROC_0326"));
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

VOID CCat008FixedField_SE::OnOK() 
{
	// 040311_HSS �߰� -> �����1, 2�� ����+u�� �� �� �ֵ��� ex) 19uu, 198u ++++++++++++++
	CString strYear, strTmp;
	INT i;
	m_pCM->GetControlMgrData(_T("�����1"), strYear);
	for (i=0; i<strYear.GetLength(); i++)
	{
		strTmp = strYear.Mid(i, 1);
		if (strTmp.Compare(_T("u")) && (strTmp < '0' || strTmp > '9') ){
			AfxMessageBox(_T("�����1���� ���� �� 'u'�� �Է��� �� �ֽ��ϴ�.\n�� : 1999, 19uu"));
			return;
		}
	}
	m_pCM->GetControlMgrData(_T("�����2"), strYear);
	for (i=0; i<strYear.GetLength(); i++)
	{
		strTmp = strYear.Mid(i, 1);
		if (strTmp.Compare(_T("u")) && (strTmp < '0' || strTmp > '9') ){
			AfxMessageBox(_T("�����2���� ���� �� 'u'�� �Է��� �� �ֽ��ϴ�.\n�� : 1999, 19uu"));
			return;
		}
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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

	m_pCM->GetControlMgrData(_T("���ڵ����"), strStatus);
	m_pCM->GetControlMgrData(_T("���ڵ�����"), strForm );
	m_pCM->GetControlMgrData(_T("��������"), strSeoji );
	m_pCM->GetControlMgrData(_T("�Է¼���"), strInputLevel );
	m_pCM->GetControlMgrData(_T("��ϱ������"), strMok );

	CString sTmp;
	sTmp.Format(_T("%5s%1s%1s%1s%s%1s%1s%s"), 
				strLeader.Left(5), 
				strStatus, 
				strForm, 
				strSeoji, 
				strLeader.Mid(8, 9), 
				strInputLevel,
				strMok,
				strLeader.Right(5));

	m_pMarc->SetLeader( sTmp.GetBuffer(0) );
	
	CDialog::OnOK();
}

BOOL CCat008FixedField_SE::PreTranslateMessage(MSG* pMsg) 
{
	return CCat008FixedFieldMgr::PreTranslateMessage(pMsg);
}

VOID CCat008FixedField_SE::Onb007EDIT() 
{
	CCat007FieldContainer dlg(this);
	dlg.m_pMarc = m_pMarc;
	dlg.m_strGroupName = m_strGroupName;
	dlg.DoModal();
}

HBRUSH CCat008FixedField_SE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
