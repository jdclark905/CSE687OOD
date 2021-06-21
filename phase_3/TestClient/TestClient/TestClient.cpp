#include <string>
#include <iostream>
#include <WS2tcpip.h>
#include "Message.h"
#pragma comment(lib, "ws2_32.lib")

constexpr auto IP_ADDRESS = "127.0.0.1";
constexpr auto PORT = 10000;

using namespace std;
Message BuildMessage(MsgAddress myAddress, MsgAddress svrAddress, string body) {
    Message msg(myAddress, svrAddress);
    msg.type(MSG_TYPE_TEST_REQ);
    msg.author("Client");
    msg.timestamp(Logger::CurrentTimeStamp());
    msg.body(body);
    return msg; 
}
void main()
{
    string ipAddress = IP_ADDRESS;     // IP Address of the server
    int port = PORT;                   // Listening port # on the server

    // Initializing Winsock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0) 
    {
        cerr << "Can't Start Winsock, Err #" << wsResult << endl;
        return;
    }

    // Create Socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) 
    {
        cerr << "Can't ceate socket, Err #" << WSAGetLastError() << endl;
        WSACleanup();
        return;
    }

    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
    
    // Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    WSAData wsaData;
    int retval = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (retval > 0) Logger::ToConsole("Return value: " + retval);

    Logger::ToConsole("Client Connection Established...");

    struct MsgAddress svrAddress(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT), myAddress(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT + 1);

    ClientSocket client;
    client.connect(svrAddress.IPAddr, svrAddress.Port);


    Message matt = BuildMessage(myAddress, svrAddress, "MattLib.dll");
    Message jamie = BuildMessage(myAddress, svrAddress, "JamieLib.dll");
    Message tanay = BuildMessage(myAddress, svrAddress, "TanayLib.dll");

    client.sendString(matt.toString());
    std::this_thread::sleep_for(std::chrono::seconds(2));
    client.sendString(jamie.toString());
    std::this_thread::sleep_for(std::chrono::seconds(2));
    client.sendString(tanay.toString());
    std::this_thread::sleep_for(std::chrono::seconds(2));


    client.shutDown();
    client.close();
    system("pause");

    // Gracefully close down everything
    closesocket(sock);
    WSACleanup();
}