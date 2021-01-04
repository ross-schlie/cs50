// Implements a dictionary's functionality

#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// * 26
const unsigned int N = 26;
long unsigned int dictionary_words = 0;

// Hash table
node *table[N];

bool addDictoryWord(char word[LENGTH + 1]);

// Returns true if word is in dictionary else false
// implementation of check must be case-insensitive.
//  In other words, if foo is in dictionary, then check should return true
//  given any capitalization thereof;
//  none of foo, foO, fOo, fOO, fOO, Foo, FoO, FOo, and FOO should be considered misspelled.
//  implementation of check should only return true for words actually in dictionary
// assume that check will only be passed words that contain
//  (uppercase or lowercase) alphabetical characters and possibly apostrophes
bool check(const char *word)
{
    char *lowercased = tolower(word);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // http://www.cse.yorku.ca/~oz/hash.html
    // djb2
    unsigned long hash = 5381;
    int c;

    while (c = *word++)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

// Loads dictionary into memory, returning true if successful else false
// assume that any dictionary passed to your program will be structured exactly like ours,
// alphabetically sorted from top to bottom with one word per line,
// each of which ends with \n. You may also assume that dictionary will contain at least one word,
// that no word will be longer than LENGTH (a constant defined in dictionary.h) characters,
// that no word will appear more than once,
// that each word will contain only lowercase alphabetical characters and possibly apostrophes,
// and that no word will start with an apostrophe
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file != NULL)
    {
        //basically copy what speller did to get words...
        int index = 0;
        int words = 0;
        char word[LENGTH + 1];
        for (int c = fgetc(file); c != EOF; c = fgetc(file))
        {
            // Allow only alphabetical characters and apostrophes
            if (isalpha(c) || (c == '\'' && index > 0))
            {
                // Append character to word
                word[index] = c;
                index++;

                // Ignore alphabetical strings too long to be words
                if (index > LENGTH)
                {
                    // Consume remainder of alphabetical string
                    while ((c = fgetc(file)) != EOF && isalpha(c));

                    // Prepare for new word
                    index = 0;
                }
            }

             // Ignore words with numbers (like MS Word can)
            else if (isdigit(c))
            {
                // Consume remainder of alphanumeric string
                while ((c = fgetc(file)) != EOF && isalnum(c));

                // Prepare for new word
                index = 0;
            }

            // We must have found a whole word
            else if (index > 0)
            {
                // Terminate current word
                word[index] = '\0';

                //put into hash
                if (!addDictoryWord(word))
                {
                    fclose(dictionaryFile);
                    return false;
                }

                // Update counter
                words++;

                // Prepare for next word
                index = 0;
            }
        }

        //check that these match? dictionary_words is incremented inside addDictoryWord on successfull malloc
        //dictionary_words == words
        fclose(dictionaryFile);
        return true;
    }

    return false;
}

bool addDictoryWord(char word[LENGTH + 1])
{
    // table[dictionary_words] = malloc(sizeof(struct node_t));
    // if(table[dictionary_words] == NULL)
    // {
    //     return false;
    // }

    // table[dictionary_words] = word;
    // node
    // hash()

    dictionary_words++;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictionary_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for(i = 0; i < N; i++)
    {
        free(table[i]);
        // table[i].word = NULL;
        // table[i].next = NULL;
    }

    dictionary_words = 0;
    //failure condition? - re-check table?
    return true;
}
