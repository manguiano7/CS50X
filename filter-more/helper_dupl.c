#include "helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;
    int avg;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (image[i][j].rgbtBlue != image[i][j].rgbtGreen || image[i][j].rgbtBlue != image[i][j].rgbtRed)
            {
            avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
            }
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j, newj;
    int dummy;

    for (i = 0; i < height; i++)
    {//reflects only half, and swaps first half with last half
        j = 0;
        do
        {
            newj = width - 1 - j; // -1 is necessary since width
            dummy = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][newj].rgbtBlue;
            image[i][newj].rgbtBlue = dummy;

            dummy = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][newj].rgbtGreen;
            image[i][newj].rgbtGreen = dummy;

            dummy = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][newj].rgbtRed;
            image[i][newj].rgbtRed = dummy;
            j += 1;
        }
        while (j < width / 2);
    }
    return;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j, num_pixels;
    int  blue, green, red;
    RGBTRIPLE(*image_copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        printf("Not enough memory to store image.\n");
    }

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            //makes copy of image
            image_copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            image_copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            image_copy[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }

    for (i = 0; i < height; i++)
    {

        for (j = 0; j < width; j++)
        {

            blue = 0;
            green = 0;
            red = 0;
            num_pixels = 0;

            if (i > 0 && j > 0)
            {
            blue  += image_copy[i-1][j-1].rgbtBlue;
            green += image_copy[i-1][j-1].rgbtGreen;
            red  += image_copy[i-1][j-1].rgbtRed;
            num_pixels += 1;
            }

            if (i > 0)
            {
            blue += image_copy[i-1][j].rgbtBlue;
            green += image_copy[i-1][j].rgbtGreen;
            red += image_copy[i-1][j].rgbtRed;
            num_pixels += 1;
            }

            if (i > 0 && j < width - 1)
            {
            blue += image_copy[i-1][j+1].rgbtBlue;
            green += image_copy[i-1][j+1].rgbtGreen;
            red += image_copy[i-1][j+1].rgbtRed;
            num_pixels += 1;
            }

            if (j > 0)
            {

            blue += image_copy[i][j-1].rgbtBlue;
            green += image_copy[i][j-1].rgbtGreen;
            red += image_copy[i][j-1].rgbtRed;
            num_pixels += 1;
            }

            blue += image_copy[i][j].rgbtBlue;
            green += image_copy[i][j].rgbtGreen;
            red += image_copy[i][j].rgbtRed;
            num_pixels += 1;

            if (j < width - 1)
            {
            blue += image_copy[i][j+1].rgbtBlue;
            green += image_copy[i][j+1].rgbtGreen;
            red += image_copy[i][j+1].rgbtRed;
            num_pixels += 1;
            }

            if (i < height - 1 && j > 0)
            {
            blue += image_copy[i+1][j-1].rgbtBlue;
            green += image_copy[i+1][j-1].rgbtGreen;
            red += image_copy[i+1][j-1].rgbtRed;
            num_pixels += 1;
            }

            if (i < height - 1)
            {
            blue += image_copy[i+1][j].rgbtBlue;
            green += image_copy[i+1][j].rgbtGreen;
            red += image_copy[i+1][j].rgbtRed;
            num_pixels += 1;
            }

            if (i < height - 1 && j < width - 1)
            {
            blue += image_copy[i+1][j+1].rgbtBlue;
            green += image_copy[i+1][j+1].rgbtGreen;
            red += image_copy[i+1][j+1].rgbtRed;
            num_pixels += 1;
            }
            //printf("blue is %i\n", blue);
            //printf("num pixels is %i\n", num_pixels);
            blue = round(1.0*blue / num_pixels);
            green = round(1.0*green / num_pixels);
            red = round(1.0*red / num_pixels);

            image[i][j].rgbtBlue  = blue;
            image[i][j].rgbtGreen = green ;
            image[i][j].rgbtRed  = red;
        }
    }

    free(image_copy);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
