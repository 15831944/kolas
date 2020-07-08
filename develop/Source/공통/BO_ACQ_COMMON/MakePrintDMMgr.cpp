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
	if(strPath.IsEmpty()) return -1;
	
	if(strPath == _T("�Ϲݸ���Ʈ���"))
		return MakeSimplePrintDM();
	else if(strPath == _T("��û�ڷᵵ��������"))
		return MakeRequestDataPrintDM();
	else if(strPath == _T("�˼��Ϸ��ڷ����"))
		return MakeCheckInCompletePrintDM();
	else if(strPath == _T("�Ұ��ڷ����"))
		return MakeSubTotalPrintDM();
	else if(strPath == _T("å�Ұ��ڷ����"))
		return MakeSubTotalBookPrintDM();
	else if(strPath == _T("�Ǹ���Ʈ���"))
		return MakeVolumePrintDM();
	return -1;
}

INT CMakePrintDMMgr::MakeSimplePrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pPrintSourceDM = NULL;
	CESL_DataMgr * pPrintObjDM = NULL;
	INT nPrintType;
	GetTempData(_T("����ƮŸ��"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	GetTempData(_T("å���˸��ƽ�"), strBookCntAlias);
	GetTempData(_T("���ݾ˸��ƽ�"), strPriceAlias);
	GetTempData(_T("���ΰ��˸��ƽ�"), strDiscPriceAlias);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("����åDM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("�������DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));

	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;

	PrintMgr.SetPrintBookDM(pBookDM);
	PrintMgr.SetPrintSourceDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintObjDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);

	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;

	return 0;
}

INT CMakePrintDMMgr::MakeSubTotalPrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	INT nPrintType;
	BOOL bKOLAS2Format = FALSE;
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias, strBookDMAlias, strSortFieldAlias;
	CGroupCollection * pGroupCollection = NULL;
	GetTempData(_T("����ƮŸ��"), nPrintType);
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	GetTempData(_T("å���˸��ƽ�"), strBookCntAlias);
	GetTempData(_T("���ݾ˸��ƽ�"), strPriceAlias);
	GetTempData(_T("���ΰ��˸��ƽ�"), strDiscPriceAlias);
	GetTempData(_T("åDM�˸��ƽ�"), strBookDMAlias);
	GetTempData(_T("�����ʵ�˸��ƽ�"), strSortFieldAlias);
	GetTempData(_T("KOLAS2����"), bKOLAS2Format);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pGroupCollection = (CGroupCollection*)GetTempPtr(_T("�׷��÷���"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));

	if(pGroupCollection == NULL || pPrintDM == NULL) return -1;

	CMakeSubTotalPrintDM PrintMgr(m_pParentMgr);
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetGroupCollection(pGroupCollection);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetSortCondition(strSortFieldAlias);
	PrintMgr.SetBookDMAlias(strBookDMAlias);
	PrintMgr.SetKolas2FormatYN(bKOLAS2Format);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;
	if(ids == 1000) return ids;

	return 0;
}

INT CMakePrintDMMgr::MakeVolumePrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CMakeVolumePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	INT nPrintType;
	CString strDivAlias;
	GetTempData(_T("����ƮŸ��"), nPrintType);
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("����åDM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	
	if(pBookDM == NULL || pPrintDM == NULL) return -1;
	
	PrintMgr.SetBookDM(pBookDM);
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	
	INT ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;

	return 0;
}

INT CMakePrintDMMgr::MakeRequestDataPrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pPrintSourceDM = NULL;
	CESL_DataMgr * pPrintObjDM = NULL;
	INT nPrintType;
	GetTempData(_T("����ƮŸ��"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	GetTempData(_T("å���˸��ƽ�"), strBookCntAlias);
	GetTempData(_T("���ݾ˸��ƽ�"), strPriceAlias);
	GetTempData(_T("���ΰ��˸��ƽ�"), strDiscPriceAlias);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("����åDM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("�������DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	
	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;
	
	PrintMgr.SetPrintBookDM(pBookDM);
	PrintMgr.SetPrintSourceDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintObjDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakeRequestDataPrintDM();
	if(ids < 0) return -1;
	
	return 0;
}

INT CMakePrintDMMgr::MakeCheckInCompletePrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CMakeSimplePrintDM PrintMgr(m_pParentMgr);
	CESL_DataMgr * pBookDM = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pPrintSourceDM = NULL;
	CESL_DataMgr * pPrintObjDM = NULL;
	INT nPrintType;
	GetTempData(_T("����ƮŸ��"), nPrintType);
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias;
	GetTempData(_T("���о˸��ƽ�"), strDivAlias);
	GetTempData(_T("å���˸��ƽ�"), strBookCntAlias);
	GetTempData(_T("���ݾ˸��ƽ�"), strPriceAlias);
	GetTempData(_T("���ΰ��˸��ƽ�"), strDiscPriceAlias);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("����åDM"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("�������DM"));
	pPrintObjDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	
	if(pBookDM == NULL || pPrintDM == NULL || pPrintObjDM == NULL) return -1;
	
	PrintMgr.SetPrintBookDM(pBookDM);
	PrintMgr.SetPrintSourceDM(pPrintDM);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetPrintDM(pPrintObjDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetDivAlias(strDivAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakeCheckInCompletePrintDM();
	if(ids < 0) return -1;
	
	return 0;
}

INT CMakePrintDMMgr::MakeSubTotalBookPrintDM()
{
	CDataProcessingDlg * pDlg = NULL;
	CESL_DataMgr * pPrintDM = NULL;
	CESL_DataMgr * pBookDM = NULL;
	INT nPrintType;
	CString strDivAlias, strBookCntAlias, strPriceAlias, strDiscPriceAlias, strBookDMAlias, strSortFieldAlias;
	CGroupCollection * pGroupCollection = NULL;
	GetTempData(_T("����ƮŸ��"), nPrintType);
	GetTempData(_T("���ݾ˸��ƽ�"), strPriceAlias);
	GetTempData(_T("���ΰ��˸��ƽ�"), strDiscPriceAlias);
	GetTempData(_T("�����ʵ�˸��ƽ�"), strSortFieldAlias);
	pDlg = (CDataProcessingDlg*)GetTempPtr(_T("���α׷�����Ʈ��"));
	pGroupCollection = (CGroupCollection*)GetTempPtr(_T("�׷��÷���"));
	pPrintDM = (CESL_DataMgr*)GetTempPtr(_T("���DM"));
	pBookDM = (CESL_DataMgr*)GetTempPtr(_T("����åDM"));
	
	if(pGroupCollection == NULL || pPrintDM == NULL || pBookDM == NULL) return -1;
	
	CMakeSubTotalBookPrintDM PrintMgr(m_pParentMgr);
	PrintMgr.SetPrintDM(pPrintDM);
	PrintMgr.SetPrintType(nPrintType);
	PrintMgr.SetBookDM(pBookDM);
	PrintMgr.SetGroupCollection(pGroupCollection);
	PrintMgr.SetDataProcessingDlg(pDlg);
	PrintMgr.SetSortCondition(strSortFieldAlias);
	PrintMgr.SetTotalInfoAlias(strBookCntAlias, strPriceAlias, strDiscPriceAlias);
	
	INT ids = PrintMgr.InitMakePrintDM();
	if(ids < 0) return -1;
	ids = PrintMgr.MakePrintDM();
	if(ids < 0) return -1;
	if(ids == 1000) return ids;
	
	return 0;
}
