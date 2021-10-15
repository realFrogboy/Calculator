#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h> 

int funcDef (const char *func);

int main ()
{
    FILE *input, *output;
    struct stat file_info;

    if ((input = fopen ("dis.txt", "rb")) == NULL)
    {
        printf ("Error openning input file\n");
        return 1;
    }

    if ((output = fopen ("code.txt", "wb")) == NULL)
    {
        printf ("Error openning file output\n");
        return 1;
    }

    int fd = fileno (input);
    fstat (fd, &file_info);

    char *str = (char*) calloc (file_info.st_size, sizeof (char));
    fread (str, file_info.st_size, sizeof (char), input);

    char *ptr_line = str; int res = 1, num = 0;

    while (res != 0)
    {    
        if (str[num] == '\n')
        {    
            str[num] = '\0';

            char func[10] = ""; int value = 0;
        
            sscanf (ptr_line, "%s %d", func, &value);

            res = funcDef (func);
            if (res == 111)
                printf ("!!!\n");
            
            if (res == 1)   
                fprintf (output, "%d %d\n", res, value);
            else 
                fprintf (output, "%d\n", res);

            ptr_line = str + num + 1;
            str[num] = '\n';
        }

        num++;
    }

    free (str);

    return 0;
}

int funcDef (const char *func)
{
    if (strcmp ("stackPush", func) == 0)
        return 1;

    if (strcmp ("sum", func) == 0)
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
    
    return 111;
}