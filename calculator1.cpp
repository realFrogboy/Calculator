#include "calculator.h"

int DEBUG_LEVEL = 3;

/*#define debug_lvl_hash
#define debug_lvl_canaries

#ifdef debug_lvl_hash
    //todo
#endif*/

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

    int ip = 0;
    while ((int)funct[ip])
        funcDef (&stk, (int)funct[ip], (int)funct[ip + 1], &ip);

    stackDtor (&stk);
    free (str);
    fclose (input);

    return 0;
}