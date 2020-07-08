// SE_LOC_15001.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_15001.h"
#include "..\\..\\..\\����\\�˻�\\SEBookDetailViewer\\SEBookDetailViewerDlg.h"
#include "SE_LOC_1597.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1500 dialog


CSE_LOC_1500::CSE_LOC_1500(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1500)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_arrStrBookKey.RemoveAll();
	m_arrStrVolKey.RemoveAll();
	m_arrStrDeleteBookKey.RemoveAll();
}

CSE_LOC_1500::~CSE_LOC_1500()
{
}

VOID CSE_LOC_1500::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1500)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1500, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1500)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1500 message handlers

BOOL CSE_LOC_1500::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1500")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new CBL_SE_LOC_15(this);
		
	const INT gridCnt = 1;
	CString gridAlias[ gridCnt ] = { _T("���α׸���") };
	CArray< CString, CString >removeListArray;
	CESL_Grid *pGrid = NULL;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_SE_LOC_1500"), gridAlias[ i ] );
		if( !pGrid )
			return -1;

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_LOC_1500::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);	

	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}
	//=====================================================
	//2009.01.29 UPDATE BY PJW : �׸��忡 ��ũ�� �������� ��
// 	pGrid->MoveWindow(-2, 35 , cx+2, cy );
	pGrid->MoveWindow(-2, 35 , cx+2, cy-35 );
	//=====================================================
	
}

BOOL CSE_LOC_1500::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}



INT CSE_LOC_1500::PrintList( CString title, CString strPrintType)
{
	EFS_BEGIN

	INT ids;

	/// 2004�� 7�� 27�� ������ ����
	/// �ǽð� �ΰ� �ڷ� ����� ����Ǿ�� �Ѵ�.
	if( title.Compare(_T("�ǽð� �ΰ��ڷ� ���(2)") ) == 0 )
	{
		PrintShelfList(strPrintType);

		return 0;
	}

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_SE_LOC_1500_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1500") , _T("���α׸���") );

	if ( strPrintType == _T("���") )
	{
		// pSrcDM -> pPrintDM���� ����
		ids = MakePrintDM( _T("DM_SE_LOC_1500"), pGrid, pPrintDM);
	}
	else if ( strPrintType == _T("�����ڷ����") )
	{
		ids = MakePrintDM(pPrintDM);
	}
	if ( ids != 0 ) return ids;

	// �Ϲ����� �ݺ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("���ӽǽð��ΰ��ڷ���"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 1;
	}

	pSIReportManager->SetDataMgr(0, pPrintDM , 0);

	// ���� �߰�
	pSIReportManager->SetTempVariable( _T("����") , title );

	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return 0;

	EFS_END
	return -1;

}

INT CSE_LOC_1500::MakePrintDM( CString strSrcDMName, CESL_Grid* pGrid, CESL_DataMgr* pPrintDM )
{
EFS_BEGIN

	if ( strSrcDMName.IsEmpty() || NULL == pGrid || NULL == pPrintDM )
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePrintDM") );

	CESL_DataMgr *pSrcDM = FindDM( strSrcDMName );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakePrintDM") ); 

	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return 1;
	}

	const INT nCopyCnt=23;
	pPrintDM->FreeData();

	//
	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("���Ա���") , _T("���Ա���") } , 
		{ _T("����") , _T("����") } , 
		{ _T("��ȣ��") , _T("��ȣ��") } , 
		{ _T("������") , _T("������") } , 
		{ _T("���Ͱ��") , _T("���Ͱ��") } , 
		{ _T("�ǽð��ΰ�����") , _T("�ǽð��ΰ�����") } ,
		{ _T("�����"), _T("�����") },
		{ _T("��������"), _T("��������") } , 
		{ _T("�����ڷ����"), _T("�����ڷ����") } ,
		{ _T("����Ϲ�ȣ"), _T("����Ϲ�ȣ") } ,
		{ _T("�谡�����������"), _T("�谡��������ڵ�") } ,
		{ _T("���ʹ谡����"), _T("�谡��������") } ,
		{ _T("�ڷ��"), _T("�ڷ��") } ,
		{ _T("�谡�����"), _T("�谡�����") } ,
		{ _T("�̿����ѱ���"), _T("�̿����ѱ���") } ,
		{ _T("�ڷ����"), _T("�ڷ����") } ,
		{ _T("�Թ�ȣ"), _T("�Թ�ȣ") } ,
		{ _T("û����ȣ"), _T("û����ȣ") },
		{ _T("�����"), _T("�����") },
		{ _T("��������ȣ��"), _T("��������ȣ��") },
		{ _T("������KEY"), _T("������KEY") },
		{ _T("��ȣKEY"), _T("��ȣKEY") },
		{ _T("�ηϿ���"), _T("�ηϿ���") }
	};


	CString sData;
	INT ids = -1;
	
	pGrid->SelectMakeList();
	
	INT nSelectCnt = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();
	
	// 0 : ��� ��� , 1 : ������ �׸��常 ���
	INT nMode = 0;
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
	INT nIndex;

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		pPrintDM->InsertRow(-1);



		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("��ȣ") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			// Get data
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );

			if( j == 0 )
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("���Ա���") , sData , sData );

			// Set data to printDM
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintList") );
		}
		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}

	return 0;
