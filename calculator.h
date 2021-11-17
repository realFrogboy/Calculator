#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "stack.h"

#ifndef OPERATION
#define OPERATION(sign) do{                                                                     \
                            int res = stk->data[stk->Size - 2] sign stk->data[stk->Size - 1];   \
                            stackPop (stk); stackPop (stk);                                     \
                            stackPush (stk, res);                                               \
                            return 0;                                                           \
                        } while (0)
#endif

const int SIZE_OF_CODE    = 1000;   //processor.code size
const int SIZE_OF_RAM     = 50;    //processor.RAM size
const int SIZE_OF_REGS    = 5;     //processor.regs size
const int NUM_OF_LABELS   = 10;    //label[?]

struct CPU 
{
    struct Stack *stk;
    struct Stack *stk_for_call;
    int *label;
    char *code;
    int ip;
    int *RAM;
    int *regs;
};

char* transform_file_to_str (FILE *input);
struct stat get_file_info (FILE *input);

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
int jmp (CPU *processor, int value);
int ja (CPU *processor, int value);
int jae (CPU *processor, int value);
int jb (CPU *processor, int value);
int jbe (CPU *processor, int value);
int je (CPU *processor, int value);
int jne (CPU *processor, int value);
int call (CPU *processor, int value);
int ret (CPU *processor);
int sqrt_ (CPU *processor);

unsigned long long pow_mod (unsigned long long n, unsigned long long k);
unsigned long long mult_mod (unsigned long long n, unsigned long long k);

bool isequal(double a, double b);

#endif