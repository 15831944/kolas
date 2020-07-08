// ESL_Mgr.cpp : implementation file
//

#include "stdafx.h"
#include "ESL_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_ControlMultiComboBox.h"
#include "ESL_Grid.h"
#include "ESLDlg_DM_Viewer.h"
#include "efs.h"
#include <Htmlhelp.h>
#include "FrameManager.h"

/////////////////////////////////////////////////////////////////////////////
// CESL_Mgr dialog


CESL_Mgr::CESL_Mgr(UINT IDD, CESL_Mgr* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	pParentMgr = pParent;

 	m_pInfo = NULL;
	if (pParent && pParent->m_pInfo) {
		m_pInfo = pParent->m_pInfo;
	}

	CONNECTION_INFO = _T("");
	
	m_bkR = 205;
	m_bkG = 229;
	m_bkB = 255;
	m_BkBrush.CreateSolidBrush(RGB(m_bkR, m_bkG, m_bkB));

	SM_STYLE = 0;
	if (pParent) {
		pMain = pParent->pMain;
		SM_STYLE = pParent->SM_STYLE;
		VIEW_STYLE = pParent->VIEW_STYLE;
	} else {
		VIEW_STYLE = 'U';
		pMain = NULL;
	}
	pDVM = new CESL_DataVerifyMgr(this);

	m_workSheet = NULL;
	m_WorkSheetRect.left = 0;
	m_WorkSheetRect.top = 0;
	m_WorkSheetRect.right = 500;
	m_WorkSheetRect.bottom = 500;

	m_nParentFrameType = 0;	
}

CESL_Mgr::~CESL_Mgr(VOID)
{
	FreeDMList();
	FreeCMList();
	FreeREList();

	delete pDVM;

	// ���� �߰�
	if( NULL != m_workSheet )
	{
		delete m_workSheet;
		m_workSheet = NULL;
	}
}

VOID CESL_Mgr::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESL_Mgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CESL_Mgr, CDialog)
	//{{AFX_MSG_MAP(CESL_Mgr)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(6000, 6000, OnShowDMViewer)
	ON_MESSAGE(WM_USER+1, OnShowDMViewer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESL_Mgr message handlers

BOOL CESL_Mgr::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE ) {
		if (pParentMgr == NULL)	{
			return FALSE;
		}
	}
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN ) return FALSE;
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F8 ) {
		OnShowDMViewer(0);
		return TRUE;		
	}
	if ( pMsg->message == WM_KEYDOWN && GetKeyState(VK_CONTROL) < 0 && pMsg->wParam == VK_F2 ) {
		if (m_nParentFrameType == 0) {
			if (pMain) pMain->PostMessage(WM_USER+2005);
			return TRUE;
		}
		return TRUE;		
	}
	if ( pMsg->message == WM_KEYDOWN && GetKeyState(VK_CONTROL) < 0 && pMsg->wParam == VK_F3 ) {
		if (m_nParentFrameType == 0) {
			if (pMain) pMain->PostMessage(WM_USER+2006);
			return TRUE;
		}
		return TRUE;		
	}

	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F1 ) {
		OnShowHtmlHelp();
		return TRUE;		
	}
	if ( pMsg->message == WM_DESTROY)
	{
		INT merong = 0;
	}
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if (Acceclerator(pMsg->wParam) >= 0)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}
//=================================================
//2008.09.26 ADD BY PJW : F4�� ������ TUNING���� ���۴��̾�α׸� ����.
VOID CESL_Mgr::OnShowFtpTransfer(UINT  nIDC)
{
EFS_BEGIN
 	CString strArgu;
	strArgu.Format(_T("%s/%s"),m_pInfo->TITLE,m_pInfo->USER_NAME);
 	ShellExecute(NULL, _T("open"), _T("..\\bin\\LogZipFtp.exe"), strArgu, NULL, NULL);
	return;
EFS_END
return;
}
//=================================================
/*
INT CESL_Mgr::Acceclerator(INT nChar)
{
EFS_BEGIN

	if (nChar == 17) return -1;
	CString strKey;
	// CTRL KEY
	if (GetKeyState(VK_CONTROL) < 0 && nChar != 17)
	{
		strKey.Format(_T("CTRL+%c"), nChar);
	}
	// ALT KEY
	else if (GetKeyState(VK_MENU) < 0 && nChar != 18)
	{
		strKey.Format(_T("ALT+%c"), nChar);
	}
	else
		return -1;

	CESL_Control_Element *pCE = GetAccelerateControl(strKey);
	if (pCE == NULL) return -1;

	// TOOLBAR ���� BUTTON���� äũ
	if (pCE->CTRL_TYPE == _T("T"))
	{
		// ��� ȭ���̸� �����Ѵ�.
		if (m_nFlags & WF_MODALLOOP) return -1;
		m_strExeToolBarAlias = pCE->CTRL_ALIAS;

		if (pMain != NULL)
			pMain->PostMessage(WM_TOOLBAR_EXE, (WPARAM)&m_strExeToolBarAlias, 0);
	}
	else if(pCE->CTRL_TYPE == _T("B"))
	{
		PostMessage(WM_COMMAND, pCE->CTRL_ID, BN_CLICKED);
	}

	return 0;

EFS_END
return -1;

}
*/
INT CESL_Mgr::Acceclerator(INT nChar)
{
EFS_BEGIN
	if (nChar == 17) return -1;
	CString strKey;
	// CTRL KEY
	if (GetKeyState(VK_CONTROL) < 0 && nChar != 17)
	{
		strKey.Format(_T("CTRL+%c"), nChar);
	}
	// ALT KEY
	else if (GetKeyState(VK_MENU) < 0 && nChar != 18)
	{
		strKey.Format(_T("ALT+%c"), nChar);
	}
	else
		return -1;

	if (strKey == _T("CTRL+C")) return -1;
	if (strKey == _T("CTRL+V")) return -1;
	if (strKey == _T("CTRL+X")) return -1;
	if (strKey == _T("CTRL+Z")) return -1;

	CESL_Control_Element *pCE = GetAccelerateControl(strKey);
	if (pCE) {
		// �Ϲ� BUTTON�ϰ��� ����
		if(pCE->CTRL_TYPE == _T("B"))
		{
			if (pCE->CTRL_ID >=0) {
				PostMessage(WM_COMMAND, pCE->CTRL_ID, BN_CLICKED);
			} else {
				INT nID = pCE->CTRL_ID * -1;
				CWnd *pWnd = GetDlgItem(nID);
				if (pWnd) pWnd->SetFocus();
			}
			return 0;
		}
	}
	// ���� ��ư���� üũ
	// ��� ȭ���̸� �����Ѵ�.
	if (m_nFlags & WF_MODALLOOP) return -1;
	//m_strExeToolBarAlias = pCE->CTRL_ALIAS;
	m_strExeToolBarAcce = strKey;

	if (m_nParentFrameType == 0) {
		if (pMain != NULL) {
			pMain->PostMessage(WM_TOOLBAR_EXE, (WPARAM)m_strExeToolBarAcce.GetBuffer(0), 0);
			return 0;
		}
	}
	return 0;
EFS_END
return -1;

}

CESL_Control_Element *CESL_Mgr::GetAccelerateControl(CString strKey)
{
EFS_BEGIN

	CESL_ControlMgr *pCM = NULL;
	CESL_Control_Element *pCE = NULL;

	POSITION posCE = NULL;
	POSITION posCM = CMList.GetHeadPosition();
	while (posCM)
	{
		pCM = (CESL_ControlMgr*)CMList.GetNext(posCM);
		if (pCM == NULL) continue;

		posCE = pCM->Control_List.GetHeadPosition();
		while (posCE)
		{
			pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(posCE);
			if (pCE == NULL) continue;

			if (pCE->CTRL_TYPE != _T("B") && pCE->CTRL_TYPE != _T("T")) continue;

			if (pCE->INIT_VALUE == strKey) 
			{
				return pCE;
			}
		}		
	}

	return NULL;

EFS_END
return NULL;

}

CString CESL_Mgr::GetSMAlias()
{
EFS_BEGIN

	return (SM_ALIAS);

EFS_END
return _T("");

}

VOID CESL_Mgr::SetSMAlias(CString alias)
{
EFS_BEGIN

	SM_ALIAS = alias;
	return;

EFS_END
}

// �ش� SM Alias�� �������� Reference ������ �а� SM�� Init �Ѵ�.
// Return Values
//		-1001	:		SM Reference ������ ���� ����
//		-1002	:		SM Sub Reference ������ ���� ����
//		-1003	:		Reference DB �ʱ� ���� ������ ����
//		0		:		����
INT CESL_Mgr::InitESL_Mgr(CString alias)
{
EFS_BEGIN

	if (CONNECTION_INFO.GetLength() < 0) return -1003;
	SetSMAlias(alias);
	CString strFileName;
	strFileName.Format(_T("..\\CFG\\REFERENCE\\%s.SM"), alias);
	if (GetSMSubReferenceFromFile(strFileName) < 0) return -1002;

	// SM�� Readonly �Ӽ��ΰ��  ReadOnly�� �Ѵ�.
	if (SM_STYLE == 'R') SetSMReadOnly();

	SetProgramPath(_T("SM"), SM_ALIAS);	

	return 0;

EFS_END
return -1;
}

