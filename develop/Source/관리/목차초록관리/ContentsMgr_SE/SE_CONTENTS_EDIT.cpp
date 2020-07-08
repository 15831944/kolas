// SE_CONTENTS_EDIT.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CONTENTS_EDIT.h"
#include "SE_CREATE_ARTICLE1.h"
#include "SE_CONTENTS_01.h"


#include "KisaMarcImport.h"

#include "CommonDefine.h"
#include "..\..\..\����\K2UP_Common\MakeSearchData.h"

#include <direct.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CONTENTS_EDIT dialog


CSE_CONTENTS_EDIT::CSE_CONTENTS_EDIT(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_KisaCount = 0 ;

	m_DbClckMode = FALSE ;
	m_idx = -1;
}
CSE_CONTENTS_EDIT::~CSE_CONTENTS_EDIT()
{
	
}


BOOL CSE_CONTENTS_EDIT::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CSE_CONTENTS_EDIT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CONTENTS_EDIT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CONTENTS_EDIT, CDialog)
	//{{AFX_MSG_MAP(CSE_CONTENTS_EDIT)
	ON_BN_CLICKED(IDC_bCreateArticle, OnbCreateArticle)
	ON_BN_CLICKED(IDC_bNext, OnbNext)
	ON_BN_CLICKED(IDC_bPrev, OnbPrev)
	ON_BN_CLICKED(IDC_bFile, OnbFile)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_BN_CLICKED(IDC_bDCMInput, OnbDCMInput)
	ON_BN_CLICKED(IDC_bKISAEXPORT, OnbKISAEXPORT)
	ON_BN_CLICKED(IDC_bKISAIMPORT, OnbKISAIMPORT)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CONTENTS_EDIT message handlers

