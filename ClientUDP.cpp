#include <iostream>
#include <WinSock2.h>
#include <fstream>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

//void chat();

int main()
{
    SOCKET clientsock;
    sockaddr_in serveraddr;
    sockaddr_in clientaddr;
    
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "init error" << std::endl;
        system("pause");
        return 1;
    }
    
    clientsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (clientsock == INVALID_SOCKET) {

        std::cout << "invalid socket" << std::endl;
        system("pause");
        return 2;

    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(5555);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int servsize = sizeof(serveraddr);

    const int bufsize = 512;
    char buffer[bufsize];

    std::cout << "enter name of file" << std::endl;
    std::cin >> buffer;

    //if (buffer == "end") break;

    char nameoffile[bufsize];
    strcpy(nameoffile, buffer);

    if (sendto(clientsock, buffer, bufsize, NULL, (sockaddr*)&serveraddr, servsize) == SOCKET_ERROR) {
        std::cout << "send error " << WSAGetLastError() << std::endl;
        closesocket(clientsock);
        WSACleanup();
        system("pause");
        return 3;
    }

    std::ofstream file;
    file.open(nameoffile, std::ios::binary);
    
    int recvsize = 0;

    while (true) {
        recvsize = recvfrom(clientsock, buffer, bufsize, NULL, (sockaddr*)&serveraddr, &servsize);

        if (recvsize == SOCKET_ERROR) {
            std::cout << "recieve error" << std::endl;
            file.close();
            closesocket(clientsock);
            WSACleanup();
            break;
        }

        if (file.bad() == true) {
            std::cout << "file not exist on server" << std::endl;
            file.close();
            remove(nameoffile);
            break;
        }

        file.write(buffer, sizeof(char));
        if (file.tellp() == file.eof()) break;
    }

    std::cout << "file recieved" << std::endl;
    file.close();
    closesocket(clientsock);
    WSACleanup();
    system("pause");
    return 0;
}

//void chat() {
//
//    int s = sizeof(sockaddr);
//    char ans[256];
//    memset(ans, 0, sizeof(ans));
//
//    while (true) {
//        
//        int alen = recvfrom(clientsock, ans, sizeof(ans), NULL, (sockaddr*)&serveraddr, &s);
//
//        if (alen == SOCKET_ERROR) {
//            std::cout << "recieve error " << WSAGetLastError() << std::endl;
//            system("pause");
//            return;
//        }
//
//        std::cout << ans << std::endl;
//
//    }
//
//}‪
/*int s = sizeof(sockaddr);
    char ans[256];
    memset(ans, 0, sizeof(ans));*/

/*while (true) {

        int alen = recvfrom(clientsock, ans, sizeof(ans), NULL, (sockaddr*)&serveraddr, &s);

        if (alen == SOCKET_ERROR) {
            std::cout << "recieve error " << WSAGetLastError() << std::endl;
            system("pause");
            return 3;
        }

        std::cout << ans << std::endl;

    }*/
