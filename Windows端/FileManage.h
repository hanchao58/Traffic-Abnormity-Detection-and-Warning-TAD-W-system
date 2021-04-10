#pragma once
#include<iostream>
#include<WinSock2.h>
#include"SocketServer.h"

using namespace std;

#define NAME_SIZE 30
#define BUF_SIZE 1024

class FileManage
{
public:
	int star(SocketServer &server);	

};

