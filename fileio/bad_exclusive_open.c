#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[]) {
    int fd;

    // Открытие файла. Проверка существования файла.
    fd = open(argv[1], O_WRONLY);

    // Открытие прошло успешно.
    if (fd != -1) {
        printf("[PID %ld] File \"%s\" already exists\n", (long) getpid(), argv[1]);
        close(fd);
    } else {
        // Сбой по неожиданной причине.
        if (errno != ENOENT) {
            errExit("open");
        } else {
            // Отрезок времени на сбой
            printf("[PID %ld] File \"%s\" doesn't exist yet\n", (long) getpid(), argv[1]);
            if (argc > 2) {
                // Приостановить текущий выполняемый процесс на N секунд.
                sleep(5);
                printf("[PID %ld] Done sleeping\n", (long) getpid());
            }

            fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if (fd == -1) {
                errExit("open");
                // Может быть ложью.
                printf("[PID %ld] Created file \"%s\" exclusively\n",
                       (long) getpid(), argv[1]);
            }
        }
    }
}
