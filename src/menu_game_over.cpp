#include "menu_game_over.h"
#include "ui.h"
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <string>

MenuGameOver::MenuGameOver(int windowWidth, int windowHeight) {
  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;
  this->restartButton =
      Button(windowWidth / 2, windowHeight / 2 + 80, "Restart");

  this->quitButton = Button(windowWidth / 2, windowHeight / 2 - 80, "Quit");
}

void MenuGameOver::display() {
  Button finalScoreButton(windowWidth / 2, windowHeight / 2 + 400,
                          "Final Score: " + std::to_string(finalScore));

  finalScoreButton.display();
  restartButton.display();
  quitButton.display();
}

bool MenuGameOver::restartButtonClicked(int x, int y) {
  return restartButton.isClicked(x, y);
}

bool MenuGameOver::quitButtonClicked(int x, int y) {
  return quitButton.isClicked(x, y);
}

void MenuGameOver::setFinalScore(int finalScore) {
  this->finalScore = finalScore;
}
