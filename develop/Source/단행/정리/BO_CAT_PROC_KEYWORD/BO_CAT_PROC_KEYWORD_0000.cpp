// BO_CAT_PROC_KEYWORD_0000.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_PROC_KEYWORD_0000.h"
//#include _T("BO_CAT_PROC_KEYWORD_UNLESS.h")
//#include _T("BO_CAT_AUXILIARY_WORD_0100.h")
//#include _T("BO_CAT_WORD_0100.h")
//#include _T("..\..\..\����\IndexToolKit\IndexTool.h")

// ��������
#include "..\..\..\����\���ں�ȯ����\MngCharSetManagement\CatCharSetManagementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_KEYWORD_0000 dialog


CBO_CAT_PROC_KEYWORD_0000::CBO_CAT_PROC_KEYWORD_0000(CESL_Mgr* pParent, CMarc &m_marc /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_KEYWORD_0000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();

	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);

	pDM_KeyWord = NULL;
	m_MakeSearchData = new CMakeSearchData(this);
}
/*
CBO_CAT_PROC_KEYWORD_0000::~CBO_CAT_PROC_KEYWORD_0000()
{
	if(m_MakeSearchData)
	{
		delete m_MakeSearchData;
		m_MakeSearchData = NULL;
	}
}
*/


CBO_CAT_PROC_KEYWORD_0000::~CBO_CAT_PROC_KEYWORD_0000()
{
	if(m_MakeSearchData)
	{
		delete m_MakeSearchData;
		m_MakeSearchData = NULL;
	}
}

BOOL CBO_CAT_PROC_KEYWORD_0000::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_KEYWORD_0000::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_KEYWORD_0000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_KEYWORD_0000, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_KEYWORD_0000)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bUNLESS_WORD_INFO, OnbUNLESSWORDINFO)
	ON_BN_CLICKED(IDC_bAUX_WORD, OnbAUXWORD)
	ON_BN_CLICKED(IDC_bWORD, OnbWORD)
	ON_BN_CLICKED(IDC_bKEYWORD, OnbKEYWORD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_KEYWORD_0000 message handlers

BOOL CBO_CAT_PROC_KEYWORD_0000::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0390")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	//�ܾ�
	pDM_KeyWord = FindDM( _T("DM_BO_CAT_PROC_0390_WORD") );
	if ( pDM_KeyWord == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0390_WORD") );
		return FALSE;
	}
	CString sPath = _T("..\\CFG\\CO_WORD_TBL.TXT");
	//�ܾ� �����о����
	FILE *fp;
	TCHAR cLine[256];
	fp = _tfopen( sPath , _T("rb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CO_WORD_TBL.txt ������ �����ϴ�.") );
		return FALSE;
	}
	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
	
	INT iRowCount = 0;
	while( _fgetts( cLine, 256, fp) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLine );
		if ( sLine.Left(1) == _T(".") ) {
			continue;
		}
		INT iPos = sLine.Find(_T("|"));
		//CString sResult = sLine.Mid( iPos+1, 2 );		
		CString sResult = sLine.Left( iPos );
		sResult.TrimRight();
		pDM_KeyWord->InsertRow(-1);
		pDM_KeyWord->SetCellData( _T("�ܾ�"), sResult, iRowCount );
		iRowCount++;
	}	
	fclose( fp );

	//����
	CESL_DataMgr *pDmAuxWord;
	pDmAuxWord = FindDM( _T("DM_BO_CAT_PROC_0390_AuxWord") );
	if ( pDmAuxWord == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0390_AuxWord") );
		return FALSE;
	}
	
	sPath = _T("..\\CFG\\CO_AUXILIARY_WORD_TBL.TXT");
	//���� �����о����
	fp = _tfopen( sPath , _T("rb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CO_AUXILIARY_WORD_TBL.txt ������ �����ϴ�.") );
		return FALSE;
	}
	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
	
	iRowCount = 0;
	while( _fgetts( cLine, 256, fp) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLine );
		if ( sLine.Left(1) == _T(".") ) {
			continue;
		}
		INT iPos = sLine.Find(_T("|"));
		CString sResult = sLine.Left( iPos );
		sResult.TrimRight();
		pDmAuxWord->InsertRow(-1);
		pDmAuxWord->SetCellData( _T("����"), sResult, iRowCount );
		iRowCount++;
	}	
	fclose( fp );


	//�ҿ��
	CESL_DataMgr *pUnlessWord;
	pUnlessWord = FindDM( _T("DM_BO_CAT_PROC_0390_UnlessWord") );
	if ( pUnlessWord == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0390_UnlessWord") );
		return FALSE;
	}

	sPath = _T("..\\CFG\\CO_UNLESS_WORD_TBL.txt");
	//�����о����
	fp = _tfopen( sPath , _T("rb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CO_UNLESS_WORD_TBL.txt ������ �����ϴ�.") );
		return FALSE;
	}
	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
	
	iRowCount = 0;
	while( _fgetts( cLine, 256, fp) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLine );
		if ( sLine.Left(1) == _T(".") ) {
			continue;
		}
		INT iPos = sLine.Find(_T("|"));
		CString sResult = sLine.Left( iPos );
		sResult.TrimRight();
		pUnlessWord->InsertRow(-1);
		pUnlessWord->SetCellData( _T("�ҿ��"), sResult, iRowCount );
		iRowCount++;
	}
	
	fclose( fp );

	Init(m_strStreamMarc);
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

