// IndexDetailViewerDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "indexgenerator.h"
#include "IndexDetailViewerDlg.h"
#include "SearchMono.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexDetailViewerDlg dialog


CIndexDetailViewerDlg::CIndexDetailViewerDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CIndexDetailViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexDetailViewerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentMgr = pParent;
	m_Create = FALSE;
	m_bRecycleData	= FALSE;
	m_pkCnt = 0;
}

CIndexDetailViewerDlg::~CIndexDetailViewerDlg()
{
	
}

void CIndexDetailViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexDetailViewerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexDetailViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CIndexDetailViewerDlg)
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_INDEX_START, OnIndexStart)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexDetailViewerDlg message handlers

BOOL CIndexDetailViewerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch( m_nDANorYON )
	{
	case DAN:
		{
			if( InitESL_Mgr(_T("SM_IndexGeneratorInfoView_Mono")) < 0 )
	{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}
		m_pIndexViewDM = FindDM(_T("DM_IndexGeneratorInfoView_Mono"));
		m_pIndexViewCM = FindCM(_T("CM_IndexGeneratorInfoView_Mono"));;
		m_pBookViewDM = FindDM(_T("DM_IndexGeneratorInfoView_Book_Mono"));
		m_pBookGrid = (CESL_Grid*)m_pIndexViewCM->FindControl(_T("�׸���"));
	}
		break;
	case YON:
		{
			if( InitESL_Mgr(_T("SM_IndexGeneratorInfoView_Serial")) < 0 )
	{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}
		m_pIndexViewDM = FindDM(_T("DM_IndexGeneratorInfoView_Serial"));
		m_pIndexViewCM = FindCM(_T("CM_IndexGeneratorInfoView_Serial"));;
		m_pBookViewDM = FindDM(_T("DM_IndexGeneratorInfoView_Book_Serial"));
		m_pBookGrid = (CESL_Grid*)m_pIndexViewCM->FindControl(_T("����_�׸���"));
	}
		break;
	default:	return FALSE;
		break;
	}


	if( Init_IndexViewerCtrl() < 0 )	return FALSE;
	if( Init_MarcViewerCtrl()  < 0 )	return FALSE;

	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CIndexDetailViewerDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// ó�� ��ũ ���� Set
	SetMarcInfo();
	// ó�� ���� ���� Set
	SetIndexInfo();
	// ó�� å ���� Set
	SetBookInfo();

	m_IndexInfoControl.Display();
	m_MarcInfoControl.Display();
	m_pBookGrid->Display();

	switch( m_nDANorYON )
	{
	case DAN:		GetDlgItem(IDC_BOOK_GRID)->ShowWindow(SW_HIDE);
		break;
	case YON:		GetDlgItem(IDC_BOOK_GRID_MONO)->ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}

	ShowHideButton();
}

HBRUSH CIndexDetailViewerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CIndexDetailViewerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CIndexDetailViewerDlg::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here

	m_idx--;
	POSITION pos = m_pkList.FindIndex( m_idx );
	m_pk = m_pkList.GetPrev( pos );
	
	m_eslGrid->SetReverse( m_selectedGridRow.GetAt( m_idx ) );

	SetMarcInfo();
	SetIndexInfo();
	SetBookInfo();

	m_IndexInfoControl.Display();
	m_MarcInfoControl.Display();
	m_pBookGrid->Display();

	ShowHideButton();

}

VOID CIndexDetailViewerDlg::OnbNEXT() 
{
	// TODO: Add your control notification handler code here

	if( m_idx == (m_pkCnt - 1) ) 
		return;
	m_idx++;
	POSITION pos = m_pkList.FindIndex( m_idx );
	m_pk = m_pkList.GetNext( pos );

	m_eslGrid->SetReverse( m_selectedGridRow.GetAt( m_idx ) );

	SetMarcInfo();
	SetIndexInfo();
	SetBookInfo();

	m_IndexInfoControl.Display();
	m_MarcInfoControl.Display();
	m_pBookGrid->Display();

	ShowHideButton();
	
}

