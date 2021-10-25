#include "stack.h"
#include "all_hashs.h"

long long hash_data = 0;
long long hash_capacity = 0;
long long hash_size = 0;

extern int DEBUG_LEVEL;

ERRORS stackCtor (Stack* st)
{
    ERROR_INFO(st == 0, "ERROR: Void ptr on stack\n");

    st->capacity = START_STACK_SIZE; 
    st->Size = 0;

    st->data = (int*) calloc (st->capacity + 4, sizeof (int));
    ERROR_INFO(st->data == NULL, "ERROR: Can't alloc memory\n");

    st->data++;

    st->leftCanary = CANARY; 
    st->rightCanary = CANARY;
    PUT_CANARY;

    CALC_HASH;

    IS_STACK_OK;

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


ERRORS stackPush (Stack* st, int value)
{
    IS_STACK_OK;

    if (st->Size >= st->capacity)
        reallocate (st, st->capacity * RESIZE_COEFFICIENT); //wrong

    st->Size++;
    *(st->data + st->Size) = value;
 
    CALC_HASH;

    IS_STACK_OK;

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


ERRORS stackPop (Stack* st)
{
    IS_STACK_OK;

    if (st->Size <= st->capacity/RESIZE_COEFFICIENT) 
        reallocate (st, st->capacity/RESIZE_COEFFICIENT);
    
    st->data[st->Size] = POISON;
    
    --st->Size;

    CALC_HASH;

    IS_STACK_OK;

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


ERRORS stackDtor (Stack* st)
{
    IS_STACK_OK;

    free (--st->data);
    st->Size = -1;

    return NO_ERRORS;
}


//-----------------------------------------------------------------------------


ERRORS reallocate (Stack* st, const size_t newSize) //static
{
    IS_STACK_OK;

    st->capacity = newSize;
    st->data--;

    int *tmp = (int*) realloc (st->data, (st->capacity + 4) * sizeof(int));
    if (tmp != NULL)
    {   
        st->data = tmp;
        st->data++;
    }
    else
        ERROR_INFO(tmp == NULL, "ERROR: Can't realloc memory\n");

    PUT_CANARY;

    CALC_HASH;

    IS_STACK_OK;

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
        
        case CAPACITY_HASH_ERROR:
            printf ("\t\tERROR CODE: Capacity's hash changed\n");
            break;
        
        case SIZE_HASH_ERROR:
            printf ("\t\tERROR CODE: Size's hash changed\n");
            break;

        case POISON_ERROR:
            printf ("\t\tERROR CODE: Poision number changed\n");
            break;
        
        case MEMSET_ERROR:
            printf ("\t\tERROR CODE: Can't memset memory\n");
            break;

    } //to write
}


//-----------------------------------------------------------------------------


ERRORS stackOK (const Stack* st)
{
    if ((DEBUG_LEVEL == 1) || (DEBUG_LEVEL == 2) || (DEBUG_LEVEL == 3))
    {
        if (!st) return VOID_STACK;                                               
        if (st->capacity < st->Size) return STACK_OVERFLOW;
    }

    if ((DEBUG_LEVEL == 2) || (DEBUG_LEVEL == 3))
    {
        if (*(canary_t*)(st->data - 1) != CANARY) return DATA_CANARY_LEFT_ERROR;                 
        if (*(canary_t*)(st->data + st->capacity + 1) != CANARY) return DATA_CANARY_RIGHT_ERROR; 
        if (st->leftCanary != CANARY) return STACK_CANARY_LEFT_ERROR;             
        if (st->rightCanary != CANARY) return STACK_CANARY_RIGHT_ERROR;
    }

    if (DEBUG_LEVEL == 3) {
        if (TEMPLATE_HASH(Hash, int) (st->data) != hash_data) return DATA_HASH_ERROR;
        if (TEMPLATE_HASH(Hash, size_t) (&st->capacity) != hash_capacity) return CAPACITY_HASH_ERROR;
        if (TEMPLATE_HASH(Hash, size_t) (&st->Size) != hash_size) return SIZE_HASH_ERROR;
    }

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


ERRORS printStack (const Stack* st)
{
    IS_STACK_OK;

    for (unsigned num = st->Size; num >= 1; num--)
    {
        printf ("%d\n", *(st->data + num));
    }

    printf ("%ld --- %ld\n", st->Size, st->capacity);

    CALC_HASH;

    IS_STACK_OK;

    return NO_ERRORS;
}
