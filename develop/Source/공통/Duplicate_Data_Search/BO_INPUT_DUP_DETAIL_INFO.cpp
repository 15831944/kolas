// BO_INPUT_DUP_DETAIL_INFO.cpp : implementation file
//

#include "stdafx.h"
#include "BO_INPUT_DUP_DETAIL_INFO.h"
#include "../BO_VOLUME_FORM/BOVolumeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_INPUT_DUP_DETAIL_INFO dialog


CBO_INPUT_DUP_DETAIL_INFO::CBO_INPUT_DUP_DETAIL_INFO(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_INPUT_DUP_DETAIL_INFO::IDD, pParent)
{
	AfxInitRichEdit();	
	//{{AFX_DATA_INIT(CBO_INPUT_DUP_DETAIL_INFO)
	m_DupFormCode = _T("");
	m_DupAllData = _T("");
	m_DupSelData = _T("");
	m_DupManageNo = _T("");
	m_DupMediaCode = _T("");
	//}}AFX_DATA_INIT
	m_pGrid = NULL;
	m_pDM = NULL;
	m_nIdx = -1;
	m_nFirstidx = -1;
	m_nLastidx = -1;
	m_bGridSelectYN = FALSE;
	m_nOpenMode = OPERATION_TYPE_INVALID;

	// KOL.UDF.022 ��ť���ڵ� ����
	m_DuplicateIdx = -1;
}

CBO_INPUT_DUP_DETAIL_INFO::~CBO_INPUT_DUP_DETAIL_INFO()
{
}



VOID CBO_INPUT_DUP_DETAIL_INFO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_INPUT_DUP_DETAIL_INFO)
	DDX_Text(pDX, IDC_edtFORM_CLASS_SEL, m_DupFormCode);
	DDX_Text(pDX, IDC_edtGUN, m_DupAllData);
	DDX_Text(pDX, IDC_edtJAE, m_DupSelData);
	DDX_Text(pDX, IDC_edtMANAGEMENT_NO_SEL, m_dupStatus);
	DDX_Text(pDX, IDC_edtMED_CLASS_SEL, m_DupMediaCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_INPUT_DUP_DETAIL_INFO, CDialog)
	//{{AFX_MSG_MAP(CBO_INPUT_DUP_DETAIL_INFO)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnPREV_SRC, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT_SRC, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnSHOW_INPUTDUP_VOL, OnbtnSHOWINPUTDUPVOL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_INPUT_DUP_DETAIL_INFO message handlers