VOID CIndexDetailViewerDlg::OnIndexStart() 
{
	INT Ids;
//	CSearchMono* m_pParentMgr;
	if( m_nDANorYON == DAN )		
	{
		Ids = ((CSearchMono*)m_pParentMgr)->CurrentIndexReproduce();
	}
//	else if( m_nDANorYON == YON )	Ids = ((CIndexReproduceMgr_Serial*)m_pParentMgr)->CurrentIndexReproduce();

	if(Ids == 0 )	ESLAfxMessageBox(_T("��������� �Ϸ�!"));	

	OnbNEXT();
}

//BOOL CIndexDetailViewerDlg::OnInitDialog() 
//{
//	CDialog::OnInitDialog();
//	
//	// �������� �������� ���Ͽ� SM/DM/CM/Grid�� �����Ѵ�.
//	
//
//	return TRUE;
//}

INT CIndexDetailViewerDlg::SetDialogInfo( CStringList &pkList, CESL_Grid *grid, CArray< INT, INT > &gridRow, INT idx )
{
	m_pkList.RemoveAll();
	
	POSITION pos = pkList.GetHeadPosition();
	while( pos )	m_pkList.AddTail( pkList.GetNext( pos ) );		

	m_pkCnt = m_pkList.GetCount();

	m_idx = idx;

	pos = m_pkList.FindIndex(m_idx);
	m_pk = m_pkList.GetNext( pos );

	m_eslGrid = grid;

	for( INT i = 0; i < gridRow.GetSize(); i++ )	m_selectedGridRow.Add( gridRow.GetAt( i ) );

	return 0;
}

INT CIndexDetailViewerDlg::Init_IndexViewerCtrl()
{
	m_IndexInfoControl.InitViewer( IDC_INDEX_VIEWER , this );
	m_IndexInfoControl.SetViewerWidth( 30 , 67 );

	return 0;
}

INT CIndexDetailViewerDlg::Init_MarcViewerCtrl()
{
	m_MarcInfoControl.InitViewer( IDC_MARC_VIEWER , this );
	m_MarcInfoControl.SetViewerWidth( 30 , 67 );

	return 0;
}

INT CIndexDetailViewerDlg::SetMarcInfo()
{
	switch( m_nDANorYON )
	{
	case DAN:		SetMarcInfo_Mono();
		break;
	case YON:		SetMarcInfo_Serial();
		break;
	default:		return -1;
		break;
	}
	
	return 0;
}


INT CIndexDetailViewerDlg::SetMarcInfo_Mono()
{
	INT Ids, i;
	CString strQuery, strMarc, strMarcAlias, strMarcData, strTbl;

	if( m_bRecycleData == TRUE )	strTbl = _T("BO_DELETE_SPECIES_TBL");
	else							strTbl = _T("BO_SPECIES_TBL");
	
	strQuery.Format(_T("SELECT MARC FROM %s WHERE REC_KEY = %s"), strTbl, m_pk);
	Ids = m_pIndexViewDM->GetOneValueQuery(strQuery, strMarc);
	if( Ids < 0 )	return -1;

	CIndexDataMgr m_IndexDataMgr(this);
	if( m_IndexDataMgr.SetDataMgr() == -1 )		return -1;

	m_IndexArray.RemoveAll();
	m_IndexDataMgr.ExtractIndexInMarc(strMarc, m_IndexArray);
	//INT nMarcDataCnt = m_IndexDataMgr.m_MarcAliasArray.GetSize();
	for( i=0; i<m_IndexArray.GetSize(); i++ )
	{
		//strMarcAlias = m_IndexDataMgr.m_MarcAliasArray.GetAt(i);
		strMarcAlias = m_IndexArray.GetAt(i);
		strMarcData = m_IndexDataMgr.m_MarcDataArray.GetAt(i);
		//strMarcData = m_IndexArray.GetAt(i);
		m_MarcInfoControl.SetControlData( strMarcAlias , strMarcData );
	}

	return 0;
}

