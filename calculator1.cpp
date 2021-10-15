#include "stack.h" 
#include "calculator.h"

int funcDef (Stack *stk, int func_code, int value, int *index);
int DEBUG_LEVEL = 3;

int main ()
{
    FILE *input = NULL;
    struct stat file_info; struct Stack stk;

    stackCtor (&stk);

    input = fopen ("dis.txt", "rb");
    ERROR_INFO(input == NULL, "Can't open file\n");

    int fd = fileno (input);
    fstat (fd, &file_info);

    char *str = (char*) calloc (file_info.st_size, sizeof (char));
    ERROR_INFO(str == NULL,  "Can't alloc meemory\n");

    int nReaded = fread (str, file_info.st_size, sizeof (char), input);
    ERROR_INFO(nReaded == file_info.st_size, "Can't read file\n");

    char *ptr_line = str; int func_code = 1, num = 0, ind = 0;
    int funct [SIZE_OF_CODE] = {0};

    while (func_code != 0)
    {    
        if (str[num] == '\n')
        {    
            str[num] = '\0';

            int value = 0;
        
            sscanf (ptr_line, "%d %d", &func_code, &value);

            if (func_code == 1)
            {
                funct[ind] = func_code; funct[ind + 1] = value;
                ind += 2;
            }
            else
            {
                funct[ind] = func_code;
                ind++;
            }

            ptr_line = str + num + 1;
            str[num] = '\n';
        }

        num++;
    }

    while (funct[ind])
    {
        funcDef (&stk, funct[ind], funct[ind + 1], &ind);
    }

    stackDtor (&stk);
    free (str);

    return 0;
}