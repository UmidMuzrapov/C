#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/**
 * File: rotate.c
 * Author: Umid Muzrapov
 * Purpose: This counts the number of times each number occurs
 */

typedef struct Node Node;
typedef struct List List;

/*
 * This is node struct.
 */
struct Node{
    int value;
    Node* next;
};

/**
 * This is linked list.
 */
struct List{
    Node* head;
};

void add(int value, List* linkedList);
void printList(List* linkedList);
void printFrequency(List* linkedList);

/**
 * This is the beginning point of  program.
 * @return 1 if fail. 0 if success.
 */
int main(){
    List* pLinkedList = malloc(sizeof (List));
    pLinkedList->head = NULL;

    int inputStatus;
    int number;
    int statusCode =0;

    inputStatus = scanf("%d", &number);

    while (inputStatus>0){
        add(number, pLinkedList);
        inputStatus = scanf("%d", &number);
        if (inputStatus==0){
            statusCode = 1;
            fprintf(stderr, "Invalid input \n");
            return  statusCode;
        }
    }

    if (inputStatus==0){
        statusCode = 1;
        fprintf(stderr, "Invalid input \n");
        return  statusCode;
    }

    printFrequency(pLinkedList);
    free(pLinkedList);
    return statusCode;
}

/**
 * Add a new element to list
 * @param value value
 * @param pLinkedList pointer to linkedList
 */
void add(int value, List* pLinkedList){
    Node* pNewNode = malloc(sizeof(Node));
    pNewNode->next = NULL;
    pNewNode->value = value;

    if ( pLinkedList->head == NULL){
        pLinkedList->head = pNewNode;
        return;
    }

    Node* pCurrentNode = pLinkedList->head;
    Node* pNextNode = pCurrentNode->next;
    int found=0;

    if(value < pCurrentNode->value){
        pLinkedList->head = pNewNode;
        pNewNode->next = pCurrentNode;
        return;
    }

    while (found==0){
        if(pNextNode == NULL){
            pCurrentNode->next = pNewNode;
            found = 1;
        } else if (value < pNextNode->value){
            pCurrentNode->next= pNewNode;
            pNewNode->next = pNextNode;
            found = 1;
        }
        else{
            pCurrentNode = pNextNode;
            pNextNode =  pCurrentNode->next;
        }
    }
}

/**
 * This method prints the frequency of each number.
 * @param linkedList pointer to LinkedList;
 */
void printFrequency(List* linkedList){

    if (linkedList->head == NULL){
        return;
    }

    Node* currentNode = linkedList->head;
    int count = 1;
    int first = 1;
    int prev = currentNode->value;

    while (currentNode!=NULL){
        if(first==1) {
            first = 0;
        } else{
            if (prev==currentNode->value){
                count++;
            } else{
                printf("%d %d\n", prev, count);
                prev = currentNode->value;
                count = 1;
            }
        }

        currentNode = currentNode->next;
    }

    printf("%d %d\n", prev, count);
}

/**
 * This method prints the elements of linkedlist.
 * @param linkedList
 */
void printList(List* linkedList){
    Node* currentNode = linkedList->head;
    while(currentNode!=NULL){
        printf("%d\n", currentNode->value);
        currentNode = currentNode->next;
    }
}
