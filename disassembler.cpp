#include "assembler.h"

int main ()
{
    FILE *input, *output;

    input = fopen ("code.txt", "rb");
    ERROR_INFO(input == NULL, "Can't open input file\n");

    output = fopen ("disass.txt", "wb");
    ERROR_INFO(output == NULL, "Can't open output file\n");

    char *str = transform_file_to_str (input);

    convertNumberIntoFunc (str, output);

    free (str);
    fclose (input); fclose (output);

    return 0;
}