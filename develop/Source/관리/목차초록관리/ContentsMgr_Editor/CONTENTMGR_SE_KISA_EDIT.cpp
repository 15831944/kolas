// CONTENTMGR_SE_KISA_EDIT.cpp : implementation file
//

#include "stdafx.h"
//#include _T("contentsmgr_editor.h")
#include "CONTENTMGR_SE_KISA_EDIT.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "CommonDefine.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SE_KISA_EDIT dialog


CCONTENTMGR_SE_KISA_EDIT::CCONTENTMGR_SE_KISA_EDIT(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCONTENTMGR_BO_EDIT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_DialogType = TRUE ;
	AfxInitRichEdit(); 

}

CCONTENTMGR_SE_KISA_EDIT::~CCONTENTMGR_SE_KISA_EDIT()
{

}

BOOL CCONTENTMGR_SE_KISA_EDIT::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CCONTENTMGR_SE_KISA_EDIT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCONTENTMGR_SE_KISA_EDIT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_SE_KISA_EDIT, CDialog)
	//{{AFX_MSG_MAP(CCONTENTMGR_SE_KISA_EDIT)
	ON_BN_CLICKED(IDCANCEL_SE, OnSe)
	ON_BN_CLICKED(IDC_bNEXT_SE, OnbNEXTSE)
	ON_BN_CLICKED(IDC_bPREV_SE, OnbPREVSE)
	ON_BN_CLICKED(IDC_bBRINGINC_SE, OnbBRINGINCSE)
	ON_BN_CLICKED(IDC_bSAVEC_SE, OnbSAVECSE)
	ON_BN_CLICKED(IDC_bCDELETE_SE, OnbCDELETESE)
	ON_BN_CLICKED(IDC_bBRINGINA_SE, OnbBRINGINASE)
	ON_BN_CLICKED(IDC_bSAVEA_SE, OnbSAVEASE)
	ON_BN_CLICKED(IDC_bADELETE_SE, OnbADELETESE)
	ON_BN_CLICKED(IDC_bDCMInput, OnbDCMInput)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_SE_KISA_EDIT message handlers

