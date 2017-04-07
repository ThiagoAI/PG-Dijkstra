#ifndef FIBHEAP_H_
#define FIBHEAP_H_

//Nodo da heap
typedef struct fib_nodestruct{
  //Ponteiros: left e right são para lista duplamente encadeada circular.
  //Child se refere a uma das crianças do nodo(só precisa de um), e parent ao pai.
  struct fib_nodestruct* left;
  struct fib_nodestruct* right;
  struct fib_nodestruct* child;
  struct fib_nodestruct* parent;
  //Key é o valor do algorítimo de dijkstra até aquele ponto (inicializamos com o maior valor possível)
  double key;
  //id é o id do vértice deste nodo
  int id;
  //degree é para a heap, dita algumas coisas
  int degree;
  //mark também é para a heap, é usado como boolean
  int mark;
}fib_node;

//A heap em si
typedef struct fib_heapstruct{
  //Número de nodos
  int n;
  //Mínimo da heap
  fib_node min;

}fib_heap;

//Funções básicas
fib_heap* create_heap();
fib_node* create_node(int id);
void insert(fib_heap* heap,fib_node* node);

#endif
