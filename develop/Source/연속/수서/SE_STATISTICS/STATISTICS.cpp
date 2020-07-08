// STATISTICS.cpp : implementation file
//

#include "stdafx.h"
#include "STATISTICS.h"
#include "StatisticsCondition.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTATISTICS dialog


CSTATISTICS::CSTATISTICS(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSTATISTICS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSTATISTICS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM				= NULL;
	m_pGrid				= NULL;
	m_strCondition		= _T("");
	m_strDate1			= _T("");
	m_strDate2			= _T("");
	m_strInputDate1		= _T("");
	m_strInputDate2		= _T("");
	m_pDM_SUB			= NULL;
	m_pGridMerge		= NULL;
	m_strClass			= _T("");
}

CSTATISTICS::~CSTATISTICS()
{
	if(m_pGridMerge) 
	{
		delete m_pGridMerge;
		m_pGridMerge = NULL;
	}
}


VOID CSTATISTICS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTATISTICS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTATISTICS, CDialog)
	//{{AFX_MSG_MAP(CSTATISTICS)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTATISTICS message handlers
BOOL CSTATISTICS::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


BOOL CSTATISTICS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("����_����_���") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_���") );
		return false;
	}

	CESL_ControlMgr* pCM = FindCM(_T("CM_����_����_���"));
	m_pGrid = (CESL_Grid*)pCM->FindControl(_T("�׸���"));
	m_pDM = FindDM(_T("DM_����_����_���"));
	
	if(!m_pDM || !m_pGrid) {
		AfxMessageBox( _T("DM,Grid Error : ����_����_���") );
		return false;
	}
	
	m_pGrid->ShowWindow(FALSE);

	m_pGridMerge = new CESL_Grid(NULL);
	m_pGridMerge->SubclassDlgItem(IDC_gridSTC,this);

	InitMergeGrid();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSTATISTICS::InitMergeGrid()
{
	//-----------------------------------------------------------------------
	// 2005-12-09 hjy : �׸��� ���� ����
	//-----------------------------------------------------------------------
	INT nCols = 5;
	m_pGridMerge->SetCols(0,nCols);
	for(INT i = 0 ; i < nCols ; i++ )
	{
		if( i == 0 )
			m_pGridMerge->SetColWidth( i , 0 , 2500);
		else
		{
			m_pGridMerge->SetColWidth( i , 0 , 3000);
			m_pGridMerge->SetTextMatrix( 1 , i , _T("      ��/      �� /      å") );
		}
		
		m_pGridMerge->SetRow(0);
		m_pGridMerge->SetCol(i);
		m_pGridMerge->SetCellFontBold(TRUE);
		m_pGridMerge->SetCellAlignment(4);
		m_pGridMerge->SetCellFontBold(TRUE);
	}
	
	m_pGridMerge->SetTextMatrix( 0 , 0 , _T("��    ��") );
	m_pGridMerge->SetTextMatrix( 0 , 1 , _T("��    ��") );
	m_pGridMerge->SetTextMatrix( 0 , 2 , _T("��    ��") );
	m_pGridMerge->SetTextMatrix( 0 , 3 , _T("��    Ÿ") );
	m_pGridMerge->SetTextMatrix( 0 , 4 , _T("   ��   ") );
/*	
	m_pGridMerge->SetCols(  0 , 10 );
	m_pGridMerge->SetMergeCells(1);
	m_pGridMerge->SetMergeRow(0,TRUE);
	
	
	m_pGridMerge->SetRow(0);
	m_pGridMerge->SetCol(0);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetColWidth( 0 , 0 , 2500);
	m_pGridMerge->SetTextMatrix( 0 , 0 , _T("����") );

	
	//merge cell
	m_pGridMerge->SetRow(0);
	m_pGridMerge->SetCol(1);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);	
	m_pGridMerge->SetTextMatrix( 0 , 1 , _T("����") );
	
	
	m_pGridMerge->SetRow(0);
	m_pGridMerge->SetCol(2);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 0 , 2 , _T("����") );

	
	m_pGridMerge->SetRow(0);
	m_pGridMerge->SetCol(3);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 0 , 3 , _T("����") );
	
	m_pGridMerge->SetRow(0);
	m_pGridMerge->SetCol(4);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);	
	m_pGridMerge->SetTextMatrix( 0 , 4 , _T("����") );
	
	
	m_pGridMerge->SetRow(0);
	m_pGridMerge->SetCol(5);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 0 , 5 , _T("����") );

	
	m_pGridMerge->SetRow(0);
	m_pGridMerge->SetCol(6);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 0 , 6 , _T("����") );


	m_pGridMerge->SetRow(0);
	m_pGridMerge->SetCol(7);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);	
	m_pGridMerge->SetTextMatrix( 0 , 7 , _T("��") );
	
	
	m_pGridMerge->SetRow(0);
	m_pGridMerge->SetCol(8);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 0 , 8 , _T("��") );

	
	m_pGridMerge->SetRow(0);
	m_pGridMerge->SetCol(9);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 0 , 9 , _T("��") );

	
	// no merge cell
	m_pGridMerge->SetRow(1);
	m_pGridMerge->SetCol(1);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 1 , 1 , _T("��") );

	
	m_pGridMerge->SetRow(1);
	m_pGridMerge->SetCol(2);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 1 , 2 , _T("��") );

	
	m_pGridMerge->SetRow(1);
	m_pGridMerge->SetCol(3);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 1 , 3 , _T("å") );

	m_pGridMerge->SetRow(1);
	m_pGridMerge->SetCol(4);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 1 , 4 , _T("��") );

	
	m_pGridMerge->SetRow(1);
	m_pGridMerge->SetCol(5);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 1 , 5 , _T("��") );

	
	m_pGridMerge->SetRow(1);
	m_pGridMerge->SetCol(6);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 1 , 6 , _T("å") );

	m_pGridMerge->SetRow(1);
	m_pGridMerge->SetCol(7);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 1 , 7 , _T("��") );

	
	m_pGridMerge->SetRow(1);
	m_pGridMerge->SetCol(8);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 1 , 8 , _T("��") );

	
	m_pGridMerge->SetRow(1);
	m_pGridMerge->SetCol(9);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 1 , 9 , _T("å") );
*/
}

