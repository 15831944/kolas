// ILL_SEARCH_BOOK_LIST.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ILL_SEARCH_BOOK_LIST.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILL_SEARCH_BOOK_LIST dialog


CILL_SEARCH_BOOK_LIST::CILL_SEARCH_BOOK_LIST(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CILL_SEARCH_BOOK_LIST)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nSelectIdx = -1;
	m_pSelectDM = NULL;

	m_bIs_BO_Search = FALSE;
	m_bIs_SE_Search = FALSE;
}


void CILL_SEARCH_BOOK_LIST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILL_SEARCH_BOOK_LIST)
	DDX_Control(pDX, IDC_Tab_SEARCH_BOOK_LIST, mCtrl_TabBoSe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CILL_SEARCH_BOOK_LIST, CDialog)
	//{{AFX_MSG_MAP(CILL_SEARCH_BOOK_LIST)	
	ON_NOTIFY(TCN_SELCHANGE, IDC_Tab_SEARCH_BOOK_LIST, OnSelchangeTabSEARCHBOOKLIST)
	ON_BN_CLICKED(IDC_btnBookSELECT, OnbtnSELECT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILL_SEARCH_BOOK_LIST message handlers

BOOL CILL_SEARCH_BOOK_LIST::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());

	int ids = 0;
	CString strFuncName = _T("CILL_SEARCH_BOOK_LIST::OnInitDialog()");

	// TODO: Add extra initialization here
	ids = InitESL_Mgr(_T("ILL_SEARCH_BOOK_LIST"));
	if(0 > ids)
	{
		AfxMessageBox(_T("ERROR : InitESL_Mgr(\"ILL_SEARCH_BOOK_LIST\")"));
		CDialog::OnCancel();
	}

	m_pDM_BO = FindDM(_T("DM_ILL_SEARCH_BOOK_LIST_BO"));
	if(NULL == m_pDM_BO) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	m_pGrid_BO = (CESL_Grid*)FindControl(
		_T("CM_ILL_SEARCH_BOOK_LIST"), _T("�׸���_����_�˻���_���"));
	if(NULL == m_pGrid_BO) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);


	m_pDM_SE = FindDM(_T("DM_ILL_SEARCH_BOOK_LIST_SE"));
	if(NULL == m_pDM_SE) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
	m_pGrid_SE = (CESL_Grid*)FindControl(
		_T("CM_ILL_SEARCH_BOOK_LIST"), _T("�׸���_����_�˻���_���"));
	if(NULL == m_pGrid_SE) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

	if(_T("D") != m_sBookCooperativeMode)
	{
		if(TRUE == m_bIs_BO_Search)
		{	
			ids = m_pDM_BO->RefreshDataManager(m_strSearch_Option_BO);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);
		}

		if(TRUE == m_bIs_SE_Search)
		{	
			ids = m_pDM_SE->RefreshDataManager(m_strSearch_Option_SE);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
		}
	}
	else
	{
		CString strBookManageCode;
  		ids = GetControlData(_T("CM_ILL_SEARCH_BOOK"), _T("��������"), strBookManageCode);

		INT aColIndex[256];	
		if(TRUE == m_bIs_BO_Search)
		{
			CString strQuery = m_pDM_BO->MakeSelectQuery(m_strSearch_Option_BO, aColIndex);
			strQuery.Replace(_T("@ANOTHER_LIB"), _T("@") + strBookManageCode);
			ids = m_pDM_BO->RestructDataManager(strQuery);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);
		}

		if(TRUE == m_bIs_SE_Search)
		{	
			CString strQuery = m_pDM_SE->MakeSelectQuery(m_strSearch_Option_SE, aColIndex);
			strQuery.Replace(_T("@ANOTHER_LIB"), _T("@") + strBookManageCode);
			ids = m_pDM_SE->RestructDataManager(strQuery);			
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
		}
	}

	{
	CString sWorkingStatus;
	const DM_BO_ROW_COUNT = m_pDM_BO->GetRowCount();
	for( INT nRow = 0 ; nRow < DM_BO_ROW_COUNT; nRow++ )
	{
		m_pDM_BO->GetCellData(_T("�ڷ����") , nRow, sWorkingStatus);		
		CLocCommonAPI::GetWorkingStatusDescToCode(sWorkingStatus , 1);		
		m_pDM_BO->SetCellData(_T("�ڷ����_����"), sWorkingStatus ,nRow);		
	}
	}
	{
	CString sWorkingStatus;
	const DM_SE_ROW_COUNT = m_pDM_SE->GetRowCount();
	for( INT nRow = 0 ; nRow < DM_SE_ROW_COUNT; nRow++ )
	{
		m_pDM_SE->GetCellData(_T("�ڷ����") , nRow, sWorkingStatus);		
		CLocCommonAPI::GetWorkingStatusDescToCode(sWorkingStatus , 1);		
		m_pDM_SE->SetCellData(_T("�ڷ����_����"), sWorkingStatus ,nRow);		
	}
	}
	ids = m_pGrid_BO->Display();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-7, strFuncName);
	ids = m_pGrid_SE->Display();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-8, strFuncName);

	mCtrl_TabBoSe.InsertItem(0,	_T("�����ڷ�"));
	mCtrl_TabBoSe.InsertItem(1,	_T("�����ڷ�"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CILL_SEARCH_BOOK_LIST::OnSelchangeTabSEARCHBOOKLIST(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIdx = mCtrl_TabBoSe.GetCurSel();

	UINT uGrid[2] = { IDC_grid_SEARCH_BOOK_LIST_MO, IDC_grid_SEARCH_BOOK_LIST_SE };
	int uShowGrid = 0;
	
	if(0 == nIdx)
	{
		uShowGrid = 0;
	}
	else
	{
		uShowGrid = 1;
	}

	for(int nCnt = 0; nCnt < 2 ; nCnt++)
	{
		CWnd* pWnd = GetDlgItem(uGrid[nCnt]);
		if(NULL != pWnd && NULL != pWnd->GetSafeHwnd())
		{
			if(nCnt == uShowGrid)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}

void CILL_SEARCH_BOOK_LIST::OnbtnSELECT() 
{
	// TODO: Add your control notification handler code here
	int ids = 0;
	CString strFuncName = _T("CILL_SEARCH_BOOK_LIST::OnbtnSELECT()");

	int nIdx = mCtrl_TabBoSe.GetCurSel();
	CESL_Grid* pSelectGrid = NULL;
	
	if(0 == nIdx)
	{
		m_pSelectDM = m_pDM_BO;
		pSelectGrid = m_pGrid_BO;
		m_strPublishForm = _T("MO");
	}
	else if(1 == nIdx)
	{
		m_pSelectDM = m_pDM_SE;
		pSelectGrid = m_pGrid_SE;
		m_strPublishForm = _T("SE");
	}
	else
	{
		m_pSelectDM = NULL;
		pSelectGrid = NULL;
	}

	if(NULL == m_pSelectDM) ERROR_MSG_RETURN_VOID2(-1, strFuncName);
	m_nSelectIdx = pSelectGrid->GetIdx(); // ����� �����ؾ��ϱ� ����

	if(0 >= m_pSelectDM->GetRowCount() || 0 > m_nSelectIdx)
	{
		AfxMessageBox(_T("�ڷḦ �����Ͽ� �ֽʽÿ�."));
	}
	else
	{
		ids = m_pSelectDM->SetCellData(_T("�������±���"), m_strPublishForm, m_nSelectIdx);
		if(0 > ids) ERROR_MSG_RETURN_VOID2(-2, strFuncName);
		
	}

	CString strGet;
	ids = m_pSelectDM->GetCellData(_T("�ڷ����"), m_nSelectIdx, strGet);
	if(0 > ids) ERROR_MSG_RETURN_VOID2(-3, strFuncName);
	
	if(_T("BOL112N") == strGet || _T("SEL212N") == strGet || _T("SEL112N") == strGet)
	{
	}
	else
	{
		AfxMessageBox(_T("��ġ���� �ڷḸ ��û�� �� �ֽ��ϴ�."));
		return;
	}
	
	ids = m_pSelectDM->GetCellData(_T("��������"), m_nSelectIdx, strGet);
	if(0 > ids) ERROR_MSG_RETURN_VOID2(-4, strFuncName);
	
	if(strGet == m_pInfo->MANAGE_CODE)
	{
		AfxMessageBox(_T("�ڰ������� ��û�� �� �����ϴ�."));
		return;
	}	

	ids = m_pSelectDM->GetCellData(_T("����Ǽ�"), m_nSelectIdx, strGet);
	if(0 > ids) ERROR_MSG_RETURN_VOID2(-5, strFuncName);
	
	if(0 < _ttoi(strGet))
	{
		AfxMessageBox(_T("�������� ������ ��û�� �� �����ϴ�."));
		return;
	}	

	CString strManageCode;
	ids = m_pSelectDM->GetCellData(_T("��������"), m_nSelectIdx, strManageCode);
	if(0 > ids) ERROR_MSG_RETURN_VOID2(-6, strFuncName);

	CString strLibName;
	ids = m_pSelectDM->GetCellData(_T("������_�̸�"), m_nSelectIdx, strLibName);
	if(0 > ids) ERROR_MSG_RETURN_VOID2(-7, strFuncName);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString strQuery;
	CString strValue;
	strQuery.Format(
		_T("SELECT VALUE_1 ")
			_T("FROM ILL_MANAGE_TBL ")
		_T("WHERE VALUE_NAME = '������ɻ������' AND MANAGE_CODE = '%s'"),
		strManageCode
		);

	ids = TmpDM.GetOneValueQuery(strQuery, strValue);
	if(0 > ids) ERROR_MSG_RETURN_VOID2(-8, strFuncName);
	
	if(_T("Y") != strValue) 
	{
		CString strMessage;
		strMessage.Format(
			_T("[%s]������ �ڰ��ڷ��� å�η���û�� �����ϰ� �ֽ��ϴ�.\r\n"),
			strLibName
			);
		AfxMessageBox(strMessage);

		return;
	}

	BOOL bIsApl;
	CString strMsg;
	IsAplCode(m_pSelectDM, m_nSelectIdx, bIsApl, strMsg);
	
	if(FALSE == bIsApl)
	{
		AfxMessageBox(strMsg);
		return;
	}

	EndDialog(IDOK);
}

int CILL_SEARCH_BOOK_LIST::IsAplCode(CESL_DataMgr *pDM, int nRow, BOOL &bIsApl, CString &strMsg)
{
	int ids = -1;
	CString strFuncName = _T("CILL_SEARCH_BOOK_LIST::IsAplCode(int nRow, BOOL &bIsApl)");
	CString strGet;

	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
	if(0 > nRow || pDM->GetRowCount() <= nRow) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

	CESL_DataMgr* pDM_IsAplCode = FindDM(_T("DM_ILL_IS_APL_CODE"));
	if(NULL == pDM_IsAplCode) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

	const int CHECK_CODE_COUNT = 3;

	CString strBookManageCode;
	pDM->GetCellData(_T("��������"), nRow, strBookManageCode);

	CString strValue[CHECK_CODE_COUNT] = 
	{
		_T("N"), _T("N"), _T("N")
	};

	{

	CString strOptionName[CHECK_CODE_COUNT] = 
	{
		_T("å�η�����ڷ�Ǹ��"), _T("å�η�����ϱ��и��"), _T("å�η���뺰ġ��ȣ���")
	};
	CString strCheckAlias[CHECK_CODE_COUNT] = 
	{
		_T("�ڷ��_�ڵ�"), _T("��ϱ���"), _T("IBS_û��_��ġ��ȣ_�ڵ�")
	};	
	CString strMsgAlias[CHECK_CODE_COUNT][2] = 
	{
		{ _T("�ڷ��_����"), _T("�ڷ��") },
		{ _T("��ϱ���"), _T("��ϱ���") },
		{ _T("IBS_û��_��ġ��ȣ_�ڵ�"), _T("��ġ��ȣ") }
	};	

	const int IS_APL_DM_ROW_COUNT = pDM_IsAplCode->GetRowCount();
	for(int nAplRowCnt = 0; nAplRowCnt < IS_APL_DM_ROW_COUNT; nAplRowCnt++)
	{
		for(int nCheckCodeCnt = 0; nCheckCodeCnt < CHECK_CODE_COUNT; nCheckCodeCnt++)
		{
			CString strLibManageCode;
			ids = pDM_IsAplCode->GetCellData(_T("��������"), nAplRowCnt, strLibManageCode);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

			if(strBookManageCode != strLibManageCode) continue;

			CString strCodeName;
			ids = pDM_IsAplCode->GetCellData(_T("���"), nAplRowCnt, strCodeName);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);

			if(strCodeName != strOptionName[nCheckCodeCnt]) continue;

			CString strValue_1;
			ids = pDM_IsAplCode->GetCellData(_T("������"), nAplRowCnt, strValue_1);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);			

			if(_T("") == strValue_1)
			{
				strValue[nCheckCodeCnt] = _T("NOT DEFINE");
				break;
			}

			CString strBookValue;
			pDM->GetCellData(strCheckAlias[nCheckCodeCnt], nRow, strBookValue);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-7, strFuncName);

			if(_T("") == strBookValue)
			{
				strBookValue = _T("�������");
			}

			strValue_1.TrimLeft();
			strValue_1.TrimRight();			
			strValue_1.Replace(_T(", "), _T(","));
			strValue_1.TrimRight(_T(","));			

			for(int nCnt = 0; ;nCnt++)
			{
				CString strCode;
				if(FALSE == AfxExtractSubString(strCode, strValue_1, nCnt, ',')) break;

				if(strBookValue == strCode) 
				{
					strValue[nCheckCodeCnt] = _T("Y");
					
					break;
				}
			}
		}
	}

	for(int nCnt = 0; nCnt < CHECK_CODE_COUNT; nCnt++)
	{
		if(_T("N") == strValue[nCnt])
		{
			CString strGet;
			pDM->GetCellData(strMsgAlias[nCnt][0], nRow, strGet);
			
			if(_T("") == strGet)
			{
				strGet = _T("�������");
			}

			strMsg.Format(
				_T("[%s]�� å�η� ����Ͽ� ���ǵ����� ���� %s�Դϴ�."), 
				strGet,
				strMsgAlias[nCnt][1]);
			bIsApl = FALSE;

			return 0;
		}
	}
	}

	if(_T("NOT DEFINE") == strValue[0])
	{
		ids = m_pSelectDM->GetCellData(_T("���Ⱑ�ɿ���_�ڷ��"), nRow, strGet);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

		if(_T("N") == strGet)
		{
			ids = m_pSelectDM->GetCellData(_T("�ڷ��_����"), nRow, strGet);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
			
			strMsg.Format(
				_T("[%s] �ڷ���� ���������� �ɷ��־� ��û�� �� �����ϴ�."),
				strGet);

			bIsApl = FALSE;
			return 0;
		}
	}
	if(_T("NOT DEFINE") == strValue[1])
	{
		ids = pDM->GetCellData(_T("���Ⱑ�ɿ���_��ϱ���"), nRow, strGet);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

		if(_T("N") == strGet)
		{
			ids = pDM->GetCellData(_T("��ϱ���"), nRow, strGet);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);
			
			strMsg.Format(
				_T("��ϱ���[%s]�� ���������� �ɷ��־� ��û�� �� �����ϴ�."),
				strGet);
			
			bIsApl = FALSE;
			return 0;
		}
	}
	if(_T("NOT DEFINE") == strValue[2])
	{
		ids = pDM->GetCellData(_T("���Ⱑ�ɿ���_��ġ��ȣ"), nRow, strGet);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-7, strFuncName);

		if(_T("N") == strGet)
		{
			ids = pDM->GetCellData(_T("IBS_û��_��ġ��ȣ_�ڵ�"), nRow, strGet);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-8, strFuncName);

			if(_T("") == strGet) strGet = _T("�������");
			
			strMsg.Format(
				_T("��ġ��ȣ[%s]�� ���������� �ɷ��־� ��û�� �� �����ϴ�."),
				strGet);
			
			bIsApl = FALSE;
			return 0;
		}
	}

	bIsApl = TRUE;

	return 0;
}

BEGIN_EVENTSINK_MAP(CILL_SEARCH_BOOK_LIST, CDialog)
    //{{AFX_EVENTSINK_MAP(CILL_SEARCH_BOOK_LIST)
	ON_EVENT(CILL_SEARCH_BOOK_LIST, IDC_grid_SEARCH_BOOK_LIST_MO, -601 /* DblClick */, OnDblClickgridSEARCHBOOKLISTMO, VTS_NONE)
	ON_EVENT(CILL_SEARCH_BOOK_LIST, IDC_grid_SEARCH_BOOK_LIST_SE, -601 /* DblClick */, OnDblClickgridSEARCHBOOKLISTSE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CILL_SEARCH_BOOK_LIST::OnDblClickgridSEARCHBOOKLISTMO() 
{
	// TODO: Add your control notification handler code here
	OnbtnSELECT();
}

void CILL_SEARCH_BOOK_LIST::OnDblClickgridSEARCHBOOKLISTSE() 
{
	// TODO: Add your control notification handler code here
	OnbtnSELECT();
}

BOOL CILL_SEARCH_BOOK_LIST::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}
