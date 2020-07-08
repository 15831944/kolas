// BO_Species_Detail_Viewer.cpp : implementation file
//

#include "stdafx.h"
#include "BO_Species_Detail_Viewer.h"
#include "BO_Species_Detail_Marc_Viewer.h"
#include "BO_Species_Detail_Appendix_Viewer.h"
#include "../COMMON_EBOOK_API/EBookViewMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_Species_Detail_Viewer dialog


CBO_Species_Detail_Viewer::CBO_Species_Detail_Viewer(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_Species_Detail_Viewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_Species_Detail_Viewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nOpenMode = 0;
	m_pCM = NULL;
	m_pDM_INDEX = NULL;
	m_pDM_SPECIES = NULL;
	m_pDM_BOOK = NULL;
	m_pDM_VOLUME = NULL;
	m_pSpeciesApi = NULL;
	m_pMarcDefaultValueConfigDlg = NULL;
	m_nIdx = -1;
	m_pParentDM = NULL;
	m_pParentGrid = NULL;
	m_pGrid_Volume = NULL;
	m_nFirstIdx = -1;;
	m_nLastIdx = -1;
	m_bGridSelectYN = FALSE;
}

CBO_Species_Detail_Viewer::~CBO_Species_Detail_Viewer()
{
	CEBookViewMgr mgr;
	mgr.DeleteEBookTempFolder();
	if(m_pSpeciesApi)
	{
		delete m_pSpeciesApi;
		m_pSpeciesApi = NULL;
	}
	if(m_pMarcDefaultValueConfigDlg)
	{
		delete m_pMarcDefaultValueConfigDlg;
		m_pMarcDefaultValueConfigDlg = NULL;
	}
}

VOID CBO_Species_Detail_Viewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_Species_Detail_Viewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_Species_Detail_Viewer, CDialog)
	//{{AFX_MSG_MAP(CBO_Species_Detail_Viewer)
	ON_BN_CLICKED(IDC_btnDVIEWER_PREV, OnbtnDVIEWERPREV)
	ON_BN_CLICKED(IDC_btnDVIEWER_NEXT, OnbtnDVIEWERNEXT)
	ON_BN_CLICKED(IDC_btnDVIEWER_MARC_VIEW, OnbtnDVIEWERMARCVIEW)
	ON_BN_CLICKED(IDC_btnDVIEWER_APPENDIX_VIEW, OnbtnDVIEWERAPPENDIXVIEW)
	ON_BN_CLICKED(IDC_btnEBOOK_VIEW, OnbtnEBOOKVIEW)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_Species_Detail_Viewer message handlers

BOOL CBO_Species_Detail_Viewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_����_��_�󼼺���")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_����_��_�󼼺���"));
	m_pDM_INDEX = FindDM(_T("DMUP_����_��_��������"));
	m_pDM_SPECIES = FindDM(_T("DMUP_����_��_������"));
	m_pDM_BOOK = FindDM(_T("DMUP_����_��_�󼼺���_å����"));
	m_pDM_VOLUME = FindDM(_T("DMUP_����_��_�󼼺���_������"));
	m_pGrid_Volume = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Volume"));
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("�� �󼼺��� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	Display(0);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_Species_Detail_Viewer::Init()
{
	if(m_nOpenMode == 0) return FALSE;
	if(m_pParentDM == NULL || m_pParentDM->GetRowCount() == 0) return FALSE;
	if(m_pParentGrid == NULL) return FALSE;
	if(m_strSpeciesKeyAlias.IsEmpty()) m_strSpeciesKeyAlias = _T("IBS_��KEY");
	
	m_nIdx = m_pParentGrid->GetRow() - 1;
	if(m_nIdx < 0) m_nIdx = 0;

	//��/å�� ���̺� �̸� ����
	if(m_nOpenMode > 0) 
	{
		m_pDM_SPECIES->TBL_NAME = _T("BO_DELETE_SPECIES_TBL");
		m_pDM_BOOK->TBL_NAME = _T("BO_DELETE_BOOK_TBL");
	}
	else if(m_nOpenMode < 0)
	{
		m_pDM_SPECIES->TBL_NAME = _T("BO_SPECIES_TBL");
		m_pDM_BOOK->TBL_NAME = _T("BO_BOOK_TBL");
	}
	
	//��API�ʱ�ȭ
	m_pSpeciesApi = new CBO_SPECIES(this);
	//��ũ �캻�� ���� ȭ�� �ʱ�ȭ
	m_pMarcDefaultValueConfigDlg = new CMarcDefaultValueConfigDlg(this);
	
	return TRUE;
}

VOID CBO_Species_Detail_Viewer::Display(INT idx)
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
	SetDetailInfo();
	m_marcEditor.Display();
	m_pCM->AllControlDisplay(0);
	m_pGrid_Volume->Display();
	SetWorkingStatusDesc();
	m_pParentGrid->SetReverse(m_nIdx);
	m_pGrid_Volume->SetReverse(0);
}

