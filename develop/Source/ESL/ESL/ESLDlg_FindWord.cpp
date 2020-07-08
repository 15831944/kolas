// ESLDlg_FindWord.cpp : implementation file
//

#include "stdafx.h"
#include "ESLDlg_FindWord.h"
#include "ESL_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_FindWord dialog


CESLDlg_FindWord::CESLDlg_FindWord(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_FindWord::IDD, pParent)
{
	m_pParentGrid = NULL;

	m_nPrevFindIdx = -1;
}

CESLDlg_FindWord::~CESLDlg_FindWord()
{
}

void CESLDlg_FindWord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESLDlg_FindWord)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESLDlg_FindWord, CDialog)
	//{{AFX_MSG_MAP(CESLDlg_FindWord)
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_COM_eFINDWORD, OnChangeFindWord)
	ON_BN_CLICKED(IDC_COM_bCANCEL, OnbtnCANCEL)
	ON_BN_CLICKED(IDC_COM_bFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_COM_bALL, OnbtnALL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_FindWord message handlers

BOOL CESLDlg_FindWord::Create(CWnd* pParentWnd) 
{	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CESLDlg_FindWord::OnInitDialog() 
{
	CDialog::OnInitDialog();

	/// �ʱⰪ �Ʒ��� ����
	GetDlgItem(IDC_COM_rDOWN)->SendMessage(BM_SETCHECK, (WPARAM)BST_CHECKED);
		
	return TRUE;
}

BOOL CESLDlg_FindWord::PreTranslateMessage(MSG* pMsg) 
{	
	if( WM_KEYDOWN == pMsg->message )
	{
		///@{{	hotkey 
		if( 0x80 & (BYTE)GetKeyState(VK_CONTROL) )
		{
			/// ã�� ���� ��Ŀ�� �̵�	Ctrl + N
			if( 0x4E == pMsg->wParam )
			{
				GetDlgItem(IDC_COM_eFINDWORD)->SetFocus();
				return TRUE;
			}
			/// �Ʒ��� ����				Ctrl + D
			else if( 0x44 == pMsg->wParam )
			{		
				GetDlgItem(IDC_COM_rUPPER)->SendMessage(BM_SETCHECK, (WPARAM)BST_UNCHECKED);
				GetDlgItem(IDC_COM_rDOWN)->SendMessage(BM_SETCHECK, (WPARAM)BST_CHECKED);
				return TRUE;
			}
			/// ���� ����				Ctrl + U
			else if( 0x55 == pMsg->wParam )
			{
				GetDlgItem(IDC_COM_rUPPER)->SendMessage(BM_SETCHECK, (WPARAM)BST_CHECKED);
				GetDlgItem(IDC_COM_rDOWN)->SendMessage(BM_SETCHECK, (WPARAM)BST_UNCHECKED);				
				return TRUE;
			}
			/// ���� ã��				Ctrl + F
			else if( 0x46 == pMsg->wParam )
			{
				if( GetDlgItem(IDC_COM_bFIND)->IsWindowEnabled() )
				{
					OnbtnFIND();				
					return TRUE;
				}
			}
			/// ��� ����				Ctrl + A
			else if( 0x41 == pMsg->wParam )
			{
				if( GetDlgItem(IDC_COM_bALL)->IsWindowEnabled() )
				{
					OnbtnALL();				
					return TRUE;
				}
			}
		}
		///@}}	
		
		if( VK_RETURN == pMsg->wParam )
		{
			if( GetDlgItem(IDC_COM_bALL)->IsWindowEnabled() )
			{
				OnbtnALL();
				OnbtnCANCEL();
			}

			return TRUE;
			
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);
}

void CESLDlg_FindWord::OnChangeFindWord() 
{
	///@{{ ã�� ���� �Է� ���� Ȯ��
	CString strData;
	GetDlgItemText(IDC_COM_eFINDWORD, strData);

	if( strData.GetLength() )
	{
		GetDlgItem(IDC_COM_bFIND)->EnableWindow(TRUE);
		GetDlgItem(IDC_COM_bALL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COM_bFIND)->EnableWindow(FALSE);
		GetDlgItem(IDC_COM_bALL)->EnableWindow(FALSE);
	}
	///@}}
}

void CESLDlg_FindWord::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	/// �÷� ������ ����Ǿ� ���� �� �ִ�.
	if( bShow )
	{
		InitColumnInfo();

		/// �˻��� �ٽ��ϱ� ���� 
		m_nPrevFindIdx = -1;

		/// �Է�â�� ��Ŀ���� ��ġ ��Ų��
		GetDlgItem(IDC_COM_eFINDWORD)->SetFocus();		
	}
}

///@{{ �޺��ڽ� ����
INT CESLDlg_FindWord::InitColumnInfo()
{
EFS_BEGIN
	
	INT		ids;

	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;

	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COM_cTARGET);
	if( NULL == pBox )	return -1011;	

	/// ���� ����� �ִ� �׸����� 
	/// ���� �÷��� �����ؾ� �Ѵ�.
	INT	nSelectColIdx = -1;
	if( pParentGrid->m_bCheck && 'S' == pParentGrid->m_cSelectType )
	{
		nSelectColIdx = pParentGrid->m_nSelectIdx;	
		GetDlgItem(IDC_COM_bALL)->ShowWindow(SW_SHOW);
	}
	/// ���� �׸��尡 �ƴ϶��
	/// ��� ���� ��ư�� �����.
	else
	{
		GetDlgItem(IDC_COM_bALL)->ShowWindow(SW_HIDE);
	}

	/// ������ �ִٸ� �ٽ� �����Ѵ�.
	if( pBox->GetCount() )
	{
		pBox->ResetContent();
	}

	/// �⺻�� �Է� 
	ids = pBox->AddString( _T("�������") );
	if( CB_ERR == ids || CB_ERRSPACE == ids )	return -1021;

	/// �׸��� �÷����� �޺��ڽ��� �����Ѵ�.
	INT	nColCount = pParentGrid->GetColCount();

	CString strColumn;
	INT		nWidth;
	INT		nAlignment;

	for( INT i=0;i<nColCount;i++ )
	{
		if( i == nSelectColIdx )
			continue;

		ids = pParentGrid->GetColumnInfo( i, strColumn, nWidth, nAlignment );
		if( 0 > ids ) continue;

		ids = pBox->AddString( strColumn );
		if( CB_ERR == ids || CB_ERRSPACE == ids )	return -1021;
	}
	
	pBox->SetCurSel(0);
	pBox->UpdateWindow();

	return 0;
EFS_END
return -1;
}

INT	CESLDlg_FindWord::GetIndexColumn(INT nIndex, CString &strColumn)
{
EFS_BEGIN

	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COM_cTARGET);
	if( NULL == pBox )	return -1001;
	
	pBox->GetLBText(nIndex, strColumn);
	return 0;

EFS_END
return -1;
}
INT CESLDlg_FindWord::GetSelectColumn(CString &strColumn)
{
EFS_BEGIN

	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COM_cTARGET);
	if( NULL == pBox )	return -1001;
		
	return GetIndexColumn(pBox->GetCurSel(), strColumn);

