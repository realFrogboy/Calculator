#include "assembler.h"

int scanLine (const char *ptr_line)
{
    char reg = '\0', func[10] = ""; 
    int value = 0, ver = 0, res = 404;
     
    sscanf (ptr_line, "%s [%cx + %d]%n", func, &reg, &value, &ver);
    if (ver)
    {
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");

        res = res | (1u << 5);

        if ((reg >= 'a') && (reg <= 'd')) 
            res = res | (1u << 6);
        else if (res != '\0')
            ERROR_INFO (res != '\0', "There is no such register\n");

        res = res | (1u << 7);

        return res;
    }
    
    sscanf (ptr_line, "%s [%cx]%n", func, &reg, &ver);
    if (ver)
    {
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");

        if ((reg >= 'a') && (reg <= 'd')) 
            res = res | (1u << 6);
        else if (res != '\0')
            ERROR_INFO (res != '\0', "There is no such register\n");
        
        res = res | (1u << 7);

        return res;
    }

    sscanf (ptr_line, "%s [%d]%n", func, &value, &ver);
    if (ver)
    {
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");

        res = res | (1u << 5); 
        res = res | (1u << 7);

        return res;   
    }

    sscanf (ptr_line, "%s %cx%n", func, &reg, &ver);
    if (ver)
    {
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");

        if ((reg >= 'a') && (reg <= 'd')) 
            res = res | (1u << 6);
        else if (res != '\0')
            ERROR_INFO (res != '\0', "There is no such register\n");
        
        return res;
    }

    sscanf (ptr_line, "%s %d%n", func, &value, &ver);
    if (ver)
    {
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n"); 

        res = res | (1u << 5); 

        return res;  
    }
     
    sscanf (ptr_line, "%s%n", func, &ver);
    if (ver)
    {
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");
    } 
    
    ERROR_INFO(ver == 0, "Incorrect input\n");

    return res;
}


//-----------------------------------------------------------------------------


int convertFuncIntoNumber (char *str, FILE *output)
{
    char *ptr_line = str; int res = 1, num = 0;

    while (res != 0)
    {    
        if (str[num] == '\n')
        {    
            str[num] = '\0';

            res = scanLine (ptr_line);

            fprintf (output, "%d\n", res);

            ptr_line = str + num + 1;
            str[num] = '\n';
        }

        num++;
    }

    return 0;
}


//-----------------------------------------------------------------------------


int AssFuncDef (const char *func)
{
    int res = 404;

    if (strcmp ("stackPush", func) == 0)
        res = 1;

    else if (strcmp ("add", func) == 0)
        res = 2;

    else if (strcmp ("sub", func) == 0)
        res = 3;

    else if (strcmp ("mul", func) == 0)
        res = 4;

    else if (strcmp ("div", func) == 0)
        res = 5;

    else if (strcmp ("out", func) == 0)
        res = 6;
    
    else if (strcmp ("hlt", func) == 0)
        res = 0;
    
    else if (strcmp ("pop", func) == 0)
        res = 7;
    
    return res;    
}