INT CESL_Mgr::GetSMSubReferenceFromFile(CString strSMCFG)
{
EFS_BEGIN
	CStdioFile fd;
	if (fd.Open(strSMCFG, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0) return -1;	// ���������� ã�� �� ����
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	INT nPos;
	
	CESL_DataMgr * newDM;
	CESL_ControlMgr * newCM;

	INT nInitCount = 0;
	CString strLine, strTag, strData;
	while(TRUE) {
		if (fd.ReadString(strLine) == FALSE) break;
		strLine.TrimLeft();			strLine.TrimRight();
		if (strLine.GetLength() == 0) continue;
		nPos = strLine.Find('=', 0);
		if (nPos == -1) continue;

		strTag = strLine.Mid(0, nPos);
		strTag.TrimLeft();				strTag.TrimRight();
		strTag.MakeUpper();
		strData = strLine.Mid(nPos+1);
		strData.TrimLeft();				strData.TrimRight();

		newDM = NULL;
		newCM = NULL;
		if (strTag == _T("DM")) {		// DataMgr �� ����� ó��
			newDM = new CESL_DataMgr;
			if (!newDM) continue;
			newDM->SetCONNECTION_INFO(CONNECTION_INFO);
			if(m_pInfo != NULL) newDM->MANAGE_CODE = m_pInfo->MANAGE_CODE;
			newDM->InitDMPKFromFile(strData);
			// ���� ��ũ �Ŵ����� DM�� �Ѱ��ش�.			

			//2004-03-22 [������ ����]
			//==================================================
			if(m_pInfo != NULL)
			{			
				newDM->SetMarcMgr(m_pInfo->pMarcMgr);				
			}
            //==================================================

			//newDM->SetMarcMgr(m_pInfo->pMarcMgr);
			DMList.AddTail((CObject*)newDM);
			nInitCount++;
		}
		if (strTag == _T("CM")) {		// ControlMgr�� ����� ó��
			newCM = new CESL_ControlMgr(this);
			if (!newCM) continue;

			newCM->CONNECTION_INFO = CONNECTION_INFO;
			newCM->LoadReferenceFromFile(strData);
			
			CMList.AddTail((CObject*)newCM);
			CollectRequireControl(newCM);
			nInitCount++;
		}		
	}
	fd.Close();
	return nInitCount;
EFS_END
return -1;
}

INT CESL_Mgr::CollectRequireControl(CESL_ControlMgr *pCM)
{
EFS_BEGIN

	INT cecount = pCM->Control_List.GetCount();
	if (cecount == 0) return false;
	INT * pINT = NULL;
	CESL_Control_Element *pCE;
	POSITION pos;
	pos = pCM->Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element *)pCM->Control_List.GetNext(pos);
		if (pCE->REQUIRECHECK.GetLength() > 0) {
			if (pCE->CTRL_TYPE == _T("E") || pCE->CTRL_TYPE == _T("M")) {
				pINT = NULL;
				pINT = new INT;
				*pINT = pCE->CTRL_ID;
				REList.AddTail((CObject*)pINT);
			}
		}
	}
	return false;

EFS_END
return -1;

}

VOID CESL_Mgr::SetCONNECTION_INFO(CString info)
{
EFS_BEGIN

	if (info.GetLength() == 0) return;
	CONNECTION_INFO = info;

EFS_END
}

INT CESL_Mgr::FreeDMList(VOID)
{
EFS_BEGIN

	INT dmcount = DMList.GetCount();
	if (dmcount == 0) return 0;
	CESL_DataMgr * pDm;
	for (INT i = 0; i < dmcount; i++) {
		pDm = (CESL_DataMgr*)DMList.RemoveHead();
		if (pDm) delete pDm;
	}
	DMList.RemoveAll();
	return(dmcount);

EFS_END
return -1;

}

INT CESL_Mgr::FreeCMList(VOID)
{
EFS_BEGIN

	INT cmcount = CMList.GetCount();
	if (cmcount == 0) return 0;
	CESL_ControlMgr * pCm;
	for (INT i = 0; i < cmcount; i++) {
		pCm = (CESL_ControlMgr*)CMList.RemoveHead();
		if (pCm) delete pCm;
	}
	CMList.RemoveAll();
	return(cmcount);

EFS_END
return -1;

}

INT CESL_Mgr::FreeREList(VOID)
{
EFS_BEGIN

	INT recount = REList.GetCount();
	if (recount == 0) return 0;
	INT * pInt;
	for (INT i = 0; i < recount; i++) {
		pInt = (INT*)REList.RemoveHead();
		if (pInt) delete pInt;
	}
	REList.RemoveAll();
	return(recount);

EFS_END
return -1;

}

// ALIAS�� �������� DataMgr�� �����͸� ��ȯ
// Return Values
//	NULL	:		�߸��� Alias Ȥ�� ���� Alias
//	NOT NULL:		CESL_DataMgr�� ������
CESL_DataMgr * CESL_Mgr::FindDM(CString alias)
{
EFS_BEGIN

	INT dmcount = DMList.GetCount();
	CESL_DataMgr * pDm = NULL;
	POSITION pos;
	pos = DMList.GetHeadPosition();
	for(INT i = 0; i < dmcount; i++) {
		pDm = NULL;
		pDm = (CESL_DataMgr*)DMList.GetNext(pos);
		if (pDm) {
			if (pDm->GETDMAlias() == alias) return pDm;
		}
	}
	// ���� Dialog�� ���� ��� ���� ���̾� �α׿��� ã�´�.
	if (pParentMgr) {
		return (pParentMgr->FindDM(alias));
	}
	return (NULL);

EFS_END
return NULL;

}

/*
@Create
 - 2010.02.02 ADD BY PWR 
@Description
 - DM�� Ư�� ROW�� �����Ѵ�.
@Parameter
 pSrcDM : �����ؿ��� DM
 nSrcRow : �����ؿ��� ROW
 pDstDM : ����� DM
 nDstRow : ����� ROW (-1�̸� �������� �߰�)
@Return Value
 -1000 : DM NULL
 -1010 : �����ؿ��� ROW�� �̻�
 -1020 : ����� ROW�� �̻�
@Mini Spec
 1. 
*/
INT CESL_Mgr::CopyRow(CESL_DataMgr* pSrcDM, INT nSrcRow, CESL_DataMgr* pDstDM, INT nDstRow/*=-1*/)
{
EFS_BEGIN

	if(NULL == pSrcDM || NULL == pDstDM)
	{
		return -1000;
	}
	if(0 > nSrcRow || pSrcDM->GetRowCount() <= nSrcRow)
	{
		return -1010;
	}
	if(pDstDM->GetRowCount() <= nDstRow)
	{
		return -1020;
	}

	DWORD* row = (DWORD*)pSrcDM->data.GetAt(pSrcDM->m_pDataPosition[nSrcRow]);
	pDstDM->data.AddTail((CObject*)row);
 	if(0 > nDstRow)
 	{
 		pDstDM->data.AddTail((CObject*)row);
 	}
 	else
 	{
 		POSITION pos = pDstDM->data.FindIndex(nDstRow);
 		pDstDM->data.InsertBefore(pos, (CObject*)row);
 	}

	pDstDM->RefreshPositionInfo();

	return 0;

EFS_END
return -1;
}

BOOL CESL_Mgr::DestroyWindow() 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();

EFS_END
return FALSE;

}

// ALIAS�� �������� ControlMgr�� �����͸� ��ȯ
// Return Values
//	NULL	:		�߸��� Alias Ȥ�� ���� Alias
//	NOT NULL:		CESL_ControlMgr�� ������
CESL_ControlMgr * CESL_Mgr::FindCM(CString alias)
{
EFS_BEGIN

	INT cmcount = CMList.GetCount();
	if (cmcount > 0) {
		CESL_ControlMgr * pCm = NULL;
		POSITION pos;
		pos = CMList.GetHeadPosition();
		for(INT i = 0; i < cmcount; i++) {
			pCm = NULL;
			pCm = (CESL_ControlMgr*)CMList.GetNext(pos);
			if (pCm) {
				if (pCm->GetAlias() == alias) return pCm;
			}
		}
	}
	// ���� Dialog�� ���� ��� ���� ���̾� �α׿��� ã�´�.
	if (pParentMgr) {
		return (pParentMgr->FindCM(alias));
	}
	return (NULL);

EFS_END
return NULL;

}

/*
@Create
 - 2009.04.01 BY CJY
@Description
 - SM�� ã�´�
@Parameter
 - strAlias[IN] : 
@Return Value
 - CESL_Mgr*  
@Mini Spec
 1. 
*/
CESL_Mgr* CESL_Mgr::FindSM(CString strAlias)
{
	if(strAlias == SM_ALIAS)
	{
		return (this);
	}

	if(NULL == pParentMgr)	
	{
		return (NULL);
	}
	return pParentMgr->FindSM(strAlias);
}
// Control Mgr�� Control�� ����Ÿ�� �����´�.
// Return values 
// 0		: ����
// -1006	: Control Mgr�� ã�� �� ����
INT CESL_Mgr::GetControlData(CString mgralias, CString ctrlalias, CString &data, INT index, INT col)
{
EFS_BEGIN

	CESL_ControlMgr * p = FindCM(mgralias);
	if (!p) return -1;		// �ش� Ctrl Mgr�� ã�� �� ����
	return p->GetControlMgrData(ctrlalias, data, index, col);

EFS_END
return -1;

}

// Control Mgr�� Control�� ����Ÿ�� �����Ѵ�.
// Return values 
// 0		: ����
// -1006	: Control Mgr�� ã�� �� ����
INT CESL_Mgr::SetControlData(CString mgralias, CString ctrlalias, CString data, INT index, INT col)
{
EFS_BEGIN

	CESL_ControlMgr * p = FindCM(mgralias);
	if (!p) return -1006;		// �ش� Ctrl Mgr�� ã�� �� ����
	return p->SetControlMgrData(ctrlalias, data, index, col);

EFS_END
return -1;

}

INT CESL_Mgr::MultiComboControlSetData(CString mgralias, CString ctrlalias, TCHAR **data, INT ndatas, INT index)
{
EFS_BEGIN

	return -999;	// ���� �����Ǿ� ���� ����

EFS_END
return -1;

}

