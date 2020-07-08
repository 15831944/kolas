// BO_ACQ_EnvironmentMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_EnvironmentMgr.h"
#include "EnvrionComboBox_ThreeState.h"
#include "EnvironComboBox_TwoState.h"
#include "EnvironComboBox_MultiState.h" // 2004.12.01

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_EnvironmentMgr dialog


CBO_ACQ_EnvironmentMgr::CBO_ACQ_EnvironmentMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_EnvironmentMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_EnvironmentMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboBox = NULL;
	
	m_pEditCtrl[0] = NULL;
	m_pEditCtrl[1] = NULL;
}

CBO_ACQ_EnvironmentMgr::~CBO_ACQ_EnvironmentMgr()
{
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}

	if(m_pEditCtrl[0])
	{
		delete m_pEditCtrl[0];		
		m_pEditCtrl[0] = NULL;
	}

	if(m_pEditCtrl[1])
	{
		delete m_pEditCtrl[1];		
		m_pEditCtrl[1] = NULL;
	}
}

VOID CBO_ACQ_EnvironmentMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_EnvironmentMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_EnvironmentMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_EnvironmentMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(100, OnKillfocusedt)
	ON_EN_KILLFOCUS(101, OnKillfocusedt2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_EnvironmentMgr message handlers

VOID CBO_ACQ_EnvironmentMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	DestroyWindow();
}

BOOL CBO_ACQ_EnvironmentMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("K2UPGRADE_�����������")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_�����������"));
	m_pDM = FindDM(_T("DM_�����������"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	
	m_pEditCtrl[0] = new CEdit();
	m_pEditCtrl[0]->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, 100);
	m_pEditCtrl[0]->ShowWindow(SW_HIDE);

	m_pEditCtrl[1] = new CEdit();
	m_pEditCtrl[1]->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_NUMBER, CRect(0, 0, 0, 0), this, 101);
	m_pEditCtrl[1]->ShowWindow(SW_HIDE);

	SetTimer(0, 500, NULL);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

VOID CBO_ACQ_EnvironmentMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridENVIRONMENT_MGR);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);
}

BOOL CBO_ACQ_EnvironmentMgr::Init()
{
	CString strQuery;

	strQuery.Format( _T("GROUP_2='����' AND SYS_SETUP_YN='N' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE );

	INT ids = m_pDM->RefreshDataManager(strQuery);
	m_pDM->SORT(_T("�����׸�"), FALSE);
	if(ids < 0) return FALSE;
	DisplayGrid(m_pCM->CM_ALIAS, _T("MainGrid"));

	return TRUE;
}

VOID CBO_ACQ_EnvironmentMgr::SaveManageData()
{
	INT cnt = m_pDM->GetRowCount();
	m_pDM->StartFrame();
	CString strKey;
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	for(INT i=0 ; i<cnt ; i++)
	{
		strKey = m_pDM->GetCellData(_T("�����׸�KEY"), i);
		m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, i, m_pDM, _T("REC_KEY"), _T("NUMERIC"), strKey);
	}
	INT ids = m_pDM->SendFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�������� ���� ������ �� �����ϴ�."));
		return ;
	}
	ids = m_pDM->EndFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�������� ���� ������ �� �����ϴ�."));
		return ;
	}

	CString strValueName;
	CString strValue;
	int nCount = m_pDM->GetRowCount();	

	BOOL strValueA = FALSE, strValueB = FALSE;

	for (int idx=0; idx<nCount; idx++)
	{
		m_pDM->GetCellData(_T("�����׸�"), idx, strValueName);
		if (strValueName.CompareNoCase(_T("����ڷ��û���ѻ�뿩��")) == 0)
		{
			m_pDM->GetCellData(_T("������"), idx, strValue);			
			strValueA = (strValue.CompareNoCase(_T("Y")) == 0);			
		}		
		else if (strValueName.CompareNoCase(_T("����ڷ��û�ִ������ѻ�뿩��")) == 0)
		{
			m_pDM->GetCellData(_T("������"), idx, strValue);
			strValueB = (strValue.CompareNoCase(_T("Y")) == 0);
		}
		else if (strValueName.CompareNoCase(_T("����ڷ�ȸ���������ѻ�뿩��")) == 0)
		{
			m_pDM->GetCellData(_T("������"), idx, strValue);
			m_pInfo->m_bRestictionToApply = (strValue.CompareNoCase(_T("Y")) == 0);
		}
	}

	if ( strValueA && strValueB)
	{
		ESLAfxMessageBox(_T("�������� ���� ������ �� �����ϴ�.")
			_T("\n����ڷ��û�Ǽ����� ����� �� ������ ����� �� �ֽ��ϴ�.") );
		return ;
	}
	
	m_pInfo->m_bFurnishWishLimitYN = strValueA;
	m_pInfo->m_bFurnishLimitByTheWeek = strValueB;
	
	if (m_pInfo->m_bFurnishWishLimitYN)
	{	
		for ( idx=0; idx<nCount; idx++)
		{
			m_pDM->GetCellData(_T("�����׸�"), idx, strValueName);
			if (strValueName.CompareNoCase(_T("����ڷ��û���ѱǼ�")) == 0)
			{
				m_pDM->GetCellData(_T("������"), idx, strValue);
				m_pInfo->m_nFurnishWishMaxCnt = _ttoi(strValue);
			}	
		}
	}
	else if (m_pInfo->m_bFurnishLimitByTheWeek)
	{
		for ( idx=0; idx<nCount; idx++)
		{
			m_pDM->GetCellData(_T("�����׸�"), idx, strValueName);
			if (strValueName.CompareNoCase(_T("����ڷ��û�ִ������ѱǼ�")) == 0)
			{
				m_pDM->GetCellData(_T("������"), idx, strValue);
				m_pInfo->m_nFurnishWishMaxCnt = _ttoi(strValue);
			}	
		}
	}

	ESLAfxMessageBox(_T("������������ �����Ͽ����ϴ�."));
}

