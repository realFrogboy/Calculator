#include "assembler.h"

int main ()
{
    FILE *input, *output;
    struct stat file_info;

    input = fopen ("dis.txt", "rb");
    ERROR_INFO(input == NULL, "Can't open input file\n");

    output = fopen ("code.txt", "wb");
    ERROR_INFO(output == NULL, "Can't open output file\n");

    int fd = fileno (input);
    ERROR_INFO(fd == -1, "Fileno error\n");

    ERROR_INFO(fstat (fd, &file_info) == -1, "Fstat error\n");

    char *str = (char*) calloc (file_info.st_size, sizeof (char));
    ERROR_INFO(str == NULL,  "Can't alloc meemory\n");

    int nReaded = fread (str, file_info.st_size, sizeof (char), input);
    ERROR_INFO(nReaded == file_info.st_size, "Can't read file\n");

    convertFuncIntoNumber (str, output);

    free (str);
    fclose (input); fclose (output);

    return 0;
}