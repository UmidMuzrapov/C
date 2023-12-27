/**
 * Author: Umid Muzrapov
 *
 * Purpose:
 * The program is invoked 'mymake -f aMakeFile aTarget'.
 * It reads target and dependencies specified in aMakeFile
 * and constructs the dependency graph. Then, it traverse the graph
 * using a postorder traversal, starting at the node corresponding to the target
 * aTarget. The behaviour of the file is as close as possible to that of c make.
 *
 * Date: 11/18/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#include "graph.h"
//#include "getline.h"
#include "makereader.h"
#include <string.h>

// used to keep track of the latest target
char *cur_target = NULL;

/**
 * Function free_memory
 *
 * This function free graph and cur_target to avoid memory leaks
 *
 * @param graph pointer to the graph reference
 */
void free_memory(vertex **graph, char *processed_args[])
{
    free_internal(graph);

    free(processed_args[0]);
    free(processed_args[1]);

    free(cur_target);
}

/**
 * Function validate_input
 *
 * The function validates the number of command line arguments.
 *
 * @param argv the number of command line arguments
 */
int validate_input(int argv, char *args[], char *processed_args[])
{
    // source file name, target name, and name of make file
    int number_of_allowed_f = 1;
    int argv_no_f_given = 2;
    int argv_f_given_min = 3;
    int argv_f_given_max = 4;

    int f_count = count_f(argv, args);

    if (f_count > number_of_allowed_f)
    {
        print_error("More than one -f was given.");
        return -1;
    }

    if (f_count == 0 && (argv > argv_no_f_given))
    {
        print_error("The wrong number of arguments was given.");
        return -1;
    }

    if (f_count == 1 && (argv < argv_f_given_min || argv > argv_f_given_max))
    {
        print_error("The wrong number of arguments was given.");
        return -1;
    }

    processed_args[0] = malloc(65 * sizeof(char));
    memory_check(processed_args[0]);
    char* makefile_name = get_makefile(argv, args);

    if (makefile_name == NULL)
    {
        if (f_count == 1)
        {
            print_error("The wrong number of arguments was given.");
            return -1;
        } else
        {
            strcpy(processed_args[0], "myMakefile");
        }
    }

    else
    {
        strcpy(processed_args[0], makefile_name);
    }

    char* target_name = get_command_target(argv, args);
    if (target_name==NULL)
    {
        processed_args[1] = NULL;
    }

    else
    {
        processed_args[1] = malloc(65*sizeof (char));
        memory_check(processed_args[1]);
        strcpy(processed_args[1], target_name);
    }

    return 1;
}

/**
 * Function add_dependencies
 *
 * The function adds dependencies as edges to the target vertex.
 *
 * @param line a line from the input stream
 * @param dependency_start index where the dependency section begins
 * @param graph_pp a pointer to the graph reference
 * @param target name of the target
 */
void add_dependencies(char *line, int dependency_start, vertex **graph_pp, char *target)
{
    char *word = malloc(sizeof(char) * (strlen(line) + 1));
    memory_check(word);
    char prev = ' ';
    char *curP = (line + dependency_start + 1);
    int word_index = 0;

    // for each dependency, add dependency as a vertex and create
    // edge between target and dependency
    while (*(curP) != '\0')
    {
        if (isspace(*curP) == 0)
        {
            prev = *curP;
            word[word_index] = *curP;
            word_index++;
        } else if (isspace(prev) == 0 && (isspace(*curP) || *curP == '\0'))
        {
            word[word_index] = '\0';
            add_node(graph_pp, word, 0);
            add_edge(*graph_pp, target, word);
            prev = *curP;
            free(word);
            word = malloc(sizeof(char) * 65);
            memory_check(word);
            word_index = 0;
        }

        curP++;
    }

    free(word);
}

/**
 * This function checks if the target exists.
 *
 * @return 1 if success. -1 if the target does not exist.
 */
