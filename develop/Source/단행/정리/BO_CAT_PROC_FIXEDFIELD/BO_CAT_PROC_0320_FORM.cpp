// BO_CAT_PROC_0320_FORM.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0320_FORM.h"
#include "BO_CAT_PROC_0320_FORM_01.h"
#include "BO_CAT_PROC_0320_FORM_02.h"
#include "BO_CAT_PROC_0320_FORM_03.h"
#include "BO_CAT_PROC_0320_FORM_04.h"
#include "BO_CAT_PROC_0320_FORM_05.h"
#include "BO_CAT_PROC_0320_FORM_06.h"
#include "BO_CAT_PROC_0320_FORM_07.h"
#include "BO_CAT_PROC_0320_FORM_08.h"
#include "BO_CAT_PROC_0320_FORM_09.h"
#include "BO_CAT_PROC_0320_FORM_10.h"
#include "BO_CAT_PROC_0320_FORM_11.h"
#include "BO_CAT_PROC_0320_FORM_12.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM dialog


CBO_CAT_PROC_0320_FORM::CBO_CAT_PROC_0320_FORM(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0320_FORM)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	/*m_pPage1 = NULL;
	m_pPage2 = NULL;
	m_pPage3 = NULL;
	m_pPage4 = NULL;
	m_pPage5 = NULL;
	m_pPage6 = NULL;
	m_pPage7 = NULL;
	m_pPage8 = NULL;
	m_pPage9 = NULL;
	m_pPage10 = NULL;
	m_pPage11 = NULL;
	m_pPage12 = NULL;
	*/

	m_pForm = NULL;

}

CBO_CAT_PROC_0320_FORM::~CBO_CAT_PROC_0320_FORM()
{
}

