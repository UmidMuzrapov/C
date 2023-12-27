//
// Created by Umid Muzrapov
// Class: CS 352
// File: reach.c
//
//Purpose: This program creates a graph from the input stream and 
// and aswers queries if it is possible to reach from one vertex to another.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structs
typedef struct vertex vertex;
typedef struct edge edge;
struct vertex{
    vertex* next_p;
    char* val_p;
    edge* edge_p;
    int marked;
};
struct edge{
    edge* next_p;
    vertex* to_p;
};

//function prototypes
void print_error(char[]);
void process_commands(vertex**, FILE*);
vertex* vertex_exists(vertex*, char*);
int dfs(vertex*, vertex*);
int find(vertex*, char*, char*);
void unmark(vertex*);
void add_node(vertex**, char*);
void add_edge(vertex*, char*, char*);
void free_edge(edge*);
void free_internal(vertex**);
int exitCode;

//This is the entry point of the program.
//param:
//	argv: number of arguments
//	argv[]: a list of command arguments
//
int main(int argv, char *args[]){

    FILE *fp;
    vertex* graph = NULL;

    if (argv>=2){
        fp = fopen(args[1], "r");
        if (fp==NULL){
            print_error("The file could not be opened.");
            exit(exitCode);
        }
    } else{
        fp = stdin;
    }

    if (argv>2){
        print_error("Too many arguments were given.");
    }

    process_commands(&graph, fp);
    fclose(fp);
    free_internal(&graph);
    return exitCode;
}

// This method processed a line of command a time
// param: 
// 	graph: pointer to the reference of thr graph
// 	fp: file pointer/inpur stream
void process_commands(vertex** graph, FILE* fp){
    int input_read;
    char* line = NULL;
    size_t bufsize = 0;
    char* command_type= (char*) malloc(3);;
    char* arg1_p=(char*) malloc(sizeof(char)*65);
    char* arg2_p=(char*) malloc(sizeof(char)*65);
    char* arg3_p=(char*) malloc(sizeof(char)*65);
    int go_on=1;

    while (getline(&line, &bufsize, fp) > 0 && go_on){
        
	// pasrse input once it read with getline
        input_read = sscanf(line, " @%2s %64s %64s %64s ", command_type, arg1_p, arg2_p, arg3_p);

        if (input_read==4 || sizeof(*command_type)>1){
            print_error("Unrecognized command\n");
            continue;
        }

        if (input_read>=2){

            if (strcmp(command_type, "n")==0 && input_read==2){
                add_node(graph, arg1_p);
            } else if (strcmp(command_type, "e") ==0 && input_read==3){
                add_edge(*graph, arg1_p, arg2_p);
            } else if (strcmp(command_type, "q")==0 && input_read==3) {
                int found = find(*graph, arg1_p, arg2_p);
                if(found>=0){
                    printf("%d\n", found);
                }
            } else{
                print_error("Unrecognized command\n");
                continue;
            }
        } else{
		//the end of thestream was reached.
            if (feof(fp)){
                go_on=0;
            }
		// there was an error reading the input stream.
            if (ferror(fp)){
                print_error("There was stream error.");
            }

	    // try to free the memory once reading is done
            free(command_type);
            free(arg1_p);
            free(arg2_p);
            free(arg3_p);
            free(line);
            command_type = NULL;
            arg1_p = NULL;
            arg2_p=NULL;
            arg3_p = NULL;
            command_type=NULL;
        }
    }
}

