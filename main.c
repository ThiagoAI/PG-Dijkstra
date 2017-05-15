#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <float.h>

#include "list.h"
#include "binheap.h"
#include "hashmap.h"
#include "state.h"
#include "dstarlite.h"

//Argumento 1 = Arquivo
//Argumento 2 = Binário (1) ou fibonacci (2)
int main (int argc, char** argv){

  hashmap* h = create_hashmap(256);

  double k[2] = {100,100};
  state a = create_state(1,1,k);
  cellinfo b = create_info();
  b.rhs = 10;
  hashmap_add(h,a,b,-1);

  double l[2] = {130,130};
  state c = create_state(1,2,l);
  cellinfo d = create_info();
  d.rhs = 11;
  hashmap_add(h,c,d,-1);

  double m[2] = {0,0};
  state e = create_state(1,3,m);
  cellinfo f = create_info();
  f.rhs = 15;
  hashmap_add(h,e,f,-1);

  hashitem* x = hashmap_get(h,a);
  printf("OI %lf\n",x->info.rhs);
  x = hashmap_get(h,c);
  printf("IA %lf\n",x->info.rhs);
  x = hashmap_get(h,e);
  printf("IA %lf\n",x->info.rhs);

  set_rhs(e,400.5,h);

  x = hashmap_get(h,e);
  printf("LOL %lf\n",x->info.rhs);

  hashmap_clear(h);

  x = hashmap_get(h,e);
  if(x == NULL) printf("PIRANHA\n");
  else printf("IA %lf\n",x->info.rhs);

  bin_heap* bin = create_heap_bin(256);
  push(bin,a);
  push(bin,c);
  push(bin,e);

  state test = *pop(bin);
  printf("EI %d | %d\n",test.x,test.y);
  test = *pop(bin);
  printf("EI %d | %d\n",test.x,test.y);
  test = *pop(bin);
  printf("EI %d | %d\n",test.x,test.y);
  push(bin,a);

  clear_heap(bin);
  test = *pop(bin);
  if(&test == NULL) printf("THIS IS HALLOWEEN\n");


  state_list* list = NULL;
  add_list(&list,a);
  add_list(&list,c);
  add_list(&list,e);

  state_list* temp = list;
  state* i;
  for(temp = list;temp != NULL;temp = temp->next){
    i = temp->s;
    printf("L3L %d %d %lf\n",i->x,i->y,i->k[0]);
  }
  clear_list(&list);
  for(temp = list;temp != NULL;temp = temp->next){
    i = temp->s;
    printf("L3L %d %d %lf\n",i->x,i->y,i->k[0]);
  }

  hashmap* h2 = create_hashmap(1024);
  hashmap* open_h = create_hashmap(256);
  bin_heap* open_list = create_heap_bin(1024);
  state_list* path = NULL;

  start.x = 1;
  start.y = 1;

  goal.x = 10;
  goal.y = 10;

  path = replan(path,h2,open_h,open_list);

  state_list* lel = path;
  state* xx;
  for(lel = path;lel != NULL;lel = lel->next){
    xx = lel->s;
    printf("%d - %d |%lf |%lf\n\n",xx->x,xx->y,xx->k[0],xx->k[1]);
  }

  return 0;

  //Verifica se o número de argumentos está correto
  /*if(argc < 2){
   printf("ERRO: NÚMERO DE ARGUMENTOS ESTÁ ERRADO!\n");
   return 1;
  }

	int choice = 0;
	//Se não foi dado opção escolhe binário
	if(argc != 3) choice = 1;
	else choice = atoi(argv[2]);

  //Variável contadora
  int i = 0;
	int u = 0;

  //Variáveis que obteremos do arquivo
  char type[1024];
  int n_vertices = -1;
  int n_edges = -1;*/


 /*
  *
  * COMEÇO DA LEITURA !!!
  *
  */

  //Abrindo arquivo tsp
  /*FILE* gr;
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
  vertice** vertices;*/

  /*
  int read;
  while(fgets(buffer, 1024, tsp)){

    printf("BUFFER: %s\n",buffer);
  }*/

  //printf("Lendo... %s %d\n",argv[1],fscanf(tsp,"%s", buffer));
  //printf("aii %s\n",buffer);

  //Pegamos o nome
  /*while(fscanf(gr,"%s", buffer)){
    if(strcmp(buffer,"p") == 0){
      u=fscanf(gr,"%s", type);
      u=fscanf(gr,"%d", &n_vertices);
      u=fscanf(gr,"%d", &n_edges);
      printf("Vertices and Edges: %d | %d\n", n_vertices, n_edges);

      //Agora que sabemos quantos vertices inicializamos o array de vertices
      //+1 é para contar a possibilidade de começar em 0 ou 1, não fara diferença
      vertices = (vertice**)malloc(sizeof(vertice*)*(n_vertices));
      for(i=0;i<n_vertices;i++) vertices[i] = NULL;

    }

		//Se entra aqui é porque encontramos a primeira aresta, pegamos ela e saimos desse loop.
    if(strcmp(buffer,"a") == 0){
      vertice* new_vertice1;
      vertice* new_vertice2;
      edge* new_edge;

      u=fscanf(gr,"%d", &temp_i1);
			temp_i1--;

      new_vertice1 = create_vertice(temp_i1);
      vertices[temp_i1] = new_vertice1;

      u=fscanf(gr,"%d", &temp_i2);
			temp_i2--;

      new_vertice2 = create_vertice(temp_i2);
      vertices[temp_i2] = new_vertice2;

      u=fscanf(gr,"%lf", &temp_d);

      new_edge = create_edge(vertices[temp_i1]->id,vertices[temp_i2]->id,temp_d);
      add_edge(vertices[temp_i1],vertices[temp_i2],new_edge);

      //Ignoramos a próxima
		  u=fscanf(gr,"%s", buffer);
      u=fscanf(gr,"%d", &temp_i1);
      u=fscanf(gr,"%d", &temp_i2);
      u=fscanf(gr,"%lf", &temp_d);

      break;
    }
  }

  //Lemos as arestas. O /2 é pois cada aresta aparece 2x uma seguida da outra e o -1 é pois já lemos a primeira antes.
  for(i=0;i<((n_edges/2)-1);i++){
    vertice* new_vertice1;
    vertice* new_vertice2;
    edge* new_edge;

    //Lê o 'a'
    u=fscanf(gr,"%s", buffer);

    u=fscanf(gr,"%d", &temp_i1);
		temp_i1--;

    if(vertices[temp_i1] == NULL){
      //printf("null1\n");
      new_vertice1 = create_vertice(temp_i1);
      vertices[temp_i1] = new_vertice1;
    }

    u=fscanf(gr,"%d", &temp_i2);
		temp_i2--;

    if(vertices[temp_i2] == NULL){
      //printf("null2\n");
      new_vertice2 = create_vertice(temp_i2);
      vertices[temp_i2] = new_vertice2;
    }

    u=fscanf(gr,"%lf", &temp_d);

    //printf("gr3: %lf | %d | %d\n\n\n\n",temp_d,vertices[temp_i1]->id,vertices[temp_i2]->id);
    new_edge = create_edge(vertices[temp_i1]->id,vertices[temp_i2]->id,temp_d);

    add_edge(vertices[temp_i1],vertices[temp_i2],new_edge);

    //Ignoramos próxima linha pois será repetição da linha anterior
    u=fscanf(gr,"%s", buffer);
    u=fscanf(gr,"%d", &temp_i1);
    u=fscanf(gr,"%d", &temp_i2);
    u=fscanf(gr,"%lf", &temp_d);
  }

  fclose(gr);

	//Criamos o grafo para mandar para o algorítimo de dijkstra
	graph g;
	g.nv = n_vertices;
	g.ne = n_edges;
	g.vertices = vertices;*/

	/*int* a = (int*)malloc(sizeof(int)*g.nv);
	int* finished = (int*)malloc(sizeof(int)*g.nv);
	double* d = (double*)malloc(sizeof(double)*g.nv);
	bin_node** tracker = (bin_node**)malloc(sizeof(bin_node*)*g.nv);

	bin_heap* heap = create_heap_bin(g.nv);
	for(i=0;i<g.nv;i++){
	a[i] = i;
	finished[i] = 0;

	bin_node* new = create_node_bin(g.vertices[i]->id);
	tracker[i] = new;
	insert_bin(heap,new);

	d[i] = DBL_MAX;
	}*/

	/*printf("%lf\n",d[0]);
	decreace_key_bin(heap,2,10);
	decreace_key_bin(heap,5,15);
	decreace_key_bin(heap,1000,5);
	bin_node* min = extract_min_bin(heap);
	printf("\n%d | %d | %lf",min->index,min->id,min->key);
	min = extract_min_bin(heap);
	printf("\n%d | %d | %lf",min->index,min->id,min->key);
	min = extract_min_bin(heap);
	printf("\n%d | %d | %lf",min->index,min->id,min->key);
	decreace_key_bin(heap,264342,0);
	min = extract_min_bin(heap);
	printf("\n%d | %d | %lf",min->index,min->id,min->key);
	*/

	//print de teste
	//print_vertices(vertices,n_vertices);

 /*
  * !!!
  * FIM DA LEITURA !!!
  * !!!
  */

	//Executamos dijkstra e medimos o tempo levado
	//ans ans;
	/*clock_t t = clock();
	//if(choice == 1) ans = dijkstra_bin(g,0);
	//else ans = dijkstra_fib(g,0);
	t = clock() - t;
	double tempo = ((double)t)/CLOCKS_PER_SEC;

	printf("Dijkstra executado em %lf segundos.\n",tempo);

	//Escrevemos arquivo de resultado
	char res[124];
  strcpy(res,argv[1]);
  strcat(res,"_result.txt");
	FILE* resultado = fopen(res,"a");

  for(i = 0; i < n_vertices; i++){
    //fprintf(resultado,"Distância( %d ): %d\tAntecessor( %d ): %d\n",i, (int)ans.d[i],i,ans.a[i]);
  }
  fclose(resultado);*/

	//TESTE PARA A FIBONACCI HEAP
	/*fib_heap* heap = create_heap();
	fib_node** nodes =(fib_node**)malloc(sizeof(fib_node*)*n_vertices);

	for(i=0;i < n_vertices;i++){
		//printf("oi\n");
		fib_node* new = create_node(vertices[i+1]->id);
		nodes[i] = new;
		//if(i == 1) new->key = 2;
		insert(heap,new);
	}

	fib_node* x = extract_min(heap);
	printf("|%d|\n",x->id);
	decreace_key(heap,nodes[100],5);
	decreace_key(heap,nodes[1],4);
	x = extract_min(heap);
	printf("|%d|\n",x->id);
	x = extract_min(heap);
	printf("|%d|\n",x->id);

	fib_node* temp = heap->min;
	x = temp;*/
	/*do{
	  printf("|%d|\n",x->id);
		if(x->child != NULL) printf("||%d||\n",x->child->id);
		if(x->child != NULL && x->child->right->id != x->child->id)printf("|||%d|||\n",x->child->right->id);
		x = x->right;
	}while(x->id != temp->id);*/


  printf("WE DID IT\n\n\n\n");
  return 0;

}
