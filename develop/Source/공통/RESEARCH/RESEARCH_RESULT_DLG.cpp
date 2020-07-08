// RESEARCH_RESULT_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "RESEARCH_RESULT_DLG.h"
#include "RESEARCH_RESULT_DESCTYPE.h"
#include "..\\MSCharMgr\\MSChartMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_RESULT_DLG dialog


CRESEARCH_RESULT_DLG::CRESEARCH_RESULT_DLG(CESL_Mgr* pParent )
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CRESEARCH_RESULT_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pTabCtrl  = NULL ;
	m_pMainGrid = NULL ;


	m_pResearchInfoDM = NULL ;
	m_pQuestionInfoDM = NULL ;
	m_pQuestionDM = NULL  ;
	m_pResultDM = NULL ;

	// KOL.UDF.022 ��ť���ڵ� ����
	m_nParentGridIdx = -1;
	m_pOutputDM = NULL;



}

CRESEARCH_RESULT_DLG::~CRESEARCH_RESULT_DLG()
{
	
}

BOOL CRESEARCH_RESULT_DLG::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CRESEARCH_RESULT_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRESEARCH_RESULT_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRESEARCH_RESULT_DLG, CDialog)
	//{{AFX_MSG_MAP(CRESEARCH_RESULT_DLG)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRESEARCH_RESULT_DLG message handlers

VOID CRESEARCH_RESULT_DLG::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CRESEARCH_RESULT_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("SM_RESEARCH_RESULT"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Failed"));
		return FALSE ;
	}


	m_pTabCtrl  = (CTabCtrl *)GetDlgItem(IDC_TAB1) ;
	m_pTabCtrl->InsertItem (0,_T("�������")) ;
	m_pTabCtrl->SetCurSel (0) ;


	m_pMainGrid = NULL ;


	m_pResearchInfoDM = FindDM(_T("DM_RESEARCH_INFO")) ;
	if(m_pResearchInfoDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return -100 ;
	}
	m_pQuestionInfoDM = FindDM(_T("DM_QUESTION_INFO")) ;
	if(m_pQuestionInfoDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return -100 ;
	}
	m_pQuestionDM = FindDM(_T("DM_QUESTION"))  ;
	if(m_pQuestionDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return -100 ;
	}
	m_pResultDM = FindDM(_T("DM_RESEARCH_RESULT")) ;
	if(m_pResultDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return -100 ;
	}


	INT ids ;
	ids = DisplayScreen (0) ;

	EnableThemeDialogTexture(GetSafeHwnd());



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRESEARCH_RESULT_DLG::SetParentManager(CString strDM, CString strCM, CString strGrid)
{
	m_strParentDM   = strDM ;
	m_strParentCM   = strCM;
	m_strParentGrid = strGrid  ;
}

INT CRESEARCH_RESULT_DLG::DisplayScreen(INT ndir)
{
	//*********************************************************************************
	//DM ���� ���� ���� 

	INT  ids ;
	switch (ndir)
	{
	case 0 :                                                                         
		
		SelectMakeList(m_strParentCM ,m_strParentGrid );
		ids = SelectGetHeadPosition(m_strParentCM ,m_strParentGrid);
		if (ids < 0)
		{
			AfxMessageBox(_T("���� ����� ������ �� �����ϴ�."));	// ����
			EndDialog(IDCANCEL);
			return -222;
		}
		break;
		
	case -1 :
		
        ids = SelectGetPrev (m_strParentCM ,m_strParentGrid);
		if (ids == -1) {
			ids = SelectGetHeadPosition(m_strParentCM ,m_strParentGrid);
			AfxMessageBox(_T(" ó�� �ڷ��Դϴ�."));
			return -223;
		}
		break;
		
	case 1 :
		
		ids = SelectGetNext (m_strParentCM ,m_strParentGrid);
		if (ids == -1) {
			ids = SelectGetTailPosition(m_strParentCM ,m_strParentGrid);
			AfxMessageBox(_T(" ������ �ڷ��Դϴ�."));
			return -224;
		}
		break;
	}
	
	//INT m_Idx ;
    m_nParentGridIdx = ids; 
    SetReverse (m_strParentCM ,m_strParentGrid ,m_nParentGridIdx) ;

	CESL_DataMgr *pParentDM = FindDM(m_strParentDM );
	if(pParentDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return -100 ;
	}

	m_strResearchInfoRecKey = pParentDM->GetCellData (_T("REC_KEY"),m_nParentGridIdx) ;


	//**************************************************
	/* ������ ���� �Է��ϱ�  */
	CString strTitle = pParentDM->GetCellData (_T("TITLE"),m_nParentGridIdx) ;
	CString strEndDate = pParentDM->GetCellData (_T("END_DATE"),m_nParentGridIdx) ;

	CEdit* pEditTitle = (CEdit*)GetDlgItem(IDC_EDIT_TITLE) ;
	pEditTitle->SetWindowText (strTitle) ;
		
	CEdit* pEditEndDate = (CEdit*)GetDlgItem(IDC_EDIT_ENDDATE) ;
	pEditEndDate->SetWindowText (strEndDate) ;
	//**************************************************


	MakeDefaultDM (m_strResearchInfoRecKey) ;

	/*
	m_pResearchInfoDM = NULL ;
	m_pQuestionInfoDM = NULL ;
	m_pQuestionDM = NULL  ;
	m_pResultDM = NULL ;
    */


	MakeOutputData() ;
	MakeOutputData() ;

	return 0 ;
}

