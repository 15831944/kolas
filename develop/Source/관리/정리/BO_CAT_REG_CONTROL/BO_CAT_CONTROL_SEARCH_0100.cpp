// BO_CAT_CONTROL_SEARCH_0100.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_CONTROL_SEARCH_0100.h"
#include "BO_CAT_CONTROL_SEARCH.h"
#include "../../../����/����/�弭����/LOC_CommonAPI/LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_CONTROL_SEARCH_0100 dialog


CBO_CAT_CONTROL_SEARCH_0100::CBO_CAT_CONTROL_SEARCH_0100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_CONTROL_SEARCH_0100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_CONTROL_SEARCH_0100::~CBO_CAT_CONTROL_SEARCH_0100()
{
}

VOID CBO_CAT_CONTROL_SEARCH_0100::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_CONTROL_SEARCH_0100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_CONTROL_SEARCH_0100, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_CONTROL_SEARCH_0100)
	ON_BN_CLICKED(IDC_bControlSearch0100OK, OnbControlSearch0100OK)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_CONTROL_SEARCH_0100 message handlers

BOOL CBO_CAT_CONTROL_SEARCH_0100::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_CONTROL_SEARCH_0100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_CONTROL_SEARCH_0100")) < 0)
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

INT CBO_CAT_CONTROL_SEARCH_0100::DisplayRegNo(CString strRegClass, INT nStart, INT nEnd)
{
EFS_BEGIN

	if (strRegClass.IsEmpty()) return -1;
	if (nStart > nEnd) return -1;

	/*CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_REG_0320"));
	if (pCM == NULL) return -1;

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("GRD_REG_0320"));
	if (pGrid == NULL) return -1;
	*/
	//CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_0320_GRD"));
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_CONTROL"));
	
	if (pDM == NULL) return -1;

	CString strAlias;

	INT nColCount = pDM->RefList.GetCount();
	INT nRowCount = pDM->GetRowCount();

	strAlias.Format(_T("�����ȣ%d"), nColCount);
	if (nRowCount == 0 || 
		!pDM->GetCellData(strAlias, nRowCount -1).IsEmpty())
	{
		pDM->InsertRow(-1);
		nRowCount++;
	}

	CString strData;
	for (INT i = 0; i < nColCount; i++)
	{
		strAlias.Format(_T("�����ȣ%d"), i + 1);
		if (pDM->GetCellData(strAlias, nRowCount - 1).IsEmpty())
		{
			// Insert Data
			if (nStart == nEnd)
			{
				strData.Format(_T("%s%09d"), strRegClass, nStart);
			}
			else if (nStart < nEnd)
			{
				//strData.Format(_T("%s%010d ~ %s%010d"), strRegClass, nStart, strRegClass, nEnd);
				strData.Format(_T("%s%09d ~ %s%09d"), strRegClass, nStart, strRegClass, nEnd);
			}
			pDM->SetCellData(strAlias, strData, nRowCount -1);
			break;
		}
	}
	
	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_CONTROL_SEARCH_0100::OnbControlSearch0100OK() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_REG_CONTROL"));
	if (pDM == NULL) return;
	pDM->FreeData();

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_REG_CONTROL"));
	pCM->AllControlDisplay();
	
	CString sControlCode;
	GetControlData( _T("CM_BO_CAT_CONTROL_SEARCH_0100"), _T("BS_�ڷᱸ��_�ڵ�"), sControlCode );
	CString sYear;
	GetControlData( _T("CM_BO_CAT_CONTROL_SEARCH_0100"), _T("�⵵"), sYear );
	sYear.TrimLeft();
	if ( sYear == _T("") ) {
		AfxMessageBox( _T("�⵵�� �Է��ϼ���.") );
		return ;
	}

	sControlCode.TrimLeft(); sControlCode.TrimRight();
	if( sControlCode.IsEmpty() )
	{
		AfxMessageBox( _T("�ڷᱸ���� �������ֽʽÿ�") );
		return ;
	}

	CESL_DataMgr dm;

	dm.SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection());

	INT ids = RestructDataManager( &dm , sYear , sControlCode );
	if( ids > 0 ) return ;
	
	INT iRowCount = dm.GetRowCount();
	if ( iRowCount < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
		//return;
	}

	CButton * pAccCan;
	pAccCan=(CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);
	
	if ( pAccCan->GetCheck() == 1 ) {
		((CBO_CAT_CONTROL_SEARCH*)pParentMgr)->m_sRegSw = _T("�����ȣ");
		CString sRowCount;
		sRowCount.Format( _T("%d") , iRowCount );
		SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("������"), sRowCount);

		CString strRegClass = sControlCode;
		//CString strRegClass = sRegCode;
		CString strRegNo;
		INT		nRegNo = -1;

		INT nStart = -1;
		INT nEnd = -1;

		for ( INT i = 0 ; i < iRowCount ; i ++ ) {		
			//strRegNo = pDM->GetCellData(_T("BB_��Ϲ�ȣ"), i);
			strRegNo = dm.GetCellData( i , 0 );
			//strRegNo.Replace(_T("EM"), NULL);
			strRegNo.Replace(strRegClass, NULL);
			nRegNo = _ttoi(strRegNo);

			nStart = nRegNo;
			nEnd = nRegNo;

			// check next val
			while (TRUE)
			{
				//strRegNo = pDM->GetCellData(_T("BB_��Ϲ�ȣ"), ++i);
				//i++;
				//i = i-1;
				++i;
				if ( i == iRowCount ) {
					break;
				}
				strRegNo = dm.GetCellData( i , 0 );
				//strRegNo = dm.GetCellData( ++i-1 , 0 );
				//strRegNo.Replace(_T("EM"), NULL);
				strRegNo.Replace(strRegClass, NULL);
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
		pCM = FindCM(_T("CM_BO_CAT_REG_CONTROL"));
		pCM->AllControlDisplay();
	}
	else {	//���������ȣ ����Ʈ
		((CBO_CAT_CONTROL_SEARCH*)pParentMgr)->m_sRegSw = _T("���������ȣ");
		/*
		dm.RestructDataManager(sQuery);
		INT iRowCount = dm.GetRowCount();
		if ( iRowCount < 1 ) {
			AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
			//return;
		}
		*/

		//CString sRowCount;
		//sRowCount.Format( _T("%d") , iRowCount );
		//SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("������"), sRowCount);

		CString strRegClass = sControlCode;
		
		CString strRegNo;
		INT		nRegNo = -1;

		INT nStart = -1;
		INT nEnd = -1;
		BOOL bStart = FALSE;

		INT iCount = 0;

		INT nSpeciesCnt = 0;
		for ( INT i = 0 ; i < iRowCount ; i ++ ) {
			if ( i == 0 ) {
				strRegNo = dm.GetCellData( i , 0 );
				
				strRegNo.Replace(strRegClass, NULL);
				nRegNo = _ttoi(strRegNo);
				CString sStartNum = strRegNo.Right(5);
				INT iStartNum = _ttoi( sStartNum );

				nStart = nRegNo;
				nEnd = nRegNo-1;

				if ( strRegNo != _T("") && i == 0 && iStartNum != 1 ) {
					CString sStart;
					sStart.Format( _T("%s00001"), sYear );
					nStart = _ttoi( sStart );
					DisplayRegNo(strRegClass, nStart, nEnd);

					nSpeciesCnt += nEnd - nStart + 1;

					bStart = true;					
					iCount = iStartNum -1;
					continue;
				}
			}

			nStart = nRegNo;

			// check next val
			while (TRUE)
			{			
				//++i;
				if ( i == iRowCount ) {
					break;
				}
				strRegNo = dm.GetCellData( i , 0 );
				//strRegNo = dm.GetCellData( ++i-1 , 0 );
				//strRegNo.Replace(_T("EM"), NULL);
				strRegNo.Replace(strRegClass, NULL);
				nRegNo = _ttoi(strRegNo);
				//nStart = nRegNo;
				++i;

				if (nStart + 1 == nRegNo)
				{
					//nEnd++;
					nStart++;
					iCount++;
				}
				else
				{
					//i--;
					nStart++;
					i--;
					iCount++;
					break;
				}
			}
			nEnd = nRegNo - 1;

			DisplayRegNo(strRegClass, nStart, nEnd);

			nSpeciesCnt += nEnd - nStart + 1;
			
		}
		//�Ѽ� ���ϱ�
		//strRegNo = dm.GetCellData( i-1 , 0 );		
		//strRegNo.Right(5);
		//nRegNo = _ttoi(strRegNo);

//		iCount = _ttoi( strRegNo.Right(5) );
//		iCount = iCount - iRowCount;
		CString sRowCount;
		sRowCount.Format( _T("%d") , nSpeciesCnt );
		SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("������"), sRowCount);

		CESL_ControlMgr *pCM;
		pCM = FindCM(_T("CM_BO_CAT_REG_CONTROL"));
		pCM->AllControlDisplay();
	}

