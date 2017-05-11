#include <stdio.h>
#include <stdlib.h>

#include "list.h"

//Adiciona a lista
state_list* add_list(state_list* l,state a){
  state_list* n = (state_list*)malloc(sizeof(state_list));
  n->s = &a;
  n->next = l;
  return n;
}

//Zera a lista
void clear_list(state_list* l){
  state_list* temp1;
  state_list* temp2;
  temp2 = l;
  for(temp1 = l;temp1 != NULL;temp1 = temp2){
    temp2 = temp1->next;
    free(temp1);
  }
}
