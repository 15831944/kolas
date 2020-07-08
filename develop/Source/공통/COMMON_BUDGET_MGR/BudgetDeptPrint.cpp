// BudgetDeptPrint.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetListDept.h"
#include "BudgetDeptPrint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetDeptPrint dialog


CBudgetDeptPrint::CBudgetDeptPrint(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetDeptPrint)
		// NOTE: the ClassWizard will add member initialization here

	m_pDM		= NULL;
	m_pParentDM = NULL;
	m_pCM		= NULL;
	m_nSPECIES1	= 0;
	m_nSPECIES2 = 0;
	m_nBOOK1	= 0;
	m_nBOOK2	= 0;
	m_nPRICE1	= 0;
	m_nPRICE2	= 0;
	m_nCodeCnt  = 0;

	//}}AFX_DATA_INIT
	
	// KOL.UDF.022 ��ť���ڵ� ����
	m_pGrid = NULL;
}

CBudgetDeptPrint::~CBudgetDeptPrint()
{

}

VOID CBudgetDeptPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetDeptPrint)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBudgetDeptPrint, CDialog)
	//{{AFX_MSG_MAP(CBudgetDeptPrint)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetDeptPrint message handlers


INT CBudgetDeptPrint::InitControl()
{
	this->SetWindowText(_T("�б⺰ ��������"));
	m_pGrid->SetTextMatrix(0,0,_T("   ��  ó"));
	m_pGrid->SetColWidth(0,0,1600);
		
	return 0;
}

VOID CBudgetDeptPrint::SetParentInfo(CESL_DataMgr * pDM)
{
	m_pParentDM = pDM;
}

//====================================================================
// [�Լ�����]
//====================================================================
VOID CBudgetDeptPrint::ShowAllData()
{
	if(!m_pParentDM)
	{
		AfxMessageBox(_T(" �θ� DM�� �����ϴ�.!!"));
		return;
	}
	
	INT idx = m_pParentDM->GetRowCount();
	if(idx < 1)
	{
		AfxMessageBox(_T("�ش� ���� ����� ������ �����ϴ�.!!"));
		return;
	}
/*	
	CString data[COUNT] = {
	_T("  ����  ����"),
	_T("  �ι�����ó"),
	_T("  ��ȸ����ó"),
	_T("  ��  ��  ó"),
	_T("  ��  ��  ó"),
	_T("  ü��  ��ó"),
	_T("  ����  ��ó"),
	_T("  ��  ��  ��"),
	_T("  ���Ⱓ�๰"),
	_T("  ����  ����"),
	_T("  ��      ��")};
*/

	INT nCode = 256;
	INT nDefaultValue = 0;
	CString strCode[256], strDesc[256];
	nDefaultValue = 0;

	
	m_pInfo->pCodeMgr->GetCode(_T("�к�"), strCode, strDesc, nCode, nDefaultValue);	
	m_nCodeCnt = m_pInfo->pCodeMgr->GetCodeCount(_T("�к�"));

	strDesc[m_nCodeCnt] = _T("�հ�");
	m_nCodeCnt += 1;

	
	idx = 0;

	for(INT i=1;i<m_nCodeCnt+1;i++)
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("UDF_��������"),_T("�Ϲ�"),i*2-2);
		m_pDM->SetCellData(_T("UDF_����"),_T("\\"),i*2-2);
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("UDF_��������"),_T("����"),i*2-1);
		m_pDM->SetCellData(_T("UDF_����"),_T("\\"),i*2-1);	
	}