bool CRESEARCH_RESULT_DLG::MakeDefaultDM(CString strReckey)
{

	/*
	m_pResearchInfoDM = NULL ;
	m_pQuestionInfoDM = NULL ;
	m_pQuestionDM = NULL  ;
	m_pResultDM = NULL ;
    */

	CString strOption ;

	strOption.Format (_T("REC_KEY = %s"),strReckey) ;
    m_pResearchInfoDM->RefreshDataManager (strOption) ;


	strOption.Format (_T("RESEARCH_KEY = %s"),strReckey) ;
	
	m_pQuestionInfoDM ->RefreshDataManager (strOption) ;
	
	m_pQuestionDM ->RefreshDataManager (strOption) ;
	
	m_pResultDM ->RefreshDataManager (strOption) ;

	

	return TRUE ;
}

INT CRESEARCH_RESULT_DLG::MakeOutputData()
{
	/*
	select A.question_info_key,A.type,A.SELECTVALUE ,count(A.SELECTVALUE) AS ���ü� , B.SUBTITLE 
	from research_result_tbl A,RESEARCH_QUESTION_INFO_TBL B 
	where A.QUESTION_INFO_KEY = B.REC_KEY AND  A.research_key = 63247629  
	group by A.question_info_key,A.type,A.SELECTVALUE , B.SUBTITLE 
    */

	CESL_DataMgr *pDM = FindDM(_T("DM_RESEARCH_TEMP"));

	CString strOption ;
	strOption.Format (_T(" A.research_key = %s "),m_strResearchInfoRecKey );
	pDM->RefreshDataManager (strOption);

	
	CString strFind =_T("");
	CString strCompare =_T("");

	m_pOutputDM = FindDM(_T("DM_RESEARCH_RESULT_OUTPUT"));
	m_pOutputDM->FreeData () ;
	
	INT nLastCnt = -1 ;
	INT nTotalCnt = 0 ;
	CString strTotalCnt ;

	CString strCompareType ;

	for(INT i = 0 ; i < pDM->GetRowCount () ; i++)
	{
		strCompare = pDM->GetCellData  (_T("QUESTION_INFO_KEY"),i) ;
		strCompareType = pDM->GetCellData (_T("TYPE"),i) ;

		if(strFind == strCompare )
		{
		/*	
			pOutputDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchInfoRecKey,nRowIdx) ;
			pOutputDM->SetCellData (_T("QUESTION_INFO_KEY"),pDM->GetCellData (_T("QUESTION_INFO_KEY"),i),nRowIdx) ;
			pOutputDM->SetCellData (_T("TYPE"),pDM->GetCellData (_T("TYPE"),i),nRowIdx) ;
			pOutputDM->SetCellData (_T("SUBTITLE"),pDM->GetCellData (_T("SUBTITLE"),i),nRowIdx) ;
		*/	
			CString strSELECTVALUE;
			CString strSELECTCOUNT ;
		
			if(strCompareType == _T("�켱������"))
			{
				strSELECTVALUE = pDM->GetCellData (_T("SAMPLEVALUE"),i);
				strSELECTCOUNT = pDM->GetCellData (_T("SAMPLECOUNT"),i);
			}
			else if (strCompareType == _T("���ϼ�����") || strCompareType == _T("����������") )
			{
				strSELECTVALUE = pDM->GetCellData (_T("SELECTVALUE"),i);
				strSELECTCOUNT = pDM->GetCellData (_T("SELECTCOUNT"),i);
			}

			CString strTemp ;
			strTemp.Format (_T("�׸�%s"),strSELECTVALUE ) ;
			
			if (strCompareType == _T("���ϼ�����") || strCompareType == _T("����������") )
			m_pOutputDM->SetCellData (strTemp,strSELECTCOUNT,nLastCnt) ;


			nTotalCnt += _ttoi(strSELECTCOUNT) ;
			strTotalCnt.Format (_T("%d"),nTotalCnt) ;
			m_pOutputDM->SetCellData (_T("TOTAL"),strTotalCnt,nLastCnt) ;

		}
		else
		{
			INT nRowIdx = m_pOutputDM->GetRowCount () ;
			m_pOutputDM->InsertRow (-1) ;
			m_pOutputDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchInfoRecKey,nRowIdx) ;
			m_pOutputDM->SetCellData (_T("QUESTION_INFO_KEY"),pDM->GetCellData (_T("QUESTION_INFO_KEY"),i),nRowIdx) ;
			m_pOutputDM->SetCellData (_T("TYPE"),pDM->GetCellData (_T("TYPE"),i),nRowIdx) ;
			m_pOutputDM->SetCellData (_T("SUBTITLE"),pDM->GetCellData (_T("SUBTITLE"),i),nRowIdx) ;
			
			CString strSELECTVALUE;
			CString strSELECTCOUNT ;
			if(strCompareType == _T("�켱������"))
			{
				strSELECTVALUE = pDM->GetCellData (_T("SAMPLEVALUE"),i);
				strSELECTCOUNT = pDM->GetCellData (_T("SAMPLECOUNT"),i);
			}
			else if (strCompareType == _T("���ϼ�����") || strCompareType == _T("����������") )
			{
				strSELECTVALUE = pDM->GetCellData (_T("SELECTVALUE"),i);
				strSELECTCOUNT = pDM->GetCellData (_T("SELECTCOUNT"),i);
			}

			CString strTemp ;
			strTemp.Format (_T("�׸�%s"),strSELECTVALUE ) ;
			
			if (strCompareType == _T("���ϼ�����") || strCompareType == _T("����������") )
			m_pOutputDM->SetCellData (strTemp,strSELECTCOUNT,nRowIdx) ;


			m_pOutputDM->SetCellData (_T("TOTAL"),strSELECTCOUNT,nRowIdx) ;


			strFind = strCompare;
			strCompare = _T("") ;
			nTotalCnt = _ttoi(strSELECTCOUNT) ;
			nLastCnt = nRowIdx ;
		}

	}


	CESL_ControlMgr *pCM = FindCM(_T("CM_RESEARCH_RESULT"));
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid"));
	pGrid->Display (-1) ;

    //�����ο� üũ �ϱ� 
	for( i = 0 ; i <m_pOutputDM->GetRowCount () ;i++)
	{
		if(m_pOutputDM->GetCellData (_T("TYPE"),i) == _T("���ϼ�����"))
		{
			CString strReplyPeople;
			strReplyPeople =m_pOutputDM->GetCellData (_T("TOTAL"),i) ;
			
			CEdit* pEditPeople = (CEdit*)GetDlgItem(IDC_EDIT_TOTAL);
			pEditPeople->SetWindowText (strReplyPeople);

			break ;
		}
	}




	return 0 ;
}

