#include "SocketServer.h"


int SocketServer::star(int &port, string &addr_string)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)    //MAKEWORD(1,1)和MAKEWORD(2,2)的区别在于，前者只能一次接收一次，不能马上发送，而后者能。
	{
		cout << "Initialization failed." << endl;
		return -1;
	}

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sock == -1)
	{
		cout << "Socket failed." << endl;
		return -1;
	}

	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(port);
	//server_addr.sin_addr.S_un.S_addr = inet_addr(SERVER_ID);
	char addr_char[20];
	strcpy(addr_char, addr_string.c_str());
	recv_addr.sin_addr.s_addr = inet_addr(addr_char);

	if (bind(sock, (sockaddr*)&recv_addr, sizeof(recv_addr)) == SOCKET_ERROR)
	{
		cout << "Bind error!" << endl;
		return -1;
	}

	return sock;
}


sockaddr_in SocketServer::set_send_addr(int &port, string &addr_string)
{
	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(port);
	char addr_char[20];
	strcpy(addr_char, addr_string.c_str());
	send_addr.sin_addr.s_addr = inet_addr(addr_char);
	return send_addr;
}
