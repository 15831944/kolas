// MarcExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcExportDlg_SE.h"
#include <direct.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\..\..\����\MarcAdjust\MarcAdjustConstant.h"

/////////////////////////////////////////////////////////////////////////////
// CMarcExportDlg_SE dialog


CMarcExportDlg_SE::CMarcExportDlg_SE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcExportDlg_SE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;

}

CMarcExportDlg_SE::~CMarcExportDlg_SE()
{
}

BOOL CMarcExportDlg_SE::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}


VOID CMarcExportDlg_SE::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcExportDlg_SE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP


	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcExportDlg_SE, CDialog)
	//{{AFX_MSG_MAP(CMarcExportDlg_SE)
	ON_BN_CLICKED(IDC_bFILEOPEN, OnbFILEOPEN)
	ON_BN_CLICKED(IDC_bMAKE_FILE, OnbMAKEFILE)
	ON_BN_CLICKED(IDC_chkDivision, OnchkDivision)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcExportDlg_SE message handlers

BOOL CMarcExportDlg_SE::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_����_��ũ����")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_����_��ũ����"));
	if (m_pCM == NULL) 
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// ����
	m_pDM = FindDM(_T("DM_����_��ũ����_����"));
	
	static_cast<CButton*>(GetDlgItem(IDC_chkMarcInfo))->SetCheck(1);
	static_cast<CButton*>(GetDlgItem(IDC_rGeneralMarc))->SetCheck(1);

	CTime t_start = CTime::GetCurrentTime();
	CString strPath, filename;

	filename.Format(_T("..\\BIN\\(����)���⸶ũ_%04d%02d%02d.TXT"), t_start.GetYear(), t_start.GetMonth(), t_start.GetDay());

	GetFullPathName( filename , MAX_PATH, strPath.GetBuffer(MAX_PATH), NULL);


	m_pCM->SetControlMgrData(_T("���ϸ�"), strPath );

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;
}

VOID CMarcExportDlg_SE::OnbFILEOPEN() 
{
EFS_BEGIN


	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	OFN.lpstrFilter = _T("TXT����(*.txt)\0*.txt\0�������(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TEXT ���� ����");
	OFN.lpstrDefExt = _T("TXT");
	
	BOOL bSuccess = GetSaveFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
		m_pCM->SetControlMgrData(_T("���ϸ�"), OFN.lpstrFile);


EFS_END
}

VOID CMarcExportDlg_SE::OnbMAKEFILE() 
{
EFS_BEGIN

	CString strFileName;
	m_pCM->GetControlMgrData( _T("���ϸ�"), strFileName );
	if ( strFileName.IsEmpty() ) 
	{
		AfxMessageBox( _T("���ϸ��� �Է��ؾ� �մϴ�.") );
		return;
	}

	if ( IDNO == AfxMessageBox( _T("������ �ۼ��Ͻðڽ��ϱ�?") , MB_YESNO) ) return;

	// ��θ� ����(-_-) ����
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	
	_tsplitpath(strFileName, drive, dir, fname, ext);

	// �ڷ� �˻�
	CString strOption;
	INT ids = MakeOptionQuery(strOption);
	if (ids < 0) return;

	if (m_pDM->RefreshDataManager(strOption) < 0) return;
	
	INT nRowCount = m_pDM->GetRowCount();
	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
		return;
	}

	// Make MarcInfo
	ids = MakeMarcInfo(m_pDM);
	if (ids < 0) return;

	//�����ڵ�����
	BOOL bDivisionFile = FALSE;
	if (static_cast<CButton*>(GetDlgItem(IDC_chkDivision))->GetCheck() == 1 )  bDivisionFile = TRUE;

	// ���� �ڵ������϶��� ���ϴ� �Ǽ��� ����Ѵ�.
	INT nDataCountPerFile = 0;
	INT nFileCount = 0;
	if (bDivisionFile)
	{
		CString strDataCount;
		m_pCM->GetControlMgrData(_T("�Ǽ�"), strDataCount);
		nDataCountPerFile = _ttoi(strDataCount);
		if (nDataCountPerFile == 0) 
		{
			AfxMessageBox(_T("�Ǽ��� �������ּ���"));
			return;
		}

		nFileCount = static_cast<INT>(ceil(static_cast<DOUBLE>(nRowCount) / static_cast<DOUBLE>(nDataCountPerFile)));
	}
	else
	{
		nFileCount = 1;
		nDataCountPerFile = nRowCount;
	}
	
	CMarc marc;
	CString strMarcStream;
	CString strExpFileName;

	INT idxData = 0;
	INT nDataCount = 0;
	CStdioFile file;
	for (INT idxFile = 0; idxFile < nFileCount; idxFile++)
	{
		if (nFileCount > 1)
			strExpFileName.Format(_T("%s%s%s_%05d%s"), drive, dir, fname, idxFile, ext);
		else
			strExpFileName = strFileName;

		if (!file.Open(strExpFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) 
		{
			AfxMessageBox(_T("���� ������ �����߽��ϴ�."));
			return;
		}

		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		
		if (nFileCount > 1)
			idxData = idxFile * nDataCountPerFile;

		nDataCount = idxData + nDataCountPerFile;
		for (; idxData < nDataCount; idxData++)
		{
			if (idxData > nRowCount - 1) break;
			ids = m_pDM->GetCellData(_T("MARC"), idxData, strMarcStream);
			if (ids < 0 || strMarcStream.IsEmpty()) continue;

			strMarcStream += _T("\r\n");
			file.WriteString(strMarcStream);
		}		

		file.Close();
	}

	CString sMessage;
	sMessage.Format( _T("�˻��� %d�ǿ� ���� �����ۼ��� �Ϸ�Ǿ����ϴ�."), nRowCount );
	AfxMessageBox( sMessage );

EFS_END
}


