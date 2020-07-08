// LocStatisticsApi.cpp: implementation of the CLocStatisticsApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LocStatisticsApi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLocStatisticsApi::CLocStatisticsApi(CESL_Mgr* pParentMgr)
{
	m_pMgr = pParentMgr;
	m_pDM_QUERY = NULL;
	m_pDM_RESULT = NULL;
	m_pDM_SUM = NULL;
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2018.07.11 ADD BY KHY: ����� ����Ʈ ����
/*//NEW------------------------------------------------------------------------*/
	m_pDM_LIST = NULL;
/*//END------------------------------------------------------------------------*/
	// KOL.RED.2018.211 ---------------------------------------------------------------------------
	m_strWhere = _T("");
	m_strExtraWhere = _T("");
	m_strClassField = _T("");

	m_bWonbuCondition = FALSE;
	m_nTYPE = 1; //default
	m_arrayWorkingStatus.RemoveAll();

}

CLocStatisticsApi::~CLocStatisticsApi()
{

}

// �޷��� ������ ��¥ ��Ʈ���� �� ��������
CString CLocStatisticsApi::GetDate( CESL_Mgr* pEslMgr , UINT nCtrlID )
{
	CString sDate;
	
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )pEslMgr->GetDlgItem( nCtrlID );
	
	CTime tTime;
	
	DWORD dwResult = pDate->GetTime( tTime );

	if (dwResult != GDT_VALID ) return _T("");

	sDate.Format( _T("%04d/%02d/%02d"), tTime.GetYear(), tTime.GetMonth(), tTime.GetDay() );
	
	return sDate;
}

INT CLocStatisticsApi::RunStatisticsProc(CString strClassName)
{

	if(!m_pDM_QUERY || !m_pDM_RESULT || !m_pDM_SUM) 
	{
		AfxMessageBox(_T("���� ������ ��ü�� ã�� �� �����ϴ�!"));
		return -1;
	}
	

	
	INT ids = 0;
	
	//DM�� �����ϰ� 
	ids = SetQueryDM(strClassName);
	if(ids<0) return -1;

	//�����͸� �������� 
	ids = RefreshQueryDM();
	if(ids<0) return -1;

	//��� DM�� ����
	ids = MakeResultDM(strClassName);
	if(ids<0) return -1;

	
	return 0;
}

INT CLocStatisticsApi::SetQueryDM(CString strClassName)
{
	//TBL_NAME 
	if(m_bWonbuCondition)
	{
		
		m_pDM_QUERY->TBL_NAME = _T("IDX_SE_TBL I,SE_SPECIES_TBL S, SE_BOOK_TBL B,")
								_T(" SE_ACQ_TYPE_VOL_TBL V, CO_ACCESSION_REC_TBL W");

	}
	else
	{
		m_pDM_QUERY->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S, SE_BOOK_TBL B, SE_ACQ_TYPE_VOL_TBL V");
	}

	m_pDM_SUM->TBL_NAME = m_pDM_QUERY->TBL_NAME;
	
	
	//������ 
	INT nColIdx;
	nColIdx = m_pDM_QUERY->FindColumn(_T("������"));
	

	//��Ÿ ����
	if(_T("������ڵ�") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("������"),_T("I.PUB_FREQ"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), I.PUB_FREQ");
		m_strClassField = _T("I.PUB_FREQ");
		
	}
	else if(_T("�ڷᱸ��") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("������"),_T("I.MAT_CODE"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), I.MAT_CODE");
		m_strClassField = _T("I.MAT_CODE");
	}
	else if(_T("��ϱ���") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("������"),_T("B.REG_CODE"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), B.REG_CODE");
		m_strClassField = _T("B.REG_CODE");

	}
	else if(_T("�ڷ�Ǳ���") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("������"),_T("B.SHELF_LOC_CODE"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), B.SHELF_LOC_CODE");	
		m_strClassField = _T("B.SHELF_LOC_CODE");

	}
	else if(_T("��ġ��ȣ") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("������"),_T("B.SEPARATE_SHELF_CODE"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), B.SEPARATE_SHELF_CODE");		
		m_strClassField = _T("B.SEPARATE_SHELF_CODE");
	}
	else if(_T("�谡����") == strClassName)
	{
		m_pDM_QUERY->SetDataMgrRef(nColIdx,_T("������"),_T("B.WORKING_STATUS"),_T("str"),_T(""),_T(""));
		m_pDM_QUERY->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1), B.WORKING_STATUS");		
		m_strClassField = _T("B.WORKING_STATUS");
	}

	return 0;
}

