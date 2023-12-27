#include "read_write.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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
int count_l(int argv, char* args[])
{
    int count = 0;

    for (int i=1; i< argv; i++)
    {
        if (strcmp(args[i], "-l")==0)
        {
            count++;
        }
    }

    return count;
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
        line[size]= '\n';
        line[size+1]='\0';
    }
}

/**
 * Function count_file_names
 *
 * This function counts the number file anmes
 * @param argv
 * @param args
 * @return
 */
int count_file_names(int argv, char* args[])
{
    int count = 0;

    for (int i=1; i< argv; i++)
    {
        if (strcmp(args[i], "-l")!=0)
        {
            count++;
        }
    }

    return count;
}

/**
 * This function gets the file name
 *
 * @param argv
 * @param args
 * @return
 */
char* get_file_name(int argv, char* args[])
{
    for (int i=1; i<argv; i++)
    {
        if (strcmp(args[i], "-l")!=0)
        {
            return args[i];
        }
    }

    return NULL;
}

/**
 * This function gets the name of the actor
 *
 * @param line
 * @param actor_name
 */
void get_actor_name(char* line, char **actor_name)
{
    size_t size = strlen(line);
    *actor_name = malloc(sizeof(char)*size);
    memory_check(*actor_name);
    for (int i=0; i<size-1; i++)
    {
        (*actor_name)[i] = line[i];
    }

    (*actor_name)[size-1] = '\0';
}

/**
 *
 * Function is_movie
 *
 * This function checks if the line is a movie
 *
 * @param line
 * @param movie_name
 * @return
 */
int is_movie(char *line, char **movie_name)
{
    int extra = 7;
    int str_len = strlen(line);
    if (str_len<extra){
        return -1;
    }

    size_t movie_size = strlen(line) - extra;
    *movie_name = malloc(sizeof(char)*movie_size);
    memory_check(*movie_name);
    int read_count = sscanf(line, "Movie: %s\n", *movie_name);

    if (read_count==1)
    {
       for (int i=0; i<movie_size-1; i++)
       {
           (*movie_name)[i] = line[i+7];
       }

        (*movie_name)[movie_size-1] = '\0';
        return 1;
    }

    return -1;
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
        free(filename);
        exit(1);
    }

    return fp;
}