INT CBO_CAT_PROC_KEYWORD_0000::Init(CString strStreamMarc)
{
	INT ids = -1;
	m_strStreamMarc = strStreamMarc;

	//decoding
	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	//653 tag �б�
	CArray<CString, CString&> pArray653;	
	CString strItemData;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("653$a"), strItemData, &pArray653);

	//ó������
	if ( strItemData == _T("") ) {
		m_sStart = _T("Y");
		GetDlgItem(IDC_bKEYWORD)->EnableWindow(false);
	}
	
	INT iYes = 0;
	for ( INT i = 0 ; i < pArray653.GetSize() ; i++ ) {
		m_strKeyword = m_strKeyword + pArray653.GetAt( i ) + _T(" ");
		if ( m_sStart == _T("Y") ) {
			pArray.Add( pArray653.GetAt( i ) );		
		}
	}

	//����ȭ�� ��ũ���� �� �а� m_marc_key ����ü�� �ֱ�
	ids = KeywordMarcInsert( _T("245") );
	ids = KeywordMarcInsert( _T("440") );
	ids = KeywordMarcInsert( _T("490") );
	ids = KeywordMarcInsert( _T("130") );
	ids = KeywordMarcInsert( _T("240") );
	ids = KeywordMarcInsert( _T("730") );
	ids = KeywordMarcInsert( _T("740") );
	ids = KeywordMarcInsert( _T("830") );
	ids = KeywordMarcInsert( _T("930") );
	ids = KeywordMarcInsert( _T("940") );
	ids = KeywordMarcInsert( _T("949") );
	ids = KeywordMarcInsert( _T("507") );
	ids = KeywordMarcInsert( _T("500") );

	//Ű���� �ֱ�
	KeywordInsert( _T("245$a") );
	KeywordInsert( _T("245$b") );
	KeywordInsert( _T("245$c") );
	KeywordInsert( _T("245$p") );
	KeywordInsert( _T("245$x") );
	
	//440 insert
	KeywordInsert( _T("440$a") );
	KeywordInsert( _T("440$p") );
	KeywordInsert( _T("440$x") );


	KeywordInsert( _T("490$a") );
	KeywordInsert( _T("490$p") );
	KeywordInsert( _T("490$x") );

	KeywordInsert( _T("130$a") );
	KeywordInsert( _T("130$p") );

	KeywordInsert( _T("240$a") );
	KeywordInsert( _T("240$p") );

	KeywordInsert( _T("730$a") );
	KeywordInsert( _T("730$p") );

	KeywordInsert( _T("740$a") );
	KeywordInsert( _T("740$p") );

	KeywordInsert( _T("830$a") );
	KeywordInsert( _T("830$p") );

	KeywordInsert( _T("930$a") );
	KeywordInsert( _T("930$p") );

	KeywordInsert( _T("940$a") );
	KeywordInsert( _T("940$p") );

	KeywordInsert( _T("949$a") );
	KeywordInsert( _T("949$p") );

	KeywordInsert( _T("507$t") );

	KeywordInsert( _T("500$b") );
	KeywordInsert( _T("500$e") );
	KeywordInsert( _T("500$f") );
	KeywordInsert( _T("500$g") );
	KeywordInsert( _T("500$h") );
	KeywordInsert( _T("500$i") );
	KeywordInsert( _T("500$j") );
	KeywordInsert( _T("500$k") );
	KeywordInsert( _T("500$r") );

	SetControlData(_T("CM_BO_CAT_PROC_0390"), _T("Ű����"), m_strKeyword);

	m_marcEditor.SubclassDlgItem(IDC_reditKey, -1, this);

	//encoding
	strStreamMarc = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc_key, strStreamMarc);

	//insert marc
	m_marcEditor.InitUserAlias();
	m_marcEditor.AddUserAlias(_T("��ǥ��"));
	m_marcEditor.AddUserAlias(_T("������"));
	m_marcEditor.AddUserAlias(_T("������"));
	m_marcEditor.AddUserAlias(_T("������"));
	m_marcEditor.AddUserAlias(_T("�����"));
	m_marcEditor.AddUserAlias(_T("�Է³�¥"));
	m_marcEditor.AddUserAlias(_T("�����1"));
	m_marcEditor.AddUserAlias(_T("����"));

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc_key);
	m_marcEditor.Display();

	CButton *pBtn = ( CButton * )GetDlgItem( IDOK );
	pBtn->SetFocus();

	return 0;
}

