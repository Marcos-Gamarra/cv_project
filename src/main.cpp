#include "arrow.h"
#include "bow.h"
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

Arrow arrow(10.f, 100.f, 400.f, 300.f, windowWidth, windowHeight);
Target target(windowWidth, windowHeight);
Bow bow(400.f, 300.f);

void resetArrow() {
  isMoving = false;
  arrow.reset(400.f, 300.f);
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

  /* if (isMoving) {
    arrow.moveForward(10.0f);
    if (arrow.hasCollidedWithWindow()) {
      printf("Arrow has collided with window\n");
      resetArrow();
    } else if (arrow.hasCollidedWith(target)) {
      printf("Arrow has collided with target\n");
      target.reset(windowWidth, windowHeight);
      resetArrow();
    }
  } else if (shooting) {
    arrow.calculateAirbornPosition();
  } */
  if (arrow.isAirborn()) {
    arrow.calculateAirbornPosition();
  }

  bow.draw();
  target.draw();
  arrow.draw();

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

// Keyboard callback function
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 't': // Move up
    arrow.translate(0.0f, 0.1f);
    break;
  case 'n': // Move down
    arrow.translate(0.0f, -0.1f);
    break;
  case 's': // Move left
    arrow.translate(-0.1f, 0.0f);
    break;
  case 'r': // Move right
    arrow.translate(0.1f, 0.0f);
    break;

  case 'l': // Rotate clockwise
    arrow.rotate(0.1f);
    bow.rotate(0.1f);
    break;
  case 'p': // Rotate counter-clockwise
    bow.rotate(-0.1f);
    arrow.rotate(-0.1f);
    break;

  case 'w':
    target.reset(windowWidth, windowHeight);
    break;

  case ' ': // Space key
    arrow.setAirborn(true, 100.0f);
    break;
  case 27: // Escape key
    exit(0);
    break;
  }

  // Request to redraw the scene with updated parameters
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

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);         // Register the idle function
  glutKeyboardFunc(keyboard); // Register the keyboard function
  glutMouseFunc(mouse);       // Register the mouse function
  glutMotionFunc(motion);     // Register the motion function for drag

  glutMainLoop();
  return 0;
}
