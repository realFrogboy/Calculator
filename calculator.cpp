#include "calculator.h"
#include <math.h>

int regNum = 0;

int CPUCtor (CPU *processor)
{
    ERROR_INFO(processor == NULL, "Void ptr on processor\n");

    processor->stk = (Stack*) calloc (1, sizeof (Stack));
    ERROR_INFO(processor->stk == NULL,  "Can't alloc memory\n");

    stackCtor (processor->stk);

    processor->stk_for_call = (Stack*) calloc (1, sizeof (Stack));
    ERROR_INFO(processor->stk_for_call == NULL,  "Can't alloc memory\n");

    stackCtor (processor->stk_for_call);

    processor->label = (int*) calloc (NUM_OF_LABELS, sizeof (int));
    ERROR_INFO(processor->label == NULL, "Can't alloc memory\n");

    processor->ip = 0;

    processor->code = (char*) calloc (SIZE_OF_CODE, sizeof (char));
    ERROR_INFO(processor->code == NULL,  "Can't alloc memory\n");

    processor->RAM = (int*) calloc (SIZE_OF_RAM, sizeof (int));
    ERROR_INFO(processor->RAM == NULL,  "Can't alloc memory\n");

    processor->regs = (int*) calloc (SIZE_OF_REGS, sizeof (int));
    ERROR_INFO(processor->regs == NULL,  "Can't alloc memory\n");

    for (int num = 0; num < SIZE_OF_REGS; num++)
        processor->regs[num] = num;

    return 0;
}

int CPUDtor (CPU *processor)
{
    ERROR_INFO(processor == NULL, "Void ptr on processor\n");
    ERROR_INFO(processor->ip == -1, "Repeated CPUDtor\n");

    stackDtor (processor->stk);
    stackDtor (processor->stk_for_call);
    
    free (processor->stk); 
    free (processor->stk_for_call);
    free (processor->code); 
    free (processor->RAM); 
    free (processor->regs); 
    free (processor->label);
    processor->ip = -1;

    return 0;    
}


//-----------------------------------------------------------------------------


int DOFunc (CPU *processor)
{
    int func = *(int*)(processor->code + processor->ip);
    
    int funcNum = 0;

    for (int num = 4; num >= 0; num--)
        funcNum += ((func >> num) & 1u) * pow_mod (2, num);
    
    if ((((func >> 5) & 1u) == 1) && (((func >> 6) & 1u) == 1))
    {    
        regNum = *(int*)(processor->code + processor->ip + 1);
        int val = *(int*)(processor->code + processor->ip + 1);

        int error = RealizeFunc (processor, funcNum, processor->regs[regNum] + val, func);
        ERROR_INFO(error == 404, "There is no such function\n");

        processor->ip += 9;

        return error;
    }

    else if ((((func >> 6) & 1u) == 1))
    {
        regNum = *(int*)(processor->code + processor->ip + 1);

        int error = RealizeFunc (processor, funcNum, processor->regs[regNum], func);
        ERROR_INFO(error == 404, "There is no such function\n");

        processor->ip += 5;

        return error; 
    }

    else if ((((func >> 5) & 1u) == 1))
    {
        int val = *(int*)(processor->code + processor->ip + 1);

        int error = RealizeFunc (processor, funcNum, val, func);
        ERROR_INFO(error == 404, "There is no such function\n");

        processor->ip += 5;

        return error;
    }
    
    else
    {    
        int NOval = 0;

        int error = RealizeFunc (processor, funcNum, NOval, func);
        ERROR_INFO(error == 404, "There is no such function\n");

        processor->ip += 1;
    
        return error;
    }
}


//-----------------------------------------------------------------------------


