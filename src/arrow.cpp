#include "arrow.h"
#include "target.h"
#include <GL/freeglut.h>
#include <math.h>

const float PI = 3.14159265358979323846;

Arrow::Arrow(float bodyWidth, float bodyHeight, float x, float y,
             float initialAngle, int windowWidth, int windowHeight) {
  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;
  this->bodyWidth = bodyWidth;
  this->bodyHeight = bodyHeight;
  reset(x, y, initialAngle);
  this->speed = 0.0f;
  this->airborn = false;
}

void Arrow::draw() {
  drawBody();
  drawHead();
}

void Arrow::translate(float x, float y) {
  cx += x;
  cy += y;
  vertex[0][0] += x;
  vertex[0][1] += y;
  vertex[1][0] += x;
  vertex[1][1] += y;
  vertex[2][0] += x;
  vertex[2][1] += y;
  vertex[3][0] += x;
  vertex[3][1] += y;
  vertex[4][0] += x;
  vertex[4][1] += y;
}

void Arrow::rotate(float angle) {
  this->angle += angle;
  for (int i = 0; i < 5; i++) {
    float x = vertex[i][0] - cx;
    float y = vertex[i][1] - cy;
    vertex[i][0] = x * cos(angle) - y * sin(angle) + cx;
    vertex[i][1] = x * sin(angle) + y * cos(angle) + cy;
  }
}

void Arrow::drawHead() {
  glBegin(GL_TRIANGLES);
  glVertex2f(vertex[4][0], vertex[4][1]);
  glVertex2f(vertex[3][0], vertex[3][1]);
  glVertex2f(vertex[2][0], vertex[2][1]);
  glEnd();
}

void Arrow::drawBody() {
  glBegin(GL_QUADS);
  glVertex2f(vertex[0][0], vertex[0][1]);
  glVertex2f(vertex[1][0], vertex[1][1]);
  glVertex2f(vertex[2][0], vertex[2][1]);
  glVertex2f(vertex[3][0], vertex[3][1]);
  glEnd();
}

void Arrow::transform(float x, float y, float angle) {
  translate(x, y);
  rotate(angle);
}

void Arrow::moveForward(float distance) {
  float x = distance * cos(angle);
  float y = distance * sin(angle);

  translate(x, y);
}

bool Arrow::hasCollidedWith(Target target) {
  return target.checkCollision(vertex[2][0], vertex[2][1]);
}

void Arrow::reset(float x, float y, float angle) {
  cx = x;
  cy = y;
  vertex[0][0] = cx - bodyWidth / 2;
  vertex[0][1] = cy;
  vertex[1][0] = cx + bodyWidth / 2;
  vertex[1][1] = cy;
  vertex[2][0] = cx + bodyWidth / 2;
  vertex[2][1] = cy + bodyHeight;
  vertex[3][0] = cx - bodyWidth / 2;
  vertex[3][1] = cy + bodyHeight;
  vertex[4][0] = cx;
  vertex[4][1] = vertex[2][1] + bodyWidth;
  rotate(angle - (90.0f * PI) / 180.0f);
  this->angle = angle;
}

bool Arrow::hasCollidedWithWindow() {
  return (vertex[2][0] < 0 || vertex[2][0] > windowWidth || vertex[2][1] < 0 ||
          vertex[2][1] > windowHeight);
}

void Arrow::calculateAirbornPosition() {
  float vx = speed * cos(angle);
  float vy = speed * sin(angle);
  float g = -9.8f;
  float dt = 0.1f;
  float dx = vx * dt;
  float dy = vy * dt + 0.5 * g * dt * dt;
  translate(dx, dy);
  vy += g * dt;
  float old_angle = angle;
  float new_angle = atan2(vy, vx);
  rotate(new_angle - old_angle);
  speed = sqrt(vx * vx + vy * vy);
}

void Arrow::setSpeed(float speed) { this->speed = speed; }
void Arrow::setAngle(float angle) { this->angle = angle; }
void Arrow::setWindow(int windowWidth, int windowHeight) {
  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;
}

void Arrow::setAirborn(bool airborn, float initialSpeed) {
  this->airborn = airborn;
  if (airborn) {
    speed = initialSpeed;
  }
}

bool Arrow::isAirborn() { return airborn; }

float *Arrow::getCentralPoint() {
  static float centralPoint[2];
  centralPoint[0] = (vertex[0][0] + vertex[1][0]) / 2;
  centralPoint[1] = (vertex[0][1] + vertex[1][1]) / 2;
  return centralPoint;
}

void Arrow::pull(float distance) {
  float deltaX = cos(angle) * distance;
  float deltaY = sin(angle) * distance;
  for (int i = 0; i < 5; i += 1) {
    vertex[i][0] -= deltaX;
    vertex[i][1] -= deltaY;
  }
}
