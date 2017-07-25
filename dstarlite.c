#include <stdlib.h>
#include <stdio.h>
#include <math.h> //Tem define M_SQRT2 que é a raiz quadrada de 2
#include <float.h>

//Open GL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "dstarlite.h"
#include "hashmap.h"
#include "state.h"
#include "binheap.h"
#include "list.h"
#include "time.h"

//Constante da nossa heuristica.
#define C1 1

//Para imitar boolean
#define TRUE 1
#define FALSE 0

//Variável global do algoritmo
double k_m = 0;

//Número máximo de passos (para se estiver cercado de obstáculos não rodar pra sempre)
int max_steps = 100000;

//Inicializa variável global
int ex_nodes_d = 0;
double time_computing = 0.0;
double best_distance = 0.0;

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
    return scale*C1;
  }

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
  ////printf("OCUPADO %d %d | %d\n",a.x,a.y,temp->info.cost < 0);
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
    //printf("New cell %d %d\n",a.x,a.y);
    //Se já estiver lá não faz nada
    if(hashmap_get(h,a) != NULL) return;
    //printf("New indeed\n");
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
    //hashitem* lol = hashmap_get(h,a);
    ////printf("set_g check: %lf | %lf\n",lol->info.g,r);
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
  hashitem* temp = hashmap_get(h,a);
  if(temp == NULL)
    return heuristic(a,goal);
  return temp->info.g;
}

