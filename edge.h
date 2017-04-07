#ifndef EDGE_H_
#define EDGE_H_

//Aresta
typedef struct edge_struct{
  int v1;
  int v2;
  double weight;
}edge;

edge* create_edge(int x,int y,double w);
#endif
