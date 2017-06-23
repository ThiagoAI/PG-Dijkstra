#include <stdlib.h>
#include <stdio.h>
#include <math.h> //Tem define M_SQRT2 que é a raiz quadrada de 2
#include <float.h>

#include "astar.h"

#define LCHILD(x) 2*x + 1
#define RCHILD(x) 2*x + 2
#define PARENT(x) (x - 1)/2

//Para imitar boolean
#define TRUE 1
#define FALSE 0

//Start e goal
cell* start_a;
cell* goal_a;

//Para criar célula
cell* create_cell_a(int x,int y){
  cell* new_cell = (cell*)malloc(sizeof(cell));
  new_cell->x = x;
  new_cell->y = y;
  new_cell->f = new_cell->h = new_cell->g = new_cell->is_closed = 0;
  new_cell->is_open = -1;
  new_cell->parent = NULL;
  return new_cell;
}

//Função hash para cell
int hash_a(cell* a){
  return a->x + a->y*12289;
}

/*  F1 - HASHMAP
 *  F2 - HEAP
 *  F3 - FUNÇÕES DO A*
 */

 // F1 - HASHMAP

 //Cria o hashmap
 hashmap_a* create_hashmap_a(int size){
   hashmap_a* h = (hashmap_a*) malloc (sizeof(hashmap_a));
   h->cells = (hashitem_a**)calloc(size,sizeof(hashitem_a*));
   int i;
   for(i=0;i<size;i++) h->cells[i] = NULL;
   h->size = size;
   h->count = 0;
   return h;
 }

 //adiciona cell a no hashmap
 //se a mesma chave já existe, atualiza o valor ao invés de colocar uma cópia
 void hashmap_add_a(hashmap_a* h,cell* a){
   int i = abs(hash_a(a) % h->size);
   hashitem_a* temp = h->cells[i];

   //Se já existir no mapa atualiza o openHash ao invés de por de novo
   while(temp != NULL){
     if(temp->c->x == a->x && temp->c->y == a->y){
       temp->c->f = a->f;
       temp->c->h = a->h;
       return;
     }
     temp = temp->next;
   }

   //Se não tinha no mapa ainda chega aqui
   temp = (hashitem_a*)malloc(sizeof(hashitem_a*));
   temp->c = a;
   temp->next = h->cells[i];

   h->cells[i] = temp;
   h->count++;
 }

 //Pega cell da hash dada a chave a
 cell* hashmap_get_a(hashmap_a* h,cell* a){
   int i = abs(hash_a(a) % h->size);

   hashitem_a* item = h->cells[i];
   if(item == NULL){
     return NULL;
   }

   for(item = h->cells[i];item != NULL;item = item->next){
     if(item->c->x == a->x && item->c->y == a->y)
       return item->c;
   }

   //se não achar retorna nulo
   return NULL;
 }

 //Remove do hashmap o item de chave a e o retorna
 void hashmap_remove_a(hashmap_a* h,cell* a){
   int i = abs(hash_a(a) % h->size);
   hashitem_a* item = h->cells[i];
   hashitem_a* next;

   if(item == NULL) return;

   //Se for o primeiro
   if(item->c->x == a->x && item->c->y == a->y){
     h->cells[i] = item->next;
     free(item);
     h->count--;
     return;
   }

   //Caso seja um dos next...
   while(item != NULL){
     next = item->next;

     if(next != NULL && next->c->x == a->x && next->c->y == a->y){
       item->next = next->next;
       h->count--;
       free(next);
       return;
     }
     item = next;
   }

 }

 //limpa o hashmap, NÃO DESTRÓI ELE APENAS RETIRA TODOS OS ELEMENTOS
 void hashmap_clear_a(hashmap_a** ha,int free_cells){
   hashmap_a* h = *ha;
   int i = 0;
   hashitem_a* temp;
   hashitem_a* temp2;

   for(i=0;i<h->size;i++){
     if(h->cells[i] != NULL){
       temp = h->cells[i];
       while(temp != NULL){

         temp2 = temp->next;
         if(free_cells) free(temp->c);
         free(temp);
         temp = temp2;
         h->count--;
       }//while
       h->cells[i] = NULL;
     }//if

   }//for

   *ha = h;
 }

  // FIM HASHMAP

  // F2 - HEAP

  //Inicializamos a heap
  heap_a* create_heap_bin_a(int size){
  	heap_a* bin = (heap_a*)malloc(sizeof(heap_a));
    bin->size = size;
  	bin->n = 0;
    bin->heap = (cell**)calloc(size,sizeof(cell*));
    return bin;
  }

  //Pega mas não remove o elemento no topo da heap
  cell* peek_a(heap_a* heap){
  		if(heap == NULL || !heap->n ) return NULL;

  		cell* k = heap->heap[0];
  		return k;
  }

  //Insere na heap
  void push_a(heap_a* heap,cell* a){
  	//bin_node* bin = create_node_bin(a);

  	//Se passar do tamanho máxmo temos que aumentar
  	if(heap->n + 1 >= heap->size){
  		heap->size = heap->size*2;
  		heap->heap = (cell**)realloc(heap->heap,heap->size*sizeof(cell*));
  	}

    //Percorremos do último nível tentando chegar a raiz para achar o lugar o certo
  	int i = heap->n;

  	while(i && a->f < heap->heap[PARENT(i)]->f){
  		heap->heap[i] = heap->heap[PARENT(i)];
  		i = PARENT(i);
  	}

  	heap->heap[i] = a;
    a->is_open = i;
  	heap->n++;
  }

  //Troca dois cells de lugar no array
  void swap_a(cell** x,cell** y){
    int t = (*x)->is_open;
    (*x)->is_open = (*y)->is_open;
    (*y)->is_open = t;
  	cell* temp = *x;
  	*x = *y;
  	*y = temp;
  }

  //Compara i com seus filhos, se um deles for menor, troca o menor com i e executa heapify no próximo nível da árvore
  void heapify_a(heap_a* heap,int i){
  	//int largest = 0;
  	int smallest = (LCHILD(i) < heap->n && heap->heap[LCHILD(i)]->f < heap->heap[i]->f) ? LCHILD(i) : i;

  	//if(smallest == LCHILD(i)) largest = i;
  	//else largest = LCHILD(i);

  	if(RCHILD(i) < heap->n && heap->heap[RCHILD(i)]->f < heap->heap[smallest]->f){
  		smallest = RCHILD(i);
  	}

  	if(smallest != i){
  		swap_a(&(heap->heap[i]),&(heap->heap[smallest]));
  		heapify_a(heap,smallest);
  	}

  }

  //Para decreace key
  void heapify_up_a(heap_a* heap,int i){

    int smallest = (LCHILD(i) < heap->n && heap->heap[LCHILD(i)]->f < heap->heap[i]->f) ? LCHILD(i) : i;

  	//if(smallest == LCHILD(i)) largest = i;
  	//else largest = LCHILD(i);

  	if(RCHILD(i) < heap->n && heap->heap[RCHILD(i)]->f < heap->heap[smallest]->f){
  		smallest = RCHILD(i);
  	}

  	if(smallest != i){
  		swap_a(&(heap->heap[i]),&(heap->heap[smallest]));
  		heapify_a(heap,PARENT(i));
  	}
  }

  //Diminui key de a
  void decreace_key_a(heap_a* heap,cell* a){
    int i = a->is_open;
    if(i < 0) return;
    cell* temp = heap->heap[i];
    temp->f = a->f;
    temp->h = a->h;
    temp->g = a->g;
    temp->parent = a->parent;

    //Consertar a heap se necessário
    heapify_up_a(heap,i);
  }

  //Extrai o mínimo da heap
  cell* pop_a(heap_a* heap){

  	//Se estiver vazia saimos
  	if(!heap->n) return NULL;

  	//O mínimo
  	cell* a = heap->heap[0];
  	heap->heap[0] = heap->heap[heap->n - 1];
  	heap->heap[heap->n - 1] = NULL;
  	heap->n--;

  	//Acerta heap
  	heapify_a(heap,0);

  	return a;
  }

  //Limpa todos os elementos da heap
  void clear_heap_a(heap_a* heap){
  	heap->n = 0;
  	int i = 0;
  	for(i=0;i<heap->size;i++){
  		if(heap->heap[i] != NULL){
  			//free(heap->heap[i]);
  			heap->heap[i] = NULL;
  		}
  	}
  }

