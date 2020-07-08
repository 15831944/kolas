// PurchaseInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PurchaseInfoDlg.h"
// 2004.09.01
#include "..\BO_DATA_CHANGE_MANAGER\EXCHANGERATE_MGR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPurchaseInfoDlg dialog
// ������
CPurchaseInfoDlg::CPurchaseInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CInfoDlg(pParent, CPurchaseInfoDlg::IDD)
{
	//==================================
	//20080616 UPDATE BY PJW : Ŭ���� ������ ����� �������� ������ ��
	m_strPriceChar = _T("");
	//==================================
	//{{AFX_DATA_INIT(CPurchaseInfoDlg)
	//m_strPriceChar = _T("");
	//}}AFX_DATA_INIT
	//==================================
	m_pCM = NULL;
	m_pCmbPriceChar	= NULL;
	m_dOrderPrice = 0.0;
	m_strDisplayOrderPrice = _T("0");
}

CPurchaseInfoDlg::~CPurchaseInfoDlg()
{
	
}

VOID CPurchaseInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPurchaseInfoDlg)
	DDX_CBString(pDX, IDC_cmbBSFORM_PRICE_CHARACTER, m_strPriceChar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPurchaseInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CPurchaseInfoDlg)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_PRICE_TYPE, OnSelchangecmbBSFORMPRICETYPE)
	ON_EN_KILLFOCUS(IDC_edtBSFORM_PRICE_DC, OnKillfocusedtBSFORMPRICEDC)
	ON_EN_KILLFOCUS(IDC_edtBSFORM_PRICE, OnKillfocusedtBSFORMPRICE)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_CURRENCY_CLASS, OnSelchangecmbBSFORMCURRENCYCLASS)
	ON_BN_CLICKED(IDC_btnEXCHANGE_RATE, OnbtnEXCHANGERATE)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_PRICE_CHARACTER, OnSelchangecmbBSFORMPRICECHARACTER)
	ON_CBN_KILLFOCUS(IDC_cmbBSFORM_PRICE_CHARACTER, OnKillfocuscmbBSFORMPRICECHARACTER)
	ON_MESSAGE(SET_TAB_ORDER, OnSetTabOrder)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurchaseInfoDlg message handlers

BOOL CPurchaseInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

// �ʱ�ȭ
BOOL CPurchaseInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// SM ����
	if( InitESL_Mgr(_T("K2UPGRADE_����_��_��������")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		

	// CM/DM ����
	m_pCM = FindCM(_T("CM_����_��_��������_UP"));	
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_����_��_��������_UP]�� ������ �� �����ϴ�."));
		return FALSE;
	}

	// ���ݼ��� �޺� ����
	m_pCmbPriceChar = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_���ݼ���"));
	if( m_pCmbPriceChar == NULL )	return FALSE;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  
}

