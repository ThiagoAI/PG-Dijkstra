#include "vertice.h"
#include "list.h"
#include "edge.h"

void add_edge(vertice* v,edge_list* e){
  e->next = v->edges;
  v->edges = e;
}
