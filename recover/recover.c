#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Please use exactly one image file as input.\n");
        return 1;
    }
    FILE *raw_file = fopen(argv[1], "r");

    if (raw_file == NULL)
    {
        printf("Image file could not be opened for reading.\n");
        return 1;
    }

    typedef uint8_t BYTE;
    BYTE buffer[BLOCK_SIZE];
    int i = 0, j;
    int sum;
    char image_name[] = "###.jpg";
    FILE *current_image;
    int found = 0;
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // first four bytes must be certain values to be jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] >= 0xe0  && buffer[3] <= 0xef)
        {
            sprintf(image_name, "%03i.jpg", i);
            current_image = fopen(image_name, "w");
            fwrite(buffer, 1, sizeof(buffer), current_image);
            i += 1;
            found = 1;
        }
        // means a continuation of the first jpeg
        else if (found == 1)
        {
            fwrite(buffer, 1, sizeof(buffer), current_image);
        }
    }

    return 0;
}