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

//helper functs
int nextBestWinner(int offset, int index, int strenght, bool indexedPairs[]);

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

        // for (int j = 0; j < candidate_count; j++)
        // {
        //     printf("DEBUG rank %i is %d\n", j, ranks[j]);
        // }

        record_preferences(ranks);

        printf("\n");
    }

    //debug
    // for (int i = 0; i < voter_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         printf("Preferences: VOTER %i CANDIDATE %i PREFER? %i \n", i, j, preferences[i][j]);
    //     }
    // }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
// If name is a match for the name of a valid candidate,
//  then you should update the ranks array to indicate that the voter has the candidate as their rank preference
//  (where 0 is the first preference, 1 is the second preference, etc.)
// Recall that ranks[i] here represents the user’s ith preference
// The function should return true if the rank was successfully recorded,
//  and false otherwise (if, for instance, name is not the name of one of the candidates).
// You may assume that no two candidates will have the same name.
bool vote(int rank, string name, int ranks[])
{
    bool ranked = false;
    // printf("DEBUG voting rank %i ranks: %s\n", rank, name);
    for (int i = 0; i < candidate_count; i++)
    {
        // 0 means a match
        if (strcmp(candidates[i], name) == 0)
        {
            //check to see if user is voting/ranking someone already ranked [this would require ranks to be initialized properly]
            // if (ranks[i] == rank)
            // {
            //     printf("DEBUG %s already ranks [%i]: %i\n", name, i, rank);
            //     return false;
            // }

            // printf("DEBUG ranking %s (%i): %i\n", name, i, rank);
            ranks[rank] = i;
            ranked = true;
        }
        else
        {
            // printf("DEBUG candidate %s (%i) did not match, so rank not set \n", candidates[i], i);
        }
    }

    return ranked;
}

