#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Define ASCII values for delimiting characters
#define SPACE 32
#define EXCLAMATION 33
#define PERIOD 46
#define QUESTION 63
#define UPPER_A 65
#define UPPER_Z 90
#define LOWER_A 97
#define LOWER_Z 122

int count_letters(string input);
int count_words(string input);
int count_sentences(string input);

int main(void)
{
    string input = get_string("Text: ");
    int letters = count_letters(input);
    int words = count_words(input);
    int sentences = count_sentences(input);

    float average_letters = (float) letters / words * 100;
    float average_sentences = (float) sentences / words * 100;

    // Coleman-Liau index used
    float index = 0.0588 * average_letters - 0.296 * average_sentences - 15.8;
    int rounded_index = (int) roundf(index);

    if (rounded_index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (rounded_index >= 1 && rounded_index < 16)
    {
        printf("Grade %i\n", rounded_index);
    }
    else
    {
        printf("Grade 16+\n");
    }
}

int count_letters(string input)
{
    int len = strlen(input);
    int counter = 0;

    for (int i = 0; i < len; i++)
    {
        char letter = input[i];
        if ((letter >= UPPER_A && letter <= UPPER_Z) || (letter >= LOWER_A && letter <= LOWER_Z))
        {
            counter++;
        }
    }

    return counter;
}

int count_words(string input)
{
    int len = strlen(input);
    int counter = 1;

    for (int i = 0; i < len; i++)
    {
        char letter = input[i];
        if (letter == SPACE)
        {
            counter++;
        }
    }

    return counter;
}

int count_sentences(string input)
{
    int len = strlen(input);
    int counter = 0;

    for (int i = 0; i < len; i++)
    {
        char letter = input[i];
        if (letter == EXCLAMATION || letter == PERIOD || letter == QUESTION)
        {
            counter++;
        }
    }

    return counter;
}
