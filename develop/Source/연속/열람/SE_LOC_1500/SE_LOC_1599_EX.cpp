// SE_LOC_1599_EX.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1599_EX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1599_EX dialog


CSE_LOC_1599_EX::CSE_LOC_1599_EX(CESL_Mgr* pParent)
	: CEquipPrintPropertyDlg(pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1599_EX)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CSE_LOC_1599_EX::DoDataExchange(CDataExchange* pDX)
{
	CEquipPrintPropertyDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1599_EX)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1599_EX, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1599_EX)
	ON_BN_CLICKED(IDC_bPRINT_EQUIP, OnbPRINTEQUIP)
	ON_BN_CLICKED(IDC_bSAVE_CONFIG, OnbSAVECONFIG)
	ON_BN_CLICKED(IDC_bFONT, OnbFONT)
	ON_CBN_SELCHANGE(IDC_cREG_NO_PLACE, OnSelchangecREGNOPLACE)
	ON_CBN_SELENDOK(IDC_CobUnit, OnSelendokCobUnit)
	ON_WM_DESTROY()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_rPRINT_LINE, IDC_rPRINT_STICKER, OnPrintFormatChange )		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1599_EX message handlers

BOOL CSE_LOC_1599_EX::OnInitDialog() 
{
	m_lpszFileName = _T("..\\CFG\\����_�ǽð�����_���_���.cfg");

	CEquipPrintPropertyDlg::OnInitDialog();

	/// ��Ʈ���� �Ⱥ��̰� �����.
	InitControl();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CSE_LOC_1599_EX::InitControl()
{
	const INT nUnVisibleCnt = 12;

	UINT nUnVisbleIDC[nUnVisibleCnt] =
	{
		IDC_rPRINT_SERIAL , IDC_rPRINT_BOOKCARD , IDC_rPRINT_STICKER , IDC_staBOOKCARD , IDC_staREG_NO , 
		IDC_cREG_CODE_CP , IDC_eREG_NO_START_CP , IDC_eREG_NO_END_CP , IDC_staSLASH , IDC_rPRINT_SERIAL ,
		IDC_CHK_SERIAL , IDC_BtnSerialLabelConfig
	};

//	for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
	for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
	{
		GetDlgItem(nUnVisbleIDC[i])->ShowWindow(SW_HIDE);
	}

	/// ��Ʈ���� �ʱ�ȭ���ش�.
	CString sText;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cREG_NO_PLACE);
	for( i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sText);
		if( sText.Compare(_T("���̺����")) == 0 )
		{
			pCombo->SetCurSel(i);
		}
	}

	((CButton*)GetDlgItem(IDC_CHK_SERIAL))->SetCheck(FALSE);

	((CComboBox*)GetDlgItem(IDC_cREG_NO_PLACE))->ShowWindow(SW_HIDE);

	return 0;
}


BOOL CSE_LOC_1599_EX::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CEquipPrintPropertyDlg::PreTranslateMessage(pMsg);
}


VOID CSE_LOC_1599_EX::OnbPRINTEQUIP() 
{
	EFS_BEGIN
/*
	CEquipPrintPropertyDlg::OnbPRINTEQUIP();
*/
	
	/// û����ȣ�� �ƴϰ� �Թ�ȣ�� ����ϱ� ������ �ٸ���.
	CPrintPropertyInfo *pInfo = NULL;
	pInfo = CEquipPrintPropertyDlg::ApplyPrintPropertyInfo();
	if (pInfo == NULL) return;
	
	this->PrintEquip(pInfo);
	
	


	EFS_END
}



VOID CSE_LOC_1599_EX::OnbSAVECONFIG() 
{
	EFS_BEGIN
		
	CEquipPrintPropertyDlg::OnbSAVECONFIG();
	
	EFS_END
}


VOID CSE_LOC_1599_EX::OnPrintFormatChange()
{
	EFS_BEGIN
	
	CEquipPrintPropertyDlg::OnPrintFormatChange();

	InitControl();
	
	EFS_END
}


VOID CSE_LOC_1599_EX::OnbFONT() 
{
	EFS_BEGIN
		
	CEquipPrintPropertyDlg::OnbFONT();
	
	EFS_END
}


VOID CSE_LOC_1599_EX::OnSelchangecREGNOPLACE() 
{
	CEquipPrintPropertyDlg::OnSelchangecREGNOPLACE();
}



VOID CSE_LOC_1599_EX::OnSelendokCobUnit() 
{
	CEquipPrintPropertyDlg::OnSelendokCobUnit();
}

