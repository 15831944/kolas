// BO_DeleteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_DeleteDlg.h"
#include "../BO_ACQ_GROUP_MANAGER/Group_Select_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_DeleteDlg dialog


CBO_DeleteDlg::CBO_DeleteDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_DeleteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_DeleteDlg)
	//}}AFX_DATA_INIT
	m_pMainDM = NULL;
	m_pMainGrid = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_nSpeciesType = SPECIES_TYPE_INVALID;
	m_nAcqType = ACQ_TYPE_INVALID;
	m_pArrSpeciesKey = NULL;
	m_pArrAcqKey = NULL;
	m_pArrVolSortNo = NULL;	
	m_nDeleteOption = 0;
	m_nDeleteType = SPECIES_DELETE;
	m_pDeleteApi = NULL;
	m_bDeleteFlag = FALSE;

	m_pMainBookDM = NULL;
	m_strBookCntAlias = _T("");
	m_strPriceAlias = _T("");	
}

CBO_DeleteDlg::~CBO_DeleteDlg()
{
	if(m_arrParentIdx.GetSize() > 0)
	{
		CString strDelOption;
		if(m_nDeleteOption < 0) 
		{
			strDelOption = _T("�ߺ��ڷ����");
			m_pDeleteApi->DeleteSelectRow(m_pMainDM, &m_arrParentIdx, strDelOption);
		}
		else 
			m_pDeleteApi->DeleteSelectRowAndBookDM(m_pMainDM, m_pMainBookDM, &m_arrParentIdx, m_nDeleteType);
	}
	if(m_pDeleteApi)
	{
		delete m_pDeleteApi;
		m_pDeleteApi= NULL;
	}
}