EFS_END
return -1;
}

INT CESLDlg_FindWord::GetColumnCount()
{
EFS_BEGIN

	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COM_cTARGET);
	if( NULL == pBox )	return -1001;
		
	return pBox->GetCount();

EFS_END
return -1;
}
///@}}

void CESLDlg_FindWord::OnbtnCANCEL() 
{
EFS_BEGIN

	CDialog::OnOK();	

EFS_END
}

void CESLDlg_FindWord::OnbtnFIND() 
{
EFS_BEGIN
	
	INT		ids;
	
	ids = FindWordProc();
	if( 0 > ids )	
	{
		AfxMessageBox(_T("����� ���������� �������� ���߽��ϴ�"), MB_ICONSTOP);
	}
	else if( 100 == ids )
	{
		AfxMessageBox(_T("ã�� ����� �����ϴ�"), MB_ICONINFORMATION);
	}
	else if( 200 == ids )
	{
		AfxMessageBox(_T("ã�� ������ �Է��ؾ� �մϴ�"), MB_ICONINFORMATION);
	}

	/// �Է�â�� ��Ŀ���� ��ġ ��Ų��
	GetDlgItem(IDC_COM_eFINDWORD)->SetFocus();

EFS_END	
}

void CESLDlg_FindWord::OnbtnALL() 
{
EFS_BEGIN

	INT		ids;

	ids = AllSelectProc();	
	if( 0 > ids )	
	{
		AfxMessageBox(_T("����� ���������� �������� ���߽��ϴ�"), MB_ICONSTOP);
	}
	else if( 100 == ids )
	{
		AfxMessageBox(_T("ã�� ����� �����ϴ�"), MB_ICONINFORMATION);
	}
	else if( 200 == ids )
	{
		AfxMessageBox(_T("ã�� ������ �Է��ؾ� �մϴ�"), MB_ICONINFORMATION);
	}

	/// �Է�â�� ��Ŀ���� ��ġ ��Ų��
	GetDlgItem(IDC_COM_eFINDWORD)->SetFocus();

EFS_END	
}

