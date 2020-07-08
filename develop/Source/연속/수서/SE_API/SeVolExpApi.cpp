// SeVolExpApi.cpp: implementation of the CSeVolExpApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SeVolExpApi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeVolExpApi::CSeVolExpApi(CESL_Mgr *pParentMgr /*=NULL*/)
{

	m_pMgr = pParentMgr;
	m_strACQ_KEY = _T("1"); //default
	m_strFORMAT_TYPE = _T("1"); //default
	m_nCount = 0;
	m_bIsPubDateVolTitle = FALSE;
}

CSeVolExpApi::~CSeVolExpApi()
{


}

INT CSeVolExpApi::InitVolExpMgr(CString strACQ_KEY, CString strFORMAT_TYPE, INT nACQ_TYPE, CArray<CString,CString> &arrayVolExpKeyList)
{
	if(strACQ_KEY.IsEmpty()) {
		AfxMessageBox(_T("���Ա���KEY��  ã�� �� �����ϴ�!"));
		return -1;
	}

 	if(!strFORMAT_TYPE.IsEmpty()) m_strFORMAT_TYPE = strFORMAT_TYPE;
	m_nACQ_TYPE = nACQ_TYPE;
	

	if(_T("1") == m_strFORMAT_TYPE)	m_pDM = m_pMgr->FindDM(VOL_EXP_DM_ALIAS);
	if(_T("2") == m_strFORMAT_TYPE)	m_pDM = m_pMgr->FindDM(SUPPORT_VOL_EXP_DM_ALIAS);

	if(!m_pDM) {
		AfxMessageBox(_T("��ȣ���� DM �� ã�� �� �����ϴ�!"));
		return -2;
	}
	
	CString strWhere;
	strWhere.Format(_T("ACQ_KEY = %s AND FORMAT_TYPE = %s"),strACQ_KEY,strFORMAT_TYPE);
	m_pDM->RefreshDataManager(strWhere);


	INT nRowCnt = m_pDM->GetRowCount();
	arrayVolExpKeyList.RemoveAll();
	m_arrayExpStartNo.RemoveAll();
	m_arrayExpLevel.RemoveAll();
	CString strtmp;
	CString strREC_KEY;


	for(INT i=0 ; i<nRowCnt ; i++)
	{
		
		m_pDM->GetCellData(_T("�������ۼ�"), i, strtmp);
		if(strtmp.GetLength() == 0) strtmp = _T("1");
		if(!CheckDigit(strtmp)) return -1000; //�߸� �Էµ� �������ۼ� 
		
		strREC_KEY = m_pDM->GetCellData(_T("��ȣ����KEY"),i);
		arrayVolExpKeyList.Add(strREC_KEY);
		m_arrayExpStartNo.Add(_ttoi(strtmp.GetBuffer(0)));

		m_pDM->GetCellData(_T("�ܰ���������"),i, strtmp);
		if(strtmp.GetLength() == 0) strtmp = _T("0");
		if(!CheckDigit(strtmp)) return -1000; //�߸� �Էµ� �ܰ���������
		m_arrayExpLevel.Add(_ttoi(strtmp.GetBuffer(0)));

		strtmp = _T("");
		m_pDM->GetCellData(_T("�����Ͽ�������"),i,strtmp);
		if(strtmp.IsEmpty())
			m_strPubPattern = _T("01");
		else
			m_strPubPattern = strtmp;
	}

	
	//������ ���� Ÿ��
	if(m_pDM->GetRowCount()>0)
	{
		CString strLevelName;
		strLevelName = m_pDM->GetCellData(_T("�ܰ��"),0);

		if(_T("<������>") == strLevelName) m_bIsPubDateVolTitle = TRUE;
	}
	

	return 0;
}


