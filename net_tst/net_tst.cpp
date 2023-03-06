// net_tst.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main() {
	//Определяем версию сокетов ОС
	WSADATA wsData;
	//запуск сокетов
	int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (erStat != 0)
	{
		std::cout << "Error WinSock initialization #";
		std::cout << WSAGetLastError();
		return 1;
	}
	else
		std::cout << "WinSock initialization is Ok" << std::endl;
	//создаем серверный сокет
	SOCKET ServSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (ServSock <= 0)
	{
		std::cout << "Error create socket \n";
		return false;
	}

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(127<<24|1);
	address.sin_port = htons((unsigned short)1111);

	if (bind(ServSock, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
	{
		std::cout << "failed to bind socket\n";
		return false;
	}
	// перевод сокета в неблокирующий режим
	DWORD nonBlocking = 1;
	if (ioctlsocket(ServSock, FIONBIO, &nonBlocking) != 0)
	{
		std::cout << "failed to set non blocking socket\n";
		return  false;
	}
	return 0;
}
