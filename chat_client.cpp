#include <iostream>
#include <winsock2.h>
#include <string.h>
#include <algorithm>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    int err;
    char sendData[101] = {0, };
    char recvData[101] = {0, };

    // Call WIN SOCKET API
    err = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (err != NO_ERROR) {
        cout << "WSAStartup failed, error code : " << err << endl;
        return -1;
    }

    // Create a socket to connect to just one server
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "Failed to create a socket" << endl;
        WSACleanup();
        return -1;
    }

    cout << "Connecting to the server..." << endl;

    // connect
    sockaddr_in server;
    int serverLen;
    memset(server.sin_zero, 0, sizeof(server.sin_zero));
    server.sin_family = AF_INET;
    // use your server IP address
    server.sin_addr.s_addr = inet_addr("x.x.x.x");
    server.sin_port = htons(51234);
    serverLen = sizeof(server);

    err = connect(s, (struct sockaddr *) &server, serverLen);
    if (err == SOCKET_ERROR) {
        cout << "Failed to connect to the server" << endl;
        closesocket(s);
        WSACleanup();
        return -1;
    }

    cout << "You can start chatting." << endl;

    /////////////////
    while (1) {
        cin.getline(sendData, 100);
        if (!strcmp(sendData, "end"))
            break;
        send(s, sendData, strlen(sendData), 0);
        recv(s, recvData, 100, 0);
        cout << recvData << endl;
    }
    /////////////////

    shutdown(s, SD_SEND);
    closesocket(s);
    WSACleanup();
    return 0;
}