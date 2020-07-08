// SeAccountStatement.cpp : implementation file
//

#include "stdafx.h"
#include "SeAccountStatement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeAccountStatement dialog


CSeAccountStatement::CSeAccountStatement(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeAccountStatement)
	//}}AFX_DATA_INIT

	m_pDM_Book = NULL;
	m_pDM_Species = NULL;
	m_pCM = NULL;
	m_pGrid_Book = NULL;
	m_pGrid_Species = NULL;

	m_strDepartMentPrint = _T("");
	m_strAcqExpDatePrint = _T("");


	m_strSpeciesCnt = _T("");
	m_strBookCnt = _T("");
	m_strTotalAccount = _T("");


}
CSeAccountStatement::~CSeAccountStatement()
{
}


VOID CSeAccountStatement::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeAccountStatement)
	DDX_Control(pDX, IDC_tabACCOUNT_STATEMENT, m_ctrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeAccountStatement, CDialog)
	//{{AFX_MSG_MAP(CSeAccountStatement)
	ON_BN_CLICKED(IDC_btnACC_SEARCH, OnbtnACCSEARCH)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabACCOUNT_STATEMENT, OnSelchangetabACCOUNTSTATEMENT)
	ON_BN_CLICKED(IDC_btnACC_PRINT, OnbtnACCPRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeAccountStatement message handlers

BOOL CSeAccountStatement::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("����_����_�������"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_�������") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_����_�������"));
	m_pGrid_Book = (CESL_Grid*)m_pCM->FindControl(_T("�׸���_å"));
	m_pGrid_Species = (CESL_Grid*)m_pCM->FindControl(_T("�׸���_��"));
	m_pDM_Book = FindDM(_T("DM_����_����_�������_å"));
	m_pDM_Species = FindDM(_T("DM_����_����_�������_��"));

	if(!m_pDM_Book || !m_pCM || !m_pGrid_Book || !m_pGrid_Species) {
		AfxMessageBox( _T("DM,CM,grid Error : ����_����_�������") );
		return false;
	}


	// Date ��Ʈ�� ����
	/// ��¥ ��Ʈ�� UnCheck�ϱ�
	SYSTEMTIME systime;	
	CDateTimeCtrl* pTimeCtrl = NULL;
	
	const INT nCnt = 2;
	INT nIDC[nCnt] =
	{
		IDC_datACC_ACQ_DATE1 , IDC_datACC_ACQ_DATE2 
	};
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[i]);
		pTimeCtrl->GetTime(&systime);
		DateTime_SetSystemtime(pTimeCtrl->m_hWnd , GDT_NONE , &systime );
	}
	

	//tab��Ʈ�� ���� 
	m_ctrlTab.InsertItem(0,_T("å����"));
	m_ctrlTab.InsertItem(1,_T("������"));

	SendMessage(WM_SIZE,NULL,NULL);



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// �޷��� ������ ��¥ ��Ʈ���� �� ��������
CString CSeAccountStatement::GetDate( UINT nCtrlID )
{
	CString sDate;
	
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( nCtrlID );
	
	CTime tTime;
	
	DWORD dwResult = pDate->GetTime( tTime );

	if (dwResult != GDT_VALID ) return _T("");

	sDate.Format( _T("%04d/%02d/%02d"), tTime.GetYear(), tTime.GetMonth(), tTime.GetDay() );
	
	return sDate;
}

