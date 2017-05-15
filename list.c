#include <stdio.h>
#include <stdlib.h>

#include "list.h"

//Adiciona a lista
state_list* add_list(state_list** l,state a){
  state_list* n = (state_list*)malloc(sizeof(state_list));

  //Criamos a cópia
  state* temp = (state*)malloc(sizeof(state));
  temp->x = a.x;
  temp->y = a.y;
  temp->k[0] = a.k[0];
  temp->k[1] = a.k[1];

  n->s = temp;
  n->next = *l;

  *l = n;
  return n;
}

//Zera a lista
void clear_list(state_list** l){
  state_list* temp1;
  state_list* temp2;
  for(temp1 = *l;temp1 != NULL;temp1 = temp2){
    temp2 = temp1->next;
    free(temp1->s);
    free(temp1);
  }
  *l = NULL;
}
