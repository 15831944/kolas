// Cat008FixedFieldMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Cat008FixedFieldMgr.h"

#include "FixedFieldInfo.h"
#include "..\..\ESL\ESL\DlgClassCode.h"

#include "..\..\����\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedFieldMgr dialog


CCat008FixedFieldMgr::CCat008FixedFieldMgr(UINT IDD, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat008FixedFieldMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_lpsz008InfoFileName = _T("..\\CFG\\008TAG�ڵ�.txt");
	m_lpszLeaderInfoFileName = _T("..\\CFG\\�����ڵ�.txt");

	m_pMarc = NULL;
	m_pCM = NULL;
}

CCat008FixedFieldMgr::~CCat008FixedFieldMgr()
{
	FreeData();
}

VOID CCat008FixedFieldMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat008FixedFieldMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

VOID CCat008FixedFieldMgr::FreeData()
{
	CFixedFieldInfo *pInfo = NULL;

	// 008 �����
	POSITION pos = m_listInfo.GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_listInfo.GetNext(pos));
		delete pInfo;
	}
	m_listInfo.RemoveAll();
}

BEGIN_MESSAGE_MAP(CCat008FixedFieldMgr, CDialog)
	//{{AFX_MSG_MAP(CCat008FixedFieldMgr)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedFieldMgr Operations

INT CCat008FixedFieldMgr::Init()
{
	if (!m_pMarc)
	{
		m_strLastErrMsg = _T("MARC Pointer�� NULL�Դϴ�.");
		return -1;
	}
	
	INT ids = -1;
	FreeData();
	ids  = LoadInfo(m_lpsz008InfoFileName);
	if (ids < 0) return ids;
	ids = LoadInfo(m_lpszLeaderInfoFileName);
	if (ids < 0) return ids;

	return 0;
}

INT CCat008FixedFieldMgr::LoadInfo(LPCTSTR lpszFileName)
{
	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary)) return -1;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT nFind = -1;
	CString strLine;
	CString strGroupName;
	DWORD dwPos = 0;
	CFixedFieldInfo *pInfo = NULL;
	while (file.ReadString(strLine))
	{
		//strLine.TrimLeft();
		//strLine.TrimRight();

		if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;

		// Group
		if (strLine.Left(1) == _T("["))
		{
			// Position ����
			dwPos = file.GetPosition();

			strGroupName = strLine;
			// Item
			while (file.ReadString(strLine))
			{
				//strLine.TrimLeft();
				//strLine.TrimRight();
				if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
				if (strLine.Left(1) == _T("[")) 
				{
					// Position ��������
					file.Seek(dwPos, CFile::begin);
					break;
				}
				if (strLine.Left(1) == _T("("))
				{
					// Position ����
					dwPos = file.GetPosition();

					// Item �߰�
					pInfo = new CFixedFieldInfo(strGroupName, strLine);
					m_listInfo.AddTail(pInfo);

					// Code, Desc �ֱ�
					while (file.ReadString(strLine))
					{
						//strLine.TrimLeft();
						//strLine.TrimRight();
						if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
						if (strLine.Left(1) == _T("(") || strLine.Left(1) == _T("[")) 
						{
							// Position ��������
							file.Seek(dwPos, CFile::begin);
							break;
						}

						nFind = strLine.Find(_T(":"));
						if (nFind < 0) continue;

						pInfo->AddItem(strLine.Mid(0, nFind), strLine.Mid(nFind + 1));
					}
				}
			}
		}		
	}

	file.Close();
	return 0;
}

INT CCat008FixedFieldMgr::ShowCodeDlg(CString &strGroupName, CString &strItemName, CString &strCode, CString &strControlAlias)
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
	POSITION pos = m_listInfo.GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_listInfo.GetNext(pos));
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

