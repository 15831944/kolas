// BO_CAT_REGNO_0320_POPUP.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_REGNO_0320_POPUP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REGNO_0320_POPUP dialog


CBO_CAT_REGNO_0320_POPUP::CBO_CAT_REGNO_0320_POPUP(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REGNO_0320_POPUP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBO_CAT_REGNO_0320_POPUP::~CBO_CAT_REGNO_0320_POPUP()
{

}

VOID CBO_CAT_REGNO_0320_POPUP::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REGNO_0320_POPUP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REGNO_0320_POPUP, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REGNO_0320_POPUP)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDCANCEL0302, OnCancel0302)
	ON_BN_CLICKED(IDC_bTOFILE, OnbRegNoTOFILE)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REGNO_0320_POPUP message handlers

BOOL CBO_CAT_REGNO_0320_POPUP::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_REGNO_0320_POPUP::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_0320")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton * pAccCan;
	pAccCan=(CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);
	pAccCan->SetCheck(1);	

	CButton * pPART;
	pPART=(CButton *)GetDlgItem(IDC_rPART);
	pPART->SetCheck(1);	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

INT CBO_CAT_REGNO_0320_POPUP::DisplayRegNo(CString strRegClass, INT nStart, INT nEnd)
{
EFS_BEGIN

	if (strRegClass.IsEmpty()) return -1;
	if (nStart > nEnd) return -1;

	/*CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_REG_0320"));
	if (pCM == NULL) return -1;

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("GRD_REG_0320"));
	if (pGrid == NULL) return -1;
	*/
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_0320_GRD"));
	if (pDM == NULL) return -1;

	CString strAlias;

	INT nColCount = pDM->RefList.GetCount();
	INT nRowCount = pDM->GetRowCount();

	strAlias.Format(_T("��Ϲ�ȣ%d"), nColCount);
	if (nRowCount == 0 || 
		!pDM->GetCellData(strAlias, nRowCount -1).IsEmpty())
	{
		pDM->InsertRow(-1);
		nRowCount++;
	}

	CString strData;
	for (INT i = 0; i < nColCount; i++)
	{
		strAlias.Format(_T("��Ϲ�ȣ%d"), i + 1);
		if (pDM->GetCellData(strAlias, nRowCount - 1).IsEmpty())
		{
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			// Insert Data
// 			if (nStart == nEnd)
// 			{
// 				strData.Format(_T("%s%010d"), strRegClass, nStart);
// 			}
// 			else if (nStart < nEnd)
// 			{
// 				strData.Format(_T("%s%010d ~ %s%010d"), strRegClass, nStart, strRegClass, nEnd);
// 			}
			//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
			// Insert Data

			if (nStart == nEnd)
			{
				CString strTempRegNo;
				strTempRegNo.Format(_T("%d"),nStart);
				strData = pParentMgr->m_pInfo->MakeRegNo(strRegClass,strTempRegNo);
			}
			else if (nStart < nEnd)
			{
				CString strStartRegNo;
				CString strEndRegNo;
				CString strTempRegNo;

				strTempRegNo.Format(_T("%d"),nStart);
				strStartRegNo = pParentMgr->m_pInfo->MakeRegNo(strRegClass,strTempRegNo);

				strTempRegNo.Format(_T("%d"),nEnd);
				strEndRegNo = pParentMgr->m_pInfo->MakeRegNo(strRegClass,strTempRegNo);

				strData.Format(_T("%s ~ %s"), strStartRegNo, strEndRegNo);
			}
			//=====================================================
			
			pDM->SetCellData(strAlias, strData, nRowCount -1);
			break;
		}
	}
	
	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_REGNO_0320_POPUP::OnbOK() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_REG_0320_GRD"));
	if (pDM == NULL) return;
	pDM->FreeData();

	pDM = FindDM(_T("DM_BO_CAT_REG_0320"));
	if (pDM == NULL) return;

	CString sRegCode;
	GetControlData(_T("CM_BO_CAT_REG_0320"), _T("BB_��ϱ���_�ڵ�"), sRegCode);

	sRegCode.Replace( _T(" ") , NULL );
	if ( sRegCode == _T("") ) {
		AfxMessageBox ( _T("��ϱ����� �����ϼ���.") );
		return;
	}

	CString sStart , sEnd;
	GetControlData(_T("CM_BO_CAT_REG_0320"), _T("��������"), sStart);
	GetControlData(_T("CM_BO_CAT_REG_0320"), _T("��������"), sEnd);

	CButton * pPart;
	pPart = (CButton *)GetDlgItem(IDC_rPART);
	if ( pPart->GetCheck() == 1 ) {
		if ( sStart == _T("") || sEnd == _T("") ) {
			AfxMessageBox(_T("��Ϲ�ȣ�� ������ �Է��Ͽ� �ֽʽÿ�."));
			return;
		}
	}

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	//CButton * pAccCan;
	//pAccCan=(CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);

	CButton * pRegNo;
	pRegNo = (CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);

	CString sRegStart, sRegEnd;
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	sRegStart.Format ( _T("%s%010s") , sRegCode, sStart );
// 	sRegEnd.Format ( _T("%s%010s") , sRegCode, sEnd );
	sRegStart = m_pInfo->MakeRegNo(sRegCode, sStart );
	sRegStart = m_pInfo->MakeRegNo(sRegCode, sEnd );
	//=====================================================
	
	CString sQuery;
	if (pRegNo->GetCheck() == 1 ) {
		sQuery = _T("SELECT DISTINCT REG_NO FROM BO_BOOK_TBL WHERE REG_NO >= '") + sRegStart + _T("' AND REG_NO <= '") + sRegEnd + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE ORDER BY REG_NO");
	}
	else {
		sQuery = _T("SELECT distinct(REG_CODE || TO_CHAR(REG_NO, 'FM0000000000')) FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_NO >= ") + sStart + _T(" AND REG_NO <= ") + sEnd + _T(" AND REG_CODE = '") + sRegCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE ORDER BY REG_CODE || TO_CHAR(REG_NO, 'FM0000000000')");
	}	

	CButton * pDropReg;
	pDropReg = (CButton *)GetDlgItem(IDC_rOMISSIONCONTROLNO_LIST);
	if ( pPart->GetCheck() != 1 && pDropReg->GetCheck() == 1 ) {
		//sQuery = _T("SELECT REG_CODE || ACCESSION_REG_NO FROM CO_DROP_ACCESSION_NO_TBL");
		sQuery = _T("SELECT distinct(REG_CODE || TO_CHAR(REG_NO, 'FM0000000000')) FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE ='") + sRegCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE ORDER BY REG_CODE || TO_CHAR(REG_NO, 'FM0000000000')");
	}
	if ( pPart->GetCheck() != 1 && pDropReg->GetCheck() != 1 ) {
		//sQuery = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE REG_NO <= '") + sRegEnd + _T("' ORDER BY REG_NO");
		CString sRegStart, sRegEnd;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		sRegStart.Format( _T("%s%010s") , sRegCode, _T("0") );
// 		sRegEnd.Format( _T("%s%010s"), sRegCode, _T("9999999999") );
		CString strTempEndRegNo;
		sRegStart = m_pInfo->MakeRegNo(sRegCode, _T("0") );
		strTempEndRegNo = _T("");
		for( INT i= 0 ; i < 12 - nRegCodeLength ; i++)
		{
			strTempEndRegNo += _T("9");			
		}
		sRegEnd = m_pInfo->MakeRegNo(sRegCode, _T("strTempEndRegNo") );
		//=====================================================
		
		sQuery = _T("SELECT DISTINCT REG_NO FROM BO_BOOK_TBL WHERE REG_NO >= '") + sRegStart + _T("' AND REG_NO <= '") + sRegEnd + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE ORDER BY REG_NO");
	}

	dm.RestructDataManager(sQuery);
	INT iRowCount = dm.GetRowCount();
	if ( iRowCount < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
		//return;
	}

	CString sRowCount;
	sRowCount.Format( _T("%d") , iRowCount );
	SetControlData(_T("CM_BO_CAT_REG_0320"), _T("��å��"), sRowCount);

	/*
	CString strWhere = _T("REG_NO >= 'EM0000000001' AND REG_NO <= 'EM0000009999' ORDER BY REG_NO");
	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	INT iRowCount = pDM->GetRowCount();
	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}
	*/


	//CString strRegClass = _T("EM");
	CString strRegClass = sRegCode;
	CString strRegNo;
	INT		nRegNo = -1;

	INT nStart = -1;
	INT nEnd = -1;

	for ( INT i = 0 ; i < iRowCount ; i ++ ) {
		//strRegNo = pDM->GetCellData(_T("BB_��Ϲ�ȣ"), i);
		strRegNo = dm.GetCellData( i , 0 );
		//strRegNo.Replace(_T("EM"), NULL);
		strRegNo.Replace(sRegCode, NULL);
		nRegNo = _ttoi(strRegNo);

		nStart = nRegNo;
		nEnd = nRegNo;

		// check next val
		while (TRUE)
		{
			//strRegNo = pDM->GetCellData(_T("BB_��Ϲ�ȣ"), ++i);
			++i;
			if ( i == iRowCount ) {
				break;
			}
			strRegNo = dm.GetCellData( i , 0 );
			//strRegNo = dm.GetCellData( 0, ++i );
			//strRegNo.Replace(_T("EM"), NULL);
			strRegNo.Replace(sRegCode, NULL);
			nRegNo = _ttoi(strRegNo);

			if (nEnd + 1 == nRegNo)
			{
				nEnd++;	
			}
			else
			{
				i--;
				break;
			}
		}

		DisplayRegNo(strRegClass, nStart, nEnd);
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_REG_0320"));
	pCM->AllControlDisplay();

EFS_END
}

VOID CBO_CAT_REGNO_0320_POPUP::OnrALL() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CButton * pSTART;
	pSTART=(CButton *)GetDlgItem(IDC_EDT_START);
	pSTART->EnableWindow(false);

	CButton * pEND;
	pEND=(CButton *)GetDlgItem(IDC_EDT_END);
	pEND->EnableWindow(false);

EFS_END
}

