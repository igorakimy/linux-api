#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include "../lib/tlpi_hdr.h"

#define BUF_SIZE 10     /* Максимальный размер сообщений,
                           которыми обмениваются сервер и клиент */

#define PORT_NUM 50002  /* Номер серверного порта */