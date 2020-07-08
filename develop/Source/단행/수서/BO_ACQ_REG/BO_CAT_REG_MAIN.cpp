// BO_CAT_REG_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_REG_MAIN.h"
#include "../../����/BO_CAT_REG/BO_CAT_REG_0400.h"
#include "../../����/BO_CAT_REGNO_GIVE\BO_CAT_REGNO_GIVE1.h"
#include "../../����/BO_CAT_REGNO_CANCEL/BO_CAT_RegNoCancel.h"

#include "../../../����/BO_ACQ_SPECIES_FORM/Species_Form.h"
#include "../../../����/BO_ACQ_API/BO_ACQ_Business_Api.h"
#include "../../../����/FileImport/FileImportDlg.h"
#include "../../../����/BO_ACQ_COMMON/BO_ACQ_DataSortDlg.h"
#include "..\..\..\����\MarcAdjustApi\CatApi.h"
#include "../../../����/BO_DATA_CHANGE_MANAGER/BookCodeChange.h"
#include "..\..\..\SmartID\BarCodeToRFID2\SmartIDScannerDlg.h"
#include "..\..\..\SmartID\BarCodeToRFID2\BarcodeToRFIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_MAIN dialog


CBO_CAT_REG_MAIN::CBO_CAT_REG_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBO_CAT_REG_MAIN::~CBO_CAT_REG_MAIN()
{

}

VOID CBO_CAT_REG_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_MAIN, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_MAIN)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+1980, OnRFID)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_MAIN message handlers

BOOL CBO_CAT_REG_MAIN::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}
BOOL CBO_CAT_REG_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_���_����")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	if(pGrid) pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	
	pGrid->SetSort(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_REG_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(-2, 35, cx + 4, cy-35);	
}

INT CBO_CAT_REG_MAIN::CreateRegNO()
{

	INT idx;
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		return FALSE;			
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG error") );
		return FALSE;
	}

	CString sTmpBookKey,sCodeTmp;
	
	sCodeTmp = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );
	sTmpBookKey = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );

	if(sCodeTmp != _T("BOR111N"))
	{
		AfxMessageBox(_T("�̹� ��ϵ� �ڷᰡ �ֽ��ϴ�."));	// ����
		return FALSE;			
	}

	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {

		sCodeTmp = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );
		CString sRecKeyTmp = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
		
		if(sCodeTmp != _T("BOR111N"))
		{
			AfxMessageBox(_T("�̹� ��ϵ� �ڷᰡ �ֽ��ϴ�."));	// ����
			return FALSE;			
		}

		if ( sRecKeyTmp != _T("") ) {
			sTmpBookKey = sTmpBookKey + _T(", ") + sRecKeyTmp;
		}
		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	if ( sTmpBookKey == _T("") ) {
		BO_CAT_REGNO_GIVE dlg(this, _T(""));
		dlg.m_strParentCMAlias= _T("CM_����_����_���_����");
		dlg.m_strParentDMAlias = _T("DM_����_����_���_����");
		dlg.m_nOpenMode =1;
		dlg.DoModal();
		INT iCount = dlg.m_aBookKey.GetSize();
	}
	else {
		AfxMessageBox( _T("��Ϲ�ȣ �ο��� �ڷᰡ �ֽ��ϴ�.") );
	}
	
	return 0;


}

