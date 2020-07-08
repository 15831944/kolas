// RegNo_Creator.cpp: implementation of the CRegNo_Creator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegNo_Creator.h"
#include "MakeSearchData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegNo_Creator::CRegNo_Creator(CESL_Mgr * pMgr):CSPF_Work(pMgr)
{

}

CRegNo_Creator::~CRegNo_Creator()
{

}

INT CRegNo_Creator::GetLastRegNo()
{
	CString regCode, CMAlias, DMAlias;
	INT lastRegno;
	GetTempData(_T("RegCode"), regCode);
	GetTempData(_T("CMAlias"), CMAlias);
	GetTempData(_T("DMAlias"), DMAlias);
	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);
	if(regCode.GetLength() != 0)
		GetLastRegNo(DMAlias, regCode, &lastRegno);	
	SetTempData(_T("LastRegNo"), lastRegno);
	return 0;
}

INT CRegNo_Creator::GetLastRegNo(CString DMAlias, CString regCode, INT *regno)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(regCode.GetLength() == 0) return -1000;
	CString tmpquery, tmpvalue;
	//++2008.10.16 UPDATE BY PWR {{++
	// ���սý��� ����
	tmpquery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					_T("WHERE DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), regCode );
	// tmpquery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE DIVISION_VALUE = '%s'"), regCode);
	//--2008.10.16 UPDATE BY PWR --}}
	pDM->GetOneValueQuery(tmpquery, tmpvalue);
	*regno = _ttoi(tmpvalue.GetBuffer(0));
	return 0;
}