VOID CBO_CAT_REGNO_0320_POPUP::OnrPART() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CButton * pSTART;
	pSTART=(CButton *)GetDlgItem(IDC_EDT_START);
	pSTART->EnableWindow(true);

	CButton * pEND;
	pEND=(CButton *)GetDlgItem(IDC_EDT_END);
	pEND->EnableWindow(true);

EFS_END
}

VOID CBO_CAT_REGNO_0320_POPUP::OnCancel() 
{
EFS_BEGIN

	// TODO: Add extra cleanup here
	EndDialog(IDOK);
	//CDialog::OnCancel();

EFS_END
}

VOID CBO_CAT_REGNO_0320_POPUP::OnCancel0302() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	EndDialog(IDOK);

EFS_END
}

VOID CBO_CAT_REGNO_0320_POPUP::OnbRegNoTOFILE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_0320_GRD"));
	if (pDM == NULL) return;

	INT iRowCount = pDM->GetRowCount();	
	if ( iRowCount < 1 ) {
		return;
	}

	CString sControlCode;
	GetControlData( _T("CM_BO_CAT_REG_0320"), _T("BB_��ϱ���_�ڵ�"), sControlCode );	
	CString sTotal;
	GetControlData( _T("CM_BO_CAT_REG_0320"), _T("��å��"), sTotal );

	//���� �����
	CFile file;
	if ( ::CreateDirectory( _T("..\\�����"), NULL ) == FALSE ) {		
	}
	
	//CFile *fp;
	FILE *fp;
	//TCHAR cLine[256];
	CString sPath = _T("..\\�����\\KMO2003_Build.txt");
	CButton * pAccCan;
	pAccCan=(CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);
	if ( pAccCan->GetCheck() == 1 ) {
		sPath = _T("..\\�����\\") + sControlCode + _T("_Build.txt");
	}
	else {
		sPath = _T("..\\�����\\") + sControlCode + _T("_Omission.txt");
	}
	fp = _tfopen( sPath , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("KMO2003_Build.txt ������ �����ϴ�.") );
		return;
	}
	
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
		
	_ftprintf( fp, _T("-----------------------------------------------------------------------------------\r\n") );
	CString sLine = _T(" ### ") + sControlCode + _T(" ### \r\n");
	_ftprintf( fp, sLine );
	_ftprintf( fp, _T("-----------------------------------------------------------------------------------\r\n") );
	
	for ( INT i = 0 ; i < iRowCount ; i++ ) {		
		CString sControlNo1 = pDM->GetCellData( _T("��Ϲ�ȣ1"), i );
		if ( sControlNo1.GetLength() == 12 ) {
			sControlNo1 = sControlNo1 + _T("               ");
		}
		CString sControlNo2 = pDM->GetCellData( _T("��Ϲ�ȣ2"), i );
		if ( sControlNo2.GetLength() == 12 ) {
			sControlNo2 = sControlNo2 + _T("               ");
		}
		CString sControlNo3 = pDM->GetCellData( _T("��Ϲ�ȣ3"), i );
		if ( sControlNo3.GetLength() == 12 ) {
			sControlNo3 = sControlNo3 + _T("               ");
		}
		CString sData = sControlNo1 + _T(" ") + sControlNo2 + _T(" ") + sControlNo3 + _T("\r\n");
		_ftprintf( fp, sData );
	}
	_ftprintf( fp, _T("\r\n-----------------------------------------------------------------------------------\r\n") );
	sTotal = _T("  �� �� :  ") + sTotal;
	_ftprintf( fp, sTotal );
	_ftprintf( fp, _T("\r\n-----------------------------------------------------------------------------------\r\n") );
	fclose( fp );

	CString sMessage = _T("[ ") + sPath + _T(" ] ���Ͽ� ���� �۾��� �Ϸ�Ǿ����ϴ�.") ;
	AfxMessageBox( sMessage );

EFS_END
}


HBRUSH CBO_CAT_REGNO_0320_POPUP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

