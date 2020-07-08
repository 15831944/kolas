// MakePrintDMMgr.cpp: implementation of the CMakePrintDMMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakePrintDMMgr.h"
#include "MakeSimplePrintDM.h"
#include "MakeSubTotalPrintDM.h"
#include "MakeVolumePrintDM.h"
#include "MakeSubTotalBookPrintDM.h"
#include "../K2UP_Common/DataProcessingDlg.h"
#include "../DATA_SORT_MANAGER/SortItemSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakePrintDMMgr::CMakePrintDMMgr(CESL_Mgr * pMgr) : CSPF_Work(pMgr)
{

}

CMakePrintDMMgr::~CMakePrintDMMgr()
{

}

INT CMakePrintDMMgr::SPFExecutePath(CString strPath)
{
	if( strPath.IsEmpty() == TRUE )		return -1;
	
	if		( strPath == _T("�Ϲݸ���Ʈ���")		)	return MakeSimplePrintDM();
	else if	( strPath == _T("��û�ڷᵵ��������") )	return MakeRequestDataPrintDM();
	else if	( strPath == _T("�˼��Ϸ��ڷ����")		)	return MakeCheckInCompletePrintDM();
	else if	( strPath == _T("�Ұ��ڷ����")			)	return MakeSubTotalPrintDM();
	else if	( strPath == _T("å�Ұ��ڷ����")		)	return MakeSubTotalBookPrintDM();
	else if	( strPath == _T("�Ǹ���Ʈ���")			)	return MakeVolumePrintDM();
	else if	( strPath == _T("�Ϸù�ȣ����")			)	return SetDataSerialNo();
	else if	( strPath == _T("��ġ����ڷ����")		)	return MakeRequestPrintDM();
	
	return -1;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - [�Ϲݸ���Ʈ���] DM ����
//
// [ �̴Ͻ��� ]
//
//////////////////////////////////////////////////////////////////////
INT CMakePrintDMMgr::MakeSimplePrintDM()
{
	//================================================================
	// ���� ����
	//================================================================
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	CMapStringToString* pArrDivValue = NULL;
	CSortItemSet* pSortItem = NULL;
	CDataProcessingDlg* pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr* pBookDM = NULL;
	CESL_DataMgr* pPrintDM = NULL;
	CESL_DataMgr* pPrintSourceDM = NULL;
	CESL_DataMgr* pPrintObjDM = NULL;

	


	//================================================================
	// 1. ����� ���� ���´�.
	//		- ����ƮŸ��
	//		- ���о˸��ƽ�
	//		- å���˸��ƽ�
	//		- ���ݾ˸��ƽ�
	//		- ���ΰ��˸��ƽ�
	//		- ����ڷ����Ŀ���	
	//================================================================
	GetTempData(_T("����ƮŸ��"), nPrintType);
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	GetTempData(_T("å���˸��ƽ�"), strBookCntAlias);
	GetTempData(_T("���ݾ˸��ƽ�"), strPriceAlias);
	GetTempData(_T("���ΰ��˸��ƽ�"), strDiscPriceAlias);
	GetTempData(_T("����ڷ����Ŀ���"), bDataSortYN);


	//================================================================
	// 2. ����� �����͸� ���´�.
	//		- ���α׷��� ��Ʈ�� â
	//		- ����å DM		(DM_�����ڷ����)
	//		- ������� DM	(DM_�����ڷ����_��)
	//		- ��� DM		(DM_�����ڷ����_��¿�)
	//		- ��������
	//		- �׸��忡 ������ Key���� (����)	
	//================================================================
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("����åDM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("�������DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("��������"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("�׸��弱���ڷ�"));
	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;
	

	//================================================================
	// 3. ������ ������ �� �����͸� ��������� �����Ѵ�.
	//================================================================
	PrintMgr.SetPrintBookDM(pBookDM);		// ����å DM
	PrintMgr.SetPrintSourceDM(pPrintDM);	// ������� DM
	PrintMgr.SetDataProcessingDlg(pDlg);	// Progress Ctrl Dialog
	PrintMgr.SetPrintDM(pPrintObjDM);		// ��� DM
	PrintMgr.SetPrintType(nPrintType);		// ����Ʈ Ÿ�� - BO_ACQ_CHECKING_DATA,...
	PrintMgr.SetDivAlias(strDivAlias);		// ���о˸��ƽ� - BP_��������KEY,...
											// å���˸��ƽ�, ���ݾ˸��ƽ�, ���ΰ��˸��ƽ�
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	PrintMgr.SetSortItemSet(pSortItem);		// ��������
	PrintMgr.SetDataSortYN(bDataSortYN);	// ���Ŀ���
	PrintMgr.SetGridSelectDataArray(pArrDivValue);	// ������ �׸��忡 ���ε� Key����


	//================================================================
	// 4. ����ƮŸ�Կ� ���� CMakeSimplePrintDM�� ��������� �����Ѵ�.
	//================================================================
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;


	//================================================================
	// 5. DM�� �����Ѵ�.
	//================================================================
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;

	return 0;
}


///////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �Ұ��ڷ� DM ����
//
// [ �̴Ͻ��� ]
///////////////////////////////////////////////////////////////////////
INT CMakePrintDMMgr::MakeSubTotalPrintDM()
{
	//================================================================
	// ��������
	//================================================================
	INT nPrintType, nPrintDataType;
	BOOL bKOLAS2Format = FALSE;
	BOOL bDataSortYN = FALSE;
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias, strBookDMAlias, strSortFieldAlias;
	CMapStringToString * pArrDivValue = NULL;
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CGroupCollection * pGroupCollection = NULL;
	// Ŭ����
	CMakeSubTotalPrintDM PrintMgr(m_pParentMgr);



	//================================================================
	// 1. ������ �����Ϳ� �����͸� ������ CMakeSubTotalPrintDM�� �״�� �����Ѵ�.
	//================================================================
	GetTempData(_T("����ƮŸ��"), nPrintType);
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	GetTempData(_T("å���˸��ƽ�"), strBookCntAlias);
	GetTempData(_T("���ݾ˸��ƽ�"), strPriceAlias);
	GetTempData(_T("���ΰ��˸��ƽ�"), strDiscPriceAlias);
	
	GetTempData(_T("�����ʵ�˸��ƽ�"), strSortFieldAlias);
	GetTempData(_T("åDM�˸��ƽ�"), strBookDMAlias);
	GetTempData(_T("KOLAS2����"), bKOLAS2Format);

	GetTempData(_T("����ڷ����Ŀ���"), bDataSortYN);
	GetTempData(_T("��µ�����Ÿ��"), nPrintDataType);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pGroupCollection = (CGroupCollection*)GetTempPtr(_T("�׷��÷���"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("��������"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("�׸��弱���ڷ�"));

	if( pGroupCollection == NULL || pPrintDM == NULL )	 return -1;

	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetGroupCollection(pGroupCollection);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetSortCondition(strSortFieldAlias);
	PrintMgr.SetBookDMAlias(strBookDMAlias);
	PrintMgr.SetKolas2FormatYN(bKOLAS2Format);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	PrintMgr.SetPrintDataType(nPrintDataType);
	PrintMgr.SetGridSelectDataArray(pArrDivValue);
	

	//================================================================
	// 2. DM�� �����ϱ� ���� �ʱ�ȭ(������� ����)�Ѵ�.
	//================================================================
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;


	//================================================================
	// 3. DM�� �����Ѵ�.
	//================================================================
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;
	else if(ids == 1000) return ids;

	return 0;
}

INT CMakePrintDMMgr::MakeVolumePrintDM()
{
	CMapStringToString * pArrDivValue = NULL;
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CMakeVolumePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	CString strDivAlias;
	GetTempData(_T("����ƮŸ��"), nPrintType);
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	GetTempData(_T("����ڷ����Ŀ���"), bDataSortYN);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("����åDM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("��������"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("�׸��弱���ڷ�"));
	
	if(pBookDM == NULL || pPrintDM == NULL) return -1;
	
	PrintMgr.SetBookDM(pBookDM);
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	PrintMgr.SetGridSelectDataArray(pArrDivValue);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;

	return 0;
}

INT CMakePrintDMMgr::MakeRequestDataPrintDM()
{
	CMapStringToString * pArrDivValue = NULL;
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pPrintSourceDM = NULL;
	CESL_DataMgr * pPrintObjDM = NULL;
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	GetTempData(_T("����ƮŸ��"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	GetTempData(_T("å���˸��ƽ�"), strBookCntAlias);
	GetTempData(_T("���ݾ˸��ƽ�"), strPriceAlias);
	GetTempData(_T("���ΰ��˸��ƽ�"), strDiscPriceAlias);
	GetTempData(_T("����ڷ����Ŀ���"), bDataSortYN);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("����åDM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("�������DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("��������"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("�׸��弱���ڷ�"));
	
	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;
	
	PrintMgr.SetPrintBookDM(pBookDM);
	PrintMgr.SetPrintSourceDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintObjDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	PrintMgr.SetGridSelectDataArray(pArrDivValue);
	

	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakeRequestDataPrintDM();
	if(ids < 0) return -1;
	
	return 0;
}

INT CMakePrintDMMgr::MakeCheckInCompletePrintDM()
{
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pPrintSourceDM = NULL;
	CESL_DataMgr * pPrintObjDM = NULL;
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	GetTempData(_T("����ƮŸ��"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	GetTempData(_T("å���˸��ƽ�"), strBookCntAlias);
	GetTempData(_T("���ݾ˸��ƽ�"), strPriceAlias);
	GetTempData(_T("���ΰ��˸��ƽ�"), strDiscPriceAlias);
	GetTempData(_T("����ڷ����Ŀ���"), bDataSortYN);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("����åDM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("�������DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("��������"));
	
	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;
	
	PrintMgr.SetPrintBookDM(pBookDM);
	PrintMgr.SetPrintSourceDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintObjDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakeCheckInCompletePrintDM();
	if(ids < 0) return -1;
	
	return 0;
}

INT CMakePrintDMMgr::MakeSubTotalBookPrintDM()
{
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias, strBookDMAlias, strSortFieldAlias;
	CMapStringToString * pArrDivValue = NULL;
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pBookDM = NULL;
	CGroupCollection * pGroupCollection = NULL;
	// Ŭ����
	CMakeSubTotalBookPrintDM PrintMgr(m_pParentMgr);

	GetTempData(_T("����ƮŸ��"), nPrintType);
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	GetTempData(_T("���ݾ˸��ƽ�"), strPriceAlias);
	GetTempData(_T("���ΰ��˸��ƽ�"), strDiscPriceAlias);
	GetTempData(_T("�����ʵ�˸��ƽ�"), strSortFieldAlias);
	GetTempData(_T("����ڷ����Ŀ���"), bDataSortYN);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pGroupCollection = (CGroupCollection*)GetTempPtr(_T("�׷��÷���"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("����åDM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("��������"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("�׸��弱���ڷ�"));
	
	if( pGroupCollection == NULL || pPrintDM == NULL || pBookDM == NULL )	 return -1;
	
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetBookDM(pBookDM);
	PrintMgr.SetGroupCollection(pGroupCollection);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetSortCondition(strSortFieldAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	PrintMgr.SetGridSelectDataArray(pArrDivValue);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;

	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;
	else if(ids == 1000) return ids;
	
	return 0;
}

INT CMakePrintDMMgr::SetDataSerialNo()
{
	CESL_DataMgr * pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	if(pPrintDM == NULL) return -1;

	CMakePrintDM PrintMgr(m_pParentMgr);
	return PrintMgr.SetDataSerialNo(pPrintDM);
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - Grid�� ������ �ڷ���� Alias�� �ش��ϴ� ���� ���� Ű������ ���ڿ� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. Grid�� DM�� ���ٸ� �����Ѵ�.
// 2. Grid�� �ڷᰡ ���ٸ� �����Ѵ�.
// 3. Grid�� ����üũ�� �ڷ���� Grid ��������� �����Ѵ�.
// 4. Grid�� ����üũ�� �ڷ�� �� �� �� �ڷḦ �����Ѵ�.
// 5. ����üũ�� �ڷ�鿡 �ش��Ͽ� ���ڷ� ���� Alias�� �����Ͱ��� �����Ѵ�.
// 6. ������ ���� Ű������ �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CMakePrintDMMgr::GetGridSelectData(CESL_Grid * pGrid, CESL_DataMgr * pDM, CString strDivAlias, CMapStringToString *pArrDivValue, CString strExtraAlias/*=_T("")*/)
{
	//===============================================================
	// ���� ����
	//===============================================================
	INT ids;
	CString strTmpData;

	//===============================================================
	// 1. Grid�� DM�� ���ٸ� �����Ѵ�.
	//===============================================================
	if(pGrid == NULL || pDM == NULL) return -1;


	//===============================================================
	// 2. Grid�� �ڷᰡ ���ٸ� �����Ѵ�.
	//===============================================================
	if(pDM->GetRowCount() == 0) return -1;
	

	//===============================================================
	// 3. Grid�� ����üũ�� �ڷ���� Grid ��������� �����Ѵ�.
	//===============================================================
	ids = pGrid->SelectMakeList();
	if(ids < 0) return ids;


	//===============================================================
	// 4. Grid�� ����üũ�� �ڷ�� �� �� �� �ڷḦ �����Ѵ�.
	//===============================================================
	ids = pGrid->SelectGetHeadPosition();
	
	//===============================================================
	// 5. ����üũ�� �ڷ�鿡 �ش��Ͽ� ���ڷ� ���� Alias�� �����Ͱ��� �����Ѵ�.
	// 6. ������ ���� Ű������ �����Ѵ�.
	//===============================================================
	while(ids >= 0)
	{
		strTmpData = pDM->GetCellData(strDivAlias, ids);

		if( strTmpData.IsEmpty() == FALSE )
		{
			if( strExtraAlias.IsEmpty() == TRUE )
			pArrDivValue->SetAt(strTmpData, _T(""));
		else
		{
			strTmpData += _T("-") + pDM->GetCellData(strExtraAlias, ids);
			pArrDivValue->SetAt(strTmpData, _T(""));
			}
		}
		ids = pGrid->SelectGetNext();
	}
	return 0;
}

INT CMakePrintDMMgr::MakeRequestPrintDM()
{
	CMapStringToString * pArrDivValue = NULL;
	CSortItemSet * pSortItem = NULL;
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pSourceDM = NULL;
	CESL_DataMgr * pSourcePrintDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	INT nPrintType;
	BOOL bDataSortYN = FALSE;
	
	GetTempData(_T("����ƮŸ��"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	GetTempData(_T("����ڷ����Ŀ���"), bDataSortYN);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pSourceDM = (CESL_DataMgr*)GetTempPtr(_T("����DM"));
	pSourcePrintDM = (CESL_DataMgr*)GetTempPtr(_T("�������DM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	pSortItem = (CSortItemSet*)GetTempPtr(_T("��������"));
	pArrDivValue = (CMapStringToString*)GetTempPtr(_T("�׸��弱���ڷ�"));
	
	if(pSourceDM == NULL || pSourcePrintDM == NULL) return -1;
	
	PrintMgr.SetPrintBookDM(pSourceDM);
	PrintMgr.SetPrintSourceDM(pSourcePrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetSortItemSet(pSortItem);
	PrintMgr.SetDataSortYN(bDataSortYN);
	PrintMgr.SetGridSelectDataArray(pArrDivValue);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;
	
	return 0;
}
