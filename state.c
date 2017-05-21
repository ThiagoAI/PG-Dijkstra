#include <stdlib.h>

#include "state.h"

state create_state(int x,int y,double* k){
  state a;
  a.x = x;
  a.y = y;
  a.k[0] = k[0];
  a.k[1] = k[1];
  return a;
}

cellinfo create_info(){
  cellinfo info;
  info.g = 0;
  info.rhs = 0;
  info.cost = 0;
  return info;
}

//Temos de comparar de forma lexcográfica
//Usaremos 0.000001 para "forçar" a comparação lexicográfica (no caso deles serem iguais)

//Verificar se é igual
int eq_states(state a,state b){
  if(a.x == b.x && a.y == b.y) return 1;
  else return 0;
}

//Verifica se é diferente
int neq_states(state a,state b){
  return ((a.x != b.x) || (a.y != b.y));
}

//greater than
int gt_states(state a,state b){
  if(a.k[0]-0.000001 > b.k[0]) return 1;
  else if(a.k[0] < b.k[0]-0.000001) return 0;

  return a.k[1] > b.k[1];
}

//less than or equal
int lte_states(state a,state b){
  if(a.k[0] < b.k[0]) return 1;
  else if(a.k[0] > b.k[0]) return 0;

  return a.k[1] < (b.k[1] + 0.000001);
}

//less than
int lt_states(state a,state b){
  if(a.k[0] + 0.000001 < b.k[0]) return 1;
  else if(a.k[0] - 0.000001 > b.k[0]) return 0;

  return a.k[1] < b.k[1];
}
