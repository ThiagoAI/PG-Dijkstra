#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <float.h>

//Open GL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "list.h"
#include "binheap.h"
#include "hashmap.h"
#include "state.h"
#include "dstarlite.h"
#include "astar.h"

//Precisam ser globais para openGL
hashmap* h;
hashmap* open_h;
bin_heap* open_list;
state_list* path;

hashmap_a* blocked_a;
hashmap_a* h_a;
heap_a* open_list_a;

//Flags
int grid_flag = -1;
int auto_replan_flag = -1;

//Se 0 roda D* Lite, do contrário A*
int astar_run = 0;

//Largura e altura da janela
int wid;
int hei;

//Goal x e y Star x e y
int gx;
int gy;
int sx;
int sy;

//Escala de x e y (para 800x600, escala 8 e 6 causam 100x100)
int scale_x = 8;
int scale_y = 6;

//Para função do mouse
int mbutton = 0;
int mstate = 0;

//Tempo levado
double ex_time = 0.0;
clock_t s,e;

//Para gravar resultados
FILE* f;
char nome[128];

//FUNÇÕES PARA DESENHAR PAREDES DO TESTE
//desenha uma parede vertical de y1 a y2
void draw_v(int x1,int y1,int y2){
  int i;
  for(i=y1;i<y2+1;i++){
    update_cell(x1,i,-1,h,open_h,open_list);
    block_cell_a(blocked_a,x1,i);
  }
}
//desenha uma parede horizontal de x1 a x2
void draw_h(int x1,int x2,int y1){
  int i;
  for(i=x1;i<x2+1;i++){
    update_cell(i,y1,-1,h,open_h,open_list);
    block_cell_a(blocked_a,i,y1);
  }
}

//Se quiser grid nos escuros também
void draw_for_dark_cells(int size){

  if(astar_run) return;

  glLineWidth(1);
  int i;
  int e;
  double temp = (double)size/2;
  int limit_x = (int)wid/scale_x;
  int limit_y = (int)hei/scale_y;
  for(i=-1;i<limit_x;i++)
    for(e=-1;e<limit_y;e++){
      glBegin(GL_LINE_LOOP);
      glColor3f(1,1,1);
      glVertex2f(i - temp,e - temp);
      glVertex2f(i - temp,e + temp);
      glVertex2f(i + temp,e + temp);
      glVertex2f(i + temp,e - temp);
      glEnd();
    }
}

void InitGL(int Width, int Height){
  wid = Width;
  hei = Height;

  glClearColor(0.0f,0.0f,0.0f,0.0f);
  glClearDepth(1.0);

  glViewport(0,0,Width,Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,Width,0,Height,-1,1);
  glMatrixMode(GL_MODELVIEW);
}

void ReshapeFunc(int Width,int Height){
  wid = Width;
  hei = Height;

  glViewport(0,0,Width,Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,Width,0,Height,-100,100);
  glMatrixMode(GL_MODELVIEW);
}

void DisplayFunc(){

  //Limpa a tela
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  //Auto replan ligado entra aqui
  if(auto_replan_flag == 1){
    if(astar_run){
      printf("lel\n");
      astar(sx,sy,gx,gy,h_a,blocked_a,open_list_a);
    }
    else{
      path = replan(path,h,open_h,open_list);
    }
      glColor3f(0,1,0);
      glRasterPos2f(5, hei - 30);
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'A');
  }

  glLoadIdentity();
  glPushMatrix();

  //Para ficar mais visível
  glScaled(scale_x,scale_y,1);

  if(astar_run){
    //astar(sx,sy,gx,gy,h_a,blocked_a,open_list_a);
    draw_grid_a(h_a,blocked_a,gx,gy);
  }
  else draw_grid(h,open_h,path);

  if(grid_flag == 1 && !astar_run){
  draw_for_dark_cells(1);
  }

  glPopMatrix();
  glutSwapBuffers();
}

