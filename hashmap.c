#include <stdlib.h>

#include "hashmap.h"

//Função hash para state
int hash(state a){
  return a.x + a.y*12289;
}

//Cria o hashmap
hashmap* create_hashmap(int size){
  hashmap* h = (hashmap*) malloc (sizeof(hashmap));
  h->bucket = (hashitem**)calloc(size,sizeof(hashitem*));
  h->size = size;
  h->count = 0;
  return h;
}

//adiciona cellinfo b com chave a no hashmap
void add_hashmap(hashmap* h,state a,cellinfo b){
  hashitem* item = (hashitem*)malloc(sizeof(hashitem));
  item->key = a;
  item->info = b;
  int i = hash(a);

  hashitem* temp = h->bucket[i];
  item->next = temp;
  h->bucket[i] = item;
  h->count++;
}

//Pega hashitem da hash dada a chave a
hashitem* hashmap_get(hashmap* h,state a){
  int i = hash(a);
  hashitem* item;

  for(item = h->bucket[i];item != NULL;item = item->next){
    if(eq_states(item->key,a))
      return item;
  }

  //se não achar retorna nulo
  return NULL;
}

//limpa o hashmap, NÃO DESTRÓI ELE APENAS RETIRA TODOS OS ELEMENTOS
void hashmap_clear(hashmap* h){
  int i = 0;
  hashitem* temp;
  hashitem* temp2;

  for(i=0;i<h->size || h->count == 0;i++){

    if(h->bucket[i] != NULL){

      temp = h->bucket[i];
      while(temp != NULL){

        temp2 = temp->next;
        free(temp);
        temp = temp2;
        h->count--;
      }//while

    }//if

  }//for

}