#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do{
    height = get_int("Height: ");
    } while (height < 1 || height > 8);
    int i;
    int num_hashes=1;

    do {

    for (i=0; i< height-num_hashes;i++){
    printf(" ");
    }

    for (i=0; i<num_hashes; i++){
    printf("#");
    }

    printf("  ");

    for (i=0; i< num_hashes;i++){
    printf("#");
    }
    num_hashes += 1;
    printf("\n");

    } while (i < height);
}