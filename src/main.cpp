#include "arrow.h"
#include "bow.h"
#include "menu_game_over.h"
#include "menu_main_page.h"
#include "spring.h"
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <string>

const float PI = 3.14159265358979323846;

static bool isDragging = false;
static int mouseClickX, mouseClickY;
static int prevMouseX, prevMouseY;

static int windowWidth = 800;
static int windowHeight = 600;
static int targetX = windowWidth / 20;

static bool shooting = false;

const float cx = 350.0f;
const float cy = 350.0f;

Target target(windowWidth, windowHeight);
float arrowInitialAngle = (-45.0f * PI) / 180.0f;
Bow bow(cx, cy, arrowInitialAngle);
Arrow arrow(10.f, 200.f, cx, cy, bow.getAngle(), windowWidth, windowHeight,
            170.f);
Spring spring(0.1f, cx, cy);

enum class CurrentPage { MAIN_MENU, GAME, GAME_OVER };

CurrentPage currentPage = CurrentPage::MAIN_MENU;
MenuMainPage menuMainPage(windowWidth, windowHeight);
MenuGameOver menuGameOver(windowWidth, windowHeight);

// Display callback function
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  if (currentPage == CurrentPage::MAIN_MENU) {
    menuMainPage.display();
    glutSwapBuffers();
    return;
  }

  if (currentPage == CurrentPage::GAME_OVER) {
    menuGameOver.display();
    glutSwapBuffers();
    return;
  }

  if (arrow.isAirborn()) {
    if (arrow.hasCollidedWithWindow()) {
      menuGameOver.setFinalScore(target.getScore());
      arrow.reset(cx, cy, bow.getAngle());
      target.resetScore();
      target.reset(windowWidth, windowHeight);
      currentPage = CurrentPage::GAME_OVER;
      return;
    } else if (arrow.hasCollidedWith(target)) {
      target.reset(windowWidth, windowHeight);
      target.incrementScore();
      arrow.reset(cx, cy, bow.getAngle());
    } else {
      arrow.calculateAirbornPosition();
    }
  }

  bow.draw();
  target.draw();
  arrow.draw();
  spring.draw(bow.getFirstPoint(), arrow.getArrowTail(), bow.getLastPoint());

  std::string score = "Score: " + std::to_string(target.getScore());

  glTranslatef(50, windowHeight - 100, 0);
  glScalef(0.5, 0.5, 0.5);
  glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char *)score.c_str());

  glutSwapBuffers();
}

// Function to initialize OpenGL settings
void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
}

// Reshape callback function
void reshape(int w, int h) {
  windowWidth = w;
  windowHeight = h;

  // reset the target
  target.reset(windowWidth, windowHeight);
  arrow.setWindow(windowWidth, windowHeight);
  menuMainPage = MenuMainPage(windowWidth, windowHeight);
  menuGameOver = MenuGameOver(windowWidth, windowHeight);

  // Set viewport to cover the entire window
  glViewport(0, 0, w, h);

  // Calculate aspect ratio
  float aspectRatio = (float)w / (float)h;

  // Use an orthographic projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0, w, 0, h);

  glMatrixMode(GL_MODELVIEW);
}

// Idle callback function
void idle() { glutPostRedisplay(); }

void handleKeyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27: // Escape key
    exit(0);
    break;
  }
}

// Mouse callback function
void mouse(int button, int state, int x, int y) {
  if (currentPage == CurrentPage::MAIN_MENU) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
      int windowY = windowHeight - y;
      if (menuMainPage.startButtonClicked(x, windowY)) {
        currentPage = CurrentPage::GAME;
      } else if (menuMainPage.quitButtonClicked(x, windowY)) {
        exit(0);
      }
    }
    return;
  }

  if (currentPage == CurrentPage::GAME_OVER) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
      int windowY = windowHeight - y;
      if (menuGameOver.restartButtonClicked(x, windowY)) {
        target.resetScore();
        target.reset(windowWidth, windowHeight);
        arrow.reset(cx, cy, bow.getAngle());
        currentPage = CurrentPage::GAME;
      } else if (menuGameOver.quitButtonClicked(x, windowY)) {
        exit(0);
      }
    }
    return;
  }

  if (button == GLUT_LEFT_BUTTON) {
    isDragging = true;
    mouseClickX = x;
    mouseClickY = y;
    prevMouseX = x;
    prevMouseY = y;
  }

  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    if (isDragging) {
      float arrowMass = 0.12f;
      arrow.release(
          spring.getArrowInitialSpeed(arrowMass, arrow.getArrowTail()));
    }
    isDragging = false;
  }
}

// Mouse motion callback function
void motion(int x, int y) {
  if (isDragging) {
    if (arrow.isAirborn()) {
      return;
    }
    float angle = atan2(y - mouseClickY, mouseClickX - x);
    float pullingDistance =
        sqrt(pow(mouseClickX - x, 2) + pow(mouseClickY - y, 2));
    float deltaX = x - prevMouseX;
    float deltaY = prevMouseY - y;
    arrow.pull(deltaX, deltaY, angle, pullingDistance);
    bow.rotateAbsolute(angle);
    prevMouseX = x;
    prevMouseY = y;
  }
}

// Main function
int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("Bow and Arrow");

  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(handleKeyboard);

  glutMainLoop();
  return 0;
}
