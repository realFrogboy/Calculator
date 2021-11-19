#include "calculator.h"

int DEBUG_LEVEL = 3;

/*#define debug_lvl_hash
#define debug_lvl_canaries

#ifdef debug_lvl_hash
    //todo
#endif*/

int main ()
{
    FILE *input_code = NULL;
    FILE *input_com = NULL;
    struct CPU processor;

    CPUCtor (&processor);

    input_code = fopen ("code.txt", "rb");
    ERROR_INFO(input_code == NULL, "Can't open file\n");

    input_com = fopen ("commands.txt", "rb");
    ERROR_INFO(input_com == NULL, "Can't open file\n"); 
    
    struct stat code_info = get_file_info (input_code);
    struct stat com_info  = get_file_info (input_com);

    printf("%ld -- %ld\n", com_info.st_atime, code_info.st_atime);

    if (com_info.st_atime > code_info.st_atime)
    {
        char commands[10] = "";

        strcpy (commands, "./ass");
        system (commands);

        strcpy (commands, "./disass");
        system (commands);
    }

    char *str = transform_file_to_str (input_code);

    arrayCtor (&processor, str);

    while ((int)processor.code[processor.ip])
    {
        DOFunc (&processor);
        /*printf ("================\n");
        prinStack (processor.stk_for_call);
        prinStack (processor.stk);
        printf ("----------------\n");
        printf ("%d\n", processor.ip);
        printf ("&&&&&&&&&&&&&&&&\n");
        for (int num = 0; num < 5; num++)
        {
            printf ("%d,  ", processor.regs[num]);
            printf ("\n");
        }
        printf ("================\n");*/
    }

    CPUDtor (&processor);
    free (str);
    fclose (input_code); 
    fclose (input_com);

    return 0;
}