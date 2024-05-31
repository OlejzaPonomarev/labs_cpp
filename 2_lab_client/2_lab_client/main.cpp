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
	setlocale(LC_CTYPE, "Russian"); // включаем русскую кодировку
	SOCKET ClientSock = INVALID_SOCKET;
	WSADATA WSStartData; /* Структура данных библиотеки сокета. */
	CHAR DefaultIPAddr[] = "127.0.0.1"; /* Локальная система. */
	struct sockaddr_in Addr; /* Адрес сокета  */

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult < 0)
	{
		cout << endl << "ошибка  на клиенте" << endl;
		getchar();
		exit(0);
	}

	// Создали сокет	
	ClientSock = socket(AF_INET, SOCK_STREAM, 0);
	// Задаем адрес сервера
	memset(&Addr, 0, sizeof(Addr));
	Addr.sin_family = AF_INET;
	Addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);// адрес для отладки
	Addr.sin_port = htons(SERVER_PORT);

	// HOSTENT* hst ;
	char hostname[] = "                ";
	DWORD size = 20;
	char* localIP;

	//Соединяемся с сервером
	int u;
	u = connect(ClientSock, (struct sockaddr*)&Addr, sizeof(Addr));
	if (u == INVALID_SOCKET)
	{
		cout << "Не соединились ";
		getchar();
		return 0;
	}
	else cout << "Cоединились!\n ";
	int bytes = 0;
	char rez[35] = "";
	char buf[20] = "начало";
	int len;

	char exit[5] = "exit";
	bool stop_word_flag = false;
	int len_stop_word = 5;

	while (!stop_word_flag) {
		cout << "input: ";
		cin >> buf;
		len = sizeof(buf);

		//Проверка стоп слова
		int count = 0;
		for (int i = 0; i < 5; ++i) {
			if (buf[i] == exit[i]) {
				count += 1;
			}
		}

		//Послали сообщение
		send(ClientSock, (char*)buf, len, 0);

		if (count == 5) {
			stop_word_flag = true;
			break;
		}

		//Получили сообщение
		bytes = recv(ClientSock, (char*)rez, sizeof(rez), 0);

		if (bytes == -1) {
			cout << "Результат с ошибкой";
		}
		else {
			cout << rez;
		}
	}
	shutdown(ClientSock, 2); /* Запретить посылку и прием сообщений. */
	closesocket(ClientSock);//закрыть сокет
	WSACleanup();//закрыть библиотеку
	cout << endl << "Выход из клиентской части программы" << endl;
	getchar(); getchar();
	return 0;
}