VOID CMarcExportDlg_SE::OnchkDivision() 
{
EFS_BEGIN

	CButton * pDivision;
	pDivision = (CButton *)GetDlgItem(IDC_chkDivision);	

	CButton * pDataCount;
	pDataCount = (CButton *)GetDlgItem(IDC_cDataCount);	
	if ( pDivision->GetCheck() == 1 ) {
		pDataCount->EnableWindow(TRUE);
	}
	else {
		pDataCount->EnableWindow(FALSE);
	}

EFS_END
}


INT CMarcExportDlg_SE::MakeOptionQuery(CString &strOption)
{
EFS_BEGIN

	strOption.Empty();

	CString sRegCode;
	m_pCM->GetControlMgrData( _T("��ϱ���"), sRegCode );
	sRegCode.Replace( _T(" "), NULL );
	if ( sRegCode == _T("") ) {
		AfxMessageBox( _T("��ϱ����� �����ϼ���.") );
		return - 1;
	}

	CString sNoStart, sNoEnd;	
	m_pCM->GetControlMgrData( _T("��Ϲ�ȣ����"), sNoStart );
	m_pCM->GetControlMgrData( _T("��Ϲ�ȣ����"), sNoEnd );

	if ( sNoStart == _T("") || sNoEnd == _T("") ) {
		AfxMessageBox( _T("��Ϲ�ȣ�� �Է��ϼ���.") );
		return -1;
	}

	//��Ϲ�ȣ �����	
	CString sRegNoStart, sRegNoEnd;
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	sRegNoStart.Format( _T("%s%010s"), sRegCode, sNoStart );
// 	sRegNoEnd.Format( _T("%s%010s"), sRegCode, sNoEnd );
	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	sRegNoStart = m_pInfo->MakeRegNo(sRegCode, sNoStart );
	sRegNoEnd = m_pInfo->MakeRegNo(sRegCode, sNoEnd );
	//=====================================================
	

	CButton * pStartReg;
	pStartReg=(CButton *)GetDlgItem(IDC_chCLASS1);	

	//�ڷ��
	CString sShelfCode;
	m_pCM->GetControlMgrData( _T("�ڷ�Ǳ���"), sShelfCode );
	sShelfCode.Replace( _T(" "), NULL );


	//ù��Ϲ�ȣ üũ
	if ( pStartReg->GetCheck() == 1 ) {		
		strOption.Format( _T("( se_get_min_reg_no(s.rec_key) between  '%s' AND '%s')  "), sRegNoStart, sRegNoEnd, sRegNoStart, sRegNoEnd);
		if ( sShelfCode != _T("") ) {
			strOption = strOption + _T(" AND S.REC_KEY IN ( SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE SHELF_LOC_CODE ='") + sShelfCode + _T("' GROUP BY SPECIES_KEY)");
		}
	}
	else {
		strOption.Format( _T("S.REC_KEY IN (SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <= '%s' GROUP BY SPECIES_KEY)"), sRegNoStart, sRegNoEnd );
		if ( sShelfCode != _T("") ) {
			strOption.Format( _T("S.REC_KEY IN (SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <= '%s' AND SHELF_LOC_CODE ='%s' GROUP BY SPECIES_KEY)"), sRegNoStart, sRegNoEnd, sShelfCode );
		}
	}

	return 0;

EFS_END
	return -1;
}

