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
      if(astar_run) astar(sx,sy,gx,gy,h_a,blocked_a,open_list_a);
      else path = replan(path,h,open_h,open_list);
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
      astar(sx,sy,gx,gy,h_a,blocked_a,open_list_a);
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
      update_cell(x,y,-1,h,open_h,open_list);
      block_cell_a(blocked_a,x,y);
    }
  }
}

//Argumento 1 = Arquivo
//Argumento 2 = Binário (1) ou fibonacci (2)
int main (int argc, char** argv){
  /*printf("lol\n");
  block_cell_a(blocked_a,5,6);
  block_cell_a(blocked_a,5,5);
  block_cell_a(blocked_a,5,6);
  block_cell_a(blocked_a,5,4);
  block_cell_a(blocked_a,5,3);

  block_cell_a(blocked_a,10,11);
  block_cell_a(blocked_a,10,9);
  block_cell_a(blocked_a,9,11);
  block_cell_a(blocked_a,9,10);
  block_cell_a(blocked_a,9,9);
  //block_cell_a(h_a,5,5);
  //block_cell_a(h_a,5,5);

  astar(sx,sy,gx,gy,h_a,blocked_a,open_list_a);

  printf("ACABOU ASTAR\n\n\n\n");
  print_path(h_a,10,10);
  printf("oi\n");
  clear_heap_a(open_list_a);
  hashmap_clear_a(&h_a,1);
  block_cell_a(blocked_a,5,7);
  astar(sx,sy,gx,gy,h_a,blocked_a,open_list_a);
  print_path(h_a,10,10);
  return 0;*/
  //Iniciando glut
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800,600);
  glutInitWindowPosition(100,100);
  glutCreateWindow("dstarlite");

  //Para pegar start e goal se passados
  sx = 5;
  sy = 5;
  gx = 95;
  gy = 95;

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
  //Reservado para passar nome de arquivo com testes...
  if(argc > 7){

  }

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

  //Replan inicial para já mostrar a linha reta
  path = replan(path,h,open_h,open_list);

  glutMainLoop();

  return 0;

}
