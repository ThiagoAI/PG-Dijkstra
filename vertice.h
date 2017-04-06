#ifndef VERTICE_H_
#define VERTICE_H_
#include "list.h"

//Vertice
typedef struct vertice_struct{
  int id;
  edge_list* edges;
}vertice;

void add_edge(vertice* v,edge_list* e);
#endif
