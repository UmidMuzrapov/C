/**
 * Author: Umid Muzrapov
 *
 * Purpose: This is the header file for utils.c.
 * It contains prototypes for the shared functions and variables.
 */

#ifndef GITHUB_UTILS_H
#define GITHUB_UTILS_H
#define FILE_EXISTS 0

#include <time.h>

extern int exit_status;

/********* The following lines are function function prototypes **********/
void print_error(char *error_message);

int update_time_stamp(char *filename);

int compare_time(struct timespec time1, struct timespec time2);

int file_exists(char *filename);

void memory_check(void *);

struct timespec get_last_modified_date(char *filename);

#endif //GITHUB_UTILS_H