INT CRESEARCH_RESULT_DLG::GetQuestionSample(CString strQuestionInfoKey)
{
	




	return 0 ;
}

BEGIN_EVENTSINK_MAP(CRESEARCH_RESULT_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CRESEARCH_RESULT_DLG)
	ON_EVENT(CRESEARCH_RESULT_DLG, IDC_RESEARCH_RESULT_GRID, -601 /* DblClick */, OnDblClickResearchResultGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CRESEARCH_RESULT_DLG::OnDblClickResearchResultGrid() 
{
	// TODO: Add your control notification handler code here
	
	CESL_ControlMgr *pCM = FindCM(_T("CM_RESEARCH_RESULT"));
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid"));


	INT nRowIdx = pGrid->GetIdx () ;

	if(m_pOutputDM->GetRowCount () == 0)
	{
		return ;
	}

	CString strQuestionInfoKey = m_pOutputDM->GetCellData (_T("QUESTION_INFO_KEY"),nRowIdx) ;
	CString strResearchKey     = m_pOutputDM->GetCellData (_T("RESEARCH_KEY"),nRowIdx) ;

	CString strQuery ;
	strQuery.Format (_T("QUESTION_KEY = %s "),strQuestionInfoKey) ;
	m_pQuestionDM->RefreshDataManager (strQuery) ;



	CESL_Grid *pSampleGrid = (CESL_Grid*)pCM->FindControl (_T("SampleGrid"));
	CESL_Grid *pSelectGrid = (CESL_Grid*)pCM->FindControl (_T("SelectGrid"));

	CESL_Grid *pMetrixGrid = (CESL_Grid*)pCM->FindControl (_T("MetrixGrid"));


	CString strType = m_pOutputDM->GetCellData (_T("TYPE"),nRowIdx) ;
	if(strType == _T("�켱������"))
	{
		pSampleGrid->ShowWindow (SW_HIDE) ;
		pMetrixGrid->ShowWindow (SW_HIDE);
		pSelectGrid->ShowWindow (SW_SHOW);
		
		GetSampleResult(nRowIdx) ;
		return ;
	}
	if(strType == _T("������"))
	{
		//AfxMessageBox (_T("������ ���� ���� ���")) ;
		CRESEARCH_RESULT_DESCTYPE dlg(this) ;
		dlg.SetQueryWhere (   strResearchKey ,strQuestionInfoKey) ;
		dlg.DoModal () ;

		return ;
	}
	if(strType == _T("��Ʈ������"))
	{

		pMetrixGrid->ShowWindow (SW_SHOW);
		pSampleGrid->ShowWindow (SW_HIDE) ;
		pSelectGrid->ShowWindow (SW_HIDE);
		
		GetMetrixResult(nRowIdx) ;

		return ;
	}


	pMetrixGrid->ShowWindow (SW_HIDE);
	pSampleGrid->ShowWindow (SW_SHOW) ;
	pSelectGrid->ShowWindow (SW_HIDE);
		


	CESL_DataMgr *pSampleTempDM = FindDM(_T("DM_RESEARCH_SAMPLE_TEMP"));
	pSampleTempDM->FreeData () ;


	CString strAlias ;
	CString strSampleTitle;
	CString strSelectCnt ;
	CString strPercent ;
	CString strTotal ;
	strTotal = m_pOutputDM->GetCellData (_T("TOTAL"),nRowIdx) ;
	
	CString strTemp ;

	CESL_DataMgr Temp_DM ;
	if(strType == _T("����������"))
	{
		/* ����������..��� ...
		select  B.SAMPLE_TITLE, ROUND(AVG(TO_NUMBER(trim(A.SELECTVALUE),'999')),1)  
		from research_result_tbl A , QUESTION_TBL B 
		WHERE  A.SAMPLEKEY = B.REC_KEY AND A.type ='����������' GROUP BY B.SAMPLE_TITLE
		*/	
       
		Temp_DM.SetCONNECTION_INFO (m_pQuestionDM->CONNECTION_INFO ) ;
		CString strQuery ;         //    0        ,   1
		strQuery.Format (_T("select  B.SAMPLE_TITLE, ROUND(AVG(TO_NUMBER(trim(A.SELECTVALUE),'999')),1) ") 
						 _T(" from research_result_tbl A , QUESTION_TBL B ")
						 _T(" WHERE  A.SAMPLEKEY = B.REC_KEY ") 
						 _T(" AND A.type ='����������' AND A.QUESTION_INFO_KEY = %s")
						 _T(" GROUP BY B.SAMPLE_TITLE ") , strQuestionInfoKey  ) ;
		Temp_DM.RestructDataManager (strQuery ) ;


	}

	for(INT i = 0 ; i < m_pQuestionDM->GetRowCount () ; i++)
	{
		m_pQuestionDM->GetCellData (_T("ALAIA"),i,strAlias); 
		m_pQuestionDM->GetCellData (_T("SAMPLE_TITLE"),i,strSampleTitle); 
		strTemp.Format (_T("�׸�%d"),i+1);
		

		if(strType != _T("����������"))
		{
			pSampleGrid->SetColumnInfo (2,_T("COUNT"));
			strSelectCnt = m_pOutputDM->GetCellData (strTemp,nRowIdx) ;
			if(strSelectCnt == _T("")) strSelectCnt = _T("0") 	;
			//strPercent.Format (_T("%2.2f"),((atof(strSelectCnt)/atof(strTotal )) *100)) ;
			TCHAR* pEndPtr = NULL;
			strPercent.Format (_T("%2.2f"),((_tcstod(strSelectCnt.GetBuffer(0), &pEndPtr)/_tcstod(strSelectCnt.GetBuffer(0), &pEndPtr )) *100)) ;
		}
		else if (strType == _T("����������"))
		{
			pSampleGrid->SetColumnInfo (2,_T("�������"));
			CString strTitle;
			CString strSelectValue ;
			for(INT i = 0  ; i < Temp_DM.GetRowCount () ;i++)
			{
				strTitle = Temp_DM.GetCellData (i,0) ;
				strSelectValue = Temp_DM.GetCellData (i,1) ;

				if(strSampleTitle == strTitle )
				{
					strSelectCnt = strSelectValue  ;
					strPercent   = strSelectValue  ;
					break ;
				}

			}
		}

		pSampleTempDM->InsertRow (-1) ;
		pSampleTempDM->SetCellData (_T("�׸�"),strTemp,i) ;
		pSampleTempDM->SetCellData (_T("TITLE"),strSampleTitle,i) ;
		pSampleTempDM->SetCellData (_T("SELECTCOUNT"),strSelectCnt,i) ;
		pSampleTempDM->SetCellData (_T("����"),strPercent,i) ;
		pSampleTempDM->SetCellData (_T("QUESTION_TITLE"),m_pQuestionDM->GetCellData (_T("SUBTITLE"),nRowIdx) ,i) ;


	}


	


	pSampleGrid->Display (-1) ;


	DisplayMSChart(pSampleTempDM);
}

VOID CRESEARCH_RESULT_DLG::DisplayMSChart(CESL_DataMgr *pDM)
{
	//IDC_MSCHART_SAMPLE ;
	CMSChartMgr* pMSChart = (CMSChartMgr*)GetDlgItem(IDC_MSCHART_SAMPLE) ;
	
	//CMSChartMgr* pMSChart = new CMSChartMgr
	
	if(pMSChart == NULL)
	{
		AfxMessageBox (_T("MSChart Failed")) ;
		return ;
	}

	INT COLNUM = pDM->GetRowCount () ;
	INT ROWNUM = 1 ;


	COleSafeArray saRet;
    INT i = 0, j = 0;
    CString str;

    CStringArray arrColName ;

	for( i = 0 ; i < COLNUM ; i++)
	{
		arrColName.Add ( pDM->GetCellData (_T("�׸�"),i) );
	}

	CStringArray arrRowName ;
	arrRowName.Add (_T("�����׸�")) ;


   
    //chart�� title ����
    pMSChart->SetTitleText(pDM->GetCellData (_T("QUESTION_TITLE"),0));
    //���� ����
    //pMSChart->SetFootnoteText(_T("WOW~!"));

    //X��, Y�� �̸��� property dialog box�� �̿��ؼ� ����

    
	
	//chart�� Ÿ�� ���� - chartType property
    pMSChart->SetChartType(1);


    //Chart���� �׸���� ���� ��� ���θ� �����Ѵ�.
    //default���� true�� �����Ǿ� �ִ� ��� chart���� �׸����
    //�����ϰų� �̵� ��ų �� �ִ�.
    //FALSE�� ������ ��� �׸���� ����, �̵���ų �� ����.
    pMSChart->SetAllowSelections(FALSE);

    //chart���� �׸� ���� ����ǥ ����
    //column�� ���̺��׸���� ��Ÿ��
    pMSChart->SetShowLegend(TRUE);

     //chart�� ���� ������, ���Ҽ��� property dialog�� ��tab����
    //�ڵ����� �ɼ��� ���� �����Ѵ�.
    
    //���콺 �����Ͱ� ��Ʈ��ü ���� ������ 
    //���콺 �������� ����� �����Ų��.
    //�ڼ��� ���� property dialog box�� MousePointer �׸� ����.
    pMSChart->SetMousePointer(2);
 
    //column ���� ���� - �� �׷��� ���� �׸��� ����
    pMSChart->SetColumnCount(COLNUM);

    //row ���� ���� - �׷����� ����
    pMSChart->SetRowCount(ROWNUM);

    //column ���̺� ����
    for(i=1; i<=COLNUM; i++)
    { 
        pMSChart->SetColumn(i);
        pMSChart->SetColumnLabel(arrColName.GetAt (i-1));
    }
    


    for(i=1; i<=ROWNUM; i++)
    {
        //row ���̺� ����
        pMSChart->SetRow(i);
        pMSChart->SetRowLabel(arrRowName.GetAt (i-1));
        
        //row���� ������ ����
        for(j=1 ; j<=COLNUM; j++)
        {
            pMSChart->SetColumn(j);

			str = pDM->GetCellData (_T("����"),j-1);

            //str.Format(_T("%d"), 5*i*j);
            pMSChart->SetData(str);
        }
    }

    //chart�� �ٽ� �׸�
    pMSChart->Refresh();      
}


//�켱������ ��� �������� 
INT CRESEARCH_RESULT_DLG::GetSampleResult(INT nRowindex)
{
	CESL_DataMgr *pSelectTempDM = FindDM(_T("DM_RESULT_SELECT_TEMP"));
	pSelectTempDM->FreeData () ;


	CString strAlias ;
	CString strSampleTitle;
	CString strSelectCnt ;
	CString strPercent ;
	CString strTotal ;
	strTotal = m_pOutputDM->GetCellData (_T("TOTAL"),nRowindex) ;
	
	CString strTemp ;
    

	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_pOutputDM->CONNECTION_INFO );
	
	CString strQuery ;
	CString strQuestion_info_key  ;

	strQuestion_info_key = m_pOutputDM->GetCellData (_T("QUESTION_INFO_KEY"),nRowindex) ;
                                   //    0        1    2          3 
	CESL_DataMgr *pDM = FindDM(_T("DM_RESULT_SELECT_OUTPUT"));

	CString strOption ;
	strOption.Format (_T(" A.research_key = %s and b.question_key = %s "),m_strResearchInfoRecKey ,strQuestion_info_key);
	pDM->RefreshDataManager (strOption);


	CString strFind =_T("");
	CString strCompare =_T("");

	
	INT nLastCnt = -1 ;
	INT nTotalCnt = 0 ;
	CString strTotalCnt ;

	for(INT i = 0 ; i < pDM->GetRowCount () ; i++)
	{
		strCompare = pDM->GetCellData (_T("SAMPLEKEY"),i) ;
		

		if(strFind == strCompare )
		{
		/*	
			pOutputDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchInfoRecKey,nRowIdx) ;
			pOutputDM->SetCellData (_T("QUESTION_INFO_KEY"),pDM->GetCellData (_T("QUESTION_INFO_KEY"),i),nRowIdx) ;
			pOutputDM->SetCellData (_T("TYPE"),pDM->GetCellData (_T("TYPE"),i),nRowIdx) ;
			pOutputDM->SetCellData (_T("SUBTITLE"),pDM->GetCellData (_T("SUBTITLE"),i),nRowIdx) ;
		*/	
			CString strSELECTVALUE;
			CString strSELECTCOUNT ;
		
		
			strSELECTVALUE = pDM->GetCellData (_T("SAMPLEVALUE"),i);
			strSELECTCOUNT = pDM->GetCellData (_T("SAMPLECOUNT"),i);
			if(strSELECTCOUNT == _T("")) strSELECTCOUNT = _T("0") 	;

			CString strTemp ;
			strTemp.Format (_T("%s����"),strSELECTVALUE ) ;

			pSelectTempDM->SetCellData (strTemp,strSELECTCOUNT,nLastCnt) ;
		

			nTotalCnt += _ttoi(strSELECTCOUNT) ;
			strTotalCnt.Format (_T("%d"),nTotalCnt) ;
			pSelectTempDM->SetCellData (_T("TOTAL"),strTotalCnt,nLastCnt) ;

		}
		else
		{
			INT nRowIdx = pSelectTempDM->GetRowCount () ;
			pSelectTempDM->InsertRow (-1) ;
			pSelectTempDM->SetCellData (_T("SAMPLETITLE"),pDM->GetCellData (_T("SAMPLETITLE"),i),nRowIdx) ;
			
			CString strSELECTVALUE;
			CString strSELECTCOUNT ;
		
			strSELECTVALUE = pDM->GetCellData (_T("SAMPLEVALUE"),i);
			strSELECTCOUNT = pDM->GetCellData (_T("SAMPLECOUNT"),i);
			if(strSELECTCOUNT == _T("")) strSELECTCOUNT = _T("0") 	;
		

			CString strTemp ;
			strTemp.Format (_T("%s����"),strSELECTVALUE ) ;

			pSelectTempDM->SetCellData (strTemp,strSELECTCOUNT,nRowIdx) ;
			pSelectTempDM->SetCellData (_T("TOTAL"),strSELECTCOUNT,nRowIdx) ;


			strFind = strCompare;
			strCompare = _T("") ;
			nTotalCnt = _ttoi(strSELECTCOUNT) ;
			nLastCnt = nRowIdx ;
		}

	}


	CESL_ControlMgr *pCM = FindCM(_T("CM_RESEARCH_RESULT"));	
	CESL_Grid *pSelectGrid = (CESL_Grid*)pCM->FindControl (_T("SelectGrid"));
	pSelectGrid->Display (-1) ;

	DisplaySelectMSChart(pSelectTempDM);

	return 0 ;
}

