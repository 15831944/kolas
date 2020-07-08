// WebHoldImport.cpp : implementation file
//

#include "stdafx.h"
#include "WebHoldImport.h"

#include "BO_CAT_AUTO_REGNO_GIVE.h"

// ���� - ���ں�ȯ�� 
#include "..\..\..\����\K2UP_Common/MakeSearchData.h"

#include "..\..\..\ESL\MARC\MarcToolKit\CreateAutorCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebHoldImport dialog


CWebHoldImport::CWebHoldImport(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebHoldImport)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_pMDataFormCombo = NULL;
	m_pMDataStatusCombo = NULL;

	m_pDM_Species = NULL;
	m_pDM_Index = NULL;
	m_pDM_Book = NULL;

	m_pCatApi = NULL;

}

CWebHoldImport::~CWebHoldImport()
{
	
}

VOID CWebHoldImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebHoldImport)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebHoldImport, CDialog)
	//{{AFX_MSG_MAP(CWebHoldImport)
	ON_BN_CLICKED(IDC_bClose, OnbClose)
	ON_BN_CLICKED(IDC_bImport, OnbImport)
	ON_BN_CLICKED(IDC_bSearch, OnbSearch)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_CBN_SELCHANGE(IDC_cDataStatus, OnSelchangecDataStatus)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebHoldImport message handlers

VOID CWebHoldImport::OnbClose() 
{
	//�ݱ� ��ư
	CDialog::OnCancel();
}


BOOL CWebHoldImport::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_pCatApi = new CCatApi_BO(this);

	if (InitESL_Mgr(_T("SM_����_����_������")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_����_������"));
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox(_T("FindCM Error"));
		return FALSE;
	}

	m_pDM = FindDM(_T("DM_����_����_������"));
	if ( m_pDM == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}
	
	m_pDM_Species = FindDM ( _T("DM_����_����DM_������") );
	if ( m_pDM_Species == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}

	m_pDM_Index = FindDM (_T("DM_����_����DM_��������") );
	if ( m_pDM_Index == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}

	m_pDM_Book = FindDM (_T("DM_����_����DM_å����") );
	if ( m_pDM_Index == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}

	m_pGrid = static_cast<CESL_Grid*>(FindControl(_T("CM_����_����_������"), _T("GRID")));
	if (m_pGrid == NULL) return FALSE;


	//�ڷ�����, �ڷ���� �޺��ڽ� �ʱ�ȭ
	if ( !InitComboControl() ) return FALSE;

	//�ʱ� ������ ComboBox ������ �˻� ������ ����
	CString strWhere = MakeWhereQuery();
	m_pDM->RefreshDataManager( strWhere );

	m_pCM->AllControlDisplay();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CWebHoldImport::InitComboControl()
{
	//Combo_Box �� �ʱ�ȭ
	//�ڷ����� �޺��ڽ�
	m_pMDataFormCombo = new CJComboBox;
	m_pMDataFormCombo->SubclassDlgItem( IDC_cDataForm, this );
	m_pMDataFormCombo->FormatList( 1, DT_LEFT, false , EDGE );
	m_pMDataFormCombo->RequireFlag = FALSE;
	
	//�ڷ������� ��ġ��ȣ�� �̿�
	m_pMDataFormCombo->AddItem( _T("�������"), 0, 0 );		//AC
	m_pMDataFormCombo->AddItem( _T("�����뿪"), 0, 1 );		//RW
	m_pMDataFormCombo->AddItem( _T("�μ������ڷ�"), 0, 2 );	//PH
	
	m_pMDataFormCombo->SetCurSel( 0 );


	//�ڷ���� �޺��ڽ�
	m_pMDataStatusCombo = new CJComboBox;
	m_pMDataStatusCombo->SubclassDlgItem( IDC_cDataStatus, this );
	m_pMDataStatusCombo->FormatList( 1, DT_LEFT, false , EDGE );
	m_pMDataStatusCombo->RequireFlag = FALSE;

	m_pMDataStatusCombo->AddItem( _T("��û"), 0, 0 ); //A01
	m_pMDataStatusCombo->AddItem( _T("�Ϸ�"), 0, 1 ); //B01
	m_pMDataStatusCombo->AddItem( _T("����"), 0, 2 ); //C01
	
	//�⺻���� ��û���� ����
	m_pMDataStatusCombo->SetCurSel( 1 );


	return TRUE;
}

VOID CWebHoldImport::OnbImport() 
{
	//�� ����
	ExecuteWebImport();
}

VOID CWebHoldImport::OnbDelete() 
{
	//���� ��� �ڷῡ ���� ���� ó��
	m_pGrid->SelectMakeList();
	INT nGridIdx = m_pGrid->SelectGetHeadPosition();
	if ( nGridIdx < 0 )
	{
		AfxMessageBox(_T("������ �����͸� ������ �ּ���"));
		return;
	}

	if (MessageBox(_T("������ �ڷḦ �����Ͻðڽ��ϱ�?"), _T("���� Ȯ��"), MB_YESNO) != IDYES)
	{
		return;
	}

	INT ids = -1;
	while ( nGridIdx != -1 ) 
	{
		/*
		CString strWorkingStatus;
		m_pDM->GetCellData(_T("�ڷ����"),  nGridIdx , strWorkingStatus ) ;
		if ( strWorkingStatus != _T("C01") ) 
		{
			AfxMessageBox(_T("�ڷ���°� '����'�� �ڷḸ �����Ͻ� �� �ֽ��ϴ�."));
			return ;
		}
		*/

		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		ids = dm.StartFrame();
		if ( ids < 0 ) return;
		
		CString strReckey;
		m_pDM->GetCellData(_T("REC_KEY"), nGridIdx , strReckey);

		CString strQuery;
		strQuery = _T("DELETE WEB_IMPORT_TMP_TBL WHERE REC_KEY = '") + strReckey + _T("';");

		dm.AddFrame(strQuery);
		ids = dm.SendFrame();
		if ( ids < 0 ) return;

		ids = dm.EndFrame();
		if ( ids < 0 ) return;

		nGridIdx = m_pGrid->SelectGetNext();
	}

	AfxMessageBox( _T("���������� ���� �Ǿ����ϴ�."));
	OnbSearch();

	return;
		
}

CString CWebHoldImport::MakeWhereQuery()
{
	//WHERE �������� ����..
	CString strWhere = _T("");
	//�ڷ�����, �ڷ���¿� ���� Where �������� �����Ѵ�~!!
	// 0 - �μ������ڷ�
	// 1 - �����뿪
	// 2 - �������
	INT nDataFormidx = m_pMDataFormCombo->GetCurSel();

	switch ( nDataFormidx )
	{
	case 0:
			strWhere = _T(" SEPARATE_SHELF_CODE = 'PH' ");
			break;
	case 1:
			strWhere = _T(" SEPARATE_SHELF_CODE = 'RW' ");
			break;
	case 2:
			strWhere = _T(" SEPARATE_SHELF_CODE = 'AC' ");
			break;
	}

	// 0 - ����
	// 1 - ��û
	// 2 - �Ϸ�
	INT nDataStatusidx = m_pMDataStatusCombo->GetCurSel();
	switch ( nDataStatusidx )
	{
	case 0:
			strWhere += _T(" AND WORKING_STATUS = 'C01' ");
			break;
	case 1:
			strWhere += _T(" AND WORKING_STATUS = 'A01' ");
			break;
	case 2:
			strWhere += _T(" AND WORKING_STATUS = 'B01' ");
			break;
	}

	return strWhere;
}

VOID CWebHoldImport::OnbSearch() 
{
	CString strWhere = MakeWhereQuery();
	m_pDM->RefreshDataManager( strWhere );
	INT i = m_pDM->GetRowCount();

	m_pCM->AllControlDisplay();
	
}

BOOL CWebHoldImport::ExecuteWebImport()
{
	m_pGrid->SelectMakeList();
	INT nGridIdx = m_pGrid->SelectGetHeadPosition();
	if ( nGridIdx < 0 ) 
	{
		AfxMessageBox(_T("������ �ڷḦ ������ �ּ���"));
		return FALSE;
	}

	UpdateData();
	m_pDM_Species->InsertRow(-1);
	m_pDM_Index->InsertRow(-1);
	m_pDM_Book->InsertRow(-1);

	INT ids = -1;
	while ( nGridIdx != -1 ) 
	{

		CMarc *pMarc = NULL;
		pMarc = new CMarc;

		ids = m_pDM->StartFrame();
		if ( 0 > ids ) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		//Marc ����
		if ( !MakeImportMarc( pMarc , nGridIdx ) ) return FALSE;

		if ( !ExecuteMarcImport( nGridIdx ) ) return FALSE;		

		
		//�߰� ������Ʈ ���� �ۼ�
		//WEB_IMPORT_TMP_TBL �� ���� ����
		//DL�� �߰� ���� �ۼ�


		//���������� ������ �Ǿ����� WEB_IMPORT_TMP_TBL TBL�� ���°��� �����Ѵ�.
		if ( !ExtraQuery( nGridIdx ) ) return FALSE;

		ids = m_pDM->SendFrame();
		if ( 0 > ids ) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		ids = m_pDM->EndFrame();
		if ( 0 > ids ) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}


		nGridIdx = m_pGrid->SelectGetNext();
	}

	AfxMessageBox(_T("���������� ���ԵǾ����ϴ�"));
	OnbSearch();

	return TRUE;
}

