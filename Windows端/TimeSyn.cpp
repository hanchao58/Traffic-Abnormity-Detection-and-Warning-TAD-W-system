#include "TimeSyn.h"


int TimeSyn::gettime_stamp()
{	//直接调用此函数就可以返回时间戳了
	auto c_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	return c_time.count();
}


TimeSyn::TimeSyn(SocketServer &server)
{
	this->server = server;
}


int TimeSyn::star()
{
	int length1;
	char fsendc[BUF_SIZE] = "success";
	char recvBuff[BUF_SIZE];
	length1 = recvfrom(server.sock, recvBuff, BUF_SIZE, 0, (sockaddr*)&server.recv_addr, &server.recv_addrlen);
	int t2 = gettime_stamp();

	int t3 = gettime_stamp();
	sendto(server.sock, fsendc, sizeof(fsendc) + 1, 0, (SOCKADDR*)&server.send_addr, server.send_addrlen);
	cout << "Successfully received!" << endl;

	char c[BUF_SIZE] = { '\0' };
	string s;
	string s2;
	string s3;
	stringstream sstream;

	sstream << t2;
	sstream << " ";
	sstream << t3;
	sstream >> s2;
	sstream >> s3;
	s = s2 + " " + s3;
	strcpy(c, s.c_str());

	sendto(server.sock, c, BUF_SIZE + 1, 0, (SOCKADDR*)&server.send_addr, server.send_addrlen);

	return 0;
}
