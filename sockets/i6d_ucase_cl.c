#include "i6d_ucase.h"

int
main(int argc, char *argv[]) {
    struct sockaddr_in6 svaddr;
    int sfd;
    size_t msg_len;
    ssize_t num_bytes;
    char resp[BUF_SIZE];

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s host-address msg...\n", argv[0]);
    }

    /* Создать клиентский сокет */
    sfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sfd == -1) {
        errExit("socket");
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_in6));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_port = htons(PORT_NUM);
    if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0) {
        errExit("inet_pton failed for address '%s'", argv[1]);
    }

    for (int j = 2; j < argc; ++j) {
        msg_len = strlen(argv[j]);
        if (sendto(sfd, argv[j], msg_len, 0, (struct sockaddr *) &svaddr,
                   sizeof(struct sockaddr_in6)) != msg_len) {
            fatal("sendto");
        }

        num_bytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
        if (num_bytes == -1) {
            errExit("recvfrom");
        }

        printf("Response %d: %.*s\n", j - 1, (int) num_bytes, resp);
    }

    exit(EXIT_SUCCESS);
}
