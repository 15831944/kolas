// CatCharSetManagementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CatCharSetManagementDlg.h"
#include "CharSetInputModifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatCharSetManagementDlg dialog


CCatCharSetManagementDlg::CCatCharSetManagementDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatCharSetManagementDlg)
	m_strKeywordFrom = _T("");
	m_strKeywordTo = _T("");
	//}}AFX_DATA_INIT

	m_pDM_Manage = NULL;
	m_pDM_Data = NULL;
	m_pGrid = NULL;

	m_bChanged = FALSE;
}

CCatCharSetManagementDlg::~CCatCharSetManagementDlg()
{
	
}

VOID CCatCharSetManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatCharSetManagementDlg)
	DDX_Text(pDX, IDC_eKEYWORD_FROM_CM, m_strKeywordFrom);
	DDX_Text(pDX, IDC_eKEYWORD_TO_CM, m_strKeywordTo);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CCatCharSetManagementDlg, CDialog)
	//{{AFX_MSG_MAP(CCatCharSetManagementDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bSEARCH_CM, OnbSEARCHCM)
	ON_BN_CLICKED(IDC_bMODIFY_CM, OnbMODIFYCM)
	ON_BN_CLICKED(IDC_bDELETE_CM, OnbDELETECM)
	ON_BN_CLICKED(IDC_bINSERT_CM, OnbINSERTCM)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatCharSetManagementDlg Operations
INT CCatCharSetManagementDlg::Init()
{
	EFS_BEGIN

	if (m_strCatagory.IsEmpty()) 
	{
		m_strLastErrMsg = _T("���� �з��� �������� �ʾҽ��ϴ�.");
		return -1;
	}

	INT ids = -1;

	SetWindowText(m_strCatagory);
	GetDlgItem(IDC_STATIC_TITLE)->SetWindowText(m_strCatagory);

	// ���� ������ �ε��� �´�.
	CString strQuery;
	strQuery.Format(_T("CHARSET_NAME = '%s' AND ROWNUM = 1"), m_strCatagory);
	ids = m_pDM_Manage->RefreshDataManager(strQuery);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("���� ������ ã�� �� �����ϴ�.");
		return -1;
	}

	if (m_pDM_Manage->GetRowCount() != 1)
	{
		m_strLastErrMsg = _T("���� ������ ã�� �� �����ϴ�.");
		return -1;
	}

	// ���� ������ ����ϴ� �����͸� �����´�.

	CString strTblName;
	m_pDM_Manage->GetCellData(_T("TABLE_NAME"), 0, strTblName);
	if (strTblName.IsEmpty())
	{
		m_strLastErrMsg = _T("���� ������ ã�� �� �����ϴ�.");
		return -1;
	}

	m_pDM_Data->TBL_NAME = strTblName;

	return 0;

	EFS_END
	return -1;
}

INT CCatCharSetManagementDlg::ApplyCharSetData()
{
	EFS_BEGIN

	BeginWaitCursor();

	INT ids = -1;

	// 040228_HSS �߰� ��ü ��˻�
	ids = m_pDM_Data->RefreshDataManager(_T(""));
	if (ids<0) return -2;

	// ���� DM Row
	INT nManagementDMRow = 0;
	
	// ���� DM REC_KEY
	CString strRecKey = m_pDM_Manage->GetCellData(_T("REC_KEY"), nManagementDMRow);
	if (strRecKey.IsEmpty()) return -1;
	
	// ���Ͽ� ���� �� �����͸� ����.
	CString strTempFileName = _T("~charsetmgr.tmp");
	CString strVersion = m_pDM_Manage->GetCellData(_T("VERSION"), nManagementDMRow);
	INT nVersion = _ttoi(strVersion) + 1;
	strVersion.Format(_T("%d"), nVersion);

	ids = MakeDataFile(strTempFileName, nVersion);
	if (ids < 0) return -1;

	// ���� ���̺� ����, ���ϱ���, ���������� ������Ʈ �Ѵ�.

	// ������ ��ä�� BLOB���� DB�� �ø���.
	CStdioFile file;
	INT nFileSize = 0;
	if (!file.Open(strTempFileName, CFile::modeRead | CFile::typeBinary))
	{
		MessageBox(_T("������ ���µ� �����߽��ϴ�."));
		return -1;
	}
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file.SeekToBegin();
			}
	
	nFileSize = file.GetLength();
	BYTE *pBuffer = (BYTE*)malloc(sizeof(BYTE) * (nFileSize) + 1024);
	ZeroMemory(pBuffer, nFileSize);
	nFileSize = file.ReadHuge(pBuffer, nFileSize);
	file.Close();

	// ����
	ids = m_pDM_Manage->LOBUpdate(m_pDM_Manage->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, nFileSize);
	if (ids < 0)
	{
		// �޸� ����
		free(pBuffer);
		MessageBox(_T("�������� ����"));
		return -1;
	}
	
	// ���ϱ���, ��������
	CString strLen;
	strLen.Format(_T("%d"), nFileSize);

	ids = m_pDM_Manage->StartFrame();
	
	m_pDM_Manage->InitDBFieldData();
	m_pDM_Manage->AddDBFieldData(_T("VERSION"), _T("NUMERIC"), strVersion);
	m_pDM_Manage->AddDBFieldData(_T("FILE_LEN"), _T("NUMERIC"), strLen);
	m_pDM_Manage->MakeUpdateFrame(m_pDM_Manage->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey, nManagementDMRow);
	
	ids = m_pDM_Manage->SendFrame();
	if (ids < 0) 
	{
		// �޸� ����
		free(pBuffer);
		MessageBox(_T("�������� �����߽��ϴ�."));
		return -1;
	}
	ids = m_pDM_Manage->EndFrame();
	if (ids < 0) 
	{
		// �޸� ����
		free(pBuffer);
		MessageBox(_T("�������� �����߽��ϴ�."));
		return -1;
	}

	
	// �޸� ����
	free(pBuffer);

	// ������ �����Ѵ�.
	CString strFileName = _T("..\\cfg\\") + m_pDM_Manage->GetCellData(_T("FILE_NAME"), nManagementDMRow);

	CopyFile(strTempFileName, strFileName, FALSE);
	
	
	// ���� ����
	DeleteFile(strTempFileName);

	EndWaitCursor();

	return 0;

	return -1;


	EFS_END
	return -1;
}

