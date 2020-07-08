// UUIM_Detail.cpp : implementation file
//

#include "stdafx.h"
#include "UnionUserInfoManager.h"
#include "UUIM_Detail.h"
#include "imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUUIM_Detail dialog


CUUIM_Detail::CUUIM_Detail(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUUIM_Detail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGrid = NULL;
	m_pUserDM = NULL;
	m_pDuplicateDM = NULL;
}

CUUIM_Detail::~CUUIM_Detail()
{
}

void CUUIM_Detail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUUIM_Detail)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUUIM_Detail, CDialog)
	//{{AFX_MSG_MAP(CUUIM_Detail)
	ON_BN_CLICKED(IDC_btnDetailPrev, OnbtnDetailPrev)
	ON_BN_CLICKED(IDC_btnDetailNext, OnbtnDetailNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CUUIM_Detail, CDialog)
    //{{AFX_EVENTSINK_MAP(CUUIM_Detail)
	ON_EVENT(CUUIM_Detail, IDC_gridMAINDetail, -600 /* DblClick */, OnClickgridMain, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUUIM_Detail message handlers

BOOL CUUIM_Detail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_UUIM_Detail")) < 0)
 	{
 		AfxMessageBox(_T("InitESL_Mgr Error : SM_UUIM_Detail"));
 		return FALSE;
 	}

	m_pParentDM = FindDM(_T("DM_UUIM"));
	m_pUserDM = FindDM(_T("DM_UUIM_Detail"));
	m_pDuplicateDM = FindDM(_T("DM_UUIM_Detail_Duplicate"));
	m_pParentGrid = (CESL_Grid*)FindControl(_T("CM_UUIM"), _T("�׸���"));
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_UUIM_Detail"), _T("�׸���"));

 	Refresh(m_nIdx);
 	ShowHidePrevNextBtn(m_nIdx);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CUUIM_Detail::Refresh(CString strUserKey)
{
	strUserKey.TrimLeft();
	strUserKey.TrimRight();
	if(FALSE == strUserKey.IsEmpty())
	{
		CString strWhere;
		strWhere.Format(_T("CLU.REC_KEY = %s"), strUserKey);
		m_pUserDM->RefreshDataManager(strWhere);

		CString strCivilNo;
		m_pUserDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), 0, strCivilNo);
		
		strWhere.Format(_T("CLU.CIVIL_NO = '%s'"), strCivilNo);
		m_pDuplicateDM->RefreshDataManager(strWhere);
		
		CString civilNum, sIndex;
		TCHAR cNum;	
		INT	i = 0 , nLength = 0;
		
		const INT ROW_CNT = m_pDuplicateDM->GetRowCount();
		for(INT nRow = 0; nRow < ROW_CNT; nRow++)
		{
			m_pDuplicateDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), nRow, civilNum);
			nLength = civilNum.GetLength();
			for( i = 0 ; i<nLength ; i++ )
			{
				cNum = civilNum.GetAt( i );
				if( 47 < cNum && 58 > cNum )
				{
					sIndex += cNum;
				}
			}	
			if( 13 == sIndex.GetLength())
			{
				sIndex = sIndex.Left( 6 ) + _T("-*******");
			}
			if(FALSE == civilNum.IsEmpty())
			{
				m_pDuplicateDM->SetCellData(_T("�ֹε�Ϲ�ȣ_ȭ��"), sIndex, nRow);
			}
			sIndex.Empty();
		}
		
		AllControlDisplay(_T("CM_UUIM_Detail"), 0);

		const INT PASSWORD_EDITBOX_CNT = 4;
		CESL_ControlEdit* pEdit = NULL;		
		CString strAlias[PASSWORD_EDITBOX_CNT] = 
		{
			_T("��й�ȣ"), _T("��й�ȣȮ��"), _T("ȸ������й�ȣ"), _T("�ֹε�Ϲ�ȣ2")
		};

		for(INT nCnt = 0; nCnt < PASSWORD_EDITBOX_CNT; nCnt++)
		{
			pEdit = (CESL_ControlEdit*)FindControl(_T("CM_UUIM_Detail"), strAlias[nCnt]);
			if(NULL != pEdit)
			{
 				pEdit->SetPasswordChar('*');
				pEdit->Invalidate();
			}
		}

		SetCivilNum();

		CString sGetData;
		// ���� Ÿ�� ���� ����
		GetDataMgrData(_T("DM_UUIM_Detail"), _T("����TYPE") , sGetData , 0 );
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbDetailBirthdayType);
		CString sStr;		
		for( i=0 ; i<pCombo->GetCount() ; i++ )
		{
			pCombo->GetLBText(i,sStr);
			if( 0 == sStr.Compare(sGetData) )
			{
				pCombo->SetCurSel(i);
			}
		}
		
		/// ȸ���� �н� �����ֱ�
		GetDataMgrData( _T("DM_UUIM_Detail") , _T("ȸ�����н�") , sGetData , 0 );
		
		if( TRUE == sGetData.IsEmpty() ) 
		{
			sGetData = _T("N");
		}
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cmbDetailLoadCard);
		
		for( i=0 ; i<pCombo->GetCount() ; i++ )
		{
			pCombo->GetLBText(i,sStr);
			if( 0 == sStr.Compare(sGetData) )
			{
				pCombo->SetCurSel(i);
			}
		}
		
		/// 2009.02.02 ADD BY PWR : ���δ��������߰�
		GetDataMgrData( _T("DM_UUIM_Detail") , _T("���δ�������") , sGetData , 0 );
		if( TRUE == sGetData.IsEmpty() ) 
		{
			sGetData = _T("N");
		}
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cmbDetailSelfLoanLimit);
		for( i=0 ; i<pCombo->GetCount() ; i++ )
		{
			pCombo->GetLBText(i,sStr);
			if( 0 == sStr.Compare(sGetData) )
			{
				pCombo->SetCurSel(i);
			}
		}

		INT nCmbIdx = 0;
		GetDataMgrData( _T("DM_UUIM_Detail") , _T("�ҷ�ȸ������") , sGetData , 0 );
		pCombo = (CComboBox*)GetDlgItem(IDC_cmbDetailUserState);
		if(_T("����ȸ��") == sGetData || _T("0") == sGetData)
		{
			nCmbIdx = 0;
		}
		else if(_T("��������") == sGetData || _T("1") == sGetData)
		{
			nCmbIdx = 1;
		}
		else if(_T("����ȸ��") == sGetData || _T("2") == sGetData)
		{
			nCmbIdx = 2;
		}		
		else if(_T("Ż��ȸ��") == sGetData || _T("3") == sGetData)
		{
			nCmbIdx = 3;
		}	
		pCombo->SetCurSel(nCmbIdx);

		nCmbIdx = 0;
		GetDataMgrData( _T("DM_UUIM_Detail") , _T("����߼�����") , sGetData , 0 );
		pCombo = (CComboBox*)GetDlgItem(IDC_cmbFileUSER3);
		if(_T("����") == sGetData || _T("H") == sGetData)
		{
			nCmbIdx = 0;
		}
		else if(_T("�ٹ���") == sGetData || _T("W") == sGetData)
		{
			nCmbIdx = 1;
		}
		pCombo->SetCurSel(nCmbIdx);

		CString strUserKey2;
		const int ROW_COUNT = m_pDuplicateDM->GetRowCount();
		for(nCnt = 0; nCnt < ROW_COUNT; nCnt++)
		{
			m_pDuplicateDM->GetCellData(_T("REC_KEY"), nCnt, strUserKey2);
			if(strUserKey2 == strUserKey)
			{
				m_pGrid->SetReverse(nCnt);
			}
		}
	}

	return 0;
}

