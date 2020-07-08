// UserMainBookDetailSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserMainBookDetailSearchDlg.h"
#include "imm.h"
#include "..\..\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserMainBookDetailSearchDlg dialog


CUserMainBookDetailSearchDlg::CUserMainBookDetailSearchDlg(CESL_Mgr* pParent /*=NULL*/, CCommonProc *pCmdProc, CRect rect)
	: CESL_Mgr(CUserMainBookDetailSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserMainBookDetailSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCmdProc = pCmdProc;
	m_rect = rect;
	m_bIsRunningThread = FALSE;
	m_pParent = (CWnd*)pParent;
}


void CUserMainBookDetailSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserMainBookDetailSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserMainBookDetailSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CUserMainBookDetailSearchDlg)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_DETAIL_SEARCH_btnClose, OnDETAILSEARCHbtnClose)
	ON_BN_CLICKED(IDC_DETAIL_SEARCH_btnSearch, OnDETAILSEARCHbtnSearch)
	ON_BN_CLICKED(IDC_DETAIL_SEARCH_btnClear, OnDETAILSEARCHbtnClear)
	ON_EN_SETFOCUS(IDC_DETAIL_SEARCH_edtTitle, OnSetfocusDETAILSEARCHedtTitle)
	ON_EN_SETFOCUS(IDC_DETAIL_SEARCH_edtTotal, OnSetfocusDETAILSEARCHedtTotal)
	ON_EN_SETFOCUS(IDC_DETAIL_SEARCH_edtAuthor, OnSetfocusDETAILSEARCHedtAuthor)
	ON_EN_SETFOCUS(IDC_DETAIL_SEARCH_edtPublisher, OnSetfocusDETAILSEARCHedtPublisher)
	ON_WM_SIZE()
	ON_EN_SETFOCUS(IDC_DETAIL_SEARCH_edtRegNo, OnSetfocusDETAILSEARCHedtRegNo)
	ON_BN_CLICKED(IDC_DETAIL_SEARCH_btnTextLang, OnDETAILSEARCHbtnTextLang)
	ON_EN_SETFOCUS(IDC_DETAIL_SEARCH_edtTextLang, OnSetfocusDETAILSEARCHedtTextLang)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserMainBookDetailSearchDlg message handlers

BOOL CUserMainBookDetailSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(VK_RETURN == pMsg->wParam)
		{
			if(GetDlgItem(IDC_DETAIL_SEARCH_edtTitle)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtKeyword1)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtKeyword2)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtKeyword3)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtTotal)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtAuthor)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtPublisher)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtPublishYear1)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtPublishYear2)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtClassNo1)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtClassNo2)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtRegNo)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_btnClear)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnClear();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_btnSearch)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnSearch();
			}
			else if(GetDlgItem(IDC_DETAIL_SEARCH_btnClose)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnClose();
			}
//+++++ 2011.9.16 ä��ȣ [�߰�]
// ���� : ����ñ���û ��ɰ������
// �䱸 : ��� ������ �˻� �ǵ��� ����
			else if(GetDlgItem(IDC_DETAIL_SEARCH_edtTextLang)->GetSafeHwnd() == pMsg->hwnd)
			{
				OnDETAILSEARCHbtnTextLang();
			}
//-----
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CUserMainBookDetailSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CUserMainBookDetailSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	if(InitESL_Mgr(_T("SM_USERMAIN_DETAIL_SEARCH")) < 0)
 	{
 		AfxMessageBox(_T("InitESL_Mgr Error!"));
 		return FALSE;
 	}

	SetSearchMode();

	OnDETAILSEARCHbtnClear();

	SetDlgPos();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserMainBookDetailSearchDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void CUserMainBookDetailSearchDlg::OnDETAILSEARCHbtnClose() 
{
	OnCancel();
}

void CUserMainBookDetailSearchDlg::OnDETAILSEARCHbtnSearch() 
{
	CWinThread * Thread;
	Thread = AfxBeginThread(SearchThread, this);
	
// 	INT ids;
// 	ids = SearchBookInfo();
// 	if(0 == ids)
// 	{
// 		OnOK();
// 	}
}

void CUserMainBookDetailSearchDlg::OnDETAILSEARCHbtnClear() 
{
	INT ids;
	CESL_ControlMgr *pCM = FindCM(_T("CM_USERMAIN_DETAIL_SEARCH"));
	if(NULL == pCM)
	{
		return;
	}
	ids = pCM->AllControlClear();
	if(0 > ids)
	{
		return ;
	}

	((CEdit*)GetDlgItem(IDC_DETAIL_SEARCH_edtTitle))->SetFocus();
}