//
//This method add a new node to the graph
//Before doing so, it check if the vertex already exists or not.
//param:
//	graph: pointer to the graph reference 
//	arg1: name of the vertex to add.
void add_node(vertex** graph, char* arg1){

    if (*graph==NULL){
        *graph = malloc(sizeof (vertex));
        (*graph)->val_p = malloc(sizeof(arg1));
        strcpy( (*graph)->val_p, arg1);
        (*graph)->next_p = NULL;
        (*graph)->edge_p = NULL;
        return;
    }

    vertex* new_vertex_p = malloc(sizeof (vertex));
    vertex* isVertexInGraph = vertex_exists(*graph, arg1);

    if (isVertexInGraph!=NULL){
        print_error("Vertex already exists");
        free(new_vertex_p);
    } else{
        vertex* prev_p = NULL;
        vertex* cur_p = *graph;

        while  (cur_p!=NULL){
            prev_p=cur_p;
            cur_p=cur_p->next_p;
        }

        new_vertex_p->next_p = NULL;
        new_vertex_p->val_p = malloc(sizeof (arg1));
        strcpy(new_vertex_p->val_p, arg1);
        new_vertex_p->edge_p = NULL;
        prev_p->next_p = new_vertex_p;
    }
}

//This method adds an edge to the graph if the edge 
//does note exist already.
//param:
//	graph: pointer to the graph
//	arg1: the name of the from vertex
//	arg2: the name of the to vertex
void add_edge(vertex* graph, char* arg1, char* arg2){
    vertex* from_p = vertex_exists(graph, arg1);
    vertex* to_p = vertex_exists(graph, arg2);
    if (from_p==NULL || to_p==NULL){
        print_error("One of the vertices does not exist.");
        return;
    }

    int findEdge = find(graph, arg1, arg2);

    if (findEdge){
        return;
    }

    edge* new_edge = malloc(sizeof(edge));
    new_edge->next_p=NULL;
    new_edge->to_p=to_p;

	//base case
    if (from_p->edge_p==NULL){
        from_p->edge_p = new_edge;
        return;
    }
	// loop over to find the right place to put the edge.
    edge* prev_edge_p = NULL;
    edge* cur_edge_p= from_p->edge_p;
    while(cur_edge_p!=NULL){
        prev_edge_p = cur_edge_p;
        cur_edge_p = cur_edge_p->next_p;
    }
    prev_edge_p->next_p= new_edge;
}

// This method returns 1 if it is possible to reach from 
// vertex 2 to vertex 1
// param: 
// 	graph: pointer to gthe graph refrence.
// 	arg1: name of vertex 1
// 	arg2: name of vertex 2
//return 1 if reachable. 0 otherwise.
int find(vertex* graph, char* arg1, char* arg2){
    vertex* from_p = vertex_exists(graph, arg1);
    vertex* to_p = vertex_exists(graph, arg2);

    if (from_p==NULL || to_p==NULL){
        print_error("One of the vertices does not exists.");
        return -1;
    }

    unmark(graph);
    return dfs(from_p, to_p);
}

// This function does recursive dfs search.
// param:
// 	from_p: pointer to the node where the path begins.
// 	to_p: pointer to the node where the path should end.
// return 1 if reachable. 0 otherwise.
int dfs(vertex* from_p, vertex* to_p){
    if (from_p==to_p){
        return 1;
    }
	// avoid cycles
    if (from_p->marked) return 0;

    edge* cur_p = from_p->edge_p;
    from_p->marked = 1;
    while (cur_p!=NULL){
        if(dfs(cur_p->to_p, to_p)) return 1;
        cur_p = cur_p->next_p;
    }

    return 0;
}

// This method unmarks all vertices before dfs search
// param:
// 	graph: pointer to the graph.
//
void unmark(vertex* graph) {
    vertex* cur_p = graph;

    while(cur_p!=NULL){
        cur_p->marked = 0;
        cur_p = cur_p->next_p;
    }
}

// This method prints error messages and changes the
// exit status to 1.
//param:
//	erro_message: error message
void print_error(char error_message[]){
    fprintf(stderr, "%s\n", error_message);
    exitCode=1;
}

//This method checks if the vertex with the given value already exists.
//param:
//	graph: pointer to the graph
//	arg1: value of the graph
//return the vertex if found. Null otherwise.
vertex* vertex_exists(vertex* graph, char* arg1){
    vertex* cur_p = graph;

    while(cur_p!=NULL){
        if (strcmp(cur_p->val_p, arg1)==0){
            return cur_p;
        }
        cur_p = cur_p->next_p;
    }

    return NULL;
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

