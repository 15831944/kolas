// TabSecondForm.cpp : implementation file
//

#include "stdafx.h"
#include "TabSecondForm.h"
#include "SubfieldDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSecondForm dialog


CTabSecondForm::CTabSecondForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabSecondForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM_SUBFIELD = NULL;
	m_strTagAlias = _T("");
	m_pGridSubfield = NULL;
	m_nCurrentIdx = -1;

	CR_user = RGB(60,179,113);
	CR_BLACK = RGB(0,0,0);
}


VOID CTabSecondForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabSecondForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabSecondForm, CDialog)
	//{{AFX_MSG_MAP(CTabSecondForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSecondForm message handlers
BOOL CTabSecondForm::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CTabSecondForm::ResizeWindow(CRect rectParent)
{
	//�ڱ� �ڽ�
	this->MoveWindow(rectParent);

	CWnd *pWnd = NULL;
	CRect rect;
	this->GetClientRect(rect);
	
	//�׸��� static
	pWnd = (CWnd*)GetDlgItem(IDC_stcSubfield);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rect.top += 10;
	rect.bottom -= 10;
	rect.left += 10;
	rect.right -= 10;	
	
	pWnd->MoveWindow(rect);	

	//�׸���
	pWnd = (CWnd*)GetDlgItem(IDC_gridSubfield);
	if(pWnd->GetSafeHwnd() == NULL) return;
	
	rect.top += 15;
	rect.bottom -= 15;
	rect.left += 15;
	rect.right -= 15;	

	pWnd->MoveWindow(rect);	

}

BOOL CTabSecondForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("��ũ����_��2") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ��ũ����_��2") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_��ũ����_��2"));
	m_pDM_SUBFIELD = FindDM(_T("DM_��ũ����_�ĺ���ȣ"));
	m_pGridSubfield = (CESL_Grid*)m_pCM->FindControl(_T("�ĺ���ȣ�׸���"));
	
	if(!m_pCM || !m_pDM_SUBFIELD)
	{
		AfxMessageBox( _T("reference Error : ��ũ����_��2") );
		return FALSE;		
	}

	m_pGridSubfield->m_bFroceReverse = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CTabSecondForm::SetAllData(CString strTagAlias)
{
	m_strTagAlias = strTagAlias;
}

VOID CTabSecondForm::ShowAllData()
{
	//�ĺ���ȣ
	CString strWhere;
	strWhere.Format(_T("MARC_TAG_ALIAS = '%s'"),m_strTagAlias);
	
	m_pDM_SUBFIELD->RefreshDataManager(strWhere);
	
	SetUdfField();
	
	DisplayGrid();

}

VOID CTabSecondForm::InsertSubfield()
{
	//�±� �Է�
	CSubfieldDlg dlg;
	dlg.m_nOpenMode = 1;
	dlg.m_strMARC_TAG_ALIAS = m_strTagAlias;
	dlg.m_pParentDM = m_pDM_SUBFIELD;
	dlg.m_pParentGrid = m_pGridSubfield;
	if(IDOK == dlg.DoModal())
	{
		DisplayGrid();
	}
}

VOID CTabSecondForm::ModifySubfield()
{
	INT nIdx;
	if(GetSelectedIdx(&nIdx)<0) return;
	

	CString strPK = m_pDM_SUBFIELD->GetCellData(_T("PK"),nIdx);

	//�±� ����
	CSubfieldDlg dlg;
	dlg.m_nOpenMode = -1;
	dlg.m_strPK = strPK;
	dlg.m_pParentDM = m_pDM_SUBFIELD;
	dlg.m_pParentGrid = m_pGridSubfield;
	dlg.m_nCurrentParentIdx = nIdx;
	if(IDOK == dlg.DoModal())
	{
		DisplayGrid();
	}
}

VOID CTabSecondForm::DeleteSubfield()
{
	INT nIdx;
	if(GetSelectedIdx(&nIdx)<0) return;

	CString strPK = m_pDM_SUBFIELD->GetCellData(_T("PK"),nIdx);
	CString strSubfieldCode = m_pDM_SUBFIELD->GetCellData(_T("SUBFIELD_CODE"),nIdx);
	CString strMsg;

	strMsg.Format(_T("[ %s ] �ĺ���ȣ�� ���� �Ͻðڽ��ϱ�?"),strSubfieldCode);
	if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return;

	CString strQuery;
    m_pDM_SUBFIELD->StartFrame();
    
	strQuery.Format(_T("DELETE FROM MARC_SUBFIELD_TBL WHERE PK = %s;"),strPK);
    m_pDM_SUBFIELD->AddFrame(strQuery);	
	
  	INT ids = m_pDM_SUBFIELD->SendFrame();
    m_pDM_SUBFIELD->EndFrame();

	if(ids<0)
	{
		AfxMessageBox(_T("�����ϴ� �� ������ �߻��Ͽ����ϴ�"));
	}
	else 
	{
		AfxMessageBox(_T("�����Ͽ����ϴ�!"));
		m_pDM_SUBFIELD->DeleteRow(nIdx);
		DisplayGrid();
	}
}

