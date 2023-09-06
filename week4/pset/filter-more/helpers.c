#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            // Use double then round to int for accurate values
            double avg_double = (red + green + blue) / 3.00;
            int avg = round(avg_double);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // The RGBTRIPLE datatype is defined in bmp.h and stores the individual channel values for any given pixel
            RGBTRIPLE *p1 = &image[i][j];
            RGBTRIPLE *p2 = &image[i][width - j - 1];

            RGBTRIPLE temp = *p1;
            *p1 = *p2;
            *p2 = temp;
        }
    }

    return;
}

// Blur image using box blur method
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int count = 0;
            int aveR, aveG, aveB;
            aveR = aveG = aveB = 0;

            // Checking all pixels in a 3x3 grid around any given pixel
            for (int k = (i - 1); k <= (i + 1); k++)
            {
                for (int l = (j - 1); l <= (j + 1); l++)
                {
                    // Ignores out of bounds indices
                    if ((k >= 0 && k <= (height - 1)) && (l >= 0 && l <= (width - 1)))
                    {
                        aveR += copy[k][l].rgbtRed;
                        aveG += copy[k][l].rgbtGreen;
                        aveB += copy[k][l].rgbtBlue;
                        count++;
                    }
                }
            }

            image[i][j].rgbtRed = (int) round((double)aveR / (double)count);
            image[i][j].rgbtGreen = (int) round((double)aveG / (double)count);
            image[i][j].rgbtBlue = (int) round((double)aveB / (double)count);
        }
    }

    return;
}

// Detect edges using the Sobel algorithm
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // The following kernels are as defined in the Sobel algorithm
    const int GxKernel[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    const int GyKernel[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // Using doubles for precision
            double GxR, GxG, GxB;
            double GyR, GyG, GyB;
            double finalR, finalG, finalB;
            GxR = GxG = GxB = GyR = GyG = GyB = 0;

            // Checking the 3x3 grid around any given pixel
            for (int offsetY = -1; offsetY < 2; offsetY++)
            {
                for (int offsetX = -1; offsetX< 2; offsetX++)
                {
                    // Skip pixel if it's index is out of bounds
                    bool row_out_of_bounds = offsetY + row < 0 || offsetY + row > height;
                    bool col_out_of_bounds = offsetX + col < 0 || offsetX + col > width;

                    if (row_out_of_bounds || col_out_of_bounds) 
                    { 
                        continue; 
                    }

                    GxR += image[offsetY + row][offsetX + col].rgbtRed * GxKernel[offsetY + 1][offsetX + 1];
                    GxG += image[offsetY + row][offsetX + col].rgbtGreen * GxKernel[offsetY + 1][offsetX + 1];
                    GxB += image[offsetY + row][offsetX + col].rgbtBlue * GxKernel[offsetY + 1][offsetX + 1];

                    GyR += image[offsetY + row][offsetX + col].rgbtRed * GyKernel[offsetY + 1][offsetX + 1];
                    GyG += image[offsetY + row][offsetX + col].rgbtGreen * GyKernel[offsetY + 1][offsetX + 1];
                    GyB += image[offsetY + row][offsetX + col].rgbtBlue * GyKernel[offsetY + 1][offsetX + 1];
                }
            }

            finalR = sqrt(GxR * GxR + GyR * GyR);
            finalG = sqrt(GxG * GxG + GyG * GyG);
            finalB = sqrt(GxB * GxB + GyB * GyB);

            // Cap pixel value to 255
            if (finalR > 255) { finalR = 255; }
            if (finalG > 255) { finalG = 255; }
            if (finalB > 255) { finalB = 255; }

            copy[row][col].rgbtRed = (int) round(finalR);
            copy[row][col].rgbtGreen = (int) round(finalG);
            copy[row][col].rgbtBlue = (int) round(finalB);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    return;
}
