// net_tst.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")
std::string int_to_string_ip(int ip)
{
	return std::to_string((ip >> 24) & 0xff) + "." + \
		std::to_string((ip >> 16) & 0xff) + "." + \
		std::to_string((ip >> 8) & 0xff) + "." + \
		std::to_string((ip >> 0) & 0xff) + ".";
}
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
	bool go = true;
	std::string cmd = "";
	while (go)
	{
		std::cout << "enter cmd (rx, tx, ex): ";
		std::cin >> cmd;
		if (cmd == "tx")
		{
			std::string tx_data = "";
			std::cout << "enter a message: ";
			std::cin >> tx_data;
			int sent_bytes = sendto(ServSock, tx_data.c_str(), tx_data.size(), 0,
				(sockaddr*)&address, sizeof(sockaddr_in));
			std::cout << "bytes send: " << sent_bytes << std::endl;
		}
		else if (cmd == "rx")
		{
			sockaddr_in tx_socket;
			int tx_socket_length = sizeof(tx_socket);
			char rx_buf[256];
			int max_size_rx = sizeof(rx_buf);
			int rx_bytes = recvfrom(ServSock, (char*)rx_buf, max_size_rx, 0,
				(sockaddr*)&tx_socket, &tx_socket_length);
			std::cout << "recive bytes: " << rx_bytes << std::endl;
			if (rx_bytes > 0)
			{
				std::cout << "recive from ip: " << int_to_string_ip(htonl(tx_socket.sin_addr.s_addr)) << std::endl;
				std::cout << "port: " << htons(tx_socket.sin_port) << std::endl;
				printf("data:\n");
				for (int i = 0; i < rx_bytes; i++)
				{
					std::cout << rx_buf[i];
				}
				printf("\n");
			}
		}
		else if (cmd == "ex")
		{
			go = false;
		}
	}
	closesocket(ServSock);
}
