#ifndef DSTARLITE_H_
#define DSTARLITE_H_

#include "state.h"
#include "hashmap.h"
#include "list.h"
#include "binheap.h"

//States globais do algoritmo
state goal;
state start;
state last;

void init(hashmap** h2,hashmap** open_h2,bin_heap** open_list2,state_list** path2,int sx,int sy,int gx,int gy);
void update_cell(int a,int b, double val,hashmap* h,hashmap* open_h,bin_heap* open_list);
void update_start(int a,int b,hashmap* h);
void set_rhs(state a,double r,hashmap* h);
state_list* replan(state_list* list,hashmap* h,hashmap* open_h,bin_heap* open_list);

//desenho
void draw_grid(hashmap* h,hashmap* open_h,state_list* path);

#endif
