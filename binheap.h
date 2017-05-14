#ifndef BINHEAP_H_
#define BINHEAP_H_

//Implementação usando array para minimizar o tempo de execução
#include "state.h"
//Nodo da heap
/*typedef struct bin_nodestruct{
	int priority;
	state key;
}bin_node;*/

//A heap em si
typedef struct bin_heapstruct{
  //Número de nodos
  int n;
	//Tamanho máximo
	int size;
  //Vetor com os nodos da heap
  state** heap;
}bin_heap;

bin_heap* create_heap_bin(int size);
void push(bin_heap* heap,state a);
state* pop(bin_heap* heap);
void clear_heap(bin_heap* heap);
/*bin_heap* create_heap_bin(int size);
state create_node_bin(state a);
void insert_bin(bin_heap* heap,state node);
state* extract_min_bin(bin_heap* heap);*/
//void decreace_key_bin(bin_heap* heap,int index,int key);

#endif