BOOL CPurchaseInfoDlg::Init()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ���� �˻�
// [ �̴Ͻ��� ]
// 1. CM���� ���ݰ� ȭ�󱸺а��� �о� �´�.
// 2. ���ݿ� �Ҽ����� �ִ��� Ȯ���Ѵ�.
// 3. �ѱ�ȭ���ϰ�쿡�� ���ݿ��� �Ҽ����� ������� �ʰ� �Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CPurchaseInfoDlg::ValidateData()
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT nPos = 0;
	CString strPrice = _T("") ;
	CString strCurrencyCode = _T("");		
	CString strTmpData = _T("");
	

	// ���� �˻�
	//=======================================================================
	// 1. CM���� ���ݰ� ȭ�󱸺а��� �о� �´�.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("BP_����"), strPrice);
	m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strCurrencyCode,-1,0);	

	//=======================================================================
	// 2. ���ݿ� �Ҽ����� �ִ��� Ȯ���Ѵ�.
	// 3. �ѱ�ȭ���ϰ�쿡�� ���ݿ��� �Ҽ����� ������� �ʰ� �Ѵ�.
	//=======================================================================
	
	nPos = strPrice.Find(_T("."));
	// 2.
	if(nPos > 0)
	{
		// 3.
		if(strCurrencyCode == _T("\\"))
		{
			ESLAfxMessageBox(_T("�ѱ�ȭ���� ��� ���ݿ� �Ҽ����� ����� �� �����ϴ�."));
			m_pCM->SetControlMgrData(_T("BP_����"), strPrice.Left(nPos));
		}

		strTmpData = strPrice.Mid(nPos+1);
		if( strTmpData.GetLength() > 2)
		{
			ESLAfxMessageBox(_T("���������� �Ҽ������� ��°�ڸ������� �Է� �����մϴ�."));
			return -1;
		}
		strTmpData.Empty();
		strTmpData = strPrice.Left(nPos);
	}
	else strTmpData = strPrice;

	if(strTmpData.GetLength() > 12)
	{
		ESLAfxMessageBox(_T("���������� �Ҽ����� �ڷᰡ 12�ڸ� �̻��� �� �����ϴ�."));
		return -1;
	}
	//--���ݰ˻� ��.--

	// ���ְ��� �˻�
	strPrice.Empty();
	strTmpData.Empty();
	m_pCM->GetControlMgrData(_T("BP_���ְ���"), strPrice);
	nPos = strPrice.Find(_T("."));
	if(nPos > 0) strTmpData = strPrice.Left(nPos);
	else strTmpData = strPrice;

	if(strTmpData.GetLength() > 12)
	{
		ESLAfxMessageBox(_T("���������� �Ҽ����� �ڷᰡ 12�ڸ� �̻��� �� �����ϴ�."));
		return -1;
	}
	//--���ְ��ݰ˻� ��.--

	// ������ �˻�
	strPrice.Empty();
	strTmpData.Empty();
	m_pCM->GetControlMgrData(_T("BP_������"), strPrice);
	if(strPrice.IsEmpty()) return 0;
	
	nPos = strPrice.Find(_T("."));
	if(nPos > 0) 
	{
		strTmpData = strPrice.Mid(nPos+1);
		if(strTmpData.GetLength() > 2)
		{
			ESLAfxMessageBox(_T("������������ �Ҽ������� ��°�ڸ������� �Է� �����մϴ�."));
			return -1;
		}
		strTmpData.Empty();
		strTmpData = strPrice.Left(nPos);
	}
	else strTmpData = strPrice;

	if(strTmpData.GetLength() > 3)
	{
		ESLAfxMessageBox(_T("������������ �Ҽ����� �ڷᰡ 3�ڸ� �̻��� �� �����ϴ�."));
		return -1;
	}

	DOUBLE dTmpData;
	//dTmpData = (DOUBLE)atof(strPrice.GetBuffer(0));
	TCHAR* pEndPtr = NULL;
	dTmpData = _tcstod(strPrice.GetBuffer(0), &pEndPtr);
	if(dTmpData > 100.0)
	{
		ESLAfxMessageBox(_T("������ ������ 100���� Ŭ �� �����ϴ�."));
		m_pCM->SetControlMgrData(_T("BP_������"), _T("0"));
		return -1;
	}
	//--�������˻� ��.--

	
	return 0;
}

