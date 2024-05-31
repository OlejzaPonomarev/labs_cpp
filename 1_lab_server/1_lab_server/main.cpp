using namespace std;

#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")// подключили библиотеку
#define SERVER_PORT 3820


int main()
{
	setlocale(LC_CTYPE, "Russian"); // включаем русскую кодировку

	struct sockaddr_in SrvAddr;     // Адресная структура сервера

	struct sockaddr_in ConnAddr;  // Адресная структура клиента

	SOCKET SrvSock, Conn;

	WSADATA wsaData;  // для работы Windows с сокетами
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // запустили библиотеку
	if (iResult < 0)
	{
		cout << "ошибка" << endl;
		getchar();
		exit(0);
	}


	SrvSock = socket(AF_INET, SOCK_STREAM, 0);//Создаем  сокет сервера

	//Задаем адрес сервера
	SrvAddr.sin_family = AF_INET;
	SrvAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);// 127.0.0.1
	SrvAddr.sin_port = htons(SERVER_PORT);
	//Настраиваем сокет
	bind(SrvSock, (sockaddr*)&SrvAddr, sizeof SrvAddr);

	//Слушаем
	listen(SrvSock, 5);

	printf("...Сервер работает...\n");
	int AddrLen = sizeof(ConnAddr);
	char buf_in[20];
	char exit[20] = "exit";
	char buf_out[34] = "Привет от сервера!";
	int nsize;
	int bytes = 0;
	bool stop = false;



	//Ждем очередного клиента
	while (1)
	{
		Conn = accept(SrvSock, (struct sockaddr*)&ConnAddr, &AddrLen);	 // сервер завис в режиме ожидания
		//HOSTENT* hst ;
	   //hst = gethostbyaddr((char *)&ConnAddr. sin_addr.s_addr, 4, AF_INET);
	   //cout<<"Подключился " << inet_ntoa(ConnAddr.sin_addr)<<endl;

		bytes = recv(Conn, (char*)buf_in, sizeof(buf_in), 0);//принял информацию в буфер
		cout << buf_in << endl;
		send(Conn, (char*)buf_out, sizeof(buf_out), 0);// отправил "привет"

	}

	shutdown(Conn, 2); // запретили передачу и прием сообщений
	closesocket(Conn); //закрыли=разрушили сокет


	cout << endl << "Выход из серверной части программы" << endl;
	getchar();
	return 0;
}