VOID CSeAccountStatement::OnbtnACCSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	
    CString strWhere;
    CString strtmp;
  
	strWhere = _T("I.REC_KEY = P.SPECIES_KEY")
			   _T(" AND I.REC_KEY = B.SPECIES_KEY")
			   _T(" AND I.REC_KEY = V.SPECIES_KEY")
			   _T(" AND A.REC_KEY = B.ACQ_TYPE_VOL_KEY")
			   _T(" AND V.REC_KEY = B.VOL_KEY")
			   _T(" AND B.BOOK_TYPE = 'C'");

  
	//�μ��ڵ�
	CString strSHELF_LOC_CODE;
	m_pCM->GetControlMgrData(_T("�μ��ڵ�"),strSHELF_LOC_CODE);
	
	strSHELF_LOC_CODE.TrimLeft();
	strSHELF_LOC_CODE.TrimRight();

    if(!strSHELF_LOC_CODE.IsEmpty())
    {
		strtmp.Format(_T(" AND B.SHELF_LOC_CODE = '%s'"),strSHELF_LOC_CODE);
		strWhere += strtmp;
    }

	//�Լ�������
	const INT nCnt = 2;
	INT nIDC[nCnt] =
	{
		IDC_datACC_ACQ_DATE1 , IDC_datACC_ACQ_DATE2 
	};

	CString strACQ_EXP_DATE1,strACQ_EXP_DATE2;


	strACQ_EXP_DATE1 = GetDate(nIDC[0]);
	if( !GetDlgItem(nIDC[0])->IsWindowEnabled() ) strACQ_EXP_DATE1 = _T("");

	strACQ_EXP_DATE2 = GetDate(nIDC[1]);
	if( !GetDlgItem(nIDC[1])->IsWindowEnabled() ) strACQ_EXP_DATE2 = _T("");


	strACQ_EXP_DATE1.TrimLeft();
	strACQ_EXP_DATE1.TrimRight();
	strACQ_EXP_DATE2.TrimLeft();
	strACQ_EXP_DATE2.TrimRight();

	if(strACQ_EXP_DATE1.IsEmpty() && !strACQ_EXP_DATE2.IsEmpty())
	{
		AfxMessageBox(_T("�Լ������� ������ �޺κп��� ������ �ԷµǾ����ϴ�!"));
		return;
	}
	
	if(!strACQ_EXP_DATE1.IsEmpty() || !strACQ_EXP_DATE2.IsEmpty())
	{
		//�谡���� ����
		strtmp = MakeRangeQuery(_T("A.ACQ_EXP_DATE"),strACQ_EXP_DATE1,strACQ_EXP_DATE2);

		if(!strtmp.IsEmpty())
		{
			strWhere += _T(" AND ");
			strWhere += strtmp;
		}

	}

	m_pDM_Book->RefreshDataManager(strWhere);

	INT nRow = m_pDM_Book->GetRowCount();
	if(0 == nRow) 
	{
		AfxMessageBox(_T("�˻� ����� �����ϴ�!"));
	}
	else
	{
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM_Book,_T("�ڷ�Ǳ���"),_T("�μ��ڵ�"));
		GetIssuePrice();
	}
	
	//��¿� �ʿ��� �μ���, �Լ������� ������ ������ ����
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�ڷ�Ǳ���"),strSHELF_LOC_CODE,m_strDepartMentPrint);

	if(!strACQ_EXP_DATE1.IsEmpty() || !strACQ_EXP_DATE2.IsEmpty())
	{
		m_strAcqExpDatePrint.Format(_T("%s ~ %s"),strACQ_EXP_DATE1,strACQ_EXP_DATE2);
	}
	else
	{
		m_strAcqExpDatePrint = _T("�������");
	}


	MakeSpeciesInfo();
	GetTotalAccount();

	m_pGrid_Book->Display();
	m_pGrid_Species->Display();
}


CString CSeAccountStatement::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = '%s'"),strFieldName,strData1);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

