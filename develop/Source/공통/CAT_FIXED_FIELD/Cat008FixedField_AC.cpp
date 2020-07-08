// Cat008FixedField_AC.cpp : implementation file
//

#include "stdafx.h"
#include "Cat008FixedField_AC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_AC dialog


CCat008FixedField_AC::CCat008FixedField_AC(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat008FixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat008FixedField_AC)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CCat008FixedField_AC::~CCat008FixedField_AC()
{
}

VOID CCat008FixedField_AC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat008FixedField_AC)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCat008FixedField_AC, CDialog)
	//{{AFX_MSG_MAP(CCat008FixedField_AC)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_AC message handlers

BOOL CCat008FixedField_AC::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_FORM_13")) < 0)
	{
		m_strLastErrMsg = _T("InitESL_Mgr Error");
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_BO_CAT_PROC_0320_FORM_13"));
	if (!m_pCM) return FALSE;
	
	//2006-01-09 Ȳ����
	if (Init() < 0) return FALSE;
	if (InitData() < 0) return FALSE;
	
	//m_pCM->
	SetWindowText(_T("�������� ��������"));
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CCat008FixedField_AC::InitData()
{
EFS_BEGIN

	//���� ����
	m_strGroupName = _T("[A]");

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Alias ����
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@0-5"),_T("�Է³�¥"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@6"),_T("��������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@7"),_T("�θ��ڹ���ǥ"));
	//8 ��ĭ
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@9"),_T("���ڵ�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@10"),_T("��ϱ������"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@11"),_T("������ǥ��ý���"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@12"),_T("�Ѽ�����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@13"),_T("�Ѽ���ȣ����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@14"),_T("ǥ����-��ǥ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@15"),_T("��������ǥ��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@16"),_T("ǥ����-�Ѽ���"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@17"),_T("������������"));
	//18~27 ��ĭ
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@28"),_T("���ڵ����"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@29"),_T("������"));
	//30 ��ĭ
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@31"),_T("���ڵ尻��"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@32"),_T("����ۼ������ȣ"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@33"),_T("ä��ǥ�����"));
	//34~38 ��ĭ
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@39"),_T("��������"));

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Control ����
	m_mapControlAlias_ItemName.SetAt(_T("��������"), _T("(��������)"));
	m_mapControlAlias_ItemName.SetAt(_T("�θ��ڹ���ǥ"), _T("(�θ��ڹ���ǥ)"));
	
	m_mapControlAlias_ItemName.SetAt(_T("���ڵ�����"), _T("(���ڵ�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("��ϱ������"), _T("(��ϱ������)"));
	m_mapControlAlias_ItemName.SetAt(_T("������ǥ��ý���"), _T("(������ǥ��ý���)"));
	m_mapControlAlias_ItemName.SetAt(_T("�Ѽ�����"), _T("(�Ѽ�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�Ѽ���ȣ����"), _T("(�Ѽ���ȣ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("ǥ����-��ǥ��"), _T("(ǥ����-��ǥ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������ǥ��"), _T("(��������ǥ��)"));
	m_mapControlAlias_ItemName.SetAt(_T("ǥ����-�Ѽ���"), _T("(ǥ����-�Ѽ���)"));
	m_mapControlAlias_ItemName.SetAt(_T("������������"), _T("(������������)"));
	
	m_mapControlAlias_ItemName.SetAt(_T("���ڵ����"), _T("(���ڵ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("������"), _T("(������)"));

	m_mapControlAlias_ItemName.SetAt(_T("���ڵ尻��"), _T("(���ڵ尻��)"));
	m_mapControlAlias_ItemName.SetAt(_T("����ۼ������ȣ"), _T("(����ۼ������ȣ)"));
	m_mapControlAlias_ItemName.SetAt(_T("ä��ǥ�����"), _T("(ä��ǥ�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("��������"), _T("(��������)"));

	m_mapControlAlias_ItemName.SetAt(_T("���ڵ����"), _T("(���ڵ����)"));
	m_mapControlAlias_ItemName.SetAt(_T("���ڵ�����"), _T("(���ڵ�����)"));
	m_mapControlAlias_ItemName.SetAt(_T("�Է¼���"), _T("(�Է¼���)"));

/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Extra ����
	INT ids = -1;
	CString strItemData = _T("");

	// 1. �Է³��ڰ� ������ ���� ���ڸ� �ִ´�.
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008@0-5"), strItemData);
	strItemData.TrimLeft();
	strItemData.TrimRight();
	if (ids < 0 || strItemData.IsEmpty())
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@0-5"), CTime::GetCurrentTime().Format(_T("%y%m%d")));

	//GetDlgItem(IDC_eINPUT_DATE)->SetWindowText(CTime::GetCurrentTime().Format(_T("%y%m%d")));	


	// 3. ������� marc�� 260$c�� �⵵�� �ִ´�. ������ ���� �⵵�� �ִ´�.
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("260$c"), strItemData);
	
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

	return 0;


EFS_END
return FALSE;
}

VOID CCat008FixedField_AC::OnOk() 
{
EFS_BEGIN

	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("�Է³�¥"));

	if ( ValidateControlData(&arrStrControlAlias) < 0 ) return;
	
	ApplyControlData();
	
	// ���� �Է��ϱ�
	CString strLeader;
	m_pMarc->GetLeader(strLeader);
	
	CString strStatus;
	CString strForm;
	CString strSeoji;
	CString strInputLevel;
	
	m_pCM->GetControlMgrData(_T("���ڵ����"), strStatus);
	m_pCM->GetControlMgrData(_T("���ڵ�����"), strForm );
	//m_pCM->GetControlMgrData(_T("��������"), strSeoji );
	m_pCM->GetControlMgrData(_T("�Է¼���"), strInputLevel );
	
	CString sTmp;
	sTmp.Format(_T("%5s%1s%1s %s%1s%s"), 
		strLeader.Left(5), 
		strStatus, 
		strForm, 
		//strSeoji, 
		strLeader.Mid(8, 9), 
		strInputLevel, 
		strLeader.Right(6));
	
	m_pMarc->SetLeader( sTmp.GetBuffer(0) );
	
	CDialog::OnOK();	
	
EFS_END
}

VOID CCat008FixedField_AC::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CCat008FixedField_AC::PreTranslateMessage(MSG* pMsg) 
{
	return CCat008FixedFieldMgr::PreTranslateMessage(pMsg);
}

HBRUSH CCat008FixedField_AC::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
