#include "spring.h"
#include <GL/glut.h>
#include <math.h>

Spring::Spring(float k, float cx, float cy) {
  this->k = k;
  this->cx = cx;
  this->cy = cy;
}

float Spring::getArrowInitialSpeed(float mass, float arrowPoint[2]) {
  float pullDistance =
      sqrt(pow(arrowPoint[0] - cx, 2) + pow(arrowPoint[1] - cy, 2));

  return pullDistance * sqrt(k / mass);
}

void Spring::draw(float initialPoint[2], float middlePoint[2],
                  float finalPoint[2]) {
  glBegin(GL_LINES);
  glVertex2f(initialPoint[0], initialPoint[1]);

  if (cx > middlePoint[0]) {
    glVertex2f(middlePoint[0], middlePoint[1]);
    glVertex2f(middlePoint[0], middlePoint[1]);
  }
  glVertex2f(finalPoint[0], finalPoint[1]);
  glEnd();
}
