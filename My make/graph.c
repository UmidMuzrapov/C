/*
 * Author: Umidjon Muzrapov
 *
 * The source file contains functions
 * that are logically related and represent graph
 * operations -- such as adding edge, vertex, adding commands,
 * and post order traversal. There are also helper function definition,
 * which are not included in the header file.
 */

#include "graph.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

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
vertex *add_node(vertex **graph, char *arg1, int target)
{
    // base case
    if (*graph == NULL)
    {
        *graph = malloc(sizeof(vertex));
        memory_check(*graph);
        (*graph)->val_p = malloc(sizeof(char) * (strlen(arg1) + 1));
        strcpy((*graph)->val_p, arg1);
        (*graph)->next_p = NULL;
        (*graph)->edge_p = NULL;
        (*graph)->commands_p = NULL;
        (*graph)->target = target;
        return *graph;
    }

    vertex *new_vertex_p = malloc(sizeof(vertex));
    memory_check(new_vertex_p);
    vertex *is_vertex_in_graph = vertex_exists(*graph, arg1);

    // if the vertex already exists
    if (is_vertex_in_graph != NULL)
    {
        if (is_vertex_in_graph->target == 0)
        {
            // if it was not target before and now target, mark as target
            if (target == 1)
            {
                is_vertex_in_graph->target = 1;
            }
        } else
        {
            // the target cannot be repeated
            if (target == 1)
            {
                print_error("Target is declared more than once.");
                free(new_vertex_p);
                return NULL;
            }
        }

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
        new_vertex_p->val_p = malloc((strlen(arg1) + 1) * sizeof(char));
        memory_check(new_vertex_p->val_p);
        strcpy(new_vertex_p->val_p, arg1);
        new_vertex_p->edge_p = NULL;
        new_vertex_p->target = target;
        new_vertex_p->commands_p = NULL;
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
        curP->completed = 0;
        curP->must_build = 0;
        curP = curP->next_p;

    }
}

/**
 * Function process_commands
 *
 * The function processes commands associated with the target.
 *
 * @param target_p pointer to target vertex
 * @return -1 if the command failed
 */
