
#include "stdafx.h"
#include "KL_STATISTICS_SEARCH1.h"
#include "KLStatisticsDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////
// CKL_STATISTICS_SEARCH1 dialog


CKL_STATISTICS_SEARCH1::CKL_STATISTICS_SEARCH1(CESL_Mgr* pParent /*=NULL*/, INT nType)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_STATISTICS_SEARCH1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pParentCM = NULL;
	m_pParentGrid = NULL;
	m_pParentDM = NULL;
	m_nType = nType;
}

CKL_STATISTICS_SEARCH1::~CKL_STATISTICS_SEARCH1()
{

}

void CKL_STATISTICS_SEARCH1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_STATISTICS_SEARCH1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_STATISTICS_SEARCH1, CDialog)
	//{{AFX_MSG_MAP(CKL_STATISTICS_SEARCH1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CKL_STATISTICS_SEARCH1 message handlers
BOOL CKL_STATISTICS_SEARCH1::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}


CString CKL_STATISTICS_SEARCH1::GetCenterServiceConnInfo()
{	
	return _T("");
}

BOOL CKL_STATISTICS_SEARCH1::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("SM_KL_STATISTICS_SEARCH") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : KL_STATISTICS_SEARCH") );
		return FALSE;
	}
	
	m_pParentCM = FindCM(_T("CM_KL_STATISTICS"));
	m_pCM = FindCM(_T("CM_KL_STATISTICS_SEARCH"));
	m_pParentDM = FindDM(_T("DM_KL_STATISTICS"));
	if(NULL == m_pCM || NULL == m_pParentCM || NULL == m_pParentDM)
	{
		return FALSE;
	}
	
	m_pParentGrid = (CESL_Grid*)(m_pParentCM->FindControl(_T("���׸���")));
	if(m_pParentGrid == NULL)
	{
		return FALSE;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	InitScreen();

	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CKL_STATISTICS_SEARCH1::InitScreen()
{
	switch (m_nType)
	{
	case KL_STAT_TYPE_REGUSER_GENDER:
		{
			SetDlgItemText(IDC_STATIC3, _T("�������"));
			SetWindowText(_T("å����ȸ��������(����)"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("�ڰ��� ȸ�������� �������� å�������񽺿� �����\r\n")
											_T("ȸ���� ���� �ǽð� ���� ��踦 �����մϴ�.\r\n\r\n\r\n")
											_T("�ص������� Ż���� ȸ���� �����ϴ� ��쿡 ��� ��ġ��\r\n")
											_T("   �Ϻ� ����Ȯ�� �� �ֽ��ϴ�.\r\n")
											_T("�ؽǽð����� ������ ������ ���� ����� �޶��� ��\r\n")
											_T("   �ֽ��ϴ�.\r\n")
											);
		}
		break;
	case KL_STAT_TYPE_REGUSER_AGE:
		{
			SetDlgItemText(IDC_STATIC3, _T("�������"));
			SetWindowText(_T("å����ȸ��������(���ɺ�)"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("�ڰ��� ȸ�������� �������� å�������񽺿� �����\r\n")
											_T("ȸ���� ���� �ǽð� ���ɺ� ��踦 �����մϴ�.\r\n\r\n\r\n")
											_T("�ص������� Ż���� ȸ���� �����ϴ� ��쿡 ��� ��ġ��\r\n")
											_T("   �Ϻ� ����Ȯ�� �� �ֽ��ϴ�.\r\n")
											_T("�ؽǽð����� ������ ������ ���� ����� �޶��� ��\r\n")
											_T("   �ֽ��ϴ�.\r\n")
											);
		}
		break;
	case KL_STAT_TYPE_SECUSER_GENDER:
		{
			SetDlgItemText(IDC_STATIC3, _T("Ż������"));
			SetWindowText(_T("å����ȸ��Ż�����(����)"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("�ڰ��� ȸ�������� �������� å�������񽺸� Ż����\r\n")
										    _T("ȸ���� ���� �ǽð� ���� ��踦 �����մϴ�.\r\n\r\n\r\n")
											_T("�ص������� Ż���� ȸ���� �����ϴ� ��쿡 ��� ��ġ��\r\n")
											_T("   �Ϻ� ����Ȯ�� �� �ֽ��ϴ�.\r\n")
											_T("�ؽǽð����� ������ ������ ���� ����� �޶��� ��\r\n")
											_T("   �ֽ��ϴ�.\r\n")
											);
		}
		break;
	case KL_STAT_TYPE_SECUSER_AGE:
		{
			SetDlgItemText(IDC_STATIC3, _T("Ż������"));
			SetWindowText(_T("å����ȸ��Ż�����(���ɺ�)"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("�ڰ��� ȸ�������� �������� å�������񽺸� Ż����\r\n")
										    _T("ȸ���� ���� �ǽð� ���ɺ� ��踦 �����մϴ�.\r\n\r\n\r\n")
											_T("�ص������� Ż���� ȸ���� �����ϴ� ��쿡 ��� ��ġ��\r\n")
											_T("   �Ϻ� ����Ȯ�� �� �ֽ��ϴ�.\r\n")
											_T("�ؽǽð����� ������ ������ ���� ����� �޶��� ��\r\n")
											_T("   �ֽ��ϴ�.\r\n")
											);

		}
		break;
	case KL_STAT_TYPE_FIRSTREG_GENDER:
		{
			SetDlgItemText(IDC_STATIC3, _T("�������"));
			SetWindowText(_T("ù����ȸ�����(����)"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("�ڰ��� ���ؼ� å�������񽺸� ���� ������ ȸ����\r\n")
										    _T("���� �ǽð� ���� ��踦 �����մϴ�.\r\n\r\n\r\n")
											_T("�ص������� Ż���� ȸ���� �����ϴ� ��쿡 ��� ��ġ��\r\n")
											_T("   �Ϻ� ����Ȯ�� �� �ֽ��ϴ�.\r\n")
											_T("�ؽǽð����� ������ ������ ���� ����� �޶��� ��\r\n")
											_T("   �ֽ��ϴ�.\r\n")
											);
		}
		break;
	case KL_STAT_TYPE_FIRSTREG_AGE:
		{
			SetDlgItemText(IDC_STATIC3, _T("�������"));
			SetWindowText(_T("ù����ȸ�����(���ɺ�)"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("�ڰ��� ���ؼ� å�������񽺸� ���� ������ ȸ����\r\n")
										    _T("���� �ǽð� ���ɺ� ��踦 �����մϴ�.\r\n\r\n\r\n")
											_T("�ص������� Ż���� ȸ���� �����ϴ� ��쿡 ��� ��ġ��\r\n")
											_T("   �Ϻ� ����Ȯ�� �� �ֽ��ϴ�.\r\n")
											_T("�ؽǽð����� ������ ������ ���� ����� �޶��� ��\r\n")
											_T("   �ֽ��ϴ�.\r\n")
											);
		}
		break;
	case KL_STAT_TYPE_OTHERIMP_GENDER:
		{
			SetDlgItemText(IDC_STATIC3, _T("�������"));
			SetWindowText(_T("Ÿ������ȸ�����(����)"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("Ÿ���� ���ؼ� å�������񽺸� ���� ������ �̿��ڰ�\r\n")
										    _T("�ڰ��� ���ԵǾ� ���Ե� ȸ���� ���� �ǽð� ����\r\n")
											_T("��踦 �����մϴ�.\r\n\r\n")
											_T("�ص������� Ż���� ȸ���� �����ϴ� ��쿡 ��� ��ġ��\r\n")
											_T("   �Ϻ� ����Ȯ�� �� �ֽ��ϴ�.\r\n")
											_T("�ؽǽð����� ������ ������ ���� ����� �޶��� ��\r\n")
											_T("   �ֽ��ϴ�.\r\n")
											);
		}
		break;
	case KL_STAT_TYPE_OTHERIMP_AGE:
		{
			SetDlgItemText(IDC_STATIC3, _T("�������"));
			SetWindowText(_T("Ÿ������ȸ�����(���ɺ�)"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("Ÿ���� ���ؼ� å�������񽺸� ���� ������ �̿��ڰ�\r\n")
										    _T("�ڰ��� ���ԵǾ� ���Ե� ȸ���� ���� �ǽð� ���ɺ�\r\n")
											_T("��踦 �����մϴ�.\r\n\r\n")
											_T("�ص������� Ż���� ȸ���� �����ϴ� ��쿡 ��� ��ġ��\r\n")
											_T("   �Ϻ� ����Ȯ�� �� �ֽ��ϴ�.\r\n")
											_T("�ؽǽð����� ������ ������ ���� ����� �޶��� ��\r\n")
											_T("   �ֽ��ϴ�.\r\n")
											);
		}
		break;
	case KL_STAT_TYPE_OTHERRETURN_OTHER:
		{
			SetDlgItemText(IDC_STATIC3, _T("�ݳ�����"));			
			SetWindowText(_T("å���� Ÿ�������� �ݳ�ó�����"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("å�������񽺸� ���� Ÿ������ �������� ������ \r\n")
										    _T("�ڰ����� �ݳ��� ��쿡���� �ݳ�ó����踦 �����մϴ�.\r\n\r\n"));
		}
		break;
	case KL_STAT_TYPE_OTHERRETURN_SELF:
		{
			SetDlgItemText(IDC_STATIC3, _T("�ݳ�����"));
			SetWindowText(_T("å���� �ڰ������� Ÿ���ݳ����"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("å�������񽺸� ���� Ÿ������ �������� ������ \r\n")
										    _T("Ÿ������ �ݳ��� ��쿡���� �ݳ�ó����踦 �����մϴ�.\r\n\r\n"));
		}
		break;
	case KL_STAT_TYPE_OTHERLOAN_SELF:
		{
			SetDlgItemText(IDC_STATIC3, _T("��������"));
			SetWindowText(_T("å���� �ڰ������� Ÿ���������"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("å�������񽺸� ���� �ڰ����� �������� ������ \r\n")
										    _T("Ÿ������ ������ ��쿡���� ����ó����踦 �����մϴ�.\r\n\r\n"));
		}
		break;
	case KL_STAT_TYPE_OTHERLOAN_OTHER:
		{
			SetDlgItemText(IDC_STATIC3, _T("��������"));
			SetWindowText(_T("å���� Ÿ�������� ����ó�����"));
			SetDlgItemText(IDC_STA_EXPLAIN, _T("\r\n")
											_T("å�������񽺸� ���� Ÿ������ �������� ������ \r\n")
										    _T("�ڰ����� ������ ��쿡 ���� ����ó����踦 �����մϴ�.\r\n\r\n"));
		}
		break;
	}

	const INT nUserStatCtrlCount = 2;
	INT nUserStatCtrlID[nUserStatCtrlCount] = {
		IDC_RADIO1, IDC_RADIO2
	};

	const INT nLoanStatCtrlCount = 4;
	INT nLoanStatCtrlID[nLoanStatCtrlCount] = {
		IDC_RADIO3, IDC_RADIO4, IDC_RADIO5, IDC_RADIO6
	};

	BOOL bUserStat = (m_nType < KL_STAT_TYPE_OTHERRETURN_OTHER);

	for (INT i=0; i<nUserStatCtrlCount; i++)
	{
		CWnd* pWnd = GetDlgItem(nUserStatCtrlID[i]);
		ASSERT(pWnd);
		pWnd->ShowWindow(bUserStat ? SW_SHOW : SW_HIDE);
	}

	for (i=0; i<nLoanStatCtrlCount; i++)
	{
		CWnd* pWnd = GetDlgItem(nLoanStatCtrlID[i]);
		ASSERT(pWnd);
		pWnd->ShowWindow(!bUserStat ? SW_SHOW : SW_HIDE);
	}

	return 0;
}

INT CKL_STATISTICS_SEARCH1::ExecStatistics()
{
	if (m_nType < KL_STAT_TYPE_OTHERRETURN_OTHER)
		return ExecDBStatistics();
	else 
		return ExecMWStatistics();

	return -1;
}

INT CKL_STATISTICS_SEARCH1::ExecDBStatistics()
{
	CString strQuery;	
	if (GetStatisticsQuery(strQuery) < 0)
		return -1;

	if (m_pParentDM->RestructDataManager(strQuery) < 0)
	{
		AfxMessageBox(_T("��� ���⿡ �����Ͽ����ϴ�."));
		return -1;
	}
	INT nRowCount = m_pParentDM->GetRowCount();

	INT nColCount = -1;
	switch (m_nType)
	{
	case KL_STAT_TYPE_REGUSER_GENDER:
	case KL_STAT_TYPE_SECUSER_GENDER:
	case KL_STAT_TYPE_FIRSTREG_GENDER:
	case KL_STAT_TYPE_OTHERIMP_GENDER:
		nColCount = 1;
		break;
	case KL_STAT_TYPE_REGUSER_AGE:
	case KL_STAT_TYPE_SECUSER_AGE:
	case KL_STAT_TYPE_FIRSTREG_AGE:
	case KL_STAT_TYPE_OTHERIMP_AGE:
		nColCount = 1;
		break;
	}

	InitGrid(nRowCount+2, nColCount+2);

	CString strColText;
	switch (m_nType)
	{
	case KL_STAT_TYPE_REGUSER_GENDER:
	case KL_STAT_TYPE_SECUSER_GENDER:
	case KL_STAT_TYPE_FIRSTREG_GENDER:
	case KL_STAT_TYPE_OTHERIMP_GENDER:
		strColText = _T("����");
		break;
	case KL_STAT_TYPE_REGUSER_AGE:
	case KL_STAT_TYPE_SECUSER_AGE:
	case KL_STAT_TYPE_FIRSTREG_AGE:
	case KL_STAT_TYPE_OTHERIMP_AGE:
		strColText = _T("���ɴ�");
		break;
	}

	m_pParentGrid->SetTextMatrix(0, 1, strColText);
	m_pParentGrid->SetTextMatrix(0, 2, _T("ȸ����"));

	for (INT i=0; i<nRowCount; i++)
	{
		m_pParentGrid->SetTextMatrix(i+1, 1, m_pParentDM->GetCellData(i, 0));
	}

	m_pParentGrid->SetTextMatrix(nRowCount+1, 1, _T("�հ�"));

	INT nTotal = 0;
	for (i=0; i<nRowCount; i++)
	{
		CString strData = m_pParentDM->GetCellData(i, 1);
		m_pParentGrid->SetTextMatrix(i+1, 2, strData);

		nTotal += _ttoi(strData);
	}

	CString strTotal;
	strTotal.Format(_T("%d"), nTotal);
	m_pParentGrid->SetTextMatrix(nRowCount+1, 2, strTotal);

	return 0;
}

INT CKL_STATISTICS_SEARCH1::ExecMWStatistics()
{
	INT ids = LILL_STATISTICS(m_pParentDM, m_nType);
	if		(-100 == ids)	return ids;
	else if	(-100 != ids && 0 > ids)
	{
		AfxMessageBox(_T("��� ���⿡ �����Ͽ����ϴ�."));
		return ids;
	}

	return 0;
}

INT CKL_STATISTICS_SEARCH1::GetStatisticsQuery(CString &strQuery)
{
	CString strCondition;
	if (GetStatisticsQueryCondition(strCondition) < 0)
		return -10;

	// ����/���ɺ��� ���� ������ �������ִ�
	switch (m_nType)
	{
	case KL_STAT_TYPE_REGUSER_GENDER:
	case KL_STAT_TYPE_SECUSER_GENDER:
	case KL_STAT_TYPE_FIRSTREG_GENDER:
	case KL_STAT_TYPE_OTHERIMP_GENDER:
		{
			strQuery.Format(_T("SELECT DECODE(GPIN_SEX, '0', '����', '1', '����', '��Ÿ') AS ����, COUNT(1) ")
							  _T("FROM CO_LOAN_USER_TBL ")
							 _T("WHERE %s ")
							 _T("GROUP BY DECODE(GPIN_SEX, '0', '����', '1', '����', '��Ÿ') ")
							 _T("ORDER BY DECODE(����, '����', '0', '����', '1', '��Ÿ', '2')"), strCondition);
		}
		break;
	case KL_STAT_TYPE_REGUSER_AGE:
	case KL_STAT_TYPE_SECUSER_AGE:
	case KL_STAT_TYPE_FIRSTREG_AGE:
	case KL_STAT_TYPE_OTHERIMP_AGE:
		{
			strQuery.Format(_T("SELECT ���ɴ�, COUNT(1) FROM ( ")
								_T("SELECT ")
								  _T("CASE ")
									_T("WHEN TO_CHAR(SYSDATE, 'YYYY')-TO_CHAR(BIRTHDAY, 'YYYY') < '10' THEN '10�� �̸�' ")
									_T("WHEN TO_CHAR(SYSDATE, 'YYYY')-TO_CHAR(BIRTHDAY, 'YYYY') BETWEEN '10' AND '19' THEN '10��' ")
									_T("WHEN TO_CHAR(SYSDATE, 'YYYY')-TO_CHAR(BIRTHDAY, 'YYYY') BETWEEN '20' AND '29' THEN '20��' ")
									_T("WHEN TO_CHAR(SYSDATE, 'YYYY')-TO_CHAR(BIRTHDAY, 'YYYY') BETWEEN '30' AND '39' THEN '30��' ")
									_T("WHEN TO_CHAR(SYSDATE, 'YYYY')-TO_CHAR(BIRTHDAY, 'YYYY') BETWEEN '40' AND '49' THEN '40��' ")
									_T("WHEN TO_CHAR(SYSDATE, 'YYYY')-TO_CHAR(BIRTHDAY, 'YYYY') BETWEEN '50' AND '59' THEN '50��' ")
									_T("WHEN TO_CHAR(SYSDATE, 'YYYY')-TO_CHAR(BIRTHDAY, 'YYYY') BETWEEN '60' AND '69' THEN '60��' ")
									_T("WHEN TO_CHAR(SYSDATE, 'YYYY')-TO_CHAR(BIRTHDAY, 'YYYY') BETWEEN '70' AND '79' THEN '70��' ")
									_T("WHEN TO_CHAR(SYSDATE, 'YYYY')-TO_CHAR(BIRTHDAY, 'YYYY') > '79' THEN '80�� �̻�' ")
									_T("ELSE '��Ÿ' ")
								  _T("END AS ���ɴ� ")
								  _T("FROM CO_LOAN_USER_TBL ")
								 _T("WHERE %s ) ")
							 _T("GROUP BY ���ɴ� ")
							 _T("ORDER BY DECODE(���ɴ�, '10�� �̸�', '0', '10��', '1', '20��', '2', '30��', '3', '40��', '4', '50��', '5', '60��', '6', '70��', '7', '80��', '8', '��Ÿ', '9')")
				, strCondition);
		}
		break;
	}

	return 0;
}

INT CKL_STATISTICS_SEARCH1::GetStatisticsQueryCondition(CString &strCondition)
{
	CString strDate1;
	CString strDate2;
	m_pCM->GetControlMgrData(_T("��¥����"), strDate1);
	m_pCM->GetControlMgrData(_T("��¥����"), strDate2);

	if (strDate1.IsEmpty())
	{
		CString strAlias;
		GetDlgItemText(IDC_STATIC3, strAlias);

		CString strMessage;
		strMessage.Format(_T("%s�� �Է��Ͽ� �ֽʽÿ�"), strAlias);
		AfxMessageBox(strMessage);

		return -10;
	}	

	CString strDateType;
	switch (m_nType)
	{
	case KL_STAT_TYPE_REGUSER_GENDER:	strDateType = _T("KL_REG_DATE");	break;
	case KL_STAT_TYPE_REGUSER_AGE:		strDateType = _T("KL_REG_DATE");	break;
	case KL_STAT_TYPE_SECUSER_GENDER:	strDateType = _T("KL_SECEDE_DATE");	break;
	case KL_STAT_TYPE_SECUSER_AGE:		strDateType = _T("KL_SECEDE_DATE");	break;
	case KL_STAT_TYPE_FIRSTREG_GENDER:	strDateType = _T("KL_REG_DATE");	break;
	case KL_STAT_TYPE_FIRSTREG_AGE:		strDateType = _T("KL_REG_DATE");	break;
	case KL_STAT_TYPE_OTHERIMP_GENDER:	strDateType = _T("KL_REG_DATE");	break;
	case KL_STAT_TYPE_OTHERIMP_AGE:		strDateType = _T("KL_REG_DATE");	break;
	}

	CString strDateCondition;
	if (!strDate2.IsEmpty())
	{
		strDateCondition.Format(_T("TO_CHAR(%s, 'YYYY/MM/DD') BETWEEN '%s' AND '%s'"), strDateType, strDate1, strDate2);
	}
	else
	{
		strDateCondition.Format(_T("TO_CHAR(%s, 'YYYY/MM/DD')='%s'"), strDateType, strDate1);
	}

	CString strMemberCondition;
	switch (m_nType)
	{
	case KL_STAT_TYPE_REGUSER_GENDER:	strMemberCondition = _T("KL_MEMBER_YN='Y'");	break;
	case KL_STAT_TYPE_REGUSER_AGE:		strMemberCondition = _T("KL_MEMBER_YN='Y'");	break;
	case KL_STAT_TYPE_SECUSER_GENDER:	strMemberCondition = _T("(KL_MEMBER_YN='N' OR KL_MEMBER_YN IS NULL)");	break;
	case KL_STAT_TYPE_SECUSER_AGE:		strMemberCondition = _T("(KL_MEMBER_YN='N' OR KL_MEMBER_YN IS NULL)");	break;
	case KL_STAT_TYPE_FIRSTREG_GENDER:	strMemberCondition = _T("KL_MEMBER_YN='Y'");	break;
	case KL_STAT_TYPE_FIRSTREG_AGE:		strMemberCondition = _T("KL_MEMBER_YN='Y'");	break;
	case KL_STAT_TYPE_OTHERIMP_GENDER:	strMemberCondition = _T("KL_MEMBER_YN='Y'");	break;
	case KL_STAT_TYPE_OTHERIMP_AGE:		strMemberCondition = _T("KL_MEMBER_YN='Y'");	break;
	}

	CString strManageCondition;
	CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO2);
	ASSERT(pRadio);
	if (pRadio->GetCheck())
	{
		strManageCondition.Format(_T("MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	}

	CString strExtraCondition;
	switch (m_nType)
	{	
	case KL_STAT_TYPE_FIRSTREG_GENDER:
	case KL_STAT_TYPE_FIRSTREG_AGE:
		{
			strExtraCondition.Format(_T("(KL_IMPORT_YN IS NULL OR KL_IMPORT_YN='N')"));
		}
		break;
	case KL_STAT_TYPE_OTHERIMP_GENDER:
	case KL_STAT_TYPE_OTHERIMP_AGE:
		{
			strExtraCondition.Format(_T("KL_IMPORT_YN='Y'"));
		}
		break;
	}

	strCondition.Format(_T("%s"), strDateCondition);

	if (!strMemberCondition.IsEmpty())
	{
		strCondition += _T(" AND ");
		strCondition += strMemberCondition;
	}
	if (!strManageCondition.IsEmpty())
	{
		strCondition += _T(" AND ");
		strCondition += strManageCondition;
	}
	if (!strExtraCondition.IsEmpty())
	{
		strCondition += _T(" AND ");
		strCondition += strExtraCondition;
	}

	return 0;
}
HBRUSH CKL_STATISTICS_SEARCH1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}


CString CKL_STATISTICS_SEARCH1::MakeCondition()
{	
	CString strCondition;
	CString strWhere;

	CString strData[2];
	
	m_pCM->GetControlMgrData(_T("��¥����"), strData[0]);
	m_pCM->GetControlMgrData(_T("��¥����"), strData[1]);	

	if(0 == m_nType || 1 == m_nType || 2 == m_nType || 3 == m_nType)
	{
		strCondition = MakeRangeCondition(_T("WORK_DATE"), strData[0], strData[1]);
		if(((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())
		{
			strWhere.Format(_T("%s AND CREATE_LIB_CODE = '%s'"), strCondition, m_pInfo->LIB_CODE);
		}
		else
		{
			strWhere.Format(_T("%s"), strCondition);
		}
	}
	else if(4 == m_nType || 5 == m_nType)
	{
		strWhere = MakeRangeCondition(_T("RETURN_DATE"), strData[0], strData[1]);
	}

	return strWhere;
}

CString CKL_STATISTICS_SEARCH1::MakeRangeCondition(CString strColumnAlias, CString strStart, CString strEnd)
{
	CString strCondition;

	strCondition = _T("");
	if ( !strStart.IsEmpty() && !strEnd.IsEmpty() )
	{
		strCondition.Format(_T("AND %s BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') "), strColumnAlias, strStart, strEnd);	
	}
	else if ( !strStart.IsEmpty() )
	{
		strCondition.Format(_T("AND %s=%s"), strColumnAlias, strStart);		
	}
	else if ( !strEnd.IsEmpty() )
	{
		strCondition.Format(_T("AND %s=%s"), strColumnAlias, strEnd);		
	}

	return strCondition;
}
void CKL_STATISTICS_SEARCH1::OnOk() 
{
	if (ExecStatistics() == 0)
	{
		CDialog::OnOK();
	}
	return;
}


VOID CKL_STATISTICS_SEARCH1::SetCellGridData(CString strColAlias, CString strRowAlias, CString strValue)
{	
	INT rows = m_pParentGrid->GetRows();
	INT cols = m_pParentGrid->GetColCount();
	INT x = -1,y = -1;

	INT i;
	CString strCol;
	CString strRow;
	for ( i=0 ; i<cols ; i++ )
	{
		strCol = m_pParentGrid->GetTextMatrix(0,i+1);
		if ( strCol == strColAlias )
		{
			x=i;
			break;
		}
	}

	for ( i=0 ; i<rows-1 ; i++ )
	{
		strRow = m_pParentGrid->GetTextMatrix(i+1,0);
		INT nOffset=strRow.Find(_T("��"), 0);
		if ( strRow=="10���̸�" ) strRow=_T("0");
		if ( strRow=="60���̻�" ) 
		{
			if ( _ttoi(strRowAlias) >= 60 )
			{
				strRowAlias=_T("60���̻�");
			}
		}


		if ( nOffset > 0 ) strRow = strRow.Mid(0,nOffset);		
		if ( strRow == strRowAlias)
		{
			y=i;
			break;
		}
	}

	if( (x >= 0 && y >= 0) && ( x < cols && y < rows))
	{
		m_pParentGrid->SetTextMatrix(y+1,x+1,strValue);
	}
}

VOID CKL_STATISTICS_SEARCH1::RowSum(INT StartRow, INT nEndRow, INT nTotalRow)
{
	CStringArray strArrayColTotal;
	CStringArray strArrayRowTotal;

	strArrayColTotal.RemoveAll();
	strArrayRowTotal.RemoveAll();

	CString strValue;
	INT nValue;
	INT nSum;

	strValue = _T("");

	for ( int x=StartRow; x<=nEndRow+1 ; x++ )
	{
		strArrayColTotal.Add(strValue);
	}	

	INT y;

	INT xSum;

	for ( x=0; x<m_pParentGrid->GetColCount() ; x++ )
	{
		nSum = 0;
		for ( y=0 ; y<=nEndRow-StartRow ; y++ )
		{
			strValue = m_pParentGrid->GetTextMatrix(y+StartRow,x+1);
			nValue = _ttoi(strValue); 						
			nSum += nValue;		
			
			xSum = 0;
			strValue = strArrayColTotal.GetAt(y);
			nValue = _ttoi(strValue);
			nValue += nSum;
			strValue.Format(_T("%d"), nValue);
			strArrayColTotal.SetAt(y, strValue);
		}

		strValue.Format(_T("%d"), nSum);
		m_pParentGrid->SetTextMatrix(nTotalRow, x+1, strValue);

		xSum = 0;
		strValue = strArrayColTotal.GetAt(nEndRow-StartRow+1);		
		xSum += nSum;
		strValue.Format(_T("%d"), xSum);
		strArrayColTotal.SetAt(nEndRow-StartRow+1, strValue);
	}

	for ( y=0 ; y<=nEndRow-StartRow ; y++ )
	{
		strArrayColTotal.SetAt(y, m_pParentGrid->GetColCount());
	}
	

}

VOID CKL_STATISTICS_SEARCH1::DM2AddViewGrid(CESL_DataMgr *m_pDM)
{

}

VOID CKL_STATISTICS_SEARCH1::InitDM(CESL_DataMgr *m_pDM, INT nRowCount, INT nColCount)
{	

	m_pDM->FreeData();
	m_pDM->FreeDMRef();	
	
}

VOID CKL_STATISTICS_SEARCH1::InitGrid(INT nRowCount, INT nColCount)
{	
	m_pParentGrid->SetRows(nRowCount);	
	m_pParentGrid->SetCols(0, nColCount);	

	m_pParentGrid->SetColWidth(-1, 0, 1200);
	for(INT nIdx = 0; nIdx < nColCount; nIdx++)
	{
		m_pParentGrid->SetColAlignmentFixed(nIdx, 4);
		m_pParentGrid->SetColAlignment(nIdx, 4);
		for(INT nRow = 0; nRow < nRowCount; nRow++)
		{
			if(0 < nRow && 0 < nIdx)
			{
				m_pParentGrid->SetTextMatrix(nRow, nIdx, _T(""));
			}
		}
	}
}

CKL_STATISTICS_SEARCH1::KL_RestructDataManager(CESL_DataMgr *m_pDM, CString strQuery, INT nColCount)
{	
	CString strColCount;
	CString strUpperQuery;

	strUpperQuery = strQuery;
	strUpperQuery.MakeUpper();
	strUpperQuery.TrimLeft();
	strUpperQuery.TrimRight();

	INT ids;


	strColCount.Format(_T("%d"), nColCount);
	CString strQueryType;

	INT nCheck1, nCheck2, nCheck3;
	nCheck1 = strUpperQuery.Find(_T("SELECT"), 0);
	nCheck2 = strUpperQuery.Find(_T("UPDATE"), 0);
	nCheck3 = strUpperQuery.Find(_T("DELETE"), 0);

	if ( nCheck1 == 0 ) strQueryType=_T("0");
	else if ( nCheck2 == 0 ) strQueryType=_T("1");
	else if ( nCheck3 == 0 ) strQueryType=_T("2");
	else return 0;	

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K26_LOC_B01_SERVICE"));		
	Writer.addRecord();			
	Writer.addElement(_T("���񽺸�")	, _T("TCT")		);
	Writer.addElement(_T("����")		, strQuery		);
	Writer.addElement(_T("�÷�����")	, strColCount	);
	Writer.addElement(_T("�ڵ�")		, strQueryType		);		
	
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	ids = Sender.Send(&Writer, &Reader);
	if(0 > ids)
	{
		return ids;
	}

	CString strResultCommand = Reader.getCommand();
	if(_T("ERROR") == strResultCommand)
	{
		m_pDM->FreeData();
	}
	else
	{
	}
	
	InitDM(m_pDM, Reader.getRecordCount(), Reader.getColumnCount() );	
	Reader.makeDM(m_pDM);
	DM2AddViewGrid(m_pDM);

	return 0;
}

INT CKL_STATISTICS_SEARCH1::DoPrev()
{
	CString strData[2];
	
	m_pCM->GetControlMgrData(_T("��¥����"), strData[0]);
	m_pCM->GetControlMgrData(_T("��¥����"), strData[1]);	

	if(strData[0].IsEmpty())
	{
		CString strMessage, strAlias;
		GetDlgItemText(IDC_STATIC3, strAlias);
		strMessage.Format(_T("%s�� �Է��Ͽ� �ֽʽÿ�"), strAlias);
		AfxMessageBox(strMessage);
		return -1000;
	}

	if(strData[1].IsEmpty())
	{
		strData[1] = strData[0];
		m_pCM->SetControlMgrData(_T("��¥����"), strData[0]);
	}

	INT nYear[2], nMonth[2], nDay[2];
	COleDateTime Date[2];
	_stscanf(strData[0], _T("%d/%d/%d"), &nYear[0], &nMonth[0], &nDay[0]);
	_stscanf(strData[1], _T("%d/%d/%d"), &nYear[1], &nMonth[1], &nDay[1]);

	Date[0].SetDate(nYear[0], nMonth[0], nDay[0]);
	Date[1].SetDate(nYear[1], nMonth[1], nDay[1]);

	INT nYearCnt = nYear[1] - nYear[0];
	if(0 > nYearCnt)
	{
		AfxMessageBox(_T("��¥�� ������ �߸��Ǿ����ϴ�."));
		return -1010;
	}

	INT nMonthCnt = ((nYearCnt * 12) + nMonth[1]) - nMonth[0] + 1;
	if(1 > nMonthCnt)
	{
		AfxMessageBox(_T("��¥�� ������ �߸��Ǿ����ϴ�."));
		return -1020;
	}

	{
		m_pParentGrid->SetRedraw(FALSE);

		if ( 0==m_nType || 2==m_nType)
		{
			InitGrid(4, (nMonthCnt) + 2);
		}
		else if ( 1==m_nType || 3==m_nType)
		{
			InitGrid(9, (nMonthCnt) + 2);
		}
		else if ( 4==m_nType || 5==m_nType)
		{
			InitGrid(50, (nMonthCnt) + 2);
		}
		CString strDay;
		INT nSetYear, nSetMonth;
		for(INT nCnt = -1; nCnt < nMonthCnt-1; nCnt++)
		{		
			nSetMonth = nMonth[0] + nCnt;
			nSetYear = nYear[0] + (nSetMonth / 12);
			nSetMonth = (nSetMonth % 12) +1;
			
			strDay.Format(_T("%04d�� %02d��"), nSetYear, nSetMonth);
			m_pParentGrid->SetTextMatrix(0, nCnt+2, strDay);
		}
		m_pParentGrid->SetTextMatrix(0, nMonthCnt+1, _T("�հ�"));
		
		if(0 == m_nType || 2 == m_nType)
		{
			const INT nRowAlias=3;
			CString strRowAlias[nRowAlias] = {			
			  _T("��"), _T("��"), _T("�հ�")			
			};
				
			m_pParentGrid->SetColAlignmentFixed(0, 4);
			for (INT i=0 ; i<nRowAlias ; i++ )
			{
				m_pParentGrid->SetTextMatrix(i+1,0, strRowAlias[i]);			
			}
		}
		else if(1 == m_nType || 3 == m_nType )
		{
			const INT nRowAlias=8;
			CString strRowAlias[nRowAlias] = {			
			  _T("10���̸�"), _T("10��"), _T("20��"), _T("30��"), _T("40��"), _T("50��"), _T("60���̻�") ,_T("�հ�") 
			};

			m_pParentGrid->SetColAlignmentFixed(0, 4);
			for (INT i=0 ; i<nRowAlias ; i++ )
			{
				m_pParentGrid->SetTextMatrix(i+1,0, strRowAlias[i]);
			}
		}
	
		m_pParentGrid->SetRedraw(FALSE);
	}

	return 0;
}

INT CKL_STATISTICS_SEARCH1::FindRow(CString& strAlias)
{
	if(_T("���") == strAlias)
	{
		return 1;
	}
	else if(_T("Ż��") == strAlias)
	{
		return 1;
	}
	else if(_T("��") == strAlias)
	{
		return 3;
	}
	else if(_T("��") == strAlias)
	{
		return 4;		
	}
	else if(_T("10���̸�") == strAlias)
	{
		return 6;
	}
	else if(_T("10��") == strAlias)
	{
		return 7;
	}
	else if(_T("20��") == strAlias)
	{
		return 8;
	}
	else if(_T("30��") == strAlias)
	{
		return 9;
	}
	else if(_T("40��") == strAlias)
	{
		return 10;
	}
	else if(_T("50��") == strAlias)
	{
		return 11;
	}
	else if(_T("60���̻�") == strAlias)
	{
		return 12;
	}

	return -1;
}

INT CKL_STATISTICS_SEARCH1::DoAfter()
{
	m_pParentGrid->SetRedraw(FALSE);

	if(4 == m_nType || 5 == m_nType)
	{		
		CESL_DataMgr TmpDM;
		INT cols = m_pParentGrid->GetColCount()+1;
		TmpDM.InitDataMgrRef(cols);
		TmpDM.m_nCols = cols;		
		
		CString* astrColName = new CString[cols];
		for(INT nCol = 0; nCol < cols; nCol++)
		{
			astrColName[nCol] = m_pParentGrid->GetTextMatrix(0, nCol);
		}
		BOOL bFind = FALSE;
		INT nRow = 0, nInsertRow = 0;
		INT nDMRow = 0, nMaxRow = m_pParentDM->GetRowCount();
		CString strData[3];
		for(nDMRow = 0; nDMRow < nMaxRow; nDMRow++)
		{			
			strData[0] = m_pParentDM->GetCellData(nDMRow, 0);
			strData[1] = m_pParentDM->GetCellData(nDMRow, 1);
			strData[2] = m_pParentDM->GetCellData(nDMRow, 2);

			nInsertRow = TmpDM.GetRowCount();
			for(nRow = 0; nRow < nInsertRow; nRow++)
			{
				CString strLib = TmpDM.GetCellData(nRow, 0);
				if(strData[1] == strLib)
				{
					break;
				}
			}
			if(nInsertRow == nRow)
			{
				TmpDM.InsertRow(-1);
				TmpDM.SetCellData(nRow, 0, strData[1]);
			}
			for(nCol = 0; nCol < cols; nCol++)
			{
				if(strData[0] == astrColName[nCol])
				{
					TmpDM.SetCellData(nRow, nCol, strData[2]);
					break;
				}
			}
		}
		delete []astrColName;

		InitGrid(TmpDM.GetRowCount()+2, cols);

		nMaxRow = TmpDM.GetRowCount();
		for(nRow = 0; nRow < nMaxRow; nRow++)
		{
			for(nCol = 0; nCol < cols; nCol++)
			{
				CString strData = TmpDM.GetCellData(nRow, nCol);
				m_pParentGrid->SetTextMatrix(nRow+1, nCol, strData);
			}
		}
		m_pParentGrid->SetTextMatrix(nMaxRow+1, 0, _T("�հ�"));
	}

	INT rows = m_pParentGrid->GetRows();
	INT cols = m_pParentGrid->GetColCount()+1;
	INT x = -1,y = -1;

	INT* pData = new INT[rows*cols];
	memset(pData, 0x00, sizeof(rows*cols));

	INT nTotalColPos = 0;
	CString strData;
	for(INT nRow = 0; nRow < rows ; nRow++)
	{
		for(INT nCol = 0; nCol < cols; nCol++)
		{
			strData = m_pParentGrid->GetTextMatrix(nRow, nCol);
			if(_T("�հ�") != strData)
			{
				pData[(cols*nRow)+nCol] = _ttoi(strData);
			}
			else
			{
				pData[(cols*nRow)+nCol] = -1;
			}
		}
	}

	for(nRow = 1; nRow < rows ; nRow++)
	{
		nTotalColPos = (cols * (nRow+1)) -1; 
		pData[nTotalColPos] = 0;
		for(INT nCol = 1; nCol < cols-1; nCol++)
		{
			pData[nTotalColPos] += pData[(cols*nRow)+nCol];
		}
	}

	INT rowTotal = 0;
	for(INT nCol = 1; nCol < cols; nCol++)
	{
		rowTotal = 0;
		for(nRow = 1; nRow < rows ; nRow++)
		{
			if(-1 == pData[(cols*nRow)])
			{
				pData[(cols*nRow)+nCol] = rowTotal;
				rowTotal = 0;
			}
			else
			{
				rowTotal += pData[(cols*nRow)+nCol];
			}
		}
	}

	for(nRow = 1; nRow < rows ; nRow++)
	{
		for(INT nCol = 1; nCol < cols; nCol++)
		{
 			strData.Format(_T("%d"), pData[(cols*nRow)+nCol]);
 			m_pParentGrid->SetTextMatrix(nRow, nCol, strData);
		}
	}

	m_pParentGrid->SetRedraw(TRUE);
	m_pParentGrid->UpdateWindow();

	delete []pData;
	pData = NULL;

	return 0;
}

INT CKL_STATISTICS_SEARCH1::ShowHideWindow()
{
	if( 0 == m_nType || 1 == m_nType)
	{
		GetDlgItem(IDC_RADIO1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO6)->ShowWindow(SW_HIDE);

		SetDlgItemText(IDC_STATIC3, _T("�������"));
	}
	else if( 2 == m_nType || 3 == m_nType)
	{
		GetDlgItem(IDC_RADIO1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO6)->ShowWindow(SW_HIDE);

		SetDlgItemText(IDC_STATIC3, _T("Ż������"));		
	}
	else if(4 == m_nType || 5 == m_nType)
	{
		SetDlgItemText(IDC_STATIC3, _T("�ݳ�����"));
	}
	else if(6 == m_nType)
	{
		SetDlgItemText(IDC_STATIC3, _T("��û����"));
	}
	else if(7== m_nType)
	{
		SetDlgItemText(IDC_STATIC3, _T("�߼�����"));
	}
	else if(8 == m_nType || 11 == m_nType)
	{
		SetDlgItemText(IDC_STATIC3, _T("��������"));
	}
	else if(9 == m_nType)
	{
		SetDlgItemText(IDC_STATIC3, _T("��û����"));
	}
	else if(10 == m_nType)
	{
		SetDlgItemText(IDC_STATIC3, _T("�Լ�����"));
	}
	else if(12 == m_nType || 13 == m_nType)
	{
		SetDlgItemText(IDC_STATIC3, _T("�ݳ�����"));
	}

	switch(m_nType)
	{
	case 0:
		SetWindowText(_T("å����ȸ��������(����)"));
		break;
	case 1:
		SetWindowText(_T("å����ȸ��������(���ɺ�)"));
		break;
	case 2:
		SetWindowText(_T("å����ȸ��Ż�����(����)"));
		break;
	case 3:
		SetWindowText(_T("å����ȸ��Ż�����(���ɺ�)"));
		break;	
	case 4:
		SetWindowText(_T("Ÿ���ݳ��߼����"));
		break;
	case 5:
		SetWindowText(_T("Ÿ���ݳ��������"));
		break;
	case 6:
		SetWindowText(_T("���� ��û(��û) ���"));
		break;
	case 7:
		SetWindowText(_T("���� �߼� ���"));
		break;
	case 8:
		SetWindowText(_T("����Ÿ���������"));
		break;
	case 9:
		SetWindowText(_T("��û(��û) ���"));
		break;
	case 10:
		SetWindowText(_T("��û(�Լ�) ���"));
		break;
	case 11:
		SetWindowText(_T("��ûŸ���������"));
		break;
	case 12:
		SetWindowText(_T("Ÿ���ݳ�(Ÿ������->Ÿ���ݳ�)���"));
		break;
	case 13:
		SetWindowText(_T("Ÿ���ݳ�(�ڰ�����->Ÿ���ݳ�)���"));
		break;
	default:
		break;
	}

	return 0;
}

CString CKL_STATISTICS_SEARCH1::MakeQuery()
{
	return _T("");
}

INT CKL_STATISTICS_SEARCH1::LILL_STATISTICS(CESL_DataMgr *m_pDM, INT nStatCode)
{
	CString strCode;
	CString strYear1,strMonth1,strDay1;
	CString strYear2,strMonth2,strDay2;
	CString strDate1,strDate2;
	INT ids;
	
	m_pCM->GetControlMgrData(_T("��¥����"),strDate1);
	m_pCM->GetControlMgrData(_T("��¥����"),strDate2);

	if(strDate1.IsEmpty() || strDate2.IsEmpty())
	{
		CString strAlias, strMessage;
		GetDlgItemText(IDC_STATIC3, strAlias);
		strMessage.Format(_T("%s�� �Է��Ͽ� �ֽʽÿ�"), strAlias);
		AfxMessageBox(strMessage);
		return -100;
	}

	strYear1 = strDate1.Left(4);
	strMonth1 = strDate1.Mid(5,2);
	strDay1 = strDate1.Right(2);

	strYear2 = strDate2.Left(4);
	strMonth2 = strDate2.Mid(5,2);
	strDay2 = strDate2.Right(2);
	CString strFlag;

	if(((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck())
	{
		strFlag = _T("QUARTER");
		if(_T("01") != strMonth1 && _T("04") != strMonth1 && _T("07") != strMonth1 && _T("10") != strMonth1 )
		{
			AfxMessageBox(_T("�б⺰ ��ȸ������ ���� ���� 1,4,7,10 �� �̾�� �մϴ�."));
			return -100;
		}

		if(_T("03") != strMonth2 && _T("06") != strMonth2 && _T("09") != strMonth2 && _T("12") != strMonth2)
		{
			AfxMessageBox(_T("�б⺰ ��ȸ������ ���� ���� 3,6,9,12 �� �̾�� �մϴ�."));
			return -100;
		}
		strDay1 = _T("01");
		strDay2 = _T("01");
		
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck())
	{
		strFlag = _T("YEAR");
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO5))->GetCheck())
	{
		strFlag = _T("MONTH");
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO6))->GetCheck())
	{
		strFlag = _T("DAY");
	}
	
	CString strQuery;
	if		(strFlag.CompareNoCase(_T("MONTH")) == 0) 
		strQuery.Format(_T("SELECT 'OK' FROM DUAL WHERE TO_CHAR(TO_DATE('%s/%s/%s', 'YYYY/MM/DD') + (INTERVAL '12' MONTH), 'YYYY/MM/DD') >= '%s/%s/%s'"), strYear1, strMonth1, strDay1, strYear2, strMonth2, strDay2);
	else if (strFlag.CompareNoCase(_T("QUARTER")) == 0)
		strQuery.Format(_T("SELECT 'OK' FROM DUAL WHERE TO_CHAR(TO_DATE('%s/%s/%s', 'YYYY/MM/DD') + (INTERVAL '12' MONTH), 'YYYY/MM/DD') >= '%s/%s/%s'"), strYear1, strMonth1, strDay1, strYear2, strMonth2, strDay2);
	else if (strFlag.CompareNoCase(_T("DAY")) == 0)
		strQuery.Format(_T("SELECT 'OK' FROM DUAL WHERE TO_CHAR(TO_DATE('%s/%s/%s', 'YYYY/MM/DD') + (INTERVAL '15' DAY), 'YYYY/MM/DD') >= '%s/%s/%s'"), strYear1, strMonth1, strDay1, strYear2, strMonth2, strDay2);
	else if (strFlag.CompareNoCase(_T("YEAR")) == 0)
		strQuery.Format(_T("SELECT 'OK' FROM DUAL WHERE TO_CHAR(TO_DATE('%s/%s/%s', 'YYYY/MM/DD') + (INTERVAL '3' YEAR), 'YYYY/MM/DD') >= '%s/%s/%s'"), strYear1, strMonth1, strDay1, strYear2, strMonth2, strDay2);
	
	CESL_DataMgr* pQueryDM = FindDM(_T("DM_KL_STATISTICS"));
	ASSERT(pQueryDM);
	
	CString strValue;
	pQueryDM->GetOneValueQuery(strQuery, strValue);
	
	if (strValue.IsEmpty())
	{
		CString strMessage;
		if		(strFlag.CompareNoCase(_T("MONTH")) == 0) 
			strMessage.Format(_T("'����' ���� �ִ� 12�����Ⱓ�� ������ �� �ֽ��ϴ�."));
		else if (strFlag.CompareNoCase(_T("QUARTER")) == 0) 
			strMessage.Format(_T("'�б⺰' ���� �ִ� 12���� �Ⱓ�� ������ �� �ֽ��ϴ�."));
		else if (strFlag.CompareNoCase(_T("DAY")) == 0) 
			strMessage.Format(_T("'�Ϻ�' ���� �ִ� 15�� �Ⱓ�� ������ �� �ֽ��ϴ�."));
		else if (strFlag.CompareNoCase(_T("YEAR")) == 0)
			strMessage.Format(_T("'�⺰' ���� �ִ� 3�� �Ⱓ�� ������ �� �ֽ��ϴ�."));
		AfxMessageBox(strMessage, MB_ICONINFORMATION);
		return -100;		
	}

	switch (nStatCode)
	{
	case KL_STAT_TYPE_OTHERRETURN_OTHER:	strCode = _T("07");	break;
	case KL_STAT_TYPE_OTHERRETURN_SELF:		strCode = _T("08");	break;
	case KL_STAT_TYPE_OTHERLOAN_SELF:		strCode = _T("03");	break;
	case KL_STAT_TYPE_OTHERLOAN_OTHER:		strCode = _T("06");	break;
	}
	CString strStartDate, strEndDate;
	strStartDate.Format(strDate1 + _T(" 00:00:00"));
	strEndDate.Format(strDate2 + _T(" 23:59:59"));

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("LILL_K04_LOC_B01_SERVICE"));		
	Writer.addRecord();			
	Writer.addElement(_T("MASTER_LIB_CODE"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("STAT_CODE"), strCode);
	Writer.addElement(_T("STAT_DEPTH"), _T("LOC"));
	Writer.addElement(_T("DATE_FLAG"), strFlag); 
	Writer.addElement(_T("START_DATE"), strStartDate); 
	Writer.addElement(_T("END_DATE"), strEndDate);
	
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;

	ids = Sender.Send(&Writer, &Reader);
	if(0 > ids)
	{
		return ids;
	}

	CString strResultCommand = Reader.getCommand();
	if(_T("ERROR") == strResultCommand)
	{
		m_pDM->FreeData();
		m_pParentGrid->Display();
	}
	else
	{
	}
	
	CString strResultMsg = Reader.getElement(0,_T("RESULT"));
	if(!strResultMsg.IsEmpty())
	{
		AfxMessageBox(strResultMsg);
		m_pDM->FreeData();
		m_pParentGrid->Display();
		return -100;
	}

	m_pParentGrid->SetRedraw(FALSE);

	CStringArray saColumnName;

	InitGrid(Reader.getRecordCount()+2, Reader.getColumnCountNew()+1); //header + �հ�

	for(int i=0; i<Reader.getColumnCountNew(); i++)
	{
		saColumnName.Add(Reader.getColumnNameNew(i));
		if(i == Reader.getColumnCountNew()-1)
		{
			m_pParentGrid->SetTextMatrix(0,i+1,_T("�հ�"));
		}
		else
		m_pParentGrid->SetTextMatrix(0,i+1,saColumnName.GetAt(i));
		
	}
	CString strColumnValue;
	for(int k = 0; k < Reader.getRecordCount(); k++) 
	{
		INT nColumnValue=0;		
		m_pParentGrid->SetTextMatrix(k+1,0,Reader.getElement(k,_T("TARGET_LIB_NAME")));	
		for(int j=0;j<Reader.getColumnCountNew();j++)
		{
			m_pParentGrid->SetTextMatrix(k+1,j+1, Reader.getElement(k,saColumnName.GetAt(j)));
			nColumnValue+= _ttoi(Reader.getElement(k,saColumnName.GetAt(j)));
		}
	}
	for(int x=0; x < Reader.getColumnCountNew(); x++)
	{
		m_pParentGrid->SetTextMatrix(Reader.getRecordCount()+1,x+1,Reader.getElementNew(0,saColumnName.GetAt(x)));	
	}

	m_pParentGrid->SetTextMatrix(Reader.getRecordCount()+1,0,_T("�հ�"));

	m_pParentGrid->SetRedraw(TRUE);
	m_pParentGrid->UpdateWindow();

	return 0;
}