// Return values 
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1007		: �ش� Control Element�� ã�� �� ����
// -1008		: �ش� Control�� �ʱ�ȭ ���� �ʾ���
// 0			: ����
INT CESL_Mgr::MultiComboControlSetData(CString mgralias, CString ctrlalias, CString data[], INT ndatas, INT index)
{
EFS_BEGIN

	CESL_ControlMgr * pControlMgr = FindCM(mgralias);
	if (!pControlMgr) return -1006;		// �ش� CtrlMgr�� ã�� �� ����
	CESL_Control_Element * pCME = pControlMgr->FindControlElement(ctrlalias);
	if (!pCME) return -1007;	// �ش� Ctrl Element�� ã�� �� ����
	CESL_ControlMultiComboBox * pCM = (CESL_ControlMultiComboBox *)pCME->Control;
	if (!pCM) return -1008;	// �ش� ctrl�� �ʱ�ȭ �Ǿ� ���� ����
	pCM->SetData(data, ndatas, index);
	return 0;

EFS_END
return -1;

}

INT CESL_Mgr::MultiComboControlGetData(CString mgralias, CString ctrlalias, TCHAR *data, INT cols, INT index)
{
EFS_BEGIN

	return -999;	// ���� �����Ǿ� ���� ����

EFS_END
return -1;

}

// Return values 
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1007		: �ش� Control Element�� ã�� �� ����
// -1008		: �ش� Control�� �ʱ�ȭ ���� �ʾ���
// 0			: ����
INT CESL_Mgr::MultiComboControlGetData(CString mgralias, CString ctrlalias, CString &data, INT cols, INT index)
{
EFS_BEGIN

	CESL_ControlMgr * pControlMgr = FindCM(mgralias);
	if (!pControlMgr) return -1006;		// �ش� CtrlMgr�� ã�� �� ����
	CESL_Control_Element * pCME = pControlMgr->FindControlElement(ctrlalias);
	if (!pCME) return -1007;	// �ش� Ctrl Element�� ã�� �� ����
	CESL_ControlMultiComboBox * pCM = (CESL_ControlMultiComboBox *)pCME->Control;
	if (!pCM) return -1008;	// �ش� ctrl�� �ʱ�ȭ �Ǿ� ���� ����
	pCM->GetData(data, cols, index);
	return 0;

EFS_END
return -1;

}

// Return values 
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1007		: �ش� Control Element�� ã�� �� ����
// -1008		: �ش� Control�� �ʱ�ȭ ���� �ʾ���
// 0			: ����
INT CESL_Mgr::MultiComboControlSelectItem(CString mgralias, CString ctrlalias, CString data)
{
EFS_BEGIN

	CESL_ControlMgr * pControlMgr = FindCM(mgralias);
	if (!pControlMgr) return -1006;		// �ش� CtrlMgr�� ã�� �� ����
	CESL_Control_Element * pCME = pControlMgr->FindControlElement(ctrlalias);
	if (!pCME) return -1007;	// �ش� Ctrl Element�� ã�� �� ����
	CESL_ControlMultiComboBox * pCM = (CESL_ControlMultiComboBox *)pCME->Control;
	if (!pCM) return -1008;	// �ش� ctrl�� �ʱ�ȭ �Ǿ� ���� ����
	pCM->SelectItem(data);
	return 0;

EFS_END
return -1;

}

// Return values 
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1007		: �ش� Control Element�� ã�� �� ����
// -1008		: �ش� Control�� �ʱ�ȭ ���� �ʾ���
// 0			: ����
INT CESL_Mgr::MultiComboControlSelectedPosition(CString mgralias, CString ctrlalias)
{
EFS_BEGIN

	CESL_ControlMgr * pControlMgr = FindCM(mgralias);
	if (!pControlMgr) return -1006;		// �ش� CtrlMgr�� ã�� �� ����
	CESL_Control_Element * pCME = pControlMgr->FindControlElement(ctrlalias);
	if (!pCME) return -1007;	// �ش� Ctrl Element�� ã�� �� ����
	CESL_ControlMultiComboBox * pCM = (CESL_ControlMultiComboBox *)pCME->Control;
	if (!pCM) return -1008;	// �ش� ctrl�� �ʱ�ȭ �Ǿ� ���� ����
	return pCM->GetCurSel();

EFS_END
return -1;

}

// Return values 
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1007		: �ش� Control Element�� ã�� �� ����
// -1008		: �ش� Control�� �ʱ�ȭ ���� �ʾ���
// 0			: ����
INT CESL_Mgr::MultiComboControlSetSelectedPosition(CString mgralias, CString ctrlalias, INT index)
{
EFS_BEGIN

	CESL_ControlMgr * pControlMgr = FindCM(mgralias);
	if (!pControlMgr) return -1006;		// �ش� CtrlMgr�� ã�� �� ����
	CESL_Control_Element * pCME = pControlMgr->FindControlElement(ctrlalias);
	if (!pCME) return -1007;	// �ش� Ctrl Element�� ã�� �� ����
	CESL_ControlMultiComboBox * pCM = (CESL_ControlMultiComboBox *)pCME->Control;
	if (!pCM) return -1008;	// �ش� ctrl�� �ʱ�ȭ �Ǿ� ���� ����
	return pCM->SetCurSel(index);

EFS_END
return -1;

}

CESL_Information *CESL_Mgr::GetInformationPtr(VOID)
{
EFS_BEGIN

	if (m_pInfo) return m_pInfo;
		else return (pParentMgr->GetInformationPtr());

EFS_END
return NULL;

}

// Return values
// -1009		: �ش� Data Mgr�� ã�� �� ����
// 0			: ����
INT CESL_Mgr::GetDataMgrData(CString mgralias, CString fieldalias, CString &data, INT index)
{
EFS_BEGIN

	if (index == -1) return -1;
	CESL_DataMgr * p = FindDM(mgralias);
	if (!p) return -1009;		// �ش� Data Mgr�� ã�� �� ����
	data = p->GetCellData(fieldalias, index);
	return 0;

EFS_END
return -1;

}

// Return values
// -1009		: �ش� Data Mgr�� ã�� �� ����
// 0			: ����
INT CESL_Mgr::SetDataMgrData(CString mgralias, CString fieldalias, CString data, INT index)
{
EFS_BEGIN

	CESL_DataMgr * p = FindDM(mgralias);
	if (!p) return -1009;		// �ش� Data Mgr�� ã�� �� ����
	p->SetCellData(fieldalias, data, index);
	return 0;

EFS_END
return -1;

}

// Return values
// -1009		: �ش� Data Mgr�� ã�� �� ����
INT CESL_Mgr::InsertDataMgrRow(CString mgralias, INT index)		// index�� -1�̸� �������� ���� ����
{
EFS_BEGIN

	CESL_DataMgr * p = FindDM(mgralias);
	if (!p) return -1009;		// �ش� Data Mgr�� ã�� �� ����
	return (p->InsertRow(index));

EFS_END
return -1;

}

// Return values
// -1009		: �ش� Data Mgr�� ã�� �� ����
INT CESL_Mgr::RefreshDataMgr(CString mgralias, CString option)
{
EFS_BEGIN

	CESL_DataMgr * p = FindDM(mgralias);	
	if (!p) return -1009;		// �ش� Data Mgr�� ã�� �� ����
	return (p->RefreshDataManager(option));	

EFS_END
return -1;

}

// Return values
// -1009		: �ش� Data Mgr�� ã�� �� ����
INT CESL_Mgr::GetRowCount(CString mgralias)							// DM�� Row �� ������ ��ȯ
{
EFS_BEGIN

	CESL_DataMgr * p = FindDM(mgralias);
	if (!p) return -1009;		// �ش� Data Mgr�� ã�� �� ����
	return (p->GetRowCount());	

EFS_END
return -1;

}

/*************************************************
Control Display ���� �Լ�
*************************************************/
// return values
// -1006		: �ش� Control Mgr�� ã�� �� ����
INT CESL_Mgr::ControlDisplay(CString cmalias, CString ctrlalias, INT index)	// Control �Ѱ��� ���� Display �Ҷ�
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	return pCM->ControlDisplay(ctrlalias, index);

EFS_END
return -1;

}

// return values
// -1006		: �ش� Control Mgr�� ã�� �� ����
INT CESL_Mgr::AllControlDisplay(CString cmalias, INT index)		// �ش� CtrlMgr �� Control�� ��� ���� Display �Ѵ�.
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	
	INT ids = pCM->AllControlDisplay(index);
	return ids;

EFS_END
return -1;

}

/*************************************************
Grid ���� ����Ʈ ���� �Լ���
*************************************************/
// return values
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1010		: �ش� Grid�� ã�� �� ����
// -1008		: �ش� Control(Grid)�� �ʱ�ȭ ���� �ʾ���
INT CESL_Mgr::SelectMakeList(CString cmalias, CString ctrlalias)			// �ش� Grid Ctrl�� Select List�� �����Ѵ�.
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	CESL_Control_Element *pCME = pCM->FindControlElement(ctrlalias);
	if (!pCME) return -1010;		// �ش� Grid�� ã�� �� ����
	CESL_Grid *pG = (CESL_Grid*)pCME->Control;
	if (!pG) return -1008;			// Grid�� �ʱ�ȭ ���� �ʾ���

	return pG->SelectMakeList();

EFS_END
return -1;

}

