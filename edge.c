#include <stdlib.h>

#include "vertice.h"
#include "list.h"
#include "edge.h"

//Cria um vértice sem arestas, x é o id do vértice.
edge* create_edge(int x,int y,double w){
  edge* new_edge = (edge*)malloc(sizeof(edge));
  new_edge->v1 = x;
  new_edge->v2 = y;
  new_edge->weight = w;
  return new_edge;
}
