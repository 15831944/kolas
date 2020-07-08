// BudgetListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetListDlg.h"
#include "BudgetApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetListDlg dialog


CBudgetListDlg::CBudgetListDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetListDlg)
	m_bIsDelayedBudget = FALSE;
	//}}AFX_DATA_INIT

	m_nOpenMode = 1; //default
	m_pChildMat = NULL;
	m_pChildUsage = NULL;
	m_pChildExecute = NULL;

	m_arraySubDeleteKey.RemoveAll();
	m_arrayExeDeleteKey.RemoveAll();

	m_pCM = NULL;
	m_pDM = NULL;
	m_pDM_EXE_SUB = NULL;

	m_pParentDM = NULL;	
	m_pParentGrid = NULL;
	m_nCurrentIdx = -1;
	m_nMoveOption = 1;

	m_strBUDGET_COMPILATIONKEY = _T("");
	m_bSubTypeLimited = TRUE;
}

CBudgetListDlg::~CBudgetListDlg()
{
	FreeChild();
}


VOID CBudgetListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetListDlg)
	DDX_Control(pDX, IDC_tabBUDGET_LIST, m_ctrlTab);
	DDX_Check(pDX, IDC_chkBUDGET_LIST_DELAYED_YN, m_bIsDelayedBudget);
	//}}AFX_DATA_MAP
}

BOOL CBudgetListDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CBudgetListDlg, CDialog)
	//{{AFX_MSG_MAP(CBudgetListDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabBUDGET_LIST, OnSelchangetabBUDGETLIST)
	ON_BN_CLICKED(IDC_chkBUDGET_LIST_DELAYED_YN, OnchkBUDGETLISTDELAYEDYN)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_PREV, OnbtnBUDGETLISTPREV)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_NEXT, OnbtnBUDGETLISTNEXT)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_EXECUTION_INFO, OnbtnBUDGETLISTEXECUTIONINFO)
	ON_MESSAGE(IDS_REFRESHALL,OnRefreshAll)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetListDlg message handlers

