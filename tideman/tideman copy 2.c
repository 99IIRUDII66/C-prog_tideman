#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

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

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

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

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])  // ranks{1, 0, 2}
{
    // ranks{3, 1, 0, 2}
    // можно обойтись одинарным циклом
    for (int i = 0; i < candidate_count; i++)
        {
            for (int j = i + 1; j < candidate_count; j++)
            {
                // i =< int k =< candidate_count
                // if(j > ranks[k])
                {
                    preferences[ranks[i]][ranks[j]] += 1;
                }
            }
        }
    return;

}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
        for (int i = 0; i < (candidate_count - 1); i++)
        {
            for (int j = i + 1; j < candidate_count; j++)
            {

                if(preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count += 1;
                }

                else if(preferences[i][j] < preferences[j][i])
                {
                    pairs[pair_count].winner = j;
                    pairs[pair_count].loser = i;
                    pair_count += 1;
                }
            }
        }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // pair_count = candidate_count * (candidate_count - 1) / 2; /// check

    for (int i = 0; i < pair_count; i++)
        {
            int max = 0;// see previous function
            int temp_winner, temp_loser;
            for (int j = i; j < pair_count; j++)
            {
                if (preferences[pairs[j].winner][pairs[j].loser] > max)
                {
                    max = preferences[pairs[j].winner][pairs[j].loser];

                    temp_winner = pairs[i].winner;
                    pairs[i].winner = pairs[j].winner;
                    pairs[j].winner = temp_winner;

                    temp_loser = pairs[i].loser;
                    pairs[i].loser = pairs[j].loser;
                    pairs[j].loser = temp_loser;
                }
                    // // Function to swap two elements
                    // void swap(int* a, int* b)
                    // {
                    //     int temp = *a;
                    //     *a = *b;
                    //     *b = temp;
                    // }
            }

         }
    return;

}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{

    // bool matched = true;
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < pair_count; j++)
    //     {
    //         if(i == pairs[j].loser)
    //         {
    //            break;
    //         }
    //         else
    //         {
    //             matched = false;
    //         }
    //     }
    //     if(matched == false)
    //     {
    //         break;
    //     }
    // }

    // if (matched == false) // в случае если не нашли всех кандидатов в
    // {
    //     for (int i = 0; i < pair_count; i++)
    //     {
    //         locked[pairs[i].winner][pairs[i].loser] = true;
    //     }
    // }


    // else
    // {
    //     // int cycle_element = pairs[candidate_count - 1].loser;
    //     int cycle_element = 0;


    //     for (int i = candidate_count - 1; i < pair_count; i++)
    //     {
    //         bool cycle = true;
    //         for (int j = 0; j < i; j++)
    //         {
    //             if(pairs[i].loser != pairs[j].loser)
    //             {
    //                 cycle_element = pairs[i].loser;

    //             }
    //             else
    //             {
    //                 cycle = false;
    //             }

    //         }

    //         if(cycle)
    //         {
    //             break;
    //         }
    //     }

// ///////
//         else
//         {

    for (int j = 0; j < pair_count; j++)
    {
        locked[pairs[j].winner][pairs[j].loser] = true;
    }

    int cycle_element = 0;
    for (int i = candidate_count - 1; i < pair_count; i++)
    {
        // bool cycle = true;
        for (int j = 0; j < i; j++)
        {
            if(pairs[i].loser != pairs[j].loser)
            {
                cycle_element = pairs[i].loser;

            }
        }
    }

    for (int j = 0; j < pair_count; j++)
    {
        if(pairs[j].loser == pairs[cycle_element].loser)
        locked[pairs[j].winner][pairs[j].loser] = false;
    }



    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool matched = true;

        for (int j = 0; j < candidate_count; j++)
        {
            if(locked[j][i] == true)
            {
               matched = false;
               break;
            }
        }
        if (matched) // в случае совпадения
        {
            printf("%s\n", candidates[i]);;
        }

    }
    return;
}
