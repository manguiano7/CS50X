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
                //makes RGB values all equal
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
    {
        //reflects only half, and swaps first half with last half
        j = 0;
        do
        {
            newj = width - 1 - j; // -1 is necessary since width is overbounds
            //pairs of pixels are swapped for each color
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
    int i, j, num_pixels, m, n;
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
            //makes copy of image, to compute box blur
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
            //these next two loops iterate over the "box"
            for (m = -1; m <= 1; m++)
            {
                for (n = -1; n <= 1; n++)
                {
                    //makes sure the box is in bounds
                    if (i + m >= 0 && i + m < height && j + n >= 0 && j + n < width)
                    {
                        //additional integer values are necessary since value goes above 255, larger than each element of image
                        blue  += image_copy[i + m][j + n].rgbtBlue;
                        green += image_copy[i + m][j + n].rgbtGreen;
                        red  += image_copy[i + m][j + n].rgbtRed;
                        num_pixels += 1;
                    }
                }
            }
            //introduces float 1.0 and round to calculate accurate average
            image[i][j].rgbtBlue = round(1.0 * blue / num_pixels);
            image[i][j].rgbtGreen = round(1.0 * green / num_pixels);
            image[i][j].rgbtRed = round(1.0 * red / num_pixels);
        }
    }

    free(image_copy);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j, num_pixels, m, n;
    int  blue, green, red, xblue, xgreen, xred, yblue, ygreen, yred;
    //kernels given by problem set
    int xkern[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int ykern[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

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
            xblue = 0;
            xgreen = 0;
            xred = 0;
            yblue = 0;
            ygreen = 0;
            yred = 0;

            num_pixels = 0;
            //iterates over "box" around pixel
            for (m = -1; m <= 1; m++)
            {
                for (n = -1; n <= 1; n++)
                {
                    //makes sure box element is in bounds
                    if (i + m >= 0 && i + m < height && j + n >= 0 && j + n < width)
                    {
                        xblue  += xkern[m + 1][n + 1] * image_copy[i + m][j + n].rgbtBlue;
                        xgreen += xkern[m + 1][n + 1] * image_copy[i + m][j + n].rgbtGreen;
                        xred  +=  xkern[m + 1][n + 1] * image_copy[i + m][j + n].rgbtRed;

                        yblue  += ykern[m + 1][n + 1] * image_copy[i + m][j + n].rgbtBlue;
                        ygreen += ykern[m + 1][n + 1] * image_copy[i + m][j + n].rgbtGreen;
                        yred  +=  ykern[m + 1][n + 1] * image_copy[i + m][j + n].rgbtRed;
                        num_pixels += 1;
                    }
                }
            }
            //calculated Gx and Gy for each color
            blue = round(sqrt(pow(xblue, 2) + pow(yblue, 2)));
            green = round(sqrt(pow(xgreen, 2) + pow(ygreen, 2)));
            red = round(sqrt(pow(xred, 2) + pow(yred, 2)));
            // if over 255, each color is just 255, the max
            if (blue > 255)
            {
                blue = 255;
            }

            if (green > 255)
            {
                green = 255;
            }

            if (red > 255)
            {
                red = 255;
            }

            image[i][j].rgbtBlue = blue;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtRed = red;

        }
    }

    free(image_copy);
    return;
}