INT CBO_Species_Detail_Viewer::SetMarcInfoAndCtrl()
{
	//��ũ�� �׸��� ��Ʈ�� ����
	m_strStreamMarc = m_pDM_SPECIES->GetCellData(_T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc);
	m_marcEditor.SubclassDlgItem(-1, IDC_gridDETAIL_VIEWER_INFO, this);
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.InitUserAlias();
	m_marcEditor.GetGridCtrl()->SetGridColWidth(-1, -1, 420);

	m_marcEditor.AddUserAlias(_T("����"));
	m_marcEditor.AddUserAlias(_T("����"));
	m_marcEditor.AddUserAlias(_T("������"));
	m_marcEditor.AddUserAlias(_T("������"));
	m_marcEditor.AddUserAlias(_T("�����"));	
	m_marcEditor.AddUserAlias(_T("�����"));
	m_marcEditor.AddUserAlias(_T("������Ư��"));
	m_marcEditor.AddUserAlias(_T("�����ڷ�"));
	m_marcEditor.AddUserAlias(_T("ũ��"));
	m_marcEditor.AddUserAlias(_T("�Ѽ���"));
	m_marcEditor.AddUserAlias(_T("�Ѽ�����"));
	m_marcEditor.AddUserAlias(_T("����"));
	return 0;
}

VOID CBO_Species_Detail_Viewer::ShowNextPrevButton(INT idx)
{
	CButton * pBtn1 = (CButton * )GetDlgItem(IDC_btnDVIEWER_PREV);
	CButton * pBtn2 = (CButton * )GetDlgItem(IDC_btnDVIEWER_NEXT);
	pBtn1->EnableWindow(TRUE);
	pBtn2->EnableWindow(TRUE);
	
	if((m_nFirstIdx >= idx || idx < 0) && pBtn1)
		pBtn1->EnableWindow(FALSE);
	if((m_nLastIdx <= idx || idx < 0) && pBtn2)
		pBtn2->EnableWindow(FALSE);
}

INT CBO_Species_Detail_Viewer::SetDetailInfo()
{
	//����/��/å ���� ����
	CString strQuery, strSpeciesKey;
	strSpeciesKey = m_pParentDM->GetCellData(m_strSpeciesKeyAlias, m_nIdx);
	strQuery.Format(_T("REC_KEY=%s"), strSpeciesKey);
	m_pDM_INDEX->RefreshDataManager(strQuery);
	m_pDM_SPECIES->RefreshDataManager(strQuery);
	strQuery.Format(_T("SPECIES_KEY=%s ORDER BY VOL_SORT_NO"), strSpeciesKey);
	m_pDM_BOOK->RefreshDataManager(strQuery);
	m_pDM_VOLUME->FreeData();
	m_pSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_BOOK, m_pDM_VOLUME);
	SetMarcInfoAndCtrl();
	return 0;
}

VOID CBO_Species_Detail_Viewer::OnbtnDVIEWERPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID CBO_Species_Detail_Viewer::OnbtnDVIEWERNEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CBO_Species_Detail_Viewer::OnbtnDVIEWERMARCVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strMarc = m_pDM_SPECIES->GetCellData(_T("BS_MARC"), 0);
	if(strMarc.IsEmpty())
	{
		ESLAfxMessageBox(_T("��ũ ��Ʈ���� �ùٸ��� �ʽ��ϴ�."));
		return ;
	}
	CBO_Species_Detail_Marc_Viewer viewer(this);
	viewer.SetMarcStream(strMarc);
	viewer.DoModal();
}

VOID CBO_Species_Detail_Viewer::OnbtnDVIEWERAPPENDIXVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strSpeciesKey = m_pDM_SPECIES->GetCellData(_T("BS_��KEY"), 0);
	CBO_Species_Detail_Appendix_Viewer viewer(this);
	viewer.SetSpeciesKey(strSpeciesKey);
	viewer.DoModal();
}

VOID CBO_Species_Detail_Viewer::SetWorkingStatusDesc()
{
	CString strWorkStatus = m_pDM_INDEX->GetCellData(_T("IBS_�۾�����INFO"), 0);
	strWorkStatus = strWorkStatus.Left(3);
	if(strWorkStatus == _T("BOA")) strWorkStatus = _T("�����ڷ�");
	else if(strWorkStatus == _T("BOR")) strWorkStatus = _T("����ڷ�");
	else if(strWorkStatus == _T("BOC")) strWorkStatus = _T("�����ڷ�");
	else if(strWorkStatus == _T("BOL")) strWorkStatus = _T("�����ڷ�");
	else strWorkStatus = _T("�����ڷ�");

	m_pCM->SetControlMgrData(_T("IBS_�ڷ����INFO"), strWorkStatus);
}

