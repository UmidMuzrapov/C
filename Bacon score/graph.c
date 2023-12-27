/*
 * Author: Umidjon Muzrapov
 *
 * The source file contains functions
 * that are logically related and represent graph
 * operations -- such as adding edge, vertex, adding commands,
 * and post order traversal. The are also helper function definition,
 * which are not included in the header file.
 */
#include "graph.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "queue.h"

/**
 * This method check if the vertex already exitst.
 * @param graph pointer to the graph
 * @param arg1 name of the vertex 1
 * @return pointer to the vertex if exists. Null otherwise.
 */
vertex *vertex_exists(vertex *graph, char *arg1)
{
    vertex *cur_p = graph;

    while (cur_p != NULL)
    {
        // vertex who values matches
        if (strcmp(cur_p->val_p, arg1) == 0)
        {
            return cur_p;
        }
        cur_p = cur_p->next_p;
    }

    return NULL;
}

/**
 * The method checks if all characters are all alpha.
 * @param name name of the vertex
 * @return 0 if not all alpha. 1 otherwise.
 */
int is_all_alpha(char *name)
{
    // this method is not used. But can be used later
    char *cur_p = name;
    while ((*cur_p) != '\0')
    {
        if (isalpha(*cur_p) == 0)
        {
            return 0;
        }
        cur_p++;
    }

    return 1;
}

/**
 * This method finds checks if the edge between two vertices
 * exists.
 *
 * @param graph pointer to the graph
 * @param from_p from vertex pointer
 * @param to_p to vertex pointer
 * @return edge if exists. NULL otherwise.
 *
 */
edge *find_edge(vertex *graph, vertex *from_p, vertex *to_p)
{
    vertex *cur_p = graph;

    // to find edge both from and to should match
    while (cur_p != NULL)
    {
        if (cur_p == from_p)
        {
            edge *cur_edge_p = cur_p->edge_p;
            while (cur_edge_p != NULL)
            {
                if ((cur_edge_p->to_p) == to_p)
                {
                    return cur_edge_p;
                }
                cur_edge_p = cur_edge_p->next_p;
            }
        }

        cur_p = cur_p->next_p;
    }

    return NULL;
}

/**
 * This method adds a node to the graph
 *
 * @param graph pointer to the graph reference
 * @param arg1 value of the new vertice
 */
vertex *add_node(vertex **graph, char *arg1)
{
    // base case
    if (*graph == NULL)
    {
        *graph = malloc(sizeof(vertex));
        memory_check(*graph);
        (*graph)->val_p = malloc(sizeof(char) * (strlen(arg1) + 1));
        memory_check((*graph)->val_p );
        strcpy((*graph)->val_p, arg1);
        (*graph)->next_p = NULL;
        (*graph)->edge_p = NULL;
        (*graph)->level = 0;
        (*graph)->marked = 0;
        return *graph;
    }

    vertex *new_vertex_p = malloc(sizeof(vertex));
    memory_check(new_vertex_p);
    vertex *is_vertex_in_graph = vertex_exists(*graph, arg1);

    // if the vertex already exists
    if (is_vertex_in_graph != NULL)
    {
        // if the name of the vertex appear again, do nothing.
        free(new_vertex_p);
        return is_vertex_in_graph;

    } else
    {
        // if vertex does not exist yet, create a new one
        vertex *prev_p = NULL;
        vertex *cur_p = *graph;

        while (cur_p != NULL)
        {
            prev_p = cur_p;
            cur_p = cur_p->next_p;
        }

        new_vertex_p->next_p = NULL;
        new_vertex_p->level = 0;
        new_vertex_p->marked = 0;
        new_vertex_p->val_p = malloc((strlen(arg1) + 1) * sizeof(char));
        memory_check(new_vertex_p->val_p);
        strcpy(new_vertex_p->val_p, arg1);
        new_vertex_p->edge_p = NULL;
        prev_p->next_p = new_vertex_p;
    }

    return new_vertex_p;
}

/**
 * Function initialize_graph
 *
 * The function prepares the graph for post-order traversal,
 * marking all vertices as unmarked.
 *
 * @param graph a pointer to the graph
 */
void initialize_graph(vertex *graph)
{
    vertex *curP = graph;
    while (curP != NULL)
    {
        curP->marked = 0;
        curP->level = 0;
        curP = curP->next_p;
    }
}

/**
 * Function post_order_recursive
 *
 * The function is an auxilary recursive function
 * that traverses graph post-order and processes commands.
 *
 * @param target_p a pointer to the target vertex
 * @param graph a pointer to the graph
 */
void post_order_recursive(vertex *target_p, vertex *graph)
{
    if (target_p->marked == 1) return;

    target_p->marked = 1;
    edge *cur_edge_p = target_p->edge_p;

    // for each edge do post-order traversal
    while (cur_edge_p != NULL)
    {
        post_order_recursive(cur_edge_p->to_p, graph);
        cur_edge_p = cur_edge_p->next_p;
    }

    // print out command associate with target
    printf("%s\n", target_p->val_p);
}

