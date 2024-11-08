#include <netdb.h>
#include "is_seqnum.h"

int
main(int argc, char** argv) {
    char* req_len_str;         /* Длина запрашиваемой последовательности */
    char seq_num_str[INT_LEN]; /* Начало выделенной последовательности */
    int cfd;
    size_t num_read;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s server-host [sequence-len]\n", argv[0]);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC; /* Поддержка IPv4 или IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    /* Вызываем getaddrinfo(), чтобы получить список адресов,
       к которым можем попытаться привязать наш сокет */
    if (getaddrinfo(argv[1], PORT_NUM, &hints, &result) != 0) {
        errExit("getaddrinfo");
    }

    /* Перебираем полученный список, пока не находим структуру с адресом,
       подходящую для успешного соединения с сокетом */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (cfd == -1) {
            /* В случае ошибки пробуем следующий адрес */
            continue;
        }

        if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            /* При успешном соединении прерываем цикл */
            break;
        }

        /* Сбой соединения: закрываем этот сокет и пробуем следующий адрес */
        close(cfd);
    }

    if (rp == NULL) {
        fatal("Could not connect socket to any address");
    }

    freeaddrinfo(result);

    /* Отправляем длину запрашиваемой последовательности с символом новой строки в конце */
    req_len_str = (argc > 2) ? argv[2] : "1";
    if (write(cfd, req_len_str, strlen(req_len_str)) != strlen(req_len_str)) {
        fatal("Partial/failed write (req_len_str)");
    }
    if (write(cfd, "\n", 1) != 1) {
        fatal("Partial/failed write (newline)");
    }

    /* Считываем и выводим число, полученное от сервера */
    num_read = readLine(cfd, seq_num_str, INT_LEN);
    if (num_read == -1) {
        errExit("readLine");
    }
    if (num_read == 0) {
        fatal("Unexpected EOF from server");
    }

    printf("Sequence number: %s", seq_num_str); /* Включает в себя '\n' */

    /* Закрываем сокет */
    exit(EXIT_SUCCESS);
}
