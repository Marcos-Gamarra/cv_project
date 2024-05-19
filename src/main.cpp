#include "arrow.h"
#include "bow.h"
#include "spring.h"
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>

// Define the rotation angle and translation offsets
static float angle = 0.0f;
static float tx = 0.0f, ty = 0.0f, tz = 0.0f;

// Define a flag to control the rotation and drag
static bool isMoving = false;
static bool isDragging = false;
static int lastMouseX, lastMouseY;

static int windowWidth = 800;
static int windowHeight = 600;
static int targetX = windowWidth / 20;

static bool shooting = false;

GLuint texture;

const float cx = 400.0f;
const float cy = 300.0f;

Target target(windowWidth, windowHeight);
Bow bow(cx, cy);
Arrow arrow(10.f, 200.f, cx, cy, bow.getAngle(), windowWidth, windowHeight);
Spring spring(0.1f, cx, cy);

void resetArrow() {
  isMoving = false;
  arrow.reset(cx, cy, bow.getAngle());
}

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
      arrow.setAirborn(false, 0.0f);
      arrow.reset(cx, cy, bow.getAngle());
      target.resetScore();
      target.reset(windowWidth, windowHeight);
    } else if (arrow.hasCollidedWith(target)) {
      target.reset(windowWidth, windowHeight);
      arrow.setAirborn(false, 0.0f);
      target.incrementScore();
      arrow.reset(cx, cy, bow.getAngle());
    } else {
      arrow.calculateAirbornPosition();
    }
  }

  bow.draw();
  target.draw();
  arrow.draw();
  spring.draw(bow.getFirstPoint(), arrow.getCentralPoint(), bow.getLastPoint());

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
void idle() {
  // Update the rotation angle if rotating

  // Request to redraw the scene
  glutPostRedisplay();
}

void handleSpecialKeys(int key, int x, int y) {
  if (arrow.isAirborn()) {
    return;
  }
  switch (key) {
  case GLUT_KEY_DOWN:
    arrow.rotate(0.1f);
    bow.rotate(0.1f);
    break;
  case GLUT_KEY_UP:
    bow.rotate(-0.1f);
    arrow.rotate(-0.1f);
    break;
  case GLUT_KEY_LEFT:
    arrow.pull(10.f);
    break;
  case GLUT_KEY_RIGHT: {
    float mass = 0.1f;
    float arrowPoint[2] = {arrow.getCentralPoint()[0],
                           arrow.getCentralPoint()[1]};
    arrow.setAirborn(true, spring.getArrowInitialSpeed(mass, arrowPoint));
    break;
  }

    // Request to redraw the scene with updated parameters
    glutPostRedisplay();
  }
}

void handleKeyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27: // Escape key
    exit(0);
    break;
  }
  glutPostRedisplay();
}

// Mouse callback function
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    isMoving = true; // Start rotation on left click
  }
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    isMoving = false; // Stop rotation on right click

    // Request to redraw the scene with updated parameters
    glutPostRedisplay();
  }
}
// Mouse motion callback function
void motion(int x, int y) {
  if (isDragging) {
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    tx += dx * 0.01f;
    ty -= dy * 0.01f; // Reverse y-axis direction for intuitive drag
    lastMouseX = x;
    lastMouseY = y;

    // Request to redraw the scene with updated parameters
    glutPostRedisplay();
  }
}

// Main function
int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("");

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
