#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include "graph.h"
//#include "vertice.h"
//#include "edge.h"

//Retorno de dijkstra
typedef struct answer_struct{
double* d;
int* a;
}ans;

//Algorítimo de dijkstra, mesma implementação porém um usa heap binária e o outro heap de fibonacci
ans dijkstra_bin(graph g,int source);
ans dijkstra_fib(graph g,int source);

#endif
