// ShelfLocRegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SHELF_LOC_REGISTER.h"
#include "ShelfLocRegDlg.h"
#include "SHELF_LOC_REGISTERDlg.h"
#include "..\..\����\����\�弭����\\Loc_CommonAPI\LocCommonAPI.h"
#include "ShelfDataInfoDLG.h"
#include "ESL_ControlMultiComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShelfLocRegDlg dialog


CShelfLocRegDlg::CShelfLocRegDlg(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CShelfLocRegDlg)
	//}}AFX_DATA_INIT
	this->pParent = pParent;
}

CShelfLocRegDlg::~CShelfLocRegDlg()
{
}

void CShelfLocRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShelfLocRegDlg)
	DDX_Control(pDX, IDC_CHK1, m_ctrCheck);
	DDX_Control(pDX, IDC_EDT_SHELF_ROW, m_EDT_SHELFROW);
	DDX_Control(pDX, IDC_EDT_SHELF_NO, m_EDT_SHELFNO);
	DDX_Control(pDX, IDC_EDT_REG_NO, m_EDT_REGNO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShelfLocRegDlg, CDialog)
	//{{AFX_MSG_MAP(CShelfLocRegDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_REG, OnBtnReg)
	ON_CBN_SELCHANGE(IDC_cmbREQ_MANAGE_CODE, OnSelchangecmbREQMANAGECODE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShelfLocRegDlg message handlers

HBRUSH CShelfLocRegDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CShelfLocRegDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();		
	
	if (InitESL_Mgr(_T("SM_SHELF_LOC_REG_SEARCH")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	dbMgr_BO = FindDM(_T("DM_SHELF_LOC_REG_BO"));
	dbMgr_SE = FindDM(_T("DM_SHELF_LOC_REG_SE"));

	CStdioFile file_stdio;
	CString strLine;
	if(file_stdio.Open(_T("..\\CFG\\������ġ����\\DMINFO.cfg"), CStdioFile::modeReadWrite|CStdioFile::typeBinary|CStdioFile::shareDenyNone))
	{
		TCHAR cUni;
		file_stdio.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file_stdio.SeekToBegin();
		}
		file_stdio.ReadString(strLine);
	}
	else
	{
		AfxMessageBox(_T("DB���� ������ �߸��Ǿ����ϴ�."));
	}

	dbMgr_BO->SetCONNECTION_INFO(strLine);
	dbMgr_SE->SetCONNECTION_INFO(strLine);

	if(dbMgr_BO == NULL || dbMgr_SE == NULL)
	{
		AfxMessageBox(_T("DM_SHELF_LOC_BO/SE ����"));
		return TRUE;
	}

	CESL_DataMgr	pCodeDM;
	pCodeDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO);
	CESL_ControlMultiComboBox* pCombo;
	pCombo = (CESL_ControlMultiComboBox*) GetDlgItem(IDC_cmbREQ_MANAGE_CODE);

	pCodeDM.RestructDataManager(_T("SELECT CODE, DESCRIPTION FROM CD_CODE_TBL WHERE CLASS=1 ORDER BY CODE"));	


	pCombo->ResetContent();
	pCombo->InitMultiComboBox(2);
	CString strData[2];
	for ( int i=0 ; i<pCodeDM.GetRowCount() ; i++ )
	{
		strData[0] = pCodeDM.GetCellData(i,0 );
		strData[1] = pCodeDM.GetCellData(i,1 );		
		pCombo->SetData(strData, 2);
	}
	pCombo->SetCurSel(0);
	m_strManageCode=pCodeDM.GetCellData(pCombo->GetCurSel(),0);
	dbMgr_BO->MANAGE_CODE=m_strManageCode;
	dbMgr_SE->MANAGE_CODE=m_strManageCode;
	((CSHELF_LOC_REGISTERDlg*)pParent)->m_strManageCode=m_strManageCode;
	

	((CButton*)GetDlgItem(IDC_CHK1))->SetCheck(TRUE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CShelfLocRegDlg::OnBtnReg() 
{
	// TODO: Add your control notification handler code here 

	CString strOption,strRegNO,strShelfNO,strShelfRow,strTemp;
	INT nBO_SE_FLAG=-1;//0:���� , 1:���� , -1:�ش��Ϲ�ȣ����
	BOOL bShelfWrite = FALSE;

	m_EDT_REGNO.GetWindowText(strRegNO);
	m_EDT_SHELFNO.GetWindowText(strShelfNO);
	m_EDT_SHELFROW.GetWindowText(strShelfRow);

	//1.���� ���� CHK...
	BOOL bResult = CheckEdtISBlank();
	if(!bResult)
	{
		GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("���� ��� ������ ���� �Ǿ����ϴ�."));
		::Beep(2000,200);
		return;
	}

	//2.��Ϲ�ȣ �������� CHK...
	nBO_SE_FLAG = CheckRegNO(strRegNO);
	if(nBO_SE_FLAG==-1)
	{
		strTemp.Format(_T("��Ϲ�ȣ %s�� �������� �ʽ��ϴ�."),strRegNO);
		GetDlgItem(IDC_MESSAGE)->SetWindowText(strTemp);
		m_EDT_REGNO.SetWindowText(_T(""));
		::Beep(2000,200);
		return;
	}

	//3.������ȣ�� �̹� ��ϵǾ� �ִ��� CHK...
	//��ϵǾ��ִٸ� TRUE ��ȯ , ��������� FALSE ��ȯ
	bShelfWrite = CheckShelfWriteYN(nBO_SE_FLAG);

	//4.������ȣ�� ��ϵǾ� �ִٸ� ���� ��ġ���� DLG DoModal...
	if(bShelfWrite)
	{
		CShelfDataInfoDLG dataInfoDlg(this);
		dataInfoDlg.SetBO_SE(nBO_SE_FLAG);
		dataInfoDlg.SetModifyShelfInfo(strShelfNO,strShelfRow);
		if(IDOK == dataInfoDlg.DoModal())
		{
			CString strShelfNum_MOD;
			CString strShelfRow_MOD;
			CString strShelfNum_PRE;
			CString strShelfRow_PRE;
			dataInfoDlg.GetModifyShelfInfo(strShelfNum_MOD,strShelfRow_MOD);
			dataInfoDlg.GetPreShelfInfo(strShelfNum_PRE,strShelfRow_PRE);

			INT ids = -1;
			ids = RecordShelfInfo(nBO_SE_FLAG,strRegNO,strShelfNum_MOD,strShelfRow_MOD,strShelfNum_PRE,strShelfRow_PRE);

			if(ids!=-1)
			{
				GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("���� ������ ���� �Ǿ����ϴ�."));
				if(m_ctrCheck.GetCheck())
				{
					m_EDT_REGNO.SetWindowText(_T(""));
					m_EDT_REGNO.SetFocus();
				}
				else
				{
					m_EDT_REGNO.SetWindowText(_T(""));
					m_EDT_SHELFNO.SetWindowText(_T(""));
					m_EDT_SHELFROW.SetWindowText(_T(""));
				}
			}
			else
			{
				GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("���� ���� ������ ���� �Ͽ����ϴ�."));
				::Beep(2000,200);
			}		
		}
		else 
		{
			GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("�۾�����ǥ��"));
				if(m_ctrCheck.GetCheck())
				{
					m_EDT_REGNO.SetWindowText(_T(""));
				}
				else
				{
					m_EDT_REGNO.SetWindowText(_T(""));
					m_EDT_SHELFNO.SetWindowText(_T(""));
					m_EDT_SHELFROW.SetWindowText(_T(""));
				}
		}
	}

	//5.������ȣ�� Emp��� ���̺� ���...
	else
	{
		INT ids = -1;
		ids = RecordShelfInfo(nBO_SE_FLAG,strRegNO,strShelfNO,strShelfRow,_T(""),_T(""));

		if(ids!=-1)
		{
			GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("���� ������ ��� �Ǿ����ϴ�."));
			if(m_ctrCheck.GetCheck())
				{
					m_EDT_REGNO.SetWindowText(_T(""));
					m_EDT_REGNO.SetFocus();
				}
				else
				{
					m_EDT_REGNO.SetWindowText(_T(""));
					m_EDT_SHELFNO.SetWindowText(_T(""));
					m_EDT_SHELFROW.SetWindowText(_T(""));
				}
		}
		else
		{
			GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("���� ���� ��Ͽ� ���� �Ͽ����ϴ�."));
			::Beep(2000,200);
		}
	}
}

