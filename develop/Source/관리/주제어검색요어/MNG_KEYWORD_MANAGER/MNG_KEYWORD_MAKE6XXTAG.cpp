// MNG_KEYWORD_MAKE6XXTAG.cpp : implementation file
//

#include "stdafx.h"
//#include _T("mng_keyword_manager.h")
#include "MNG_KEYWORD_MAKE6XXTAG.h"
#include "EnvironComboBox_MultiState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_MAKE6XXTAG dialog


CMNG_KEYWORD_MAKE6XXTAG::CMNG_KEYWORD_MAKE6XXTAG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_KEYWORD_MAKE6XXTAG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	
}

VOID CMNG_KEYWORD_MAKE6XXTAG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_KEYWORD_MAKE6XXTAG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_KEYWORD_MAKE6XXTAG, CDialog)
	//{{AFX_MSG_MAP(CMNG_KEYWORD_MAKE6XXTAG)
	ON_BN_CLICKED(IDC_MAKE_TAG, OnMakeTag)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_DELETE_TAG, OnDeleteTag)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_MAKE6XXTAG message handlers
BOOL CMNG_KEYWORD_MAKE6XXTAG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_����_������˻����_�±׻���")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	// 
	SetArray6XXTag();


	if ( m_pmultiComboBox )
		m_pmultiComboBox = NULL;
	
	m_pmultiComboBox = new CESL_ControlMultiComboBox(NULL);
	m_pmultiComboBox->SubclassDlgItem(IDC_c6XX_CODE, this);

	AllControlDisplay(_T("CM_����_������˻����_�±׻���"));
		
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_KEYWORD_MAKE6XXTAG::OnMakeTag() 
{
EFS_BEGIN
	
	INT ids = -1;
	//EDIT ���ڿ� TAG �����ֱ�
	ids = MakeTagText();
	if ( ids < 0 ) return;
	
EFS_END	
}



BEGIN_EVENTSINK_MAP(CMNG_KEYWORD_MAKE6XXTAG, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_KEYWORD_MAKE6XXTAG)
	ON_EVENT(CMNG_KEYWORD_MAKE6XXTAG, IDC_6XX_GRID, -601 /* DblClick */, OnDblClick6xxGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_KEYWORD_MAKE6XXTAG::OnDblClick6xxGrid() 
{
	// �޺��ڽ� �����ֱ�
	ShowControl();
	
}

INT CMNG_KEYWORD_MAKE6XXTAG::ShowControl()
{
EFS_BEGIN

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_������˻����_�󼼺���_�������Ʈ"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return FALSE;
	}
	
	CESL_Grid *pGrid = static_cast<CESL_Grid*>(GetDlgItem(IDC_6XX_GRID));

	INT nCol = pGrid->GetCol();
	INT nRow = pGrid->GetRow();

	if ( nCol != 3 ) return -1;

	m_pComboBox = new CEnvironComboBox_MultiState;
	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), pGrid, 23004);

	CStringArray ArrCmbData;
	for( INT i=0; i<m_map6XXTags.GetSize(); i++)
	{
		ArrCmbData.Add( m_map6XXTags.GetAt(i) );
	}
	m_pComboBox->Init(&ArrCmbData);
	
	
	m_pComboBox->SetIndex(nRow-1);
	m_pComboBox->SetDataMgr(pDM);
	m_pComboBox->SetParent(pGrid);
	CString strData = _T("650");
	m_pComboBox->SetCurSelData(strData);

	m_pComboBox->SetHorizontalExtent(10);
	CFont * pFont = this->GetFont();
	m_pComboBox->SetFont(pFont);
	
	CRect rect,gridRect;
	RECT parentRect;
	pGrid->GetWindowRect(&gridRect);
	this->GetClientRect(&parentRect);


	::ClientToScreen(this->m_hWnd,(POINT*)&(parentRect.left));
	

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left = (long)((float)pGrid->GetCellLeft()/15.0f) - (long)((float)gridRect.left/9.0f);
	rect.top = (long)((float)pGrid->GetCellTop()/15.0f) - (long)((float)gridRect.top/5.0f);

	
	rect.right = rect.left + (long)((float)pGrid->GetCellWidth()/15.0f);
	rect.bottom = rect.top + (long)((float)pGrid->GetCellHeight()/15.0f);

	
	rect.right = rect.left + (long)((float)pGrid->GetCellWidth()/15.0f);
	rect.bottom = rect.top + (long)((float)pGrid->GetCellHeight()/15.0f);




	m_pComboBox->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	

	m_pComboBox->ShowWindow(TRUE);
	m_pComboBox->SetFocus();

	return 0;
EFS_END
	return -1;

}

VOID CMNG_KEYWORD_MAKE6XXTAG::OnOk() 
{
	//��ũ�� ����
	MakeTagMarc();

	CDialog::OnOK();
}