CString CSeVolExpApi::GetNextVolTitle(CString &strACQ_EXP_NO, CString strPUB_EXP_DATE)
{
	
	//������ ���� Ÿ��
	if(m_bIsPubDateVolTitle)
	{
		strACQ_EXP_NO = _T("");
		return ConvertDateForUser(strPUB_EXP_DATE);
	}
	
	CString strVolExpTitle = _T("");
	strACQ_EXP_NO = _T("");

	INT cnt = m_arrayExpStartNo.GetSize();
	if(cnt == 0) return _T("");
	
	MakeExpStartNoToString();

	INT inc_unit = -1;
	CString tmpstr;
	m_pDM->GetCellData(_T("��������"), cnt-1, tmpstr);
	if(!tmpstr.IsEmpty()) inc_unit = _ttoi(tmpstr.GetBuffer(0));
	SetExpStartNo(cnt - 1, inc_unit);
	
	INT nDisOrder = 1;
	CString strDisOrder = _T("");

	//ù��° �������� ����Ȱ��� ������ ������� ����
	CString strFirstDisOrder;
	m_pDM->GetCellData(_T("����"), 0, strFirstDisOrder);

	//2005-11-09 Ȳ���� �߰� : ������ �Է����� ��� ���� ���� �������� Ʋ������ ���� ����
	/*
	for(INT i=0 ; i<cnt ; i++)
	{
		tmpstr.Format(_T("%d"), m_arrayExpStartNo.GetAt(i));
		if(i != 0) tmpstr = _T(",")+tmpstr;
		strACQ_EXP_NO += tmpstr;
		m_pDM->GetCellData(_T("�ܰ��"), i, tmpstr);

		m_pDM->GetCellData(_T("����"), i, strDisOrder);
	*/

	INT nCol = 0;
	for(INT i=0 ; i<cnt ; i++)
	{
		if(nCol < cnt)
		{
			tmpstr.Format(_T("%d"), m_arrayExpStartNo.GetAt(nCol));
			if(nCol != 0) tmpstr = _T(",")+tmpstr;
			strACQ_EXP_NO += tmpstr;
			nCol++;
		}

		m_pDM->GetCellData(_T("�ܰ��"), i, tmpstr);
		m_pDM->GetCellData(_T("����"), i, strDisOrder);
		
		if(tmpstr == _T("<������>"))
		{
			tmpstr = ConvertDateForUser(strPUB_EXP_DATE);
		}
		else
		{
			if( tmpstr.Find(_T("?")) > -1 ) 
				tmpstr.Replace(_T("?"), m_arrayExpStartNoString.GetAt(i));
			else if( tmpstr.Find(_T("*")) > -1 )
				tmpstr.Replace(_T("*"), _T(""));
		}

		if ( strFirstDisOrder.IsEmpty() ) 
		{
			if( i != 0 ) tmpstr = _T(" ") + tmpstr;
				strVolExpTitle += tmpstr;
		}
		else
		{
			if ( _ttoi(strDisOrder) == nDisOrder )
			{
				if(nDisOrder != 1) tmpstr = _T(" ") + tmpstr;
				strVolExpTitle += tmpstr;
				i = -1;
				nDisOrder++;
			}
		}
	}


	return strVolExpTitle;
}

