// ChangeValue.cpp: implementation of the CChangeValue class.
// 2008.07.28 MAKE BY PWR
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChangeValue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChangeValue::CChangeValue()
{

}

CChangeValue::~CChangeValue()
{

}

// ** �ش���(sAbility)�� �������� �ڵ带 �������� �����Ѵ�.
CString CChangeValue::CodeToDesc(CString sAbility, CString sCode)
{
	CString sDesc;
	// 02Dlg (�ݳ�����&����ȯ��)
	if( sAbility == _T("�ݳ������뿩��")			 || sAbility == _T("���ϴ����ڷ� �ݳ��������")	||
		sAbility == _T("�ݳ������� �ڵ��ο�")		 || sAbility == _T("�����ڵ����")				||
		sAbility == _T("���ุ���� �ڵ��ο�")		 || sAbility == _T("���ุ���Ϻο��� SMS�߼ۿ���") ||
		sAbility == _T("���ุ���� �ް��� ��������") || sAbility == _T("������� �ȳ����� �߼ۿ���") || 
		sAbility == _T("�켱���⿹���� ��뿩��")	 || sAbility == _T("���ټ��⿹����") )
	{
		if( sCode == _T("Y") )
			sDesc = _T("�����");
		else if( sCode == _T("N") ) 
			sDesc = _T("������");
		else
			sDesc = _T("������");
	}
	else if( sAbility == _T("�ݳ������ ����켱���� ����") )
	{
		if( sCode == _T("0") )
			sDesc = _T("�ݳ�����켱");
		else if( sCode == _T("1") ) 
			sDesc = _T("����켱");
		else
			sDesc = _T("�ݳ�����켱");
	}
	else if( sAbility == _T("�ݳ������� ���� ������") )
	{
		if( sCode == _T("0") )
			sDesc = _T("�ݳ�������");
		else if( sCode == _T("1") ) 
			sDesc = _T("������");
		else
			sDesc = _T("�ݳ�������");
	}
	else if( sAbility == _T("������ ��뿩��") )
	{
		if( sCode == _T("0") )
			sDesc = _T("������");
		else if( sCode == _T("1") )
			sDesc = _T("�������� �ڷḸ ���డ��");
		else if( sCode == _T("2") )
			sDesc = _T("����ڷ� ���డ��");
		else
			sDesc = _T("������");
	}
	else if( sAbility == _T("�ݳ����� ����Ƚ��") )// EditBox
	{
		sDesc = sCode + _T("ȸ");
	}
	else if( sAbility == _T("��������ο�") )
	{
		sDesc = sCode + _T("��");
	}
	//08Dlg (����������ȣ��å)
	else if( sAbility == _T("�ֹε�Ϲ�ȣ ǥ�ÿ���") || sAbility == _T("�����ڹ�ȣ ǥ�ÿ���") )
	{
		if( sCode == _T("Y") )
			sDesc = _T("ǥ����");
		else if( sCode == _T("N") )
			sDesc = _T("ǥ�þ���");
		else
			sDesc = _T("ǥ�þ���");
	}
	//09Dlg (���δ��⿹�༳��)
	else if( sAbility == _T("����������") )// EditBox
	{
		sDesc = sCode + _T("��");
	}
	//01Dlg (����/�ݳ�����&�̿���)
	else if( sAbility == _T("�Ϲݴ�����")		|| sAbility == _T("����������")		
			 || sAbility == _T("Ư��������")		|| sAbility == _T("��������")		
			 || sAbility == _T("�Ϲݹݳ����")		|| sAbility == _T("�ϰ��ݳ����")		
			 || sAbility == _T("�ұ޹ݳ����")		|| sAbility == _T("������������")		
			 || sAbility == _T("������������")	|| sAbility == _T("������̷º����ֱ�")	
			 || sAbility == _T("�η��ڷ����")		|| sAbility == _T("�η��ڷ�ݳ�")		
			 || sAbility == _T("����å���� ����")	|| sAbility == _T("����ȸ�����")		
			 || sAbility == _T("���������� �ڵ��ϰ�����/����") 
			 || sAbility == _T("�ֹε�Ϲ�ȣ �ڵ��ߺ��˻�")	
			 || sAbility == _T("�����ڹ�ȣ �����ID�� �ڵ��Է�")			// 2009.04.21 ADD BY KSJ
			 || sAbility == _T("�ֹε�Ϲ�ȣ ���ڸ� ��й�ȣ�� �ڵ��Է�") 	// 2009.04.21 ADD BY KSJ
			 // 17/05/18 ������ : ȸ���� ���������� �ź� Ȯ���� ���Ͽ� ����������� �߰�
//*/ ADD ---------------------------------------------------------------------------
			 || sAbility ==  _T("���������� ���� �̿�����ȸ ��뿩��")
//*/ END ---------------------------------------------------------------------------
			 //JOB.2019.0033 ADD BY KYJ
			 || sAbility ==  _T("Ÿ�ý���ī���ȣ��뿩��")
			)
	{
		if( sCode == _T("Y") )
			sDesc = _T("�����");
		else if( sCode == _T("N") ) 
			sDesc = _T("������");
		else
			sDesc = _T("������");
	}
	else if( sAbility == _T("�����ڹ�ȣ �ڵ��ο�") )
	{
		if( sCode == _T("N") )
			sDesc = _T("������");
		else if( sCode == _T("Y") )
			sDesc = _T("��������ȣ");
		else if( sCode == _T("A") )
			sDesc = _T("�ֹε�Ϲ�ȣ");
		else if( sCode == _T("U") )
			sDesc = _T("�����ID");
		else
			sDesc = _T("������");
	}
	else if( sAbility == _T("��������»��") )
	{
		if( sCode == _T("YY") )
			sDesc = _T("����,�ݳ� ���");
		else if( sCode.GetAt(0) == 'Y' )
			sDesc = _T("�����϶���");
		else if( sCode.GetAt(1) == 'Y' )
			sDesc = _T("�ݳ��϶���");
		else
			sDesc = _T("������");
	}
	else if( sAbility == _T("�����������") )// EditBox
	{
		sDesc = sCode + _T("��");
	}
	//03Dlg (��ü�ڷ�)
	else if( sAbility == _T("���������Ϻο�")	|| sAbility == _T("�ηϿ�ü�� ���������Ϻο�") || 
			 sAbility == _T("��ü��ο�")		|| sAbility == _T("���������� �ް�������") 
			 // 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
			 || sAbility == _T("��ü�Ⱓ �ް�������") 			 
//*/ END ---------------------------------------------------------------------------	
			 )
	{
		if( sCode == _T("Y") )
			sDesc = _T("�����");
		else if( sCode == _T("N") ) 
			sDesc = _T("������");
		else
			sDesc = _T("������");
	}
	else if( sAbility == _T("���������� �ο�����") )
	{
		if( sCode == _T("0") )
			sDesc = _T("��ü���� ���� ū ��ü��");
		else if( sCode == _T("1") )
			sDesc = _T("�� �ڷ��� ��ü�� ��");
		else
			sDesc = _T("�� �ڷ��� ��ü�� ��");
	}
	else if( sAbility == _T("��ü�ϰ�����1") || sAbility == _T("��ü�ϰ�����2") )
	{
		sDesc = sCode;
	}
	else if( sAbility == _T("���� ��üȽ�� �ο����") )
	{
		if( sCode == _T("0") )
			sDesc = _T("���������ο����� ��������");
		else if( sCode == _T("1") )
			sDesc = _T("��ü�� å���� �ο�");
		else
			sDesc = _T("���������ο����� ��������");
	}
	else if( sAbility == _T("��ü�� ����ġ") )// EditBox
	{
		sDesc = sCode;
	}
	else if( sAbility == _T("�⺻��ü��") )// EditBox
	{
		sDesc = sCode + _T("��");
	}	
	else if( sAbility == _T("�ִ���������ο��ϼ�") )// EditBox
	{
		sDesc = sCode + _T("��");
	}
	//07Dlg (���մ������)
	else if( sAbility == _T("�̿��ڰ˻��� ��������ȸ�� �ȳ�") || sAbility == _T("�̿��ڰ˻��� ����ȸ�� �ȳ�") )
	{
		if( sCode == _T("0") )
			sDesc = _T("������");
		else if( sCode == _T("1") )
			sDesc = _T("���޽���");
		else if( sCode == _T("2") )
			sDesc = _T("��������");
		else
			sDesc = _T("������");
	}
	else if( sAbility == _T("���͹��Խ� ID, ��й�ȣ ���Կ���") || sAbility == _T("���մ���å�� ��뿩��") )
	{
		if( sCode == _T("Y") )
			sDesc = _T("�����");
		else if( sCode == _T("N") ) 
			sDesc = _T("������");
		else
			sDesc = _T("������");
	}
	//10Dlg (���������)
	else if( sAbility == _T("�ηϿ�ü�� ���������� �ο�") || sAbility == _T("���������� �ް��� ��������") 
			// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
			|| sAbility == _T("��ü�Ⱓ �ް��� ��������")
//*/ END ---------------------------------------------------------------------------
			)
	{
		if( sCode == _T("Y") )
			sDesc = _T("�����");
		else if( sCode == _T("N") ) 
			sDesc = _T("������");
		else
			sDesc = _T("������");
	}
	else if( sAbility == _T("�⺻ ��ü��") )// EditBox
	{
		sDesc = sCode + _T("��");
	}	
	else if( sAbility == _T("�������� ��ϱ��� �ߺ�����"))
	{
		if( sCode == _T("Y") )
			sDesc = _T("�����");
		else if( sCode == _T("N") ) 
			sDesc = _T("������");
		else
			sDesc = _T("������");
	}
	else if(sAbility == _T("�⺻ �������"))
	{
		sDesc = sCode + _T("��");
	}

	// 2012.09.27 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
	//08Dlg (ȸ���������űⰣ)
	else if( sAbility == _T("ȸ������ ���űⰣ ����") )// EditBox
	{
		sDesc = sCode + _T("��");
	}
	
	return sDesc;
}

