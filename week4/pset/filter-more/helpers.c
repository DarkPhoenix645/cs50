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
        for (int j = 0; j <= width / 2; j++)
        {
            // The RGBTRIPLE datatype is defined in bmp.h and stores the individual channel values for any given pixel
            RGBTRIPLE *p1 = &image[i][j];
            RGBTRIPLE *p2 = &image[i][width - j + 1];

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
    // For easy reference to all three channels of image
    // The channels for pixel in ith row and jth column is stored at channels[i][j] in the order RGB
    int channels[height][width][3];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            channels[i][j][0] = image[i][j].rgbtRed;
            channels[i][j][1] = image[i][j].rgbtGreen;
            channels[i][j][2] = image[i][j].rgbtBlue;
        }
    }

    for (int channel = 0; channel < 3; channel++)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                bool topLeft = (i == 0 && j == 0);
                bool topRight = (i == 0 && j == width - 1);
                bool bottomLeft = (i == height - 1 && j == 0);
                bool bottomRight = (i == height - 1 && j == width - 1);

                bool top = i == 0;
                bool bottom = i == height - 1;
                bool left = j == 0;
                bool right = j == width - 1;

                bool corner =  topLeft || topRight || bottomLeft || bottomRight;
                bool edge = top || bottom || left || right;

                int upLeft  = channels[i - 1][j - 1][channel];
                int upTop   = channels[i - 1][j][channel];
                int upRight = channels[i - 1][j + 1][channel];

                int sameLeft  = channels[i][j - 1][channel];
                int sameRight = channels[i][j + 1][channel];

                int downLeft  = channels[i + 1][j - 1][channel];
                int downLow   = channels[i + 1][j][channel];
                int downRight = channels[i + 1][j + 1][channel];

                if (corner)
                {
                    if (topLeft)
                    {
                        upLeft = 0;
                        upTop = 0;
                        upRight = 0;
                        sameLeft = 0;
                        downLeft = 0;
                    }
                    else if (top-right)
                    {
                        upLeft = 0;
                        upTop = 0;
                        upRight = 0;
                        sameRight = 0;
                        downRight = 0;
                    }
                    else if (bottom-left)
                    {
                        upLeft = 0;
                        sameLeft = 0;
                        downLeft = 0;
                        downLow = 0;
                        downRight = 0;
                    }
                    else
                    {
                        upRight = 0;
                        sameRight = 0;
                        downLeft = 0;
                        downLow = 0;
                        downRight = 0;
                    }
                }
                else if (edge)
                {
                    if (top)
                    {
                        upLeft = 0;
                        upTop = 0;
                        upRight = 0;
                    }
                    else if (bottom)
                    {
                        downLeft = 0;
                        downLow = 0;
                        downRight = 0;
                    }
                    else if (left)
                    {
                        upLeft = 0;
                        sameLeft = 0;
                        downLeft = 0;
                    }
                    else
                    {
                        upRight = 0;
                        sameRight = 0;
                        downRight = 0;
                    }
                }

                double avg_double = (upLeft + upTop + upRight + sameLeft + sameRight + downLeft + downLow + downRight) / 8.00;
                int average = round(avg_double);
                channels[i][j][channel] = average;
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = channels[i][j][0];
            image[i][j].rgbtGreen = channels[i][j][1];
            image[i][j].rgbtBlue = channels[i][j][2];
        }
    }

    return;
}

