#include <raylib.h>

// Global Scores
int player_score = 0;
int cpu_score    = 0;

class Ball {
public:
    int speed_x;
    int speed_y;
    int x, y;
    int radius;

    void draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }

        if (x + radius >= GetScreenWidth()) {
            cpu_score++;
            ResetBall();
        }
        if (x - radius <= 0) {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choice[2] = {-1, 1};
        speed_x = 7 * speed_choice[GetRandomValue(0, 1)];
        speed_y = 7 * speed_choice[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height, speed;

    void draw() {
        DrawRectangle((int)x, (int)y, (int)width, (int)height, WHITE);
    }

    void update() {
        if (IsKeyDown(KEY_UP)) {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y += speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle {
public:
    void update(int ball_y) {
        if (y + height / 2 > ball_y) {
            y -= speed;
        } else {
            y += speed;
        }
        LimitMovement();
    }
};

// Game Objects
Ball       ball;
Paddle     player;
CpuPaddle  cpu;

int main() {
    const int screen_width  = 1280;
    const int screen_height = 800;

    InitWindow(screen_width, screen_height, "My pong Game!");
    SetTargetFPS(60);

    ball.radius  = 20;
    ball.x       = screen_width / 2;
    ball.y       = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width  = 25;
    player.height = 120;
    player.x      = screen_width - player.width - 10;
    player.y      = screen_height / 2 - player.height / 2;
    player.speed  = 12;

    cpu.width  = 25;
    cpu.height = 120;
    cpu.x      = 10;
    cpu.y      = screen_height / 2 - player.height / 2;
    cpu.speed  = 6;

    TraceLog(LOG_INFO, "Starting the game...");

    while (!WindowShouldClose()) {
        // Update
        ball.update();
        player.update();
        cpu.update(ball.y);

        // Collision Detection
        if (CheckCollisionCircleRec(Vector2{(float)ball.x, (float)ball.y}, (float)ball.radius,
                                    Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{(float)ball.x, (float)ball.y}, (float)ball.radius,
                                    Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
        }

        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        ball.draw();
        player.draw();
        cpu.draw();

        DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


/*
    In my raylib setup, you can build this code using 
        - g++ -std=c++23 -I "C:\raylib\w64devkit\x86_64-w64-mingw32\include" -L "C:\raylib\w64devkit\x86_64-w64-mingw32\lib" Raylib-CPP-Starter-Template-for-VSCODE-main\main.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -o main
    And after that, you can run by executing the main.exe by
        - .\main.exe
    
    This varies according to your setup. Thank You!
*/