INT CMarcExportDlg_SE::MakeMarcInfo(CESL_DataMgr *pDM)
{
EFS_BEGIN

	if (pDM == NULL) return -1;
	INT ids = -1;

	// �ڰ� ����
	BOOL bMarcInfo = FALSE;
	if (static_cast<CButton*>(GetDlgItem(IDC_chkMarcInfo))->GetCheck() == 1 )  bMarcInfo = TRUE;
	
	//++ 040217_HSS ���Ǻ����� ��Ϲ�ȣ ���� ++++++++++++++++
	CButton * pDelRegNo;
	pDelRegNo = (CButton *)GetDlgItem(IDC_chCLASS2);

	// ���� select
	CString sRegCode;
	m_pCM->GetControlMgrData( _T("��ϱ���"), sRegCode );
	sRegCode.Replace( _T(" "), NULL );

	CString sNoStart, sNoEnd;	
	m_pCM->GetControlMgrData( _T("��Ϲ�ȣ����"), sNoStart );
	m_pCM->GetControlMgrData( _T("��Ϲ�ȣ����"), sNoEnd );
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++

	CMarc marc;
	POSITION pos = NULL;
	CTag *pTag = NULL;
	CString strHoldInfo;
	CString strStreamMarc;
	INT nRowCount = pDM->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		ids = pDM->GetCellData(_T("BS_MARC"), idx, strStreamMarc);
		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) continue;
		ids = pDM->GetCellData(_T("SH_��������"), idx, strHoldInfo);
		if (ids >= 0 && !strHoldInfo.IsEmpty())
		{
			CMarc marcHold;
			ids = m_pInfo->pMarcMgr->Decoding(strHoldInfo, &marcHold);
			if (ids >= 0)
			{
				pos = marcHold.m_listTag.GetHeadPosition();
				while (pos)
				{
					pTag = (CTag*)marcHold.m_listTag.GetNext(pos);
					if (!pTag) continue;
					marc.AddTag(pTag->Clone());
				}
			}
		}

		if (!bMarcInfo)
		{
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("090"));
		}

		//++ 040217_HSS ���Ǻ����� ��Ϲ�ȣ ���� ++++++++++++++++
		if ( pDelRegNo->GetCheck() == 1 ) {
			CString strRegNo, strRegCode;	// ��Ϲ�ȣ,��ϱ���
			INT nRegNum;					// ��Ϲ�ȣ ����
			CArray<CString, CString&> p049lArray;
			m_pInfo->pMarcMgr->GetItem( &marc, _T("049$l"), strRegNo, &p049lArray );
			for ( INT j=0 ; j < p049lArray.GetSize() ; j++ ) {
				strRegNo = p049lArray.GetAt(j);
				strRegCode = strRegNo.Mid(0, strRegNo.GetLength()-10);
				nRegNum = _ttoi(strRegNo.Mid(strRegNo.GetLength()-10, 10));

				// ��ϱ����� �ٸ��ų� ��ȣ�� ���� ���
				if (sRegCode.Compare(strRegCode) 
					|| nRegNum < _ttoi(sNoStart)
					|| nRegNum > _ttoi(sNoEnd) )
				{
					m_pInfo->pMarcMgr->DeleteItem( &marc, _T("049$l"), strRegNo );
				}
			}
		}
		ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc,TRUE);
		ids = pDM->SetCellData(_T("MARC"), strStreamMarc, idx);
	}

	return 0;

EFS_END
	return -1;
}

HBRUSH CMarcExportDlg_SE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
