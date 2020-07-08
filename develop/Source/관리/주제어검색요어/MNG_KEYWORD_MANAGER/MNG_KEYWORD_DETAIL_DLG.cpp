// MNG_KEYWORD_DETAIL_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_KEYWORD_DETAIL_DLG.h"
#include "MNG_KEYWORD_DUP_DLG.h"
#include "MNG_KEYWORD_MAKE6XXTAG.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_DETAIL_DLG dialog


CMNG_KEYWORD_DETAIL_DLG::CMNG_KEYWORD_DETAIL_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_KEYWORD_DETAIL_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_CMName_Main = _T("CM_����_������˻����_����");
	m_DMName_Main = _T("DM_����_������˻����_������");

	m_MainType = _T("MAIN");	// MAIN / POPUP

}

CMNG_KEYWORD_DETAIL_DLG::~CMNG_KEYWORD_DETAIL_DLG()
{
}

VOID CMNG_KEYWORD_DETAIL_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_KEYWORD_DETAIL_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_KEYWORD_DETAIL_DLG, CDialog)
	//{{AFX_MSG_MAP(CMNG_KEYWORD_DETAIL_DLG)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bBfrWord, OnbBfrWord)
	ON_BN_CLICKED(IDC_bMakeTag, OnbMakeTag)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_KEYWORD_DETAIL_DLG message handlers

BOOL CMNG_KEYWORD_DETAIL_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_������˻����_�󼼺���")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
//////////////////////////////////////////////////////////////////////////////////////////////
// ����  ++
//////////////////////////////////////////////////////////////////////////////////////////////
	SelectMakeList(m_CMName_Main, _T("MAIN_GRID"));
	INT n_index = SelectGetHeadPosition(m_CMName_Main, _T("MAIN_GRID"));
	if(n_index < 0)
	{
		//����ó��
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		EndDialog(0);
		return FALSE;
	}	

	CESL_DataMgr *pMainDM;
	pMainDM = FindDM(m_DMName_Main);
	if ( NULL == pMainDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return FALSE;
	}

//////////////////////////////////////////////////////////////////////////////////////////////
// ����  --
//////////////////////////////////////////////////////////////////////////////////////////////



	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_������˻����_�󼼺���"));
	if ( NULL == pCM ) 
	{
		AfxMessageBox(_T("CM �ʱ�ȭ ����"));
		return FALSE;
	}
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_������˻����_�󼼺���_���þ�"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return FALSE;
	}

	// 6XX Tag ������ư ó��
	if ( m_MainType == _T("MAIN") ) 
	{
		GetDlgItem(IDC_bMakeTag)->ShowWindow(FALSE);
	}
	else if ( m_MainType == _T("POPUP") ) 
	{
		GetDlgItem(IDC_bMakeTag)->ShowWindow(TRUE);
	}

	CString sRecKey, sKeyword;
	sRecKey = pMainDM->GetCellData(_T("REC_KEY"), n_index);
	sKeyword = pMainDM->GetCellData(_T("������"), n_index);

	pCM->SetControlMgrData(_T("������"), sKeyword);


	m_pmng = new CMNG_KEYWORD();
	m_pmng->SearchRelatedKeyword(pDM, sRecKey);

	SetCurrKeyword(sRecKey, sKeyword);

	AllControlDisplay(_T("CM_����_������˻����_�󼼺���"));
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/// ���� ///////////////////////////////////
VOID CMNG_KEYWORD_DETAIL_DLG::OnbOK() 
{
EFS_BEGIN	

	ShowSearchKeyword();
	
EFS_END	
}

INT CMNG_KEYWORD_DETAIL_DLG::SetCheckGrid()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gDetail );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();
	
	if( 1 != col ) return 0;
	
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );
		
		if( row == i )
		{
			pGrid->SetText( _T("V") );
		}
		else if( _T("V") == pGrid->GetText() )
			pGrid->SetText( _T("") );
	}
	
	pGrid->SetRow( row );	
	return row;
}

BEGIN_EVENTSINK_MAP(CMNG_KEYWORD_DETAIL_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_KEYWORD_DETAIL_DLG)
	ON_EVENT(CMNG_KEYWORD_DETAIL_DLG, IDC_gDetail, -600 /* Click */, OnClickgDetail, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_KEYWORD_DETAIL_DLG::OnClickgDetail() 
{
	INT iClickGridRow = SetCheckGrid();	
}

/// ������ ����Ʈ�� ������ �߰�  ///////////////////////////////////
INT CMNG_KEYWORD_DETAIL_DLG::AddKeywordList(CESL_DataMgr *pDM, CString sRecKey, CString sKeyword)
{
EFS_BEGIN

	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return -1;
	}

	if ( sRecKey == _T("") || sKeyword == _T("") ) return -1;

	INT n_index;
	n_index = pDM->GetRowCount();

	INT ids = -1;
	ids = pDM->InsertRow(-1);
	ids = pDM->SetCellData(_T("REC_KEY"), sRecKey, n_index);
	ids = pDM->SetCellData(_T("������"), sKeyword, n_index);

	return 0;
EFS_END
	return -1;
}

