#include <errno.h>
#include "read_line.h"

size_t
readLine(int fd, void* buffer, size_t n) {
    ssize_t numRead; /* Сколько байтов было прочитано */
    size_t totRead; /* Общее количество прочитанных байтов на этот момент */
    char* buf;
    char ch;

    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* Арифметика указателей не поддерживается для "void *" */
    buf = buffer;

    totRead = 0;
    for (;;) {
        numRead = read(fd, &ch, 1);

        if (numRead == -1) {
            if (errno == EINTR) { /* Прерывание --> перезапускаем read() */
                continue;
            } else {
                return -1; /* Какая-то другая ошибка */
            }
        } else if (numRead == 0) { /* Конец файла */
            if (totRead == 0) {    /* Ничего не прочитано; возвращаем 0 */
                return 0;
            } else {
                /* Прочитано какое-то количество байтов; добавляем '\0' */
                break;
            }
        } else {                   /* На данном этапе 'numRead' должно быть равно 1 */
            if (totRead < n - 1) { /* Отклоняем лишние байты: > (n - 1) */
                totRead++;
                *buf++ = ch;
            }
            if (ch == '\n') {
                break;
            }
        }
    }

    *buf = '\0';
    return totRead;
}