VOID CBO_CAT_REG_MAIN::CancelGivenRegNo()
{
	INT idx,ids;
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	if(idx < 0)
	{
		//����ó��
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return ;
	}

	CString BookKeyList,sRecKeyTmp,sRegNo,sTmpRegNo;
	sRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );

	if (sRegNo != _T(""))
	{
		BookKeyList = pDM->GetCellData( _T("BB_åKEY") , idx );
	}

	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		sTmpRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );

		if (sTmpRegNo != _T(""))
		{
			sRecKeyTmp = pDM->GetCellData( _T("BB_åKEY") , idx );
			BookKeyList = BookKeyList + _T(", ") + sRecKeyTmp;
		}

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	if(BookKeyList.GetLength() == 0)
	{
		ESLAfxMessageBox(_T("��Ϲ�ȣ�� �ο��� �ڷᰡ �����ϴ�."));
		return;
	}

	if(IDCANCEL == ESLAfxMessageBox(_T("�����Ͻ� �ڷῡ �ο��� ��Ϲ�ȣ�� �����Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return;

	CBO_CAT_RegNoCancel dlg(this);
	ids = dlg.RegNoCancel(BookKeyList);

	if(ids < 0)
		ESLAfxMessageBox(_T("��Ϲ�ȣ�� ������ �� �����ϴ�."));
	else
		ESLAfxMessageBox(_T("��Ϲ�ȣ�� �����Ͽ����ϴ�."));

//	CString GridAlias = GetGridCtrlAlias(m_tabACQ.GetCurSel(), m_IsSpecies);
//	ids = m_pPurchaseMgr->UpdateDMbyRegNoRelativeField(_T("CM_LIGHT_����_����"), GridAlias, m_tabACQ.GetCurSel(), m_IsSpecies);
//	if(ids < 0) return ;

	DisplayGrid(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	return;	
}

VOID CBO_CAT_REG_MAIN::DisplayGrid(CString cmAlias, CString gridAlias)
{
	CESL_Grid * pGrid = (CESL_Grid*)FindControl(cmAlias, gridAlias);
	if(!pGrid) return;
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	pGrid->Display();
}

INT CBO_CAT_REG_MAIN::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
	CString strAcqType, strAcqYear, strGroupNo, strTmpData;
	INT nGroupNo;
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));

    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_�ڷ����_�ڵ�")) {
            if ( str == _T("BOR111N")) {
				str = _T("����ΰ��ڷ�");
			}
			else if ( str == _T("BOR112N") ) {
				str = _T("����ڷ�");
			}
			else if ( str == _T("BOR113O") ) {
				str = _T("�������ڷ�");
			}

			if (pDM)
			{
				pDM->SetCellData(_T("BB_�ڷ����_����"), str, nRow);
			}
        }
		if(strDMFieldAlias == _T("BB_���Ա���"))
		{
			if(str == _T("1")) str = _T("����");
			else if(str == _T("2")) str = _T("����");
			else if(str == _T("3")) str = _T("��ȯ");
			else str = _T("�������");
		}
		if(strDMFieldAlias == _T("UDF_�׷��ȣ"))
		{
			strAcqType = pDM->GetCellData(_T("BB_���Ա���"), nRow);
			strAcqYear = pDM->GetCellData(_T("BP_���Գ⵵"), nRow);
			strGroupNo = pDM->GetCellData(_T("BP_�׷��ȣ"), nRow);
			if(strAcqType == _T("1") || strAcqType == _T("2"))
			{
				if(!strAcqYear.IsEmpty() && !strGroupNo.IsEmpty())
				{
					nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
					strTmpData.Format(_T("%s-%04d"), strAcqYear, nGroupNo);
					str = strTmpData;
					pDM->SetCellData(strDMFieldAlias, str, nRow);
				}
			}
		}
    }
	return 0;

EFS_END
return -1;

}
VOID CBO_CAT_REG_MAIN::InputBook()
{

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	CESL_Grid *pGrid = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	CSpecies_Form dlg(this);

	dlg.SetAcqType(ACQ_TYPE_INVALID);
	dlg.SetSpeciesType(BO_REG_INVALID);	
	dlg.SetOpType(SINSERT);
	dlg.SetSpeciesKeyAlias(_T("IBS_��KEY"));
	dlg.SetAccuKeyAlias(_T("BB_��������KEY"));

	CString m_Year;
	CTime mTime = pDM->GetDBTime();
	m_Year.Format(_T("%d"), mTime.GetYear());

	dlg.SetParentGroupInfo((INT)PURCHASE, m_Year , _T(""), pDM->CONNECTION_INFO);
	dlg.SetParentType(BOOK);
	dlg.SetParentDM(pDM);
	dlg.SetParentBookDM(pDM);	
	dlg.SetParentGrid(pGrid);
	dlg.DoModal();
	
}
INT CBO_CAT_REG_MAIN::DeleteBook()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_����_���_����"));
	CESL_Grid * pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_REG_INVALID);
	api.SetTempData(_T("ACQ_TYPE"), (INT)ACQ_TYPE_INVALID);
	api.SetTempData(_T("����Ÿ��"), (INT)BOOK_DELETE);
	api.SetTempData(_T("å���˸��ƽ�"), _T("BB_��Ϲ�ȣ"));
	api.SetTempData(_T("���ݾ˸��ƽ�"), _T("���ι�ȣ"));
	api.SetTempData(_T("��������"), _T(""));
	api.SetTempPtr(_T("��Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("��������Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("���Ϸù�ȣ����Ʈ"), NULL);
	INT ids = api.SPFExecutePath(_T("����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ������ �� �����ϴ�."));
		return 0;
	}

	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid->Display();
	ESLAfxMessageBox(_T("������ �ڷḦ �����Ͽ����ϴ�."));

	return 0;

}


