// MakeSimplePrintDM.cpp: implementation of the CMakeSimplePrintDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeSimplePrintDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeSimplePrintDM::CMakeSimplePrintDM(CESL_Mgr * pParentMgr) : CMakePrintDM(pParentMgr)
{
	m_pPrintObjDM = NULL;
	m_pPrintSourceDM = NULL;
	m_pPrintBookDM = NULL;
}

CMakeSimplePrintDM::~CMakeSimplePrintDM()
{
	if(m_pPrintObjDM)
	{
		delete m_pPrintObjDM;
		m_pPrintObjDM = NULL;
	}
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ����ƮŸ�Կ� ���� ��������� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. DM�� �������� �ʾҴٸ� �����Ѵ�.
// 2. ����ƮŸ���� [��ġ�����Ȳ����Ʈ]�� �ƴϸ� 
//	  m_pPrintObjDM�� m_pPrintBookDM�� ������ �о�� �ʱ�ȭ�Ѵ�.
// 3. ����ƮŸ�Կ� ���� ��������� �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CMakeSimplePrintDM::InitMakePrintDM()
{
	//================================================================
	// 1. DM�� �������� �ʾҴٸ� �����Ѵ�.
	//================================================================
	if(m_pPrintSourceDM == NULL || m_pPrintBookDM == NULL)		return -1;

	
	//================================================================
	// 2. ����ƮŸ���� [��ġ�����Ȳ����Ʈ]�� �ƴϸ� 
	//	  m_pPrintObjDM�� m_pPrintBookDM�� ������ �о�� �ʱ�ȭ�Ѵ�.
	//================================================================
	if(m_nPrintType != BO_ACQ_REQUEST_DATA)
	{
		m_pPrintObjDM = new CESL_DataMgr;
		m_pPrintObjDM->SetCONNECTION_INFO(m_pPrintSourceDM->CONNECTION_INFO);
		m_pPrintObjDM->InitDMPKFromFile(m_pPrintBookDM->DB_MGR_ALIAS);
		m_pPrintObjDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	}


	//================================================================
	// 3. ����ƮŸ�Կ� ���� ��������� �����Ѵ�.
	//================================================================
	switch ( m_nPrintType )
	{
	// �����ڷ���
	case BO_ACQ_BASIC_DATA:
	{
		m_IgnoreAlias.Add(_T("BB_�Է���"));
		m_IgnoreAlias.Add(_T("UDF_�׷��"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalSpeciesCntAlias = _T("IBS_�����");
	}
		break;

	// �����ڷ���
	case BO_ACQ_IMPORT_DATA:
	{
		m_IgnoreAlias.Add(_T("BB_�Է���"));
		m_IgnoreAlias.Add(_T("UDF_�׷��"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalSpeciesCntAlias = _T("IBS_�����");
	}
		break;

	// ���������ڷ���
	case BO_ACQ_DONATE_DATA :
	{
		m_IgnoreAlias.Add(_T("UDF_���������ȣ����"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_IgnoreAlias.Add(_T("BP_�����ڸ�"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalSpeciesCntAlias = _T("IBS_�����");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ����");
	}
		break;

	// �����̵���ڷ���
	case BO_ACQ_NONDONATE_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_���������ȣ����"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_IgnoreAlias.Add(_T("BP_�����ڸ�"));
		m_IgnoreAlias.Add(_T("BP_�����δ����"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalSpeciesCntAlias = _T("IBS_�����");
	}
		break;

	// ��������ΰ����ڷ���
	case BO_ACQ_DONATE_REGOBJ_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_���������ȣ����"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_IgnoreAlias.Add(_T("BP_�����ڸ�"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalSpeciesCntAlias = _T("IBS_�����");
	}
		break;

	// ���������¹� ����Ʈ(����)
	case BO_DONATE_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_���������ȣ����"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_IgnoreAlias.Add(_T("BP_�����ڸ�"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalSpeciesCntAlias = _T("BP_�����ڸ�");
	}
		break;
	
	// ���Դ���ڷ���
	case BO_ACQ_PURCHASE_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_�����������"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ������");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");
		m_strTotalSpeciesCntAlias = _T("IBS_�����");
	}
		break;

	// ���������¹�����Ʈ(����)
	case BO_PURCHASE_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_�����������"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ������");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");
		m_strTotalSpeciesCntAlias = _T("IBS_�����");
	}
		break;

	// ��Ȱ���ڷ���
	case BO_ACQ_RECYCLE_DATA:
	{
		m_IgnoreAlias.Add(_T("IBS_������"));
		m_IgnoreAlias.Add(_T("UDF_�׷��"));
		m_IgnoreAlias.Add(_T("BP_���Ա���"));
		m_IgnoreAlias.Add(_T("CF_���Խ�û��"));
		m_IgnoreAlias.Add(_T("CF_���Խ�û��"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ����");
		m_strTotalSpeciesCntAlias = _T("IBS_�����");
	}
		break;

	// �˼�����ڷ���
	case BO_ACQ_CHECKINING_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_�����������"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_IgnoreAlias.Add(_T("BP_������"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ������");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");
		m_strTotalSpeciesCntAlias = _T("IBS_�����");
	}
		break;

	// ���Խ�û�ڷᵵ�����
	case BO_ACQ_CHECKIN_REQUEST_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_�����������"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_IgnoreAlias.Add(_T("BP_������"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ������");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");
	}
		break;

	// �˼��Ϸ��ڷ� ���
	case BO_ACQ_CHECKIN_COMPLETE_DATA:
	{
		m_IgnoreAlias.Add(_T("UDF_�����������"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_IgnoreAlias.Add(_T("BP_�˼���"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ������");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");
	}
		break;
	
	// ���Դ���ڷ���_��û�μ�����
	case BO_ACQ_PURCHASE_DATA_5163:		// 2005-01-29 5163��
	{
		m_IgnoreAlias.Add(_T("UDF_�����������"));
		m_IgnoreAlias.Add(_T("UDF_�������м���"));
		m_IgnoreAlias.Add(_T("UDF_�Է±��м���"));
		m_strTotalInfoTitle = _T("UDF_�Ϸù�ȣ");
		m_strTotalBookCntAlias = _T("UDF_å��");
		m_strTotalPriceAlias = _T("UDF_��ȯ������");
		m_strTotalDiscPriceAlias = _T("UDF_��ȯ����������");
		m_strTotalSpeciesCntAlias = _T("UDF_���δܰ�");
	}
		break;
			
	default :	return 0;	// m_nPrintType < 0
		break;
	}

	return 0;
}



//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - DM�� �����Ѵ�.
// [ �̴Ͻ��� ] - ����
//////////////////////////////////////////////////////////////////////
INT CMakeSimplePrintDM::MakePrintDM()
{
	INT ids;

	//================================================================
	// 1. ��¿� DM�� �����ϱ� ���� ��ҵ��� üũ�Ѵ�.
	//================================================================
	ids = CheckValid();
	if(ids < 0) return ids;

	if(	m_nPrintType == BO_ACQ_BASIC_DATA || 
		m_nPrintType == BO_ACQ_IMPORT_DATA || 
		m_nPrintType == BO_ACQ_DONATE_DATA || 
		m_nPrintType == BO_ACQ_NONDONATE_DATA ||
		m_nPrintType == BO_ACQ_DONATE_REGOBJ_DATA || 
		m_nPrintType == BO_ACQ_PURCHASE_DATA ||
		m_nPrintType == BO_ACQ_RECYCLE_DATA || 
		m_nPrintType == BO_ACQ_CHECKINING_DATA	||
		m_nPrintType == BO_PURCHASE_DATA || 
		m_nPrintType == BO_DONATE_DATA ||  
		m_nPrintType == DONATOR_LABEL ||  
		m_nPrintType == BO_ACQ_PURCHASE_DATA_5163 )   // 2005-01-29 5163�� �߰�.
	{
		ids = MakeSpeciesVolumePrintDM(m_pPrintBookDM);
	}
	else if( m_nPrintType == BO_ACQ_REQUEST_DATA )
		ids = MakeRequestPrintDM();

	return ids;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �Ϲݸ���Ʈ ��¿� DM�� �����Ѵ�.
// - m_pPrintDM - ��¿�DM
// - m_pPrintSourceDM - ����DM
//
// [ �̴Ͻ��� ]
// 1. ���α׷��� ��Ʈ�� â�� �����ش�.	
// 2. ��DM�� �����Ѵ�.
// 3. �� DM�� ���� �˸��ƽ��� �����Ѵ�.	
// 4. ���� ���DM���� �Ѱ������� �����Ѵ�.	
// 5. ��¿� DM�� Clear��Ų��.
// 6. ���� �� ���DM�� �ϳ��� Ȯ���Ͽ� �ش��ϴ� �� DM�� �����Ѵ�.
// 7. ���Ŀ��ΰ� Y��� ��¿� DM�� �����Ѵ�.
// 8. �Ѱ������� �ִ´�.
//
//////////////////////////////////////////////////////////////////////
INT CMakeSimplePrintDM::MakeSpeciesVolumePrintDM(CESL_DataMgr *pDM_BOOK)
{
	//================================================================
	// ��������
	//================================================================
	INT ids, nRowCnt, nSerialNo, i, nCondColumns = 0;
	INT nCond[2];
	DOUBLE dTotalOrderPrice = 0.0;
	CString strKey, strTmpData;	
	CString strAlias[2];
	


	//================================================================	
	// 1. ���α׷��� ��Ʈ�� â�� �����ش�.	
	//================================================================
	ShowDataProcessingDlg();
	
	// ����DM�� �������� �ʰ� ��¿� DM�� �����Ͽ� ����DM�� ������Ų��.
	// 2005.10.25 ������ ����
	/*
	//================================================================
	// 2. ���Ŀ��ΰ� Y��� ��¿� ���� DM�� �����Ѵ�.
	//================================================================
	if( m_bDataSortYN == TRUE ) 
		ids = PrintDataSortByUserSelect(m_pPrintSourceDM);	// ����
		//ids = PrintDataSortByUserSelect(m_pPrintDM);
	else 
		ids = PrintDataSortByBasicInfo(m_pPrintSourceDM);	// ������������ ( �׳ɸ��� )
		//ids = PrintDataSortByBasicInfo(m_pPrintDM);
	if(ids < 0)
	{
		if(m_pParentMgr->ESLAfxMessageBox(_T("����ڷ� �����۾��� ������ �� �����ϴ�. ��� ���� �Ͻðڽ��ϱ�?"), MB_YESNO) == IDNO)
			return 0;
	}
	*/

	//================================================================
	// 2. ��DM�� �����Ѵ�. - m_pPrintObjDM
	//================================================================
	ids = MakeVolumeDM(m_pPrintObjDM, pDM_BOOK, m_strDivAlias);		
	if(ids < 0) return ids;

	//================================================================
	// 3. �� DM�� ���� �˸��ƽ��� �����Ѵ�.	
	//================================================================	
	strAlias[nCondColumns] = m_strDivAlias;					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintObjDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	

	//================================================================
	// 4. ���� ���DM���� �Ѱ������� �����Ѵ�.	
	//================================================================
	ids = m_cTotalInfo.SetTotalInfo(m_pArrDivValeu, m_strDivAlias, m_pPrintSourceDM);
	if( ids < 0 )	return ids;


	//================================================================
	// 5. ��¿� DM�� Clear��Ų��.
	//================================================================
	m_pPrintDM->FreeData();

	//================================================================
	// 6. ���� �� ���DM�� �ϳ��� Ȯ���Ͽ� �ش��ϴ� �� DM�� �����Ѵ�.
	//================================================================
	nRowCnt = m_pPrintSourceDM->GetRowCount();
	if( m_pDataProcessingDlg != NULL )
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}

	//================================================================
	// 7. ���Ŀ��ΰ� Y��� ��¿� DM�� �����Ѵ�.
	//================================================================
	// �⺻ȭ�鿡�� ���Ǵ� ������ �ٷ� �����ϸ� �ȵǹǷ� ���̸� �ϳ� ���� �����Ѵ�.
	CESL_DataMgr pDummyDM;

	// Dummy DM�� Source DM�� ������ ������ �����.	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT col, nCols;
	nCols = m_pPrintSourceDM->m_nCols;	
	if(nCols < 0) nCols = m_pPrintSourceDM->GetRefColumnCount();
	ids = pDummyDM.InitDataMgrRef(nCols);
	for (col = 0; col < nCols; col++) {
		if (m_pPrintSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;		
		ids = pDummyDM.SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);		
	}
	
	for( i=0 ; i<nRowCnt ; i++ )
	{
		// ��¿� DM�� ����
		DWORD* pRow = (DWORD*)pDummyDM.InsertRow(-2);
		if( pRow == NULL )		continue;

		ids = CopyDM(m_pPrintSourceDM, i, &pDummyDM, pRow);
		if( ids < 0 )	return ids;		
	}	

	pDummyDM.RefreshPositionInfo();

	if( m_bDataSortYN == TRUE ) 	ids = PrintDataSortByUserSelect(&pDummyDM);
	else							ids = PrintDataSortByBasicInfo(&pDummyDM);
	if( ids < 0 )
	{
		if( m_pParentMgr->ESLAfxMessageBox(_T("����ڷ� �����۾��� ������ �� �����ϴ�. ��� ���� �Ͻðڽ��ϱ�?"), MB_YESNO) == IDNO )
		{
			return -1;
		}
	}	
	
	nSerialNo = 0;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		if( m_pDataProcessingDlg != NULL )	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i);
		
		strKey = pDummyDM.GetCellData(m_strDivAlias, i);
		if( CheckPrintData(strKey) == FALSE )		continue;
		
		// ��¿� DM�� ����
		DWORD* pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if( pRow == NULL )		continue;

		// ��DM�� ��¿� DM���� ����
		ids = CopyDM(&pDummyDM, i, m_pPrintDM, pRow);
		if( ids < 0 )	return ids;		
		
		// �Ϸù�ȣ ����
		strTmpData.Format(_T("%d"), ++nSerialNo);
		m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_�Ϸù�ȣ")), strTmpData);
		
		// ISBN ����
		strTmpData = m_pPrintSourceDM->GetCellData(_T("IBS_ISBN"), i);
		m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("BB_����ISBN")), strTmpData);
		// 2006.03.16 KBS - �μ��� ���� (����� �߰�)
		if( pDummyDM.GetCellData(_T("BP_��û����"), i) == _T("CLA") )
		{
			strTmpData.Empty();
			strTmpData = pDummyDM.GetCellData(_T("BP_�μ��ڵ�"), i);
			m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�����μ�"), strTmpData, strTmpData);
			m_pPrintDM->SetCellData(_T("UDF_�μ���"), strTmpData, i);
		}
		else	m_pPrintDM->SetCellData(_T("UDF_�μ���"), _T(""), i);

		// ��DM�� ��¿� DM���� ����( strKey = ��������KEY )
		ids = InsertObjInfo(m_pPrintDM, m_pPrintObjDM, strKey, SPECIES_PRINT);
		if( ids < 0 )	return ids;
		else if( ids == 1000 )	m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_�ܰ�")), _T(""));
		//strTmpData = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_��ȯ����������")) );
		strTmpData = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_��ȯ����")) );
		strTmpData.Replace(_T(","), _T(""));
		//dTotalOrderPrice += (DOUBLE)atof(strTmpData);
		TCHAR* pEndPtr = NULL;
		dTotalOrderPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
	}
	//m_cTotalInfo.m_dTotalPrice = dTotalOrderPrice;
	ids = m_pPrintDM->RefreshPositionInfo();
	if( ids < 0 )	return -1;

	/*
	//================================================================
	// 7. ���Ŀ��ΰ� Y��� ��¿� DM�� �����Ѵ�.
	//================================================================
	if( m_bDataSortYN == TRUE ) 	ids = PrintDataSortByUserSelect(m_pPrintDM);
	else							ids = PrintDataSortByBasicInfo(m_pPrintDM);
	if( ids < 0 )
	{
		if( m_pParentMgr->ESLAfxMessageBox(_T("����ڷ� �����۾��� ������ �� �����ϴ�. ��� ���� �Ͻðڽ��ϱ�?"), MB_YESNO) == IDNO )
		{
			return -1;
		}
	}	
	*/
	
	//================================================================
	// 8. �Ѱ������� �ִ´�.
	//================================================================
	m_pPrintDM->InsertRow(-1);	
	SetTotalInfo(m_pPrintDM, &m_cTotalInfo, m_pPrintDM->GetRowCount()-1, _T("�Ѱ�"), dTotalOrderPrice);
	// 2004.12.03 ȯ���Ѿ��� �־����
	// 2005-01-27 ȯ���Ѿ� �ּ�ó��
	//m_pPrintDM->InsertRow(-1);	m_pPrintDM->InsertRow(-1);
	//SetTotalInfo(m_pPrintDM, &m_cTotalInfo, m_pPrintDM->GetRowCount()-1, _T("ȯ���Ѿ�"));

	return 0;
}


//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��¿� DM�� �����ϱ� ���� �ʿ��� ��Ҹ� üũ�Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ����ƮŸ���� [��ġ�����Ȳ����Ʈ]�� ������ ��� ����ƮŸ�Ե���
//	  ���DM�� ������ �����Ѵ�.
// 2. �������DM�� ����DM�� ���ٸ� �����Ѵ�.
// 3. ���о˸��ƽ��� ���ٸ� �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CMakeSimplePrintDM::CheckValid()
{
	//================================================================
	// 1. ����ƮŸ���� [��ġ�����Ȳ����Ʈ]�� ������ ��� ����ƮŸ�Ե���
	//	  ���DM�� ������ �����Ѵ�.
	//================================================================
	if( m_nPrintType != BO_ACQ_REQUEST_DATA )
		if( m_pPrintDM == NULL || m_pPrintObjDM == NULL ) return -1;


	//================================================================
	// 2. �������DM�� ����DM�� ���ٸ� �����Ѵ�.
	//================================================================
	if( m_pPrintSourceDM == NULL || m_pPrintBookDM == NULL ) return -1;	
	

	//================================================================
	// 3. ���о˸��ƽ��� ���ٸ� �����Ѵ�.
	//================================================================
	if(m_strDivAlias.IsEmpty()) return -1;
	
	return 0;
}

INT CMakeSimplePrintDM::MakeRequestDataPrintDM()
{
	//���α׷��� ��Ʈ���� �����ش�.
	ShowDataProcessingDlg();
	
	// åDM�� ��ȯ�ϸ鼭 BP_�Է±��а��� '2' ���� �̿��� ������ �����Ѵ�.
	CESL_DataMgr DM_BOOK;
	DM_BOOK.SetCONNECTION_INFO(m_pPrintBookDM->CONNECTION_INFO);
	DM_BOOK.InitDMPKFromFile(m_pPrintBookDM->DB_MGR_ALIAS);
	DM_BOOK.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	CESL_DataMgr DM_SPECIES;
	DM_SPECIES.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	DM_SPECIES.InitDMPKFromFile(m_pPrintDM->DB_MGR_ALIAS);
	DM_SPECIES.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	INT ids = MakeRequestData(m_pPrintBookDM, &DM_BOOK);
	if(ids < 0) return -1;
	if(DM_BOOK.GetRowCount() == 0) return 1000;

	// ��DM�� �����Ѵ�.
	ids = MakeSpeciesDM(&DM_SPECIES, &DM_BOOK, FALSE);
	if(ids < 0) return ids;
	ids = SetUDFDMFieldData(&DM_SPECIES, TRUE);
	if(ids < 0) return ids;

	//����
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(&DM_SPECIES);
	else ids = PrintDataSortByBasicInfo(&DM_SPECIES);
	if(ids < 0)
	{
		if(m_pParentMgr->ESLAfxMessageBox(_T("����ڷ� �����۾��� ������ �� �����ϴ�. ��� ���� �Ͻðڽ��ϱ�?"), MB_YESNO) == IDNO)
			return 0;
	}

	//��ġ�����û�� ������ �����ؾ��Ѵ�.
	ids = SetRequestInfo(&DM_SPECIES);
	if(ids < 0) return ids;
	//�Ϸù�ȣ ����
	ids = SetDataSerialNo(&DM_SPECIES);
	if(ids < 0) return ids;

	//��DM�� �����Ѵ�.
	ids = MakeVolumeDM(m_pPrintObjDM, &DM_BOOK, m_strDivAlias);
	if(ids < 0) return ids;
	
	// �� DM�� ���� �˸��ƽ��� �����Ѵ�.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = m_strDivAlias;					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintObjDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	//���� ���DM���� �Ѱ������� �����Ѵ�.
	ids = m_cTotalInfo.SetTotalInfo(&DM_SPECIES);
	if(ids < 0) return ids;
	
	// ���DM�� Clear��Ų��.
	m_pPrintDM->FreeData();
	
	// ���� �� ���DM�� �ϳ��� Ȯ���Ͽ� �ش��ϴ� �� DM�� �����Ѵ�.
	INT nRowCnt = DM_SPECIES.GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	CString strKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(m_pDataProcessingDlg)	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i);
		
		strKey = DM_SPECIES.GetCellData(m_strDivAlias, i);
		
		DWORD * pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		if(pRow == NULL) return -1;
		ids = CopyDM(&DM_SPECIES, i, m_pPrintDM, pRow);
		if(ids < 0) return ids;
		
		ids = InsertObjInfo(m_pPrintDM, m_pPrintObjDM, strKey, SPECIES_PRINT);
		if(ids < 0) return ids;
		else if(ids == 1000) m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_�ܰ�")), _T(""));
	}
	ids = m_pPrintDM->RefreshPositionInfo();
	if(ids < 0) return ids;
	
	if( m_pPrintDM->GetRowCount() > 0 )
	{
	// �Ѱ������� �־ƾ���
		m_pPrintDM->InsertRow(-1);
		SetTotalInfo(m_pPrintDM, &m_cTotalInfo, m_pPrintDM->GetRowCount()-1, _T("�Ѱ�"));
	}

	return 0;
}

