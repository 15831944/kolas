// StaticsInfoMgr.cpp : implementation file
//

#include "stdafx.h"
#include "StaticsInfoMgr.h"
#include <afxdisp.h>
// 2011.09.09 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define DIVCHAR			_T(",")
/////////////////////////////////////////////////////////////////////////////
// CStaticsInfoMgr dialog


CStaticsInfoMgr::CStaticsInfoMgr(CESL_Mgr* pParent /*=NULL*/, CString strMenuName )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CStaticsInfoMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sSMAlias = _T("STATICS_INFO_MGR");
	m_sXYSettingCMAlias = _T("CM_BO_STATICS_1120");

	m_pSearchCM = NULL;
	m_pOrderStaticsDM = NULL;

	m_strMenuName = strMenuName;

	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
	m_pAxisCode_CellClick = NULL;
	// KOL.RED.2018.211 ---------------------------------------------------------------------------

}

CStaticsInfoMgr::~CStaticsInfoMgr()
{
	if( m_pOrderStaticsDM ) 
		delete[] m_pOrderStaticsDM;
	m_pOrderStaticsDM = NULL;

	if( m_pPrintMgr )
		delete m_pPrintMgr;
	m_pPrintMgr = NULL;

	// 2012.11.20 ADD BY PWR : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (�ӵ�����)
	m_dmCodeMgr.FreeData();
}

VOID CStaticsInfoMgr::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStaticsInfoMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CStaticsInfoMgr, CDialog)
	//{{AFX_MSG_MAP(CStaticsInfoMgr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticsInfoMgr message handlers

// ##############################################################################################################
// [ �Լ����� ]
//   ��迡 �ʿ��� �⺻ ���� ����
// 
// [ �̴Ͻ��� ]
//   
// ##############################################################################################################
INT CStaticsInfoMgr::MakeDMInfo()
{	
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. SM Init
	// ==========================================================================================================
	ids = InitESL_Mgr( m_sSMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeDMInfo()") );
		
	// ==========================================================================================================
	// 2. Manage Value ���� 
	// ==========================================================================================================
	ids = SetManageValue();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	// ==========================================================================================================
	// 3. DM ����
	// ==========================================================================================================
	// ���� �÷��� DM
	m_pExpColumnDM = FindDM( _T("DM_BO_STATICS_COLUMN_NAME") );
	if( !m_pExpColumnDM ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	// ����� ���� �˻� ���� ���� DM
	m_pUserMakeSearchCondDM = FindDM( _T("DM_BO_STATICS_USER_MAKE_CONDITION_INFO") );
	if( !m_pUserMakeSearchCondDM ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pResultDM = FindDM( _T("DM_BO_STATICS_RESULT") );
	if( !m_pResultDM ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pRealResultDM = FindDM( _T("DM_BO_STATICS_REAL_RESULT") );
	if( !m_pRealResultDM ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pXSumDM = FindDM( _T("DM_XSUM") );
	if( !m_pXSumDM ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pYSumDM = FindDM( _T("DM_YSUM") );
	if( !m_pYSumDM ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pXPriceSumDM = FindDM( _T("DM_X_PRICE_SUM") );
	if( !m_pXPriceSumDM ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pYPriceSumDM = FindDM( _T("DM_Y_PRICE_SUM") );
	if( !m_pYPriceSumDM ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pUserSearchCond = FindDM( _T("DM_BO_STATICS_USER_SEARCH_COND") );
	if( !m_pUserSearchCond ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	// ��� �⺻ ���� DM �����ϱ�
	m_pStaticsMainInfoDM = FindDM( _T("DM_BO_STATICS_MAIN_INFO") );
	if( !m_pStaticsMainInfoDM ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	ids = MakeStaticsMainInfoDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	// �÷� �˸��ƽ� ���� DM �����ϱ�
	m_pColumnAliasInfoDM = FindDM( _T("DM_BO_STATICS_COLUMN_ALIAS_INFO") );
	if( !m_pColumnAliasInfoDM ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	ids = MakeColumnAliasInfoDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::MakeDMInfo()") );
	
	// ����ڰ˻� ���� ���� DM �����ϱ�
	ids = MakeUserSearchCondDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("CStaticsInfoMgr::MakeDMInfo()") );
	
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// 2018.07.11 ADD BY KHY: ����� ����Ʈ ����
/*//NEW------------------------------------------------------------------------*/					
	m_pResultListDM = FindDM( _T("DM_BO_STATICS_LIST") );
	if( !m_pResultListDM ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CStaticsInfoMgr::MakeDMInfo()") );
/*//END------------------------------------------------------------------------*/
	// KOL.RED.2018.211 ---------------------------------------------------------------------------


	// 2012.11.20 ADD BY PWR : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (�ӵ�����)
	CString strQuery = _T("SELECT CLASS, TRIM(CODE), DESCRIPTION FROM CD_CODE_TBL ORDER BY CLASS, CODE");
	m_dmCodeMgr.SetCONNECTION_INFO(_T(""));
	m_dmCodeMgr.InitDataMgrRef(3);
	m_dmCodeMgr.SetDataMgrRef(0, _T("CLASS"), _T("CLASS"), _T("str"), _T(""), _T(""));
	m_dmCodeMgr.SetDataMgrRef(1, _T("CODE"), _T("CODE"), _T("str"), _T(""), _T(""));
	m_dmCodeMgr.SetDataMgrRef(2, _T("DESCRIPTION"), _T("DESCRIPTION"), _T("str"), _T(""), _T(""));
	m_dmCodeMgr.RestructDataManager(strQuery);

	// ==========================================================================================================
	// 4. Print ����
	// ==========================================================================================================	
	m_pPrintMgr = new CStatPrintMgr(this);

	return 0;	

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   �⺻ ���� ����
//   FILE���� ���� �����Ѵ�.   
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeStaticsMainInfoDM()
{
EFS_BEGIN

	INT ids;
	
	// ==========================================================================================================
	// 1. ������ Flag�� ���� Path�� �޶�����.
	// ==========================================================================================================
	// ��� Ȯ��
	CString strDir;
	GetDir( strDir );

	// 18.09.27 JMJ KOL.RED.2018.008
	// å�η� ��� �������� å�η� �ڷ� ����
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	CString strQuery;
	CString strValue;
	strQuery.Format(_T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='å�η��ý���' AND VALUE_NAME='å�η���뿩��' AND MANAGE_CODE = '%s'"),m_pInfo->MANAGE_CODE);
	dm.GetOneValueQuery(strQuery,strValue);			
	CString sMenuName = GetMenuName();	


	CString strPath;
	if ( m_sIs64lib == _T("Y") )	        strPath = strDir + _T("�⺻����_����.txt");
	else if ( m_sIsNDlib == _T("Y") )		strPath = strDir + _T("�⺻����_�����.txt");
	else if ( m_sIs5163lib == _T("Y") )		strPath = strDir + _T("�⺻����_5163.txt");

	// 18.09.27 JMJ KOL.RED.2018.008
	// å�η� ��� �������� å�η� �ڷ� ����
	else if (strValue.Compare(_T("Y"))==0 && sMenuName.Compare( LOAN_STATICS ) == 0)	strPath = strDir + _T("�⺻����_å�η�����.txt");
	else if (sMenuName.Compare(BOOK_STATICS) == 0)
	{
		CESL_ControlMgr* pCM = pParentMgr->FindCM(_T("CM_BO_LOC_2100"));		
		CESL_Control_Element* pIncludeAppendix = pCM->FindControlElement( _T("�η�����üũ") );		
		CButton* btn1 = (CButton*)(pParentMgr->GetDlgItem( pIncludeAppendix->CTRL_ID ));
		if( btn1 == NULL ) {
			btn1 = (CButton*)(pParentMgr->pParentMgr->GetDlgItem( pIncludeAppendix->CTRL_ID ));
		}
		if( btn1 == NULL || btn1->GetCheck() == FALSE ) 
		{
			strPath = strDir + _T("�⺻����.txt");
		}
		else
		{
			strPath = strDir + _T("�⺻����_�η�.txt");			
		}
	}
	else								strPath = strDir + _T("�⺻����.txt");

	// ==========================================================================================================
	// 2. ���õ� ���Ϸ� DM ����
	// ==========================================================================================================
	ids = MakeFileToDM( strPath , m_pStaticsMainInfoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeStaticsMainInfoDM()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   �÷��˸��ƽ� ���� ����
//   FILE���� ���� �����Ѵ�.   
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeColumnAliasInfoDM()
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. ������ Flag�� ���� Path�� �޶�����.
	// ==========================================================================================================
	// ��� Ȯ��
	CString strDir;
	GetDir( strDir );

	CString strPath;
	if ( m_sIsNDlib == _T("Y") )			strPath = strDir + _T("�÷��˸��ƽ�����_�����.txt");
	else if ( m_sIs5163lib == _T("Y") )		strPath = strDir + _T("�÷��˸��ƽ�����_5163.txt");
	else								strPath = strDir + _T("�÷��˸��ƽ�����.txt");	

	// ==========================================================================================================
	// 2. ���õ� ���Ϸ� DM ����
	// ==========================================================================================================
	ids = MakeFileToDM( strPath , m_pColumnAliasInfoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeColumnAliasInfoDM()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ����� �˻����� ���� ����
//   FILE���� ���� �����Ѵ�.   
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeUserSearchCondDM()
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. ������ Flag�� ���� Path�� �޶�����.
	// ==========================================================================================================
	// ��� Ȯ��
	CString strDir;
	GetDir( strDir );

	CString strPath = strDir + _T("����ڰ˻���������.txt");

	// ==========================================================================================================
	// 2. ���õ� ���Ϸ� DM ����
	// ==========================================================================================================
	ids = MakeFileToDM( strPath , m_pUserMakeSearchCondDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeUserSearchCondDM()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ���� ������ DM���� �����Ѵ�.
//   
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeFileToDM( CString sFilePath , CESL_DataMgr *pDM )
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. ���� ����
	// ==========================================================================================================
	CStdioFile fMainInfo;
	ids = fMainInfo.Open( sFilePath , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( ids==0 )
	{
		AfxMessageBox( _T("���ϸ��� �߸��Ǿ����ϴ�.\n�ùٸ� ������� Ȯ���� �ֽʽÿ�.") );
		return 1;
    }
	TCHAR cUni;
	fMainInfo.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fMainInfo.SeekToBegin();
	}

	if( fMainInfo.GetLength() == 0 ) 
	{
		fMainInfo.Close();
		return 0;
	}

	// ==========================================================================================================
	// 2. �÷� ����Ʈ ����
	//    �ֻ���� ù���� �÷������� ����
	// ==========================================================================================================
	CString strLine;
	CArray < CString , CString > asColumnList;

	fMainInfo.ReadString( strLine );
	strLine.TrimRight(); strLine.TrimLeft();

	ids = CLocCommonAPI::MakeArrayList( strLine , _T("|") , asColumnList );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeStaticsMainInfoDM()") );

	// ==========================================================================================================
	// 3. DM�� �Է�
	// ==========================================================================================================
	CArray < CString , CString > asDataList;

    while ( fMainInfo.ReadString( strLine ) ) 
	{
        strLine.TrimRight(); strLine.TrimLeft();
		if( strLine.IsEmpty() ) continue;

		// 2011.10.04 ADD BY PWR : ����ȸ������ �߰� ����
		if(FALSE == m_pInfo->m_bKLUse)
		{
			if(-1 < sFilePath.Find(_T("�÷��˸��ƽ�����")) && -1 < strLine.Find(_T("����ȸ�����к�"))) continue;
		}

		pDM->InsertRow(-1);

		ids = CLocCommonAPI::MakeArrayList( strLine , _T("|") , asDataList );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeStaticsMainInfoDM()") );

		for( INT i = 0 ; i < asDataList.GetSize() ; i++ )
		{
			CString strManageCode;
			CString strData;
			strData = asDataList.GetAt(i);
			strManageCode.Format(_T("'%s'"), m_pInfo->MANAGE_CODE);
			strData.Replace(_T("UDF_MANAGE_CODE"), strManageCode);

			// 2011.10.04 ADD BY PWR : ����ȸ������ �߰� ����
			if(FALSE == m_pInfo->m_bKLUse) strData.Replace(_T(",����ȸ�����к�"), _T(""));
			ids = pDM->SetCellData( asColumnList.GetAt(i) , strData , pDM->GetRowCount()-1 );
			if( ids < 0 )
			{
				CString sMsg;
				sMsg.Format( _T("LINE : %s , pDM->SetCellData(%s,%s,%d) "), strLine ,  asColumnList.GetAt(i) , asDataList.GetAt(i) , pDM->GetRowCount()-1 );
				AfxMessageBox( sMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::MakeStaticsMainInfoDM()") );
		}
	}

	fMainInfo.Close();

	return 0;

EFS_END
	return -1;
}

// �÷��� ������ ���´�.
// nMode�� ���� �����̳� �ڵ带 �����´�.
INT CStaticsInfoMgr::GetColumnNames( CString sStaticsClass , CString sAlias , CStringArray &asColumnName , INT nMode )
{
EFS_BEGIN

	INT ids;
	
	asColumnName.RemoveAll();

	if( m_pExpColumnDM->GetRowCount() < 1 )
		m_pExpColumnDM->FreeData();

	CString sTableName;
	CString sExpColumnName;

	// ���̺� �̸� ����
	// �˻� ���� ����
	CString sColumnAlias;
	if( nMode == 2 )
		sColumnAlias = _T("�����ڵ�");
	else 
		sColumnAlias = _T("��������");
	ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") , sAlias , _T("���̺��") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );

	ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") , sAlias , sColumnAlias , sExpColumnName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );

	// ���� ���̺��� �������� �ʴ´ٸ�
	if( sTableName.IsEmpty() )
	{
		// ����Ͽ� ���õ� �˸��ƽ��� �����Ѵٸ� �װɷ� ����Ѵ�. 
		CString sDate1 , sDate2;
		if( (sAlias.Compare(_T("��"))==0) || (sAlias.Compare(_T("��"))==0) || (sAlias.Compare(_T("��"))==0) || (sAlias.Compare(_T("�ð��뺰"))==0) )
		{
			ids = GetTimeColumnNames( sStaticsClass , sAlias , asColumnName );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetColumnNames") );

		}
		// 2009.10.19 ADD BY CJY : �����߰�
		// �̰����� Į���̸��� �߰����ְ�, Į���˸��ƽ����������� �����׸��� �׷쿩�θ� Y���� N���� �����Ͽ���

 		// 2012.11.20 UPDATE BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (����������ȣ �Ǹ����� ��뿡 ���� ���� ��� ����)
		//if(sAlias.Compare(_T("����"))==0 )
		if(sAlias.Compare(_T("����"))==0 || sAlias.Compare(_T("����2"))==0 )
 		{
			asColumnName.Add(_T("��"));
 			asColumnName.Add(_T("��"));
 			asColumnName.Add(_T("�˼�����"));
			
 		}
		
		if( asColumnName.GetSize() != 0 ) return 0;

		if( sExpColumnName.IsEmpty() ) return 0;
	
		// ������� �׳� ����.
		ids = CLocCommonAPI::MakeArrayList( sExpColumnName , _T(",") , asColumnName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );
	}
	// ���̺��� �����Ѵٸ�
	else
	{
		// 2012.11.20 UPDATE BY PWR : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (�ӵ�����)
		CString sOption,sTableName,sCodeField,sDescField;
		ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") , sAlias , _T("WHERE��") , sOption );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );
		ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") , sAlias , _T("���̺��") , sTableName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );
		
		// 18.09.27 JMJ KOL.RED.2018.008
		m_pExpColumnDM->TBL_NAME = sTableName;

		ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") , sAlias , _T("�����ڵ�") , sCodeField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );
		ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") , sAlias , _T("��������") , sDescField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );
		if(_T("CD_CODE_TBL") == sTableName)
		{ // �ڵ尪�� �̹� ������ ������ Ȱ��
			// 13/11/18 �ڴ�� : 2012�⵵ ��ɰ�����ġ �����۾�(20130604005)
			//                   ���������������� ���� ���񽺷� ���������� ����
/*//BEFORE-----------------------------------------------------------------------
			CString strValue;
			CString strClass = _T("");
			bool bIsClass = FALSE;
			int nFirstIdx = sOption.Find(_T("="));
			if(0 < nFirstIdx)
			{
				int nLastIdx = sOption.Find(_T("ORDER"));
				if(0 < nLastIdx && nFirstIdx < nLastIdx)
				{
					strClass = sOption.Mid(nFirstIdx+1, nLastIdx-nFirstIdx-1);
					strClass.TrimLeft(); strClass.TrimRight();
				}
			}
			if(strClass.IsEmpty()) return 0;
			for(INT i = 0; i < m_dmCodeMgr.GetRowCount(); i++)
			{
				m_dmCodeMgr.GetCellData(_T("CLASS"), i, strValue);
				if(strValue == strClass)
				{
					bIsClass = TRUE;
					if(2 == nMode)
					{ // CODE
						m_dmCodeMgr.GetCellData(_T("CODE"), i, strValue);
					}
					else
					{ // DESC
						m_dmCodeMgr.GetCellData(_T("DESCRIPTION"), i, strValue);
					}
					asColumnName.Add(strValue);
				}
				else
				{ // ORDER BY CLASS, CODE �̹Ƿ� �ش� CLASS���ٰ� �ٲ�� ����
					if(TRUE == bIsClass)
					{
						break;
					}
				}
			}
*///AFTER------------------------------------------------------------------------
			CString strClass;
			int nFind1 = sOption.Find(_T("="));
			if(0 < nFind1)
			{
				int nFind2 = sOption.Find(_T("ORDER"));
				if (0 < nFind2 && nFind1 < nFind2)
				{
					strClass = sOption.Mid(nFind1+1, nFind2-nFind1-1);
					strClass.TrimLeft(); strClass.TrimRight();
				}
			}

			if (strClass.IsEmpty()) 
			{
				return 0;
			}

			CString strValueType = (nMode == 2) ? _T("CODE") : _T("DESCRIPTION");
			CString strValue;

			int nCount = m_dmCodeMgr.GetRowCount();
			int nIndex = nCount;
			while (--nIndex >= 0)
			{
				m_dmCodeMgr.GetCellData(_T("CLASS"), nIndex, strValue);
				if (strClass.CompareNoCase(strValue) == 0)
				{
					m_dmCodeMgr.GetCellData(strValueType, nIndex, strValue);
					strValue.TrimLeft();strValue.TrimRight();
					
					if (strValue.Compare(_T("�������")) == 0 || (strValue.IsEmpty()))
					{
						asColumnName.Add(strValue); // �������� �߰��ǵ���
					}
					else
					{
						// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
						//asColumnName.InsertAt(0, strValue);	// �������� �Էµǵ���					
						if (nMode == 9)
						{
							CString strCode;
							m_dmCodeMgr.GetCellData(_T("CODE"), nIndex, strCode);

							CString strDispValue;
							strDispValue.Format(_T("%s(%s)"), strValue, strCode);

							asColumnName.InsertAt(0, strDispValue);	// �������� �Էµǵ���					
						}
						else
						{
						asColumnName.InsertAt(0, strValue);	// �������� �Էµǵ���					
					}
						// KOL.RED.2018.210 ---------------------------------------------------------------------------						
					}
				}
				else
				{
					if (asColumnName.GetSize())
						break; // �ѹ��̶� �Է��� �Ǿ��ٸ�
				}
			}
/*//END------------------------------------------------------------------------*/
		}
		else
		{
			m_pExpColumnDM->TBL_NAME = sTableName;
			m_pExpColumnDM->SetDataMgrRef( 0 , _T("�ڵ��") , sCodeField , _T("str") , _T("") ,  _T("") );
			m_pExpColumnDM->SetDataMgrRef( 1 , _T("�÷���") , sDescField , _T("str") , _T("") ,  _T("") );
			ids = m_pExpColumnDM->RefreshDataManager(sOption);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetColumnNames") );

			CString sColumn;
			CString sColumnAlias;
			CString sColumnName;
			CString sEmptyColumn;
			BOOL IsEmptyColumn = FALSE;

			if ( sAlias==_T("�з���ȣ��(DDC)") && m_sIs64lib == _T("Y") )
			{
				sColumnAlias = _T("�÷���");
				m_pExpColumnDM->SetCellData(sColumnAlias, _T("3\n355-359"), 4 );
			}

			for( INT i = 0 ; i < m_pExpColumnDM->GetRowCount() ; i++ )
			{
				if( nMode != 2 ) sColumnAlias = _T("�÷���");
				else sColumnAlias = _T("�ڵ��");
				
				ids = m_pExpColumnDM->GetCellData( sColumnAlias , i , sColumn );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetColumnNames") );

				ids = m_pExpColumnDM->GetCellData( _T("�÷���") , i , sColumnName );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetColumnNames") );

				sColumnName.TrimLeft(); sColumnName.TrimRight();
				if( sColumnName.Compare(_T("�������")) == 0 || (sColumnName.IsEmpty()) )
				{
					IsEmptyColumn = TRUE;
					sEmptyColumn = sColumn;
					continue;
				}

				asColumnName.Add( sColumn );
			}

			if( IsEmptyColumn )
				asColumnName.Add( sEmptyColumn );
		}
	}

	return 0;

EFS_END
	return -1;
}


INT CStaticsInfoMgr::GetCellData( CESL_DataMgr *pDM , CString sConditionAlias , CString sConditionData , CString sGetAlias , CString &sGetData )
{
	EFS_BEGIN

	INT ids;

	CString sTmpData;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( sConditionAlias , i , sTmpData );
		if( ids < 0 ) continue;
		/*
		if( ids < 0 )
		{
			CString sMsg;
			sMsg.Format( _T("ALIAS : %s , Data : %s ") , sConditionAlias , sTmpData );
			AfxMessageBox( sMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetCellData") );
		*/
		if( sTmpData.Compare( sConditionData ) == 0 )
		{
			ids = pDM->GetCellData( sGetAlias , i , sGetData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetCellData") );

			return 0;
		}
	}

	return 1;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::SetXYAlias( CString sStaticsClass , CString sXAlias , CString sYAlias , CString sOption , CString sOrder )
{
	EFS_BEGIN

	INT ids;
	

	// 14/08/20 �ڴ�� : ���� �߰�
/*// BEFORE ----------------------------------------------------------------------------------
	// 2012.11.20 ADD BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (����������ȣ �Ǹ����� ��뿡 ���� ���� ��� ����)
	// ����������ȣ ��� �� �÷��˸��ƽ������� ���� ��� ����2�� ���(GPIN_SEX)�Ͽ� ���� ��踦 ����
	CString strQuery,strTemp,strCertify;
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����������ȣ' AND VALUE_NAME='�Ǹ�������뿩��' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_pStaticsMainInfoDM->GetOneValueQuery(strQuery,strTemp);
	if(_T("Y") == strTemp) strCertify.Format(_T("�Ǹ��������"));

	if(strCertify.Compare(_T("�Ǹ��������"))==0 && sXAlias.Compare(_T("����"))==0)
	{
		sXAlias.Format(_T("����2"));
	}
*/// AFTER -----------------------------------------------------------------------------------
	// �ֹε�Ϲ�ȣ ��������� ���� �����ϵ��� ��� ����
	if (sXAlias.Compare(_T("����")) == 0)
	{
		CString strQuery;
		CString strValue;
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='�̿��ڰ���' AND VALUE_NAME='�ֹε�Ϲ�ȣ�������' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_pStaticsMainInfoDM->GetOneValueQuery(strQuery, strValue);

		if (strValue.CompareNoCase(_T("Y")) != 0)
		{
			sXAlias.Format(_T("����2"));
		}
	}
//*/ END -------------------------------------------------------------------------------------
	
	///////////////////////////////////////////////////////////////////////////////////////////////


 	ids = SetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("X�༱�þ˸��ƽ�") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetXYAlias") );

	ids = SetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("Y�༱�þ˸��ƽ�") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SetXYAlias") );

	ids = SetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����") , sOption );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetXYAlias") );

	ids = SetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("��������") , sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetXYAlias") );

	return 0;

	EFS_END
	return -1;

}

// 
INT CStaticsInfoMgr::SetCellData( CESL_DataMgr *pDM , CString sCAlias , CString sCData , CString sIAlias , CString &sIData )
{
EFS_BEGIN

	INT ids;
	
	CString sTmpData;
	for ( INT i = 0;i < pDM->GetRowCount();i++ )
	{
		ids = pDM->GetCellData( sCAlias , i , sTmpData );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::SetCellData()") );

		if ( sTmpData.Compare( sCData ) != 0 ) continue;

		ids = pDM->SetCellData( sIAlias , sIData , i );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::SetCellData()") );

		return 0;
	}

	return 1;

EFS_END
	return -1;
}

INT CStaticsInfoMgr::GetXYAlias( CString sStaticsClass , CString &sXAlias , CString &sYAlias , CString &sOption , CString &sOrder )
{
	EFS_BEGIN

	INT ids;

	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("X�༱�þ˸��ƽ�") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetXYAlias") );

	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("Y�༱�þ˸��ƽ�") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetXYAlias") );

	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����") , sOption );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetXYAlias") );

	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("��������") , sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetXYAlias") );

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::GetUserSearchCondFieldName( CString sAlias , CString &sFieldName )
{
	EFS_BEGIN

	INT ids;

	ids = GetCellData( m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sAlias , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetUserSearchCondFieldName") );

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::SetUserCondition( CString sStaticsClass , CString sWhere )
{
	EFS_BEGIN

	INT ids = SetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����ڰ˻�����") , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetXYAlias") );

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::GetUserCondition( CString sStaticsClass , CString &sWhere )
{
	EFS_BEGIN

	INT ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����ڰ˻�����") , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetXYAlias") );

	return 0;

	EFS_END
	return -1;

}

// ##############################################################################################################
// [ �Լ����� ]
//   ���� ����
//   ��¥ ��� DM�� �����ϴµ� �ַ��Ѵ�.
//
// [ �̴Ͻ��� ]
// 
// ##############################################################################################################
INT CStaticsInfoMgr::MakeStaticsCondition( CString sStaticsClass )
{
EFS_BEGIN

	INT ids;

	CString sUserCondition;
	CString sXAlias;
	CString sYAlias;

	// ==========================================================================================================
	// 1. ����� �˻� ������ �����Ǿ� �ִ��� ����
	//    �弭 ��������� ����
	// ==========================================================================================================	
	if( GetMenuName().Compare( BOOK_STATICS ) != 0 )
	{		
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����ڰ˻�����") , sUserCondition );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeStaticsCondition()") );

		if ( sUserCondition.IsEmpty() )
		{
			AfxMessageBox( _T("���� ������ ���� �Ͻð� ��踦 �����Ͻʽÿ�.") );
			return 101;
		}
	}

	// ==========================================================================================================	
	// 2. ��, ���� �����Ǿ� �ִ��� ����
	// ==========================================================================================================	
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("X�༱�þ˸��ƽ�") , sXAlias );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeStaticsCondition()") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("Y�༱�þ˸��ƽ�") , sYAlias );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::MakeStaticsCondition()") );

	if( sXAlias.IsEmpty() || sYAlias.IsEmpty() )
	{
		AfxMessageBox( _T("��, �� ������ ���� �Ͻð� ��踦 �����Ͻʽÿ�.") );
		return 102;
	}

	// ==========================================================================================================
	// 3. ��� ��� DM ����
	// ==========================================================================================================
	ids = MakeRealDM( sStaticsClass , sXAlias , sYAlias );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::MakeStaticsCondition()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ���� ��� DM ����
//   DM�� ���¸� �����.
// 
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeRealDM( CString sStaticsClass , CString sXAlias , CString sYAlias )
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. DM�ʱ�ȭ
	// ==========================================================================================================
	m_pRealResultDM->FreeData();
	m_pRealResultDM->FreeDMRef();
	
	// ==========================================================================================================
	// 2. ���� �÷� �ڵ尪 ��������
	// ==========================================================================================================
	INT nCanUseXColumnName , nCanUseYColumnName;
	CStringArray saXColumnName;
	CStringArray saYColumnName;

	nCanUseXColumnName = GetColumnNames( sStaticsClass , sXAlias , saXColumnName , 2 );
	if( nCanUseXColumnName < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeRealDM()") );
	
	nCanUseYColumnName = GetColumnNames( sStaticsClass , sYAlias , saYColumnName , 2 );
	if( nCanUseYColumnName < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeRealDM()") );	

	// ==========================================================================================================
	// 3. DM �����ϱ�
	// ==========================================================================================================
	INT nColumnCnt;
	nColumnCnt = saXColumnName.GetSize()+1;
	m_pRealResultDM->InitDataMgrRef(nColumnCnt);

	ids = m_pRealResultDM->SetDataMgrRef( 0 , _T("�÷���") , _T(""),_T("") ,_T(""),_T("") );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::MakeRealDM()") );

	// �÷��� ����
	for ( INT i = 0;i < saXColumnName.GetSize();i++ )
	{
		ids = m_pRealResultDM->SetDataMgrRef( i + 1, saXColumnName.GetAt(i), _T(""), _T("str"), _T("0"), _T("udf") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::MakeRealDM()") );
	}

	m_pRealResultDM->m_nCols = nColumnCnt;

	// ����ο� ���� ��ŭ Insert�ϰ� �ʱⰪ�� �����Ѵ�.
	for ( i = 0;i < saYColumnName.GetSize();i++ )
	{
		m_pRealResultDM->InsertRow(-1);
		ids = m_pRealResultDM->SetCellData( _T("�÷���") ,  saYColumnName.GetAt(i) , m_pRealResultDM->GetRowCount()-1 );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::MakeRealDM()") );

		for ( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			// ????-??-?? UPDATE BY ??? +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ���� ����з���ȣ�� ����ó��
			// ���� �۾��� �־�� ������.......
			CString sNotValue;
			if ( m_sIs64lib == _T("Y") && sXAlias == _T("�з���ȣ��(DDC)") && j == 4 ) 
				sNotValue.Format( _T("0 %s 0"), DIVCHAR );
			else
				sNotValue.Format( _T("0") );
			// --------------------------------------------------------------------------------------------------
			ids = m_pRealResultDM->SetCellData( saXColumnName.GetAt(j-1) , sNotValue , m_pRealResultDM->GetRowCount() - 1 );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::MakeRealDM()") );
		}
	}

	return 0;

EFS_END
	return -1;
}


// ��� �� ��������
INT CStaticsInfoMgr::RunStatics( CString sStaticsClass , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;

	// ��踦 ���� ���� �⺻���� ��������
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
	INT nPos = 1;
		
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM ���̺� �̸� �߰�
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);

		
		// �⺻ �˻� ���ǿ� ����ڰ˻�����,�׷������� ���Խ�Ų��.
		CString sWhere;

		CString sGroupSql , sStaticWhere , sSendWhere;
		if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			CString sXData , sYData , sTmp;
			// ���α׷����� �˻�
			pTotalProgress->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()+1)*(m_pRealResultDM->GetRowCount()+1) );
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
				for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
				{
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
				
					ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,sYData ,sXGroupYN,sYGroupYN , i , sWhere );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

					ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

					pTotalProgress->SetPos(nPos); nPos++;
				}
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sYData;
			for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),sYData , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

				ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

				pTotalProgress->SetPos( k+1 );
			}

		}
		else if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sXData , sTmp;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

				ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

				pTotalProgress->SetPos( j+1 );
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , 1 );

			ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

			ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
	
			pTotalProgress->SetPos(1);
		}
		
	}

	// �հ� ����
	ids = MakeTotalCount( sStaticsClass , saTableName , sXFieldName,sYFieldName , pProgressCtrl , pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );


	// �ڵ尪�� �������� �ٲ۴�.
	ids = CodeToDesc(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );


	return 0;

	EFS_END
	return -1;

}


// ��� �� ��������
INT CStaticsInfoMgr::RunStatics( CString sStaticsClass , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if( m_pAxisCode_CellClick != NULL )
		m_pAxisCode_CellClick->Clear();
	// KOL.RED.2018.211 ---------------------------------------------------------------------------

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;
	CStringArray saXFieldName,saYFieldName;
	CString sXGroupYN , sYGroupYN;

	// ��踦 ���� ���� �⺻���� ��������
	ids = GetRunStaticsCond(sStaticsClass,saTableName,sXAlias,sYAlias,saXFieldName,saYFieldName,sXGroupYN,sYGroupYN);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
	INT nPos = 1;
	
	CString sRegNoList;
	CString sRegNo;
	bool IsSend;

	//=====================================================
	//2008.11.24 UPDATE BY PJW : ��Ϲ�ȣ���� ������ 1000�� ���Ϸ� �ٲ۴�.
	// 	INT nLimitCnt = 50;
    INT nLimitCnt = 999;
	//=====================================================
	
	for( INT n = 0 ; n < pRegNoDM->GetRowCount() ; n++ )
	{
		IsSend = FALSE;

		ids = pRegNoDM->GetCellData( _T("��Ϲ�ȣ") , n , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

		if( !sRegNoList.IsEmpty() )
			if( !sRegNo.IsEmpty() )
				sRegNoList += _T(",");
		if( !sRegNo.IsEmpty() )
			sRegNoList += _T("'") + sRegNo + _T("'");

		if( (n!=0) && (n%nLimitCnt==0) ) IsSend = TRUE;
		if( n == pRegNoDM->GetRowCount()-1 ) IsSend = TRUE;

		if( !IsSend ) continue;

		if( sRegNoList.IsEmpty() ) continue;

		for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
		{
			// DM �ʱ���
			ids = InitStaticsDM( sStaticsClass ,saTableName.GetAt(i) , sXAlias , sYAlias , saXFieldName.GetAt(i) , saYFieldName.GetAt(i) , sXGroupYN , sXGroupYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

			// DM ���̺� �̸� �߰�
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);

			
			// �⺻ �˻� ���ǿ� ����ڰ˻�����,�׷������� ���Խ�Ų��.
			CString sWhere,sSendWhere;
			if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
			{
				CString sXData , sYData , sTmp;
				// ���α׷����� �˻�
				pTotalProgress->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()+1)*(m_pRealResultDM->GetRowCount()+1) );
				for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
				{
					ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
					if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
					for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
					{
						ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

						ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , saYFieldName.GetAt(i) , sXData,sYData , sXGroupYN,sYGroupYN ,i , sWhere );
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

						sSendWhere = _T(" B.REG_NO IN (")+ sRegNoList +_T(") AND ") + sWhere;

						ids = FillStaticsValue( saXFieldName.GetAt(i),saYFieldName.GetAt(i),sXData,sYData,sXGroupYN,sYGroupYN,sSendWhere,pProgressCtrl);
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

						pTotalProgress->SetPos(nPos); nPos++;
					}
				}	
			}
			else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
			{
				pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

				CString sYData;
				for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
				{
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

					ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , saYFieldName.GetAt(i) , _T(""),sYData , sXGroupYN,sYGroupYN ,i , sWhere );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

					sSendWhere = _T(" B.REG_NO IN (")+ sRegNoList +_T(") AND ") + sWhere;

					ids = FillStaticsValue( saXFieldName.GetAt(i),saYFieldName.GetAt(i),_T(""),sYData,sXGroupYN,sYGroupYN,sSendWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

					pTotalProgress->SetPos( k+1 );
				}

			}
			else if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
			{
				pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

				CString sXData , sTmp;
				for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
				{
					ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
					if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

					ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , saYFieldName.GetAt(i) , sXData,_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

					sSendWhere = _T(" B.REG_NO IN (")+ sRegNoList +_T(") AND ") + sWhere;

					ids = FillStaticsValue( saXFieldName.GetAt(i),saYFieldName.GetAt(i),sXData,_T(""),sXGroupYN,sYGroupYN,sSendWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

					pTotalProgress->SetPos( j+1 );
				}	
			}
			else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
			{
				pTotalProgress->SetRange32( 0 , 1 );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , saYFieldName.GetAt(i) , _T(""),_T("") ,sXGroupYN,sYGroupYN , i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

				sSendWhere = _T(" B.REG_NO IN (")+ sRegNoList +_T(") AND ") + sWhere;

				ids = FillStaticsValue( saXFieldName.GetAt(i),saYFieldName.GetAt(i),_T(""),_T(""),sXGroupYN,sYGroupYN,sSendWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
		
				pTotalProgress->SetPos(1);
			}
			
		}
		sRegNoList.Empty();
	}

	// �հ� ����
	ids = MakeTotalCount3( sStaticsClass , saTableName , saXFieldName,saYFieldName , pProgressCtrl , pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

	// ���� ����
	ids = MakePriceSum3( sStaticsClass , saTableName , saXFieldName,saYFieldName , pProgressCtrl , pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

	// �ڵ尪�� �������� �ٲ۴�.
	ids = CodeToDesc(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );


	return 0;

	EFS_END
	return -1;

}

// ���� / �ݳ� ��� 
// �ʹ� �̻��� �Ѥ�;;
// �Ϻ��� �踦 ���� �ջ��ϴ� ������� �ߺ��� å�̳� �̿��ڸ� �����ϴ� ������� �ϰ� �ִ�.
// 
// ��� �� ��������
INT CStaticsInfoMgr::RunLoanStatics( CString sStaticsClass , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;
	CString sTableName;

	// ��踦 ���� ���� �⺻���� �������� ���� �����ϴ°���
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
	INT nPos = 1;

	CString sResultField;
	if( sXAlias.Compare(_T("��")) == 0 || (sXAlias.Compare(_T("��")) == 0) || (sXAlias.Compare(_T("��")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultField );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����2") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	// ����� �Ϻ��� ��踦 �����ϴ°��� �����ϴ��� �˻�
	CString sDayField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�Ϻ�") , sDayField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	sDayField.TrimLeft(); sDayField.TrimRight();

	if( !sDayField.IsEmpty() )
	{
		ids = GetCellData( m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sDayField , _T("FieldName") , sDayField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunLoanStatics") );
	}
	
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM ���̺� �̸� �߰�
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);

		sTableName = saTableName.GetAt(i);

		
		// �⺻ �˻� ���ǿ� ����ڰ˻�����,�׷������� ���Խ�Ų��.
		CString sWhere;
		
		CString sTableSql;
		CString sSelectFields;
		CString sGroupByFields;


		CString sGroupSql , sStaticWhere , sSendWhere;
		if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			CString sXData , sYData , sTmp;
			// ���α׷����� �˻�
			pTotalProgress->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()+1)*(m_pRealResultDM->GetRowCount()+1) );
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
				for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
				{
							
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

					ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,sYData ,sXGroupYN,sYGroupYN , i , sWhere );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
					
					if( !sDayField.IsEmpty() )
					{
						CString sTmpXFieldName , sTmpYFieldName;
						sSelectFields.Empty();
						sGroupByFields.Empty();
						sTableSql.Empty();
						
						if( sXFieldName.Find(_T(".")) > - 1 )
							sTmpXFieldName = sXFieldName.Mid(sXFieldName.Find(_T("."))+1);
						if( sYFieldName.Find(_T(".")) > - 1 )
							sTmpYFieldName = sYFieldName.Mid(sYFieldName.Find(_T("."))+1);

						sSelectFields.Format( _T("%s AS CNT") , sResultField );
						sGroupByFields.Format( _T("%s") ,  sDayField );
						
						sTableSql.Format( _T("( SELECT %s FROM %s WHERE %s GROUP BY %s ) V") , sSelectFields , sTableName , sWhere , sGroupByFields );

						m_pResultDM->TBL_NAME = sTableSql;
						ids = FillStaticsValue( sTmpXFieldName,sTmpYFieldName,sXData,sYData,sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
					}
					else
					{
						ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
					}

					pTotalProgress->SetPos(nPos); nPos++;
				}
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sYData;
			for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),sYData , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

				if( !sDayField.IsEmpty() )
				{
					CString sTmpXFieldName , sTmpYFieldName;
					sSelectFields.Empty();
					sGroupByFields.Empty();
					sTableSql.Empty();
					
					if( sXFieldName.Find(_T(".")) > - 1 )
						sTmpXFieldName = sXFieldName.Mid(sXFieldName.Find(_T("."))+1);
					
					sSelectFields.Format( _T("%s as XFIELD,%s AS CNT") , sXFieldName , sResultField );
					sGroupByFields.Format( _T("%s,%s") , sXFieldName , sDayField );
					
					sTableSql.Format( _T("( SELECT %s FROM %s WHERE %s GROUP BY %s ) V") , sSelectFields , sTableName , sWhere , sGroupByFields );
					
					m_pResultDM->TBL_NAME = sTableSql;
//					ids = FillStaticsValue( sTmpXFieldName,sTmpYFieldName,_T(""),sYData,sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
					ids = FillStaticsValue( _T("XFIELD"),sTmpYFieldName,_T(""),sYData,sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
				}
				else
				{
					ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
				}
				

				pTotalProgress->SetPos( k+1 );
			}

		}
		else if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sXData , sTmp;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

				if( !sDayField.IsEmpty() )
				{
					CString sTmpXFieldName , sTmpYFieldName;
					sSelectFields.Empty();
					sGroupByFields.Empty();
					sTableSql.Empty();
					
					if( sYFieldName.Find(_T(".")) > - 1 )
						sTmpYFieldName = sYFieldName.Mid(sYFieldName.Find(_T("."))+1);
					
					sSelectFields.Format( _T("%s as YFIELD,%s AS CNT") , sYFieldName , sResultField );
					sGroupByFields.Format( _T("%s,%s") , sYFieldName , sDayField );
					
					sTableSql.Format( _T("( SELECT %s FROM %s WHERE %s GROUP BY %s ) V") , sSelectFields , sTableName , sWhere , sGroupByFields );
					
					m_pResultDM->TBL_NAME = sTableSql;
					ids = FillStaticsValue( sTmpXFieldName,_T("YFIELD"),sXData,_T(""),sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
//					ids = FillStaticsValue( sTmpXFieldName,sTmpYFieldName,sXData,_T(""),sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
				}
				else
				{
					ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
				}

				pTotalProgress->SetPos( j+1 );
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , 1 );

			ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

			if( !sDayField.IsEmpty() )
			{
				CString sTmpXFieldName , sTmpYFieldName;
				sSelectFields.Empty();
				sGroupByFields.Empty();
				sTableSql.Empty();
				
				if( sXFieldName.Find(_T(".")) > - 1 )
					sTmpXFieldName = sXFieldName.Mid(sXFieldName.Find(_T("."))+1);
				if( sYFieldName.Find(_T(".")) > - 1 )
					sTmpYFieldName = sYFieldName.Mid(sYFieldName.Find(_T("."))+1);
				
				sSelectFields.Format( _T("%s as XFIELD ,%s as YFIELD,%s AS CNT") , sXFieldName , sYFieldName , sResultField );
				sGroupByFields.Format( _T("%s,%s,%s") , sXFieldName , sYFieldName , sDayField );
				
				sTableSql.Format( _T("( SELECT %s FROM %s WHERE %s GROUP BY %s ) V") , sSelectFields , sTableName , sWhere , sGroupByFields );
				
				m_pResultDM->TBL_NAME = sTableSql;
//				ids = FillStaticsValue( sTmpXFieldName,sTmpYFieldName,_T(""),_T(""),sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
				ids = FillStaticsValue( _T("XFIELD"),_T("YFIELD"),_T(""),_T(""),sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
			}
			else
			{
				ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
			}
			
			pTotalProgress->SetPos(1);
		}
		
	}

	// �հ� ����
	ids = MakeTotalCount( sStaticsClass , saTableName , sXFieldName,sYFieldName , sXAlias , sXGroupYN , sYGroupYN ,  pProgressCtrl , pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );


	// �ڵ尪�� �������� �ٲ۴�.
	ids = CodeToDesc(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );


	return 0;

	EFS_END
	return -1;

}

// 2009.01.05 ADD BY PWR 
// ** å�η����
INT CStaticsInfoMgr::RunCooperativeStatics( CString sStaticsClass , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
EFS_BEGIN

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;

	// ��踦 ���� ���� �⺻���� ��������
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunCooperativeStatics") );
	INT nPos = 1;
		
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM ���̺� �̸� �߰�
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);

		// �⺻ �˻� ���ǿ� ����ڰ˻�����,�׷������� ���Խ�Ų��.
		CString sWhere;

		CString sGroupSql , sStaticWhere , sSendWhere;
		if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			CString sXData , sYData , sTmp;
			// ���α׷����� �˻�
			pTotalProgress->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()+1)*(m_pRealResultDM->GetRowCount()+1) );
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunCooperativeStatics") );
				for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
				{
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );

					ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,sYData ,sXGroupYN,sYGroupYN , i , sWhere );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunCooperativeStatics") );

					ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );

					pTotalProgress->SetPos(nPos); nPos++;
				}
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sYData;
			for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),sYData , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunCooperativeStatics") );

				ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );

				pTotalProgress->SetPos( k+1 );
			}

		}
		else if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sXData , sTmp;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunCooperativeStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunCooperativeStatics") );

				ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );

				pTotalProgress->SetPos( j+1 );
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , 1 );

			ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunCooperativeStatics") );

			ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );
	
			pTotalProgress->SetPos(1);
		}
	}

	// �հ� ����
	ids = MakeTotalCount( sStaticsClass , saTableName , sXFieldName,sYFieldName , pProgressCtrl , pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunCooperativeStatics") );

	// �ڵ尪�� �������� �ٲ۴�.
	ids = CodeToDesc(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunCooperativeStatics") );

	return 0;

EFS_END
return -1;
}


//###############################################################################################################
// [ �Լ����� ]
//   DB���� ���� �������� ��� DM�� ����Ѵ�.
//
// [ �̴Ͻ��� ]
//
//###############################################################################################################
INT CStaticsInfoMgr::GetDBAndMakeRealResultDM( CString sSendWhere , CProgressCtrl *pProgress )
{
EFS_BEGIN

	CString strNormal, strArmy;
	INT ids;

	ids = m_pResultDM->RefreshDataManager( sSendWhere , pProgress );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );

	INT nXIndex , nYIndex;
	CString sXAlias , sYAlias , sResult , sPreResult;

	for ( INT i = 0;i < m_pResultDM->GetRowCount();i++ )
	{
		ids = m_pResultDM->GetCellData( _T("X��˸��ƽ�") , i , sXAlias );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );

		ids = m_pResultDM->GetCellData( _T("Y��˸��ƽ�") , i , sYAlias );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );

		ids = m_pResultDM->GetCellData( _T("�����") , i , sResult );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );

		// �ε��� ã��
		ids = GetDMIndex( m_pRealResultDM , sXAlias,sYAlias,nXIndex,nYIndex);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );

		// �� �Է�
		//sResult.Format( _T("%d") , _ttoi(sPreResult) + _ttoi(sResult) );
		// 2005.07.28 ADD BY PDJ
		// ����з���ȣ ó��
		// ? ( ? ) ���Ŀ� ���� ����
		if ( m_sIs64lib == _T("Y") && (sXAlias==_T("355-9") || sXAlias==_T("3") )) 
		{		
			CString strNormalResult;

			if ( sXAlias==_T("3") )
			{
				m_pRealResultDM->GetCellData(_T("3"),nYIndex, strNormalResult);
				DividNormalArmy(strNormalResult, strNormal, strArmy);
				sResult.Format( _T("%d %s %d") , _ttoi(sResult), DIVCHAR, _ttoi(strArmy) );
			}
			else
			{
				m_pRealResultDM->GetCellData(_T("3"),nYIndex, strNormalResult);
				DividNormalArmy(strNormalResult, strNormal, strArmy);
				sResult.Format( _T("%d %s %d") , _ttoi(strNormal), DIVCHAR, _ttoi(sResult) );
			}				
			
			ids = m_pRealResultDM->SetCellData( _T("3"), sResult , nYIndex);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );
		}
		else
		{	
			sPreResult = m_pRealResultDM->GetCellData(sXAlias,nYIndex);				
			sResult.Format( _T("%d") , _ttoi(sPreResult) + _ttoi(sResult) );
			ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );		
		}
	}

	return 0;

EFS_END
	return -1;
}


INT CStaticsInfoMgr::GetDMIndex( CESL_DataMgr *pDM , CString sXAlias , CString sYAlias , INT &nXIndex , INT &nYIndex )
{
	EFS_BEGIN

	INT ids;

	nXIndex = -1;
	nYIndex = -1;

	CString sTmpYAlias;

	// X�� �˻�
	CString sGroupYN;
	INT refcount = pDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = pDM->RefList.GetHeadPosition();
	CString query, fields, isudf;
	
	// 2005.07.27 ADD BY PDJ
	// ������ ��� _T("355-9") �� ������ �׸��� �з�_T("3")���� ���� _T("�Ϲ�(����)") ������
	if ( m_sIs64lib == _T("Y") && sXAlias==_T("355-9")) 
	{
		sXAlias = _T("3");				
	}

	for( INT i = 0 ; i < pDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)pDM->RefList.GetNext(pos);		

		if( pRef->FIELD_ALIAS.Compare(sXAlias) == 0 )
		{
			nXIndex = i;
			break;
		}
	}


	if( nXIndex == -1 )
	{
		pRef = new CESL_DataMgr::reference;
		pRef->DATA_TYPE = _T(""); pRef->FIELD_NAME = _T(""); pRef->INIT_VALUE = _T("");
		pRef->FIELD_ALIAS = sXAlias;

		pDM->RefList.AddTail((CObject*)pRef);

		INT nColCnt = pDM->RefList.GetCount();

		pDM->m_nCols = nColCnt;

		ids = PostInsertDMColumn( pDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetDMIndex") );

		nXIndex = pDM->RefList.GetCount() - 1;
	}

	// Y�� �˻�
	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( i , 0 , sTmpYAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetDMIndex") );

		if( sTmpYAlias.Compare( sYAlias ) == 0 )
			nYIndex = i;
	}

	if( nYIndex == -1 )
	{
		pDM->InsertRow(-1);
		pDM->SetCellData( pDM->GetRowCount()-1 , 0 , sYAlias );

		nYIndex = pDM->GetRowCount() - 1;
	}

	return 0;

	EFS_END
	return -1;

}


// ��迡 ���� �⺻ ���� ����
// 1. TableName
// 2. X�� Y�� �˸��ƽ�
// 3. X�� �ʵ� , Y�� �ʵ�
// 4. Group ����
INT CStaticsInfoMgr::GetRunStaticsCond( CString sStaticsClass , CStringArray &saTableName , CString &sXAlias , CString &sYAlias ,CStringArray &saXFieldName,CStringArray &saYFieldName,CString &sXGroupYN,CString &sYGroupYN)
{
	EFS_BEGIN
		
	INT ids;
	
	// 1. TABLE NAME ����
	CString sTableName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�˻����̺��̸�") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitRunStatics") );
	
	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );
	
	
	// DM �÷���(�ʵ��) �߰�
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("X�༱�þ˸��ƽ�") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("Y�༱�þ˸��ƽ�") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	
	CString sResultField;
	CString sXFieldName , sYFieldName;
	ids = GetFieldName( sStaticsClass , sXAlias , sXFieldName , sXGroupYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	ids = GetFieldName( sStaticsClass , sYAlias , sYFieldName , sYGroupYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	ids = CLocCommonAPI::MakeArrayList( sXFieldName , _T("/") , saXFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );
	ids = CLocCommonAPI::MakeArrayList( sYFieldName , _T("/") , saYFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );
	
	

	// �հ� DM �ʱ�ȭ
	m_pXSumDM->FreeData();
	m_pYSumDM->FreeData();
	
	return 0;
	
	EFS_END
		return -1;
	
}

// ��迡 ���� DM �ʱ�ȭ
INT CStaticsInfoMgr::InitStaticsDM( CString sStaticsClass , CString sTableName , CString sXAlias , CString sYAlias ,CString sXFieldName,CString sYFieldName,CString sXGroupYN,CString sYGroupYN) 
{
	EFS_BEGIN

	INT ids;

	CString sResultField;
	
	if( sXAlias.Compare(_T("��")) == 0 || (sXAlias.Compare(_T("��")) == 0) || (sXAlias.Compare(_T("��")) == 0) )
	{
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	}
	else
	{
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����2") , sResultField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );		
	}

	// �ʵ�� ����
	INT refcount = m_pResultDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = m_pResultDM->RefList.GetHeadPosition();
	CString query, fields, isudf;
	for( INT i = 0 ; i < m_pResultDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)m_pResultDM->RefList.GetNext(pos);
		
		if( pRef->FIELD_ALIAS.Compare(_T("X��˸��ƽ�")) == 0 )
		{
			pRef->FIELD_NAME = sXFieldName;
			pRef->FIELD_TYPE = _T("");
		}
		else if( pRef->FIELD_ALIAS.Compare(_T("Y��˸��ƽ�")) == 0 )
		{
			pRef->FIELD_NAME = sYFieldName;
			pRef->FIELD_TYPE = _T("");
		}
		else if( pRef->FIELD_ALIAS.Compare(_T("�����")) == 0 )
		{
			pRef->FIELD_NAME = sResultField;
		}
		
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitRunStatics") );
		
	}

	return 0;

	EFS_END

	return -1;
}

// ��迡 ���� DM �ʱ�ȭ
INT CStaticsInfoMgr::InitRunStatics( CString sStaticsClass , CStringArray &saTableName , CString &sXAlias , CString &sYAlias ,CString &sXFieldName,CString &sYFieldName,CString &sXGroupYN,CString &sYGroupYN
									// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
									, bool	noFreeXYSum
									// KOL.RED.2018.211 ---------------------------------------------------------------------------
									)
{
	EFS_BEGIN

	INT ids;

	// 1. TABLE NAME ����
	CString sTableName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�˻����̺��̸�") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitRunStatics") );

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );


	// DM �÷���(�ʵ��) �߰�
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("X�༱�þ˸��ƽ�") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("Y�༱�þ˸��ƽ�") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	CString sResultField;
	ids = GetFieldName( sStaticsClass , sXAlias , sXFieldName , sXGroupYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	ids = GetFieldName( sStaticsClass , sYAlias , sYFieldName , sYGroupYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	if( sXAlias.Compare(_T("��")) == 0 || (sXAlias.Compare(_T("��")) == 0) || (sXAlias.Compare(_T("��")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultField );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����2") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	if( sResultField.IsEmpty() )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultField );

	CString sDayField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�Ϻ�") , sDayField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	sDayField.TrimLeft(); sDayField.TrimRight();

	if( !sDayField.IsEmpty() )
	{
		sResultField.Format( _T("SUM(CNT)") , sDayField );
	}
	
	
	// �ʵ�� ����
	INT refcount = m_pResultDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = m_pResultDM->RefList.GetHeadPosition();
	CString query, fields, isudf;

	CString sTmpTableName = saTableName.GetAt(0);
	CStringArray saTmpTableName;
	CLocCommonAPI::MakeArrayList( sTmpTableName , _T(","), saTmpTableName );
	sTmpTableName = saTmpTableName.GetAt(0);
	CLocCommonAPI::MakeArrayList( sTmpTableName , _T(" "), saTmpTableName ); 
	sTmpTableName = saTmpTableName.GetAt(1);

	for( INT i = 0 ; i < m_pResultDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)m_pResultDM->RefList.GetNext(pos);

		if( pRef->FIELD_ALIAS.Compare(_T("X��˸��ƽ�")) == 0 )
		{
			if( sXFieldName.Find(_T(".")) < 0 )
			{
				sXFieldName = sTmpTableName + _T(".") + sXFieldName;
			}

			pRef->FIELD_NAME = sXFieldName;
			pRef->FIELD_TYPE = _T("");
		}
		else if( pRef->FIELD_ALIAS.Compare(_T("Y��˸��ƽ�")) == 0 )
		{
			pRef->FIELD_NAME = sYFieldName;
			pRef->FIELD_TYPE = _T("");

			if( sYFieldName.Find(_T(".")) < 0 )
			{
				sYFieldName = sTmpTableName + _T(".") + sYFieldName;
			}
		}
		else if( pRef->FIELD_ALIAS.Compare(_T("�����")) == 0 )
		{
			pRef->FIELD_NAME = sResultField;
		}

		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitRunStatics") );

	}

	//csj
	if( noFreeXYSum == false )
	{
	// �հ� DM �ʱ�ȭ
	m_pXSumDM->FreeData();
	m_pYSumDM->FreeData();
	}

	return 0;

	EFS_END
	return -1;

}


INT CStaticsInfoMgr::CodeToDesc( CString sStaticsClass )
{
	EFS_BEGIN

	INT ids;
	CString sXAlias , sYAlias , sXCode , sYCode;

	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("X�༱�þ˸��ƽ�") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("Y�༱�þ˸��ƽ�") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

	ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") , sXAlias , _T("����") , sXCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
	ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") , sYAlias , _T("����") , sYCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunStatics") );

	CString sCode;
	if( !sXCode.IsEmpty() )
	{
		CString sFieldName , sFieldType , sInitValue , sDataType;
		for( INT i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
		{
			ids = m_pRealResultDM->GetColumnRefInfo( i , sCode , sFieldName , sFieldType , sInitValue , sDataType );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("RunStatics") );

			// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			// ��Ŭ���� ������ ����
			CString		sCode_Temp = sCode;
			// KOL.RED.2018.211 ---------------------------------------------------------------------------

			// �ڵ尡 ȸ�� �����̶��
			if( sXAlias.Compare(_T("ȸ�����к�"))==0 )
				ids = ConvertInferiorCodeToDesc( sCode , sCode );
			// 2006-03-03 UPDATE BY PDW(Jr) ===================================================
			// ����� ��ȸ���� ��� �߰�
			else if( sXAlias.Compare(_T("�з���ȣ��"))==0 || 
				(sXAlias.Compare(_T("KDC��"))==0) || 
				(sXAlias.Compare(_T("DDC��"))==0) || 
				(sXAlias.Compare(_T("LC��"))==0)  ||
				(sXAlias.Compare(_T("�з���ȣ��(DDC)"))==0) ||
				(sXAlias.Compare(_T("�з���ȣ��(��ȸ����)"))==0)
				)
				ids = ConvertClassNo(sStaticsClass,sCode,sCode,sXAlias);
			// ================================================================================
			else if( sXAlias.Compare(_T("�谡���º�")) == 0 )
			{
				ids = CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
			}
			else
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( sXCode , sCode , sCode );
			/*
			if( ids < 0 )
			{
				CString sEMsg;
				sEMsg.Format( _T(" sXAlias : %s , sXCode : %s , sCode : %s ") , sXAlias , sXCode , sCode );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("RunStatics") );
			*/

			ids = m_pRealResultDM->SetDataMgrRef( i , sCode , sFieldName , sFieldType , sInitValue , sDataType );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("RunStatics") );

			// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			// ��Ŭ���� ������ ����
			if( m_pAxisCode_CellClick != NULL )
			{
				m_pAxisCode_CellClick->SetCodeToDesc( 0 , i-1 , sXAlias , sCode_Temp , sCode );
			}
			// KOL.RED.2018.211 ---------------------------------------------------------------------------
		}
	}
	if( !sYCode.IsEmpty() )
	{
		for( INT i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
		{
			ids = m_pRealResultDM->GetCellData( i , 0 , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("RunStatics") );

			// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			// ��Ŭ���� ������ ����
			CString		sCode_Temp = sCode;
			// KOL.RED.2018.211 ---------------------------------------------------------------------------

			CString sTmpCode;
			sTmpCode = sCode;

			if( sYAlias.Compare(_T("ȸ�����к�"))==0 )
				ids = ConvertInferiorCodeToDesc( sCode , sCode );
			// 2006-03-03 UPDATE BY PDW(Jr) ===================================================
			// ����� ��ȸ���� ��� �߰�
			else if( sYAlias.Compare(_T("�з���ȣ��"))==0 ||
				(sYAlias.Compare(_T("KDC��"))==0) || 
				(sYAlias.Compare(_T("DDC��"))==0) || 
				(sYAlias.Compare(_T("LC��"))==0)  ||
				(sYAlias.Compare(_T("�з���ȣ��(DDC)"))==0) ||
				(sYAlias.Compare(_T("�з���ȣ��(DDC)"))==0) )
			{
				ids = ConvertClassNo(sStaticsClass,sCode,sCode,sYAlias);
			}
			// ================================================================================
			else if( sYAlias.Compare(_T("�谡���º�")) == 0 )
			{
				ids = CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
			}
			else if(sYAlias.Compare(_T("������º�")) == 0 )
			{
				if(_T("0001") == sCode)
				{
					sCode = _T("��û���ڷ�");
				}
				else if(_T("0002") == sCode)
				{
					sCode = _T("��û���ڷ�");
				}
				else if(_T("0004") == sCode)
				{
					sCode = _T("�߼۵��ڷ�");
				}
				else if(_T("0014") == sCode)
				{
					sCode = _T("�Լ����ڷ�");
				}
				else if(_T("0008") == sCode)
				{
					sCode = _T("�������ڷ�");
				}
				else if(_T("0009") == sCode)
				{
					sCode = _T("�Ϸ���ڷ�");
				}
				else if(_T("0011") == sCode)
				{
					sCode = _T("�߼۰������ڷ�");
				}
				else if(_T("0012") == sCode)
				{
					sCode = _T("��û�������ڷ�");
				}
				else if(_T("0016") == sCode)
				{
					sCode = _T("��û��ҵ��ڷ�(�̿���)");
				}
			}
			else if(sYAlias.Compare(_T("���������")) == 0 )
			{
				if(_T("0001") == sCode)
				{
					sCode = _T("�̿������");
				}
				else if(_T("0002") == sCode)
				{
					sCode = _T("��û����");
				}
				else if(_T("0003") == sCode)
				{
					sCode = _T("������/�̴���");
				}
				else if(_T("0004") == sCode)
				{
					sCode = _T("�߼۰���(��������Ұ�)");
				}
				else if(_T("0010") == sCode)
				{
					sCode = _T("�Ϸ�");
				}
				else if(_T("0099") == sCode)
				{
					sCode = _T("��Ÿ");
				}
			}
			// 2011.10.04 ADD BY PWR : ����ȸ������ �߰�
			else if(_T("����ȸ�����к�") == sYAlias)
			{
				if(_T("Y") == sCode)
				{
					sCode = _T("����ȸ��");
				}
				else if(_T("N") == sCode)
				{
					sCode = _T("�Ϲ�ȸ��");
				}
				else
				{
					sCode = _T("�˼�����");
				}
			}
			else
			{
				// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( sYCode , sCode , sCode );

				CString strCodeBuf = sCode;
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( sYCode , sCode , sCode );

				CString strDispBuf;
				strDispBuf.Format(_T("%s(%s)"), sCode, strCodeBuf);

				sCode = strDispBuf;
				// KOL.RED.2018.210 ---------------------------------------------------------------------------
			}
			if( ids < 0 )
				sCode = sTmpCode;

			/*
			if( ids < 0 )
			{
				CString sEMsg;
				sEMsg.Format( _T(" sYAlias : %s , sYCode : %s , sCode : %s ") , sYAlias , sYCode , sCode );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("RunStatics") );
			*/

			ids = m_pRealResultDM->SetCellData( i , 0 , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("RunStatics") );

			// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			// ��Ŭ���� ������ ����
			if( m_pAxisCode_CellClick != NULL )
			{
				m_pAxisCode_CellClick->SetCodeToDesc( 1 , i , sYAlias , sCode_Temp , sCode );
			}
			// KOL.RED.2018.211 ---------------------------------------------------------------------------

		}
	}
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	else
	{
		// �ڵ� ��ȯ�Ұ��� ������ �ڵ尪 �״�� �ִ´�.
		for( INT i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
		{
			ids = m_pRealResultDM->GetCellData( i , 0 , sCode );
			if( m_pAxisCode_CellClick != NULL )
			{
				m_pAxisCode_CellClick->SetCodeToDesc( 1 , i , sYAlias , sCode , sCode );
			}
		}
	}
	// KOL.RED.2018.211 ---------------------------------------------------------------------------

	return 0;

	EFS_END
	return -1;

}

// ȸ�����к� �ڵ� DESC�� ����
INT CStaticsInfoMgr::ConvertInferiorCodeToDesc( CString sCode , CString &sDesc )
{
	EFS_BEGIN

	if( sCode.Compare(_T("0"))==0 ) sDesc = _T("��ȸ��");
	if( sCode.Compare(_T("1"))==0 ) sDesc = _T("��ȸ��");
	// 2007.04.03 add by pdw {{++++++++++++++++++++++++++++++++++++++++++++
	if( sCode.Compare(_T("2"))==0 ) sDesc = _T("��ȸ��");
	// ------------------------------------------------------------------}}

	return 0;

	EFS_END
	return -1;

}

// ##############################################################################################################
// [ �Լ����� ]
//   ����� �׸��忡 �ѷ��ֱ�
//
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::StaticsResultViewGrid( CString sStaticsClass , CMSHFlexGrid *pGrid , bool bDupDel , bool bZeroShow , CString sMenuName)
{
EFS_BEGIN

	INT ids;
	
	if ( m_pRealResultDM->GetRowCount() < 1 || !pGrid->GetSafeHwnd() ) return 0;

	CString sSumAlias;
	if ( !bDupDel )		sSumAlias = _T("�ߺ����_�հ�");
	else        		sSumAlias = _T("�ߺ�����_�հ�");

	// ==========================================================================================================
	// 1. �׸��� ����
	// ==========================================================================================================
	pGrid->SetRedraw(FALSE);
	pGrid->Clear();

	pGrid->SetRows( m_pRealResultDM->GetRowCount()+2 );
	pGrid->SetCols( 0 , m_pRealResultDM->RefList.GetCount()+1 );

	// ==========================================================================================================
	// 2. X�� �÷� �ѷ��ֱ�
	// ==========================================================================================================
	INT refcount = m_pRealResultDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = m_pRealResultDM->RefList.GetHeadPosition();
	CString query, fields, isudf;
	CString sResult;

	//2012.10.26 ADD BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (���� ǥ��)
	//x�� �˸��ƽ��� �����´�
	CString sXAlias;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("X�༱�þ˸��ƽ�") , sXAlias );
	////////////////////////////////////////////////////////////////
	
	for( INT i = 0 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)m_pRealResultDM->RefList.GetNext(pos);

		pGrid->SetTextMatrix( 0 , i ,  pRef->FIELD_ALIAS );

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( pRef->FIELD_ALIAS , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::StaticsResultViewGrid()") );

			//2012.10.26 ADD BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (���� ǥ��)
			//������ �� ������ �����迡�� ��� �� ������ ǥ��
			if(sStaticsClass == _T("������ ���") || sStaticsClass == _T("������ ������") )
			{	
				if((sXAlias.Compare(_T("����"))==0 && i > 0) || (sXAlias.Compare(_T("����2"))==0 && i > 0))
				{
					CString sSumAll,sPercent;
					double dSumAll,dResult,dSum;
					ids = m_pXSumDM->GetCellData( sSumAlias , j , sSumAll );
					dSumAll=_ttoi(sSumAll); dResult=_ttoi(sResult);
					dSum=(dResult/dSumAll)*100;

					if(dSum<0) dSum=0;

					sPercent.Format(_T("%.1f"), dSum);			
					sResult = sResult + _T(" (") + sPercent + _T("%)");						
				}			
			}
			///////////////////////////////////////////////////////////////////////////////////
			pGrid->SetTextMatrix( j+1 , i , sResult );
		}
	}

	// ==========================================================================================================
	// 3. ���� �հ� �ѷ��ֱ�
	// ==========================================================================================================
	pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+1 , 0 , _T("�հ�") );	
	for( i = 0 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		for( INT j = 0 ; j < m_pYSumDM->GetRowCount() ; j++ )
		{
			ids = m_pYSumDM->GetCellData( sSumAlias , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 ,  _T("CStaticsInfoMgr::StaticsResultViewGrid()") );

			// 2012.10.26 ADD BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (���� ǥ��)
			//������ �� ������ �����迡�� ��� �� ������ ǥ��	
			ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("X�༱�þ˸��ƽ�") , sXAlias );
			if(sStaticsClass == _T("������ ���") || sStaticsClass == _T("������ ������") )
			{
				if(sXAlias.Compare(_T("����"))==0 || sXAlias.Compare(_T("����2"))==0)
				{
					CString sSumAll,sPercent;
					double dSumAll,dResult,dSum;
					ids = m_pXSumDM->GetCellData( sSumAlias ,m_pXSumDM->GetRowCount()-1 , sSumAll );
					dSumAll=_ttoi(sSumAll); dResult=_ttoi(sResult);
					dSum=(dResult/dSumAll)*100;

					if(dSum<0) dSum=0;

					sPercent.Format(_T("%.1f"), dSum);				
					sResult = sResult + _T(" (") + sPercent + _T("%)");	
				}
			}
			//////////////////////////////////////////////////////////////////
			pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+1 , j+1 , sResult );
		}		
	}

	// ==========================================================================================================
	// 4. ���� �հ� �ѷ��ֱ�
	// ==========================================================================================================
	INT nSumCol = m_pRealResultDM->RefList.GetCount();
	pGrid->SetTextMatrix( 0 , nSumCol , _T("�հ�") );

	CString sSum;
	for( i = 0 ; i < m_pXSumDM->GetRowCount() ; i++ )
	{
		ids = m_pXSumDM->GetCellData( sSumAlias , i , sSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 ,  _T("CStaticsInfoMgr::StaticsResultViewGrid()") );

		// 2012.10.26 ADD BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (���� ǥ��)
		//������ �� ������ �����迡�� ��� �� ������ ǥ��
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("X�༱�þ˸��ƽ�") , sXAlias );
		if(sStaticsClass == _T("������ ���") || sStaticsClass == _T("������ ������") )
		{
			if(sXAlias.Compare(_T("����"))==0 || sXAlias.Compare(_T("����2"))==0 )
			{
				sSum=sSum + _T(" (100.0%)");
			}
		}
		///////////////////////////////////////////////////
		pGrid->SetTextMatrix( i+1 , nSumCol , sSum );
	}

	pGrid->SetTextMatrix( 0 , 0 , _T("") );

	// ==========================================================================================================
	// 5. �հ� 0 �����ֱ� 
	//    ������ �������� �״�� �����ش�.
	// ========================================================================================================== 
	if( bZeroShow )
	{
		SetGridCellAlignment(pGrid);
		pGrid->SetRedraw(TRUE);
		pGrid->Refresh();
		return 0;
	}

	// ========================================================================================================== 
	// 6. �հ谡 0�� �ڷ� �����ֱ� ����
	// ========================================================================================================== 
	// ���� ����
	INT nGridRow = pGrid->GetRows();
	INT nGridCol = pGrid->GetCols(0);

	CString sSpeciesSum , sVolSum, sBookSum;
	for( i = nGridRow-2 ; i > 0 ; i-- )
	{
		m_pXSumDM->GetCellData( sSumAlias , i-1 , sSum );

		// 2005.07.28 ADD BY PDJ ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ������ ��� ����з���ȣ�� �հ� 0�� üũ
		if ( m_sIs64lib == _T("Y") && sSum.FindOneOf( DIVCHAR ) > -1 )
		{			
			// _T("(") ��ȣ�� �ִٴ°��� ���ڰ� �־ ������ �Ѱ��̹Ƿ�
			// count�� 0������ üũ�ϹǷ� ���� ���ʿ�� ����.
			sSum = _T("1");
		}
		// ------------------------------------------------------------------------------------------------------

		// ======================================================================================================
		// 7. ��躰 ���� ������ ������ �ٸ���.
		// ======================================================================================================
		if ( sMenuName.Compare( LOAN_STATICS ) == 0 || sMenuName.Compare( REPAIR_BOOK_STATICS ) == 0 || sMenuName.Compare( COOPERATIVE_STATICS ) == 0 )
		{
			if ( _ttoi(sSum) == 0 )
				GridDeleteRow( i , pGrid );
		}
		else if ( sMenuName.Compare( WORKING_STATICS ) == 0 || sMenuName.Compare( HOLDING_STATICS ) == 0 )
		{
			MakeSpeciesBookCnt( sSum , sSpeciesSum , sBookSum );

			if ( _ttoi(sSpeciesSum) == 0 && _ttoi(sBookSum) == 0 )
				GridDeleteRow( i , pGrid );
		}
		// 2006-03-30 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		else if ( sMenuName.Compare( SE_HOLDING_STATICS ) == 0 )
		{
			if(sStaticsClass == _T("�ڷ�Ǻ��̿����") || sStaticsClass == _T("�űԱ��Ե����̿����"))
			{
				MakeSpeciesBookCnt( sSum , sSpeciesSum , sBookSum );
				if ( _ttoi(sSpeciesSum) == 0 && _ttoi(sBookSum) == 0 )
					GridDeleteRow( i , pGrid );
			}else
			{
				MakeSpeciesVolBookCnt( sSum , sSpeciesSum , sVolSum, sBookSum );
				if ( _ttoi(sSpeciesSum) == 0 && _ttoi(sVolSum) == 0 && _ttoi(sBookSum) == 0 )
					GridDeleteRow( i , pGrid );
				
			}
			/*MakeSpeciesVolBookCnt( sSum , sSpeciesSum , sVolSum, sBookSum );
			if ( _ttoi(sSpeciesSum) == 0 && _ttoi(sVolSum) == 0 && _ttoi(sBookSum) == 0 )
				GridDeleteRow( i , pGrid );
				*/
		}
		// -----------------------------------------------------------------------------------------------------
	}

	SetGridCellAlignment(pGrid);

	pGrid->SetRedraw(TRUE);
	pGrid->Refresh();

	return 0;

EFS_END
	return -1;
}


// ����� �׸��忡 �ѷ��ֱ�
INT CStaticsInfoMgr::StaticsResultViewGrid3( CString sStaticsClass , CMSHFlexGrid *pGrid , bool bDupDel , bool bZeroShow , CString sMenuName)
{
	EFS_BEGIN

	INT ids;
	
	if( m_pRealResultDM->GetRowCount() < 1 ) return 0;

	CString sSumAlias;
	if( !bDupDel )
		sSumAlias = _T("�ߺ����_�հ�");
	else
		sSumAlias = _T("�ߺ�����_�հ�");

	if( pGrid->GetSafeHwnd() == NULL ) return 0;

	pGrid->SetRedraw(FALSE);

	// �׸��忡 �ѷ��ش�.
	pGrid->Clear();

	pGrid->SetRows( m_pRealResultDM->GetRowCount()+3 );
	pGrid->SetCols( 0 , m_pRealResultDM->RefList.GetCount()+2 );

	// X�� �÷� �ѷ��ֱ�
	INT refcount = m_pRealResultDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = m_pRealResultDM->RefList.GetHeadPosition();
	CString query, fields, isudf;

	CString sResult;
	for( INT i = 0 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)m_pRealResultDM->RefList.GetNext(pos);

		pGrid->SetTextMatrix( 0 , i ,  pRef->FIELD_ALIAS );

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( pRef->FIELD_ALIAS , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			pGrid->SetTextMatrix( j+1 , i , sResult );
		}

	}

	// ���� �հ� �ѷ��ֱ�
	pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+1 , 0 , _T("�հ�") );	
	for( i = 0 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{		
		
		for( INT j = 0 ; j < m_pYSumDM->GetRowCount() ; j++ )
		{
			ids = m_pYSumDM->GetCellData( sSumAlias , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+1 , j+1 , sResult );
		}		
	}


	// ���� �հ� �ѷ��ֱ�
	INT nSumCol = m_pRealResultDM->RefList.GetCount();
	pGrid->SetTextMatrix( 0 , nSumCol , _T("�հ�") );

	CString sSum;
	for( i = 0 ; i < m_pXSumDM->GetRowCount() ; i++ )
	{
		ids = m_pXSumDM->GetCellData( sSumAlias , i , sSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

		pGrid->SetTextMatrix( i+1 , nSumCol , sSum );
	}

	// ���� �ѷ��ֱ�
	// ���� ���� �ѷ��ֱ�
	pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+2 , 0 , _T("����") );	
	for( i = 0 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		for( INT j = 0 ; j < m_pYPriceSumDM->GetRowCount() ; j++ )
		{
			ids = m_pYPriceSumDM->GetCellData( sSumAlias , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			//pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+2 , j+1 , sResult );
			pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+2 , j+1 , ConvertPrice( sResult ) );
		}
	}

	// ���� ���� �ѷ��ֱ�
	pGrid->SetTextMatrix( 0 , nSumCol+1 , _T("����") );

	for( i = 0 ; i < m_pXPriceSumDM->GetRowCount() ; i++ )
	{
		ids = m_pXPriceSumDM->GetCellData( sSumAlias , i , sSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

		if( i == m_pXPriceSumDM->GetRowCount()-1 )
			pGrid->SetTextMatrix( i+2 , nSumCol+1 , ConvertPrice( sSum ) );
			//pGrid->SetTextMatrix( i+2 , nSumCol+1 , sSum );
		else
			pGrid->SetTextMatrix( i+1 , nSumCol+1 , ConvertPrice( sSum ) );
			//pGrid->SetTextMatrix( i+1 , nSumCol+1 , sSum );
	}


	pGrid->SetTextMatrix( 0 , 0 , _T("") );

	if( bZeroShow )
	{
		SetGridCellAlignment(pGrid);
		pGrid->SetRedraw(TRUE);
		pGrid->Refresh();

		return 0;
	}


	// �հ谡 0�� �ڷ� �����ֱ� ����
	// ���� ����
	INT nGridRow = pGrid->GetRows();
	INT nGridCol = pGrid->GetCols(0);

	CString sSpeciesSum , sBookSum;
	for( i = nGridRow-3 ; i > 0 ; i-- )
	{
		m_pXSumDM->GetCellData( sSumAlias , i-1 , sSum );

		if( _ttoi(sSum) == 0 )
			GridDeleteRow( i , pGrid );
	}

	/* ���� �Ⱥ����ֱ�
	for( i = nGridCol-3 ; i > 0 ; i-- )
	{
		m_pYSumDM->GetCellData( sSumAlias , i-1 , sSum );
		if( _ttoi(sSum) == 0 )
			GridDeleteCol( i , pGrid );

	}
	*/
	SetGridCellAlignment(pGrid);

	pGrid->SetRedraw(TRUE);
	pGrid->Refresh();

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::IsEqualGridColumn( CMSHFlexGrid *pGrid , CString sXAlias , CString sYAlias , INT &nXIndex , INT &nYIndex , INT nIndex )
{
	EFS_BEGIN

	CString sTmpAlias;
	nXIndex = -1;
	nYIndex = -1;
	// X�� ��������
	for( INT i = 1 ; i < pGrid->GetCols(0) ; i++ )
	{

		if( nIndex == 0 )
		{
			pGrid->SetTextMatrix( 0 , i , sXAlias );
		}

		sTmpAlias = pGrid->GetTextMatrix( 0 , i );

		if( sTmpAlias.Compare( sXAlias ) == 0 ) 
			nXIndex = i;
	}


	// Y�� ��������
	for( i = 1 ; i < pGrid->GetRows() ; i++ )
	{

		if( nIndex == 0 )
		{
			pGrid->SetTextMatrix( i , 0 , sYAlias );
		}

		sTmpAlias = pGrid->GetTextMatrix( i , 0 );

		if( sTmpAlias.Compare( sYAlias ) == 0 ) 
			nYIndex = i;

		
	}


	return 0;

	EFS_END
	return -1;

}

// �ʵ�� ������
INT CStaticsInfoMgr::GetFieldName( CString sStaticsClass , CString sAlias , CString &sFieldName , CString &sGroupYN )
{
	EFS_BEGIN

	CString sFunctionName;
	INT ids;

	ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") , sAlias , _T("�˻��ʵ��") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetFieldName") );

	// 2005.07.21 ADD BY PDJ
	// SELECT ���� FUNCTION�� �̿��� ��쿡 ���� ó��
	if ( ids > 0 )
	{		
		INT		nStartPos, nEndPos;
		
		nStartPos = sAlias.Find(_T("("), 0 );	
		nEndPos = sAlias.Find(_T(")"), 0 );	
		sFunctionName = sAlias;
		sAlias = sAlias.Mid(nStartPos+1, nEndPos-nStartPos-1);
		
		ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") ,sAlias , _T("�˻��ʵ��") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetFieldName") );

		// �̸��κи� Field������ ��ü ( ������� )
		sFunctionName.Replace(sAlias, sFieldName);
		sFieldName = sFunctionName;		
	}
	
	ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") , sAlias , _T("�׷쿩��") , sGroupYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetFieldName") );
	
	// ���� �ʹ� ��ƴ� �Ѥ�;;
	// ���� ��ȸ�� ����..  ������ �Ѥ�;;
	CString sSpecialFieldAlias , sSpecialField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sSpecialFieldAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetFieldName") );

	ids = GetCellData( m_pUserMakeSearchCondDM   , _T("�˸��ƽ�")  , sSpecialFieldAlias , _T("FieldName") , sSpecialField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetFieldName") );

	// 2012.11.24 UPDATE BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (ȸ���� �߱�Ƚ����� �߰��� ���� ���� ����)
	// ȸ�����߱���� ����� ���� ����
	if( sAlias.Compare(_T("��")) == 0 )
	{
		if( sStaticsClass.Compare(_T("ȸ�����߱����"))==0 )
		{
			sSpecialField=_T("CM.REISSUE_DATE");
		}

		sFieldName.Format( _T("to_char( %s , 'YYYY' )") , sSpecialField );
	}

	else if( sAlias.Compare(_T("��")) == 0 )
	{
		if( sStaticsClass.Compare(_T("ȸ�����߱����"))==0 )
		{
			sSpecialField=_T("CM.REISSUE_DATE");
		}

		sFieldName.Format( _T("to_char( %s , 'YYYY/MM' )") , sSpecialField );
	}
	else if( sAlias.Compare(_T("��")) == 0 )
	{
		if( sStaticsClass.Compare(_T("ȸ�����߱����"))==0 )
		{
			sSpecialField=_T("CM.REISSUE_DATE");
		}

		sFieldName.Format( _T("to_char( %s , 'YYYY/MM/DD' )") , sSpecialField );
	}

	else if( sAlias.Compare(_T("�ð��뺰")) == 0 )
	{
		if( sStaticsClass.Compare(_T("ȸ�����߱����"))==0 )
		{
			sSpecialField=_T("CM.REISSUE_DATE");
		}

		//++2008.09.02 DEL BY LKS {{++
//REMOVE�� 		if( m_pInfo->MODE == 10000 || (m_pInfo->MODE == 30000) )
//REMOVE�� 		{
//REMOVE�� 			sFieldName.Format( _T("extract(HOUR FROM %s)") , sSpecialField );
//REMOVE�� 		}
		//--2008.09.02 DEL BY LKS --}}
//		else
			sFieldName.Format( _T("to_char( %s , 'HH24' )") , sSpecialField );
	}
	else if( sAlias.Compare(_T("����")) == 0 )
	{
	}


	return 0;

	EFS_END
	return -1;

}


INT CStaticsInfoMgr::GetTimeColumnNames( CString sStaticsClass , CString sAlias , CStringArray &asArray  )
{
	EFS_BEGIN

	INT ids;

	CString sControlAlias;

	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sControlAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetTimeColumnNames") );

	if( m_pSearchCM == NULL ) return 0;

	ids = MakeUserSearchList( sStaticsClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetTimeColumnNames") );

	// ����� ���ǿ��� �����´�.
	
	
	CString sDate1,sDate2;
	ids = GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias+_T("1") , _T("��") , sDate1 );
	if( ids < 0 ) 
	{
		CString sMsg;
		sMsg.Format( _T("m_pSearchCM->GetControlMgrData( %s )") , sControlAlias+_T("1") );
		AfxMessageBox( sMsg );
	}
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetTimeColumnNames") );
	
	ids = GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias+_T("2") , _T("��") , sDate2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetTimeColumnNames") );

	m_pUserSearchCond->FreeData();

// 2009.05.20 UPDATE BY CJY : �� ������ ��� �˻������ϵ��� ����
	if( sAlias.Compare(_T("�ð��뺰")) == 0 )
	{
		CString sHour;
		for( INT i = 0 ; i < 24 ; i++ )
		{
			sHour.Format(_T("%02d"),i);
			asArray.Add(sHour);
		}

		return 0;
	}

	if( sDate1.IsEmpty() ) return 0;

	// 050126_��������
	// CTime -> COleDateTime �� ����
	COleDateTime tDate1( _ttoi(sDate1.Mid(0,4)) , _ttoi(sDate1.Mid(5,2)) , _ttoi(sDate1.Mid(8,2)) , 0 , 0 , 0 );
	COleDateTime tDate2;
//	�����ҽ�
//  	if( sDate1.IsEmpty() ) return 0;
// 
// 	// 050126_��������
// 	// CTime -> COleDateTime �� ����
// 	COleDateTime tDate1( _ttoi(sDate1.Mid(0,4)) , _ttoi(sDate1.Mid(5,2)) , _ttoi(sDate1.Mid(8,2)) , 0 , 0 , 0 );
// 	COleDateTime tDate2;
// 
// 	if( sAlias.Compare(_T("�ð��뺰")) == 0 )
// 	{
// 		CString sHour;
// 		for( INT i = 0 ; i < 24 ; i++ )
// 		{
// 			sHour.Format(_T("%02d"),i);
// 			asArray.Add(sHour);
// 		}
// 
// 		return 0;
// 	}

	if( sDate2.IsEmpty() )
		tDate2 = tDate1;
	else
		tDate2 = COleDateTime( _ttoi(sDate2.Mid(0,4)) , _ttoi(sDate2.Mid(5,2)) , _ttoi(sDate2.Mid(8,2)) , 0 , 0 , 0 );
	


	while(TRUE)
	{		
		if( tDate1 > tDate2 )
			break;
		
		if( sAlias.Compare(_T("��")) == 0 )
			asArray.Add( tDate1.Format(_T("%Y")) );
		else if( sAlias.Compare(_T("��")) == 0 )
			asArray.Add( tDate1.Format(_T("%Y/%m")) );
		else if( sAlias.Compare(_T("��")) == 0 )
			asArray.Add( tDate1.Format(_T("%Y/%m/%d")) );
		
		if( sAlias.Compare(_T("��"))==0 )
			GetNextYear(tDate1);
		else if( sAlias.Compare(_T("��"))==0 )
			GetNextMonth(tDate1);
		else if( sAlias.Compare(_T("��"))==0 )
			GetNextDay(tDate1);
	}

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::GetNextYear( COleDateTime &tTime )
{
	EFS_BEGIN

	INT nYear;
	nYear = tTime.GetYear()+1;

	tTime = COleDateTime( nYear , 1 , 1 , 0 , 0 , 0  );

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::GetNextMonth( COleDateTime &tTime )
{
	EFS_BEGIN

	INT nYear,nMonth;
	nYear = tTime.GetYear();
	nMonth = tTime.GetMonth()+1;

	if( nMonth > 12 )
	{
		nYear ++;
		nMonth = nMonth - 12;
	}

	tTime = COleDateTime( nYear , nMonth , 1 , 0 , 0 , 0  );

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::GetNextDay( COleDateTime &tTime )
{
	EFS_BEGIN

	COleDateTimeSpan tOneDay( 1 , 0 , 0 , 0 );
	tTime += tOneDay;

	return 0;

	EFS_END
	return -1;

}

// DM�� �÷� �߰��ϱ�
INT CStaticsInfoMgr::SetSearchCM( CESL_ControlMgr *pCM )
{
	EFS_BEGIN

	m_pSearchCM = pCM;
	
	return 0;

	EFS_END
	return -1;

}

// nMode == 0 : X��
// nMode == 1 : Y��
INT CStaticsInfoMgr::SetDataMgrRef( CESL_DataMgr *pDM , CString sCondition , CString sInitValue , CString sGroupYN , INT nMode)
{
	EFS_BEGIN

	CString sUdf;
	CString sAlias;

	if( sGroupYN.Compare(_T("Y")) == 0 ) sUdf = _T("");
	else sUdf = _T("udf");

	if( nMode == 0 ) 
		sAlias= _T("X��˸��ƽ�");
	else if( nMode == 1 )
		sAlias = _T("Y��˸��ƽ�");
	else 
		sAlias = _T("�����");

	INT refcount = pDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
      	pos = pDM->RefList.GetHeadPosition();
	CString query, fields, isudf;
	for( INT i = 0 ; i < pDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)pDM->RefList.GetNext(pos);

		if( pRef->FIELD_ALIAS.Compare(sAlias) == 0 )
		{
			pRef->INIT_VALUE = sInitValue;
			pRef->DATA_TYPE = sUdf;
			pRef->FIELD_NAME = sCondition;
		}
	}

	return 0;

	EFS_END
	return -1;

}


// �հ� ����
INT CStaticsInfoMgr::MakeTotalCount( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName, CString sYFieldName , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	// ���� �հ� ����
	ids = YSum(sStaticsClass,saTableName,sXFieldName , sYFieldName ,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("MakeTotalCount") );

	// ���� �հ� ����
	ids = XSum(sStaticsClass,saTableName,sXFieldName , sYFieldName ,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-2 , _T("MakeTotalCount") );

	return 0;

	EFS_END
	return -1;

}


/// ����/�ݳ� ����� �հ� ���� 
/// 20040204 - ������ - ������
INT CStaticsInfoMgr::MakeTotalCount( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName, CString sYFieldName , CString sXAlias , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;
		
	// ���� �հ� ����
	ids = YLoanSum(sStaticsClass,saTableName,sXFieldName , sYFieldName ,sXAlias,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("MakeTotalCount") );
	
	// ���� �հ� ����
	ids = XLoanSum(sStaticsClass,saTableName,sXFieldName , sYFieldName ,sXAlias,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-2 , _T("MakeTotalCount") );
	return 0;
	
	EFS_END
	return -1;
	
}

//###############################################################################################################
// [ �Լ����� ]
//   ����/�ݳ� ����� �հ� ���� 
//   20040204 - ������ - �׷��� �հ�� �ٸ��� ������ ����
//
// [ �̴Ͻ��� ]
//
//###############################################################################################################
INT CStaticsInfoMgr::MakeTotalCount( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName, CString sYFieldName , CString sXAlias , CString sXGroupYN , CString sYGroupYN ,  CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
EFS_BEGIN
		
	INT ids;

	// ==========================================================================================================
	// 1. �հ� DM�ʱ�ȭ
	// ==========================================================================================================
	m_pXSumDM->FreeData();
	m_pYSumDM->FreeData();

	// ==========================================================================================================
	// 2. �ߺ� ��� �հ� ����
	// ==========================================================================================================
	ids = XYLoanSum_duplicate_allow();
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeTotalCount()") );

	// ==========================================================================================================
	// 3. �ߺ� ���� �հ� ����
	//    �׷� ���ο� ���� �޶���
	// ==========================================================================================================
	ids = XLoanSum_duplicate_delete(sStaticsClass,saTableName,sXFieldName , sYFieldName ,sXAlias,pTotalProgress);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeTotalCount()") );

	return 0;
	
EFS_END
	return -1;	
}

//###############################################################################################################
// [ �Լ����� ]
// 20040204 ������ - ����ݳ� �ߺ� ��� �հ�
//
// [ �̴Ͻ��� ]
//
//###############################################################################################################
INT CStaticsInfoMgr::XYLoanSum_duplicate_allow()
{
EFS_BEGIN

	INT ids;
	
	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSum;
	CString sData;
	CESL_DataMgr::reference *Ref;
	CString strNormal, strArmy, strNormal2, strArmy2;
	POSITION pos;	

	// ==========================================================================================================
	// 1. ���� �ߺ� ��� �հ� ����
	// ==========================================================================================================
	for ( INT i = 1;i < m_pRealResultDM->RefList.GetCount();i++ )
	{
		sSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;
		
		for ( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XYLoanSum_duplicate_allow()") );
			
			if ( m_sIs64lib == _T("Y") && (sData.FindOneOf(_T(",")) > -1 || sSum.FindOneOf( DIVCHAR ) > -1) )
			{	
				// UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �Է��� �����͵� �и��ؾ��Ѵ�.
				DividNormalArmy(sSum, strNormal, strArmy);				
				DividNormalArmy(sData, strNormal2, strArmy2);
				sSum.Format( _T("%d %s %d") , _ttoi(strNormal) + _ttoi(strNormal2) ,DIVCHAR , _ttoi(strArmy) + _ttoi(strArmy2) );
				// ----------------------------------------------------------------------------------------------
			}
			else
				sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );			
		}
		
		if( m_pYSumDM->GetRowCount() < i )	m_pYSumDM->InsertRow(-1);
		
		m_pYSumDM->SetCellData( sSumAlias , sSum ,i-1 );
	}	

	// ==========================================================================================================
	// 2. ���� �ߺ� ��� �հ� ����
	// ==========================================================================================================
	sAlias.Empty();
	sSum.Empty();
	sData.Empty();
	CString sTotalSum;
	Ref = NULL;
	for( i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;
			
			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XYLoanSum_duplicate_allow()") );
			
			if ( m_sIs64lib == _T("Y") && (sData.FindOneOf( DIVCHAR ) > -1 || sSum.FindOneOf( DIVCHAR ) > -1) )
			{
				// UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// �Է��� �����͵� �и��ؾ��Ѵ�.
				DividNormalArmy(sSum, strNormal, strArmy);				
				DividNormalArmy(sData, strNormal2, strArmy2);
				sSum.Format( _T("%d") , _ttoi(strNormal) + _ttoi(strNormal2) + _ttoi(strArmy) + _ttoi(strArmy2) );
				// ----------------------------------------------------------------------------------------------
			}
			else
				sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		sTotalSum.Format( _T("%d") , _ttoi(sTotalSum) + _ttoi(sSum) );
		
		if( m_pXSumDM->GetRowCount() < i+1 )	m_pXSumDM->InsertRow(-1);
		
		m_pXSumDM->SetCellData( sSumAlias , sSum , i);		
	}
	
	if( m_pXSumDM->GetRowCount() < i+1 )	m_pXSumDM->InsertRow(-1);
	
	m_pXSumDM->SetCellData( sSumAlias , sTotalSum , i );
	
	return 0;

EFS_END
	return -1;
}

//###############################################################################################################
// [ �Լ����� ]
//   ������� �ߺ����� �հ� 
// 
// [ �̴Ͻ��� ]
//
//###############################################################################################################
INT CStaticsInfoMgr::XLoanSum_duplicate_delete( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CString sXAlias , CProgressCtrl *pTotalProgress )
{
EFS_BEGIN
	
	INT ids;

	INT i = 0;
	
	// ==========================================================================================================
	// 1. �Ⱓ ��� ����
	// ==========================================================================================================
	CString sResultFieldName;
	if ( sXAlias.Compare(_T("��")) == 0 || (sXAlias.Compare(_T("��")) == 0) || (sXAlias.Compare(_T("��")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultFieldName );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����2") , sResultFieldName );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	
	// ==========================================================================================================
	// 2. DM ���̺� �̸� �߰�
	// ==========================================================================================================
	m_pResultDM->TBL_NAME = saTableName.GetAt(0);
		
	CStringArray saTableAlias;

	ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
			
	// ==========================================================================================================
	// 3. ���� �հ� ���
	//    where ���� �����Ͽ� DM�� �籸������ ���DM�� �����.
	// ==========================================================================================================
	// SELECT �ʵ� ����
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("Y") , 1 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sResultFieldName , _T("0") , _T("Y") , 2 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	
	// WHERE�� ���� 		
	CString sWhere;
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , _T("") , _T("N") , _T("Y") ,i , sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

	sWhere += _T(" GROUP BY ") + sYFieldName;
		
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	
	CString sYSumAlias;
	CString sTmpResult , sResult;
	CString sYData , sTmp;	
	INT nIndex;

	// �˸��ƽ��� �´� �ε����� ���Ͽ� �����͸� �Է��Ѵ�.
	for ( INT k = 0;k < m_pResultDM->GetRowCount();k++ )
	{
		ids = m_pResultDM->GetCellData( _T("Y��˸��ƽ�") , k , sYSumAlias );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
		
		nIndex = -1;
		for ( INT j = 0;j < m_pRealResultDM->GetRowCount();j++ )
		{
			ids = m_pRealResultDM->GetCellData( j , 0 , sYData );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
			
			if ( sYData.Compare( sYSumAlias ) == 0 ) nIndex = j;
		}
		
		if ( nIndex == -1 ) continue;
		
		ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
		
		if ( _ttoi(sResult) == 0 )	sResult = _T("0");
		
		ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , nIndex );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );		
	}

	// ==========================================================================================================
	// 4. ���� �հ� ���
	//    where ���� �����Ͽ� DM�� �籸������ ���DM�� �����.
	// ==========================================================================================================
	// SELECT �ʵ� ����
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("Y") , 0 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sResultFieldName , _T("0") , _T("Y") , 2 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
		
	// WHERE�� ����
	sWhere.Empty();
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , _T("") , _T("") ,_T("Y") , _T("N") , i , sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

	sWhere += _T(" GROUP BY ") + sXFieldName;
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

	CString sXSumAlias;
	sTmpResult.Empty() , sResult.Empty();
	CString sXData;
	sTmp.Empty();	

	for ( k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
	{
		ids = m_pResultDM->GetCellData( _T("X��˸��ƽ�") , k , sXSumAlias );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
		
		nIndex = -1;

		// 2005.07.28 ADD BY PDJ ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ���� ����з� �հ� �κ� ����ó��
		if ( m_sIs64lib == _T("Y") && ( sXSumAlias==_T("355-9") ) )  nIndex = 4;		
		else
		{		
			for ( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if ( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

				if ( sXData.Compare( sXSumAlias ) == 0 ) nIndex = j;
			}
		}		
		
		if ( nIndex == -1 ) continue;

		ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

		if ( m_sIs64lib == _T("Y") && ( sXSumAlias==_T("355-9") || sXSumAlias==_T("3") ) ) 
		{
			CString strNormal, strArmy;
			CString sPrevData;
			ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , nIndex-1 , sPrevData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -32 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );				

			DividNormalArmy(sPrevData, strNormal, strArmy);

			if ( sXSumAlias==_T("3") )
				sResult.Format(_T("%d %s %d"), _ttoi(strNormal)+_ttoi(sResult), DIVCHAR, _ttoi(strArmy) );
			else
				sResult.Format(_T("%d %s %d"), _ttoi(strNormal), DIVCHAR, _ttoi(strArmy)+_ttoi(sResult) );			
		}

		if ( m_sIs64lib == _T("Y") && ( sXSumAlias==_T("355-9") || sXSumAlias==_T("3") ) ) 		
		{
			CString strNormal, strArmy;
			DividNormalArmy(sResult, strNormal, strArmy);

			if( _ttoi(strArmy) == 0 && _ttoi(strNormal) == 0 )	sResult = _T("0");		
		}
		else
			if( _ttoi(sResult) == 0 )	sResult = _T("0");		
		// ------------------------------------------------------------------------------------------------------

		ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , nIndex-1  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );				
	}
	
	// ==========================================================================================================
	// 5. ���հ�
	// ==========================================================================================================
	// SELECT �ʵ� ����
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sResultFieldName , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	
	// WHERE�� ����
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
			
	ids = m_pResultDM->GetCellData( _T("�����") , 0 , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -26 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	
	if( _ttoi(sResult) == 0 )	sResult = _T("0");
	
	ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , m_pXSumDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -27 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );	
	
	// ==========================================================================================================
	// 6. ��� �հ��� ����ִ� ������ 0���� �����
	// ==========================================================================================================
	for( INT m = 0 ; m < m_pXSumDM->GetRowCount() ; m++ )
	{
		ids = m_pXSumDM->GetCellData( _T("�ߺ�����_�հ�") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -28 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;

		ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -29 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	}

	for( m = 0 ; m < m_pYSumDM->GetRowCount() ; m++ )
	{
		ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -30 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
		
		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;
		
		ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	}
	
	return 0;	

EFS_END
	return -1;
}


INT CStaticsInfoMgr::MakeTotalCount3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName, CStringArray &saYFieldName , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	// ���� ����
	ids = YSum3(sStaticsClass,saTableName,saXFieldName , saYFieldName ,pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );

	// ���� �հ� ����
	ids = XSum3(sStaticsClass,saTableName,saXFieldName , saYFieldName ,pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );

	return 0;


	EFS_END
	return -1;

}

// ���� �ջ� �����ϱ�
INT CStaticsInfoMgr::MakePriceSum3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName, CStringArray &saYFieldName , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	// ���� �հ� ����
	ids = YPriceSum3(sStaticsClass,saTableName,saXFieldName , saYFieldName ,pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );

	// ���� �հ� ����
	ids = XPriceSum3(sStaticsClass,saTableName,saXFieldName , saYFieldName ,pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );

	return 0;


	EFS_END
	return -1;

}

INT CStaticsInfoMgr::PostInsertDMColumn( CESL_DataMgr *pDM )
{
	EFS_BEGIN

	INT ids;

	INT nInsertCnt = pDM->RefList.GetCount();

	CString sData;

	INT nRowCount = pDM->GetRowCount();

	for( INT i = 0 ; i < nRowCount; i++ )
	{
		pDM->InsertRow(i);
		
		for( INT j = 0 ; j < nInsertCnt ; j++ )
		{
			if( j == nInsertCnt-1 )
				sData = _T("0");
			else
			{
				ids = pDM->GetCellData( i+1 , j , sData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PostInsertDMColumn") );

			}

			ids = pDM->SetCellData( i , j , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PostInsertDMColumn") );

		}	
		pDM->DeleteRow( i+1 , nInsertCnt-1 );

		CString sTest;
		pDM->GetCellData( i , 0 , sTest );
	}

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::MakeStaticSql( CStringArray &saTableAlias ,CString sXFieldName ,  CString sXData , CString sYFieldName , CString sYData , CString &sInStaticWhere , CString &sOutStaticWhere )
{
	EFS_BEGIN

	sInStaticWhere.Empty();
	sOutStaticWhere.Empty();

	CString sTmpWhere;
	CString sTmpField;
	CString sTmpData;

	for( INT i = 0 ; i < 2 ; i++ )
	{
		sTmpWhere.Empty();
		sTmpData.Empty();

		if( i == 0 )
		{
			sTmpField = sXFieldName;
			sTmpData = sXData;
		}
		else
		{
			sTmpField = sYFieldName;
			sTmpData = sYData;
		}

		if( sTmpField.IsEmpty() ) continue;
		
		if( sTmpData.IsEmpty() )
			sTmpWhere.Format( _T(" %s IS NULL ") , sTmpField );
		else
			sTmpWhere.Format( _T(" %s = '%s' ") , sTmpField , sTmpData );
		
		if( IsFieldFromTable( sTmpField , saTableAlias ) == 1 )
		{
			if( sOutStaticWhere.GetLength() > 0 ) sOutStaticWhere += _T(" AND ");
			sOutStaticWhere += sTmpWhere;
		}
		else
		{
			if( sInStaticWhere.GetLength() > 0 ) sInStaticWhere += _T(" AND ");
			sInStaticWhere += sTmpWhere;
		}

	}

	return 0;

	EFS_END
	return -1;

}


// ���� �հ� ����
INT CStaticsInfoMgr::YSum( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	m_pYSumDM->FreeData();
	// ���� �հ� ����
	pTotalProgress->SetRange32( 0 , saTableName.GetSize()*(m_pRealResultDM->RefList.GetCount()+m_pRealResultDM->GetRowCount()) );
	INT nPos = 0;

	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM ���̺� �̸� �߰�
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		CStringArray saTableAlias;

		ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		// 3.3. WHERE�� ���� 
		CString sWhere;
		CString sTmpResult , sResult;
		CString sXData , sTmp;
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			sWhere.Empty();
			ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// SELECT �ʵ� ����
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );


			// WHERE�� ����
			ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , sXData , _T("") ,_T("N") , _T("N")  , i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeTotalCount") );

			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// RESULT DM -> REAL RESULT DM ���� �̵�
			// RESULT DM -> REAL RESULT DM ���� �̵�
			if( m_pYSumDM->GetRowCount() < j )
				m_pYSumDM->InsertRow(-1);

			ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , j-1 , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("�����") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			if ( m_sIs64lib == _T("Y") && sTmpResult.FindOneOf( DIVCHAR ) > -1 )
			{				
				CString strNormal, strArmy, strNormal2, strArmy2;
				
				DividNormalArmy(sResult, strNormal, strArmy);				
				DividNormalArmy(sTmpResult, strNormal2, strArmy2);
				sResult.Format( _T("%d %s %d") , _ttoi(strNormal) + _ttoi(strNormal2),DIVCHAR , _ttoi(strArmy) + _ttoi(strArmy2) );
			}
			else
			{
				sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
			}
			
			ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , j-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( nPos+1 ); nPos++;

		}
	}

	// �ߺ� ��� �հ� ����

	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSum;
	CString sData;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			if ( m_sIs64lib == _T("Y") && sData.FindOneOf( DIVCHAR ) > -1 )
			{				
				CString strNormal, strArmy, strNormal2, strArmy2;
				
				DividNormalArmy(sSum, strNormal, strArmy);				
				DividNormalArmy(sData, strNormal2, strArmy2);
				sSum.Format( _T("%d %s %d") , _ttoi(strNormal) + _ttoi(strNormal2), DIVCHAR, _ttoi(strArmy) + _ttoi(strArmy2) );
			}
			else
			{
				sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
			}
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);

		m_pYSumDM->SetCellData( sSumAlias , sSum ,i-1 );
	}	

	return 0;

	EFS_END
	return -1;

}

// ���� �հ� ����
INT CStaticsInfoMgr::XSum( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	m_pXSumDM->FreeData();

	// �ߺ� ���� �հ� ����
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM ���̺� �̸� �߰�
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		CStringArray saTableAlias;

		ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		// 3.3. WHERE�� ���� 
		CString sWhere;
		/*
		, sDefaultCond , sUserCond;
		*/

		/*
		ids = GetDefaultCond( sStaticsClass ,sDefaultCond ,sUserCond ,sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );
		*/
		
		CString sInStaticWhere , sOutStaticWhere , sSendWhere;		
		CString sYData;
		CString sTmpResult , sResult;
		for( INT k = 0 ; k < m_pRealResultDM->GetRowCount()+1 ; k++ )
		{
			sWhere.Empty();
			if( k == m_pRealResultDM->GetRowCount() )
				sYData = _T("�հ�");
			else
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			}

			// SELECT �ʵ� ����
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
			
			// WHERE�� ����
			sSendWhere = sWhere;
			/*
			if( sYData.Compare(_T("�հ�"))==0 )
			{
				sInStaticWhere = _T("");
				sOutStaticWhere = _T("");
			}
			else
			{
				ids = MakeStaticSql( saTableAlias , _T("") , _T("") , sYFieldName , sYData , sInStaticWhere , sOutStaticWhere);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
				if( sUserCond.GetLength() > 6&& !sInStaticWhere.IsEmpty() ) 
					sInStaticWhere = CString( _T(" AND ")  + sInStaticWhere );
				if( sUserCond.GetLength() > 6&& !sOutStaticWhere.IsEmpty() ) 
					sOutStaticWhere = CString( _T(" AND ")  + sOutStaticWhere );

			}
			*/
			if( sYData.Compare(_T("�հ�"))==0 )
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
			else
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );

			/*
			sSendWhere.Replace( _T("%�ȱ׷�����%") , sInStaticWhere );
			sSendWhere.Replace( _T("%�۱׷�����%") , sOutStaticWhere );
			*/
			
			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			// RESULT DM -> REAL RESULT DM ���� �̵�
			if( m_pXSumDM->GetRowCount() < k+1 )
				m_pXSumDM->InsertRow(-1);

			ids = m_pXSumDM->GetCellData( _T("�ߺ�����_�հ�") , k , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("�����") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
			
			ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , k );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( (i+1)*(k+1)+(saTableName.GetSize()*m_pRealResultDM->RefList.GetCount()) );
		}	
	}

	// �ߺ� ��� �հ� ����
	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSum;
	CString sData;
	CString sTotalSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;

			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		sTotalSum.Format( _T("%d") , _ttoi(sTotalSum) + _ttoi(sSum) );

		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);

		m_pXSumDM->SetCellData( sSumAlias , sSum , i);
		
	}

	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);

	m_pXSumDM->SetCellData( sSumAlias , sTotalSum , i );
	
	return 0;

	EFS_END
	return -1;

}

// ���� �հ� ���� - 20040204 ������
INT CStaticsInfoMgr::YLoanSum( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CString sXAlias , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CString sResultField;
	if( sXAlias.Compare(_T("��")) == 0 || (sXAlias.Compare(_T("��")) == 0) || (sXAlias.Compare(_T("��")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultField );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����2") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	SetDataMgrRef( m_pResultDM , sResultField , _T("0") , _T("Y") , 2 );

	m_pYSumDM->FreeData();
	// ���� �հ� ����
	pTotalProgress->SetRange32( 0 , saTableName.GetSize()*(m_pRealResultDM->RefList.GetCount()+m_pRealResultDM->GetRowCount()) );
	INT nPos = 0;

	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM ���̺� �̸� �߰�
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		CStringArray saTableAlias;

		ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		// 3.3. WHERE�� ���� 
		CString sWhere;
		CString sTmpResult , sResult;
		CString sXData , sTmp;
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			sWhere.Empty();
			ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// SELECT �ʵ� ����
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );


			// WHERE�� ����
			ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , sXData , _T("") ,_T("N") , _T("N")  , i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeTotalCount") );

			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// RESULT DM -> REAL RESULT DM ���� �̵�
			// RESULT DM -> REAL RESULT DM ���� �̵�
			if( m_pYSumDM->GetRowCount() < j )
				m_pYSumDM->InsertRow(-1);

			ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , j-1 , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("�����") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
			
			ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , j-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( nPos+1 ); nPos++;

		}
	}

	// �ߺ� ��� �հ� ����
	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSum;
	CString sData;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);

		m_pYSumDM->SetCellData( sSumAlias , sSum ,i-1 );
	}	

	return 0;

	EFS_END
	return -1;

}

// ���� �հ� ���� - 20040204 ������
INT CStaticsInfoMgr::XLoanSum( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CString sXAlias , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CString sResultField;
	if( sXAlias.Compare(_T("��")) == 0 || (sXAlias.Compare(_T("��")) == 0) || (sXAlias.Compare(_T("��")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultField );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����2") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	
	SetDataMgrRef( m_pResultDM , sResultField , _T("0") , _T("Y") , 2 );

	m_pXSumDM->FreeData();

	// �ߺ� ���� �հ� ����
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM ���̺� �̸� �߰�
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		CStringArray saTableAlias;

		ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		// 3.3. WHERE�� ���� 
		CString sWhere;
		/*
		, sDefaultCond , sUserCond;
		*/

		/*
		ids = GetDefaultCond( sStaticsClass ,sDefaultCond ,sUserCond ,sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );
		*/
		
		CString sInStaticWhere , sOutStaticWhere , sSendWhere;		
		CString sYData;
		CString sTmpResult , sResult;
		for( INT k = 0 ; k < m_pRealResultDM->GetRowCount()+1 ; k++ )
		{
			sWhere.Empty();
			if( k == m_pRealResultDM->GetRowCount() )
				sYData = _T("�հ�");
			else
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			}

			// SELECT �ʵ� ����
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
			
			// WHERE�� ����
			sSendWhere = sWhere;
			/*
			if( sYData.Compare(_T("�հ�"))==0 )
			{
				sInStaticWhere = _T("");
				sOutStaticWhere = _T("");
			}
			else
			{
				ids = MakeStaticSql( saTableAlias , _T("") , _T("") , sYFieldName , sYData , sInStaticWhere , sOutStaticWhere);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
				if( sUserCond.GetLength() > 6&& !sInStaticWhere.IsEmpty() ) 
					sInStaticWhere = CString( _T(" AND ")  + sInStaticWhere );
				if( sUserCond.GetLength() > 6&& !sOutStaticWhere.IsEmpty() ) 
					sOutStaticWhere = CString( _T(" AND ")  + sOutStaticWhere );

			}
			*/
			if( sYData.Compare(_T("�հ�"))==0 )
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
			else
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );

			/*
			sSendWhere.Replace( _T("%�ȱ׷�����%") , sInStaticWhere );
			sSendWhere.Replace( _T("%�۱׷�����%") , sOutStaticWhere );
			*/
			
			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			// RESULT DM -> REAL RESULT DM ���� �̵�
			if( m_pXSumDM->GetRowCount() < k+1 )
				m_pXSumDM->InsertRow(-1);

			ids = m_pXSumDM->GetCellData( _T("�ߺ�����_�հ�") , k , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("�����") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
			
			ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , k );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( (i+1)*(k+1)+(saTableName.GetSize()*m_pRealResultDM->RefList.GetCount()) );
		}	
	}

	// �ߺ� ��� �հ� ����
	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSum;
	CString sData;
	CString sTotalSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;

			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		sTotalSum.Format( _T("%d") , _ttoi(sTotalSum) + _ttoi(sSum) );

		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);

		m_pXSumDM->SetCellData( sSumAlias , sSum , i);
		
	}

	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);

	m_pXSumDM->SetCellData( sSumAlias , sTotalSum , i );
	
	return 0;

	EFS_END
	return -1;

}


// ���� �հ� ����
INT CStaticsInfoMgr::XSum3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName , CStringArray &saYFieldName , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	m_pXSumDM->FreeData();

	CString sRegNoList;
	CString sRegNo;
	bool IsSend;
	//=====================================================
	//2008.11.24 UPDATE BY PJW : ��Ϲ�ȣ���� ������ 1000�� ���Ϸ� �ٲ۴�.
	// 	INT nLimitCnt = 50;
    INT nLimitCnt = 999;
	//=====================================================

	for( INT n = 0 ; n < pRegNoDM->GetRowCount() ; n++ )
	{
		IsSend = FALSE;

		ids = pRegNoDM->GetCellData( _T("��Ϲ�ȣ") , n , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

		if( !sRegNoList.IsEmpty() )
			if( !sRegNo.IsEmpty() )
				sRegNoList += _T(",");
		if( !sRegNo.IsEmpty() )
			sRegNoList += _T("'") + sRegNo + _T("'");

		if( (n!=0) && (n%nLimitCnt==0) ) IsSend = TRUE;
		if( n == pRegNoDM->GetRowCount()-1 ) IsSend = TRUE;

		if( !IsSend ) continue;

		if( sRegNoList.IsEmpty() ) continue;

		// �ߺ� ���� �հ� ����
		for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
		{
			// DM ���̺� �̸� �߰�
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);
			
			CStringArray saTableAlias;

			ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

			// 3.3. WHERE�� ���� 
			CString sWhere,sSendWhere;		
			CString sYData;
			CString sTmpResult , sResult;
			for( INT k = 0 ; k < m_pRealResultDM->GetRowCount()+1 ; k++ )
			{
				sWhere.Empty();
				if( k == m_pRealResultDM->GetRowCount() )
					sYData = _T("�հ�");
				else
				{
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				}

				// SELECT �ʵ� ����
				ids = SetDataMgrRef( m_pResultDM , saXFieldName.GetAt(i) , _T("0") , _T("N") , 0 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
				ids = SetDataMgrRef( m_pResultDM , saYFieldName.GetAt(i) , _T("0") , _T("N") , 1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
				
				// WHERE�� ����
				sSendWhere = sWhere;
				if( sYData.Compare(_T("�հ�"))==0 )
					ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
				else
				// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
				//ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , saYFieldName.GetAt(i) , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );
				{
					ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , saYFieldName.GetAt(i) , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );
					// ��Ŭ���� ������ ����
					if( m_pAxisCode_CellClick != NULL )
					{
						m_pAxisCode_CellClick->AddFieldNameVal( 1 , k , i , saYFieldName.GetAt(i) , sYData );
					}
				}
				// KOL.RED.2018.211 ---------------------------------------------------------------------------

				sSendWhere = _T(" B.REG_NO IN (") + sRegNoList +_T(")");
				if( !sWhere.IsEmpty() ) 
					sSendWhere += _T(" AND ") + sWhere ;

				ids = m_pResultDM->RefreshDataManager( sSendWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				// RESULT DM -> REAL RESULT DM ���� �̵�
				if( m_pXSumDM->GetRowCount() < k+1 )
					m_pXSumDM->InsertRow(-1);

				ids = m_pXSumDM->GetCellData( _T("�ߺ�����_�հ�") , k , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				ids = m_pResultDM->GetCellData( _T("�����") , 0 , sTmpResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
				
				ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , k );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				pTotalProgress->SetPos( (i+1)*(k+1)+(saTableName.GetSize()*m_pRealResultDM->RefList.GetCount()) );
			}	
		}
		sRegNoList.Empty();
	}

	// �ߺ� ��� �հ� ����
	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSum;
	CString sData;
	CString sTotalSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( INT i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;

			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		sTotalSum.Format( _T("%d") , _ttoi(sTotalSum) + _ttoi(sSum) );

		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);

		m_pXSumDM->SetCellData( sSumAlias , sSum , i);
		
	}

	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);

	m_pXSumDM->SetCellData( sSumAlias , sTotalSum , i );
	
	return 0;

	EFS_END
	return -1;

}

// ���� �հ� ����
INT CStaticsInfoMgr::YSum3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName , CStringArray &saYFieldName , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN


	INT ids;
	// ���� �հ� ����
	pTotalProgress->SetRange32( 0 , saTableName.GetSize()*(m_pRealResultDM->RefList.GetCount()+m_pRealResultDM->GetRowCount()) );

	m_pYSumDM->FreeData();

	CString sRegNoList;
	CString sRegNo;
	bool IsSend;
	//=====================================================
	//2008.11.24 UPDATE BY PJW : ��Ϲ�ȣ���� ������ 1000�� ���Ϸ� �ٲ۴�.
	// 	INT nLimitCnt = 50;
    INT nLimitCnt = 999;
	//=====================================================
	for( INT n = 0 ; n < pRegNoDM->GetRowCount() ; n++ )
	{
		IsSend = FALSE;

		ids = pRegNoDM->GetCellData( _T("��Ϲ�ȣ") , n , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

		if( !sRegNoList.IsEmpty() )
			if( !sRegNo.IsEmpty() )
				sRegNoList += _T(",");
		if( !sRegNo.IsEmpty() )
			sRegNoList += _T("'") + sRegNo + _T("'");

		if( (n!=0) && (n%nLimitCnt==0) ) IsSend = TRUE;
		if( n == pRegNoDM->GetRowCount()-1 ) IsSend = TRUE;

		if( !IsSend ) continue;

		if( sRegNoList.IsEmpty() ) continue;

		for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
		{
			// DM ���̺� �̸� �߰�
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);
			
			CStringArray saTableAlias;

			ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

			// 3.3. WHERE�� ���� 
			CString sWhere,sSendWhere;
			CString sTmpResult , sResult;
			CString sXData , sTmp;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				sWhere.Empty();
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

				// SELECT �ʵ� ����
				ids = SetDataMgrRef( m_pResultDM , saXFieldName.GetAt(i) , _T("0") , _T("N") , 0 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
				ids = SetDataMgrRef( m_pResultDM , saYFieldName.GetAt(i) , _T("0") , _T("N") , 1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );

				// WHERE�� ����
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , _T("") , sXData , _T("") , _T("N") , _T("N") , i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeTotalCount") );

				// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
				// ��Ŭ���� ������ ����
				if( m_pAxisCode_CellClick != NULL )
				{
					m_pAxisCode_CellClick->AddFieldNameVal( 0 , j-1 , i , saXFieldName.GetAt(i) , sXData );
				}
				// KOL.RED.2018.211 ---------------------------------------------------------------------------

				sSendWhere = _T(" B.REG_NO IN (") + sRegNoList + _T(")");
				if( !sWhere.IsEmpty() ) 
					sSendWhere += _T(" AND ") + sWhere ;

				ids = m_pResultDM->RefreshDataManager( sSendWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

				// RESULT DM -> REAL RESULT DM ���� �̵�
				// RESULT DM -> REAL RESULT DM ���� �̵�
				if( m_pYSumDM->GetRowCount() < j )
					m_pYSumDM->InsertRow(-1);

				ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , j-1 , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				ids = m_pResultDM->GetCellData( _T("�����") , 0 , sTmpResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
				
				ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , j-1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				pTotalProgress->SetPos( (i+1)*(j+1) );

			}
		}
		sRegNoList.Empty();
	}
	// �ߺ� ��� �հ� ����
	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSum;
	CString sData;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( INT i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);

		m_pYSumDM->SetCellData( sSumAlias , sSum ,i-1 );
	}	

	return 0;

	EFS_END
	return -1;

}

// ���� �հ� ����
INT CStaticsInfoMgr::XPriceSum3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName , CStringArray &saYFieldName , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	m_pXPriceSumDM->FreeData();

	CString sRegNoList;
	CString sRegNo;
	bool IsSend;
	//=====================================================
	//2008.11.24 UPDATE BY PJW : ��Ϲ�ȣ���� ������ 1000�� ���Ϸ� �ٲ۴�.
	// 	INT nLimitCnt = 50;
    INT nLimitCnt = 999;
	//=====================================================
	for( INT n = 0 ; n < pRegNoDM->GetRowCount() ; n++ )
	{
		IsSend = FALSE;

		ids = pRegNoDM->GetCellData( _T("��Ϲ�ȣ") , n , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

		if( !sRegNoList.IsEmpty() )
			if( !sRegNo.IsEmpty() )
				sRegNoList += _T(",");
		if( !sRegNo.IsEmpty() )
			sRegNoList += _T("'") + sRegNo + _T("'");

		if( (n!=0) && (n%nLimitCnt==0) ) IsSend = TRUE;
		if( n == pRegNoDM->GetRowCount()-1 ) IsSend = TRUE;

		if( !IsSend ) continue;

		if( sRegNoList.IsEmpty() ) continue;

		// �ߺ� ���� �հ� ����
		for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
		{
			// DM ���̺� �̸� �߰�
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);
			
			CStringArray saTableAlias;

			ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

			// 3.3. WHERE�� ���� 
			CString sWhere,sSendWhere;		
			CString sYData;
			CString sTmpResult , sResult;
			for( INT k = 0 ; k < m_pRealResultDM->GetRowCount()+1 ; k++ )
			{
				sWhere.Empty();
				if( k == m_pRealResultDM->GetRowCount() )
					sYData = _T("�հ�");
				else
				{
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				}

				// SELECT �ʵ� ����
				ids = SetDataMgrRef( m_pResultDM , saXFieldName.GetAt(i) , _T("0") , _T("N") , 0 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
				ids = SetDataMgrRef( m_pResultDM , saYFieldName.GetAt(i) , _T("0") , _T("N") , 1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
				ids = SetDataMgrRef( m_pResultDM , _T("SUM(PRICE)") , _T("0") , _T("Y") , 2 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
				
				// WHERE�� ����
				sSendWhere = sWhere;
				if( sYData.Compare(_T("�հ�"))==0 )
					ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
				else
					ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , saYFieldName.GetAt(i) , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );


				sSendWhere = _T(" B.REG_NO IN (") + sRegNoList +_T(")");
				if( !sWhere.IsEmpty() ) 
					sSendWhere += _T(" AND ") + sWhere ;

				ids = m_pResultDM->RefreshDataManager( sSendWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				// RESULT DM -> REAL RESULT DM ���� �̵�
				if( m_pXPriceSumDM->GetRowCount() < k+1 )
					m_pXPriceSumDM->InsertRow(-1);

				ids = m_pXPriceSumDM->GetCellData( _T("�ߺ�����_�հ�") , k , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				ids = m_pResultDM->GetCellData( _T("�����") , 0 , sTmpResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
				
				ids = m_pXPriceSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , k );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				pTotalProgress->SetPos( (i+1)*(k+1)+(saTableName.GetSize()*m_pRealResultDM->RefList.GetCount()) );
			}	
		}
		sRegNoList.Empty();
	}

	ids = SetDataMgrRef( m_pResultDM , _T("COUNT(*)") , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
	
	return 0;

	EFS_END
	return -1;

}

// ���� �հ� ����
INT CStaticsInfoMgr::YPriceSum3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName , CStringArray &saYFieldName , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;
	// ���� �հ� ����
	pTotalProgress->SetRange32( 0 , saTableName.GetSize()*(m_pRealResultDM->RefList.GetCount()+m_pRealResultDM->GetRowCount()) );

	m_pYPriceSumDM->FreeData();

	CString sRegNoList;
	CString sRegNo;
	bool IsSend;
	//=====================================================
	//2008.11.24 UPDATE BY PJW : ��Ϲ�ȣ���� ������ 1000�� ���Ϸ� �ٲ۴�.
	// 	INT nLimitCnt = 50;
    INT nLimitCnt = 999;
	//=====================================================
	for( INT n = 0 ; n < pRegNoDM->GetRowCount() ; n++ )
	{
		IsSend = FALSE;

		ids = pRegNoDM->GetCellData( _T("��Ϲ�ȣ") , n , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

		if( !sRegNoList.IsEmpty() )
			if( !sRegNo.IsEmpty() )
				sRegNoList += _T(",");
		if( !sRegNo.IsEmpty() )
			sRegNoList += _T("'") + sRegNo + _T("'");

		if( (n!=0) && (n%nLimitCnt==0) ) IsSend = TRUE;
		if( n == pRegNoDM->GetRowCount()-1 ) IsSend = TRUE;

		if( !IsSend ) continue;

		if( sRegNoList.IsEmpty() ) continue;

		for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
		{
			// DM ���̺� �̸� �߰�
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);
			
			CStringArray saTableAlias;

			ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

			// 3.3. WHERE�� ���� 
			CString sWhere,sSendWhere;
			CString sTmpResult , sResult;
			CString sXData , sTmp;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				sWhere.Empty();
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

				// SELECT �ʵ� ����
				ids = SetDataMgrRef( m_pResultDM , saXFieldName.GetAt(i) , _T("0") , _T("N") , 0 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
				
				ids = SetDataMgrRef( m_pResultDM , saYFieldName.GetAt(i) , _T("0") , _T("N") , 1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );

				ids = SetDataMgrRef( m_pResultDM , _T("SUM(PRICE)") , _T("0") , _T("Y") , 2 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );

				// WHERE�� ����
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , _T("") , sXData , _T("") , _T("N") , _T("N") , i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeTotalCount") );

				sSendWhere = _T(" B.REG_NO IN (") + sRegNoList + _T(")");
				if( !sWhere.IsEmpty() ) 
					sSendWhere += _T(" AND ") + sWhere ;

				ids = m_pResultDM->RefreshDataManager( sSendWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

				// RESULT DM -> REAL RESULT DM ���� �̵�
				// RESULT DM -> REAL RESULT DM ���� �̵�
				if( m_pYPriceSumDM->GetRowCount() < j )
					m_pYPriceSumDM->InsertRow(-1);

				ids = m_pYPriceSumDM->GetCellData( _T("�ߺ�����_�հ�") , j-1 , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				ids = m_pResultDM->GetCellData( _T("�����") , 0 , sTmpResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
				
				ids = m_pYPriceSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , j-1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				pTotalProgress->SetPos( (i+1)*(j+1) );

			}
		}
		sRegNoList.Empty();
	}

	ids = SetDataMgrRef( m_pResultDM , _T("COUNT(*)") , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );

	return 0;

	EFS_END
	return -1;

}

// �⺻���ǰ� ����� ���� +
INT CStaticsInfoMgr::GetDefaultCond( CString sStaticsClass ,CString &sDefaultCond ,CString &sUserCond  ,CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�⺻����") , sDefaultCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetDefaultCond") );
	
	sWhere = sDefaultCond;
	
	// ��������� �˻�
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����ڰ˻�����") , sUserCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetDefaultCond") );
	
	sDefaultCond.TrimLeft(); sDefaultCond.TrimRight(); sUserCond.TrimLeft(); sUserCond.TrimRight();

	// ����� �˻� ������ �������� �����Ѵ�.
	// ���̺� �̸� �����ϱ�
	CString sTableName;
	CString sInWhere,sOutWhere;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�˻����̺��̸�") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetDefaultCond") );
	ids = GetBoSearchWhere( sStaticsClass , sTableName , sInWhere , sOutWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetDefaultCond") );

	sWhere.Replace( _T("%������%") , sInWhere );	
	sWhere.Replace( _T("%������%") , sOutWhere );
	
	return 0;

	EFS_END
	return -1;

}



// ������ �����Ͽ� ���� Real DM�� ���� �ִ´�.
INT CStaticsInfoMgr::FillStaticsValue( CString sXFieldName, CString sYFieldName, CString sXData , CString sYData , CString sXGroupYN , CString sYGroupYN , CString sWhere , /*CString sUserCond ,*/ CProgressCtrl *pProgressCtrl )
{
	EFS_BEGIN

	OutputDebugString( _T("FillStaticsValue : ") +  sXFieldName + _T(" : ") + sYFieldName + _T(" : ") + sXData + _T(" : ") + sYData + _T(" \n"));

	INT ids;
	CString sInStaticWhere,sOutStaticWhere;	// �׷춫�� ���� ���� : %�ȱ׷�����% , %�۱׷�����%
	CString sSendWhere;							// ���� SQL��
	CString sGroupSql;							// Group By SQL��
	CString sTmpXData,sTmpYData;				
	CString sTmpXFieldName , sTmpYFieldName;

	CString sTableName = m_pResultDM->TBL_NAME;

	CStringArray saTableAlias;

	ids = MakeTableAliasArray( sTableName , saTableAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );


	if( sXGroupYN.Compare(_T("Y"))== 0 )
	{
		sTmpXData = _T("");
		sTmpXFieldName = _T("");
	}
	else
	{
		sTmpXData = sXData;
		sTmpXFieldName = sXFieldName;
	}

	if( sYGroupYN.Compare(_T("Y"))== 0 )
	{
		sTmpYData = _T("");
		sTmpYFieldName = _T("");
	}
	else
	{
		sTmpYData = sYData;
		sTmpYFieldName = sYFieldName;
	}


	// SELECT �ʵ� ����
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , sTmpXData , sXGroupYN ,  0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , sTmpYData , sYGroupYN , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunStatics") );

	// WHERE ���� ����
	sSendWhere = sWhere;

	/*
	ids = MakeStaticSql( saTableAlias ,sTmpXFieldName ,  sTmpXData , sTmpYFieldName , sTmpYData , sInStaticWhere , sOutStaticWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

	if( sUserCond.GetLength() > 6&& !sInStaticWhere.IsEmpty() ) 
		sInStaticWhere = CString( _T(" AND ")  + sInStaticWhere );
	if( sUserCond.GetLength() > 6&& !sOutStaticWhere.IsEmpty() ) 
		sOutStaticWhere = CString( _T(" AND ")  + sOutStaticWhere );

	sSendWhere.Replace( _T("%�ȱ׷�����%") , sInStaticWhere );
	sSendWhere.Replace( _T("%�۱׷�����%") , sOutStaticWhere );
	*/

	// GROUP BY ����
	if( sXGroupYN.Compare(_T("Y"))== 0 )
		sGroupSql += _T(" ") + sXFieldName + _T(" ");
	if( sYGroupYN.Compare(_T("Y"))== 0 )
	{
		if( !sGroupSql.IsEmpty() ) sGroupSql += _T(" , ");
		sGroupSql += _T(" ") + sYFieldName + _T(" ");
	}
	if( !sGroupSql.IsEmpty() ) sGroupSql = (_T(" GROUP BY ") + sGroupSql);

	sSendWhere += sGroupSql;

	// RESULT DM -> REAL RESULT DM ���� �̵�
	ids = GetDBAndMakeRealResultDM( sSendWhere , pProgressCtrl );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

	return 0;

	EFS_END
	return -1;

}


INT CStaticsInfoMgr::MakeTableAliasArray( CString sTableName , CStringArray &saTableAlias )
{
	EFS_BEGIN

	INT ids;

	CStringArray saTableArray;
	CStringArray saTempArray;
	
	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableArray );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTableAliasArray") );

	for( INT i = 0 ; i < saTableArray.GetSize() ; i++ )
	{
		ids = CLocCommonAPI::MakeArrayList( saTableArray.GetAt(i) , _T(" ") , saTempArray );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTableAliasArray") );

		if( saTempArray.GetSize() == 2 )
			saTableAlias.Add( saTempArray.GetAt(1) );
	}

	return 0;

	EFS_END
	return -1;

}

// ���̺� �̸��ӿ� ������ return 1 ������ return 0
// CO_TEST A , CD_TEST_B B 
// -> CO_TEST A , CO_TEST_B B �� ������.
// -> A
INT CStaticsInfoMgr::IsFieldFromTable( CString sFieldName , CStringArray &saTableArray )
{
	EFS_BEGIN

	INT ids;

	CStringArray saOneTableArray;
	CStringArray saOneTableArray2;
	CStringArray saFieldArray;
	bool IsIn = FALSE;

	// �ʵ� �̸��� ���̺� ������ ���� �ʴٸ� �׻� �������� �ȴ�.
	if( sFieldName.Find(_T(".")) < 0 )
		return 1;

	INT nRemoveIndex = sFieldName.Find(_T("("));

	if( nRemoveIndex > 0 )
		sFieldName = sFieldName.Mid( nRemoveIndex+1 , sFieldName.GetLength() - nRemoveIndex );

	sFieldName.TrimLeft(); sFieldName.TrimRight();

	ids = CLocCommonAPI::MakeArrayList( sFieldName , _T(".") , saFieldArray );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTableAliasArray") );


	INT nIndex;
	CString sFieldTbl;
	CString sOutTbl;
	for( INT i = 0 ; i < saTableArray.GetSize() ; i++ )
	{
		saOneTableArray.RemoveAll();
		CLocCommonAPI::MakeArrayList( saTableArray.GetAt(i) , _T(",") , saOneTableArray ); 



		for( INT j = 0 ; j < saOneTableArray.GetSize() ; j++ )
		{
			saOneTableArray2.RemoveAll();
			CLocCommonAPI::MakeArrayList( saOneTableArray.GetAt(j) , _T(" ") , saOneTableArray2 );

			nIndex = saOneTableArray2.GetSize()-1;

			sOutTbl = saOneTableArray2.GetAt(nIndex); sOutTbl.TrimLeft(); sOutTbl.TrimRight();
			sFieldTbl = saFieldArray.GetAt(0); sFieldTbl.TrimLeft(); sFieldTbl.TrimRight();

			if( sFieldTbl.Find(_T("(")) >= 0 )
			{
				CStringArray saRealFieldTablAlias;
				CLocCommonAPI::MakeArrayList( sFieldTbl , _T("(") , saRealFieldTablAlias );
				sFieldTbl = saRealFieldTablAlias.GetAt(saRealFieldTablAlias.GetSize()-1 );
			}
			
			// 2007.08.10 update by pdw
			
			if( sOutTbl.Compare(sFieldTbl) == 0 )
				IsIn = TRUE;
			
//			if( sFieldTbl.Compare(sOutTbl) >= 0 )
//				IsIn = TRUE;
		}
	}

	if( IsIn ) return 1;
	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::DeBugDM()
{
	EFS_BEGIN

	OnShowDMViewer(0);
	return 0;

	EFS_END
	return -1;

}


// ���� DM �ʱ�ȭ �ϱ�
INT CStaticsInfoMgr::MakeOrderStaticsDM( CString sStaticsClass , CProgressCtrl *pProgressCtrl,CProgressCtrl *pTotalProgressCtrl )
{
	EFS_BEGIN

	INT ids;

	CString sXAlias , sYAlias , sOption , sOrder;

	ids = GetXYAlias( sStaticsClass , sXAlias , sYAlias , sOption , sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStaticsDM") );

	// DM�� ������ ������ ������ ���� �޶�����.
	CStringArray asColumnName;
	if( sOption.Compare(_T("��ü")) ==  0 )
	{
		asColumnName.Add( sOption );
	}
	else
	{
		ids = GetColumnNames( sStaticsClass , sOption , asColumnName , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeOrderStaticsDM") );
	}

	m_nOrderStaticsDMCnt = asColumnName.GetSize();

	if( m_pOrderStaticsDM != NULL ) 
		delete[] m_pOrderStaticsDM;
	m_pOrderStaticsDM = NULL;

	// ���̺� �̸� �����ϱ�
	CStringArray saTableName;
	CString sTableName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�˻����̺��̸�") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );

	m_pOrderStaticsDM = new CESL_DataMgr*[saTableName.GetSize()+1];

	for( INT i = 0 ; i < saTableName.GetSize()+1 ; i++ )
	{
		m_pOrderStaticsDM[i] = new CESL_DataMgr[m_nOrderStaticsDMCnt];
	}

	// DB�� ������ �� ��������
	// 18.09.27 JMJ KOL.RED.2018.008
	if( _T("�ִ�����ڷ�") != sStaticsClass && _T("�ִ�����ڷ�(å�η�����)") != sStaticsClass )
	{
		ids = MakeOrderStatics( sStaticsClass , sXAlias , sOption , asColumnName , pProgressCtrl,pTotalProgressCtrl );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeOrderStaticsDM") );
		if( ids > 0 ) return ids;
	}
	else
	{
		ids = MakeBestLoanStatics( sStaticsClass , sXAlias , sOption , asColumnName , pProgressCtrl,pTotalProgressCtrl );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeOrderStaticsDM") );
		if( ids > 0 ) return ids;
	}

	// �����ϰ� ���� �ޱ��
	ids = MakeSumOrderStatics( sStaticsClass , saTableName.GetSize() );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeOrderStaticsDM") );

	return 0;

	EFS_END
	return -1;

}

// ���� DM �÷��� ���̺� �̸� ���� �߰��ϱ� , DB�� ������ ���� ��� ��������
INT CStaticsInfoMgr::MakeOrderStatics( CString sStaticsClass , CString sXAlias , CString sOption , CStringArray &asColumnName , CProgressCtrl *pProgressCtrl,CProgressCtrl *pTotalProgressCtrl)
{
	EFS_BEGIN

	INT ids;

	CString sTableName;	// ���̺� �̸�
	CString sWhere,sDefaultCond,sUserCond;		// �⺻ ����
	CStringArray saFieldAliasList;
	CStringArray asFieldAlias;	// �ʵ� �˸��ƽ� 
	CStringArray saTableName;
	CStringArray saFieldNameList;
	CStringArray saTableAlias;
	CString sResultField;	//�����
	CString sResultField2;
	CStringArray saWhere;
	CString sOrderBy;	// ����

	CString sTmpTableName;	// ���� ���̺� ( SELECT COUNT(*) , ... FROM ... WHERE .. GROUP BY .. ORDER BY.. �� ���� �Ǿ� �մ� )

	CESL_DataMgr *pDM;

	

	// ������ �����ϱ�
	// �⺻ ���ǿ� ����� ���Ǹ� ���� �ȴ�.
	ids = GetDefaultCond( sStaticsClass , sDefaultCond , sUserCond , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

	if( sUserCond.IsEmpty() )
	{
		AfxMessageBox( _T("���� ������ ���� �Ͻð� ��踦 �����Ͻʽÿ�.") );
		return 101;
	}
	CLocCommonAPI::MakeArrayList( sWhere , _T(";") , saWhere );

	// �ʵ� �˸��ƽ� ����
	ids = CLocCommonAPI::MakeArrayList( sXAlias , _T("/") , saFieldAliasList );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );


	// ���̺� �̸� �����ϱ�
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�˻����̺��̸�") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );
	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );


	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeOrderStatics") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����2") , sResultField2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeOrderStatics") );
	
	// 2007.09.03 add by pdw
	// �ִ�����ڷ� ���ɺ� ������ ����
	CString strOptionBuf = sOption;

	ids = GetCellData( m_pColumnAliasInfoDM , _T("�˸��ƽ���") , sOption , _T("�˻��ʵ��") , sOption );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetFieldName") );	

	CString sOrderNumber;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("��������") , sOrderNumber );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

	//++2008.09.02 DEL BY LKS {{++
//REMOVE�� 	if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
//REMOVE�� 		sOrderBy.Format( _T(" LIMIT 0,%s ") , sOrderNumber );
	//--2008.09.02 DEL BY LKS --}}
//	else
		sOrderBy.Format( _T(" ROWNUM <= %s ") , sOrderNumber );

	CString sFieldName , sGroupYN;

	CString sInWhere;
	sUserCond.Empty();
	ids = GetBoSearchWhere( sStaticsClass , saTableName.GetAt(0) , sUserCond , sInWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

	sUserCond += sInWhere;

	INT nIndex;
	pTotalProgressCtrl->SetRange32( 0 , saTableName.GetSize() );
	for( INT k = 0 ; k < saTableName.GetSize() ; k++ )
	{
		pDM = &m_pOrderStaticsDM[k][0];

		ids = MakeTableAliasArray( saTableName.GetAt(k) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		

		if( saFieldAliasList.GetSize()-1 < k ) nIndex = 0;
		else nIndex = k;
		ids = CLocCommonAPI::MakeArrayList( saFieldAliasList.GetAt(nIndex) , _T(",") , asFieldAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );
		INT nSumIndex = 0;
		CString strSumAlais = _T("");
		for( INT i = 0 ; i < m_nOrderStaticsDMCnt ; i++ )
		{
			sTmpTableName = _T(" SELECT ");

			// DB CONNECTION �ʱ�ȭ
			pDM->SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pInfo->MODE));


			// �÷� �����ϱ�
			pDM->InitDataMgrRef( asFieldAlias.GetSize()+1 );

			//++2008.09.02 DEL BY LKS {{++
//REMOVE�� 			if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
//REMOVE�� 				pDM->SetDataMgrRef( 0 , _T("����") , _T("") , _T("") , _T("") , _T("udf") );
			//--2008.09.02 DEL BY LKS --}}
			//else
				pDM->SetDataMgrRef( 0 , _T("����") , _T("ROWNUM") , _T("") , _T("") , _T("") );

			CString sGroupBy;
			for( INT j = 0 ; j < asFieldAlias.GetSize() ; j++ )
			{
				ids = GetFieldName( sStaticsClass , asFieldAlias.GetAt(j) , sFieldName , sGroupYN );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

				ids = CLocCommonAPI::MakeArrayList( sFieldName , _T("/") , saFieldNameList );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

				if( saFieldNameList.GetSize()-1 < k ) nIndex = 0;
				else nIndex = k;

				sFieldName = saFieldNameList.GetAt(nIndex);
				if( sFieldName.Find(_T("���������")) > -1 )
				{
					CString sWhere2;					
					sWhere2 = sUserCond;
					if( sWhere2.Mid(0,4).Compare( _T(" AND") ) != 0 )
						sWhere2 = _T(" AND ") + sWhere2;

					if( sOption.Compare(_T("��ü")) != 0 )
					{
						if( IsFieldFromTable( sOption , saTableAlias ) == 0 )
						{
							if( !asColumnName.GetAt(i).IsEmpty() )
								sWhere2 += _T(" AND ") + sOption + _T("= '") + asColumnName.GetAt(i) + _T("' ") ;
							else
								sWhere2 += _T(" AND ") + sOption + _T(" IS NULL ") ;
						}
					}

					sFieldName.Replace( _T("%���������%") , sWhere2 );
				}


				// SELECT�ؿ� �ʵ� ����
				if( j !=0 )
				{
					sTmpTableName += _T(",");
				}

				if( sFieldName.Find(_T("COUNT(")) >= 0 )
				{
					sTmpTableName += sFieldName + _T(" AS CNT");
					pDM->SetDataMgrRef( j+1 , asFieldAlias.GetAt(j) , _T("CNT") , _T("") , _T("") , _T("") );	
				}
				else
				{			
					CString sTmpFieldName;
					if( sFieldName.Find(_T(".")) < 0 )
						sFieldName = _T("CLU.") + sFieldName;

					if( sFieldName.Find(_T(".")) > -1 && (sFieldName.Find(_T(",")) > -1) )
						sTmpFieldName = sFieldName.Mid( sFieldName.Find(_T("."))+1 , sFieldName.Find(_T(",")) - sFieldName.Find(_T(".")) - 1 );
					else if( sFieldName.Find(_T(".")) > -1 )
					{
						// 2005.07.26
						// ����� �̿��ص� token�� �ǵ��� ����
						CString strTmp;
						strTmp = sFieldName;
						strTmp.Replace(_T("CLFI."), _T(""));
						strTmp.Replace(_T("CL."), _T(""));
						strTmp.Replace(_T("CLU."), _T(""));
						sTmpFieldName = strTmp;

						// 2005.07.26 REM BY PDJ
						//sTmpFieldName = sFieldName.Mid( sFieldName.Find(_T("."))+1 );
					}

					if ( sFieldName.Find(_T("$"),0) < 0 )
					{
						// WHERE�� ����
						sTmpTableName += _T("MAX(") + sFieldName + _T(") AS ") + sTmpFieldName;

						// 2007.07.10 ADD BY PDW
						// �ִ�����ڷ� ���� Ʃ�� ���� �����۾�
						// 18.09.27 JMJ KOL.RED.2018.008
						if( _T("�ִ�����ڷ�") == sStaticsClass || _T("�ִ�����ڷ�(å�η�����)") == sStaticsClass )
						{
							sTmpTableName = _T("SELECT MAX(CL.REC_KEY) AS REC_KEY ");
						}
					}
					else
					{
						// 2005.07.26 ADD BY PDJ
						// WHERE���� ��ǻ�����
						CString strFieldName, strFieldAlias;
						INT		nStartPos, nEndPos;
						
						nStartPos = sFieldName.Find(_T("("), 0 );	
						nEndPos = sFieldName.Find(_T(")"), 0 );							
						strFieldName = sFieldName.Mid(nStartPos+1, nEndPos-nStartPos-1);
						strFieldAlias =  strFieldName.Mid( strFieldName.Find(_T("."))+1 );
						sTmpTableName += _T("MAX(") + strFieldName + _T(") AS ") + strFieldAlias;
					}
					// DM ����
					// 2005.07.30 ADD BY PDJ 
					// Function�� �̿����� ��� Column Alias�� �̸� ����
					if ( asFieldAlias.GetAt(j).FindOneOf(_T("$")) > -1 )
					{
						CString strFieldAlias;
						MakeFunctionFieldName(asFieldAlias.GetAt(j), strFieldAlias);
						pDM->SetDataMgrRef( j+1 , strFieldAlias , sTmpFieldName , _T("") , _T("") , _T("") );
					}
					else
					{
						pDM->SetDataMgrRef( j+1 , asFieldAlias.GetAt(j) , sTmpFieldName , _T("") , _T("") , _T("") );
					}
				}
			}
			
			CString sSendWhere = saWhere.GetAt(k);
			if( IsFieldFromTable( sOption , saTableAlias ) == 1)
			{
				if( sOption.Compare(_T("��ü")) != 0 )
				{
					if( sOption.Find(_T(".")) < 0 )
						sOption = _T("CLU.") + sOption;

					if( !sWhere.IsEmpty() ) sSendWhere += _T(" AND ");
						if( !asColumnName.GetAt(i).IsEmpty() )
							sSendWhere += sOption + _T("= '") + asColumnName.GetAt(i) + _T("' ") ;
						else
							sSendWhere += sOption + _T(" IS NULL ") ;
				}
			}

			// 2007.09.03 add by pdw
			// �ִ�����ڷ� ���ɺ� ������ ����
			// 18.09.27 JMJ KOL.RED.2018.008
			//if(_T("�ִ�����ڷ�") == sStaticsClass && _T("���ɺ�") == strOptionBuf )
			if((_T("�ִ�����ڷ�") == sStaticsClass && _T("���ɺ�") == strOptionBuf) || (_T("�ִ�����ڷ�(å�η�����)") == sStaticsClass && _T("���ɺ�") == strOptionBuf) )
			{
				if( !sWhere.IsEmpty() ) sSendWhere += _T(" AND ");
						
				if( !asColumnName.GetAt(i).IsEmpty() )
				{
					sSendWhere += _T(" (SELECT ") + sOption + _T(" FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY=CL.USER_KEY) = '") + asColumnName.GetAt(i) + _T("' ") ;
				}
				else
				{					
					sSendWhere += _T(" (SELECT ") + sOption + _T(" FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY=CL.USER_KEY) IS NULL ") ;
				}
			}

			// �̰Ÿ� where ���� ������ ��
			pDM->QueryLog( sSendWhere.GetBuffer( 0 ) );
			
			////////////////////////////////////////////////////////////////////////////////////////////////////
			// ���ɾ� �̾��ؿ�. ���ɾ� ����� ���ظ� ���ؼ� ��Ÿ� �׳� �䷸�� ���鲲��.
			// ���ɾ� ��谡 �ʹ� �������... -,.-;;....
			// ���� ����
			// 2004-04-09
			////////////////////////////////////////////////////////////////////////////////////////////////////
			// (2004-0915_M_HAN...)�� �̷��� ������ �𸣰���. �� �κ� ������ '�ִ�����ڷ�'�� ������ 
			// �׻� 100������ ���� �ο��� ����. �� �κ��� �����, ���� �������� ���󺹱Ͱ� �� �� ����
			////////////////////////////////////////////////////////////////////////////////////////////////////
			//if( _T("�ִ�����ڷ�") == sStaticsClass && !( m_pInfo->MODE == 10000 || (m_pInfo->MODE == 30000) ) )
			//{
			//	CString query;
			//	query.Format(
			//				_T("SELECT /*+ ORDERED USE_NL( V, C ) INDEX( C CO_LOAN_TBL_PK ) */ ")
			//				_T("ROWNUM, C.REG_NO, C.TITLE, C.AUTHOR, C.PUBLISHER, SUBSTR( C.CALL_NO, 1, 20 ), V.CNT ")
			//				_T("FROM ( ")
			//				_T("	 SELECT CL.BOOK_KEY AS BOOKKEY, MAX( CL.REC_KEY ) AS CLKEY, COUNT( * ) AS CNT FROM CO_LOAN_USER_TBL CLU, LH_STORAGE_V01 CL ")
			//				_T("	 WHERE ")
			//				_T(" %s ")
			//				_T("	 GROUP BY CL.BOOK_KEY ORDER BY CNT DESC ) V,")
			//				_T("	 LH_STORAGE_V01 C ")
			//				_T("WHERE C.REC_KEY = V.CLKEY AND ROWNUM <= 100 "),
			//				sSendWhere );
			//
			//	pDM->RestructDataManager( query, pProgressCtrl );
			//}
			//else
			////////////////////////////////////////////////////////////////////////////////////////////////////
			if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
			{
				CString sOrder;
				for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
				{
					sOrder.Format(_T("%d") , i+1 );
					pDM->SetCellData(_T("����") , sOrder , i );
				}
			}

			if ( !(m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000) )
			{				
				// Group By ����
				sSendWhere += _T(" GROUP BY ") + sResultField2;
				
				// Order By ����
				// 2007.07.10 update by pdw 
				// �ִ�����ڷ� ��� Ʃ�� ���� order by ����
				// 18.09.27 JMJ KOL.RED.2018.008
				//if ( _T("�ִ�����ڷ�") == sStaticsClass ) //��Ϲ�ȣ�� ���� �߰�_050420	
				if ( _T("�ִ�����ڷ�") == sStaticsClass || _T("�ִ�����ڷ�(å�η�����)") == sStaticsClass ) 
					sSendWhere += _T(" ORDER BY CNT DESC");
				else if ( _T("�ٵ���") == sStaticsClass ) //����ڹ�ȣ�� ���� �߰�_050420	
					sSendWhere += _T(" ORDER BY CNT DESC, USER_NO");
				else 
					sSendWhere += _T(" ORDER BY CNT DESC");
				
				sTmpTableName += _T(" FROM ") + saTableName.GetAt(k) + _T(" WHERE ") + sSendWhere;
				// 2007.07.10 ADD BY PDW
				// �ִ�����ڷ� ���� Ʃ�� ���� �����۾�
				// 18.09.27 JMJ KOL.RED.2018.008
				if( _T("�ִ�����ڷ�") != sStaticsClass && _T("�ִ�����ڷ�(å�η�����)") != sStaticsClass )
				{
					sTmpTableName = _T("(") + sTmpTableName +_T(") V");
				}
				
				// 2007.07.10 ADD BY PDW
				// �ִ�����ڷ� ���� Ʃ�� ���� �����۾�
				// 18.09.27 JMJ KOL.RED.2018.008
				if( _T("�ִ�����ڷ�") == sStaticsClass || _T("�ִ�����ڷ�(å�η�����)") == sStaticsClass )
				{
					// 2007.08.20 ADD BY PDW
					// ���� Ʃ��
					// ( SELECT * FROM  ( K) WHERE ROWNUM <= 10 ) V
					sTmpTableName = _T("( SELECT * FROM ( ") + sTmpTableName + _T(" ) WHERE ") + sOrderBy + _T(" ) V ");
//					sOrderBy = _T(" CL2.REC_KEY=V.REC_KEY ");
					sTmpTableName = _T("LH_STORAGE_V01 CL2,") + sTmpTableName;
				}
				
				// ���̺� �����ϱ�
				pDM->TBL_NAME = sTmpTableName;
				
				if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000  )
				{
					// 18.09.27 JMJ KOL.RED.2018.008
					if( _T("�ִ�����ڷ�") == sStaticsClass || _T("�ִ�����ڷ�(å�η�����)") == sStaticsClass )
					{ 
						pDM->EXTRA_CONDITION = _T(" CL2.REC_KEY=V.REC_KEY ");
					}
					else
					{
						pDM->EXTRA_CONDITION = sOrderBy;
					}
				}
				else
				{
					// 18.09.27 JMJ KOL.RED.2018.008
					if( _T("�ִ�����ڷ�") == sStaticsClass || _T("�ִ�����ڷ�(å�η�����)") == sStaticsClass )
					{
						pDM->CONDITION = _T(" CL2.REC_KEY=V.REC_KEY ");
					}
					else
					{
						pDM->CONDITION = sOrderBy;
					}					
				}
				
				// DB�� ������
				ids = pDM->RefreshDataManager( _T("") , pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitOrderStaticsDM") );
								
				// 2011.09.09 ADD : 3000Api
				if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
				{
					BOOL bCheckConnect = FALSE;
					CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
					if(NULL != pApi)
					{
						if(TRUE == pApi->CheckConnection())
						{
							INT nC, nR, nCIdx, nRIdx;
							CString strColumnAlias;
							CString strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType;
							nR = pDM->GetRowCount();
							nC = pDM->GetColCount();						
							for(nCIdx = 0; nCIdx < nC; nCIdx++)
							{
								pDM->GetColumnRefInfo(nCIdx, strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType);
								if( (sStaticsClass == _T("�ٵ���") || sStaticsClass == _T("��ü�����")) &&	(strFieldName == _T("H_ADDR1") || strFieldName == _T("H_PHONE") || strFieldName == _T("HANDPHONE")) )
								{
									CStringArray DataArray;
									for(nRIdx = 0; nRIdx < nR; nRIdx++)
									{
										pDM->GetCellData(nRIdx, nCIdx, strFieldValue);
										DataArray.Add(strFieldValue);
									}
									if(TRUE == pApi->ConnectSocket())
									{
										if(TRUE == pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), strFieldName, DataArray))
										{
											for(nRIdx = 0; nRIdx < nR; nRIdx++)
											{
												pDM->SetCellData(nRIdx, nCIdx, DataArray.GetAt(nRIdx));
											}
										}
										pApi->CloseSocket();
									}
									DataArray.RemoveAll(); DataArray.FreeExtra();
								}
							}
						}
						delete pApi;
						pApi = NULL;
					}
				} // END IF Lib-Secu
			}

			pDM->EXTRA_CONDITION = _T("");
			pDM->CONDITION = _T("");

			// �ڵ� -> ����
// 			m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("����Ҽ�����") , _T("�Ҽ�") );
			// 2009.10.12 ADD BY PWR : �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ��
			for(INT k = 0; k < pDM->GetRowCount(); k++)
			{
				// �ڵ�->����
				CString strCode;
				pDM->GetCellData(_T("�Ҽ�"), k, strCode);
				m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����Ҽ�����"), strCode, strCode);
				pDM->SetCellData(_T("�Ҽ�"), strCode, k);
				// �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ��
				CString strUserNo;
				pDM->GetCellData(_T("�����ڹ�ȣ"), k, strUserNo);
				if(_T("Y") != m_sIsCivilNoHide )
				{
					// 2010.09.08 ADD BY KSJ : ���յ������� ��뿩�ο� ���� �ֹι�ȣ ǥ�ù�� ����
//DEL 					CString strBuf = strUserNo;
//DEL 					if(13 == strUserNo.GetLength() )
//DEL 					{
//DEL 						strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 					}				
//DEL 					ids = CLocCommonAPI::CivilNoCheck(strBuf);
//DEL 					if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );
//DEL 					if(0 == ids)
//DEL 					{
//DEL 						// 2009.12.14 ADD BY CJY : �ֹι�ȣ ǥ�ù�� ����
//DEL 						strUserNo.Format(_T("%s%s******"),strBuf.Left(6),strBuf.Mid(7,1));
//DEL 						if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );
//DEL 					}
					CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
				}
				pDM->SetCellData(_T("�����ڹ�ȣ"), strUserNo, k);
			}

			if( i != m_nOrderStaticsDMCnt-1 ) pDM++;
		}
		pTotalProgressCtrl->SetPos( (k+1) );		
	}


	return 0;

	EFS_END
	return -1;

}

///{{++
/// 2008.04.08 add by pdw
/// �ִ�����ڷ� ���� ������ ������ �����Ѵ�
/// �����ڷḦ ���� ��� ���� ���� BO_BOOK_TBL, SE_BOOK_TBL�� �ʿ��ϴ�
/// ��� ���°� ���۷����� ������ �� ���� ������ 
/// �ҽ��ȿ� ���� ���״�
/*
SELECT ROWNUM, REG_NO, TITLE, AUTHOR, PUBLISHER, CALL_NO, CNT  FROM ( SELECT *
           FROM ( SELECT MAX(CL.REG_NO) AS REG_NO,MAX(CL.TITLE) AS TITLE,MAX(CL.AUTHOR) AS AUTHOR,MAX(CL.PUBLISHER) AS PUBLISHER,MAX(CL.CALL_NO) AS CALL_NO,
                         COUNT(1) AS CNT
                    FROM LH_STORAGE_V01 CL,
                         BO_BOOK_TBL BB
                   WHERE CL.BOOK_KEY=BB.REC_KEY
                     AND CL.STATUS IN ('0','1','2','5','6')
                     AND CL.LOAN_KEY IS NULL
                     AND CL.CLASS_NO_TYPE='1'
                     AND CL.MANAGE_CODE='MA'
                     AND CL.PUBLISH_FORM_CODE='MO'
                     AND CL.LOAN_DATE BETWEEN TO_DATE('2008/01/07 00:00:00','YYYY/MM/DD HH24:MI:SS')
                                          AND TO_DATE('2008/04/07 00:00:00','YYYY/MM/DD HH24:MI:SS')
                   GROUP BY BB.SPECIES_KEY, BB.VOL_CODE
                  UNION 
                  SELECT MAX(CL.REG_NO) AS REG_NO,MAX(CL.TITLE) AS TITLE,MAX(CL.AUTHOR) AS AUTHOR,MAX(CL.PUBLISHER) AS PUBLISHER,MAX(CL.CALL_NO) AS CALL_NO,
                         COUNT(1) AS CNT
                    FROM LH_STORAGE_V01 CL,
                         SE_BOOK_TBL BB
                   WHERE CL.BOOK_KEY=BB.REC_KEY
                     AND CL.STATUS IN ('0','1','2','5','6')
                     AND CL.LOAN_KEY IS NULL
                     AND CL.CLASS_NO_TYPE='1'
                     AND CL.MANAGE_CODE='MA'
                     AND CL.PUBLISH_FORM_CODE='SE'
                     AND CL.LOAN_DATE BETWEEN TO_DATE('2008/01/07 00:00:00','YYYY/MM/DD HH24:MI:SS')
                                          AND TO_DATE('2008/04/07 00:00:00','YYYY/MM/DD HH24:MI:SS')
                   GROUP BY BB.SPECIES_KEY, BB.VOL_CODE
                ) ORDER BY CNT DESC ) WHERE ROWNUM <=10
*/
INT CStaticsInfoMgr::MakeBestLoanStatics( CString sStaticsClass , CString sXAlias , CString sOption , CStringArray &asColumnName , CProgressCtrl *pProgressCtrl,CProgressCtrl *pTotalProgressCtrl)
{
EFS_BEGIN

	INT		ids;

	///{{++	
	/// SELECT FIELD ����
	/// 1. VIEW ���̺� ���Ǵ� SELECT FIELD�� �����
	/// 2. ���� SELECT FIELD�� �����	
	CStringArray	saFieldAlias;
	CStringArray    saFieldNames;
	INT				nFieldAliasCount;
	CString         strFieldName;
	CString			strFieldAlias;
	CString			strGroupYN;
	CString			strViewSelectField;
	CString			strStaticSelectField;	

	ids = CLocCommonAPI::MakeArrayList(sXAlias, _T(","), saFieldAlias);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("MakeBestLoanStatics") );
	}

	nFieldAliasCount = saFieldAlias.GetSize();

	for( INT i=0; i<nFieldAliasCount; i++ )
	{
		ids = GetFieldName(sStaticsClass, saFieldAlias.GetAt(i), strFieldName, strGroupYN);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("MakeBestLoanStatics") );
		}

		/// COUNT(*)�� ���� ó�� �Ѵ�
		if( -1 < strFieldName.Find(_T("COUNT")) )
		{
			strFieldAlias = _T("CNT");			
		}
		else
		{
			strFieldAlias = strFieldName;			

			if( -1 < strFieldName.Find(_T(",")) )
			{
				strFieldAlias = strFieldAlias.Mid( strFieldAlias.Find(_T("."))+1 , strFieldAlias.Find(_T(",")) - strFieldAlias.Find(_T("."))-1 );
				strFieldName  = _T("CL.") + strFieldAlias;
			}
			else
			{
				strFieldAlias.Replace(_T("CL."),_T(""));
			}

			strFieldName = _T("MAX(") + strFieldName + _T(")");			
		}

		if( TRUE != strViewSelectField.IsEmpty() )
		{
			strViewSelectField += _T(",");
		}
		
		/// MAX �Լ��� �ٿ��� �� �˸��ƽ� ó���� �Ѵ�
		strViewSelectField += ( strFieldName + _T(" AS ") + strFieldAlias );

		if( TRUE != strStaticSelectField.IsEmpty() )
		{
			strStaticSelectField += _T(",");
		}
		
		strStaticSelectField += strFieldAlias;

		saFieldNames.Add(strFieldAlias);
	}

	/// ROWNUM �߰�
	strStaticSelectField = _T("ROWNUM,") + strStaticSelectField;
	///}}--

	///{{++
	/// WHERE ���� �����Ѵ�
	CString	strWhere;
	CString	strDefaultCondition;
	CString	strUserCondition;
	CString	strTableName;

	/// ���̺� �̸� �����ϱ�
	ids = GetCellData(m_pStaticsMainInfoDM, _T("�������"), sStaticsClass, _T("�˻����̺��̸�"), strTableName);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("MakeBestLoanStatics") );
	}

	/// �⺻������ �����ϱ�
	ids = GetDefaultCond(sStaticsClass, strDefaultCondition, strUserCondition, strWhere);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("MakeBestLoanStatics"));
	}

	if( TRUE == strUserCondition.IsEmpty() )
	{
		AfxMessageBox(_T("���� ������ ���� �Ͻð� ��踦 �����Ͻʽÿ�."), MB_ICONINFORMATION);
		return 101;
	}
	///}}--

	///{{++
	/// GROUP BY ����
	CString strResult;
	CString strGroupBy;

	ids = GetCellData(m_pStaticsMainInfoDM, _T("�������"), sStaticsClass, _T("�����"), strResult);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("MakeBestLoanStatics") );
	}

	ids = GetCellData(m_pStaticsMainInfoDM, _T("�������"), sStaticsClass, _T("�����2"), strGroupBy);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("MakeBestLoanStatics") );
	}
	///}}--

	///{{++
	/// ROWNUM ���� ����
	CString strRowNum;
	
	ids = GetCellData(m_pStaticsMainInfoDM, _T("�������"), sStaticsClass, _T("��������"), strRowNum);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("MakeBestLoanStatics") );
	}

	strRowNum = _T(" ROWNUM <= ") + strRowNum;
	///}}--
	
	///{{++
	/// ��� ���� ����
	CString strUnitFieldName;

	ids = GetCellData(m_pColumnAliasInfoDM, _T("�˸��ƽ���"), sOption, _T("�˻��ʵ��"), strUnitFieldName);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("MakeBestLoanStatics") );
	}

	/// ������ ������ ó���Ѵ�
	if( _T("���ɺ�") == sOption )
	{		
		strUnitFieldName = _T("(SELECT ") + strUnitFieldName + _T(" FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY=CL.USER_KEY)");
	}
	///}}--

	///{{++
	/// ������ ���� ó�� �Ѵ�
	CString strBoTable;
	CString strSeTable;
	CString strViewTable;
	CString strQuery;

	CESL_DataMgr* pDM = NULL;
	CString       strUnitCondition;

	pTotalProgressCtrl->SetRange32(0 , m_nOrderStaticsDMCnt);

	
	for( i=0 ; i<m_nOrderStaticsDMCnt ; i++ )
	{
		/// DM ����
		pDM = &m_pOrderStaticsDM[0][i];
		pDM->SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pInfo->MODE));

		/// DM �÷� ����
		pDM->InitDataMgrRef(nFieldAliasCount+1);

		pDM->SetDataMgrRef(0, _T("����"), _T("ROWNUM"), _T(""), _T(""), _T(""));

		INT j;
		for( j=0 ; j<nFieldAliasCount ; j++ )
		{
			pDM->SetDataMgrRef(j+1, saFieldAlias.GetAt(j), saFieldNames.GetAt(j), _T(""), _T(""), _T(""));
		}


		if( _T("��ü") != sOption )
		{
			if( TRUE == asColumnName.GetAt(i).IsEmpty() )
			{
				strUnitCondition = strUnitFieldName + _T(" IS NULL AND ");
			}
			else 
			{
				strUnitCondition = strUnitFieldName + _T("='") + asColumnName.GetAt(i) +_T("' AND ");
			}

			// 2009.08.07 UPDATE BY KSJ : �⺻ Where ���� AND ������ �����Ǿ� ������ ���� �Ǵ� ������ ���Ͽ� DELETE
			//strWhere += (_T(" AND ") + strUnitCondition);
		}

		// 2009.08.07 UPDATE BY KSJ : �⺻ Where ���� AND ������ �����Ǿ� ������ ���� �Ǵ� ������ ���Ͽ� DELETE
		//DEL 		strBoTable.Format(_T("SELECT %s FROM %s, BO_BOOK_TBL BB ")
		//DEL 			              _T("WHERE %s ")
		//DEL 						  _T("AND CL.PUBLISH_FORM_CODE='MO' ")
		//DEL 						  _T("GROUP BY %s "), strViewSelectField, strTableName, strWhere, strGroupBy);
		//DEL 
		//DEL 		strSeTable.Format(_T("SELECT %s FROM %s, SE_BOOK_TBL BB ")
		//DEL 			              _T("WHERE %s ")
		//DEL 						  _T("AND CL.PUBLISH_FORM_CODE='SE' ")
		//DEL 						  _T("GROUP BY %s "), strViewSelectField, strTableName, strWhere, strGroupBy);
		strBoTable.Format(_T("SELECT %s FROM %s, BO_BOOK_TBL BB ")
						  _T("WHERE %s ")
						  _T("AND %s")
						  _T("CL.PUBLISH_FORM_CODE='MO' ")
						  _T("GROUP BY %s "), strViewSelectField, strTableName, strWhere, strUnitCondition , strGroupBy);

		strSeTable.Format(_T("SELECT %s FROM %s, SE_BOOK_TBL BB ")
						  _T("WHERE %s ")
						  _T("AND %s")
						  _T("CL.PUBLISH_FORM_CODE='SE' ")
						  _T("GROUP BY %s "), strViewSelectField, strTableName, strWhere, strUnitCondition , strGroupBy);		

		strViewTable.Format(_T("SELECT * FROM (%s UNION %s) ORDER BY CNT DESC"), strBoTable, strSeTable);

		strQuery.Format(_T("SELECT %s FROM (%s) WHERE %s"), strStaticSelectField, strViewTable, strRowNum);

		// 2009.08.07 DELETE BY KSJ : Query �α״� RestructDataManager ���� �ϸ鼭 ����ϰԵ�. �ߺ� ������� ���Ͽ� DELETE
		/// �α� ����
//DEL 		pDM->QueryLog(strQuery.GetBuffer(0));

		ids = pDM->RestructDataManager(strQuery);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("MakeBestLoanStatics") );
		}

		pTotalProgressCtrl->SetPos(i);		
	}
	///}}--

	return 0;

EFS_END
return -1;
}
///}}--

INT CStaticsInfoMgr::ViewTab( CString sStaticsClass , CTabCtrl *pTab )
{
	EFS_BEGIN

	INT ids;


	CString sXAlias , sYAlias , sOption , sOrder;

	ids = GetXYAlias( sStaticsClass , sXAlias , sYAlias , sOption , sOrder );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ViewTab") );
	}

	// DM�� ������ ������ ������ ���� �޶�����.
	CStringArray asColumnName;
	if( 0 == sOption.Compare(_T("��ü")) )
	{
		asColumnName.Add( sOption );
	}
	else
	{
		ids = GetColumnNames( sStaticsClass , sOption , asColumnName );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ViewTab") );
		}
	}

	pTab->DeleteAllItems();
	INT i;
	for( i = 0 ; i < asColumnName.GetSize() ; i++ )
	{
		pTab->InsertItem( i , asColumnName.GetAt(i) );
	}

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::OrderStaticsView( CString sStaticsClass , CMSHFlexGrid *pGrid , INT nIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM;

	CStringArray saTableName;
	CString sTableName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�˻����̺��̸�") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

	CString sOrderNumber;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("��������") , sOrderNumber );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );

	pDM = &m_pOrderStaticsDM[saTableName.GetSize()][nIndex];	

	// �׸��忡 �ѷ��ش�.
	pGrid->Clear();		

	if ( pDM->GetRowCount() == 0 ) 
	{
		pGrid->SetRows( 2 );
		pGrid->SetRedraw(TRUE);
		return 0;
	}
	
	INT nViewRowCnt = pDM->GetRowCount();
	if( nViewRowCnt > _ttoi(sOrderNumber) )
		nViewRowCnt = _ttoi(sOrderNumber);


	pGrid->SetRows( nViewRowCnt+1 );
	pGrid->SetCols( 0 , pDM->RefList.GetCount() );
	if( pDM->GetRowCount() < 2 ) 
		pGrid->SetRows( 2 );

	// X�� �÷� �ѷ��ֱ�
	INT refcount = pDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = pDM->RefList.GetHeadPosition();
	CString query, fields, isudf;


	CString sResult;
	CString strAlias;	

	for( INT i = 0 ; i < pDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)pDM->RefList.GetNext(pos);

		strAlias = pRef->FIELD_ALIAS;
		if ( strAlias.Find(_T("$"), 0 ) > -1 )
		{	
			MakeFunctionFieldName(strAlias, strAlias);
		}
		
		pGrid->SetTextMatrix( 0 , i ,  strAlias );
		

		for( INT j = 0 ; j < nViewRowCnt ; j++ )
		{
			ids = pDM->GetCellData( pRef->FIELD_ALIAS , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );
			pGrid->SetTextMatrix( j+1 , i , sResult );
		}

	}

	pGrid->SetFixedCols(1);
	pGrid->SetFixedRows(1);

	
	// �׸��� ���� �����ϱ�
	// ���� �߰�
	for( i = 0; i < pGrid->GetCols( 0 ); i++ )
		pGrid->SetColAlignment( i, 1 );
	
	pGrid->SetRedraw(TRUE);
	pGrid->Refresh();	

	return 0;

	EFS_END
	return -1;

}

// �������� DM�� �踦 ���ļ� ���ı��� �ϰ� �������� �ű��.
INT CStaticsInfoMgr::MakeSumOrderStatics( CString sStaticsClass , INT nTableCnt )
{
	EFS_BEGIN

	INT ids;

	CString sResultAlias;
	// �� ��ġ��
	INT nTargetIndex = nTableCnt;
	
	// ����
	CESL_DataMgr *pTargetDM;
	CESL_DataMgr *pSrcDM;
	CString sTargetData;
	CString sSrcData;
	CString sAlias;

	// ��� �˸��ƽ�
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeSumOrderStatics") );

	CESL_DataMgr *pTestDM;
	pTestDM = new CESL_DataMgr[11];

	// �÷� ����
	CString sFieldAlias,sFieldName,sFieldType,sInitValue,sDataType;
	for( INT i = 0 ; i < m_nOrderStaticsDMCnt ; i++ )
	{
		pTargetDM = &m_pOrderStaticsDM[nTargetIndex][i];

		pSrcDM = &m_pOrderStaticsDM[0][i];

		// DB CONNECTION �ʱ�ȭ
		pTargetDM->SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pInfo->MODE));

		pTargetDM->InitDataMgrRef( pSrcDM->RefList.GetCount() );
		pTargetDM->m_nCols = pSrcDM->RefList.GetCount();

		POSITION pos;
		CESL_DataMgr::reference *Ref;
		for( INT k = 0 ; k < pSrcDM->RefList.GetCount() ; k++ )
		{
			pos = pSrcDM->RefList.FindIndex(k);
			Ref = (CESL_DataMgr::reference*)(pSrcDM->RefList.GetAt(pos));

			sFieldAlias = Ref->FIELD_ALIAS;
			sFieldType = _T("str");
			if( sFieldAlias.Compare( sResultAlias ) == 0 )
				sFieldType = _T("int");

			pTargetDM->SetDataMgrRef( k , sFieldAlias , _T("") , sFieldType  , _T("")  , sFieldType );			
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSumOrderStatics") );
		}		
	}

	// ����Ÿ ����
	for( i = 0 ; i < m_nOrderStaticsDMCnt ; i++ )
	{
		pTargetDM = &m_pOrderStaticsDM[nTargetIndex][i];

		for( INT j = 0 ; j < nTableCnt   ; j++ )
		{
			pSrcDM = &m_pOrderStaticsDM[j][i];

			for( INT k = 0 ; k < pSrcDM->RefList.GetCount() ; k++ )
			{
				CESL_DataMgr::reference *Ref;
				POSITION pos = pSrcDM->RefList.FindIndex(k);
				Ref = (CESL_DataMgr::reference*)(pSrcDM->RefList.GetAt(pos));
				sAlias = Ref->FIELD_ALIAS;
				for( INT m = 0 ; m < pSrcDM->GetRowCount() ; m++ )
				{
					if( pTargetDM->GetRowCount() < m+1 )
						pTargetDM->InsertRow(-1);

					ids = pSrcDM->GetCellData( sAlias , m , sSrcData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSumOrderStatics") );

					ids = pTargetDM->GetCellData( sAlias , m , sTargetData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSumOrderStatics") );

					if( sAlias.Compare(sResultAlias) == 0 )
						sTargetData.Format( _T("%d") , _ttoi(sTargetData)+_ttoi(sSrcData) );
					else
						sTargetData = sSrcData;

					sTargetData.Replace( _T("\r\n") , _T(" ") );
					sTargetData.Replace( _T("\n") , _T(" ") );

					ids = pTargetDM->SetCellData( sAlias , sTargetData , m );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeSumOrderStatics") );

				}
			}
		}	
	}

	return 0;

EFS_END
return -1;
}

// ���� �����Ѵ�.
INT CStaticsInfoMgr::GridDeleteRow( INT nIndex , CMSHFlexGrid *pGrid )
{
	EFS_BEGIN

	INT nCols = pGrid->GetCols(0);
	INT nRows = pGrid->GetRows();

	CString sData;

	for( INT j = nIndex ; j < nRows-1 ; j++ )
	{
		for( INT i = 0 ; i < nCols ; i++ )
		{
			sData = pGrid->GetTextMatrix( j+1 , i );
			pGrid->SetTextMatrix( j , i , sData );
		}
	}


	pGrid->SetRows( pGrid->GetRows()-1 );

	return 0;

	EFS_END
	return -1;

}

// ���� �����Ѵ�.
INT CStaticsInfoMgr::GridDeleteCol( INT nIndex , CMSHFlexGrid *pGrid )
{
	EFS_BEGIN

	INT nRows = pGrid->GetRows();
	INT nCols = pGrid->GetCols(0);

	CString sData;

	for( INT j = nIndex ; j < nCols-1 ; j++ )
	{
		for( INT i = 0 ; i < nRows ; i++ )
		{
			sData = pGrid->GetTextMatrix( i , j+1 );
			pGrid->SetTextMatrix( i , j  , sData );
		}
	}

	pGrid->SetCols( 0 , pGrid->GetCols(0)-1 );

	return 0;

	EFS_END
	return -1;

}

// �з���ȣ����
INT CStaticsInfoMgr::ConvertClassNo(CString sStaticsClass ,  CString sCode,CString &sDesc , CString sClassNoType )
{
	EFS_BEGIN

 	INT ids;

	if( sClassNoType.IsEmpty() )
		return 0;
//		sClassNoType = _T("�з���ȣ��");

	CStringArray asColumnCode;
	CStringArray asColumnName;
	ids = GetColumnNames( sStaticsClass , sClassNoType, asColumnCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("ConvertClassNo") );
	ids = GetColumnNames( sStaticsClass , sClassNoType , asColumnName , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("ConvertClassNo") );

	for( INT i = 0 ; i < asColumnName.GetSize() ; i++ )
	{
		if( asColumnCode.GetAt(i).Compare(sCode)==0 )
		{
			sDesc = asColumnName.GetAt(i);
			return 0;
		}
	}

	return 0;

	EFS_END
	return -1;

}

// ##############################################################################################################
// [ �Լ����� ]
//   ���������� �ִ� ���丮 ��� ����
//   ����� ��迡 ���� ����ó���� ����
//
// [ �̴Ͻ��� ]                          
//
// ##############################################################################################################
VOID CStaticsInfoMgr::GetDir( CString &sDir )
{
EFS_BEGIN

	CString strMenuName = GetMenuName();

	// ==========================================================================================================
	// CFG ���������� ����� ���� �Ϲ� ��踦 �����Ѵ�.
	// ==========================================================================================================
	CFile File;
	// 1. �Ϲ� ������ ���
	if( File.Open( _T("..\\cfg\\_seoul") , CFile::modeRead | CFile::typeBinary ) == 0 )
	{
		// �������� MySQL�� �����Ѵ�.
		if ( strMenuName.Compare( LOAN_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���\\�������\\");
		else if ( strMenuName.Compare( WORKING_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���\\�������\\");
		else if ( strMenuName.Compare( HOLDING_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���\\�������\\");
		else if ( strMenuName.Compare( BOOK_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���\\�弭�������\\");
		else if ( strMenuName.Compare( REPAIR_BOOK_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���\\����_��������_���\\");
		else if ( strMenuName.Compare( SE_HOLDING_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���\\����_�������\\");
		//++2009.01.05 ADD BY PWR {{++
		// å�η���� �߰�
		else if ( strMenuName.Compare( COOPERATIVE_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���\\å�η����\\");
		//--2009.01.05 ADD BY PWR --}}
	}
	// 2. ����� ���
	else
	{
		if ( strMenuName.Compare( LOAN_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���_�����\\�������\\");
		else if ( strMenuName.Compare( WORKING_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���_�����\\�������\\");
		else if ( strMenuName.Compare( HOLDING_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���_�����\\�������\\");
		else if ( strMenuName.Compare( BOOK_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���_�����\\�弭�������\\");
		else if ( strMenuName.Compare( REPAIR_BOOK_STATICS ) == 0 )
			sDir = _T("..\\cfg\\���_�����\\����_��������_���\\");
		
		File.Close();
	}
	
EFS_END
}

// ##############################################################################################################
// [ �Լ����� ]
//   �弭 ������ ���
//   �۾���, ������ ��迡 ���� ���ȴ�.
// 
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::RunBoStatics( CString sStaticsClass , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgressCtrl )
{
EFS_BEGIN

	// 1. ���� ��踦 �����´�.
	// 2. å�� ��踦 �������鼭 ���� ��迡 ���� ADD�Ѵ�.
	// 3. ���� �հ踦 �����´�. 
	// 4. å�� �հ踦 �����´�.
	// 5. ���� �հ迡 å�� �հ踦 ADD�Ѵ�.
	// ��~~

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;	
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;
	CString sResultFieldName;
	CStringArray saResultFieldName;
	CString sWhere;

	// =========================================================================================================
	// 1. ��踦 ���� ���� �⺻���� ��������
	// =========================================================================================================
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::RunBoStatics()") );

	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultFieldName );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::RunBoStatics()") );

	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );
	
	CString sXData,sYData,sTmp;
	pTotalProgressCtrl->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()*m_pRealResultDM->GetRowCount()*2) );
	INT nProgressCnt = 0;

	// =========================================================================================================
	// 2. �� , å�� ��踦 �����.
	// =========================================================================================================
	for ( INT i = 0;i < saTableName.GetSize();i++ )
	{
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);

		pTotalProgressCtrl->SetRange32( 0 , 1 );

		ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStatics()") );

		sWhere += _T(" Group By ") + sXFieldName + _T(" , ") + sYFieldName;

		ids = FillStaticsValue2( sXFieldName,sYFieldName,saResultFieldName.GetAt(i),_T(""),_T(""),_T("Y"),_T("Y"),sWhere,i, pProgressCtrl);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStatics()") );

		pTotalProgressCtrl->SetPos(1);
	}

	// =========================================================================================================
	// 3. �հ� ����
	// =========================================================================================================
	ids = MakeTotalCount2( sStaticsClass , saTableName , sXFieldName,sYFieldName , pProgressCtrl , pTotalProgressCtrl);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::RunBoStatics()") );

	// =========================================================================================================
	// 4. �ڵ尪�� �������� �ٲ۴�.
	// =========================================================================================================
	ids = CodeToDesc(sStaticsClass);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::RunBoStatics()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   �۾���, ������ ��� WHERE�� ������
//
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::GetBoWhere( CString sStaticsClass,CString sTableName,CString sXFieldName,CString sYFieldName,CString sXData,CString sYData,CString sXGroupYN , CString sYGroupYN , INT nMode,CString &sWhere )
{
EFS_BEGIN

	INT ids;

	CStringArray saWhere;
	CString sMenuName = GetMenuName();
	CString sInWhere;	// ������
	CString sOutWhere;	// ������

	// ==========================================================================================================
	// 1. �⺻ ���� ��������
	// ==========================================================================================================
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�⺻����") , sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetBoWhere()") );

	CLocCommonAPI::MakeArrayList( sWhere , _T("/") , saWhere );

	if( saWhere.GetSize() < nMode+1 )
		nMode = 0;
	
	sWhere = saWhere.GetAt(nMode);
	
	// ==========================================================================================================
	// 2. �˻����ǿ��� ������ ���� ������
	// ==========================================================================================================
	if ( sMenuName.Compare(BOOK_STATICS) != 0 )
	{
		ids = GetBoSearchWhere( sStaticsClass , sTableName , sInWhere , sOutWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoWhere()") );
		if ( ids > 0 ) return ids;
	}

	// ==========================================================================================================
	// 3. Group�� ���ؼ� ������ ���� �����ϱ�
	// ==========================================================================================================
	CStringArray asTableName;
	CLocCommonAPI::MakeArrayList( sTableName , _T(",") , asTableName );

	CString sTmpWhere;
	if( !sXFieldName.IsEmpty() && (sXGroupYN.Compare(_T("N"))==0) )
	{
		sXData.Replace( _T("'"), _T("''") );
		
		if( !sXData.IsEmpty() )
			sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName,sXData );
		else
			sTmpWhere.Format( _T(" AND ( %s='%s' OR %s IS NULL ) "),sXFieldName,sXData,sXFieldName);

		if( IsFieldFromTable( sXFieldName , asTableName ) == 1 )
			sOutWhere += sTmpWhere;
		else
			sInWhere += sTmpWhere;
	}

	if( !sYFieldName.IsEmpty() && (sYGroupYN.Compare(_T("N"))==0) )
	{
		sYData.Replace( _T("'"), _T("''") );
		
		if( !sYData.IsEmpty() )
			sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName,sYData );
		else
			sTmpWhere.Format( _T(" AND ( %s='%s' OR %s IS NULL ) "),sYFieldName,sYData,sYFieldName);
		if( IsFieldFromTable( sYFieldName , asTableName ) == 1 )
			sOutWhere += sTmpWhere;
		else
			sInWhere += sTmpWhere;
	}

	// ��ũ�� ����
	sWhere.Replace( _T("%������%") , sInWhere );
	sWhere.Replace( _T("%������%") , sOutWhere );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ���������� �������� �����ϱ�
//
// [ �̴Ͻ��� ]
// 
// ##############################################################################################################
INT CStaticsInfoMgr::GetBoSearchWhere( CString sStaticsClass , CString sTableName , CString &sInWhere , CString &sOutWhere )
{
EFS_BEGIN

	INT ids;

	CString sMenuName = GetMenuName();

	// ==========================================================================================================
	// 1. ��Ʈ�� �˸��ƽ��� �� DM ����
	// ==========================================================================================================
	ids = MakeUserSearchList( sStaticsClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	// ==========================================================================================================
	// 2. �޴� �� ���� �޶�����.
	// ==========================================================================================================
	CString sPrintSearchWhere;

	if( sMenuName.Compare(LOAN_STATICS) == 0 )
	{
		bool bFlag = FALSE;
		if( _T("������ ������") == sStaticsClass )			bFlag = TRUE;
		ids = GetSearchWhere1110( sTableName , sInWhere , sOutWhere , sPrintSearchWhere, bFlag );
	}
	else if( sMenuName.Compare(WORKING_STATICS) == 0 )
		ids = GetSearchWhere1111( sTableName , sInWhere , sOutWhere , sPrintSearchWhere );
	else if( sMenuName.Compare(HOLDING_STATICS) == 0 )
		ids = GetSearchWhere1112( sStaticsClass,sTableName , sInWhere , sOutWhere , sPrintSearchWhere );
	else if( sMenuName.Compare(REPAIR_BOOK_STATICS) == 0 )
		ids = GetSearchWhere1440( sTableName , sInWhere , sOutWhere , sPrintSearchWhere );
	//++2009.01.06 ADD BY PWR {{++
	// å�η���� �߰�
	else if( sMenuName.Compare(COOPERATIVE_STATICS) == 0 )
		ids = GetSearchWhereCooperative( sTableName , sInWhere , sOutWhere , sPrintSearchWhere );
	//--2009.01.06 ADD BY PWR --}}

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	// ==========================================================================================================
	// 3. ��¿� �˻�����
	// ==========================================================================================================
	ids = SetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����ڰ˻�����_���") , sPrintSearchWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	return 0;

EFS_END
	return -1;
}


INT CStaticsInfoMgr::MakeUserSearchList(CString sStaticsClass)
{
	EFS_BEGIN

	INT ids;
	// �˻� ���ǿ��� ������ �� ��������
	CString sSearchCond;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����ڰ˻�����") , sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeUserSearchList") );

	if( sSearchCond.IsEmpty() )
	{
//		AfxMessageBox( _T("����� ������ ���� �Է��Ͻʽÿ�") );
		return 1;
	}

	CStringArray saSearchCond;
	CStringArray saOneSearchCond;
	ids = CLocCommonAPI::MakeArrayList( sSearchCond , _T(",") , saSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeUserSearchList") );

	m_pUserSearchCond->FreeData();

	for( INT i = 0 ; i < saSearchCond.GetSize() ; i++ )
	{
		saOneSearchCond.RemoveAll();
		
		ids = CLocCommonAPI::MakeArrayList( saSearchCond.GetAt(i) , _T("=") , saOneSearchCond );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeUserSearchList") );
		
		if( saOneSearchCond.GetSize() != 2 ) continue;

		m_pUserSearchCond->InsertRow(-1);
		ids = m_pUserSearchCond->SetCellData( _T("��Ʈ�Ѿ˸��ƽ�") , saOneSearchCond.GetAt(0) , m_pUserSearchCond->GetRowCount()-1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeUserSearchList") );
		ids = m_pUserSearchCond->SetCellData( _T("��") , saOneSearchCond.GetAt(1) , m_pUserSearchCond->GetRowCount()-1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeUserSearchList") );
	}

	return 0;


	EFS_END
	return -1;

}

INT CStaticsInfoMgr::FillStaticsValue2( CString sXFieldName , CString sYFieldName , CString sResultFieldName , CString sXData , CString sYData , CString sXGroupYN , CString sYGroupYN , CString sWhere , INT nMode , CProgressCtrl *pProgressCtrl )
{
	EFS_BEGIN

	INT ids;

	

	// SELECT �ʵ� ����
	ids = SetDataMgrRef( m_pResultDM , sXFieldName ,_T("") , sXGroupYN ,  0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("") , sYGroupYN , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunStatics") );
	ids = SetDataMgrRef( m_pResultDM , sResultFieldName , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("RunStatics") );

	// WHERE ���� ����
	CString sSendWhere = sWhere;
	
	//===================================================
	//2009.10.05 ADD BY PJW : ���ں� ������ ���� ���ڷ� �����Ѵ�.
	if( sYFieldName == _T("TO_CHAR(B.REG_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.REG_DATE,'YYYY/MM/DD')");
	}
	else if( sYFieldName == _T("TO_CHAR(B.SHELF_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.SHELF_DATE,'YYYY/MM/DD')");
	}
	else if( sYFieldName == _T("TO_CHAR(B.CHECKIN_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.CHECKIN_DATE,'YYYY/MM/DD')");
	}
	//===================================================

	// RESULT DM -> REAL RESULT DM ���� �̵�
	ids = m_pResultDM->RefreshDataManager( sSendWhere , pProgressCtrl );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue2") );

	INT nXIndex , nYIndex;
	CString sXAlias , sYAlias , sResult , sPreResult;

	for( INT i = 0 ; i < m_pResultDM->GetRowCount() ; i++ )
	{
		ids = m_pResultDM->GetCellData( _T("�����") , i , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsValue2") );

		ids = m_pResultDM->GetCellData( _T("X��˸��ƽ�") , i , sXAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsValue2") );

		ids = m_pResultDM->GetCellData( _T("Y��˸��ƽ�") , i , sYAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsValue2") );
		
		// --�弭 ���
		// �ε��� ã��
		ids = GetDMIndex( m_pRealResultDM , sXAlias,sYAlias,nXIndex,nYIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("FillStaticsValue2") );

		// �� �Է�
		if ( m_sIs64lib == _T("Y") && sXAlias==_T("355-9") )
			ids = m_pRealResultDM->GetCellData( _T("3") , nYIndex , sPreResult );
		else
			ids = m_pRealResultDM->GetCellData( sXAlias , nYIndex , sPreResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("FillStaticsValue2") );
				
		if( nMode == 0 )
		{
			// 2005.07.28 ADD BY PDJ
			// ����з���ȣ ó��
			// ? ( ? ) ���Ŀ� ���� ����
			if ( m_sIs64lib == _T("Y") && ( sXAlias==_T("355-9") || sXAlias==_T("3") ) ) 
			{
				CString strNormal, strArmy, sTemp;
				DividNormalArmy2(sPreResult, sXAlias, sResult, nMode, &sTemp);
				ids = m_pRealResultDM->SetCellData( _T("3") , sTemp , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("FillStaticsValue2") );
			}
			else
			{
				sResult.Format( _T("%ld") , _ttoi(sPreResult) + _ttoi(sResult) );
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("FillStaticsValue2") );
			}
		}
		else if( nMode == 1 )
		{
			if ( m_sIs64lib == _T("Y") && ( sXAlias==_T("355-9") || sXAlias==_T("3") ) ) 
			{
				CString strNormal, strArmy, sTemp;
				DividNormalArmy2(sPreResult, sXAlias, sResult, nMode, &sTemp);		
				ids = m_pRealResultDM->SetCellData( _T("3") , sTemp , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("FillStaticsValue2") );
			}
			else
			{   
				sResult.Format( _T("%ld\r\n(%ld)") , _ttoi(sPreResult) , _ttoi(sResult) );							
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -102 , _T("FillStaticsValue2") );
		
			}

		}
	}


	return 0;

	EFS_END
	return -1;

}

// �հ� ���� - �۾�,���� �ڷ� ��� �հ� ����
INT CStaticsInfoMgr::MakeTotalCount2( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName, CString sYFieldName , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

//	bool IsInsert = TRUE;

/*	
	// ���� �հ� ����
	ids = YSum2(sStaticsClass,saTableName,sXFieldName , sYFieldName ,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );

	// ���� �հ� ����
	ids = XSum2(sStaticsClass,saTableName,sXFieldName , sYFieldName ,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );
*/

	/// 20040223 ������ ����
	/// �ӵ� ������ �ϱ� ���� �հ赵 �׷������� ����ϱ�
	m_pYSumDM->FreeData();
	m_pXSumDM->FreeData();
	
	ids = XYSum2( sStaticsClass , saTableName , sXFieldName , sYFieldName , pTotalProgress );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("MakeTotalCount2") );


	return 0;


	EFS_END
	return -1;

}

// �հ� ���� - ���� �ڷ� ��� �հ� ����
INT CStaticsInfoMgr::XYSum2( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	/// �ߺ� ��� �հ� ���
	ids = XYSum2_duplication_allow();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("XYSum2") );

	/// �ߺ� ���� �հ� ���
	ids = XYSum2_duplication_delete( sStaticsClass , saTableName , sXFieldName , sYFieldName , pTotalProgress );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XYSum2") );

	//===================================================
	//2009.05.20 ADD BY PJW : ������ ��ȯ �Ѵ�.
	INT nRowCnt;
	INT i;
	POSITION pos;
	CString strPrice;
	CString strConverPrice;
	CString strResultAlias;
	CString sDupAddAlias;
	CString sDupDelAlias;

	CESL_DataMgr::reference *pRef;

	sDupAddAlias = _T("�ߺ����_�հ�");
	sDupDelAlias = _T("�ߺ�����_�հ�");

	if( sStaticsClass == _T("�����ջ����") )
	{
		nRowCnt = m_pRealResultDM->GetRowCount();
		for( i = 0 ; i < nRowCnt ; i++ )
		{
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				pos = m_pRealResultDM->RefList.FindIndex(j);
				pRef = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
				strResultAlias = pRef->FIELD_ALIAS;
					
				m_pRealResultDM->GetCellData( strResultAlias, i, strPrice );
				strConverPrice = ConvertPrice( strPrice );
				m_pRealResultDM->SetCellData( strResultAlias, strConverPrice, i );
			}		
		}
		

		nRowCnt = m_pYSumDM->GetRowCount();
		for( i = 0 ; i < nRowCnt ; i++ )
		{
			m_pYSumDM->GetCellData( sDupAddAlias, i, strPrice );
			strConverPrice = ConvertPrice( strPrice );
			m_pYSumDM->SetCellData( sDupAddAlias, strConverPrice, i );

			m_pYSumDM->GetCellData( sDupDelAlias, i, strPrice );
			strConverPrice = ConvertPrice( strPrice );
			m_pYSumDM->SetCellData( sDupDelAlias, strConverPrice, i );
		}

		nRowCnt = m_pXSumDM->GetRowCount();
		for( i = 0 ; i < nRowCnt ; i++ )
		{
			m_pXSumDM->GetCellData( sDupAddAlias, i, strPrice );
			strConverPrice = ConvertPrice( strPrice );
			m_pXSumDM->SetCellData( sDupAddAlias, strConverPrice, i );

			m_pXSumDM->GetCellData( sDupDelAlias, i, strPrice );
			strConverPrice = ConvertPrice( strPrice );
			m_pXSumDM->SetCellData( sDupDelAlias, strConverPrice, i );
		}
	}
	//===================================================
	
	return 0;

	EFS_END

	return -1;
}

//###############################################################################################################
// [ �Լ����� ]
//   �ߺ� ���� �հ� - ��� �׷�ȭ ���ִٰ� ���� - å���� �Ȱ��ٰ� ����
//
// [ �̴Ͻ��� ]
//
//###############################################################################################################
INT CStaticsInfoMgr::XYSum2_duplication_delete(CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
EFS_BEGIN

	INT ids;

	CStringArray saResultFieldName;
	CString sResultFieldName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	// 2012.11.20 ADD BY PWR : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (�����ջ���� �ӵ�����)
	if(_T("�����ջ����") == sStaticsClass) return 0;

	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );
	
	INT i = 0;
	
	// DM ���̺� �̸� �߰�
	m_pResultDM->TBL_NAME = saTableName.GetAt(i);
	
	CStringArray saTableAlias;	
	ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );	

	/// ������ �Ѳ����� �����´�.
	// ==========================================================================================================
	/// ���� �հ� ���
	// ==========================================================================================================	
	// SELECT �ʵ� ����
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("Y") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	// pjw
//	ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT S.REC_KEY )") , _T("0") , _T("Y") , 2 );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	if( _T("�ڷ�Ǻ��̿����") == sStaticsClass ||   _T("�űԱ��Ե����̿����")== sStaticsClass)
	{
		ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT CL.BOOK_KEY )") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}
	else
	{
		ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT S.REC_KEY )") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}
	
	// 3.3. WHERE�� ���� 
	CString sWhere;
		
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , _T("") , _T("N") , _T("Y") ,i , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	sWhere += _T(" GROUP BY ") + sYFieldName;
		
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	
	CString sYSumAlias , sDuplicateAllowSum;
	CString sTmpResult , sResult;
	CString sYData , sTmp;
	CString sSpeciesCnt , sBookCnt;
	INT nIndex;
	for( INT k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
	{
		ids = m_pResultDM->GetCellData( _T("Y��˸��ƽ�") , k , sYSumAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		nIndex = -1;
		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( j , 0 , sYData );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
			
			if( sYData.Compare( sYSumAlias ) == 0 ) nIndex = j;
		}
		
		if( nIndex == -1 ) continue;
		
		ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		ids = m_pXSumDM->GetCellData( _T("�ߺ����_�հ�") , nIndex , sDuplicateAllowSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
		//pjw����
		if( _T("�ڷ�Ǻ��̿����") == sStaticsClass ||   _T("�űԱ��Ե����̿����")== sStaticsClass)
		{
			if( _ttoi(sBookCnt) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
			}
			
		}else
		{
			if( _ttoi(sResult) == 0 )
			sResult = _T("0");
			else if( 0 != _ttoi(sResult) && 0 == _ttoi(sBookCnt) )
				sResult.Format( _T("%d") , _ttoi(sResult));
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
			}
		}
		
		/*if( _ttoi(sResult) == 0 )
		sResult = _T("0");
		else
		{
			sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
		}*/

		ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , nIndex  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
	}

	// ==========================================================================================================
	// ���� �հ� ���
	// ==========================================================================================================
	// 3.3. WHERE�� ���� 
	sWhere.Empty();
	
	// SELECT �ʵ� ����
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("Y") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	// pjw
//	ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT S.REC_KEY)") , _T("0") , _T("Y") , 2 );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	if( _T("�ڷ�Ǻ��̿����") == sStaticsClass ||_T("�űԱ��Ե����̿����") == sStaticsClass  )
	{
		ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT CL.BOOK_KEY)") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}
	else
	{
		ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT S.REC_KEY)") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}	
	
	
	// WHERE�� ����
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , _T("") , _T("") ,_T("Y") , _T("N") , i , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	sWhere += _T(" GROUP BY ") + sXFieldName;
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	CString sXSumAlias;
	sDuplicateAllowSum.Empty();
	sTmpResult.Empty() , sResult.Empty();
	CString sXData;
	sTmp.Empty();
	sSpeciesCnt.Empty() , sBookCnt.Empty();
	
	for( k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
	{
		
		ids = m_pResultDM->GetCellData( _T("X��˸��ƽ�") , k , sXSumAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		nIndex = -1;
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

			if( sXData.Compare( sXSumAlias ) == 0 ) nIndex = j;

			//##################################################################################################
			if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )		nIndex = 4;
			//##################################################################################################
		}

		if( nIndex == -1 ) continue;

		ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

		ids = m_pYSumDM->GetCellData( _T("�ߺ����_�հ�") , nIndex-1 , sDuplicateAllowSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ���� ����з���ȣ ����ó��
		if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
		{
			// sDuplicateAllowSum ���� �и� �۾�
			CString sSpecies, sBook;
			DividArmyCell( sDuplicateAllowSum, sSpecies, sBook );			
			
			sSpecies = sSpecies.Mid( sSpecies.Find( DIVCHAR ) + 1 );
			sBook = sBook.Mid( sBook.Find( DIVCHAR ) + 1 );
			sDuplicateAllowSum.Format( _T("%d\r\n(%d)"), _ttoi( sSpecies ), _ttoi( sBook ) );
		}
		// ------------------------------------------------------------------------------------------------------

		MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
	
		
		//PJW����
		if( _T("�ڷ�Ǻ��̿����") == sStaticsClass ||   _T("�űԱ��Ե����̿����")== sStaticsClass)
		{
			if( _ttoi(sBookCnt) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );

				// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ���� ����з���ȣ ����ó��
				if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
				{
					CString sTemp;
					ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , nIndex-1, sTemp );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

					CString sForSpeci, sForBook;
					MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

					ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

					sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
				}
			}
			
		}else
		{
			if( _ttoi(sResult) == 0 )
			sResult = _T("0");
			else if( 0 != _ttoi(sResult) && 0 == _ttoi(sBookCnt) )
				sResult.Format( _T("%d") , _ttoi(sResult));
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );

				// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ���� ����з���ȣ ����ó��
				if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
				{
					CString sTemp;
					ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , nIndex-1, sTemp );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

					CString sForSpeci, sForBook;
					MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

					ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

					sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
				}
			}
		}
		/*if( _ttoi(sResult) == 0 )
			sResult = _T("0");
		else
		{
			sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );

			// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ���� ����з���ȣ ����ó��
			if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
			{
				CString sTemp;
				ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , nIndex-1, sTemp );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				CString sForSpeci, sForBook;
				MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

				ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
			}
			// --------------------------------------------------------------------------------------------------
		}*/

		ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , nIndex-1  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );				
	}
	
	// ==========================================================================================================
	// ���հ�
	// ==========================================================================================================
	// SELECT �ʵ� ����

	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , saResultFieldName.GetAt(i) , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	// WHERE�� ����
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -26 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
			
	ids = m_pResultDM->GetCellData( _T("�����") , 0 , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -27 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	ids = m_pXSumDM->GetCellData( _T("�ߺ����_�հ�") , m_pXSumDM->GetRowCount()-1 , sDuplicateAllowSum );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -28 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
	
	if( _ttoi(sResult) == 0 )
		sResult = _T("0");
	else if( 0 != _ttoi(sResult) && 0 == _ttoi(sBookCnt) )
		sResult.Format( _T("%d") , _ttoi(sResult));
	else
		sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );	
	
	ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , m_pXSumDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -29 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	// ==========================================================================================================
	// ��� �հ��� ����ִ� ������ 0���� �����
	// ==========================================================================================================
	
		
	for( INT m = 0 ; m < m_pXSumDM->GetRowCount() ; m++ )
	{
		ids = m_pXSumDM->GetCellData( _T("�ߺ�����_�հ�") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -30 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;

		ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}

	for( m = 0 ; m < m_pYSumDM->GetRowCount() ; m++ )
	{
		ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -32 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;
		
		ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -33 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}
	return 0;

EFS_END
	return -1;
}

//###############################################################################################################
// [ �Լ����� ]
//   �ߺ� ��� �հ� ���
//
// [ �̴Ͻ��� ]
//
//###############################################################################################################
INT CStaticsInfoMgr::XYSum2_duplication_allow()
{
EFS_BEGIN

	INT ids;
	
	// ==========================================================================================================
	// �ߺ� ��� �հ� ����
	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSpeciesSum,sBookSum;
	CString sData;
	CString sSpeciesCnt,sBookCnt;
	CString sTotalSpeciesSum,sTotalBookSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	// ==========================================================================================================
	
	// ==========================================================================================================
	// 1. ���� �հ� ����
	// ==========================================================================================================
	for( INT i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;
			
			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XYSum2_duplication_allow()") );
			
			if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			{
				MakeArmySpeciesBookCnt( sData, &sData );
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
			
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
			else
			{
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
				
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
		}
		
		sTotalSpeciesSum.Format( _T("%d") , _ttoi(sTotalSpeciesSum) + _ttoi(sSpeciesSum) );
		sTotalBookSum.Format( _T("%d") , _ttoi(sTotalBookSum) + _ttoi(sBookSum) );
		
		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);
		
		if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");
		else if( (_ttoi(sSpeciesSum) != 0) && (_ttoi(sBookSum) == 0) )
			sData.Format( _T("%s") , sSpeciesSum );
		else
			sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );
		
		m_pXSumDM->SetCellData( sSumAlias , sData , i);

	}
	
	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);
	
	if( (_ttoi(sTotalSpeciesSum) == 0) && (_ttoi(sTotalBookSum) == 0) )
		sData = _T("0");
	else if( (_ttoi(sTotalSpeciesSum) != 0) && (_ttoi(sTotalBookSum) == 0) )
			sData.Format( _T("%s") , sTotalSpeciesSum );
	else
		sData.Format( _T("%s\r\n(%s)") , sTotalSpeciesSum , sTotalBookSum );
	
	m_pXSumDM->SetCellData( sSumAlias , sData , i );

	// ==========================================================================================================
	// �ߺ� ��� �հ� ����
	sAlias.Empty();
	sSpeciesSum.Empty();
	sBookSum.Empty();
	sData.Empty();
	sSpeciesCnt.Empty();
	sBookCnt.Empty();
	// ==========================================================================================================
	
	// ==========================================================================================================
	// 2. ���� �հ� ����
	// ==========================================================================================================
	for( i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;
			
		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XYSum2_duplication_allow()") );
						
			// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ������ ����з���ȣ �հ踦 ��Ÿ���� ���� ���� ó��
			if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			{
				// ����� �����ٰ� ���� ����Ѵ�.
				MakeArmySum( sSpeciesSum , sData, sSpeciesSum );				
			}
			else
			{
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
			
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
			// --------------------------------------------------------------------------------------------------
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);
		
		if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			sData.Format( _T("%s"), sSpeciesSum );
		else
		{
			if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
				sData = _T("0");
			else if( (_ttoi(sSpeciesSum) != 0) && (_ttoi(sBookSum) == 0) )
				sData.Format( _T("%s") , sSpeciesSum );			
			else
				sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );			
		}
		
		m_pYSumDM->SetCellData( sSumAlias , sData ,i-1 );		
	}	
		
	return 0;
EFS_END
	return -1;
}

// ���� �հ� ���� - 20040203 ������ ������ ����
INT CStaticsInfoMgr::YSum2( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CStringArray saResultFieldName;
	CString sResultFieldName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunBoStatics") );

	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );

	// ���� �հ� ����
	pTotalProgress->SetRange32( 0 , saTableName.GetSize()*(m_pRealResultDM->RefList.GetCount()+m_pRealResultDM->GetRowCount()) );

	m_pYSumDM->FreeData();

	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM ���̺� �̸� �߰�
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		CStringArray saTableAlias;

		ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		// 3.3. WHERE�� ���� 
		CString sWhere;
				
		CString sTmpResult , sResult;
		CString sXData , sTmp;
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// SELECT �ʵ� ����
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , saResultFieldName.GetAt(i) , _T("0") , _T("Y") , 2 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("RunStatics") );


			// WHERE�� ����
			ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , sXData , _T("") ,_T("N") , _T("N") , i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeTotalCount") );
	
			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// RESULT DM -> REAL RESULT DM ���� �̵�
			if( m_pYSumDM->GetRowCount() < j )
				m_pYSumDM->InsertRow(-1);

			ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , j-1 , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("�����") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			if( i == 0 )
				sResult.Format( _T("%d") , _ttoi(sTmpResult)  );
			else if( i == 1 )
			{
				if( (_ttoi(sResult) == 0) && (_ttoi(sTmpResult) == 0) )
					sResult = _T("0");
				else
					sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sTmpResult) );
			}
			
			ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , j-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( (i+1)*(j+1) );

		}
	}

	// �ߺ� ��� �հ� ����
	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSpeciesSum,sBookSum;
	CString sData;
	CString sSpeciesCnt,sBookCnt;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );

			sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
			sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);

		if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");
		else
			sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );

		m_pYSumDM->SetCellData( sSumAlias , sData ,i-1 );
	}	

	return 0;

	EFS_END
	return -1;

}


// ���� �հ� ���� - 20040203 ������ - ������ ����
INT CStaticsInfoMgr::XSum2( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CStringArray saResultFieldName;
	CString sResultFieldName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunBoStatics") );

	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );

	m_pXSumDM->FreeData();

	// �ߺ� ���� �հ� ����
	CString sWhere;
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM ���̺� �̸� �߰�
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		// 3.3. WHERE�� ���� 
		CString sYData;
		CString sTmpResult , sResult;
		for( INT k = 0 ; k < m_pRealResultDM->GetRowCount()+1 ; k++ )
		{
			if( k == m_pRealResultDM->GetRowCount() )
				sYData = _T("�հ�");
			else
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			}

			// SELECT �ʵ� ����
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , saResultFieldName.GetAt(i) , _T("0") , _T("Y") , 2 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("RunStatics") );
			
			// WHERE�� ����
			if( sYData.Compare(_T("�հ�"))==0 )
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
			else
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );
			
			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			// RESULT DM -> REAL RESULT DM ���� �̵�
			if( m_pXSumDM->GetRowCount() < k+1 )
				m_pXSumDM->InsertRow(-1);

			ids = m_pXSumDM->GetCellData( _T("�ߺ�����_�հ�") , k , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("�����") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			if( i == 0 )
				sResult.Format( _T("%ld") , _ttoi(sTmpResult) );
			else if( i == 1 )
			{
				if( (_ttoi(sResult) == 0) && (_ttoi(sTmpResult) == 0) )
					sResult = _T("0");
				else
					sResult.Format( _T("%ld\r\n(%ld)") , _ttoi(sResult) , _ttoi(sTmpResult) );
			}
			
			ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , k );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( (i+1)*(k+1)+(saTableName.GetSize()*m_pRealResultDM->RefList.GetCount()) );
		}	
	}

	// �ߺ� ��� �հ� ����
	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSpeciesSum,sBookSum;
	CString sData;
	CString sSpeciesCnt,sBookCnt;
	CString sTotalSpeciesSum,sTotalBookSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;

			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );

			sSpeciesSum.Format( _T("%ld") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
			sBookSum.Format( _T("%ld") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
		}
		
		sTotalSpeciesSum.Format( _T("%ld") , _ttoi(sTotalSpeciesSum) + _ttoi(sSpeciesSum) );
		sTotalBookSum.Format( _T("%ld") , _ttoi(sTotalBookSum) + _ttoi(sBookSum) );

		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);

		if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");
		else
			sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );

		m_pXSumDM->SetCellData( sSumAlias , sData , i);
		
	}

	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);

	if( (_ttoi(sTotalSpeciesSum) == 0) && (_ttoi(sTotalBookSum) == 0) )
		sData = _T("0");
	else
		sData.Format( _T("%s\r\n(%s)") , sTotalSpeciesSum , sTotalBookSum );

	m_pXSumDM->SetCellData( sSumAlias , sData , i );
	
	return 0;

	EFS_END
	return -1;

}


INT CStaticsInfoMgr::MakeSpeciesBookCnt( CString sData ,CString &sSpeciesCnt ,CString &sBookCnt )
{
	EFS_BEGIN

	INT	 nPIndex = sData.Find(_T("("));
	INT	 nLIndex = sData.Find(_T(")"));

	sSpeciesCnt.Empty();
	sBookCnt.Empty();

	if( (nPIndex > -1 && nLIndex > -1) )
	{
		sSpeciesCnt = sData.Mid(0,nPIndex);
		sBookCnt = sData.Mid( nPIndex+1 , nLIndex-nPIndex-1 );
	}

	if( -1 == nPIndex && -1 == nLIndex )
	{
		sSpeciesCnt = sData;
	}

	return 0;

	EFS_END
	return -1;
}


INT CStaticsInfoMgr::GetSearchWhere1111( CString sTableName , CString &sInWhere , CString &sOutWhere , CString &sPrintSearchWhere )
{
	EFS_BEGIN

	INT ids;

	// �˻� ���� - �̰� �밡�� ��¿�� ���� �Ѥ�
	CStringArray saTableName;
	CString sDBField;

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

	// ���Ա��� , �������� , �з���ȣ���� , ���Գ⵵
	const INT nCnt1 = 4;
	TCHAR *sControlAlias1[nCnt1] =
	{
		_T("���Ա���") , _T("��������") , _T("�з���ȣ����") , _T("���Գ⵵") 
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING , ONESTRING , ONESTRING , ONENUMERIC
	};

	CString sWhere;
	CString sValue;
	CString sFieldName;
	for( INT i = 0 ; i < nCnt1 ; i++ )
	{
		sValue.Empty();
		sWhere.Empty();

		GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias1[i] , _T("��") , sValue );

		if( sValue.IsEmpty() ) continue;

		// �ʵ�� ��������
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		if( sFieldType1[i] == ONESTRING )
			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue);
		else if( sFieldType1[i] == ONENUMERIC )
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue );

		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

		if( i != 3 )
		{
			if( i == 2 )
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�з�ǥ����") , sValue , sValue );
			else
				m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );
		}
		 

		/// ���� �⵵�� �����ֱ�
		if( i == 3 )
			sPrintSearchWhere += sControlAlias1[i] + CString(_T(":")) + sValue + SPACE_STRING;
	}

	// ��Ϲ�ȣ
	sWhere.Empty();
	const INT nCnt2 = 3;
	TCHAR *sControlAlias2[nCnt2] =
	{
		_T("��Ϲ�ȣ����") , _T("��Ϲ�ȣ1") , _T("��Ϲ�ȣ2") 
	};


	CString sValue1,sValue2,sValue3;

	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias2[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias2[1] , _T("��") , sValue2 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias2[2] , _T("��") , sValue3 );

	if( !sValue1.IsEmpty() )
	{
		if( !sValue2.IsEmpty() )
		{
			CString sRegNo1 , sRegNo2;
			sRegNo1 = sValue1+sValue2;
			sRegNo2 = sValue1+sValue3;

			CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo1 );

//			if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
			sPrintSearchWhere += CString(_T("��Ϲ�ȣ:"));

			if( !sValue3.IsEmpty() )
			{
				CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo2 );
				sWhere.Format( _T(" B.REG_NO BETWEEN '%s' AND '%s' ") ,  sRegNo1 , sRegNo2 );
				sPrintSearchWhere += sRegNo1 + _T("~") + sRegNo2;
			}
			else
			{
				sWhere.Format( _T(" B.REG_NO = '%s' ") ,  sRegNo1 );
				sPrintSearchWhere += sRegNo1;
			}

			sPrintSearchWhere += SPACE_STRING;
		}
//		else
//			sWhere.Format( _T(" B.REG_CODE = '%s' ") ,  sValue1 );
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.REG_NO") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}		 

	// ���ι�ȣ , �������
	// ������ ���� 2003�� 12�췯 5�� �������(����) �߰�
	sWhere.Empty();
	const INT nCnt3 = 6;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("���ι�ȣ1") , _T("���ι�ȣ2") , _T("�������1") , _T("�������2") , _T("�������(����)1") , _T("�������(����)2")
	};


	CString sValue4,sValue5,sValue6;
	sValue1.Empty(); sValue2.Empty(); sValue3.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[1] , _T("��") , sValue2 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[2] , _T("��") , sValue3 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[3] , _T("��") , sValue4 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[4] , _T("��") , sValue5 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[5] , _T("��") , sValue6 );
	

	// ���ι�ȣ
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("���ι�ȣ") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

	if( !sValue1.IsEmpty() )
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
		sPrintSearchWhere += CString(_T("���ι�ȣ:"));

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN %s AND %s ") , sFieldName , sValue1 , sValue2 );		
			sPrintSearchWhere += sValue1 + _T("~") + sValue2;
		}
		else
		{
			sPrintSearchWhere += sValue1;
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue1 );
		}
		sPrintSearchWhere += SPACE_STRING;
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.A") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	sWhere.Empty();
	// �������
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("�������") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

	if( !sValue3.IsEmpty() )
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
//		sPrintSearchWhere += CString(_T("[�������:")) + sValue3;
		
		if( !sValue4.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue3 , sValue4 );
//			sPrintSearchWhere += _T("~") + sValue4;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue3 , sValue3 );
//		sPrintSearchWhere += _T("]");
		
	}
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}
	
	sWhere.Empty();
	// �������(����)
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("�������(����)") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );
	
	if( !sValue5.IsEmpty() )
	{
		//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
		//		sPrintSearchWhere += CString(_T("[�������:")) + sValue3;
		
		if( !sValue6.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue5 , sValue6 );
			//			sPrintSearchWhere += _T("~") + sValue4;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue5 , sValue5 );
		//		sPrintSearchWhere += _T("]");
		
	}

	if( !sWhere.IsEmpty() )
	{
		sOutWhere += _T(" AND ") + sWhere;
	}

	// �η��ڷ� ���� ����
	sValue.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�η��ڷ�����") , _T("��") , sValue );
	
	if( !sValue.IsEmpty() )
	{
		sFieldName = _T("B.BOOK_APPENDIX_FLAG");

		sWhere = _T(" B.BOOK_APPENDIX_FLAG = 'B' ");
		
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
//		sPrintSearchWhere += CString(_T("[�η��ڷ�����:N]"));	
	}
	else
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
//		sPrintSearchWhere += CString(_T("[�η��ڷ�����:Y]"));	
	}

	if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += SPACE_STRING ;
	sPrintSearchWhere += CString(_T("����:��(å)"));

	return 0;

	EFS_END
	return -1;
}

INT CStaticsInfoMgr::GetSearchWhere1112( CString sStaticsClass ,CString sTableName , CString &sInWhere , CString &sOutWhere , CString &sPrintSearchWhere )
{
	EFS_BEGIN

	INT ids;

	// �˻� ���� - �̰� �밡�� ��¿�� ���� �Ѥ�
	CStringArray saTableName;
	CString sDBField;

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

	// ���Ա��� , �������� , �з���ȣ���� , ���Գ⵵
	const INT nCnt1 = 4;
	TCHAR *sControlAlias1[nCnt1] =
	{
		_T("�谡�μ�") , _T("��������") , _T("�з���ȣ����") , _T("���Ա���")
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING , ONESTRING , ONESTRING , ONESTRING 
	};


	CString sWhere;
	CString sValue;
	CString sFieldName;
	for( INT i = 0 ; i < nCnt1 ; i++ )
	{
		sValue.Empty();
		sWhere.Empty();

		GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias1[i] , _T("��") , sValue );

		if( sValue.IsEmpty() ) continue;

		// �ʵ�� ��������
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		if( sFieldType1[i] == ONESTRING )
			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue);
		else if( sFieldType1[i] == ONENUMERIC )
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue );

		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

		if( i == 0 )
			m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�ڷ�Ǳ���") , sValue , sValue );
		else if( i == 2 )
			m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�з�ǥ����") , sValue , sValue );
		else
			m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );
	}

	// �ڷ� ����
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�ڷ����") , _T("��") , sValue );
	
	if( !sValue.IsEmpty() )
	{
		if( sValue.Compare( _T("�����ڷ�(��ġ�ڷ�+�������ڷ�+���������ڷ�+�ļ��ڷ�+�н��ڷ�)")) == 0 )
		{
			// 17/07/20 ������ : å���������ڷ� ���� �߰�
/*//BEFORE-----------------------------------------------------------------------------
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ����(BOL218O)�ڷ���� �߰�
			//2010.01.14 UPDATE BY PJW : ���ιݳ�(��ġ�ڷῡ ����)(BOL215O), ��޴����ڷ�(BOL214O), ����ڷ�(BOL2185), �������ڷ��߰���(BOL217O)
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL252N', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare(_T("�����ڷ�(��ġ�ڷ�+�������ڷ�)")) == 0 )
		{
			// 17/07/20 ������ : å���������ڷ� ���� �߰�
/*//BEFORE-----------------------------------------------------------------------------
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
			//2010.01.14 UPDATE BY PJW : ���ιݳ�(��ġ�ڷῡ ����)(BOL215O), ��޴����ڷ�(BOL214O), ����ڷ�(BOL218O), �������ڷ��߰���(BOL217O)
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare(_T("��ġ�ڷ�")) == 0 )
		{
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N' ) ") );
		}
		else if( sValue.Compare(_T("�������ڷ�")) == 0 )
		{
			// 17/07/20 ������ : å���������ڷ� ���� �߰�
/*//BEFORE-----------------------------------------------------------------------------
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
			//2010.01.14 UPDATE BY PJW : ���ιݳ�(��ġ�ڷῡ ����)(BOL216O), ��޴����ڷ�(BOL214O), ����ڷ�(BOL215O), �������ڷ��߰���(BOL217O)
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL217O', 'BOL218O', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL217O', 'BOL218O', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare(_T("���������ڷ�")) == 0 )
		{
			//===================================================
			//2010.01.05 ADD BY PJW : ���ιݳ�(��ġ�ڷῡ ����)(BOL216O) ��
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL113O', 'BOL216O' ) ") );
			//===================================================
			
		}
		else if( sValue.Compare(_T("�н��ڷ�")) == 0 )
		{
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL252N' ) ") );
		}
		else if( sValue.Compare(_T("�����ڷ�")) == 0 )
		{
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL115O' ) ") );
		}
		else if( sValue.Compare(_T("�ļ��ڷ�")) == 0 )
		{
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL114O' ) ") );
		}
		// �����߰� - 2003/12/10
		// �������ڷ� ���� �߰�
		else if( sValue.Compare( _T("�����ڷ�(��ġ�ڷ�+�������ڷ�+���������ڷ�+�ļ��ڷ�+�н��ڷ�+�������ڷ�)") ) == 0 )
		{
			// 17/07/20 ������ : å���������ڷ� ���� �߰�
/*//BEFORE-----------------------------------------------------------------------------
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O', 'BOL116O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
			//2010.01.14 UPDATE BY PJW : ���ιݳ�(��ġ�ڷῡ ����)(BOL215O), ��޴����ڷ�(BOL214O), ����ڷ�(BOL2185), �������ڷ��߰���(BOL217O)
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O', 'BOL116O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL252N', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare( _T("�����ڷ�(��ġ�ڷ�+�������ڷ�+�������ڷ�)") ) == 0 )
		{
			// 17/07/20 ������ : å���������ڷ� ���� 
/*//BEFORE-----------------------------------------------------------------------------
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL116O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O', 'BOL116O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare( _T("�����ڷ�(��ġ�ڷ�+�������ڷ�+���������ڷ�+�ļ��ڷ�+�н��ڷ�+�������ڷ�+�����ڷ�)") ) == 0 )
		{
			// 17/07/20 ������ : å���������ڷ� ���� �߰�
/*//BEFORE-----------------------------------------------------------------------------
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O', 'BOL116O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O', 'BOL116O', 'BOL115O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL252N', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare( _T("�������ڷ�") ) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL116O' ) ") );
		
		if( IsFieldFromTable( _T("B.WORKING_STATUS"), saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;		

		if( (sValue.Compare(_T("�����ڷ�(��ġ�ڷ�+�������ڷ�+���������ڷ�+�ļ��ڷ�+�н��ڷ�)")) != 0) &&
			(sValue.Compare(_T("�����ڷ�(��ġ�ڷ�+�������ڷ�)")) != 0) )
			sPrintSearchWhere += CString(_T("�ڷ����:")) + sValue + SPACE_STRING;		
	}	

	// ��Ϲ�ȣ
	sWhere.Empty();
	const INT nCnt2 = 3;
	TCHAR *sControlAlias2[nCnt2] =
	{
		_T("��Ϲ�ȣ����"), _T("��Ϲ�ȣ1"), _T("��Ϲ�ȣ2") 
	};


	CString sValue1,sValue2,sValue3;

	GetCellData( m_pUserSearchCond, _T("��Ʈ�Ѿ˸��ƽ�"), sControlAlias2[0], _T("��"), sValue1 );
	GetCellData( m_pUserSearchCond, _T("��Ʈ�Ѿ˸��ƽ�"), sControlAlias2[1], _T("��"), sValue2 );
	GetCellData( m_pUserSearchCond, _T("��Ʈ�Ѿ˸��ƽ�"), sControlAlias2[2], _T("��"), sValue3 );

	if( !sValue1.IsEmpty() )
	{
		if( !sValue2.IsEmpty() )
		{
			CString sRegNo1 , sRegNo2;
			sRegNo1 = sValue1+sValue2;
			sRegNo2 = sValue1+sValue3;

			CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo1 );

//			if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
			sPrintSearchWhere += CString(_T("��Ϲ�ȣ:")) + sRegNo1;

			if( !sValue3.IsEmpty() )
			{
				CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo2 );
				sWhere.Format( _T(" B.REG_NO BETWEEN '%s' AND '%s' ") ,  sRegNo1 , sRegNo2 );
				sPrintSearchWhere += _T("~") + sRegNo2;
			}
			else
				sWhere.Format( _T(" B.REG_NO = '%s' ") ,  sRegNo1 );

			sPrintSearchWhere += SPACE_STRING;
		}
//		else
//			sWhere.Format( _T(" B.REG_CODE = '%s' ") ,  sValue1 );
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.REG_NO") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}		 

	// �谡����
	sWhere.Empty();
	const INT nCnt3 = 2;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("�谡����1") , _T("�谡����2")
	};

	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[1] , _T("��") , sValue2 );

	// �谡����
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("�谡����") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

	if( !sValue1.IsEmpty() )
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
		sPrintSearchWhere += CString(_T("�谡����:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
		
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	// �谡��������
	sWhere.Empty();
	const INT nCnt4 = 2;
	TCHAR *sControlAlias4[nCnt4] =
	{
		_T("�谡��������1") , _T("�谡��������2")
	};
	
	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias4[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias4[1] , _T("��") , sValue2 );
	
	// �谡��������
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("�谡��������") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );
	
	if( !sValue1.IsEmpty() )
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
		sPrintSearchWhere += CString(_T("�谡��������:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	
	///���������߰� pjw
	sWhere.Empty();
	const INT nCnt6 = 2;
	TCHAR *sControlAlias6[nCnt6] =
	{
		_T("��������1") , _T("��������2")
	};
	
	sValue1.Empty();sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias6[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias6[1] , _T("��") , sValue2 );
		
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("��������") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );
	
	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("��������:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		/*
		// �ʵ�� ��������
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );
			/*
		sFieldName = _T("B.BOOK_APPENDIX_FLAG");

		sWhere = _T(" B.BOOK_APPENDIX_FLAG = 'B' ");
		
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
		*/
		if( _T("�ڷ�Ǻ��̿����") == sStaticsClass ||   _T("�űԱ��Ե����̿����")== sStaticsClass)
		{
			sOutWhere += _T(" AND ") + sWhere;
			sInWhere += _T(" AND ") + sWhere;
		}
		else
		{
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}
	}	

	///�ڷ���º��������߰� pjw
	sWhere.Empty();
	const INT nCnt7 = 2;
	TCHAR *sControlAlias7[nCnt7] =
	{
		_T("�ڷ���º�������1") , _T("�ڷ���º�������2")
	};
	
	sValue1.Empty();sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias7[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias7[1] , _T("��") , sValue2 );
		
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("�ڷ���º�������") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );
	
	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("�ڷ���º�������:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}	

	///�������� �߰�
	sWhere.Empty();
	const INT nCnt8 = 2;
	TCHAR *sControlAlias8[nCnt8] =
	{
		_T("��������1") , _T("��������2")
	};
	
	sValue1.Empty();sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias8[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias8[1] , _T("��") , sValue2 );
		
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("��������") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );
	
	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("��������:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}	

	// 20040203 ������ ���� - �����ۼ��⵵ �߰�
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�����ۼ��⵵") , _T("��") , sValue );

	
	if( !sValue.IsEmpty() )
	{
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("�����ۼ��⵵") , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		sPrintSearchWhere += CString(_T("�����ۼ��⵵:"));

		sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue );

		sPrintSearchWhere += sValue;
		sPrintSearchWhere += SPACE_STRING;
		
	}
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.A") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}


	// ���ι�ȣ , �������
	sWhere.Empty();
	const INT nCnt5 = 2;
	TCHAR *sControlAlias5[nCnt5] =
	{
		_T("���ι�ȣ1") , _T("���ι�ȣ2") 
	};
	
	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias5[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias5[1] , _T("��") , sValue2 );	
	
	// ���ι�ȣ
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("���ι�ȣ") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );
	
	if( !sValue1.IsEmpty() )
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
		sPrintSearchWhere += CString(_T("���ι�ȣ:"));
		
		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN %s AND %s ") , sFieldName , sValue1 , sValue2 );		
			sPrintSearchWhere += sValue1 + _T("~") + sValue2;
		}
		else
		{
			sPrintSearchWhere += sValue1;
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue1 );
		}
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.A") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	

	// �η��ڷ� ���� ����
	sWhere.IsEmpty();
	sValue.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�η��ڷ�����") , _T("��") , sValue );
	
	if( !sValue.IsEmpty() )
	{
		sFieldName = _T("B.BOOK_APPENDIX_FLAG");

		sWhere = _T(" B.BOOK_APPENDIX_FLAG = 'B' ");
		
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
//		sPrintSearchWhere += CString(_T("[�η��ڷ�����:N"));
	}
	else
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
//		sPrintSearchWhere += CString(_T("[�η��ڷ�����:Y"));		
	}

	if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += SPACE_STRING ;
	//PJW����
	if(sStaticsClass == _T("�ڷ�Ǻ��̿����") || sStaticsClass == _T("�űԱ��Ե����̿����"))
	{
		sPrintSearchWhere += CString(_T("����:�����̷�å(����å)"));
	}else
	{
		sPrintSearchWhere += CString(_T("����:��(å)"));
	}
//	sPrintSearchWhere += CString(_T("����:��(å)"));
	
	return 0;


	EFS_END
	return -1;

}

// ����/�ݳ� ��迡�� �����ǰ� ������ ���ϱ� 
INT CStaticsInfoMgr::GetSearchWhere1110( CString sTableName , CString &sInWhere , CString &sOutWhere , CString &sPrintSearchWhere, bool flag )
{
	EFS_BEGIN

	INT ids;

	// �˻� ���� - �̰� �밡�� ��¿�� ���� �Ѥ�
	CStringArray saTableName;
	CString sDBField;

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

	// 2007.06.17 add by pdw
	CString sStaticsClass;
	GetControlData( _T("CM_BO_STATICS_1100") , _T("�������") , sStaticsClass , -1 , 0 );

	// �з���ȣ����,��������
	const INT nCnt1 = 2;
	TCHAR *sControlAlias1[nCnt1] =
	{
		_T("�з���ȣ����") , _T("��������") 
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING , ONESTRING 
	};

	CString sWhere;
	CString sValue;
	CString sFieldName;
	for( INT i = 0 ; i < nCnt1 ; i++ )
	{
		sValue.Empty();
		sWhere.Empty();

		GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias1[i] , _T("��") , sValue );

		if( sValue.IsEmpty() ) continue;

		// �ʵ�� ��������
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSearchWhere1110") );

		// 2008.06.13 ADD BY PDW {{++
		// ������ ������ >> ��������
		if( 0 == sStaticsClass.Compare(_T("������ ������")) &&
			1 == i )
		{
			// 18.09.27 JMJ KOL.RED.2018.008
			// å�η��ڷ� ����ó���ÿ� LS_WORK_T0.manage_code �� ��������� ����. ������̴� ����ó�����̴� �������� �����ϰ� �Ѵ�.
			// CL.MANAGE_CODE �� �ٲ㼭 CL.BOOK_MANAGE_CODE �� �ߴ�. �ٽ� ������ �������϶��� MANAGE_CODE�� ����ǰ� �Ѵ�.
			INT nDivPos = sFieldName.Find(_T("_"));
			//INT nDivPos = sFieldName.Find(_T("."));
			sFieldName = sFieldName.Mid(nDivPos+1);
		}
		// 2008.06.13 ADD BY PDW --}}

		// 18.09.27 JMJ KOL.RED.2018.008
		if( sFieldType1[i] == ONESTRING )
		{
			if((_T("���� �ڷ� ���") == sStaticsClass || _T("���� �ڷ� ���(å�η�����)") == sStaticsClass) && i == 0)
			{
				sWhere.Format( _T(" (%s = '%s' OR %s IS NULL) ") , sFieldName , sValue , sFieldName);
			}
			else
				sWhere.Format( _T(" (%s = '%s') ") , sFieldName , sValue);
		}
		else if( sFieldType1[i] == ONENUMERIC )
		{
			if((_T("���� �ڷ� ���") == sStaticsClass || _T("���� �ڷ� ���(å�η�����)") == sStaticsClass) && i == 0)
			{
				sWhere.Format( _T(" (%s = '%s' OR %s IS NULL) ") , sFieldName , sValue , sFieldName);
			}
			else
				sWhere.Format( _T(" (%s = '%s') ") , sFieldName , sValue);
		}


		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

		if( i == 0 )
			m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�з�ǥ����") , sValue , sValue );
		else
			m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );

		sPrintSearchWhere += sControlAlias1[i] + CString(_T(":")) + sValue + SPACE_STRING;
	}

	// 2007.07.10 update pdw 
	// �ִ�����ڷ� ��� ���� Ʃ�װ��� ����
	CString strBestLoanBookWhere;

	// 20040223 ������ ���� : �����ȣ , �ּ� �߰�
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�����ȣ") , _T("��") , sValue );
	if( !sValue.IsEmpty() )		
	{
		sFieldName = _T("CLU.H_ZIPCODE");
		
		sPrintSearchWhere += _T("�����ȣ") + CString(_T(":")) + sValue + SPACE_STRING;
				
		sWhere.Format( _T(" %s LIKE '%s%%' ") , sFieldName , sValue );
		
		// 18.09.27 JMJ KOL.RED.2018.008
		if( _T("�ִ�����ڷ�") == sStaticsClass || _T("�ִ�����ڷ�(å�η�����"))
		{
			strBestLoanBookWhere = sWhere;
		}
		else
		{
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}
	}
	
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�ּ�") , _T("��") , sValue );
	if( !sValue.IsEmpty() )		
	{
		sFieldName = _T("CLU.H_ADDR1");
		
		sPrintSearchWhere += _T("�ּ�") + CString(_T(":")) + sValue + SPACE_STRING;
		
		sWhere.Format( _T(" %s LIKE '%%%s%%' ") , sFieldName , sValue );
		
		// 18.09.27 JMJ KOL.RED.2018.008
		if( _T("�ִ�����ڷ�") == sStaticsClass || _T("�ִ�����ڷ�(å�η�����)") == sStaticsClass )
		{
			if( strBestLoanBookWhere.GetLength() )
			{
				strBestLoanBookWhere += _T(" AND ");
			}
			strBestLoanBookWhere += sWhere;
		}
		else
		{
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}
	}

	// 18.09.27 JMJ KOL.RED.2018.008
	if( _T("�ִ�����ڷ�") == sStaticsClass || _T("�ִ�����ڷ�(å�η�����)") == sStaticsClass )
	{
		if( strBestLoanBookWhere.GetLength() )
		{
			strBestLoanBookWhere = _T(" CL.USER_KEY IN ( SELECT REC_KEY FROM CO_LOAN_USER_TBL CLU WHERE ") + strBestLoanBookWhere + _T(" ) ");

			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + strBestLoanBookWhere;
			else
				sInWhere += _T(" AND ") + strBestLoanBookWhere;
		}		
	}

	// ���� ���� , �ݳ�����
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("��������") , _T("��") , sValue );
	
	if( sValue.Compare(_T("��ü")) != 0 )
	{

		if( !sValue.IsEmpty() )		
		{
			sFieldName = _T("CL.LOAN_TYPE_CODE");
					
			sPrintSearchWhere += _T("��������") + CString(_T(":")) + sValue + SPACE_STRING;
			
			if( sValue.Compare(_T("�Ϲ�")) == 0 )
				sValue = _T("0");
			else if( sValue.Compare(_T("Ư��")) == 0 )
				sValue = _T("1");
			else if( sValue.Compare(_T("����")) == 0 )
				sValue = _T("2");
			else if( sValue.Compare(_T("����")) == 0 )
				sValue = _T("3");
			// 2005.09.27 ADD BY PDW(Jr) +++++++++++++++++++++++
			// ��� ���� �߰� 
			else if( sValue.Compare(_T("���")) == 0 )
				sValue = _T("4");
			// _________________________________________________

			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue );
			
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;

		}
	}
	
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�ݳ�����") , _T("��") , sValue );
	
	if( sValue.Compare(_T("��ü")) != 0 )
	{
		if( !sValue.IsEmpty() )
		{
		
			sFieldName = _T("CL.RETURN_TYPE_CODE");
			
			sPrintSearchWhere += _T("�ݳ�����") + CString(_T(":")) + sValue + SPACE_STRING;
			
			if( sValue.Compare(_T("�Ϲ�")) == 0 )
				sValue = _T("0");
			else if( sValue.Compare(_T("�ϰ�")) == 0 )
				sValue = _T("1");
			else if( sValue.Compare(_T("�ұ�")) == 0 )
				sValue = _T("2");
			else if( sValue.Compare(_T("����")) == 0 )
				sValue = _T("3");
			
			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue );
			
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}		
		
	}
	
	// 2007.06.22 ADD BY PDW +++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �������� �߰�
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�������") , _T("��") , sValue );
	
	if( sValue.Compare(_T("��ü")) != 0 )
	{
		if( !sValue.IsEmpty() )
		{
		
			sFieldName = _T("CL.STATUS");
			
			sPrintSearchWhere += _T("�������") + CString(_T(":")) + sValue + SPACE_STRING;
			
			if( sValue.Compare(_T("����")) == 0 )
				sValue = _T("0");
			else if( sValue.Compare(_T("�ݳ�")) == 0 )
				sValue = _T("1");
			else if( sValue.Compare(_T("�ݳ�����")) == 0 )
				sValue = _T("2");
			else if( sValue.Compare(_T("����")) == 0 )
				sValue = _T("3");
			else if( sValue.Compare(_T("�������")) == 0 )
				sValue = _T("4");

			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue );
			
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}		
		
	}
	// ---------------------------------------------------------------------------

	// �ڷ�� ����
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�ڷ��_����") , _T("��") , sValue );

	if( !sValue.IsEmpty() )
	{
		CStringArray saShelfLocCode;
		CLocCommonAPI::MakeArrayList( sValue , _T("|") , saShelfLocCode );
		
		if( flag )
			sFieldName = _T("CLU.SHELF_LOC_CODE");
		else
			sFieldName = _T("CL.SHELF_LOC_CODE");
		
		// 2012.10.25 ADD BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (�̴����� ��迡�� �ڷ�� ���� �ݿ� �ȵǴ°� ����)
		if (_T("�̴����� ���")==sStaticsClass )
		{
			sFieldName = _T("CLU.SHELF_LOC_CODE");
		}

		sPrintSearchWhere += _T("�ڷ��:") ;

		for( INT m = 0 ; m < saShelfLocCode.GetSize() ; m++ )
		{
			if( !sWhere.IsEmpty() ) sWhere += _T(",");
			sWhere += _T("'") + saShelfLocCode.GetAt(m) + _T("'");

			sValue = saShelfLocCode.GetAt(m);

			m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�ڷ�Ǳ���") , sValue , sValue );
			if( m != 0 ) sPrintSearchWhere += _T(",");
			sPrintSearchWhere += sValue;
		}

		sPrintSearchWhere += SPACE_STRING;

		sWhere = sFieldName + _T(" IN (") + sWhere + _T(")");

		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
		
	}



	// �����,������,�ݳ���,�ݳ�������
	sWhere.Empty();
	const INT nCnt3 = 12;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("�����1") , _T("�����2") , _T("������1") , _T("������2") , 
		_T("�ݳ���1") , _T("�ݳ���2") , _T("������1") , _T("������2") , 
		_T("���������1"), _T("���������2"),
		_T("�ݳ�������1") , _T("�ݳ�������2") 
	};

	TCHAR *sDBAlias3[nCnt3] =
	{
		_T("�����") , _T("�����") , _T("������") , _T("������") , 
		_T("�ݳ���") , _T("�ݳ���") , _T("������") , _T("������") ,
		_T("���������") , _T("���������"),
		_T("�ݳ�������") , _T("�ݳ�������") 
	};

	CString sValue1,sValue2;	

	// ����� , ������ , �ݳ���
	for( i = 0 ; i < nCnt3 ; i++ )
	{
		sWhere.Empty();
		sValue1.Empty(); sValue2.Empty();
	
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sDBAlias3[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[i] , _T("��") , sValue1 );
		GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[i+1] , _T("��") , sValue2 );

		if( !sValue1.IsEmpty() )
		{
			if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
			sPrintSearchWhere += sDBAlias3[i] + CString(_T(":")) + sValue1;

			/* 2007.06.17 add by pdw
			*/
			// 18.09.27 JMJ KOL.RED.2018.008
			if ((_T("�ִ�����ڷ�")==sStaticsClass && _T("�����")==sDBAlias3[i]) || (_T("�ִ�����ڷ�(å�η�����)")==sStaticsClass && _T("�����")==sDBAlias3[i]) )
			{
				sFieldName=_T("BB.REG_DATE");
			}
			// 2012.10.25 ADD BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (ȸ���� �߱�Ƚ����� �߰��� ���� ���� ����)
			else if (_T("ȸ�����߱����")==sStaticsClass )
			{
				sFieldName = _T("CM.REISSUE_DATE");
			}
			///////////////////////////

	
			if( !sValue2.IsEmpty() )
			{
				sWhere.Format( _T(" %s BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
				sPrintSearchWhere += _T("~") + sValue2;
			}
			else
				sWhere.Format( _T(" %s BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
			
			sPrintSearchWhere += SPACE_STRING;			
		}

		if( !sWhere.IsEmpty() )
		{
			// 18.09.27 JMJ KOL.RED.2018.008
			//if( _T("�ִ�����ڷ�")==sStaticsClass && _T("�����")==sDBAlias3[i] ) 
			if( (_T("�ִ�����ڷ�")==sStaticsClass && _T("�����")==sDBAlias3[i]) || (_T("�ִ�����ڷ�(å�η�����)")==sStaticsClass && _T("�����")==sDBAlias3[i]) ) 
			{
				sOutWhere += _T(" AND ") + sWhere;
			}
			else
			{
				if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
					sOutWhere += _T(" AND ") + sWhere;
				else
					sInWhere += _T(" AND ") + sWhere;
			}
		}

		i++;
	}

	// 2011.10.04 ADD BY PWR : ����ȸ������ �߰�
	// 18.09.27 JMJ KOL.RED.2018.008
	if( _T("�ִ�����ڷ�") != sStaticsClass && _T("�ִ�����ڷ�(å�η�����)") != sStaticsClass )
	{
		GetCellData(m_pUserSearchCond, _T("��Ʈ�Ѿ˸��ƽ�"), _T("����ȸ������"), _T("��"), sValue);
		if(!sValue.IsEmpty() && _T("��ü") != sValue)
		{
			if(_T("�Ϲ�ȸ��") == sValue)
			{
				sWhere.Format(_T(" (CLU.KL_MEMBER_YN IS NULL OR CLU.KL_MEMBER_YN='N') "), sFieldName, sFieldName);
				if(!sPrintSearchWhere.IsEmpty()) sPrintSearchWhere += _T(",");
				sPrintSearchWhere += _T("����ȸ������:�Ϲ�ȸ�� ");
			}
			else if(_T("����ȸ��") == sValue)
			{
				sWhere.Format(_T(" CLU.KL_MEMBER_YN='Y' "), sFieldName);
				if(!sPrintSearchWhere.IsEmpty()) sPrintSearchWhere += _T(",");
				sPrintSearchWhere += _T("����ȸ������:����ȸ�� ");
			}
			if(1 == IsFieldFromTable(_T("CLU.KL_MEMBER_YN"), saTableName))
			{
				sOutWhere += _T(" AND ") + sWhere;
			}
			else
			{
				sInWhere += _T(" AND ") + sWhere;
			}
		}
	}

	// 2009.03.27 ADD BY KSJ : �˻������� �� ��� ȭ�� ��¿��� ©���� ������ ���Ͽ� ������ ���� ���� �̻��̸� ���๮�ڸ� �߰��Ѵ�.
	INT		nWhereLength	=	sPrintSearchWhere.GetLength();
	if(		_T("�ִ�����ڷ�")		== sStaticsClass 
		// 18.09.27 JMJ KOL.RED.2018.008
		// �ִ�����ڷῡ å�η� ����
		||	_T("�ִ�����ڷ�(å�η�����)")		== sStaticsClass 
		||	_T("�ٵ���")			== sStaticsClass 
		||  _T("�������ٵ���")		== sStaticsClass	
		||  _T("��ü�����")	== sStaticsClass	)
	{
		if( 52 < nWhereLength )
		{
			CString sLeft, sRight;
			sLeft = sPrintSearchWhere.Left(52);
			sRight = sPrintSearchWhere.Mid(52);
			sPrintSearchWhere = sLeft + _T("\r\n") + sRight;
		}
	}

	return 0;

	EFS_END
	return -1;

}

// 2009.01.06 ADD BY PWR
// ** å�η���迡�� �����ǰ� �����Ǳ��ϱ�
INT CStaticsInfoMgr::GetSearchWhereCooperative( CString sTableName , CString &sInWhere , CString &sOutWhere , CString &sPrintSearchWhere )
{
EFS_BEGIN

	INT ids;
	CStringArray saTableName;
	CString sDBField;

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

	CString sStaticsClass;
	GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN") , _T("�������") , sStaticsClass , -1 , 0 );

	// ��������, �����Ȳ
	const INT nCnt1 = 2;
	TCHAR *sControlAlias1[nCnt1] =
	{
		_T("��������"), _T("�������")
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING, ONESTRING
	};

	CString sWhere;
	CString sValue;
	CString sFieldName;
	for( INT i = 0 ; i < nCnt1 ; i++ )
	{
		sValue.Empty();
		sWhere.Empty();

		if(sControlAlias1[i] == _T("��������"))
		{
			// 18.09.27 JMJ KOL.RED.2018.008
			if(sStaticsClass==_T("�����ڷ����(�߼�)"))
			{
				sControlAlias1[i] = _T("������������");
			}
			//�����ڷ���迡���� ��û�������� �����ϵ��� �ݿ�
			else if(sStaticsClass==_T("��û�ڷ����(�Լ�)") || sStaticsClass==_T("���� �ڷ� ���") ||  sStaticsClass==_T("�����ڷ����") || sStaticsClass==_T("���������") )
 			{
				sControlAlias1[i] = _T("��û��������");
			}
			//�ݳ��� ��� ����� �̵�
			else if(sStaticsClass==_T("�ݳ��ڷ����") || sStaticsClass==_T("�ݳ������") )
			{
				sControlAlias1[i] = _T("�ݳ���������");
			}
			else
				sControlAlias1[i] = _T("��������");
		}

		GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias1[i] , _T("��") , sValue );

		if( sValue.IsEmpty() ) continue;
		// ��������̸� ���ǿ� �߰����Ѵ�.
		if(sValue == _T("----")) continue;

		// �ʵ�� ��������
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSearchWhere1110") );		

		if( sFieldType1[i] == ONESTRING )
			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue);
		else if( sFieldType1[i] == ONENUMERIC )
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue );

		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
		
		/*
		if( i == 0 )// �з���ȣ����
			m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�з�ǥ����") , sValue , sValue );
		else		// ��������
			m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );
		*/
		m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );

		sPrintSearchWhere += sControlAlias1[i] + CString(_T(":")) + sValue + SPACE_STRING;
	}

	// ��û��, ������, �����
	sWhere.Empty();
//	const INT nCnt3 = 4;
	// 18.09.27 JMJ KOL.RED.2018.008
	// å�η���� �Լ��� �߰�
	const INT nCnt3 = 10;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("��û��1") , _T("��û��2") , _T("������1") , _T("������2"),
		_T("å�η�������1") , _T("å�η�������2") , _T("å�η��ݳ���1") , _T("å�η��ݳ���2"),
		_T("�Լ���1") , _T("�Լ���2")

//		_T("��û��1") , _T("��û��2") , _T("������1") , _T("������2")
	};

	TCHAR *sDBAlias3[nCnt3] =
	{
		_T("��û��") , _T("��û��") , _T("������") , _T("������"),
		_T("å�η�������") , _T("å�η�������") , _T("å�η��ݳ���") , _T("å�η��ݳ���"),
		_T("�Լ���") , _T("�Լ���")
//		_T("��û��") , _T("��û��") , _T("������") , _T("������")
	};

	CString sValue1,sValue2;
	for( i = 0 ; i < nCnt3 ; i++ )
	{
		sWhere.Empty();
		sValue1.Empty(); sValue2.Empty();
	
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sDBAlias3[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[i] , _T("��") , sValue1 );
		GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[i+1] , _T("��") , sValue2 );

		if( !sValue1.IsEmpty() )
		{
			if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
			sPrintSearchWhere += sDBAlias3[i] + CString(_T(":")) + sValue1;

			if( !sValue2.IsEmpty() )
			{
				sWhere.Format( _T(" %s BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
				sPrintSearchWhere += _T("~") + sValue2;
			}
			else
				sWhere.Format( _T(" %s BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
			
			sPrintSearchWhere += SPACE_STRING;			
		}

		if( !sWhere.IsEmpty() )
		{
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}

		i++;
	}

	return 0;

EFS_END
return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   �޴� �˸��ƽ��� �����Ѵ�.
//   ������� ������������ ���� ��ɰ��� ���ɼ��� ���� ������ �Լ��� ����
//
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
CString CStaticsInfoMgr::GetMenuName()
{
EFS_BEGIN

	return m_strMenuName;

EFS_END
	return _T("");
}

INT CStaticsInfoMgr::GetSearchWhere1440( CString sTableName , CString &sInWhere , CString &sOutWhere , CString &sPrintSearchWhere )
{
	EFS_BEGIN

	INT ids;

	// �˻� ���� - �̰� �밡�� ��¿�� ���� �Ѥ�
	CStringArray saTableName;
	CString sDBField;

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

	// �����⵵
	const INT nCnt1 = 1;
	TCHAR *sControlAlias1[nCnt1] =
	{
		_T("�����⵵")
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING 
	};


	CString sWhere;
	CString sValue;
	CString sFieldName;
	for( INT i = 0 ; i < nCnt1 ; i++ )
	{
		sValue.Empty();
		sWhere.Empty();

		GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias1[i] , _T("��") , sValue );

		if( sValue.IsEmpty() ) continue;

		// �ʵ�� ��������
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		if( sFieldType1[i] == ONESTRING )
			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue);
		else if( sFieldType1[i] == ONENUMERIC )
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue );

		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

		sPrintSearchWhere += sControlAlias1[i] +CString(_T(":"))+ sValue + SPACE_STRING;
		
		 
	}


	// ����
	sWhere.Empty();
	const INT nCnt3 = 2;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("����1") , _T("����2") 
	};


	CString sValue1,sValue2;
	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[1] , _T("��") , sValue2 );


	// ���ι�ȣ
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("����") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += _T("����:")+sValue1;
		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN %s AND %s ") , sFieldName , sValue1 , sValue2 );		
			sPrintSearchWhere += _T("~")+sValue2;
		}
		else
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}
	return 0;

	EFS_END

	return -1;

}

// ##############################################################################################################
// [ �Լ����� ]
//   ��� DM�� ����Ѵ�.
//
// [ �̴Ͻ��� ]
// ##############################################################################################################
INT CStaticsInfoMgr::PrintResult( CString sStaticsClass , bool bDupDel , bool bDelZero  , CTabCtrl* pTab )
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. ����, �Ϲ� ��� ����
	// ==========================================================================================================
	CString sXAlias , sYAlias , sOption , sOrder;
	ids = GetXYAlias( sStaticsClass , sXAlias , sYAlias , sOption , sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::PrintResult()") );	

	if( sOrder.Compare( _T("-1") ) == 0 )
	{
		ids = MakeStatPrintDM( sStaticsClass , bDupDel , bDelZero );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::PrintResult()") );
		if( ids > 0 ) return ids;
	}
	else
	{	
		ids = MakeRankingPrintDM( sStaticsClass , pTab );
		if( ids > 0 ) return ids;
	}
	
	return 0;
EFS_END
	return -1;
}


// ##############################################################################################################
// [ �Լ����� ]
//   �Ϲ� ��� ��� DM�� ����Ѵ�.
//
// [ �̴Ͻ��� ]
// ##############################################################################################################
INT CStaticsInfoMgr::MakeStatPrintDM( CString sStaticsClass , bool bDupDel , bool bDelZero )
{
EFS_BEGIN

	INT ids;
	CString sXAlias,sYAlias,sPrintName,sPrintTitle,sSearchCond;	

	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("X�༱�þ˸��ƽ�") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );
	
	// 2012.11.20 ADD BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (����������ȣ �Ǹ����� ��뿡 ���� ���� ��� ���)
	if(sXAlias.Compare(_T("����2"))==0)	sXAlias = _T("����");

	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("Y�༱�þ˸��ƽ�") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );
	
	CESL_DataMgr* pPrintDM;
	// UPDATE BY ���� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �弭���� ��迡 ������ �߰��Ǹ鼭 ���� ó���� �ش�.	
	if( _T("�弭�������") == sStaticsClass )
	{
		pPrintDM = m_pPrintMgr->GetPrintDM( _T("�弭����_�з���ȣ��") );
		MakePrintDMFromGrid( pPrintDM );
	}
	else
	{
		pPrintDM = m_pPrintMgr->GetPrintDM( sXAlias );
		if( !pPrintDM ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );

		ids = MakePrintDMFromResultDM( sStaticsClass , bDupDel , pPrintDM , sXAlias , bDelZero );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );
		if( ids > 0 ) return ids;
	}
	// ----------------------------------------------------------------------------------------------------------

	// UPDATE BY ���� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �弭���� ��迡 �����߰��ϸ� rpt ���� ����
	if( _T("�弭�������") == sStaticsClass )
		sPrintName.Format( _T("�弭����_�з���ȣ�����") );
	// 2006-04-07 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ���� ��� �и�
	else
	{
		sPrintName.Format( _T("�������_%s���") , sXAlias );
		if ( m_strMenuName == SE_HOLDING_STATICS )
			sPrintName.Format( _T("�������_����_%s���") , sXAlias );
	}
	// ----------------------------------------------------------------------------------------------------------

	if( sXAlias.Compare(_T("��")) == 0 || (sXAlias.Compare(_T("��")) == 0) || (sXAlias.Compare(_T("��")) == 0) )
	{		
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sXAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );
	}	sPrintTitle.Format( _T("�� : '%s' , �� : '%s'") , sYAlias , sXAlias );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����ڰ˻�����_���") , sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);

	if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
	{
		CString sRptFileFullPath;
		sRptFileFullPath.Format(_T("..\\rpt\\%s.rpt") , sPrintName );
		
		ids = pSIReportManager->InitWorkFormFile( sRptFileFullPath , sStaticsClass , 1 );
		if (ids < 0) {
			CString sEMsg;
			sEMsg.Format(_T("File�� ���� SI Report ������ �������� ���Ͽ����ϴ�.[%s]") , sRptFileFullPath);
			AfxMessageBox(sEMsg);
			return 2;
		}
	}
	else
	{
		ids = pSIReportManager->GetSIReportFileFromDB(sPrintName);
		if (ids < 0) {
			AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
			return 1;
		}	
	}

	///{{++
	/// 2008.04.17 update by pdw
	/// ��� ������ �����Ҽ� �ֵ��� �Ѵ�
//	pSIReportManager->SetTempVariable( _T("�������") , sStaticsClass );	
	GetControlData(_T("CM_BO_STATICS_1100") , _T("�������") , sStaticsClass , -1 , 0 );
	pSIReportManager->SetTempVariable( _T("�������") , sStaticsClass );
	///}}--

	pSIReportManager->SetTempVariable( _T("������") , sPrintTitle );
	pSIReportManager->SetTempVariable( _T("��") , sXAlias );
	pSIReportManager->SetTempVariable( _T("��") , sYAlias );
	pSIReportManager->SetTempVariable( _T("�˻�����") , sSearchCond );
	
	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return 0;

EFS_END
	return -1;
}

// ����� �׸��忡 �ѷ��ֱ�
INT CStaticsInfoMgr::MakePrintDMFromResultDM( CString sStaticsClass , bool bDupDel , CESL_DataMgr* pTargetDM , CString sXAlias , bool bDelZero )
{
	EFS_BEGIN

	INT ids;

	pTargetDM->FreeData();
	for( INT i = 0 ; i < pTargetDM->RefList.GetCount() ; i++ )
	{
		pTargetDM->SetDataMgrRef( i , _T("-1") , _T("") , _T("") , _T("") , _T("") );
	}
	
	if( m_pRealResultDM->GetRowCount() < 1 ) return 0;

	CString sSumAlias;
	if( !bDupDel )
		sSumAlias = _T("�ߺ����_�հ�");
	else
		sSumAlias = _T("�ߺ�����_�հ�");

	INT nLimitCnt = pTargetDM->RefList.GetCount();
	INT nStatColCnt = m_pRealResultDM->RefList.GetCount();

	// DM �޸� �����ϱ�
	CString sTmpSum;
	for( i = 0 ; i < m_pRealResultDM->GetRowCount()+2 ; i++ )
	{
		// �հ谡 0�� �� �Է¾��ϱ�
		if( !bDelZero )
		{
			if( i !=0 && i != m_pRealResultDM->GetRowCount()+1 )
			{
				m_pXSumDM->GetCellData( sSumAlias , i-1 , sTmpSum );
				if( _ttoi(sTmpSum) == 0 )
					continue;
			}
		}
		pTargetDM->InsertRow(-1);
	}


	// X�� �÷� �ѷ��ֱ�
	INT refcount = m_pRealResultDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = m_pRealResultDM->RefList.GetHeadPosition();
	CString query, fields, isudf;

	CString sResult;
	CString sColumnAlias;

	INT nCopyColCnt = nStatColCnt;
	if( nStatColCnt > nLimitCnt )
	{
		CString sMsg;
		sMsg.Format(_T("���� ������ �ʹ� ���� ��¹��� ��� ǥ���Ҽ� �����ϴ�.\r\n������ �κ��� [%d]��° �̻���� ������� ����� �ؾ��մϴ�.") , nLimitCnt-1 );
		AfxMessageBox( sMsg );
		nCopyColCnt = nLimitCnt;
	}
	
	INT nInsertIndex;
	for( i = 0 ; i < nCopyColCnt ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)m_pRealResultDM->RefList.GetNext(pos);

		sResult = pRef->FIELD_ALIAS;
		sResult.TrimLeft(); sResult.TrimRight();

		if( sResult.IsEmpty() || (sResult.Compare(_T("�������")) == 0)  )
		{
			sResult = _T("�������");
		}

		sColumnAlias.Format(_T("%d"),i);
		pTargetDM->SetDataMgrRef( i , sColumnAlias , _T("") , _T("") , _T("") , _T("") );
			
		if( sResult.Compare(_T("�÷���")) == 0 )
			sResult = _T("");
		else
		{
			if( sXAlias.Compare(_T("��")) == 0 )
			{
				sResult = sResult.Right(2);
			}
		}
		pTargetDM->SetCellData( 0 , i , sResult );

		nInsertIndex = 1;
		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{

			ids = m_pXSumDM->GetCellData( sSumAlias , j , sTmpSum );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			if( !bDelZero && _ttoi(sTmpSum) == 0 )
				continue;
			
			ids = m_pRealResultDM->GetCellData( pRef->FIELD_ALIAS , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			if( sResult.Compare(_T("�������")) == 0 )
			{
				sResult = _T("�������");
			}

			pTargetDM->SetCellData( nInsertIndex , i , sResult );

			nInsertIndex++;
		}

	}



	// ���� �հ� �ѷ��ֱ�
	pTargetDM->SetCellData( nInsertIndex , 0 , _T("�հ�") );	
	for( i = 0 ; i < nCopyColCnt ; i++ )
	{
		for( INT j = 0 ; j < nCopyColCnt-1 ; j++ )
		{
			ids = m_pYSumDM->GetCellData( sSumAlias , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			pTargetDM->SetCellData( nInsertIndex , j+1 , sResult );
		}
	}


	// ���� �հ� �ѷ��ֱ�
//	INT nSumCol = m_pRealResultDM->RefList.GetCount();
	INT nSumCol = nCopyColCnt;
	sColumnAlias.Format( _T("%d") , nLimitCnt-1 );
	pTargetDM->SetCellData( 0 , nLimitCnt-1 , _T("�հ�") );
	pTargetDM->SetDataMgrRef( nLimitCnt-1 , sColumnAlias , _T("") , _T("") , _T("") , _T("") );
	
	CString sSum;
	nInsertIndex = 1;
	for( i = 0 ; i < m_pXSumDM->GetRowCount() ; i++ )
	{
		ids = m_pXSumDM->GetCellData( sSumAlias , i , sSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

		// 16/07/20 ������ : �����ڷ����/�ݳ��ڷ����� ����� ���� �ʴ� ���� ����
/*// BEFORE ----------------------------------------------------------------------------------
		if( !bDelZero && _ttoi(sSum) == 0 )
*/// AFTER ----------------------------------------------------------------------------------
		if( !bDelZero && _ttoi(sSum) == 0  && !((_T("���� �ڷ� ���") == sStaticsClass || _T("�ݳ��ڷ����") == sStaticsClass) && pTargetDM->GetRowCount() <= 2 ) )
//*/ END -------------------------------------------------------------------------------------	
			continue;

		pTargetDM->SetCellData( nInsertIndex , nLimitCnt-1 , sSum );
		nInsertIndex++;
	}

	
	
	
	/*
	if( _T("�弭�������") == sStaticsClass )
	{
		// ���� ���� �հ�
		pTargetDM->SetCellData( 0, 12, _T("�����հ�") );
		for( i = 0; i < m_pXPriceSumDM->GetRowCount(); i++ )
			pTargetDM->SetCellData( i + 1, 12, m_pXPriceSumDM->GetCellData( _T("�ߺ�����_�հ�"), i ) );
		
		// ���� ���� �հ�
		pTargetDM->InsertRow( - 1 );
		INT lastRow = pTargetDM->GetRowCount();
		pTargetDM->SetCellData( lastRow, 0, _T("�����հ�") );
		for( i = 0; i < m_pYPriceSumDM->GetRowCount(); i++ )
			pTargetDM->SetCellData( lastRow, i + 1 , m_pYPriceSumDM->GetCellData( _T("�ߺ�����_�հ�"), i ) );
	}*/
	
	
	
	
	
	
	
	return 0;

	EFS_END
	return -1;

}


INT CStaticsInfoMgr::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}



/// ��¹� DM����
INT CStaticsInfoMgr::MakeRankingPrintDM( CString sStaticsClass , CTabCtrl* pTab )
{
	EFS_BEGIN
		
	CStringArray saTableName;
	CString sTableName;
	INT ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�˻����̺��̸�") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );
		
	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );
		
	CESL_DataMgr* pSrcDM;
	CESL_DataMgr* pPrintDM;

	pPrintDM = m_pPrintMgr->GetPrintDM( sStaticsClass );
	if( pPrintDM == NULL )
	{
		AfxMessageBox(_T("�����Ǵ� ��¹��� �������� �ʽ��ϴ�."));
		return 1;
	}
	
	CString sPrintTitle,sSearchCond,sOption;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����ڰ˻�����_���") , sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePrintDM") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����") , sOption );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePrintDM") );


	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);

	pSIReportManager->m_nDefaultPaperType = PAPER_A4_PORTRAIT;

	if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
	{
		CString sRptFileFullPath;
		sRptFileFullPath.Format(_T("..\\rpt\\%s.rpt") , sStaticsClass );
		
		ids = pSIReportManager->InitWorkFormFile( sRptFileFullPath , sStaticsClass , 1 );
		if (ids < 0) {
			CString sEMsg;
			sEMsg.Format(_T("File�� ���� SI Report ������ �������� ���Ͽ����ϴ�.[%s]") , sRptFileFullPath);
			AfxMessageBox(sEMsg);
			return 2;
		}
	}
	else
	{
		ids = pSIReportManager->GetSIReportFileFromDB(sStaticsClass);
		if (ids < 0) {
			AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
			return 2;
		}
		
	}


	pSIReportManager->SetDataMgr( 0 , pPrintDM , 0 );

	ids = pSIReportManager->DeleteTempPageFiles();

	// 2007.06.20 update by pdw
	// ����Ƚ����� ����ó��	
	// 18.09.27 JMJ KOL.RED.2018.008
	if( _T("����Ƚ�����")==sStaticsClass || _T("����Ƚ�����(å�η�����)")==sStaticsClass )
	{
		///{{++
		/// 2008.04.17 update by pdw
		/// ��� ������ �����Ҽ� �ֵ��� �Ѵ�
	//	pSIReportManager->SetTempVariable( _T("�������") , sStaticsClass );	
		GetControlData(_T("CM_BO_STATICS_1100") , _T("�������") , sStaticsClass , -1 , 0 );
		pSIReportManager->SetTempVariable( _T("�������") , sStaticsClass );
		///}}--
		pSIReportManager->SetTempVariable( _T("�˻�����") , _T("�˻����� : ") + sSearchCond );

		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// ��ũ�� Manual �ݺ� ���
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintUserList") );
	}
	else
	{
		CStringArray asColumnName;
		if( sOption.Compare(_T("��ü")) != 0 )
		{
			ids = GetColumnNames( sStaticsClass , sOption , asColumnName );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeOrderStaticsDM") );
		}
		else
			asColumnName.Add(sOption);

		for( INT i = 0 ; i < asColumnName.GetSize() ; i++ )
		{
			pSrcDM = &m_pOrderStaticsDM[saTableName.GetSize()][i];
			
			CLocCommonAPI::AddDM( pSrcDM , pPrintDM , this );

			if( pPrintDM->GetRowCount() == 0 ) continue;

			if(sOption.Compare(_T("��ü")) != 0 )
				sPrintTitle.Format( _T("���� : %s(%s)") , sOption , asColumnName.GetAt(i) );
			else
				sPrintTitle.Format( _T("") );

			if ( sPrintTitle == _T("") )
			{			
				pSIReportManager->SetTempVariable( _T("����������") , sStaticsClass );
			}
			else
			{
				pSIReportManager->SetTempVariable( _T("����������") , sPrintTitle );
			}

			///{{++
			/// 2008.04.17 add by pdw
			/// ��� ������ �����Ҽ� �ֵ��� �Ѵ�
			CString strTitle;
			GetControlData(_T("CM_BO_STATICS_1100") , _T("�������") , strTitle , -1 , 0 );
			pSIReportManager->SetTempVariable( _T("�������") , strTitle );
			///}}--

			pSIReportManager->SetTempVariable( _T("�˻�����") , _T("�˻����� : ") + sSearchCond );

			ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// ��ũ�� Manual �ݺ� ���
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintUserList") );

			pPrintDM->FreeData();
		}
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


// �׸����� ��� ���� ��� ���ķ� 
INT CStaticsInfoMgr::SetGridCellAlignment(CMSHFlexGrid* pGrid)
{
	EFS_BEGIN

	INT nColCnt = pGrid->GetCols(0);
	INT nRowCnt = pGrid->GetRows();

	
	
	for( INT i = 0 ; i < nColCnt ; i++ )
	{
		if( i == 0 )
			pGrid->SetColAlignment(i,1);
		else
			pGrid->SetColAlignment(i,4);

		pGrid->SetRow(0);
		pGrid->SetCol(i);
		pGrid->SetCellFontBold(TRUE);
		pGrid->SetCellAlignment(4);

	}

	for( INT j = nRowCnt-1 ; j >= 0 ; j-- )
	{
		pGrid->SetCol(0);
		pGrid->SetRow(j);
		pGrid->SetCellFontBold(TRUE);	
	}

	return 0;

	EFS_END

	return -1;
}

CString CStaticsInfoMgr::ConvertPrice(CString price)
{
	INT len = price.GetLength();
	if( len < 4 )
		return price;

	INT cnt = 0;
	CString tmp;
	for( INT i = len -1; i > -1; i-- )
	{
		cnt++;
		tmp = price.Mid( i, 1 ) + tmp;

		if( 3 == cnt )
		{
			tmp = _T(",") + tmp;
			cnt = 0;
		}		
	}
	
	if( ',' == tmp.GetAt( 0 ) )
		price = tmp.Mid( 1, tmp.GetLength() - 1 );
	else
		price = tmp;
	
	return price;
}



INT CStaticsInfoMgr::MakePrintDMFromGrid( CESL_DataMgr *pDM )
{
	CString CMAlias = _T("CM_BO_LOC_2100_STATICS_GRID");
	CESL_ControlMgr *pCM = FindCM( CMAlias );

	CESL_Grid *pGrid = ( CESL_Grid * )( pCM->FindControl( _T("���׸���") ) );

	INT cols = pGrid->GetCols( 0 );
	INT rows = pGrid->GetRows();

	pDM->FreeData();
	CString colAlias;
	for( INT i = 0; i < rows; i++ )
	{
		pDM->InsertRow( -1 );
		for( INT j = 0; j < cols; j++ )
		{
			colAlias.Format( _T("%d"), j );
			pDM->SetCellData( colAlias, pGrid->GetTextMatrix( i, j ), i );
		}
	}
	
	return 0;
}

// 2005.07.28 ADD BY PDJ
// �з���ȣ 3���� �Ϲݰ� ����з���ȣ�� �и��Ͽ� ����
INT CStaticsInfoMgr::DividNormalArmy(CString strClassNoIn, CString &f_strNormalOut, CString &f_strArmyOut)
{
	INT	nStartPos, nEndPos;
	CString strNormal, strArmy;
	
	// ADD BY PDW(jr) 
	// ���� ���� 
	strClassNoIn.Replace( _T(" "), _T("") );
	nStartPos = strClassNoIn.FindOneOf( DIVCHAR );
	nEndPos	  = strClassNoIn.GetLength();
	if ( nStartPos == -1 )
	{					
		// �Ϲ��ڷᰡ ������ ���
		strArmy = _T("0");
		strNormal = strClassNoIn;		
	}				
	else if (nStartPos == 0)
	{
		// �����ڷᰡ ������ ���	
		strNormal = _T("0");
		strArmy = strClassNoIn;		
	}
	else
	{
		// ������ �Ǿ� �ִ°��
		strNormal = strClassNoIn.Left(nStartPos);
		strArmy   = strClassNoIn.Mid(nStartPos+1, nEndPos-nStartPos-1);
	}

	if ( strNormal.GetLength()	== 0 ) strNormal=_T("0");
	if ( strArmy.GetLength()	== 0 ) strArmy=_T("0");

	f_strNormalOut = strNormal;
	f_strArmyOut = strArmy;	

	return 0;
}

INT CStaticsInfoMgr::MakeFunctionFieldName(CString strFuncNameIn, CString &strFieldNameOut)
{	
	INT nStartPos;
	INT nEndPos;

	if ( strFuncNameIn.Find(_T("$"), 0 ) > -1 )
	{	
		nStartPos = strFuncNameIn.Find(_T("("), 0 );
		nEndPos = strFuncNameIn.Find(_T(")"), 0 );			
		strFieldNameOut = strFuncNameIn.Mid(nStartPos+1, nEndPos-nStartPos-1);
	}
	else
	{
		strFieldNameOut = strFuncNameIn;
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ������ ��迡�� �����ڷ� ���� ��/å ������ �����ϱ� ���� 
// �Էµ� �����Ϳ� �ش� �˸��ƽ� �κ���  å�Ǽ��� �ο��Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////
INT CStaticsInfoMgr::DividNormalArmy2(CString sPreData, CString sXAlias, CString sData, INT nMode, CString *psGetData)
{
	CString sGetNormal, sGetArmy;
	///////////////////////////////////////////////////////////////////////
	// 1. �������� �Է�
	///////////////////////////////////////////////////////////////////////
	if ( nMode == 0 )
	{		
		DividNormalArmy( sPreData, sGetNormal, sGetArmy);
		if ( sXAlias == _T("3") )
		{
			psGetData->Format( _T("%d %s %d"), _ttoi( sGetNormal ) + _ttoi( sData ), DIVCHAR, _ttoi( sGetArmy ) );
		}
		else
		{
			psGetData->Format( _T("%d %s %d"), _ttoi( sGetNormal ), DIVCHAR, _ttoi( sGetArmy ) + _ttoi( sData ) );
		}
	}
	///////////////////////////////////////////////////////////////////////
	// 2. å������ �Է�
	///////////////////////////////////////////////////////////////////////
	else if ( nMode == 1 )
	{
		INT nPos = sPreData.Find( _T("\r\n") );
		CString sGetBoNormal, sGetBoArmy, sTemp;
		// 2.1) ���� �Է�
		if ( nPos == -1 )
		{
			DividNormalArmy( sPreData, sGetNormal, sGetArmy);
			if ( sXAlias == _T("3") )
			{
				sGetBoNormal.Format( _T("%d") , _ttoi(sData) );
				sGetBoArmy = _T("0");
			}
			else
			{
				sGetBoNormal = _T("0");
				sGetBoArmy.Format( _T("%d") , _ttoi(sData) );
			}			
		}
		// 2.2) �������� ����
		else
		{
			sTemp = sPreData.Mid( nPos + 2 );
			sPreData = sPreData.Mid( 0, nPos );
			sTemp.Replace(_T("("),_T(""));sTemp.Replace(_T(")"),_T(""));
			sTemp.TrimLeft();sTemp.TrimRight();
			DividNormalArmy( sTemp, sGetNormal, sGetArmy);

			if ( sXAlias == _T("3") )
			{
				sGetBoNormal.Format( _T("%d") ,  _ttoi(sData) );						
				sGetBoArmy = sGetArmy;
			}
			else
			{		
				sGetBoArmy.Format( _T("%d") , _ttoi(sData) );
				sGetBoNormal = sGetNormal;
			}
		}

		sTemp.Format( _T("(%d %s %d)"), _ttoi( sGetBoNormal ), DIVCHAR, _ttoi( sGetBoArmy ) );
		psGetData->Format( _T("%s\r\n%s") , sPreData, sTemp );
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ���� ������ ��� �հ踦 ���� �۾�
//////////////////////////////////////////////////////////////////////////////////////////
INT CStaticsInfoMgr::MakeArmySpeciesBookCnt(CString sData, CString *sGetData)
{
	INT nPos = sData.Find( _T("\r\n") );
	
	//////////////////////////////////////////////////////////////////////////////////
	// 1. �����ϰ��
	//////////////////////////////////////////////////////////////////////////////////
	if ( nPos == -1 )
	{
		//ex) 5 / 6    �Ѵ� ���� ������ -> 11�� �����Ѵ�.
		// �����̽��� ���� 
		sData.Replace( _T(" "), _T("") );
		nPos = sData.Find( DIVCHAR, 0 );
		// ������ ���� 
		if ( nPos == -1 ) 
			*sGetData = sData;
		else
		{
			CString sValue1, sValue2;
			sValue1 = sData.Mid( 0, nPos );
			sValue2 = sData.Mid( nPos + 1 );
			sGetData->Format( _T("%d"), _ttoi( sValue1 ) + _ttoi( sValue2 ) );
		}

	}
	//////////////////////////////////////////////////////////////////////////////////
	// 2. �����ϰ��
	//////////////////////////////////////////////////////////////////////////////////
	else
	{
		// ex)  5 / 6     ��, å�� ���ļ� 11\r\n(13)���� �����Ѵ�.
		//    ( 6 / 7 ) 
		// ��å�� �и�
		CString sValue1, sValue2;
	    CString sSpeices = sData.Mid( 0, nPos );
		CString sBook    = sData.Mid( nPos + 2 );

		// �� ī��Ʈ ��ġ�� 
	    sSpeices.Replace( _T(" "), _T("") );
		nPos = sSpeices.Find( DIVCHAR, 0 );		
		if ( nPos != -1 ) 
		{
			sValue1 = sSpeices.Mid( 0, nPos );
			sValue2 = sSpeices.Mid( nPos + 1 );
			sSpeices.Format( _T("%d"), _ttoi( sValue1 ) + _ttoi( sValue2 ) );
		}

		// å ī��Ʈ ��ġ��
		sBook.Replace( _T(" "), _T("") );
		sBook.Replace( _T("("), _T("") );sBook.Replace( _T(")"), _T("") );
		nPos = sBook.Find( DIVCHAR, 0 );
		if ( nPos != -1 ) 
		{
			sValue1 = sBook.Mid( 0, nPos );
			sValue2 = sBook.Mid( nPos + 1 );
			sBook.Format( _T("%d"), _ttoi( sValue1 ) + _ttoi( sValue2 ) );
		}

		sGetData->Format( _T("%s\r\n(%s)"), sSpeices, sBook );
	}

	return 0;
}

// ##############################################################################################################
// [ �Լ����� ]
//   �⺻ DB ��ȸ ���� ����
//
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::SetManageValue()
{
	INT ids;

	// ==========================================================================================================
	// 1. ������ �÷��� ����
	// ==========================================================================================================
	ids = GetManageValue( _T("��Ÿ") , _T("����") , _T("����") ,  _T("����")  , m_sIs64lib );
	if ( ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::SetManageValue()") );
	m_sIs64lib.TrimLeft();m_sIs64lib.TrimRight();

	ids = GetManageValue( _T("��Ÿ") , _T("����") , _T("�����") , _T("")     , m_sIsNDlib );
	if ( ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::SetManageValue()") );
	m_sIsNDlib.TrimLeft();m_sIsNDlib.TrimRight();

	ids = GetManageValue( _T("��Ÿ") , _T("����") , _T("5163") ,   _T("5163") , m_sIs5163lib );
	if ( ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::SetManageValue()") );
	m_sIs5163lib.TrimLeft();m_sIs5163lib.TrimRight();

	// 2009.10.12 ADD BY PWR : �����ڹ�ȣ �ֹι�ȣ�϶� *ǥ��
	ids = GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), m_sIsCivilNoHide);
	if ( ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::SetManageValue()") );
	m_sIsCivilNoHide.TrimLeft();m_sIsCivilNoHide.TrimRight();

	// 2010.09.08 ADD BY KSJ : ���յ������� ��뿩��
	GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");

	return 0;	
}

// ##############################################################################################################
// [ �Լ����� ]
//   ����_��������� �˻������� ���ϱ�.
//
// [ �̴Ͻ��� ]                          
// ##############################################################################################################
INT CStaticsInfoMgr::GetSearchWhere1113(CString sStaticsClass,CString sTableName, CString &sInWhere, CString &sOutWhere, CString &sPrintSearchWhere)
{
EFS_BEGIN

	INT ids;
		
	// ==========================================================================================================
	// 1. ��迡 ���Ǵ� ���̺� ����Ʈ ����
	// ==========================================================================================================
	CStringArray saTableName;
	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );

	// ==========================================================================================================
	// 2. ���Ա��� , �������� , �з���ȣ���� ���� ����
	// ==========================================================================================================
	const INT nCnt1 = 3;
	TCHAR *sControlAlias1[nCnt1] = 
	{	
		_T("�谡�μ�") , _T("��������") , _T("�з���ȣ����") 
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING , ONESTRING , ONESTRING 
	};

	CString sWhere;
	CString sValue;
	CString sFieldName;

	for ( INT i = 0;i < nCnt1;i++ )
	{
		sValue.Empty();sWhere.Empty();

		// �ش� ��Ʈ���� ���� ���Ѵ�.
		GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias1[i] , _T("��") , sValue );
		if( sValue.IsEmpty() ) continue;

		// ��Ʈ�ѿ� �ش��ϴ� �ʵ�� ��������
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );

		// �ʵ�Ÿ�Կ� ���� ���� �Է¹�� �б�
		if( sFieldType1[i] == ONESTRING )			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue);
		else if( sFieldType1[i] == ONENUMERIC )		sWhere.Format( _T(" %s = %s ") , sFieldName , sValue );

		// �����ǰ� ������ �б�
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

		// �ڵ带 �������� ���� �Ѥ�? �� ������ 
		if( i == 0 )		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�ڷ�Ǳ���") , sValue , sValue );
		else if( i == 2 )	m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("�з�ǥ����") , sValue , sValue );
		else    			m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );
	}

	// ==========================================================================================================
	// 3. �ڷ� ���� ���� ����     
	// ==========================================================================================================
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�ڷ����") , _T("��") , sValue );
	
	if( !sValue.IsEmpty() )
	{
		if( sValue.Compare( _T("�����ڷ�(��ġ+�ǽð��谡+������+��������+�ļ�+�н�)")) == 0 )
		{
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O' ) ") );
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O', 'SEL411O', 'SEL511O', 'SEL611O' ) ") );
			//=====================================================
		}
		else if( sValue.Compare(_T("�����ڷ�(��ġ+�ǽð��谡+������)")) == 0 )
		{
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' ) ") );
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O', 'SEL411O', 'SEL511O', 'SEL611O' ) ") );
			//=====================================================
			
		}
		else if( sValue.Compare(_T("��ġ�ڷ�+�ǽð��谡�ڷ�")) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' ) ") );
		else if( sValue.Compare(_T("�������ڷ�")) == 0 )
		{
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' ) ") );
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL411O', 'SEL511O', 'SEL611O' ) ") );
			//=====================================================
		}
		else if( sValue.Compare(_T("���������ڷ�")) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL213O'  ) ") );
		else if( sValue.Compare(_T("�н��ڷ�")) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL216O' ) ") );
		else if( sValue.Compare(_T("�����ڷ�")) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL215O' ) ") );
		else if( sValue.Compare(_T("�ļ��ڷ�")) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL214O' ) ") );
		else if( sValue.Compare( _T("�����ڷ�(��ġ+�ǽð��谡+������+��������+�ļ�+�н�+������)") ) == 0 )
		{
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O', 'SEL217O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O', 'SEL217O' ) ") );
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O', 'SEL217O', 'SEL411O', 'SEL511O', 'SEL611O' ) ") );
			//=====================================================
		}
		else if( sValue.Compare( _T("�����ڷ�(��ġ+�ǽð��谡+������+������)") ) == 0 )
		{
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL217O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL217O' ) ") );
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL217O', 'SEL411O', 'SEL511O', 'SEL611O' ) ") );
			//=====================================================
		}
		else if( sValue.Compare( _T("�������ڷ�") ) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL217O' ) ") );
		
		if( IsFieldFromTable( _T("B.WORKING_STATUS") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;		

		if( (sValue.Compare(_T("�����ڷ�(��ġ+�ǽð��谡+������+��������+�ļ�+�н�)")) != 0) &&
			(sValue.Compare(_T("�����ڷ�(��ġ+�ǽð��谡+������)")) != 0) )
			sPrintSearchWhere += CString(_T("�ڷ����:")) + sValue + SPACE_STRING;		
	}	

	// ==========================================================================================================
	// 4. ��Ϲ�ȣ ���ǻ���
	// ==========================================================================================================
	sWhere.Empty();
	const INT nCnt2 = 3;
	TCHAR *sControlAlias2[nCnt2] =
	{
		_T("��Ϲ�ȣ����") , _T("��Ϲ�ȣ1") , _T("��Ϲ�ȣ2") 
	};

	CString sValue1,sValue2,sValue3;

	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias2[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias2[1] , _T("��") , sValue2 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias2[2] , _T("��") , sValue3 );

	if( !sValue1.IsEmpty() && !sValue2.IsEmpty() )
	{
		CString sRegNo1 , sRegNo2;
		sRegNo1 = sValue1+sValue2;
		sRegNo2 = sValue1+sValue3;

		CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo1 );

		sPrintSearchWhere += CString(_T("��Ϲ�ȣ:")) + sRegNo1;

		if( !sValue3.IsEmpty() )
		{
			CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo2 );
			sWhere.Format( _T(" B.REG_NO BETWEEN '%s' AND '%s' ") ,  sRegNo1 , sRegNo2 );
			sPrintSearchWhere += _T("~") + sRegNo2;
		}
		else
			sWhere.Format( _T(" B.REG_NO = '%s' ") ,  sRegNo1 );

		sPrintSearchWhere += SPACE_STRING;	
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.REG_NO") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}		 

	// ==========================================================================================================
	// 5. �谡���� ���ǻ���
	// ==========================================================================================================
	sWhere.Empty();
	const INT nCnt3 = 2;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("�谡����1") , _T("�谡����2")
	};

	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias3[1] , _T("��") , sValue2 );
	
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("�谡����") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );

	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("�谡����:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		
		sPrintSearchWhere += SPACE_STRING;		
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	// ==========================================================================================================
	// 6. �谡�������� ���ǻ���
	// ==========================================================================================================
	sWhere.Empty();
	const INT nCnt4 = 2;
	TCHAR *sControlAlias4[nCnt4] =
	{
		_T("�谡��������1") , _T("�谡��������2")
	};
	
	sValue1.Empty();sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias4[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias4[1] , _T("��") , sValue2 );
		
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("�谡��������") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );
	
	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("�谡��������:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	// ==========================================================================================================
	// 7. �������� ���ǻ��� �߰� pjw
	// ==========================================================================================================
	sWhere.Empty();
	const INT nCnt6 = 2;
	TCHAR *sControlAlias6[nCnt6] =
	{
		_T("��������1") , _T("��������2")
	};

	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias6[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias6[1] , _T("��") , sValue2 );
	
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("��������") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );

	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("��������:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		
		sPrintSearchWhere += SPACE_STRING;		
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	// ==========================================================================================================
	// 8. �����ۼ��⵵ ���ǻ���
	// ==========================================================================================================
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�����ۼ��⵵") , _T("��") , sValue );
	
	if( !sValue.IsEmpty() )
	{
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("�����ۼ��⵵") , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );

		sPrintSearchWhere += CString(_T("�����ۼ��⵵:"));

		sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue );

		sPrintSearchWhere += sValue;
		sPrintSearchWhere += SPACE_STRING;		
	}
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.A") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	// ==========================================================================================================
	// 9. ���ι�ȣ ���ǻ���
	// ==========================================================================================================
	sWhere.Empty();
	const INT nCnt5 = 2;
	TCHAR *sControlAlias5[nCnt5] =
	{
		_T("���ι�ȣ1") , _T("���ι�ȣ2") 
	};
	
	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias5[0] , _T("��") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , sControlAlias5[1] , _T("��") , sValue2 );	
		
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , _T("���ι�ȣ") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );
	
	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("���ι�ȣ:"));
		
		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN %s AND %s ") , sFieldName , sValue1 , sValue2 );		
			sPrintSearchWhere += sValue1 + _T("~") + sValue2;
		}
		else
		{
			sPrintSearchWhere += sValue1;
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue1 );
		}
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{

		if( IsFieldFromTable( _T("B.A") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}	

	// ==========================================================================================================
	// 10. �η��ڷ� ���� ���� ���ǻ���
	// ==========================================================================================================
	// 2006-04-03 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �������¿� ���������� ������ �����Ѵ�.	
	sValue.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("��������") , _T("��") , sValue );
	CString sBinded;
	if ( sValue == _T("�������") )		  sBinded = _T(" 'B' , 'C' ");
	else if ( sValue == _T("�������") )  sBinded = _T(" 'B' ");
	else if ( sValue == _T("���ǵ��") )  sBinded = _T(" 'C' ");

	sWhere.IsEmpty();
	sValue.Empty();
	GetCellData( m_pUserSearchCond , _T("��Ʈ�Ѿ˸��ƽ�") , _T("�η��ڷ�����") , _T("��") , sValue );
	
	if( !sValue.IsEmpty() )
		sWhere.Format(_T(" B.BOOK_TYPE IN (%s) ") , sBinded);
	else
		sWhere.Format(_T(" B.BOOK_TYPE IN (%s, 'A' ) ") , sBinded);

	sFieldName = _T("B.BOOK_TYPE");		
	if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
		sOutWhere += _T(" AND ") + sWhere;
	else
		sInWhere += _T(" AND ") + sWhere;
	// ----------------------------------------------------------------------------------------------------------

	//PJW����
	if(sStaticsClass == _T("�ڷ�Ǻ��̿����") || sStaticsClass == _T("�űԱ��Ե����̿����"))
	{
		sPrintSearchWhere += CString(_T("����:�����̷�å(����å)"));
	}else
	{
		sPrintSearchWhere += CString(_T("����:��[��](å)"));
	}
	/*if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += SPACE_STRING ;
	sPrintSearchWhere += CString(_T("����:��[��](å)"));
	*/
	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ���� �弭 ������ ������ ���
// 
// [ �̴Ͻ��� ]   
//
// ##############################################################################################################
INT CStaticsInfoMgr::RunSeStatics(CString sStaticsClass, CProgressCtrl *pProgressCtrl, CProgressCtrl *pTotalProgressCtrl)
{
EFS_BEGIN

	// 1. ���� ��踦 �����´�.
	// 2. �Ǽ� ��踦 �����´�. ���� ��迡 ���� ADD�Ѵ�.	
	// 4. å�� ��踦 �������鼭 ���Ǽ� ��迡 ���� ADD�Ѵ�.
	// 5. ���� �հ踦 �����´�. 
	// 6. �Ǽ� �հ踦 �����´�. 
	// 7. å�� �հ踦 �����´�.
	// 8. ���� �հ迡 �Ǽ� �հ踦 ADD�Ѵ�.
	// 9. ���Ǽ� �հ迡 å�� �հ踦 ADD�Ѵ�.
	// ��~~

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;	
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;
	CString sResultFieldName;
	CStringArray saResultFieldName;
	CString sWhere;

	// =========================================================================================================
	// 1. ��踦 ���� ���� �⺻���� ��������
	// =========================================================================================================
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::RunBoStatics()") );

	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultFieldName );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::RunBoStatics()") );

	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );
	
	CString sXData,sYData,sTmp;
	pTotalProgressCtrl->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()*m_pRealResultDM->GetRowCount()*2) );
	INT nProgressCnt = 0;

	// =========================================================================================================
	// 2. �� , �� , å�� ��踦 �����.//2007.04.13 pjw����:�ڷ�Ǻ�,�űԱ��Ե��� �̿����
	// =========================================================================================================
	if(sStaticsClass == _T("�ڷ�Ǻ��̿����") || sStaticsClass == _T("�űԱ��Ե����̿����"))
	{
		for( INT i = 0;i < 2;i++ )
		{
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);	
			
			pTotalProgressCtrl->SetRange32( 0 , 1 );

			ids = GetSeWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStatics()") );

			sWhere += _T(" Group By ") + sXFieldName + _T(" , ") + sYFieldName;

			ids = FillStaticsValue3(sStaticsClass, sXFieldName,sYFieldName,saResultFieldName.GetAt(i),_T(""),_T(""),_T("Y"),_T("Y"),sWhere,i, pProgressCtrl);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStatics()") );

			pTotalProgressCtrl->SetPos(1);
		}
	}else
	{
		for( INT i = 0;i < 3;i++ )
		{
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);	
			
			pTotalProgressCtrl->SetRange32( 0 , 1 );

			ids = GetSeWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStatics()") );

			sWhere += _T(" Group By ") + sXFieldName + _T(" , ") + sYFieldName;

			ids = FillStaticsValue3(sStaticsClass, sXFieldName,sYFieldName,saResultFieldName.GetAt(i),_T(""),_T(""),_T("Y"),_T("Y"),sWhere,i, pProgressCtrl);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStatics()") );

			pTotalProgressCtrl->SetPos(1);
		}
	}
	/*for( INT i = 0;i < 3;i++ )
	{
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);	
		
		pTotalProgressCtrl->SetRange32( 0 , 1 );
		ids = GetSeWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStatics()") );

		sWhere += _T(" Group By ") + sXFieldName + _T(" , ") + sYFieldName;
		ids = FillStaticsValue3( sXFieldName,sYFieldName,saResultFieldName.GetAt(i),_T(""),_T(""),_T("Y"),_T("Y"),sWhere,i, pProgressCtrl);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStatics()") );

		pTotalProgressCtrl->SetPos(1);
	}*/
	// =========================================================================================================
	// 3. �հ� ����
	// =========================================================================================================
	ids = MakeTotalCount4( sStaticsClass , saTableName , sXFieldName,sYFieldName , pProgressCtrl , pTotalProgressCtrl);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::RunBoStatics()") );
	
	
	// =========================================================================================================
	// 4. �ڵ尪�� �������� �ٲ۴�.
	// =========================================================================================================
	ids = CodeToDesc(sStaticsClass);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::RunBoStatics()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ���� ������ ��� WHERE�� ������
//
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::GetSeWhere(CString sStaticsClass, CString sTableName, CString sXFieldName, CString sYFieldName, CString sXData, CString sYData, CString sXGroupYN, CString sYGroupYN, INT nMode, CString &sWhere)
{
EFS_BEGIN

	INT ids;

	CStringArray saWhere;
	CString sMenuName = GetMenuName();
	CString sInWhere;	// ������
	CString sOutWhere;	// ������

	// ==========================================================================================================
	// 1. �⺻ ���� ��������
	// ==========================================================================================================
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�⺻����") , sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetBoWhere()") );

	CLocCommonAPI::MakeArrayList( sWhere , _T("/") , saWhere );

	if( saWhere.GetSize() < nMode+1 )
		nMode = 0;
	
	sWhere = saWhere.GetAt(nMode);
	
	// ==========================================================================================================
	// 2. �˻����ǿ��� ������ ���� ������
	// ==========================================================================================================
	if ( sMenuName.Compare(BOOK_STATICS) != 0 )
	{
		ids = GetSeSearchWhere( sStaticsClass , sTableName , sInWhere , sOutWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoWhere()") );
		if ( ids > 0 ) return ids;
	}

	// ==========================================================================================================
	// 3. Group�� ���ؼ� ������ ���� �����ϱ�
	// ==========================================================================================================
	CStringArray asTableName;
	CLocCommonAPI::MakeArrayList( sTableName , _T(",") , asTableName );

	CString sTmpWhere;
	if( !sXFieldName.IsEmpty() && (sXGroupYN.Compare(_T("N"))==0) )
	{
		sXData.Replace( _T("'"), _T("''") );
		
		if( !sXData.IsEmpty() )
			sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName,sXData );
		else
			sTmpWhere.Format( _T(" AND ( %s='%s' OR %s IS NULL ) "),sXFieldName,sXData,sXFieldName);

		if( IsFieldFromTable( sXFieldName , asTableName ) == 1 )
			sOutWhere += sTmpWhere;
		else
			sInWhere += sTmpWhere;
	}

	if( !sYFieldName.IsEmpty() && (sYGroupYN.Compare(_T("N"))==0) )
	{
		sYData.Replace( _T("'"), _T("''") );
		
		if( !sYData.IsEmpty() )
			sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName,sYData );
		else
			sTmpWhere.Format( _T(" AND ( %s='%s' OR %s IS NULL ) "),sYFieldName,sYData,sYFieldName);
		if( IsFieldFromTable( sYFieldName , asTableName ) == 1 )
			sOutWhere += sTmpWhere;
		else
			sInWhere += sTmpWhere;
	}

	// ��ũ�� ����
	sWhere.Replace( _T("%������%") , sInWhere );
	sWhere.Replace( _T("%������%") , sOutWhere );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ���������� �������� �����ϱ�
//
// [ �̴Ͻ��� ]
// 
// ##############################################################################################################
INT CStaticsInfoMgr::GetSeSearchWhere(CString sStaticsClass, CString sTableName, CString &sInWhere, CString &sOutWhere)
{
EFS_BEGIN

	INT ids;

	CString sMenuName = GetMenuName();

	// ==========================================================================================================
	// 1. ��Ʈ�� �˸��ƽ��� �� DM ����
	// ==========================================================================================================
	ids = MakeUserSearchList( sStaticsClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	// ==========================================================================================================
	// 2. �޴� �� ���� �޶�����.
	// ==========================================================================================================
	CString sPrintSearchWhere;

    if( sMenuName.Compare(SE_HOLDING_STATICS) == 0 )
		ids = GetSearchWhere1113(sStaticsClass, sTableName , sInWhere , sOutWhere , sPrintSearchWhere );

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	// ==========================================================================================================
	// 3. ��¿� �˻�����
	// ==========================================================================================================
	ids = SetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("����ڰ˻�����_���") , sPrintSearchWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ����_��������� ��� ����
//   �� ��� �� ��� å ����� ���� ǥ���� �ش�.
//
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
//CString sStaticsClass �߰� :: pjw
INT CStaticsInfoMgr::FillStaticsValue3(CString sStaticsClass,CString sXFieldName, CString sYFieldName, CString sResultFieldName, CString sXData, CString sYData, CString sXGroupYN, CString sYGroupYN, CString sWhere, INT nMode, CProgressCtrl *pProgressCtrl)
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. SELECT �ʵ� ����
	// ==========================================================================================================
	ids = SetDataMgrRef( m_pResultDM , sXFieldName ,_T("") , sXGroupYN ,  0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("") , sYGroupYN , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunStatics") );
	ids = SetDataMgrRef( m_pResultDM , sResultFieldName , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("RunStatics") );

	// ==========================================================================================================
	// 2. WHERE ���� ����
	// ==========================================================================================================
	CString sSendWhere = sWhere;

	// ==========================================================================================================
	// 3. RESULT DM -> REAL RESULT DM ���� �̵�
	// ==========================================================================================================
	ids = m_pResultDM->RefreshDataManager( sSendWhere , pProgressCtrl );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue2") );

	INT nXIndex , nYIndex;
	CString sXAlias , sYAlias , sResult , sPreResult;



	for( INT i = 0 ; i < m_pResultDM->GetRowCount() ; i++ )
	{
		ids = m_pResultDM->GetCellData( _T("�����") , i , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsValue2") );

		ids = m_pResultDM->GetCellData( _T("X��˸��ƽ�") , i , sXAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsValue2") );

		ids = m_pResultDM->GetCellData( _T("Y��˸��ƽ�") , i , sYAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsValue2") );
		
		// �ε��� ã��
		ids = GetDMIndex( m_pRealResultDM , sXAlias,sYAlias,nXIndex,nYIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("FillStaticsValue2") );

		// �� �Է�		
		ids = m_pRealResultDM->GetCellData( sXAlias , nYIndex , sPreResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("FillStaticsValue2") );
				
		// ��, ��, å�� ���� �Ѱ��� ǥ�� ����!!//pjw�����ؾ��Һκ� �� �հ� �հ�κ� ����	
	
		if(sStaticsClass == _T("�ڷ�Ǻ��̿����") || sStaticsClass == _T("�űԱ��Ե����̿����"))
		{
			if( nMode == 0 )
			{
				sResult.Format( _T("%d") , _ttoi(sPreResult) + _ttoi(sResult) );
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("FillStaticsValue2") );			
			}
			else if( nMode == 1 )
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sPreResult) , _ttoi(sResult) );							
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -102 , _T("FillStaticsValue2") );
			}
		}else
		{
			if( nMode == 0 )
			{
				sResult.Format( _T("%d") , _ttoi(sPreResult) + _ttoi(sResult) );
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("FillStaticsValue2") );			
			}
			else if( nMode == 1 )
			{
				sResult.Format( _T("%d\r\n[%d]") , _ttoi(sPreResult) , _ttoi(sResult) );							
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -102 , _T("FillStaticsValue2") );
			}
			else if( nMode == 2 )
			{
				sResult.Format( _T("%s\r\n(%d)") , sPreResult , _ttoi(sResult) );							
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -102 , _T("FillStaticsValue2") );
			}
		}
	}

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ����_��������� �հ�
//
// [ �̴Ͻ��� ]
//
// ##############################################################################################################

INT CStaticsInfoMgr::MakeTotalCount4( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName, CString sYFieldName, CProgressCtrl *pProgressCtrl, CProgressCtrl *pTotalProgress)
{
EFS_BEGIN

	INT ids;

	m_pYSumDM->FreeData();
	m_pXSumDM->FreeData();
	
	/// �ߺ� ��� �հ� ���pjw����-New_XYSum4_duplication(_allow,_delete�߰�)

		
	if(sStaticsClass == _T("�ڷ�Ǻ��̿����") || sStaticsClass == _T("�űԱ��Ե����̿����"))
	{
		ids = New_XYSum4_duplication_allow();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeTotalCount4()") );

		/// �ߺ� ���� �հ� ���
		ids = New_XYSum4_duplication_delete( sStaticsClass , saTableName , sXFieldName , sYFieldName , pTotalProgress );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeTotalCount4()") );
	}else
	{
		ids = XYSum4_duplication_allow();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeTotalCount4()") );

		/// �ߺ� ���� �հ� ���
		ids = XYSum4_duplication_delete( sStaticsClass , saTableName , sXFieldName , sYFieldName , pTotalProgress );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeTotalCount4()") );
	}
/*	ids = XYSum4_duplication_allow();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeTotalCount4()") );

	/// �ߺ� ���� �հ� ���
	ids = XYSum4_duplication_delete( sStaticsClass , saTableName , sXFieldName , sYFieldName , pTotalProgress );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeTotalCount4()") );
*/

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ����_��������� ���� �и��ϴ� �Լ� 
//
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeSpeciesVolBookCnt(CString sDuplicateAllowSum, CString& sSpeciesCnt, CString& sVolCnt, CString& sBookCnt)
{
EFS_BEGIN

	sSpeciesCnt.Empty();sVolCnt.Empty();sBookCnt.Empty();

	// �Ǽ� ǥ�ð� �и� �÷����� ����� �ȴ�.
	INT nVolSpos, nVolEpos;
	nVolSpos = sDuplicateAllowSum.Find( _T("[") );
	nVolEpos = sDuplicateAllowSum.Find( _T("]") );

	if ( nVolSpos > -1 && nVolEpos > -1 )
	{
		/// ���� ���ϱ� 
		sSpeciesCnt.Format( _T("%s"), sDuplicateAllowSum.Left( nVolSpos ) );
		sSpeciesCnt.Replace( _T("\r\n") , _T("") );
		
		/// �Ǽ� 
		sVolCnt.Format( _T("%s"), sDuplicateAllowSum.Mid( nVolSpos + 1, nVolEpos - (nVolSpos + 1) ) );
		
		/// å��
		if ( sDuplicateAllowSum.GetLength() >=  nVolEpos + 1 )
		{
			sBookCnt.Format( _T("%s"), sDuplicateAllowSum.Mid( nVolEpos + 1 ) );
			sBookCnt.Replace( _T("\r\n") , _T("") );
			sBookCnt.Replace( _T("(") , _T("") );
			sBookCnt.Replace( _T(")") , _T("") );
		}
	}
	else
	{
		if ( sDuplicateAllowSum.GetLength() )
			sSpeciesCnt.Format( _T("%s") , sDuplicateAllowSum );
	}

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ����_��������� �հ� �ߺ����� �հ�
//   ������ �����´�.
//   å���� ���ٰ� ����.
//   �ߺ����� �հ� ���Ŀ� ����Ǿ�� �Ѵ�.
//
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::XYSum4_duplication_delete(CString sStaticsClass, CStringArray &saTableName, CString sXFieldName, CString sYFieldName, CProgressCtrl *pTotalProgress)
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. ��踦 �� DM�� ���̺� �̸� �߰�	
	// ==========================================================================================================
	CString sResultFieldName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultFieldName );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

	CStringArray saResultFieldName;
	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );
	
	INT i = 0;	
	
	m_pResultDM->TBL_NAME = saTableName.GetAt(i);
	
	CStringArray saTableAlias;
	
	ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );	

	CString strCountField;
	CString sYSumAlias , sDuplicateAllowSum;
	CString sTmpResult , sResult;
	CString sYData , sTmp;
	CString sSpeciesCnt , sVolCnt, sBookCnt;
	INT nIndex;
	CString sWhere;
	/// ���� �հ� ��� //////////////////////////////////////////////////////////////////////////////////////////		
	// ������ �ǿ� ���� �հ� ������ �ѹ� �� �����ؾ��Ѵ�.	
	for ( INT ii = 0;ii < 2;ii++ )
	{
		// ==========================================================================================================
		// 2. ��,�� ���� �հ� SELECT �ʵ� ����
		// ==========================================================================================================
	   
		if ( !ii ) strCountField = _T("COUNT( DISTINCT S.REC_KEY )");
		else       strCountField = _T("COUNT( DISTINCT V.REC_KEY )"); 
		
		ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
		ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("Y") , 1 );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
		ids = SetDataMgrRef( m_pResultDM , strCountField , _T("0") , _T("Y") , 2 );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

		// ==========================================================================================================
		// 3. ��,�� ���� �հ� WHERE�� ����	
		// ==========================================================================================================
		ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , _T("") , _T("N") , _T("Y") ,i , sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

		sWhere += _T(" GROUP BY ") + sYFieldName;
				
		ids = m_pResultDM->RefreshDataManager( sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

		for ( INT k = 0;k < m_pResultDM->GetRowCount();k++ )
		{
			ids = m_pResultDM->GetCellData( _T("Y��˸��ƽ�") , k , sYSumAlias );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
			
			nIndex = -1;
			for ( INT j = 0;j < m_pRealResultDM->GetRowCount();j++ )
			{
				ids = m_pRealResultDM->GetCellData( j , 0 , sYData );
				if ( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
				
				if ( sYData.Compare( sYSumAlias ) == 0 ) nIndex = j;
			}
			
			if ( nIndex == -1 ) continue;
			
			ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
			
			ids = m_pXSumDM->GetCellData( _T("�ߺ����_�հ�") , nIndex , sDuplicateAllowSum );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
			
			MakeSpeciesVolBookCnt( sDuplicateAllowSum , sSpeciesCnt , sVolCnt , sBookCnt );
			
			if( _ttoi(sResult) == 0 )
				sResult = _T("0");
			else
			{
				if ( !ii )
						sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sResult) , _ttoi(sVolCnt) , _ttoi(sBookCnt) );
					else
						sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sSpeciesCnt) , _ttoi(sResult) , _ttoi(sBookCnt) );
						
			}
			
			ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , nIndex  );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );			
		}	
	}

	/// ���� �հ� ��� //////////////////////////////////////////////////////////////////////////////////////////		
	// ������ �ǿ� ���� �հ� ������ �ѹ� �� �����ؾ��Ѵ�.	
	sWhere.Empty();
	for ( ii = 0;ii < 2;ii++ )
	{
		// ==========================================================================================================
		// 4. ��,�� ���� �հ� SELECT �ʵ� ����  
		// ==========================================================================================================
			 if ( !ii ) strCountField = _T("COUNT( DISTINCT S.REC_KEY )");
			 else       strCountField = _T("COUNT( DISTINCT V.REC_KEY )"); 
	
		ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("Y") , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 ,_T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
		ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
		ids = SetDataMgrRef( m_pResultDM , strCountField , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
		
		// ==========================================================================================================
		// 5. ��,�� ���� �հ� WHERE�� ����	
		// ==========================================================================================================
		ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , _T("") , _T("") ,_T("Y") , _T("N") , i , sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

		sWhere += _T(" GROUP BY ") + sXFieldName;
		
		ids = m_pResultDM->RefreshDataManager( sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

		CString sXSumAlias;
		sDuplicateAllowSum.Empty();
		sTmpResult.Empty() , sResult.Empty();
		CString sXData;
		sTmp.Empty();
		sSpeciesCnt.Empty() , sBookCnt.Empty();
		for( INT k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
		{
			ids = m_pResultDM->GetCellData( _T("X��˸��ƽ�") , k , sXSumAlias );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
			
			nIndex = -1;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

				if( sXData.Compare( sXSumAlias ) == 0 ) nIndex = j;
			}

			if( nIndex == -1 ) continue;

			ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

			ids = m_pYSumDM->GetCellData( _T("�ߺ����_�հ�") , nIndex-1 , sDuplicateAllowSum );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
			
			MakeSpeciesVolBookCnt( sDuplicateAllowSum , sSpeciesCnt , sVolCnt , sBookCnt );
			if( _ttoi(sResult) == 0 )
				sResult = _T("0");
			else
			{
				if ( !ii )
					sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sResult) , _ttoi(sVolCnt) , _ttoi(sBookCnt) );
				else
					sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sSpeciesCnt) , _ttoi(sResult) , _ttoi(sBookCnt) );
			}

			ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , nIndex-1  );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					
		}
	}
	
		/// ���հ� //////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ �ǿ� ���� �հ������� �ѹ��� �����ؾ� �Ѵ�.
	for ( ii = 0;ii < 2;ii++ )
	{
		// ==========================================================================================================
		// 6. ���հ� SELECT �ʵ� ����  
		// ==========================================================================================================
		ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					
		ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );	
		ids = SetDataMgrRef( m_pResultDM , saResultFieldName.GetAt(ii) , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					

		// ==========================================================================================================
		// 7. ���հ� WHERE�� ����
		// ==========================================================================================================
		sWhere.Empty();
		ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
		
		ids = m_pResultDM->RefreshDataManager( sWhere);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -26 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					
				
		ids = m_pResultDM->GetCellData( _T("�����") , 0 , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -27 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					

		ids = m_pXSumDM->GetCellData( _T("�ߺ����_�հ�") , m_pXSumDM->GetRowCount()-1 , sDuplicateAllowSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -28 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					
		
		MakeSpeciesVolBookCnt( sDuplicateAllowSum , sSpeciesCnt , sVolCnt, sBookCnt );

		if( _ttoi(sResult) == 0 )//�׸��忡 �Ѱ� ���� �� ��Ÿ���� �κ�
			sResult = _T("0");
		else
		{
			if ( !ii )
				sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sResult) , _ttoi(sVolCnt) , _ttoi(sBookCnt) );
			else
				sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sSpeciesCnt) , _ttoi(sResult) , _ttoi(sBookCnt) );
		}
		
		ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , m_pXSumDM->GetRowCount()-1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -29 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					
	}	
	
	// ==========================================================================================================
	// 8. ��� �հ��� ����ִ� ������ 0���� �����
	// ==========================================================================================================
	for( INT m = 0 ; m < m_pXSumDM->GetRowCount() ; m++ )
	{
		ids = m_pXSumDM->GetCellData( _T("�ߺ�����_�հ�") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -30 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;

		ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}

	for( m = 0 ; m < m_pYSumDM->GetRowCount() ; m++ )
	{
		ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -32 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;
		
		ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -33 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}
	
	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]PJW�ۼ�-���� �űԱ��Ե����̿����,�ڷ�Ǻ��̿���� 
//   ����_��������� �հ� �ߺ����� �հ�
//   ������ �����´�.
//   å���� ���ٰ� ����.
//   �ߺ����� �հ� ���Ŀ� ����Ǿ�� �Ѵ�.
//
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::New_XYSum4_duplication_delete(CString sStaticsClass, CStringArray &saTableName, CString sXFieldName, CString sYFieldName, CProgressCtrl *pTotalProgress)
{
EFS_BEGIN

	INT ids;

	CStringArray saResultFieldName;
	CString sResultFieldName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	
	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );
	
	INT i = 0;
	
	// DM ���̺� �̸� �߰�
	m_pResultDM->TBL_NAME = saTableName.GetAt(i);
	
	CStringArray saTableAlias;	
	ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );	

	/// ������ �Ѳ����� �����´�.
	// ==========================================================================================================
	/// ���� �հ� ���
	// ==========================================================================================================	
	// SELECT �ʵ� ����
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("Y") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

	// pjw
//	ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT S.REC_KEY )") , _T("0") , _T("Y") , 2 );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	if( _T("�ڷ�Ǻ��̿����") == sStaticsClass ||   _T("�űԱ��Ե����̿����")== sStaticsClass)
	{
		ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT CL.BOOK_KEY )") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}
	else
	{
		ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT S.REC_KEY )") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}
	
	// 3.3. WHERE�� ���� 
	CString sWhere;
		
	ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , _T("") , _T("N") , _T("Y") ,i , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

	sWhere += _T(" GROUP BY ") + sYFieldName;
		
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

	
	CString sYSumAlias , sDuplicateAllowSum;
	CString sTmpResult , sResult;
	CString sYData , sTmp;
	CString sSpeciesCnt , sBookCnt;
	INT nIndex;
	for( INT k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
	{
		ids = m_pResultDM->GetCellData( _T("Y��˸��ƽ�") , k , sYSumAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		nIndex = -1;
		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( j , 0 , sYData );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
			
			if( sYData.Compare( sYSumAlias ) == 0 ) nIndex = j;
		}
		
		if( nIndex == -1 ) continue;
		
		ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		ids = m_pXSumDM->GetCellData( _T("�ߺ����_�հ�") , nIndex , sDuplicateAllowSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
		
		//pjw����
		if( _T("�ڷ�Ǻ��̿����") == sStaticsClass ||   _T("�űԱ��Ե����̿����")== sStaticsClass)
		{
			if( _ttoi(sBookCnt) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
			}
			
		}else
		{
			if( _ttoi(sResult) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
			}
		}
		/*if( _ttoi(sResult) == 0 )
			sResult = _T("0");
		else
		{
			sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
		}*/
		
		ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , nIndex  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
	}

	// ==========================================================================================================
	// ���� �հ� ���
	// ==========================================================================================================
	// 3.3. WHERE�� ���� 
	sWhere.Empty();
	
	// SELECT �ʵ� ����
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("Y") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	// pjw
//	ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT S.REC_KEY)") , _T("0") , _T("Y") , 2 );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	if( _T("�ڷ�Ǻ��̿����") == sStaticsClass ||_T("�űԱ��Ե����̿����") == sStaticsClass  )
	{
		ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT CL.BOOK_KEY)") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}
	else
	{
		ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT S.REC_KEY)") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}	
	
	
	// WHERE�� ����
	ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , _T("") , _T("") ,_T("Y") , _T("N") , i , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

	sWhere += _T(" GROUP BY ") + sXFieldName;
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

	CString sXSumAlias;
	sDuplicateAllowSum.Empty();
	sTmpResult.Empty() , sResult.Empty();
	CString sXData;
	sTmp.Empty();
	sSpeciesCnt.Empty() , sBookCnt.Empty();
	
	for( k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
	{
		
		ids = m_pResultDM->GetCellData( _T("X��˸��ƽ�") , k , sXSumAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		nIndex = -1;
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

			if( sXData.Compare( sXSumAlias ) == 0 ) nIndex = j;

			//##################################################################################################
			if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )		nIndex = 4;
			//##################################################################################################
		}

		if( nIndex == -1 ) continue;

		ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

		ids = m_pYSumDM->GetCellData( _T("�ߺ����_�հ�") , nIndex-1 , sDuplicateAllowSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ���� ����з���ȣ ����ó��
		if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
		{
			// sDuplicateAllowSum ���� �и� �۾�
			CString sSpecies, sBook;
			DividArmyCell( sDuplicateAllowSum, sSpecies, sBook );			
			
			sSpecies = sSpecies.Mid( sSpecies.Find( DIVCHAR ) + 1 );
			sBook = sBook.Mid( sBook.Find( DIVCHAR ) + 1 );
			sDuplicateAllowSum.Format( _T("%d\r\n(%d)"), _ttoi( sSpecies ), _ttoi( sBook ) );
		}
		// ------------------------------------------------------------------------------------------------------

		MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
	
		//pjw����
		if( _T("�ڷ�Ǻ��̿����") == sStaticsClass ||   _T("�űԱ��Ե����̿����")== sStaticsClass)
		{
			if( _ttoi(sBookCnt) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
				// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ���� ����з���ȣ ����ó��
			if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
			{
				CString sTemp;
				ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , nIndex-1, sTemp );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				CString sForSpeci, sForBook;
				MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

				ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
			}
			// --------------------------------------------------------------------------------------------------
		
			}
			
		}else
		{
			if( _ttoi(sResult) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
				// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// ���� ����з���ȣ ����ó��
				if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
				{
					CString sTemp;
					ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , nIndex-1, sTemp );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );	

					CString sForSpeci, sForBook;
					MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

					ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

					sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
				}
			// --------------------------------------------------------------------------------------------------
			}
		}
	/*	if( _ttoi(sResult) == 0 )
			sResult = _T("0");
		else
		{
			sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );

			// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ���� ����з���ȣ ����ó��
			if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
			{
				CString sTemp;
				ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , nIndex-1, sTemp );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				CString sForSpeci, sForBook;
				MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

				ids = m_pResultDM->GetCellData( _T("�����") , k , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
			}
			// --------------------------------------------------------------------------------------------------
		}*/

		ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , nIndex-1  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );				
	}
	
	// ==========================================================================================================
	// ���հ�
	// ==========================================================================================================
	// SELECT �ʵ� ����

	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , saResultFieldName.GetAt(i) , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	// WHERE�� ����
	ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -26 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
			
	ids = m_pResultDM->GetCellData( _T("�����") , 0 , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -27 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	ids = m_pXSumDM->GetCellData( _T("�ߺ����_�հ�") , m_pXSumDM->GetRowCount()-1 , sDuplicateAllowSum );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -28 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
	
	if( _ttoi(sResult) == 0 )
		sResult = _T("0");
	else
		sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );	
	
	ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , sResult , m_pXSumDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -29 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	
	// ==========================================================================================================
	// ��� �հ��� ����ִ� ������ 0���� �����
	// ==========================================================================================================
	for( INT m = 0 ; m < m_pXSumDM->GetRowCount() ; m++ )
	{
		ids = m_pXSumDM->GetCellData( _T("�ߺ�����_�հ�") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -30 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;

		ids = m_pXSumDM->SetCellData( _T("�ߺ�����_�հ�") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}


	for( m = 0 ; m < m_pYSumDM->GetRowCount() ; m++ )
	{
		ids = m_pYSumDM->GetCellData( _T("�ߺ�����_�հ�") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -32 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;
		
		ids = m_pYSumDM->SetCellData( _T("�ߺ�����_�հ�") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -33 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}
	
	return 0;
EFS_END
	return -1;
}

// ##############################################################################################################
// [ �Լ����� ]
//   ����_��������� �ߺ����� �հ�
//   ��� ����� �ο캰�� �÷����� ��ģ��.
//
// [ �̴Ͻ��� ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::XYSum4_duplication_allow()
{
EFS_BEGIN

	INT ids;
	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSpeciesSum, sVolSum, sBookSum;
	CString sData;
	CString sSpeciesCnt, sVolCnt, sBookCnt;
	CString sTotalSpeciesSum, sTotalVolSum, sTotalBookSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;

	// ==========================================================================================================
	// 1. ���� �հ�
	// ==========================================================================================================
	for ( INT i = 0;i < m_pRealResultDM->GetRowCount();i++ )
	{
		sSpeciesSum.Empty();sVolSum.Empty();sBookSum.Empty();
		for ( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;
			
			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XYSum4_duplication_allow()") );
		
			MakeSpeciesVolBookCnt( sData , sSpeciesCnt , sVolCnt , sBookCnt );
			
			sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
			sVolSum.Format( _T("%d") , _ttoi(sVolSum)+_ttoi(sVolCnt) );
			sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
		}
		
		sTotalSpeciesSum.Format( _T("%d") , _ttoi(sTotalSpeciesSum) + _ttoi(sSpeciesSum) );
		sTotalVolSum.Format( _T("%d") , _ttoi(sTotalVolSum) + _ttoi(sVolSum) );
		sTotalBookSum.Format( _T("%d") , _ttoi(sTotalBookSum) + _ttoi(sBookSum) );
		
		if ( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);
		
		if ( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sVolSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");
		else
			sData.Format( _T("%s\r\n[%s]\r\n(%s)") , sSpeciesSum , sVolSum , sBookSum );
		
		m_pXSumDM->SetCellData( sSumAlias , sData , i);		
	}
	
	// ==========================================================================================================
	// 2. �ߺ� ��Ż�հ踦 ���Ѵ�.
	// ==========================================================================================================
	if ( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);
	
	if ( (_ttoi(sTotalSpeciesSum) == 0) && (_ttoi(sTotalVolSum) == 0) && (_ttoi(sTotalBookSum) == 0) )
		sData = _T("0");
	else
		sData.Format( _T("%s\r\n[%s]\r\n(%s)") , sTotalSpeciesSum , sTotalVolSum , sTotalBookSum );
	
	m_pXSumDM->SetCellData( sSumAlias , sData , i );
	
	// ==========================================================================================================
	/// 3. ���� �հ�
	// ==========================================================================================================	
	sAlias.Empty();
	sSpeciesSum.Empty();
	sVolSum.Empty();
	sBookSum.Empty();
	sData.Empty();
	sSpeciesCnt.Empty();
	sVolCnt.Empty();
	sBookCnt.Empty();


	for ( i = 1;i < m_pRealResultDM->RefList.GetCount();i++ )
	{
		sSpeciesSum.Empty();sVolSum.Empty();sBookSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;
		
		for ( INT j = 0;j < m_pRealResultDM->GetRowCount();j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XYSum4_duplication_allow()") );
		
			MakeSpeciesVolBookCnt( sData , sSpeciesCnt , sVolCnt , sBookCnt );
			sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
			sVolSum.Format( _T("%d") , _ttoi(sVolSum)+_ttoi(sVolCnt) );
			sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
		}
		
		if ( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);
		
		if ( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sVolSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");
		else
			sData.Format( _T("%s\r\n[%s]\r\n(%s)") , sSpeciesSum , sVolSum , sBookSum );
		
		m_pYSumDM->SetCellData( sSumAlias , sData ,i-1 );
	}	
		
	return 0;

EFS_END
	return -1;
}


// ##############################################################################################################
// [ �Լ����� ]
//   ����_���������(�űԱ����̿����,�ڷ�Ǻ��̿����) �ߺ����� �հ�
//   ��� ����� �ο캰�� �÷����� ��ģ��.
//
// [ �̴Ͻ��� ]pjw�ۼ�
//
// ##############################################################################################################
INT CStaticsInfoMgr::New_XYSum4_duplication_allow()
{
EFS_BEGIN

	INT ids;
	
	// ==========================================================================================================
	// �ߺ� ��� �հ� ����
	CString sSumAlias = _T("�ߺ����_�հ�");
	CString sAlias;
	CString sSpeciesSum,sBookSum;
	CString sData;
	CString sSpeciesCnt,sBookCnt;
	CString sTotalSpeciesSum,sTotalBookSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	// ==========================================================================================================
	
	// ==========================================================================================================
	// 1. ���� �հ� ����
	// ==========================================================================================================
	for( INT i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;
			
			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::New_XYSum4_duplication_allow()") );
			
			if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			{
				MakeArmySpeciesBookCnt( sData, &sData );
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
			
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
			else
			{
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
				
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
		}
		
		sTotalSpeciesSum.Format( _T("%d") , _ttoi(sTotalSpeciesSum) + _ttoi(sSpeciesSum) );
		sTotalBookSum.Format( _T("%d") , _ttoi(sTotalBookSum) + _ttoi(sBookSum) );
		
		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);
		
		if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");		
		else  
			sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );
		
		m_pXSumDM->SetCellData( sSumAlias , sData , i);
		
	}
	
	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);
	
	if( (_ttoi(sTotalSpeciesSum) == 0) && (_ttoi(sTotalBookSum) == 0) )
		sData = _T("0");
	else
		sData.Format( _T("%s\r\n(%s)") , sTotalSpeciesSum , sTotalBookSum );
	
	m_pXSumDM->SetCellData( sSumAlias , sData , i );

	// ==========================================================================================================
	// �ߺ� ��� �հ� ����
	sAlias.Empty();
	sSpeciesSum.Empty();
	sBookSum.Empty();
	sData.Empty();
	sSpeciesCnt.Empty();
	sBookCnt.Empty();
	// ==========================================================================================================
	
	// ==========================================================================================================
	// 2. ���� �հ� ����
	// ==========================================================================================================
	for( i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;
			
		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::New_XYSum4_duplication_allow()") );
						
			// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ������ ����з���ȣ �հ踦 ��Ÿ���� ���� ���� ó��
			if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			{
				// ����� �����ٰ� ���� ����Ѵ�.
				MakeArmySum( sSpeciesSum , sData, sSpeciesSum );				
			}
			else
			{
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
			
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
			// --------------------------------------------------------------------------------------------------
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);
		
		if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			sData.Format( _T("%s"), sSpeciesSum );
		else
		{
			if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
				sData = _T("0");
			else
				sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );			
		}
		
		m_pYSumDM->SetCellData( sSumAlias , sData ,i-1 );		
	}	
		
	return 0;
EFS_END
	return -1;
}

//###############################################################################################################
// [ �Լ����� ]
//   ���� ����� �հ踦 ���Ѵ�.
//   Ex)
//    3,8  +  5,6  -->  8,14
//   (5,9)   (5,6)    (10,15)
//
// [ �̴Ͻ��� ]
//
//###############################################################################################################
INT CStaticsInfoMgr::MakeArmySum(CString sData1, CString sData2, CString &sGetData)
{
EFS_BEGIN

	if ( !sData1.GetLength() )	sData1 = _T("0");
	if ( !sData2.GetLength() )	sData2 = _T("0");

	/************************************************************************************************************
	���� å�� �и� ���� ������ ���·� �����. 

		5,10  ->    5,10     9,26  �ΰ��� �и��Ѵ�.
	   (9,26) ->	
	************************************************************************************************************/
	// ==========================================================================================================
	// 1. sData1�� ���� å���� �и�
	// ==========================================================================================================
	CString sData1SpeCnt, sData1BookCnt;	
	DividArmyCell( sData1, sData1SpeCnt, sData1BookCnt );

	// ==========================================================================================================
	// 2. sData2�� ���� å���� �и�
	// ==========================================================================================================
	CString sData2SpeCnt, sData2BookCnt;
	DividArmyCell( sData2, sData2SpeCnt, sData2BookCnt );

	// ==========================================================================================================
	// 3. �и��� ����� ������ �°� ���� ��ģ��.
	// ==========================================================================================================
	CString sTemp1, sTemp2, sTemp3, sTemp4;
	CString sSpeResult, sBookResult;

	// �� ����
	DividNormalArmy( sData1SpeCnt, sTemp1, sTemp2 );
	DividNormalArmy( sData2SpeCnt, sTemp3, sTemp4 );
	sSpeResult.Format( _T("%d %s %d"), _ttoi( sTemp1 ) + _ttoi( sTemp3 ), DIVCHAR , _ttoi( sTemp2 ) + _ttoi( sTemp4 ) );
	
	// å ����
	DividNormalArmy( sData1BookCnt, sTemp1, sTemp2 );
	DividNormalArmy( sData2BookCnt, sTemp3, sTemp4 );
	sBookResult.Format( _T("(%d %s %d)"), _ttoi( sTemp1 ) + _ttoi( sTemp3 ), DIVCHAR , _ttoi( sTemp2 ) + _ttoi( sTemp4 ) );
	// ==========================================================================================================
	// 4. ������
	// ==========================================================================================================
	sGetData.Format( _T("%s\r\n%s"), sSpeResult, sBookResult );

	return 0;
EFS_END
	return -1;
}

//###############################################################################################################
// [ �Լ����� ]
//   ���� ���缿�� ���� å���� �и��Ѵ�.
//   Ex)
//    3,8  -->  3,8   5,9
//   (5,9) 
//
// [ �̴Ͻ��� ]
//
//###############################################################################################################
INT CStaticsInfoMgr::DividArmyCell(CString sData, CString &sSpecies, CString &sBook)
{
EFS_BEGIN
	
	sData.Replace( _T(" "), _T("") );
	sData.Replace( _T("("), _T("") );sData.Replace( _T(")"), _T("") );

	INT nPos = sData.Find( _T("\r\n") );
	
	// ==========================================================================================================
	// 1. ���๮�ڰ� ���ٸ� ���� �����Ѵٰ� ����.
	// ==========================================================================================================
	if ( nPos == -1 )
	{
		nPos = sData.Find( DIVCHAR );

		if ( nPos == -1 )
		{
			// �̷���찡 �������� ������ ;
			sSpecies = sData + _T(",0");
		}
		else
			sSpecies = sData;
		
		sBook = _T("0,0");		
	}
	// ==========================================================================================================
	// 2. ��å�� �и��Ѵ�.
	// ==========================================================================================================
	else
	{
		sSpecies = sData.Mid( 0, nPos );
		sBook    = sData.Mid( nPos + 2 );		
	}

	return 0;
EFS_END
	return -1;
}


// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//

// 2018.07.11 ADD BY KHY: ����� ����Ʈ ����
/*//NEW------------------------------------------------------------------------*/					
INT CStaticsInfoMgr::RunBoStaticsList( CString sStaticsClass, CString sXValue, CString sYValue )
{
EFS_BEGIN

	// 1. ���� ��踦 �����´�.
	// 2. å�� ��踦 �������鼭 ���� ��迡 ���� ADD�Ѵ�.
	// 3. ���� �հ踦 �����´�. 
	// 4. å�� �հ踦 �����´�.
	// 5. ���� �հ迡 å�� �հ踦 ADD�Ѵ�.
	// ��~~

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;	
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;
	CString sWhere;

	// =========================================================================================================
	// 1. ��踦 ���� ���� �⺻���� ��������
	// =========================================================================================================	
	//ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);

	//csj
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN,true);

	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::RunBoStaticsList()") );
	

	CString sTmpWhere;
	// =========================================================================================================
	// 2. �� , å�� ��踦 �����.
	// =========================================================================================================
	for ( INT i = 0;i < saTableName.GetSize();i++ )
	{
		m_pResultListDM->TBL_NAME = saTableName.GetAt(i);

		ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::RunBoStaticsList()") );

		ids = GetFieldName( sStaticsClass , sXAlias , sXFieldName , sXGroupYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStaticsList()") );
		ids = GetFieldName( sStaticsClass , sYAlias , sYFieldName , sYGroupYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStaticsList()") );

		if(!sXValue.IsEmpty())
		{
			if(sXValue != _T("NULL"))
			{
				sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName, sXValue);
			}
			else 
			{
				sTmpWhere.Format( _T(" AND %s IS NULL"),sXFieldName);
			}
			sWhere += sTmpWhere;
		}
		if(!sYValue.IsEmpty())
		{
			if(sYValue != _T("NULL"))
			{
				sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName, sYValue);
			}
			else 
			{
				sTmpWhere.Format( _T(" AND %s IS NULL"),sYFieldName);
			}
			sWhere += sTmpWhere;
		}

		ids = FillStaticsListValue2( sStaticsClass, sYFieldName, sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::RunBoStaticsList()") );
	}

	return 0;

EFS_END
	return -1;
}

INT CStaticsInfoMgr::FillStaticsListValue2(CString sStaticsClass, CString sYFieldName, CString sWhere)
{
	EFS_BEGIN

	INT ids;
	// SELECT �ʵ� ����
	// hykim
	/*
	ids = SetDataMgrRef( m_pResultListDM , _T("DISTINCT B.REG_NO") , _T(""), _T("Y"),  0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsListValue2") );
	ids = SetDataMgrRef( m_pResultListDM , _T("(SELECT TITLE_INFO FROM IDX_BO_TBL WHERE REC_KEY = B.SPECIES_KEY)") , _T("") , _T("Y"), 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsListValue2") );
	*/
	CString strListField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("��¹�") , strListField );
	if ( ids < 0  || strListField.IsEmpty() ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsListValue2()") );

	CStringArray saListField;
	ids = CLocCommonAPI::MakeArrayList( strListField , _T("/") , saListField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsListValue2()") );

	ids = SetDataMgrRefList( m_pResultListDM , _T("DISTINCT B.REC_KEY") , _T(""), _T("Y"), 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsListValue2") );

	for(INT i = 0; i < saListField.GetSize() ; i++)
	{
		ids = SetDataMgrRefList( m_pResultListDM , saListField.GetAt(i) , _T(""), _T("Y"), i+1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("FillStaticsListValue2") );
	}

	// WHERE ���� ����
	CString sSendWhere = sWhere;
	
	//===================================================
	//2009.10.05 ADD BY PJW : ���ں� ������ ���� ���ڷ� �����Ѵ�.
	if( sYFieldName == _T("TO_CHAR(B.REG_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.REG_DATE,'YYYY/MM/DD')");
	}
	else if( sYFieldName == _T("TO_CHAR(B.SHELF_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.SHELF_DATE,'YYYY/MM/DD')");
	}
	else if( sYFieldName == _T("TO_CHAR(B.CHECKIN_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.CHECKIN_DATE,'YYYY/MM/DD')");
	}
	//===================================================

	// RESULT DM -> REAL RESULT DM ���� �̵�
	ids = m_pResultListDM->RefreshDataManager( sSendWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("FillStaticsListValue2") );

	return 0;

	EFS_END
	return -1;
}


INT CStaticsInfoMgr::RunSeStaticsList(CString sStaticsClass, CString sXValue, CString sYValue)
{
EFS_BEGIN

	// 1. ���� ��踦 �����´�.
	// 2. �Ǽ� ��踦 �����´�. ���� ��迡 ���� ADD�Ѵ�.	
	// 4. å�� ��踦 �������鼭 ���Ǽ� ��迡 ���� ADD�Ѵ�.
	// 5. ���� �հ踦 �����´�. 
	// 6. �Ǽ� �հ踦 �����´�. 
	// 7. å�� �հ踦 �����´�.
	// 8. ���� �հ迡 �Ǽ� �հ踦 ADD�Ѵ�.
	// 9. ���Ǽ� �հ迡 å�� �հ踦 ADD�Ѵ�.
	// ��~~

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;	
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;	
	CString sWhere;

	// =========================================================================================================
	// 1. ��踦 ���� ���� �⺻���� ��������
	// =========================================================================================================
	//ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);

	//csj
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN , true);

	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::RunSeStaticsList()") );

	CString sXData,sYData,sTmp;
	INT nProgressCnt = 0;
	CString sTmpWhere;

	// =========================================================================================================
	// 2. �� , �� , å�� ��踦 �����.
	// =========================================================================================================
	if(sStaticsClass == _T("�ڷ�Ǻ��̿����") || sStaticsClass == _T("�űԱ��Ե����̿����"))
	{
		for( INT i = 0; i < 2; i++ )
		{
			m_pResultListDM->TBL_NAME = saTableName.GetAt(i);	
			
			ids = GetSeWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::RunSeStaticsList()") );

			ids = GetFieldName( sStaticsClass , sXAlias , sXFieldName , sXGroupYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStaticsList()") );
			ids = GetFieldName( sStaticsClass , sYAlias , sYFieldName , sYGroupYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStaticsList()") );

			if(sXValue != "")
			{
				if(sXValue != _T("NULL"))
				{
					sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName, sXValue);
				}
				else 
				{
					sTmpWhere.Format( _T(" AND %s IS NULL"),sXFieldName);
				}
				sWhere += sTmpWhere;
			}
			if(sYValue != "")
			{
				if(sYValue != _T("NULL"))
				{
					sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName, sYValue);
				}
				else 
				{
					sTmpWhere.Format( _T(" AND %s IS NULL"),sYFieldName);
				}
				sWhere += sTmpWhere;
			}
				
			ids = FillStaticsListValue3(sStaticsClass, sWhere);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunSeStaticsList()") );
		}
	}else
	{
		for( INT i = 0; i < 3; i++ )
		{
			m_pResultListDM->TBL_NAME = saTableName.GetAt(i);	
			
			ids = GetSeWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::RunSeStaticsList()") );

			ids = GetFieldName( sStaticsClass , sXAlias , sXFieldName , sXGroupYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStaticsList()") );
			ids = GetFieldName( sStaticsClass , sYAlias , sYFieldName , sYGroupYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStaticsList()") );

			if(sXValue != "")
			{
				if(sXValue != _T("NULL"))
				{
					sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName, sXValue);
				}
				else 
				{
					sTmpWhere.Format( _T(" AND %s IS NULL"),sXFieldName);
				}
				sWhere += sTmpWhere;
			}
			if(sYValue != "")
			{
				if(sYValue != _T("NULL"))
				{
					sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName, sYValue);
				}
				else 
				{
					sTmpWhere.Format( _T(" AND %s IS NULL"),sYFieldName);
				}
				sWhere += sTmpWhere;
			}

			ids = FillStaticsListValue3(sStaticsClass, sWhere);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::RunSeStaticsList()") );
		}
	}

	return 0;

EFS_END
	return -1;
}

INT CStaticsInfoMgr::FillStaticsListValue3(CString sStaticsClass, CString sWhere)
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. SELECT �ʵ� ����
	// ==========================================================================================================
	/*
	ids = SetDataMgrRef( m_pResultListDM , _T("DISTINCT B.REG_NO") , _T(""), _T("Y"),  0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsListValue3") );
	ids = SetDataMgrRef( m_pResultListDM , _T("(SELECT TITLE_INFO FROM IDX_SE_TBL WHERE REC_KEY = B.SPECIES_KEY)") , _T("") , _T("Y"), 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsListValue3") );
	*/
	CString strListField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("��¹�") , strListField );
	if ( ids < 0 || strListField.IsEmpty() ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsListValue3()") );

	CStringArray saListField;
	ids = CLocCommonAPI::MakeArrayList( strListField , _T("/") , saListField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsListValue3()") );

	ids = SetDataMgrRefList( m_pResultListDM , _T("DISTINCT B.REC_KEY") , _T(""), _T("Y"), 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsListValue3") );

	for(INT i = 0; i < saListField.GetSize(); i++)
	{
		ids = SetDataMgrRefList( m_pResultListDM , saListField.GetAt(i) , _T(""), _T("Y"), i+1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("FillStaticsListValue3") );
	}	

	// ==========================================================================================================
	// 2. WHERE ���� ����
	// ==========================================================================================================
	CString sSendWhere = sWhere;

	// ==========================================================================================================
	// 3. RESULT DM -> REAL RESULT DM ���� �̵�
	// ==========================================================================================================
	ids = m_pResultListDM->RefreshDataManager( sSendWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("FillStaticsListValue3") );
	return 0;

EFS_END
	return -1;
}

INT CStaticsInfoMgr::RunLoanStaticsList( CString sStaticsClass , CString sXValue, CString sYValue, bool bDupDel )
{
	EFS_BEGIN

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;
	CString sTableName;
	
	INT nPos = 1;

	// ��踦 ���� ���� �⺻���� �������� ���� �����ϴ°���
	//ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);

	//csj
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN , true);

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunLoanStaticsList") );

	CString sResultField;
	if( sXAlias.Compare(_T("��")) == 0 || (sXAlias.Compare(_T("��")) == 0) || (sXAlias.Compare(_T("��")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����") , sResultField );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�����2") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunLoanStaticsList") );


	// ����� �Ϻ��� ��踦 �����ϴ°��� �����ϴ��� �˻�
	CString sDayField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("�Ϻ�") , sDayField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	sDayField.TrimLeft(); sDayField.TrimRight();

	if( !sDayField.IsEmpty() )
	{
		ids = GetCellData( m_pUserMakeSearchCondDM , _T("�˸��ƽ�") , sDayField , _T("FieldName") , sDayField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunLoanStatics") );
	}

	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM ���̺� �̸� �߰�
		m_pResultListDM->TBL_NAME = saTableName.GetAt(i);

		sTableName = saTableName.GetAt(i);
		
		// �⺻ �˻� ���ǿ� ����ڰ˻�����,�׷������� ���Խ�Ų��.
		CString sWhere;
		
		ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXValue, sYValue , _T("Y"), _T("Y"), i, sWhere);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunLoanStaticsList") );

		ids = FillStaticsListValue( sStaticsClass, sXFieldName, sYFieldName, sXValue, sYValue, sWhere, bDupDel, sResultField, sDayField);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunLoanStaticsList") );
	}
	return 0;

	EFS_END
	return -1;

}

// ������ �����Ͽ� ���� Real DM�� ���� �ִ´�.
INT CStaticsInfoMgr::FillStaticsListValue( CString sStaticsClass, CString sXFieldName, CString sYFieldName, CString sXData, CString sYData, CString sWhere, bool bDupDel, CString sResultField, CString sDayField)
{
	EFS_BEGIN

	INT ids;	
	CString sSendWhere;							// ���� SQL��	
	CString sTmpXData;
	CString strListField;
	CString sTableName = m_pResultListDM->TBL_NAME;
	
	ids = GetCellData( m_pStaticsMainInfoDM , _T("�������") , sStaticsClass , _T("��¹�") , strListField );
	if ( ids < 0 || strListField.IsEmpty() ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsListValue()") );

	CStringArray saListField;
	ids = CLocCommonAPI::MakeArrayList( strListField , _T("/") , saListField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsListValue()") );

	sResultField = GetRecKey(sResultField);

	if(!sResultField.IsEmpty() && (sXData.IsEmpty() || sYData.IsEmpty()))
	{	
		if(!bDupDel && sDayField.IsEmpty()) // �ߺ�����
		{
			ids = SetDataMgrRefList( m_pResultListDM , sResultField , _T(""), _T("Y"), 0);
		}
		else // �ߺ�����
		{ 
			ids = SetDataMgrRefList( m_pResultListDM , _T("DISTINCT ") + sResultField , _T(""), _T("Y"), 0);
		}
	}
	else
	{
		if(sDayField.IsEmpty() || sResultField.IsEmpty())
		{
			ids = SetDataMgrRefList( m_pResultListDM , sResultField , _T(""), _T("N"), 0);
		}
		else // �ߺ�����
		{
			ids = SetDataMgrRefList( m_pResultListDM , _T("DISTINCT ") + sResultField , _T(""), _T("Y"), 0);
		}
	}
	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsListValue") );

	for(INT i = 0; i < saListField.GetSize(); i++)
	{
		ids = SetDataMgrRefList( m_pResultListDM , saListField.GetAt(i) , _T(""), _T("Y"), i+1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("FillStaticsListValue") );
	}	

	if(!sWhere.IsEmpty())
	{
		CString sTmpWhere = _T("");
		if(sXData != "")
		{
			if(sXData != _T("NULL"))
			{
				sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName, sXData);
			}
			else 
			{
				sTmpWhere.Format( _T(" AND %s IS NULL"),sXFieldName);
			}
			sWhere += sTmpWhere;
		}
		if(sYData != "")
		{
			if(sYData != _T("NULL"))
			{
				sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName, sYData);
			}
			else 
			{
				sTmpWhere.Format( _T(" AND %s IS NULL"),sYFieldName);
			}
			sWhere += sTmpWhere;
		}
	}

	// WHERE ���� ����
	sSendWhere = sWhere;

	CString sWhereSql;
	CString sSelectFields;
	CString sGroupByFields;

	if( !sDayField.IsEmpty() && !sResultField.IsEmpty() )
	{
		sSelectFields.Empty();
		sGroupByFields.Empty();
		sWhereSql.Empty();
		
		sSelectFields.Format( _T("%s") , sResultField );
		sGroupByFields.Format( _T("%s, %s") ,  sDayField, sResultField);			
		sWhereSql.Format( _T(" AND %s IN ( SELECT %s FROM %s WHERE %s GROUP BY %s )"), sResultField, sResultField , sTableName , sWhere , sGroupByFields );

		sSendWhere += sWhereSql;
	}

	// RESULT DM -> REAL RESULT DM ���� �̵�
	ids = m_pResultListDM->RefreshDataManager( sSendWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("FillStaticsListValue") );

	return 0;

	EFS_END
	return -1;

}

// ��������� ���� ��������� ����
// Return Value (0: USER LIST, 1: BOOK LIST)
INT CStaticsInfoMgr::GetListType(CString sStaticsClass)
{

	if( sStaticsClass == _T("������ ������") || sStaticsClass == _T("�̴����� ���") || sStaticsClass == _T("������ ���") ||
	    sStaticsClass == _T("�ݳ��� ���") || sStaticsClass == _T("ȸ�����߱����") )
	{
		return 0;
	}
	else //�����ڷ����/�ݳ��ڷ����/�ݳ������ڷ����/�߰������ڷ����
	{
		return 1;
	}
}

CString CStaticsInfoMgr::GetRecKey(CString sResultField)
{
	// �ߺ������� ���� Ű�� ã��
	INT nStart, nEnd;
	CString keyField;
	nStart = sResultField.Find(_T("("));
	nEnd = sResultField.Find(_T(")"))-1;
	sResultField = sResultField.Mid(nStart+1, nEnd-nStart);
	sResultField.Replace(_T("DISTINCT"), _T(""));
	sResultField.Replace(_T("*"), _T(""));
	sResultField.TrimLeft();sResultField.TrimRight();
	return sResultField;
}

INT CStaticsInfoMgr::SetDataMgrRefList( CESL_DataMgr *pDM, CString sCondition, CString sInitValue, CString sGroupYN, INT nCol )
{
	EFS_BEGIN

	CString sUdf;

	if( sGroupYN.Compare(_T("Y")) == 0 ) sUdf = _T("");
	else sUdf = _T("udf");

	POSITION pos = pDM->RefList.FindIndex(nCol);
	if (pos == NULL) return -1;

	CESL_DataMgr::reference * pRef = (CESL_DataMgr::reference*)pDM->RefList.GetAt(pos);
	if (pRef == NULL) return -2;

	pRef->INIT_VALUE = sInitValue;
	pRef->DATA_TYPE = sUdf;
	pRef->FIELD_NAME = sCondition;

	return 0;

	EFS_END
	return -1;

}
/*//END------------------------------------------------------------------------*/


void	CStaticsInfoMgr::SetInit_CStatics_AXISVal_CELLCLICK(CStatics_AXISVal_CELLCLICK* p)
{
	m_pAxisCode_CellClick = p;
	m_pAxisCode_CellClick->Clear();
}


//-----------------------------------------------------------------------------

void	_CStatics_AXISVal_CELLCLICK_Alias::Clear()
{
	arStr_FieldName.RemoveAll();
	arStr_FieldValue.RemoveAll();
}

void	_CStatics_AXISVal_CELLCLICK_Alias::AddFieldNameVal( int index_Table , CString name , CString val )
{
	if( arStr_FieldName.GetSize() <= index_Table )
	{
		arStr_FieldName.Add( name );
		arStr_FieldValue.Add( val );
	}else{
		arStr_FieldName[index_Table]  = name;
		arStr_FieldValue[index_Table] = val;
	}
}

CString		_CStatics_AXISVal_CELLCLICK_Alias::GetWhere( int index_Table )
{
	if( arStr_FieldName.GetSize() <= index_Table ) return "";
	CString	where;
	where.Format( _T(" %s = '%s' ") , arStr_FieldName[index_Table]  , arStr_FieldValue[index_Table] );
	return where;
}
//-----------------------------------------------------------------------------


CStatics_AXISVal_CELLCLICK::CStatics_AXISVal_CELLCLICK()
{

}

CStatics_AXISVal_CELLCLICK::~CStatics_AXISVal_CELLCLICK()
{
	Clear();
}

void	CStatics_AXISVal_CELLCLICK::Clear()
{
	for( int i = 0 ; i < 2 ; i++ )	
	{
		CArray<_CStatics_AXISVal_CELLCLICK_Alias*,_CStatics_AXISVal_CELLCLICK_Alias*> *pAxis = GetAxis( i );
		for( int o = 0 ; o < pAxis->GetSize(); o++ )
		{
			_CStatics_AXISVal_CELLCLICK_Alias* p = pAxis->GetAt(o);
			p->Clear();
			delete p;
		}
		pAxis->RemoveAll();
	}
}

CArray< _CStatics_AXISVal_CELLCLICK_Alias*,_CStatics_AXISVal_CELLCLICK_Alias*>* CStatics_AXISVal_CELLCLICK::GetAxis( int axis )
{
	if( axis == 0 )
	{
		return &m_arAXISVal_X;
	}else if( axis == 1 ){
		return &m_arAXISVal_Y;
	}
	return NULL;
}	


void	CStatics_AXISVal_CELLCLICK::AddFieldNameVal( int axis , int arIdx , int index_Table , CString name , CString val )
{
	CArray<_CStatics_AXISVal_CELLCLICK_Alias*,_CStatics_AXISVal_CELLCLICK_Alias*>* pAxis = GetAxis( axis );

	_CStatics_AXISVal_CELLCLICK_Alias* pField = NULL;
	if( pAxis->GetSize() <= arIdx )
	{
		pField = new _CStatics_AXISVal_CELLCLICK_Alias();
		pAxis->Add( pField );
		pField->axis = axis;
	}

	if( pAxis->GetSize() <= arIdx )
	{
		// ����!!! , ���������� �ȵ��Դ�.
		// �̷��� ���°��� ��������..
		AfxMessageBox( _T("���� : CStatics_AXISVal_CELLCLICK::AddFieldNameVal ") );
		return;
	}

	pField = pAxis->GetAt( arIdx );
	pField->AddFieldNameVal( index_Table , name , val );
}

void	CStatics_AXISVal_CELLCLICK::SetCodeToDesc( int axis , int arIdx , CString Alias , CString Code , CString Desc )
{
	CArray<_CStatics_AXISVal_CELLCLICK_Alias*,_CStatics_AXISVal_CELLCLICK_Alias*>* pAxis = GetAxis( axis );
	if( pAxis->GetSize() <= arIdx )
	{
		AfxMessageBox( _T("���� : CStatics_AXISVal_CELLCLICK::SetCodeToDesc ") );
		return;
	}
	_CStatics_AXISVal_CELLCLICK_Alias*	pField = pAxis->GetAt( arIdx );
	pField->Code = Code;
	pField->Desc = Desc;
}

// int axis : �� ���� ����
// int index_Table : ���̺� �ε��� ( ���� ���̺� , �������̺� )
// CString Desc_Click : �׸��忡�� Ŭ���� ���� �� �÷� �̸� ( ESL_Grid ���� ������ )
// 
int		CStatics_AXISVal_CELLCLICK::GetWhere( int axis , int index_Table , CString Desc_Click , CString& out_Where )
{	
	out_Where = _T("");

	CArray<_CStatics_AXISVal_CELLCLICK_Alias*,_CStatics_AXISVal_CELLCLICK_Alias*>* pAxis = GetAxis( axis );
	
	int i;
	_CStatics_AXISVal_CELLCLICK_Alias* pField = NULL;
	for( i = 0 ; i < pAxis->GetSize() ; i ++)
	{
		_CStatics_AXISVal_CELLCLICK_Alias* p = pAxis->GetAt(i);
		if( p->Desc == Desc_Click )
		{
			pField = p;
			break;
		}
	}

	if( pField == NULL ) return -1;

	CString where_field = pField->GetWhere( index_Table );
	if( where_field.IsEmpty() )
	{
		return -1;
	}
	where_field = _T(" AND ") + where_field;
	out_Where = where_field;
	return 0;
}

//-----------------------------------------------------------------------------

//
// KOL.RED.2018.211 ---------------------------------------------------------------------------
