#ifndef MENU_GAME_OVER_H
#define MENU_GAME_OVER_H
#include "ui.h"

class MenuGameOver {
public:
  MenuGameOver(int windowWidth, int windowHeight);
  bool restartButtonClicked(int x, int y);
  bool quitButtonClicked(int x, int y);
  void display();
  void setFinalScore(int finalScore);

private:
  int windowWidth;
  int windowHeight;
  int finalScore;
  Button restartButton;
  Button quitButton;
};

#endif
