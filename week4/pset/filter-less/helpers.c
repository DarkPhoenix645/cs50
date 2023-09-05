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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
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
            float sepiaR, sepiaG, sepiaB;
            sepiaR = .393 * copy[i][j].rgbtRed + .769 * copy[i][j].rgbtGreen + .189 * copy[i][j].rgbtBlue;
            sepiaG = .349 * copy[i][j].rgbtRed + .686 * copy[i][j].rgbtGreen + .168 * copy[i][j].rgbtBlue;
            sepiaB = .272 * copy[i][j].rgbtRed + .534 * copy[i][j].rgbtGreen + .131 * copy[i][j].rgbtBlue;

            if (sepiaR > 255) { sepiaR = 255; };
            if (sepiaG > 255) { sepiaG = 255; };
            if (sepiaB > 255) { sepiaB = 255; };

            image[i][j].rgbtRed = (int) round(sepiaR);
            image[i][j].rgbtGreen = (int) round(sepiaG);
            image[i][j].rgbtBlue = (int) round(sepiaB);
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

// Blur image
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
