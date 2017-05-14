#include <stdlib.h>
#include <stdio.h>
#include <math.h> //Tem define M_SQRT2 que é a raiz quadrada de 2
#include <float.h>

#include "dstarlite.h"
#include "hashmap.h"
#include "state.h"
#include "binheap.h"
#include "list.h"

//Constante da nossa heuristica.
#define C1 1

//Para imitar boolean
#define TRUE 1
#define FALSE 0

//Variável global do algoritmo
double k_m = 0;
state goal;
state start;
state last;

//Verifica se dois double estão a 10^-5 de distância
int close(double x, double y){
  if(x == DBL_MAX && y == DBL_MAX) return TRUE;
  return fabs(x-y) < 0.00001;
}

//Custo de ir de a para b (8-way)
double cost(state a,state b,hashmap* h){
  int xd = fabs(a.x-b.x);
  int yd = fabs(a.y-b.y);
  double scale = 1;

  if(xd + yd > 1) scale = M_SQRT2;

  hashitem* temp = hashmap_get(h,a);
  if(temp == NULL) return scale*C1;
  return scale*temp->info.cost;
}

//Custo euclidiano
double true_dist(state a,state b){
  double x = a.x - b.x;
  double y = a.y - b.y;
  return sqrt((x*x) + (y*y));
}

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

//Verifica se a cell está ocupada (cost < 0)
int occupied(state a,hashmap* h){
  hashitem* temp = hashmap_get(h,a);
  if(temp == NULL) return FALSE;
  return temp->info.cost < 0;
}

//Heurística do algoritmo
double heuristic(state a,state b){
  return eight_condist(a,b)*C1;
}

//Pega o RHS tratando dos casos especiais
double get_rhs(state a,hashmap* h){
  //Se for o objetivo, é 0 por definição
  if(eq_states(a,goal)) return 0;

  //Se não estiver no hashmap temos de calcular
  hashitem* temp = hashmap_get(h,a);
  if(temp == NULL)
    return heuristic(a,goal);
  //Se passar do if ai sim retornamos o rhs
  return temp->info.rhs;
}

//Pega o g tratando dos casos especiais
double get_g(state a,hashmap* h){
  hashitem* temp = hashmap_get(h,a);
  if(temp == NULL)
    return heuristic(a,goal);
  return temp->info.rhs;
}

//Calcula o valor da tupla de uma chave
state calculate_key(hashmap* h,state a){
  //TEM QUE USAR O GET RHS E GET G
  //cellinfo c = hashmap_get(h,a);
  double v = fmin(get_rhs(a,h),get_g(a,h));

    //TODO CHECAR SE É START MESMO
    a.k[0] = v + heuristic(a,start) + k_m;
    a.k[1] = v;

    return a;
}

//Inicializa nosso algoritmo
void init(hashmap* h,hashmap* open_h,bin_heap* heap,state_list* path,int sx,int sy,int gx,int gy){
  //Limpamos as estruturas de dados
  clear_heap(heap);
  clear_list(path);
  hashmap_clear(h);
  hashmap_clear(open_h);

  //zeramos km
  k_m = 0;

  //Atualizamos valores passados para o state inicial e o objetivo
  start.x = sx;
  start.y = sy;
  goal.x = gx;
  goal.y = gy;

  //Inserimos eles na cellhash
  cellinfo temp = create_info();
  temp.cost = C1;

  hashmap_add(h,goal,temp,-1);
  //temp = create_info();
  double d = heuristic(start,goal);
  temp.g = d;
  temp.rhs = d;
  temp.cost = C1;

  hashmap_add(h,start,temp,-1);
  start = calculate_key(h,start);
  last = start;
}


int replan(state_list* list,hashmap* h){

  //Zeramos o caminho
  clear_list(list);

  //Encontramos o menor caminho para o objetivo e agimos apropriadamente
  int res = compute_shortest_path();

  //Se não há caminho...
  if(res < 0){
    printf("ERRO 1: Nao ha caminho para o objetivo\n");
    return FALSE;
  }

  state_list* n = NULL;
  state cur = start;

  //Se nosso g é infinito, também não há caminho
  if(get_g(start,h) == DBL_MAX){
    printf("ERRO 2: Nao ha caminho para o objetivo\n");
    return FALSE;
  }

  while(neq_states(start,goal)){
    //TODO falta algo no n aqui?
    add_list(list,cur);
    n = get_succ(cur);

    //Se a lista está vazia...
    if(n == NULL){
      printf("ERRO 3: Nao ha caminho para o objetivo\n");
      return FALSE;
    }

    double cmin = DBL_MAX;
    double tmin = 0;
    state smin;
    state* i;

    //Percorremos a lista de sucessores
    for(i = n->s;i != NULL;i = n->next->s){
      double val = cost(cur,*i,h);
      double val2 = true_dist(*i,goal) + true_dist(*i,start);
      val += get_g(*i,h);

      //Se são iguais (considerando nossa margem de erro)
      if(close(val,cmin)){
        if(tmin > val2){
          tmin = val2;
          cmin = val;
          smin = *i;
        }
      }
      else if (val < cmin){
        tmin = val2;
        cmin = val;
        smin = *i;
      }
    }//for

    //TODO VER SE CUR = SMIN É DE FATO CORRETO
    clear_list(n);
    cur = smin;
  }//while neq
  add_list(list,goal);
  return TRUE;
}
