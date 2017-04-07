#include <stdlib.h>
#include <stdio.h>
#include "vertice.h"
#include "list.h"
#include "edge.h"

//Cria um vértice sem arestas, x é o id do vértice.
vertice* create_vertice(int x){
  vertice* new_vertice = (vertice*)malloc(sizeof(vertice));
  new_vertice->id = x;
  new_vertice->edges = NULL;
  return new_vertice;
}

//Adiciona uma aresta em ambos os vértices que compoem a aresta.
void add_edge(vertice* x,vertice* y,edge* e){
  edge_list* new1 = (edge_list*)malloc(sizeof(edge_list));
  edge_list* new2 = (edge_list*)malloc(sizeof(edge_list));

  new1->edge = e;
  new1->next = NULL;

  new2->edge = e;
  new2->next = NULL;

  if(x->edges == NULL) x->edges = new1;
  else{
    new1->next = x->edges;
    x->edges = new1;
  }

  if(y->edges == NULL) y->edges = new2;
  else{
    new2->next = y->edges;
    y->edges = new2;
  }
}

//Função de print para teste
//USE APENAS APÓS LEITURA
void print_vertices(vertice** vertices,int n_vertices){
  if(n_vertices == 0){
    printf("Erro ao tentar imprimir vertices: numero de vertices e 0.\n\n");
    return;
  }

  printf("Comecando print de teste para %d vertices...\n\n",n_vertices);

  int i = 0;
  edge_list* temp;  
  if(vertices[0] != NULL){
    for(i=0;i<n_vertices;i++){
      printf(":%d ",vertices[i]->id);
      temp = vertices[i]->edges;

      while(temp != NULL){
				printf("|%d %d %lf| ", temp->edge->v1, temp->edge->v2,temp->edge->weight); 
        temp = temp->next;
      }
    	
			printf("\n\n");
    }

  }
  else{
		for(i=1;i<n_vertices+1;i++){ 
      printf(":%d ",vertices[i]->id);
      temp = vertices[i]->edges;

      while(temp != NULL){
				printf("|%d %d %lf| ", temp->edge->v1, temp->edge->v2,temp->edge->weight); 
        temp = temp->next;
      }
    	
			printf("\n\n");
    }

  }

}
