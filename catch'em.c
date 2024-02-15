#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t BYTE;
#define block 512

void checkBlock(BYTE *buffer, FILE **file, int *counter);

int main(int argc, char *argv[])
{
    // Checks correct usage.
    if (argc != 2)
    {
        printf("Correct usage: -./recover infile-\n");
        return 1;
    }

    char *infile = argv[1];

    // Open file and checks if it exists.
    FILE *recovering = fopen(infile, "r");
    if (recovering == NULL)
    {
        printf("The \"infile\" file can't open..\n");
        return 1;
    }

    // Buffer and out-file creation.
    BYTE buffer[block];
    FILE *file = NULL;
    int counter = 0;

    // Loop for read all memory and write all JPEG files.
    while (fread(buffer, 1, block, recovering) == block)
    {
        checkBlock(buffer, &file, &counter);
    }

    if (file != NULL)
    {
        fclose(file);
    }

    // File closes.
    fclose(recovering);
    return 0;
}

void checkBlock(BYTE *buffer, FILE **file, int *counter)
{
    // Checking JPEG conditions.
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        if (*file != NULL)
        {
            fclose(*file);
        }

        // File's name creation and counter of files.
        char fileName[9];
        sprintf(fileName, "%03i.jpg", *counter);
        (*counter)++;

        // File's open and checks if works.
        *file = fopen(fileName, "w");
        if (*file == NULL)
        {
            printf("The \"file\" file cant't open.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Copying blocks of buffer into each JPEG file.
    if (*file != NULL)
    {
        fwrite(buffer, 1, block, *file);
    }
}