INT CMakeSimplePrintDM::MakeRequestData(CESL_DataMgr *pSourceBookDM, CESL_DataMgr *pTargetBookDM)
{
	if(pSourceBookDM == NULL || pTargetBookDM == NULL) return -1;
	INT nRowCnt = pSourceBookDM->GetRowCount();
	CString strTmpData;
	INT ids;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pSourceBookDM->GetCellData(_T("BP_�Է±���"), i);
		if(strTmpData == _T("2") && CheckPrintData(pSourceBookDM->GetCellData(m_strDivAlias, i)))
		{
			ids = CopyDM(pSourceBookDM, i, pTargetBookDM, -1);
			if(ids < 0) return -1;
		}
	}
	return 0;
}

INT CMakeSimplePrintDM::SetRequestInfo(CESL_DataMgr *pSourceDM)
{
	if(pSourceDM == NULL) return -1;
	if(pSourceDM->GetRowCount() == 0) return 0;

	CString strAcqKey;	strAcqKey.Empty();
	CString strQuery;
	INT nRowCnt = pSourceDM->GetRowCount();
	INT nTmpData, ids;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		nTmpData = (i+1)%600;
		if(nTmpData != 0)
		{
			if(strAcqKey.IsEmpty()) strAcqKey = pSourceDM->GetCellData(_T("BP_��������KEY"), i);
			else strAcqKey += _T(", ") + pSourceDM->GetCellData(_T("BP_��������KEY"), i);
		}
		else 
		{
			strQuery.Format(
				_T("SELECT ACQ_KEY, APPLICANT_NAME, APPLICANT_DATE ")
				_T("FROM BO_FURNISH_TBL ")
				_T("WHERE ACQ_KEY IN (%s)"), strAcqKey);
			ids = SetRequestInfo(pSourceDM, strQuery);
			if(ids < 0) return -1;
			strAcqKey.Empty();
		}
	}

	strQuery.Format(
		_T("SELECT ACQ_KEY, APPLICANT_NAME, APPLICANT_DATE ")
		_T("FROM BO_FURNISH_TBL ")
		_T("WHERE ACQ_KEY IN (%s)"), strAcqKey);
	ids = SetRequestInfo(pSourceDM, strQuery);
	if(ids < 0) return -1;
	
	return 0;
}