INT CUserMainBookDetailSearchDlg::SearchBookInfo()
{
	INT ids;
	CString sBoWhere,sSeWhere;

	BeginWaitCursor();

	// �������ǻ���
	ids = GetSqlSearchBookInfo(sBoWhere, 0);	
	if(0 > ids) 
	{
		EndWaitCursor();
		return -1;
	}
	if(ids > 0 )
	{
		EndWaitCursor();
		return ids;
	}
	// �������ǻ���
	ids = GetSqlSearchBookInfo(sSeWhere, 1);	
	if(0 > ids)
	{
		EndWaitCursor();
		return -2;
	}

	// �˻�:DM����
	CESL_DataMgr *pDM;
	if(_T("0") == m_pCmdProc->m_strSearchMode)
	{
		pDM = FindDM(_T("DM_USERMAIN_BOOKSEARCH_B_BO"));
		if(NULL == pDM)
		{
			EndWaitCursor();
			return -2;
		}
	}
	else
	{
		pDM = FindDM(_T("DM_USERMAIN_BOOKSEARCH_S_BO"));
		if(NULL == pDM)
		{
			EndWaitCursor();
			return -2;
		}
	}

// 	m_pParent->SendMessage(MSG_SHOW_WAITDISPLAY, TRUE, NULL);

	CString sResult, sPublishForm;
	GetControlData(_T("CM_USERMAIN_DETAIL_SEARCH"), _T("�ڷᱸ��"), sPublishForm);
	ids = m_pCmdProc->SendSearchBookInfoSQL(pDM, sPublishForm, sBoWhere, sSeWhere, sResult);
	if(0 > ids)
	{
		return -3;
	}
	if(1000 == ids)
	{
		m_pParent->SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
		MessageBox(_T("�ڷᰡ �������� �ʽ��ϴ�. \r\n�˻�� Ȯ���ϼ���."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
		return -1;
	}
	if(ids > 0)
	{
		return ids;
	}

//+++++ 2011.9.16 ä��ȣ [�߰�]
// ���� : ����ñ���û ��ɰ������
// �䱸 : �ڷ�ã�⿡�� �����ڷ��� ��� �������� ǥ�õǵ��� ����
	CString sData, sData2;
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		ids = pDM->GetCellData(_T("���������̷°Ǽ�"), i, sData);
		if(0 > ids) { return -12; }

		// �������˻��� ��� '�����-å��' ������ ��� å�� ����Ǿ����� '0'���� ����
		// å�����˻��� ��� ������� ����.
		if ( 
			(_T("0") == m_pCmdProc->m_strSearchMode && sData > _T("0")) ||
			(_T("0") != m_pCmdProc->m_strSearchMode && sData == _T("0")) 
		) 
		{
			sData2.Format(_T("X(������(%s��))"), sData);
			ids = pDM->SetCellData(_T("���Ⱑ�ɿ���"), sData2, i);
			if(0 > ids) { return -15; }

			ids = pDM->GetCellData(_T("�ڷ���¼���"), i, sData);
			if(0 > ids) { return -12; }

			if ( sData != _T("������") ) {
				ids = pDM->SetCellData(_T("�ڷ���¼���"), _T("������"), i);
				if(0 > ids) { return -15; }
			} 
		}
	}
//-----

// 	g_pUserMainView->SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);

	// SetWhere
	pParentMgr->SendMessage(WM_USER_SET_MSG_WHERE, 0, (LPARAM)&m_strMsgWhere);
	// GridDisplay
	pParentMgr->SendMessage(WM_USER_GRID_DISPLAY, 0, (LPARAM)&sResult);
	// ShowReSearchControl
	pParentMgr->SendMessage(WM_USER_SHOWRESEARCH, TRUE);

	return 0;
}

INT CUserMainBookDetailSearchDlg::GetSqlSearchBookInfo(CString &sWhere, INT nMode)
{
	INT nEcoSearchCnt = 0;
	INT nSearchField = 0;
	CString sEcoSearchWhere;

//+++++ 2011.9.16 ä��ȣ [�߰�]
// ���� : ����ñ���û ��ɰ������
// �䱸 : ��� ������ �˻� �ǵ��� ����
/* OLD  : 
	const INT nFieldCnt = 4;
	CString sFieldAlias[nFieldCnt] = 
	{ _T("����"), _T("���ڸ�"), _T("������"), _T("��ü") };
*/
	const INT nFieldCnt = 5;
	CString sFieldAlias[nFieldCnt] = 
	{ _T("����"), _T("���ڸ�"), _T("������"), _T("��ü"), _T("���") };
//-----
	CString sGetData[nFieldCnt];
	for(INT i = 0; i < nFieldCnt; i++)
	{
		GetControlData(_T("CM_USERMAIN_DETAIL_SEARCH"), sFieldAlias[i], sGetData[i]);
		sGetData[i].TrimRight();
	}
	
	if(0 == nMode)
	{
		m_strMsgWhere = _T("\"");
	}

	BOOL bIsEqualSearch;
	// 1. ����
	if(!sGetData[0].IsEmpty() )
	{
		if(1 < sWhere.GetLength()) 
		{
			sWhere += _T(" AND ");
		}
		
		bIsEqualSearch = FALSE;
		if('\"' == sGetData[0].GetAt(0))
		{
			bIsEqualSearch = TRUE;
		}
		
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(sGetData[0]);
		if(!bIsEqualSearch ) 
		{
			sGetData[0] += _T("*");
		}
		else
		{
			sGetData[0].Replace(_T("\""), _T(""));
		}
		if(0 == nMode)
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_TITLE, '") + sGetData[0] + _T("') ");
			if(_T("\"") != m_strMsgWhere)
			{
				m_strMsgWhere += _T(",  ");
			}
			m_strMsgWhere += sGetData[0];
		}
		else
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_TITLE, '") + sGetData[0] + _T("') ");
		}		
		nEcoSearchCnt++;
		nSearchField++;
	}
	// 2. ���ڸ�
	if(!sGetData[1].IsEmpty() )
	{
		if(1 < sWhere.GetLength())
		{
			sWhere += _T(" AND ");
		}

		bIsEqualSearch = FALSE;
		if('\"' == sGetData[1].GetAt(0))
		{
			bIsEqualSearch = TRUE;
		}

		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(sGetData[1]);
		
		if(!bIsEqualSearch )
		{
			sGetData[1] += _T("*");
		}
		else
		{
			sGetData[1].Replace(_T("\""), _T(""));
		}
		if(!sEcoSearchWhere.IsEmpty() )
		{
			sEcoSearchWhere += _T(" INTERSECT ");
		}
		if(0 == nMode)
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
			if(_T("\"") != m_strMsgWhere)
			{
				m_strMsgWhere += _T(",  ");
			}
			m_strMsgWhere += sGetData[1];
		}
		else
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
		}
		nEcoSearchCnt++;
		nSearchField++;
	}
	// 3. ������
	if(!sGetData[2].IsEmpty() )
	{
		if(1 < sWhere.GetLength())
		{
			sWhere += _T(" AND ");
		}

		bIsEqualSearch = FALSE;
		if('\"' == sGetData[2].GetAt(0))
		{
			bIsEqualSearch = TRUE;
		}
		
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(sGetData[2]);

		if(!bIsEqualSearch ) 
		{
			sGetData[2] += _T("*");
		}
		else
		{
			sGetData[2].Replace(_T("\""), _T(""));
		}
		
		if(!sEcoSearchWhere.IsEmpty())
		{
			sEcoSearchWhere += _T(" INTERSECT ");
		}
		if(0 == nMode)
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_PUBLISHER , '") + sGetData[2] + _T("') ");
			if(_T("\"") != m_strMsgWhere)
			{
				m_strMsgWhere += _T(",  ");
			}
			m_strMsgWhere += sGetData[2];
		}
		else
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_PUBLISHER , '") + sGetData[2] + _T("') ");
		}
		nEcoSearchCnt++;
		nSearchField++;
	}
	// 4. ��ü
	if(!sGetData[3].IsEmpty() )
	{
		if(1 < sWhere.GetLength())
		{
			sWhere += _T(" AND ");
		}

		bIsEqualSearch = FALSE;
		if('\"' == sGetData[3].GetAt(0))
		{
			bIsEqualSearch = TRUE;
		}
		
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(sGetData[3]);

		if(!bIsEqualSearch ) 
		{
			sGetData[3] += _T("*");
		}
		else
		{
			sGetData[3].Replace(_T("\""), _T(""));
		}
		if(!sEcoSearchWhere.IsEmpty() )
		{
			sEcoSearchWhere += _T(" INTERSECT ");
		}
		if(0 == nMode)
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_ALLITEM , '") + sGetData[3] + _T("') ");
			if(_T("\"") != m_strMsgWhere)
			{
				m_strMsgWhere += _T(",  ");
			}
			m_strMsgWhere += sGetData[3];
		}
		else
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_ALLITEM , '") + sGetData[3] + _T("') ");
		}		
		nEcoSearchCnt++;
		nSearchField++;
	}