INT CMNG_KEYWORD_MAKE6XXTAG::MakeTagMarc()
{
EFS_BEGIN	

	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_����_������˻����_�±׻���"), _T("6XX_GRID"));
	pGrid->SelectMakeList();
	INT nIdx = pGrid->SelectGetHeadPosition();
	if ( nIdx < 0 )
	{
		AfxMessageBox(_T("������ �����Ͱ� �����ϴ�."));
		return -1;
	}
	
	//����
	INT nSelectCnt = pGrid->SelectGetCount();
	CString sTag=_T(""), sKeyword=_T("");
	for( INT i=0; i<nSelectCnt; i++)
	{
		sTag = pGrid->GetAt( nIdx, 2);
		sKeyword = pGrid->GetAt( nIdx, 1);
		
		if ( _tcsclen(sTag) == 0 ) {
			AfxMessageBox(_T("TAG�� �����ϼ���."));
			return -1;
		}

		m_pMarcMgr->SetItem( m_pMarc, sTag + _T("$a"), sKeyword);

		nIdx =  SelectGetNext(_T("CM_����_������˻����_�±׻���"), _T("6XX_GRID"));
	}

	return 0;
EFS_END
	return -1;
}

VOID CMNG_KEYWORD_MAKE6XXTAG::SetArray6XXTag()
{
EFS_BEGIN

	m_map6XXTags.RemoveAll();

	m_map6XXTags.Add(_T("600"));
	m_map6XXTags.Add(_T("610"));
	m_map6XXTags.Add(_T("611"));
	m_map6XXTags.Add(_T("630"));
	m_map6XXTags.Add(_T("650"));
	m_map6XXTags.Add(_T("651"));

EFS_END
}

/*INT CMNG_KEYWORD_MAKE6XXTAG::Add6XXTagToMarc(CString sTag, CString sKeyword)
{
EFS_BEGIN

	
	CString sTmp = sTag + _T("$a");

	//m_pMarcMgr->SetItem(pMarc, sTmp, sKeyword);

	return 0;
EFS_END
	return -1;
}*/

// ����Ʈ �ڽ��� �±� ����
INT CMNG_KEYWORD_MAKE6XXTAG::MakeTagText()
{
EFS_BEGIN

	//DM �ʱ�ȭ
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_������˻����_�󼼺���_�������Ʈ"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return -1;
	}
	
	SelectMakeList(_T("CM_����_������˻����_�±׻���"), _T("6XX_GRID"));
	INT nIdx = SelectGetHeadPosition(_T("CM_����_������˻����_�±׻���"), _T("6XX_GRID"));
	if ( nIdx < 0 )
	{
		AfxMessageBox(_T("������ �����Ͱ� �����ϴ�."));
		return -1;
	}
	
	// tmp Marc
	CMarc *pTmpMarc = NULL;
	pTmpMarc = new CMarc();

	while (TRUE) 
	{
		CString sSubJectTag = _T("");
		CString sSubJectKeyWord = _T("");

		pDM->GetCellData( _T("������"), nIdx, sSubJectTag );
		pDM->GetCellData( _T("������"), nIdx, sSubJectKeyWord );

		if ( _tcsclen(sSubJectTag) == 0 ) 
		{
			AfxMessageBox(_T("TAG�� �����ϼ���."));
			return -1;
		}

		m_pMarcMgr->SetItem( pTmpMarc, sSubJectTag + _T("$a"), sSubJectKeyWord);

		nIdx = SelectGetNext(_T("CM_����_������˻����_�±׻���"), _T("6XX_GRID"));

		if ( nIdx < 0 ) break;
	}

	// ���
	PrintTagText( pTmpMarc );

	return 0;
EFS_END
	return -1;
}

// ����Ʈ �ڽ� �ʱ�ȭ
VOID CMNG_KEYWORD_MAKE6XXTAG::OnDeleteTag() 
{
EFS_BEGIN

	GetDlgItem(IDC_e6XXTag)->SetWindowText(_T(""));

EFS_END	
}

VOID CMNG_KEYWORD_MAKE6XXTAG::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

// ����Ʈ �ڽ��� �±� ���
VOID CMNG_KEYWORD_MAKE6XXTAG::PrintTagText( CMarc *pTmpMarc )
{
EFS_BEGIN
	
	//6XX 
	CString sTmpField;
	CArray<CString, CString&> pArray6XX;
	CString strData;
	CString sTag = _T("") ;
	
	CString sEditBoxShowMarc=_T("");
	for( INT j=0; j<m_map6XXTags.GetSize(); j++)
	{
		sTag = m_map6XXTags.GetAt(j);

		pArray6XX.RemoveAll();
		m_pMarcMgr->GetField(pTmpMarc,  sTag, sTmpField, &pArray6XX);
		INT nCount = pArray6XX.GetSize();

		for (INT i = 0; i < nCount; i++)
		{
			strData = pArray6XX.GetAt(i);
			
			sEditBoxShowMarc += sTag + strData;
			sEditBoxShowMarc += _T("\r\n");

		}

	}// end of for(Map)
	
	GetDlgItem(IDC_e6XXTag)->SetWindowText(sEditBoxShowMarc);

EFS_END
}

// �Ҹ���
CMNG_KEYWORD_MAKE6XXTAG::~CMNG_KEYWORD_MAKE6XXTAG()
{
	if ( m_pComboBox ) { delete m_pComboBox; m_pComboBox=NULL;}
	if ( m_pmultiComboBox ) { delete m_pmultiComboBox; m_pmultiComboBox=NULL; }

	if ( m_pMarcMgr ) { m_pMarcMgr = NULL; }
	if ( m_pMarc ) { m_pMarc = NULL; }

	m_map6XXTags.RemoveAll();
	
}

HBRUSH CMNG_KEYWORD_MAKE6XXTAG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