INT CMakeSimplePrintDM::SetRequestInfo(CESL_DataMgr * pSourceDM, CString strQuery)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pSourceDM->CONNECTION_INFO);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;

	INT nSourceCnt = pSourceDM->GetRowCount();
	INT nRowCnt = tmpDM.GetRowCount();
	CString strAcqKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strAcqKey = tmpDM.GetCellData(i, 0);
		for(INT j=0 ; j<nSourceCnt ; j++)
		{
			if(pSourceDM->GetCellData(_T("BP_��������KEY"), j) == strAcqKey) break;
		}
		if(nSourceCnt == j) continue;
		pSourceDM->SetCellData(_T("UDF_��û��"), tmpDM.GetCellData(i, 1), j);
		pSourceDM->SetCellData(_T("UDF_��û��"), tmpDM.GetCellData(i, 2), j);
	}
	return 0;
}

INT CMakeSimplePrintDM::MakeCheckInCompletePrintDM()
{
	DOUBLE dTotalOrderPrice = 0.0;
	CString strTmpData;
	//���α׷��� ��Ʈ���� �����ش�.
	ShowDataProcessingDlg();
	
	CESL_DataMgr DM_SPECIES;
	DM_SPECIES.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
	DM_SPECIES.InitDMPKFromFile(m_pPrintDM->DB_MGR_ALIAS);
	DM_SPECIES.SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	
	// ��DM�� �����Ѵ�.
	INT ids = MakeSpeciesDM(&DM_SPECIES, m_pPrintBookDM, FALSE);
	if(ids < 0) return ids;
	ids = SetUDFDMFieldData(&DM_SPECIES, TRUE);
	if(ids < 0) return ids;	

	//��DM�� �����Ѵ�.
	ids = MakeVolumeDM(m_pPrintObjDM, m_pPrintBookDM, m_strDivAlias);
	if(ids < 0) return ids;
	
	// �� DM�� ���� �˸��ƽ��� �����Ѵ�.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = m_strDivAlias;					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_���Ϸù�ȣ");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pPrintObjDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	SetTotalInfoAlias(m_strTotalBookCntAlias, m_strTotalPriceAlias, m_strTotalDiscPriceAlias);
	//���� ���DM���� �Ѱ������� �����Ѵ�.
	ids = m_cTotalInfo.SetTotalInfo(&DM_SPECIES);
	if(ids < 0) return ids;
	
	// ���DM�� Clear��Ų��.
	m_pPrintDM->FreeData();
	
	// ���� �� ���DM�� �ϳ��� Ȯ���Ͽ� �ش��ϴ� �� DM�� �����Ѵ�.
	INT nRowCnt = DM_SPECIES.GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}
	CString strKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(m_pDataProcessingDlg)
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i);
		
		strKey = DM_SPECIES.GetCellData(m_strDivAlias, i);
		DWORD* pRow = (DWORD*)m_pPrintDM->InsertRow(-2);
		ids = CopyDM(&DM_SPECIES, i, m_pPrintDM, pRow);
		if(ids < 0) return ids;
		
		ids = InsertObjInfo(m_pPrintDM, m_pPrintObjDM, strKey, SPECIES_PRINT);
		if(ids < 0) return ids;
		else if(ids == 1000) m_pPrintDM->SetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_�ܰ�")), _T(""));
		strTmpData = m_pPrintDM->GetCellData(pRow, m_pPrintDM->FindColumn(_T("UDF_��ȯ����������")) );
		strTmpData.Replace(_T(","), _T(""));
		//dTotalOrderPrice += (DOUBLE)atof(strTmpData);
		TCHAR* pEndPtr = NULL;
		dTotalOrderPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);
	}
	ids = m_pPrintDM->RefreshPositionInfo();
	if(ids < 0) return -1;

	//����
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(m_pPrintDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintDM);
	if(ids < 0)
	{
		if(m_pParentMgr->ESLAfxMessageBox(_T("����ڷ� �����۾��� ������ �� �����ϴ�. ��� ���� �Ͻðڽ��ϱ�?"), MB_YESNO) == IDNO)
			return 0;
	}

	if( m_pPrintDM->GetRowCount() > 0 )
	{
	// �Ѱ������� �־ƾ���
		m_pPrintDM->InsertRow(-1);
		SetTotalInfo(m_pPrintDM, &m_cTotalInfo, m_pPrintDM->GetRowCount()-1, _T("�Ѱ�"), dTotalOrderPrice);
	}

	return 0;
}

