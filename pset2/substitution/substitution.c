// Your program must accept a single command-line argument, the key to use for the substitution.
// The key itself should be case-insensitive, so whether any character in the key is uppercase or lowercase should not affect the behavior of your program.

// If your program is executed without any command-line arguments or with more than one command-line argument,
// your program should print an error message of your choice (with printf)
// and return from main a value of 1 (which tends to signify an error) immediately.

// If the key is invalid (as by not containing 26 characters,
// containing any character that is not an alphabetic character,
// or not containing each letter exactly once),
// your program should print an error message of your choice (with printf)
// and return from main a value of 1 immediately.

// Your program must output plaintext: (without a newline)
// and then prompt the user for a string of plaintext (using get_string).

// Your program must output ciphertext: (without a newline)
// followed by the plaintext’s corresponding ciphertext,
// with each alphabetical character in the plaintext substituted
// for the corresponding character in the ciphertext;
// non-alphabetical characters should be outputted unchanged.

// Your program must preserve case: capitalized letters must remain capitalized letters;
// lowercase letters must remain lowercase letters.

// After outputting ciphertext, you should print a newline. Your program should then exit by returning 0 from main.

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

bool checkArg(string argv);
string encrypt(string plainText, string cypher);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    if (!checkArg(argv[1]))
    {
        printf("Key must contain all 26 english alphabet characters (each one only once).\n");
        return 1;
    }

    string plainText = get_string("plaintext: ");
    string cypherText = encrypt(plainText, argv[1]);
    printf("ciphertext: %s", cypherText);

    printf("\n");
    return 0;
}

//check length, alphabetics, occurence
bool checkArg(string argv)
{
    int length = strlen(argv);
    char alphabetMap[26] = "abcdefghijklmnopqrstuvwxyz";
    char foundMap[26] = "                          ";
    //char foundMap[25];
    string stringFound = "";
    char c = ' ';

    if (length != 26)
    {
        return false;
    }

    for (int i = 0; i < length; i++)
    {
        if (!isalpha(argv[i]))
        {
            return false;
        }

        c = tolower(argv[i]);
        stringFound = strchr(alphabetMap, c);
        if (stringFound == NULL)
        {
            printf("Not found in alphabet: %c\n", c);
            return false;
        }

        //implicit else

        //check to see if duplicate
        stringFound = strchr(foundMap, c);
        if (stringFound != NULL)
        {
            printf("Duplicate %s character: %c in %s\n", stringFound, c, foundMap);
            return false;
        }

        //record finding of this letter of the alphabet in a string
        foundMap[i] = c;
        // printf("map: %s\n", foundMap);
    }

    if (strlen(foundMap) != 26)
    {
        printf("Found chars: %s\n", foundMap);
        return false;
    }

    return true;
}

//walk over plaintext, find cypher to replace it with and build cyphered text to output
string encrypt(string plainText, string cypher)
{
    //should be 26.
    int cypherLength = strlen(cypher);
    char alphabetMap[26] = "abcdefghijklmnopqrstuvwxyz";
    char cypherMap[25];
    for (int i = 0; i < cypherLength; i++)
    {
        cypherMap[i] = tolower(cypher[i]);
    }

    // printf("ALPHABET = %s \n", alphabetMap);
    // printf("CYPHER = %s \n", cypherMap);

    //this way we are iterating over the plain text, it would be better to iterate over the cypher (once)
    //but str replace is ~~~~
    int length = strlen(plainText);
    //copy original text so we have a string of the same length to replace char by char
    string cypherText = plainText;
    // string charFound = "";
    char c = ' ';
    bool isLowercased = false;

    for (int i = 0; i < length; i++)
    {
        //lookup which char in cypher equates to plainText[i]
        isLowercased = islower(plainText[i]);
        if (isLowercased)
        {
            c = plainText[i];
        }
        else
        {
            c = tolower(plainText[i]);
        }

        //hmmmm
        for (int j = 0; j < cypherLength; j++)
        {
            //alphabetMap is neatly indexed 1 - 25
            //if the current plain text character matches the index in the alphabet
            //we know to replace this plaintext character with the cypher character at the same index
            if (alphabetMap[j] == c)
            {
                // printf("%c \n", plainText[i]);
                // printf("%c \n", alphabetMap[j]);
                // printf("%c \n", cypherMap[j]);
                // printf("%i \n", j);

                if (isLowercased)
                {
                    cypherText[i] = cypherMap[j];
                }
                else
                {
                    cypherText[i] = toupper(cypherMap[j]);
                }
            }
            // else
            // {
            //     printf("%c \n", cypherMap[j]);
            // }
        }

    }

    // printf("CURRENT TEXT = %s \n", cypherText);
    return cypherText;
}