// Client-Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	//B1 - khoi tao winsock
	WSAData wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	int ret = WSAStartup(wVersion, &wsaData);

	//neu tra ve 0 thi thanh cong
	if (ret)
	{
		printf("Version is not supported");
		getchar();
		return 0;
	}

	//B2 - dien dia chi server
	SOCKADDR_IN serverAddr;	//cau truc sockaddr_in bieu dien d/c IP

	char szHostname[1024];	//luu host ket noi toi
	printf("Nhap dia chi ip/ten mien: ");
	scanf("%s", &szHostname);

	addrinfo* result;
	//cau truc addrinfo co 8 truong, qtrong la 4 truong dau
	//luu ten mien/IP dc phan giai
	//luu loai d/c IP dc phan giai (v4, v6)
	//luu kieu socket: SOCK_DGRAM/SOCK_STREAM
	//luu kieu giao thuc: thuong la IPPROTO_TCP

	ret = getaddrinfo(szHostname, "8888", NULL, &result);
	//szHostname: ten mien hoac dia chi can phan giai
	//8888: xau chua so nguyen hoac chuoi mo ta dich vu
	//	(vi du "ftp" va "21" la tuong duong nhau)
	//NULL: con tro den cau truc addrinfo chua goi y cho ham thuc hien
	//&result: con tro den da^u\ cau truc danh sach lien ket addrinfo
	// (chua danh sach cac ket qua phan giai dc)


	if (ret != 0)
	{
		printf("Loi phan giai ten mien: %d!!!", ret);
		getchar();
		return 0;
	}

	memcpy(&serverAddr, result->ai_addr, result->ai_addrlen);
	//copy struct sockaddr* ai_addr vao serverAddr

	freeaddrinfo(result);
	//giai phong result sau khi dung
	// (giai phong addrinfo do he thong cap phat)

	//B3 - ket noi
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	ret = connect(client, (sockaddr*)&serverAddr, sizeof(serverAddr));
	//client: socket da dc khoi tao
	//&serverAddr: IP va PORT cua Server
	//sizeof(serverAddr): kich thuoc cau truc sockaddr

	if (ret == SOCKET_ERROR)
	{
		printf("Loi ket noi: %d", WSAGetLastError());
		getchar();
		return 0;
	}

	printf("Da ket noi den server !\n");

	//B4 - nhan du lieu tu server
	char szBuff[1024];
	int len;
	len = recv(client, szBuff, 1024, 0);
	if (len > 0)
	{
		szBuff[len] = 0;
		printf("Server: %s\n", szBuff);
	}

	//B5 - Gui du lieu den Server
	while (1)
	{
		printf("Nhap mot xau: ");
		scanf("%s", szBuff);
		strcat(szBuff, "\n");
		send(client, szBuff, strlen(szBuff), 0);
	}

	//B6 - dong socket
	closesocket(client);

	//B7 - giai phong winsock
	WSACleanup();
	printf("Xong !");

	getchar();
	return 0;
}

