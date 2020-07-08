// SubfieldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SubfieldDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubfieldDlg dialog


CSubfieldDlg::CSubfieldDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubfieldDlg)
	m_bUSER_DEFINE = FALSE;
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_pParentDM = NULL;
	m_pParentGrid = NULL;

	m_nOpenMode = 1; //default
	m_strMARC_TAG_ALIAS = _T("");
	m_strPK = _T("");

	m_nCurrentParentIdx = -1;

}


VOID CSubfieldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubfieldDlg)
	DDX_Check(pDX, IDC_chkUSER_DEFINE_YN, m_bUSER_DEFINE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubfieldDlg, CDialog)
	//{{AFX_MSG_MAP(CSubfieldDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubfieldDlg message handlers

BOOL CSubfieldDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(InitESL_Mgr( _T("��ũ����_�ĺ���ȣ") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ��ũ����_�ĺ���ȣ") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_��ũ����_�ĺ���ȣ"));
	m_pDM = FindDM(_T("DM_��ũ����_�ĺ���ȣ_����"));

	if(!m_pCM || !m_pDM)
	{
		AfxMessageBox( _T("reference Error : ��ũ����_�ĺ���ȣ") );
		return FALSE;		
	}

	if(InitOpenMode()<0)
	{
		AfxMessageBox( _T("�ĺ���ȣ ���� �ʱ�ȭ�� ����!") );
		return FALSE;		
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CSubfieldDlg::InitOpenMode()
{

	if(m_nOpenMode>0) //�Է�
	{
		if(!m_pParentDM || !m_pParentGrid)
		{
			AfxMessageBox( _T("�θ� ������ ã�� �� �����ϴ�!") );
			return -1;		
		}
		
		
		if(m_strMARC_TAG_ALIAS.IsEmpty())
		{
			AfxMessageBox( _T("�±� �˸��ƽ��� ã�� �� �����ϴ�!") );
			return -1;		
		}

	}
	else //���� 
	{

		if(!m_pParentDM || !m_pParentGrid || m_nCurrentParentIdx<0)
		{
			AfxMessageBox( _T("�θ� ������ ã�� �� �����ϴ�!") );
			return -1;		
		}
		
		if(m_strPK.IsEmpty())
		{
			AfxMessageBox( _T("PK�� ã�� �� �����ϴ�!") );
			return -1;		
		}	
		
		CString strWhere;
		strWhere.Format(_T("PK = %s"),m_strPK);
		m_pDM->RefreshDataManager(strWhere);

		INT nRow = m_pDM->GetRowCount();

		if(0 == nRow) 
		{
			AfxMessageBox( _T("�ش� PK�� �����Ͱ� �����ϴ�!") );
			return -1;					
		}

		m_pCM->AllControlDisplay(0);
		
		
		//�±� �ڵ�� �������� ���ϰ� 
		CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("�±��ڵ�"));
		if(pEdit) pEdit->SetReadOnly();


		//�ݺ�����, �ʼ�����
		CString strRepeat,strEssential;
		
		strRepeat = m_pDM->GetCellData(_T("REPEAT_TYPE"),0);
		strEssential = m_pDM->GetCellData(_T("ESSENTIAL_TYPE"),0);
		
		CComboBox* pCombo = (CComboBox*)m_pCM->FindControl(_T("�ݺ�����"));

		if(pCombo)
		{
			if(_T("Y") == strRepeat)
			{
				pCombo->SetCurSel(0);
			}
			else 
			{
				pCombo->SetCurSel(1);
			}

		}

		pCombo = (CComboBox*)m_pCM->FindControl(_T("�ʼ�����"));

		if(pCombo)
		{
			if(_T("M") == strEssential)
			{
				pCombo->SetCurSel(0);
			}
			else if(_T("A") == strEssential)
			{
				pCombo->SetCurSel(1);
			}
			else if(_T("O") == strEssential)
			{
				pCombo->SetCurSel(2);
			}
			else if(_T("B") == strEssential)
			{
				pCombo->SetCurSel(3);
			}

		}

		//����� ���� 
		CString strUserDefineYn;
		strUserDefineYn = m_pDM->GetCellData(_T("USER_DEFINE_YN"),0);
		
		if(_T("Y") == strUserDefineYn) m_bUSER_DEFINE = TRUE;
		else m_bUSER_DEFINE = FALSE;

		UpdateData(FALSE);

	}

	return 0;
}


