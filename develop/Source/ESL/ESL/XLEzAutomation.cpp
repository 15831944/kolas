// EzAutomation.cpp: implementation of the CXLEzAutomation class.
//This wrapper classe is provided for easy access to basic automation  
//methods of the CXLAutoimation.
//Only very basic set of methods is provided here.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XLEzAutomation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//

//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////
CXLEzAutomation::CXLEzAutomation()
{
	//Starts Excel with bVisible = TRUE and creates empty worksheet 
	

	// 18.10.30 JMJ KOL.RED.2018.021
	//m_pXLServer = new CXLAutomation;
	m_pXLServer = NULL;
	m_bVisible = TRUE;
	// CXLEzAutomation ���� �� ���� �������� ���� �� �Ʒ��� ���� �⺻�� ����
	CLSID clsExcelApp;
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		m_bIsHancell = TRUE;
	}
	else if (!FAILED(CLSIDFromProgID(_T("Excel.Application"), &clsExcelApp))) 
	{
		m_bIsExcel = TRUE;
	}
}

CXLEzAutomation::CXLEzAutomation(BOOL bVisible)
{
	//Can be used to start Excel in background (bVisible = FALSE)

	// 18.10.30 JMJ KOL.RED.2018.021
	//m_pXLServer = new CXLAutomation(bVisible);
	m_pXLServer = NULL;
	m_bVisible = bVisible;
	// CXLEzAutomation ���� �� ���� �������� ���� �� �Ʒ��� ���� �⺻�� ����
	CLSID clsExcelApp;
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		m_bIsHancell = TRUE;
	}
	else if (!FAILED(CLSIDFromProgID(_T("Excel.Application"), &clsExcelApp))) 
	{
		m_bIsExcel = TRUE;
	}

}

CXLEzAutomation::~CXLEzAutomation()
{
	if(NULL != m_pXLServer)
		delete m_pXLServer;
}


//Create XY plot of the type xlXYScatterLinesNoMarkers
BOOL CXLEzAutomation::CreateXYChart(INT nYColumn)
{
	return m_pXLServer->CreateXYChart(nYColumn);
}

BOOL CXLEzAutomation::Create3YChart(INT nX1Column,INT nY1Column)
{
	return m_pXLServer->Create3YChart(nX1Column, nY1Column);
}
//Force update data source range . If new data points have been added to
//the data source, these points will be added to the plot.
//Default assumes data are placed in A and B columns of the Excel worksheet.
BOOL CXLEzAutomation::UpdateXYChart(INT nYColumn)
{
	return m_pXLServer->UpdatePlotRange(nYColumn);
}
//Use clipboard to export szDataCollection to Excel worksheet
// 18.10.30 JMJ KOL.RED.2018.021
//BOOL CXLEzAutomation::ExportCString(CString szDataCollection)
BOOL CXLEzAutomation::ExportCString(CString szDataCollection, CString strFileName)
{
	// 18.10.30 JMJ KOL.RED.2018.021
	// return m_pXLServer->PasteStringToWorksheet(szDataCollection);

	if(m_bIsHancell == TRUE || m_bIsExcel == TRUE){
		if(m_pXLServer == NULL)
			m_pXLServer = new CXLAutomation(m_bVisible, m_bIsExcel);

		//return m_pXLServer->PasteStringToWorksheet(szDataCollection);
		return m_pXLServer->PasteStringToWorksheet(szDataCollection, strFileName);
	}
	else if(m_bIsCSV == TRUE)
	{
		//return m_pXLServer->PasteStringToCSV(szDataCollection);
		return m_pXLServer->PasteStringToCSV(szDataCollection, strFileName);
	}
	else
	{
		return FALSE;
	}

}
//Set cell value: Worksheet.Cells(nColumn, nRow).Value = szValue
BOOL CXLEzAutomation::SetCellValue(INT nColumn, INT nRow, CString szValue)
{
	BOOL bResult;
	
	bResult = m_pXLServer->SetCellsValueToString((double) nColumn, (double)nRow, szValue);
	return bResult;
}
//Quit Excel
BOOL CXLEzAutomation::ReleaseExcel()
{
	return m_pXLServer->ReleaseExcel();
}
//Delete line from worksheet
BOOL CXLEzAutomation::DeleteRow(INT nRow)
{
	return m_pXLServer->DeleteRow(nRow);
}
//Save workbook as Excel file
BOOL CXLEzAutomation::SaveFileAs(CString szFileName)
{
	return m_pXLServer->SaveAs(szFileName, xlNormal,  _T(""), _T(""), FALSE, FALSE); 
}
//Returns Worksheet.Cells(nColumn, nRow).Value
CString CXLEzAutomation::GetCellValue(INT nColumn, INT nRow)
{
	return m_pXLServer->GetCellValueCString(nColumn, nRow);
}
//Insert picture from file. Position it at (Column, Row) on the worksheet. 
//The method resizes the picture to 50% of the original size (see 0.5, 0.5)
BOOL CXLEzAutomation::InsertPictureFromFile(CString szFileName, INT Column, INT Row)
{
	return m_pXLServer->InsertPictureToWorksheet(szFileName, Column, Row, 0.5, 0.5);
}
//Insert picture which was previously placed on clipboard
//Does not resize picture (see 0.0, 0.0)
BOOL CXLEzAutomation::InsertPictureFromClipboard(INT Column, INT Row)
{
	return m_pXLServer->InsertPictureToWorksheet(NULL, Column, Row, 0.0, 0.0);
}
//Place image to clipboard using XLAutomation method
BOOL CXLEzAutomation::PlacePictureToClipboard(BYTE *pImage)
{
	return m_pXLServer->PlaceImageToClipboard(pImage);
}
//Open Excell file
BOOL CXLEzAutomation::OpenExcelFile(CString szFileName)
{
	return m_pXLServer->OpenExcelFile(szFileName);
}
//Insert picture from buffer. If pImage = NULL, the picture from clipboard will be inserted.
//The picture is placed at (Column, Row) on worksheet
//The picture can be resized in % of its original size
//by specifying dXScale and dYScale
//If no resize is needed, make dXScal = 0.0 and dYScale = 0.0 
BOOL CXLEzAutomation::InsertPictureFromBuffer(BYTE *pImage, INT Column, INT Row, double dXScale, double dYScale)
{
	return m_pXLServer->InsertPictureToWorksheet(pImage, Column, Row, dXScale, dYScale);
}


BOOL CXLEzAutomation::SetCellBackgroundColor(long Row, long Column, INT ColorIndex)
{
	return m_pXLServer->SetCellBackgroundColor(Row, Column, ColorIndex);
}