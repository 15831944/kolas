// BO_ACQ_PrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_PrintDlg.h"
#include "ExcelExporter.h"
#include "SubTotalPrintDataOptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PrintDlg dialog


CBO_ACQ_PrintDlg::CBO_ACQ_PrintDlg(SPECIES_TYPE nSpeciesType, ACQ_TYPE nAcqType, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_PrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_PrintDlg)
	m_bDataSortYN = FALSE;
	//}}AFX_DATA_INIT
	m_pPrintMgr = NULL;
	m_arrData.RemoveAll();
	m_arrReportFileName.RemoveAll();
	m_arrReportType.RemoveAll();
	m_nSpeciesType = nSpeciesType;
	m_nAcqType = nAcqType;
	m_bCreate = FALSE;
	m_strReportFileName.Empty();
	m_nReportType = -1;
	m_pPrintDM = NULL;
	m_pGroupCollection = NULL;
	m_bSpecies = TRUE;
	m_bSeriesPaper = FALSE;
	m_bExcelExport = FALSE;
	m_bKOLAS2Format = FALSE;
	m_nPrintDataType = -1;

	m_strGroupingName = _T("");
	// m_pPrintDM�� �з����� ������ �˸��ƽ���
	m_strDMGroupingAlias = _T("");

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	PrivacyPrintLogYN = FALSE;
}

CBO_ACQ_PrintDlg::~CBO_ACQ_PrintDlg()
{
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
}

VOID CBO_ACQ_PrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_PrintDlg)
	DDX_Control(pDX, IDC_cmbCPNT_PRINT_LIST, m_ComboBox);
	DDX_Check(pDX, IDC_chkPRINT_DATA_SORT_YN, m_bDataSortYN);
	// 16/03/29 ������ : �Ѽ� �����ϱ�
/// ADD -------------------------------------------------------------------------------------
	// 18.10.30 JMJ KOL.RED.2018.021
	/*
	CLSID clsExcelApp;
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		DDX_Text(pDX, IDC_btnEXCEL_EXPORT, CString(_T("�Ѽ����")));	// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
	}
	*/
/// END -------------------------------------------------------------------------------------
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_PrintDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_PrintDlg)
	ON_CBN_SELCHANGE(IDC_cmbCPNT_PRINT_LIST, OnSelchangecmbPRINTLIST)
	ON_BN_CLICKED(IDC_btnSERIES_PAPER, OnbtnSERIESPAPER)
	ON_BN_CLICKED(IDC_btnEXCEL_EXPORT, OnbtnEXCELEXPORT)
	ON_BN_CLICKED(IDC_btnKOLAS2_FORMAT, OnbtnKOLAS2FORMAT)
	ON_BN_CLICKED(IDC_chkPRINT_DATA_SORT_YN, OnchkPRINTDATASORTYN)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PrintDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ] 
