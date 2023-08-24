#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int start;
    int end;
    int years = 0;

    do
    {
        start = get_int("Start Size:");
    }
    while (start < 9);

    do
    {
        end = get_int("End Size:");
    }
    while (start > end);

    while (start < end)
    {
        start = start + (start/3) - (start/4);
        years++;
    }

    printf("Years: %i\n", years);
}