BOOL CWebHoldImport::MakeImportMarc(CMarc *pMarc, INT nGridIdx )
{
	//���Կ� ��ũ�� �����Ѵ�
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapMarcAlias_DMAlias;
	InitMapMarcTag( mapMarcAlias_DMAlias );

	CString strStreamMarc = MakeStreamMarc( pMarc, mapMarcAlias_DMAlias, nGridIdx );
	if ( strStreamMarc.IsEmpty() ) return FALSE;
	

	m_pInfo->pMarcMgr->Decoding( strStreamMarc, pMarc );
	
	INT ids = -1;
	ids = MakeCallNo( pMarc , nGridIdx);
	if ( ids < 0 ) return FALSE;

	m_pInfo->pMarcMgr->Encoding( pMarc, strStreamMarc );
	
	//�ڵ尪���� ��������
	CodeApply ();

	//������ StreamMarc�� �� Tbl�� �ִ´�
	m_pDM_Species->SetCellData(_T("BS_MARC"), strStreamMarc, 0 ) ;

	return TRUE;
}

VOID CWebHoldImport::InitMapMarcTag( CMap<CString, LPCTSTR, 
									 CString, LPCTSTR>& mapMarcAlias_DMAlias )
{
	//pmapMarcAlias_DMAlias->RemoveAll( );
	mapMarcAlias_DMAlias.SetAt(_T("008@0-39"),_T("008�����"));
	mapMarcAlias_DMAlias.SetAt(_T("245$d"),_T("����"));
	mapMarcAlias_DMAlias.SetAt(_T("245$a"),_T("����"));
	mapMarcAlias_DMAlias.SetAt(_T("300$a"),_T("������"));
	mapMarcAlias_DMAlias.SetAt(_T("260$b"),_T("������"));
	mapMarcAlias_DMAlias.SetAt(_T("260$c"),_T("�����"));
	mapMarcAlias_DMAlias.SetAt(_T("950$b"),_T("����"));
	mapMarcAlias_DMAlias.SetAt(_T("020$a"),_T("ISBN"));
	mapMarcAlias_DMAlias.SetAt(_T("505$a"),_T("���"));
}