INT CLocStatisticsApi::RefreshQueryDM()
{
	m_pDM_QUERY->RefreshDataManager(m_strWhere);
	
	INT nRowCnt = m_pDM_QUERY->GetRowCount();
	
	//===================================================
	//2010.01.11 ADD BY PJW : �Ϸ�Ǿ����ϴٸ� �ѷ��ش�.
//	if(0 == nRowCnt)
//	{
//		AfxMessageBox(_T("�˻� ����� �����ϴ�!"));
//		return -1;
//	}
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("�˻� ����� �����ϴ�!"));
		return -1;
	}
	else
	{
		AfxMessageBox(_T("�Ϸ�Ǿ����ϴ�!"));
	}
	//===================================================
	

	return 0;
}

INT CLocStatisticsApi::MakeResultDM(CString strClassName)
{
	m_pDM_RESULT->FreeData();
	
	//���DM�� �������� setting�Ѵ�
	//��� DM�� ù row���� �� ���� header(�з���ȣ)�� �ִ´�
	INT nKDCCode = 256;
	INT nDefaultValue = 0;
	CString strKDCCode[256], strKDCDesc[256];
	nDefaultValue = 0;

	m_pMgr->m_pInfo->pCodeMgr->GetCode(_T("KDC��������ȣ����"), strKDCCode, strKDCDesc, nKDCCode, nDefaultValue);	
	INT nKDCCodeCnt = m_pMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("KDC��������ȣ����"));

	INT idx = 0;
	m_pDM_RESULT->InsertRow(-1);
	CString strAlias;
	
	for(INT i=0;i<nKDCCodeCnt;i++)
	{
		
		strAlias.Format(_T("%d"),i+1);
		m_pDM_RESULT->SetCellData(strAlias,strKDCCode[i],idx);
	}

	m_pDM_RESULT->SetCellData(_T("�հ�"),_T("��"),idx);
	

	//���DM�� �������� setting�Ѵ�
	if(_T("�谡����") == strClassName) //�谡 ������ ��� 
	{
		INT nSize = m_arrayWorkingStatus.GetSize();
		for(INT i=0;i<nSize;i++)
		{
			m_pDM_RESULT->InsertRow(-1);
			idx = m_pDM_RESULT->GetRowCount()-1;
			m_pDM_RESULT->SetCellData(_T("������"),m_arrayWorkingStatus.GetAt(i),idx);
		}

	}
	else //�Ϲ����� ��� 
	{
		INT nCode = 256;
		CString strCode[256], strDesc[256];
		nDefaultValue = 0;

		
		m_pMgr->m_pInfo->pCodeMgr->GetCode(strClassName, strCode, strDesc, nCode, nDefaultValue);	
		INT nCodeCnt = m_pMgr->m_pInfo->pCodeMgr->GetCodeCount(strClassName);
		
		
		for(i=0;i<nCodeCnt;i++)
		{
			m_pDM_RESULT->InsertRow(-1);
			idx = m_pDM_RESULT->GetRowCount()-1;
			m_pDM_RESULT->SetCellData(_T("������"),strCode[i],idx);
		}
	}

	//���DM�� ����DM�� ī���Ѵ� 
	CopyQueryDMToResultDM();

	//��� DM�� ������ �ٷ� �� row���� �ߺ���� �հ�(���� �� ���� ���Ѱ�)�� �ִ´�.
	SetDuplicatedSumInResultDM();

	//��� DM�� ������ row���� �࿭ ���ǿ� ���� �հ� query����� �ִ´�
	SetQuerySumInResultDM();

	//���DM�� �ڵ带 �������� �ٲ۴�
	ConvertCodeToDescInResultDM(strClassName);


	return 0;
}
VOID CLocStatisticsApi::CopyQueryDMToResultDM()
{
	INT nResultRow = m_pDM_RESULT->GetRowCount();
	INT nQueryRowCnt = m_pDM_QUERY->GetRowCount();
	CString strRow;
	CString strColumn;
	CString strResultRow;
	CString strResultColumn;
	CString strAlias;


	for(INT i=0;i<nQueryRowCnt;i++)
	{
		
		strRow = _T("");
		strColumn = _T("");
		
		strRow = m_pDM_QUERY->GetCellData(_T("������"),i);
		strColumn = m_pDM_QUERY->GetCellData(_T("�з���ȣ"),i);
		
	
		for(INT j=1;j<nResultRow;j++)
		{
			strResultRow = m_pDM_RESULT->GetCellData(_T("������"),j);
			
			if(strRow == strResultRow)
			{
				for(INT k=1;k<12;k++)
				{
					strAlias.Format(_T("%d"),k);
					strResultColumn = m_pDM_RESULT->GetCellData(strAlias,0);

					if(strColumn == strResultColumn)
					{
						SetDataInResultDM(i,j,strAlias);
					}
				}

			}
		
		}
	}

}


