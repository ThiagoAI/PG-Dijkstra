#include <stdio.h>
#include <stdlib.h>

#include "list.h"


/*int get_point(int_list* list,int i){
  if(i >= list->amount) return -10;
  
  city* temp = list->cities;
  int k = 0;
  
  for(k = 0; k < i; k++){
  temp = temp->next;  
  }
  
  return temp->point;
}

void print_list(int_list* list){
  printf("Quantidade: %d\n",list->amount);
  city* temp = list->cities;
  while(temp != NULL){
    printf("|%d\n",temp->point);
    temp = temp->next;
  }

}

int_list* destroy_list(int_list* list){
 city* temp = list->cities;
  while(temp != NULL){
    city* temp2 = temp->next;
    free(temp);
    temp = temp2;
  }
  
  free(list);
  return NULL;
}*/

