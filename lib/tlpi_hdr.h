#ifndef TLPI_HDR_H
// Предотвращает случайное двойное включение.
#define TLPI_HDR_H

// Определения типов, которые используются многими программами.
#include <sys/types.h>

// Стандартные функции ввода-вывода.
#include <stdio.h>
// Прототипы, наиболее востребованных библиотечных функций
// плюс константы EXIT_SUCCESS и EXIT_FAILURE.
#include <stdlib.h>

// Прототипы многих системных вызовов.
#include <unistd.h>
// Объявление errno и определение констант ошибок.
#include <errno.h>
// Наиболее используемые функции обработки строк.
#include <string.h>
// Объявление кастомных функций для обработки числовых аргументов.
#include "get_num.h"

// Объявление кастомный функций обработки ошибок.
#include "error_functions.h"

// Определение собственных функций минимума и максимума.
#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))

#endif
