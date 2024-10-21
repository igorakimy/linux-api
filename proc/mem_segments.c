#include <stdio.h>
#include <stdlib.h>

char globBuf[65536];           /* Сегмент неинициализированных данных */
int primes[] = {2, 3, 5, 7};   /* Сегмент инициализированных данных */

static int
square(int x) {            /* Размещается в фрейме для square() */
    int result;            /* Размещается в фрейме для square() */
    result = x * x;
    return result;         /* Возвращаемое значение передается через регистр */
}

static void
do_calc(int val) {             /* Размещается в фрейме для do_calc() */
    printf("The square of %d is %d", val, square(val));
    if (val < 1000) {
        int t;                 /* Размещается в фрейме для do_calc() */
        t = val * val * val;
        printf("The cube of %d is %d", val, t);
    }
}

int
main(int argc, char *argv[]) {    /* Размещается в фрейме для main() */
    static int key = 9973;        /* Сегмент инициализированных данных */
    static char mbuf[10240000];   /* Сегмент неинициализированных данных */
    char* p;                      /* Размещается в фрейме для main() */
    p = malloc(1024);         /* Указывает на память в сегменте кучи */
    do_calc(key);
    free(p);
    exit(EXIT_SUCCESS);
}