///@{{ ���ڿ� �˻�
/// ���� DM�� ALIAS�� FIELD�� �����´�.
INT	CESLDlg_FindWord::GetLinkAlias(CString strColumn, CString &strDM_ALIAS, CString &strFIELD_ALIAS)
{
EFS_BEGIN

	INT	nColIdx = GetColIdx(strColumn);
	if( 0 > nColIdx )	return -3001;

	if( 0 > GetLinkAlias(nColIdx, strDM_ALIAS, strFIELD_ALIAS) )	return -3001;

	return 0;
EFS_END	
return -1;
}

/// idx�� �ش��ϴ� DM�� ALIAS�� FIELD�� �����´�.
INT CESLDlg_FindWord::GetLinkAlias(INT nColIdx, CString &strDM_ALIAS, CString &strFIELD_ALIAS)
{
EFS_BEGIN

	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;
	
	POSITION pos;
	pos = pParentGrid->m_arrColumnInfo.FindIndex(nColIdx);
	if( NULL == pos )	return -2001;
		
	CESL_GridColumnInfo* pColumnInfo = (CESL_GridColumnInfo*)pParentGrid->m_arrColumnInfo.GetAt(pos);
	if( NULL == pColumnInfo ) return -2011;

	strDM_ALIAS    = pColumnInfo->strColumnDMAlias;
	strFIELD_ALIAS = pColumnInfo->strColumnDMFieldAlias;

	return 0;
EFS_END	
return -1;
}

/// �÷��� �ش��ϴ� idx�� �����´�.
INT CESLDlg_FindWord::GetColIdx(CString strColumn)
{
EFS_BEGIN

	INT		ids;

	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;

	/// ���� ����� �ִ� �׸����� 
	/// ���� �÷��� �����ؾ� �Ѵ�.
	INT	nSelectColIdx = -1;
	if( pParentGrid->m_bCheck && 'S' == pParentGrid->m_cSelectType )
		nSelectColIdx = pParentGrid->m_nSelectIdx;

	INT	nColCount = pParentGrid->GetColCount();

	CString strColumnInfo;
	INT		nWidth;
	INT		nAlignment;

	for( INT i=0;i<nColCount;i++ )
	{
		if( i == nSelectColIdx )
			continue;

		ids = pParentGrid->GetColumnInfo( i, strColumnInfo, nWidth, nAlignment );
		if( 0 > ids ) continue;

		if( strColumnInfo == strColumn )
			return i;
	}

	return -1;
EFS_END	
return -1;
}