INT CPurchaseInfoDlg::ApplyData()
{
	INT ids, i, nBookCount;
	DOUBLE dPrice, dDisc, dExchange;
	CString strVolPriceSum, strPrice, strDisc, strExchange, strData, strTmpData = _T("");

	nBookCount = GetBookDM()->GetRowCount();
	for ( i=0; i<nBookCount; i++ )
	{
		// ���ݼ��� ����
		GetBookDM()->SetCellData(_T("BB_���ݼ���"), m_strPriceChar, i);
		
		// 2006.01.17 ������ - ������å���׹ݿ�, å���ְ����� �����Ѵ�.
		dPrice = dDisc = dExchange = 0.0;
		strPrice	= GetBookDM()->GetCellData(_T("BB_����"), 0);
		strDisc		= GetDM()->GetCellData(_T("BP_������"), 0);
		strExchange	= GetDM()->GetCellData(_T("BP_ȯ��"), 0);	
		//dPrice		= (DOUBLE)atof(strPrice.GetBuffer(0)	);
		TCHAR* pEndPtr = NULL;
		dPrice = _tcstod(strPrice.GetBuffer(0), &pEndPtr);

		//dExchange	= (DOUBLE)atof(strExchange.GetBuffer(0)	);
		dPrice = _tcstod(strExchange.GetBuffer(0), &pEndPtr);

		//dDisc		= (DOUBLE)atof(strDisc.GetBuffer(0)		);
		dDisc		= _tcstod(strDisc.GetBuffer(0), &pEndPtr);

		dPrice = (dPrice - ((dPrice*dDisc)/100)) * dExchange;
		if( dPrice == 0 )
		{
			GetBookDM()->SetCellData(_T("BB_���ְ���_å"), _T("0"), i);
		}
		else
		{
			// ���ε� +++++
			EDBM_BIND_DATA_TYPE* pBind = NULL;
			InitEDBMBindDataType( &pBind, 0, 1);	
			strPrice.Format(_T("%f"), dPrice);
			AddEDBMBindDataByString(pBind, STRING , _T(":PRICE"), strPrice.GetBuffer(0) );		
			GetDM()->GetOneValueQuery(_T("SELECT GET_ORDERPRICE(:PRICE, UDF_MANAGE_CODE) FROM DUAL"), strData, TRUE, pBind);
//DEL 			GetDM()->GetOneValueQuery(_T("SELECT GET_ORDERPRICE(:PRICE) FROM DUAL"), strData, TRUE, pBind);
			GetBookDM()->SetCellData(_T("BB_���ְ���_å"), strData, i);
		}
	}

	GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
	if( GetFormApi()->GetVolumeCount(GetBookDM()) == 1 )
	{
		for ( i=0; i<nBookCount; i++ )
		{
			GetBookDM()->SetCellData(_T("BB_����������"), GetDM()->GetCellData(_T("BP_����������"), 0), i);
		}
	}

	if( GetApplyPriceInfo() )
	{
		ids = ApplyCalculation();
		if( ids < 0 )
		{
			AfxMessageBox(_T("�������� ���� ����"));
			return -1;
		}
		else if( ids > 0 )
		{
			AfxMessageBox(_T("�Ǻ������� ������ �����ִ� ���� �ֽ��ϴ�."));
			return -1;
		}
	}
	//�������� ����
	GetVolPriceSum(strVolPriceSum);
	GetDM()->SetCellData(_T("BP_����"), strVolPriceSum, 0);	

	strTmpData.Format(_T("%f"), m_dOrderPrice);
	GetDM()->SetCellData(_T("BP_���ְ���"), strTmpData, 0);

	return 0;
}

INT CPurchaseInfoDlg::ApplyCalculation()
{
	INT ids = -1;
	DOUBLE dPrice = 0;
	CString strPriceType, strTemp, strValue, str950$b, strTotalPrice;	
	
	ids = GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
	if( ids < 0 )	return -1;

	ids = m_pCM->GetControlMgrData(_T("BP_�ֹ�����Ÿ��"), strPriceType);
	if( ids < 0 )	return -1;	

	ids = m_pCM->GetControlMgrData(_T("BP_����"), strTemp);
	if( ids < 0 )	return -1;

	CESL_ControlComboBox* pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_�ֹ�����Ÿ��"));	

	//dPrice = (DOUBLE)atof(strTemp.GetBuffer(0));
	TCHAR* pEndPtr = NULL;
	dPrice = _tcstod(strTemp.GetBuffer(0), &pEndPtr);
	
	// ���� == �������� ��
	if (strPriceType == _T("������") && (!GetApplyVolPriceFromSpeciesPrice() || GetFormApi()->GetVolumeCount(GetBookDM()) == 1))
	{
		// ������ 1/n���� ������ ������ �ǿ� �����Ų��.
		// 2004.09.14 nPrice->dPrice
		// 2004.12.06
		m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strTemp);
		ids = GetFormApi()->ApplySpeciesPriceToVolPrice(GetBookDM(), dPrice, strTotalPrice);
		if( ids < 0 )	return -1;
		SetApplyVolPriceFromSpeciesPrice(TRUE);
	}
	// ���� == ���� ���� ������ ���Ѱ�
	else if( strPriceType == _T("�ǰ���") )
	{
		if(GetFormApi()->GetVolumeCount(GetBookDM()) > 1)
		{
			if( GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->IsWindowEnabled() == TRUE )
			{
				if( IDNO == ESLAfxMessageBox(_T("�����Ͻ� �ǰ����� �� �ǿ� �����Ͻðڽ��ϱ�?"), MB_YESNO) )
				{
					if( pCmb )	pCmb->SetCurSel(0);
					Display(0);
					GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
					return 0;
				}
			}
		}
		// 2004.09.14  nPrice->dPrice
		// 2004.12.06
		m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strTemp);
		ids = GetFormApi()->ApplyVolPrice(GetBookDM(), dPrice, strTotalPrice, str950$b);
		if (ids < 0) return -1;
		//m_pCM->SetControlMgrData(_T("BP_����"), str950$b);

		m_pCM->SetControlMgrData(_T("BP_����"), GetBookDM()->GetCellData(_T("BB_����"), 0));

		GetDM()->SetCellData(_T("BP_����"), str950$b, 0);
		
		Display(0);
	}
	// ���ǰ�ϰ�� �ǰ����� ��� 0���� �����Ѵ�.
	// 2005-12-15 ������ �߰�	
	else if( m_strPriceChar == _T("���ǰ") )
	{
		m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strTemp);
		ids = GetFormApi()->ApplyVolPrice(GetBookDM(), dPrice, strTotalPrice, str950$b);
		if( ids < 0 )	return -1;

		m_pCM->SetControlMgrData(_T("BP_����"), GetBookDM()->GetCellData(_T("BB_����"), 0));

		GetDM()->SetCellData(_T("BP_����"), str950$b, 0);	
		
		Display(0);
	}

	// ���ְ����� ����Ѵ�.
	SetOrderPrice();

	CStringArray arrIgnoreList;	
	arrIgnoreList.Add(_T("BP_����"));
	ids = GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0, _T(""), &arrIgnoreList);
	
	return 0;
}

