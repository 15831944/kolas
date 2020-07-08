// SE_LOC_1599.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1599.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1599 dialog

#define MAX_LINE 13

CSE_LOC_1599::CSE_LOC_1599(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1599)
	m_nOption_Paper = 0;
	m_nOption_Print = 0;
	m_nOption_PaperDir = 1;
	m_strRegNoStart = _T("");
	m_strRegNoEnd = _T("");
	//}}AFX_DATA_INIT
	m_pDM_Parent = NULL;
	m_pSIReportManager = NULL;
	m_pCM = NULL;
}


VOID CSE_LOC_1599::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1599)
	DDX_Radio(pDX, IDC_rPAPER_SHEET, m_nOption_Paper);
	DDX_Radio(pDX, IDC_rPRINT_LINE, m_nOption_Print);
	DDX_Radio(pDX, IDC_rPAPER_WIDTH, m_nOption_PaperDir);
	DDX_Text(pDX, IDC_eREG_NO_START_CP, m_strRegNoStart);
	DDX_Text(pDX, IDC_eREG_NO_END_CP, m_strRegNoEnd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1599, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1599)
	ON_BN_CLICKED(IDC_bPRINT_EQUIP, OnbPRINTEQUIP)
	ON_BN_CLICKED(IDC_bPRINT_EQUIP2, OnbPRINTEQUIP2)
	ON_BN_CLICKED(IDC_bSAVE_CONFIG, OnbSAVECONFIG)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bFONT, OnbFONT)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_rPRINT_LINE, IDC_rPRINT_STICKER, OnPrintFormatChange )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1599 message handlers



INT CSE_LOC_1599::Init()
{
	EFS_BEGIN

	if (!m_pDM_Parent) return -1;

	if (m_pSIReportManager)
		delete m_pSIReportManager;
	m_pSIReportManager = new CSIReportManager(this);

	FreePrintPropertyInfo();

	m_pCM = FindCM(_T("CM_����_���_���_�Ӽ�"));
	if (!m_pCM) return -1;

	m_lpszFileName = _T("..\\CFG\\����_���_���.cfg");

	INT ids = -1;

	// ���������� �о�´�.
	ids = LoadPrintPropertyInfo();
	if (ids < 0) return ids;

	// �������� ��������
/*
	CString strLibName;
	m_pDM_Parent->GetOneValueQuery(_T("SELECT VALUE FROM MN_LIBINFO_TBL WHERE VALUE_NAME ='��������'"), strLibName);
*/
	CString sFieldName[1] = { _T("LIB_NAME") };
	CString strLibName[1];
	CLocCommonAPI::GetLibInfo( sFieldName , strLibName , 1 , this );

	m_pCM->SetControlMgrData(_T("��������"), strLibName[0]);


	OnPrintFormatChange();

	return 0;

	EFS_END
	return -1;
}

INT CSE_LOC_1599::FreePrintPropertyInfo()
{
	EFS_BEGIN

	CString strKey;
	CPrintPropertyInfo *pInfo = NULL;
	POSITION pos = m_mapPrintPropertyInfo.GetStartPosition();
	while (pos)
	{
		m_mapPrintPropertyInfo.GetNextAssoc(pos, strKey, (VOID*&)pInfo);
		if (!pInfo) continue;

		delete pInfo;
	}

	m_mapPrintPropertyInfo.RemoveAll();

	return 0;

	EFS_END
	return -1;
}

INT CSE_LOC_1599::LoadPrintPropertyInfo()
{
	EFS_BEGIN

	CStdioFile file;
	if (!file.Open(m_lpszFileName, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(m_lpszFileName, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary))
			return -1;
		else
		{
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
		}
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CPrintPropertyInfo *pInfo = NULL;

	CString strKey;
	CString strLine;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty() || strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

		
		pInfo = new CPrintPropertyInfo;
		_stscanf(strLine, _T("%s\t=\t%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%s"),
			strKey.GetBuffer(MAX_PATH), 
			&pInfo->m_nMarginUp,
			&pInfo->m_nMarginSide,
			&pInfo->m_nSpaceVert,
			&pInfo->m_nSpaceHorz,
			&pInfo->m_nLabelHight,
			&pInfo->m_nLabelWidth,
			&pInfo->m_nWidthCount,
			&pInfo->m_nLengthCount,
			&pInfo->m_nMarginBarCode,
			&pInfo->m_nFontHeight,
			&pInfo->m_nFontSize,
			pInfo->m_strFontFace.GetBuffer(MAX_PATH)
			);
		strKey.ReleaseBuffer();
		pInfo->m_strFontFace.ReleaseBuffer();
		m_mapPrintPropertyInfo.SetAt(strKey, pInfo);
	}

	file.Close();

	return 0;

	EFS_END
	return -1;
}

