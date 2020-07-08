// SE_CAT_INPUT_VOL.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_INPUT_VOL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_INPUT_VOL dialog


CSE_CAT_INPUT_VOL::CSE_CAT_INPUT_VOL(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pParent = NULL;
	m_pParent = pParent;

	m_pCM = NULL;
	m_pDM_Volume = NULL;
	m_pDM_Species = NULL;
	m_pDM_Index = NULL;

//	m_pCatApi = NULL;
}

CSE_CAT_INPUT_VOL::~CSE_CAT_INPUT_VOL()
{
}

VOID CSE_CAT_INPUT_VOL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSE_CAT_INPUT_VOL, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_INPUT_VOL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_INPUT_VOL message handlers

BOOL CSE_CAT_INPUT_VOL::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_����_����_���Է�")) < 0)
	{
		AfxMessageBox ( _T("Init SM_����_����_���Է� Error") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_����_����_��"));
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox ( _T("Init CM_����_����_�� Error") );
		return FALSE;
	}

	m_pDM_Volume = FindDM(_T("DM_����_����DM_������"));
	if ( m_pDM_Volume == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_����_����DM_������ Error") );
		return FALSE;
	}

	m_pDM_Species = FindDM(_T("DM_����_����DM_������"));
	if ( m_pDM_Species == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_����_����DM_������ Error") );
		return FALSE;
	}

	m_pDM_Index = FindDM(_T("DM_����_����DM_��������"));
	if ( m_pDM_Index == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_����_����DM_�������� Error") );
		return FALSE;
	}

	// �� �Ѹ���
	//m_pCM->AllControlDisplay(0);

	// �ʱⰪ ����
	INT ids = InitData();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;

EFS_END
	return FALSE;
}

INT CSE_CAT_INPUT_VOL::InitData()
{
	CString strItemData;
	INT ids;

	// Combo �ʱ�ȭ ********************************************
	// å�Է»���
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_INPUT_BOOK_STATUS );
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("��������ڷ�") );
		pCombo->InsertString( 1, _T("�谡�ڷ�(�����ڷ�)") );
		pCombo->SetCurSel( 0 );
	}

	// ��ȣ����
	pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("�Ϲ�") );
		pCombo->InsertString( 1, _T("�պ�ȣ") );
		pCombo->SetCurSel( 0 );
	}

	// ���ݼ���
	pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_PRICE_CHARACTER );
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("����") );
		pCombo->InsertString( 1, _T("���ǰ") );
		pCombo->InsertString( 2, _T("���ݺҸ�") );
		strItemData.Format(_T(""));
		m_pDM_Species->GetCellData(_T("SS_���ݼ���"), 0, strItemData);
		if ( !strItemData.Compare(_T("����")) ) pCombo->SetCurSel( 0 );
		else if ( !strItemData.Compare(_T("���ǰ")) ) pCombo->SetCurSel( 1 );
		else if ( !strItemData.Compare(_T("���ݺҸ�")) ) pCombo->SetCurSel( 2 );
	}
	// ********************************************************

	// ��ũ ��������
	CMarc marc;
	CString strStreamMarc;
	ids = m_pDM_Species->GetCellData( _T("SS_MARC"), 0, strStreamMarc);
	ids = this->m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	// ������
	strItemData.Format(_T(""));
	this->m_pInfo->pMarcMgr->GetItem(&marc,_T("245$a"),strItemData);
	m_pCM->SetControlMgrData(_T("SV_������"), strItemData );

	// ���ı���
	m_pCM->SetControlMgrData(_T("SV_���ı���_�ڵ�"), m_pDM_Species->GetCellData(_T("SS_���ı���_�ڵ�"), 0) );

	// ��ü����
	m_pCM->SetControlMgrData(_T("SV_��ü����_�ڵ�"), m_pDM_Species->GetCellData(_T("SS_��ü����_�ڵ�"), 0) );

	// ��ȣ�Ϸù�ȣ
	strItemData = m_pDM_Species->GetCellData(_T("SS_������ȣ�Ϸù�ȣ"), 0);
	strItemData.Format(_T("%d"), _ttoi(strItemData)+10 );
	m_pCM->SetControlMgrData(_T("SV_��ȣ�Ϸù�ȣ"), strItemData );

	// ȭ�󱸺�
	m_pCM->SetControlMgrData(_T("SS_ȭ�󱸺�_�ڵ�"), m_pDM_Species->GetCellData(_T("SS_ȭ�󱸺�_�ڵ�"), 0) );

	//����
	m_pCM->SetControlMgrData(_T("SS_����"), m_pDM_Species->GetCellData(_T("SS_����"), 0) );

	//���ݼ���
	m_pCM->SetControlMgrData(_T("SV_���ݼ���"), m_pDM_Species->GetCellData(_T("SS_���ݼ���"), 0) );

	// �����
	strItemData.Format(_T(""));
	this->m_pInfo->pMarcMgr->GetItem(&marc,_T("300$a"),strItemData);
	m_pCM->SetControlMgrData(_T("SV_�����"), strItemData );

	// ������Ư��
	strItemData.Format(_T(""));
	this->m_pInfo->pMarcMgr->GetItem(&marc,_T("300$b"),strItemData);
	m_pCM->SetControlMgrData(_T("SV_������Ư��"), strItemData );

	// åũ��
	strItemData.Format(_T(""));
	this->m_pInfo->pMarcMgr->GetItem(&marc,_T("300$c"),strItemData);
	m_pCM->SetControlMgrData(_T("SV_åũ��"), strItemData );

	// �����ڷ�
	strItemData.Format(_T(""));
	this->m_pInfo->pMarcMgr->GetItem(&marc,_T("300$d"),strItemData);
	m_pCM->SetControlMgrData(_T("SV_�����ڷ�"), strItemData );

	return 0;
}

