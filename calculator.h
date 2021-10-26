#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "stack.h"

#ifndef OPERATION
#define OPERATION(sign) do{                                                                 \
                            int res = stk->data[stk->Size - 1] sign stk->data[stk->Size];   \
                            stackPop (stk); stackPop (stk);                                 \
                            stackPush (stk, res);                                           \
                            return 0;                                                       \
                        } while (0)
#endif

const int SIZE_OF_CODE = 100;
const int STACK_SIZE   = 1000;
const int SIZE_OF_RAM  = 50;
const int SIZE_OF_REGS = 10;

struct CPU 
{
    struct Stack *stk;
    char *code;
    int ip;
    int *RAM;
    int *regs;
};

int arrayCtor (CPU *processor, char *str);
int funcDef (CPU *processor);
int CPUCtor (CPU *processor);
int add (Stack *stk);
int sub (Stack *stk);
int mul (Stack *stk);
int div (Stack *stk);
int out (Stack *stk);

#endif