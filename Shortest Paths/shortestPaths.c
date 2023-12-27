/**
 * File: shortestPaths.c
 * Purpose: The programs creates a graph from the given file.
 * Then it asnwers queries about the shortest distance between
 * two vertices. Assume the graph is connected.
 *
 * @author Umid Muzrapov
 * Class: CS352
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>

// The following lines are struct definitions
typedef struct vertex vertex;
typedef struct edge edge;
struct vertex {
    vertex *next_p;
    char *val_p;
    edge *edge_p;
    int marked;
    int minDist;
};
struct edge {
    edge *next_p;
    vertex *to_p;
    int dist;
};

// Function prototypes
void print_error(char[]);

void build_graph(FILE *, vertex **);

vertex *vertex_exists(vertex *, char *);

int dfs(vertex *, vertex *);

int find(vertex *, char *, char *);

void unmark(vertex *);

int add_node(vertex **, char *);

void add_edge(vertex *, char *, char *, int);

void memory_check(void *);

void initialize_graph(vertex *, vertex *);

vertex *find_min_distance_vertex(vertex *graph);

void find_shortest_paths(vertex *, vertex *);

void answer_query(vertex *);

edge *find_edge(vertex *graph, vertex *from_p, vertex *to_p);

int is_all_alpha(char *name);
void free_edge(edge*);
void free_internal(vertex**);


// used to keep track of the exit status
int exitCode = 0;

/**
 * This is the beginning point of the program.
 * @param argv number of command-line arguments
 * @param args a list of string arguments
 * @return 0 if normal. 1 otherwise
 */
int main(int argv, char *args[]) {
    FILE *fp = NULL;
    vertex *graph = NULL;

    if (argv < 2) {
        print_error("Too few arguments");
        exit(exitCode);
    }

    char *filename = args[1];
    fp = fopen(filename, "r");

    if (fp == NULL) {
        print_error("The file could not be opened.");
        exit(exitCode);
    }

    // build graph and answer queries
    build_graph(fp, &graph);
    answer_query(graph);
    fclose(fp);
    free_internal(&graph);
    free(graph);

    return exitCode;
}

// This method is a custome memory free method to
// strategically free allocated memory.
// param:
// 	poitner to the graph reference.
void free_internal(vertex** graph){
    vertex* cur_p = *graph;
    while(cur_p!=NULL){
        free_edge(cur_p->edge_p);
        free(cur_p->val_p);
        vertex* temp = cur_p;
        cur_p = cur_p->next_p;
        free(temp);
    }

    *graph = NULL;
}

//
//This method free the memory allocated to edges
//of a certain vertex.
//param:
//	e: the start of the edge for a certain vertex.
void free_edge(edge* e){
    edge* cur_edge_p= e;

    while(cur_edge_p!=NULL){
        edge* temp = cur_edge_p;
        cur_edge_p = cur_edge_p->next_p;
        free(temp);
    }
}


/**
 * The method builds a graph from the given file.
 * @param fp file pointer
 * @param graph pointer to graph reference
 */
void build_graph(FILE *fp, vertex **graph) {

    char *line = NULL;
    size_t bufsize = 0;
    int go_on = 1; // used to check edditional conditions
    int get_line_read = getline(&line, &bufsize, fp);

    while (get_line_read > 0 && go_on) {
        // variables to parse the line
        char *arg1_p = (char *) malloc(sizeof(char) * 65);
        char *arg2_p = (char *) malloc(sizeof(char) * 65);
        int *arg3_p = malloc(sizeof(int));
        // extra to check the validity of the command.
        char *arg4_p = (char *) malloc(sizeof(char) * 65);
        //memory checks
        memory_check(arg1_p);
        memory_check(arg2_p);
        memory_check(arg3_p);
        memory_check(arg4_p);

        int input_read = 0;
        input_read = sscanf(line, "%64s %64s %d %64s", arg1_p, arg2_p, arg3_p, arg4_p);

        if (input_read >= 4 || (input_read>=0 && input_read<3)) {
            print_error("Unrecognized command");
            continue;
        }

        if (input_read != 3) {
            if (feof(fp)) {
                go_on = 0;
            }

            if (ferror(fp)) {
                // error
                continue;
            }

            if (input_read<3){
                print_error("Illegal edge");
            }

        } else {
            // add first node
            if (add_node(graph, arg1_p)==-1 || add_node(graph, arg2_p)==-1){
                // no need to coninue
            } else{
                // add edges from point a to b, and b to a
                add_edge(*graph, arg1_p, arg2_p, *arg3_p);
                add_edge(*graph, arg2_p, arg1_p, *arg3_p);
            }

        }

        free(arg1_p);
        free(arg2_p);
        free(arg3_p);
        free(arg4_p);
        // read next line
        get_line_read = getline(&line, &bufsize, fp);
    }

    free(line);

}

