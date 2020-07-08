// MarcSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcSampleDlg.h"
#include "..\..\..\����\IndexToolKit\IndexTool.h"
#include "ESL_CharSetMgr.h"
#include "..\..\esl\DlgClassCode.h"
#include "..\MarcToolKit\MarcDefaultValueConfigDlg.h"
#include "..\..\KOLAS2UP_MAIN\KOLAS2UP_MAINApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcSampleDlg dialog

CMarcSampleDlg::CMarcSampleDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcSampleDlg)
	m_strOutput = _T("");
	m_strExample = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	AfxInitRichEdit();
}

BOOL CMarcSampleDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMarcSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcSampleDlg)
	DDX_Text(pDX, IDC_eOUTPUT, m_strOutput);
	DDX_Text(pDX, IDC_eSAMPLE, m_strExample);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMarcSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcSampleDlg)
	ON_BN_CLICKED(IDC_bENCODING, OnbENCODING)
	ON_BN_CLICKED(IDC_bDECODING, OnbDECODING)
	ON_BN_CLICKED(IDC_bEDIT_MARC, OnbEDITMARC)
	ON_BN_CLICKED(IDC_bSORT_MARC, OnbSORTMARC)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bINSERT1, OnbINSERT1)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bALIASTOFILE, OnbALIASTOFILE)
	ON_BN_CLICKED(IDC_bMARC2TEXT, OnbMARC2TEXT)
	ON_BN_CLICKED(IDC_bSETSTREAMMARC, OnbSETSTREAMMARC)
	ON_BN_CLICKED(IDC_bINDEX, OnbINDEX)
	ON_BN_CLICKED(IDC_INSERT_FIELD, OnInsertField)
	ON_BN_CLICKED(IDC_bCLASS_CODE, OnbCLASSCODE)
	ON_BN_CLICKED(IDC_bDELTE_FIELD, OnbDELTEFIELD)
	ON_BN_CLICKED(IDC_bDELTE_EMPTY_FIELD, OnbDELTEEMPTYFIELD)
	ON_BN_CLICKED(IDC_bSET_DEFAULT_MARC, OnbSETDEFAULTMARC)
	ON_BN_CLICKED(IDC_bCHECK_DEEP_SYNTAX, OnbCHECKDEEPSYNTAX)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcSampleDlg message handlers

BOOL CMarcSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	// Sample Start

	// Init Marc Manager
	m_pInfo->pMarcMgr->SetMarcGroup(_T("����"));

	// Init Marc Editor
	m_marcEditor.SubclassDlgItem(IDC_RICHEDIT, IDC_MSHFLEXGRID, this);
	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);
	m_marcEditor.SetExampleEditBox(GetDlgItem(IDC_eEXAMPLE)->GetSafeHwnd());

	// Read Sample Marc
	CStdioFile file;
	if (!file.Open(_T("TESTMARC.TXT"), CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("Test Marc Load Filed!"));
		return FALSE;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	file.ReadString(m_strStreamMarc);
	m_strStreamMarc.TrimLeft();
	m_strStreamMarc.TrimRight();

	m_strExample = 
		_T("$ : ���б�ȣ \r\n")
		_T("* : Tag item data \r\n")
		_T("@ : �ڸ��� (�����ʵ忡�� ����) \r\n")
		_T("EX)  001, *, \r\n")
		_T("     020, $, a	\r\n")
		_T("     008, @, 0-5 \r\n")
		_T("     008, @, 6 \r\n");

	UpdateData(FALSE);
		
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CMarcSampleDlg::ShowErrMsg()
{
	CString strTagCode, strErrMsg, strHelpMsg;

	POSITION pos;
	pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
	while (pos)
	{
		m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
		m_strOutput += ( strErrMsg + strHelpMsg + _T("\r\n"));
	}
	
	UpdateData(FALSE);
	
	CEdit *pEdit = ((CEdit*)GetDlgItem(IDC_eOUTPUT));
	pEdit->LineScroll(pEdit->GetLineCount());
}

///////////////////////////////////////////////////////////////////////////////
// Sample Code Start.~~~~~

VOID CMarcSampleDlg::OnbDECODING() 
{
	m_strOutput += _T("====Decode Start====\r\n");
	INT ids = m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc);

	if (ids < 0)
		ShowErrMsg();
	else
	{
		m_strOutput += m_strStreamMarc + _T("\r\n");
		m_strOutput += _T("====Decode Finish====\r\n");

		UpdateData(FALSE);
		CEdit *pEdit = ((CEdit*)GetDlgItem(IDC_eOUTPUT));
		pEdit->LineScroll(pEdit->GetLineCount());
	}	
}