BOOL CSE_CONTENTS_EDIT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_TocEditdlg.MyCreate(this,CRect(12,160,10+650,45+410),_YON_);
  	//m_ChorokDlg.MyCreate(this,&m_TocEditdlg,CRect(0,10,11,11));
 
	if(InitESL_Mgr(_T("SM_CONTENTMGR_SE_EDIT"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}
  

	m_ViewerControl2.InitViewer(IDC_VIEW_Grid, this); 
	m_ViewerControl2.SetViewerWidth(20, 75);    
   

	m_nTabCur = ((CSE_CONTENTS_01*)pParentMgr)->m_TabCtrl.GetCurSel () ;
	if(m_nTabCur == 0)
	{   
		m_CurrentDM = _T("DM_CONTENTMGR_SE_TBL")  ;
		m_CurrentCM = _T("CM_CONTENTMGR_SE_EDIT") ;
		m_ParentCM  = _T("CM_CONTENTMGR_SE_MAIN") ;
		m_ParentDM  = _T("DM_CONTENTMGR_SE_MAIN") ;
		m_ParentGrid = _T("MainGrid") ;
		
		if(m_DbClckMode == TRUE)
		{
		   DbClick_Move(0) ;
		}
		else SetScreen(0) ;

	}
    else
	{

	}
	
	EnableThemeDialogTexture(GetSafeHwnd());
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CONTENTS_EDIT::OnbCreateArticle() 
{
	CString strBasicMokcha =_T("");
	CString strLastMokcha  =_T("");
	
	strBasicMokcha=m_TocEditdlg.GetFinalToc();
	if(m_TocEditdlg.CheckSyntax())return;

	strLastMokcha = strBasicMokcha ;

	if(CheckControlNo() == FALSE)
	{
		AfxMessageBox(_T("�����ȣ�� �����ϽǼ� �����ϴ�.")) ;
		if(m_DbClckMode == TRUE)
		{
			DbClick_Move(0) ;
		}
		else SetScreen(0) ;

		return ;
	}

	CSE_CREATE_ARTICLE dlg(0,this) ;
	dlg.GetTabCurSel(m_nTabCur) ;
	dlg.GetMokcha (strBasicMokcha) ;
	dlg.GetKeyValue (m_Contents_key,m_sVol_Key,m_Control_Key) ;
	if(dlg.DoModal() == IDCANCEL)
	{
		INT first,end ;
		CString strBody ;
		strBody = strBasicMokcha ;
		first = strBody.Find (_T("<body>")) ;
		end   = strBody.Find (_T("</body>"));

		strBody = strBody.Mid (first,end-first) ;
		first = strBody.Find (_T("\r\n")) ;

		strBody = strBody.Mid (first+2,strBody.GetLength () ) ;

        strLastMokcha.Replace (strBody,dlg.sLast_Mokcha ) ;
		
		if(strLastMokcha.Compare (strBasicMokcha) != 0 ||
			dlg.m_bUpdateFlag == TRUE )
		{
			CESL_DataMgr *pDM = FindDM(m_CurrentDM ) ;
			if(pDM == NULL) 
			{
				AfxMessageBox(_T("FindDM() Error~!")) ;
				return ;
			}
			
			pDM->StartFrame () ;
			pDM->InitDBFieldData () ;
			pDM->AddDBFieldData (_T("CONTENTS"),_T("STRING"),strLastMokcha) ;
			pDM->MakeUpdateFrame (_T("SE_CONTENTS_TBL"),_T("VOL_KEY"),_T("NUMERIC"),m_sVol_Key );
			pDM->SendFrame ();
			pDM->EndFrame () ;
        } 


		if(m_DbClckMode == TRUE)
		{
		   DbClick_Move(0) ;
		}
		else SetScreen(0) ;
	//	CString msg ;
	//	msg.Format (_T("%d kisa "),m_KisaCount);
	//	AfxMessageBox(msg) ;
	}
}
 
INT CSE_CONTENTS_EDIT::SetScreen(INT ndir)
{
	INT ids ;
	
	CESL_ControlMgr *pCM = FindCM(m_ParentCM) ;
	if(pCM == NULL) AfxMessageBox(_T("FindCM() Error~!")) ;
	
    CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (m_ParentGrid) ;
	
	switch(ndir)
	{
	case 0 :
		
		SelectMakeList(m_ParentCM  ,m_ParentGrid);
		ids = SelectGetHeadPosition(m_ParentCM  ,m_ParentGrid);
		if (ids < 0)
		{
			//2004-07-09 ������ 
				//AfxMessageBox(_T("���� ����� ������ �� �����ϴ�."));	// ����
				AfxMessageBox(_T("���� �׸��� �����ϴ�."));	// ����
			EndDialog(IDCANCEL);
			return -222;
		}
		break;
		
	case -1 :
		
        ids = SelectGetPrev (m_ParentCM  ,m_ParentGrid);
		if (ids == -1) {
			ids = SelectGetHeadPosition(m_ParentCM  ,m_ParentGrid);
			AfxMessageBox(_T("ó�� �ڷ��Դϴ�."));
			return -223;
		}
		break;
		
	case 1 :
		
		ids = SelectGetNext (m_ParentCM  ,m_ParentGrid);
		if (ids == -1) {
			ids = SelectGetTailPosition(m_ParentCM  ,m_ParentGrid);
			AfxMessageBox(_T("������ �ڷ��Դϴ�."));
			return -224;
		}
		break;
	
	case 2 :
		ids = m_idx;
		break;
	}
    m_idx = ids; 
	SetReverse(m_ParentCM  ,m_ParentGrid, m_idx);
    
	
    CESL_DataMgr *pDM = FindDM(m_ParentDM ) ;
    if(pDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -2;
	}

	CESL_DataMgr *pContentDM = FindDM(m_CurrentDM) ;
	if(pContentDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -3;
	}
    
	CString strSQL ;

	CString sContentYN ;
   // CString sSpecies_Key ;

    CString sControl_no,sTitle,sVol_title,sPublish_date,sPublisher,sPub_Freq,sSub_Vol_Title ;

    pDM->GetCellData (_T("��Ű"),m_idx,m_sSpecies_Key ) ;
	pDM->GetCellData (_T("��������"),m_idx,sContentYN) ;
	pDM->GetCellData (_T("�����ȣ"),m_idx,sControl_no);
    
	pDM->GetCellData (_T("����"),m_idx,sTitle);
	pDM->GetCellData (_T("��ȣ��"),m_idx,sVol_title);
	pDM->GetCellData (_T("������"),m_idx,sPublish_date);
  //  pDM->GetCellData (_T("��ȣŰ")m_idx,sVol_Key);
	pDM->GetCellData (_T("��ȣŰ"),m_idx,m_sVol_Key ) ;
	
	pDM->GetCellData (_T("������"),m_idx,sPublisher ) ;
    pDM->GetCellData (_T("�����"),m_idx,sPub_Freq ) ;
    pDM->GetCellData (_T("��������ȣ��"),m_idx,sSub_Vol_Title ) ;

    m_Control_Key = sControl_no ; 

	if(sContentYN == _T("Y") || sContentYN == _T("y"))
	{
		pDM->GetCellData (_T("��ȣŰ"),m_idx,m_sVol_Key ) ;
        
		strSQL.Format (_T(" VOL_KEY = %s"),m_sVol_Key ) ;

        pContentDM->RefreshDataManager (strSQL) ;
		
        pContentDM->GetCellData (_T("����"),0,m_Mokcha) ;
       
        basic_Mokcha = m_Mokcha ;

		pContentDM->GetCellData (_T("���Ӹ���Ű"),0,m_Contents_key);
        
      
		if(pContentDM->GetRowCount ()>0) 
		{
			(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(TRUE) ;
			(CButton*)GetDlgItem(IDC_bDelete)->EnableWindow(TRUE) ;
		    CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
            pIDOK->SetWindowText (_T("����"));
			m_SaveCheck = FALSE ;
			ContentYN = TRUE ;
		}
		else
		{
			(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(FALSE) ;
			(CButton*)GetDlgItem(IDC_bDelete)->EnableWindow(FALSE) ;
			 CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
			 pIDOK->SetWindowText (_T("����"));
			 m_SaveCheck = TRUE ;
			 ContentYN = FALSE ;
			 m_Mokcha = _T("") ;
		}

	}
	else
    {
		(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(FALSE) ;
		(CButton*)GetDlgItem(IDC_bDelete)->EnableWindow(FALSE) ;
	     CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
         pIDOK->SetWindowText (_T("����"));
		 m_SaveCheck = TRUE ;
		 ContentYN = FALSE ;
		 m_Mokcha = _T("") ;
	}


/*
	CESL_ControlMgr *pCurrentCM_Mgr = FindCM(m_CurrentCM );
	if(pCurrentCM_Mgr == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -22 ;
	}

	pCurrentCM_Mgr->SetControlMgrData (_T("�����ȣ"),sControl_no ,-1) ;
	pCurrentCM_Mgr->SetControlMgrData (_T("����"),sTitle ,-1) ;
	pCurrentCM_Mgr->SetControlMgrData (_T("��ȣ��"),sVol_title,-1) ;
*/    

	//m_Mokcha= _T("<id>KSE000011632\r\n<key>17638754\r\n<vol>â��ȣ\r\n<call>\r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\n<body>����\r\n������ / ����� = 2[!--17806227--]\r\n������ / ��٥�� = 4[!--17806228--]\r\n��������Ѻ������Ҵ������ǳ������ / ˩�� = 10[!--17806229--]\r\n��������������� : �����̰���ٽ������� / ������ = 14[!--17806230--]\r\n�����϶� / ����� = 19[!--17806231--]\r\n���������濡���Ͼ� / ���� = 22[!--17806232--]\r\n���������� / ����� = 24[!--17806233--]\r\n���������������� / ���� = 29[!--17806234--]\r\n������������������θ����ϳ�� / ������ = 34[!--17806235--]\r\n����϶��������� / ������ = 39[!--17806236--]\r\n����������� / ��� = 42[!--17806237--]\r\n���� = 46[!--17806238--]\r\n����������? / ��٥�� = 47[!--17806239--]\r\n���������� / ������ = 55[!--17806240--]\r\n��˧ߣ��� / ��� = 57[!--17806241--]\r\n������������ / ���� = 64[!--17806242--]\r\n�����������[!--17806243--]\r\n��������ġ��[!--17806244--]\r\n</body>\r\n</vol>\r\n");

    m_TocEditdlg.SetTocFromString(m_Mokcha) ;
	
    //m_TocEditdlg.SetYonKeyFunc( (INT (CWnd::*)(TCHAR*))MakePrimaryKey ) ;
   
	//AllControlDisplay (m_currentCM ,m_idx) ;
/*
 == ���� ======

 �����ȣ    <id>
 ��ȣ key    <key>
 �Ѽ���
 ����
 ��ȣ��
 ������
 û����ȣ
 ������ 
*/
	CString strDefualt[8];
    strDefualt[0] = sControl_no ;
    strDefualt[1] = m_sVol_Key ;
	strDefualt[2] = _T("") ;
	strDefualt[3] = sTitle ;
	strDefualt[4] = sVol_title ;
	strDefualt[5] = sPublish_date ;
	strDefualt[6] = _T("") ;
    strDefualt[7] = _T("") ;

	m_arrBringDefault[0] = _T("<id>")+strDefualt[0]+_T("\r\n");
	m_arrBringDefault[1] = _T("<call>")+strDefualt[1]+_T("\r\n"); 
	m_arrBringDefault[2] = _T("<count>")+strDefualt[2]+_T("\r\n") ;
	m_arrBringDefault[3] = _T("<date>")+strDefualt[3]+_T("\r\n");    
	m_arrBringDefault[4] = _T("<pub>")+strDefualt[4]+_T("\r\n") ;
	m_arrBringDefault[5] = _T("<sertitle>")+strDefualt[5]+_T("\r\n") ;
	m_arrBringDefault[6] = _T("<title>")+strDefualt[6]+_T("\r\n") ;


	m_TocEditdlg.SetTocDefault (strDefualt) ;
	//sPublisher,sPub_Freq,sSub_Vol_Title
    //m_ViewerControl2.SetControlData(_T("�����ȣ"),sControl_no);
    m_ViewerControl2.SetControlData(_T("��    ��"),sTitle);
    m_ViewerControl2.SetControlData(_T("�� �� ��"),sPublisher);
	m_ViewerControl2.SetControlData(_T("�����"),sPub_Freq);
	m_ViewerControl2.SetControlData(_T("�� ȣ ��"),sVol_title);
    m_ViewerControl2.SetControlData(_T("��������ȣ��"),sSub_Vol_Title);
	m_ViewerControl2.SetControlData(_T("�� �� ��"),sPublish_date);

    m_ViewerControl2.Display();
    
	return 0 ;
}

VOID CSE_CONTENTS_EDIT::OnbNext() 
{
	// TODO: Add your control notification handler code here
	if(m_nTabCur == 0)
	{
		if(m_DbClckMode == TRUE)
		{
		   DbClick_Move(1) ;
		}
		else SetScreen(1) ;

    }
	else 
	{
		SetScreen_KisaTab(1) ;
	}


}

VOID CSE_CONTENTS_EDIT::OnbPrev() 
{
	// TODO: Add your control notification handler code here
	if(m_nTabCur == 0)
	{
		if(m_DbClckMode == TRUE)
		{
		   DbClick_Move(-1) ;
		}
		else SetScreen(-1) ;

    }
	else 
	{
		SetScreen_KisaTab(1) ;
	}
}

VOID CSE_CONTENTS_EDIT::OnOK() 
{
	// TODO: Add extra validation here
	
	CESL_DataMgr *pContentsDM_Mgr = FindDM(m_CurrentDM);
	if(pContentsDM_Mgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

    CString sButtonText ;
    CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
    pIDOK->GetWindowText (sButtonText) ;
     
     
	CString sContents_RecKey ;

    CESL_DataMgr *pParentDM_Mgr = FindDM(m_ParentDM );
	if(pParentDM_Mgr == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}
    // ���ڵ�_KEY,������_KEY,�����ȣ,��� ����,����,�̿����ѱ���,�Է���,������ 
    CString sSpecies_Key ,sControl_No ,sUselimitCode ,sVol_Key;

    pParentDM_Mgr->GetCellData (_T("��Ű"),m_idx,sSpecies_Key) ;
    pParentDM_Mgr->GetCellData (_T("�����ȣ"),m_idx,sControl_No) ;
    pParentDM_Mgr->GetCellData (_T("�̿����ѱ���"),m_idx,sUselimitCode) ;
    pParentDM_Mgr->GetCellData (_T("��ȣŰ"),m_idx,sVol_Key) ;
    

	//TocEditor ���� �޾� �ð�.........../////////////////////////////
 	CString sMokcha ;
   	sMokcha=m_TocEditdlg.GetFinalToc();
	if(m_TocEditdlg.CheckSyntax())return;

	//������ư ������ ���� .....
    last_Mokcha = sMokcha ;

	//��¥ �ð� //////////////////////////////////////////////////////////
    CString system_Date ;
	CTime t = CTime::GetCurrentTime () ;
    system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;

    //�������� UPDATE ////////////////////////////////////////////////////
//	CString sContentsYN ;
//	sContentsYN = _T("Y") ;

    CString sMokcha_Key;



    // ���� �Է� �� ��츸 ���� //////////////////////////////////////////////////
	if(m_SaveCheck == TRUE )
	{
	   	pContentsDM_Mgr->MakeRecKey (sContents_RecKey);
        
        pContentsDM_Mgr->StartFrame () ;
		pContentsDM_Mgr->InitDBFieldData () ;
		
		pContentsDM_Mgr->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),sContents_RecKey );
        pContentsDM_Mgr->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"),sSpecies_Key );
		pContentsDM_Mgr->AddDBFieldData (_T("VOL_KEY"),_T("NUMERIC"),sVol_Key );
		
		///////////////////////////////////////////////////
        //LOC_FLAG ��� ���� 
		//  IF ( COUNT >= 1 ) THEN nReturnCnt = 0 ���� 
		//  IF ( COUNT == 0 ) THEN nReturnCnt = 1 �۾� 
        
		INT ids = GetLocFlag(pContentsDM_Mgr,sVol_Key);
		if( ids == 0 )
		{
			pContentsDM_Mgr->AddDBFieldData (_T("LOC_FLAG"),_T("STRING"),_T("1") );	
		}
        else if( ids >= 1)
		{
            pContentsDM_Mgr->AddDBFieldData (_T("LOC_FLAG"),_T("STRING"),_T("0") );
		}
		else 
		{
            AfxMessageBox(_T("GetLogFlag() Error~!")) ;
		}
        
        ///////////////////////////////////////////////////

		pContentsDM_Mgr->AddDBFieldData (_T("CONTROL_NO"),_T("STRING"),sControl_No  );
		pContentsDM_Mgr->AddDBFieldData (_T("CONTENTS"),_T("STRING"),sMokcha );
		pContentsDM_Mgr->AddDBFieldData (_T("USE_LIMIT_CODE"),_T("STRING"),sUselimitCode );
        pContentsDM_Mgr->AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),system_Date);
        pContentsDM_Mgr->AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),system_Date);
        
		pContentsDM_Mgr->MakeInsertFrame (_T("SE_CONTENTS_TBL")) ;
         
        pContentsDM_Mgr->InitDBFieldData () ;

		pContentsDM_Mgr->AddDBFieldData (_T("CONTENTS_YN"),_T("STRING"),_T("Y")) ;
        pContentsDM_Mgr->MakeUpdateFrame (_T("SE_VOL_TBL"),_T("REC_KEY"),_T("NUMERIC"),sVol_Key);


		pContentsDM_Mgr->SendFrame () ;
		pContentsDM_Mgr->EndFrame () ;
         
		
        pParentDM_Mgr->SetCellData (_T("��������"),_T("Y"),m_idx) ;
        

		//2003�� 6�� 14�� ���� 
        AllControlDisplay (m_ParentCM ,-1) ;
		
        //AfxMessageBox (_T("���� ������ �Ϸ� �Ǿ����ϴ�.")) ;

	}
	// ������ ������ ������� ���� //////////////////////////////////////////////////
    else
	{
        //2004-07-21 ������ 
		if(CheckControlNo() == FALSE)
		{
			AfxMessageBox(_T("�����ȣ�� �����ϽǼ� �����ϴ�.")) ;
			if(m_DbClckMode == TRUE)
			{
				DbClick_Move(1) ;
			}
			else
			{
				if( 224 == SetScreen(1) ) 
					SetScreen(2);
			}

			return ;
		}

	
        pContentsDM_Mgr->GetCellData (_T("���Ӹ���Ű"),0,sMokcha_Key) ;
       
		
		pContentsDM_Mgr->StartFrame () ;
		pContentsDM_Mgr->InitDBFieldData () ;
        pContentsDM_Mgr->AddDBFieldData (_T("CONTENTS"),_T("STRING"),sMokcha) ;
        pContentsDM_Mgr->AddDBFieldData (_T("UPDATE_DATE"),_T("STRING"),system_Date ) ;
      
        pContentsDM_Mgr->MakeUpdateFrame (_T("SE_CONTENTS_TBL"),_T("REC_KEY"),_T("NUMERIC"),sMokcha_Key ) ;

		
		if(basic_Mokcha.Compare (last_Mokcha) != 0 ) 
		{
			MakeUpdateQuery(pContentsDM_Mgr) ;
		}

		pContentsDM_Mgr->SendFrame () ;
		pContentsDM_Mgr->EndFrame () ;

        Update_Check() ;
	}
	
	if(m_DbClckMode == TRUE)
	{
		DbClick_Move(1) ;
	}
	else
	{
		if( m_SaveCheck == TRUE )
			SetScreen(2);
		else if ( 224 == SetScreen(1) ) 
			SetScreen(2);
	}
    
  //    m_TocEditdlg.SetTocFromString(sMokcha) ;
  //	(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(TRUE) ;
  //	CDialog::OnOK();
}

