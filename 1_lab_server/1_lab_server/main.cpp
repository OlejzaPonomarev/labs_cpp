using namespace std;

#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")// ���������� ����������
#define SERVER_PORT 3820


int main()
{
	setlocale(LC_CTYPE, "Russian"); // �������� ������� ���������

	struct sockaddr_in SrvAddr;     // �������� ��������� �������

	struct sockaddr_in ConnAddr;  // �������� ��������� �������

	SOCKET SrvSock, Conn;

	WSADATA wsaData;  // ��� ������ Windows � ��������
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // ��������� ����������
	if (iResult < 0)
	{
		cout << "������" << endl;
		getchar();
		exit(0);
	}


	SrvSock = socket(AF_INET, SOCK_STREAM, 0);//�������  ����� �������

	//������ ����� �������
	SrvAddr.sin_family = AF_INET;
	SrvAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);// 127.0.0.1
	SrvAddr.sin_port = htons(SERVER_PORT);
	//����������� �����
	bind(SrvSock, (sockaddr*)&SrvAddr, sizeof SrvAddr);

	//�������
	listen(SrvSock, 5);

	printf("...������ ��������...\n");
	int AddrLen = sizeof(ConnAddr);
	char buf_in[20];
	char exit[20] = "exit";
	char buf_out[34] = "������ �� �������!";
	int nsize;
	int bytes = 0;
	bool stop = false;



	//���� ���������� �������
	while (1)
	{
		Conn = accept(SrvSock, (struct sockaddr*)&ConnAddr, &AddrLen);	 // ������ ����� � ������ ��������
		//HOSTENT* hst ;
	   //hst = gethostbyaddr((char *)&ConnAddr. sin_addr.s_addr, 4, AF_INET);
	   //cout<<"����������� " << inet_ntoa(ConnAddr.sin_addr)<<endl;

		bytes = recv(Conn, (char*)buf_in, sizeof(buf_in), 0);//������ ���������� � �����
		cout << buf_in << endl;
		send(Conn, (char*)buf_out, sizeof(buf_out), 0);// �������� "������"

	}

	shutdown(Conn, 2); // ��������� �������� � ����� ���������
	closesocket(Conn); //�������=��������� �����


	cout << endl << "����� �� ��������� ����� ���������" << endl;
	getchar();
	return 0;
}
