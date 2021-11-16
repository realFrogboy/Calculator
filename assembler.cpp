#include "assembler.h"

int main ()
{
    FILE *input, *output;

    input = fopen ("commands.txt", "rb");
    ERROR_INFO(input == NULL, "Can't open input file\n");

    output = fopen ("code.txt", "wb");
    ERROR_INFO(output == NULL, "Can't open output file\n");

    char *str = transform_file_to_str (input);

    /*int num = 0;
    while (str[num] != '\0'){
        printf("%c", str[num]);
        num++;
    }
    printf("%c", str[num]);*/

    convertFuncIntoNumber (str, output);

    free (str);
    fclose (input); fclose (output);

    return 0;
}