INT CSeVolExpApi::SetExpStartNo(INT idx, INT inc_unit)
{

	if(idx < 0) return 0;
	INT startno = m_arrayExpStartNo.GetAt(idx);
	INT nLevel  = m_arrayExpLevel.GetAt(idx);
	if(inc_unit < 0) startno++;
	else startno += inc_unit;
	
	CString tmpstr, strEnd, strLevel;
	m_pDM->GetCellData(_T("�ܰ�������"), idx, tmpstr);
	m_pDM->GetCellData(_T("����"), idx, strEnd);
	
	tmpstr.TrimLeft();			tmpstr.TrimRight();
	strEnd.TrimLeft();			strEnd.TrimRight();

	if(!CheckDigit(tmpstr)) return -1000;
	if(!CheckDigit(strEnd)) return -1000;

	INT limit = _ttoi(tmpstr.GetBuffer(0));
	INT last  = _ttoi(strEnd.GetBuffer(0));
	
	if(tmpstr.GetLength() == 0) 
	{
		
		//------------------------------------------------------------
		//�ܰ��������� ���� ��쿡�� ������ ���ؼ� ������ MAX�� �Ѵ�
		CString strEndNo; 
		INT endno = 0;
		m_pDM->GetCellData(_T("����"), idx, strEndNo);
		if(!strEndNo.IsEmpty()) endno = _ttoi(strEndNo.GetBuffer(0));
		
		if(endno>0)
		{
			if(startno > endno)
			{
				m_pDM->GetCellData(_T("���ۼ�"), idx, tmpstr);
				if(!CheckDigit(tmpstr)) return -1000;
				startno = _ttoi(tmpstr.GetBuffer(0)); 
			}
		}
		//-------------------------------------------------------------

		m_arrayExpStartNo.SetAt(idx, startno);
		return SetExpStartNo(idx-1, inc_unit);
	}
	
	m_pDM->GetCellData(_T("��������"), idx-1, tmpstr);

	if(tmpstr.GetLength() == 0) tmpstr = _T("1");
	if(!CheckDigit(tmpstr)) return -1000;
	inc_unit = _ttoi(tmpstr.GetBuffer(0));
	
	//2006-01-18 Ȳ���� ����
/*	if(startno > limit)
	{
		m_pDM->GetCellData(_T("���ۼ�"), idx, tmpstr);
		if(!CheckDigit(tmpstr)) return -1000;
		startno = _ttoi(tmpstr.GetBuffer(0)); 
		m_arrayExpStartNo.SetAt(idx, startno); //recursive ������ setting

		return SetExpStartNo(idx-1, inc_unit);
	}
	
	m_arrayExpStartNo.SetAt(idx, startno);

	return 0;
*/
	if( startno > last )
	{
		if( limit == last )
		{
			m_pDM->GetCellData(_T("���ۼ�"), idx, tmpstr);
			if(!CheckDigit(tmpstr)) return -1000;
			startno = _ttoi(tmpstr.GetBuffer(0)); 
			m_arrayExpStartNo.SetAt(idx, startno); //recursive ������ setting
			
			if( nLevel != 0 )
			{
				if( nLevel == limit )
					m_arrayExpLevel.SetAt(idx, 1);
				else
					m_arrayExpLevel.SetAt(idx, nLevel+1);
			}

			return SetExpStartNo(idx-1, inc_unit);
		}
	}
	
	INT nExpStartNo = m_arrayExpStartNo.GetAt(idx);
	if(  nLevel != 0 && nExpStartNo != 0 )
	{
		if( nLevel == limit )
		{
			m_arrayExpLevel.SetAt(idx, 1);
			m_arrayExpStartNo.SetAt(idx, startno);
					
			return SetExpStartNo(idx-1, inc_unit);
		}
		
		m_arrayExpLevel.SetAt(idx, nLevel+1);
	}
	

	m_arrayExpStartNo.SetAt(idx, startno);

	return 0;
}

VOID CSeVolExpApi::MakeExpStartNoToString()
{

	CString strsign, strtarget;
	INT cnt = m_arrayExpStartNo.GetSize();
	m_arrayExpStartNoString.RemoveAll();
	
	for(INT i=0 ; i<cnt ; i++)
	{
		m_pDM->GetCellData(_T("�ܰ�ĺ���"), i, strsign);
		MakeIntegerToString(strsign, strtarget, m_arrayExpStartNo.GetAt(i));
		m_arrayExpStartNoString.Add(strtarget);

	}


}

VOID CSeVolExpApi::MakeIntegerToString(CString strsign, CString &strtarget, INT src)
{


	CString desc;
	if(strsign == _T("����"))
		strtarget.Format(_T("%d"), src);
	else if(strsign == _T("�θ���(��)"))
		ConvertToNumberToRomanString(src, TRUE, strtarget);
	else if(strsign == _T("�θ���(��)"))
		ConvertToNumberToRomanString(src, FALSE, strtarget);
	else
	{
		INT cnt = MakeReplaceStringArray(strsign);
		if(src-1 < cnt) 
			strtarget = m_arraySignString.GetAt(src-1);
	}


}