INT CBO_CAT_PROC_KEYWORD_0000::KeywordMarcInsert( CString sTag )
{
EFS_BEGIN

	CString sValue;
	CArray<CString, CString&> arrItem;
	m_pInfo->pMarcMgr->GetField( &m_marc, sTag , sValue, &arrItem);

	INT nCount = arrItem.GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		sValue = arrItem.GetAt(idx);
		if ( sValue != _T("") ) {
			m_pInfo->pMarcMgr->InsertField ( &m_marc_key, sTag + sValue );
		}		
	}
	

	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_PROC_KEYWORD_0000::KeywordInsert(CString sTag)
{
EFS_BEGIN
	CString strItemData;
	CArray<CString, CString&> arrItem;
	INT ids = m_pInfo->pMarcMgr->GetItem ( &m_marc , sTag, strItemData, &arrItem);
	if (ids < 0) 
	{
		///AfxMessageBox ( _T("m_pInfo->pMarcMgr->GetItem Error") );
		return ;
	}
	
	LPTSTR lpszTok = NULL;
	
	INT nRowCount = arrItem.GetSize();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		//245tag�� ��� ���ñ�ȣ�� �����Ͽ� Ű���� ����
		//��1���ñ�ȣ�� 1�ϰ�� ��Ī�� �����ϰ� Ű���� ����
		//��1���ñ�ȣ�� 2�ϰ�� ��Ī�� �����ؼ� Ű���� ����	

		strItemData = arrItem.GetAt(idx);
		if (strItemData.IsEmpty()) continue;

		if ( sTag == _T("245$a") ) 
		{
			CString s245;
			m_pInfo->pMarcMgr->GetField( &m_marc, _T("245"), s245 );
			if ( s245.Left(1) == _T("1") ) {
				//��Ī����
				INT iStart = strItemData.Find( _T("(") );
				if ( iStart != -1 ) 
				{
					INT iEnd = strItemData.Find( _T(")") );
					if ( iStart < iEnd ) {
						CString sTmp;
						sTmp = strItemData.Mid( iStart, iEnd-iStart+1 );
						strItemData.Replace( sTmp, _T("") );
					}
				}			
			}
			else if ( s245.Left(1) == _T("2") ) 
			{
				//��Ī����
				strItemData.Replace( _T("(") , _T(" ") );
				strItemData.Replace( _T(")") , _T(" ") );
				
				INT iStart = strItemData.Find( _T(" ") );
				if ( iStart == 0 ) {
					strItemData = strItemData.Mid(1);
					iStart = strItemData.Find( _T(" ") );
				}
				while ( iStart != -1 ) {
					CString sToken;
					sToken = strItemData.Mid( 0, iStart );
					//Ű���� ����
					ids = KeywordMake( sToken );
					
					strItemData = strItemData.Mid( iStart + 1 );
					iStart = strItemData.Find( _T(" ") );
				}		
			}
		}
		
		strItemData.Replace( _T("(") , _T("") );
		strItemData.Replace( _T(")") , _T("") );
		
		strItemData.TrimLeft();
		strItemData.TrimRight();
		
		lpszTok = _tcstok(strItemData.GetBuffer(0), _T(" "));
		while (lpszTok)
		{
			ids = KeywordMake(lpszTok);

			lpszTok = _tcstok(NULL, _T(" "));
		}
		/*
		
		INT iStart = strItemData.Find( _T(" ") );
		if ( iStart == 0 ) {
			strItemData = strItemData.Mid(1);
			iStart = strItemData.Find( _T(" ") );
		}
		while ( iStart != -1 ) 
		{
			CString sToken;
			sToken = strItemData.Mid( 0, iStart );
			//Ű���� ����
			ids = KeywordMake( sToken );
			
			strItemData = strItemData.Mid( iStart + 1 );
			iStart = strItemData.Find( _T(" ") );
		}		
		
		ids = KeywordMake( strItemData );
		*/
	}

