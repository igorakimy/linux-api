#include "../lib/tlpi_hdr.h"

int
main(const int argc, char *argv[]) {
    for (size_t i = 0; i < argc; ++i) {
        printf("argv[%lld] = %s\n", i, argv[i]);
    }

    /* Или */

    for (char **p = argv; *p != NULL; ++p) {
        puts(*p);
    }

    exit(EXIT_SUCCESS);
}