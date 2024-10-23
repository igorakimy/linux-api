#include "us_xfr.h"

/* Кол-во запросов в очереди на прослушивание сокета */
#define BACKLOG 5

int
main(int argc, char *argv[]) {
    /* Адрес UNIX-сокета */
    struct sockaddr_un addr;
    /* Файловые дескрипторы серверного сокета и клиентского */
    int sfd, cfd;
    /* Кол-во прочитанных байт из клиентского сокета */
    ssize_t numRead;
    /* Буфер для записи в него прочитанных из клиентского сокета данных */
    char buf[BUF_SIZE];

    /* Создать серверный потоковый сокет на локальном UNIX домене */
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        errExit("socket");
    }

    /* Удалить ранее созданный сокетный файл, чтобы не возникло ошибки */
    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        errExit("remove-%s", SV_SOCK_PATH);
    }

    /* Инициализировать структуру адреса UNIX-сокета */
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    /* Связать созданный сокет с конкретным адресом */
    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        errExit("bind");
    }

    /* Слушать конкретный сокет, указав максимальный размер очереди запросов */
    if (listen(sfd, BACKLOG) == -1) {
        errExit("listen");
    }

    /* Запустить бесконечный цикл */
    for (;;) {
        /* Принимать соединение. Оно будет назначено новому сокету, `cfd`; слушающий сокет
           `sfd` остается при этом открытым и может принимать последующие соединения */
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1) {
            errExit("accept");
        }

        /* Читать содержимое клиентского сокета в буфер */
        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
            /* Записать в стандартный поток вывода содержимое буфера */
            if (write(STDOUT_FILENO, buf, numRead) != numRead) {
                fatal("partial/failed write");
            }
        }

        if (numRead == -1) {
            errExit("read");
        }

        /* Закрыть файловый дескриптор клиентского сокета */
        if (close(cfd) == -1) {
            errMsg("close");
        }
    }
}