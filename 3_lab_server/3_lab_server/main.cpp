using namespace std;

#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")// ���������� ����������
#define SERVER_PORT 3820

DWORD WINAPI ClientFunc(LPVOID lpParam) // �������, ������� ����� ����� ��������� � ��������� ������
{
	SOCKET Conn = *(SOCKET*)lpParam; // ������� �������� � ������� ���� 

	bool stop_word_flag = false;
	int len_stop_word = 5;
	char buf_in[20];
	char exit[5] = "exit";
	char buf_out[35] = "������ �� �������!\n";

	cout << "������ �����������\n";

	while (!stop_word_flag) {
		int bytes = recv(Conn, (char*)buf_in, sizeof(buf_in), 0); //������ ���������� � �����

		//��������� ���� �����
		int count = 0;
		for (int i = 0; i < 5; ++i) {
			if (buf_in[i] == exit[i]) {
				count += 1;
			}
		}

		if (count != 5) {
			cout << buf_in << endl;
			send(Conn, (char*)buf_out, sizeof(buf_out), 0);// �������� "������"
		}
		else {
			cout << "������ �������� ������� �����!\n";
			stop_word_flag = true;
		}
	}
	shutdown(Conn, 2); // ��������� �������� � ����� ���������
	closesocket(Conn); //�������=��������� �����
	return 0;
}

int main()
{
	DWORD ThreadId;
	INT ThreadParameter = 19;
	HANDLE hThread;
	setlocale(LC_CTYPE, "Russian");	//�������� ������� ���������
	struct sockaddr_in SrvAddr;     // �������� ��������� �������
	struct sockaddr_in ConnAddr;  // �������� ��������� �������
	SOCKET SrvSock, Conn;	//������� ������

	WSADATA wsaData;  // ��� ������ Windows � ��������
	//��������� WSADATA �������� �������� � ���������� ������� Windows.

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // ��������� ����������
	//������� WSAStartup ���������� ������������� winsock DLL ���������.
	//MAKEWORD(2,2) - �������� ��������� ������ ���������� �������
	//����� ���� ��������� �� ���������, �������� ������� ������ ���������� �������

	if (iResult < 0)	//� ������ ������ iResult = 0
	{
		cout << "������!\n" << endl;
		getchar();
		exit(0);
	}


	SrvSock = socket(AF_INET, SOCK_STREAM, 0); //�������  ����� �������
	//SOCKET socket(int <��������� ������������ �������>, int <��� ������>, int <��� ���������>)
	//IPv4 - AF_INET, TCP (SOCK_STREAM) ��� UDP (SOCK_DGRAM), 0 - �������� ���� ���������

	//������ ����� �������
	SrvAddr.sin_family = AF_INET;	//��������� �������
	SrvAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);// 127.0.0.1
	SrvAddr.sin_port = htons(SERVER_PORT);
	//SrvAddr ������ ���������� ��� �������� ������

	//����������� �����
	bind(SrvSock, (sockaddr*)&SrvAddr, sizeof SrvAddr);

	//�������
	listen(SrvSock, 5);
	//int listen(SOCKET <���������� �����, ������� �� ��������� �� ���������� ������>,
	// int <������������ ���������� ���������, ����������� � �����������>)

	printf("...������ ��������...\n");
	int AddrLen = sizeof(ConnAddr);
	int nsize;
	int bytes = 0;
	bool stop = false;
	

	//���� ���������� �������
	while (1)
	{
		Conn = accept(SrvSock, (struct sockaddr*)&ConnAddr, &AddrLen);	 // ������ ����� � ������ ��������
		// ������� ����� � �������� ������ ������� ������ �������
		hThread = CreateThread(
			NULL,              // �������� ������������ �� ��������� 
			0,                 // ������ ����� �� ���������  
			ClientFunc,     // ��������� �� ������� ������������ ������
			&Conn,            // ��������, ������������ ������� ������ 
			0,                 // ����� �������� �� ���������
			&ThreadId);        // ������������ ������������� ������

		if (hThread == NULL)  printf("CreateThread failed.");
	}
	getchar();
	CloseHandle(hThread);
	cout << endl << "����� �� ��������� ����� ���������" << endl;
	return 0;
}