VOID CLocStatisticsApi::SetDataInResultDM(INT nQueryRow, INT nResultRow, CString strColumnAlias)
{
	CString strSpeciesCnt,strVolCnt,strBookCnt;

	strSpeciesCnt = m_pDM_QUERY->GetCellData(_T("����"),nQueryRow);
	strVolCnt = m_pDM_QUERY->GetCellData(_T("�Ǽ�"),nQueryRow);
	strBookCnt = m_pDM_QUERY->GetCellData(_T("å��"),nQueryRow);

	CString strFormat;
	strFormat.Format(_T("%s,%s,%s"),strSpeciesCnt,strVolCnt,strBookCnt);
	m_pDM_RESULT->SetCellData(strColumnAlias,strFormat,nResultRow);
}

VOID CLocStatisticsApi::SetDuplicatedSumInResultDM()
{
	//�ߺ� column�հ踦 ���Ѵ�
	INT nSpeciesSum,nVolSum,nBookSum;
	nSpeciesSum = nVolSum = nBookSum = 0;
	
	INT nRowCnt = m_pDM_RESULT->GetRowCount();
	CString strAlias;
	CString strData;
	CString strFormat;

	for(INT i=1;i<nRowCnt;i++)
	{
		for(INT j=1;j<12;j++)
		{
			strAlias.Format(_T("%d"),j);
			strData	= m_pDM_RESULT->GetCellData(strAlias,i);
			strData.TrimLeft();
			strData.TrimRight();
			
			if(!strData.IsEmpty()) 
			{
				GetSumTotal(strData,&nSpeciesSum,&nVolSum,&nBookSum);
			}
		
		}
		
		strFormat.Format(_T("%d,%d,%d"),nSpeciesSum,nVolSum,nBookSum);
		m_pDM_RESULT->SetCellData(_T("�ߺ��հ�"),strFormat,i);
		nSpeciesSum = nVolSum = nBookSum = 0;
	}

	
	
	//��� DM�� �ߺ� row �հ� ������ �� row �߰�
	nSpeciesSum = nVolSum = nBookSum = 0;
	m_pDM_RESULT->InsertRow(-1);
	INT nIdx = m_pDM_RESULT->GetRowCount()-1;
	
	for(i=1;i<12;i++)
	{
		strAlias.Format(_T("%d"),i);
		
		for(INT j=1;j<nRowCnt;j++)
		{
			strData	= m_pDM_RESULT->GetCellData(strAlias,j);
			strData.TrimLeft();
			strData.TrimRight();

			if(!strData.IsEmpty()) 
			{
				GetSumTotal(strData,&nSpeciesSum,&nVolSum,&nBookSum);
			}

		}

		strFormat.Format(_T("%d,%d,%d"),nSpeciesSum,nVolSum,nBookSum);
		m_pDM_RESULT->SetCellData(strAlias,strFormat,nIdx);
		nSpeciesSum = nVolSum = nBookSum = 0;
	}
	

	//�� �հ��߰�
	nSpeciesSum = nVolSum = nBookSum = 0;

	for(i=1;i<nRowCnt;i++)
	{
		strData	= m_pDM_RESULT->GetCellData(_T("�ߺ��հ�"),i);
		strData.TrimLeft();
		strData.TrimRight();

		if(!strData.IsEmpty()) 
		{
			GetSumTotal(strData,&nSpeciesSum,&nVolSum,&nBookSum);
		}

	}
	
	strFormat.Format(_T("%d,%d,%d"),nSpeciesSum,nVolSum,nBookSum);
	m_pDM_RESULT->SetCellData(_T("�ߺ��հ�"),strFormat,nIdx);

}

