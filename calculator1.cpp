#include "stack.h" 
#include "calculator.h"

int funcDef (Stack *stk, int func_code, int value, int *index);
int DEBUG_LEVEL = 3;

int main ()
{
    FILE *input = NULL;
    struct stat file_info; struct Stack stk;

    stackCtor (&stk);

    input = fopen ("code.txt", "rb");
    ERROR_INFO(input == NULL, "Can't open file\n");

    int fd = fileno (input);
    ERROR_INFO(fd == -1, "Fileno error\n");

    ERROR_INFO(fstat (fd, &file_info) == -1, "Fstat error\n");

    char *str = (char*) calloc (file_info.st_size, sizeof (char));
    ERROR_INFO(str == NULL,  "Can't alloc meemory\n");

    int nReaded = fread (str, file_info.st_size, sizeof (char), input);
    ERROR_INFO(nReaded == file_info.st_size, "Can't read file\n");

    char funct [SIZE_OF_CODE] = {0};

    arrayCtor (funct, str);

    int ind = 0;
    while ((int)funct[ind])
        funcDef (&stk, (int)funct[ind], (int)funct[ind + 1], &ind);

    stackDtor (&stk);
    free (str);
    fclose (input);

    return 0;
}