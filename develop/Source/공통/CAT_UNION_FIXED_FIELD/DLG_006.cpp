// DLG_006.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_006.h"
#include "FixedFieldInfo.h"

#include "DLG_006_BK.h"
#include "DLG_006_CR.h"
#include "DLG_006_ER.h"
#include "DLG_006_MP.h"
#include "DLG_006_MU.h"
#include "DLG_006_MX.h"
#include "DLG_006_RB.h"
#include "DLG_006_VM.h"

// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
// ���ո�ũ ���� ����
/*//NEW------------------------------------------------------------------------*/
#include "DLG_006_BK_1.h"
#include "DLG_006_MU_1.h"
#include "DLG_006_CR_1.h"
#include "DLG_006_VM_1.h"
#include "DLG_006_MX_1.h"
#include "DLG_006_ER_1.h"
#include "DLG_006_MP_1.h"
#include "DLG_006_RB_1.h"
/*//END------------------------------------------------------------------------*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_006 dialog


CDLG_006::CDLG_006(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CESL_Mgr(IDD, pParent), m_MultiComboBox(NULL)
{
	//{{AFX_DATA_INIT(CDLG_006)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
	m_lpszInfoFileName = _T("..\\CFG\\UNION_008TAG�ڵ�.txt");

	m_p006Form = NULL;
}

CDLG_006::~CDLG_006()
{
}

void CDLG_006::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_006)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDLG_006, CDialog)
	//{{AFX_MSG_MAP(CDLG_006)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_cMAT_CATEGORY, OnSelchangecMATCATEGORY)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_006 message handlers
VOID CDLG_006::FreeData()
{
	CFixedFieldInfo *pInfo = NULL;

	// 006 �����
	POSITION pos = m_listInfo.GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_listInfo.GetNext(pos));
		delete pInfo;
	}
	m_listInfo.RemoveAll();
}

INT CDLG_006::Init()
{
	INT ids = -1;

	FreeData();
	ids = LoadInfo(m_lpszInfoFileName);
	if (ids < 0) return -1;	
	
	m_mapMatCatCode_MatCatDesc.SetAt(_T("a"), _T("�����ڷ�"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("c"), _T("�ʻ�Ǻ� �̿��� �Ǻ�"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("d"), _T("�ʻ�Ǻ�"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("e"), _T("�����ڷ�"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("f"), _T("�ʻ�����"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("g"), _T("��鿵���ڷ�"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("i"), _T("�����ڷ�(���� �̿�)"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("j"), _T("�����ڷ�(����)"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("k"), _T("��鿵���ڷ�"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("m"), _T("�����ڷ�"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("o"), _T("ŰƮ"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("p"), _T("�����ڷ�"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("r"), _T("��ü�ڷ�(�ǹ�)"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("s"), _T("���Ӱ��๰/�����ڷ�"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("t"), _T("�ʻ繮���ڷ�"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("y"), _T("��"));

	// ComboBox Init
	ids  = InitMultiComboBox();
	if (ids < 0) return ids;

	CreateScreen();

	return 0;
}

INT CDLG_006::InitMultiComboBox()
{
	// Map �� ������ Code�� ������ �����ش�.
	m_MultiComboBox.FreeComboData();
	m_MultiComboBox.PrevRow = -1;
	m_MultiComboBox.PrevCol = -1;
	m_MultiComboBox.m_nMaxRowCnt = 0;

	m_MultiComboBox.ResetContent();
	m_MultiComboBox.InitMultiComboBox(2);

	// 2008.06.26 UPDATE BY PDJ : ���� ����ġ�� ���� �ִ´�.
	m_MultiComboBox.AddItem(_T("a"), 0, 0); m_MultiComboBox.AddItem(_T("�����ڷ�")				, 1, 0);
	m_MultiComboBox.AddItem(_T("c"), 0, 1); m_MultiComboBox.AddItem(_T("�ʻ�Ǻ� �̿��� �Ǻ�")	, 1, 1);
	m_MultiComboBox.AddItem(_T("d"), 0, 2); m_MultiComboBox.AddItem(_T("�ʻ�Ǻ�")				, 1, 2);
	m_MultiComboBox.AddItem(_T("e"), 0, 3); m_MultiComboBox.AddItem(_T("�����ڷ�")				, 1, 3);
	m_MultiComboBox.AddItem(_T("f"), 0, 4); m_MultiComboBox.AddItem(_T("�ʻ�����")				, 1, 4);
	m_MultiComboBox.AddItem(_T("g"), 0, 5); m_MultiComboBox.AddItem(_T("��鿵���ڷ�")			, 1, 5);
	m_MultiComboBox.AddItem(_T("i"), 0, 6); m_MultiComboBox.AddItem(_T("�����ڷ�(���� �̿�)")	, 1, 6);
	m_MultiComboBox.AddItem(_T("j"), 0, 7); m_MultiComboBox.AddItem(_T("�����ڷ�(����)")		, 1, 7);
	m_MultiComboBox.AddItem(_T("k"), 0, 8); m_MultiComboBox.AddItem(_T("���񿵻��ڷ�")		, 1, 8);
	m_MultiComboBox.AddItem(_T("m"), 0, 9); m_MultiComboBox.AddItem(_T("�����ڷ�")				, 1, 9);
	m_MultiComboBox.AddItem(_T("o"), 0,10); m_MultiComboBox.AddItem(_T("ŰƮ")					, 1,10);
	m_MultiComboBox.AddItem(_T("p"), 0,11); m_MultiComboBox.AddItem(_T("�����ڷ�")				, 1,11);
	m_MultiComboBox.AddItem(_T("r"), 0,12); m_MultiComboBox.AddItem(_T("��ü�ڷ�(�ǹ�)")		, 1,12);
	m_MultiComboBox.AddItem(_T("s"), 0,13); m_MultiComboBox.AddItem(_T("���Ӱ��๰/�����ڷ�")	, 1,13);
	m_MultiComboBox.AddItem(_T("t"), 0,14); m_MultiComboBox.AddItem(_T("�ʻ繮���ڷ�")			, 1,14);
	m_MultiComboBox.AddItem(_T("y"), 0,15); m_MultiComboBox.AddItem(_T("��")					, 1,15);
	 
	/* 0, 0);
	INT idx = 0;
	CString strCode;
	CString strDesc;
	POSITION pos = m_mapMatCatCode_MatCatDesc.GetStartPosition();	
	while (pos)
	{
		m_mapMatCatCode_MatCatDesc.GetNextAssoc(pos, strCode, strDesc);
		m_MultiComboBox.AddItem(strCode, 0, idx);
		m_MultiComboBox.AddItem(strDesc, 1, idx);	

		idx++;
	}
	*/
	
	// Set Sel�Ѵ�.
	CString strCat;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("006@0"), strCat);
	strCat.TrimLeft();
	strCat.TrimRight();
	if (strCat.IsEmpty())
		AfxMessageBox(_T("��ġ�ϴ� �ڷ���ְ� �����ϴ�.") );

	m_MultiComboBox.SelectString(strCat);
	return 0;
}