INT CCat008FixedFieldMgr::CheckCode(CString &strGroupName, CString &strItemName, CString &strCode)
{
	if (strCode.IsEmpty()) return 0;

	CString strOriginCode;
	CString strDesc;
	CFixedFieldInfo *pInfo = NULL;
	POSITION pos = m_listInfo.GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_listInfo.GetNext(pos));
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

INT CCat008FixedFieldMgr::Display()
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

INT CCat008FixedFieldMgr::ValidateControlData(CStringArray *pArrStrControlMgrAlias)
{
	if (!pArrStrControlMgrAlias) return -1;

	CString strMessage;
	CString strControlAlias;
	CString strControlData;
	INT nCount = pArrStrControlMgrAlias->GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		/*
		strControlAlias = pArrStrControlMgrAlias->GetAt(idx);
        
		m_pCM->GetControlMgrData(strControlAlias, strControlData);
		if (strControlData.IsEmpty())
		{
			strMessage += _T("[") + strControlAlias + _T("] ");
		}
		*/
			strControlAlias = pArrStrControlMgrAlias->GetAt(idx);
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.13 ������ ����.
		// �������� �Է¿��� ������ n�̸� �����1�� �Է����� �ʾƵ� ������ �Ǿ�� �Ѵ�.
		// 
			
		bool bPublishYearForm = false ;

        if(m_pCM->CM_ALIAS == _T("CM_BO_CAT_PROC_0323") || m_pCM->CM_ALIAS == _T("CM_BO_CAT_PROC_0324") )
		{
			m_pCM->GetControlMgrData(_T("����"), strControlData);
			//if(strControlData == _T("n") || strControlData == _T("m") )
			if( strControlData != _T("s") )
			{
				bPublishYearForm = true ;
			}
		}
        else
		{
			m_pCM->GetControlMgrData(_T("���������"), strControlData);
			//if(strControlData == _T("n") || strControlData == _T("m") )
			if( strControlData != _T("s") )
			{
				bPublishYearForm = true ;
			}
		}

		m_pCM->GetControlMgrData(strControlAlias, strControlData);
		if(strControlAlias== _T("�����1") && bPublishYearForm == true )
		{
			continue ;
		}
		else if (strControlData.IsEmpty())
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

INT CCat008FixedFieldMgr::ApplyControlData()
{
	INT ids = -1;
	// 008 �� �����.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("008"));

	CString strMarcAlias;
	CString strControlMgrAlias;
	CString strItemData;
	POSITION pos = m_mapMarcAlias_ControlAlias.GetStartPosition();
	while (pos)
	{		
		m_mapMarcAlias_ControlAlias.GetNextAssoc(pos, strMarcAlias, strControlMgrAlias);

		ids = m_pCM->GetControlMgrData(strControlMgrAlias, strItemData);
		if (ids >= 0)
			ids = m_pInfo->pMarcMgr->SetItem(m_pMarc, strMarcAlias, strItemData);
	}
	return 0;
}

INT CCat008FixedFieldMgr::GetItemNameFromControlHwnd(HWND hWnd, CString &strItemName, CString &strControlAlias)
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

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedFieldMgr message handlers

BOOL CCat008FixedFieldMgr::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9 || pMsg->wParam == MK_RBUTTON)
	{
		CString strCode;
		CString strItemName;
		CString strControlAlias;
		
		INT ids = GetItemNameFromControlHwnd(pMsg->hwnd, strItemName, strControlAlias);
		if (ids >= 0)
		{
			ids = ShowCodeDlg(m_strGroupName, strItemName, strCode, strControlAlias);
			// �ݱ�(Cancle)��ư ������ ���Ͱ��� -400 (�������� 2004.10.08)
			if ((ids < 0) && (ids != -400) )
				ids = ShowCodeDlg(CString(_T("[AV]")), strItemName, strCode, strControlAlias);	// ���Ͽ� ����.. AV�� �����ؼ�..��.��;
			if (ids >= 0)
			{
				m_pCM->SetControlMgrData(strControlAlias, strCode);
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
	
	return CDialog::PreTranslateMessage(pMsg);
}
