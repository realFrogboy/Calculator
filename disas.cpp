#include "assembler.h"

struct Labels *label;

char* DisFuncDef (const char *ptr_line)
{   
    int funcNum = 0, func = 0, regNum = 0, val = 0, ver = 0;

    char *str = (char*) calloc (100, sizeof (char));
    ERROR_INFO(str == NULL, "Can't alloc memory\n");

    char *name_of_func = (char*) calloc (10, sizeof (char));
    ERROR_INFO(name_of_func == NULL, "Can't alloc memory\n");

    sscanf (ptr_line, ":%d%n", &func, &ver);
    if (ver)
    {
        for (int num = 0; num < NUM_OF_LABELS; num++)
        {
            if (num == func)
                sprintf (str, ": %s", label[num].name);
        }

        return str;
    }
    
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
    label = (Labels*) calloc(NUM_OF_LABELS, sizeof (Labels));
    ERROR_INFO(label == NULL, "Can't alloc memory\n");
    
    LabelsCtor (label);
    fill_labels (label);

    for (int num = 0; num < NUM_OF_LABELS; num++)
        printf ("%s----\n", label[num].name);

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

    LabelsDtor (label);
    free (label);

    return 0;
}


//-----------------------------------------------------------------------------


int fill_labels (Labels *strc)
{
    FILE *label_file = fopen ("lebel.txt", "r");
    ERROR_INFO(label_file == NULL, "Can't open file\n");

    char *str = transform_file_to_str (label_file);

    char *ptr_line = str;
    int num = 0;

    while (str[num] != '\0')
    {    
        if (str[num] == '\n')
        {    
            char name[10] = "";
            int pos = 0;

            sscanf (ptr_line, "%s - %d", name, &pos);
            printf ("%s,,,,,%d\n", name, pos);

            if (strcmp (name, "-"))
                strcpy (strc[pos].name, name);

            ptr_line = str + num + 1;
        }

        num++;
    }

    free (str);
    fclose (label_file);

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


//-----------------------------------------------------------------------------


int LabelsCtor (Labels *strc)
{
    ERROR_INFO(strc == NULL, "Void ptr on label\n");

    for (int num = 0; num < NUM_OF_LABELS; num++)
    {
        strc[num].name = (char*) calloc (LABEL_NAME_SIZE, sizeof (char));
        ERROR_INFO(strc[num].name == NULL, "Can't alloc memeory\n");
        strc[num].position = -2;
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