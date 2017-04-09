#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

#include "edge.h"
#include "vertice.h"
#include "list.h"
#include "fibheap.h"
#include "graph.h"

//Argumento 1 = Arquivo
int main (int argc, char** argv){
  
  //Verifica se o número de argumentos está correto
  if(argc < 2){
   printf("ERRO: NÚMERO DE ARGUMENTOS ESTÁ ERRADO!\n");
   return 1;
  }
  
  //Variável contadora
  int i = 0;
  
  //Variáveis que obteremos do arquivo
  char type[1024];
  int n_vertices = -1;
  int n_edges = -1;
  
  
 /*
  * 
  * COMEÇO DA LEITURA !!!
  * 
  */
 
  //Abrindo arquivo tsp
  FILE* gr;
  gr = fopen(argv[1],"r");
  
  //Se não achar o arquivo ou falhar entra aqui
  if(gr == NULL){
   printf("\nERRO: ARQUIVO NULO, VERIFIQUE SE O CAMINHO ESTÁ CORRETO!\n");
   return 1;
  }

  //Variaveis de leitura
  char buffer[1024];
  int temp_i1;
  int temp_i2;
  double temp_d;
  
  //Declaração do array de vertices que sera inicializado durante leitura
  vertice** vertices;
 
  /*
  int read;
  while(fgets(buffer, 1024, tsp)){
    
    printf("BUFFER: %s\n",buffer);
  }*/

  //printf("Lendo... %s %d\n",argv[1],fscanf(tsp,"%s", buffer));
  //printf("aii %s\n",buffer);
  
  //Pegamos o nome
  while( fscanf(gr,"%s", buffer)){
    if(strcmp(buffer,"p") == 0){
      fscanf(gr,"%s", type);
      fscanf(gr,"%d", &n_vertices);
      fscanf(gr,"%d", &n_edges);
      printf("Vertices and Edges: %d | %d\n", n_vertices, n_edges);

      //Agora que sabemos quantos vertices inicializamos o array de vertices
      //+1 é para contar a possibilidade de começar em 0 ou 1, não fara diferença 
      vertices = (vertice**)malloc(sizeof(vertice*)*(n_vertices+1));
      for(i=0;i<n_vertices;i++) vertices[i] = NULL;

    }

		//Se entra aqui é porque encontramos a primeira aresta, pegamos ela e saimos desse loop.
    if(strcmp(buffer,"a") == 0){
      vertice* new_vertice1;
      vertice* new_vertice2;
      edge* new_edge;

      fscanf(gr,"%d", &temp_i1);

      new_vertice1 = create_vertice(temp_i1);
      vertices[temp_i1] = new_vertice1;
   
      fscanf(gr,"%d", &temp_i2);
     
      new_vertice2 = create_vertice(temp_i2);
      vertices[temp_i2] = new_vertice2;

      fscanf(gr,"%lf", &temp_d);

      new_edge = create_edge(vertices[temp_i1]->id,vertices[temp_i2]->id,temp_d);
      add_edge(vertices[temp_i1],vertices[temp_i2],new_edge);

      //Ignoramos a próxima
		  fscanf(gr,"%s", buffer);
      fscanf(gr,"%d", &temp_i1);
      fscanf(gr,"%d", &temp_i2);
      fscanf(gr,"%lf", &temp_d);

      break;
    }
  }  

  //Lemos as arestas. O /2 é pois cada aresta aparece 2x uma seguida da outra e o -1 é pois já lemos a primeira antes.
  for(i=0;i<((n_edges/2)-1);i++){
    vertice* new_vertice1;
    vertice* new_vertice2;
    edge* new_edge;

    //Lê o 'a'
    fscanf(gr,"%s", buffer);

    fscanf(gr,"%d", &temp_i1);

    if(vertices[temp_i1] == NULL){
      //printf("null1\n");
      new_vertice1 = create_vertice(temp_i1);
      vertices[temp_i1] = new_vertice1;
    }

    fscanf(gr,"%d", &temp_i2);

    if(vertices[temp_i2] == NULL){
      //printf("null2\n");
      new_vertice2 = create_vertice(temp_i2);
      vertices[temp_i2] = new_vertice2;
    }

    fscanf(gr,"%lf", &temp_d);

    //printf("gr3: %lf | %d | %d\n\n\n\n",temp_d,vertices[temp_i1]->id,vertices[temp_i2]->id);
    new_edge = create_edge(vertices[temp_i1]->id,vertices[temp_i2]->id,temp_d);

    add_edge(vertices[temp_i1],vertices[temp_i2],new_edge);

    //Ignoramos próxima linha pois será repetição da linha anterior
    fscanf(gr,"%s", buffer);
    fscanf(gr,"%d", &temp_i1);
    fscanf(gr,"%d", &temp_i2);
    fscanf(gr,"%lf", &temp_d);
  }
  
  fclose(gr);

	print_vertices(vertices,n_vertices);
  
 /*
  * !!!
  * FIM DA LEITURA !!!
  * !!!
  */

	//TESTE PARA A FIBONACCI HEAP
	fib_heap* heap = create_heap();

	/*for(i=0;i < n_vertices;i++){
		//printf("oi\n");
		fib_node* new = create_node(vertices[i+1]->id);
		//if(i == 1) new->key = 2;
		insert(heap,new);
	}

	fib_node* x = extract_min(heap);
	printf("|%d|\n",x->id);
	x = extract_min(heap);

	fib_node* temp = heap->min;
	x = temp;
	do{
	  printf("|%d|\n",x->id);
		if(x->child != NULL) printf("||%d||\n",x->child->id);
		if(x->child != NULL && x->child->right->id != x->child->id)printf("|||%d|||\n",x->child->right->id);
		x = x->right;
	}while(x->id != temp->id);*/
	

  printf("WE DID IT\n\n\n\n");
  return 0;
  
}
