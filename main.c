#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

#include "point.h"
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
  char buffer[1024];
  
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
     break;
   }
  } 
   
  //Double temporário para leitura
  double temp;
  
  //Criando array de pontos
  vertice** vertices = (vertice**)malloc(sizeof(vertice*)*(n_vertices));
  
  //Lendo os pontos
  for(i=0;i<size;i++){
   point* new_point = (point*)malloc(sizeof(point));
   fscanf(tsp,"%lf", &temp);
   fscanf(tsp,"%lf", &temp);
   // printf("X: %lf\n", temp);
   new_point->x = temp;
   fscanf(tsp,"%lf", &temp);
   // printf("Y: %lf\n", temp);
   new_point->y = temp;
   new_point->id = i;
   points[i] = new_point;
  }
  
  fclose(tsp);
  
   /*
  * !!!
  * FIM DA LEITURA !!!
  * !!!
  */

  return 0;
  
}