INT CSubfieldDlg::CheckillegalSign(CString strData)
{
	INT pos = strData.Find(_T("*"));
	if(pos<0) return 0;
	
	AfxMessageBox(_T("* ��ȣ�� ����� �� �����ϴ�. Ȯ�� �� �ֽʽÿ�!"));

	return -1;
}

INT CSubfieldDlg::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}


CString CSubfieldDlg::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CSubfieldDlg::SaveAllData()
{
	UpdateData();

	const INT cnt = 3;
	
	CString strCMAlias[cnt] = {
		_T("�ĺ���ȣ"),
		_T("��������Ʈ"),
		_T("����")};


	CString strDMAlias[cnt] = {
		_T("SUBFIELD_CODE"),
		_T("PUNCTUATION_MARK_SET"),
		_T("DESCRIP")};
	

	if(m_nOpenMode>0)
	{
		m_pDM->FreeData();
		m_pDM->InsertRow(-1);
	
		CString strPK;
		m_pDM->MakeRecKey(strPK);
		m_pDM->SetCellData(_T("PK"),strPK,0);
		m_pDM->SetCellData(_T("MARC_TAG_ALIAS"),m_strMARC_TAG_ALIAS,0);
		m_pDM->SetCellData(_T("CREATE_DATE"),GetTodayDate(),0);
	}
	else
	{
		m_pDM->SetCellData(_T("MODIFY_DATE"),GetTodayDate(),0);
	}
	
	CString strData;

	for(INT i=0;i<cnt;i++)
	{
		m_pCM->GetControlMgrData(strCMAlias[i],strData);
		m_pDM->SetCellData(strDMAlias[i],strData,0);
	}
	
	//�ݺ����� 
	CString strText;
	CComboBox* pCombo = (CComboBox*)m_pCM->FindControl(_T("�ݺ�����"));
	pCombo->GetWindowText(strText);

	if(_T("�ݺ�") == strText)
	{
		strData = _T("Y");
	}
	else
	{
		strData = _T("N");
	}

	m_pDM->SetCellData(_T("REPEAT_TYPE"),strData,0);

	//�ʼ�����
	pCombo = (CComboBox*)m_pCM->FindControl(_T("�ʼ�����"));
	pCombo->GetWindowText(strText);

	if(_T("�ʼ�") == strText)
	{
		strData = _T("M");
	}
	else if(_T("�ش���ʼ�") == strText)
	{
		strData = _T("A");
	}
	else if(_T("�緮") == strText)
	{
		strData = _T("O");
	}
	else if(_T("����") == strText)
	{
		strData = _T("B");
	}

	m_pDM->SetCellData(_T("ESSENTIAL_TYPE"),strData,0);
	

	
	//����� ���� 
	if(m_bUSER_DEFINE)
		m_pDM->SetCellData(_T("USER_DEFINE_YN"),_T("Y"),0);
	else
		m_pDM->SetCellData(_T("USER_DEFINE_YN"),_T("N"),0);



	CArray<CString,CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	m_pDM->StartFrame();

	if(m_nOpenMode>0)
	{
		m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("MARC_SUBFIELD_TBL"), 0 , m_pDM);
	}
	else
	{
		RemoveAliasList.Add(_T("PK"));
		m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("MARC_SUBFIELD_TBL"), 0, m_pDM, _T("PK"), _T("NUMERIC"), m_strPK);
	}
	
	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	
	return ids;
}

