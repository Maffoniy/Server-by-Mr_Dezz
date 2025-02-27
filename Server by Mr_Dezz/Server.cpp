#include "Server.h"
#include <iostream>
#include <stdio.h>

using namespace std;

const char IP_SERV[] = "127.0.0.1"; // Enter local Server IP address

Server::Server(int port) {
    PORT_NUM = port;

    erStat = inet_pton(AF_INET, IP_SERV, &ip_to_num);
    if (erStat <= 0) {
        cout << "Error in IP translation to special numeric format" << endl;
        return;
    }

    WSADATA wsData;
    erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
    if (erStat != 0) {
        cout << "Error WinSock version initialization #" << WSAGetLastError() << endl;
        return;
    }
    cout << "WinSock initialization is OK" << endl;

    ServSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ServSock == INVALID_SOCKET) {
        cout << "Error initializing socket # " << WSAGetLastError() << endl;
        WSACleanup();
        return;
    }
    cout << "Server socket initialization is OK" << endl;

    sockaddr_in servInfo = {};
    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(PORT_NUM);

    erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));
    if (erStat != 0) {
        cout << "Error binding socket to server info. Error # " << WSAGetLastError() << endl;
        closesocket(ServSock);
        WSACleanup();
        return;
    }
    cout << "Binding socket to Server info is OK" << endl;

    erStat = listen(ServSock, SOMAXCONN);
    if (erStat != 0) {
        cout << "Can't start listening. Error # " << WSAGetLastError() << endl;
        closesocket(ServSock);
        WSACleanup();
        return;
    }
    cout << "Listening..." << endl;
}

Server::~Server() {
    closesocket(ServSock);
    WSACleanup();
}

void Server::Run() {
    sockaddr_in clientInfo;
    ZeroMemory(&clientInfo, sizeof(clientInfo));
    int clientInfo_size = sizeof(clientInfo);
    serverStatus = true;

    while (serverStatus) {
        SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);
        if (ClientConn == INVALID_SOCKET) {
            cout << "Client detected, but can't connect. Error # " << WSAGetLastError() << endl;
            closesocket(ClientConn);
            continue;
        }

        cout << "Client connected" << endl;
        clientThreads.emplace_back([this, ClientConn]() {
            HandleClient(ClientConn);
            });
        clientThreads.back().detach();
    }
}


void Server::Stop() {
    serverStatus = false;
}

void Server::HandleClient(SOCKET ClientConn) {
    vector<char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
    short packet_size = 0;

    while (true) {
        packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
        if (packet_size <= 0) break;

        cout << "Client: " << servBuff.data() << endl;

        if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
            shutdown(ClientConn, SD_BOTH);
            closesocket(ClientConn);
            return;
        }
        int number = 0; 
        for (char ch : servBuff)
            if (isdigit(ch))   number = number * 10 + (ch - '0');

        string response = "No";
        int root = static_cast<int>(sqrt(number));
        if (root * root == number) {
            response = "Yes";
        }
        cout << "Your message: "<< number;

        send(ClientConn, response.c_str(), response.size(), 0);
    }
    closesocket(ClientConn);
}
