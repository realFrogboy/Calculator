#include "assembler.h"

char* DisFuncDef (const char *ptr_line)
{
    int funcNum = 0, func = 0, regNum = 0, val = 0;

    char *str = (char*) calloc (100, sizeof (char));
    ERROR_INFO(str == NULL, "Can't alloc memory\n");

    char *name_of_func = (char*) calloc (10, sizeof (char));
    ERROR_INFO(name_of_func == NULL, "Can't alloc memory\n");

    sscanf (ptr_line, "%d", &func);

    for (int num = 4; num >= 0; num--)
        funcNum += ((func >> num) & 1u) * pow_mod (2, num);

    name_of_func = defineName (funcNum);
    
    if ((((func >> 5) & 1u) == 1) && (((func >> 6) & 1u) == 1))
    {    
        sscanf (ptr_line, "%d %d %d", &func, &regNum, &val);

        char reg = (char)(regNum + (int)('a') - 1);

        (((func >> 7) & 1u) == 1) ? sprintf (str, "%s [%cx + %d]", name_of_func, reg, val) : sprintf (str, "%s %cx + %d", name_of_func, reg, val);

        return str;
    }

    else if ((((func >> 6) & 1u) == 1))
    {
        sscanf (ptr_line, "%d %d", &func, &regNum);

        char reg = (char)(regNum + (int)('a') - 1);

        (((func >> 7) & 1u) == 1) ? sprintf (str, "%s [%cx]", name_of_func, reg) : sprintf (str, "%s %cx", name_of_func, reg);

        return str;   
    }

    else if ((((func >> 5) & 1u) == 1))
    {
        sscanf (ptr_line, "%d %d", &func, &val);

        (((func >> 7) & 1u) == 1) ? sprintf (str, "%s [%d]", name_of_func, val) : sprintf (str, "%s %d", name_of_func, val);

        return str;   
    }

    else
        sprintf (str, "%s", name_of_func);

    free (name_of_func);

    return str;
}


//-----------------------------------------------------------------------------


char* defineName (int funcNum)
{
    char *res = (char*) calloc (10, sizeof (char));
    ERROR_INFO(res == NULL, "Can't alloc memory\n");

    switch (funcNum)
    {
        case 0:
        {  
            strcpy (res, "hlt");
            return res;
        }
        
        case 1:
        {
            strcpy (res, "push");
            return res;
        }
        
        case 2:
        {
            strcpy (res, "add");
            return res;
        }
        
        case 3:
        {
            strcpy (res, "sub");
            return res;
        }
        
        case 4:
        {
            strcpy (res, "mul");
            return res;
        }
        
        case 5:
        {
            strcpy (res, "div");
            return res;
        }
        
        case 6:
        {
            strcpy (res, "out");
            return res;
        }

        case 7:
        {
            strcpy (res, "pop");
            return res;
        }
        
        default:
        {  
            ERROR_INFO(res != NULL, "There is no such function\n");
            strcpy (res, "dich");
            return res;
        } 
    }  
}


//-----------------------------------------------------------------------------


int convertNumberIntoFunc (char *str, FILE *output)
{
    char *ptr_line = str;
    int num = 0;

    while (str[num] != '\0')
    {    
        if (str[num] == '\n')
        {    
            char *out = (char*) calloc (100, sizeof (char));
            ERROR_INFO(out == NULL, "Can't alloc memory\n");
        
            out = DisFuncDef (ptr_line);

            fprintf (output, "%s\n", out);

            ptr_line = str + num + 1;

            free (out);
        }

        num++;
    }

    return 0;
}


//-----------------------------------------------------------------------------


unsigned long long pow_mod (unsigned long long n, unsigned long long k) 
{
    unsigned long long mult = n;
    unsigned long long prod = 1;
    while (k > 0) 
    {
        if ((k % 2) == 1) 
        {
            prod = mult_mod (prod, mult); k = k - 1;
        }
        
        mult = mult_mod (mult, mult); k = k / 2;
    }
    
    return prod;
}


unsigned long long mult_mod (unsigned long long n, unsigned long long k) 
{
    unsigned long long mult = n;
    unsigned long long prod = 0;
    while (k > 0) 
    {
        if ((k % 2) == 1) 
        {
            prod = prod + mult; 
            k = k - 1;
        }
    
        mult = mult + mult; 
        k = k / 2;
    }
    
    return prod;
}