VOID CLocStatisticsApi::SetQuerySumInResultDM()
{

	//�� ��, ���� �հ踦 ������ ���ؼ� �ߺ����� ���Ѵ�
	//(��,���� ���� group by �� total�� ���Ѵ�)
	CString strSpeciesCnt,strVolCnt,strBookCnt;
	CString strFormat;



	//GROUP ����
	INT nColIdx;
	nColIdx = m_pDM_SUM->FindColumn(_T("GROUP"));
	
	
	//column(�з���ȣ)�� ���
	CString strCode;
	CString strColumn;
	m_pDM_SUM->SetDataMgrRef(nColIdx,_T("GROUP"),_T("SUBSTR (B.CLASS_NO, 0, 1)"),_T("str"),_T(""),_T(""));
	m_pDM_SUM->EXTRA_CONDITION = _T("GROUP BY SUBSTR (B.CLASS_NO, 0, 1)");
	m_pDM_SUM->RefreshDataManager(m_strWhere);

	INT nRowCnt = m_pDM_SUM->GetRowCount();

	CString strAlias;
	m_pDM_RESULT->InsertRow(-1);
	INT nResultIdx = m_pDM_RESULT->GetRowCount()-1;

	if(nRowCnt>0)
	{
		for(INT i=0;i<nRowCnt;i++)
		{
			strCode = m_pDM_SUM->GetCellData(_T("GROUP"),i);
			strCode.TrimLeft();
			strCode.TrimRight();
			
			for(INT j=1;j<12;j++)
			{
				strAlias.Format(_T("%d"),j);

				strColumn = m_pDM_RESULT->GetCellData(strAlias,0); //column header
				strColumn.TrimLeft();
				strColumn.TrimRight();

				if(strCode == strColumn)
				{
					strSpeciesCnt = m_pDM_SUM->GetCellData(_T("����"),i);
					strVolCnt = m_pDM_SUM->GetCellData(_T("�Ǽ�"),i);
					strBookCnt = m_pDM_SUM->GetCellData(_T("å��"),i);
						
					strFormat.Format(_T("%s,%s,%s"),strSpeciesCnt,strVolCnt,strBookCnt);
					m_pDM_RESULT->SetCellData(strAlias,strFormat,nResultIdx);

				}

			}
		

		}
	}

	
	//row(����)�� ���
	CString strExtraCondition;
	strExtraCondition.Format(_T("GROUP BY %s"),m_strClassField);
		
	m_pDM_SUM->SetDataMgrRef(nColIdx,_T("GROUP"),m_strClassField,_T("str"),_T(""),_T(""));
	m_pDM_SUM->EXTRA_CONDITION = strExtraCondition;
	m_pDM_SUM->RefreshDataManager(m_strWhere);

	nRowCnt = m_pDM_SUM->GetRowCount();
	INT nResultRowCnt = m_pDM_RESULT->GetRowCount()-2;


	if(nRowCnt>0)
	{
		for(INT i=0;i<nRowCnt;i++)
		{
			strCode = m_pDM_SUM->GetCellData(_T("GROUP"),i);
			strCode.TrimLeft();
			strCode.TrimRight();
			
			for(INT j=1;j<nResultRowCnt;j++)
			{
				strColumn = m_pDM_RESULT->GetCellData(_T("������"),j);
				strColumn.TrimLeft();
				strColumn.TrimRight();

				if(strCode == strColumn)
				{
					strSpeciesCnt = m_pDM_SUM->GetCellData(_T("����"),i);
					strVolCnt = m_pDM_SUM->GetCellData(_T("�Ǽ�"),i);
					strBookCnt = m_pDM_SUM->GetCellData(_T("å��"),i);
					
					strFormat.Format(_T("%s,%s,%s"),strSpeciesCnt,strVolCnt,strBookCnt);
					m_pDM_RESULT->SetCellData(_T("�հ�"),strFormat,j);

				}

			}
		

		}
	}


	//���� �հ� 
	CString strQuery;
	CString strResult[3];
	strQuery.Format(_T("SELECT COUNT(DISTINCT S.REC_KEY), COUNT(DISTINCT V.REC_KEY),COUNT(DISTINCT B.REC_KEY)")
					_T(" FROM %s WHERE %s"),m_pDM_SUM->TBL_NAME,m_strWhere);

	m_pDM_SUM->GetValuesQuery(strQuery,strResult,3);
				
	strFormat.Format(_T("%s,%s,%s"),strResult[0],strResult[1],strResult[2]);
	
	INT nLastIdx = m_pDM_RESULT->GetRowCount()-1;
	m_pDM_RESULT->SetCellData(_T("�հ�"),strFormat,nLastIdx);
}