/*	//==========================================================================
	// ��ó�� �Ϲ�/������ ��������, ���������� DM�� ������ ���´�.
	//==========================================================================
	for(INT i = 1; i<COUNT+1; i++)
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("UDF_��������"),_T("�Ϲ�"),i*2-2);
		m_pDM->SetCellData(_T("UDF_����"),_T("\\"),i*2-2);
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("UDF_��������"),_T("����"),i*2-1);
		m_pDM->SetCellData(_T("UDF_����"),_T("\\"),i*2-1);
	}
*/	
	//==========================================================================
	// �������������� ���ؼ� DM�� ������ ������ �����Ѵ�.
	//==========================================================================
	if(SetDMBySeqNo() < 1) // ���� ������ ���´�.
	{
		AfxMessageBox(_T("������ ���������� �����ϴ�."));
	}

	m_pCM->AllControlDisplay();
	
	//==========================================================================
	// �� 0 ��° �÷��� ��ó�� ����
	//==========================================================================
	for(i = 1; i<m_nCodeCnt+1; i = i++)
	{
		if(i == 1){
			m_pGrid->SetTextMatrix(i,0,strDesc[i-1]);
			m_pGrid->SetTextMatrix(i+1,0,strDesc[i-1]);
		}
		else{
			m_pGrid->SetTextMatrix(i*2-1,0,strDesc[i-1]);
			m_pGrid->SetTextMatrix(i*2,0,strDesc[i-1]);
		}
	}

	//==========================================================================
	// ��ó���� �׸��� ����
	//==========================================================================

	i = 0;
	for(INT j=1 ; j<m_nCodeCnt ; j+=2)
	{
		m_pGrid->SetMergeCells(2);
		m_pGrid->SetMergeCol(0,TRUE);
		m_pGrid->SetTextMatrix(j,0,strDesc[i]); 
		m_pGrid->SetTextMatrix(j+1,0,strDesc[i]); 
		
		i++;
	}
}
//==========================================================================
// [�Լ�����]
//  ������ ��ó���� 2 ROW(�Ϲ�/����)��  DM�� ���� Update�Ѵ�.
//  - argument 1. ��ó�� ��Ī�Ǵ� ��ȣ
// [MINI SPEC]
//  1.�Ϲ�/���� ���� ������ Update�Ѵ�.
//  2.�Ϲ�/���� ���� �Ǽ��� Update�Ѵ�.
//  3.�Ϲ�/���� ���� ������ Update�Ѵ�.
//  4.�ش� �ɹ������� �ʱ�ȭ �Ѵ�. 
//==========================================================================
INT CBudgetDeptPrint::SaveDM(INT nDept)
{
	CString sSPECIES1, sSPECIES2, sBOOK1, sBOOK2, sPRICE1, sPRICE2;
	CString sTemp;
	sSPECIES1.Format(_T("%d"),m_nSPECIES1);
	sSPECIES2.Format(_T("%d"),m_nSPECIES2);
	sBOOK1.Format(_T("%d"),m_nBOOK1);
	sBOOK2.Format(_T("%d"),m_nBOOK2);
	sPRICE1.Format(_T("%d"),m_nPRICE1);
	sPRICE2.Format(_T("%d"),m_nPRICE2);
	
	//----------------------------------------------------------------------
	// 1.�Ϲ�/���� ���� ������ Update�Ѵ�.
	//----------------------------------------------------------------------
	sTemp = m_pDM->GetCellData(_T("UDF_����"),nDept*2);
	m_pDM->SetCellData(_T("UDF_����"),ModifyCell(sTemp, sSPECIES1),nDept*2   );
	sTemp = m_pDM->GetCellData(_T("UDF_����"),nDept*2+1);
	m_pDM->SetCellData(_T("UDF_����"),ModifyCell(sTemp, sSPECIES2),nDept*2+1 );

	//----------------------------------------------------------------------
	// 2.�Ϲ�/���� ���� �Ǽ��� Update�Ѵ�.
	//----------------------------------------------------------------------
	sTemp = m_pDM->GetCellData(_T("UDF_�Ǽ�"),nDept*2);
	m_pDM->SetCellData(_T("UDF_�Ǽ�"),ModifyCell(sTemp, sBOOK1),nDept*2   );
	sTemp = m_pDM->GetCellData(_T("UDF_�Ǽ�"),nDept*2+1);
	m_pDM->SetCellData(_T("UDF_�Ǽ�"),ModifyCell(sTemp, sBOOK2),nDept*2+1 );
	
	//----------------------------------------------------------------------
	// 3.�Ϲ�/���� ���� ������ Update�Ѵ�.
	//----------------------------------------------------------------------
	sTemp = m_pDM->GetCellData(_T("UDF_����"),nDept*2);
	m_pDM->SetCellData(_T("UDF_����"),ModifyCell(sTemp, sPRICE1),nDept*2   );
	sTemp = m_pDM->GetCellData(_T("UDF_����"),nDept*2+1);
	m_pDM->SetCellData(_T("UDF_����"),ModifyCell(sTemp, sPRICE2),nDept*2+1 );
	
	//----------------------------------------------------------------------
	// 4.�ش� �ɹ������� �ʱ�ȭ �Ѵ�. 
	//----------------------------------------------------------------------
	m_nSPECIES1 = 0;
	m_nSPECIES2 = 0;
	m_nBOOK1	= 0;
	m_nBOOK2	= 0;
	m_nPRICE1	= 0;
	m_nPRICE2	= 0;
	
	return 0;		
}