EFS_END
}

VOID CBO_CAT_CONTROL_SEARCH_0100::OnbRegTOFILE(CString sRegSw)
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM;
	//pDM = FindDM(_T("DM_BO_CAT_REG_CONTROL"));
	//if (pDM == NULL) return;
	//pDM->FreeData();

	pDM = FindDM(_T("DM_BO_CAT_REG_CONTROL"));
	if (pDM == NULL) return;

	INT iRowCount = pDM->GetRowCount();
	//iRowCount = dm.GetRowCount();
	if ( iRowCount < 1 ) {
		return;
	}

	CString sControlCode;
	/*
	GetControlData( _T("CM_BO_CAT_REG_0320"), _T("BS_�ڷᱸ��_�ڵ�"), sControlCode );
	CString sYear;
	GetControlData( _T("CM_BO_CAT_REG_0320"), _T("�⵵"), sYear );
	CString sTotal;
	GetControlData( _T("CM_BO_CAT_REG_0320"), _T("������"), sTotal );
	*/
	GetControlData( _T("CM_BO_CAT_CONTROL_SEARCH_0100"), _T("BS_�ڷᱸ��_�ڵ�"), sControlCode );
	CString sYear;
	GetControlData( _T("CM_BO_CAT_CONTROL_SEARCH_0100"), _T("�⵵"), sYear );
	CString sTotal;
	GetControlData( _T("CM_BO_CAT_REG_CONTROL"), _T("������"), sTotal );
	

	//���� �����
	CFile file;
	if ( ::CreateDirectory( _T("..\\�����"), NULL ) == FALSE ) {
		//AfxMessageBox( _T("���� ����� ����") );
	}
	
	//CFile *fp;
	FILE *fp;
	//TCHAR cLine[256];
	CString sPath = _T("..\\�����\\KMO2003_Build.txt");
	//CButton * pAccCan;
	//pAccCan=(CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);
	//if ( pAccCan->GetCheck() == 1 ) {
	if ( sRegSw == _T("�����ȣ") ) {
		sPath = _T("..\\�����\\") + sControlCode + sYear + _T("_Build.txt");
	}
	else {
		sPath = _T("..\\�����\\") + sControlCode + sYear + _T("_Omission.txt");
	}
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
	CString sLine = _T(" ### ") + sControlCode + sYear + _T(" ### \r\n");
	_ftprintf( fp, sLine );
	_ftprintf( fp, _T("-----------------------------------------------------------------------------------\r\n") );
	
	for ( INT i = 0 ; i < iRowCount ; i++ ) {		
		CString sControlNo1 = pDM->GetCellData( _T("�����ȣ1"), i );
		if ( sControlNo1.GetLength() == 12 ) {
			sControlNo1 = sControlNo1 + _T("               ");
		}
		CString sControlNo2 = pDM->GetCellData( _T("�����ȣ2"), i );
		if ( sControlNo2.GetLength() == 12 ) {
			sControlNo2 = sControlNo2 + _T("               ");
		}
		CString sControlNo3 = pDM->GetCellData( _T("�����ȣ3"), i );
		if ( sControlNo3.GetLength() == 12 ) {
			sControlNo3 = sControlNo3 + _T("               ");
		}
		CString sData = sControlNo1 + _T(" ") + sControlNo2 + _T(" ") + sControlNo3 + _T("\r\n");
		_ftprintf( fp, sData );
	}
	_ftprintf( fp, _T("\r\n-----------------------------------------------------------------------------------\r\n") );
	sTotal = _T("  �� �� :  ") + sTotal;
	_ftprintf( fp, sTotal );
	_ftprintf( fp, _T("\r\n-----------------------------------------------------------------------------------\\n") );
	fclose( fp );

	CString sMessage = _T("[ ") + sPath + _T(" ] ���Ͽ� ���� �۾��� �Ϸ�Ǿ����ϴ�.") ;
	AfxMessageBox( sMessage );

	return;



	//CButton * pAccCan;
	//pAccCan=(CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);
	//if ( pAccCan->GetCheck() == 1 ) {
	if ( sRegSw == _T("�����ȣ") ) {
		CString sRowCount;
		sRowCount.Format( _T("%d") , iRowCount );
		//SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("������"), sRowCount);

		CString strRegClass = sControlCode;
		//CString strRegClass = sRegCode;
		CString strRegNo;
		INT		nRegNo = -1;

		INT nStart = -1;
		INT nEnd = -1;

		for ( INT i = 0 ; i < iRowCount ; i ++ ) {		
			//strRegNo = pDM->GetCellData(_T("BB_��Ϲ�ȣ"), i);
			strRegNo = dm.GetCellData( i , 0 );
			//strRegNo.Replace(_T("EM"), NULL);
			strRegNo.Replace(strRegClass, NULL);
			nRegNo = _ttoi(strRegNo);

			nStart = nRegNo;
			nEnd = nRegNo;

			// check next val
			while (TRUE)
			{
				//strRegNo = pDM->GetCellData(_T("BB_��Ϲ�ȣ"), ++i);
				//i++;
				//i = i-1;
				++i;
				if ( i == iRowCount ) {
					break;
				}
				strRegNo = dm.GetCellData( i , 0 );
				//strRegNo = dm.GetCellData( ++i-1 , 0 );
				//strRegNo.Replace(_T("EM"), NULL);
				strRegNo.Replace(strRegClass, NULL);
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

			//DisplayRegNo(strRegClass, nStart, nEnd);
			CString strData;			
			strData.Format(_T("%s%09d ~ %s%09d"), strRegClass, nStart, strRegClass, nEnd);
			_ftprintf( fp, strData );
		}
		//CESL_ControlMgr *pCM;
		//pCM = FindCM(_T("CM_BO_CAT_REG_CONTROL"));
		//pCM->AllControlDisplay();
	}
	else {	//���������ȣ ����Ʈ
		/*
		dm.RestructDataManager(sQuery);
		INT iRowCount = dm.GetRowCount();
		if ( iRowCount < 1 ) {
			AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
			//return;
		}
		*/

		//CString sRowCount;
		//sRowCount.Format( _T("%d") , iRowCount );
		//SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("������"), sRowCount);

		CString strRegClass = sControlCode;
		
		CString strRegNo;
		INT		nRegNo = -1;

		INT nStart = -1;
		INT nEnd = -1;
		BOOL bStart = FALSE;

		INT iCount = 0;
		for ( INT i = 0 ; i < iRowCount ; i ++ ) {
			if ( i == 0 ) {
				strRegNo = dm.GetCellData( i , 0 );
				
				strRegNo.Replace(strRegClass, NULL);
				nRegNo = _ttoi(strRegNo);
				CString sStartNum = strRegNo.Right(5);
				INT iStartNum = _ttoi( sStartNum );

				nStart = nRegNo;
				nEnd = nRegNo-1;

				if ( strRegNo != _T("") && i == 0 && iStartNum != 1 ) {
					CString sStart;
					sStart.Format( _T("%s00001"), sYear );
					nStart = _ttoi( sStart );
					DisplayRegNo(strRegClass, nStart, nEnd);
					bStart = true;					
					iCount = iStartNum -1;
					continue;
				}
			}

			nStart = nRegNo;

			// check next val
			while (TRUE)
			{			
				//++i;
				if ( i == iRowCount ) {
					break;
				}
				strRegNo = dm.GetCellData( i , 0 );
				//strRegNo = dm.GetCellData( ++i-1 , 0 );
				//strRegNo.Replace(_T("EM"), NULL);
				strRegNo.Replace(strRegClass, NULL);
				nRegNo = _ttoi(strRegNo);
				//nStart = nRegNo;
				++i;

				if (nStart + 1 == nRegNo)
				{
					//nEnd++;
					nStart++;
					iCount++;
				}
				else
				{
					//i--;
					nStart++;
					i--;
					iCount++;
					break;
				}
			}
			nEnd = nRegNo - 1;

			DisplayRegNo(strRegClass, nStart, nEnd);
		}
		//�Ѽ� ���ϱ�
		//strRegNo = dm.GetCellData( i-1 , 0 );		
		//strRegNo.Right(5);
		//nRegNo = _ttoi(strRegNo);

		iCount = _ttoi( strRegNo.Right(5) );
		iCount = iCount - iRowCount;
		CString sRowCount;
		sRowCount.Format( _T("%d") , iCount );
		SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("������"), sRowCount);

		CESL_ControlMgr *pCM;
		pCM = FindCM(_T("CM_BO_CAT_REG_CONTROL"));
		pCM->AllControlDisplay();
	}

	fclose( fp );

