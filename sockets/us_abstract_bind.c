#include <sys/un.h>
#include <sys/socket.h>
#include "../lib/tlpi_hdr.h"

int
main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_un addr;
    char *str;

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    /* Бит addr.sun_path[0] уже был обнулен вызовом memset() выше */
    /* Абстрактное имя "xyz", за которым следуют нулевые байты */
    strncpy(&addr.sun_path[1], "xyz", sizeof(addr.sun_path) - 2);

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        errExit("socket");
    }

    if (bind(sockfd, (struct sockaddr *) &addr,
            sizeof(sa_family_t) + strlen(str) + 1) == -1) {
        errExit("bind");
    }

    sleep(60);

    exit(EXIT_SUCCESS);
}