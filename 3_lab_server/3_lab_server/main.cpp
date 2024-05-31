using namespace std;

#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")// подключили библиотеку
#define SERVER_PORT 3820

DWORD WINAPI ClientFunc(LPVOID lpParam) // функция, которую потом будем запускать в отдельном потоке
{
	SOCKET Conn = *(SOCKET*)lpParam; // привели параметр к нужному типу 

	bool stop_word_flag = false;
	int len_stop_word = 5;
	char buf_in[20];
	char exit[5] = "exit";
	char buf_out[35] = "Привет от сервера!\n";

	cout << "Клиент подключился\n";

	while (!stop_word_flag) {
		int bytes = recv(Conn, (char*)buf_in, sizeof(buf_in), 0); //принял информацию в буфер

		//Проверяем стоп слово
		int count = 0;
		for (int i = 0; i < 5; ++i) {
			if (buf_in[i] == exit[i]) {
				count += 1;
			}
		}

		if (count != 5) {
			cout << buf_in << endl;
			send(Conn, (char*)buf_out, sizeof(buf_out), 0);// отправил "привет"
		}
		else {
			cout << "Клиент отправил кодовое слово!\n";
			stop_word_flag = true;
		}
	}
	shutdown(Conn, 2); // запретили передачу и прием сообщений
	closesocket(Conn); //закрыли=разрушили сокет
	return 0;
}

int main()
{
	DWORD ThreadId;
	INT ThreadParameter = 19;
	HANDLE hThread;
	setlocale(LC_CTYPE, "Russian");	//Включаем русскую кодировку
	struct sockaddr_in SrvAddr;     // Адресная структура сервера
	struct sockaddr_in ConnAddr;  // Адресная структура клиента
	SOCKET SrvSock, Conn;	//Создаем сокеты

	WSADATA wsaData;  // для работы Windows с сокетами
	//Структура WSADATA содержит сведения о реализации сокетов Windows.

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // запустили библиотеку
	//Функция WSAStartup инициирует использование winsock DLL процессом.
	//MAKEWORD(2,2) - указание диапазона версий реализации сокетов
	//Далее идет указатель на структуру, хранящую текущую версию реализации сокетов

	if (iResult < 0)	//В случае успеха iResult = 0
	{
		cout << "Ошибка!\n" << endl;
		getchar();
		exit(0);
	}


	SrvSock = socket(AF_INET, SOCK_STREAM, 0); //Создаем  сокет сервера
	//SOCKET socket(int <семейство используемых адресов>, int <тип сокета>, int <тип протокола>)
	//IPv4 - AF_INET, TCP (SOCK_STREAM) или UDP (SOCK_DGRAM), 0 - значение типа протокола

	//Задаем адрес сервера
	SrvAddr.sin_family = AF_INET;	//Семейство адресов
	SrvAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);// 127.0.0.1
	SrvAddr.sin_port = htons(SERVER_PORT);
	//SrvAddr хранит информацию для привязки сокета

	//Настраиваем сокет
	bind(SrvSock, (sockaddr*)&SrvAddr, sizeof SrvAddr);

	//Слушаем
	listen(SrvSock, 5);
	//int listen(SOCKET <«слушающий» сокет, который мы создавали на предыдущих этапах>,
	// int <максимальное количество процессов, разрешенных к подключению>)

	printf("...Сервер работает...\n");
	int AddrLen = sizeof(ConnAddr);
	int nsize;
	int bytes = 0;
	bool stop = false;
	

	//Ждем очередного клиента
	while (1)
	{
		Conn = accept(SrvSock, (struct sockaddr*)&ConnAddr, &AddrLen);	 // сервер завис в режиме ожидания
		// создаем поток с функцией вместо прямого вызова функции
		hThread = CreateThread(
			NULL,              // атрибуты безопасности по умолчанию 
			0,                 // размер стека по умолчанию  
			ClientFunc,     // указатель на функцию создаваемого потока
			&Conn,            // аргумент, передаваемый функции потока 
			0,                 // флаги создания по умолчанию
			&ThreadId);        // возвращаемый идентификатор потока

		if (hThread == NULL)  printf("CreateThread failed.");
	}
	getchar();
	CloseHandle(hThread);
	cout << endl << "Выход из серверной части программы" << endl;
	return 0;
}
