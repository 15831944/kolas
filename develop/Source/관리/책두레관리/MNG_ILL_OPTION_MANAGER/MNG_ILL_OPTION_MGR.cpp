// MNG_ILL_OPTION_MGR.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "MNG_ILL_OPTION_MGR.h"
#include "MNG_ILL_PERMISSION_LIST_DLG.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CANCEL_DIALOG(x,y)\
	{\
		SHOW_MESSAGE2(x,y);\
		CDialog::OnCancel();\
	}\

/////////////////////////////////////////////////////////////////////////////
// CMNG_ILL_OPTION_MGR dialog


CMNG_ILL_OPTION_MGR::CMNG_ILL_OPTION_MGR(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_ILL_OPTION_MGR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_ILL_OPTION_MGR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCmb = NULL;
	m_pEdit = NULL;
	m_pGrid = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
}
CMNG_ILL_OPTION_MGR::~CMNG_ILL_OPTION_MGR()
{	
	FreeComboBox();
	FreeEditBox();
}


void CMNG_ILL_OPTION_MGR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_ILL_OPTION_MGR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_ILL_OPTION_MGR, CDialog)
	//{{AFX_MSG_MAP(CMNG_ILL_OPTION_MGR)
	ON_WM_SIZE()
 	ON_MESSAGE(UPDATE_OPTION_DM, UpdateDM)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_ILL_OPTION_MGR message handlers

void CMNG_ILL_OPTION_MGR::CreateComboBoxInGridSell()
{
	int ids = 0;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::CreateComboBoxInGridSell()");

	if(NULL == m_pGrid) return;
	if(NULL == m_pDM) return;

	FreeComboBox();

	///////////////////////////////////////////////////////////////////////////
	// ���⼭���� �޺��ڽ� ũ�� ����
	CRect rGridClient, rGridWindow;
		
	m_pGrid->GetWindowRect(&rGridWindow);
	ScreenToClient(&rGridWindow);
	m_pGrid->GetClientRect(&rGridClient);

	const WIDTH_OFFSET = (rGridWindow.Width() - rGridClient.Width())/2;
	const HEIGHT_OFFSET = (rGridWindow.Height() - rGridClient.Height())/2;
	
	CRect rCmb;
	rCmb.left = (long)((float)m_pGrid->GetCellLeft()/15.0f) - WIDTH_OFFSET-2;
	rCmb.top = (long)((float)m_pGrid->GetCellTop()/15.0f) - HEIGHT_OFFSET-2;
	rCmb.right = rCmb.left + (long)((float)m_pGrid->GetCellWidth()/15.0f)+2;
	rCmb.bottom = rCmb.top + (long)((float)m_pGrid->GetCellHeight()/15.0f);

	// ������� �޺��ڽ� ũ�� ����
	///////////////////////////////////////////////////////////////////////////

	UINT nFlag = WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_AUTOHSCROLL;

	m_pCmb = new CCmb_GridItem;	
	m_pCmb->m_pGrid = m_pGrid;
	m_pCmb->m_pDM = m_pDM;

	ids = m_pCmb->Create(nFlag, rCmb, m_pGrid, IDC_cmbILL_OPTION);
 	if(FALSE == ids) ERROR_MSG_RETURN_VOID2(-10, strFuncName);

 	m_pCmb->SetHorizontalExtent(10);
 	CFont * pFont = this->GetFont();
 	m_pCmb->SetFont(pFont);

	int nIdx = (int)m_pGrid->GetRow()-1;

	/////////////////////////////////////////////////////////////////////////
	// ���⼭���� �޺��ڽ� ������ ����
	CString strValueType;
	ids = m_pDM->GetCellData(_T("������"), nIdx, strValueType);
 	if(0 > ids) ERROR_MSG_RETURN_VOID2(-11, strFuncName);

	CString strValue;
	ids = m_pDM->GetCellData(_T("������"), nIdx, strValue);
 	if(0 > ids) ERROR_MSG_RETURN_VOID2(-12, strFuncName);

	
	if(_T("Y/N") == strValueType)
	{
		m_pCmb->AddString(_T("�����"));
		m_pCmb->AddString(_T("������"));

		int nIdx = (_T("Y") == strValue || _T("�����") == strValue) ? 0 : 1;
		m_pCmb->SetCurSel(nIdx);
	}

	// ������� �޺��ڽ� ������ ����
	/////////////////////////////////////////////////////////////////////////
	m_pCmb->ShowWindow(SW_SHOW);
	m_pCmb->SetFocus();

}

