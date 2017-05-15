#ifndef LIST_H_
#define LIST_H_

#include "state.h"

//Nodo da lista de arestas
typedef struct node_list{
  struct node_list* next;
  state* s;
}state_list;

void clear_list(state_list** l);
state_list* add_list(state_list** l,state a);

#endif