INT CSubfieldDlg::RefreshParent()
{
	if(m_nOpenMode>0)
	{
		m_pParentDM->InsertRow(-1);
		m_nCurrentParentIdx = m_pParentDM->GetRowCount()-1;
	}

	CopyDMToDMByAlias(m_pDM,0,m_pParentDM,m_nCurrentParentIdx);

	
	//UDF_�ݺ�����,UDF_�ʼ�����
	CString strRepeat,strEssential;

	strRepeat = m_pParentDM->GetCellData(_T("REPEAT_TYPE"),m_nCurrentParentIdx);
	strEssential = m_pParentDM->GetCellData(_T("ESSENTIAL_TYPE"),m_nCurrentParentIdx);

	CString strTmp;

	if(_T("Y") == strRepeat)
	{
		strTmp  = _T("�ݺ�");
	}
	else if(_T("N") == strRepeat)
	{
		strTmp  = _T("�ݺ��Ұ�");
	}

	m_pParentDM->SetCellData(_T("UDF_�ݺ�����"),strTmp,m_nCurrentParentIdx);


	if(_T("M") == strEssential)
	{
		strTmp = _T("�ʼ�");
	}
	else if(_T("A") == strEssential)
	{
		strTmp = _T("�ش���ʼ�");
	}
	else if(_T("O") == strEssential)
	{
		strTmp = _T("�緮");
	}
	else if(_T("B") == strEssential)
	{
		strTmp = _T("����");
	}

	m_pParentDM->SetCellData(_T("UDF_�ʼ�����"),strTmp,m_nCurrentParentIdx);
	
	/*
	if(m_pParentGrid)
	{
		m_pParentGrid->DisplayLine(m_nCurrentParentIdx);
	}
	*/
	
	return 0;
}

/*
INT CSubfieldDlg::SetGridRowUniqueColor(CESL_Grid* pGrid, COLORREF& colorref, INT nRowIdx)
{

	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	
	if (nRowIdx >= 0)
	{
		pGrid->SetRow(nRowIdx + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(colorref);
			pGrid->SetCellForeColor(CR_BLACK);
		}
	}
	
	pGrid->SetCol(nCol);
	
	return 0;
}
*/

VOID CSubfieldDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strSubfieldCode;
	m_pCM->GetControlMgrData(_T("�ĺ���ȣ"),strSubfieldCode);
	
	if(strSubfieldCode.IsEmpty())
	{
		AfxMessageBox(_T("�ĺ���ȣ�� �Էµ��� �ʾҽ��ϴ�!"));
		return;
	}
	
	CString strPuncMark;
	m_pCM->GetControlMgrData(_T("��������Ʈ"),strPuncMark);

	CString strDescrip;
	m_pCM->GetControlMgrData(_T("����"),strDescrip);

	if(m_nOpenMode>0)
	{
		
		CString strResult;

		CString strQuery;
		strQuery.Format(_T("SELECT COUNT(*) FROM MARC_SUBFIELD_TBL WHERE MARC_TAG_ALIAS = '%s' AND SUBFIELD_CODE = '%s'"),m_strMARC_TAG_ALIAS,strSubfieldCode);
    
		m_pDM->GetOneValueQuery(strQuery,strResult);

		if(_ttoi(strResult.GetBuffer(0)) > 0)
		{
			AfxMessageBox(_T("�ش� �±� ���� ������ �ĺ���ȣ�� �־ �Է� �� �� �����ϴ�!"));
			return;
		}
   	}
	

	INT ids = 0;
	ids = CheckillegalSign(strSubfieldCode);
	if(ids<0) return;
	
	ids = CheckillegalSign(strPuncMark);
	if(ids<0) return;

	ids = CheckillegalSign(strDescrip);
	if(ids<0) return;
	

	if(SaveAllData()<0)
	{
		AfxMessageBox(_T("���忡 �����Ͽ����ϴ�!"));
		return;
	}
	else
	{
		AfxMessageBox(_T("�����Ͽ����ϴ�!"));
	}

	RefreshParent();

	CDialog::OnOK();
}