VOID CSTATISTICS::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridSTATISTICS);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);	


	pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridSTC);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);	

}

VOID CSTATISTICS::SetConditionInfo()
{
	//���� ���������� Y�� ȭ���� �����ش�

	CStatisticsCondition dlg(this);
	
	if(dlg.DoModal() != IDOK) return;
	
	CString msg;
	
	m_strCondition = dlg.m_strCondition;
	m_strDate1 = dlg.m_strDate1;
	m_strDate2 = dlg.m_strDate2;
	m_strInputDate1 = dlg.m_strInputDate1;
	m_strInputDate2 = dlg.m_strInputDate2;

	msg.Format(_T("[����: %s] [�Լ�����: %s ~ %s ����][�Է�����: %s ~ %s]"),
		m_strCondition,m_strDate1,m_strDate2,m_strInputDate1, m_strInputDate2);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)msg.GetBuffer(0));
	
	m_pDM->FreeData();

	if(dlg.m_bIsDirectPrint)
	{
		SetVerticalCoordinate(FALSE);
		MakeStatistics();
		PrintData();
	}
	else
	{
		SetVerticalCoordinate();
	}

}

VOID CSTATISTICS::SetVerticalCoordinate(BOOL bDisplayGrid)
{
	if(m_strCondition.IsEmpty())
	{
		AfxMessageBox(_T("���Ǽ����� �Ǿ����� �ʽ��ϴ�!"));
		return;
	}

	if(_T("�����") == m_strCondition) {
		m_strClass = _T("������ڵ�");
		m_pDM_SUB = FindDM(_T("DM_����_����_���_�����"));

	}
	else if(_T("�ڷᱸ��") == m_strCondition) {
		m_strClass = _T("�ڷᱸ��");
		m_pDM_SUB = FindDM(_T("DM_����_����_���_�ڷᱸ��"));

	}
	else if(_T("��ϱ���") == m_strCondition) {
		m_strClass = _T("��ϱ���");
		m_pDM_SUB = FindDM(_T("DM_����_����_���_��ϱ���"));

	}
	else if(_T("�ڷ�Ǳ���") == m_strCondition) {
		m_strClass = _T("�ڷ�Ǳ���");
		m_pDM_SUB = FindDM(_T("DM_����_����_���_�ڷ��"));

	}
	else if(_T("������ϱ���") == m_strCondition) {
		m_strClass = _T("������ϱ���");
		m_pDM_SUB = FindDM(_T("DM_����_����_���_������ϱ���"));
	}
	else{	// ����� �����ڵ屸���� ���� ���� ���..
		CString sClass,sQuery;
			
		sQuery.Format(_T("SELECT CLASS_CODE FROM MN_USER_DEFINE_TBL WHERE CLASS_ALIAS = '%s'"),m_strCondition);
		m_pDM->GetOneValueQuery(sQuery, sClass);
	
		if(sClass == _T("103")) 
		{
			m_pDM_SUB = FindDM(_T("DM_����_����_���Ӻз��ڵ�1"));
			m_strClass = _T("���Ӻз��ڵ�1");
		}
		else if(sClass == _T("101"))
		{
			m_pDM_SUB = FindDM(_T("DM_����_����_���Ӻз��ڵ�2"));
			m_strClass = _T("���Ӻз��ڵ�2");
		}
		else if(sClass == _T("102"))
		{
			m_pDM_SUB = FindDM(_T("DM_����_����_���Ӻз��ڵ�3"));
			m_strClass = _T("���Ӻз��ڵ�3");
		}	
	}

	if(!m_pDM_SUB) {
		
		AfxMessageBox(_T("������ �����͸� ã�� �� �����ϴ�!"));
		return;
	}
	
	m_pDM_SUB->FreeData();

	INT nCode = 256;
	INT nDefaultValue = 0;
	CString strCode[256], strDesc[256];
	nDefaultValue = 0;

	
	m_pInfo->pCodeMgr->GetCode(m_strClass, strCode, strDesc, nCode, nDefaultValue);	
	INT nCodeCnt = m_pInfo->pCodeMgr->GetCodeCount(m_strClass);

	
	INT idx = 0;

	for(INT i=0;i<nCodeCnt;i++)
	{

		m_pDM->InsertRow(-1);
		idx = m_pDM->GetRowCount()-1;
		m_pDM->SetCellData(_T("UDF_��"),strDesc[i],idx);
		m_pDM->SetCellData(_T("�ڵ�"),strCode[i],idx);
	}
	
	m_pDM->InsertRow(-1);
	idx = m_pDM->GetRowCount()-1;
	m_pDM->SetCellData(_T("UDF_��"),_T("��"),idx);
	
	if(bDisplayGrid) GridDisplay(FALSE);
}

