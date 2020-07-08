// Light_UC_NOTIFY_DETAIL.cpp : implementation file
//

#include "stdafx.h"
#include "Light_UC_NOTIFY_DETAIL.h"
#include "Light_UC_NOTIFY_03.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLight_UC_NOTIFY_DETAIL dialog


CLight_UC_NOTIFY_DETAIL::CLight_UC_NOTIFY_DETAIL(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_DbClckMode = FALSE ;
	m_idx = -1;


}

CLight_UC_NOTIFY_DETAIL::~CLight_UC_NOTIFY_DETAIL()
{	
	
}

BOOL CLight_UC_NOTIFY_DETAIL::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CLight_UC_NOTIFY_DETAIL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLight_UC_NOTIFY_DETAIL)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLight_UC_NOTIFY_DETAIL, CDialog)
	//{{AFX_MSG_MAP(CLight_UC_NOTIFY_DETAIL)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bINCLUDE_FILE, OnbINCLUDEFILE)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnFileDownload, OnbtnFileDownload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLight_UC_NOTIFY_DETAIL message handlers

BOOL CLight_UC_NOTIFY_DETAIL::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(InitESL_Mgr(_T("SM_UC_NOTIFY_DETAIL"))<0)
	{
		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
    
	//////////////////////////////////////////////////////////////////////////////////
	// 2. ÷������ �̹����� ��ư�� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////
	m_button1.AutoLoad( IDC_bINCLUDE_FILE, this );
	
	if(m_DbClckMode == true)
	{
       DbClick_Move (0) ;
	}
	else MoveDataAndDisplay(0) ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLight_UC_NOTIFY_DETAIL::MoveDataAndDisplay(INT ndir)
{
	
	CString m_ParentCM;
	CString m_ParentGrid ;
	CString m_CurrentCM ;
 

    m_CurrentCM = _T("CM_UC_NOTIFY_DETIAL");
    m_ParentCM =_T("CM_UC_NOTIFY_MAIN");
    m_ParentGrid = _T("MainGrid") ;

	
	CESL_ControlMgr *pCtrlMgr = FindCM(m_ParentCM );
	if(pCtrlMgr == NULL) {
		AfxMessageBox(_T("FindCM() Error~!"));
		return - 1 ;
	}
   
    
	CESL_Grid * pGrid = (CESL_Grid*)pCtrlMgr->FindControl (m_ParentGrid );

	INT  ids ;
	switch (ndir)
	{
	case 0 :

		SelectMakeList(m_ParentCM,m_ParentGrid);
		ids = SelectGetHeadPosition(m_ParentCM,m_ParentGrid);
    	if (ids < 0)
		{
			AfxMessageBox(_T("���� ����� ������ �� �����ϴ�."));	// ����
			EndDialog(IDCANCEL);
			return -222;
		}
		break;

	case -1 :

        ids = SelectGetPrev (m_ParentCM,m_ParentGrid);
		if (ids == -1) {
			ids = SelectGetHeadPosition(m_ParentCM,m_ParentGrid);
			AfxMessageBox(_T("ó�� �ڷ��Դϴ�."));
			return -223;
		}
		break;
    
	case 1 :

		ids = SelectGetNext (m_ParentCM,m_ParentGrid);
		if (ids == -1) {
			ids = SelectGetTailPosition(m_ParentCM,m_ParentGrid);
			AfxMessageBox(_T("������ �ڷ��Դϴ�."));
			return -224;
		}
		break;
	}
     
    
    m_idx = ids; 
    
	CString RecKey ;

    ids = GetDataMgrData (_T("DM_UC_NOTIFY_MAIN"), _T("REC_KEY"), RecKey ,m_idx );
	
    ids = GetFileInfo( RecKey, m_idx );



//	SetReverse(m_ParentCM,m_ParentGrid ,m_idx ) ;
//	ids = AllControlDisplay (m_CurrentCM ,m_idx) ;  
    CString sConfirm ;
	ids = GetDataMgrData(_T("DM_UC_NOTIFY_MAIN"),_T("Ȯ��"),sConfirm,m_idx) ;

	if(sConfirm == _T("0"))
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_UC_NOTIFY_MAIN")) ;
		if(pDM == NULL) {
			AfxMessageBox (_T("FindDM() Error~!"));
			return - 234 ;
		}
		pDM->StartFrame ();
		pDM->InitDBFieldData () ;
		pDM->AddDBFieldData (_T("confirm"),_T("STRING"),_T("1")) ;
		pDM->MakeUpdateFrame (_T("UC_NOTIFY_TBL"),_T("REC_KEY"),_T("NUMERIC"),RecKey) ;
		pDM->SendFrame ();
		pDM->EndFrame () ;	
	}


	SetReverse(m_ParentCM,m_ParentGrid ,m_idx ) ;
	ids = AllControlDisplay (m_CurrentCM ,m_idx) ;  

    return 0;

}

VOID CLight_UC_NOTIFY_DETAIL::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	if(m_DbClckMode == true)
	{
       DbClick_Move (1) ;
	}
	else MoveDataAndDisplay(1) ;
	
}

VOID CLight_UC_NOTIFY_DETAIL::OnbPREV() 
{
	// TODO: Add your control notification handler code here
	if(m_DbClckMode == true)
	{
       DbClick_Move (-1) ;
	}
	else MoveDataAndDisplay(-1) ;
	
}

