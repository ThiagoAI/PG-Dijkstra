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

void set_rhs(state a,double r,hashmap* h);
state_list* replan(state_list* list,hashmap* h,hashmap* open_h,bin_heap* open_list);

#endif
