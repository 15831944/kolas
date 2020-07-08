// RevisionPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RevisionPrintDlg.h"

// CAT API
#include "..\MarcAdjustApi\MarcAdjustConstant.h"

#include "..\..\ESL\Marc\MarcEdit\MarcEditCtrl.h"
#include "..\..\ESL\SIReportMgr\SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRevisionPrintDlg dialog


CRevisionPrintDlg::CRevisionPrintDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CRevisionPrintDlg)
	m_bSearchRegNo = FALSE;
	//}}AFX_DATA_INIT

	m_nModeMONO_SERIAL = MODE_MONO;

	m_pDM_Parent = NULL;
	m_pGrid_Parent = NULL;

	m_pDM = NULL;
	m_pCM = NULL;
}


VOID CRevisionPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRevisionPrintDlg)
	DDX_Check(pDX, IDC_chkREG_NO, m_bSearchRegNo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRevisionPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CRevisionPrintDlg)
	ON_BN_CLICKED(IDC_bPRINT_CP, OnbPRINTCP)
	ON_BN_CLICKED(IDC_chkREG_NO, OnchkREGNO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRevisionPrintDlg Operations

INT CRevisionPrintDlg::Init()
{
	EFS_BEGIN
	if (!m_pDM_Parent || !m_pGrid_Parent|| m_strSpeciesKeyAlias.IsEmpty()) return -1;


	// ��� ���� ����
	m_pDM = FindDM(_T("DM_����_���_������"));
	m_pCM = FindCM(_T("CM_����_���_������"));

	if (!m_pDM || !m_pCM) return -1;

	return 0;

	EFS_END
	return -1;
}

INT CRevisionPrintDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias)
{
	EFS_BEGIN

	if (!pDM_Parent || !pGrid_Parent || strSpeciesKeyAlias.IsEmpty()) return -1;

	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strSpeciesKeyAlias = strSpeciesKeyAlias;

	return 0;

	EFS_END
	return -1;
}

