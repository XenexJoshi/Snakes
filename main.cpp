#include <deque>
#include <iostream>
#include <raylib.h>
#include <stdlib.h>

using namespace std;

const int gameWidth = 800;
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
    DrawRectangleRounded(pt, 1.0, 4, GOLD);
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
  Vector2 p1;
  Vector2 p2;

  void initialize_portals() {
    p1 = {(float)(rand() % (rows / 2 - 6) + 3),
          (float)(rand() % (cols - 6) + 3)};
    p2 = {(float)(rand() % (rows / 2 - 6) + 3 + (rows / 2)),
          (float)(rand() % (cols - 6) + 3)};
  }

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

class Score {
public:
  int high_score = 0;
  int score = 0;

  void increment() { score += 200; }

  string output_score() {
    string x = to_string(score);
    while (x.length() != 5) {
      x = ((string) "0").append(x);
    }
    return x;
  }

  string output_hi_score() {
    string x = to_string(high_score);
    while (x.length() != 5) {
      x = ((string) "0").append(x);
    }
    return x;
  }

  void update_score() {
    if (score > high_score) {
      high_score = score;
      score = 0;
    }
  }
};
int main() {

  InitWindow(gameWidth, screenHeight, "Snakes...");
  SetTargetFPS(60);

  int state = 0; // Can be 0, 1, 2
  Point p = Point();
  Background grids = Background();
  Snake snake = Snake();
  Portal port = Portal();
  Score sc = Score();
  port.initialize_portals();
  int state_counter = 0;

  while (WindowShouldClose() == false) {
    if (state == 0) {

      Color begin_color;
      Color quit_color;
      if (state_counter == 0) {
        begin_color = GREEN;
        quit_color = WHITE;
      } else {
        begin_color = WHITE;
        quit_color = GREEN;
      }

      BeginDrawing();
      ClearBackground(BLACK);
      DrawText("SNAKE", 3 * gameWidth / 10, screenHeight / 4, 60, RED);
      DrawText("Begin", 4 * gameWidth / 10, screenHeight / 2, 30, begin_color);
      DrawText("Quit", 4 * gameWidth / 10, 3 * screenHeight / 5, 30,
               quit_color);
      EndDrawing();

      if (IsKeyPressed(KEY_ENTER)) {
        if (state_counter == 0) {
          SetTargetFPS(20);
          state = 1;
        } else {
          CloseWindow();
        }
      }

      if (IsKeyPressed(KEY_DOWN) && state_counter == 0) {
        state_counter = 1;
      } else if (IsKeyPressed(KEY_UP) && state_counter == 1) {
        state_counter = 0;
      }
    } else if (state == 1) {
      BeginDrawing();
      // Generating game layout
      ClearBackground(BLACK);
      grids.draw_grid();
      p.draw_points();
      port.draw_portals();
      snake.draw_snake();
      string s = sc.output_score();
      char *ch_arr = new char[s.length() + 1];
      strcpy(ch_arr, s.c_str());
      DrawText("SCORE: ", 620, 160, 40, WHITE);
      DrawText(ch_arr, 630, 200, 40, WHITE);
      string x = sc.output_hi_score();
      char *ch_ar = new char[x.length() + 1];
      strcpy(ch_ar, x.c_str());
      DrawText("HSCORE: ", 620, 400, 40, WHITE);
      DrawText(ch_ar, 630, 440, 40, WHITE);
      EndDrawing();

      // Updating for next frame
      snake.update_pos();
      snake.port(port.get_portA(), port.get_portB());

      // Checking trigger events
      if (snake.check_boundary() || snake.check_collision()) {
        state = 2;
      }

      if (snake.consume(p.get_pos())) {
        sc.increment();
        p.set_point();
        Vector2 updated_pos = p.get_pos();
        // Ensuring that new point is not generated atop portals
        while ((updated_pos.x == port.get_portA().x &&
                updated_pos.y == port.get_portA().y) ||
               (updated_pos.x == port.get_portB().x &&
                updated_pos.y == port.get_portB().y)) {
          p.set_point();
          updated_pos = p.get_pos();
        }
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
      p.set_point();
      snake = Snake();
      port.initialize_portals();
      sc.update_score();
      state_counter = 0;
      state = 0;
    }
  }

  CloseWindow();
  return 0;
}