BOOL CBudgetListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	if(InitESL_Mgr( _T("�������_������") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : �������_������") );
		return false;
	}
		
	m_pCM = FindCM(_T("CM_������"));
	m_pDM = FindDM(_T("DM_������"));
	m_pDM_EXE_SUB = FindDM(_T("DM_�������ຸ��"));

	if(!m_pCM || !m_pDM || !m_pDM_EXE_SUB) {
		AfxMessageBox( _T("CM,DM Error : CM_������,DM_������,DM_�������ຸ��") );
		return FALSE;
	}

	//modified by loveisverb 
	//2004-12-22
	//ECOLAS�� K2UP�� �޸� ������� ���� ������ ������� �ʴ´�.
	//m_bSubTypeLimited = IsBudgetSubTypeLimited(m_pDM);
	m_bSubTypeLimited = FALSE;

	INT codeCnt = m_pInfo->pCodeMgr->GetCodeCount(_T("������ⱸ��"));

	CRect rect;
	this->GetClientRect(rect);
	
	rect.top += 265;
	rect.bottom -= 62;
	rect.left += 35;
	rect.right -= 35;

//////////////////////////////////////////////////////////////////////
		m_pChildForm = new CBudgetListForm(this);
		m_pChildForm->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildForm->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildForm->Create(this);
		m_pChildForm->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildMat = new CBudgetListMat(this);
		m_pChildMat->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildMat->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildMat->Create(this);
		m_pChildMat->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildMedia = new CBudgetListMedia(this);
		m_pChildMedia->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildMedia->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildMedia->Create(this);
		m_pChildMedia->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildSubReg = new CBudgetListSubReg(this);
		m_pChildSubReg->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildSubReg->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildSubReg->Create(this);
		m_pChildSubReg->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildUseObj = new CBudgetListUseObj(this);
		m_pChildUseObj->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildUseObj->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildUseObj->Create(this);
		m_pChildUseObj->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildManage = new CBudgetListManage(this);
		m_pChildManage->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildManage->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildManage->Create(this);
		m_pChildManage->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildUsage = new CBudgetListUsage(this);
		m_pChildUsage->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildUsage->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildUsage->Create(this);
		m_pChildUsage->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildCource = new CBudgetListCource(this);
		m_pChildCource->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildCource->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildCource->Create(this);
		m_pChildCource->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////


		m_pChildDept = new CBudgetListDept(this);
		m_pChildDept->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildDept->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildDept->Create(this);
		m_pChildDept->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildExecute = new CBudgetExecutionMgr(this);
		m_pChildExecute->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildExecute->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildExecute->Create(this);
		//m_pChildExecute->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////
	CString strTmp;
	INT startrow = 0;
	INT nDeptYN = 0;
	
	//���翡�� ��ó���� �ֿ켱���� �䱸.
	for ( INT nIdx = 0 ; nIdx < codeCnt ; nIdx++ )
	{
		startrow = m_pInfo->pCodeMgr->GetCode(_T("������ⱸ��"), _T("�ڵ�"), strTmp, startrow);
		startrow++;
		if ( strTmp == _T("�кκ�") )
		{		
			m_ctrlTab.InsertItem(0, _T("��ó��") );
			m_pChildDept->ShowWindow(SW_SHOW);
			nDeptYN = 1;
		}
	}
	
	startrow = 0;

	for ( nIdx = 0 ; nIdx < codeCnt ; nIdx++ )
	{
		startrow = m_pInfo->pCodeMgr->GetCode(_T("������ⱸ��"), _T("�ڵ�"), strTmp, startrow);
		startrow++;

		INT nRow = m_ctrlTab.GetRowCount();

		if( nDeptYN == 1 && strTmp == _T("�кκ�") )
			continue;

		if ( strTmp == _T("�кκ�") )
		{		
			if ( nRow == 0 )
				m_pChildDept->ShowWindow(SW_SHOW);
			else 
				m_pChildDept->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, _T("��ó��") );
		}
		else if ( strTmp == _T("���ı���") )
		{

			if ( nRow == 0 )
				m_pChildForm->ShowWindow(SW_SHOW);
			else
				m_pChildForm->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("�ڷᱸ��") )
		{
			if ( nRow == 0 )
				m_pChildMat->ShowWindow(SW_SHOW);
			else 
				m_pChildMat->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("��ü����") )
		{
			if ( nRow == 0 )
				m_pChildMedia->ShowWindow(SW_SHOW);
			else 
				m_pChildMedia->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("������ϱ���") )
		{
			if ( nRow == 0 )
				m_pChildSubReg->ShowWindow(SW_SHOW);
			else 
				m_pChildSubReg->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("�̿��󱸺�") )
		{
			if ( nRow == 0 )
				m_pChildUseObj->ShowWindow(SW_SHOW);
			else 
				m_pChildUseObj->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("��������") )
		{		
			if ( nRow == 0 )
				m_pChildManage->ShowWindow(SW_SHOW);
			else 
				m_pChildManage->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("�뵵����") )
		{		
			if ( nRow == 0 )
				m_pChildUsage->ShowWindow(SW_SHOW);
			else 
				m_pChildUsage->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("�а���") )
		{		
			if ( nRow == 0 )
				m_pChildCource->ShowWindow(SW_SHOW);
			else 
				m_pChildCource->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nIdx, strTmp );
		}
	}

	if(InitOpenMode()<0)
	{
		EndDialog(0);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBudgetListDlg::OnRefreshAll()
{
	if(!m_strBUDGET_COMPILATIONKEY.IsEmpty())
		RefreshAll(m_strBUDGET_COMPILATIONKEY);
}

INT CBudgetListDlg::InitOpenMode()
{
	
	if(m_nOpenMode>0) //�Է� 
	{
	
		GetDlgItem(IDC_btnBUDGET_LIST_PREV)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnBUDGET_LIST_NEXT)->ShowWindow(FALSE);

		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtBUDGET_LIST_CF_BUDGET);
		if(pEdit) pEdit->SetReadOnly(TRUE);
	}
	else //����
	{
		//�θ� �׸��忡�� ���������� ���´�
		if(!m_pParentGrid) {
			AfxMessageBox(_T("�θ� �׸��带 ã�� �� �����ϴ�!"));
			return -1;
		}

		if(!m_pParentDM) {
			AfxMessageBox(_T("�θ� DM�� ã�� �� �����ϴ�!"));
			return -2;
		}
		
	
		if(m_nCurrentIdx<0) {
			AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
			return -3;
		}
	
		m_strBUDGET_COMPILATIONKEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);

		if(m_strBUDGET_COMPILATIONKEY.IsEmpty())
		{
			AfxMessageBox(_T("�ش� KEY�� ã�� �� �����ϴ�!"));
			return -4;
		}
		
		if(!m_strBUDGET_COMPILATIONKEY.IsEmpty())
			RefreshAll(m_strBUDGET_COMPILATIONKEY);		
	}

	return 0;

}