INT CRevisionPrintDlg::PrintRevision()
{
	BeginWaitCursor();
	// DM ����
	CESL_DataMgr DM_Species;
	DM_Species.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	
	// Where ���� �����Ѵ�.
	CString strOption;
	CString strQuery;
	CStringArray arrStrSpeciesKey;

	if (m_bSearchRegNo)
	{
		CString strRegCode;
		CString strTemp;
		CString strFromRegNo;
		CString strToRegNo;
		

		m_pCM->GetControlMgrData(_T("��ϱ���"), strRegCode);
		m_pCM->GetControlMgrData(_T("���۵�Ϲ�ȣ"), strTemp);
		strFromRegNo.Format(_T("%s%010s"), strRegCode, strTemp);
		m_pCM->GetControlMgrData(_T("����Ϲ�ȣ"), strTemp);
		strToRegNo.Format(_T("%s%010s"), strRegCode, strTemp);
		

		if (strFromRegNo.IsEmpty() || strToRegNo.IsEmpty()) 
		{
			MessageBox(_T("��Ϲ�ȣ �˻������� �ٸ��� �Է��� �ּ���"));
			return -1;
		}

		//strOption.Format(_T("REC_KEY IN (SELECT DISTINCT SPECIES_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <= '%s') ORDER BY BO_GET_MIN_REG_NO(REC_KEY)"), strFromRegNo, strToRegNo);
		strQuery.Format(_T("SELECT MARC, ")
						_T("(SELECT MIN(REG_NO) FROM BO_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <= '%s' AND SPECIES_KEY = S.REC_KEY AND BOOK_APPENDIX_FLAG = 'B') AS REG_NO ")
						_T("FROM BO_SPECIES_TBL S WHERE S.REC_KEY IN ")
						_T("(SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <= '%s' AND BOOK_APPENDIX_FLAG = 'B') ")
						_T("ORDER BY REG_NO")
						, strFromRegNo, strToRegNo, strFromRegNo, strToRegNo);
	}
	else
	{
		// Grid ������ �� Key�� �����´�.
		m_pGrid_Parent->SelectMakeList();
		INT nCount = m_pGrid_Parent->SelectGetCount();
		if (nCount <= 0)
		{
			m_strLastErrMsg = _T("������ ����Ʈ�� �����ϴ�.");
			return -1;
		}

		// ���� ����Ʈ�� ���鼭 ��Ű�� �����´�.
		

		INT nIdx = m_pGrid_Parent->SelectGetHeadPosition();
		if (nIdx < 0) return -1;

		CString strSpeciesKey;
		while (nIdx >= 0)
		{
			// DM���� ��Ű�� �����´�.
			m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, nIdx, strSpeciesKey);
			if (!strSpeciesKey.IsEmpty())
				arrStrSpeciesKey.Add(strSpeciesKey);

			nIdx = m_pGrid_Parent->SelectGetNext();
		}

		// ����� ��Ű�� ������ ������ �����Ѵ�.
		strOption = _T("REC_KEY IN (");
		nCount = arrStrSpeciesKey.GetSize();
		for (INT idx = 0; idx < nCount; idx++)
		{
			strOption += arrStrSpeciesKey.GetAt(idx);
			if (idx != nCount - 1)
				strOption += _T(", ");
		}

		strOption += _T(")");

		// å �� WHERE ������ ���Ѵ�.
		CString strBookWhere = _T("1=1");
		CString strBookFrom = _T("BO_BOOK_TBL b");
		CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
		if (pDM_Where)
		{
			strBookWhere = pDM_Where->GetCellData(_T("å������"), 0);
			
			// From TBL �� �ٲ۴�.
			// ������ ���� ���̺��� �ֳ� �˻��Ѵ�.
			if (strBookWhere.Find(_T("a.")) >= 0)
			{
				// CO_ACCESSION_REC_TBL �� ����
				strBookFrom += _T(" , CO_ACCESSION_REC_TBL a ");
			}
			if (strBookWhere.Find(_T("p.")) >= 0)
			{
				// BO_PURCHASE_SEQ_NO_TBL �� ����
				strBookFrom += _T(" , BO_PURCHASE_SEQ_NO_TBL p ");
			}
		}

		//strOption += _T(") ORDER BY BO_GET_MIN_REG_NO(REC_KEY)");

		strQuery.Format(_T("SELECT MARC, ")
						_T("(SELECT MIN(REG_NO) FROM %s WHERE %s AND b.SPECIES_KEY = s.REC_KEY) AS REG_NO ")
						_T("FROM BO_SPECIES_TBL S WHERE %s ")
						_T("ORDER BY REG_NO")
						, strBookFrom, strBookWhere, strOption);
	}


	INT ids = DM_Species.RestructDataManager(strQuery);
	if (ids < 0) return -1;

	INT nRowCount = DM_Species.GetRowCount();
	if (nRowCount <= 0) 
	{
		AfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return 0;
	}

	m_pDM->FreeData();

	// Marc Editor�� �־��ٰ� ����.
	CString strEditMarc;
	CMarcEditCtrl MarcEditCtrl;
	MarcEditCtrl.Create(WS_CHILD | ES_MULTILINE, CRect(0,0,0,0), this, 10202);

	CMarc marc;
	INT iCount = 0;
	INT iRec = 0;
	CString sSpeciesKey;

	INT iRowCount = DM_Species.GetRowCount();
	for( INT i = 0 ; i < iRowCount ; i++ ) {
		CString strStreamMarc;
		CMarc marc;
		strStreamMarc = DM_Species.GetCellData(i, 0);
		
		if (m_pInfo->pMarcMgr->Decoding( strStreamMarc, &marc ) < 0) continue;
		MarcEditCtrl.Init(m_pInfo->pMarcMgr, &marc);
		MarcEditCtrl.Display();
			
		MarcEditCtrl.GetWindowText( strEditMarc );

		CString str;
		str.Format(_T("%c"), SUBFIELD_CODE);
		strEditMarc.Replace(str, _T("��"));

		str.Format(_T("%c"), FIELD_TERMINATOR);
		strEditMarc.Replace(str, _T("��"));

		CString sLeader;
		marc.GetLeader(sLeader);
		strEditMarc = sLeader + _T("��") + strEditMarc;
		
		//��ũ ¥����
		INT iStart = 0;
		
/* ���� ���� : ������ ��Ϲ�ȣ�� �˻� �����ϵ��� ���� +++
 * ���� ��¥ : 2003.09.3
 * ���� ��� : ��ȫ�� 
 */

		if (arrStrSpeciesKey.GetSize() != 0 )
		{
			sSpeciesKey = arrStrSpeciesKey.GetAt(iRec);
			iRec++;
		}

// ���� ���� : ������ ��Ϲ�ȣ�� �˻� �����ϵ��� ���� ---

		while ( TRUE ) {
			INT iPos = strEditMarc.Find(_T("��"));
			if ( iPos == 0 ) {
				iPos = strEditMarc.Find(_T("\r\n"));
			}
			CString sData = strEditMarc.Mid( iStart, iPos+2 );			
			strEditMarc = strEditMarc.Mid( iPos+2 );
			strEditMarc.TrimLeft();
			INT iLineWordCount = 1000;

			



			while ( sData.GetLength() > iLineWordCount ) {
				CString sDataSub = sData.Left( iLineWordCount );
				BOOL bCh = IsCanCut( sDataSub, --iLineWordCount );
				if ( bCh == FALSE ) {
					iLineWordCount;
					sDataSub = sData.Left( iLineWordCount );
				}
				m_pDM->InsertRow(-1);
				m_pDM->SetCellData( _T("MarcLine"), sDataSub, iCount );
				iCount++;
				sData = sData.Mid(iLineWordCount);
			}
			iStart = 0;
			m_pDM->InsertRow(-1);
			m_pDM->SetCellData( _T("MarcLine"), sData, iCount );
			iCount++;

			
			
			/* ���� ���� : ������ ��½� û����ȣ �ߺ����簡 �ǵ��� ���� +++
			 * ���� ��¥ : 2003.08.24
			 * ���� ��� : ��ȫ�� 
			 */
			
			if ( sData.Left(3) == _T("090") ) {

				CString strCss;
				ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("090$a"), strCss);
				// û����ȣ�� �����ϴ��� üũ

				CString strBookCode;
				m_pInfo->pMarcMgr->GetItem(&marc,_T("090$b"),strBookCode);


				CString strWhere;
				//strWhere.Format(_T("SELECT CLASS_NO, BOOK_CODE, VOL_CODE, COPY_CODE , A.ACCESSION_REC_NO, REG_NO FROM BO_BOOK_TBL B, CO_ACCESSION_REC_TBL A WHERE b.accession_rec_key = a.rec_key AND CLASS_NO = '%s' AND BOOK_CODE = '%s' AND B.SPECIES_KEY != '%s'"), strCss, strBookCode, sSpeciesKey);
				//strWhere.Format(_T("SELECT CLASS_NO, BOOK_CODE, VOL_CODE, COPY_CODE , A.ACCESSION_REC_NO, REG_NO FROM BO_BOOK_TBL B, CO_ACCESSION_REC_TBL A WHERE b.book_appendix_flag = 'B' And b.accession_rec_key = a.rec_key AND CLASS_NO = '%s' AND BOOK_CODE = '%s' AND B.SPECIES_KEY != '%s'"), strCss, strBookCode, sSpeciesKey);


				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//2003.09.08 ������ 
				//strWhere.Format(_T("SELECT CLASS_NO, BOOK_CODE, VOL_CODE, COPY_CODE , A.ACCESSION_REC_NO, REG_NO FROM BO_BOOK_TBL B, CO_ACCESSION_REC_TBL A WHERE b.accession_rec_key = a.rec_key AND CLASS_NO = '%s' AND BOOK_CODE = '%s' AND B.SPECIES_KEY != '%s'"), strCss, strBookCode, sSpeciesKey);
				strBookCode.Replace(_T("'"),_T("''"));
				strWhere.Format(_T("SELECT CLASS_NO, BOOK_CODE, VOL_CODE, COPY_CODE, REG_NO FROM BO_BOOK_TBL B WHERE CLASS_NO = '%s' AND BOOK_CODE = '%s' AND B.SPECIES_KEY != '%s'"), strCss, strBookCode, sSpeciesKey);


				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


				CString strBookCodeTemp;
				CString strBookCodeTemp1;

				CESL_DataMgr DM_SPECIES;
				DM_SPECIES.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
				
				DM_SPECIES.FreeData();
				DM_SPECIES.RestructDataManager( strWhere );
				INT iSpeciesCount = DM_SPECIES.GetRowCount();
			
				INT strCssLen = strCss.GetLength();
				INT strBookCodeLen = strBookCode.GetLength();
				INT vlen = 1;
				INT tmp = DM_SPECIES.GetRowCount();

				//if (strBookCodeLen >= 1)
				for (INT xx = 1 ; xx < tmp ; xx++);
				{
					for (INT y = 1; y < iSpeciesCount-1 ; y ++)
					{
						for (INT x = 1; x < 5 ; x ++)
						{	
							//�տ������� ����
	
							if (x == 1 && DM_SPECIES.GetCellData(y,1) != _T(""))
							{
								for (INT z = 0; z < strCssLen + 15 ; z++ ) 
								{
									strBookCodeTemp.Format(_T(" "));
									strBookCodeTemp1 = strBookCodeTemp1 + strBookCodeTemp;
								}
									strBookCodeTemp.Format(_T("��b%s"), DM_SPECIES.GetCellData(y,1));
									strBookCodeTemp1 = strBookCodeTemp1 + strBookCodeTemp;
							}
	

							if (x == 2 && DM_SPECIES.GetCellData(y,2) != _T(""))
							{							
								strBookCodeTemp.Format(_T("��c%s"), DM_SPECIES.GetCellData(y,2));
								strBookCodeTemp1 = strBookCodeTemp1 + strBookCodeTemp;
							}

							if (x == 3 && DM_SPECIES.GetCellData(y,3) != _T(""))
							{
								/*
								for (INT z = 0; z < strBookCodeTemp1.GetLength() ; z++ ) 
								{
									strBookCodeTemp.Format(_T(" "));
									strBookCodeTemp1 = strBookCodeTemp1 + strBookCodeTemp;
								}
								
								strBookCodeTemp.Format(_T("��v%s"), DM_SPECIES.GetCellData(y,3));
								strBookCodeTemp1 = strBookCodeTemp1 + strBookCodeTemp;
								vlen = 0;
								*/
							}	
							
							//�ڿ��� ���� ����

							if (x == 4 && DM_SPECIES.GetCellData(y,4) != _T(""))
							{	
								//INT sDmLen;
								INT iTmp = strBookCodeTemp1.GetLength();
								for (INT z = 0; z < (90 - strBookCodeTemp1.GetLength()) ; z++ ) 
								{
									strBookCodeTemp.Format(_T(" "));
									strBookCodeTemp1 = strBookCodeTemp1 + strBookCodeTemp;
								}
								
								strBookCodeTemp.Format(_T("%s"), DM_SPECIES.GetCellData(y,4));
								strBookCodeTemp1 = strBookCodeTemp1 + strBookCodeTemp;
							}	

							if (x == 5 && DM_SPECIES.GetCellData(y,5) != _T(""))
							{
								for (INT z = 0; z < 5 ; z++ ) 
								{
									strBookCodeTemp.Format(_T(" "));
									strBookCodeTemp1 = strBookCodeTemp1 + strBookCodeTemp;
								}
								strBookCodeTemp.Format(_T("%s"), DM_SPECIES.GetCellData(y,5));
								strBookCodeTemp1 = strBookCodeTemp1 + strBookCodeTemp;
							}	


						}
						if (strBookCodeTemp1 != _T("")) 
						{
							iCount++;
							m_pDM->InsertRow(-1);
							m_pDM->SetCellData(_T("MarcLine"), strBookCodeTemp1, iCount-1);
							strBookCodeTemp1 = _T("");
						}
					}
				}
				
			// ���� ���� : ������ ��½� û����ȣ �ߺ����簡 �ǵ��� ���� ---
			}

			if ( iPos == -1 ) {
				break;
			}
		}

		m_pDM->InsertRow(-1);
		m_pDM->SetCellData( _T("MarcLine"), _T(""), iCount );
		iCount++;


	}	

	
	CSIReportManager SIReportManager(this);
	SIReportManager.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = SIReportManager.GetSIReportFileFromDB(_T("�Է±�����"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return -1;
	}
	
	SIReportManager.SetDataMgr(0, m_pDM, 0);

	
	EndWaitCursor();
	
	ids = SIReportManager.Print();
	return 0;
}