EFS_END
}

/// �˻� �ϱ�
/// �����ȣ �˻��� 1000���� ��� �˻��Ͽ� DM�� �����Ѵ�.
INT CBO_CAT_CONTROL_SEARCH_0100::RestructDataManager( CESL_DataMgr* pDM , CString sYear , CString sControlCode )
{
	EFS_BEGIN

	CButton * pRegNo;
	pRegNo = (CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);
	CButton * pDropReg;
	pDropReg = (CButton *)GetDlgItem(IDC_rOMISSIONCONTROLNO_LIST);
	CButton * pPART;
	pPART=(CButton *)GetDlgItem(IDC_rPART);
	
	CString sMaxControlNo = GetMaxControlNo( pDM , sYear , sControlCode );
	CString sStartNo;
	sStartNo.Format( _T("%s00000") , sYear );
	CString sEndNo = sMaxControlNo;

	CString sQuery;

	/// 1000���� ��� �����Ѵ�.
	INT nLoopCnt = _ttoi(sEndNo) - _ttoi(sStartNo);

	if( nLoopCnt == 0 )
	{
		AfxMessageBox( _T("�����ȣ�� ������ �ʾҽ��ϴ�.") );
		return 1;
	}

	INT nMaxCnt = 1000;
	nLoopCnt = nLoopCnt/nMaxCnt + 1;
	INT nTmpStartNo , nTmpEndNo;
	nTmpStartNo = _ttoi(sStartNo);

	CString sControlStart, sControlEnd;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	pDM->m_nCols = 1;

	// ���α׷����� ����
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nLoopCnt);
	

	CString sData;
	for( INT i = 0 ; i < nLoopCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);
		

		if( i != 0 )
			nTmpStartNo = nTmpEndNo+1;

		if( nTmpStartNo + nMaxCnt > _ttoi(sEndNo) )
			nTmpEndNo = _ttoi(sEndNo);
		else
			nTmpEndNo = nTmpStartNo + nMaxCnt;

		sControlStart.Format ( _T("%s%09d") , sControlCode, nTmpStartNo );
		sControlEnd.Format ( _T("%s%09d") , sControlCode, nTmpEndNo );

		//++2008.10.14 UPDATE BY PWR {{++
		// ���սý��� ����
		sQuery.Format(	_T("SELECT DISTINCT CONTROL_NO FROM BO_SPECIES_TBL WHERE SUBSTR(CONTROL_NO,4,4)='%s' ")
						_T("AND CONTROL_NO BETWEEN '%s' AND '%s' AND MANAGE_CODE='%s' ")
						_T("UNION ")
						_T("SELECT DISTINCT CONTROL_NO FROM SE_SPECIES_TBL WHERE SUBSTR(CONTROL_NO,4,4)='%s' ")
						_T("AND CONTROL_NO BETWEEN '%s' AND '%s' AND MANAGE_CODE='%s' ")
						_T("ORDER BY CONTROL_NO") 
						, sYear, sControlStart, sControlEnd, m_pInfo->MANAGE_CODE
						, sYear, sControlStart, sControlEnd, m_pInfo->MANAGE_CODE
						);
		//sQuery.Format(	_T("SELECT DISTINCT CONTROL_NO FROM BO_SPECIES_TBL WHERE SUBSTR(CONTROL_NO,4,4)='%s' AND CONTROL_NO BETWEEN '%s' AND '%s' ")
		//				_T("UNION ")
		//				_T("SELECT DISTINCT CONTROL_NO FROM SE_SPECIES_TBL WHERE SUBSTR(CONTROL_NO,4,4)='%s' AND CONTROL_NO BETWEEN '%s' AND '%s' ")
		//				_T(" ORDER BY CONTROL_NO") 
		//					, sYear , sControlStart, sControlEnd , sYear , sControlStart , sControlEnd
		//				);
		//--2008.10.14 UPDATE BY PWR --}}
		TmpDM.RestructDataManager(sQuery);
		
		for( INT j = 0 ; j < TmpDM.GetRowCount() ; j++ )
		{
			TmpDM.GetCellData(j,0,sData);
			pDM->InsertRow(-1);
			INT ids = pDM->SetCellData(pDM->GetRowCount()-1,0,sData);
		}
		
	}

	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );
	
	return 0;

	EFS_END

	return -1;
}