// - ���̾˷α�â �ʱ�ȭ (�޺��ڽ� �� SIReport ���ϸ�� Ÿ�Լ���)
//
// [ �̴Ͻ��� ]
// 1. SIReport ����� ���Ͽ� CBasicPrintMgr Ŭ������ ��������� �Ҵ��Ѵ�.
// 2. �޺��ڽ��� �ʱ�ȭ �Ѵ�.
// 3. ��Ÿ���� �Ǵ��Ͽ� �޺��ڽ� �� SIReport ���ϸ�/Ÿ���� �����Ͽ�
//	  Dialog â�� �ʱ�ȭ �Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CBO_ACQ_PrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//=======================================================================
	// 1. SIReport ����� ���Ͽ� CBasicPrintMgr Ŭ������ ��������� �Ҵ��Ѵ�.
	//=======================================================================
	m_pPrintMgr = new CBasicPrintMgr(this);

	
	//=======================================================================
	// 2. �޺��ڽ��� �ʱ�ȭ �Ѵ�.
	//=======================================================================
	m_ComboBox.ResetContent();
	
	
	//=======================================================================
	// 3. ��Ÿ���� �Ǵ��Ͽ� �޺��ڽ� �� SIReport ���ϸ�/Ÿ���� �����Ͽ�
	//	  Dialog â�� �ʱ�ȭ �Ѵ�.
	//=======================================================================
	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("��� ����Ʈ ���� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ش� ��Ÿ���� ���� �޺��ڽ� �� SiReport ���ϸ�/Ÿ���� �����Ͽ� 
//	 Dialog â�� �ʱ�ȭ �Ѵ�.
//	
// [ �̴Ͻ��� ]
// 1. ESL_MANAGE_TBL�� ���� ��� DB�� 5163�̳� ��������� üũ�ϱ����� 
//	  ���� �޾ƿ´�.
// 2. ��Ÿ�Կ����� ������ �߰��Ѵ�.
//	  �޺��ڽ� �� SIReport ���ϸ�, SIReport Ÿ���� �����Ѵ�.
// 3. ��Ÿ�Կ� �°� ���ǵ� �޺��ڽ� ������ �߰��Ѵ�.
// 4. �޺��ڽ��� �⺻���� ù��° ������ �Ѵ�.
// 5. ���õ� �޺��ڽ��� ���� SIReport Ÿ���� �����ϰ� 
//	  ��ư �� üũ�ڽ��� Visible�� �Ǵ��Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CBO_ACQ_PrintDlg::Init()
{

	//=======================================================================
	// 1. ESL_MANAGE_TBL�� ���� ��� DB�� 5163�̳� ��������� üũ�ϱ����� 
	//	  ���� �޾ƿ´�.
	//=======================================================================
	INT nCnt = 0;
	CString strValue, str64, strTemp;
	CStringArray astrHeadTitle, astrTailTitle;
	GetManageValue( _T("��Ÿ") , _T("����") , _T("5163") , _T("5163") , strValue );
	GetManageValue( _T("��Ÿ") , _T("����") , _T("����") , _T("����") , str64 );
	GetManageValue( _T("��Ÿ") , _T("����") , _T("�����"), _T("�����") , m_strIs90 );
	

	astrHeadTitle.RemoveAll();		astrTailTitle.RemoveAll();
	//=======================================================================
	// 2. ��Ÿ�Կ����� ������ �߰��Ѵ�.
	//	  �޺��ڽ� �� SIReport ���ϸ�, SIReport Ÿ���� �����Ѵ�.
	//=======================================================================
	switch ( m_nSpeciesType )
	{
	// ���ǵ��� ���� ��Ÿ��
	case SPECIES_TYPE_INVALID:	return FALSE;
		break;

	// ���ʼ����ڷ�
	case BO_BASIC:
	{
		SetWindowText(_T("�����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�����ڷ���"));				m_arrReportFileName.Add(_T("K2UP_�����ڷ���"));				m_arrReportType.Add(BO_ACQ_BASIC_DATA);

	}
		break;

	// �����ڷ�
	case BO_IMPORT:
	{
		SetWindowText(_T("�����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�����ڷ���"));				m_arrReportFileName.Add(_T("K2UP_�����ڷ���"));				m_arrReportType.Add(BO_ACQ_IMPORT_DATA);
	}
		break;

	// ��Ȱ���ڷ�
	case BO_RECYCLE:
	{
		SetWindowText(_T("��Ȱ���ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("��Ȱ���ڷ���"));			m_arrReportFileName.Add(_T("K2UP_��Ȱ���ڷ���"));				m_arrReportType.Add(BO_ACQ_RECYCLE_DATA);
	}
		break;

	// ����(��ġ)��� �ڷ�
	case BO_REQUEST:
	{
			if( str64 == _T("Y") )	strTemp = _T("����");
			else				strTemp = _T("��ġ");
		
			SetWindowText(strTemp+_T("����ڷ� ��¹� ����Ʈ"));
			m_arrData.Add(strTemp+_T("����ڷ���"));			m_arrReportFileName.Add(_T("K2UP_��ġ����ڷ���"));			m_arrReportType.Add(BO_ACQ_REQUEST_DATA);
		//JOB.2019.0073 : �浹 License ����
//		m_arrData.Add(_T("���Ϲ߼۸��"));				m_arrReportFileName.Add(_T("K2UP_��ġ������Ϲ߼۸��"));		m_arrReportType.Add(BO_ACQ_REQUEST_MAIL_SEND_LIST);
	}	
		break;

	// ����(���)�ڷ�
	case BO_PURCHASE:
	{
		// 2005-01-18 5163�� ��� ���Դ���ڷ���(��û�μ�����) �̶�� RPT ������ �߰��Ѵ�.
		
		SetWindowText(_T("���Դ���ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("���Դ���ڷ���"));				m_arrReportFileName.Add(_T("K2UP_���Դ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_DATA);

		// 2005-01-18
		if( strValue == _T("Y") && str64 != _T("Y") )
		{
			m_arrData.Add(_T("���Դ���ڷ���(��û�μ�����)"));	m_arrReportFileName.Add(_T("K2UP_���Դ���ڷ���(��û�μ�����)"));	m_arrReportType.Add(BO_ACQ_PURCHASE_DATA_5163);
		}
			
		// ���� �������� �޺��ڽ��� �ش� ���� �߰� (64)
		// 2005-12-02 ����뿡���� ��� ��û
		// �����з��ڵ�1��
		// 2005-09-05 ������
		//
		// ��ó��
		// 2005-09-26 ������
		//
		if( str64 ==_T("Y") )
		{
			m_arrData.Add(_T("���Դ���ڷ���(��ó��)"));			m_arrReportFileName.Add(_T("K2UP_���Դ���ڷ���_��ó��"));	m_arrReportType.Add(BO_ACQ_PURCHASE_DATA_DEPT);
		}

		m_arrData.Add(_T("���Դ���ڷ���(�����з��ڵ庰)"));	m_arrReportFileName.Add(_T("K2UP_���Դ���ڷ���_�����з�1��"));	m_arrReportType.Add(BO_ACQ_PURCHASE_DATA_USERDEFINECODE1);
		m_arrData.Add(_T("���Դ���ڷ���(��ϱ��к�)")	);	m_arrReportFileName.Add(_T("K2UP_���Դ���ڷ���_��ϱ��к�"));	m_arrReportType.Add(BO_ACQ_PURCHASE_DATA_REGCODE);
		m_arrData.Add(_T("���Ǵ���ڷ���(������)")		);	m_arrReportFileName.Add(_T("K2UP_���Ǵ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO);
		m_arrData.Add(_T("���Ǵ���ڷ���(����ó��)")		);	m_arrReportFileName.Add(_T("K2UP_���Ǵ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER);
		m_arrData.Add(_T("���Ǵ���ڷ���(�������к�)")	);	m_arrReportFileName.Add(_T("K2UP_���Ǵ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE);
		m_arrData.Add(_T("���ǰ���ڷ���(������)")		);	m_arrReportFileName.Add(_T("K2UP_���ǰ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO);
		m_arrData.Add(_T("���ǰ���ڷ���(����ó��)")		);	m_arrReportFileName.Add(_T("K2UP_���ǰ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER);
		m_arrData.Add(_T("���ǰ���ڷ���(�������к�)")	);	m_arrReportFileName.Add(_T("K2UP_���ǰ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE);
		m_arrData.Add(_T("�ֹ��ڷ���(������)")			);	m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_������"));			m_arrReportType.Add(BO_ACQ_ORDER_DATA_DIVNO);
		m_arrData.Add(_T("�ֹ��ڷ���(����ó��)")			);	m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_����ó��"));			m_arrReportType.Add(BO_ACQ_ORDER_DATA_PROVIDER);
		m_arrData.Add(_T("�ֹ��ڷ���(�������к�)")		);	m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_�������к�"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_SUBJECTCODE);
		// 2005-01-26
		if( strValue == _T("Y") && str64 != _T("Y") )
		{
			CString strName;
			strName = _T("�ֹ��ڷ���(�����з��ڵ庰)") ;
			m_arrData.Add(strName);	m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_�����з��ڵ�1��"));	m_arrReportType.Add(BO_ACQ_ORDER_DATA_MEDIACODE);
		}
	}
		break;

	// �˼�����ڷ�
	case BO_ORDER:
	{
		SetWindowText(_T("�˼�����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�˼�����ڷ���"));			m_arrReportFileName.Add(_T("K2UP_�˼�����ڷ���"));			m_arrReportType.Add(BO_ACQ_CHECKINING_DATA);
		m_arrData.Add(_T("�ֹ��ڷ���(������)"));		m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_������"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_DIVNO);
		m_arrData.Add(_T("�ֹ��ڷ���(����ó��)"));	m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_����ó��"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_PROVIDER);
		m_arrData.Add(_T("�ֹ��ڷ���(�������к�)"));	m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_�������к�"));	m_arrReportType.Add(BO_ACQ_ORDER_DATA_SUBJECTCODE);
		m_arrData.Add(_T("�˼��ڷ���"));				m_arrReportFileName.Add(_T("K2UP_�˼��ڷ���"));				m_arrReportType.Add(BO_ACQ_CHECKIN_DATA);
		m_arrData.Add(_T("�̰˼��ڷ���"));			m_arrReportFileName.Add(_T("K2UP_�̰˼��ڷ���"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_DATA);
		m_arrData.Add(_T("���Խ�û�ڷᵵ�����"));		m_arrReportFileName.Add(_T("K2UP_���Խ�û�ڷᵵ�����"));		m_arrReportType.Add(BO_ACQ_CHECKIN_REQUEST_DATA);
		m_arrData.Add(_T("�˼��ڷ����"));				m_arrReportFileName.Add(_T("K2UP_�˼��ڷ����"));				m_arrReportType.Add(BO_ACQ_CHECKIN_STATISTICS);
		m_arrData.Add(_T("�̰˼��ڷ����"));			m_arrReportFileName.Add(_T("K2UP_�̰˼��ڷ����"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_STATISTICS);
		if(strValue == _T("Y"))
		{
			m_arrData.Add(_T("�����ڷ���"));				m_arrReportFileName.Add(_T("K2UP_�����ڷ���"));				m_arrReportType.Add(BO_ACQ_ORDER_DATA_ORDER);
		}
	}
		break;

	// �̰˼��ڷ�, �̳��ڷ�
	case BO_MISSING:
	{
		SetWindowText(_T("�̳��ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�̳��ڷ���"));				m_arrReportFileName.Add(_T("K2UP_�̳��ڷ���"));				m_arrReportType.Add(BO_ACQ_MISSING_DATA);
		m_arrData.Add(_T("�̰˼��ڷ����"));			m_arrReportFileName.Add(_T("K2UP_�̰˼��ڷ����"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_STATISTICS);
	}
		break;

	// �˼��Ϸ��ڷ�, ����ΰ��ڷ�
	case BO_REGOBJ:
		{
			if( m_nAcqType == PURCHASE )
	{
		SetWindowText(_T("���Ե���ΰ����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("���Ե���ΰ����ڷ���"));	m_arrReportFileName.Add(_T("K2UP_���Ե���ΰ����ڷ���"));	m_arrReportType.Add(BO_ACQ_PURCHASE_REGOBJ_DATA);
		m_arrData.Add(_T("�˼��Ϸ��ڷ���"));			m_arrReportFileName.Add(_T("K2UP_�˼��Ϸ��ڷ���"));			m_arrReportType.Add(BO_ACQ_CHECKIN_COMPLETE_DATA);
		m_arrData.Add(_T("�˼��ڷ����"));				m_arrReportFileName.Add(_T("K2UP_�˼��ڷ����"));				m_arrReportType.Add(BO_ACQ_CHECKIN_STATISTICS);
	}
			else if( m_nAcqType == DONATE )
			{
				SetWindowText(_T("��������ΰ����ڷ� ��¹� ����Ʈ"));
				m_arrData.Add(_T("��������ΰ����ڷ���"));	m_arrReportFileName.Add(_T("K2UP_��������ΰ����ڷ���"));	m_arrReportType.Add(BO_ACQ_DONATE_REGOBJ_DATA);
			}
		}
		break;

	// ���������ڷ�
	case BO_DONATE:
	{
		SetWindowText(_T("���������ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("���������ڷ���"));					m_arrReportFileName.Add(_T("K2UP_���������ڷ���"));			m_arrReportType.Add(BO_ACQ_DONATE_DATA);
		m_arrData.Add(_T("���Ǵ���ڷ���(������)")		);	m_arrReportFileName.Add(_T("K2UP_���Ǵ���ڷ���_����"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKING_DATA_DIVNO);
		m_arrData.Add(_T("���Ǵ���ڷ���(�����ں�)")		);	m_arrReportFileName.Add(_T("K2UP_���Ǵ���ڷ���_����"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKING_DATA_PROVIDER);
		m_arrData.Add(_T("���Ǵ���ڷ���(�������к�)")	);	m_arrReportFileName.Add(_T("K2UP_���Ǵ���ڷ���_����"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE);
		m_arrData.Add(_T("���ǰ���ڷ���(������)")		);	m_arrReportFileName.Add(_T("K2UP_���ǰ���ڷ���_����"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKED_DATA_DIVNO);
		m_arrData.Add(_T("���ǰ���ڷ���(�����ں�)")		);	m_arrReportFileName.Add(_T("K2UP_���ǰ���ڷ���_����"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKED_DATA_PROVIDER);
		m_arrData.Add(_T("���ǰ���ڷ���(�������к�)")	);	m_arrReportFileName.Add(_T("K2UP_���ǰ���ڷ���_����"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE);
		m_arrData.Add(_T("�����ڶ����"));					m_arrReportFileName.Add(_T("K2UP_�����ڶ����"));				m_arrReportType.Add(DONATOR_LABEL);
	}
		break;

	// ���� �̵���ڷ�
	case BO_NONDONATE:
	{
		SetWindowText(_T("�����̵���ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�����̵���ڷ���"));		m_arrReportFileName.Add(_T("K2UP_�����̵���ڷ���"));			m_arrReportType.Add(BO_ACQ_NONDONATE_DATA);
	}
		break;

	//�����ڷ�
	case BO_PURCHASE_DATA_PRINT:
	{
		SetWindowText(_T("�����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�����ڷ���"));	m_arrReportFileName.Add(_T("K2UP_�����ڷ���"));	m_arrReportType.Add(BO_PURCHASE_DATA);
	}
		break;

	// �����ڷ�
	case BO_DONATE_DATA_PRINT:
	{
		SetWindowText(_T("�����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�����ڷ���"));	m_arrReportFileName.Add(_T("K2UP_�����ڷ���"));	m_arrReportType.Add(BO_DONATE_DATA);
		m_arrData.Add(_T("�����ڸ��"));	m_arrReportFileName.Add(_T("K2UP_�����ڸ��"));		m_arrReportType.Add(BO_ACQ_DEFAULT);      
	}
		break;

	default:	return FALSE;
		break;
	}


	//=======================================================================
	// 3. ��Ÿ�Կ� �°� ���ǵ� �޺��ڽ� ������ �߰��Ѵ�.
	//=======================================================================
	nCnt = m_arrData.GetSize();
	for( INT i = 0 ; i < nCnt ; i++ ) 
		m_ComboBox.InsertString(i, (m_arrData.GetAt(i)).GetBuffer(0));
	

	//=======================================================================
	// 4. �޺��ڽ��� �⺻���� ù��° ������ �Ѵ�.
	//=======================================================================
	m_ComboBox.SetCurSel(0);


	//=======================================================================
	// 5. ���õ� �޺��ڽ��� ���� SIReport Ÿ���� �����ϰ� 
	//	  ��ư �� üũ�ڽ��� Visible�� �Ǵ��Ѵ�.
	//=======================================================================
	OnSelchangecmbPRINTLIST();

	return TRUE;
}

BOOL CBO_ACQ_PrintDlg::Create(CWnd * pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ɼ�â�� ���� �������� ���� ��������� �����ϰ�, ����� ����� ������ �Ŀ�
//	 �ڽ��� ȣ���� Ŭ�������� DM�� �����϶�� �޽����� �����ش�.(����)
//
// [ �̴Ͻ��� ]
// 1. SIReport Ÿ���� �������� �ʾҴٸ� �����Ѵ�.
// 2. _T("KOLAS2����") ��ư�� �����ٸ� nExtraInfo=1000�� �����Ѵ�.
// 3. SIReport Ÿ���� �Ʒ��� �ش��Ҷ����� ��� �ɼ�â���� _T("��µ����� ��˻�")�� ������ �ʰ� �Ѵ�.
//		- i.	�ֹ��ڷ����ϰ�(��/å)
//		- ii.	�ֹ��ڷᱸ��ó��(��/å)
//		- iii.	�ֹ��ڷ���������(��/å)
//		- iv.	���Ǵ���ڷ���_������
//		- v.	���Ǵ���ڷ���_����ó��
//		- vi.	���Ǵ���ڷ���_�������к�
//		- vii.	���ǰ���ڷ���_������
//		- viii.	���ǰ���ڷ���_����ó��
//		- ix.	���ǰ���ڷ���_�������к�
//		- x.	�ֹ��ڷ��ϸ�ü���к�_5163(��/å)
// 4. SIReport Ÿ���� �Ʒ��� �ش���� ���������� ��� �ɼ�â���� ���õ� �ɼǰ��� �޾ƿ� ��������� �����Ѵ�.
//		- i.	���Ϲ߼���Ȳ����Ʈ
//		- ii.	����Ʈ
//		- iii.	�˼��ڷ����
//		- iv.	�̰˼��ڷ����
// 5. DM�� �����ϱ� ���� �޽����� �ڽ��� ȣ���� Ŭ�������� ������.
//
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_PrintDlg::OnOK() 
{
	INT nExtraInfo;
	CSubTotalPrintDataOptionDlg dlg(this);


	//=======================================================================
	// 1. SIReport Ÿ���� �������� �ʾҴٸ� �����Ѵ�.
	//=======================================================================
	if(m_nReportType < 0) return ;
	

	//=======================================================================
	// 2. _T("KOLAS2����") ��ư�� �����ٸ� nExtraInfo=1000�� �����Ѵ�.
	//=======================================================================
	if(m_bKOLAS2Format) nExtraInfo = 1000;
	else nExtraInfo = 0;


	//=======================================================================
	// 3. SIReport Ÿ���� �Ʒ��� �ش��Ҷ����� ��� �ɼ�â���� _T("��µ����� ��˻�")�� ������ �ʰ� �Ѵ�.
	//		- i.	�ֹ��ڷ����ϰ�(��/å)
	//		- ii.	�ֹ��ڷᱸ��ó��(��/å)
	//		- iii.	�ֹ��ڷ���������(��/å)
	//		- iv.	���Ǵ���ڷ���_������
	//		- v.	���Ǵ���ڷ���_����ó��
	//		- vi.	���Ǵ���ڷ���_�������к�
	//		- vii.	���ǰ���ڷ���_������
	//		- viii.	���ǰ���ڷ���_����ó��
	//		- ix.	���ǰ���ڷ���_�������к�
	//		- x.	�ֹ��ڷ��ϸ�ü���к�_5163(��/å)
	//=======================================================================
	if( m_nReportType == BO_ACQ_ORDER_DATA_DIVNO	||
		m_nReportType == BO_ACQ_ORDER_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_ORDER_DATA_SUBJECTCODE || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE	||
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE ||  
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_DIVNO ||  
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_PROVIDER ||  
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE ||  
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_DIVNO ||  
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_PROVIDER ||  
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE ||  
		m_nReportType == BO_ACQ_ORDER_DATA_MEDIACODE || //2005-01-29 5163�� �߰�.
		m_nReportType == BO_ACQ_ORDER_DATA_ORDER )  //2006-01-23 5163�� �߰�.
	{
		dlg.SetSubTotalPrintYN(TRUE);
	}
	else
	{
		dlg.SetSubTotalPrintYN(FALSE);
	}


	//=======================================================================
	// 4. SIReport Ÿ���� �Ʒ��� �ش���� ���������� ��� �ɼ�â���� ���õ� �ɼǰ��� �޾ƿ� ��������� �����Ѵ�.
	//		- i.	���Ϲ߼���Ȳ����Ʈ
	//		- ii.	����Ʈ
	//		- iii.	�˼��ڷ����
	//		- iv.	�̰˼��ڷ����
	//=======================================================================
	if( m_nReportType != BO_ACQ_REQUEST_MAIL_SEND_LIST && 
		m_nReportType != BO_ACQ_DEFAULT &&
		m_nReportType != BO_ACQ_CHECKIN_STATISTICS && 
		m_nReportType != BO_ACQ_NONCHECKIN_STATISTICS )
	{
		if( dlg.DoModal() == IDOK )		m_nPrintDataType = dlg.GetPrintDataType();
		else
		{
			if(	m_bExcelExport )
				m_bExcelExport = FALSE;
			return ;
		}
	}

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	CString strLogName;
	GetDlgItem(IDC_cmbCPNT_PRINT_LIST)->GetWindowText(strLogName);
	
	if(PrivacyPrintLogYN) {		
		m_pPrintMgr->SetPrivacyPrintLogParm( TRUE, m_pInfo->USER_PK, m_pInfo->USER_ID, strLogName, m_pInfo->MANAGE_CODE, strLogName,0);
	}
	//

	//=======================================================================
	// 5. DM�� �����ϱ� ���� �޽����� �ڽ��� ȣ���� Ŭ�������� ������.
	//=======================================================================
	::PostMessage(pParentMgr->GetSafeHwnd(), MAKE_PRINT_DM, m_nReportType, nExtraInfo);
}

BOOL CBO_ACQ_PrintDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

INT CBO_ACQ_PrintDlg::Print()
{
	INT ids;

	if( m_bExcelExport == TRUE )	ids = PrintExcel();
	else							ids = PrintPaper();
	
	m_bExcelExport = m_bKOLAS2Format = m_bSeriesPaper = FALSE;
	
	return ids;
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���õ� �޺��ڽ��� ���� SIReport Ÿ���� �����ϰ� ��ư �� üũ�ڽ��� Visible�� �Ǵ��Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ���� ���õ� �޺��ڽ��� �ε��� ��ȣ�� �����´�.
// 2. ���� ���õ� �޺��ڽ��� ������ �����´�.
// 3. �޺��ε��� ��ȣ�� �ش� �޺��� �ش��ϴ� SIReport ���ϸ��� ��������� �����Ѵ�.
// 4. �ش� �޺��� �ش��ϴ� SIReport Ÿ���� ��������� �����Ѵ�.
// 5. ������¹�ư�� ȭ�鿡 ���̰� �Ѵ�.
// 5-1. ����� ��¹� ���Ŀ� ���Ͽ� �з���� �з��� DM�˸��ƽ��� �����Ѵ�.
// 6. SIReport Ÿ���� �Ʒ��� �ش��Ҷ����� _T("���ӿ���") ��ư�� ���̰� �Ѵ�.
//		- i.	���Ǵ���ڷ���_������
//		- ii.	���Ǵ���ڷ���_����ó��
//		- iii.	���Ǵ���ڷ���_�������к�
//		- iv.	���ǰ���ڷ���_������
//		- v.	���ǰ���ڷ���_����ó��
//		- vi.	���ǰ���ڷ���_�������к�
// 7. SIReport Ÿ���� �Ʒ��� �ش��Ҷ����� _T("KOLAS2����") ��ư�� ���̰� �Ѵ�.
//		- i.	���Ǵ���ڷ���_������
//		- ii.	���Ǵ���ڷ���_����ó��
//		- iii.	���Ǵ���ڷ���_�������к�
//		- iv.	���ǰ���ڷ���_������
//		- v.	���ǰ���ڷ���_����ó��
//		- vi.	���ǰ���ڷ���_�������к�
//		- vii.	�ֹ��ڷ����ϰ�(��/å)
//		- viii.	�ֹ��ڷ��ϱ���ó��(��/å)
//		- ix.	�ֹ��ڷ���������(��/å)
//		- x.	�ֹ��ڷ��ϸ�ü���к�_5163(��/å)
// 8. SIReport Ÿ���� �Ʒ��� �ش��Ҷ����� ��½� ������ ���� ���ϵ���
//	  �������Ǽ�����ư �� �ڷ����� üũ�ڽ��� Disable ��Ų��.
//		- i.	���Ϲ߼���Ȳ����Ʈ
//		- ii.	�̰˼��ڷ����
//		- iii.	�˼��ڷ����
//		- iv.	����Ʈ (??)
// 9. SIReport Ÿ���� �Ʒ��� �ش��Ҷ����� _T("�������") ��ư�� �Ⱥ��̰� �Ѵ�.	
//		- i.	�˼��ڷ����
//		- ii.	�̰˼��ڷ����	
//
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_PrintDlg::OnSelchangecmbPRINTLIST() 
{
	INT nCurSel ;
	CString str;
	
	//=======================================================================
	// 1. ���� ���õ� �޺��ڽ��� �ε��� ��ȣ�� �����´�.
	//=======================================================================
	nCurSel = m_ComboBox.GetCurSel();
	
	//=======================================================================
	// 2. ���� ���õ� �޺��ڽ��� ������ �����´�.
	//=======================================================================
	m_ComboBox.GetWindowText(str);

	
	//=======================================================================
	// 3. �޺��ε��� ��ȣ�� �ش� �޺��� �ش��ϴ� SIReport ���ϸ��� ��������� �����Ѵ�.
	//=======================================================================
	m_strReportFileName = m_arrReportFileName.GetAt(nCurSel);


	//=======================================================================
	// 4. �ش� �޺��� �ش��ϴ� SIReport Ÿ���� ��������� �����Ѵ�.
	//=======================================================================
	m_nReportType = m_arrReportType.GetAt(nCurSel);

	
	//=======================================================================
	// 5. ������¹�ư�� ȭ�鿡 ���̰� �Ѵ�.
	//=======================================================================
	// 18.10.30 JMJ KOL.RED.2018.021
	// ��±�� ����
	//GetDlgItem(IDC_btnEXCEL_EXPORT)->ShowWindow(SW_SHOW);


	//=======================================================================
	// 5-1. ����� ��¹� ���Ŀ� ���Ͽ� �з���� �з��� DM�˸��ƽ��� �����Ѵ�.
	// 2006.03.15 KBS - ����� ��� ���Ĺ� ����
	//=======================================================================
	switch( m_nReportType )
	{
	case BO_ACQ_PURCHASE_DATA_USERDEFINECODE1:
		{
			m_strGroupingName	 = _T("�����з�");
			m_strDMGroupingAlias = _T("UDF_����������ڵ�1");
		}
		break;
	
	case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
	case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
	case BO_ACQ_ORDER_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		{
			m_strGroupingName	 = _T("������ȣ");
			m_strDMGroupingAlias = _T("BP_�׷��ȣ");
		}
		break;

	case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
	case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
	case BO_ACQ_ORDER_DATA_PROVIDER:
		{
			m_strGroupingName	 = _T("����ó");
			m_strDMGroupingAlias = _T("UDF_����ó��ȣ����");
		}
		break;
	case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		{
			m_strGroupingName	 = _T("������");
			m_strDMGroupingAlias = _T("BP_�����ڸ�");
		}
		break;
	case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:
	case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:
	case BO_ACQ_ORDER_DATA_SUBJECTCODE:
	case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
	case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
		{
			m_strGroupingName	 = _T("��������");
			m_strDMGroupingAlias = _T("UDF_�������м���");
		}
		break;
	default:
		break;
	}

	//=======================================================================
	// 6. SIReport Ÿ���� �Ʒ��� �ش��Ҷ����� _T("���ӿ���") ��ư�� ���̰� �Ѵ�.
	//		- i.	���Ǵ���ڷ���_������
	//		- ii.	���Ǵ���ڷ���_����ó��
	//		- iii.	���Ǵ���ڷ���_�������к�
	//		- iv.	���ǰ���ڷ���_������
	//		- v.	���ǰ���ڷ���_����ó��
	//		- vi.	���ǰ���ڷ���_�������к�
	//=======================================================================
	if( m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_DIVNO || 
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE		)
	{
		GetDlgItem(IDC_btnSERIES_PAPER)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_btnSERIES_PAPER)->ShowWindow(SW_HIDE);
	}


	//=======================================================================
	// 7. SIReport Ÿ���� �Ʒ��� �ش��Ҷ����� _T("KOLAS2����") ��ư�� ���̰� �Ѵ�.
	//		- i.	���Ǵ���ڷ���_������
	//		- ii.	���Ǵ���ڷ���_����ó��
	//		- iii.	���Ǵ���ڷ���_�������к�
	//		- iv.	���ǰ���ڷ���_������
	//		- v.	���ǰ���ڷ���_����ó��
	//		- vi.	���ǰ���ڷ���_�������к�
	//		- vii.	�ֹ��ڷ����ϰ�(��/å)
	//		- viii.	�ֹ��ڷ��ϱ���ó��(��/å)
	//		- ix.	�ֹ��ڷ���������(��/å)
	//		- x.	�ֹ��ڷ��ϸ�ü���к�_5163(��/å)
	//=======================================================================
	if( m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_DIVNO || 
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_ORDER_DATA_DIVNO || 
		m_nReportType == BO_ACQ_ORDER_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_ORDER_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_ORDER_DATA_MEDIACODE || //2005-01-29 5163�� �߰�.
		m_nReportType == BO_ACQ_ORDER_DATA_ORDER )  //2006-01-23 5163�� �߰�.
	{
		GetDlgItem(IDC_btnKOLAS2_FORMAT)->ShowWindow(SW_SHOW);
	}
	else
		GetDlgItem(IDC_btnKOLAS2_FORMAT)->ShowWindow(SW_HIDE);



	//=======================================================================
	// 8. SIReport Ÿ���� �Ʒ��� �ش��Ҷ����� ��½� ������ ���� ���ϵ���
	//	  �������Ǽ�����ư �� �ڷ����� üũ�ڽ��� Disable ��Ų��.
	//		- i.	���Ϲ߼���Ȳ����Ʈ
	//		- ii.	�̰˼��ڷ����
	//		- iii.	�˼��ڷ����
	//		- iv.	����Ʈ (??)
	//=======================================================================
	if( m_nReportType == BO_ACQ_REQUEST_MAIL_SEND_LIST || 
		m_nReportType == BO_ACQ_NONCHECKIN_STATISTICS ||
		m_nReportType == BO_ACQ_CHECKIN_STATISTICS || 		
		m_nReportType == BO_ACQ_DEFAULT ||
		m_nReportType == DONATOR_LABEL)
	{
		GetDlgItem(IDC_chkPRINT_DATA_SORT_YN)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSET_SORT_CONDITION)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_chkPRINT_DATA_SORT_YN)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnSET_SORT_CONDITION)->EnableWindow(TRUE);
	}


	//=======================================================================
	// 9. SIReport Ÿ���� �Ʒ��� �ش��Ҷ����� _T("�������") ��ư�� �Ⱥ��̰� �Ѵ�.	
	//		- i.	�˼��ڷ����
	//		- ii.	�̰˼��ڷ����	
	//=======================================================================
	if( m_nReportType == BO_ACQ_CHECKIN_STATISTICS || 
		m_nReportType == BO_ACQ_NONCHECKIN_STATISTICS ||
		m_nReportType == DONATOR_LABEL)
	{
		GetDlgItem(IDC_btnEXCEL_EXPORT)->ShowWindow(SW_HIDE);
	}

	CString strTmp;
	m_ComboBox.GetWindowText(strTmp);
	//20070521 ADD PJW : ���������ϰ�� ����������
	if ( strTmp.Find(_T("��������")) >= 0 )
	{
		( (CButton*) GetDlgItem(IDC_chkPRINT_DATA_SORT_YN) )->SetCheck(1);
		m_bDataSortYN = TRUE;
	}
}

VOID CBO_ACQ_PrintDlg::OnbtnSERIESPAPER() 
{
	// TODO: Add your control notification handler code here
	m_bSeriesPaper = TRUE;
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������¹�ư �̺�Ʈ
// 
// [ �̴Ͻ��� ]
// 1. ��������� ��������̶�� �����Ѵ�.
// 2. OnOK()�Լ��� ȣ���Ѵ�. (��¹�ưȣ��)
// 
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_PrintDlg::OnbtnEXCELEXPORT() 
{
	//=======================================================================
	// 1. ��������� ��������̶�� �����Ѵ�.
	//=======================================================================
	m_bExcelExport = TRUE;

	//=======================================================================
	// 2. OnOK()�Լ��� ȣ���Ѵ�. (��¹�ưȣ��)
	//=======================================================================
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - CBasicPrintMgrŬ������ ����� ������ �����ϰ� ����Ѵ�.
// [ �̴Ͻ��� ]
// 1. ��¿� DM�� ������� �ʰų� �����Ͱ� ������ ���������Ѵ�.
// 2. ���ε� ��������� �ʱ�ȭ �Ѵ�.
// 3. �׷�����Ŭ������ ���Գ⵵�� �׷������� ������ �Լ�Ÿ�Կ� ���� ��������� �����Ѵ�.
// 4. ��¿� DM�� �����Ѵ�.
// 5. ���ӿ��� ����̶�� ���Ÿ�԰� SIReport�����̸��� �����Ѵ�.
// 6. KOLAS2���� ����̶�� ���Ÿ�԰� SIReport�����̸��� �����Ѵ�.
// 7. �׿� ����̶�� ���Ǵ���ڷ����� üũ�Ͽ� ���Ÿ�԰� SIReport�����̸��� �����Ѵ�.
// 8. ������ ������ ����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PrintDlg::PrintPaper()
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT i, j, ids;
	CString strAcqYear, strGroupInfo, strTmpData, strPurchaseGroupInfo;


	//=======================================================================
	// 1. ��¿� DM�� ������� �ʰų� �����Ͱ� ������ ���������Ѵ�.
	//=======================================================================
	if( m_pPrintDM == NULL )	return -1;
	if( m_pPrintDM->GetRowCount() == 0 )
	{
		ESLAfxMessageBox(_T("��� �����Ͱ� �����ϴ�."));
		return -1;
	}
	
	//=======================================================================
	// 2. ���ε� ��������� �ʱ�ȭ �Ѵ�.
	//=======================================================================
	m_pPrintMgr->InitPrintData();

	//=======================================================================
	// 3. �׷�����Ŭ������ ���Գ⵵�� �׷������� ������ �Լ�Ÿ�Կ� ���� ��������� �����Ѵ�.
	//=======================================================================
	if( m_pGroupCollection != NULL && m_pGroupCollection->GetGroupCount() > 0 )
	{
		strAcqYear = m_pGroupCollection->GetAcqYear();
		m_pGroupCollection->GetGroupInfo(strGroupInfo);
		if( strAcqYear.IsEmpty()	== TRUE	)	strAcqYear	 = _T("�������");
		if( strGroupInfo.IsEmpty()  == TRUE	)	strGroupInfo = _T("�������");
		
		// Key:���Գ⵵, Value:strAcqYear
		m_pPrintMgr->AddPrintData(_T("���Գ⵵"), strAcqYear);

		switch( m_nAcqType )
		{
		// Key:�׷��̸�, Value:strGroupInfo
		case ACQ_TYPE_INVALID:	m_pPrintMgr->AddPrintData(_T("�׷��̸�"), strGroupInfo);
			break;

		// Key:������ȣ, Value:strGroupInfo
		case DONATE:			m_pPrintMgr->AddPrintData(_T("������ȣ"), strGroupInfo);
			break;
		
		// Key:������ȣ, Value:�������� : m_pGroupCollection->GetWorkCode()		������ȣ : strGroupInfo
		case PURCHASE:
		{
			strTmpData = m_pGroupCollection->GetWorkCode();
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������"), strTmpData, strTmpData);
				if( strTmpData.IsEmpty() == TRUE )		strTmpData = _T("�������");
			strPurchaseGroupInfo.Format(_T("�������� : %s     ������ȣ : %s"), strTmpData, strGroupInfo);
			m_pPrintMgr->AddPrintData(_T("������ȣ"), strPurchaseGroupInfo);
		}
			break;
		}
	}


	//=======================================================================
	// 4. ��¿� DM�� �����Ѵ�.
	//=======================================================================
	m_pPrintMgr->SetPrintDM(m_pPrintDM);

	//=======================================================================
	// 5. ���ӿ��� ����̶�� ���Ÿ�԰� SIReport�����̸��� �����Ѵ�.
	//=======================================================================
	if( m_bSeriesPaper == TRUE )
	{
		switch( m_nReportType )
	{
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:		strTmpData = _T("K2UP_���Ǵ���ڷ���_���ӿ���");
			break;

		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:		strTmpData = _T("K2UP_���ǰ���ڷ���_���ӿ���");
			break;

		case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:		strTmpData = _T("K2UP_���Ǵ���ڷ���_����_���ӿ���");
			break;

		case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:		strTmpData = _T("K2UP_���ǰ���ڷ���_����_���ӿ���");
			break;
		
		default:
		{
				ESLAfxMessageBox(_T("���ӿ��� ����� �� �� �����ϴ�."));
				return -1;
		}
			break;
		}

		m_pPrintMgr->SetPrintType(SERIES_PAPER);
		m_pPrintMgr->SetSIReportFilename(strTmpData);
	}
	//=======================================================================
	// 6. KOLAS2���� ����̶�� ���Ÿ�԰� SIReport�����̸��� �����Ѵ�.
	//=======================================================================
	else if( m_bKOLAS2Format == TRUE )
	{
		switch( m_nReportType )
		{
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:		strTmpData = _T("K2_���Ǵ���ڷ���");
			break;

		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:		strTmpData = _T("K2_���ǰ���ڷ���");
			break;

		case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:		strTmpData = _T("K2_���Ǵ���ڷ���_����");
			break;

		case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:		strTmpData = _T("K2_���ǰ���ڷ���_����");
			break;
			
		case BO_ACQ_ORDER_DATA_MEDIACODE:	// 2005-01-29 5163�� �߰�.
		case BO_ACQ_ORDER_DATA_DIVNO:
		case BO_ACQ_ORDER_DATA_PROVIDER:
		case BO_ACQ_ORDER_DATA_SUBJECTCODE:					strTmpData = _T("K2_�ֹ��ڷ���");	
			break;

		default:
		{
				ESLAfxMessageBox(_T("KOLAS2�������� ����� �� �� �����ϴ�."));
				return -1;
		}
			break;
		}
		
		m_pPrintMgr->SetPrintType(m_nReportType);
		m_pPrintMgr->SetSIReportFilename(strTmpData);
	}
	//=======================================================================
	// 7. �׿� ����̶�� ���Ǵ���ڷ����� üũ�Ͽ� ���Ÿ�԰� SIReport�����̸��� �����Ѵ�.
	//=======================================================================
	else
	{
		CString strReportFileName;
		
		if( m_bSpecies == FALSE && m_nReportType == BO_ACQ_CHECKINING_DATA ) 
		{
			strReportFileName = m_strReportFileName + _T("_��");
		}
		else
		{
			strReportFileName = m_strReportFileName;
		}

		m_pPrintMgr->SetPrintType(m_nReportType);
		m_pPrintMgr->SetSIReportFilename(strReportFileName);
	}


	//=======================================================================
	// 8. ������ ������ ����Ѵ�.
	// - 2006.03.14 KBS - ������� ��� �з����� ������������ ����Ѵ�.
	//=======================================================================	
	if( _T("Y") == m_strIs90 )
	{
		switch( m_nReportType )
		{
		case BO_ACQ_PURCHASE_DATA_USERDEFINECODE1:
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:
		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:
		case BO_ACQ_ORDER_DATA_DIVNO:
		case BO_ACQ_ORDER_DATA_PROVIDER:
		case BO_ACQ_ORDER_DATA_SUBJECTCODE:
			{
				INT nStart = 0;
				CString strData, strGrouping;
				POSITION pos;
				CESL_DataMgr TempDM ;
				CESL_DataMgr::reference* pRef = NULL;
				
				m_pPrintMgr->DeleteTempPages();
				TempDM.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
				TempDM.InitDMPKFromFile(m_pPrintDM->DB_MGR_ALIAS);
				TempDM.FreeData();
				strGrouping.Empty();
				for( i=0; i<m_pPrintDM->GetRowCount(); i++ )
				{
					if( m_pPrintDM->GetCellData(_T("UDF_�Ϸù�ȣ"),i) == _T("�Ұ�") )
					{
						if( i < m_pPrintDM->GetRowCount()-1 )
						{
							if( m_pPrintDM->GetCellData(i+1,0) == _T("�Ѱ�") )	i++;
						}
						
						strPurchaseGroupInfo =_T("�������� : "); 
						strPurchaseGroupInfo += strAcqYear +_T(" - ")+ strTmpData +_T(" - ")+ m_pPrintDM->GetCellData(_T("BP_�׷��ȣ"), nStart);
						m_pPrintMgr->AddPrintData( _T("������ȣ"), strPurchaseGroupInfo );

						strGrouping = _T("< ") + m_pPrintDM->GetCellData(m_strDMGroupingAlias,nStart)+_T(" >");
						m_pPrintMgr->AddPrintData( _T("�з�"), strGrouping);
						/*
						if( m_strGroupingName.IsEmpty() == FALSE )
						{
							strGrouping = _T("< ") + m_strGroupingName + _T(" : ") + m_pPrintDM->GetCellData(m_strDMGroupingAlias,nStart)+_T(" >");
							m_pPrintMgr->AddPrintData( _T("�з�"), strGrouping);
						}
						*/

						for( j=nStart; j<=i; j++ )
						{				
							TempDM.InsertRow(-1);
							pos = m_pPrintDM->RefList.GetHeadPosition();
							while( pos )
							{
								pRef = (CESL_DataMgr::reference*)m_pPrintDM->RefList.GetNext(pos);
								if( pRef == NULL ) continue;
								ids = m_pPrintDM->GetCellData(pRef->FIELD_ALIAS, j, strData);
								if( ids < 0 )	continue;
								ids = TempDM.SetCellData(pRef->FIELD_ALIAS, strData, TempDM.GetRowCount()-1);
								if( ids < 0 )	continue;
							}
						}
						m_pPrintMgr->SetPrintDM(&TempDM);
						m_pPrintMgr->MakeXMLPageFile();
						TempDM.FreeData();
						nStart = i+1;
					}		
				}
				ids = m_pPrintMgr->Print2();
			}
			break;

		default:	
			{
				strPurchaseGroupInfo =_T("�������� : "); 
				strPurchaseGroupInfo += strAcqYear +_T(" - ")+ strTmpData +_T(" - ")+ strGroupInfo;
				m_pPrintMgr->AddPrintData( _T("������ȣ"), strPurchaseGroupInfo );
	ids = m_pPrintMgr->Print();
			}
			break;
		}
	}
	else	ids = m_pPrintMgr->Print();

	m_bSeriesPaper = FALSE;
	return ids;
}

INT CBO_ACQ_PrintDlg::PrintExcel()
{
	if(m_pPrintDM == NULL) return -1;
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("��� �����Ͱ� �����ϴ�."));
		return -1;
	}
	CString strTmpData = m_strReportFileName;

	
	//JOB.2019.0017 ADD BY KYJ
	//�������� ���� ����,�α� ����
	//--------------------------------------------------------------------------------------------------------------------------
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	if( PrivacyPrintLogRecord(m_pPrintDM, _T("�������"), _T("E") ,0 ) < 0) return -1;
	//--------------------------------------------------------------------------------------------------------------------------
	

	if( m_bSeriesPaper == TRUE )
	{
		switch( m_nReportType )
		{
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2UP_���Ǵ���ڷ���_���ӿ���");			
			}
			break;

		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2UP_���ǰ���ڷ���_���ӿ���");	
			}
			break;

		case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2UP_���Ǵ���ڷ���_����_���ӿ���");			
			}
			break;

		case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2UP_���ǰ���ڷ���_����_���ӿ���");	
			}
			break;
		}
	}
	else if( m_bKOLAS2Format == TRUE )
	{
		switch( m_nReportType )
		{
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2_���Ǵ���ڷ���");			
			}
			break;

		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2_���ǰ���ڷ���");	
			}
			break;

		case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2_���Ǵ���ڷ���_����");			
			}
			break;

		case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2_���ǰ���ڷ���_����");	
			}
			break;

		case BO_ACQ_ORDER_DATA_DIVNO:
		case BO_ACQ_ORDER_DATA_PROVIDER:
		case BO_ACQ_ORDER_DATA_SUBJECTCODE:
		case BO_ACQ_ORDER_DATA_MEDIACODE:
		case BO_ACQ_ORDER_DATA_ORDER:  // 2005-01-29 5163�� �߰�.
			{
				strTmpData = _T("K2_�ֹ��ڷ���");	
			}
			break;
		}
	}
	else
	{
		if( !m_bSpecies && m_nReportType == BO_ACQ_CHECKINING_DATA )
		{
			strTmpData = m_strReportFileName + _T("_��");
		}
	}
	
	CSIReportManager * pSIReportManager = m_pPrintMgr->GetSIReportManager();

	CString sValue;
	for( INT i = 0; i< m_pPrintDM->GetRowCount()-1 ; i++ )
	{
		m_pPrintDM->GetCellData(_T("IBS_ISBN"),i,sValue );
		sValue = _T("'") + sValue;
		m_pPrintDM->SetCellData(_T("IBS_ISBN"),sValue,i);
	}

	if(pSIReportManager == NULL) return -1;
	CExcelExporter ExcelExporter(this, pSIReportManager, m_pPrintDM, strTmpData);
	INT ids = ExcelExporter.InitExcelExporter();
	if(ids < 0) return ids;

	ids = ExcelExporter.ExeclExport();
	if(ids < 0) return ids;

	return 0;
}

INT CBO_ACQ_PrintDlg::CreatePrintMgr()
{
	if(m_pPrintMgr) return  0;
	else
	{
		m_pPrintMgr = new CBasicPrintMgr(this);
	}
	return 0;
}

VOID CBO_ACQ_PrintDlg::OnbtnKOLAS2FORMAT() 
{
	m_bKOLAS2Format = TRUE;
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);	
}

VOID CBO_ACQ_PrintDlg::OnchkPRINTDATASORTYN() 
{
	CString strTmp;
	m_ComboBox.GetWindowText(strTmp);
	//20070521 ADD PJW : ���������ϰ�� ������ ����
	if ( strTmp.Find(_T("��������")) >= 0 )
	{
		( (CButton*) GetDlgItem(IDC_chkPRINT_DATA_SORT_YN) )->SetCheck(1);
		m_bDataSortYN = TRUE;
		AfxMessageBox(_T("�������к� ��¹��� �⺻ �����Դϴ�."));
		return;
	}

	m_bDataSortYN = !m_bDataSortYN;


}

VOID CBO_ACQ_PrintDlg::OnbtnSETSORTCONDITION() 
{
	::PostMessage(pParentMgr->GetSafeHwnd(), SET_PRINT_DATA_ORDER_CONDITION, 0, 0);
}

HBRUSH CBO_ACQ_PrintDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
