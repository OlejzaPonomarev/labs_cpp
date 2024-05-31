using namespace std;

#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#define  SERVER_PORT 3820
using namespace std;
int main()
{
	setlocale(LC_CTYPE, "Russian"); // �������� ������� ���������
	SOCKET ClientSock = INVALID_SOCKET;
	WSADATA WSStartData; /* ��������� ������ ���������� ������. */
	CHAR DefaultIPAddr[] = "127.0.0.1"; /* ��������� �������. */
	struct sockaddr_in Addr; /* ����� ������  */

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult < 0)
	{
		cout << endl << "������  �� �������" << endl;
		getchar();
		exit(0);
	}

	// ������� �����	
	ClientSock = socket(AF_INET, SOCK_STREAM, 0);
	// ������ ����� �������
	memset(&Addr, 0, sizeof(Addr));
	Addr.sin_family = AF_INET;
	Addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);// ����� ��� �������
	Addr.sin_port = htons(SERVER_PORT);

	// HOSTENT* hst ;
	char hostname[] = "                ";
	DWORD size = 20;
	char* localIP;

	//����������� � ��������
	int u;
	u = connect(ClientSock, (struct sockaddr*)&Addr, sizeof(Addr));
	if (u == INVALID_SOCKET)
	{
		cout << "�� ����������� ";
		getchar();
		return 0;
	}
	else cout << "C����������!\n ";
	int bytes = 0;
	char rez[35] = "";
	char buf[20] = "������";
	int len;

	char exit[5] = "exit";
	bool stop_word_flag = false;
	int len_stop_word = 5;

	while (!stop_word_flag) {
		cout << "input: ";
		cin >> buf;
		len = sizeof(buf);

		//�������� ���� �����
		int count = 0;
		for (int i = 0; i < 5; ++i) {
			if (buf[i] == exit[i]) {
				count += 1;
			}
		}

		//������� ���������
		send(ClientSock, (char*)buf, len, 0);

		if (count == 5) {
			stop_word_flag = true;
			break;
		}

		//�������� ���������
		bytes = recv(ClientSock, (char*)rez, sizeof(rez), 0);

		if (bytes == -1) {
			cout << "��������� � �������";
		}
		else {
			cout << rez;
		}
	}
	shutdown(ClientSock, 2); /* ��������� ������� � ����� ���������. */
	closesocket(ClientSock);//������� �����
	WSACleanup();//������� ����������
	cout << endl << "����� �� ���������� ����� ���������" << endl;
	getchar(); getchar();
	return 0;
}

