/**
 * Author: Umid Muzrapov
 *
 * Purpose: This programs takes words and build a list
 * where each node contains an inner list of anagrams.
 * Once the input stream ends, the groups of anagrams are printed.
 * Print each group of anagrams in a line.
 *
 * Class: CS 352, assignment
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Struct definitions
typedef struct OneNode OneNode;
typedef struct TwoNode TwoNode;
typedef struct List List;

struct OneNode {
    char value[65];
    OneNode *next;
};

struct TwoNode {
    char value[65];
    TwoNode *nextList;
    OneNode *next;
};

struct List {
    TwoNode *head;
};

// function prototypes
void add(List *, char[]);
int checkAnagram(char[], char[]);
int checkWord(char[]);
void toLowerString(char[]);
void addToInnerList(TwoNode *plist, char[]);
void printList(TwoNode *list);
void printInnerList(TwoNode *innerList);
// global variable to keep exit status
int exitCode = 0;

/**
 * This is the entry point of the program.
 * @return 0 if normal. 1 if error.
 */
int main() {
    int inputStatus;
    char word[65];
    List *list = malloc(sizeof(List));

    // check if there is memory
    if (list == NULL) {
        printf("Insufficient memory\n");
        exit(1);
    }

    list->head = NULL;
    inputStatus = scanf("%64s", word);

    while (inputStatus > 0) {
        if (checkWord(word)) {
            add(list, word);
        } else {
            exitCode = 1;
            fprintf(stderr, "Invalid input\n");
        }

        inputStatus = scanf("%64s", word);
    }

    printList(list->head);
    free(list);
    return exitCode;
}

/**
 * This method adds word to the linked list.
 * It first find the right node that contains the anagram inner list.
 * If not found the word will begin a new branch.
 * @param plist a pointer to linked list.
 * @param word a new word to add.
 */
void add(List *plist, char word[]) {

    TwoNode *newTwoNode = malloc(sizeof(TwoNode));
    if (newTwoNode == NULL) {
        printf("Insufficient memory\n");
        exit(1);
    }

    // if not element is there
    if (plist->head == NULL) {

        strcpy(newTwoNode->value, word);
        newTwoNode->next = NULL;
        newTwoNode->nextList = NULL;
        plist->head = newTwoNode;
        return;
    }

    TwoNode *currentNode = plist->head;

    while (currentNode->nextList != NULL) {
        if (checkAnagram(currentNode->value, word) == 1) {
            addToInnerList(currentNode, word);
            free(newTwoNode);
            return;
        } else {
            currentNode = currentNode->nextList;
        }
    }

    // logic if it is at the very end or there is not existing anagram of the word.
    if (checkAnagram(currentNode->value, word) == 1) {
        addToInnerList(currentNode, word);
        free(newTwoNode);
        return;
    } else {
        strcpy(newTwoNode->value, word);
        newTwoNode->next = NULL;
        newTwoNode->nextList = NULL;
        currentNode->nextList = newTwoNode;
    }

}

/**
 * This method adds a word to the inner list that contains
 * the word's anagrams.
 * @param innerList a head pointer to the inner list.
 * @param word a word to add to inner list.
 * @return 0 if not sufficient memory. 1 if success.
 */
void addToInnerList(TwoNode *innerList, char word[]) {

    OneNode *newNode = malloc(sizeof(OneNode));

    if (newNode == NULL) {
        printf("In sufficient memory\n");
        exit(1);
    }

    strcpy(newNode->value, word);
    newNode->next = NULL;

    if (innerList->next == NULL) {
        innerList->next = newNode;
        return;
    }

    OneNode *currentNode = innerList->next;
    OneNode *previousNode = NULL;
    while (currentNode != NULL) {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    previousNode->next = newNode;
}

/**
 * This program check if the inputWord is an anagram of
 * the first word.
 * @param word word to check against.
 * @param inputWord word to check
 * @param wordLength length of the word to check against
 * @return 0 if not anagram. 1 if anagram.
 */
int checkAnagram(char word[], char inputWord[]) {
    char *ret;
    int inputLength = strlen(inputWord);
    int wordLength = strlen(word);
    char tempWord[65];
    char tempInput[65];
    strcpy(tempInput, inputWord);
    strcpy(tempWord, word);
    toLowerString(tempInput);
    toLowerString(tempWord);

    if (inputLength != wordLength) {
        // no anagram
        return 0;
    }

    for (int i = 0; i < wordLength; i++) {
        char wordChar = tempWord[i];
        ret = strchr(tempInput, wordChar);
        if (ret != NULL) {
            int positionFound = ret - tempInput;
            tempInput[positionFound] = '1';
        } else {
            // no anaram
            return 0;
        }
    }

    return 1;
}

/**
 * This method checks if the given word is valid
 * -> contains only alpha.
 * @param word word to check for validity.
 * @return 0 if invalid. 1 if valid.
 */
int checkWord(char word[]) {
    int wordLength = strlen(word);
    for (int i = 0; i < wordLength; i++) {
        char wordChar = word[i];
        if (isalpha(wordChar) == 0) {
            // does not pass the check
            return 0;
        }
    }

    return 1;
}

/**
 * This method converts the given string
 * to lower case.
 * @param string the string to convert to lowercase.
 */
void toLowerString(char string[]) {
    int stringLength = strlen(string);

    for (int i = 0; i < stringLength; i++) {
        string[i] = tolower(string[i]);
    }
}

/**
 * This method prints the linked list.
 * @param list head pointer to a linked list.
 */
void printList(TwoNode *list) {
    TwoNode *currentNode = list;
    while (currentNode != NULL) {
        printInnerList(currentNode);
        currentNode = currentNode->nextList;
    }
}

/**
 * This method prints the inner list.
 * @param innerList a head pointer to inner list.
 */
void printInnerList(TwoNode *innerList) {

    if (innerList->next != NULL) {
        printf("%s ", innerList->value);
    } else {
        printf("%s", innerList->value);
    }

    OneNode *currentNode = innerList->next;

    while (currentNode != NULL) {
        if (currentNode->next != NULL) {
            printf("%s ", currentNode->value);
        } else {
            printf("%s", currentNode->value);
        }

        currentNode = currentNode->next;
    }

    printf("\n");
}