// EquipPrintSaveDlg.cpp: implementation of the CEquipPrintSaveDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EquipPrintSaveDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEquipPrintSaveDlg::CEquipPrintSaveDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_nModeMONO_SERIAL = 0;
}

CEquipPrintSaveDlg::~CEquipPrintSaveDlg()
{		
}

BOOL CEquipPrintSaveDlg::Create(CWnd *pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

INT CEquipPrintSaveDlg::Init()
{
	EFS_BEGIN

	if (!m_pDM_Parent || !m_pGrid_Parent|| m_strSpeciesKeyAlias.IsEmpty()) return -1;

	CString strBookKey;
	INT i ,j;
	
	m_pDM = FindDM(_T("DM_����_���_���_å"));	

	if (!m_pDM ) return -1;	

	INT ids = -1;
	ids = m_pGrid_Parent->SelectMakeList();
	if (ids < 0) return ids;

	INT nCount = m_pGrid_Parent->SelectGetCount();
	if (nCount <= 0)
	{
		if ( m_arrStrSpeciesKey.GetSize() < 1 ) {
			m_strLastErrMsg = _T("������ �ڷᰡ �����ϴ�.");
			return -1;
		}
	}

	CStringArray arrStrSpeciesKey;

	INT nIdx = m_pGrid_Parent->SelectGetHeadPosition();

	if (nIdx < 0 && m_arrStrSpeciesKey.GetSize() < 1) return -1;

	CString strSpeciesKey;
	while (nIdx >= 0)
	{
		m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, nIdx, strSpeciesKey);
		if (!strSpeciesKey.IsEmpty())
			arrStrSpeciesKey.Add(strSpeciesKey);

		nIdx = m_pGrid_Parent->SelectGetNext();
	}
	
	INT iSwSave = 0;
	if ( m_arrStrSpeciesKey.GetSize() > 0 ) {
		arrStrSpeciesKey.RemoveAll();
		for ( i = 0 ; i < m_arrStrSpeciesKey.GetSize() ; i++ ) {
			arrStrSpeciesKey.Add( m_arrStrSpeciesKey.GetAt(i) );
		}
		iSwSave = 1;		
	}

	CString strOption = _T("I.REC_KEY IN (");
	nCount = arrStrSpeciesKey.GetSize();
	for ( i = 0; i < nCount; i++)
	{
		strOption += arrStrSpeciesKey.GetAt(i);
		if (i != nCount - 1)
			strOption += _T(", ");
	}	

	strOption += _T(")");

	CString strAppendix;
	CString strVol;
	CString strPage;
	CString strPublishYear;
	CString strVolTitle;

	if (m_nModeMONO_SERIAL == 0)
	{
		m_pDM->TBL_NAME = _T("BO_BOOK_TBL B, IDX_BO_TBL I");
		strAppendix = _T("DECODE(BOOK_APPENDIX_FLAG, 'A', '�η�')");
		strVol = _T("B.VOL");
		strPage = _T("B.PAGE");
		strPublishYear = _T("B.PUBLISH_YEAR");
		strVolTitle = _T("B.VOL_TITLE");
	}
	else
	{
		m_pDM->TBL_NAME = _T("SE_BOOK_TBL B, IDX_SE_TBL I");
		strAppendix = _T("DECODE(BOOK_TYPE, 'A', '�η�')");
		strVol = _T("B.VOL_CODE");
		strPage = _T("(SELECT PAGE FROM SE_VOL_TBL WHERE B.VOL_KEY = REC_KEY)");
		strPublishYear = _T("(SELECT SUBSTR(PUBLISH_DATE,0,4) FROM SE_VOL_TBL WHERE B.VOL_KEY = REC_KEY)");
		strVolTitle = _T("(SELECT VOL_TITLE FROM SE_VOL_TBL WHERE B.VOL_KEY = REC_KEY)");
	}

	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = m_pDM->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)m_pDM->RefList.GetNext(pos);
		if (pRef && pRef->FIELD_ALIAS == _T("å�η�"))
		{
			pRef->FIELD_NAME = strAppendix;
		}		
		else if (pRef && pRef->FIELD_ALIAS == _T("�����"))
		{
			pRef->FIELD_NAME = strVol;
		}
		else if (pRef && pRef->FIELD_ALIAS == _T("�����"))
		{
			pRef->FIELD_NAME = strPage;
		}
		else if (pRef && pRef->FIELD_ALIAS == _T("�����"))
		{
			pRef->FIELD_NAME = strPublishYear;
		}
		else if (pRef && pRef->FIELD_ALIAS == _T("�Ǽ���"))
		{
			pRef->FIELD_NAME = strVolTitle;
		}
	}

	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
    CString bookWhere ;
	pDM_Where->GetCellData (_T("å������"),0,bookWhere) ;
    
	CString strFrom = m_pDM->TBL_NAME ;

	if(bookWhere.Find (_T("a."))>=0 || bookWhere.Find (_T("A."))>=0)
	{
		strFrom +=  _T(" , co_accession_rec_tbl a ") ;
		strOption = strOption + _T(" AND B.ACCESSION_REC_KEY = A.REC_KEY");
	}
	
	if(bookWhere.Find (_T("p."))>=0 || bookWhere.Find (_T("P."))>=0) 
	{
        strFrom +=  _T(" , BO_PURCHASE_SEQ_NO_TBL p  ") ;
		strOption = strOption + _T(" AND P.MANAGE_CODE = UDF_MANAGE_CODE");
	}
    
	m_pDM->TBL_NAME = strFrom ;

    if ( iSwSave == 0 ) {
		strOption += _T(" and ") +bookWhere ;
	}

	if ( iSwSave == 1 ) {
		CESL_DataMgr *pDmDeleteBook;
		pDmDeleteBook = FindDM(_T("DM_��������ȣ"));
		if ( NULL == pDmDeleteBook ) {
			AfxMessageBox ( _T("pDmDeleteBook error") );
			return -1;
		}

		CString sDeleteBookKey, sDeleteBookKeyList;
		INT iDeleteCount = pDmDeleteBook->GetRowCount();
		for ( INT i = 0 ; i < iDeleteCount ; i++ ) {
			sDeleteBookKey = pDmDeleteBook->GetCellData( _T("����åKEY"), i );
			sDeleteBookKeyList = sDeleteBookKeyList + _T(",") + sDeleteBookKey;
		}
		
		if ( sDeleteBookKeyList != _T("") ) {
			sDeleteBookKeyList = sDeleteBookKeyList.Mid(1);
			strOption = strOption + _T(" AND B.REC_KEY NOT IN (") + sDeleteBookKeyList + _T(")");
		}
	}

	ids = m_pDM->RefreshDataManager(strOption);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("�����ͺ��̽��� �۾��߿� ������ �߻��߽��ϴ�.");
		return ids;
	}

	for ( i = m_pDM->GetRowCount()-1 ; i >= 0 ; i-- ) {
		CString sBookKey = m_pDM->GetCellData( _T("BB_åKEY"), i );
		INT iBookCount = m_arrStrDeleteBookKey.GetSize();
		for ( INT j = 0 ; j < m_arrStrDeleteBookKey.GetSize() ; j++ ) {
			if ( sBookKey == m_arrStrDeleteBookKey.GetAt(j) ) {
				m_pDM->DeleteRow(i);
				break;
			}
		}
	}
	CString strCallNo;
	CString strItem;
	nCount = m_pDM->GetRowCount();
	for (i = 0; i < nCount; i++)
	{
		strCallNo = MakeCallNoPrintFormat2(i);

		ids = m_pDM->SetCellData(_T("û����ȣ"), strCallNo, i);

		ids = m_pDM->GetCellData(_T("���̺���±���"), i, strItem);

		ids = m_pDM->SetCellData(_T("���̺���±���"), _T("2"), i);
	}

	CESL_DataMgr *m_pSaveDM;
	
	const INT nColCnt = 23;

	CString sColAliasData[nColCnt];
	CString sColAlias[nColCnt] =
	{
		_T("������ȣ"), _T("�з���ȣ"), _T("������ȣ"), _T("���̺���±���"), _T("��Ϲ�ȣ"),
		_T("RFID_SERIAL"), _T("��ġ��ȣ_�ڵ�"), _T("�谡�ڵ�"), _T("��å��ȣ"), _T("�۾�����"), 
		_T("������"), _T("������"), _T("BS_��KEY"), _T("��ǥ��"), _T("û����ȣ"), 
		_T("����"), _T("����åKEY"), _T("å�η�"), _T("BB_åKEY")	
		,_T("�����"), _T("�����"), _T("�����"), _T("�Ǽ���")
	};
	
	m_pSaveDM = FindDM(_T("DM_����_���_���_å_����"));
	nCount = m_pDM->GetRowCount();
	nIdx = m_pSaveDM->GetRowCount();

	CString strBookKey2;
	BOOL	bCheck;
	nIdx = m_pSaveDM->GetRowCount();
	for ( i=0 ; i<nCount ; i++ )
	{
		m_pDM->GetCellData(_T("BB_åKEY"), i, strBookKey );
		bCheck = FALSE;

		for ( j=0 ; j<nIdx ; j++ )
		{
			m_pSaveDM->GetCellData(_T("BB_åKEY"), j, strBookKey2 );
			if ( strBookKey == strBookKey2 ) 
			{
				bCheck = TRUE;
				break;
			}
		}

		if ( bCheck ) continue;
				
		m_pDM->GetCellData(sColAlias, 23, sColAliasData, i );				
		ids = m_pSaveDM->InsertRow(-1);
		m_pSaveDM->SetCellData( sColAlias , 23, sColAliasData , nIdx );
			
		nIdx++;
	}

	nIdx = m_pSaveDM->GetRowCount();
	strItem.Format(_T("�����ڷᰡ �����Ǿ����ϴ�.\n������ å�� : %d\n ��ü å�� : %d"),nCount, nIdx);
	AfxMessageBox(strItem);	

	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintSaveDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias, CStringArray *arrStrSpeciesKey)
{
	EFS_BEGIN

	if (!pDM_Parent || !pGrid_Parent || strSpeciesKeyAlias.IsEmpty()) return -1;

	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strSpeciesKeyAlias = strSpeciesKeyAlias;

	for ( INT i = 0 ; i < arrStrSpeciesKey->GetSize() ; i++ ) {
		m_arrStrSpeciesKey.Add( arrStrSpeciesKey->GetAt(i) );
	}	
	
	return 0;

	EFS_END
	return -1;
}


