// BasicStatistics.cpp: implementation of the CBasicStatistics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BasicStatistics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������
CBasicStatistics::CBasicStatistics(UINT IDD, CESL_Mgr * pParentMgr):CESL_Mgr(IDD, pParentMgr)
{
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDataProcessingDlg = NULL;
	m_pStatCondition = NULL;
	m_pGroupCollection		= NULL;
	///����ڷḦ �����ϴ� �������� ������ �����ϴ� ����
	m_bRunning = FALSE;
	m_bDonatorFlag	= FALSE;
	m_nStartRow = 0;
}

// �Ҹ���
CBasicStatistics::~CBasicStatistics()
{
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

// Progress Ctrl Dlg
VOID CBasicStatistics::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL)
	{
		m_pDataProcessingDlg = new CDataProcessingDlg(this);
	}
	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(this))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
}

// ��豸��
VOID CBasicStatistics::GetAcqStatistics()
{
	CString strMsg;
	strMsg.Format(_T("%s�� �����ϰ� �ֽ��ϴ�."), m_pStatCondition->GetStatType());
	m_pDataProcessingDlg->InitPos();
	m_pDataProcessingDlg->SetMessage(strMsg);

	EvalueateStatistics();
	
	InsertSumRowToTargetDM(m_nStartRow);
	
	InsertPriceSumRowToTargetDM(m_nStartRow);
	

	// 2012.11.14 UPDATE BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (���� ��ȯȭ�� ����)
	// �׸��� ��� ���� ���濡 ���� �ʿ������ �ּ�ó����

	// 2007.08.14 ADD BY PDJ
	// 0/0�� �Ǿ� �ִ� �׸��� 0(0)���� ����
	//CStringArray strArray;
	//CString strValue;
	//CString strOut;
	//INT nCol;
	//INT nRow;
	//INT i,y;
	
	//nRow = m_pDM->GetRowCount();
	//nCol = m_pDM->GetColCount();
	
	//for ( i=0 ; i<nRow ; i++ )
	//{
	//	for ( y=0 ; y<nCol ; y++ )
	//	{
	//		m_pDM->GetCellData(i, y, strValue );
	//		if ( strValue.FindOneOf(_T("/")) > 0 )
	//		{
	//			strArray.RemoveAll();
	//			m_pGroupCollection->ParsingStringApplyDuplicate(_T("/"), strValue, &strArray);
	//			strOut.Format(_T("%s(%s)"), strArray.GetAt(0), strArray.GetAt(1));
	//			m_pDM->SetCellData(i, y, strOut );			
	//		}
	//	}
	//}
	
	if( m_bRunning == FALSE )	return ;
	m_pDataProcessingDlg->EndDialog(IDOK);
	//::PostMessage(this->GetSafeHwnd(), QUERY_CREATE, 0, 0);
	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
	
	/*
	if( m_pGrid == NULL )	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
	else ::PostMessage(this->GetSafeHwnd(), QUERY_CREATE, 0, 0);
	*/
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - CStaticsCondition���� ����� �⺻ ������ ������ ������ �߰��Ͽ� �������� �ϼ��Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. �ӽ�DM�� ���������� �����Ѵ�.
// 2. �����ں� ����� ������ID�� ��Ű�� ������ ������ �Ϸ��Ѵ�.
// 3. �׿��� ��� ������ �ڵ尪�� �о�� �������� �ϼ��Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CBasicStatistics::EvalueateStatistics()
{
	if( m_bRunning == FALSE )	 return 0;
	
	//================================================================
	// ��������
	//================================================================
	INT codeCnt, curPos, i, startrow = -1;
	CString code, desc, tmpquery, stracqCode, strOption, strResult;
	CString codes[6];
	CESL_DataMgr tmpDM, donatorDM;
	

	//================================================================
	// 1. �ӽ�DM�� ���������� �����Ѵ�.
	//================================================================
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	donatorDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);


	//================================================================
	// 2. �����ں� ����� ������ID�� ��Ű�� ������ ������ �Ϸ��Ѵ�.
	// 2005.11.16 ������ �߰�
	//================================================================
	if( m_bDonatorFlag == TRUE )
	{
		//=========================================================
		//2008.10.17 UPDATE BY PJW : �޴����ڵ带 DEFINE�Ѵ�.
		// donatorDM.RestructDataManager(_T("SELECT REC_KEY, DONATOR_ID, DONATOR FROM CO_DONATOR_TBL ORDER BY DONATOR"));
		donatorDM.RestructDataManager(_T("SELECT REC_KEY, DONATOR_ID, DONATOR FROM CO_DONATOR_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE ORDER BY DONATOR"));
		//=========================================================
		
		codeCnt = donatorDM.GetRowCount();
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, codeCnt);
		curPos = m_pDataProcessingDlg->GetProgressCtrl()->GetPos();
		for( i=-1; i<codeCnt; i++)
		{
			if( m_bRunning == FALSE )	return 0;
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++curPos);

			if( i==-1 ) 
			{
				strOption.Format(_T(" AND ( %s IS NULL OR NOT %s IN (SELECT REC_KEY FROM CO_DONATOR_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE) )"), m_pStatCondition->GetVerticalField(), m_pStatCondition->GetVerticalField());
				desc = _T("�������");
			}
			else
			{
				strOption.Format(_T(" AND %s = %s"), m_pStatCondition->GetVerticalField(), donatorDM.GetCellData(i,0));
				desc = donatorDM.GetCellData(i,2) + _T("  [") + donatorDM.GetCellData(i,1) + _T("]");
			}
			tmpquery = m_pStatCondition->GetWhereStmt() + strOption + _T(" GROUP BY p.SUBJECT_CODE ");	
			tmpDM.RestructDataManager(tmpquery);			
			CopyTargetDM(&tmpDM, desc, m_pStatCondition->GetAcqType());
		}
	}
	//================================================================
	// 3. �׿��� ��� ������ �ڵ尪�� �о�� �������� �ϼ��Ѵ�.
	//================================================================
	else
	{
		// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		CString strTempQuery;
		codeCnt = m_pStatCondition->GetVerticalDescCount();
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, codeCnt);
		curPos = m_pDataProcessingDlg->GetProgressCtrl()->GetPos();
		for( i=0 ; i<codeCnt ; i++ )
		{
			if( m_bRunning == FALSE )	 return 0;
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++curPos);
			startrow = m_pInfo->pCodeMgr->GetCode(m_pStatCondition->GetVerticalDesc(), codes, 6, ++startrow);
			code = codes[2];
			desc = codes[3];
			
			code.TrimLeft(); code.TrimRight();
			desc.TrimLeft(); desc.TrimRight();
			if(code.IsEmpty()) 
			{
				strOption.Format(_T(" AND (%s = '%s' OR %s IS NULL)"), m_pStatCondition->GetVerticalField(), code, m_pStatCondition->GetVerticalField());
			}
			else
			{
				strOption.Format(_T(" AND %s = '%s'"), m_pStatCondition->GetVerticalField(), code);
			}

			strTempQuery = m_pStatCondition->GetWhereStmt();

			if ( strTempQuery.Find(_T("B.REG_CODE")) > 0 )
			{
				tmpquery =  strTempQuery + strOption + _T(" GROUP BY B.REG_CODE ");	
			}
			else if ( strTempQuery.Find(_T("B.ACQ_CODE")) > 0  )
			{
				tmpquery =  strTempQuery + strOption + _T(" GROUP BY B.ACQ_CODE ");	
			}
			else
			{
				tmpquery =  strTempQuery + strOption + _T(" GROUP BY SUBSTR(B.CLASS_NO, 1, 1) ");
			}
			
			tmpDM.RestructDataManager(tmpquery);
			CopyTargetDM(&tmpDM, desc, m_pStatCondition->GetAcqType());
		}
		*/
		INT nRunCount = m_pStatCondition->GetVerticalDescCount() +1;		
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRunCount);
		INT nCurPos = m_pDataProcessingDlg->GetProgressCtrl()->GetPos();

		CString strTempQuery;
		CString strTempCodes;

		INT nStartIndex = 0;

		for (i=0; i<nRunCount; i++)
		{
			if (!m_bRunning)
				return 0;

			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(++nCurPos);

			if (nRunCount-1 > i)
			{
				CString strCode, strDesc;
				m_pInfo->pCodeMgr->GetCode(m_pStatCondition->GetVerticalDesc(), _T("�ڵ�"), strCode, nStartIndex);
				nStartIndex = m_pInfo->pCodeMgr->GetCode(m_pStatCondition->GetVerticalDesc(), _T("����"), strDesc, nStartIndex);
				nStartIndex++;

				strCode.TrimLeft(); strCode.TrimRight();
				strDesc.TrimLeft(); strDesc.TrimRight();

				if(strCode.IsEmpty()) 
				{
					strOption.Format(_T(" AND (%s = '%s' OR %s IS NULL)"), m_pStatCondition->GetVerticalField(), strCode, m_pStatCondition->GetVerticalField());
				}
				else
				{
					strOption.Format(_T(" AND %s = '%s'"), m_pStatCondition->GetVerticalField(), strCode);

					if (strTempCodes.IsEmpty())
						strTempCodes.Format(_T("'%s'"), strCode);
					else
					{
						CString strTempBuf = strTempCodes;
						strTempCodes.Format(_T("%s,'%s'"), strTempBuf, strCode);
					}
				}

				strTempQuery = m_pStatCondition->GetWhereStmt();

				if (strTempQuery.Find(_T("B.REG_CODE")) > 0)
				{
					tmpquery =  strTempQuery + strOption + _T(" GROUP BY B.REG_CODE ");	
				}
				else if (strTempQuery.Find(_T("B.ACQ_CODE")) > 0)
				{
					tmpquery =  strTempQuery + strOption + _T(" GROUP BY B.ACQ_CODE ");	
				}
				else
				{
					tmpquery =  strTempQuery + strOption + _T(" GROUP BY SUBSTR(B.CLASS_NO, 1, 1) ");
				}

				tmpDM.RestructDataManager(tmpquery);

				if(strCode.IsEmpty())
				{
					CopyTargetDM(&tmpDM, strDesc, m_pStatCondition->GetAcqType());
				}
				else
				{
					CString strDisp;
					strDisp.Format(_T("%s(%s)"), strDesc, strCode);

					CopyTargetDM(&tmpDM, strDisp, m_pStatCondition->GetAcqType());
				}
			}
			else // ��Ÿ �߰�
			{
				strOption.Format(_T(" AND %s NOT IN (%s) AND %s IS NOT NULL"), m_pStatCondition->GetVerticalField(), strTempCodes, m_pStatCondition->GetVerticalField());

				strTempQuery = m_pStatCondition->GetWhereStmt();

				if (strTempQuery.Find(_T("B.REG_CODE")) > 0)
				{
					tmpquery =  strTempQuery + strOption + _T(" GROUP BY B.REG_CODE ");	
				}
				else if (strTempQuery.Find(_T("B.ACQ_CODE")) > 0)
				{
					tmpquery =  strTempQuery + strOption + _T(" GROUP BY B.ACQ_CODE ");	
				}
				else
				{
					tmpquery =  strTempQuery + strOption + _T(" GROUP BY SUBSTR(B.CLASS_NO, 1, 1) ");
				}

				tmpDM.RestructDataManager(tmpquery);

				CopyTargetDM(&tmpDM, _T("��Ÿ"), m_pStatCondition->GetAcqType());				
			}
			// KOL.RED.2018.210 ---------------------------------------------------------------------------
		}
	}	
	return 0;
}