BOOL CCONTENTMGR_SE_KISA_EDIT::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rect1, rect2;

			
	( (CStatic*) GetDlgItem (IDC_sContestsRect) )->GetWindowRect(&rect1);
	( (CStatic*) GetDlgItem (IDC_sAbstractsRect) )->GetWindowRect(&rect2);

	ScreenToClient(&rect1);	
	ScreenToClient(&rect2);	
	
	m_TocEditdlg.MyCreate(this,rect1,_YON_);// _YON_ ,_DAN_SINGLE
	m_ChorokDlg.MyCreate(this,&m_TocEditdlg,rect2);

	INT ids ;
   
	if(InitESL_Mgr(_T("SM_CONTENTMGR_SE_KISA_EDIT"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}

    GetDlgItem(IDC_COMBO_KisaEdit1)->EnableWindow(FALSE); 
	
	m_MakeSearchData = new CMakeSearchData(this);

	ids = Init_Screen (0) ;
	if(ids <0)
	{
		//AfxMessageBox(_T("Init_Screen() Error~!")) ;
		return FALSE ;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CCONTENTMGR_SE_KISA_EDIT::Set_Mgr(CString CMAlias, CString DMAlias, CString GridAlias)
{
   m_ParentGrid =  GridAlias ;                            //_T("MainGrid_CONTENT")	  �׸��� Alias 
   m_ParentCM   =  CMAlias ;                            //_T("CM_CONTENTMGR_MAIN")  ����ȭ��
   m_ParentDM   =  DMAlias ;							   //_T("DM_BO_CONTENTMGR_SPECIES") ����ȭ�� DM
    
   return 0 ;
}

VOID CCONTENTMGR_SE_KISA_EDIT::OnSe() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

VOID CCONTENTMGR_SE_KISA_EDIT::OnbNEXTSE() 
{
	// TODO: Add your control notification handler code here
	Init_Screen(1); 
}

VOID CCONTENTMGR_SE_KISA_EDIT::OnbPREVSE() 
{
	// TODO: Add your control notification handler code here
	Init_Screen(-1) ;
}

INT CCONTENTMGR_SE_KISA_EDIT ::Init_Screen (INT nDir)
{
    INT ids ;
	
    CESL_DataMgr *pParentDM = FindDM(m_ParentDM );
	if(pParentDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1 ;
	}

    CESL_ControlMgr *pParentCM = FindCM(m_ParentCM );
	if(pParentCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!"));
		return -2 ;
	}

	CESL_Grid *pParentGrid = (CESL_Grid*)pParentCM->FindControl (m_ParentGrid );
	if(pParentGrid == NULL) {
		AfxMessageBox(_T("FindControl() Error~!"));
		return -3 ;
	}
    
//	m_TocEditdlg.SetTocFromString(_T(""));  //������ 
//	m_ChorokDlg.SetChorok(_T(""));          //����ʷ� 
    

	switch(nDir)
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
	
//	CString m_Article_Rec_Key ,m_Article_Control_No,m_Article_Title,m_Article_Author,m_Article_Page ;
//  CString m_Articl_Contents ,m_Article_Abstracts ;
    

    if(m_DialogType == TRUE)
	{

		ids = pParentDM->GetCellData (_T("REC_KEY"),m_idx ,m_Article_Rec_Key) ;
		 //2004-03-04 [������ ����]
		if(m_Article_Rec_Key ==  _T(""))
		{
			AfxMessageBox (_T("��� ������ �����Ͽ� �ֽʽÿ�")) ;
			EndDialog(IDCANCEL);
			return -222;
		}
    
		ids = pParentDM->GetCellData (_T("ARTICLE_CONTROL_NO"),m_idx,m_Article_Control_No) ;

		ids = pParentDM->GetCellData (_T("ARTICLE_INFO"),m_idx,m_Article_Title) ;
		
		ids = pParentDM->GetCellData (_T("ARTICLE_AUTHOR_INFO"),m_idx,m_Article_Author) ;

		ids = pParentDM->GetCellData (_T("ARTICLE_PAGE_INFO"),m_idx,m_Article_Page) ;
		
		ids = pParentDM ->GetCellData (_T("ARTICLE_TOC_YN"),m_idx,m_Article_Toc_YN);

		ids = pParentDM ->GetCellData (_T("ABS_YN"),m_idx,m_Article_Abs_YN);

	}
    else
	{
        ids = pParentDM->GetCellData (_T("���Ű"),m_idx,m_Article_Rec_Key) ;
        //2004-03-04 [������ ����]
		if(m_Article_Rec_Key ==  _T(""))
		{
			AfxMessageBox (_T("��� ������ �����Ͽ� �ֽʽÿ�")) ;
			EndDialog(IDCANCEL);
			return -222;
		}

		ids = pParentDM->GetCellData (_T("��������ȣ"),m_idx,m_Article_Control_No) ;

		ids = pParentDM->GetCellData (_T("����"),m_idx,m_Article_Title) ;
		
		ids = pParentDM->GetCellData (_T("����"),m_idx,m_Article_Author) ;

		ids = pParentDM->GetCellData (_T("������"),m_idx,m_Article_Page) ;
		
        ids = pParentDM->GetCellData (_T("��ȣŰ"),m_idx,m_Vol_Key) ;

		CString sQuery;
		sQuery.Format (_T("SELECT  ARTICLE_TOC_YN FROM IDX_SE_TOC_TBL WHERE REC_KEY = %s"),m_Article_Rec_Key);
        ids = pParentDM->GetOneValueQuery (sQuery,m_Article_Toc_YN );
		//ids = pParentDM ->GetCellData (_T("ARTICLE_TOC_YN"),m_idx,m_Article_Toc_YN);
		
		sQuery.Format (_T("SELECT  ABS_YN FROM IDX_SE_TOC_TBL WHERE REC_KEY = %s"),m_Article_Rec_Key);
		ids = pParentDM->GetOneValueQuery (sQuery,m_Article_Abs_YN );
		//ids = pParentDM ->GetCellData (_T("ABS_YN"),m_idx,m_Article_Abs_YN);
	}

    //DM_SE_ARTICLE_ABSTRACTS_TBL,DM_SE_ARTICLE_TOC_TBL
    //m_Article_Con_DM ,m_Article_Abs_DM
    
	m_Article_Con_DM = FindDM(_T("DM_SE_ARTICLE_TOC_TBL"));
	if(m_Article_Con_DM == NULL) {
		AfxMessageBox(_T("FindDM(DM_SE_ARTICLE_TOC_TB) Error~!"));
		return -4;
	}

	m_Article_Abs_DM = FindDM(_T("DM_SE_ARTICLE_ABSTRACTS_TBL"));
	if(m_Article_Con_DM == NULL) {
	AfxMessageBox(_T("FindDM(DM_SE_ARTICLE_ABSTRACTS_TBL) Error~!"));
	return -5;
	}

    ///////////////////////////////////////////////////////////////
	// ȭ�� ���� 
    ///////////////////////////////////////////////////////////////
	//m_TocEditdlg.SetTocFromString(m_strCSource);  //������ 
	//m_ChorokDlg.SetChorok(m_strASource);          //����ʷ� 

    CESL_ControlMgr *pCurrentCM = FindCM(_T("CM_CONTENTMGR_SE_KISA_EDIT")) ;
	if(pCurrentCM == NULL){
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -5 ;
	}
    
	

	ids = pCurrentCM->SetControlMgrData (_T("��������ȣ"),m_Article_Control_No,-1);
    ids = pCurrentCM->SetControlMgrData (_T("���"),m_Article_Title,-1);
	ids = pCurrentCM->SetControlMgrData (_T("����"),m_Article_Author,-1);
	ids = pCurrentCM->SetControlMgrData (_T("������"),m_Article_Page,-1);

    


    if(m_Article_Toc_YN == _T("Y"))
	{
		SetDlgItemText(IDC_STATIC_01_SE,_T("<�� �� : ����>"));
		GetDlgItem(IDC_bCDELETE_SE)->EnableWindow(true);
		m_TocEditdlg.SetReadOnly(false);
        
		CString sWhere ;
		sWhere.Format (_T("ARTICLE_KEY = %s"),m_Article_Rec_Key);
        ids = m_Article_Con_DM->RefreshDataManager (sWhere) ;
		ids = m_Article_Con_DM->GetCellData (_T("CONTENTS"),0,m_Articl_Contents);
			
	}
	else
	{
        SetDlgItemText(IDC_STATIC_01_SE,_T("<�� �� : ����>"));
		GetDlgItem(IDC_bCDELETE_SE)->EnableWindow(false);
		m_TocEditdlg.SetReadOnly(false);
		m_Articl_Contents.Empty ();
	}
    

    if(m_Article_Abs_YN == _T("Y"))
	{
		
		SetDlgItemText(IDC_STATIC_02_SE,_T("<�� �� : ����>"));
		GetDlgItem(IDC_bCDELETE_SE)->EnableWindow(true);
	//	m_TocEditdlg.SetReadOnly(false);
			
        CString sWhere ;
		sWhere.Format (_T("ARTICLE_KEY = %s"),m_Article_Rec_Key);
        ids = m_Article_Abs_DM->RefreshDataManager (sWhere) ;
		ids = m_Article_Abs_DM->GetCellData (_T("ABSTRACTS"),0,m_Article_Abstracts);

	}
	else
	{
        SetDlgItemText(IDC_STATIC_02_SE,_T("<�� �� : ����>"));
		GetDlgItem(IDC_bADELETE_SE)->EnableWindow(false);
	//	m_TocEditdlg.SetReadOnly(false);
        m_Article_Abstracts.Empty ();
		  
	}
    

    m_TocEditdlg.SetTocFromString(m_Articl_Contents);  //������ 
	m_ChorokDlg.SetChorok(m_Article_Abstracts);          //����ʷ� 
    //�Է°� : �����ȣ ,��ȣŰ,û����ȣ,��������,�⵵,������,�Ѽ���
    //m_TocEditdlg.SetTocDefault(_T(m_Article_Control_No), _T("1call"), _T("1Count"), _T("1Date"),
	//	_T("1Pub"), _T("1Sertitle"), _T("1Title") );

    CString sVol_Key ,sTitle_Info ,sVol_Title,sPUB_YEAR_INFO;
	
	
	if(m_DialogType == TRUE) 
	{
		ids = GetDataMgrData(m_ParentDM ,_T("VOL_KEY"),sVol_Key,m_idx);
		ids = GetDataMgrData(m_ParentDM ,_T("TITLE_INFO"),sTitle_Info,m_idx);
		//ids = GetDataMgrData(m_ParentDM ,_T("VOL_INFO"),sVOL_INFO,m_idx);
		ids = GetDataMgrData(m_ParentDM ,_T("PUB_YEAR_INFO"),sPUB_YEAR_INFO,m_idx);
    }
    else
	{
	    ids = GetDataMgrData(m_ParentDM ,_T("��ȣŰ"),sVol_Key,m_idx);
		CString strQuery ;
		strQuery.Format (_T("select title_info from IDX_SE_TOC_TBL where rec_key =%s"),m_Article_Rec_Key );
        m_Article_Abs_DM->GetOneValueQuery (strQuery,sTitle_Info);
		strQuery.Format (_T("select PUB_YEAR_INFO from IDX_SE_TOC_TBL where rec_key =%s"),m_Article_Rec_Key );
        m_Article_Abs_DM->GetOneValueQuery (strQuery,sPUB_YEAR_INFO);
	}


	CString strQuery, sPublish_Date;
	strQuery.Format (_T("select publish_date from SE_vol_tbl where rec_key = %s"),sVol_Key);
    m_Article_Abs_DM->GetOneValueQuery (strQuery,sPublish_Date);

    strQuery.Format (_T("select vol_title from SE_vol_tbl where rec_key = %s"),sVol_Key);
    m_Article_Abs_DM->GetOneValueQuery (strQuery,sVol_Title);



    CString strDefualt[8];
    strDefualt[0] = m_Article_Control_No ;   //  ��������ȣ 
    strDefualt[1] = sVol_Key ;               //  ��ȣ KEY
	strDefualt[2] = _T("") ;                     //  �Ѽ���
	strDefualt[3] = sTitle_Info ;            //  ����
	strDefualt[4] = sVol_Title ;              //  ��ȣ��
	strDefualt[5] = sPublish_Date ;          //  ������
	strDefualt[6] = _T("") ;                     //  û����ȣ
    strDefualt[7] = _T("") ;                     //  �������� 

	m_TocEditdlg.SetTocDefault (strDefualt) ;
    
	m_arrBringDefault[0] = _T("<id>")+strDefualt[0]+_T("\r\n");
	m_arrBringDefault[1] = _T("<call>")+strDefualt[1]+_T("\r\n"); 
	m_arrBringDefault[2] = _T("<count>")+strDefualt[2]+_T("\r\n") ;
	m_arrBringDefault[3] = _T("<date>")+strDefualt[3]+_T("\r\n");    
	m_arrBringDefault[4] = _T("<pub>")+strDefualt[4]+_T("\r\n") ;
	m_arrBringDefault[5] = _T("<sertitle>")+strDefualt[5]+_T("\r\n") ;
	m_arrBringDefault[6] = _T("<title>")+strDefualt[6]+_T("\r\n") ;

	//m_arrBringDefault[7] = _T("<body>")+strDefualt[7]+_T("\r\n</body>") ;

//	ids = pCurrentCM->SetControlMgrData (_T("��������ȣ"),m_Article_Control_No,-1);


	return 0 ;
}



//////////////////////////////////////////////////////////////////////////////
//������ ���Ϲ��� 
//////////////////////////////////////////////////////////////////////////////
VOID CCONTENTMGR_SE_KISA_EDIT::OnbBRINGINCSE() 
{
	if(!BringIn(0))AfxMessageBox(_T("������ ���Խ���"));
}

//////////////////////////////////////////////////////////////////////////////
//������ ����  
//////////////////////////////////////////////////////////////////////////////
VOID CCONTENTMGR_SE_KISA_EDIT::OnbSAVECSE() 
{
	Save(0);
	m_TocEditdlg.SetTocFromString(m_strConAbs);
	
}

//////////////////////////////////////////////////////////////////////////////
//������ ���� 
//////////////////////////////////////////////////////////////////////////////
VOID CCONTENTMGR_SE_KISA_EDIT::OnbCDELETESE() 
{
	Delete(0);
	m_TocEditdlg.SetTocFromString(_T(""));
	
}

//////////////////////////////////////////////////////////////////////////////
//����ʷ� ���Ϲ��� 
//////////////////////////////////////////////////////////////////////////////
VOID CCONTENTMGR_SE_KISA_EDIT::OnbBRINGINASE() 
{
	if(!BringIn(1))AfxMessageBox(_T("����ʷ� ���Խ���"));
}

//////////////////////////////////////////////////////////////////////////////
//����ʷ� ���� 
//////////////////////////////////////////////////////////////////////////////
VOID CCONTENTMGR_SE_KISA_EDIT::OnbSAVEASE() 
{
	Save(1);
	m_ChorokDlg.SetChorok(m_strConAbs);
	
}

//////////////////////////////////////////////////////////////////////////////
//����ʷ� ���� 
//////////////////////////////////////////////////////////////////////////////
VOID CCONTENTMGR_SE_KISA_EDIT::OnbADELETESE() 
{
	Delete(1);
	m_ChorokDlg.SetChorok(_T(""));
	
}


BOOL CCONTENTMGR_SE_KISA_EDIT::BringIn(INT nType)
{
	// ���� ��θ� �����Ѵ�.
	INT nCodePage = 0;// UNICODE : 0, UTF-8 : 1, ANSI : 2
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
		if(!file.Open(filedlg.GetPathName(),CFile::modeRead)){
			AfxMessageBox(_T("ȭ���� �� �� �����ϴ�."));
			goto ERR_RETURN;
		}
		else{
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				//===================================================
				//2010.05.24 ADD BY PJW : ������ CODE PAGE�� ���� ��ȯ�� �ش�.
//				file.SeekToBegin();
				file.SeekToBegin();
				file.Read( &cUni, 3 );
				if(  0xBBEF == cUni || 0xAAEB == cUni )
				{/// UTF�� ���
					//fFile.SetCodePage(CP_UTF8);
					nCodePage = 1;
				}
//				else if( 0xFFFE == cUni )
//				{
//				}
				else
				{/// ANSI�� ���
					//fFile.SetCodePage(CP_ACP);
					file.SeekToBegin();
					nCodePage = 2;
				}
				//===================================================	
			}
			
			// 1. �������� ������
			CString strBackup;
			if(nType==0)
				strBackup=m_TocEditdlg.GetFinalToc();
			else
				strBackup=m_ChorokDlg.GetChorokText();

			// 2. ���µ� ��Ʈ�� ���� �����⿡ �־��ش�.
			CString strFileStream;
			//===================================================
			//2010.05.24 ADD BY PJW : ������ CODE PAGE�� ���� ��ȯ�� �ش�.
//			if(file.ReadHuge( strFileStream.GetBuffer(file.GetLength()),file.GetLength())<=0){
//				strFileStream.ReleaseBuffer();
//				goto ERR_RETURN;
//			}
//			strFileStream.ReleaseBuffer();
			if( nCodePage == 0 )
			{
				if(file.ReadHuge( strFileStream.GetBuffer(file.GetLength()),file.GetLength())<=0){
					strFileStream.ReleaseBuffer();
					goto ERR_RETURN;
				}
				strFileStream.ReleaseBuffer();
			}
			else
			{
				CHAR * cpData;
				DWORD dSize = file.GetLength();

				cpData = (CHAR *)malloc(sizeof(CHAR) * (dSize ));
				file.ReadHuge(cpData, dSize);
				cpData[dSize] = '\0';
				file.Close();

				CharConvert cvt;
				TCHAR *lpBuffer;
				INT nNeedByte;
				if( nCodePage == 2)
				{
					nNeedByte = MultiByteToWideChar( CP_ACP, 0, cpData, -1, NULL, 0 );
					lpBuffer = (TCHAR *)malloc( sizeof( TCHAR )*nNeedByte+sizeof( TCHAR ) );
					cvt.MultiByteToUnicode(cpData, &lpBuffer, &nNeedByte );	
				}
				else
				{
					INT nLen;
					nLen = strlen(cpData);
					nNeedByte = MultiByteToWideChar( CP_UTF8, 0, cpData, nLen, NULL, 0 );
					lpBuffer = (TCHAR *)malloc( sizeof( TCHAR )*nNeedByte+sizeof( TCHAR ) );
					lpBuffer[nNeedByte] = '\0';
					nNeedByte = MultiByteToWideChar(	CP_UTF8, 0, cpData, nLen, lpBuffer, nNeedByte);
				}
//				free(cpData);
//				free(lpBuffer);

				strFileStream.Replace(_T("\r\n"), _T("\n"));
				strFileStream.Replace(_T("\r"), _T("\n"));
				strFileStream.Replace(_T("\n"), _T("\r\n"));
				strFileStream.Format(_T("%s"), lpBuffer);
			}
			//===================================================

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



// nType      0 : ����          1:�ʷ�   
VOID CCONTENTMGR_SE_KISA_EDIT::Save(INT nType)
{
	INT ids ;

	CString sCurrent_Date ;
	CTime t = CTime::GetCurrentTime ();
	sCurrent_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
 
	CESL_DataMgr *pDM = FindDM(_T("DM_SE_ARTICLE_TOC_TBL")) ;
	if(pDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}
    
	// Syntex Check and Get Values
    if(nType==0){
        m_strConAbs=m_TocEditdlg.GetFinalToc();
        if(m_TocEditdlg.CheckSyntax())return;
    }
    else {
        m_strConAbs=m_ChorokDlg.GetChorokText();
    }
	
	CString m_Rec_Key ,m_Abs_Use_Limit;

	pDM->MakeRecKey (m_Rec_Key);

    pDM->StartFrame ();
	pDM->InitDBFieldData ();
	
	// ������ �Է� & ���� /////////////////////////////////////////////////
	if(nType == 0 )
	{   

		if( m_Article_Toc_YN != _T("Y")) // INSERT  
		{
			pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),m_Rec_Key);
			pDM->AddDBFieldData (_T("ARTICLE_KEY"),_T("NUMERIC"),m_Article_Rec_Key);
			pDM->AddDBFieldData (_T("CONTENTS"),_T("STRING"),m_strConAbs);
            pDM->AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),sCurrent_Date);
			
			ids = pDM->MakeInsertFrame (_T("SE_ARTICLE_TOC_TBL"));
            if(ids <0) {
				AfxMessageBox(_T("MakeInsertFrame() Error~!"));
				pDM->EndFrame ();
				return ;
			}

			/*
			////////////////////////////////////////////////////
			// UPDATE : IDX_SE_TOC_TBL .
			////////////////////////////////////////////////////

		    ids = Update_Index_Table (pDM , 0 ,m_Article_Rec_Key) ;
            if(ids <0) {
				AfxMessageBox(_T("Update_Index_Table() Error~!"));
				pDM->EndFrame ();
				return ;
			}*/

            ids = SetDataMgrData (m_ParentDM ,_T("ARTICLE_TOC_YN"),_T("Y"),m_idx);
            
            if(m_DialogType == TRUE)
			{
			  CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM ,m_ParentGrid );
			  pGrid->SetAt (m_idx,4,_T("Y"));
			}

		}
		else  //UPDATE
		{
			
			ids = pDM->AddDBFieldData (_T("CONTENTS"),_T("STRING"),m_strConAbs);
			ids = pDM->MakeUpdateFrame (_T("SE_ARTICLE_TOC_TBL"),_T("ARTICLE_KEY"),_T("NUMERIC"),m_Article_Rec_Key);
            if(ids <0) {
				AfxMessageBox(_T("MakeUpdateFrame() Error~!"));
				pDM->EndFrame ();
				return ;
			}
           
		}

	}

	// ����ʷ� �Է� & ���� /////////////////////////////////////////////////
    else
	{
		if(m_Article_Abs_YN !=_T("Y"))
		{
        	pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),m_Rec_Key);
			pDM->AddDBFieldData (_T("ARTICLE_KEY"),_T("NUMERIC"),m_Article_Rec_Key);
			pDM->AddDBFieldData (_T("ABSTRACTS"),_T("STRING"),m_strConAbs);
			pDM->AddDBFieldData (_T("ARTICLE_CONTROL_NO"),_T("STRING"),m_Article_Control_No );

            ids = GetControlData (_T("CM_CONTENTMGR_SE_KISA_EDIT"),_T("�ʷ��̿����ѱ���"),m_Abs_Use_Limit,-1);
            pDM->AddDBFieldData (_T("USE_LIMIT_CODE"),_T("STRING"),m_Abs_Use_Limit );

            pDM->AddDBFieldData (_T("CREATE_DATE"),_T("STRING"),sCurrent_Date);

			ids = pDM->MakeInsertFrame (_T("SE_ABSTRACTS_TBL"));
            if(ids <0) {
				AfxMessageBox(_T("MakeInsertFrame(SE_ABSTRACTS_TBL) Error~!"));
				return ;
				pDM->EndFrame () ;
			}
            
			ids = SetDataMgrData (m_ParentDM ,_T("ABS_YN"),_T("Y"),m_idx);
			CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM ,m_ParentGrid );
			pGrid->SetAt (m_idx,5,_T("Y"));

		}
		else
		{
			pDM->AddDBFieldData (_T("ABSTRACTS"),_T("STRING"),m_strConAbs);
			ids = GetControlData (_T("CM_CONTENTMGR_SE_KISA_EDIT"),_T("�ʷ��̿����ѱ���"),m_Abs_Use_Limit,-1);
            pDM->AddDBFieldData (_T("USE_LIMIT_CODE"),_T("STRING"),m_Abs_Use_Limit );
         
           	ids = pDM->MakeUpdateFrame (_T("SE_ABSTRACTS_TBL"),_T("ARTICLE_KEY"),_T("NUMERIC"),m_Article_Rec_Key);
            if(ids <0) {
				AfxMessageBox(_T("MakeUpdateFrame(SE_ABSTRACTS_TBL) Error~!"));
				pDM->EndFrame ();
				return ;
			}			
		}
	}
    
	// COMMIT ////////////////////////////////////////////////////////////////

	ids = pDM->SendFrame () ;
	if(ids <0) 
	{
		AfxMessageBox(_T("SendFrame() Error~!"));
		pDM->EndFrame ();
		return ;
	}

	ids = pDM->EndFrame () ;
	if(ids <0) 
	{
		AfxMessageBox(_T("EndFrame() Error~!"));
		pDM->EndFrame ();
		return ;
	}

	////////////////////////////////////////////////////
	// UPDATE : IDX_SE_TOC_TBL .
	////////////////////////////////////////////////////
	
	ids = Update_Index_Table (pDM , nType ,m_Article_Rec_Key) ;
	if(ids <0) {
		AfxMessageBox(_T("Update_Index_Table() Error~!"));
		pDM->EndFrame ();
		return ;
	}	
    
	if(nType == 0)
	{
	    SetDlgItemText(IDC_STATIC_01_SE,_T("<�� �� : ����>"));
		GetDlgItem(IDC_bCDELETE_SE)->EnableWindow(true);
		MessageBox(_T("�������� �����߽��ϴ�."));
	}
	else
	{
		SetDlgItemText(IDC_STATIC_02_SE,_T("<�� �� : ����>"));
		GetDlgItem(IDC_bADELETE_SE)->EnableWindow(true);
		MessageBox(_T("����ʷ��� �����߽��ϴ�."));

	}
    