INT CRegNo_Creator::EARegProcessing()
{
	CString DMAlias, CMAlias;
	INT idx, lastRegno, opcode;
	GetTempData(_T("DMAlias"), DMAlias);
	GetTempData(_T("CMAlias"), CMAlias);
	GetTempData(_T("DMIndex"), idx);
	GetTempData(_T("LastRegNo"), lastRegno);
	GetTempData(_T("OperationCode"), opcode);
	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	CString regno;
	pCM->GetControlMgrData(_T("��Ϲ�ȣ"), regno);
	
	if(opcode == 1000 || opcode == 2000)
	{
		//����� �Է°� �˻�
		INT ids = IsValidRegNo(regno);
		if(ids < 0) return ids;
		//�ߺ��˻�
		if(IsDuplcateRegNo(pDM, regno)) return -2000;
	}
	
	//��Ϻο� Ÿ��
	//1000:������Ϲ�ȣ, 2000:��������Ϲ�ȣ, 3000:��������Ϲ�ȣ�ε� ������Ϲ�ȣ����
	CStringList regnoList;
	INT type = CheckRegNoType(lastRegno, regno, regnoList, opcode);
	//å�� ������������ �� AddFrame
	INT ids = MakeQueryAndAddFrame(pDM, regno, idx, opcode);
	if(ids < 0) return ids;
	//������ ��Ϲ�ȣ ����
	if(type == 1000)
	{
		//������Ϲ�ȣ���� �ش��ȣ����
		ids = MakeMissingRegNoUpdate(pDM, regno, regnoList);
	}
	else if(type == 2000)
	{
		//������ ��Ϲ�ȣ ����
		ids = MakeLastRegNoUpdate(pDM, regno);
	}
	else if(type == 3000)
	{
		//������Ϲ�ȣ ���� �� ������ ��Ϲ�ȣ ����
		CString first, last, regcode, tmpstr;
		first = regnoList.GetHead();
		last = regnoList.GetTail();
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		regcode = regno.Left(2);
		INT nRegCodeLength;
		nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
		regcode = regno.Left(nRegCodeLength);
		//===================================================================
		
		tmpstr.Format(_T("[%s%s]���� [%s%s]������\n������Ϲ�ȣ�� �����Ͻðڽ��ϱ�?"), regcode, first, regcode, last);
		if(IDCANCEL == AfxMessageBox(tmpstr.GetBuffer(0), MB_OKCANCEL))
			return 0;
		MakeMissingRegNoUpdate(pDM, regno, regnoList);
		MakeLastRegNoUpdate(pDM, regno);
		regnoList.RemoveAll();
	}
	else if(type == 4000)
	{
		regnoList.RemoveAll();
		regnoList.AddTail(regno);
		MakeMissingRegNoUpdate(pDM, regno, regnoList);
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -4000;
	if(opcode == 1000 || opcode == 2000)
		pDM->SetCellData(_T("BB_��Ϲ�ȣ"), regno, idx);
	else if(opcode == -1000 || opcode == -2000)
		pDM->SetCellData(_T("BB_��Ϲ�ȣ"), _T(""), idx);
	
	return 0;
}

INT CRegNo_Creator::MakeMissingRegNoUpdate(CESL_DataMgr *pDM, CString regno, CStringList &regnoList)
{
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	CString regcode = regno.Left(2);
// 	INT regNo = _ttoi((regno.Right(10)).GetBuffer(0));
	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;

	CString regcode = regno.Left(nRegCodeLength);
	INT regNo = _ttoi((regno.Right(12 - nRegCodeLength )).GetBuffer(0));
	//===================================================================
	CString tmpquery, tmpkey, tmpstr;
	INT cnt = regnoList.GetCount();
	if(cnt == 0)
	{
		//++2008.10.16 UPDATE BY PWR {{++
		// ���սý��� ����
		tmpquery.Format( _T("DELETE FROM CO_DROP_ACCESSION_NO_TBL ")
						 _T("WHERE REG_CODE='%s' AND ACCESSION_REG_NO=%d AND MANAGE_CODE=UDF_MANAGE_CODE;")
						 , regcode, regNo );
//		tmpquery.Format(
//			_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE='%s' AND ACCESSION_REG_NO=%d;"),
//			regcode,
//			regNo
//		);
		//--2008.10.16 UPDATE BY PWR --}}
		pDM->AddFrame(tmpquery);
		return 0;
	}
	POSITION pos = regnoList.GetHeadPosition();
	for(INT i=0 ; i<cnt && pos != NULL ; i++)
	{
		pDM->MakeRecKey(tmpkey);
		tmpstr = regnoList.GetNext(pos);
		regNo = _ttoi(tmpstr.GetBuffer(0));
		//++2008.10.16 UPDATE BY PWR {{++
		// ���սý��� ����
		tmpquery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ")
						 _T("(REC_KEY, PUBLISH_FORM_CODE, REG_CODE, ACCESSION_REG_NO, MANAGE_CODE) ")
						 _T("VALUES(%s, 'MO', '%s', %d, UDF_MANAGE_CODE);")
						 , tmpkey, regcode, regNo );
//		tmpquery.Format(
//			_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, REG_CODE, ACCESSION_REG_NO) VALUES(%s, 'MO', '%s', %d);"),
//			tmpkey,
//			regcode,
//			regNo
//		);
		//--2008.10.16 UPDATE BY PWR --}}
		pDM->AddFrame(tmpquery);
	}
	return 0;
}

