#ifndef STATE_H_
#define STATE_H_

//Posição x e y, mais a chave k identifica as células do nosso mapa
typedef struct state_struct{
  int x;
  int y;
  double k[2];
}state;

//Informação de cada célula
typedef struct cellinfo_struct{
  double g;
  double rhs;
  double cost;
}cellinfo;

//Para criar uma cellinfo vazia
cellinfo create_info();

//Temos de comparar de forma lexcográfica
//Usaremos 0.000001 para "forçar" a comparação lexicográfica (no caso deles serem iguais)

//Verificar se é igual
int eq_states(state a,state b);

//greater than
int gt_states(state a,state b);

//less than or equal
int lte_states(state a,state b);

//less than
int lt_states(state a,state b);


#endif
