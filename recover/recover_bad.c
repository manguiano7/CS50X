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

    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        //printf("buffer[1] is %i\n", buffer[0]);
        // first three bytes must be certain values to be jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
        {
            // fourth byte must be in certain range to be jpeg
            if (buffer[3] >= 0xe0  && buffer[3] <= 0xef )
            {
                sprintf(image_name, "%03i.jpg", i);
                FILE *current_image = fopen(image_name, "w");
                //printf("i is %i\n", i);
                fwrite(buffer, 1, sizeof(buffer), current_image);
                //printf("buffer[1] is %i\n", buffer[1]);
                i += 1;
                sum = 0 ;
                for (j=1; j < 20; j++)
                {
                    sum += buffer[BLOCK_SIZE-j];
                }
                if (sum != 0)
                {
                    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
                    {
                        fwrite(buffer, 1, sizeof(buffer), current_image);
                        sum = 0 ;
                        for (j=1; j < 20; j++)
                        {
                            sum += buffer[BLOCK_SIZE-j];
                        }
                        if (sum ==0)
                        {
                            fclose(current_image);
                            break;
                        }
                    }
                }
                else
                {
                    fclose(current_image);
                }
            }
        }
    }

    return 0;
}