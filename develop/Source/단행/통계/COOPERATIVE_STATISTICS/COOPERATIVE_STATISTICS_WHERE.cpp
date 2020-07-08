// COOPERATIVE_STATISTICS_WHERE.cpp : implementation file
//

#include "stdafx.h"
#include "COOPERATIVE_STATISTICS_WHERE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_WHERE dialog


CCOOPERATIVE_STATISTICS_WHERE::CCOOPERATIVE_STATISTICS_WHERE(CESL_Mgr* pParent, CStaticsInfoMgr *pSInfoMgr)
	: CESL_Mgr(CCOOPERATIVE_STATISTICS_WHERE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCOOPERATIVE_STATISTICS_WHERE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSInfoDM = pSInfoMgr->m_pStaticsMainInfoDM;
	m_pSInfoMgr = pSInfoMgr;

}


void CCOOPERATIVE_STATISTICS_WHERE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOOPERATIVE_STATISTICS_WHERE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCOOPERATIVE_STATISTICS_WHERE, CDialog)
	//{{AFX_MSG_MAP(CCOOPERATIVE_STATISTICS_WHERE)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_WHERE message handlers

BOOL CCOOPERATIVE_STATISTICS_WHERE::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CCOOPERATIVE_STATISTICS_WHERE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;
	ids = InitESL_Mgr( _T("COOPERATIVE_STATISTICS_WHERE") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	m_pParentCM = FindCM( _T("CM_COOPERATIVE_STATISTICS_MAIN") );
	if( m_pParentCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	m_pSInfoMgr->SetSearchCM(FindCM(_T("CM_COOPERATIVE_STATISTICS_WHERE")));

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE; 
}

INT CCOOPERATIVE_STATISTICS_WHERE::SetControlSetting()
{
	INT ids;
	CString sStaticsClass;
	ids = m_pParentCM->GetControlMgrData( _T("�������") , sStaticsClass , -1 , 0 );
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("SetControlDisable") );
	CESL_ControlMgr *pCM = FindCM( _T("CM_COOPERATIVE_STATISTICS_WHERE") );
	if(pCM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("SetControlDisable") );

	// Disable
	
	// ����/�ݳ� ��� �߰�
	const INT nDateCnt = 8;
	CESL_Control_Element *pCEtmp;
	pCEtmp = (CESL_Control_Element*)pCM->FindControlElement(_T("��������"));

	CString sDateCtrlAlias[nDateCnt] =
	{
		_T("��û��1"), _T("��û��2"), 
		_T("������1"), _T("������2"),
		_T("å�η�������1"), _T("å�η�������2"), 
		_T("å�η��ݳ���1"), _T("å�η��ݳ���2")
//		_T("��û��1"), _T("��û��2"), 
//		_T("������1"), _T("������2")
	};

	for( INT i = 0 ; i < nDateCnt ; i++ )
	{
		CESL_Control_Element *pCE;
		pCE = (CESL_Control_Element*)pCM->FindControlElement( sDateCtrlAlias[i] );
			
		if(sStaticsClass == _T("�����ڷ����(�߼�)"))
		{
			if(i==2 || i==3)
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			else
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
				
			SetDlgItemText(IDC_staLoanDate, _T("��������"));


			// üũ ������ ��¥ ��Ʈ�� �ٽ� üũ���·� ����
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
 			const INT nCnt = 2;

			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
 			};

			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}
		
		// ��û�ڷ����(�Լ�) ����, ��û�ڷ����->��û�ڷ����� ��Ī ����
		else if(sStaticsClass == _T("��û�ڷ����(�Լ�)"))
		{
			//�Լ����� üũ ����
 			SYSTEMTIME systime;	
 			CDateTimeCtrl* pTimeCtrl = NULL;
 			const INT nCnt = 2;
 			
 			INT nIDC[nCnt] =
 			{
 				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
 			};
 			
 			for( INT j = 0 ; j < nCnt ; j++ )
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&systime);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd , GDT_NONE , &systime );
			}
			///////////////////////////////////////
			if(i==0 || i==1 || i==4 || i==5)
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			else
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);

			SetDlgItemText(IDC_staLoanDate, _T("�Լ�����"));
		}
// 		else if(sStaticsClass == _T("��û�ڷ����(�Լ�)"))
// 		{
// 			if(i==0 || i==1 )
// 				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
// 			else
// 				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
// 		}
		
		
		// ����/�ݳ� ��� �߰�
		else if(sStaticsClass == _T("�����ڷ����"))
		{
			if(i==4 || i==5)
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
			}
			
			SetDlgItemText(IDC_staLoanDate, _T("��������"));


			// üũ ������ ��¥ ��Ʈ�� �ٽ� üũ���·� ����
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
			const INT nCnt = 2;
			
			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
			};
			
			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}
		else if(sStaticsClass == _T("�ݳ��ڷ����"))
		{
			if(i==6 || i==7)
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
			}
			
			SetDlgItemText(IDC_staLoanDate, _T("��������"));


			// üũ ������ ��¥ ��Ʈ�� �ٽ� üũ���·� ����
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
			const INT nCnt = 2;
			
			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
			};
			
			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}


		// ������/�ݳ��� ��� �߰�
		else if(sStaticsClass == _T("���������"))
		{
			if(i==4 || i==5)
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
			}
			
			SetDlgItemText(IDC_staLoanDate, _T("��������"));


			// üũ ������ ��¥ ��Ʈ�� �ٽ� üũ���·� ����
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
			const INT nCnt = 2;
			
			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
			};
			
			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}
		else if(sStaticsClass == _T("�ݳ������"))
		{
			if(i==6 || i==7)
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
		}
		
			SetDlgItemText(IDC_staLoanDate, _T("��������"));


			// üũ ������ ��¥ ��Ʈ�� �ٽ� üũ���·� ����
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
			const INT nCnt = 2;
			
			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
			};
			
			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}

		else
		{
			GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
			
			SetDlgItemText(IDC_staLoanDate, _T("��������"));


			// üũ ������ ��¥ ��Ʈ�� �ٽ� üũ���·� ����
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
			const INT nCnt = 2;
			
			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
			};
			
			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}	
	} 

	// ������� (Default:0009(�Ϸ���ڷ�))
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSTATE);
	pCombo->SetCurSel(4);


	return 0;
}