/// ������ ����Ʈ���� ������ ������ ���� ///////////////////////////////////
INT CMNG_KEYWORD_DETAIL_DLG::DeleteKeywordList(CESL_DataMgr *pDM, INT n_idx)
{
EFS_BEGIN
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return -1;
	}
	
	INT n_index;
	n_index = pDM->GetRowCount();

	if ( n_idx >= n_index ) return -1;
	
	pDM->DeleteRow(n_idx);

	return 0;
EFS_END
	return -1;
}

/// ������ ����Ʈ ȭ�鿡 ��� ���� parsing ///////////////////////////////////
INT CMNG_KEYWORD_DETAIL_DLG::PrintKeywordList( CESL_DataMgr *pDM )
{
EFS_BEGIN
	
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return -1;
	}
	
	INT n_cnt;
	n_cnt = pDM->GetRowCount();

	CString sWordList=_T(""), sTemp=_T("");
	for( INT i=0; i<n_cnt; i++)
	{
		sTemp = pDM->GetCellData(_T("������"), i);
		if ( _T("") == sWordList ) sWordList = sTemp;
		else sWordList += _T("=>") + sTemp;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_������˻����_�󼼺���"));
	if ( NULL == pCM ) 
	{
		AfxMessageBox(_T("CM �ʱ�ȭ ����"));
		return -1 ;
	}

	pCM->SetControlMgrData(_T("�������Ʈ"), sWordList);

	return 0;
EFS_END
	return -1;
}

/// �����ܾ� 
VOID CMNG_KEYWORD_DETAIL_DLG::OnbBfrWord() 
{
EFS_BEGIN

	ShowBeforeKeyword();


EFS_END
}


/// ������ Keyword �� Setting
VOID CMNG_KEYWORD_DETAIL_DLG::SetCurrKeyword(CString sReckey, CString sKeyword)
{
EFS_BEGIN
	
	sReckey.TrimLeft();
	sReckey.TrimRight();
	if( sReckey == _T("") ) return;
	
	sKeyword.TrimLeft();
	sKeyword.TrimRight();
	if( sKeyword == _T("") ) return;

	m_CurrRecKey = sReckey;
	m_CurrKeyword = sKeyword;

EFS_END
}

BOOL CMNG_KEYWORD_DETAIL_DLG::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CMNG_KEYWORD_DETAIL_DLG::SetCMName_Main(CString sName)
{
	m_CMName_Main =  sName;
}

VOID CMNG_KEYWORD_DETAIL_DLG::SetDMName_Main(CString sName)
{
	m_DMName_Main =  sName;
}

// 6XX Tag ����
VOID CMNG_KEYWORD_DETAIL_DLG::OnbMakeTag() 
{
EFS_BEGIN
	Make6XXTag();

	

EFS_END	
}

VOID CMNG_KEYWORD_DETAIL_DLG::OnOk() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
	
}

