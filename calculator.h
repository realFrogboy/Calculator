#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <sys/stat.h>

#define ERROR_INFO(statement, text) do { \
    if (statement) {                     \
        printf (" %s:%d, IN FUNCTION %s:\n.", __FILE__, __LINE__, __PRETTY_FUNCTION__); \
        printf (#text); \
    } \
} while (0)

const int SIZE_OF_CODE = 50;

#endif