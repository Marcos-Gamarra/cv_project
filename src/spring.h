#ifndef SPRING_H
#define SPRING_H

#include <math.h>

class Spring {
public:
  Spring(float k, float cx, float cy);
  float getArrowInitialSpeed(float mass, float arrowPoint[2]);
  void draw(float initialPoint[2], float middlePoint[2], float finalPoint[2]);

private:
  float cx, cy;
  float k;
};

#endif
