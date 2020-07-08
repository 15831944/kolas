// SettingGrid.cpp: implementation of the CSettingGrid class.
// 2008.07.29 MAKE BY PWR
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SettingGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSettingGrid::CSettingGrid()
{
	// 2010.07.28 ADD BY KSJ : ���� ȸ�� ���� �ý��� ��뿩��
	m_bIsUnityLoanService = FALSE;
}

CSettingGrid::~CSettingGrid()
{

}

// 01Dlg (����/�ݳ�����&�̿���)
VOID CSettingGrid::SettingGrid01(CESL_DataMgr *pDM, CESL_Grid *pGrid)
{
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	CString sValue , strUserNoAutoMode , strUserNoToUserID;
	// ������������, �η��ڷ���⿡ ���� ����
	// DM����
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		sValue = pDM->GetCellData( _T("���"), i );
		if( sValue == _T("������������") || sValue == _T("�η��ڷ����") )
		{
			sValue = pDM->GetCellData( _T("������"), i );
			if( sValue != _T("Y") )
			{
				pDM->SetCellData( _T("������"), _T("N"), i+1 ); 
				pGrid->SetCol(4);
				pGrid->Display(i+2);
			}
		}

		// 2009.10.19 ADD BY KSJ : �����ڹ�ȣ �ڵ��ο��� '�����ID'�� ��� �����ڹ�ȣ �����ID�� �ڵ��Է� �ɼ� '������'
		if( _T("�����ڹ�ȣ �ڵ��ο�") == sValue )
		{
			strUserNoAutoMode = pDM->GetCellData( _T("������"), i );
			// 2010.12.22 DELETE BY KSJ : �Ϲ�ȸ���̶�� �����ڹ�ȣ �ڵ��ο� ������ ��� �� �� �ֵ��� ����
			// 2010.07.28 ADD BY KSJ : ���� ȸ�� ���� �ý��� ��� �� �� �����ڹ�ȣ �ڵ��ο��� �׻� '�ڵ��ο�'
//DEL 			if(m_bIsUnityLoanService)
//DEL 			{
//DEL 				pDM->SetCellData( _T("������"), _T("Y"), i ); 
//DEL 				pGrid->SetCol(4);
//DEL 				pGrid->Display(i+1);			
//DEL 			}

			if( _T("U") == strUserNoAutoMode  )
			{
				pDM->SetCellData( _T("������"), _T("N"), i+1 ); 
				pGrid->SetCol(4);
				pGrid->Display(i+2);
			}
		}

		if( _T("�����ڹ�ȣ �����ID�� �ڵ��Է�") == sValue )
		{
			strUserNoToUserID = pDM->GetCellData( _T("������"), i );
		}

		if( _T("�ֹε�Ϲ�ȣ ���ڸ� ��й�ȣ�� �ڵ��Է�") == sValue )
		{
			if( _T("U") != strUserNoAutoMode && _T("Y") != strUserNoToUserID )
			{
				pDM->SetCellData( _T("������"), _T("N"), i ); 
				pGrid->SetCol(4);
				pGrid->Display(i+1);
			}
		}
	}

	// �׸��忡 ����ȵǴ� �κ� -ǥ��
	for( i = 0; i < pDM->GetRowCount(); i++ )
	{
		sValue = pDM->GetCellData( _T("���"), i );
		if( sValue == _T("������������") || sValue == _T("�η��ڷ����") )
		{
			sValue = pDM->GetCellData( _T("������"), i );
			if( sValue == _T("Y") )
			{
				pGrid->SetCol(4);
				pGrid->SetRow(i+2);
				pGrid->SetCellFontStrikeThrough(FALSE);
			}
			else
			{
				pGrid->SetCol(4);
				pGrid->SetRow(i+2);
				pGrid->SetCellFontStrikeThrough(TRUE);
			}
		}

		// 2009.10.19 ADD BY KSJ : �����ڹ�ȣ �ڵ��ο��� '�����ID'�� ��� �����ڹ�ȣ �����ID�� �ڵ��Է� �ɼ� �����Ұ�
		if( _T("�����ڹ�ȣ �ڵ��ο�") == sValue )
		{
			// 2010.12.22 DELETE BY KSJ : �Ϲ�ȸ���̶�� �����ڹ�ȣ �ڵ��ο� ������ ��� �� �� �ֵ��� ����
			// 2010.07.28 ADD BY KSJ : ���� ȸ�� ���� �ý��� ��� �� �� �����ڹ�ȣ �ڵ��ο��� �׻� '�ڵ��ο�'
//DEL 			if(m_bIsUnityLoanService)
//DEL 			{
//DEL 				pGrid->SetCol(4);
//DEL 				pGrid->SetRow(i+1);
//DEL 				pGrid->SetCellFontStrikeThrough(TRUE);
//DEL 			}
//DEL 			else
//DEL 			{
				strUserNoAutoMode = pDM->GetCellData( _T("������"), i );
				if( _T("U") == strUserNoAutoMode )
				{
					pGrid->SetCol(4);
					pGrid->SetRow(i+2);
					pGrid->SetCellFontStrikeThrough(TRUE);
				}
				else
				{
					pGrid->SetCol(4);
					pGrid->SetRow(i+2);
					pGrid->SetCellFontStrikeThrough(FALSE);
				}
//DEL 			}
		}

		if( _T("�����ڹ�ȣ �����ID�� �ڵ��Է�") == sValue )
		{
			strUserNoToUserID = pDM->GetCellData( _T("������"), i );
		}

		if( _T("�ֹε�Ϲ�ȣ ���ڸ� ��й�ȣ�� �ڵ��Է�") == sValue )
		{
			if( _T("U") != strUserNoAutoMode && _T("Y") != strUserNoToUserID )
			{
				pGrid->SetCol(4);
				pGrid->SetRow(i+1);
				pGrid->SetCellFontStrikeThrough(TRUE);
			}
			else
			{
				pGrid->SetCol(4);
				pGrid->SetRow(i+1);
				pGrid->SetCellFontStrikeThrough(FALSE);
			}
		}
	}
	// ��Ŀ������ġ
	pGrid->SetCol(0);
	pGrid->SetRow(0);
}

