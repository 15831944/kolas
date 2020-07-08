// Statistics.cpp: implementation of the CStatistics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Statistics.h"
#include "StatisticsConditionDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������
CStatistics::CStatistics(UINT IDD, CESL_Mgr * pParentMgr):CESL_Mgr(IDD, pParentMgr)
{
	m_nDialogID = IDD;
	m_pCondition = NULL;
	m_pConditionDlg = NULL;
	m_pPrintMgr = NULL;
}
// �Ҹ���
CStatistics::~CStatistics()
{
	if(m_pCondition)
	{
		delete m_pCondition;
		m_pCondition = NULL;
	}
	if(m_pConditionDlg)
	{
		delete m_pConditionDlg;
		m_pConditionDlg = NULL;
	}
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
}

BOOL CStatistics::Create(CWnd * pWnd)
{
	return CDialog::Create(m_nDialogID, pWnd);
}

VOID CStatistics::ShowConditionDlg()
{
	if(!m_pConditionDlg->GetCreate()) m_pConditionDlg->Create(this);
	//===================================================
	//2009.04.08 ADD BY PJW : �����ڷ����������� ���� �����Ѵ�.
	CString strRemoveBookBringYN;
	CString strRemoveBookBringYNQuery;

	strRemoveBookBringYNQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='��Ÿ' AND GROUP_3='����' AND CLASS_ALIAS='�ڻ���� �����ڷ�����' AND MANAGE_CODE = UDF_MANAGE_CODE"));
	m_pDM->GetOneValueQuery( strRemoveBookBringYNQuery, strRemoveBookBringYN );
	strRemoveBookBringYN.TrimLeft();	strRemoveBookBringYN.TrimRight();
	m_pCondition->SetRemoveBookBringYN( strRemoveBookBringYN );
	//===================================================
	m_pConditionDlg->ShowWindow(SW_SHOW);
	UpdateWindow();
	CenterWindow(this);
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ��蹰 ��¹�ư
// [ �̴Ͻ��� ]
//////////////////////////////////////////////////////////////////////
VOID CStatistics::PrintStatistics()
{
	if( m_pDM->GetRowCount() == 0 )
	{
		ESLAfxMessageBox(_T("����� ��������� �����ϴ�."));
		return;
	}
	CString strClassNoType, strSIRfile;

	strClassNoType = m_pCondition->GetClassNoType();
	
	m_pPrintMgr->InitPrintData();
	m_pPrintMgr->AddPrintData(_T("����"), m_pCondition->GetStatType());
	m_pPrintMgr->AddPrintData(_T("�ڷᱸ��"), m_pCondition->GetVerticalDesc());
	m_pPrintMgr->AddPrintData(_T("����"), m_pCondition->GetCondition());
	m_pPrintMgr->AddPrintData(_T("�з�ǥ����"), strClassNoType);
	m_pPrintMgr->SetPrintDM(m_pDM);	
		
	m_pPrintMgr->SetSIReportFilename(_T("K2UP_�ڻ�����ڷ�"));
	m_pPrintMgr->Print();
}

VOID CStatistics::DisplayGrid()
{
	m_pGrid->Display();
}

INT CStatistics::SetStatisticsInfo()
{
	/*
	if(m_pCondition->GetAcqType() == PURCHASE) m_pCM->SetControlMgrData(_T("���Գ⵵"), _T("����"));
	else if(m_pCondition->GetAcqType() == DONATE) m_pCM->SetControlMgrData(_T("���Գ⵵"), _T("����"));
	*/
	
	
	//m_pCM->SetControlMgrData(_T("��蹰"), m_pCondition->GetStatType());
	m_pCM->SetControlMgrData(_T("����"), m_pCondition->GetCondition());
	
	return 0;
}

VOID CStatistics::CreateConditionDlg()
{
	if( m_pConditionDlg )
	{
		delete m_pConditionDlg;
		m_pConditionDlg = NULL;
	}

	if( m_pPrintMgr )
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}

	m_pConditionDlg = new CStatisticsConditionDlg(this);
	m_pPrintMgr = new CBasicPrintMgr(this);
	CreateCondition();
	m_pConditionDlg->SetCondition(m_pCondition);
	m_pConditionDlg->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
}

//////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �׸��� ����� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. �׸��� ����� ���ڸ� �⺻������ �����Ѵ�.
// 2. �ӽ�DM�� ���������� �����Ѵ�.
// 3. ���Ǽ��� Ŭ������ ����Ǿ� �ִ� _T("�ڷᱸ��") ���� ���� �����´�. (KDC, DDC, LC)
// 4. ����� �ڷᱸ�а��� ���� �ڵ尪���� ������ ������ DM�� �����ϰ� �׸��� ����� �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////
VOID CStatistics::ChangeColumnHeaderOfSubjectCode()
{
	//================================================================
	// ��������
	//================================================================
	INT i, idx, nCodeCnt;	
	CString codeClassName, strQuery, strResult;
	CESL_DataMgr tmpDM;

	//================================================================
	// 1. �׸��� ����� ���ڸ� �⺻������ �����Ѵ�.
	//================================================================
	for(i=0; i<m_pGrid->GetColCount()-4; i++)
	{
		strResult.Format(_T("%d"), i);
		m_pGrid->SetTextMatrix(0, i+3, strResult);
	}

	//================================================================
	// 2. �ӽ�DM�� ���������� �����Ѵ�.
	//================================================================
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	//================================================================
	// 3. ���Ǽ��� Ŭ������ ����Ǿ� �ִ� _T("�ڷᱸ��") ���� ���� �����´�. (KDC, DDC, LC)
	//================================================================
	codeClassName = m_pCondition->GetClassNoType();


	if( codeClassName.Left(2) != _T("LC") && m_pGrid->GetColCount() > 14 )
	{
		for( i=12; i<24; i++ )
		{
			m_pGrid->SetColWidth(i,0,0);
		}
	}
	

	//================================================================
	// 4. ����� �ڷᱸ�а��� ���� �ڵ尪���� ������ ������ DM�� �����ϰ� �׸��� ����� �����Ѵ�.
	//================================================================
	strQuery.Format(_T("SELECT CLASS_CODE FROM CD_CLASS_CODE_TBL WHERE CLASS_NAME = '%s'"), codeClassName);
	tmpDM.GetOneValueQuery(strQuery, strResult);
	
	if( strResult.IsEmpty() == FALSE )
	{
		strQuery.Format(_T("SELECT DESCRIPTION, CODE FROM CD_CODE_TBL WHERE CLASS = '%s' ORDER BY CODE"), strResult);
		tmpDM.RestructDataManager(strQuery);

		idx = 0;
		nCodeCnt = tmpDM.GetRowCount();	

		m_pGrid->SetCols(0,nCodeCnt+4);
		m_pGrid->Display();
		for( i=0; i<nCodeCnt; i++ )
		{
			strResult.Empty();
			strResult = tmpDM.GetCellData(i,0);
			if( strResult.IsEmpty() == TRUE || strResult == _T("�������") )	continue;
			m_pGrid->SetTextMatrix(0, idx+3, strResult );
			idx++;
		}
		m_pGrid->SetTextMatrix(0, idx+3, _T("��") );
		m_pGrid->SetTextMatrix(0, idx+4, _T("�����հ�") );
	}
}

VOID CStatistics::CreateCondition()
{
	if(m_pCondition)
	{
		delete m_pCondition;
		m_pCondition = NULL;
	}
	m_pCondition = new CStatisticsCondition(this);
}

VOID CStatistics::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}