CString CWebHoldImport::MakeStreamMarc( CMarc *pMarc , 
									   CMap<CString, LPCTSTR, 
									   CString, LPCTSTR>& mapMarcAlias_DMAlias, 
									   INT nGridIdx )
{
	INT ids = -1;
	CString strMarcAlias;
	CString strDMAlias;
	CString strItemData;
	CString strStreamMarc = _T("");

	POSITION pos = mapMarcAlias_DMAlias.GetStartPosition();
	while (pos)
	{		
		mapMarcAlias_DMAlias.GetNextAssoc(pos, strMarcAlias, strDMAlias);
		ids = m_pDM->GetCellData(strDMAlias, nGridIdx, strItemData);
		if ( strDMAlias == _T("008�����") )
		{
			CString result;
			CTime t = CTime::GetCurrentTime();
			CString strTmpYear;
			strTmpYear.Format(_T("%d"), t.GetYear());
			strTmpYear = strTmpYear.Mid(2);
			strItemData.Format(_T("%s%02d%02d"), strTmpYear, t.GetMonth(), t.GetDay());
			ids = 0;
		}
		
		if ( strDMAlias == _T("����") ) 
		{
			if ( strItemData.Left(1) != _T("\\") )
			{
				CString strTmp;
				strTmp.Format (_T("\\%s"), strItemData );
				strItemData = strTmp;
			}
		}
		
		if ( strMarcAlias == _T("008@0-39") ) 
		{
			for ( INT i = strItemData.GetLength() ; i < 40 ; i++ )
			{
				if ( i < 38 ) 
					strItemData = strItemData + _T(" ");
				else 
				{
					//�����뿪 �ڷ��ϰ�� �����ڵ忡 AK �־��ش�
					INT nDataFormidx = m_pMDataFormCombo->GetCurSel();
					if ( nDataFormidx == 1 )
					{
						strItemData = strItemData + _T("AK");
						break;
					}
					else
					{
						strItemData = strItemData + _T("  ");
						break;
					}
				}
			}		
		}

		if (ids >= 0)
			ids = m_pInfo->pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData);
	}

	//��Ϲ�ȣ �ο��Ͽ� 049 Tag ����
	MakeRegNO049Tag( pMarc, nGridIdx );
	m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc );

	return strStreamMarc;
}

BOOL CWebHoldImport::MakeRegNO049Tag( CMarc *pMarc, INT nGridIdx )
{
	
	INT ids = -1;
	INT nTotalSelectGridCnt = m_pGrid->SelectGetCount();
	CString strRegCode;
	m_pCM->GetControlMgrData(_T("BB_��ϱ���"), strRegCode );

	//��������ȣ�� ������.
	CString strGetLastNumQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '") + strRegCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE");
	CString strLasuNum;
	ids = m_pDM->GetOneValueQuery( strGetLastNumQuery, strLasuNum );
	if ( ids < 0 ) return FALSE;

	//������ ��ȣ TBL�� Lock�� �Ǵ�
//	CString strTblLockQuery = _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = LAST_NUMBER WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '") + strRegCode + _T("';");
//	ids = m_pDM->AddFrame( strTblLockQuery );
//	if ( ids < 0 ) return FALSE;

	INT iLastNum = _ttoi( strLasuNum.GetBuffer(0) );
	iLastNum ++;		

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	CString strTmp;	
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	strTmp.Format(_T("%010d"), iLastNum );
// 
// 	//��Ϲ�ȣ �����
// 	CString strRegNo = strRegCode + strTmp;
	
	CString strRegNo;
	strTmp.Format(_T("%d"), iLastNum );
	//��Ϲ�ȣ �����
	strRegNo = m_pInfo->MakeRegNo(strRegCode, strTmp);
	strTmp = strRegNo.Mid(nRegCodeLength);
	//=====================================================
	

	m_pDM->SetCellData(_T("��Ϲ�ȣ"), strRegNo, nGridIdx );
	
	//��Ϲ�ȣ �ֱ�
	m_pInfo->pMarcMgr->SetItem( pMarc, _T("049$l"), strRegNo );
	m_pDM_Book->SetCellData(_T("BB_��Ϲ�ȣ"), strRegNo , 0 );


	//��ġ��ȣ ���� �ֱ�
	CString strShelfCode;
	m_pDM->GetCellData( _T("��ġ��ȣ"), nGridIdx, strShelfCode );
	m_pDM_Book->SetCellData( _T("BB_��ġ��ȣ_�ڵ�"), strShelfCode, 0 );
	m_pInfo->pMarcMgr->SetItem( pMarc, _T("049$f"), strShelfCode );

	m_pDM_Book->SetCellData( _T("BB_��ϱ���_�ڵ�"), strRegCode, 0 );

	CString strUpdateLastNum;
	strUpdateLastNum.Format(_T("%d"), iLastNum );
	CString strUpdateQuery = _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = ") + strUpdateLastNum  +_T(" WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '") + strRegCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE;");
	ids = m_pDM->AddFrame( strUpdateQuery );
	if ( ids < 0 ) return FALSE;

	return TRUE;
}


VOID CWebHoldImport::CodeApply()
{
	EFS_BEGIN

	BeginWaitCursor();
	
	INT nRowCount = m_pDM_Index->GetRowCount();
	for (INT row = 0; row < nRowCount; row++)
		CodeApply(row);
	
	EndWaitCursor();


	EFS_END
}