//+++++ 2011.9.16 ä��ȣ [�߰�]
// ���� : ����ñ���û ��ɰ������
// �䱸 : ��� ������ �˻� �ǵ��� ����
	// 5. ���
	if(!sGetData[4].IsEmpty() )
	{
		if(1 < sWhere.GetLength())
		{
			sWhere += _T(" AND ");
		}

		bIsEqualSearch = FALSE;
		if('\"' == sGetData[4].GetAt(0))
		{
			bIsEqualSearch = TRUE;
		}
		
		if(!bIsEqualSearch ) 
		{
			sGetData[4] += _T("*");
		}
		else
		{
			sGetData[4].Replace(_T("\""), _T(""));
		}
		
		if(!sEcoSearchWhere.IsEmpty())
		{
			sEcoSearchWhere += _T(" INTERSECT ");
		}
		if(0 == nMode)
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , BO_TEXT_LANG , '") + sGetData[4] + _T("') ");
			if(_T("\"") != m_strMsgWhere)
			{
				m_strMsgWhere += _T(",  ");
			}
			m_strMsgWhere += sGetData[4];
		}
		else
		{
			sEcoSearchWhere += _T(" &ECOSEARCH( , SE_TEXT_LANG , '") + sGetData[4] + _T("') ");
		}
		nEcoSearchCnt++;
		nSearchField++;
	}