/**
 * This method answer user queries about the
 * shortest paths between two vertices.
 * Assume the graph is connected.
 * @param graph a reference to graph
 */
void answer_query(vertex *graph) {
    // variables to read a line from stdin stream
    char *line = NULL;
    size_t bufsize = 0;
    FILE *fp = stdin;
    int read_item_get_line = getline(&line, &bufsize, fp);
    int go_on = 1;

    while (read_item_get_line > 0 && go_on) {
        // prepare variable to parse the string
        char *from_value_p = malloc(65 * sizeof(char));
        char *to_value_p = malloc(65 * sizeof(char));
        char *auxilary = malloc(65 * sizeof(char));
        memory_check(from_value_p);
        memory_check(to_value_p);
        memory_check(auxilary);

        int read_items = 0;

        read_items = sscanf(line, "%64s %64s %64s", from_value_p, to_value_p, auxilary);

        // if two items are read
        if (read_items == 2) {
            vertex *src_p = vertex_exists(graph, from_value_p);
            vertex *destination_p = vertex_exists(graph, to_value_p);
            if (src_p == NULL || destination_p == NULL) {
                print_error("One of the vertices does not exists");
                read_item_get_line = getline(&line, &bufsize, fp);
                continue;
            }

            initialize_graph(graph, src_p);
            find_shortest_paths(graph, src_p);
            printf("%d\n", destination_p->minDist);
        } else {
            // check if it is the end of line
            if (feof(fp)) {
                go_on = 0;
            }

            if (ferror(fp)) {
                print_error("Something went wrong with reading your file.");
                continue;
            }
        }

        free(from_value_p);
        free(to_value_p);
        free(auxilary);
        read_item_get_line = getline(&line, &bufsize, fp);
    }

    free(line);
}

/**
 * This method adds a node to the graph
 * @param graph pointer to the graph reference
 * @param arg1 value of the new vertice
 */
int add_node(vertex **graph, char *arg1) {
    // graph is empty: the base case
    int is_alpha = is_all_alpha(arg1);
    if (is_alpha == 0) {
        print_error("Name should contain only letters.");
        return -1;
    }

    if (*graph == NULL) {
        *graph = malloc(sizeof(vertex));
        memory_check(*graph);
        (*graph)->val_p = malloc(65);
        strcpy((*graph)->val_p, arg1);
        (*graph)->next_p = NULL;
        (*graph)->edge_p = NULL;
        return 0;
    }

    vertex *new_vertex_p = malloc(sizeof(vertex));
    memory_check(new_vertex_p);
    vertex *isVertexInGraph = vertex_exists(*graph, arg1);

    if (isVertexInGraph != NULL) {
        free(new_vertex_p);
    } else {
        vertex *prev_p = NULL;
        vertex *cur_p = *graph;

        while (cur_p != NULL) {
            prev_p = cur_p;
            cur_p = cur_p->next_p;
        }

        new_vertex_p->next_p = NULL;
        new_vertex_p->val_p = malloc(65);
        memory_check(new_vertex_p->val_p);
        strcpy(new_vertex_p->val_p, arg1);
        new_vertex_p->edge_p = NULL;
        prev_p->next_p = new_vertex_p;
    }

    return 0;
}

/**
 * The method checks if all characters are all alpha.
 * @param name name of the vertex
 * @return 0 if not all alpha. 1 otherwise.
 */