VOID CWebHoldImport::CodeApply(INT nIdxDM)
{
	EFS_BEGIN

	// HSS_040524 �������
	CString strHD_Species, strHD_Index;
	strHD_Species = _T("BS");
	strHD_Index = _T("IBS");

	// TODO : ��ũ ���к�~ �ڵ� ����
	INT ids = -1;
	
	CStringArray arrIgnorAlias;
	arrIgnorAlias.Add( strHD_Index + _T("_���ı���_�ڵ�"));
	arrIgnorAlias.Add( strHD_Index + _T("_��ü����_�ڵ�"));

	// ���� ����, ��ü������ �˸°� �־��ش�.

	CMarc marc;
	CString strStreamMarc;
	CString strFormCode;
	CString strMatCode;

	// ���� ����, ��ü������ �˸°� �־��ش�.
	m_pCM->GetControlMgrData(_T("IBS_���ı���_�ڵ�"), strFormCode ) ;
	m_pCM->GetControlMgrData(_T("IBS_��ü����_�ڵ�"), strMatCode ) ;


	m_pDM_Index->SetCellData( strHD_Index + _T("_���ı���_�ڵ�"), strFormCode, nIdxDM);
	m_pDM_Index->SetCellData( strHD_Index + _T("_��ü����_�ڵ�"), strMatCode, nIdxDM);

	
	MakeDMFromCM(m_pCM, m_pDM_Species, -1, nIdxDM, strHD_Species, &arrIgnorAlias);
	MakeDMFromCM(m_pCM, m_pDM_Index, -1 , nIdxDM, strHD_Index, &arrIgnorAlias);

	EFS_END
}



INT CWebHoldImport::MakeDMFromCM(CESL_ControlMgr *pCM, 
								  CESL_DataMgr *pDM, 
								  INT nAppMode /*= -1*/,
								  INT nIdx /*= -1*/, 
								  CString strToAliasHeader /*= _T("")*/, 
								  CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	EFS_BEGIN

	if (pCM == NULL || pDM == NULL) return -1;
	
	INT i = 0;
	INT ids = -1;
	INT nFind = -1;

	if (nIdx == -1)
	{
		ids = pDM->InsertRow(-1);
		if (ids < 0) return -1;
		nIdx = pDM->GetRowCount() - 1;
	}

	CString strAlias;
	CString strItemData;
	CESL_Control_Element *pCE = NULL;
	
	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		
		strAlias = pCE->GetAlias();
		if (strAlias.IsEmpty()) continue;

		// �Ⱥ��̴��� üũ
		if (!reinterpret_cast<CWnd*>(pCM->FindControl(strAlias))->IsWindowEnabled()) continue;

		// Control ���� �����´�
		ids = pCM->GetControlMgrData(strAlias, strItemData);
		if (ids < 0) continue;

		// ��� ����
		if (!strToAliasHeader.IsEmpty())
		{
			nFind = strAlias.Find(_T("_"));
			if (nFind >= 0)
				strAlias = strAlias.Mid(nFind + 1);
			strAlias = strToAliasHeader + _T("_") + strAlias;
		}

		// Default Ignore List
		// ����
		if ( strAlias == _T("IBS_���Ա���_�ڵ�") ) continue;

		// �ٸ��� DB FLAG�� �ٲپ��ش�.
		if (pDM->GetCellData(strAlias, nIdx) != strItemData)
		{
			ids = pDM->SetCellData(strAlias, strItemData, nIdx);
			ids = pDM->SetCellData(strToAliasHeader + _T("_DB_FLAG"), _T("I"), nIdx);
		}
		if (ids < 0) continue;
	}
	return 0;

	EFS_END
	return -1;
}


