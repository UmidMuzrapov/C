/**
 * Author: Umid Muzrapov
 *
 * This is the header file for makereader.c
 * It contains function prototypes, which allow
 * certain functions to be shared.
 */

#include <stdio.h>
#include "graph.h"

#ifndef GITHUB_MAKEREADER_H
#define GITHUB_MAKEREADER_H

/************* These are function prototypes ***************/
// their definitions and documentation are given in makereader.c

FILE *open_file(char *filename);

int count_f(int argv, char *args[]);

char *get_makefile(int argv, char *args[]);

char *get_command_target(int argv, char *args[]);

int find_dependency_start(char *line);

int validate_target(char *line, int dependency_start);

char *process_tab(char *line, size_t buff_size);

char *get_target_name(char *line, int dependency_start);

int begin_with_tab(char *line);

int is_all_space(char *line);

void add_end_line(char *line);

#endif //GITHUB_MAKEREADER_H