int is_all_alpha(char *name) {

    char *cur_p = name;
    while ((*cur_p) != '\0') {
        if (isalpha(*cur_p) == 0) {
            return 0;
        }
        cur_p++;
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
void add_edge(vertex *graph, char *arg1, char *arg2, int dist) {
    vertex *from_p = vertex_exists(graph, arg1);
    vertex *to_p = vertex_exists(graph, arg2);

    if (from_p == NULL || to_p == NULL) {
        print_error("Illegal edge");
        return;
    }

    edge *findEdge = find_edge(graph, from_p, to_p);
    // if the edge already exists, ignore it.
    if (findEdge) {
        print_error("Edge already exists");
        return;
    }

    edge *new_edge = malloc(sizeof(edge));
    memory_check(new_edge);

    // initiate new edge
    new_edge->next_p = NULL;
    new_edge->to_p = to_p;
    new_edge->dist = dist;

    if (from_p->edge_p == NULL) {
        from_p->edge_p = new_edge;
        return;
    }

    edge *prev_edge_p = NULL;
    edge *cur_edge_p = from_p->edge_p;

    while (cur_edge_p != NULL) {
        prev_edge_p = cur_edge_p;
        cur_edge_p = cur_edge_p->next_p;
    }

    prev_edge_p->next_p = new_edge;
}

/**
 * This method unmarks the graph before finding the shortest paths.
 * @param graph pointer to the graph
 */
void unmark(vertex *graph) {
    vertex *cur_p = graph;

    while (cur_p != NULL) {
        cur_p->marked = 0;
        cur_p = cur_p->next_p;
    }
}

/**
 * This method check if the vertex already exitst.
 * @param graph pointer to the graph
 * @param arg1 name of the vertex 1
 * @return pointer to the vertex if exists. Null otherwise.
 */
vertex *vertex_exists(vertex *graph, char *arg1) {
    vertex *cur_p = graph;

    while (cur_p != NULL) {
        if (strcmp(cur_p->val_p, arg1) == 0) {
            return cur_p;
        }
        cur_p = cur_p->next_p;
    }

    return NULL;
}

/**
 * This method initializes the graph, setting up values
 * before finding the shrotest distance.
 * @param graph pointer to the graph
 * @param src pointer to the source vertex
 */
void initialize_graph(vertex *graph, vertex *src) {
    vertex *cur_p = graph;

    while (cur_p != NULL) {
        cur_p->minDist = INT_MAX;
        cur_p->marked = 0;
        cur_p = cur_p->next_p;
    }

    src->minDist = 0;
}

/**
 * This method finds the shortest from the source vertex to
 * other vertices.
 * @param graph pointer to the graph
 * @param src pointer to the source vertex
 */
void find_shortest_paths(vertex *graph, vertex *src) {
    vertex *u = find_min_distance_vertex(graph);

    while (u != NULL) {
        u->marked = 1;
        edge *cur_edge = u->edge_p;

        while (cur_edge != NULL) {
            int n = u->minDist + cur_edge->dist;
            if (n < cur_edge->to_p->minDist) {
                cur_edge->to_p->minDist = n;
            }
            cur_edge = cur_edge->next_p;
        }

        u = find_min_distance_vertex(graph);
    }
}

/**
 * This method finds the vertex with min distance.
 * @param graph pointer to the graph.
 * @return pointer to the vertex whose distance is min. NULL if all vertices are marked.
 */
vertex *find_min_distance_vertex(vertex *graph) {
    vertex *cur_p = graph;
    int min = INT_MAX;
    vertex *min_vertex = NULL;

    while (cur_p != NULL) {
        if (cur_p->marked == 0 && cur_p->minDist < min) {
            min_vertex = cur_p;
            min = cur_p->minDist;
        }

        cur_p = cur_p->next_p;
    }

    return min_vertex;
}

/**
 * This method prints error messages
 * @param error_message error message to be printed
 */
void print_error(char error_message[]) {
    fprintf(stderr, "%s\n", error_message);
    exitCode = 1;
}

/**
 * This method checks the allocated memory and ends
 * programs if necessary.
 * @param pointer generic pointer
 */
void memory_check(void *pointer) {
    if (pointer == NULL) {
        print_error("Memory is full.");
        exit(1);
    }
}

/**
 * This method finds checks if the edge between two vertices
 * exists.
 * @param graph pointer to the graph
 * @param from_p from vertex pointer
 * @param to_p to vertex pointer
 * @return edge if exists. NULL otherwise.
 *
 *
 */
edge *find_edge(vertex *graph, vertex *from_p, vertex *to_p) {
    vertex *cur_p = graph;

    while (cur_p != NULL) {
        if (cur_p == from_p) {
            edge *cur_edge_p = cur_p->edge_p;
            while (cur_edge_p != NULL) {
                if ((cur_edge_p->to_p) == to_p) {
                    return cur_edge_p;
                }
                cur_edge_p = cur_edge_p->next_p;
            }
        }

        cur_p = cur_p->next_p;
    }

    return NULL;
}