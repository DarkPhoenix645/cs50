#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// each of our text files contains 1000 words
#define LISTSIZE 1000

// values for colors and score (EXACT == right letter, right place; CLOSE == right letter, wrong place; WRONG == wrong letter)
#define EXACT 2
#define CLOSE 1
#define WRONG 0

// ANSI color codes for boxed in letters
#define GREEN   "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW  "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED     "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET   "\e[0;39m"

// user-defined function prototypes
string get_guess(int wordsize);
int check_word(string guess, int wordsize, int status[], string choice);
void print_word(string guess, int wordsize, int status[]);

int main(int argc, string argv[])
{
    if (argc == 1)
    {
        printf("Error: wordsize must be either 5, 6, 7, or 8\n");
        return 1;
    }
    else if (argc > 2)
    {
        printf("Error: Too many arguments\n");
        return 1;
    }
    else
    {
        int input = atoi(argv[1]);
        if (input < 5 || input > 8)
        {
            printf("Error: wordsize must be either 5, 6, 7, or 8\n");
            return 1;
        }
    }

    int wordsize = atoi(argv[1]);

    // Open correct file, each file has exactly LISTSIZE words
    char wl_filename[6];
    sprintf(wl_filename, "%i.txt", wordsize);
    FILE *wordlist = fopen(wl_filename, "r");
    if (wordlist == NULL)
    {
        printf("Error opening file %s.\n", wl_filename);
        return 1;
    }

    // Load word file into an array of size LISTSIZE
    char options[LISTSIZE][wordsize + 1];

    for (int i = 0; i < LISTSIZE; i++)
    {
        fscanf(wordlist, "%s", options[i]);
    }

    // Pseudorandomly select a word for this game
    srand(time(NULL));
    string choice = options[rand() % LISTSIZE];

    // Allow one more guess than the length of the word
    int guesses = wordsize + 1;
    bool won = false;
    int number;

    // Print greeting, using ANSI color codes to demonstrate
    printf(GREEN"This is WORDLE50"RESET"\n");
    printf("You have %i tries to guess the %i-letter word I'm thinking of\n", guesses, wordsize);

    // Main game loop, one iteration for each guess
    for (int i = 0; i < guesses; i++)
    {
        string guess = get_guess(wordsize);

        // Array to hold guess status, initially set to zero
        int status[wordsize];
        for (int j = 0; j < wordsize; j++)
        {
            status[i] = 0;
        }

        // Calculate score for the guess
        int score = check_word(guess, wordsize, status, choice);
        printf("Guess %i: ", i + 1);

        // Print the guess
        print_word(guess, wordsize, status);

        // if they guessed it exactly right, set terminate loop
        if (score == EXACT * wordsize)
        {
            won = true;
            number = i + 1;
            break;
        }
    }

    // Print the game's result
    if (won == true)
    {
        printf("You won in %i guesses!\n", number);
    }
    else
    {
        printf("Sorry, you ran all out of guesses!\n");
        printf("Better luck next time!\n");
        printf("%s was the word\n", choice);
    }

    return 0;
}

string get_guess(int wordsize)
{
    string guess = "";
    int len;

    // Ensure users actually provide a guess that is the correct length
    do
    {
        guess = get_string("Input a %i-letter word: ", wordsize);
        len = strlen(guess);
    }
    while (len != wordsize);

    return guess;
}

int check_word(string guess, int wordsize, int status[], string choice)
{
    int score = 0;

    // Compare guess to choice and score points as appropriate, storing points in status
    for (int i = 0; i < wordsize; i++)
    {
        status[i] = 0;
        for (int j = 0; j < wordsize; j++)
        {
            if (guess[i] == choice[i])
            {
                status[i] = EXACT;
                score += EXACT;
                break;
            }
            else if (guess[i] == choice[j] && i != j)
            {
                status[i] = CLOSE;
                score += CLOSE;
            }
        }
    }

    return score;
}

void print_word(string guess, int wordsize, int status[])
{
    // Print word character-for-character with correct color coding, then reset terminal font to normal
    for (int i = 0; i < wordsize; i++)
    {
        if (status[i] == 0)
        {
            printf(RED"%c"RESET, guess[i]);
        }
        else if (status[i] == 1)
        {
            printf(YELLOW"%c"RESET, guess[i]);
        }
        else if (status[i] == 2)
        {
            printf(GREEN"%c"RESET, guess[i]);
        }

    }

    printf("\n");
    return;
}
