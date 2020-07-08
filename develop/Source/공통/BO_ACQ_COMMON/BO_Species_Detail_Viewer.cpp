// BO_Species_Detail_Viewer.cpp : implementation file
//

#include "stdafx.h"
#include "BO_Species_Detail_Viewer.h"

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
}

CBO_Species_Detail_Viewer::~CBO_Species_Detail_Viewer()
{
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
	//åDM->��DM����
	m_pSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_BOOK, m_pDM_VOLUME);
	//��ũ �캻�� ���� ȭ�� �ʱ�ȭ
	m_pMarcDefaultValueConfigDlg = new CMarcDefaultValueConfigDlg(this);
	
	return TRUE;
}

VOID CBO_Species_Detail_Viewer::Display(INT idx)
{
	switch(idx)
	{
	case 0:
		ShowNextPrevButton(m_nIdx);
		break;
	case -1:
		m_nIdx--;
		ShowNextPrevButton(m_nIdx);
		break;
	case 1:
		if(m_pParentDM->GetRowCount()-1 > m_nIdx) m_nIdx++;
		ShowNextPrevButton(m_nIdx);
		break;
	default:
		return ;
	}
	SetDetailInfo();
	m_marcEditor.ApplyEdit();
	m_pCM->AllControlDisplay();
	m_pGrid_Volume->Display();
}

INT CBO_Species_Detail_Viewer::SetMarcInfoAndCtrl()
{
	//��ũ�� �׸��� ��Ʈ�� ����
	m_strStreamMarc = m_pDM_SPECIES->GetCellData(_T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc);
	m_marcEditor.SubclassDlgItem(-1, IDC_gridDETAIL_INFO, this);
	m_marcEditor.InitUserAlias();
	
	CMapStringToPtr MapUserAlias;
	m_pMarcDefaultValueConfigDlg->LoadDefaultUserMarcAlias(&MapUserAlias);
	
	CStringArray *pStringArray = NULL;
	INT ids = m_pMarcDefaultValueConfigDlg->GetUserMarcAlias(&MapUserAlias, m_pInfo->pMarcMgr->GetMarcGroup(), &pStringArray);
	if (ids >= 0)
	{
		INT nCount = pStringArray->GetSize();
		for (INT idx = 0; idx < nCount; idx++)
		{
			m_marcEditor.AddUserAlias(pStringArray->GetAt(idx));
		}
	}
	return 0;
}

VOID CBO_Species_Detail_Viewer::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnDVIEWER_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDVIEWER_NEXT)->EnableWindow(TRUE);

	if(idx == 0 || idx < 0)
		GetDlgItem(IDC_btnDVIEWER_PREV)->EnableWindow(FALSE);
	if(m_pParentDM->GetRowCount()-1 == idx || idx < 0)
		GetDlgItem(IDC_btnDVIEWER_NEXT)->EnableWindow(FALSE);
}

INT CBO_Species_Detail_Viewer::SetDetailInfo()
{
	//����/��/å ���� ����
	CString strQuery, strSpeciesKey;
	strSpeciesKey = m_pParentDM->GetCellData(m_strSpeciesKeyAlias, m_nIdx);
	strQuery.Format(_T("REC_KEY=%s"), strSpeciesKey);
	m_pDM_INDEX->RefreshDataManager(strQuery);
	m_pDM_SPECIES->RefreshDataManager(strQuery);
	strQuery.Format(_T("SPECIES_KEY=%s"), strSpeciesKey);
	m_pDM_BOOK->RefreshDataManager(strQuery);
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
