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

const int SIZE_OF_CODE = 200;
const int STACK_SIZE   = 1000;
const int SIZE_OF_RAM  = 50;
const int SIZE_OF_REGS = 5;

struct CPU 
{
    struct Stack *stk;
    char *code;
    int ip;
    int *RAM;
    int *regs;
};

int arrayCtor (CPU *processor, char *str);
int CPUFuncDef (CPU *processor, const char *ptr_line);

int DOFunc (CPU *processor);
int RealizeFunc (CPU *processor, int funcNum, int value, int func);

int CPUCtor (CPU *processor);
int CPUDtor (CPU *processor);

int push (CPU *processor, int value, int func);
int add (Stack *stk);
int sub (Stack *stk);
int mul (Stack *stk);
int div (Stack *stk);
int out (Stack *stk);
int pop (CPU *processor, int value, int func);

unsigned long long pow_mod (unsigned long long n, unsigned long long k);
unsigned long long mult_mod (unsigned long long n, unsigned long long k);

#endif