INT CShelfLocRegDlg::RecordShelfInfo(INT nBO_SE,CString strRegNO,CString strShelfNO,CString strShelfRow,CString strShelfNO_PRE,CString strShelfRow_PRE)
{
	CString strOption;
	INT ids=-1;

//	CString strValue;

//	CESL_ControlMgr *pCM = FindCM(_T("CM_SHELF_LOC_REG_SEARCH"));
//	pCM->GetControlMgrData(_T("��������"), strValue );
//	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.MANAGE_CODE = strValue;

//	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.MANAGE_CODE=_T("MA");

	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.StartFrame();	
	
	if(nBO_SE==0)
	{
		strOption.Format(_T("UPDATE BO_BOOK_TBL SET SHELF_NUM='%s',SHELF_ROW='%s' WHERE REG_NO='%s' AND MANAGE_CODE='%s';"),strShelfNO,strShelfRow,strRegNO,m_strManageCode);
	}
	
	else if(nBO_SE==1)
	{
		strOption.Format(_T("UPDATE SE_BOOK_TBL SET SHELF_NUM='%s',SHELF_ROW='%s' WHERE REG_NO='%s'AND MANAGE_CODE='%s';"),strShelfNO,strShelfRow,strRegNO,m_strManageCode);
	}
  	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.AddFrame(strOption);
 	ids = ((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.SendFrame();
  	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.EndFrame();

	if(ids!=-1) WriteLOG(strRegNO,strShelfNO,strShelfRow,strShelfNO_PRE,strShelfRow_PRE);

	return ids;
}

VOID CShelfLocRegDlg::WriteLOG(CString strRegNO,
							   CString strShelfNO,CString strShelfRow,
							   CString strShelfNO_PRE,CString strShelfRow_PRE)
{
	CStdioFile file_stdio;
	CString strWriteFileName;

	//������ ���(strShelfNO_PRE�� ���� ������ �ƴ� ���) �ش� �α����Ͽ��� ��Ϲ�ȣ�� �����.
	if(!strShelfNO_PRE.IsEmpty())
	{
		CString strLine;
		strWriteFileName.Format(_T("..\\cfg\\������ġ����\\LOG\\%s+%s.dat"),strShelfNO_PRE,strShelfRow_PRE);	
		if(file_stdio.Open(strWriteFileName, CStdioFile::modeReadWrite|CStdioFile::typeBinary|CStdioFile::shareDenyNone))
		{
			TCHAR cUni;
			DWORD dwPosition;
			file_stdio.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file_stdio.SeekToBegin();
			}
			while(TRUE)
			{ 
				dwPosition = file_stdio.GetPosition();
				if(file_stdio.ReadString(strLine)==FALSE) break;
//				dwPosition = file_stdio.GetPosition();
				strLine.TrimLeft();strLine.TrimRight();
				if(strLine.GetLength==0) continue;

				if(strLine==strRegNO)
				{
					file_stdio.Seek(dwPosition,CStdioFile::begin);
					file_stdio.WriteString(_T("            \r\n"));
					break;
				}
			}
			file_stdio.Close();
		}
	}

	//���ο� ���� ������ ����Ѵ�.
	strWriteFileName.Format(_T("..\\cfg\\������ġ����\\LOG\\%s+%s.dat"),strShelfNO,strShelfRow);	
	if(!file_stdio.Open(strWriteFileName, CStdioFile::modeRead))
	{
		// ������ ������ ����
		if(file_stdio.Open(strWriteFileName, CStdioFile::modeCreate|CStdioFile::modeWrite|CStdioFile::modeNoTruncate|CStdioFile::typeBinary))
		{
			TCHAR cUni = 0xFEFF;
			file_stdio.Write( &cUni, sizeof(TCHAR));
		}
		else
		{
			AfxMessageBox(_T("Log���� ������ �����߽��ϴ�."));
			return;
		}		
	}
		if(file_stdio!=NULL) file_stdio.Close();
		// �� ������.... ������ ������ ������.. �ڿ� �̾��..
		file_stdio.Open(strWriteFileName, CStdioFile::modeCreate|CStdioFile::modeWrite|CFile::modeNoTruncate|CFile::typeBinary);
		file_stdio.SeekToEnd();
		file_stdio.WriteString(strRegNO); 
		file_stdio.WriteString(_T("\r\n")); 
		if(file_stdio!=NULL) file_stdio.Close();
}

void CShelfLocRegDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
//	((CSHELF_LOC_REGISTERDlg*)pParent)->DlgQuit();
//	CDialog::OnCancel();
}