VOID CBudgetListDlg::FreeChild()
{
	if(m_pChildMat) delete m_pChildMat;
	m_pChildMat = NULL;
	
	if(m_pChildUsage) delete m_pChildUsage;
	m_pChildUsage = NULL;

	if(m_pChildForm) delete m_pChildForm;
	m_pChildForm = NULL;

	if(m_pChildMedia) delete m_pChildMedia;
	m_pChildMedia = NULL;

	if(m_pChildSubReg) delete m_pChildSubReg;
	m_pChildSubReg = NULL;

	if(m_pChildUseObj) delete m_pChildUseObj;
	m_pChildUseObj = NULL;

	if(m_pChildManage) delete m_pChildManage;
	m_pChildManage = NULL;

	if(m_pChildCource) delete m_pChildCource;
	m_pChildCource = NULL;

	if(m_pChildDept) delete m_pChildDept;
	m_pChildDept = NULL;

	if(m_pChildExecute) delete m_pChildExecute;
	m_pChildExecute = NULL;

}

VOID CBudgetListDlg::OnSelchangetabBUDGETLIST(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT nCurSel = m_ctrlTab.GetCurSel();

	TC_ITEM tcm;
	CString strTabMenu;

	TCHAR	szBuff[64];
	tcm.mask = TCIF_TEXT;
	tcm.pszText = szBuff;
	tcm.cchTextMax = sizeof(szBuff);
	m_ctrlTab.GetItem(nCurSel, &tcm );
	strTabMenu.Format(_T("%s"), tcm.pszText );

	if ( strTabMenu == _T("���ı���") )
	{
		m_pChildForm->ShowWindow(SW_SHOW);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);

	}
	else if ( strTabMenu == _T("�ڷᱸ��") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_SHOW);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("��ü����") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_SHOW);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("������ϱ���") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_SHOW);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("�뵵����") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_SHOW);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("�̿��󱸺�") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_SHOW);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("��������") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_SHOW);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("�а���") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_SHOW);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("��ó��") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}

INT CBudgetListDlg::SaveControlDataToDM()
{
	
	if(m_nOpenMode>0) m_pDM->InsertRow(-1);


	//DM���� ����������� data��
	const INT cnt = 9;
	CString alias[ cnt ] = {
		_T("�������⵵"),				//	0
		_T("����������"),				//	1
		_T("����������"),				//	2
		_T("��������"),				//	3
		_T("��������"),				//	4
		_T("�̿��ȿ����"),				//	5
		_T("�̿��ҿ����"),				//	6
		_T("��������"),				//	7
		_T("��������������")			//	8
		};
	
	CString tmp;

	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");	
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM->SetCellData(alias[i],tmp,0);
	}
	
	UpdateData(TRUE);
	
	//�̿�ó������
	if(m_bIsDelayedBudget)
		m_pDM->SetCellData(_T("�̿�ó������"),_T("Y"),0);
	else 
		m_pDM->SetCellData(_T("�̿�ó������"),_T("N"),0);

	return 0;
}

