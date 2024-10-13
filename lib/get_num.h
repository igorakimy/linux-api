#ifndef GET_NUM_H
#define GET_NUM_H
// Значение должно быть >= 0
#define GN_NONNEG 01
// Значение должно быть > 0
#define GN_GT_0   02

// По умолчанию целые числа являются десятичными.
// Можно использовать любое основание - наподобие strtol(3).
#define GN_ANY_BASE 0100

#define GN_BASE_8   0200
#define GN_BASE_16  0400

// Функции для более удобного преобразования числовых аргументов командной строки.
long getLong(const char *arg, int flags, const char *name);
int getInt(const char *arg, int flags, const char *name);

#endif
