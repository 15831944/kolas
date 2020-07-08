// CONTENTMGR_02.cpp : implementation file
//

#include "stdafx.h"
#include "CONTENTMGR_02.h"
#include "CONTENTMGR_01.h"
// Ű���� ����
#include "..\..\..\����\����\BO_CAT_PROC_KEYWORD\BO_CAT_PROC_KEYWORD_0000.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "CommonDefine.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCONTENTMGR_01 *g_pParent;

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_02 dialog


CCONTENTMGR_02::CCONTENTMGR_02(INT nDlgType,CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nDlgType = nDlgType ;
	
	m_DbClckMode = FALSE ;
	m_idx = -1;

	m_KeyWord_update = false ;
	m_MakeSearchData = NULL;
	g_pParent = (CCONTENTMGR_01*)pParent;

	m_str90=_T("");
}
CCONTENTMGR_02::~CCONTENTMGR_02()
{

}

BOOL CCONTENTMGR_02::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CCONTENTMGR_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCONTENTMGR_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_02, CDialog)
	//{{AFX_MSG_MAP(CCONTENTMGR_02)
	ON_BN_CLICKED(IDC_bBRINGINC, OnbBRINGINC)
	ON_BN_CLICKED(IDC_bBRINGINA, OnbBRINGINA)
	ON_BN_CLICKED(IDC_bSAVEA, OnbSAVEA)
	ON_BN_CLICKED(IDC_bSAVEC, OnbSAVEC)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_MESSAGE(MSG_SHOW_VIEWER, OnShowViewer)
	ON_BN_CLICKED(IDC_bCDELETE, OnbCDELETE)
	ON_BN_CLICKED(IDC_bADELETE, OnbADELETE)
	ON_BN_CLICKED(IDC_bMOK_KEYWORD, OnbMOKKEYWORD)
	ON_BN_CLICKED(IDC_bCHO_KEYWORD, OnbCHOKEYWORD)
	ON_BN_CLICKED(IDC_bDCMInput, OnbDCMInput)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_02 message handlers

