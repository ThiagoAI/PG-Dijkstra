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

	//Tamanho da heap aumenta
	heap->n++;
}

//Extrai o mínimo da heap
fib_node* extract_min(fib_heap* heap){
	//Variaveis contadoras e temporárias
	int i = 0;
	fib_node* temp_min,temp,temp_next;
	fib_node** child_list = NULL;

  //Guardamos o mínimo
	temp_min = heap->min;

  //Checamos se a heap está vazia
  if(temp_min == NULL) return NULL;

	//Se tiver filhos, entra no if	
	temp = temp_min->child;
	if(temp != NULL){

		//Inicializamos a lista de crianças do mínimo
		child_list = (fib_node**)malloc(sizeof((fib_node*)*temp_min->degree));
		temp_next = temp;
		for(i=0;i<temp_min->degree;i++){
			child_list[i] = temp_next; 
			temp_next = temp_next->right;
		}

		//Colocamos os filhos na root list a direita do min
		for(i=0;i<temp_min->degree;i++){

			temp = child_list[i];
			temp_min->left->right = temp;
			temp->left = temp_min->left;
			temp_min->left = temp;
			temp->right = temp_min;
			temp->parent = NULL;		
		
		}
		//Com sua função cumprida, deletamos a child_list
		free(child_list);

	}//Fim do if onde tratamos as crianças do mínimo se elas existirem	

	//Acertando os vizinhos do mínimo
	temp_min->left->right = temp_min->right;
	temp_min->right->left = temp_min->left;

	//Se for o último elemento entra nesse if
	if( temp_min == temp_min->right){
		heap->min = NULL;		
	}
	else{
		//Se não for o último entra aqui e precisamos chamar consolidate
		heap->min = temp_min->right;
		consolidade(heap);
	}

	heap->n--;
	
  return temp_min; 
}



