
#pragma once
#include<iostream>
#include<WinSock2.h>
#include<string>
#pragma comment (lib, "ws2_32.lib")

using namespace std;


class SocketServer
{
public:
	int star(int &port, string &addr);
	sockaddr_in set_send_addr(int &port, string &addr_string);
public:
	SOCKET sock;
	sockaddr_in send_addr;
	sockaddr_in recv_addr;
	int recv_addrlen = sizeof(recv_addr);
	int send_addrlen = sizeof(send_addr);

};


