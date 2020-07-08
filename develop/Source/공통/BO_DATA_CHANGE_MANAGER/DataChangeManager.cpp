// DataChangeManager.cpp: implementation of the CDataChangeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataChangeManager.h"
#include "../BO_ACQ_GROUP_MANAGER/GroupPair.h"
#include "../COMMON_CONVERSION_MGR/MarcInfoConversionMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("����/�ڵ���������"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������
CDataChangeManager::CDataChangeManager(CESL_Mgr * pMgr):CSPF_Work(pMgr)
{
	pFormApi = new CBOFormApi(pMgr);
	pSpeciesApi = new CBO_SPECIES(pMgr);
	m_pDataProcessingDlg = NULL;
	m_MapSpeciesKey.RemoveAll();
	m_bSpeciesSubjectCode = FALSE;
	m_bVolYN = FALSE;
	m_bRefreshCopyCode = FALSE;
	m_bCatCode = FALSE;
	bChangeSubjectCode = FALSE;
}

CDataChangeManager::SetExchangeRate( CString strExchangeRate )
{
	m_ExchangeRate  =  strExchangeRate;
}	

// �Ҹ���
CDataChangeManager::~CDataChangeManager()
{
	if( pFormApi != NULL )
	{
		delete pFormApi;
		pFormApi = NULL;
	}
	
	if( pSpeciesApi != NULL )
	{
		delete pSpeciesApi;
		pSpeciesApi = NULL;
	}

	if( m_pDataProcessingDlg != NULL )
	{
		DeleteDataProcessingDlg();
	}

	DeleteDMArray();
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���ڷ� ���� ���� �ش��ϴ� �Լ��� ȣ���Ѵ�.
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::SPFExecutePath(CString strPath)
{
	if( strPath.GetLength() == 0 )	return -1;

	if		( strPath == _T("�ڵ庯��")		)		return CodeChange();
	else if	( strPath == _T("������������") )		return DonateInfoChange();
	else if	( strPath == _T("������������") )		return AcqInfoChange();
	else if	( strPath == _T("������������") )		return PurchaseInfoChange();
	else if	( strPath == _T("å�ڵ庯��")	)		return BookCodeChange();
	else if	( strPath == _T("�ֱ�ȯ����������") )	return GetLatestExchangeRate();

	return -1;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ڵ� ������ �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ����� �ӽõ����� �� �����Ϳ��� ���� �����´�.
// 2. �Լ��ڵ尡 �����ڷᰡ �ƴϰ� ���� �ƴ϶�� ������� m_bVolYN�� TRUE�� �����Ѵ�.
// 3. ������ ���� ������ å������� ���Ϸù�ȣ�� �����´�.
// 4. ��Ű���� �����´�.
// 5. ������ �ڷ���� �ڵ尪�� �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::CodeChange()
{
	//================================================================
	// ��������
	//================================================================
	INT ids, i, nPos, nBookIdx, nAcqCode = -1;
	BOOL bSpeciesSubjectCode = FALSE;
	BOOL bDeleteData = FALSE;
	BOOL bSpeciesYN, bApplyYN, bWorkCodeIgnore, bTmpData;
	CString strSpeciesKeyAlias, strVolSortNoAlias, strSpeciesKey, strVolSortNo, strMsg, strTmpData;
	CStringArray arrIgnoreSpecies;
	CCodeChangeCondition* pCont = NULL;

	
	//================================================================
	// 1. ����� �ӽõ����� �� �����Ϳ��� ���� �����´�.
	//================================================================
	bSpeciesYN = bApplyYN = bWorkCodeIgnore = bTmpData = FALSE;

	CESL_DataMgr* pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_DataMgr* pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	CESL_ControlMgr* pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	CESL_Grid* pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	pCont = (CCodeChangeCondition*)GetTempPtr(_T("Condition"));
	//CM/DM����
	if( pDM == NULL || pCM == NULL || pCont == NULL || pGrid == NULL)	return -1000;	
	
	GetTempData(_T("AcqCode"), nAcqCode);
	GetTempData(_T("SPECIES_SUBJECT_CODE"), bSpeciesSubjectCode);
	GetTempData(_T("IS_DELETE_DATA"), bDeleteData);
	GetTempData(_T("SPECIES_YN"), bSpeciesYN);
	GetTempData(_T("IS_CAT_CODE"), m_bCatCode);



	//================================================================
	// 2. �Լ��ڵ尡 �����ڷᰡ �ƴϰ� ���� �ƴ϶�� ������� m_bVolYN�� TRUE�� �����Ѵ�.
	//================================================================
	if( nAcqCode != 0 && bSpeciesYN == FALSE )		m_bVolYN = TRUE;
	else m_bVolYN = FALSE;
	
	//================================================================
	// 3. ������ ���� ������ å������� ���Ϸù�ȣ�� �����´�.
	//================================================================
	if( m_bVolYN == TRUE )	strVolSortNoAlias = _T("BB_���Ϸù�ȣ");
	else					strVolSortNoAlias.Empty();
	

	//================================================================
	// 4. ��Ű���� �����´�.
	//================================================================
	strSpeciesKeyAlias = _T("IBS_��KEY");


	//================================================================
	// 5. ������ �ڷ���� �ڵ尪�� �����Ѵ�.
	//================================================================
	ids = pGrid->SelectMakeList();
	if( ids < 0 )	return -2000;
	i = pGrid->SelectGetHeadPosition();
	if( i < 0 )		return -2000;

	// Progress Ctrl ����
	ShowDataProcessingDlg();
	strMsg.Format(_T("�����Ͻ� %d���� �ڷḦ �ڵ庯���ϰ� �ֽ��ϴ�."), pGrid->SelectGetCount());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pGrid->SelectGetCount());
	
	nPos = 0;
	nBookIdx = -1;
	arrIgnoreSpecies.RemoveAll();
	// Start
	pDM->StartFrame();
	while( i >= 0 )
	{
		// Progress Ctrl ��ġ���� �ϳ��� �ø���.
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nPos);

		// ���̰ų� ��ǥå�̶�� bApplyYN�� TRUE�� �����Ѵ�.
		if( bSpeciesYN == TRUE )	bApplyYN = TRUE;
		else
		{
			strTmpData = pDM->GetCellData(_T("BB_��ǥå����"), i);
			if( strTmpData == _T("Y") )	bApplyYN = TRUE;
		}
		
		// ���ʴ���ڷ� �Լ��̰� ���� �ƴ϶�� �����ϰ�쿡 ���������ڵ尪�� �������� ���ϵ��� �Ѵ�.
		if( nAcqCode == 0 && bSpeciesYN == FALSE )
		{
			strTmpData = pDM->GetCellData(_T("BB_���Ա���"), i);
			if( strTmpData == _T("1") )
			{
				bWorkCodeIgnore = TRUE;
				bTmpData = pCont->GetWorkCode();
				pCont->SetWorkCode(FALSE);
			}
		}
		// ���������� �ڵ庯�������� �ۼ��Ѵ�.
		MakeSpeciesCodeChangeQuery(pDM, pCM, pCont, i, &arrIgnoreSpecies, bSpeciesSubjectCode, bDeleteData, bApplyYN);
		// ������������ �ڵ庯�������� �ۼ��Ѵ�.
		MakeAccuCodeChangeQuery(pDM, pCM, pCont, nAcqCode, i, &arrIgnoreSpecies, bSpeciesSubjectCode);
		// å�������� �ڵ庯�������� �ۼ��Ѵ�.
		MakeBookCodeChangeQuery(pDM, pCM, pCont, i, bSpeciesYN, &arrIgnoreSpecies, bDeleteData);
		// ������������ �ڵ庯�������� �ۼ��Ѵ�.
		MakeIndexCodeChangeQuery(pDM, pCM, pCont, i, &arrIgnoreSpecies, bApplyYN);
		
		
		if( bWorkCodeIgnore == TRUE )
		{
			pCont->SetWorkCode(bTmpData);
			bWorkCodeIgnore = FALSE;
		}

		// ��DM�� CM�� ������ �����Ѵ�.
		pFormApi->MakeDMFromCM(pCM, pDM, i, _T(""), &arrIgnoreSpecies);

		// åDM�� �ִٸ� åDM�� CM�� ������ �����Ѵ�.
		if( pDM_BOOK )
		{
			CMapStringToString tmpMap;	tmpMap.RemoveAll();
			tmpMap.SetAt(strSpeciesKeyAlias, pDM->GetCellData(strSpeciesKeyAlias, i));
			
			if( strVolSortNoAlias.IsEmpty() == FALSE )
				tmpMap.SetAt(strVolSortNoAlias, pDM->GetCellData(strVolSortNoAlias, i));

			nBookIdx = FindIndex(pDM_BOOK, &tmpMap, 0);
			while( nBookIdx >= 0 )
			{
				pFormApi->MakeDMFromCM(pCM, pDM_BOOK, nBookIdx, _T(""), &arrIgnoreSpecies);
				nBookIdx = FindIndex(pDM_BOOK, &tmpMap, nBookIdx+1);
			}
		}

		//=====================================================
		//2009.02.03 UPDATE BY PJW : ���������ڵ尡 ""�ϰ��� ����������� ����.
// 		if( bSpeciesSubjectCode == FALSE && m_strSubjectCode.IsEmpty() == FALSE ) 
		
		//2009.03.27 UPDATE BY LKS : ���������� �������� ���� �� ������ "" �� �� ����������� ����Ǵ� ���� ����
		if( bSpeciesSubjectCode == FALSE && bChangeSubjectCode ) 
		{
			pDM->SetCellData(_T("BS_��������"), m_strSubjectCode, i);
			//20071101 ADD PJW : �ڵ庯��� BP_���� ���� ����
			pDM->SetCellData(_T("BP_��������"), m_strSubjectCode, i);
			m_strSubjectCode.Empty();
		}
		//=====================================================
		
		
		// ���� �����ڷ�� �̵��Ѵ�.
		i = pGrid->SelectGetNext();
	}

	strMsg.Format(_T("�����ڵ尪�� ������ �Դϴ�."));
	m_pDataProcessingDlg->SetMessage(strMsg);

	// Send
	ids = pDM->SendFrame();
	DeleteDataProcessingDlg();
	if( ids < 0 )	return -3000;
	// End
	ids = pDM->EndFrame();
	if( ids < 0 )	return -3000;

	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �� ���̺� �����ڵ带 �����ϴ� ������ �ۼ��Ͽ� �߰��Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ���� ���ο� ���� Head������ �ۼ��Ѵ�.(������-���������̺�, �׿�-�����̺�)
// 2. CatCode�� ���� ������ �ڷ��� �ڷ���� ������ �����ϴ� Tail������ �ۼ��Ѵ�.
// 3. �з�ǥ���а� ���� ���� �߰�
// 4. �������а� ���� ���� �߰�
// 5. �����ڷᱸ�а� ���� ���� �߰�
// 6. �������а� ���� ���� �߰�(�������а� ����� bApply�� TRUE�� ���� ����)
// 7. �ڷᱸ�а� ���� ���� �߰�
// 8. ��ü���а� ���� ���� �߰� (��ü���а� ����� bApplyYN�� TRUE�϶��� ����)
// 9. ������ϱ��а� ���� ���� �߰�
// 10. �̿����ѱ��а� ���� ���� �߰�(����� bApplyYN�� TRUE�϶��� ����)
// 11. �̿��󱸺а� ���� ���� �߰�(����� bApplyYN�� TRUE�϶��� ����)
// 12. �������а� ���� ���� �߰�(����� ���Ա����� �����ϰ�츸 ����)
// 13. �����з��ڵ尪 ���� ���� �߰�
// 14. ������ �߰��� Mid�������� SQL���Ŀ� �°� �߰��Ѵ�.
// 15. �ϼ��������� AddFrame �Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::MakeSpeciesCodeChangeQuery(CESL_DataMgr *pDM, 
												   CESL_ControlMgr *pCM, 
												   CCodeChangeCondition *pCont, 
												   INT idx, 
												   CStringArray * pArrIgnoreSpecies, 
												   BOOL bSpeciesSubjectCode, 
												   BOOL bDeleteData,
												   BOOL bApplyYN)
{
	//================================================================
	// ��������
	//================================================================
	INT cnt, i;
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strQuery;
	CStringArray arrMidQuery;

	//�� �����ڵ庯��

	
	//================================================================
	// 1. ���� ���ο� ���� Head������ �ۼ��Ѵ�.(������-���������̺�, �׿�-�����̺�)
	//================================================================
	if( bDeleteData == TRUE )	strHeadQuery = _T("UPDATE BO_DELETE_SPECIES_TBL SET ");
	else						strHeadQuery = _T("UPDATE BO_SPECIES_TBL SET ");

	//================================================================
	// 2. CatCode�� ���� ������ �ڷ��� �ڷ���� ������ �����ϴ� Tail������ �ۼ��Ѵ�.
	//================================================================
	if( m_bCatCode == FALSE )
	{
		strTailQuery.Format(_T(" WHERE REC_KEY IN ")
							_T("(SELECT REC_KEY FROM IDX_BO_TBL ")							
 							_T("WHERE REC_KEY=%s AND (WORKING_STATUS<'BOC' OR WORKING_STATUS>'BOR'));"),
		pDM->GetCellData(_T("IBS_��KEY"), idx));
	}
	else
	{
		strTailQuery.Format(_T(" WHERE REC_KEY = %s;"), pDM->GetCellData(_T("IBS_��KEY"), idx));
	}


	//================================================================
	// 3. �з�ǥ���а� ���� ���� �߰�
	//================================================================
	if( pCont->GetClassNoType() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_�з�ǥ����"), strTmpData);
		strTmpQuery.Format(_T("CLASS_NO_TYPE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);

		pDM->SetCellData(_T("IBS_�з�ǥ����"), strTmpData, idx);
	}
	else pArrIgnoreSpecies->Add(_T("BS_�з�ǥ����"));

	//================================================================
	// 4. �������а� ���� ���� �߰�
	//================================================================
	if( pCont->GetSubjectCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BP_��������"), strTmpData);
		strTmpQuery.Format(_T("SUBJECT_CODE = '%s'"), strTmpData);
		if(!bSpeciesSubjectCode) m_strSubjectCode = strTmpData;
		arrMidQuery.Add(strTmpQuery);
		bChangeSubjectCode = TRUE;
	}
	else pArrIgnoreSpecies->Add(_T("BP_��������"));


	//================================================================
	// 5. �����ڷᱸ�а� ���� ���� �߰�
	//================================================================
	if( pCont->GetControlCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_�����ڷᱸ��"), strTmpData);
		strTmpQuery.Format(_T("CONTROL_MAT_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_�����ڷᱸ��"));

	//================================================================
	// 6. �������а� ���� ���� �߰�(�������а� ����� bApply�� TRUE�� ���� ����)
	//================================================================
	if( pCont->GetManageCode() == TRUE && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_��������"), strTmpData);
		strTmpQuery.Format(_T("MANAGE_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_��������"));

	//================================================================
	// 7. �ڷᱸ�а� ���� ���� �߰�
	//================================================================
	if( pCont->GetMatCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_�ڷᱸ��"), strTmpData);
		strTmpQuery.Format(_T("MAT_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_�ڷᱸ��"));

	//================================================================
	// 8. ��ü���а� ���� ���� �߰� (��ü���а� ����� bApplyYN�� TRUE�϶��� ����)
	//================================================================
	if( pCont->GetMediaCode() == TRUE && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_��ü����"), strTmpData);
		strTmpQuery.Format(_T("MEDIA_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		//=====================================================
		//2009.01.07 ADD BY PJW : �׸��� DISPLAY�� ���� DM�� �����͵� �������ش�.
		pDM->SetCellData(_T("IBS_��ü����_�ڵ�"), strTmpData, idx);
		pCM->GetControlMgrData(_T("BB_��ü����"), strTmpData,-1,1);
		pDM->SetCellData(_T("IBS_��ü����_����"), strTmpData, idx);

		//=====================================================
		

	}
	else pArrIgnoreSpecies->Add(_T("BS_��ü����"));

	//================================================================
	// 9. ������ϱ��а� ���� ���� �߰�
	//================================================================
	if( pCont->GetSubRegCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_������ϱ���"), strTmpData);
		strTmpQuery.Format(_T("SUB_REG_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		//=====================================================
		//2009.01.07 ADD BY PJW : �׸��� DISPLAY�� ���� DM�� �����͵� �������ش�.
		pDM->SetCellData(_T("IBS_������ϱ���_�ڵ�"), strTmpData, idx);
		//=====================================================
	}
	else pArrIgnoreSpecies->Add(_T("BS_������ϱ���"));

	//================================================================
	// 10. �̿����ѱ��а� ���� ���� �߰�(����� bApplyYN�� TRUE�϶��� ����)
	//================================================================
	if( pCont->GetUseLimitCode() == TRUE && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_�̿����ѱ���"), strTmpData);
		strTmpQuery.Format(_T("USE_LIMIT_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_�̿����ѱ���"));


	//================================================================
	// 11. �̿��󱸺а� ���� ���� �߰�(����� bApplyYN�� TRUE�϶��� ����)
	//================================================================
	if( pCont->GetUseObjCode() == TRUE && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_�̿��󱸺�"), strTmpData);
		strTmpQuery.Format(_T("USE_OBJ_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		//=====================================================
		//2009.01.07 ADD BY PJW : �׸��� DISPLAY�� ���� DM�� �����͵� �������ش�.
		pDM->SetCellData(_T("IBS_�̿��󱸺�_�ڵ�"), strTmpData, idx);
		//=====================================================
	}
	else pArrIgnoreSpecies->Add(_T("BS_�̿��󱸺�"));

	//================================================================
	// 12. �������а� ���� ���� �߰�(����� ���Ա����� �����ϰ�츸 ����)
	//================================================================
	if( pCont->GetWorkCode() == TRUE )
	{
		strTmpData = pDM->GetCellData(_T("BP_���Ա���"), idx);
		if( strTmpData != _T("1") )
		{
			pCM->GetControlMgrData(_T("BS_��������"), strTmpData);
			strTmpQuery.Format(_T("WORK_CODE = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
			//2006-02-06 hjy �߰�: DM�� ���������� �������ش�.
			pDM->SetCellData(_T("IBS_��������"),strTmpData,idx);
		}
	}
	else pArrIgnoreSpecies->Add(_T("BS_��������"));

	//================================================================
	// 13. �����з��ڵ尪 ���� ���� �߰�
	//================================================================
	if( pCont->GetUserDefineCode1() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_����������ڵ�1"), strTmpData);
		strTmpQuery.Format(_T("USER_DEFINE_CODE1 = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_����������ڵ�1"));
	if(pCont->GetUserDefineCode2())
	{
		pCM->GetControlMgrData(_T("BS_����������ڵ�2"), strTmpData);
		strTmpQuery.Format(_T("USER_DEFINE_CODE2 = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_����������ڵ�2"));


	//================================================================
	// 14. ������ �߰��� Mid�������� SQL���Ŀ� �°� �߰��Ѵ�.
	//================================================================
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE )		strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}
	
	//================================================================
	// 15. �ϼ��������� AddFrame �Ѵ�.
	//================================================================
	if( cnt > 0 )
	{
		strTmpData.Format(_T("LAST_WORK='%s'"), m_pParentMgr->GetWorkLogMsg(WORK_LOG));
		strMidQuery += _T(", ") + strTmpData;
		strQuery = strHeadQuery + strMidQuery + strTailQuery;
		pDM->AddFrame(strQuery);
	}


	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ����/�����������̺� ���������ڵ带 �����ϴ� ������ �ۼ��Ͽ� �߰��Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. �Լ������� ���� ���� ���̺��� �����ϴ� Head������ �ۼ��Ѵ�.
// 2. ��������Key���� ������ Tail������ �ۼ��Ѵ�.
// 3. �������а��� �����ϴ� ������ �߰��Ѵ�.
// 4. �ϼ��� ������ AddFrame�Ѵ�.(���Ա��а��� ���ٸ� ��������/�������̺� ��� AddFrame�Ѵ�.)
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::MakeAccuCodeChangeQuery(CESL_DataMgr *pDM,
												CESL_ControlMgr *pCM,
												CCodeChangeCondition *pCont,
												INT nAcqCode, INT idx,
												CStringArray * pArrIgnoreSpecies,
												BOOL bSpeciesSubjectCode
												)
{
	
	//================================================================
	// ��������
	//================================================================
	INT cnt, i;
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strAcqKey, strQuery;
	CStringArray arrMidQuery;

	//�������� �����ڵ庯��
	//================================================================
	// 1. �Լ������� ���� ���� ���̺��� �����ϴ� Head������ �ۼ��Ѵ�.
	//================================================================
	if		( nAcqCode == 1 )	strHeadQuery = _T("UPDATE BO_PURCHASEINFO_TBL SET ");
	else if	( nAcqCode == 2 )	strHeadQuery = _T("UPDATE BO_DONINFO_TBL SET ");
	
	//================================================================
	// 2. ��������Key���� ������ Tail������ �ۼ��Ѵ�.
	//================================================================
	strAcqKey = pDM->GetCellData(_T("BP_��������KEY"), idx);
	if( strAcqKey.IsEmpty() == TRUE )	return 0;	
	strTailQuery.Format(_T(" WHERE REC_KEY = %s;"), strAcqKey);
	
	//================================================================
	// 3. �������а��� �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetSubjectCode() == TRUE )
	{
		if( bSpeciesSubjectCode == TRUE )
		{
			pCM->GetControlMgrData(_T("BP_��������"), strTmpData);
			strTmpQuery.Format(_T("SUBJECT_CODE = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
		}
	}
	else pArrIgnoreSpecies->Add(_T("BP_��������"));

	//================================================================
	// 4. �ϼ��� ������ AddFrame�Ѵ�.(���Ա��а��� ���ٸ� ��������/�������̺� ��� AddFrame�Ѵ�.)
	//================================================================
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE )		strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}
	
	if( cnt > 0 )
	{
		if( strHeadQuery.IsEmpty() == FALSE )
		{
			strQuery = strHeadQuery + strMidQuery + strTailQuery;
			pDM->AddFrame(strQuery);
		}
		else
		{
			if( strAcqKey.IsEmpty() == FALSE ) 
			{
				strQuery = _T("UPDATE BO_PURCHASEINFO_TBL SET ") + strMidQuery + strTailQuery;
				pDM->AddFrame(strQuery);
				strQuery = _T("UPDATE BO_DONINFO_TBL SET ") + strMidQuery + strTailQuery; 
				pDM->AddFrame(strQuery);
			}
		}
	}

	return 0;	
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - å���̺� �����ڵ带 �����ϴ� ������ �ۼ��Ͽ� �߰��Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. �������ο� ���� å/����å���̺��� �����ϴ� Head������ �ۼ��Ѵ�.
// 2. ���� ���������� ����Ǿ� �ִ��� Ȯ���Ͽ� WHERE���� �����ϴ� Tail������ �ۼ��Ѵ�.
// 3. m_bCatCode�� ���� Tail������ �۾������� ������ �߰��Ѵ�.
// 4. ��ϱ����ڵ带 �����ϴ� ������ �߰��Ѵ�. (��Ϲ�ȣ�� �ο����� ���� �ڷ��϶��� ����)
// 5. ��ġ��ȣ�� �����ϴ� ������ �߰��Ѵ�.
// 6. ���������� �����ϴ� ������ �߰��Ѵ�.
// 7. ��ü������ �����ϴ� ������ �߰��Ѵ�.
// 8. �̿����ѱ����� �����ϴ� ������ �߰��Ѵ�.
// 9. �̿��󱸺��� �����ϴ� ������ �߰��Ѵ�.
// 10. �з�ǥ������ �����ϴ� ������ �߰��Ѵ�.
// 11. �ϼ��� �������� AddFrame�Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::MakeBookCodeChangeQuery(CESL_DataMgr *pDM, CESL_ControlMgr *pCM, CCodeChangeCondition *pCont, INT idx, BOOL bSpecies, CStringArray * pArrIgnoreSpecies, BOOL bDeleteData/*=FALSE*/)
{
	//================================================================
	// ��������
	//================================================================
	INT cnt, i;
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strVolSortNo, strQuery;
	CStringArray arrMidQuery;

	//å �����ڵ庯��
	//================================================================
	// 1. �������ο� ���� å/����å���̺��� �����ϴ� Head������ �ۼ��Ѵ�.
	//================================================================
	if( bDeleteData == TRUE )	strHeadQuery = _T("UPDATE BO_DELETE_BOOK_TBL SET ");
	else						strHeadQuery = _T("UPDATE BO_BOOK_TBL SET ");

	//================================================================
	// 2. ���� ���������� ����Ǿ� �ִ��� Ȯ���Ͽ� WHERE���� �����ϴ� Tail������ �ۼ��Ѵ�.
	//================================================================
	if( bSpecies == TRUE )
	{
		strTmpData = pDM->GetCellData(_T("BP_��������KEY"), idx);
		if( strTmpData.IsEmpty() == TRUE )
		{
			strTailQuery.Format(_T(" WHERE SPECIES_KEY = %s"), pDM->GetCellData(_T("IBS_��KEY"), idx));
		}
		else
		{
			strTailQuery.Format(_T(" WHERE SPECIES_KEY = %s AND ACQ_KEY = %s"), 
								pDM->GetCellData(_T("IBS_��KEY"), idx), strTmpData);
		}
	}
	else
	{
		if( m_bVolYN == TRUE )
		{
			strTmpData = pDM->GetCellData(_T("BP_��������KEY"), idx);
			if( strTmpData.IsEmpty() == TRUE )	return 0;

			strVolSortNo = pDM->GetCellData(_T("BB_���Ϸù�ȣ"), idx);
			if( strVolSortNo.IsEmpty() == TRUE )	
			{
				strTailQuery.Format(_T(" WHERE ACQ_KEY=%s"), strTmpData);
			}
			else
			{
				strTailQuery.Format(_T(" WHERE ACQ_KEY=%s AND VOL_SORT_NO=%s"), strTmpData, strVolSortNo);
		}
		}
		else
		{
			strTailQuery.Format(_T(" WHERE REC_KEY = %s"), pDM->GetCellData(_T("BB_åKEY"), idx));
	}
	}
	
	//================================================================
	// 3. m_bCatCode�� ���� Tail������ �۾������� ������ �߰��Ѵ�.
	//================================================================
	if( m_bCatCode == TRUE )	strTailQuery += _T(";");
	else						strTailQuery += _T(" AND (WORKING_STATUS<'BOC' OR WORKING_STATUS>'BOR');");
	

	//================================================================
	// 4. ��ϱ����ڵ带 �����ϴ� ������ �߰��Ѵ�. (��Ϲ�ȣ�� �ο����� ���� �ڷ��϶��� ����)
	//================================================================
	if( pCont->GetRegCode() == TRUE && pDM->GetCellData(_T("BB_��Ϲ�ȣ"), idx).IsEmpty() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_��ϱ���"), strTmpData);
		strTmpQuery.Format(_T("REG_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_��ϱ���_�ڵ�"), strTmpData, idx);
		//====================================================================
		//2009.01.07 ADD BY LKS : �׸����� UDF_����ϱ��а� ���ŵǵ��� ����
		//(�׸��� �̺�Ʈ���� IBS_���ε�ϱ��а��� UDF_����ϱ��п� SET!!)
		pDM->SetCellData(_T("IBS_���ε�ϱ���"), strTmpData, idx);
		//====================================================================
		arrMidQuery.Add(strTmpQuery);		
		//������ȣ�ο����ؿ� ��ϱ����� �����Ǿ� �ִٸ� ������ȣ�� �缳���ؾ� �Ѵ�.
		if( m_cCopyCodeMgr.GetRegCodeCheckYN() == TRUE )	m_bRefreshCopyCode = TRUE;
	}
	else pArrIgnoreSpecies->Add(_T("BB_��ϱ���"));


	//================================================================
	// 5. ��ġ��ȣ�� �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetShelfCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_��ġ��ȣ"), strTmpData);
		strTmpQuery.Format(_T("SEPARATE_SHELF_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_��ġ��ȣ_�ڵ�"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
		//������ȣ�ο����ؿ� ��ġ��ȣ�� �����Ǿ� �ִٸ� ������ȣ�� �缳���ؾ� �Ѵ�.
		if(m_cCopyCodeMgr.GetSeShelfCodeCheckYN()) m_bRefreshCopyCode = TRUE;
	}
	else pArrIgnoreSpecies->Add(_T("BB_��ġ��ȣ"));


	//================================================================
	// 6. ���������� �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetManageCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_��������"), strTmpData);
		strTmpQuery.Format(_T("MANAGE_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_��������_�ڵ�"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BB_��������"));

	//================================================================
	// 7. ��ü������ �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetMediaCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_��ü����"), strTmpData);
		strTmpQuery.Format(_T("MEDIA_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_��ü����_�ڵ�"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BB_��ü����"));


	//================================================================
	// 8. �̿����ѱ����� �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetUseLimitCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_�̿����ѱ���"), strTmpData);
		strTmpQuery.Format(_T("USE_LIMIT_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_�̿����ѱ���_�ڵ�"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BB_�̿����ѱ���"));


	//================================================================
	// 9. �̿��󱸺��� �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetUseObjCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_�̿��󱸺�"), strTmpData);
		strTmpQuery.Format(_T("USE_OBJECT_CODE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_�̿��󱸺�_�ڵ�"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BB_�̿��󱸺�"));


	//================================================================
	// 10. �з�ǥ������ �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetClassNoType() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_�з�ǥ����"), strTmpData);
		strTmpQuery.Format(_T("CLASS_NO_TYPE = '%s'"), strTmpData);
		pDM->SetCellData(_T("BB_�з�ǥ����"), strTmpData, idx);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("BS_�з�ǥ����"));

	//================================================================
	// 11. �ϼ��� �������� AddFrame�Ѵ�.
	//================================================================
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE )		strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}

	strQuery = strHeadQuery + strMidQuery + strTailQuery;
	if(cnt > 0)		pDM->AddFrame(strQuery);

	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ε������̺� �����ڵ带 �����ϴ� ������ �ۼ��Ͽ� �߰��Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ���̺��� �����ϴ� Head������ �ۼ��Ѵ�.
// 2. m_bCatCode�� ���� WHERE���� ����� Tail������ �ۼ��Ѵ�.
// 3. �з�ǥ������ �����ϴ� ������ �߰��Ѵ�.
// 4. ���������� �����ϴ� ������ �ۼ��Ѵ�. (������ �ƴҶ��� ����)
// 5. �ڷᱸ���� �����ϴ� ������ �߰��Ѵ�.
// 6. ������ϱ��б����� �����ϴ� ������ �߰��Ѵ�.
// 7. ��ϱ����� �����ϴ� ������ �߰��Ѵ�.
// 8. �̿����ѱ����� �����ϴ� ������ �߰��Ѵ�.
// 9. ���������� �����ϴ� ������ �߰��Ѵ�.
// 10. ��ġ��ȣ �����ϴ� ������ �߰��Ѵ�.
// 11. ����������ڵ带 �����ϴ� ������ �߰��Ѵ�.
// 12. �̿��󱸺��� �����ϴ� ������ �߰��Ѵ�.(bApplyYN�� TRUE�ϰ�츸 ����)
// 13. ��ü������ �����ϴ� ������ �߰��Ѵ�.(bApplyYN�� TRUE�ϰ�츸 ����)
// 14. �ϼ��� �������� �ۼ��Ͽ� AddFrame �Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::MakeIndexCodeChangeQuery(CESL_DataMgr *pDM, 
												 CESL_ControlMgr *pCM, 
												 CCodeChangeCondition *pCont, 
												 INT idx, 
												 CStringArray * pArrIgnoreSpecies,
												 BOOL bApplyYN)
{
	
	//================================================================
	// ��������
	//================================================================
	INT cnt, i;
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strQuery;
	CStringArray arrMidQuery;

	//���� �����ڵ庯��
	//================================================================
	// 1. ���̺��� �����ϴ� Head������ �ۼ��Ѵ�.
	//================================================================
	strHeadQuery = _T("UPDATE IDX_BO_TBL SET ");
	
	//================================================================
	// 2. m_bCatCode�� ���� WHERE���� ����� Tail������ �ۼ��Ѵ�.
	//================================================================
	if( m_bCatCode == TRUE )
	{
		strTailQuery.Format(_T(" WHERE REC_KEY = %s;"), pDM->GetCellData(_T("IBS_��KEY"), idx));
	}
	else
	{
		strTailQuery.Format(_T(" WHERE REC_KEY = %s AND (WORKING_STATUS<'BOC' OR WORKING_STATUS>'BOR');"), 
							pDM->GetCellData(_T("IBS_��KEY"), idx));
	}

	//================================================================
	// 3. �з�ǥ������ �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetClassNoType() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_�з�ǥ����"), strTmpData);
		strTmpQuery.Format(_T("CLASS_NO_TYPE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T(""));


	//================================================================
	// 4. ���������� �����ϴ� ������ �ۼ��Ѵ�. (������ �ƴҶ��� ����)
	//================================================================
	if( pCont->GetWorkCode() == TRUE )
	{
		strTmpData = pDM->GetCellData(_T("BP_���Ա���"), idx);
		if(strTmpData != _T("1"))
		{
			pCM->GetControlMgrData(_T("BS_��������"), strTmpData);
			strTmpQuery.Format(_T("WORK_CODE = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
		}
	}
	else pArrIgnoreSpecies->Add(_T("IBS_��������"));


	//================================================================
	// 5. �ڷᱸ���� �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetMatCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_�ڷᱸ��"), strTmpData);
		strTmpQuery.Format(_T("MAT_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_�ڷᱸ��"));


	//================================================================
	// 6. ������ϱ��б����� �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetSubRegCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_������ϱ���"), strTmpData);
		strTmpQuery.Format(_T("SUB_REG_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_������ϱ���"));


	//================================================================
	// 7. ��ϱ����� �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetRegCode() == TRUE )
	{
		strTmpQuery.Format(_T("REG_CODE = GET_BO_BOOK_INFO(%s,'REG_CODE')"), pDM->GetCellData(_T("IBS_��KEY"), idx));
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_��ϱ���"));


	//================================================================
	// 8. �̿����ѱ����� �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetUseLimitCode() == TRUE )
	{
		strTmpQuery.Format(_T("USE_LIMIT_CODE = GET_BO_BOOK_INFO(%s,'USE_LIMIT_CODE')"), pDM->GetCellData(_T("IBS_��KEY"), idx));
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_�̿����ѱ���"));


	//================================================================
	// 9. ���������� �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetManageCode() == TRUE )
	{
		strTmpQuery.Format(_T("MANAGE_CODE = GET_BO_BOOK_INFO(%s,'MANAGE_CODE')"), pDM->GetCellData(_T("IBS_��KEY"), idx));
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_��������"));


	//================================================================
	// 10. ��ġ��ȣ �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetShelfCode() == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_��ġ��ȣ"), strTmpData);
		strTmpQuery.Format(_T("IDX_SE_SHELF_CODE = GET_BO_BOOK_INFO(%s,'SEPARATE_SHELF_CODE')"), pDM->GetCellData(_T("IBS_��KEY"), idx));
		arrMidQuery.Add(strTmpQuery);
		if( bApplyYN == TRUE )
		{
			strTmpQuery.Format(_T("SE_SHELF_CODE='%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
		}
	}
	else pArrIgnoreSpecies->Add(_T("IBS_��ġ��ȣ"));

	//================================================================
	// 11. ����������ڵ带 �����ϴ� ������ �߰��Ѵ�.
	//================================================================
	if( pCont->GetUserDefineCode1() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_����������ڵ�1"), strTmpData);
		strTmpQuery.Format(_T("USER_DEFINE_CODE1='%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_����������ڵ�1"));
	if( pCont->GetUserDefineCode2() == TRUE )
	{
		pCM->GetControlMgrData(_T("BS_����������ڵ�2"), strTmpData);
		strTmpQuery.Format(_T("USER_DEFINE_CODE2='%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_����������ڵ�2"));


	//================================================================
	// 12. �̿��󱸺��� �����ϴ� ������ �߰��Ѵ�.(bApplyYN�� TRUE�ϰ�츸 ����)
	//================================================================
	if( pCont->GetUseObjCode() == TRUE  && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_�̿��󱸺�"), strTmpData);
		pDM->SetCellData(_T("IBS_�̿��󱸺�"), strTmpData, idx);
		strTmpQuery.Format(_T("USE_OBJ_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_�̿��󱸺�"));

	//================================================================
	// 13. ��ü������ �����ϴ� ������ �߰��Ѵ�.(bApplyYN�� TRUE�ϰ�츸 ����)
	//================================================================
	if( pCont->GetMediaCode() == TRUE && bApplyYN == TRUE )
	{
		pCM->GetControlMgrData(_T("BB_��ü����"), strTmpData);
		pDM->SetCellData(_T("IBS_��ü����"), strTmpData, idx);
		strTmpQuery.Format(_T("MEDIA_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else pArrIgnoreSpecies->Add(_T("IBS_��ü����"));


	//================================================================
	// 14. �ϼ��� �������� �ۼ��Ͽ� AddFrame �Ѵ�.
	//================================================================
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE )		strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}

	if( cnt > 0 )
	{
		strTmpData.Format(_T("LAST_WORK = '%s'"), m_pParentMgr->GetWorkLogMsg(WORK_LOG));
		strMidQuery += _T(", ") + strTmpData;
		strQuery = strHeadQuery + strMidQuery + strTailQuery;
		pDM->AddFrame(strQuery);
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		pSpeciesApi->BO_INDEX_TEMP_INSERT(pDM, _T("IDX_TMP_BO_TBL"), pDM->GetCellData(_T("IBS_��KEY"), idx), _T("I"));
		//=====================================================
		//ECOSEARCH�� ���ؼ� �������
		if( m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH )
		{
			strTmpQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), pDM->GetCellData(_T("IBS_��KEY"), idx));
			pDM->AddFrame(strTmpQuery);
		}	
	}
		
	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
//
// [ �̴Ͻ��� ]
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::DonateInfoChange()
{
	//================================================================
	// ��������
	//================================================================
	INT ids, i, cnt, nPos;
	CString strDonatorKey, strMsg;
	CArray<INT, INT> idxArray;	idxArray.RemoveAll();
	CESL_DataMgr* pDM		= NULL ;
	CESL_DataMgr* pDM_BOOK	= NULL ;
	CESL_ControlMgr* pCM	= NULL ;
	CESL_Grid* pGrid		= NULL ;
	CDonateInfoChangeCondition* pCont = NULL;
	
	
	
	//================================================================
	// 1. ����� �����͸� �����´�.
	//================================================================
	pDM			= (CESL_DataMgr*)GetTempPtr(_T("DM"));
	pDM_BOOK	= (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	pCM			= (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	pGrid		= (CESL_Grid*)GetTempPtr(_T("Grid"));
	pCont = (CDonateInfoChangeCondition*)GetTempPtr(_T("Condition"));
	if( pDM == NULL || pCM == NULL || pGrid == NULL || pCont == NULL )	return -1000;	

	//================================================================
	// 2. ����� ������Ű���� �����´�.
	//================================================================
	GetTempData(_T("DonatorKey"), strDonatorKey);


	//================================================================
	// 3. �׸��忡 ������ �ڷḸ�� �������� �����Ѵ�.
	//================================================================
	ids = pGrid->SelectMakeList();
	if( ids < 0 )	return -2000;
	i = pGrid->SelectGetHeadPosition();
	if( i < 0 )		return -2000;

	// Progress Ctrl
	ShowDataProcessingDlg();
	strMsg.Format(_T("�����Ͻ� %d���� ���������� �����ϰ� �ֽ��ϴ�."), pGrid->SelectGetCount());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pGrid->SelectGetCount());

	nPos = 0;
	// Start
	pDM->StartFrame();
	while( i >= 0 )
	{
		// Progress Ctrl ++
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nPos);

		MakeDonateInfoChangeQuery(pDM, pDM_BOOK, pCM, pCont, strDonatorKey, i);
		if( pCont->GetRegYn() ) 
		{
			if		( pDM->DB_MGR_ALIAS == _T("DM_���������ڷ����_��") && pCont->GetNRegYn() == 1 )
			{
				idxArray.Add(i);
			}
			else if	( pDM->DB_MGR_ALIAS == _T("DM_�����δ��ڷ����_��") && pCont->GetNRegYn() == 0 )
		{
				idxArray.Add(i);
			}
		}
		i = pGrid->SelectGetNext();
	}
	
	CGroupPair api(m_pParentMgr);
	strMsg.Format(_T("�������� ������ �Դϴ�."));
	m_pDataProcessingDlg->SetMessage(strMsg);
	//ids = api.SendFrame(pDM, MIN_QUERY_SIZE);
	ids = pDM->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -3000;
	ids = pDM->EndFrame();
	if(ids < 0) return -3000;

	cnt = idxArray.GetSize();
	for( i=cnt-1 ; i>=0 ; i-- )
	{
		pDM->DeleteRow(idxArray.GetAt(i));
	}
	
	return 0;
}

INT CDataChangeManager::MakeDonateInfoChangeQuery(CESL_DataMgr *pDM, CESL_DataMgr *pDM_BOOK, CESL_ControlMgr *pCM, CDonateInfoChangeCondition *pCont, CString strDonatorKey, INT idx)
{
	//�������� ����
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strSpeciesQuery, strMarcQuery;
	CStringArray arrMidQuery, arrIgnoreSpecies;
	CString strDonInputDate;

		
	strSpeciesQuery.Empty();	
	strMarcQuery.Empty();
	arrIgnoreSpecies.RemoveAll();

	strHeadQuery = _T("UPDATE BO_DONINFO_TBL SET ");

	strTailQuery.Format(_T(" WHERE REC_KEY = %s"), pDM->GetCellData(_T("BP_��������KEY"), idx));

	if( pCont->GetDonator() )
	{
		pCM->GetControlMgrData(_T("BP_������"), strTmpData);
		pDM->SetCellData(_T("BP_�����ڸ�"), strTmpData, idx);
		pDM->SetCellData(_T("BP_����������KEY"), strDonatorKey, idx);
		/**********************************************************
		 **********************************************************/
		//20080417 ADD BY PJW : �������̺��� INPUT_DATE�� NULL�� ��� ���� �Է����ش�
		pDM->GetCellData(_T("BP_�Է���"), idx, strDonInputDate);
		if( strDonInputDate.IsEmpty() )
		{
			CTime t = CTime::GetCurrentTime();
			strDonInputDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
			pDM->SetCellData(_T("BP_�Է���"), strDonInputDate, idx);
			pDM_BOOK->SetCellData(_T("BP_�Է���"), strDonInputDate, idx);
		}
		/**********************************************************
		 **********************************************************/
		strTmpData.Replace(_T("'"), _T("''"));
		strTmpQuery.Format(_T("DONATOR_NAME = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		if( strDonatorKey.IsEmpty() == TRUE ) strDonatorKey = _T("-1");
		strTmpQuery.Format(_T("DONATOR_KEY = %s"), strDonatorKey);
		arrMidQuery.Add(strTmpQuery);
		/**********************************************************
		 **********************************************************/
		//20080424 ADD BY PJW : �������̺��� �������ڵ� �Բ� UPDATE�Ѵ�.
		strTmpQuery.Format(_T("INPUT_DATE = '%s'"), strDonInputDate);
		arrMidQuery.Add(strTmpQuery);
		/**********************************************************
		 **********************************************************/
		//===================================================
		//2009.05.20 ADD BY LKS : �������� �Ҽ� ���ŵǵ��� ����
		CString sTempQuery,sPositionResult;
		sTempQuery.Format(_T("SELECT POSITION FROM CO_DONATOR_TBL WHERE REC_KEY = %s"),strDonatorKey);
		pDM->GetOneValueQuery(sTempQuery,sPositionResult);
		pDM->SetCellData(_T("DO_�Ҽ�"), sPositionResult, idx);
		//===================================================
		//===================================================
		//2009.05.28 ADD BY LKS : �������� �ּ� ���ŵǵ��� ����
		CString sAddressResult;
		sTempQuery.Format(_T("SELECT ADDRESS FROM CO_DONATOR_TBL WHERE REC_KEY = %s"),strDonatorKey);
		pDM->GetOneValueQuery(sTempQuery,sAddressResult);
		pDM->SetCellData(_T("DO_�ּ�"), sAddressResult, idx);
		//===================================================
	}
	else arrIgnoreSpecies.Add(_T("BP_������"));

	if(pCont->GetRegYn())
	{
		if(pCont->GetNRegYn() == 0)
		{
			strTmpQuery.Format(_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = '%s' WHERE SPECIES_KEY = %s AND ACQ_KEY = %s;"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), _T("BOA211O"), pDM->GetCellData(_T("IBS_��KEY"), idx), pDM->GetCellData(_T("BP_��������KEY"), idx));
			pDM->AddFrame(strTmpQuery);
			arrIgnoreSpecies.Add(_T("BP_�����δ����"));
		}
		else if(pCont->GetNRegYn() == 1)
		{
			strTmpQuery.Format(_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = '%s' WHERE SPECIES_KEY = %s AND ACQ_KEY = %s;"),
				m_pParentMgr->GetWorkLogMsg(WORK_LOG), _T("BOA212O"), pDM->GetCellData(_T("IBS_��KEY"), idx), pDM->GetCellData(_T("BP_��������KEY"), idx));
			pDM->AddFrame(strTmpQuery);
			pCM->GetControlMgrData(_T("BP_�����δ����"), strTmpData);
			strTmpData.Replace(_T("'"), _T("''"));
			strTmpQuery.Format(_T("NO_REG_REASON = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
		}
	}
	else arrIgnoreSpecies.Add(_T("BP_�����δ����"));
	if(pCont->GetRemark())
	{
		pCM->GetControlMgrData(_T("BS_���"), strTmpData);
		if(!strTmpData.IsEmpty())
		{	
			strTmpData.Replace(_T("'"), _T("''"));
			strTmpQuery.Format(_T("REMARK = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
			strSpeciesQuery.Format(
				_T("UPDATE BO_SPECIES_TBL SET REMARK='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_��KEY"), idx));
			pDM->SetCellData(_T("BP_���"), strTmpData, idx);
			pDM->SetCellData(_T("BP_���2"), strTmpData, idx);  // 2005-02-04
		}
	}		
	else arrIgnoreSpecies.Add(_T("BS_���"));
	if(pCont->GetCurrencyCode())
	{
		pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strTmpData);
		strTmpQuery.Format(_T("CURRENCY_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		strTmpData = SetCurrencyCodeToMarc(pDM->GetCellData(_T("BS_MARC"), idx), strTmpData);
		if( strTmpData.IsEmpty() == TRUE )	return -1;
		else
		{
			strTmpData.Replace(_T("'"), _T("''"));
			strMarcQuery.Format(
				_T("UPDATE BO_SPECIES_TBL SET MARC='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_��KEY"), idx));
			pDM->SetCellData(_T("BS_MARC"), strTmpData, idx);
		}
	}
	else arrIgnoreSpecies.Add(_T("BP_ȭ�󱸺�"));

	strMidQuery.Empty();
	INT cnt = arrMidQuery.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strMidQuery.IsEmpty()) strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}
	CString strQuery = strHeadQuery + strMidQuery + strTailQuery + _T(" ;");
	if(cnt > 0)
	{
		pDM->AddFrame(strQuery);
		if(!strSpeciesQuery.IsEmpty()) pDM->AddFrame(strSpeciesQuery);
		if(!strMarcQuery.IsEmpty()) pDM->AddFrame(strMarcQuery);
		pFormApi->MakeDMFromCM(pCM, pDM, idx, _T(""), &arrIgnoreSpecies);
		if(pDM_BOOK)
		{
			CMapStringToString tmpMap;	tmpMap.RemoveAll();
			tmpMap.SetAt(_T("IBS_��KEY"), pDM->GetCellData(_T("IBS_��KEY"), i));

			INT nBookIdx = FindIndex(pDM_BOOK, &tmpMap, 0);
			while(nBookIdx >= 0)
			{
				pFormApi->MakeDMFromCM(pCM, pDM_BOOK, nBookIdx, _T(""), &arrIgnoreSpecies);
				nBookIdx = FindIndex(pDM_BOOK, &tmpMap, nBookIdx+1);
			}
		}
	}
	return 0;
}

INT CDataChangeManager::PurchaseInfoChange()
{
	CPurchaseInfoChangeCondition * pCont = NULL;
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_DataMgr * pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	CESL_ControlMgr * pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	pCont = (CPurchaseInfoChangeCondition*)GetTempPtr(_T("Condition"));
	CString strMsg;

	if(!pDM || !pCM || !pGrid || !pCont) return -1000;
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -2000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -2000;

	ShowDataProcessingDlg();
	strMsg.Format(_T("�����Ͻ� %d���� ���������� �����ϰ� �ֽ��ϴ�."), pGrid->SelectGetCount());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pGrid->SelectGetCount());
	INT nPos = 0;

	pDM->StartFrame();
	while(i >= 0)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nPos);
		MakePurchaseInfoChangeQuery(pDM, pDM_BOOK, pCM, pCont, i);
		i = pGrid->SelectGetNext();
	}
	
	CGroupPair api(m_pParentMgr);
	strMsg.Format(_T("�������� ������ �Դϴ�."));
	m_pDataProcessingDlg->SetMessage(strMsg);
	//ids = api.SendFrame(pDM, MIN_QUERY_SIZE);
	ids = pDM->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -3000;
	ids = pDM->EndFrame();
	if(ids < 0) return -3000;

	//=====================================================================
	//2009.05.28 ADD BY LKS : å���� ����� ��� ������ �����۾��� 
	//					���� å�� ���濡 ���� ���� ������ �����Ѵ�.
	
	if(pCont->GetBookCount())
	{
		pCont->SetCurrencyCode(0);
		pCont->SetDiscountRate(1);
		pCont->SetExchangeRate(0);
		pCont->SetProvider(0);
		pCont->SetRemark(0);
		pCont->SetBookCount(0);

		INT ids = pGrid->SelectMakeList();
		INT i = pGrid->SelectGetHeadPosition();
		pDM->StartFrame();
		while(i >= 0)
		{
			MakePurchaseInfoChangeQuery(pDM, pDM_BOOK, pCM, pCont, i);
			i = pGrid->SelectGetNext();
		}
		ids = pDM->SendFrame();
		if(ids < 0) return -3000;
		ids = pDM->EndFrame();
		if(ids < 0) return -3000;
	}
	//=====================================================================
	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �����������濡 �ش��ϴ� ������ ����� DM�� �籸���Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. HeadQuery�� TailQuery�� �����Ѵ�.
// 2. ����ó üũ
// 3.ȭ�󱸺� �� ȯ��üũ
// 4. ������ üũ
// 5. ���üũ
// 6. 2~5������ üũ�� ���� MidQuery�� �����.
// 7. Head/Mid/Tail Query�� ��� ���Ͽ� �ϼ��� �������� �����.
// 8. MidQuery�� �ִٸ� �ϼ��� �������� �߰��Ѵ�.
// 9. ���üũ�� �ش��ϴ� ������ �ִٸ� �߰��Ѵ�.
// 10. ȭ�󱸺�üũ�� �ش��ϴ� ������ �ִٸ� �߰��Ѵ�.
// 11. CM���κ��� DM�� �籸���Ѵ�.
// 12.
// 13. å��üũ
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::MakePurchaseInfoChangeQuery(CESL_DataMgr *pDM, CESL_DataMgr *pDM_BOOK, CESL_ControlMgr *pCM, CPurchaseInfoChangeCondition *pCont, INT idx)
{
	//================================================================
	// ��������
	//================================================================
	INT cnt, i, nBookIdx ;
	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strSpeciesQuery, strMarcQuery, strQuery, strVolCnt, strBookCnt;
	CString strItemData		= _T("");	// ȭ�󱸺�
	CString strExchangeRate = _T("");	// ȯ��
	CString strDiscountRate = _T("");	// ������	
	CStringArray arrIgnoreSpecies, arrMidQuery;	
	CMapStringToString tmpMap;
	arrIgnoreSpecies.RemoveAll();
	strSpeciesQuery.Empty();	
	strMarcQuery.Empty();
	

	//================================================================
	// 1. HeadQuery�� TailQuery�� �����Ѵ�.
	//================================================================
	strHeadQuery = _T("UPDATE BO_PURCHASEINFO_TBL SET ");
	strTailQuery.Format(_T(" WHERE REC_KEY = %s ;"), pDM->GetCellData(_T("BP_��������KEY"), idx));
	
	//================================================================
	// 2. ����ó üũ
	//================================================================
	if(pCont->GetProvider())
	{
		pCM->GetControlMgrData(_T("BP_����ó��ȣ"), strTmpData);
		strTmpQuery.Format(_T("PROVIDER_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else arrIgnoreSpecies.Add(_T("BP_����ó��ȣ"));


	//================================================================
	// 3.ȭ�󱸺� �� ȯ��üũ
	//================================================================
		pCM->GetControlMgrData(_T("BP_ȭ�󱸺�����"), strItemData );
	if( pCont->GetCurrencyCode() && pDM->GetCellData(_T("BP_ȭ�󱸺�"), idx) == strItemData )
		{
			pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strTmpData);
			strTmpQuery.Format(_T("CURRENCY_CODE = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);

			strTmpData = SetCurrencyCodeToMarc(pDM->GetCellData(_T("BS_MARC"), idx), strTmpData);
		if( strTmpData.IsEmpty() == TRUE )	return -1;
		else
			{
				strTmpData.Replace(_T("'"), _T("''"));
				strMarcQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
					strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_��KEY"), idx));
				pDM->SetCellData(_T("BS_MARC"), strTmpData, idx);
			}

			// ȭ�󱸺а� ȯ���� ���� ó���Ѵ�.
			// 2005-10-12 ������
			//
			if( pCont->GetExchangeRate() )
			{
				strTmpQuery.Format(_T("EXCHANGE_RATE = %s"), m_ExchangeRate);
		arrMidQuery.Add(strTmpQuery);
			// ����� ȯ���� �����Ѵ�.
			strExchangeRate = m_ExchangeRate;
			if( strExchangeRate.IsEmpty() == TRUE )	strExchangeRate = _T("1");
			pDM->SetCellData(_T("BP_ȯ��"), strExchangeRate, idx);
		}
	}		
	else 
	{
		arrIgnoreSpecies.Add(_T("BP_ȭ�󱸺�"));
		arrIgnoreSpecies.Add(_T("BP_ȯ��"));
		// ����ȯ���� �����Ѵ�.
		strExchangeRate = pDM->GetCellData(_T("BP_ȯ��"), idx);		
		if( strExchangeRate.IsEmpty() == TRUE )	strExchangeRate = _T("1");
	}		

	//================================================================
	// 4. ������ üũ �� ȯ���� �������� ���� ���ְ���, ��ǰ���� ����
	//================================================================
	// 2005-02-02
	if( pCont->GetDiscountRate() || pCont->GetExchangeRate() )
	{
		// ���� �������� ������ �ִ´�.
		strDiscountRate = pDM->GetCellData(_T("BP_������"), idx);
		if( strDiscountRate.IsEmpty() == TRUE )	strDiscountRate = _T("0");
		
		// ����� ��쿡�� CM���� ����� ������ ��ü�Ѵ�.
		if( pCont->GetDiscountRate() )
		{
			//�̺κ� �����ؾ���..
			pCM->GetControlMgrData(_T("BP_������"), strDiscountRate);
			if( strDiscountRate.IsEmpty() == TRUE )	strDiscountRate = _T("0");
			strTmpQuery.Format(_T("PRICE_DISCOUNT_RATE = %s"), strDiscountRate);
			arrMidQuery.Add(strTmpQuery);
		}
		else	arrIgnoreSpecies.Add(_T("BP_������"));

		// ���ְ��ݰ� ��ǰ������ Update�Ѵ�.
		strTmpQuery.Format(_T("ORDER_PRICE = GetSpeciesPrice(%s, %s, NULL, %s)"), pDM->GetCellData(_T("BP_��������KEY"), idx), strDiscountRate, strExchangeRate);
		arrMidQuery.Add(strTmpQuery);
		strTmpQuery.Format(_T("DELIVERY_PRICE = GetSpeciesPrice(%s, %s, 'Y', %s)"), pDM->GetCellData(_T("BP_��������KEY"), idx), strDiscountRate, strExchangeRate);
		arrMidQuery.Add(strTmpQuery);
	
	}
	else	arrIgnoreSpecies.Add(_T("BP_������"));
	
	//================================================================
	// 5. ���üũ
	//================================================================
	if(pCont->GetRemark())
	{
		pCM->GetControlMgrData(_T("BS_���"), strTmpData);
		strTmpData.Replace(_T("'"), _T("''"));
		strTmpQuery.Format(_T("REMARK = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		strSpeciesQuery.Format( _T("UPDATE BO_SPECIES_TBL SET REMARK = '%s', LAST_WORK='%s' WHERE REC_KEY = %s;"),
			strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_��KEY"), idx));
		//=====================================================
		//2008.11.19 UPDATE BY PJW : BP_���,BP_���2�� DM���� �������� ���� -> BS_���
// 		pDM->SetCellData(_T("BP_���"), strTmpData, idx);
// 		pDM->SetCellData(_T("BP_���2"), strTmpData, idx);  // 2005-02-04
		pDM->SetCellData(_T("BS_���"), strTmpData, idx);
		//=====================================================
		INT nDmRowCnt;
		CString strBookSpeciesKey;
		CString strSpeciesKey;
		nDmRowCnt = pDM_BOOK->GetRowCount();
 		strSpeciesKey = pDM->GetCellData(_T("IBS_��KEY"),idx);
 		for( INT i = 0 ; i < nDmRowCnt ; i++ )
 		{
			strBookSpeciesKey = pDM_BOOK->GetCellData(_T("IBS_��KEY"),i);
			if( strSpeciesKey == strBookSpeciesKey )
			{
				pDM_BOOK->SetCellData(_T("BS_���"), strTmpData, i);
			}
			
 		}
	}
	else arrIgnoreSpecies.Add(_T("BS_���"));

	//================================================================
	// 6. ��ó üũ 20070605 ADD PJW : ��ó �������� 
	//================================================================
	if(pCont->GetApplicant())
	{
		pCM->GetControlMgrData(_T("BP_��ó��"), strTmpData);
		strTmpQuery.Format(_T("DEPT_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
	}
	else arrIgnoreSpecies.Add(_T("BP_��ó��"));
	
	//================================================================
	// 7. 2~6������ üũ�� ���� MidQuery�� �����.
	//================================================================
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE ) strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}


	//================================================================
	// 8. Head/Mid/Tail Query�� ��� ���Ͽ� �ϼ��� �������� �����.
	//================================================================
	strQuery = strHeadQuery + strMidQuery + strTailQuery;


	//================================================================
	// 9. MidQuery�� �ִٸ� �ϼ��� �������� �߰��Ѵ�.
	// 10. ���üũ�� �ش��ϴ� ������ �ִٸ� �߰��Ѵ�.
	// 11. ȭ�󱸺�üũ�� �ش��ϴ� ������ �ִٸ� �߰��Ѵ�.
	// 12. CM���κ��� DM�� �籸���Ѵ�.
	// 13.
	//================================================================
	if(cnt > 0)
	{
		//9.
		pDM->AddFrame(strQuery);

		//10.
		if( strSpeciesQuery.IsEmpty() == FALSE )	pDM->AddFrame(strSpeciesQuery);

		//11.
		if( strMarcQuery.IsEmpty() == FALSE )		pDM->AddFrame(strMarcQuery);

		//12.
		pFormApi->MakeDMFromCM(pCM, pDM, idx, _T(""), &arrIgnoreSpecies);
		
		//13.
		if( pDM_BOOK )
		{
			tmpMap.RemoveAll();
			//=====================================================
			//2008.11.19 UPDATE BY PJW : ��DM�� INDEX�� idx�̴�.
// 			tmpMap.SetAt(_T("IBS_��KEY"), pDM->GetCellData(_T("IBS_��KEY"), i));
			tmpMap.SetAt(_T("IBS_��KEY"), pDM->GetCellData(_T("IBS_��KEY"), idx));
			//=====================================================
			
			nBookIdx = FindIndex(pDM_BOOK, &tmpMap, 0) ;
			while( nBookIdx >= 0 )
			{
				pFormApi->MakeDMFromCM(pCM, pDM_BOOK, nBookIdx, _T(""), &arrIgnoreSpecies);
				nBookIdx = FindIndex(pDM_BOOK, &tmpMap, nBookIdx+1);
			}
		}
	}

	//================================================================
	// 14. å��üũ
	// 2005-01-27 �۽¹�
	// �ܱǺ��� ��쿡�� å���� �����Ѵ�.
	//================================================================
	if( pCont->GetBookCount() )
	{
		strQuery.Format(_T("SELECT COUNT(DISTINCT(VOL_SORT_NO)) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s"), pDM->GetCellData(_T("IBS_��KEY"), idx));
		pDM->GetOneValueQuery(strQuery, strVolCnt);
		if(_ttoi(strVolCnt) == 1)
		{
			pCM->GetControlMgrData(_T("BB_å��"), strBookCnt);
			//��Key, �Լ�����,�����⵵,��������,����,å����,���ϴ�å��
			strQuery.Format(_T("ECO$P_SET_BOBOOK(%s, '1', '%s', '%s', %s, '%s', %s);"),
							pDM->GetCellData(_T("IBS_��KEY"), idx),
							pDM->GetCellData(_T("BP_���Գ⵵"), idx),
							pDM->GetCellData(_T("IBS_��������"), idx),
							pDM->GetCellData(_T("BP_�׷��ȣ"), idx),
							pDM->GetCellData(_T("UDF_�ڷ���¼���"), idx),
							strBookCnt);
			pDM->SetCellData(_T("UDF_å��"), strBookCnt, idx);  // 2005-01-29
//DEL			pDM->SetCellData(_T("BP_����å��"), strBookCnt, idx);  // 2005-01-29
							
			pDM->AddFrame(strQuery);
		}
	}

	return 0;
}

INT CDataChangeManager::BookCodeChange()
{		
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_ControlMgr * pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	CCodeChangeCondition * pCont = (CCodeChangeCondition*)GetTempPtr(_T("Condition"));
	CString strMsg, strTmpData;
	if(!pDM || !pCM || !pCont || !pGrid) return -1000;

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -2000;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -2000;

	ShowDataProcessingDlg();
	strMsg.Format(_T("�����Ͻ� %d���� �ڷḦ �ڵ庯���ϰ� �ֽ��ϴ�."), pGrid->SelectGetCount());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pGrid->SelectGetCount());
	INT nPos = 0;
	
	CStringArray arrIgnoreSpecies;
	arrIgnoreSpecies.RemoveAll();
	
	pDM->StartFrame();
	while(i >= 0)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nPos);
		strTmpData = pDM->GetCellData(_T("BB_��ǥå����"), i);
		if(strTmpData == _T("Y"))
			MakeSpeciesCodeChangeQuery(pDM, pCM, pCont, i, &arrIgnoreSpecies, FALSE, FALSE, false);
		MakeBookCodeChangeQuery(pDM, pCM, pCont, i, FALSE, &arrIgnoreSpecies);
		MakeIndexCodeChangeQuery(pDM, pCM, pCont, i, &arrIgnoreSpecies, false);
		pFormApi->MakeDMFromCM(pCM, pDM, i, _T(""), &arrIgnoreSpecies);
		
		i = pGrid->SelectGetNext();
	}
	CGroupPair api(m_pParentMgr);
	strMsg.Format(_T("�����ڵ尪�� ������ �Դϴ�."));
	m_pDataProcessingDlg->SetMessage(strMsg);
	//ids = api.SendFrame(pDM, MIN_QUERY_SIZE);
	ids = pDM->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -3000;
	ids = pDM->EndFrame();
	if(ids < 0) return -3000;

	return 0;	
}

INT CDataChangeManager::GetLatestExchangeRate()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_ControlMgr * pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	if(!pDM || !pCM) return -1000;
	
	CString tmpstr, strExchangeRate;
	pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), tmpstr);
	if(tmpstr.IsEmpty()) strExchangeRate = _T("");
	else
	{
		/********************************************************************
		 ********************************************************************/
		//20080131 UPDATE BY PJW : ȭ�󱸺��� \�̸� APPLY_YN�� N �̴��� ���� �ü� �ִ�.
		CString query;
		CString query1;
		query1.Format(
			_T("SELECT exchange_rate FROM co_exchange_rate_tbl ")
			_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND user_code_mng_key in (SELECT rec_key FROM cd_code_tbl WHERE code LIKE '%s%%') ")
			// �ֱ����� ��ſ� ���뿩�θ� ������ ���� �����´�.
			// 2005-09-28 ������
			//\
			//"AND input_date = "
			//"(SELECT MAX (input_date) FROM co_exchange_rate_tbl "
			//"WHERE user_code_mng_key in (SELECT rec_key FROM cd_code_tbl WHERE code LIKE '%s%%')) "
			_T("AND INPUT_DATE >= (SELECT TRUNC(SYSDATE, 'YEAR') FROM DUAL) ")
			_T("AND INPUT_DATE < (SELECT ADD_MONTHS(TRUNC(SYSDATE, 'YEAR'), 12) FROM DUAL)")
			,
			tmpstr,
			tmpstr
			);
	
		if( tmpstr == _T("\\") )
		{
			query.Format( _T("%s") , query1);
		}
		else
		{
			query.Format( _T("%s AND APPLY_YN = 'Y'") , query1);
		}

		/*
		query.Format(
			_T("SELECT exchange_rate FROM co_exchange_rate_tbl ")
			_T("WHERE user_code_mng_key in (SELECT rec_key FROM cd_code_tbl WHERE code LIKE '%s%%') ")
			// �ֱ����� ��ſ� ���뿩�θ� ������ ���� �����´�.
			// 2005-09-28 ������
			//
			//_T("AND input_date = ")
			//_T("(SELECT MAX (input_date) FROM co_exchange_rate_tbl ")
			//_T("WHERE user_code_mng_key in (SELECT rec_key FROM cd_code_tbl WHERE code LIKE '%s%%')) ")
			_T("AND APPLY_YN = 'Y' ")
			_T("AND INPUT_DATE >= (SELECT TRUNC(SYSDATE, 'YEAR') FROM DUAL) ")
			_T("AND INPUT_DATE < (SELECT ADD_MONTHS(TRUNC(SYSDATE, 'YEAR'), 12) FROM DUAL)")
			,
			tmpstr,
			tmpstr
			);
		*/
		/********************************************************************
		 ********************************************************************/
		
		pDM->GetOneValueQuery(query, strExchangeRate);
		if(strExchangeRate.IsEmpty())		return -2000;
	}
	SetTempData(_T("ExchangeRate"), strExchangeRate.GetBuffer(0));
	return 0;
}

VOID CDataChangeManager::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(m_pParentMgr);

	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(m_pParentMgr))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//���α׷��� ��Ʈ�� �ʱ�ȭ
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CDataChangeManager::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

INT CDataChangeManager::MakeDM(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;

	INT ids, nCnt, nDMRefreshCnt;
	nCnt = m_MapSpeciesKey.GetCount();
	if(nCnt == 0) return 0;
		
	nDMRefreshCnt = nCnt/800;
	if(nCnt%800 > 0) nDMRefreshCnt++;
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nDMRefreshCnt);

	nCnt = 0;
	CString strKeyList, strKey, strValue, strQuery;
	CESL_DataMgr * pTmpDM = NULL;
	POSITION pos = m_MapSpeciesKey.GetStartPosition();
	while( pos != NULL )
	{
		if(nCnt == 800)
		{
			m_pDataProcessingDlg->GetProgressCtrl()->StepIt();
			pTmpDM = MakeDMClone(pDM);
			if(pTmpDM == NULL) return -1;
			strQuery.Format(_T("SPECIES_KEY IN (%s)"), strKeyList);
			ids = MakeDM(pTmpDM, strQuery);
			if(ids < 0) return -1;
			m_ArrDM.Add(pTmpDM);
			nCnt = 0;
			strKeyList.Empty();
		}
		m_MapSpeciesKey.GetNextAssoc(pos, strKey, strValue);
		if(strKeyList.IsEmpty()) strKeyList = strKey;
		else strKeyList += _T(", ") + strKey;
		nCnt++;
	}

	m_pDataProcessingDlg->GetProgressCtrl()->StepIt();
	pTmpDM = MakeDMClone(pDM);
	if(pTmpDM == NULL) return -1;
	strQuery.Format(_T("SPECIES_KEY IN (%s)"), strKeyList);
	ids = MakeDM(pTmpDM, strQuery);
	if(ids < 0) return -1;
	m_ArrDM.Add(pTmpDM);
	
	m_pDataProcessingDlg->SetMessage(_T("����� å������ ���� ���Դϴ�."));
	
	ids = MergeDMArray(pDM);
	if(ids < 0) return -1;

	return 0;
}

CESL_DataMgr * CDataChangeManager::MakeDMClone(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return NULL;
	CESL_DataMgr * pTmpDM = new CESL_DataMgr;
	if(pTmpDM == NULL) return NULL;
	pTmpDM->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	pTmpDM->InitDMPKFromFile(pDM->DB_MGR_ALIAS);
	pTmpDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	return pTmpDM;
}

INT CDataChangeManager::MakeDM(CESL_DataMgr *pDM, CString strQuery)
{
	if(pDM == NULL) return -1;
	if(strQuery.IsEmpty()) return -1;
	INT ids = pDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	return 0;
}

INT CDataChangeManager::MergeDMArray(CESL_DataMgr *pMergeDM)
{
	if(pMergeDM == NULL) return -1;
	INT ids;
	INT nCnt = m_ArrDM.GetSize();
	if(nCnt == 0) return 0;
	CESL_DataMgr * pTmpDM = NULL;
	for(INT i=0 ; i<nCnt ; i++)
	{
		pTmpDM = m_ArrDM.GetAt(i);
		if(pTmpDM == NULL) continue;
		ids = AddToDM(pMergeDM, pTmpDM);
	}
	return 0;
}

INT CDataChangeManager::AddToDM(CESL_DataMgr *pTargetDM, CESL_DataMgr *pSourceDM)
{
	if(pTargetDM == NULL || pSourceDM == NULL) return -1;
	INT ids, nCnt, nColCnt, nCurIdx;
	nCnt = pSourceDM->GetRowCount();
	if(nCnt == 0) return 0;
	
	nColCnt = pSourceDM->GetRefColumnCount();
	if(nColCnt == 0) return -1;
	CString field_alias, field_name, field_type, data_type, strData;

	for(INT i=0 ; i<nCnt ; i++)
	{
		ids = pTargetDM->InsertRow(-1);
		if(ids < 0) continue;
		nCurIdx = pTargetDM->GetRowCount()-1;
		for(INT j=0 ; j<nColCnt ; j++)
		{
			ids = pSourceDM->FindColumnFieldName(j, field_alias, field_name, field_type, data_type, i, strData);
			if(ids < 0) continue;
			pTargetDM->SetCellData(field_alias, strData, nCurIdx);
		}
	}
	return 0;
}


INT CDataChangeManager::DeleteDMArray()
{
	INT nCnt = m_ArrDM.GetSize();
	if(nCnt == 0) return 0;
	CESL_DataMgr * pTmpDM = NULL;
	for(INT i=0 ; i<nCnt ; i++)
	{
		pTmpDM = m_ArrDM.GetAt(i);
		if(pTmpDM)
		{
			delete pTmpDM;
			pTmpDM = NULL;
		}
	}
	m_ArrDM.RemoveAll();
	return 0;
}

INT CDataChangeManager::RefreshBookCopyCode(CESL_DataMgr *pDM_OUT, CESL_ControlMgr * pCM, CCodeChangeCondition *pCont)
{
	if(pDM_OUT == NULL || pCM == NULL || pCont == NULL) return -1;
	CESL_DataMgr * pBookDM = m_pParentMgr->FindDM(_T("DM_������ȣ���ſ�DM"));
	if(pBookDM == NULL) return -1;

	m_pDataProcessingDlg->SetMessage(_T("������ȣ�� �����ϱ� ���� å������ �������Դϴ�."));
	INT ids = MakeDM(pBookDM);
	if(ids < 0) return -1;

	CString strTmpData;
	INT nCnt = pBookDM->GetRowCount();
	for(INT i=0 ; i<nCnt ; i++)
	{
		if(pCont->GetRegCode()) 
		{
			pCM->GetControlMgrData(_T("BB_��ϱ���"), strTmpData);
			pBookDM->SetCellData(_T("BB_��ϱ���_�ڵ�"), strTmpData, i);
		}
		if(pCont->GetShelfCode())
		{
			pCM->GetControlMgrData(_T("BB_��ġ��ȣ"), strTmpData);
			pBookDM->SetCellData(_T("BB_��ġ��ȣ_�ڵ�"), strTmpData, i);
		}
	}
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];
	
	strAlias[nCondColumns] = _T("BB_��KEY");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	strAlias[nCondColumns] = _T("BB_������ȣ");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	pBookDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);	

	CString strSpeciesKey;
	INT nStart, nSCnt, nNext;
	nStart = nSCnt = nNext = 0;
	
	m_pDataProcessingDlg->SetMessage(_T("������ȣ�� �������Դϴ�."));
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nCnt);

	while(nCnt > nStart)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(nStart);
		pBookDM->GetCellData(_T("BB_��KEY"), nStart, strSpeciesKey);
		nSCnt = FindSameSpeciesKeyCnt(pBookDM, &nNext, nCnt, _T("BB_��KEY"), strSpeciesKey);
		ids = RefreshCopyCode(pBookDM, nStart, nStart+nSCnt);
		nStart = nNext;
	}
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.Add(_T("BB_åKEY"));
	for(i=0 ; i<nCnt ; i++)
	{
		strTmpData = pBookDM->GetCellData(_T("UDF_����YN"), i);
		if(strTmpData != _T("Y")) continue;
		strTmpData = pBookDM->GetCellData(_T("BB_�ڷ����"), i);
		if(strTmpData.Left(2) == _T("BOC") || strTmpData.Left(2) == _T("BOL")) continue;
		strTmpData = pBookDM->GetCellData(_T("BB_å�η��÷���"), i);
		if(strTmpData != _T("B")) continue;
		
		strTmpData = pBookDM->GetCellData(_T("BB_åKEY"), i);
		if(!strTmpData.IsEmpty())
		{
			ids = pBookDM->MakeQueryByDM(RemoveAliasList, pBookDM, pBookDM->TBL_NAME, i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
			if(ids < 0) continue;
		}
	}

	return 0;
}

INT CDataChangeManager::FindSameSpeciesKeyCnt(CESL_DataMgr * pBook_DM, INT *nStart, INT nRowCnt, CString strDivAlias, CString strSpeciesKey)
{
	INT i = *nStart;
	i++;
	INT cnt = 0;
	CString strTmpData;
	while(i < nRowCnt)
	{
		pBook_DM->GetCellData(strDivAlias, i, strTmpData);
		if(strTmpData == strSpeciesKey)
		{
			cnt++;
			i++;
		}
		else break;
	}
	*nStart = i;
	return cnt;
}

INT CDataChangeManager::RefreshCopyCode(CESL_DataMgr *pBookDM, INT nStartIdx, INT nEndIdx)
{
	if(pBookDM == NULL) return -1;
	if(nEndIdx == nStartIdx) return 0;
	
	CString strRegCode, strSeShelfCode, strVol, strTmpData;
	INT nLastCopyCode;
	for(INT i=nStartIdx ; i<=nEndIdx ; i++)
	{
		strRegCode = pBookDM->GetCellData(_T("BB_��ϱ���_�ڵ�"), i);
		strSeShelfCode = pBookDM->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), i);
		strVol = pBookDM->GetCellData(_T("BB_�����"), i);
		m_cCopyCodeMgr.SetSrcInfo(strVol, strRegCode, strSeShelfCode);
		nLastCopyCode = pSpeciesApi->GetLastDupCodeNumber(pBookDM, &m_cCopyCodeMgr, i, nStartIdx, nEndIdx, _T("-1"));
		if(nLastCopyCode == 0) strTmpData = _T("");
		else strTmpData.Format(_T("%d"), ++nLastCopyCode);
		pBookDM->SetCellData(_T("BB_������ȣ"), strTmpData, i);
		pBookDM->SetCellData(_T("BB_�������۾�����"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		pBookDM->SetCellData(_T("UDF_����YN"), _T("Y"), i);
	}
	return 0;
}

CString CDataChangeManager::SetCurrencyCodeToMarc(CString strMarc, CString strCurrencyCode)
{
	INT ids;
	CString strPriceInfo;
	CMarc marc;
	CMarcInfoConversionMgr mgr;

	if( strMarc.IsEmpty() == TRUE )		return _T("");
	
	mgr.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	if( ids < 0 )	return _T("");

	ids = mgr.GetPriceInfo(&marc, strPriceInfo);
	if( ids < 0 )	return _T("");

	strPriceInfo = strCurrencyCode+strPriceInfo;
	ids = m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, _T("950$b"));
	// 2005.11.07 ������ ����
	// 950�±׿� ���� ������ ����ǵ��� ����
	//if( ids < 0 )	return _T("");
	ids = m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, _T("950$b"), strPriceInfo);
	if( ids < 0 )	return _T("");
	ids = m_pParentMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if( ids < 0 )	return _T("");

	return strMarc;
}

// ������������
INT CDataChangeManager::AcqInfoChange()
{
	INT ids, i, nPos;
	CString strMsg;	

	CESL_DataMgr* pDM		= (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_DataMgr* pDM_BOOK	= (CESL_DataMgr*)GetTempPtr(_T("BOOK_DM"));
	CESL_ControlMgr* pCM	= (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	CESL_Grid* pGrid		= (CESL_Grid*)GetTempPtr(_T("Grid"));
	
	CDonateInfoChangeCondition* pCont = NULL;
	pCont = (CDonateInfoChangeCondition*)GetTempPtr(_T("Condition"));
	
	if( pDM == NULL || pCM == NULL || pGrid == NULL || pCont == NULL )	return -1000;
	
	ids = pGrid->SelectMakeList();
	if(ids < 0) return -2000;
	//i = pGrid->SelectGetHeadPosition();
	i = pGrid->SelectGetTailPosition();
	if(i < 0) return -2000;
	
	ShowDataProcessingDlg();
	if( m_pDataProcessingDlg != NULL )
	{
	strMsg.Format(_T("�����Ͻ� %d���� ���������� �����ϰ� �ֽ��ϴ�."), pGrid->SelectGetCount());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pGrid->SelectGetCount());
	}
	
	nPos = 0;	
	pDM->StartFrame();
	while( i >= 0 )
	{
		if( m_pDataProcessingDlg != NULL )		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nPos);		
		MakeAcqInfoChangeQuery(pDM, pDM_BOOK, pCM, pCont, i);
		
		i = pGrid->SelectGetPrev();
	}
	
	//CGroupPair api(m_pParentMgr);
	if( m_pDataProcessingDlg != NULL )
	{
	strMsg.Format(_T("�������� ������ �Դϴ�."));
	m_pDataProcessingDlg->SetMessage(strMsg);
	}
	//ids = api.SendFrame(pDM, MIN_QUERY_SIZE);
	ids = pDM->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -3000;
	ids = pDM->EndFrame();
	if(ids < 0) return -3000;
		
	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���ڷο� DM�� ���ε� ���� pMap�� �˸��ƽ��� �����Ͱ� ������� DM�� Row��ȣ�� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. for���� DM�� Row��ŭ Ž���Ѵ�. -> ���η� Ž���Ѵ�.
// 2. while���� pMap�� �˸��ƽ��� �����͸� ���η� Ž���Ѵ�.
// 3. ���� Ʋ���ٸ� �����ο츦 �˻��Ѵ�.
// 4. DM���� pMap ������ ������ ��� ã�Ҵٸ� DM�� Row��ȣ�� �����Ѵ�.
// 5. DM���� �ᱹ ã�����ߴٸ� -1�� �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CDataChangeManager::FindIndex(CESL_DataMgr *pDM, CMapStringToString * pMap, INT nIdx/*=0*/)
{
	if( pMap->GetCount() == 0 )			return -1;
	if( nIdx > pDM->GetRowCount()-1 )	return -1;

	//================================================================
	// ��������
	//================================================================
	INT nRowCnt , i ;
	CString strTmpData, strAlias, strData;
	BOOL bSameFlag ;

	//================================================================
	// 1. for���� DM�� Row��ŭ Ž���Ѵ�. -> ���η� Ž���Ѵ�.
	// 2. while���� pMap�� �˸��ƽ��� �����͸� ���η� Ž���Ѵ�.
	// 3. ���� Ʋ���ٸ� �����ο츦 �˻��Ѵ�.
	// 4. DM���� pMap ������ ������ ��� ã�Ҵٸ� DM�� Row��ȣ�� �����Ѵ�.
	// 5. DM���� �ᱹ ã�����ߴٸ� -1�� �����Ѵ�.
	//================================================================	
	nRowCnt = pDM->GetRowCount();	
	// 1.
	for( i=nIdx ; i<nRowCnt ; i++ )
	{
		bSameFlag = TRUE;
		POSITION pos = pMap->GetStartPosition();
		// 2.
		while( pos != NULL )
		{
			pMap->GetNextAssoc(pos, strAlias, strData);
			strTmpData = pDM->GetCellData(strAlias, i);
			// 3.
			if( strTmpData != strData )	
			{
				bSameFlag = FALSE;
				break;
			}
		}
		
		// 4.
		// ������ pos���� ���� Ʋ����쿡�� �´ٰ� üũ�ϰ� ������ ���� ���� �ϵ��� ����
		// 2005.10.26 ������ bSameFlag �߰� 
		if( pos == NULL && bSameFlag == TRUE )	return i;
	}

	// 5.
	return -1;
}
//20070611 UPDATE PJW : CString strDonatorKey
INT CDataChangeManager::MakeAcqInfoChangeQuery(CESL_DataMgr *pDM, CESL_DataMgr *pDM_BOOK, CESL_ControlMgr *pCM, CDonateInfoChangeCondition *pCont, INT idx)
{
	//�������� ����
	CESL_Grid* pGrid		= (CESL_Grid*)GetTempPtr(_T("Grid"));

	CString strHeadQuery, strTailQuery, strTmpQuery, strTmpData, strMidQuery, strSpeciesQuery, strMarcQuery, strQuery;	
	CStringArray arrMidQuery, arrIgnoreSpecies;
	CString strDonatorKey;
	
	strSpeciesQuery.Empty();	
	strMarcQuery.Empty();
	arrIgnoreSpecies.RemoveAll();

	// ���濡 ���� ����	
	CString strAcqKey;
	CString strOldAcqKey;
	CString strSpecieskey;
	CString strAcqYear;
	CString strWorkNo;
	CString strWorkCode;
	CString strSerialNo;
	CString strSubjectCode;
	CString strBookKey;
	CString strProvider;
	CString strDonName;
	CString strAcqCode;

	CString strOneValueQuery;
	CString strInputDate;
	
	// 1. ���Ա��� ������ ��� ���Ա����� üũ�Ͽ� �������� �ʴ´�.
	GetTempData(_T("BP_���Ա���"), strAcqCode);
	if ( strAcqCode == _T("") )
	{
		// ���Ա��� üũ		
		pDM->GetCellData(_T("BP_��������KEY"), idx, strOldAcqKey);
		GetTempData(_T("BP_����ó"), strProvider);	
		
		strTmpData = pDM->GetCellData(_T("BB_���Ա���"), idx);
		if(strTmpData == _T("1") && strProvider != _T("") )
		{
			strQuery.Format(_T("UPDATE BO_PURCHASEINFO_TBL SET PROVIDER_CODE='%s' WHERE REC_KEY=%s;"),
				strProvider, strOldAcqKey );
			pDM->AddFrame(strQuery);
		}
		//20070611 ADD PJW : ������ �˻��߰�
		// �����ڰ˻�
		// ����� ������Ű���� �����´�.	
		else if(strTmpData == _T("2"))
		{
			GetTempData(_T("DonatorKey"), strDonatorKey);
			if( pCont->GetDonator() )
			{
				pCM->GetControlMgrData(_T("BP_������"), strDonName);
				strDonName.Replace(_T("'"), _T("''"));	
				strTmpQuery.Format(_T("DONATOR_NAME = '%s'"), strTmpData);
				arrMidQuery.Add(strTmpQuery);
				if( strDonatorKey.IsEmpty() == TRUE ) strDonatorKey = _T("-1");	
				strTmpQuery.Format(_T("DONATOR_KEY = %s"), strDonatorKey);
				arrMidQuery.Add(strTmpQuery);
				
				/**********************************************************
				 **********************************************************/
				//20080417 UPDATE BY PJW : �������̺��� INPUT_DATE�� NULL�� ��� ���� �Է����ش�.
				//strQuery.Format(_T("UPDATE BO_DONINFO_TBL SET DONATOR_KEY='%s', DONATOR_NAME='%s' WHERE REC_KEY=%s;"),
				//	strDonatorKey, strDonName, strOldAcqKey );
				strOneValueQuery.Format(_T("SELECT INPUT_DATE FROM BO_DONINFO_TBL %s"), strTailQuery);
				pDM->GetOneValueQuery(strOneValueQuery,strInputDate);
				if( strInputDate.IsEmpty() )
				{
					CTime t = CTime::GetCurrentTime();
					strInputDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
					strTmpQuery.Format(_T("INPUT_DATE = '%s'"), strInputDate);
					arrMidQuery.Add(strTmpQuery);
					
				}
				else
				{
					strQuery.Format(_T("UPDATE BO_DONINFO_TBL SET DONATOR_KEY='%s', DONATOR_NAME='%s' WHERE REC_KEY=%s;"),
						strDonatorKey, strDonName, strOldAcqKey );
				}
				/**********************************************************
				 **********************************************************/
				
				
				pDM->AddFrame(strQuery);
			}
		}
		else 
		{
			strHeadQuery.Empty();
			strTailQuery.Empty();
		}	
	}
	else
	{	
		pDM->MakeRecKey(strAcqKey);
		pDM->GetCellData(_T("BP_��������KEY"), idx, strOldAcqKey);
		pDM->GetCellData(_T("IBS_��KEY"), idx, strSpecieskey);
		pDM->GetCellData(_T("BS_��������"), idx, strSubjectCode);
		pDM->GetCellData(_T("BB_åKEY"), idx, strBookKey);		
		GetTempData(_T("BP_���Գ⵵"), strAcqYear);
		
		// �������� ����
		if ( strAcqCode == _T("1") )
		{
			GetTempData(_T("BP_����ó"), strProvider);	
			GetTempData(_T("BS_��������"), strWorkCode);
			GetTempData(_T("BP_�׷��ȣ"), strSerialNo);
			GetTempData(_T("BP_�۾���ȣ"), strWorkNo);
			// ��ġ����� ����� ������ ������ ������Ʈ
			strQuery.Format(_T("UPDATE BO_FURNISH_TBL SET ACQ_KEY=%s WHERE REC_KEY=%s;"), strAcqKey, strOldAcqKey );
			pDM->AddFrame(strQuery);
			// ���� �������� ����
			strQuery.Format(_T("DELETE FROM BO_PURCHASEINFO_TBL WHERE REC_KEY=%s;"), strOldAcqKey );
			pDM->AddFrame(strQuery);			
			// �������� ����
			strQuery.Format(_T("INSERT INTO BO_PURCHASEINFO_TBL ( REC_KEY, SPECIES_KEY, ORDER_YEAR, ORDER_SEQ_NO, ORDER_WORK_NO, SUBJECT_CODE, PROVIDER_CODE, INPUT_DATE, INPUT_TYPE, MANAGE_CODE ) VALUES ( %s, %s, '%s', '%s', '%s', '%s', '%s', SYSDATE, '0', UDF_MANAGE_CODE );"),
//DEL 			strQuery.Format(_T("INSERT INTO BO_PURCHASEINFO_TBL ( REC_KEY, SPECIES_KEY, ORDER_YEAR, ORDER_SEQ_NO, ORDER_WORK_NO, SUBJECT_CODE, PROVIDER_CODE, INPUT_DATE, INPUT_TYPE ) VALUES ( %s, %s, '%s', '%s', '%s', '%s', '%s', SYSDATE, '0' );"),
				strAcqKey, strSpecieskey, strAcqYear, strSerialNo, strWorkNo, strSubjectCode, strProvider );
			pDM->AddFrame(strQuery);
			// å���� ���Ա���, �������� ������Ʈ
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ACQ_CODE='1', ACQ_KEY=%s, SEQ_NO=%s WHERE REC_KEY=%s;"), strAcqKey, strSerialNo, strBookKey );
			pDM->AddFrame(strQuery);
			//======================================================
			//20080624 ADD BY PJW : ����ó�� �ٲ�� �������е� �ٲ��ش�.
			strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET WORK_CODE='%s' WHERE REC_KEY=%s;"), strWorkCode, strSpecieskey );
			pDM->AddFrame(strQuery);
			strQuery.Format(_T("UPDATE IDX_BO_TBL SET WORK_CODE='%s' WHERE REC_KEY=%s;"), strWorkCode, strSpecieskey );
			pDM->AddFrame(strQuery);
			//======================================================
			pDM->DeleteRow(idx);
			//=================================================
			//20080612 UPDATE BY PJW : DM�� �ϳ��� ������� �׸��� ROW������ �����ϱ� ���ؼ� �׸��带 DISPLY�Ѵ�.
			if( 1 < pDM->GetRowCount() )
			{
				pGrid->RemoveAt(idx+1);
			}
			else
			{
				pGrid->Display();
			}
			//=================================================			
		}
		else if ( strAcqCode == _T("2") )
		{				
			GetTempData(_T("DonatorKey"), strDonatorKey);
			GetTempData(_T("BP_�׷��ȣ"), strSerialNo);

			// ��ġ����� ����� ������ ������ ������Ʈ
			strQuery.Format(_T("UPDATE BO_FURNISH_TBL SET ACQ_KEY=%s WHERE REC_KEY=%s;"), strAcqKey, strOldAcqKey );
			pDM->AddFrame(strQuery);
			
			//========================���� to ����========================================================
			CString strTemp,strOldDonatorKey,strOldDonatorName;
			strTemp.Format(_T("SELECT DONATOR_KEY FROM BO_DONINFO_TBL WHERE REC_KEY=%s"),strOldAcqKey);
			pDM->GetOneValueQuery(strTemp, strOldDonatorKey);
			if(!strOldDonatorKey.IsEmpty())
			{
				strTemp.Format(_T("SELECT DONATOR_NAME FROM BO_DONINFO_TBL WHERE REC_KEY=%s"),strOldAcqKey);
				pDM->GetOneValueQuery(strTemp, strOldDonatorName);
			}
			///////////////////////////////////////////////////////////////////////////////////////////////

			// ���� �������� ����
			strQuery.Format(_T("DELETE FROM BO_DONINFO_TBL WHERE REC_KEY=%s;"), strOldAcqKey );
			pDM->AddFrame(strQuery);			

			strQuery = _T("INSERT INTO BO_DONINFO_TBL ( REC_KEY, SPECIES_KEY, ACQ_YEAR, RECEIPT_NO, SUBJECT_CODE, INPUT_DATE, INPUT_TYPE");
			// �������� ����
			if( pCont->GetDonator() || (!strOldDonatorKey.IsEmpty()))
			//if( pCont->GetDonator())
			{
				if(!strOldDonatorKey.IsEmpty())
				{
					strDonName = strOldDonatorName;
					strDonName.Replace(_T("'"), _T("''"));		
					strDonatorKey = strOldDonatorKey;
				}
				else
				{
					pCM->GetControlMgrData(_T("BP_������"), strDonName);
					strDonName.Replace(_T("'"), _T("''"));				
					
					if( strDonatorKey.IsEmpty() == TRUE ) strDonatorKey = _T("-1");		
				}
					/*pCM->GetControlMgrData(_T("BP_������"), strDonName);
					strDonName.Replace(_T("'"), _T("''"));				
					
					if( strDonatorKey.IsEmpty() == TRUE ) strDonatorKey = _T("-1");		*/

				strTmpQuery.Format(_T(", DONATOR_KEY, DONATOR_NAME , MANAGE_CODE) VALUES ( %s, %s, '%s', '%s', '%s', SYSDATE, '0', %s, '%s', UDF_MANAGE_CODE );"),
					strAcqKey, strSpecieskey, strAcqYear, strSerialNo, strSubjectCode, strDonatorKey, strDonName );
				strQuery += strTmpQuery;
				pDM->AddFrame(strQuery);

			}			
			else
			{
				strTmpQuery.Format(_T(",MANAGE_CODE) VALUES ( %s, %s, '%s', '%s', '%s', SYSDATE, '0' ,UDF_MANAGE_CODE);"),
					strAcqKey, strSpecieskey, strAcqYear, strSerialNo, strSubjectCode );
				strQuery += strTmpQuery;
				pDM->AddFrame(strQuery);
			}		
			// å���� ������Ʈ
			// 17/01/18 ������ : ����>���>����ڷ������ ������������ ���Ա��� �� ������ ����� ���� ����
/*//BEFORE-----------------------------------------------------------------------------
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ACQ_CODE='2', ACQ_KEY=%s, SEQ_NO=%s WHERE REC_KEY=%s;"), strAcqKey, strSerialNo, strBookKey );
*///AFTER------------------------------------------------------------------------------
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ACQ_CODE='2', ACQ_KEY=%s, SEQ_NO='%s' WHERE REC_KEY=%s;"), strAcqKey, strSerialNo, strBookKey );
/*//END-------------------------------------------------------------------------------*/			
			pDM->AddFrame(strQuery);			
			pDM->DeleteRow(idx);
			
			//20090327 UPDATE BY LKS : DM�� �ϳ��� ������� �׸��� ROW������ �����ϱ� ���ؼ� �׸��带 DISPLY�Ѵ�.
			if( 1 < pDM->GetRowCount() )
			{
				pGrid->RemoveAt(idx+1);
			}
			else
			{
				pGrid->Display();
			}
		}
		//===================================================
		//2009.10.20 ADD BY PJW : ����,�����̿��� ���Ա����ϰ���� ó�� �߰���
		else
		{				
			GetTempData(_T("BP_�׷��ȣ"), strSerialNo);

			// ��ġ����� ����� ������ ������ ������Ʈ
			strQuery.Format(_T("UPDATE BO_FURNISH_TBL SET ACQ_KEY=%s WHERE ACQ_KEY=%s;"), strAcqKey, strOldAcqKey );
			pDM->AddFrame(strQuery);

			// ���� �������� ����
//			strQuery.Format(_T("DELETE FROM BO_DONINFO_TBL WHERE REC_KEY=%s;"), strOldAcqKey );
//			pDM->AddFrame(strQuery);				
			// å���� ������Ʈ
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET ACQ_CODE='%s', ACQ_KEY=%s, SEQ_NO=%s WHERE REC_KEY=%s;"), strAcqCode, strAcqKey, strSerialNo, strBookKey );
			pDM->AddFrame(strQuery);			
			pDM->DeleteRow(idx);
			
			//20090327 UPDATE BY LKS : DM�� �ϳ��� ������� �׸��� ROW������ �����ϱ� ���ؼ� �׸��带 DISPLY�Ѵ�.
			if( 1 < pDM->GetRowCount() )
			{
				pGrid->RemoveAt(idx+1);
			}
			else
			{
				pGrid->Display();
			}
		}
		//===================================================
	}

	// ��� üũ
	if( pCont->GetRemark() )
	{
		pCM->GetControlMgrData(_T("BS_���"), strTmpData);
		if( strTmpData.IsEmpty() == FALSE )
		{	
			strTmpData.Replace(_T("'"), _T("''"));
			strTmpQuery.Format(_T("REMARK = '%s'"), strTmpData);
			strSpeciesQuery.Format(_T("UPDATE BO_SPECIES_TBL SET REMARK='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_��KEY"), idx));
			//=============================================
			//2008.07.25 ADD BY PJW : ��� ��DM�� �߰��Ѵ�.
			pDM->SetCellData(_T("BS_���"), strTmpData, idx);
			//=============================================
			pDM->SetCellData(_T("BP_���"), strTmpData, idx);
			pDM->SetCellData(_T("BP_���2"), strTmpData, idx);  // 2005-02-04
			pDM->AddFrame(strSpeciesQuery);
		}
	}			

	// ȭ�󱸺� üũ
	if( pCont->GetCurrencyCode() )
	{
		pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strTmpData);
		
		//===================================================
		//2009.03.24 ADD BY LKS : ����â�� ȭ�󱸺��� �ݿ��ȵǾ� �߰�
		CString strTmpQuery,strAcqKey;
		pDM->GetCellData(_T("BP_��������KEY"), idx, strAcqKey);
		strTmpQuery.Format(_T("UPDATE BO_PURCHASEINFO_TBL SET CURRENCY_CODE='%s' WHERE REC_KEY=%s;"),strTmpData,strAcqKey);
		//�����ڷ���������� BP_��������KEY �� ����.
		if(!strAcqKey.IsEmpty())
			pDM->AddFrame(strTmpQuery);
		//===================================================

		strTmpQuery.Format(_T("CURRENCY_CODE = '%s'"), strTmpData);		
		strTmpData = SetCurrencyCodeToMarc(pDM->GetCellData(_T("BS_MARC"), idx), strTmpData);
		if( strTmpData.IsEmpty() == TRUE )	return -1;
		else
		{
			strTmpData.Replace(_T("'"), _T("''"));
			strMarcQuery.Format(_T("UPDATE BO_SPECIES_TBL SET MARC='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_��KEY"), idx));
			
			pDM->SetCellData(_T("BS_MARC"), strTmpData, idx);
			pDM->AddFrame(strMarcQuery);
		}
	}


	//
	// =======================================================================
	// 

	//20070611 ADD PJW : ������ �˻��߰�
	// �����ڰ˻�
	// ����� ������Ű���� �����´�.
		/*
	if(strTmpData == _T("2"))
	{
		GetTempData(_T("DonatorKey"), strDonatorKey);
		if( pCont->GetDonator() )
		{
			pCM->GetControlMgrData(_T("BP_������"), strTmpData);
			strTmpData.Replace(_T("'"), _T("''"));
			strTmpQuery.Format(_T("DONATOR_NAME = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);
			if( strDonatorKey.IsEmpty() == TRUE ) strDonatorKey = _T("-1");
			strTmpQuery.Format(_T("DONATOR_KEY = %s"), strDonatorKey);
			arrMidQuery.Add(strTmpQuery);
		}
		else arrIgnoreSpecies.Add(_T("BP_������"));
	}
	
	// ��� üũ
	if( pCont->GetRemark() )
	{
		pCM->GetControlMgrData(_T("BS_���"), strTmpData);
		if( strTmpData.IsEmpty() == FALSE )
		{	
			strTmpData.Replace(_T("'"), _T("''"));
			strTmpQuery.Format(_T("REMARK = '%s'"), strTmpData);
			arrMidQuery.Add(strTmpQuery);

			strSpeciesQuery.Format(_T("UPDATE BO_SPECIES_TBL SET REMARK='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
				strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_��KEY"), idx));

			pDM->SetCellData(_T("BP_���"), strTmpData, idx);
			pDM->SetCellData(_T("BP_���2"), strTmpData, idx);  // 2005-02-04
		}
	}		
	else arrIgnoreSpecies.Add(_T("BS_���"));

	// ȭ�󱸺� üũ
	if( pCont->GetCurrencyCode() )
	{
		pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strTmpData);
		strTmpQuery.Format(_T("CURRENCY_CODE = '%s'"), strTmpData);
		arrMidQuery.Add(strTmpQuery);
		strTmpData = SetCurrencyCodeToMarc(pDM->GetCellData(_T("BS_MARC"), idx), strTmpData);
		if( strTmpData.IsEmpty() == TRUE )	return -1;
		else
		{
		strTmpData.Replace(_T("'"), _T("''"));
			strMarcQuery.Format(_T("UPDATE BO_SPECIES_TBL SET MARC='%s', LAST_WORK='%s' WHERE REC_KEY=%s;"),
			strTmpData, m_pParentMgr->GetWorkLogMsg(WORK_LOG), pDM->GetCellData(_T("IBS_��KEY"), idx));

		pDM->SetCellData(_T("BS_MARC"), strTmpData, idx);
	}
	}
	else arrIgnoreSpecies.Add(_T("BP_ȭ�󱸺�"));


	
	// Mid������  �߰�
	strMidQuery.Empty();
	cnt = arrMidQuery.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		if( strMidQuery.IsEmpty() == TRUE )		strMidQuery = arrMidQuery.GetAt(i);
		else strMidQuery += _T(", ") + arrMidQuery.GetAt(i);
	}
	

	// �ϼ��� ������ �ۼ�
	if( strHeadQuery.IsEmpty() == FALSE )	strQuery = strHeadQuery + strMidQuery + strTailQuery + _T(" ;");
	else strQuery.Empty();
	
	if( cnt > 0 )
	{
		if( strQuery.IsEmpty() == FALSE )			pDM->AddFrame(strQuery);
		if( strSpeciesQuery.IsEmpty() == FALSE )	pDM->AddFrame(strSpeciesQuery);
		if( strMarcQuery.IsEmpty() == FALSE )		pDM->AddFrame(strMarcQuery);

		pFormApi->MakeDMFromCM(pCM, pDM, idx, _T(""), &arrIgnoreSpecies);
		if( pDM_BOOK )
		{
			CMapStringToString tmpMap;	tmpMap.RemoveAll();
			tmpMap.SetAt(_T("IBS_��KEY"), pDM->GetCellData(_T("IBS_��KEY"), i));
			
			INT nBookIdx = FindIndex(pDM_BOOK, &tmpMap, 0);
			while(nBookIdx >= 0)
			{
				pFormApi->MakeDMFromCM(pCM, pDM_BOOK, nBookIdx, _T(""), &arrIgnoreSpecies);
				nBookIdx = FindIndex(pDM_BOOK, &tmpMap, nBookIdx+1);
			}
		}
	}
	*/

	return 0;
}