INT CBasicStatistics::InsertSumRowToTargetDM(INT startRow)
{
	if( m_bRunning == FALSE )	return 0;	
	if( startRow < 0 )			return 0;	
	if( m_pDM->GetRowCount() == startRow )	return 0;
	
	//================================================================
	// ��������
	//================================================================
	INT nCurRow, nBookCnt, nSpeciesCnt, i, j;
	CString strSpeciesCnt, strBookCnt, tmpstr, tmpstr1, tmpstr3, strTmpData;	
	
	m_pDM->InsertRow(-1);
	nBookCnt = nSpeciesCnt = 0;
	nCurRow = m_pDM->GetRowCount() - 1;		
	CESL_ControlMgr *m_pCM = FindCM(_T("CM_�ڻ����"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));	
	INT nColCnt = m_pGrid->GetCols( 0 );
	for( i=0 ; i<nColCnt ; i++ )
	{
		if( m_bRunning == FALSE )	return 0;
		nSpeciesCnt = nBookCnt = 0;
		/*
		if( i == 0 )	tmpstr = _T("�������");
		else if( i == 1 )	tmpstr = _T("����");
		else if( i == 2 )	tmpstr = _T("����");
		else if( i >= 3 && i < nColCnt-2 )	tmpstr = _T("��Ÿ");
		else if(i == nColCnt-2) tmpstr = _T("��");
		else tmpstr.Format(_T("%d"), i-1);
		*/
		if( i == 0 )	tmpstr = _T("�������");
		else if( i == 1 )	tmpstr = _T("�������(å)");
		else if( i == 2 )	tmpstr = _T("����");
		else if( i == 3 )	tmpstr = _T("����(å)");
		else if( i == 4 )	tmpstr = _T("����");
		else if( i == 5 )	tmpstr = _T("����(å)");
		else if( i == 6 )	tmpstr = _T("��Ÿ");
		else if( i == 7 )	tmpstr = _T("��Ÿ(å)");
		else if( i == 8 )	tmpstr = _T("��");
		else if(i == nColCnt-2) tmpstr = _T("��(å)");
		else tmpstr.Format(_T("%d"), i-1);

		for( j=startRow ; j<nCurRow ; j++ )
		{
			strTmpData = m_pDM->GetCellData(tmpstr, j);
			CStringArray tmpArray;
			m_pGroupCollection->ParsingStringApplyDuplicate(_T("/"), strTmpData, &tmpArray);
			if(tmpstr.Compare(_T("�������"))==0 || tmpstr.Compare(_T("����"))==0 || tmpstr.Compare(_T("����"))==0 || tmpstr.Compare(_T("��Ÿ"))==0 || tmpstr.Compare(_T("��"))==0 )
			{
				nSpeciesCnt += _ttoi(tmpArray.GetAt(0));
			}
			else if(tmpstr.Compare(_T("�������(å)"))==0 || tmpstr.Compare(_T("����(å)"))==0 || tmpstr.Compare(_T("����(å)"))==0 || tmpstr.Compare(_T("��Ÿ(å)"))==0 || tmpstr.Compare(_T("��(å)"))==0 )
				nBookCnt += _ttoi(tmpArray.GetAt(0));			
		}
		//tmpstr1.Format(_T("%d/%d"), nSpeciesCnt, nBookCnt);
	
		tmpstr1.Format(_T("%d"), nSpeciesCnt);
		tmpstr3.Format(_T("%d"), nBookCnt);


		if(tmpstr.Compare(_T("�������"))==0 || tmpstr.Compare(_T("����"))==0 || tmpstr.Compare(_T("����"))==0 || tmpstr.Compare(_T("��Ÿ"))==0 || tmpstr.Compare(_T("��"))==0 )
		{
			m_pDM->SetCellData(tmpstr, tmpstr1, nCurRow);
		}
		else if(tmpstr.Compare(_T("�������(å)"))==0 || tmpstr.Compare(_T("����(å)"))==0 || tmpstr.Compare(_T("����(å)"))==0 || tmpstr.Compare(_T("��Ÿ(å)"))==0 || tmpstr.Compare(_T("��(å)"))==0 )
			m_pDM->SetCellData(tmpstr, tmpstr3, nCurRow);
	}
	m_pDM->SetCellData(_T("�ڷᱸ�м���"), _T("��"), nCurRow);
	
	return nCurRow+1;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������ ���ο� ������� ���� dm�� �����հ��� ���� ���Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ���DM�� ���� �ϳ��� �߰��Ѵ�. (�Ѱ踦 ��Ÿ������ ����)
// 2. ����� �÷��׿� ���� Į�� ������ ����ȴ�.
// 3. ������ ���ο� �Ѱ������� �����Ѵ�. (Į��->�ο�)
//		- 11/22 : ��
//		- 12/23 : �����հ�
//
//////////////////////////////////////////////////////////////////////
INT CBasicStatistics::InsertPriceSumRowToTargetDM(INT startRow)
{
	//================================================================
	// ��������
	//================================================================
	INT rowCnt, nColCnt, nCurRow, i, j;
	DOUBLE fPrice = 0.0;
	CString strPrice, strAlias, strPriceAlias;


	if( m_bRunning == FALSE )	return 0;
	if( startRow < 0 )			return 0;
	
	rowCnt = m_pDM->GetRowCount();
	if( rowCnt == startRow )	return 0;
	

	//================================================================
	// 1. ���DM�� ���� �ϳ��� �߰��Ѵ�. (�Ѱ踦 ��Ÿ������ ����)
	//================================================================
	m_pDM->InsertRow(-1);
	nCurRow = m_pDM->GetRowCount() - 1;
	m_pDM->SetCellData(_T("�ڷᱸ�м���"), _T("�����հ�"), nCurRow);
	

	//================================================================
	// 2. ����� �÷��׿� ���� Į�� ������ ����ȴ�.
	//================================================================
	
	CESL_ControlMgr *m_pCM = FindCM(_T("CM_�ڻ����"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));	
	nColCnt = m_pGrid->GetCols( 0 );

	/*
	if( _T("Y") == m_strIs90 )	nColCnt = 24;
	else						nColCnt = 13;
	*/	

	//================================================================
	// 3. ������ ���ο� �Ѱ������� �����Ѵ�. (Į��->�ο�)
	//		- 11/22 : ��
	//		- 12/23 : �����հ�
	//================================================================	
	for( i=0 ; i<nColCnt ; i++ )
	{
		fPrice = 0.0;
		if( m_bRunning == FALSE )	return 0;
		
		if ( i == 0 )
		{
			strAlias = _T("�������");
		}
		else if ( i == 1 )
		{
			strAlias = _T("����");
		}
		else if ( i == 2 )
		{
			strAlias = _T("����");
		}
		else if ( i == 3 )
		{
			strAlias = _T("��Ÿ");
		}
		else if ( i == nColCnt-2 )
		{
			m_pDM->SetCellData(_T("��"), _T("0"), nCurRow);
			continue;
		}
		else if ( i == nColCnt-1 )
		{
			strAlias = _T("�����հ�");
		}
		else
		{
			strAlias.Format(_T("%d"), i-1);
		}		

		for( j=startRow ; j<nCurRow ; j++ )
		{
			if( strAlias == _T("�����հ�") )	strPriceAlias = strAlias;
			else								strPriceAlias = strAlias + _T("_����");

			strPrice = m_pDM->GetCellData(strPriceAlias, j);
			strPrice.Replace(_T(","), _T(""));
			//fPrice += (DOUBLE)atof(strPrice.GetBuffer(0));
			TCHAR* pEndPtr = NULL;
			fPrice += _tcstod(strPrice.GetBuffer(0), &pEndPtr);

		}

		strPrice.Format(_T("%.0f"), fPrice);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strPrice, strPrice);
		m_pDM->SetCellData(strAlias, strPrice, nCurRow);
	}
	
	
	return nCurRow+1;
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ӽ÷� ���ؿ� DM�� ���DM�� ����ڷ� ���Ŀ� �°� ä���ش�.
//
// [ �̴Ͻ��� ]
// 1. ���DM�� �� ������ �߰��Ѵ�.
// 2. ���ڷ� �� DM�� ���� ���ٸ� ����� �����Ƿ� ��� 0���� ä���ش�.
// 3. ���DM�� ���ڷο� DM�� ������ �߰��Ѵ�.
// 4. �������� DM�� �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////
INT CBasicStatistics::CopyTargetDM(CESL_DataMgr *pDM, CString desc, ACQ_TYPE nAcqType)
{
	//================================================================
	// ��������
	//================================================================
	INT cnt, nCurRow, nBookCnt, nSpeciesCnt, i ;
	DOUBLE fPrice = 0.0;
	CString strBookCnt, strSpeciesCnt, strTmpData, strTmpData2, strTmpData3, tmpstr, tmpstr2, strPrice, strPrice2;

	//================================================================	
	// 1. ���DM�� �� ������ �߰��Ѵ�.
	//================================================================		
	m_pDM->InsertRow(-1);
	nCurRow = m_pDM->GetRowCount() - 1;	

	//================================================================
	// 2. ���ڷ� �� DM�� ���� ���ٸ� ����� �����Ƿ� ��� 0���� ä���ش�.
	//================================================================	

	nBookCnt = nSpeciesCnt = 0;
	cnt = pDM->GetRowCount();	
	if( cnt == 0 )
	{
		m_pDM->SetCellData(_T("�ڷᱸ�м���"),	desc,	nCurRow);
		return 0;
	}

	//================================================================	
	// 3. ���DM�� ���ڷο� DM�� ������ �߰��Ѵ�.
	//================================================================	
	for( i=0 ; i<cnt ; i++ )
	{
		tmpstr			= pDM->GetCellData(i, 0);	// p.Subject_code
		strSpeciesCnt	= pDM->GetCellData(i, 1);	// COUNT(DISTINCT(I.REC_KEY))
		strBookCnt		= pDM->GetCellData(i, 2);	// COUNT(DISTINCT(B.REC_KEY))
		strPrice		= pDM->GetCellData(i, 3);	// SUM(B.PRICE) or SUM(GetPriceWon(B.ACQ_KEY, B.PRICE, 'Y'))
				

		// �ش� �ڵ尪�� ã�Ƽ� ��/�ǰ� �� ���ݰ��� �����Ѵ�.
		tmpstr.TrimLeft();		
		tmpstr.TrimRight();
		if ( tmpstr.IsEmpty() )		tmpstr = _T("�������");		
		else if ( tmpstr == _T("1") )	tmpstr = _T("����");		
		else if ( tmpstr == _T("2") )	tmpstr = _T("����");		
		else						tmpstr = _T("��Ÿ");	

		CStringArray tmpArray;		
		m_pDM->GetCellData(tmpstr, nCurRow, strTmpData2);
		m_pGroupCollection->ParsingStringApplyDuplicate(_T("/"), strTmpData2, &tmpArray);

		// _T("��/��")�� ���·� ���� ����

		// 2012.11.14 UPDATE BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� (���� ��ȯȭ�� ����)
		//strTmpData.Format(_T("%d/%d"), 
		//	_ttoi(strSpeciesCnt) + _ttoi(tmpArray.GetAt(0)), _ttoi(strBookCnt) + _ttoi(tmpArray.GetAt(0))); 		
		strTmpData.Format(_T("%d"),_ttoi(strSpeciesCnt) + _ttoi(tmpArray.GetAt(0)));
		strTmpData3.Format(_T("%d"),_ttoi(strBookCnt) + _ttoi(tmpArray.GetAt(0)));

		if(tmpstr.Compare(_T("�������"))==0) tmpstr2 = _T("�������(å)");
		else if (tmpstr.Compare(_T("����"))==0) tmpstr2 = _T("����(å)");
		else if (tmpstr.Compare(_T("����"))==0) tmpstr2 = _T("����(å)");
		else if (tmpstr.Compare(_T("��Ÿ"))==0) tmpstr2 = _T("��Ÿ(å)");
		
		m_pDM->SetCellData(tmpstr, strTmpData, nCurRow);
		m_pDM->SetCellData(tmpstr2, strTmpData3, nCurRow);

		/////////////////////////////////////////

		m_pDM->GetCellData(tmpstr+_T("_����"), nCurRow, strPrice2);

		//strTmpData.Format(_T("%f"), (DOUBLE)atof(strPrice.GetBuffer(0)) + (DOUBLE)atof(strPrice2.GetBuffer(0)) );
		TCHAR* pEndPtr = NULL;
		strTmpData.Format(_T("%f"), _tcstod(strPrice.GetBuffer(0), &pEndPtr) + _tcstod(strPrice2.GetBuffer(0), &pEndPtr) );
		
		m_pDM->SetCellData(tmpstr+_T("_����"),	strTmpData,	nCurRow);
		
		tmpstr.Format(_T("%d"), nAcqType);
		m_pDM->SetCellData(_T("���Ա���"), tmpstr, nCurRow);
		m_pDM->SetCellData(_T("�ڷᱸ�м���"), desc, nCurRow);
		
		strPrice.Replace(_T(","), _T(""));

		// �� ������ ���Ѵ�.
		nBookCnt += _ttoi(strBookCnt.GetBuffer(0));
		nSpeciesCnt += _ttoi(strSpeciesCnt.GetBuffer(0));
		
		//fPrice += (DOUBLE)atof(strPrice.GetBuffer(0));
		fPrice += _tcstod(strPrice.GetBuffer(0), &pEndPtr);
	}
	
	//================================================================		
	// 4. �������� DM�� �����Ѵ�.
	//================================================================		
	tmpstr.Format(_T("%d"), nSpeciesCnt);
	tmpstr2.Format(_T("%d"), nBookCnt);
	strPrice.Format(_T("%.0f"), fPrice);
	
	m_pDM->SetCellData(_T("��"), tmpstr, nCurRow);
	m_pDM->SetCellData(_T("��(å)"), tmpstr2, nCurRow);
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strPrice, strPrice);
	m_pDM->SetCellData(_T("�����հ�"), strPrice, nCurRow);	

	return 0;
}

