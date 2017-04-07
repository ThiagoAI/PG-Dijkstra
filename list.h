#ifndef LIST_H_
#define LIST_H_
#include "edge.h"

//Nodo da lista de arestas
typedef struct node_list{
  struct node_list* next;
  edge* edge;
}edge_list;

#endif