int check_target_exits()
{
    if (cur_target == NULL)
    {
        print_error("Command before target.");
        return -1;
    }

    return 1;
}

/**
 * Function process_line
 *
 * The function processes a line read from the
 * input stream. It handles three cases:
 * a line is a command, a line is a rule,
 * a line is just spaces.
 *
 * @param line a line read from the input stream
 * @param graph_pp a pointer to the graph reference
 *
 * @return 1 if the line is processed successfully. -1 otherwise.
 */
int process_line(char *line, vertex **graph_pp, size_t buff_size, char *processed_args[])
{
    add_end_line(line);

    // if it is a command
    if (begin_with_tab(line) == 1)
    {
        if (check_target_exits() < 0)
        {
            return -1;
        }

        char *fline_p = process_tab(line, buff_size);
        add_commands(fline_p, *graph_pp, cur_target);
        free(fline_p);
    }

        // if all spaces
    else if (is_all_space(line) == 1)
    {
        return 1;
    } else
    {
        int dependency_start = find_dependency_start(line);
        if (dependency_start == -1)
        {
            print_error("Inappropriate format for the rule");
            return -1;
        }

        int target_validation = validate_target(line, dependency_start);
        if (target_validation < 0)
        {
            print_error("Inappropriate format for the target");
            return -1;
        }

        char *target_name = get_target_name(line, dependency_start);

        if (processed_args[1] == NULL)
        {
            processed_args[1] = malloc(65 * sizeof(char));
            memory_check(processed_args[1]);
            strcpy(processed_args[1], target_name);
        }

        free(cur_target);
        cur_target = malloc(sizeof(char) * (strlen(target_name) + 1));
        memory_check(cur_target);
        strcpy(cur_target, target_name);

        // if the target could not be added -> duplicate target or memory issue
        if (add_node(graph_pp, target_name, 1) == NULL)
        {
            free(target_name);
            return -1;
        }

        add_dependencies(line, dependency_start, graph_pp, target_name);
        free(target_name);
    }

    return 1;
}


/**
 * Function construct_dependency_graph
 *
 * The function constructs dependency graph, through the coordination
 * of a number of functions. The graph is constructed based on data from
 * a MakeFile.
 *
 * @param filename the name of the file to read data from
 * @return a pointer to the dependency graph. Null if error is encountered.
 */
vertex *construct_dependency_graph(char *processed_args[])
{
    FILE *fp = open_file(processed_args[0]);
    //FILE* fp = stdin;
    vertex *graph_p = NULL;
    char *line = NULL;
    size_t buff_size = 0;

    while (getline(&line, &buff_size, fp) != EOF)
    {
        int process_status = process_line(line, &graph_p, buff_size, processed_args);

        // if line processing failed.
        if (process_status < 0)
        {
            fclose(fp);
            free(line);
            free_internal(&graph_p);
            return NULL;
        }
    }

    fclose(fp);
    free(line);

    if (graph_p == NULL)
    {
        print_error("No target exist. Empty file");
    }

    return graph_p;
}

/**
 * This is the beginning point of the program.
 *
 * @param argv number of arguments
 * @param args an array of command-line arguments
 * @return 0 if normal. 1 otherwise
 */
int main(int argv, char *args[])
{


    char* processed_args[2];
    processed_args[0] = NULL;
    processed_args[1] = NULL;

    int input_validation_status = validate_input(argv, args, processed_args);

    // if input is invalid
    if (input_validation_status < 0)
    {
        free(processed_args[0]);
        free(processed_args[1]);
        exit(1);
    }

    vertex *graph = construct_dependency_graph(processed_args);
    // if the graph could be constructed
    if (graph == NULL)
    {
        free_memory(&graph, processed_args);
        exit(1);
    }

    int status = post_order(processed_args[1], graph);

    // if post order was faulty
    if (status < 0)
    {
        free_memory(&graph, processed_args);
        exit(1);
    }

    free_memory(&graph, processed_args);
    return exit_status;
}


