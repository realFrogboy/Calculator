#ifndef ALL_HASHS_H_   
#define ALL_HASHS_H_     

#include "templates.h"     
#define T int 
long long TEMPLATE_HASH(Hash,T)(T const *val);
#ifdef T  
#undef T  
#endif  
#define T size_t  
long long TEMPLATE_HASH(Hash,T)(T const *val); 

#endif