VOID CSTATISTICS::MakeStatistics(BOOL bDisplayGrid)
{
	if(m_strCondition.IsEmpty()) 
	{
		AfxMessageBox(_T("���� ������ ���� ���ֽʽÿ�!"));
		return;
	}

	CString strQuery = _T("");

	if (m_strCondition == _T("���ӱ���"))
		MakeSubStatistics(m_strClass);
	else
		MakeSubStatistics(m_strCondition);
 

	INT nValue;
	INT nSum;
	CString tmp;
	CString alias;

	INT nRowCnt = m_pDM->GetRowCount();
	
	//�հ� ó��
	// 2005-04-11 �հ�κ��� ������ ���� ���� �հ踸 ���´�.
	/*
	for(INT i=0;i<9;i++)
	{
		nSum = 0;
		alias.Format(_T("%d"),i+1);
		
		for(INT j=0;j<nRowCnt-1;j++)
		{
			tmp = _T("");
			tmp = m_pDM->GetCellData(alias,j);
			nValue = _ttoi(tmp.GetBuffer(0));
			nSum += nValue;
		}
		
		tmp.Format(_T("%d"),nSum);
		m_pDM->SetCellData(alias,tmp,nRowCnt-1);
	}
	*/

/*	// BEGIN:
	CString strResult;
	// 1. ����-���հ� ������
	strQuery.Format(_T("select count(distinct(i.rec_key)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_PURCHASE_MANAGE_TBL P ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY "));
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("1"), strResult, nRowCnt-1);

	// 2.����-���հ� ������
	strQuery.Format(_T("select count(distinct(v.rec_key)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_PURCHASE_MANAGE_TBL P, se_vol_tbl v, se_book_tbl b ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY and i.rec_key = v.specieS_key and v.rec_key = b.vol_key and ")
					_T("b.acq_code = '1' and b.book_type = 'C'"));
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("2"), strResult, nRowCnt-1);

	// 3. ����-���հ� ������
	strQuery.Format(_T("select count(distinct(i.rec_key)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_DONATE_MANAGE_TBL D ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND I.REC_KEY = D.SPECIES_KEY"));
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("4"), strResult, nRowCnt-1);

	// 4. ����-���հ� ������
	strQuery.Format(_T("select count(distinct(v.rec_key)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_DONATE_MANAGE_TBL D, se_vol_tbl v, se_book_tbl b ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND I.REC_KEY = D.SPECIES_KEY and i.rec_key = v.species_key and v.rec_key = b.vol_key and ")
					_T("b.acq_code = '2' and b.book_type = 'C'"));
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("5"), strResult, nRowCnt-1);

	// 5. ��ü-���հ� ������
	strQuery.Format(_T("select count(distinct(i.rec_key)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S ")
					_T("WHERE I.REC_KEY = S.REC_KEY")); 	
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("7"), strResult, nRowCnt-1);

	// 6. ��ü-���հ� ������
	strQuery.Format(_T("select count(distinct(v.rec_key)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, se_vol_tbl v, se_book_tbl b ")
					_T("WHERE I.REC_KEY = S.REC_KEY and i.rec_key = v.species_key and v.rec_key = b.vol_key and ")
					_T("b.book_type = 'C'"));
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("8"), strResult, nRowCnt-1);
*/
	//�Լ����� �Ⱓ����
	CString sTemp, sQuery;
	sTemp = MakeRangeQuery(_T("b.acq_date"),m_strDate1,m_strDate2);
	
	if(!sTemp.IsEmpty())
	{
		sQuery += _T(" and ");
		sQuery += sTemp;
	}
	
	sTemp = _T("");
	
	//�Է����� �Ⱓ����
	sTemp = MakeRangeQuery(_T("s.input_date"),m_strInputDate1,m_strInputDate2);
	
	if(!sTemp.IsEmpty())
	{
		sQuery += _T(" and ");
		sQuery += sTemp;
	}

	// BEGIN:
	CString strResult;
	// 1. ����-���հ� ������
	strQuery.Format(_T("SELECT COUNT(DISTINCT(I.REC_KEY)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_PURCHASE_MANAGE_TBL P, SE_BOOK_TBL B ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND ")
					_T("I.REC_KEY = B.SPECIES_KEY AND ")
					_T("B.ACQ_CODE = '1' AND B.BOOK_TYPE = 'C' AND ")
					_T("I.REC_KEY = P.SPECIES_KEY %s AND B.MANAGE_CODE=UDF_MANAGE_CODE"), sQuery);
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("1"), strResult, nRowCnt-1);

	// 2.����-���հ� ������
	strQuery.Format(_T("SELECT COUNT (DISTINCT(V.REC_KEY)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_PURCHASE_MANAGE_TBL P, SE_VOL_TBL V, SE_BOOK_TBL B ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY and I.REC_KEY = V.SPECIES_KEY AND V.REC_KEY = B.VOL_KEY AND ")
					_T("B.ACQ_CODE = '1' AND B.BOOK_TYPE = 'C' %s  AND B.MANAGE_CODE=UDF_MANAGE_CODE")
					,sQuery);
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("2"), strResult, nRowCnt-1);

	// 3. ����-���հ� ������
	strQuery.Format(_T("SELECT COUNT(DISTINCT(I.REC_KEY)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_DONATE_MANAGE_TBL D, SE_BOOK_TBL B ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND ")
					_T("B.ACQ_CODE = '2' AND B.BOOK_TYPE = 'C' AND ")
					_T("B.SPECIES_KEY = I.REC_KEY AND ")
					_T("D.SPECIES_KEY = I.REC_KEY %s  AND B.MANAGE_CODE=UDF_MANAGE_CODE")
					, sQuery);
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("4"), strResult, nRowCnt-1);

	// 4. ����-���հ� ������
	strQuery.Format(_T("SELECT COUNT(DISTINCT(V.REC_KEY)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_DONATE_MANAGE_TBL D, SE_VOL_TBL V, SE_BOOK_TBL B ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND I.REC_KEY = D.SPECIES_KEY and I.REC_KEY = V.SPECIES_KEY AND V.REC_KEY = B.VOL_KEY AND ")
					_T("B.ACQ_CODE = '2' AND B.BOOK_TYPE = 'C' %s  AND B.MANAGE_CODE=UDF_MANAGE_CODE")
					, sQuery);
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("5"), strResult, nRowCnt-1);
	
	// 5. ��Ÿ-���հ� ������
	strQuery.Format(_T("SELECT COUNT(DISTINCT(I.REC_KEY)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_BOOK_TBL B ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND ")
					_T("B.ACQ_CODE IS NULL AND ")
					_T("B.BOOK_TYPE = 'C' AND ")
					_T("B.SPECIES_KEY = I.REC_KEY %s  AND B.MANAGE_CODE=UDF_MANAGE_CODE")
					//_T("D.SPECIES_KEY = I.REC_KEY %s")
					, sQuery);
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("7"), strResult, nRowCnt-1);

	// 6. ��Ÿ-���հ� ������
	strQuery.Format(_T("SELECT COUNT(DISTINCT(V.REC_KEY)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_VOL_TBL V, SE_BOOK_TBL B ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND I.REC_KEY = V.SPECIES_KEY AND V.REC_KEY = B.VOL_KEY AND ")
					_T("B.ACQ_CODE IS NULL AND B.BOOK_TYPE = 'C' %s  AND B.MANAGE_CODE=UDF_MANAGE_CODE")
					, sQuery);
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("8"), strResult, nRowCnt-1);

	// 5. ��ü-���հ� ������
	strQuery.Format(_T("SELECT COUNT(DISTINCT(I.REC_KEY)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_BOOK_TBL B ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND ")
					_T("I.REC_KEY = B.SPECIES_KEY AND ")
					_T("B.ACQ_CODE IS NOT NULL AND B.BOOK_TYPE = 'C' %s  AND B.MANAGE_CODE=UDF_MANAGE_CODE")
					, sQuery); 	
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("10"), strResult, nRowCnt-1);

	// 6. ��ü-���հ� ������
	strQuery.Format(_T("SELECT COUNT(DISTINCT(V.REC_KEY)) ")
					_T("FROM IDX_SE_TBL I, SE_SPECIES_TBL S, SE_VOL_TBL V, SE_BOOK_TBL B ")
					_T("WHERE I.REC_KEY = S.REC_KEY AND I.REC_KEY = V.SPECIES_KEY AND V.REC_KEY = B.VOL_KEY AND ")
					_T("B.ACQ_CODE IS NOT NULL AND B.BOOK_TYPE = 'C' %s  AND B.MANAGE_CODE=UDF_MANAGE_CODE")
					, sQuery);
	m_pDM->GetOneValueQuery(strQuery, strResult);
	m_pDM->SetCellData(_T("11"), strResult, nRowCnt-1);
	// 7. ����-å��,����-å��,��ü-å�� ������
	for(INT i=0;i<4;i++)
	{
		nSum = 0;
		alias.Format(_T("%d"),(i+1)*3);
		
		for(INT j=0;j<nRowCnt-1;j++)
		{
			tmp = _T("");
			tmp = m_pDM->GetCellData(alias,j);
			nValue = _ttoi(tmp.GetBuffer(0));
			nSum += nValue;
		}
		
		tmp.Format(_T("%d"),nSum);
		m_pDM->SetCellData(alias,tmp,nRowCnt-1);
	}
	// END:

	if(bDisplayGrid) GridDisplay();
}