VOID CSeVolExpApi::ConvertToNumberToRomanString(INT src, BOOL isUpper, CString &targetstr)
{


	//modified by loveisverb
	//2004-10-18
	//�θ��� ��ȯ ��� ��ȯ

	CString units[] = {_T(""),_T("I"),_T("II"),_T("III"),_T("IV"),_T("V"),_T("VI"),_T("VII"),_T("VIII"),_T("IX")};
	CString tens[] = {_T(""),_T("X"),_T("XX"),_T("XXX"),_T("XL"),_T("L"),_T("LX"),_T("LXX"),_T("LXXX"),_T("XC")};
	CString hundreds[] = {_T(""),_T("C"),_T("CC"),_T("CCC"),_T("CD"),_T("D"),_T("DC"),_T("DCC"),_T("DCCC"),_T("CM")};
	CString thousands[] = {_T(""),_T("M"),_T("MM"),_T("MMM"),_T("MV"),_T("V"),_T("VM"),_T("VMM"),_T("VMMM"),_T("MX")};

	CString billionsdashes[] = {_T(""),_T("="),_T("=="),_T("==="),_T("=��"),_T("��"),_T("��="),_T("��=="),_T("��==="),_T("=��")};
	CString hundredmillionsdashes[] = {_T(""),_T("="),_T("=="),_T("==="),_T("=="),_T("="),_T("=="),_T("==="),_T("===="),_T("==")};
	CString tenmillionsdashes[] = {_T(""),_T("_"),_T("__"),_T("___"),_T("_="),_T("="),_T("=_"),_T("=__"),_T("=___"),_T("_=")};
	CString millionsdashes[] = {_T(""),_T("_"),_T("__"),_T("___"),_T("_="),_T("="),_T("=_"),_T("=__"),_T("=___"),_T("_=")};
	CString hundredthousandsdashes[] = {_T(""),_T("_"),_T("__"),_T("___"),_T("__"),_T("_"),_T("__"),_T("___"),_T("____"),_T("__")};
	CString tenthousandsdashes[] = {_T(""),_T("_"),_T("__"),_T("___"),_T("__"),_T("_"),_T("__"),_T("___"),_T("____"),_T("__")};
	CString thousandsdashes[] = {_T(""),_T(""),_T(""),_T(""),_T(" _"),_T("_"),_T("_"),_T("_"),_T("_"),_T(" _")};

	CString roman;
	CString romandashes;

	CString strArabic;
	strArabic.Format(_T("%d"),src);
	targetstr.Empty();
	
	INT nLength = strArabic.GetLength();
	INT nNumber = 0;
	TCHAR szNumber;
	CString strNumber;


	for(INT i=0;i<nLength;i++)
	{
		szNumber = strArabic.GetAt(i);
		strNumber.Format(_T("%c"),szNumber);

		nNumber = _ttoi(strNumber.GetBuffer(0));

		switch(nLength-i)
		{
			case 10:
				roman += thousands[nNumber];
				romandashes += billionsdashes[nNumber];
				break;
			case 9:
				roman += hundreds[nNumber];
				romandashes += hundredmillionsdashes[nNumber];
				break;
			case 8:
				roman += tens[nNumber];
				romandashes += tenmillionsdashes[nNumber];
				break;
			case 7:
				roman += thousands[nNumber];
				romandashes += millionsdashes[nNumber];
				break;
			case 6:
				roman += hundreds[nNumber];
				romandashes += hundredthousandsdashes[nNumber];
				break;		
			case 5:
				roman += tens[nNumber];
				romandashes += tenthousandsdashes[nNumber];
				break;		
			case 4:
				roman += thousands[nNumber];
				romandashes += thousandsdashes[nNumber];
				break;
			case 3:
				roman += hundreds[nNumber];
				break;
			case 2:
				roman += tens[nNumber];
				break;
			case 1:
				roman += units[nNumber];
				break;
			default :
				break;
		}


	}
	
	targetstr = romandashes + _T(" ") + roman;
	targetstr.TrimLeft();
	targetstr.TrimRight();

	if(!isUpper) targetstr.MakeLower();
		
	
	/*
	unsigned short code;
	if(isUpper) code = 0xa5af;
	else code = 0xa5a0;
	INT multiples, remain;
	multiples = src/10;
	remain = src%10;
	CString tmpstr;
	targetstr = _T("");
	
	for(INT i=0 ; i<multiples ; i++)
	{
		SwapHighLowByte(tmpstr, code+10);
		targetstr += tmpstr;
	}
	tmpstr = _T("");
	if(remain != 0)
		SwapHighLowByte(tmpstr, code + remain);
		
	targetstr += tmpstr;
	*/

}

