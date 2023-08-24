#include <cs50.h>
#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    int digits[16];
    int multipliers[8];
    int non_multipliers[8];
    int counter = 0;
    int multipliers_counter = 0;
    int non_multipliers_counter = 0;
    int sum = 0;

    long number;
    long temp;

    //Input Validation
    do
    {
        number = get_long("Number: ");
        temp = number;
    }
    while (number < 0);

    if (number < 99999999999 || number > 99999999999999999)
    {
        printf("INVALID\n");
    }
    else
    {
        // Get individual digits into the array digits and also count how many there are
        do
        {
            digits[counter] = temp % 10;
            temp /= 10;
            counter++;
        }
        while (temp > 0);

        // Separate the digits which are to be doubled from those that will not be doubled
        for (int i = 0; i < counter; i++)
        {
            if (i % 2 == 0)
            {
                non_multipliers[non_multipliers_counter] = digits[i];
                non_multipliers_counter++;
            }
            else
            {
                multipliers[multipliers_counter] = digits[i];
                multipliers_counter++;
            }
        }

        // Calculate final sum
        for (int i = 0; i < multipliers_counter; i++)
        {
            int element = multipliers[i] * 2;
            do
            {
                sum += element % 10;
                element /= 10;
            }
            while (element > 0);
        }

        for (int i = 0; i < non_multipliers_counter; i++)
        {
            sum += non_multipliers[i];
        }

        // Generate output
        if (sum % 10 == 0)
        {
            if (digits[counter - 1] == 4)
            {
                printf("VISA\n");
            }
            else if (digits[counter - 1] == 3)
            {
                if (digits[counter - 2] == 4 || digits[counter - 2] == 7)
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else if (digits[counter - 1] == 5)
            {
                if (digits[counter - 2] == 1 || digits[counter - 2] == 2 || digits[counter - 2] == 3
                    || digits[counter - 2] == 4 || digits[counter - 2] == 5)
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
        }
        else
        {
            printf("%iINVALID\n", sum);
        }
    }
}