INT CPurchaseInfoDlg::Display(INT idx)
{
	INT nIdx;
	CString strPriceType, strExchangeRate, strCurrencyCode, strPrice;
	CESL_ControlComboBox* pCombo = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_�ֹ�����Ÿ��"));
	CESL_ControlEdit* pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_����"));
	
	GetFormApi()->GetRepresentationBook(GetBookDM(), nIdx);

	// �ܱǺ��ϰ��
	if( GetFormApi()->GetVolumeCount(GetBookDM()) == 1 )
	{
		if( nIdx < 0 )	nIdx = 0;
		GetDM()->SetCellData(_T("BP_����������"), GetBookDM()->GetCellData(_T("BB_����������"), nIdx), idx);
		
		//Ȯ�尳�� ���׿� �°� ����
		if ( GetBookDM()->GetCellData(_T("BB_����"), nIdx) != _T("0.00") &&
			 GetBookDM()->GetCellData(_T("BB_����"), nIdx) != _T("0.0") &&
			 GetBookDM()->GetCellData(_T("BB_����"), nIdx) != _T("0")		 )
		{
			GetDM()->SetCellData(_T("BP_����"), GetBookDM()->GetCellData(_T("BB_����"), nIdx), idx);
		}
		else
		{
			GetDM()->SetCellData(_T("BP_����"), _T(""), idx);
		}

		if( pEdt )	pEdt->SetReadOnly(FALSE);
		if( pCombo )	pCombo->EnableWindow(TRUE);
	}
	// �ٱǺ��ϰ��
	else 
	{
		if( pCombo )	pCombo->EnableWindow(FALSE);
		
		if( pCombo->GetCurSel() == 0 )
		{
			CString strVolPriceSum;
			GetVolPriceSum(strVolPriceSum);
			GetDM()->SetCellData(_T("BP_����"), strVolPriceSum, idx);
			
			if( pEdt )	pEdt->SetReadOnly(TRUE);
		}
		else 
		{
			CString strVolPrice;
			if( nIdx < 0 ) nIdx = 0;
			GetBookDM()->GetCellData(_T("BB_����"), nIdx, strVolPrice);
			GetDM()->SetCellData(_T("BP_����"), strVolPrice, idx);
			if( pEdt )	pEdt->SetReadOnly(FALSE);
		}
	}
	// 2005-01-26 ȯ������Ʈ�ڽ��� ȯ�� ������.
	GetDM()->GetCellData(_T("BP_ȯ��"), nIdx, strExchangeRate);
	CEdit* pEdit;
	pEdit = (CEdit*)GetDlgItem(IDC_edtEXCHANGE_RATE);
	pEdit->SetWindowText(strExchangeRate);	
	
	// CM ��ü�� �ٽ� �ѷ��ش�.
	m_pCM->AllControlDisplay(idx);

	// ���ݼ����� �ѷ��ش�.
	m_strPriceChar = GetBookDM()->GetCellData(_T("BB_���ݼ���"), nIdx);
	m_strPriceChar.TrimLeft();		m_strPriceChar.TrimRight();	
	GetDM()->SetCellData(_T("BP_���ݼ���"), m_strPriceChar, idx);
	UpdateData(FALSE);

	OnKillfocuscmbBSFORMPRICECHARACTER();
	
	// ���ְ��ݰ��
	SetOrderPrice();

	GetDlgItem(IDC_edtBSFORM_ORDER_PRICE)->EnableWindow(FALSE);

	//���ݰ˻� �ѹ� �� �Ѵ�. 
	CString strTmpData = _T("");
	strPrice = _T("");

	m_pCM->GetControlMgrData(_T("BP_����"), strPrice);
	m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strCurrencyCode,-1,0);	
	
	INT  nPos = strPrice.Find(_T("."));
	// 2.
	if(nPos > 0)
	{
		// 3.
		if(strCurrencyCode == _T("\\"))
		{
			//ESLAfxMessageBox(_T("�ѱ�ȭ���� ��� ���ݿ� �Ҽ����� ����� �� �����ϴ�."));
			m_pCM->SetControlMgrData(_T("BP_����"), strPrice.Left(nPos));
		}
	}	

	return 0;
}

