// MngSpecialSort.cpp : implementation file
//

#include "stdafx.h"
#include "MngSpecialSort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngSpecialSort dialog


CMngSpecialSort::CMngSpecialSort(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngSpecialSort)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pGrid = NULL;
	m_pEditCtrl = NULL;

	m_strFilePath = _T("..\\CFG\\Marc_SpecialSort.cfg");
}

CMngSpecialSort::~CMngSpecialSort()
{
EFS_BEGIN

	if ( m_pCM != NULL) 
		m_pCM = NULL;

	if ( m_pGrid != NULL) 
		m_pGrid = NULL;

	if ( m_pEditCtrl != NULL) 
		m_pEditCtrl = NULL;


EFS_END
}

VOID CMngSpecialSort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngSpecialSort)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngSpecialSort, CDialog)
	//{{AFX_MSG_MAP(CMngSpecialSort)
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_EDIT, OnKillfocusEdit)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngSpecialSort message handlers

VOID CMngSpecialSort::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}


BOOL CMngSpecialSort::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

BOOL CMngSpecialSort::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_����_����ȼ�Ʈ")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_����_����ȼ�Ʈ"));
	if (m_pCM == NULL) 
	{
		AfxMessageBox(_T("CM Init Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pGrid = reinterpret_cast<CESL_Grid*>(m_pCM->FindControl(_T("GRID")));
	
	m_pEditCtrl = new CEdit();
	m_pEditCtrl->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE, CRect(0, 0, 0, 0), this, IDC_EDIT);
	m_pEditCtrl->ShowWindow(SW_HIDE);	

	//�׸��� ���� �ʱ�ȭ
	InitGrid();

	SetGridIndexNo();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMngSpecialSort::InitGrid()
{
EFS_BEGIN
	
	// ���Ͽ� ����� TAG�� ���� ��� �׸��忡 �����ֱ�
	FILE *file;
	file = _tfopen( m_strFilePath , _T("rb") ); 
	if ( file == NULL ) return;
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	FILE* fpUni = file;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
	
	CString sArrTag[100], sArrSubFieldCode[100];
	TCHAR cfileLine[256];
	CString sLine;
	INT nArrIdx = 0,  nIdx=0;;

	
	

	while ( _fgetts( cfileLine , 256, file) != NULL )
	{
		sLine.Format( _T("%s"), cfileLine );

		if ( sLine.GetAt(0) != '#' ) 
		{
			nIdx = 	sLine.Find(_T("="));
			if( nIdx >=  0 ) 
			{
				sArrTag[nArrIdx] = sLine.Mid(0, nIdx);
				sArrSubFieldCode[nArrIdx] = sLine.Mid(nIdx+1);
				sArrSubFieldCode[nArrIdx].TrimRight();
				
				nArrIdx++;
			}

		}


	}


	fclose(file);

	m_pGrid->SetRows(nArrIdx + 2);		 

	for ( INT idx = 0 ; idx < nArrIdx ; idx++ ) 
	{
		m_pGrid->SetTextMatrix( idx + 1 , 1 , sArrTag[idx] );
		m_pGrid->SetTextMatrix( idx + 1 , 2 , sArrSubFieldCode[idx] );
	}



EFS_END

}

VOID CMngSpecialSort::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	//pGrid->MoveWindow(0, 0, cx, cy);
	pGrid->MoveWindow(-2, 35, cx+2, cy);
EFS_END

	
}

BEGIN_EVENTSINK_MAP(CMngSpecialSort, CDialog)
    //{{AFX_EVENTSINK_MAP(CMngSpecialSort)
	ON_EVENT(CMngSpecialSort, IDC_GRID, -601 /* DblClick */, OnDblClickGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMngSpecialSort::OnDblClickGrid() 
{
EFS_BEGIN
	if (m_pGrid->GetMouseRow() == 0) return;
	ShowControl();
EFS_END		
}

VOID CMngSpecialSort::ShowControl()
{

EFS_BEGIN

	INT nCol = m_pGrid->GetCol();
	INT nRow = m_pGrid->GetRow();

	CString sOldGirdData = m_pGrid->GetTextMatrix(nRow, nCol);

	CString strText;
	strText.Format(_T("%s"), m_pGrid->GetTextMatrix(0, nCol));

	if ( strText != _T("TAG") && strText != _T("�ĺ���ȣ") ) return ;


	// �޺��� �ƴϸ�	
	// ����Ʈ �ڽ��� �����ٰ�
	ShowEditBox(m_pGrid);

	return ;

EFS_END
return ;

}

VOID CMngSpecialSort::ShowEditBox(CESL_Grid *pGrid)
{
EFS_BEGIN

	m_pEditCtrl->GetSafeHwnd();

	if (pGrid == NULL) return ;

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if (nRow < 1 || nCol < 1) return ;

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	// �ؽ�Ʈ ����
	m_pEditCtrl->SetWindowText(pGrid->GetTextMatrix(nRow, nCol));

	m_pEditCtrl->MoveWindow(rcCell);
	m_pEditCtrl->ShowWindow(SW_SHOW);
	m_pEditCtrl->SetFocus();
	m_pEditCtrl->BringWindowToTop();

	m_nSelectedGridRow = nRow;
	m_nSelectedGridCol = nCol;

	return ;

	EFS_END
	return ;
}

VOID CMngSpecialSort::GetCellRect(CESL_Grid *pGrid, CRect &rect)
{
	EFS_BEGIN

	if (pGrid == NULL) return ;

	CRect rcGrid;
	CRect rcClient;

	pGrid->GetWindowRect(rcGrid);

	GetClientRect(rcClient);

	ClientToScreen((POINT*)&(rcClient.left));

	rcGrid.top -= rcClient.top;
	rcGrid.left -= rcClient.left;

	rect.left	= rcGrid.left	+ (long)((float)pGrid->GetCellLeft() / 15.0f);
	rect.top	= rcGrid.top	+ (long)((float)pGrid->GetCellTop() / 15.0f);
	rect.right	= rect.left		+ (long)((float)pGrid->GetCellWidth() / 15.0f) - 1;
	rect.bottom = rect.top		+ (long)((float)pGrid->GetCellHeight() / 15.0f) - 1;

	return ;

	EFS_END
	return ;
}

VOID CMngSpecialSort::OnKillfocusEdit() 
{
EFS_BEGIN

	SetFocus();

	if (m_pEditCtrl == NULL) return ;
	
	if (!::IsWindowVisible(m_pEditCtrl->GetSafeHwnd())) return ;

	m_pEditCtrl->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl->GetWindowText(strData);

	ApplyDataChange(strData);

	return ;	
EFS_END
	
}


INT CMngSpecialSort::ApplyDataChange(LPCTSTR lpszData)
{
	EFS_BEGIN

	if (lpszData == NULL) return -1;
	if ( m_pGrid == NULL) return -1;

	// �׸��忡 ������ �����Ų��.
	m_pGrid->SetFocus();
	m_pGrid->SetTextMatrix(m_nSelectedGridRow, m_nSelectedGridCol, lpszData);

	if ( m_pGrid->GetRows()-1 == m_nSelectedGridRow  && m_pGrid->GetTextMatrix( m_nSelectedGridRow , 1 ) != _T("") ) 
		m_pGrid->SetRows( m_nSelectedGridRow + 2 );


	SetGridIndexNo();

	return 0;

	EFS_END
	return -1;
}

VOID CMngSpecialSort::SetGridIndexNo()
{
EFS_BEGIN

	// �׸��忡 Index ��ȣ ����
	CString strGridIdx;
	for ( INT idx = 1; idx < m_pGrid->GetRows()-1 ; idx++ )
	{
		strGridIdx.Format( _T("%d"), idx ); 
		m_pGrid->SetTextMatrix( idx, 0, strGridIdx );
	}


EFS_END
}

VOID CMngSpecialSort::SaveToFile()
{
	INT nRowCount = m_pGrid->GetRows();

	//CString sArrTag[100], sArrSubFieldCode[100];

	CString strTmp=_T("");
	for ( INT idx = 1; idx < nRowCount-1 ; idx++ )
	{
		//TAG //////////////////
		strTmp = m_pGrid->GetTextMatrix( idx  , 1);
		if( strTmp < _T("001") || strTmp > _T("999") ) 
		{
			AfxMessageBox(_T("TAG�� �߸� �ԷµǾ����ϴ�. 001~999 ������ ���� �Է��ϼ���."));
			m_pGrid->SetFocus();
			return;
		}
		m_pGrid->SetTextMatrix( idx , 1, strTmp);

		// �ĺ���ȣ ///////////////////
		strTmp = m_pGrid->GetTextMatrix( idx  , 2);
		// �ĺ���ȣ üũ
		strTmp = CheckSubFieldCode( strTmp );
		if ( strTmp == _T("") ) 
		{
			AfxMessageBox(m_pGrid->GetTextMatrix( idx  , 1)+ _T("TAG�� �ĺ���ȣ�� �߸� �ԷµǾ����ϴ�."));
			m_pGrid->SetFocus();
			return;
		}
		m_pGrid->SetTextMatrix( idx  , 2, strTmp);

	}

	writeToFile();


	AfxMessageBox(_T("����Ǿ����ϴ�."));


}


CString CMngSpecialSort::CheckSubFieldCode( CString sSubFieldCode )
{
	CString strTmpResult = _T("");
	CString strTmp = sSubFieldCode;

	strTmp.TrimLeft();
	strTmp.TrimRight();

	if ( strTmp == _T("") ) return _T(""); 

	INT idx = strTmp.Find(_T("$"));

	while( idx >= 0 )
	{
		CString ss = strTmp.Mid(idx+1, 1 );

		// check
		if( !(ss>=_T("a") && ss<=_T("z")) 
			&& !(ss>=_T("0") && ss <=_T("9") ) ) return _T("");

		

		strTmp = strTmp.Mid(idx + 2);

		idx = strTmp.Find(_T("$"));


		if ( idx < 0 ) strTmpResult += _T("$")+ss;
		else strTmpResult += _T("$")+ss+_T(",");

	}
	

	return strTmpResult;

}

BOOL CMngSpecialSort::writeToFile()
{
	EFS_BEGIN

	// ���Ϸ� ����
	FILE *file;
	file = _tfopen( m_strFilePath , _T("w") ); 
	if ( file == NULL )
	{
		AfxMessageBox(_T("File Open Ȯ��"));
		return FALSE;
	}
	fseek(file, 0, SEEK_END);		
	if( 0 == ftell(file) )
	{
		/*UNCHANGE*/fputc(0xFF, file);
		/*UNCHANGE*/fputc(0xFE, file);
	}

	CString strGridData = _T("");
	INT nRowCount = m_pGrid->GetRows();
	for ( INT nGridRowIdx = 1 ; nGridRowIdx < nRowCount-1 ; nGridRowIdx++ )
	{
		strGridData += m_pGrid->GetTextMatrix( nGridRowIdx  , 1);
		strGridData += _T("=");
		strGridData += m_pGrid->GetTextMatrix( nGridRowIdx  , 2);
		strGridData += _T("\r\n");

		_ftprintf( file, strGridData );
		strGridData = _T("");
	}

	_ftprintf( file, strGridData );
	fclose(file);

	return TRUE;

EFS_END
return FALSE;
}

BOOL CMngSpecialSort::PreTranslateMessage(MSG* pMsg) 
{
	
	//DELETE Ű �Է½� �ش� �׸��� ROW ����
	INT nGridIdx = 0;
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE ) 
	{
		if( GetFocus() == m_pGrid) {
		nGridIdx = m_pGrid->GetIdx();
		if ( nGridIdx + 2 != m_pGrid->GetRows() ) 	
			m_pGrid->RemoveAt( nGridIdx ) ;

		SetGridIndexNo();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CMngSpecialSort::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