INT CSeVolExpApi::MakeReplaceStringArray(CString desc)
{

	m_arraySignString.RemoveAll();

	if (desc.GetLength() == 0) return 0;
	CString element, sztemp;
	for (INT i = 0; true; i++) {
		element.Empty();
		element = desc.SpanExcluding(_T(","));
		element.TrimLeft();			element.TrimRight();
	
		if (element.GetLength() > 0) {
			m_arraySignString.Add(element);
		}
		if (element == desc) break;
		sztemp = desc.Mid(element.GetLength()+1);
		if (sztemp.GetLength() == 0) break;
		desc = sztemp;
	}

	return m_arraySignString.GetSize();	
}

VOID CSeVolExpApi::SwapHighLowByte(CString &tmpstr, unsigned short code)
{

	USHORT low, high;
	low = 0x00FF & code;
	low <<= 8;
	high = 0xFF00 & code;
	high >>= 8;

	USHORT tmpcode;
	tmpcode = low | high;
	
	TCHAR strTemp[3];
	strTemp[2] = 0;

	*(USHORT *)strTemp = tmpcode;

	tmpstr.Format(_T("%s"), strTemp);

}


BOOL CSeVolExpApi::CheckDigit(CString str)
{
	if(str.IsEmpty()) return TRUE;

	TCHAR * tmp = str.GetBuffer(0);
	INT size = str.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;

}

CString CSeVolExpApi::ConvertDateForUser(CString strDate)
{
	CString strUserDate = _T("");
	CString strTmp		= _T("");
	if(strDate.IsEmpty()) return strUserDate;
	
	INT year,month,day;
	year = month = day = -1;
	ExtractStringDateToIntDate(strDate, &year, &month, &day);
	if(m_strPubPattern.IsEmpty() == FALSE)
	{
		if( m_strPubPattern == _T("01") )
			strUserDate.Format(_T("%d�� %d�� %d��"), year,month,day);
		else if( m_strPubPattern == _T("02") )
		{
			strTmp.Format(_T("%d"),year);
			strUserDate.Format(_T("%sҴ %d�� %d��"), strTmp.Mid(2) ,month,day);
		}
		else if ( m_strPubPattern ==  _T("03") ) 
			strUserDate.Format(_T("%d�� %d��"), year,month);
		else if ( m_strPubPattern ==  _T("04") )
			strUserDate.Format(_T("%d��%d��"), month,day);
		else if ( m_strPubPattern ==  _T("05") ) 
		{
			strTmp.Format(_T("%d"),year);
			strUserDate.Format(_T("%s-%d-%d"), strTmp.Mid(2),month,day);
		}
		else if ( m_strPubPattern ==  _T("06") )
		{
			strTmp.Format(_T("%d"),year);
			strUserDate.Format(_T("%s/%d/%d"), strTmp.Mid(2),month,day);
		}
		else if ( m_strPubPattern ==  _T("07") )
		{
			strTmp.Format(_T("%d"),year);
			strUserDate.Format(_T("%s-%d-%d"), strTmp.Mid(2),month,day);
		}
		else if ( m_strPubPattern ==  _T("08") ) 
			strUserDate.Format(_T("%d/%d/%d"), year,month,day);
		else if ( m_strPubPattern ==  _T("09") )
			strUserDate.Format(_T("%d/%d"), month,day);
		else if ( m_strPubPattern ==  _T("10") )
			strUserDate.Format(_T("%d/%d/%d"), month, day, year);
		else if ( m_strPubPattern ==  _T("11") )
			strUserDate.Format(_T("%d-%d-%d"), month, day, year);
		else if ( m_strPubPattern ==  _T("12") )
			strUserDate.Format(_T("%d. %d. %d"), year,month,day);
		else if ( m_strPubPattern ==  _T("13") )
			strUserDate.Format(_T("%d. %d, %2d"), month,day,year);
		else if ( m_strPubPattern ==  _T("14") )
			strUserDate.Format(_T("%d. %d"), year,month);
		else if ( m_strPubPattern ==  _T("15") )
			strUserDate.Format(_T("%d. %d"), month,year);
		else if ( m_strPubPattern ==  _T("16") )
		{
			strTmp = GetMonthEng(1, month);
			strUserDate.Format(_T("%s %d %d"), strTmp, day, year);
		}
		else if ( m_strPubPattern ==  _T("17") )
		{
			strTmp = GetMonthEng(2, month);
			strUserDate.Format(_T("%s %d %d"), strTmp, day, year);
		}
		else if ( m_strPubPattern ==  _T("18") )
		{
			strTmp = GetMonthEng(2, month);
			strUserDate.Format(_T("%s %d"), strTmp, day);
		}
		else if ( m_strPubPattern ==  _T("19") )
		{
			strTmp = GetMonthEng(2, month);
			strUserDate.Format(_T("%s-%d"), strTmp, day);
		}
		else if ( m_strPubPattern ==  _T("20") )
		{
			strTmp = GetMonthEng(1, month);
			strUserDate.Format(_T("%s %d"), strTmp, year);
		}
		else if( m_strPubPattern == _T("21") ) 
		{
			strTmp = GetMonthEng(2, month);
			strUserDate.Format(_T("%s %d"), strTmp, year);
		}
		else if( m_strPubPattern == _T("22") ) 
		{
			strTmp = GetMonthEng(1, month);
			strUserDate.Format(_T("%d %s %d"), year, strTmp, day);
		}
		else if( m_strPubPattern == _T("23") ) 
		{
			strTmp = GetMonthEng(2, month);
			strUserDate.Format(_T("%d %s %d"), year, strTmp, day);
		}
		else if( m_strPubPattern == _T("24") )
		{
			CTime t(year, month, day, 0,0,0);
			CTime result = t + CTimeSpan(6,0,0,0);
			CString sMonth, sDay;
			sMonth.Format(_T("%2d"),result.GetMonth());
			sDay.Format(_T("%2d"),result.GetDay());
			strUserDate.Format(_T("%d.%d.%d~%s.%s"), year, month, day, sMonth, sDay);
		}		
		else
			return strUserDate;
	}
	//strUserDate.Format(_T("%4d�� %2d�� %2d��"), year,month,day);

	return strUserDate;
/*	CString strUserDate = _T("");
	if(strDate.IsEmpty()) return strUserDate;
	
	INT year,month,day;
	year = month = day = -1;
	ExtractStringDateToIntDate(strDate, &year, &month, &day);
	
	strUserDate.Format(_T("%4d�� %2d�� %2d��"), year,month,day);

	return strUserDate;
*/
}