//==========================================================================
// [�Լ�����]
//  ���� ���� ������ �߰��Ǵ� ������ ���� ���� �����Ѵ�.
//  - argument 
//		1. ���� ������
//		2. �߰� ������  
//  - return
//      1. Update�� �� ����
//==========================================================================
CString CBudgetDeptPrint::ModifyCell(CString str1, CString str2)
{
	if(str1.IsEmpty() == TRUE)
		str1 = _T("0");
	if(str2.IsEmpty() == TRUE)
		str2 = _T("0");
	CString sResult;
	INT number;
	number = _ttoi(str1) + _ttoi(str2);
	sResult.Format(_T("%d"), number);

	return sResult;
}

INT CBudgetDeptPrint::SetDMBySeqNo()
{
	INT nCnt;
	CString sQuery, result, sREC_KEY;
	CESL_DataMgr tempDM;
	tempDM.SetCONNECTION_INFO(m_pParentDM->CONNECTION_INFO);
	
	// ��Ű�� �����´�.
	m_pParentDM->GetCellData(_T("������KEY"),0,sREC_KEY);

	// ������ ������ ������ �����´�.
	sQuery.Format(_T("SELECT EXECUTE_GROUP_KEY FROM ")
		_T("CO_BUDGET_EXECUTION_TBL WHERE ")
		_T("BUDGET_COMPILATION_KEY IN ")
		_T("(SELECT REC_KEY ")
		_T("FROM CO_BUDGET_COMPILATION_TBL ")
		_T("WHERE REC_KEY = %s)"), sREC_KEY);

	tempDM.RestructDataManager(sQuery);
	nCnt = tempDM.GetRowCount();

	if(nCnt < 1)
		return -1;

	
	for(INT i = 0; i< nCnt ; i++)
	{
		sREC_KEY.Empty();
		sREC_KEY = tempDM.GetCellData(i,0);
		if(sREC_KEY.IsEmpty() == FALSE)
			GetSpeciesCnt(sREC_KEY);
	}
	
	SaveDMSUM();
	
	return nCnt;
}