int RealizeFunc (CPU *processor, int funcNum, int value, int func)
{
    switch (funcNum)
    {
        case 1:
        {   
            push (processor, value, func);
            return 0;
        }

        case 2:
        {    
            add (processor->stk);
            return 0;
        }

        case 3:
        {    
            sub (processor->stk);
            return 0;
        }

        case 4:
        {    
            mul (processor->stk);
            return 0;
        }

        case 5:
        {    
            div (processor->stk);
            return 0;
        }

        case 6:
        {    
            out (processor->stk);
            return 0;
        }

        case 7:
        {
            if (((func >> 7) & 1u) == 1)
                value = value;
            else   
                value = regNum; 
    
            pop (processor, value, func);
            return 0;
        }

        case 8:
        {
            jmp (processor, value);
            return 0;
        }

        case 9:
        {
            ja (processor, value);
            return 0;
        }

        case 10:
        {
            jae (processor, value);
            return 0;
        }

        case 11:
        {
            jb (processor, value);
            return 0;
        }

        case 12:
        {
            jbe (processor, value);
            return 0;
        }

        case 13:
        {
            je (processor, value);
            return 0;
        }

        case 14:
        {
            jne (processor, value);
            return 0;
        }

        case 15:
        {
            call (processor, value);
            return 0;
        }

        case 16:
        {
            ret (processor);
            return 0;
        }

        case 17:
        {
            sqrt_ (processor);
            return 0;
        }

        case 0:
        {    
            //hlt
            return 0;
        }

        default: 
        {
            return 404;
        }
    }
}


//-----------------------------------------------------------------------------


int arrayCtor (CPU *processor, char *str)
{
    char *ptr_line = str; 
    int num = 0;

    while (str[num] != '\0')
    {    
        if (str[num] == '\n')
        {   
            str[num] = '\0';
            CPUFuncDef (processor, ptr_line);
            ptr_line = str + num + 1;
            str[num] = '\n';
        }
        num++;
    }

    CPUFuncDef (processor, ptr_line);

    for (num = 0; num < processor->ip; num++)
    {
        printf ("%d\n", processor->code[num]);
    }

    for (int num = 0; num < NUM_OF_LABELS; num++)
        printf ("%d,  ", processor->label[num]);

    printf ("\n");
    processor->ip = 0;

    return 0;
}


//-----------------------------------------------------------------------------



int CPUFuncDef (CPU *processor, const char *ptr_line)
{
    int func = 0, ver = 0;

    sscanf (ptr_line, ":%d%n", &func, &ver);
    if (ver)
    {
        for (int num = 1; num < NUM_OF_LABELS; num++)
        {
            if (num == func)
                processor->label[num] = processor->ip;
        }

        return 0;
    }

    sscanf (ptr_line, "%d", &func);

    if ((((func >> 5) & 1u) == 1) && (((func >> 6) & 1u) == 1))
    {    
        int regNum = 0, val = 0;
        sscanf (ptr_line, "%d %d %d", &func, &regNum, &val);

        processor->code[processor->ip] = (char) func;
        *(int*)(processor->code + processor->ip + 1) = regNum;
        *(int*)(processor->code + processor->ip + 5) = val;

        processor->ip += 9;

        return 0;
    }

    else if ((((func >> 6) & 1u) == 1))
    {
        int regNum = 0;
        sscanf (ptr_line, "%d %d", &func, &regNum);

        processor->code[processor->ip] = (char) func;
        *(int*)(processor->code + processor->ip + 1) = regNum;

        processor->ip += 5;

        return 0;   
    }

    else if ((((func >> 5) & 1u) == 1))
    {
        int val = 0;
        sscanf (ptr_line, "%d %d", &func, &val);

        processor->code[processor->ip] = (char) func;
        *(int*)(processor->code + processor->ip + 1) = val;

        processor->ip += 5;

        return 0;   
    }

    else
    {
        processor->code[processor->ip] = (char) func;
        processor->ip += 1;

        return 0;
    }
}


//-----------------------------------------------------------------------------


int push (CPU *processor, int value, int func)
{
    if (((func >> 7) & 1u) == 1)
        stackPush (processor->stk, processor->RAM[value]);
    else
        stackPush (processor->stk, value);

    return 0;
}

int add (Stack *stk)
{
    OPERATION(+);
}

int sub (Stack *stk)
{
    OPERATION(-);
}

int mul (Stack *stk)
{
    OPERATION(*);
}

