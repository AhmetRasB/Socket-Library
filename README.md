# Socket Library
Soket Kütüphanesi Programlama Agota 
Öncelikle Soket Programlama Nedir?

Soket Programlama,birbiriyle iletişim kurmak için bir ağdaki iki düğümü bağlamanın bir yoludur.Bir soket düğümü bir IP'deki
belirli bir bağlantı noktasını dinlerken, diğer soket bir bağlantı oluşturmak için diğerine ulaşır. İstemci sunucuya ulaşırken
sunucu dinleyici soketini oluşturur.

1. Soket oluşturma:
int sockfd = soketi ( alan adı, türü, protokolü )

sockfd: soket tanımlayıcısı, dosya tanıtıcısı ( gibi bir tam sayı )
etki alanı: tamsayı, iletişim alanını belirtir. Aynı ana bilgisayardaki işlemler arasındaki iletişim için POSIX standardında 
tanımlandığı gibi AF_ LOCAL kullanıyoruz. IPV4 ile bağlanan farklı ana bilgisayarlardaki işlemler arasında iletişim kurmak 
için IPV6 ile bağlanan işlemler için AF_INET ve AF_I NET 6 kullanıyoruz.
yazın: iletişim türü
SOCK_STREAM: TCP ( güvenilir, bağlantı odaklı )
SOCK_DGRAM: UDP ( güvenilmez, bağlantısız )
protokol: İnternet Protokolü ( IP ) için protokol değeri, 0'dır. Bu, bir paketin IP başlığındaki protokol alanında görünen 
numarayla aynıdır. ( daha fazla ayrıntı için insan protokolleri )
2. Setsockopt:
Bu, dosya tanımlayıcı sockfd tarafından belirtilen soket için seçeneklerin değiştirilmesine yardımcı olur. Bu tamamen isteğe 
bağlıdır, ancak adres ve bağlantı noktasının yeniden kullanılmasına yardımcı olur. Aşağıdaki gibi hataları önler: “ 
kullanımda olan adres ”.

int setsockopt ( int sockfd, int seviyesi, int optname, const void * optval, socklen_t optlen );

3. Bağ:
int bind ( int sockfd, const struct sockaddr * addr, socklen_t addrlen );

Soketin oluşturulmasından sonra, bağlama işlevi soketi addr ( özel veri yapısı ) 'de belirtilen adrese ve bağlantı noktası 
numarasına bağlar. Örnek kodda, sunucuyu localhost'a bağlarız, bu nedenle IP adresini belirtmek için INADDR_ANY kullanırız.

4. Dinleyin:
int listen ( int sockfd, int biriktirme listesi );

Sunucu soketini pasif bir moda sokar ve istemcinin bağlantı kurmak için sunucuya yaklaşmasını bekler. İş yükü, sockfd için 
bekleyen bağlantı kuyruğunun büyüyebileceği maksimum uzunluğu tanımlar. Kuyruk dolduğunda bir bağlantı isteği gelirse, 
istemci ECONNREFUSED göstergesiyle bir hata alabilir.

5. Kabul etmek:
int new_socket = kabul ( int sockfd, struct sockaddr * addr, socklen_t * addrlen );

Dinleme soketi, sockfd için bekleyen bağlantılar kuyruğundaki ilk bağlantı isteğini çıkarır, yeni bir bağlı soket oluşturur 
ve bu sokete atıfta bulunan yeni bir dosya tanımlayıcı döndürür. Bu noktada, istemci ve sunucu arasında bağlantı kurulur ve
 veri aktarmaya hazırdır.

Müşteri Aşamaları
Soket bağlantısı: Sunucunun soket oluşturma ile tam olarak aynı
Bağlan: Connect ( ) sistem çağrısı, dosya tanımlayıcı sockfd tarafından belirtilen soketi addr tarafından belirtilen adrese
 bağlar. Sunucunun adresi ve bağlantı noktası ek olarak belirtilir.
int connect ( int sockfd, construct sockaddr * addr, socklen_t addrlen );

Server.c
// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char* hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket
		= accept(server_fd, (struct sockaddr*)&address,
				(socklen_t*)&addrlen))
		< 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	send(new_socket, hello, strlen(hello), 0);
	printf("Hello message sent\n");

	// closing the connected socket
	close(new_socket);
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
client.c

// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status
		= connect(client_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	send(client_fd, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	valread = read(client_fd, buffer, 1024);
	printf("%s\n", buffer);

	// closing the
	connected socket
	close(client_fd);
	return 0;
}
Derleme:

gcc client.c -o istemci
gcc server.c -o sunucusu
Çıktı:

Müşteri: Merhaba mesajı gönderildi
Sunucudan merhaba
Sunucu: Müşteriden merhaba
Merhaba mesaj gönderildi

Sistem böyle işler ve bizim amacımız bu kütüphaneyi sıfırdan yazmak. 

socket.h header dosyamızı oluşturuyoruz 
// Standart giriş/çıkış fonksiyonları
// Standart kütüphane fonksiyonları
// String işlemleri için fonksiyonlar
// Windows için Winsock başlık dosyası
// Winsock kütüphanesini bağla
// Sistem türleri
// Soket işlemleri için fonksiyonlar
// İnternet adresleri için yapılar
// IP dönüşümleri için fonksiyonlar

Bu temel işlevleri sağlayacak olan başlık dosyalarını dahil ediyoruz 
#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>     
#include <stdlib.h>    
#include <string.h>   

#ifdef _WIN32
#include <winsock2.h> 
#pragma comment(lib, "ws2_32.lib")  
#else
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#endif

#endif

Yukarıdaki kodda, _WIN32 önişlemci direktifiyle Windows işletim sistemi için gerekli başlık dosyalarını ve kütüphaneleri 
dahil ediyoruz.

socket.c src dosyamızı oluşturuyoruz.

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
Yukarıdaki kodda, _WIN32 önişlemci direktifiyle Windows işletim sistemi için gerekli kütüphane ve işlev çağrıları 
yapılmıştır.

main.c src dosyamızı oluşturuyoruz.

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
Socket kütüphanesini kullanarak basit bir TCP sunucusu başlatan bir uygulamanın temel bir şablonu.

Yukarıdaki kod, "main" fonksiyonunu içeren ve socket kütüphanesini kullanarak bir TCP sunucusu başlatan bir uygulama 
oluşturur. Bu örnek, Windows ve Unix/Linux üzerinde çalışabilir.

Uygulamayı oluşturduktan sonra, kütüphane ve uygulama dosyalarını birleştirerek çalışabilir bir uygulama oluşturmanız 
gerekecektir.

Umarım bu bilgiler yardımcı olur. İyi çalışmalar! Agota.

Bağlantı komutlarınızı, işletim sisteminizin komut satırına yazmanız gerekmektedir. İşletim sistemine bağlı olarak, komut 
satırını farklı şekillerde açabilirsiniz.

Windows için:

Başlat menüsünü açın.
"cmd" veya "Command Prompt" (Komut İstemi) yazarak Komut İstemi uygulamasını bulun ve açın.
Açılan komut istemi penceresine bağlantı komutunuzu yazın ve Enter tuşuna basın.

Linux için:

Terminal uygulamasını açın. Genellikle uygulama başlatıcısında veya sistem menüsünde bulunur.
Terminal penceresine bağlantı komutunuzu yazın ve Enter tuşuna basın.

Bu komutları ilgili işletim sistemi komut istemine doğru şekilde yazdıktan sonra, sunucuyla bağlantı kurulacak ve sunucuyla 
iletişim sağlayabileceksiniz.
