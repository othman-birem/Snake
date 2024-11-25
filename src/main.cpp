#include "raylib.h"
#include "resource_dir.h"
#include <iostream>
#include <deque>
#include <raymath.h>

using namespace std;

Color Blue = {167, 216, 240, 255};
Color DarkBlue = {0, 95, 115, 255};

int cellSize = 25;
int cellCount = 20;

double last_update_time = 0;
bool IsUpdateRequired(double interval = 0.2){
    double current_time = GetTime();
    if(current_time - last_update_time >= interval) {
        last_update_time = current_time;
        return true;
    }
    else return false;
}
bool ElementInDeque(Vector2 element, deque<Vector2> deque){
    for(unsigned int i = 0; i < deque.size(); i++){
        if(Vector2Equals(deque[i], element)){
            return true;
        }
    }
    return false;
}

class Snake{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = Vector2{1, 0};
    bool IsGrow = false;

    void Draw(){
        for(unsigned int i = 0; i < body.size(); i++){
            float x = body[i].x;
            float y = body[i].y;

            Rectangle segment = Rectangle{x*cellSize, y*cellSize, (float)cellSize, (float)cellSize};

            DrawRectangleRounded(segment, 0.8, 6, DarkBlue);
        } 
    }
    void Update(){
        if (IsGrow) {
        body.push_back(Vector2Subtract(body[body.size() - 1],
                      Vector2Subtract(body[body.size() - 1], body[body.size() - 2])));
        IsGrow = false;
        }
        body.pop_back();
        Vector2 next_position = Vector2Add(body[0], direction);

        if(next_position.x >= cellCount) next_position.x = 0;
        if(next_position.x < 0) next_position.x = cellCount - 1;
        if(next_position.y >= cellCount) next_position.y = 1;
        if(next_position.y < 1) next_position.y = cellCount - 1;

        body.push_front(next_position);
    }
};

class Food{
public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snake_body){
        Image img = LoadImage("resources/apple.png");
        texture = LoadTextureFromImage(img);
        UnloadImage(img);
        position = GeneratePosition(snake_body);
    }
    ~Food(){
        UnloadTexture(texture);
    }

    void Draw(){
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
    }
    Vector2 GenerateRandomCell(){
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }
    Vector2 GeneratePosition(deque<Vector2> snake_body){
        Vector2 new_position = GenerateRandomCell();
        while(ElementInDeque(new_position, snake_body) || new_position.y == 0){
            new_position = GenerateRandomCell();
        }
        return new_position;
    }
};

class Game{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);

    void Draw(){
        food.Draw();
        snake.Draw();
    }
    void Update(){
        snake.Update();
    }
    void CheckCollisionWithFood(){
        if(Vector2Equals(snake.body[0], food.position)){
            food.position = food.GeneratePosition(snake.body);
            snake.IsGrow = true;
        }
    }
};

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	const int SCREEN_WIDTH = cellSize * cellCount;
    const int TOPBAR_HEIGHT = 25;
    const int SCREEN_HEIGHT = (cellSize * cellCount) + TOPBAR_HEIGHT;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(60);

    Game game = Game();

    while (WindowShouldClose() == false){
        BeginDrawing();
            ClearBackground(Blue);
            DrawLine(0, TOPBAR_HEIGHT, SCREEN_WIDTH, TOPBAR_HEIGHT, GRAY);
            DrawText(TextFormat("Score: %d", game.snake.body.size()), 10, 5, 18, DarkBlue);
            DrawText("Game Area", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 20, Blue);

            if(IsUpdateRequired()) game.Update();
            game.CheckCollisionWithFood();

            if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) game.snake.direction = Vector2{0, -1};
            if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) game.snake.direction = Vector2{0, 1};
            if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) game.snake.direction = Vector2{1, 0};
            if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) game.snake.direction = Vector2{-1, 0};

            game.Draw();
        EndDrawing();
    }

    CloseWindow();
	return 0;
}
