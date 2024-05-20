#include "arrow.h"
#include "target.h"
#include <GL/freeglut.h>
#include <math.h>

const float PI = 3.14159265358979323846;

Arrow::Arrow(float bodyWidth, float bodyHeight, float cx, float cy,
             float initialAngle, int windowWidth, int windowHeight,
             float maxPullDistance) {
  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;
  this->arrowWidth = bodyWidth;
  this->arrowLength = bodyHeight;
  this->cx = cx;
  this->cy = cy;
  this->speed = 0.0f;
  this->airborn = false;
  this->maxPullDistance = maxPullDistance;
  reset(cx, cy, initialAngle);
}

void Arrow::draw() {
  drawBody();
  drawHead();
}

void Arrow::translateRelative(float x, float y) {
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

void Arrow::rotateRelative(float angle) {
  this->angle += angle;
  float *tail = getArrowTail();

  for (int i = 0; i < 5; i++) {
    float x = vertex[i][0] - tail[0];
    float y = vertex[i][1] - tail[1];
    vertex[i][0] = x * cos(angle) - y * sin(angle) + tail[0];
    vertex[i][1] = x * sin(angle) + y * cos(angle) + tail[1];
  }
}

void Arrow::drawHead() {
  glBegin(GL_TRIANGLES);
  glVertex2f(vertex[1][0], vertex[1][1]);
  glVertex2f(vertex[2][0], vertex[2][1]);
  glVertex2f(vertex[4][0], vertex[4][1]);
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
  translateRelative(x, y);
  rotateRelative(angle);
}

void Arrow::moveForward(float distance) {
  float x = distance * cos(angle);
  float y = distance * sin(angle);

  translateRelative(x, y);
}

bool Arrow::hasCollidedWith(Target target) {
  return target.checkCollision(vertex[2][0], vertex[2][1]);
}

void Arrow::reset(float x, float y, float angle) {
  vertex[0][0] = cx;
  vertex[0][1] = cy - arrowWidth / 2;
  vertex[1][0] = cx + arrowLength;
  vertex[1][1] = cy - arrowWidth / 2;
  vertex[2][0] = cx + arrowLength;
  vertex[2][1] = cy + arrowWidth / 2;
  vertex[3][0] = cx;
  vertex[3][1] = cy + arrowWidth / 2;
  vertex[4][0] = cx + arrowLength + arrowWidth;
  vertex[4][1] = cy;
  this->angle = 0.0;
  rotateRelative(angle);
  speed = 0.0f;
  airborn = false;
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
  translateRelative(dx, dy);
  vy += g * dt;
  float old_angle = angle;
  float new_angle = atan2(vy, vx);
  float relativeAngle = new_angle - old_angle;

  this->angle += relativeAngle;
  float *centralPoint = getCentralPoint();

  for (int i = 0; i < 5; i++) {
    float x = vertex[i][0] - centralPoint[0];
    float y = vertex[i][1] - centralPoint[1];
    vertex[i][0] =
        x * cos(relativeAngle) - y * sin(relativeAngle) + centralPoint[0];
    vertex[i][1] =
        x * sin(relativeAngle) + y * cos(relativeAngle) + centralPoint[1];
  }

  speed = sqrt(vx * vx + vy * vy);
}

void Arrow::setSpeed(float speed) { this->speed = speed; }
void Arrow::setWindow(int windowWidth, int windowHeight) {
  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;
}

void Arrow::release(float initialSpeed) {
  airborn = true;
  if (airborn) {
    speed = initialSpeed;
  }
}

bool Arrow::isAirborn() { return airborn; }

float *Arrow::getCentralPoint() {
  static float centralPoint[2];
  centralPoint[0] = (vertex[0][0] + vertex[2][0]) / 2;
  centralPoint[1] = (vertex[0][1] + vertex[2][1]) / 2;
  return centralPoint;
}

float *Arrow::getArrowTail() {
  static float centralPoint[2];
  centralPoint[0] = (vertex[0][0] + vertex[3][0]) / 2;
  centralPoint[1] = (vertex[0][1] + vertex[3][1]) / 2;
  return centralPoint;
}

void Arrow::pull(float deltaX, float deltaY, float pullingAngle,
                 float pullingDistance) {
  if (pullingDistance > maxPullDistance) {
    float relativeAngle = pullingAngle - angle;
    this->angle += relativeAngle;

    for (int i = 0; i < 5; i++) {
      float x = vertex[i][0] - cx;
      float y = vertex[i][1] - cy;
      vertex[i][0] = x * cos(relativeAngle) - y * sin(relativeAngle) + cx;
      vertex[i][1] = x * sin(relativeAngle) + y * cos(relativeAngle) + cy;
    }
  } else {
    translateRelative(deltaX, deltaY);
    rotateAbsolute(pullingAngle);
  }
}

void Arrow::rotateAbsolute(float angle) { rotateRelative(angle - this->angle); }