INT CTabSecondForm::GetSelectedIdx(INT* nIdx)
{
	
	if(0 == m_pDM_SUBFIELD->GetRowCount())
	{
		AfxMessageBox(_T("���� �Էµ� �ĺ���ȣ�� �����ϴ�!"));
	    return -1;
	}

	m_pGridSubfield->SelectMakeList();

	INT nSelectedCnt = -1;
	
	nSelectedCnt  = m_pGridSubfield->SelectGetCount();

	if(0 == nSelectedCnt)
	{
		AfxMessageBox(_T("������ �ĺ���ȣ�� �����ϴ�!"));
	    return -1;
	}

	if(nSelectedCnt>1)
	{
		AfxMessageBox(_T("�ѹ��� �ϳ��� �ĺ���ȣ�� ������ �� �ֽ��ϴ�!"));
	    return -1;
	}
	
	*nIdx = m_pGridSubfield->SelectGetHeadPosition();

	return 0;
}

VOID CTabSecondForm::SetUdfField()
{
	INT nRowCnt = m_pDM_SUBFIELD->GetRowCount();
	CString strRepeat,strEssential;

	for(INT i=0;i<nRowCnt;i++)
	{
		strRepeat = m_pDM_SUBFIELD->GetCellData(_T("REPEAT_TYPE"),i);
		strEssential = m_pDM_SUBFIELD->GetCellData(_T("ESSENTIAL_TYPE"),i);

		CString strTmp;

		if(_T("Y") == strRepeat)
		{
			strTmp  = _T("�ݺ�");
		}
		else if(_T("N") == strRepeat)
		{
			strTmp  = _T("�ݺ��Ұ�");
		}

		m_pDM_SUBFIELD->SetCellData(_T("UDF_�ݺ�����"),strTmp,i);


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

		m_pDM_SUBFIELD->SetCellData(_T("UDF_�ʼ�����"),strTmp,i);
	}


}

VOID CTabSecondForm::DisplayGrid()
{
	m_pGridSubfield->Display();
	CString strUserDefine;

	for(INT i=0;i<m_pDM_SUBFIELD->GetRowCount();i++)
	{
		strUserDefine = m_pDM_SUBFIELD->GetCellData(_T("USER_DEFINE_YN"),i);
		if(_T("Y") == strUserDefine) SetGridRowUniqueColor(CR_user, i);

	}


}

INT CTabSecondForm::SetGridRowUniqueColor(COLORREF& colorref, INT nRowIdx)
{

	INT nCol = m_pGridSubfield->GetCol();
	INT nCols = m_pGridSubfield->GetCols(0);
	
	if (nRowIdx >= 0)
	{
		m_pGridSubfield->SetRow(nRowIdx + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGridSubfield->SetCol(i);
			m_pGridSubfield->SetCellBackColor(colorref);
			m_pGridSubfield->SetCellForeColor(CR_BLACK);
		}
	}
	
	m_pGridSubfield->SetCol(nCol);
	
	return 0;
}

BEGIN_EVENTSINK_MAP(CTabSecondForm, CDialog)
    //{{AFX_EVENTSINK_MAP(CTabSecondForm)
	ON_EVENT(CTabSecondForm, IDC_gridSubfield, -600 /* Click */, OnClickgridSubfield, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CTabSecondForm::OnClickgridSubfield() 
{
	// TODO: Add your control notification handler code here
	INT nDMRow = m_pDM_SUBFIELD->GetRowCount();
	if(0 == nDMRow) return;

	INT row = m_pGridSubfield->GetRow();
	if (row == 0) return;
	if (row > 0) {

		if(m_nCurrentIdx>=0) {
			
			if(m_nCurrentIdx<nDMRow)
				SetGridRowBoldFont(m_nCurrentIdx,FALSE);
		}

		m_nCurrentIdx = row-1;
		SetGridRowBoldFont(m_nCurrentIdx,TRUE);
	
	}	
}

VOID CTabSecondForm::SetGridRowBoldFont(INT nRowIdx, BOOL bNewValue)
{
	INT nCol = m_pGridSubfield->GetCol();
	INT nCols = m_pGridSubfield->GetCols(0);
	
	if (nRowIdx >= 0)
	{
		m_pGridSubfield->SetRow(nRowIdx + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGridSubfield->SetCol(i);
			m_pGridSubfield->SetCellFontBold(bNewValue);
		}

	}
	
	m_pGridSubfield->SetCol(nCol);
}