EFS_END
}

INT CBO_CAT_PROC_KEYWORD_0000::KeywordMake( CString strItemData )
{
EFS_BEGIN
	if ( !strItemData.IsEmpty() ) {
		INT iCh = m_MakeSearchData->ChangeHanjaToHangul(strItemData);	//���� �ѱ� ��ȯ
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strItemData ) error") );
			return -1;
		}
		iCh = m_MakeSearchData->GetKeyWord(strItemData);	//����, Ư������ ����
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strItemData ) error") );
			return -1;
		}

		if ( strItemData != _T("") ) {
			//�ܾ ���Ե� ��� Ű���� ����
			INT iWord = SearchWord( strItemData );
			if ( iWord < 0 ) {
				AfxMessageBox( _T("SearchWord( strItemData ) error") );
			}
			
			//���翡 ���Ե� ��� ���縦 �����ϰ� Ű���� ����
			CString sResult=_T("");
			INT iAuxiliaryWord = SearchAuxiliaryWord( strItemData, sResult );
			
			// �ҿ� ���Ե� ��� Ű���带 �������� �ʴ´�.
			INT iUnlessWord = SearchUnlessWord( strItemData );

			INT ids;
			if ( iWord == 1 ) {
				ids = ArrayInsert( strItemData );
			}
			if ( iUnlessWord == 1 && iWord == 1 ) {
				CString sMessage;
				sMessage.Format( _T("'%s'�� �ܾ�� �ҿ� ��� ���ԵǾ� �ֽ��ϴ�.") , strItemData );
				AfxMessageBox( sMessage );
			}
			if ( iWord != 1 && iAuxiliaryWord == 1 ) {
				ids = ArrayInsert ( sResult );
			}
			if ( iWord == 0 && iAuxiliaryWord == 0 && iUnlessWord == 0 ) {
				ids = ArrayInsert ( strItemData );
			}
		}
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_KEYWORD_0000::SearchWord(CString sWord)
{
EFS_BEGIN
	
	INT iRowCount = pDM_KeyWord->GetRowCount();
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sWordDb = pDM_KeyWord->GetCellData( _T("�ܾ�") , i );
		if ( sWordDb == sWord ) {
			return 1;
		}
	}
	
	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_KEYWORD_0000::SearchAuxiliaryWord( CString sWord, CString &sResult )
{
EFS_BEGIN

	CESL_DataMgr *pDmAuxWord;
	pDmAuxWord = FindDM( _T("DM_BO_CAT_PROC_0390_AuxWord") );
	if ( pDmAuxWord == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0390_AuxWord") );
		return FALSE;
	}

	INT iRowCount = pDmAuxWord->GetRowCount();
	CString sAuxWord = _T("");
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sWordDb = pDmAuxWord->GetCellData( i, 0 );
		if ( sWordDb.IsEmpty() ) {
			continue;
		}
		INT iWordDb = sWordDb.GetLength();
		if ( sWord.GetLength() <= sWordDb.GetLength() ) {
			continue;
		}
		CString sTmp = sWord.Right( iWordDb );		

		if ( sWordDb == sTmp ) {
			//===================================================
			//2010.05.20 UPDATE BY PJW : ���縦 ���� �ܾ� �񱳽� ���� �������� ���̿��� ������ ���̸� ���ϰ� ���Ѵ�.
//			if ( sAuxWord.IsEmpty() || sAuxWord.GetLength() > sWord.Left(iWordDb).GetLength() ) 
			if ( sAuxWord.IsEmpty() || sAuxWord.GetLength() > sWord.Left(sWord.GetLength() - iWordDb).GetLength() ) 
			//===================================================			
			{
				INT iPosition;
				iPosition = sWord.GetLength() - iWordDb;
				sAuxWord = sWord.Left( iPosition );
			}
		}
	}
	if ( !sAuxWord.IsEmpty() ) {
		sResult = sAuxWord;
		return 1;
	}
	return 0;

