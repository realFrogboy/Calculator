#include "calculator.h"

int funcDef (Stack *stk, int func_code, int value, int *ip)
{
    switch (func_code)
    {
        case 1:
        {    
            stackPush (stk, value);
            *ip += 5;
            return 0;
        }

        case 2:
        {    
            sum (stk);
            *ip += 1;
            return 0;
        }

        case 3:
        {    
            sub (stk);
            *ip += 1;
            return 0;
        }

        case 4:
        {    
            mul (stk);
            *ip += 1;
            return 0;
        }

        case 5:
        {    
            div (stk);
            *ip += 1;
            return 0;
        }

        case 6:
        {    
            out (stk);
            *ip += 1;
            return 0;
        }

        case 0:
        {    
            return 0;
        }

        default: 
        {
            printf ("There is no such function\n");
            return 111;
        }
    }
}


//-----------------------------------------------------------------------------


int arrayCtor (char *funct, char *str)
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
                funct[ind] = (char)func_code; 
                *(int *)(funct + ind + 1) = value;
                ind += 5;
            }
            else
            {
                funct[ind] = (char)func_code;
                ind++;
            }

            ptr_line = str + num + 1;
            str[num] = '\n';
        }

        num++;
    }

    for (num = 0; num < ind; num++)
    {
        printf ("%d\n", funct[num]);
    }

    return 0;
}


//-----------------------------------------------------------------------------


int sum (Stack *stk)
{
    int res = stk->data[stk->Size] + stk->data[stk->Size - 1];
    stackPop (stk); stackPop (stk);
    stackPush (stk, res);
    return 0;
}

int sub (Stack *stk)
{
    int res = stk->data[stk->Size - 1] - stk->data[stk->Size];
    stackPop (stk); stackPop (stk);
    stackPush (stk, res);
    return 0;
}

int mul (Stack *stk)
{
    int res = stk->data[stk->Size - 1] * stk->data[stk->Size];
    stackPop (stk); stackPop (stk);
    stackPush (stk, res);
    return 0;
}

int div (Stack *stk)
{
    int res = stk->data[stk->Size - 1] / stk->data[stk->Size];
    stackPop (stk); stackPop (stk);
    stackPush (stk, res);
    return 0;
}

int out (Stack *stk)
{
    printf ("%d\n", stk->data[stk->Size]); 
    stackPop (stk);
    return 0;   
}