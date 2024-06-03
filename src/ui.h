#ifndef UI_H
#define UI_H

#include <GL/freeglut.h>
#include <string>
class Button {
public:
  Button();
  Button(int x, int y, std::string text);
  void display();
  bool isClicked(int x, int y);

private:
  int x;
  int y;
  std::string text;
};

#endif