VOID CBO_CAT_REG_MAIN::DoClassify() //�з����ó��
{
	INT idx;
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	CESL_DataMgr *p049DM = FindDM(_T("DM_����_����_���_049"));
	p049DM->FreeData();

	if(idx < 0)
	{
		//����ó��
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return ;
	}

	CString BookKey,strStatus,sRec_Key,strRegNo,strCopyCode,strVolTitle;
	strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

	if (strStatus != _T("BOR112N")) //����ڷ� 
	{
		//����ó��
		ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
		return;
	}

	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		if (strStatus != _T("BOR112N")) //����ڷ� 
		{
			//����ó��
			ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
			return;
		}

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}



	if(IDCANCEL == ESLAfxMessageBox(_T("�з����ó�� �۾��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return;



	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_����_���_����"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_����_����_���_���� error") );
		return ;
	}


	//��� å Ű ��������
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

	// ���� spl ����
	INT Dm_index=0;
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC112O") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
	pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	

	//049 tag ����
	sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , idx );
	strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
	strCopyCode = pDM->GetCellData( _T("BB_������ȣ") , idx );  
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.08.21 ������ ���� 
	// 049 Tag $v ������ VOL_TITLE ���� VOL �� ���� 
	strVolTitle = pDM->GetCellData( _T("BB_�����") , idx );

	// ���� �ҽ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//strVolTitle = pDM->GetCellData( _T("BB_������ηϸ�") , idx );
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);

	
	pDM->DeleteRow(idx);
	Dm_index++;
	
	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC112O") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
		pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));
		//049 tag ����
		sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , (idx - Dm_index) );
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , (idx - Dm_index) );
		strCopyCode = pDM->GetCellData( _T("BB_������ȣ") , (idx - Dm_index) );  
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.21 ������ ���� 
		// 049 Tag $v ������ VOL_TITLE ���� VOL �� ���� 
		strVolTitle = pDM->GetCellData( _T("BB_�����") , (idx - Dm_index) );
		
		// ���� �ҽ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//strVolTitle = pDM->GetCellData( _T("BB_������ηϸ�") , idx );
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);

		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;


		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	if( pDM->SendFrame() >= 0 )
	{
		DBto049();
	}
	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid_MainGrid_All->Display();

	AfxMessageBox(_T("���� �Ǿ����ϴ�."));

	return;
}
VOID CBO_CAT_REG_MAIN::DoCatalog() //���� ���ó��
{
	INT idx;
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	CESL_DataMgr *p049DM = FindDM(_T("DM_����_����_���_049"));
	p049DM->FreeData();

	if(idx < 0)
	{
		//����ó��
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return ;
	}

	CString BookKey,strStatus,sRec_Key,strRegNo,strCopyCode,strVolTitle;
	strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

	if (strStatus != _T("BOR112N")) //����ڷ� 
	{
		//����ó��
		ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
		return;
	}

	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		if (strStatus != _T("BOR112N")) //����ڷ� 
		{
			//����ó��
			ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
			return;
		}

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}



	if(IDCANCEL == ESLAfxMessageBox(_T("�������ó�� �۾��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return;



	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_����_���_����"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_����_����_���_���� error") );
		return ;
	}


	//��� å Ű ��������
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

	// ���� spl ����
	INT Dm_index=0;
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
	pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.08.24 ������ �߰� 
	// INDEX TABLE �� WORKING_STATUS �ʵ� UPDATE , BOOK TABLE �� ���� �����ͷ� UPDATE
    sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , idx );

	pDM->InitDBFieldData ();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
	pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, idx);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//049 tag ����
	sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , idx );
	strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
	strCopyCode = pDM->GetCellData( _T("BB_������ȣ") , idx );  
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.08.21 ������ ���� 
	// 049 Tag $v ������ VOL_TITLE ���� VOL �� ���� 
	strVolTitle = pDM->GetCellData( _T("BB_�����") , idx );

	// ���� �ҽ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//strVolTitle = pDM->GetCellData( _T("BB_������ηϸ�") , idx );
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);

	//�۾� �ο� ����� 	
	pDM->DeleteRow(idx);
	Dm_index++;
	
	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
		pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.24 ������ �߰� 
		// INDEX TABLE �� WORKING_STATUS �ʵ� UPDATE , BOOK TABLE �� ���� �����ͷ� UPDATE
		sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData ();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
		pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, (idx - Dm_index));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//049 tag ����
		sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , (idx - Dm_index));
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , (idx - Dm_index));
		strCopyCode = pDM->GetCellData( _T("BB_������ȣ") , (idx - Dm_index));  
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.21 ������ ���� 
		// 049 Tag $v ������ VOL_TITLE ���� VOL �� ���� 
		strVolTitle = pDM->GetCellData( _T("BB_�����") , (idx - Dm_index) );
		
		// ���� �ҽ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//strVolTitle = pDM->GetCellData( _T("BB_������ηϸ�") , idx - Dm_index );
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);

		//�۾� �ο� ����� 	
		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	if( pDM->SendFrame() >= 0 )
	{
		DBto049();
	}


	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid_MainGrid_All->Display();

	AfxMessageBox(_T("���� �Ǿ����ϴ�."));

	return;

}
VOID CBO_CAT_REG_MAIN::DoFindCopy()
{
	INT idx;
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	CESL_DataMgr *p049DM = FindDM(_T("DM_����_����_���_049"));
	p049DM->FreeData();

	if(idx < 0)
	{
		//����ó��
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return ;
	}

	CString BookKey,strStatus,sRec_Key,strRegNo,strCopyCode,strVolTitle;
	strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

	if (strStatus != _T("BOR112N")) //����ڷ� 
	{
		//����ó��
		ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
		return;
	}

	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		if (strStatus != _T("BOR112N")) //����ڷ� 
		{
			//����ó��
			ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
			return;
		}

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}



	if(IDCANCEL == ESLAfxMessageBox(_T("�������ó�� �۾��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL))
		return;



	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_����_���_����"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_����_����_���_���� error") );
		return ;
	}


	//��� å Ű ��������
	SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

	// ���� spl ����
	INT Dm_index=0;
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC111O") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
	pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
	

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.08.20 ������ �߰� 
	// INDEX TABLE �� WORKING_STATUS �ʵ� UPDATE , BOOK TABLE �� ���� �����ͷ� UPDATE
    sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , idx );

	pDM->InitDBFieldData ();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC111O") );
	pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, idx);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	//049 tag ����
	sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , idx );
	strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
	strCopyCode = pDM->GetCellData( _T("BB_������ȣ") , idx );  
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.08.21 ������ ���� 
	// 049 Tag $v ������ VOL_TITLE ���� VOL �� ���� 
	strVolTitle = pDM->GetCellData( _T("BB_�����") , idx );

	// ���� �ҽ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//strVolTitle = pDM->GetCellData( _T("BB_������ηϸ�") , idx );
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);
	
	pDM->DeleteRow(idx);
	Dm_index++;
	
	idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC111O") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
		pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));

        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.20 ������ �߰�  
		// INDEX TABLE �� WORKING_STATUS �ʵ� UPDATE , BOOK TABLE �� ���� �����ͷ� UPDATE
		sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData ();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC111O") );
		pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, (idx - Dm_index));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


		//049 tag ����
