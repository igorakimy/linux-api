#ifndef INET_SOCKETS_H
#define INET_SOCKETS_H

#include <sys/socket.h>
#include <netdb.h>

int inetConnect(const char* host, const char* service, int type);
int inetListen(const char* service, int backlog, socklen_t* addrlen);
int inetBind(const char* service, int type, socklen_t* addrlen);
char* inetAddressStr(const struct sockaddr* addr, socklen_t addrlen,
    char* addrStr, int  addrStrLen);

/* Рекомендуемая дилна строкового буфера, который нужно передать
   в inetAddressStr(). Должная быть больше чем (NI_MAXHOST + NI_MAXSERV + 4) */
#define IS_ADDR_STR_LEN 4096
#endif
