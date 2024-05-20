#include "arrow.h"
#include "bow.h"
#include "spring.h"
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>

const float PI = 3.14159265358979323846;

static bool isDragging = false;
static int mouseClickX, mouseClickY;
static int prevMouseX, prevMouseY;

static int windowWidth = 800;
static int windowHeight = 600;
static int targetX = windowWidth / 20;

static bool shooting = false;

GLuint texture;

const float cx = 400.0f;
const float cy = 300.0f;

Target target(windowWidth, windowHeight);
float arrowInitialAngle = (-45.0f * PI) / 180.0f;
Bow bow(cx, cy, arrowInitialAngle);
Arrow arrow(10.f, 200.f, cx, cy, bow.getAngle(), windowWidth, windowHeight,
            170.f);
Spring spring(0.1f, cx, cy);

void loadTexture(const char *filename) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  int width, height;
  unsigned char *image =
      SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
  if (!image) {
    printf("Failed to load texture: %s\n", SOIL_last_result());
    return;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// Display callback function
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  if (arrow.isAirborn()) {
    if (arrow.hasCollidedWithWindow()) {
      arrow.reset(cx, cy, bow.getAngle());
      target.resetScore();
      target.reset(windowWidth, windowHeight);
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

  glutSwapBuffers();
}

// Function to initialize OpenGL settings
void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  loadTexture("./textures/test_texture.jpg");
}

// Reshape callback function
void reshape(int w, int h) {
  windowWidth = w;
  windowHeight = h;

  // reset the target
  target.reset(windowWidth, windowHeight);
  arrow.setWindow(windowWidth, windowHeight);

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

void handleSpecialKeys(int key, int x, int y) {
  if (arrow.isAirborn()) {
    return;
  }
  switch (key) {
  case GLUT_KEY_DOWN:
    break;
  case GLUT_KEY_UP:
    bow.rotateRelative(-0.1f);
    break;
  case GLUT_KEY_LEFT:
    break;
  case GLUT_KEY_RIGHT: {
    break;
  }
  }
}

void handleKeyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27: // Escape key
    exit(0);
    break;
  }
}

// Mouse callback function
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    isDragging = true;
    mouseClickX = x;
    mouseClickY = y;
    prevMouseX = x;
    prevMouseY = y;
  }

  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    if (isDragging) {
      float arrowMass = 0.1f;
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

  glutSpecialFunc(handleSpecialKeys);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(handleKeyboard);

  glutMainLoop();
  return 0;
}
