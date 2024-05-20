#include "bow.h"
#include <GL/glut.h>
#include <math.h>

float spline(float x, float scale, float horizontalOffset,
             float verticalOffset);

const float PI = 3.14159265358979323846;

Bow::Bow(float centerX, float centerY, float initialAngle) {
  cx = centerX;
  cy = centerY;
  int startX = centerX - 150;
  for (int x = 0; x < 150; x += 1) {
    points[x][0] = x + startX;
    points[x][1] = spline(points[x][0], 100.0f, startX, cy);
    glVertex2f(points[x][0], points[x][1]);
  }
  for (int x = 0; x < 150; x += 1) {
    points[299 - x][0] = 2 * (startX + 150) - points[x][0];
    points[299 - x][1] = points[x][1];
    glVertex2f(points[299 - x][0], points[299 - x][1]);
  }

  this->angle = (90.0f * PI) / 180.0f;
  rotateRelative(initialAngle);
}

void Bow::draw() {
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < 300; i++) {
    glVertex2f(points[i][0], points[i][1]);
  }
  glEnd();
}

void Bow::rotateRelative(float angle) {
  this->angle += angle;
  for (int i = 0; i < 300; i++) {
    float x = points[i][0] - cx;
    float y = points[i][1] - cy;

    points[i][0] = x * cos(angle) - y * sin(angle) + cx;
    points[i][1] = x * sin(angle) + y * cos(angle) + cy;
  }
}

float spline(float x, float scale, float horizontalOffset,
             float verticalOffset) {
  return (0.9f * pow((x - horizontalOffset) / scale, 3) -
          1.8f * pow((x - horizontalOffset) / scale, 2) +
          1.0f * (x - horizontalOffset) / scale) *
             scale +
         verticalOffset;
}

float Bow::getAngle() { return this->angle; }

void Bow::rotateAbsolute(float angle) { rotateRelative(angle - this->angle); }
