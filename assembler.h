#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h> 

#ifndef ERROR_INFO
#define ERROR_INFO(statement, text) do {                                                                                    \
                                        if (statement) {                                                                    \
                                            printf (" %s:%d, IN FUNCTION %s:\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
                                            printf (#text);                                                                 \
                                        }                                                                                   \
                                    } while (0)
#endif

#ifndef PASS
#define PASS do {                                                              \
                int num = 1;                                                   \
                char *ptr_line = str;                                          \
                while (str[num - 1] != '\0')                                   \
                {                                                              \
                    if ((str[num] == '\n') || (str[num] == '\0'))              \
                    {                                                          \
                        char *out = (char*) calloc (100, sizeof (char));       \
                        ERROR_INFO(out == NULL, "Can't alloc memeory\n");      \
                        out = scanLine (ptr_line);                             \
                        fprintf (output, "%s\n", out);                         \
                        ptr_line = str + num + 1;                              \
                        free (out);                                            \
                    }                                                          \
                num++;                                                         \
                }                                                              \
            }while (0)
#endif

struct Labels
{
    char *name;
    int position;
};

const int NUM_OF_LABELS   = 10;
const int LABEL_NAME_SIZE = 20;

int LabelsCtor (Labels *label);
int LabelsDtor (Labels *label);

char* transform_file_to_str (FILE *input);

int AssFuncDef (const char *func);
int placeReg (char reg, int *res);
char* scanLine (const char *ptr_line);
int convertFuncIntoNumber (char *str, FILE *output);


char* DisFuncDef (const char *ptr_line);
int fill_labels (Labels *strc);
char* defineName (int funcNum);
int convertNumberIntoFunc (char *str, FILE *output);
unsigned long long pow_mod (unsigned long long n, unsigned long long k);
unsigned long long mult_mod (unsigned long long n, unsigned long long k);

#endif