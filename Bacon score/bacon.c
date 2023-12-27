/**
 * Author: Umid Muzrapov
 *
 * Purpose:
 * The program is invoked 'bacon [-f] filename'.
 * It reads movie file and constructs a graph where edges are movies.
 * Vertices are actors. After that the user can query bacon score for an actor,
 * which shows through how many movies the actor is connected to Kevin Bacon.
 *
 * Date: 11/18/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "read_write.h"
#include "graph.h"
//#include "getline.h"

char* cur_movie_name = NULL;

/**
 * Function valdiate input
 *
 * This function validates the command line input
 *
 * @param argv number of command line arguments
 * @param args command line arguments
 * @param movie_name_p name of the movie file
 * @return >0 if valid. <0 otherwise.
 */
int validate_input(int argv, char *args[], char** movie_name_p)
{
    int min_argv = 2;

    if (argv<min_argv)
    {
        print_error("The wrong number of arguments was given.");
        return -1;
    }

    int l_count = count_l(argv, args);
    int file_count = count_file_names(argv, args);

    if (l_count<0 || file_count!=1)
    {
        print_error("The wrong format. bacon [-l] inFile");
        return -1;
    }

    /*if (argv==(l_count+1))
    {
        printf("The file name was not given.");
        return -1;
    }

    if (argv>min_argv && l_count==0)
    {
        printf("Too many file names were given.");
        return -1;
    }

    if (argv>min_argv && (l_count+2!=argv))
    {
        printf("Too many file names were given.");
        return -1;
    }*/

    *movie_name_p = malloc(65*sizeof (char));
    memory_check(*movie_name_p);
    strcpy(*movie_name_p, get_file_name(argv, args));
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
void process_line(char *line, vertex **graph_pp,  edge** movie_list_pp)
{

    add_end_line(line);
        // if all spaces
    if (is_all_space(line) == 1)
    {
        return;
    } else
    {
        char* movie_name = NULL;
        int movie_check = is_movie(line, &movie_name);

        if (movie_check>0)
        {
            free(cur_movie_name);
            cur_movie_name = malloc(sizeof (char) * (strlen(movie_name)+1));
            memory_check(cur_movie_name);
            strcpy(cur_movie_name, movie_name);
            add_movie(movie_list_pp, cur_movie_name);
        }

        else
        {
            char* actor_name = NULL;
            get_actor_name(line, &actor_name);
            add_node(graph_pp, actor_name);
            add_actor_to_movie(*graph_pp, *movie_list_pp, actor_name, cur_movie_name);
            add_edge(*graph_pp, *movie_list_pp, actor_name, cur_movie_name);
            free(actor_name);
        }

        free(movie_name);
    }

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
vertex *construct_graph(char *file_name, edge** movie_list)
{
    FILE *fp = open_file(file_name);
    //FILE* fp = stdin;
    vertex *graph_p = NULL;
    char *line = NULL;
    size_t buff_size = 0;

    while (getline(&line, &buff_size, fp) != EOF)
    {
        process_line(line, &graph_p, movie_list);

        // if line processing failed.
        /*
        if (process_status < 0)
        {
            fclose(fp);
            free(line);
            free_internal(&graph_p);
            return NULL;
        }*/
    }

    fclose(fp);
    free(line);

    if (graph_p == NULL)
    {
        //print_error("No target exist. Empty file");
    }

    return graph_p;
}

/**
 * Function print_movie_list
 *
 * This function prints a list of movies for debugging purposes
 *
 * @param movie_list a list of movies
 */
void print_movie_list(edge *movie_list)
{
    edge* cur_p =movie_list;
    while(cur_p!=NULL)
    {
        printf("Movie:%s\n", cur_p->movie_name);
        actor_node * cur_actor = cur_p->actor;
        while (cur_actor !=NULL)
        {
            printf("Actor:%s\n", cur_actor->actor->val_p);
            cur_actor = cur_actor->next_p;
        }

        cur_p=cur_p->next_p;
    }
}

/**
 * Function print score
 *
 * This function prints score
 *
 * @param score score of an actor
 */
void print_score(int score)
{
    if (score==-2)
    {
        printf("Score: No Bacon!\n");
    }

    else if (score>=0)
    {
        printf("Score: %d\n", score);
    }
}

/**
 * Function answer_query
 *
 * This function actor name and get the score for them.
 *
 * @param graph pointer to graph
 * @param movie_list a pointer to a list of movies
 */
void answer_query(vertex* graph, edge* movie_list)
{
    char *line = NULL;
    size_t buff_size = 0;
    FILE* fp = stdin;
    int input_read = getline(&line, &buff_size, fp);
    add_end_line(line);
    char* actorName = NULL;
    get_actor_name(line, &actorName);

    while (input_read>0)
    {
        if (is_all_space(line)==1)
        {
            input_read = getline(&line, &buff_size, fp);
            add_end_line(line);
            free(actorName);
            actorName = NULL;
            get_actor_name(line, &actorName);
            continue;
        }

        int score=BFS(graph, movie_list, actorName);
        print_score(score);
        input_read = getline(&line, &buff_size, fp);
        add_end_line(line);
        free(actorName);
        actorName = NULL;
        get_actor_name(line, &actorName);
    }

    free(line);
    free(actorName);
}

/**
 * This is where the program begins.
 *
 * @param argv number of command-line arguments
 *
 * @param args command-line arguments
 * @return 0 if success. 1 otherwise.
 */
int main(int argv, char * args[])
{
    char* file_name = NULL;
    int input_validation_status = validate_input(argv, args, &file_name);

    if (input_validation_status<0)
    {
        free(file_name);
        exit(1);
    }

    edge* movie_list = NULL;
    vertex *graph = construct_graph(file_name, &movie_list);
    answer_query(graph, movie_list);
    free_internal(&graph);
    free_movies(movie_list);
    free(cur_movie_name);
    free(file_name);
    //free_movies(movie_list);
    exit(exit_status);
}
