// BOAppendixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOAppendixDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOAppendixDlg dialog


CBOAppendixDlg::CBOAppendixDlg(CESL_Mgr* pParent /*= NULL*/, CESL_DataMgr *pDM_Book /*= NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOAppendixDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM_Book = pDM_Book;
	m_pCM = NULL;
	m_pDM_Appendix = NULL;
	m_pFormApi = NULL;

	m_nSelectedRow = -1;

	m_bReadOnly = FALSE;
}

BOOL CBOAppendixDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBOAppendixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOAppendixDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOAppendixDlg, CDialog)
	//{{AFX_MSG_MAP(CBOAppendixDlg)
	ON_BN_CLICKED(IDC_bINSERT_APPENDIX, OnbINSERTAPPENDIX)
	ON_BN_CLICKED(IDC_bSAVE_APPENDIX, OnbSAVEAPPENDIX)
	ON_BN_CLICKED(IDC_bDELETE_APPENDIX, OnbDELETEAPPENDIX)
	ON_BN_CLICKED(IDC_bCLEARSCREEN, OnbCLEARSCREEN)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBOAppendixDlg Operations
INT CBOAppendixDlg::Init()
{
	if (m_pDM_Book == NULL || m_pFormApi == NULL) return -1;
	
	// �η� ���� �����.
	m_pDM_Appendix = FindDM(_T("DM_����_�η�_�η�����"));
	if (m_pDM_Appendix == NULL) return -1;

	m_pCM = FindCM(_T("CM_����_�η�_�η�����"));
	if (m_pCM == NULL) return -1;

	CString strKey;
	
	// å�����϶�
	if (m_nMode == 1)
		strKey = m_pFormApi->GetBookKey(m_pDM_Book, m_nBookIdx);
	
	// �η����� �����
	INT ids = m_pFormApi->MakeAppendixDM(m_pDM_Book, m_pDM_Appendix, m_strVol, strKey);
	if (ids < 0) return -1;
	
	return 0;
}

VOID CBOAppendixDlg::SetAppendixMode(INT nMode, CString strVol, INT nBookIdx /*= -1*/) 
{
	m_nMode = nMode; 
	m_strVol = strVol;
	m_nBookIdx = nBookIdx;
}

INT CBOAppendixDlg::Display()
{
	if (m_nSelectedRow < 0) return -1;

	CESL_Control_Element * pCE = NULL;

	POSITION pos = m_pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;

		pCE->Display(m_nSelectedRow);
	}

	// ����� �����ֱ�
	m_pCM->SetControlMgrData(_T("BB_�����"), m_strVol);
	
	// �η��� ���� ��� �����ư, ������ư Disable
	if (m_pDM_Appendix->GetRowCount() < 1)
	{
		GetDlgItem(IDC_bSAVE_APPENDIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_bDELETE_APPENDIX)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_bSAVE_APPENDIX)->EnableWindow(TRUE);
		GetDlgItem(IDC_bDELETE_APPENDIX)->EnableWindow(TRUE);
	}

	if (m_bReadOnly)
		DisplayControlReadOnly();
	
	return 0;
}

INT CBOAppendixDlg::DisplayControlReadOnly()
{
	m_pCM->AllControlReadOnly();

	INT nButtonCount = 0;
	INT nArrButtonCtrl[16];
	
	nArrButtonCtrl[nButtonCount++] = IDC_bINSERT_APPENDIX;
	nArrButtonCtrl[nButtonCount++] = IDC_bSAVE_APPENDIX;
	nArrButtonCtrl[nButtonCount++] = IDC_bCLEARSCREEN;
	nArrButtonCtrl[nButtonCount++] = IDC_bDELETE_APPENDIX;
	
	CWnd *pWnd = NULL;

	for (INT i = 0; i < nButtonCount; i++)
	{
		pWnd = GetDlgItem(nArrButtonCtrl[i]);
		if (pWnd == NULL) continue;

		pWnd->EnableWindow(FALSE);
	}

	return 0;
}