EFS_END
	return -1;
}


INT CSE_LOC_1500::MakePrintDM( CESL_DataMgr *pPrintDM )
{
EFS_BEGIN

	if ( NULL == pPrintDM ) 
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePrintDM") );


	// 1. ����� ��Ű�� ������ ������ �����Ѵ�.
	CString strOption = _T("B.REC_KEY IN (");
	INT nCount = m_arrStrBookKey.GetSize();

	if ( nCount<=0 ) 
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return 1;
	}

	for (INT idx = 0; idx < nCount; idx++)
	{
		strOption += m_arrStrBookKey.GetAt(idx);
		if (idx != nCount - 1)
			strOption += _T(", ");
	}

	strOption += _T(")");


	// ������ �ڷ� ����� ���
	// ���� Ű�� ������ 
//	nCount = m_arrStrDeleteBookKey.GetSize();
//	if ( nCount > 0 )
//	{
//		CString strTmpOption = _T(" AND B.REC_KEY NOT IN (");
//		
//		for (INT idx = 0; idx < nCount; idx++)
//		{
//			strTmpOption += m_arrStrDeleteBookKey.GetAt(idx);
//			if (idx != nCount - 1)
//				strTmpOption += _T(", ");
//		}
//		strTmpOption += _T(")");
//
//		if (  nCount > 0 ) 
//			strOption += strTmpOption;
//		
//	}

	// 2. ����� DM ����
	INT ids = pPrintDM->RefreshDataManager(strOption);
	if ( ids<0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakePrintDM") );

	// 3. ��ȣ, ���Ա��� SET
	CString sData;
	nCount = pPrintDM->GetRowCount();
	for (  INT i=0; i<nCount; i++ )
	{
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("��ȣ") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakePrintDM") );

		ids = pPrintDM->GetCellData(_T("���Ա���"), i, sData);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakePrintDM") );

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("���Ա���") , sData , sData );

		// Set data to printDM
		ids = pPrintDM->SetCellData( _T("���Ա���") , sData , i );
	}


	return 0;
EFS_END
	return -1;
}

INT CSE_LOC_1500::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}

// ����ڷ� ����
INT CSE_LOC_1500::SavePrintList( CString strSpeciesKeyAlias)
{
EFS_BEGIN
	
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_SE_LOC_1500") );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePrintDM") ); 

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1500") , _T("���α׸���") );
	if ( NULL == pGrid ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("SavePrintList") );

	pGrid->SelectMakeList();
	
	INT nSelectCnt = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();

	CString strKey=_T("");

	INT ids= -1;
	INT nIndex = pGrid->SelectGetIdx();
	for ( INT i=0; i<nSelectCnt; i++ )
	{
		//�ߺ�üũ BookKey
		ids = pSrcDM->GetCellData(_T("å����KEY"), nIndex, strKey );
		if ( ids<0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("SavePrintList") );

		
		if ( CheckPrintListDup(&m_arrStrBookKey, strKey) > 0 ) ;
		else m_arrStrBookKey.Add(strKey);

		//�ߺ�üũ���� VolKey
		ids = pSrcDM->GetCellData(_T("��ȣKEY"), nIndex, strKey );
		if ( ids<0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("SavePrintList") );

		if ( CheckPrintListDup(&m_arrStrVolKey, strKey) > 0 ) ;
		else m_arrStrVolKey.Add(strKey);


		nIndex = pGrid->SelectGetNext();
	}
	
	INT cnt  = m_arrStrBookKey.GetSize();
	CString sMsg;
	sMsg.Format(_T("�� %d���� �ڷᰡ �����Ǿ����ϴ�."), cnt);
	AfxMessageBox(sMsg);

	return 0;