int CMNG_ILL_OPTION_MGR::Refresh()
{
	int ids = 0;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::Refresh()");

	ids = m_pDM->RefreshDataManager(_T(""));
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	m_pGrid->SetRedraw(FALSE);
	ids = m_pGrid->Display();
	ShowValue_For_Grid();
	m_pGrid->SetRedraw(TRUE);

	return 0;
}

BOOL CMNG_ILL_OPTION_MGR::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	INT ids = 0;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::OnInitDialog()");
	
	ids = InitESL_Mgr(_T("SM_ILL_OPTION_MGR"));
	if(0 > ids)
	{		
		CANCEL_DIALOG(-1000, _T("ERROR : InitESL_Mgr(\"SM_ILL_OPTION_MGR\")"));
	}
	m_pCM = FindCM(_T("CM_ILL_OPTION_MGR"));

	m_pGrid = (CESL_Grid*)m_pCM->FindControl( _T("OPTION_GRID"));
	if(NULL == m_pGrid) 
	{
		CANCEL_DIALOG(-1010, strFuncName);
	}

	m_pDM = FindDM(_T("DM_ILL_OPTION_MGR"));
	if(NULL == m_pDM) 
	{
		CANCEL_DIALOG(-1020, strFuncName);
	}

	m_pDM->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	ids = Refresh();
