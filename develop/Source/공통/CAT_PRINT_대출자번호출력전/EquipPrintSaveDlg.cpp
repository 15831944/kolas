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
	
	// ��� ���� ����
	m_pDM = FindDM(_T("DM_����_���_���_å"));	

	if (!m_pDM ) return -1;	

	// ���� ����Ʈ üũ�Ѵ�.
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

	// ���� ����Ʈ�� ���鼭 ��Ű�� �����´�.	
	CStringArray arrStrSpeciesKey;

	INT nIdx = m_pGrid_Parent->SelectGetHeadPosition();
	//if (nIdx < 0) return -1;
	if (nIdx < 0 && m_arrStrSpeciesKey.GetSize() < 1) return -1;

	CString strSpeciesKey;
	while (nIdx >= 0)
	{
		// DM���� ��Ű�� �����´�.
		m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, nIdx, strSpeciesKey);
		if (!strSpeciesKey.IsEmpty())
			arrStrSpeciesKey.Add(strSpeciesKey);

		nIdx = m_pGrid_Parent->SelectGetNext();
	}
	
	// ����ڷ� ������ ������ ������ �� ������ ����Ѵ�.
	INT iSwSave = 0;	// ����ڷ� ������ ������ ������ 1, ������ 0
	if ( m_arrStrSpeciesKey.GetSize() > 0 ) {
		arrStrSpeciesKey.RemoveAll();
		for ( i = 0 ; i < m_arrStrSpeciesKey.GetSize() ; i++ ) {
			arrStrSpeciesKey.Add( m_arrStrSpeciesKey.GetAt(i) );
		}
		//AfxMessageBox( _T("����ڷ� ������ ������ ����մϴ�.") );
		iSwSave = 1;		
	}

	// ����� ��Ű�� ������ ������ �����Ѵ�.	
	CString strOption = _T("I.REC_KEY IN (");
	nCount = arrStrSpeciesKey.GetSize();
	for ( i = 0; i < nCount; i++)
	{
		strOption += arrStrSpeciesKey.GetAt(i);
		if (i != nCount - 1)
			strOption += _T(", ");
	}	

	strOption += _T(")");

	// ���� ���ӿ� ���� ���̺� �̸��� �� ������ �����Ѵ�.
	CString strFieldName;
	if (m_nModeMONO_SERIAL == 0)
	{
		m_pDM->TBL_NAME = _T("BO_BOOK_TBL B, IDX_BO_TBL I");
		strFieldName = _T("DECODE(BOOK_APPENDIX_FLAG, 'A', '�η�')");
	}
	else
	{
		m_pDM->TBL_NAME = _T("SE_BOOK_TBL B, IDX_SE_TBL I");
		strFieldName = _T("DECODE(BOOK_TYPE, 'A', '�η�')");
	}

	// å�ηϿ� ���� ������ �ٲپ��ش�.
	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = m_pDM->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)m_pDM->RefList.GetNext(pos);
		if (pRef && pRef->FIELD_ALIAS == _T("å�η�"))
		{
			pRef->FIELD_NAME = strFieldName;
			break;
		}		
	}

 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // 2003.08.23 ������ ����
    // �����߱��� ������ ����å ������ ���� ���� �ʰ� ����.  
    
	//strOption += _T("AND B.DUP_SPECIES_KEY IS NULL ") ;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
    CString bookWhere ;
	pDM_Where->GetCellData (_T("å������"),0,bookWhere) ;
    
	CString strFrom = m_pDM->TBL_NAME ;

	if(bookWhere.Find (_T("a."))>=0)
	{
		strFrom +=  _T(" , co_accession_rec_tbl a ") ;
		strOption = strOption + _T(" AND B.ACCESSION_REC_KEY = A.REC_KEY");
	}
	
	if(bookWhere.Find (_T("p."))>=0) 
	{
        strFrom +=  _T(" , BO_PURCHASE_SEQ_NO_TBL p  ") ;
	}
    
	m_pDM->TBL_NAME = strFrom ;
	//ids = m_pDM->RefreshDataManager(strOption);

    if ( iSwSave == 0 ) {	// ����ڷẸ���� ������ ������ å������ ����
		strOption += _T(" and ") +bookWhere ;
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if ( iSwSave == 1 ) {
		CESL_DataMgr *pDmDeleteBook;
		pDmDeleteBook = FindDM(_T("DM_��������ȣ"));
		if ( NULL == pDmDeleteBook ) {
			AfxMessageBox ( _T("pDmDeleteBook error") );
			return -1;
		}
		//����åKEY
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

	//strOption = strOption + _T(" AND I.REC_KEY = B.SPECIES_KEY AND B.REG_NO = 'EM0000000011'");
	ids = m_pDM->RefreshDataManager(strOption);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("�����ͺ��̽��� �۾��߿� ������ �߻��߽��ϴ�.");
		return ids;
	}

	// ���� ������ ���� ����
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
	// ������� �� û����ȣ�� ������ �ش�. (û����ȣ : ��ġ��ȣ �з���ȣ-������ȣ-��å��ȣ=������ȣ)
	CString strCallNo;
	CString strItem;
	nCount = m_pDM->GetRowCount();
	for (i = 0; i < nCount; i++)
	{
		// ���� ����(2004.10.08)
		// û����ȣ ���� Format 2(�ٹٲ����)
		strCallNo = MakeCallNoPrintFormat2(i);

		// û����ȣ
		ids = m_pDM->SetCellData(_T("û����ȣ"), strCallNo, i);

		// �������
		ids = m_pDM->GetCellData(_T("���̺���±���"), i, strItem);
		//if (strItem.IsEmpty())
		ids = m_pDM->SetCellData(_T("���̺���±���"), _T("2"), i);
	}

	// �˻��� ����� �����Ѵ�.
	CESL_DataMgr *m_pSaveDM;
	const INT nColCnt = 19;
	CString sColAliasData[nColCnt];
	CString sColAlias[nColCnt] =
	{
		_T("������ȣ"), _T("�з���ȣ"), _T("������ȣ"), _T("���̺���±���"), _T("��Ϲ�ȣ"),
		_T("RFID_SERIAL"), _T("��ġ��ȣ_�ڵ�"), _T("�谡�ڵ�"), _T("��å��ȣ"), _T("�۾�����"), 
		_T("������"), _T("������"), _T("BS_��KEY"), _T("��ǥ��"), _T("û����ȣ"), 
		_T("����"), _T("����åKEY"), _T("å�η�"), _T("BB_åKEY")
	};
	
	m_pSaveDM = FindDM(_T("DM_����_���_���_å_����"));
	nCount = m_pDM->GetRowCount();
	nIdx = m_pSaveDM->GetRowCount();

	CString strBookKey2;
	BOOL	bCheck;
	nIdx = m_pSaveDM->GetRowCount();
	for ( i=0 ; i<nCount ; i++ )
	{	
		// �ߺ�����, �̹� ������ �ڷ��̸� SKIP
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
		
		m_pDM->GetCellData(sColAlias, 19, sColAliasData, i );				
		ids = m_pSaveDM->InsertRow(-1);
		m_pSaveDM->SetCellData( sColAlias , 19, sColAliasData , nIdx );
		nIdx++;
	}

	nIdx = m_pSaveDM->GetRowCount();
	strItem.Format(_T("�����ڷᰡ �����Ǿ����ϴ�.\n������ å�� : %d\n ��ü å�� : %d"),nCount, nIdx);
	AfxMessageBox(strItem);	

	return 0;

	EFS_END
	return -1;
}

//INT CEquipPrintSaveDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias)
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
	
	//�ӽ÷� ����.. �Ф�;; ��� ������ ������.. ����..
	CString sQuery = _T("");
	CString sValue = _T("") ;
	sQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '����' AND GROUP_3 = '����' AND CLASS_ALIAS = '���̺��å��ȣ����'");
	m_pDM_Parent->GetOneValueQuery( sQuery , sValue );	
	sValue.TrimLeft(); sValue.TrimRight();

	INT ids = m_pDM->GetCellData(_T("��ġ��ȣ_�ڵ�"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		// ��ġ��ȣ�� �������� �ٲ۴�.
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