VOID CPurchaseInfoDlg::OnSelchangecmbBSFORMPRICETYPE() 
{
	Display(0);
}

VOID CPurchaseInfoDlg::OnKillfocusedtBSFORMPRICEDC() 
{	
	INT ids = ValidateData();
	if(ids < 0) return ;
	SetOrderPrice();
}

INT CPurchaseInfoDlg::ClearScreen()
{
	CESL_ControlEdit * pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_����"));
	if(pEdt) pEdt->SetReadOnly(FALSE);
	return m_pCM->AllControlClear();
}

DOUBLE CPurchaseInfoDlg::OrderPrice_Calculate()
{
	DOUBLE dRate, dOrderPrice, dExchangeRate;
	CString strBookPriceSum, strDiscRate, strExchangeRate;
	
	GetBookPriceSum(strBookPriceSum);

	m_pCM->GetControlMgrData(_T("BP_������"), strDiscRate);
	if( strDiscRate.IsEmpty() == TRUE )
	{
		strDiscRate = _T("0");
		m_pCM->SetControlMgrData(_T("BP_������"), strDiscRate);
	}
	GetDM()->SetCellData(_T("BP_������"), strDiscRate, 0);
	GetDM()->GetCellData(_T("BP_ȯ��"), 0, strExchangeRate);

	//dRate			= (DOUBLE)atof(strDiscRate.GetBuffer(0)		);
	//dOrderPrice		= (DOUBLE)atof(strBookPriceSum.GetBuffer(0)	);
	//dExchangeRate	= (DOUBLE)atof(strExchangeRate.GetBuffer(0)	);
	TCHAR* pEndPtr = NULL;
	dRate = _tcstod(strDiscRate.GetBuffer(0), &pEndPtr);
	dOrderPrice = _tcstod(strBookPriceSum.GetBuffer(0), &pEndPtr);
	dExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);
	
	// ���ְ��� ��� �Ϸ�
	dOrderPrice = (dOrderPrice - ((dOrderPrice*dRate)/100)) * dExchangeRate;

	return dOrderPrice;
}

VOID CPurchaseInfoDlg::SetOrderPrice()
{
	// 2006.01.11 ������ - ���ְ����� ������� ���������� �����´�.	
	m_dOrderPrice = OrderPrice_Calculate();
	CString strOrderPrice, strBind;	
	strBind = _T("");
	m_strDisplayOrderPrice = _T("0");
	if( m_dOrderPrice > 0 )
	{
		EDBM_BIND_DATA_TYPE* pBind = NULL;	
		InitEDBMBindDataType( &pBind, 0, 1);	
		strBind.Format(_T("%f"), m_dOrderPrice );
		AddEDBMBindDataByString(pBind, STRING , _T(":PRICE"), strBind.GetBuffer(0) );
		GetDM()->GetOneValueQuery(_T("SELECT GET_ORDERPRICE(:PRICE, UDF_MANAGE_CODE) FROM DUAL"), m_strDisplayOrderPrice, TRUE, pBind);
//DEL 		GetDM()->GetOneValueQuery(_T("SELECT GET_ORDERPRICE(:PRICE) FROM DUAL"), m_strDisplayOrderPrice, TRUE, pBind);
	}
	
	m_pCM->SetControlMgrData(_T("BP_���ְ���"), m_strDisplayOrderPrice);		

	/*
	DOUBLE dOrderPrice = OrderPrice_Calculate();
	CString strIs64			= _T("");
	CString strIs90			= _T("");
	CString strOrderPrice	= _T("");
	INT nOrderPrice = 0;
	// ������ ��쿡�� ���ְ����� _T("��") ���� �ݿø� �Ѵ�.
	// 2005-09-29 ������
	//
	GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T(""), strIs64);
	GetManageValue(_T("��Ÿ"), _T("����"), _T("�����"), _T(""), strIs90);
	if( strIs64 == _T("Y") )
	{
		dOrderPrice /= 10;
		nOrderPrice = (INT)(dOrderPrice+0.5);
		nOrderPrice *= 10;
		strOrderPrice.Format(_T("%d"), nOrderPrice);
	}
	//2005-11-23 Ȳ���� : ������ �Ҽ��� 2°�ڸ� ���翩�� ��ȭ�ϰ�� 
	//�������� ǥ����
	else if( strIs90 == _T("Y") )
	{
		CString strCurrency = _T("");
		strCurrency = GetDM()->GetCellData(_T("BP_ȭ�󱸺�"), 0);
		if(strCurrency == _T("\\"))
		{
			strOrderPrice.Format(_T("%d"), (INT)dOrderPrice);
		}
		else
			strOrderPrice = TruncPrice(GetDM(), dOrderPrice);
	}
	else	// ���� �̿�~ ����
	{
		// ��� ȭ�� ���ְ��� �Ҽ��� 2�ڸ�
		// 2005-09-29 ������
		//		
		strOrderPrice = TruncPrice(GetDM(), dOrderPrice);
	}
	m_pCM->SetControlMgrData(_T("BP_���ְ���"), strOrderPrice);
	*/
}

