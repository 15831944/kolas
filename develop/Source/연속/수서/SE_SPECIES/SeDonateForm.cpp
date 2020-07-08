// SeDonateForm.cpp : implementation file
//

#include "stdafx.h"
#include "SeDonateForm.h"
#include "..\..\..\����\COMMON_DONATOR_MANAGER\DonatorNameGetterDlg.h"
#include "..\..\..\����\COMMON_DONATOR_MANAGER\DonatorNameGetter_Search.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeDonateForm dialog


CSeDonateForm::CSeDonateForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeDonateForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeDonateForm)
	m_bCLAIM_YN = FALSE;
	m_bREAD_FIRST_YN = FALSE;

/*
	//����Ʈ ������ ���� ������ �����켱�̴�
#ifdef _LIGHT_VERSION_
	m_bREAD_FIRST_YN = TRUE;
#else
	m_bREAD_FIRST_YN = FALSE;
#endif
*/

	//}}AFX_DATA_INIT

	m_pParentMgr = pParent;
	m_pDM = NULL;
	m_pCM = NULL;
	m_strDonatorKey = _T("");
	m_bIsLastCtrlFocused = FALSE;

	m_bIsLightVersion = FALSE;
}

CSeDonateForm::~CSeDonateForm()
{
}

VOID CSeDonateForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeDonateForm)
	DDX_Check(pDX, IDC_chkDON_CLAIM_YN, m_bCLAIM_YN);
	DDX_Check(pDX, IDC_chkDON_READ_FISRT_YN, m_bREAD_FIRST_YN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeDonateForm, CDialog)
	//{{AFX_MSG_MAP(CSeDonateForm)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnDON_DONATOR_SEARCH, OnbtnDONDONATORSEARCH)
	ON_EN_KILLFOCUS(IDC_edtDON_CLAIM_INTERVAL, OnKillfocusedtDONCLAIMINTERVAL)
	ON_EN_SETFOCUS(IDC_edtDON_CLAIM_INTERVAL, OnSetfocusedtDONCLAIMINTERVAL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeDonateForm message handlers

BOOL CSeDonateForm::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}
	
BOOL CSeDonateForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( InitESL_Mgr( _T("����_����_��������") ) < 0 ) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_��������") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_����_��������"));
	m_pDM = FindDM(_T("DM_����_��������"));
	if(!m_pDM || !m_pCM)  {
		AfxMessageBox( _T("DM,CM Error : DM_����_�������� or CM_����_��������") );
		return false;
	}
	
	//control color
	AddColorControl(IDC_edtDON_ACQ_BOOK_CNT, _T(""), _T(""), 'E');
	
	//����Ʈ ���� settting
	if(m_bIsLightVersion)
	{
		SetLightVersion();
	}

	CString str64;
	GetManageValue( _T("��Ÿ"), _T("����"), _T("����"), _T("����"), str64 );
	if ( str64 == _T("Y") ) 
	{
		GetDlgItem(IDC_sYookSa1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbDept)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_sYookSa2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbCource)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_sYookSa1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbDept)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_sYookSa2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbCource)->ShowWindow(SW_HIDE);
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSeDonateForm::SetLightVersion()
{
	//����Ʈ ������ ��� �����켱����, ���å�� ���� ��Ʈ�Ѿ��ְ� �Լ�å�� ���� ��Ʈ���� ��ġ�� ���ġ �Ѵ� 
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_chkDON_READ_FISRT_YN);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_STATIC_DON_REG_BOOK_CNT);
	if(pWnd) pWnd->ShowWindow(FALSE);

	pWnd = GetDlgItem(IDC_edtDON_REG_BOOK_CNT);
	if(pWnd) pWnd->ShowWindow(FALSE);

	CRect rect;
	
	pWnd = GetDlgItem(IDC_STATIC_DON_ACQ_BOOK_CNT);
	if(pWnd)
	{
		pWnd->GetClientRect(rect);
	
		rect.left = 16;
		rect.right = 65;
		rect.top = 41;
		rect.bottom = 55;

		pWnd->MoveWindow(rect);
	}
	
	pWnd = GetDlgItem(IDC_edtDON_ACQ_BOOK_CNT);
	if(pWnd)
	{
		pWnd->GetClientRect(rect);
		
		rect.left = 80;
		rect.right = 185;
		rect.top = 39;
		rect.bottom = 59;		
		
		pWnd->MoveWindow(rect);
	}

	//����Ʈ ������ ���� ������ �����켱�̴�
	m_bREAD_FIRST_YN = TRUE;

	UpdateData(FALSE);
}