// Detect edges using the Sobel algorithm
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // For easy reference to all three channels of image
    // The channels for pixel in ith row and jth column is stored at channels[i][j] in the order RGB
    int channels[height][width][3];

    // The following kernels are as defined in the Sobel algorithm
    const int GxKernel[3][3] = {
                                {-1, 0, 1},
                                {-2, 0, 2},
                                {-1, 0, 1}
                               };
    const int GyKernel[3][3] = {
                                {-1, -2, -1},
                                { 0,  0,  0},
                                { 1,  2,  1}
                               };

    // Separate out channels for easy (one by one) computation
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            channels[i][j][0] = image[i][j].rgbtRed;
            channels[i][j][1] = image[i][j].rgbtGreen;
            channels[i][j][2] = image[i][j].rgbtBlue;
        }
    }

    for (int channel = 0; channel < 3; channel++)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                bool topLeft = (i == 0 && j == 0);
                bool topRight = (i == 0 && j == width - 1);
                bool bottomLeft = (i == height - 1 && j == 0);
                bool bottomRight = (i == height - 1 && j == width - 1);

                bool top = i == 0;
                bool bottom = i == height - 1;
                bool left = j == 0;
                bool right = j == width - 1;

                bool corner =  topLeft || topRight || bottomLeft || bottomRight;
                bool edge = top || bottom || left || right;

                int upLeft  = channels[i - 1][j - 1][channel];
                int upTop   = channels[i - 1][j][channel];
                int upRight = channels[i - 1][j + 1][channel];

                int sameLeft  = channels[i][j - 1][channel];
                int sameRight = channels[i][j + 1][channel];

                int downLeft  = channels[i + 1][j - 1][channel];
                int downLow   = channels[i + 1][j][channel];
                int downRight = channels[i + 1][j + 1][channel];

                // For pixels on the edge or corner, the above definitions would point to a non-existent element of the
                // array channels. Thus, we set the problematic pixel values to 0 essentially giving the whole image a 1
                // pixel wide black border. This would have no effect on the edge detection as we have assigned 0
                if (corner)
                {
                    if (topLeft)
                    {
                        upLeft = 0;
                        upTop = 0;
                        upRight = 0;
                        sameLeft = 0;
                        downLeft = 0;
                    }
                    else if (top-right)
                    {
                        upLeft = 0;
                        upTop = 0;
                        upRight = 0;
                        sameRight = 0;
                        downRight = 0;
                    }
                    else if (bottom-left)
                    {
                        upLeft = 0;
                        sameLeft = 0;
                        downLeft = 0;
                        downLow = 0;
                        downRight = 0;
                    }
                    else
                    {
                        upRight = 0;
                        sameRight = 0;
                        downLeft = 0;
                        downLow = 0;
                        downRight = 0;
                    }
                }
                else if (edge)
                {
                    if (top)
                    {
                        upLeft = 0;
                        upTop = 0;
                        upRight = 0;
                    }
                    else if (bottom)
                    {
                        downLeft = 0;
                        downLow = 0;
                        downRight = 0;
                    }
                    else if (left)
                    {
                        upLeft = 0;
                        sameLeft = 0;
                        downLeft = 0;
                    }
                    else
                    {
                        upRight = 0;
                        sameRight = 0;
                        downRight = 0;
                    }
                }

                int upLeftX  = upLeft * GxKernel[0][0];
                int upTopX   = upTop * GxKernel[0][1];
                int upRightX = upRight * GxKernel[0][2];

                int sameLeftX  = sameLeft * GxKernel[1][0];
                int sameRightX = sameRight * GxKernel[1][2];

                int downLeftX  = downLeft * GxKernel[2][0];
                int downLowX   = downLow * GxKernel[2][1];
                int downRightX = downRight * GxKernel[2][2];

                int upLeftY  = upLeft * GyKernel[0][0];
                int upTopY   = upTop * GyKernel[0][1];
                int upRightY = upRight * GyKernel[0][2];

                int sameLeftY  = sameLeft * GyKernel[1][0];
                int sameRightY = sameRight * GyKernel[1][2];

                int downLeftY  = downLeft * GyKernel[2][0];
                int downLowY   = downLow * GyKernel[2][1];
                int downRightY = downRight * GyKernel[2][2];

                int Gx = upLeftX + upTopX + upRightX + sameLeftX + sameRightX + downLeftX + downLowX + downRightX;
                int Gy = upLeftY + upTopY + upRightY + sameLeftY + sameRightY + downLeftY + downLowY + downRightY;

                double vecSum = pow((pow(Gx, 2) + pow(Gy, 2)), 0.5);
                int final = round(vecSum);

                // Cap the pixel channel value to 255
                if (final > 255) { final = 255; }

                channels[i][j][channel] = final;
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = channels[i][j][0];
            image[i][j].rgbtGreen = channels[i][j][1];
            image[i][j].rgbtBlue = channels[i][j][2];
        }
    }

    return;
}
