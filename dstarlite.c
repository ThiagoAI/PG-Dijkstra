#include <stdlib.h>
#include <stdio.h>
#include <math.h> //Tem define M_SQRT2 que é a raiz quadrada de 2

#include "dstarlite.h"
#include "hashmap.h"
#include "state.h"
#include "binheap.h"
#include "list.h"

//Constante da nossa heuristica.
#define C1 1

//Variável global do algoritmo
double k_m = 0;

//Cálculo da distância 8-way
double eight_condist(state a,state b){
  double temp;
  double min = fabs(a.x - b.x);
  double max = fabs(a.y - b.y);
  if(min > max){
    temp = min;
    min = max;
    max = temp;
  }
  return ((M_SQRT2 - 1.0)*min + max);
}

//Heurística do algoritmo
double heuristic(state a,state b){
  return eight_condist(a,b)*C1;
}

//Pega o RHS tratando dos casos especiais
double get_rhs(){

}

//Pega o g tratando dos casos especiais
double get_g(){

}

//Calcula o valor da tupla de uma chave
state calculate_key(hashmap* h,state a,state start){
  //TEM QUE USAR O GET RHS E GET G
  //cellinfo c = hashmap_get(h,a);
  double v = fmin(c.rhs,c.g);

    a.k[0] = v + heuristic(a,start) + k_m;
    a.k[1] = v;

    return a;
}

//Inicializa nosso algoritmo
void init(){

}
