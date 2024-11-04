#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "../lib/tlpi_hdr.h"

#define BUF_SIZE     10 /* Максимальный размер сообщений, которыми обмениваются клиент и сервер */

#define SV_SOCK_PATH "/tmp/ud_ucase"