// 02Dlg (�ݳ�����&����ȯ��)
VOID CSettingGrid::SettingGrid02(CESL_DataMgr *pDM, CESL_Grid *pGrid)
{
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	// �ݳ������뿡 ���� ����
	const int nCnt = 5;
	INT nNumber[nCnt] = { 2, 3, 4, 5, 6 };
	CString sValue;
	sValue = pDM->GetCellData( _T("������"), 1 - 1 );
	pGrid->SetCol(4);
	if( sValue == _T("Y") )
	{
		for( INT i = 0; i < nCnt; i++ )
		{
			pGrid->SetRow(nNumber[i]);
			pGrid->SetCellFontStrikeThrough(FALSE);
		}
	}
	else
	{
		for( INT i = 0; i < nCnt; i++ )
		{
			pGrid->SetRow(nNumber[i]);
			pGrid->SetCellFontStrikeThrough(TRUE);
		}
	}

	// �����ɻ�뿡 ���� ����
	const int nCnt2 = 2;
	INT nNumber2[nCnt] = { 7, 11 };
	sValue = pDM->GetCellData( _T("������"), 9 - 1 );
	if( sValue == _T("0") )
	{
		for( INT i = 0; i < nCnt2; i++ )
		{
			pGrid->SetRow(nNumber2[i]);
			pGrid->SetCellFontStrikeThrough(TRUE);
		}
	}
	else
	{
		for( INT i = 0; i < nCnt2; i++ )
		{
			pGrid->SetRow(nNumber2[i]);
			pGrid->SetCellFontStrikeThrough(FALSE);
		}
	}
	// ��Ŀ�� ����ġ
	pGrid->SetCol(0);
	pGrid->SetRow(0);
}

VOID CSettingGrid::SettingGrid08(CESL_DataMgr *pDM, CESL_Grid *pGrid)
{
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	// 14/07/16 �ڴ�� : ���������� �������� ���� ���� �����κ� ����
/*//REM--------------------------------------------------------------------------
	// ���� ȸ�� ���� �ý��� ��뿡 ���� ����
	CString strValue;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		strValue = pDM->GetCellData( _T("���"), i );
		if( _T("�ֹε�Ϲ�ȣ ǥ�ÿ���") == strValue )
		{
			if(m_bIsUnityLoanService)
			{
				pDM->SetCellData( _T("������"), _T("Y"), i ); 
				pGrid->SetCol(4);
				pGrid->Display(i+1);
			}
		}
	}

	// �׸��忡 ����ȵǴ� �κ� -ǥ��
	for( i = 0; i < pDM->GetRowCount(); i++ )
	{
		strValue = pDM->GetCellData( _T("���"), i );
		if( _T("�ֹε�Ϲ�ȣ ǥ�ÿ���") == strValue )
		{
			if(m_bIsUnityLoanService)
			{
				pGrid->SetCol(4);
				pGrid->SetRow(i+1);
				pGrid->SetCellFontStrikeThrough(TRUE);
			}
		}
	}
*///END--------------------------------------------------------------------------
	// ��Ŀ�� ����ġ
	pGrid->SetCol(0);
	pGrid->SetRow(0);
}

VOID CSettingGrid::SetIsUnityLoanService(BOOL bIsUnityLoanService) 
{
	m_bIsUnityLoanService = bIsUnityLoanService;
};