// ** ��������
INT CCOOPERATIVE_STATISTICS_WHERE::SaveCondition()
{
EFS_BEGIN

	INT ids;

	// 1. ��Ʈ�ѿ��� SQL�� ����
	CString sSearchCond;
	ids = CMToSql( sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveCondition") );

	if( sSearchCond.IsEmpty() )
	{
		AfxMessageBox( _T("������ �Է����ֽʽÿ�") );
		return 1;
	}

	// 2. DM�� ���ǵ��� �����ϱ�
	CString sStaticsClass;
	ids = m_pParentCM->GetControlMgrData( _T("�������") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveCondition") );

	ids = m_pSInfoMgr->SetUserCondition( sStaticsClass , sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SaveCondition") );

	return 0;

EFS_END
return -1;
}

// Control���� WHERE�� ����
INT CCOOPERATIVE_STATISTICS_WHERE::CMToSql( CString &sSearchCond )
{
EFS_BEGIN

	INT ids;

	CString sStaticsClass;
	ids = m_pParentCM->GetControlMgrData( _T("�������"), sStaticsClass, -1, 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("SaveCondition") );

	CESL_ControlMgr *pCM = FindCM( _T("CM_COOPERATIVE_STATISTICS_WHERE") );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMToSql") );
	
	INT nControlCnt = pCM->Control_List.GetCount();
	CString sData;

	CESL_Control_Element *pCE;
	POSITION pos;
	CString sCtrlType;
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		pos = pCM->Control_List.FindIndex(i);
		pCE = (CESL_Control_Element*)pCM->Control_List.GetAt(pos);

		sCtrlType = pCE->CTRL_TYPE;
		sCtrlType.MakeUpper();
		if( sCtrlType == 'S' ) continue;

		CString sAlias = pCE->CTRL_ALIAS;
		ids = pCM->GetControlMgrData( sAlias, sData, -1, 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMToSql") );
		sData.TrimLeft(); sData.TrimRight();

		if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() )
			sData = _T("");

		if( sData.IsEmpty() ) continue;

		if( !sSearchCond.IsEmpty() ) sSearchCond += _T(",") ;

		if(sAlias == _T("��������"))
		{
			
			// ����/�ݳ� ��� �߰�
			if(sStaticsClass==_T("�����ڷ����(�߼�)"))
			{
				sAlias = _T("������������");
			}
			
			// ��Ī ����(��û�ڷ����->��û�ڷ����)
			else if(sStaticsClass==_T("��û�ڷ����(�Լ�)") || sStaticsClass==_T("�����ڷ����") || sStaticsClass==_T("���������") )
//			else if(sStaticsClass==_T("��û�ڷ����(�Լ�)") || sStaticsClass==_T("�����ڷ����"))
			{
				sAlias = _T("��û��������");
			}
			
			// �ݳ��� ��� ����� �̵�
			else if(sStaticsClass==_T("�ݳ��ڷ����") || sStaticsClass==_T("�ݳ������") )
			{
				sAlias = _T("�ݳ���������");
			}
//			if(sStaticsClass==_T("�����ڷ����(�߼�)"))
//				sAlias = _T("������������");
//			else if(sStaticsClass==_T("��û�ڷ����(�Լ�)"))
//				sAlias = _T("��û��������");
			else
				sAlias = _T("��������");
		}
		sSearchCond += sAlias + _T("=") + sData;
	}

	// ��¥ ��Ʈ�� ����ϱ�
	UpdateData(FALSE);


	// ����/�ݳ� ��� �߰�
	const INT nDateCnt = 9;
//	const INT nDateCnt = 5;


	// �Լ����� �ݿ��ϵ��� ����
	CString strColumnName1=_T("");
	CString strColumnName2=_T("");
	
	// ��Ī����(��û�ڷ����->��û�ڷ����)
	if(sStaticsClass==_T("��û�ڷ����(�Լ�)"))
//	if(sStaticsClass==_T("��û�ڷ����(�Լ�)"))
	{
		strColumnName1=_T("�Լ���1");
		strColumnName2=_T("�Լ���2");

	}else{
		strColumnName1=_T("å�η�������1");
		strColumnName2=_T("å�η�������2");
	}
	CString sDateCtrlAlias[nDateCnt] =
	{
		_T("��û��1"), _T("��û��2"), 
		_T("������1"), _T("������2"),
		strColumnName1, strColumnName2, 
		_T("å�η��ݳ���1"), _T("å�η��ݳ���2"),
		_T("�������")
	};

	for( i = 0 ; i < nDateCnt ; i++ )
	{
		pCE = (CESL_Control_Element*)pCM->FindControlElement( sDateCtrlAlias[i] );
				
		sData = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
		sData.TrimLeft(); sData.TrimRight();
		
		if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() )
			sData = _T("");
		
		if( sData.IsEmpty() ) continue;
		
		if( !sSearchCond.IsEmpty() ) sSearchCond += _T(",") ;
		
		sSearchCond += pCE->CTRL_ALIAS + _T("=") + sData;
	}
	
	return 0;

EFS_END
return -1;
}