VOID CBO_ACQ_EnvironmentMgr::ChangeManageData()
{

}

INT CBO_ACQ_EnvironmentMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strData, strTmpData;
	if(strDMFieldAlias == _T("������"))
	{
		strTmpData = m_pDM->GetCellData(_T("�����׸�"), nRow);
		if(strTmpData == _T("������ȭ�麹������˻�����"))
		{
			strData = m_pDM->GetCellData(_T("������"), nRow);
			if(strData == _T("Y")) str = _T("���Ǽ�����");
			else if(strData == _T("N")) str = _T("�������");
		}
		else if( strTmpData == _T("���������������")		|| 
				strTmpData == _T("���԰��ݴ�������")		|| 
				strTmpData == _T("���԰���ǥ�����ļ���")	|| 
				strTmpData == _T("�������_����ǥ�ü���")	||
				strTmpData == _T("�ּҷ��̺�����")			)
		{
			str = m_pDM->GetCellData(_T("������"), nRow);
		}
		else if(strTmpData == _T("����ڷ��û���ѱǼ�"))
		{
			str = CodeToDesc(strTmpData, str);
		}
		else if(strTmpData == _T("����ڷ��û�ִ������ѱǼ�"))
		{
			str = CodeToDesc(strTmpData, str);
		}
		else
		{
			strData = m_pDM->GetCellData(_T("������"), nRow);
			if(strData == _T("Y")) str = _T("�����");
			else if(strData == _T("O")) str = _T("���û���");
			else 
			{
				strTmpData = m_pDM->GetCellData(_T("�����׸�"), nRow);
				if(strTmpData.Find(_T("���࿩��")) >= 0)
					str = _T("���û���");
				else
					str = _T("������");
			}
		}
	}
	
	return 0;
}

VOID CBO_ACQ_EnvironmentMgr::DisplayGrid(CString cmAlias, CString gridAlias)
{
	CESL_Grid * pGrid = (CESL_Grid*)FindControl(cmAlias, gridAlias);
	if(!pGrid) return;
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	pGrid->Display();
}

