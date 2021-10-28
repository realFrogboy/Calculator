#include "assembler.h"

char* scanLine (const char *ptr_line)
{
    char reg = '\0', func[10] = "";
    int value = 0, ver = 0, res = 404;
    
    char *str = (char*) calloc (100, sizeof (char));
    ERROR_INFO(str == NULL, "Can't alloc memory\n");

    sprintf (str, "%s", "bad");
     
    sscanf (ptr_line, "%s [%cx + %d]%n", func, &reg, &value, &ver);
    if (ver)
    {
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");

        res = res | (1u << 5);

        int regNum = placeReg (reg, &res);

        res = res | (1u << 7);

        sprintf (str, "%d %d %d", res, regNum, value);

        return str;
    }

    sscanf (ptr_line, "%s %cx + %d%n", func, &reg, &value, &ver);
    if (ver)
    {
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");

        res = res | (1u << 5);

        int regNum = placeReg (reg, &res);

        sprintf (str, "%d %d %d", res, regNum, value);

        return str;
    }
    
    sscanf (ptr_line, "%s [%cx]%n", func, &reg, &ver);
    if (ver)
    {
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");

        int regNum = placeReg (reg, &res);

        res = res | (1u << 7);

        sprintf (str, "%d %d", res, regNum);

        return str;
    }

    sscanf (ptr_line, "%s [%d]%n", func, &value, &ver);
    if (ver)
    {   
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");

        res = res | (1u << 5); 
        res = res | (1u << 7);

        sprintf (str, "%d %d", res, value);

        return str;   
    }

    sscanf (ptr_line, "%s %cx%n", func, &reg, &ver);
    if (ver)
    {
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");

        int regNum = placeReg (reg, &res);
        
        sprintf (str, "%d %d", res, regNum);

        return str;
    }

    sscanf (ptr_line, "%s %d%n", func, &value, &ver);
    if (ver)
    { 
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n"); 

        res = res | (1u << 5); 

        sprintf (str, "%d %d", res, value);

        return str;  
    }
     
    sscanf (ptr_line, "%s%n", func, &ver);
    if (ver)
    {   
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");

        sprintf (str, "%d", res);
        
        return str;
    } 
    
    ERROR_INFO(ver == 0, "Incorrect input\n");

    return str;
}


//-----------------------------------------------------------------------------


int convertFuncIntoNumber (char *str, FILE *output)
{
    char *ptr_line = str;
    int num = 0;

    while (str[num] != '\0')
    {    
        if (str[num] == '\n')
        {    
            char *out = (char*) calloc (100, sizeof (char));
            ERROR_INFO(out == NULL, "Can't alloc memeory\n");

            out = scanLine (ptr_line);

            fprintf (output, "%s\n", out);

            ptr_line = str + num + 1;
            free (out);
        }

        num++;
    }

    return 0;
}


//-----------------------------------------------------------------------------


int AssFuncDef (const char *func)
{
    int res = 404;

    if (strcmp ("push", func) == 0)
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


//-----------------------------------------------------------------------------


int placeReg (char reg, int *res)
{
    int regNum = 0;

    if ((reg >= 'a') && (reg <= 'd')) 
    {   
        *res = *res | (1u << 6);
        regNum = (int)reg - (int)('a') + 1;
    }
    else if (reg != '\0')
        ERROR_INFO (reg != '\0', "There is no such register\n");
    
    return regNum;
}