#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "state.h"

//HASHMAP USANDO STATE COMO CHAVE E CELLINFO COMO VALOR GUARDADO

//itens do hashmap
typedef struct hashitem_struct{
  struct hashitem_struct* next;
  state key;
  cellinfo info;
}hashitem;

//hashmap em si
typedef struct hash_struct{
  hashitem** bucket;
  int size;
  int count;
}hashmap;

//Função hash para state
int hash(state a);
hashmap* create_hashmap(int size);
void add_hashmap(hashmap* h,state a,cellinfo b);
hashitem* hashmap_get(hashmap* h,state a);

#endif