INT CIndexDetailViewerDlg::SetMarcInfo_Serial()
{
//	INT Ids;
//	CString strQuery, strMarc, strMarcAlias, strMarcData;
//
//	strQuery.Format(_T("SELECT MARC FROM SE_SPECIES_TBL WHERE REC_KEY = %s"), m_pk);
//
//	Ids = m_pIndexViewDM->GetOneValueQuery(strQuery, strMarc);
//	if(Ids < 0) return -1;
//
//	CIndexDataMgr_Serial m_IndexDataMgr_Serial(this);
//	if(m_IndexDataMgr_Serial.SetDataMgr() == -1) return -1;
//
//	m_IndexArray.RemoveAll();
//	m_IndexDataMgr_Serial.ExtractIndexInMarc(strMarc, m_IndexArray);
//	INT nMarcDataCnt = m_IndexDataMgr_Serial.m_MarcAliasArray.GetSize();
//	for(INT i=0; i < nMarcDataCnt; i++)
//	{
//		strMarcAlias = m_IndexDataMgr_Serial.m_MarcAliasArray.GetAt(i);
//		strMarcData = m_IndexDataMgr_Serial.m_MarcDataArray.GetAt(i);
//		if(strMarcAlias == _T("�������")) strMarcData = GetPublicationStatus(strMarcData);
//		else if(strMarcAlias == _T("�����")) strMarcData = GetPublicationFreq(strMarcData);
//
//		m_MarcInfoControl.SetControlData( strMarcAlias , strMarcData );
//	}
	
	return 0;
}

INT CIndexDetailViewerDlg::SetIndexInfo()
{
	INT Ids, nPos, i;	
	CString strQuery, strIdxTitle, strIdxAuthor, strIdxPublisher, strIdxIPubYear, strIdxKeyword, strAllItem;
	CString strIdxIPublisher, strIdxITitle, strIdxVolTitle, strIndexAlias, strData, strParam, strAlias;

	switch( m_nDANorYON )
	{
	case DAN:		strQuery.Format(_T("REC_KEY = %s"), m_pk);
		break;
	case YON:		strQuery.Format(_T("I.REC_KEY = %s"), m_pk);
		break;
	default:		return -1;
		break;
	}

	Ids = m_pIndexViewDM->RefreshDataManager(strQuery);
	if( Ids < 0 )	return -1;

	//�ߺ� �����Ͱ� �����ϴ� ��� �Ǵ� �ش��ϴ� �����Ͱ� ���� ��� ����.
	Ids = m_pIndexViewDM->GetRowCount();
	if( Ids != 1 ) return -1;  
	
	for( i=0; i<m_IndexArray.GetSize(); i++ )
	{
		strIndexAlias = m_IndexArray.GetAt(i);
		m_pIndexViewDM->GetCellData(strIndexAlias, 0, strData);

		if		( m_nDANorYON == DAN )		strParam = _T("IBS_");
		else if	( m_nDANorYON == YON )		strParam = _T("ISS_");

		nPos = strIndexAlias.Find(strParam, 0);
		strAlias = strIndexAlias.Mid(nPos+_tcsclen(strParam));
		if( m_nDANorYON == YON )
		{
			if		( strAlias == _T("�������") )	strData = GetPublicationStatus(strData);
			else if	( strAlias == _T("�����") )	strData = GetPublicationFreq(strData);
		}

		m_IndexInfoControl.SetControlData(strAlias , strData );
	}
	// �Ǽ���
	if( m_nDANorYON == DAN )
	{
		m_pIndexViewDM->GetCellData(_T("IBS_�˻���Ǽ���"), 0, strData);
		m_IndexInfoControl.SetControlData(_T("�˻���Ǽ���") , strData );
	}

	return 0;
}