// return values
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1010		: �ش� Grid�� ã�� �� ����
// -1008		: �ش� Control(Grid)�� �ʱ�ȭ ���� �ʾ���
INT CESL_Mgr::SelectGetCount(CString cmalias, CString ctrlalias)			// �ش� Grid�� Select ������ ��ȯ�Ѵ�.
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	CESL_Control_Element *pCME = pCM->FindControlElement(ctrlalias);
	if (!pCME) return -1010;		// �ش� Grid�� ã�� �� ����
	CESL_Grid *pG = (CESL_Grid*)pCME->Control;
	if (!pG) return -1008;			// Grid�� �ʱ�ȭ ���� �ʾ���

	return pG->SelectGetCount();

EFS_END
return -1;

}

// return values
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1010		: �ش� Grid�� ã�� �� ����
// -1008		: �ش� Control(Grid)�� �ʱ�ȭ ���� �ʾ���
INT CESL_Mgr::SelectGetHeadPosition(CString cmalias, CString ctrlalias)		// �ش� Grid�� ��ó�� ���õ� index�� ��ȯ�Ѵ�.
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	CESL_Control_Element *pCME = pCM->FindControlElement(ctrlalias);
	if (!pCME) return -1010;		// �ش� Grid�� ã�� �� ����
	CESL_Grid *pG = (CESL_Grid*)pCME->Control;
	if (!pG) return -1008;			// Grid�� �ʱ�ȭ ���� �ʾ���

	return pG->SelectGetHeadPosition();

EFS_END
return -1;

}

// return values
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1010		: �ش� Grid�� ã�� �� ����
// -1008		: �ش� Control(Grid)�� �ʱ�ȭ ���� �ʾ���
INT CESL_Mgr::SelectGetTailPosition(CString cmalias, CString ctrlalias)		// �ش� Grid�� �Ǹ����� ���õ� index�� ��ȯ�Ѵ�.
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	CESL_Control_Element *pCME = pCM->FindControlElement(ctrlalias);
	if (!pCME) return -1010;		// �ش� Grid�� ã�� �� ����
	CESL_Grid *pG = (CESL_Grid*)pCME->Control;
	if (!pG) return -1008;			// Grid�� �ʱ�ȭ ���� �ʾ���

	return pG->SelectGetTailPosition();

EFS_END
return -1;

}

// return values
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1010		: �ش� Grid�� ã�� �� ����
// -1008		: �ش� Control(Grid)�� �ʱ�ȭ ���� �ʾ���
INT CESL_Mgr::SelectGetNext(CString cmalias, CString ctrlalias)				// �ش� Grid�� ���� ���õ� item�� index�� ��ȯ�Ѵ�.
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	CESL_Control_Element *pCME = pCM->FindControlElement(ctrlalias);
	if (!pCME) return -1010;		// �ش� Grid�� ã�� �� ����
	CESL_Grid *pG = (CESL_Grid*)pCME->Control;
	if (!pG) return -1008;			// Grid�� �ʱ�ȭ ���� �ʾ���

	return pG->SelectGetNext();

EFS_END
return -1;

}

// return values
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1010		: �ش� Grid�� ã�� �� ����
// -1008		: �ش� Control(Grid)�� �ʱ�ȭ ���� �ʾ���
INT CESL_Mgr::SelectGetPrev(CString cmalias, CString ctrlalias)				// �ش� Grid�� ���� ���õ� item�� index�� ��ȯ�Ѵ�.
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	CESL_Control_Element *pCME = pCM->FindControlElement(ctrlalias);
	if (!pCME) return -1010;		// �ش� Grid�� ã�� �� ����
	CESL_Grid *pG = (CESL_Grid*)pCME->Control;
	if (!pG) return -1008;			// Grid�� �ʱ�ȭ ���� �ʾ���

	return pG->SelectGetPrev();

EFS_END
return -1;

}

// return values
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1010		: �ش� Grid�� ã�� �� ����
// -1008		: �ش� Control(Grid)�� �ʱ�ȭ ���� �ʾ���
INT CESL_Mgr::SetReverse(CString cmalias, CString ctrlalias, INT index)		// �ش� Grid�� index�� ������Ų��.
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	CESL_Control_Element *pCME = pCM->FindControlElement(ctrlalias);
	if (!pCME) return -1010;		// �ش� Grid�� ã�� �� ����
	CESL_Grid *pG = (CESL_Grid*)pCME->Control;
	if (!pG) return -1008;			// Grid�� �ʱ�ȭ ���� �ʾ���

	return pG->SetReverse(index);

EFS_END
return -1;

}

// return values
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1010		: �ش� Grid�� ã�� �� ����
// -1008		: �ش� Control(Grid)�� �ʱ�ȭ ���� �ʾ���
INT CESL_Mgr::UnSelectAll(CString cmalias, CString ctrlalias)			// �ش� Grid�� ���ø���Ʈ�� �����Ѵ�.
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	CESL_Control_Element *pCME = pCM->FindControlElement(ctrlalias);
	if (!pCME) return -1010;		// �ش� Grid�� ã�� �� ����
	CESL_Grid *pG = (CESL_Grid*)pCME->Control;
	if (!pG) return -1008;			// Grid�� �ʱ�ȭ ���� �ʾ���
	
	return pG->UnSelectAll();

EFS_END
return -1;

}

CString CESL_Mgr::GetUserID(VOID)
{
EFS_BEGIN

	if (m_pInfo == NULL) {
		if (pParentMgr) {
			return pParentMgr->GetUserID();
		} else {
			return _T("");
		}
	} else {
		return m_pInfo->USER_ID;
	}

EFS_END
return _T("");

}

CString CESL_Mgr::GetUserLEVEL(VOID)
{
EFS_BEGIN

	if (m_pInfo == NULL) {
		if (pParentMgr) {
			return pParentMgr->GetUserLEVEL();
		} else {
			return _T("");
		}
	} else {
		return m_pInfo->USER_PRIVILEGE;
	}

EFS_END
return _T("");

}

// return values
// -1006		: �ش� Control Mgr�� ã�� �� ����
// -1010		: �ش� Grid�� ã�� �� ����
// -1008		: �ش� Control(Grid)�� �ʱ�ȭ ���� �ʾ���
INT CESL_Mgr::GridSetReverse(CString cmalias, CString gridalias, INT row)	// �ش� Grid�� row�� ������Ų��.
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	CESL_Control_Element *pCME = pCM->FindControlElement(gridalias);
	if (!pCME) return -1010;		// �ش� Grid�� ã�� �� ����
	CESL_Grid *pG = (CESL_Grid*)pCME->Control;
	if (!pG) return -1008;			// Grid�� �ʱ�ȭ ���� �ʾ���

	return(pG->SetReverse(row));

EFS_END
return -1;

}

UINT CESL_Mgr::ESLMessageBox(CString msg, INT style)
{
EFS_BEGIN

	CESLDlg_Confirm dlg;
	dlg.m_msg = msg;
	return dlg.DoModal();
EFS_END
return -1;
}

INT CESL_Mgr::AllControlReadOnly(CString cmalias)
{
EFS_BEGIN
	CESL_ControlMgr * pCM = FindCM(cmalias);
	if (!pCM) return -1006;		// ControlMgr�� ã�� �� ����
	return pCM->AllControlReadOnly();	
EFS_END
return -1;
}

INT CESL_Mgr::SetSMReadOnly(VOID)
{
EFS_BEGIN
	INT cmcount = CMList.GetCount();
	if (cmcount == 0) return NULL;
	CESL_ControlMgr * pCm = NULL;
	POSITION pos;
	pos = CMList.GetHeadPosition();
	for(INT i = 0; i < cmcount; i++) {
		pCm = NULL;
		pCm = (CESL_ControlMgr*)CMList.GetNext(pos);
		if (pCm) {
			pCm->AllControlReadOnly();
		}
	}
	return 0;
EFS_END
return -1;
}

CString CESL_Mgr::GetCurrentTimeStamp(VOID)
{
EFS_BEGIN
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	return result;
EFS_END
return _T("");
}

// Report File�� DB�� ���� ���� �޾Ƽ� tmp ���Ϸ� �����Ѵ�.
// 0			:		����
// -1020		:		Information �� �ʱ�ȭ ���� �ʾ���
// -1021		:		DM�� ã�� �� ����
// -1022		:		Report Data�� ���� 
// -1023		:		Report Data�� ������ ã�� �� ����
// -1024		:		��Ÿ ����
// -1025		:		tmp ���� ���� ����
INT CESL_Mgr::GetReportFileFromDB(CString RptName, CString tmpFilename)
{
EFS_BEGIN
	if (m_pInfo == NULL) return -1020;
	CESL_DataMgr *pDM = m_pInfo->pESLMgr;
	if (!pDM) return -1021;

	INT ids;
	CString szBuf, sQuery;
	sQuery.Format(_T("SELECT count(*) FROM ESL_REPORT_TBL WHERE TITLE='%s'"), RptName);
	ids = pDM->GetOneValueQuery(sQuery, szBuf);
	if (_ttoi(szBuf) < 1) return -1022;		// �ش� Report �� �����ϴ�.
	sQuery.Format(_T("SELECT PK, RPTSIZE FROM ESL_REPORT_TBL WHERE TITLE='%s'"), RptName);
	CString result[2];
	ids = pDM->GetValuesQuery(sQuery, result, 2);
	if (ids < 0) return -1023;
	INT RptSize = _ttoi(result[1]);
	INT BufferSize = sizeof(BYTE)*RptSize + 1024;
	BYTE *pBuffer = NULL;
	pBuffer = (BYTE*)malloc(BufferSize);
	if (!pBuffer) {
		AfxMessageBox(_T("Malloc Memory Fail!"));
		return -1024;
	}
	ids = pDM->LOBGet(_T("ESL_REPORT_TBL"), _T("PK"), _T("NUMERIC"), result[0], _T("RPTDATA"), pBuffer, &BufferSize);
	if (ids < 0) {
		AfxMessageBox(_T("Get Report Data Fail!"));
		free(pBuffer);
		return -1024;
	}
	if (BufferSize <= 0) {
		AfxMessageBox(_T("Empty Report Data!"));
		return -1024;
	}

	CFile f;
    if( !f.Open(tmpFilename, CFile::modeWrite|CFile::modeCreate | CFile::typeBinary) ) {
		free(pBuffer);
		AfxMessageBox(_T("Create Temp File Fail"));
		return - 1025;
	}
	
	if( 0 == f.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		f.Write( &cUni, sizeof(TCHAR));
	}
		
	f.WriteHuge(pBuffer, BufferSize);
	f.Close();
	free (pBuffer);
	return 0;
EFS_END
return -1;
}

