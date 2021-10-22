#ifdef T    
#include "templates.h"    
long long TEMPLATE_HASH(Hash,T) (T const *val)  
{            
    const int ret_size = 32;
    long long ret = 0x555555;
    const int per_char = 7;

    while (*val)
    {
        ret ^=*val++;
        ret = ((ret << per_char) | (ret >> (ret_size -per_char)));
    }

    return ret;  
}   
#endif