INT CIndexDetailViewerDlg::SetBookInfo()
{
	if		( m_nDANorYON == DAN )	SetBookInfo_Mono();
	else if	( m_nDANorYON == YON )	SetBookInfo_Serial();
	else return -1;

	return 0;
}

INT CIndexDetailViewerDlg::SetBookInfo_Mono()
{
	INT Ids, i;
	CString strQuery, strCode, strValue, strTitle, strAllTitle = _T("");
	CStringArray TargetArray;
	

	strQuery.Format(_T("S.REC_KEY = B.SPECIES_KEY AND S.REC_KEY = %s"), m_pk);

	if( m_bRecycleData == TRUE )	m_pBookViewDM->TBL_NAME = _T("BO_DELETE_BOOK_TBL B, BO_DELETE_SPECIES_TBL S");
	else							m_pBookViewDM->TBL_NAME = _T("BO_BOOK_TBL B, BO_SPECIES_TBL S");

	Ids = m_pBookViewDM->RefreshDataManager(strQuery);
	if( Ids < 0 )	return -1;

	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("�ڷ�Ǳ���"), _T("BB_�ڷ�Ǳ���"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("��ϱ���"), _T("BB_��ϱ���"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("������ϱ���"), _T("BS_������ϱ���"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("�̿����ѱ���"), _T("BB_�̿����ѱ���"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("��������"), _T("BB_��������"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("��ġ��ȣ"), _T("BB_��ġ��ȣ"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("���ı���"), _T("BS_���ı���"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("�ڷᱸ��"), _T("BS_�ڷᱸ��"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("��ü����"), _T("BS_��ü����"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("��������"), _T("BS_��������"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("�з�ǥ����"), _T("BB_�з�ǥ����"));

	for( i=0; i<m_pBookViewDM->GetRowCount(); i++ )
	{
		m_pBookViewDM->GetCellData(_T("BB_�ڷ����"), i, strCode);
		strValue = GetWorkingStatusDesc_Mono(strCode);
		m_pBookViewDM->SetCellData(_T("UDF_�ڷ����"), strValue, i);

		m_pBookViewDM->GetCellData(_T("BB_��������"), i, strTitle);
		strAllTitle += strTitle + _T(" ");
	}

	TargetArray.RemoveAll();
	ParsingString(_T(" "), strAllTitle, &TargetArray);
	strAllTitle.Empty();
	for( i=0 ; i<TargetArray.GetSize(); i++ )
	{
		if( strAllTitle.IsEmpty() == TRUE )		strAllTitle = TargetArray.GetAt(i);
		else strAllTitle += _T(" ") + TargetArray.GetAt(i);
	}

	m_MarcInfoControl.SetControlData(_T("�Ǽ���") , strAllTitle );

	return 0;
}

INT CIndexDetailViewerDlg::SetBookInfo_Serial()
{
	CString strQuery;
	strQuery.Format(_T("S.REC_KEY = V.SPECIES_KEY AND V.REC_KEY = B.VOL_KEY AND S.REC_KEY = %s"), m_pk);
	INT Ids;
	Ids = m_pBookViewDM->RefreshDataManager(strQuery);
	if(Ids < 0) return -1;

	// �ڷ�� �� �޾ƿ���.
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("�ڷ�Ǳ���"), _T("SB_�ڷ�Ǳ���"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("��ϱ���"), _T("SB_��ϱ���"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("������ϱ���"), _T("SS_������ϱ���"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("�̿����ѱ���"), _T("SB_�̿����ѱ���"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("��������"), _T("SB_��������"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("��ġ��ȣ"), _T("SB_��ġ��ȣ"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("���ı���"), _T("SS_���ı���"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("�ڷᱸ��"), _T("SS_�ڷᱸ��"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("��ü����"), _T("SS_��ü����"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("�з�ǥ����"), _T("SB_�з�ǥ����"));

	// �ڷ���� �� �޾ƿ���.
	CString strCode, strValue;
	INT nRowCount;
	nRowCount = m_pBookViewDM->GetRowCount();
	for(INT i=0; i < nRowCount; i++)
	{
		m_pBookViewDM->GetCellData(_T("SB_�ڷ����"), i, strCode);
		strValue = GetWorkingStatusDesc_Serial(strCode);
		m_pBookViewDM->SetCellData(_T("UDF_�ڷ����"), strValue, i);
	}

	return 0;
}

CString CIndexDetailViewerDlg::MakeALLITEM(CString strIdxTitle, CString strIdxAuthor, 
										   CString strIdxPublisher, CString strIdxKeyword)
{
	//IDX_ALL_ITEM�ʵ带 ä��� ���ؼ� �˻��뺻ǥ��, ������, ������, Ű���� �� �����Ѵ�.
	CString strIdxAllItem =_T("");
	strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublisher + _T(" ") + strIdxKeyword;
	CStringArray TargetArray;
	TargetArray.RemoveAll();
	ParsingString(_T(" "), strIdxAllItem, &TargetArray);
	INT cnt = TargetArray.GetSize();
	strIdxAllItem.Empty();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strIdxAllItem.IsEmpty()) strIdxAllItem = TargetArray.GetAt(i);
		else strIdxAllItem += _T(" ") + TargetArray.GetAt(i);
	}
	
	return strIdxAllItem;
}

INT CIndexDetailViewerDlg::ParsingString(CString DelimeterStr, CString strSource, 
										 CStringArray *pTargetArray, BOOL bIndexing/*=TRUE*/)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos >= 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+DelimeterStr.GetLength());
		if(bIndexing) ((CIndexTool*)pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		if(!bIndexing || !CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	if(bIndexing) ((CIndexTool*)pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(!bIndexing || !CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	return 0;
}

BOOL CIndexDetailViewerDlg::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}



CString CIndexDetailViewerDlg::GetWorkingStatusDesc_Serial(CString strCode)
{
	CString code[64];
	CString desc[64];
	INT cnt = 0;

	code[cnt] = _T("SEA111N");	desc[cnt] = _T("�����Լ��ڷ�"); cnt++;
	code[cnt] = _T("SEA112N");	desc[cnt] = _T("���԰�ȣ�ڷ�"); cnt++;
	code[cnt] = _T("SEA211N");	desc[cnt] = _T("�����Լ��ڷ�"); cnt++;
	code[cnt] = _T("SEA212N");	desc[cnt] = _T("������ȣ�ڷ�"); cnt++;
	code[cnt] = _T("SEB111N");	desc[cnt] = _T("�����ΰ��ڷ�"); cnt++;
	code[cnt] = _T("SEB112N");	desc[cnt] = _T("���������ڷ�"); cnt++;
	code[cnt] = _T("SEB113N");	desc[cnt] = _T("���������ڷ�"); cnt++;	
	code[cnt] = _T("SEB114N");	desc[cnt] = _T("�����˼��ڷ�"); cnt++;	
	code[cnt] = _T("SEL111O");	desc[cnt] = _T("�ǽð��ΰ��ڷ�"); cnt++;	
	code[cnt] = _T("SEL112N");	desc[cnt] = _T("�ǽð��谡�ڷ�"); cnt++;	
	code[cnt] = _T("SEL113O");	desc[cnt] = _T("�ǽð�����ڷ�"); cnt++;	
	code[cnt] = _T("SER111N");	desc[cnt] = _T("����ΰ��ڷ�"); cnt++;
	code[cnt] = _T("SER112N");	desc[cnt] = _T("����ڷ�"); cnt++;	
	code[cnt] = _T("SER113O");	desc[cnt] = _T("�����ι�ȣ�ο��ڷ�"); cnt++;
	code[cnt] = _T("SEC111N");	desc[cnt] = _T("��������ڷ�"); cnt++;
	code[cnt] = _T("SEC112O");	desc[cnt] = _T("�����ΰ����ڷ�"); cnt++;
	code[cnt] = _T("SEL211O");	desc[cnt] = _T("�����ΰ��ڷ�"); cnt++;
	code[cnt] = _T("SEL212N");	desc[cnt] = _T("�谡�ڷ�"); cnt++;
	code[cnt] = _T("SEL213O");	desc[cnt] = _T("���������ڷ�"); cnt++;
	code[cnt] = _T("SEL214O");	desc[cnt] = _T("�ļ��ڷ�"); cnt++;
	code[cnt] = _T("SEL215O");	desc[cnt] = _T("�����ڷ�"); cnt++;
	code[cnt] = _T("SEL216O");	desc[cnt] = _T("�н��ڷ�"); cnt++;
	code[cnt] = _T("SEL217O");	desc[cnt] = _T("�������ڷ�"); cnt++;
	code[cnt] = _T("SEL311O");	desc[cnt] = _T("���ܴ����ڷ�"); cnt++;
	code[cnt] = _T("SEL312O");	desc[cnt] = _T("���������ڷ�"); cnt++;
	//=====================================================
	//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
	// 18.09.27 JMJ KOL.RED.2018.008
	code[cnt] = _T("SEL411O");	desc[cnt] = _T("����å�η����������ڷ�"); cnt++;
	code[cnt] = _T("SEL511O");	desc[cnt] = _T("���յ���Ÿ���ݳ�"); cnt++;
	//=====================================================
	code[cnt] = _T("SEL313O");	desc[cnt] = _T("Ư�������ڷ�"); cnt++;
	code[cnt] = _T("SEL314O");	desc[cnt] = _T("��޴����ڷ�"); cnt++;
    code[cnt] = _T("SEL315O");  desc[cnt] = _T("����ڷ�"); cnt++; 


	CString strDesc = _T("");
	for(INT i=0;i<cnt;i++)
	{
		if(code[i] == strCode)
		{
			strDesc = desc[i];
			break;
		}
	
	}

	return strDesc;
}

CString CIndexDetailViewerDlg::GetWorkingStatusDesc_Mono(CString strCode)
{
	strCode.TrimLeft();		strCode.TrimRight();

	CString str;
	if(strCode == _T("BOT111O")) str = _T("�����ڷ�");
	else if(strCode == _T("BOT112O")) str = _T("�����ڷ�");
	else if(strCode == _T("BOA111N")) str = _T("���Դ���ڷ�");
	else if(strCode == _T("BOA112N")) str = _T("�ֹ��ڷ�");
	else if(strCode == _T("BOA113O")) str = _T("�̳�ǰ�ڷ�");
	else if(strCode == _T("BOA114O")) str = _T("�����ΰ����ڷ�");
	else if(strCode == _T("BOA211O")) str = _T("���������ڷ�");
	else if(strCode == _T("BOA212O")) str = _T("�����̵���ڷ�");
	else if(strCode == _T("BOR111N")) str = _T("����ΰ��ڷ�");
	else if(strCode == _T("BOR112N")) str = _T("����ڷ�");
	else if(strCode == _T("BOR113O")) str = _T("�����ι�ȣ�ο��ڷ�");
	else if(strCode == _T("BOC111O")) str = _T("�����������ڷ�");
	else if(strCode == _T("BOC112O")) str = _T("�з�����ڷ�");
	else if(strCode == _T("BOC113N")) str = _T("��������ڷ�");
	else if(strCode == _T("BOC114O")) str = _T("�����ΰ����ڷ�");
	else if(strCode == _T("BOL111O")) str = _T("�����ΰ��ڷ�");
	else if(strCode == _T("BOL112N")) str = _T("�谡�ڷ�");
	else if(strCode == _T("BOL113O")) str = _T("���������ڷ�");
	else if(strCode == _T("BOL114O")) str = _T("�ļ��ڷ�");
	else if(strCode == _T("BOL115O")) str = _T("�����ڷ�");
	else if(strCode == _T("BOL211O")) str = _T("���ܴ����ڷ�");
	else if(strCode == _T("BOL212O")) str = _T("���������ڷ�");
	//=====================================================
	//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
	// 18.09.27 JMJ KOL.RED.2018.008
	else if(strCode == _T("BOL411O")) str = _T("����å�η����������ڷ�");
	else if(strCode == _T("BOL511O")) str = _T("���յ���Ÿ���ݳ�");
	// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
	else if(strCode == _T("BOL611O")) str = _T("���յ���Ÿ������");
	//=====================================================
	else if(strCode == _T("BOL213O")) str = _T("Ư�������ڷ�");
	else if(strCode == _T("BOL214O")) str = _T("��޴����ڷ�");
	else if(strCode == _T("BOL215O")) str = _T("����ڷ�");

	return str;
}


VOID CIndexDetailViewerDlg::DisplayNext()
{
	OnbNEXT();
}


VOID CIndexDetailViewerDlg::ShowHideButton()
{
	CButton *pBtn;
	BOOL bBefore, bNext;

	// ���õ� ������ �ϳ��� ��.
	if( 1 == m_pkCnt ) {
		bBefore = FALSE;
		bNext = FALSE;
	}
	else {
		// �� ó�� Row�� ��.
		if( !m_idx ){
			bBefore = FALSE;
			bNext = TRUE;
		}
		// �� �� Row�� ��.
		else if( m_idx == m_pkCnt - 1 ) {
			bBefore = TRUE;
			bNext = FALSE;
		}
		// �� ��.
		else{
			bBefore = TRUE;
			bNext = TRUE;
		}

	}

	pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
	pBtn->EnableWindow(bBefore);

	pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
	pBtn->EnableWindow(bNext);

}



CString CIndexDetailViewerDlg::GetPublicationStatus(CString strCode)
{
	strCode.TrimLeft();		strCode.TrimRight();

	CString strData;
	strData = _T("");
	if(strCode == _T("c")) 			strData.Format(_T("��� ����ǰ� �ִ� ���๰"));
	else if(strCode == _T("d"))		strData.Format(_T("�󰣵� ���๰"));

	return strData;
}

CString CIndexDetailViewerDlg::GetPublicationFreq(CString strCode)
{
	strCode.TrimLeft();		strCode.TrimRight();

	CString strData;
	strData = _T("");
	if(strCode == _T("a")) 			strData.Format(_T("����"));
	else if(strCode == _T("b")) 	strData.Format(_T("�ݿ���"));  
	else if(strCode == _T("c")) 	strData.Format(_T("��2ȸ��"));
	else if(strCode == _T("d"))		strData.Format(_T("�ϰ�"));
	else if(strCode == _T("e")) 	strData.Format(_T("���ְ�"));
	else if(strCode == _T("f")) 	strData.Format(_T("�ݳⰣ"));
	else if(strCode == _T("g")) 	strData.Format(_T("�ݳⰣ"));
	else if(strCode == _T("h")) 	strData.Format(_T("3�� 1ȸ��"));
	else if(strCode == _T("i")) 	strData.Format(_T("��3ȸ��"));
	else if(strCode == _T("j")) 	strData.Format(_T("��3ȸ��"));
	else if(strCode == _T("m")) 	strData.Format(_T("����"));
	else if(strCode == _T("q")) 	strData.Format(_T("�谣"));
	else if(strCode == _T("s")) 	strData.Format(_T("�ݿ���"));
	else if(strCode == _T("t")) 	strData.Format(_T("��3ȸ��"));
	else if(strCode == _T("u")) 	strData.Format(_T("����� �Ҹ�"));
	else if(strCode == _T("w")) 	strData.Format(_T("�ְ�"));
	else if(strCode == _T("z")) 	strData.Format(_T("��Ÿ �����"));

	return strData;

}