BOOL CShelfLocRegDlg::CheckEdtISBlank()
{
	BOOL bResult=TRUE;
	CString strRegNO,strShelfNO,strShelfRow;
	m_EDT_REGNO.GetWindowText(strRegNO);
	m_EDT_SHELFNO.GetWindowText(strShelfNO);
	m_EDT_SHELFROW.GetWindowText(strShelfRow);
	if(strRegNO.IsEmpty()||strShelfNO.IsEmpty()||strShelfRow.IsEmpty())
	{
		bResult = FALSE;
	}
	return bResult;
}

INT CShelfLocRegDlg::CheckRegNO(CString strRegNO)
{
	INT nBO_SE_FLAG=-1;
	INT nResultCNT=0;
	CString strOption;

//	dbMgr_BO->MANAGE_CODE = _T("MA");
//	dbMgr_SE->MANAGE_CODE = _T("MA");

	strOption.Format(_T(" BB.REG_NO='%s' AND IB.MANAGE_CODE=UDF_MANAGE_CODE"),strRegNO);	
	dbMgr_BO->RefreshDataManager(strOption);
	
	 nResultCNT = dbMgr_BO->GetRowCount();
	 
	 if(nResultCNT > 0)
	 {
		 //���࿡ �ڷᰡ �����Ѵٸ� û����ȣ ���� �� 0 RETURN
		CLocCommonAPI::MakeShelfCode(this,_T("DM_SHELF_LOC_REG_BO"));
		nBO_SE_FLAG = 0;
		return nBO_SE_FLAG;
	 }
	
	dbMgr_SE->RefreshDataManager(strOption);
	nResultCNT = dbMgr_SE->GetRowCount();
	if(nResultCNT > 0)
	 {
		//���ӿ� �ڷᰡ �����Ѵٸ� û����ȣ ���� �� 1 RETURN
		CLocCommonAPI::MakeShelfCode(this,_T("DM_SHELF_LOC_REG_SE"));
		nBO_SE_FLAG = 1;
		return nBO_SE_FLAG;
	 }
	//����/���ӿ� �ش� ��Ϲ�ȣ�� ���ٸ� -1 RETURN
	return nBO_SE_FLAG;
}