BOOL CCONTENTMGR_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect	TocRect;
	CRect	ChoRect;
	INT		ids;	
	
    m_ParentGrid = _T("MainGrid_CONTENT");
	m_ParentCM   = _T("CM_CONTENTMGR_MAIN");
	m_ParentDM   = _T("DM_BO_CONTENTMGR_SPECIES") ;

    if(InitESL_Mgr(_T("SM_CONTENTMGR_02"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_CONTENTMGR_02")) ;
	if(pCM == NULL) 
	{
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -3 ;
	}

	m_pGrid = (CESL_Grid*)pCM->FindControl(_T("VolGrid")) ;
	if(m_pGrid == NULL)
	{
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return -4;
	}

	CESL_DataMgr* pDataMgr;	
	CString strWhere;

	pDataMgr = FindDM(m_ParentDM);
	if(pDataMgr == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -2 ;
	}	
	
	//++2008.10.22 UPDATE BY PWR {{++
	// ���սý��� ����
	strWhere.Format( _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
					 _T("WHERE CLASS_ALIAS='�����' AND VALUE_NAME='�����' AND GROUP_1='L' ")
					 _T("AND GROUP_2='��Ÿ' AND GROUP_3='����' AND MANAGE_CODE=UDF_MANAGE_CODE") );
// 	strWhere.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='�����' AND VALUE_NAME='�����' AND GROUP_1='L' AND GROUP_2='��Ÿ' AND GROUP_3='����'"));
	//--2008.10.22 UPDATE BY PWR --}}
	pDataMgr->GetOneValueQuery( strWhere, m_str90, FALSE );	

	if ( m_str90 != _T("Y") )
	{
		m_pGrid->ShowWindow(SW_HIDE);
		
		( (CStatic*) GetDlgItem (IDC_STATIC_01) )->GetWindowRect(TocRect);
		ScreenToClient(&TocRect);
		TocRect.top = TocRect.top - 90;
		TocRect.bottom = TocRect.bottom - 90;
		( (CStatic*) GetDlgItem (IDC_STATIC_01) )->MoveWindow(TocRect);		

		( (CStatic*) GetDlgItem (IDC_STATIC_02) )->GetWindowRect(TocRect);
		ScreenToClient(&TocRect);
		TocRect.top = TocRect.top - 90;
		TocRect.bottom = TocRect.bottom - 90;
		( (CStatic*) GetDlgItem (IDC_STATIC_02) )->MoveWindow(TocRect);		

		( (CStatic*) GetDlgItem (IDC_TocRect) )->GetWindowRect(TocRect);
		ScreenToClient(&TocRect);
		TocRect.top = TocRect.top - 90;		
		( (CStatic*) GetDlgItem (IDC_TocRect) )->MoveWindow(TocRect);		

		( (CStatic*) GetDlgItem (IDC_ChoRect) )->GetWindowRect(TocRect);
		ScreenToClient(&TocRect);
		TocRect.top = TocRect.top - 90;		
		( (CStatic*) GetDlgItem (IDC_ChoRect) )->MoveWindow(TocRect);	
	}

	(( CStatic* ) GetDlgItem (IDC_TocRect))->GetWindowRect(TocRect);
	(( CStatic* ) GetDlgItem (IDC_ChoRect))->GetWindowRect(ChoRect);

	ScreenToClient(&TocRect);
	ScreenToClient(&ChoRect);

	m_TocEditdlg.MyCreate(this,TocRect ,_DAN_SINGLE);
	m_ChorokDlg.MyCreate(this,&m_TocEditdlg,ChoRect);

    if(m_DbClckMode == TRUE)
	{
         DbClick_Move(0) ;
	}
    else
	{

		ids = Init(0);
		if ( ids ) return TRUE;
    }

	LoadVolInfo();

	// ����,�ʷ� ó���� �����ֱ�
	PrevNextProc(0);
	m_pGrid->SetReverse(0);

	
	m_MakeSearchData = new CMakeSearchData(this);	 

    //IDC_bSPECIES
	(CButton*)GetDlgItem(IDC_bSPECIES)->ShowWindow(FALSE) ;
	
	strOldContents = m_TocEditdlg.GetFinalToc();
	strOldAbstract = m_ChorokDlg.GetChorokText();

	( (CEdit*) GetDlgItem (IDC_eMOK_KEYWORD) )->SetLimitText(4000);
	( (CEdit*) GetDlgItem (IDC_eCHO_KEYWORD) )->SetLimitText(4000);

	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



INT CCONTENTMGR_02::Init(INT nDir)
{
    INT ids ;

	CString strShow;

   	// Member Initialize
	m_strCommonRecKey=_T("");  //���� �ʷ� ���������� ���ڵ�Ű(���ڵ� �������ε� �̰����� üũ)
	m_strCSource=_T("");	   //���� ����
	m_strASource=_T("");	   //�ʷ� ����
	m_strCCreateDate=_T("");   //���� ������
	m_strCUpdateDate=_T("");   //���� ������ 
	m_strACreateDate=_T("");   //�ʷ� ������
	m_strAUpdateDate=_T("");   //�ʷ� ������ 
	m_strMarc=_T("");	       //��ũ
	m_strFormCode=_T("");	   //���ı���
	m_strMatCode=_T("");	   //�ڷᱸ��
	m_strRegCode=_T("");	   //��ϱ���
	m_strUseLimit=_T("");	   //�̿����ѱ���
	m_strSpeciesKey=_T("");    //��Ű
	m_strControlNo=_T("");     //�����ȣ
//	m_strUbControlNo=_T("");   //.NET�����ȣ(key)
	m_strClassCode=_T("");	   //������
	m_strConYN=_T("");		   //��������
	m_strAbsYN=_T("");		   //��������
//	m_strNetConYN=_T("");	   //NET��������
//	m_strNetAbsYN=_T("");	   //NET�ʷ�����
	
	m_strConKey=_T("");	       //����Ű
	m_strAbsKey=_T(""); 	   //�ʷ�Ű

    //<-  �����ʷ� ���� �˻����� ����
    m_strWorkStatus;       //�۾�����
    m_strPlaceInfo;        //�ڷ��ִ°�
    //->  �����ʷ� ���� �˻����� ����
	
	// Initialize
	CString strWhere;
	CString strTemp;
	
	CESL_DataMgr* pDataMgr;	

	pDataMgr = FindDM(m_ParentDM);
	if(pDataMgr == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -2 ;
	}

	CESL_ControlMgr *pParentCM = FindCM(m_ParentCM) ;
	if(pParentCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -3 ;
	}

	CESL_Grid *pGrid = (CESL_Grid*)pParentCM->FindControl(m_ParentGrid) ;
	if(pGrid == NULL)
	{
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return -4;
	}	

	CESL_DataMgr* pDataContents;

    pDataContents = FindDM(_T("DM_�����ʷ�")) ;
	if(pDataContents == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}
		
	switch(nDir)
	{
	case 0 :
		
		SelectMakeList(m_ParentCM ,m_ParentGrid);
		ids = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid);
		if (ids < 0)
		{
			//2004-07-07 ������ 
			//AfxMessageBox(_T("���� ����� ������ �� �����ϴ�."));	// ����
			AfxMessageBox(_T("���� �׸��� �����ϴ�."));	// ����
			EndDialog(IDCANCEL);
			return -222;
		}
		break;
		
	case -1 :
		
		ids = SelectGetPrev (m_ParentCM ,m_ParentGrid);
		if (ids == -1) {
			ids = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid);
			AfxMessageBox(_T("ó�� �ڷ��Դϴ�."));
			return -223;
		}
		break;
		
	case 1 :
		
		ids = SelectGetNext (m_ParentCM ,m_ParentGrid);
		if (ids == -1) {
			ids = SelectGetTailPosition(m_ParentCM ,m_ParentGrid);
			AfxMessageBox(_T("������ �ڷ��Դϴ�."));
			return -224;
		}
		break;
		
	}
	
	m_idx = ids; 
	SetReverse(m_ParentCM ,m_ParentGrid, m_idx);   
	
	if(pDataMgr->GetRowCount()>0){
		m_nDataRowIdx= pGrid->SelectGetIdx ();
	}else{
		m_nDataRowIdx=-1;
	}
	
	

	// 3.ȭ���� �����Ѵ�.
	if(m_nDlgType==0){
		//SetDlgItemText(IDC_eCONTROL_NO,m_strControlNo);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_�����ȣ"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eCONTROL_NO,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_��ǥ��"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eTITLE,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_������"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eAUTHOR,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_������"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_ePUBLISHER,strTemp);
	}else{
        if(GetDataMgrData (m_ParentDM ,_T("IBS_�����ȣ"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eCONTROL_NO,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_��ǥ��"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eTITLE,strTemp);
        if(GetDataMgrData (m_ParentDM ,_T("IBS_������"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eAUTHOR,strTemp);
        if(GetDataMgrData (m_ParentDM ,_T("IBS_������"),strTemp,m_nDataRowIdx ) == 0)
		{	SetDlgItemText(IDC_ePUBLISHER,strTemp);
			SetControlData (_T("CM_CONTENTMGR_02"),_T("������"),strTemp,-1) ;
		}
	}	
	
	GetDataMgrData (m_ParentDM ,_T("IBS_��KEY"),m_strSpeciesKey,m_nDataRowIdx );

	return 0 ;
}

VOID CCONTENTMGR_02::OnbBRINGINC() 
{
	// TODO: Add your control notification handler code here
	if(!BringIn(0))AfxMessageBox(_T("�������Խ���"));
}

VOID CCONTENTMGR_02::OnbBRINGINA() 
{
	// TODO: Add your control notification handler code here
	if(!BringIn(1))AfxMessageBox(_T("�ʷϹ��Խ���"));
}

VOID CCONTENTMGR_02::OnCancel() 
{
	CString strFinalContents, strFinalAbstract;
	CString strMsg;

	strFinalContents = m_TocEditdlg.GetFinalToc();
	strFinalAbstract = m_ChorokDlg.GetChorokText();

	strMsg = _T("");
	if ( strOldContents != strFinalContents && strOldAbstract != strFinalAbstract)
	{
		strMsg = _T("������ �ʷ��� ����Ǿ����ϴ�.\n");		
	} 
	else if ( strOldContents != strFinalContents )
	{
		strMsg = _T("������ ����Ǿ����ϴ�.\n");		
	}
	else if ( strOldAbstract != strFinalAbstract )
	{
		strMsg = _T("�ʷ��� ����Ǿ����ϴ�.\n");		
	}	

	if ( !strMsg.IsEmpty() )
	{
		strMsg += _T("�����ðڽ��ϱ�?");
		if ( AfxMessageBox(strMsg, MB_OKCANCEL)!=IDOK )
		{
			return;
		}
	}
	
	CDialog::OnCancel();
}

VOID CCONTENTMGR_02::OnbSAVEA() 
{	
	Save(1);
	m_ChorokDlg.SetChorok(m_strTemp);
	strOldAbstract = m_strTemp;
}

VOID CCONTENTMGR_02::OnbSAVEC() 
{	
	Save(0);
	m_TocEditdlg.SetTocFromString(m_strTemp);
	strOldContents = m_strTemp;	
}


BOOL CCONTENTMGR_02::BringIn(INT nType)
{
	// ���� ��θ� �����Ѵ�.
	TCHAR szOriginPath[1024] =_T("\0"); //������
	GetCurrentDirectory(1024,szOriginPath);
	// Ž�� ����
	CFileDialog filedlg(true,_T(""),_T(""),OFN_HIDEREADONLY |OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_LONGNAMES|OFN_PATHMUSTEXIST,_T("����/�ʷ� (*.toc) |*.toc|�ؽ�Ʈ (*.txt) |*.txt|"),this);
POPUPDLG:
	if(_tcscmp(m_szCurrentPath,_T(""))!=0)
		SetCurrentDirectory(m_szCurrentPath);
	if(filedlg.DoModal()==IDOK){
		GetCurrentDirectory(1024,m_szCurrentPath);
		SetCurrentDirectory(szOriginPath);
		CFile file;
		if(!file.Open(filedlg.GetPathName(),CFile::modeRead | CFile::typeBinary))
		{
			AfxMessageBox(_T("ȭ���� �� �� �����ϴ�."));
			goto ERR_RETURN;
		}
		else
		{
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file.SeekToBegin();
			}
			
			// 1. �������� ������
			CString strBackup;
			if(nType==0)
				strBackup=m_TocEditdlg.GetFinalToc();
			else
				strBackup=m_ChorokDlg.GetChorokText();

			// 2. ���µ� ��Ʈ�� ���� �����⿡ �־��ش�.
			CString strFileStream;
			if(file.ReadHuge( strFileStream.GetBuffer(file.GetLength()),file.GetLength())<=0){
				strFileStream.ReleaseBuffer();
				goto ERR_RETURN;
			}
			strFileStream.ReleaseBuffer();

			//=====================================================
			//2010.01.18 ADD BY PJW : �����Ⱚ ���� �ڵ�
 			TCHAR cData = 52685;

			for( INT i = strFileStream.GetLength() -1 ; i >= 0 ; i-- )
			{
				if ( cData != strFileStream.GetAt(i) )
				{
					strFileStream = strFileStream.Left(i+1);
					break;
				}
			}
			//=====================================================


			if(nType==0)
			{
				CString strTempStream ;
				strTempStream = ReMake_BringInStream(nType,strFileStream) ;
				m_TocEditdlg.SetTocFromString( strTempStream );
				//m_TocEditdlg.SetTocFromString( strFileStream );
            }
			else
				m_ChorokDlg.SetChorok( strFileStream );

			// 3. �״�� �����Ұ����� ����ڿ��� ���´�.
			if(AfxMessageBox(_T("����Ͻðڽ��ϱ�?"),MB_YESNO)==IDNO)
			{
				if(nType==0)m_TocEditdlg.SetTocFromString( strBackup );
				else m_ChorokDlg.SetChorok( strBackup  );
				goto POPUPDLG;
			}
		}
	}
	SetCurrentDirectory(szOriginPath);
	return true;
ERR_RETURN:
	SetCurrentDirectory(szOriginPath);
	return false;
}

VOID CCONTENTMGR_02::OnbNEXT() 
{	
     if(m_DbClckMode == TRUE)
	{
        DbClick_Move(1);
		LoadVolInfo();
	}
    else
	{
	     Init(1);
		 PrevNextProc(0);
		 LoadVolInfo();
		 m_pGrid->SetReverse(0);
	}
	 
}

VOID CCONTENTMGR_02::OnbPREV() 
{	
     if(m_DbClckMode == TRUE)
	{
        DbClick_Move(-1) ;
		LoadVolInfo();
	}
    else
	{
	    Init(-1);
		PrevNextProc(0);
		LoadVolInfo();
		m_pGrid->SetReverse(0);
	}
}


// m_nDlgType 0 : �������� �Է� 1: �⺻ȭ�鿡�� ����     // DISP���� ��ũ �޴��� ������ �������� ������ �д�.
// nType      0 : ����          1:�ʷ�   
VOID CCONTENTMGR_02::Save(INT nType)
{ 
	// Initialize
	CString strBtnText;
	CString strConAbsYN;		//����
	CString strConAbs  ;		//������ �ʷ� 
	CString szRecKey;			//������ �ʷ�Ű 
	CString system_date;		// YYYY/MM/DD
	CString strVolSortNo;
	CString strQuery;
	CString strValue;
	BOOL	bFirstInsert;		// ���� �ʷ� �������̺� ���� ���������� �����ϴ� �÷�
	INT ids=-1;	
	CESL_DataMgr* pDataMgr;

    pDataMgr = FindDM(m_ParentDM ) ;
	if(pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

	CESL_DataMgr* pVolDataMgr;
	CString	      strWhere;
	
	pVolDataMgr = FindDM(_T("DM_CONTENTS_VOL")) ;
	if(pVolDataMgr == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

	// 2006.01.11 ADD BY PDJ
	// �ٱǿ� ���� ��ȣ
	INT nRow = m_pGrid->GetRow();
	pVolDataMgr->GetCellData(_T("�Ϸù�ȣ"), nRow-1, strVolSortNo );	

    if ( m_strCommonRecKey == _T("") ) 
	{
		bFirstInsert = TRUE;
	}
	else 
	{
		bFirstInsert = FALSE;
	}

	// Get Current Date
	CTime t = CTime::GetCurrentTime () ;
    system_date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;

	
	// Syntex Check and Get Values
    if(nType==0){
        strConAbs=m_TocEditdlg.GetFinalToc();
         m_strTemp=strConAbs;
        if(m_TocEditdlg.CheckSyntax())return;
    }
    else {
        strConAbs=m_ChorokDlg.GetChorokText();
        m_strTemp=strConAbs;

    }

	if(nType==0)GetDlgItemText(IDC_bSAVEC,strBtnText);
	else GetDlgItemText(IDC_bSAVEA,strBtnText);

	if( strBtnText == _T("��  ��") ) 
	{

		///////////////////////////////////////////////////////////////////////////////////////////////
		// DB Operation
		///////////////////////////////////////////////////////////////////////////////////////////////
	    
		pDataMgr->StartFrame () ;
	
		
		// 1. ����/�ʷ� ������ ���� (LS)
		if(m_strSpeciesKey!=_T(""))
		{		
			pDataMgr->InitDBFieldData () ;
			
			if(nType==0)pDataMgr->AddDBFieldData(_T("CONTENTS_YN"),_T("STRING"),_T("Y"));
			else pDataMgr->AddDBFieldData(_T("ABSTRACTS_YN"),_T("STRING"),_T("Y"));						
			
			ids = pDataMgr->MakeUpdateFrame (_T("IDX_BO_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strSpeciesKey);
			if(ids <0)goto ERR_RETURN;

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 2003.11.3 ������ �߰� 
			// IDX_BO_TBL UPDATE �� IDX_TMP_BO_TBL �Է��ϱ� 
			//=====================================================
			//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 			pDataMgr->InitDBFieldData ();
// 			
// 			CString strTmp_Key ;
// 			CString system_Date ;
// 			CTime t = CTime::GetCurrentTime ();
// 			system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
// 			
// 			// 1. �����ʷ� �÷��� ���� TEMP TABLE : IDX_TMP_BO_TBL
// 			pDataMgr->MakeRecKey (strTmp_Key) ;
// 			pDataMgr->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
// 			pDataMgr->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
// 			pDataMgr->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("I"));
// 			pDataMgr->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);
// 			ids = pDataMgr->MakeInsertFrame (_T("IDX_TMP_BO_TBL"));
// 			if(ids<0) {
// 				AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_BO_TBL) Error~!"));
// 				return ;
// 			}			
			//=====================================================
			
		}

		// 2. ���� DB �۾�
		// 2.1. ���� �ʷ� ���������� ����. -> ���ο� ���ڵ带 �����Ѵ�.
		if(m_strCommonRecKey==_T("")){  // bFirstInsert�� ���� �ǹ������� �̰��� �� �������̴�.
			if(pDataMgr->MakeRecKey (m_strCommonRecKey))goto ERR_RETURN; 
			m_strCommonRecKey.ReleaseBuffer();
			pDataMgr->InitDBFieldData () ;
			pDataMgr->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey);
			pDataMgr->AddDBFieldData(_T("SPECIES_KEY"),_T("STRING"),m_strSpeciesKey);
			pDataMgr->AddDBFieldData(_T("SPECIES_CLASS"),_T("STRING"),_T("1"));//�谡����
			pDataMgr->AddDBFieldData(_T("CONTROL_NO"),_T("STRING"),m_strControlNo);
			pDataMgr->AddDBFieldData(_T("VOL_SORT_NO"),_T("NUMERIC"),strVolSortNo);			
			//pDataMgr->AddDBFieldData(_T("USE_LIMIT_CODE"),_T("STRING"),m_strUseLimit);
			// 2009.09.16 ADD BY PWR : �����ʷ� ����� MANAGE_CODE�߰�
			pDataMgr->AddDBFieldData(_T("MANAGE_CODE"),_T("STRING"),m_pInfo->MANAGE_CODE);
		    ids = pDataMgr->MakeInsertFrame (_T("BO_TOC_COMMON_TBL")) ; 
			if(ids < 0 )goto ERR_RETURN;

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 2006.1.2 ADD BY PDJ ����/�ʷϿ��� TOCŰ�� �������� �ʴ´�. 1:N ����
			//2003.11.3 ������ �߰� 
			//BO_SPECIES_TBL �� TOC_COMMON_KEY -> UPDATE 			
			/*
			pDataMgr->InitDBFieldData () ;
			pDataMgr->AddDBFieldData(_T("TOC_COMMON_KEY"),_T("NUMERIC"),m_strCommonRecKey);
			ids = pDataMgr->MakeUpdateFrame (_T("BO_SPECIES_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strSpeciesKey);
			if(ids<0) {
				AfxMessageBox(_T("MakeUpdateFrame(BO_SPECIES_TBL) Error~!"));
				return ;
			}
			*/
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			
		}

		// 2.2. �����ϰų� �����Ѵ�.
		if(nType==0){
			if(m_strConKey==_T("")){ 
				if(pDataMgr->MakeRecKey (szRecKey))goto ERR_RETURN; 
			}else{
				szRecKey = m_strConKey.GetBuffer(0);
			}
			// Set 'CONTENTS_YN' 
			if(m_strSpeciesKey!=_T("")){
			/*	
				pDataMgr->InitDBFieldData();
				pDataMgr->AddDBFieldData(_T("KOLIS_CON_YN"),_T("STRING"),_T("Y"));
				ids = pDataMgr->MakeUpdateFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey);
				if(ids <0 )goto ERR_RETURN;
			*/
			}

		}else{
			if(m_strAbsKey==_T("")){
				if(pDataMgr->MakeRecKey(szRecKey))goto ERR_RETURN; 
			}else{
				szRecKey = m_strAbsKey.GetBuffer(0);
			}
			// Set 'ABSTRACTS_YN' 
			if(m_strSpeciesKey!=_T("")){
			/*(
				pDataMgr->InitDBFieldData();
				pDataMgr->AddDBFieldData(_T("KOLIS_ABS_YN"),_T("STRING"),_T("Y"));
				ids = pDataMgr->MakeUpdateFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey);
				if(ids <0 )goto ERR_RETURN;
			*/
			}
		}

		// 2.3. ����/�ʷ� ������ ���̺� ������ �ִ´�.
		pDataMgr->InitDBFieldData();
		if(nType==0){
			if(m_strConKey==_T("")){
				pDataMgr->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),szRecKey);
				pDataMgr->AddDBFieldData(_T("CREATE_DATE"),_T("STRING"),system_date);
				m_strCCreateDate=system_date;
			}
			pDataMgr->AddDBFieldData (_T("TOC_COMMON_KEY"),_T("NUMERIC"),m_strCommonRecKey);
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//2003.10.24 ������ ���� 
			//' '�ο��ȣ ������� PARAMETER :true �߰� 
			pDataMgr->AddDBFieldData(_T("CONTENTS"),_T("STRING"),strConAbs,true);
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			pDataMgr->AddDBFieldData(_T("UPDATE_DATE"),_T("STRING"),system_date);
			m_strCUpdateDate=system_date;
			// 2009.09.16 ADD BY PWR : �����ʷ� ����� MANAGE_CODE�߰�
			pDataMgr->AddDBFieldData(_T("MANAGE_CODE"),_T("STRING"),m_pInfo->MANAGE_CODE);
		}else{
			if(m_strAbsKey==_T("")){
				pDataMgr->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),szRecKey);
				pDataMgr->AddDBFieldData(_T("CREATE_DATE"),_T("STRING"),system_date);
				m_strACreateDate=system_date;
			}
			pDataMgr->AddDBFieldData (_T("TOC_COMMON_KEY"),_T("NUMERIC"),m_strCommonRecKey);
		     //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//2003.10.24 ������ ���� 
			//' '�ο��ȣ ������� PARAMETER :true �߰� 
			pDataMgr->AddDBFieldData(_T("ABSTRACTS"),_T("STRING"),strConAbs,true);
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			pDataMgr->AddDBFieldData(_T("UPDATE_DATE"),_T("STRING"),system_date);
			m_strAUpdateDate=system_date;
		}
		
		if(nType==0)
		{
			if(m_strConKey==_T("")){
				ids = pDataMgr->MakeInsertFrame(_T("BO_CONTENTS_TBL")) ;
				if(ids <0)goto ERR_RETURN;

				pDataMgr->InitDBFieldData();
				pDataMgr->AddDBFieldData(_T("CONTENTS_KEY"),_T("NUMERIC"),szRecKey);
				ids  = pDataMgr->MakeUpdateFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey);
				if(ids <0)goto ERR_RETURN;			
			}else{
				ids = pDataMgr->MakeUpdateFrame(_T("BO_CONTENTS_TBL"),_T("REC_KEY"),_T("NUMERIC"),szRecKey) ;
				if(ids <0 )goto ERR_RETURN;			
			}

			// 2006.01.14 ADD BY PDJ
			// ���̳� �ڷ� ó��	: ���̳��� �߰��� ��´�.
			if ( m_strMatCode==_T("MI") && m_str90==_T("Y") )
			{
				m_strConKey=szRecKey;
				MakeSemiCollection(	0, pDataMgr, strConAbs );	
			}
		} 
		else
		{
			if(m_strAbsKey==_T("")){
				ids = pDataMgr->MakeInsertFrame (_T("BO_ABSTRACTS_TBL")) ;
				if(ids <0)goto ERR_RETURN;		

				pDataMgr->InitDBFieldData();
				pDataMgr->AddDBFieldData(_T("ABSTRACTS_KEY"),_T("NUMERIC"),szRecKey);
				ids  = pDataMgr->MakeUpdateFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey) ;
				if(ids <0 )goto ERR_RETURN;
			}else{
				ids = pDataMgr->MakeUpdateFrame(_T("BO_ABSTRACTS_TBL"),_T("REC_KEY"),_T("NUMERIC"),szRecKey) ;
				if(ids <0)goto ERR_RETURN;			
			}
		}
		// 2.4 �˻� ���� �۾����� ��� �������� DB�� ��ġ ��Ų��.
		if(nType==0)
		{
			m_strConYN=_T("Y");
			m_strCSource=strConAbs;
			m_strConKey=szRecKey;
		}
		else
		{
			m_strAbsYN=_T("Y");
			m_strASource=strConAbs;
			m_strAbsKey=szRecKey;
		}

		////////////////////////////////////////////////////////////
		// �˻� ���� �۾�
		////////////////////////////////////////////////////////////
		// ����,�ʷ��� ó���Է��̸� insert
		ids = InsertIndexTable(pDataMgr,nType) ;
		if(ids == 100)
		{
			pDataMgr->EndFrame () ;
			return ;
		}
	
		ids = pDataMgr->SendFrame ();
		if(ids <0)goto ERR_RETURN;
		
		pDataMgr ->EndFrame () ;
		
        
	}
	      
	else  //���� ///////////////////////////////////////////////////////////////
	{
		AfxMessageBox(_T("����ȭ�� �Դϴ�.")) ;
		if(ids == 100)
		{
			pDataMgr->EndFrame () ;
			return ;
		}
	}

    ////////////////////////////////////////////////////////////
	// Saved �����̴�. !!!
	////////////////////////////////////////////////////////////
	
	// ȭ�� �۾�
	if(nType==0){


		if(SetDataMgrData(m_ParentDM,_T("BS_��������"),_T("Y"),m_nDataRowIdx))goto ERR_RETURN;
		if(m_nDlgType==1){
			if(SetDataMgrData(m_ParentDM,_T("BS_��������"),_T("Y"),m_nDataRowIdxForDlgType1))goto ERR_RETURN;
		}
		SetDlgItemText(IDC_STATIC_01,_T("<�� �� : ����>"));
		GetDlgItem(IDC_bCDELETE)->EnableWindow(true);
		MessageBox(_T("������ �����߽��ϴ�."));
        /* 2003�� 6�� 25�� ���� 
		CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM , m_ParentGrid );
		pGrid->SetAt (m_idx,6,_T("Y")) ;
        */
		//2006-03-08 Ȳ���� ����
		CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM , m_ParentGrid );
		pGrid->SetAt (m_idx,6,_T("Y")) ;
		pGrid->m_bProtectSelectColumn = TRUE;
		AllControlDisplay (m_ParentCM ,-1) ;
		pGrid->m_bProtectSelectColumn = FALSE;


	}else{
		
		if(SetDataMgrData(m_ParentDM,_T("BS_�ʷ�����"),_T("Y"),m_nDataRowIdx))goto ERR_RETURN;
		if(m_nDlgType==1){
			if(SetDataMgrData(m_ParentDM,_T("BS_�ʷ�����"),_T("Y"),m_nDataRowIdxForDlgType1))goto ERR_RETURN;
		}
		SetDlgItemText(IDC_STATIC_02,_T("<�� �� : ����>"));
 		GetDlgItem(IDC_bADELETE)->EnableWindow(true);
		MessageBox(_T("�ʷ��� �����߽��ϴ�."));  
		//2006-03-08 Ȳ���� ����
		CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM , m_ParentGrid );
		pGrid->SetAt (m_idx,7,_T("Y")) ;
		pGrid->m_bProtectSelectColumn = TRUE;
		AllControlDisplay (m_ParentCM ,-1) ;
		pGrid->m_bProtectSelectColumn = FALSE;
	}

	return;
	

ERR_RETURN:	// �������� ���
	
/*	

	ECO_SetServerType(CS);
	if(pDataMgr)pDataMgr->ExecuteRollback();
	ECO_EndFrame();

	AfxMessageBox(_T("�ڷ����忡 �����Ͽ����ϴ�."));
*/
  Init(0);

  return;
} 

