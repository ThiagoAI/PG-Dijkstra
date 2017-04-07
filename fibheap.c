#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "fibheap.h"

//Inicializamos a heap
fib_heap* create_heap(){
	fib_heap* new = (fib_heap*)malloc(sizeof(fib_heap));
  heap->n = 0;
  heap->min = NULL;
  return new;
}

//Inicialização do nodo
fib_node* create_node(int id){
	fib_node* new = (fib_node*)malloc(sizeof(fib_node));
	new->id = id;
  new->key = DBL_MAX;
  new->mark = 0;
  new->degree = 0;
	new->parent = NULL;
  new->child = NULL;
  new->left = NULL;
  new->right = NULL;
  return new;
}

//Insere nodo na heap
void insert(fib_heap* heap,fib_node* node){

  //Se a heap está vazia o primeiro novo é o mínimo
	if(heap->min == NULL){
		node->right = node;
		node->left = node;
		heap->min = node;
  }
  else{
 		//Inserimos o novo a esquerda do mínimo na root list
    node->right = min;
    node->left = min->left;
    heap->min->left = node;
    node->left->right = node;
 
		//Se for menor que o mínimo, vira o novo mínimo
		if(node->key < min->key) heap->min = node;
	}

	heap->n++;
}