EFS_END
	return -1;	
}

// ����ڷ� ������ Ű �ߺ�üũ
INT CSE_LOC_1500::CheckPrintListDup(CStringArray *arrStrKey, CString strKey)
{
EFS_BEGIN
	
	INT nCnt = arrStrKey->GetSize();
	
	CString strTmp;
	for ( INT i=0; i<nCnt; i++)
	{
		strTmp = arrStrKey->GetAt(i);

		if ( strTmp == strKey ) return 1;
	}
	

	return 0;
EFS_END
	return -1;
}

INT CSE_LOC_1500::DetailView()
{
	EFS_BEGIN
		
	CString sParentCMAlias = _T("CM_SE_LOC_1500");
	CString sParentDMAlias = _T("DM_SE_LOC_1500");
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("DetailView") );
	
	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("DetailView") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("���α׸���") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("DetailView") );
	
	if( pParentDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("�ڷḦ �˻����ֽʽÿ�") );
		return 0;
	}
	
	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT , INT > asGridRow;
	
	INT nMode = 0;
	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	INT nStartPos = 0;
	INT nTmpStartPos = pGrid->GetIdx();
	if( nSelectCnt > 1 )
	{
		nMode = 1;
		pGrid->SelectGetHeadPosition();
	}
	else
	{
		nSelectCnt = pParentDM->GetRowCount();
		nStartPos = pGrid->GetIdx();
	}
	
	INT nIndex;
	
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 1 )
			nIndex = pGrid->SelectGetIdx();
		else
			nIndex = i;
		
		pParentDM->GetCellData( _T("������KEY") , nIndex, sSpeciesKey );
		sSpeciesKeyList.AddTail( sSpeciesKey );
		asGridRow.Add( nIndex );

		if( nIndex == nTmpStartPos )
			nStartPos = i;
		
		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}
	
	CSEBookDetailViewerDlg Dlg(this);
	
	Dlg.SetPKList( sSpeciesKeyList , pGrid , asGridRow , nStartPos );
	Dlg.DoModal();
	
	return 0;
	
	EFS_END
	return -1;
	
}

BEGIN_EVENTSINK_MAP(CSE_LOC_1500, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_LOC_1500)
	ON_EVENT(CSE_LOC_1500, IDC_gridMAIN, -601 /* DblClick */, OnDblClickgridMAIN, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_LOC_1500::OnDblClickgridMAIN() 
{
	// �� ���� �����ֱ�	
	CString sCMAlias = _T("CM_SE_LOC_1500");
	CString sGridAlias = _T("���α׸���");
	
	CESL_ControlMgr *pCM = FindCM(sCMAlias);
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnClickgridMAIN") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("OnClickgridMAIN") );
	
	if( pGrid->GetMouseRow() < 1 ) return;
	
	pGrid->SelectMakeList();
	if( pGrid->SelectGetCount() > 0 )
		pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );

	
	DetailView();
}

INT CSE_LOC_1500::PrintShelfList(CString strPrintType)
{
	EFS_BEGIN

	INT ids;

	// �ΰ��� ��������
	CString sShelfDate , sShelfLoc , sTitle , sBookCnt;

	CSE_LOC_1597 ShelfDate(this);
	ShelfDate.DoModal();

	if( !ShelfDate.m_bPrint ) return 0;

	sShelfDate = ShelfDate.m_sShelfDate;

	// �ڷ�Ǻ� > ���� > ��ȣ�� �� ������ �Ǿ�� �Ѵ�.

//	CESL_DataMgr *pSrcDM = FindDM( _T("DM_SE_LOC_1500") );
//	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintShelfList") ); 

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_SE_LOC_1500_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("PrintShelfList") );

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SE_LOC_1500") , _T("���α׸���") );

