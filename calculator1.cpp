#include <sys/stat.h>
#include "stack.h" 

int DEBUG_LEVEL = 3;

int funcDef (Stack *stk, const char *func, int value);
int sum (Stack *stk);
int sub (Stack *stk);
int mul (Stack *stk);
int div (Stack *stk);

int main ()
{
    FILE *input = NULL;
    struct stat file_info; struct Stack stk;

    stackCtor (&stk);

    if ((input = fopen ("dis.txt", "rb")) == NULL)
    {
        LOG_INFO;
        printf ("Can't open file\n");
        return 1;
    }

    int fd = fileno (input);
    fstat (fd, &file_info);

    char *str = (char*) calloc (file_info.st_size, sizeof (char));
    fread (str, file_info.st_size, sizeof (char), input);

    char *ptr_line = str; int res = 0, num = 0;

    while (res != 1)
    {    
        if (str[num] == '\n')
        {    
            str[num] = '\0';

            char func[10] = ""; int value = 0;
        
            sscanf (ptr_line, "%s %d", func, &value);

            res = funcDef (&stk, func, value) != 0;
            if (res == 111)
                printf ("!!!\n");

            ptr_line = str + num + 1;
            str[num] = '\n';
        }

        num++;
    }

    stackDtor (&stk);
    free (str);

    return 0;
}

int funcDef (Stack *stk, const char *func, int value)
{
    if (strcmp ("stackPush", func) == 0)
    {
        stackPush (stk, value);
        return 0;
    }

    if (strcmp ("sum", func) == 0)
    {    
        sum (stk);
        return 0;
    }

    if (strcmp ("sub", func) == 0)
    {
        sub (stk);
        return 0;
    }

    if (strcmp ("mul", func) == 0)
    {
        mul (stk);
        return 0;
    }

    if (strcmp ("div", func) == 0)
    {
        div (stk);
        return 0;
    }

    if (strcmp ("out", func) == 0)
    {    
        printf ("%d\n", stk->data[stk->Size]);
        return 0;
    }
    
    if (strcmp ("hlt", func) == 0)
        return 1;
    
    return 111;
}

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