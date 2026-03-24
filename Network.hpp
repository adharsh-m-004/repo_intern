    #pragma once
    #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0601
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iostream>
    #include <string>

    #pragma comment(lib, "ws2_32.lib")

    class UDPSocket {
    protected:
        SOCKET sock;
        sockaddr_in address;

    public:
        UDPSocket() : sock(INVALID_SOCKET) {
            WSADATA wsaData;
            WSAStartup(MAKEWORD(2, 2), &wsaData);
        }
        virtual ~UDPSocket() {
            if (sock != INVALID_SOCKET) closesocket(sock);
            WSACleanup();
        }
    };

    class UDPSender : public UDPSocket {
    public:
        UDPSender(const std::string& ip, int port) {
            sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            address.sin_family = AF_INET;
            address.sin_port = htons(port);
            unsigned long addr = inet_addr(ip.c_str());
            if (addr == INADDR_NONE && ip != "255.255.255.255") {
                std::cerr << "UDP sender: invalid IPv4 address: " << ip << std::endl;
            }
            address.sin_addr.s_addr = addr;
        }

        void Send(const void* data, size_t length) {
            sendto(sock, (const char*)data, length, 0, (sockaddr*)&address, sizeof(address));
        }
    };

    class UDPReceiver : public UDPSocket {
    public:
        UDPReceiver(int port) {
            sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            address.sin_family = AF_INET;
            address.sin_port = htons(port);
            address.sin_addr.s_addr = INADDR_ANY;
            bind(sock, (sockaddr*)&address, sizeof(address));
        }

        int Receive(void* buffer, size_t length) {
            sockaddr_in senderAddr;
            int senderAddrSize = sizeof(senderAddr);
            return recvfrom(sock, (char*)buffer, length, 0, (sockaddr*)&senderAddr, &senderAddrSize);
        }
    };