#include "FileManage.h"

int FileManage::star(SocketServer &server)
{
	while (true)
	{
	cout << "---------------------RECEIVING...---------------------" << endl;
	//接收文件名
	char fileName[NAME_SIZE];
	int ret = recvfrom(server.sock, fileName, BUF_SIZE, 0, (sockaddr*)&server.recv_addr, &server.recv_addrlen);

	cout << "Filename: " << fileName << endl;

	FILE *fp;
	errno_t err;
	if ((err = fopen_s(&fp, fileName, "wb")) < 0)
	{
		cout << "Create failed." << endl;
		return -1;
	}

	int length;
	char recvBuff1[BUF_SIZE];

	while ((length = recvfrom(server.sock, recvBuff1, BUF_SIZE, 0, (sockaddr*)&server.recv_addr, &server.recv_addrlen)))
	{

		if (!strcmp(recvBuff1, "end"))//接收结束信息
			break;
		if (length == 0)
		{
			cout << "An error occurred while receiving." << endl;
			return -1;
		}
		int ret = fwrite(recvBuff1, 1, length, fp);
		//int ret = fwrite(recvBuff1, 1, BUF_SIZE, fp);
		if (ret < length)
		{
			cout << "Write failed." << endl;
			return -1;
		}
		//sendto(server, "success", sizeof("success") + 1, 0, (SOCKADDR*)&server_addr, sizeof(server_addr));
	}

	//int t_end = timesyn.gettime_stamp();
	//cout << "Successfully received!" << endl;

	//string s_end;
	//stringstream sstream_end;
	//sstream_end << t_end;
	//sstream_end >> s_end;

	//char c_end[BUF_SIZE] = { '\0' };
	//strcpy(c_end, s_end.c_str());
	//cout << c_end << endl;

	//_sleep(2);           //延时
	//sendto(server.sock, c_end, BUF_SIZE + 1, 0, (SOCKADDR*)&server.send_addr, server.send_addrlen);

	fclose(fp);
	}


}