BOOL CBO_ACQ_EnvironmentMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_EnvironmentMgr::CreateComboBoxInGridSell()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	INT nRow = m_pGrid->GetRow();
	CString strData = m_pDM->GetCellData(_T("������"), nRow - 1);
	CString strElement = m_pDM->GetCellData(_T("�����׸�"), nRow - 1);

	if( m_pComboBox )
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}

	if (strElement.CompareNoCase(_T("����ڷ��û���ѱǼ�")) == 0)
	{
		if(m_pInfo->m_bFurnishWishLimitYN)
		{
			ShowEditBox(m_pGrid, m_pEditCtrl[0]);
		}		
		return;
	}
	else if (strElement.CompareNoCase(_T("����ڷ��û�ִ������ѱǼ�")) == 0)
	{
		if(m_pInfo->m_bFurnishLimitByTheWeek)
		{
			ShowEditBox(m_pGrid, m_pEditCtrl[1]);		
		}
		return;
	}

	CStringArray ArrCmbData;	ArrCmbData.RemoveAll();
	if(strElement == _T("����������࿩��") || strElement == _T("�ߺ�������࿩��") || strElement == _T("����ó�����࿩��"))
	{
		m_pComboBox = new CEnvrionComboBox_ThreeState;
		ArrCmbData.Add(_T("�����"));		ArrCmbData.Add(_T("���û���"));
	}
	else if(strElement == _T("������ȭ�麹������˻�����"))
	{
		m_pComboBox = new CEnvironComboBox_TwoState;
		ArrCmbData.Add(_T("���Ǽ�����"));		ArrCmbData.Add(_T("�������"));
	}
	else if( strElement == _T("���������������") )
	{
		m_pComboBox = new CEnvironComboBox_MultiState;
		INT nValue = 3; CString strValue;
		for(INT i=0; i < nValue; i++)
		{
			strValue.Format(_T("%d"), i);
			ArrCmbData.Add(strValue);
		}
	}
	else if( strElement == _T("���԰��ݴ�������")		|| 
			strElement == _T("���԰���ǥ�����ļ���")	|| 
			strElement == _T("�������_����ǥ�ü���")	||
			strElement == _T("�ּҷ��̺�����")			)
	{
		TCHAR* cVal;
		CString strValue;
		m_pComboBox = new CEnvironComboBox_MultiState;

		for( INT i=0; i<m_pDM->GetRowCount(); i++ )
		{
			if( strElement == m_pDM->GetCellData(_T("�����׸�"), i) )	break;
		}
		
		if( i<m_pDM->GetRowCount() )	strValue = m_pDM->GetCellData(_T("������"), i);
		else							strValue = _T("");

		cVal = _tcstok(strValue.GetBuffer(0), _T(","));		
		while(cVal)
		{
			ArrCmbData.Add(cVal);
			cVal = _tcstok(NULL, _T(","));
		}
	}
	else 
	{
		m_pComboBox = new CEnvironComboBox_TwoState;
		ArrCmbData.Add(_T("�����"));		ArrCmbData.Add(_T("������"));
	}

	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), m_pGrid, 6450);
	m_pComboBox->Init(&ArrCmbData);
	m_pComboBox->SetIndex(nRow-1);
	m_pComboBox->SetDataMgr(m_pDM);
	m_pComboBox->SetParent(m_pGrid);
	m_pComboBox->SetCurSelData(strData);

	m_pComboBox->SetHorizontalExtent(10);
	CFont * pFont = this->GetFont();
	m_pComboBox->SetFont(pFont);
	
	CRect rect,gridRect;
	RECT parentRect;
	m_pGrid->GetWindowRect(&gridRect);
	this->GetClientRect(&parentRect);

	::ClientToScreen(this->m_hWnd,(POINT*)&(parentRect.left));
	

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;
	rect.left = (long)((float)m_pGrid->GetCellLeft()/15.0f) - gridRect.left;
	rect.top = (long)((float)m_pGrid->GetCellTop()/15.0f) - gridRect.top;
	rect.right = rect.left + (long)((float)m_pGrid->GetCellWidth()/15.0f);
	rect.bottom = rect.top + (long)((float)m_pGrid->GetCellHeight()/15.0f);

	m_pComboBox->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());

	m_pComboBox->ShowWindow(TRUE);
	m_pComboBox->SetFocus();
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_EnvironmentMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_EnvironmentMgr)
	ON_EVENT(CBO_ACQ_EnvironmentMgr, IDC_gridENVIRONMENT_MGR, -601 /* DblClick */, OnDblClickgridENVIRONMENTMGR, VTS_NONE)
	ON_EVENT(CBO_ACQ_EnvironmentMgr, IDC_gridENVIRONMENT_MGR, -600 /* Click */, OnClickgridENVIRONMENTMGR, VTS_NONE)	
	ON_EVENT(CBO_ACQ_EnvironmentMgr, IDC_gridENVIRONMENT_MGR, 72 /* SelChange */, OnChaneCombo, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_EnvironmentMgr::OnDblClickgridENVIRONMENTMGR() 
{
}

VOID CBO_ACQ_EnvironmentMgr::OnChaneCombo(){
	
	CString strAlias = m_pDM->GetCellData(_T("�����׸�"), m_pGrid->GetRow() - 1 );
	CString strResult = _T("");
	int idx = -1,col_idx= -1;
	if (strAlias == _T("����ڷ��û�ִ������ѻ�뿩��"))
	{
		strResult = m_pDM->GetCellData(_T("������"), m_pGrid->GetRow() - 1 );
		if (strResult == _T("Y"))
		{
			for (int i = 0 ; i < m_pDM->GetRowCount() ; i++)
			{
				if ( m_pDM->GetCellData(_T("�����׸�") , i) == _T("����ڷ��û���ѻ�뿩��") ) idx = i;
			}
			m_pDM->SetCellData(_T("������"), _T("N"), idx);
			
			for (int j = 0 ; j <m_pGrid->GetColCount() +1  ; j++)
			{
				if (m_pGrid->GetTextMatrix(0,j) == _T("������")) col_idx = j;
			}
			
			m_pGrid->SetTextMatrix(idx+1,col_idx,_T("�ÿ����"));
		}
	}
	else if (strAlias == _T("����ڷ��û���ѻ�뿩��"))
	{
		strResult = m_pDM->GetCellData(_T("������"), m_pGrid->GetRow() - 1 );
		if (strResult == _T("Y"))
		{
			for (int i = 0 ; i < m_pDM->GetRowCount() ; i++)
			{
				if ( m_pDM->GetCellData(_T("�����׸�") , i) == _T("����ڷ��û�ִ������ѻ�뿩��") ) idx = i;
			}
			m_pDM->SetCellData(_T("������"), _T("N"), idx);
			
			for (int j = 0 ; j <m_pGrid->GetColCount() +1  ; j++)
			{
				if (m_pGrid->GetTextMatrix(0,j) == _T("������")) col_idx = j;
			}
			
			m_pGrid->SetTextMatrix(idx+1,col_idx,_T("�ÿ����"));
		}
	}		
	
}
VOID CBO_ACQ_EnvironmentMgr::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0) {
		KillTimer(0);
		if(!Init())
		{
			ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		}		
	}
	CDialog::OnTimer(nIDEvent);
}

