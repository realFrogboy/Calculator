#include "assembler.h"

struct Labels *label;

int DisFuncDef (const char *ptr_line, char *out)
{   
    ERROR_INFO(ptr_line == NULL, "Void ptr on string\n");
    ERROR_INFO(out == NULL, "Void ptr on string\n");

    int funcNum = 0, func = 0, regNum = 0, ver = 0;
    double val = 0;

    char *name_of_func = (char*) calloc (10, sizeof (char));
    ERROR_INFO(name_of_func == NULL, "Can't alloc memory\n");

    sscanf (ptr_line, ":%d%n", &func, &ver);
    if (ver)
    {
        for (int num = 0; num < NUM_OF_LABELS; num++)
        {
            if (num == func)
                sprintf (out, ": %s", label[num].name);
        }

        free (name_of_func);
        return 0;
    }
    
    sscanf (ptr_line, "%d", &func);

    for (int num = 4; num >= 0; num--)
        funcNum += ((func >> num) & 1u) * pow_mod (2, num);

    int tmp = defineName (funcNum, name_of_func);
    ERROR_INFO(tmp == 1, "There is no such function");
    
    if ((((func >> 5) & 1u) == 1) && (((func >> 6) & 1u) == 1))
    {    
        sscanf (ptr_line, "%d %d %lf", &func, &regNum, &val);

        char reg = (char)(regNum + (int)('a') - 1);

        (((func >> 7) & 1u) == 1) ? sprintf (out, "%s [%cx + %lf]", name_of_func, reg, val) : sprintf (out, "%s %cx + %lf", name_of_func, reg, val);

        free (name_of_func);
        return 0;
    }

    else if ((((func >> 6) & 1u) == 1))
    {
        sscanf (ptr_line, "%d %d", &func, &regNum);

        char reg = (char)(regNum + (int)('a') - 1);

        (((func >> 7) & 1u) == 1) ? sprintf (out, "%s [%cx]", name_of_func, reg) : sprintf (out, "%s %cx", name_of_func, reg);

        free (name_of_func);
        return 0;   
    }

    else if ((((func >> 5) & 1u) == 1))
    {
        sscanf (ptr_line, "%d %lf", &func, &val);

        if ((funcNum >= 8) && (funcNum <=15))                               // FOR FUNCTIONS WITH LABELS
        {                                                                   // 
            for (int num = 0; num < NUM_OF_LABELS; num++)                   //
                if (num == (int)val)                                        //
                {                                                           // FOR FUNCTIONS WITH LABELS
                    sprintf (out, "%s :: %s", name_of_func, label[num].name);//
                    free (name_of_func);                                    //
                    return 1;                                               //
                }                                                           //
        }                                                                   // FOR FUNCTIONS WITH LABELS

        (((func >> 7) & 1u) == 1) ? sprintf (out, "%s [%lf]", name_of_func, val) : sprintf (out, "%s %lf", name_of_func, val);

        free (name_of_func);
        return 0;   
    }

    else
        sprintf (out, "%s", name_of_func);

    free (name_of_func);

    return 0;
}


//-----------------------------------------------------------------------------


int defineName (int funcNum, char *name_of_func)
{
    ERROR_INFO(name_of_func == NULL, "Void ptr on string\n");

    switch (funcNum)
    {
        case 0:
        {  
            strcpy (name_of_func, "hlt");
            return 0;
        }
        
        case 1:
        {
            strcpy (name_of_func, "push");
            return 0;
        }
        
        case 2:
        {
            strcpy (name_of_func, "add");
            return 0;
        }
        
        case 3:
        {
            strcpy (name_of_func, "sub");
            return 0;
        }
        
        case 4:
        {
            strcpy (name_of_func, "mul");
            return 0;
        }
        
        case 5:
        {
            strcpy (name_of_func, "div");
            return 0;
        }
        
        case 6:
        {
            strcpy (name_of_func, "out");
            return 0;
        }

        case 7:
        {
            strcpy (name_of_func, "pop");
            return 0;
        }

        case 8:
        {
            strcpy (name_of_func, "jmp");
            return 0;
        }

        case 9:
        {
            strcpy (name_of_func, "ja");
            return 0;
        }

        case 10:
        {
            strcpy (name_of_func, "jae");
            return 0;
        }

        case 11:
        {
            strcpy (name_of_func, "jb");
            return 0;
        }

        case 12:
        {
            strcpy (name_of_func, "jbe");
            return 0;
        }

        case 13:
        {
            strcpy (name_of_func, "je");
            return 0;
        }

        case 14:
        {
            strcpy (name_of_func, "jne");
            return 0;
        }

        case 15:
        {
            strcpy (name_of_func, "call");
            return 0;
        }

        case 16:
        {
            strcpy (name_of_func, "ret");
            return 0;
        }

        case 17:
        {
            strcpy (name_of_func, "sqrt");
            return 0;
        }
        
        default:
        {  
            ERROR_INFO(name_of_func != NULL, "There is no such function\n");
            strcpy (name_of_func, "dich");
            return 1;
        } 
    }  
}


//-----------------------------------------------------------------------------


int convertNumberIntoFunc (char *str, FILE *output)
{
    ERROR_INFO(str == NULL, "Void ptr on string\n");
    ERROR_INFO(output == NULL, "Void ptr on file\n");

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
            str[num] = '\0';
            DIS_PASS;
            str[num] = '\n';
        }
        num++;
    }

    DIS_PASS;

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
            str[num] = '\0';
            
            PUSH_LABELS;

            ptr_line = str + num + 1;

            str[num] = '\n';
        }

        num++;
    }

    PUSH_LABELS;

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
    ERROR_INFO(input == NULL, "Void ptr on file\n");

    struct stat file_info;

    int fd = fileno (input);
    ERROR_INFO(fd == -1, "Fileno error\n");

    ERROR_INFO(fstat (fd, &file_info) == -1, "Fstat error\n");

    char *str = (char*) calloc (file_info.st_size + 1, sizeof (char));
    ERROR_INFO(str == NULL,  "Can't alloc meemory\n");

    int nReaded = fread (str, sizeof (char), file_info.st_size, input);
    ERROR_INFO(nReaded != file_info.st_size, "Can't read file\n");

    str[file_info.st_size] = '\0';

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