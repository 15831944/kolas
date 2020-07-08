// CSS_2_02.cpp : implementation file
//

#include "stdafx.h"
#include "CSS_2_02.h"

#include "../../����/����/BO_ACQ_EVIRONMENT_MANAGER/EnvironComboBox_MultiState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSS_2_02 dialog


CCSS_2_02::CCSS_2_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSS_2_02)
	//}}AFX_DATA_INIT

	m_pComboBox = NULL;

	// KOL.UDF.022 ��ť���ڵ� ����
	m_nRow = -1;
	m_pMarc = NULL;
	m_pMarcMgr = NULL;
	m_pComboBox = NULL;
	m_pCM = NULL;
	m_pDM = NULL;
}

CCSS_2_02::~CCSS_2_02()
{
	
}



VOID CCSS_2_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSS_2_02)
	DDX_Control(pDX, IDC_cTag, m_cTag);
	//}}AFX_DATA_MAP
}

VOID CCSS_2_02::SetMarcInfo( CMarcMgr *pMarcMgr, CMarc *pMarc )
{
	m_pMarcMgr = pMarcMgr ;
	m_pMarc = pMarc ;
}

BEGIN_MESSAGE_MAP(CCSS_2_02, CDialog)
	//{{AFX_MSG_MAP(CCSS_2_02)
	ON_CBN_KILLFOCUS(IDC_CBO_TAG, OnKillfocusCboTag)
	ON_CBN_SELCHANGE(IDC_CBO_TAG, OnSelchangeCboTag)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CCSS_2_02, CDialog)
    //{{AFX_EVENTSINK_MAP(CCSS_2_02)
	ON_EVENT(CCSS_2_02, IDC_VCF_SELECTED_TERM, -600 /* Click */, OnClickVcfSelectedTerm, VTS_NONE)
	ON_EVENT(CCSS_2_02, IDC_VCF_SELECTED_TERM, -601 /* DblClick */, OnDblClickVcfSelectedTerm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCSS_2_02 message handlers

VOID CCSS_2_02::OnCancel() 
{
	//����
	
	CDialog::OnCancel();
}

VOID CCSS_2_02::OnOK() 
{
	//Tag ����
	SetDataToMarc ();
	CDialog::OnOK();
}

VOID CCSS_2_02::SetTerms( CString sTerms )
{
	m_sTerms = sTerms;
}

VOID CCSS_2_02::OnKillfocusCboTag() 
{
	CESL_Grid* pGrid;
	pGrid	= static_cast<CESL_Grid*>(m_pCM->FindControl(_T("GRID")));

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	CString sTag;
	GetDlgItemText(IDC_CBO_TAG, sTag);
	pGrid->SetTextMatrix(nRow, nCol, sTag);
}

VOID CCSS_2_02::OnSelchangeCboTag() 
{
//	CF1Book *pVcf = (CF1Book*)GetDlgItem(IDC_VCF_SELECTED_TERM);
//    CComboBox *pTag = (CComboBox*)GetDlgItem(IDC_CBO_TAG);
//    CString sTag;
//    GetDlgItemText(IDC_CBO_TAG, sTag);
//    pTag->GetLBText(pTag->GetCurSel(), sTag);
//    pVcf->SetTextRC(m_nRow, 3, sTag);
//    pTag->ShowWindow(SW_HIDE);
//    OnClickVcfSelectedTerm(pVcf->GetRow(), 1);	
}

VOID CCSS_2_02::OnClickVcfSelectedTerm() 
{

/*
	CComboBox *pTag = (CComboBox*)GetDlgItem(IDC_CBO_TAG);
    pTag->ShowWindow(SW_HIDE);

	if(nRow == 0 || nCol == 0) return;
	// �޺��ڽ��� �� ��ġ�� �°� �̵���Ų��....^^
    if(nCol == 3) {
        CComboBox *pTag = (CComboBox*)GetDlgItem(IDC_CBO_TAG);
        pTag->ShowWindow(SW_SHOW);
        //pTag->ShowDropDown();
        m_nRow = pGrid->GetRow();
        //pGrid->SetTextRC(nRow, 1, _T("V"));
    }

	
	// �׸��� ���� Ŭ��
	/*
	CF1Book *pVcf = (CF1Book*)GetDlgItem(IDC_VCF_SELECTED_TERM);
    CString sMarc, sTagData , sTemp , sTmpTagData;
    INT nTag , nMax , i;
    
    BOOL bNeedSort = TRUE;
    if(nCol == -1) {bNeedSort = FALSE; nCol = 1;}

    // Tag������ ���� �׸��� ����
    if(nRow == 0 && nCol == 3) {
        pVcf->Sort3(1, -1, pVcf->GetMaxRow(), 1, 1, 3, 2, 0);
        pVcf->SetSelection(1, 3, 1, 3);
    }

    if(nRow == 0 || nCol == 0) return;

    // �÷� 3���� Tag�� �ٲܶ� ���
    // �޺��ڽ��� �� ��ġ�� �°� �̵���Ų��....^^
    if(nCol == 3) {
        CComboBox *pTag = (CComboBox*)GetDlgItem(IDC_CBO_TAG);
        pTag->ShowWindow(SW_SHOW);
        pTag->MoveWindow(380, 32 + (nRow - pVcf->GetTopRow()) * 17, 70, 20);
        pTag->SetWindowText(pVcf->GetTextRC(nRow, 3));
        pTag->ShowDropDown();
        m_nRow = pVcf->GetRow();
        pVcf->SetTextRC(nRow, 1, _T("V"));
    }
    sTagData = pVcf->GetTextRC(nRow, 2);
    nTag = _ttoi(pVcf->GetTextRC(nRow, 3));

    // Tag�� ����
	sTmpTagData.Empty();
    switch(nTag) {
    case  56:
		nMax = pVcf->GetMaxRow();

		// ���������� ����ϰ� Tag������ ����ڿ� �ٽ� ����Ѵ�.
		// ������ Tag������ ���鶧 �׸��� ������� ����� ���ؼ�..
		// �׸��� �Ʒ����� �ٽ� ����� �����Ѵ�..
		// 56 & 82 ����..
		if ( pVcf->GetTextRC( nRow , 1 ) == _T("V") )
			pVcf->SetTextRC( nRow , 1 , _T("") );
		else
			pVcf->SetTextRC( nRow , 1 , _T("V") );
		
		for ( i = 1 ; i <= nMax ; i++ )
		{
			if ( pVcf->GetTextRC(i,1) == _T("V") && pVcf->GetTextRC(i,3) == _T("056") )
			{
				sTemp.Format(_T("%s"),sTmpTagData);
				sTagData = pVcf->GetTextRC(i, 2);
				sTmpTagData.Format(_T("%s%ca%s"), sTemp , 31 , sTagData );
			}
		}

		// �������� �������.
		if ( pVcf->GetTextRC( nRow , 1 ) == _T("V") )
			pVcf->SetTextRC( nRow , 1 , _T("") );
		else
			pVcf->SetTextRC( nRow , 1 , _T("V") );
		if ( !sTmpTagData.IsEmpty() )
			sMarc.Format(_T("%03d  %s%c24%c\r\n"),nTag, sTmpTagData, 31, 30);
        break;
    case  82:
		nMax = pVcf->GetMaxRow();
		// �������� ���.
		if ( pVcf->GetTextRC( nRow , 1 ) == _T("V") )
			pVcf->SetTextRC( nRow , 1 , _T("") );
		else
			pVcf->SetTextRC( nRow , 1 , _T("V") );
		for ( i = 1 ; i <= nMax ; i++ )
		{
			if ( pVcf->GetTextRC(i,1) == _T("V") && pVcf->GetTextRC(i,3) == _T("082") )
			{
				sTemp.Format(_T("%s"),sTmpTagData);
				sTagData = pVcf->GetTextRC(i, 2);
				sTmpTagData.Format(_T("%s%ca%s"), sTemp , 31 , sTagData );
			}
		}
		// �������� �������.
		if ( pVcf->GetTextRC( nRow , 1 ) == _T("V") )
			pVcf->SetTextRC( nRow , 1 , _T("") );
		else
			pVcf->SetTextRC( nRow , 1 , _T("V") );
		if ( !sTmpTagData.IsEmpty() )
			sMarc.Format(_T("%03d  %s%c221%c\r\n"),nTag, sTmpTagData, 31, 30);
        break;
	
	// ==============================================================================================
	// modified 2003.11.07 wsw
	// 650 7$a������$2�����߾ӵ�����������ǥ��ǥ -> 650 8$a������
	// ==============================================================================================
    case 650:    case 610:    case 611:    case 651:
        sMarc.Format(_T("%03d 8%ca%s\r\n"),nTag, 31, sTagData);
        break;
    case 600:    case 630:
        sMarc.Format(_T("%03d08%ca%s\r\n"),nTag, 31, sTagData);
        break;
    default:
		sMarc.Format(_T("%03d 8%ca%s\r\n"),nTag, 31, sTagData);
        break;
    }

    GetDlgItemText(IDC_EDT_SELECTED_TAG, m_sMarc);

	INT nPos = 0 , nPos1 = 0;
	CString sFind;

    // ���� ǥ�� ���
    if(pVcf->GetTextRC(nRow, 1) == _T("")) {
        // ���� �з���ȣ ����
		if ( nTag < 100 )
		{
			sFind.Format(_T("%03d  %ca"),nTag , 31);
			nPos = m_sMarc.Find(sFind , nPos1);
			if ( nPos >= 0 )
			{
				nPos1 = m_sMarc.Find(_T("\r\n"),nPos);
			}
			if( nPos >= 0 && nPos1 > 0 )
				m_sMarc.Replace(m_sMarc.Mid(nPos,nPos1-nPos) , _T(""));
            // �з���ȣ�� ���ʿ� ���δ�
            pVcf->SetTextRC(nRow, 1, _T("V"));
            m_sMarc = sMarc + m_sMarc;
        } else {
            // ��Ÿ tag�� ���ʿ� ���δ�
            pVcf->SetTextRC(nRow, 1, _T("V"));
            m_sMarc += sMarc;
        }
    } else {
		if( nTag < 100 )
		{
			sFind.Format(_T("%03d  %ca"),nTag , 31);
			nPos = m_sMarc.Find(sFind , nPos1);
			if ( nPos >= 0 )
			{
				nPos1 = m_sMarc.Find(_T("\r\n"),nPos);
			}
			if( nPos >= 0 && nPos1 > 0 )
				m_sMarc.Replace(m_sMarc.Mid(nPos,nPos1-nPos) , _T(""));
			m_sMarc = sMarc + m_sMarc;
		}
		else
			m_sMarc.Replace(sMarc, _T(""));
		pVcf->SetTextRC(nRow, 1, _T(""));
    }

    // ���ʿ��� ���� ����
    if(m_sMarc.Compare(_T("\r\n")) == 0) m_sMarc.Empty();
    else if(m_sMarc.GetLength() > 2) {
        if(m_sMarc.Right(2).Compare(_T("\r\n")) != 0) m_sMarc += _T("\r\n");
        if(m_sMarc.Left(2).Compare(_T("\r\n")) == 0) m_sMarc = m_sMarc.Mid(2);
        m_sMarc.Replace(_T("\r\n\r\n"), _T("\r\n"));
    }

    // �׸��� ������� �ٽ� ���
    if(bNeedSort) {
        INT nMaxRow = pVcf->GetMaxRow();
        INT nSelCnt = 0;
        INT j;
        CString *sMarcArray = new CString[nMaxRow];
        while((i = m_sMarc.Find(_T("\r\n"))) > 0) {
            sMarcArray[nSelCnt++] = m_sMarc.Left(i + 2);
            m_sMarc = m_sMarc.Mid(i + 2);
        }

        for(i = 0; i < nSelCnt; i++) {
            for(j = i; j < nSelCnt; j++) {
                if(sMarcArray[i].Compare(sMarcArray[j]) > 0) {
                    sMarc           = sMarcArray[i];
                    sMarcArray[i]   = sMarcArray[j];
                    sMarcArray[j]   = sMarc;
                }
            }
        }

        for(i = 0; i < nSelCnt; i++) {
            m_sMarc += sMarcArray[i];
        }

        delete [] sMarcArray;
    }

    // ȭ�鿡 ���÷���
    SetDlgItemText(IDC_EDT_SELECTED_TAG, m_sMarc);
    GetDlgItem (IDC_EDT_SELECTED_TAG)->SetFocus();
	*/

	return;
	
}

BOOL CCSS_2_02::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_������˻�_TAG�ݿ�")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_������˻�_TAG�ݿ�"));
	if (m_pCM == NULL) 
	{
		AfxMessageBox(_T("CM Init Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pDM = FindDM(_T("DM_������˻�_TAG�ݿ�"));
	if ( m_pDM == NULL ) 
	{
		AfxMessageBox(_T("DM Init Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	INT nTermsCnt = 0;
	CString sTerm;
	INT pos1 = 0, pos2 = 0;;

	do {
		m_pDM->InsertRow(-1);
        pos2 = m_sTerms.Find(_T("��"), pos1);
        if(pos2 < 0)sTerm = m_sTerms.Mid(pos1);
        else        sTerm = m_sTerms.Mid(pos1, pos2 - pos1);
        pos1 = pos2 + 2;

		m_pDM->SetCellData ( nTermsCnt, 0, sTerm );
		nTermsCnt++;

		//OnClickVcfSelectedTerm(nTermsCnt, -1);
    } while(pos2 >= 0);

	//TAG �߰�
	m_cTag.ResetContent();
	m_cTag.InsertString( 0, _T("056") );
	m_cTag.InsertString( 1, _T("082") );
	m_cTag.InsertString( 2, _T("600") );
	m_cTag.InsertString( 3, _T("610") );
	m_cTag.InsertString( 4, _T("611") );
	m_cTag.InsertString( 5, _T("630") );
	m_cTag.InsertString( 6, _T("650") );
	m_cTag.InsertString( 7, _T("651") );

	m_pCM->AllControlDisplay();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



VOID CCSS_2_02::OnDblClickVcfSelectedTerm() 
{
	/*
	CESL_Grid* pGrid;
	pGrid	= static_cast<CESL_Grid*>(m_pCM->FindControl(_T("GRID")));
	
	INT nRowIdx = pGrid->GetRow();
	INT nColIdx = pGrid->GetCol();
	if (nRowIdx < 1 || nColIdx < 1) return ;

	CString sOldGirdData = pGrid->GetTextMatrix(nRowIdx, nColIdx);

	CString strText;
	strText.Format(_T("%s"), pGrid->GetTextMatrix(0, nColIdx));

	if ( strText != _T("TAG") ) return ;

	ShowComboBox (pGrid);

	*/
	return;
}


INT CCSS_2_02::ShowComboBox(CESL_Grid *pGrid)
{
	EFS_BEGIN

	if (pGrid == NULL) return -1;

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if (nRow < 1 || nCol < 1) return -1;

	CRect rect;
	GetCellRect(pGrid, rect);
	
	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}

	m_pComboBox = new CEnvironComboBox_MultiState;

	m_pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(0,0,500,300), pGrid, 6450);

	CStringArray ArrCmbData;
	ArrCmbData.Add(_T("650"));		ArrCmbData.Add(_T("653"));
	m_pComboBox->Init(&ArrCmbData);
	m_pComboBox->SetIndex(nRow-1);
	m_pComboBox->SetDataMgr(m_pDM);
	m_pComboBox->SetParent(pGrid);
	m_pComboBox->SetHorizontalExtent(10);
	CFont * pFont = this->GetFont();
	m_pComboBox->SetFont(pFont);

	m_pComboBox->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());

	m_pComboBox->ShowWindow(TRUE);
	m_pComboBox->SetFocus();

	return 0;

	EFS_END
	return -1;
}

INT CCSS_2_02::GetCellRect(CESL_Grid *pGrid, CRect &rect)
{
	EFS_BEGIN

	if (pGrid == NULL) return -1;

	CRect rcGrid;
	CRect rcClient;

	pGrid->GetWindowRect(rcGrid);

	GetClientRect(rcClient);

	ClientToScreen((POINT*)&(rcClient.left));

	rcGrid.top -= rcClient.top;
	rcGrid.left -= rcClient.left;

	rect.left	= rcGrid.left	+ (long)((float)pGrid->GetCellLeft() / 15.0f);
	rect.top	= rcGrid.top	+ (long)((float)pGrid->GetCellTop() / 15.0f);
	rect.right	= rect.left		+ (long)((float)pGrid->GetCellWidth() / 15.0f) - 1;
	rect.bottom = rect.top		+ (long)((float)pGrid->GetCellHeight() / 15.0f) - 1;

	return 0;

	EFS_END
	return -1;
}

BOOL CCSS_2_02::SetDataToMarc () 
{
	//���õ� �����͸� ��ũ�� ���� ��Ų��.
	
	CString strTag = _T("") ;
	CComboBox *pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cTag );
	pCombo->GetWindowText( strTag );

	if ( strTag == _T("") )
	{
		AfxMessageBox ( _T("TAG�� ������ �ּ���."));
		return FALSE;
	}

	CESL_Grid* pGrid;
	pGrid	= static_cast<CESL_Grid*>(m_pCM->FindControl(_T("GRID")));
	
	pGrid->SelectMakeList();
	INT nGridIdx = pGrid->SelectGetHeadPosition();
	INT ids = -1;

	while ( nGridIdx > -1 )
	{
		ids = m_pMarcMgr->SetItem(m_pMarc, strTag + _T("$a"), pGrid->GetTextMatrix( nGridIdx+1 , 2 ) );
		//m_pMarcMgr->InsertField( m_pMarc, strTag + _T("$a") + idx );

		nGridIdx = pGrid->SelectGetNext();
	}

	return TRUE;
}

HBRUSH CCSS_2_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