INT CSE_LOC_1599::SavePrintPropertyInfo()
{
	EFS_BEGIN

	CStdioFile file;
	if (!file.Open(m_lpszFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		m_strLastErrMsg.Format(_T("[%s] ������ �� �� �����ϴ�."), m_lpszFileName);
		return -1;
	}
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	CString strKey;
	CString strLine;
	CPrintPropertyInfo *pInfo = NULL;
	POSITION pos = m_mapPrintPropertyInfo.GetStartPosition();
	while (pos)
	{
		m_mapPrintPropertyInfo.GetNextAssoc(pos, strKey, (VOID*&)pInfo);
		if (!pInfo) continue;

		strLine.Format(_T("%s\t=\t%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%s\r\n"),
			strKey, 
			pInfo->m_nMarginUp,
			pInfo->m_nMarginSide,
			pInfo->m_nSpaceVert,
			pInfo->m_nSpaceHorz,
			pInfo->m_nLabelHight,
			pInfo->m_nLabelWidth,
			pInfo->m_nWidthCount,
			pInfo->m_nLengthCount,
			pInfo->m_nMarginBarCode,
			pInfo->m_nFontHeight,
			pInfo->m_nFontSize,
			pInfo->m_strFontFace
			);
		
		file.WriteString(strLine);
	}

	file.Close();
	return 0;

	EFS_END
	return -1;
}

INT CSE_LOC_1599::DisplayPrintPropertyInfo()
{
	EFS_BEGIN

	CPrintPropertyInfo *pInfo = NULL;

	// ã�Ƽ� ������ �����Ѵ�.
	if (!m_mapPrintPropertyInfo.Lookup(m_strPrintFormatKey, (VOID*&)pInfo))
	{
		pInfo = new CPrintPropertyInfo;
		m_mapPrintPropertyInfo.SetAt(m_strPrintFormatKey, pInfo);
	}

	if (!pInfo) return -1;

	// ������Ƽ�� �����ͼ� �����ش�.
	CString strData;
	
	strData.Format(_T("%d"), pInfo->m_nMarginUp);
	m_pCM->SetControlMgrData(_T("���ʿ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nMarginSide);
	m_pCM->SetControlMgrData(_T("���ʿ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nSpaceVert);
	m_pCM->SetControlMgrData(_T("���ΰ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nSpaceHorz);
	m_pCM->SetControlMgrData(_T("���ΰ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nLabelHight);
	m_pCM->SetControlMgrData(_T("���̺����"), strData);

	strData.Format(_T("%d"), pInfo->m_nLabelWidth);
	m_pCM->SetControlMgrData(_T("���̺�ʺ�"), strData);

	strData.Format(_T("%d"), pInfo->m_nWidthCount);
	m_pCM->SetControlMgrData(_T("���ΰ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nLengthCount);
	m_pCM->SetControlMgrData(_T("���ΰ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nMarginBarCode);
	m_pCM->SetControlMgrData(_T("���ڵ忩��"), strData);

	// ��Ʈ
	CString strFont;
	strFont.Format(_T("%s, %d"), pInfo->m_strFontFace, pInfo->m_nFontSize);
	GetDlgItem(IDC_bFONT)->SetWindowText(strFont);	

	return 0;

	EFS_END
	return -1;
}

CPrintPropertyInfo *CSE_LOC_1599::ApplyPrintPropertyInfo()
{
	EFS_BEGIN

	UpdateData();
	
	CPrintPropertyInfo *pInfo = NULL;

	// ã�Ƽ� ������ �����Ѵ�.
	if (!m_mapPrintPropertyInfo.Lookup(m_strPrintFormatKey, (VOID*&)pInfo))
	{
		pInfo = new CPrintPropertyInfo;
		m_mapPrintPropertyInfo.SetAt(m_strPrintFormatKey, pInfo);
	}

	// ���� ������ �����Ѵ�.
	CString strData;

	m_pCM->GetControlMgrData(_T("���ʿ���"), strData);
	pInfo->m_nMarginUp = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("���ʿ���"), strData);
	pInfo->m_nMarginSide = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("���ΰ���"), strData);
	pInfo->m_nSpaceVert = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("���ΰ���"), strData);
	pInfo->m_nSpaceHorz = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("���̺����"), strData);
	pInfo->m_nLabelHight = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("���̺�ʺ�"), strData);
	pInfo->m_nLabelWidth = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("���ΰ���"), strData);
	pInfo->m_nWidthCount = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("���ΰ���"), strData);
	pInfo->m_nLengthCount = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("���ڵ忩��"), strData);
	pInfo->m_nMarginBarCode = _ttoi(strData);

	return pInfo;

	EFS_END
	return NULL;
}

INT CSE_LOC_1599::PrintEquip(CPrintPropertyInfo *pInfo)
{
	EFS_BEGIN

	if (pInfo == NULL) return -1;

	UpdateData();

	CSIReportPrintObjectMgr PrintMgr;

	// ���ʿ���
	PrintMgr.nStartX = pInfo->m_nMarginUp;

	// ���ʿ���
	PrintMgr.nStartY = pInfo->m_nMarginSide;

	// ���ΰ���
	PrintMgr.nTableHeightGap = pInfo->m_nSpaceVert;

	// ���ΰ���
	PrintMgr.nTableWidthGap = pInfo->m_nSpaceHorz;

	// ���̺����
	
	// ���ڵ� ���̰� �ʹ� ª�ٰ� ���ڵ� ���̸� ���� �� �� �ֵ��� ���谡 ��������!
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eBARCODE_HEIGHT );
	CString barcodeHeight;
	pEdt->GetWindowText( barcodeHeight );
	//pInfo->m_nLabelHight += 100;
	pInfo->m_nLabelHight += _ttoi( barcodeHeight );	
	
	PrintMgr.nLineHeight = pInfo->m_nLabelHight / MAX_LINE;
	
	// ���̺�ʺ�
	PrintMgr.nTableWidth = pInfo->m_nLabelWidth;

	// ���ΰ���
	PrintMgr.m_nRows = pInfo->m_nWidthCount;
	
	// ���ΰ���
	PrintMgr.m_nCols = pInfo->m_nLengthCount;

	// ���ڵ忩��
	PrintMgr.m_nBarcodeWidthGap  = pInfo->m_nMarginBarCode;

	// ��Ʈ
	PrintMgr.SetFont(pInfo->m_nFontSize, pInfo->m_strFontFace);

	// ���� ���� / ����
	if (m_nOption_PaperDir == 0)
	{
		m_pSIReportManager->m_nDefaultPaperType = PAPER_A4_PORTRAIT;
		PrintMgr.nPaper = PAPER_A4_PORTRAIT;
	}
	else
	{
		m_pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;
		PrintMgr.nPaper = PAPER_A4_LANDSCAPE;
	}

	// ��� ���� (������ ������ ������ �׸�ŭ �Ѿ��
	INT nStartPos = 0;
	CString strStartPos;
	m_pCM->GetControlMgrData(_T("������ġ"), strStartPos);
	
	if (!strStartPos.IsEmpty())
		nStartPos = _ttoi(strStartPos);
	nStartPos--;

	// ���̺� - �ٹٲ����
	if (m_nOption_Print == 0)
		PrintEquip_Line(&PrintMgr, nStartPos);
	// �̾����
	else if (m_nOption_Print == 1)
		PrintEquip_Serial(&PrintMgr, nStartPos);
	// ���ڵ�
	else if (m_nOption_Print == 2)
		PrintEquip_BarCode(&PrintMgr, nStartPos);
	
	INT ids = m_pSIReportManager->DeleteTempPageFiles();

	// ��¹� ����
	m_pSIReportManager->FreeWorkForm();
	ids = PrintMgr.Print(m_pSIReportManager);

	ids = m_pSIReportManager->Print2();
	
	return 0;

	EFS_END
	return -1;
}


INT CSE_LOC_1599::PrintEquip_Line(CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;
	// �ǽð� �谡 �ڷ��� û����ȣ�� �ٹٲ�����Ѵ�.
	
	// �Թ�ȣ + CRLF
	// ���� + CRLF
	// ��ȣ�� + CRLF

	INT ids = -1;
	INT nLine = 0;
	CString strItemData;
	CSIReportPrintObject *pObject = NULL;

	
	CESL_Grid* pGrid = GetParentGridPtr();
	if( pGrid == NULL ) return -2;

	pGrid->SelectMakeList();

	/// nMode = 0 -> �׸��忡 ���õ� ���� �����Ѵ�. , nMode = 1 -> ��ü�� �� ����Ѵ�.
	INT nMode = 0;
	INT nRowCount = pGrid->SelectGetCount();
	if( nRowCount == 0 )
	{
		nMode = 1;
		nRowCount = m_pDM_Parent->GetRowCount();
	}
	else
		pGrid->SelectGetHeadPosition();

	INT nIndex;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if( nMode == 0 )
			nIndex = pGrid->SelectGetIdx();
		else
			nIndex = idx;


		nLine = 0;

		pObject = pPrintMgr->AddCard(MAX_LINE);
		if (!pObject) continue;

		if (--nStartPosition >= 0) 
		{
			idx--;
			continue;
		}

		// ���̺��� ��±��� üũ
		ids = m_pDM_Parent->GetCellData(_T("�Թ�ȣ"), nIndex, strItemData);
		pObject->SetAt(nLine++, strItemData);
		
		ids = m_pDM_Parent->GetCellData(_T("����"), nIndex, strItemData);
		pObject->SetAt(nLine++, strItemData);

		ids = m_pDM_Parent->GetCellData(_T("��ȣ��"), nIndex , strItemData);
		pObject->SetAt(nLine++, strItemData);

		if( nMode == 0 )
			pGrid->SelectGetNext();
	}

	return 0;

	EFS_END
	return -1;
}

INT CSE_LOC_1599::PrintEquip_Serial(CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;
	// û����ȣ�� �̾ ��´�.
	// ��ġ��ȣ�ڵ� �з���ȣ-������ȣ-��å��ȣ=������ȣ

	INT ids = -1;
	CString strItemData;
	CString strRegNo;
	CString strCallNo;
	CSIReportPrintObject *pObject = NULL;
	INT nRowCount = m_pDM_Parent->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pObject = pPrintMgr->AddCard(MAX_LINE);
		if (!pObject) continue;

		if (--nStartPosition >= 0) 
		{
			idx--;
			continue;
		}

		// ���̺��� ��±��� üũ
		if (m_pDM_Parent->GetCellData(_T("���̺���±���"), idx) != _T("2")) continue;

		ids = m_pDM_Parent->GetCellData(_T("��Ϲ�ȣ"), idx, strRegNo);

		ids = m_pDM_Parent->GetCellData(_T("û����ȣ"), idx, strCallNo);
		
		strItemData.Format(_T("%-50s   %s"), strCallNo, strRegNo);
		
		pObject->SetAt(0, strItemData);
	}


	return 0;

	EFS_END
	return -1;
}



/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1599 message handlers

BOOL CSE_LOC_1599::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eBARCODE_HEIGHT );
	pEdt->SetWindowText( _T("100") );
	
	
	if (InitESL_Mgr(_T("SM_����_���_���_�Ӽ�")) < 0) 
	{
		m_strLastErrMsg = _T("Init ESL Mgr Error!!");
		goto ERR;
	}

	if (Init() < 0) goto ERR;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CSE_LOC_1599::OnPrintFormatChange()
{
	EFS_BEGIN

	UpdateData();

	// ���ڵ� ���� ��Ʈ�� �����ֱ� / �����
	CWnd *pWndTBarCode = GetDlgItem(IDC_tMARGIN_BARCODE);
	CWnd *pWndEBarCode = GetDlgItem(IDC_eMARGIN_BARCODE);

	CEdit *pWndELibName = (CEdit*)GetDlgItem(IDC_eLIB_NAME_CP);

	pWndTBarCode->ShowWindow(SW_HIDE);
	pWndEBarCode->ShowWindow(SW_HIDE);
//	pWndELibName->SetReadOnly(TRUE);

	// ���̺� - �ٹٲ����
	if (m_nOption_Print == 0)
	{
		m_strPrintFormatKey = _T("LABEL_LINE");
	}
	// ���̺� - �̾����
	else if (m_nOption_Print == 1)
	{
		m_strPrintFormatKey = _T("LABEL_SERIAL");
	}

	DisplayPrintPropertyInfo();

	EFS_END
}

VOID CSE_LOC_1599::OnbPRINTEQUIP() 
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_SE_LOC_1500") );
	if( !pDM->GetRowCount() )
	{
		MessageBox( _T("����� ����� �������� �ʽ��ϴ�."), _T("KOLAS II Upgrade"), MB_OK );
		return;
	}

	CPrintPropertyInfo *pInfo = NULL;
	pInfo = ApplyPrintPropertyInfo();
	if (pInfo == NULL) return;
	
	PrintEquip(pInfo);

	EFS_END
}

