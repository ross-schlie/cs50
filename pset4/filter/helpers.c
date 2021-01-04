#include "helpers.h"
#include <cs50.h>
#include <stdio.h>
#include <math.h>

typedef struct
{
    int offsetY;
    int offsetX;
}
BOXOFFSET;

typedef struct
{
    int Gy;
    int Gx;
}
BOXKERNELS;

BYTE pixelAvg(BYTE blue, BYTE green, BYTE red);
bool isWithinFrame(int y, int x, int maxY, int maxX);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixel;
    BYTE avgColor;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            pixel = image[y][x];
            // printf("DEBUG BLUE VALUE = %i \n", pixel.rgbtBlue);
            // printf("DEBUG GREEN VALUE = %i \n", pixel.rgbtGreen);
            // printf("DEBUG RED VALUE = %i \n", pixel.rgbtRed);
            avgColor = pixelAvg(pixel.rgbtBlue, pixel.rgbtGreen, pixel.rgbtRed);
            // printf("DEBUG AVGD VALUE = %i \n", avgColor);
            image[y][x].rgbtBlue = avgColor;
            image[y][x].rgbtGreen = avgColor;
            image[y][x].rgbtRed = avgColor;
        }
    }

    return;
}

//average of blue, green and red for grayscale
BYTE pixelAvg(BYTE blue, BYTE green, BYTE red)
{
    float average = ((float) blue + (float) green + (float) red) / 3;
    return (BYTE) round(average);
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflection[height][width];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            reflection[y][x].rgbtBlue = image[y][width - x - 1].rgbtBlue;
            reflection[y][x].rgbtGreen = image[y][width - x - 1].rgbtGreen;
            reflection[y][x].rgbtRed = image[y][width - x - 1].rgbtRed;
        }
    }

    //change the point for image to point to reflection instead.
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            image[y][x].rgbtBlue = reflection[y][x].rgbtBlue;
            image[y][x].rgbtGreen = reflection[y][x].rgbtGreen;
            image[y][x].rgbtRed = reflection[y][x].rgbtRed;
        }
    }

    return;
}