VOID CLocStatisticsApi::ConvertCodeToDescInResultDM(CString strClassName)
{

	//�� 
	if(strClassName == _T("�谡����"))
	{
		INT nRowCnt = m_pDM_RESULT->GetRowCount();
		CString strStatus;
		
		for(INT i=0;i<nRowCnt;i++)
		{
			strStatus = m_pDM_RESULT->GetCellData(_T("������"),i);
			strStatus =	GetWorkingStatusDesc(strStatus);
			if(!strStatus.IsEmpty()) m_pDM_RESULT->SetCellData(_T("������"),strStatus,i);
		}
	}
	else
	{
		m_pMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM_RESULT,strClassName,_T("������"));
	}
	
	CString strAlias;
	CString strCode,strDesc;
	
	//��
	for(INT i=1;i<12;i++)
	{
		strAlias.Format(_T("%d"),i);
		strCode = m_pDM_RESULT->GetCellData(strAlias,0);
		m_pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC��������ȣ����"),strCode,strDesc);
		m_pDM_RESULT->SetCellData(strAlias,strDesc,0);

	}


}


VOID CLocStatisticsApi::GetSumTotal(CString strData,INT* nSpeciesSum,INT* nVolSum,INT* nBookSum)
{
	if(strData.GetLength() == 0) return ;
	INT idx, startidx, lastidx;
	idx = startidx = 0;
	lastidx = strData.GetLength();
	idx = strData.Find(_T(","), startidx);

	//�� 
	if(idx > 0) *nSpeciesSum += _ttoi((strData.Left(idx)).GetBuffer(0));

	startidx = idx + 1;
	idx = strData.Find(_T(","), startidx);
	
	//��
	if(idx > 0)	*nVolSum += _ttoi((strData.Mid(startidx, idx - 1)).GetBuffer(0));

	startidx = idx + 1;
	
	//å
	*nBookSum += _ttoi((strData.Right(lastidx - startidx)).GetBuffer(0));
}

