#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "fibheap.h"

/*
 * IMPLEMENTAÇÃO FOI BASEADA NO LIVRO INTRODUCTION TO ALGORITHMS
 * VÍDEOS EXPLICANDO A IMPLEMENTAÇÃO PODEM SER ENCONTRADOS NO YOUTUBE
 * NO CANAL "Leprofesseur }" 
 */

//Inicializamos a heap
fib_heap* create_heap(){
	fib_heap* new = (fib_heap*)malloc(sizeof(fib_heap));
  new->n = 0;
  new->min = NULL;
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
    node->right = heap->min;
    node->left = heap->min->left;
    heap->min->left = node;
    node->left->right = node;
 
		//Se for menor que o mínimo, vira o novo mínimo
		if(node->key < heap->min->key) heap->min = node;
	}

	//Tamanho da heap aumenta
	heap->n++;
}


void heap_link(fib_node* y,fib_node* x){


}	

//Consolida a heap, é chamada por extract-min
void consolidate(fib_heap* heap){
	//Variáveis temporárias
	fib_node* w;
	fib_node* next;
	fib_node* x;
	fib_node* y;
	fib_node* temp;
	fib_node** A;
	fib_node** root_list;

	int i;
	int d;
	int root_size;
	
	//Calculamos o grau máximo - é o floor do logarítimo de n na base do golden ratio ((1 + raiz de 5)/2)
	int max_degree = floor(log((double)heap->n)/log((double)(1 + sqrt(5))/(double)2));

	//Inicializamos A
	A = (fib_node**)malloc(sizeof(fib_node*)*(max_degree + 2));
	for(i=0;i<max_degree+2;i++) A[i] = NULL;

	w = heap->min;
	root_size = 0;
	next = w;

	//Contamos o tamanho da rootlist
	do{
		root_size++;
		next = next->right;		
	}while(next->id != w->id);

	//Inicializamos a rootlist
	root_list = (fib_node**)malloc(sizeof(fib_node*)*root_size);
	for(i=0;i<root_size;i++){
		root_list[i] = next;
		next = next->right;
	}

	for(i=0;i<root_size;i++){
		w = root_list[i];
		
		x = w;

		d = x->degree;

		while(A[d] != NULL){
			
			y = A[d];

			//Se x for pior, trocamos os dois
			if(x->key > y->key){
				temp = x;
				x = y;
				y = temp;
			}

			//Chamamos heaplink para ligar os nodos
			heap_link(y,x);			
	
			A[d] = NULL;
			d++;
		}//Fim do while de A[d] != NULL

		A[d] = x;	

	}//Fim do for que percorre a rootlist

	//Não vamos mais usar a root_list, então liberamos espaço
	free(root_list);	

	//Chamamos consolidate dentro de extract_min, temos que refazer a rootlist e atualizar o mínimo
	heap->min = NULL;

	for(i=0;i<max_degree + 2;i++){
		
		if(A[i] != NULL){

			//Se for o primeiro entra aqui
			if(heap->min == NULL){
				A[i]->left = A[i];
				A[i]->right = A[i];
				heap->min = A[i];
			}
			else{
		  	
				heap->min->left->right = A[i];
				A[i]->left = heap->min->left;	
				heap->min->left = A[i];
				A[i]->right = heap->min;

				//Se for menor que o mínimo, trocamos
				if(heap->min->key > A[i]->key) heap->min = A[i];
			}
			


		}//Fim do if A[i] != NULL

	}//Fim do for

	//Acabamos o consolidate, falta liberar o A
	free(A);
}

//Extrai o mínimo da heap
fib_node* extract_min(fib_heap* heap){
	//Variaveis contadoras e temporárias
	int i = 0;
	fib_node* temp_min;
  fib_node* temp;
  fib_node* temp_next;
	fib_node** child_list = NULL;

  //Guardamos o mínimo
	temp_min = heap->min;

  //Checamos se a heap está vazia
  if(temp_min == NULL) return NULL;

	//Se tiver filhos, entra no if	
	temp = temp_min->child;
	if(temp != NULL){

		//Inicializamos a lista de crianças do mínimo
		child_list = (fib_node**)malloc(sizeof(fib_node*)*(temp_min->degree));
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
		consolidate(heap);
	}

	heap->n--;
	
  return temp_min; 
}