//determeine if cooordinates are inside image
bool isWithinFrame(int y, int x, int maxY, int maxX)
{
    if (x < 0 || y < 0)
    {
        return false;
    }

    //off by 1?
    if (x > (maxX - 1) || y > (maxY - 1))
    {
        return false;
    }

    return true;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurredImg[height][width];

    //create the blur box grid offsets
    int offsets = 9;
    BOXOFFSET boxOffsets[offsets];
    //top left
    boxOffsets[0].offsetY = -1;
    boxOffsets[0].offsetX = -1;
    //top
    boxOffsets[1].offsetY = -1;
    boxOffsets[1].offsetX = 0;
    //top right
    boxOffsets[2].offsetY = -1;
    boxOffsets[2].offsetX = 1;
    //left
    boxOffsets[3].offsetY = 0;
    boxOffsets[3].offsetX = -1;
    //center
    boxOffsets[4].offsetY = 0;
    boxOffsets[4].offsetX = 0;
    //right
    boxOffsets[5].offsetY = 0;
    boxOffsets[5].offsetX = 1;
    //bottom left
    boxOffsets[6].offsetY = 1;
    boxOffsets[6].offsetX = -1;
    //bottom
    boxOffsets[7].offsetY = 1;
    boxOffsets[7].offsetX = 0;
    //bottom right
    boxOffsets[8].offsetY = 1;
    boxOffsets[8].offsetX = 1;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int coordsY = 0;
            int coordsX = 0;

            int boxesToAvg = 0;

            long blue = 0;
            long green = 0;
            long red = 0;

            float blueAvg = 0;
            float greenAvg = 0;
            float redAvg = 0;

            //iterate over the offsets in the 'box'
            for (int i = 0; i < offsets; i++)
            {
                coordsY = boxOffsets[i].offsetY + y;
                coordsX = boxOffsets[i].offsetX + x;

                //only include those within the img (if off the edges, skip)
                if (isWithinFrame(coordsY, coordsX, height, width))
                {
                    blue += image[coordsY][coordsX].rgbtBlue;
                    green += image[coordsY][coordsX].rgbtGreen;
                    red += image[coordsY][coordsX].rgbtRed;
                    boxesToAvg++;
                }
                // else
                // {
                //     printf("DEBUG out of frame x %i , y %i max x %i max y %i \n", coordsX, coordsY, width, height);
                // }
            }

            blueAvg = round((float) blue / (float) boxesToAvg);
            greenAvg = round((float) green / (float) boxesToAvg);
            redAvg = round((float) red / (float) boxesToAvg);

            blurredImg[y][x].rgbtBlue = blueAvg;
            blurredImg[y][x].rgbtGreen = greenAvg;
            blurredImg[y][x].rgbtRed = redAvg;
        }
    }

    //change the point for image to point to blurred instead.
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            image[y][x].rgbtBlue = blurredImg[y][x].rgbtBlue;
            image[y][x].rgbtGreen = blurredImg[y][x].rgbtGreen;
            image[y][x].rgbtRed = blurredImg[y][x].rgbtRed;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE sobelImg[height][width];

    //create the box grid offsets
    int offsets = 9;
    BOXOFFSET boxOffsets[offsets];
    BOXKERNELS kernels[offsets];
    //top left
    boxOffsets[0].offsetY = -1;
    boxOffsets[0].offsetX = -1;
    kernels[0].Gy = -1;
    kernels[0].Gx = -1;
    //top
    boxOffsets[1].offsetY = -1;
    boxOffsets[1].offsetX = 0;
    kernels[1].Gy = -2;
    kernels[1].Gx = 0;
    //top right
    boxOffsets[2].offsetY = -1;
    boxOffsets[2].offsetX = 1;
    kernels[2].Gy = -1;
    kernels[2].Gx = 1;
    //left
    boxOffsets[3].offsetY = 0;
    boxOffsets[3].offsetX = -1;
    kernels[3].Gy = 0;
    kernels[3].Gx = -2;
    //center
    boxOffsets[4].offsetY = 0;
    boxOffsets[4].offsetX = 0;
    kernels[4].Gy = 0;
    kernels[4].Gx = 0;
    //right
    boxOffsets[5].offsetY = 0;
    boxOffsets[5].offsetX = 1;
    kernels[5].Gy = 0;
    kernels[5].Gx = 2;
    //bottom left
    boxOffsets[6].offsetY = 1;
    boxOffsets[6].offsetX = -1;
    kernels[6].Gy = 1;
    kernels[6].Gx = -1;
    //bottom
    boxOffsets[7].offsetY = 1;
    boxOffsets[7].offsetX = 0;
    kernels[7].Gy = 2;
    kernels[7].Gx = 0;
    //bottom right
    boxOffsets[8].offsetY = 1;
    boxOffsets[8].offsetX = 1;
    kernels[8].Gy = 1;
    kernels[8].Gx = 1;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int coordsX = 0;
            int coordsY = 0;

            int tmpBlue = 0;
            int tmpGreen = 0;
            int tmpRed = 0;

            long blueX = 0;
            long greenX = 0;
            long redX = 0;

            long blueY = 0;
            long greenY = 0;
            long redY = 0;

            float blue = 0;
            float green = 0;
            float red = 0;

            //iterate over the offsets in the 'box'
            for (int i = 0; i < offsets; i++)
            {
                coordsY = boxOffsets[i].offsetY + y;
                coordsX = boxOffsets[i].offsetX + x;

                //There are many ways to handle pixels at the edge, but for the purposes of this problem,
                //  we’ll ask you to treat the image as if there was a 1 pixel solid black border around the edge of the image:
                //  therefore, trying to access a pixel past the edge of the image should be treated as
                //  a solid black pixel (values of 0 for each of red, green, and blue)
                if (isWithinFrame(coordsY, coordsX, height, width))
                {
                    tmpBlue = image[coordsY][coordsX].rgbtBlue;
                    tmpGreen = image[coordsY][coordsX].rgbtGreen;
                    tmpRed = image[coordsY][coordsX].rgbtRed;
                }
                else
                {
                    tmpBlue = 0;
                    tmpGreen = 0;
                    tmpRed = 0;
                }

                // take the original red values for the nine pixels that form a 3x3 box around the pixel,
                //  multiply them each by the corresponding value in the Gx kernel, and take the sum of the resulting values
                blueX += kernels[i].Gx * tmpBlue;
                greenX += kernels[i].Gx * tmpGreen;
                redX += kernels[i].Gx * tmpRed;

                blueY += kernels[i].Gy * tmpBlue;
                greenY += kernels[i].Gy * tmpGreen;
                redY += kernels[i].Gy * tmpRed;
            }

            //sqrt of Gx^2 + Gy^2
            blue = pow(blueX, 2) + pow(blueY, 2);
            green = pow(greenX, 2) + pow(greenY, 2);
            red = pow(redX, 2) + pow(redY, 2);

            blue = round(sqrt(blue));
            green = round(sqrt(green));
            red = round(sqrt(red));

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

            sobelImg[y][x].rgbtBlue = blue;
            sobelImg[y][x].rgbtGreen = green;
            sobelImg[y][x].rgbtRed = red;
        }
    }

    //change the point for image to point to soebel-ified instead.
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            image[y][x].rgbtBlue = sobelImg[y][x].rgbtBlue;
            image[y][x].rgbtGreen = sobelImg[y][x].rgbtGreen;
            image[y][x].rgbtRed = sobelImg[y][x].rgbtRed;
        }
    }

    return;
}