void KeyboardFunc(unsigned char key,int x,int y){
  usleep(100);

/* CONTROLES
 * R - causa o replan
 * G - coloca grid nas células escuras
 */

  switch(key){
    case 'R':
    case 'r':
      //Executa o algoritmo selecionado e escreve os resultados
      best_distance = 0;
      ex_nodes_a = 0;
      ex_nodes_d = 0;

      s = clock();
      if(astar_run) astar(sx,sy,gx,gy,h_a,blocked_a,open_list_a);
      else path = replan(path,h,open_h,open_list);
      e = clock();
      ex_time = ((double)(e - s))/ CLOCKS_PER_SEC;

      printf("%s | Tempo levado: %lf | Tempo computando: %lf| Nós expandidos: %d| Melhor caminho: %lf\n",astar_run? "Astar": "Dstar Lite",ex_time,astar_run? ex_time : time_computing,astar_run? ex_nodes_a : ex_nodes_d,best_distance);
      fprintf(f,"%s, %lf, %lf, %d, %lf\n",astar_run? "Astar" : "Dstar Lite",ex_time,astar_run? ex_time : time_computing,astar_run? ex_nodes_a : ex_nodes_d,best_distance);

      break;
    case 'G':
    case 'g':
      grid_flag = -grid_flag;
      break;
    case 'A':
    case 'a':
      auto_replan_flag = -auto_replan_flag;
      break;
    case 'C':
    case 'c':
      astar_run = (astar_run == 0)? 1 : 0;
      s = clock();
      if(astar_run) astar(sx,sy,gx,gy,h_a,blocked_a,open_list_a);
      e = clock();
      ex_time = ((double)(e - s))/ CLOCKS_PER_SEC;
      printf("%s | Tempo levado: %lf | Tempo computando: %lf| Nós expandidos: %d| Melhor caminho: %lf\n",astar_run? "Astar": "Dstar Lite",ex_time,astar_run? ex_time : time_computing,astar_run? ex_nodes_a : ex_nodes_d,best_distance);
      fprintf(f,"%s, %lf, %lf, %d, %lf\n",astar_run? "Astar" : "Dstar Lite",ex_time,astar_run? ex_time : time_computing,astar_run? ex_nodes_a : ex_nodes_d,best_distance);
      ex_nodes_a = 0;
      ex_nodes_d = 0;
      break;
    case 'Q':
    case 'q':
      fclose(f);
      exit(0);
      break;
    case '1':

      draw_v(10,-10,55);
      draw_v(15,55,110);
      draw_v(20,-10,55);
      draw_v(25,55,110);
      draw_v(35,55,110);
      draw_v(40,-10,55);

      //draw_v(150,-100,103);
      //draw_v(150,105,300);

      /*draw_v(47,23,27);
      draw_v(43,24,25);
      draw_v(41,25,26);
      draw_h(35,46,23);
      draw_h(35,46,27);*/

      //draw_h(45,51,94);
      //update_cell(51,95,-1,h,open_h,open_list);
      //block_cell_a(blocked_a,51,95);
      //draw_h(40,65,4);
      //draw_h(40,65,20);
      //draw_v(40,5,19);
      //draw_v(55,5,15);
      //draw_v(65,7,19);
      //draw_v(40,5,95);
      //draw_v(60,5,95);

      //Teste 4
      /*draw_h(-10,4,10);
      draw_h(6,20,10);
      draw_v(20,-10,9);

      draw_h(-10,15,20);
      draw_h(17,30,20);
      draw_v(30,-10,3);
      draw_v(30, 5,19);*/
      break;
    case '2':
      x = 38;
      y = 25;

        draw_v(30,-10,55);

      //draw_h(151,155,110);

      /*update_cell(x,y,-1,h,open_h,open_list);
      block_cell_a(blocked_a,x,y);*/

      //draw_h(47,53,93);
      //draw_h(45,59,40);
      //draw_h(41,55,30);
      //1
      //for(y = 51 ;y<54;y++){
      //}
      //y = 94;
      //for(x = 97;x>94;x--){
        //update_cell(x,y,-1,h,open_h,open_list);
        //block_cell_a(blocked_a,x,y);
      //}
      break;
    case '3':
      x = 50;
      y = 55;
      draw_v(36,24,25);
      //1
      /*for(y = 52;y<56;y++){
        update_cell(x,y,-1,h,open_h,open_list);
        block_cell_a(blocked_a,x,y);
      }*/
      break;
    case '4':
      draw_h(43,48,70);
      break;

  }

}

