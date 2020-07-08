// BO_CAT_REG_SEARCH_0100.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_REG_SEARCH_0100.h"
#include "BO_CAT_REG_SEARCH1.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_SEARCH_0100 dialog


CBO_CAT_REG_SEARCH_0100::CBO_CAT_REG_SEARCH_0100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_SEARCH_0100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_REG_SEARCH_0100::~CBO_CAT_REG_SEARCH_0100()
{
}

VOID CBO_CAT_REG_SEARCH_0100::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_SEARCH_0100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_SEARCH_0100, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_SEARCH_0100)
	ON_BN_CLICKED(IDC_bRegSearch0100OK, OnbRegSearch0100OK)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_SEARCH_0100 message handlers

BOOL CBO_CAT_REG_SEARCH_0100::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_REG_SEARCH_0100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_SEARCH_0100")) < 0)
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

VOID CBO_CAT_REG_SEARCH_0100::OnbRegSearch0100OK() 
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
	GetControlData(_T("CM_BO_CAT_REG_SEARCH_0100"), _T("BB_��ϱ���_�ڵ�"), sRegCode);

	sRegCode.Replace( _T(" ") , NULL );
	if ( sRegCode == _T("") ) {
		AfxMessageBox ( _T("��ϱ����� �����ϼ���.") );
		return;
	}

	CString sStart , sEnd;
	GetControlData(_T("CM_BO_CAT_REG_SEARCH_0100"), _T("��������"), sStart);
	GetControlData(_T("CM_BO_CAT_REG_SEARCH_0100"), _T("��������"), sEnd);

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

	//��������ȣ �˻�
	CString sQueryLastNumber;
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	sQueryLastNumber.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='REG_NO' ")
							 _T("AND DIVISION_VALUE='%s' AND MANAGE_CODE='%s'")
							 , sRegCode, m_pInfo->MANAGE_CODE );
	//sQueryLastNumber.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE ='REG_NO' AND DIVISION_VALUE ='%s'"), sRegCode );
	//--2008.10.14 UPDATE BY PWR --}}
	CString sValue;	//���� ��Ϲ�ȣ
	pDM->GetOneValueQuery( sQueryLastNumber , sValue );

	//CButton * pAccCan;
	//pAccCan=(CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);

	CButton * pRegNo;
	pRegNo = (CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);

	CString sRegStart, sRegEnd;

	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