INT CWebHoldImport::MakeCallNo(CMarc *pMarc, INT nGridIdx )
{

	CCreateAutorCode *pCreateAuthorCode = NULL;

	CString strClass;
	m_pCM->GetControlMgrData(_T("IBS_�з�ǥ����"), strClass );

	CString strClassNo;
	m_pDM->GetCellData(_T("�з���ȣ"), nGridIdx, strClassNo );

	if (strClass.IsEmpty())
	{
		AfxMessageBox( _T("�з�ǥ������ �����ϼ���.") );
		return -1;
	}

	CString strClassCodeTag;
	if (strClass == _T("1"))
	{
		strClassCodeTag = _T("056$a");
	}
	else if (strClass == _T("2"))
	{
		strClassCodeTag = _T("082$a");
	}
	else
		return -1;

	m_pInfo->pMarcMgr->SetItem( pMarc, strClassCodeTag , strClassNo );
	m_pInfo->pMarcMgr->SetItem( pMarc, _T("090$a") , strClassNo );

	CString s090a = strClassNo;

	CESL_DataMgr *pDmAuthorRule =  FindDM( _T("DM_BO_CAT_PROC_0300_AUTHOR_RULE") );
	if ( pDmAuthorRule == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_AUTHOR_RULE error") );
		return -1;
	}

	CString strPublishFormCode;
	INT ids = -1;
	strPublishFormCode = _T("MO");
	
	CString sFormCode;
	CString sSubRegCode;
	CString strItemData;
	m_pCM->GetControlMgrData(_T("IBS_���ı���_�ڵ�"), sFormCode) ;
	m_pCM->GetControlMgrData(_T("IBS_������ϱ���_�ڵ�"), sSubRegCode) ;


	// �з���ȣ�� �������� �ϰ� �˻��� �ؼ� ������, �з���ȣ�� NULL�� �ڷḦ �˻��Ѵ�.
	CString strWhere;
	strWhere.Format(_T("PUBLISH_FORM_CODE = '%s' AND FORM_CODE = '%s' AND SUB_REG_CODE = '%s' AND CLASS_CODE = '%s'")
					 , strPublishFormCode, sFormCode, sSubRegCode, strClass);

	ids = pDmAuthorRule->RefreshDataManager(strWhere);
	if (ids < 0) return -1;
	if ( pDmAuthorRule->GetRowCount() < 1 ) {
		// �з���ȣ�� NULL �ΰ����� �ٽ� �˻��Ѵ�.
		strWhere.Format(_T("PUBLISH_FORM_CODE = '%s' AND FORM_CODE = '%s' AND SUB_REG_CODE = '%s' AND CLASS_CODE IS NULL")
					 , strPublishFormCode, sFormCode, sSubRegCode);
		ids = pDmAuthorRule->RefreshDataManager(strWhere);

		if (ids < 0) return -1;
		if (pDmAuthorRule->GetRowCount() < 1) {
			AfxMessageBox (_T("���ڱ�ȣ������ �߸��Ǿ����ϴ�.\n�ڵ� ���� �´� ���ڱ�ȣ ������ �ִ��� Ȯ�����ּ���"));
			return -1;
		}
	}
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDmAuthorRule, _T("ǥ�����ڱ�ȣ"), _T("�������"));
	INT nAuthorCount = pDmAuthorRule->GetRowCount();
		
	CString strTagCount;
	CString strTag;
	CString strSubfield;
	CString strRuleType;
	if ( nAuthorCount > 0 ) 
	{
		pDmAuthorRule->GetCellData( _T("TAG_CNT"), 0, strTagCount );
		pDmAuthorRule->GetCellData( _T("TAG"), 0, strTag );
		pDmAuthorRule->GetCellData( _T("�ĺ���ȣ"), 0, strSubfield );
		pDmAuthorRule->GetCellData( _T("�������"), 0, strRuleType );
	}

		if (strRuleType != _T("���Լ���ȣ")) 
	{	
		CString strFilePath;
		// ���ڱ�ȣ ������ �´� ��ũ �����͸� �����´�.
		INT iTagCnt = _ttoi( strTagCount.GetBuffer(0) );
		for ( INT i = 1 ; i <= iTagCnt ; i++ ) {
			CString sTagSub;
			if ( i == 1 ) {
				if ( strTag.GetLength() < 3 || strSubfield.GetLength() < 1 ) {
					AfxMessageBox( _T("���ڱ�ȣ������ �߸��Ǿ����ϴ�.") );
					return -1;
				}
				sTagSub = strTag.Left(3) + _T("$") + strSubfield.Left(1);
			}
			else {
				INT iStart = (i-1)*4;
				INT iTmp = (i-1)*2 +1;
				if ( strTag.GetLength() < iStart+3 || strSubfield < iTmp ) {
					AfxMessageBox( _T("���ڱ�ȣ������ �߸��Ǿ����ϴ�.") );
					return -1;
				}
				sTagSub = strTag.Mid(iStart, 3) + _T("$") + strSubfield.Mid( (i-1)*2 , 1 );
			}
			strItemData = _T("");
			m_pInfo->pMarcMgr->GetItem(pMarc, sTagSub, strItemData);
			if ( strItemData != _T("") ) {
				break;
			}
		}

		CMakeSearchData MakeData(this);

		//���� �ѱ� ��ȯ
		ids = MakeData.ChangeHanjaToHangul( strItemData );	
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}
		
		//����, Ư������ ����
		ids = MakeData.GetKeyWord( strItemData );				
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}

		// �����Ͱ� ���̸�
		if (strItemData.IsEmpty())
		{
			AfxMessageBox (_T("���ڱ�ȣ������ �߸��Ǿ����ϴ�."));
			return -1;
		}

		// ���ڱ�ȣ ������ ���� strItemData : result�Դϴ�.
		
		// ���ϼ�
		if (strRuleType == _T("���ϼ�"))
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;

			CString strAuthorAddData;
			CString strAuthorCode;

			ids = pCreateAuthorCode->GetJangIlSeaAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();

			if (ids < 0) return -1;

			strItemData = strAuthorCode;
		}
		//���ʸ�
		else if( strRuleType == _T("���ʸ�") )
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			pCreateAuthorCode->GetJungPilMoAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			if (ids < 0) return -1;
			
			strItemData = strAuthorCode;
			
		}
		//���缭���ڱ�ȣ
		else if( strRuleType == _T("���缭���ڱ�ȣ") )
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;

			CString strAuthorAddData;
			CString strAuthorCode;

			strAuthorCode = pCreateAuthorCode->GetOrientBookData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();

			if (ids < 0) return -1;

			strItemData = strAuthorCode;

		}
		else if ( strRuleType == _T("ī�ͼ���") )
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			strAuthorCode = pCreateAuthorCode->GetCutterSanbornAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			if (ids < 0) return -1;
			
			strItemData = strAuthorCode;
		
		}
		else if ( strRuleType == _T("ELORD") ) 
		{

			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			//Elord ���ڱ�ȣ ������ ���ڿ� ������ ������� ������������ ©�� ó���Ѵ�.
			INT iTagCnt = _ttoi( strTagCount.GetBuffer(0) );
			for ( INT i = 1 ; i <= iTagCnt ; i++ ) {
				CString sTagSub;
				if ( i == 1 ) {
					sTagSub = strTag.Left(3) + _T("$") + strSubfield.Left(1);
				}
				else {
					INT iStart = (i-1)*3+1;
					INT iTmp = (i-1)*2 +1;
					sTagSub = strTag.Mid(iStart, 3) + _T("$") + strSubfield.Mid( (i-1)*2 , 1 );
				}
				
				m_pInfo->pMarcMgr->GetItem(pMarc, sTagSub, strItemData);
				if ( strItemData != _T("") ) {
					break;
				}
			}

			ids = pCreateAuthorCode->GetElordAuthorDate(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			if (ids < 0) return -1;
			
			strItemData = strAuthorCode;
			
		}
		else
		{
			if (strItemData.GetLength()<2)
			{
				AfxMessageBox( _T("���缭�� ���ڴ� �ѱ� 1�ڸ��̻��̰ų� ����2�ڸ� �̻��̾�� �մϴ�.") );
				return -1;
			}
				
			CString sData = strItemData.Left(2);
			CString sAuthorMid, sAuthorNo;
			if ( strItemData.Mid(2,1) == _T(" ") ) 
				sAuthorMid = _T(",") + strItemData.Mid(3,2);
			else 
				sAuthorMid = strItemData.Mid(2,2);
			
			if ( sAuthorMid != _T("") ) 
			{			
				//���ϰ��
				strFilePath = _T("..\\CFG\\") + strRuleType + _T(".txt");
				
				FILE *fp;
				fp = _tfopen( strFilePath , _T("rb") );
				
				if ( fp == NULL ) {
					AfxMessageBox( _T("���ڱ�ȣ���� ������ �����ϴ�.") );
					return -1;
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
				
				TCHAR cLine[256];
				while( _fgetts( cLine, 256, fp ) ) {
					CString sLine;
					sLine.Format( _T("%s"), cLine );
					CString tnp = sLine.Mid(0,3);
					tnp.Replace( _T(" "), NULL );
					if ( sAuthorMid == tnp ) {
						INT iPos = sLine.Find(_T("|"));
						sAuthorNo = sLine.Mid( iPos+1, 3 );
						sAuthorNo.Replace( _T(" "), NULL );
						if ( sAuthorMid.Left(1) == _T(",") ) {
							sAuthorNo = _T(",") + sAuthorNo;
						}
						break;
					}				
				}
			}
			
			if ( sAuthorNo.IsEmpty())
			{
				AfxMessageBox( _T("���ڱ�ȣ�� ã�� �� �����ϴ�. ���ڱ�ȣ���� ������ Ȯ���ϼ���.") );
			}
			
			strItemData = sData + sAuthorNo;
		}
		
		//�ʼ������
		CString sTitle;
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$a"), sTitle);

		// ��Ī ����
		CIndex::RemoveAllParenthesis(sTitle);

		//���� �ѱ� ��ȯ
		ids = MakeData.ChangeHanjaToHangul( sTitle );	
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}
		
		//����, Ư������ ����
		ids = MakeData.GetKeyWord( sTitle );				
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}
		
		// ������ ���� ���ڸ� ��������
		INT nCount = 0;
		CString sFirstWord;
		INT nLenght = sTitle.GetLength();
		for (INT idx = 0; idx < nLenght; idx++)
		{
			TCHAR tch = sTitle.GetAt(idx);
			if (!_istspace(tch)) 
			{
				sFirstWord += tch;
				nCount++;
			}
			
			if (nCount == 2) break;
		}