//	if( pSrcDM->GetRowCount() < 1 )
//	{
//		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
//		return 1;
//	}


	if ( strPrintType == _T("���") )
	{
		// pSrcDM -> pPrintDM���� ����
		ids = MakePrintDM( _T("DM_SE_LOC_1500"), pGrid, pPrintDM);
	}
	else if ( strPrintType == _T("�����ڷ����") )
	{
		ids = MakePrintDM(pPrintDM);
	}
	if ( ids != 0 ) return ids;

	// pSrcDM -> pPrintDM���� ����
	const INT nCopyCnt=22;
	//pPrintDM->FreeData();

	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("���Ա���") , _T("���Ա���") } , 
		{ _T("����") , _T("����") } , 
		{ _T("��ȣ��") , _T("��ȣ��") } , 
		{ _T("������") , _T("������") } , 
		{ _T("���Ͱ��") , _T("���Ͱ��") } , 
		{ _T("�ǽð��ΰ�����") , _T("�ǽð��ΰ�����") } ,
		{ _T("�����"), _T("�����") },
		{ _T("��������"), _T("��������") } , 
		{ _T("�����ڷ����"), _T("�����ڷ����") } ,
		{ _T("����Ϲ�ȣ"), _T("����Ϲ�ȣ") } ,
		{ _T("�谡�����������"), _T("�谡��������ڵ�") } ,
		{ _T("���ʹ谡����"), _T("�谡��������") } ,
		{ _T("�ڷ��"), _T("�ڷ��") } ,
		{ _T("�谡�����"), _T("�谡�����") } ,
		{ _T("�̿����ѱ���"), _T("�̿����ѱ���") } ,
		{ _T("�ڷ����"), _T("�ڷ����") } ,
		{ _T("�Թ�ȣ"), _T("�Թ�ȣ") } ,
		{ _T("û����ȣ"), _T("û����ȣ") },
		{ _T("�����"), _T("�����") },
		{ _T("��������ȣ��"), _T("��������ȣ��") },
		{ _T("������KEY"), _T("������KEY") },
		{ _T("��ȣKEY"), _T("��ȣKEY") }
	};


	CString sData;
	
/*	pGrid->SelectMakeList();
	
	INT nSelectCnt = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();
	
	// 0 : ��� ��� , 1 : ������ �׸��常 ���
	INT nMode = 0;
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
	INT nIndex;

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();

		pPrintDM->InsertRow(-1);



		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("��ȣ") , sData , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("PrintShelfList") );

		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("PrintShelfList") );

			if( j == 0 )
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("���Ա���") , sData , sData );

			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintShelfList") );
		}
		if( nMode == 1)
			pGrid->SelectGetNext();
		
	}
*/

	const INT nSortCnt = 3;
	CString sSortAlias[nSortCnt] =
	{
		_T("�ڷ��") , _T("������KEY") , _T("��ȣKEY") 
	};

	INT nSortCond[nSortCnt] =
	{
		TRUE, TRUE, TRUE
	};

	ids = pPrintDM->SORT( nSortCnt , sSortAlias , nSortCond , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("PrintShelfList") );

	/// ���� , ��ȣ���� �̾Ƴ���.
	

	// �Ϲ����� �ݺ��� ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("���ӽǽð��ΰ��ڷ���(�����߰�)"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 1;
	}


	INT nRowCnt = pPrintDM->GetRowCount();
	CESL_DataMgr TmpPrintDM;
	CLocCommonAPI::MakeCopyDM( this , pPrintDM , &TmpPrintDM );

	pSIReportManager->SetDataMgr(0, &TmpPrintDM , 0);

	pSIReportManager->DeleteTempPageFiles();
	
	// �ΰ� ���� , �ڷ�� , ���� , å��
	BOOL bFirst = TRUE;
	CString sNextShelfLoc;
	CString sTotalCnt;
	INT nSpeciesCnt = 0;
	INT nVolCnt = 0;
	INT nInterIndex = 0;
	BOOL bPrint = FALSE;
	for( INT i = 0 ; i < nRowCnt ; i++ )
	{
		if( bFirst )
		{
			ids = pPrintDM->GetCellData( _T("�ڷ��") , i , sShelfLoc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("PrintShelfList") );
			ids = pPrintDM->GetCellData( _T("����") , i , sTitle );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("PrintShelfList") );

			bFirst = FALSE;
		}
		
		if( i == (nRowCnt-1) )
			bPrint = TRUE;
		else
		{
			ids = pPrintDM->GetCellData( _T("�ڷ��") , i+1 , sNextShelfLoc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("PrintShelfList") );

			if( sShelfLoc.Compare( sNextShelfLoc ) != 0 )
				bPrint = TRUE;
		}

		TmpPrintDM.InsertRow(-1);

		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pPrintDM->GetCellData( sField[j][0] , i , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("PrintShelfList") );

			ids = TmpPrintDM.SetCellData( sField[j][1] , sData , nInterIndex );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nInterIndex , sField[j][1] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintShelfList") );
		}
		CString sIndex;
		sIndex.Format( _T("%d") , nInterIndex+1 );
		ids = TmpPrintDM.SetCellData( _T("��ȣ") , sIndex  , nInterIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("PrintShelfList") );

		nInterIndex++;

		if( !bPrint ) 
			continue;

		ids = GetSpeciesVolCnt( nSpeciesCnt , nVolCnt , &TmpPrintDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("PrintShelfList") );

		if( sTitle.GetLength() > 30 )
			CutString( sTitle , 30 );

		sTitle = sTitle + _T(" ��");
		sBookCnt.Format( _T("%d") , TmpPrintDM.GetRowCount() );
		sTotalCnt.Format( _T("%d �� %d �� %s å") , nSpeciesCnt , nVolCnt , sBookCnt );
		pSIReportManager->SetTempVariable( _T("�ڷ��") , sShelfLoc );
		pSIReportManager->SetTempVariable( _T("����") , sTitle );
		pSIReportManager->SetTempVariable( _T("å��") , sBookCnt );
		pSIReportManager->SetTempVariable( _T("�ΰ�����") , sShelfDate );
		pSIReportManager->SetTempVariable( _T("�հ�") , sTotalCnt );



		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("PrintShelfList") );

		bFirst = TRUE;
		bPrint = FALSE;
		nInterIndex = 0;
		TmpPrintDM.FreeData();
	}

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	



	return 0;


	EFS_END

	return -1;

}

