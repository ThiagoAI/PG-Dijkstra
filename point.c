#include <math.h>
#include "point.h"

int euclidian_distance(point* a, point* b){
  double dx = a->x - b->x;
  double dy = a->y - b->y;
  
  return round(sqrt((dx*dx) + (dy*dy)));
}