int process_commands(vertex *target_p)
{
    command *cur_command_p = target_p->commands_p;
    // print out command associate with target

    while (cur_command_p != NULL)
    {
        printf("%s", cur_command_p->val);
        int system_call_status = system(cur_command_p->val);

        // setting up call fails
        if (system_call_status != 0)
        {
            print_error("Command failed.");
            return -1;
        } else
        {
            // if rc 127, the command cannot be executed.
            int rc = WEXITSTATUS(system_call_status);
            if (rc == 127)
            {
                print_error("Command failed.");
                return -1;
            }
        }

        cur_command_p = cur_command_p->next_p;
    }

    target_p->file_date = get_last_modified_date(target_p->val_p);
    //update_time_stamp(target_p->val_p);

    return 1;
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
int post_order_recursive(vertex *target_p, vertex *graph, int *any_update)
{
    if (target_p->marked == 1) return 1;

    target_p->marked = 1;
    int file_exist = file_exists(target_p->val_p);
    if (file_exist == FILE_EXISTS)
    {
        target_p->file_date = get_last_modified_date(target_p->val_p);
    }

    if (file_exist != FILE_EXISTS)
    {
        if (target_p->target != 1)
        {
            print_error("File does not exists and it is not a target.");
            return -1;
        } else
        {
            target_p->must_build = 1;
        }
    }

    edge *cur_edge_p = target_p->edge_p;

    // for each edge do post-order traversal
    while (cur_edge_p != NULL)
    {
        if (post_order_recursive(cur_edge_p->to_p, graph, any_update) == -1)
        {
            return -1;
        }

        if (cur_edge_p->to_p->completed == 0)
        {
            fprintf(stderr, "Circular %s <- %s dependency dropped.\n", cur_edge_p->to_p->val_p, target_p->val_p);
        } else
        {
            if (target_p->must_build == 0)
            {
                cur_edge_p->to_p->file_date = get_last_modified_date(cur_edge_p->to_p->val_p);
                if (file_exists(target_p->val_p) != 0 ||
                    (compare_time(cur_edge_p->to_p->file_date, target_p->file_date) == 1))
                {
                    target_p->must_build = 1;
                }
            }
        }

        cur_edge_p = cur_edge_p->next_p;
    }

    if (target_p->must_build == 1)
    {
        *any_update = 1;
        if (process_commands(target_p) == -1)
        {
            return -1;
        }
    }

    target_p->completed = 1;
    return 1;
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
    int any_update = 0;

    vertex *target_p = vertex_exists(graph, target);
    if (target_p == NULL || (target_p->target == 0))
    {
        print_error("The target does not exist.");
        return -1;
    }

    // initialize graph -> mark all vertices as unmarked.
    initialize_graph(graph);
    if (post_order_recursive(target_p, graph, &any_update) == -1)
    {
        return -1;
    }

    if (any_update == 0)
    {
        printf("%s is up to date.\n", target);
    }

    return 1;
}

/**
 * This method adds an edge.
 * If the edge already exists, ignore.
 * @param graph pointer to graph
 * @param arg1 name of vertex 1
 * @param arg2 name of vertex 2
 * @param dist the distance between vertices 1 and 2
 */
void add_edge(vertex *graph, char *arg1, char *arg2)
{
    vertex *from_p = vertex_exists(graph, arg1);
    vertex *to_p = vertex_exists(graph, arg2);

    // one of the vertices of the edge does not exist
    if (from_p == NULL || to_p == NULL)
    {
        print_error("Illegal edge");
        return;
    }

    edge *is_edge_in_graph = find_edge(graph, from_p, to_p);
    // if the edge already exists, ignore it.
    if (is_edge_in_graph)
    {
        print_error("Edge already exists");
        return;
    }

    edge *new_edge = malloc(sizeof(edge));
    memory_check(new_edge);

    // initiate new edge
    new_edge->next_p = NULL;
    new_edge->to_p = to_p;

    if (from_p->edge_p == NULL)
    {
        from_p->edge_p = new_edge;
        return;
    }

    edge *prev_edge_p = NULL;
    edge *cur_edge_p = from_p->edge_p;

    while (cur_edge_p != NULL)
    {
        prev_edge_p = cur_edge_p;
        cur_edge_p = cur_edge_p->next_p;
    }

    prev_edge_p->next_p = new_edge;
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
    vertex *target_node_p = vertex_exists(graph_p, cur_target);

    // base case
    if (target_node_p->commands_p == NULL)
    {
        target_node_p->commands_p = malloc(sizeof(command));
        memory_check(target_node_p->commands_p);
        target_node_p->commands_p->val = malloc(sizeof(char) * (strlen(commands) + 1));
        memory_check(target_node_p->commands_p->val);
        target_node_p->commands_p->next_p = NULL;
        strcpy(target_node_p->commands_p->val, commands);
        return;
    }

    command *command_p = target_node_p->commands_p;
    command *prev_command_p = NULL;

    // otherwise traverse commands and add the new command at the end.
    while (command_p != NULL)
    {
        prev_command_p = command_p;
        command_p = command_p->next_p;
    }

    command *new_command = malloc(sizeof(command));
    memory_check(new_command);
    new_command->val = malloc(sizeof(char) * (strlen(commands) + 1));
    memory_check(new_command->val);
    strcpy(new_command->val, commands);
    new_command->next_p = NULL;
    prev_command_p->next_p = new_command;
}

/**
 * This funciton free the memory allocated to edges of a certain vertex.
 *
 * @param e the start of the edges for a certain vertex
 */
void free_edge(edge *e)
{
    edge *cur_edge_p = e;

    while (cur_edge_p != NULL)
    {
        edge *temp = cur_edge_p;
        cur_edge_p = cur_edge_p->next_p;
        free(temp);
    }
}

/**
 * Function command_p
 *
 * This method frees a list of command
 *
 * @param command_p a pointer to the first command associated with the vertex
 */
void free_command(command *command_p)
{
    command *cur_command_p = command_p;

    while (cur_command_p != NULL)
    {
        command *temp = cur_command_p;
        free(cur_command_p->val);
        cur_command_p = cur_command_p->next_p;
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
        free_edge(cur_p->edge_p);
        free_command(cur_p->commands_p);
        free(cur_p->val_p);
        vertex *temp = cur_p;
        cur_p = cur_p->next_p;
        free(temp);
    }

    *graph = NULL;
}