VOID CBO_INPUT_DUP_DETAIL_INFO::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CBO_INPUT_DUP_DETAIL_INFO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_marceditor_dup.SubclassDlgItem(IDC_richINPUT_DUPLICATE, -1, this);
	//m_pInfo->pMarcMgr->SetMarcGroup(_T("����"));
	
	if(m_pGrid == NULL || m_pDM == NULL)
	{
		ESLAfxMessageBox(_T("��ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	//this->Display(m_DuplicateIdx);	
	Display(0);

	EnableThemeDialogTexture(GetSafeHwnd());
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_INPUT_DUP_DETAIL_INFO::OnbtnPREV() 
{
	// TODO: Add your control notification handler code here
/*	CDuplicateSearchMgr Mgr(this);
	Mgr.InitTempData();
	Mgr.SetTempData(_T("CMAlias"), _T("CM_����������"));
	Mgr.SetTempData(_T("DMAlias"), _T("DM_�ߺ�������"));
	Mgr.SetTempData(_T("GridAlias"), _T("MainGrid2"));
	Mgr.SetTempData(_T("��������"), _T("PrevOneSelect"));
	Mgr.SetTempData(_T("Index"), m_DuplicateIdx-1);
	INT ids = Mgr.SPFExecutePath(_T("�׸��弱��ó��"));
	if(ids < 0)
	{
		//����ó��
		CString errstr;
		switch(ids)
		{
		case -1003:
			errstr = _T("ó���ڷ��Դϴ�.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}	
	ids = Mgr.GetPrevDuplicateDataIndex(_T("CM_����������"), _T("MainGrid2"), -1, -1);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("ó�� �ڷ��Դϴ�"));
		return;
	}
	m_DuplicateIdx = ids ;
	Display(m_DuplicateIdx);		
*/
	Display(-1);
}

VOID CBO_INPUT_DUP_DETAIL_INFO::OnbtnNEXT() 
{
	// TODO: Add your control notification handler code here
/*	CDuplicateSearchMgr Mgr(this);
	Mgr.InitTempData();
	Mgr.SetTempData(_T("CMAlias"), _T("CM_����������"));
	Mgr.SetTempData(_T("DMAlias"), _T("DM_�ߺ�������"));
	Mgr.SetTempData(_T("GridAlias"), _T("MainGrid2"));
	Mgr.SetTempData(_T("��������"), _T("NextOneSelect"));
	Mgr.SetTempData(_T("Index"), m_DuplicateIdx+1);
	INT ids = Mgr.SPFExecutePath(_T("�׸��弱��ó��"));
	if(ids < 0)
	{
		//����ó��
		CString errstr;
		switch(ids)
		{
		case -1002:
			errstr = _T("�������ڷ��Դϴ�.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	ids = Mgr.GetNextDuplicateDataIndex(_T("CM_����������"), _T("MainGrid2"), -1, -1);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷ��Դϴ�"));
		return;
	}
	m_DuplicateIdx = ids ;
	Display(m_DuplicateIdx);		
*/
	Display(1);
}

VOID CBO_INPUT_DUP_DETAIL_INFO::SetDuplicateDataIdx(INT idx)
{
	this->m_DuplicateIdx = idx;
}

VOID CBO_INPUT_DUP_DETAIL_INFO::Display(INT idx)
{
	switch(idx)
	{
	case 0:
		InitFirstLastIndex();
		m_nIdx = GetFirstSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	case -1:
		m_nIdx = GetPrevSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	case 1:
		m_nIdx = GetNextSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	default:
		return ;
	}
	//��ġ����ڷ� ���������� ��� ������ġ����ڷᰡ ���̱� ������ �ش� �ڷ�� �󼼺��⸦ �� �� ����.
	CString strTmpData = m_pDM->GetCellData(_T("IBS_�ڷ����"), m_nIdx);
	if(strTmpData == _T("��ġ����ڷ�"))
	{
		ESLAfxMessageBox(_T("��ġ����ڷ�� �󼼺��� ����� ������ �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return ;
	}
	
	CString dupmarc, strQuery;
	INT ids = m_pDM->GetCellData(_T("IBS_��KEY"), m_nIdx, m_DupManageNo);
	ids = m_pDM->GetCellData(_T("IBS_���ı���"), m_nIdx, m_DupFormCode);
	ids = m_pDM->GetCellData(_T("IBS_��ü����"), m_nIdx, m_DupMediaCode);
	ids = m_pDM->GetCellData(_T("�����ڷ�"), m_nIdx, m_DupAllData);
	ids = m_pDM->GetCellData(_T("�Ϸù�ȣ"), m_nIdx, m_DupSelData);
	strQuery.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), m_DupManageNo);
	m_pDM->GetOneValueQuery(strQuery, dupmarc);
	
	m_pDM->SetCellData(_T("BS_MARC"), dupmarc, m_nIdx);
	ids = m_pDM->GetCellData(_T("UDF_�ڷ���¼���"), m_nIdx, m_dupStatus);
	
	//decoding
	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(dupmarc, &m_marc_dup))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	m_marceditor_dup.InitUserAlias();
	m_marceditor_dup.Init(m_pInfo->pMarcMgr, &m_marc_dup);
	m_marceditor_dup.Display();
		
	this->UpdateData(FALSE);
	m_pGrid->SetReverse(m_nIdx);
}
VOID CBO_INPUT_DUP_DETAIL_INFO::OnbtnSHOWINPUTDUPVOL() 
{
	// TODO: Add your control notification handler code here
	CESL_DataMgr * pDM = this->FindDM(_T("DM_�ߺ�������"));
	if(!pDM) return;
	CBOVolumeDlg dlg(this);
	dlg.SetSpeciesKey(m_DupManageNo);
	CString strTmpKey = pDM->GetCellData(_T("BP_��������KEY"), this->m_DuplicateIdx);
	if(!strTmpKey.IsEmpty() && strTmpKey != _T("-1"))
		dlg.SetPurchaseKey(strTmpKey);
	dlg.SetOpenMode(8000);
	dlg.DoModal();
}

VOID CBO_INPUT_DUP_DETAIL_INFO::InitFirstLastIndex()
{
	INT ids = m_pGrid->SelectMakeList();
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pDM->GetRowCount();
		m_nFirstidx = 0;
		m_nLastidx = cnt-1;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_nLastidx = m_pGrid->SelectGetTailPosition();
		m_nFirstidx = m_pGrid->SelectGetHeadPosition();
	}
}

VOID CBO_INPUT_DUP_DETAIL_INFO::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnPREV_SRC)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT_SRC)->EnableWindow(TRUE);

	if(m_nFirstidx >= idx || idx < 0)
		GetDlgItem(IDC_btnPREV_SRC)->EnableWindow(FALSE);
	if(m_nLastidx <= idx || idx < 0)
		GetDlgItem(IDC_btnNEXT_SRC)->EnableWindow(FALSE);
}

INT CBO_INPUT_DUP_DETAIL_INFO::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;
	if(m_nOpenMode == GUPDATE)
	{
		nFirstSelIdx = m_pGrid->GetRow() - 1;
	}
	else if(m_nOpenMode == SUPDATE)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_nFirstidx;
		else
			nFirstSelIdx = m_pGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	return nFirstSelIdx;
}

INT CBO_INPUT_DUP_DETAIL_INFO::GetPrevSelectIndex()
{
	INT nPrevSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nPrevSelIndex = m_pGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_nIdx != m_nFirstidx)
			nPrevSelIndex = m_nIdx - 1;
	}
	return nPrevSelIndex;
}

INT CBO_INPUT_DUP_DETAIL_INFO::GetNextSelectIndex()
{
	INT nNextSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nNextSelIndex = m_pGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_nIdx != m_nLastidx)
			nNextSelIndex = m_nIdx + 1;
	}
	return nNextSelIndex;
}

HBRUSH CBO_INPUT_DUP_DETAIL_INFO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