// 	sRegStart.Format ( _T("%s%010s") , sRegCode, sStart );
// 	sRegEnd.Format ( _T("%s%010s") , sRegCode, sEnd );
	sRegStart = pParentMgr->m_pInfo->MakeRegNo(sRegCode, sStart );
	sRegEnd = pParentMgr->m_pInfo->MakeRegNo(sRegCode, sEnd );
	//=====================================================

	CString sQuery;
	if (pRegNo->GetCheck() == 1 ) {
		//++2008.10.14 UPDATE BY PWR {{++
		// ���սý��� ����
		sQuery = _T("SELECT /*+ INDEX(BO_BOOK_TBL BO_BOOK_TBL_REG_NO_I)*/ REG_NO FROM BO_BOOK_TBL ")
				 _T("WHERE REG_NO >= '") + sRegStart + _T("' AND REG_NO <= '") + sRegEnd + 
				 _T("' AND MANAGE_CODE='") + m_pInfo->MANAGE_CODE + _T("' ")
				 _T("UNION SELECT /*+ INDEX(SE_BOOK_TBL SE_BOOK_TBL_REG_NO_I)*/ REG_NO FROM SE_BOOK_TBL ")
				 _T("WHERE REG_NO >= '") + sRegStart + _T("' AND REG_NO <= '") + sRegEnd + 
				 _T("' AND MANAGE_CODE='") + m_pInfo->MANAGE_CODE + _T("'");
		//sQuery = _T("SELECT /*+ INDEX(BO_BOOK_TBL BO_BOOK_TBL_REG_NO_I)*/ REG_NO FROM BO_BOOK_TBL WHERE REG_NO >= '") + sRegStart + _T("' AND REG_NO <= '") + sRegEnd + _T("'")
		//	_T(" UNION SELECT /*+ INDEX(SE_BOOK_TBL SE_BOOK_TBL_REG_NO_I)*/ REG_NO FROM SE_BOOK_TBL WHERE REG_NO >= '") + sRegStart + _T("' AND REG_NO <= '") + sRegEnd + _T("'");
		//--2008.10.14 UPDATE BY PWR --}}
		//m_sRegSw = _T("��Ϲ�ȣ");
		((CBO_CAT_REG_SEARCH*)pParentMgr)->m_sRegSh = _T("��Ϲ�ȣ");	
	}
	else {
		//++2008.10.14 UPDATE BY PWR {{++
		// ���սý��� ����
		sQuery = _T("SELECT REG_CODE || REG_NO FROM CO_DROP_ACCESSION_NO_TBL ")
				 _T("WHERE  REG_NO >= ") + sStart + _T(" AND REG_NO <= ") + sEnd + 
				 _T(" AND REG_CODE = '") + sRegCode + _T("' AND MANAGE_CODE='") + m_pInfo->MANAGE_CODE + 
				 _T("' ORDER BY REG_NO");
		//sQuery = _T("SELECT REG_CODE || REG_NO FROM CO_DROP_ACCESSION_NO_TBL WHERE  REG_NO >= ") + sStart + _T(" AND REG_NO <= ") + sEnd + _T(" AND REG_CODE = '") + sRegCode + _T("' ORDER BY REG_NO");
		//--2008.10.14 UPDATE BY PWR --}}
		//m_sRegSw = _T("������Ϲ�ȣ");
		((CBO_CAT_REG_SEARCH*)pParentMgr)->m_sRegSh = _T("������Ϲ�ȣ");	
	}	

	CButton * pDropReg;
	pDropReg = (CButton *)GetDlgItem(IDC_rOMISSIONCONTROLNO_LIST);
	if ( pPart->GetCheck() != 1 && pDropReg->GetCheck() == 1 ) {
		//++2008.10.14 UPDATE BY PWR {{++
		// ���սý��� ����
		sQuery = _T("SELECT REG_CODE || REG_NO FROM CO_DROP_ACCESSION_NO_TBL ")
				 _T("WHERE REG_CODE ='") + sRegCode + _T("' AND MANAGE_CODE='") + m_pInfo->MANAGE_CODE + 
				 _T("' ORDER BY REG_NO");
		//sQuery = _T("SELECT REG_CODE || REG_NO FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE ='") + sRegCode + _T("' ORDER BY REG_NO");
		//--2008.10.14 UPDATE BY PWR --}}
	}
	if ( pPart->GetCheck() != 1 && pDropReg->GetCheck() != 1 ) {
		//sQuery = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE REG_NO <= '") + sRegEnd + _T("' ORDER BY REG_NO");
		CString sRegStart, sRegEnd;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		sRegStart.Format( _T("%s%010s") , sRegCode, _T("0") );
// 		sRegEnd.Format( _T("%s%010s"), sRegCode, _T("9999999999") );
		sRegStart = pParentMgr->m_pInfo->MakeRegNo(sRegCode, _T("0") );
		sRegEnd = pParentMgr->m_pInfo->MakeRegNo(sRegCode, _T("9999999999") );
		//=====================================================
		
		//++2008.10.14 UPDATE BY PWR {{++
		// ���սý��� ����
		sQuery = _T("SELECT /*+ INDEX(BO_BOOK_TBL BO_BOOK_TBL_REG_NO_I)*/ REG_NO FROM BO_BOOK_TBL ")
				 _T("WHERE REG_NO >= '") + sRegStart + _T("' AND REG_NO <= '") + sRegEnd + 
				 _T("' AND MANAGE_CODE='") + m_pInfo->MANAGE_CODE +
				 _T("' UNION SELECT /*+ INDEX(SE_BOOK_TBL SE_BOOK_TBL_REG_NO_I)*/ REG_NO FROM SE_BOOK_TBL ")
				 _T("WHERE REG_NO >= '") + sRegStart + _T("' AND REG_NO <= '") + sRegEnd + 
				 _T("' AND MANAGE_CODE='") + m_pInfo->MANAGE_CODE + _T("'");
		//sQuery = _T("SELECT /*+ INDEX(BO_BOOK_TBL BO_BOOK_TBL_REG_NO_I)*/ REG_NO FROM BO_BOOK_TBL WHERE REG_NO >= '") + sRegStart + _T("' AND REG_NO <= '") + sRegEnd + _T("'")
		//	_T(" UNION SELECT /*+ INDEX(SE_BOOK_TBL SE_BOOK_TBL_REG_NO_I)*/ REG_NO FROM SE_BOOK_TBL WHERE REG_NO >= '") + sRegStart + _T("' AND REG_NO <= '") + sRegEnd + _T("'");
		//--2008.10.14 UPDATE BY PWR --}}
	}

	dm.RestructDataManager(sQuery);
	INT iRowCount = dm.GetRowCount();
	if ( iRowCount < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
		//return;
	}

	CString sRowCount;
	sRowCount.Format( _T("%d") , iRowCount );
	SetControlData(_T("CM_BO_CAT_REG_SEARCH_0100"), _T("��å��"), sRowCount);

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

		//===================================================
		//2010.01.20 UPDATE BY PJW : ��ϱ����� �����ϰ�� ��Ϲ�ȣ�� �����
//		strRegNo.Replace(sRegCode, NULL);
		strRegNo.TrimLeft();	strRegNo.TrimRight();
		CString strTempCode;
		strTempCode = strRegNo.Left( m_pInfo->nREG_CODE_LENGTH );
		if(  strTempCode == sRegCode )
		{
			strRegNo = strRegNo.Mid(m_pInfo->nREG_CODE_LENGTH);
		}
		//===================================================

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
			//strRegNo.Replace(_T("EM"), NULL);
			//===================================================
			//2010.01.20 UPDATE BY PJW : ��ϱ����� �����ϰ�� ��Ϲ�ȣ�� �����
