#include "templates.h"  
#include "all_hashs.h"
#ifdef T 
#undef T  
#endif  
#define T int  
#include "hash_temp.cpp"    
#ifdef T  
#undef T  
#endif  
#define T size_t  
#include "hash_temp.cpp"  