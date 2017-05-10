#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "binheap.h"
#include "state.h"

/*
 * IMPLEMENTAÇÃO FOI BASEADA NO LIVRO INTRODUCTION TO ALGORITHMS E USA UM ARRAY PARA GUARDAR OS NODOS
 */

//Inicializamos a heap
bin_heap* create_heap_bin(int size){
	bin_heap* bin = (bin_heap*)malloc(sizeof(bin_heap));
  bin->size = size;
	bin->n = 0;
  bin->heap = (state*)malloc(sizeof(state)*size);
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

//Insere na heap
void push(bin_heap* heap,state a){
	//bin_node* bin = create_node_bin(a);

	//Se passar do tamanho máxmo temos que aumentar
	if(heap->n + 1 >= heap->size){
		heap->size = heap->size*2;
		heap->heap = (state*)realloc(heap->heap,heap->size*sizeof(state));
	}

//Percorremos do último nível tentando chegar a raiz para achar o lugar o certo
	int i = heap->n + 1;
	int j = i/2;
	while(i > 1 && gt_states(heap->heap[j],a)){
		heap->heap[i] = heap->heap[j];
		i = j;
		j = j/2;
	}
	heap->heap[i] = a;
	heap->n++;
}

state pop(bin_heap* heap){

	//Se estiver vazia saimos
	double key[2] = {0,0};
	state b = create_state(0,0,key);
	if(!heap->n) return b;

	int i = 1;
	int j;
	int k;

	//O mínimo
	state a = heap->heap[1];
	heap->heap[1] = heap->heap[heap->n];
	heap->n--;

	//Acertamos a heap para termos um novo mínimo
	while(1){

		k = i;
		j = 2 * i;

		if( j <= heap->n && lt_states(heap->heap[j],heap->heap[k])){
			k = j;
		}
		if(j + 1 <= heap->n && lt_states(heap->heap[j+1],heap->heap[k])){
			k = j+1;
		}

		if(k == i) break;
	}
	heap->heap[i] = heap->heap[heap->n+1];

	return a;
}
//Função auxiliar de extract_min para manter suas propriedades após extração
/*void min_heapify(bin_heap* heap,int x){
	int l = left(x);
	int r = right(x);
	int smallest;

	//Verifica qual dos 3 é o menor
	if(l <= heap->n && lt_states(heap->heap[l]->key,heap->heap[x]->key)) smallest = l;
	else smallest = x;

	if(r <= heap->n && gt_states(heap->heap[r]->key,heap->heap[smallest]->key)) smallest = r;

	//Se não for o x temos que repetir o processo
	if(smallest != x){
		//Trocamos
		bin_node* temp = heap->heap[x];
		heap->heap[x] = heap->heap[smallest];
		heap->heap[smallest] = temp;

		int temp_int = heap->heap[x]->index;
		heap->heap[x]->index = heap->heap[smallest]->index;
		heap->heap[smallest]->index = temp_int;

		//Repetimos para o novo
		min_heapify(heap,smallest);
	}

}*/

//Extrai o mínimo da heap
/*bin_node* extract_min_bin(bin_heap* heap){
	//Se estiver vazia e for chamada há algum problema com o algorítimo
	if(heap->n == 0){
		printf("ERRO FATAL: Heap binária está vazia!\n\n");
		exit(1);
	}

	//Trocamos o último e o primeiro de lugar
	bin_node* min = heap->heap[0];
	heap->heap[0] = heap->heap[heap->n-1];
	heap->heap[heap->n-1] = min;

	int temp = heap->heap[0]->index;
	heap->heap[0]->index = heap->heap[heap->n-1]->index;
	heap->heap[heap->n-1]->index = temp;

	heap->n--;
	min_heapify(heap,0);
	return min;
}*/

//Diminui valor da chave de um nó especificado
/*void decreace_key_bin(bin_heap* heap,int index,int key){

	//Se a chave for maior, saimos
	if(key > heap->heap[index]->key){
		printf("ERRO FATAL: Chave nova é maior do que a anterior, algorítimo tem erro!\n\n");
		exit(1);
	}

	heap->heap[index]->key = key;
	int p = parent(index);

	//Ajeitamos a heap trocando o pai com o filho até estar certo
	while(index > 0 && heap->heap[p]->key > heap->heap[index]->key){

		bin_node* temp = heap->heap[p];
		heap->heap[p] = heap->heap[index];
		heap->heap[index] = temp;

		int temp_int = heap->heap[p]->index;
		heap->heap[p]->index = heap->heap[index]->index;
		heap->heap[index]->index = temp_int;

		index = p;
		p = parent(index);
	}

}*/
