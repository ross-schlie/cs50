// Design and implement a program, readability, that computes the Coleman-Liau index of the text.
// Your program must prompt the user for a string of text (using get_string).
// Your program should count the number of letters, words, and sentences in the text.
// You may assume that a letter is any lowercase character from a to z or any uppercase character from A to Z,
//  any sequence of characters separated by spaces should count as a word, and that any occurrence of a period,
//  exclamation point, or question mark indicates the end of a sentence.
// Your program should print as output "Grade X" where X is the grade level computed by the Coleman-Liau formula, rounded to the nearest integer.
// If the resulting index number is 16 or higher (equivalent to or greater than a senior undergraduate reading level),
//  your program should output "Grade 16+" instead of giving the exact index number. If the index number is less than 1, your program should output "Before Grade 1".

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
float calc_index(int letterCount, int wordCount, int sentenceCount);
float calc_index_efficient(string userText);

int main(void)
{
    string userText = get_string("Text: ");

    // int letterCount = count_letters(userText);
    // printf("%i letter(s)\n", letterCount);

    // int wordCount = count_words(userText);
    // printf("%i word(s)\n", wordCount);

    // int sentenceCount = count_sentences(userText);
    // printf("%i sentence(s)\n", sentenceCount);

    // float index = calc_index(letterCount, wordCount, sentenceCount);
    float index = calc_index_efficient(userText);

    //outputs the grade level as given by the Coleman-Liau index (e.g. "Grade 2" or "Grade 8").
    // Be sure to round the resulting index number to the nearest whole number!
    // If the resulting index number is 16 or higher (equivalent to or greater than a senior undergraduate reading level),
    // your program should output "Grade 16+" instead of giving the exact index number.
    // If the index number is less than 1, your program should output "Before Grade 1".
    if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", (int) index);
    }
}

// Treat a string as an array and check each character to see if it's alphabetical or not. return the count of alphabeticals
int count_letters(string text)
{
    int length = strlen(text);
    int letters = 0;
    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }

    return letters;
}

// Treat a string as array and count "any sequence of characters separated by a space to be a word"
int count_words(string text)
{
    int length = strlen(text);
    int words = 0;
    for (int i = 0; i < length; i++)
    {
        // this will be wrong if there's an double space.
        // if (isspace(text[i]))
        if (isspace(text[i]) && !isspace(text[i - 1]))
        {
            words++;
        }
    }

    //check to see if the very last letter is a space or not. if not, increment by one (since the last word should also count.)
    if (!isspace(text[length]))
    {
        words++;
    }

    return words;
}

// You might first imagine that a sentence is just any sequence of characters that ends with a period,
//  but of course sentences could end with an exclamation point or a question mark as well.
// But of course, not all periods necessarily mean the sentence is over.
// we’ll ask you to ignore that subtlety: you should consider any sequence of characters that ends with a . or a ! or a ? to be a sentence
int count_sentences(string text)
{
    int length = strlen(text);
    int sentences = 0;

    for (int i = 0; i < length; i++)
    {
        //ignoring odd cases of double punctuations
        //ispunct does not only specify .!?

        //converting to string and comparing is more annoying than converting to int and comparing
        //char *c = "";
        //c = (string) text[i];
        //if (strcmp(c, ".") != 0 || strcmp(c, "?") != 0 || strcmp(c, "!") != 0)
        if (text[i] == (int) '.' || text[i] == (int) '?' || text[i] == (int) '!')
        {
            sentences++;
        }
    }

    return sentences;
}

//index = 0.0588 * L - 0.296 * S - 15.8
//where L is the average number of letters per 100 words in the text, and S is the average number of sentences per 100 words in the text.
float calc_index(int letterCount, int wordCount, int sentenceCount)
{
    float perHundred = (float) wordCount / 100;
    float L = (float) letterCount / perHundred;
    float S = (float) sentenceCount / perHundred;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    // printf("%i letter(s)\n", letterCount);
    // printf("%i word(s)\n", wordCount);
    // printf("%i sentence(s)\n", sentenceCount);
    // printf("%'.2f per 100 ?? \n", perHundred);
    // printf("%'.2f L\n", L);
    // printf("%'.2f S\n", S);
    // printf("%'.2f index\n", index);

    return round(index);
}

// instead of 3N we do N (single loop!)
float calc_index_efficient(string text)
{
    int length = strlen(text);
    int letters = 0;
    int words = 0;
    int sentences = 0;
    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }

        if (isspace(text[i]) && !isspace(text[i - 1]))
        {
            words++;
        }

        if (text[i] == (int) '.' || text[i] == (int) '?' || text[i] == (int) '!')
        {
            sentences++;
        }
    }

    //check to see if the very last letter is a space or not. if not, increment by one (since the last word should also count.)
    if (!isspace(text[length]))
    {
        words++;
    }

    // printf("%i letter(s)\n", letters);
    // printf("%i word(s)\n", words);
    // printf("%i sentence(s)\n", sentences);

    return calc_index(letters, words, sentences);
}