//-----

	// 5. ComboBox (��������,�ڷ��,�ڷ�����,��ü����)
	const INT nComboFieldCnt = 4;
	CString sComboAlias[nComboFieldCnt] =
	{ _T("�ڷ�����"), _T("��ü����"), _T("�ڷ��"), _T("��������") };
	CString sDBFieldAlias[ nComboFieldCnt ] =
	{ _T("IB.FORM_CODE"), _T("IB.MEDIA_CODE"), _T("BB.SHELF_LOC_CODE"), _T("BB.MANAGE_CODE") };
	CString sClassAlias[nComboFieldCnt] =
	{ _T("���ı���"), _T("��ü����"), _T("�ڷ�Ǳ���"), _T("��������") };
	CString sComboGetData[nComboFieldCnt];
	for(i = 0; i < nComboFieldCnt; i++)
	{
		GetControlData(_T("CM_USERMAIN_DETAIL_SEARCH"), sComboAlias[i], sComboGetData[i]);
		sComboGetData[i].TrimRight();
		if(sComboGetData[i].IsEmpty() || _T("0") == sComboGetData[i] || _T("�������") == sComboGetData[i])
		{
			continue;
		}
		if(1 < sWhere.GetLength())
		{
			sWhere += _T(" AND ");
		}
		sWhere += sDBFieldAlias[i] + _T(" = '") + sComboGetData[i] + _T("' ");
		if(0 == nMode)
		{
			CString sDesc;
			if(_T("\"") != m_strMsgWhere)
			{
				m_strMsgWhere += _T(",  ");
			}
			m_pInfo->pCodeMgr->ConvertCodeToDesc(sClassAlias[i], sComboGetData[i], sDesc);
			m_strMsgWhere += sDesc;
		}
	}
	// 6. Ű����
	const INT nKeyWordFieldCnt = 5;
	CString sKeyWordAlias[nKeyWordFieldCnt] =
	{ _T("Ű����1"), _T("Ű����2"), _T("Ű����3"), _T("Ű����4"), _T("Ű����5") };
	CString sKeyWordGetData[nKeyWordFieldCnt];
	for(i = 0; i < nKeyWordFieldCnt; i++)
	{
		GetControlData(_T("CM_USERMAIN_DETAIL_SEARCH"), sKeyWordAlias[i], sKeyWordGetData[i]);
		sKeyWordGetData[i].TrimRight();
	}
	CString sKeyWordWhere;
	if(!sKeyWordGetData[0].IsEmpty())
	{
		bIsEqualSearch = FALSE;
		if('\"' == sKeyWordGetData[0].GetAt(0))
		{
			bIsEqualSearch = TRUE;
		}
		
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(sKeyWordGetData[0]);

		if(!bIsEqualSearch ) 
		{
			sKeyWordGetData[0] += _T("*");
		}
		else
		{
			sKeyWordGetData[0].Replace(_T("\""), _T(""));
		}
		sKeyWordWhere = sKeyWordGetData[0];
		if(!sKeyWordGetData[2].IsEmpty())
		{
			bIsEqualSearch = FALSE;
			if('\"' == sKeyWordGetData[2].GetAt(0))
			{
				bIsEqualSearch = TRUE;
			}
			
			((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(sKeyWordGetData[2]);

			if(!bIsEqualSearch) 
			{
				sKeyWordGetData[2] += _T("*");
			}
			else
			{
				sKeyWordGetData[2].Replace(_T("\""), _T(""));
			}
							
			sKeyWordGetData[1].MakeUpper();
			if(_T("OR") == sKeyWordGetData[1])
			{
				sKeyWordWhere += _T(" | ");
			}
			else
			{
				sKeyWordWhere += _T(" ");
			}
			sKeyWordWhere += sKeyWordGetData[2];				
		}
		if(!sKeyWordGetData[4].IsEmpty() )
		{
			bIsEqualSearch = FALSE;
			if('\"' == sKeyWordGetData[4].GetAt(0))
			{
				bIsEqualSearch = TRUE;
			}
			
			((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(sKeyWordGetData[4]);

			if(!bIsEqualSearch ) 
			{
				sKeyWordGetData[4] += _T("*");
			}
			else
			{
				sKeyWordGetData[4].Replace(_T("\""), _T(""));
			}
			sKeyWordGetData[3].MakeUpper();
			if(_T("OR") == sKeyWordGetData[3])
			{
				sKeyWordWhere += _T(" | ");
			}
			else
			{
				sKeyWordWhere += _T(" ");
			}
			sKeyWordWhere += sKeyWordGetData[4];				
		}
		if(!sKeyWordWhere.IsEmpty())
		{
			if(!sEcoSearchWhere.IsEmpty())
			{
				sEcoSearchWhere += _T(" INTERSECT ");
			}	
			if(0 == nMode)
			{
				sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_KEYWORD, '") + sKeyWordWhere + _T("') ");
				if(_T("\"") != m_strMsgWhere)
				{
					m_strMsgWhere += _T(",  ");
				}
				m_strMsgWhere += sKeyWordWhere;
			}
			else
			{
				sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_KEYWORD, '") + sKeyWordWhere + _T("') ");
			}
			nEcoSearchCnt++;
			nSearchField++;
		}
	}
	// 7. �����
	const INT nPublishYearFieldCnt = 2;
	CString sPublishYearAlias[nPublishYearFieldCnt] = { _T("����⵵1"), _T("����⵵2") };
	CString sPublishYearGetData[nPublishYearFieldCnt];
	for(i = 0; i < nPublishYearFieldCnt; i++)
	{
		GetControlData(_T("CM_USERMAIN_DETAIL_SEARCH"), sPublishYearAlias[i], sPublishYearGetData[i]);
		sPublishYearGetData[i].TrimRight();
	}
	if(!sPublishYearGetData[0].IsEmpty() )
	{
		if(1 < sWhere.GetLength())
		{
			sWhere += _T(" AND ");
		}
		if(sPublishYearGetData[1].IsEmpty())
		{
			sWhere += _T(" IB.IDX_IPUB_YEAR = '") + sPublishYearGetData[0] + _T("' ");
			if(0 == nMode)
			{
				if(_T("\"") != m_strMsgWhere)
				{
					m_strMsgWhere += _T(",  ");
				}
				m_strMsgWhere += sPublishYearGetData[0];
			}
		}
		else
		{
			sWhere +=	_T(" (IB.IDX_IPUB_YEAR >= '") + sPublishYearGetData[0] + _T("' AND ") +
						_T(" IB.IDX_IPUB_YEAR <= '") + sPublishYearGetData[1] + _T("')");
			if(0 == nMode)
			{
				if(_T("\"") != m_strMsgWhere)
				{
					m_strMsgWhere += _T(",  ");
				}
				m_strMsgWhere += sPublishYearGetData[0] + _T("~") + sPublishYearGetData[1];
			}
		}
	}
	// 8. �з���ȣ
	const INT nClassNoCnt = 2;
	CString sClassNoAlias[nClassNoCnt] = { _T("�з���ȣ1"), _T("�з���ȣ2") };
	CString sClassNoData[nClassNoCnt];
	for(i = 0; i < nClassNoCnt; i++)
	{
		GetControlData(_T("CM_USERMAIN_DETAIL_SEARCH"), sClassNoAlias[i], sClassNoData[i]);
		sClassNoData[i].TrimRight();
	}
	if(!sClassNoData[0].IsEmpty())
	{
		if(1 < sWhere.GetLength())
		{
			sWhere += _T(" AND ");
		}
		if(sClassNoData[1].IsEmpty())
		{
			sWhere += _T(" BB.CLASS_NO = '") + sClassNoData[0] + _T("' ");
			if(0 == nMode)
			{
				if(_T("\"") != m_strMsgWhere)
				{
					m_strMsgWhere += _T(",  ");
				}
				m_strMsgWhere += sClassNoData[0];
			}
		}
		else
		{
			sWhere += _T(" BB.CLASS_NO BETWEEN '") + sClassNoData[0] + _T("' AND '") + sClassNoData[1] + _T("' ") ;
			if(0 == nMode)
			{
				if(_T("\"") != m_strMsgWhere)
				{
					m_strMsgWhere += _T(",  ");
				}
				m_strMsgWhere += sClassNoData[0] + _T("~") + sClassNoData[1];
			}
		}
	}
	// 9. ��Ϲ�ȣ
	CString sRegNoData;	
	GetControlData(_T("CM_USERMAIN_DETAIL_SEARCH"), _T("��Ϲ�ȣ"), sRegNoData);
	sRegNoData.TrimRight();
	if(!sRegNoData.IsEmpty())
	{
		m_pCmdProc->m_pLocCommonAPI->MakeRegNoSpaceToZero(m_pCmdProc->m_pParent, sRegNoData, m_pCmdProc->m_pPreRegNoMgr);
// 		m_pCmdProc->m_pLocCommonAPI->MakeRegNoSpaceToZero(sRegNoData);
		SetControlData(_T("CM_USERMAIN_DETAIL_SEARCH"), _T("��Ϲ�ȣ"), sRegNoData);
		if(1 < sWhere.GetLength())
		{
			sWhere += _T(" AND ");
		}
		sWhere += _T(" BB.REG_NO='") + sRegNoData + _T("' ") ;
		if(0 == nMode)
		{
			if(_T("\"") != m_strMsgWhere)
			{
				m_strMsgWhere += _T(",  ");
			}
			m_strMsgWhere += sRegNoData;
		}
		nSearchField++;
	}
	// 10. �缭����
	//===================================================
	//2009.10.22 REM BY PJW : �������ڷ�ã��� �缭���ѵ� �˻��ǵ��� �Ѵ�.
//	if(!sWhere.IsEmpty())
//	{
//		sWhere += _T(" AND ");
//	}
//	sWhere += _T(" BB.USE_LIMIT_CODE != 'CL' ");
	//===================================================
	
	// 11. ECOSEARCH
	if(!sEcoSearchWhere.IsEmpty())
	{
		if(!sWhere.IsEmpty())
		{
			sWhere += _T(" AND ");
		}
		if(1 == nEcoSearchCnt)
		{
			sWhere += _T(" IB.REC_KEY IN ") + sEcoSearchWhere;
		}
		else
		{
			sWhere += _T(" IB.REC_KEY IN ( ") + sEcoSearchWhere + _T(" ) ");
		}
	}

	if(0 == nMode)
	{
		if(_T("\"") == m_strMsgWhere)
		{
			m_strMsgWhere = _T("��ü�˻�");
		}
		else
		{
			m_strMsgWhere += _T("\"");
		}
	}

	// 2012.11.05 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2��
	// ISBN(ISSN)
	CString strISBN;	
	GetControlData(_T("CM_USERMAIN_DETAIL_SEARCH"), _T("ISBN"), strISBN);
	strISBN.TrimRight();
	if(!strISBN.IsEmpty())
	{		
		if(0 == nMode)
		{
			if(1 < sWhere.GetLength())
			{
				sWhere += _T(" AND ");
			}
			sWhere += _T(" BB.EA_ISBN='") + strISBN + _T("' ") ;
			if(0 == nMode)
			{
				if(_T("\"") != m_strMsgWhere)
				{
					m_strMsgWhere += _T(",  ");
				}
				m_strMsgWhere += strISBN;
			}
			nSearchField++;
		}
		else if(1 == nMode)
		{
			if(1 < sWhere.GetLength())
			{
				sWhere += _T(" AND ");
			}
			sWhere += _T(" IB.ST_ISSN='") + strISBN + _T("' ") ;
			if(0 == nMode)
			{
				if(_T("\"") != m_strMsgWhere)
				{
					m_strMsgWhere += _T(",  ");
				}
				m_strMsgWhere += strISBN;
			}
			nSearchField++;
		}
	}

	// 12. ����Ȯ��
	if(sWhere.IsEmpty())
	{
		m_pParent->SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
		MessageBox(_T("�˻�� �Է¼���."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
		return 1;
	}
	if(0 == nSearchField && 0 == nMode)
	{
		INT ids;
		m_pParent->SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);
		ids = MessageBox(_T("�˻� ������ �����Ͽ� �ð��� ���� �ɸ��ų� ����� ����� �������� ���� �� �ֽ��ϴ�. \r\n�˻��� ���� �Ͻðڽ��ϱ�?"), _T("�ڷ�˻�"), MB_ICONWARNING | MB_YESNO);
		if(ids == IDNO)
		{
			return 1;
		}
		m_pParent->SendMessage(MSG_SHOW_WAITDISPLAY, TRUE, NULL);
	}
	// 13. ������ �ڷ�	
	if(0 == nMode )
	{
		if(!sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		
		sWhere += _T(" BB.WORKING_STATUS NOT IN ('BOT211O','BOT212O') ");
	}
	return 0;
}

VOID CUserMainBookDetailSearchDlg::SetSearchMode()
{
	// �ڷ�˻���� (0:å�����˻�, 1:�������˻�)
	if(_T("0") == m_pCmdProc->m_strSearchMode)
	{
		CWnd* pWnd = GetDlgItem(IDC_DETAIL_SEARCH_stcRegNo);
		if ( pWnd == NULL ) return;
		pWnd->ShowWindow(SW_SHOW);
		// GetDlgItem(IDC_DETAIL_SEARCH_stcRegNo)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DETAIL_SEARCH_edtRegNo)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DETAIL_SEARCH_stcRegNo)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DETAIL_SEARCH_edtRegNo)->ShowWindow(SW_HIDE);

//+++++ 2011.9.16 ä��ȣ [�߰�]
// ���� : ����ñ���û ��ɰ������
// �䱸 : ��� ������ �˻� �ǵ��� ����
// OLD  :		INT nPos = 25;
		INT nPos = 5;
//-----
		CRect sRect, sRect1, sRect2, sRect3, sRect4;
		CWnd* pControl1 = GetDlgItem(IDC_DETAIL_SEARCH_stcLine);
		CWnd* pControl2 = GetDlgItem(IDC_DETAIL_SEARCH_btnClear);
		CWnd* pControl3 = GetDlgItem(IDC_DETAIL_SEARCH_btnSearch);
		CWnd* pControl4 = GetDlgItem(IDC_DETAIL_SEARCH_btnClose);
		GetWindowRect(sRect);
		pControl1->GetWindowRect(sRect1);
		pControl2->GetWindowRect(sRect2);
		pControl3->GetWindowRect(sRect3);
		pControl4->GetWindowRect(sRect4);
		ScreenToClient(sRect1);
		ScreenToClient(sRect2);
		ScreenToClient(sRect3);
		ScreenToClient(sRect4);
		sRect.bottom = sRect.bottom-nPos;
		sRect1.bottom = sRect1.bottom-nPos;
		sRect2.top = sRect2.top-nPos;
		sRect2.bottom = sRect2.bottom-nPos;
		sRect3.top = sRect3.top-nPos;
		sRect3.bottom = sRect3.bottom-nPos;
		sRect4.top = sRect4.top-nPos;
		sRect4.bottom = sRect4.bottom-nPos;
		this->MoveWindow(sRect);
		pControl1->MoveWindow(sRect1);
		pControl2->MoveWindow(sRect2);
		pControl3->MoveWindow(sRect3);
		pControl4->MoveWindow(sRect4);
	}
}


void CUserMainBookDetailSearchDlg::OnSetfocusDETAILSEARCHedtTitle() 
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
    hIME = ::ImmGetContext(GetDlgItem(IDC_DETAIL_SEARCH_edtTitle)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_HANGUL;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
        ImmReleaseContext(GetDlgItem(IDC_DETAIL_SEARCH_edtTitle)->m_hWnd,hIME); 
    }
}

void CUserMainBookDetailSearchDlg::OnSetfocusDETAILSEARCHedtTotal() 
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
    hIME = ::ImmGetContext(GetDlgItem(IDC_DETAIL_SEARCH_edtTotal)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_HANGUL;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
        ImmReleaseContext(GetDlgItem(IDC_DETAIL_SEARCH_edtTotal)->m_hWnd,hIME); 
    }
}

void CUserMainBookDetailSearchDlg::OnSetfocusDETAILSEARCHedtAuthor() 
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
    hIME = ::ImmGetContext(GetDlgItem(IDC_DETAIL_SEARCH_edtAuthor)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_HANGUL;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
        ImmReleaseContext(GetDlgItem(IDC_DETAIL_SEARCH_edtAuthor)->m_hWnd,hIME); 
    }
}