INT CUUIM_Detail::Refresh(INT nIdx)
{
	if(0 > m_nIdx)
	{
		m_nIdx = 0;
		return 1000;
	}
	if(m_pParentDM->GetRowCount() <= m_nIdx)
	{
		m_nIdx = m_pParentDM->GetRowCount() - 1;
		return 1010;
	}
	CString strUserKey;
	m_pParentDM->GetCellData(_T("REC_KEY"), nIdx, strUserKey);

	strUserKey.TrimLeft();
	strUserKey.TrimRight();
	if(FALSE == strUserKey.IsEmpty())
	{
		CString strWhere;
		strWhere.Format(_T("CLU.REC_KEY = %s"), strUserKey);
		m_pUserDM->RefreshDataManager(strWhere);

		CString strCivilNo;
		m_pParentDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), nIdx, strCivilNo);
		
//		strWhere.Format(_T("CLU.CIVIL_NO = '%s' AND CLU.REC_KEY <> %s"), strCivilNo, strUserKey);
		strWhere.Format(_T("CLU.CIVIL_NO = '%s'"), strCivilNo);
		m_pDuplicateDM->RefreshDataManager(strWhere);
		
		CString civilNum, sIndex;
		TCHAR cNum;	
		INT	i = 0 , nLength = 0;
		
		const INT ROW_CNT = m_pDuplicateDM->GetRowCount();
		for(INT nRow = 0; nRow < ROW_CNT; nRow++)
		{
			m_pDuplicateDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), nRow, civilNum);
			nLength = civilNum.GetLength();
			for( i = 0 ; i<nLength ; i++ )
			{
				cNum = civilNum.GetAt( i );
				if( 47 < cNum && 58 > cNum )
				{
					sIndex += cNum;
				}
			}	
			if( 13 == sIndex.GetLength())
			{
				sIndex = sIndex.Left( 6 ) + _T("-*******");
			}
			if(FALSE == civilNum.IsEmpty())
			{
				m_pDuplicateDM->SetCellData(_T("�ֹε�Ϲ�ȣ_ȭ��"), sIndex, nRow);
			}
			sIndex.Empty();
		}
		
		m_pParentGrid->SetReverse(nIdx);
		AllControlDisplay(_T("CM_UUIM_Detail"), 0);

		const INT PASSWORD_EDITBOX_CNT = 4;
		CESL_ControlEdit* pEdit = NULL;		
		CString strAlias[PASSWORD_EDITBOX_CNT] = 
		{
			_T("��й�ȣ"), _T("��й�ȣȮ��"), _T("ȸ������й�ȣ"), _T("�ֹε�Ϲ�ȣ2")
		};

		for(INT nCnt = 0; nCnt < PASSWORD_EDITBOX_CNT; nCnt++)
		{
			pEdit = (CESL_ControlEdit*)FindControl(_T("CM_UUIM_Detail"), strAlias[nCnt]);
			if(NULL != pEdit)
			{
 				pEdit->SetPasswordChar('*');
				pEdit->Invalidate();
			}
		}

		SetCivilNum();

		CString sGetData;
		// ���� Ÿ�� ���� ����
		GetDataMgrData(_T("DM_UUIM_Detail"), _T("����TYPE") , sGetData , 0 );
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbDetailBirthdayType);
		CString sStr;		
		for( i=0 ; i<pCombo->GetCount() ; i++ )
		{
			pCombo->GetLBText(i,sStr);
			if( 0 == sStr.Compare(sGetData) )
			{
				pCombo->SetCurSel(i);
			}
		}
		
		/// ȸ���� �н� �����ֱ�
		GetDataMgrData( _T("DM_UUIM_Detail") , _T("ȸ�����н�") , sGetData , 0 );
		
		if( TRUE == sGetData.IsEmpty() ) 
		{
			sGetData = _T("N");
		}
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cmbDetailLoadCard);
		
		for( i=0 ; i<pCombo->GetCount() ; i++ )
		{
			pCombo->GetLBText(i,sStr);
			if( 0 == sStr.Compare(sGetData) )
			{
				pCombo->SetCurSel(i);
			}
		}
		
		/// 2009.02.02 ADD BY PWR : ���δ��������߰�
		GetDataMgrData( _T("DM_UUIM_Detail") , _T("���δ�������") , sGetData , 0 );
		if( TRUE == sGetData.IsEmpty() ) 
		{
			sGetData = _T("N");
		}
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cmbDetailSelfLoanLimit);
		for( i=0 ; i<pCombo->GetCount() ; i++ )
		{
			pCombo->GetLBText(i,sStr);
			if( 0 == sStr.Compare(sGetData) )
			{
				pCombo->SetCurSel(i);
			}
		}

		INT nCmbIdx = 0;
		GetDataMgrData( _T("DM_UUIM_Detail") , _T("�ҷ�ȸ������") , sGetData , 0 );
		pCombo = (CComboBox*)GetDlgItem(IDC_cmbDetailUserState);
		if(_T("����ȸ��") == sGetData || _T("0") == sGetData)
		{
			nCmbIdx = 0;
		}
		else if(_T("��������") == sGetData || _T("1") == sGetData)
		{
			nCmbIdx = 1;
		}
		else if(_T("����ȸ��") == sGetData || _T("2") == sGetData)
		{
			nCmbIdx = 2;
		}		
		else if(_T("Ż��ȸ��") == sGetData || _T("3") == sGetData)
		{
			nCmbIdx = 3;
		}	
		pCombo->SetCurSel(nCmbIdx);

		nCmbIdx = 0;
		GetDataMgrData( _T("DM_UUIM_Detail") , _T("����߼�����") , sGetData , 0 );
		pCombo = (CComboBox*)GetDlgItem(IDC_cmbFileUSER3);
		if(_T("����") == sGetData || _T("H") == sGetData)
		{
			nCmbIdx = 0;
		}
		else if(_T("�ٹ���") == sGetData || _T("W") == sGetData)
		{
			nCmbIdx = 1;
		}
		pCombo->SetCurSel(nCmbIdx);

		m_nIdx = nIdx;

		CString strUserKey2;
		const int ROW_COUNT = m_pDuplicateDM->GetRowCount();
		for(nCnt = 0; nCnt < ROW_COUNT; nCnt++)
		{
			m_pDuplicateDM->GetCellData(_T("REC_KEY"), nCnt, strUserKey2);
			if(strUserKey2 == strUserKey)
			{
				m_pGrid->SetReverse(nCnt);
			}
		}
	}

	return 0;
}

