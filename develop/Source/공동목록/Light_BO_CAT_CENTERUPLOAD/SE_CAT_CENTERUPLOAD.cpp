// SE_CAT_CENTERUPLOAD.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_CENTERUPLOAD.h"
#include "..\..\�������\check_dup_api\rs_sql.h"
#include "..\..\include\�������\CU_ComApi.h"

#include "..\CenterUpload_api\CenterUpload.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_CENTERUPLOAD dialog


CSE_CAT_CENTERUPLOAD::CSE_CAT_CENTERUPLOAD(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCENTER_UPLOAD_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	 m_SE_CENTERUPLOAD = new CSE_CENTERUPLOAD_API(this) ;

	 // KOL.UDF.022 ��ť���ڵ� ����
	 m_pDMAlias = _T("");
	 m_pDupDMAlias = _T("");
	 m_pParentCM = _T("");
	 m_pBaseDataMgr = NULL;
	 m_pDupDataMgr = NULL;
}

CSE_CAT_CENTERUPLOAD::~CSE_CAT_CENTERUPLOAD()
{

   delete m_SE_CENTERUPLOAD ;

}

BOOL CSE_CAT_CENTERUPLOAD::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CSE_CAT_CENTERUPLOAD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_CENTERUPLOAD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_CENTERUPLOAD, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_CENTERUPLOAD)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_CENTERUPLOAD message handlers
VOID CSE_CAT_CENTERUPLOAD::SetDMAlias (CString BasicDM,CString DupDM,CString pParentCM) 
{
	m_pDMAlias      = BasicDM ;
	m_pDupDMAlias   = DupDM ;
	m_pParentCM     = pParentCM ;

}


INT CSE_CAT_CENTERUPLOAD::CenterUpload (bool m_bMarcSyntaxCheck)
{

    INT ids, i;
    INT iTotalCnt = 0;
    INT iSelectedCnt = 0;
    INT iFinishedCnt = 0;
    INT iDupMatCnt = 0;
    INT iSyntaxErrorCnt = 0;
    INT iEtcErrorCnt = 0;
    INT iCurIndex = 0;
	INT iSpeciesCnt = 0 ;
    CString sFlag;
    CString lib_code;
    CString frm;



	//DataMgr �ʱ�ȭ+++++++++++++++++++++++++++++++++
    m_pBaseDataMgr = FindDM(m_pDMAlias ) ;
	if(m_pBaseDataMgr == NULL){
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1 ;
	}
	
    m_pDupDataMgr = FindDM(m_pDupDMAlias  ) ;
	if(m_pDupDataMgr == NULL){
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1 ;
	}

    CRS_SQL m_RS_SQL(this) ;
    ids  = m_RS_SQL.GetLibraryInfo (lib_code) ;
    
	CCenterUpload Check_Upload_api(this) ;
	CString str_DIRECT_UPLOAD_YN ;
	Check_Upload_api.Check_Direct_Upload (str_DIRECT_UPLOAD_YN) ;

	iTotalCnt = m_pBaseDataMgr->GetRowCount () ;

	for ( i=0; i < iTotalCnt; i++ ) {
		
        iCurIndex = i ;	
		if(str_DIRECT_UPLOAD_YN == _T("N"))
		{   
			CString strSpeciesKey ;
			CString strMarc ;

			// 2012.11.26 ADD BY PWR : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (�����ȣ�߰�)
			CString strControlNo;
            
			m_pBaseDataMgr->GetCellData (_T("REC_KEY"),iCurIndex,strSpeciesKey) ;
			m_pBaseDataMgr->GetCellData (_T("MARC"),iCurIndex,strMarc) ;

			// 2012.11.26 ADD BY PWR : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (�����ȣ�߰�)
			m_pBaseDataMgr->GetCellData (_T("�����ȣ"), iCurIndex, strControlNo);
		
			ids = Check_Upload_api.Input_Temp_Mater_tbl (strSpeciesKey, strMarc, _T(""), strControlNo, _T("") , _T("") , _T(""));

			if(ids < 0 )
			{
				ids = UpdateUploadStatus( this, m_pDMAlias, iCurIndex, _T("4") );
				if (ids<0) AfxMessageBox (_T("UpdateUploadStatus(4) Error!!!"), FALSE);
				
				iEtcErrorCnt ++ ;
				iFinishedCnt ++ ;
				continue;
			}
			ids = UpdateUploadStatus( this, m_pDMAlias, iCurIndex, _T("0") );
			iSpeciesCnt++ ;
			
			iFinishedCnt ++ ;
			continue ;

		}
		else 
		{			
			AfxMessageBox(_T("�ش� ������ ��ȣ�� ������� ������ ��ȣ�� ��ϵǾ� ���� �ʽ��ϴ�.")) ;
			return -1;	
		}

    }

	if(str_DIRECT_UPLOAD_YN == _T("N"))
	{
		ids = Check_Upload_api.Update_Temp_Lib_Statistics_Tbl(iSpeciesCnt,_T("U")) ;
	}


     
	CString Msg ;
	CString temp ;
	Msg.Format( _T("�� %d���� �ڷ� �߿��� %d���� ���ͷ� ���ε� �Ǿ����ϴ�.\n\n"), iFinishedCnt,iSpeciesCnt);
    if ( iDupMatCnt > 0 ) {
        temp.Format( _T("Ȯ���� ���ϴ��ڷ� : %d��\n"), iDupMatCnt );
        Msg += temp;
    }
    if ( iSyntaxErrorCnt > 0 ) {
        temp.Format( _T("MARC �����ڷ�     : %d��\n"), iSyntaxErrorCnt );
        Msg += temp;
    }
    if ( iEtcErrorCnt > 0 ) {
        temp.Format( _T("��Ÿ �����ڷ�     : %d��\n"), iEtcErrorCnt );
        Msg += temp;
    }
    if(iDupMatCnt > 0 || iSyntaxErrorCnt > 0 || iEtcErrorCnt > 0 )
	{
		INT errCnt ;
		errCnt = iDupMatCnt + iSyntaxErrorCnt + iEtcErrorCnt ;

		CString tmpCnt ;
		tmpCnt.Format (_T("%d"),errCnt);
	    Msg += tmpCnt + _T("���� UPLOAD���� �ʾҽ��ϴ�. �Ǻ��� �۾��Ͻʽÿ�.");	
		
	}

    AfxMessageBox(Msg) ; 

	return 0 ;
}