/// �˻�
/// �ӵ� ����� ���� �Լ�ȭ�� ���� �ʾҴ�.
INT	CESLDlg_FindWord::FindWordProc()
{
EFS_BEGIN
	
	INT		ids;

	/// �׸��� ����� ���ٸ� 
	/// �˻����� �ʴ´�
	INT	nGridRowCount;
	nGridRowCount = GetGridRowCount();
	if( 0 > nGridRowCount )  return -1001;
	if( 0 == nGridRowCount ) return 100;

	/// �˻�� �����´�.
	CString strFindWord;
	ids = GetFindWord(strFindWord);
	if( 0 > ids )  return -1001;

	strFindWord.TrimLeft();strFindWord.TrimRight();
	if( 0 == strFindWord.GetLength() )	return 200;

	/// �˻� ���� ����
	INT		nFindDirection = GetFindDirection();
	if( 0 > nFindDirection )	return -1021;

	/// �˻� ó�� ���� ��ġ�� ���Ѵ�.
	INT		nCurFindIdx = GetCurGridRowIdx();
	if( 0 > nCurFindIdx )	return -1011;

	/// ó�� �˻��̶��
	/// �Ǵ� ���� �˻����� ã�� ���ߴٸ�
	/// ���� ���õ� �ο찡 �˻� ��ġ�̴�.

	/// �����˻� ����� �ִٸ� 
	/// ���õ� �ο��� ���� ����� �˻� ��ġ�̴�.
	if( -1 != m_nPrevFindIdx )
	{		
		/// ���õ� ��ġ�� ���� ��ġ�� �Ȱ��ٸ� �̾ �˻��Ѵ�.
		/// �ٸ��ٸ� ���õ� ��ġ���� �˻��Ѵ�.
		if( m_nPrevFindIdx == nCurFindIdx )
			nCurFindIdx = GetNextFindIdx(nFindDirection, m_nPrevFindIdx);		

		if( !IsValidGridIdx(nCurFindIdx) )
		{
			nCurFindIdx = m_nPrevFindIdx;
		}
	}

	/// �˻���� �÷����� �����´�.
	CString strColumn;
	ids = GetSelectColumn(strColumn);
	if( 0 > ids )	return -1022;
	
	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;

	CESL_GridData* pData = (CESL_GridData*)pParentGrid->m_pData;
	if( NULL == pData )	return -1000;

	CESL_Mgr* pMgr = pData->pMgr;
	if( NULL == pMgr )	return -1000;

	CESL_DataMgr* pDM = NULL;

	CString strDM_ALIAS;
	CString strFIELD_ALIAS;

	/// ��������̶�� ��ü �÷��� ������� �ؾ��Ѵ�.
	if( _T("�������") == strColumn )
	{
		INT	nColumnCount = GetColumnCount();
		if( 0 > nColumnCount )	return -1023;

		/// ��������� �����Ѵ�.
		CStringArray arDM_ALIAS;
		CStringArray arFIELD_ALIAS;

		for( INT i=1;i<nColumnCount;i++ )
		{
			ids = GetIndexColumn( i, strColumn );
			if( 0 > ids )	return -2001;		

			ids = GetLinkAlias( strColumn, strDM_ALIAS, strFIELD_ALIAS );
			if( 0 > ids )	return -2001;
			
			if( 0 == arDM_ALIAS.GetSize() )
			{
				arDM_ALIAS.Add(strDM_ALIAS);
			}
			else
			{
				if( strDM_ALIAS != arDM_ALIAS.GetAt(0) )
				{
					AfxMessageBox(_T("'�������'�� �������� �ʴ� �׸����Դϴ�.\r\n")
								  _T("'�˻�����'�� ������ �ֽʽÿ�"), MB_ICONINFORMATION);
					return 300;
				}
			}
			arFIELD_ALIAS.Add(strFIELD_ALIAS);
		}
		
		pDM = pMgr->FindDM(strDM_ALIAS);
		if( NULL == pDM )	return -1000;

		do
		{
			CString strData;

			for( INT j=0;j<arFIELD_ALIAS.GetSize();j++ )
			{
				ids = pDM->GetCellData(arFIELD_ALIAS.GetAt(j), nCurFindIdx, strData);
				if( 0 > ids )	return -3001;

				strData.TrimLeft();strData.TrimRight();

				/// ã�Ҵٸ�
				/// ã�� �׸����� �ο츦 �����Ѵ�
				/// m_nPrevFindIdx�� �����Ѵ�.
				if( -1 != strData.Find(strFindWord) )
				{
					/// �׸��� ������ �̵�					
					pParentGrid->SendMessage( WM_VSCROLL, SB_TOP );
					for( INT k=0;k<nCurFindIdx;k++ )
						pParentGrid->SendMessage( WM_VSCROLL, SB_LINEDOWN );

					m_nPrevFindIdx = nCurFindIdx;
					pParentGrid->SetReverse(m_nPrevFindIdx);

					return 0;
				}
			}

			/// ã�� ���ߴٸ� ������ �˻��Ѵ�.
			nCurFindIdx = GetNextFindIdx(nFindDirection, nCurFindIdx);				
		}
		while( 0 <= nCurFindIdx && nGridRowCount > nCurFindIdx );		
	}
	else
	{
		ids = GetLinkAlias( strColumn, strDM_ALIAS, strFIELD_ALIAS );
		if( 0 > ids )	return -2001;		

		pDM = pMgr->FindDM(strDM_ALIAS);
		if( NULL == pDM )	return -1000;

		do
		{
			CString strData;
			ids = pDM->GetCellData(strFIELD_ALIAS, nCurFindIdx, strData);
			if( 0 > ids )	return -3001;

			strData.TrimLeft();strData.TrimRight();

			/// ã�Ҵٸ�
			/// ã�� �׸����� �ο츦 �����Ѵ�
			/// m_nPrevFindIdx�� �����Ѵ�.
			if( -1 != strData.Find(strFindWord) )
			{				
				/// �׸��� ������ �̵�					
				pParentGrid->SendMessage( WM_VSCROLL, SB_TOP );
				for( INT k=0;k<nCurFindIdx;k++ )
					pParentGrid->SendMessage( WM_VSCROLL, SB_LINEDOWN );

				m_nPrevFindIdx = nCurFindIdx;
				pParentGrid->SetReverse(m_nPrevFindIdx);
				
				return 0;
			}

			/// ã�� ���ߴٸ� ������ �˻��Ѵ�.
			nCurFindIdx = GetNextFindIdx(nFindDirection, nCurFindIdx);			
		}
		while( 0 <= nCurFindIdx && nGridRowCount > nCurFindIdx );		
	}

	/// ���� �˻� idx�� ������ �����ߴٸ�		
	CString strMessage;
	strMessage.Format(_T("\"%s\"��(��) ã�� �� �����ϴ�."), strFindWord);
	AfxMessageBox(strMessage, MB_ICONINFORMATION);

	return 0;
EFS_END	
return -1;
}