/* ���� ���� : 73������(3���̻�)���� ������� ó���� SQL Error �߻� ����   +++
 * ���� ��¥ : 2003.08.20
 * ���� ��� : ��ȫ�� 
 */
		/*
		sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , idx );
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
		strCopyCode = pDM->GetCellData( _T("BB_������ȣ") , idx );  
		strVolTitle = pDM->GetCellData( _T("BB_������ηϸ�") , idx );
		*/

		sRec_Key = pDM->GetCellData( _T("IBS_��KEY") , (idx - Dm_index) );
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , (idx - Dm_index) );
		strCopyCode = pDM->GetCellData( _T("BB_������ȣ") , (idx - Dm_index) );  
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.21 ������ ���� 
		// 049 Tag $v ������ VOL_TITLE ���� VOL �� ���� 
		strVolTitle = pDM->GetCellData( _T("BB_�����") , (idx - Dm_index) );
		
		// ���� �ҽ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//strVolTitle = pDM->GetCellData( _T("BB_������ηϸ�") , idx );
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ���� ���� : 73������(3���̻�)���� ������� ó���� SQL Error �߻� ����   ---

		GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);

		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;

		idx = SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	}

	if( pDM->SendFrame() >= 0 )
	{
		DBto049();
	}

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid_MainGrid_All->Display();

	AfxMessageBox(_T("���� �Ǿ����ϴ�."));

	return;

}