INT CLight_UC_NOTIFY_DETAIL::GetFileInfo(CString parentReckey, INT rowindex)
{
	CESL_DataMgr * m_DataMgr;
	CString        SqlBuf, FileName, FileSize;
	INT            ids, RecCnt, i, filelen;

	//////////////////////////////////////////////////////////////////////////////////
	// 1. �ε������ش��ϴ� ������ ���Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////
	
	m_DataMgr = FindDM(_T("DM_UC_NOTIFY_FILEINFO")) ;
	if(m_DataMgr == NULL){
		AfxMessageBox (_T("FindDM() Error~!"));
		return - 1 ;
	}

	SqlBuf.Format ( _T(" PARENT_KEY = %s"), parentReckey );
	ids = m_DataMgr->RefreshDataManager (SqlBuf);
	

	//////////////////////////////////////////////////////////////////////////////////
	// 2. ����Ʈ ��Ʈ���� Ŭ�����Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////
	m_List.DeleteAllItems();
    
	//////////////////////////////////////////////////////////////////////////////////
	// 3. ���������� ����Ʈ ��Ʈ�ѿ� �߰��Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////
	
	RecCnt = m_DataMgr->GetRowCount () ;

	CString sFileName ;

	// 2017/11/22 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
	// KOLIS-NET OpenAPI
/*//NEW------------------------------------------------------------------------*/
	m_List.InsertColumn (0, _T(""));
/*//END------------------------------------------------------------------------*/

	for ( i = 0; i < RecCnt; i++ ) {
	     
		m_DataMgr->GetCellData (_T("�����̸�"),i,FileName);
		m_DataMgr->GetCellData (_T("���ϻ�����"),i,FileSize);
		
		filelen =FileSize.GetLength ();
		
		if (4 > filelen) {
			sFileName.Format(_T("%s(1KB)"), FileName);
		} else {
			sFileName.Format(_T("%s(%sKB)"), FileName, FileSize);
		}
		m_List.InsertItem (i, sFileName, -1) ;
	}

	if (0 < RecCnt) {
		// ù��° ���� ����
		ListView_SetItemState (m_List.GetSafeHwnd(), 0, LVIS_FOCUSED|LVIS_SELECTED, 0x000F);
		GetDlgItem(IDC_btnFileDownload)->EnableWindow(TRUE);
	} else {
		GetDlgItem(IDC_btnFileDownload)->EnableWindow(FALSE);
	}

	return 0;
}

VOID CLight_UC_NOTIFY_DETAIL::OnbINCLUDEFILE() 
{
	// TODO: Add your control notification handler code here
	INT        ids, RecCnt;

	///////////////////////////////////////////////////////////////////////////////////////////
	// 1. ���������� ������ return�Ѵ�.
	///////////////////////////////////////////////////////////////////////////////////////////
	CESL_DataMgr *pDM = FindDM(_T("DM_UC_NOTIFY_FILEINFO"));
	if(pDM == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}

	RecCnt = pDM->GetRowCount ();
	
	//if ( RecCnt == 0 ) return;

	///////////////////////////////////////////////////////////////////////////////////////////
	// 4. ÷���������� ȭ���� ����.
	///////////////////////////////////////////////////////////////////////////////////////////
	CLight_UC_NOTIFY_03 dlg( this );
	dlg.GetRowIndex (m_idx) ;
	ids = dlg.DoModal();
	if ( ids == IDOK ) {
		MessageBox( _T("���������� �Ǿ����ϴ�."), _T("��������"), MB_OK );
	}
}



INT CLight_UC_NOTIFY_DETAIL::DbClick_Move (INT ndir)
{
    

	CString m_ParentCM;
	CString m_ParentGrid ;
	CString m_CurrentCM ;
 

    m_CurrentCM = _T("CM_UC_NOTIFY_DETIAL");
    m_ParentCM =_T("CM_UC_NOTIFY_MAIN");
    m_ParentGrid = _T("MainGrid") ;



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
				AfxMessageBox(_T("���� ����� ������ �� �����ϴ�."));	// ����
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
  
	CString RecKey ;

    ids = GetDataMgrData (_T("DM_UC_NOTIFY_MAIN"), _T("REC_KEY"), RecKey ,m_idx );
	
    ids = GetFileInfo( RecKey, m_idx );

    CString sConfirm ;
	ids = GetDataMgrData(_T("DM_UC_NOTIFY_MAIN"),_T("Ȯ��"),sConfirm,m_idx) ;

	if(sConfirm == _T("0"))
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_UC_NOTIFY_MAIN")) ;
		if(pDM == NULL) {
			AfxMessageBox (_T("FindDM() Error~!"));
			return - 234 ;
		}
		pDM->StartFrame ();
		pDM->InitDBFieldData () ;
		pDM->AddDBFieldData (_T("confirm"),_T("STRING"),_T("1")) ;
		pDM->MakeUpdateFrame (_T("UC_NOTIFY_TBL"),_T("REC_KEY"),_T("NUMERIC"),RecKey) ;
		pDM->SendFrame ();
		pDM->EndFrame () ;	
	}


	SetReverse(m_ParentCM,m_ParentGrid ,m_idx ) ;
	ids = AllControlDisplay (m_CurrentCM ,m_idx) ;  

    return 0;
}

HBRUSH CLight_UC_NOTIFY_DETAIL::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

// 2017/11/22 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
// KOLIS-NET OpenAPI
void CLight_UC_NOTIFY_DETAIL::OnbtnFileDownload() 
{
	m_List.SetFocus();
	
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nIdx = m_List.GetNextSelectedItem(pos);
	
	if (0 > nIdx) {
		MessageBox( _T("�ٿ�ε��� ������ �����ϼ���."), _T("�ٿ�ε�"), MB_OK );
		return;
	}
	
	CString strURL;
	GetDataMgrData (_T("DM_UC_NOTIFY_FILEINFO"), _T("URL"), strURL, nIdx);
	if (strURL.IsEmpty()) {
		return;
	}
	
	ShellExecute(0, _T("open"), strURL, 0, 0, 1);
}
