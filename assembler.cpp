#include "assembler.h"

int main ()
{
    FILE *input, *output;

    input = fopen ("dis.txt", "rb");
    ERROR_INFO(input == NULL, "Can't open input file\n");

    output = fopen ("code.txt", "wb");
    ERROR_INFO(output == NULL, "Can't open output file\n");

    char *str = transform_file_to_str (input);

    convertFuncIntoNumber (str, output);

    free (str);
    fclose (input); fclose (output);

    return 0;
}