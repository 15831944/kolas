// SE_CAT_GIVE_REGNO.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\..\����\����\BO_CAT_REGNO_GIVE\BO_CAT_REGNO_GIVE1.h"
#include "SE_CAT_GIVE_REGNO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_GIVE_REGNO dialog


CSE_CAT_GIVE_REGNO::CSE_CAT_GIVE_REGNO(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_GIVE_REGNO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CSE_CAT_GIVE_REGNO::~CSE_CAT_GIVE_REGNO()
{
}

BOOL CSE_CAT_GIVE_REGNO::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}

VOID CSE_CAT_GIVE_REGNO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_GIVE_REGNO)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_GIVE_REGNO, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_GIVE_REGNO)
	ON_BN_CLICKED(IDC_GIVEREGNO, OnGiveregno)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_GIVE_REGNO message handlers

BOOL CSE_CAT_GIVE_REGNO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_�ϰ��ο�")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_������_����"));

	CString m_Year;
	// 040304_HSS ���� LocalTime���� Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_����_����_�ϰ��ο�"), _T("�Լ��⵵"), m_Year);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CAT_GIVE_REGNO::OnGiveregno() 
{
	// TODO: Add your control notification handler code here
	INT idx;
	idx = SearchData();

	if ( idx ) return;
	
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_������_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG error") );
		return ;
	}

	idx = pDM->GetRowCount();
	if (idx == 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		return ;			
	}


	CString sTmpBookKey;

	for (INT i = 0 ; i < idx ; i++)
	{
		CString sRecKeyTmp = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , i);
		if ( sRecKeyTmp != _T("") ) {
			sTmpBookKey = sTmpBookKey + _T(", ") + sRecKeyTmp;
		}

	}

	if ( sTmpBookKey == _T("") ) {
		BO_CAT_REGNO_GIVE dlg(this, _T(""));
		dlg.m_strParentCMAlias= _T("CM_����_����_������_����");
		dlg.m_strParentDMAlias = _T("DM_����_����_������_����");
		dlg.m_nOpenMode = 3;
		dlg.DoModal();
		INT iCount = dlg.m_aBookKey.GetSize();
	}
	else {
		AfxMessageBox( _T("��Ϲ�ȣ �ο��� �ڷᰡ �ֽ��ϴ�.") );
		return ;
	}
	
	DeletePreGroup();

	return ;
	
}
INT CSE_CAT_GIVE_REGNO::SearchData()
{
	CString strQry,strpreGroup1,strpreGroup2,strYear;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_������_����"));

	// ���� ��ȣ �� ������ ��ȣ�� ���ο� ���� �ش�.
	GetControlData(_T("CM_����_����_�ϰ��ο�"), _T("�����ι�ȣ����"), strpreGroup1);
	GetControlData(_T("CM_����_����_�ϰ��ο�"), _T("�����ι�ȣ��"), strpreGroup2);		 

	if( (strpreGroup1 != _T(""))&&(strpreGroup2 != _T("")) )
	{
		strQry = _T(" A.ACCESSION_REC_NO >=")+strpreGroup1+_T(" AND A.ACCESSION_REC_NO <= ") + strpreGroup2;
	}

	if( (strpreGroup1 != _T(""))&&(strpreGroup2 == _T("")) )
	{
		strQry = _T(" A.ACCESSION_REC_NO =")+strpreGroup1;
	}

	if( (strpreGroup1 == _T(""))&&(strpreGroup2 != _T("")) )
	{
		strpreGroup1 = strpreGroup2;
		strQry = _T(" A.ACCESSION_REC_NO =")+strpreGroup1;
	}

	if( strQry == _T(""))
	{
		AfxMessageBox(_T("������ ������ ������ �ֽʽÿ� !"));
		return -1;
	}

	strpreGroup1 = strpreGroup1 + _T(" - ") +strpreGroup2;
	SetControlData(_T("CM_����_����_������_����"), _T("�����ι�ȣ"), strpreGroup1);

	GetControlData(_T("CM_����_����_�ϰ��ο�"), _T("�Լ��⵵"), strYear);
	if( strYear != _T("") )
	{
		strQry = strQry + _T(" AND A.ACCESSION_REC_MAKE_YEAR ='")+strYear+_T("'");
	}


	strQry = strQry+_T(" AND B.WORKING_STATUS = 'SER113O' ");


	pDM->RefreshDataManager(strQry);


	if ( pDM->GetRowCount() == 0 )
	{		
		AfxMessageBox(_T("�ڷᰡ �����ϴ�. !"));
		return -2;
	}

	CESL_ControlMgr *pCM;
	CESL_Grid *pGrid_MainGrid_All;

	pCM = FindCM(_T("CM_����_����_������_����"));
	pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;


	//�׸��� �ݹ� �Լ� ���� 

	pCM->AllControlDisplay();
	
	INT max = pGrid_MainGrid_All->GetRows() - 1;
	
	for ( INT i = 0 ; i < max ; i++ )
	{
		pGrid_MainGrid_All->SetAt(i, 0, _T("V"));
	}

	return 0;
}

INT CSE_CAT_GIVE_REGNO::DeletePreGroup()
{
	return 0;
}
INT CSE_CAT_GIVE_REGNO::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_�ڷ����_�ڵ�")) {
            if ( str == _T("SER111N")) {
				str = _T("����ΰ��ڷ�");
			}
			else if ( str == _T("SER112N") ) {
				str = _T("����ڷ�");
			}
			else if ( str == _T("SER113O") ) {
				str = _T("�������ڷ�");
			}
        }
    }
	return 0;


EFS_END
return -1;

}

HBRUSH CSE_CAT_GIVE_REGNO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
