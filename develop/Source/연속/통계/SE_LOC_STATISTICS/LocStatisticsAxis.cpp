// LocStatisticsAxis.cpp : implementation file
//

#include "stdafx.h"
#include "LocStatisticsAxis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsAxis dialog


CLocStatisticsAxis::CLocStatisticsAxis(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocStatisticsAxis)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strClassName = _T("");
	m_pGrid = NULL;
	m_bCreated = FALSE;
	m_nTYPE = 1; //default
}

CLocStatisticsAxis::~CLocStatisticsAxis()
{
}

VOID CLocStatisticsAxis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocStatisticsAxis)
	DDX_Control(pDX, IDC_treeCOLUMMN_NAME, m_ctrlTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocStatisticsAxis, CDialog)
	//{{AFX_MSG_MAP(CLocStatisticsAxis)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsAxis message handlers
BOOL CLocStatisticsAxis::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	m_bCreated = TRUE;

	return CDialog::Create(IDD, pParentWnd);
}

BOOL CLocStatisticsAxis::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr( _T("����_����_���_���") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_���_���") );
		return FALSE;
	}
	
	CESL_ControlMgr* pCM = FindCM(_T("CM_����_����_���_���"));
	m_pGrid = (CESL_Grid*)pCM->FindControl(_T("�׸���"));
	
	if(!m_pGrid)
	{
		AfxMessageBox(_T("���� ��� �׸��带 ã�� �� �����ϴ�!"));
		return FALSE;
	}

	if(m_strClassName.IsEmpty())
	{
		AfxMessageBox(_T("�� ������ �Էµ��� �ʾҽ��ϴ�!"));
		return FALSE;
	}

	m_TreeImage.Create(IDB_TREEBITMAP, 16, 0, RGB(255, 255, 255));
	
	SetClassCondition();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLocStatisticsAxis::SetGrid(CString strClassName)
{
	//���� KDC�з���ȣ�� ���� ���ǿ� ���� �����Ѵ�
	
	INT nKDCCode = 256;
	INT nDefaultValue = 0;
	CString strKDCCode[256], strKDCDesc[256];
	nDefaultValue = 0;
	
	m_pInfo->pCodeMgr->GetCode(_T("KDC��������ȣ����"), strKDCCode, strKDCDesc, nKDCCode, nDefaultValue);	
	INT nColCnt = m_pInfo->pCodeMgr->GetCodeCount(_T("KDC��������ȣ����"));
	
	INT nRowCnt;
	INT nCode = 256;
	CString strCode[256], strDesc[256];

	if(_T("�谡����") == strClassName) //�谡������ ��� 
	{
		
		if(3 == m_nTYPE) //�ǽð� ����
		{
			GetWorkingStatusDescReal(strDesc);
			nRowCnt = 6;

		}
		else 
		{
			GetWorkingStatusDesc(strDesc);
			nRowCnt = 10;
		}
	
	}
	else //��Ÿ ������ ��� 
	{
		m_pInfo->pCodeMgr->GetCode(strClassName, strCode, strDesc, nCode, nDefaultValue);	
		nRowCnt = m_pInfo->pCodeMgr->GetCodeCount(strClassName);
	}

	m_pGrid->SetCols(0,nColCnt+1);
	m_pGrid->SetRows(nRowCnt+1);

	//�� ����
	m_pGrid->SetRow(0);

	for(INT i=0;i<nColCnt;i++)
	{
		m_pGrid->SetCol(i+1);
		m_pGrid->SetText(strKDCDesc[i]);
	}
	
	//�� ����
	m_pGrid->SetCol(0);

	for(i=0;i<nRowCnt;i++)
	{
		m_pGrid->SetRow(i+1);
		m_pGrid->SetText(strDesc[i]);
	}

	return 0;
}

