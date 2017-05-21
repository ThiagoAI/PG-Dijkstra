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

//Número máximo de passos (para se estiver cercado de obstáculos não rodar pra sempre)
int max_steps = 10000;

//Foward declarations
double get_rhs(state a,hashmap* h);
double get_g(state a,hashmap* h);

/* INDEX
 * F1 - FUNÇÕES MATEMÁTICAS
 * F2 - FUNÇÕES DE UTILIDADE
 * F3 - FUNÇÕES UPDATE
 * F4 - FUNÇÕES PRINCIPAIS
 * F5 - FUNÇÕES DE DESENHO
 */

/*
 * F1 - FUNÇÕES MATEMÁTICAS (DE DISTÂNCIA E BOOLEANAS SIMPLES)
 */

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
  if(temp == NULL){
    //printf("NULL LUL %lf\n",scale*C1);
    return scale*C1;
  }
  //printf("LUL %d %d| %d %d| %lf\n",a.x,a.y,b.x,b.y,scale*temp->info.cost);
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
  //printf("OCUPADO %d %d | %d\n",a.x,a.y,temp->info.cost < 0);
  return temp->info.cost < 0;
}

//Heurística do algoritmo
double heuristic(state a,state b){
  return eight_condist(a,b)*C1;
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

/*
 * FIM DE FUNÇÕES MATEMÁTICAS
 */

 /*
  * F2 - FUNÇÕES DE UTILIDADE COMO AS "GET"
  */

  //Checa se o state está na hash, se não adiciona
  void make_new_cell(state a,hashmap* h){

    //Se já estiver lá não faz nada
    if(hashmap_get(h,a) != NULL) return;

    cellinfo x;
    double heu = heuristic(a,goal);
    x.rhs = heu;
    x.g = heu;
    x.cost = C1;
    hashmap_add(h,a,x,-1);
  }

  //Muda valor de rhs na cellhash
  void set_rhs(state a,double r,hashmap* h){
    make_new_cell(a,h);
    hashitem* i = hashmap_get(h,a);
    i->info.rhs = r;
  }

  //Muda valor de rhs na cellhash
  void set_g(state a,double r,hashmap* h){
    make_new_cell(a,h);
    hashitem* i = hashmap_get(h,a);
    i->info.g = r;
  }

  //Pega os sucessores de um state (todos os 8 vizinhos no caso 8-way)
  state_list* get_succ(state a,hashmap* h){
    state_list* l = NULL;

    if(occupied(a,h)) return l;

    a.k[0] = -1;
    a.k[1] = -1;

    a.x += 1;
    add_list(&l,a);
    a.y += 1;
    add_list(&l,a);
    a.x -= 1;
    add_list(&l,a);
    a.x -= 1;
    add_list(&l,a);
    a.y -= 1;
    add_list(&l,a);
    a.y -= 1;
    add_list(&l,a);
    a.x += 1;
    add_list(&l,a);
    a.x += 1;
    add_list(&l,a);

    return l;
  }

  //Pega os predecessores de um state (todos os 8 vizinhos MENOS OS OCUPADOS)
  state_list* get_pred(state a,hashmap* h){
    state_list* l = NULL;

    a.k[0] = -1;
    a.k[1] = -1;

    a.x += 1;
    if(!occupied(a,h)) add_list(&l,a);
    a.y += 1;
    if(!occupied(a,h)) add_list(&l,a);
    a.x -= 1;
    if(!occupied(a,h)) add_list(&l,a);
    a.x -= 1;
    if(!occupied(a,h)) add_list(&l,a);
    a.y -= 1;
    if(!occupied(a,h)) add_list(&l,a);
    a.y -= 1;
    if(!occupied(a,h)) add_list(&l,a);
    a.x += 1;
    if(!occupied(a,h)) add_list(&l,a);
    a.x += 1;
    if(!occupied(a,h)) add_list(&l,a);

    return l;
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
  //printf("getting you g nigga\n");
  hashitem* temp = hashmap_get(h,a);
  if(temp == NULL)
    return heuristic(a,goal);
  return temp->info.g;
}

//Verifica se o state passado está na openlist vendo se ele está na openHash
int is_valid(state a,hashmap* open_h){
  hashitem* i = hashmap_get(open_h,a);
  if(i == NULL) return FALSE;
  if(!close(key_hash(a),i->sum)) return FALSE;
  return TRUE;
}

//Insere state passado na openlist e openHash
void insert(state a,hashmap* h,hashmap* open_h,bin_heap* open_list){
  double csum;

  a = calculate_key(h,a);
  csum = key_hash(a);
  cellinfo temp = create_info();
  temp.cost = C1;

  //TODO DEVERIA NAO ESTAR COMENTADO
  //hashitem* cur = hashmap_get(open_h,a);
  //if(cur != NULL && close(csum,cur->key.k[1])) return;

  hashmap_add(open_h,a,temp,csum);
  printf("PUXI\n\n");
  push(open_list,a);
  printf("PUXI IS OVER\n");
}

/*
 * FIM DAS FUNÇÕES DE UTILIDADE
 */

 /*
  * F3 - FUNÇÕES DE UPDATE
  */

  //Função que "move o robo", não força um replan devido ao k_m
  void update_start(int a,int b,hashmap* h){
    start.x = a;
    start.y = b;

    k_m += heuristic(last,start);

    start = calculate_key(h,start);
    last = start;
  }

  //Função do pseudocódigo do algoritmo, atualiza valores do vértice
  void update_vertex(state a,hashmap* h,hashmap* open_h,bin_heap* open_list){
    state_list* s;

    //Enquanto não chegarmos no objetivo...
    if(neq_states(a,goal)){
      printf("update vertex %d %d\n",a.x,a.y);
      s = get_succ(a,h);
      double temp = DBL_MAX;
      double temp2;
      state* i;
      state_list* e;
      //printf("top kek\n\n\n\n");

      printf("seara q vai entrar no for?\n");
      for(e = s;e != NULL;e = e->next){
        i = e->s;

        //printf("top lul\n");
        //printf("%lf %lf//\n",get_g(*i,h),cost(a,*i,h));
        temp2 = get_g(*i,h) + cost(a,*i,h);
        //printf("muh seg faults\n");
        if(temp2 < temp) temp = temp2;
      }

      if(!close(get_rhs(a,h),temp)){
        set_rhs(a,temp,h);
      }
    }

    //printf("%lf %lf|\n",get_g(a,h),get_rhs(a,h));
    if(!close(get_g(a,h),get_rhs(a,h))){
      printf("inserindo update vertex...\n");
      insert(a,h,open_h,open_list);
    }

    clear_list(&s);
  }

  //Atualza valor de uma cell (para fazê-la um obstáculo por exemplo)
  void update_cell(int a,int b, double val,hashmap* h,hashmap* open_h,bin_heap* open_list){
    state u;
    u.x = a;
    u.y = b;
    u.k[0] = 0;
    u.k[1] = 0;

    if (eq_states(start,u) || eq_states(goal,u)) return;

    make_new_cell(u,h);
    hashitem* i = hashmap_get(h,u);
    i->info.cost = val;
    printf("update cell vertex\n\n");
    update_vertex(u,h,open_h,open_list);
    hashitem* lol = hashmap_get(h,u);
    if(lol == NULL) printf("NUUUUlo\n");
    else printf("updatecell: %d %d| %.2lf | %.2lf\n",lol->key.x,lol->key.y,get_g(u,h),get_rhs(u,h));
    //hashitem* lol = hashmap_get(h,u);
    //printf("update cell %lf\n",lol->info.cost);
  }

  /*x
   * FIM DAS FUNÇÕES DE UPDATE
   */

   /*
    * FUNÇÕES PRINCIPAIS
    */
    //Inicializa nosso algoritmo
    void init(hashmap** h2,hashmap** open_h2,bin_heap** open_list2,state_list** path2,int sx,int sy,int gx,int gy){
      hashmap* h = *h2;
      hashmap* open_h = *open_h2;
      state_list* path = *path2;
      bin_heap* open_list = *open_list2;
      //Limpamos as estruturas de dados
      clear_heap(open_list);
      clear_list(&path);
      hashmap_clear(&h);
      hashmap_clear(&open_h);

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

      *h2 = h;
      *open_h2 = open_h;
      *path2 = path;
      *open_list2 = open_list;
    }

//Acha o menor caminho
int compute_shortest_path(hashmap* h,hashmap* open_h,bin_heap* open_list){
  state_list* s = NULL;

  //Se não há nada na openlist, não tem o que mexer
  if(open_list->n == 0) return 1;
  printf("SUVIVAH\n");
  int k = 0;

  //TODO checar se condição do while está realmente correta
  //Enquanto a openlist não estiver vazia E o topo da open_list for menor que start OU start não for mais consistente
  state* xx = peek(open_list);
  printf("|||| %d ||||",lt_states(*xx,start));
  printf("shortest path ------ %d || %.2lf | %.2lf || %.2lf %.2lf\n",open_list->n,start.k[0],start.k[1],peek(open_list)->k[0],peek(open_list)->k[1]);
  while((open_list->n != 0) && (
        (lt_states(*peek(open_list),start = calculate_key(h,start)))
        || (get_rhs(start,h) != get_g(start,h)))
       ){
    printf("Entramos no loop...\n");
    if(k++ > max_steps){
      printf("Limite de passos atingido\n");
      return -1;
    }

    state* u;

    //Testa se start é consistente
    int test = (get_rhs(start,h) != get_g(start,h));
    printf("test deu: %d\n",test);
    //Removemos de forma preguiçosa (vai removendo até ficar vazio ou achar um bom)
    while(TRUE){
      //Se a openlist for vazia retornamos 1
      if(open_list->n == 0) return 1;

      u = pop(open_list);
      if(u == NULL) printf("uh oh\n");
      else printf("we popped: %d %d",u->x,u->y);

      if(u == NULL){
        printf("pop em CSP deu nulo\n");
        exit(0);
      }
      else printf("pop de CSP - %d %d\n",u->x,u->y);
      //printf("Nosso u e %d %d |E valido - %d\n",u.x,u.y,(!is_valid(u,open_h)));
      //Se o elemento na open_list não for valido, executamos o loop de novo
      if(!is_valid(*u,open_h)) continue;
      //Se u não for menor que start e start é consistente, retornamos 2
      if((!lt_states(*u,start)) && (!test)) return 2;
      break;
    }

    hashmap_remove(&open_h,*u);
    state k_old = *u;

    //Se estiver desatualizado...
    if(lt_states(k_old,calculate_key(h,*u))){
      insert(*u,h,open_h,open_list);
    }
    else if(get_g(*u,h) > get_rhs(*u,h)){ //Houve melhora
      set_g(*u,get_rhs(*u,h),h);
      s = get_pred(*u,h);
      state_list* l;
      state* i;

      for(l = s;l != NULL;l = l->next){
        i = l->s;
        printf("shortest path vertex\n\n");
        update_vertex(*i,h,open_h,open_list);
      }
      //TODO Falta um update(u) aqui?
    }
    else{ //Houve piora
      set_g(*u,DBL_MAX,h);
      s = get_pred(*u,h);
      state_list* l;
      state* i;

      for(l = s;l != NULL;l = l->next){
        i = l->s;
            printf("shortest path 2 vertex\n\n");
        update_vertex(*i,h,open_h,open_list);
      }
      printf("shortest path 3 vertex\n\n");
        update_vertex(*u,h,open_h,open_list);
    }

    clear_list(&s);
  } //while

  return 0;
}

//Replaneja o caminho (list)
//É nossa função principal
state_list* replan(state_list* list,hashmap* h,hashmap* open_h,bin_heap* open_list){

  //Zeramos o caminho
  clear_list(&list);

  //Encontramos o menor caminho para o objetivo e agimos apropriadamente
  int res = compute_shortest_path(h,open_h,open_list);

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

  while(neq_states(cur,goal)){
    printf("cur replan: %d %d\n",cur.x,cur.y);
    //TODO falta algo no n aqui?
    add_list(&list,cur);
    n = get_succ(cur,h);

    //Se a lista está vazia...
    if(n == NULL){
      printf("ERRO 3: Nao ha caminho para o objetivo\n");
      return FALSE;
    }
    //printf("t2: %d %d %d %d\n",);
    double cmin = DBL_MAX;
    double tmin = 0;
    state smin;
    state* i;
    state_list* e = n;

    //Percorremos a lista de sucessores
    for(e = n;e != NULL;e = e->next){
      i = e->s;

      printf("info replan: x %d y %d \n",i->x,i->y);
      //if(occupied(*i,h)) continue;
      double val = cost(cur,*i,h);
      double val2 = true_dist(*i,goal) + true_dist(start,*i);
      val += get_g(*i,h);
      printf("info replan part 2: val - %.4lf|val2 - %.4lf|cost - %.2lf g - %.2lf\n",val,val2,cost(cur,*i,h),get_g(*i,h));

      //Se são iguais (considerando nossa margem de erro)
      if(close(val,cmin)){
        printf("Too close to call\n");
        if(tmin > val2){
          printf("val2 is best\n");
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

    clear_list(&n);
    cur = smin;
  }//while neq
  add_list(&list,goal);
  return list;
}