// Update preferences given one voter's ranks
// The function is called once for each voter, and takes as argument the ranks array,
//  (recall that ranks[i] is the voter’s ith preference, where ranks[0] is the first preference).
// The function should update the global preferences array to add the current voter’s preferences.
//  Recall that preferences[i][j] should represent the number of voters who prefer candidate i over candidate j.
// You may assume that every voter will rank each of the candidates.
void record_preferences(int ranks[])
{
    // iterate over ranks [i] (since number f ranks = candidate count)
    //  and then iterate over all the lower ranked candidates and record them as [j] being preferenced over
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            //compare the ranks of candidates
            if (ranks[j] == ranks[i])
            {
                // printf("DEBUG ranks[j] = %i and matches candidate [i]  %i corresponding to %s (ignoring) \n", ranks[j], ranks[i], candidates[j]);
            }
            else if (ranks[i] < ranks[j])
            {
                // printf("DEBUG ranks[i] %i (%s) is lower ranked than ranks[j] %i (%s) (recording) \n", ranks[j], candidates[j], ranks[i], candidates[i]);
                // printf("DEBUG recording preference for ith candidate %s (%i) being prefered over jth candidate %s (%i).\n", candidates[i], ranks[i], candidates[j], ranks[j]);
                preferences[i][j]++;
            }
            else
            {
                if (preferences[i][j] > 0)
                {
                    // printf("DEBUG candidate # %i being preferred over # %i so far = %i \n", i, j, preferences[i][j]);
                }
                else
                {
                    preferences[i][j] = 0;
                }

                // else if (ranks[j] < ranks[i])
                // {
                //     // printf("DEBUG ranks[j] %i (%s) is lower ranked than ranks[i] %i (%s) (recording) \n", ranks[j], candidates[j], ranks[i], candidates[i]);
                //     printf("DEBUG recording preference for jth candidate %s (%i) being prefered over ith candidate %s (%i).\n", candidates[j], ranks[j], candidates[i], ranks[i]);
                //     preferences[j][i]++;
                // }

                //implied ranks[i] being higher ranked than ranks[i], so don't record
                // printf("DEBUG NOT recording preference for ith candidate %s (%i) NOT being prefered over jth candidate %s (%i).\n", candidates[i], ranks[i], candidates[j], ranks[j]);
            }
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
// The function should add all pairs of candidates where one candidate is preferred to the pairs array.
//  A pair of candidates who are tied (one is not preferred over the other) should not be added to the array.
// The function should update the global variable pair_count to be the number of pairs of candidates.
//  (The pairs should thus all be stored between pairs[0] and pairs[pair_count - 1], inclusive).
void add_pairs(void)
{
    //preferences[i][j] need to be checked to see the winner/loser or tied
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            //if itself, tied, or lost then ignore
            if (i == j)
            {
                //ignore
            }
            else if (preferences[i][j] == preferences[j][i])
            {
                //  printf("DEBUG candidate[i] %i (%s) is evenly matched with candidate[j] %i (%s) winning %i times vs losing %i times \n", i, candidates[i], j, candidates[j], preferences[i][j], preferences[j][i]);
            }
            else if (preferences[i][j] > preferences[j][i])
            {
                // printf("DEBUG candidate[i] %i (%s) is prefered over candidate[j] %i (%s) winning %i times vs losing %i times (recording in pairs array) \n",
                //     i, candidates[i], j, candidates[j], preferences[i][j], preferences[j][i]);

                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else
            {
                //implied that preferences[i][j] < preferences[j][i]
                // printf("DEBUG candidate[i] %i (%s) loses to candidate[j] %i (%s) winning %i times vs losing %i times \n", i, candidates[i], j, candidates[j], preferences[i][j], preferences[j][i]);
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
// The function should sort the pairs array in decreasing order of strength of victory,
//  where strength of victory is defined to be the number of voters who prefer the preferred candidate.
//  If multiple pairs have the same strength of victory, you may assume that the order does not matter.
void sort_pairs(void)
{
    pair sortedPairs[pair_count];
    int index = 0;
    int indexedWinner = 0;
    bool indexedPairs[pair_count];

    int winner;
    int loser;
    //set all to false
    for (int i = 0; i < pair_count; i++)
    {
        indexedPairs[i] = false;
        winner = pairs[i].winner;
        loser = pairs[i].loser;
        // printf("DEBUG PAIRS index %i has a winner (%s) %i and a loser %s (%i). \n", i, candidates[winner], winner, candidates[loser], loser);
    }

    for (int i = 0; i < pair_count; i++)
    {
        // printf("\nDEBUG sort pair loop : %i \n", i);

        //find the index of pairs that has the highest winning strenght
        //recurse to check other pairs to see if any have bigger strenght;
        //if not, record that into sorted pairs as ith position (lowest first search =/)
        //always start from 0 for offset
        indexedWinner = nextBestWinner(0, 0, 0, indexedPairs);
        sortedPairs[i] = pairs[indexedWinner];
        indexedPairs[indexedWinner] = true;

        // printf("DEBUG sorted pair best rank index of pairs : %i \n", indexedWinner);
    }

    // printf("\nDEBUG done with sort pair loop\n\n");

    // check sortedPairs
    // int winner;
    // int loser;
    // int difference;
    for (int i = 0; i < pair_count; i++)
    {
        winner = sortedPairs[i].winner;
        loser = sortedPairs[i].loser;
        // difference = preferences[winner][loser] - preferences[loser][winner];
        // printf("DEBUG %s (%i) has strenght of victory of %i over %s (%i) \n", candidates[winner], winner, difference, candidates[loser], loser);

        //replace original pairs with sorted
        pairs[i].winner = sortedPairs[i].winner;
        pairs[i].loser = sortedPairs[i].loser;
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
// The function should create the locked graph,
//  adding all edges in decreasing order of victory strength so long as the edge would not create a cycle.
void lock_pairs(void)
{
    int winner;
    int loser;
    for (int i = 0; i < candidate_count; i++)
    {
        winner = pairs[i].winner;
        loser = pairs[i].loser;

        //can lock in a win over yourself?
        if (winner == loser)
        {
            continue;
        }

        //if the current loser didn't already win over the loser, lock it in?
        if (!locked[loser][winner])
        {
            locked[winner][loser] = true;
            // printf("DEBUG %s (%i) has locked in victory over %s (%i) \n", candidates[winner], winner, candidates[loser], loser);
        }
        else
        {
            // printf("DEBUG %s (%i) has NOT locked in victory over %s (%i) \n", candidates[winner], winner, candidates[loser], loser);
        }
    }

    return;
}

// Print the winner of the election
// The function should print out the name of the candidate who is the source of the graph.
//  You may assume there will not be more than one source.
void print_winner(void)
{
    int wins[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        wins[i] = 0;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                wins[i]++;
                // printf("DEBUG incrementing win count for %s (%i) \n", candidates[i], i);
            }
            else
            {
                // printf("DEBUG NOT incrementing win count for %s (%i) \n", candidates[i], i);
            }
        }
    }

    int mostWins = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (wins[i] > mostWins)
        {
            // printf("DEBUG HAS MORE WINS: %i vs %i index = %i\n", wins[i], mostWins, i);
            mostWins = wins[i];
        }
    }

    //Winner(s)
    for (int i = 0; i < candidate_count; i++)
    {
        if (wins[i] == mostWins)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }

    return;
}

// if any (other) pairs have a bigger winning strenght (same strenght will be ignored)
int nextBestWinner(int offset, int index, int strenght, bool indexedPairs[])
{
    // printf("DEBUG next best winner with offset: %i \n", offset);
    // printf("DEBUG index : %i \n", index);
    // printf("DEBUG winning margin: %i \n", strenght);
    int winner;
    int loser;
    int difference;
    for (int i = offset; i < pair_count; i++)
    {
        //skip already sorted pairs
        if (indexedPairs[i])
        {
            // printf("DEBUG indexed so skipping %i \n", i);
            continue;
        }

        winner = pairs[i].winner;
        loser = pairs[i].loser;

        //if winner == loser, then this is a 'blank' pair. skip
        if (winner == loser)
        {
            // printf("DEBUG %i (%s) is same as %i (%s) not a real pair. \n", winner, candidates[winner], loser,
            //       candidates[loser]);
            continue;
        }

        difference = preferences[winner][loser] - preferences[loser][winner];
        // printf("DEBUG %s (%i) has strenght of victory of %i over %i (%s) \n", candidates[winner], winner, difference, candidates[loser],
        //     loser);

        //if the difference is bigger, that's now the highest (if same, treat it as weaker)
        if (difference > strenght)
        {
            index = i;
            strenght = difference;
            // printf("DEBUG updating index : %i \n", index);
            // printf("DEBUG strenght index : %i \n", strenght);
        }
        else
        {
            // printf("DEBUG not updating strenght %i as it's the same as difference %i \n", strenght, difference);
        }

        //recurse to check other pairs to see if any have bigger strenght;
        index = nextBestWinner(i + 1, index, strenght, indexedPairs);
    }

    return index;
}