INT CLocStatisticsAxis::SetTreeCtrl(CString strClassName)
{
	m_ctrlTree.SetImageList( &m_TreeImage , TVSIL_NORMAL );

	m_ctrlTree.DeleteAllItems();

	TVINSERTSTRUCTW tvinsert;

	// �⺻
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE;
	tvinsert.item.state = 0;
	tvinsert.item.stateMask = 0;
	tvinsert.item.cchTextMax = 60;
	tvinsert.item.cChildren = 0;
	tvinsert.item.iSelectedImage = 0;
	
	const INT nCnt = 2;
	TCHAR *sTitleName[nCnt] = {_T("��"),_T("��")};

	HTREEITEM hItem;
	
	CString strDefaultColumn = _T("�з���ȣ��");


	for( INT i = 0 ; i < 2 ; i++ )
	{
		tvinsert.item.iImage = i+1;
		tvinsert.hParent = NULL;
		tvinsert.item.pszText = (LPTSTR)(LPCTSTR)sTitleName[i];//(LPWSTR)sTitleName[i];
		hItem = m_ctrlTree.InsertItem(&tvinsert);
		tvinsert.hParent = hItem;
		

		tvinsert.item.iImage = 3;

		TCHAR sTemp[200];
		
		if(0 == i) 
			_tcscpy( sTemp , strDefaultColumn.GetBuffer(0) );
		else
			_tcscpy( sTemp , strClassName.GetBuffer(0) );

		tvinsert.item.pszText = (LPTSTR)(LPCTSTR)sTemp;
		m_ctrlTree.InsertItem( &tvinsert );
		m_ctrlTree.Expand(hItem,TVE_EXPAND);
		
	}

	
	return 0;
}

VOID CLocStatisticsAxis::ReSizeControl()
{
	if( GetSafeHwnd() == NULL ) return;

	INT cx , cy;
	CRect WndPos;
	GetWindowRect(WndPos);
	cx = WndPos.Width();
	cy = WndPos.Height();
	
	// ������ ����
	if( GetDlgItem(IDC_staRIGHTTITLE)->GetSafeHwnd() == NULL ) return;
	CRect WndRect;
	GetDlgItem(IDC_staRIGHTTITLE)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_staRIGHTTITLE)->MoveWindow( WndRect.left , WndRect.top , cx - WndRect.left - 10 , WndRect.Height() );
	
	// Ʈ��
	if( GetDlgItem(IDC_treeCOLUMMN_NAME)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_treeCOLUMMN_NAME)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_treeCOLUMMN_NAME)->MoveWindow( WndRect.left , WndRect.top , WndRect.Width() , cy - WndRect.top - 50 );
	
	INT nTop;
	nTop = WndRect.top;
	
	// �׸���
	if( GetDlgItem(IDC_gridEXP_GRID)->GetSafeHwnd() == NULL ) return;
	
	GetDlgItem(IDC_gridEXP_GRID)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	
	GetDlgItem(IDC_gridEXP_GRID)->MoveWindow( WndRect.left , nTop , cx - WndRect.left - 10 , cy - nTop - 50 );
	GetDlgItem(IDC_gridEXP_GRID)->RedrawWindow();
	
}

VOID CLocStatisticsAxis::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	ReSizeControl();

}

VOID CLocStatisticsAxis::SetClassCondition()
{
	SetTreeCtrl(m_strClassName);
	SetGrid(m_strClassName);

}

VOID CLocStatisticsAxis::GetWorkingStatusDesc(CString strDesc[])
{
	strDesc[0] = _T("���ܴ����ڷ�");
	strDesc[1] = _T("���������ڷ�");
	strDesc[2] = _T("Ư�������ڷ�");
	strDesc[3] = _T("��޴����ڷ�");
	strDesc[4] = _T("����ڷ�");
	strDesc[5] = _T("��ġ�ڷ�");
	strDesc[6] = _T("���������ڷ�");
	strDesc[7] = _T("�н��ڷ�");
	strDesc[8] = _T("�����ڷ�");
	strDesc[9] = _T("�ļ��ڷ�");
	//=====================================================
	// 18.09.27 JMJ KOL.RED.2018.008
	strDesc[10] = _T("����å�η����������ڷ�");
	//=====================================================
	
}

VOID CLocStatisticsAxis::GetWorkingStatusDescReal(CString strDesc[])
{
	strDesc[0] = _T("�ǽð��谡�ڷ�");
	strDesc[1] = _T("���ܴ����ڷ�");
	strDesc[2] = _T("���������ڷ�");
	strDesc[3] = _T("Ư�������ڷ�");
	strDesc[4] = _T("��޴����ڷ�");
	strDesc[5] = _T("����ڷ�");
	//=====================================================
	// 18.09.27 JMJ KOL.RED.2018.008
	strDesc[5] = _T("����å�η����������ڷ�");
	//=====================================================
	

}
HBRUSH CLocStatisticsAxis::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
