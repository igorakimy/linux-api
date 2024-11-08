#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include "read_line.h"
#include "../lib/tlpi_hdr.h"

/* Номер порта для сервера */
#define PORT_NUM "50000"

/* Размер строки, достаточный для хранения наибольшего целого числа
   (включая завершающий символ '\n') */
#define INT_LEN  30
