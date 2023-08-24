#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means that the edge i --> j exists
bool locked[MAX][MAX];

// Each pair has a winner and a loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
int determine_winner(void);
void debug(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of previously locked in pairs if any
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // ranks[i] is voter's ith preference
    int ranks[candidate_count];

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        printf("\n");

        record_preferences(ranks);
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new "valid" vote
// A valid vote is constituted by the input name being in the candidates array
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            // Assign rank chosen by voter for the candidate at the
            // ith position in the candidates array into ranks array
            //
            // Here i denotes the index corresponding to the candidate
            // "name" in the candidates array
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}


void record_preferences(int ranks[])
{
    int indices[candidate_count];

    // The rank of the ith candidate (in the candidates array) is stored at indices[i]
    // This is done for easy comparsion in the next loop
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            indices[ranks[j]] = j;
        }
    }

    // Count the number of voters who prefer i over j
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // If index of i is lower, that implies i has higher preference
            if (indices[i] < indices[j])
            {
                preferences[i][j] += 1;
            }
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;

                // DEBUG
                // printf("winner - %s, loser - %s\n", candidates[pairs[pair_count].winner], candidates[pairs[pair_count].loser]);

                pair_count += 1;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // strength is a 2D array in which the sub-arrays have the format
    // {strength of given pair, index of given pair in the pairs array}
    //  The index of the pair is also stored so that the strength array can be sorted
    int strength[pair_count][2];
    pair pairs_sorted[pair_count];

    for (int i = 0; i < pair_count; i++)
    {
        // The strength of a winner-loser pair is represented by the number of
        // people that prefer the winner over the loser
        strength[i][0] = preferences[pairs[i].winner][pairs[i].loser];
        strength[i][1] = i;
    }

    // Implements bubble sort
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 1; j < pair_count; j++)
        {
            if (strength[j][0] > strength[j - 1][0])
            {
                int temp[2];

                // Swap the arrays if strength of the succeeding array is greater
                // Pairs with greater strength have lower indices
                for (int k = 0; k < 2; k++)
                {
                    temp[k] = strength[j - 1][k];
                }

                for (int k = 0; k < 2; k++)
                {
                    strength[j - 1][k] = strength[j][k];
                }

                for (int k = 0; k < 2; k++)
                {
                    strength[j][k] = temp[k];
                }
            }
        }
    }

    // The following creates a temp array pairs_sorted as the actual pairs are not stored
    // in the strength sub-array, only the index of that pair in pairs array is stored
    for (int i = 0; i < pair_count; i++)
    {
        pairs_sorted[i] = pairs[strength[i][1]];

        // DEBUG
        // printf("strength - %i, winner - %i, loser - %i\n", strength[i][0], pairs_sorted[i].winner, pairs_sorted[i].loser);
    }

    for (int i = 0; i < pair_count; i++)
    {
        pairs[i] = pairs_sorted[i];
    }

    // DEBUG
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("%i %i\n", strength[i][0], strength[i][1]);
    // }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int curr_winner;

    for (int i = 0; i < pair_count; i++)
    {
        curr_winner = determine_winner();

        // Passes if the endpoint of new edge is the current winner
        // and we are on the last iteration
        if (pairs[i].loser == curr_winner && i == pair_count - 1)
        {
            bool cycle = false;

            for (int j = 0; j < candidate_count; j++)
            {
                /**/

                bool condition = (pairs[i].winner == j) ||
                                 (locked[j][j + 1] && j < candidate_count - 1) ||
                                 (j == candidate_count - 1 && locked[j][0]);

                if (condition)
                {
                    if (j == candidate_count - 1) { cycle = true; }
                    else { continue; }
                }
                else { break; }
            }

            if (cycle) { continue; }
            else { locked[pairs[i].winner][pairs[i].loser] = true; }
        }
        else { locked[pairs[i].winner][pairs[i].loser] = true; }
    }

    debug();

    return;
}

// Locates the source of the current graph as in locked arr (if any)
// and returns index (in candidates array) of winner
// Returns -1 if no winner present
int determine_winner(void)
{
    int winner = -1;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == 1) { break; }
            else if (locked[i][j] == 1)
            {
                for (int k = 0; k < candidate_count; k++)
                {
                    // printf("%i, %i, %i\n", i, j, k);

                    if (locked[k][i] == 1) { break; }
                    else if (locked[k][i] == 0 && k != candidate_count - 1) { continue; }
                    else if (locked[k][i] == 0 && k == candidate_count - 1) { winner = i; continue; }
                    else { continue; }
                }

                break;
            }
        }

        continue;
    }

    return winner;
}

void debug(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            printf("i - %i, j - %i, locked[i][j] - %i\n", i, j, locked[i][j]);
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    int winner = determine_winner();
    (winner < 0) ? printf("Winner could not be determined.\n") : printf("%s\n", candidates[winner]);
    return;
}

/* CYCLIC EXAMPLE ==> C WINNER
A   A   A   B   B   C   C   C   C
B   B   B   C   C   A   A   A   A
C   C   C   A   A   B   B   B   B
*/


/* NON CYCLIC EXAMPLE ==> A WINNER
A   A   B   B   B   C   C   C   C
C   C   A   A   A   A   A   A   A
B   B   C   C   C   B   B   B   B
*/