void MouseFunc(int button,int state,int x,int y){
  y = hei - y + (scale_y/2);
  x += scale_x/2;

  y /= scale_y;
  x /= scale_x;

  mbutton = button;

  if((mstate = state) == GLUT_DOWN){
    if(button == GLUT_LEFT_BUTTON){
      printf("Cell %d %d bloqueada.\n",x,y);
      update_cell(x,y,-1,h,open_h,open_list);
      block_cell_a(blocked_a,x,y);
    }
    if(button == GLUT_RIGHT_BUTTON){
      update_start(x,y,h);
    }
  }
}

//Para o movimento do mouse
void MotionFunc(int x,int y){
  y = hei - y + (scale_y/2);
  x += scale_x/2;

  y /= scale_y;
  x /= scale_x;

  if(mstate == GLUT_DOWN){
    if(mbutton == GLUT_LEFT_BUTTON){
      //printf("Cell %d %d bloqueada.\n",x,y);
      update_cell(x,y,-1,h,open_h,open_list);
      block_cell_a(blocked_a,x,y);
    }
  }
}

//Argumento 1 = Arquivo
//Argumento 2 = Binário (1) ou fibonacci (2)
int main (int argc, char** argv){

  scale_x = 16;
  scale_y = 12;
  sx = 5;
  sy = 5;
  gx = 45;
  gy = 45;
  strcpy(nome,"default");

  if(argc > 2){
    scale_x = atoi(argv[1]);
    scale_y = atoi(argv[2]);
  }
  if(argc > 6 ){
    sx = atoi(argv[3]);
    sy = atoi(argv[4]);
    gx = atoi(argv[5]);
    gy = atoi(argv[6]);
  }
  if(argc > 7){
    strcpy(nome,argv[7]);
  }

  //Abrindo Arquivo
  char temp_nome[128];
  strcpy(temp_nome,nome);
  strcat(temp_nome,"_resultados.txt");
  f = fopen(temp_nome,"a");

  //Iniciando glut
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800,600);
  glutInitWindowPosition(100,100);
  glutCreateWindow("dstarlite");

  //Display
  glutDisplayFunc(&DisplayFunc);
  glutReshapeFunc(&ReshapeFunc);
  glutIdleFunc(&DisplayFunc);
  //Mouse
  glutMouseFunc(&MouseFunc);
  glutMotionFunc(&MotionFunc);
  //Keyboard
  glutKeyboardFunc(&KeyboardFunc);

  //Inicializando estruturas de dados do D* Lite
  h = create_hashmap(32768);
  open_h = create_hashmap(32768);
  open_list = create_heap_bin(32768);
  path = NULL;

  //Inicializando estruturas do A*
  blocked_a = create_hashmap_a(32768);
  h_a = create_hashmap_a(32768);
  open_list_a = create_heap_bin_a(32768);

  //Inicializamos o algoritmo D* Lite
  init(&h,&open_h,&open_list,&path,sx,sy,gx,gy);

  /*int test1 = 45;
  int test2 = 0;
  for(test2 = 4;test2 < 96;test2++){
    update_cell(test1,test2,-1,h,open_h,open_list);
    block_cell_a(blocked_a,test1,test2);
    //test2++;
  }
  test1 = 55;
  for(test2 = 4;test2 < 96;test2++){
    update_cell(test1,test2,-1,h,open_h,open_list);
    block_cell_a(blocked_a,test1,test2);
    //test2++;
  }*/

  /*draw_h(12,88,2);
  draw_h(12,88,8);
  draw_h(47,53,93);

  best_distance = 0;
  ex_nodes_a = 0;
  ex_nodes_d = 0;
  astar_run = 0;
  s = clock();
  if(astar_run) astar(sx,sy,gx,gy,h_a,blocked_a,open_list_a);
  else path = replan(path,h,open_h,open_list);
  e = clock();
  ex_time = ((double)(e - s))/ CLOCKS_PER_SEC;

  printf("%s | Tempo levado: %lf | Tempo computando: %lf| Nós expandidos: %d| Melhor caminho: %lf\n",astar_run? "Astar": "Dstar Lite",ex_time,astar_run? ex_time : time_computing,astar_run? ex_nodes_a : ex_nodes_d,best_distance);
  fprintf(f,"%s, %lf, %lf, %d, %lf\n",astar_run? "Astar" : "Dstar Lite",ex_time,astar_run? ex_time : time_computing,astar_run? ex_nodes_a : ex_nodes_d,best_distance);
  */

  //Replan inicial para já mostrar a linha reta
  path = replan(path,h,open_h,open_list);

  glutMainLoop();

  return 0;

}