INT CBOAppendixDlg::InsertAppendix(CString strParentKey)
{
	if (strParentKey.IsEmpty()) return -1;

	// ȭ������ �߰�
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Book);
	INT nRowCount = m_pDM_Book->GetRowCount();
	
	// �θ�Ű ����
	m_pDM_Book->SetCellData(_T("BB_�θ�åKEY"), strParentKey, nRowCount - 1);
	
	// �η� �÷��� �߰�
	m_pDM_Book->SetCellData(_T("BB_å�η��÷���"), _T("A"), nRowCount - 1);

	// ����� �ֱ�
	m_pDM_Book->SetCellData(_T("BB_�����"), m_strVol, nRowCount - 1);
	
	// ���� �÷��� ����
	CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_INSERT);
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, nRowCount - 1);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBOAppendixDlg message handlers

BOOL CBOAppendixDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = InitESL_Mgr(_T("SM_����_�η�"));
	if (ids < 0) 
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	ids = Init();
	if (ids < 0)
	{
		AfxMessageBox(_T("Init Data Manger Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_nSelectedRow = 0;
	AllControlDisplay(_T("CM_����_�η�_�η�����"), 0);

	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gBO_APPENDIX);
	if (pGrid != NULL)
		pGrid->SetReverse(m_nSelectedRow);
	
	Display();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOAppendixDlg::OnbINSERTAPPENDIX() 
{
	INT ids = -1;

	// �źθ���Ʈ ����� (å���� �������ϰ�..)
	m_pFormApi->InitDefaultIgnoreAlias();
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��ǥå����"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��Ϲ�ȣ"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_������ȣ"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));

	// ������ å���� Ȯ��  ���̸� å ���� ��ŭ �η� �߰��Ѵ�.
	// �Ǵ���
	if (m_nMode == 0)
	{
		CArray<CString, CString&> arrReckey;
		INT nRowCount = m_pDM_Book->GetRowCount();
		for (INT idx = 0; idx < nRowCount; idx++)
		{
			// å�� �ƴϸ� �Ѿ��.
			if (!m_pFormApi->IsBook(m_pDM_Book, idx, m_strVol)) continue;
			
			// �θ��� rec_key�� �ִ��� üũ���� Ű �������� ����
			CString strKey = m_pDM_Book->GetCellData(_T("BB_åKEY"), idx);
			if (strKey.IsEmpty())
			{
				strKey = m_pDM_Book->GetCellData(_T("BB_UNIQUE_KEY"), idx);
				if (strKey.IsEmpty())
				{
					// Ű ����
					strKey.Format(_T("U%d"), GetTickCount());
					m_pDM_Book->SetCellData(_T("BB_UNIQUE_KEY"), strKey, idx);
				}
			}

			arrReckey.Add(strKey);

			Sleep(20);
		}

		// �η��� �߰��Ѵ�.
		nRowCount = arrReckey.GetSize();
		for (idx = 0; idx < nRowCount; idx++)
		{
			InsertAppendix(arrReckey.GetAt(idx));
		}

		// �η����� DM�ٽ� �����
		m_pFormApi->InitDefaultIgnoreAlias();
		ids = m_pFormApi->MakeAppendixDM(m_pDM_Book, m_pDM_Appendix, m_strVol);
		if (ids < 0) return;
	}
	// å����
	else if (m_nMode == 1)
	{
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_�谡��ġ��ȣ"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_��ġ��ȣ_�ڵ�"));
		// �θ��� rec_key�� �ִ��� üũ���� Ű �������� ����
		CString strKey = m_pFormApi->GetBookKey(m_pDM_Book, m_nBookIdx);
		if (strKey.IsEmpty())
		{
			// Ű ����
			strKey.Format(_T("U%d"), GetTickCount());
			m_pDM_Book->SetCellData(_T("BB_UNIQUE_KEY"), strKey, m_nBookIdx);
		}

		InsertAppendix(strKey);

		// �η����� DM�ٽ� �����
		m_pFormApi->InitDefaultIgnoreAlias();
		ids = m_pFormApi->MakeAppendixDM(m_pDM_Book, m_pDM_Appendix, m_strVol, strKey);
		if (ids < 0) return;
	}
	
	// ������ ����ȭ�鿡 �Ѹ���
	m_nSelectedRow = m_pDM_Appendix->GetRowCount() - 1;
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gBO_APPENDIX);
	if (pGrid != NULL)
	{
		pGrid->Display();
		pGrid->SetReverse(m_nSelectedRow);
	}
	
	Display();
}