BOOL CBO_CAT_PROC_0320_FORM::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0320_FORM::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0320_FORM)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0320_FORM, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0320_FORM)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_cMAT_CATEGORY, OnSelchangecMATCATEGORY)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM message handlers
BOOL CBO_CAT_PROC_0320_FORM::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cMAT_CATEGORY);
	if (pComboBox)
	{
		if ( m_sType == _T("BK") ) {
			pComboBox->AddString(_T("����ũ�������ڷ�"));
			pComboBox->AddString(_T("�μ��ڷ�"));
			pComboBox->AddString(_T("���¸������Ҽ������ڷ�"));
		}
		else {
			pComboBox->AddString(_T("�����ڷ�"));
			pComboBox->AddString(_T("��ü"));
			pComboBox->AddString(_T("��鿵���ڷ�"));
			pComboBox->AddString(_T("����ũ�������ڷ�"));
			pComboBox->AddString(_T("���񿵻��ڷ�"));
			pComboBox->AddString(_T("��ȭ"));
			pComboBox->AddString(_T("�����ڷ�"));
			pComboBox->AddString(_T("�μ��ڷ�"));
			pComboBox->AddString(_T("������ȭ�ڷ�"));
			pComboBox->AddString(_T("���¸������Ҽ������ڷ�"));
			pComboBox->AddString(_T("��ǻ������"));
		}

		pComboBox->SetCurSel(0);
	}

	CString sCategory = m_sReceive007.Left(1);
	TCHAR cCategory;
	if ( sCategory == _T("") ) {
		AfxMessageBox(_T("��ġ�ϴ� �ڷ���ְ� �����ϴ�.") );
		//cCategory = a;
	}
	else {
		cCategory = sCategory.GetAt(0);
	}
	

	if ( m_sType == _T("BK") ) {
		if ( sCategory == _T("") ) {
			CreateScreen('h');
			return TRUE;
		}
		else if ( sCategory == _T("t") ) {
			pComboBox->SetCurSel(1);
			//CreateScreen( cCategory );	
		}
		else if ( sCategory == _T("z") ) {
			pComboBox->SetCurSel(2);
			//CreateScreen( cCategory );
		}
		CreateScreen( cCategory );
	}
	else {		//TX�� �ƴҰ��
		if ( sCategory == _T("") ) {
			//AfxMessageBox( _T("��ġ�ϴ� �ڷ���ְ� �����ϴ�.") );
			CreateScreen( 'a' );
			return TRUE;
		}
		else if ( sCategory == _T("d") ) {
			pComboBox->SetCurSel(1);
			//CreateScreen( cCategory );
		}
		else if ( sCategory == _T("g") ) {
			pComboBox->SetCurSel(2);
			//CreateScreen( cCategory );
		}
		else if ( sCategory == _T("h") ) {
			pComboBox->SetCurSel(3);		
		}
		else if ( sCategory == _T("k") ) {
			pComboBox->SetCurSel(4);		
		}
		else if ( sCategory == _T("m") ) {
			pComboBox->SetCurSel(5);		
		}
		else if ( sCategory == _T("s") ) {
			pComboBox->SetCurSel(6);		
		}
		else if ( sCategory == _T("t") ) {
			pComboBox->SetCurSel(7);		
		}
		else if ( sCategory == _T("v") ) {
			pComboBox->SetCurSel(8);		
		}
		else if ( sCategory == _T("z") ) {
			pComboBox->SetCurSel(9);		
		}
		else if ( sCategory == _T("c") ) {
			pComboBox->SetCurSel(10);		
		}
		CreateScreen( cCategory );
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0320_FORM::OnOK() 
{
EFS_BEGIN

	if (m_pForm == NULL) return;
	CESL_ControlMgr *pCM;
	INT iCh;
	pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_01"));
	iCh = 1;
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_02"));
		iCh = 2;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_03"));
		iCh = 3;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_04"));
		iCh = 4;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_05"));
		iCh = 5;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_06"));
		iCh = 6;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_07"));
		iCh = 7;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_08"));
		iCh = 8;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_09"));
		iCh = 9;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_10"));
		iCh = 10;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_11"));
		iCh = 11;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_12"));
		iCh = 12;
	}

	CString sMessage;	//�ʼ��׸� �޼���
	s007 = _T("");
	CString sItemData;
	if ( iCh == 1 ) {		
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("�ڷ����ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", �ڷ����ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("�ڷ�ĺ�ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ڷ�ĺ�ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("��ä"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��ä");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("��������ü"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��������ü");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
	}
	else if ( iCh == 2 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("�ڷ����ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", �ڷ����ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("�ڷ�ĺ�ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ڷ�ĺ�ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("��ä"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��ä");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("��������ü"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��������ü");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
	}
	else if ( iCh == 3 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("�ڷ����ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", �ڷ����ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("�ڷ�ĺ�ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ڷ�ĺ�ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("��ä"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��ä");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("��ü���������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��ü���������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("������ϸ�ü"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ������ϸ�ü");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("�ʸ�����/ũ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ʸ�����/ũ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("Ʋ������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", Ʋ������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;		
	}
	else if ( iCh == 4 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("�ڷ����ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", �ڷ����ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("�ڷ�ĺ�ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ڷ�ĺ�ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("�԰�"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �԰�");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("��Һ�������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��Һ�������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("��Һ���"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��Һ���");
		}
		if ( sItemData == _T("") ) sItemData = _T("   ");
		if ( sItemData.GetLength() < 3 ) {
			while ( sItemData.GetLength() < 3 ) {
				sItemData = sItemData + _T(" ");
			}
		}
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("��ä"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��ä");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("�ʸ��Ǳ⺻���"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ʸ��Ǳ⺻���");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
	}
	else if ( iCh == 5 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("�ڷ����ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", �ڷ����ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("�ڷ�ĺ�ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ڷ�ĺ�ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("��ä"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��ä");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("�⺻���"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �⺻���");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("Ʋ������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", Ʋ������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;		
	}
	else if ( iCh == 6 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("�ڷ����ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", �ڷ����ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("�ڷ�ĺ�ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ڷ�ĺ�ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("��ä"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��ä");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("��ũ������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��ũ������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("��ü���������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��ü���������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("������ϸ�ü"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ������ϸ�ü");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("�ʸ�����"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ʸ�����");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("�������������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �������������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("���ۿ��"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("���ۻ���"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("�ʸ��Ǳ⺻���"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("�����ǹ���"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("õ������ȭ����"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("�ʸ��ǰ˻�����"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T("      ");
		s007 = s007 + sItemData;
	}
	else if ( iCh == 7 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("�ڷ����ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", �ڷ����ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("�ڷ�ĺ�ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ڷ�ĺ�ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("�ӵ�"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ӵ�");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("�����������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �����������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("��������/����"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��������/����");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("ũ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ũ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("����������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ����������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("�������Ǳ���"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �������Ǳ���");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("�������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("����"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("Ư���������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("������������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;		
	}
	else if ( iCh == 8 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_08"), _T("�ڷ����ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", �ڷ����ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_08"), _T("�ڷ�ĺ�ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ڷ�ĺ�ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;		
	}
	else if ( iCh == 9 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("�ڷ����ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", �ڷ����ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("�ڷ�ĺ�ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ڷ�ĺ�ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("��ä"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��ä");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("������ȭ����"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ������ȭ����");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("��ü���������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ��ü���������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("������ϸ�ü"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ������ϸ�ü");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("ũ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", ũ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("�������������"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �������������");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
	}
	else if ( iCh == 10 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_10"), _T("�ڷ����ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", �ڷ����ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_10"), _T("�ڷ�ĺ�ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ڷ�ĺ�ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;		
	}
	else if ( iCh == 11 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("�ڷ����ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", �ڷ����ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("�ڷ�ĺ�ǥ��"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", �ڷ�ĺ�ǥ��");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("��ä"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("�԰�"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("����"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("�ػ�"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T("   ");
		if ( sItemData.GetLength() < 3 ) {
			while ( sItemData.GetLength() < 3 ) {
				sItemData = sItemData + _T(" ");
			}
		}
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("��������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("ǰ������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("������������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("�������"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("��������ǰ��"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
	}

	//�޼��� �����ֱ�
	if ( sMessage.GetLength() > 3 ) {
		sMessage = sMessage.Mid(2) + _T("��(��) �Է��ؾ� �մϴ�.");
		AfxMessageBox( sMessage );
		return;
	}

	CDialog::OnOK();

EFS_END
}

INT CBO_CAT_PROC_0320_FORM::CreateScreen(TCHAR tchCatagory)
{
EFS_BEGIN

	if (tchCatagory == 0) return -1;

	CRect rect;
	GetClientRect(rect);

	rect.top	+= 50; 
	rect.left	+= 5;
	rect.bottom -= 50; 
	rect.right	-= 5;


	if (m_pForm != NULL)
	{
		delete m_pForm;
		m_pForm = NULL;
	}

	switch(tchCatagory)
	{
		case 'a':
			m_pForm = new CBO_CAT_PROC_0320_FORM_01(this, m_sReceive007);
			//((CBO_CAT_PROC_0320_FORM_01*)m_pForm)->
			((CBO_CAT_PROC_0320_FORM_01*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'd':
            m_pForm = new CBO_CAT_PROC_0320_FORM_02(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_02*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
            break;
        case 'g':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_03(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_03*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
            break;
        case 'h':
			m_pForm = new CBO_CAT_PROC_0320_FORM_04(this, m_sReceive007);
	        //m_pForm = new CBO_CAT_PROC_0320_FORM_04(this);
            ((CBO_CAT_PROC_0320_FORM_04*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'k':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_05(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_05*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'm':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_06(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_06*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 's':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_07(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_07*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 't':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_08(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_08*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'v':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_09(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_09*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'z':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_10(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_10*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'c':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_11(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_11*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;

		default:
			m_pForm = new CBO_CAT_PROC_0320_FORM_12(this, m_sReceive007);
			((CBO_CAT_PROC_0320_FORM_12*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();

			break;
	}

	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_PROC_0320_FORM::OnSelchangecMATCATEGORY() 
{
EFS_BEGIN

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cMAT_CATEGORY);
	if (pComboBox)
	{
		INT nSel = pComboBox->GetCurSel();
		if (nSel != CB_ERR)
		{
			CString strBuffer;
			pComboBox->GetLBText(nSel, strBuffer);
			if (!strBuffer.IsEmpty())
			{
				TCHAR cForm;
				if ( strBuffer == _T("�����ڷ�") ) {
					cForm = 'a';
				}
				else if ( strBuffer == _T("��ü") ) {
					cForm = 'd';
				}
				else if ( strBuffer == _T("��鿵���ڷ�") ) {
					cForm = 'g';
				}		
				else if ( strBuffer == _T("����ũ�������ڷ�") ) {
					cForm = 'h';
				}
				else if ( strBuffer == _T("���񿵻��ڷ�") ) {
					cForm = 'k';
				}
				else if ( strBuffer == _T("��ȭ") ) {
					cForm = 'm';
				}
				else if ( strBuffer == _T("�����ڷ�") ) {
					cForm = 's';
				}
				else if ( strBuffer == _T("�μ��ڷ�") ) {
					cForm = 't';
				}
				else if ( strBuffer == _T("������ȭ�ڷ�") ) {
					cForm = 'v';
				}
				else if ( strBuffer == _T("���¸������Ҽ������ڷ�") ) {
					cForm = 'z';
				}
				else if ( strBuffer == _T("��ǻ������") ) {
					cForm = 'c';
				}
				else if ( strBuffer == _T("��") ) {
					cForm = 'o';
				}
				CreateScreen(cForm);
			}
		}
	}

EFS_END
}

VOID CBO_CAT_PROC_0320_FORM::OnDestroy() 
{
EFS_BEGIN

	CDialog::OnDestroy();
	
	if (m_pForm != NULL)
	{
		delete m_pForm;
		m_pForm = NULL;
	}
	

EFS_END
}

INT CBO_CAT_PROC_0320_FORM::SetType007(CString sType, CString s007)
{
EFS_BEGIN

	m_sReceive007 = s007;
	m_sType = sType;
	return 0;

EFS_END
return -1;

}


HBRUSH CBO_CAT_PROC_0320_FORM::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}