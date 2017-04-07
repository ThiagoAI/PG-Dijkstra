#include <stdlib.h>

#include "vertice.h"
#include "list.h"
#include "edge.h"

//Cria um vértice sem arestas, x é o id do vértice.
vertice* create_vertice(int x){
  vertice* new_vertice = (vertice*)malloc(sizeof(vertice));
  new_vertice->id = x;
  new_vertice->edges = NULL;
  return new_vertice;
}

//Adiciona uma aresta em ambos os vértices que compoem a aresta.
void add_edge(vertice* x,vertice* y,edge* e){
  edge_list* new1 = (edge_list*)malloc(sizeof(edge_list));
  edge_list* new2 = (edge_list*)malloc(sizeof(edge_list));

  new1->edge = e;
  new1->next = NULL;

  new2->edge = e;
  new2->next = NULL;

  if(x->edges == NULL) x->edges = new1;
  else{
    new1->next = x->edges;
    x->edges = new1;
  }

  if(y->edges == NULL) x->edges = new2;
  else{
    new2->next = x->edges;
    x->edges = new2;
  }
}