VOID CSeVolExpApi::ExtractStringDateToIntDate(CString lastExpDate, INT *year, INT *month, INT *day)
{
	if(lastExpDate.GetLength() == 0) return ;
	INT idx, startidx, lastidx;
	idx = startidx = 0;
	lastidx = lastExpDate.GetLength();
	idx = lastExpDate.Find(_T("/"), startidx);
	if(idx > 0)
		*year = _ttoi((lastExpDate.Left(idx)).GetBuffer(0));
	startidx = idx + 1;
	idx = lastExpDate.Find(_T("/"), startidx);
	if(idx > 0)
		*month = _ttoi((lastExpDate.Mid(startidx, idx - 1)).GetBuffer(0));
	startidx = idx + 1;
	*day = _ttoi((lastExpDate.Right(lastidx - startidx)).GetBuffer(0));
	return ;
}

INT CSeVolExpApi::ConvertVolExpLogic(CESL_DataMgr* pDM)
{
	//���� KOLAS2�� KOLAS2UP���� ������ ��ȯ�� 
	//�������ۼ��� ��LOGIC�� �µ��� �ϴ� �Լ�   
	
	m_arrayExpStartNoConvert.RemoveAll();

	INT nRowCnt = pDM->GetRowCount();
	
	CString strTmp;
	strTmp = pDM->GetCellData(_T("�ܰ��"), 0);
	strTmp.TrimLeft();
	strTmp.TrimRight();
	
	//������ ������ ��� ��ȯ�� �ʿ� ����
	if(strTmp == _T("<������>")) return 0;


	for(INT i=0;i<nRowCnt;i++)
	{
		strTmp = _T("");
		strTmp = m_pDM->GetCellData(_T("�������ۼ�"), i);
		if(strTmp.GetLength() == 0) strTmp = _T("1");
		if(!CheckDigit(strTmp)) return -1000; //�߸� �Էµ� �������ۼ� 
		
		m_arrayExpStartNoConvert.Add(_ttoi(strTmp.GetBuffer(0)));
	}
	
	INT cnt = m_arrayExpStartNoConvert.GetSize();
	
	//��ȯ�� ������ ����
	if(cnt == 0) return 0;
	
	INT inc_unit = -1;
	strTmp = _T("");
	strTmp = m_pDM->GetCellData(_T("��������"), cnt - 1);
	if(!strTmp.IsEmpty()) inc_unit = _ttoi(strTmp.GetBuffer(0));

	SetExpStartNoConvert(cnt - 1, inc_unit);
	

	INT nExpStartNo = 0;
	
	for(i=0;i<nRowCnt;i++)
	{
		strTmp = _T("");
		nExpStartNo = m_arrayExpStartNoConvert.GetAt(i);
		strTmp.Format(_T("%d"),nExpStartNo);
		pDM->SetCellData(_T("�������ۼ�"),strTmp,i);
	}


	return 0;
}


