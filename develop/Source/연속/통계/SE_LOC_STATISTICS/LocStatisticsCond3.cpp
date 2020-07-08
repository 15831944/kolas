// LocStatisticsCond3.cpp : implementation file
//

#include "stdafx.h"
#include "LocStatisticsCond3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsCond3 dialog


CLocStatisticsCond3::CLocStatisticsCond3(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocStatisticsCond3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pApi = NULL;
	m_pCM = NULL;

}

CLocStatisticsCond3::~CLocStatisticsCond3()
{
}

VOID CLocStatisticsCond3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocStatisticsCond3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocStatisticsCond3, CDialog)
	//{{AFX_MSG_MAP(CLocStatisticsCond3)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsCond3 message handlers
BOOL CLocStatisticsCond3::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CLocStatisticsCond3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("����_����_���_����3") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_���_����3") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_����_���_����3"));
	
	if(!m_pCM)
	{
		AfxMessageBox( _T("CM,DM error : ����_����_���_����3") );
		return FALSE;
	}


	if(!m_pApi)
	{
		AfxMessageBox( _T("��� API�� ã�� �� �����ϴ�!") );
		return FALSE;
	}
	
	// Date ��Ʈ�� ����
	/// ��¥ ��Ʈ�� UnCheck�ϱ�
	SYSTEMTIME systime;	
	CDateTimeCtrl* pTimeCtrl = NULL;
	
	const INT nCnt = 2;
	INT nIDC[nCnt] =
	{
		IDC_datCON3_SHELF_DATE1 , IDC_datCON3_SHELF_DATE2 
	};
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[i]);
		pTimeCtrl->GetTime(&systime);
		DateTime_SetSystemtime(pTimeCtrl->m_hWnd , GDT_NONE , &systime );
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	// 2009.08.31 ADD BY CJY : ���� �����ڿ� ��쿡�� ���������� ���ð���
	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_MANAGECODE3)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmbCON3_MANAGE_CODE)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLocStatisticsCond3::SaveCondition()
{
	
	//�ش� ��Ʈ�ѵ��� ���� �о�� �������� ������ api�� ���� �����Ѵ�
    CString strWhere;
    CString strtmp;
  
	strWhere = _T("I.REC_KEY = S.REC_KEY")
			   _T(" AND S.REC_KEY = B.SPECIES_KEY")
			   _T(" AND B.ACQ_TYPE_VOL_KEY = V.REC_KEY");

	// 2007.09.04 add by pdw
	// �η��ڷ� �������� ����	
	CButton* pBtn = (CButton*)GetDlgItem(IDC_chkAPPENDIX);
	if( pBtn )
	{
		if( pBtn->GetCheck() )
		{
			strWhere += _T(" AND B.BOOK_TYPE IN ( 'B' , 'C' , 'A' ) ");
		}
		else
		{
			strWhere += _T(" AND B.BOOK_TYPE IN ( 'B' , 'C' ) ");
		}
	}

	//���� ����
	CString strMANAGE_CODE;
	m_pCM->GetControlMgrData(_T("��������"),strMANAGE_CODE);
	
	strMANAGE_CODE.TrimLeft();
	strMANAGE_CODE.TrimRight();

    if(!strMANAGE_CODE.IsEmpty())
    {
		strtmp.Format(_T(" AND B.MANAGE_CODE = '%s'"),strMANAGE_CODE);
		strWhere += strtmp;
    }

	//�谡 �μ�
	//20070820 ADD PJW : �谡�μ��� ����
	CString strSHELF_LOC_CODE;
	m_pCM->GetControlMgrData(_T("�谡�μ�"),strSHELF_LOC_CODE);
	
	strSHELF_LOC_CODE.TrimLeft();
	strSHELF_LOC_CODE.TrimRight();

    if(!strSHELF_LOC_CODE.IsEmpty())
    {
		strtmp.Format(_T(" AND B.SHELF_LOC_CODE = '%s'"),strSHELF_LOC_CODE);
		strWhere += strtmp;
    }



	//�谡 ����
	CString strSHELF_DATE1,strSHELF_DATE2;

	CESL_Control_Element* pCE = NULL;
	pCE = (CESL_Control_Element*)m_pCM->FindControlElement(_T("�谡����1"));
	strSHELF_DATE1 = m_pApi->GetDate(this,pCE->CTRL_ID);
	if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() ) strSHELF_DATE1 = _T("");


	pCE = (CESL_Control_Element*)m_pCM->FindControlElement(_T("�谡����2"));
	strSHELF_DATE2 = m_pApi->GetDate(this,pCE->CTRL_ID);
	if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() ) strSHELF_DATE2 = _T("");

	strSHELF_DATE1.TrimLeft();
	strSHELF_DATE1.TrimRight();
	strSHELF_DATE2.TrimLeft();
	strSHELF_DATE2.TrimRight();

	if(strSHELF_DATE1.IsEmpty() && !strSHELF_DATE2.IsEmpty())
	{
		AfxMessageBox(_T("�谡���� ������ �޺κп��� ������ �ԷµǾ����ϴ�!"));
		return -1;
	}
	
	if(!strSHELF_DATE1.IsEmpty() || !strSHELF_DATE2.IsEmpty())
	{
		//�谡���� ����
		strtmp = m_pApi->MakeRangeQuery(_T("B.SHELF_DATE"),strSHELF_DATE1,strSHELF_DATE2);

		if(!strtmp.IsEmpty())
		{
			strWhere += _T(" AND ");
			strWhere += strtmp;
		}

	}
	
	// �ڷ� ����
	CString strStatus;
	m_pCM->GetControlMgrData(_T("�ڷ����"),strStatus);
	strtmp = _T("");
	
	m_pApi->m_arrayWorkingStatus.RemoveAll();

	if( !strStatus.IsEmpty() )
	{
		if( strStatus.Compare( _T("�ǽð������ڷ�(�ǽð��谡�ڷ�+�ǽð��������ڷ�)") ) == 0 )
		{
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 			strtmp.Format( _T(" AND (B.WORKING_STATUS = 'SEL112N'")
// 						   _T(" OR (B.PREV_WORKING_STATUS = 'SEL112N' AND B.WORKING_STATUS IN ('SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O')))"));
			strtmp.Format( _T(" AND (B.WORKING_STATUS = 'SEL112N'")
							// 18.09.27 JMJ KOL.RED.2018.008
						   _T(" OR (B.PREV_WORKING_STATUS = 'SEL112N' AND B.WORKING_STATUS IN ('SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O', 'SEL411O', 'SEL511O', 'SEL611O')))"));
			//=====================================================
		
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL112N"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL311O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL312O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL313O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL314O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL315O"));
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
			// 18.09.27 JMJ KOL.RED.2018.008
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL411O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL511O"));
			//=====================================================
	
		}
		else if( strStatus.Compare(_T("�ǽð��谡�ڷ�")) == 0 )
		{
			strtmp.Format( _T(" AND B.WORKING_STATUS = 'SEL112N'"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL112N"));
		}
		else if( strStatus.Compare(_T("�ǽð��������ڷ�")) == 0 )
		{
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 			strtmp.Format( _T(" AND B.PREV_WORKING_STATUS = 'SEL112N' AND B.WORKING_STATUS IN ('SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O')") );
			// 18.09.27 JMJ KOL.RED.2018.008
			strtmp.Format( _T(" AND B.PREV_WORKING_STATUS = 'SEL112N' AND B.WORKING_STATUS IN ('SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL315O', 'SEL411O', 'SEL511O', 'SEL611O')") );
			//=====================================================
			
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL311O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL312O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL313O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL314O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL315O"));
			//=====================================================
			//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
			// 18.09.27 JMJ KOL.RED.2018.008
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL411O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL511O"));
			//=====================================================
	
		}

	}	
	
	if(!strtmp.IsEmpty())
	{
		strWhere += strtmp;
	}
	


	m_pApi->m_strWhere = _T("");
	m_pApi->m_strWhere = strWhere;


	return 0;
}


HBRUSH CLocStatisticsCond3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
