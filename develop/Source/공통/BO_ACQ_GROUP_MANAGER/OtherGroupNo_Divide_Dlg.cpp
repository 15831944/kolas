// OtherGroupNo_Divide_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "OtherGroupNo_Divide_Dlg.h"
#include "OtherGroupNo_Select_Dlg.h"
#include "GroupPair.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_Divide_Dlg dialog


COtherGroupNo_Divide_Dlg::COtherGroupNo_Divide_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(COtherGroupNo_Divide_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COtherGroupNo_Divide_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM_BOOK = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pParentDM = NULL;
	m_pParentGrid = NULL;
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	m_nRow = -1;
	m_pSortItemSet = NULL;
}

COtherGroupNo_Divide_Dlg::~COtherGroupNo_Divide_Dlg()
{
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
	RemoveAllSelectDataArray();
}

VOID COtherGroupNo_Divide_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherGroupNo_Divide_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COtherGroupNo_Divide_Dlg, CDialog)
	//{{AFX_MSG_MAP(COtherGroupNo_Divide_Dlg)
	ON_BN_CLICKED(IDC_btnOTHER_GROUP_NO_DIVIDE_PREV, OnbtnOTHERGROUPNODIVIDEPREV)
	ON_BN_CLICKED(IDC_btnOTHER_GROUP_NO_DIVIDE_NEXT, OnbtnOTHERGROUPNODIVIDENEXT)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnDATA_SORT, OnbtnDATASORT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_Divide_Dlg message handlers

