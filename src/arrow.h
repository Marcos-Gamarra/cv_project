#ifndef ARROW_H
#define ARROW_H

#include "target.h"

const float GRAVITY = 9.8f;

class Arrow {
public:
  Arrow(float bodyWidth, float bodyHeight, float x, float y, float initialAngle,
        int windowWidth, int windowHeight);
  void draw();
  void transform(float x, float y, float angle);
  void translate(float x, float y);
  void rotate(float angle);
  void moveForward(float distance);
  void reset(float x, float y, float angle);
  void calculateAirbornPosition();
  bool hasCollidedWith(Target target);
  bool hasCollidedWithWindow();
  void setVelocity(float velocity);
  void setAngle(float angle);
  void setWindow(int windowWidth, int windowHeight);
  bool isAirborn();
  void setAirborn(bool airborn, float initialVelocity);

private:
  int windowWidth, windowHeight;
  float bodyWidth, bodyHeight;
  float vertex[5][2];
  float tx, ty;
  float velocity;
  float angle;
  void drawHead();
  void drawBody();
  bool airborn;
};

#endif