void CUserMainBookDetailSearchDlg::OnSetfocusDETAILSEARCHedtPublisher() 
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
    hIME = ::ImmGetContext(GetDlgItem(IDC_DETAIL_SEARCH_edtPublisher)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_HANGUL;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
        ImmReleaseContext(GetDlgItem(IDC_DETAIL_SEARCH_edtPublisher)->m_hWnd,hIME); 
    }
}

BOOL CUserMainBookDetailSearchDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

void CUserMainBookDetailSearchDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

VOID CUserMainBookDetailSearchDlg::SetDlgPos()
{
	CRect rDlg;
    GetWindowRect(rDlg);
    MoveWindow(m_rect.left-80, m_rect.bottom+2, rDlg.Width(), rDlg.Height());
} 


void CUserMainBookDetailSearchDlg::OnSetfocusDETAILSEARCHedtRegNo() 
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
    hIME = ::ImmGetContext(GetDlgItem(IDC_DETAIL_SEARCH_edtRegNo)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_ALPHANUMERIC;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
        ImmReleaseContext(GetDlgItem(IDC_DETAIL_SEARCH_edtRegNo)->m_hWnd,hIME); 
    }
}

UINT CUserMainBookDetailSearchDlg::SearchThread(LPVOID pParam)
{
	CUserMainBookDetailSearchDlg* pDlg = (CUserMainBookDetailSearchDlg*)pParam;

	pDlg->m_bIsRunningThread = TRUE;
	
	pDlg->m_pParent->SendMessage(MSG_SHOW_WAITDISPLAY, TRUE, NULL);
	pDlg->EnableWindow(FALSE);
	INT	ids = pDlg->SearchBookInfo();
 	if(0 == ids)
 	{
		pDlg->PostMessage(WM_CLOSE, NULL, NULL);
 	}
	pDlg->EnableWindow(TRUE);
	pDlg->m_pParent->SendMessage(MSG_SHOW_WAITDISPLAY, FALSE, NULL);

	pDlg->m_bIsRunningThread = FALSE;

	return 0;
}