BOOL CRevisionPrintDlg::IsCanCut(CString sStr, INT nPos)
{
EFS_BEGIN

	INT i, nLastPos = -1;
	//���ڼ� �ʰ��� FALSE
	if(nPos > sStr.GetLength())
		return FALSE;
	TCHAR ch = sStr.GetAt(nPos);
	//������ ���� || �ѱ��� �ƴϸ� TRUE
	if(nPos + 1 == sStr.GetLength() || (ch & (0x80)) != 128)
		return TRUE;
	//ù���ڰ� 1byte���ڸ� TRUE �ƴϸ� FALSE
	if(nPos == 0)
		if((ch & (0x80)) != 128)
			return TRUE;
		else
			return FALSE;

	for(i = 0; i < nPos; i++) {
		ch = sStr.GetAt(i);
		//�ڸ��� �κп��� ���� �����(��������)������ ��ġ
		if((ch & (0x80)) != 128)
			nLastPos = i;
	}
	if (nLastPos == -1 && nPos % 2 == 1) return FALSE;
	//¦���� ���̳��� TRUE
	return !((nPos - nLastPos) % 2);

EFS_END
return FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// CRevisionPrintDlg message handlers

BOOL CRevisionPrintDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_����_���_������")) < 0) 
	{
		m_strLastErrMsg = _T("Init ESL Mgr Error!!");
		goto ERR;
	}

	if (Init() < 0) goto ERR;
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CRevisionPrintDlg::OnbPRINTCP() 
{
	INT ids = PrintRevision();
}

VOID CRevisionPrintDlg::OnchkREGNO() 
{
	UpdateData();
	if (m_bSearchRegNo)
	{
		GetDlgItem(IDC_cCLASS_CODE_CP)->EnableWindow(TRUE);
		GetDlgItem(IDC_REG_NO_START_CP)->EnableWindow(TRUE);
		GetDlgItem(IDC_eREG_NO_END_CP)->EnableWindow(TRUE);		
	}
	else
	{
		GetDlgItem(IDC_cCLASS_CODE_CP)->EnableWindow(FALSE);
		GetDlgItem(IDC_REG_NO_START_CP)->EnableWindow(FALSE);
		GetDlgItem(IDC_eREG_NO_END_CP)->EnableWindow(FALSE);		
	}
}

BOOL CRevisionPrintDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnbPRINTCP();
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
