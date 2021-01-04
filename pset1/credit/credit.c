#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

bool checkCC(long ccnum, int cclength);

int getCCLength(long ccnum);

bool isAmex(long ccnum, int cclength);
bool isMasterC(long ccnum, int cclength);
bool isVisa(long ccnum, int cclength);

int main(void)
{
    bool valid = false;

    //ask the user the cc number
    long ccnum = get_long("Number: ");

    int cclength = getCCLength(ccnum);
    //debug info
    // printf("length %i\n", cclength);
    if (cclength > 12 && cclength < 17)
    {
        //check checksum
        //it's easier to use arrays but we don't know arrays in week 1, so this is without using them
        valid = checkCC(ccnum, cclength);
    }

    if (valid)
    {
        //use length and starting nums to decide type
        if (isVisa(ccnum, cclength))
        {
            printf("VISA\n");
        }
        else if (isAmex(ccnum, cclength))
        {
            printf("AMEX\n");
        }
        else if (isMasterC(ccnum, cclength))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

//using exponents of 10;
//determine the length of the credit card number (when the division results in 0)
int getCCLength(long ccnum)
{
    int ccLength = 1;
    bool ccLengthReached = false;

    long tenPow = 0;
    long tenDiv = 0;
    int i = 1;

    do
    {
        tenPow = pow((double) 10, (double) i);
        tenDiv = ccnum / tenPow;
        //printf("DEBUG DIGIT length, digits, division = %i , %li , %li\n", i, tenPow, tenDiv);
        if (tenDiv == 0)
        {
            ccLengthReached = true;
        }
        else
        {
            ccLength++;
        }

        i++;
    }
    while (!ccLengthReached);

    return ccLength;
}

//using modular, get every digit; use Luhn’s Algorithm
//Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
//Add the sum to the sum of the digits that weren’t multiplied by 2.
//If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
bool checkCC(long ccnum, int cclength)
{
    //originaly i was using the exponent of 10 to get to each higher digit, but with ccnumMinusMod - modulus / 10 this seems unnecessary
    // long tenPow = 0;
    long tenPow = 10;
    long modulus = 0;
    long ccnumMinusMod = ccnum;

    int oddSum = 0;
    int evenSum = 0;
    int product = 0;
    int productMod = 0;

    for (int i = 1; i <= cclength; i++)
    {
        // tenPow = pow((double) 10, (double) i);
        // printf("DEBUG DIGIT length, digits= %i , %li\n", i, tenPow);
        modulus = ccnumMinusMod % tenPow;
        // printf("DEBUG DIGIT modulus %li mod %li = %li\n", ccnumMinusMod, tenPow, modulus);
        ccnumMinusMod -= modulus;
        ccnumMinusMod = ccnumMinusMod / tenPow;
        // printf("DEBUG DIGIT cc minus modulus and dived by pow = %li\n", ccnumMinusMod);

        if (i % 2 == 1)
        {
            //digits that weren’t multiplied by 2 (odd digits)
            evenSum += modulus;
        }
        else
        {
            //add those products’ digits together
            product = modulus * 2;
            productMod = product % 10;
            if (product / 10 != 0)
            {
                // printf("DEBUG product %i modulus = %i\n", product, productMod);
                oddSum += 1;
                oddSum += productMod;
            }
            else
            {
                oddSum += product;
            }

            // printf("DEBUG modulus = %li\n", modulus);
            // printf("DEBUG product = %i\n", product);
            // printf("DEBUG odd SUM = %i\n", oddSum);
        }
    }

    // printf("DEBUG final odd SUM = %i\n", oddSum);
    // printf("DEBUG final even SUM = %i\n", evenSum);
    int sum = evenSum + oddSum;
    if (sum % 10 == 0)
    {
        //debug info
        // printf("VALID CHECKSUM \n");
        return true;
    }

    return false;
}

//American Express uses 15-digit numbers
//All American Express numbers start with 34 or 37
bool isAmex(long ccnum, int cclength)
{
    if (cclength == 15)
    {
        //non-array version
        long firstDigit = ccnum / 100000000000000;
        long secondDigit = (ccnum / 10000000000000) % 10;

        //debug info
        // printf("AMEX length, digits = %li , %li\n", firstDigit, secondDigit);
        if (firstDigit == 3 && (secondDigit == 4 || secondDigit == 7))
        {
            return true;
        }
    }

    return false;
}

//most MasterCard numbers start with 51, 52, 53, 54, or 55
//MasterCard uses 16-digit numbers
bool isMasterC(long ccnum, int cclength)
{
    if (cclength == 16)
    {
        //non-array version
        long firstDigit = ccnum / 1000000000000000;
        long secondDigit = (ccnum / 100000000000000) % 10;

        //debug info
        // printf("MASTERC length, digits = %li , %li\n", firstDigit, secondDigit);
        if (firstDigit == 5 && (secondDigit >= 1 && secondDigit <= 5))
        {
            return true;
        }
    }

    return false;
}

//and all Visa numbers start with 4
//Visa uses 13- and 16-digit numbers
bool isVisa(long ccnum, int cclength)
{
    if (cclength == 16 || cclength == 13)
    {
        long firstDigit = 0;
        //non-array version
        if (cclength == 16)
        {
            firstDigit = ccnum / 1000000000000000;
        }
        else if (cclength == 13)
        {
            firstDigit = ccnum / 1000000000000;
        }

        //debug info
        // printf("VISA length, first digit = %li\n", firstDigit);
        if (firstDigit == 4)
        {
            return true;
        }
    }

    return false;
}