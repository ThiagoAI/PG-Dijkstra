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

//Precisam ser globais para openGL
hashmap* h;
hashmap* open_h;
bin_heap* open_list;
state_list* path;

//Flags
int grid_flag = -1;
int auto_replan_flag = -1;

//Largura e altura da janela
int wid;
int hei;

//Escala de x e y (para 800x600, escala 8 e 6 causam 100x100)
int scale_x = 8;
int scale_y = 6;

//Para função do mouse
int mbutton = 0;
int mstate = 0;

//Se quiser grid nos escuros também
void draw_for_dark_cells(int size){
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
    path = replan(path,h,open_h,open_list);
    glColor3f(0,1,0);
    glRasterPos2f(5, hei - 30);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'A');
  }

  glLoadIdentity();
  glPushMatrix();

  //Para ficar mais visível
  glScaled(scale_x,scale_y,1);

  draw_grid(h,open_h,path);

  if(grid_flag == 1){
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
      path = replan(path,h,open_h,open_list);
      break;
    case 'G':
    case 'g':
      grid_flag = -grid_flag;
      break;
    case 'A':
    case 'a':
      auto_replan_flag = -auto_replan_flag;
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
    }
  }
}

//Argumento 1 = Arquivo
//Argumento 2 = Binário (1) ou fibonacci (2)
int main (int argc, char** argv){

  //Iniciando glut
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800,600);
  glutInitWindowPosition(100,100);
  glutCreateWindow("dstarlite");

  //Para pegar start e goal se passados
  int xs = 5;
  int ys = 5;
  int xg = 95;
  int yg = 95;

  if(argc > 2){
    scale_x = atoi(argv[1]);
    scale_y = atoi(argv[2]);
  }
  if(argc > 6 ){
    xs = atoi(argv[3]);
    ys = atoi(argv[4]);
    xg = atoi(argv[5]);
    yg = atoi(argv[6]);
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

  //Inicializando estruturas de dados
  h = create_hashmap(1024);
  open_h = create_hashmap(1024);
  open_list = create_heap_bin(1024);
  path = NULL;

  //Inicializamos o algoritmo
  init(&h,&open_h,&open_list,&path,xs,ys,xg,yg);
  //printf("|%d|\n",h->count);

  //hashmap_print(h);
  //hashmap_print(open_h);

  //update_cell(2,2,-1,h,open_h,open_list);
  //path = replan(path,h,open_h,open_list);

  //update_cell(3,3,-1,h,open_h,open_list);

  //update_cell(2,1,-1,h,open_h,open_list);
  //update_cell(2,2,-1,h,open_h,open_list);

  //path = replan(path,h,open_h,open_list);

  /*update_cell(1,3,-1,h,open_h,open_list);
  //update_cell(2,1,-1,h,open_h,open_list);
  update_cell(2,3,-1,h,open_h,open_list);
  update_cell(3,3,-1,h,open_h,open_list);
  update_cell(3,2,-1,h,open_h,open_list);
  update_cell(3,1,-1,h,open_h,open_list);
  update_cell(1,1,-1,h,open_h,open_list);
  update_cell(1,2,-1,h,open_h,open_list);

  update_cell(4,6,-1,h,open_h,open_list);
  //update_cell(2,1,-1,h,open_h,open_list);
  update_cell(5,6,-1,h,open_h,open_list);
  update_cell(6,5,-1,h,open_h,open_list);
  update_cell(6,4,-1,h,open_h,open_list);
  update_cell(5,4,-1,h,open_h,open_list);
  update_cell(4,4,-1,h,open_h,open_list);
  update_cell(4,5,-1,h,open_h,open_list);*/

  //Replan inicial para já mostrar a linha reta
  path = replan(path,h,open_h,open_list);

  /*state_list* lel = path;
  state* xx;
  printf("\n\n");
  for(lel = path;lel != NULL;lel = lel->next){
    xx = lel->s;
    printf("%d - %d |%lf |%lf\n\n",xx->x,xx->y,xx->k[0],xx->k[1]);
  }*/

  //InitGL(800,800);

  glutMainLoop();

  return 0;

  //hashmap* h = create_hashmap(256);

  /*double k[2] = {100,100};
  state a = create_state(1,1,k);
  cellinfo b = create_info();
  b.rhs = 10;
  hashmap_add(h,a,b,-1);

  double l[2] = {130,130};
  state c = create_state(1,2,l);
  cellinfo d = create_info();
  d.rhs = 11;
  hashmap_add(h,c,d,-1);

  double m[2] = {0,0};
  state e = create_state(1,3,m);
  cellinfo f = create_info();
  f.rhs = 15;
  hashmap_add(h,e,f,-1);

  hashitem* x = hashmap_get(h,a);
  printf("OI %lf\n",x->info.rhs);
  x = hashmap_get(h,c);
  printf("IA %lf\n",x->info.rhs);
  x = hashmap_get(h,e);
  printf("IA %lf\n",x->info.rhs);

  set_rhs(e,400.5,h);

  x = hashmap_get(h,e);
  printf("LOL %lf\n",x->info.rhs);

  hashmap_clear(h);

  x = hashmap_get(h,e);
  if(x == NULL) printf("PIRANHA\n");
  else printf("IA %lf\n",x->info.rhs);
  */
}
