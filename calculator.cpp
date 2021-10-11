#include "stack.h"

int DEBUG_LEVEL = 3;

int getNumber (char *input, int *res);

int main ()
{
    struct Stack stk;
    stackCtor (&stk);

    char input[5] = ""; int res = 0;

    int val = getNumber (input, &res);

    stackPush (&stk, val);

    while (1)
    {
        val = getNumber (input, &res);

        stackPush (&stk, val);

        char sign = 0;
        printf ("Enter a sign\n");
        scanf ("%c", &sign);

        switch (sign)
        {
            case '+':
            {
                res = stk.data[stk.Size] + stk.data[stk.Size - 1];
                stackPop (&stk); stackPop (&stk);
                break;
            }

            case '-':
            {
                res = stk.data[stk.Size - 1] - stk.data[stk.Size];
                stackPop (&stk); stackPop (&stk);
                break;
            }

            case '*':
            {
                res = stk.data[stk.Size] * stk.data[stk.Size - 1];
                stackPop (&stk); stackPop (&stk);
                break;
            }

            case '/':
            {
                res = stk.data[stk.Size - 1] / stk.data[stk.Size];
                stackPop (&stk); stackPop (&stk);
                break;
            }
        }

        printf ("%d\n", res);

        stackPush (&stk, res);
    }

    printf ("%d\n", res);

    stackDtor (&stk);

    return 0;
}

int getNumber (char *input, int *res)
{
    printf ("Enter a number or \"q\" to end the programm.\n");
    if (fscanf (stdin, "%s", input) == 0)
    {
        LOG_INFO;
        printf ("fscanf ERROR\n");
    }

    if (input[0] == 'q') 
    {
        printf ("%d\n", *res);
        return 0;
    }

    int val = atoi (input);
    if ((val == 0) && (input[0] != '0'))
    {
        LOG_INFO;
        printf ("atoi ERROR\n");
    }

    printf ("%d\n", val);
    cleanBuffer ();    

    return val;
}