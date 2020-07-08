#include "stdafx.h"
#include "NetworkInfoMgr.h"
//#include "Iphlpapi.h"

// typedef struct _ASTAT_
// {
//     ADAPTER_STATUS adapt;
//     NAME_BUFFER    NameBuff [30];
// } ASTAT, * PASTAT;


CString GetLocalIP()
{
    WSADATA wsd ;
    unsigned int optval = 1 ;
    if(WSAStartup(MAKEWORD(1,1),&wsd) != 0) {   return ""; } 

    CString ip = "";
    CHAR szHostName[256];
    PHOSTENT pHostInfo;

    if( gethostname(szHostName,sizeof(szHostName)) ==0)
    {
        if((pHostInfo = gethostbyname(szHostName)) != NULL)
        {
            ip = inet_ntoa(*(struct in_addr *)*pHostInfo->h_addr_list); 
        }
    }
	WSACleanup(); 

	return ip;
}

// CString GetLocalNetID()
// {	
// 	INT i;
// 	ASTAT Adapter;
// 	CString Message;
// 	CString strMyNetAddress;
// 
// 	NCB			Ncb;
// 	UCHAR		uRetCode;
// 	LANA_ENUM	lenum;
// 
// 	memset( &Ncb, 0, sizeof(Ncb) );
// 	Ncb.ncb_command = NCBENUM;
// 	Ncb.ncb_buffer = (UCHAR *)&lenum;
// 	Ncb.ncb_length = sizeof(lenum);
// 	uRetCode = Netbios( &Ncb );	
// 
// 	if ( lenum.length > 0)
// 	{
// 		for( i = 0; i < lenum.length; i++)
// 		{
// 			memset( &Ncb, 0, sizeof(Ncb) );
// 			Ncb.ncb_command = NCBRESET;
// 			Ncb.ncb_lana_num = lenum.lana[i];
// 
// 			uRetCode = Netbios( &Ncb );
// 			memset( &Ncb, 0, sizeof (Ncb) );
// 			Ncb.ncb_command = NCBASTAT;
// 			Ncb.ncb_lana_num = lenum.lana[i];					
// 
// 			strcpy( (CHAR*)Ncb.ncb_callname, "* " );
// 			
// 			Ncb.ncb_buffer = (UCHAR *) &Adapter;
// 			Ncb.ncb_length = sizeof(Adapter);
// 
// 			uRetCode = Netbios( &Ncb );
// 			if ( uRetCode == 0 )
// 			{
// 				strMyNetAddress.Format(_T("%02x%02x%02x%02x%02x%02x"),
// 				Adapter.adapt.adapter_address[0],
// 				Adapter.adapt.adapter_address[1],
// 				Adapter.adapt.adapter_address[2],
// 				Adapter.adapt.adapter_address[3],
// 				Adapter.adapt.adapter_address[4],
// 				Adapter.adapt.adapter_address[5] );
// 			}
// 			else
// 			{
// 				strMyNetAddress = _T("") ;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		strMyNetAddress = _T("") ;
// 	}
// 
// 	strMyNetAddress.MakeUpper();	
// 	return strMyNetAddress ;
// 	return _T("");
// }


// CString GetLocalNetID2()
// {
// ������ �۴� �Լ�. �Ʒ� �ּ��� �����ϸ� 
// ��Ÿ���� RSMgr�� LoadConfig���� -3������ ����.

// 	DWORD dwStatus;
//     IP_ADAPTER_INFO *pAdapterInfo = NULL;
//     IP_ADAPTER_INFO *pOriginalPtr = NULL;
//     ULONG ulSizeAdapterInfo = 0;
//     dwStatus = GetAdaptersInfo(pAdapterInfo,&ulSizeAdapterInfo);
//     //***********************************************************************
//     //���� ���� �÷ο� �϶� ulSizeAdapterInfo ũ��� �޸𸮸� �Ҵ��ϰ� 
//     //�ٽ� GetAdaptersInfo�� ȣ���Ѵ�.
//     if( dwStatus == ERROR_BUFFER_OVERFLOW)
//     {
//         if( !(pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulSizeAdapterInfo)) )
//         {
//             MessageBox(NULL,_T("Insufficient Memory"),_T("GetMacAddress Error"),MB_OK);
//             return "";
//         }
//         dwStatus = GetAdaptersInfo(pAdapterInfo,&ulSizeAdapterInfo);
//     }
// 	
//     //***********************************************************************
//     TCHAR MacAddr[18];
//     pOriginalPtr = pAdapterInfo;
//     if( pAdapterInfo == NULL )
//     {
//         MessageBox(NULL,_T("No Interfaces Present \n"),_T("Error"),MB_OK);
//         return _T("");
//     }
// 	//Nicī�尡 ������ �ִٸ� ���⼭ ó���ϸ�ǰ���..
//     while( pAdapterInfo != NULL )
//     {
// 		//���⼭�� ���ּҸ��� ó���մϴ�.
//         if( pAdapterInfo->AddressLength == 6 )
//         {
//             wsprintf(MacAddr,_T("%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X"),
//                 (DWORD)pAdapterInfo->Address[0],                
//                 (DWORD)pAdapterInfo->Address[1],                
//                 (DWORD)pAdapterInfo->Address[2],                
//                 (DWORD)pAdapterInfo->Address[3],                
//                 (DWORD)pAdapterInfo->Address[4],                
//                 (DWORD)pAdapterInfo->Address[5]);           
//         }
//         pAdapterInfo = pAdapterInfo->Next;
//     }
//     free(pAdapterInfo);
// 
// 	CString strMacAddr;
// 	strMacAddr.Format(_T("%s"), MacAddr);    
// 	
//  return strMacAddr;
// 
// 	return _T("");
// }