/// ��μ���
/// ��μ����� ���⿡ �������
/// ó������ ������ ���Ѵ�.
INT	CESLDlg_FindWord::AllSelectProc()
{
EFS_BEGIN
	
	INT		ids;
	
	/// �׸��� ����� ���ٸ� 
	/// �˻����� �ʴ´�
	INT	nGridRowCount;
	nGridRowCount = GetGridRowCount();
	if( 0 > nGridRowCount )  return -1001;
	if( 0 == nGridRowCount ) return 100;

	/// �˻�� �����´�.
	CString strFindWord;
	ids = GetFindWord(strFindWord);
	if( 0 > ids )  return -1001;

	strFindWord.TrimLeft();strFindWord.TrimRight();
	if( 0 == strFindWord.GetLength() )	return 200;

	/// �˻� ó�� ���� ��ġ�� ���Ѵ�.
	INT		nCurFindIdx = 0;
	
	/// �˻���� �÷����� �����´�.
	CString strColumn;
	ids = GetSelectColumn(strColumn);
	if( 0 > ids )	return -1022;
	
	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;

	CESL_GridData* pData = (CESL_GridData*)pParentGrid->m_pData;
	if( NULL == pData )	return -1000;

	CESL_Mgr* pMgr = pData->pMgr;
	if( NULL == pMgr )	return -1000;

	CESL_DataMgr* pDM = NULL;

	CString strDM_ALIAS;
	CString strFIELD_ALIAS;
	
	INT nFindCount = 0;

	/// ���� �˻� ��ġ�� �׸��� ����Ʈ�� �̵���Ű�� ����
	INT	nFirstFindIdx = -1;
	pParentGrid->SendMessage( WM_VSCROLL, SB_TOP );
	
	/// �����ϱ����� �׸��忡 ���õ� ��� üũ�� Ǯ���ش�.
	pParentGrid->UnSelectAll();

	/// ��������̶�� ��ü �÷��� ������� �ؾ��Ѵ�.
	if( _T("�������") == strColumn )
	{
		INT	nColumnCount = GetColumnCount();
		if( 0 > nColumnCount )	return -1023;

		/// ��������� �����Ѵ�.
		CStringArray arDM_ALIAS;
		CStringArray arFIELD_ALIAS;

		for( INT i=1;i<nColumnCount;i++ )
		{
			ids = GetIndexColumn( i, strColumn );
			if( 0 > ids )	return -2001;		

			ids = GetLinkAlias( strColumn, strDM_ALIAS, strFIELD_ALIAS );
			if( 0 > ids )	return -2001;
			
			if( 0 == arDM_ALIAS.GetSize() )
			{
				arDM_ALIAS.Add(strDM_ALIAS);
			}
			else
			{
				if( strDM_ALIAS != arDM_ALIAS.GetAt(0) )
				{
					AfxMessageBox(_T("'�������'�� �������� �ʴ� �׸����Դϴ�.\r\n")
								  _T("'�˻�����'�� ������ �ֽʽÿ�"), MB_ICONINFORMATION);
					return 300;
				}
			}
			arFIELD_ALIAS.Add(strFIELD_ALIAS);
		}
		
		pDM = pMgr->FindDM(strDM_ALIAS);
		if( NULL == pDM )	return -1000;

		do
		{
			CString strData;

			for( INT j=0;j<arFIELD_ALIAS.GetSize();j++ )
			{
				ids = pDM->GetCellData(arFIELD_ALIAS.GetAt(j), nCurFindIdx, strData);
				if( 0 > ids )	return -3001;

				strData.TrimLeft();strData.TrimRight();

				/// ã�Ҵٸ�
				/// ã�� �׸��带 üũ�Ѵ�.
				if( -1 != strData.Find(strFindWord) )
				{			
					nFindCount++;
					pParentGrid->SetAt(nCurFindIdx, pParentGrid->m_nSelectIdx, pParentGrid->m_sSelectString);

					if( -1 == nFirstFindIdx )
						nFirstFindIdx = nCurFindIdx;
					break;
				}
			}

			/// ã�� ���ߴٸ� ������ �˻��Ѵ�.
			nCurFindIdx ++;
			/// �׸��� ������ �̵�
//			pParentGrid->SendMessage( WM_VSCROLL, SB_LINEDOWN );
				
		}
		while( 0 <= nCurFindIdx && nGridRowCount > nCurFindIdx );		
	}
	else
	{
		ids = GetLinkAlias( strColumn, strDM_ALIAS, strFIELD_ALIAS );
		if( 0 > ids )	return -2001;		

		pDM = pMgr->FindDM(strDM_ALIAS);
		if( NULL == pDM )	return -1000;

		do
		{
			CString strData;
			ids = pDM->GetCellData(strFIELD_ALIAS, nCurFindIdx, strData);
			if( 0 > ids )	return -3001;

			strData.TrimLeft();strData.TrimRight();

			/// ã�Ҵٸ�
			/// ã�� �׸����� �ο츦 �����Ѵ�
			/// m_nPrevFindIdx�� �����Ѵ�.
			if( -1 != strData.Find(strFindWord) )
			{
				nFindCount++;
				pParentGrid->SetAt(nCurFindIdx, pParentGrid->m_nSelectIdx, pParentGrid->m_sSelectString);
				if( -1 == nFirstFindIdx )
					nFirstFindIdx = nCurFindIdx;
			}

			/// ã�� ���ߴٸ� ������ �˻��Ѵ�.
			nCurFindIdx ++;
			/// �׸��� ������ �̵�
//			pParentGrid->SendMessage( WM_VSCROLL, SB_LINEDOWN );
			
		}
		while( 0 <= nCurFindIdx && nGridRowCount > nCurFindIdx );		
	}

	/// ���� �˻� idx�� ������ �����ߴٸ�		
	CString strMessage;
	if( 0 == nFindCount )
	{
		strMessage.Format(_T("\"%s\"��(��) ã�� �� �����ϴ�."), strFindWord);
	}
	else
	{
		strMessage.Format(_T("'%d' ���� �����Ͽ����ϴ�."), nFindCount);

		/// �׸��� �̵�
		for( INT k=0;k<nFirstFindIdx;k++ )
			pParentGrid->SendMessage( WM_VSCROLL, SB_LINEDOWN );

	}
//	AfxMessageBox(strMessage, MB_ICONINFORMATION);

	return 0;
EFS_END	
return -1;
}

