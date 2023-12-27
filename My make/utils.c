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
 * Function get_last_modified_date
 *
 * The function gets the date when the file was last modified.
 *
 * @param filename the name of the file
 * @return timespec which represents the last modification time.
 */
struct timespec get_last_modified_date(char *filename)
{
    struct stat buf;
    if (stat(filename, &buf)==0)
    {
        return buf.st_mtim;
    }

    else
    {
        struct timespec tp = {0, 0};
        return tp;
    }

}

/**
 * Function compare_time
 *
 * This function compares given timespecs.
 * If time1 > time2, it returns 1.
 * If time2 > time2, it return -1.
 * if time1 = time2, return 0.
 *
 * @param time1 first timespec
 * @param time2 second timespec
 *
 * @return 1 if time1 > time2. It time2 > time1, -1. 0 if equal.
 */
int compare_time(struct timespec time1, struct timespec time2)
{

    if (time1.tv_sec > time2.tv_sec)
    {
        return 1;
    } else if (time1.tv_sec < time2.tv_sec)
    {
        return -1;
    } else
    {
        // Compare nanoseconds only if seconds are equal
        if (time1.tv_nsec > time2.tv_nsec)
        {
            return 1;
        } else if (time1.tv_nsec < time2.tv_nsec)
        {
            return -1;
        }

        return 0;  // Files have the same modification time
    }
}

/**
 * Function update_time_stamp
 *
 * The function updates the timestamp of the file
 * associated with the filename.
 *
 * @param filename the name of the file
 *
 * @return return 1 if the timestamp is updated successfully.
 * -1 otherwise.
 */
int update_time_stamp(char *filename)
{
    struct stat buf;
    int stat_status = stat(filename, &buf);

    if (stat_status == 0)
    {
        char touch_command[80] = "touch ";
        strcat(touch_command, filename);
        int system_call_status = system(touch_command);
        if (system_call_status == -1)
        {
            print_error("The system call failed");
            return -1;
        }

        return 1;
    }

    return -1;
}

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
