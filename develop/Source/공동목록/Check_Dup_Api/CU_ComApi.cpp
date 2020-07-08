c//
// ���� ���ε��� ������
// - LOCAL���ε�� �������� �� ��쿡�� ����MASTER�� UPDATE�ϴ� ��찡 �߻�
//

#include "stdafx.h"
#include "..\..\include\�������\CU_ComAPI.h"
#include   "..\..\include\�������\search_struct.h"
#include "..\..\include\�������\MarcConversion.h"
//#include _T("ViewUploadMarc.h")
#include "check_dup.h"
#include "..\..\esl\marc\Marcedit\MarcEditor.h"
#include "SearchMake.h"
#include "ViewUploadMarc.h"
#include "RS_SQL.h" 

//#include "..\\..\\�������\\RS_COMMON\\RS_DM_Mgr.h"

// ����/�ʷ� ��� ��뿩�� ����
//#define __TOC_PROCESS__
#undef __TOC_PROCESS__

//
#define TEMP_UB_NO  _T("UB00000000000")
#define TEMP_UH_NO  _T("UH00000000000")

//2004-02-11 ������ 
#define STRING 1 
#define NUMBER 2


//
#define UPLOAD_INTERVAL_TIME  (1000*20)  // 10 sec.

// +++ private function
INT _GetControlNo( CESL_Mgr* pMgr, CESL_DataMgr* pDataMgr, CString f_icCentMatCode, CString f_icYear, CString &pControlNo );
INT _MakeLocalControlNoList( TCHAR* control_no, TCHAR* control_no_list );
INT _TocProcess( CESL_Mgr* pMgr, CString &sDispAlias, CString &sDBAlias, INT iRowIndex, CString &ub_control_no, CString *sDupDBAlias = NULL, INT iDupRowIndex = -1 );
// ---

//
TCHAR g_BibMarc[100000];
TCHAR g_HoldingMarc[100000];
TCHAR g_LocalMarc[100000];
TCHAR g_TempMarc[100000];

CString test_g_BibMarc ;


// for compare previous marc ( only used in file & batch upload )
BOOL g_bCheckInterval = FALSE;
// 
UINT g_bWaited        = 0;
TCHAR g_PrevMarc[100000];

//
// CU_ComApi���� �Լ��� �������ڷ� DataMgr�� REC_KEY�� ALIAS�� �����ϰ����Ҷ� ����Ѵ�. ( default = _T("REC_KEY") )
TCHAR g_sRecKeyAlias[40] = _T("REC_KEY");

// LOCAL �����ڷῡ UPLOAD���¸� UPDATE�Ѵ�.
// ���ǻ��� : �Լ����ο��� COMMIT������.
INT UpdateUploadStatus( CESL_Mgr* pMgr, CString &sDBAlias, INT iRowIndex, CString sUploadStatus )
{

    INT  ids;
	CString species_key;
    CESL_DataMgr *pDataMgr;

	pDataMgr = pMgr->FindDM (sDBAlias);
    if(pDataMgr == NULL)
	{
		AfxMessageBox(_T("FindDM() Fail")) ;
		return -1 ;
	}
	
	CString tmp_g_sRecKeyAlias ;
	tmp_g_sRecKeyAlias.Format (_T("%s"),g_sRecKeyAlias);
	
	ids = pDataMgr->GetCellData (tmp_g_sRecKeyAlias.GetBuffer(0),iRowIndex,species_key);
	if (ids) return -3;

	pDataMgr->StartFrame ();
	pDataMgr->InitDBFieldData();
	pDataMgr->AddDBFieldData( _T("CENTER_UPLOAD_STATUS"), _T("STRING"), sUploadStatus );
    ids = pDataMgr->MakeUpdateFrame (_T("BO_SPECIES_TBL"),_T("REC_KEY"),_T("NUMERIC"),species_key,-1);
	if (ids<0) return -4;
    
	ids = pDataMgr->SendFrame ();
	ids = pDataMgr->EndFrame ();
    return 0;  

}

INT UpdateBTPStatisticsInfo( CESL_Mgr* pMgr, CString sDispAlias, CString sDBAlias, INT SpeciesCnt,CString sUpFlag ,CString sClientType )
{

	CESL_DataMgr* pDM = pMgr->FindDM(sDBAlias);
	if(pDM == NULL)
	{
		AfxMessageBox(_T("UpdateStatisticsInfo FindDM() Fail"));
		return -1 ;
	}

	CString strRecKey;
	pDM->GetCellData(_T("REC_KEY"), 0, strRecKey);

	CString strSpeciesCount;
	strSpeciesCount.Format(_T("%d"), SpeciesCnt);

	if (!CKolisNetService::MakeStatiticsBtp(pMgr, strRecKey, pMgr->m_pInfo->LIB_CODE, sClientType, _T("A"), sUpFlag, CKolisNetService::GetSysdate(), strSpeciesCount))
	{
		AfxMessageBox(_T("UpdateStatisticsInfo KOLIS-NET OPEN API Fail"));
		return -1;
	}

    return 0;
}