INT CMakeSimplePrintDM::PrintDataSortByBasicInfo(CESL_DataMgr *pDM)
{
	return 0;
}

INT CMakeSimplePrintDM::PrintDataSortByUserSelect(CESL_DataMgr *pDM)
{
	INT ids;
	if( m_pSortItem )
	{
		ids =  m_pSortItem->DataSort(pDM);
		if(ids < 0) return ids;
	}
	return 0;
}

INT CMakeSimplePrintDM::MakeRequestPrintDM()
{
	//���α׷��� ��Ʈ���� �����ش�.
	ShowDataProcessingDlg();
	
	INT ids;
	//����
	if(m_bDataSortYN) ids = PrintDataSortByUserSelect(m_pPrintBookDM);
	else ids = PrintDataSortByBasicInfo(m_pPrintBookDM);
	if(ids < 0)
	{
		if(m_pParentMgr->ESLAfxMessageBox(_T("����ڷ� �����۾��� ������ �� �����ϴ�. ��� ���� �Ͻðڽ��ϱ�?"), MB_YESNO) == IDNO)
			return 0;
	}
	
	// ���DM�� Clear��Ų��.
	m_pPrintSourceDM->FreeData();

	// 2005-03-11
	//if(m_pArrDivValeu && m_pArrDivValeu->GetCount() > 0)
	if(m_pArrDivValeu)
	{
		
		// ���� �� ���DM�� �ϳ��� Ȯ���Ͽ� �ش��ϴ� �� DM�� �����Ѵ�.
		INT nRowCnt = m_pPrintBookDM->GetRowCount();
		if(m_pDataProcessingDlg)
		{
			m_pDataProcessingDlg->SetMessage(_T("��� ������ �������Դϴ�."));
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
			m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
		}
		INT nSerialNo = 0;
		CString strKey, strTmpData;
		for(INT i=0 ; i<nRowCnt ; i++)
		{
			if(m_pDataProcessingDlg)
				m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i);
			
			strKey = m_pPrintBookDM->GetCellData(m_strDivAlias, i);
			// 2005-03-11
			if(m_pArrDivValeu->GetCount() > 0)
			{
				if(!CheckPrintData(strKey)) continue;
			}
			
			DWORD * pRow = (DWORD*)m_pPrintSourceDM->InsertRow(-2);
			if(pRow == NULL) continue;
			ids = CopyDM(m_pPrintBookDM, i, m_pPrintSourceDM, pRow);
			if(ids < 0) return ids;
			strTmpData.Format(_T("%d"), ++nSerialNo);
			// 2005-03-11
			// m_pPrintSourceDM->SetCellData(pRow, m_pPrintSourceDM->FindColumn(_T("UDF_�Ϸù�ȣ")), strTmpData);
			m_pPrintSourceDM->SetCellData(nSerialNo-1, m_pPrintSourceDM->FindColumn(_T("UDF_�Ϸù�ȣ")), strTmpData);
		}
		ids = m_pPrintSourceDM->RefreshPositionInfo();
		if(ids < 0) return -1;
	}

	// �Ѱ������� �־ƾ���
	return 0;
}