CString CLocStatisticsApi::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = '%s'"),strFieldName,strData1);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

CString CLocStatisticsApi::MakeIntRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = %s"),strFieldName,strData1);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN %s AND %s"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

CString CLocStatisticsApi::GetWorkingStatusDesc(CString strStatus)
{
	if(strStatus.IsEmpty()) return _T("");
	
	CString strtmp;

	if( strStatus.Compare(_T("SEL311O")) == 0 )
	{
		strtmp.Format( _T("���ܴ����ڷ�") );
	}
	else if( strStatus.Compare(_T("SEL312O")) == 0 )
	{
		strtmp.Format( _T("���������ڷ�") );
	}
	//=====================================================
	//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
	// 18.09.27 JMJ KOL.RED.2018.008
	else if( strStatus.Compare(_T("SEL411O")) == 0 )
	{
		strtmp.Format( _T("����å�η����������ڷ�") );
	}
	else if( strStatus.Compare(_T("SEL511O")) == 0 )
	{
		strtmp.Format( _T("���յ���Ÿ���ݳ�") );
	}
	//=====================================================
	else if( strStatus.Compare(_T("SEL313O")) == 0 )
	{
		strtmp.Format( _T("Ư�������ڷ�") );
	}
	else if( strStatus.Compare(_T("SEL314O")) == 0 )
	{
		strtmp.Format( _T("��޴����ڷ�") );
	}
	else if( strStatus.Compare(_T("SEL315O")) == 0 )
	{
		strtmp.Format( _T("����ڷ�") );
	}
	else if( strStatus.Compare(_T("SEL212N")) == 0 )
	{
		strtmp.Format( _T("��ġ�ڷ� ") );
	}
	else if( strStatus.Compare(_T("SEL213O")) == 0 )
	{
		strtmp.Format( _T("���������ڷ�") );
	}
	else if( strStatus.Compare(_T("SEL216O")) == 0 )
	{
		strtmp.Format( _T("�н��ڷ�") );
	}
	else if( strStatus.Compare(_T("SEL215O")) == 0 )
	{
		strtmp.Format( _T("�����ڷ�") );
	}
	else if( strStatus.Compare(_T("SEL214O")) == 0 )
	{
		strtmp.Format( _T("�ļ��ڷ�") );
	}
	else if( strStatus.Compare(_T("SEL112N")) == 0 )
	{
		strtmp.Format( _T("�ǽð��谡�ڷ�") );
	}

	return strtmp;
}

// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2018.07.11 ADD BY KHY: ����� ����Ʈ ����
/*//NEW------------------------------------------------------------------------*/
INT CLocStatisticsApi::RunStatisticsListProc(CString strClassName, CString sXValue, CString sYValue)
{
	if(!m_pDM_QUERY || !m_pDM_RESULT || !m_pDM_SUM) 
	{
		AfxMessageBox(_T("���� ������ ��ü�� ã�� �� �����ϴ�!"));
		return -1;
	}
	
	//TBL_NAME 
	if(m_bWonbuCondition)
	{		
		m_pDM_LIST->TBL_NAME = _T("IDX_SE_TBL I,SE_SPECIES_TBL S, SE_BOOK_TBL B,")
								_T(" SE_ACQ_TYPE_VOL_TBL V, CO_ACCESSION_REC_TBL W");
	}
	else
	{
		m_pDM_LIST->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S, SE_BOOK_TBL B, SE_ACQ_TYPE_VOL_TBL V");
	}
	
		
	CString strWhereQuery = MakeSearcListQuery(strClassName, sXValue, sYValue);
	m_pDM_LIST->RefreshDataManager(strWhereQuery);

	return 0;
}