VOID CSE_LOC_1599_EX::OnDestroy() 
{
	CEquipPrintPropertyDlg::OnDestroy();
}


INT CSE_LOC_1599_EX::PrintEquip(CPrintPropertyInfo *pInfo)
{
	EFS_BEGIN

	if (pInfo == NULL) return -1;

	BeginWaitCursor();

	UpdateData();

	CSIReportPrintObjectMgr PrintMgr;

	CString sUnit;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CobUnit);	
	pCombo->GetLBText( pCombo->GetCurSel() , sUnit );

	if ( sUnit == _T("�и�����(mm)") ) {
		// ���ʿ���
		PrintMgr.nStartY = pInfo->m_nMarginUp * 10/3;

		// ���ʿ���
		PrintMgr.nStartX = pInfo->m_nMarginSide * 10/3;

		// ���ΰ���
		PrintMgr.nTableHeightGap = pInfo->m_nSpaceVert * 10/3;

		// ���ΰ���
		PrintMgr.nTableWidthGap = pInfo->m_nSpaceHorz * 10/3;

		// ���̺�ʺ�
		PrintMgr.nTableWidth = pInfo->m_nLabelWidth * 10/3;	
	}
	else {
		// ���ʿ���
		PrintMgr.nStartY = pInfo->m_nMarginUp;

		// ���ʿ���
		PrintMgr.nStartX = pInfo->m_nMarginSide;

		// ���ΰ���
		PrintMgr.nTableHeightGap = pInfo->m_nSpaceVert;

		// ���ΰ���
		PrintMgr.nTableWidthGap = pInfo->m_nSpaceHorz;

		// ���̺�ʺ�
		PrintMgr.nTableWidth = pInfo->m_nLabelWidth;
	}

	// ���ڵ忩��
	PrintMgr.m_nBarcodeWidthGap  = pInfo->m_nMarginBarCode;

	// ���ΰ���
	PrintMgr.m_nRows = pInfo->m_nRowCount;
	
	// ���ΰ���
	PrintMgr.m_nCols = pInfo->m_nColCount;
	
	// ��Ʈ
	PrintMgr.SetFont(pInfo->m_nFontSize, pInfo->m_strFontFace);

	// ���� ���� / ����
	if (m_nOption_PaperDir == 1)
	{
		m_pSIReportManager->m_nDefaultPaperType = PAPER_A4_PORTRAIT;
		PrintMgr.nPaper = PAPER_A4_PORTRAIT;
	}
	else
	{
		m_pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;
		PrintMgr.nPaper = PAPER_A4_LANDSCAPE;
	}

	// ��� ���� (������ ������ ������ �׸�ŭ �Ѿ��)
	INT nStartPos = 0;
	CString strStartPos;
	m_pCM->GetControlMgrData(_T("������ġ"), strStartPos);
	
	if (!strStartPos.IsEmpty())
	{
		nStartPos = _ttoi(strStartPos);
		nStartPos--;
	}
	
	//INT ids = 0;	// ������� ���� ����(-1:��� ������ ����.)
	INT iPrintYesNo = 0;	// ������� ���� ����(-1:��� ������ ����.)

	// ���̺� - �ٹٲ����
	if (m_nOption_Print == 0)
	{
		// ��Ϲ�ȣ�� ��� ������ ����
		CString strRegNoPlace;
		m_pCM->GetControlMgrData(_T("��Ϲ�ȣ��ġ"), strRegNoPlace);
		if (strRegNoPlace == _T("����������"))
		{
			m_nMaxLine = 6;
			double dMaxLine = 6.0;			
			double dTmp;
			if ( sUnit == _T("�и�����(mm)") ) {
				dTmp = pInfo->m_nLabelHight * 10/3;
			}
			else {
				dTmp = pInfo->m_nLabelHight;
			}
			PrintMgr.nLineHeight = dTmp / dMaxLine;
			//PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
			
			//PrintMgr.nLineHeight = 13.5;

			// col ���� 1 �÷��ش�.
			PrintMgr.m_nCols++;
			iPrintYesNo = PrintEquip_LineNormal(pInfo, &PrintMgr, nStartPos);
			if ( iPrintYesNo == -1 ) {
				AfxMessageBox( _T("����� �����Ͱ� �����ϴ�.") );
				return -1;
			}
		}
		else if (strRegNoPlace == _T("���̺����"))
		{
			m_nMaxLine = 6;
			if ( sUnit == _T("�и�����(mm)") ) {
				pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
			}
			PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
			PrintEquip_LineExtra(pInfo, &PrintMgr, nStartPos);
		}
	}
	/// ��� ��¹��� �Ȱ���.
	else if (m_nOption_Print == 2)
	{
		m_nMaxLine = 4;
		if ( sUnit == _T("�и�����(mm)") ) {
			pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
		}
		PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
		CEquipPrintPropertyDlg::PrintEquip_BarCode(pInfo, &PrintMgr, nStartPos);
	}

	INT ids = m_pSIReportManager->DeleteTempPageFiles();

	EndWaitCursor();

	// ��¹� ����
	m_pSIReportManager->FreeWorkForm();
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	m_pSIReportManager->m_bPrivacyPrintLogRecord = TRUE;
	ids = PrintMgr.Print(m_pSIReportManager);

	ids = m_pSIReportManager->Print2();
	
	return 0;

	EFS_END
	return -1;
}