// FIM HEAP

/*
 * F3 - FUNÇÕES A*
 */

//Heuristica do A* (eight way distance)
double heuristic_a(cell* a,cell* b){
  double x = a->x - b->x;
  double y = a->y - b->y;
  return sqrt((x*x) + (y*y));
  /*double temp;
  double min = fabs(a->x - b->x);
  double max = fabs(a->y - b->y);
  if(min > max){
    temp = min;
    min = max;
    max = temp;
  }
  return ((M_SQRT2 - 1.0)*min + max);*/
}

//Custo de ir de a para b (8-way)
double cost_a(cell* a,cell* b){
  int xd = fabs(a->x - b->x);
  int yd = fabs(a->y - b->y);
  double scale = 1;

  if(xd + yd > 1) scale = M_SQRT2;

  return scale;
}

//Bloqueia celula
void block_cell_a(hashmap_a* h,int x,int y){
  cell c;
  c.x = x;
  c.y = y;
  cell* temp = hashmap_get_a(h,&c);
  if(temp == NULL){
    cell* new_cell = create_cell_a(x,y);
    new_cell->is_closed = TRUE;
    hashmap_add_a(h,new_cell);
  }
  else{
    temp->is_closed = TRUE;
  }
}

//Faz update no vizinho. Se ele não está na hash, coloca ele lá. No final, coloca na open_list
void update_sucessor(int x,int y,cell* a,hashmap_a* h_a,heap_a* open_list){
  cell* temp = create_cell_a(x,y);
  cell* temp2;

  //Se não existe criamos
  temp2 = hashmap_get_a(h_a,temp);
  //if(temp2 == NULL) temp2 = temp;
  temp->h = heuristic_a(temp,goal_a);
  temp->g = cost_a(temp,a);
  temp->f = temp->h + temp->g;
  temp->parent = a;
  //printf("%.2lf| %.2lf | %.2lf\n",temp->f,temp->h,temp->g);
  //if(x == 10 && y == 10){
    //printf("ola chegamos no goal | %d %d | %lf\n",a->x,a->y,temp->f);
  //}

  //Se já avaliamos antes...
  if(temp2 != NULL){
    if(x == 5 && y == 5){
      printf("why though | %d %d | %lf\n",a->x,a->y,temp->f);
    }
    //Se estiver closed...
    if(temp2->is_closed == TRUE){
      //cell* temp3 = temp2;
      //temp2 = temp;
      //temp2->is_closed = FALSE;
      //free(temp3);
      //push_a(open_list,temp2);
      free(temp);
      return;
    }

    //Se estiver na openlist
    if(temp2->is_open >= 0 && temp2->f > temp->f){
      decreace_key_a(open_list,temp);
      free(temp);
      return;
    }

    free(temp);
    return;
  }

  //Se nunca avaliamos antes entra aqui
  hashmap_add_a(h_a,temp);
  push_a(open_list,temp);
}

 void astar(int xs,int ys,int xg,int yg,hashmap_a* h_a,heap_a* open_list){
   //Limpamos os hashmaps e closedlist
   //clear_heap_a(open_list);
   //hashmap_clear_a(&h_a,TRUE);
   //hashmap_clear_a(&closed_list,FALSE);
   //TODO vai dar double free aqui...

   start_a = create_cell_a(xs,ys);
   goal_a = create_cell_a(xg,yg);

   start_a->f = start_a->h = heuristic_a(start_a,goal_a);
   start_a->is_open = 0;

   //Colocamos começo e fim nos lugares apropriados (começo na openlist)
   //hashmap_add_a(h_a,goal_a);
   hashmap_add_a(h_a,start_a);
   push_a(open_list,start_a);

   while(peek_a(open_list) != NULL){
     cell* temp = pop_a(open_list);
     temp->is_open = -1;
     temp->is_closed = TRUE;
     printf("%d %d | \n\n",temp->x,temp->y);
     if(temp->x < 0){
       printf("ERRO\n");
       exit(0);
     }
     //Se é goal, acabou.
     if(temp->x == xg && temp->y == yg){
       printf("GOAL FOUND\n\n\n\n");
       return;
     }

     //Se estiver ocupado, ignora
     if(temp->f < 0) continue;

     //Verificamos todos os 8 vizinhos
     int x_temp = temp->x;
     int y_temp = temp->y;

     //Verificamos os vizinhos
     update_sucessor(x_temp - 1,y_temp    ,temp,h_a,open_list);
     update_sucessor(x_temp - 1,y_temp + 1,temp,h_a,open_list);
     update_sucessor(x_temp    ,y_temp + 1,temp,h_a,open_list);
     update_sucessor(x_temp + 1,y_temp + 1,temp,h_a,open_list);
     update_sucessor(x_temp + 1,y_temp    ,temp,h_a,open_list);
     update_sucessor(x_temp + 1,y_temp - 1,temp,h_a,open_list);
     update_sucessor(x_temp    ,y_temp - 1,temp,h_a,open_list);
     update_sucessor(x_temp - 1,y_temp - 1,temp,h_a,open_list);

   }
   //Se chegou aqui não há caminho para o final
 }

//Imprime caminho de start até goal
 void print_path(hashmap_a* h,int xg,int yg){
   cell go;
   go.x = xg;
   go.y = yg;
   cell* temp = hashmap_get_a(h,&go);

   if(temp->parent != NULL)
     print_path(h,temp->parent->x,temp->parent->y);

   printf(" %d| %d | %.2lf\n",temp->x,temp->y,temp->f);
 }