INT CSE_CONTENTS_EDIT::MakeUpdateQuery (CESL_DataMgr *pDM) 
{
	
//	CMapStringToString map_Basic_Mokcha;
//	CMapStringToString map_Last_Mokcha; 
	
	CString strBasicBody ;
	CString strLastBody ;
	
	strBasicBody = basic_Mokcha ;
	strLastBody  = last_Mokcha  ;
	
//	AfxMessageBox (strBasicBody) ;
//	AfxMessageBox (strLastBody) ;

	map_Basic_Mokcha.RemoveAll () ;
	map_Last_Mokcha.RemoveAll () ;
	
	INT first,end ;
	
	first = strBasicBody.Find (_T("<body>")) ;
	end   = strBasicBody.Find (_T("</body>"));
	
	strBasicBody = strBasicBody.Mid (first,end-first) ;
	first = strBasicBody.Find (_T("\r\n")) ;
	
	strBasicBody = strBasicBody.Mid (first,strBasicBody.GetLength ()-(first+2) ) ;
	
	
	
	first = strLastBody.Find (_T("<body>")) ;
	end   = strLastBody.Find (_T("</body>"));
	
	strLastBody = strLastBody.Mid (first,end-first) ;
	first = strLastBody.Find (_T("\r\n")) ;
	
	strLastBody = strLastBody.Mid (first,strLastBody.GetLength ()-(first+2) ) ;
	
	//AfxMessageBox (strBasicBody +_T("\r\n\r\n")+strLastBody) ;
	
	INT nPos ;
	INT nIndex;
	CString strLine ;
	CString strFeildData ;
	CString strControlNo ;
	CString sDelimiter ;
	sDelimiter = _T("\r\n") ;
	bool IsStop ;
	
	for(INT i = 0 ;i < 2; i++)
	{
		IsStop = FALSE;

		if(i == 0 )strLine = strBasicBody;
		else strLine = strLastBody ;
		
		while( TRUE )
		{
			nIndex = strLine.Find(sDelimiter);
			
			if( nIndex < 0 )
			{
				if(strLine.Find (_T("[!--")) == -1)
				{
					IsStop = TRUE;
					//ontinue ;
				}
				else
				{
					nPos = strLine.Find (_T("[!--")) ;
					strFeildData = strLine.Mid (0,nPos) ;
					strControlNo = strLine.Mid (nPos+4,23) ;
					strControlNo.Replace(_T("-"), _T(""));strControlNo.Replace(_T("]"), _T(""));
					strControlNo.Replace(_T("\n"), _T(""));strControlNo.Replace(_T("\r"), _T(""));
					
					if(i == 0)map_Basic_Mokcha.SetAt (strControlNo,strFeildData) ;
					else map_Last_Mokcha.SetAt (strControlNo,strFeildData) ;
					
				}
				IsStop = TRUE;
			}
			else
			{
				if(strLine.Find (_T("[!--")) == -1)
				{
					//continue ;
				}
				else
				{
					nPos = strLine.Find (_T("[!--")) ;
					strFeildData = strLine.Mid (0,nPos) ;
					strControlNo = strLine.Mid (nPos+4,23) ;
					strControlNo.Replace(_T("-"), _T(""));					
					strControlNo.Replace(_T("]"), _T(""));
					strControlNo.Replace(_T("\n"), _T(""));
					strControlNo.Replace(_T("\r"), _T(""));
			
					strFeildData.TrimLeft () ; strFeildData.TrimRight () ;
					
					if(i== 0)map_Basic_Mokcha.SetAt (strControlNo,strFeildData) ;
					else map_Last_Mokcha.SetAt (strControlNo,strFeildData) ;
					
				}

				strLine = strLine.Mid( nIndex+2 , strLine.GetLength()-nIndex );
				strLine.TrimLeft () ;strLine.TrimRight () ;
			}
			
			if( IsStop ) break;
		}
		
	}

	//���� ������ ���� üũ �� �����ϱ� 
	INT ids ;
	CString strCompareLine ;
	CString strUpdateLine ;
	CString strUpdateControlNo ;
	CStringArray arrUpdateData ;
	BOOL bFirstCheck  ;
	BOOL bSecondCheck ;
	TCHAR	szTemp[10000];
	
	POSITION pos ;
	for(pos = map_Basic_Mokcha.GetStartPosition () ; pos != NULL ;)
	{
		bFirstCheck = false ;
		bSecondCheck = false ;

		map_Basic_Mokcha.GetNextAssoc (pos,strUpdateControlNo,strCompareLine ) ;
		
		ids = map_Last_Mokcha.Lookup (strUpdateControlNo,strUpdateLine);
		if(ids != 0 && strCompareLine.Compare (strUpdateLine) != 0)
		{
            if(strUpdateLine.Find (_T("/")) != -1) bFirstCheck = true ;
			if(strUpdateLine.Find (_T("=")) != -1) bSecondCheck = true ;
			
			arrUpdateData.RemoveAll () ;
			//TCHAR seps[2] = {'/','='} ;
			TCHAR *token;

			_stprintf(szTemp, _T("%s"), strUpdateLine );
			
			token = _tcstok( szTemp, _T("/=") );
			while( token != NULL )
			{
				arrUpdateData.Add (token) ;
				token = _tcstok( NULL, _T("/=") );				
			}
			
			CString strAticle,strAuthor,strPage ;
            
			if(bFirstCheck == TRUE && bSecondCheck == TRUE )
			{
				strAticle = arrUpdateData.GetAt (0) ;
				strAuthor = arrUpdateData.GetAt (1) ;
				strPage = arrUpdateData.GetAt (2) ;
			}
			else if ( bFirstCheck == TRUE && bSecondCheck == FALSE)
			{
				strAticle = arrUpdateData.GetAt (0) ;
				strAuthor = arrUpdateData.GetAt (1) ;
				strPage = _T("") ;	
			}
			else if ( bFirstCheck == FALSE && bSecondCheck == TRUE)
			{
				strAticle = arrUpdateData.GetAt (0) ;
				strAuthor = _T("") ;
				strPage = arrUpdateData.GetAt (1) ;
			}
			else if ( bFirstCheck == FALSE && bSecondCheck == FALSE && arrUpdateData.GetSize () == 1)
			{
				strAticle = arrUpdateData.GetAt (0) ;
				strAuthor = _T("") ;
				strPage = _T("") ;
			}

			MakeFrameQuery(pDM,strUpdateControlNo,strAticle,strAuthor,strPage) ;
			
		}
		
	}
	
	return 0 ;
}