INT CBO_CAT_REG_MAIN::Make049Tag(INT idx,CString strRegNO,CString strCopyCode, CString strVolTitle)
{
	CString strMarc;
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_049"));
	strMarc = pDM->GetCellData( _T("MARC") , idx);

	//strMARC�� strRegNO�� ������ �� 
	
	CMarc marc;
	INT ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);

	// ��Ϲ�ȣ�� MARC�� �����ϴ��� üũ�Ѵ�.
	CString strItemData;
	CArray<CString, CString&> arrItem;
	m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arrItem);
	CCatApi CatApi(this);
	if (CatApi.IsRegNoInArray(&arrItem, strRegNO)) return 0;

	// 049 ã�ƿ���
	CString strTagCode = _T("049");
	CString str049Field;
	ids = m_pInfo->pMarcMgr->GetField(&marc, strTagCode, str049Field);
	if (str049Field.IsEmpty())
	{
		str049Field = _T("0 ");
	}
	else
	{
		str049Field = str049Field.Mid(0,str049Field.GetLength()-1);

	}

	CString strTemp;
	if( !strRegNO.IsEmpty() )//��Ϲ�ȣ �� �������
	{
		strTemp.Format(_T("$l%s"),strRegNO);
		str049Field+= strTemp;
	}
	if( !strVolTitle.IsEmpty() )//�����  
	{
		strTemp.Format(_T("$v%s"),strVolTitle);		
		str049Field+= strTemp;
	}	
	if( !strCopyCode.IsEmpty() )//�����  
	{
		strTemp.Format(_T("$c%s"),strCopyCode);		
		str049Field+= strTemp;
	}	

	CString Tmp;

	m_pInfo->pMarcMgr->GetField(&marc, strTagCode , Tmp);
	if ( Tmp.IsEmpty() )
	{
		m_pInfo->pMarcMgr->DeleteField(&marc, strTagCode);
		m_pInfo->pMarcMgr->InsertField(&marc, strTagCode + str049Field);
	}
	else
	{
		Tmp = Tmp.Mid(0,Tmp.GetLength()-1);
		m_pInfo->pMarcMgr->DeleteField(&marc, strTagCode);
		m_pInfo->pMarcMgr->InsertField(&marc, strTagCode + str049Field);
//		m_pInfo->pMarcMgr->InsertField(&marc, strTagCode + Tmp + str049Field.Mid(2));

	}
//	m_pInfo->pMarcMgr->DeleteField(&marc, strTagCode);

	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	

	pDM->SetCellData( _T("MARC"),strMarc, idx);

	
	return 0;
}

INT CBO_CAT_REG_MAIN::GetMARCTag(CString sRec_key,CString strRegNO,CString strCopyCode, CString strVolTitle)
{
	INT idx=0;
	CString sKey,strQry,strMARC;
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_049"));

	//������ �޴����� ��ũ�� �ִ��� Ȯ���Ѵ�.
	INT count = pDM->GetRowCount();

	for( INT i=0 ; i< count ; i++)
	{
		sKey = pDM->GetCellData( _T("IBS_��KEY") , i );
		if( sKey == sRec_key)
		{
			Make049Tag(i,strRegNO,strCopyCode,strVolTitle);
			return 0;
		}

	}

	//������ �޴����� ��ũ�� ������ �����ͼ� �ִ´�. 
	pDM->InsertRow(-1);

	strQry.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY = %s"),sRec_key);
	pDM->GetOneValueQuery(strQry,strMARC);

	pDM->SetCellData(_T("IBS_��KEY"), sRec_key , count);
	pDM->SetCellData(_T("MARC"), strMARC , count);

	Make049Tag(count,strRegNO,strCopyCode,strVolTitle);
	return 0;
}

