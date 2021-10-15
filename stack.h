#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h> 


#ifndef LOG_INFO
#define LOG_INFO printf (" %s:%d, IN FUNCTION %s:\n.", __FILE__, __LINE__, __PRETTY_FUNCTION__); 
#endif

#ifndef PUT_CANARY
#define PUT_CANARY *(canary_t*)(st->data - 1) = CANARY; *(canary_t*)(st->data + st->capacity + 1) = CANARY;
#endif

#ifndef BAD_INPUT
#define BAD_INPUT  printf ("Incorrect input!\nPlease, enter a number.\n")
#endif


enum ERRORS
{
    NO_ERRORS                = 0,
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


typedef long long canary_t;

const canary_t CANARY        = 0xBADDCAFE;
const int START_STACK_SIZE   = 80;
const int RESIZE_COEFFICIENT = 2;

const int POISON             = 0xDEADBEEF;


#ifndef STRUCT_STACK
#define STRUCT_STACK
struct Stack
{

    canary_t leftCanary;

    int* data;
    size_t capacity;
    size_t Size;

    canary_t rightCanary;
};
#endif



enum ERRORS stackCtor (Stack* st);
enum ERRORS stackPush (Stack* st, int value);
enum ERRORS stackPop (Stack* st); 
enum ERRORS stackDtor (Stack* st);
enum ERRORS printStack(const Stack* st);
enum ERRORS reallocate (Stack* st, size_t newSize);
void stackDump (int error);
enum ERRORS stackOK (const Stack* st);
int CHECK_ERRORS (const Stack* st);
size_t intHash (int const *input);
#endif