VOID CSeAccountStatement::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	CRect rectTmp;
	GetClientRect(rect);
	rectTmp = rect;
	
	//�� 
	CWnd* pWnd = GetDlgItem(IDC_tabACCOUNT_STATEMENT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp.top = 50;
	rectTmp.bottom -= 35;
	rectTmp.left += 5;
	rectTmp.right -= 5;

	pWnd->MoveWindow(rectTmp);


	//�׸���1
	pWnd = GetDlgItem(IDC_gridACCOUNT_STATEMENT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp.top += 25;
	rectTmp.bottom -= 5;
	rectTmp.left += 5;
	rectTmp.right -= 5;

	pWnd->MoveWindow(rectTmp);


	//�׸���2
	pWnd = GetDlgItem(IDC_gridACCOUNT_STATEMENT2);
	if(pWnd->GetSafeHwnd() == NULL) return;

	pWnd->MoveWindow(rectTmp);


	//��ư
	pWnd = GetDlgItem(IDCANCEL);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 30;
	rectTmp.bottom -= 5;
	rectTmp.left = rectTmp.right - 110;
	rectTmp.right -= 5;

	pWnd->MoveWindow(rectTmp);
	

	//���� 
	//static
	pWnd = GetDlgItem(IDC_stcACC_SPECIES_CNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 25;
	rectTmp.bottom -= 5;
	rectTmp.left += 5;
	rectTmp.right = rectTmp.left + 30;

	pWnd->MoveWindow(rectTmp);

	//edit
	pWnd = GetDlgItem(IDC_edtACC_SPECIES_CNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 28;
	rectTmp.bottom -= 8;
	rectTmp.left += 40;
	rectTmp.right = rectTmp.left + 40;

	pWnd->MoveWindow(rectTmp);


	//å��
	//static
	pWnd = GetDlgItem(IDC_stcACC_BOOK_CNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 25;
	rectTmp.bottom -= 5;
	rectTmp.left += 85;
	rectTmp.right = rectTmp.left + 30;

	pWnd->MoveWindow(rectTmp);

	//edit
	pWnd = GetDlgItem(IDC_edtACC_BOOK_CNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 28;
	rectTmp.bottom -= 8;
	rectTmp.left += 120;
	rectTmp.right = rectTmp.left + 40;

	pWnd->MoveWindow(rectTmp);


	//�����갡
	//static
	pWnd = GetDlgItem(IDC_stcACC_TOTAL_ACCOUNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 25;
	rectTmp.bottom -= 5;
	rectTmp.left += 170;
	rectTmp.right = rectTmp.left + 50;

	pWnd->MoveWindow(rectTmp);

	//edit
	pWnd = GetDlgItem(IDC_edtACC_TOTAL_ACCOUNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 28;
	rectTmp.bottom -= 8;
	rectTmp.left += 225;
	rectTmp.right = rectTmp.left + 70;

	pWnd->MoveWindow(rectTmp);


	/*
	//���� 
	//static
	pWnd = GetDlgItem(IDC_stcACC_SPECIES_CNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 30;
	rectTmp.bottom -= 5;
	rectTmp.left += 5;
	rectTmp.right = rectTmp.left + 50;

	pWnd->MoveWindow(rectTmp);

	//edit
	pWnd = GetDlgItem(IDC_edtACC_SPECIES_CNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 30;
	rectTmp.bottom -= 5;
	rectTmp.left += 55;
	rectTmp.right = rectTmp.left + 105;

	pWnd->MoveWindow(rectTmp);


	//å��
	//static
	pWnd = GetDlgItem(IDC_stcACC_BOOK_CNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 30;
	rectTmp.bottom -= 5;
	rectTmp.left += 110;
	rectTmp.right = rectTmp.left + 160;

	pWnd->MoveWindow(rectTmp);

	//edit
	pWnd = GetDlgItem(IDC_edtACC_BOOK_CNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 30;
	rectTmp.bottom -= 5;
	rectTmp.left += 165;
	rectTmp.right = rectTmp.left + 215;

	pWnd->MoveWindow(rectTmp);


	//�����갡
	//static
	pWnd = GetDlgItem(IDC_stcACC_TOTAL_ACCOUNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 30;
	rectTmp.bottom -= 5;
	rectTmp.left += 230;
	rectTmp.right = rectTmp.left + 260;

	pWnd->MoveWindow(rectTmp);

	//edit
	pWnd = GetDlgItem(IDC_edtACC_TOTAL_ACCOUNT);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 30;
	rectTmp.bottom -= 5;
	rectTmp.left += 265;
	rectTmp.right = rectTmp.left + 320;

	pWnd->MoveWindow(rectTmp);
	*/

	/*
	//static
	pWnd = GetDlgItem(IDC_stcACCOUNT_TOTAL);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rectTmp = rect;
	rectTmp.top = rectTmp.bottom - 30;
	rectTmp.bottom -= 5;
	rectTmp.left += 5;
	rectTmp.right = rectTmp.left + 300;

	pWnd->MoveWindow(rectTmp);
	*/

}

VOID CSeAccountStatement::GetIssuePrice()
{

	CString strPrice;
	CString strACQ_EXP_DATE;
	CString strSTART_DATE;
	CString strEND_DATE;

	INT nPrice = 0;
	INT nRow = m_pDM_Book->GetRowCount();

	for(INT i=0;i<nRow;i++)
	{
		strACQ_EXP_DATE = m_pDM_Book->GetCellData(_T("�Լ�������"),i);
		
		if(strACQ_EXP_DATE.IsEmpty()) 
		{
			m_pDM_Book->SetCellData(_T("�ܰ�"),_T("0"),i);
			continue;
		}
		
		strSTART_DATE = m_pDM_Book->GetCellData(_T("����������"),i);
		strEND_DATE = m_pDM_Book->GetCellData(_T("����������"),i);		
		
		
		if(CompareStringDates(strSTART_DATE,strACQ_EXP_DATE)<=0 &&
		   CompareStringDates(strEND_DATE,strACQ_EXP_DATE)>=0)
		{
			strPrice = m_pDM_Book->GetCellData(_T("���Դܰ�"),i);
			strPrice.TrimLeft();
			strPrice.TrimRight();
			if(strPrice.IsEmpty()) strPrice = _T("0");

			m_pDM_Book->SetCellData(_T("�ܰ�"),strPrice,i);

		}
		else
		{
			
			strPrice = m_pDM_Book->GetCellData(_T("�����̷´ܰ�"),i);
			strPrice.TrimLeft();
			strPrice.TrimRight();
			if(strPrice.IsEmpty()) strPrice = _T("0");

			m_pDM_Book->SetCellData(_T("�ܰ�"),strPrice,i);

		}
	
	}


}

INT CSeAccountStatement::CompareStringDates(CString date1, CString date2)
{
	if(date1.GetLength() == 0 || date2.GetLength() == 0) return 0;
	INT year, month, day;
	ExtractStringDateToIntDate(date1, &year, &month, &day);
	COleDateTime t1(year, month, day, 0, 0, 0);
	ExtractStringDateToIntDate(date2, &year, &month, &day);
	COleDateTime t2(year, month, day, 0, 0, 0);
	if(t1 > t2) return 1;
	else if(t1 == t2) return 0;
	return -1;

}

VOID CSeAccountStatement::ExtractStringDateToIntDate(CString lastExpDate, INT *year, INT *month, INT *day)
{
	if(lastExpDate.GetLength() == 0) return ;
	INT idx, startidx, lastidx;
	idx = startidx = 0;
	lastidx = lastExpDate.GetLength();
	idx = lastExpDate.Find(_T("/"), startidx);
	if(idx > 0)
		*year = _ttoi((lastExpDate.Left(idx)).GetBuffer(0));
	startidx = idx + 1;
	idx = lastExpDate.Find(_T("/"), startidx);
	if(idx > 0)
		*month = _ttoi((lastExpDate.Mid(startidx, idx - 1)).GetBuffer(0));
	startidx = idx + 1;
	*day = _ttoi((lastExpDate.Right(lastidx - startidx)).GetBuffer(0));
	return ;
}

VOID CSeAccountStatement::OnSelchangetabACCOUNTSTATEMENT(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT nCurSel = m_ctrlTab.GetCurSel();


	if(0 == nCurSel)
	{
		m_pGrid_Book->ShowWindow(TRUE);
		m_pGrid_Species->ShowWindow(FALSE);
		m_pGrid_Book->BringWindowToTop();
	}
	else
	{
		m_pGrid_Book->ShowWindow(FALSE);
		m_pGrid_Species->ShowWindow(TRUE);
		m_pGrid_Species->BringWindowToTop();
	}


	*pResult = 0;
}

VOID CSeAccountStatement::MakeSpeciesInfo()
{
	//åDM�� ���� ��DM�� �����Ѵ�


	m_pDM_Species->FreeData();
	INT nRowCntBook = m_pDM_Book->GetRowCount();
	INT nRowCntSpecies = -1;
	INT nRowIdxSpecies = -1;
	CString strSHELF_LOC_CODE;
	CString strBeforeCode;
	CString strREC_KEY;
	CString strBeforeKey;
	CString strBeforeVolKey;
	CString strVolKey;

	CString strPrice;
	CString strBookCnt;
	CString strVolCnt;
	INT nPrice = 0;
	INT nPriceSum = 0;
	INT nBookCnt = 0;
	INT nVolCnt = 0;


	for(INT i=0;i<nRowCntBook;i++)
	{
		strSHELF_LOC_CODE = m_pDM_Book->GetCellData(_T("�μ��ڵ�"),i);
		strREC_KEY = m_pDM_Book->GetCellData(_T("��Ű"),i);
		strVolKey = m_pDM_Book->GetCellData(_T("��Ű"),i);
		
		nBookCnt++;
		if( strVolKey != strBeforeVolKey || i == 0)
			nVolCnt++;

		if(0 == i) 
		{
			strBeforeCode = strSHELF_LOC_CODE;
			strBeforeKey = strREC_KEY;		
			strBeforeVolKey = strVolKey;

			strPrice = m_pDM_Book->GetCellData(_T("�ܰ�"),i);
			nPrice = 0;
			if(!strPrice.IsEmpty()) 
				nPrice = _ttoi(strPrice.GetBuffer(0));
			nPriceSum += nPrice;
			

			m_pDM_Species->InsertRow(-1);	
			nRowCntSpecies = m_pDM_Species->GetRowCount();		
			nRowIdxSpecies = nRowCntSpecies - 1;
			CopyDMToDMByAlias(m_pDM_Book,i,m_pDM_Species,nRowIdxSpecies);

		}
		else
		{
			if(strBeforeCode == strSHELF_LOC_CODE)
			{	
				if(strBeforeKey == strREC_KEY)
				{					
					//�����ջ길 �Ѵ�
					strPrice = m_pDM_Book->GetCellData(_T("�ܰ�"),i);
					nPrice = 0;
					if(!strPrice.IsEmpty()) nPrice = _ttoi(strPrice.GetBuffer(0));
					nPriceSum += nPrice;
				}
				else
				{
					//������ ���갡�� å���� �����ϰ� ���ο� row�� �����
					strPrice.Format(_T("%d"),nPriceSum);
					m_pDM_Species->SetCellData(_T("���갡"),strPrice,nRowIdxSpecies);
					nPriceSum = 0;

					//�����ջ�
					strPrice = m_pDM_Book->GetCellData(_T("�ܰ�"),i);
					nPrice = 0;
					if(!strPrice.IsEmpty()) nPrice = _ttoi(strPrice.GetBuffer(0));
					nPriceSum += nPrice;
					
					//å�� ���
					nBookCnt--;
					nVolCnt--;
					strBookCnt.Format(_T("%d"),nBookCnt);
					strVolCnt.Format(_T("%d"),nVolCnt);
					m_pDM_Species->SetCellData(_T("å��"),strBookCnt,nRowIdxSpecies);
					m_pDM_Species->SetCellData(_T("�Ǽ�"),strVolCnt,nRowIdxSpecies);
					nBookCnt = 1; nVolCnt = 1;
					m_pDM_Species->InsertRow(-1);	
					nRowCntSpecies = m_pDM_Species->GetRowCount();		
					nRowIdxSpecies = nRowCntSpecies - 1;
					CopyDMToDMByAlias(m_pDM_Book,i,m_pDM_Species,nRowIdxSpecies);
				}
			}
			else
			{
				
				//������ ���갡�� �����ϰ� ���ο� row�� �����
				strPrice.Format(_T("%d"),nPriceSum);
				m_pDM_Species->SetCellData(_T("���갡"),strPrice,nRowIdxSpecies);
				nPriceSum = 0;
				
				//�����ջ�
				strPrice = m_pDM_Book->GetCellData(_T("�ܰ�"),i);
				nPrice = 0;
				if(!strPrice.IsEmpty()) nPrice = _ttoi(strPrice.GetBuffer(0));
				nPriceSum += nPrice;

				//å�� ���
				nBookCnt--;
				nVolCnt--;
				strBookCnt.Format(_T("%d"),nBookCnt);
				strVolCnt.Format(_T("%d"),nVolCnt);
				m_pDM_Species->SetCellData(_T("å��"),strBookCnt,nRowIdxSpecies);
				m_pDM_Species->SetCellData(_T("�Ǽ�"),strVolCnt,nRowIdxSpecies);
				nBookCnt = 1; nVolCnt = 1;
				
				m_pDM_Species->InsertRow(-1);	
				nRowCntSpecies = m_pDM_Species->GetRowCount();		
				nRowIdxSpecies = nRowCntSpecies - 1;
				CopyDMToDMByAlias(m_pDM_Book,i,m_pDM_Species,nRowIdxSpecies);				

			}

			strBeforeCode = strSHELF_LOC_CODE;
			strBeforeKey = strREC_KEY;	
			strBeforeVolKey = strVolKey;
		}

	}

	strPrice.Format(_T("%d"),nPriceSum);
	m_pDM_Species->SetCellData(_T("���갡"),strPrice,nRowIdxSpecies);

	strBookCnt.Format(_T("%d"),nBookCnt);
	m_pDM_Species->SetCellData(_T("å��"),strBookCnt,nRowIdxSpecies);

	strBookCnt.Format(_T("%d"),nBookCnt);
	m_pDM_Species->SetCellData(_T("�Ǽ�"),strVolCnt,nRowIdxSpecies);


}


INT CSeAccountStatement::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

VOID CSeAccountStatement::GetTotalAccount()
{

	INT nRowCntBook = m_pDM_Book->GetRowCount();
	
	CStringArray arraySpeciesKey;
	CString strSpeciesKey;

	CString strPrice;

	INT nPrice = 0;
	INT nPriceSum = 0;
	

	for(INT i=0;i<nRowCntBook;i++)
	{
		strSpeciesKey = m_pDM_Book->GetCellData(_T("��Ű"),i);
		if(!CheckIsHave(&arraySpeciesKey, strSpeciesKey))
		{
			arraySpeciesKey.Add(strSpeciesKey);
		}
		
		nPrice = 0;
		strPrice = m_pDM_Book->GetCellData(_T("�ܰ�"),i);
		if(!strPrice.IsEmpty()) nPrice = _ttoi(strPrice.GetBuffer(0));
		nPriceSum += nPrice;

	}

	INT nSpeciesCnt = arraySpeciesKey.GetSize();

	m_strSpeciesCnt.Format(_T("%d"),nSpeciesCnt);
	m_pCM->SetControlMgrData(_T("����"),m_strSpeciesCnt);

	m_strBookCnt.Format(_T("%d"),nRowCntBook);
	m_pCM->SetControlMgrData(_T("å��"),m_strBookCnt);

	m_strTotalAccount.Format(_T("%d"),nPriceSum);
	m_pCM->SetControlMgrData(_T("�����갡"),m_strTotalAccount);

}

BOOL CSeAccountStatement::CheckIsHave(CStringArray *codeValue, CString code)
{

	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;

}

VOID CSeAccountStatement::OnbtnACCPRINT() 
{
	// TODO: Add your control notification handler code here
	// ���
	INT nRowCnt = m_pDM_Book->GetRowCount();
	INT nCurSel = m_ctrlTab.GetCurSel();
	INT ids = -1;

	CESL_DataMgr * pDM_PRINT;
	CESL_Grid * pGrid;

	pDM_PRINT = new CESL_DataMgr();
	pDM_PRINT->SetCONNECTION_INFO(m_pDM_Book->CONNECTION_INFO);
	

	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("����� �ڷᰡ �����ϴ�!"));
		return;
	}

	if(0 == nCurSel)
	{
		m_pGrid_Book->SelectMakeList();
		if (m_pGrid_Book->SelectGetCount() <= 0) 
		{
			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return;
		}
		MakeCopyDM(m_pDM_Book, pDM_PRINT);
		pDM_PRINT->CopyDM( m_pDM_Book );
		pGrid = m_pGrid_Book;
	}
	else
	{
		m_pGrid_Species->SelectMakeList();
		if (m_pGrid_Species->SelectGetCount() <= 0) 
		{
			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return;
		}
		MakeCopyDM(m_pDM_Species, pDM_PRINT);
		pDM_PRINT->CopyDM( m_pDM_Species );
		pGrid = m_pGrid_Species;
	}

	INT index = pGrid->SelectGetTailPosition();
	INT nCnt = pDM_PRINT->GetRowCount()-1;

	for( INT i = nCnt; i >= 0 ; i-- )
	{
		if( i != index )
			ids = pDM_PRINT->DeleteRow(i);
		else
			index = pGrid->SelectGetPrev();

	}

	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	
	if(0 == nCurSel) //å
	{
		pSIReportManager->SetCONNECTION_INFO(m_pDM_Book->CONNECTION_INFO);
		ids = pSIReportManager->GetSIReportFileFromDB(_T("����å��������"));
	}
	else //��
	{
		pSIReportManager->SetCONNECTION_INFO(m_pDM_Species->CONNECTION_INFO);
		ids = pSIReportManager->GetSIReportFileFromDB(_T("��������������"));
	}
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return ;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;

/*	if(0 == nCurSel) //å
		pSIReportManager->SetDataMgr(0, m_pDM_Book, 0);
	else //��
		pSIReportManager->SetDataMgr(0, m_pDM_Species, 0);
*/
	pSIReportManager->SetDataMgr(0, pDM_PRINT, 0);
	
	pSIReportManager->SetTempVariable(_T("����"),m_strSpeciesCnt);
	pSIReportManager->SetTempVariable(_T("å��"),m_strBookCnt);
	pSIReportManager->SetTempVariable(_T("�����갡"),m_strTotalAccount);
	pSIReportManager->SetTempVariable(_T("�μ�"),m_strDepartMentPrint);
	pSIReportManager->SetTempVariable(_T("�Լ�������"),m_strAcqExpDatePrint);


	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
}

INT CSeAccountStatement::MakeCopyDM(CESL_DataMgr * pSrcDM, CESL_DataMgr * pDstDM)
{
	INT ids = -1;

	pDstDM->TBL_NAME = pSrcDM->TBL_NAME;
	pDstDM->EXTRA_CONDITION = pSrcDM->EXTRA_CONDITION;
	pDstDM->CONDITION = pSrcDM->CONDITION;
	pDstDM->DB_MGR_ALIAS = pSrcDM->DB_MGR_ALIAS;
	

	pDstDM->InitDataMgrRef( pSrcDM->RefList.GetCount() );
	pDstDM->m_nCols = pSrcDM->RefList.GetCount();

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < pSrcDM->RefList.GetCount() ; k++ )
	{
		sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = pSrcDM->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) return -1;

		ids = pDstDM->SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
		if( ids < 0 ) return -1;
	}
	return 0;
}

HBRUSH CSeAccountStatement::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
