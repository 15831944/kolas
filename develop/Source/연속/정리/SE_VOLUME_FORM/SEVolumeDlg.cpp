// SEVolumeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SEVolumeDlg.h"

// 040327_HSS ���Ӽ���api
#include "..\..\����\SE_API\SeApi.h"

// ���Է�
#include "SE_CAT_INPUT_VOL.h"
// å�Է�
#include "SE_CAT_INPUT_BOOK.h"
// �η��Է�
#include "SE_CAT_INPUT_APPENDIX.h"

#include "..\..\..\����\MarcAdjust\MarcAdjustConstant.h"

// ǥ������
#include "..\..\..\����\COVERMANAGER\COVER_MAIN.h"
#include "webbrowser2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSEVolumeDlg dialog


CSEVolumeDlg::CSEVolumeDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSEVolumeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCatApi = NULL;

	m_strBookGroupName = _T("å����");
	m_strVolumeGroupName = _T("��ȣ����");
	m_strHoldBookGroupName = _T("����å����");
	m_strAppendixGroupName = _T("�η�����");
	m_strHoldAppendixGroupName = _T("����η�����");
	m_strSelectedGroupName = _T("");

	m_strVolumeGrid = _T("GRID_VOLUME");
	m_strBookGrid = _T("GRID_BOOK");
	m_strHoldBookGrid = _T("GRID_HOLD_BOOK");
	m_strAppendixGrid = _T("GRID_APPENDIX");
	m_strHoldAppendixGrid = _T("GRID_HOLD_APPENDIX");
	m_strSelectedGrid = _T("");

	m_pGrid_Volume = NULL;
	m_pGrid_Book = NULL;
	m_pGrid_HoldBook = NULL;
	m_pGrid_Appendix = NULL;
	m_pGrid_HoldAppendix = NULL;
	m_pGrid_Selected = NULL;

	m_pDM_Volume = NULL;
	m_pDM_Book = NULL;
	m_pDM_HoldBook = NULL;
	m_pDM_Appendix = NULL;
	m_pDM_HoldAppendix = NULL;
	m_pDM_Selected = NULL;

	m_bGripperSizing = FALSE;
}

CSEVolumeDlg::~CSEVolumeDlg()
{
}

VOID CSEVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSEVolumeDlg)
	DDX_Control(pDX, IDC_STATIC_GRID_GUIDLINE, m_GroupBoxCtrl);
	DDX_Control(pDX, IDC_TAB_SVF, m_TabCtrl);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSEVolumeDlg, CDialog)
	//{{AFX_MSG_MAP(CSEVolumeDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SVF, OnSelchangeTabSvf)
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_bCONTENT_MGR, OnbCONTENTMGR)
	ON_BN_CLICKED(IDC_bDEL_VOL_BOOK, OnbDELETE)
	ON_BN_CLICKED(IDC_bINPUT_APPENDIX, OnbINPUT_APPENDIX)
	ON_BN_CLICKED(IDC_bINPUT_BOOK, OnbINPUT_BOOK)
	ON_BN_CLICKED(IDC_bINPUT_VOLUME, OnbINPUT_VOLUME)
	ON_BN_CLICKED(IDC_bCOVER, OnbCOVER)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MPROPERTY_GRID_UPDATE, OnMPropertyGridUpdate)
	ON_MESSAGE(WM_MPROPERTY_GRID_GET_DATA, OnMPropertyGridGetData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEVolumeDlg Operations

INT CSEVolumeDlg::Init()
{
	EFS_BEGIN

	if (m_pCatApi == NULL) return -1;
	
	if (!m_pDM_Volume || !m_pDM_Book || !m_pDM_HoldBook || !m_pDM_Appendix || !m_pDM_HoldAppendix) return -1;

	m_pGrid_Volume = static_cast<CESL_Grid*>(FindControl(_T("CM_����_��ȣ����"), m_strVolumeGrid));
	m_pGrid_Book = static_cast<CESL_Grid*>(FindControl(_T("CM_����_��ȣ����"), m_strBookGrid));
	m_pGrid_HoldBook = static_cast<CESL_Grid*>(FindControl(_T("CM_����_��ȣ����"), m_strHoldBookGrid));
	m_pGrid_Appendix = static_cast<CESL_Grid*>(FindControl(_T("CM_����_��ȣ����"), m_strAppendixGrid));
	m_pGrid_HoldAppendix = static_cast<CESL_Grid*>(FindControl(_T("CM_����_��ȣ����"), m_strHoldAppendixGrid));
	if (!m_pGrid_Volume || !m_pGrid_Book || !m_pGrid_HoldBook || !m_pGrid_Appendix || !m_pGrid_HoldAppendix ) return -1;

	m_TabCtrl.InsertItem(0, _T("��ȣ����"));
	m_TabCtrl.InsertItem(1, _T("å����"));
	m_TabCtrl.InsertItem(2, _T("�η�����"));
	m_TabCtrl.InsertItem(3, _T("����å����"));
	m_TabCtrl.InsertItem(4, _T("����η�����"));

	m_pGrid_Volume->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid_Book->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid_HoldBook->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid_Appendix->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid_HoldAppendix->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	//m_strDMAlias_Volume = _T("DM_����_����DM_������");
	//m_strDMAlias_Book = _T("DM_����_����DM_å����");

	// 040930_HSS �Է��� ��� �����ʷϰ��� �Ⱥ��̰�
	if (m_nAppMode == MNG_INSERT)
	{
		CWnd *pWndContent = GetDlgItem(IDC_bCONTENT_MGR);
		pWndContent->ShowWindow(SW_HIDE);
	}

	// ǥ������ ��� ���ο� ���� ��ư ����� 041129_����
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO);

	CString strQuery, strReturnValue;
	strQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USECOVERIMG'");
	dm.GetOneValueQuery(strQuery, strReturnValue);
	if ( strReturnValue != _T("Y") )
	{
		GetDlgItem(IDC_bCOVER)->ShowWindow(FALSE);
	}

	INT ids = -1;

	ids = InitGridData(0);

	AllControlDisplay(_T("CM_����_��ȣ����"), 0);
	
	Display();

	return 0;

	EFS_END
	return -1;
}

