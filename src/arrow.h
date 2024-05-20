#ifndef ARROW_H
#define ARROW_H

#include "target.h"

const float GRAVITY = 9.8f;

class Arrow {
public:
  Arrow(float bodyWidth, float bodyHeight, float cx, float cy,
        float initialAngle, int windowWidth, int windowHeight,
        float maxPullDistance);
  void draw();
  void transform(float x, float y, float angle);
  void translateRelative(float x, float y);
  void rotateRelative(float angle);
  void moveForward(float distance);
  void reset(float x, float y, float angle);
  void calculateAirbornPosition();
  bool hasCollidedWith(Target target);
  bool hasCollidedWithWindow();
  void setSpeed(float velocity);
  void rotateAbsolute(float angle);
  void setWindow(int windowWidth, int windowHeight);
  bool isAirborn();
  void release(float initialVelocity);
  float *getCentralPoint();
  void pull(float deltaX, float deltaY, float pullingAngle,
            float pullingDistance);
  float *getArrowTail();

private:
  int windowWidth, windowHeight;
  float arrowWidth, arrowLength;
  float vertex[5][2];
  float cx, cy;
  float speed;
  float angle;
  void drawHead();
  void drawBody();
  bool airborn;
  float maxPullDistance;
};

#endif
