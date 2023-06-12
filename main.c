#include "socket.h"

int main() {
    int sockfd = createSocket();
    bindSocket(sockfd, 8080);
    listenSocket(sockfd, 5);

    printf("Sunucu başlatıldı. Bağlantı bekleniyor...\n");

    int clientfd = acceptConnection(sockfd);
    printf("Yeni bir bağlantı kabul edildi\n");

    // Bağlantı üzerinden veri alışverişi yapabilirsiniz

    // Bağlantıyı kapatın
#ifdef _WIN32
    closesocket(clientfd);
    WSACleanup();
#else
    close(clientfd);
#endif

    return 0;
}