//+++++ 2011.9.16 ä��ȣ [�߰�]
// ���� : ����ñ���û ��ɰ������
// �䱸 : ��� ������ �˻� �ǵ��� ����
// ���� : ����ȣ �˻�ȭ���� �����Ѵ�.
void CUserMainBookDetailSearchDlg::OnDETAILSEARCHbtnTextLang() 
{
	_TCHAR sCode[8];

	GetDlgItemText(IDC_DETAIL_SEARCH_edtTextLang, sCode, 4);

	CKOMarcGetterDlg dlg(this);
	dlg.InitSetProperty(2, sCode);
	if (dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_DETAIL_SEARCH_edtTextLang, dlg.m_code);
	}
	
}
//-----

//+++++ 2011.9.16 ä��ȣ [�߰�]
// ���� : ����ñ���û ��ɰ������
// �䱸 : ��� ������ �˻� �ǵ��� ����
// ���� : ��Ŀ���� ���� ���, Ű���带 �������� �����Ѵ�.
void CUserMainBookDetailSearchDlg::OnSetfocusDETAILSEARCHedtTextLang() 
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
    hIME = ::ImmGetContext(GetDlgItem(IDC_DETAIL_SEARCH_edtTextLang)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_ALPHANUMERIC;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
        ImmReleaseContext(GetDlgItem(IDC_DETAIL_SEARCH_edtTextLang)->m_hWnd,hIME); 
    }
}
//----- 