HBRUSH CESL_Mgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
EFS_BEGIN
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (FindRE(pWnd->GetDlgCtrlID())) {
		pDC->SetBkMode(OPAQUE );
		pDC->SetBkColor(RGB(m_bkR, m_bkG, m_bkB));
		return m_BkBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
EFS_END
return NULL;
}

bool CESL_Mgr::FindRE(INT id)
{
EFS_BEGIN
	INT recount = REList.GetCount();
	if (recount == 0) return false;
	INT * pINT = NULL;
	POSITION pos;
	pos = REList.GetHeadPosition();
	for(INT i = 0; i < recount; i++) {
		pINT = NULL;
		pINT = (INT*)REList.GetNext(pos);
		if (pINT && *pINT == id) return true;
	}
	return false;
EFS_END
return FALSE;
}

VOID CESL_Mgr::PostNcDestroy() 
{
EFS_BEGIN
	if (pMain)
		pMain->SendMessage(WM_DESTROY_MODELESS, (WPARAM)this, 0);	
	
	CDialog::PostNcDestroy();
EFS_END
return;
}



INT CESL_Mgr::Init_DVM(CString DVM_alias)
{
EFS_BEGIN
	if ( Check_DVM_Mode() ) return 0;
	
	pDVM->InitDVMAliasFromMemory(DVM_alias);

	return 0;
EFS_END
return -1;
}

INT CESL_Mgr::Start_DVM(CString Field , CString Type, CString Data)
{
EFS_BEGIN
	if ( Check_DVM_Mode() ) return 0;
    
    pDVM->SetKeyField(Field, Type, Data);
    pDVM->StartPointCheck();

	return 0;
EFS_END
return -1;
}

INT CESL_Mgr::End_DVM()
{
EFS_BEGIN
	if ( Check_DVM_Mode() ) return 0;

	pDVM->EndPointCheck();

	return 0;
EFS_END
return -1;
}

INT CESL_Mgr::Finish_DVM()
{
EFS_BEGIN
	if ( Check_DVM_Mode() ) return 0;

	INT ids;
	
	ids = pDVM->EndDVM();

	return ids;
EFS_END
return -1;
}


INT CESL_Mgr::Check_DVM_Mode()
{
EFS_BEGIN
	return 0;
EFS_END
return -1;
}

INT CESL_Mgr::GetDVMLog(CString &strLog)
{
EFS_BEGIN
	pDVM->GetHtmlFormatData(strLog);

	return 0;
EFS_END
return -1;
}

// worksheet �Է� 

// WorkSheet �� �ʱ�ȭ �Ѵ� 
INT CESL_Mgr::InitWorkSheet_Mgr( CString cmAlias, INT flag )
{
EFS_BEGIN
	// 1 == flag	: ���� Init
	// 2 == flag	: ��ư�޼����� ���� Init
	
	m_workSheetCMAlias = cmAlias;
	m_workSheetType = GetWorkSheetType( cmAlias );
	
	if(	1 == flag && ( 2 == m_workSheetType || 4 == m_workSheetType ) )		
	{
		CDialog::EndDialog( 0 );
		return m_workSheetType;
	}
	
	if( InputOnWorkSheet( this ) )
		return -1;
	
	return m_workSheetType;
EFS_END
return -1;
}

// ����, cmAlias �� ���� WorkSheet Type�� �޾ƿ´�
INT CESL_Mgr::GetWorkSheetType( CString cmAlias )
{
EFS_BEGIN
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
	
	CString query, type;
	query.Format( _T("SELECT TYPE FROM ESL_WORKSHEET_TBL WHERE K2U_USER = '%s' AND CM_ALIAS = '%s'"), GetUserID(), cmAlias );

	dm.GetOneValueQuery( query, type );

	return _ttoi( type );
EFS_END
return -1;
}

// ��ũ��Ʈ�� ���� ��������� �ʱ�ȭ �ϰ� ���̾�α׸� Create�Ѵ�
INT CESL_Mgr::InputOnWorkSheet( CWnd* pParent )
{
EFS_BEGIN
	if( m_workSheet != NULL )
	{
		delete m_workSheet ;
		m_workSheet = NULL;
	}
	m_workSheet = new CESL_WorkSheet_dlg( this );
	m_workSheet->SetRect(m_WorkSheetRect);
	m_workSheet->m_parentWnd		= pParentMgr;
	m_workSheet->m_workSheetType	= m_workSheetType;
	m_workSheet->m_cmAlias			= m_workSheetCMAlias;
	m_workSheet->m_ctrlMgr			= pParentMgr->FindCM( m_workSheetCMAlias );
	m_workSheet->m_smAlias			= pParentMgr->SM_ALIAS;
	m_workSheet->m_pInfo			= pParentMgr->m_pInfo;
	
	if( NULL == m_workSheet->m_ctrlMgr )
	{
		AfxMessageBox( _T("Error : CM Alias�� ã�� �� �����ϴ�.") );
		
		delete m_workSheet;
		m_workSheet = NULL;

		return -1;
	}
	
	m_workSheet->Create( IDD_DLG_WORKSHEET, pParent );
	m_workSheet->MoveWindow(&m_workSheet->m_rect, true);
	m_workSheet->ReSizeWindow();
	m_workSheet->ShowWindow( SW_SHOW );
	m_workSheet->SetFocus();

	return 0;
EFS_END
return -1;
}

// ��ũ��Ʈ�� �Էµ� ������ ��ũ��Ʈ�� ������ �ִ� CM�� control�� ���� ������.
INT CESL_Mgr::SaveWorkSheetData()
{
EFS_BEGIN
	return m_workSheet->SaveWorkSheetData();
EFS_END
return -1;
}

// ��ũ��Ʈ�� ������ �ִ� CM�� control ������ �ٽ� �о�´�
VOID CESL_Mgr::RefreshWorkSheetData()
{
EFS_BEGIN
	m_workSheet->RefreshWorkSheetData();
EFS_END
return;
}

INT CESL_Mgr::SetWorkSheetRect(CRect rect)
{
EFS_BEGIN
	m_WorkSheetRect = rect;
	return 0;
EFS_END
return -1;
}

INT CESL_Mgr::GetWorkSheetRect(CRect &rect)
{
EFS_BEGIN
	rect = m_WorkSheetRect;
	return 0;
EFS_END
return -1;
}

CString CESL_Mgr::GetWorkLogProc(BOOL bReplace)
{
EFS_BEGIN
	if (!m_pInfo) return _T("");
	CString strMsg;
	if (bReplace) {
		strMsg.Format(_T("GET_WORK_INFO(''%s'', ''%s'')"), m_pInfo->USER_ID, m_pInfo->LOCAL_IP);
	} else {
		strMsg.Format(_T("GET_WORK_INFO('%s', '%s')"), m_pInfo->USER_ID, m_pInfo->LOCAL_IP);
	}
	return strMsg;
EFS_END
return _T("");
}

CString CESL_Mgr::GetWorkLogMsg(CString strMenu, CString sSourcePath, INT nSourceFileLine)
{
EFS_BEGIN

	CString strMsg;
	strMsg.Empty();

#ifdef __WORK_LOG_WRITE__

	//____00_________01________02________03________04________05________06________07________08________09________0@
	// ex : YYYYMMDD hhmm �۾���ID@�۾���IP �۾��޴�20byte>SourceFile��22byte#SourceFile�ٹ�ȣ
	// ex : 20021121 1326 ID34567890123456789@192.168.100.131 ��Ͽϼ���Ͽϼ����>SourceFile123456789012#12345
	//____00_________01________02________03________04________05________06________07________08________09________0@

	strMenu.TrimLeft();
	strMenu.TrimRight();
	if (20 < strMenu.GetLength()) strMenu = strMenu.Left(20);

	sSourcePath.TrimLeft();
	sSourcePath.TrimRight();

	INT nBackSlashPos = sSourcePath.ReverseFind(_T('\\'));

	CString sSourceFile;
	sSourceFile = sSourcePath.Mid(nBackSlashPos+1);
	if (22 < sSourceFile.GetLength()) sSourceFile = sSourceFile.Left(22);


	CTime t = CTime::GetCurrentTime();

	if (NULL==m_pInfo) 
	{
		strMsg.Format(_T("%04d%02d%02d %02d%02d %s>%s#%d"), 
					  t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), //t.GetSecond(), 
					  //m_pInfo->USER_ID, m_pInfo->LOCAL_IP,
					  strMenu, sSourceFile, nSourceFileLine);
	}
	else
	{
		strMsg.Format(_T("%04d%02d%02d %02d%02d %s@%s %s>%s#%d"), 
					  t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), //t.GetSecond(), 
					  m_pInfo->USER_ID, m_pInfo->LOCAL_IP,
					  strMenu, sSourceFile, nSourceFileLine);
	}

	if (99 < strMsg.GetLength()) strMsg = strMsg.Left(99);

