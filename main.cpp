#include <iostream>
#include <raylib.h>
#include <stdlib.h>
#include <deque>

using namespace std;

const int screenWidth = 600;
const int screenHeight = 600;
int rows = 40;
int cols = 40;

class Point
{
public:
    Vector2 pos = {10, 10};

    void set_point()
    {
        pos.x = (float)(rand() % (rows - 1) + 1);
        pos.y = (float)(rand() % (cols - 1) + 1);
    }

    void draw_points()
    {
        int width = screenWidth / rows;
        int height = screenHeight / cols;
        Rectangle pt = Rectangle{pos.x * width, pos.y * height, (float)width, (float)height};
        DrawRectangleRounded(pt, 1.0, 4, WHITE);
    }

    Vector2 get_pos()
    {
        return pos;
    }
};

class Snake
{
public:
    deque<Vector2> snake = {Vector2{4, 5}, Vector2{3, 5}, Vector2{2, 5}};
    Vector2 motion = Vector2{0, 0};
    string direction = "";

    void update_heading(string key, Vector2 x)
    {
        if (!direction.compare(""))
        {
            direction = key;
            motion = x;
        }
        if (!direction.compare("up") || !direction.compare("down"))
        {
            if (!key.compare("left") || (!key.compare("right")))
            {
                direction = key;
                motion = x;
            }
        }
        if (!direction.compare("left") || !direction.compare("right"))
        {
            if (!key.compare("up") || (!key.compare("down")))
            {
                direction = key;
                motion = x;
            }
        }
    }

    void update_pos()
    {
        if (direction.compare(""))
        {
            snake.pop_back();
            Vector2 head = snake[0];
            snake.push_front(Vector2{head.x + motion.x, head.y + motion.y});
        }
    }

    void draw_snake()
    {
        int width = screenWidth / rows;
        int height = screenHeight / cols;
        for (int i = 0; i < snake.size(); ++i)
        {
            Vector2 curr = snake[i];
            Rectangle seg = Rectangle{curr.x * width, curr.y * height, (float)width, (float)height};
            DrawRectangleRounded(seg, 0.8, 4, RED);
        }
    }

    bool check_boundary()
    {
        int x = snake[0].x;
        int y = snake[0].y;
        if (x == 0 || x == rows - 1)
        {
            return true;
        }
        if (y == 0 || y == cols - 1)
        {
            return true;
        }
        return false;
    }

    bool check_collision()
    {
        Vector2 head = snake[0];
        for (int i = 1; i < snake.size(); ++i)
        {
            Vector2 curr = snake[i];
            if ((head.x == curr.x) && (head.y == curr.y))
            {
                return true;
            }
        }
        return false;
    }

    bool consume(Vector2 pt)
    {
        Vector2 head = snake[0];
        if ((head.x == pt.x) && (head.y == pt.y))
        {
            snake.push_back(snake[snake.size() - 1]);
            return true;
        }
        return false;
    }
};

class Background
{
public:
    Color border_outline = BROWN;
    int tile_width = screenWidth / rows;
    int tile_height = screenHeight / cols;

    void draw_grid()
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                Rectangle x = Rectangle{(float)i * tile_width, (float)j * tile_height, (float)tile_width, (float)tile_height};
                Color tile_color;
                if ((i == 0) || (j == 0) || (i == rows - 1) || (j == cols - 1))
                {
                    tile_color = BROWN;
                }
                else
                {
                    tile_color = LIME;
                }
                DrawRectangleRounded(x, 0.6, 4, tile_color);
            }
        }
    }
};

int main()
{

    cout << "Hello World" << endl;

    InitWindow(screenWidth, screenHeight, "Snakes...");
    SetTargetFPS(20);

    Point p = Point();
    Background grids = Background();
    Snake snake = Snake();
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        grids.draw_grid();
        p.draw_points();
        snake.draw_snake();
        snake.update_pos();
        EndDrawing();

        if (snake.consume(p.get_pos()))
        {
            p.set_point();
        }
        if (snake.check_boundary() || snake.check_collision())
        {
            break;
        }
        if (IsKeyPressed(KEY_UP))
        {
            snake.update_heading("up", Vector2{0, -1});
        }
        else if (IsKeyPressed(KEY_DOWN))
        {
            snake.update_heading("down", Vector2{0, 1});
        }
        else if (IsKeyPressed(KEY_LEFT))
        {
            snake.update_heading("left", Vector2{-1, 0});
        }
        else if (IsKeyPressed(KEY_RIGHT))
        {
            snake.update_heading("right", Vector2{1, 0});
        }
    }

    CloseWindow();
    return 0;
}