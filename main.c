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
  
  //Criando array de vertices
  vertice** vertices = (vertice**)malloc(sizeof(vertice*)*(n_vertices));
  for(i=0;i<n_vertices;i++) vertices[i] = NULL;
 
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
   }

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
     break;
   }
  }  

  //Lemos as arestas
  for(i=0;i<n_edges;i++){
    vertice* new_vertice1;
    vertice* new_vertice2;
    edge* new_edge;

    //Lê o 'a'
    fscanf(gr,"%s", &buffer);

    fscanf(gr,"%d", &temp_i1);
    if(vertices[temp_i1] != NULL){
      new_vertice1 = create_vertice(temp_i1);
      vertices[temp_i1] = new_vertice1;
    }
   
    fscanf(gr,"%d", &temp_i2);
    if(vertices[temp_i2] != NULL){
      new_vertice2 = create_vertice(temp_i2);
      vertices[temp_i2] = new_vertice2;
    }

    fscanf(gr,"%lf", &temp_d);
    // printf("Y: %lf\n", temp);

    new_edge = create_edge(vertices[temp_i1]->id,vertices[temp_i2]->id,temp_d);
    add_edge(vertices[temp_i1],vertices[temp_i2],new_edge);
  }
  
  fclose(gr);
  
   /*
  * !!!
  * FIM DA LEITURA !!!
  * !!!
  */

  return 0;
  
}