INT CDLG_006::LoadInfo(LPCTSTR lpszFileName)
{
	CStdioFile file;
	// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
	// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary)) return -1;
*///AFTER------------------------------------------------------------------------
	if (_T("..\\CFG\\UNION_008TAG�ڵ�.txt") == lpszFileName) {
		CString strFileName = lpszFileName;
		CString strGetGroupName = _T("");
		CString strGroupFileName = _T("..\\cfg\\MarcRelation\\���ո�ũ_����_�׷��.cfg");
		CFileFind find;
		if (TRUE == find.FindFile(strGroupFileName)) {
			CStdioFile file;
			if (TRUE == file.Open(strGroupFileName, CFile::modeRead|CFile::typeBinary)) {
				TCHAR cUni;
				file.Read(&cUni, sizeof(TCHAR));
				if (0xFEFF != cUni) file.SeekToBegin();
				CString strTempData, strGetData;
				while (file.ReadString(strGetData)) {
					strGetData.TrimLeft();
					if (strGetData.IsEmpty()) continue;
					if (_T("#") == strGetData.Left(1)) continue;
					INT nPos = strGetData.Find(_T("="));
					if (0 <= nPos) {
						strTempData = strGetData.Left(nPos);
						strTempData.TrimLeft(); strTempData.TrimRight();
						if (_T("�׷��") == strTempData) {
							strGetGroupName = strGetData.Mid(nPos+1, strGetData.GetLength());
							strGetGroupName.TrimLeft(); strGetGroupName.TrimRight();
							break;
						}
					}
				}
				file.Close ();
			}
			find.Close();
		}
		if (!strGetGroupName.IsEmpty()) {
			strFileName = _T("..\\CFG\\UNION_008TAG�ڵ�_") + strGetGroupName +_T(".txt");
		}
		if (!file.Open(strFileName, CFile::modeRead | CFile::typeBinary)) return -1;
	} else {
		if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary)) return -1;
	}