/**
 * Function post_order
 *
 * The function traverse the graph post-order.
 * Doing so, it prints commands the vertices associated with.
 * @param target the name of the target
 * @param graph a pointer to the graph
 *
 * return 1 if successfully traverse. -1 otherwise.
 */
int post_order(char *target, vertex *graph)
{
    vertex *target_p = vertex_exists(graph, target);
    if (target_p == NULL)
    {
        print_error("The actor is not in the graph.");
        return -1;
    }

    // initialize graph -> mark all vertices as unmarked.
    initialize_graph(graph);
    post_order_recursive(target_p, graph);
    return 1;
}

/**
 * Function find_movie
 *
 * This function find a movie given a movie name
 *
 * @param movie_list
 * @param movie_name
 * @return
 */
edge *find_movie(edge *movie_list, char *movie_name)
{
    edge *cur_p = movie_list;
    edge *result = NULL;

    while (cur_p != NULL)
    {
        if (strcmp(cur_p->movie_name, movie_name) == 0)
        {
            result = cur_p;
        }

        cur_p = cur_p->next_p;
    }

    return result;
}

/**
 * Function add_movie
 *
 * This function add a movie to a movie list.
 *
 * @param movie_list
 * @param movie_name
 */
void add_movie(edge **movie_list, char *movie_name)
{
    if (*movie_list == NULL)
    {
        edge *new_edge = malloc(sizeof(edge));
        memory_check(new_edge);
        new_edge->next_p = NULL;
        new_edge->to_p = NULL;
        new_edge->movie_name = malloc((strlen(movie_name) + 1) * sizeof(char));
        memory_check(new_edge->movie_name);
        strcpy(new_edge->movie_name, movie_name);
        new_edge->actor = NULL;
        *movie_list = new_edge;
        return;
    }


    edge *cur_p = *movie_list;
    edge *prev = NULL;

    while (cur_p != NULL)
    {
        prev = cur_p;
        cur_p = cur_p->next_p;
    }

    edge *new_edge = malloc(sizeof(edge));
    memory_check(new_edge);
    new_edge->next_p = NULL;
    new_edge->to_p = NULL;
    new_edge->movie_name = malloc((strlen(movie_name) + 1) * sizeof(char));
    memory_check( new_edge->movie_name);
    strcpy(new_edge->movie_name, movie_name);
    new_edge->actor = NULL;
    prev->next_p = new_edge;

}


/**
 * This method adds an edge.
 * If the edge already exists, ignore.
 * @param graph pointer to graph
 * @param arg1 name of vertex 1
 * @param arg2 name of vertex 2
 * @param dist the distance between vertices 1 and 2
 */
void add_edge(vertex *graph, edge *movie_list, char *arg1, char *arg2)
{
    vertex *from_p = vertex_exists(graph, arg1);
    edge *movie_name = find_movie(movie_list, arg2);
    edge *movie_name_copy = malloc(sizeof(edge));
    memory_check(movie_name_copy);
    movie_name_copy->next_p = NULL;
    movie_name_copy->movie_name = malloc((strlen(movie_name->movie_name) + 1) * sizeof(char));
    memory_check(movie_name_copy->movie_name);
    strcpy(movie_name_copy->movie_name, movie_name->movie_name);
    movie_name_copy->actor = movie_name->actor;

    // one of the vertices of the edge does not exist
    if (from_p == NULL || movie_name == NULL)
    {
        print_error("Illegal edge.");
        return;
    }

    // base
    if (from_p->edge_p == NULL)
    {
        from_p->edge_p = movie_name_copy;
        return;
    }

    edge *prev_edge_p = NULL;
    edge *cur_edge_p = from_p->edge_p;

    while (cur_edge_p != NULL)
    {
        prev_edge_p = cur_edge_p;
        cur_edge_p = cur_edge_p->next_p;
    }

    prev_edge_p->next_p = movie_name_copy;
}

/**
 * Function add_commands
 *
 * The function associates the given commands with the last
 * target.
 *
 * @param commands commands to associate the last target with
 * @param graph_p a pointer to the graph
 * @param cur_target the last read target
 */
void add_commands(char *commands, vertex *graph_p, char *cur_target)
{

}

/**
 * Function free_actors
 *
 * This function frees a list of actors
 *
 * @param actorNode a pointer to actor list
 */
void free_actors(actor_node **actorNode)
{
    actor_node *cur_p = *actorNode;

    while (cur_p != NULL)
    {
        actor_node *temp = cur_p;
        cur_p = cur_p->next_p;
        free(temp);
    }

    *actorNode = NULL;
}


