#include <deque>
#include <iostream>
#include <raylib.h>
#include <stdlib.h>

using namespace std;

const int screenWidth = 600;
const int screenHeight = 600;
int rows = 40;
int cols = 40;

class Point {
public:
  Vector2 pos = {10, 10};

  void set_point() {
    pos.x = (float)(rand() % (rows - 2) + 1);
    pos.y = (float)(rand() % (cols - 2) + 1);
  }

  void draw_points() {
    int width = screenWidth / rows;
    int height = screenHeight / cols;
    Rectangle pt =
        Rectangle{pos.x * width, pos.y * height, (float)width, (float)height};
    DrawRectangleRounded(pt, 1.0, 4, WHITE);
  }

  Vector2 get_pos() { return pos; }
};

class Snake {
public:
  deque<Vector2> snake = {Vector2{4, 5}, Vector2{3, 5}, Vector2{2, 5}};
  Vector2 motion = Vector2{0, 0};
  string direction = "";

  void update_heading(string key, Vector2 x) {
    if (!direction.compare("")) {
      direction = key;
      motion = x;
    }
    if (!direction.compare("up") || !direction.compare("down")) {
      if (!key.compare("left") || (!key.compare("right"))) {
        direction = key;
        motion = x;
      }
    }
    if (!direction.compare("left") || !direction.compare("right")) {
      if (!key.compare("up") || (!key.compare("down"))) {
        direction = key;
        motion = x;
      }
    }
  }

  void update_pos() {
    if (direction.compare("")) {
      snake.pop_back();
      Vector2 head = snake[0];
      snake.push_front(Vector2{head.x + motion.x, head.y + motion.y});
    }
  }

  void draw_snake() {
    int width = screenWidth / rows;
    int height = screenHeight / cols;
    for (int i = 0; i < snake.size(); ++i) {
      Vector2 curr = snake[i];
      Rectangle seg = Rectangle{curr.x * width, curr.y * height, (float)width,
                                (float)height};
      DrawRectangleRounded(seg, 0.8, 4, RED);
    }
  }

  bool check_boundary() {
    int x = snake[0].x;
    int y = snake[0].y;
    if (x == 0 || x == rows - 1) {
      return true;
    }
    if (y == 0 || y == cols - 1) {
      return true;
    }
    return false;
  }

  bool check_collision() {
    Vector2 head = snake[0];
    for (int i = 1; i < snake.size(); ++i) {
      Vector2 curr = snake[i];
      if ((head.x == curr.x) && (head.y == curr.y)) {
        return true;
      }
    }
    return false;
  }

  bool consume(Vector2 pt) {
    Vector2 head = snake[0];
    if ((head.x == pt.x) && (head.y == pt.y)) {
      snake.push_back(snake[snake.size() - 1]);
      return true;
    }
    return false;
  }

  void port(Vector2 p1, Vector2 p2) {
    Vector2 head = snake[0];
    if (p1.x == head.x && p1.y == head.y) {
      snake[0] = p2;
    } else if (p2.x == head.x && p2.y == head.y) {
      snake[0] = p1;
    }
  }
};

class Background {
public:
  Color border_outline = BROWN;
  int tile_width = screenWidth / rows;
  int tile_height = screenHeight / cols;

  void draw_grid() {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        Rectangle x = Rectangle{(float)i * tile_width, (float)j * tile_height,
                                (float)tile_width, (float)tile_height};
        Color tile_color;
        if ((i == 0) || (j == 0) || (i == rows - 1) || (j == cols - 1)) {
          tile_color = BROWN;
        } else {
          tile_color = LIME;
        }
        DrawRectangleRounded(x, 0.6, 4, tile_color);
      }
    }
  }
};

class Portal {
public:
  Vector2 p1 = {10, 15};
  Vector2 p2 = {20, 20};

  Vector2 get_portA() { return p1; }

  Vector2 get_portB() { return p2; }

  void draw_portals() {
    int tile_width = screenWidth / rows;
    int tile_height = screenHeight / cols;
    Rectangle x = Rectangle{p1.x * tile_width, p1.y * tile_height,
                            (float)tile_width, (float)tile_height};
    DrawRectangleRounded(x, 1.0, 8, PURPLE);
    Rectangle y = Rectangle{p2.x * tile_width, p2.y * tile_height,
                            (float)tile_width, (float)tile_height};
    DrawRectangleRounded(y, 1.0, 8, BLUE);
  }
};

int main() {

  InitWindow(screenWidth, screenHeight, "Snakes...");
  SetTargetFPS(20);

  int state = 0; // Can be 0, 1, 2
  Point p = Point();
  Background grids = Background();
  Snake snake = Snake();
  Portal port = Portal();

  while (WindowShouldClose() == false) {
    if (state == 0) {
      BeginDrawing();
      ClearBackground(BLACK);
      if (IsKeyPressed(KEY_ENTER)) {
        state = 1;
      }
      EndDrawing();
    } else if (state == 1) {
      BeginDrawing();
      // Generating game layout
      ClearBackground(BLACK);
      grids.draw_grid();
      p.draw_points();
      port.draw_portals();
      snake.draw_snake();
      EndDrawing();

      // Updating for next frame
      snake.update_pos();
      snake.port(port.get_portA(), port.get_portB());

      // Checking trigger events
      if (snake.check_boundary() || snake.check_collision()) {
        state = 2;
      }

      if (snake.consume(p.get_pos())) {
        p.set_point();
      }

      // Handling keyboard input
      if (IsKeyPressed(KEY_UP)) {
        snake.update_heading("up", Vector2{0, -1});
      } else if (IsKeyPressed(KEY_DOWN)) {
        snake.update_heading("down", Vector2{0, 1});
      } else if (IsKeyPressed(KEY_LEFT)) {
        snake.update_heading("left", Vector2{-1, 0});
      } else if (IsKeyPressed(KEY_RIGHT)) {
        snake.update_heading("right", Vector2{1, 0});
      }
    } else if (state == 2) {
      p = Point();
      grids = Background();
      snake = Snake();
      state = 0;
    }
  }

  CloseWindow();
  return 0;
}