#pragma once

#define WIN32_LEAN_AND_MEAN

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
#include <ws2def.h>


#include <iostream>
#include <string>


std::string wsa_error() {
    int code = WSAGetLastError();

    switch (code) {
        case WSAEINTR: {
            return "WSAEINTR";
        }
        case WSAEBADF: {
            return "WSAEBADF";
        }
        case WSAEACCES: {
            return "WSAEACCES";
        }
        case WSAEDISCON: {
            return "WSAEDISCON";
        }
        case WSAEFAULT: {
            return "WSAEFAULT";
        }
        case WSAEINVAL: {
            return "WSAEINVAL";
        }
        case WSAEMFILE: {
            return "WSAEMFILE";
        }
        case WSAEWOULDBLOCK: {
            return "WSAEWOULDBLOCK";
        }
        case WSAEINPROGRESS: {
            return "WSAEINPROGRESS";
        }
        case WSAEALREADY: {
            return "WSAEALREADY";
        }
        case WSAENOTSOCK: {
            return "WSAENOTSOCK";
        }
        case WSAEDESTADDRREQ: {
            return "WSAEDESTADDRREQ";
        }
        case WSAEMSGSIZE: {
            return "WSAEMSGSIZE";
        }
        case WSAEPROTOTYPE: {
            return "WSAEPROTOTYPE";
        }
        case WSAENOPROTOOPT: {
            return "WSAENOPROTOOPT";
        }
        case WSAEPROTONOSUPPORT: {
            return "WSAEPROTONOSUPPORT";
        }
        case WSAESOCKTNOSUPPORT: {
            return "WSAESOCKTNOSUPPORT";
        }
        case WSAEOPNOTSUPP: {
            return "WSAEOPNOTSUPP";
        }
        case WSAEPFNOSUPPORT: {
            return "WSAEPFNOSUPPORT";
        }
        case WSAEAFNOSUPPORT: {
            return "WSAEAFNOSUPPORT";
        }
        case WSAEADDRINUSE: {
            return "WSAEADDRINUSE";
        }
        case WSAEADDRNOTAVAIL: {
            return "WSAEADDRNOTAVAIL";
        }
        case WSAENETDOWN: {
            return "WSAENETDOWN";
        }
        case WSAENETUNREACH: {
            return "WSAENETUNREACH";
        }
        case WSAENETRESET: {
            return "WSAENETRESET";
        }
        case WSAECONNABORTED: {
            return "WSWSAECONNABORTEDAEINTR";
        }
        case WSAECONNRESET: {
            return "WSAECONNRESET";
        }
        case WSAENOBUFS: {
            return "WSAENOBUFS";
        }
        case WSAEISCONN: {
            return "WSAEISCONN";
        }
        case WSAENOTCONN: {
            return "WSAENOTCONN";
        }
        case WSAESHUTDOWN: {
            return "WSAESHUTDOWN";
        }
        case WSAETOOMANYREFS: {
            return "WSAETOOMANYREFS";
        }
        case WSAETIMEDOUT: {
            return "WSAETIMEDOUT";
        }
        case WSAECONNREFUSED: {
            return "WSAECONNREFUSED";
        }
        case WSAELOOP: {
            return "WSAELOOP";
        }
        case WSAENAMETOOLONG: {
            return "WSAENAMETOOLONG";
        }
        case WSAEHOSTDOWN: {
            return "WSAEHOSTDOWN";
        }
        case WSASYSNOTREADY: {
            return "WSASYSNOTREADY";
        }
        case WSAVERNOTSUPPORTED: {
            return "WSAVERNOTSUPPORTED";
        }
        case WSANOTINITIALISED: {
            return "WSANOTINITIALISED";
        }
        case WSAHOST_NOT_FOUND: {
            return "WSAHOST_NOT_FOUND";
        }
        case WSATRY_AGAIN: {
            return "WSATRY_AGAIN";
        }
        case WSANO_RECOVERY: {
            return "WSANO_RECOVERY";
        }
        case WSANO_DATA: {
            return "WSANO_DATA";
        }
        default: {
            return "NO ERROR";
        }
    }
}

int main() {
    // Startup winsock
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wsOk = WSAStartup(version, &data);

    if (wsOk != 0) {
        std::cerr << "Can't start winsock! " << std::endl;
        wsa_error();
    }

    // Bind socket to ip address and port
    SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);// SOCK_DGRAM - UDP socket

    // Create a hint structure for the client
    sockaddr_in server_hint{};
    server_hint.sin_addr.S_un.S_addr = ADDR_ANY;// Bind server to all interfaces - not just "localhost"
    server_hint.sin_family = AF_INET;           // IPv4
    server_hint.sin_port = htons(54000);        // Convert from little to big endian

    if (bind(in, (sockaddr *) &server_hint, sizeof(server_hint)) == SOCKET_ERROR) {
        std::cerr << "Can't bind socket! " << std::endl;
        wsa_error();
    }

    // Connection meta data
    sockaddr_in client{};
    int client_length = sizeof(client);
    ZeroMemory(&client, client_length);

    // Actual data received from client
    char buffer[1024];

    // Enter loop
    while (true) {
        ZeroMemory(buffer, 1024);// Cleanup buffer

        // Wait for message
        int incoming_bytes = recvfrom(in, buffer, 1024, 0, (sockaddr *) &client, &client_length);// Last 2 parameters is unnecessary

        if (incoming_bytes == SOCKET_ERROR) {
            std::cerr << "Error receiving from client " << std::endl;
            wsa_error();
            continue;
        }

        // Display message
        char client_ip[256];
        ZeroMemory(&client_ip, 256);

        inet_ntop(AF_INET, &client.sin_addr, client_ip, 256);// Convert clinet_ip into string

        std::cout << "Message received from " << client_ip << " : " << buffer << std::endl;
    }

    // Close socket
    closesocket(in);

    // Shutdown winsock
    WSACleanup();

    return 0;
}