void CUUIM_Detail::OnbtnDetailPrev() 
{
	// TODO: Add your control notification handler code here	
	Refresh(--m_nIdx);
	ShowHidePrevNextBtn(m_nIdx);
}

void CUUIM_Detail::OnbtnDetailNext() 
{
	// TODO: Add your control notification handler code here

	Refresh(++m_nIdx);
	ShowHidePrevNextBtn(m_nIdx);
}

INT CUUIM_Detail::ShowHidePrevNextBtn(INT nIdx)
{
	if(0 >= nIdx)
	{
		GetDlgItem(IDC_btnDetailPrev)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_btnDetailPrev)->EnableWindow(TRUE);
	}

	if(m_pParentDM->GetRowCount() - 1 <= nIdx)
	{
		GetDlgItem(IDC_btnDetailNext)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_btnDetailNext)->EnableWindow(TRUE);
	}

	return 0;
}

void CUUIM_Detail::SetCivilNum()
{
	CString	sCivilNo;
	CString	strTemp;
	CString	strMessage;

	INT	nLength	=	0;
	INT	ids = GetControlData(_T("CM_UUIM_Detail"), _T("�ֹε�Ϲ�ȣ"), sCivilNo);
	if(0 > ids) 
	{
		return;
	}

	strTemp	= sCivilNo;

	sCivilNo.Replace(_T("-"),_T(""));

	//�ֹι�ȣ �ڸ��� Check, 13�ڸ��� �ƴϸ� ���̿� ���� ó���� �ٸ���
	nLength = sCivilNo.GetLength();
	if(13 != nLength)
	{
		//���� �Ǿ� �ִ� �ֹι�ȣ�� 6�ڸ� ���� �� ���
		if( 6 >= nLength )
		{
			//'IDC_edtDetailCivilNo1'�� ����
			GetDlgItem(IDC_edtDetailCivilNo1)->SetWindowText(sCivilNo);
			GetDlgItem(IDC_edtDetailCivilNo2)->SetWindowText(_T(""));
			return;
		}
		//���� �Ǿ� �ִ� �ֹι�ȣ�� 7�ڸ� �̻�, 12�ڸ� ���� �� ���
		else if( 7 <= nLength && 12 >= nLength )
		{
			// 6�ڸ����� 'IDC_edtDetailCivilNo1'�� ����
			GetDlgItem(IDC_edtDetailCivilNo1)->SetWindowText( sCivilNo.Left(6) );
			// ������ 'IDC_edtDetailCivilNo2'�� ����
			GetDlgItem(IDC_edtDetailCivilNo2)->SetWindowText( sCivilNo.Right(nLength-6) );
			return;
		}
		//���� �Ǿ� �ִ� �ֹι�ȣ�� 13�ڸ� �ʰ� �� ���
		else if( 13 < nLength )
		{
			// 6�ڸ����� 'IDC_edtDetailCivilNo1'�� ����
			GetDlgItem(IDC_edtDetailCivilNo1)->SetWindowText( sCivilNo.Left(6) );
			// �� ���� 7�ڸ����� 'IDC_edtDetailCivilNo2'�� ����
			GetDlgItem(IDC_edtDetailCivilNo2)->SetWindowText( sCivilNo.Mid(6,7) );
			return;
		}
	}

	GetDlgItem(IDC_edtDetailCivilNo1)->SetWindowText( sCivilNo.Left(6) );
	GetDlgItem(IDC_edtDetailCivilNo2)->SetWindowText( sCivilNo.Right(7) );
}