VOID CPurchaseInfoDlg::SetDBOrderPrice()
{
	CString strOrderPrice = _T("");
	strOrderPrice.Format(_T("%f"), OrderPrice_Calculate() );
	m_pCM->SetControlMgrData(_T("BP_���ְ���"), strOrderPrice);	
}

BOOL CPurchaseInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
		else if(pMsg->wParam == VK_TAB)
		{
			if(pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_PRICE_OTHER_INFO)->GetSafeHwnd())
			{
				pParentMgr->PostMessage(SET_TAB_ORDER, 0, 1);
				return TRUE;
			}
		}
	}
	if (pMsg->message == WM_CHAR && 
		(pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_PRICE)->GetSafeHwnd() || 
		 pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_ORDER_PRICE)->GetSafeHwnd() ||
		 pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_PRICE_DC)->GetSafeHwnd()))
    {
		INT nVirtKey = (INT) pMsg->wParam;
		if ((nVirtKey < '0' || nVirtKey > '9') && nVirtKey != '.' && nVirtKey != 8)
		{
			ESLAfxMessageBox(_T("���ڸ� �Է��Ͻ� �� �ֽ��ϴ�.")); 
            return TRUE;
		}
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

INT CPurchaseInfoDlg::AllControlClear()
{
	return GetFormApi()->AllControlClear(m_pCM, NULL);
}

VOID CPurchaseInfoDlg::OnKillfocusedtBSFORMPRICE() 
{
	INT ids = ValidateData();
	if( ids < 0 ) return ;

	ApplyCalculation();
}

UINT CPurchaseInfoDlg::OnSetTabOrder(LPARAM lParam, WPARAM wParam)
{
	GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->SetFocus();
	return 0;
}


VOID CPurchaseInfoDlg::OnSelchangecmbBSFORMCURRENCYCLASS() 
{
	CString strExchangeRate, strCurrencyCode;
	CEdit* pEdit = NULL;

	// 2006-02-17 KBS - ȭ�󱸺к���� ȯ�� ����(_T("Y"))���� �ڵ����� �о���δ�.
	UpdateData(TRUE);
	m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strCurrencyCode);

	// ��������� ��� ȯ������ �Է��� �� �����Ƿ� _T("1")�� �⺻�����Ѵ�.
	if( strCurrencyCode.IsEmpty() == TRUE )	strExchangeRate = _T("1");
	else
	{
		CString query;
		query.Format(_T("SELECT exchange_rate ")
					 _T("FROM co_exchange_rate_tbl ")
					 _T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND user_code_mng_key in ")
//DEL 					 _T("WHERE user_code_mng_key in ")
					 _T("(SELECT rec_key FROM cd_code_tbl WHERE code LIKE '%s%%') ")
					 _T("AND APPLY_YN = 'Y' ")
					 _T("AND INPUT_DATE >= (SELECT TRUNC(SYSDATE, 'YEAR') FROM DUAL) ")
					 _T("AND INPUT_DATE < (SELECT ADD_MONTHS(TRUNC(SYSDATE, 'YEAR'), 12) FROM DUAL)")
					 , strCurrencyCode, strCurrencyCode	);
		GetDM()->GetOneValueQuery(query, strExchangeRate);

		// ����� ���� ���� ��쿡�� ȯ������ â�� ����.
		if( strExchangeRate.IsEmpty() )
		{
			if(IDYES == ESLAfxMessageBox(_T("������ ȭ�󱸺п� ���� ȯ�������� �����ϴ�. \nȯ�������� �����Ͻðڽ��ϱ�?"), MB_YESNO|MB_ICONQUESTION) )
			{
				CEXCHANGERATE_MGR dlg(pParentMgr, strCurrencyCode);
				if( IDOK == dlg.DoModal() )		strExchangeRate = dlg.m_exchangerate;
				else							strExchangeRate = _T("1");
			}
			else	strExchangeRate = _T("1");
		}
	}
	// ����� ȯ������ ����Ʈ�ڽ��� �����ش�.	
	pEdit = (CEdit*)GetDlgItem(IDC_edtEXCHANGE_RATE);
	if( pEdit ) 	pEdit->SetWindowText(strExchangeRate);
	// ȯ���� DM�� SetCellData ���ش�.
	GetDM()->SetCellData(_T("BP_ȯ��"), strExchangeRate, 0); // == FindDM(_T("DMUP_����_��_��������"))->SetCellData(_T("BP_ȯ��"), strExchangeRate, 0);	
	
	// KBS - ����� ȯ���� ��� �����Ͽ� ���԰����� ����Ѵ�.	
	SetOrderPrice();
	
	/*
	// 2004.09.01 ȯ������ �����ؾ��� BP_ȯ��
	CString strCurrencyCode;
	m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strCurrencyCode);
	CEXCHANGERATE_MGR dlg(pParentMgr, strCurrencyCode);
	if(IDOK == dlg.DoModal())
	{
		//ȯ���� DM�� SetCellData ���ְ� ����Ǿ��ٴ� �޽��� ������.
		FindDM(_T("DMUP_����_��_��������"))->SetCellData(_T("BP_ȯ��"), dlg.m_exchangerate, 0);

		// 2005-01-26 ����� ȯ������ ����Ʈ�ڽ��� �����ش�.
		//	m_pCM->GetControlMgrData(_T("BP_ȯ��"), strExchangeRate);
		//	GetDM()->SetCellData(_T("BP_ȯ��"), strExchangeRate, 0);

		ESLAfxMessageBox(_T("����� ȯ���� ����Ǿ����ϴ�."), MB_ICONINFORMATION);
	}
	*/

}

