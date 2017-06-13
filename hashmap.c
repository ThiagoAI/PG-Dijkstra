#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

//Função hash para state
int hash(state a){
  return a.x + a.y*12289;
}

//Para a openHash, é o valor guardado para q possamos saber se houve mudanças na openlist
double key_hash(state a){
  return a.k[0] + a.k[1]*1193;
}

//Cria o hashmap
hashmap* create_hashmap(int size){
  hashmap* h = (hashmap*) malloc (sizeof(hashmap));
  h->bucket = (hashitem**)calloc(size,sizeof(hashitem*));
  int i;
  for(i=0;i<size;i++) h->bucket[i] = NULL;
  h->size = size;
  h->count = 0;
  return h;
}

//adiciona cellinfo b com chave a no hashmap
//se a mesma chave já existe, atualiza o valor ao invés de colocar uma cópia
void hashmap_add(hashmap* h,state a,cellinfo b,double d){
  int i = abs(hash(a) % h->size);
  hashitem* temp = h->bucket[i];

  //Se já existir no mapa atualiza o openHash ao invés de por de novo
  while(temp != NULL){
    if(eq_states(temp->key,a)){
      temp->key = a;
      temp->info = b;
      temp->sum = d;
      return;
    }
    temp = temp->next;
  }

  //Se não tinha no mapa ainda chega aqui
  hashitem* item = (hashitem*)malloc(sizeof(hashitem));
  item->key = a;
  item->info = b;
  item->sum = d;

  temp = h->bucket[i];
  item->next = temp;
  h->bucket[i] = item;
  h->count++;
}

//Pega hashitem da hash dada a chave a
hashitem* hashmap_get(hashmap* h,state a){
  int i = abs(hash(a) % h->size);

  hashitem* item = h->bucket[i];
  if(item == NULL){
    return NULL;
  }

  for(item = h->bucket[i];item != NULL;item = item->next){
    if(eq_states(item->key,a))
      return item;
  }
  //se não achar retorna nulo
  return NULL;
}

//Remove do hashmap o item de chave a e o retorna
void hashmap_remove(hashmap* h,state a){
  int i = abs(hash(a) % h->size);
  hashitem* item = h->bucket[i];
  hashitem* next;

  if(item == NULL) return;

  //Se for o primeiro
  if(eq_states(item->key,a)){
    h->bucket[i] = item->next;
    free(item);
    h->count--;
    return;
  }

  //Caso seja um dos next...
  while(item != NULL){
    next = item->next;

    if(next != NULL && eq_states(a,next->key)){
      item->next = next->next;
      h->count--;
      free(next);
      return;
    }
    item = next;
  }

}

//limpa o hashmap, NÃO DESTRÓI ELE APENAS RETIRA TODOS OS ELEMENTOS
void hashmap_clear(hashmap** ha){
  hashmap* h = *ha;
  int i = 0;
  hashitem* temp;
  hashitem* temp2;

  for(i=0;i<h->size;i++){
    if(h->bucket[i] != NULL){
      temp = h->bucket[i];
      while(temp != NULL){

        temp2 = temp->next;
        free(temp);
        temp = temp2;
        h->count--;
      }//while
      h->bucket[i] = NULL;
    }//if

  }//for

  *ha = h;
}

//Função para teste imprime conteudo do hashmap
void hashmap_print(hashmap* h){
  hashitem* temp;
  int i = 0;
  printf("||| ||| |||\n\n");
  for(i = 0;i < h->size;i++){
    temp = h->bucket[i];
    if(temp != NULL){
      printf("h: %.2lf |State: x %d y %d k0 %.2lf k1 %.2lf|Info: g %.2lf rhs %.2lf cost %.2lf|\n",temp->sum,temp->key.x,temp->key.y,temp->key.k[0],temp->key.k[1],temp->info.g,temp->info.rhs,temp->info.cost);
      while(temp->next != NULL){
        temp = temp->next;
        printf("h: %.2lf |State: x %d y %d k0 %.2lf k1 %.2lf|Info: g %.2lf rhs %.2lf cost %.2lf|\n",temp->sum,temp->key.x,temp->key.y,temp->key.k[0],temp->key.k[1],temp->info.g,temp->info.rhs,temp->info.cost);
        }
    }
  }
  printf("||| ||| |||\n\n");
}