/*
	if (strMenu.GetLength() > 0) {
		strMsg.Format(_T("%04d-%02d-%02d %02d:%02d:%02d %s %s"), 
					  t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), 
					  m_pInfo->USER_ID, m_pInfo->LOCAL_IP);
	} else {
		strMsg.Format(_T("%04d-%02d-%02d %02d:%02d:%02d %s %s [%s]"), 
			          t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), 
					  m_pInfo->USER_ID, m_pInfo->LOCAL_IP, 
					  strMenu);
	}
*/
#endif

	return strMsg;
EFS_END
return _T("");
}

CObject *CESL_Mgr::GetCommonMarcMgr(VOID)
{
EFS_BEGIN
	if (m_pInfo->pMarcMgr == NULL) return NULL;
	return ((CObject*)m_pInfo->pMarcMgr);
EFS_END
return NULL;
}

INT CESL_Mgr::GetManageValue( CString group_2, CString group_3, CString alias, CString valueName, CString &value )
{
EFS_BEGIN
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	CString query, tmp;
	if( valueName.GetLength() )
		tmp.Format( _T(" AND VALUE_NAME = '%s'"), valueName );

	query.Format( _T("SELECT COUNT(*) FROM ESL_MANAGE_TBL ")
				  _T("WHERE GROUP_1='L' AND GROUP_2='%s' AND GROUP_3='%s' AND CLASS_ALIAS='%s' ")
				  _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), group_2, group_3, alias );
	query += tmp;	
	dm.GetOneValueQuery( query, value );
	
	if ( _ttoi(value) == 0 )	
	{
		value = _T("");
		return 100;
	}

	query.Format( _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
				  _T("WHERE GROUP_1='L' AND GROUP_2='%s' AND GROUP_3='%s' AND CLASS_ALIAS='%s' ")
				  _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), group_2, group_3, alias );
	query += tmp;
	
	dm.GetOneValueQuery( query, value );
	
	return 0;
EFS_END
return -1;
}

CObject * CESL_Mgr::FindControl(CString CMAlias, CString CtrlAlias)
{
EFS_BEGIN
	CESL_ControlMgr *pCM = FindCM(CMAlias);
	if (!pCM) return NULL;

	return pCM->FindControl(CtrlAlias);
EFS_END
return NULL;
}

INT CESL_Mgr::SetManageValue( CString group_2, CString group_3, CString alias, CString valueName, CString value )
{
EFS_BEGIN
	CString query, where, tmp, result;
	where.Format( _T(" WHERE GROUP_2 = '%s' AND GROUP_3 = '%s'"), group_2, group_3 );

	if( alias.GetLength() )
	{
		tmp.Format( _T(" AND CLASS_ALIAS = '%s'"), alias);
		where += tmp;
	}
	
	if( valueName.GetLength() )
	{	
		tmp.Format( _T(" AND VALUE_NAME = '%s'"), valueName );
		where += tmp;
	}

	// 2008.10.13 ADD BY PDJ MANAGE_CODE �߰�
	where += _T(" AND MANAGE_CODE = '") + m_pInfo->MANAGE_CODE + _T("'");
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	query = _T("SELECT COUNT( * ) FROM ESL_MANAGE_TBL ") + where;
	dm.GetOneValueQuery( query, result );

	// 2005.04.02 UPDATE BY PDJ
	if( 0 == _ttoi( result ) )
		return -1;		// ���ǿ� �´� ���� ����.

	/*
	if( 1 != _ttoi( result ) )
		return -1;		// ���ǿ� �´� ���� ����.
	*/

	query.Format( _T("UPDATE ESL_MANAGE_TBL SET VALUE_1 = '%s' %s;"), value, where );
	dm.ExecuteQuery( query, 1 );	
	
	return 0;
EFS_END
return -1;
}

VOID CESL_Mgr::OnShowDMViewer(UINT  nIDC)
{
EFS_BEGIN
	FILE *fd = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
	if (fd == NULL) return;
	fclose(fd);

	CESLDlg_DM_Viewer dlg;
	dlg.pMgr = this;
	dlg.DoModal();

	return;
EFS_END
return;
}

INT CESL_Mgr::ESLAfxMessageBox(CString strMsg, UINT nType /*= MB_ICONEXCLAMATION*/)
{
EFS_BEGIN
	return MessageBox(strMsg, NULL, nType);
EFS_END
return -1;
}

INT CESL_Mgr::OnShowHtmlHelp(VOID)
{
	CString strCHMfileName = _T("..\\HELP\\kolas2up.chm");
	CStdioFile fd;
	if (fd.Open(_T("..\\CFG\\HELP.CFG"), CFile::modeRead|CFile::typeBinary) == FALSE) {
		MessageBox(_T("���� ������ ã�� �� �����ϴ�."), _T("���� ���"));
		return 0;
	} else {
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
		CString strLine;
		fd.ReadString(strLine);
		strLine.TrimLeft();			strLine.TrimRight();
		if (strLine.GetLength() > 0) strCHMfileName = strLine;
		fd.Close();
	}

	HH_AKLINK link;
	link.cbStruct =     sizeof(HH_AKLINK);
	link.fReserved =    FALSE;
	link.pszKeywords =  SM_ALIAS; 
	link.pszUrl =       NULL; 
	link.pszMsgText =   NULL; 
	link.pszMsgTitle =  NULL; 
	link.pszWindow =    NULL;
	link.fIndexOnFail = TRUE;

	HtmlHelp(m_hWnd, strCHMfileName, HH_KEYWORD_LOOKUP, (DWORD)&link);

	return 0;
}

INT CESL_Mgr::AddColorControl(INT nCtrlID, CString strCtrlMgrAlias, CString strCtrlAlias, TCHAR cCtrlType)
{
EFS_BEGIN
	m_arrColorControl.Add(nCtrlID);
	if (cCtrlType == 'E') return 0;
	if (strCtrlMgrAlias.GetLength() == 0) return 0;
	CESL_ControlMgr *pCM = FindCM(strCtrlMgrAlias);
	if (pCM) {
		if (cCtrlType == 'M') {
			CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)pCM->FindControl(strCtrlAlias);
			if (pCMB) {
				pCMB->RequireFlag = TRUE;
				pCMB->m_bkR = m_bkR;
				pCMB->m_bkG = m_bkG;
				pCMB->m_bkB = m_bkB;
			}
		}
		if (cCtrlType == 'D') {
			CECO_ControlDateEdit *pCDATE = (CECO_ControlDateEdit *)pCM->FindControl(strCtrlAlias);
			if (pCDATE) {
				pCDATE->RequireFlag = TRUE;
				pCDATE->m_bkR = m_bkR;
				pCDATE->m_bkG = m_bkG;
				pCDATE->m_bkB = m_bkB;
			}
		}
	}
	return 0;
EFS_END
return -1;
}

BOOL CESL_Mgr::IsColorControl(INT nCtrlID)
{
EFS_BEGIN
	INT nControls = m_arrColorControl.GetSize();
	for (INT i = 0; i < nControls; i++) {
		if (m_arrColorControl.GetAt(i) == nCtrlID) {
			return TRUE;
		}
	}
	return FALSE;
EFS_END
return FALSE;
}

HBRUSH CESL_Mgr::SetColor(CDC* pDC)
{
	if (pDC == NULL) return m_BkBrush;
	pDC->SetBkMode(OPAQUE );
	pDC->SetBkColor(RGB(m_bkR, m_bkG, m_bkB));
	return m_BkBrush;
}

INT CESL_Mgr::SearchInFile(CString f_strpath, CString f_strKeyword )
{
	const INT LINE_SIZE = 202;
	const INT MAX_BUFFER = 202;
	const INT IDX_OFFSET = 0;
	const INT RECKEY_OFFSET= 0;
	const INT USER_IDXNO_OFFSET= 0;
	
	CFile	fp;	
	CString strLine;
	TCHAR	szBuf[1024];
	TCHAR	szKey[1024];
	
	LONG	lFileSize;					
	UINT	mid;		//�̺а˻��� ���� ��ġ ��
	UINT	left;		//ù��° ��
	UINT	right;	
	INT		idx;	
	INT		nOffset;
	INT		nDefaultOffset = 0;		

	_stprintf(szKey, _T("%s"), f_strKeyword);
	
	if(!fp.Open(f_strpath,CFile::modeRead|CFile::typeBinary)) return -100;
	TCHAR cUni;
	fp.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fp.SeekToBegin();
	}

	// ù��° ���� ���� �̵�
	// �������� �˻� ��ġ ����
	lFileSize = fp.GetLength();
	
	// �ּ� �� ������ ���� ã��
	fp.Read(szBuf, 25);
	strLine.Format(_T("%s"), szBuf);
	// ù���� �ּ��̸�
	left = 0;
	if ( szBuf[0] == '.' ) 
	{		
		nOffset = strLine.FindOneOf(_T("\n"));		
		nDefaultOffset = nOffset;
		right = (lFileSize-nOffset)/MAX_BUFFER;
	}
	else
	{	
		nDefaultOffset = 0;
		right = lFileSize/MAX_BUFFER;
	}	
	
	while( right >= left )
	{
		if ( left == 0 && right == 0 ) return 0;
		
		if ( (left+right) == 1 ) mid = 1;
		else mid = (left+right)/2 ;

		nOffset = (mid-1)*(LINE_SIZE)+nDefaultOffset;
		fp.Seek(nOffset+1, SEEK_SET);
		
		fp.Read(szBuf, 64);	
		strLine.Format(_T("%s"), szBuf);		
		idx = strLine.FindOneOf(_T(" "));
		szBuf[idx] = '\0';
		
		idx = _tcscmp(szBuf, szKey);
		if(idx==0)
		{
			// FIND YES
			return 1;
		}
		// ������ �Ʒ�������
		else if(idx<0)
		{
			left = mid + 1;
		}
		// ������ ��������
		else 
		{
			right = mid - 1;
		}
	}
	
	fp.Close();

	// FIND NO

	return 0;	
}