VOID CRESEARCH_RESULT_DLG::DisplaySelectMSChart(CESL_DataMgr *pDM)
{
//IDC_MSCHART_SAMPLE ;
	CMSChartMgr* pMSChart = (CMSChartMgr*)GetDlgItem(IDC_MSCHART_SAMPLE) ;
	
	//CMSChartMgr* pMSChart = new CMSChartMgr
	
	if(pMSChart == NULL)
	{
		AfxMessageBox (_T("MSChart Failed")) ;
		return ;
	}

	INT COLNUM = 8 ;
	INT ROWNUM = pDM->GetRowCount () ;


	COleSafeArray saRet;
    INT i = 0, j = 0;
    CString str;

    CStringArray arrColName ;
	CStringArray arrRowName ;
	
	CString strTemp ;
	for( i = 0 ; i < COLNUM ; i++)
	{
		strTemp.Format (_T("%d����"),i+1) ;
		arrColName .Add (strTemp) ;
	}


	for( i = 0 ; i < ROWNUM ; i++)
	{
		arrRowName.Add ( pDM->GetCellData (_T("SAMPLETITLE"),i) );
		
	}
	


   
    //chart�� title ����
    //pMSChart->SetTitleText(pDM->GetCellData (_T("SAMPLE"),0));
    //���� ����
    //pMSChart->SetFootnoteText(_T("WOW~!"));

    //X��, Y�� �̸��� property dialog box�� �̿��ؼ� ����

    
	
	//chart�� Ÿ�� ���� - chartType property
    pMSChart->SetChartType(1);


    //Chart���� �׸���� ���� ��� ���θ� �����Ѵ�.
    //default���� true�� �����Ǿ� �ִ� ��� chart���� �׸����
    //�����ϰų� �̵� ��ų �� �ִ�.
    //FALSE�� ������ ��� �׸���� ����, �̵���ų �� ����.
    pMSChart->SetAllowSelections(FALSE);

    //chart���� �׸� ���� ����ǥ ����
    //column�� ���̺��׸���� ��Ÿ��
    pMSChart->SetShowLegend(TRUE);

     //chart�� ���� ������, ���Ҽ��� property dialog�� ��tab����
    //�ڵ����� �ɼ��� ���� �����Ѵ�.
    
    //���콺 �����Ͱ� ��Ʈ��ü ���� ������ 
    //���콺 �������� ����� �����Ų��.
    //�ڼ��� ���� property dialog box�� MousePointer �׸� ����.
    pMSChart->SetMousePointer(2);
 
    //column ���� ���� - �� �׷��� ���� �׸��� ����
    pMSChart->SetColumnCount(COLNUM);

    //row ���� ���� - �׷����� ����
    pMSChart->SetRowCount(ROWNUM);

    //column ���̺� ����
    for(i=1; i<=COLNUM; i++)
    { 
        pMSChart->SetColumn(i);
        pMSChart->SetColumnLabel(arrColName.GetAt (i-1));
    }
    


	CString strTempAlias ;
    for(i=1; i<=ROWNUM; i++)
    {
        //row ���̺� ����
        pMSChart->SetRow(i);
        pMSChart->SetRowLabel(arrRowName.GetAt (i-1));
        
        //row���� ������ ����
        for(j=1 ; j<=COLNUM; j++)
        {
            pMSChart->SetColumn(j);
             
			strTempAlias.Format (_T("%d����"),j) ;
			str = pDM->GetCellData (strTempAlias,i-1);
			if(str == _T("")) str = _T("0") ;

            //str.Format(_T("%d"), 5*i*j);
            pMSChart->SetData(str);
        }
    }

    //chart�� �ٽ� �׸�
    pMSChart->Refresh();   
}

