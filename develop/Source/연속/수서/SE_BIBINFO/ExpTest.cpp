// ExpTest.cpp : implementation file
//

#include "stdafx.h"
#include "ExpTest.h"

#include "..\SE_API\SeExpectMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExpTest dialog


CExpTest::CExpTest(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CExpTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExpTest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pParentGrid = NULL;
	m_pParentDM = NULL;
	m_nACQ_TYPE = 1;
	m_strSpeciesKey = _T("");
	m_strPUB_FREQ = _T("");
}


VOID CExpTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpTest)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpTest, CDialog)
	//{{AFX_MSG_MAP(CExpTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpTest message handlers

BOOL CExpTest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("����_����_�׽�Ʈ") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_�׽�Ʈ") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_����_����_�׽�Ʈ"));
	m_pDM = FindDM(_T("DM_üũ��_����"));
	

	if(!m_pCM || !m_pDM) 	{
		AfxMessageBox(_T("CM,DM Error! : CM_����_����_�׽�Ʈ"));
		return FALSE;
	}		
	
	if(!m_pParentGrid) {
		AfxMessageBox(_T("�θ� �׸��带 ã�� �� �����ϴ�!"));
		return -1;
	}

	if(!m_pParentDM) {
		AfxMessageBox(_T("�θ� DM�� ã�� �� �����ϴ�!"));
		return -2;
	}
		
	INT nHead;
	m_pParentGrid->SelectMakeList();
	nHead = m_pParentGrid->SelectGetHeadPosition();
	if(nHead<0) {
		AfxMessageBox(_T("������ ������ �����ϴ�!"));
		return -3;
	}	
		
	m_strSpeciesKey = m_pParentDM->GetCellData(_T("ISS_��KEY"),nHead);
	m_strPUB_FREQ = m_pParentDM->GetCellData(_T("ISS_�����"),nHead);

	if(!m_strSpeciesKey.IsEmpty()) ShowExpectedData();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CExpTest::ShowExpectedData()
{


}