//	if(0 > ids) CANCEL_DIALOG(-3, strFuncName);
	
	// 2009.07.07 ADD BY PWR : å�η�����(å�η� ����Ҷ� ���� ����(I:����, D:�л�))
	CString strValue;
	ids = GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η���뿩��"), strValue);
	if(0 > ids) 
	{
		CANCEL_DIALOG(-1030, strFuncName);
	}
	if(strValue.IsEmpty()) 
	{
		m_sBookCooperativeMode = _T("I");
	}
	else
	{
		if(strValue == _T("Y"))
		{ // �����
			ids = GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η�����"), strValue);
			if(0 > ids) 
			{
				CANCEL_DIALOG(-1040, strFuncName);
			}
			if(strValue.IsEmpty()) 
			{
				m_sBookCooperativeMode = _T("I");
			}
			else
			{
				m_sBookCooperativeMode = strValue;
			}
		}
		else
		{ // ������
			m_sBookCooperativeMode = _T("I");
		}
	}
	// �л�ý��� �������� ���� HIDE
	if(_T("D") == m_sBookCooperativeMode)
	{
		CString strButtonAlias = _T("��������");
		pMain->SendMessage( USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CMNG_ILL_OPTION_MGR, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_ILL_OPTION_MGR)
	ON_EVENT(CMNG_ILL_OPTION_MGR, IDC_gridMNG_ILL_OPTION_MGR, -600 /* Click */, OnClickgridMNGILLOPTIONMGR, VTS_NONE)
	ON_EVENT(CMNG_ILL_OPTION_MGR, IDC_gridMNG_ILL_OPTION_MGR, -601 /* DblClick */, OnDblClickMNGILLOPTIONMGR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMNG_ILL_OPTION_MGR::OnClickgridMNGILLOPTIONMGR() 
{
	// TODO: Add your control notification handler code here
	FreeComboBox();
	FreeEditBox();

	int nCol = (int)m_pGrid->GetCol();
	if(4 == nCol) 
	{
		CString strValueType;
		m_pDM->GetCellData(_T("������"), (int)m_pGrid->GetRow()-1, strValueType);
		if(_T("Y/N") == strValueType) CreateComboBoxInGridSell();
		else if(_T("�ڷ���ڵ�,") == strValueType || _T("��ϱ���,") == strValueType || _T("��ġ��ȣ,") == strValueType)
		{
			ViewPermissionListDlg();
		}
		else if(_T("����") == strValueType)
		{
			CreateEditBoxInGridSell();
		}
	}
	
}

void CMNG_ILL_OPTION_MGR::OnOptionApply() 
{
	// TODO: Add your control notification handler code here
	int ids = 0;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::OnOptionApply()");

	BOOL bIsModify = FALSE;
	m_pDM->StartFrame();

	const int ROW_COUNT = m_pDM->GetRowCount();
	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{
		CString strKey;
		CString strValue;

		ids = m_pDM->GetCellData(_T("���濩��"), nRow, strValue);
 		if(0 > ids) ERROR_MSG_RETURN_VOID2(-2, strFuncName);

		if(_T("") != strValue)
		{
			bIsModify = TRUE;

			ids = m_pDM->GetCellData(_T("REC_KEY"), nRow, strKey);
 			if(0 > ids) ERROR_MSG_RETURN_VOID2(-3, strFuncName);

			ids = m_pDM->GetCellData(_T("������"), nRow, strValue);
 			if(0 > ids) ERROR_MSG_RETURN_VOID2(-4, strFuncName);

			if(_T("�����") == strValue) strValue = _T("Y");
			else if(_T("������") == strValue) strValue = _T("N");

			m_pDM->InitDBFieldData();
			m_pDM->AddDBFieldData(_T("VALUE_1"), _T("STRING"), strValue);
			m_pDM->MakeUpdateFrame(_T("ILL_MANAGE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strKey);
//DEL 			m_pDM->MakeUpdateFrame(_T("ILL.ILL_MANAGE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strKey);
		}
	}
	
	if(TRUE == bIsModify)
	{
		ids = m_pDM->SendFrame();
 		if(0 > ids) ERROR_MSG_RETURN_VOID2(-5, strFuncName);	
	}

	m_pDM->EndFrame();

	AfxMessageBox(_T("����Ǿ����ϴ�."));
}

BOOL CMNG_ILL_OPTION_MGR::Create(CWnd *pParent)
{
	return CDialog::Create(IDD, pParent);
}

void CMNG_ILL_OPTION_MGR::OnSize(UINT nType, int cx, int cy) 
{
	//2014-05-01 UPDATE BY BJY : 2014�� å�η� ��ɰ��� 1��
	//�ɼ� �߰��� ���� �׸��� ũ�� ����
	CDialog::OnSize(nType, cx, cy);
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMNG_ILL_OPTION_MGR);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 40, cx-10, cy-45);		
	//pGrid->MoveWindow(-2, 0, cx, 220);
	pGrid->MoveWindow(-2, 0, cx, 260);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_STATIC1);
//  	pEdt->GetWindowRect(&rect);
// 	pEdt->MoveWindow(-2, 225, cx+2, 60);
//	pEdt->MoveWindow(0, 225, cx, 60);
	pEdt->MoveWindow(0, 265, cx, 60);
	pEdt->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_STATIC2);
// 	pEdt->GetWindowRect(&rect);
// 	pEdt->MoveWindow(-2, 290, cx+2, 50);
//	pEdt->MoveWindow(0, 290, cx, 50);
	pEdt->MoveWindow(0, 330, cx, 50);
	pEdt->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_STATIC3);
// 	pEdt->GetWindowRect(&rect);
// 	pEdt->MoveWindow(-2, 345, cx+2, 130);
//	pEdt->MoveWindow(0, 345, cx, 130);
	pEdt->MoveWindow(0, 385, cx, 130);
	pEdt->ShowWindow(SW_SHOW);


	
	// TODO: Add your message handler code here
	
}

