// CatApi_SE.cpp: implementation of the CCatApi_SE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CatApi_SE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "..\..\����\����\SE_API\SeApi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCatApi_SE::CCatApi_SE(CESL_Mgr *pParentMgr)
	: CCatApi(pParentMgr)
{
	LoadRegCode();
	Init();
}

CCatApi_SE::~CCatApi_SE()
{

}

INT CCatApi_SE::InitMemberData()
{
	EFS_BEGIN

	m_mapMarcAlias.RemoveAll();
	m_mapCatDMtoMarc.RemoveAll();
	m_mapIndexDMtoIndexToolKit.RemoveAll();
	m_mapWorkingStatusCodeDesc.RemoveAll();

	m_mapMarcAlias.SetAt(_T("�����"), _T("245$n"));
	m_mapMarcAlias.SetAt(_T("����"), _T("245$p"));
	m_mapMarcAlias.SetAt(_T("����"), _T("245$a"));
	m_mapMarcAlias.SetAt(_T("�μ���"), _T("245$b"));
	m_mapMarcAlias.SetAt(_T("����"), _T("245$x"));
	m_mapMarcAlias.SetAt(_T("����"), _T("245$d"));
	m_mapMarcAlias.SetAt(_T("������"), _T("245$e"));
	m_mapMarcAlias.SetAt(_T("������"), _T("260$b"));
	m_mapMarcAlias.SetAt(_T("�Ѽ�ǥ��"), _T("440$a"));
	m_mapMarcAlias.SetAt(_T("�Ѽ�����"), _T("440$n"));
	m_mapMarcAlias.SetAt(_T("����"), _T("350$a"));
	m_mapMarcAlias.SetAt(_T("�����"), _T("300$a"));
	m_mapMarcAlias.SetAt(_T("������Ư��"), _T("300$b"));
	m_mapMarcAlias.SetAt(_T("ũ��"), _T("300$c"));
	m_mapMarcAlias.SetAt(_T("�����ڷ�"), _T("300$e"));

	m_mapCatDMtoMarc.SetAt(_T("ISS_�ǻ���"), _T("250$a"));
	m_mapCatDMtoMarc.SetAt(_T("ISS_������"), _T("260$a"));
	m_mapCatDMtoMarc.SetAt(_T("ISS_������"), _T("260$b"));
	m_mapCatDMtoMarc.SetAt(_T("ISS_�����"), _T("260$c"));

	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�˻���������"), _T("������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�����˻������⵵"), _T("�����˻������⵵"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�����˻��������"), _T("�����˻��������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�˻������Ŀ뺻ǥ��"), _T("�����˻��뺻ǥ��"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_Ű����"), _T("Ű��������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�˻��������"), _T("������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�˻��뺻ǥ��"), _T("��ǥ��"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_ǥ������"), _T("ǥ������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�����ڻ���"), _T("�����ڻ���"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�����ڻ���"), _T("�����ڻ���"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_����⵵����"), _T("����⵵����"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_��������"), _T("��������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_����"), _T("����"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_������ȣ"), _T("��Ÿ��ȣ_û����ȣ_������ȣ"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_��å��ȣ"), _T("��Ÿ��ȣ_û����ȣ_��å��ȣ"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_û��_��åDISP"), _T("��Ÿ��ȣ_û����ȣ_��å��ȣ_DISP"));

	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�����˻��뺻ǥ��"), _T("�����˻��뺻ǥ��"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_����⵵2"), _T("�����"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_ISBN"), _T("ǥ�غ�ȣ_ISBN"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_STRN"), _T("ǥ�غ�ȣ_STRN"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_RNSTRN"), _T("ǥ�غ�ȣ_RNSTRN"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_����������ȣ"), _T("ǥ�غ�ȣ_����������ȣ"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_���۱ǵ�Ϲ�ȣ"), _T("ǥ�غ�ȣ_���۱ǵ�Ϲ�ȣ"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�з���ȣKDCP"), _T("KDCP"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�з���ȣKDC"), _T("KDC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�з���ȣDDC"), _T("DDC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�з���ȣCEC"), _T("CEC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�з���ȣCWC"), _T("CWC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�з���ȣCOC"), _T("COC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�з���ȣUDC"), _T("UDC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�з���ȣNDC"), _T("NDC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�з���ȣLC"), _T("LC"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�ѱ����к�ȣ"), _T("�ѱ����к�ȣ"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�ѱ����α����ȣ"), _T("���α����ȣ"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_�������"), _T("�������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("ISS_��๮���"), _T("��๮���"));

	m_mapWorkingStatusCodeDesc.SetAt(_T("SEA111N"),_T("�����Լ��ڷ�(�ΰ����ڷ�)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEA112N"),_T("���԰�ȣ�ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEA211N"),_T("�����Լ��ڷ�(�ΰ����ڷ�)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEA212N"),_T("������ȣ�ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEB111N"),_T("�����ΰ��ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEB112N"),_T("���������ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEB113N"),_T("���������ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEB114N"),_T("�����˼��ڷ�(����ΰ����ڷ�)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL111N"),_T("�ǽð��ΰ��ڷ�(�ǽð��谡����ڷ�)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL112N"),_T("�ǽð��谡�ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL113O"),_T("�ǽð�����ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SER111N"),_T("����ΰ��ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SER112N"),_T("����ڷ�(�����ΰ����ڷ�)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SER113O"),_T("�����ι�ȣ�ο��ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEC111N"),_T("��������ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEC112O"),_T("�����ΰ����ڷ�(�����Ϸ��ڷ�)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL211O"),_T("�����ΰ��ڷ�(�谡����ڷ�)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL212N"),_T("�谡�ڷ�(�����ڷ�)"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL213O"),_T("���������ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL214O"),_T("�ļ��ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL215O"),_T("�����ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL216O"),_T("�н��ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL217O"),_T("�������ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL311O"),_T("���ܴ����ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL312O"),_T("���������ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL411O"),_T("����å�η����������ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL511O"),_T("���յ���Ÿ���ݳ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL313O"),_T("Ư�������ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL314O"),_T("��޴����ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL315O"),_T("����ڷ�"));
	m_mapWorkingStatusCodeDesc.SetAt(_T("SEL317O"),_T("�������ڷ�"));

	m_map008ToUseObjCode.SetAt(_T(" "), _T("PU"));
	m_map008ToUseObjCode.SetAt(_T("j"), _T("JU"));
	m_map008ToUseObjCode.SetAt(_T("a"), _T("JU"));
	m_map008ToUseObjCode.SetAt(_T("b"), _T("ES"));
	m_map008ToUseObjCode.SetAt(_T("c"), _T("MS"));
	m_map008ToUseObjCode.SetAt(_T("d"), _T("HS"));
	m_map008ToUseObjCode.SetAt(_T("e"), _T("AD"));
	m_map008ToUseObjCode.SetAt(_T("f"), _T("SP"));

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyDefaultValue_Species(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Book)
{
	EFS_BEGIN

	if (pDM_Species == NULL) return -1;
	
	if ( pDM_Species->GetCellData( _T("SS_�۾����±���"), 0).IsEmpty() )
	{
		pDM_Species->SetCellData( _T("SS_�۾����±���"), _T("1"), 0 );
	}

	if (pDM_Species->GetCellData( _T("SS_���Գ⵵"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_���Գ⵵"), CTime::GetCurrentTime().Format(_T("%Y")), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_�Է±���"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_�Է±���"), _T("0"), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_�Է���"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_�Է���"), CTime::GetCurrentTime().Format(_T("%Y%m%d")), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_�����Լ�����"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_�����Լ�����"), _T("N"), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_��������"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_��������"), _T("N"), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_������ȣ�Ϸù�ȣ"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_������ȣ�Ϸù�ȣ"), _T("0"), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_�����Լ�����"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_�����Լ�����"), _T("N"), 0);
	}

	if (pDM_Species->GetCellData( _T("SS_�����Լ�����"), 0).IsEmpty())
	{
		pDM_Species->SetCellData( _T("SS_�����Լ�����"), _T("N"), 0);
	}

	pDM_Species->SetCellData( _T("SS_�۾���"), m_pParnetMgr->m_pInfo->USER_ID, 0);

	if (pDM_Species->GetCellData( _T("SS_�̿����ѱ���_�ڵ�"), 0).IsEmpty())
	{
		CString strUseLitmitCode;
		GetSpeciesUseLimitCode(pDM_Book, strUseLitmitCode, _T("SE"));
		pDM_Species->SetCellData( _T("SS_�̿����ѱ���_�ڵ�"), strUseLitmitCode, 0);
	}

	INT ids = -1;
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("SS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	CString strClass;
	CString strVersion;

	CArray <CString ,CString&> arrClass ;    
	m_pMarcMgr->GetItem(&marc,_T("056$a"),strClass,&arrClass);
    if(arrClass.GetSize () >1) 
	{
		strClass.Empty () ;
		for(INT i = 0 ; i < arrClass.GetSize () ; i++)
		{   
			if(strClass.IsEmpty())
			strClass = arrClass.GetAt (i) ;
			else
			strClass += _T(" ") + arrClass.GetAt (i) ;
		}
	}

	m_pMarcMgr->GetItem(&marc, _T("056$2"), strVersion);
	pDM_Species->SetCellData( _T("SS_KDC�з���ȣ"), strClass, 0);
	pDM_Species->SetCellData( _T("SS_KDC����"), strVersion, 0);

	arrClass.RemoveAll () ;
	m_pMarcMgr->GetItem(&marc,_T("082$a"),strClass,&arrClass);
    if(arrClass.GetSize () >1) 
	{
		strClass.Empty () ;
		for(INT i = 0 ; i < arrClass.GetSize () ; i++)
		{   
			if(strClass.IsEmpty())
				strClass = arrClass.GetAt (i) ;
			else
				strClass += _T(" ") + arrClass.GetAt (i) ;
		}
	}
	m_pMarcMgr->GetItem(&marc, _T("082$2"), strVersion);
	pDM_Species->SetCellData( _T("SS_DDC�з���ȣ"), strClass, 0);
	pDM_Species->SetCellData( _T("SS_DDC����"), strVersion, 0);

	arrClass.RemoveAll () ;
	m_pMarcMgr->GetItem(&marc,_T("085$a"),strClass,&arrClass);
    if(arrClass.GetSize () >1) 
	{
		strClass.Empty () ;
		for(INT i = 0 ; i < arrClass.GetSize () ; i++)
		{   
			if(strClass.IsEmpty())
				strClass = arrClass.GetAt (i) ;
			else
				strClass += _T(" ") + arrClass.GetAt (i) ;
		}
	}	
	pDM_Species->SetCellData( _T("SS_LC�з���ȣ"), strClass, 0);

	CString sTmpFirstWork;
	pDM_Species->GetCellData(_T("FIRST_WORK"), 0, sTmpFirstWork);
		
	if ( sTmpFirstWork == _T("") )
	{
		pDM_Species->SetCellData(_T("FIRST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), 0);
	}
	pDM_Species->SetCellData(_T("LAST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), 0);

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyDefaultValue_Index(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book)
{
	EFS_BEGIN

	if (pDM_Index == NULL || pDM_Book == NULL) return -1;

	INT ids = -1;
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("SS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	
	CString strClassCode;
	ids = m_pMarcMgr->GetItem(&marc, _T("090$a"), strClassCode);
	if (ids < 0) 
	{
		return 0;
	}

	CString strBookCode;
	ids = m_pMarcMgr->GetItem(&marc, _T("090$b"), strBookCode);
	
	ids = pDM_Index->SetCellData( _T("ISS_�з���ȣ"), strClassCode, 0);
	ids = pDM_Index->SetCellData( _T("ISS_������ȣ"), strBookCode, 0);

	CString strClass;
	pDM_Species->GetCellData( _T("SS_KDC�з���ȣ"), 0, strClass);
	pDM_Index->SetCellData( _T("ISS_�з���ȣKDC"), strClass, 0);

	pDM_Species->GetCellData( _T("SS_DDC�з���ȣ"), 0, strClass);
	pDM_Index->SetCellData( _T("ISS_�з���ȣDDC"), strClass, 0);

	pDM_Species->GetCellData( _T("SS_LC�з���ȣ"), 0, strClass);
	pDM_Index->SetCellData( _T("ISS_�з���ȣLC"), strClass, 0);

	CString sPriceChar;
	ids = m_pMarcMgr->GetItem(&marc, _T("950$a"), sPriceChar);
	for ( INT i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {
		pDM_Book->SetCellData( _T("SB_���ݼ���"), sPriceChar, i );
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyDefaultValue_Book(CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Index)
{
	EFS_BEGIN

	if (pDM_Book == NULL || pDM_Index == NULL) return -1;

	INT ids = -1;
	CIndex *pIndex = m_pIndexTool->GetIndex();
	CString strItem;
	CString strAcqCode = pDM_Index->GetCellData( _T("ISS_���Ա���_�ڵ�"), 0);
	CString strInpuDate = GetToday();

	CString strClassCode = pDM_Index->GetCellData( _T("ISS_�з���ȣ"), 0);
	CString strBookCode = pDM_Index->GetCellData( _T("ISS_������ȣ"), 0);

	CString strClass;
	ids = pDM_Index->GetCellData( _T("ISS_�з�ǥ����"), 0, strClass);
	ids = pDM_Book->SetCellData( _T("SB_�з���ȣ����_�ڵ�"), strClass, 0);

	INT nRowCount = pDM_Book->GetRowCount();

	CString sAccessionRecKey;
	CString sClassNo;
	CString sBookCode;
	CString sClassNoType;
	for ( INT i = 0 ; i < nRowCount ; i++ ) {
		CString sRepresentBookYn = pDM_Book->GetCellData( _T("SB_��ǥå����"), i );
		if ( sRepresentBookYn == _T("Y") ) {
			sAccessionRecKey = pDM_Book->GetCellData( _T("SB_��Ͽ���KEY"), i );
			sClassNo = pDM_Book->GetCellData( _T("SB_�з���ȣ"), i );
			sClassNoType = pDM_Book->GetCellData( _T("SB_�з���ȣ����_�ڵ�"), i );
			sBookCode = pDM_Book->GetCellData( _T("SB_������ȣ"), i );
			break;
		}
	}

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("SB"))) continue;

		if (pDM_Book->GetCellData( _T("SB_���Ա���_�ڵ�"), idx).IsEmpty())
			pDM_Book->SetCellData( _T("SB_���Ա���_�ڵ�"), strAcqCode, idx);
		if (pDM_Book->GetCellData( _T("SB_�Է���"), idx).IsEmpty())
			pDM_Book->SetCellData( _T("SB_�Է���"), strInpuDate, idx);

		ids = pDM_Book->GetCellData( _T("SB_��ǥå����"), idx, strItem);
		if (ids < 0 || strItem.CompareNoCase(_T("Y")) != 0)
			pDM_Book->SetCellData( _T("SB_��ǥå����"), _T("N"), idx);

		if (pDM_Book->GetCellData( _T("SB_����"), idx).IsEmpty()) 
			pDM_Book->SetCellData( _T("SB_����"), _T("0"), idx);

		ids = pDM_Book->GetCellData( _T("SB_����ISBN"), idx, strItem);
		pIndex->MakeIndex(strItem);
		pDM_Book->SetCellData( _T("SB_����ISBN"), strItem, idx);

		ids = pDM_Book->SetCellData( _T("SB_�з���ȣ"), strClassCode, idx);
		
		ids = pDM_Book->SetCellData( _T("SB_������ȣ"), strBookCode, idx);

		CString strItemData = pDM_Index->GetCellData( _T("ISS_�з�ǥ����"), 0);
		ids = pDM_Book->SetCellData( _T("SB_�з���ȣ����_�ڵ�"), strItemData, idx);

		CString sTmpFirstWork;
		pDM_Book->GetCellData(_T("FIRST_WORK"), idx, sTmpFirstWork);
		if ( sTmpFirstWork == _T("") )
		{
			pDM_Book->SetCellData(_T("FIRST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), idx);
		}
		pDM_Book->SetCellData(_T("LAST_WORK"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), idx);

	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyExtraQuery(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book, CString &strSpeciesKey, CStringArray *pArrStrAddFrame /*= NULL*/)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book) return -1;

	CMap<CString, LPCTSTR, CString, LPCTSTR> mapFuncName3Fields;
	
	mapFuncName3Fields.SetAt(_T("PLACE_INFO"), _T("SHELF_LOC_CODE"));
	mapFuncName3Fields.SetAt(_T("USE_LIMIT_CODE"), _T("USE_LIMIT_CODE"));
	mapFuncName3Fields.SetAt(_T("MANAGE_CODE"), _T("MANAGE_CODE"));
	mapFuncName3Fields.SetAt(_T("REG_CODE"), _T("REG_CODE"));

	CString strQuery;
	CString strSetQuery;

	CString strTemp;

	strTemp.Format(_T("IDX_HOLD_PLACE = GET_SE_MAIN_SHELF_CODE(%s)"), strSpeciesKey);
	strSetQuery += strTemp;

	strTemp.Format(_T(", MAIN_PLACE_INFO = GET_SE_MAIN_SHELF_CODE(%s)"), strSpeciesKey);
	strSetQuery +=  strTemp;

	CString strField;
	CString strParam;
	POSITION pos = mapFuncName3Fields.GetStartPosition();
	while (pos)
	{
		mapFuncName3Fields.GetNextAssoc(pos, strField, strParam);
		
		strTemp.Format(_T(", %s = GET_SE_BOOK_INFO(%s, '%s')"), strField, strSpeciesKey, strParam);
		strSetQuery += strTemp;
	}

	strQuery.Format(_T("UPDATE %s SET %s WHERE REC_KEY = %s;"), pDM_Index->TBL_NAME, strSetQuery, strSpeciesKey);
	INT ids = pDM_Species->AddFrame(strQuery);
	if (ids < 0) return ids;

	CString strRecKey;
	ids = pDM_Species->MakeRecKey(strRecKey);
	if (ids < 0) return ids;

	strQuery.Format(_T("INSERT INTO IDX_TMP_SE_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
					_T("VALUES (%s, %s, 'I', '%s');")
					, strRecKey, strSpeciesKey, GetToday());

	ids = pDM_Species->AddFrame(strQuery);
	if (ids < 0) return ids;

	CESL_DataMgr *pDmLastNum = m_pParnetMgr->FindDM( _T("DM_����_��������ȣ") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("DM_����_��������ȣ Ȯ��") );
		return -1;
	}
	for ( INT i = 0 ; i < pDmLastNum->GetRowCount() ; i++ ) {
		CString sRegCode = pDmLastNum->GetCellData( _T("SB_��ϱ���_�ڵ�"), i );
		CString sLastNum = pDmLastNum->GetCellData( _T("��������ȣ"), i );
		INT iLastNum = _ttoi ( sLastNum );
		CString sQuery;

		sQuery.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER='%d', LAST_WORK='%s' ")
						_T("WHERE DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;") 
						, iLastNum, _T("�ڵ���Ϲ�ȣ�ο�"), sRegCode );

		ids = pDM_Species->AddFrame( sQuery );
		if ( 0 > ids ) {
			AfxMessageBox( _T("��������ȣ AddFrame Ȯ��") );
			return -1;
		}
	}

	if (pArrStrAddFrame)
	{
		INT nRowCount = pArrStrAddFrame->GetSize();
		for (INT idx = 0; idx < nRowCount; idx++) {
			ids = pDM_Species->AddFrame(pArrStrAddFrame->GetAt(idx));
			if (ids < 0) return ids;
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyIndex(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book) return -1;

	INT ids = -1;
	CMarc marc;
	CString strStreamMarc;
	strStreamMarc = pDM_Species->GetCellData( _T("SS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return ids;

	if (m_pIndexTool == NULL)
		m_pIndexTool = new CIndexTool;
	m_pIndexTool->Init(m_pMarcMgr, &marc);

	CString strIndexItem;
	CString strIndexDMFieldAlias;
	CString strIndexToolKitAlias;
	POSITION pos = m_mapIndexDMtoIndexToolKit.GetStartPosition();
	while (pos)
	{
		strIndexItem.Empty();
		m_mapIndexDMtoIndexToolKit.GetNextAssoc(pos, strIndexDMFieldAlias, strIndexToolKitAlias);
		
		ids = m_pIndexTool->GetIndex(strIndexToolKitAlias, strIndexItem);
		if (ids < 0) continue;

		pDM_Index->SetCellData(strIndexDMFieldAlias, strIndexItem, 0);
	}

	CString strItemData;

	ids = pDM_Index->GetCellData( _T("ISS_������"), 0, strItemData);
	if (strItemData.IsEmpty())
	{
		m_pMarcMgr->GetItem(&marc, _T("245$d"), strItemData);
		DeleteRoleWord(strItemData);
		pDM_Index->SetCellData( _T("ISS_������"), strItemData, 0);

		if (strItemData.IsEmpty()) 
		{
			CString strFormCode;
			pDM_Index->GetCellData( _T("ISS_���ı���_�ڵ�"), 0, strFormCode);
			if (strFormCode == _T("BK")) 
			{
				m_pMarcMgr->GetItem(&marc, _T("100$a"), strItemData);
				pDM_Index->SetCellData( _T("ISS_������"), strItemData, 0);
			} 
			else 
			{
				m_pMarcMgr->GetItem(&marc, _T("700$a"), strItemData);
				pDM_Index->SetCellData( _T("ISS_������"), strItemData, 0);
			}
		}
	}

	m_pMarcMgr->GetItem(&marc, _T("008@7-10"), strItemData);
	pDM_Index->SetCellData( _T("ISS_â����"), strItemData, 0);

	m_pMarcMgr->GetItem(&marc, _T("008@11-14"), strItemData);
	pDM_Index->SetCellData( _T("ISS_�󰣳�"), strItemData, 0);

	m_pMarcMgr->GetItem(&marc, _T("022$a"), strItemData);
	pDM_Index->SetCellData( _T("ISS_ISSN"), strItemData, 0);

	m_pMarcMgr->GetItem(&marc, _T("300$a"), strItemData);
	pDM_Index->SetCellData( _T("ISS_CODEN"), strItemData, 0);

	m_pMarcMgr->GetItem(&marc, _T("008@18"), strItemData);
	if (strItemData.GetLength()==0) strItemData = _T("_");
	pDM_Index->SetCellData( _T("ISS_�����"), strItemData, 0);

	m_pMarcMgr->GetItem(&marc, _T("008@6"), strItemData);
	pDM_Index->SetCellData( _T("ISS_�������"), strItemData, 0);

	ids = pDM_Index->GetCellData( _T("ISS_��ǥ��"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		pDM_Index->GetCellData( _T("ISS_�˻��뺻ǥ��"), 0, strItemData);
		pDM_Index->SetCellData( _T("ISS_��ǥ��"), strItemData, 0);
	}

	ids = pDM_Index->GetCellData( _T("ISS_�����˻��������"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("������"), strItemData);
		pDM_Index->SetCellData( _T("ISS_�����˻��������"), strItemData, 0);
	}

	ids = pDM_Index->GetCellData( _T("ISS_������"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("������"), strItemData);
		pDM_Index->SetCellData( _T("ISS_������"), strItemData, 0);
	}

	ids = pDM_Index->GetCellData( _T("ISS_�����"), 0, strItemData);	
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("�����˻������⵵"), strItemData);
		pDM_Index->SetCellData( _T("ISS_�����"), strItemData, 0);
	}

	CString strIdxVolTitle, strIdxPlaceCode, strIdxSeShelfCode;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		ids = pDM_Book->GetCellData( _T("SB_���������"), idx, strItemData);
		if (ids >= 0 && !strItemData.IsEmpty()) strIdxVolTitle += _T(" ") + strItemData;
	
		ids = pDM_Book->GetCellData( _T("SB_�谡��ġ��ȣ"), idx, strItemData);
		if (ids >= 0 && !strItemData.IsEmpty()) strIdxPlaceCode += _T(" ") + strItemData;

		ids = pDM_Book->GetCellData( _T("SB_��ġ��ȣ_�ڵ�"), idx, strItemData);
		if (ids >= 0 && !strItemData.IsEmpty()) strIdxSeShelfCode += _T(" ") + strItemData;
		
	}
	if (strIdxVolTitle.Mid(0,1)==_T(" ")) strIdxVolTitle = strIdxVolTitle.Mid(1);
	if (strIdxPlaceCode.Mid(0,1)==_T(" ")) strIdxPlaceCode = strIdxPlaceCode.Mid(1);
	if (strIdxSeShelfCode.Mid(0,1)==_T(" ")) strIdxSeShelfCode = strIdxSeShelfCode.Mid(1);
	pDM_Index->SetCellData( _T("ISS_�Ǽ���"), strIdxVolTitle, 0);
	pDM_Index->SetCellData( _T("ISS_�ڷ���ڵ�"), strIdxPlaceCode, 0);
	pDM_Index->SetCellData( _T("ISS_��ġ��ȣ�ڵ�"), strIdxSeShelfCode, 0);
	
	CString strIdxTitle;
	CString strIdxAuthor;
	CString strIdxPublisher;
	CString strIdxKeyword;

	ids = pDM_Index->GetCellData( _T("ISS_�˻��뺻ǥ��"), 0, strIdxTitle);
	ids = pDM_Index->GetCellData( _T("ISS_�˻���������"), 0, strIdxAuthor);
	ids = pDM_Index->GetCellData( _T("ISS_�˻��������"), 0, strIdxPublisher);
	ids = pDM_Index->GetCellData( _T("ISS_Ű����"), 0, strIdxKeyword);	
	
	CString strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublisher + _T(" ") + strIdxKeyword;
	CStringArray arrStrTarget;
	MakeIndexArray(&arrStrTarget, strIdxAllItem);
	
	INT nCount = arrStrTarget.GetSize();

	strIdxAllItem.Empty();
	for (idx = 0 ; idx < nCount ; idx++)
	{
		strIndexItem = arrStrTarget.GetAt(idx);

		if(strIdxAllItem.IsEmpty()) 
			strIdxAllItem = strIndexItem;
		else 
			strIdxAllItem += _T(" ") + strIndexItem;
	}
	pDM_Index->SetCellData(_T("ISS_��ü�˻�����ξ�"), strIdxAllItem, 0);

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyIndex_ST_CODE(CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book, INT nMode)
{
	EFS_BEGIN

	CString strSTCODE, strSETISBN, strDelimeter;

	CStringArray arrStrISBN;
		
	if (nMode > 0)
	{
		strDelimeter = _T(" ");
		pDM_Index->GetCellData( _T("ISS_ISBN"), 0, strSTCODE);
		
		MakeIndexArray(&arrStrISBN, strSTCODE);
	}

	pDM_Index->GetCellData( _T("ISS_��Ʈ_ISBN1"), 0, strSETISBN);
	if(strSETISBN.GetLength() > 0)
	{
		m_pIndexTool->GetIndex()->MakeIndex(strSETISBN);
		if(!IsStringInArray(&arrStrISBN, strSETISBN)) 
			arrStrISBN.Add(strSETISBN);
	}
	
	CString strEAISBN, strEXTRA_EAISBN, strDBFlag;
	strDelimeter.Format(_T("%c"), 3);
	INT rowCnt = pDM_Book->GetRowCount();
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM_Book->GetCellData( _T("SB_DB_FLAG"), i, strDBFlag);
		if(strDBFlag == _T("D")) continue;
		pDM_Book->GetCellData( _T("SB_����ISBN"), i, strEAISBN);
		m_pIndexTool->GetIndex()->MakeIndex(strEAISBN);
		if(!IsStringInArray(&arrStrISBN, strEAISBN)) arrStrISBN.Add(strEAISBN);
		pDM_Book->GetCellData( _T("SB_�߰�����ISBN"), i, strEXTRA_EAISBN);
		MakeIndexArray(&arrStrISBN, strEXTRA_EAISBN);
	}

	strSTCODE.Empty();
	for(i=0 ; i<arrStrISBN.GetSize() ; i++)
	{
		if(strSTCODE.IsEmpty()) strSTCODE = arrStrISBN.GetAt(i);
		else strSTCODE += _T(" ") + arrStrISBN.GetAt(i);
	}

	pDM_Index->SetCellData( _T("ISS_ISBN"), strSTCODE, 0);
	return 0;


	EFS_END
	return -1;
}

INT CCatApi_SE::ApplyShelfCodeData(CESL_DataMgr *pDM_Book, 
								   CMarc *pMarc, 
								   INT nAppMode,
								   BOOL bCatHold /*=FALSE*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL || !pMarc) return -1;

	CString strQuery =	_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
						_T("WHERE GROUP_1='L' AND GROUP_2='����' AND CLASS_ALIAS='�谡�ǿ켱����' ")
						_T("AND MANAGE_CODE=UDF_MANAGE_CODE ORDER BY VALUE_1");

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM_Book->CONNECTION_INFO);
	INT ids = dm.RestructDataManager(strQuery);
	if (ids < 0)
	{
		AfxMessageBox(_T("�谡�� �켱���� ������ ã�� �� �����ϴ�."));
		return ids;
	}

	INT nPriCnt = dm.GetRowCount();

	CString strShelfLocCode;
	CString strManageCode;
	CString strOption;
	CString strKDC;
	CString strDDC;
	CString strLC;
	CString strClassType;

	BOOL bSuccess = FALSE;
	INT nPriority = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		nPriority = 0;
		bSuccess = FALSE;
        
		CString strStatusCode, sDbFlag;
		strStatusCode = pDM_Book->GetCellData ( _T("SB_�ڷ����"), idx );
		sDbFlag = pDM_Book->GetCellData(  _T("SB_DB_FLAG"), idx );

		if ( (strStatusCode > _T("SEL212") && sDbFlag != _T("I") ) || sDbFlag == _T("D") )
			continue ;
		
		ids = pDM_Book->GetCellData( _T("SB_��������_�ڵ�"), idx, strManageCode);
		if (ids < 0) 
			strManageCode = _T("");
		
		while (nPriority < nPriCnt)
		{
			CString sTmp = dm.GetCellData(nPriority, 0);
			if (dm.GetCellData(nPriority, 0) == _T("�з���ȣ"))
			{
				strOption=_T("");
				strClassType=_T("");
				strKDC = _T("");
				strDDC = _T("");
				strLC = _T("");

				m_pMarcMgr->GetItem(pMarc, _T("056$a"), strKDC);
				m_pMarcMgr->GetItem(pMarc, _T("082$a"), strDDC);
				m_pMarcMgr->GetItem(pMarc, _T("085$a"), strLC);
				
				if ( !strKDC.IsEmpty() )
				{	
					strOption = strKDC;
					strClassType = _T("1");
				}
				else if ( !strDDC.IsEmpty() )
				{
					strOption = strDDC;
					strClassType = _T("2");
				}
				else if ( !strLC.IsEmpty() )
				{
					strOption = strLC;					
					strClassType = _T("7");
				}

				if ( strOption.IsEmpty() )
				{					
					AfxMessageBox(_T("�з���ȣ�� �����ϴ�.\n�谡���� ������ �� �����ϴ�."));
					return -1;
				}
				if ( strClassType==_T("") )
				{
					strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_CLS_NUM_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'SE' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("START_CLASS_NO <= '%s' AND END_CLASS_NO >= '%s'"))
								   , strManageCode, strOption, strOption);
				}
				else
				{
					strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_CLS_NUM_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'SE' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("START_CLASS_NO <= '%s' AND END_CLASS_NO >= '%s' AND CLASS_NO_TYPE='%s'"))
								   , strManageCode, strOption, strOption, strClassType);
				}
			}
			else if (dm.GetCellData(nPriority, 0) == _T("��ϱ���"))
			{
				ids = pDM_Book->GetCellData( _T("SB_��ϱ���_�ڵ�"), idx, strOption);
				if (ids < 0)
				{
					AfxMessageBox(_T("��ϱ����� �����ϴ�.\n�谡���� ������ �� �����ϴ�."));
					return -1;
				}
				strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_REG_CODE_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'SE' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("REG_CODE = '%s'")), strManageCode, strOption);
			}
			else if (dm.GetCellData(nPriority, 0) == _T("��ġ��ȣ"))
			{
				ids = pDM_Book->GetCellData( _T("SB_��ġ��ȣ_�ڵ�"), idx, strOption);
				if (ids < 0)
				{
					AfxMessageBox(_T("��ġ��ȣ�� �����ϴ�.\n�谡���� ������ �� �����ϴ�."));
					return -1;
				}

				strQuery.Format(_T("SELECT SHELF_LOC_CODE FROM MN_SEP_CODE_SHELF_RULE_MNG_TBL "
								   _T("WHERE PUBLISH_FORM_CODE = 'SE' AND ")
								   _T("(MANAGE_CODE IS NULL OR MANAGE_CODE = '%s') AND ")
								   _T("SEPARATE_SHELF_CODE = '%s'")), strManageCode, strOption);
			}
			else
			{
				nPriority++;
				continue;
			}

			ids = pDM_Book->GetOneValueQuery(strQuery, strShelfLocCode);
			if (ids < 0 || strShelfLocCode.IsEmpty())
			{
				nPriority++;
				continue;
			}
			else
			{
				if (!bCatHold || pDM_Book->GetCellData( _T("SB_�谡��ġ��ȣ"), idx).IsEmpty())
				{
					pDM_Book->SetCellData( _T("SB_�谡��ġ��ȣ"), strShelfLocCode, idx);

					CString strQueryFlag = GetQueryFlag(MNG_MODIFY, nAppMode, pDM_Book->GetCellData( _T("SB_DB_FLAG"), idx));
					pDM_Book->SetCellData( _T("SB_DB_FLAG"), strQueryFlag, idx);
				}
				bSuccess = TRUE;
				break;
			} 
		}

		if (!bSuccess)
		{
			AfxMessageBox(_T("�谡��ġ�� �����Ҽ� �����ϴ�."));
			return -1;
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::CompleteCataloging(CESL_DataMgr *pDM_Out, 
								CESL_DataMgr *pDM_Species, 
								INT nIdx /*= 0*/, 
								CESL_DataMgr *pDM_Book /*= NULL*/, 
								BOOL bCommit /*= TRUE*/, 
								CESL_DataMgr *pDM_Index /*= NULL*/, 
								CMarc *pMarc /*= NULL*/)
{
	EFS_BEGIN

	if (!pDM_Out ||
		!pDM_Species) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;

	CString strItem;
	CString strSpeciesKey;
	CString strBookKey;
	CString strControlCode;
	CString strQuery;

	CString strYear = CTime::GetCurrentTime().Format(_T("%Y"));
	
	ids = pDM_Species->GetCellData( _T("SS_��KEY"), nIdx, strSpeciesKey);
	if (ids < 0 || strSpeciesKey.IsEmpty()) 
	{
		m_strLastErrMsg = _T("��Ű�� ���� �ڷ��Դϴ�.");
		return -1;
	}

	CMarc marc;
	CString strStreamMarc;

	if (pMarc == NULL)
	{
		pMarc = &marc;
		
		ids = pDM_Species->GetCellData( _T("SS_MARC"), nIdx, strStreamMarc);
		ids = m_pMarcMgr->Decoding(strStreamMarc, pMarc);
	}

	ids = m_pMarcMgr->GetItem(pMarc, _T("001*"), strItem);
	strItem.TrimLeft();
	if (ids >= 0 && !strItem.IsEmpty())
	{
		if (strItem.GetLength() == 7)
		{
			TCHAR szTemp[4];
			_stscanf(strItem, _T("%3s%4s"), szTemp, strYear.GetBuffer(5));
			strYear.ReleaseBuffer();
		}
		else
		{
			pDM_Species->SetCellData( _T("SS_�����ȣ"), strItem, nIdx);
			return 0;
		}
	}

	ids = pDM_Species->GetCellData( _T("SS_�����ڷᱸ��_�ڵ�"), nIdx, strControlCode);
	if (ids < 0 || strControlCode.IsEmpty()) 
	{
		m_strLastErrMsg = _T("�����ڷᱸ��_�ڵ尡 ���� �ڷ��Դϴ�.");
		return -1;
	}

	if (pDM_Book)
	{
		nRowCount = pDM_Book->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			ids = pDM_Book->GetCellData( _T("SB_��Ϲ�ȣ"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty()) 
			{
				m_strLastErrMsg = _T("��Ϲ�ȣ�� �ο����� ���� å�� �ֽ��ϴ�.");
				return -1;
			}			

			ids = pDM_Book->GetCellData( _T("SB_�谡��ġ��ȣ"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty())
			{
				m_strLastErrMsg = _T("�谡�� �ڵ尡 �ο����� ���� å�� �ֽ��ϴ�.");
				return -1;
			}
		}
	}

	if (bCommit) {
		ids = pDM_Out->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
			return -1;
		}
	}

	CString strReckey;
	CString strControlNo;
	INT iControlNoCount = 0;

	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format( _T("%04d"), time.GetYear() );

	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					_T("WHERE MAKE_YEAR='%s' AND DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
					, strYear, strControlCode );

	ids = pDM_Out->GetOneValueQuery(strQuery, strItem);
	if (ids < 0)
	{
		strControlNo = _T("1");

		CString sControlNo;
		sControlNo.Format( _T("%s%s%05s"), strControlCode, strYear, strControlNo );
	
		strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL WHERE CONTROL_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
						, sControlNo);
	
		ids = pDM_Out->GetOneValueQuery(strQuery, strItem);
		if ( strItem != _T("") ) {
			AfxMessageBox( _T("�����ȣ�� �ߺ��˴ϴ�. ��������ȣ�� �����ϰ� �ٽ� �ο��ϼ���.") );
			return -1;
		}

		ids = pDM_Out->MakeRecKey(strReckey);
		if (ids < 0) return ids;

		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ")
						_T("(REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, ")
						_T("LAST_NUMBER, FIRST_WORK, LAST_WORK, MANAGE_CODE) ")
						_T("VALUES('%s','CO','CON_NO','%s','%s','%s', '%s', '%s', UDF_MANAGE_CODE);")
						, strReckey, strControlCode, strYear, strControlNo
						, m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG)
						, m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG)	);
	}
	else
	{
		if ( iControlNoCount < 0 ) {
			iControlNoCount = 0;
		}
		CESL_DataMgr *pDmControlNo = m_pParnetMgr->FindDM( _T("DM_����_���_���") );
		INT iControlCount = pDmControlNo->GetRowCount();
		if ( iControlCount == 0 ) {
			pDmControlNo->InsertRow(-1);
			pDmControlNo->SetCellData( _T("�۾�����"), strItem, 0 );
		}
		else {
			CString sControl = pDmControlNo->GetCellData( _T("�۾�����"), 0 );
			if ( sControl != strItem ) {
				iControlNoCount = 0;
			}
		}

		strControlNo.Format(_T("%d"), _ttoi(strItem) + 1 + iControlNoCount);		

		CString sControlNo;
		sControlNo.Format( _T("%s%s%05s"), strControlCode, strYear, strControlNo );

		strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL WHERE CONTROL_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
						, sControlNo );
		
		ids = pDM_Out->GetOneValueQuery(strQuery, strItem);
		if ( strItem != _T("") ) {
			AfxMessageBox( _T("�����ȣ�� �ߺ��˴ϴ�. ��������ȣ�� �����ϰ� �ٽ� �ο��ϼ���.") );
			return -1;
		}
		
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%s, LAST_WORK='%s' ")
						_T("WHERE DIVISION_VALUE='%s' AND MAKE_YEAR='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;")
						, strControlNo, m_pParnetMgr->GetWorkLogMsg(_T("Marc����"),WORK_LOG)
						, strControlCode, strYear );

		iControlNoCount++;
	}

	ids = pDM_Out->AddFrame(strQuery);
	if ( 0 > ids ) {
		AfxMessageBox( _T("��������ȣ ������Ʈ AddFrame() Ȯ��!") );
		return -1;
	}

	strItem = strControlNo;

	m_pMarcMgr->DeleteField(pMarc, _T("001"));
	
	strControlNo.Format(_T("%s%s%05s"), strControlCode, strYear, strItem);
	ids = m_pMarcMgr->InsertField(pMarc, _T("001") + strControlNo);
	m_pMarcMgr->Encoding(pMarc, strStreamMarc);


	pDM_Out->InitDBFieldData();
	pDM_Out->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, 1);
	pDM_Out->AddDBFieldData(_T("CONTROL_NO"), _T("STRING"), strControlNo, 1);
	pDM_Out->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParnetMgr->GetWorkLogMsg(_T("�����Ϸ�"),WORK_LOG), 1);


	ids = pDM_Out->MakeUpdateFrame( _T("SE_SPECIES_TBL"), _T("REC_KEY"), _T("STRING"), strSpeciesKey );
	if ( ids < 0 ) {
		AfxMessageBox( _T("CatApi MakeUpdateFrame 1 Ȯ��") );
		return -1;
	}	

	strQuery.Format(_T("UPDATE IDX_SE_TBL SET CONTROL_NO = '%s' WHERE REC_KEY = '%s';")
		, strControlNo, strSpeciesKey);
	
	ids = pDM_Out->AddFrame(strQuery);
	if (ids < 0) return ids;

	ids = pDM_Species->SetCellData( _T("SS_�����ȣ"), strControlNo, nIdx);
	ids = pDM_Species->SetCellData( _T("SS_MARC"), strStreamMarc, nIdx);

	if (pDM_Index)
		ids = pDM_Index->SetCellData( _T("ISS_�����ȣ"), strControlNo, nIdx);

	if (pDM_Book)
	{
		nRowCount = pDM_Book->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			ids = pDM_Book->GetCellData( _T("SB_åKEY"), idx, strBookKey);
			if (ids < 0 || strBookKey.IsEmpty()) continue;

			strQuery.Format(_T("UPDATE %s SET WORKING_STATUS = 'BOC114O', LAST_WORK = '%s' WHERE REC_KEY = %s;"), pDM_Book->TBL_NAME, m_pParnetMgr->GetWorkLogMsg(_T("Marc����"),WORK_LOG), strBookKey);
			ids = pDM_Out->AddFrame(strQuery);
			if (ids < 0) return ids;
		}
	}


	if (bCommit)
	{
		ids = pDM_Out->SendFrame();
		if (ids < 0) return ids;

		ids = pDM_Out->EndFrame();
		if (ids < 0) return ids;
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::UpdateSpecies(CESL_DataMgr *pDM_Species, 
							  CESL_DataMgr *pDM_Index, 
							  CESL_DataMgr *pDM_Book, 
							  CESL_DataMgr *pDM_Volume, 
							  CESL_DataMgr *pDM_Appendix, 
							  CESL_DataMgr *pDM_HoldBook, 
							  CESL_DataMgr *pDM_HoldAppendix, 
							  CStringArray *pArrStrAddFrame /*= NULL*/)
{
	EFS_BEGIN

	if (pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL ||
		pDM_Volume == NULL ||
		pDM_Appendix == NULL ||
		pDM_HoldBook == NULL ||
		pDM_HoldAppendix == NULL
		) return -1;

	INT ids = -1;
	CString strItem;

	CArray <CString, CString> RemoveAliasList;

	ids = pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return -1;
	}

	CString strSpeciesKey;
	ids = pDM_Species->GetCellData(_T("SS_��KEY"), 0, strSpeciesKey);
	
	CopyDM(pDM_Index, 0, pDM_Species, 0, _T("SS"));

	ids = ApplyDefaultValue(pDM_Species, pDM_Index, pDM_Book);
	if (ids < 0) return ids;

	pDM_Species->GetCellData( _T("SS_�̿����ѱ���_�ڵ�"), 0, strItem);
	GetSpeciesUseLimitCode(pDM_Book, strItem, _T("SE"), 1, strItem);
	pDM_Species->SetCellData( _T("SS_�̿����ѱ���_�ڵ�"), strItem, 0);

	pDM_Species->SetCellData(_T("SS_�������۾�"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), 0);

	RemoveAliasList.Add( _T("SS_��KEY") );

	ids = ConvertBookTo049(pDM_Species, pDM_Book, MNG_MODIFY);
	if (ids >= 0)
	{
		CString strHoldData = pDM_Species->GetCellData( _T("SS_��������"), 0);
		if ( !strHoldData.IsEmpty() )
		{
			CString strQuery;
			CString strHoldInfoKey = pDM_Species->GetCellData( _T("SS_��������KEY"), 0);
			if (!strHoldInfoKey.IsEmpty()) 
			{
				strQuery.Format(_T("UPDATE SE_HOLDING_TBL SET HOLDING_DATA='%s' WHERE REC_KEY=%s"), strHoldData, strHoldInfoKey);

				TCHAR  *pFieldName[] = {_T("HOLDING_DATA")};
				TCHAR  *pFieldData[1];
				INT	  InDataType[] = {STRING};

			    pFieldData[0] = 	strHoldData.GetBuffer(0);
				ids = pDM_Species->ESL_UpdateProc(_T("SE_HOLDING_TBL"),
											1				,						     
											pFieldName     ,
											pFieldData     ,
											InDataType    ,
											_T("REC_KEY"),
											strHoldInfoKey.GetBuffer(0));			
			}
			else 
			{
				ids = pDM_Species->MakeRecKey(strHoldInfoKey);
				if (ids < 0) return ids;

				pDM_Species->InitDBFieldData();
				pDM_Species->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strHoldInfoKey, TRUE);
				pDM_Species->AddDBFieldData(_T("HOLDING_DATA"), _T("STRING"), strHoldData, TRUE);
				ids = pDM_Species->MakeInsertFrame(_T("SE_HOLDING_TBL"));
				if (ids < 0) return ids;
			}
		}
	}

	pDM_Species->InitDBFieldData();
	pDM_Species->MakeQueryByDM(RemoveAliasList, pDM_Species, pDM_Species->TBL_NAME, 0, pDM_Species, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);

	ids = ApplyIndex(pDM_Species, pDM_Index, pDM_Book);

	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add( _T("ISS_REC_KEY") );

	pDM_Species->InitDBFieldData();
	pDM_Species->MakeQueryByDM(RemoveAliasList, pDM_Index, pDM_Index->TBL_NAME, 0, pDM_Species, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);

	CStringArray Index_classfycodes;
	Index_classfycodes.Add( _T("ISS_�̿����ѱ���_�ڵ�"));
	Index_classfycodes.Add( _T("ISS_��������_�ڵ�"));
	Index_classfycodes.Add( _T("ISS_�ڷ��ִ°�INFO_�ڵ�"));
	Index_classfycodes.Add( _T("ISS_��ϱ���_�ڵ�"));
	CStringArray Book_classfycodes;
	Book_classfycodes.Add( _T("SB_�̿����ѱ���_�ڵ�"));
	Book_classfycodes.Add( _T("SB_��������_�ڵ�"));
	Book_classfycodes.Add( _T("SB_�谡��ġ��ȣ"));
	Book_classfycodes.Add( _T("SB_��ϱ���_�ڵ�"));
	CString Index_Update_sql;
	MakeIndexClassifyCodeConcatenationSQL(pDM_Index, pDM_Book, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpeciesKey, _T("SB"));
	ids = pDM_Species->AddFrame(Index_Update_sql);
	if (ids < 0) return ids;

	ids = MakeVolumeQueryFrame(pDM_Volume, pDM_Book, pDM_HoldBook, pDM_Appendix, pDM_HoldAppendix, strSpeciesKey, pDM_Species);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("��ȣ���� ��������� ����");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_Book, strSpeciesKey, _T("BOOK"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("å���� ��������� ����");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_HoldBook, strSpeciesKey, _T("HOLD_BOOK"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("����å ��������� ����");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_Appendix, strSpeciesKey, _T("APPENDIX"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("�η����� ��������� ����");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_HoldAppendix, strSpeciesKey, _T("HOLD_APPENDIX"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("����η����� ��������� ����");
		return ids;
	}

	ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);

	ids = pDM_Species->SendFrame();
	if ( 0 > ids ) return -1;
	ids = pDM_Species->EndFrame();	
	if (ids < 0)
	{
		m_strLastErrMsg = _T("DB�� ��ũ�� �Է��ϴµ� ������ �߻��߽��ϴ�.");
		return ids;
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ConvertBookTo049(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Book, INT nAppMode )
{
	EFS_BEGIN

	if (!pDM_Species || !pDM_Book) return -1;

	CESL_DataMgr dm;
	INT ids;
	dm.SetCONNECTION_INFO(pDM_Book->CONNECTION_INFO);
	dm.SetMarcMgr(pDM_Book->m_pMarcMgr);
	dm.InitDMPKFromFile(_T("DM_����_����DM_å����"));

	if (nAppMode == MNG_MODIFY)
	{
		CString strSpeciesKey = pDM_Species->GetCellData(_T("SS_��KEY"), 0);
		if (strSpeciesKey.IsEmpty()) return -1;

		CString strOption;
		strOption.Format(_T(" ((ASCII(SUBSTR(REG_NO,0,1)) < 48) OR (ASCII(SUBSTR(REG_NO,0,1)) > 57))"
			_T(" AND (SUBSTR(WORKING_STATUS,0,3) NOT IN ('SER'))")
			_T(" AND SPECIES_KEY = %s ")), strSpeciesKey);
		ids = dm.RefreshDataManager(strOption);
		if (ids < 0) return ids;
	}

	ids = MergeDM(pDM_Book, &dm, _T("SB_åKEY"), 1);
	if (ids < 0) return ids;

	CString str049;
	CStringArray arrStr049;

	CString strRegNo;
	CString strDupCode;
	CString strVolCode;
	CString strSepCode;

	INT nRowCount = dm.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		str049.Empty();
		
		strRegNo = dm.GetCellData( _T("SB_��Ϲ�ȣ"), idx);
		strDupCode = dm.GetCellData( _T("SB_������ȣ"), idx);
		strVolCode = dm.GetCellData( _T("SB_��å��ȣ"), idx);
		strSepCode = dm.GetCellData( _T("SB_��ġ��ȣ_�ڵ�"), idx);

		if (strRegNo.IsEmpty()) continue;
				
		str049.Format(_T("%cl%s"), SUBFIELD_CODE, strRegNo);
		if (!strVolCode.IsEmpty())
		{
			CString strTmp = str049;
			str049.Format(_T("%s%cv%s"), strTmp, SUBFIELD_CODE, strVolCode);
		}
		if (!strDupCode.IsEmpty())
		{
			CString strTmp = str049;
			str049.Format(_T("%s%cc%s"), strTmp, SUBFIELD_CODE, strDupCode);
		}
		if (!strSepCode.IsEmpty())
		{
			CString strTmp = str049;
			str049.Format(_T("%s%cf%s"), strTmp, SUBFIELD_CODE, strSepCode);
		}


		arrStr049.Add(str049);		
	}

	str049 = _T("0491 ");
	nRowCount = arrStr049.GetSize();
	for (idx = 0; idx < nRowCount; idx++)
	{
		str049 += arrStr049.GetAt(idx);
	}
	
	CMarc marc;
	ids = m_pMarcMgr->InsertField(&marc, str049);
	if (ids < 0) return ids;

	CString strStreamMarc;
	ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	if (ids < 0) return ids;

	pDM_Species->SetCellData( _T("SS_��������"), strStreamMarc, 0);

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ImportSpecies(CESL_DataMgr *pDM_Species, 
								CESL_DataMgr *pDM_Index, 
								CESL_DataMgr *pDM_Book, 
								CESL_DataMgr *pDM_Volume, 
								CESL_DataMgr *pDM_Appendix, 
								CESL_DataMgr *pDM_HoldBook, 
								CESL_DataMgr *pDM_HoldAppendix, 
								CStringArray *pArrStrAddFrame /*= NULL*/, 
								LPCTSTR lpszShelfDate /*= NULL*/,
								INT nAppMode)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book ||
		!pDM_Volume ||
		!pDM_Appendix ||
		!pDM_HoldBook ||
		!pDM_HoldAppendix ) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;

	CString strRegNo;
	CString strQuery;
	CString strResult;
	CString strTemp;

	CESL_DataMgr *DM_LastNumber = NULL;

	if ( nAppMode == MNG_IMPORT || nAppMode == MNG_IMPORT_WORKING )
	{
		DM_LastNumber = m_pParnetMgr->FindDM( _T("DM_��ũ����_��������ȣ") );
		
		CString sLastNumberQuery = _T("") ;

		sLastNumberQuery.Format(_T("SELECT DIVISION_VALUE, LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
								_T("WHERE KIND_CODE='REG_NO' AND MANAGE_CODE=UDF_MANAGE_CODE") );

		DM_LastNumber->RestructDataManager(sLastNumberQuery);

		nRowCount = pDM_Book->GetRowCount();
	}

	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;

	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("SB"))) continue;
		
		ids = pDM_Book->GetCellData( _T("SB_��Ϲ�ȣ"), idx, strRegNo);
		if (ids < 0 || strRegNo.IsEmpty()) continue;

		if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
			if ( strRegNo.GetLength() > nRegCodeLength ) {
				strRegNo = m_pParnetMgr->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
			}
		}

		pDM_Book->SetCellData( _T("SB_��Ϲ�ȣ"), strRegNo, idx );

		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
						, pDM_Book->TBL_NAME, strRegNo );

		ids = pDM_Species->GetOneValueQuery(strQuery, strResult);

		if (ids >= 0 && !strResult.IsEmpty())
		{
			m_strLastErrMsg.Format(_T("��Ϲ�ȣ�� �ߺ��˴ϴ�. [%s]"), strRegNo);
			return -1;
		}
	}
	
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("SS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	ids = ApplyShelfCodeData(pDM_Book, &marc, FALSE);
	if (ids < 0) return -1;

	CString strLibCode;
	GetLibInfo(&CString(_T("LIB_CODE")), &strLibCode, 1);
	if ( strLibCode.IsEmpty() )
	{
		m_strLastErrMsg.Format(_T("������ ��ȣ�� �����ϴ�. �������ּ���"));
		return -1;
	}
	ApplyLastModifyData(&marc);


	ids = pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return -1;
	}

	ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("SS_MARC"), strStreamMarc, 0);
	ids = pDM_Species->SetCellData( _T("SS_�������ڷ�"), _T("Y"), 0 );
	
	ids = pDM_Species->SetCellData( _T("SS_�۾����±���"), _T("1"), 0 );

	ids = InsertSpecies(pDM_Species, pDM_Index, pDM_Book, pDM_Volume, pDM_Appendix, pDM_HoldBook, pDM_HoldAppendix, pArrStrAddFrame, FALSE);
	if (ids < 0) return -1;

	ids = CompleteCataloging(pDM_Species, pDM_Species, 0, pDM_Book, FALSE);
	if (ids < 0) return -1;

	CString strShelfDate;
	if (lpszShelfDate == NULL)
	{
		strShelfDate = _T("1200/01/01");
	}
	else
		strShelfDate = lpszShelfDate;		

	CStringArray arrSpeciesKey;
	CStringArray arrBookKey;
	CStringArray arrAppendixKey;

	arrSpeciesKey.Add(pDM_Species->GetCellData( _T("SS_��KEY"), 0));
	
	nRowCount = pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("SB"))) continue;

		arrBookKey.Add(pDM_Book->GetCellData( _T("SB_åKEY"), idx));
	}
	if(pDM_Appendix)
	{
		nRowCount = pDM_Appendix->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			arrAppendixKey.Add(pDM_Appendix->GetCellData( _T("SB_åKEY"), idx));
		}
	}

	ids = RecordLocDate(pDM_Species, &arrSpeciesKey, &arrBookKey, &arrAppendixKey, strShelfDate, FALSE);
	if (ids < 0) return -1;

	ids = pDM_Species->SendFrame();
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("DB�� ������ ���� ����");
		return ids;
	}

	ids = pDM_Species->EndFrame();
	
	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::InsertSpecies(CESL_DataMgr *pDM_Species, 
								CESL_DataMgr *pDM_Index, 
								CESL_DataMgr *pDM_Book, 
								CESL_DataMgr *pDM_Volume, 
								CESL_DataMgr *pDM_Appendix, 
								CESL_DataMgr *pDM_HoldBook, 
								CESL_DataMgr *pDM_HoldAppendix, 
								CStringArray *pArrStrAddFrame /*= NULL*/, 
								BOOL bCommit /*= TRUE*/)
{
	EFS_BEGIN

	if (pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL ||
		pDM_Volume == NULL ||
		pDM_Appendix == NULL ||
		pDM_HoldBook == NULL ||
		pDM_HoldAppendix == NULL
		) return -1;

	INT ids = -1;

	if (bCommit)
	{
		INT ids = pDM_Species->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
			return -1;
		}
	}

	CString strSpeciesKey;
	CArray<CString, CString> arrRemoveAliasList;

	CopyDM(pDM_Index, 0, pDM_Species, 0,  _T("SS") );

	ids = ApplyDefaultValue(pDM_Species, pDM_Index, pDM_Book);
	if (ids < 0) return ids;

	arrRemoveAliasList.Add( _T("SS_MARC"));

	ids = pDM_Species->MakeRecKey(strSpeciesKey);
	if (ids < 0) return ids;
	pDM_Species->SetCellData( _T("SS_��KEY"), strSpeciesKey, 0);

	ids = ConvertBookTo049(pDM_Species, pDM_Book, MNG_INSERT);
	if (ids >= 0)
	{
		CString strHoldData = pDM_Species->GetCellData( _T("SS_��������"), 0);
		CString strQuery;
		CString strHoldInfoKey;
		ids = pDM_Species->MakeRecKey(strHoldInfoKey);
		if (ids < 0) return ids;
		
		pDM_Species->InitDBFieldData();
		pDM_Species->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strHoldInfoKey, TRUE);
		pDM_Species->AddDBFieldData(_T("HOLDING_DATA"), _T("STRING"), strHoldData, TRUE);
		ids = pDM_Species->MakeInsertFrame(_T("SE_HOLDING_TBL"));

		if (ids < 0) return ids;
	}

	pDM_Species->SetCellData(_T("SS_�����۾�"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), 0);
	pDM_Species->SetCellData(_T("SS_�������۾�"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), 0);

	pDM_Species->InitDBFieldData();
	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Species, pDM_Species->TBL_NAME, 0, pDM_Species);
	LogToFile(pDM_Species->m_lastmsg);

	pDM_Index->SetCellData( _T("ISS_REC_KEY"), strSpeciesKey, 0);
	pDM_Index->SetCellData( _T("ISS_�����а�"), _T("1"), 0);

	arrRemoveAliasList.RemoveAll();
	ids = ApplyIndex(pDM_Species, pDM_Index, pDM_Book);
	
	pDM_Species->InitDBFieldData();
	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Index, pDM_Index->TBL_NAME, 0, pDM_Species);
	LogToFile(pDM_Species->m_lastmsg);

	CStringArray Index_classfycodes;
	Index_classfycodes.Add( _T("ISS_�̿����ѱ���_�ڵ�"));
	Index_classfycodes.Add( _T("ISS_��������_�ڵ�"));
	Index_classfycodes.Add( _T("ISS_�ڷ��ִ°�INFO_�ڵ�"));
	Index_classfycodes.Add( _T("ISS_��ϱ���_�ڵ�"));
	CStringArray Book_classfycodes;
	Book_classfycodes.Add( _T("SB_�̿����ѱ���_�ڵ�"));
	Book_classfycodes.Add( _T("SB_��������_�ڵ�"));
	Book_classfycodes.Add( _T("SB_�谡��ġ��ȣ"));
	Book_classfycodes.Add( _T("SB_��ϱ���_�ڵ�"));
	CString Index_Update_sql;
	MakeIndexClassifyCodeConcatenationSQL(pDM_Index, pDM_Book, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpeciesKey, _T("SB"));
	ids = pDM_Species->AddFrame(Index_Update_sql);
	if (ids < 0) return ids;

	ids = MakeVolumeQueryFrame(pDM_Volume, pDM_Book, pDM_HoldBook, pDM_Appendix, pDM_HoldAppendix, strSpeciesKey, pDM_Species);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("��ȣ���� ��������� ����");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_Book, strSpeciesKey, _T("BOOK"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("å���� ��������� ����");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_HoldBook, strSpeciesKey, _T("HOLD_BOOK"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("����å ��������� ����");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_Appendix, strSpeciesKey, _T("APPENDIX"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("�η����� ��������� ����");
		return ids;
	}

	ids = MakeBookQueryFrame(pDM_HoldAppendix, strSpeciesKey, _T("HOLD_APPENDIX"), pDM_Species);
	if (ids < 0) 
	{
		m_strLastErrMsg = _T("����η����� ��������� ����");
		return ids;
	}

	ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);
	
	CString strStreamMarc;
	pDM_Species->GetCellData( _T("SS_MARC"), 0, strStreamMarc);
	pDM_Species->InitDBFieldData();
	pDM_Species->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, TRUE);
	pDM_Species->MakeUpdateFrame(_T("SE_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);

	if (bCommit)
	{
		ids = pDM_Species->SendFrame();
		pDM_Species->EndFrame();
		if (ids < 0) 
		{
			m_strLastErrMsg = _T("DB�� ���� �Է��ϴµ� ������ �߻��߽��ϴ�.");
			return ids;
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::MakeRemoveListByHD( CESL_DataMgr *pDM_Book, CString strRemoveFieldHD, CArray <CString, CString> &RemoveAliasList )
{
	INT i;
	CString strFieldAlias, strFieldName, strFieldType, strInitValue, strDataType;

	for (i=0; i<pDM_Book->GetRefColumnCount(); i++)
	{
		strFieldAlias = _T("");
		pDM_Book->GetColumnRefInfo(i, strFieldAlias, strFieldName, strFieldType, strInitValue, strDataType);
		if (strFieldAlias.Find( strRemoveFieldHD ) >= 0)
			RemoveAliasList.Add( strFieldAlias );
	}
	return 0;
}

INT CCatApi_SE::MakeBookQueryFrame(CESL_DataMgr *pDM_Book, CString strSpeciesKey, CString strType,
								   CESL_DataMgr *pDM_OUT, INT nSendCount/*=-1*/)
{
	EFS_BEGIN

	INT ids = -1;
	INT idx = -1;
	
	CString strRegNo;
	CString strQuery;
	CString strResult;
	CString strBookKey;
	CString strQueryFlag;

	CIndex *pIndex = m_pIndexTool->GetIndex();

	CArray <CString, CString> RemoveAliasList;

	CSeApi api(m_pParnetMgr);
	CString strBOOK_KEY, strACQ_TYPE_VOL_KEY, strVOL_KEY;
	CString strSPECIES_KEY, strBOOK_TYPE, strBooktitle, msg;

	INT nRowCount = pDM_Book->GetRowCount();

	CString strIsRepresentBook;
	CString strRegNoList = _T("");
	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;
	for (idx = 0; idx < nRowCount; idx++) 
	{
		ids = pDM_Book->GetCellData( _T("SB_DB_FLAG"), idx, strQueryFlag);
		if (ids < 0) continue;

		if (strQueryFlag == _T("I"))
		{
			ids = pDM_Book->GetCellData( _T("SB_��Ϲ�ȣ"), idx, strRegNo);
            if( ids < 0 || strRegNo.IsEmpty ())
			{
				AfxMessageBox(_T("��Ϲ�ȣ�� �ο����� ���� å�� �ֽ��ϴ�.\r\n��Ϲ�ȣ�� Ȯ���Ͽ� �ֽʽÿ�"));
				return -1;
			}

			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
				if ( strRegNo.GetLength() > nRegCodeLength ) {
					strRegNo = m_pParnetMgr->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
				}
			}
			
			if ( strRegNoList.GetLength() > 0 ) strRegNoList += _T(", '") + strRegNo + _T("'");
			else strRegNoList.Format(_T("'%s'"), strRegNo);
		}
	}
	
	if (!strRegNoList.IsEmpty())
	{
		strQuery.Format(_T("SELECT REC_KEY FROM SE_BOOK_TBL WHERE REG_NO IN (%s) AND MANAGE_CODE=UDF_MANAGE_CODE")
						, strRegNoList);
		ids = pDM_OUT->GetOneValueQuery(strQuery, strResult);
		if (ids >= 0 && !strResult.IsEmpty())
		{
			AfxMessageBox(_T("��Ϲ�ȣ�� �ߺ��˴ϴ�. [") + strRegNo + _T("]") );
			return -1;
		}
	}

	for (idx = 0; idx < nRowCount; idx++) {
		RemoveAliasList.RemoveAll();
		ids = pDM_Book->GetCellData(_T("SB_DB_FLAG"), idx, strQueryFlag);
		if (ids < 0) continue;

		CString sPrice;
		CString sRealPrice;
		ids = pDM_Book->GetCellData(_T("SV_����"), idx, sPrice);
		if ( sPrice != _T("") ) {
			for( INT i = 0 ; i < sPrice.GetLength() ; i++ ) {
				if ( sPrice.Mid( i, 1 ) >= _T("0") && sPrice.Mid( i, 1 ) <= _T("9") ) {
					sRealPrice = sRealPrice + sPrice.Mid( i, 1 );
				}
			}
			ids = pDM_Book->SetCellData(_T("SV_����"), sRealPrice, idx);
		}

		if (strQueryFlag == _T("I")) 
		{
			pDM_Book->SetCellData(_T("SB_��KEY"), strSpeciesKey, idx);
			pDM_Book->SetCellData(_T("SB_�����۾�"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), idx);
			pDM_Book->SetCellData(_T("SB_�������۾�"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), idx);
			if (strType == _T("BOOK"))
			{
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);
			}
			else if (strType == _T("HOLD_BOOK"))
			{
				MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);
			}
			else if (strType == _T("APPENDIX"))
			{
				MakeRemoveListByHD( pDM_Book, _T("SA"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);

				pDM_Book->SetCellData(_T("SA_�����۾�"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), idx);
				pDM_Book->SetCellData(_T("SA_�������۾�"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), idx);
				RemoveAliasList.RemoveAll();
				MakeRemoveListByHD( pDM_Book, _T("SB"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_APPENDIX_BOOK_TBL AP"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);
			}
			else if (strType == _T("HOLD_APPENDIX"))
			{
				MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );
				MakeRemoveListByHD( pDM_Book, _T("SA"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);

				pDM_Book->SetCellData(_T("SA_�����۾�"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), idx);
				pDM_Book->SetCellData(_T("SA_�������۾�"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), idx);
				RemoveAliasList.RemoveAll();
				MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );
				MakeRemoveListByHD( pDM_Book, _T("SB"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_APPENDIX_BOOK_TBL AP"), idx, pDM_OUT);
				LogToFile(pDM_OUT->m_lastmsg);
			}
			
		}
		else if (strQueryFlag == _T("U")) 
		{
			RemoveAliasList.Add(_T("SB_åKEY"));
			ids = pDM_Book->GetCellData(_T("SB_åKEY"), idx, strBookKey);
			pDM_Book->SetCellData(_T("SB_�������۾�"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), idx);
			
			if (strType == _T("BOOK") || strType == _T("HOLD_BOOK"))
			{
				if( strType == _T("HOLD_BOOK") )
					MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );

				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBookKey);
				LogToFile(pDM_OUT->m_lastmsg);

				if( pDM_Book->GetCellData(_T("SB_å����"), idx) == _T("B") )
				{
					RemoveAliasList.Add(_T("SB_��Ͽ���KEY"));
					RemoveAliasList.Add(_T("SB_���Ա���_�ڵ�"));
					RemoveAliasList.Add(_T("SB_�Լ���"));
					RemoveAliasList.Add(_T("SB_���Ա��к���KEY"));
					RemoveAliasList.Add(_T("SB_���Գ⵵"));
					RemoveAliasList.Add(_T("SB_����å����"));
					RemoveAliasList.Add(_T("SB_����åKEY"));
					RemoveAliasList.Add(_T("SB_�����ΰ���"));
					RemoveAliasList.Add(_T("SB_�弭����"));
					RemoveAliasList.Add(_T("SB_������ȣ"));
					RemoveAliasList.Add(_T("SB_å����"));
					RemoveAliasList.Add(_T("SB_�з���ȣ"));
					RemoveAliasList.Add(_T("SB_�з���ȣ����_�ڵ�"));
					RemoveAliasList.Add(_T("SB_������ȣ"));
					RemoveAliasList.Add(_T("SB_�ǽð��ΰ���"));
					RemoveAliasList.Add(_T("SB_�Է���"));
					RemoveAliasList.Add(_T("SB_���̺���±���"));
					RemoveAliasList.Add(_T("SB_������ȣ"));
					RemoveAliasList.Add(_T("SB_�����Ϸù�ȣ"));
					RemoveAliasList.Add(_T("SB_��ϱ���_�ڵ�"));
					RemoveAliasList.Add(_T("SB_��Ϲ�ȣ"));
					RemoveAliasList.Add(_T("SB_����ΰ���"));
					RemoveAliasList.Add(_T("SB_���"));
					RemoveAliasList.Add(_T("SB_��ġ��ȣ_�ڵ�"));
					RemoveAliasList.Add(_T("SB_��KEY"));
					RemoveAliasList.Add(_T("SB_��å��ȣ"));
					RemoveAliasList.Add(_T("SB_��KEY"));
					RemoveAliasList.Add(_T("SB_�ΰ迩��"));

					pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT, _T("BINDING_BOOK_KEY"), _T("NUMERIC"), strBookKey);
					LogToFile(pDM_OUT->m_lastmsg);
				}
			}
			else if (strType == _T("APPENDIX") || strType == _T("HOLD_APPENDIX"))
			{
				if( strType == _T("HOLD_APPENDIX") )
					MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );

				MakeRemoveListByHD( pDM_Book, _T("SA"), RemoveAliasList );
				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_BOOK_TBL b"), idx, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBookKey);
				LogToFile(pDM_OUT->m_lastmsg);

				pDM_Book->SetCellData(_T("SA_�������۾�"), m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG), idx);
				RemoveAliasList.RemoveAll();
				ids = pDM_Book->GetCellData(_T("SA_�η�KEY"), idx, strBookKey);
				MakeRemoveListByHD( pDM_Book, _T("SB"), RemoveAliasList );

				if( strType == _T("HOLD_APPENDIX") )
					MakeRemoveListByHD( pDM_Book, _T("SV"), RemoveAliasList );

				pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Book, _T("SE_APPENDIX_BOOK_TBL AP"), idx, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBookKey);
				LogToFile(pDM_OUT->m_lastmsg);
			}
		}
		else if (strQueryFlag == _T("D")) 
		{
			strBOOK_KEY = pDM_Book->GetCellData(_T("SB_åKEY"),idx);
			strACQ_TYPE_VOL_KEY = pDM_Book->GetCellData(_T("SB_���Ա��к���KEY"),idx);
			strVOL_KEY = pDM_Book->GetCellData(_T("SB_��KEY"),idx);
			strSPECIES_KEY = pDM_Book->GetCellData(_T("SB_��KEY"),idx);
			strBOOK_TYPE = pDM_Book->GetCellData(_T("SB_å����"),idx);

			if (strType == _T("BOOK"))
				strBooktitle = pDM_Book->GetCellData(_T("SV_��ȣ��"),idx);
			else if (strType == _T("APPENDIX"))
				strBooktitle = pDM_Book->GetCellData(_T("SA_�ηϸ�"),idx);

			ids = api.BOOK_CAT_DELETE(pDM_OUT,
							strBOOK_KEY,strACQ_TYPE_VOL_KEY,strVOL_KEY,
							strSPECIES_KEY,strBOOK_TYPE,strBooktitle,TRUE, FALSE);
			if(ids<0)
			{
				if(-1002 == ids) 
				{
					msg.Format(_T("[ %s ] �ڷḦ ������ �� �����ϴ�!"), strBooktitle);
					AfxMessageBox(msg);
					continue;
				}
				else
				{
					msg.Format(_T("[ %s ] �ڷḦ �����ϴ� �� ������ �߻��� �۾��� �ߴ��մϴ�!"), strBooktitle);
					AfxMessageBox(msg);
					return -200;
				}
			}

			CString strRegNo;
			ids = pDM_Book->GetCellData(_T("SB_��Ϲ�ȣ"), idx, strRegNo);
			if ( strRegNo != _T("")){
				INT iRegNumber;
				iRegNumber = _ttoi(strRegNo.Mid(3, strRegNo.GetLength()));
			
				CString strRegCode;
				pDM_Book->GetCellData(_T("SB_��ϱ���_�ڵ�"), idx, strRegCode);

				CString strRecKey;
				ids = pDM_OUT->MakeRecKey(strRecKey);
				if ( 0 > ids ) return -1;

				strQuery.Format(_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
								_T("(REC_KEY, PUBLISH_FORM_CODE, REG_CODE, REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE) ")
								_T("VALUES (%s, '%s', '%s', %d, '%s', '%s', UDF_MANAGE_CODE);")
								, strRecKey, _T("MO"), strRegCode, iRegNumber
								, m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG)
								, m_pParnetMgr->GetWorkLogMsg(_T("Marc�Է�"),WORK_LOG)	);

				ids = pDM_OUT->AddFrame(strQuery);
				if ( 0 > ids ) return -1;
			}
			LogToFile(pDM_OUT->m_lastmsg);
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::MakeMarcAdjustDM(CESL_DataMgr *pDM_From_Species, INT nFromSpeciesRow, CESL_DataMgr *pDM_From_Index, INT nFromIndexRow, CESL_DataMgr *pDM_To_Species, CESL_DataMgr *pDM_To_Index, CESL_DataMgr *pDM_To_Book, CESL_DataMgr *pDM_To_Volume)
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

	pDM_To_Species->FreeData();
	pDM_To_Index->FreeData();

	pDM_To_Species->InsertRow(-1);
	pDM_To_Index->InsertRow(-1);

	INT ids = -1;
	CMarc marcSource;
	CMarc marcOutput;
	CString strItemData;
	CString strDMFieldAlias;
	CString strMarcAlias;
	CString strFirstInd;
	CString strDefaultShelfLocCode;

	CArray<CString, CString&> arrItemData;

	CopyDM(pDM_From_Species, nFromSpeciesRow, pDM_To_Species, 0);

	CopyDM(pDM_From_Index, nFromSpeciesRow, pDM_To_Index, 0);

	CString strStreamMarc;
	strStreamMarc = pDM_To_Species->GetCellData( _T("SS_MARC"), 0);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marcSource);

	if (ids < 0) 
	{
		m_strLastErrMsg = _T("Marc Decoding ����");
		return ids;
	}

	CMap<CString, LPCTSTR, CString, LPCTSTR> mapCatDMtoMarc;
	mapCatDMtoMarc.RemoveAll();
	mapCatDMtoMarc.SetAt(_T("ISS_�ǻ���"), _T("250$a"));
	mapCatDMtoMarc.SetAt(_T("ISS_������"), _T("260$a"));
	mapCatDMtoMarc.SetAt(_T("ISS_������"), _T("260$b"));
	mapCatDMtoMarc.SetAt(_T("ISS_�����"), _T("260$c"));

	POSITION pos = mapCatDMtoMarc.GetStartPosition();
	while (pos)
	{
		mapCatDMtoMarc.GetNextAssoc(pos, strDMFieldAlias, strMarcAlias);

		ids = m_pMarcMgr->GetItem(&marcSource, strMarcAlias, strItemData);
		if (ids < 0) continue;

		pDM_To_Index->SetCellData(strDMFieldAlias, strItemData, 0);	
	}

	POSITION posTag = NULL;
	POSITION posSubfield = NULL;

	CString strTitle;
	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;
	CString strSubfieldCode;
	posTag = marcSource.m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)marcSource.m_listTag.GetNext(posTag);
		if (pTag && pTag->GetTagCode() == _T("245"))
		{
			posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield)
			{
				pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (!pSubfield) continue;

				strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") ||
					strSubfieldCode == _T("$x") ||
					strSubfieldCode == _T("$b"))
				{
					strTitle += pSubfield->GetPunctuationMark() + pSubfield->GetItemData();
				}
			}
		}		
	}
	
	pDM_To_Index->SetCellData(_T("ISS_��ǥ��"), strTitle, 0);
	
	m_mapMarcAlias.Lookup(_T("����"), strMarcAlias);
	m_pMarcMgr->GetItem(&marcSource, strMarcAlias, strItemData);
	if (!strItemData.IsEmpty())
	{
		CString strAuthor = strItemData;
		arrItemData.RemoveAll();
		m_mapMarcAlias.Lookup(_T("������"), strMarcAlias);
		ids = m_pMarcMgr->GetItem(&marcSource, strMarcAlias, strItemData, &arrItemData);
		if (ids >= 0)
		{
			INT nCount = arrItemData.GetSize();
			for (INT idx = 0; idx < nCount; idx++)
			{
				strItemData = arrItemData.GetAt(idx);
				if (strItemData.IsEmpty()) continue;

				strAuthor += _T(";") + strItemData;
			}
		}

		pDM_To_Index->SetCellData(_T("ISS_������"), strAuthor, 0);
	}

	CString strPublisher;
	arrItemData.RemoveAll();

	m_mapMarcAlias.Lookup(_T("������"), strMarcAlias);
	m_pMarcMgr->GetItem(&marcSource, strMarcAlias, strItemData);
	if (!strItemData.IsEmpty())
	{
		strPublisher = strItemData;
		INT nCount = arrItemData.GetSize();
		for (INT idx = 1; idx < nCount; idx++)
		{
			strItemData = arrItemData.GetAt(idx);
			if (strItemData.IsEmpty()) continue;

			strPublisher += _T(":") + strItemData;
		}

		pDM_To_Index->SetCellData(_T("ISS_������"), strPublisher, 0);
	}

	ids = MakeTemp049Marc(&marcSource, &marcOutput, strFirstInd, strDefaultShelfLocCode);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("049 Tag ����� ����");
		return 0;
	}
	
	if (marcOutput.m_listTag.GetCount() < 0) return 0;

	pDM_To_Book->FreeData();
	pDM_To_Volume->FreeData();

	CMap<CString, LPCTSTR, CString, LPCTSTR> mapVolume;

	CString strUniqueKey;

	ids = pDM_To_Index->MakeRecKey(strUniqueKey);
	if (ids < 0) return ids;
	mapVolume.SetAt(_T(""), strUniqueKey);

	posTag = marcOutput.m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = reinterpret_cast<CTag*>(marcOutput.m_listTag.GetNext(posTag));
		if (!pTag) continue;

		posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield)
		{
			pSubfield = reinterpret_cast<CSubfield*>(pTag->m_listSubfield.GetNext(posSubfield));
			if (!pSubfield) continue;

			strSubfieldCode = pSubfield->GetSubfieldCode();
			strSubfieldCode.Replace(SUBFIELD_CODE, '$');

			if (strSubfieldCode == _T("%v"))
			{
				mapVolume.SetAt(pSubfield->GetItemData(), strUniqueKey);
				Sleep(10);
			}
		}
	}

	INT nDMIdx = 0;
	CString strVolTitle;
	pos = mapVolume.GetStartPosition();
	while (pos)
	{
		mapVolume.GetNextAssoc(pos, strVolTitle, strUniqueKey);
		
		pDM_To_Volume->InsertRow(-1);

		ids = pDM_To_Volume->SetCellData( _T("SV_��KEY"), strUniqueKey, nDMIdx);

		ids = pDM_To_Volume->SetCellData( _T("SV_��ȣ����"), _T("V"), nDMIdx);

		ids = m_pMarcMgr->GetItem(&marcSource, _T("245$a"), strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_������"), strItemData, nDMIdx);

		ids = pDM_To_Species->GetCellData( _T("SS_���ı���_�ڵ�"), nDMIdx, strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_���ı���_�ڵ�"), strItemData, nDMIdx);

		ids = pDM_To_Species->GetCellData( _T("SS_��ü����_�ڵ�"), nDMIdx, strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_��ü����_�ڵ�"), strItemData, nDMIdx);

		strItemData.Format(_T("%d"), (nDMIdx + 1) * 10);
		ids = pDM_To_Volume->SetCellData( _T("SV_���Ϸù�ȣ"), strItemData, nDMIdx);

		ids = pDM_To_Volume->SetCellData( _T("SV_��ȣ��"), strVolTitle, nDMIdx);

		GetIndexTool()->GetIndex()->MakeIndex(strVolTitle);
		ids = pDM_To_Volume->SetCellData( _T("SV_���α�ȣ��"), strVolTitle, nDMIdx);

		ids = m_pMarcMgr->GetItem(&marcSource, _T("300$a"), strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_�����"), strItemData, nDMIdx);
		ids = m_pMarcMgr->GetItem(&marcSource, _T("300$b"), strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_������Ư��"), strItemData, nDMIdx);
		ids = m_pMarcMgr->GetItem(&marcSource, _T("300$c"), strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_åũ��"), strItemData, nDMIdx);
		ids = m_pMarcMgr->GetItem(&marcSource, _T("300$e"), strItemData);
		ids = pDM_To_Volume->SetCellData( _T("SV_�����ڷ�"), strItemData, nDMIdx);
		ids = pDM_To_Volume->SetCellData( _T("SV_��ȣ����"), _T("1"), nDMIdx);
		ids = pDM_To_Volume->SetCellData( _T("SV_��������"), _T("N"), nDMIdx);
		ids = pDM_To_Volume->SetCellData( _T("SV_DB_FLAG"), _T("I"), nDMIdx);

		nDMIdx++;
	}

	nDMIdx = 0;
	posTag = marcOutput.m_listTag.GetHeadPosition();
	INT nRegCodeLength;
	nRegCodeLength = m_pParnetMgr->m_pInfo->nREG_CODE_LENGTH;

	while (posTag)
	{
		pTag = reinterpret_cast<CTag*>(marcOutput.m_listTag.GetNext(posTag));
		if (!pTag) continue;

		pDM_To_Book->InsertRow(-1);

		posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield)
		{
			pSubfield = reinterpret_cast<CSubfield*>(pTag->m_listSubfield.GetNext(posSubfield));
			if (!pSubfield) continue;

			strSubfieldCode = pSubfield->GetSubfieldCode();
			strSubfieldCode.Replace(SUBFIELD_CODE, '$');

			if (strSubfieldCode == _T("$v"))
			{
				strItemData = pSubfield->GetItemData();
				if (mapVolume.Lookup(strItemData, strUniqueKey))
				{
					pDM_To_Book->SetCellData( _T("SB_��KEY"), strUniqueKey, nDMIdx);
				}

				pDM_To_Book->SetCellData( _T("SB_��å��ȣ"), strItemData, nDMIdx);

			}
			else if (strSubfieldCode == _T("$l"))
			{
				strItemData = pSubfield->GetItemData();

				pDM_To_Book->SetCellData( _T("SB_��Ϲ�ȣ"), strItemData, nDMIdx);
				pDM_To_Book->SetCellData( _T("SB_��ϱ���_�ڵ�"), strItemData.Left(nRegCodeLength), nDMIdx);
			}
			else if (strSubfieldCode == _T("$c"))
			{
				pDM_To_Book->SetCellData( _T("SB_������ȣ"), strItemData, nDMIdx);
			}
			else if (strSubfieldCode == _T("$f"))
			{
				pDM_To_Book->SetCellData( _T("SB_�谡��ġ��ȣ"), strItemData, nDMIdx);
			}
		}

		if (pDM_To_Book->GetCellData( _T("SB_��KEY"), nDMIdx).IsEmpty())
		{
			if (mapVolume.Lookup(_T(""), strUniqueKey))
			{
				pDM_To_Book->SetCellData( _T("SB_��KEY"), strUniqueKey, nDMIdx);
			}
		}

		strItemData = pDM_To_Index->GetCellData( _T("ISS_���Ա���_�ڵ�"), 0);
		ids = pDM_To_Book->SetCellData( _T("SB_���Ա���_�ڵ�"), strItemData, nDMIdx);

		strItemData = pDM_To_Index->GetCellData( _T("ISS_���Գ⵵"), 0);
		ids = pDM_To_Book->SetCellData( _T("SB_���Գ⵵"), strItemData, nDMIdx);

		ids = pDM_To_Book->SetCellData( _T("SB_å����"), _T("C"), nDMIdx);

		strItemData = CTime::GetCurrentTime().Format(_T("%Y/%m/%d"));
		ids = pDM_To_Book->SetCellData( _T("SB_�Է���"), strItemData, nDMIdx);

		ids = pDM_To_Book->SetCellData( _T("SB_�Լ���"), strItemData, nDMIdx);

		strItemData = pDM_To_Index->GetCellData( _T("ISS_�з�ǥ����"), 0);
		ids = pDM_To_Book->SetCellData( _T("SB_�з���ȣ����_�ڵ�"), strItemData, nDMIdx);

		ids = m_pMarcMgr->GetItem(&marcSource, _T("090$a"), strItemData);
		ids = pDM_To_Book->SetCellData( _T("SB_�з���ȣ"), strItemData, nDMIdx);

		ids = m_pMarcMgr->GetItem(&marcSource, _T("090$b"), strItemData);
		strItemData.Replace(_T("'"),_T("''"));
		ids = pDM_To_Book->SetCellData( _T("SB_������ȣ"), strItemData, nDMIdx);
		
		ids = pDM_To_Book->SetCellData( _T("SB_�۾���"), m_pParnetMgr->m_pInfo->USER_ID, nDMIdx);

		ids = pDM_To_Book->SetCellData( _T("SB_DB_FLAG"), _T("I"), nDMIdx);

		nDMIdx++;
	}	
		
	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::UpdateSpeciesMarcbyStream(CESL_DataMgr *pDM_OUT_SPECIES, CString strTBL, CString strSpeciesKey, INT RowIdx, BOOL bCommit /*= TRUE*/)
{
	EFS_BEGIN

	if (!pDM_OUT_SPECIES) return-1;

	INT ids = -1;
	if (bCommit)
	{
		ids = pDM_OUT_SPECIES->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
			return -1;
		}
	}

	CMarc marc;
	CString strStreamMarc;
	
    ids = pDM_OUT_SPECIES->GetCellData( _T("SS_MARC"), RowIdx, strStreamMarc);
	ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
	
	if ( bCommit ) {
		ids = m_pMarcMgr->DeleteField( &marc, _T("001") );
		ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	}

	if(strTBL == _T("BO_DELETE_SPECIES_TBL"))
	{
		ids = m_pMarcMgr->DeleteField(&marc, _T("049"));
		ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);
	}

	CString str049Field;
	ids = m_pMarcMgr->GetField(&marc, _T("049"), str049Field);
	if (ids >= 0)
	{
		ids = m_pMarcMgr->DeleteField(&marc, _T("049"));
		ids = m_pMarcMgr->Encoding(&marc, strStreamMarc);

		ids = m_pMarcMgr->GetField(&marc, _T("049"), str049Field);

		CString strRecKey;
		ids = pDM_OUT_SPECIES->MakeRecKey(strRecKey);
		if (ids < 0) return ids;

		CString strQuery;
		strQuery.Format(_T("INSERT INTO SE_HOLDING_TBL (REC_KEY, HOLDING_DATA) VALUES (%s, '%s');"),
						strRecKey, _T("049") + str049Field);
		ids = pDM_OUT_SPECIES->AddFrame(strQuery);
		if (ids < 0) return ids;
		strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET HOLDING_KEY = %s WHERE REC_KEY = %s;"), strRecKey, strSpeciesKey);
		ids = pDM_OUT_SPECIES->AddFrame(strQuery);
		if (ids < 0) return ids;

		pDM_OUT_SPECIES->SetCellData( _T("SS_MARC"), strStreamMarc, 0);
	}

	pDM_OUT_SPECIES->InitDBFieldData();
	pDM_OUT_SPECIES->AddDBFieldData(_T("MARC"), _T("STRING"), strStreamMarc, TRUE);
	ids = pDM_OUT_SPECIES->MakeUpdateFrame(_T("SE_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
	if (ids < 0) return ids;


	if (bCommit)
	{
		ids = pDM_OUT_SPECIES->SendFrame();
		pDM_OUT_SPECIES->EndFrame();
		return ids;
	}

	return 0;
	

	EFS_END
	return -1;
}

INT CCatApi_SE::MergeDM(CESL_DataMgr *pDM_Source, CESL_DataMgr *pDM_Output, LPCTSTR lpszKeyAlias, INT nMode /*= 0*/)
{
	EFS_BEGIN

	if (!pDM_Source || !pDM_Output) return -1;

	INT nSourceRowCount = pDM_Source->GetRowCount();
	INT nOutputRowCount = -1;

	CString strKey;

	BOOL bCopy = TRUE;
	INT nOutputIdx = -1;
	INT nSourceIdx = -1;

	for (nSourceIdx = 0; nSourceIdx < nSourceRowCount; nSourceIdx++)
	{
		nOutputRowCount = pDM_Output->GetRowCount();
		bCopy  = TRUE;

		if (nMode == 2)
			bCopy = TRUE;
		else
		{
			strKey = pDM_Source->GetCellData(lpszKeyAlias, nSourceIdx);
			for (nOutputIdx = 0; nOutputIdx < nOutputRowCount; nOutputIdx++)
			{
				if (strKey == pDM_Output->GetCellData(lpszKeyAlias, nOutputIdx))
				{
					if (nMode == 0)
						bCopy = FALSE;
					else if (nMode == 1)
					{
						pDM_Output->DeleteRow(nOutputIdx);
						bCopy = TRUE;
					}

					break;
				}
			}
		}

		if (bCopy)
			CopyDM(pDM_Source, nSourceIdx, pDM_Output, -1);
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::SyncVolumeBookAppendixDM(CESL_DataMgr *pDM_Volume, 
										 CESL_DataMgr *pDM_Book, 
										 CESL_DataMgr *pDM_HoldBook, 
										 CESL_DataMgr *pDM_Appendix, 
										 CESL_DataMgr *pDM_HoldAppendix,
										 INT nAppMode)
{
	EFS_BEGIN

	if (!pDM_Book || !pDM_Volume || !pDM_HoldBook || !pDM_Appendix || !pDM_HoldAppendix) return -1;

	INT ids = -1;
	INT nVolIdx, nTmpIdx;
	INT nBookCountInVol, nHoldBookCountInVol, nAppendixCountInVol, nHoldAppendixCountInVol;
	INT nVolCount = pDM_Volume->GetRowCount();
	INT nBookCount = pDM_Book->GetRowCount();
	INT nHoldBookCount = pDM_HoldBook->GetRowCount();
	INT nAppendixCount = pDM_Appendix->GetRowCount();
	INT nHoldAppendixCount = pDM_HoldAppendix->GetRowCount();
	INT nTotDataCnt;

	CString strTmpCnt;

	CString strVolKey, strBookVolKey, strBookKey, strTotDataCnt;
	CString strVolDBFlag, strTmpDBFlag;
	CStringArray arrBookKeyList;
	BOOL bKeyInList;
	
	for (nVolIdx = 0; nVolIdx < nVolCount; nVolIdx++)
	{
		nBookCountInVol = 0;
		nHoldBookCountInVol = 0;
		nAppendixCountInVol = 0;
		nHoldAppendixCountInVol = 0;
		nTotDataCnt = 0;
		arrBookKeyList.RemoveAll();

		ids = pDM_Volume->GetCellData(_T("SV_��KEY"), nVolIdx, strVolKey);
		if (ids < 0 || strVolKey.IsEmpty()) continue;

		ids = pDM_Volume->GetCellData(_T("SV_���ڷ��"), nVolIdx, strTotDataCnt);
		if (ids < 0 || strTotDataCnt.IsEmpty()) continue;
		nTotDataCnt = _ttoi(strTotDataCnt);

		ids = pDM_Volume->GetCellData(_T("SV_DB_FLAG"), nVolIdx, strVolDBFlag);
		if (ids < 0 ) continue;

		for (nTmpIdx = 0; nTmpIdx < nBookCount; nTmpIdx++)
		{
			strBookVolKey = pDM_Book->GetCellData(_T("SB_��KEY"), nTmpIdx);
			strTmpDBFlag = pDM_Book->GetCellData(_T("SB_DB_FLAG"), nTmpIdx);
			strBookKey = pDM_Book->GetCellData(_T("SB_åKEY"), nTmpIdx);
			if (strVolKey == strBookVolKey)
			{
				arrBookKeyList.Add(strBookKey);
				if (strVolDBFlag==_T("D")) 
				{
					pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T("D"), nTmpIdx);
					continue;
				}
				else if (strTmpDBFlag ==_T("D"))
				{
					nTotDataCnt--;
					continue;
				}

				CopyDM(pDM_Volume, nVolIdx, pDM_Book, nTmpIdx);
				if ( strTmpDBFlag == _T("") ) pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T("U"), nTmpIdx);

				if (pDM_Book->GetCellData(_T("SB_��å��ȣ"), nTmpIdx).IsEmpty())
				{
					pDM_Book->SetCellData(_T("SB_��å��ȣ"),  pDM_Book->GetCellData(_T("SV_��ȣ��"), nTmpIdx), nTmpIdx);
				}
				nBookCountInVol++;
				nTotDataCnt--;
			}
		}

		for (nTmpIdx = 0; nTmpIdx < nHoldBookCount; nTmpIdx++)
		{
			strBookVolKey = pDM_HoldBook->GetCellData(_T("SB_��KEY"), nTmpIdx);
			strTmpDBFlag = pDM_HoldBook->GetCellData(_T("SB_DB_FLAG"), nTmpIdx);
			strBookKey = pDM_HoldBook->GetCellData(_T("SB_åKEY"), nTmpIdx);
			if (strVolKey == strBookVolKey)
			{
				bKeyInList = FALSE;
				for (INT i=0; i< arrBookKeyList.GetSize(); i++)
					if ( arrBookKeyList.GetAt(i) == strBookKey) bKeyInList = TRUE;
				if (!bKeyInList) arrBookKeyList.Add(strBookKey);

				if (strVolDBFlag==_T("D")) 
				{
					pDM_HoldBook->SetCellData(_T("SB_DB_FLAG"), _T("D"), nTmpIdx);
					continue;
				}
				else if (strTmpDBFlag ==_T("D"))
				{
					if (!bKeyInList) nTotDataCnt--;
					continue;
				}

				if (strTmpDBFlag ==_T("I"))
					CopyDM(pDM_Volume, nVolIdx, pDM_HoldBook, nTmpIdx);

				nHoldBookCountInVol++;
				if (!bKeyInList) nTotDataCnt--;
			}
		}

		for (nTmpIdx = 0; nTmpIdx < nAppendixCount; nTmpIdx++)
		{
			strBookVolKey = pDM_Appendix->GetCellData(_T("SB_��KEY"), nTmpIdx);
			strTmpDBFlag = pDM_Appendix->GetCellData(_T("SB_DB_FLAG"), nTmpIdx);
			strBookKey = pDM_Appendix->GetCellData(_T("SB_åKEY"), nTmpIdx);
			if (strVolKey == strBookVolKey)
			{
				bKeyInList = FALSE;
				for (INT i=0; i< arrBookKeyList.GetSize(); i++)
					if ( arrBookKeyList.GetAt(i) == strBookKey) bKeyInList = TRUE;
				if (!bKeyInList) arrBookKeyList.Add(strBookKey);

				if (strVolDBFlag==_T("D")) 
				{
					pDM_Appendix->SetCellData(_T("SB_DB_FLAG"), _T("D"), nTmpIdx);
					continue;
				}
				else if (strTmpDBFlag ==_T("D"))
				{
					if (!bKeyInList) nTotDataCnt--;
					continue;
				}

				CopyDM(pDM_Volume, nVolIdx, pDM_Appendix, nTmpIdx);
				if ( strTmpDBFlag == _T("") ) pDM_Appendix->SetCellData(_T("SB_DB_FLAG"), _T("U"), nTmpIdx);

				if (pDM_Appendix->GetCellData(_T("SB_��å��ȣ"), nTmpIdx).IsEmpty())
				{
					pDM_Appendix->SetCellData(_T("SB_��å��ȣ"),  pDM_Appendix->GetCellData(_T("SV_��ȣ��"), nTmpIdx), nTmpIdx);
				}
				nAppendixCountInVol++;
				if (!bKeyInList) nTotDataCnt--;
			}
		}

		for (nTmpIdx = 0; nTmpIdx < nHoldAppendixCount; nTmpIdx++)
		{
			strBookVolKey = pDM_HoldAppendix->GetCellData(_T("SB_��KEY"), nTmpIdx);
			strTmpDBFlag = pDM_HoldAppendix->GetCellData(_T("SB_DB_FLAG"), nTmpIdx);
			strBookKey = pDM_HoldAppendix->GetCellData(_T("SB_åKEY"), nTmpIdx);
			if (strVolKey == strBookVolKey)
			{
				bKeyInList = FALSE;
				for (INT i=0; i< arrBookKeyList.GetSize(); i++)
					if ( arrBookKeyList.GetAt(i) == strBookKey) bKeyInList = TRUE;
				if (!bKeyInList) arrBookKeyList.Add(strBookKey);

				if (strVolDBFlag==_T("D")) 
				{
					pDM_HoldAppendix->SetCellData(_T("SB_DB_FLAG"), _T("D"), nTmpIdx);
					continue;
				}
				else if (strTmpDBFlag ==_T("D"))
				{
					if (!bKeyInList) nTotDataCnt--;
					continue;
				}

				if (strTmpDBFlag ==_T("I"))
					CopyDM(pDM_Volume, nVolIdx, pDM_HoldAppendix, nTmpIdx);

				nHoldAppendixCountInVol++;
				if (!bKeyInList) nTotDataCnt--;
			}
		}

		strTmpCnt.Format(_T("%d"), nBookCountInVol);
		pDM_Volume->SetCellData(_T("SV_å��"), strTmpCnt, nVolIdx);
		strTmpCnt.Format(_T("%d"), nHoldBookCountInVol);
		pDM_Volume->SetCellData(_T("SV_����å��"), strTmpCnt, nVolIdx);
		strTmpCnt.Format(_T("%d"), nAppendixCountInVol);
		pDM_Volume->SetCellData(_T("SV_�ηϼ�"), strTmpCnt, nVolIdx);
		strTmpCnt.Format(_T("%d"), nHoldAppendixCountInVol);
		pDM_Volume->SetCellData(_T("SV_����ηϼ�"), strTmpCnt, nVolIdx);
		strTotDataCnt.Format(_T("%d"), nTotDataCnt);
		pDM_Volume->SetCellData(_T("SV_��Ÿ�ڷ��"), strTotDataCnt, nVolIdx);
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::RecordLocDate(CESL_DataMgr *pDM_Out, CStringArray *pArrSpeciesKey, CStringArray *pArrBookKey, CStringArray *pArrAppendixKey, CString &strShelfDate, BOOL bCommit /*= TRUE*/)
{
	EFS_BEGIN

	if (!pDM_Out || !pArrSpeciesKey || !pArrBookKey) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;

	CString strRecKey;
	CString strQuery;

	if (bCommit)
	{
		ids = pDM_Out->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
			return -1;
		}
	}

	nRowCount = pArrSpeciesKey->GetSize();
	for (idx = 0; idx < nRowCount; idx++)
	{
		strRecKey = pArrSpeciesKey->GetAt(idx);
		if (strRecKey.IsEmpty()) continue;

		strQuery.Format(_T("UPDATE IDX_SE_TBL SET SPECIES_CLASS = '1', WORKING_STATUS = 'SEL212N' WHERE REC_KEY = %s;")
							, strRecKey);

		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;

		strQuery.Format(_T("UPDATE SE_SPECIES_TBL SET LOC_FLAG = '0' WHERE REC_KEY = %s;"), strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;
		
	}

	nRowCount = pArrBookKey->GetSize();
	for (idx = 0; idx < nRowCount; idx++)
	{
		strRecKey = pArrBookKey->GetAt(idx);
		if (strRecKey.IsEmpty()) continue;

		strQuery.Format(_T("UPDATE SE_BOOK_TBL SET SHELF_DATE = '%s', WORKING_STATUS = 'SEL212N', SHELF_WORKER = '%s' WHERE REC_KEY = %s or BINDING_BOOK_KEY=%s;")
					, strShelfDate, m_pParnetMgr->m_pInfo->USER_ID, strRecKey, strRecKey);
		ids = pDM_Out->AddFrame(strQuery);
		if (ids < 0) return ids;
	}

	if (pArrAppendixKey)
	{
		nRowCount = pArrAppendixKey->GetSize();
		for (idx = 0; idx < nRowCount; idx++)
		{
			strRecKey = pArrAppendixKey->GetAt(idx);
			if (strRecKey.IsEmpty()) continue;

			strQuery.Format(_T("UPDATE SE_BOOK_TBL SET SHELF_DATE = '%s', WORKING_STATUS = 'SEL212N', SHELF_WORKER = '%s' WHERE REC_KEY = %s or BINDING_BOOK_KEY=%s;")
					, strShelfDate, m_pParnetMgr->m_pInfo->USER_ID, strRecKey, strRecKey);
			ids = pDM_Out->AddFrame(strQuery);
			if (ids < 0) return ids;
		}
	}

	if (bCommit)
	{
		ids = pDM_Out->SendFrame();
		if (ids < 0) 
		{
			m_strLastErrMsg = _T("DB�� ������ ���� ����");
			return ids;
		}

		ids = pDM_Out->EndFrame();
		if (ids < 0) return ids;
	}

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::MakeVolumeQueryFrame(CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_HoldBook, 
		CESL_DataMgr *pDM_Appendix, CESL_DataMgr *pDM_HoldAppendix, CString strSpeciesKey, CESL_DataMgr *pDM_OUT)
{
	EFS_BEGIN

	if (!pDM_Volume || !pDM_Book || 
		!pDM_HoldBook || !pDM_Appendix || 
		!pDM_HoldAppendix || !pDM_OUT || 
		strSpeciesKey.IsEmpty()) return -1;

	INT ids = -1;
	INT idx = -1;
	CString strQuery;
	CString strVolumeKey;
	CString strQueryFlag;

	CIndex *pIndex = m_pIndexTool->GetIndex();

	CArray <CString, CString> RemoveAliasList;

	CSeApi api(m_pParnetMgr);
	CString strSPECIES_KEY, strVOL_KEY, strVOL_TYPE, strVolTitle, msg;
	CStringArray arrayChildBookKeys;

	INT nRowCount = pDM_Volume->GetRowCount();

	for (idx = 0; idx < nRowCount; idx++) {
		RemoveAliasList.RemoveAll();
		ids = pDM_Volume->GetCellData(_T("SV_DB_FLAG"), idx, strQueryFlag);
		if (ids < 0) continue;

		ids = pDM_Volume->GetCellData(_T("SV_��KEY"), idx, strVolumeKey);
		if (strQueryFlag == _T("I")) 
		{
			RemoveAliasList.Add(_T("SV_���ڷ��"));
			pDM_Volume->SetCellData(_T("SV_��KEY"), strSpeciesKey, idx);
			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Volume, pDM_Volume->TBL_NAME, idx, pDM_OUT);
			LogToFile(pDM_OUT->m_lastmsg);
		}
		if (strQueryFlag == _T("U")) 
		{
			RemoveAliasList.Add(_T("SV_��KEY"));
			RemoveAliasList.Add(_T("SV_���ڷ��"));

			pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM_Volume, pDM_Volume->TBL_NAME, idx, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strVolumeKey);
			LogToFile(pDM_OUT->m_lastmsg);
		}
		else if (strQueryFlag == _T("D")) 
		{
			strSPECIES_KEY = pDM_Volume->GetCellData(_T("SV_��KEY"),idx);
			strVOL_KEY = pDM_Volume->GetCellData(_T("SV_��KEY"),idx);
			strVOL_TYPE = pDM_Volume->GetCellData(_T("SV_��ȣ����"),idx);
			strVolTitle = pDM_Volume->GetCellData(_T("SV_��ȣ��"),idx);

			INT i, nCount;
			nCount = pDM_Book->GetRowCount();
			for (i=0; i<nCount; i++){
				if ( strVOL_KEY == pDM_Book->GetCellData(_T("SB_��KEY"),i) )
				{
					arrayChildBookKeys.Add( pDM_Book->GetCellData(_T("SB_åKEY"),idx) );
					pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T(""), idx);	// 050112_�����߰�
				}
			}
			nCount = pDM_HoldBook->GetRowCount();
			for (i=0; i<nCount; i++){
				if ( strVOL_KEY == pDM_HoldBook->GetCellData(_T("SB_��KEY"),i) )
				{
					arrayChildBookKeys.Add( pDM_HoldBook->GetCellData(_T("SB_åKEY"),idx) );
					pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T(""), idx);	// 050112_�����߰�
				}
			}
			nCount = pDM_Appendix->GetRowCount();
			for (i=0; i<nCount; i++){
				if ( strVOL_KEY == pDM_Appendix->GetCellData(_T("SB_��KEY"),i) )
				{
					arrayChildBookKeys.Add( pDM_Appendix->GetCellData(_T("SB_åKEY"),idx) );
					pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T(""), idx);	// 050112_�����߰�
				}
			}
			nCount = pDM_HoldAppendix->GetRowCount();
			for (i=0; i<nCount; i++){
				if ( strVOL_KEY == pDM_HoldAppendix->GetCellData(_T("SB_��KEY"),i) )
				{
					arrayChildBookKeys.Add( pDM_HoldAppendix->GetCellData(_T("SB_åKEY"),idx) );
					pDM_Book->SetCellData(_T("SB_DB_FLAG"), _T(""), idx);	// 050112_�����߰�
				}
			}

			ids = api.VOL_CAT_DELETE(pDM_OUT,
					   strSPECIES_KEY,
					   strVOL_KEY,
					   strVOL_TYPE,
					   strVolTitle,
					   arrayChildBookKeys,
					   TRUE, FALSE);
			if(ids<0)
			{
				if(-1002 == ids)
				{
					msg.Format(_T("[ %s ] �ڷḦ ������ �� �����ϴ�!"), strVolTitle);
					AfxMessageBox(msg);
					continue;
				}
				else
				{
					msg.Format(_T("[ %s ] �ڷḦ �����ϴ� �� ������ �߻��� �۾��� �ߴ��մϴ�!"), strVolTitle);
					AfxMessageBox(msg);
					return -200;
				}
			}

			LogToFile(pDM_OUT->m_lastmsg);
		}
	}	

	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::GetBookCount(CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Appendix, CString strRegCode /*= _T("")*/, LPCTSTR lpszVol /*=NULL*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL || pDM_Appendix == NULL) return -1;

	INT nReturnCnt = 0;

	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("SB"), lpszVol)) continue;

		if (pDM_Book->GetCellData( _T("SB_DB_FLAG"), idx) == _T("D")) continue;

		if (!strRegCode.IsEmpty())
			if (pDM_Book->GetCellData( _T("SB_��ϱ���_�ڵ�"), idx) != strRegCode) continue;
		
		nReturnCnt++;
	}

	nRowCount = pDM_Appendix->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Appendix, idx, _T("SB"), lpszVol)) continue;

		if (pDM_Appendix->GetCellData( _T("SB_DB_FLAG"), idx) == _T("D")) continue;

		if (!strRegCode.IsEmpty())
			if (pDM_Appendix->GetCellData( _T("SB_��ϱ���_�ڵ�"), idx) != strRegCode) continue;

		nReturnCnt++;
	}

	return nReturnCnt;

	EFS_END
	return -1;
}

INT CCatApi_SE::InitLastNumDM()
{
	CESL_DataMgr *pDmLastNum = m_pParnetMgr->FindDM( _T("DM_����_��������ȣ") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("[DM_��������ȣ]�� �ʱ�ȭ�� �����Ͽ����ϴ�") );
		return -1;
	}
	pDmLastNum->FreeData();

	return 0;
}

INT CCatApi_SE::InsertAppendixToDM(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Appendix
							   , INT nVolIndex, CString strWorkingStatus, INT nDupNumber
							   , CString strRegCode, CString strAcqDate, CString strShelfLocCode, CString strRemark
							   , CString strAppendixName, CString strAppFormCode, CString strMediaCode)
{
	INT nAppDMIdx = InsertBookToDM(pDM_Species, pDM_Volume, pDM_Appendix, nVolIndex
			, strWorkingStatus, nDupNumber, strRegCode, strAcqDate, strShelfLocCode, strRemark );
	if ( nAppDMIdx<0 ) 
		return nAppDMIdx;
	else 
	{
		CString strItemData;
		INT ids;

		ids = pDM_Appendix->MakeRecKey(strItemData);
		if (ids < 0) return ids;
		pDM_Appendix->SetCellData( _T("SA_�η�KEY"), strItemData, nAppDMIdx);
		pDM_Appendix->SetCellData( _T("SA_åKEY"), pDM_Appendix->GetCellData(_T("SB_åKEY"), nAppDMIdx), nAppDMIdx);
		pDM_Appendix->SetCellData( _T("SA_�ηϸ�"), strAppendixName, nAppDMIdx );
		pDM_Appendix->SetCellData( _T("SA_���ı���"), strAppFormCode, nAppDMIdx );
		pDM_Appendix->SetCellData( _T("SA_��ü����"), strMediaCode, nAppDMIdx );
		pDM_Appendix->SetCellData( _T("SA_���ົ��Ͽ���"), _T("N"), nAppDMIdx );
		pDM_Appendix->SetCellData( _T("SA_��⿩��"), _T("N"), nAppDMIdx );

		pDM_Appendix->SetCellData( _T("SB_å����"), _T("A"), nAppDMIdx );

	}

	return nAppDMIdx;
}

INT CCatApi_SE::InsertBookToDM(CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Book
							   , INT nVolIndex, CString strWorkingStatus, INT nDupNumber
							   , CString strRegCode, CString strAcqDate, CString strShelfLocCode, CString strRemark)
{
	if (pDM_Species == NULL ||
		pDM_Volume == NULL ||
		pDM_Book == NULL ||
		nVolIndex < 0 ||
		strWorkingStatus.GetLength() == 0)
		return -1;

	INT ids;
	CString strItemData;

	ids = pDM_Book->InsertRow(-1);
	INT nBookIndex = pDM_Book->GetRowCount() -1;

	ids = pDM_Book->MakeRecKey(strItemData);
	if (ids < 0) return ids;
	pDM_Book->SetCellData( _T("SB_åKEY"), strItemData, nBookIndex);

	pDM_Book->SetCellData( _T("SB_��KEY"), pDM_Species->GetCellData( _T("SS_��KEY"), 0), nBookIndex );
	pDM_Book->SetCellData( _T("SB_��KEY"), pDM_Volume->GetCellData( _T("SV_��KEY"), nVolIndex), nBookIndex );

	strItemData = _T("");
	strItemData = CTime::GetCurrentTime().Format(_T("%Y"));
	pDM_Book->SetCellData( _T("SB_���Գ⵵"), strItemData, nBookIndex );

	pDM_Book->SetCellData( _T("SB_å����"), _T("C"), nBookIndex );
	pDM_Book->SetCellData( _T("SB_����å����"), _T("N"), nBookIndex );

	if (strRegCode.GetLength() > 0)
		pDM_Book->SetCellData( _T("SB_��ϱ���_�ڵ�"), strRegCode, nBookIndex );
	else pDM_Book->SetCellData( _T("SB_��ϱ���_�ڵ�"), pDM_Species->GetCellData( _T("SS_��ϱ���_�ڵ�"), 0), nBookIndex );

	pDM_Book->SetCellData( _T("SB_�̿����ѱ���_�ڵ�"), pDM_Species->GetCellData( _T("SS_�̿����ѱ���_�ڵ�"), 0), nBookIndex );
	pDM_Book->SetCellData( _T("SB_��������_�ڵ�"), pDM_Species->GetCellData( _T("SS_��������_�ڵ�"), 0), nBookIndex );

	strItemData = _T("");
	strItemData = CTime::GetCurrentTime().Format(_T("%Y/%m/%d"));
	pDM_Book->SetCellData( _T("SB_�Է���"), strItemData, nBookIndex );

	if (strAcqDate.GetLength() > 0)
		pDM_Book->SetCellData( _T("SB_�Լ���"), strAcqDate, nBookIndex );
	else pDM_Book->SetCellData( _T("SB_�Լ���"), strItemData, nBookIndex );

	pDM_Book->SetCellData( _T("SB_�ΰ迩��"), _T("Y"), nBookIndex );

	if (strShelfLocCode.GetLength() > 0)
		pDM_Book->SetCellData( _T("SB_�谡��ġ��ȣ"), strShelfLocCode, nBookIndex );
	else pDM_Book->SetCellData( _T("SB_�谡��ġ��ȣ"), pDM_Species->GetCellData( _T("SS_�谡��ġ_�ڵ�"), 0), nBookIndex );

	pDM_Book->SetCellData( _T("SB_�з���ȣ����_�ڵ�"), pDM_Species->GetCellData( _T("SS_�з�ǥ����"), 0), nBookIndex );

	if (nDupNumber == 0) strItemData = _T("");
	else strItemData.Format(_T("%d"), nDupNumber);
	pDM_Book->SetCellData( _T("SB_������ȣ"), strItemData, nBookIndex );

	pDM_Book->SetCellData( _T("SB_���̺���±���"), _T("1"), nBookIndex );
	pDM_Book->SetCellData( _T("SB_�ڷ����"), strWorkingStatus, nBookIndex );
	pDM_Book->SetCellData( _T("SB_���"), strRemark, nBookIndex );
	pDM_Book->SetCellData( _T("SB_�۾���"), m_pParnetMgr->m_pInfo->USER_ID, nBookIndex );
	pDM_Book->SetCellData( _T("SB_�弭����"), _T("N"), nBookIndex );
	pDM_Book->SetCellData( _T("SB_DB_FLAG"), _T("I"), nBookIndex );


	return nBookIndex;
}

INT CCatApi_SE::ConvertCatDMToMarc(CMarc *pMarc, CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Purchase /*= NULL*/)
{
	EFS_BEGIN

	if (pMarc == NULL ||
		pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL ||
		pDM_Volume == NULL ) return -1;

	INT ids = -1;
	
	CString strDMFieldAlias;
	CString strMarcAlias;
	CString strItemData;
	CArray<CString, CString&> arrItemData;

	POSITION pos = m_mapCatDMtoMarc.GetStartPosition();
	while (pos)
	{
		m_mapCatDMtoMarc.GetNextAssoc(pos, strDMFieldAlias, strMarcAlias);

		pDM_Index->GetCellData(strDMFieldAlias, 0, strItemData);

		if (strItemData.IsEmpty()) continue;

		m_pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData, _T(""), MODIFY_ITEM);
	}

	CString strCreateDate ;
    m_pMarcMgr->GetItem(pMarc,_T("008*"),strCreateDate) ;
    if(!strCreateDate.IsEmpty ())
    {   
		ids = -1;
		
		ids = m_pMarcMgr->GetItem(pMarc, _T("008@0-5"), strItemData);
		strItemData.TrimLeft();
		strItemData.TrimRight();
		if (ids < 0 || strItemData.IsEmpty())
			m_pMarcMgr->SetItem(pMarc, _T("008@0-5"), CTime::GetCurrentTime().Format(_T("%y%m%d")));

		ids = m_pMarcMgr->GetItem(pMarc, _T("008@6"), strItemData);
		if (ids < 0 || strItemData.IsEmpty())
		{
			strItemData = _T("c");
			m_pMarcMgr->SetItem(pMarc, _T("008@6"), strItemData);
		}
		
		if (strItemData == _T("c"))
		{
			ids = m_pMarcMgr->GetItem(pMarc, _T("260$c"), strItemData);
			
			CString strYear;
			INT nLength = strItemData.GetLength();
			TCHAR tch = 0;
			for (INT idx = 0; idx < nLength; idx++)
			{
				tch = strItemData.GetAt(idx);
				if (_istdigit(tch))
					strYear += tch;
			}
			
			if (ids < 0 || strYear.IsEmpty())
				strYear = CTime::GetCurrentTime().Format(_T("%Y"));
			
			m_pMarcMgr->SetItem(pMarc, _T("008@7-10"), strYear);
		}
    }

	strItemData = pDM_Index->GetCellData(_T("ISS_������"), 0);
	if (!strItemData.IsEmpty())
	{
		m_mapMarcAlias.Lookup(_T("����"), strMarcAlias);
		m_pMarcMgr->DeleteItem(pMarc, strMarcAlias);
		m_mapMarcAlias.Lookup(_T("������"), strMarcAlias);
		m_pMarcMgr->DeleteItem(pMarc, strMarcAlias);

		INT n = 0;

		LPTSTR lpszTok = _tcstok(strItemData.GetBuffer(0), _T(";"));
		while (lpszTok)
		{
			if (n == 0)
				m_mapMarcAlias.Lookup(_T("����"), strMarcAlias);
			else
				m_mapMarcAlias.Lookup(_T("������"), strMarcAlias);
			
			m_pMarcMgr->SetItem(pMarc, strMarcAlias, lpszTok);
			n++;
			lpszTok = _tcstok(NULL, _T(";"));
		}
	}
	
	strItemData = pDM_Index->GetCellData(_T("ISS_������"), 0);
	if (!strItemData.IsEmpty())
	{
		m_mapMarcAlias.Lookup(_T("������"), strMarcAlias);
		m_pMarcMgr->DeleteItem(pMarc, strMarcAlias);

		LPTSTR lpszTok = _tcstok(strItemData.GetBuffer(0), _T(":"));
		while (lpszTok)
		{
			m_pMarcMgr->SetItem(pMarc, strMarcAlias, lpszTok);

			lpszTok = _tcstok(NULL, _T(":"));
		}
	}

	m_pMarcMgr->CheckSimpleSyntax(pMarc);
	
	return 0;

	EFS_END
	return -1;
}

INT CCatApi_SE::ConvertMarcToCatDM(CMarc *pMarc, CESL_DataMgr *pDM_Species, CESL_DataMgr *pDM_Index, CESL_DataMgr *pDM_Book, CESL_DataMgr *pDM_Volume, CESL_DataMgr *pDM_Purchase /*= NULL*/)
{
	EFS_BEGIN

	if (pMarc == NULL ||
		pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL ||
		pDM_Volume == NULL
		) return -1;

	INT ids = -1;
	INT nVolCount = -1;

	CString strDMFieldAlias;
	CString strMarcAlias;
	CString strItemData;
	CArray<CString, CString&> arrItemData;

	CString sStreamMarc;
	ids = m_pMarcMgr->Encoding(pMarc, sStreamMarc);
	if (ids<0 || sStreamMarc.GetLength() <=0 ) return -1;
	ids = pDM_Species->SetCellData(_T("SS_MARC"), sStreamMarc, 0);
	if (ids<0) return -1;

	POSITION pos = m_mapCatDMtoMarc.GetStartPosition();
	while (pos)
	{
		m_mapCatDMtoMarc.GetNextAssoc(pos, strDMFieldAlias, strMarcAlias);

		ids = m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
		if (ids < 0) continue;

		pDM_Index->SetCellData(strDMFieldAlias, strItemData, 0);		
	}

	POSITION posTag = NULL;
	POSITION posSubfield = NULL;

	CString strTitle;
	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;
	CString strSubfieldCode;
	posTag = pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)pMarc->m_listTag.GetNext(posTag);
		if (pTag && pTag->GetTagCode() == _T("245"))
		{
			posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield)
			{
				pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (!pSubfield) continue;

				strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") ||
					strSubfieldCode == _T("$x") ||
					strSubfieldCode == _T("$b"))
				{
					strTitle += pSubfield->GetPunctuationMark() + pSubfield->GetItemData();
				}
			}
		}
	}

	pDM_Index->SetCellData(_T("ISS_��ǥ��"), strTitle, 0);
		
	m_mapMarcAlias.Lookup(_T("����"), strMarcAlias);
	m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);

	{
		CString strAuthor = strItemData;
		arrItemData.RemoveAll();
		m_mapMarcAlias.Lookup(_T("������"), strMarcAlias);
		ids = m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData, &arrItemData);
		if (ids >= 0)
		{
			INT nCount = arrItemData.GetSize();
			for (INT idx = 0; idx < nCount; idx++)
			{
				strItemData = arrItemData.GetAt(idx);
				if (strItemData.IsEmpty()) continue;

				strAuthor += _T(";") + strItemData;
			}
		}

		pDM_Index->SetCellData(_T("ISS_������"), strAuthor, 0);
	}

	CString strPublisher;
	arrItemData.RemoveAll();

	m_mapMarcAlias.Lookup(_T("������"), strMarcAlias);
	m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData, &arrItemData);

	{
		strPublisher = strItemData;
		INT nCount = arrItemData.GetSize();
		for (INT idx = 1; idx < nCount; idx++)
		{
			strItemData = arrItemData.GetAt(idx);
			if (strItemData.IsEmpty()) continue;

			strPublisher += _T(":") + strItemData;
		}

		pDM_Index->SetCellData(_T("ISS_������"), strPublisher, 0);
	}

	return 0;

	EFS_END
	return -1;
}