/*	
    CESL_ControlMgr *pParentCM = FindCM(m_ParentCM );
	if(pParentCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!"));
		return -2 ;
	}

	CESL_Grid *pParentGrid = (CESL_Grid*)pParentCM->FindControl (m_ParentGrid );
	if(pParentGrid == NULL) {
		AfxMessageBox(_T("FindControl() Error~!"));
		return -3 ;
	}
*/
//	AllControlDisplay (m_ParentCM  ,-1) ;
}

INT CCONTENTMGR_SE_KISA_EDIT::Update_Index_Table(CESL_DataMgr *pTempDM, INT nType, CString m_Article_RecKey)
{
    INT ids;

	pTempDM->StartFrame();

	if(nType == 0 )
	{   
		CString sBody;
	    sBody=m_TocEditdlg.GetFinalToc();
     
		/////////////////////////////////////////////////////////////////////
		//�ε��� Ű���� ���� �ϱ� 
		/////////////////////////////////////////////////////////////////////
		MakeIndexKeyWord(sBody) ;

		pTempDM->InitDBFieldData ();
		pTempDM->AddDBFieldData (_T("ARTICLE_TOC_YN"),_T("STRING"),_T("Y"));
		pTempDM->AddDBFieldData (_T("IDX_ARTICLE_TOC_KEYWORD"),_T("STRING"),sBody);
		ids = pTempDM->MakeUpdateFrame (_T("IDX_SE_TOC_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_Article_RecKey);
        if(ids<0) {
			pTempDM->EndFrame();
			AfxMessageBox(_T("MakeUpdateFrame() Error~!"));
			return -1 ;
		}
        		
	   //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		 //////////////////////////////////////////////////////////////////////////
		 // INSERT IDX_TMP_SE_TOC_TBL 
		 //////////////////////////////////////////////////////////////////////////
		 pTempDM->InitDBFieldData ();

		 CString strTmp_Key ;
		 CString system_Date ;
		 CTime t = CTime::GetCurrentTime ();
		 system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());


		 pTempDM->MakeRecKey (strTmp_Key) ;
		 pTempDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
		 pTempDM->AddDBFieldData (_T("ARTICLE_KEY"),_T("NUMERIC"),m_Article_RecKey);
		 pTempDM->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("I"));
		 pTempDM->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);
  
		 ids = pTempDM->MakeInsertFrame (_T("IDX_TMP_SE_TOC_TBL"));
		 if(ids<0) {
			 AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_SE_TOC_TBL) Error~!"));
			 pTempDM->EndFrame();
			 return 100 ;
		 }

		 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.11.13 ������ �߰� 
		if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{   
			CString strData ;
			strData.Format(_T("ECO$P_MAKE_SETOC$I(%s);"), m_Article_RecKey);
			pTempDM->AddFrame(strData);
		}
	}
	else
	{
		pTempDM->InitDBFieldData ();
	    pTempDM->AddDBFieldData (_T("ABS_YN"),_T("STRING"),_T("Y"));
		ids = pTempDM->MakeUpdateFrame (_T("IDX_SE_TOC_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_Article_RecKey);
        if(ids<0) {
			AfxMessageBox(_T("MakeUpdateFrame() Error~!"));
			pTempDM->EndFrame();
			return -2 ;
		}

	     //////////////////////////////////////////////////////////////////////////
		 // INSERT IDX_TMP_SE_TOC_TBL 
		 //////////////////////////////////////////////////////////////////////////
		 pTempDM->InitDBFieldData ();

		 CString strTmp_Key ;
		 CString system_Date ;
		 CTime t = CTime::GetCurrentTime ();
		 system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());


		 pTempDM->MakeRecKey (strTmp_Key) ;
		 pTempDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
		 pTempDM->AddDBFieldData (_T("ARTICLE_KEY"),_T("NUMERIC"),m_Article_RecKey);
		 pTempDM->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("I"));
		 pTempDM->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);
  
		 ids = pTempDM->MakeInsertFrame (_T("IDX_TMP_SE_TOC_TBL"));
		 if(ids<0) {
			 AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_SE_TOC_TBL) Error~!"));
			 pTempDM->EndFrame();
			 return 100 ;
		 }
    }

	ids = pTempDM->SendFrame();
	if(ids) return ids;

	pTempDM->EndFrame();
	return 0 ;
}