// ** �ش���(sAbility)�� �������� ������ �ڵ�� �����Ѵ�.
CString CChangeValue::DescToCode(CString sAbility, CString sDesc)
{
	CString sCode;
	// 02Dlg (�ݳ�����&����ȯ��)
	if( sAbility == _T("�ݳ������뿩��")			 || sAbility == _T("���ϴ����ڷ� �ݳ��������")	||
		sAbility == _T("�ݳ������� �ڵ��ο�")		 || sAbility == _T("�����ڵ����")				||
		sAbility == _T("���ุ���� �ڵ��ο�")		 || sAbility == _T("���ุ���Ϻο��� SMS�߼ۿ���") ||
		sAbility == _T("���ุ���� �ް��� ��������") || sAbility == _T("������� �ȳ����� �߼ۿ���") || 
		sAbility == _T("���ټ��⿹����") )
	{
		if( sDesc == _T("�����") )
			sCode = _T("Y");
		else if( sDesc == _T("������") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	else if( sAbility == _T("�ݳ������ ����켱���� ����") || sAbility == _T("�ݳ������� ���� ������") )
	{
		if( sDesc == _T("�ݳ�����켱") || sDesc == _T("�ݳ�������") )
			sCode = _T("0");
		else if( sDesc == _T("����켱") || sDesc == _T("������") ) 
			sCode = _T("1");
		else
			sCode = _T("0");
	}
	else if( sAbility == _T("������ ��뿩��") )
	{
		if( sDesc == _T("������") )
			sCode = _T("0");
		else if( sDesc == _T("�������� �ڷḸ ���డ��") )
			sCode = _T("1");
		else if( sDesc == _T("����ڷ� ���డ��") )
			sCode = _T("2");
		else
			sCode = _T("0");
	}
	//08Dlg (����������ȣ��å)
	else if( sAbility == _T("�ֹε�Ϲ�ȣ ǥ�ÿ���") || sAbility == _T("�����ڹ�ȣ ǥ�ÿ���") )
	{
		if( sDesc == _T("ǥ����") )
			sCode = _T("Y");
		else if( sDesc == _T("ǥ�þ���") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	//01Dlg (����/�ݳ�����&�̿���)
	else if( sAbility == _T("�Ϲݴ�����")		|| sAbility == _T("����������")		
			 || sAbility == _T("Ư��������")		|| sAbility == _T("��������")		
			 || sAbility == _T("�Ϲݹݳ����")		|| sAbility == _T("�ϰ��ݳ����")		
			 || sAbility == _T("�ұ޹ݳ����")		|| sAbility == _T("������������")		
			 || sAbility == _T("������������")	|| sAbility == _T("������̷º����ֱ�")	
			 || sAbility == _T("�η��ڷ����")		|| sAbility == _T("�η��ڷ�ݳ�")		
			 || sAbility == _T("����å���� ����")	|| sAbility == _T("����ȸ�����")		
			 || sAbility == _T("���������� �ڵ��ϰ�����/����")	
			 || sAbility == _T("�ֹε�Ϲ�ȣ �ڵ��ߺ��˻�")	
			 || sAbility == _T("�����ڹ�ȣ �����ID�� �ڵ��Է�")			// 2009.04.21 ADD BY KSJ
			 || sAbility == _T("�ֹε�Ϲ�ȣ ���ڸ� ��й�ȣ�� �ڵ��Է�")	// 2009.04.21 ADD BY KSJ)
			 // 17/05/18 ������ : ȸ���� ���������� �ź� Ȯ���� ���Ͽ� ����������� �߰�
//*/ ADD ---------------------------------------------------------------------------
			 || sAbility ==  _T("���������� ���� �̿�����ȸ ��뿩��")
//*/ END ---------------------------------------------------------------------------
			 //JOB.2019.0033 ADD BY KYJ
			 || sAbility ==  _T("Ÿ�ý���ī���ȣ��뿩��")
			)
	{
		if( sDesc == _T("�����") )
			sCode = _T("Y");
		else if( sDesc == _T("������") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	else if( sAbility == _T("�����ڹ�ȣ �ڵ��ο�") )
	{
		if( sDesc == _T("������") )
			sCode = _T("N");
		else if( sDesc == _T("��������ȣ") )
			sCode = _T("Y");
		else if( sDesc == _T("�ֹε�Ϲ�ȣ") )
			sCode = _T("A");
		else if( sDesc == _T("�����ID") )
			sCode = _T("U");
		else
			sCode = _T("N");
	}
	else if( sAbility == _T("��������»��") )
	{
		if( sDesc == _T("����,�ݳ� ���") )
			sCode = _T("YY");
		else if( sDesc == _T("�����϶���") )
			sCode = _T("YN");
		else if( sDesc == _T("�ݳ��϶���") )
			sCode = _T("NY");
		else
			sCode = _T("NN");
	}
	//03Dlg (��ü�ڷ�)
	else if( sAbility == _T("���������Ϻο�")	|| sAbility == _T("�ηϿ�ü�� ���������Ϻο�") || 
			 sAbility == _T("��ü��ο�")		|| sAbility == _T("���������� �ް�������") 
			 // 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
		     || sAbility == _T("��ü�Ⱓ �ް�������") 			 
//*/ END ---------------------------------------------------------------------------	
			)
	{
		if( sDesc == _T("�����") )
			sCode = _T("Y");
		else if( sDesc == _T("������") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	else if( sAbility == _T("���������� �ο�����") )
	{
		if( sDesc == _T("��ü���� ���� ū ��ü��") )
			sCode = _T("0");
		else if( sDesc == _T("�� �ڷ��� ��ü�� ��") )
			sCode = _T("1");
		else
			sCode = _T("1");
	}
	else if( sAbility == _T("��ü�ϰ�����1") || sAbility == _T("��ü�ϰ�����2") )
	{
		sCode = sDesc;
	}
	else if( sAbility == _T("���� ��üȽ�� �ο����") )
	{
		if( sDesc == _T("���������ο����� ��������") )
			sCode = _T("0");
		else if( sDesc == _T("��ü�� å���� �ο�") )
			sCode = _T("1");
		else
			sCode = _T("0");
	}
	//07Dlg (���մ������)
	else if( sAbility == _T("�̿��ڰ˻��� ��������ȸ�� �ȳ�") || sAbility == _T("�̿��ڰ˻��� ����ȸ�� �ȳ�") )
	{
		if( sDesc == _T("������") )
			sCode = _T("0");
		else if( sDesc == _T("���޽���") )
			sCode = _T("1");
		else if( sDesc == _T("��������") )
			sCode = _T("2");
		else
			sCode = _T("0");
	}
	else if( sAbility == _T("���͹��Խ� ID, ��й�ȣ ���Կ���") || sAbility == _T("���մ���å�� ��뿩��") )
	{
		if( sDesc == _T("�����") )
			sCode = _T("Y");
		else if( sDesc == _T("������") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	//10Dlg (���������)
	else if( sAbility == _T("�ηϿ�ü�� ���������� �ο�") || sAbility == _T("���������� �ް��� ��������") 
			// 17/04/17 ������ : ��ü�Ⱓ�� �ް������Կ��� �Ӽ� �߰�
//*/ ADD ---------------------------------------------------------------------------
			 || sAbility == _T("��ü�Ⱓ �ް��� ��������") 			 
//*/ END ---------------------------------------------------------------------------	
			)
	{
		if( sDesc == _T("�����") )
			sCode = _T("Y");
		else if( sDesc == _T("������") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	else if( sAbility == _T("�������� ��ϱ��� �ߺ�����"))
	{
		if( sDesc == _T("�����") )
			sCode = _T("Y");
		else if( sDesc == _T("������") ) 
			sCode = _T("N");
		else
			sCode = _T("N");
	}
	return sCode;
}