VOID CSTATISTICS::MakeSubStatistics(CString strClass)
{
	//class�� ��� ó��
	CString strQuery = _T("");
	CString tmp = _T("");
	CString strtmpclass;
	CString strUdf;
	CString strSpeciesCnt,strVolCnt,strBookCnt;


	//����
	strQuery = _T("i.rec_key = p.species_key and i.rec_key = s.rec_key and i.rec_key = b.species_key and v.rec_key = b.vol_key and b.acq_code = '1' and i.rec_key = v.species_key and b.book_type = 'C' AND B.MANAGE_CODE=UDF_MANAGE_CODE");
	
	//�Լ����� ���� 
	tmp = MakeRangeQuery(_T("b.acq_date"),m_strDate1,m_strDate2);
	
	if(!tmp.IsEmpty())
	{
		strQuery += _T(" and ");
		strQuery += tmp;
	}

	tmp = _T("");
	//�Է����� �����߰�
	tmp = MakeRangeQuery(_T("s.input_date"),m_strInputDate1,m_strInputDate2);
	
	if(!tmp.IsEmpty())
	{
		strQuery += _T(" and ");
		strQuery += tmp;
	}


	m_pDM_SUB->TBL_NAME = _T("idx_se_tbl i, se_species_tbl s ,se_purchase_manage_tbl p, se_vol_tbl v,se_book_tbl b");

	m_pDM_SUB->RefreshDataManager(strQuery);

	for(INT i=0;i<m_pDM_SUB->GetRowCount();i++)
	{
		strtmpclass = _T("");
		strtmpclass = m_pDM_SUB->GetCellData(strClass,i);

		for(INT j=0;j<m_pDM->GetRowCount();j++)
		{
			strUdf = _T("");
			strUdf = m_pDM->GetCellData(_T("�ڵ�"),j);
			
			if(strtmpclass == strUdf)
			{
				strSpeciesCnt = m_pDM_SUB->GetCellData(_T("��"),i);
				strVolCnt = m_pDM_SUB->GetCellData(_T("��"),i);
				strBookCnt = m_pDM_SUB->GetCellData(_T("å"),i);

				m_pDM->SetCellData(_T("1"),strSpeciesCnt,j);
				m_pDM->SetCellData(_T("2"),strVolCnt,j);
				m_pDM->SetCellData(_T("3"),strBookCnt,j);
				break;
			}

		}
	
	}
	

	//����
	strQuery = _T("i.rec_key = d.species_key and i.rec_key = s.rec_key and i.rec_key = b.species_key and v.rec_key = b.vol_key and b.acq_code = '2' and i.rec_key = v.species_key and b.book_type = 'C' AND B.MANAGE_CODE=UDF_MANAGE_CODE");
	
	tmp = MakeRangeQuery(_T("b.acq_date"),m_strDate1,m_strDate2);
	
	if(!tmp.IsEmpty())
	{
		strQuery += _T(" and ");
		strQuery += tmp;
	}

	tmp = _T("");
	tmp = MakeRangeQuery(_T("s.input_date"),m_strInputDate1,m_strInputDate2);
	
	if(!tmp.IsEmpty())
	{
		strQuery += _T(" and ");
		strQuery += tmp;
	}

	m_pDM_SUB->TBL_NAME = _T("idx_se_tbl i, se_species_tbl s, se_donate_manage_tbl d, se_vol_tbl v,se_book_tbl b");

	m_pDM_SUB->RefreshDataManager(strQuery);

	for(i=0;i<m_pDM_SUB->GetRowCount();i++)
	{
		strtmpclass = _T("");
		strtmpclass = m_pDM_SUB->GetCellData(strClass,i);

		for(INT j=0;j<m_pDM->GetRowCount();j++)
		{
			strUdf = _T("");
			strUdf = m_pDM->GetCellData(_T("�ڵ�"),j);
			
			if(strtmpclass == strUdf)
			{
				strSpeciesCnt = m_pDM_SUB->GetCellData(_T("��"),i);
				strVolCnt = m_pDM_SUB->GetCellData(_T("��"),i);
				strBookCnt = m_pDM_SUB->GetCellData(_T("å"),i);

				m_pDM->SetCellData(_T("4"),strSpeciesCnt,j);
				m_pDM->SetCellData(_T("5"),strVolCnt,j);
				m_pDM->SetCellData(_T("6"),strBookCnt,j);
				break;
			}

		}
	
	}
	
	//��Ÿ
	strQuery = _T("i.rec_key = b.species_key and i.rec_key = s.rec_key and v.rec_key = b.vol_key and b.acq_code is null and i.rec_key = v.species_key and b.book_type = 'C' AND B.MANAGE_CODE=UDF_MANAGE_CODE");
	
	tmp = MakeRangeQuery(_T("b.acq_date"),m_strDate1,m_strDate2);
	
	if(!tmp.IsEmpty())
	{
		strQuery += _T(" and ");
		strQuery += tmp;
	}

	tmp = MakeRangeQuery(_T("s.input_date"),m_strInputDate1,m_strInputDate2);
	
	if(!tmp.IsEmpty())
	{
		strQuery += _T(" and ");
		strQuery += tmp;
	}

	m_pDM_SUB->TBL_NAME = _T("idx_se_tbl i, se_species_tbl s, se_vol_tbl v,se_book_tbl b");

	m_pDM_SUB->RefreshDataManager(strQuery);

	for(i=0;i<m_pDM_SUB->GetRowCount();i++)
	{
		strtmpclass = _T("");
		strtmpclass = m_pDM_SUB->GetCellData(strClass,i);

		for(INT j=0;j<m_pDM->GetRowCount();j++)
		{
			strUdf = _T("");
			strUdf = m_pDM->GetCellData(_T("�ڵ�"),j);
			
			if(strtmpclass == strUdf)
			{
				strSpeciesCnt = m_pDM_SUB->GetCellData(_T("��"),i);
				strVolCnt = m_pDM_SUB->GetCellData(_T("��"),i);
				strBookCnt = m_pDM_SUB->GetCellData(_T("å"),i);

				m_pDM->SetCellData(_T("7"),strSpeciesCnt,j);
				m_pDM->SetCellData(_T("8"),strVolCnt,j);
				m_pDM->SetCellData(_T("9"),strBookCnt,j);
				break;
			}
		}
	}

	//��ü
	strQuery = _T("i.rec_key = b.species_key and i.rec_key = s.rec_key and v.rec_key = b.vol_key and i.rec_key = v.species_key and b.book_type = 'C' AND B.MANAGE_CODE=UDF_MANAGE_CODE");
	
	tmp = MakeRangeQuery(_T("b.acq_date"),m_strDate1,m_strDate2);
	
	if(!tmp.IsEmpty())
	{
		strQuery += _T(" and ");
		strQuery += tmp;
	}

	tmp = _T("");

	tmp = MakeRangeQuery(_T("s.input_date"),m_strInputDate1,m_strInputDate2);
	
	if(!tmp.IsEmpty())
	{
		strQuery += _T(" and ");
		strQuery += tmp;
	}

	m_pDM_SUB->TBL_NAME = _T("idx_se_tbl i, se_species_tbl s, se_vol_tbl v,se_book_tbl b");

	m_pDM_SUB->RefreshDataManager(strQuery);

	for(i=0;i<m_pDM_SUB->GetRowCount();i++)
	{
		strtmpclass = _T("");
		strtmpclass = m_pDM_SUB->GetCellData(strClass,i);

		for(INT j=0;j<m_pDM->GetRowCount();j++)
		{
			strUdf = _T("");
			strUdf = m_pDM->GetCellData(_T("�ڵ�"),j);
			
			if(strtmpclass == strUdf)
			{
				strSpeciesCnt = m_pDM_SUB->GetCellData(_T("��"),i);
				strVolCnt = m_pDM_SUB->GetCellData(_T("��"),i);
				strBookCnt = m_pDM_SUB->GetCellData(_T("å"),i);

				m_pDM->SetCellData(_T("10"),strSpeciesCnt,j);
				m_pDM->SetCellData(_T("11"),strVolCnt,j);
				m_pDM->SetCellData(_T("12"),strBookCnt,j);
				break;
			}

		}
	
	}

}


