// TCP_Server-Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <WinSock2.h>

#pragma comment (lib, "ws2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	//B1 - khoi tao WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);	//phien ban 0x0202
	int ret;
	ret = WSAStartup(wVersion, &wsaData);

	//neu tra ve 0 thi thanh cong
	if (ret)
	{
		printf( "Version is not supported");
		getchar();
		return 0;
	}


	//B2 - Tao socket
	SOCKET server;
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//AF_INET: chon giao thuc TCP/IP de ket noi
	//SOCK_STREAM: chon giao thuc TCP de transport data
	//IPPROTO_TCP: chon giao thuc kiem soat mang, kiem tra ket noi
	// (voi TCP thi protocol nay la IPPROTO_IP)


	//B3 - Bind
	SOCKADDR_IN serverAddr;
	//cau truc SOCKADDR_IN de bieu dien d/c IP

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8888);	//port de listen ket noi
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
								//inet_addr("0.0.0.0");
	//sin_addr: la 1 struct de xac dinh d/c IP
	//neu sin_addr.s_addr=INADDR_ANY; thi IP la IP cua minh
	// (cac server thuong dung d/c IP cua minh de listen ket noi)

	//neu sin_addr.s_addr=inet_addr("x.x.x.x"); thi IP la theo x.x.x.x
	// (client phai chi dinh dc IP cua server moi ket noi dc)


	//bind giup cho SOCKET cua SERVER biet rang
	// n se cho doi ket noi va nhan du lieu tren IP nao va PORT bnhieu
	ret = bind(server, (sockaddr*)&serverAddr, sizeof(serverAddr));
	//server: SOCKET duoc thiet lap o tren
	//&serverAddr: la cau truc sockaddr, bao gom IP va PORT
	//sizeof(serverAddr): kich thuoc cua cau truc sockaddr

	if (ret==SOCKET_ERROR)
	{
		printf( "Loi bind: %d", WSAGetLastError());
		getchar();
		return 0;
	}


	//B4 - Listen
	ret = listen(server, 16);
	//server: SOCKET dc thiet lap o tren
	//16: so ket noi cho phep cho trong hang doi (queue)
	// khi server chua chap nhan ket noi 
	// (vi co the 2,3 client ket noi cung luc) (tot nhat la 5-10)

	if (ret == SOCKET_ERROR)
	{
		printf( "Loi listen: %d", WSAGetLastError());
		getchar();
		return 0;
	}

	printf("Dang doi ket noi...\n");

	//B5 - accept
	SOCKET client;
	SOCKADDR_IN clientAddr;
	int clientAddrLen = sizeof(clientAddr);

	client = accept(server, (sockaddr*)&clientAddr, &clientAddrLen);
	//server: SOCKET lang nghe tren server
	//&clientAddr: la cau truc sockaddr, lua IP va PORT cua Client knoi toi
	//&clientAddrLen: k/thuoc cua cau truc d/c IP sockaddr_in clientAddr


	printf( "Co ket noi moi tu: %s:%d !\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

	//B6 - send: gui du lieu
	char* szXau = "Hello Network Programming\n";

	send(client, szXau, (strlen(szXau)), 0);
	//client: SOCKET dc tao ra khi Server chap nhan ket noi tu client
	//szXau: la du lieu (dang BYTE - char) gui
	//strlen(szXau): kich thuoc du lieu
	//0: flag di kem, thuong = 0

	//B7 - recv: nhan du lieu
	char szBuff[1024];	//buf de luu du lieu nhan dc
	int len = 0;
	while (1)
	{
		len = recv(client, szBuff, 1024, 0);
		//client: tuong tu send
		//szBuff: du lieu (dang BYTE - char) nhan
		//1024: tuong tu send
		//0: tuong tu send

		if (len>0)
		{
			szBuff[len] = 0; //them ky tu NULL \0 vao cuoi xau
			printf("Client: %s\n", szBuff);
		}
		else
		{
			printf("Client dong ket noi !");
			break;
		}
	}

	getchar();
	return 0;
}

