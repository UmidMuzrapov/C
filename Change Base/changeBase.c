/**
 * File: changeBase.c
 * Author: Umid Muzrapov
 * Purpose: This programs converts input from any base
 * to the output in the decimal base.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
int checkBase(int);
int checkBaseInput(char[], int );
int parseChar(char);
unsigned long makeConversion(char[], int);

/**
 * This is where the program begins and is the main logic.
 * @return o if normal. 1 if error.
 */
int main()
{
    int isBaseValid;
    int base;
    int returnCode=0;
    char baseInput[7];
    int readSuccess;
    scanf("%d", &base);
    isBaseValid = checkBase(base);

    if (isBaseValid == 0)
    {
        fprintf(stderr, "The base is not supported.\n");
        returnCode = 1;
        return returnCode;
    }

    readSuccess = scanf("%6s", baseInput);

    while (readSuccess>0)
    {
        int isBaseInputValid = checkBaseInput(baseInput, base);
        if (isBaseInputValid == 0)
        {
            fprintf(stderr, "Given input for base is wrong.\n");
            returnCode = 1;
        }

        else
        {
            unsigned long valueInDecimal = makeConversion(baseInput, base);
            printf("%lu\n", valueInDecimal);
        }
        readSuccess = scanf("%6s", baseInput);
    }

    if (readSuccess==0)
    {
        fprintf(stderr, "Given input for base is wrong.\n");
        returnCode = 1;
    }

    return returnCode;
}

/**
 * This method validates the given base
 * @param base base
 * @return 0 if invalid. 1 if valid.
 */
int checkBase(int base)
{
    if (base<2 || base > 36)
    {
        // invalid base
        return 0;
    }

    return 1;
}

/**
 * This method checks if the given input
 * is valid for the given base.
 * @param baseInput input string
 * @param base base
 * @return 0 if invalid. 1 if valid.
 */
int checkBaseInput(char baseInput[], int base)
{
    int baseInputLength = strlen(baseInput);
    int conversionVal;
    for (int i=0; i<baseInputLength; i++)
    {
        conversionVal = parseChar(baseInput[i]);
        if (conversionVal>=base)
        {
            // invalid
            return 0;
        }
    }

    return 1;
}

/**
 * This method converts the given string number
 * in any base to the decimal number.
 * @param baseInput string number to be converted
 * @param base base of the baseInput
 * @return unsigned long decimal version of the baseInput.
 */
unsigned long makeConversion(char baseInput[], int base)
{
    int x=0;

    for (int i=0; baseInput[i]!='\0'; i++)
    {
        x= base*x + parseChar(baseInput[i]);
    }

    return x;
}

/**
 * This method parse given character to
 * the integer -> 0-36.
 * @param character character to parse
 * @return integer value of character
 */
int parseChar(char character)
{
    int isDigit = isdigit(character);
    int conversionConst= 87;

    if (isDigit >0)
    {
        return character - '0';
    }
    else if (isalpha(character)>0)
    {
        char curChar = tolower(character);
        int conversionVal = curChar - conversionConst;
        return conversionVal;
    }
    else
    {
        //any value over 36
        return 38;
    }
}