CString CSTATISTICS::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData2.IsEmpty()) nOption = 2;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s >= '%s'"),strFieldName,strData1);
		if(nOption == 2) tmpquery.Format(_T("%s <= '%s'"),strFieldName,strData2);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

VOID CSTATISTICS::GridDisplay(BOOL bDataShow)
{
	if(m_strCondition.IsEmpty()) return;

	m_pGridMerge->SetRow(1);
	m_pGridMerge->SetCol(0);
	m_pGridMerge->SetCellAlignment(4);
	m_pGridMerge->SetCellFontBold(TRUE);
	m_pGridMerge->SetTextMatrix( 1 , 0 , m_strCondition );


	INT nRow,nGridRow;

	nRow = m_pDM->GetRowCount();
	nGridRow = nRow+2;

	m_pGridMerge->SetRows(nGridRow);
	
	//��(����) ����
	CString tmp;

	for(INT i=0;i<nRow;i++)
	{
		tmp = _T("");
		tmp = m_pDM->GetCellData(_T("UDF_��"),i);
		m_pGridMerge->SetRow(i+2);
		m_pGridMerge->SetCol(0);	
		m_pGridMerge->SetCellAlignment(1);
		m_pGridMerge->SetCellFontBold(TRUE);
		m_pGridMerge->SetTextMatrix( i+2 , 0 , tmp );		
	}

	CString strAlias, strTotal;	
	
	if(bDataShow) //�Ϲݵ����� �� �����ֱ� 
	{
		for(i=0;i<nRow;i++)
		{
			bool bEmptyRow = TRUE;
			for(INT j=0;j<12;j++)
			{
				strAlias.Format(_T("%d"),j+1);

				tmp = _T("");
				tmp = m_pDM->GetCellData(strAlias,i);
				if((j+1)%3 == 0)
				{
					strTotal.Format(_T("%7s / %7s / %7s"),m_pDM->GetCellData(i,j-1), m_pDM->GetCellData(i,j), tmp);
					if( strTotal != _T("      0 /       0 /       0") )
						bEmptyRow = FALSE;
					m_pGridMerge->SetTextMatrix( i+2 , (j+1)/4+1 , strTotal );		
				}
				//m_pGridMerge->SetTextMatrix( i+2 , j+1 , tmp );	
			}
			if( bEmptyRow == TRUE && i != nRow-1 ) // ��谪�� ���� �ο�� ����
			{
				m_pDM->DeleteRow(i);
				m_pGridMerge->RemoveAt(i+1);
				i -= 1;
				nRow -= 1;
			}
		}
	}
	else //�Ϲ� ������ ����� 
	{
		
		tmp = _T("");

		for(i=0;i<nRow;i++)
		{
	
			for(INT j=0;j<4;j++)
			{
				strAlias.Format(_T("%d"),j+1);
				m_pGridMerge->SetTextMatrix( i+2 , j+1 , tmp );	
			}
		}
	}

}

