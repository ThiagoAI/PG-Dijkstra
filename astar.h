#ifndef ASTAR_H_
#define ASTAR_H_

#include <stdlib.h>

//Posição x e y, mais a chave k identifica as células do nosso mapa
typedef struct cell_struct{
  //Posição
  int x;
  int y;
  //Célula pai (para termos caminho)
  struct cell_struct* parent;
  //f = g + h ;  g é o custo real.
  double f;
  //Heuristica
  double h;
  //Valor real
  double g;
  //Para sabermos se está na openlist ou closedlist
  //A da openlist é o index na open_list
  unsigned int is_open;
  unsigned int is_closed;
}cell;

/*
 * -----------------------------------------------------------------------------
 */


typedef struct hash_a_struct{
  cell* c;
  struct hash_a_struct* next;
}hashitem_a;

//Hashmap do A* (será openlist e closedlist)
typedef struct hash_struct_a{
  hashitem_a** cells;
  int size;
  int count;
}hashmap_a;

/*
 * -----------------------------------------------------------------------------
 */

//A heap de A*
typedef struct bin_heapstruct_a{
  //Número de nodos
  int n;
	//Tamanho máximo
	int size;
  //Vetor com os nodos da heap
  cell** heap;
}heap_a;

//Para contar nós expandidos
extern int ex_nodes_a;

hashmap_a* create_hashmap_a(int size);
heap_a* create_heap_bin_a(int size);
void hashmap_clear_a(hashmap_a** ha,int free_cells);
void clear_heap_a(heap_a* heap);

void block_cell_a(hashmap_a* h,int x,int y);
void astar(int xs,int ys,int xg,int yg,hashmap_a* h_a,hashmap_a* blocked_a,heap_a* open_list);
void print_path(hashmap_a* h,int xg,int yg);

void draw_grid_a(hashmap_a* h_a,hashmap_a*blocked_a,int gx,int gy);

#endif