INT CBO_CAT_REG_MAIN::DBto049()
{
	INT idx=0;
	CString sKey,strQry,strMARC;
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_049"));

	//������ �޴����� ��ũ�� �ִ��� Ȯ���Ѵ�.
	INT count = pDM->GetRowCount();

	for( INT i=0 ; i< count ; i++)
	{
		sKey = pDM->GetCellData( _T("IBS_��KEY") , i );
		strMARC = pDM->GetCellData( _T("MARC") , i );

		pDM->StartFrame();
		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMARC, -1);
		pDM->ORACLE_OCIMgr_MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sKey);
		pDM->SendFrame();
		pDM->EndFrame();
	}

	return 0;

}

INT CBO_CAT_REG_MAIN::ModifyBook()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));

	CSpecies_Form dlg(this);
	dlg.SetAcqType(ACQ_TYPE_INVALID);
	dlg.SetSpeciesType(BO_REG_INVALID);
		
	dlg.SetOpType(SUPDATE);
	dlg.SetSpeciesKeyAlias(_T("IBS_��KEY"));
	dlg.SetAccuKeyAlias(_T("BB_��������KEY"));

	CString m_Year;
	CTime mTime = pDM->GetDBTime();
	m_Year.Format(_T("%d"), mTime.GetYear());

	dlg.SetParentGroupInfo((INT)PURCHASE, m_Year , _T(""), pDM->CONNECTION_INFO);
	dlg.SetParentType(BOOK);
	dlg.SetParentDM(pDM);
	dlg.SetParentBookDM(pDM);	

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	dlg.SetParentGrid(pGrid_MainGrid_All);
	dlg.DoModal();

	return 0;
}

INT CBO_CAT_REG_MAIN::FileImport()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));

	CFileImportDlg dlg(this);
	dlg.SetBeginImportStatus(_T("���"), _T("BOR111N"));
	dlg.SetOpenMode(4000);

	CString m_Year;
	CTime mTime = pDM->GetDBTime();
	m_Year.Format(_T("%d"), mTime.GetYear());

	dlg.SetAcqYear(m_Year);
	
	dlg.SetGroupInfo(_T(""));


	dlg.SetParentDM(pDM);

	CESL_Grid *m_pGrid = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	dlg.SetParentGrid(m_pGrid);
	
	dlg.DoModal();
	m_pGrid->DisplayNumber();



	return 0;
}