// 2008.11.07 ADD BY PWR
// ���̾�α� XP�׸�����
VOID CESL_Mgr::EnableThemeDialogTexture(HWND hwndDlg)
{
	HINSTANCE hDll=LoadLibrary(_T("UxTheme.dll"));	// 'UxTheme.dll'
	if(hDll==NULL)
		return;	// the DLL won't be available on anything except Windows XP
	ULONG (PASCAL *lpfnEnableTheme)(HWND, DWORD);
	(FARPROC&)lpfnEnableTheme=GetProcAddress(hDll,"EnableThemeDialogTexture");	// 'EnableThemeDialogTexture'
	if(lpfnEnableTheme)
		lpfnEnableTheme(hwndDlg, 6);	// ETDT_ENABLETAB = (ETDT_ENABLE | ETDT_USETABTEXTURE) = (2|4) = 6
	else ASSERT(FALSE);
	FreeLibrary(hDll);
}

// ���յ������� ��ȣȭ
INT CESL_Mgr::ESL_DES_Encryption(CString strSrc, CString &strDesc)
{
EFS_BEGIN	

	strDesc.Empty();
	if(0 == strSrc.GetLength())
	{
		return 1000;
	}

	INT nSrcLen = 0; 
	char* szSrc = NULL;

#ifdef _UNICODE
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL,NULL);
	szSrc = new char[nSrcLen];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szSrc, nSrcLen, NULL, NULL);
#else
	nSrcLen = strSrc.GetLength();
	szSrc = new char[nSrcLen+1];
	strcpy(szSrc, strSrc.GetBuffer(0));
	strSrc.ReleaseBuffer();
	szSrc[nSrcLen] = '\0';
#endif

	INT nDstLen = 0;
	char* szDst = NULL;
	Encryption(szSrc, nSrcLen, &szDst, &nDstLen, 0);

	delete []szSrc;
	szSrc = NULL;

	char* szResult = Char2Hexa(szDst, nDstLen);
  	delete []szDst;
  	szDst = NULL;

#ifdef _UNICODE
	WCHAR* szBuff = NULL;
	INT nSize = ::MultiByteToWideChar(CP_ACP, NULL, szResult, -1, NULL, NULL);	
	if(0 < nSize)
	{	
		szBuff = new WCHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::MultiByteToWideChar(CP_ACP, NULL, szResult, -1, szBuff, nSize);
	}
	delete []szResult;
	szResult = NULL;
	
	strDesc.Format(_T("%s"), szBuff);
	delete [] szBuff;
	szBuff = NULL;
#else
	strDesc.Format(_T("%s"), szResult);
	delete []szResult;
	szResult = NULL;
#endif

	return 0;
EFS_END
	return 0;
}

// ���յ������� ��ȣȭ 
INT CESL_Mgr::ESL_DES_Decryption(CString strSrc, CString &strDesc)
{
EFS_BEGIN
	
	strDesc.Empty();
	if(0 == strSrc.GetLength())
	{
		return 1000;
	}

	INT nInputLen = 0; 
	char* szInput = NULL;

#ifdef _UNICODE
	nInputLen = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL,NULL);
	szInput = new char[nInputLen];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szInput, nInputLen, NULL, NULL);
#else
	nInputLen = strSrc.GetLength();
	szInput = new char[nInputLen+1];
	strcpy(szInput, strSrc.GetBuffer(0));
	strSrc.ReleaseBuffer();
	szInput[nInputLen] = '\0';
#endif

	// ����� �� ��ȣ���̶�� ���ڿ��� ��� HEXA�ڵ�� ���ִ�. 
	// ù�ڸ��� �������κи� üũ�غ���.
	CString strCheckSum = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	if(0 != strSrc.FindOneOf(strCheckSum))
	{
		return -1000;
	}
	CString strEnd = strSrc.Right(1);
	if(0 != strEnd.FindOneOf(strCheckSum))
	{
		return -1000;
	}	

	char* szSrc = Hexa2Char(szInput, nInputLen);
// 	INT nSrcLen = atoi(szSrc);
	INT nSrcLen = nInputLen;
	delete []szInput;
	szInput = NULL;

	INT nDstLen = 0;
	char* szDst = NULL;
	Encryption(szSrc, nSrcLen, &szDst, &nDstLen, 1);

	delete []szSrc;
	szSrc = NULL;

#ifdef _UNICODE
	WCHAR* szBuff = NULL;
	INT nSize = ::MultiByteToWideChar(CP_ACP, NULL, szDst, -1, NULL, NULL);	
	if(0 < nSize)
	{	
		szBuff = new WCHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::MultiByteToWideChar(CP_ACP, NULL, szDst, -1, szBuff, nSize);
	}

	delete []szDst;
	szDst = NULL;
	
	strDesc.Format(_T("%s"), szBuff);
	delete [] szBuff;
	szBuff = NULL;
#else
	strDesc.Format(_T("%s"), szDst);
	delete []szDst;
	szDst = NULL;

#endif
	return 0;	
EFS_END
	return 0;
}


CString CESL_Mgr::KL_MakeRecKey()
{
	INT ids;
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K25_LOC_B01_SERVICE"));		

	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	ids = Sender.Send(&Writer, &Reader);
	if(0 > ids) return _T("");
	
	CESL_DataMgr pDM;
	Reader.makeDM(&pDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strMsg,strTemp;
		pDM.GetCellData(0, 0, strTemp);
		if(0 < strTemp.Find(_T("DT")))
		{
			return _T("");
		}
		strMsg.Format(_T("Error Code : %s"), strTemp);
		pDM.QueryLog(strMsg.GetBuffer(0));
		return _T("");
	}
	CString strRecKey = _T("");
	if(1 == pDM.GetRowCount())
	{
		pDM.GetCellData(_T("SEQ_NO"), 0, strRecKey);
	}
	else
	{
		strRecKey=_T("");
	}

	return strRecKey;
}

char *CESL_Mgr::Char2Hexa(char *pData, INT nSize)
{
    int     x=0, y=0;
    char    *pEnc;

    pEnc = (char*)malloc(3 * nSize + 1);
    if (!pEnc) return NULL;

	INT nResult = 0;
    for (x; x < nSize; x++) 
    {
        sprintf(&pEnc[y], "%02X", (unsigned char)pData[x]);
        y += 2;
    }
    pEnc[y] = '\0';
    return(pEnc);

}

char* CESL_Mgr::Hexa2Char(char* pData, INT nSize)
{		
	char* pResult = new char[nSize+1];

	INT x = 0, y = 0;
	INT nResult = 0;
	char *pStr = pData;
	for (x; x < nSize; x++) 
	{
		UINT val = 0; // ���
		int len = 2; // ����
		while(len > 0)
		{
			char k = *pStr++;
			int nk = (k >= 'A')? k-'7':k-'0';    // 'A' -> 10, 'B' -> 11....  or  '0' -> 0, '1' -> 1
			nk = nk << (--len * 4);
			val |= nk;
		}
		pResult[x] = val;		
	}
	pResult[nSize] = '\0';

	return pResult;
}

