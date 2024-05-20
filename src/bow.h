#ifndef BOW_H
#define BOW_H

class Bow {
public:
  Bow(float centerX, float centerY, float initialAngle);
  void draw();
  void rotateRelative(float angle);
  float getAngle();
  void rotateAbsolute(float angle);
  float *getFirstPoint() { return points[0]; }
  float *getLastPoint() { return points[299]; }

private:
  float angle;
  float cx, cy;
  float points[300][2];
};

#endif
