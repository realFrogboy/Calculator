#include "calculator.h"

int DEBUG_LEVEL = 3;

/*#define debug_lvl_hash
#define debug_lvl_canaries

#ifdef debug_lvl_hash
    //todo
#endif*/

int main ()
{
    FILE *input = NULL;
    struct CPU processor;

    CPUCtor (&processor);

    input = fopen ("code.txt", "rb");
    ERROR_INFO(input == NULL, "Can't open file\n");

    char *str = transform_file_to_str (input);

    arrayCtor (&processor, str);

    while ((int)processor.code[processor.ip])
    {
        DOFunc (&processor);
        /*printf ("================\n");
        //prinStack (processor.stk_for_call);
        prinStack (processor.stk);
        printf ("----------------\n");
        printf ("%d\n", processor.ip);
        //printf ("&&&&&&&&&&&&&&&&\n");
        //for (int num = 0; num < 5; num++)
        //{
        //    printf ("%d,  ", processor.regs[num]);
        //    printf ("\n");
        //}
        printf ("================\n");*/
    }

    CPUDtor (&processor);
    free (str);
    fclose (input);

    return 0;
}