/**
 * Auhtor: Umid Muzrapov
 *
 * Purpose: This is the source file, which comprises utility functions and variables.
 */

#include <stdio.h>
#include "utils.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

int exit_status = 0;

/**
 * Function filename
 *
 * This function returns the status
 * that indicates if the file with the given filename exists.
 *
 * @param filename the name of the file
 * @return integer that represents existence of the file.
 * 0 if the file exists.
 */
int file_exists(char *filename)
{
    struct stat buf;
    int stat_status = stat(filename, &buf);

    return stat_status;
}

/**
 * This method checks the allocated memory and ends
 * programs if necessary.
 * @param pointer generic pointer
 */
void memory_check(void *pointer)
{
    if (pointer == NULL)
    {
        print_error("Memory is full.");
        exit(1);
    }
}


/**
 * This method prints error messages
 * @param error_message error message to be printed
 */
void print_error(char *error_message)
{
    fprintf(stderr, "%s\n", error_message);
    exit_status = 1;
}