/* ���� ���� : ���ڱ�ȣ������ ����ö2ǥ���� ������ ù���ڸ� �������� �ʼ��� ���������� ���� 
 * ���� ��¥ : 2003.12.05
 * ���� ��� : ��ȫ�� 
 */

		CString strResult;
		CString strQuery = _T(" SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '����' and CLASS_ALIAS = '���ڱ�ȣ�������' AND MANAGE_CODE = UDF_MANAGE_CODE");
		
		//m_pDM_Parent->GetOneValueQuery(strQuery, strResult);
		pDmAuthorRule->GetOneValueQuery(strQuery, strResult);


		if ( strResult != _T("����ù��")) {
			if ( sFirstWord != _T("") ) {
				FILE *fpFirst;
				strFilePath = _T("..\\CFG\\FIRST_CHAR_TBL.txt");
				fpFirst = _tfopen( strFilePath , _T("rb") );			
				if ( fpFirst == NULL ) {
					AfxMessageBox( _T("FIRST_CHAR_TBL.tx ������ �����ϴ�.") );
					return -1;
				}
				
				//++++++++++++++++++++++++++++++++++++++++++
				//Unicode�϶� 2byte�ǳʶ��
				FILE* fpUni = fpFirst;
			
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
				
				TCHAR cLine[256];
				while( _fgetts( cLine, 256, fpFirst ) ) {
					CString sLine;
					sLine.Format( _T("%s"), cLine );
					if ( sLine.Left(1) == _T(".") ) {
						continue;
					}
					CString sTmp = sLine.Left(2);
					if ( sFirstWord == sLine.Left(2) ) {
						INT iPos = sLine.Find(_T("|"));
						CString sResult = sLine.Mid( iPos+1, 2 );
						strItemData = strItemData + sResult;
						break;
					}
				}
			}
		}
		else 
			strItemData = strItemData + sFirstWord;

		m_pInfo->pMarcMgr->SetItem(pMarc, _T("090$a"), s090a);
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("090$b"), strItemData);
	}
	
	// �����ϰ�� 090$c �����
	CArray<CString, CString&> pArray;
	CString sItemData;
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("049$v"), sItemData, &pArray);
		
	CString sRegPre;
	INT iCnt = pArray.GetSize();
	for ( INT i = 1 ; i <= iCnt ; i ++ ){
		CString sReg = pArray.GetAt(i-1);
		if ( 1 == i ) {
			sItemData = sReg;
		}
		else if ( sRegPre != sReg ) {
			sItemData = sItemData + _T(",") + sReg;
		}
		sRegPre = sReg;
	}

	if ( sItemData != _T("") ) {
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("090$c"), sItemData);
	}


	return 0;

}