void CMNG_ILL_OPTION_MGR::CreateEditBoxInGridSell()
{
	int ids = 0;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::CreateEditBoxInGridSell()");

	if(NULL == m_pGrid) return;
	if(NULL == m_pDM) return;

	FreeEditBox();

	///////////////////////////////////////////////////////////////////////////
	// ���⼭���� �޺��ڽ� ũ�� ����
	CRect rGridClient, rGridWindow;
		
	m_pGrid->GetWindowRect(&rGridWindow);
	ScreenToClient(&rGridWindow);
	m_pGrid->GetClientRect(&rGridClient);

	const WIDTH_OFFSET = (rGridWindow.Width() - rGridClient.Width())/2;
	const HEIGHT_OFFSET = (rGridWindow.Height() - rGridClient.Height())/2;
	
	CRect rCmb;
	rCmb.left = (long)((float)m_pGrid->GetCellLeft()/15.0f) - WIDTH_OFFSET;
	rCmb.top = (long)((float)m_pGrid->GetCellTop()/15.0f) - HEIGHT_OFFSET;
	rCmb.right = rCmb.left + (long)((float)m_pGrid->GetCellWidth()/15.0f)-2;
	rCmb.bottom = rCmb.top + (long)((float)m_pGrid->GetCellHeight()/15.0f)-2;

	// ������� �޺��ڽ� ũ�� ����
	///////////////////////////////////////////////////////////////////////////

	UINT nFlag = WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER;
 
	m_pEdit = new CEdit_GridItem;	
	m_pEdit->m_pGrid = m_pGrid;
	m_pEdit->m_pDM = m_pDM;

	ids = m_pEdit->Create(nFlag, rCmb, m_pGrid, IDC_cmbILL_OPTION);
 	if(FALSE == ids) ERROR_MSG_RETURN_VOID2(-10, strFuncName);

 	CFont * pFont = this->GetFont();
 	m_pEdit->SetFont(pFont);

	int nIdx = (int)m_pGrid->GetRow()-1;

	/////////////////////////////////////////////////////////////////////////
	// ���⼭���� ����Ʈ ������ ����

	CString strValue;
	ids = m_pDM->GetCellData(_T("������"), nIdx, strValue);
 	if(0 > ids) ERROR_MSG_RETURN_VOID2(-12, strFuncName);

	m_pEdit->SetWindowText(strValue);
	m_pEdit->LimitText(3);	// 3�� ���ϸ� ����
	m_pEdit->SetSel(0, -1);
	// ������� ����Ʈ ������ ����
	/////////////////////////////////////////////////////////////////////////

	m_pEdit->ShowWindow(SW_SHOW);
	m_pEdit->SetFocus();
}

int CMNG_ILL_OPTION_MGR::FreeEditBox()
{
	if(NULL != m_pEdit)
	{
		if(NULL != m_pEdit->GetSafeHwnd())
		{
			m_pEdit->DestroyWindow();
			delete m_pEdit;
		}			
		m_pEdit = NULL;
	}

	return 0;
}

