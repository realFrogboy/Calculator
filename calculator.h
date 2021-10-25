#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "stack.h"

const int SIZE_OF_CODE = 100;

struct CPU 
{
    struct Stack stk;
    int *code;
    int ip;
    int *RAM;
    int *regs;
};

int arrayCtor (char *funct, char *str);
int funcDef (Stack *stk, int func_code, int value, int *index);
int sum (Stack *stk);
int sub (Stack *stk);
int mul (Stack *stk);
int div (Stack *stk);

#endif