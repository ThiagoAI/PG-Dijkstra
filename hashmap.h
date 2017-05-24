#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "state.h"

//HASHMAP USANDO STATE COMO CHAVE E CELLINFO COMO VALOR GUARDADO

//TODO fazer o open_h certo

//itens do hashmap
typedef struct hashitem_struct{
  struct hashitem_struct* next;
  state key;
  cellinfo info;
  //Apenas para open_h
  double sum;
}hashitem;

//hashmap em si
typedef struct hash_struct{
  hashitem** bucket;
  int size;
  int count;
}hashmap;

//Função hash para state (usa x e y)
int hash(state a);
//Função hash para key (usa k)
double key_hash(state a);

hashmap* create_hashmap(int size);
void hashmap_add(hashmap* h,state a,cellinfo b,double d);
hashitem* hashmap_get(hashmap* h,state a);
void hashmap_remove(hashmap* h,state a);
void hashmap_clear(hashmap** h);
void hashmap_print(hashmap* h);

#endif
