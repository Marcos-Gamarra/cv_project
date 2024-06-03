#ifndef TARGET_H
#define TARGET_H

class Target {
public:
  Target(int windowWidth, int windowHeight);
  void reset(int windowWidth, int windowHeight);
  void draw();
  bool checkCollision(int vertex_x, int vertex_y);
  void resetScore();
  void incrementScore();
  int getScore();

private:
  int score;
  int windowWidth, windowHeight;
  int width, height;
  int x, y; // bottom-left corner
};

#endif
