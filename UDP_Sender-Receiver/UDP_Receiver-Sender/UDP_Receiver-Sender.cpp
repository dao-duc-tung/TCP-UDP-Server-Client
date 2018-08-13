// UDP_Receiver-Sender.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
//#include <WS2tcpip.h>
#include <conio.h>
#include <stdio.h>

#pragma comment (lib, "ws2_32.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	//B1 - khoi tao winsock
	WSAData wsaData;
	int ret = WSAStartup(0x0202, &wsaData);
	if (ret != 0)
	{
		printf("khong the tao dc winsock");
		return 0;
	}

	//B2 - tao socket de nhan data
	SOCKET receiver;
	receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//B3 - bind
	SOCKADDR_IN receiverAddr;
	receiverAddr.sin_family = AF_INET;
	receiverAddr.sin_port = htons(8888);
	receiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(receiver, (sockaddr*)&receiverAddr, sizeof(receiverAddr));
	if (ret == SOCKET_ERROR){
		printf("Loi bind: %d!", WSAGetLastError());
		return 0;
	}

	//B4 - nhan data
	SOCKADDR_IN senderAddr;
	int senderAddrLen = sizeof(senderAddr);
	char szXau[1024];
	int len = 0;
	while (1){
		len = recvfrom(receiver, szXau, 1024, 0, (sockaddr*)&senderAddr, &senderAddrLen);
		if (len > 0)
		{
			szXau[len] = 0;
			printf("[%s:%d] says: %s\n", inet_ntoa(senderAddr.sin_addr), ntohs(senderAddr.sin_port), szXau);
			sendto(receiver, szXau, len, 0, (sockaddr*)&senderAddr, sizeof(senderAddr));
			if (strncmp(szXau, "bye", 3) == 0) //so sanh 3 ki tu dau
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	//B5 - giai phong
	closesocket(receiver);
	WSACleanup();

	return 0;
}