VOID CSE_CONTENTS_EDIT::OnbFile() 
{
	// TODO: Add your control notification handler code here
		if(!BringIn(0))AfxMessageBox(_T("�������Խ���"));
}
BOOL CSE_CONTENTS_EDIT::BringIn(INT nType)
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
		if(!file.Open(filedlg.GetPathName(),CFile::modeRead | CFile::typeBinary)){
			AfxMessageBox(_T("ȭ���� �� �� �����ϴ�."));
			goto ERR_RETURN;
		}
		else{
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
			}
			else
				m_ChorokDlg.SetChorok( strFileStream );

			// 3. �״�� �����Ұ����� ����ڿ��� ���´�.
			if(AfxMessageBox(_T("����Ͻðڽ��ϱ�?"),MB_YESNO)==IDNO){
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
VOID CSE_CONTENTS_EDIT::OnbDelete() 
{
	// TODO: Add your control notification handler code here
	CString sContents_RecKey ;

	CESL_DataMgr *pContentsDM_Mgr = FindDM(m_CurrentDM);
	if(pContentsDM_Mgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

    CESL_DataMgr *pParentDM_Mgr = FindDM(m_ParentDM );
	if(pParentDM_Mgr == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}
	 // ���ڵ�_KEY,������_KEY,�����ȣ,��� ����,����,�̿����ѱ���,�Է���,������ 
    CString sSpecies_Key ,sControl_No ,sUselimitCode ,sVol_Key;

    pParentDM_Mgr->GetCellData (_T("��Ű"),m_idx,sSpecies_Key) ;
    pParentDM_Mgr->GetCellData (_T("��ȣŰ"),m_idx,sVol_Key) ;
    
	

    pContentsDM_Mgr->GetCellData (_T("���Ӹ���Ű"),0,sContents_RecKey);
   
	// �������� _T("N") ���� UPDATE 
    pParentDM_Mgr->StartFrame () ;
 
	pParentDM_Mgr->InitDBFieldData () ;
	pParentDM_Mgr->MakeDeleteFrame (_T("SE_CONTENTS_TBL"),_T("VOL_KEY"),_T("NUMERIC"),sVol_Key);

    pParentDM_Mgr->InitDBFieldData () ;    
	pParentDM_Mgr->MakeDeleteFrame (_T("SE_ARTICLE_TBL"),_T("VOL_KEY"),_T("NUMERIC"),sVol_Key);
    

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2004-07-27 leehakjoong

	// IDX_TMP_SE_TOC_TBL

	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (m_pInfo->CONNECTION_INFO ) ;
	CString strQuery ;         //0
	strQuery.Format (_T("select rec_key from idx_se_toc_Tbl where vol_key = %s"),sVol_Key) ;
	dm.RestructDataManager (strQuery) ;

	//	
    CString strArticleKey =_T("") ;
	for(INT i = 0 ; i < dm.GetRowCount () ;i++)
	{
		strArticleKey = dm.GetCellData (i,0) ;

		if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{   
			CString strData ;
			strData.Format(_T("ECO$P_DELETE_SETOC$I(%s);"), strArticleKey);
			pParentDM_Mgr->AddFrame(strData);
		}
		
		pParentDM_Mgr->InitDBFieldData ();
		
		CString strTmp_Key ;
		CString system_Date ;
		CTime t = CTime::GetCurrentTime ();
		system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
		
		
		pParentDM_Mgr->MakeRecKey (strTmp_Key) ;
		pParentDM_Mgr->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
		pParentDM_Mgr->AddDBFieldData (_T("ARTICLE_KEY"),_T("NUMERIC"),strArticleKey);
		pParentDM_Mgr->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("D"));
		pParentDM_Mgr->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);
		
		INT ids ;
		ids = pParentDM_Mgr->MakeInsertFrame (_T("IDX_TMP_SE_TOC_TBL"));
		if(ids<0) 
		{
			AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_SE_TOC_TBL) Error~!"));
			return ;
		}
		
    }

	// IDX_SE_TOC_TBL ���� �ϱ�
    pParentDM_Mgr->InitDBFieldData () ;    
	pParentDM_Mgr->MakeDeleteFrame (_T("IDX_SE_TOC_TBL"),_T("VOL_KEY"),_T("NUMERIC"),sVol_Key);

 
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    pParentDM_Mgr->InitDBFieldData () ;
	pParentDM_Mgr->AddDBFieldData (_T("CONTENTS_YN"),_T("STRING"),_T("N")) ;
	pParentDM_Mgr->MakeUpdateFrame (_T("SE_VOL_TBL"),_T("REC_KEY"),_T("NUMERIC"),sVol_Key) ;

	pParentDM_Mgr->SendFrame () ;
	pParentDM_Mgr ->EndFrame () ;

    pParentDM_Mgr->SetCellData (_T("��������"),_T("N"),m_idx) ;

	AllControlDisplay (m_ParentCM ,-1) ;
    
	AfxMessageBox (_T("������ ���� �Ǿ����ϴ�."));

	OnCancel() ;
  
}

