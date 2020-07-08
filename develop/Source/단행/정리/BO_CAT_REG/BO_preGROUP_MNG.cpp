// BO_preGROUP_MNG.cpp : implementation file
//

#include "stdafx.h"
#include "BO_preGROUP_MNG.h"
#include "BO_CAT_MAKE_preGROUP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_preGROUP_MNG dialog


CBO_preGROUP_MNG::CBO_preGROUP_MNG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_preGROUP_MNG)
	m_Year = _T("");
	//}}AFX_DATA_INIT
}
CBO_preGROUP_MNG::~CBO_preGROUP_MNG()
{
	


}

BOOL CBO_preGROUP_MNG::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}


VOID CBO_preGROUP_MNG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_preGROUP_MNG)
	DDX_Text(pDX, IDC_GROUP_YEAR, m_Year);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_preGROUP_MNG, CDialog)
	//{{AFX_MSG_MAP(CBO_preGROUP_MNG)
	ON_BN_CLICKED(IDC_GROUP_SEARCH, OnGroupSearch)
	ON_BN_CLICKED(IDC_NewGroup, OnNewGroup)
	ON_BN_CLICKED(IDC_DeleteGroup, OnDeleteGroup)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_preGROUP_MNG message handlers

BOOL CBO_preGROUP_MNG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_�����ΰ���")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
		
	DisplayData();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_preGROUP_MNG::DisplayData()
{
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_�����ΰ���"));

	// 040304_HSS ���� LocalTime���� Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	UpdateData(FALSE);

	OnGroupSearch();

	return ;

}

VOID CBO_preGROUP_MNG::OnGroupSearch() 
{
	// TODO: Add your control notification handler code here
	CString strQry,tmpQry;
	INT ids;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_�����ΰ���"));

	UpdateData(TRUE);

	m_Year.TrimLeft();
	m_Year.TrimRight();
	
	if( m_Year != _T("") )
	{
		tmpQry.Format(_T(" ACCESSION_REC_MAKE_YEAR = %s "), m_Year);
		strQry = strQry + tmpQry;
	}	

	ids = pDM->RefreshDataManager(strQry);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ���� �ּ���"));
		return;
	}
	

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_����_����_�����ΰ���"));

	pCM->AllControlDisplay();

}

VOID CBO_preGROUP_MNG::OnNewGroup() 
{
	// TODO: Add your control notification handler code here
	CBO_CAT_MAKE_preGROUP Dlg(this);
	Dlg.DoModal();

	DisplayData();
}

BEGIN_EVENTSINK_MAP(CBO_preGROUP_MNG, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_preGROUP_MNG)
	ON_EVENT(CBO_preGROUP_MNG, IDC_GROUPGRID, -600 /* Click */, OnClickGroupgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_preGROUP_MNG::OnClickGroupgrid() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid=( CESL_Grid * )FindControl(_T("CM_����_����_�����ΰ���"), _T("GRID"));;

	INT row = pGrid->GetRow();
	
	if (row == 0) return;
	if (row > 0) {
		pGrid->SetReverse(row-1);
		m_GROUP_NO = pGrid->GetAt(row-1,0);
		m_GROUP_REC_KEY = pGrid->GetAt(row-1,4);
	}	
}

VOID CBO_preGROUP_MNG::OnDeleteGroup() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	SelectMakeList(_T("CM_����_����_�����ΰ���"), _T("GRID"));
	INT idx = SelectGetTailPosition(_T("CM_����_����_�����ΰ���"), _T("GRID"));
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		return;
	}

	if (MessageBox(_T("������ ���θ� �����Ͻðڽ��ϱ�?"), _T("���� Ȯ��"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_����_����_�����ΰ���"));
		INT ids = pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
			return;
		}


		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		

			//pDM->MakeDeleteFrame(_T("CO_TEMP_ACCESSION_REC_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_����_����_�����ΰ���"), _T("GRID"));			
		}
		while (idx >= 0);

		sAccRecKey = sAccRecKeyTmp.Mid(1);
		
		CString sQuery;
		sQuery.Format( _T("DELETE FROM CO_TEMP_ACCESSION_REC_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = pDM->AddFrame(sQuery);
		
		ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
			return;
		}
		ids = pDM->EndFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("EndFrame �� Ȯ���ϼ���"));
			return;
		}

	}
	

	AllControlDisplay(_T("CM_����_����_�����ΰ���"));
EFS_END
	

}

HBRUSH CBO_preGROUP_MNG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
