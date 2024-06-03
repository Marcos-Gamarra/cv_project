#include "menu_main_page.h"
#include "ui.h"
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <string>

MenuMainPage::MenuMainPage(int windowWidth, int windowHeight) {
  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;
  this->startButton = Button(windowWidth / 2, windowHeight / 2 + 80, "Start");
  this->quitButton = Button(windowWidth / 2, windowHeight / 2 - 80, "Quit");
}

void MenuMainPage::display() {
  startButton.display();
  quitButton.display();
}

bool MenuMainPage::startButtonClicked(int x, int y) {
  return startButton.isClicked(x, y);
}

bool MenuMainPage::quitButtonClicked(int x, int y) {
  return quitButton.isClicked(x, y);
}
