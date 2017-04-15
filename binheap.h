#ifndef BINHEAP_H_
#define BINHEAP_H_

//Implementação usando array para minimizar o tempo de execução

//Nodo da heap
typedef struct bin_nodestruct{
	int id;
	double key;
	int index;
}bin_node;

//A heap em si
typedef struct bin_heapstruct{
  //Número de nodos
  int n;
  //Vetor com os nodos da heap
  bin_node** heap;
}bin_heap;

bin_heap* create_heap_bin(int size);
bin_node* create_node_bin(int id);
void insert_bin(bin_heap* heap,bin_node* node);
bin_node* extract_min_bin(bin_heap* heap);
void decreace_key_bin(bin_heap* heap,int index,int key);

#endif
