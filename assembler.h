#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h> 

#ifndef ERROR_INFO
#define ERROR_INFO(statement, text) do {                                                \
    if (statement) {                                                                    \
        printf (" %s:%d, IN FUNCTION %s:\n", __FILE__, __LINE__, __PRETTY_FUNCTION__); \
        printf (#text);                                                                 \
    }                                                                                   \
} while (0)
#endif

int AssFuncDef (const char *func);
int placeReg (char reg, int *res);
char* scanLine (const char *ptr_line);
int convertFuncIntoNumber (char *str, FILE *output);


char* DisFuncDef (const char *ptr_line);
char* defineName (int funcNum);
int convertNumberIntoFunc (char *str, FILE *output);
unsigned long long pow_mod (unsigned long long n, unsigned long long k);
unsigned long long mult_mod (unsigned long long n, unsigned long long k);

#endif