VOID CSeDonateForm::SaveAllDataToDM()
{
	
	UpdateData(TRUE);

	//DM���� ����������� data��
	const INT cnt = 7;
	CString alias[ cnt ] = {
		_T("SD_����������"),	//	0
		_T("SD_�����ڸ�"),		//	1
		_T("SD_���å��"),		//	2
		_T("SD_�Ƿڰ���"),		//	3
		_T("SD_�Լ�å��"),
		_T("SD_��û�а�"),
		_T("SD_��û�к�")};		//	4
	
	
	CString tmp;

	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");	
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM->SetCellData(alias[i],tmp,0);
	}
	

	//�Ƿڿ���
	if(m_bCLAIM_YN)
		m_pDM->SetCellData(_T("SD_�Ƿڿ���"),_T("Y"),0);
	else 
	    m_pDM->SetCellData(_T("SD_�Ƿڿ���"),_T("N"),0);

	//�����켱����
	if(m_bREAD_FIRST_YN)
		m_pDM->SetCellData(_T("SD_�����켱����"),_T("Y"),0);
	else 
	    m_pDM->SetCellData(_T("SD_�����켱����"),_T("N"),0);

	
	
	//������ KEY
	CString strDonatorName = _T("");
	strDonatorName = m_pDM->GetCellData(_T("SD_�����ڸ�"),0);
	
	if(strDonatorName.IsEmpty())
	{
		m_pDM->SetCellData(_T("SD_������KEY"),_T(""),0);
	}
	
	if(!m_strDonatorKey.IsEmpty() && !strDonatorName.IsEmpty()) 
	{
		m_pDM->SetCellData(_T("SD_������KEY"),m_strDonatorKey,0);
	}

}

INT CSeDonateForm::RefreshAll(CString strSpeciesKey)
{
	CString strWhere;
	strWhere.Format(_T("SPECIES_KEY = %s"),strSpeciesKey);
	m_pDM->RefreshDataManager(strWhere);
	m_pCM->AllControlDisplay(0);
	
	CString tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SD_�Ƿڿ���"),0);
	if(_T("Y") == tmp)
		m_bCLAIM_YN = TRUE;
	else 
		m_bCLAIM_YN = FALSE;

	tmp = _T("");
	

	tmp = m_pDM->GetCellData(_T("SD_�����켱����"),0);
	if(_T("Y") == tmp)
		m_bREAD_FIRST_YN = TRUE;
	else 
		m_bREAD_FIRST_YN = FALSE;
	
	
	//������ KEY
	m_strDonatorKey = _T("");
	
	UpdateData(FALSE);
	

	return 0;
}

VOID CSeDonateForm::ClearAll()
{

	m_pCM->AllControlClear();
	m_bCLAIM_YN = FALSE;
	m_bREAD_FIRST_YN = FALSE;

	//������ KEY
	m_strDonatorKey = _T("");
	UpdateData(FALSE);

}

HBRUSH CSeDonateForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

VOID CSeDonateForm::OnbtnDONDONATORSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	CDonatorNameGetter_Search searchDlg(this);
	searchDlg.SetOpenMode(-1000);
	if(IDOK == searchDlg.DoModal())
	{
		CDonatorNameGetterDlg searchResult(this);
		if(IDOK == searchResult.DoModal())
		{
			m_pCM->SetControlMgrData(_T("SD_�����ڸ�"),searchResult.GetDonatorName());
			m_strDonatorKey = searchResult.GetDonatorKey();
		}
	}
	
	
}

VOID CSeDonateForm::SetFocusOnFirstCtrl()
{
	GetDlgItem(IDC_edtDON_DONATOR_NAME)->SetFocus();
}
VOID CSeDonateForm::OnKillfocusedtDONCLAIMINTERVAL() 
{
	// TODO: Add your control notification handler code here
	
	m_bIsLastCtrlFocused = FALSE;
}

BOOL CSeDonateForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_TAB)
		{
			if(m_bIsLastCtrlFocused)
			{
				// ������ ��Ʈ���� killfocus�ϸ� parent���� �޽����� ������
				:: SendMessage(	m_pParentMgr->GetSafeHwnd(), KILLFOCUS_LAST_CTRL , 0, 0);

				return TRUE;
			}
		}
		else if ( pMsg->wParam == VK_ESCAPE )
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSeDonateForm::OnSetfocusedtDONCLAIMINTERVAL() 
{
	// TODO: Add your control notification handler code here
	m_bIsLastCtrlFocused = TRUE;
	
}


