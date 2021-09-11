#include <iostream>
#include <winsock2.h>
#include <string.h>
#include <algorithm>

using namespace std;

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in myAddr, clientAddr;
    int myAddrLen, clientAddrLen;
    int err;
    char recvData[101] = {0, };
    char sendData[101] = {0, };

    // Call WIN SOCKET API
    err = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (err != NO_ERROR) {
        cout << "WSAStartup failed, error code : " << err << endl;
        return -1;
    }

    // Create a socket to connect to just one client
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "Failed to create a socket" << endl;
        WSACleanup();
        return -1;
    }
    
    cout << "Looking for a client..." << endl;

    // Binding
    memset(myAddr.sin_zero, 0, sizeof(myAddr.sin_zero));
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddr.sin_port = htons(51234);
    myAddrLen = sizeof(myAddr);

    err = bind(s, (struct sockaddr *) &myAddr, myAddrLen);
    if (err == SOCKET_ERROR) {
        cout << "Failed to bind a socket" << endl;
        goto socketError;
    }

    // Listening
    err = listen(s, SOMAXCONN);
    if (err == SOCKET_ERROR) {
        cout << "Failed to listen a socket" << endl;
        goto socketError;
    }

    // Accept (handshaking)
    clientSocket = accept(s, (struct sockaddr *) &clientAddr, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Failed to proceed with TCP handshake" << endl;
        goto socketError;
    }

    cout << "Find a client." << endl;

    /////////////////
    while(1) {
        recv(clientSocket, recvData, 100, 0);
        if (!strcmp(recvData, ""))
            break;
        cout << recvData << endl;
        cin.getline(sendData, 100);
        send(clientSocket, sendData, strlen(sendData), 0);
    }
    /////////////////

    cout << "END" << endl;
    shutdown(clientSocket, SD_SEND);
    shutdown(s, SD_SEND);
    closesocket(clientSocket);
    closesocket(s);
    
    WSACleanup();
    return 0;

    socketError:
        closesocket(s);
        WSACleanup();
        return -1;
}
