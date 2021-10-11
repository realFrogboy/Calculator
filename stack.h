#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>


#ifndef LOG_INFO
#define LOG_INFO printf (" %s:%d, IN FUNCTION %s:\n.", __FILE__, __LINE__, __PRETTY_FUNCTION__); 
#endif


#ifndef BAD_INPUT
#define BAD_INPUT  printf ("Incorrect input!\nPlease, enter a number.\n")
#endif


enum ERRORS
{
    NO_ERRORS                = 0,
    TEST_FAILD               = 1,
    STACK_UNDERFLOW          = 10101,
    STACK_OVERFLOW           = 10201,
    ALLOC_ERROR              = 10301,
    REALLOC_ERROR            = 10401,
    VOID_STACK               = 10501,
    DATA_CANARY_LEFT_ERROR   = 10601,
    DATA_CANARY_RIGHT_ERROR  = 10701,
    STACK_CANARY_LEFT_ERROR  = 10801,
    STACK_CANARY_RIGHT_ERROR = 10901,
    DATA_HASH_ERROR          = 101001,
    POISON_ERROR             = 101101,
    MEMSET_ERROR             = 101201
};


//typedef long canary_t;


const int CANARY             = 0xBADDCAFE;
const int START_STACK_SIZE   = 2;
const int RESIZE_COEFFICIENT = 2;

const int POISON             = 0xDEADBEEF;



struct Stack
{

    int leftCanary;

    int* data;
    size_t capacity;
    size_t Size;

    int rightCanary;
};



int stackCtor (Stack* st);
int stackPush (Stack* st, int value);
int stackPop (Stack* st); 
int stackDtor (Stack* st);
int printStack(const Stack* st);
int reallocate (Stack* st, size_t newSize);
void stackDump (int error);
void cleanBuffer ();
int stackOK (const Stack* st);
int CHECK_ERRORS (const Stack* st);
size_t intHash (int const *input);
void getValue (const char *str, int *val);


const int NUM_OF_TESTS = 8;
int RunUnitTest ();


#endif