//Verifica se o state passado está na openlist vendo se ele está na openHash
int is_valid(state a,hashmap* open_h){
  //printf("\nChecking %d %d validity.\n",a.x,a.y);
  hashitem* i = hashmap_get(open_h,a);
  if(i == NULL) return FALSE;
  //printf("Survive 1 - %.2lf %.2lf\n",key_hash(a),i->sum);
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
  /*hashitem* cur = hashmap_get(open_h,a);
  if(cur != NULL){
    if(close(csum,cur->sum)) return;
    else{

    }
  }*/

  hashmap_add(open_h,a,temp,csum);

  push(open_list,a);

  //printf("Tamanho da heap e %d\n",open_list->n);
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
    state_list* s = NULL;

    //Enquanto não chegarmos no objetivo...
    if(neq_states(a,goal)){
      ////printf("update vertex %d %d\n",a.x,a.y);
      s = get_succ(a,h);
      double temp = DBL_MAX;
      double temp2;
      state* i;
      state_list* e;


      for(e = s;e != NULL;e = e->next){
        i = e->s;

        temp2 = get_g(*i,h) + cost(a,*i,h);
        if(temp2 < temp) temp = temp2;
      }

      if(!close(get_rhs(a,h),temp)){
        set_rhs(a,temp,h);
      }
    }

    //if(a.x == 1 && a.y == 1) //printf("Update em 1 1: %.2lf | %.2lf\n",get_g(a,h),get_rhs(a,h));
    if(!close(get_g(a,h),get_rhs(a,h))){
      //printf("Insert 1 | %d %d\n",a.x,a.y);
      insert(a,h,open_h,open_list);
    }
    //else{
      ////printf("Nao deu update em 1 1.\n");
    //}


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

    update_vertex(u,h,open_h,open_list);

  }

  /*
   * FIM DAS FUNÇÕES DE UPDATE
   */

   /*
    * F4 - FUNÇÕES PRINCIPAIS
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
  int k = 0;

  //Enquanto a openlist não estiver vazia E o topo da open_list for menor que start OU start não for mais consistente

  ////printf("shortest path ------ %d || %.2lf | %.2lf || %.2lf %.2lf\n",open_list->n,start.k[0],start.k[1],peek(open_list)->k[0],peek(open_list)->k[1]);
  while((open_list->n != 0) && (
        (lt_states(*peek(open_list),start = calculate_key(h,start)))
        || (get_rhs(start,h) != get_g(start,h)))
       ){
    //printf("Start While: %.2lf %.2lf | %d\n",start.k[0],start.k[1],open_list->n);

    if(k++ > max_steps){
      //printf("Limite de passos atingido\n");
      return -1;
    }

    state* u;

    //Testa se start é consistente
    int test = (get_rhs(start,h) != get_g(start,h));

    //Removemos de forma preguiçosa (vai removendo até ficar vazio ou achar um bom)
    while(TRUE){
      //Se a openlist for vazia retornamos 1
      if(open_list->n == 0) return 1;

      u = pop(open_list);

      //Se o elemento na open_list não for valido, executamos o loop de novo
      if(!is_valid(*u,open_h)) continue;

      //Se u não for menor que start e start é consistente, retornamos 2
      if((!lt_states(*u,start)) && (!test)) return 2;
      //printf("lazy remove survive - %d %d %lf %lf\n",u->x,u->y,u->k[0],u->k[1]);
      break;
    }

    //Para contar os nós explorados
    ex_nodes_d++;

    //printf("OpenHash size is: %d\n",open_h->count);
    //printf("Removendo %d %d\n",u->x,u->y);
    hashmap_remove(open_h,*u);
    //printf("OpenHash size after removal is: %d\n",open_h->count);

    state k_old = *u;

    //Se estiver desatualizado...
    if(lt_states(k_old,calculate_key(h,*u))){
      //printf("%d %d desatualizado!!!!!!!!!!.\n\n\n\n\n",u->x,u->y);
      insert(*u,h,open_h,open_list);
    }
    else if(get_g(*u,h) > get_rhs(*u,h)){ //Houve melhora
      //printf("Melhorou\n");
      set_g(*u,get_rhs(*u,h),h);
      s = get_pred(*u,h);
      state_list* l;
      state* i;

      for(l = s;l != NULL;l = l->next){
        i = l->s;
        ////printf("|%d %d|\n",i->x,i->y);
        update_vertex(*i,h,open_h,open_list);
      }

    }
    else{ //Houve piora
      //printf("Piorou\n");
      set_g(*u,DBL_MAX,h);
      s = get_pred(*u,h);
      state_list* l;
      state* i;

      for(l = s;l != NULL;l = l->next){
        i = l->s;
        //printf("||%d %d||\n",i->x,i->y);
        update_vertex(*i,h,open_h,open_list);
        //printf("LEL\n\n");
      }
        update_vertex(*u,h,open_h,open_list);
    }

    free(u);
    clear_list(&s);
    //printf("WHILE END\n\n");
  } //while

  return 0;
}

//Replaneja o caminho (list)
//É nossa função principal
state_list* replan(state_list* list,hashmap* h,hashmap* open_h,bin_heap* open_list){

  clock_t sta,end;
  //Zeramos o caminho
  clear_list(&list);

  sta = clock();
  //Encontramos o menor caminho para o objetivo e agimos apropriadamente
  int res = compute_shortest_path(h,open_h,open_list);
  end = clock();
  time_computing = ((double)(end-sta))/CLOCKS_PER_SEC;
  //printf("shortest: %lf\n",tempo);

  //Se não há caminho...
  if(res < 0){
    //printf("ERRO 1: Nao ha caminho para o objetivo\n");
    return FALSE;
  }

  state_list* n = NULL;
  state cur = start;

  //Se nosso g é infinito, também não há caminho
  if(get_g(start,h) == DBL_MAX){
    //printf("ERRO 2: Nao ha caminho para o objetivo\n");
    return FALSE;
  }

  //sta = clock();
  while(neq_states(cur,goal)){
    ////printf("cur replan: %d %d\n",cur.x,cur.y);
    add_list(&list,cur);
    n = get_succ(cur,h);

    //Se a lista está vazia...
    if(n == NULL){
      return FALSE;
    }

    double cmin = DBL_MAX;
    double tmin = 0;
    double temp_best = 0;
    state smin;
    state* i;
    state_list* e = n;

    //Percorremos a lista de sucessores
    for(e = n;e != NULL;e = e->next){
      i = e->s;

      ////printf("info replan: x %d y %d \n",i->x,i->y);
      //if(occupied(*i,h)) continue;
      double val = cost(cur,*i,h);
      double temp = val;

      double val2 = true_dist(*i,goal) + true_dist(start,*i);
      val += get_g(*i,h);
      ////printf("info replan: %d %d|val2 - %.2lf|cost - %.2lf ",i->x,i->y,val2,cost(cur,*i,h));
      //if(get_g(*i,h) == DBL_MAX) //printf("g MAX\n");
      //else //printf("g %.2lf\n",get_g(*i,h));

      //Se são iguais (considerando nossa margem de erro)
      if(close(val,cmin)){

        if(tmin > val2){
          tmin = val2;
          cmin = val;
          smin = *i;
          temp_best = temp;
        }
      }
      else if (val < cmin){
        tmin = val2;
        cmin = val;
        smin = *i;
        temp_best = temp;
      }
    }//for

    best_distance += temp_best;
    clear_list(&n);
    cur = smin;
  }//while neq
  printf("%d %d %d %d | %lf\n",cur.x,cur.y,goal.x,goal.y,cost(cur,goal,h));

  add_list(&list,goal);
  //end = clock();

  //tempo = ((double)(end-sta))/CLOCKS_PER_SEC;
  //printf("path: %lf | %d\n",tempo,z);

  return list;
}

/*
 * FIM DAS FUNÇÕES PRINCIPAIS
 */

 /*
  * F5 - FUNÇÕES DE DESENHO
  */

