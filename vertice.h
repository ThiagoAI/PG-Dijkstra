#ifndef VERTICE_H_
#define VERTICE_H_
#include "list.h"

//Vertice
typedef struct vertice_struct{
  int id;
  edge_list* edges;
}vertice;

vertice* create_vertice(int x);
void add_edge(vertice* x,vertice* y,edge* e);
void print_vertices(vertice** vertices,int n_vertices);
#endif