/// ����� �Է°��� �����´�.
INT	CESLDlg_FindWord::GetFindWord(CString &strFindWord)
{
EFS_BEGIN

	return GetDlgItemText(IDC_COM_eFINDWORD, strFindWord);

EFS_END	
return -1;
}

/// �˻� ����
/// 0  �Ʒ��� 
/// 1  ���� 
INT	CESLDlg_FindWord::GetFindDirection()
{
EFS_BEGIN

	CButton* pBtn = (CButton*)GetDlgItem(IDC_COM_rUPPER);
	if( NULL == pBtn )	return -1001;

	return pBtn->GetCheck();

EFS_END	
return -1;
}

/// ���� �˻� ��ġ
INT CESLDlg_FindWord::GetNextFindIdx(INT nFindDirection, INT nCurFindIdx)
{
EFS_BEGIN
	
	/// �Ʒ���
	if( 0 == nFindDirection )	nCurFindIdx++;
	/// ����
	else if( 1 == nFindDirection )	nCurFindIdx--;
	
	return nCurFindIdx;
EFS_END	
return -1;
}
INT CESLDlg_FindWord::GetNextFindIdx(INT nCurFindIdx)
{
EFS_BEGIN
	
	INT	nFindDirection;
	nFindDirection = GetFindDirection();
	if( 0 > nFindDirection )	return nFindDirection;

	return GetNextFindIdx(nFindDirection, nCurFindIdx);	

EFS_END	
return -1;
}
///@}}

///@{{ �׸��� ����
/// ���� ���õ� �׸����� �ο츦 �����´�.
INT	CESLDlg_FindWord::GetCurGridRowIdx()
{
EFS_BEGIN

	/// �׸��忡 ��µ� �ο찡 ���ٸ� 
	if( 0 == GetGridRowCount() )	return -100;

	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1001;

	/// ���õ� �ο찡 ���ٸ� 
	if(0 == pParentGrid->GetMouseRow()) return 0;

	INT nRowIdx = pParentGrid->GetRow()-1;
	return nRowIdx < 0 ? 0 : nRowIdx;

EFS_END	
return -1;
}

INT CESLDlg_FindWord::GetGridRowCount()
{
EFS_BEGIN

	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;

	return pParentGrid->m_nRealRowCount;

EFS_END	
return -1;
}

/// �Է� �ο� idx�� ��ȿ���� Ȯ�����ش�.
BOOL CESLDlg_FindWord::IsValidGridIdx(INT nGridIdx)
{
EFS_BEGIN

	if( 0 > nGridIdx )
		return FALSE;

	if( GetGridRowCount() <= nGridIdx )
		return FALSE;

	return TRUE;
EFS_END	
return -1;
}
///@}}