INT CBudgetDeptPrint::SaveDMSUM()
{
	CString sTemp;
	INT nTotalSpeciesCnt1, nTotalBookCnt1, nTotalPrice1;
	INT nTotalSpeciesCnt2, nTotalBookCnt2, nTotalPrice2;
	
	nTotalSpeciesCnt1 = 0;
	nTotalBookCnt1    = 0;
	nTotalPrice1      = 0;
	nTotalSpeciesCnt2  = 0;
	nTotalBookCnt2    = 0;
	nTotalPrice2      = 0;

	sTemp.Empty();
	for(INT i = 0; i<m_nCodeCnt-1 ; i++)
	{
		// �Ϲ�
		m_pDM->GetCellData(_T("UDF_����"),i*2,sTemp);
		nTotalSpeciesCnt1 += _ttoi(sTemp);
		m_pDM->GetCellData(_T("UDF_�Ǽ�"),i*2,sTemp);
		nTotalBookCnt1 += _ttoi(sTemp);
		m_pDM->GetCellData(_T("UDF_����"),i*2,sTemp);
		nTotalPrice1 += _ttoi(sTemp);
		
		// ����
		m_pDM->GetCellData(_T("UDF_����"),i*2+1,sTemp);
		nTotalSpeciesCnt2 += _ttoi(sTemp);
		m_pDM->GetCellData(_T("UDF_�Ǽ�"),i*2+1,sTemp);
		nTotalBookCnt2 += _ttoi(sTemp);
		m_pDM->GetCellData(_T("UDF_����"),i*2+1,sTemp);
		nTotalPrice2 += _ttoi(sTemp);
	}
	
	sTemp.Format(_T("%d"),nTotalSpeciesCnt1);
	m_pDM->SetCellData(_T("UDF_����"),sTemp,m_nCodeCnt*2-2);
	sTemp.Format(_T("%d"),nTotalBookCnt1);
	m_pDM->SetCellData(_T("UDF_�Ǽ�"),sTemp,m_nCodeCnt*2-2);
	sTemp.Format(_T("%d"),nTotalPrice1);
	m_pDM->SetCellData(_T("UDF_����"),sTemp,m_nCodeCnt*2-2);
	sTemp.Format(_T("%d"),nTotalSpeciesCnt2);
	m_pDM->SetCellData(_T("UDF_����"),sTemp,m_nCodeCnt*2-1);
	sTemp.Format(_T("%d"),nTotalBookCnt2);
	m_pDM->SetCellData(_T("UDF_�Ǽ�"),sTemp,m_nCodeCnt*2-1);
	sTemp.Format(_T("%d"),nTotalPrice2);
	m_pDM->SetCellData(_T("UDF_����"),sTemp,m_nCodeCnt*2-1);

	return 0;
}

INT CBudgetDeptPrint::GetSpeciesCnt(CString data)
{
	if(data.IsEmpty() == TRUE)
	{
		return -1;
	}

	//������ȣ�� ���Ѵ�.
	CString sQuery, sSeq_No, sAcq_Year, sPrice;
	INT nSpeciesCnt;
	CESL_DataMgr tempDM;
	tempDM.SetCONNECTION_INFO(m_pParentDM->CONNECTION_INFO);

	
	sQuery.Format(_T("SELECT SEQ_NO FROM BO_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s"),data);
	m_pDM->GetOneValueQuery(sQuery,sSeq_No);

	sQuery.Format(_T("SELECT ACQ_YEAR FROM BO_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s"),data);
	m_pDM->GetOneValueQuery(sQuery,sAcq_Year);


	CString sCondition;
	CString sSpecies_key;

	// ��ó����...
	for(INT k = 0 ; k<m_nCodeCnt-1 ; k++)
	{
		sCondition = GetCondition(k);
		//������ ���� �������к� ������ ������ ���Ѵ�.
		//���������� �Ϲ��϶�..
		sQuery.Format(_T("SELECT SPECIES_KEY, ROUND(ORDER_PRICE,-1) ")
			_T("FROM BO_PURCHASEINFO_TBL ")
			_T("WHERE %s ORDER_SEQ_NO = '%s' ")
			_T("AND ORDER_YEAR = '%s' ")
			_T("AND SPECIES_KEY IN ")
			_T("(SELECT REC_KEY FROM BO_SPECIES_TBL WHERE USER_DEFINE_CODE1 = '01' AND MANAGE_CODE = UDF_MANAGE_CODE)")
//DEL 			_T("(SELECT REC_KEY FROM BO_SPECIES_TBL WHERE USER_DEFINE_CODE1 = '01')")
			,sCondition, sSeq_No, sAcq_Year);

		tempDM.RestructDataManager(sQuery);
		nSpeciesCnt = tempDM.GetRowCount();
		
		if(nSpeciesCnt > 0)
		{
			m_nSPECIES1 += nSpeciesCnt;
			for( INT i = 0; i<nSpeciesCnt; i++)
			{
				sPrice = tempDM.GetCellData(i,1);
				if(sPrice.IsEmpty())
				{
					sPrice = _T("0");
				}
				m_nPRICE1 += _ttoi(sPrice);
				sSpecies_key = tempDM.GetCellData(i,0);
				GetBookCnt(sSpecies_key,0);
			}
		}
		
		tempDM.FreeData();
		sPrice.Empty();
		
		//���������� �����϶�...
		sQuery.Format(_T("SELECT SPECIES_KEY, ROUND(ORDER_PRICE,-1) ")
			_T("FROM BO_PURCHASEINFO_TBL ")
			_T("WHERE %s ORDER_SEQ_NO = '%s' ")
			_T("AND ORDER_YEAR = '%s' ")
			_T("AND SPECIES_KEY IN ")
			_T("(SELECT REC_KEY FROM BO_SPECIES_TBL WHERE USER_DEFINE_CODE1 = '02' AND MANAGE_CODE = UDF_MANAGE_CODE)")
//DEL 			_T("(SELECT REC_KEY FROM BO_SPECIES_TBL WHERE USER_DEFINE_CODE1 = '02')")
			,sCondition, sSeq_No, sAcq_Year);

		tempDM.RestructDataManager(sQuery);
		nSpeciesCnt = tempDM.GetRowCount();
		
		if(nSpeciesCnt > 0)
		{
			m_nSPECIES2 += nSpeciesCnt;
			for( INT i = 0; i<nSpeciesCnt; i++)
			{
				sPrice = tempDM.GetCellData(i,1);
				if(sPrice.IsEmpty())
				{
					sPrice = _T("0");
				}
				m_nPRICE2 += _ttoi(sPrice);
				sSpecies_key = tempDM.GetCellData(i,0);
				GetBookCnt(sSpecies_key,1);
			}
		}

		SaveDM(k);
		tempDM.FreeData();
		sPrice.Empty();
		

	}
	return 0;
}

