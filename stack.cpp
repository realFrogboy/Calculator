#include "stack.h"

size_t hash_value = 0;
extern int DEBUG_LEVEL; 

enum ERRORS stackCtor (Stack* st)
{
    if (st == 0)
    {
        LOG_INFO;
        stackDump (VOID_STACK);
    }

    st->capacity = START_STACK_SIZE; st->Size = 0;

    st->data = (int*) calloc (st->capacity + 4, sizeof (int));
    if (st->data == NULL) 
    {    
        LOG_INFO;
        stackDump (ALLOC_ERROR);
    }

    st->data = st->data + 1;

    st->leftCanary = CANARY; st->rightCanary = CANARY;
    PUT_CANARY;

    hash_value = intHash (st->data);

    int error = 0;
    if ((error = CHECK_ERRORS (st)) != 0) LOG_INFO;

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


enum ERRORS stackPush (Stack* st, int value)
{
    int error = 0;   
    if ((error = CHECK_ERRORS (st)) != 0) LOG_INFO;

    if (st->Size >= st->capacity)
        reallocate (st, st->capacity * RESIZE_COEFFICIENT);

    st->Size++;
    *(st->data + st->Size) = value;

    hash_value = intHash (st->data);

    if ((error = CHECK_ERRORS (st)) != 0) LOG_INFO;

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


enum ERRORS stackPop (Stack* st)
{
    int error = 0;
    if ((error = CHECK_ERRORS (st)) != 0) LOG_INFO;

    if ((st->Size <= st->capacity/RESIZE_COEFFICIENT) && (st->Size != START_STACK_SIZE))
        reallocate (st, st->capacity/RESIZE_COEFFICIENT);
    
    st->data[st->Size] = POISON;
    
    --st->Size;

    hash_value = intHash (st->data);

    if ((error = CHECK_ERRORS (st)) != 0) LOG_INFO;

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


enum ERRORS stackDtor (Stack* st)
{
    int error = 0;   
    if ((error = CHECK_ERRORS (st)) != 0) LOG_INFO;

    free (st->data - 1);
    st->Size = -1;

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


enum ERRORS printStack (const Stack* st)
{
    int error = 0;
    if ((error = CHECK_ERRORS (st)) != 0) LOG_INFO;

    for (unsigned num = st->Size; num >= 1; num--)
    {
        printf ("%d\n", *(st->data + num));
    }

    printf ("%ld --- %ld\n", st->Size, st->capacity);

    hash_value = intHash (st->data);

    if ((error = CHECK_ERRORS (st)) != 0) LOG_INFO;

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


enum ERRORS reallocate (Stack* st, size_t newSize)
{
    int error = 0;
    if ((error = CHECK_ERRORS (st)) != 0) LOG_INFO;

    st->capacity = newSize;

    int *ptrBegin = st->data - 1;

    ptrBegin = (int*) realloc (ptrBegin, (st->capacity + 4) * sizeof (int));
    if (ptrBegin == NULL) 
    {
        LOG_INFO;
        stackDump (REALLOC_ERROR);
    }
    
    PUT_CANARY;

    hash_value = intHash (st->data);

    if ((error = CHECK_ERRORS (st)) != 0) LOG_INFO;

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


void stackDump (int error)
{
    switch (error)
    {
        case STACK_UNDERFLOW:
            printf ("\t\tERROR CODE: Stack Underflow\n");
            break;

        case STACK_OVERFLOW:
            printf ("\t\tERROR CODE: Stack Overflow\n");
            break;

        case VOID_STACK:
            printf ("\t\tERROR CODE: Pointer on tack is NULL\n");
            break;
        
        case ALLOC_ERROR:
            printf ("\t\tERROR CODE: Can't alloc meemory\n");
            break;
        
        case REALLOC_ERROR:
            printf ("\t\tERROR CODE: Can't realloc meemory\n");
            break;
        
        case DATA_CANARY_LEFT_ERROR:
            printf ("\t\tERROR CODE: Data's left canary was changed\n");
            break;
        
        case DATA_CANARY_RIGHT_ERROR:
            printf ("\t\tERROR CODE: Data's right canary was changed\n");
            break;
        
        case STACK_CANARY_LEFT_ERROR:
            printf ("\t\tERROR CODE: Stack's left canary was changed\n");
            break;
        
        case STACK_CANARY_RIGHT_ERROR:
            printf ("\t\tERROR CODE: Stack's right canary was changed\n");
            break;
        
        case DATA_HASH_ERROR:
            printf ("\t\tERROR CODE: Data's hash changed\n");
            break;

        case POISON_ERROR:
            printf ("\t\tERROR CODE: Poision number changed\n");
            break;
        
        case MEMSET_ERROR:
            printf ("\t\tERROR CODE: Can't memset memory\n");
            break;

    }
}


//-----------------------------------------------------------------------------


enum ERRORS stackOK (const Stack* st)
{
    if ((DEBUG_LEVEL == 1) || (DEBUG_LEVEL == 2) || (DEBUG_LEVEL == 3))
    {
        if (!st) return VOID_STACK;                                               
        if (st->Size < 0) return STACK_UNDERFLOW;                                 
        if (st->capacity < st->Size) return STACK_OVERFLOW;
    }

    if ((DEBUG_LEVEL == 2) || (DEBUG_LEVEL == 3))
    {
        if (*(canary_t*)(st->data - 1) != CANARY) return DATA_CANARY_LEFT_ERROR;                 
        if (*(canary_t*)(st->data + st->capacity + 1) != CANARY) return DATA_CANARY_RIGHT_ERROR; 
        if (st->leftCanary != CANARY) return STACK_CANARY_LEFT_ERROR;             
        if (st->rightCanary != CANARY) return STACK_CANARY_RIGHT_ERROR;
    }

    if (DEBUG_LEVEL == 3)
        if (intHash (st->data) != hash_value) return DATA_HASH_ERROR;

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


int CHECK_ERRORS (const Stack* st)
{
    int error = 0;
    if ((error = stackOK(st)) != 0)
    {
        stackDump (error);
        return error;
    }

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


size_t intHash (int const *input)
{
    const int ret_size = 32;
    size_t ret = 0x555555;
    const int per_char = 7;

    while (*input)
    {
        ret ^=*input++;
        ret = ((ret << per_char) | (ret >> (ret_size -per_char)));
    }

    return ret;
}