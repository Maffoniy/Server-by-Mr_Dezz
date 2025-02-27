#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

class Server {
private:
    in_addr ip_to_num;
    int PORT_NUM;                           // Enter Open working server port
    const short BUFF_SIZE = 1024;           // Maximum size of buffer for exchange info between server and client
    SOCKET ServSock;                        // Server socket initialization
    int erStat;                             // Keeps socket errors status
    bool serverStatus = false;              // Controlls users connection
    std::vector<std::thread> clientThreads; // thread vector for each user

public:
    Server(int port);
    ~Server();

    //Start connecting clients, create thread for each
    void Run();
    //Stoping connecting clients, created thread still online
    void Stop();
    //Exchange text data between Server and Client. Disconnection if a client send "xxx"       
    void HandleClient(SOCKET ClientConn);
};