#include "arrow.h"
#include "target.h"
#include <GL/freeglut.h>
#include <cstdio>
#include <math.h>

Arrow::Arrow(float bodyWidth, float bodyHeight, float x, float y,
             int windowWidth, int windowHeight) {
  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;
  reset(bodyWidth, bodyHeight, x, y);
}

void Arrow::draw() {
  drawBody();
  drawHead();
}

void Arrow::translate(float x, float y) {
  tx += x;
  ty += y;
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
    float x = vertex[i][0] - tx;
    float y = vertex[i][1] - ty;
    vertex[i][0] = x * cos(angle) - y * sin(angle) + tx;
    vertex[i][1] = x * sin(angle) + y * cos(angle) + ty;
  }
}

void Arrow::drawHead() {
  glBegin(GL_TRIANGLES);
  glVertex2f(vertex[1][0], vertex[1][1]);
  glVertex2f(vertex[3][0], vertex[3][1]);
  glVertex2f(vertex[2][0], vertex[2][1]);
  glEnd();
}

void Arrow::drawBody() {
  glBegin(GL_QUADS);
  glVertex2f(vertex[0][0], vertex[0][1]);
  glVertex2f(vertex[1][0], vertex[1][1]);
  glVertex2f(vertex[3][0], vertex[3][1]);
  glVertex2f(vertex[4][0], vertex[4][1]);
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

void Arrow::reset(float bodyWidth, float bodyHeight, float x, float y) {
  tx = x;
  ty = y;
  vertex[0][0] = tx - bodyWidth / 2;
  vertex[0][1] = ty - bodyHeight / 2;
  vertex[1][0] = tx + bodyWidth / 2;
  vertex[1][1] = ty - bodyHeight / 2;
  vertex[2][0] = vertex[1][0] + bodyHeight;
  vertex[2][1] = ty;
  vertex[3][0] = tx + bodyWidth / 2;
  vertex[3][1] = ty + bodyHeight / 2;
  vertex[4][0] = tx - bodyWidth / 2;
  vertex[4][1] = ty + bodyHeight / 2;
  angle = 0.0f;
}

bool Arrow::hasCollidedWithWindow() {
  return (vertex[2][0] < 0 || vertex[2][0] > windowWidth || vertex[2][1] < 0 ||
          vertex[2][1] > windowHeight);
}

void Arrow::calculateAirbornPosition() {
  float vx = velocity * cos(angle);
  float vy = velocity * sin(angle);
  float g = -9.8f;
  float dt = 0.1f;
  float dx = vx * dt;
  float dy = vy * dt + 0.5 * g * dt * dt;
  translate(dx, dy);
  vy += g * dt;
  float old_angle = angle;
  float new_angle = atan(vy / vx);
  rotate(new_angle - old_angle);
  velocity = sqrt(vx * vx + vy * vy);
  if (hasCollidedWithWindow()) {
    printf("Arrow has collided with window\n");
    reset(100.f, 100.f, 400.f, 300.f);
    velocity = 0;
    this->airborn = false;
  } else if (ty < 0) {
    printf("Arrow has collided with ground\n");
    reset(100.f, 100.f, 400.f, 300.f);
    velocity = 0;
    this->airborn = false;
  }
}

void Arrow::setVelocity(float velocity) { this->velocity = velocity; }
void Arrow::setAngle(float angle) { this->angle = angle; }
void Arrow::setWindow(int windowWidth, int windowHeight) {
  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;
}

void Arrow::setAirborn(bool airborn, float initialVelocity) {
  this->airborn = airborn;
  if (airborn) {
    velocity = initialVelocity;
  }
}

bool Arrow::isAirborn() { return airborn; }