VOID CCONTENTMGR_SE_KISA_EDIT::Delete(INT nType)
{
	 INT ids;

     CESL_DataMgr *pDM = FindDM(_T("DM_SE_ARTICLE_TOC_TBL"));
	 if(pDM == NULL)
	 {
		 AfxMessageBox(_T("FindDM() Error~!"));
		 return ;
	 }

	 pDM->StartFrame ();
	 
     if(nType == 0 )
	 {
		 pDM->InitDBFieldData () ;
		 ids = pDM->MakeDeleteFrame (_T("SE_ARTICLE_TOC_TBL"),_T("ARTICLE_KEY"),_T("NUMERIC"),m_Article_Rec_Key);
         if(ids <0) {
			 AfxMessageBox(_T("MakeDeleteFrame() Error~!"));
			 pDM->EndFrame ();
			 return ;
		 }
         
		 pDM->InitDBFieldData ();
		 pDM->AddDBFieldData (_T("ARTICLE_TOC_YN"),_T("STRING"),_T("N"));
		 ids = pDM->MakeUpdateFrame (_T("IDX_SE_TOC_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_Article_Rec_Key);
         if(ids <0) {
			 AfxMessageBox(_T("MakeUpdateFrame() Error~!"));
			 pDM->EndFrame ();
			 return ;
		 }

        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.11.13 ������ �߰� 
		if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{   
			CString strData ;
			strData.Format(_T("ECO$P_MAKE_SETOC(%s)$I;"), m_Article_Rec_Key);
			pDM->AddFrame(strData);
		}
		
	    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




		 if(m_DialogType == TRUE)
		 {
			 CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM ,m_ParentGrid );
			 pGrid->SetAt (m_idx,4,_T("N"));
		 }
		 	
	 }
	 else
	 {
		 pDM->InitDBFieldData () ;
		 ids = pDM->MakeDeleteFrame (_T("SE_ABSTRACTS_TBL"),_T("ARTICLE_KEY"),_T("NUMERIC"),m_Article_Rec_Key);
         if(ids <0) {
			 AfxMessageBox(_T("MakeDeleteFrame() Error~!"));
			 pDM->EndFrame ();
			 return ;
		 }

		 pDM->InitDBFieldData ();
		 pDM->AddDBFieldData (_T("ABS_YN"),_T("STRING"),_T("N"));
		 ids = pDM->MakeUpdateFrame (_T("IDX_SE_TOC_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_Article_Rec_Key);
         if(ids <0) {
			 AfxMessageBox(_T("MakeUpdateFrame() Error~!"));
			 pDM->EndFrame ();
			 return ;
		 }
        if(m_DialogType == TRUE)
		{
			ids = SetDataMgrData (m_ParentDM ,_T("ABS_YN"),_T("N"),m_idx);
			CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM ,m_ParentGrid );
			pGrid->SetAt (m_idx,5,_T("N"));
		}
	 }

	 pDM->SendFrame ();
	 pDM->EndFrame () ;
     
	 if(nType == 0)
	{
	    SetDlgItemText(IDC_STATIC_01_SE,_T("<�� �� : ����>"));
		GetDlgItem(IDC_bCDELETE_SE)->EnableWindow(false);
		MessageBox(_T("�������� �����߽��ϴ�."));
	}
	else
	{
		SetDlgItemText(IDC_STATIC_02_SE,_T("<�� �� : ����>"));
		GetDlgItem(IDC_bADELETE_SE)->EnableWindow(false);
		MessageBox(_T("����ʷ��� �����߽��ϴ�."));

	}

    //Init_Screen(0) ;

}


CString CCONTENTMGR_SE_KISA_EDIT::ReMake_BringInStream(INT nType,CString strStream)
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
		 AfxMessageBox (str_BringStream) ;
		 
	}
	else
	{


	}

    
	return str_BringStream ;
	
}
VOID CCONTENTMGR_SE_KISA_EDIT::OnbDCMInput() 
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
    //m_Article_Rec_Key.Format (_T("%d"),60152245);
	sWhere.Format (_T("SELECT CONTENTS FROM CO_DCM_TOC_TBL WHERE ARTICLE_KEY = %s"),m_Article_Rec_Key) ;

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

INT CCONTENTMGR_SE_KISA_EDIT::MakeIndexKeyWord(CString &tmpValue)
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

CString CCONTENTMGR_SE_KISA_EDIT::KeywordMake( CString strItemData )
{
EFS_BEGIN

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
	}

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


INT CCONTENTMGR_SE_KISA_EDIT::SearchWord(CString sWord)
{
EFS_BEGIN

	INT ids;

	ids = SearchInFile(_T("..\\CFG\\CO_WORD_TBL.TXT"), sWord);
	if ( ids ) return ids;	
	return 0;

EFS_END
return -1;

}

INT CCONTENTMGR_SE_KISA_EDIT::SearchAuxiliaryWord( CString sWord, CString &sResult )
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
INT CCONTENTMGR_SE_KISA_EDIT::SearchUnlessWord( CString sWord )
{
EFS_BEGIN

	INT ids;

	ids = SearchInFile(_T("..\\CFG\\CO_UNLESS_WORD_TBL.TXT"), sWord);
	if ( ids ) return ids;
	return 0;

EFS_END
return -1;

}



HBRUSH CCONTENTMGR_SE_KISA_EDIT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}