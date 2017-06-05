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

int hei;
int wid;
int window;

int scale = 8;
int mbutton = 0;
int mstate = 0;

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

void ReSizeGLScene(int Width,int Height){
  wid = Width;
  hei = Height;

  glViewport(0,0,Width,Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,Width,0,Height,-100,100);
  glMatrixMode(GL_MODELVIEW);
}

void DrawGLScene(){
  usleep(100);

  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glPushMatrix();

  glScaled(scale,scale,1);

  draw_grid(h,open_h,path);

  glPopMatrix();
  glutSwapBuffers();
}

void keyPressed(unsigned char key,int x,int y){
  usleep(100);

  switch(key){
    case 'R':
    case 'r':
      path = replan(path,h,open_h,open_list);
      break;
  }

}

void mouseFunc(int button,int state,int x,int y){
  y = hei - y + (scale/2);
  x += scale/2;

  mbutton = button;

  if((mstate = state) == GLUT_DOWN){
    if(button == GLUT_LEFT_BUTTON){
      update_cell(x/scale,y/scale,-1,h,open_h,open_list);
    }
    if(button == GLUT_RIGHT_BUTTON){
      update_start(x/scale,y/scale,h);
    }
  }
}

void mouseMotionFunc(int x,int y){
  y = hei - y + (scale/2);
  x += scale/2;

  y /= scale;
  x /= scale;

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
  glutInitWindowSize(800,800);
  glutInitWindowPosition(100,100);

  window = glutCreateWindow("dstarlite");

  glutDisplayFunc(&DrawGLScene);
  glutIdleFunc(&DrawGLScene);
  glutReshapeFunc(&ReSizeGLScene);
  glutKeyboardFunc(&keyPressed);
  glutMouseFunc(&mouseFunc);
  glutMotionFunc(&mouseMotionFunc);

  //Inicializando estruturas de dados
  h = create_hashmap(1024);
  open_h = create_hashmap(1024);
  open_list = create_heap_bin(1024);
  path = NULL;

  //Inicializamos o algoritmo
  init(&h,&open_h,&open_list,&path,1,1,80,80);
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

  //printf("VAI COMECAR O SEGUNDO REPLAN ! ! !\n\n\n\n\n");
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