VOID CSE_CAT_INPUT_VOL::OnOK() 
{
	// ������ �Է�
	INT ids = SaveDataToVolDM();

	// å���� �Է�

	CDialog::OnOK();
}

INT CSE_CAT_INPUT_VOL::SaveDataToVolDM()
{
	CComboBox * pCombo;
	CString strItemData;
	INT ids;
	INT nCurSel = 0;

	// 1/ ���Է� ++++++++++++++++++++++++++++++++++++++++++++++++
	m_pDM_Volume->InsertRow(-1);
	INT nCurtVolIdx = m_pDM_Volume->GetRowCount() -1;

	const INT cnt  = 17;
	CString alias[cnt] = {
			_T("SV_����"),
			_T("SV_���ݼ���"),
			_T("SV_��ȣ����"),
			_T("SV_��ȣ��"),
			_T("SV_��ȣ������"),
			_T("SV_��ȣ�Ϸù�ȣ"),
			_T("SV_������"),
			_T("SV_�����ڷ�"),
			_T("SV_��ü����_�ڵ�"),
			_T("SV_�����"),
			_T("SV_������Ư��"),
			_T("SV_��������"),
			_T("SV_��������ȣ��"),
			_T("SV_���"),
			_T("SV_åũ��"),
			_T("SV_���ı���_�ڵ�"),
			_T("SV_ȭ�󱸺�")
			};

	CString tmp;
	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM_Volume->SetCellData(alias[i],tmp,nCurtVolIdx);
		// ��ȣ�Ϸù�ȣ ���� ����
		if (i == 5)	m_pDM_Species->SetCellData(_T("SS_������ȣ�Ϸù�ȣ"), tmp, 0);
	}

	// SV_��KEY
	m_pDM_Volume->SetCellData(_T("SV_��KEY"),m_pDM_Species->GetCellData(_T("SS_��KEY"), 0),nCurtVolIdx);

	// SV_��ȣ����
	m_pDM_Volume->SetCellData(_T("SV_��ȣ����"),_T("V"),nCurtVolIdx);

	// SV_���α�ȣ�� -> ����� �Է��ϵ���

	// SV_���� ���� 
	strItemData = _T("");
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_PRICE_CHARACTER );
	if(pCombo) pCombo->GetWindowText(strItemData);
	m_pDM_Volume->SetCellData(_T("SV_���ݼ���"),strItemData,nCurtVolIdx);
	

	// SV_��ȣ����
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
	if(pCombo) 
	{
		nCurSel = pCombo->GetCurSel();
		
		if(0 == nCurSel) //�Ϲ�
			m_pDM_Volume->SetCellData(_T("SV_��ȣ����"),_T("1"),nCurtVolIdx);
		else if(1 == nCurSel) //�պ�ȣ
			m_pDM_Volume->SetCellData(_T("SV_��ȣ����"),_T("2"),nCurtVolIdx);
	}

	// SV_DB_FLAG
	m_pDM_Volume->SetCellData(_T("SV_DB_FLAG"),_T("I"),nCurtVolIdx);

	// ����, �������۾��Ͻ� ������ Ȯ�� ��

	// ��Ű tmp : å���� Sync�� ���߾��־�� �Ѵ�.
	m_pDM_Volume->MakeRecKey(strItemData);
	m_pDM_Volume->SetCellData(_T("SV_��KEY"),strItemData,nCurtVolIdx);

	// 2/ å�Է� ++++++++++++++++++++++++++++++++++++++++++++++++
	// �������
	pCombo = ( CComboBox * )GetDlgItem( IDC_INPUT_BOOK_STATUS );
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
		strWorkingStatus.Format(_T("SEL212N"));	// ������
		pDM_Book = FindDM(_T("DM_����_����DM_����å����"));
		if ( pDM_Book == NULL ) 
		{
			AfxMessageBox ( _T("Init DM_����_����DM_����å���� Error") );
			return -1;
		}
	}

	// �Է�å����ŭ �־��ֱ�
	tmp = _T("");
	m_pCM->GetControlMgrData(_T("UDF_�Է�å��"),tmp);
	INT nDupNumber = 0;

	for(i=0; i<_ttoi(tmp); i++)
	{
		if (i != 0) nDupNumber = i+1;
		ids = ((CCatApi_SE*)m_pCatApi)->InsertBookToDM(m_pDM_Species, m_pDM_Volume, pDM_Book
								, nCurtVolIdx, strWorkingStatus, nDupNumber, _T(""), _T(""), _T(""), _T(""));

		if (ids<0) 
		{
			AfxMessageBox(_T("å �Է��� ���������� �Ϸ����� ���Ͽ����ϴ�."));
			return -1;
		}
	}
	
	// ��å��
	m_pDM_Volume->SetCellData(_T("SV_���ڷ��"), tmp, nCurtVolIdx);


	return 0;
}
HBRUSH CSE_CAT_INPUT_VOL::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