INT CSE_LOC_1500::GetSpeciesVolCnt( INT &nSpeciesCnt , INT &nVolCnt , CESL_DataMgr* pDM )
{
	EFS_BEGIN

	INT ids;

	nSpeciesCnt = 0;
	nVolCnt = 0;

	CString sPreSpeciesKey , sPreVolKey;
	CString sSpeciesKey , sVolKey;

	INT nRowCnt = pDM->GetRowCount();

	for( INT i = 0 ; i < nRowCnt ; i++ )
	{
		ids = pDM->GetCellData( _T("������KEY") , i , sSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSpeciesVolCnt") );
		ids = pDM->GetCellData( _T("��ȣKEY") , i , sVolKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetSpeciesVolCnt") );

		if( i != 0 )
		{
			ids = pDM->GetCellData( _T("������KEY") , i-1 , sPreSpeciesKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetSpeciesVolCnt") );
			ids = pDM->GetCellData( _T("��ȣKEY") , i-1 , sPreVolKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetSpeciesVolCnt") );
		}

		if( sSpeciesKey.Compare( sPreSpeciesKey ) != 0 ) nSpeciesCnt++;
		if( sVolKey.Compare( sPreVolKey ) != 0 ) nVolCnt++;

	}


	return 0;

	EFS_END

	return -1;
}

INT CSE_LOC_1500::CutString( CString &sStr , INT nMaxLength )
{
	EFS_BEGIN

	BOOL bPreHanguel = FALSE;
	for( INT i = 0 ; i < nMaxLength ; i++ )
	{
		if( sStr.GetAt(i) < 0 )
			bPreHanguel = TRUE;
		if( sStr.GetAt(i) >= 0 )
			bPreHanguel = FALSE;
	}
	
	if( sStr.GetAt(nMaxLength-1) < 0 && bPreHanguel )
	{
		sStr = sStr.Mid( 0 , nMaxLength-1 );
	}
	else
		sStr = sStr.Mid( 0 , nMaxLength );


	sStr += _T(".. ");



	return 0;


	EFS_END

	return -1;
}

HBRUSH CSE_LOC_1500::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