BOOL COtherGroupNo_Divide_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_�����׷��ȣ�и�")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	GetWindowRect(&m_rcInitial);

	m_pCM = FindCM(_T("CM_�����׷��ȣ�и�"));
	m_pDM = FindDM(_T("DM_�����׷��ȣ�и�"));
	m_pDM_BOOK = FindDM(_T("DM_�����׷��ȣ�и�_å"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	InitSortItem();
	Display(0);	
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID COtherGroupNo_Divide_Dlg::Display(INT idx)
{
	INT ids;
	CString errstr;
	switch(idx)
	{
	case 0:
		InitFirstLastIndex();
		m_pParentGrid->SelectMakeList();
		ids = m_pParentGrid->SelectGetHeadPosition();
		ShowNextPrevButton(ids);
		if (ids < 0)
		{
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		ids = m_pParentGrid->SelectGetPrev();
		ShowNextPrevButton(ids);
		if (ids == -1) return;
		break;
	case 1:
		ids = m_pParentGrid->SelectGetNext();
		ShowNextPrevButton(ids);
		if (ids == -1) return;
		break;
	}
	
	INT res = MakeCurSelGroupData(m_pParentDM->GetCellData(_T("���Ա���"), ids), m_pParentDM->GetCellData(_T("���Գ⵵"), ids), m_pParentDM->GetCellData(_T("�׷��ȣ"), ids));
	if( res < 0 )
	{
		ESLAfxMessageBox(_T("�׷쵥���͸� ������ �� �����ϴ�."));
		return ;
	}
	m_pCM->AllControlDisplay(ids);
	m_pGrid->Display();
	m_nIdx = ids;
}

VOID COtherGroupNo_Divide_Dlg::InitFirstLastIndex()
{
	INT ids = m_pParentGrid->SelectMakeList();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	m_nFirstIdx = m_pParentGrid->SelectGetHeadPosition();
	m_nLastIdx = m_pParentGrid->SelectGetTailPosition();
}

VOID COtherGroupNo_Divide_Dlg::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnOTHER_GROUP_NO_DIVIDE_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnOTHER_GROUP_NO_DIVIDE_NEXT)->EnableWindow(TRUE);
	
	if(m_nFirstIdx == idx || idx < 0)
		GetDlgItem(IDC_btnOTHER_GROUP_NO_DIVIDE_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx == idx || idx < 0)
		GetDlgItem(IDC_btnOTHER_GROUP_NO_DIVIDE_NEXT)->EnableWindow(FALSE);
}

INT COtherGroupNo_Divide_Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData;
	
	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("IBS_�ǻ���"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pDM->GetCellData(_T("IBS_��ǥ��"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_ǥ������"), strTmpData, nRow);
		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_������"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pDM->GetRowCount()-1) m_nRow = -1;
	}
	
	if(strDMFieldAlias == _T("UDF_���Ա���"))
	{	
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�Լ�����"), str, str);
	}
	else if(strDMFieldAlias == _T("UDF_�ڷ���¼���"))
	{
		if(str == _T("BOT111O")) str = _T("�����ڷ�");
		else if(str == _T("BOT112O")) str = _T("�����ڷ�");
		else if(str == _T("BOT211O")) str = _T("���������߱��ڷ�");
		else if(str == _T("BOT212O")) str = _T("���������߱��ڷ�");
		else if(str == _T("BOA111N")) str = _T("���Դ���ڷ�");
		else if(str == _T("BOA112N")) str = _T("�ֹ��ڷ�");
		else if(str == _T("BOA113O")) str = _T("�̳�ǰ�ڷ�");
		else if(str == _T("BOA114O")) str = _T("�����ΰ����ڷ�");
		else if(str == _T("BOA211O")) str = _T("���������ڷ�");
		else if(str == _T("BOA212O")) str = _T("�����̵���ڷ�");
		else if(str == _T("BOR111N")) str = _T("����ΰ��ڷ�");
		else if(str == _T("BOR112N")) str = _T("����ڷ�");
		else if(str == _T("BOR113O")) str = _T("�����ι�ȣ�ο��ڷ�");
		else if(str == _T("BOC111O")) str = _T("�����������ڷ�");
		else if(str == _T("BOC112O")) str = _T("�з�����ڷ�");
		else if(str == _T("BOC113N")) str = _T("��������ڷ�");
		else if(str == _T("BOC114O")) str = _T("�����ΰ����ڷ�");
		else if(str == _T("BOL111O")) str = _T("�����ΰ��ڷ�");
		else if(str == _T("BOL112N")) str = _T("�谡�ڷ�");
		else if(str == _T("BOL113O")) str = _T("���������ڷ�");
		else if(str == _T("BOL114O")) str = _T("�ļ��ڷ�");
		else if(str == _T("BOL115O")) str = _T("�����ڷ�");
		else if(str == _T("BOL211O")) str = _T("���ܴ����ڷ�");
		else if(str == _T("BOL212O")) str = _T("���������ڷ�");
		
		else if(str == _T("BOL411O")) str = _T("����å�η����������ڷ�");
		else if(str == _T("BOL511O")) str = _T("���յ���Ÿ���ݳ�");

		else if(str == _T("BOL611O")) str = _T("���յ���Ÿ������");

		else if(str == _T("BOL213O")) str = _T("Ư�������ڷ�");
		else if(str == _T("BOL214O")) str = _T("��޴����ڷ�");
		else if(str == _T("BOL215O")) str = _T("����ڷ�");
	}
	else if(strDMFieldAlias == _T("UDF_����"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
	}
	else if(strDMFieldAlias == _T("BS_��������"))
	{
		strTmpData = m_pDM->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		if(strTmpData.IsEmpty()) str = _T("");
		else if(strTmpData == _T("1"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC��������ȣ����"), str, str);
		}
		else if(strTmpData == _T("2"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC��������ȣ����"), str, str);
		}
	}
	return 0 ;
}

INT COtherGroupNo_Divide_Dlg::MakeCurSelGroupData(CString strAcqCode, CString strAcqYear, CString strGroupNo)
{
	CString strQuery;
	strQuery.Format(
		_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = B.SPECIES_KEY AND B.ACQ_CODE='%s' AND B.ACQ_YEAR='%s' AND B.SEQ_NO=%s"),
		strAcqCode, strAcqYear, strGroupNo);
	INT ids = m_pDM_BOOK->RefreshDataManager(strQuery);
	if(ids < 0) return -1;

	m_pDM->FreeData();
	ids = MakeSpeciesInfoByBook();
	return ids;
}

VOID COtherGroupNo_Divide_Dlg::OnbtnOTHERGROUPNODIVIDEPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID COtherGroupNo_Divide_Dlg::OnbtnOTHERGROUPNODIVIDENEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);	
}

VOID COtherGroupNo_Divide_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	COtherGroupNo_Select_Dlg dlg(this);
	dlg.SetAcqYear(m_pParentDM->GetCellData(_T("���Գ⵵"), m_nIdx));
	dlg.SetAcqCode(m_pParentDM->GetCellData(_T("���Ա���"), m_nIdx));
	
	if(IDOK == dlg.DoModal())
	{
		CGroupInfo * pGroup = dlg.GetSelGroup();
		if(CheckIsSameGroup(pGroup->GetAcqYear(), pGroup->GetGroupInfo()))
		{
			ESLAfxMessageBox(_T("���ϱ׷��� �����Ͽ����ϴ�. �ٽ� ������ �ֽʽÿ�."));
			return ;
		}
		ids = MakeSelectDataArray(m_pDM, m_pGrid, _T("IBS_��KEY"));
		if(ids < 0) return ;
		
		CGroupPair mgr(this);
		mgr.SetTargetGroup(pGroup);
		mgr.SetSelectDataArray(&m_ArrSelData);
		mgr.SetGroupType(OTHER_GROUP_NO);
		mgr.SetAcqCode(m_pParentDM->GetCellData(_T("���Ա���"), m_nIdx));
		mgr.SetMainDM(m_pParentDM);
		ids = mgr.DivideGroup();
		if(ids < 0)
			ESLAfxMessageBox(_T("������ �ڷḦ �и��� �� �����ϴ�."));
		else 
		{
			ESLAfxMessageBox(_T("������ �ڷḦ �и��Ͽ����ϴ�."));
			DeleteSelectedRow();
			CGroupInfo Group;
			Group.SetAcqYear(m_pParentDM->GetCellData(_T("���Գ⵵"), m_nIdx));
			Group.SetGroupInfo(m_pParentDM->GetCellData(_T("�׷��ȣ"), m_nIdx));
			mgr.UpdateGroupBookCnt(&Group);
			m_pGrid->Display();
		}
	}
}

BOOL COtherGroupNo_Divide_Dlg::CheckIsSameGroup(CString strAcqYear, CString strGroupNo)
{
	CString strCurAcqYear, strCurGroupNo;
	strCurAcqYear = m_pParentDM->GetCellData(_T("���Գ⵵"), m_nIdx);
	strCurGroupNo = m_pParentDM->GetCellData(_T("�׷��ȣ"), m_nIdx);
	if(strCurGroupNo == strGroupNo && strCurAcqYear == strAcqYear)
		return TRUE;
	return FALSE;	
}

INT COtherGroupNo_Divide_Dlg::DeleteSelectedRow()
{
	INT ids = m_pGrid->SelectMakeList();
	INT i = m_pGrid->SelectGetTailPosition();
	while(i>=0)
	{
		m_pDM->DeleteRow(i);
		i = m_pGrid->SelectGetPrev();
	}
	return 0;
}

INT COtherGroupNo_Divide_Dlg::RemoveAllSelectDataArray()
{
	INT nCnt = m_ArrSelData.GetSize();
	for(INT i=0 ; i<nCnt ; i++)
	{
		delete m_ArrSelData.GetAt(i);
	}
	
	m_ArrSelData.RemoveAll();
	return 0;
}

INT COtherGroupNo_Divide_Dlg::MakeSelectDataArray(CESL_DataMgr *pDM, CESL_Grid *pGrid, CString strAlias)
{
	if(pGrid == NULL || pDM == NULL || strAlias.IsEmpty()) return -1;
	
	RemoveAllSelectDataArray();
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = pGrid->SelectGetHeadPosition();
	
	CStringArray * pArrRecKey = new CStringArray;
	pArrRecKey->RemoveAll();
	
	INT nCnt = 0;
	CString strRecKey;
	while(i >= 0)
	{
		++nCnt;
		strRecKey = pDM->GetCellData(strAlias, i);
		if(MAX_REC_KEY_CNT == nCnt)
		{
			m_ArrSelData.Add(pArrRecKey);
			pArrRecKey = new CStringArray;
			pArrRecKey->Add(strRecKey);
			nCnt = 1;
		}
		else 
		{
			pArrRecKey->Add(strRecKey);
		}
		
		i = pGrid->SelectGetNext();
	}
	m_ArrSelData.Add(pArrRecKey);
	return 0;
}

INT COtherGroupNo_Divide_Dlg::CheckSelectDataValid()
{
	if(m_pDM->GetRowCount() == 0) return -1;
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return ids;
	INT i = m_pGrid->SelectGetHeadPosition();
	if(i < 0) return i;
	
	return 0;
}

INT COtherGroupNo_Divide_Dlg::MakeSpeciesInfoByBook()
{
	if(!m_pDM_BOOK || !m_pDM) return -1;
	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	if(nRowCnt == 0) return 0;
	
	m_pDM_BOOK->SORT(_T("IBS_��KEY"));
	
	MakeOtherInfoByBook(m_pDM, m_pDM_BOOK, 0, nRowCnt-1, _T("IBS_��KEY"));
	
	return 0;
}

INT COtherGroupNo_Divide_Dlg::MakeOtherInfoByBook(CESL_DataMgr * pTargetDM, CESL_DataMgr * pSourceDM, INT startIdx, INT endIdx, CString strAlias)
{
	CString strKey, strTmpKey, strTmpData, strDeliveryYN, strSpeciesWorkingStatus, strAcqCode, strGroupNo;
	INT ids, cnt, nDBookCnt;
	DOUBLE nTotalPrice = 0.0;
	cnt = nDBookCnt = 0;

	TCHAR* pEndPtr = NULL;
	
	for(INT i=startIdx ; i<=endIdx; i++)
	{
		pSourceDM->GetCellData(strAlias, i, strTmpKey);
		if(!strKey.IsEmpty() && strKey != strTmpKey)
		{
			ids = AddToTargetDM(pTargetDM, pSourceDM, i-1);
			if(ids < 0) break;
			strTmpData.Format(_T("%d"), cnt);
			pTargetDM->SetCellData(_T("UDF_å��"), strTmpData, ids);
			strTmpData.Format(_T("%.0f"), nTotalPrice);
			pTargetDM->SetCellData(_T("UDF_������"), strTmpData, ids);
			pTargetDM->SetCellData(_T("UDF_����"), strTmpData, ids);
			pTargetDM->SetCellData(_T("UDF_�ڷ���¼���"), strSpeciesWorkingStatus, ids);
			pTargetDM->SetCellData(_T("UDF_���Ա���"), strAcqCode, ids);
			pTargetDM->SetCellData(_T("UDF_�׷��ȣ"), strGroupNo, ids);

			strSpeciesWorkingStatus.Empty();
			strKey = strTmpKey;
			cnt = 1;
			strTmpData = pSourceDM->GetCellData(_T("BB_����"), i);

			nTotalPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			strSpeciesWorkingStatus = pSourceDM->GetCellData(_T("BB_�ڷ����"), i);
			strAcqCode = pSourceDM->GetCellData(_T("BB_���Ա���"), i);
			strGroupNo = pSourceDM->GetCellData(_T("BB_�׷��ȣ"), i);
		}
		else 
		{
			cnt++;
			strTmpData = pSourceDM->GetCellData(_T("BB_����"), i);

			nTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			strTmpData = pSourceDM->GetCellData(_T("BB_�ڷ����"), i);
			strSpeciesWorkingStatus = strTmpData;
			strAcqCode = pSourceDM->GetCellData(_T("BB_���Ա���"), i);
			strGroupNo = pSourceDM->GetCellData(_T("BB_�׷��ȣ"), i);
		}
	}
	if(i > endIdx)
	{
		ids = AddToTargetDM(pTargetDM, pSourceDM, i-1);
		strTmpData.Format(_T("%d"), cnt);
		pTargetDM->SetCellData(_T("UDF_å��"), strTmpData, ids);
		strTmpData.Format(_T("%.0f"), nTotalPrice);
		pTargetDM->SetCellData(_T("UDF_������"), strTmpData, ids);
		pTargetDM->SetCellData(_T("UDF_����"), strTmpData, ids);
		pTargetDM->SetCellData(_T("UDF_�ڷ���¼���"), strSpeciesWorkingStatus, ids);
		pTargetDM->SetCellData(_T("UDF_���Ա���"), strAcqCode, ids);
		pTargetDM->SetCellData(_T("UDF_�׷��ȣ"), strGroupNo, ids);
	}
	return 0;
}

INT COtherGroupNo_Divide_Dlg::AddToTargetDM(CESL_DataMgr *pTargetDM, CESL_DataMgr *pSourceDM, INT idx)
{
	if (!pTargetDM || !pSourceDM) return -1000;
	if( idx < 0) return -1000;
	
	INT col, nCol;
	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT nCurRow, nColCnt;
	pTargetDM->InsertRow(-1);
	nCurRow = pTargetDM->GetRowCount() - 1;
	nColCnt = pSourceDM->m_nCols;
	if(nColCnt < 0) nColCnt = pSourceDM->GetRefColumnCount();
	
	for (col = 0; col < nColCnt; col++) {
		if (pSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		nCol = pTargetDM->FindColumn(FIELD_ALIAS);
		if(nCol >= 0)	pTargetDM->SetCellData(nCurRow, nCol, pSourceDM->GetCellData(idx, col));
	}
	
	return nCurRow;
}
VOID COtherGroupNo_Divide_Dlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_pGrid->GetSafeHwnd() == NULL) return;
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		
		MoveWindow(rcWnd);
		return;
	}
	
	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			
		
		MoveWindow(rcWnd);
		return;
	}
	CRect rect;
	GetClientRect(rect);
	rect.top += 58;
	rect.bottom -= 4;
	rect.left += 4;
	rect.right -= 4;
	
	m_pGrid->MoveWindow(rect);		
}

VOID COtherGroupNo_Divide_Dlg::OnbtnDATASORT() 
{
	// TODO: Add your control notification handler code here
	if(m_pSortItemSet == NULL || m_pDM == NULL) 
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return;
	}
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(1);
	if(IDOK == dlg.DoModal())
		m_pGrid->Display();
}

BOOL COtherGroupNo_Divide_Dlg::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("UDF_�׷��ȣ"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_�������м���"), _T("BS_��������"));
	m_pSortItemSet->ReadSortItem();
	return TRUE;
}

HBRUSH COtherGroupNo_Divide_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
