#include "calculator.h"

int CPUCtor (CPU *processor)
{
    ERROR_INFO(processor == NULL, "Void ptr on processor\n");

    processor->stk = (Stack*) calloc (STACK_SIZE, sizeof (int));
    ERROR_INFO(processor->code == NULL,  "Can't alloc memory\n");

    stackCtor (processor->stk);

    processor->ip = 0;

    processor->code = (char*) calloc (SIZE_OF_CODE, sizeof (char));
    ERROR_INFO(processor->code == NULL,  "Can't alloc memory\n");

    processor->RAM = (int*) calloc (SIZE_OF_RAM, sizeof (int));
    ERROR_INFO(processor->RAM == NULL,  "Can't alloc memory\n");

    processor->regs = (int*) calloc (SIZE_OF_REGS, sizeof (int));
    ERROR_INFO(processor->regs == NULL,  "Can't alloc memory\n");

    return 0;
}


//-----------------------------------------------------------------------------


int funcDef (CPU *processor)
{
    switch ((int)processor->code[processor->ip])
    {
        case 1:
        {    
            stackPush (processor->stk, (int)processor->code[processor->ip + 1]);
            processor->ip += 5;
            return 0;
        }

        case 2:
        {    
            add (processor->stk);
            processor->ip += 1;
            return 0;
        }

        case 3:
        {    
            sub (processor->stk);
            processor->ip += 1;
            return 0;
        }

        case 4:
        {    
            mul (processor->stk);
            processor->ip += 1;
            return 0;
        }

        case 5:
        {    
            div (processor->stk);
            processor->ip += 1;
            return 0;
        }

        case 6:
        {    
            out (processor->stk);
            processor->ip += 1;
            return 0;
        }

        case 0:
        {    
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
    char *ptr_line = str; int func_code = 1, num = 0, ind = 0;
    while (func_code != 0)
    {    
        if (str[num] == '\n')
        {    
            str[num] = '\0';

            int value = 0;
        
            sscanf (ptr_line, "%d %d", &func_code, &value);

            if (func_code == 1)
            {
                processor->code[ind] = (char)func_code; 
                *(int *)(processor->code + ind + 1) = value;
                ind += 5;
            }
            else
            {
                processor->code[ind] = (char)func_code;
                ind++;
            }

            ptr_line = str + num + 1;
            str[num] = '\n';
        }

        num++;
    }

    for (num = 0; num < ind; num++)
    {
        printf ("%d\n", processor->code[num]);
    }

    return 0;
}


//-----------------------------------------------------------------------------


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
    ERROR_INFO(stk->data[stk->Size] == 0, "Division by 0!\n");

    OPERATION(/);
}

int out (Stack *stk)
{
    printf ("%d\n", stk->data[stk->Size]); 
    stackPop (stk);
    return 0;   
}