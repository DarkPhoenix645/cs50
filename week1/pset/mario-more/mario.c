#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    int counter = 0;

    do
    {
        height = get_int("Height: ");
    }
    while (height > 8 || height < 1);

    // Make as many rows as the input height
    for (int i = 0; i < height; i++)
    {
        // Space out the characters so that they line up
        for (int j = 0; j < height - i - 1; j++)
        {
            printf(" ");
        }

        // Main pattern
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }

        printf("  ");

        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }

        // Go to next row
        printf("\n");
    }
}