EFS_END
return -1;

}

//INT CESL_DataMgr::GetCellData(INT rowidx, INT idx, CString &result)
INT CBO_CAT_PROC_KEYWORD_0000::SearchUnlessWord( CString sWord )
{
EFS_BEGIN

	CESL_DataMgr *pUnlessWord;
	pUnlessWord = FindDM( _T("DM_BO_CAT_PROC_0390_UnlessWord") );
	if ( pUnlessWord == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0390_UnlessWord") );
		return FALSE;
	}

	INT iRowCount = pUnlessWord->GetRowCount();
	CString sAuxWord = _T("");
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sWordDb = pUnlessWord->GetCellData( _T("�ҿ��") , i );
		if ( sWordDb == sWord ) {
			return 1;
		}
	}
	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_KEYWORD_0000::ArrayInsert( CString strItemData )
{
EFS_BEGIN

	INT iYes = 0;
	for ( INT i = 0 ; i < pArray.GetSize() ; i++ ) {
		if ( strItemData == pArray.GetAt( i ) ) {
			iYes = 1;
			break;
		}
	}
	if ( iYes == 0 ) {
		if ( m_sStart == _T("Y") ) {
			m_strKeyword = m_strKeyword + strItemData + _T(" ");
		}
		pArray.Add( strItemData );
	}
	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_PROC_KEYWORD_0000::MakeKeyWord()
{
	m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("653") );
	
	//ȭ�鿡�� �������� ��� �ܾ� ����
	CString strItemData;
	GetControlData( _T("CM_BO_CAT_PROC_0390"), _T("Ű����"), strItemData );
	strItemData.TrimLeft();
	strItemData.TrimRight();
	INT iStart = strItemData.Find( _T(" ") );
	if ( iStart == 0 ) {
		strItemData = strItemData.Mid(1);
		iStart = strItemData.Find( _T(" ") );
	}
	while ( iStart != -1 ) {
		CString sToken;
		sToken = strItemData.Mid( 0, iStart );
		//===================================================
		//2010.01.07 ADD BY PJW : �����Ͱ� null�� ���� �������� �ʴ´�.
//		m_pInfo->pMarcMgr->SetItem(&m_marc, _T("653$a"), sToken , _T(""), ADD_SUBFIELD);
		if( !sToken.IsEmpty() )
		{
			m_pInfo->pMarcMgr->SetItem(&m_marc, _T("653$a"), sToken , _T(""), ADD_SUBFIELD);
		}
		//===================================================

		strItemData = strItemData.Mid( iStart + 1 );
		iStart = strItemData.Find( _T(" ") );
	}		
	//ȭ�鿡�� �������� ��� �ܾ� ����
	//marc�� insert
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("653$a"), strItemData , _T(""), ADD_SUBFIELD);

	//Ű���� �������� �߰�..
	CString strStreamMarc;
	m_pInfo->pMarcMgr->Encoding( &m_marc, strStreamMarc ) ;
	SetStreamMarc (strStreamMarc);

}

VOID CBO_CAT_PROC_KEYWORD_0000::OnOK() 
{
EFS_BEGIN
	MakeKeyWord();

	CDialog::OnOK();

EFS_END
}

BOOL CBO_CAT_PROC_KEYWORD_0000::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_KEYWORD_0000::OnbUNLESSWORDINFO() 
{
EFS_BEGIN
	
	CCatCharSetManagementDlg dlg(this);
	dlg.SetCatagory(_T("�ҿ��"));
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_PROC_KEYWORD_0000::OnbAUXWORD() 
{
EFS_BEGIN

	CCatCharSetManagementDlg dlg(this);
	dlg.SetCatagory(_T("����"));
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_PROC_KEYWORD_0000::OnbWORD() 
{
EFS_BEGIN

	CCatCharSetManagementDlg dlg(this);
	dlg.SetCatagory(_T("�ܾ�"));
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_PROC_KEYWORD_0000::OnbKEYWORD() 
{
	m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("653") );

	m_strKeyword.Empty();
	
	for ( INT i = 0 ; i < pArray.GetSize() ; i++ ) {
		m_strKeyword += _T(" ") + pArray.GetAt( i );
	}

	m_strKeyword.TrimLeft();
	m_strKeyword.TrimRight();
	
	SetControlData( _T("CM_BO_CAT_PROC_0390"), _T("Ű����"), m_strKeyword);
}

HBRUSH CBO_CAT_PROC_KEYWORD_0000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}