VOID CSE_CONTENTS_EDIT::OnCancel() 
{
// TODO: Add extra cleanup here
//	CString msg ;
//	msg.Format (_T("%d kisa "),SetKisaCount());
//   	AfxMessageBox(msg) ;
    
    CESL_ControlMgr *pCM = FindCM(m_ParentCM );
	if(pCM == NULL)
	{
		AfxMessageBox(_T("FindCM() Error~!"));
		return ;
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (m_ParentGrid );
	if(pGrid == NULL)
	{
		AfxMessageBox(_T("FindControl() Error~!"));
	}
     
    CString msg ;
	msg.Format (_T("%d"),SetKisaCount());
	pGrid->SetAt (m_idx,8,msg) ;

	CDialog::OnCancel();
}

INT CSE_CONTENTS_EDIT::Update_Check()
{
    //������ư �������� ����  
    basic_Mokcha ;
   
	//������ư ������ ���� 
    last_Mokcha ;

    if(basic_Mokcha.Compare (last_Mokcha) == 0) 
	{
		 AfxMessageBox (_T("���� �������� �����ϴ�.")) ;
	}
	/*
    else
	{
       	 AfxMessageBox (_T("������ ���� �Ǿ����ϴ�.��� ������ �����մϴ�.")) ;
	}
	*/

	//--------------------------------------------------------------
	//2004-07-19 ������ 
	else
	{
		 AfxMessageBox (_T("������ ���� �Ǿ����ϴ�.")) ;
	}
	//--------------------------------------------------------------

	
	return 0 ;
}

INT CSE_CONTENTS_EDIT::SetScreen_KisaTab(INT ndir)
{
	INT ids ;
	
	CESL_ControlMgr *pCM = FindCM(m_ParentCM) ;
	if(pCM == NULL) AfxMessageBox(_T("FindCM() Error~!")) ;
	
    CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (m_ParentGrid) ;
	
	switch(ndir)
	{
	case 0 :
		
		SelectMakeList(m_ParentCM  ,m_ParentGrid);
		ids = SelectGetHeadPosition(m_ParentCM  ,m_ParentGrid);
		if (ids < 0)
		{
			//2004-07-09 ������
			//AfxMessageBox(_T("���� ����� ������ �� �����ϴ�."));	// ����
			AfxMessageBox(_T("���� �׸��� �����ϴ�."));	// ����
			EndDialog(IDCANCEL);
			return -222;
		}
		break;
		
	case -1 :
		
        ids = SelectGetPrev (m_ParentCM  ,m_ParentGrid);
		if (ids == -1) {
			ids = SelectGetHeadPosition(m_ParentCM  ,m_ParentGrid);
			AfxMessageBox(_T("ó�� �ڷ��Դϴ�."));
			return -223;
		}
		break;
		
	case 1 :
		
		ids = SelectGetNext (m_ParentCM  ,m_ParentGrid);
		if (ids == -1) {
			ids = SelectGetTailPosition(m_ParentCM  ,m_ParentGrid);
			AfxMessageBox(_T("������ �ڷ��Դϴ�."));
			return -224;
		}
		break;
		
	}
    m_idx = ids; 
	SetReverse(m_ParentCM  ,m_ParentGrid, m_idx);
    

    
    CString strSQL ;
    CESL_DataMgr *pDM = FindDM(m_ParentDM ) ;
    if(pDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -2;
	}

	pDM->GetCellData (_T("VOL_KEY"),m_idx,m_sVol_Key ) ;       
	strSQL.Format (_T(" V.REC_KEY = %s"),m_sVol_Key ) ;
    

    
	CESL_DataMgr *pContentDM = FindDM(m_CurrentDM) ;
	if(pContentDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -3;
	}
    
	/////////////////////////////////////////////////////////////
	//2003�� 6�� 9�� 
    //KISA TAB ���� ������ ���� ���� ���� DM
	/////////////////////////////////////////////////////////////
	CESL_DataMgr *pDM_Temp = FindDM(_T("DM_CONTENTMGR_SE_MAIN_TEMP"));
	if(pDM_Temp == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!"));
		return -3;
	}

	pDM_Temp->RefreshDataManager (strSQL);


	CString sContentYN ;
   // CString sSpecies_Key ;

    CString sControl_no,sTitle,sVol_title ;

    pDM_Temp->GetCellData (_T("��Ű"),0,m_sSpecies_Key ) ;
	pDM_Temp->GetCellData (_T("��������"),0,sContentYN) ;
	pDM_Temp->GetCellData (_T("�����ȣ"),0,sControl_no);
    
	pDM_Temp->GetCellData (_T("����"),0,sTitle);
	pDM_Temp->GetCellData (_T("��ȣ��"),0,sVol_title);

	
    m_Control_Key = sControl_no ; 

	if(sContentYN == _T("Y") || sContentYN == _T("y"))
	{
		pDM->GetCellData (_T("VOL_KEY"),0,m_sVol_Key ) ;
        
		strSQL.Format (_T(" VOL_KEY = %s"),m_sVol_Key ) ;

        pContentDM->RefreshDataManager (strSQL) ;
		
        pContentDM->GetCellData (_T("����"),0,m_Mokcha) ;
       
        basic_Mokcha = m_Mokcha ;

		pContentDM->GetCellData (_T("���Ӹ���Ű"),0,m_Contents_key);
        
      
		if(pContentDM->GetRowCount ()>0) 
		{
			(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(TRUE) ;
			(CButton*)GetDlgItem(IDC_bDelete)->EnableWindow(TRUE) ;
		    CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
            pIDOK->SetWindowText (_T("����"));
			m_SaveCheck = FALSE ;
			ContentYN = TRUE ;
		}
	}
	else
    {
		(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(FALSE) ;
		(CButton*)GetDlgItem(IDC_bDelete)->EnableWindow(FALSE) ;
	     CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
         pIDOK->SetWindowText (_T("����"));
		 m_SaveCheck = TRUE ;
		 ContentYN = FALSE ;
		 m_Mokcha = _T("") ;
	}



	CESL_ControlMgr *pCurrentCM_Mgr = FindCM(m_CurrentCM );
	if(pCurrentCM_Mgr == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -22 ;
	}

	pCurrentCM_Mgr->SetControlMgrData (_T("�����ȣ"),sControl_no ,-1) ;
	pCurrentCM_Mgr->SetControlMgrData (_T("����"),sTitle ,-1) ;
	pCurrentCM_Mgr->SetControlMgrData (_T("��ȣ��"),sVol_title,-1) ;
    

	//m_Mokcha= _T("<id>KSE000011632\r\n<key>17638754\r\n<vol>â��ȣ\r\n<call>\r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\n<body>����\r\n������ / ����� = 2[!--17806227--]\r\n������ / ��٥�� = 4[!--17806228--]\r\n��������Ѻ������Ҵ������ǳ������ / ˩�� = 10[!--17806229--]\r\n��������������� : �����̰���ٽ������� / ������ = 14[!--17806230--]\r\n�����϶� / ����� = 19[!--17806231--]\r\n���������濡���Ͼ� / ���� = 22[!--17806232--]\r\n���������� / ����� = 24[!--17806233--]\r\n���������������� / ���� = 29[!--17806234--]\r\n������������������θ����ϳ�� / ������ = 34[!--17806235--]\r\n����϶��������� / ������ = 39[!--17806236--]\r\n����������� / ��� = 42[!--17806237--]\r\n���� = 46[!--17806238--]\r\n����������? / ��٥�� = 47[!--17806239--]\r\n���������� / ������ = 55[!--17806240--]\r\n��˧ߣ��� / ��� = 57[!--17806241--]\r\n������������ / ���� = 64[!--17806242--]\r\n�����������[!--17806243--]\r\n��������ġ��[!--17806244--]\r\n</body>\r\n</vol>\r\n");

    m_TocEditdlg.SetTocFromString(m_Mokcha) ;
	

	(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(FALSE);
    (CButton*)GetDlgItem(IDOK)->EnableWindow(FALSE);
    (CButton*)GetDlgItem(IDC_bDelete)->EnableWindow(FALSE);
//	(CButton*)GetDlgItem(IDC_bFile)->EnableWindow(FALSE);
//	(CButton*)GetDlgItem(IDC_bNext)->EnableWindow(FALSE);
//	(CButton*)GetDlgItem(IDC_bPrev)->EnableWindow(FALSE);
	//AllControlDisplay (m_currentCM ,m_idx) ;
	
     

	return 0 ;
}


CString CSE_CONTENTS_EDIT::ReMake_BringInStream(INT nType,CString strStream)
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
		// AfxMessageBox (str_BringStream) ;
		 
	}
	else
	{


	}

    
	return str_BringStream ;
	
}


//  IF ( COUNT >= 1 ) THEN nReturnCnt = 0 ���� 
//  IF ( COUNT == 0 ) THEN nReturnCnt = 1 �۾� 

INT CSE_CONTENTS_EDIT::GetLocFlag(CESL_DataMgr * pDM , CString str_Vol_Key)
{
   
	INT nReturnCnt  ;
	CString strResult ;
    CString strQuery ;
	strQuery.Format (_T("SELECT COUNT(WORKING_STATUS) FROM SE_BOOK_TBL WHERE ")
                     //=====================================================
					 //2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 					 _T("WORKING_STATUS IN('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O') ")
					 // 18.09.27 JMJ KOL.RED.2018.008
					 _T("WORKING_STATUS IN('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O','SEL611O') ")
					 //=====================================================
					 _T("AND VOL_KEY = %s "),str_Vol_Key);
	
	pDM->GetOneValueQuery (strQuery,strResult);

	nReturnCnt = _ttoi(strResult.GetBuffer (0)) ;
    
	return nReturnCnt ;
     
}


INT CSE_CONTENTS_EDIT::SetKisaCount ()
{
	//m_sVol_Key ;
    //select count(*) from se_article_tbl where vol_key = m_sVol_Key ;

    CESL_DataMgr *pDM = FindDM(m_ParentDM );
	if(pDM == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}
	CString strQuery ;
	strQuery.Format (_T("select count(*) from se_article_tbl where vol_key = %s"),m_sVol_Key) ;
    CString nCount ;
	pDM->GetOneValueQuery (strQuery,nCount) ;

	m_KisaCount = _ttoi(nCount.GetBuffer (0)) ;

	return m_KisaCount ;
}



INT CSE_CONTENTS_EDIT::DbClick_Move (INT ndir)
{
    
	CESL_ControlMgr *pCM = FindCM(m_ParentCM );
	if(pCM == NULL)
	{
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl (m_ParentGrid );
    if(pCM == NULL)
	{
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -2;
	}
     
	INT  ids ;
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
  
    //  m_idx = ids; 
    SetReverse (m_ParentCM ,m_ParentGrid ,m_idx) ;
      
    ////////////////////////////////////////////////////////////////////////////////////////////

    CESL_DataMgr *pDM = FindDM(m_ParentDM ) ;
    if(pDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -2;
	}

	CESL_DataMgr *pContentDM = FindDM(m_CurrentDM) ;
	if(pContentDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -3;
	}
    
	CString strSQL ;

	CString sContentYN ;
   // CString sSpecies_Key ;

    CString sControl_no,sTitle,sVol_title,sPublish_date,sPublisher,sPub_Freq,sSub_Vol_Title ;

    pDM->GetCellData (_T("��Ű"),m_idx,m_sSpecies_Key ) ;
	pDM->GetCellData (_T("��������"),m_idx,sContentYN) ;
	pDM->GetCellData (_T("�����ȣ"),m_idx,sControl_no);
    
	pDM->GetCellData (_T("����"),m_idx,sTitle);
	pDM->GetCellData (_T("��ȣ��"),m_idx,sVol_title);
	pDM->GetCellData (_T("������"),m_idx,sPublish_date);
  //  pDM->GetCellData (_T("��ȣŰ")m_idx,sVol_Key);
	pDM->GetCellData (_T("��ȣŰ"),m_idx,m_sVol_Key ) ;
	
	pDM->GetCellData (_T("������"),m_idx,sPublisher ) ;
    pDM->GetCellData (_T("�����"),m_idx,sPub_Freq ) ;
    pDM->GetCellData (_T("��������ȣ��"),m_idx,sSub_Vol_Title ) ;

    m_Control_Key = sControl_no ; 

	if(sContentYN == _T("Y") || sContentYN == _T("y"))
	{
		pDM->GetCellData (_T("��ȣŰ"),m_idx,m_sVol_Key ) ;
        
		strSQL.Format (_T(" VOL_KEY = %s"),m_sVol_Key ) ;

        pContentDM->RefreshDataManager (strSQL) ;
		
        pContentDM->GetCellData (_T("����"),0,m_Mokcha) ;
       
        basic_Mokcha = m_Mokcha ;

		pContentDM->GetCellData (_T("���Ӹ���Ű"),0,m_Contents_key);
        
      
		if(pContentDM->GetRowCount ()>0) 
		{
			(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(TRUE) ;
			(CButton*)GetDlgItem(IDC_bDelete)->EnableWindow(TRUE) ;
		    CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
            pIDOK->SetWindowText (_T("����"));
			m_SaveCheck = FALSE ;
			ContentYN = TRUE ;
		}
	}
	else
    {
		(CButton*)GetDlgItem(IDC_bCreateArticle)->EnableWindow(FALSE) ;
		(CButton*)GetDlgItem(IDC_bDelete)->EnableWindow(FALSE) ;
	     CButton *pIDOK = (CButton*)GetDlgItem(IDOK) ;
         pIDOK->SetWindowText (_T("����"));
		 m_SaveCheck = TRUE ;
		 ContentYN = FALSE ;
		 m_Mokcha = _T("") ;
	}


/*
	CESL_ControlMgr *pCurrentCM_Mgr = FindCM(m_CurrentCM );
	if(pCurrentCM_Mgr == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -22 ;
	}

	pCurrentCM_Mgr->SetControlMgrData (_T("�����ȣ"),sControl_no ,-1) ;
	pCurrentCM_Mgr->SetControlMgrData (_T("����"),sTitle ,-1) ;
	pCurrentCM_Mgr->SetControlMgrData (_T("��ȣ��"),sVol_title,-1) ;
*/    

	//m_Mokcha= _T("<id>KSE000011632\r\n<key>17638754\r\n<vol>â��ȣ\r\n<call>\r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\n<body>����\r\n������ / ����� = 2[!--17806227--]\r\n������ / ��٥�� = 4[!--17806228--]\r\n��������Ѻ������Ҵ������ǳ������ / ˩�� = 10[!--17806229--]\r\n��������������� : �����̰���ٽ������� / ������ = 14[!--17806230--]\r\n�����϶� / ����� = 19[!--17806231--]\r\n���������濡���Ͼ� / ���� = 22[!--17806232--]\r\n���������� / ����� = 24[!--17806233--]\r\n���������������� / ���� = 29[!--17806234--]\r\n������������������θ����ϳ�� / ������ = 34[!--17806235--]\r\n����϶��������� / ������ = 39[!--17806236--]\r\n����������� / ��� = 42[!--17806237--]\r\n���� = 46[!--17806238--]\r\n����������? / ��٥�� = 47[!--17806239--]\r\n���������� / ������ = 55[!--17806240--]\r\n��˧ߣ��� / ��� = 57[!--17806241--]\r\n������������ / ���� = 64[!--17806242--]\r\n�����������[!--17806243--]\r\n��������ġ��[!--17806244--]\r\n</body>\r\n</vol>\r\n");

    m_TocEditdlg.SetTocFromString(m_Mokcha) ;
	
    //m_TocEditdlg.SetYonKeyFunc( (INT (CWnd::*)(TCHAR*))MakePrimaryKey ) ;
   
	//AllControlDisplay (m_currentCM ,m_idx) ;
/*
 == ���� ======

 �����ȣ    <id>
 ��ȣ key    <key>
 �Ѽ���
 ����
 ��ȣ��
 ������
 û����ȣ
 ������ 
*/
	CString strDefualt[8];
    strDefualt[0] = sControl_no ;
    strDefualt[1] = m_sVol_Key ;
	strDefualt[2] = _T("") ;
	strDefualt[3] = sTitle ;
	strDefualt[4] = sVol_title ;
	strDefualt[5] = sPublish_date ;
	strDefualt[6] = _T("") ;
    strDefualt[7] = _T("") ;

	m_arrBringDefault[0] = _T("<id>")+strDefualt[0]+_T("\r\n");
	m_arrBringDefault[1] = _T("<call>")+strDefualt[1]+_T("\r\n"); 
	m_arrBringDefault[2] = _T("<count>")+strDefualt[2]+_T("\r\n") ;
	m_arrBringDefault[3] = _T("<date>")+strDefualt[3]+_T("\r\n");    
	m_arrBringDefault[4] = _T("<pub>")+strDefualt[4]+_T("\r\n") ;
	m_arrBringDefault[5] = _T("<sertitle>")+strDefualt[5]+_T("\r\n") ;
	m_arrBringDefault[6] = _T("<title>")+strDefualt[6]+_T("\r\n") ;


	m_TocEditdlg.SetTocDefault (strDefualt) ;
	//sPublisher,sPub_Freq,sSub_Vol_Title
    //m_ViewerControl2.SetControlData(_T("�����ȣ"),sControl_no);
    m_ViewerControl2.SetControlData(_T("��    ��"),sTitle);
    m_ViewerControl2.SetControlData(_T("�� �� ��"),sPublisher);
	m_ViewerControl2.SetControlData(_T("�����"),sPub_Freq);
	m_ViewerControl2.SetControlData(_T("�� ȣ ��"),sVol_title);
    m_ViewerControl2.SetControlData(_T("��������ȣ��"),sSub_Vol_Title);
	m_ViewerControl2.SetControlData(_T("�� �� ��"),sPublish_date);

    m_ViewerControl2.Display();

	return 0 ;
}
VOID CSE_CONTENTS_EDIT::OnbDCMInput() 
{
	// TODO: Add your control notification handler code here
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

	sWhere.Format (_T("SELECT CONTENTS FROM CO_DCM_TOC_TBL WHERE SPECIES_KEY = %s and VOL_KEY = %s") ,m_sSpecies_Key ,m_sVol_Key) ;

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

INT CSE_CONTENTS_EDIT::MakeFrameQuery(CESL_DataMgr *pDM, CString strKey, CString strArticle, CString strAuthor, CString strPage)
{
    INT ids ;
	CString strIndexArticle,strIndexAuthor ,strAllItem ;

	CMakeSearchData *pMakeData = NULL; 
    pMakeData = new CMakeSearchData(this);


	strIndexArticle = strArticle ;
	strIndexAuthor  = strAuthor ;

	INT iCh = pMakeData->ChangeHanjaToHangul( strIndexArticle );	//���� �ѱ� ��ȯ
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strIndexArticle ) error") );
		return -1;
	}
	iCh = pMakeData->GetKeyWord( strIndexArticle );				//����, Ư������ ����
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strIndexArticle ) error") );
		return -2;
	}

	iCh = pMakeData->ChangeHanjaToHangul( strIndexAuthor );	//���� �ѱ� ��ȯ
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strIndexAuthor ) error") );
		return -1;
	}
		   
	if(strIndexAuthor.Find (_T(";"))!=-1)
	{    
		CString strTemp = _T("");
		CString strToken = _T("") ;
		TCHAR seps[2] ;
		TCHAR *token ;
		seps[0] = ';';
		seps[1] = '\0';
		token = _tcstok( strIndexAuthor.GetBuffer (0), seps );
		while( token != NULL )
		{
			strToken = token ;
			strToken.TrimLeft ();strToken.TrimRight () ;
		    if(strTemp == _T("")) 
			{
				strTemp = strToken ;
			}
			else
			{
				strTemp += _T(" ") + strToken ;
			}
			token = _tcstok( NULL, seps );
		}
       
        strIndexAuthor = strTemp ;
	}


	strAllItem.Format (_T("%s %s"),strIndexArticle,strIndexAuthor) ;


	CString strArticleKey = _T("") ;
	CString strQuery ;
	strQuery.Format (_T("select rec_key from idx_se_toc_tbl where article_control_no = '%s'"),strKey);
	pDM->GetOneValueQuery (strQuery ,strArticleKey ) ;
    
	
	//1. IDX_SE_TOC_TBL
	pDM->InitDBFieldData ();	
	pDM->AddDBFieldData (_T("IDX_ARTICLE"),_T("STRING"),strIndexArticle) ;
	pDM->AddDBFieldData (_T("IDX_IARTICLE"),_T("STRING"),strIndexArticle) ;
	
	pDM->AddDBFieldData (_T("IDX_ARTICLE_KEYWORD"),_T("STRING"),strIndexArticle);
	pDM->AddDBFieldData (_T("IDX_ARTICLE_AUTHOR"),_T("STRING"),strIndexAuthor );
	pDM->AddDBFieldData (_T("IDX_ALL_ITEM"),_T("STRING"),strAllItem );
	
	pDM->AddDBFieldData (_T("ARTICLE_INFO"),_T("STRING"),strArticle );
	pDM->AddDBFieldData (_T("ARTICLE_AUTHOR_INFO"),_T("STRING"),strAuthor  );
	pDM->AddDBFieldData (_T("ARTICLE_PAGE_INFO"),_T("STRING"),strPage );
	
	pDM->MakeUpdateFrame (_T("IDX_SE_TOC_TBL"),_T("REC_KEY"),_T("NUMERIC"),strArticleKey);

	
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{   
		CString strData ;
		strData.Format(_T("ECO$P_MAKE_SETOC$I(%s);"), strArticleKey);
		pDM->AddFrame(strData);
	}

	//2. SE_ARTICLE_TBL
	
	pDM->InitDBFieldData ();
	pDM->AddDBFieldData (_T("ARTICLE_NAME"),_T("STRING"),strArticle );
	pDM->AddDBFieldData (_T("ARTICLE_AUTHOR"),_T("STRING"),strAuthor);
	pDM->AddDBFieldData (_T("START_PAGE"),_T("STRING"),strPage);
	pDM->MakeUpdateFrame  (_T("SE_ARTICLE_TBL"),_T("REC_KEY"),_T("NUMERIC"),strArticleKey);
	

	
	CString strTmp_Key ;
	CString system_Date ;
	CTime t = CTime::GetCurrentTime ();
	system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
	
	//3. IDX_TMP_SE_TOC_TBL
     
	pDM->MakeRecKey (strTmp_Key) ;
	pDM->InitDBFieldData ();
	pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
	pDM->AddDBFieldData (_T("ARTICLE_KEY"),_T("NUMERIC"),strArticleKey);
	pDM->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("I"));
	pDM->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);
	
	ids = pDM->MakeInsertFrame (_T("IDX_TMP_SE_TOC_TBL"));
	if(ids<0) 
	{
		AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_SE_TOC_TBL) Error~!"));
		return -1;
	}


	delete  pMakeData ; 
    pMakeData = NULL ;

				
    return 0 ;
}

