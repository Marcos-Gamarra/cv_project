#ifndef BOW_H
#define BOW_H

class Bow {
public:
  Bow(float centerX, float centerY);
  void draw();
  void rotate(float angle);
  float getAngle();
  float *getFirstPoint() { return points[0]; }
  float *getLastPoint() { return points[299]; }

private:
  float angle;
  float cx, cy;
  float points[300][2];
};

#endif