void CUUIM_Detail::OnClickgridMain()
{
	INT nRow = m_pGrid->GetRow()-1;
//	m_pGrid->SetReverse(nRow);	
	CString strRecKey, strTemp;
	m_pDuplicateDM->GetCellData(_T("REC_KEY"), nRow, strRecKey);

	Refresh(strRecKey);


/*
	if(FALSE == strRecKey.IsEmpty())
	{
		// �̹� ���� �׸��� ��Ͽ� �ִ� ���
		const INT ROW_COUNT = m_pParentDM->GetRowCount();
		for (INT nRowCnt = 0 ; nRowCnt < ROW_COUNT; nRowCnt++ )
		{
			strTemp = m_pParentDM->GetCellData(_T("REC_KEY"), nRowCnt);
			if ( strRecKey == strTemp )
			{
				m_pParentGrid->SetReverse(nRowCnt);
				Refresh(nRowCnt);
				ShowHidePrevNextBtn(nRowCnt);
				return;
			}
		}

		// �׸��忡 ���� ���
		CESL_DataMgr TmpDM;
		TmpDM.InitDMPKFromFile(_T("DM_UUIM"));
		TmpDM.SetCONNECTION_INFO(_T(""));

		CString strWhere;
		strWhere.Format(_T("CLU.REC_KEY = %s"), strRecKey);
		TmpDM.RefreshDataManager(strWhere);

		INT nRow = TmpDM.GetRowCount();
		if(0 == nRow) return;

		INT nCurRow;
		INT row, col;
		CString strTmp;

		for (row = 0; row < nRow; row++) {			
			m_pParentDM->InsertRow(-1);
			nCurRow = m_pParentDM->GetRowCount() - 1;
			for (col = 0; col < TmpDM.m_nCols; col++) {
				m_pParentDM->SetCellData(nCurRow, col, TmpDM.GetCellData(row, col));
			}
		}
		m_pParentGrid->Display();
		INT nLastRow = m_pParentDM->GetRowCount() -1;
		m_pParentGrid->SetReverse(nLastRow);
		Refresh(nLastRow);
		ShowHidePrevNextBtn(nLastRow);
	}
*/
}