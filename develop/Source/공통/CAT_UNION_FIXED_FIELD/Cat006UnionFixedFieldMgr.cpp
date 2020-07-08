// Cat006UnionFixedFieldMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Cat006UnionFixedFieldMgr.h"

#include "FixedFieldInfo.h"
#include "..\..\ESL\ESL\DlgClassCode.h"
#include "DLG_006.h"

#include "..\..\����\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat006UnionFixedFieldMgr dialog
CDLG_006 *m_pParent;

CCat006UnionFixedFieldMgr::CCat006UnionFixedFieldMgr(UINT IDD, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat006UnionFixedFieldMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = NULL;
	m_pCM = NULL;
	m_pListInfo = NULL;
	m_pParent = (CDLG_006*)pParent;
}


CCat006UnionFixedFieldMgr::~CCat006UnionFixedFieldMgr()
{

}
void CCat006UnionFixedFieldMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat006UnionFixedFieldMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCat006UnionFixedFieldMgr, CDialog)
	//{{AFX_MSG_MAP(CCat006UnionFixedFieldMgr)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat006UnionFixedFieldMgr message handlers
BOOL CCat006UnionFixedFieldMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


INT CCat006UnionFixedFieldMgr::Init()
{
	if (!m_pMarc)
	{
		m_strLastErrMsg = _T("MARC Pointer�� NULL�Դϴ�.");
		return -1;
	}

	if (!m_pListInfo)
	{
		m_strLastErrMsg = _T("007 Info List Pointer�� NULL�Դϴ�.");
		return -1;
	}

	return 0;
}

INT CCat006UnionFixedFieldMgr::ShowCodeDlg(CString &strGroupName, CString &strItemName, CString &strCode, CString &strControlAlias)
{
	strCode.Empty();

	// ���౹��ȣ
	if (strItemName == _T("���౹��ȣ"))
	{
		m_pCM->GetControlMgrData(strControlAlias, strCode);
		CKOMarcGetterDlg dlg(this);
		dlg.InitSetProperty(0, strCode);
		if (dlg.DoModal() == IDOK)
		{
			strCode = dlg.m_code;
		}
		return 0;
	}
	else if (strItemName == _T("�ѱ����к�ȣ"))
	{
		m_pCM->GetControlMgrData(strControlAlias, strCode);
		CKOMarcGetterDlg dlg(this);
		dlg.InitSetProperty(1, strCode);
		if (dlg.DoModal() == IDOK)
		{
			strCode = dlg.m_code;
		}
		return 0;
	}
	else if (strItemName == _T("���"))
	{
		m_pCM->GetControlMgrData(strControlAlias, strCode);
		CKOMarcGetterDlg dlg(this);
		dlg.InitSetProperty(2, strCode);
		if (dlg.DoModal() == IDOK)
		{
			strCode = dlg.m_code;
		}
		return 0;
	}
	else if (strItemName == _T("���α����ȣ"))
	{
		m_pCM->GetControlMgrData(strControlAlias, strCode);
		CKOMarcGetterDlg dlg(this);
		dlg.InitSetProperty(3, strCode);
		if (dlg.DoModal() == IDOK)
		{
			strCode = dlg.m_code;
		}
		return 0;
	}
/*	else if (strItemName == _T("(�θ��ڹ���ǥ)"))
	{
		m_pCM->GetControlMgrData(strControlAlias, strCode);
		CKOMarcGetterDlg dlg(this);
		dlg.InitSetProperty(3, strCode);
		if (dlg.DoModal() == IDOK)
		{
			strCode = dlg.m_code;
		}
		return 0;
	}
*/
	// Code Dialog�� �����ֱ�
	CFixedFieldInfo *pInfo = NULL;
	POSITION pos = m_pListInfo->GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_pListInfo->GetNext(pos));
		if (!pInfo) continue;

		if (pInfo->m_strGroupName == strGroupName && pInfo->m_strItemName == strItemName)
		{
			// �ڵ� ���̾�α� �����ֱ�
			CString strDesc;
			CDlgClassCode dlg(this);
			dlg.InitData();
			
			POSITION posItem = pInfo->GetHeadPositionItem();
			while (posItem)
			{
				pInfo->GetNextItem(posItem, strCode, strDesc);
				dlg.AddData(strCode, strDesc);
			}
			
			if (dlg.DoModal() == IDOK)
			{
				strCode = dlg.m_strSelCode;
				return 0;
			}
			
			return -400;
		}
	}

	return -1;
}

INT CCat006UnionFixedFieldMgr::CheckCode(CString &strGroupName, CString &strItemName, CString &strCode)
{
	if (strCode.IsEmpty()) return 0;

	CString strOriginCode;
	CString strDesc;
	CFixedFieldInfo *pInfo = NULL;
	POSITION pos = m_pListInfo->GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_pListInfo->GetNext(pos));
		if (!pInfo) continue;

		if (pInfo->m_strGroupName == strGroupName && 
			pInfo->m_strItemName == strItemName)
		{
			// �ڵ尡 �ִ��� üũ
			POSITION posItem = pInfo->GetHeadPositionItem();
			while (posItem)
			{
				pInfo->GetNextItem(posItem, strOriginCode, strDesc);
				if (strCode == strOriginCode)
					return 0;
			}

			return -1;
		}
	}

	return 0;
}

INT CCat006UnionFixedFieldMgr::Display()
{
	INT ids = -1;
	CString strMarcAlias;
	CString strControlMgrAlias;
	CString strItemData;
	POSITION pos = m_mapMarcAlias_ControlAlias.GetStartPosition();
	while (pos)
	{
		m_mapMarcAlias_ControlAlias.GetNextAssoc(pos, strMarcAlias, strControlMgrAlias);
		ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, strMarcAlias, strItemData);
		strItemData.TrimLeft();
		strItemData.TrimRight();
		if (!strItemData.IsEmpty())
			ids = m_pCM->SetControlMgrData(strControlMgrAlias, strItemData);
	}
	return 0;
}