// 2005-01-26 ȯ�� ��ư�� ���� ȯ���� �����Ѵ�.
VOID CPurchaseInfoDlg::OnbtnEXCHANGERATE() 
{
	CString strCurrencyCode;

	m_pCM->GetControlMgrData(_T("BP_ȭ�󱸺�"), strCurrencyCode);
	CEXCHANGERATE_MGR dlg(pParentMgr, strCurrencyCode);
	if(IDOK == dlg.DoModal())
	{
		//ȯ���� DM�� SetCellData ���ְ� ����Ǿ��ٴ� �޽��� ������.
		FindDM(_T("DMUP_����_��_��������"))->SetCellData(_T("BP_ȯ��"), dlg.m_exchangerate, 0);

		// 2005-01-26 ����� ȯ������ ����Ʈ�ڽ��� �����ش�.
		CEdit * pEdit;
		pEdit = (CEdit*)GetDlgItem(IDC_edtEXCHANGE_RATE);
		pEdit->SetWindowText(dlg.m_exchangerate);

		AfxMessageBox(_T("����� ȯ���� ����Ǿ����ϴ�."), MB_ICONINFORMATION);
		
		// ����� ȯ���� ��� ����
		//
		SetOrderPrice();
	}	
}

VOID CPurchaseInfoDlg::OnSelchangecmbBSFORMPRICECHARACTER() 
{
	INT idx;
	CESL_ControlEdit* pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_����"));
	CESL_ControlMultiComboBox* pMcmb = (CESL_ControlMultiComboBox*)m_pCM->FindControl(_T("BP_ȭ�󱸺�"));
	
	if( pEdt == NULL || pMcmb == NULL ) return;
	
	idx = m_pCmbPriceChar->GetCurSel();
	if( idx < 0 )	return;

	UpdateData(TRUE);

	m_pCmbPriceChar->GetLBText(idx, m_strPriceChar);
	if( m_strPriceChar == _T("���ǰ") )	
	{
		INT nIdx;
		pEdt->SetWindowText(_T(""));
		m_pCM->SetControlMgrData(_T("BP_���ְ���"), _T(""));
		//==========================================
		//20080616 UPDATE BY PJW : SetCurSel(0)���ϸ� ��������� ������ ������ �ִ�.
		//pMcmb->SetCurSel(0);
		nIdx = pMcmb->SelectString(_T("    "));
		pMcmb->SetCurSel(nIdx);
		//==========================================
	
		pMcmb->EnableWindow(FALSE);
		pEdt->SetReadOnly(TRUE);
		GetDlgItem(IDC_edtEXCHANGE_RATE)->SetWindowText(_T(""));
		GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->EnableWindow(FALSE);	
	}
	else
	{
		pMcmb->EnableWindow(TRUE);
		// ����ó������ ���� �ڷḸ ������ �����ϴ�.
		if( GetFormApi()->GetVolumeCount(GetBookDM()) == 1 )
		{
		pEdt->SetReadOnly(FALSE);
			GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->EnableWindow(TRUE);
		}
	}

	UpdateData(FALSE);
}