VOID CBudgetListDlg::OnOK() 
{
	// TODO: Add extra validation here
	//�����Լ�

	CString strBUDGET_YEAR;
	m_pCM->GetControlMgrData(_T("�������⵵"),strBUDGET_YEAR);

	if(strBUDGET_YEAR.IsEmpty()) 
	{
		AfxMessageBox(_T("�������⵵�� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}

	CString strBUDGET_CLASS;
	m_pCM->GetControlMgrData(_T("����������"),strBUDGET_CLASS);

	if(strBUDGET_CLASS.IsEmpty()) 
	{
		AfxMessageBox(_T("������������ �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}

	SaveControlDataToDM();
	
	CBudgetApi *pApi = NULL;
	pApi = new CBudgetApi (this);
	INT ids = -1;

	if(m_nOpenMode>0)
	{

		ids = pApi->InsertBudgetList(m_pDM,
								   m_pChildMat->m_pDM, 
								   m_pChildUsage->m_pDM,
								   m_pChildForm->m_pDM,
								   m_pChildMedia->m_pDM,
								   m_pChildSubReg->m_pDM,
								   m_pChildUseObj->m_pDM,
								   m_pChildManage->m_pDM,
								   m_pChildCource->m_pDM,
								   m_pChildDept->m_pDM,
								   m_pChildExecute->m_pDM
								   );
	}
	else
	{

		ids = pApi->UpdateBudgetList(m_pDM,
								   m_pChildMat->m_pDM, 
								   m_pChildUsage->m_pDM,
								   m_pChildForm->m_pDM,
								   m_pChildMedia->m_pDM,
								   m_pChildSubReg->m_pDM,
								   m_pChildUseObj->m_pDM,
								   m_pChildManage->m_pDM,
								   m_pChildCource->m_pDM,
								   m_pChildDept->m_pDM,
								   m_pChildExecute->m_pDM,
								   m_arraySubDeleteKey,
	                               m_arrayExeDeleteKey);
	}
	

	if(ids<0)
	{
		AfxMessageBox(_T("���忡 �����Ͽ����ϴ�!"));
		return;
	}
	else
	{
		if(m_nOpenMode>0) 
		{
			AfxMessageBox(_T("�Է��Ͽ����ϴ�!"));
			m_strBUDGET_COMPILATIONKEY = m_pDM->GetCellData(_T("REC_KEY"),0);
		}
		else 
		{
			AfxMessageBox(_T("�����Ͽ����ϴ�!"));
		}

		RefreshParent();
		RefreshAll(m_strBUDGET_COMPILATIONKEY);		
		return;
	}


	CDialog::OnOK();
}

VOID CBudgetListDlg::OnchkBUDGETLISTDELAYEDYN() 
{
	// TODO: Add your control notification handler code here
	//�̿� �Ǿ��� ��� - �̿��� �޾׶��� Ȱ��ȭ 

	UpdateData(TRUE);
	
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtBUDGET_LIST_CF_BUDGET);

	//�̿�ó������
	if(m_bIsDelayedBudget)
	{
		if(pEdit) pEdit->SetReadOnly(FALSE);
	}
	else
	{
		if(pEdit) pEdit->SetReadOnly(TRUE);
	}

}

INT CBudgetListDlg::RefreshAll(CString strREC_KEY)
{
	
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strREC_KEY);


	m_pDM->RefreshDataManager(strWhere);
	m_pCM->AllControlDisplay(0);
	

	//�������ຸ�� 
	strWhere.Format(_T("BUDGET_EXECUTION_KEY IN (SELECT REC_KEY FROM CO_BUDGET_EXECUTION_TBL WHERE ")
					_T("BUDGET_COMPILATION_KEY = %s)"),strREC_KEY);
	m_pDM_EXE_SUB->RefreshDataManager(strWhere);



	//delete key refresh
	m_arraySubDeleteKey.RemoveAll();
	m_arrayExeDeleteKey.RemoveAll();


	//child refresh
	m_pChildMat->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildUsage->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildForm->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildMedia->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildSubReg->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildUseObj->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildManage->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildCource->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildDept->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildExecute->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	
	m_pChildMat->RefreshAll();
	m_pChildUsage->RefreshAll();
	m_pChildForm->RefreshAll();
	m_pChildMedia->RefreshAll();
	m_pChildSubReg->RefreshAll();
	m_pChildUseObj->RefreshAll();
	m_pChildManage->RefreshAll();
	m_pChildCource->RefreshAll();
	m_pChildDept->RefreshAll();
	m_pChildExecute->RefreshAll();

	return 0;
}

VOID CBudgetListDlg::OnbtnBUDGETLISTPREV() 
{
	// TODO: Add your control notification handler code here
	if(1 == m_nMoveOption)
	{
		m_nCurrentIdx--;

		if(m_nCurrentIdx<0)
		{
			AfxMessageBox(_T("ó�� �׸��Դϴ�!"));
			m_nCurrentIdx++;
			return;
		}
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetPrev();

		if(nIdx<0) 
		{
			INT nHeadIdx = m_pParentGrid->SelectGetHeadPosition();
			
			if(m_nCurrentIdx == nHeadIdx)
			{
				AfxMessageBox(_T("ó�� �׸��Դϴ�!"));
				m_pParentGrid->SelectGetHeadPosition();
				return;
			}
			else //���� ���� ������ ���� ��� �� ���� ���������� ���Ѵ� 
			{
				m_nCurrentIdx = nHeadIdx;
			}
		
		}
		else //���� ������ �������� �����δ�
		{
			m_nCurrentIdx = nIdx;
		}

	}

	m_strBUDGET_COMPILATIONKEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
		
	if(!m_strBUDGET_COMPILATIONKEY.IsEmpty())
			RefreshAll(m_strBUDGET_COMPILATIONKEY);

	m_pParentGrid->SetReverse(m_nCurrentIdx);
}

VOID CBudgetListDlg::OnbtnBUDGETLISTNEXT() 
{
	// TODO: Add your control notification handler code here
		if(1 == m_nMoveOption)
	{
		m_nCurrentIdx++;
		INT nMaxIdx = m_pParentDM->GetRowCount()-1;
		if(m_nCurrentIdx > nMaxIdx)
		{
			AfxMessageBox(_T("������ �׸��Դϴ�!"));
			m_nCurrentIdx--;
			return;
		}
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetNext();

		if(nIdx<0) 
		{
			INT nTailIdx = m_pParentGrid->SelectGetTailPosition();
			
			if(m_nCurrentIdx == nTailIdx)
			{
				AfxMessageBox(_T("������ �׸��Դϴ�!"));
				m_pParentGrid->SelectGetTailPosition();
				return;
			}
			else //���� ���� ������ ���� ��� �ǵ��� ���������� ���Ѵ� 
			{
				m_nCurrentIdx = nTailIdx;
			}
		}
		else //���� ������ �������� �����δ�
		{
			m_nCurrentIdx = nIdx;
		}
	}


	m_strBUDGET_COMPILATIONKEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
		
	if(!m_strBUDGET_COMPILATIONKEY.IsEmpty())
		RefreshAll(m_strBUDGET_COMPILATIONKEY);

	m_pParentGrid->SetReverse(m_nCurrentIdx);
}

VOID CBudgetListDlg::OnbtnBUDGETLISTEXECUTIONINFO() 
{
	// TODO: Add your control notification handler code here
	
	if(m_pChildExecute)
	{
		m_pChildExecute->CenterWindow();
		m_pChildExecute->ShowWindow(SW_SHOW);	
	}

}
VOID CBudgetListDlg::RefreshParent()
{
	//�θ� DM�� ������ DM�� copy���ְ� grid�� display�Ѵ�
	INT idx;
	
	if(m_nOpenMode>0) //�Է� 
	{
		m_pParentDM->InsertRow(-1);
		idx = m_pParentDM->GetRowCount()-1;
	}	
	else
	{
		
		idx = m_nCurrentIdx;
	}


	CopyDMToDMByAlias(m_pDM,0,m_pParentDM,idx);
	

	CString strTmp;	
	CString strDesc;

	//UDF_����������
	strTmp = m_pDM->GetCellData(_T("����������"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����������"),strTmp,strDesc);
	m_pParentDM->SetCellData(_T("UDF_����������"),strDesc,idx);


	//UDF_����������
	strTmp = m_pDM->GetCellData(_T("����������"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����������"),strTmp,strDesc);
	m_pParentDM->SetCellData(_T("UDF_����������"),strDesc,idx);
	

	m_pParentGrid->DisplayLine(idx);

}

INT CBudgetListDlg::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

BOOL CBudgetListDlg::IsBudgetSubTypeLimited(CESL_DataMgr* pDM)
{
	CString strQuery;
	CString strResult;

	strQuery.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'BUDGETMNGLIMIT'"));

	pDM->GetOneValueQuery(strQuery,strResult);

	strResult.TrimLeft();
	strResult.TrimRight();

	if(strResult.IsEmpty() || _T("N") == strResult) return FALSE;

	return TRUE;
}


HBRUSH CBudgetListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
