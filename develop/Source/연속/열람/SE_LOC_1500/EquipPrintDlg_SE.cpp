// EquipPrintDlg_SE.cpp : implementation file
//

#include "stdafx.h"
#include "EquipPrintDlg_SE.h"
#include "SE_LOC_1599_EX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintDlg_SE dialog


CEquipPrintDlg_SE::CEquipPrintDlg_SE(CESL_Mgr* pParent /*=NULL*/)
	: CEquipPrintDlg(pParent)
{
	//{{AFX_DATA_INIT(CEquipPrintDlg_SE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//m_pParentMgr = pParent;
}


VOID CEquipPrintDlg_SE::DoDataExchange(CDataExchange* pDX)
{
	CEquipPrintDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEquipPrintDlg_SE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEquipPrintDlg_SE, CDialog)
	//{{AFX_MSG_MAP(CEquipPrintDlg_SE)
	ON_BN_CLICKED(IDC_bCHANGE_PRINT_FORMAT, OnbCHANGEPRINTFORMAT)
	ON_BN_CLICKED(IDC_bPRINT_EQUIP, OnbPRINTEQUIP)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bDeleteDm, OnbDeleteDm)
//	ON_EN_KILLFOCUS(IDC_EDIT_CALLNO, OnKillfocusEditCallno)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintDlg_SE message handlers


INT CEquipPrintDlg_SE::Init()
{
	EFS_BEGIN

	if (!m_pDM_Parent || !m_pGrid_Parent|| m_strSpeciesKeyAlias.IsEmpty()) return -1;
	
	// ��� ���� ����
	m_pDM = FindDM(_T("DM_����_���_���_����"));
	m_pCM = FindCM(_T("CM_����_���_���_����"));
	m_pGrid = reinterpret_cast<CESL_Grid*>(m_pCM->FindControl(_T("GRID_LIST2")));

	if (!m_pDM || !m_pCM || !m_pGrid) return -1;

	m_pGrid->SetProtectSelectColumn(TRUE);
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	// ���� ����Ʈ üũ�Ѵ�.
	INT ids = -1;
	ids = m_pGrid_Parent->SelectMakeList();
	if (ids < 0) return ids;

	INT nCount = m_pGrid_Parent->SelectGetCount();
	if (nCount <= 0)
	{
		if ( m_arrStrSpeciesKey.GetSize() < 1 ) {
			m_strLastErrMsg = _T("������ �ڷᰡ �����ϴ�.");
			return -1;
		}
	}

	// ��å��ȣ
	CString sQuery = _T("");
	CString sValue = _T("") ;

	sQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '����' AND GROUP_3 = '����' AND CLASS_ALIAS = '���̺��å��ȣ����' AND MANAGE_CODE = UDF_MANAGE_CODE");
	m_pDM->GetOneValueQuery( sQuery , sValue );	
	sValue.TrimLeft(); sValue.TrimRight();
	//m_strVolCode = sValue;

	// ���� ����Ʈ�� ���鼭 ��Ű�� �����´�.
	CStringArray arrStrSpeciesKey;

	INT nIdx = m_pGrid_Parent->SelectGetHeadPosition();
	//if (nIdx < 0) return -1;
	if (nIdx < 0 && m_arrStrSpeciesKey.GetSize() < 1) return -1;

	CString strSpeciesKey;
	while (nIdx >= 0)
	{
		// DM���� ��ȣŰ�� �����´�.(������ ��ȣŰ)
		m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, nIdx, strSpeciesKey);
		if (!strSpeciesKey.IsEmpty())
			arrStrSpeciesKey.Add(strSpeciesKey);

		nIdx = m_pGrid_Parent->SelectGetNext();
	}

	GetDlgItem(IDC_bDeleteDm)->ShowWindow(SW_HIDE);
	// ����ڷ� ������ ������ ������ �� ������ ����Ѵ�.
	INT iSwSave = 0;	// ����ڷ� ������ ������ ������ 1, ������ 0
	if ( m_arrStrSpeciesKey.GetSize() > 0 ) {
		arrStrSpeciesKey.RemoveAll();
		for ( INT i = 0 ; i < m_arrStrSpeciesKey.GetSize() ; i++ ) {
			arrStrSpeciesKey.Add( m_arrStrSpeciesKey.GetAt(i) );
		}
		//AfxMessageBox( _T("����ڷ� ������ ������ ����մϴ�.") );
		iSwSave = 1;
		GetDlgItem(IDC_bDeleteDm)->ShowWindow(SW_SHOW);
	}

	// ����� ��Ű�� ������ ������ �����Ѵ�.
	CString strOption = _T("B.VOL_KEY IN (");
	nCount = arrStrSpeciesKey.GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		strOption += arrStrSpeciesKey.GetAt(idx);
		if (idx != nCount - 1)
			strOption += _T(", ");
	}

	strOption += _T(")");

	// ���� ���ӿ� ���� ���̺� �̸��� �� ������ �����Ѵ�.
	CString strFieldName;
	strFieldName = _T("DECODE(BOOK_TYPE, 'A', '�η�', 'B', '����')");

	// å�ηϿ� ���� ������ �ٲپ��ش�.
	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = m_pDM->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)m_pDM->RefList.GetNext(pos);
		if (pRef && pRef->FIELD_ALIAS == _T("å�η�"))
		{
			pRef->FIELD_NAME = strFieldName;
			break;
		}		
	}


	// ������ �ڷ� ����� ���
	// ���� Ű�� ������ 
	if ( iSwSave == 1 ) {
		CString strTmpOption = _T(" AND B.REC_KEY NOT IN (");
		nCount = m_arrStrDeleteBookKey.GetSize();
		
		for (INT idx = 0; idx < nCount; idx++)
		{
			strTmpOption += m_arrStrDeleteBookKey.GetAt(idx);
			if (idx != nCount - 1)
				strTmpOption += _T(", ");
		}
		strTmpOption += _T(")");

		if (  nCount > 0 ) 
			strOption += strTmpOption;
	
	}

	//strOption = strOption + _T(" AND I.REC_KEY = B.SPECIES_KEY AND B.REG_NO = 'EM0000000011'");
	ids = m_pDM->RefreshDataManager(strOption);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("�����ͺ��̽��� �۾��߿� ������ �߻��߽��ϴ�.");
		return ids;
	}

	// ���� ������ ���� ����
	for ( INT i = m_pDM->GetRowCount()-1 ; i >= 0 ; i-- ) {
		CString sBookKey = m_pDM->GetCellData( _T("BB_åKEY"), i );
		INT iBookCount = m_arrStrDeleteBookKey.GetSize();
		for ( INT j = 0 ; j < m_arrStrDeleteBookKey.GetSize() ; j++ ) {
			if ( sBookKey == m_arrStrDeleteBookKey.GetAt(j) ) {
				m_pDM->DeleteRow(i);
				break;
			}
		}
	}
	// ������� �� û����ȣ�� ������ �ش�. (û����ȣ : ��ġ��ȣ �з���ȣ-������ȣ-��å��ȣ=������ȣ)
	CString strCallNo;
	CString strItem;
	nCount = m_pDM->GetRowCount();
	for (idx = 0; idx < nCount; idx++)
	{
		// ���� ����(2004.10.08)
		// û����ȣ ���� Format 2(�ٹٲ����)
		strCallNo = MakeCallNoPrintFormat2(idx);

		// û����ȣ
		ids = m_pDM->SetCellData(_T("û����ȣ"), strCallNo, idx);

		// �������
		ids = m_pDM->GetCellData(_T("���̺���±���"), idx, strItem);
		//if (strItem.IsEmpty())
		ids = m_pDM->SetCellData(_T("���̺���±���"), _T("2"), idx);
	}

	// Display
	m_pCM->AllControlDisplay();
	//SetGridHeight();

	// ��ü �����ϱ�
	nCount = m_pGrid->GetRows();
	for (idx = 0; idx < nCount - 1; idx++)
	{
		m_pGrid->SetAt(idx, m_pGrid->m_nSelectIdx, m_pGrid->m_sSelectString);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ���ӿ��� �߰� _0503
	// �׸���� ���ӿ� �׸��常 ������
	GetDlgItem(IDC_GRID_EQUIP_BOOK_LIST)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_GRID_EQUIP_BOOK_LIST2)->ShowWindow(SW_SHOW);

	// ������� ���� ��ư ���� - ������ �ٹٲ޸� �����
	GetDlgItem(IDC_bCHANGE_PRINT_FORMAT)->EnableWindow(FALSE);

	// ������� �޺��ڽ� ��Ȱ��ȭ
	GetDlgItem(IDC_cPRINT_FORMAT)->EnableWindow(FALSE);


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	return 0;

	EFS_END
	return -1;
}

