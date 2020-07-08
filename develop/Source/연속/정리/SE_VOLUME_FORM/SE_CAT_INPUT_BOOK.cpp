// SE_CAT_INPUT_BOOK.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_INPUT_BOOK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_INPUT_BOOK dialog


CSE_CAT_INPUT_BOOK::CSE_CAT_INPUT_BOOK(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pParent = NULL;
	m_pParent = pParent;

	m_pCM = NULL;
	m_pDM_Volume = NULL;

	m_nSlectedVolIdx = 0;

//	m_pCatApi = NULL;
}

CSE_CAT_INPUT_BOOK::~CSE_CAT_INPUT_BOOK()
{
}

VOID CSE_CAT_INPUT_BOOK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_INPUT_BOOK)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_INPUT_BOOK, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_INPUT_BOOK)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_INPUT_BOOK message handlers

BOOL CSE_CAT_INPUT_BOOK::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_����_����_å�Է�")) < 0)
	{
		AfxMessageBox ( _T("Init SM_����_����_å�Է� Error") );
		return FALSE;
	}
	m_pCM = FindCM(_T("CM_����_����_å"));
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox ( _T("Init CM_����_����_å Error") );
		return FALSE;
	}

	m_pDM_Volume = FindDM(_T("DM_����_����DM_������"));
	if ( m_pDM_Volume == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_����_����DM_������ Error") );
		return FALSE;
	}

	//�ڷ� ���¸� ���� ���ش�.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("��������ڷ�") );
		pCombo->InsertString( 1, _T("�谡�ڷ�(�����ڷ�)") );
		pCombo->SetCurSel( 0 );
	}

	// ��ȣ�� Setting
	m_pCM->SetControlMgrData(_T("SV_��ȣ��"), m_pDM_Volume->GetCellData(_T("SV_��ȣ��"), m_nSlectedVolIdx) );

	// �谡��ġ��ȣ default -> Link Data�� �������� ������ ��������,,
	CESL_DataMgr* pDM_Species = FindDM(_T("DM_����_����DM_������"));
	if ( pDM_Species == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_����_����DM_������ Error") );
		return FALSE;
	}
	CString strShelfLocCode = pDM_Species->GetCellData(_T("SS_�谡��ġ_�ڵ�"), 0);
	m_pCM->SetControlMgrData(_T("SB_�谡��ġ��ȣ"), strShelfLocCode );

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;

EFS_END
	return FALSE;
}

VOID CSE_CAT_INPUT_BOOK::OnOK() 
{
	// å���� �Է�
	INT ids = SaveDataToDM();
	if (ids<0) return;

	CDialog::OnOK();
}

INT CSE_CAT_INPUT_BOOK::SaveDataToDM()
{
EFS_BEGIN
	CComboBox * pCombo;
	CString strItemData;
	INT ids;
	INT nCurSel = 0;

	// �������
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOOK_WORKING_STATUS );
	if(pCombo) nCurSel = pCombo->GetCurSel();

	CESL_DataMgr* pDM_Book;
	CString strWorkingStatus;
	if (nCurSel == 0) {
		strWorkingStatus.Format(_T("SEC111N"));	// ������
		pDM_Book = FindDM(_T("DM_����_����DM_å����"));
		if ( pDM_Book == NULL ) 
		{
			AfxMessageBox ( _T("Init DM_����_����DM_å���� Error") );
			return -1;
		}
	}
	if (nCurSel == 1) {
		strWorkingStatus.Format(_T("SEL212N"));	// ����å
		pDM_Book = FindDM(_T("DM_����_����DM_����å����"));
		if ( pDM_Book == NULL ) 
		{
			AfxMessageBox ( _T("Init DM_����_����DM_����å���� Error") );
			return -1;
		}
	}

	// �� DM
	CESL_DataMgr* pDM_Species = FindDM(_T("DM_����_����DM_������"));
	if ( pDM_Species == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_����_����DM_������ Error") );
		return -1;
	}

	// Data
	CString strRegCode, strAcqDate, strShelfLocCode, strRemark;
	m_pCM->GetControlMgrData(_T("SB_��ϱ���_�ڵ�"), strRegCode);
	m_pCM->GetControlMgrData(_T("SB_�Լ���"), strAcqDate);
	m_pCM->GetControlMgrData(_T("SB_�谡��ġ��ȣ"), strShelfLocCode);
	m_pCM->GetControlMgrData(_T("SB_���"), strRemark);

	// å�Է�
	ids = ((CCatApi_SE*)m_pCatApi)->InsertBookToDM(pDM_Species, m_pDM_Volume, pDM_Book
			, m_nSlectedVolIdx, strWorkingStatus, 0, strRegCode, strAcqDate, strShelfLocCode, strRemark );
	if (ids<0) 
	{
		AfxMessageBox(_T("å �Է��� ���������� �Ϸ����� ���Ͽ����ϴ�."));
		return -1;
	}
	
	// ���� ��å�� + 1
	m_pDM_Volume->GetCellData(_T("SV_���ڷ��"), m_nSlectedVolIdx, strItemData);
	ids = _ttoi(strItemData) + 1;
	strItemData.Format(_T("%d"), ids);
	m_pDM_Volume->SetCellData(_T("SV_���ڷ��"), strItemData, m_nSlectedVolIdx);

	CDialog::OnOK();

EFS_END
return -1;
}

HBRUSH CSE_CAT_INPUT_BOOK::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