BEGIN_EVENTSINK_MAP(CBO_CAT_REG_MAIN, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_REG_MAIN)
	ON_EVENT(CBO_CAT_REG_MAIN, IDC_MSHFLEXGRID, -601 /* DblClick */, OnDblClickMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_REG_MAIN::OnDblClickMshflexgrid() 
{
	CESL_Grid *m_pGrid = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	if(m_pGrid->GetMouseRow() == 0)
	{
		SortRegData(m_pGrid);
		return;
	}
	ModifyBook();	
}

INT CBO_CAT_REG_MAIN::GetRegnoTitle()
{
	CString strParentCMAlias, strParentDMAlias, strParentGridAlias;
	if (strParentCMAlias.IsEmpty())
		strParentCMAlias = _T("CM_����_����_���_����");
	if (strParentDMAlias.IsEmpty())
		strParentDMAlias = _T("DM_����_����_���_����");
	if (strParentGridAlias.IsEmpty())
		strParentGridAlias = _T("MAIN_GRID");

	INT idx;
	SelectMakeList(strParentCMAlias, strParentGridAlias);
	idx = SelectGetHeadPosition(strParentCMAlias, strParentGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		//EndDialog(IDCANCEL);
		//m_bInitDlg = FALSE;
		return -1;		
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(strParentDMAlias);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}

	CString sRecKey = pDM->GetCellData( _T("IBS_��KEY") , idx );
	if ( sRecKey == _T("") ) {
		AfxMessageBox (_T("������ �ڷᰡ �����ϴ�."));
//		m_bInitDlg = FALSE;
		//return 0;
		return FALSE;
	}
	INT iRowCount = 1;

	
	//RFID ��Ϲ�ȣ, ���� �ֱ�
	CBarcodeToRFIDDlg dlg;	
	//dlg.CONNECTION_INFO = _T("kolas2@kolas2/kolas2#[]");
	dlg.CONNECTION_INFO = pDM->CONNECTION_INFO;	
	CRFIDBookInfo *pBook;

	CString sRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ"), idx );
	CString sTitle = pDM->GetCellData( _T("IBS_��ǥ��"), idx );
	CString sRfidSerial = pDM->GetCellData( _T("BB_RfidSerial"), idx );	
	CString sBookKey = pDM->GetCellData( _T("BB_åKEY"), idx );
	CString sBookStatus = pDM->GetCellData( _T("�۾�����"), idx );

	//ù��° ��Ϲ�ȣ, ����
	pBook = dlg.AddBook();
	if (pBook) {
		pBook->strAccessionNo = sRegNo;
		pBook->strTitle = sTitle;
		if ( sRfidSerial != _T("") ) {
			pBook->strRFIDSerialNo = sRfidSerial;
		}
		pBook->strBookKey = sBookKey;
		pBook->strStatus = sBookStatus;
	}

	idx = SelectGetNext(strParentCMAlias, strParentGridAlias);
	CString sRecKeySub;
	while ( idx != -1 ) {
		/*
		CString sKey;
		pDM->GetCellData( _T("IBS_��KEY") , idx , sKey );	
		sRecKey = sRecKey + _T(" , ") + sKey;
		*/
		CString sRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ"), idx );
		CString sTitle = pDM->GetCellData( _T("IBS_��ǥ��"), idx );
		CString sRfidSerial = pDM->GetCellData( _T("BB_RfidSerial"), idx );	
		CString sBookKey = pDM->GetCellData( _T("BB_åKEY"), idx );
		CString sBookStatus = pDM->GetCellData( _T("�۾�����"), idx );

		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = sRegNo;
			pBook->strTitle = sTitle;
			if ( sRfidSerial != _T("") ) {
				pBook->strRFIDSerialNo = sRfidSerial;
			}
			pBook->strBookKey = sBookKey;
			pBook->strStatus = sBookStatus;
		}

		idx = SelectGetNext(strParentCMAlias, strParentGridAlias);		
		iRowCount ++ ;
	} 

	dlg.AddSaveOption(IDX_REG_CODE);
	dlg.AddSaveOption(IDX_REG_NUM);
	dlg.AddSaveOption(IDX_STATUS);

	dlg.DoModal();
/*
	//RFID ��Ϲ�ȣ, ���� �ֱ�
	CBarcodeToRFIDDlg dlg;
	dlg.CONNECTION_INFO = _T("kolas2@kolas2/kolas2#[]");
	CRFIDBookInfo *pBook;

	pBook = dlg.AddBook();
	if (pBook) {
		pBook->strAccessionNo = _T("EM0000030180");
		pBook->strTitle = _T("����1987");
	}
	pBook = dlg.AddBook();
	if (pBook) {
		pBook->strAccessionNo = _T("EM0000030181");
		pBook->strTitle = _T("�𳪸���");
	}
	pBook = dlg.AddBook();
	if (pBook) {
		pBook->strAccessionNo = _T("EM0000030182");
		pBook->strTitle = _T("��");
	}
	pBook = dlg.AddBook();
	if (pBook) {
		pBook->strAccessionNo = _T("EM0000030183");
		pBook->strTitle = _T("���� �׷����� ���ڳ�");
	}
	pBook = dlg.AddBook();
	if (pBook) {
		pBook->strAccessionNo = _T("EM0000030184");
		pBook->strTitle = _T("����ϴ� �״뿡��");
	}

	dlg.DoModal();

	/*
	if (InitESL_Mgr(_T("SM_����_����_���_����")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
*/
	return 0;
}