VOID CMNG_KEYWORD_DETAIL_DLG::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CMNG_KEYWORD_DETAIL_DLG::ShowSearchKeyword()
{
EFS_BEGIN
	SelectMakeList(_T("CM_����_������˻����_�󼼺���"), _T("GRID"));
	INT n_index = SelectGetHeadPosition(_T("CM_����_������˻����_�󼼺���"), _T("GRID"));
	if(n_index < 0)
	{
		//����ó��
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return FALSE;
	}	

	CESL_DataMgr *pDM_Keyword;
	pDM_Keyword = FindDM(_T("DM_����_������˻����_�󼼺���_������"));
	if ( NULL == pDM_Keyword ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return FALSE;
	}

	CESL_DataMgr *pDM_Rword;
	pDM_Rword = FindDM(_T("DM_����_������˻����_�󼼺���_���þ�"));
	if ( NULL == pDM_Rword ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return FALSE;
	}

	CString sRword=_T(""), sRwordCode=_T(""), sKeyword_New=_T(""), sRecKey=_T("");
	//1. �׸��忡�� ���õ� ���þ� �����´�.
	sRword = pDM_Rword->GetCellData(_T("���þ�"), n_index);
	sRwordCode = pDM_Rword->GetCellData(_T("���þ��ڵ�"), n_index);

	// BT, RT, NT�� ��˻�
	if ( sRwordCode!=_T("BT") && sRwordCode!=_T("RT") && sRwordCode!=_T("NT") ) 
	{
		AfxMessageBox(_T("������ ���а����δ� �˻��� �� �����ϴ�. \r\nBT, RT, NT�� �˻� �����մϴ�."));
		return FALSE;
	}

	// ���þ��� '(' �� ' ' �������� �˻�
	CString stmpWord=sRword;
	INT n_tmpidx = stmpWord.Find('(');
	if ( n_tmpidx > 0 ) stmpWord = stmpWord.Left(n_tmpidx);
	else stmpWord = sRword;
	n_tmpidx = stmpWord.Find(' ');
	if ( n_tmpidx > 0 ) stmpWord = stmpWord.Left(n_tmpidx);
	sRword = stmpWord;
	

	//2. ���þ ���� ������� ����. ������ ���̺��� �˻��Ѵ�.
	m_pmng->SearchKeyword(pDM_Keyword, sRword);

	INT n_RowCount;
	n_RowCount = pDM_Keyword->GetRowCount();
	if ( n_RowCount <= 0 ) 
	{
		AfxMessageBox(_T("���ǿ� �´� �ڷᰡ �������� �ʽ��ϴ�."));
		return FALSE;
	}

	//2-1. �˻��� ����� �������̸� �ߺ� DLG���� �ϳ� �����Ѵ�.
	INT n_idxKeyword = -1;
	if( n_RowCount > 1 )
	{
		CMNG_KEYWORD_DUP_DLG *pDupDlg = new CMNG_KEYWORD_DUP_DLG(this);
		if ( IDOK == pDupDlg->DoModal() )
		{
			n_idxKeyword = pDupDlg->GetSelectIDX();
		}
		else return FALSE;
	}
	else {
		n_idxKeyword = 0;
	}

	//3. ���õ� ������� ���þ� ���̺��� �˻��Ͽ� �����ش�.
	sRecKey = pDM_Keyword->GetCellData(_T("REC_KEY"), n_idxKeyword);
	sKeyword_New = pDM_Keyword->GetCellData(_T("������"), n_idxKeyword);
	m_pmng->SearchRelatedKeyword(pDM_Rword, sRecKey);

	//4. Display
	SetControlData(_T("CM_����_������˻����_�󼼺���"), _T("������"), sKeyword_New);

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_������˻����_�󼼺���_�������Ʈ"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return FALSE;
	}

	// ������ ����Ʈ�� �߰�
	INT ids = -1;
	ids = AddKeywordList( pDM, m_CurrRecKey, m_CurrKeyword);
	if ( ids < 0 ) return FALSE;

	ids = PrintKeywordList( pDM );
	if ( ids < 0 ) return FALSE;

	SetCurrKeyword(sRecKey, sKeyword_New);

	
	AllControlDisplay(_T("CM_����_������˻����_�󼼺���"));

	return TRUE;
EFS_END
	return FALSE;
}

BOOL CMNG_KEYWORD_DETAIL_DLG::ShowBeforeKeyword()
{
EFS_BEGIN
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_������˻����_�󼼺���_�������Ʈ"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return FALSE;
	}

	CESL_DataMgr *pDM_Rword;
	pDM_Rword = FindDM(_T("DM_����_������˻����_�󼼺���_���þ�"));
	if ( NULL == pDM_Rword ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return FALSE;
	}

	INT n_index;
	n_index = pDM->GetRowCount();
	if (  n_index<=0 ) return FALSE;

	// 1. ������ ����Ʈ���� ������ ������ ������
	CString sRecKey=_T(""), sKeyword=_T("");
	sRecKey = pDM->GetCellData(_T("REC_KEY"), n_index-1);
	sKeyword = pDM->GetCellData(_T("������"), n_index-1);


	// 2. ������� ���þ� �˻�
	m_pmng->SearchRelatedKeyword(pDM_Rword, sRecKey);

	// 3. ������ ����Ʈ���� �����ϰ� ����Ʈ ���
	SetCurrKeyword(sRecKey, sKeyword);
	DeleteKeywordList(pDM, n_index-1);
	PrintKeywordList( pDM );

	SetControlData(_T("CM_����_������˻����_�󼼺���"), _T("������"), sKeyword);
	
	AllControlDisplay(_T("CM_����_������˻����_�󼼺���"));

	return TRUE;
EFS_END
	return FALSE;
}

BOOL CMNG_KEYWORD_DETAIL_DLG::Make6XXTag()
{
EFS_BEGIN

	CMNG_KEYWORD_MAKE6XXTAG *pDlg;
	pDlg = new CMNG_KEYWORD_MAKE6XXTAG(this);

	pDlg->SetMarcMgr( m_pMarcMgr, m_pMarc );

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_������˻����_�󼼺���_�������Ʈ"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM �ʱ�ȭ ����"));
		return FALSE;
	}

	// ���� Ű���嵵 Tag �������� �߰�
	AddKeywordList( pDM,  m_CurrRecKey, m_CurrKeyword);

	if ( IDOK == pDlg->DoModal() ) 	CDialog::OnOK();
	
	INT nCnt = pDM->GetRowCount();
	
	// ����Ű���� �ٽ� ����
	DeleteKeywordList(pDM, nCnt-1 );
	
	return TRUE;
EFS_END
	return FALSE;
}

HBRUSH CMNG_KEYWORD_DETAIL_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