VOID CSTATISTICS::PrintData()
{
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	
	/*
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		ESLAfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return ;
	}
	*/
	
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("���Ӽ������"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return ;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	//pSIReportManager->m_pWorkForm->paper = PAPER_A4_PORTRAIT;
	pSIReportManager->SetDataMgr(0, m_pDM, 0);
	pSIReportManager->SetTempVariable(_T("����"),m_strCondition);
	pSIReportManager->SetTempVariable(_T("������"),m_strDate1);
	pSIReportManager->SetTempVariable(_T("������"),m_strDate2);


	ids = pSIReportManager->MakeXMLPageFiles();
	// ������ ������ �׸�
	Print_SetLastPageLine();
	ids = pSIReportManager->Print2();

	//ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
}

VOID CSTATISTICS::Print_SetLastPageLine(VOID)
{
	INT ids;

	// XML �� WorkForm��ü�� �����Ѵ�.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) {
		AfxMessageBox(_T("XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}
	if (pPageWorkForm == NULL) {
		AfxMessageBox(_T("�߰� XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}

	// ���� ������ ������ ��ȣ�� �����´�.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) {
		AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�."));
		return;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// ���� ������ ������ ������ �ҷ��´�.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) {
		AfxMessageBox(_T("�������� �ε� ���� ���Ͽ����ϴ�."));
		return;
	}
	// �ҷ��� WorkForm�߿��� First Sheet�� �����´�.(�߰� ������ ���Ͽ��� Sheet�� �Ѱ����̴�.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) {
		AfxMessageBox(_T("�������� ã�� �� �����ϴ�."));
		return;
	}
	// ������ ������ ó���� �̹� �Ǿ� �ִ��� ����. �̹� ó���Ǿ� �ִٸ� �ι� �� �ʿ� ����.
	CSIReportObject * pObject = pSheet->FindObject(_T("�߰��ȸ�����������"));
	if (pObject) return;
	// �ܺ����̺� �� �ݺ� ���̺��� �����´�.(���̺� �̸��� ��Ȳ�� ���� ������ ��)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("CHILD_TBL"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("PARENT_TBL"));
	if (pIn == NULL) return;
	if (pOut == NULL) return;

	CRect InLeft, InRight, OutLeft, OutRight, NewRect;

	CSIReportObject_Table_Cell *pCell;

	// �ݺ����̺��� ������ ������ ���� ó���� ������ ���������� Rect�� ���ϰ�
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, 0);
	if (pCell) InLeft = pCell->GetRect();
	
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, pIn->col_count-1);
	if (pCell) InRight = pCell->GetRect();

	// �ܺ����̺��� ������ ������ ���� ó�� ���� Rect�� ���Ѵ�.
	pCell = NULL;
	pCell = pOut->GetCell(pOut->row_count-1, 0);
	if (pCell) OutLeft = pCell->GetRect();

	// �ܺ����̺��� �ݺ����̺��� ������ ������ �������� �߰����ش�.
	if (InLeft.bottom < OutLeft.bottom) {
		// ������ �������� Rect�� ����Ѵ�.
		NewRect.top = InLeft.bottom;
		NewRect.bottom = InLeft.bottom;
		NewRect.left = InLeft.left;
		NewRect.right = InRight.right;

		// ������ �������� �߰� ������ ���Ͽ� �߰��Ѵ�.
		CSIReportObject_Line *pLine = (CSIReportObject_Line *)pSheet->AddNewObject(OBJECT_LINE);
		if (pLine) {
			pLine->SetName(_T("�߰��ȸ�����������"));
			pLine->SetRect(NewRect);
		}

		// ����� �߰������� ������ �����Ѵ�.
		ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	}

	// ������ Object�� ����
	if (pXMLIOMgr) {
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	if (pPageWorkForm) {
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}
	return;

}

HBRUSH CSTATISTICS::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
