// SeExpectMgr.cpp: implementation of the CSeExpectMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SeExpectMgr.h"
#include "SeDateExpApi.h"
#include "SeAppendixExpApi.h"
#include "SeVolExpApi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeExpectMgr::CSeExpectMgr(CESL_Mgr *pParentMgr)
{
	m_pMgr = pParentMgr;
	m_pDM = NULL;

	m_strACQ_KEY = _T("");
	m_strPUB_EXP_KEY = _T("");
	m_strACQ_EXP_KEY = _T("");
	m_strPUB_EXP_TYPE = _T("");
	m_strACQ_EXP_TYPE = _T("");

	m_nACQ_TYPE = 1; //default

}

CSeExpectMgr::~CSeExpectMgr()
{
	
}


INT CSeExpectMgr::InitMgr(CESL_DataMgr* pDM_EXP, 
						  CESL_DataMgr* pDM_ACQ, 
						  CString strPUB_FREQ, 
						  INT nACQ_TYPE,
						  CArray<CString,CString> &arrayVolKeyList,
						  CArray<CString,CString> &arrayOtherVolKeyList,
						  INT nMoreExp,
						  CProgressCtrl* pProgress)
{
	
	if(!pDM_ACQ || !pDM_EXP) {
		AfxMessageBox(_T("���� DM �� ã�� �� �����ϴ�"));
		return -1;
	}

	m_pDM = pDM_EXP; 
	m_nACQ_TYPE = nACQ_TYPE;
	CString strEND_DATE = _T("");

	if(1 == nACQ_TYPE)
	{
		m_strACQ_KEY = pDM_ACQ->GetCellData(_T("SP_���԰���KEY"),0);
		m_strPUB_EXP_KEY = pDM_ACQ->GetCellData(_T("SP_���࿹������KEY"),0); 
		m_strACQ_EXP_KEY = pDM_ACQ->GetCellData(_T("SP_�Լ���������KEY"),0);
		m_strPUB_EXP_TYPE = pDM_ACQ->GetCellData(_T("SP_���࿹������"),0);
		m_strACQ_EXP_TYPE = pDM_ACQ->GetCellData(_T("SP_�Լ���������"),0);
		strEND_DATE = pDM_ACQ->GetCellData(_T("SP_����������"),0);
	}
	else
	{
		m_strACQ_KEY = pDM_ACQ->GetCellData(_T("SD_��������KEY"),0);
		m_strPUB_EXP_KEY = pDM_ACQ->GetCellData(_T("SD_���࿹������KEY"),0); 
		m_strACQ_EXP_KEY = pDM_ACQ->GetCellData(_T("SD_�Լ���������KEY"),0);
		m_strPUB_EXP_TYPE = pDM_ACQ->GetCellData(_T("SD_���࿹������"),0);
		m_strACQ_EXP_TYPE = pDM_ACQ->GetCellData(_T("SD_�Լ���������"),0);
	}
	

	if(m_strPUB_EXP_TYPE.IsEmpty() || m_strACQ_EXP_TYPE.IsEmpty())
	{	
		AfxMessageBox(_T("����/�Լ��� ���� ������ ��� ���������� ������ �� �����ϴ�!"));
		return -2;
	}
	
	//������ 
	CSeDateExpApi PubExpDateMgr(m_pMgr);
	PubExpDateMgr.m_bIsPubExpType = TRUE;
	
	//�Լ��� 
	CSeDateExpApi AcqExpDateMgr(m_pMgr);
	
	//�η� 
	CSeAppendixExpApi AppendixExpMgr(m_pMgr);
	
	//��ȣ 
	CSeVolExpApi VolExpMgr(m_pMgr);
	
	//��������ȣ 
	CSeVolExpApi SupportVolExpMgr(m_pMgr);


	//�Լ����� �������� �Ѵ� 
	if(pProgress) pProgress->SetPos(0);
	INT ids = AcqExpDateMgr.InitDateExpMgr(strPUB_FREQ,m_strACQ_EXP_KEY,m_strACQ_EXP_TYPE,nMoreExp);
	if(-1001 == ids) {
		return -3; //�������� ���� 
	}
	
	INT nVolCnt = AcqExpDateMgr.m_arrayExpDate.GetSize()-1; //������ �ϳ��� ���� ������ ���� �Լ� ���̴� 
	if(nVolCnt<0)
	{
		return -3; //�������� ���� 
	}
	
	//�������� �Լ��� ���� ����� �����
	if(pProgress) pProgress->SetPos(30);
	PubExpDateMgr.InitDateExpMgr(strPUB_FREQ,m_strPUB_EXP_KEY,m_strPUB_EXP_TYPE,nVolCnt+1);
	

	INT nAcqExpSize = AcqExpDateMgr.m_arrayExpDate.GetSize();
	INT nPubExpSize = PubExpDateMgr.m_arrayExpDate.GetSize();
		
	if(nAcqExpSize != nPubExpSize)
	{
		AfxMessageBox(_T("�Լ��ϰ� ������ ������ �ұ������Դϴ�!"));
		return -4;
	}

	if(pProgress) pProgress->SetPos(60);
	VolExpMgr.InitVolExpMgr(m_strACQ_KEY,_T("1"),nACQ_TYPE,arrayVolKeyList);
	
	if(pProgress) pProgress->SetPos(70);
	SupportVolExpMgr.InitVolExpMgr(m_strACQ_KEY,_T("2"),nACQ_TYPE,arrayOtherVolKeyList);
	
	AppendixExpMgr.InitAppendixExpMgr(m_strACQ_KEY,nACQ_TYPE);


	CString strACQ_EXP_NO_for_VOL,strACQ_EXP_NO_for_SUPPORT_VOL,strVolTitle,strSupportVolTitle;
	CString strAcqExpDate,strPubExpDate;
	CString strNextAcqExpDate,strNextPubExpDate;
	CString strLastAcqExpCount,strLastPubExpCount;
	
	
	if(pProgress) pProgress->SetPos(80);
	
	
	m_arrayVolTileAcqExpNo.RemoveAll();
	m_arrayOtherVolTileAcqExpNo.RemoveAll();
	

	CString strLastAcqExpDate;
	CString strLastPubExpData;
		

	if(nVolCnt>0)
	{
		strLastAcqExpDate = AcqExpDateMgr.m_arrayExpDate.GetAt(nVolCnt);
		strLastPubExpData = PubExpDateMgr.m_arrayExpDate.GetAt(nVolCnt);
	}
	
	CString strUDF;
	
	
	for(INT i=0;i<nVolCnt;i++)
	{
		m_pDM->InsertRow(-1);
	}
	

	INT nNonExpData = 0;
	CString msg;


	for(i=0;i<nVolCnt;i++)
	{
		
		//�Լ��� 
		strAcqExpDate = _T("");
		
		strAcqExpDate = AcqExpDateMgr.m_arrayExpDate.GetAt(i);

		//�����ϰ�� �Լ������ϰ� ������������ ���� �� ���İ� �̸� ������ ���� �ʴ´�
		if(1 == nACQ_TYPE && !strEND_DATE.IsEmpty()) 
		{
			if(CompareStringDates(strEND_DATE,strAcqExpDate)<0) 
			{
				for(INT j=nVolCnt-1-i;j>=0;j--)
				{
					m_pDM->DeleteRow(j);
					nNonExpData++;
				}
				
				// 17/08/21 ������ : ���Ӱ��๰�� �⺻�߰������� ����Ҷ� �߰������ڷᰡ ��������Ⱓ�� ������ �����Ⱓ���� �޽��� ������� �ʵ��� ��ɰ���
/*//BEFORE-----------------------------------------------------------------------------
				if(nNonExpData>0)
*///AFTER------------------------------------------------------------------------------
				if( 0 < nNonExpData && 0 >= nMoreExp )
/*//END-------------------------------------------------------------------------------*/
				{
					//msg.Format(_T("[ %d ] ���� �ڷᰡ ���������� ���Ŀ� �����Ǿ����ϴ�!"),nNonExpData);
					msg.Format(_T("���� ���� �Ǿ����ϴ�!"));
					AfxMessageBox(msg);
				}

				if(pProgress) pProgress->SetPos(90);
				return 0;
			}
		}


		m_pDM->SetCellData(_T("SA_�Լ�������"),strAcqExpDate,nVolCnt-1-i);
		
		strNextAcqExpDate = _T("");
		strNextAcqExpDate = AcqExpDateMgr.m_arrayExpDate.GetAt(i+1);
		m_pDM->SetCellData(_T("UDF_�����Լ�������"),strNextAcqExpDate,nVolCnt-1-i);
		
		
		strLastAcqExpCount = _T("");
		strLastAcqExpCount = AcqExpDateMgr.m_arrayLastExpCount.GetAt(i);
		m_pDM->SetCellData(_T("UDF_�Լ������Ȱ���"),strLastAcqExpCount,nVolCnt-1-i);
	

		//������
		strPubExpDate = _T("");
		strPubExpDate = PubExpDateMgr.m_arrayExpDate.GetAt(i);
		m_pDM->SetCellData(_T("SV_������"),strPubExpDate,nVolCnt-1-i);
		
		strNextPubExpDate = _T("");
		strNextPubExpDate = PubExpDateMgr.m_arrayExpDate.GetAt(i+1);
		m_pDM->SetCellData(_T("UDF_�������࿹����"),strNextPubExpDate,nVolCnt-1-i);
		
		
		strLastPubExpCount = _T("");
		strLastPubExpCount = PubExpDateMgr.m_arrayLastExpCount.GetAt(i);
		m_pDM->SetCellData(_T("UDF_���࿹���Ȱ���"),strLastPubExpCount,nVolCnt-1-i);

	
		//��ȣ�� 
		strVolTitle = VolExpMgr.GetNextVolTitle(strACQ_EXP_NO_for_VOL,strPubExpDate);
		if(!strVolTitle.IsEmpty())
			m_pDM->SetCellData(_T("SA_�Լ�������ȣ��"),strVolTitle,nVolCnt-1-i);
		
		m_pDM->SetCellData(_T("UDF_��ȣ�������ۼ�"),strACQ_EXP_NO_for_VOL,nVolCnt-1-i);
	
		
		//��������ȣ��
		strSupportVolTitle = SupportVolExpMgr.GetNextVolTitle(strACQ_EXP_NO_for_SUPPORT_VOL,strPubExpDate);
		if(!strSupportVolTitle.IsEmpty())
			m_pDM->SetCellData(_T("SV_��������ȣ��"),strSupportVolTitle,nVolCnt-1-i);
		
		m_pDM->SetCellData(_T("UDF_��������ȣ�������ۼ�"),strACQ_EXP_NO_for_SUPPORT_VOL,nVolCnt-1-i);
	}


	if(pProgress) pProgress->SetPos(90);


	return 0;
}

INT CSeExpectMgr::CompareStringDates(CString date1, CString date2)
{
	if(date1.GetLength() == 0 || date2.GetLength() == 0) return 0;
	INT year, month, day;
	ExtractStringDateToIntDate(date1, &year, &month, &day);
	COleDateTime t1(year, month, day, 0, 0, 0);
	
	ExtractStringDateToIntDate(date2, &year, &month, &day);
	COleDateTime t2(year, month, day, 0, 0, 0);

	if(t1 > t2) return 1;
	else if(t1 == t2) return 0;
	return -1;

}

VOID CSeExpectMgr::ExtractStringDateToIntDate(CString lastExpDate, INT *year, INT *month, INT *day)
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