BOOL CEquipPrintDlg_SE::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);
	
	if (InitESL_Mgr(_T("SM_����_���_���")) < 0) 
	{
		m_strLastErrMsg = _T("Init ESL Mgr Error!!");
		goto ERR;
	}

	if (Init() < 0) goto ERR;	
	
//	m_pEditCtrl = new CEdit();
//	m_pEditCtrl->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN, CRect(0, 0, 0, 0), this, IDC_EDIT_CALLNO);
//	m_pEditCtrl->ShowWindow(SW_HIDE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
	
}

VOID CEquipPrintDlg_SE::OnbPRINTEQUIP() 
{
	EFS_BEGIN

	// ����ڷḦ ����Ѵ�.
	CSE_LOC_1599_EX dlg(this);
	dlg.SetParentInfo(m_pDM, m_pGrid);
	dlg.DoModal();

	EFS_END
}

/// �����ڷ� ����
VOID CEquipPrintDlg_SE::OnbDeleteDm() 
{
EFS_BEGIN
	
	if ( NULL == m_pGrid ) return;
	if ( NULL == m_pDM ) return;

	// 1. ������ �ڷ� �ִ��� Ȯ��.
	m_pGrid->SelectMakeList();
	INT idx = m_pGrid->SelectGetTailPosition();

	if( idx < 0 ) 
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	INT ids;
	CString sBookKey;
	// 2. ������ �ڷḦ DM���� ����.
	// 3. ����Ű ����Ʈ�� �߰�
	while (  idx>=0 )
	{
		ids = m_pDM->GetCellData(_T("BB_åKEY"), idx, sBookKey );
		if ( ids< 0 ) return;

		m_arrStrDeleteBookKey.Add(sBookKey);
		m_pDM->DeleteRow(idx);

		idx = m_pGrid->SelectGetPrev();
	}
	
	m_pGrid->SetProtectSelectColumn(FALSE);
	m_pGrid->Display();

EFS_END
}


