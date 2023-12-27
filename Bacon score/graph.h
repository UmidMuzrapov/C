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

/********** The following lines are struct definitions *******/

typedef struct vertex vertex;
typedef struct edge edge;
typedef struct actor_node actor_node;

/**
 * struct vertex represents a graph node/vertex.
 */
struct vertex
{
    vertex *next_p;
    char *val_p;
    // pointer to the first edge
    edge *edge_p;
    // indicates if the vertex was visited or not
    int marked;
    int level;
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
    char *movie_name;
    actor_node *actor;
};

/**
 * struct command represent a command that can be associated with a rule.
 */
struct actor_node
{
    vertex *actor;
    actor_node *next_p;
};

/********* The following lines are function function prototypes **********/

// their definitions and documentation are given in graph.c.
vertex *add_node(vertex **graph, char *arg1);

void free_internal(vertex **graph);

void add_edge(vertex *graph, edge *movie_list, char *arg1, char *arg2);

void add_commands(char *commands, vertex *graph_p, char *cur_target);

int post_order(char *target, vertex *graph);

void add_movie(edge **movie_list, char *movie_name);

void add_actor_to_movie(vertex *graph, edge *movie_list, char *actor_name, char *movie_name);

int BFS(vertex *graph, edge *movie_list, char *actor_name);

void free_movies(edge *e);

#endif //GITHUB_GRAPH_H