BOOL CWebHoldImport::ExecuteMarcImport( INT nGridIdx )
{

	// ��DM
	CESL_DataMgr DM_Species;
	DM_Species.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Species.InitDMPKFromFile(_T("DM_����_����DM_������"));
	DM_Species.SetMarcMgr(m_pInfo->pMarcMgr);

	// ����DM
	CESL_DataMgr DM_Index;
	DM_Index.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Index.InitDMPKFromFile(_T("DM_����_����DM_��������"));
	DM_Index.SetMarcMgr(m_pInfo->pMarcMgr);
	
	// åDM
	CESL_DataMgr DM_Book;
	DM_Book.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Book.InitDMPKFromFile(_T("DM_����_����DM_å����"));
	DM_Book.SetMarcMgr(m_pInfo->pMarcMgr);

	// ��DM
	CESL_DataMgr DM_Volume;
	DM_Volume.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Volume.InitDMPKFromFile(_T("DM_����_����DM_������"));
	DM_Volume.SetMarcMgr(m_pInfo->pMarcMgr);

	INT ids = -1;
	INT idxDM = -1;
	INT nCount = -1;

	INT nColCount = -1;

	INT nRowTo = 0;
	INT nRowsTo = 0;

	CString strData;
	CString strErrorMsg;
    CString nTotalCnt;
    CString strTotalMsg;
    CString nFalseCnt;
	CString nTrueCnt;

	// HSS_040524 �������
	CString strHD_Book, strHD_Species, strHD_Index;
	strHD_Species = _T("BS");
	strHD_Book = _T("BB");
	strHD_Index = _T("IBS");

	// ��ũ���� DM���� �����Ѵ�.
	MakeMarcAdjustDM(m_pDM_Species, 0, m_pDM_Index, 0, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);


	//����� ����
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	ids = DM_Book.SetCellData( strHD_Book + _T("_�����"), result, 0);

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//��Ϲ�ȣ 12�ڸ� �ǵ��
	CString strRegNo;
	ids = m_pDM_Book->GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), 0, strRegNo);
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 		if ( strRegNo.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// 			strRegNo = strRegNo.Left(2) + sRight;
// 		}
// 	}

	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
		if ( strRegNo.GetLength() > nRegCodeLength ) {
			strRegNo = m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength) );
		}
	}
	//=====================================================
	
	ids = DM_Book.SetCellData( strHD_Book + _T("_��Ϲ�ȣ"), strRegNo, 0 );

	//��������
	DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("1") , 0 );

	//REC_KEY ó��
	//Web ���Խÿ��� web ���� TBL�� Rec_key�� ���ƾ� �Ѵ�.
	DM_Species.SetCellData( strHD_Species + _T("_��KEY"), m_pDM->GetCellData(_T("REC_KEY"), nGridIdx ), 0 );

	//��ġ��ȣ ��ϱ��� ����
	CString strRegCode;
	strRegCode.Empty();
	m_pCM->GetControlMgrData( _T("BB_��ϱ���"), strRegCode);
	DM_Book.SetCellData(_T("BB_��ϱ���_�ڵ�"), strRegCode , 0);

	CString strShelfCode;
	strShelfCode.Empty();
	m_pDM->GetCellData( _T("��ġ��ȣ"), nGridIdx, strShelfCode );
	m_pDM_Book->SetCellData( _T("BB_��ġ��ȣ_�ڵ�"), strShelfCode, 0 );
	DM_Book.SetCellData( _T("BB_��ġ��ȣ_�ڵ�"), strShelfCode, 0 );

	m_pDM_Index->SetCellData( _T("IBS_��ġ��ȣ_�ڵ�"), strShelfCode, 0 );
	DM_Index.SetCellData( _T("IBS_��ġ��ȣ_�ڵ�"), strShelfCode, 0 );

	// 0 - �μ������ڷ�
	// 1 - �����뿪
	// 2 - �������
	INT nDataFormidx = m_pMDataFormCombo->GetCurSel();
	switch ( nDataFormidx )
	{
		case 1:
			m_pDM_Index->SetCellData( _T("IBS_�ѱ����α����ȣ"), _T("AK"), 0 );
			break;
	}

	// 2005.08.03 ADD BY PDJ
	// 5163 �������� ��� �谡���� ������ ������ �����Ѵ�.
	CString	strShelfLocCode;
	INT nBookCnt;	
	INT	i;
	
	m_pCM->GetControlMgrData( _T("IBS_�ڷ�Ǳ���_�ڵ�"), strShelfLocCode);
	nBookCnt = DM_Book.GetRowCount();
	for ( i=0 ; i<nBookCnt ; i++ )
	{
		DM_Book.SetCellData(_T("BB_�谡��ġ��ȣ"), strShelfLocCode, i );
	}
	//���� ����
	ids = m_pCatApi->WebImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, NULL, NULL, NULL, MNG_IMPORT );
	if ( ids < 0 ) 
	{
		AfxMessageBox( m_pCatApi->GetLastError() );
		return FALSE;
	}

	return TRUE;
}

INT CWebHoldImport::MakeMarcAdjustDM(	CESL_DataMgr *pDM_From_Species, 
										INT nFromSpeciesRow, 
										CESL_DataMgr *pDM_From_Index, 
										INT nFromIndexRow, 
										CESL_DataMgr *pDM_To_Species, 
										CESL_DataMgr *pDM_To_Index, 
										CESL_DataMgr *pDM_To_Book, 
										CESL_DataMgr *pDM_To_Volume)
{
	EFS_BEGIN

	if (!pDM_From_Species ||
		!pDM_From_Index ||
		!pDM_To_Index ||
		!pDM_To_Species ||
		!pDM_To_Book ||
		!pDM_To_Volume ||
		nFromSpeciesRow < 0 ||
		nFromIndexRow < 0) return -1;

	// ������ �ʱ�ȭ
	pDM_To_Species->FreeData();
	pDM_To_Index->FreeData();
	pDM_To_Book->FreeData();
	pDM_To_Volume->FreeData();

	pDM_To_Species->InsertRow(-1);
	pDM_To_Index->InsertRow(-1);
	pDM_To_Book->InsertRow(-1);
	pDM_To_Volume->InsertRow(-1);
	

	// �� ī��
	INT ii = pDM_To_Species->GetRowCount();
	m_pCatApi->CopyDM(pDM_From_Species, nFromSpeciesRow, pDM_To_Species, 0);
	// ���� ī��
	m_pCatApi->CopyDM(pDM_From_Index, nFromIndexRow, pDM_To_Index, 0);

	// ��ũ��ũ�� ���߱� ���� 049�� �ش��ϴ� ������ å DM�� �ִ´�...
	CMarc marc;
	CString strStreamMarc = pDM_To_Species->GetCellData( _T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	CString strRegNo;
	CString strItemData;
	CArray<CString, CString&> arrItem;
	m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arrItem);
	INT nRowCount = arrItem.GetSize();

	// 049 Tag�� ������ �ٽ� �����.
	if (nRowCount >= 0) 
	{	
		pDM_To_Book->FreeData();
	}

	// 049 Tag�� ������ŭ å�� �����.
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pDM_To_Book->InsertRow(-1);

		// å�η� �÷���
		pDM_To_Book->SetCellData(_T("BB_å�η��÷���"), _T("B"), idx);

		// DB FLAG ����
		pDM_To_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);

		// �� -> å
		m_pCatApi->CopyDM(pDM_To_Species, 0, pDM_To_Book, idx, _T("BB"));
		// ���� -> å
		m_pCatApi->CopyDM(pDM_To_Index, 0, pDM_To_Book, idx, _T("BB"));
	}

	// ��ǥå ����
	INT nRepBookIdx = m_pCatApi->SetRepresentationBook(pDM_To_Book, _T("BB"));

	// ��ũ�� ��ũ����
	m_pCatApi->ConvertMarcToCatDM(&marc, pDM_To_Species, pDM_To_Index, pDM_To_Book, pDM_To_Volume);

	return 0;

	EFS_END
	return -1;

}