VOID CSE_LOC_1599::OnbSAVECONFIG() 
{
	EFS_BEGIN

	ApplyPrintPropertyInfo();
	SavePrintPropertyInfo();

	EFS_END
}

VOID CSE_LOC_1599::OnbFONT() 
{
	EFS_BEGIN

	CPrintPropertyInfo *pInfo = NULL;
	pInfo = ApplyPrintPropertyInfo();
	if (pInfo == NULL) return;

	LOGFONT logFont;
	ZeroMemory(&logFont,sizeof(logFont));
	logFont.lfCharSet = DEFAULT_CHARSET;
	_tcscpy(logFont.lfFaceName, pInfo->m_strFontFace);
	logFont.lfHeight = pInfo->m_nFontHeight;

	CFontDialog FontDialog(&logFont);
	if (FontDialog.DoModal() != IDOK)
		return;

	// ������ ���� �����Ѵ�.

	FontDialog.GetCurrentFont(&logFont);
	pInfo->m_nFontSize = FontDialog.GetSize() / 10;
	pInfo->m_nFontHeight = logFont.lfHeight;
	pInfo->m_strFontFace = logFont.lfFaceName;
	

	DisplayPrintPropertyInfo();

	EFS_END
}

VOID CSE_LOC_1599::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();

	delete m_pSIReportManager;
	m_pSIReportManager = NULL;

	FreePrintPropertyInfo();	

	EFS_END
}