//Desenha uma célula (um quadradinho da grid)
//Size é o tamanho dos lados do quadrado
void draw_cell(state a,double size){
  double temp = size/2;
  double x = a.x;
  double y = a.y;
  glBegin(GL_QUADS);
  glVertex2f(x - temp,y - temp);
  glVertex2f(x - temp,y + temp);
  glVertex2f(x + temp,y + temp);
  glVertex2f(x + temp,y - temp);
  glEnd();

  glLineWidth(1);
  glBegin(GL_LINE_LOOP);
  glColor3f(1,1,1);
  glVertex2f(x - temp,y - temp);
  glVertex2f(x - temp,y + temp);
  glVertex2f(x + temp,y + temp);
  glVertex2f(x + temp,y - temp);
  glEnd();
}

//Função principal para desenhar a grid inteira
void draw_grid(hashmap* h,hashmap* open_h,state_list* path){
  state a;
  hashitem* temp;
  int i = 0;

  //Desenhamos as células no hashmap
  for(i=0;i<h->size;i++){
      temp = h->bucket[i];
      while(temp != NULL){
        if(temp->info.cost > 0) glColor3f(0.5,0.5,0.5);
        else if(temp->info.cost < 0) glColor3f(1,1,1);
        else glColor3f(0,1,1);
        draw_cell(temp->key,1);
        temp = temp->next;
      }//Fim do while
    }//Fim do for

    //Começo e Fim
    glBegin(GL_QUADS);
    glColor3f(1,0,0);
    draw_cell(start,1);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0,0,1);
    draw_cell(goal,1);
    glEnd();

    //Desenha a openHash

    for(i=0;i<open_h->size;i++){
        temp = open_h->bucket[i];
        while(temp != NULL){
          glColor3f(0.72,0.42,0.27);
          draw_cell(temp->key,1);
          temp = temp->next;
        }//Fim do while
      }//Fim do for

      //Agora uma linha para o path
      glLineWidth(5);
      glBegin(GL_LINE_STRIP);
      glColor3f(0,0.9,0.3);

      state_list* temp_l;
      for(temp_l = path;temp_l != NULL;temp_l = temp_l->next){
        glVertex3f(temp_l->s->x,temp_l->s->y,0.2);
      }
      glEnd();
  }