INT CCatCharSetManagementDlg::MakeDataFile(CString strFilePath, INT nVersion)
{
	EFS_BEGIN

	if (strFilePath.IsEmpty() || nVersion < 0) return -1;

	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		MessageBox(_T("������ �����ϴµ� �����߽��ϴ�."));
		return -1;
	}
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	// ����
	CString strVersion;
	strVersion.Format(_T(".FILE_VERSION = %d\r\n"), nVersion);
	file.WriteString(strVersion);

	// ������
	//m_pDM_Data->RefreshDataManager(_T(""));
	INT nRowCount = m_pDM_Data->GetRowCount();

	INT nColFromData = m_pDM_Data->FindColumn(_T("FROM_DATA"));
	INT nColToData = m_pDM_Data->FindColumn(_T("TO_DATA"));

	CString strLine;
	CString strFromData;
	CString strToData;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strFromData = m_pDM_Data->GetCellData(idx, nColFromData);
		strToData = _T("Y");
		if (strFromData.IsEmpty() || strToData.IsEmpty()) continue;

		strLine.Format(_T("%-100s|%-100s\r\n"), strFromData, strToData);

		// ���Ϸ� ����.
		file.WriteString(strLine);
	}

	file.Close();

	return 0;

	return -1;


	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CCatCharSetManagementDlg message handlers

BOOL CCatCharSetManagementDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_����_��������")) < 0)
	{
		m_strLastErrMsg = _T("Init ESL Mgr Error");
		goto ERR;
	}

	m_pDM_Manage = FindDM(_T("DM_����_���ں�ȯ_��������"));
	if (m_pDM_Manage == NULL) goto ERR;

	m_pDM_Data = FindDM(_T("DM_����_���ں�ȯ"));
	if (m_pDM_Data == NULL) goto ERR;

	m_pGrid = (CESL_Grid*)FindControl(_T("CM_����_��������"), _T("GRID_DATA"));
	if (m_pGrid == NULL) goto ERR;

	if (Init() < 0) goto ERR;

	PostMessage(WM_SIZE);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);

	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CCatCharSetManagementDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (!m_pGrid || !m_pGrid->GetSafeHwnd()) return;

	CRect rcWnd;
	GetClientRect(rcWnd);

	CRect rcGrid;
	m_pGrid->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	rcWnd.DeflateRect(5, 5);
	rcGrid.bottom = rcWnd.bottom;
	rcGrid.left = rcWnd.left;
	rcGrid.right = rcWnd.right;

	m_pGrid->MoveWindow(rcGrid);

	EFS_END
}

BOOL CCatCharSetManagementDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnbSEARCHCM();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;
}

VOID CCatCharSetManagementDlg::OnbSEARCHCM() 
{
	EFS_BEGIN

	UpdateData();

	CString strQuery;
	
	if (m_strKeywordFrom.IsEmpty() && m_strKeywordTo.IsEmpty())
		;
	else if (!m_strKeywordFrom.IsEmpty() && m_strKeywordTo.IsEmpty())
		strQuery.Format(_T("FROM_DATA LIKE('%s%%')"), m_strKeywordFrom);
	else if (!m_strKeywordFrom.IsEmpty() && !m_strKeywordTo.IsEmpty())
		strQuery.Format(_T("FROM_DATA >= '%s' AND FROM_DATA <= '%s'"), m_strKeywordFrom, m_strKeywordTo);	
	
	m_pDM_Data->RefreshDataManager(strQuery);
	m_pGrid->Display();

	EFS_END
}