VOID CBO_CAT_REG_MAIN::OnRFID(WPARAM wParam, LPARAM IParam)
{
	TCHAR szSerial[32];
	_stprintf(szSerial, _T("%s\0"), IParam);
	//AfxMessageBox(szSerial);

	CString sSerialNo;
	sSerialNo.Format( _T("%s"), szSerial );

	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	if ( pDM == NULL ) return;
	//CString strWhere = _T("REG_NO = 'EM0000070006'");
	//CString strWhere = _T("B.WORKNO = ") + sSerialNo;
	//if (pDM->RefreshDataManager(strWhere) < 0)	return;

	CESL_DataMgr DM_BOOK;
	DM_BOOK.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString sQuery;
	//sQuery.Format( _T("SELECT DISTINCT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY IN (%s)"), m_strBookKeys );
	sQuery.Format( _T("SELECT WORKING_STATUS, ACCESSION_REC_KEY, SEQ_NO FROM BO_BOOK_TBL WHERE WORKNO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sSerialNo );
	//sQuery.Format( _T("SELECT WORKING_STATUS, ACCESSION_REC_KEY, SEQ_NO FROM BO_BOOK_TBL WHERE REG_NO = 'EM0000124321'") );
	DM_BOOK.RestructDataManager( sQuery );
	INT iBookCount = DM_BOOK.GetRowCount();

	CString sBookStatus = DM_BOOK.GetCellData( 0, 0 );
	CString sAccessionRecKey = DM_BOOK.GetCellData( 0, 1 );
	CString sSeqNo = DM_BOOK.GetCellData( 0, 2 );

	CString strWhere;
	if ( sBookStatus >= _T("BOL112N") && sBookStatus <= _T("BOL215O") ) {
		//strWhere = _T("I.REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE reg_no = 'EM0000070006' ) AND I.REC_KEY = S.REC_KEY");
		strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE WORKNO = '%s' AND WORKING_STATUS >= 'BOR111N' AND WORKING_STATUS <= 'BOR113O' ) AND I.REC_KEY = S.REC_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE"), sSerialNo );
	}
	else {
		if ( sAccessionRecKey != _T("") ) {
			strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY = %s AND WORKING_STATUS >= 'BOR111N' AND WORKING_STATUS <= 'BOR113O') AND I.MANAGE_CODE = UDF_MANAGE_CODE"), sAccessionRecKey );
		}
		else {
			strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE SEQ_NO = '%s' AND WORKING_STATUS >= 'BOR111N' AND WORKING_STATUS <= 'BOR113O' ) AND I.MANAGE_CODE = UDF_MANAGE_CODE"), sSeqNo );
		}
	}
	if (pDM->RefreshDataManager(strWhere) < 0)	return;

	INT iRowCount = pDM->GetRowCount();
	
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_����_���_����"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_����_����_���_���� error") );
		return;
	}
	pCM->AllControlDisplay();
	
}
//�������߰�===================================================================
VOID CBO_CAT_REG_MAIN::RegDataOrder(CESL_ControlMgr * pCM, CESL_DataMgr * pDM)
{
	if(pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CBO_ACQ_DataSortDlg dlg(this);
	dlg.SetParentDM(pDM);
	dlg.SetType(2);
	if(IDCANCEL == dlg.DoModal()) return;
	pCM->AllControlDisplay();
}

VOID CBO_CAT_REG_MAIN::CodeChange(CESL_ControlMgr *pCM, CESL_DataMgr *pDM, CESL_Grid *pGrid)
{
	INT ids = pGrid->SelectMakeList();
	if(ids >= 0) ids = pGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	CBookCodeChange dlg(this);
	dlg.SetDM(pDM);
	dlg.SetMainGrid(pGrid);
	if(IDOK == dlg.DoModal())
		pGrid->Display();
}
//�������߰�===================================================================

INT CBO_CAT_REG_MAIN::SortRegData(CESL_Grid *pGrid)
{
	CESL_DataMgr *pDM = FindDM(_T("DM_����_����_���_����"));
	INT nCol = pGrid->GetCol();
	POSITION pos = pGrid->m_arrColumnInfo.FindIndex(nCol-1);
	CESL_GridColumnInfo *pColumnInfo = (CESL_GridColumnInfo*)pGrid->m_arrColumnInfo.GetAt(pos);
	CString strSortAlias = (CString)pColumnInfo->strColumnDMFieldAlias;
	
	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("BB_���Ա���");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_���Գ⵵");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BP_�׷��ȣ");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = strSortAlias;			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_�Ϸù�ȣ");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	pGrid->Display();
	return 0;
}
