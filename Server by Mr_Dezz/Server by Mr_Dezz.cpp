// Server part of Server-Client chat. Developed by Mr_Dezz

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include "Server.h"
#pragma comment(lib, "Ws2_32.lib")

using namespace std;


int main(void)
{
	Server server(5544);
	server.Run();
	return 0;

}