VOID CCatCharSetManagementDlg::OnbINSERTCM() 
{
	EFS_BEGIN

	CCharSetInputModifyDlg dlg(this);
	dlg.SetMode(MODE_INSERT_EXT);
	dlg.m_strCatagory = m_strCatagory;
	if (dlg.DoModal() != IDOK) return;

	m_pDM_Data->StartFrame();

	CString strRecKey;
	m_pDM_Data->MakeRecKey(strRecKey);
	if (strRecKey.IsEmpty()) return;

	m_pDM_Data->InitDBFieldData();
	m_pDM_Data->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strRecKey);
	m_pDM_Data->AddDBFieldData(_T("FROM_DATA"), _T("STRING"), dlg.m_strFromData);
	m_pDM_Data->AddDBFieldData(_T("TO_DATA"), _T("STRING"), _T("Y"));
	m_pDM_Data->MakeInsertFrame(m_pDM_Data->TBL_NAME);

	if (m_pDM_Data->SendFrame() < 0)
	{
		MessageBox(_T("�Է¿� �����߽��ϴ�."));
		return;
	}
	m_pDM_Data->EndFrame();

	// �����͸� �ִ´�.
	m_pDM_Data->InsertRow(-1);
	INT nRow = m_pDM_Data->GetRowCount() -1;
	m_pDM_Data->SetCellData(_T("REC_KEY"), strRecKey, nRow);
	m_pDM_Data->SetCellData(_T("FROM_DATA"), dlg.m_strFromData, nRow);
	m_pGrid->DisplayLine(nRow);
	
	MessageBox(_T("�Է��߽��ϴ�."));

	m_bChanged = TRUE;

	EFS_END
}

VOID CCatCharSetManagementDlg::OnbMODIFYCM() 
{
	EFS_BEGIN

	INT ids = -1;
	ids = m_pGrid->SelectMakeList();
	if (ids < 0) 
	{
		MessageBox(_T("���ø�� ������ �����߽��ϴ�."));
		return;
	}
	ids = m_pGrid->SelectGetCount();
	if (ids <= 0)
	{
		MessageBox(_T("������ �׸��� �����ϴ�."));
		return;
	}

	// 040915_HSS - Frame�� �Է¼���ȭ�鿡�� ���� ������!
	//m_pDM_Data->StartFrame();

	CCharSetInputModifyDlg dlg(this);
	dlg.SetMode(MODE_MODIFY_EXT);
	dlg.SetParentGrid(m_pGrid);
	dlg.m_strCatagory = m_strCatagory;
	dlg.DoModal();

	/*
	if (m_pDM_Data->SendFrame() < 0)
	{
		MessageBox(_T("������ �����߽��ϴ�."));
		return;
	}
	m_pDM_Data->EndFrame();
	*/

	m_bChanged = TRUE;

	EFS_END
}

VOID CCatCharSetManagementDlg::OnbDELETECM() 
{
	EFS_BEGIN

	INT ids = -1;
	ids = m_pGrid->SelectMakeList();
	if (ids < 0) 
	{
		MessageBox(_T("���ø�� ������ �����߽��ϴ�."));
		return;
	}

	INT nCount = m_pGrid->SelectGetCount();

	CString strMsg;
	strMsg.Format(_T("������ [%d]���� �ڷḦ �����Ͻðڽ��ϱ�"), nCount);
	if (MessageBox(strMsg, _T("����Ȯ��"), MB_YESNO | MB_ICONQUESTION) == IDNO) return;

	m_pGrid->SelectGetPrev();

	INT idx = -1;
	
	ids = m_pDM_Data->StartFrame();

	CString strRecKey;
	idx = m_pGrid->SelectGetTailPosition();
	while (idx >= 0)
	{
		strRecKey = m_pDM_Data->GetCellData(_T("REC_KEY"), idx);
		if (!strRecKey.IsEmpty())
		{
			m_pDM_Data->MakeDeleteFrame(m_pDM_Data->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
		}
		
		idx = m_pGrid->SelectGetPrev();
	}

	ids = m_pDM_Data->SendFrame();
	if (ids < 0) 
	{
		MessageBox(_T("������ �����߽��ϴ�."));
	}
	ids = m_pDM_Data->EndFrame();
	if (ids < 0) 
	{
		MessageBox(_T("������ �����߽��ϴ�."));
	}
	

	MessageBox(_T("�����߽��ϴ�."));

	OnbSEARCHCM();

	m_bChanged = TRUE;

	EFS_END
}

VOID CCatCharSetManagementDlg::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();
	
	// ���� ����
	if (m_bChanged)
		ApplyCharSetData();

	EFS_END
}

HBRUSH CCatCharSetManagementDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