INT CSeVolExpApi::SetExpStartNoConvert(INT idx, INT inc_unit)
{

	if(idx < 0) return 0;
	INT startno = m_arrayExpStartNoConvert.GetAt(idx);
	
	if(inc_unit < 0) startno++;
	else startno += inc_unit;
	
	CString strTmp;

	strTmp = m_pDM->GetCellData(_T("�ܰ�������"), idx);
	strTmp.TrimLeft();			
	strTmp.TrimRight();
	if(!CheckDigit(strTmp)) return -1000;

	if(strTmp.GetLength() == 0) 
	{
		m_arrayExpStartNoConvert.SetAt(idx, startno);
		return SetExpStartNoConvert(idx-1, inc_unit);
	}

	INT limit = _ttoi(strTmp.GetBuffer(0));
	
	if(startno > limit)
	{
		strTmp = m_pDM->GetCellData(_T("���ۼ�"), idx);
		if(!CheckDigit(strTmp)) return -1000;
		startno = _ttoi(strTmp.GetBuffer(0)); 
		m_arrayExpStartNoConvert.SetAt(idx, startno); //recursive ������ setting

		strTmp = m_pDM->GetCellData(_T("��������"), idx-1);
		strTmp.TrimLeft();
		strTmp.TrimRight();
		if(strTmp.GetLength() == 0) strTmp = _T("1");
		if(!CheckDigit(strTmp)) return -1000;
		inc_unit = _ttoi(strTmp.GetBuffer(0));

		return SetExpStartNoConvert(idx-1, inc_unit);
	}
	
	m_arrayExpStartNoConvert.SetAt(idx, startno);

	return 0;

}

CString CSeVolExpApi::GetMonthEng(INT mode, INT month)
{
	if(month < 1)
		return _T("?");

	CString sResult;
	sResult = _T("");
	
	if(mode == 1)// ���� �ϼ�
	{
		switch(month)
		{
		case 1:
			sResult = _T("January");
			break;
		case 2:
			sResult = _T("February");
			break;
		case 3:
			sResult = _T("March");
			break;
		case 4:
			sResult = _T("April");
			break;
		case 5:
			sResult = _T("May");
			break;
		case 6:
			sResult = _T("June");
			break;
		case 7:
			sResult = _T("July");
			break;
		case 8:
			sResult = _T("August");
			break;
		case 9:
			sResult = _T("September");
			break;
		case 10:
			sResult = _T("October");
			break;
		case 11:
			sResult = _T("November");
			break;
		case 12:
			sResult = _T("December");
			break;
		default:
			break;
		}
	}
	else// ���� ��ü
	{
		switch(month)
		{
		case 1:
			sResult = _T("Jan");
			break;
		case 2:
			sResult = _T("Feb");
			break;
		case 3:
			sResult = _T("Mar");
			break;
		case 4:
			sResult = _T("Apr");
			break;
		case 5:
			sResult = _T("May");
			break;
		case 6:
			sResult = _T("Jun");
			break;
		case 7:
			sResult = _T("Jul");
			break;
		case 8:
			sResult = _T("Aug");
			break;
		case 9:
			sResult = _T("Sep");
			break;
		case 10:
			sResult = _T("Oct");
			break;
		case 11:
			sResult = _T("Nov");
			break;
		case 12:
			sResult = _T("Dec");
			break;
		default:
			break;
		}

	}
	return sResult;
}