// MNG_Common_EnvironmentMgr.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_Common_EnvironmentMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_Common_EnvironmentMgr dialog


CMNG_Common_EnvironmentMgr::CMNG_Common_EnvironmentMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_Common_EnvironmentMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_Common_EnvironmentMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid =  NULL;
}
CMNG_Common_EnvironmentMgr::~CMNG_Common_EnvironmentMgr()
{	
	
}
	

VOID CMNG_Common_EnvironmentMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_Common_EnvironmentMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_Common_EnvironmentMgr, CDialog)
	//{{AFX_MSG_MAP(CMNG_Common_EnvironmentMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_Common_EnvironmentMgr message handlers

VOID CMNG_Common_EnvironmentMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CMNG_Common_EnvironmentMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_����ȯ�����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_����ȯ�����"));
	m_pDM = FindDM(_T("DM_����ȯ�����"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_bGridEdit = TRUE;
	m_pGrid->SELEDITOR_RemoveAllColumns();
	m_pGrid->SELEDITOR_AddColumn(4);
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid->SetSort(FALSE);
	if(!Init())
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	  
}

VOID CMNG_Common_EnvironmentMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridCOMMON_ENVIRONMENT_MGR);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 5, cx-10, cy-10);
	pGrid->MoveWindow(0, 0, cx, cy);

}