VOID CCONTENTMGR_02::OnShowViewer()
{
	m_TocEditdlg.ShowViewer (TRUE, FALSE) ;
//	m_TocEditorDlg.ShowViewer(TRUE, FALSE) ;
}


//nType 0 : ���� , 1 : �ʷ� 
INT CCONTENTMGR_02::InsertIndexTable( CESL_DataMgr *pTempMgr ,INT nType )
{
    INT ids ;
	CMakeSearchData *pMakeData = NULL;
	pMakeData = new CMakeSearchData(this);
    CString sBody;
		// Syntex Check and Get Values

    CESL_DataMgr *IndexDM_Mgr = FindDM(_T("DM_CONTENTMGR_INDEX")) ;
	if(IndexDM_Mgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		delete pMakeData;
		return -1;
	}

    CString sWhere ,check_TOC_YN;

    if(nType==0)
	{
        sBody=m_TocEditdlg.GetFinalToc();
        if(m_TocEditdlg.CheckSyntax()) 
		{
			delete pMakeData;
			return -1;
		}
    }
    else 
	{
        sBody=m_ChorokDlg.GetChorokText();
	}
	/////////////////////////////////////////////////////////////////////
	//�ε��� Ű���� ���� �ϱ� 
	/////////////////////////////////////////////////////////////////////
	MakeIndexKeyWord(sBody) ;

	INT iCh = pMakeData->ChangeHanjaToHangul( sBody );	//���� �ѱ� ��ȯ
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
		delete pMakeData;
		return -1;
	}
	LPCTSTR lpszTok = _tcstok(sBody.GetBuffer(0), _T(" "));
	CString tmp_Data;
	CString strKeyWord;
	
	while (lpszTok)
	{   
		tmp_Data.Format (_T("%s"),lpszTok);
		iCh = pMakeData->GetKeyWord( tmp_Data );				//����, Ư������ ����
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
			delete pMakeData;
			return -2;
		}

		strKeyWord +=  _T(" ") + tmp_Data;

		lpszTok = _tcstok(NULL, _T(" "));
	}

	// ��� ����
	delete pMakeData;

	strKeyWord.TrimLeft () ;
    sBody = strKeyWord ;
    

	if(m_KeyWord_update == TRUE)
    sBody = m_strUpdateKeyword ;
    
    
	if(m_KeyWord_update == TRUE)
    sBody = m_strUpdateKeyword ;


    if(nType==0)
	{
		 CString tmp_Body ;
         ((CEdit*)GetDlgItem(IDC_eMOK_KEYWORD))->GetWindowText(tmp_Body) ;
		 if(sBody != tmp_Body)
			 sBody = tmp_Body ;
	}
	else
	{
         CString tmp_Body ;
         ((CEdit*)GetDlgItem(IDC_eCHO_KEYWORD))->GetWindowText(tmp_Body) ;
		 if(sBody != tmp_Body)
			 sBody = tmp_Body ;
	}     


    //m_nDataRowIdx,m_ParentDM 
   
    //m_nDlgType==0 : //�Է�(��������)
	//m_nDlgType==1 : //����(���� �����ͻ������.);
   
    CString  v_SPECIES_KEY,v_SE_SHELF_CODE,v_CLASS_NO ,v_BOOK_CODE,v_VOL_CODE_DISP,v_TITLE_INFO, v_AUTHOR_INFO, v_PUB_INFO, v_PUB_YEAR_INFO ,v_PLACE_INFO;
    CString  v_RecKey , v_SPECIES_CLASS ,v_TOC_USE_LIMIT_CODE , v_ABS_USE_LIMIT_CODE;
	CString  v_TOC_CREATE_DATE ,v_TOC_UPDATE_DATE, v_ABS_CREATE_DATE ,v_ABS_UPDATE_DATE;
	CString  v_TOC_PRESENT, v_ABS_PRESENT;
    
    CTime t = CTime::GetCurrentTime () ;
	CString systemDate ;
	systemDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
    

	//�Է�ȭ�� �� ��� //////////////////////////////////////////////////////////////////////////
	//if(m_nDlgType == 0)
	//{

		CESL_DataMgr *pDM = FindDM(m_ParentDM );
		if(pDM == NULL) {
			AfxMessageBox(_T("FindDM() Error~!")) ;
		}        
		pDM->GetCellData (_T("IBS_��KEY"),m_nDataRowIdx,v_SPECIES_KEY );
        pDM->GetCellData (_T("��ġ��ȣ"),m_nDataRowIdx,v_SE_SHELF_CODE );
		pDM->GetCellData (_T("�з���ȣ"),m_nDataRowIdx,v_CLASS_NO  );
		pDM->GetCellData (_T("������ȣ"),m_nDataRowIdx,v_BOOK_CODE );
        
		pDM->GetCellData (_T("��å��ȣ���"),m_nDataRowIdx,v_VOL_CODE_DISP );
        pDM->GetCellData (_T("ǥ������"),m_nDataRowIdx,v_TITLE_INFO );
		pDM->GetCellData (_T("�����ڻ���"),m_nDataRowIdx,v_AUTHOR_INFO  );
        pDM->GetCellData (_T("�����ڻ���"),m_nDataRowIdx,v_PUB_INFO  );
		pDM->GetCellData (_T("����⵵����"),m_nDataRowIdx,v_PUB_YEAR_INFO );
        pDM->GetCellData (_T("�ڷ��ִ°�"),m_nDataRowIdx,v_PLACE_INFO  );  
		pDM->GetCellData (_T("�����а�"),m_nDataRowIdx,v_SPECIES_CLASS  );

		// 2009.09.16 ADD BY PWR : ����, �ʷ� �����߰� (CREATE_DATE�� ������ UPDATE_DATE�� ������ ������ ����)
		pDM->GetCellData(_T("BS_��������"),m_nDataRowIdx,v_TOC_PRESENT);
		pDM->GetCellData(_T("BS_�ʷ�����"),m_nDataRowIdx,v_ABS_PRESENT);
		
		sWhere.Format (_T(" REC_KEY = %s"),m_strCommonRecKey) ;
		IndexDM_Mgr->RefreshDataManager (sWhere);	

        if(nType == 0 )
		{
		    GetControlData (_T("CM_CONTENTMGR_02"),_T("�����̿����ѱ���"),v_TOC_USE_LIMIT_CODE) ;
            v_TOC_CREATE_DATE = systemDate ;
		}
		else
		{
        	GetControlData (_T("CM_CONTENTMGR_02"),_T("�ʷ��̿����ѱ���"),v_ABS_USE_LIMIT_CODE) ;
			v_ABS_CREATE_DATE = systemDate ;
		}
          
		// 2006.1.3 ADD BY PDJ
		// �Է������� ������ �ʷ��� ���߿� �ԷµǴ� ��� ��������̴�.
		BOOL bInsert;
		bInsert = FALSE;
		if ( IndexDM_Mgr->GetRowCount() <= 0 )
		{
			bInsert = TRUE;						
		}
	
        pTempMgr->InitDBFieldData () ;       
		if ( bInsert )
		{
			// �Է¸���̸�
			pTempMgr->AddDBFieldData (_T("REC_KEY") ,_T("NUMERIC"),m_strCommonRecKey) ;			
			if(nType == 0 ) 
			{
				pTempMgr->AddDBFieldData (_T("TOC_CREATE_DATE") ,_T("STRING"),v_TOC_CREATE_DATE) ;			
			}
			else
			{
				pTempMgr->AddDBFieldData (_T("ABS_CREATE_DATE") ,_T("STRING"),v_ABS_CREATE_DATE) ;   			
			}
		}		
		else
		{
			// 2009.09.16 UPDATE BY PWR : ������ ����, �ʷ� ������ �Է��� �� �ִ�. �Է��̸� CREATE_DATE�� �ԷµǾ���Ѵ�.
			if(nType == 0 )
			{
				if(_T("Y") == v_TOC_PRESENT)
				{
					pTempMgr->AddDBFieldData (_T("TOC_UPDATE_DATE"),_T("STRING"),v_TOC_CREATE_DATE);
				}
				else
				{
					pTempMgr->AddDBFieldData (_T("TOC_CREATE_DATE"),_T("STRING"),v_TOC_CREATE_DATE);
				}
			}
			else
			{
				if(_T("Y") == v_ABS_PRESENT)
				{
					pTempMgr->AddDBFieldData (_T("ABS_UPDATE_DATE"),_T("STRING"),v_ABS_CREATE_DATE);
				}
				else
				{
					pTempMgr->AddDBFieldData (_T("ABS_CREATE_DATE"),_T("STRING"),v_ABS_CREATE_DATE);
				}
			}
// 			if(nType == 0 )
// 			{
// 				pTempMgr->AddDBFieldData (_T("TOC_UPDATE_DATE") ,_T("STRING"),v_TOC_CREATE_DATE) ;			
// 			}
// 			else
// 			{
// 				pTempMgr->AddDBFieldData (_T("ABS_UPDATE_DATE") ,_T("STRING"),v_ABS_CREATE_DATE) ;   			
// 			}
		}

		if(nType == 0 )
		{   
			// �����϶�			
			pTempMgr->AddDBFieldData (_T("IDX_TOC_KEYWORD") ,_T("STRING"),sBody) ;			
			pTempMgr->AddDBFieldData (_T("TOC_YN"),_T("STRING"),_T("Y"));
			pTempMgr->AddDBFieldData (_T("TOC_USE_LIMIT_CODE") ,_T("STRING"),v_TOC_USE_LIMIT_CODE) ;            
			pTempMgr->AddDBFieldData (_T("CONTENTS_KEY") ,_T("NUMERIC"),m_strConKey ) ;
		}
		else
		{
			// �ʷ��϶�			
			pTempMgr->AddDBFieldData (_T("IDX_ABS_KEYWORD") ,_T("STRING"),sBody) ;			
			pTempMgr->AddDBFieldData (_T("ABS_YN"),_T("STRING"),_T("Y"));
			pTempMgr->AddDBFieldData (_T("ABS_USE_LIMIT_CODE") ,_T("STRING"),v_ABS_USE_LIMIT_CODE) ;
			pTempMgr->AddDBFieldData (_T("ABSTRACTS_KEY") ,_T("NUMERIC"),m_strAbsKey  );			
		}
        
		ids =   pTempMgr->AddDBFieldData (_T("SPECIES_KEY") ,_T("NUMERIC"),v_SPECIES_KEY  ) ;
		ids =   pTempMgr->AddDBFieldData (_T("SE_SHELF_CODE") ,_T("STRING"),v_SE_SHELF_CODE) ;
		ids =	pTempMgr->AddDBFieldData (_T("CLASS_NO") ,_T("STRING"),v_CLASS_NO) ;
		ids =	pTempMgr->AddDBFieldData (_T("BOOK_CODE") ,_T("STRING"),v_BOOK_CODE) ;
		ids =	pTempMgr->AddDBFieldData (_T("VOL_CODE_DISP") ,_T("STRING"),v_VOL_CODE_DISP) ;
		ids =	pTempMgr->AddDBFieldData (_T("TITLE_INFO") ,_T("STRING"),v_TITLE_INFO) ;
		ids =	pTempMgr->AddDBFieldData (_T("PUB_INFO") ,_T("STRING"),v_PUB_INFO) ;
		ids =	pTempMgr->AddDBFieldData (_T("PUB_YEAR_INFO") ,_T("STRING"),v_PUB_YEAR_INFO) ;
		ids =	pTempMgr->AddDBFieldData (_T("PLACE_INFO") ,_T("STRING"),v_PLACE_INFO) ;
		ids =	pTempMgr->AddDBFieldData (_T("SPECIES_CLASS") ,_T("STRING"),v_SPECIES_CLASS) ;
		// 2009.09.16 ADD BY PWR : ������ ���� �߰�
		ids =	pTempMgr->AddDBFieldData (_T("AUTHOR_INFO") ,_T("STRING"),v_AUTHOR_INFO) ;
       
		if ( bInsert )
		{
			pTempMgr->MakeInsertFrame (_T("IDX_BO_TOC_TBL"));
		}
		else
		{
			pTempMgr->MakeUpdateFrame (_T("IDX_BO_TOC_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey) ;					
		}
        
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.11.12 ������ �߰� 
		if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{   
			CString strData ;
			strData.Format(_T("ECO$P_MAKE_BOTOC$I(%s);"), m_strCommonRecKey);
			pTempMgr->AddFrame(strData);
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//}

	return 0 ;
}



INT CCONTENTMGR_02::UpdateIndexTable(CESL_DataMgr *pTempMgr, INT nType)
{

  
	CMakeSearchData *pMakeData = NULL;
    
	pMakeData = new CMakeSearchData(this); 


    CString sBody;
		// Syntex Check and Get Values
   
    CESL_DataMgr *IndexDM_Mgr = FindDM(_T("DM_CONTENTMGR_INDEX")) ;
	if(IndexDM_Mgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1;
	}

    CString sWhere ,check_TOC_YN;

    if(nType==0)
	{
        sBody=m_TocEditdlg.GetFinalToc();
        if(m_TocEditdlg.CheckSyntax())return -1;
    }
    else 
	{
        sBody=m_ChorokDlg.GetChorokText();
	}
	
	/////////////////////////////////////////////////////////////////////
	//�ε��� Ű���� ���� �ϱ� 
	/////////////////////////////////////////////////////////////////////
	INT iCh = pMakeData->ChangeHanjaToHangul( sBody );	//���� �ѱ� ��ȯ
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
		return -1;
	}

	MakeIndexKeyWord(sBody) ;

	LPCTSTR lpszTok = _tcstok(sBody.GetBuffer(0), _T(" "));
	CString tmp_Data;
	CString strKeyWord;
	
	while (lpszTok)
	{   
		tmp_Data.Format (_T("%s"),lpszTok);
		iCh = pMakeData->GetKeyWord( tmp_Data );				//����, Ư������ ����
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
			return -2;
		}

		strKeyWord +=  _T(" ") + tmp_Data;

		lpszTok = _tcstok(NULL, _T(" "));
	}

	strKeyWord.TrimLeft () ;
    sBody = strKeyWord ;

	if(m_KeyWord_update == TRUE)
    sBody = m_strUpdateKeyword ;


    if(nType==0)
	{
		 CString tmp_Body ;
         ((CEdit*)GetDlgItem(IDC_eMOK_KEYWORD))->GetWindowText(tmp_Body) ;
		 if(sBody != tmp_Body)
			 sBody = tmp_Body ;
	}
	else
	{
         CString tmp_Body ;
         ((CEdit*)GetDlgItem(IDC_eCHO_KEYWORD))->GetWindowText(tmp_Body) ;
		 if(sBody != tmp_Body)
			 sBody = tmp_Body ;
	}     

    //m_nDataRowIdx,m_ParentDM 
   
    //m_nDlgType==0 : //�Է�(��������)
	//m_nDlgType==1 : //����(���� �����ͻ������.);
   
    CString  v_SPECIES_KEY,v_SE_SHELF_CODE,v_CLASS_NO ,v_BOOK_CODE,v_VOL_CODE_DISP,v_TITLE_INFO, v_AUTHOR_INFO, v_PUB_INFO, v_PUB_YEAR_INFO ,v_PLACE_INFO ;
    CString  v_RecKey , v_SPECIES_CLASS ,v_TOC_USE_LIMIT_CODE , v_ABS_USE_LIMIT_CODE;
	CString  v_TOC_CREATE_DATE ,v_TOC_UPDATE_DATE, v_ABS_CREATE_DATE ,v_ABS_UPDATE_DATE    ;
    
    CTime t = CTime::GetCurrentTime () ;
	CString systemDate ;
	systemDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());

    
	CESL_DataMgr *pDM = FindDM(m_ParentDM );
	if(pDM == NULL) {
	AfxMessageBox(_T("FindDM() Error~!")) ;
	}        
	pDM->GetCellData (_T("IBS_��KEY"),m_nDataRowIdx,v_SPECIES_KEY );

    
    
	if(nType == 0 )
	{
		GetControlData (_T("CM_CONTENTMGR_02"),_T("�����̿����ѱ���"),v_TOC_USE_LIMIT_CODE) ;
		v_TOC_UPDATE_DATE = systemDate ;
	}
	else
	{
		sWhere.Format (_T(" SPECIES_KEY = %s"),v_SPECIES_KEY) ;
		IndexDM_Mgr->RefreshDataManager (sWhere);
	 /*
		IndexDM_Mgr->GetCellData (_T("TOC_YN"),0,check_TOC_YN);
		if(!(check_TOC_YN == _T("Y")))
		{
			AfxMessageBox(_T("������ ���� �����Ͻ��� �۾��� ���� �Ͻñ� �ٶ��ϴ�.")) ;
			return 100 ;
		}
	 */
		GetControlData (_T("CM_CONTENTMGR_02"),_T("�ʷ��̿����ѱ���"),v_ABS_USE_LIMIT_CODE) ;
		v_ABS_UPDATE_DATE = systemDate ;
	}

    
	pTempMgr->InitDBFieldData () ;
	
	if(nType == 0 )
	{   
	//	pTempMgr->AddDBFieldData (_T("REC_KEY") ,_T("NUMERIC"),v_RecKey) ;
		pTempMgr->AddDBFieldData (_T("IDX_TOC_KEYWORD") ,_T("STRING"),sBody) ;
		pTempMgr->AddDBFieldData (_T("TOC_UPDATE_DATE") ,_T("STRING"),v_TOC_UPDATE_DATE) ;
		pTempMgr->AddDBFieldData (_T("TOC_USE_LIMIT_CODE") ,_T("STRING"),v_TOC_USE_LIMIT_CODE) ;
		
	}
	else
	{
		pTempMgr->AddDBFieldData (_T("IDX_ABS_KEYWORD") ,_T("STRING"),sBody) ;
		pTempMgr->AddDBFieldData (_T("ABS_UPDATE_DATE") ,_T("STRING"),v_ABS_UPDATE_DATE) ;  
		pTempMgr->AddDBFieldData (_T("ABS_USE_LIMIT_CODE") ,_T("STRING"),v_ABS_USE_LIMIT_CODE) ;
		
		pTempMgr->MakeUpdateFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),v_SPECIES_KEY) ;
		
		return 0 ;
	}

    pTempMgr->MakeUpdateFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),v_SPECIES_KEY) ;
    
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003.11.12 ������ �߰� 
	CString strQuery ;
	//CString v_RecKey ;
	strQuery.Format (_T("SELECT REC_KEY FROM IDX_BO_TOC_TBL WHERE SPECIES_KEY = %s "),v_SPECIES_KEY );
	pTempMgr->GetOneValueQuery (strQuery,v_RecKey);
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{   
		CString strData ;
		strData.Format(_T("ECO$P_MAKE_BOTOC$I(%s);"), v_RecKey);
		pTempMgr->AddFrame(strData);
	}
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	delete pMakeData;

	return 0 ;
}

