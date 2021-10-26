#include "assembler.h"

int funcDef (const char *func)
{
    if (strcmp ("stackPush", func) == 0)
        return 1;

    if (strcmp ("add", func) == 0)
        return 2;

    if (strcmp ("sub", func) == 0)
        return 3;

    if (strcmp ("mul", func) == 0)
        return 4;

    if (strcmp ("div", func) == 0)
        return 5;

    if (strcmp ("out", func) == 0)
        return 6;
    
    if (strcmp ("hlt", func) == 0)
        return 0;
    
    return 404;
}

int convertFuncIntoNumber (char *str, FILE *output)
{
    char *ptr_line = str; int res = 1, num = 0;

    while (res != 0)
    {    
        if (str[num] == '\n')
        {    
            str[num] = '\0';

            char func[10] = ""; int value = 0;
        
            sscanf (ptr_line, "%s %d", func, &value);

            res = funcDef (func);
            ERROR_INFO(res == 404, "There is no such function\n");
            
            if (res == 1)   
                fprintf (output, "%d %d\n", res, value);
            else 
                fprintf (output, "%d\n", res);

            ptr_line = str + num + 1;
            str[num] = '\n';
        }

        num++;
    }

    return 0;
}