VOID CBOAppendixDlg::OnbSAVEAPPENDIX() 
{
	if (m_nSelectedRow < 0) return;

	m_pFormApi->InitDefaultIgnoreAlias();

	// Appdedix row �� ����Ű�� �ִ� Book Row ã��
	INT idx = _ttoi(m_pDM_Appendix->GetCellData(_T("BB_BOOK_IDX"), m_nSelectedRow));
	if (idx < 0) return;

	INT ids = -1;

	// ȭ������ ����
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Book, idx);

	// ���� �÷��� ����
	CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

	CString strKey;
	// å����
	if (m_nMode == 1)
	{
		strKey = m_pFormApi->GetBookKey(m_pDM_Book, m_nBookIdx);
	}

	// �η����� DM �ٽ� �����
	ids = m_pFormApi->MakeAppendixDM(m_pDM_Book, m_pDM_Appendix, m_strVol, strKey);
	if (ids < 0) return;

	// ���� ���õ� ���� ȭ�鿡 �ٽ� �Ѹ���
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gBO_APPENDIX);
	if (pGrid != NULL)
	{
		pGrid->Display();
		pGrid->SetReverse(m_nSelectedRow);
	}
}

VOID CBOAppendixDlg::OnbDELETEAPPENDIX() 
{
	if (m_nSelectedRow < 0) return;

	// Appdedix row �� ����Ű�� �ִ� Book Row ã��
	INT idx = _ttoi(m_pDM_Appendix->GetCellData(_T("BB_BOOK_IDX"), m_nSelectedRow));
	if (idx < 0) return;

	CString strVolTitle;
	m_pCM->GetControlMgrData(_T("BB_������ηϸ�"), strVolTitle);
		
	CString strMsg;
	strMsg.Format(_T("�ηϸ��� [%s]�� �η��� �����Ͻðڽ��ϱ�?"), strVolTitle);
	if (MessageBox(strMsg, NULL, MB_ICONQUESTION | MB_YESNO) == IDNO) return;

	// ����
	m_pFormApi->DeleteBookDMRow(m_pDM_Book, idx);

	CString strKey;
	// å����
	if (m_nMode == 1)
	{
		strKey = m_pFormApi->GetBookKey(m_pDM_Book, m_nBookIdx);
	}

	// �η����� DM �ٽ� �����
	INT ids = m_pFormApi->MakeAppendixDM(m_pDM_Book, m_pDM_Appendix, m_strVol, strKey);
	if (ids < 0) return;

	if (m_nSelectedRow > 0)
		m_nSelectedRow--;

	// ���� ���õ� ���� ȭ�鿡 �ٽ� �Ѹ���
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gBO_APPENDIX);
	if (pGrid != NULL)
	{
		pGrid->Display();
		pGrid->SetReverse(m_nSelectedRow);
	}

	Display();
}

BEGIN_EVENTSINK_MAP(CBOAppendixDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBOAppendixDlg)
	ON_EVENT(CBOAppendixDlg, IDC_gBO_APPENDIX, -605 /* MouseDown */, OnMouseDowngBOAPPENDIX, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBOAppendixDlg::OnMouseDowngBOAPPENDIX(short Button, short Shift, long x, long y) 
{
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gBO_APPENDIX);
	if (pGrid == NULL) return;

	INT nRow = 	pGrid->GetRow();
	if (nRow == 0) return;

	pGrid->SetReverse(nRow - 1);

	m_nSelectedRow = nRow - 1;

	Display();
}

VOID CBOAppendixDlg::OnbCLEARSCREEN() 
{
	CESL_Control_Element * pCE = NULL;

	POSITION pos = m_pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;

		pCE->Clear();
	}

	return;	
}