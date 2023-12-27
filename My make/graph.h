/**
 * Author: Umidjon Muzrapov
 *
 * This is a header file for graph.c
 * It includes function prototypes and struct definitions
 * that can be shared with other source files.
 */

#ifndef GITHUB_GRAPH_H
#define GITHUB_GRAPH_H

#include <stdlib.h>
#include <time.h>

/********** The following lines are struct definitions *******/

typedef struct vertex vertex;
typedef struct edge edge;
typedef struct command command;

/**
 * struct vertex represents a graph node/vertex.
 */
struct vertex
{
    vertex *next_p;
    char *val_p;
    // pointer to the first edge
    edge *edge_p;
    // pointer to the first command
    command *commands_p;
    // indicates if the vertex was visited or not
    int marked;
    int target;
    int must_build;
    int completed;
    struct timespec file_date;
};

/**
 * struct edge represents an edge of the graph
 */
struct edge
{
    // points to the next edge that belong to the same vertex
    edge *next_p;
    // the vertex the edge connect the vertex to
    vertex *to_p;
};

/**
 * struct command represent a command that can be associated with a rule.
 */
struct command
{
    char *val;
    command *next_p;
};

/********* The following lines are function function prototypes **********/

// their definitions and documentation are given in graph.c.
vertex *add_node(vertex **graph, char *arg1, int target);

void free_internal(vertex **graph);

void add_edge(vertex *graph, char *arg1, char *arg2);

void add_commands(char *commands, vertex *graph_p, char *cur_target);

int post_order(char *target, vertex *graph);

#endif //GITHUB_GRAPH_H