// ���� ROW ���� �� ȭ�� Data ����
INT CSEVolumeDlg::InitGridData(INT nFlag)
{
	EFS_BEGIN

	m_grid.Init();
	m_grid.SetFixedColCount(1);
	m_grid.SetFixedColWidth(100);
	m_grid.SetDataColWidth(100);

	// ��ȣ���� �׷�
	
	//m_grid.AddItem(_T("��   ǥ   ��"), CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T(""), _T(""), _T(""));
	//m_grid.AddItem(_T("��        ��"), CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, );
	//m_grid.AddItem(_T("�� �� ǥ ��"), CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T(""));
	//m_grid.AddItem(_T("��   ��   ��"), CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("001"), _T("!�ڷ�Ǳ���.�ڵ�,!�ڷ�Ǳ���.����"));
	
	m_grid.AddItem(_T("��ȣ�Ϸù�ȣ"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_���Ϸù�ȣ"));
	m_grid.AddItem(_T("�� �� �� ��"), m_strVolumeGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SV_���ı���_�ڵ�"), _T("!���ı���.�ڵ�,!���ı���.����"));
	m_grid.AddItem(_T("�� ü �� ��"), m_strVolumeGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SV_��ü����_�ڵ�"), _T("!��ü����.�ڵ�,!��ü����.����"));
	m_grid.AddItem(_T("��       ȣ"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_��ȣ��"));
	m_grid.AddItem(_T("������ ��ȣ"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_��������ȣ��"));
	m_grid.AddItem(_T("��   ��   ��"), m_strVolumeGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SV_��������"));
	m_grid.AddItem(_T("��        ��"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_����"));
	m_grid.AddItem(_T("ȭ �� �� ��"), m_strVolumeGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SV_ȭ�󱸺�"), _T("!ȭ���ȣ����.�ڵ�,!ȭ���ȣ����.����"));
	m_grid.AddItem(_T("�� �� �� ��"), m_strVolumeGroupName, CMPropertyItem::typeComboBox, CMPropertyItem::styleNormal, _T("SV_���ݼ���"), _T("����,���ΰ�"));
	m_grid.AddItem(_T("��   ��   ��"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_�����"));
	m_grid.AddItem(_T("������ Ư��"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_������Ư��"));
	m_grid.AddItem(_T("ũ        ��"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_åũ��"));
	m_grid.AddItem(_T("�� �� �� ��"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_�����ڷ�"));
	m_grid.AddItem(_T("��ȣ�� ����"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_��ȣ������"));
	m_grid.AddItem(_T("��       ��"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_���"));

	// �� Color ����
	m_grid.SetGroupColor(m_strVolumeGroupName, RGB(208, 230, 238));

	// å���� �׷�
	if (nFlag==0)		// å
	{
		m_grid.AddItem(_T("�� �� �� ��"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_���Ա���_�ڵ�"), _T("!���Ա���.�ڵ�,!���Ա���.����"));
		m_grid.AddItem(_T("��  ��  ��"), m_strBookGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_�Լ���"));
		m_grid.AddItem(_T("å  ��  ��"), m_strBookGroupName, CMPropertyItem::typeComboBox, CMPropertyItem::styleReadOnly, _T("SB_�ڷ����"));
		m_grid.AddItem(_T("��  ��  ��"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleReadOnly, _T("SB_�谡��ġ��ȣ"), _T("!�ڷ�Ǳ���.�ڵ�,!�ڷ�Ǳ���.����"));
		m_grid.AddItem(_T("��      ��"), m_strBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_���"));
		m_grid.AddItem(_T("��/������ȣ"), m_strBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_��å��ȣ"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_�з���ȣ"));
		m_grid.AddItem(_T("�� ġ �� ȣ"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_��ġ��ȣ_�ڵ�"), _T("!��ġ��ȣ.�ڵ�,!��ġ��ȣ.����"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_��������_�ڵ�"), _T("!��������.�ڵ�,!��������.����"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strBookGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_���ʹ谡����"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_������ȣ"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_��ϱ���_�ڵ�"), _T("!��ϱ���.�ڵ�,!��ϱ���.����"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleReadOnly, _T("SB_��Ϲ�ȣ"));

		// å Color ����
		m_grid.SetGroupColor(m_strBookGroupName, RGB(220, 238, 208));
	}
	else if (nFlag==1)	// �η�
	{
		m_grid.AddItem(_T("��  ��  ��"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SA_�ηϸ�"));
		m_grid.AddItem(_T("�η����ı���"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SA_���ı���"), _T("!���ı���.�ڵ�,!���ı���.����"));
		m_grid.AddItem(_T("�ηϸ�ü����"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SA_��ü����"), _T("!��ü����.�ڵ�,!��ü����.����"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_���Ա���_�ڵ�"), _T("!���Ա���.�ڵ�,!���Ա���.����"));
		m_grid.AddItem(_T("��  ��  ��"), m_strAppendixGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_�Լ���"));
		m_grid.AddItem(_T("å  ��  ��"), m_strAppendixGroupName, CMPropertyItem::typeComboBox, CMPropertyItem::styleReadOnly, _T("SB_�ڷ����"));
		m_grid.AddItem(_T("��  ��  ��"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleReadOnly, _T("SB_�谡��ġ��ȣ"), _T("!�ڷ�Ǳ���.�ڵ�,!�ڷ�Ǳ���.����"));
		m_grid.AddItem(_T("��      ��"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_���"));
		m_grid.AddItem(_T("��/������ȣ"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_��å��ȣ"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_�з���ȣ"));
		m_grid.AddItem(_T("�� ġ �� ȣ"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_��ġ��ȣ_�ڵ�"), _T("!��ġ��ȣ.�ڵ�,!��ġ��ȣ.����"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_��������_�ڵ�"), _T("!��������.�ڵ�,!��������.����"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strAppendixGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_���ʹ谡����"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_������ȣ"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_��ϱ���_�ڵ�"), _T("!��ϱ���.�ڵ�,!��ϱ���.����"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleReadOnly, _T("SB_��Ϲ�ȣ"));

		// �η� Color ����
		m_grid.SetGroupColor(m_strAppendixGroupName, RGB(220, 238, 208));
	}
	else if (nFlag==3)		// å
	{
		m_grid.AddItem(_T("�� �� �� ��"), m_strHoldBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_���Ա���_�ڵ�"), _T("!���Ա���.�ڵ�,!���Ա���.����"));
		m_grid.AddItem(_T("��  ��  ��"), m_strHoldBookGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_�Լ���"));
		m_grid.AddItem(_T("å  ��  ��"), m_strHoldBookGroupName, CMPropertyItem::typeComboBox, CMPropertyItem::styleReadOnly, _T("SB_�ڷ����"));
		m_grid.AddItem(_T("��  ��  ��"), m_strHoldBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleReadOnly, _T("SB_�谡��ġ��ȣ"), _T("!�ڷ�Ǳ���.�ڵ�,!�ڷ�Ǳ���.����"));
		m_grid.AddItem(_T("��      ��"), m_strHoldBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_���"));
		m_grid.AddItem(_T("��/������ȣ"), m_strHoldBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_��å��ȣ"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strHoldBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_�з���ȣ"));
		m_grid.AddItem(_T("�� ġ �� ȣ"), m_strHoldBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_��ġ��ȣ_�ڵ�"), _T("!��ġ��ȣ.�ڵ�,!��ġ��ȣ.����"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strHoldBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_��������_�ڵ�"), _T("!��������.�ڵ�,!��������.����"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strHoldBookGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_���ʹ谡����"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strHoldBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_������ȣ"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strHoldBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleReadOnly, _T("SB_��Ϲ�ȣ"));

		// å Color ����
		m_grid.SetGroupColor(m_strHoldBookGroupName, RGB(220, 238, 208));
	}
	else if (nFlag==4)	// �η�
	{
		m_grid.AddItem(_T("��  ��  ��"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SA_�ηϸ�"));
		m_grid.AddItem(_T("�η����ı���"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SA_���ı���"), _T("!���ı���.�ڵ�,!���ı���.����"));
		m_grid.AddItem(_T("�ηϸ�ü����"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SA_��ü����"), _T("!��ü����.�ڵ�,!��ü����.����"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_���Ա���_�ڵ�"), _T("!���Ա���.�ڵ�,!���Ա���.����"));
		m_grid.AddItem(_T("��  ��  ��"), m_strHoldAppendixGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_�Լ���"));
		m_grid.AddItem(_T("å  ��  ��"), m_strHoldAppendixGroupName, CMPropertyItem::typeComboBox, CMPropertyItem::styleReadOnly, _T("SB_�ڷ����"));
		m_grid.AddItem(_T("��  ��  ��"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleReadOnly, _T("SB_�谡��ġ��ȣ"), _T("!�ڷ�Ǳ���.�ڵ�,!�ڷ�Ǳ���.����"));
		m_grid.AddItem(_T("��      ��"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_���"));
		m_grid.AddItem(_T("��/������ȣ"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_��å��ȣ"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_�з���ȣ"));
		m_grid.AddItem(_T("�� ġ �� ȣ"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_��ġ��ȣ_�ڵ�"), _T("!��ġ��ȣ.�ڵ�,!��ġ��ȣ.����"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_��������_�ڵ�"), _T("!��������.�ڵ�,!��������.����"));
		m_grid.AddItem(_T("�� �� �� ��"), m_strHoldAppendixGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_���ʹ谡����"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_������ȣ"));
		m_grid.AddItem(_T("�� �� �� ȣ"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleReadOnly, _T("SB_��Ϲ�ȣ"));

		// �η� Color ����
		m_grid.SetGroupColor(m_strHoldAppendixGroupName, RGB(220, 238, 208));
	}

	// �׷캰 DM Pointer ����
	m_grid.SetGroupDMPtr(m_strVolumeGroupName, m_pDM_Volume);
	m_grid.SetGroupDMPtr(m_strBookGroupName, m_pDM_Book);
	m_grid.SetGroupDMPtr(m_strHoldBookGroupName, m_pDM_HoldBook);
	m_grid.SetGroupDMPtr(m_strAppendixGroupName, m_pDM_Appendix);
	m_grid.SetGroupDMPtr(m_strHoldAppendixGroupName, m_pDM_HoldAppendix);

	return 0;

	EFS_END
	return -1;
}

// ������ �׸��� ���� �ѷ��ֱ� (Tab Change, Sell Select)
INT CSEVolumeDlg::Display()
{
EFS_BEGIN

	INT ids;
	
	INT nCurSel = m_TabCtrl.GetCurSel();
	if (nCurSel < 0) return -1;

	CString strGroup, strGrid;
	CESL_Grid *pGrid = NULL;
	CESL_DataMgr *pDM;
	
	if (nCurSel == 0)
	{
		strGroup = m_strVolumeGroupName;
		pGrid = m_pGrid_Volume;
		pDM = m_pDM_Volume;
		strGrid = m_strVolumeGrid;

		ids = InitGridData(0);
	}
	else if (nCurSel == 1)
	{
		strGroup = m_strBookGroupName;
		pGrid = m_pGrid_Book;
		pDM = m_pDM_Book;
		strGrid = m_strBookGrid;

		ids = InitGridData(0);
	}
	else if (nCurSel == 2)
	{
		strGroup = m_strAppendixGroupName;
		pGrid = m_pGrid_Appendix;
		pDM = m_pDM_Appendix;
		strGrid = m_strAppendixGrid;

		ids = InitGridData(1);
	}
	else if (nCurSel == 3)
	{
		strGroup = m_strHoldBookGroupName;
		pGrid = m_pGrid_HoldBook;
		pDM = m_pDM_HoldBook;
		strGrid = m_strHoldBookGrid;

		ids = InitGridData(3);
	}
	else if (nCurSel == 4)
	{
		strGroup = m_strHoldAppendixGroupName;
		pGrid = m_pGrid_HoldAppendix;
		pDM = m_pDM_HoldAppendix;
		strGrid = m_strHoldAppendixGrid;

		ids = InitGridData(4);
	}
	else
		return -1;

	INT nRow = pGrid->GetRow() - 1;
	if (nRow < 0) return -1;

	// 040322_HSS ���õ� ���� �Է� ++++++++++++++
	m_pDM_Selected = pDM;
	m_pGrid_Selected = pGrid;
	m_strSelectedGroupName = strGroup;
	m_strSelectedGrid = strGrid;
	// ++++++++++++++++++++++++++++++++++++++++++

	m_grid.Display(nRow, strGroup);	

	// 040323_HSS DB_Flag�� D�� ��� Height=0 ++++++++++++++++++
	INT nRows = m_pDM_Selected->GetRowCount();
	for (INT idx = 0; idx < nRows; idx++)
	{
		if (m_strSelectedGrid == m_strVolumeGrid)	// ���� ���
		{
			if (m_pDM_Selected->GetCellData(_T("SV_DB_FLAG"), idx) == _T("D"))
			{
				m_pGrid_Selected->SetRowHeight(idx + 1, 0);
			}
		}
		else	// å,����å,�η�,����η��� ���
		{
			if (m_pDM_Selected->GetCellData(_T("SB_DB_FLAG"), idx) == _T("D"))
			{
				m_pGrid_Selected->SetRowHeight(idx + 1, 0);
			}
		}
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	return 0;

EFS_END
	return -1;
}

// Tap Control ����� �����ֱ� ����
INT CSEVolumeDlg::DisplayGrids()
{
	EFS_BEGIN

	INT nCurSel = m_TabCtrl.GetCurSel();
	if (nCurSel < 0) return -1;

	CRect rcClient;
	m_GroupBoxCtrl.GetWindowRect(rcClient);
	ScreenToClient(rcClient);

	if (nCurSel == 0)
	{
		m_pGrid_Volume->MoveWindow(rcClient);

		m_pGrid_Book->ShowWindow(SW_HIDE);
		m_pGrid_Volume->ShowWindow(SW_SHOW);
		m_pGrid_HoldBook->ShowWindow(SW_HIDE);
		m_pGrid_Appendix->ShowWindow(SW_HIDE);
		m_pGrid_HoldAppendix->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 1)
	{
		m_pGrid_Book->MoveWindow(rcClient);

		m_pGrid_Book->ShowWindow(SW_SHOW);
		m_pGrid_Volume->ShowWindow(SW_HIDE);
		m_pGrid_HoldBook->ShowWindow(SW_HIDE);
		m_pGrid_Appendix->ShowWindow(SW_HIDE);
		m_pGrid_HoldAppendix->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 3)
	{
		m_pGrid_HoldBook->MoveWindow(rcClient);

		m_pGrid_Book->ShowWindow(SW_HIDE);
		m_pGrid_Volume->ShowWindow(SW_HIDE);
		m_pGrid_HoldBook->ShowWindow(SW_SHOW);
		m_pGrid_Appendix->ShowWindow(SW_HIDE);
		m_pGrid_HoldAppendix->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 2)
	{
		m_pGrid_Appendix->MoveWindow(rcClient);

		m_pGrid_Book->ShowWindow(SW_HIDE);
		m_pGrid_Volume->ShowWindow(SW_HIDE);
		m_pGrid_HoldBook->ShowWindow(SW_HIDE);
		m_pGrid_Appendix->ShowWindow(SW_SHOW);
		m_pGrid_HoldAppendix->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 4)
	{
		m_pGrid_HoldAppendix->MoveWindow(rcClient);

		m_pGrid_Book->ShowWindow(SW_HIDE);
		m_pGrid_Volume->ShowWindow(SW_HIDE);
		m_pGrid_HoldBook->ShowWindow(SW_HIDE);
		m_pGrid_Appendix->ShowWindow(SW_HIDE);
		m_pGrid_HoldAppendix->ShowWindow(SW_SHOW);
	}

	return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CSEVolumeDlg message handlers

BOOL CSEVolumeDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	if (InitESL_Mgr(_T("SM_����_��ȣ����")) < 0)
	{
		m_strErrMsg = _T("Init ESL Mgr Error");
		goto ERR;
	}

	if (!m_grid.SubclassDlgItem(IDC_GRID_VALUES, this))
	{
		m_strErrMsg = _T("�׸��� ����Ŭ���� ����");
		goto ERR;
	}

	if (Init() < 0)
	{
		m_strErrMsg = _T("���̾�α� �ʱ�ȭ ����");
		goto ERR;
	}

	PostMessage(WM_SIZE);

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strErrMsg.IsEmpty())
		AfxMessageBox(m_strErrMsg);

	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CSEVolumeDlg::OnSelchangeTabSvf(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	DisplayGrids();
	Display();

	*pResult = 0;

	EFS_END
}

VOID CSEVolumeDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (!GetSafeHwnd()) return;

	if (m_pGrid_Book == NULL || m_pGrid_Volume == NULL) return;

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcGrid;
	m_grid.GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	INT nWidth = rcGrid.Width();

	rcGrid.right = rcClient.right - 5;
	rcGrid.left = rcGrid.right - nWidth;
	rcGrid.bottom = rcClient.bottom;
	m_grid.MoveWindow(rcGrid);
	
	rcClient.top = rcGrid.top + 38;
	rcClient.right = rcGrid.left - 15;

	GetDlgItem(IDC_SIZE_GRIPPER)->SetWindowPos(this, rcClient.right, rcGrid.top, 7, rcGrid.Height(), SWP_NOZORDER);

	rcClient.right -= 5;
	m_TabCtrl.MoveWindow(rcClient);
		
	CRect rcTab;
	m_TabCtrl.GetItemRect(0, rcTab);

	rcClient.top += rcTab.Height();

	rcClient.DeflateRect(10, 10);
	m_GroupBoxCtrl.MoveWindow(rcClient);

	DisplayGrids();

	EFS_END
}

VOID CSEVolumeDlg::OnbCONTENTMGR() 
{
	INT ids;
	ids = m_pDM_Volume->GetRowCount();
	if (ids<=0)
	{
		AfxMessageBox(_T("�������� �����ϴ�."));
		return;
	}
	
	CCONTENTMGR_SE_EDIT dlg(this);
	dlg.Set_Mgr(_T(""), m_pDM_Volume->DB_MGR_ALIAS, _T(""), _T("SV_��KEY"), m_pGrid_Volume->GetRow() - 1);
	dlg.DoModal() ;
}

BEGIN_EVENTSINK_MAP(CSEVolumeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSEVolumeDlg)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_VOLUME_SVF, -607 /* MouseUp */, OnMouseUpGridVolumeSvf, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_BOOK_SVF, -607 /* MouseUp */, OnMouseUpGridBookSvf, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_HOLD_BOOK_SVF, -605 /* MouseDown */, OnMouseDownGridHoldBookSvf, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_APPENDIX_SVF, -607 /* MouseUp */, OnMouseUpGridAppendixSvf, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_HOLD_APPENDIX_SVF, -605 /* MouseDown */, OnMouseDownGridHoldAppendixSvf, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_BOOK_SVF, -604 /* KeyUp */, OnKeyUpGridBookSvf, VTS_PI2 VTS_I2)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_HOLD_BOOK_SVF, -604 /* KeyUp */, OnKeyUpGridHoldBookSvf, VTS_PI2 VTS_I2)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_HOLD_APPENDIX_SVF, -604 /* KeyUp */, OnKeyUpGridHoldAppendixSvf, VTS_PI2 VTS_I2)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_VOLUME_SVF, -604 /* KeyUp */, OnKeyUpGridVolumeSvf, VTS_PI2 VTS_I2)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_APPENDIX_SVF, -604 /* KeyUp */, OnKeyUpGridAppendixSvf, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSEVolumeDlg::OnMouseUpGridVolumeSvf(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnMouseUpGridBookSvf(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnMouseDownGridHoldBookSvf(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnMouseUpGridAppendixSvf(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnMouseDownGridHoldAppendixSvf(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

// ������ ������ ���� - �������� å������ ��ũ�� ���߾��ش�.
// ??? - ��� ������ ������ �ش� DM�� ����Ǵ��� �𸣰��� - ??
VOID CSEVolumeDlg::OnMPropertyGridUpdate(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	INT nCurSel = m_TabCtrl.GetCurSel();
	if (nCurSel < 0) return;

	// 040322_HSS å/�η������� �������� Sync
	// 1) �������� å/�η��� UDF������ Copy
	// 2) �������� å/�η� �� �ٽ� ���߱�(����/�Է� ������ ��������)
	m_pCatApi->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);
	//static_cast<CCatESL_Mgr_SE*>(pParentMgr)->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);

	// 040529_HSS �׸��� ����
	if (nCurSel == 0)			// ��
	{
		// ���϶� ���������� ǥ��
		INT nIdx = wParam;
		if (m_pDM_Volume->GetCellData(_T("SV_DB_FLAG"), nIdx) != _T("I"))
		{
			m_pDM_Volume->SetCellData(_T("SV_DB_FLAG"), m_pCatApi->GetQueryFlag(MNG_MODIFY, m_nAppMode, m_pDM_Volume->GetCellData(_T("SV_DB_FLAG"), nIdx)), nIdx);
			//m_pDM_Volume->SetCellData(_T("SV_DB_FLAG"), GetQueryFlag(MNG_UPDATE, m_nAppMode, m_pDM_Volume->GetCellData(_T("SV_DB_FLAG"), nIdx)), nIdx);
		}

		m_pGrid_Volume->Display();
		m_pGrid_Book->Display();
		m_pGrid_Appendix->Display();
		m_pGrid_Volume->SetRow(wParam+1);
	}
	else if (nCurSel == 1)
	{
		m_pGrid_Book->Display();
		m_pGrid_Book->SetRow(wParam+1);
	}
	else if (nCurSel == 2)
	{
		m_pGrid_Appendix->Display();
		m_pGrid_Appendix->SetRow(wParam+1);
	}
	else if (nCurSel == 3)
	{
		// ����å ����
		INT nIdx = wParam;
		if (m_pDM_HoldBook->GetCellData(_T("SB_DB_FLAG"), nIdx) == _T(""))
		{
			m_pDM_HoldBook->SetCellData(_T("SB_DB_FLAG"), m_pCatApi->GetQueryFlag(MNG_MODIFY, m_nAppMode, m_pDM_HoldBook->GetCellData(_T("SB_DB_FLAG"), nIdx)), nIdx);
			//m_pDM_Volume->SetCellData(_T("SV_DB_FLAG"), GetQueryFlag(MNG_UPDATE, m_nAppMode, m_pDM_Volume->GetCellData(_T("SV_DB_FLAG"), nIdx)), nIdx);
		}

		m_pGrid_HoldBook->Display();
		m_pGrid_HoldBook->SetRow(wParam+1);
	}
	else if (nCurSel == 4)
	{
		// ����å ����
		INT nIdx = wParam;
		if (m_pDM_HoldAppendix->GetCellData(_T("SB_DB_FLAG"), nIdx) == _T(""))
		{
			m_pDM_HoldAppendix->SetCellData(_T("SB_DB_FLAG"), m_pCatApi->GetQueryFlag(MNG_MODIFY, m_nAppMode, m_pDM_HoldAppendix->GetCellData(_T("SB_DB_FLAG"), nIdx)), nIdx);
		}

		m_pGrid_HoldAppendix->Display();
		m_pGrid_HoldAppendix->SetRow(wParam+1);
	}

	Display();

	EFS_END
}

// ���α׸��� �ѷ��� �� ����
// �׸��� ���� ��ȯ �Լ� - Descript���� �ٲپ� �ش�.
INT CSEVolumeDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	EFS_BEGIN

	strGridAlias.TrimLeft();		strGridAlias.TrimRight();
	
	if (strDMFieldAlias.Find(_T("�ڷ����")) >= 0)
		str = GetWorkingStatusDesc(str);
	

	// 040323_HSS ����å����, ��ȣ���� �߰�
	else if (strDMFieldAlias.Find(_T("SB_å����")) >= 0)
	{
		if (str == _T("B")) str.Format(_T("����å"));
		else if (str == _T("C")) str.Format(_T("������å"));
	}
	else if (strDMFieldAlias.Find(_T("SV_��ȣ����")) >= 0)
	{
		if (str == _T("V")) str.Format(_T("�Ϲݱ�ȣ"));
		else if (str == _T("B")) str.Format(_T("������ȣ"));
	}

	return 0;

	EFS_END
	return -1;
}

// ������ ���� - ��Ͽ��� ���� �� �ڷ������ ���� Description���� ����
UINT CSEVolumeDlg::OnMPropertyGridGetData(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	CString strAlias;
	strAlias.Format(_T("%s"), reinterpret_cast<CString*>(wParam)->GetBuffer(0));

	if (strAlias == _T("SB_�ڷ����"))
	{
		CString strWorkingStatusCode;
		strWorkingStatusCode.Format(_T("%s"), reinterpret_cast<CString*>(lParam)->GetBuffer(0));
		strWorkingStatusCode = GetWorkingStatusDesc(strWorkingStatusCode);

		reinterpret_cast<CString*>(lParam)->Format(_T("%s"), strWorkingStatusCode);

		return 0;
	}
	else return -1;

	EFS_END
	return -1;
}

VOID CSEVolumeDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	CWnd *pWnd = GetDlgItem(IDC_SIZE_GRIPPER);
	CRect rcWnd;
	pWnd->GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);
	if (rcWnd.PtInRect(point))
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		SetCapture();
		m_bGripperSizing = TRUE;
	}
	
	CDialog::OnLButtonDown(nFlags, point);

	EFS_END
}


VOID CSEVolumeDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	if (m_bGripperSizing)
	{
		m_bGripperSizing = FALSE;

		// �׸��۸� ������� �׸������ �ű��.
		CWnd *pWnd = GetDlgItem(IDC_SIZE_GRIPPER);
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		ScreenToClient(rcWnd);

		CRect rcTabCtrl;
		m_TabCtrl.GetWindowRect(rcTabCtrl);
		ScreenToClient(rcTabCtrl);
		rcTabCtrl.right = rcWnd.left - 5;

		m_TabCtrl.MoveWindow(rcTabCtrl);
		
		CRect rcTabItem;
		m_TabCtrl.GetItemRect(0, rcTabItem);

		rcTabCtrl.top += rcTabItem.Height();

		rcTabCtrl.DeflateRect(10, 10);
		m_GroupBoxCtrl.MoveWindow(rcTabCtrl);

		DisplayGrids();

		CRect rcGrid;
		m_grid.GetWindowRect(rcGrid);
		ScreenToClient(rcGrid);
		rcGrid.left = rcWnd.right + 5;
		m_grid.MoveWindow(rcGrid);

	}

	ReleaseCapture();
	
	CDialog::OnLButtonUp(nFlags, point);

	EFS_END
}

VOID CSEVolumeDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	if (m_bGripperSizing)
	{
		CRect rcClinet;
		GetClientRect(rcClinet);
		if (!rcClinet.PtInRect(point)) return;

		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));

		CWnd *pWnd = GetDlgItem(IDC_SIZE_GRIPPER);
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		ScreenToClient(rcWnd);

		rcWnd.left = point.x;

		pWnd->SetWindowPos(this, rcWnd.left, rcWnd.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
	else
	{
		CWnd *pWnd = GetDlgItem(IDC_SIZE_GRIPPER);
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		ScreenToClient(rcWnd);
		if (rcWnd.PtInRect(point))
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		}
	}
		
	CDialog::OnMouseMove(nFlags, point);

	EFS_END
}


// 040322_HSS ����, �Է� �߰� ++++++++++++++++++++++++++++
VOID CSEVolumeDlg::OnbDELETE() 
{
EFS_BEGIN
	CString strMsg;
	INT idx, ids;

	m_pGrid_Selected->SelectMakeList();

	idx = m_pGrid_Selected->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
	    return;
	}

	ids = m_pGrid_Selected->SelectGetCount();
	strMsg.Format(_T("�����Ͻ� [ %d ]���� %s�� �����Ͻðڽ��ϱ�?\n����ȭ�鿡�� ����ÿ� DB���� ������ �����˴ϴ�.")
		, ids, m_strSelectedGroupName);
	if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return;

	CSeApi api(this);

	// �׸��带 ���鼭 SB_DB_FLAG�� _T("D")�� �ٲ۴�.
	// ������������ Ȯ���ϰ�, ���� ��� å,�ηϱ��� �ٲپ�� ��
	while ( idx != -1 ) {
		if (m_strSelectedGrid == m_strVolumeGrid)	// ���� ���
		{
			if (m_pDM_Selected->GetCellData(_T("SV_DB_FLAG"),idx) == _T("I"))
			{
				//m_pDM_Selected->SetCellData(_T("SV_DB_FLAG"), _T("D"), idx);
				m_pDM_Selected->DeleteRow(idx);
				idx = SelectGetNext(_T("CM_����_��ȣ����"), m_strSelectedGrid);
				continue;
			}

			CString strSPECIES_KEY, strVOL_KEY, strVOL_TYPE, strVolTitle;
			CStringArray arrayChildBookKeys;

			strSPECIES_KEY = m_pDM_Selected->GetCellData(_T("SV_��KEY"),idx);
			strVOL_KEY = m_pDM_Selected->GetCellData(_T("SV_��KEY"),idx);
			strVOL_TYPE = m_pDM_Selected->GetCellData(_T("SV_��ȣ����"),idx);
			strVolTitle = m_pDM_Selected->GetCellData(_T("SV_��ȣ��"),idx);

			// ǥ������ 041130_�����߰�
			if ( !strVOL_KEY.IsEmpty() )
			{
				CCOVER_MAIN dlg(this);
				dlg.DeleteCoverInfo ( strVOL_KEY.GetBuffer(0) ) ;
			
			}

			ids = api.VOL_CAT_DELETE(m_pDM_Selected,
					   strSPECIES_KEY,
					   strVOL_KEY,
					   strVOL_TYPE,
					   strVolTitle,
					   arrayChildBookKeys,
					   TRUE, TRUE);
			if (ids < 0)
			{
				if(-1002 == ids) // verifying fail
				{
					//strMsg.Format(_T("[ %s ] �ڷḦ ������ �� �����ϴ�!"), strVolTitle);
					//AfxMessageBox(strMsg);

					idx = SelectGetNext(_T("CM_����_��ȣ����"), m_strSelectedGrid);
					continue;
				}
				else 
				{
					strMsg.Format(_T("[ %s ] �ڷḦ �����ϴ� �� ������ �߻��� �۾��� �ߴ��մϴ�!"), strVolTitle);
					AfxMessageBox(strMsg);
					return;
				}
			}
			else m_pDM_Selected->SetCellData(_T("SV_DB_FLAG"), _T("D"), idx);
		}
		else	// å,����å,�η�,����η��� ���
		{
			if (m_pDM_Selected->GetCellData(_T("SB_DB_FLAG"),idx) == _T("I"))
			{
				//m_pDM_Selected->SetCellData(_T("SB_DB_FLAG"), _T("D"), idx);
				m_pDM_Selected->DeleteRow(idx);
				idx = SelectGetNext(_T("CM_����_��ȣ����"), m_strSelectedGrid);
				continue;
			}

			CString strBOOK_KEY, strACQ_TYPE_VOL_KEY, strVOL_KEY;
			CString strSPECIES_KEY, strBOOK_TYPE, strBooktitle;
			
			strBOOK_KEY = m_pDM_Selected->GetCellData(_T("SB_åKEY"),idx);
			strACQ_TYPE_VOL_KEY = m_pDM_Selected->GetCellData(_T("SB_���Ա��к���KEY"),idx);
			strVOL_KEY = m_pDM_Selected->GetCellData(_T("SB_��KEY"),idx);
			strSPECIES_KEY = m_pDM_Selected->GetCellData(_T("SB_��KEY"),idx);
			strBOOK_TYPE = m_pDM_Selected->GetCellData(_T("SB_å����"),idx);

			if (m_pDM_Selected == m_pDM_Book || m_pDM_Selected == m_pDM_HoldBook )
				strBooktitle = m_pDM_Selected->GetCellData(_T("SV_��ȣ��"),idx);
			if (m_pDM_Selected == m_pDM_Appendix || m_pDM_Selected == m_pDM_HoldAppendix )
				strBooktitle = m_pDM_Selected->GetCellData(_T("SA_�ηϸ�"),idx);

			
			// ������ å�� ���(041206_�����߰�)
			BOOL fIsLastBook = IsLastBook(idx);
			if( fIsLastBook )
			{
				strMsg.Format(_T("%s ���� ������ å�Դϴ�. ������ ��� ��ȣ������ �Բ� �����˴ϴ�.\n���� �����Ͻðڽ��ϱ�?")
					, strBooktitle);
				if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) 
				{
					idx = SelectGetNext(_T("CM_����_��ȣ����"), m_strSelectedGrid);
					continue;
				}
			}
	
			ids = api.BOOK_CAT_DELETE(m_pDM_Selected,
							strBOOK_KEY,strACQ_TYPE_VOL_KEY,strVOL_KEY,
							strSPECIES_KEY,strBOOK_TYPE,strBooktitle,TRUE, FALSE);
			if (ids < 0)
			{
				if(-1002 == ids) // verifying fail
				{
					//strMsg.Format(_T("[ %s ] �ڷḦ ������ �� �����ϴ�!"), strBooktitle);
					//AfxMessageBox(strMsg);

					idx = SelectGetNext(_T("CM_����_��ȣ����"), m_strSelectedGrid);
					continue;
				}
				else 
				{
					strMsg.Format(_T("[ %s ] �ڷḦ �����ϴ� �� ������ �߻��� �۾��� �ߴ��մϴ�!"), strBooktitle);
					AfxMessageBox(strMsg);
					return;
				}
			}
			else {
				m_pDM_Selected->SetCellData(_T("SB_DB_FLAG"), _T("D"), idx);
				// ������ å�̸� �ǿ��� _T("D")
				if (fIsLastBook) SetVolumeDBFlag(strVOL_KEY, _T("D"));
			}
		}
		idx = SelectGetNext(_T("CM_����_��ȣ����"), m_strSelectedGrid);
	}

	// �������� Sync
	m_pCatApi->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);

	m_pGrid_Volume->Display();
	m_pGrid_Book->Display();
	m_pGrid_Appendix->Display();
	DisplayGrids();
	Display();

	return;
EFS_END
}

VOID CSEVolumeDlg::OnbINPUT_VOLUME() 
{
	if (m_strSelectedGroupName != m_strVolumeGroupName)
	{
		m_TabCtrl.SetCurSel(0);
		DisplayGrids();
		Display();
	}
	CSE_CAT_INPUT_VOL dlg(this);
	dlg.SetCatApi(m_pCatApi);
	if(dlg.DoModal() == IDOK)
	{
		m_pCatApi->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);

		m_pGrid_Volume->Display();
		m_pGrid_Book->Display();
		m_pGrid_HoldBook->Display();
		DisplayGrids();
		Display();
	}

	return;
}

VOID CSEVolumeDlg::OnbINPUT_BOOK() 
{
	// ��ȣ������ ����
	if (m_strSelectedGroupName != m_strVolumeGroupName)
	{
		AfxMessageBox(_T("��ȣ�������� å�� �߰��� ��ȣ�� �����Ͽ� �ֽʽÿ�"));
		m_TabCtrl.SetCurSel(0);
		DisplayGrids();
		Display();
		return;
	}
	
// ��ȣ���� �ִ��� Ȯ��
	INT nVolCnt = m_pDM_Volume->GetRowCount();
	if ( nVolCnt <= 0 )
	{
		AfxMessageBox(_T("å�� �߰��� ��ȣ������ ���� �Է��Ͽ� �ֽʽÿ�."));
		m_TabCtrl.SetCurSel(0);
		DisplayGrids();
		Display();
		return;
	}

	INT nIdx = -1;
	INT nSelectedCnt = 0;
	CString sCheck;
	//2018�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
	//KOL.RED.019-006 JY ���Ӱ��๰ ��� ����ȭ
	//BEFORE=======================================
	//for( INT i = 0 ; i< nVolCnt+1; i++ )
	//AFTER========================================
	for( INT i = 0 ; i< nVolCnt; i++ )
	{
		m_pGrid_Selected->GetAt(i,0,sCheck);
		if( sCheck == _T("V") )
		{
			nIdx = i;
			nSelectedCnt += 1;
			//2018�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
			//KOL.RED.018-006 JY ���Ӱ��๰ ��� ����ȭ
			//ADD===================================
			sCheck = _T("");
			//======================================
		}
	}

	if( nSelectedCnt <= 0 )
	{
		AfxMessageBox(_T("å�� �߰��� ��ȣ�� �������ֽʽÿ�"));
		return;
	}
	else if( nSelectedCnt > 1 )
	{
		AfxMessageBox(_T("å�� �߰��� ��ȣ�� �Ѱ��� �������ֽʽÿ�"));
		return;
	}

	CString sBookCnt;
	m_pDM_Volume->GetCellData(_T("SV_å��"),nIdx,sBookCnt);
	INT nCnt = _ttoi(sBookCnt);
	
	if( nCnt < 1 )//���� ��ȣ�� �����Ѵ�..
	{
		AfxMessageBox(_T("�����ȣ�� å�� �߰��� �� �����ϴ�"));
		return;
	}
	
	CSE_CAT_INPUT_BOOK dlg(this);
	dlg.InitMemberData(m_pCatApi, nIdx);
	if(dlg.DoModal() == IDOK)
	{
		m_pCatApi->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);

		m_TabCtrl.SetCurSel(1);
		m_pGrid_Volume->Display();
		m_pGrid_Book->Display();
		m_pGrid_HoldBook->Display();
		DisplayGrids();
		Display();
	}

	return;
}

VOID CSEVolumeDlg::OnbINPUT_APPENDIX() 
{
	// ��ȣ������ ����
	if (m_strSelectedGroupName == m_strVolumeGroupName ) //|| m_strHoldBookGroupName != m_strSelectedGroupName)
	{
		// ��ȣ���� �ִ��� Ȯ��
		INT nVolCnt = m_pDM_Volume->GetRowCount();
		if ( nVolCnt <= 0 )
		{
			AfxMessageBox(_T("�η��� �߰��� ��ȣ������ ���� �Է��Ͽ� �ֽʽÿ�."));
			m_TabCtrl.SetCurSel(0);
			DisplayGrids();
			Display();
			return;
		}

		INT nIdx = -1;
		INT nSelectedCnt = 0;
		CString sCheck;
		// 17/11/27 ������ : ����>����>��Ͽϼ����� �η��Է½� �Ѱ��̻� �ڷᰡ ���õǾ��ٴ� ���� �߻�
/*//BEFORE-----------------------------------------------------------------------------
		for( INT i = 0 ; i< nVolCnt+1; i++ )
*///AFTER------------------------------------------------------------------------------
		for( INT i = 0 ; i< nVolCnt; i++ )	
		{
			sCheck = _T("");
/*//END-------------------------------------------------------------------------------*/		
			m_pGrid_Selected->GetAt(i,0,sCheck);
			if( sCheck == _T("V") )
			{
				nIdx = i;
				nSelectedCnt += 1;
			}
		}

		if( nSelectedCnt <= 0 )
		{
			AfxMessageBox(_T("�η��� �߰��� ��ȣ�� �������ֽʽÿ�"));
			return;
		}
		else if( nSelectedCnt > 1 )
		{
			AfxMessageBox(_T("�η��� �߰��� ��ȣ�� �Ѱ��� �������ֽʽÿ�"));
			return;
		}
		
		CString sHoldCnt;
		m_pDM_Volume->GetCellData(_T("SV_����å��"),nIdx,sHoldCnt);
		INT nCnt = _ttoi(sHoldCnt);
		
		CSE_CAT_INPUT_APPENDIX dlg(this);

		if( nCnt > 0 )//���� ��ȣ�� �����Ѵ�..
		{
			dlg.InitMemberData(m_pCatApi, nIdx, 1);	
		}
		else
		{
			dlg.InitMemberData(m_pCatApi, nIdx, 0);
		}
					
		if(dlg.DoModal() == IDOK)
		{
			m_pCatApi->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);

			m_TabCtrl.SetCurSel(2);
			m_pGrid_Volume->Display();
			m_pGrid_Appendix->Display();
			m_pGrid_HoldAppendix->Display();
			DisplayGrids();
			Display();
		}
	}
	else
	{
		AfxMessageBox(_T("��ȣ�������� å�� �߰��� ��ȣ�� �����Ͽ� �ֽʽÿ�"));
		m_TabCtrl.SetCurSel(0);
		DisplayGrids();
		Display();
		return;
	}
	return;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

CString CSEVolumeDlg::GetWorkingStatusDesc(CString strWorkingStatusCode)
{
	EFS_BEGIN

	CString strDesc = _T("") ;

	FindWorkingStatusCodeDesc( strWorkingStatusCode, strDesc );
	
	if ( strDesc != _T("") )  return strDesc;
	else return _T("");


	EFS_END
	return _T("");
}

VOID CSEVolumeDlg::FindWorkingStatusCodeDesc(CString strKey, CString &strValue )
{
	// å ���� ����, �ڵ�
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapWorkingStatusCodeDesc;
	mapWorkingStatusCodeDesc.RemoveAll();
	mapWorkingStatusCodeDesc.SetAt(_T("SEA111N"),_T("�����Լ��ڷ�(�ΰ����ڷ�)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEA112N"),_T("���԰�ȣ�ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEA211N"),_T("�����Լ��ڷ�(�ΰ����ڷ�)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEA212N"),_T("������ȣ�ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEB111N"),_T("�����ΰ��ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEB112N"),_T("���������ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEB113N"),_T("���������ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEB114N"),_T("�����˼��ڷ�(����ΰ����ڷ�)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL111N"),_T("�ǽð��ΰ��ڷ�(�ǽð��谡����ڷ�)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL112N"),_T("�ǽð��谡�ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL113O"),_T("�ǽð�����ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SER111N"),_T("����ΰ��ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SER112N"),_T("����ڷ�(�����ΰ����ڷ�)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SER113O"),_T("�����ι�ȣ�ο��ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEC111N"),_T("��������ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEC112O"),_T("�����ΰ����ڷ�(�����Ϸ��ڷ�)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL211O"),_T("�����ΰ��ڷ�(�谡����ڷ�)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL212N"),_T("�谡�ڷ�(�����ڷ�)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL213O"),_T("���������ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL214O"),_T("�ļ��ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL215O"),_T("�����ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL216O"),_T("�н��ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL217O"),_T("�������ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL311O"),_T("���ܴ����ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL312O"),_T("���������ڷ�"));
	//=====================================================
	//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
	// 18.09.27 JMJ KOL.RED.2018.008
	mapWorkingStatusCodeDesc.SetAt(_T("SEL411O"),_T("����å�η����������ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL511O"),_T("���յ���Ÿ���ݳ�"));
	//=====================================================
	mapWorkingStatusCodeDesc.SetAt(_T("SEL313O"),_T("Ư�������ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL314O"),_T("��޴����ڷ�"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL315O"),_T("����ڷ�"));

	mapWorkingStatusCodeDesc.Lookup(strKey, strValue);
}

// �ش� ���� ������ å���� Ȯ��( ������ å�̸� return TRUE)
BOOL CSEVolumeDlg::IsLastBook(INT nIdx)
{
EFS_BEGIN
	if ( nIdx < 0 ) return FALSE;

	// �η��� ��� check ����
	if (m_strSelectedGrid == m_strAppendixGrid  || m_strSelectedGrid == m_strHoldAppendixGrid)	return FALSE;

	CString sVolKey = m_pDM_Selected->GetCellData(_T("SV_��KEY"),nIdx);
	if ( sVolKey == _T("") ) return FALSE;

	CString sTmpVolKey=_T("");
	INT nCountBook = 0;

	// å DM���� �ش� �ǿ� ���� å �� Ȯ��
	for ( INT i=0; i<m_pDM_Book->GetRowCount(); i++)
	{
		if ( m_pDM_Book->GetCellData(_T("SB_DB_FLAG"),i) != _T("D") ) // ������ å�� �ƴϸ�
		{
			sTmpVolKey = m_pDM_Book->GetCellData(_T("SV_��KEY"), i);
			
			if ( sTmpVolKey == sVolKey )  nCountBook++;
		}
	}

	// ���� DM���� å �ش� �ǿ� ���� å ��  Ȯ��
	for ( i=0; i<m_pDM_HoldBook->GetRowCount(); i++)
	{
		if ( m_pDM_HoldBook->GetCellData(_T("SB_DB_FLAG"),i) != _T("D") ) 
		{
			sTmpVolKey = m_pDM_HoldBook->GetCellData(_T("SB_��KEY"), i);//SB_��KEY

			if ( sTmpVolKey == sVolKey )  nCountBook++;
		}
	}

	if ( nCountBook == 1 ) return TRUE;

	return FALSE;
EFS_END
	return FALSE;

}


INT CSEVolumeDlg::SetVolumeDBFlag(CString sVolKey, CString sFlag)
{
EFS_BEGIN
	
	if ( sVolKey == _T("") ) return -1;
	
	CString sTmpVolKey = _T("");
	for ( INT i=0; i<m_pDM_Volume->GetRowCount(); i++)
	{
		sTmpVolKey = m_pDM_Volume->GetCellData(_T("SV_��KEY"), i);
		if ( sTmpVolKey == sVolKey ) 
		{
			m_pDM_Volume->SetCellData(_T("SV_DB_FLAG"), sFlag, i);
			return 0;
		}
	}

	return 0;

EFS_END
	return -1;
}
// ǥ������ (���� �߰� 2004.11.29)
VOID CSEVolumeDlg::OnbCOVER() 
{
	// ��ȣ������ ����
	if (m_strSelectedGroupName != m_strVolumeGroupName)
	{
		AfxMessageBox(_T("��ȣ�������� ǥ�������� �߰��� ��ȣ�� �����Ͽ� �ֽʽÿ�"));
		m_TabCtrl.SetCurSel(0);
		DisplayGrids();
		Display();
		return;
	}
	// ���õ� �׸��� Index
	INT nVolIdx = m_pGrid_Selected->GetRow() - 1;

	//5163 ǥ������ ��������
	CString strVolumeKey;	// 
	strVolumeKey.Empty();
	TCHAR szVolumeKey[32];
	szVolumeKey[0] = _T('\0');

	CCOVER_MAIN dlg(this);
	dlg.SetPubType(_SERIAL_);
	dlg.SetLocalPath ( _T("..\\bmp\\cover_info\\") );

//	if ( m_nManageMode == MNG_INSERT )
//	{
//		dlg.SetInsertMode(TRUE);
//		//if ( dlg.DoModal() == IDOK )
//		//{
//			dlg.SetBasicName ( szVolumeKey );
//			dlg.SetSpeciesKey ( szVolumeKey );
//			dlg.DoModal();
//			dlg.GetSpeciesKey ( strVolumeKey );
//			m_pCatESLMgr->m_pDM_Volume->SetCellData(_T("SV_��KEY"), strVolumeKey , 0 );
//		//}
//	}
//	else if ( m_nManageMode == MNG_MODIFY )
//	{
		CString strRecKey;
		strRecKey = m_pDM_Volume->GetCellData(_T("SV_��KEY"), nVolIdx );
		_tcscpy ( szVolumeKey , strRecKey.GetBuffer(0) );
		dlg.SetInsertMode(FALSE);
		dlg.SetBasicName ( szVolumeKey );
		dlg.SetSpeciesKey ( szVolumeKey );
		dlg.DoModal();
//	}

	
}

VOID CSEVolumeDlg::OnKeyUpGridBookSvf(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnKeyUpGridHoldBookSvf(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnKeyUpGridHoldAppendixSvf(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnKeyUpGridVolumeSvf(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnKeyUpGridAppendixSvf(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	Display();

	EFS_END
}


HBRUSH CSEVolumeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