INT CBudgetDeptPrint::GetBookCnt(CString data, INT mode)
{
	if( data.IsEmpty() == TRUE)
	{
		return -1;
	}
	CString sQuery, sResult;
	
	sQuery.Format(_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s"),data);
	m_pDM->GetOneValueQuery(sQuery,sResult);
	
	if(sResult.IsEmpty() == TRUE)
	{
		sResult = _T("0");
	}
	
	//=============================================================
	// 2. �������к��� ���� å���� ���Ѵ�.
	//=============================================================
	if(mode == 0)
	{
		m_nBOOK1 += _ttoi(sResult);
	}
	else
	{
		m_nBOOK2 += _ttoi(sResult);
	}

	return 0;
}

CString CBudgetDeptPrint::GetCondition(INT number)
{
	CString cTemp;
	switch(number)
	{
	case 0:
		cTemp = _T("DEPT_CODE IS NULL AND");
		break;
	case 1: 
		cTemp = _T("DEPT_CODE = '01' AND");
		break;
	case 2: 
		cTemp = _T("DEPT_CODE = '02' AND");
		break;
	case 3: 
		cTemp = _T("DEPT_CODE = '03' AND");
		break;
	case 4: 
		cTemp = _T("DEPT_CODE = '04' AND");
		break;
	case 5: 
		cTemp = _T("DEPT_CODE = '05' AND");
		break;
	case 6: 
		cTemp = _T("DEPT_CODE = '06' AND");
		break;
	case 7: 
		cTemp = _T("DEPT_CODE = '07' AND");
		break;
	case 8: 
		cTemp = _T("DEPT_CODE = '08' AND");
		break;
	case 9: 
		cTemp = _T("DEPT_CODE = '09' AND");
		break;
	default:
		cTemp.Empty();
	}

	return cTemp;
}

BOOL CBudgetDeptPrint::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	if(InitESL_Mgr( _T("�������_�б⺰_��������") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : �������_�б⺰_��������") );
		return false;
	}
	
	
	m_pCM = FindCM(_T("CM_�������_�б⺰_��������"));
	m_pDM = FindDM(_T("DM_�������_�б⺰_��������"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));

	if(!m_pCM || !m_pDM || !m_pGrid) {
		ESLAfxMessageBox( _T("CM,DM,Grid Error : CM_�������_�б⺰_��������,DM_�������_�б⺰_��������,�׸���") );
		return FALSE;
	}
	
	if(InitControl()<0)
	{
		AfxMessageBox( _T(" ��Ʈ�� �ʱ�ȭ ����!! "));
		return FALSE;
	}
	
	ShowAllData();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CBudgetDeptPrint::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