VOID CBO_ACQ_EnvironmentMgr::OnClickgridENVIRONMENTMGR() 
{
	if( m_pGrid->GetMouseRow() == 0 )	return ;
	INT col = m_pGrid->GetCol();
	INT row = m_pGrid->GetRow();
	if( col == 5 )	
	{
		CreateComboBoxInGridSell();
	}

}

HBRUSH CBO_ACQ_EnvironmentMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

INT CBO_ACQ_EnvironmentMgr::ShowEditBox(CESL_Grid *pGrid, CEdit* pEditCtrl)
{
EFS_BEGIN

	pEditCtrl->GetSafeHwnd();
	if ( NULL == pGrid ) 
	{
		return -1;
	}

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if ( 1 > nRow || 1 > nCol ) 
	{
		return -1;
	}

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	CString sValue = pGrid->GetTextMatrix(nRow, nCol);
	sValue.Replace( _T("��"), _T("") );
	pEditCtrl->SetWindowText(sValue);

	pEditCtrl->MoveWindow(rcCell);
	pEditCtrl->ShowWindow(SW_SHOW);
	pEditCtrl->SetFocus();
	pEditCtrl->BringWindowToTop();
	
	return 0;

EFS_END
return -1;
}

INT CBO_ACQ_EnvironmentMgr::GetCellRect(CESL_Grid *pGrid, CRect &rect)
{
EFS_BEGIN

	if (pGrid == NULL) return -1;

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

	return 0;

EFS_END
return -1;
}

INT CBO_ACQ_EnvironmentMgr::ApplyDataChange(LPCTSTR lpszData)
{
EFS_BEGIN

	if (lpszData == NULL) return -1;
	if (m_pDM == NULL || m_pGrid == NULL) return -1;

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();

	m_pGrid->SetFocus();
	m_pGrid->SetTextMatrix(nRow, nCol, lpszData);

	m_pDM->SetCellData( _T("������"), lpszData, nRow - 1 );
	m_pGrid->Display();

	return 0;

EFS_END
return -1;
}

void CBO_ACQ_EnvironmentMgr::OnKillfocusedt()
{
	SetFocus();
	if (NULL == m_pEditCtrl[0]) return ;
	if (!::IsWindowVisible(m_pEditCtrl[0]->GetSafeHwnd())) return ;

	m_pEditCtrl[0]->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl[0]->GetWindowText(strData);

	ApplyDataChange(strData);

	return;
}
void CBO_ACQ_EnvironmentMgr::OnKillfocusedt2()
{
	SetFocus();
	if (NULL == m_pEditCtrl[1]) return ;
	if (!::IsWindowVisible(m_pEditCtrl[1]->GetSafeHwnd())) return ;
	
	m_pEditCtrl[1]->ShowWindow(SW_HIDE);
	
	CString strData;
	m_pEditCtrl[1]->GetWindowText(strData);
	
	ApplyDataChange(strData);
	
	return;
}

CString CBO_ACQ_EnvironmentMgr::CodeToDesc(CString sAbility, CString sCode)
{
	CString sDesc;
	if( sAbility == _T("����ڷ��û���ѱǼ�") )
	{
		sDesc = sCode + _T("��");
	}
	else if( sAbility == _T("����ڷ��û�ִ������ѱǼ�") )
	{
		sDesc = sCode + _T("��");
	}
	
	return sDesc;
}