CString CBasicStatistics::CheckValidateGroupNo(CString strGroupInfo, CString strAcqYear, CString strWorkCode)
{
	INT nInitCnt = -1;

	CStringArray arrDivNo;		arrDivNo.RemoveAll();
	m_pGroupCollection->ParsingString(_T(","), strGroupInfo, &arrDivNo);
	
	INT cnt = arrDivNo.GetSize();
	nInitCnt = cnt;

	INT ids = -1;

	CString strMsg;
	for(INT i=0 ; i<cnt && cnt>0; i++)
	{
		ids = m_pGroupCollection->CheckValidate(arrDivNo.GetAt(i), strAcqYear, strWorkCode);
		if(ids < 0)
		{
			if(m_pGroupCollection->GetAcqType() == PURCHASE)
				strMsg.Format(_T("�Է��Ͻ� ������ȣ [%s]�� ����� �� �����ϴ�."), arrDivNo.GetAt(i));
			else if(m_pGroupCollection->GetAcqType() == DONATE)
				strMsg.Format(_T("�Է��Ͻ� ������ȣ [%s]�� ����� �� �����ϴ�."), arrDivNo.GetAt(i));
			ESLAfxMessageBox(strMsg);
			arrDivNo.RemoveAt(i);
			i--;
			cnt--;
		}
	}
	if(nInitCnt != cnt)
	{
		CString strTmpData;		strTmpData.Empty();
		for(i=0 ; i<cnt ; i++)
		{
			if(strTmpData.IsEmpty()) strTmpData = arrDivNo.GetAt(i);
			else strTmpData += _T(", ") + arrDivNo.GetAt(i);
		}
		return strTmpData;
	}

	return strGroupInfo;
}
