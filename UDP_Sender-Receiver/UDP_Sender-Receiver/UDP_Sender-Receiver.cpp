// UDP_Sender-Receiver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <conio.h>

//#define _CRT_SECURE_NO_WARNINGS  

#pragma comment (lib, "ws2_32.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	//B1 - khoi tao winsock
	WSAData wsaData;
	int ret = WSAStartup(0x0202, &wsaData);
	if (ret != 0){
		printf("khong the tao dc winsock");
		return 0;
	}

	//B2 - tao socket
	SOCKET sender;
	sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//B3 - xac dinh dia chi
	addrinfo* result;
	SOCKADDR_IN receiverAddr;
	if (getaddrinfo("127.0.0.1", "8888", NULL, &result) != 0)
	{
		printf("loi phan giai ten mien");
		return 0;
	}

	memcpy(&receiverAddr, result->ai_addr, result->ai_addrlen);
	freeaddrinfo(result);

	//B4 - sendto
	char szXau[1024];
	int len;
	while (1){
		printf("Nhap 1 xau: ");
		scanf("%s", &szXau);
		strcat(szXau, "\n");
		ret = sendto(sender, szXau, strlen(szXau), 0, (sockaddr*)&receiverAddr, sizeof(receiverAddr));

		if (ret == SOCKET_ERROR)
		{
			printf("loi gui xau: %d", WSAGetLastError());
		}

		//nhan data tu receiver
		int receiverAddrLen = sizeof(receiverAddr);
		len = recvfrom(sender, szXau, 1024, 0, (sockaddr*)&receiverAddr, &receiverAddrLen);
		if (len > 0){
			szXau[len] = 0;
			printf("Receiver reply: %s", szXau);
		}
	}

	//B5 - giai phong winsock
	closesocket(sender);
	WSACleanup();

	return 0;
}