VOID CMarcSampleDlg::OnbENCODING() 
{
	m_strOutput += _T("====Encode Start====\r\n");
	CString strStreamMarc;
	INT ids = m_pInfo->pMarcMgr->Encoding(&m_marc, strStreamMarc);
	if (ids < 0)
		ShowErrMsg();
	else
		m_strOutput += strStreamMarc + _T("\r\n");
	
	m_strOutput += _T("====Encode Finish====\r\n");
	UpdateData(FALSE);
	CEdit *pEdit = ((CEdit*)GetDlgItem(IDC_eOUTPUT));
	pEdit->LineScroll(pEdit->GetLineCount());
}

VOID CMarcSampleDlg::OnbEDITMARC() 
{
	m_marcEditor.InitUserAlias();
	m_marcEditor.AddUserAlias(_T("�����ȣ"));
	m_marcEditor.AddUserAlias(_T("����ISBN"));
	m_marcEditor.AddUserAlias(_T("����ISBN�ΰ���ȣ"));
	m_marcEditor.AddUserAlias(_T("��ƮISBN"));
	m_marcEditor.AddUserAlias(_T("��ƮISBN�ΰ���ȣ"));
	m_marcEditor.AddUserAlias(_T("��ǥ��"));
	m_marcEditor.AddUserAlias(_T("��ǥ��"));
	m_marcEditor.AddUserAlias(_T("���ǥ��"));
	m_marcEditor.AddUserAlias(_T("������"));
	m_marcEditor.AddUserAlias(_T("������"));
	m_marcEditor.AddUserAlias(_T("�ǻ���"));
	m_marcEditor.AddUserAlias(_T("��/����"));
	m_marcEditor.AddUserAlias(_T("����"));
	m_marcEditor.AddUserAlias(_T("������"));
	m_marcEditor.AddUserAlias(_T("������"));
	m_marcEditor.AddUserAlias(_T("�����"));
	m_marcEditor.AddUserAlias(_T("�����"));
	m_marcEditor.AddUserAlias(_T("������Ư��"));
	m_marcEditor.AddUserAlias(_T("ũ��"));
	m_marcEditor.AddUserAlias(_T("�����ڷ�"));
	m_marcEditor.AddUserAlias(_T("�Ѽ�ǥ��"));
	m_marcEditor.AddUserAlias(_T("�Ѽ�����"));
	m_marcEditor.AddUserAlias(_T("���ݼ���"));
	m_marcEditor.AddUserAlias(_T("����"));
	m_marcEditor.AddUserAlias(_T("����������"));
	m_marcEditor.AddUserAlias(_T("��Ϲ�ȣ"));
	m_marcEditor.AddUserAlias(_T("û����ȣ"));
	m_marcEditor.AddUserAlias(_T("������ȣ"));
	m_marcEditor.AddUserAlias(_T("��ġ��ȣ"));
	m_marcEditor.AddUserAlias(_T("Ű����"));
	m_marcEditor.AddUserAlias(_T("�Է³���"));
	m_marcEditor.AddUserAlias(_T("����"));
	m_marcEditor.AddUserAlias(_T("�����1"));
	m_marcEditor.AddUserAlias(_T("�����2"));
	m_marcEditor.AddUserAlias(_T("���౹��"));
	m_marcEditor.AddUserAlias(_T("��ǥ��"));
	m_marcEditor.AddUserAlias(_T("�ڷ�ĺ�ǥ��"));
	

	m_marcEditor.InitLimitTag();
	//m_marcEditor.AddLimitTag(_T("245"));

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();
}

VOID CMarcSampleDlg::OnbSORTMARC() 
{
	INT ids = m_pInfo->pMarcMgr->SortMarc(&m_marc);
	if (ids < 0)
		ShowErrMsg();
	else
		OnbEDITMARC();
}

VOID CMarcSampleDlg::OnbSEARCH() 
{
	CString strAlias;
	CString strItemData;
	GetDlgItem(IDC_eALIAS)->GetWindowText(strAlias);
	
	INT ids = m_pInfo->pMarcMgr->GetItem(&m_marc, strAlias, strItemData);
	if (ids < 0)
		GetDlgItem(IDC_eOUTPUT)->SetWindowText(m_marc.GetLastError());
	else
		GetDlgItem(IDC_eOUTPUT)->SetWindowText(strItemData);
}