//			strRegNo.Replace(sRegCode, NULL);
			strRegNo.TrimLeft();	strRegNo.TrimRight();
			if( strRegNo.Left( m_pInfo->nREG_CODE_LENGTH ) == sRegCode )
			{
				strRegNo = strRegNo.Mid(m_pInfo->nREG_CODE_LENGTH);
			}
			//===================================================

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
	//===============================================
	//2008.07.03 ADD BY PJW : ��Ϲ�ȣ���� ���̾�α׿��� ��å�� ����ǵ��� ����
 	pCM->SetControlMgrData(_T("��å��"), sRowCount);
	//===============================================
	pCM->AllControlDisplay();

	//((CBO_CAT_REG_SEARCH*)pParentMgr)->m_sRegSh = _T("���");	
	//SetRegSwitch();
	//m_pWindow->SetRegSwitch();

EFS_END
}

INT CBO_CAT_REG_SEARCH_0100::DisplayRegNo(CString strRegClass, INT nStart, INT nEnd)
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
			// Insert Data
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
			//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
// 			if (nStart == nEnd)
// 			{
// 				strData.Format(_T("%s%010d"), strRegClass, nStart);
// 			}
// 			else if (nStart < nEnd)
// 			{
// 				strData.Format(_T("%s%010d ~ %s%010d"), strRegClass, nStart, strRegClass, nEnd);
// 			}

			CString strTemp;
			CString strStartRegNo;
			CString strEndRegNo;
			if (nStart == nEnd)
			{
				strTemp.Format(_T("%d"),nStart);
				strData = pParentMgr->m_pInfo->MakeRegNo(strRegClass, strTemp);
			}
			else if (nStart < nEnd)
			{
				strTemp.Format(_T("%d"),nStart);
				strStartRegNo = pParentMgr->m_pInfo->MakeRegNo(strRegClass, strTemp);

				strTemp.Format(_T("%d"),nEnd);
				strEndRegNo = pParentMgr->m_pInfo->MakeRegNo(strRegClass, strTemp);

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

VOID CBO_CAT_REG_SEARCH_0100::Onb0320TOFILE(CString sRegSw) 
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
	GetControlData( _T("CM_BO_CAT_REG_SEARCH_0100"), _T("BB_��ϱ���_�ڵ�"), sControlCode );	
	CString sTotal;
	GetControlData( _T("CM_BO_CAT_REG_SEARCH_0100"), _T("��å��"), sTotal );

	//���� ���̾�α�
	CString sFirstFileName;
	if ( sRegSw == _T("��Ϲ�ȣ") ) {
		sFirstFileName = _T("��Ϲ�ȣ����Ʈ.txt");
	}
	else {
		sFirstFileName = _T("������Ϲ�ȣ����Ʈ.txt");		
	}

	//  �� ���丮�� �����Ѵ�.
	TCHAR    sCurrentDirectory[260];	
    _tgetcwd( sCurrentDirectory, 260 ) ;

	CFileDialog dlg(FALSE, _T("*.*"), sFirstFileName,
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						_T("��������(*.txt)|All Files(*.*)|*.*||") );
	/*
    if ( IDOK != dlg.DoModal() ) return;
	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return;
    sFileName = dlg.GetPathName();
	*/
	
	CString sFileName;
	
	//CFileDialog  dlg;
	if( dlg.DoModal() == IDOK) {
		sFileName = dlg.GetPathName();
		// strFullPath�� ������Ʈ���� ����..
	}
	else {
		return;
	}	

	// ���� ���丮 ����
	_tchdir( sCurrentDirectory ) ;

	/*
	//���� �����
	CFile file;
	if ( ::CreateDirectory( _T("..\\�����"), NULL ) == FALSE ) {		
	}
	*/
	
	
	//CFile *fp;
	FILE *fp;
	//TCHAR cLine[256];
	CString sPath = _T("..\\�����\\KMO2003_Build.txt");
	CButton * pAccCan;
	pAccCan=(CButton *)pParentMgr->GetDlgItem(IDC_rCONTROLNO_LIST);
	//if ( pAccCan->GetCheck() == 1 ) {
	if ( sRegSw == _T("��Ϲ�ȣ") ) {
		sPath = _T("..\\�����\\") + sControlCode + _T("_Build.txt");
	}
	else {
		sPath = _T("..\\�����\\") + sControlCode + _T("_Omission.txt");
	}


	sPath = sFileName;

	fp = _tfopen( sPath , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("KMO2003_Build.txt ������ �����ϴ�.") );
		return;
	}
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}
	
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
	//return;

EFS_END
}
HBRUSH CBO_CAT_REG_SEARCH_0100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

void CBO_CAT_REG_SEARCH_0100::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