INT CRegNo_Creator::MakeLastRegNoUpdate(CESL_DataMgr *pDM, CString regno)
{
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	CString regcode = regno.Left(2);
// 	INT regNo = _ttoi((regno.Right(10)).GetBuffer(0));
	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;

	CString regcode = regno.Left(nRegCodeLength);
	INT regNo = _ttoi((regno.Right(12 - nRegCodeLength )).GetBuffer(0));
	//===================================================================
	
	CString tmpquery, tmpstr;
	//++2008.10.16 UPDATE BY PWR {{++
	// ���սý��� ����
	tmpquery.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					 _T("WHERE DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
					 , regcode );
//	tmpquery.Format(
//		_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE DIVISION_VALUE='%s'"),
//		regcode
//	);
	//--2008.10.16 UPDATE BY PWR --}}
	pDM->GetOneValueQuery(tmpquery, tmpstr);
	if(tmpstr.IsEmpty())
	{
		CString result;
		CTime t = CTime::GetCurrentTime();
		result.Format(_T("%04d"), t.GetYear());
		pDM->MakeRecKey(tmpstr);
		//++2008.10.16 UPDATE BY PWR {{++
		// ���սý��� ����
		tmpquery.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL ")
						 _T("(REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, ")
						 _T("LAST_NUMBER, MANAGE_CODE) ")
						 _T("VALUES(%s, 'MO', '��Ϲ�ȣ', '%s', '%s', %d, UDF_MANAGE_CODE);")
						 , tmpstr, regcode, result,	_ttoi((regno.Right(10).GetBuffer(0))) );
//		tmpquery.Format(
//			_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER) VALUES(%s, 'MO', '��Ϲ�ȣ', '%s', '%s', %d);"),
//			tmpstr,
//			regcode,
//			result,
//			_ttoi((regno.Right(10).GetBuffer(0)))
//		);
		//--2008.10.16 UPDATE BY PWR --}}
	}
	else
	{
		//++2008.10.16 UPDATE BY PWR {{++
		// ���սý��� ����
		tmpquery.Format( _T("UPDATE CO_LAST_NUMBER_TBL ")
						 _T("SET LAST_NUMBER=%d WHERE DIVISION_VALUE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;")
						 , regNo, regcode );
//		tmpquery.Format(
//			_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d WHERE DIVISION_VALUE='%s';"),
//			regNo,
//			regcode
//		);
		//--2008.10.16 UPDATE BY PWR --}}
	}
	
	pDM->AddFrame(tmpquery);
	return 0;
}

INT CRegNo_Creator::MakeQueryAndAddFrame(CESL_DataMgr *pDM, CString regno, INT idx, INT opcode)
{
	CString result, tmpstr, key, pkey, skey, dprice, acq_code, drate, price;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	pDM->GetCellData(_T("BB_åKEY"), idx, key);
	if(key.IsEmpty()) return -3000;
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	//===================================================================
	pDM->StartFrame();
	if(opcode == 1000)
		tmpstr.Format(
			_T("UPDATE BO_BOOK_TBL SET REG_CODE='%s', REG_NO='%s', REG_DATE='%s', WORKING_STATUS = '%s', DELIVERY_YN = '%s', LAST_WORK = '%s', CHECKIN_DATE='%s' WHERE REC_KEY = %s;"), 
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			regno.Left(2),
			regno.Left(nRegCodeLength),
			//===================================================================
			regno, 
			result,
			_T("BOA141N"),
			_T("Y"),
			m_pParentMgr->GetWorkLogMsg(),
			result,
			key
		);
	else if(opcode == -1000)
		tmpstr.Format(
			_T("UPDATE BO_BOOK_TBL SET REG_NO=NULL, REG_DATE=NULL, WORKING_STATUS='%s', DELIVERY_YN='N', LAST_WORK='%s', CHECKIN_DATE=NULL WHERE REC_KEY = %s;"), 
			_T("BOA113N"),
			m_pParentMgr->GetWorkLogMsg(),
			key
		);
	else if(opcode == 2000)
		tmpstr.Format(
			_T("UPDATE BO_BOOK_TBL SET REG_CODE='%s', REG_NO='%s', REG_DATE='%s', LAST_WORK = '%s' WHERE REC_KEY = %s;"), 
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			regno.Left(2),
			regno.Left(nRegCodeLength),
			//===================================================================
			regno, 
			result,
			m_pParentMgr->GetWorkLogMsg(),
			key
		);
	else if(opcode == -2000)
		tmpstr.Format(
			_T("UPDATE BO_BOOK_TBL SET REG_NO=NULL, REG_DATE=NULL, LAST_WORK='%s' WHERE REC_KEY = %s;"), 
			m_pParentMgr->GetWorkLogMsg(),
			key
		);

	pDM->AddFrame(tmpstr);
	pDM->GetCellData(_T("BB_���Ա���"), idx, acq_code);
	pDM->GetCellData(_T("BB_��������KEY"), idx, pkey);
	pDM->GetCellData(_T("BB_���ΰ�"), idx, dprice);
	pDM->GetCellData(_T("BP_������"), idx, drate);
	pDM->GetCellData(_T("BB_����"), idx, price);
	if(dprice.IsEmpty())
	{
		DOUBLE nprice = atof(price.GetBuffer(0));
		DOUBLE nrate = atof(drate.GetBuffer(0));
		DOUBLE ndprice = nprice - (nprice*(nrate/100));
		dprice.Format(_T("%10.2f"), ndprice);
	}
	if(pkey.IsEmpty()) return -3001;
	//���Ա��к��� �۾��� �ؾ��Ѵ�.
	if(opcode == 1000)
		tmpstr.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET CHECKIN_DATE='%s', DELIVERY_BOOK_CNT=DELIVERY_BOOK_CNT+%s, DELIVERY_PRICE=DELIVERY_PRICE+%s, LAST_WORK='%s' WHERE REC_KEY = %s;"),
			result,
			_T("1"),
			dprice,
			m_pParentMgr->GetWorkLogMsg(),
			pkey
		);
	else if(opcode == -1000)
		tmpstr.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET DELIVERY_BOOK_CNT=DELIVERY_BOOK_CNT-%s, DELIVERY_PRICE=DELIVERY_PRICE-%s, LAST_WORK='%s' WHERE REC_KEY = %s;"),
			_T("1"),
			dprice,
			m_pParentMgr->GetWorkLogMsg(),
			pkey
		);

	pDM->AddFrame(tmpstr);
	
	pDM->GetCellData(_T("IBS_��KEY"), idx, skey);
	if(skey.IsEmpty()) return -3002;
	CString strMarc;
	tmpstr.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY = %s"), skey);
	pDM->GetOneValueQuery(tmpstr, strMarc);
	CMarc marc;
	m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);	
	if(opcode == 1000)
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), regno);
	else if(opcode == -1000)
	{
		m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, _T("049$l"), regno);
		m_pParentMgr->m_pInfo->pMarcMgr->GetField(&marc, _T("049"), tmpstr);
		if(tmpstr.IsEmpty())
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
	}
	m_pParentMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	tmpstr.Format(_T("UPDATE BO_SPECIES_TBL SET MARC='%s' WHERE REC_KEY=%s;"),
		strMarc,
		skey
	);
	pDM->AddFrame(tmpstr);
	return 0;
}