BOOL CMNG_Common_EnvironmentMgr::Init()
{
	CString strQuery;
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	strQuery.Format(_T("GROUP_2='��Ÿ' AND SYS_SETUP_YN='N' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE );
	//strQuery.Format(_T("GROUP_2='��Ÿ' AND SYS_SETUP_YN = 'N'"));// AND GROUP_3='����'_T(");
	//--2008.10.14 UPDATE BY PWR --}}
	INT ids = m_pDM->RefreshDataManager(strQuery);
	m_pDM->SORT(_T("3�ܰ����"), FALSE);
	if(ids < 0) return FALSE;
	m_pGrid->Display();

	//DB�� �߸� �� �׸� �����Ͽ� DISPLAY
	CString strTmpData;
	INT rowCnt = m_pGrid->GetCount();
	INT nRow;

	for(INT i=0;i<rowCnt;i++)
	{
		if(m_pDM->GetCellData(_T("�����׸�"), i) == _T("�����ڰ�������") ||
			// 2013.11.13 UPDATE BY PWR : 2013���������ռ���Ȯ�뱸��װ��� (����ݳ������߰�)
			m_pDM->GetCellData(_T("�����׸�"), i) == _T("������ϰ�������") ||
			m_pDM->GetCellData(_T("�����׸�"), i) == _T("����ݳ�����"))
//				m_pDM->GetCellData(_T("�����׸�"), i) == _T("������ϰ�������"))
		{
			strTmpData = m_pDM->GetCellData(_T("������"), i);
			m_pGrid->SetTextMatrix(i+1,3,strTmpData);
			strTmpData = m_pDM->GetCellData(_T("������_1"), i);
			if(strTmpData==_T("Y")) strTmpData=_T("�����");
			else if(strTmpData==_T("N")) strTmpData=_T("������");
			m_pGrid->SetTextMatrix(i+1,5,strTmpData);
		}
		else if(m_pDM->GetCellData(_T("�����׸�"), i) == _T("���;��ε��뿩��")||
			m_pDM->GetCellData(_T("�����׸�"), i) == _T("�����ϰ��߼۽ð�����"))
		{
			strTmpData = m_pDM->GetCellData(_T("������_1"), i);
			if(strTmpData==_T("Y")) strTmpData=_T("�����");
			else if(strTmpData==_T("N")) strTmpData=_T("������");
			m_pGrid->SetTextMatrix(i+1,5,strTmpData);
		}
		// 2009.05.22 ADD BY PWR : ���ı��� �߰� (����ݳ� ȭ���� ��Ȳ�׸���)
		else if(m_pDM->GetCellData(_T("�����׸�"), i) == _T("���ı���"))
		{
			strTmpData = m_pDM->GetCellData(_T("������_1"), i);
			if(strTmpData==_T("0")) strTmpData=_T("0");
			else if(strTmpData==_T("1")) strTmpData=_T("1");
			else strTmpData=_T("0");
			m_pGrid->SetTextMatrix(i+1,5,strTmpData);
		}
	}
	return TRUE;
}

VOID CMNG_Common_EnvironmentMgr::SaveManageData()
{
	INT cnt = m_pDM->GetRowCount();
	m_pDM->StartFrame();
	CString strKey;
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	CString strTmp;

	for(INT i=0 ; i<cnt ; i++)
	{
		strTmp = m_pGrid->GetTextMatrix(i,3);
		if(strTmp==_T("�����ϰ��߼۽ð�����"))
		{
			strTmp=m_pGrid->GetTextMatrix(i,5);
			m_pDM->SetCellData(_T("������_1"), m_pGrid->GetTextMatrix(i,5), i-1);
		}
		else if(strTmp==_T("��ȣ��������"))
		{
			strTmp=m_pGrid->GetTextMatrix(i,5);
			m_pDM->SetCellData(_T("������"),  m_pGrid->GetTextMatrix(i,5), i-1);
		}
	}

	for(i=0 ; i<cnt ; i++)
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
	ESLAfxMessageBox(_T("������������ �����Ͽ����ϴ�."));
}

BOOL CMNG_Common_EnvironmentMgr::Create(CWnd *pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

BOOL CMNG_Common_EnvironmentMgr::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (ESL_GRID_MODIFY == pMsg->message)
	{
		if (pMsg->lParam == 4) 
		{	
			INT nRow = m_pGrid->GetRow();
			CString strTmpData = m_pGrid->GetTextMatrix(nRow, pMsg->lParam+1);
			if(strTmpData == _T("�����")) strTmpData = _T("Y");
			else if(strTmpData == _T("������")) strTmpData = _T("N");

			if(m_pDM->GetCellData(_T("�����׸�"), nRow) != _T("�����ڰ�������") &&
				m_pDM->GetCellData(_T("�����׸�"), nRow) != _T("������ϰ�������") &&
				m_pDM->GetCellData(_T("�����׸�"), nRow) != _T("���;��ε��뿩��") &&
				m_pDM->GetCellData(_T("�����׸�"), nRow) != _T("�����ϰ��߼۽ð�����") &&
				m_pDM->GetCellData(_T("�����׸�"), nRow) != _T("��ȣ��������") && 
				// 2013.11.13 UPDATE BY PWR : 2013���������ռ���Ȯ�뱸��װ��� (����ݳ������߰�)
				m_pDM->GetCellData(_T("�����׸�"), nRow) != _T("���ı���") && 
				m_pDM->GetCellData(_T("�����׸�"), nRow) != _T("����ݳ�����"))
//				m_pDM->GetCellData(_T("�����׸�"), nRow) != _T("���ı���")) // 2009.05.22 ADD BY PWR : ���ı��� �߰� (����ݳ� ȭ���� ��Ȳ�׸���)
			{
				m_pDM->SetCellData(_T("������"), strTmpData, nRow-1);
			}			

			//m_pDM->SetCellData(_T("������"), strTmpData, nRow-1);
		}
	}	
/*	else if(pMsg->wParam == VK_RETURN ) {
		m_pGrid->PostMessage(WM_LBUTTONDBLCLK, NULL, NULL);
	}
*/	return CDialog::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CMNG_Common_EnvironmentMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_Common_EnvironmentMgr)
	ON_EVENT(CMNG_Common_EnvironmentMgr, IDC_gridCOMMON_ENVIRONMENT_MGR, -601 /* DblClick */, OnDblClickgridCOMMONENVIRONMENTMGR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_Common_EnvironmentMgr::OnDblClickgridCOMMONENVIRONMENTMGR() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return;

	if(m_pDM->GetRowCount() == 0) return;
	CString strTmpData;
	INT nRow = m_pGrid->GetRow() - 1;
	INT nCol = m_pGrid->GetCol();
	if(nCol != 5) return ;

	if(m_pDM->GetCellData(_T("�����׸�"), nRow) == _T("�ڷ�ã��˻�����"))
	{
		strTmpData = m_pDM->GetCellData(_T("������"), nRow);
		if(strTmpData == _T("��")) strTmpData = _T("å");
		else if(strTmpData == _T("å")) strTmpData = _T("��");
		m_pDM->SetCellData(_T("������"), strTmpData, nRow);
		m_pGrid->DisplayLine(nRow);
		m_pGrid->PostMessage(ESL_GRID_MODIFY, 0, 0);
		return ;
	}
	else if(m_pDM->GetCellData(_T("�����׸�"), nRow) == _T("�ηϵ�Ϲ�ȣ�ڵ��ο�����") ||
		    m_pDM->GetCellData(_T("�����׸�"), nRow) == _T("�����ϰ��߼�")			   ||
			m_pDM->GetCellData(_T("�����׸�"), nRow) == _T("�ڻ���� �����ڷ�����")	   )
	{
		strTmpData = m_pDM->GetCellData(_T("������"), nRow);
		if(strTmpData == _T("Y")) strTmpData = _T("N");
		else if(strTmpData == _T("N")) strTmpData = _T("Y");
		m_pDM->SetCellData(_T("������"), strTmpData, nRow);
		m_pGrid->DisplayLine(nRow);
		m_pGrid->PostMessage(ESL_GRID_MODIFY, 0, 0);
		return ;
	}
	else if(m_pDM->GetCellData(_T("�����׸�"), nRow) == _T("�����ڰ�������") ||
				m_pDM->GetCellData(_T("�����׸�"), nRow) == _T("������ϰ�������") ||
				// 2013.11.13 UPDATE BY PWR : 2013���������ռ���Ȯ�뱸��װ��� (����ݳ������߰�)
				m_pDM->GetCellData(_T("�����׸�"), nRow) == _T("���;��ε��뿩��") ||
				m_pDM->GetCellData(_T("�����׸�"), nRow) == _T("����ݳ�����"))
//				m_pDM->GetCellData(_T("�����׸�"), nRow) == _T("���;��ε��뿩��"))
	{
		strTmpData = m_pDM->GetCellData(_T("������_1"), nRow);
		if(strTmpData == _T("Y")) strTmpData = _T("N");
		else if(strTmpData == _T("N")) strTmpData = _T("Y");
		m_pDM->SetCellData(_T("������_1"), strTmpData, nRow);

		m_pGrid->DisplayLine(nRow);
		m_pGrid->PostMessage(ESL_GRID_MODIFY, 0, 0);
		INT rowCnt = m_pGrid->GetCount();
		for(INT i=0;i<rowCnt;i++)
		{
			if(m_pDM->GetCellData(_T("�����׸�"), i) == _T("�����ڰ�������") ||
				// 2013.11.13 UPDATE BY PWR : 2013���������ռ���Ȯ�뱸��װ��� (����ݳ������߰�)
				m_pDM->GetCellData(_T("�����׸�"), i) == _T("������ϰ�������") ||
				m_pDM->GetCellData(_T("�����׸�"), i) == _T("����ݳ�����"))
//					m_pDM->GetCellData(_T("�����׸�"), i) == _T("������ϰ�������"))
			{
				strTmpData = m_pDM->GetCellData(_T("������"), i);
				m_pGrid->SetTextMatrix(i+1,3,strTmpData);
				strTmpData = m_pDM->GetCellData(_T("������_1"), i);
				if(strTmpData==_T("Y")) strTmpData=_T("�����");
				else if(strTmpData==_T("N")) strTmpData=_T("������");
				m_pGrid->SetTextMatrix(i+1,5,strTmpData);
			}
			else if(m_pDM->GetCellData(_T("�����׸�"), i) == _T("���;��ε��뿩��"))
			{
				strTmpData = m_pDM->GetCellData(_T("������_1"), i);
				if(strTmpData==_T("Y")) strTmpData=_T("�����");
				else if(strTmpData==_T("N")) strTmpData=_T("������");
				m_pGrid->SetTextMatrix(i+1,5,strTmpData);
			}
		}

		return ;
	}
	// 2009.05.22 ADD BY PWR : ���ı��� �߰� (����ݳ� ȭ���� ��Ȳ�׸���)
	else if(m_pDM->GetCellData(_T("�����׸�"), nRow) == _T("���ı���"))
	{
		strTmpData = m_pDM->GetCellData(_T("������_1"), nRow);
		if(strTmpData == _T("0")) strTmpData = _T("1");
		else if(strTmpData == _T("1")) strTmpData = _T("0");
		m_pDM->SetCellData(_T("������_1"), strTmpData, nRow);
		m_pGrid->DisplayLine(nRow);
		m_pGrid->SetTextMatrix(nRow+1,nCol,strTmpData);
		m_pGrid->PostMessage(ESL_GRID_MODIFY, 0, 0);
		return ;
	}
}

INT CMNG_Common_EnvironmentMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("������"))
	{
		if(str == _T("Y")) str = _T("�����");
		else if(str == _T("N")) str = _T("������");
	}
	return 0;
}


HBRUSH CMNG_Common_EnvironmentMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
