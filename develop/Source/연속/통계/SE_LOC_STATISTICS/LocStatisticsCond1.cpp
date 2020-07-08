// LocStatisticsCond1.cpp : implementation file
//

#include "stdafx.h"
#include "LocStatisticsCond1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsCond1 dialog


CLocStatisticsCond1::CLocStatisticsCond1(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocStatisticsCond1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pCM = NULL;
	m_pApi = NULL;
}

CLocStatisticsCond1::~CLocStatisticsCond1()
{
}

VOID CLocStatisticsCond1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocStatisticsCond1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocStatisticsCond1, CDialog)
	//{{AFX_MSG_MAP(CLocStatisticsCond1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsCond1 message handlers
BOOL CLocStatisticsCond1::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CLocStatisticsCond1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		
	if(InitESL_Mgr( _T("����_����_���_����1") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_���_����1") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_����_���_����1"));
	
	if(!m_pCM)
	{
		AfxMessageBox( _T("CM,DM error : ����_����_���_����1") );
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
		IDC_datREG_DATE3 , IDC_datREG_DATE2 
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
		GetDlgItem(IDC_MANAGECODE1)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmbMANAGE_CODE)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLocStatisticsCond1::SaveCondition()
{
   //�ش� ��Ʈ�ѵ��� ���� �о�� �������� ������ api�� ���� �����Ѵ�

    CString strWhere;
    CString strtmp;
    BOOL bIsWonbu = FALSE;
	
	

	//���ι�ȣ ���� �ľ�
	CString strWonbuNum1,strWonbuNum2;
	m_pCM->GetControlMgrData(_T("���ι�ȣ1"),strWonbuNum1);
	m_pCM->GetControlMgrData(_T("���ι�ȣ2"),strWonbuNum2);

	if(strWonbuNum1.IsEmpty() && !strWonbuNum2.IsEmpty())
	{
		AfxMessageBox(_T("���ι�ȣ ������ �޺κп��� ������ �ԷµǾ����ϴ�!"));
		return -1;
	}


	if(!strWonbuNum1.IsEmpty() || !strWonbuNum2.IsEmpty())
	{
		bIsWonbu = TRUE;
	}


	if(bIsWonbu) //���ι�ȣ ���� ���� ��� 
	{
		strWhere = _T("I.REC_KEY = S.REC_KEY")
				   _T(" AND S.REC_KEY = B.SPECIES_KEY")
				   _T(" AND B.ACQ_TYPE_VOL_KEY = V.REC_KEY")
			       _T(" AND W.REC_KEY = B.ACCESSION_REC_KEY")
				   _T(" AND W.PUBLISH_FORM_CODE = 'SE'");

		//���� ��ȣ ����
		strtmp = m_pApi->MakeIntRangeQuery(_T("W.ACCESSION_REC_NO"),strWonbuNum1,strWonbuNum2);

		if(!strtmp.IsEmpty())
		{
			strWhere += _T(" AND ");
			strWhere += strtmp;
		}
		
	}
	else //���ι�ȣ ������ ���� ���
	{
		strWhere = _T("I.REC_KEY = S.REC_KEY")
			       _T(" AND S.REC_KEY = B.SPECIES_KEY")
				   _T(" AND B.ACQ_TYPE_VOL_KEY = V.REC_KEY");
	}
	
	

    //���� ���� 
    CString strACQ_CODE;
    m_pCM->GetControlMgrData(_T("���Ա���"),strACQ_CODE);

    strACQ_CODE.TrimLeft();
	strACQ_CODE.TrimRight();
	
    if(!strACQ_CODE.IsEmpty())
    {
		strtmp.Format(_T(" AND B.ACQ_CODE = '%s'"),strACQ_CODE);
		strWhere += strtmp;
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
	
	//���� �⵵
	CString strACQ_YEAR;
	m_pCM->GetControlMgrData(_T("���Գ⵵"),strACQ_YEAR);
	
	strACQ_YEAR.TrimLeft();
	strACQ_YEAR.TrimRight();

    if(!strACQ_YEAR.IsEmpty())
    {
		strtmp.Format(_T(" AND B.ACQ_YEAR = '%s'"),strACQ_YEAR);
		strWhere += strtmp;
    }



	//��� ��ȣ 
	CString strRegCode,strRegNo1,strRegNo2;

	m_pCM->GetControlMgrData(_T("��Ϲ�ȣ����"),strRegCode);
	m_pCM->GetControlMgrData(_T("��Ϲ�ȣ1"),strRegNo1);
	m_pCM->GetControlMgrData(_T("��Ϲ�ȣ2"),strRegNo2);

	strRegCode.TrimLeft();
	strRegCode.TrimRight();
	strRegNo1.TrimLeft();
	strRegNo1.TrimRight();
	strRegNo2.TrimLeft();
	strRegNo2.TrimRight();
	
	if(strRegNo1.IsEmpty() && !strRegNo2.IsEmpty())
	{
		AfxMessageBox(_T("��Ϲ�ȣ ������ �޺κп��� ������ �ԷµǾ����ϴ�!"));
		return -1;
	}

	if(!strRegNo1.IsEmpty() && strRegCode.IsEmpty())
	{
		AfxMessageBox(_T("��ϱ����� �Էµ��� �ʾҽ��ϴ�!"));
		return -1;
	}
	
	if(!strRegNo1.IsEmpty() || !strRegNo2.IsEmpty())
	{
		//��� ��ȣ ����
		strRegNo1 = strRegCode + strRegNo1;
		strRegNo2 = strRegCode + strRegNo2;
		strtmp = m_pApi->MakeRangeQuery(_T("B.REG_NO"),strRegNo1,strRegNo2);

		if(!strtmp.IsEmpty())
		{
			strWhere += _T(" AND ");
			strWhere += strtmp;
		}

	}

	//��� ����
	CString strREG_DATE1,strREG_DATE2;

	CESL_Control_Element* pCE = NULL;
	pCE = (CESL_Control_Element*)m_pCM->FindControlElement(_T("�������1"));
	strREG_DATE1 = m_pApi->GetDate(this,pCE->CTRL_ID);
	if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() ) strREG_DATE1 = _T("");


	pCE = (CESL_Control_Element*)m_pCM->FindControlElement(_T("�������2"));
	strREG_DATE2 = m_pApi->GetDate(this,pCE->CTRL_ID);
	if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() ) strREG_DATE2 = _T("");

	strREG_DATE1.TrimLeft();
	strREG_DATE1.TrimRight();
	strREG_DATE2.TrimLeft();
	strREG_DATE2.TrimRight();

	if(strREG_DATE1.IsEmpty() && !strREG_DATE2.IsEmpty())
	{
		AfxMessageBox(_T("��� ���� ������ �޺κп��� ������ �ԷµǾ����ϴ�!"));
		return -1;
	}
	
	if(!strREG_DATE1.IsEmpty() || !strREG_DATE2.IsEmpty())
	{
		//��� ���� ����
		strtmp = m_pApi->MakeRangeQuery(_T("B.REG_DATE"),strREG_DATE1,strREG_DATE2);

		if(!strtmp.IsEmpty())
		{
			strWhere += _T(" AND ");
			strWhere += strtmp;
		}

	}
	
	//�۾� ��(SEC111N,SEC112O,SEL211O) ���� 
	strWhere += _T(" AND B.WORKING_STATUS IN ('SEC111N','SEC112O','SEL211O')");


	m_pApi->m_strWhere = _T("");
	m_pApi->m_bWonbuCondition = bIsWonbu;
	m_pApi->m_strWhere = strWhere;

	return 0;
}
HBRUSH CLocStatisticsCond1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