bool CSE_CONTENTS_EDIT::CheckControlNo()
{

	
	CString strBasicBody ;
	CString strLastBody ;
	
	strBasicBody = m_Mokcha ;
	strLastBody  = m_TocEditdlg.GetFinalToc();  ;
	
	//AfxMessageBox (strBasicBody);
	//AfxMessageBox (strLastBody) ;


	map_Basic_Mokcha.RemoveAll () ;
	map_Last_Mokcha.RemoveAll () ;
	
	INT first,end ;
	
	first = strBasicBody.Find (_T("<body>")) ;
	end   = strBasicBody.Find (_T("</body>"));
	
	strBasicBody = strBasicBody.Mid (first,end-first) ;
	first = strBasicBody.Find (_T("\r\n")) ;
	
	strBasicBody = strBasicBody.Mid (first,strBasicBody.GetLength ()-(first+2) ) ;
	
	
	
	first = strLastBody.Find (_T("<body>")) ;
	end   = strLastBody.Find (_T("</body>"));
	
	strLastBody = strLastBody.Mid (first,end-first) ;
	first = strLastBody.Find (_T("\r\n")) ;
	
	strLastBody = strLastBody.Mid (first,strLastBody.GetLength ()-(first+2) ) ;
	
	//AfxMessageBox (strBasicBody +_T("\r\n\r\n")+strLastBody) ;
	
	INT nPos ;
	INT nIndex;
	CString strLine ;
	CString strFeildData ;
	CString strControlNo ;
	CString sDelimiter ;
	sDelimiter = _T("\r\n") ;
	bool IsStop ;
	
	for(INT i = 0 ;i < 2; i++)
	{
		IsStop = FALSE;

		if(i == 0 )strLine = strBasicBody;
		else strLine = strLastBody ;
		
		while( TRUE )
		{
			nIndex = strLine.Find(sDelimiter);
			
			if( nIndex < 0 )
			{
				if(strLine.Find (_T("[!--")) == -1)
				{
					IsStop = TRUE;
					//ontinue ;
				}
				else
				{
					nPos = strLine.Find (_T("[!--")) ;
					strFeildData = strLine.Mid (0,nPos) ;
					strControlNo = strLine.Mid (nPos+4,23) ;
					strControlNo.Replace(_T("-"), _T(""));
					strControlNo.Replace(_T("]"), _T(""));
					strControlNo.Replace(_T("\n"), _T(""));
					strControlNo.Replace(_T("\r"), _T(""));
					
					if(i == 0)map_Basic_Mokcha.SetAt (strControlNo,strFeildData) ;
					else map_Last_Mokcha.SetAt (strControlNo,strFeildData) ;
					
				}
				IsStop = TRUE;
			}
			else
			{
				if(strLine.Find (_T("[!--")) == -1)
				{
					//continue ;
				}
				else
				{
					nPos = strLine.Find (_T("[!--")) ;
					strFeildData = strLine.Mid (0,nPos) ;
					strControlNo = strLine.Mid (nPos+4,23) ;
					strControlNo.Replace(_T("-"), _T(""));
					strControlNo.Replace(_T("]"), _T(""));					
					strControlNo.Replace(_T("\n"), _T(""));
					strControlNo.Replace(_T("\r"), _T(""));
					strFeildData.TrimLeft () ; strFeildData.TrimRight () ;				
					
					if(i== 0) map_Basic_Mokcha.SetAt (strControlNo,strFeildData) ;
					else map_Last_Mokcha.SetAt (strControlNo,strFeildData) ;
					
				}

				strLine = strLine.Mid( nIndex+2 , strLine.GetLength()-nIndex );
				strLine.TrimLeft () ;strLine.TrimRight () ;
			}
			
			if( IsStop ) break;
		}
		
	}
	
	//�����ȣ üũ �ϱ� 
    INT ids ;
	CString strCompareLine ;
	CString strUpdateLine ;
	CString strCompareControlNo ;
	CString strUpdateControlNo ;
	CStringArray arrUpdateData ;

	POSITION pos ;
	for(pos = map_Basic_Mokcha.GetStartPosition () ; pos != NULL ;)
	{
	
		map_Basic_Mokcha.GetNextAssoc (pos,strCompareControlNo,strCompareLine ) ;
		
		ids = map_Last_Mokcha.Lookup (strCompareControlNo,strUpdateLine);
		if(ids == 0 )
		{
			return FALSE ;
		}
		
	}

	return TRUE ;
}