VOID CSE_LOC_1599::OnbPRINTEQUIP2() 
{
	OnCancel();	
}



INT CSE_LOC_1599::PrintEquip_BarCode(CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN
	
	if (pPrintMgr == NULL) return -1;
	
	pPrintMgr->MODE = _T("���ڵ�");
	
	pPrintMgr->m_nBarcodeHeight = pPrintMgr->nLineHeight * 2;
	
	INT ids = -1;

	CString strLibName;
	CString strRegNo;
	CString strCallNo;

	m_pCM->GetControlMgrData(_T("��������"), strLibName);
	
	CSIReportPrintObject *pObject = NULL;

	CESL_Grid* pGrid = GetParentGridPtr();
	if( pGrid == NULL ) return -2;
	
	pGrid->SelectMakeList();
	
	/// nMode = 0 -> �׸��忡 ���õ� ���� �����Ѵ�. , nMode = 1 -> ��ü�� �� ����Ѵ�.
	INT nMode = 0;
	INT nRowCount = pGrid->SelectGetCount();
	if( nRowCount == 0 )
	{
		nMode = 1;
		nRowCount = m_pDM_Parent->GetRowCount();
	}
	else
		pGrid->SelectGetHeadPosition();
	
	INT nIndex;
	
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pObject = pPrintMgr->AddCard(MAX_LINE);
		if (!pObject) continue;
		
		if( nMode == 0 )
			nIndex = pGrid->SelectGetIdx();
		else
			nIndex = idx;

		if (--nStartPosition >= 0) 
		{
			idx--;
			continue;
		}
		
		ids = m_pDM_Parent->GetCellData(_T("����Ϲ�ȣ"), nIndex, strRegNo);
		
		pObject->SetAt(0, strLibName);
		pObject->SetAt(1, strRegNo);
		pObject->SetAt(2, strRegNo);

		if( nMode == 0 )
			pGrid->SelectGetNext();
		
	}
	
	return 0;
	
	EFS_END
	return -1;
}




CESL_Grid* CSE_LOC_1599::GetParentGridPtr()
{
	EFS_BEGIN

	CESL_ControlMgr* pCM = FindCM( _T("CM_SE_LOC_1500") );
	if( pCM == NULL ) return NULL;

	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl(_T("���α׸���"));

	return pGrid;

	EFS_END

	return NULL;
}
