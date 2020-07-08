// Cat008UnionFixedFieldMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Cat008UnionFixedFieldMgr.h"

#include "FixedFieldInfo.h"
#include "..\..\ESL\ESL\DlgClassCode.h"

#include "..\..\����\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat008UnionFixedFieldMgr dialog


CCat008UnionFixedFieldMgr::CCat008UnionFixedFieldMgr(UINT IDD, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat008UnionFixedFieldMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_lpsz008InfoFileName = _T("..\\CFG\\UNION_008TAG�ڵ�.txt");
	m_lpszLeaderInfoFileName = _T("..\\CFG\\UNION_�����ڵ�.txt");

	m_pMarc = NULL;
	m_pCM = NULL;
}

CCat008UnionFixedFieldMgr::~CCat008UnionFixedFieldMgr()
{
}


void CCat008UnionFixedFieldMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat008UnionFixedFieldMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


VOID CCat008UnionFixedFieldMgr::FreeData()
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

BEGIN_MESSAGE_MAP(CCat008UnionFixedFieldMgr, CDialog)
	//{{AFX_MSG_MAP(CCat008UnionFixedFieldMgr)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat008UnionFixedFieldMgr Operations

INT CCat008UnionFixedFieldMgr::Init()
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

INT CCat008UnionFixedFieldMgr::LoadInfo(LPCTSTR lpszFileName)
{
	CStdioFile file;
	// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
	// ���ո�ũ ���� ����
/*//BEFORE-----------------------------------------------------------------------
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary)) return -1;
*///AFTER------------------------------------------------------------------------
	CString strFileName = lpszFileName;
	CString strGetGroupName = _T("");
	CString strGroupFileName = _T("..\\cfg\\MarcRelation\\���ո�ũ_����_�׷��.cfg");
	CFileFind find;
	if (TRUE == find.FindFile(strGroupFileName)) {
		CStdioFile file;
		if (TRUE == file.Open(strGroupFileName, CFile::modeRead|CFile::typeBinary)) {
			TCHAR cUni;
			file.Read(&cUni, sizeof(TCHAR));
			if (0xFEFF != cUni) file.SeekToBegin();
			CString strTempData, strGetData;
			while (file.ReadString(strGetData)) {
				strGetData.TrimLeft();
				if (strGetData.IsEmpty()) continue;
				if (_T("#") == strGetData.Left(1)) continue;
				INT nPos = strGetData.Find(_T("="));
				if (0 <= nPos) {
					strTempData = strGetData.Left(nPos);
					strTempData.TrimLeft(); strTempData.TrimRight();
					if (_T("�׷��") == strTempData) {
						strGetGroupName = strGetData.Mid(nPos+1, strGetData.GetLength());
						strGetGroupName.TrimLeft(); strGetGroupName.TrimRight();
						break;
					}
				}
			}
			file.Close ();
		}
		find.Close();
	}
	if (!strGetGroupName.IsEmpty()) {
		if (_T("..\\CFG\\UNION_008TAG�ڵ�.txt") == lpszFileName) {
			strFileName = _T("..\\CFG\\UNION_008TAG�ڵ�_") + strGetGroupName +_T(".txt");
		} else {
			strFileName = _T("..\\CFG\\UNION_�����ڵ�_") + strGetGroupName +_T(".txt");
		}	
	}
	if (!file.Open(strFileName, CFile::modeRead | CFile::typeBinary)) return -1;
/*//END------------------------------------------------------------------------*/
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT nFind = -1;
	CString strLine;
	CString strGroupName;
	CString strTmp;
	DWORD dwPos = 0;
	CFixedFieldInfo *pInfo = NULL;
	while (file.ReadString(strLine))
	{		
		strLine.TrimRight();

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
				strLine.TrimRight();
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
						strLine.TrimRight();
						if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
						if (strLine.Left(1) == _T("(") || strLine.Left(1) == _T("[")) 
						{
							// Position ��������
							file.Seek(dwPos, CFile::begin);
							break;
						}

						nFind = strLine.Find(_T(":"));
						if (nFind < 0) continue;

						strTmp = strLine.Mid(0, nFind);
						strTmp.Replace(_T("~"), _T("|"));


						pInfo->AddItem(strTmp, strLine.Mid(nFind + 1));
					}
				}
			}
		}		
	}

	file.Close();
	return 0;
}

INT CCat008UnionFixedFieldMgr::ShowCodeDlg(CString &strGroupName, CString &strItemName, CString &strCode, CString &strControlAlias)
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

INT CCat008UnionFixedFieldMgr::CheckCode(CString &strGroupName, CString &strItemName, CString &strCode)
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

INT CCat008UnionFixedFieldMgr::Display()
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

INT CCat008UnionFixedFieldMgr::ValidateControlData(CStringArray *pArrStrControlMgrAlias)
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

INT CCat008UnionFixedFieldMgr::ApplyControlData()
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

INT CCat008UnionFixedFieldMgr::GetItemNameFromControlHwnd(HWND hWnd, CString &strItemName, CString &strControlAlias)
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
// CCat008UnionFixedFieldMgr message handlers

BOOL CCat008UnionFixedFieldMgr::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9 || pMsg->wParam == MK_RBUTTON)
	{
		CString strCode;
		CString strItemName;
		CString strControlAlias;
		
		INT ids = GetItemNameFromControlHwnd(pMsg->hwnd, strItemName, strControlAlias);
		if (ids >= 0)
		{
			if ( strControlAlias.GetLength() > 2 && strControlAlias.Left(2) == _T("����") )
			{
				CString strTemp;
				strTemp = "[LEADER]";
				ids = ShowCodeDlg(strTemp, strItemName, strCode, strControlAlias);
			}
			else
			{
				ids = ShowCodeDlg(m_strGroupName, strItemName, strCode, strControlAlias);
			}			
			// �ݱ�(Cancle)��ư ������ ���Ͱ��� -400 (�������� 2004.10.08)
			if ((ids < 0) && (ids != -400) )
				ids = ShowCodeDlg(CString(_T("[AV]")), strItemName, strCode, strControlAlias);	// ���Ͽ� ����.. AV�� �����ؼ�..��.��;
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
			if ( m_strGroupName == _T("[VM]") && strControlAlias==_T("�󿵽ð�") )
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
				// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
				// ���ո�ũ ���� ����
				// 008 ���� �κ� ���ؽ� üũ ���ϴ� ���� ����
/*//BEFORE-----------------------------------------------------------------------
				ids = CheckCode(m_strGroupName, strItemName, strCode);			
*///AFTER------------------------------------------------------------------------
				if (2 < strControlAlias.GetLength() && _T("����") == strControlAlias.Left(2)) {
					CString strTemp;
					strTemp = "[LEADER]";
					ids = CheckCode(strTemp, strItemName, strCode);
				} else {
					ids = CheckCode(m_strGroupName, strItemName, strCode);
				}
/*//END------------------------------------------------------------------------*/		
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
	
	return CDialog::PreTranslateMessage(pMsg);
}
