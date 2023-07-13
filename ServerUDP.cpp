#include <iostream>
#include <fstream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

//void chat();

//SOCKET serversock;
//sockaddr_in clientaddr;
//sockaddr_in serveraddr;

int main()
{
    SOCKET serversock;
    sockaddr_in clientaddr;
    sockaddr_in serveraddr;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "init error" << std::endl;
        return 1;
    }

    serversock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serversock == INVALID_SOCKET) {
        std::cout << " invalid socket" << std::endl;
        return 2;
    }

    memset(&serveraddr, 0, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(5555);

    if (bind(serversock, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        std::cout << "error bind" << std::endl;
        return 3;
    }

    std::cout << "waiting" << std::endl;
    int slen = sizeof(sockaddr_in);
    const int bufsize = 512;
    char buffer[bufsize];

    if (recvfrom(serversock, buffer, bufsize, NULL, (sockaddr*)&clientaddr, &slen) == SOCKET_ERROR) {
        std::cout << "recieve error" << std::endl;
        closesocket(serversock);
        WSACleanup();
        system("pause");
        return 4;
    }

    //else if (buffer == "end") break;

    else {
        std::ifstream recfile;
        recfile.open(buffer, std::ios::binary);

        std::cout << "file " << buffer << std::endl;

        if (recfile.is_open() == false) {
            std::cout << "could not open file" << std::endl;
            strcpy(buffer, "nofile");
            system("pause");
            return 4;
        }

        if (sendto(serversock, buffer, bufsize, NULL, (sockaddr*)&serveraddr, slen) == SOCKET_ERROR) {
            std::cout << " send error" << WSAGetLastError() << std::endl;
            recfile.close();
            closesocket(serversock);
            WSACleanup();
            system("pause");
            return 4;
        }

        size_t fsize;
        recfile.seekg(0, std::ios::end);
        fsize = recfile.tellg();

        int part = 0;
        recfile.seekg(std::ios::beg);

        while (recfile.eof() == false) {

            recfile.read(buffer, 1);
            if (sendto(serversock, buffer, bufsize, NULL, (sockaddr*)&clientaddr, slen) == SOCKET_ERROR) {
                std::cout << " send error" << WSAGetLastError() << std::endl;
                recfile.close();
                closesocket(serversock);
                WSACleanup();
                break;
            }
            ++part;
            //std::cout << "now send" << part << " of " << fsize << std::endl;
        }


        std::cout << "send is over" << std::endl;
        std::cout << "send " << --part << " of " << fsize << std::endl;

        if (sendto(serversock, "end", 4, NULL, (sockaddr*)&clientaddr, slen) == SOCKET_ERROR) {
            std::cout << " send error" << WSAGetLastError() << std::endl;
            closesocket(serversock);
            WSACleanup();
        }

        /*if (sendto(serversock, buffer, bufsize, NULL, (sockaddr*)&clientaddr, slen) == SOCKET_ERROR) {
            std::cout << " send error" << WSAGetLastError() << std::endl;
            recfile.close();
            closesocket(serversock);
            WSACleanup();
            system("pause");
            return 4;
        }*/
        recfile.close();
    }


    /*char answer[256];
    memset(answer, 0, sizeof(answer));

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)chat, NULL, NULL, NULL);

    while (true) {

        int slen = sizeof(sockaddr_in);

        std::cin.getline(answer, sizeof(answer));

        int sen = sendto(serversock, answer, strlen(answer), NULL, (sockaddr*)&clientaddr, slen);

        if (sen == SOCKET_ERROR) {
            std::cout << "send error" << std::endl;
            return 5;
        }
    }*/

    closesocket(serversock);
    WSACleanup();
    system("pause");
    return 0;
}

//void chat() {
//
//    char message[256];
//    memset(message, 0, sizeof(message));
//
//    int slen = sizeof(sockaddr_in);
//
//    while (true) {
//        int msglen = recvfrom(serversock, message, sizeof(message), NULL, (sockaddr*)&clientaddr, &slen);
//
//        if (msglen == SOCKET_ERROR) {
//
//            std::cout << "recieve error" << std::endl;
//            return;
//
//        }
//
//        std::cout << message << std::endl;
//        ZeroMemory(message, sizeof(message));
//
//        char clientip[128];
//        ZeroMemory(clientip, sizeof(clientip));
//
//        inet_ntop(AF_INET, &clientaddr.sin_addr, clientip, sizeof(clientip));
//        std::cout << "message recieved from " << clientip << std::endl;
//    }
//
//}

