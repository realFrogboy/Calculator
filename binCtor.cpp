#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main ()
{
    FILE *input = NULL, *output = NULL;
    input = fopen ("code.txt", "r"); output = fopen ("code11.bin", "wb");

    char c = '\0';
    while ((c = getc (input)) != EOF)
    {
        fputc (c, output);
    }


    fclose (input); fclose (output);

    return 0;
}