
#pragma once
#include<iostream>
#include<WinSock2.h>
#include<string>
#include<cstring>
#include <fstream>
#include<ctime>
#include <io.h>
#include<sstream>
#include<chrono>
#include "SocketServer.h"


#define BUF_SIZE 1024
using namespace std;
using namespace std::chrono;

class TimeSyn
{
public:
	TimeSyn(SocketServer &server);
	int star();
	int gettime_stamp();
public:
	SocketServer server;
};