int CMNG_ILL_OPTION_MGR::GetValueToCode(int nRow, CString &strResult)
{
	int ids = -1;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::GetValueToCode");

 	if(NULL == m_pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
 	if(0 > nRow || m_pDM->GetRowCount() <= nRow) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

	CString strValueType;
	ids = m_pDM->GetCellData(_T("������"), nRow, strValueType);
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
	
	CString strValue = m_pGrid->GetTextMatrix(nRow+1, 4);
	
	if(_T("�����") == strValue || _T("��") == strValue)
	{
		strResult = _T("Y");
		return 0;
	}
	else if(_T("������") == strValue || _T("�ƴϿ�") == strValue)
	{
		strResult = _T("N");
		return 0;
	}
	else if(_T("����") == strValueType)
	{
		strResult.Format(_T("%d"), _ttoi(strValue));

		return 0;
	}
	else return 1;
	
	return 0;
}

int CMNG_ILL_OPTION_MGR::UpdateDM(WPARAM wParam, LPARAM lParam)
{
	CString strGet;
	const int ROW_COUNT = m_pDM->GetRowCount();
	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{
		m_pDM->GetCellData(_T("���濩��"), nRow, strGet);

		if(_T("Y") == strGet)
		{
			CString strValue;
			if(0 == GetValueToCode(nRow, strValue))
			{
				m_pDM->SetCellData(_T("������"), strValue, nRow);
			}

		}
	}

	return 0;
}

int CMNG_ILL_OPTION_MGR::ShowValue_For_Grid()
{
	const int ROW_COUNT = m_pDM->GetRowCount();
	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{
		CString strValue;
		GetCodeToValue(nRow, strValue);
		m_pGrid->SetTextMatrix(nRow+1, 4, strValue);
	}
	return 0;
}

int CMNG_ILL_OPTION_MGR::GetCodeToValue(int nRow, CString &strResult)
{
	int ids = -1;
	CString strFuncName = _T("CMNG_ILL_OPTION_MGR::GetCodeToAlias");

 	if(NULL == m_pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
 	if(0 > nRow || m_pDM->GetRowCount() <= nRow) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

	CString strValueType;
	ids = m_pDM->GetCellData(_T("������"), nRow, strValueType);
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

	CString strValue;
	ids = m_pDM->GetCellData(_T("������"), nRow, strValue);
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

	CString strValueName;
	ids = m_pDM->GetCellData(_T("���"), nRow, strValueName);
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);

	if(_T("Y/N") == strValueType)
	{
		if(_T("Y") == strValue || _T("y") == strValue || _T("�����") == strValue)
		{ 
			strResult = _T("�����"); 
		}
		else if(_T("N") == strValue || _T("n") == strValue || _T("������") == strValue)
		{ 
			strResult = _T("������"); 
		}
 		else 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);
		}

		return 0;
	}
	else if(_T("�ڷ���ڵ�,") == strValueType || _T("��ϱ���,") == strValueType || _T("��ġ��ȣ,") == strValueType)
	{
		strResult = _T("����");

		return 0;
	}
	else if(_T("��û���ѵǴº���(����)Ƚ��") == strValueName)
	{
		strResult.Format(_T("%dȸ"), _ttoi(strValue));
	}
	else if(_T("����(����)�ν�û���ѽ������ϼ�") == strValueName || _T("å�η�������Ⱓ") == strValueName)
	{
		strResult.Format(_T("%d��"), _ttoi(strValue));		
	}
	else if(_T("å�η��ִ��û����å��") == strValueName)
	{
		strResult.Format(_T("%d��"), _ttoi(strValue));		
	}
	else return 1;
	

	return 0;
}

int CMNG_ILL_OPTION_MGR::FreeComboBox()
{
	if(NULL != m_pCmb)
	{
		if(NULL != m_pCmb->GetSafeHwnd())
		{
			m_pCmb->DestroyWindow();
			delete m_pCmb;
		}			
		m_pCmb = NULL;
	}

	return 0;
}

int CMNG_ILL_OPTION_MGR::ViewPermissionListDlg()
{
	CString strResult;
	m_pDM->GetCellData(_T("������"), (int)m_pGrid->GetRow()-1, strResult);

	CString strValue;		
	
 	CMNG_ILL_PERMISSION_LIST_DLG Dlg(this);
 	Dlg.m_pInfo = m_pInfo;
 	Dlg.m_strValueType = strResult;
 	
 	m_pDM->GetCellData(_T("������"), (int)m_pGrid->GetRow()-1, strValue);
 	Dlg.m_strValue = strValue;
 	
 	m_pDM->GetCellData(_T("REC_KEY"), (int)m_pGrid->GetRow()-1, strValue);
 	Dlg.m_strRecKey = strValue;
 	
 	int nID = Dlg.DoModal();
 	if(IDOK == nID)
 	{
 		Refresh();
 	}

	return 0;
}

void CMNG_ILL_OPTION_MGR::OnDblClickMNGILLOPTIONMGR()
{
	ShowValue_For_Grid();
}