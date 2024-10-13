#include <stdarg.h>
#include <stdbool.h>
#include "error_functions.h"
#include "tlpi_hdr.h"
// Определяет ename и MAX_ENAME.
#include "ename.c.inc"

#ifdef __GNUC__
__attribute__ ((__noreturn__))
#endif

static void
terminate(bool useExit3) {
    char *s;

    // Сохранить дамп ядра, если переменная среды EF_DUMPCORE определена
    // и содержит непустую строку; в противном случае вызывает exit(3)
    // или _exit(2), в зависимости от значения 'useExit3'.
    s = getenv("EF_DUMPCORE");
    if (s != NULL && *s != '\0') {
        abort();
    } else if (useExit3) {
        exit(EXIT_FAILURE);
    } else {
        _exit(EXIT_FAILURE);
    }
}

static void
outputError(bool useErr, int err, bool flushStdout, const char *format, va_list ap) {
#define BUF_SIZE 500
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];
    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr) {
        const char* errName = (err > 0 && err <= MAX_ENAME) ? ename[err] : "?UNKNOWN?";
        snprintf(errText, BUF_SIZE, " [%s %s]", errName, strerror(err));
    } else {
        snprintf(errText, BUF_SIZE, ":");
    }

    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

    if (flushStdout) {
        // Сброс всего ожидающего стандартного вывода.
        fflush(stdout);
    }

    fputs(buf, stderr);
    // При отсутствии построчной буферизации в stderr.
    fflush(stderr);
}

void
errMsg(const char *format, ...) {
    va_list argList;
    int savedErrno;
    savedErrno = errno;
    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);
    errno = savedErrno;
}

void
errExit(const char *format, ...) {
    va_list argList;
    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);
    terminate(true);
}

void
err_exit(const char *format, ...) {
    va_list argList;
    va_start(argList, format);
    outputError(true, errno, false, format, argList);
    va_end(argList);
    terminate(false);
}

void
errExitEN(int errNum, const char *format, ...) {
    va_list argList;
    va_start(argList, format);
    outputError(true, errNum, true, format, argList);
    va_end(argList);
    terminate(true);
}

void
fatal(const char *format, ...) {
    va_list argList;
    va_start(argList, format);
    outputError(false, 0, true, format, argList);
    va_end(argList);
    terminate(true);
}

void
usageErr(const char *format, ...) {
    va_list argList;
    // Сбросить весь ожидающий стандартный вывод.
    fflush(stdout);
    fprintf(stderr, "Usage: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);
    // При отсутствии построчной буферизации в stderr.
    fflush(stderr);
    exit(EXIT_FAILURE);
}

void
cmdLineErr(const char *format, ...) {
    va_list argList;
    // Сбросить весь ожидающий стандартный вывод.
    fflush(stdout);
    fprintf(stderr, "Command-line usage error: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);
    // При отсутствии построчной буферизации в stderr.
    fflush(stderr);
    exit(EXIT_FAILURE);
}