#include "stdafx.h"
#include "bookcooperativegroup.h"
#include "ILL_API_APPENDIX_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CILL_API_APPENDIX_DLG::CILL_API_APPENDIX_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CILL_API_APPENDIX_DLG::IDD, pParent)
{
	m_pAPL = (CILL_APL*)pParent;
	m_strWhere = _T("");
}


void CILL_API_APPENDIX_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CILL_API_APPENDIX_DLG)	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CILL_API_APPENDIX_DLG, CDialog)
	//{{AFX_MSG_MAP(CILL_API_APPENDIX_DLG)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()


BOOL CILL_API_APPENDIX_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	INT ids = 0;

	ids = InitESL_Mgr(_T("ILL_APL_APPENDIX"));
	if(0 > ids)
	{
		AfxMessageBox(_T("ERROR : InitESL_Mgr(\"ILL_APL_APPENDIX\")"));
		CDialog::OnCancel();
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_ILL_APL_APPENDIX"));
	if(NULL == pDM)
	{
		AfxMessageBox(_T("ERROR : FindDM(\"ILL_APL_APPENDIX\")"));
		CDialog::OnCancel();
		return FALSE;
	}

	ids = pDM->RefreshDataManager(m_strWhere);
	if(0 > ids) 
	{
		AfxMessageBox(_T("ERROR : RefreshDataManager(\"ILL_APL_APPENDIX\")"));
		CDialog::OnCancel();
		return FALSE;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_ILL_APL_APPENDIX"));
	
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_ILL_APL_APPENDIX"), _T("APPENDIX_GRID"));
	if(NULL == m_pGrid)
	{
		AfxMessageBox(_T("ERROR : FindControl(\"ILL_APL_APPENDIX\")"));
		CDialog::OnCancel();
		return FALSE;
	}

	ids = pDM->GetRowCount();
	if(0 >= ids)
	{
		
		AfxMessageBox(_T("���Ⱑ���� �η��ڷᰡ �����ϴ�."));
		CDialog::OnCancel();
		return FALSE;
	}

	
	CString sWorkingStatus;
	const DM_BO_ROW_COUNT = pDM->GetRowCount();
	for( INT nRow = 0 ; nRow < DM_BO_ROW_COUNT; nRow++ )
	{
		pDM->GetCellData(_T("�ڷ����") , nRow, sWorkingStatus);		
		CLocCommonAPI::GetWorkingStatusDescToCode(sWorkingStatus , 1);		
		pDM->SetCellData(_T("�ڷ����_����"), sWorkingStatus ,nRow);		
	}

	ids = m_pGrid->Display();
	if(0 > ids)
	{
		AfxMessageBox(_T("ERROR : Display(\"ILL_APL_APPENDIX\")"));
		CDialog::OnCancel();
		return FALSE;
	}

	
	return TRUE;  
	              
}

void CILL_API_APPENDIX_DLG::OnOK() 
{
	
	INT ids = 0;

	CESL_DataMgr* pDM = FindDM(_T("DM_ILL_APL_APPENDIX"));
	if(NULL == pDM)
	{
		AfxMessageBox(_T("ERROR : FindDM(\"ILL_APL_APPENDIX\")"));
		return;
	}
	m_pAPL->m_pSelectedAppendixDm->FreeData();
	CString strData;
	CString strTemp;
	INT nIdx = 0;
	for(INT i=0; i<pDM->GetRowCount(); i++)
	{		
		strData = m_pGrid->GetAt(i, 0);
		if(_T("V") == strData)
		{
			ids = pDM->GetCellData(_T("��������"), i, strTemp);
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_VOID2(-4, _T("ERROR : FindDM(\"ILL_APL_APPENDIX\")"));
			}
			if(strTemp == m_pInfo->MANAGE_CODE)
			{
				AfxMessageBox(_T("�ڰ������� ��û�� �� �����ϴ�."));
				return;
			}


			ids = pDM->GetCellData(_T("����Ǽ�"), i, strTemp);
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_VOID2(-5, _T("ERROR : FindDM(\"ILL_APL_APPENDIX\")"));
			}
			if(0 < _ttoi(strTemp))
			{
				AfxMessageBox(_T("�������� �ڷ�� ��û�� �� �����ϴ�."));
				return;
			}	
			
			
			ids = pDM->GetCellData(_T("�̿����ѱ���"), i, strTemp);
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_VOID2(-6, _T("ERROR : FindDM(\"ILL_APL_APPENDIX\")"));
			}
			if(strTemp.Compare(_T("CL")) == 0)
			{
				AfxMessageBox(_T("�缭���� �ڷ�� ��û�� �� �����ϴ�."));
				return;
			}


			m_pAPL->m_pSelectedAppendixDm->InsertRow(-1);
			m_pAPL->m_pSelectedAppendixDm->SetCellData(_T("�η�_Ű"), pDM->GetCellData(_T("åKEY"), i), nIdx);
			m_pAPL->m_pSelectedAppendixDm->SetCellData(_T("�η�_��"), pDM->GetCellData(_T("�����"), i), nIdx);
			m_pAPL->m_pSelectedAppendixDm->SetCellData(_T("�η�_��Ϲ�ȣ"), pDM->GetCellData(_T("��Ϲ�ȣ"), i), nIdx);
			m_pAPL->m_pSelectedAppendixDm->SetCellData(_T("�η�_����⵵"), pDM->GetCellData(_T("����⵵"), i), nIdx);
			m_pAPL->m_pSelectedAppendixDm->SetCellData(_T("�η�_������"), pDM->GetCellData(_T("������"), i), nIdx);
			m_pAPL->m_pSelectedAppendixDm->SetCellData(_T("�η�_�������±���"), pDM->GetCellData(_T("�������±���"), i), nIdx);
			m_pAPL->m_pSelectedAppendixDm->SetCellData(_T("�η�_����"), pDM->GetCellData(_T("����"), i), nIdx);
			m_pAPL->m_pSelectedAppendixDm->SetCellData(_T("�η�_����"), pDM->GetCellData(_T("������"), i), nIdx);
			m_pAPL->m_pSelectedAppendixDm->SetCellData(_T("�η�_û����ȣ"), pDM->GetCellData(_T("û����ȣ"), i), nIdx);
			nIdx++;
		}
	}

	if(0 >= m_pAPL->m_pSelectedAppendixDm->GetRowCount())
	{
		
		if ( IDNO == MessageBox( _T("������ �η��ڷᰡ �����ϴ�.\r\n�η��ڷḦ ��û���� �ʰ� �����Ͻðڽ��ϱ�?"), NULL, MB_YESNO|MB_ICONQUESTION ) )
		{
			return;
		}
	}
	CDialog::OnOK();	
}

void CILL_API_APPENDIX_DLG::OnCancel() 
{	
	CDialog::OnCancel();
}