CString CLocStatisticsApi::MakeSearcListQuery(CString strClassName, CString sXValue, CString sYValue)
{
	CString strExtraWhere = _T("");
	CString sTmpWhere;
	INT ids;

	//��
	m_pMgr->m_pInfo->pCodeMgr->ConvertDescToCode(_T("KDC��������ȣ����"), sXValue, sXValue);
	if(sXValue != _T("�հ�"))
	{
		if(!sXValue.IsEmpty())
		{
			sTmpWhere.Format( _T(" AND SUBSTR (B.CLASS_NO, 0, 1)='%s'"), sXValue);
		}
		else
		{
			sTmpWhere.Format( _T(" AND SUBSTR (B.CLASS_NO, 0, 1) IS NULL"));
		}	
		strExtraWhere += sTmpWhere;
	}

	//�� 
	if(strClassName == _T("�谡����"))
	{
		sYValue = GetWorkingStatusDescToCode(sYValue);
	}
	else
	{
		ids = m_pMgr->m_pInfo->pCodeMgr->ConvertDescToCode(strClassName , sYValue, sYValue);
	}
	if(sYValue != _T("�հ�"))
	{
		if(!sYValue.IsEmpty())
		{
			sTmpWhere.Format( _T(" AND %s='%s'"), m_strClassField, sYValue);
		}
		else
		{
			sTmpWhere.Format( _T(" AND %s IS NULL"), m_strClassField);
		}	
		strExtraWhere += sTmpWhere;
	}

	return m_strWhere + strExtraWhere;
}
CString CLocStatisticsApi::GetWorkingStatusDescToCode(CString strStatus)
{
	if(strStatus.IsEmpty()) return _T("");
	
	CString strtmp;

	if( strStatus.Compare(_T("���ܴ����ڷ�")) == 0 )
	{
		strtmp.Format( _T("SEL311O") );
	}
	else if( strStatus.Compare(_T("���������ڷ�")) == 0 )
	{
		strtmp.Format( _T("SEL312O") );
	}
	else if( strStatus.Compare(_T("���յ���Ÿ���ݳ�")) == 0 )
	{
		strtmp.Format( _T("SEL511O") );
	}
	else if( strStatus.Compare(_T("Ư�������ڷ�")) == 0 )
	{
		strtmp.Format( _T("SEL313O") );
	}
	else if( strStatus.Compare(_T("��޴����ڷ�")) == 0 )
	{
		strtmp.Format( _T("SEL314O") );
	}
	else if( strStatus.Compare(_T("����ڷ�")) == 0 )
	{
		strtmp.Format( _T("SEL315O") );
	}
	else if( strStatus.Compare(_T("��ġ�ڷ�")) == 0 )
	{
		strtmp.Format( _T("SEL212N") );
	}
	else if( strStatus.Compare(_T("���������ڷ�")) == 0 )
	{
		strtmp.Format( _T("SEL213O") );
	}
	else if( strStatus.Compare(_T("�н��ڷ�")) == 0 )
	{
		strtmp.Format( _T("SEL216O") );
	}
	else if( strStatus.Compare(_T("�����ڷ�")) == 0 )
	{
		strtmp.Format( _T("SEL215O") );
	}
	else if( strStatus.Compare(_T("�ļ��ڷ�")) == 0 )
	{
		strtmp.Format( _T("SEL214O") );
	}
	else if( strStatus.Compare(_T("�ǽð��谡�ڷ�")) == 0 )
	{
		strtmp.Format( _T("SEL112N") );
	}

	return strtmp;
}

INT CLocStatisticsApi::SetDataMgrRefList( CESL_DataMgr *pDM, CString sCondition, CString sInitValue, CString sGroupYN, INT nCol )
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
// KOL.RED.2018.211 ---------------------------------------------------------------------------