INT CCat006UnionFixedFieldMgr::ValidateControlData(CStringArray *pArrStrControlMgrAlias)
{
	if (!pArrStrControlMgrAlias) return -1;

	CString strMessage;
	CString strControlAlias;
	CString strControlData;
	INT nCount = pArrStrControlMgrAlias->GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		strControlAlias = pArrStrControlMgrAlias->GetAt(idx);

		m_pCM->GetControlMgrData(strControlAlias, strControlData);
		if (strControlData.IsEmpty())
		{
			strMessage += _T("[") + strControlAlias + _T("] ");
		}
	}

	if (!strMessage.IsEmpty())
	{
		strMessage += _T("�׸��� �Է����ּ���");
		AfxMessageBox(strMessage);
		return -1;
	}

	return 0;
}

INT CCat006UnionFixedFieldMgr::ApplyControlData()
{
	INT ids = -1;
	CString strMarcAlias;
	CString strControlMgrAlias;
	CString strItemData;
	POSITION pos = m_mapMarcAlias_ControlAlias.GetStartPosition();

	// �ڷ���ִ� �������� �⺻ ǥ���Ѵ�.
	m_pParent->m_MultiComboBox.GetData(strItemData);
	ids = m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("006@0"), strItemData, _T(""), ADD_SUBFIELD);
	
	while (pos)
	{		
		m_mapMarcAlias_ControlAlias.GetNextAssoc(pos, strMarcAlias, strControlMgrAlias);

		ids = m_pCM->GetControlMgrData(strControlMgrAlias, strItemData);
		if (ids >= 0)
			ids = m_pInfo->pMarcMgr->SetItem(m_pMarc, strMarcAlias, strItemData, _T(""), ADD_SUBFIELD);
	}
	return 0;
}

INT CCat006UnionFixedFieldMgr::GetItemNameFromControlHwnd(HWND hWnd, CString &strItemName, CString &strControlAlias)
{
	if (!hWnd) return -1;

	INT ids = -1;
	CWnd *pWnd = NULL;
	CString strItemData;
	POSITION pos = m_mapControlAlias_ItemName.GetStartPosition();
	while (pos)
	{		
		m_mapControlAlias_ItemName.GetNextAssoc(pos, strControlAlias, strItemName);

		pWnd = (CWnd*)m_pCM->FindControl(strControlAlias);
		if (!pWnd) continue;

		if (pWnd->GetSafeHwnd() == hWnd)
			return 0;
	}

	return -1;
}

INT CCat006UnionFixedFieldMgr::Get006()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCat006UnionFixedFieldMgr message handlers

BOOL CCat006UnionFixedFieldMgr::PreTranslateMessage(MSG* pMsg) 
{
	if ( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9) || (pMsg->wParam == MK_RBUTTON) )
	{
		CString strCode;
		CString strItemName;
		CString strControlAlias;
		
		INT ids = GetItemNameFromControlHwnd(pMsg->hwnd, strItemName, strControlAlias);
		if (ids >= 0)
		{
			ids = ShowCodeDlg(m_strGroupName, strItemName, strCode, strControlAlias);
			if (ids >= 0)
			{
				m_pCM->SetControlMgrData(strControlAlias, strCode);
				CObject *obj;
				obj = m_pCM->FindControl(strControlAlias);
				((CEdit*)obj)->SetFocus();
			}	
			
			return TRUE;
		}
	}
	else if (pMsg->message == WM_KEYUP)
	{
		CString strCode;
		CString strItemName;
		CString strControlAlias;

		INT ids = GetItemNameFromControlHwnd(pMsg->hwnd, strItemName, strControlAlias);
		if (ids >= 0)
		{
			ids = m_pCM->GetControlMgrData(strControlAlias, strCode);
			// Ư���� ��Ʈ�� üũ
			// VM �󿵽ð�
			if ( strControlAlias==_T("�󿵽ð�") )
			{
				// ����üũ �� 0���� ���ڸ� ä���ֱ�
				CString strNum;
				INT nNum;
				strCode.TrimLeft(_T('0'));
				nNum = _ttoi(strCode.GetBuffer(0));
				if ( nNum <= 0 )				
				{
					ids = CheckCode(m_strGroupName, strItemName, strCode);			
					if (ids < 0)
					{
						AfxMessageBox(_T("�߸��� ���Դϴ�."));
						m_pCM->SetControlMgrData(strControlAlias, _T(""));
						((CWnd*)m_pCM->FindControl(strControlAlias))->SetFocus();
						return TRUE;
					}
				}				
			}	
			else
			{
				ids = CheckCode(m_strGroupName, strItemName, strCode);			
				if (ids < 0)
				{
					AfxMessageBox(_T("�߸��� ���Դϴ�."));
					m_pCM->SetControlMgrData(strControlAlias, _T(""));
					((CWnd*)m_pCM->FindControl(strControlAlias))->SetFocus();
					return TRUE;
				}
			}			
		}
	}
	
	// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
	// ���ո�ũ ���� ����
	// ����Ʈ�ڽ����� ESCŰ�� ������ ���� ȭ���� ������� ���� ����
/*//NEW------------------------------------------------------------------------*/
	else if (WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam) {
		return m_pParent->PreTranslateMessage(pMsg);
	}
/*//END------------------------------------------------------------------------*/

	return CDialog::PreTranslateMessage(pMsg);
}