VOID CSE_CONTENTS_EDIT::OnbKISAEXPORT() 
{
	// ����
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );


	CString strQuery;
	strQuery = _T("SELECT ARTICLE FROM SE_ARTICLE_TBL WHERE VOL_KEY = ") + m_sVol_Key + _T("");
	dm.RestructDataManager( strQuery );


	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	CFileDialog dlg(FALSE, _T("*.*"), _T("����.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("��������(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return ;

	_tchdir(szCurrentDir);
	
	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return ;
    sFileName = dlg.GetPathName();	

	FILE *fp;
	fp = _tfopen( sFileName , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox(_T("������ �����ϼ���."));
		return ;
	}
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}

	for ( INT nIdx = 0 ; nIdx < dm.GetRowCount() ; nIdx ++ )
	{
		CString strStreamMarc;
		dm.GetCellData( nIdx, 0, strStreamMarc ) ;
		strStreamMarc += _T("\r\n");
		_ftprintf( fp, strStreamMarc );
	}

	fclose( fp );
	 
	CString sMessage;
	sMessage.Format( _T("������ %d�ǿ� ���� �����ۼ��� �Ϸ�Ǿ����ϴ�."), dm.GetRowCount() );
	AfxMessageBox( sMessage );


}

VOID CSE_CONTENTS_EDIT::OnbKISAIMPORT() 
{
	//��� ��ũ ����
	CKisaMarcImport dlg(this);
	//dlg.SetKisaData( strBody ) 
	dlg.DoModal();

	
}
HBRUSH CSE_CONTENTS_EDIT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