VOID CPurchaseInfoDlg::OnKillfocuscmbBSFORMPRICECHARACTER() 
{
	//===================================================
	//20080620 REM BY PJW : ������ ���ؼ� �Լ��� ����.
//	INT i;
//	CString strData;		

//	UpdateData(TRUE);
//	m_strPriceChar.TrimLeft();	m_strPriceChar.TrimRight();	

//	// �޺��ȿ� ���ٸ� �߰��Ѵ�
//	{
//		for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
//		{
//			m_pCmbPriceChar->GetLBText(i, strData);
//			if( strData == m_strPriceChar )		break;
//		}
//
//		if( i >= m_pCmbPriceChar->GetCount() )
//		{
//			m_pCmbPriceChar->AddString(m_strPriceChar);
//		}
//	}
//	
//	// �߰��� �ܾ ��Ʈ�Ǿ����Ƿ� �ٽ� �״ܾ ã�� �ε����� �����Ѵ�.
//	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
//	{
//		m_pCmbPriceChar->GetLBText(i, strData);
//		if( strData == m_strPriceChar )	break;
//	}
//
//	m_pCmbPriceChar->SetCurSel(i);
	//===================================================

	//===================================================
	//20080620 ADD BY PJW : ���ݼ����� �߰� OR SEL����
	FormPriceCharacterAdd(); 
	//===================================================

	OnSelchangecmbBSFORMPRICECHARACTER();
}
//===================================================
//20080620 ADD BY PJW : ���ݼ����� �߰� OR SEL����
VOID CPurchaseInfoDlg::FormPriceCharacterAdd() 
{
	INT i;
	CString strData;		

	UpdateData(TRUE);
	m_strPriceChar.TrimLeft();	m_strPriceChar.TrimRight();	
	//================================
	//20080620 ADD BY PJW : ""�� ������ ""�� �߰����ش�.
	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
	{
		m_pCmbPriceChar->GetLBText(i, strData);
		if( strData == _T("") )		break;
	}
	if( i >= m_pCmbPriceChar->GetCount() )
	{
		m_pCmbPriceChar->AddString(_T(""));
	}
	//================================
	// �޺��ȿ� ���ٸ� �߰��Ѵ�
	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
	{
		m_pCmbPriceChar->GetLBText(i, strData);
		if( strData == m_strPriceChar )		break;
	}

	if( i >= m_pCmbPriceChar->GetCount() )
	{
		m_pCmbPriceChar->AddString(m_strPriceChar);
	}
	
	// �߰��� �ܾ ��Ʈ�Ǿ����Ƿ� �ٽ� �״ܾ ã�� �ε����� �����Ѵ�.
	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
	{
		m_pCmbPriceChar->GetLBText(i, strData);
		if( strData == m_strPriceChar )	break;
	}

	m_pCmbPriceChar->SetCurSel(i);
}
//===================================================

VOID CPurchaseInfoDlg::SetOrderPrice_InDM(CESL_DataMgr *pDM, INT index)
{
	pDM->SetCellData(_T("BP_���ְ���"), m_strDisplayOrderPrice, index);
}

HBRUSH CPurchaseInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