//2014-05-01 ADD BY BJY : 2014 ��ݱ� ������Ʈ
//�������� ��� �α� ���
//strPrintType = E: ���� F: ���� R: ����Ʈ 
INT CESL_Mgr::PrivacyPrintLogRecord(CESL_DataMgr *pDM, CString strPrintInfo, CString strPrintType, INT nOption /* = 0 */)
{
EFS_BEGIN
	// KU0303 JMC 20170410 �������� ��·α� - ����. ������������ �ʰ� ����.
	INT     nProcCnt = 0;
	CString strQuery;
	CString strPrintKey;

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	CString strReason;
	if(strPrintType != _T("G"))
	{
		CESLDlg_UserExportReason Dlg;
		if(Dlg.DoModal() != IDOK)
		{
			AfxMessageBox(_T("����� ����մϴ�."));
			return -200;
		}
		strReason = Dlg.m_strReason;
	}
	else
	{
		strReason.Format(_T("KOLAS III - �Ϲ���ȸ"));
	}
	
	strQuery.Format(_T("SELECT ESL_SEQ.NEXTVAL FROM DUAL"));
	pDM->GetOneValueQuery(strQuery, strPrintKey);
	if(_T("") == strPrintKey) 
	{
		AfxMessageBox(_T("�α׻����� �����Ͽ����ϴ�."));
		return -100;
	}	

	CString strMaskingYN = _T("N");
	if(1 == nOption) strMaskingYN = _T("Y");
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	strQuery.Format(_T("INSERT INTO MN_PRIVACY_PRINT_LOG_TBL (REC_KEY, PRINT_USER_KEY, PRINT_USER_ID, PRINT_TYPE, PRINT_INFO, PRINT_DATE, PRINT_COUNT, MASKING_YN, FIRST_WORK, MANAGE_CODE, REASON) ")
					_T("VALUES (%s, %s, '%s', '%s', '%s', SYSDATE, %d, '%s', '%s', '%s', '%s');")
					,strPrintKey, m_pInfo->USER_PK, m_pInfo->USER_ID, strPrintType, strPrintInfo, pDM->GetRowCount(), strMaskingYN, GetWorkLogMsg( _T("�������� ��·α� ���"), __WFILE__, __LINE__ ), m_pInfo->MANAGE_CODE, strReason);
	pDM->StartFrame();
	// KU0303 JMC 20170410 �������� ��·α� - ����. ������������ �ʰ� ����.
	pDM->AddFrame(strQuery);	
	pDM->SendFrame();
	pDM->EndFrame();
	
	CString strDmAlais = pDM->GETDMAlias();
	CString strUserKeyAlias = _T("");
	
	//�󼼷α׸� ����ϱ� ���� ����ó��...�밡��..��...
	//������ DM���� �̿���KEY�� ����ִ� ALIAS�̸��� �ٸ���.
	if(_T("DM_BO_LOC_3300_LIST") == strDmAlais)	strUserKeyAlias = _T("REC_KEY");
	else if(_T("DM_BO_LOC_3200") == strDmAlais)	strUserKeyAlias = _T("������KEY");
	else if(_T("DM_BO_LOC_RESERVE") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");
	else if(_T("DM_BO_LOC_RESERVE_SE") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");
	else if(_T("DM_BO_LOC_3400") == strDmAlais)	strUserKeyAlias = _T("�̿���KEY");				
	else if(_T("DM_SMS_DLG_01") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");			
	else if(_T("DM_SMS_DLG_03_USER") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");		
	else if(_T("DM_SMS_4100") == strDmAlais)	strUserKeyAlias = _T("�̿���KEY");							
	else if(_T("DM_BO_LOC_3100_MEMBER_INFO") == strDmAlais)	strUserKeyAlias = _T("������KEY");				
	else if(_T("DM_BOOK_COOPERATIVE_GROUP_OFFER") == strDmAlais)	strUserKeyAlias = _T("�̿���KEY");		
	else if(_T("DM_BOOK_COOPERATIVE_GROUP_REQUEST") == strDmAlais)	strUserKeyAlias = _T("�̿���KEY");		
	else if(_T("DM_BO_LOC_SECEDER_LIST") == strDmAlais)	strUserKeyAlias = _T("REC_KEY");	
	else if(_T("DM_BO_LOC_LOAN_HIS_USER") == strDmAlais)	strUserKeyAlias = _T("REC_KEY");	
	else if(_T("DM_USER_PRIVACY_ACCESS_INFO") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");	
	else if(_T("DM_BO_LOC_3240_BO") == strDmAlais)	strUserKeyAlias = _T("������KEY");	
	else if(_T("DM_BO_LOC_3170_RESERVE_INFO") == strDmAlais)	strUserKeyAlias = _T("������KEY");	
	else if(_T("DM_FEE_INSERT_USER") == strDmAlais)	strUserKeyAlias = _T("REC_KEY");
	else if(_T("DM_SMSSendDlg") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");	
	else if(_T("DM_FAMILY_ID_INFO") == strDmAlais)	strUserKeyAlias = _T("������KEY");		
	else if(_T("DM_FAMILY_LOAN_HISTORY") == strDmAlais)	strUserKeyAlias = _T("������KEY");		
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	else if(_T("DM_SMS_5120_KatokPL") == strDmAlais)	strUserKeyAlias = _T("�����̷�����KEY");		

	// KU0303 JMC 20170410 �������� ��·α� - ����. ������������ �ʰ� ����.
	pDM->StartFrame();
	if(_T("") != strUserKeyAlias)
	{
		for(INT i=0; i<pDM->GetRowCount(); i++)
		{
			CString strUserKey;
			strUserKey = pDM->GetCellData(strUserKeyAlias, i);
			if(_T("") == strUserKey || _T("") == strPrintKey) continue;
			strQuery.Format(_T("INSERT INTO MN_PRIVACY_PRINT_DETAIL (REC_KEY, PRINT_KEY, USER_KEY) ")
						_T("VALUES (ESL_SEQ.NEXTVAL, %s, %s);")
						,strPrintKey, strUserKey);
			pDM->AddFrame(strQuery);
			// KU0303 JMC 20170410 �������� ��·α� - ����. ������������ �ʰ� ����. 50 �Ǹ��� Ŀ��.
			nProcCnt++;
			if ( nProcCnt%50 == 0 ) {
				pDM->SendFrame();
				pDM->EndFrame();
				pDM->StartFrame();
				pDM->InitDBFieldData();
			}
		}
	}
	pDM->SendFrame();
	pDM->EndFrame();
	return 0;
EFS_END
return -1;
}

//2014-05-08 ADD BY BJY : 2014 ��ݱ� ������Ʈ
//�̿������� ����ŷ
//nType = 1: �̸�, 2: �������, 3:��ȭ��ȣ, 4:�ּ�, 5: �����ڹ�ȣ
CString CESL_Mgr::MaskingString(CString strData, INT nType, INT nOption)
{
EFS_BEGIN

	CString strResult = _T("");

	if(_T("") == strData) return strResult;

	if(1 == nType)
	{
		if(2 == strData.GetLength()) strResult.Format(_T("%s*"), strData.Left(1));	//2����
		else if(3 == strData.GetLength()) strResult.Format(_T("%s*%s"), strData.Left(1), strData.Right(1));	//3����
		else if(4 == strData.GetLength()) strResult.Format(_T("%s*%s"), strData.Left(2), strData.Right(1));	//4����
		else if(4 < strData.GetLength()) strResult.Format(_T("%s**%s"), strData.Left(2), strData.Mid(4));
		else  return strResult;
	}
	else if(2 == nType)
	{	// YYYY/DD/DD
		// 18.08.09 JMJ KU0457
		if(10 != strData.GetLength()){
			strResult = strData;
		}else{
			strResult.Format(_T("**%s/**/**"), strData.Mid(2, 2));
			//strResult = _T("****/**/**");
		}
	}
	else if(3 == nType)
	{	// 010-1234-5678 , 02-123-5678, 010-233-6868, 02-3244-3434
		INT nFirstIds, nLastIds;
		nFirstIds = strData.Find(_T("-"), 0);
		if(0 >= nFirstIds)
		{
			if(7 <= strData.GetLength()) strResult.Format(_T("%s***%s"), strData.Left(3), strData.Mid(6));
			else return strResult;
		}
		nLastIds = strData.Find(_T("-"), nFirstIds+1);
		if(0 >= nLastIds)
		{
			if(7 <= strData.GetLength()) strResult.Format(_T("%s***%s"), strData.Left(3), strData.Mid(6));
			else return strResult;
		}
		else strResult.Format(_T("%s****%s"), strData.Left(nFirstIds+1), strData.Mid(nLastIds));
	}
	else if(4 == nType)
	{
		if(8 > strData.GetLength()) return strResult;
		else strResult.Format(_T("%s**************"), strData.Left(8));
	}
	else if(5 == nType)
	{
		if(4 > strData.GetLength()) return strResult;
		else if(14 == strData.GetLength()) strResult.Format(_T("%s****%s"), strData.Left(8), strData.Right(2));
		else if(13 == strData.GetLength()) strResult.Format(_T("%s****%s"), strData.Left(7), strData.Right(2));
		else if(12 == strData.GetLength()) strResult.Format(_T("%s****%s"), strData.Left(6), strData.Right(2));
		else if(11 == strData.GetLength()) strResult.Format(_T("%s****%s"), strData.Left(5), strData.Right(2));
		else if(10 == strData.GetLength()) strResult.Format(_T("%s****%s"), strData.Left(4), strData.Right(2));
		else if(9 == strData.GetLength()) strResult.Format(_T("%s***%s"), strData.Left(3), strData.Right(3));
		else if(8 == strData.GetLength()) strResult.Format(_T("%s***%s"), strData.Left(2), strData.Right(3));
		else if(7 == strData.GetLength()) strResult.Format(_T("%s***%s"), strData.Left(2), strData.Right(2));
		else if(6 == strData.GetLength()) strResult.Format(_T("%s**%s"), strData.Left(2), strData.Right(2));
		else if(5 == strData.GetLength()) strResult.Format(_T("%s**%s"), strData.Left(1), strData.Right(2));
		else if(4 == strData.GetLength()) strResult.Format(_T("%s*%s"), strData.Left(1), strData.Right(2));
	}
	// 18.08.09 JMJ KU0457
	else if(6 == nType)
	{
		INT i = 0;
		strResult.Empty();
		for(i = 0; i < strData.GetLength(); i++){
			strResult += _T("*");
		}
	}
	
	return strResult;
EFS_END
return _T("");
}

// KU0306 JMC 20170417 �ڶ� ��� ȭ���� ��Ϲ�ȣ�Է�ȭ�鿡�� ���չ��ڵ� �����ϱ�
INT CESL_Mgr::ConvertILLRegNoTONormal(CString strIn, CString& strLibCode, CString& strRegNo12, CString& strRegCode, BOOL bDBSearchRegCode )
{
	INT ids= -1;
	CString strQuery = _T(""), strResult = _T("");
	strLibCode = _T("");
	strRegNo12 = _T("");
	strRegCode = _T("");

	if ( strIn.GetLength() == 19 && strIn.Left(1) == _T("$") ) {
		strLibCode = strIn.Mid(1, 6);
		strRegNo12 = strIn.Mid(7);
	}
	else if ( strIn.GetLength() == 18 ) {
		strLibCode = strIn.Mid(0, 6);
		strRegNo12 = strIn.Mid(6);
	}
	else return 100;	

	// ��ϱ���
	// �α��ε������� ���ٸ� DB��ȸ���� ���������� ���Ѵ�.
	if ( m_pInfo->LIB_CODE == strLibCode) {
		strRegCode = strRegNo12.Mid(0, m_pInfo->nREG_CODE_LENGTH );
	}
	// �ϴ� ������ุ ����
	else {
		if ( bDBSearchRegCode == FALSE ) return 0;
		strQuery.Format(_T("SELECT REG_CODE FROM BO_BOOK_TBL WHERE REG_NO='%s'"), strRegNo12);

		CESL_DataMgr * pDM = NULL;
		pDM->SetCONNECTION_INFO(CONNECTION_INFO);
		pDM->GetOneValueQuery(strQuery, strResult);
		
		if ( strResult.GetLength() > 0 ) strRegCode = strResult;		
	}	

	return 0;
}