INT CCONTENTMGR_02::MakeIndexKeyWord(CString &tmpValue)
{	
    
	tmpValue.TrimLeft () ; tmpValue.TrimRight () ;

	INT first,end ;
	first = tmpValue.Find (_T("<body>")) ;
    end   = tmpValue.Find (_T("</body>"));
    
	if(first<0)
	{
		tmpValue.Replace(_T("\r\n"),_T(" "));
		return 0;
	}
    
    tmpValue = tmpValue.Mid (first+6,end-first-6) ;

	// 2005.12.09 ADD BY PDJ
	// ���� ��ȣ�� Ű���忡�� ����
	// '/' ���� ����
	CString sBodyTemp;
	CString strNewBody;
	CString strLine;
	CString strChar;
	
	INT		nOffSet;	
	INT		nPos;
	INT		nLength;
	INT		i;
	BOOL	bNumFlag;

	sBodyTemp = tmpValue;

	nOffSet = 0;
	nPos = 0;
	strNewBody = _T("");
	while ( nOffSet >= 0)
	{
		nOffSet = sBodyTemp.Find(_T("\r\n"), nPos );		
		if ( nOffSet == -1 )
		{
			strLine = sBodyTemp.Mid(nPos);
		}
		else
		{
			strLine = sBodyTemp.Mid(nPos, nOffSet-nPos);
		}		

		bNumFlag = FALSE;
		// ����, ������ ����
		nLength = strLine.Find(_T("/"));
		if ( nLength < 0 ) 
		{
			nLength = strLine.GetLength();			
		}
		else
		{
			strLine = strLine.Left(nLength);
		}

		// ���ڰ� ���� ��� ������ ����
		nLength = strLine.Find(_T("="));
		if ( nLength < 0 ) 
		{
			nLength = strLine.GetLength();			
		}
		else
		{
			strLine = strLine.Left(nLength);
		}

		for ( i=0 ; i<nLength ; i++ )
		{
			strChar = strLine.Mid(i, 1);
			if ( strChar >= '0' && strChar <= '9' )
			{
				bNumFlag = TRUE;
				continue;
			}
			else
			{
				if ( bNumFlag == TRUE )
				{
					if ( strChar == _T(".") || strChar == _T(" ") )
					{
						strLine = strLine.Mid(i+1);
						break;
					}
					else
					{
						bNumFlag = FALSE;
						break;

					}
				}
				else
				{
					bNumFlag = FALSE;
					break;
				}				
			}			
		}			
		
		strLine.TrimLeft(_T(" "));
		strLine.TrimRight(_T(" "));
		nPos = nOffSet+2;
		strNewBody += strLine + _T("\r\n");
	}

	strNewBody.TrimRight(_T("\r\n"));	
	strNewBody.Replace(_T("\r\n"),_T(" "));	

	strNewBody.MakeUpper();

	// Ű���� �� �����ϱ�
	nPos = 0;
	nOffSet = 0;
	sBodyTemp = _T("");
	while(nOffSet >= 0)
	{
		nOffSet = strNewBody.Find(_T(" "), nPos );		
		if ( nOffSet == -1 )
		{
			strLine = strNewBody.Mid(nPos);
		}
		else
		{
			strLine = strNewBody.Mid(nPos, nOffSet-nPos);
		}
		

		strLine.TrimLeft(_T(" "));
		strLine.TrimRight(_T(" "));
		
		strChar = KeywordMake(strLine);
		if ( !strChar.IsEmpty() )
		{
			sBodyTemp += strChar + _T(" ");
		}

		nPos = nOffSet+1;
	}

	tmpValue = sBodyTemp;

	return 0 ;
}


