#include "socket.h"


int createSocket() {
    int sockfd;
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Hata: Winsock başlatılamadı");
        exit(EXIT_FAILURE);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
#else
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif

    if (sockfd == -1) {
        perror("Hata: Soket oluşturulamadı");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void bindSocket(int sockfd, int port) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Hata: Soket bağlanamadı");
        exit(EXIT_FAILURE);
    }
}

void listenSocket(int sockfd, int backlog) {
    if (listen(sockfd, backlog) == -1) {
        perror("Hata: Soket dinlenemiyor");
        exit(EXIT_FAILURE);
    }
}

int acceptConnection(int sockfd) {
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    int clientfd = accept(sockfd, (struct sockaddr*)&addr, &addrlen);
    if (clientfd == -1) {
        perror("Hata: Bağlantı kabul edilemedi");
        exit(EXIT_FAILURE);
    }
    return clientfd;
}

void sendAll(int sockfd, const char* data, int length) {
    int total = 0;
    int bytesleft = length;
    int n;

    while (total < length) {
        n = send(sockfd, data + total, bytesleft, 0);
        if (n == -1) {
            perror("Hata: Veri gönderilemedi");
            exit(EXIT_FAILURE);
        }
        total += n;
        bytesleft -= n;
    }
}

void receiveAll(int sockfd, char* buffer, int length) {
    int total = 0;
    int bytesleft = length;
    int n;

    while (total < length) {
        n = recv(sockfd, buffer + total, bytesleft, 0);
        if (n == -1) {
            perror("Hata: Veri alınamadı");
            exit(EXIT_FAILURE);
        }
        total += n;
        bytesleft -= n;
    }
}
