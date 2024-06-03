#ifndef MENU_MAIN_PAGE_H
#define MENU_MAIN_PAGE_H

#include "ui.h"

class MenuMainPage {
public:
  MenuMainPage(int windowWidth, int windowHeight);
  bool startButtonClicked(int x, int y);
  bool quitButtonClicked(int x, int y);
  void display();

private:
  int windowWidth;
  int windowHeight;
  Button startButton;
  Button quitButton;
};

#endif
