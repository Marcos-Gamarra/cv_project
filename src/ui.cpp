#include "ui.h"
#include <GL/gl.h>

std::pair<float, float> getStrokeStringSize(const std::string &str,
                                            void *font) {
  float width = 0.0f;

  for (char c : str) {
    width += glutStrokeWidth(font, c);
  }

  // The height of the stroke font
  float height = 119.05f;

  return std::make_pair(width, height);
}

Button::Button() {}

Button::Button(int x, int y, std::string text) {
  this->x = x;
  this->y = y;
  this->text = text;
}

void Button::display() {
  std::pair<float, float> size = getStrokeStringSize(text, GLUT_STROKE_ROMAN);
  glPushMatrix();

  glTranslatef(x - size.first / 2, y - size.second / 2, 0);
  glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char *)text.c_str());

  glPopMatrix();
}

bool Button::isClicked(int x, int y) {
  std::pair<float, float> size = getStrokeStringSize(text, GLUT_STROKE_ROMAN);

  return x >= this->x - size.first / 2 && x <= this->x + size.first / 2 &&
         y >= this->y - size.second / 2 && y <= this->y + size.second / 2;
}