VOID CCONTENTMGR_02::Delete(INT nType)
{
    
	INT ids ;

	CESL_DataMgr *pDM = FindDM(_T("DM_�����ʷ�"));
	if(pDM == NULL){
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;

	}

	CString sQuery ,tmp_Common_Key ;
    
	sQuery.Format (_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s"),m_strSpeciesKey); 
	pDM->GetOneValueQuery (sQuery,tmp_Common_Key) ;
    

	pDM->StartFrame ();

	if(nType == 0) // ���� ����
	{    
		pDM->InitDBFieldData ();
		pDM->MakeDeleteFrame (_T("BO_CONTENTS_TBL"),_T("TOC_COMMON_KEY"),_T("NUMERIC"),tmp_Common_Key);

		pDM->InitDBFieldData ();
	    pDM->MakeDeleteFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.11.12 ������ �߰� 
		CString strQuery;
		CString v_RecKey;
		strQuery.Format (_T("SELECT REC_KEY FROM IDX_BO_TOC_TBL WHERE SPECIES_KEY = %s "),m_strSpeciesKey );
		pDM->GetOneValueQuery (strQuery,v_RecKey);
		if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{   
			CString strData ;
			strData.Format(_T("ECO$P_DELETE_BOTOC$I(%s);"), v_RecKey);
			pDM->AddFrame(strData);
		}
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		pDM->AddDBFieldData (_T("TOC_YN"),_T("STRING"),_T("N"));
		pDM->AddDBFieldData (_T("CONTENTS_KEY"),_T("NUMERIC"),_T("")) ;
		pDM->AddDBFieldData (_T("TOC_USE_LIMIT_CODE"),_T("STRING"),_T(""));
		pDM->AddDBFieldData (_T("IDX_TOC_KEYWORD"),_T("STRING"),_T("")) ;
		pDM->AddDBFieldData (_T("TOC_CREATE_DATE"),_T("STRING"),_T(""));
		pDM->AddDBFieldData (_T("TOC_UPDATE_DATE"),_T("STRING"),_T(""));
		
		pDM->MakeUpdateFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
		*/

		pDM->InitDBFieldData ();
		pDM->AddDBFieldData (_T("CONTENTS_YN"),_T("STRING"),_T("N")) ;
		pDM->MakeUpdateFrame (_T("IDX_BO_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strSpeciesKey) ;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.11.3 ������ �߰� 
		// IDX_BO_TBL UPDATE �� IDX_TMP_BO_TBL �Է��ϱ� 
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		pDM->InitDBFieldData ();
// 		
// 		CString strTmp_Key ;
// 		CString system_Date ;
// 		CTime t = CTime::GetCurrentTime ();
// 		system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
// 		
// 		pDM->MakeRecKey (strTmp_Key) ;
// 		pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
// 		pDM->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
// 		pDM->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("I"));
// 		pDM->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);		
// 		ids = pDM->MakeInsertFrame (_T("IDX_TMP_BO_TBL"));
// 		if(ids<0) {
// 			AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_BO_TBL) Error~!"));
// 			return ;
// 		}
		//=====================================================
		
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2006.01.14 ADD BY PDJ
		// ���̳� �ڷ� ó��	: ���̳��� �߰��� ��´�.
		if ( m_strMatCode==_T("MI") && m_str90==_T("Y") )
		{			
			// ���� ���� ����			
			// DELETE 
			MakeSemiCollection(1, pDM);
		}

		m_strConYN = _T("N") ;

		ids = SetDataMgrData(m_ParentDM ,_T("BS_��������"),_T("N"),m_idx) ;

		SetDlgItemText(IDC_STATIC_01,_T("<�� �� : ����>"));
		GetDlgItem(IDC_bCDELETE)->EnableWindow(false);
		MessageBox(_T("������ �����߽��ϴ�."));

		CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM , m_ParentGrid );
		pGrid->SetAt (m_idx,6,_T("N")) ;

        
	}
    else  //�ʷ� ���� 
	{

        pDM->InitDBFieldData ();
		pDM->MakeDeleteFrame (_T("BO_ABSTRACTS_TBL"),_T("TOC_COMMON_KEY"),_T("NUMERIC"),tmp_Common_Key);
        /*
		pDM->InitDBFieldData ();
	    pDM->MakeDeleteFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
		*/
		/*
		pDM->AddDBFieldData (_T("TOC_YN"),_T("STRING"),_T("N"));
		pDM->AddDBFieldData (_T("CONTENTS_KEY"),_T("NUMERIC"),_T("")) ;
		pDM->AddDBFieldData (_T("TOC_USE_LIMIT_CODE"),_T("STRING"),_T(""));
		pDM->AddDBFieldData (_T("IDX_TOC_KEYWORD"),_T("STRING"),_T("")) ;
		pDM->AddDBFieldData (_T("TOC_CREATE_DATE"),_T("STRING"),_T(""));
		pDM->AddDBFieldData (_T("TOC_UPDATE_DATE"),_T("STRING"),_T(""));
		
		pDM->MakeUpdateFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
		*/

		pDM->InitDBFieldData ();
		pDM->AddDBFieldData (_T("ABSTRACTS_YN"),_T("STRING"),_T("N")) ;
		pDM->MakeUpdateFrame (_T("IDX_BO_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strSpeciesKey) ;

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.11.3 ������ �߰� 
		// IDX_BO_TBL UPDATE �� IDX_TMP_BO_TBL �Է��ϱ� 
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		pDM->InitDBFieldData ();
// 		
// 		CString strTmp_Key ;
// 		CString system_Date ;
// 		CTime t = CTime::GetCurrentTime ();
// 		system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
// 		
// 		pDM->MakeRecKey (strTmp_Key) ;
// 		pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
// 		pDM->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
// 		pDM->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("I"));
// 		pDM->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);
// 		
// 		ids = pDM->MakeInsertFrame (_T("IDX_TMP_BO_TBL"));
// 		if(ids<0) {
// 			AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_BO_TBL) Error~!"));
// 			return ;
// 		}
		//=====================================================
		
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
		m_strAbsYN = _T("N") ;
        
        ids = SetDataMgrData(m_ParentDM ,_T("BS_�ʷ�����"),_T("N"),m_idx) ;

		SetDlgItemText(IDC_STATIC_02,_T("<�� �� : ����>"));
		GetDlgItem(IDC_bADELETE)->EnableWindow(false);
		MessageBox(_T("�ʷ��� �����߽��ϴ�."));

		CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM , m_ParentGrid );
		pGrid->SetAt (m_idx,7,_T("N")) ;
	}
    
	if((m_strConYN == _T("N")) && (m_strAbsYN == _T("N")))
	{
		pDM->InitDBFieldData ();
		pDM->MakeDeleteFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),tmp_Common_Key) ;

		
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //2003.11.3 ������ �߰� 
		//BO_SPECIES_TBL �� TOC_COMMON_KEY -> UPDATE 
        pDM->InitDBFieldData () ;
		//2003.11.12 ������ ���� 
		pDM->AddDBFieldData(_T("TOC_COMMON_KEY"),_T("NUMERIC"),_T("''"));
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		ids = pDM->MakeUpdateFrame (_T("BO_SPECIES_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strSpeciesKey);
		if(ids<0) {
		AfxMessageBox(_T("MakeUpdateFrame(BO_SPECIES_TBL) Error~!"));
		return ;
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}
    
   

    pDM->SendFrame ();
	pDM->EndFrame ();

}

VOID CCONTENTMGR_02::OnbADELETE() 
{
	// TODO: Add your control notification handler code here
	
    if(IDYES == AfxMessageBox(_T("�ʷ��� ���� �Ͻðڽ��ϱ� ?"),MB_YESNO,0) ) 
	{
	   Delete(1) ;
	   strOldAbstract = _T("");
	   m_ChorokDlg.SetChorok(_T(""));
	}
}
VOID CCONTENTMGR_02::OnbCDELETE() 
{
	// TODO: Add your control notification handler code here
//	AfxMessageBox(_T("������ ���� �Ͻðڽ��ϱ� ?"),MB_YESNO,0) ;
	
	if(IDYES == AfxMessageBox(_T("������ ���� �Ͻðڽ��ϱ� ?"),MB_YESNO,0) ) 
	{
	   Delete(0) ;
	   strOldContents = _T("");
	   m_TocEditdlg.SetTocFromString(_T(""));
	}
}

CString CCONTENTMGR_02::ReMake_BringInStream(INT nType,CString strStream)
{
	CString str_BringStream ;

	if(nType == 0)
	{  
         m_arrBringDefault[7] = _T("<body>����\r\n")+strStream+_T("\r\n</body>") ;
		 for(INT i=0 ;i<8;i++)
		 {
			str_BringStream += m_arrBringDefault[i] ;
		 }
         //�׽�Ʈ�� ���� �Ұ� .
		 //AfxMessageBox (str_BringStream) ;
		 
	}
	else
	{


	}

    
	return str_BringStream ;
	
}


INT CCONTENTMGR_02::DbClick_Move (INT ndir)
{
	CString strShow;   
	INT ids ;

	// Initialize
	CString strWhere;
	CString strTemp;
	
	CESL_DataMgr* pDataMgr;
	CESL_DataMgr* pDataContents;

    pDataContents = FindDM(_T("DM_�����ʷ�")) ;
	if(pDataContents == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}

	pDataMgr = FindDM(_T("DM_BO_CONTENTMGR_SPECIES"));
	if(pDataMgr == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -2 ;
	}

	CESL_ControlMgr *pParentCM = FindCM(m_ParentCM) ;
	if(pParentCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -3 ;
	}

	CESL_Grid *pGrid = (CESL_Grid*)pParentCM->FindControl(m_ParentGrid) ;
	if(pGrid == NULL)
	{
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return -4;
	}



	INT nIdx = -1;

    ids = pGrid->SelectMakeList () ;
	if (m_idx >= 0)
	{
		nIdx = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid);
		while (nIdx >= 0)
		{
			if (m_idx == nIdx) break;
			nIdx = pGrid->SelectGetNext();
		}
	}
    INT nCount ;
	
	nCount = pGrid->SelectGetCount () ;
	if(nCount >0)
	{
		switch (ndir)
		{
		case 0 :

			pGrid->SetAt (pGrid->GetIdx (),0,_T("V")) ;
			//pGrid->SelectMakeList();
			m_idx = pGrid->GetIdx () ;
      
			nIdx = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid);
			while (nIdx >= 0)
			{
				if (m_idx == nIdx) break;
				nIdx = pGrid->SelectGetNext();
			}
			if (m_idx < 0)
			{
				//2004-07-09 ������ 
				//AfxMessageBox(_T("���� ����� ������ �� �����ϴ�."));	// ����
				AfxMessageBox(_T("���� �׸��� �����ϴ�."));	// ����
				EndDialog(IDCANCEL);
				return -222;
			}
			break;
			
		case -1 :
		
			m_idx = SelectGetPrev (m_ParentCM ,m_ParentGrid);
			if (m_idx == -1) {
				m_idx = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid);
				AfxMessageBox(_T(" ó�� �ڷ��Դϴ�."));
				return -223;
			}
			break;
			
		case 1 :
		
			m_idx = SelectGetNext (m_ParentCM ,m_ParentGrid);
			if (m_idx == -1) {
				m_idx = SelectGetTailPosition(m_ParentCM ,m_ParentGrid);
				AfxMessageBox(_T(" ������ �ڷ��Դϴ�."));
				return -224;
			}
			break;
		}


	}
    else
	{

		switch (ndir)
		{
		case 0 :
				  m_idx = pGrid->GetIdx () ;
				 
			break;

		case -1 :
				  if(m_idx ==0 )
				  {
					  AfxMessageBox (_T("ó�� ��� �Դϴ�.")) ;
					  return - 333 ;
				  }

				  m_idx-- ;
				  
			break;
    
		case 1 :
				  if(m_idx+1 == pGrid->GetCount ())
				  {
					  AfxMessageBox (_T("������ ��� �Դϴ�.")) ;
					  return - 333 ;
				  }

				  m_idx++ ;


			break;
		}
	}

	   	// Member Initialize
	m_strCommonRecKey=_T("");  //���� �ʷ� ���������� ���ڵ�Ű(���ڵ� �������ε� �̰����� üũ)
	m_strCSource=_T("");	   //���� ����
	m_strASource=_T("");	   //�ʷ� ����
	m_strCCreateDate=_T("");   //���� ������
	m_strCUpdateDate=_T("");   //���� ������ 
	m_strACreateDate=_T("");   //�ʷ� ������
	m_strAUpdateDate=_T("");   //�ʷ� ������ 
	m_strMarc=_T("");	       //��ũ
	m_strFormCode=_T("");	   //���ı���
	m_strMatCode=_T("");	   //�ڷᱸ��
	m_strRegCode=_T("");	   //��ϱ���
	m_strUseLimit=_T("");	   //�̿����ѱ���
	m_strSpeciesKey=_T("");    //��Ű
	m_strControlNo=_T("");     //�����ȣ
//	m_strUbControlNo=_T("");   //.NET�����ȣ(key)
	m_strClassCode=_T("");	   //������
	m_strConYN=_T("");		   //��������
	m_strAbsYN=_T("");		   //��������
//	m_strNetConYN=_T("");	   //NET��������
//	m_strNetAbsYN=_T("");	   //NET�ʷ�����
	m_strLibCode=_T("");	   //��������ȣ 

	
	m_strConKey=_T("");	       //����Ű
	m_strAbsKey=_T(""); 	   //�ʷ�Ű

    //<-  �����ʷ� ���� �˻����� ����
    m_strWorkStatus;       //�۾�����
    m_strPlaceInfo;        //�ڷ��ִ°�
    //->  �����ʷ� ���� �˻����� ����
  
  //  m_idx = ids; 
    SetReverse (m_ParentCM ,m_ParentGrid ,m_idx) ;
      
    ////////////////////////////////////////////////////////////////////////////////////////////
	INT nTempIndex ;
	CString sMokcha_Check ;
	nTempIndex = m_idx ;

	GetDataMgrData(m_ParentDM,_T("��������"),sMokcha_Check,nTempIndex) ;

	if(sMokcha_Check == _T("Y"))
	{
         m_nDlgType = 1 ;
	}
	else
	{
         m_nDlgType = 0 ; 
	}


	//m_nDlgType==0 : //�Է�(��������)
	//m_nDlgType==1 : //����(���� �����ͻ������.);

    // 1.���� �ʷ��� ���� �´�. 2. ������ �����´�.

	if(m_nDlgType==0){

		m_nDataRowIdx = pGrid->GetIdx () ;

        GetDataMgrData (m_ParentDM ,_T("IBS_��KEY"),m_strSpeciesKey,m_nDataRowIdx );

		strWhere=_T(" toc.SPECIES_KEY=")+m_strSpeciesKey;

		INT ids = pDataContents ->RefreshDataManager (strWhere) ;

	}
	else
	{
		//���� ��Ű �������� 
      	
		m_nDataRowIdxForDlgType1 = pGrid->GetIdx () ;

        GetDataMgrData (m_ParentDM ,_T("IBS_��KEY"),m_strSpeciesKey,m_nDataRowIdxForDlgType1 ) ;

		strWhere=_T(" toc.SPECIES_KEY=")+m_strSpeciesKey;

		INT ids = pDataContents->RefreshDataManager (strWhere);
		if( pDataContents->GetRowCount () <= 0)
		{
		    AfxMessageBox(_T("���� �ʷ��� DB�� ���� ������ ���� �Ͻ� �� �����ϴ�.\n ���� ������ ���� �Ͻñ� �ٶ��ϴ�."));
			//return -1 ;
			OnCancel () ;
		}

      
    	GetDataMgrData (_T("DM_�����ʷ�"),_T("��Ű"),m_strSpeciesKey,0) ;//��Ű
		if(_ttoi(m_strSpeciesKey.GetBuffer (0))>0){
			/*
			strWhere=_T(" i.rec_key=")+m_strSpeciesKey;
			//ECO_SetServerType(LS);
			pDataMgr->RefreshDataManager (strWhere) ;
			*/
		}
		else
		{
		//	pDataMgr->FreeData () ;
		}
	
		if(pDataMgr->GetRowCount()>0){
			m_nDataRowIdx= pGrid->GetIdx ();
		}else{
			m_nDataRowIdx=-1;
		}
  
	}

     if(pDataContents->GetRowCount()>0){
		
		pDataContents->GetCellData (_T("���ڵ�Ű"),0,m_strCommonRecKey ) ;
		pDataContents->GetCellData (_T("����Ű"),0,m_strConKey ) ;
		pDataContents->GetCellData (_T("����"),0,m_strCSource ) ;
	    
		pDataContents->GetCellData (_T("����������"),0,m_strCCreateDate ) ;
		pDataContents->GetCellData (_T("����������"),0,m_strCUpdateDate ) ;
		pDataContents->GetCellData (_T("�ʷ�Ű"),0,m_strAbsKey ) ;
	     
		pDataContents->GetCellData (_T("�ʷ�"),0,m_strASource ) ;
		pDataContents->GetCellData (_T("�ʷϻ�����"),0,m_strACreateDate ) ;
		pDataContents->GetCellData (_T("�ʷϼ�����"),0,m_strAUpdateDate ) ;
	   
	
		pDataContents->GetCellData (_T("�ʷϻ�����"),0,m_strACreateDate ) ;
		pDataContents->GetCellData (_T("�ʷϼ�����"),0,m_strAUpdateDate ) ;
        
	 // pDataContents->GetCellData (_T("��������"),0,m_strConYN ) ;
	 //	pDataContents->GetCellData (_T("�ʷ�����"),0,m_strAbsYN ) ;	
	
	    pDataContents->GetCellData (_T("�����ȣ"),0,m_strControlNo ) ;
		pDataContents->GetCellData (_T("��Ű"),0,m_strSpeciesKey ) ;
		
	}  

   if(m_nDataRowIdx>=0){
		// �Է��϶��� �����ΰ�� ���� �������� ���õȴ�.

        GetDataMgrData (m_ParentDM ,_T("IBS_���ı���_�ڵ�"),m_strFormCode ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_�ڷᱸ��_�ڵ�"),m_strMatCode ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_�̿����ѱ���_�ڵ�"),m_strUseLimit ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_��KEY"),m_strSpeciesKey ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_�����ȣ"),m_strControlNo ,m_nDataRowIdx ) ;
        GetDataMgrData (m_ParentDM ,_T("BS_��������"),m_strConYN ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("BS_�ʷ�����"),m_strAbsYN ,m_nDataRowIdx ) ;
		m_strClassCode=_T("1");
	}

    
   	m_strLibCode=_T("011001");
	if(_ttoi(m_strCommonRecKey.GetBuffer (0))<=0)m_strCommonRecKey=_T("");
	if(_ttoi(m_strConKey.GetBuffer (0))<=0)m_strConKey=_T("");
	if(_ttoi(m_strAbsKey.GetBuffer (0))<=0)m_strAbsKey=_T("");
	if(_ttoi(m_strSpeciesKey.GetBuffer (0))<=0)m_strSpeciesKey=_T("");
	if(m_strConYN==_T("Y") || m_strConYN==_T("y"))m_strConYN==_T("Y"); else m_strConYN=_T("N");
	if(m_strAbsYN==_T("Y") || m_strAbsYN==_T("y"))m_strAbsYN==_T("Y"); else m_strAbsYN=_T("N");

     