VOID CBO_Species_Detail_Viewer::InitFirstLastIndex()
{
	INT ids = m_pParentGrid->SelectMakeList();
	ids = m_pParentGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pParentDM->GetRowCount();
		m_nFirstIdx = 0;
		m_nLastIdx = cnt-1;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_nLastIdx = m_pParentGrid->SelectGetTailPosition();
		m_nFirstIdx = m_pParentGrid->SelectGetHeadPosition();
	}
}

INT CBO_Species_Detail_Viewer::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;
	if(m_nOpenMode == 2000 || m_nOpenMode == -2000)
	{
		nFirstSelIdx = m_pParentGrid->GetRow() - 1;
	}
	else if(m_nOpenMode == 1000 || m_nOpenMode == -1000)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_nFirstIdx;
		else
			nFirstSelIdx = m_pParentGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	return nFirstSelIdx;
}

INT CBO_Species_Detail_Viewer::GetPrevSelectIndex()
{
	//===============================
	//2008.07.09 UPDATE BY PJW : �񱳸� ���ؼ� �ʱⰪ�� 0���� �����Ѵ�.
// 	INT nPrevSelIndex = -1;
 	INT nPrevSelIndex = 0;
	//===============================
	
	if(m_bGridSelectYN)
	{
		//================================================
		//2009.04.08 ADD BY PJW : INDEX������������ nPrevSelIndex�� 0���������� ������ ���� INDEX�� �ִ´�.
		while( 1 )
		{
			if(nPrevSelIndex == m_nIdx || nPrevSelIndex < 0 )
			{
				break;
			}
			else if( nPrevSelIndex < m_nIdx )
			{
				nPrevSelIndex = m_pParentGrid->SelectGetNext();
				if( nPrevSelIndex < 0 )
				{
					nPrevSelIndex = m_pParentGrid->SelectGetTailPosition();
					break;
				}
			}
			else if( nPrevSelIndex > m_nIdx )
			{
				nPrevSelIndex = m_pParentGrid->SelectGetPrev();
			}
		}
		//================================================
		nPrevSelIndex = m_pParentGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pParentGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_nIdx != m_nFirstIdx)
			nPrevSelIndex = m_nIdx - 1;
	}
	return nPrevSelIndex;
}

INT CBO_Species_Detail_Viewer::GetNextSelectIndex()
{
	//===============================
	//2008.07.09 UPDATE BY PJW : �񱳸� ���ؼ� �ʱⰪ�� 0���� �����Ѵ�.
// 	INT nNextSelIndex = -1;
 	INT nNextSelIndex = 0;
	//===============================
	
	if(m_bGridSelectYN)
	{
		//================================================
		//2008.07.08 ADD BY PJW : �������� ������ ����,���� �Ұ�� �ε����� ���� ����
		while( 1 )
		{
			if(nNextSelIndex == m_nIdx || nNextSelIndex < 0 )
			{
				break;
			}
			else if( nNextSelIndex < m_nIdx )
			{
				nNextSelIndex = m_pParentGrid->SelectGetNext();
			}
			else if( nNextSelIndex > m_nIdx )
			{
				nNextSelIndex = m_pParentGrid->SelectGetPrev();
			}
		}
		//================================================

		nNextSelIndex = m_pParentGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pParentGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_nIdx != m_nLastIdx)
			nNextSelIndex = m_nIdx + 1;
	}
	return nNextSelIndex;
}

VOID CBO_Species_Detail_Viewer::OnbtnEBOOKVIEW() 
{
	// TODO: Add your control notification handler code here
	CEBookViewMgr mgr;
	INT nRow = m_pGrid_Volume->GetRow()-1;
	CString strMediaCode = m_pDM_VOLUME->GetCellData(_T("BB_��ü����_�ڵ�"), nRow);
	if(strMediaCode != _T("EB"))
	{
		ESLAfxMessageBox(_T("�����Ͻ� å�� ����å�� �ƴմϴ�."));
		return;
	}
	CString strBookKey = m_pDM_VOLUME->GetCellData(_T("BB_åKEY"), nRow);
	mgr.EBookViewByBookKey(strBookKey, m_pDM_BOOK->CONNECTION_INFO);
}

HBRUSH CBO_Species_Detail_Viewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