/*//END------------------------------------------------------------------------*/
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT nFind = -1;
	CString strLine;
	CString strGroupName;
	CString strTmp;
	DWORD dwPos = 0;
	CFixedFieldInfo *pInfo = NULL;
	while (file.ReadString(strLine))
	{		
		strLine.TrimRight();

		if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;

		// Group
		if (strLine.Left(1) == _T("["))
		{
			// Position ����
			dwPos = file.GetPosition();

			strGroupName = strLine;
			// Item
			while (file.ReadString(strLine))
			{				
				strLine.TrimRight();
				if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
				if (strLine.Left(1) == _T("[")) 
				{
					// Position ��������
					file.Seek(dwPos, CFile::begin);
					break;
				}
				if (strLine.Left(1) == _T("("))
				{
					// Position ����
					dwPos = file.GetPosition();

					// Item �߰�
					pInfo = new CFixedFieldInfo(strGroupName, strLine);
					m_listInfo.AddTail(pInfo);

					// Code, Desc �ֱ�
					while (file.ReadString(strLine))
					{						
						strLine.TrimRight();
						if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
						if (strLine.Left(1) == _T("(") || strLine.Left(1) == _T("[")) 
						{
							// Position ��������
							file.Seek(dwPos, CFile::begin);
							break;
						}

						nFind = strLine.Find(_T(":"));
						if (nFind < 0) continue;

						strTmp = strLine.Mid(0, nFind);
						strTmp.Replace(_T("~"), _T("|"));


						pInfo->AddItem(strTmp, strLine.Mid(nFind + 1));
					}
				}
			}
		}		
	}

	file.Close();
	return 0;
}

INT CDLG_006::CreateScreen()
{
	delete m_p006Form;
	m_p006Form = NULL;

	m_MultiComboBox.GetData(m_strSubGroupName);

	CRect rcWnd;
	GetClientRect(rcWnd);
	rcWnd.DeflateRect(5, 50);

	// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
	// ���ո�ũ ���� ����
/*//NEW------------------------------------------------------------------------*/
	CString strFileName = _T("..\\cfg\\MarcRelation\\���ո�ũ_����_�׷��.cfg");
	CFileFind find;
	BOOL bRevision = find.FindFile(strFileName);
	find.Close();
/*//END------------------------------------------------------------------------*/
	
	if (m_strSubGroupName == _T("a"))
	{
		m_strGroupName = _T("[BK]");	
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_BK(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_BK_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_BK(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}	
	else if (m_strSubGroupName == _T("c"))
	{
		m_strGroupName = _T("[MU]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MU(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MU_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MU(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("d"))
	{
		m_strGroupName = _T("[MU]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MU(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MU_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MU(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("e"))
	{
		m_strGroupName = _T("[MP]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MP(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MP_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MP(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("f"))
	{
		m_strGroupName = _T("[MP]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MP(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MP_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MP(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("g"))
	{
		m_strGroupName = _T("[VM]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_VM(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_VM_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_VM(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("i"))
	{
		m_strGroupName = _T("[MU]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MU(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MU_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MU(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("j"))
	{
		m_strGroupName = _T("[MU]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MU(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MU_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MU(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("k"))
	{
		m_strGroupName = _T("[VM]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_VM(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_VM_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_VM(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("m"))
	{
		m_strGroupName = _T("[ER]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_ER(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_ER_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_ER(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("o"))
	{
		m_strGroupName = _T("[VM]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_VM(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_VM_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_VM(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("p"))
	{
		m_strGroupName = _T("[MX]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MX(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MX_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MX(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("r"))
	{
		m_strGroupName = _T("[VM]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_VM(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_VM_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_VM(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("s"))
	{
		m_strGroupName = _T("[CR]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_CR(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_CR_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_CR(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("t"))
	{
		m_strGroupName = _T("[BK]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_BK(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_BK_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_BK(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("y"))
	{
		m_strGroupName = _T("[RB]");
		// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
		// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_RB(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_RB_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_RB(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}	
	
	if (!m_p006Form) return -1;		
	m_p006Form->m_pListInfo = &m_listInfo;
	m_p006Form->m_strGroupName = m_strGroupName;
	if (!m_p006Form->Create(this)) return -1;

	m_p006Form->MoveWindow(rcWnd);
	m_p006Form->ShowWindow(SW_SHOW);
	m_p006Form->SetFocus();	
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_006 message handlers

BOOL CDLG_006::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_pMarc)
	{
		m_strLastErrMsg = _T("MARC Pointer�� NULL�Դϴ�.");
		goto ERR;
	}

	// Multi ComboBox Subclassing
	if (!m_MultiComboBox.SubclassDlgItem(IDC_cMAT_CATEGORY, this))
		goto ERR;

	if (Init() < 0) goto ERR;
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;
}

VOID CDLG_006::OnSelchangecMATCATEGORY() 
{
	CreateScreen();
}

VOID CDLG_006::OnOK() 
{	
	INT ids = -1;
	
	if (m_p006Form)
	{
		ids = m_p006Form->Get006();
		if (ids < 0) return;
	}
		
	CDialog::OnOK();
}

VOID CDLG_006::OnDestroy() 
{
	CDialog::OnDestroy();

	FreeData();
	
	delete m_p006Form;
	m_p006Form = NULL;
}

HBRUSH CDLG_006::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