/**
 * This funciton free the memory allocated to edges of a certain vertex.
 *
 * @param e the start of the edges for a certain vertex
 */
void free_edge(edge **e)
{
    edge *cur_edge_p = *e;

    while (cur_edge_p != NULL)
    {
        //free_actors(&(cur_edge_p->actor));
        //cur_edge_p->actor = NULL;
        free(cur_edge_p->movie_name);
        edge *temp = cur_edge_p;
        cur_edge_p = cur_edge_p->next_p;
        free(temp);
    }

    *e = NULL;
}

/**
 * Function free_movies
 *
 * This function frees movie list
 *
 * @param e pointer to movie list
 */
void free_movies(edge *e)
{
    edge *cur_edge_p = e;

    while (cur_edge_p != NULL)
    {
        free(cur_edge_p->movie_name);
        free_actors(&(cur_edge_p->actor));
        edge *temp = cur_edge_p;
        cur_edge_p = cur_edge_p->next_p;
        free(temp);
    }
}

/** This function is a custome memory free method to
* strategically free allocated memory.
* @param: poitner to the graph reference.
 */
void free_internal(vertex **graph)
{
    vertex *cur_p = *graph;

    // for each vertex, free commands and edges
    while (cur_p != NULL)
    {
        free_edge(&(cur_p->edge_p));
        free(cur_p->val_p);
        vertex *temp = cur_p;
        cur_p = cur_p->next_p;
        free(temp);
    }

    *graph = NULL;
}

/**
 * Function add_actor_to_movie
 *
 * This function adds actor to a movie
 *
 * @param graph
 * @param movie_list
 * @param actor_name
 * @param movie_name
 */
void add_actor_to_movie(vertex *graph, edge *movie_list, char *actor_name, char *movie_name)
{
    edge *movie_found = find_movie(movie_list, movie_name);
    vertex *actor_found = vertex_exists(graph, actor_name);
    actor_node *new_actor = malloc(sizeof(actor_node));
    memory_check(new_actor);
    new_actor->actor = actor_found;
    new_actor->next_p = NULL;

    // base case
    if (movie_found->actor == NULL)
    {
        movie_found->actor = new_actor;
        return;
    }

    actor_node *cur_p = movie_found->actor;
    actor_node *prev = NULL;

    while (cur_p != NULL)
    {
        prev = cur_p;
        cur_p = cur_p->next_p;
    }

    prev->next_p = new_actor;
}

/**
 * Function free_head
 *
 * This function frees queue
 * @param head
 */
void free_head(queue_node** head)
{
    queue_node* cur_p = *head;
    while (cur_p!=NULL)
    {
        queue_node * temp = cur_p;
        cur_p=cur_p->next;
        free(temp);
    }

    *head = NULL;
}

/**
 * Function free_tail
 *
 * This function frees tail
 *
 * @param tail
 */
void free_tail(queue_node** tail)
{
    queue_node* cur_p = *tail;
    while (cur_p!=NULL)
    {
        queue_node * temp = cur_p;
        cur_p=cur_p->previous;
        free(temp);
        printf("test");
    }

    *tail=NULL;
}

/**
 * Function BFS
 *
 * This function does bfs search to find the bacon score.
 *
 * @param graph
 * @param movie_list
 * @param actor_name
 * @return
 */
int BFS(vertex *graph, edge *movie_list, char *actor_name)
{
    initialize_graph(graph);
    vertex *start = vertex_exists(graph, "Kevin Bacon");
    vertex *target = vertex_exists(graph, actor_name);

    if (target == NULL)
    {
        print_error("The actor is not entered.");
        return -1;
    }

    if (start == NULL)
    {
        return -2;
    }

    if (start == target)
    {

        return 0;
    }

    start->level = 0;
    start->marked = 1;
    queue_node *head = NULL;
    queue_node *tail = NULL;
    add_last(&head, &tail, start);

    while (head != NULL)
    {
        queue_node* q_node=remove_first(&head, &tail);
        vertex *actor = (vertex *) (q_node->val_p);
        free(q_node);
        edge *actor_movie_list = actor->edge_p;

        while (actor_movie_list != NULL)
        {
            actor_node *cur_actor_p = actor_movie_list->actor;
            while (cur_actor_p != NULL)
            {
                if (target == (cur_actor_p->actor))
                {
                    if (head!=NULL)
                    {
                        free_head(&head);
                    }

                    //free_tail(&tail);
                    return (actor->level + 1);
                }

                if (cur_actor_p->actor->marked == 0)
                {
                    cur_actor_p->actor->marked = 1;
                    cur_actor_p->actor->level = actor->level + 1;
                    add_last(&head, &tail, cur_actor_p->actor);
                }

                cur_actor_p = cur_actor_p->next_p;
            }
            actor_movie_list = actor_movie_list->next_p;
        }

    }

    //free_head(&head);
    //free_tail(&tail);
    return -2;
}

