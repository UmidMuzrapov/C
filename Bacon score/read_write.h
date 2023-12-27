/**
 * Author: Umid Muzrapov
 *
 * Purpose: This is the header file for read_write.c
 */

#ifndef GITHUB_READ_WRITE_H
#define GITHUB_READ_WRITE_H

#include <stdio.h>
#include <string.h>
#include "utils.h"

FILE *open_file(char *filename);

int count_l(int, char *args[]);

char *get_file_name(int argv, char *args[]);

int count_file_names(int argv, char *args[]);

void add_end_line(char *line);

int is_all_space(char *line);

int is_movie(char *line, char **movie_name);

void get_actor_name(char *line, char **actor_name);

#endif //READ_WRITE_H
