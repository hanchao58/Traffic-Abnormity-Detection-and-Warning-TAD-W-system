#include<iostream>
#include<WinSock2.h>
#include<winsock.h>
#include<sstream>

#include "Python.h"
#include <numpy/arrayobject.h>	
#include <opencv2/opencv.hpp>

#include"SocketServer.h"
#include"TimeSyn.h"
#include"FileManage.h"
#include"PyCar.h"
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include "opencv2/imgproc/types_c.h"

using namespace cv;
using namespace std;

#pragma comment(lib,"ws2_32.lib")
#define BUF_SIZE 1024
#define PATH_LENGTH 20
using namespace std;
char sendBuff[BUF_SIZE];
char recvBuff[BUF_SIZE];
char fileName[PATH_LENGTH];


int main() {

	int recv_port = 4040;
	//string recv_addr = "192.168.253.1";
	string recv_addr = "169.254.157.2";      //��λ����ַ
	//cout << "������Ҫ�󶨵�IP��ַ��" << endl;
	//cin >> recv_addr;
	//cout << "������Ҫ�󶨵Ķ˿ںţ�" << endl;
	//cin >> recv_port;
	int send_port = 4040;
	//string send_addr = "192.168.135.85";
	string send_addr = "169.254.157.11";       //��λ����ַ

	SocketServer server;
	int socket_server = server.star(recv_port, recv_addr);
	server.set_send_addr(send_port, send_addr);

	//TimeSyn timesyn(server);      //��ʼ��timesyn
	//timesyn.star();         //ʱ��ͬ��

	//FileManage filemanage;    
	//filemanage.star(server);    //�ļ�����

	Mat image;
	char recv_buf[380000] = { '\0' };
	char buf[65500] = { '\0' };
	PyCar pycar;
	pycar.star();     //��ʼ��python�������
	int n = 0;     //ÿ�ν��յĳ���
	int m = 0;     //�ܽ��ճ���
	int len = 0;   //�Է����͹����ĳ���

	while (true)
	{
		n = 0;     //ÿ�ν��յĳ���
		m = 0;     //�ܽ��ճ���
		len = 0;   //�Է����͹����ĳ���
		while (true)
		{
			n = recvfrom(server.sock, buf, 65500, 0, (sockaddr*)&server.recv_addr, &server.recv_addrlen);

			if ((buf[0] == 'e')&&(buf[1] == 'n')&&(buf[2] == 'd'))
			{
				char c[20] = { '\0' };
				stringstream sstream;

				sstream << buf;
				sstream >> c;
				sstream >> len;
				memset(buf, 0, n);
				break;
			}

			memcpy(recv_buf + m, buf, n);
			m = m + n ;

			memset(buf, 0, n);
		}
		//if (m != len)
		if (1.1*m < len)      //��������
		{
			memset(recv_buf, 0, 380000);
			continue;
		}

		vector<uchar> decode(&recv_buf[0], &recv_buf[m]);
		image = imdecode(decode, CV_LOAD_IMAGE_COLOR);

		if (image.rows == 0 || image.cols == 0)
		{
			memset(recv_buf, 0, 380000);
			continue;
		}
		memset(recv_buf, 0, 380000);


		//imshow("server_recv_video", image);
		pycar.findcar(image);

		if (waitKey(1) == 27)
			break;
	}
	cv::destroyAllWindows();


	closesocket(server.sock);
	WSACleanup();
	return 0;
}

