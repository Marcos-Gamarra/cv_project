#include "target.h"
#include <GL/glut.h>
#include <random>

Target::Target(int windowWidth, int windowHeight) {
  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;
  this->score = 0;
  reset(windowWidth, windowHeight);
}

void Target::reset(int windowWidth, int windowHeight) {
  std::random_device rd;
  std::mt19937 gen(rd());
  this->height = windowHeight / (score + 2);
  std::uniform_int_distribution<int> vertex_dis(0, windowHeight - height);
  this->y = vertex_dis(gen);
  this->width = 50;
  this->x = windowWidth - width;
}

void Target::draw() {
  glBegin(GL_QUADS);
  glVertex2f(x, y);
  glVertex2f(x + width, y);
  glVertex2f(x + width, y + height);
  glVertex2f(x, y + height);
  glEnd();
}

bool Target::checkCollision(int vertex_x, int vertex_y) {
  return (vertex_x >= this->x && vertex_x <= (this->x + this->width) &&
          vertex_y >= this->y && (vertex_y <= this->y + this->height));
}

void Target::resetScore() { this->score = 0; }
void Target::incrementScore() { this->score++; }
int Target::getScore() { return this->score; }
