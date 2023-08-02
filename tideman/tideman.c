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
void print_winner(void);
bool cycle(int i, int j, int k);

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
    int i;
    for (i = 0; i < candidate_count; i++)
    {
        if (!strcmp(name, candidates[i])) // true if they DO match
        {
            ranks[rank] = i; //updates rank if they match; rank is the array of ranks, updates to ith candidate
            return true;
        }
    }
    return false; // if they never matched above
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int i, j;
    for (i = 0; i < candidate_count; i++)
    {
        for (j = i + 1; j < candidate_count; j++) //counts from i + 1 since rank[i] would always beat rank[j]
        {
            preferences[ranks[i]][ranks[j]] += 1; //rank[i] beats rank[j] N times, N is from i+1 to < candidate_count
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int i, j, k = 0;
    for (i = 0; i < candidate_count; i++)
    {
        for (j = i + 1; j < candidate_count; j++)
        {
            // adds pairs where one is preferred over the other
            //preferences[i,j] is how many voters preferred i over j
            if (preferences[i][j] > preferences [j][i])
            {
                pairs[k].winner = i;
                pairs[k].loser = j;
                k = k + 1;
            }
//less than -> makes sure not to include pairs which have equal amount of voters preferring them
            else if (preferences[i][j] < preferences [j][i])
            {
                pairs[k].winner = j;
                pairs[k].loser = i;
                k = k + 1;
            }
        }
    }
    pair_count = k; // k results in pair_count at the end
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int i, j, winner, loser, max, max_index;
    for (i = 0; i < pair_count; i++)
    {
        for (j = i + 1; j < pair_count; j++)
        {
            // basically a version of selection sort
            //finds max value in preferences, indexed by pairs
            //max value means the maximum amount of preferred voters of pairs.winner over pairs.loser
            max = preferences[pairs[i].winner][pairs[i].loser];
            max_index = i;
            if (preferences[pairs[j].winner][pairs[j].loser] > max) //finds max in j loop
            {
                max = preferences[pairs[j].winner][pairs[j].loser]; //updates max
                max_index = j;
            }
            //swaps max pairs.winner over pairs.loser AND pairs[i].winner over pairs[i].loser
            winner = pairs[i].winner;
            loser = pairs[i].loser;
            pairs[i].winner = pairs[max_index].winner;
            pairs[i].loser = pairs[max_index].loser;
            pairs[max_index].winner = winner;
            pairs[max_index].loser = loser;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int i, j;
    bool create_cycle;
    locked[pairs[0].winner][pairs[0].loser] = true; // first pair of sorted pair array is locked automatically;

    for (i = 1; i < pair_count; i++) // starts at 1
    {
        create_cycle = cycle(i - 1, i, i); //recusive call to find cycled
        if (!create_cycle) // not cycled
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int i, j, sum;
    for (j = 0; j < candidate_count; j++)
    {
        sum = 0;
        for (i = 0; i < candidate_count; i++)
        {
            if (locked[i][j] == true)
            {
                sum += 1;
            }
        }
        if (sum == 0)
        {
            printf("%s\n", candidates[j]);
        }
    }
    return;
}


bool cycle(int i, int j, int k)
{
    if (i == 0)
    {
        //pairs[k] is the loser of the pair we're trying to add, this we need to compare, k stays unchanged
        // compare pairs[i].loser == pairs[j].winner to make sure there is a "link" in chain
        //if no link, then cannot be cycled
        if (pairs[i].winner == pairs[k].loser && pairs[i].loser == pairs[j].winner)
        {
            return true; // true if cycle
        }
        else //no link or no match, and this is when i =0, so false is returned
        {
            return false; // false if not cycle, no matches
        }
    }
    // makes sure 1) locked value is locked 2) ith pair winner matches loser from pair we're trying to add, and
    // 3) ith pair loser matched jth pair winner to make sure there is a "link" in the chain
    if (locked[pairs[i].winner][pairs[i].loser] == true && pairs[i].winner == pairs[k].loser && pairs[i].loser == pairs[j].winner)
    {
        return true;
    } // difference here is that the ith pair winner doesn't match loser from pair we're trying to add
    //if doesn't match,  and if there's a link, then the link continues, j becomes i, and i goes down to consider other pairs
    else if (locked[pairs[i].winner][pairs[i].loser] == true && pairs[i].winner != pairs[k].loser && pairs[i].loser == pairs[j].winner)
    {
        j = i;
        i = i - 1;
        return cycle(i, j, k); //keeps calling function, going back in pairs
    }
    else
    {
        i = i - 1;
        return cycle(i, j, k); //keeps calling function, going back in pairs
    }

}