VOID CWebHoldImport::OnSelchangecDataStatus() 
{
	// 2007.06.25 REM BY PDJ
	// 5163�䱸�������� ����ڷ���� �����ϴٷ�
	/*
	INT nDataStatusidx = m_pMDataStatusCombo->GetCurSel();
	if ( 0 == nDataStatusidx )
		GetDlgItem(IDC_bDelete)->ShowWindow(SW_SHOW);
	else 
		GetDlgItem(IDC_bDelete)->ShowWindow(SW_HIDE);
	*/	
}

BOOL CWebHoldImport::ExtraQuery( INT nGridIdx )
{

	//WEB_IMPORT_TMP_TBL �� ���� ���� ����
	INT ids = -1;
	CString strQuery;
	strQuery = _T("UPDATE WEB_IMPORT_TMP_TBL SET WORKING_STATUS = 'B01' WHERE REC_KEY ='") + m_pDM->GetCellData(_T("REC_KEY"), nGridIdx) + _T("';");
	ids = m_pDM->AddFrame( strQuery );
	if ( ids < 0 ) return FALSE;

	//DL �� �䱸����
	//DL_CONTENTS_MAP_TBL �� �Է����־�� �� ����

	//�����ؼ� ������־�� �� ��
	//REC_KEY, REG_DATE

	//DB���� ���� ������ �Է��ؾ� �� ��
	//SPECIES_KEY, PATH, FILE_NAME, , SERVER_ID, ACCESSION_NO,

	//�⺻�� ����
	//TYPE				= ????????
	//VOL_SORT_NO		='10'
	//DB_FLAG			='M'
	//MAX_SERVICE		=DB �⺻��
	//SERVICE_CLASS		=DB �⺻��

	CString strServerId;
	strQuery = _T("SELECT SERVER_ID FROM DL_CONTENTS_SERVER_TBL WHERE TYPE = 'PDF' AND ACTIVE_FLAG = '0'");
	ids = m_pDM->GetOneValueQuery ( strQuery , strServerId );
//	if ( ids < 0 ) return FALSE;

	CString strRecKey;
	strRecKey.Empty();
	ids = m_pDM->MakeRecKey( strRecKey );
	if ( ids < 0 ) return FALSE;
	
	CString strRegDate;
	CTime t = CTime::GetCurrentTime();
	strRegDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	m_pDM->InitDBFieldData();
    m_pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"), strRecKey );
	m_pDM->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"), m_pDM->GetCellData(_T("REC_KEY"), nGridIdx ) );
	m_pDM->AddDBFieldData (_T("VOL_SORT_NO"),_T("NUMERIC"), _T("10") );
	m_pDM->AddDBFieldData (_T("DB_FLAG"),_T("STRING"), _T("M") );
	m_pDM->AddDBFieldData (_T("ACCESSION_NO"),_T("STRING"), m_pDM->GetCellData(_T("��Ϲ�ȣ"), nGridIdx ) );
	m_pDM->AddDBFieldData (_T("REG_DATE"),_T("STRING"), strRegDate );
	m_pDM->AddDBFieldData (_T("MAX_SERVICE"), _T("STRING") , _T("9999") );
	m_pDM->AddDBFieldData (_T("SERVICE_CLASS"),_T("STRING"), _T("0") );
	m_pDM->AddDBFieldData (_T("TYPE"),_T("STRING"), _T("PDF") );
	//m_pDM->AddDBFieldData (_T("SERVER_ID"),_T("STRING"), strServerId );
	m_pDM->AddDBFieldData (_T("SERVER_ID"),_T("NUMERIC"), m_pDM->GetCellData(_T("SERVER_ID"), nGridIdx ) );

	m_pDM->AddDBFieldData (_T("PATH"),_T("STRING"), m_pDM->GetCellData(_T("PATH"), nGridIdx ) );
	m_pDM->AddDBFieldData (_T("FILENAME"),_T("STRING"), m_pDM->GetCellData(_T("FILENAME"), nGridIdx ) );
//	m_pDM->AddDBFieldData (_T("ORI_FILENAME"),_T("STRING"), m_pDM->GetCellData(_T("ORI_FILENAME"), nGridIdx ));

    ids = m_pDM->MakeInsertFrame (_T("DL_CONTENTS_MAP_TBL"));
	if ( ids < 0 ) return FALSE;


	//IDX_BO_TBL �� ���������� ������� WONMUN_YN�� _T("Y") �� ����
	strQuery = _T("UPDATE IDX_BO_TBL SET WONMUN_YN = 'Y' WHERE REC_KEY = ") + m_pDM->GetCellData(_T("REC_KEY"), nGridIdx ) + _T(";");
	ids = m_pDM->AddFrame( strQuery );
	if ( ids < 0 ) return FALSE;

	return TRUE;

}

HBRUSH CWebHoldImport::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