VOID CBO_DeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_DeleteDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_DeleteDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_DeleteDlg)
	ON_BN_CLICKED(IDC_btnDB_DELETE, OnbtnDBDELETE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_DeleteDlg message handlers
BOOL CBO_DeleteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_�����ڷ�Ȯ��")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_�����ڷ�Ȯ��"));
	if(m_nDeleteType == SPECIES_DELETE)
	{
		m_pDM = FindDM(_T("DM_�����ڷ�Ȯ��_��"));
		if(m_nDeleteOption > 0)
		{
			m_pGrid = (CESL_Grid * )m_pCM->FindControl(_T("MainGrid_Species"));
			GetDlgItem(IDC_gridDELETE_DATA_VOLUME)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_gridDELETE_DATA_DUPLICATE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_gridDELETE_DATA_BOOK)->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pGrid = (CESL_Grid * )m_pCM->FindControl(_T("MainGrid_Duplicate"));
			GetDlgItem(IDC_gridDELETE_DATA_SPECIES)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_gridDELETE_DATA_VOLUME)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_gridDELETE_DATA_BOOK)->ShowWindow(SW_HIDE);
		}
	}
	else if(m_nDeleteType == VOLUME_DELETE)
	{
		m_pDM = FindDM(_T("DM_�����ڷ�Ȯ��_��"));
		m_pGrid = (CESL_Grid * )m_pCM->FindControl(_T("MainGrid_Volume"));
		GetDlgItem(IDC_gridDELETE_DATA_SPECIES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridDELETE_DATA_DUPLICATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridDELETE_DATA_BOOK)->ShowWindow(SW_HIDE);
	}
	else if(m_nDeleteType == BOOK_DELETE)
	{
		m_pDM = FindDM(_T("DM_�����ڷ�Ȯ��_å"));
		m_pGrid = (CESL_Grid * )m_pCM->FindControl(_T("MainGrid_Book"));
		GetDlgItem(IDC_gridDELETE_DATA_SPECIES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridDELETE_DATA_DUPLICATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_gridDELETE_DATA_VOLUME)->ShowWindow(SW_HIDE);
	}
	if(!Init())
	{
		ESLAfxMessageBox(_T("�����ڷ�Ȯ�� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	m_pDeleteApi = new CBO_ACQ_Business_Api(this);
	
	if(m_nSpeciesType == BO_IMPORT || m_nSpeciesType == BO_BASIC)
	{
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_DeleteDlg::DisplayGrid()
{
	INT ids = m_pDM->GetRowCount();
	CString strMsg;
	strMsg.Format(_T("%d���� �ڷᰡ ���õǾ����ϴ�."), ids);
	GetDlgItem(IDC_stcDELETE_INFO)->SetWindowText(strMsg);
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pDM->GetRowCount() == 0) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

BOOL CBO_DeleteDlg::Init()
{
	CString strTmpData;
	if(m_pMainDM == NULL || m_pMainGrid == NULL) return FALSE;
	CString strAlias_Speices[6];
	CString strAlias_Volume[8];
	CString strAlias_Book[8];
	CString strAlias_Extra[2];

	CString strData_Speices[6];
	CString strData_Volume[8];
	CString strData_Book[8];
	CString strData_Extra[2];
	
	strAlias_Speices[0] = _T("IBS_��ǥ��");	strAlias_Speices[1] = _T("IBS_������");	strAlias_Speices[2] = _T("IBS_������");
	strAlias_Speices[3] = _T("IBS_�����");	strAlias_Speices[4] = _T("IBS_��KEY");	strAlias_Speices[5] = _T("BP_��������KEY");
	strAlias_Extra[0] = m_strPriceAlias;	strAlias_Extra[1] = m_strBookCntAlias;
	strAlias_Volume[0] = _T("IBS_��ǥ��");	strAlias_Volume[1] = _T("IBS_������");	strAlias_Volume[2] = _T("IBS_������");
	strAlias_Volume[3] = _T("IBS_�����");	strAlias_Volume[4] = _T("IBS_��KEY");	strAlias_Volume[5] = _T("BP_��������KEY");
	strAlias_Volume[6] = _T("BB_���Ϸù�ȣ");	strAlias_Volume[7] = _T("BB_�����");
	strAlias_Book[0] = _T("IBS_��ǥ��");	strAlias_Book[1] = _T("IBS_������");	strAlias_Book[2] = _T("IBS_������");
	strAlias_Book[3] = _T("IBS_�����");	strAlias_Book[4] = _T("IBS_��KEY");		strAlias_Book[5] = _T("BP_��������KEY");
	strAlias_Book[6] = _T("BB_�����");		strAlias_Book[7] = _T("BB_åKEY");

	INT ids = m_pMainGrid->SelectMakeList();
	ids = m_pMainGrid->SelectGetHeadPosition();
	INT nCurRow;
	while(ids >= 0)
	{
		m_pDM->InsertRow(-1);
		nCurRow = m_pDM->GetRowCount()-1;
		if(m_nDeleteType == SPECIES_DELETE)
		{
			m_pMainDM->GetCellData(strAlias_Speices, 6, strData_Speices, ids);
			m_pDM->SetCellData(strAlias_Speices, 6, strData_Speices, nCurRow);
		}
		else if(m_nDeleteType == VOLUME_DELETE)
		{
			m_pMainDM->GetCellData(strAlias_Volume, 8, strData_Volume, ids);
			m_pDM->SetCellData(strAlias_Volume, 8, strData_Volume, nCurRow);
		}		
		else if(m_nDeleteType == BOOK_DELETE)
		{
			m_pMainDM->GetCellData(strAlias_Book, 8, strData_Book, ids);
			m_pDM->SetCellData(strAlias_Book, 8, strData_Book, nCurRow);
		}
		
		m_pMainDM->GetCellData(strAlias_Extra, 2, strData_Extra, ids);
		m_pDM->SetCellData(_T("����"), strData_Extra[0], nCurRow);
		m_pDM->SetCellData(_T("å��"), strData_Extra[1], nCurRow);
		strTmpData.Format(_T("%d"), ids);
		m_pDM->SetCellData(_T("�θ��ε���"), strTmpData, nCurRow);
		ids = m_pMainGrid->SelectGetNext();
	}
	DisplayGrid();
	ids = m_pDM->GetRowCount();
	for(INT i=0 ; i<ids ; i++)
	{
		m_pGrid->SetAt(i, 0, _T("V"));
	}
	
	return TRUE;
}

VOID CBO_DeleteDlg::OnOK() 
{
	// TODO: Add extra validation here
	INT ids = -1;

	CGroupInfo pTargetGroup;
	pTargetGroup.SetAcqYear(m_pDeleteApi->GetTodayYear());
	pTargetGroup.SetGroupInfo(_T(""));
	pTargetGroup.SetGroupNo(_T("0"));

	CGroup_Select_Dlg dlg(this);
	dlg.SetSpeciesType(BO_RECYCLE);
	dlg.SetAcqYear(m_pDeleteApi->GetTodayYear());

	if(IDCANCEL == dlg.DoModal())
		return ;
		
	pTargetGroup.SetAcqYear(dlg.GetSelGroup()->GetAcqYear());
	pTargetGroup.SetGroupInfo(dlg.GetSelGroup()->GetGroupInfo());
	pTargetGroup.SetGroupNo(dlg.GetSelGroup()->GetGroupNo());

	if(m_nDeleteType == SPECIES_DELETE)
		ids = DeteleSpeciesData(TRUE, &pTargetGroup);
	else if(m_nDeleteType == VOLUME_DELETE)
		ids = DeleteVolumeData(TRUE, &pTargetGroup);
	else if(m_nDeleteType == BOOK_DELETE)
		ids = DeleteBookData(TRUE, &pTargetGroup);

	CString strDelOption;
	if(m_nDeleteOption > 0) strDelOption = _T("");
	else if(m_nDeleteOption < 0) strDelOption = _T("�ߺ��ڷ����");
	if(m_arrIdx.GetSize() > 0) m_bDeleteFlag = TRUE;
	MakeParentDeleteRowArray();
	m_pDeleteApi->DeleteSelectRow(m_pDM, &m_arrIdx, strDelOption);
	
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�����Ͻ� �ڷḦ ������ �� �����ϴ�."));
		return;
	}	
	else if(ids == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ �����Ͽ����ϴ�."));
	}
	DisplayGrid();	
	m_arrIdx.RemoveAll();
}

VOID CBO_DeleteDlg::OnbtnDBDELETE() 
{
	// TODO: Add your control notification handler code here
	INT ids = -1;
	if(m_nDeleteType == SPECIES_DELETE)
		ids = DeteleSpeciesData(FALSE);
	else if(m_nDeleteType == VOLUME_DELETE)
		ids = DeleteVolumeData(FALSE);
	else if(m_nDeleteType == BOOK_DELETE)
		ids = DeleteBookData(FALSE);

	CString strDelOption;
	if(m_nDeleteOption > 0) strDelOption = _T("");
	else if(m_nDeleteOption < 0) strDelOption = _T("�ߺ��ڷ����");
	if(m_arrIdx.GetSize() > 0) m_bDeleteFlag = TRUE;
	MakeParentDeleteRowArray();
	m_pDeleteApi->DeleteSelectRow(m_pDM, &m_arrIdx, strDelOption);

	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�����Ͻ� �ڷḦ ������ �� �����ϴ�."));
		return;
	}	
	else if(ids == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ �����Ͽ����ϴ�."));
	}
	DisplayGrid();	
	m_arrIdx.RemoveAll();
}

INT CBO_DeleteDlg::DeteleSpeciesData(BOOL bRecycleYN, CGroupInfo * pGroup/*=NULL*/)
{
	m_pDeleteApi->InitTempData();
	m_pDeleteApi->InitTempPtr();
	m_pDeleteApi->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pDeleteApi->SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	m_pDeleteApi->SetTempData(_T("SPECIES_TYPE"), (INT)m_nSpeciesType);
	m_pDeleteApi->SetTempData(_T("ACQ_TYPE"), (INT)m_nAcqType);
	m_pDeleteApi->SetTempData(_T("��Ű�˸��ƽ�"), _T("IBS_��KEY"));
	m_pDeleteApi->SetTempData(_T("��������Ű�˸��ƽ�"), _T("BP_��������KEY"));
	m_pDeleteApi->SetTempData(_T("��Ȱ��ó������"), bRecycleYN);
	m_pDeleteApi->SetTempPtr(_T("�����ε���"), (DWORD*)&m_arrIdx);
	m_pDeleteApi->SetTempPtr(_T("��Ű����Ʈ"), (DWORD*)m_pArrSpeciesKey);
	m_pDeleteApi->SetTempPtr(_T("��������Ű����Ʈ"), (DWORD*)m_pArrAcqKey);
	m_pDeleteApi->SetTempPtr(_T("�׷�����"), (DWORD*)pGroup);
	INT ids = m_pDeleteApi->SPFExecutePath(_T("������"));

	return ids;
}

INT CBO_DeleteDlg::DeleteVolumeData(BOOL bRecycleYN, CGroupInfo * pGroup/*=NULL*/)
{
	m_pDeleteApi->InitTempData();
	m_pDeleteApi->InitTempPtr();
	m_pDeleteApi->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pDeleteApi->SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	m_pDeleteApi->SetTempData(_T("SPECIES_TYPE"), (INT)m_nSpeciesType);
	m_pDeleteApi->SetTempData(_T("ACQ_TYPE"), (INT)m_nAcqType);
	m_pDeleteApi->SetTempData(_T("��Ű�˸��ƽ�"), _T("IBS_��KEY"));
	m_pDeleteApi->SetTempData(_T("��������Ű�˸��ƽ�"), _T("BP_��������KEY"));
	m_pDeleteApi->SetTempData(_T("���Ϸù�ȣ�˸��ƽ�"), _T("BB_���Ϸù�ȣ"));
	m_pDeleteApi->SetTempData(_T("��Ȱ��ó������"), bRecycleYN);
	m_pDeleteApi->SetTempPtr(_T("�����ε���"), (DWORD*)&m_arrIdx);
	m_pDeleteApi->SetTempPtr(_T("��Ű����Ʈ"), (DWORD*)m_pArrSpeciesKey);
	m_pDeleteApi->SetTempPtr(_T("��������Ű����Ʈ"), (DWORD*)m_pArrAcqKey);
	m_pDeleteApi->SetTempPtr(_T("���Ϸù�ȣ����Ʈ"), (DWORD*)m_pArrVolSortNo);
	m_pDeleteApi->SetTempPtr(_T("�׷�����"), (DWORD*)pGroup);

	INT ids;
	if(m_nSpeciesType == (INT)BO_MISSING)
		ids = m_pDeleteApi->SPFExecutePath(_T("�̳��ڷ����"));
	else
		ids = m_pDeleteApi->SPFExecutePath(_T("�ǻ���"));

	return ids;
}

INT CBO_DeleteDlg::MakeParentDeleteRowArray()
{
	INT nTmpData;
	INT nCnt = m_arrIdx.GetSize();

	CString strTmpData;
	for(INT i=0 ; i<nCnt ; i++)
	{
		strTmpData = m_pDM->GetCellData(_T("�θ��ε���"), m_arrIdx.GetAt(i));
		nTmpData = _ttoi(strTmpData.GetBuffer(0));
		AddParentIdxBySort(&m_arrParentIdx, nTmpData);
	}
	return 0;
}

INT CBO_DeleteDlg::AddParentIdxBySort(CArray<INT, INT> * pArrParentIdx, INT nData)
{
	INT nTmpData;
	INT nCnt = pArrParentIdx->GetSize();
	if(nCnt == 0)
	{
		pArrParentIdx->Add(nData);
		return 0;
	}
	for(INT i=0 ; i<nCnt ; i++)
	{
		nTmpData = pArrParentIdx->GetAt(i);
		if(nTmpData > nData)
		{
			pArrParentIdx->SetAtGrow(i, nData);
			return 0;
		}
	}
	pArrParentIdx->Add(nData);
	return 0;
}

INT CBO_DeleteDlg::DeleteBookData(BOOL bRecycleYN, CGroupInfo * pGroup/*=NULL*/)
{
	m_pDeleteApi->InitTempData();
	m_pDeleteApi->InitTempPtr();
	m_pDeleteApi->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pDeleteApi->SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	m_pDeleteApi->SetTempData(_T("SPECIES_TYPE"), (INT)m_nSpeciesType);
	m_pDeleteApi->SetTempData(_T("ACQ_TYPE"), (INT)m_nAcqType);
	m_pDeleteApi->SetTempData(_T("��Ű�˸��ƽ�"), _T("IBS_��KEY"));
	m_pDeleteApi->SetTempData(_T("��������Ű�˸��ƽ�"), _T("BP_��������KEY"));
	m_pDeleteApi->SetTempData(_T("åŰ�˸��ƽ�"), _T("BB_åKEY"));
	m_pDeleteApi->SetTempData(_T("��Ȱ��ó������"), bRecycleYN);
	m_pDeleteApi->SetTempPtr(_T("�����ε���"), (DWORD*)&m_arrIdx);
	m_pDeleteApi->SetTempPtr(_T("��Ű����Ʈ"), (DWORD*)m_pArrSpeciesKey);
	m_pDeleteApi->SetTempPtr(_T("��������Ű����Ʈ"), (DWORD*)m_pArrAcqKey);
	m_pDeleteApi->SetTempPtr(_T("���Ϸù�ȣ����Ʈ"), (DWORD*)m_pArrVolSortNo);
	m_pDeleteApi->SetTempPtr(_T("�׷�����"), (DWORD*)pGroup);
	INT ids = m_pDeleteApi->SPFExecutePath(_T("å����"));

	return ids;
}

HBRUSH CBO_DeleteDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