INT CRESEARCH_RESULT_DLG::GetMetrixResult(INT nRowindex)
{

	/*

	SELECT A.QUESTION_INFO_KEY, A.TYPE,  B.SUBTITLE, A.SAMPLEKEY, A.SAMPLEVALUE, COUNT(A.SAMPLEVALUE) 
	FROM research_result_tbl A,METRIX_QUESTION_INFO_TBL B 
	WHERE A.SAMPLEKEY = B.REC_KEY AND  A.QUESTION_INFO_key = 63270066 
	group by A.question_info_key,A.type , B.SUBTITLE ,A.SAMPLEKEY, A.SAMPLEVALUE

	*/

	CString strQuestion_info_key  ;
	strQuestion_info_key = m_pOutputDM->GetCellData (_T("QUESTION_INFO_KEY"),nRowindex) ;

	//DM_RESULT_METRIX_OUTPUT
	CESL_DataMgr *pMakeMatrixDM = FindDM(_T("DM_RESULT_METRIX_OUTPUT")) ;
	CString strQuery ;
	strQuery.Format (_T(" A.QUESTION_INFO_KEY = %s "),strQuestion_info_key ) ;
	pMakeMatrixDM->RefreshDataManager (strQuery ) ;

	//DM_RESEARCH_RESULT_OUTPUT_METRIXTEMP
	

	
	CString strFind =_T("");
	CString strCompare =_T("");

	CESL_DataMgr *pOutPutDM ;

	pOutPutDM = FindDM(_T("DM_RESEARCH_RESULT_OUTPUT_METRIXTEMP"));
	pOutPutDM->FreeData () ;
	
	INT nLastCnt = -1 ;
	INT nTotalCnt = 0 ;
	CString strTotalCnt ;

	CString strCompareType ;

	for(INT i = 0 ; i < pMakeMatrixDM->GetRowCount () ; i++)
	{
		strCompare = pMakeMatrixDM->GetCellData  (_T("SAMPLEKEY"),i) ;
		strCompareType = pMakeMatrixDM->GetCellData (_T("TYPE"),i) ;

		if(strFind == strCompare )
		{
	
			CString strSELECTVALUE;
			CString strSELECTCOUNT ;
		
			strSELECTVALUE = pMakeMatrixDM->GetCellData (_T("SAMPLEVALUE"),i);
			strSELECTCOUNT = pMakeMatrixDM->GetCellData (_T("SAMPLECOUNT"),i);
		
		
			CString strTemp ;
			strTemp.Format (_T("�׸�%s"),strSELECTVALUE ) ;
			
			pOutPutDM->SetCellData (strTemp,strSELECTCOUNT,nLastCnt) ;


			nTotalCnt += _ttoi(strSELECTCOUNT) ;
			strTotalCnt.Format (_T("%d"),nTotalCnt) ;
			pOutPutDM->SetCellData (_T("TOTAL"),strTotalCnt,nLastCnt) ;

		}
		else
		{
			INT nRowIdx = pOutPutDM->GetRowCount () ;
			pOutPutDM->InsertRow (-1) ;
			pOutPutDM->SetCellData (_T("RESEARCH_KEY"),m_strResearchInfoRecKey,nRowIdx) ;
			pOutPutDM->SetCellData (_T("QUESTION_INFO_KEY"),pMakeMatrixDM->GetCellData (_T("QUESTION_INFO_KEY"),i),nRowIdx) ;
			pOutPutDM->SetCellData (_T("TYPE"),pMakeMatrixDM->GetCellData (_T("TYPE"),i),nRowIdx) ;
			pOutPutDM->SetCellData (_T("SUBTITLE"),pMakeMatrixDM->GetCellData (_T("SUBTITLE"),i),nRowIdx) ;
			
			CString strSELECTVALUE;
			CString strSELECTCOUNT ;
		
			strSELECTVALUE = pMakeMatrixDM->GetCellData (_T("SAMPLEVALUE"),i);
			strSELECTCOUNT = pMakeMatrixDM->GetCellData (_T("SAMPLECOUNT"),i);
		

			CString strTemp ;
			strTemp.Format (_T("�׸�%s"),strSELECTVALUE ) ;
			
			pOutPutDM->SetCellData (strTemp,strSELECTCOUNT,nRowIdx) ;


			pOutPutDM->SetCellData (_T("TOTAL"),strSELECTCOUNT,nRowIdx) ;


			strFind = strCompare;
			strCompare = _T("") ;
			nTotalCnt = _ttoi(strSELECTCOUNT) ;
			nLastCnt = nRowIdx ;
		}

	}


	CESL_ControlMgr *pCM = FindCM(_T("CM_RESEARCH_RESULT"));
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MetrixGrid"));
	pGrid->Display (-1) ;





    //�����ο� üũ �ϱ� 
/*
	for( i = 0 ; i <m_pOutputDM->GetRowCount () ;i++)
	{
		if(m_pOutputDM->GetCellData (_T("TYPE"),i) == _T("���ϼ�����"))
		{
			CString strReplyPeople;
			strReplyPeople =m_pOutputDM->GetCellData (_T("TOTAL"),i) ;
			
			CEdit* pEditPeople = (CEdit*)GetDlgItem(IDC_EDIT_TOTAL);
			pEditPeople->SetWindowText (strReplyPeople);

			break ;
		}
	}
*/
	

	return 0 ;
}

HBRUSH CRESEARCH_RESULT_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