BOOL CShelfLocRegDlg::CheckShelfWriteYN(INT nBO_SE)
{
	INT nBO_SE_FLAG = nBO_SE; //0:���� 1:����
	BOOL bResult = FALSE;
	CString strResult;

	if(nBO_SE_FLAG==0)
	{
		dbMgr_BO->GetCellData(_T("������ȣ"),0,strResult);
	}
	
	else if(nBO_SE_FLAG==1)
	{
		dbMgr_SE->GetCellData(_T("������ȣ"),0,strResult);
	}

	if(!strResult.IsEmpty()) bResult = TRUE;

	return bResult;
}

BOOL CShelfLocRegDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			OnBtnReg();
			return TRUE;
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}



//	strOption.Format(_T(" BB.REG_NO='%s' "),strRegNO);
//	dbMgr_BO->RefreshDataManager(strOption);
	


/*
	strOption.Format(_T(" BB.REG_NO='%s' "),strRegNO);

	dbMgr_BO = FindDM(_T("DM_SHELF_LOC_REG_BO"));
	dbMgr_BO->RefreshDataManager(strOption);

	CLocCommonAPI::MakeShelfCode(this,_T("DM_SHELF_LOC_REG"));
//	CString result;
//  CString sQuery;

	INT nResultCNT = dbMgr_BO->GetRowCount();
//	dbMgr->GetCellData(_T("û����ȣ"),0,result);
//	dbMgr->GetCellData(_T("����"),0,result);

	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.StartFrame();	
  	strOption.Format(_T("UPDATE BO_BOOK_TBL SET SHELF_NUM='%s',SHELF_ROW='%s' WHERE REG_NO='%s';"),strShelfNO,strShelfRow,strRegNO);
  	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.AddFrame(strOption);
 	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.SendFrame();
  	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.EndFrame();




	m_EDT_REGNO.SetWindowText(_T(""));
	m_EDT_SHELFNO.SetWindowText(_T(""));
	m_EDT_SHELFROW.SetWindowText(_T(""));
*/

void CShelfLocRegDlg::OnSelchangecmbREQMANAGECODE() 
{
	// TODO: Add your control notification handler code here
	CESL_DataMgr	pCodeDM;
	pCodeDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO);
	CESL_ControlMultiComboBox* pCombo;
	pCombo = (CESL_ControlMultiComboBox*) GetDlgItem(IDC_cmbREQ_MANAGE_CODE);

	pCodeDM.RestructDataManager(_T("SELECT CODE, DESCRIPTION FROM CD_CODE_TBL WHERE CLASS=1 ORDER BY CODE"));	

	m_strManageCode=pCodeDM.GetCellData(pCombo->GetCurSel(),0);
	dbMgr_BO->MANAGE_CODE=m_strManageCode;;
	dbMgr_SE->MANAGE_CODE=m_strManageCode;
	
	((CSHELF_LOC_REGISTERDlg*)pParent)->m_strManageCode=m_strManageCode;

}
