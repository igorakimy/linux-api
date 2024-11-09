#define _DEFAULT_SOURCE

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "../lib/tlpi_hdr.h"
#include "inet_sockets.h"

int
inetConnect(const char* host, const char* service, int type) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC; /* Поддерживает IPv4 или IPv6 */
    hints.ai_socktype = type;
    s = getaddrinfo(host, service, &hints, &result);
    if (s != 0) {
        errno = ENOSYS;
        return -1;
    }

    /* Перебираем полученный список, пока не найдем структуру с адресом,
       с помощью которого можно успешно подключиться к сокету */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            /* В случае ошибки пробуем следующий адрес */
            continue;
        }
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            /* Адрес найден; завершаем цикл */
            break;
        }
        /* Ошибка соединения: закрываем этот сокет и пробуем следующий адрес */
        close(sfd);
    }

    freeaddrinfo(result);
    return (rp == NULL) ? -1 : sfd;
}

static int
inetPassiveSocket(const char* service, int type, socklen_t* addrlen, short doListen, int backlog) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, opt_val, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = type;
    hints.ai_family = AF_UNSPEC; /* Поддерживает IPv4 или IPv6 */
    hints.ai_flags = AI_PASSIVE; /* Используем универсальный IP-адрес */
    s = getaddrinfo(NULL, service, &hints, &result);
    if (s != 0) {
        return -1;
    }

    /* Перебираем полученный список, пока не найдем структуру с адресом,
       с помощью которого можно успешно создать и привязать сокет */
    opt_val = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            /* В случае ошибки пробуем следующий адрес */
            continue;
        }

        if (doListen && setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1) {
            close(sfd);
            freeaddrinfo(result);
            return -1;
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            /* Адрес найден; завершаем цикл */
            break;
        }

        /* Сбой в вызове bind(): закрываем этот сокет и пробуем следующий адрес */
        close(sfd);
    }

    if (rp != NULL && addrlen != NULL) {
        *addrlen = rp->ai_addrlen;
    }

    freeaddrinfo(result);
    return (rp == NULL) ? -1 : sfd;
}

int
inetListen(const char* service, int backlog, socklen_t* addrlen) {
    return inetPassiveSocket(service, SOCK_STREAM, addrlen, 1, backlog);
}

int
inetBind(const char* service, int type, socklen_t* addrlen) {
    return inetPassiveSocket(service, type, addrlen, 0, 0);
}

char*
inetAddressStr(const struct sockaddr* addr, socklen_t addrlen, char* addrStr, int addrStrLen) {
    char host[NI_MAXHOST], service[NI_MAXSERV];
    if (getnameinfo(addr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV) == 0) {
        snprintf(addrStr, addrStrLen, "(%s, %s)", host, service);
    } else {
        snprintf(addrStr, addrStrLen, "(?UNKNOW?)");
    }
    /* Не забываем добавить нулевой байт в конце */
    addrStr[addrStrLen - 1] = '\0';
    return addrStr;
}