int div (Stack *stk)
{
    ERROR_INFO(stk->data[stk->Size - 1] == 0, "Division by 0!\n");

    OPERATION(/);
}

int out (Stack *stk)
{

    printf ("%5.2f\n", stk->data[stk->Size - 1]); 
    stackPop (stk);

    return 0;   
}

int pop (CPU *processor, int value, int func)
{
    if (((func >> 7) & 1u) == 1)
        processor->RAM[value] = processor->stk->data[processor->stk->Size - 1];
    else    
        processor->regs[value] = processor->stk->data[processor->stk->Size - 1];
    
    stackPop (processor->stk);

    return 0;
}

int jmp (CPU *processor, int value)
{
    processor->ip = processor->label[value] - 5;

    return 0;
}

int ja (CPU *processor, int value)
{
    if (processor->stk->data[processor->stk->Size - 2] > processor->stk->data[processor->stk->Size - 1])
        processor->ip = processor->label[value] - 5;
    
    stackPop (processor->stk); stackPop (processor->stk);

    return 0;
}

int jae (CPU *processor, int value)
{
    if (processor->stk->data[processor->stk->Size - 2] >= processor->stk->data[processor->stk->Size - 1])
        processor->ip = processor->label[value] - 5;
    
    stackPop (processor->stk); stackPop (processor->stk);

    return 0;
}

int jb (CPU *processor, int value)
{
    if (processor->stk->data[processor->stk->Size - 2] < processor->stk->data[processor->stk->Size - 1])
        processor->ip = processor->label[value] - 5;
    
    stackPop (processor->stk); stackPop (processor->stk);

    return 0;
}

int jbe (CPU *processor, int value)
{
    if (processor->stk->data[processor->stk->Size - 2] <= processor->stk->data[processor->stk->Size - 1])
        processor->ip = processor->label[value] - 5;
    
    stackPop (processor->stk); stackPop (processor->stk);

    return 0;
}

int je (CPU *processor, int value)
{
    printf ("==============%f, %f\n", processor->stk->data[processor->stk->Size - 2], processor->stk->data[processor->stk->Size - 1]);
    if (isequal (processor->stk->data[processor->stk->Size - 2], processor->stk->data[processor->stk->Size - 1]))
        processor->ip = processor->label[value] - 5;
    
    stackPop (processor->stk); stackPop (processor->stk);

    return 0;
}

int jne (CPU *processor, int value)
{
    if (processor->stk->data[processor->stk->Size - 2] != processor->stk->data[processor->stk->Size - 1])
        processor->ip = processor->label[value] - 5;
    
    stackPop (processor->stk); stackPop (processor->stk);

    return 0;
}

int call (CPU *processor, int value)
{
    stackPush (processor->stk_for_call, processor->ip + 5);
    processor->ip = processor->label[value] - 5;

    return 0;
}

int ret (CPU *processor)
{
    processor->ip = processor->stk_for_call->data[processor->stk_for_call->Size - 1] - 1;
    stackPop (processor->stk_for_call);

    return 0;
}

int sqrt_ (CPU *processor)
{
    int value = processor->stk->data[processor->stk->Size - 1];
    stackPop (processor->stk);
    double num = (double)value;
    double res = sqrt (num);
    int res_i = (int)res;
    stackPush (processor->stk, res_i); 

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

    char *str = (char*) calloc (file_info.st_size + 1, sizeof (char));
    ERROR_INFO(str == NULL,  "Can't alloc meemory\n");

    int nReaded = fread (str, sizeof (char), file_info.st_size, input);
    ERROR_INFO(nReaded != file_info.st_size, "Can't read file\n");

    str[file_info.st_size] = '\0';

    return str;
}

struct stat get_file_info (FILE *input) 
{
    struct stat file_info;

    int fd = fileno (input);
    ERROR_INFO(fd == -1, "Fileno error\n");

    ERROR_INFO(fstat (fd, &file_info) == -1, "Fstat error\n");

    return file_info;    
}

bool isequal(double a, double b){
    const double EPSILON = 0.0001; //measurement error
    
    if (fabs(a - b) <= EPSILON)
        return 1;
    else 
        return 0;
}