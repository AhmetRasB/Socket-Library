#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>     // Standart giriş/çıkış fonksiyonları
#include <stdlib.h>    // Standart kütüphane fonksiyonları
#include <string.h>    // String işlemleri için fonksiyonlar

#ifdef _WIN32
#include <winsock2.h>  // Windows için Winsock başlık dosyası
#pragma comment(lib, "ws2_32.lib")  // Winsock kütüphanesini bağla
#else
#include <sys/types.h>  // Sistem türleri
#include <sys/socket.h> // Soket işlemleri için fonksiyonlar
#include <netinet/in.h> // İnternet adresleri için yapılar
#include <arpa/inet.h>  // IP dönüşümleri için fonksiyonlar
#endif

#endif
