/**
 * Author: Umidjon Muzrapov
 *
 * THe source files contains functions
 * that provide services to read and covert date
 * from the make file/text.
 */

#include "makereader.h"
#include "utils.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * Function count_f
 *
 * The function counts the number -f from the command line.
 *
 * @param argv the number of command-line arguments
 * @param args an array of command line arguments
 *
 * @return number of -f(s)
 */
int count_f(int argv, char* args[])
{
    int count = 0;

    for (int i=0; i< argv; i++)
    {
        if (strcmp(args[i], "-f")==0)
        {
            count++;
        }
    }

    return count;
}

/**
 * Function get_makefile
 *
 * This function get the name of the makefile.
 * If it is not given, return null.
 *
 * @param argv the number of command line arguments
 * @param args an array of command line arguments
 * @return return the name of makefile.
 */
char* get_makefile(int argv, char* args[])
{
    for (int i=2; i<argv; i++)
    {
        if (strcmp(args[i-1], "-f")==0)
        {
            return args[i];
        }
    }

    return NULL;
}

/**
 * Function get_command_target
 *
 * The function get the name of the target
 * from the command-line arguments. If none given, return null.
 *
 * @param argv the number of command line arguments
 * @param args an array of command line arguments
 *
 * @return name of target or target if not given.
 */
char* get_command_target(int argv, char* args[])
{
    if (argv==1)
    {
        return NULL;
    }

    if (strcmp(args[1], "-f")!=0)
    {
        return args[1];
    }

    for (int i=3; i<argv; i++)
    {
        if (strcmp(args[i-1], "-f")!=0)
        {
            return args[i];
        }
    }

    return NULL;
}

/**
 * Function add_end_line
 *
 * The function checks if the line ends with '\n'.
 * if no, it add '\n' to the end of the string literal.
 * @param line a line read from the input stream
 */
void add_end_line(char *line)
{
    size_t size = strlen(line);
    if (line[size - 1] != '\n')
    {
        line[size] = '\n';
    }
}

/**
 * Function find_dependency_start
 *
 * The function finds the index, where dependency section
 * of the string begins.
 *
 * @param line a line read from input stream
 * @return location of the ':', which indicates the start of dependency section.
 */
int find_dependency_start(char *line)
{
    char *cur_p = line;
    int count = 0;

    while (*cur_p != '\0')
    {
        if ((*cur_p) == ':')
        {
            return count;
        }

        count++;
        cur_p++;
    }

    return -1;
}

/**
 * Function is_all_space
 *
 * The function checks if the given line
 * contains only spaces.
 *
 * @param line a line read from the input stream
 * @return 1 if it is all spaces. -1 otherwise.
 */
int is_all_space(char *line)
{
    size_t length = strlen(line);
    char *cur_p = line;
    int space_count = 0;

    while (*cur_p != '\0')
    {
        if (isspace(*cur_p))
        {
            space_count++;
        }

        cur_p++;
    }

    return space_count == length ? 1 : -1;
}

/**
 * Function valdiate_target
 *
 * The function validate the target section of the rule.
 * It ensures that only one target is specified in a rule.
 *
 * @param line a line from the input stream
 * @param dependency_start index where the dependency section begins
 * @return 1 if valid. -1 if not.
 */
int validate_target(char *line, int dependency_start)
{
    char target_section[dependency_start];

    for (int i = 0; i < dependency_start; i++)
    {
        target_section[i] = *(line + i);
    }
    target_section[dependency_start] = '\0';

    char *temp1 = malloc(sizeof(char) * (strlen(line)+1));
    char *temp2 = malloc(sizeof(char) * (strlen(line)+1));
    memory_check(temp1);
    memory_check(temp2);
    int number_of_words = sscanf(target_section, "%s %s", temp1, temp2);
    free(temp1);
    free(temp2);

    return number_of_words == 1 ? 1 : -1;
}

/**
 * Function get_target_name
 *
 * The function extracts the target name from the rule.
 *
 * @param line a line from the input stream
 * @param dependency_start index where the dependency section begins
 * @return name of the target
 */
char *get_target_name(char *line, int dependency_start)
{
    char *target_name = malloc(sizeof(char) * (dependency_start + 1));
    memory_check(target_name);
    int word_index = 0;

    for (int i = 0; i < dependency_start; i++)
    {
        if (isspace(*(line + i)) == 0)
        {
            target_name[word_index] = *(line + i);
            word_index++;
        }
    }

    target_name[word_index] = '\0';

    return target_name;
}

/**
 * Function begin_with_tab
 *
 * The function checks if the line begins with a tab.
 *
 * @param line a line from the input stream
 * @return 1 if begins with a tab. -1 otherwise.
 */
int begin_with_tab(char *line)
{
    return *(line) == '\t' ? 1 : -1;
}

/**
 * Function process tab
 *
 * This function processes a line that begins with a tab
 * and replaces with "  ".
 *
 * @param line a line read from the input stream
 * @param buff_size size of the line
 * @return a formatted line
 */
char *process_tab(char *line, size_t buff_size)
{
    // fline is formatted line
    char *fline = malloc((buff_size + 2) * sizeof(char));
    memory_check(fline);
    int wordIndex = 0;
    // first two characters of fline are spaces
    char *cur_p = line + 1;

    while (*cur_p != '\0')
    {
        *(fline + wordIndex) = *cur_p;
        wordIndex++;
        cur_p++;
    }

    fline[wordIndex] = '\0';
    return fline;
}

/**
 * Function open_file
 *
 * The function opens a file with a given name in read mode.
 *
 * @param filename name of the file to open
 *
 * @return a pointer to file. If the file cannot be opened, exit the program.
 */
FILE *open_file(char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        perror(filename);
        exit(1);
    }

    return fp;
}



