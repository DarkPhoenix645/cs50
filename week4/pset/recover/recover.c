#include <cs50.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: recover <file name>\n");
        return 1;
    }

    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        printf("The file specified could not be opened.");
        return 1;
    }

    uint8_t *buffer = malloc(BLOCK_SIZE);
    int file_count = 0;
    bool writing = 0;
    char *outfile;
    FILE *outptr;

    while(fread(buffer, 1, BLOCK_SIZE, inptr) == BLOCK_SIZE)
    {
        if (!writing)
        {
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
            {
                goto create_new_stream;
            }
            else
            {
                continue;
            }
        }
        else
        {
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
            {
                fclose(outptr);
                writing = 0;
                file_count++;

                goto create_new_stream;
            }
            else
            {
                goto write;
            }
        }

        create_new_stream:
            outfile = malloc(8);

            if (file_count <= 9) { sprintf(outfile, "00%i.jpg", file_count); }
            else if (file_count <= 99) { sprintf(outfile, "0%i.jpg", file_count); }
            else if (file_count <= 999) { sprintf(outfile, "%i.jpg", file_count); }

            outptr = fopen(outfile, "w");
            if (outptr == NULL)
            {
                printf("Out file could not be opened.");
                return 1;
            }

            free(outfile);

            writing = 1;
            goto write;

        write:
            fwrite(buffer, BLOCK_SIZE, 1, outptr);
    }

    fclose(inptr);
    fclose(outptr);
    free(buffer);
    return 0;
}