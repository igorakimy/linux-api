#define _DEFAULT_SOURCE

/* Получаем определения NI_MAXHOST и NI_MAXSERV из файла <netdb.h> */
#include <netdb.h>
#include "is_seqnum.h"

#define BACKLOG      50
#define ADDR_STR_LEN (NI_MAXHOST + NI_MAXSERV + 10)

int
main(int argc, char** argv) {
    uint32_t seq_num;
    char req_len_str[INT_LEN]; /* Длина запрашиваемой последовательности */
    char seq_num_str[INT_LEN]; /* Начало выделенной последовательности */
    struct sockaddr_storage cl_addr;
    int lfd, cfd, opt_val, req_len;
    socklen_t addr_len;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    char addr_str[ADDR_STR_LEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        usageErr("%s [init-seq-num]\n", argv[0]);
    }

    /* Начинаем последовательность либо с 1, либо с числа, переданного в виде аргумента командной строки */
    seq_num = (argc > 1) ? getInt(argv[1], 0, "init-seq-num") : 0;

    /* Игнорирует сигнал SIGPIPE, возникающий, когда сервер пытается выполнить запись в сокет,
       другой конец которого был закрыт; вместо этого операция write() завершается ошибкой EPIPE */
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        errExit("signal");
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC; /* Поддержка IPv4 или IPv6 */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV; /* Универсальный IP-адрес */

    /* Вызываем getaddrinfo(), чтобы получить список адресов, подключенных к TCP-порту PORT_NUM */
    if (getaddrinfo(NULL, PORT_NUM, &hints, &result) != 0) {
        errExit("getaddrinfo");
    }

    /* Перебираем полученный список, пока не находим структуру с адресов,
       подходящую для создания и привязывания сокета */
    opt_val = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (lfd == -1) {
            /* В случае ошибки пробуем следующий адрес */
            continue;
        }
        /* Устанавливаем параметр SO_REUSEADDR для слушающего сокета */
        if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1) {
            errExit("setsockopt");
        }
        if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            /* Если адрес, с помощью которого можно успешно создать и привязать сокет найден, то завершить цикл */
            break;
        }
        /* Вызов bind() завершился неудачно; закрываем этот сокет и пробуем следующий адрес */
        close(lfd);
    }

    if (rp == NULL) {
        fatal("Could not bind socket to any address");
    }

    /* Делаем сокет слушающим */
    if (listen(lfd, BACKLOG) == -1) {
        errExit("listen");
    }

    freeaddrinfo(result);

    /* Обслуживаем клиентов итерационно (сервер не принимает очередной клиентский запрос,
       пока не закончит обслуживание текущего) */
    for (;;) {
        addr_len = sizeof(struct sockaddr_storage);
        /* Принимаем соединение со стороны клиента и получаем его адрес */
        cfd = accept(lfd, (struct sockaddr*)&cl_addr, &addr_len);
        if (cfd == -1) {
            errMsg("accept");
            continue;
        }

        /* Вывести информацию о клиентском адресе */
        if (getnameinfo((struct sockaddr*)&cl_addr, addr_len,
                        host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
            snprintf(addr_str, ADDR_STR_LEN, "(%s, %s)", host, service);
        } else {
            snprintf(addr_str, ADDR_STR_LEN, "(?UNKNOWN?)");
        }
        printf("Connection from %s\n", addr_str);

        /* Считываем запрос клиента, возвращаем в ответ элемент последовательности */
        if (readLine(cfd, req_len_str, INT_LEN) <= 0) {
            close(cfd);
            /* Ошибка при чтении; пропускаем запрос */
            continue;
        }

        req_len = atoi(req_len_str);
        /* Отслеживаем клиентов, которые ведут себя некорректно */
        if (req_len <= 0) {
            close(cfd);
            /* Некорректный запрос, пропускаем его */
            continue;
        }

        /* Возвращаем клиенту текущий элемент последовательности (seq_num) в виде строки
           с нулевым символом в конце */
        snprintf(seq_num_str, INT_LEN, "%d\n", seq_num);
        if (write(cfd, seq_num_str, strlen(seq_num_str)) != strlen(seq_num_str)) {
            fprintf(stderr, "Error on write");
        }

        /* Обновляем текущий элемент последовательности */
        seq_num += req_len;

        /* Закрываем соединение */
        if (close(cfd) == -1) {
            errMsg("close");
        }
    }
}