/// �ִ� �����ȣ ��������
CString CBO_CAT_CONTROL_SEARCH_0100::GetMaxControlNo( CESL_DataMgr* pDM , CString sYear ,CString sControlCode )
{
	EFS_BEGIN

	CString sMaxControlNo;
	CString sSql;

	CString sBOMaxControlNo , sSEMaxControlNo , sTmpBOMax , sTmpSEMax;
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	sSql.Format( _T("SELECT MAX(CONTROL_NO) FROM BO_SPECIES_TBL WHERE SUBSTR(CONTROL_NO,4,4)='%s' ")
				 _T("AND SUBSTR(CONTROL_NO,0,3)='%s' AND MANAGE_CODE='%s'") 
				 , sYear, sControlCode, m_pInfo->MANAGE_CODE );
	//sSql.Format( _T("SELECT MAX(CONTROL_NO) FROM BO_SPECIES_TBL WHERE SUBSTR(CONTROL_NO,4,4)='%s' AND SUBSTR(CONTROL_NO,0,3)='%s'") , sYear , sControlCode );
	//--2008.10.14 UPDATE BY PWR --}}
	pDM->GetOneValueQuery(sSql,sBOMaxControlNo);

	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	sSql.Format( _T("SELECT MAX(CONTROL_NO) FROM SE_SPECIES_TBL WHERE SUBSTR(CONTROL_NO,4,4)='%s' ")
				 _T("AND SUBSTR(CONTROL_NO,0,3)='%s' AND MANAGE_CODE='%s'") 
				 , sYear, sControlCode, m_pInfo->MANAGE_CODE );
	//sSql.Format( _T("SELECT MAX(CONTROL_NO) FROM SE_SPECIES_TBL WHERE SUBSTR(CONTROL_NO,4,4)='%s' AND SUBSTR(CONTROL_NO,0,3)='%s'") , sYear , sControlCode );
	//--2008.10.14 UPDATE BY PWR --}}
	pDM->GetOneValueQuery(sSql,sSEMaxControlNo);
	
	if( sBOMaxControlNo.GetLength() > 3 )
		sTmpBOMax = sBOMaxControlNo.Mid(3);
	if( sSEMaxControlNo.GetLength() > 3 )
		sTmpSEMax = sSEMaxControlNo.Mid(3);

	if( _ttoi(sTmpBOMax) > _ttoi(sTmpSEMax) )
		sMaxControlNo = sTmpBOMax;
	else
		sMaxControlNo = sTmpSEMax;

	return sMaxControlNo;

	EFS_END

	return _T("");
}


// ���α׷����� ����
CLocProgressBarDlg* CBO_CAT_CONTROL_SEARCH_0100::CreateProgressBarDlg( INT nUpper )
{
	CLocProgressBarDlg* pProgressBar = new CLocProgressBarDlg(this);
	pProgressBar->Create( this );
	pProgressBar->ShowWindow( SW_SHOW );
	pProgressBar->InitRange(nUpper);
	
	return pProgressBar;
}

// ���α׷����� ����
INT CBO_CAT_CONTROL_SEARCH_0100::DeleteProgressBarDlg( CLocProgressBarDlg *pProgressBarDlg )
{
	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;
}

HBRUSH CBO_CAT_CONTROL_SEARCH_0100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
