// MarcExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcExportDlg.h"
#include <direct.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcExportDlg dialog


CMarcExportDlg::CMarcExportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcExportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;

}

CMarcExportDlg::~CMarcExportDlg()
{
}

BOOL CMarcExportDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}


VOID CMarcExportDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcExportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP


	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcExportDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcExportDlg)
	ON_BN_CLICKED(IDC_bFILEOPEN, OnbFILEOPEN)
	ON_BN_CLICKED(IDC_bMAKE_FILE, OnbMAKEFILE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcExportDlg message handlers

BOOL CMarcExportDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_����_���Ÿ�ũ����")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_����_���Ÿ�ũ����"));
	if (m_pCM == NULL) 
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CString strPath;
	GetFullPathName(_T("..\\BIN\\����_AV.TXT"), MAX_PATH, strPath.GetBuffer(MAX_PATH), NULL);


	m_pCM->SetControlMgrData(_T("���ϸ�"), strPath );

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;
}

VOID CMarcExportDlg::OnbFILEOPEN() 
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

VOID CMarcExportDlg::OnbMAKEFILE() 
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

	CESL_DataMgr *pDM = NULL;
	pDM = FindDM(_T("DM_����_���Ÿ�ũ����"));
	if ( pDM == NULL ) return ;


	if (pDM->RefreshDataManager(strOption) < 0) return;
	
	INT nRowCount = pDM->GetRowCount();
	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
		return;
	}

	// Make MarcInfo
	ids = MakeMarcInfo(pDM);
	if (ids < 0) return;

	
	//�����ڵ�����
	INT nFileCount = 1;
	INT nDataCountPerFile = nRowCount;

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
			ids = pDM->GetCellData(_T("MARC"), idxData, strMarcStream);
			if (ids < 0 || strMarcStream.IsEmpty()) continue;

			//ȫ��
			//strMarcStream += _T("\n");
			file.WriteString(strMarcStream);
		}		

		file.Close();
	}
	
	CString sMessage;
	sMessage.Format( _T("�˻��� %d�ǿ� ���� �����ۼ��� �Ϸ�Ǿ����ϴ�."), nRowCount );
	AfxMessageBox( sMessage );

EFS_END
}


INT CMarcExportDlg::MakeOptionQuery(CString &strOption)
{
EFS_BEGIN

	strOption.Empty();

	CString sACControlStart = _T("");
	CString sACControlEnd = _T("");
	
	m_pCM->GetControlMgrData( _T("���������ȣ����"), sACControlStart );
	m_pCM->GetControlMgrData( _T("���������ȣ����"), sACControlEnd );
	
	sACControlStart.TrimLeft();
	sACControlStart.TrimRight();
	sACControlEnd.TrimLeft();
	sACControlEnd.TrimRight();
	
	if ( sACControlStart == _T("") || sACControlEnd == _T("") ) {
		AfxMessageBox( _T("���������ȣ�� �Է��� �ּ���.") );
		return - 1;
	}
	if ( sACControlStart.Left(3) != _T("KAC") || sACControlEnd.Left(3) != _T("KAC") )
	{
		AfxMessageBox( _T("���������ȣ�� KAC�� �����մϴ�.") );
		return - 1;	
	}

	//�ڸ��� ���߱�
	CString sTmpChar0;
	if ( sACControlStart.GetLength() < 12 )
	{
		sTmpChar0 = _T("");
		for ( INT i = sACControlStart.GetLength(); i < 12 ; i++ )
			sTmpChar0 += _T("0");
		
		sACControlStart = sACControlStart.Left(3) + sTmpChar0 + sACControlStart.Mid(3);		
	}
	
	if ( sACControlEnd.GetLength() < 12 )
	{
		sTmpChar0 = _T("");
		for ( INT i = sACControlEnd.GetLength(); i < 12 ; i++ )
			sTmpChar0 += _T("0");
		
		sACControlEnd = sACControlEnd.Left(3) + sTmpChar0 + sACControlEnd.Mid(3);		
	}

	strOption.Format(_T("AC_CONTROL_NO >= '%s' AND AC_CONTROL_NO <= '%s'"),sACControlStart, sACControlEnd );

	return 0;

EFS_END
	return -1;
}

INT CMarcExportDlg::MakeMarcInfo(CESL_DataMgr *pDM)
{
EFS_BEGIN

	/*
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
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.09.09 ������ ���� ��
		//ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
		//ids = pDM->SetCellData(_T("MARC"), strStreamMarc, idx);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.09.09 ������ 
		//MARC ����� TAG ���� �ϱ� 
		ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc,TRUE);
		ids = pDM->SetCellData(_T("MARC"), strStreamMarc, idx);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}
	*/
	return 0;

EFS_END
	return -1;
}

VOID CMarcExportDlg::OnCancel() 
{
EFS_BEGIN
	
	CDialog::OnCancel();

EFS_END
}

HBRUSH CMarcExportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