/*
	if(m_nDlgType== 0 && m_strConYN == _T("Y"))
	{
		AfxMessageBox(_T("������ �̹� �����Ǿ� �ֽ��ϴ�.���� ������ �̿��Ͻñ� �ٶ��ϴ�."));
		OnCancel() ;
		return 0 ;

	}
*/
	// 3.ȭ���� �����Ѵ�.
	if(m_nDlgType==0){
		SetDlgItemText(IDC_eCONTROL_NO,m_strControlNo);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_��ǥ��"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eTITLE,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_������"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eAUTHOR,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_������"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_ePUBLISHER,strTemp);
	}else{

        if(GetDataMgrData (m_ParentDM ,_T("IBS_�����ȣ"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eCONTROL_NO,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_��ǥ��"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eTITLE,strTemp);
        if(GetDataMgrData (m_ParentDM ,_T("IBS_������"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eAUTHOR,strTemp);
        if(GetDataMgrData (m_ParentDM ,_T("IBS_������"),strTemp,m_nDataRowIdx ) == 0)
		{	SetDlgItemText(IDC_ePUBLISHER,strTemp);
			SetControlData (_T("CM_CONTENTMGR_02"),_T("������"),strTemp,-1) ;
		}


	}

	if(m_nDlgType==0)GetDlgItem(IDC_bSPECIES)->EnableWindow(false);
	else{
		if(m_nDataRowIdx>=0)GetDlgItem(IDC_bSPECIES)->EnableWindow(false);
		else GetDlgItem(IDC_bSPECIES)->EnableWindow(true);
	}

    // 4. �ٽ� ȭ���� �����Ѵ�.
	m_TocEditdlg.SetTocFromString(m_strCSource);
	m_ChorokDlg.SetChorok(m_strASource);

	GetDlgItem(IDC_bBRINGINC)->EnableWindow(true);

	if(m_strSpeciesKey!=_T(""))
	{
		if(m_strConYN==_T("Y") ){
			SetDlgItemText(IDC_STATIC_01,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(true);
			m_TocEditdlg.SetReadOnly(false);
			SetDlgItemText(IDC_bSAVEC,_T("��  ��"));
		}
		else{
			SetDlgItemText(IDC_STATIC_01,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(false);
			if(m_strUbControlNo!=_T("") && m_strNetConYN==_T("Y")){ 
				m_TocEditdlg.SetReadOnly(true);
				SetDlgItemText(IDC_bSAVEC,_T("��  ��"));
				GetDlgItem(IDC_bBRINGINC)->EnableWindow(false);
			}
			else
			{
				SetDlgItemText(IDC_bSAVEC,_T("��  ��"));
				m_TocEditdlg.SetReadOnly(false);
			}
		}
	}else{
		if(m_strConKey!=_T("") ){
			SetDlgItemText(IDC_STATIC_01,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(true);
		}else{
			SetDlgItemText(IDC_STATIC_01,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(false);
		}
		SetDlgItemText(IDC_bSAVEC,_T("��  ��"));
		m_TocEditdlg.SetReadOnly(false);
	}


	
	GetDlgItem(IDC_bBRINGINA)->EnableWindow(true);

	if(m_strSpeciesKey!=_T("")){
		if(m_strAbsYN==_T("Y"))
		{
			SetDlgItemText(IDC_STATIC_02,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(true);
			SetDlgItemText(IDC_bSAVEA,_T("��  ��"));
			m_ChorokDlg.SetReadOnly(false);
		}else{
			// KOLIS �������� ����
			SetDlgItemText(IDC_STATIC_02,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(false);
			if(m_strUbControlNo!=_T("") && m_strNetAbsYN==_T("Y")){ 
				m_ChorokDlg.SetReadOnly(true);
				SetDlgItemText(IDC_bSAVEA,_T("��  ��"));
				GetDlgItem(IDC_bBRINGINA)->EnableWindow(false);
			}else{
				SetDlgItemText(IDC_bSAVEA,_T("��  ��"));
				m_ChorokDlg.SetReadOnly(false);
			}
		}
	}else{
		if(m_strAbsKey!=_T(""))
		{
			SetDlgItemText(IDC_STATIC_02,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(true);
		}else{
			// KOLIS �������� ����
			SetDlgItemText(IDC_STATIC_02,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(false);
		}
		SetDlgItemText(IDC_bSAVEA,_T("��  ��"));
		m_ChorokDlg.SetReadOnly(false);
	}

   
    
    CString tmp_Title ,tmp_PubYear ,tmp_Publisher;

    ids = GetDataMgrData (m_ParentDM ,_T("IBS_��ǥ��"),tmp_Title,m_idx);
    ids = GetDataMgrData (m_ParentDM ,_T("IBS_����⵵"),tmp_PubYear,m_idx);
    ids = GetDataMgrData (m_ParentDM ,_T("IBS_������"),tmp_Publisher,m_idx); 

	CString strDefualt[8];
    strDefualt[0] = m_strControlNo ;            // �����ȣ
	strDefualt[1] = _T("") ;               // û����ȣ 
	strDefualt[2] = _T("") ;               // ����������
	strDefualt[3] = tmp_PubYear ;               // ����⵵
	strDefualt[4] = tmp_Publisher ;             // ������
	strDefualt[5] = _T("") ;               // �Ѽ���
    strDefualt[6] = tmp_Title ;                 // ���� 
    strDefualt[7] = _T("") ;

	m_TocEditdlg.SetTocDefault (strDefualt) ;
    
	/*
	<id>
	<call>
	<count>
	<date>2003
	<pub>��
	<sertitle>
	<title>������ �����ֳ�?
	<body>����

	</body>
    */
   
    m_arrBringDefault[0] = _T("<id>")+strDefualt[0]+_T("\r\n");
	m_arrBringDefault[1] = _T("<call>")+strDefualt[1]+_T("\r\n"); 
    m_arrBringDefault[2] = _T("<count>")+strDefualt[2]+_T("\r\n") ;
	m_arrBringDefault[3] = _T("<date>")+strDefualt[3]+_T("\r\n");    
	m_arrBringDefault[4] = _T("<pub>")+strDefualt[4]+_T("\r\n") ;
    m_arrBringDefault[5] = _T("<sertitle>")+strDefualt[5]+_T("\r\n") ;
    m_arrBringDefault[6] = _T("<title>")+strDefualt[6]+_T("\r\n") ;
			  
    //m_arrBringDefault[7] = _T("<body>")+strDefualt[7]+_T("\r\n</body>") ;
    
    CString strQuery ;
	CString strConKeyword ,strAbsKeyword ;
	if(m_strSpeciesKey!=_T("") && m_strConYN==_T("Y"))
	{
	     
		strQuery.Format (_T("select IDX_TOC_KEYWORD from IDX_BO_TOC_TBL where species_key = %s"),m_strSpeciesKey);
		pDataContents->GetOneValueQuery (strQuery,strConKeyword);
        ((CEdit*)GetDlgItem(IDC_eMOK_KEYWORD))->SetWindowText(strConKeyword);

	}

	if(m_strSpeciesKey!=_T("") && m_strAbsYN ==_T("Y"))
	{
        strQuery.Format (_T("select IDX_ABS_KEYWORD from IDX_BO_TOC_TBL where rec_key = %s"),m_strSpeciesKey);
		pDataContents->GetOneValueQuery (strQuery,strAbsKeyword);
        ((CEdit*)GetDlgItem(IDC_eCHO_KEYWORD))->SetWindowText(strAbsKeyword);

	}
  
	return 0 ;
}


//////////////////////////////////////////////////////////////////////////
//���� Ű���� ���� 
//////////////////////////////////////////////////////////////////////////
VOID CCONTENTMGR_02::OnbMOKKEYWORD() 
{
	// TODO: Add your control notification handler code here
	CString sBody ;
	sBody=m_TocEditdlg.GetFinalToc();
	if(m_TocEditdlg.CheckSyntax())return ;	

    CMakeSearchData *pMakeData = NULL;
    pMakeData = new CMakeSearchData(this); 

	INT iCh = pMakeData->ChangeHanjaToHangul( sBody );	//���� �ѱ� ��ȯ
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
		return ;
	}

	MakeIndexKeyWord(sBody) ;
   
	LPCTSTR lpszTok = _tcstok(sBody.GetBuffer(0), _T(" "));
	CString tmp_Data;
	CString strKeyWord;
	
	while (lpszTok)
	{   
		tmp_Data.Format (_T("%s"),lpszTok);
		iCh = pMakeData->GetKeyWord( tmp_Data );				//����, Ư������ ����
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
			return ;
		}

		strKeyWord +=  _T(" ") + tmp_Data;

		lpszTok = _tcstok(NULL, _T(" "));
	}

	strKeyWord.TrimLeft ();

	//�ߺ� Ű���� ����
	CArray<CString, CString&> pArrayKeyword;
	strKeyWord += _T(" ");
	INT iPos = strKeyWord.Find( _T(" ") );
	while ( iPos != -1 ) {
		CString sItem = strKeyWord.Left( iPos );
		INT iFind = 0;
		for ( INT i = 0 ; i < pArrayKeyword.GetSize() ; i++ ) {
			if ( sItem == pArrayKeyword.GetAt(i) ) {
				iFind = 1;
				break;
			}
		}
		if ( iFind == 0 ) {
			pArrayKeyword.Add( sItem );
		}
		strKeyWord = strKeyWord.Mid( iPos+1 );
		iPos = strKeyWord.Find( _T(" ") );
	}
	strKeyWord = _T("");

	for ( INT i = 0 ; i < pArrayKeyword.GetSize() ; i++ ) {
		strKeyWord = strKeyWord + _T(" ") + pArrayKeyword.GetAt(i);
	}

	if( strKeyWord.GetLength() > 1)
		strKeyWord = strKeyWord.Mid(1);

    CEdit * pEdit = (CEdit*)GetDlgItem(IDC_eMOK_KEYWORD) ;
	pEdit->SetWindowText (_T(""));
	pEdit->SetWindowText (strKeyWord) ;

	m_KeyWord_update = true ;
	m_strUpdateKeyword = strKeyWord ;
 
	delete pMakeData ;
}

VOID CCONTENTMGR_02::OnbCHOKEYWORD() 
{
	CString sBody ;
	sBody=m_ChorokDlg.GetChorokText();    

    CMakeSearchData *pMakeData = NULL;
    pMakeData = new CMakeSearchData(this); 

	INT iCh = pMakeData->ChangeHanjaToHangul( sBody );	//���� �ѱ� ��ȯ
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
		return ;
	}

	MakeIndexKeyWord(sBody) ;
   
	LPCTSTR lpszTok = _tcstok(sBody.GetBuffer(0), _T(" "));
	CString tmp_Data;
	CString strKeyWord;
	
	while (lpszTok)
	{   
		tmp_Data.Format (_T("%s"),lpszTok);
		iCh = pMakeData->GetKeyWord( tmp_Data );				//����, Ư������ ����
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
			return ;
		}

		strKeyWord +=  _T(" ") + tmp_Data;

		lpszTok = _tcstok(NULL, _T(" "));
	}

	strKeyWord.TrimLeft ();

	//�ߺ� Ű���� ����
	CArray<CString, CString&> pArrayKeyword;
	strKeyWord += _T(" ");
	INT iPos = strKeyWord.Find( _T(" ") );
	while ( iPos != -1 ) {
		CString sItem = strKeyWord.Left( iPos );
		INT iFind = 0;
		for ( INT i = 0 ; i < pArrayKeyword.GetSize() ; i++ ) {
			if ( sItem == pArrayKeyword.GetAt(i) ) {
				iFind = 1;
				break;
			}
		}
		if ( iFind == 0 ) {
			pArrayKeyword.Add( sItem );
		}
		strKeyWord = strKeyWord.Mid( iPos+1 );
		iPos = strKeyWord.Find( _T(" ") );
	}
	strKeyWord = _T("");

	for ( INT i = 0 ; i < pArrayKeyword.GetSize() ; i++ ) {
		strKeyWord = strKeyWord + _T(" ") + pArrayKeyword.GetAt(i);
	}

	if( strKeyWord.GetLength() > 1)
		strKeyWord = strKeyWord.Mid(1);

    CEdit * pEdit = (CEdit*)GetDlgItem(IDC_eCHO_KEYWORD) ;
	pEdit->SetWindowText (_T(""));
	pEdit->SetWindowText (strKeyWord) ;

    m_KeyWord_update = true ;
	m_strUpdateKeyword = strKeyWord ;

	delete pMakeData ;
}

VOID CCONTENTMGR_02::OnbDCMInput() 
{
	// TODO: Add your control notification handler code here
	
	CString strDBFLAG ;
	strDBFLAG = _T("M");

	CString sWhere = _T("");
	CString strTempContents = _T(""); 
    CString strBackup = _T("");

	CESL_DataMgr *pDM = FindDM(m_ParentDM );
	if(pDM == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!"));
        return ;
	}
     
	//�׽�Ʈ �� ���� �� �� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//m_strSpeciesKey.Format (_T("%d"),60212958) ;

	sWhere.Format (_T("SELECT CONTENTS FROM CO_DCM_TOC_TBL WHERE SPECIES_KEY = %s"),m_strSpeciesKey ) ;

	pDM->GetOneValueQuery (sWhere,strTempContents) ;
	
	if(strTempContents.IsEmpty ())
	{   
		AfxMessageBox(_T("���� �ڷᰡ �����ϴ�."));
		return ;
	}
    else
	{   
		
		strBackup=m_TocEditdlg.GetFinalToc();
        
        CString strDCMStream = _T("");
        //strDCMStream = strTempContents ;
        INT nStart ,nEnd ;
        nStart = strTempContents.Find (_T("<body>"));
        nEnd   = strTempContents.Find (_T("</body>"));
		if(nStart <0 ) return ;

		strDCMStream = strTempContents.Mid (nStart+6, nEnd-nStart-6) ; 
        strDCMStream.TrimRight ();
		strDCMStream.TrimLeft ();

        strTempContents = ReMake_BringInStream(0,strDCMStream) ;
        strTempContents.TrimLeft ();
		strTempContents.TrimRight ();
		m_TocEditdlg.SetTocFromString(strTempContents);

	}
    
    
    // 3. �״�� �����Ұ����� ����ڿ��� ���´�.
	if(AfxMessageBox(_T("����Ͻðڽ��ϱ�?"),MB_YESNO)==IDNO)
	{
		m_TocEditdlg.SetTocFromString( strBackup );
		return ;
	}

	
}

CString CCONTENTMGR_02::KeywordMake( CString strItemData )
{
EFS_BEGIN
/*
	if ( !strItemData.IsEmpty() ) {		
		INT iCh = m_MakeSearchData->ChangeHanjaToHangul(strItemData);	//���� �ѱ� ��ȯ
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strItemData ) error") );
			return _T("");
		}
		iCh = m_MakeSearchData->GetKeyWord(strItemData);	//����, Ư������ ����
		if ( 0 != iCh ) {						
			return _T("");
		}
		*/

		if (strItemData.IsEmpty()) return _T("");
		
		//�ܾ ���Ե� ��� Ű���� ����
		INT iWord = SearchWord( strItemData );
		if ( iWord == 1 ) {
			// Ű���� ä��
			return strItemData;
		}
		
		while (1)
		{			
			// �ҿ� ���Ե� ��� Ű���带 �������� �ʴ´�.
			INT iUnlessWord = SearchUnlessWord( strItemData );
			if ( iUnlessWord == 1 ) return _T("");
			
			//���翡 ���Ե� ��� ���縦 �����ϰ� Ű���� ����
			CString sResult=_T("");
			INT iAuxiliaryWord = SearchAuxiliaryWord( strItemData, sResult );
			if ( iAuxiliaryWord == 1 ) 
			{
				strItemData = sResult;				
			}
			else
			{
				// Ű���� ä��
				return strItemData;				
			}
			
		}
	//}

	return _T("");

EFS_END
return _T("");
}


INT CCONTENTMGR_02::SearchWord(CString sWord)
{
EFS_BEGIN

	INT ids;

	ids = SearchInFile(_T("..\\CFG\\CO_WORD_TBL.TXT"), sWord);
	if ( ids ) return ids;	
	return 0;

EFS_END
return -1;

}

INT CCONTENTMGR_02::SearchAuxiliaryWord( CString sWord, CString &sResult )
{
EFS_BEGIN

	// 
	CString strWord;
	INT ids;
	INT i;
	INT nLength;

	nLength = sWord.GetLength();

	for ( i=1 ; i<nLength ; i++ )
	{
		strWord = sWord.Right(nLength-i);
		ids = SearchInFile(_T("..\\CFG\\CO_AUXILIARY_WORD_TBL.TXT"), strWord);
		if ( ids == 1) 
		{
			sResult = sWord.Left(i);
			return ids;
		}
	}	

EFS_END
return -1;

}

//INT CESL_DataMgr::GetCellData(INT rowidx, INT idx, CString &result)
INT CCONTENTMGR_02::SearchUnlessWord( CString sWord )
{
EFS_BEGIN

	INT ids;

	ids = SearchInFile(_T("..\\CFG\\CO_UNLESS_WORD_TBL.TXT"), sWord);
	if ( ids ) return ids;
	return 0;

EFS_END
return -1;

}

INT CCONTENTMGR_02::LoadVolInfo()
{
	CESL_DataMgr* pVolDataMgr;
	CString	      strWhere;
	
	pVolDataMgr = FindDM(_T("DM_CONTENTS_VOL")) ;
	if(pVolDataMgr == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}

	strWhere.Format(_T("SPECIES_KEY=%s AND BOOK_APPENDIX_FLAG = 'B'"), m_strSpeciesKey);
	pVolDataMgr->RefreshDataManager(strWhere);		

	m_pGrid->Display();	

	return 0;
}

BEGIN_EVENTSINK_MAP(CCONTENTMGR_02, CDialog)
    //{{AFX_EVENTSINK_MAP(CCONTENTMGR_BO_EDIT)
	ON_EVENT(CCONTENTMGR_02, IDC_GRID_VOLUME, -600 /* Click */, OnClickGridVolume, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


VOID CCONTENTMGR_02::OnClickGridVolume() 
{
	PrevNextProc(0);
}

INT CCONTENTMGR_02::PrevNextProc(INT nPrevNext)
{	
	CESL_DataMgr* pDataContents;
	CString sMokcha_Check ;
	CString strWhere;
	INT		nTempIndex ;
	INT		ids;

	nTempIndex = m_idx ;	

    pDataContents = FindDM(_T("DM_�����ʷ�")) ;
	if(pDataContents == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}

	CESL_DataMgr* pVolDataMgr;
		
	pVolDataMgr = FindDM(_T("DM_CONTENTS_VOL")) ;
	if(pVolDataMgr == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}

	// 1.���� �ʷ��� ���� �´�. 2. ������ �����´�.
	// �Ǻ������� Vol�� Ȯ��
	CString strVolSortNo;
	INT nRow = m_pGrid->GetRow();	
	
	pVolDataMgr->GetCellData(_T("�Ϸù�ȣ"), nRow-1, strVolSortNo );	

	GetDataMgrData (m_ParentDM ,_T("IBS_��KEY"),m_strSpeciesKey,m_nDataRowIdx );
	
	if ( strVolSortNo.IsEmpty() || m_str90 != _T("Y"))
	{
		strWhere.Format(_T("toc.SPECIES_KEY=%s"), m_strSpeciesKey);	
	}
	else
	{
		strWhere.Format(_T("toc.SPECIES_KEY=%s AND toc.vol_sort_no=%s"), m_strSpeciesKey, strVolSortNo);	
	}
	
	pDataContents->RefreshDataManager (strWhere) ;

	// 2006.1.2 ADD BY PDJ
	// ���� ���� ���������� �Ǵܱ��ؿ��� ��ȣ�� ���� ������ �Ǵ�
	m_strConYN = _T("N");
	m_strAbsYN = _T("N");
	if ( pDataContents->GetRowCount() > 0 )
	{
		// �������
		m_nDlgType = 1;		
	}
	else
	{
		// �Է¸��
		m_nDlgType = 0;			
	}

	// �����̸� ����,�ʷ��� �����ش�.
	if ( m_nDlgType == 1 )
	{
		pDataContents->GetCellData (_T("���ڵ�Ű"),0,m_strCommonRecKey ) ;
		pDataContents->GetCellData (_T("����Ű"),0,m_strConKey ) ;
		pDataContents->GetCellData (_T("����"),0,m_strCSource ) ;
		
		pDataContents->GetCellData (_T("����������"),0,m_strCCreateDate ) ;
		pDataContents->GetCellData (_T("����������"),0,m_strCUpdateDate ) ;
		pDataContents->GetCellData (_T("�ʷ�Ű"),0,m_strAbsKey ) ;
		
		pDataContents->GetCellData (_T("�ʷ�"),0,m_strASource ) ;
		pDataContents->GetCellData (_T("�ʷϻ�����"),0,m_strACreateDate ) ;
		pDataContents->GetCellData (_T("�ʷϼ�����"),0,m_strAUpdateDate ) ;	   		
		
		pDataContents->GetCellData (_T("�����ȣ"),0,m_strControlNo ) ;
		pDataContents->GetCellData (_T("��Ű"),0,m_strSpeciesKey ) ;
		
		// ���� �ʷ� ����
		if ( m_strConKey.IsEmpty() ) m_strConYN = _T("N");
		else m_strConYN = _T("Y");

		if ( m_strAbsKey.IsEmpty() ) m_strAbsYN = _T("N");
		else m_strAbsYN = _T("Y");
	}
	else
	{
		// �Է��̸� ���� �ʱ�ȭ
		// 1. �������� �ʱ�ȭ
		m_strCommonRecKey = _T("");
		m_strConKey = _T("");
		m_strAbsKey = _T("");
		m_strSpeciesKey = _T("");
		
		// 2. �������� �ʱ�ȭ		
		m_strCSource = _T("");
		m_strCCreateDate = _T("");
		m_strCUpdateDate = _T("");
		
		// 3. �ʷ����� �ʱ�ȭ
		m_strASource = _T("");
		m_strACreateDate = _T("");
		m_strAUpdateDate = _T("");		
	}
	
	if(m_nDataRowIdx>=0){
		// �Է��϶��� �����ΰ�� ���� �������� ���õȴ�.
		
        GetDataMgrData (m_ParentDM ,_T("IBS_���ı���_�ڵ�"),m_strFormCode ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_�ڷᱸ��_�ڵ�"),m_strMatCode ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_�̿����ѱ���_�ڵ�"),m_strUseLimit ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_��KEY"),m_strSpeciesKey ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_�����ȣ"),m_strControlNo ,m_nDataRowIdx ) ;
        //GetDataMgrData (m_ParentDM ,_T("BS_��������"),m_strConYN ,m_nDataRowIdx ) ;
		//GetDataMgrData (m_ParentDM ,_T("BS_�ʷ�����"),m_strAbsYN ,m_nDataRowIdx ) ;
		m_strClassCode=_T("1");
	}       			
	
     
	if(m_nDlgType==0)GetDlgItem(IDC_bSPECIES)->EnableWindow(false);
	else{
		if(m_nDataRowIdx>=0)GetDlgItem(IDC_bSPECIES)->EnableWindow(false);
		else GetDlgItem(IDC_bSPECIES)->EnableWindow(true);
	}

    // 4. �ٽ� ȭ���� �����Ѵ�.
	m_TocEditdlg.SetTocFromString(m_strCSource);
	m_ChorokDlg.SetChorok(m_strASource);

	GetDlgItem(IDC_bBRINGINC)->EnableWindow(true);

	if(m_strSpeciesKey!=_T(""))
	{
		if(m_strConYN==_T("Y") ){
			SetDlgItemText(IDC_STATIC_01,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(true);
			m_TocEditdlg.SetReadOnly(false);
			SetDlgItemText(IDC_bSAVEC,_T("��  ��"));
		}
		else{
			SetDlgItemText(IDC_STATIC_01,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(false);
			if(m_strUbControlNo!=_T("") && m_strNetConYN==_T("Y")){ 
				m_TocEditdlg.SetReadOnly(true);
				SetDlgItemText(IDC_bSAVEC,_T("��  ��"));
				GetDlgItem(IDC_bBRINGINC)->EnableWindow(false);
			}
			else
			{
				SetDlgItemText(IDC_bSAVEC,_T("��  ��"));
				m_TocEditdlg.SetReadOnly(false);
			}
		}
	}else{
		if(m_strConKey!=_T("") ){
			SetDlgItemText(IDC_STATIC_01,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(true);
		}else{
			SetDlgItemText(IDC_STATIC_01,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(false);
		}
		SetDlgItemText(IDC_bSAVEC,_T("��  ��"));
		m_TocEditdlg.SetReadOnly(false);
	}


	
	GetDlgItem(IDC_bBRINGINA)->EnableWindow(true);

	if(m_strSpeciesKey!=_T("")){
		if(m_strAbsYN==_T("Y"))
		{
			SetDlgItemText(IDC_STATIC_02,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(true);
			SetDlgItemText(IDC_bSAVEA,_T("��  ��"));
			m_ChorokDlg.SetReadOnly(false);
		}else{
			// KOLIS �������� ����
			SetDlgItemText(IDC_STATIC_02,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(false);
			if(m_strUbControlNo!=_T("") && m_strNetAbsYN==_T("Y")){ 
				m_ChorokDlg.SetReadOnly(true);
				SetDlgItemText(IDC_bSAVEA,_T("��  ��"));
				GetDlgItem(IDC_bBRINGINA)->EnableWindow(false);
			}else{
				SetDlgItemText(IDC_bSAVEA,_T("��  ��"));
				m_ChorokDlg.SetReadOnly(false);
			}
		}
	}else{
		if(m_strAbsKey!=_T(""))
		{
			SetDlgItemText(IDC_STATIC_02,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(true);
		}else{
			// KOLIS �������� ����
			SetDlgItemText(IDC_STATIC_02,_T("<�� �� : ����>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(false);
		}
		SetDlgItemText(IDC_bSAVEA,_T("��  ��"));
		m_ChorokDlg.SetReadOnly(false);
	}
    
    CString tmp_Title ,tmp_PubYear ,tmp_Publisher;

    ids = GetDataMgrData (m_ParentDM ,_T("IBS_��ǥ��"),tmp_Title,m_idx);
    ids = GetDataMgrData (m_ParentDM ,_T("IBS_����⵵"),tmp_PubYear,m_idx);
    ids = GetDataMgrData (m_ParentDM ,_T("IBS_������"),tmp_Publisher,m_idx); 

	CString strDefualt[8];
    strDefualt[0] = m_strControlNo ;            // �����ȣ
	strDefualt[1] = _T("") ;               // û����ȣ 
	strDefualt[2] = _T("") ;               // ����������
	strDefualt[3] = tmp_PubYear ;               // ����⵵
	strDefualt[4] = tmp_Publisher ;             // ������
	strDefualt[5] = _T("") ;               // �Ѽ���
    strDefualt[6] = tmp_Title ;                 // ���� 
    strDefualt[7] = _T("") ;

	m_TocEditdlg.SetTocDefault (strDefualt) ;
    
	m_arrBringDefault[0] = _T("<id>")+strDefualt[0]+_T("\r\n");
	m_arrBringDefault[1] = _T("<call>")+strDefualt[1]+_T("\r\n"); 
	m_arrBringDefault[2] = _T("<count>")+strDefualt[2]+_T("\r\n") ;
	m_arrBringDefault[3] = _T("<date>")+strDefualt[3]+_T("\r\n");    
	m_arrBringDefault[4] = _T("<pub>")+strDefualt[4]+_T("\r\n") ;
	m_arrBringDefault[5] = _T("<sertitle>")+strDefualt[5]+_T("\r\n") ;
	m_arrBringDefault[6] = _T("<title>")+strDefualt[6]+_T("\r\n") ;

	if ( !m_strCommonRecKey.IsEmpty() )
	{
		CString strQuery ;
		CString strConKeyword ,strAbsKeyword, strValue ;
		CString strConUseLimitCode, strAbsUseLimitCode;
		INT		nOffset, nPos, nIdx;

		strQuery.Format (_T("select IDX_TOC_KEYWORD||'��'||TOC_USE_LIMIT_CODE||'��'||IDX_ABS_KEYWORD||'��'||ABS_USE_LIMIT_CODE from IDX_BO_TOC_TBL where REC_KEY = %s"),m_strCommonRecKey);
		pDataContents->GetOneValueQuery (strQuery,strValue);
		if ( strValue.IsEmpty() )
		{
			strConKeyword=_T("");
			strAbsKeyword=_T("");
			strConUseLimitCode=_T("");
			strAbsUseLimitCode=_T("");		
		}
		else
		{
			nOffset = 0;
			nPos = 0;
			nIdx = 0;

			while ( nOffset >= 0 ) 
			{
				nOffset = strValue.Find(_T("��"), nPos );
				switch ( nIdx )
				{
				case 0:
					strConKeyword = strValue.Mid(nPos, nOffset-nPos);
					break;
				case 1:
					strConUseLimitCode = strValue.Mid(nPos, nOffset-nPos);
					break;
				case 2:
					strAbsKeyword = strValue.Mid(nPos, nOffset-nPos);
					break;
				case 3:
					strAbsUseLimitCode = strValue.Mid(nPos, strValue.GetLength()-nPos);
					break;
				default:
					break;
				}
				nPos = nOffset+1;
				nIdx++;
			} // END WHILE			
		} // END IF
			
		if( strConUseLimitCode != _T("") )
			SetControlData (_T("CM_CONTENTMGR_02"),_T("�����̿����ѱ���"),strConUseLimitCode) ;
		if( strAbsUseLimitCode != _T("") )
			SetControlData (_T("CM_CONTENTMGR_02"),_T("�ʷ��̿����ѱ���"),strAbsUseLimitCode) ;	
		((CEdit*)GetDlgItem(IDC_eMOK_KEYWORD))->SetWindowText(strConKeyword);
		((CEdit*)GetDlgItem(IDC_eCHO_KEYWORD))->SetWindowText(strAbsKeyword);
	}
	else
	{
//		SetControlData (_T("CM_CONTENTMGR_02"),_T("�����̿����ѱ���"),_T("")) ;
//		SetControlData (_T("CM_CONTENTMGR_02"),_T("�ʷ��̿����ѱ���"),_T("")) ;
		((CEdit*)GetDlgItem(IDC_eMOK_KEYWORD))->SetWindowText(_T(""));
		((CEdit*)GetDlgItem(IDC_eCHO_KEYWORD))->SetWindowText(_T(""));
	}
	

	return 0;
}


VOID CCONTENTMGR_02::MakeSemiCollection(INT nMode, CESL_DataMgr* pDM, CString strContentsIn )
{
	CString strDate;
	CString strRecKey;
	CString strQuery;	
	
	// ���� ���� ����
	// ������ �����.
	// DELETE & INSERT
	// Ű�� ���ٸ� ���ʻ����̴�. => ����� ����.
	if ( m_strSpeciesKey.IsEmpty() || m_strConKey.IsEmpty() ) return;
	strQuery.Format(_T("INSERT INTO IDX_TMP_MBO_TOC_TBL (REC_KEY, MBO_TOC_KEY,IDX_WORK_FLAG,INPUT_DATE) ")
		_T("( SELECT ESL_SEQ.nextval, REC_KEY, 'D', sysdate FROM IDX_MBO_TOC_TBL WHERE CONTENTS_KEY=%s AND SPECIES_KEY = %s );"), 		
		m_strConKey, m_strSpeciesKey);
	pDM->ExecuteQuery(strQuery, FALSE, FALSE );	

	strQuery.Format(_T("DELETE FROM IDX_MBO_TOC_TBL WHERE CONTENTS_KEY=%s AND SPECIES_KEY = %s; "), 
		m_strConKey, m_strSpeciesKey );
	pDM->ExecuteQuery(strQuery, FALSE, FALSE );	

	// ���� ����̸� ������ �ϰ� return
	if ( nMode == 1 ) 
	{
		return;
	}
			
	CTime t = CTime::GetCurrentTime ();
	strDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());


	CString title,ori_title,author,ori_author, strConSortNo;
	CString OneLine;
	
	// 01.
	INT idx = strContentsIn.Find(_T("<body>"),0);
	// 02.
	idx=strContentsIn.Find(_T("\n"),idx);
	
	// 03.	
	INT s_idx=idx+1,e_idx=idx+1;	
	INT con_sort_no=1;

	CIndex index;	
	
	while(strContentsIn.Mid(s_idx,7)!=_T("</body>"))
	{
		e_idx=strContentsIn.Find(_T("\n"),s_idx);			
		if(e_idx==-1) break;
		OneLine=strContentsIn.Mid(s_idx,e_idx-s_idx);

		INT token_idx=OneLine.Find(_T("//"),0);
		if(token_idx!=-1)
		{
			ori_title=title=OneLine.Mid(0,token_idx);
			ori_author=author=OneLine.Mid(token_idx+2);
			ori_author.Remove('\r');
		}
		else
		{
			ori_title=title=OneLine;
			ori_title.Remove('\r');
			ori_author=author=_T("");
		}

		// 02. TITLE
		index.MakeIndex_NonSpace(title);
		index.ConvertHanjaToHangul(title);
		
		title.Replace(_T("��"),_T(""));
		title.TrimLeft();
		title.TrimRight();		

		strConSortNo.Format(_T("%d"), con_sort_no);

		// 03. AUTHOR
		index.MakeIndex_NonSpace(author);
		index.ConvertHanjaToHangul(author);	
		
		// DB �۾�
		pDM->InitDBFieldData () ;
		pDM->MakeRecKey (strRecKey) ;
		pDM->AddDBFieldData (_T("REC_KEY")					, _T("NUMERIC")	,strRecKey);
		pDM->AddDBFieldData (_T("CONTENTS_KEY")				, _T("NUMERIC")	,m_strConKey);
		pDM->AddDBFieldData (_T("SPECIES_KEY")				, _T("NUMERIC")	,m_strSpeciesKey);
		pDM->AddDBFieldData (_T("CON_SORT_NO")				, _T("NUMERIC")	,strConSortNo);
		pDM->AddDBFieldData (_T("IDX_BO_TOC_TITLE")			, _T("STRING")	,title);
		pDM->AddDBFieldData (_T("IDX_BO_TOC_AUTHOR")		, _T("STRING")	,author);
		pDM->AddDBFieldData (_T("IDX_BO_TOC_TITLE_INFO")	, _T("STRING")	,ori_title);
		pDM->AddDBFieldData (_T("IDX_BO_TOC_AUTHOR_INFO")	, _T("STRING")	,ori_author);
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		ids = pDM->MakeInsertFrame (_T("IDX_MBO_TOC_TBL"));
// 		if(ids<0) {
// 			AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_BO_TBL) Error~!"));
// 			return ;
// 		}		
		//=====================================================
		
				
		// 2. ���̳� �ڷ� �÷��� ���� TEMP TABLE : IDX_TMP_MBO_TOC_TBL			
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		pDM->InitDBFieldData () ;
// 		pDM->AddDBFieldData (_T("REC_KEY")			, _T("NUMERIC")	,strRecKey);
// 		pDM->AddDBFieldData (_T("MBO_TOC_KEY")		, _T("NUMERIC")	,strRecKey);
// 		pDM->AddDBFieldData (_T("IDX_WORK_FLAG")	, _T("STRING")	,_T("I"));				
// 		pDM->AddDBFieldData (_T("INPUT_DATE")		, _T("STRING")	,strDate);	
// 		ids = pDM->MakeInsertFrame (_T("IDX_TMP_MBO_TOC_TBL"));
// 		if(ids<0) {
// 			AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_BO_TBL) Error~!"));
// 			return ;
// 		}		
		//=====================================================
		
		
		s_idx=e_idx+1;
		con_sort_no++;
	}
}



HBRUSH CCONTENTMGR_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