VOID CMarcSampleDlg::OnbINSERT1() 
{
	CString strAlias;
	CString strItemData;
	GetDlgItem(IDC_eALIAS_KEYVALUE)->GetWindowText(strAlias);
	GetDlgItem(IDC_eITEMDATA)->GetWindowText(strItemData);

	INT ids = m_pInfo->pMarcMgr->SetItem(&m_marc, strAlias, strItemData,_T(""));
	if (ids < 0)
		ShowErrMsg();
	else
		OnbEDITMARC();
}

VOID CMarcSampleDlg::OnbDELETE() 
{
	CString strAlias;
	CString strItemData;
	GetDlgItem(IDC_eALIAS_KEYVALUE)->GetWindowText(strAlias);
	GetDlgItem(IDC_eITEMDATA)->GetWindowText(strItemData);

	INT ids = m_pInfo->pMarcMgr->DeleteItem(&m_marc, strAlias, strItemData);
	if (ids < 0)
		ShowErrMsg();
	else
		OnbEDITMARC();
}

VOID CMarcSampleDlg::OnbALIASTOFILE() 
{
	CMarcAliasToFile dlg;
	dlg.DoModal();
}

VOID CMarcSampleDlg::OnbMARC2TEXT() 
{
	CMarcRelationToText marcRelationToText;
	marcRelationToText.DoModal();		
}

VOID CMarcSampleDlg::OnbSETSTREAMMARC() 
{
	GetDlgItem(IDC_eOUTPUT)->GetWindowText(m_strStreamMarc);	
}

VOID CMarcSampleDlg::OnbINDEX() 
{
	CString strIndexAlias;
	CString strIndexItem;
	GetDlgItem(IDC_eINDEX_ALIAS)->GetWindowText(strIndexAlias);
	
	CIndexTool indexTool;
	indexTool.Init(m_pInfo->pMarcMgr, &m_marc);
	INT ids = indexTool.GetIndex(strIndexAlias, strIndexItem);
	 if (ids < 0)
		GetDlgItem(IDC_eOUTPUT)->SetWindowText(indexTool.GetLastError());
	else
		GetDlgItem(IDC_eOUTPUT)->SetWindowText(strIndexItem);
}

VOID CMarcSampleDlg::OnInsertField() 
{
	CString strField;
	GetDlgItem(IDC_eINDEX_ALIAS)->GetWindowText(strField);
	INT ids = m_pInfo->pMarcMgr->InsertField(&m_marc, strField);
	if (ids >=0 )
	{
		OnbEDITMARC();
	}
}

VOID CMarcSampleDlg::OnbCLASSCODE() 
{
	/*for (INT i = 0; i <= 100; i++)
	{
		pMain->GetParentFrame()->SendMessage(WM_SET_PROGRESS_POS, i, 100);
		Sleep(100);
	}
	*/	

	//CString strText = _T("��ְ�");
	//pMain->GetParentFrame()->SendMessage(WM_SET_PROGRESS_TEXT, (WPARAM)&strText, 100);
	/*CDlgClassCode dlg;
	dlg.m_strKeyword = _T("a");
	dlg.AddData(_T("A"), _T("B"));
	if (dlg.DoModal() == IDOK)
	{
		AfxMessageBox(dlg.m_strSelCode + dlg.m_strSelDesc);
	}
	*/
}

VOID CMarcSampleDlg::OnbDELTEFIELD() 
{
	CString strTagCode;
	GetDlgItem(IDC_eFIELD_TAG_CODE)->GetWindowText(strTagCode);
	INT ids = m_pInfo->pMarcMgr->DeleteField(&m_marc, strTagCode);
	if (ids >=0 )
	{
		OnbEDITMARC();
	}	
}

VOID CMarcSampleDlg::OnbDELTEEMPTYFIELD() 
{
	INT ids = m_pInfo->pMarcMgr->DeleteEmptyField(&m_marc);
	if (ids >= 0)
	{
		OnbEDITMARC();
	}
}

VOID CMarcSampleDlg::OnbSETDEFAULTMARC() 
{
	CMarcDefaultValueConfigDlg dlg(this);
	dlg.DoModal();
}

VOID CMarcSampleDlg::OnbCHECKDEEPSYNTAX() 
{
	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
	if (ids >= 0)
	{
		OnbEDITMARC();
	}
}
