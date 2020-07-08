// BO_CAT_PROC_0203_01.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0203_01.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0203_01 dialog


CBO_CAT_PROC_0203_01::CBO_CAT_PROC_0203_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0203_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0203_01::~CBO_CAT_PROC_0203_01()
{
}

VOID CBO_CAT_PROC_0203_01::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0203_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0203_01, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0203_01)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0203_01 message handlers

BOOL CBO_CAT_PROC_0203_01::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0203_01::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0203_01")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton * pPrint01;
	pPrint01=(CButton *)GetDlgItem(IDC_rLINE_PRINT);
	pPrint01->SetCheck(1);

	CButton * pPIECE_PAPER;
	pPIECE_PAPER=(CButton *)GetDlgItem(IDC_rPIECE_PAPER);
	pPIECE_PAPER->SetCheck(1);

	INT idx;
	SelectMakeList(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	if (idx < 0)
	{
		return TRUE;		
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}

	sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );
	idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));

	while ( idx != -1 ) {
		CString sRecKeyTmp = pDM->GetCellData( _T("BS_��KEY") , idx );
		sRecKey = sRecKey + _T(", ") + sRecKeyTmp;
		idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	}

	CESL_DataMgr * pDM_0203_01;
	pDM_0203_01 = FindDM( _T("DM_BO_CAT_PROC_0203_01") );
	if ( NULL == pDM_0203_01 ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0203_01 error") );
		return FALSE;
	}

	CString strWhere = _T("SPECIES_KEY IN(") + sRecKey + _T(")");
	if (pDM_0203_01->RefreshDataManager(strWhere) < 0)	return false;
	INT iRowCount = pDM_0203_01->GetRowCount();
	if ( pDM_0203_01->GetRowCount() < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}
	for ( INT i = pDM_0203_01->GetRowCount()-1 ; i >= 0 ; i-- ) {
		CString sRegNo = pDM_0203_01->GetCellData( _T("BB_��Ϲ�ȣ"), i );
		if ( sRegNo == _T("") ) {
			pDM_0203_01->DeleteRow( i );
		}
	}

	CString sLibraryName;
	CString sQuery = _T("SELECT VALUE FROM MN_LIBINFO_TBL WHERE VALUE_NAME ='��������'");
	pDM_0203_01->GetOneValueQuery( sQuery , sLibraryName );

	SetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("��������"), sLibraryName );

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0203_01::OnOK() 
{
EFS_BEGIN

	CString sRegCode;
	GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("��ϱ���_�ڵ�"), sRegCode );
	if ( sRegCode == _T("") ) {

	}

	CString sRegNoStart, sRegNoEnd;
	GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("��Ϲ�ȣ����"), sRegNoStart );
	GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("��Ϲ�ȣ����"), sRegNoEnd );
	
	if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
		
		sRegNoStart = m_pInfo->MakeRegNo(sRegCode, sRegNoStart );
		sRegNoEnd = m_pInfo->MakeRegNo(sRegCode, sRegNoEnd );		
	}

	CESL_DataMgr *m_pDM = FindDM( _T("DM_BO_CAT_PROC_0203_01") );
	if ( m_pDM == NULL ) {
		AfxMessageBox ( _T("finddm error") );
		return;
	}


	CButton *pLINE_PRINT;
	pLINE_PRINT = (CButton *)GetDlgItem(IDC_rLINE_PRINT);
	CButton *pSERIAL_PRINT;
	pSERIAL_PRINT = (CButton *)GetDlgItem(IDC_rSERIAL_PRINT);
	CButton *pBARCODE;
	pBARCODE = (CButton *)GetDlgItem(IDC_rBARCODE);

	if ( pLINE_PRINT->GetCheck() == 1 || pSERIAL_PRINT->GetCheck() == 1 ) {	//�ٹٲ����
		CESL_DataMgr *pDm0203;
		pDm0203 = FindDM(_T("DM_BO_CAT_PROC_0203"));
		if ( NULL == pDm0203 ) {
			AfxMessageBox ( _T("DM_BO_CAT_PROC_0203 error") );
			return;
		}

		CESL_DataMgr *pDmLabel;
		pDmLabel = FindDM(_T("DM_BO_CAT_PROC_0203_LABEL"));
		if ( NULL == pDmLabel ) {
			AfxMessageBox ( _T("DM_BO_CAT_PROC_0203_LABEL error") );
			return;
		}
		pDmLabel->FreeData();
		INT i0203Count = pDm0203->GetRowCount();
		INT iLabelCount = pDmLabel->GetRowCount();
		INT iCount = -1;
		if ( pLINE_PRINT->GetCheck() == 1 ) {			
			for ( INT i = 0 ; i < i0203Count ; i++ ) {
				CString sRegNo = pDm0203->GetCellData( _T("BB_��Ϲ�ȣ"), i );
				if ( sRegNoStart != _T("") && sRegNoEnd == _T("") ) {
					if ( sRegNo != sRegNoStart ) {
						continue;
					}
				}
				else if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
					if ( sRegNo < sRegNoStart || sRegNo > sRegNoEnd ) {
						continue;
					}
				}
				else if ( sRegNoStart == _T("") && sRegNoEnd != _T("") ) {
					AfxMessageBox( _T("���۵�Ϲ�ȣ�� �Է��ϼ���.") );
					return;
				}

				CString sLabelType = pDm0203->GetCellData( _T("BB_���̺���±���"), i );
				if ( sLabelType == _T("�ٹٲ����") ) {
					iCount ++;
					CString sShelfCode = pDm0203->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), i );
					CString sClassNo = pDm0203->GetCellData( _T("BB_�з���ȣ"), i );
					CString sBookCode = pDm0203->GetCellData( _T("BB_������ȣ"), i );
					CString sVolCode = pDm0203->GetCellData( _T("BB_��å��ȣ"), i );
					CString sCopyCode = pDm0203->GetCellData( _T("BB_������ȣ"), i );
					CString sRegNo = pDm0203->GetCellData( _T("BB_��Ϲ�ȣ"), i );
					CString sResult;
					CString sNewLine;
					if ( sShelfCode != _T("") ) {
						sResult = sShelfCode;
						sNewLine = _T("\r\n");
					}
					else {
						sNewLine = _T("\r\n");
					}
					if ( sClassNo != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("\r\n") + sClassNo;
						}
						else {
							sResult = sResult + sClassNo;
							sNewLine = sNewLine + _T("\r\n");
						}
					}
					else {
						sNewLine = sNewLine + _T("\r\n");
					}
					if ( sBookCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("\r\n") + sBookCode;
						}
						else {
							sResult = sResult + sBookCode;
							sNewLine = sNewLine + _T("\r\n");
						}
					}
					else {
						sNewLine = sNewLine + _T("\r\n");
					}
					if ( sVolCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("\r\n") + sVolCode;
						}
						else {
							sResult = sResult + sVolCode;
							sNewLine = sNewLine + _T("\r\n");
						}						
					}
					else {
						sNewLine = sNewLine + _T("\r\n");
					}
					if ( sCopyCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("\r\n") + sCopyCode;
						}
						else {
							sResult = sResult + sCopyCode;
							sNewLine = sNewLine + _T("\r\n");
						}						
					}
					else {
						sNewLine = sNewLine + _T("\r\n");
					}
					sResult = sResult + sNewLine + _T("\r\n") + sRegNo;
					
					INT iNod = iCount % 6;
					INT iMok =  iCount / 6 ;

					if ( iNod == 0 ) {
						pDmLabel->InsertRow(-1);
					}
					CString sNum;
					sNum.Format( _T("%d") , iNod+1 );
					CString sCallAlias = _T("BB_û����ȣ") + sNum;
					pDmLabel->SetCellData( sCallAlias, sResult, iMok );
				}
			}
		}
		else {			
			for ( INT i = 0 ; i < i0203Count ; i++ ) {

				CString sRegNo = pDm0203->GetCellData( _T("BB_��Ϲ�ȣ"), i );
				if ( sRegNoStart != _T("") && sRegNoEnd == _T("") ) {
					if ( sRegNo != sRegNoStart ) {
						continue;
					}
				}
				else if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
					if ( sRegNo < sRegNoStart || sRegNo > sRegNoEnd ) {
						continue;
					}
				}
				else if ( sRegNoStart == _T("") && sRegNoEnd != _T("") ) {
					AfxMessageBox( _T("���۵�Ϲ�ȣ�� �Է��ϼ���.") );
					return;
				}

				CString sLabelType = pDm0203->GetCellData( _T("BB_���̺���±���"), i );
				if ( sLabelType == _T("�̾����") ) {
					iCount ++;
					CString sShelfCode = pDm0203->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), i );
					CString sClassNo = pDm0203->GetCellData( _T("BB_�з���ȣ"), i );
					CString sBookCode = pDm0203->GetCellData( _T("BB_������ȣ"), i );
					CString sVolCode = pDm0203->GetCellData( _T("BB_��å��ȣ"), i );
					CString sCopyCode = pDm0203->GetCellData( _T("BB_������ȣ"), i );
					CString sRegNo = pDm0203->GetCellData( _T("BB_��Ϲ�ȣ"), i );
					CString sResult = sShelfCode;
						if ( sResult != _T("") ) {
							sResult = sResult + _T(" ") + sClassNo;
						}
						else {
							sResult = sClassNo;
						}
					if ( sBookCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("-") + sBookCode;
						}
						else {
							sResult = sBookCode;
						}
					}
					if ( sVolCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("-") + sVolCode;
						}						
					}
					if ( sCopyCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("=") + sCopyCode;
						}
						else {
							sResult = sCopyCode;
						}
					}

					INT iNod = (iCount+1) % 2;
					INT iMok = iCount / 2;

					if ( iNod == 1 ) {
						pDmLabel->InsertRow(-1);
						pDmLabel->SetCellData( _T("BB_û����ȣ3"), sRegNo, iMok );
					}
					if ( iNod == 0 ) {
						iNod = 2;
					}
					CString sNum;
					sNum.Format( _T("%d") , iNod );
					CString sCallAlias = _T("BB_û����ȣ") + sNum;
					pDmLabel->SetCellData( sCallAlias, sResult, iMok );
				}
			}
		}

		if ( iCount < 0 ) {
			AfxMessageBox( _T("����� �����Ͱ� �����ϴ�.") );
			return;
		}

		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
			return;
		}
		INT ids;
		pSIReportManager->SetCONNECTION_INFO(pDmLabel->CONNECTION_INFO);
		if ( pSERIAL_PRINT->GetCheck() == 1 ) {
			ids = pSIReportManager->GetSIReportFileFromDB(_T("����_�̾����_�ϼ�"));
		}
		else {
			ids = pSIReportManager->GetSIReportFileFromDB(_T("����_�ٹٲ����"));
		}
		
		if (ids < 0) {
			AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
			return;
		}

		pSIReportManager->SetDataMgr(0, pDmLabel);
		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}	
	}

	if ( pBARCODE->GetCheck() == 1 ) {
		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
			return;
		}
		INT ids;
		pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		ids = pSIReportManager->GetSIReportFileFromDB(_T("����-���ڵ����"));		
		
		if (ids < 0) {
			AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
			return;
		}
		
		CString sLibraryName;
		GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("��������"), sLibraryName );
		pSIReportManager->SetTempVariable(_T("%BARCODE���ڵ�����%"), sLibraryName);

		pSIReportManager->SetDataMgr(0, m_pDM);
		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}	
	}

	CButton *pBOOKCARD;
	pBOOKCARD = (CButton *)GetDlgItem(IDC_rBOOKCARD);
	CButton *pSTICKER;
	pSTICKER = (CButton *)GetDlgItem(IDC_rSTICKER);
	if ( pBOOKCARD->GetCheck() == 1 || pSTICKER->GetCheck() == 1 ) {
		CString sPrintCount;
		GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("����"), sPrintCount );
		INT iPrintCount = _ttoi ( sPrintCount );
		if ( iPrintCount > 4 || sPrintCount == _T("") ) {
			AfxMessageBox( _T("������ 4 �����Դϴ�.") );
			return;
		}
		CESL_DataMgr *pDm0203;
		pDm0203 = FindDM(_T("DM_BO_CAT_PROC_0203"));
		if ( NULL == pDm0203 ) {
			AfxMessageBox ( _T("DM_BO_CAT_PROC_0203 error") );
			return;
		}

		CESL_DataMgr *pDmLabel;
		pDmLabel = FindDM(_T("DM_BO_CAT_PROC_0203_LABEL"));
		if ( NULL == pDmLabel ) {
			AfxMessageBox ( _T("DM_BO_CAT_PROC_0203_LABEL error") );
			return;
		}
		pDmLabel->FreeData();
		INT i0203Count = pDm0203->GetRowCount();
		INT iLabelCount = pDmLabel->GetRowCount();
		INT iCount = -1;
		for ( INT i = 0 ; i < i0203Count ; i++ ) {
			
			CString sRegNo = pDm0203->GetCellData( _T("BB_��Ϲ�ȣ"), i );
			if ( sRegNoStart != _T("") && sRegNoEnd == _T("") ) {
				if ( sRegNo != sRegNoStart ) {
					continue;
				}
			}
			else if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
				if ( sRegNo < sRegNoStart || sRegNo > sRegNoEnd ) {
					continue;
				}
			}
			else if ( sRegNoStart == _T("") && sRegNoEnd != _T("") ) {
				AfxMessageBox( _T("���۵�Ϲ�ȣ�� �Է��ϼ���.") );
				return;
			}

			iCount ++;
			CString sShelfCode = pDm0203->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), i );
			CString sClassNo = pDm0203->GetCellData( _T("BB_�з���ȣ"), i );
			CString sBookCode = pDm0203->GetCellData( _T("BB_������ȣ"), i );
			CString sVolCode = pDm0203->GetCellData( _T("BB_��å��ȣ"), i );
			CString sCopyCode = pDm0203->GetCellData( _T("BB_������ȣ"), i );
			sRegNo = pDm0203->GetCellData( _T("BB_��Ϲ�ȣ"), i );
			CString sResult;

			CString sNewLine;
			if ( sShelfCode != _T("") ) {
				sResult = sShelfCode;
				sNewLine = _T("\r\n");
			}
			else {
				sNewLine = _T("\r\n");
			}
			if ( sClassNo != _T("") ) {
				if ( sResult != _T("") ) {
					sResult = sResult + _T("\r\n") + sClassNo;
				}
				else {
					sResult = sResult + sClassNo;// + _T("\r\n");
					sNewLine = sNewLine + _T("\r\n");
				}
			}
			else {
				sNewLine = sNewLine + _T("\r\n");
			}
			if ( sBookCode != _T("") ) {
				if ( sResult != _T("") ) {
					sResult = sResult + _T("\r\n") + sBookCode;
				}
				else {
					sResult = sResult + sBookCode;
					sNewLine = sNewLine + _T("\r\n");
				}
			}
			else {
				sNewLine = sNewLine + _T("\r\n");
			}
			if ( sVolCode != _T("") ) {
				if ( sResult != _T("") ) {
					sResult = sResult + _T("\r\n") + sVolCode;
				}
				else {
					sResult = sResult + sVolCode;
					sNewLine = sNewLine + _T("\r\n");
				}						
			}
			else {
				sNewLine = sNewLine + _T("\r\n");
			}
			if ( sCopyCode != _T("") ) {
				if ( sResult != _T("") ) {
					sResult = sResult + _T("\r\n") + sCopyCode;
				}
				else {
					sResult = sResult + sCopyCode;
					sNewLine = sNewLine + _T("\r\n");
				}						
			}
			else {
				sNewLine = sNewLine + _T("\r\n");
			}
			sResult = sResult + sNewLine;

			CString sTitle = pDm0203->GetCellData( _T("IBS_��ǥ��"), i );
			sTitle = _T("��  �� : ") + sTitle;
			INT iTitleLen = sTitle.GetLength();
			if ( iTitleLen > 37 ) {
				sTitle = sTitle.Left(36);
			}
			CString sAuthor = pDm0203->GetCellData( _T("IBS_������"), i );
			sAuthor = _T("��  �� : ") + sAuthor;
			sResult = sResult + _T("\r\n\r\n\r\n") + sTitle + _T("\r\n\r\n") + sAuthor;
			sResult = sResult + _T("\r\n") + _T("               ") + sRegNo;
			if ( sRegNo == _T("") ) {
				sResult = sResult + _T("\r\n");
			}

			INT iNod = iCount % iPrintCount;
			INT iMok =  iCount / iPrintCount ;

			if ( iNod == 0 ) {
				pDmLabel->InsertRow(-1);
			}
			CString sNum;
			sNum.Format( _T("%d") , iNod+1 );
			CString sCallAlias = _T("BB_û����ȣ") + sNum;
			pDmLabel->SetCellData( sCallAlias, sResult, iMok );
		}

		if ( iCount < 0 ) {
			AfxMessageBox( _T("����� �����Ͱ� �����ϴ�.") );
			return;
		}

		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
			return;
		}
		INT ids;
		pSIReportManager->SetCONNECTION_INFO(pDmLabel->CONNECTION_INFO);
		if ( pBOOKCARD->GetCheck() == 1 ) {
			ids = pSIReportManager->GetSIReportFileFromDB(_T("����_��ī��"));
		}
		else {
			ids = pSIReportManager->GetSIReportFileFromDB(_T("����_��ƼĿ"));
		}
		
		if (ids < 0) {
			AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
			return;
		}

		pSIReportManager->SetDataMgr(0, pDmLabel);
		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}
	}

EFS_END
}

INT CBO_CAT_PROC_0203_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

EFS_END
return -1;

}
HBRUSH CBO_CAT_PROC_0203_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
