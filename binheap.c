#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "binheap.h"
#include "state.h"

#define LCHILD(x) 2*x + 1
#define RCHILD(x) 2*x + 2
#define PARENT(x) (x - 1)/2

/*
 * IMPLEMENTAÇÃO FOI BASEADA NO LIVRO INTRODUCTION TO ALGORITHMS E USA UM ARRAY PARA GUARDAR OS NODOS
 */

//Inicializamos a heap
bin_heap* create_heap_bin(int size){
	bin_heap* bin = (bin_heap*)malloc(sizeof(bin_heap));
  bin->size = size;
	bin->n = 0;
  bin->heap = (state**)calloc(size,sizeof(state*));
  return bin;
}

//Inicialização do nodo
/*bin_node* create_node_bin(state a){
	bin_node bin = (bin_node*)malloc(sizeof(bin_node));
	bin->key = a;
	bin->priority = 0;
  return bin;
}*/

//FUNÇÕES PARA ACHAR OS "RELATIVES" DO NODO
//Note que usamos index-1 para o pai pois os IDs dos testes começam em 1
/*int parent(int index){
	return (index)/2;
}

int left(int index){
	return (2 * index);
}

int right(int index){
	return (2 * index + 1);
}*/

//Pega mas não remove o elemento no topo da heap
state* peek(bin_heap* heap){
		if(heap == NULL || !heap->n ) return NULL;

		state* k = heap->heap[0];
		return k;
}

//Insere na heap
void push(bin_heap* heap,state a){
	//bin_node* bin = create_node_bin(a);

	//Se passar do tamanho máxmo temos que aumentar
	if(heap->n + 1 >= heap->size){
		heap->size = heap->size*2;
		heap->heap = (state**)realloc(heap->heap,heap->size*sizeof(state*));
	}

	//Criamos a cópia
	state* temp = (state*)malloc(sizeof(state));
	temp->x = a.x;
  temp->y = a.y;
  temp->k[0] = a.k[0];
  temp->k[1] = a.k[1];

  //Percorremos do último nível tentando chegar a raiz para achar o lugar o certo
	int i = heap->n;

	while(i && lt_states(*temp,*heap->heap[PARENT(i)])){
		heap->heap[i] = heap->heap[PARENT(i)];
		i = PARENT(i);
	}

	heap->heap[i] = temp;
	heap->n++;
}

//Troca dois states de lugar no array
void swap(state** x,state** y){
	state* temp = *x;
	*x = *y;
	*y = temp;
}

//Compara i com seus filhos, se um deles for menor, troca o menor com i e executa heapify no próximo nível da árvore
void heapify(bin_heap* heap,int i){
	//int largest = 0;
	int smallest = (LCHILD(i) < heap->n && lt_states(*heap->heap[LCHILD(i)],*heap->heap[i])) ? LCHILD(i) : i;

	//if(smallest == LCHILD(i)) largest = i;
	//else largest = LCHILD(i);

	if(RCHILD(i) < heap->n && lt_states(*heap->heap[RCHILD(i)],*heap->heap[smallest])){
		smallest = RCHILD(i);
	}

	if(smallest != i){
		swap(&(heap->heap[i]),&(heap->heap[smallest]));
		heapify(heap,smallest);
	}

}

//Extrai o mínimo da heap
state* pop(bin_heap* heap){

	//Se estiver vazia saimos
	if(!heap->n) return NULL;

	//O mínimo
	state* a = heap->heap[0];
	heap->heap[0] = heap->heap[heap->n - 1];
	heap->heap[heap->n - 1] = NULL;
	heap->n--;

	/*if(a->x == 4 && a->y == 5){
		state* x1 = heap->heap[0];
		state* x2 = heap->heap[1];
		state* x3 = heap->heap[2];
		printf("WE HERE BOIS %d %d %.3lf %.3lf| %d %d %.3lf %.3lf| %d %d %.3lf %.3lf\n",x1->x,x1->y,x1->k[0],x1->k[1],x2->x,x2->y,x2->k[0],x2->k[1],x3->x,x3->y,x3->k[0],x3->k[1]);
		printf("wut wat - %d\n",lt_states(*x2,*x1));
		printf("wut wat - %d\n",lt_states(*x3,*x1));
		heapify(heap,0);
		 x1 = heap->heap[0];
		 x2 = heap->heap[1];
		 x3 = heap->heap[2];
		printf("WE HERE BOIS %d %d %.3lf %.3lf| %d %d %.3lf %.3lf| %d %d %.3lf %.3lf\n",x1->x,x1->y,x1->k[0],x1->k[1],x2->x,x2->y,x2->k[0],x2->k[1],x3->x,x3->y,x3->k[0],x3->k[1]);
		printf("wut wat - %d\n",lt_states(*x2,*x1));
		printf("wut wat - %d\n",lt_states(*x3,*x1));
		exit(0);
	}*/
	//Acerta heap
	heapify(heap,0);

	return a;
}

//Limpa todos os elementos da heap
void clear_heap(bin_heap* heap){
	heap->n = 0;
	int i = 0;
	for(i=0;i<heap->size;i++){
		if(heap->heap[i] != NULL){
			free(heap->heap[i]);
			heap->heap[i] = NULL;
		}
	}
}
