#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int funcDef (int func, char *res);

int main ()
{
    FILE *input, *output;
    struct stat file_info;

    if ((input = fopen ("code.txt", "rb")) == NULL)
    {
        printf ("Error openning input file\n");
        return 1;
    }

    if ((output = fopen ("disass.txt", "wb")) == NULL)
    {
        printf ("Error openning file output\n");
        return 1;
    }

    int fd = fileno (input);
    fstat (fd, &file_info);

    char *str = (char*) calloc (file_info.st_size, sizeof (char));
    fread (str, file_info.st_size, sizeof (char), input);

    char *ptr_line = str; char res[10] = ""; int num = 0;

    while (strcmp (res, "hlt\0") != 0)
    {    
        if (str[num] == '\n')
        {    
            str[num] = '\0';

            int func, value = 0;
        
            sscanf (ptr_line, "%d %d", &func, &value);

            funcDef (func, res);
            if (res == "dich")
                printf ("!!!\n");
            
            if (func == 1)
                fprintf (output, "%s %d\n", res, value);
            else 
                fprintf (output, "%s\n", res);

            ptr_line = str + num + 1;
            str[num] = '\n';
        }

        num++;
    }

    free (str);

    return 0;
}

int funcDef (int func, char *res)
{
    switch (func)
    {
        case 0:
        {  
            strcpy (res, "hlt\0");
            return 0;
        }
        
        case 1:
        {
            strcpy (res, "stackPush\0");
            return 0;
        }
        
        case 2:
        {
            strcpy (res, "sum\0");
            return 0;
        }
        
        case 3:
        {
            strcpy (res, "sub\0");
            return 0;
        }
        
        case 4:
        {
            strcpy (res, "mul\0");
            return 0;
        }
        
        case 5:
        {
            strcpy (res, "div\0");
            return 0;
        }
        
        case 6:
        {
            strcpy (res, "out\0");
            return 0;
        }
        
        default:
        {  
            strcpy (res, "dich\0");
            return 0;
        }
    }
}