BOOL CRegNo_Creator::IsDuplcateRegNo(CESL_DataMgr * pDM, CString regno)
{
	CString tmpquery, tmpstr;
	//++2008.10.16 UPDATE BY PWR {{++
	// ���սý��� ����
	tmpquery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE REG_NO='%s' ")
					_T("AND MANAGE_CODE=UDF_MANAGE_CODE"), regno );
	// tmpquery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE REG_NO = '%s'"), regno);
	//--2008.10.16 UPDATE BY PWR --}}
	pDM->GetOneValueQuery(tmpquery, tmpstr);
	if(tmpstr.GetLength() == 0)
		return FALSE;
	return TRUE;
}

INT CRegNo_Creator::IsValidRegNo(CString regno)
{
	CMakeSearchData checker;
	if(regno.GetLength() != 12) return -1000;
	
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	CString tmpstr = regno.Right(10);
	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	CString tmpstr = regno.Right(12- nRegCodeLength);
	//===================================================================
	if(!checker.CheckDigit(tmpstr)) return -1001;
	return 0;
}

INT CRegNo_Creator::CheckRegNoType(INT lastRegno, CString strRegno, CStringList &regnoList, INT opcode)
{
	if(opcode == -1000 || opcode == -2000) return 4000;
	CString regcode, tmpstr;
	INT regno, div;
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	regcode = strRegno.Left(2);
// 	tmpstr = strRegno.Right(10);
	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	regcode = strRegno.Left(nRegCodeLength);
	tmpstr = strRegno.Right(12- nRegCodeLength);
	//===================================================================
	
	regno = _ttoi(tmpstr.GetBuffer(0));
	div = regno - lastRegno;
	if(div < 0) return 1000;
	else if(div == 1) return 2000;
	for(INT i=1 ; i<div ; i++)
	{
		tmpstr.Format(_T("%010d"), lastRegno + i);
		regnoList.AddTail(tmpstr);
	}
	return 3000;
}
