#include "assembler.h"

struct Labels *label;
int THE_SECOND_PASS = 0;
int count = 1;

char* scanLine (const char *ptr_line)
{
    char reg = '\0', func[10] = "", lab[10] = "";
    int value = 0, ver = 0, res = 404;
    
    char *str = (char*) calloc (100, sizeof (char));
    ERROR_INFO(str == NULL, "Can't alloc memory\n");

    sprintf (str, "%s", "bad");

    sscanf (ptr_line, ":%s%n", lab, &ver);
    if (ver)
    {
        strcpy (label[count].name, lab);

        sprintf (str, ":%d", count++);

        return str;
    }
     
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

    sscanf (ptr_line, "%s :: %s%n", func, lab, &ver);
    if (ver)
    {
        res = AssFuncDef (func);
        ERROR_INFO(res == 404, "There is no such function\n");

        res = res | (1u << 5);

        for (int num = 1; num < NUM_OF_LABELS; num++)
        {
            if (strcmp (lab, label[num].name) == 0)
                value = num;
        }

        if (value == 0)
            THE_SECOND_PASS = 1; // do assembler 2 times

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
    label = (Labels*) calloc(NUM_OF_LABELS, sizeof (Labels));
    ERROR_INFO(label == NULL, "Can't alloc memory\n");
    
    LabelsCtor (label);

    PASS;

    if (THE_SECOND_PASS == 1)
    {
        count = 1;

        fclose (output);
        output = fopen ("code.txt", "wb");
        ERROR_INFO(output == NULL, "Can't open output file\n");

        PASS;
    }

    for (int num = 0; num < NUM_OF_LABELS; num++)
        printf ("%s----\n", label[num].name);

    LabelsDtor(label);

    free (label);

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
    
    else if (strcmp ("jmp", func) == 0)
        res = 8;
    
    else if (strcmp ("ja", func) == 0)
        res = 9;
    
    else if (strcmp ("jae", func) == 0)
        res = 10;
    
    else if (strcmp ("jb", func) == 0)
        res = 11;
    
    else if (strcmp ("jbe", func) == 0)
        res = 12;
    
    else if (strcmp ("je", func) == 0)
        res = 13;
    
    else if (strcmp ("jne", func) == 0)
        res = 14;
    
    else if (strcmp ("call", func) == 0)
        res = 15;
    
    else if (strcmp ("ret", func) == 0)
        res = 16;

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


//-----------------------------------------------------------------------------


int LabelsCtor (Labels *strc)
{
    ERROR_INFO(strc == NULL, "Void ptr on label\n");

    for (int num = 0; num < NUM_OF_LABELS; num++)
    {
        strc[num].name = (char*) calloc (LABEL_NAME_SIZE, sizeof (char));
        ERROR_INFO(strc[num].name == NULL, "Can't alloc memeory\n");
        strc[num].position = 0;
    }

    return 0;
}

int LabelsDtor (Labels *strc)
{
    ERROR_INFO(strc == NULL, "Void ptr on label\n");

    FILE* label_file = fopen ("lebel.txt", "wb");
    ERROR_INFO(label_file == NULL, "Can't open file\n");

    for (int num = 0; num < NUM_OF_LABELS; num++)
    {
        ERROR_INFO(strc[num].position == -1, "Repeated LabelsDtor\n");

        fprintf (label_file, "%s - %d\n", strc[num].name, num);

        free (strc[num].name);
        strc[num].position = -1;
    }

    fclose (label_file);

    return 0;
}


//-----------------------------------------------------------------------------


char* transform_file_to_str (FILE *input)
{
    struct stat file_info;

    int fd = fileno (input);
    ERROR_INFO(fd == -1, "Fileno error\n");

    ERROR_INFO(fstat (fd, &file_info) == -1, "Fstat error\n");

    char *str = (char*) calloc (file_info.st_size, sizeof (char));
    ERROR_INFO(str == NULL,  "Can't alloc meemory\n");

    int nReaded = fread (str, sizeof (char), file_info.st_size, input);
    ERROR_INFO(nReaded != file_info.st_size, "Can't read file\n");

    return str;
}