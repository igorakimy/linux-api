#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;

static void
do_jump(int nvar, int rvar, int vvar) {
    printf("Inside do_jump(): nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
    longjmp(env, 1);
}

int
main(int argc, char *argv[]) {
    int nvar;
    register int rvar;          /* По возможности выделяется в регистре */
    volatile int vvar;          /* Не будет оптимизировано компилятором,
                                   т.к. значение может изменяться извне */

    nvar = 111;
    rvar = 222;
    vvar = 333;

    if (setjmp(env) == 0) {     /* Код, выполняемый после setjmp() */
        nvar = 777;
        rvar = 888;
        vvar = 999;
        do_jump(nvar, rvar, vvar);
    } else {                    /* Код, выполняемый после longjmp() */
        printf("After longjmp(): nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
    }

    exit(EXIT_SUCCESS);
}