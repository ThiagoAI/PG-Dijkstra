#ifndef FIBHEAP_H_
#define FIBHEAP_H_

//Nodo da heap
typedef struct fib_node{
  struct fib_node* left;
  struct fib_node* right;
  struct fib_node* child;
  struct fib_node* parent;
  int key;
  int id;
  int degree;
  int mark;

}fibnode;

//A heap em si
typedef struct fib_heap{
  int n;
  fibnode min;
 
}fib_heap;

//Funções básicas
fib_heap* create_heap();
//fib_node* 
//void add_list(int_list* list,int x);
//int get_point(int_list* list,int i);
//int_list* destroy_list(int_list* list);

#endif
