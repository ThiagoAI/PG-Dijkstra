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
typedef struct fib_heapstruct{
  //Número de nodos
  int n;
  //Vetor com os nodos da heap
  bin_node** heap;
}bin_heap;

#endif