INT CSE_LOC_1599_EX::PrintEquip_LineNormal(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN
	INT iStartPosOri = nStartPosition;
	
	if (pPrintMgr == NULL) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nCol = 0;
	INT nLine = 0;
	BOOL bAddCard = TRUE;
	CString strItemData;
	CString strRegNo;
	CString strRegNoField;
	CSIReportPrintObject *pObject = NULL;
	INT iTotalPrintCount;


	iTotalPrintCount = m_pDM_Parent->GetRowCount();
	INT iRealPrintCount = 0;	// ���� ��� ����

	m_pGrid_Parent->SelectMakeList();

	INT nSelectedCnt = m_pGrid_Parent->SelectGetCount();

	
	INT i;

	i = m_pGrid_Parent->SelectGetHeadPosition();
	
	INT iPrintCount = 0;
	INT iStart = 0;


	while (i >= 0)
	{
		
		idx = i;
		 
		//if ( iPrintCount - pPrintMgr->m_nCols == (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {
		if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1))  == (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {
			idx = iTotalPrintCount-1;				
		}
		//if ( iPrintCount - pPrintMgr->m_nCols > (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {			
		//if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1)) > (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {			
		if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1)) == (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {			
			break;
		}

		//i = m_pGrid_Parent->SelectGetNext();
		if ( i == m_pGrid_Parent->SelectGetCount() ) {
			i = m_pGrid_Parent->SelectGetCount();
		}
		else {
			i = m_pGrid_Parent->SelectGetNext();
		}
		
		
		nLine = 0;


		if (bAddCard)
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);
			iPrintCount ++;
			if (!pObject) continue;
			nCol++;
		}

		bAddCard = TRUE;

		if (--nStartPosition >= 0) 
		{
			i = m_pGrid_Parent->SelectGetHeadPosition();
		}


		// ���� ��� ����
		iRealPrintCount++;
		// �ǽð� ���� �ڷῡ �°� ����
		// ���̺��� ��±��� üũ
		ids = m_pDM_Parent->GetCellData(_T("�Թ�ȣ"), idx, strItemData);
		pObject->SetAt(nLine++, strItemData);
		
		ids = m_pDM_Parent->GetCellData(_T("����"), idx, strItemData);
		pObject->SetAt(nLine++, strItemData);
		
		ids = m_pDM_Parent->GetCellData(_T("��ȣ��"), idx , strItemData);
		pObject->SetAt(nLine++, strItemData);
			

	}

	// ���� ���� ������ ������ ����
	if ( iRealPrintCount == 0 ) return -1;
	
	return 0;

	EFS_END
	return -1;
}

INT CSE_LOC_1599_EX::PrintEquip_LineExtra(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nLine = 0;
	BOOL bAddCard = TRUE;
	CString strItemData;
	CSIReportPrintObject *pObject = NULL;
		
	INT i = m_pGrid_Parent->SelectGetHeadPosition();
	while (i >= 0)
	{
		idx = i;
		i = m_pGrid_Parent->SelectGetNext();

		nLine = 0;

		if (bAddCard)
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);
			if (!pObject) continue;
		}

		bAddCard = TRUE;

		if (--nStartPosition >= 0) 
		{
			i = m_pGrid_Parent->SelectGetHeadPosition();
			continue;
		}

		// ���̺��� ��±��� üũ
		ids = m_pDM_Parent->GetCellData(_T("�Թ�ȣ"), idx, strItemData);
		pObject->SetAt(nLine++, strItemData);
		ids = m_pDM_Parent->GetCellData(_T("����"), idx, strItemData);
		pObject->SetAt(nLine++, strItemData);
		ids = m_pDM_Parent->GetCellData(_T("��ȣ��"), idx , strItemData);
		pObject->SetAt(nLine++, strItemData);
		

	}

	return 0;

	EFS_END
	return -1;
}
