#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h> 

#define ERROR_INFO(statement, text) do { \
    if (statement) {                     \
        printf (" %s:%d, IN FUNCTION %s:\n.", __FILE__, __LINE__, __PRETTY_FUNCTION__); \
        printf (#text); \
    } \
} while (0)

int funcDef (const char *func);
int convertFuncIntoNumber (char *str, FILE *output);