CString CEquipPrintSaveDlg::MakeCallNoPrintFormat2(INT idx )
{
	CString strCallNo = _T("");
	CString strItemData=_T("");
	INT nHeight;

	nHeight = 0;
	
	CString sQuery = _T("");
	CString sValue = _T("") ;
	sQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_1 = 'L' AND GROUP_2 = '����' AND GROUP_3 = '����' AND CLASS_ALIAS = '���̺��å��ȣ����'");
	m_pDM_Parent->GetOneValueQuery( sQuery , sValue );	
	sValue.TrimLeft(); sValue.TrimRight();

	INT ids = m_pDM->GetCellData(_T("��ġ��ȣ_�ڵ�"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��ġ��ȣ"), strItemData, strItemData);
		strCallNo = strItemData + _T("\r\n");		
	}
	
	ids = m_pDM->GetCellData(_T("�з���ȣ"), idx, strItemData);
	if (!strItemData.IsEmpty())		
	{
		strCallNo +=  strItemData;		
	}
	
	ids = m_pDM->GetCellData(_T("������ȣ"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		strCallNo += _T("\r\n") + strItemData;
	}

	ids = m_pDM->GetCellData(_T("��å��ȣ"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		strCallNo += _T("\r\n") + sValue + strItemData;
	}

	ids = m_pDM->GetCellData(_T("������ȣ"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		strCallNo += _T("\r\nc.") + strItemData;
	}

	

	return strCallNo;
}
